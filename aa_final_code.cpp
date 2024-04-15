#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_VERTICES 100

typedef struct {
    int src;
    int dest;
} Edge;

typedef struct {
    int V;
    int E;
    Edge* edges;
} Graph;

void back_edge_marking(Graph* graph, int v, bool* visited, bool* onStack, bool* isRemoved) {
    visited[v] = true;
    onStack[v] = true;

    int i;
    for (i = 0; i < graph->E; i++) {
        int dest = graph->edges[i].dest;
        if (graph->edges[i].src == v) {
            if (!visited[dest]) {
                back_edge_marking(graph, dest, visited, onStack, isRemoved);
            } else if (onStack[dest]) {
                isRemoved[i] = true;
            }
        }
    }

    onStack[v] = false;
}

void findLinearArrangement(Graph* graph) {
    int V = graph->V;
    int s1[V], s2[V];
    int s1_count = 0, s2_count = 0;
    bool removed[V];
    int i, j, u;

    for (i = 0; i < V; i++)
        removed[i] = false;

    printf("edges \n");
    for (j = 0; j < graph->E; j++) {
        printf(" src %d dest %d \n", graph->edges[j].src, graph->edges[j].dest);
    }

    while (true) {
        bool has_sink = false;
        bool has_source = false;
        bool has_node = false;

        for (i = 0; i < V; i++) {
            if (!removed[i]) {
                bool is_source = true;
                for (j = 0; j < graph->E; j++) {
                    if (!removed[graph->edges[j].src] && graph->edges[j].dest == i) {
                      //  printf(" src %d dest %d \n", graph->edges[j].src, graph->edges[j].dest);
                        is_source = false;
                        break;
                    }
                }
                if (is_source) {
                    has_source = true;
                    s2[s2_count++] = i;
                    removed[i] = true;
                }
            }
        }

        for (i = 0; i < V; i++) {
            if (!removed[i]) {
                bool is_sink = true;
                for (j = 0; j < graph->E; j++) {
                    if (!removed[graph->edges[j].dest] && graph->edges[j].src == i) {
                        is_sink= false;
                        break;
                    }
                }
                if (is_sink) {
                    has_sink = true;
                    s1[s1_count++] = i;
                    removed[i] = true;
                }
            }
        }

        int max_out_degree = -1;
        for (i = 0; i < V; i++) {
            if (!removed[i]) {
                has_node = true;
                int out_degree = 0;
                for (j = 0; j < graph->E; j++) {
                    if (!removed[i] && graph->edges[j].src == i) {
                        out_degree++;
                    }
                }
                if (out_degree > max_out_degree) {
                    max_out_degree = out_degree;
                    u = i;
                }
            }
        }

        if (!has_sink && !has_source && !has_node) {
            break;
        }
       if(!removed[u])
       {
            s1[s1_count++] = u;
          removed[u] = true;
       }
       
    }

    printf("Linear Arrangement:\n");
    for (i = 0; i < s1_count; i++) {
        printf("%d ", s1[i]);
    }
     printf("\n");
    for (i = s2_count - 1; i >= 0; i--) {
        printf("%d ", s2[i]);
    }
    printf("\n");
}

void findFeedbackArcSet(Graph* graph) {
    int V = graph->V;
    bool visited[V];
    bool onStack[V];
    bool isRemoved[graph->E];
    int i;

    for (i = 0; i < V; i++) {
        visited[i] = false;
        onStack[i] = false;
    }
    for (i = 0; i < graph->E; i++) {
        isRemoved[i] = false;
    }

    for (i = 0; i < V; i++) {
        if (!visited[i]) {
            back_edge_marking(graph, i, visited, onStack, isRemoved);
        }
    }

    printf("Feedback Arc Set:\n");
    for (i = 0; i < graph->E; i++) {
        if ((isRemoved[i] && graph->edges[i].src != graph->edges[i].dest) || ((graph->edges[i].src == graph->edges[i].dest) && graph->edges[i].src != 0)) {
            printf("%d -> %d\n", graph->edges[i].src, graph->edges[i].dest);
            isRemoved[i] = true;
        }
    }

    printf("Edges not removed:\n");
    for (i = 0; i < graph->E; i++) {
        if (!isRemoved[i]) {
            printf("%d -> %d\n", graph->edges[i].src, graph->edges[i].dest);
        }
    }
}

int main() {
    int V = 5;  // Number of vertices
    int E = 6;  // Number of edges
    int edge_list[][2] = {
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 4},
        {4, 1},
        {2, 0}
    };

    Graph graph;
    graph.V = V;
    graph.E = E;
    graph.edges = (Edge*)malloc(E * sizeof(Edge));

    // Add edges to the graph
    for (int i = 0; i < E; i++) {
        graph.edges[i].src = edge_list[i][0];
        graph.edges[i].dest = edge_list[i][1];
    }

    findFeedbackArcSet(&graph);
    findLinearArrangement(&graph);

    free(graph.edges);  // Free dynamically allocated memory

    return 0;
}
