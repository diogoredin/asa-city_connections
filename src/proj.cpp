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
		priority_queue< Edge, vector<Edge>, greater<Edge> > _airports;
		vector<int> _rank;
		vector<bool> _visited;
		vector<Vertex> _parent;

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
	_visited.resize(num_vertices + 1);

	_status = CORRECT;
	_num_vertices = num_vertices;
	_total_cost = 0;
	_final_airports = 0;
	_final_roads = 0;

}
Graph::~Graph() { /* Nothing here */ }

/* Adds Edge */
void Graph::connect(Vertex u, Vertex v, int cost) {

	if ( u != 0 ) {
		_roads.push(new_edge(u, v, cost));
	} else {
		_airports.push(new_edge(u, v, cost));
	}

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

/* Generates a Minimum Spanning Tree */
void Graph::min_span_tree() {

	for ( Vertex city = 1; city < _num_vertices; city++ ) {
		make_set(city);
	}

	while ( _roads.size() > 0 || _airports.size() > 0 ) {

		int cost_airport = 1000;
		int cost_road = 1000;
		int city_a = 0;
		int city_b = 0;
		int city_c = 0;

		if ( _roads.size() > 0 ) {
			city_a = _roads.top().second.first;
			city_b = _roads.top().second.second;
			cost_road = _roads.top().first;
			if ( _visited[city_a] ) { _roads.pop(); continue; }
		}

		if ( _airports.size() > 0 ) {
			city_c = _airports.top().second.second;
			cost_airport = _airports.top().first;
			if ( _visited[city_c] ) { _airports.pop(); continue; }
		}

		if ( cost_road < cost_airport || cost_road == cost_airport ) {

			Vertex set_a = find_set(city_a);
			Vertex set_b = find_set(city_b);

			if ( set_a != set_b ) {
				merge_set(set_a, set_b);

				_final_roads++;
				_total_cost += cost_road;

				_visited[city_a] = true;
			}

			_roads.pop();

		}

		if ( cost_airport < cost_road ) {

			_final_airports++;
			_total_cost += cost_airport;

			_visited[city_c] = true;
			_airports.pop();
		}

	}

	/* Define Graph Status */
	if ( _final_airports + _final_roads != (int) _num_vertices-1 ) {
		//_status = INSUFFICIENT;
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
