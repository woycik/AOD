using JuMP
import HiGHS


model = Model(HiGHS.Optimizer)

#Koszty za godzinę prace maszyny Mi
costMachine = [2,2,3]

#Koszty materiałowe
cost = [4,1,1,1]

#Cena sprzedarzy
price = [9,7,6,5]

#Maksymalny popyt
demand = [400,100,150,500]

#Czas pracy maszyny(w minutach) na 1 kg produktu Pi
M =[5 10 6;
    3 6 4;
    4 5 3;
    4 2 1
] 

@variable(model,x[i=1:4]>=0) #ilość wyprodukowanego produktu Pi

@objective(model,Max,sum(x[i]*(price[i]-cost[i]-sum(M[i,j]*costMachine[j]/60 for j in 1:3)) for i in 1:4))

#Nie można przekraczac popytu
@constraint(model,[i in 1:4],sum(x[i])<=demand[i])

#Czas pracy żadnej maszyny nie może przekraczać 60h
@constraint(model,[j in 1:3],sum(x[i]*M[i,j] for i in 1:4)<=3600)

optimize!(model)

println("Całkowity zysk: $(objective_value(model))")
for i in 1:4
    println("Ilość wyprodukowanego produktu p$i : $(value(x[i]))")
end    