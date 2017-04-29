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
Edge new_edge(Vertex city_a, Vertex city_b, int cost) {
	return make_pair(cost, make_pair(city_a, city_b));
}

/* Project Budget Structure */
typedef struct {
	int cost;
	int num_roads, num_airports;
} Budget;

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
		Budget _budget;

		priority_queue< Edge, vector<Edge>, GreaterEdge > _edges;
		int *_rank;
		Vertex *_parent;

	public:
		Graph(int num_vertices);
		~Graph();

		/* Class attribute methods */
		int cost()         const { return _budget.cost; }
		int num_roads()    const { return _budget.num_roads; }
		int num_airports() const { return _budget.num_airports; }
		Status status()    const { return _status; }
		size_t size()      const { return _num_vertices; }

		/* Class functional methods */
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

	_rank = new int[num_vertices + 1];
	_parent = new Vertex[num_vertices + 1];

	_status = CORRECT;
	_num_vertices = num_vertices;

}
Graph::~Graph() {
	delete [] _rank;
	delete [] _parent;
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

/* Generates a Minimum Spanning Tree */
void Graph::min_span_tree(void) {

	priority_queue< Edge, vector<Edge>, GreaterEdge > edges2;

	/* Shared variables */
	int cost;
	Vertex city_a, city_b;
	Budget roads = { 0, 0, 0 }, roads_airports = { 0, 0, 0 };

	/* MST has no airports */
	for ( Vertex city = 1; city <= _num_vertices; city++ ) {
		make_set(city);
	}

	for ( ; !_edges.empty(); _edges.pop() ) {
		edges2.push(_edges.top());

		cost = _edges.top().first;
		city_a = _edges.top().second.first;
		city_b = _edges.top().second.second;

		if (city_a == AIRPORT) { continue; }

		Vertex set_a = find_set(city_a);
		Vertex set_b = find_set(city_b);

		if ( set_a != set_b ) {

			merge_set(set_a, set_b);

			roads.num_roads++;
			roads.cost += cost;

		}
	}

	/* MST has airports */
	bool *visited = new bool[_num_vertices + 1];
	for ( Vertex city = 0; city <= _num_vertices; city++ ) {
		make_set(city);
	}

	for ( ; !edges2.empty(); edges2.pop() ) {
		cost = edges2.top().first;
		city_a = edges2.top().second.first;
		city_b = edges2.top().second.second;

		Vertex set_a = find_set(city_a);
		Vertex set_b = find_set(city_b);

		if ( set_a != set_b ) {

			merge_set(set_a, set_b);

			if (city_a == AIRPORT) {
				roads_airports.num_airports++;
			} else {
				roads_airports.num_roads++;
			}
			roads_airports.cost += cost;
			visited[city_b] = true;

		}
	}

	/* Taking road costs */
	if (roads.num_roads == (int) _num_vertices-1) {
		if (roads.cost <= roads_airports.cost) {
			_budget = roads;
		} else {
			_budget = roads_airports;
		}
	} else {
		for (Vertex city = 1; city <= _num_vertices; city++) {
			if (!visited[city]) {
				_status = INSUFFICIENT;
			}
		}
		_budget = roads_airports;
	}

	/* Cleaning up */
	delete [] visited;
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
