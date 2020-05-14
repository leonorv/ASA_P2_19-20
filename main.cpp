#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <tuple>
using namespace std;


struct Graph {

    int _numM, _numN, _numV, _max_flow = 0;
    
    vector<tuple<int, int>> *_adjLists;

    bool *_visited;
    int *_parents;

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
        _adjLists[id2].emplace_back(id1, 0);
        _adjLists[id1].emplace_back(id2, 1);
    }

    void addClusterEdge(int id1, int id2) {
        _adjLists[id2].emplace_back(id1, 1);
        _adjLists[id1].emplace_back(id2, 1);
    }

    void createEdges() { //create all edges in a grid-like graph
        for (int i = 1; i < _numV - 1; i++) {
            if (i%2 != 0) addClusterEdge(i, i+1); //added edge between in and out
            if (i > _numM*2 && i%2 == 0) addSingleEdge(i, i-_numM*2-1); //up
            if (_numV-1 - i > _numM*2 && i%2 == 0) addSingleEdge(i, i + _numM*2-1); //down
            if ((i/2)%_numM != 1 && i%2 == 0) addSingleEdge(i, i-3); //left
            if (i%(_numM*2) != 0 && i%2 == 0) addSingleEdge(i, i+1); //right
        }
    } 

    bool DFS(int s, int t) {
        if (s == t) return true;
        _visited[s] = true;
        for (tuple<int,int> &i : _adjLists[s]) {
            if (!_visited[get<0>(i)] && get<1>(i) > 0) {
                _parents[get<0>(i)] = s;
                if (DFS(get<0>(i), t)) return true;
            }
        }
        return false;
    }

    int Ford_Fulkerson() {
        while (DFS(0, _numV-1)) { 
            fill_n(_visited, _numV, false);
            for (int i = _numV - 1; i != 0; i = _parents[i]) {
                int p = _parents[i];
                
                for (tuple<int,int> &j : _adjLists[p]) {
                    if (get<0>(j) == i) {
                        get<1>(j) = 0;
                        break;
                    }
                }
                for (tuple<int,int> &k : _adjLists[i]) {
                    if (get<0>(k) == p) {
                        get<1>(k) = 2;
                        break;
                    }
                }
            }
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

    while (i < S) {
        getline(cin, line);
        sscanf(line.c_str(), "%d %d", &m, &n);
        id = (n-1)*M + m; //ins sao impar e outs sao par
        id = 2*id - 1; 
        graph.addSingleEdge(id+1, M*N*2+1); //added connections with supermarkets and sink
        i++;
    }
    i = 0;
    while(i < C) {
        getline(cin, line);
        sscanf(line.c_str(), "%d %d", &m, &n);
        id = (n-1)*M + m;
        id = 2*id - 1; 
        graph.addSingleEdge(0, id); //added connections with citizens and source
        i++;
    }
}

int main(int argc, char* argv[]) { 
    processInput(argc, argv);
    graph.createEdges();
    cout << graph.Ford_Fulkerson() << '\n';
    return 0; 
}