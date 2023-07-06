using JuMP
import HiGHS
using  CSV
using DataFrames
using DelimitedFiles



struct Vertex
    id::Int
    color::String
end

mutable struct Graph
    numberOfVertex::Int
    numberOfEdges::Int
    C::Matrix{Int}
    edges::Dict{Vertex, Vector{Vertex}}

    function Graph(x, y)
        p = new(x, y, zeros(Int, x, x), Dict{Vertex, Vector{Vertex}}())
        return p
    end
end

function addEdge(graph::Graph, u::Vertex, v::Vertex, c::Int)
    if !haskey(graph.edges, u)
        graph.edges[u] = Vertex[]
    end
    push!(graph.edges[u], v)
    graph.C[u.id, v.id] = c
end




function main(args)

    data = readdlm("zad2.csv", '\0', String, '\n', header=false)
    numberOfVertexes = parse(Int64, data[1, 1])
    numberOfVertexesInSet = div(numberOfVertexes - 2,2)
    # Creating Graph
    graph = Graph(numberOfVertexes, numberOfVertexes)
    red_vertexes = Vector{Vertex}(undef, numberOfVertexesInSet+1)
    black_vertexes = Vector{Vertex}(undef, numberOfVertexesInSet+1)

    # Initializing black vertexes
    for i = 2:numberOfVertexesInSet+1
        u = Vertex(i, "Black")
        black_vertexes[i-1] = u
    end

    #Initializing red vertexes
    for i = numberOfVertexesInSet+2:2*numberOfVertexesInSet+1
        u = Vertex(i, "Red")
        red_vertexes[i-numberOfVertexesInSet-1] = u
    end

    #Initializing start and end vertexes
    start_vertex = Vertex(1,"Black")
    black_vertexes[numberOfVertexesInSet+1] = start_vertex

    end_vertex = Vertex(2*numberOfVertexesInSet+2,"Red")
    red_vertexes[numberOfVertexesInSet+1] = end_vertex

          


    #Initializing edges
    for v in red_vertexes
        for i = 1:numberOfVertexes
            row = string(data[v.id+1,1])
            if row[i]=='1'
                u = Vertex(i,"Black")
                addEdge(graph,v,u,1)
            end
        end
    end



    
    for v in black_vertexes
        for i=1:numberOfVertexes
            row = string(data[v.id+1,1])
            if row[i]=='1'
                u = Vertex(i,"Red")
                addEdge(graph,v,u,1)
            end
        end
    end 



    
    # Define optimization model
    model = Model(HiGHS.Optimizer)
    #Initializing flows
    @variable(model, F[1:2*numberOfVertexesInSet+2, 1:2*numberOfVertexesInSet+2]>=0)
    @variable(model, fmax >= 0)

    # Add flow conservation constraints
    for i = 1:2*numberOfVertexesInSet+2

        @constraint(model, sum(F[j, i] for j = 1:2*numberOfVertexesInSet+2) - sum(F[i, j] for j = 1:2*numberOfVertexesInSet+2) ==
            if i == 1
                -fmax
            elseif i == 2*numberOfVertexesInSet+2
                fmax
            else
                0
            end)
    end

    #capacity constraints
    for i = 1:2*numberOfVertexesInSet+2
        for j= 1:2*numberOfVertexesInSet+2
            @constraint(model, F[i, j] <= graph.C[i, j])
        end
    end

    
    @objective(model, Max, fmax)

    optimize!(model)


    println("Maximum Flow: $((objective_value(model)))")

    println("Optimized F:")
    for i in 1:(2 * numberOfVertexesInSet + 2)
        for j in 1:(2 * numberOfVertexesInSet + 2)
            if value(F[i,j])>0
                println("$i $j  $(value(F[i, j]))")
            end    
        end
    end

    return 0
end

# Run the main function with command line arguments
main(ARGS)