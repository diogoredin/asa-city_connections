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

/* Kind */
#define AIRPORT 0

/******************** Data structures and their "methods" *********************/

/* Vertex structure */
typedef size_t Vertex;
Vertex new_vertex(int val) { return val; }

/* Edge Structure */
typedef pair< int, pair<Vertex, Vertex> > Edge;

/* Build Edge */
Edge new_edge(Vertex city_a, Vertex city_b, int cost) {
	return make_pair(cost, make_pair(city_a, city_b));
}

/* Compare Edges */
struct GreaterEdge { // Returns true if edge_a comes after edge_b
	bool operator()(const Edge& edge_a, const Edge& edge_b) const {

		if ( edge_a.first == edge_b.first ) {
			return edge_a.second.first == AIRPORT;
		}
		return ( edge_a.first > edge_b.first );

	}
};

/* Graph Structure */
class Graph {
	private:
		Status _status;
		size_t _num_vertices;
		int _total_cost;
		int _final_roads, _final_airports;

		priority_queue< Edge, vector<Edge>, GreaterEdge > _edges;
		vector<int> _rank;
		vector<Vertex> _parent;

	public:
		Graph(int num_vertices);
		~Graph();

		/* Class attribute methods */
		int cost()             const { return _total_cost; }
		int num_roads()        const { return _final_roads; }
		int num_airports()     const { return _final_airports; }
		Status status()        const { return _status; }

		/* Class functional methods */
		void print_queue(priority_queue< Edge, vector<Edge>, GreaterEdge > queue);
		void connect(Vertex u, Vertex v, int cost) {
			_edges.push(new_edge(u, v, cost));
		}
		void make_set(Vertex u) {
			_rank[u] = 0;
			_parent[u] = u;
		}

		Vertex find_set(Vertex u) {
			if (u != _parent[u]) { _parent[u] = find_set(_parent[u]); }
			return _parent[u];
		}

		void merge_set(Vertex u, Vertex v) {
			if (u == v) { return; }

			if (_rank[u] < _rank[v]) { _parent[u] = v; }
			else if (_rank[u] > _rank[v]) { _parent[v] = u; }
			else {
				_parent[v] = u;
				_rank[u]++;
			}
		}

		/* Operator overrides */
		friend ostream& operator<<(ostream& os, const Graph &graph);

		/* Algorithmic methods */
		void min_span_tree();
};

/* Builds Graph */
Graph::Graph(int num_vertices) {

	_rank.resize(num_vertices + 1);
	_parent.resize(num_vertices + 1);

	_status = CORRECT;
	_num_vertices = num_vertices;
	_total_cost = 0;
	_final_airports = 0;
	_final_roads = 0;

}
Graph::~Graph() { /* Nothing here */ }

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

void Graph::print_queue(priority_queue< Edge, vector<Edge>, GreaterEdge > queue) {
	int cost;
	Vertex city_a, city_b;

	for ( ; !queue.empty(); queue.pop() ) {
		cost = queue.top().first;
		city_a = queue.top().second.first;
		city_b = queue.top().second.second;

		cout << city_a << " <-- " << cost << " --> " << city_b << endl;
	}
}

/* Generates a Minimum Spanning Tree */
void Graph::min_span_tree(void) {

	priority_queue< Edge, vector<Edge>, GreaterEdge > roads;

	/* Shared variables */
	int cost;
	Vertex city_a, city_b;

	/* MST has no airports */
	int num_roads = 0;
	int total_roads_cost = 0;
	for ( Vertex city = 1; city < _num_vertices; city++ ) {
		make_set(city);
	}

	for ( ; _edges.size() > 1; _edges.pop() ) {
		roads.push(_edges.top()); /* FIXME: Stop using this hack and clone the queue */

		cost = _edges.top().first;
		city_a = _edges.top().second.first;
		city_b = _edges.top().second.second;

		if (city_a == AIRPORT) { continue; }

		Vertex set_a = find_set(city_a);
		Vertex set_b = find_set(city_b);

		if ( set_a != set_b ) {

			merge_set(set_a, set_b);

			num_roads++;
			total_roads_cost += cost;

		}
	}
	roads.push(_edges.top()); /* Pushing final value */

	/* MST has airports */
	int num_edges = 0;
	int num_airports = 0;
	int total_edges_cost = 0;
	for ( Vertex city = 0; city < _num_vertices; city++ ) {
		make_set(city);
	}

	for ( ; roads.size() > 1; roads.pop() ) {
		cost = roads.top().first;
		city_a = roads.top().second.first;
		city_b = roads.top().second.second;

		Vertex set_a = find_set(city_a);
		Vertex set_b = find_set(city_b);

		if ( set_a != set_b ) {

			merge_set(set_a, set_b);

			if (city_a == AIRPORT) {
				num_airports++;
			} else {
				num_edges++;
			}
			total_edges_cost += cost;

		}
	}

	/* Taking road costs */
	if (total_roads_cost <= total_edges_cost) {
		_final_roads = num_roads;
		_total_cost = total_roads_cost;
	} else {
		_final_roads = num_edges;
		_final_airports = num_airports;
		_total_cost = total_edges_cost;
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
		g.connect(AIRPORT, city, cost);
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

	/* Define Graph Status */
	//if ( g.num_roads() + g.num_airports() != (int) _num_vertices-1 ) {
		//_status = INSUFFICIENT;
	//}

	cout << g << endl;

	return 0;
}
