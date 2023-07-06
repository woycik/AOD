using JuMP
import HiGHS


model = Model(HiGHS.Optimizer)

#Liczba lotnisk
M=4

#Liczba firm
N=3

#Zapotrzebowanie lotnisk
demand = [110000,220000,330000,440000]

#Limity dostaw firm
deliveryLimit = [275000,550000,660000]

#Koszt jednego galonu paliwa dostarczanego na lotnisko i przez dostawce j
cost =[10 7 8;
        10 11 14;
        9 12 4;
        11 13 9]
 

#ilość paliwa dostarczoneho przez firme i na lotnisko j
@variable(model,x[i=1:N, j=1:M]>=0)

#Dostarczona ilość paliwa nie może przekroczyć zasobów dostawcy
@constraint(model,[i in 1:N],sum(x[i,j] for j in 1:M)<=deliveryLimit[i])

#Dostarczona ilość musi zaspokoić zapotrzebowanie lotnisk
@constraint(model,[j in 1:M],sum(x[i,j] for i in 1:N)>=demand[j])

#Minimalizuje koszta
@objective(model,Min,sum(x[i,j]*cost[j,i] for i in 1:3,j in 1:4))

optimize!(model)



println("Minimalny koszt dostaw paliwa to $((objective_value(model)))")

for i in 1:3
    println("Firma" *string(i))
    for j in 1:4
        println(value(x[i,j]))
    end
end       
 
for i in 1:3
        if (sum(value(x[i,j]) for j in 1:4)==0)
            println("Nie każda firma dostarcza paliwo.")
            break
        end        
end 

