#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <sstream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <chrono>
#include <bits/stdc++.h>
using namespace std::chrono;
using namespace std;

enum Color {
    Red,
    Black
};

class Vertex {
public:
    int id; // id wierzchołka
    Color color; // kolor wierzchołka

    Vertex() {}

    Vertex(int idx, Color c) {
        id = idx;
        color = c;
    }

    bool operator<(const Vertex& other) const {
        return id < other.id;
    }
};

class Graph {
public:
    int numberOfVertex;
    int numberOfEdges;
    std::vector<std::vector<int>> C;
    std::map<Vertex, std::vector<Vertex>> edges;

    Graph(int n, int m) {
        numberOfVertex = n;
        numberOfEdges = m;
        C.resize(numberOfVertex + 2, std::vector<int>(numberOfVertex + 2, 0));
    }

    void addEdge(Vertex u, Vertex v, int c) {
        edges[u].push_back(v);
        C[u.id][v.id] = c;
    }
};


pair<vector<vector<int>>, int> EdmondsKarpAlgorithm(Graph& graph, int source, int t) {
    int fmax = 0;
    vector<vector<int>> F(graph.numberOfVertex+2, vector<int>(graph.numberOfVertex+2));
    vector<int> P(graph.numberOfVertex+2);
    vector<int> CPF(graph.numberOfVertex+2);

    for (int i = 0; i < graph.numberOfVertex+2; i++) {
        for (int j = 0; j < graph.numberOfVertex+2; j++) {
            F[i][j] = 0;
        }
    }

    while (true) {
        // Wykonaj przeszukiwanie wszerz, aby znaleźć ścieżkę powiększającą
        fill(P.begin(), P.end(), -1);
        fill(CPF.begin(), CPF.end(), 0);

        P[source] = -2;
        CPF[source] = INT_MAX;

        queue<int> q;
        q.push(source);

        while (!q.empty()) {
            int x = q.front();
            q.pop();

            for (int y = 0; y < graph.numberOfVertex +2; y++) {
                if (P[y] == -1 && graph.C[x][y] > F[x][y]) {
                    P[y] = x;
                    CPF[y] = min(CPF[x], graph.C[x][y] - F[x][y]);
                    if (y == t) {
                        // Znaleziono ścieżkę powiększającą
                        while (y != source) {
                            int prev = P[y];
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
    }

    return make_pair(F, fmax);
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Invalid number of arguments" << std::endl;
        return 1;
    }

    int size;
    bool printflow = false;
    int degree;
    string file;

    for (int i = 1; i < argc; i += 2) {
        std::string option = argv[i];
        std::string value = argv[i + 1];

        if (option == "--size") {
            size = std::stoi(value);
        } 
        else if (option == "--degree") {
            degree = std::stoi(value);
        } 
        else if (option == "--printflow") {
            printflow = true;
        } 
        else if(option == "--glpk"){
            file = value;
        }
        else {
            std::cerr << "Unknown option: " << option << std::endl;
            return 1;
        }
    }

    int numberOfVertexes = pow(2,size);

    // Creating Graph (V1 and V2 are both size 2^size )
    Graph graph(2 * numberOfVertexes, 2 * size * (1 << (size - 1)));

    std::vector<Vertex> red_vertexes(numberOfVertexes);
    std::vector<Vertex> black_vertexes(numberOfVertexes);

    // Initializing vertexes and edges
    for (int i = 1; i <= numberOfVertexes; i++) {
        Vertex u(i + numberOfVertexes, Red);
        Vertex v(i, Black);
        red_vertexes[i - 1] = u;
        black_vertexes[i - 1] = v;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, numberOfVertexes - 1);

    // Initializing start and end vertexes
    Vertex start(0, Black);
    Vertex end(2 * numberOfVertexes + 1, Red);

    // Initializing red vertexes
    for (Vertex v : red_vertexes) {
        graph.addEdge(start, v, 1);
        for (int i = 0; i < degree; i++) {
            Vertex neighbour = black_vertexes[dist(gen)];
            bool add = true;
            for (Vertex w : graph.edges[v]) {
                if (neighbour.id == 0 || w.id == neighbour.id) {
                    i--;
                    add = false;
                    break;
                }
            }
            if (add) {
                graph.addEdge(v, neighbour, 1);
            }
        }
    }

    // Initializing black vertexes
    for (Vertex v : black_vertexes) {
        graph.addEdge(v, end, 1);
    }

    if(!file.empty()){
        ofstream myfile;
        myfile.open ("zad2.csv");
        myfile<<endl;
        myfile << graph.numberOfVertex + 2<< endl;
        for(int i=0;i<graph.numberOfVertex+2;i++){
            for(int j=0;j<graph.numberOfVertex+2;j++){
                myfile<< graph.C[i][j];
            }
            myfile<<endl;
        }
    }

   
    // Musimy określić maksymalną wielkość przepływu ze źródła do ujścia sieci przy ograniczeniach przepustowości nałożonych na poszczególne kanały.
    auto start1 = high_resolution_clock::now();
    std::pair<std::vector<std::vector<int>>, int> maxFlow = EdmondsKarpAlgorithm(graph, start.id, end.id);
    std::cout<<"Max flow: "<<maxFlow.second<<std::endl;
    auto end1 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end1 - start1);
    cout<<maxFlow.second<<endl;
    cout<<"Czas: "<<duration.count()<<endl;

    if(printflow){
        cout<<"Przeplywy: "<<endl;
        for(int i=0;i<graph.numberOfVertex+2;i++){
            for(int j=0;j<graph.numberOfVertex+2;j++){
                int flow = maxFlow.first[i][j];
                if(flow>0){
                    cout<<i<<" "<<j<<" "<<flow<<endl;
                }
            }
        }
    }


    return 0;
}
