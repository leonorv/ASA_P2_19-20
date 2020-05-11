#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <math.h>
#include <vector>
#include <tuple>
#include <set>
using namespace std;

int idToCluster(int id) {
    return static_cast <int>(floor((id+1)/2));
}


struct Graph {

    int _numM, _numN, _numV, _numClusters, _max_flow = 0;
    //int **_capacities;
    
    vector<tuple<int, int>> *_adjLists;

    bool *_visited;
    int *_parents;

    queue<int> _queue;

    public:

    Graph(int i) {
    }

    void setGraph(int M, int N) {
        _numM = M; //avenidas verticais
        _numN = N; //ruas horizontais
        _numV = _numM*_numN*2 + 2;
        _visited = new bool[_numV];
        _parents = new int[_numV];



        _adjLists = new vector<tuple<int, int>>[_numV];

    }

    void addSingleEdge(int id1, int id2) {
        _adjLists[id2].emplace_back(make_tuple(id1, 0));
        _adjLists[id1].emplace_back(make_tuple(id2, 1));
    }

    void addClusterEdge(int id1, int id2) {
        _adjLists[id2].emplace_back(make_tuple(id1, 1));
        _adjLists[id1].emplace_back(make_tuple(id2, 1));
    }

    void createEdges() { //create all edges in a grid-like graph
        for (int i = 1; i < _numV - 1; i++) {
            if (i%2 != 0) addClusterEdge(i, i+1); //added edge between in and out
            if (i > _numM*2 && i%2 == 0) addSingleEdge(i, i-_numM*2-1); //up
            if (_numV-1 - i > _numM*2 && i%2 == 0) addSingleEdge(i, i + _numM*2-1); //down
            if (fmod(i-1, _numM*2) != 0 && i%2 == 0 && (i-2)%_numM != 0) addSingleEdge(i, i-3); //left
            if (i%(_numM*2) != 0 && i%2 == 0) addSingleEdge(i, i+1); //right
        }
    }

    void addVertex(int id) {
        _visited[id] = false;
        _parents[id] = -1;
    }


    void printGraph() 
{ 
    for (int v = 0; v < _numV; ++v) 
    { 
        cout << "\n Adjacency list of vertex "
             << v << "\n head "; 
        for (tuple<int, int> x : _adjLists[v]) {
           cout << "-> " << get<0>(x); 
           cout << ", " << get<1>(x);
        }
        printf("\n"); 
    } 
} 

    bool BFS() { //is there a path from s to t?
        fill_n(_visited, _numV, false);
        _queue.push(0);
        _visited[0] = true;
        int current;
        while (!_queue.empty()) {

            current = _queue.front();
            _queue.pop();

                       
            for (tuple<int,int> &i : _adjLists[current]) {
                if (get<1>(i) > 0 && !_visited[get<0>(i)]) {
                    _parents[get<0>(i)] = current;
                    _visited[get<0>(i)] = true;
                    _queue.push(get<0>(i));
                }
            }
        }
        //printf("%d\n", _numV-1);
        return _visited[_numV - 1];
    } 

    int Ford_Fulkerson() {
        while (BFS()) { 
            for (int i = _numV - 1; i != 0; i = _parents[i]) {
                int p = _parents[i];
                
                for (tuple<int,int> &j : _adjLists[p]) {
                    if (get<0>(j) == i) get<1>(j) = 0;
                }
                for (tuple<int,int> &k : _adjLists[i]) {
                    if (get<0>(k) == p) get<1>(k) = 2;
                }


            }
            //printf("\n");
            _max_flow++;
        }
        return _max_flow;
    }

     
};

Graph graph(0);

void processInput(int argc, char*argv[]) {
    int M, N, S, C, m, n, id, i = 0;

    string line;   


    getline(cin, line);
    sscanf(line.c_str(), "%d %d", &M, &N);
    getline(cin, line);
    sscanf(line.c_str(), "%d %d", &S, &C);


    graph.setGraph(M, N);

    graph.addVertex(0); //added source
    graph.addVertex(M*N*2+1); //added sink; 

    while (i < S) {
        getline(cin, line);
        sscanf(line.c_str(), "%d %d", &m, &n);
        id = (n-1)*M + m; //ins sao impar e outs sao par
        id = 2*id - 1; 
        graph.addVertex(id); //has S but not C (added supermarket)
        graph.addVertex(id+1); //has S but not C (added supermarket)
        graph.addSingleEdge(id+1, M*N*2+1); //added connections with supermarkets and sink
        i++;
    }
    i = 0;
    while(i < C) {
        getline(cin, line);
        sscanf(line.c_str(), "%d %d", &m, &n);
        id = (n-1)*M + m;
        id = 2*id - 1; 
        graph.addVertex(id); //has C but not S (added citizen)
        graph.addVertex(id+1); //has C but not S (added citizen)
        graph.addSingleEdge(0, id); //added connections with citizens and source
        i++;
    }
}

int main(int argc, char* argv[]) { 
    processInput(argc, argv);
    graph.createEdges();
    //graph.printGraph();
    cout << graph.Ford_Fulkerson() << '\n';
    return 0; 
}