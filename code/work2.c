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

int *createVector(int length) {
    int *array = (int*)malloc(length * sizeof(int));
    for (int i = 0; i < length; i++) array[i] = -1; // Inicializa com -1
    return array;
}

Graph *createGraph(int v) {
    Graph *graph = (Graph*)malloc(sizeof(Graph));
    graph->vertex = v;
    graph->edges = 0;
    graph->adjMatrix = createMatrix(v, v);
    return graph;
}

void addEdge(Graph *graph, int v, int w) {
    if (!graph->adjMatrix[v][w]) {
        graph->adjMatrix[v][w] = 1;
        graph->adjMatrix[w][v] = 1;
        graph->edges++;
    }
}

void removeEdge(Graph *graph, int v, int w) {
    if (graph->adjMatrix[v][w]) {
        graph->adjMatrix[v][w] = 0;
        graph->adjMatrix[w][v] = 0;
        graph->edges--;
    }
}

int dfs(Graph *graph, int v, int *visited, int target) {
    visited[v] = 1; // Marca o vértice como visitado
    printf("%d ", v); // Imprime o vértice visitado

    if (v == target) {
        return 1; // Retorna 1 se o vértice alvo foi encontrado
    }

    for (int i = 0; i < graph->vertex; i++) {
        if (graph->adjMatrix[v][i] && visited[i] == -1) {
            if (dfs(graph, i, visited, target)) {
                return 1; // Propaga o retorno se o vértice alvo foi encontrado
            }
        }
    }

    return 0; // Retorna 0 se o vértice alvo não foi encontrado
}

int graphDFS(Graph *graph, int target) {
    int *visited = createVector(graph->vertex); // Inicializa o vetor de visitados
    int found = 0;

    printf("DFS traversal:\n");
    for (int i = 0; i < graph->vertex; i++) {
        if (visited[i] == -1) { // Se o vértice não foi visitado
            if (dfs(graph, i, visited, target)) {
                found = 1; // Marca como encontrado se o vértice alvo foi encontrado
            }
            printf("\n"); // Nova linha para separar componentes conectados
        }
    }

    free(visited); // Libera a memória alocada para o vetor de visitados
    return found;
}

int bfs(Graph *graph, int start, int target, int *visited) {
    int *queue = (int*)malloc(graph->vertex * sizeof(int)); // Fila para BFS
    int front = 0, rear = 0; // Ponteiros da fila

    // Adiciona o vértice inicial à fila e marca como visitado
    queue[rear++] = start;
    visited[start] = 1;

    printf("BFS traversal: ");

    while (front < rear) {
        int current = queue[front++]; // Remove o vértice da fila
        printf("%d ", current);

        if (current == target) {
            free(queue);
            return 1; // Retorna 1 se o vértice alvo foi encontrado
        }

        // Adiciona os vizinhos não visitados à fila
        for (int i = 0; i < graph->vertex; i++) {
            if (graph->adjMatrix[current][i] && visited[i] == -1) {
                queue[rear++] = i;
                visited[i] = 1; // Marca como visitado
            }
        }
    }

    free(queue);
    return 0; // Retorna 0 se o vértice alvo não foi encontrado
}

int graphBFS(Graph *graph, int target) {
    int *visited = createVector(graph->vertex); // Inicializa o vetor de visitados
    int found = 0;

    for (int i = 0; i < graph->vertex; i++) {
        if (visited[i] == -1) { // Se o vértice não foi visitado
            if (bfs(graph, i, target, visited)) {
                found = 1; // Marca como encontrado se o vértice alvo foi encontrado
            }
            printf("\n"); // Nova linha para separar componentes conectados
        }
    }

    free(visited); // Libera a memória alocada para o vetor de visitados
    return found;
}

void freeMatrix(int **matrix, int row) {
    for (int i = 0; i < row; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void freeGraph(Graph *graph) {
    freeMatrix(graph->adjMatrix, graph->vertex);
    free(graph);
}

void main() {
    Graph *graph = createGraph(4);
    addEdge(graph, 0, 1);    
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 3);

    int target = 3;
    graphDFS(graph, target) ? 
        printf("Vertex %d was found in the graph.\n", target) : 
        printf("Vertex %d was not found in the graph.\n", target);

    graphBFS(graph, target) ? 
        printf("Vertex %d was found in the graph.\n", target) : 
        printf("Vertex %d was not found in the graph.\n", target);

    freeGraph(graph); // Libera a memória do grafo
}