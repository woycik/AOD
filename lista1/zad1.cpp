#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
using namespace std;

string d;
string numbers;
string number1;
string number2;
string numberOfEdges;
string numberOfVertices;


class Graph{
    public:
        bool isDirected; 
        int numberOfVerticles;
        int numberOfEdges;
        bool *isVisited;
        map<int,list<int>> edges;
        list<int> parents;
        list<int> children;

    Graph(bool isD,int numEdges,int numVerticles);
    ~Graph();
    void addEdge(int start,int end);
    void DFS(int start);
    void BFS(int start);
    void show();
    void save(int parent,int child);

};
Graph::Graph(bool isD,int numEdges,int numVerticles){
    isDirected = isD;
    numberOfEdges = numEdges;
    numberOfVerticles = numVerticles;
    isVisited = new bool[numberOfVerticles];
    for(int i = 0;i<=numberOfVerticles;i++){
        isVisited[i]=false;
    }
}

Graph::~Graph(){
    parents.clear();
    children.clear();
    edges.clear();
}
void Graph::addEdge(int start, int end){
    edges[start].push_front(end);
    if(!isDirected){
        edges[end].push_front(start);
    }
}

void Graph::DFS(int start){
    isVisited[start] = true;
    cout<<start<<endl;
    list<int>::iterator i;
    for(i = edges[start].begin(); i!= edges[start].end();i++){
        if(!isVisited[*i]){
            save(start,*i);
            DFS(*i);
        }
    } 
}

void Graph::BFS(int start){
    isVisited[start] = true;
    list<int> queue;
    queue.push_front(start);
    while(!queue.empty()){
        int v = queue.front();
        queue.pop_front();
        cout<<v<<endl;
        list<int>::iterator i;
        for(i=edges[v].begin();i!=edges[v].end();i++){
            if(!isVisited[*i]){
                isVisited[*i]=true;
                queue.push_back(*i);
                save(v,*i);
                

            }
        }
    }

}
void Graph::show(){
    int size = children.size();
    cout<<"Drzewo przeszukiwania"<<endl;
    for(int i=0;i<size;i++){
        cout<<parents.front()<<" : " << children.front()<<endl;
        parents.pop_front();
        children.pop_front();
    }
    parents.clear();
    children.clear();
}

void Graph::save(int parent,int child){
    parents.push_back(parent);
    children.push_back(child);
}




int main(int argc, char *argv[]){
    
    ifstream testFile; 
    string num;
    if(argc>=2){
    string filename = argv[1];
    
    testFile.open("C:/Users/woks0/Downloads/aod_testy1/2/"+filename);
    if(testFile.is_open()){
            auto start = chrono::high_resolution_clock::now();
            getline(testFile,d);
            getline(testFile,numberOfVertices);
            getline(testFile,numberOfEdges);
            bool dir = d[0]== 'D';
            Graph g=Graph(dir,stoi(numberOfEdges),stoi(numberOfVertices));
            size_t pos = 0;
    
            while(getline(testFile, numbers)){ 
                pos = numbers.find(" ");
                number1 = numbers.substr(0,pos);
                number2 =numbers.substr(pos,numbers.size());
                g.addEdge(stoi(number1), stoi(number2));

            }while(getline(testFile, numbers)){ 
                pos = numbers.find(" ");
                number1 = numbers.substr(0,pos);
                number2 =numbers.substr(pos,numbers.size());
                g.addEdge(stoi(number1), stoi(number2));

            }
    
            for(int i = 0;i<stoi(numberOfVertices);i++){
                if(!g.isVisited[i+1]){
                    g.DFS(i+1);
                }
            }
            if(argc>=3){
                if(stoi(argv[2])==1){
                    g.show();
                    }
            }    

            
            auto stop1 = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::seconds>(stop1-start);
            cout<<"Czas dzialnia programu: "<<duration.count()<<" s"<<endl;
            }
        }


    return 0;
}