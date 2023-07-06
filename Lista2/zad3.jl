using JuMP
import HiGHS


model = Model(HiGHS.Optimizer)

#liczba dzielniec
D=3

#liczba zmian
Z=3

minShift = [10 20 18] # minimalna liczba radiowozów dla zmian
minDistrict = [10 14 13] # minimalna liczba radiowozów dla dzielnic

#Minimalna liczba radiowozów dla i-tej dzielnicy podczsas j-tej zmiany
l = [ 2 4 3;
    3 6 5;
    5 7 6;
]

#Maksymalna liczba radiowozów dla i-tej dzielnicy podczas j-tej zmiany
u = [ 3 7 5;
    5 7 10;
    8 12 10;
]

@variable(model,x[i=1:3, j=1:3]>=0) #ilość radiowozów w i-tej dzielnicy w j-tej zmianie 

@objective(model, Min, sum(x[i,j] for i in 1:3,j in 1:3))

@constraint(model,[j in 1:Z],sum(x[i,j] for i in 1:3)>=minShift[j]) #Każda zmiana musi mieć dostęp do conajmniej i radiowozów
@constraint(model,[i in 1:D],sum(x[i,j] for j in 1:3)>=minDistrict[i]) #Każda dzielnica musi mieć dostęp do conajmniej i radiowozów

#Ograniczenia z tabelki 
@constraint(model,[i in 1:D, j in 1:Z],l[i,j]<=x[i,j]<=u[i,j])



optimize!(model)

println("Sumaryczna ilość radiowozów: $(objective_value(model))")

for i in 1:D
    for j in 1:Z
        println("Dzielnica $(i) zmiana $(j): $(value(x[i,j]))")
    end
end        