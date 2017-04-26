/******************************* ASA - Grupo 20 *******************************
** António Sarmento - 77906
** Diogo Redin      - 84711
*******************************************************************************/

/* Libraries. Only the bare minimum, no need for clutter */
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <list>
#include <queue>
#include <vector>

using namespace std;
#define INF 0x3f3f3f3f

/*************************** Auxiliary functions *****************************/
void get_numbers(int *a) {
	scanf("%d", a);
}
void get_numbers(int *a, int *b) {
	scanf("%d %d", a, b);
}
void get_numbers(int *a, int *b, int *c) {
	scanf("%d %d %d", a, b, c);
}

/* Status */
enum Status {
	CORRECT = 0,
	INSUFFICIENT
};

/******************** Data structures and their "methods" *********************/
 
/* Vertex Structure */
typedef pair<int, int> iPair;

/* Graph Structure */
class Graph {

    private:
		Status _status;
        int _num_vertices;
		int _total_cost;

		//std::vector< std::list<Vertex> > _cities;
        list< pair<int, int> > *adj;

    public:
        Graph(int _num_vertices);
        ~Graph();

        /* Graph Functions */
        void addEdge(int u, int v, int w);

        /* Algorithm Functions */
        void primMST();
};

/* Builds Graph */
Graph::Graph(int num_vertices) {
	this->_num_vertices = num_vertices;
    adj = new list<iPair> [num_vertices];
}
Graph::~Graph() { /* Nothing here */ }

/* Add Edge */
void Graph::addEdge(int u, int v, int w) {
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w));
}

/****************************** Sort Algorithm ***********************************/
void Graph::primMST() {

    /* Create Priority Queue */
    priority_queue< iPair, vector <iPair> , greater<iPair> > pq;
 
    int src = 0; // Taking vertex 0 as source
 
    // Create a vector for keys and initialize all
    // keys as infinite (INF)
    vector<int> key(_num_vertices, INF);
 
    // To store parent array which in turn store MST
    vector<int> parent(_num_vertices, -1);
 
    // To keep track of vertices included in MST
    vector<bool> inMST(_num_vertices, false);
 
    // Insert source itself in priority queue and initialize
    // its key as 0.
    pq.push(make_pair(0, src));
    key[src] = 0;
 
    /* Looping till priority queue becomes empty */
    while (!pq.empty()) {
        // The first vertex in pair is the minimum key
        // vertex, extract it from priority queue.
        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted key (key must be first item
        // in pair)
        int u = pq.top().second;
        pq.pop();
 
        inMST[u] = true;  // Include vertex in MST
 
        // 'i' is used to get all adjacent vertices of a vertex
        list< pair<int, int> >::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
        {
            // Get vertex label and weight of current adjacent
            // of u.
            int v = (*i).first;
            int weight = (*i).second;
 
            //  If v is not in MST and weight of (u,v) is smaller
            // than current key of v
            if (inMST[v] == false && key[v] > weight)
            {
                // Updating key of v
                key[v] = weight;
                pq.push(make_pair(key[v], v));
                parent[v] = u;
            }
        }
    }

    // Print edges of MST using parent array
    for (int i = 1; i < _num_vertices; ++i)
        printf("%d - %d\n", parent[i], i);
}

// Driver program to test methods of graph class
int main() {

    // create the graph given in above fugure
    int num_vertices = 9;
    Graph g(num_vertices);

    //  making above shown graph
    g.addEdge(0, 1, 4);
    g.addEdge(0, 7, 8);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 7, 11);
    g.addEdge(2, 3, 7);
    g.addEdge(2, 8, 2);
    g.addEdge(2, 5, 4);
    g.addEdge(3, 4, 9);
    g.addEdge(3, 5, 14);
    g.addEdge(4, 5, 10);
    g.addEdge(5, 6, 2);
    g.addEdge(6, 7, 1);
    g.addEdge(6, 8, 6);
    g.addEdge(7, 8, 7);

    g.primMST();

    return 0;
}
