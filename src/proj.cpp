/******************************* ASA - Grupo 20 *******************************
** António Sarmento - 77906
** Diogo Redin      - 84711
*******************************************************************************/

/* Libraries. Only the bare minimum, no need for clutter */
#include <stdio.h>
#include <iostream>
#include <vector>

/*************************** Auxiliary functions ******************************/
void get_numbers(int *a) {
	scanf("%d", a);
}
void get_numbers(int *a, int *b) {
	scanf("%d %d", a, b);
}
void get_numbers(int *a, int *b, int *c) {
	scanf("%d %d %d", a, b, c);
}

/*********************** Visit States & Graph Status **************************/
enum graphStatus {
	CORRECT = 0,
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
		unsigned char _status;
		int _nr_edges;
		int _total_cost;
		int _final_roads, _final_airports;

		std::vector<Edge> _first;    /* _first[Vertex] = Edge   */
		std::vector<Vertex> _vertex; /* _vertex[Edge]  = Vertex */
		std::vector<Edge> _next;     /* _next[Edge]    = Edge   */

		std::vector<int> _airport_cost; /* _airport_cost[Vertex] = (int) cost */
		std::vector<int> _road_cost;    /* _road_cost[Edge]      = (int) cost */

	public:
		Graph(int num_vertices);
		~Graph();
		void sort();
		void connect(Vertex a, Vertex b);

		int& operator[](size_t idx) {
			return _airport_cost[idx];
		}
		friend std::ostream& operator<<(std::ostream& os, const Graph &graph);
};

Graph::Graph(int num_vertices) {

	_status = INSUFFICIENT;
	_first = std::vector<Edge>(num_vertices + 1);

}
Graph::~Graph() { /* Nothing here */ }

/* Connects two Vertices */
void Graph::connect(Vertex a, Vertex b) {
	_vertex[_nr_edges] = new_edge(b);

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
		case INSUFFICIENT:
			return os << "Insuficiente";

		default: {
			os << graph._total_cost << std::endl;
			return os << graph._final_airports << " " << graph._final_roads;
		}
	}
}

/************************* Vertex "Deletion" Algorithm ***************************/
void Graph::sort() {
	// TODO
}

/***************************** MAIN function **********************************/
int main(void) {

	int num_cities, num_airports, num_roads;

	get_numbers(&num_cities);
	Graph g(num_cities);

	// Get Cost of each Airport (city, cost)
	get_numbers(&num_airports);
	while ( num_airports-- > 0 ) {
		int city, cost;
		get_numbers(&city, &cost);

		g[city] = cost;
	}

	// Get Cost of each Road (city_a, city_b, cost)
	get_numbers(&num_roads);
		// TODO: Create roads
	while ( num_roads-- > 0 ) {
	}

	g.sort();
	std::cout << g << std::endl;

	return 0;
}
