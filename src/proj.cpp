/******************************* ASA - Grupo 20 *******************************
** António Sarmento - 77906
** Diogo Redin      - 84711
*******************************************************************************/

/* Libraries. Only the bare minimum, no need for clutter */
#include <stdio.h>
#include <iostream>
#include <vector>

/*************************** Auxiliary functions ******************************/
#define get_numbers(a, b) scanf("%d %d", a, b)

/*********************** Visit States & Graph Status **************************/
enum graphStatus {
	CORRECT = 0,
	INCOHERENT,
	INSUFFICIENT
};

/******************** Data structures and their "methods" *********************/

/* Vertex Structure */
typedef int Vertex;
#define new_vertex(a) a
#define next_vertex(a) a + 1

/* Edge Structure */
typedef int Edge;
#define new_edge(a) a

/* Global queue */
std::vector<Vertex> queue;
#define enqueue(a) queue.push_back(a)
#define dequeue()  queue.back(); queue.pop_back()
#define is_empty() queue.empty()

/* Graph Structure */
class Graph {
	private:
		int _nr_edges;
		unsigned char _status;

		std::vector<Edge> _first;    /* _first[Vertex] = Edge   */
		std::vector<Vertex> _vertex; /* _vertex[Edge]  = Vertex */
		std::vector<Edge> _next;     /* _next[Edge]    = Edge   */

		std::vector<int> _indegree;
		std::vector<Vertex> _result;

	public:
		Graph(int num_v, int num_e);
		~Graph();
		void sort();
		void connect(Vertex a, Vertex b);
		friend std::ostream& operator<<(std::ostream& os, const Graph &graph);
};

Graph::Graph(int num_v, int num_e) {
	Vertex u;

	_status   = INCOHERENT;
	_first    = std::vector<Edge>(num_v+1);
	_vertex   = std::vector<Vertex>(num_e+1);
	_next     = std::vector<Edge>(num_e+1);

	_indegree = std::vector<int>(num_v+1);

	for (_nr_edges = 1; _nr_edges <= num_e; _nr_edges++) {
		int num1, num2;
		Vertex v;

		get_numbers(&num1, &num2);
		u = new_vertex(num1);
		v = new_vertex(num2);

		connect(u, v);
	}

	/* Adds all orphans to Queue */
	for (u = 1; u <= num_v; u = next_vertex(u)) {
		if (_indegree[u] == 0) {
			enqueue(u);
		}
	}
}
Graph::~Graph() { /* Nothing here */ }

/* Connects two Vertices */
void Graph::connect(Vertex a, Vertex b) {
	_vertex[_nr_edges] = new_edge(b);
	_indegree[b]++;

	if ( _first[a] == 0 ) {
		_first[a] = _nr_edges;
	} else {
		Edge find_edge = _first[a];

		for ( ; _next[find_edge] != 0; find_edge = _next[find_edge] );
		_next[find_edge] = _nr_edges;
	}
}

/* Examines Graph */
std::ostream& operator<<(std::ostream& os, const Graph &graph) {
	switch ( graph._status ) {
		case INCOHERENT:
			return os << "Incoerente";

		case INSUFFICIENT:
			return os << "Insuficiente";

		default: {
			int i, size = graph._result.size() - 1;
			for ( i = 0; i < size; i++ ) {
				os << graph._result[i] << " ";
			}
			return os << graph._result[i];
		}
	}
}

/************************* Vertex "Deletion" Algorithm ***************************/
void Graph::sort() {
	while ( !is_empty() ) {
		Vertex u = dequeue();
		_result.push_back(u);

		if ( _indegree[u] == 0 ) {
			int max_solutions = 0;

			for ( Edge find_son = _first[u]; find_son != 0; find_son = _next[find_son] ) {
				Vertex v = _vertex[find_son];

				if ( --_indegree[v] == 0 ) {
					enqueue(v);
					max_solutions++;
				}

				/* If suddenly u has more than 1 adjacent whose _indegree == 0 */
				if (max_solutions > 1) {
					_status = INSUFFICIENT;
					return;
				}
			}
		}
	}

	if ( _result.size() == _first.size() - 1 ) {
 		_status = CORRECT;
 	}
}

/***************************** MAIN function **********************************/
int main(void) {
	int num_v, num_e;
	get_numbers(&num_v, &num_e);

	Graph g(num_v, num_e);
	g.sort();
	std::cout << g << std::endl;

	return 0;
}
