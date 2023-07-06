using JuMP
import HiGHS


model = Model(HiGHS.Optimizer)
set_silent(model)


m = 10
n = 10
k = 2
k2 = 4
containers = [(1,2),(4,3),(5,1),(2,4),(1,10),(7,2),(9,10)]


@variable(model,x[i=1:m, j=1:n]>=0) #Zmianna ustala miejsca w których jest kamera na planszy m x n dla obszaru widzenia kamery k1

@objective(model,Min,sum(x[i,j] for i in 1:m,j in 1:n))

@variable(model,x2[i=1:m, j=1:n],Bin) #Zmianna ustala miejsca w których jest kamera na planszy m x n dla obszaru widzenia kamery k2




for c in containers
    i,j = c
    #Każdy kontener musi stać w w miejscu monitorowanym przez conajmniej jedna kamerę o polu widzenia k.
    @constraint(model,sum(x[i+a,j+b] for a in -k:k,b in -k:k if i+a>=1 && i+a<=m && j+b >= 1 && j+b<=n)>=1)
    @constraint(model,sum(x2[i+a,j+b] for a in -k2:k2,b in -k2:k2 if i+a>=1 && i+a<=m && j+b >= 1 && j+b<=n)>=1)
    @constraint(model,sum(x[i,j])==0)
    @constraint(model,sum(x2[i,j])==0)
end

optimize!(model)

println("Obszar widzenia kamery: $k")
println("Liczba użytych kamer $(objective_value(model))")
println("Lokalizacje kamer:")
for i in 1:m
    for j in 1:n
        if value(x[i,j]) == 1
            println("($i, $j)")
        end
    end
end

@objective(model,Min,sum(x2[i,j] for i in 1:m,j in 1:n))
optimize!(model)

println("Obszar widzenia kamery: $k2")
println("Liczba użytych kamer $(objective_value(model))")
println("Lokalizacje kamer:")
for i in 1:m
    for j in 1:n
        if value(x2[i,j]) == 1
            println("($i, $j)")
        end
    end
end

