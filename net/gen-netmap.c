#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_STRING_LENGTH 50

typedef struct {
    char source[MAX_STRING_LENGTH];
    char target[MAX_STRING_LENGTH];
} Edge;

typedef struct {
    char nodes[MAX_NODES][MAX_STRING_LENGTH];
    Edge edges[MAX_NODES];
    int numNodes;
    int numEdges;
} Graph;

void parseCSV(const char* filename, Graph* graph) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%49[^,],%49[^,]\n", graph->edges[graph->numEdges].source, graph->edges[graph->numEdges].target) == 2) {
        int foundSource = 0, foundTarget = 0;

        for (int i = 0; i < graph->numNodes; i++) {
            if (strcmp(graph->nodes[i], graph->edges[graph->numEdges].source) == 0) {
                foundSource = 1;
            }

            if (strcmp(graph->nodes[i], graph->edges[graph->numEdges].target) == 0) {
                foundTarget = 1;
            }
        }

        if (!foundSource) {
            strcpy(graph->nodes[graph->numNodes++], graph->edges[graph->numEdges].source);
        }

        if (!foundTarget) {
            strcpy(graph->nodes[graph->numNodes++], graph->edges[graph->numEdges].target);
        }

        graph->numEdges++;
    }

    fclose(file);
}

void printGraph(const Graph* graph) {
    printf("Nodes: ");
    for (int i = 0; i < graph->numNodes; i++) {
        printf("%s ", graph->nodes[i]);
    }

    printf("\nEdges:\n");
    for (int i = 0; i < graph->numEdges; i++) {
        printf("%s -> %s\n", graph->edges[i].source, graph->edges[i].target);
    }
}

int main() {
    Graph graph;
    graph.numNodes = 0;
    graph.numEdges = 0;

    parseCSV("./netstat.csv", &graph);
    printGraph(&graph);

    return 0;
}
