#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <limits>
#include <list>
#include <algorithm>
#include <sstream>
#include <chrono>

using namespace std::chrono;



unsigned long long timeSum = 0LL;


class Graph {
public:
    int numberOfVertex;
    int numberOfEdges;
    std::vector<std::vector<std::pair<int, int>>> edges;

    Graph(int n, int m) {
        numberOfVertex = n;
        numberOfEdges = m;
        edges.resize(n);
    }

    void addEdge(int u, int v, int w) {
        edges[u].push_back(std::make_pair(v, w));
    }
};

std::vector<int> dijkstra(int source, const Graph& graph, int number) {
    auto start_time = high_resolution_clock::now();

    std::vector<int> dist(graph.numberOfVertex, std::numeric_limits<int>::max());
    std::vector<bool> visited(graph.numberOfVertex, false);
    dist[source] = 0;

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    pq.push(std::make_pair(0, source));

    while (!pq.empty()) {
        int cost = pq.top().first;
        int vertex = pq.top().second;
        pq.pop();
        
        if(visited[vertex]){
        	continue;
        }
        visited[vertex] = true;

       for (const auto& neighbour : graph.edges[vertex]) {
           int newDist = dist[vertex] + neighbour.second;
              if (newDist < dist[neighbour.first]) {
                  dist[neighbour.first] = newDist;
                  pq.push(std::make_pair(newDist, neighbour.first));
          }
       }
        
    }

    auto stop_time = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_time - start_time);
    timeSum += duration.count()/number;

    return dist;
}

void prepareOutputFile(std::ofstream& output,std::string data,std::string test_data){
    output<<"c"<<" Wyniki testu dla plików:"<<std::endl;
    output<<"f"<<" "<<data<<" "<<test_data<<std::endl;
}


int main(int argc, char** argv) {

    if (argc != 8) {
        std::cout<<argc<<std::endl;
        std::cerr << "Usage: " << argv[0] << " dijkstra -d file.gr -ss source.ss -oss output.res" << std::endl;
        return 1;
    }

    std::string algo = argv[1];
    if (algo != "dijkstra") {
        std::cerr << "Unsupported algorithm: " << algo << std::endl;
        return 1;
    }

    std::string data_file = "";
    std::string source_file = "";
    std::string output_file = "";
    std::string pair_source_file = "";
    std::string pair_output_file = "";
    for (int i = 2; i < argc; i += 2) {
        std::string option = argv[i];
        std::string value = argv[i+1];
        if (option == "-d") {
            data_file = value;
        } else if (option == "-ss") {
            source_file = value;
        } else if (option == "-oss") {
            output_file = value;
        } else if (option == "-op2p") {
            pair_output_file = value;    
        } else if (option == "-p2p") {
            pair_source_file = value;
        }
        else {
            std::cerr << "Unknown option: " << option << std::endl;
            return 1;
        }
    }
    

    if (data_file.empty() || (source_file.empty() && pair_source_file.empty()) || (output_file.empty() && pair_output_file.empty())) {
        std::cout << "Missing option(s)." << std::endl;
        return 1;
    }
    


    std::ifstream data(data_file);
    if (!data.is_open()) {
        std::cout<< "Error opening data file: " << data_file << std::endl;
        return 1;
    }
    



    std::string header;
    int n,m;
    while(std::getline(data,header)){
    	
    	if(header[0] == 'p'){
    		size_t start = 0;
    		size_t pos = header.find(" ");
    		std::string word;
    		std::vector<std::string> words;
    		while (pos != std::string::npos) {
        		word = header.substr(start, pos - start);
        		words.push_back(word);
        		start = pos + 1;
        		pos = header.find(' ', start);
   		}
    		n = stoi(words[2]);
    		m = stoi(header.substr(start));
    		break;
    	}
    }
   
    Graph graph(n,m);
    std::string line;
    while(std::getline(data,line)){
    	if(line[0] == 'a')
    	{
    		size_t start = 0;
    		size_t pos = line.find(" ");
    		std::string word;
    		std::vector<std::string> words;
    		while (pos != std::string::npos) {
        		word = line.substr(start, pos- start);
        		words.push_back(word);
        		start = pos + 1;
        		pos = line.find(' ', start);
   		 }
   		 int value = stoi(line.substr(start));
    		graph.addEdge(stoi(words[1]), stoi(words[2]), value);
	
    	}
    }
    
    if(!source_file.empty() && !output_file.empty()){

    std::ifstream sourceFile(source_file);
    if (!sourceFile.is_open()) {
        std::cout << "Error opening source file: " << source_file << std::endl;
        return 1;
    }
    
    std::ofstream output(output_file);
    if (!output.is_open()) {
        std::cout << "Error opening output file: " << output_file << std::endl;
        return 1;
    }
    
	std::string source;

	prepareOutputFile(output,data_file,source_file);
    std::string ch,aux,sp,ss;
    int number;
	
	while(getline(sourceFile,source)){
		if(source[0] == 'p'){
            std::istringstream iss = std::istringstream(source);
            iss >>ch>> aux >> sp >> ss >> number;
        }
    	if(source[0] == 's')
    	{
    		size_t start = 0;
    		size_t pos = source.find(" ");
    		std::string word;
    		std::vector<std::string> words;
    		while (pos != std::string::npos) {
            		word = source.substr(start, pos - start);
            		words.push_back(word);
            		start = pos + 1;
            		pos = source.find(' ', start);
        	}

        	int sourceValue = stoi(source.substr(start));	
        	std::vector<int> dist = dijkstra(sourceValue,graph,number);
        	
    	}
    }
	output<<"t "<<timeSum<<" msec"<<std::endl;

    }
    else if(!pair_source_file.empty() && !pair_output_file.empty()){

        std::ifstream pairsourceFile(pair_source_file);
    if (!pairsourceFile.is_open()) {
        std::cout << "Error opening source file: " << pair_source_file << std::endl;
        return 1;
    }
    
    std::ofstream pair_output(pair_output_file);
    if (!pair_output.is_open()) {
        std::cout << "Error opening output file: " << pair_output_file << std::endl;
        return 1;
    }

	prepareOutputFile(pair_output,data_file,pair_source_file);
    std::string ch,aux,sp,p2p;
    int number;
    
	std::string pair_source;
	
	while(std::getline(pairsourceFile,pair_source)){
		if(pair_source[0] == 'p'){
            std::istringstream iss = std::istringstream(pair_source);
            iss >>ch>> aux >> sp >> p2p >> number;
        }
    	if(pair_source[0] == 'q')
    	{
    		size_t start = 0;
    		size_t pos = pair_source.find(" ");
    		std::string word;
    		std::vector<std::string> words;
    		while (pos != std::string::npos) {
            		word = pair_source.substr(start, pos - start);
            		words.push_back(word);
            		start = pos + 1;
            		pos = pair_source.find(' ', start);
        	}                                    

        	int sourceValue = stoi(words[1]);                                                                                                  
			int pair = stoi(pair_source.substr(start));	
        	std::vector<int> dist = dijkstra(sourceValue,graph,number);
        	
        	pair_output<< "d "<<sourceValue<<" "<<pair<<" "<<dist[pair]<<std::endl; 
    	}                                                            
    }
    pair_output<<"t "<<timeSum<<" msec"<<std::endl;

    }
        
}
