
using JuMP
import HiGHS


model = Model(HiGHS.Optimizer)

N=10
Tmax = 15

cityStart = 1
cityEnd = 10


c= rand(1:N-1,N,N)      #koszty podróży z miejsca i do j

t = fill(0, 10,10) 


for i in 1:N
    for j in 1:N
        t[i,j]=10-c[i,j] #tablica czasów z miejsca i do j
    end
end        



@variable(model,x[i = 1:N, j = 1:N]>=0) #trasa od i do j 

@objective(model,Min,sum(c[i,j]*x[i,j] for i in 1:N,j in 1:N))


@constraint(model, sum(x[i,j]*t[i,j] for j in 1:N, i in 1:N)<=Tmax)

#można wybrać co najwyżej jedną drogę wychodzącą z miasta i
@constraint(model,[i in 1:N],sum(x[i,j] for j in 1:N) <= 1)

#można wybrać co najwyżej jedną drogę wychodzącą z miasta i 
@constraint(model,[j in 1:N],sum(x[i,j] for i in 1:N) <= 1)

#nie możesz wracac do poprzedniago miasta
@constraint(model,[i in 1:N,j in 1:N], sum(x[i, j] + x[j, i]) <= 1)


#ile weszło do miasta j tyle musi wyjść(Oprócz miasta początkowego i końcowego)
@constraint(model, [j in 2:N-1], sum(x[i,j] for i in 1:N ) == sum(x[j,m] for m in 1:N))

#z miasta początkowego do miasta końcowego
@constraint(model,sum(x[cityStart,j] for j in 1:N) == 1)
@constraint(model,sum(x[i,cityEnd] for i in 1:N) == 1)

#nie można się zapętlać
@constraint(model,sum(x[i,i] for i in 1:N)==0)

optimize!(model)



path = [[i,j] for i=1:N, j=1:N if value(x[i,j]) >0]

time = 0
for i=1:N, j=1:N
    if value(x[i,j])>0
        global time += t[i,j]
    end
end 

println("Koszt $(objective_value(model))")
println("Najkrótsza ścieżka $(path)")
println("Czas ścieżki $(time)")
