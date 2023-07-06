using JuMP
import HiGHS


model = Model(HiGHS.Optimizer)

N=10
Tmax = 30

cityStart = 1
cityEnd = 10


c = fill(9999,N,N)
t = fill(9999, N,N) 

c[1,2] = 2
c[2,1] = 2

c[2,5] = 4
c[5,2] = 4

c[5,7] = 6
c[7,5] = 6

c[7,10] = 3
c[10,7] = 3

c[1,3] = 7
c[3,1] = 7

c[7,9] = 5
c[9,7] = 5

c[4,9] = 2
c[9,4] = 2

c[1,6] = 6
c[6,1] = 6

c[6,8] = 7
c[8,6] = 7

c[3,7] = 8
c[7,3] = 8

c[1,4] = 6
c[4,1] = 6


for i in 1:N
    for j in 1:N
        if c[i,j] != 9999
            t[i,j]=10- c[i,j] #tablica czasów z miejsca i do j
        end    
    end
end        



@variable(model,x[i = 1:N, j = 1:N],Bin) #trasa od i do j 

@objective(model,Min,sum(c[i,j]*x[i,j] for i in 1:N,j in 1:N))

#suma czasów musibyc mniejsza niż Tmax
@constraint(model, sum(x[i,j]*t[i,j] for j in 1:N, i in 1:N)<=Tmax)

#można wybrać co najwyżej jedną drogę wychodzącą z miasta i
@constraint(model,[i in 1:N],sum(x[i,j] for j in 1:N) <= 1)

#można wybrać co najwyżej jedną drogę wychodzącą z miasta i 
@constraint(model,[j in 1:N],sum(x[i,j] for i in 1:N) <= 1)

#nie możesz wracac do poprzedniago miasta
@constraint(model,[i in 1:N,j in 1:N], sum(x[i, j] + x[j, i]) <= 1)


#ile weszło do miasta j tyle musi z niego wyjść(Oprócz miasta początkowego i końcowego)
@constraint(model, [j in 2:N-1], sum(x[i,j] for i in 1:N ) == sum(x[j,m] for m in 1:N))

#z miasta początkowego do miasta końcowego
@constraint(model,sum(x[cityStart,j] for j in 1:N) == 1)
@constraint(model,sum(x[i,cityEnd] for i in 1:N) == 1)

#nie można się zapętlać
#@constraint(model,sum(x[i,i] for i in 1:N)==0)

optimize!(model)



path = [[i,j] for i=1:N, j=1:N if value(x[i,j]) == 1]

time = 0
for i=1:N, j=1:N
    if value(x[i,j])==1
        global time += t[i,j]
    end
end 

println("Koszt $(objective_value(model))")
println("Najkrótsza ścieżka $(path)")
println("Czas ścieżki $(time)")
