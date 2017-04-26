/******************************* ASA - Grupo 20 *******************************
** António Sarmento - 77906
** Diogo Redin      - 84711
*******************************************************************************/

/* Libraries. Only the bare minimum, no need for clutter */
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <list>
#include <map>
#include <queue>
#include <vector>

using namespace std;

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

/* Status */
enum Status {
	CORRECT = 0,
	INSUFFICIENT
};

/******************** Data structures and their "methods" *********************/

/* Vertex Structure */
typedef size_t Vertex;
Vertex new_vertex(int val) { return val; }
#define next_vertex(a) a + 1

/* Edge Structure */
typedef pair<Vertex, Vertex> Edge;
Edge new_edge(int a, int b) {
	Vertex u = new_vertex(a), v = new_vertex(b);
	return make_pair(u, v);
}
Edge new_edge(Vertex a, Vertex b) { return make_pair(a, b); }

/* Graph Structure */
class Graph {
	private:
		Status _status;
		int _total_cost;
		int _possible_roads, _possible_airports;
		int _final_roads, _final_airports;

		vector< list<Vertex> > _cities;

		map<Vertex, int> _airport_cost; /* _airport_cost[Vertex] = (int) cost */
		map<Edge, int>   _road_cost;    /* _road_cost[Edge]      = (int) cost */

	public:
		Graph(int num_vertices);
		~Graph();

		/* Class attribute methods */
		int cost()         const { return _total_cost; }
		int num_roads()    const { return _final_roads; }
		int num_airports() const { return _final_airports; }
		size_t size()      const { return _cities.size() - 1; }
		Status status()    const { return _status; }

		int cost_airport(Vertex city) { return _airport_cost[city]; }
		int cost_road(Edge road)      { return _road_cost[road]; }

		/* Class functional methods */
		// TODO

		/* Operator overrides */
		int& operator[](Vertex city) { return _airport_cost[city]; }
		int& operator[](Edge road)   { return _road_cost[road]; }
		friend ostream& operator<<(ostream& os, const Graph &graph);

		/* Magic methods */
		void sort_airports_cost();
		void min_span_tree();
};

/* Builds Graph */
Graph::Graph(int num_vertices) {

	_status = INSUFFICIENT;
	_cities = vector< list<Vertex> >(num_vertices + 1);

}
Graph::~Graph() { /* Nothing here */ }

/* Examines Graph */
ostream& operator<<(ostream& os, const Graph &graph) {
	switch ( graph.status() ) {
		case INSUFFICIENT:
			return os << "Insuficiente";

		default: {
			os << graph.cost() << endl;
			return os << graph.num_airports() << " " << graph.num_roads();
		}
	}
}

/****************************** Sort Algorithm ***********************************/
void Graph::sort_airports_cost() {

	// TODO

}

/***************************** Prim's Algorithm *********************************/
void Graph::min_span_tree() {

    //for ( city = _cities.begin(); city != _cities.end(); _cities++ ) {
		//cout << cost_airport(city->first) << endl;
	//}

}

/***************************** MAIN function **********************************/
int main(void) {

	int num_cities, num_airports, num_roads;

	get_numbers(&num_cities);
	Graph g(num_cities);

	/* Get number of possible Airports */
	get_numbers(&num_airports);

	/* Get Cost of each Airport (city, cost) */
	while ( num_airports-- > 0 ) {
		int a;
		int cost;
		get_numbers(&a, &cost);

		Vertex city = new_vertex(a);
		g[city] = cost;
	}

	/* Get number of possible roads */
	get_numbers(&num_roads);

	/* Get Cost of each Road (city_a, city_b, cost) */
	while ( num_roads-- > 0 ) {
		int a, b;
		int cost;
		get_numbers(&a, &b, &cost);

		Edge road = new_edge(a, b);
		g[road] = cost;
	}

	// TODO: apply algorithms
	g.sort_airports_cost();
	g.min_span_tree();
	cout << g << endl;

	return 0;
}
