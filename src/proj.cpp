/******************************* ASA - Grupo 20 *******************************
** António Sarmento - 77906
** Diogo Redin      - 84711
*******************************************************************************/

/* Libraries. Only the bare minimum, no need for clutter */
#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>

/****************************** auxy functions *********************************/
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
		int _nr_cities;
		int _total_cost;
		int _possible_roads, _possible_airports;
		int _final_roads, _final_airports;

		std::vector<Edge> 	_first;    	/* _first[Vertex] = Edge   */
		std::vector<Vertex> _vertex; 	/* _vertex[Edge]  = Vertex */
		std::vector<Edge> 	_next;     	/* _next[Edge]    = Edge   */

		// TODO: Main structure for our data

		std::map<Vertex, int> _airport_cost; /* _airport_cost[Vertex] = (int) cost */
		std::map<Edge,   int> _road_cost;    /* _road_cost[Edge]      = (int) cost */

	public:
		Graph(int num_vertices);
		~Graph();
		void min_span_tree();

		int& operator[](size_t idx) {
			return _airport_cost[idx];
		}
		friend std::ostream& operator<<(std::ostream& os, const Graph &graph);
};

/* Builds Graph */
Graph::Graph(int num_vertices) {

	_status = INSUFFICIENT;
	_nr_cities = num_vertices;
	_first = std::vector<Edge>(num_vertices + 1);

}
Graph::~Graph() { /* Nothing here */ }

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

/**************************** Merge Sort Algorithm *******************************/
/* DIDN'T WORK IN C++ */

/***************************** Prim's Algorithm *********************************/

/******************* Finds Minimum Spanning Tree of Graph **********************/
void Graph::min_span_tree() {

	// Goes through all Cities
	for (int c = 1; c <= _nr_cities; c++ ) {

		// City has no connection decided yet (visited), we haven't reached the limit of roads
		// or airports, and it's possible to build at least a road or an airport
		if ( (_first[c] != -1 ) && 
			( _possible_roads > _final_roads || _possible_airports > _final_airports ) &&
			( _road_cost[c] != -1 || !(_airport_cost[c] == -1 && _airport_cost[c+1] == -1) ) ) {

			// Total Cost (a -> b)
			int airports_cost = _airport_cost[c] + _airport_cost[c+1];

			// Airport doesn't exist and is cheaper than a Road
			if ( airports_cost < _road_cost[c] ) {
					
				_total_cost += _airport_cost[c];
				_final_airports++;

				_airport_cost[c] = 0;
				_airport_cost[c+1] = 0;

			}

			// Airport doesn't exist but a Road is cheaper or the cost is the same
			else if ( airports_cost > _road_cost[c] || airports_cost > _road_cost[c] ) {
					
				_total_cost += _road_cost[c];
				_final_roads++;

				_road_cost[c] = 0;
				_road_cost[c+1] = 0;

			}

			// It's connection has been decided!
			_first[c] = -1;
		
		}

	}

}

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
	g.min_span_tree();
	std::cout << g << std::endl;

	return 0;
}
