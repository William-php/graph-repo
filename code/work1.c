#include <stdio.h>
#include <stdlib.h>

typedef struct Graph {
    int vertex;
    int edges;
    int **adjMatrix;
} Graph;

int **createMatrix(int row, int column) {
    int **matrix = (int**)malloc(row * sizeof(int*));
    for (int i = 0; i < row; i++) {
        matrix[i] = (int*)malloc(column * sizeof(int));
        for (int j = 0; j < column; j++) matrix[i][j] = 0;
    }
    return matrix;
}

Graph* createGraph(int v) {
    Graph *graph = (Graph*)malloc(sizeof(Graph));
    graph->vertex = v;
    graph->edges = 0;
    graph->adjMatrix = createMatrix(v, v);
    return graph;
}

void addEdges(Graph *graph, int v, int w) {
    if (!graph->adjMatrix[v][w]) {
        graph->adjMatrix[v][w] = 1;
        graph->adjMatrix[w][v] = 1;
        graph->edges++;
    }
}

void removeEdges(Graph *graph, int v, int w) {
    if (graph->adjMatrix[v][w]) {
        graph->adjMatrix[v][w] = 0;
        graph->adjMatrix[w][v] = 0;
        graph->edges--;
    }
}

void printWalk(int *walk, int length) {
    for (int i = 0; i < length; i++) {
        printf("|%d|", walk[i]);
        if (i < length - 1) printf(" -> ");
    }
    printf("\n");
}

void findWalk(Graph *graph, int current, int *walk, int length, int **visitedEdges) {
    // Exibe o passeio atual
    printWalk(walk, length);
    if (current == graph->vertex - 1) return;
    // Percorre todos os vértices adjacentes
    for (int i = 0; i < graph->vertex; i++) {
        if (graph->adjMatrix[current][i] && !visitedEdges[current][i]) {
            visitedEdges[current][i] = 1;
            visitedEdges[i][current] = 1;
            walk[length] = i;
            findWalk(graph, i, walk, length + 1, visitedEdges);
            visitedEdges[current][i] = 0;            
        }
    }
}

void getWalk(Graph *graph) {
    int **visitedEdges = createMatrix(graph->vertex, graph->vertex);    
    int *walk = (int*)malloc(graph->vertex * sizeof(int));
    for (int i = 0; i < graph->vertex; i++) {
        // Inicializa o array `walk` com o vértice inicial
        walk[0] = i;
        findWalk(graph, i, walk, 1, visitedEdges);
    }
    free(walk);
}

void printPath(int *path, int length) {
    for (int i = 0; i < length; i++) {
        printf("|%d|", path[i]);
        if (i < length - 1) printf(" -> ");
    }
    printf("\n");
}

void findPath(Graph * graph, int current, int *path, int length, int **visitedEdges) {
    if (length > 1) printPath(path, length);

    for (int i = current; i < graph->vertex; i++) {
        if (graph->adjMatrix[current][i] && !visitedEdges[current][i]) {
            visitedEdges[current][i] = 1;
            visitedEdges[i][current] = 1;
            path[length] = i;
            findPath(graph, i, path, length + 1, visitedEdges);
            return;
        }
    }
}

void getPath(Graph * graph) {
    int **visitedEdges = createMatrix(graph->vertex, graph->vertex);
    int *path = (int*)malloc(graph->vertex * sizeof(int));
    for (int i = 0; i < graph->vertex; i++) {
        path[0] = i;
        findPath(graph, i, path, 1, visitedEdges);
        visitedEdges = createMatrix(graph->vertex, graph->vertex);
    }
    free(path);
}

void findTrail(Graph *graph, int current, int *trail, int length, int **visitedEdges) {
    if (length > 1) {
        printWalk(trail, length); // reaproveita a função de impressão
    }

    for (int i = 0; i < graph->vertex; i++) {
        if (graph->adjMatrix[current][i] && !visitedEdges[current][i]) {
            visitedEdges[current][i] = 1;
            visitedEdges[i][current] = 1;
            trail[length] = i;
            findTrail(graph, i, trail, length + 1, visitedEdges);
            visitedEdges[current][i] = 0;
            visitedEdges[i][current] = 0;
        }
    }
}

void getTrail(Graph *graph) {
    int **visitedEdges = createMatrix(graph->vertex, graph->vertex);
    int *trail = (int*)malloc(graph->vertex * sizeof(int));

    printf("Trails:\n");
    for (int i = 0; i < graph->vertex; i++) {
        trail[0] = i;
        findTrail(graph, i, trail, 1, visitedEdges);
    }

    free(trail);
    for (int i = 0; i < graph->vertex; i++) {
        free(visitedEdges[i]);
    }
    free(visitedEdges);
}

int isEulerian(Graph *graph) {
    

    for (int i = 0; i < graph->vertex; i++) {
        int degree = 0;
        for (int j = 0; j < graph->vertex; j++) if (graph->adjMatrix[i][j]) degree++;
        if (degree % 2 != 0) return 0;
    }
    return 1;
}

void main() {
    Graph *graph = createGraph(3);
    addEdges(graph, 0, 1);
    addEdges(graph, 0, 2);
    addEdges(graph, 1, 2);
    
    //addEdges(graph, 1, 2);
    
    printf("Walks:\n");
    getWalk(graph);
    printf("Paths:\n");
    getPath(graph);
    printf("Trails:\n");
    getTrail(graph);

    if (isEulerian(graph)) {
        printf("The graph is Eulerian.\n");
    }
    else {
        printf("The graph is not Eulerian.\n");
    }
    
}