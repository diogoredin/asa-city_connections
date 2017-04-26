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

#define INF 0x3f3f3f3f
#define UDF -1

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
		void connect(size_t u, size_t v, int cost);

		/* Operator overrides */
		int& operator[](size_t city) { return _airport_costs[city]; }
		friend ostream& operator<<(ostream& os, const Graph &graph);

		/* Magic methods */
		void min_span_tree();
};

/* Builds Graph */
Graph::Graph(int num_vertices) {

	_status = INSUFFICIENT;
	_cities = vector< list<Vertex> >(num_vertices + 1);
	_airport_costs = vector<int>(num_vertices + 1);

}
Graph::~Graph() { /* Nothing here */ }

void Graph::connect(size_t u, size_t v, int cost) {
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

/***************************** Prim's Algorithm *********************************/
void Graph::min_span_tree() {

    /* Build Priority Queue */
    priority_queue< Vertex, vector<Vertex>, greater<Vertex> > queue;

	/* Start from som Random City (0) */
	int source = 0;

	/* Store Costs */
	vector<int> cost(size(), INF);

    /* Store MST */
    vector<int> result(size(), UDF);

    /* Keep track of cities visited */
    vector<bool> visited(size(), false);

	/* Insert Source City in the Priority Queue and Init its cost as 0 */
    queue.push(make_pair(0, source));
	cost[source] = 0;

	/* Go through all Cities */
	while ( !queue.empty() ) {

        int city_u = queue.top().second;
        queue.pop();
        visited[city_u] = true;
 
        /* Iterate over all adjacent Cities of U */
		list< Vertex >::iterator i;
        for ( i = _cities[city_u].begin(); i != _cities[city_u].end(); ++i ) {

			/* Get Number of the City and Weight of the Current Ajacent */
            int city_v = i->first;
            int city_cost = _airport_costs[city_v];
 
            /*  If City V hasnt been visited and the weight of (u,v) is smaller 
			than current key of V */
            if ( visited[city_v] == false && cost[city_v] > city_cost ) {

                /* Updating Cost of V */
                cost[city_v] = city_cost;
                queue.push(make_pair(cost[city_v], city_v));
                result[city_v] = city_u;

			}

        }
    }

    /* Print MST */
    for (size_t i = 1; i < result.size(); ++i)
        printf("%d / %zu\n", result[i], i);

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
	g.min_span_tree();
	cout << g << endl;

	return 0;
}
