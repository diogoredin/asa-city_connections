/******************************* ASA - Grupo 20 *******************************
** António Sarmento - 77906
** Diogo Redin      - 84711
*******************************************************************************/

/* Libraries. Only the bare minimum, no need for clutter */
#include <stdio.h>
#include <iostream>
#include <queue>

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

/* Vertex structure */
typedef size_t Vertex;
Vertex new_vertex(int val) { return val; }

/* Edge Structure */
typedef pair< int, pair<Vertex, Vertex> > Edge;
Edge new_edge(Vertex city_a, Vertex city_b, int cost) {
	return make_pair(cost, make_pair(city_a, city_b));
}

/* Graph Structure */
class Graph {
	private:
		Status _status;
		size_t _num_vertices;
		int _total_cost;
		int _final_roads, _final_airports;

		priority_queue< Edge, vector<Edge>, greater<Edge> > _roads;

	public:
		Graph(int num_vertices);
		~Graph();

		/* Class attribute methods */
		int cost()         const { return _total_cost; }
		int num_roads()    const { return _final_roads; }
		int num_airports() const { return _final_airports; }
		size_t size()      const { return _roads.size(); }
		Status status()    const { return _status; }

		/* Class functional methods */
		void connect(Vertex u, Vertex v, int cost);

		/* Operator overrides */
		friend ostream& operator<<(ostream& os, const Graph &graph);

		/* Algorithmic methods */
		void min_span_tree();
};

/* Builds Graph */
Graph::Graph(int num_vertices) {

	_status = CORRECT;
	_num_vertices = num_vertices;
	_total_cost = 0;
	_final_airports = 0;
	_final_roads = 0;

}
Graph::~Graph() { /* Nothing here */ }

/* Adds Edge */
void Graph::connect(Vertex u, Vertex v, int cost) {
	_roads.push(new_edge(u, v, cost));
}

/* Examines Graph */
ostream& operator<<(ostream& os, const Graph &graph) {
	switch ( graph.status() ) {
		case INSUFFICIENT:
			return os << "Insuficiente";

		default: {
			os << "Total Cost : " << graph.cost() << endl;
			os << "Airports : " << graph.num_airports() << endl;
			return os << "Roads : " << graph.num_roads();
		}
	}
}

/***************************** Prim's Algorithm *********************************/
void Graph::min_span_tree() {

	/* XXX: Reading queue */
	while (!_roads.empty()) {
		Vertex city_a = _roads.top().second.first;
		Vertex city_b = _roads.top().second.second;
		int cost = _roads.top().first;
		cout << city_a << "-- " << cost << " --" << city_b << '\n';
		_roads.pop();
	}

}

/***************************** MAIN function **********************************/
int main(void) {
	int num_cities, num_airports, num_roads;

	/* Get number of Cities */
	get_numbers(&num_cities);
	Graph g(num_cities);

	/* Get Cost of each Airport (city, cost) */
	get_numbers(&num_airports);
	while ( num_airports-- > 0 ) {
		int city, cost;
		get_numbers(&city, &cost);
		g.connect(0, city, cost);
	}

	/* Get Cost of each Road (city_a, city_b, cost) */
	get_numbers(&num_roads);
	while ( num_roads-- > 0 ) {
		int city_a, city_b, cost;
		get_numbers(&city_a, &city_b, &cost);
		g.connect(city_a, city_b, cost);
	}

	/* Generate a Minimum Spanning Tree */
	g.min_span_tree();
	cout << g << endl;

	return 0;
}
