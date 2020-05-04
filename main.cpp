#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <math.h>
using namespace std;

struct Graph {

    int _numM, _numN, _numS, _numC, _numV, _max_flow = 0;
    int **_capacities;
    int  *_dtimeList;
    int  *_SList;
    int *_CList; // c[1] = 1 se o vertex 1 tiver uma pessoa
    bool *_visited;
    int *_parents;
    queue<int> _queue;

    public:

    Graph(int i) {
    }

    void setGraph(int M, int N, int S, int C) {
        _numM = M; //avenidas verticais
        _numN = N; //ruas horizontais
        _numS = S;
        _numC = C;
        _numV = _numM * _numN + 2;
        _visited = new bool[_numV];
        _parents = new int[_numV];
        _SList = new int[_numV];
        fill_n(_SList, _numV, 0);
        _CList = new int[_numV];
        fill_n(_CList, _numV, 0);

        //ALLOCATE AND INITIALIZE CAPACITIES AT 0
        _capacities = new int*[_numV];
        for (int i = 0; i < _numV; i++) {
            _capacities[i] = new int[_numV];
            fill_n(_capacities[i], _numV, 0);
        }
    }

    void addEdge(int id1, int id2) {
        _capacities[id1][id2] = 1;
        _capacities[id2][id1] = 1;
    }

    void createEdges() { //create all edges in a grid-like graph
        for (int i = 1; i < _numV - 1; i++) {
            if (i > _numM) addEdge(i, i - _numM); //up
            if (_numV-1 - i > _numM) {
                //printf("add edge %d e %d\n", i, i+_numM);
                addEdge(i, i + _numM); //down
            }
            if (fmod(i-1, _numM) != 0) addEdge(i, i-1); //left
            if (i%_numM != 0) addEdge(i, i+1); //right
        }
    }

    void addVertex(int id, int hasS, int hasC) {
        _SList[id] = hasS;
        _CList[id] = hasC;
        _visited[id] = false;
        _parents[id] = -1;
    }

    void printVertexList() {
        for (int i = 0; i < _numV; i++) {
            cout << _CList[i] << endl;
        }
    }

    void printGraph() { 
        for (int i = 0; i < _numV; i++) {
            cout << i << " : ";
            for (int j = 0; j < _numV; j++)
                cout << _capacities[i][j] << " ";
            cout << "\n";
        } 
    }

    bool BFS() { //is there a path from s to t?
        fill_n(_visited, _numV, false);
        //_visited[_numV - 1] = false;
        _queue.push(0);
        _visited[0] = true;
        int current;
        while(!_queue.empty()) {
            current = _queue.front();
            _queue.pop();
            for (int i = 0; i < _numV; i++) {
                if (_capacities[current][i] > 0 && !_visited[i]) {
                    _parents[i] = current;
                    //printf("estamos a visitar %d\n", i);
                    _visited[i] = true;
                    _queue.push(i);
                }
            }
        }
        return _visited[_numV - 1];
    }

    int Ford_Fulkerson() {
        while (BFS()) {
            for (int i = _numV - 1; i != 0; i = _parents[i]) {
                int p = _parents[i];
                printf("(%d,%d)  ", p, i);
                _capacities[p][i] = 0;
                _capacities[i][p] = 2;
            }
            printf("\n");
            _max_flow+=1;
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


    graph.setGraph(M, N, S, C);

    graph.addVertex(0, 0, 0); //added source
    graph.addVertex(M*N+1, 0, 0); //added sink; 

    while (i < S) {
        getline(cin, line);
        sscanf(line.c_str(), "%d %d", &m, &n);
        id = (n-1)*M + m;
        graph.addVertex(id, 1, 0); //has S but not C (added supermarket)
        graph.addEdge(M*N+1, id); //added connections with supermarkets and sink
        i++;
    }
    i = 0;
    while(i < C) {
        getline(cin, line);
        sscanf(line.c_str(), "%d %d", &m, &n);
        id = (n-1)*M + m;
        graph.addVertex(id, 0, 1); //has C but not S (added citizen)
        graph.addEdge(0, id); //added connections with citizens and source
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