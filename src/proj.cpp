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
#define INFINITY 0xFFFFFF

/******************** Data structures and their "methods" *********************/

/* Vertex structure */
typedef size_t Vertex;
Vertex new_vertex(int val) { return val; }

/* Edge Structure */
typedef pair< Vertex, int > Edge;
Edge new_edge(int cost, int city) { return make_pair(cost, city); }

/* Graph Structure */
class Graph {
	private:
		Status _status;
		int _total_cost;
		int _final_roads, _final_airports;

		vector< list<Edge> > _roads;

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

	_roads = vector< list<Edge> >(num_vertices + 1);

	_status = INSUFFICIENT;
	_total_cost = 0;
	_final_airports = 0;
	_final_roads = 0;

}
Graph::~Graph() { /* Nothing here */ }

/* Adds Edge */
void Graph::connect(Vertex u, Vertex v, int cost) {
	_roads[u].push_back(new_edge(cost, v));
    _roads[v].push_back(new_edge(cost, u));
}

/* Examines Graph */
ostream& operator<<(ostream& os, const Graph &graph) {
	switch ( graph.status() ) {
		/*
		case INSUFFICIENT:
			return os << "Insuficiente";
		*/
		default: {
			os << "Total Cost : " << graph.cost() << endl;
			os << "Airports : " << graph.num_airports() << endl;
			return os << "Roads : " << graph.num_roads();
		}
	}
}

/***************************** Prim's Algorithm *********************************/
void Graph::min_span_tree() {

    /* Build a Priority Queue */
    priority_queue< Edge, vector<Edge>, greater<Edge> > queue;

	/* Store Costs, Minimum Spanning Tree and Visited Cities */
	vector<int> cost(size(), INFINITY);
    vector<Vertex> result(size());
    vector<bool> visited(size(), false);

	/* Insert random City into the Priority Queue */
	queue.push(new_edge(0, 1));
	cost[1] = 0;

	/* Run through the graph always choosing the
	edge with the minimum cost (Greedy Approach) */
	while ( !queue.empty() ) {

		/* Get current city from queue */
        Vertex city = queue.top().second;
		visited[city] = true;
		queue.pop();

        /* Determine the next city with the minimum cost */
		list< Edge >::iterator i;
        for ( i = _roads[city].begin(); i != _roads[city].end(); i++ ) {

            Vertex adj_city = i->second;
            int adj_city_cost = i->first;

			/*  Choose the next Best City - Valid if the cost is less than
				the one registered and the new alternative is a road; or the
				cost is the same but the new alternative is a road.
			*/
			if ( visited[adj_city] == false && adj_city_cost < cost[adj_city] ) {
				cost[adj_city] = adj_city_cost;
				result[adj_city] = city;

				if ( adj_city == 0 ) {
					_final_airports++;
				} else if ( adj_city > 0 ) {
					_final_roads++;
				}

				_total_cost += cost[adj_city];
				queue.push(new_edge(cost[adj_city], adj_city));
			}

		}

    }

    /* Print Minimum Spanning Tree */
    for (Vertex k = 1; k < result.size(); k++) {
        printf("%zu <-> %zu\n", k, result[k]);
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
