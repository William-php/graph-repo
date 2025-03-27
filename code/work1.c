#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100

// Estrutura para representar o grafo
typedef struct {
    int adj[MAX_VERTICES][MAX_VERTICES];
    int numVertices;
} Grafo;

// Função para inicializar o grafo
void inicializarGrafo(Grafo *g, int vertices) {
    g->numVertices = vertices;
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            g->adj[i][j] = 0;
        }
    }
}

// Função para adicionar aresta
void adicionarAresta(Grafo *g, int origem, int destino) {
    g->adj[origem][destino]++;
    g->adj[destino][origem]++; // Para grafos não direcionados
}

// Função para verificar se é um passeio válido
bool ehPasseio(Grafo *g, int sequencia[], int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        if (g->adj[sequencia[i]][sequencia[i+1]] == 0) {
            return false;
        }
    }
    return true;
}

// Função para verificar se é uma trilha (passeio sem arestas repetidas)
bool ehTrilha(Grafo *g, int sequencia[], int tamanho) {
    if (!ehPasseio(g, sequencia, tamanho)) {
        return false;
    }
    
    // Cria uma cópia da matriz de adjacência para marcar arestas usadas
    int copiaAdj[MAX_VERTICES][MAX_VERTICES];
    for (int i = 0; i < g->numVertices; i++) {
        for (int j = 0; j < g->numVertices; j++) {
            copiaAdj[i][j] = g->adj[i][j];
        }
    }
    
    for (int i = 0; i < tamanho - 1; i++) {
        int u = sequencia[i];
        int v = sequencia[i+1];
        if (copiaAdj[u][v] == 0) {
            return false;
        }
        copiaAdj[u][v]--;
        copiaAdj[v][u]--;
    }
    
    return true;
}

// Função para verificar se é um caminho (trilha sem vértices repetidos)
bool ehCaminho(Grafo *g, int sequencia[], int tamanho) {
    if (!ehTrilha(g, sequencia, tamanho)) {
        return false;
    }
    
    // Verifica vértices repetidos
    for (int i = 0; i < tamanho; i++) {
        for (int j = i + 1; j < tamanho; j++) {
            if (sequencia[i] == sequencia[j]) {
                return false;
            }
        }
    }
    
    return true;
}

// Função para verificar se o grafo é conexo
bool ehConexo(Grafo *g) {
    if (g->numVertices == 0) return true;
    
    bool visitado[MAX_VERTICES] = {false};
    int pilha[MAX_VERTICES];
    int topo = -1;
    
    // DFS a partir do vértice 0
    pilha[++topo] = 0;
    visitado[0] = true;
    int count = 1;
    
    while (topo >= 0) {
        int v = pilha[topo--];
        for (int u = 0; u < g->numVertices; u++) {
            if (g->adj[v][u] > 0 && !visitado[u]) {
                visitado[u] = true;
                pilha[++topo] = u;
                count++;
            }
        }
    }
    
    return (count == g->numVertices);
}

// Função para verificar se o grafo é Euleriano
bool ehEuleriano(Grafo *g) {
    if (!ehConexo(g)) {
        return false;
    }
    
    // Verifica se todos os vértices têm grau par
    for (int i = 0; i < g->numVertices; i++) {
        int grau = 0;
        for (int j = 0; j < g->numVertices; j++) {
            grau += g->adj[i][j];
        }
        if (grau % 2 != 0) {
            return false;
        }
    }
    
    return true;
}

// Função para imprimir sequências
void imprimirSequencia(int sequencia[], int tamanho) {
    printf("[");
    for (int i = 0; i < tamanho; i++) {
        printf("%d", sequencia[i]);
        if (i < tamanho - 1) printf(", ");
    }
    printf("]");
}

int main() {
    Grafo g;
    int vertices, arestas;
    
    printf("Digite o número de vértices: ");
    scanf("%d", &vertices);
    inicializarGrafo(&g, vertices);
    
    printf("Digite o número de arestas: ");
    scanf("%d", &arestas);
    
    printf("Digite as arestas (origem destino):\n");
    for (int i = 0; i < arestas; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        adicionarAresta(&g, u, v);
    }
    
    // Verifica se é Euleriano
    if (ehEuleriano(&g)) {
        printf("\nO grafo é Euleriano.\n");
    } else {
        printf("\nO grafo não é Euleriano.\n");
    }
    
    // Testar sequências
    printf("\nTeste de sequências (digite 0 para encerrar):\n");
    while (1) {
        int tamanho;
        printf("\nTamanho da sequência (0 para sair): ");
        scanf("%d", &tamanho);
        
        if (tamanho <= 0) break;
        
        int sequencia[tamanho];
        printf("Digite a sequência de vértices (separados por espaço): ");
        for (int i = 0; i < tamanho; i++) {
            scanf("%d", &sequencia[i]);
        }
        
        printf("Sequência: ");
        imprimirSequencia(sequencia, tamanho);
        
        if (ehPasseio(&g, sequencia, tamanho)) {
            printf("\n- É um passeio válido");
            
            if (ehTrilha(&g, sequencia, tamanho)) {
                printf("\n- É uma trilha válida");
                
                if (ehCaminho(&g, sequencia, tamanho)) {
                    printf("\n- É um caminho válido");
                }
            }
        } else {
            printf("\n- Não é um passeio válido");
        }
        printf("\n");
    }
    
    return 0;
}