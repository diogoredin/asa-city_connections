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
typedef pair< size_t, int > Vertex;
Vertex new_vertex(int city, int cost) { return make_pair(city, cost); }

/* Graph Structure */
class Graph {
	private:
		Status _status;
		int _total_cost;
		int _final_roads, _final_airports;

		vector< list<Vertex> > _cities;
		vector<int> _airport_costs;

	public:
		Graph(int num_vertices);
		~Graph();

		/* Class attribute methods */
		int cost()         const { return _total_cost; }
		int num_roads()    const { return _final_roads; }
		int num_airports() const { return _final_airports; }
		size_t size()      const { return _cities.size() - 1; }
		Status status()    const { return _status; }

		/* Class functional methods */
		void connect(int u, int v, int cost);

		/* Operator overrides */
		int& operator[](size_t city) { return _airport_costs[city]; }
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

void Graph::connect(int u, int v, int cost) {
	_cities[u].push_back(new_vertex(v, cost));
    _cities[v].push_back(new_vertex(u, cost));
}

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

		g[a] = cost;
	}

	/* Get number of possible roads */
	get_numbers(&num_roads);

	/* Get Cost of each Road (city_a, city_b, cost) */
	while ( num_roads-- > 0 ) {
		int a, b;
		int cost;
		get_numbers(&a, &b, &cost);

		g.connect(a, b, cost);
	}

	// TODO: apply algorithms
	g.sort_airports_cost();
	g.min_span_tree();
	cout << g << endl;

	return 0;
}
