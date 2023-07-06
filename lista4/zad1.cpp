#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <sstream>
#include <chrono>
#include <cmath>
#include <chrono>
#include <bits/stdc++.h>
using namespace std::chrono;
using namespace std;

long long timeSum = 0LL;
random_device rd;
mt19937 gen(rd());



class Vertex{
    public:
    int id; 
    string e; 

    Vertex(){}

    Vertex(int idx,string ex){
        id = idx;
        e = ex;
    }

    bool operator<(const Vertex& other) const {
        if (id <= other.id){
            return true;
        }        
        return false;
    }
};

std::vector<Vertex> vertexes;

class Graph{
    public:
    long numberOfVertex;
    long numberOfEdges;
    long num;
    vector<vector<int>> C;
    std::map<Vertex, vector<Vertex>> edges;


    Graph(int n, int m){
        numberOfVertex = n;
        numberOfEdges = m;
        num = 0;
        C.resize(numberOfVertex, std::vector<int>(numberOfVertex));
        for (int i = 0; i < numberOfVertex; i++) {
            for (int j = 0; j < numberOfVertex; j++) {
                C[i][j] = 0;
            }
        }
    }

    void addEdge(Vertex u, Vertex v, int c) {
        edges[u].push_back(v);
        C[u.id][v.id] = c;
        num++;
    }

};

int calculateSum(int** array,int num){
    int sum = 0;
    for(int i =0; i<num; i++){
        for(int j=0;j<num;j++){
            sum+=array[i][j];   
        }
    }
    return sum;
}

pair<vector<vector<int>>, int> EdmondsKarpAlgorithm(Graph& graph, int source, int t, int &count) {
    int fmax = 0;

    //Wektor przepływu
    vector<vector<int>> F(graph.numberOfVertex, vector<int>(graph.numberOfVertex));
    //Parent
    vector<int> P(graph.numberOfVertex);
    //Wektor CPF przechowuje wartości cf(p), dla ścieżki kończącej się w danym węźle w sieci 
    vector<int> CPF(graph.numberOfVertex);

    for (int i = 0; i < graph.numberOfVertex; i++) {
        for (int j = 0; j < graph.numberOfVertex; j++) {
            F[i][j] = 0;
        }
    }

    while (true) {
        // Wykonuję przeszukiwanie wszerz, aby znaleźć ścieżkę powiększającą
        fill(P.begin(), P.end(), -1); 
        fill(CPF.begin(), CPF.end(), 0);

        P[source] = -2; //Inicjalizuje wartośc parenta źródła na -2, by nie wybierano go w BFS
        CPF[source] = INT_MAX;

        queue<int> q;
        q.push(source);

        while (!q.empty()) 
        {
            int x = q.front();
            q.pop();

            for (int y = 0; y < graph.numberOfVertex; y++) {
                if (P[y] == -1 && graph.C[x][y] > F[x][y]) {
                    P[y] = x;
                    //Wyznaczam wartość tej krawędzi w sieci rezydualnej
                    CPF[y] = min(CPF[x], graph.C[x][y] - F[x][y]);
                    if (y == t) {
                        //Znaleziono ścieżkę powiększającą
                        while (y != source) {
                            int prev = P[y];
                            //Ustalenie przepływów w sieci
                            F[prev][y] += CPF[t];
                            F[y][prev] -= CPF[t];
                            y = prev;
                        }
                        fmax += CPF[t];
                        break;
                    }
                    q.push(y);
                }
            }
        }

        // Jeżeli nie znaleziono ścieżki powiększającej, zakończ
        if (P[t] == -1) {
            break;
        }
        count++;
    }

    return make_pair(F, fmax);
}


int binaryToDecimal(int binary) {
    int decimal = 0;
    int base = 1;
    
    while (binary > 0) {
        int lastDigit = binary % 10;
        decimal += lastDigit * base;
        binary /= 10;
        base *= 2;
    }
    
    return decimal;
}


vector<Vertex> getVertexes(string labelString, int size) {
    int len = labelString.length();
    string modifiedLabel;
    vector<Vertex> v;

    for (int i = len - 1; i >= len - size; i--) {
        if (labelString[i] == '0') {
            modifiedLabel = labelString;
            modifiedLabel[i] = '1';

            bool isValidBinary = true;
            for (char c : modifiedLabel) {
                if (c != '0' && c != '1') {
                    isValidBinary = false;
                    break;
                }
            }

            if (isValidBinary) {
                int labelNumber = std::stoi(modifiedLabel, nullptr, 2);
                v.push_back(vertexes[labelNumber]);
            }
        }
    }
    return v;
}



int H(string label){
    int count = 0;
    for (char c : label) {
        if (c == '1') {
            count++;
        }
    }
    return count;
}


int Z(string label) {
    int count = 0;
    for (char c : label) {
        if (c == '0') {
            count++;
        }
    }
    return count;
}



int main(int argc, char** argv) {

    if (argc <2) {
        cout<<argc<<endl;
        cerr << "Invalid number of arguments" << endl;
        return 1;
    }

    int size;
    bool printflow = false;

    for (int i = 1; i < argc; i += 2) {

        string option = argv[i];
        string value = argv[i+1];

        if (option == "--size") {
            size = stoi(value);

        } 
        else if (option == "--printflow") {
            printflow = true;
        } 
        else {
            cerr << "Unknown option: " << option << endl;
            return 1;

        }

    }


    int numberOfVertexes = pow(2,size);

    //Creating Graph
    Graph graph(numberOfVertexes,size * pow(2,size-1));
    vertexes.resize(graph.numberOfVertex);


    //Initializing vertexes and edgse
    for(int i=0;i<numberOfVertexes;i++){
        std::bitset<32> binaryNumber(i);
        Vertex u(i, binaryNumber.to_string().substr(32 - size));
        vertexes[i] = u;
    }

    uniform_int_distribution<>dist(numberOfVertexes);

    for(int i=0;i<numberOfVertexes;i++){
        string currentLabel = vertexes[i].e;
        vector<Vertex> sameLabels = getVertexes(currentLabel,size);
        for(Vertex v : sameLabels){
            int max = std::max(std::max(H(v.e),H(currentLabel)), std::max(Z(v.e),Z(currentLabel)));
            int randomCappasity = dist(gen)%((int)pow(2,max)); //Losowy wybór wartości przepustowości  
            graph.addEdge(vertexes[i],v,randomCappasity); //Dodawanie krawędzi po wierzchołkach z ustalonymi wartosciami przepustowości
        }
    }


    int count = 0;
    auto start = high_resolution_clock::now();
    pair<vector<vector<int>>,int> maxFlow = EdmondsKarpAlgorithm(graph,0,pow(2,size)-1,count);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout<<"Liczba ściezek rozszerzajacych: "<<count<<endl;
    cout<<maxFlow.second<<endl;
    cout<<"Czas: "<<duration.count()<<endl;

    if(printflow){
        cout<<"Przeplywy: "<<endl;
        vector<vector<int>> flow = maxFlow.first;
        for(int i=0;i<graph.numberOfVertex;i++){
            for(int j=0;j<graph.numberOfVertex;j++){
                int flowvalue = flow[i][j];
                if(flowvalue>0){
                    cout<<i<<" "<<j<<" "<<flowvalue<<endl;
                }
            }
        }
    }
}