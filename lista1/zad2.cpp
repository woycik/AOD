#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
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
        bool* isVisited;
        int* numberOfIncomingEdges;
        map<int,list<int>> edges;
        list<int> topologicalOrder;

    Graph(bool isD,int numEdges,int numVerticles);
    ~Graph();
    void addEdgeStart(int start,int end);
    void deleteVerticle(int start);
    void chooseFirstVerticle();
    void increseVerticle(int start);
    bool doesItHaveCycles();
    void cycles();
    void showTopologicalOrder();

};
Graph::Graph(bool isD,int numEdges,int numVerticles){
    isDirected = isD;
    numberOfEdges = numEdges;
    numberOfVerticles = numVerticles;
    numberOfIncomingEdges = new int[numberOfVerticles];
    isVisited = new bool[numberOfVerticles];
    for(int i = 0;i<=numberOfVerticles;i++){
        numberOfIncomingEdges[i]=0;
        isVisited[i]=false;
    }
}

Graph::~Graph(){
    edges.clear();
    topologicalOrder.clear();
}
void Graph::addEdgeStart(int start, int end){
    edges[start].push_front(end);
}

void Graph::increseVerticle(int end){
    numberOfIncomingEdges[end] = numberOfIncomingEdges[end] +1; 
}

void Graph::deleteVerticle(int v){
    list<int>::iterator i;
    for(i = edges[v].begin(); i!= edges[v].end();i++){
        numberOfIncomingEdges[*i]--;         
    }
    isVisited[v]=true;
    if(numberOfVerticles<=200){
        topologicalOrder.push_back(v);
    }

    for(i = edges[v].begin(); i!= edges[v].end();i++){
        if(numberOfIncomingEdges[*i]==0){
            if(!isVisited[*i]){
                deleteVerticle(*i);
            }
        }
    }        
} 



void Graph::chooseFirstVerticle(){
    for(int i = 1; i<=numberOfVerticles;i++){
        if(numberOfIncomingEdges[i]==0 && !isVisited[i]){
            deleteVerticle(i);
        }
    }
}

bool Graph::doesItHaveCycles(){
        for(int i = 1; i<=numberOfVerticles;i++){
        if(!isVisited[i]){
            return true;
        }
    }
    return false;
}

void Graph::cycles(){
    if(doesItHaveCycles()){
        cout<<"Graf zawiera cykl skierowany"<<endl;
        if(numberOfVerticles<=200){
            showTopologicalOrder();
          }
      }
      else{
        cout<<"Graf nie zawiera cyklu skierowanego"<<endl;

      }
}

void Graph::showTopologicalOrder(){
    list<int>::iterator i;
    cout<<"Porzadek topologiczny :"<<endl;
    for(i=topologicalOrder.begin();i!=topologicalOrder.end();i++){
        cout<<*i<<endl;
    }
}


int main(int argc, char *argv[])
{   
    ifstream testFile; 
    if(argc>=1){
        string filename = argv[1];
        testFile.open("C:/Users/woks0/Downloads/aod_testy1/2/"+filename);
        if(testFile.is_open()){
            auto start = chrono::high_resolution_clock::now();
            getline(testFile,d);
            getline(testFile,numberOfVertices);
            getline(testFile,numberOfEdges);
            bool dir = d[0]== 'D';
            Graph g=Graph(dir,stoi(numberOfEdges),stoi(numberOfVertices));
            int i = 0;
            size_t pos = 0;
    
            while(getline(testFile, numbers)){ 
                pos = numbers.find(" ");
                number1 = numbers.substr(0,pos);
                number2 =numbers.substr(pos,numbers.size());
                g.addEdgeStart(stoi(number1), stoi(number2));
                g.increseVerticle(stoi(number2));

          }
          g.chooseFirstVerticle();
          g.cycles();
          
           
          auto stop1 = chrono::high_resolution_clock::now();
          auto duration = chrono::duration_cast<chrono::seconds>(stop1-start);
          cout<<"Czas dzialnia programu: "<<duration.count()<<" s"<<endl;
        }
    }
 
    return 0;
}