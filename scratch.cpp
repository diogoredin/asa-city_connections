/******************************* ASA - Grupo 20 *******************************
** António Sarmento - 77906
** Diogo Redin      - 84711
*******************************************************************************/

/* Libraries. Only the bare minimum, no need for clutter */
#include <stdio.h>
#include <iostream>
#include <map>
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
typedef int Vertex;
#define new_vertex(a) a
#define next_vertex(a) a + 1

/* Global queue */
std::vector<Vertex> queue;
#define enqueue(a) queue.push_back(a)
#define dequeue()  queue.back(); queue.pop_back()
#define is_empty() queue.empty()

/* Graph Structure */
class Graph {
	private:
		unsigned char _status;
		int _nr_cities;
		int _possible_roads, _possible_airports;

		int _total_cost;
		int _final_roads, _final_airports;

		// Main structure for our Data
		std::vector<Vertex>		_city;
		std::map<Vertex, int>	_road_cost;
		std::map<Vertex, int>	_airport_cost;

	public:
		Graph(int nr_cities);
		~Graph();
		
		// Set Functions
		void set_status(unsigned char val) {
			this->_status = val;
		}

		void set_nr_cities(int val) {
			this->_nr_cities = val;
		}

		void set_possible_roads(int val) {
			this->_possible_roads = val;
		}

		void set_possible_airports(int val) {
			this->_possible_airports = val;
		}

		void set_total_cost(int val) {
			this->_total_cost = val;
		}

		void set_final_roads(int val) {
			this->_final_roads = val;
		}

		void set_final_airports(int val) {
			this->_final_airports = val;
		}

		void set_city(int index, int val) {
			this->_city[index] = val;
		}

		void set_road_cost(int index, int val) {
			this->_road_cost[index] = val;
		}

		void set_airport_cost(int index, int val) {
			this->_airport_cost[index] = val;
		}

		// Get Functions
		void get_status() {
			return this->_status;
		}

		void get_nr_cities() {
			return this->_nr_cities;
		}

		void get_possible_roads() {
			return this->_possible_roads;
		}

		void get_possible_airports() {
			return this->_possible_airports;
		}

		void get_total_cost() {
			return this->_total_cost;
		}

		void get_final_roads() {
			return this->_final_roads;
		}

		void get_final_airports() {
			return this->_final_airports;
		}

		void get_city(int index) {
			return this->_city[index];
		}

		void get_road_cost(int index) {
			return this->_road_cost[index];
		}

		void get_airport_cost(int index) {
			return this->_airport_cost[index];
		}

};

/* Build Graph */
Graph::Graph(int nr_cities) {

	_status = INSUFFICIENT;
	_nr_cities = nr_cities;

	_city = std::vector<Vertex>(nr_cities + 1);

	_possible_roads = 0;
	_possible_airports = 0;

	_total_cost = 0;
	_final_roads = 0;
	_final_airports = 0;

}
Graph::~Graph() { /* Nothing here */ }

/* Examines Graph */
std::ostream& operator<<(std::ostream& os, const Graph &graph) {
	switch ( graph.get_status() ) {
		case INSUFFICIENT:
			return os << "Insuficiente";

		default: {
			os << graph.get_total_cost() << std::endl;
			return os << graph.get_final_airports() << " " << graph.get_final_roads();
		}
	}
}

/************************** Algorithm-based functions *****************************/

/****************************** Sort Algorithm ***********************************/

void sort_costs(Graph g) {

	// Go through all cities and choose minimum cost possible for each (greedy approach)
    for ( int city = 0; city <= g.get_nr_cities(); city++ ) {

		if ( g.get_city() == 0 ) {

			// Prefer Roads over Airports unless they are cheaper
			if ( g.get_road_cost(city) <= g.get_airport_cost(city) ) {
				set_city(city, minimized_cost);
				//set_city(road_to(city), 0);
			}
			
			else if ( g.get_road_cost(city) > g.get_airport_cost(city) ) {
				set_city(city, minimized_cost);
			}

		}

	}

}

/***************************** Prim's Algorithm *********************************/
/*void Graph::min_span_tree() {

	// Sort the costs indexes
	sort_costs();

	// Goes through all adjacent cities (in our case every city is a possibility)
	// Prim's algorithm is greedy so we always choose a minimum cost vertex
	// Because of our prior sorting we just have to dequeue a vertex
	for (int c = 1; c <= _nr_cities; c++ ) {

		// City has no connection decided yet (visited), we haven't reached the limit of roads
		// or airports, and it's possible to build at least a road or an airport
		if ( (_first[c] != -1 ) && 
			( _possible_roads > _final_roads || _possible_airports > _final_airports ) &&
			( _road_cost[c] != -1 || !(_airport_cost[c] == -1 && _airport_cost[c+1] == -1) ) ) {

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

			// It's connection has been decided!
			_first[c] = -1;
		
		}

	}

}*/

/***************************** MAIN function **********************************/
int main(void) {

	int num_cities, num_airports, num_roads;

	// Get number of cities
	get_numbers(&num_cities);
	Graph g(num_cities);

	// Get number of possible airports
	get_numbers(&num_airports);
	g.set_possible_airports(num_airports);

	// Get Cost of each possible Airport (city, cost)
	while ( num_airports-- > 0 ) {
		Vertex city;
		int cost;

		get_numbers(&city, &cost);
		g.set_airport_cost(city, cost);
	}

	// Get number of possible roads
	get_numbers(&num_roads);
	g.set_possible_roads(num_roads);

	// Get Cost of each possible Road (city_a, city_b, cost)
	while ( num_roads-- > 0 ) {
		Vertex city_a, city_b;
		int cost;

		get_numbers(&city_a, &city_b, &cost);
		g.set_road_cost(city_a, cost);
		g.set_road_cost(city_b, cost);
	}

	// Apply Sort & Prim's Algorithm
	sort_costs(g);
	//min_span_tree(g);

	std::cout << g << std::endl;

	return 0;
}
