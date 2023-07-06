#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <map>
#include<list>
#include <sstream>
#include <chrono>

using namespace std::chrono;

using namespace std;
long long timeSum = 0LL;


class Graph{
    public:
    int numberOfVertex;
    int numberOfEdges;
    vector<vector<pair<int,int>>> edges;

    Graph(int n, int m){
        numberOfVertex = n;
        numberOfEdges = m;
        edges.resize(n); 
    }

    void addEdge(int u, int v, int w) {
        edges[u].push_back(make_pair(v, w));
    }

};


vector<int> dial(int start, Graph& graph, int C) {
    auto start_time = high_resolution_clock::now();
    int n = graph.numberOfVertex;
    vector<int> dist(n, numeric_limits<int>::max());
    dist[start] = 0;

    vector<list<int>> buckets(C + 1);

    buckets[0].push_back(start);

    int current_bucket = 0;

    while (true) {
        while (buckets[current_bucket].empty()) {
            current_bucket = (current_bucket + 1) % (C + 1);
        }

        int current_node = buckets[current_bucket].front();
        buckets[current_bucket].pop_front();

        for (const auto& neighbor : graph.edges[current_node]) {
            int new_distance = dist[current_node] + neighbor.second;

            if (new_distance < dist[neighbor.first]) {
                int old_distance = dist[neighbor.first];
                dist[neighbor.first] = new_distance;

                if (old_distance < numeric_limits<int>::max()) {
                    auto it = find(buckets[old_distance%(C+1)].begin(), buckets[old_distance%(C+1)].end(), neighbor.first);
                    if (it != buckets[old_distance%(C+1)].end()) {
                        buckets[old_distance%(C+1)].erase(it);
                    }
                }

                buckets[new_distance % (C + 1)].push_back(neighbor.first);
            }
        }

        if (buckets[current_bucket].empty()) {
            int next_bucket = (current_bucket + 1) % (C + 1);
            while (buckets[next_bucket].empty() && next_bucket != current_bucket) {
                next_bucket = (next_bucket + 1) % (C + 1);
            }

            if (next_bucket == current_bucket) {
                break;
            }

            current_bucket = next_bucket;
        }
    }
    
    auto stop_time = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_time - start_time);
    timeSum += duration.count();

    return dist;
}


void prepareOutputFile(ofstream& output,string data,string test_data){
    output<<"c"<<" Wyniki testu dla plików:"<<endl;
    output<<"f"<<" "<<data<<" "<<test_data<<endl;
}


int main(int argc, char** argv) {

    if (argc != 8) {
        cout<<argc<<endl;
        cerr << "Usage: " << argv[0] << " dial -d file.gr -ss source.ss -oss output.res" << endl;
        return 1;
    }

    string algo = argv[1];
    if (algo != "dial") {
        cerr << "Unsupported algorithm: " << algo << endl;
        return 1;
    }

    string data_file = "";
    string source_file = "";
    string output_file = "";
    string pair_source_file = "";
    string pair_output_file = "";
    for (int i = 2; i < argc; i += 2) {
        string option = argv[i];
        string value = argv[i+1];
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
            cerr << "Unknown option: " << option << endl;
            return 1;
        }
    }
    

    if (data_file.empty() || (source_file.empty() && pair_source_file.empty()) || (output_file.empty() && pair_output_file.empty())) {
        cout << "Missing option(s)." << endl;
        return 1;
    }
    


    ifstream data(data_file);
    if (!data.is_open()) {
        cout<< "Error opening data file: " << data_file << endl;
        return 1;
    }
    



    string header;
    int n,m;
    while(getline(data,header)){
    	
    	if(header[0] == 'p'){
    		size_t start = 0;
    		size_t pos = header.find(" ");
    		string word;
    		std::vector<string> words;
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
    string line;
    int maxValue =0;
    while(getline(data,line)){
    	if(line[0] == 'a')
    	{
    		size_t start = 0;
    		size_t pos = line.find(" ");
    		string word;
    		std::vector<string> words;
    		while (pos != std::string::npos) {
        		word = line.substr(start, pos- start);
        		words.push_back(word);
        		start = pos + 1;
        		pos = line.find(' ', start);
   		 }
   		int value = stoi(line.substr(start));
        if(value> maxValue){
            maxValue = value;
        }
    	graph.addEdge(stoi(words[1]), stoi(words[2]), value);
	
    	}
    }


    if(!source_file.empty() && !output_file.empty()){

    ifstream sourceFile(source_file);
    if (!sourceFile.is_open()) {
        cout << "Error opening source file: " << source_file << endl;
        return 1;
    }

    ofstream output(output_file);
    if (!output.is_open()) {
        cout << "Error opening output file: " << output_file << endl;
        return 1;
    }
    
	string source;

    prepareOutputFile(output,data_file,source_file);
    string ch,aux,sp,ss;
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
    		string word;
    		std::vector<string> words;
    		while (pos != std::string::npos) {
            		word = source.substr(start, pos - start);
            		words.push_back(word);
            		start = pos + 1;
            		pos = source.find(' ', start);
        	}
        	int sourceValue = stoi(source.substr(start));	
        	vector<int> dist = dial(sourceValue,graph,maxValue);
    	}
    }
    output<<"t "<<timeSum/number<<"msc"<<endl;
    }
    else if(!pair_source_file.empty() && !pair_output_file.empty()){

        ifstream pairsourceFile(pair_source_file);
    if (!pairsourceFile.is_open()) {
        cout << "Error opening source file: " << pair_source_file << endl;
        return 1;
    }
    
    ofstream pair_output(pair_output_file);
    if (!pair_output.is_open()) {
        cout << "Error opening output file: " << pair_output_file << endl;
        return 1;
    }
    
	string pair_source;

    prepareOutputFile(pair_output,data_file,pair_source_file);
    string ch,aux,sp,p2p;
    int number;
    
	
	while(getline(pairsourceFile,pair_source)){
        if(pair_source[0] == 'p'){
            std::istringstream iss = std::istringstream(pair_source);
            iss >>ch>> aux >> sp >> p2p >> number;
        }
    	if(pair_source[0] == 'q')
    	{
    		size_t start = 0;
    		size_t pos = pair_source.find(" ");
    		string word;
    		std::vector<string> words;
    		while (pos != std::string::npos) {
            		word = pair_source.substr(start, pos - start);
            		words.push_back(word);
            		start = pos + 1;
            		pos = pair_source.find(' ', start);
        	}

        	int sourceValue = stoi(words[1]);
			int pair = stoi(pair_source.substr(start));	
        	vector<int> dist = dial(sourceValue,graph,maxValue);
        	
        	pair_output<< "d "<<sourceValue<<" "<<pair<<" "<<dist[pair]<<endl; 
    	}
    }
        pair_output<<"t "<<timeSum/number<<" msec"<<endl;


    }
        
}
