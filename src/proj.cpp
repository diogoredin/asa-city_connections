/******************************* ASA - Grupo 20 *******************************
** António Sarmento - 77906
** Diogo Redin      - 84711
*******************************************************************************/

/* Libraries. Only the bare minimum, no need for clutter */
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

/****************************** auxy functions *********************************/
void get_numbers(int &a) { cin >> a; }
void get_numbers(int &a, int &b) { cin >> a >> b; }
void get_numbers(int &a, int &b, int &c) { cin >> a >> b >> c; }

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
struct LowerEdge {
	bool operator()(const Edge& edge_a, const Edge& edge_b) const {

		if (edge_a.first < edge_b.first) { return true; }
		if (edge_a.first > edge_b.first) { return false; }
		if (edge_a.second.first > edge_b.second.first) { return true; }
		return false;

	}
} lower_edge;

/* Graph Structure */
class Graph {
	private:
		Status _status;
		size_t _num_vertices;
		Budget _budget;

		vector<Edge> _edges;
		int *_rank;
		Vertex *_parent;

	public:
		Graph(int num_vertices);
		~Graph();

		/* Class attribute methods */
		int cost()          const { return _budget.cost; }
		int num_roads()     const { return _budget.num_roads; }
		int num_airports()  const { return _budget.num_airports; }
		void reserve(size_t val)  { _edges.reserve(val); }
		size_t size()       const { return _num_vertices; }
		Status status()     const { return _status; }

		/* Class functional methods */
		void connect(Vertex u, Vertex v, int city_cost) {
			_edges.push_back(new_edge(u, v, city_cost));
		}
		void re_set() {
			for ( Vertex city = 0; city <= size(); city++ ) {
				make_set(city);
			}
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
		void min_span_tree(Budget &roads, vector<bool> &visited);
		void solve();
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
void Graph::min_span_tree(Budget &roads, vector<bool> &visited) {
	re_set(); /* Resetting Graph's ranks and parents */

	vector<Edge>::const_iterator it;
	for ( it = _edges.begin() ; it != _edges.end(); it++ ) {
		int city_cost = (*it).first;
		Vertex city_a = (*it).second.first;
		Vertex city_b = (*it).second.second;

		if (visited.empty() && city_a == AIRPORT) { continue; }

		Vertex set_a = find_set(city_a);
		Vertex set_b = find_set(city_b);

		if ( set_a != set_b ) {

			merge_set(set_a, set_b);

			roads.cost += city_cost;

			if (!visited.empty()) {
				visited[city_b] = true;
				if (city_a != AIRPORT) {
					roads.num_roads++;
				} else {
					roads.num_airports++;
				}
			} else {
				roads.num_roads++;
			}

		}
	}
}

void Graph::solve(void) {

	/* Shared variables */
	vector<bool> visited;
	Budget roads = { 0, 0, 0 }, roads_airports = { 0, 0, 0 };

	/* Sorting our vector ONCE */
	sort(_edges.begin(), _edges.end(), lower_edge);

	/* MST has no airports */
	min_span_tree(roads, visited);

	/* MST has airports */
	visited = vector<bool>(size() + 1, false);
	min_span_tree(roads_airports, visited);

	/* Taking road costs */
	if (roads.num_roads == (int) size()-1) {
		_budget = (roads.cost <= roads_airports.cost) ? roads : roads_airports;
	} else {
		_budget = roads_airports;

		for (Vertex city = 1; city <= size(); city++) {
			if (!visited[city]) {
				_status = INSUFFICIENT;
				break;
			}
		}
	}
}

/***************************** MAIN function **********************************/
int main(void) {
	int num_cities, num_airports, num_roads;

	/* Get number of Cities */
	get_numbers(num_cities);
	Graph g(num_cities);

	/* Get Cost of each Airport (city, cost) */
	get_numbers(num_airports);
	g.reserve(num_airports);
	while ( num_airports-- > 0 ) {
		int city, cost;
		get_numbers(city, cost);
		g.connect(AIRPORT, city, cost);
	}

	/* Get Cost of each Road (city_a, city_b, cost) */
	get_numbers(num_roads);
	g.reserve(num_airports + num_roads);
	while ( num_roads-- > 0 ) {
		int city_a, city_b, cost;
		get_numbers(city_a, city_b, cost);
		g.connect(city_a, city_b, cost);
	}

	/* Solve the graph */
	g.solve();

	cout << g << endl;

	return 0;
}
