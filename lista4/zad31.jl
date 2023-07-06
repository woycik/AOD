using JuMP
import HiGHS

timeSum = 0
vertexes = []

struct Vertex
    id::Int
    e::String
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
    graph.C[u.id+1, v.id+1] = c
end



function binaryToDecimal(binary::Int)
    decimal = 0
    base = 1

    while binary > 0
        lastDigit = binary % 10
        decimal += lastDigit * base
        binary ÷= 10
        base *= 2
    end

    return decimal
end

function getVertexes(labelString::String, size::Int)
    len = length(labelString)
    modifiedLabel = Vector{Char}(labelString)
    v = Vertex[]

    for i = len:-1:len-size+1
        if labelString[i] == '0'
            modifiedLabel[i] = '1'
            labelNumber = parse(Int, String(modifiedLabel))
            binaryNumber = bitstring(labelNumber)
            number = binaryToDecimal(parse(Int, String(modifiedLabel)))
            push!(v, Vertex(number, String(modifiedLabel)))
            modifiedLabel[i] = '0'  # przywrócenie pierwotnej wartości znaku
        end
    end

    return v
end









function H(label::String)
    count = 0
    for c in label
        if c == '1'
            count += 1
        end
    end
    return count
end

function Z(label::String)
    count = 0
    for c in label
        if c == '0'
            count += 1
        end
    end
    return count
end

function main(args)
    if length(args) < 2
        println("Invalid number of arguments")
        return 1
    end

    size = 0
    printflow = false

    for i = 1:2:length(args)
        option = args[i]
        value = args[i+1]

        if option == "--size"
            size = parse(Int, value)
        elseif option == "--printflow"
            printflow = true
        else
            println("Unknown option: ", option)
            return 1
        end
    end

    numberOfVertexes = 2^size

    # Creating Graph
    graph = Graph(numberOfVertexes, size * 2^(size-1))
    vertexes = Vector{Vertex}(undef, graph.numberOfVertex)

    # Initializing vertexes and edges
    for i = 1:numberOfVertexes
        binaryNumber = bitstring(i-1)[64-size+1:64]
        u = Vertex(i-1, binaryNumber)
        vertexes[i] = u
    end

          



    for i = 1:numberOfVertexes
        currentLabel = vertexes[i].e
        sameLabels = getVertexes(currentLabel, size)
        println("Current label ",currentLabel)
        println(sameLabels)
        for v in sameLabels
            max_value = max(max(H(v.e), H(currentLabel)), max(Z(v.e), Z(currentLabel)))
            randomCappasity = rand(0:2^max_value) # Randomly choose the capacity value
            addEdge(graph, vertexes[i], v, randomCappasity)
        end
        println("----------------------")
    end

    for i = 1:numberOfVertexes
        for j = 1:numberOfVertexes
            print("$(graph.C[i, j]) ")
        end
        println("")
    end

    # Define optimization model
    model = Model(HiGHS.Optimizer)
    #Initializing flows
    @variable(model, F[1:numberOfVertexes, 1:numberOfVertexes]>=0)
    @variable(model, fmax >= 0)

    # Add flow conservation constraints
    for i = 1:numberOfVertexes
        # to co wchodzi do i                                         to co wychodzi z i
        @constraint(model, sum(F[j, i] for j = 1:numberOfVertexes) - sum(F[i, j] for j = 1:numberOfVertexes) ==
            if i == 1
                -fmax
            elseif i == numberOfVertexes
                fmax
            else
                0
            end)
    end

    # Add capacity constraints
    for i = 1:numberOfVertexes
        for j= 1:numberOfVertexes
            @constraint(model, F[i, j] <= graph.C[i, j])
        end
    end

    

    # Set objective function
    @objective(model, Max, fmax)

    # Solve the optimization problem
    optimize!(model)


    println("Maximum Flow: $((objective_value(model)))")
    return 0
end

# Run the main function with command line arguments
main(ARGS)