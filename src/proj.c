#include <stdlib.h>
#include <stdio.h>

/******** Shared structures ********/
typedef unsigned char uint8_t;
typedef size_t Cost;
typedef size_t Vertex;

typedef struct airport {
    Vertex city;
    Cost cost;
} Airport;

typedef struct road {
    Vertex city1;
    Vertex city2;
    Cost cost;
} Road;


/******** Graph objects ********/
typedef enum Status {
	CORRECT = 0,
	INSUFFICIENT
} Status;

typedef struct {
	Cost cost;
	size_t num_roads, num_airports;
} Budget;

typedef struct graph {
	Status status;
	Budget budget;

    size_t num_cities;
    size_t num_airports, num_roads;
	size_t size_airports, size_roads;
    Airport *airports;
	Road *roads;
} Graph;

void init_graph(Graph *g, int num_cities) {
	g->status = CORRECT;
    g->num_cities = num_cities;
    g->num_airports = g->num_roads = 0;
    g->size_airports = g->size_roads = 0;
    g->airports = NULL;
	g->roads = NULL;
}
void destroy_graph(Graph *g) {
    free(g->airports); g->airports = NULL;
    free(g->roads); g->roads = NULL;
}

void set_num_airports(Graph *g, int num_airports) {
    g->num_airports = num_airports;
    g->airports = malloc(num_airports * sizeof(Airport));
}
void set_num_roads(Graph *g, int num_roads) {
    g->num_roads = num_roads;
    g->roads = malloc(num_roads * sizeof(Road));
}

void graph_insert_airport(Graph *g, Airport airport) {
    g->airports[g->size_airports++] = airport;
}
void graph_insert_road(Graph *g, Road road) {
    g->roads[g->size_roads++] = road;
}

const char* graph_print_budget(Graph *g) {
	switch (g->status) {
		case INSUFFICIENT:
			return "Insuficiente";
		default:
			printf("%zu\n%zu %zu", g->budget.cost, g->budget.num_airports, g->budget.num_roads);
	}
	return "";
}

/******** Sets ********/
struct {
	int *rank;
	Vertex *parent;
} Sets;

void make_set(Vertex u) {
	Sets.rank[u] = 0;
	Sets.parent[u] = u;
}
void init_sets(size_t num_cities) {
	Vertex city;

	num_cities++;
	Sets.rank   = malloc(num_cities * sizeof(*Sets.rank));
	Sets.parent = malloc(num_cities * sizeof(*Sets.parent));

    for (city = 0; city < num_cities; city++) {
        make_set(city);
    }
}
void destroy_sets(void) {
	free(Sets.rank); Sets.rank = NULL;
    free(Sets.parent); Sets.parent = NULL;
}
Vertex find_set(Vertex u) {
	if (u != Sets.parent[u]) { Sets.parent[u] = find_set(Sets.parent[u]); }
	return Sets.parent[u];
}
void merge_set(Vertex u, Vertex v) {
	if (u == v) { return; }

	if (Sets.rank[u] < Sets.rank[v]) { Sets.parent[u] = v; }
	else if (Sets.rank[u] > Sets.rank[v]) { Sets.parent[v] = u; }
	else {
		Sets.parent[v] = u;
		Sets.rank[u]++;
	}
}


/******** Algorithm stuff ********/
int comp_airports(const void *airport1, const void *airport2) {
    Airport a1 = *(const Airport*) airport1;
    Airport a2 = *(const Airport*) airport2;

    return a1.cost - a2.cost;
}
int comp_roads(const void *road1, const void *road2) {
    Road r1 = *(const Road*) road1;
    Road r2 = *(const Road*) road2;

    return r1.cost - r2.cost;
}

void mst_add_road(Road road, Budget *budget) {
	int city_cost = road.cost;
	Vertex city_a = road.city1;
	Vertex city_b = road.city2;

	Vertex set_a = find_set(city_a);
	Vertex set_b = find_set(city_b);

	if (set_a != set_b) {
		merge_set(set_a, set_b);
		budget->cost += city_cost;
		budget->num_roads++;
	}
}
void mst_add_airport(Airport airport, Budget *budget) {
	int city_cost = airport.cost;
	Vertex city = airport.city;
	Vertex sky = 0;

	Vertex set_a = find_set(sky);
	Vertex set_b = find_set(city);

	if (set_a != set_b) {
		merge_set(set_a, set_b);
		budget->cost += city_cost;
		budget->num_airports++;
	}
}

int can_add_road(Graph *g, size_t i_airports, size_t i_roads) {
	return g->roads[i_roads].cost <= g->airports[i_airports].cost
		&& i_roads < g->size_roads;
}
int can_add_airport(Graph *g, size_t i_airports, size_t i_roads) {
	return g->roads[i_roads].cost > g->airports[i_airports].cost
		&& i_airports < g->size_airports;
}

void min_span_tree(Graph *g, Budget *budget, uint8_t *visited) {
	size_t i_airports = 0, i_roads = 0;

	init_sets(g->num_cities);
	while (i_roads < g->size_roads || i_airports < g->size_airports) {
		if (can_add_road(g, i_airports, i_roads)) {
			mst_add_road(g->roads[i_roads++], budget);
		} else if (can_add_airport(g, i_airports, i_roads)) {
			if (visited != NULL) {
				mst_add_airport(g->airports[i_airports], budget);
				visited[g->airports[i_airports].city] = 1;
			}
			i_airports++;
		} else {
			break; /* Nothing works and code somehow fell through here */
		}
	}
}

void kruskal(Graph *g) {
	Budget roads = { 0, 0, 0 }, roads_airports = { 0, 0, 0 };
	uint8_t *visited = calloc(g->num_cities+1, sizeof(uint8_t));
	Vertex city;

    init_sets(g->num_cities);

    qsort(g->airports, g->num_airports, sizeof(Airport), comp_airports);
    qsort(g->roads, g->num_roads, sizeof(Road), comp_roads);

	/* First MST without airports */
    min_span_tree(g, &roads, NULL);

	/* Second MST with airports*/
	min_span_tree(g, &roads_airports, visited);

	if (roads.num_roads == g->num_cities-1) {
		g->budget = (roads.cost <= roads_airports.cost) ? roads : roads_airports;
	} else {
		g->budget = roads_airports;

		for (city = 1; city <= g->num_cities; city++) {
			if (!visited[city]) {
				g->status = INSUFFICIENT;
				break;
			}
		}
	}

	free(visited); visited = NULL;
    destroy_sets();
}

/*** MAIN ***/
#define get_number(a) scanf("%zu", a)
#define get_2_numbers(a, b) scanf("%zu %zu", a, b)
#define get_3_numbers(a, b, c) scanf("%zu %zu %zu", a, b, c)

void graph_from_input(Graph *g) {
    size_t num_cities, i;

    get_number(&num_cities);
    init_graph(g, num_cities);

    get_number(&g->num_airports);
    set_num_airports(g, g->num_airports);
    for (i = 0; i < g->num_airports; i++) {
		Airport airport;
        get_2_numbers(&airport.city, &airport.cost);
        graph_insert_airport(g, airport);
    }

    get_number(&g->num_roads);
    set_num_roads(g, g->num_roads);
    for (i = 0; i < g->num_roads; i++) {
		Road road;
        get_3_numbers(&road.city1, &road.city2, &road.cost);
        graph_insert_road(g, road);
    }
}

int main() {
    Graph g;
	graph_from_input(&g);

    kruskal(&g);
	printf("%s\n", graph_print_budget(&g));

	destroy_graph(&g);
    return 0;
}
