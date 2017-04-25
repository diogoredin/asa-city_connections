/******************************* ASA - Grupo 20 *******************************
** António Sarmento - 77906
** Diogo Redin      - 84711
*******************************************************************************/

/* Libraries. Only the bare minimum, no need for clutter */
#include <stdio.h>
#include <iostream>
#include <map>
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
		int _total_cost;
		int _final_roads, _final_airports;

		// TODO: Main structure for our data

		std::map<Vertex, int> _airport_cost; /* _airport_cost[Vertex] = (int) cost */
		std::map<Edge,   int> _road_cost;    /* _road_cost[Edge]      = (int) cost */

	public:
		Graph(int num_vertices);
		~Graph();
		void connect(Vertex a, Vertex b, int cost);

		int& operator[](size_t idx) {
			return _airport_cost[idx];
		}
		friend std::ostream& operator<<(std::ostream& os, const Graph &graph);
};

Graph::Graph(int num_vertices) {

	_status = INSUFFICIENT;

}
Graph::~Graph() { /* Nothing here */ }

/* Connects two Vertices */
void Graph::connect(Vertex a, Vertex b, int cost) {
	// TODO: depends on our data structure
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

/************************ Algorithm-based functions ***************************/
// TODO

/***************************** MAIN function **********************************/
int main(void) {

	int num_cities, num_airports, num_roads;

	get_numbers(&num_cities);
	Graph g(num_cities);

	// Get Cost of each Airport (city, cost)
	get_numbers(&num_airports);
	while ( num_airports-- > 0 ) {
		Vertex city;
		int cost;
		get_numbers(&city, &cost);

		g[city] = cost;
	}

	// Get Cost of each Road (city_a, city_b, cost)
	get_numbers(&num_roads);
	while ( num_roads-- > 0 ) {
		Vertex a, b;
		int cost;
		get_numbers(&a, &b, &cost);

		g.connect(a, b, cost);
	}

	// TODO: apply algorithms
	std::cout << g << std::endl;

	return 0;
}
