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
        stack<int> s;
        bool isDirected; 
        int numberOfVerticles;
        int numberOfEdges;
        bool *isVisited;
        map<int,list<int>> edgesBegin;

    Graph(bool isD,int numEdges,int numVerticles);
    ~Graph();
    void addEdge(int start,int end);
    void DFS(int start);
    void DFS2(int start);
    void findSCC();
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
    while(!s.empty()){
        s.pop();
    }
}

void Graph::addEdge(int start, int end){
    edgesBegin[start].push_front(end);
}

void Graph::DFS(int start){
    stack<int> dfsStack;
    dfsStack.push(start);

    while(!dfsStack.empty()){
        int value = dfsStack.top();
        dfsStack.pop();
        if(!isVisited[value]){
            s.push(value);
            isVisited[value] = true;
            list<int>::iterator i;
            for(i = edgesBegin[value].begin(); i!= edgesBegin[value].end();i++){
                if(!isVisited[*i]){
                   dfsStack.push(*i);
                }
            } 
        }
    }
}

void Graph::DFS2(int start){
    stack<int> dfsStack;
    dfsStack.push(start);

    while(!dfsStack.empty()){
        int value = dfsStack.top();
        dfsStack.pop();
        if(!isVisited[value]){
            if(numberOfVerticles<=200){
                cout<<value<<endl;

            }
            isVisited[value] = true;
            list<int>::iterator i;
            for(i = edgesBegin[value].begin(); i!= edgesBegin[value].end();i++){
                if(!isVisited[*i]){
                   dfsStack.push(*i);
                }
            } 
        }
    }
}

void Graph::findSCC(){
    int count=0;
    int v;
    for(int v = 0; v <= numberOfVerticles; v++ ){
        isVisited[v]=false;
    }

    while(!s.empty()){
        v = s.top(); 
        s.pop( );      
        if(!isVisited[v]){
            count++;
            if(numberOfVerticles<=200){
                cout << "Silnie spojna skladowa " << count << " :"<<endl;
            }
            DFS2(v);

        }
    }
    cout<<"Liczba silne spojnych skladowych: "<<count<<endl;
}





int main(int argc, char *argv[])
{   
    ifstream testFile; 
    string num;
    if(argc>=2){
        string filename = argv[1];
        testFile.open("C:/Users/woks0/Downloads/aod_testy1/3/"+filename);
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

          }


        for(int v = 1; v <= g.numberOfVerticles; v++ ){
            if( !g.isVisited[v] ){
                g.DFS(v);
            };
        }
        g.findSCC();

        auto stop1 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::seconds>(stop1-start);
        cout<<"Czas dzialnia programu: "<<duration.count()<<" s"<<endl;

        }
    }
    return 0;
}