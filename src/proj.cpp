/******************************* ASA - Grupo 20 *******************************
** António Sarmento - 77906
** Diogo Redin      - 84711
*******************************************************************************/

/* Libraries. Only the bare minimum, no need for clutter */
#include <stdio.h>
#include <iostream>
#include <map>
#include <queue>
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
typedef size_t Vertex;
Vertex new_vertex(int val) { return val; }
#define next_vertex(a) a + 1

/* Edge Structure */
typedef size_t Edge;
Edge new_edge(int val) { return val; }

/* Global queue */
std::priority_queue<Vertex, std::vector<Vertex>, std::greater<Vertex> > queue;
#define enqueue(a) queue.push_back(a)
#define dequeue()  queue.back(); queue.pop_back()
#define is_empty() queue.empty()

/* Graph Structure */
class Graph {
	private:
		unsigned char _status;
		int _total_cost;
		int _possible_roads, _possible_airports;
		int _final_roads, _final_airports;

		std::vector<Edge> 	_first;    	/* _first[Vertex] = Edge   */
		std::vector<Vertex> _vertex; 	/* _vertex[Edge]  = Vertex */
		std::vector<Edge> 	_next;     	/* _next[Edge]    = Edge   */

		// TODO: Main structure for our data

		std::map<Vertex, int> _airport_cost; /* _airport_cost[Vertex] = (int) cost */
		std::map<Edge, int>   _road_cost;    /* _road_cost[Edge]      = (int) cost */

	public:
		Graph(int num_vertices);
		~Graph();

		void sort_airports_cost();
		void min_span_tree();

		size_t size() const { return _first.size(); }

		int& operator[](size_t idx) { return _airport_cost[idx]; }
		friend std::ostream& operator<<(std::ostream& os, const Graph &graph);
};

/* Builds Graph */
Graph::Graph(int num_vertices) {

	_status = INSUFFICIENT;
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

/****************************** Sort Algorithm ***********************************/
void Graph::sort_airports_cost() {

	// Define our iterator
	std::map<Vertex, int>::iterator it;

	// Go through the costs and put them in a priority queue (used in prim's algorithm)
    for ( it = _airport_cost.begin(); it != _airport_cost.end(); it++ ) {
        queue.push(it->first);
	}

}

/***************************** Prim's Algorithm *********************************/
void Graph::min_span_tree() {

	// Sort the costs indexes
	// sort_airports_cost(); and replace c's with the indexes of the sorted array

	// Goes through all adjacent cities (in our case every city is a possibility)
	for (Vertex c = 1; c < size(); c = next_vertex(c) ) {

		// City has no connection decided yet (visited), we haven't reached the limit of roads
		// or airports, and it's possible to build at least a road or an airport
		if ( (_first[c] != 0 ) &&
			( _possible_roads > _final_roads || _possible_airports > _final_airports ) &&
			( _road_cost[c] != -1 || !(_airport_cost[c] == -1 && _airport_cost[c+1] == -1) )
		) {

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

			// Its connection has been decided!
			_first[c] = 0;

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
		int a;
		int cost;
		get_numbers(&a, &cost);

		Vertex city = new_vertex(a);
		g[city] = cost;
	}

	// Get Cost of each Road (city_a, city_b, cost)
	get_numbers(&num_roads);
	while ( num_roads-- > 0 ) {
		int a, b;
		int cost;
		get_numbers(&a, &b, &cost);

		Vertex city_a = new_vertex(a), city_b = new_vertex(b);
		// TODO: what do?
	}

	// TODO: apply algorithms
	g.min_span_tree();
	std::cout << g << std::endl;

	return 0;
}
