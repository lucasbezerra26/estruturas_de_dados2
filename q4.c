#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits.h>
int t;

struct Grafo{
	int nro_vertices;
	int** aresta; //as conexões 
	float** pesos;
	int* grau; //quantas aresta o vertice ja possue
};
typedef struct Grafo grafo;

grafo *cria_grafo(int nro_vertices){
	grafo *gr = (grafo*) malloc(sizeof(grafo));
	gr->nro_vertices = nro_vertices;
	gr->grau = (int*) calloc(nro_vertices,sizeof(int)); //criando a lista
	gr->aresta = (int**) malloc(sizeof(int*)*nro_vertices);
	gr->pesos = (float**) malloc(nro_vertices * sizeof(float*));
	for (int i = 0; i < nro_vertices; i++){
		gr->aresta[i] = (int*) malloc(nro_vertices * sizeof(int)); //colocando para apontar cada posição da lista para outra lista  
        gr->pesos[i] = (float*) malloc(nro_vertices  *sizeof(float)); //fazendo a mesma coisa com os pesos
		
	}
	return gr; 
}

void libera_grafo(grafo* gr){
	if ( gr != NULL){
		for (int i = 0; i < gr->nro_vertices; i++){
			free(gr->aresta[i]);
            free(gr->pesos[i]);
		}
		free(gr->aresta);
        free(gr->pesos);
		free(gr->grau);
		free(gr);
	}
}

int insereAresta(grafo* gr, int orig, int dest, float peso){
	//inicio (verificando se tudo existe)
	
	if (gr == NULL)
		return 0;
	if(orig-1 < 0 || orig-1 >= gr->nro_vertices)
		return 0;
	if(dest-1 < 0 || dest-1 >= gr->nro_vertices)
		return 0;
	//fim;
	gr->aresta[orig-1][gr->grau[orig-1]] = dest-1; //adicionando o destino a ultima posicao da lista
	gr->pesos[orig-1][gr->grau[orig-1]] = peso; //se for ponderado faz a mesma coisa ao peso
	gr->grau[orig-1]++; // e incrementa o numero de ligações para aquele vertice 	

	return 1;
}

int existe_aberto(int v[],int tam){
    for(int i = 0; i < tam; i++){
        if(v[i]){
            return 1;
        }
    }
    return 0;
}

int menor_distancia(grafo *g, int abertos[], float distancia[]){
    int retorno = -1;
    distancia[-1] = (float)INT_MAX / 2;
    for(int i = 0; i < g->nro_vertices; i++){
        if (abertos[i] && (distancia[i] < distancia[retorno]) ){
            retorno = i;
        }   
    }
    return retorno;
}

void relaxa(grafo *g, float distancia[], int predecessores[], int u, int valor,float peso){
    if(distancia[valor] > distancia[u] + peso){
        distancia[valor] = distancia[u] + peso;
        predecessores[valor] = u;
    }
}

float dijkstra(grafo *g, float **pesos, int ini, int fim,int *predecessores){
    int tamanho = g->nro_vertices;
    float *distancia=NULL;
    int *abertos=NULL;
    abertos = (int *) malloc(sizeof(int) * tamanho);
    distancia = (float *)malloc(sizeof(float) * tamanho);

    for (int i = 0; i < tamanho; i++){
        distancia[i] = (float) INT_MAX/2;
        predecessores[i] = -1;
        abertos[i] = 1;
    }
    distancia[ini] = 0;

    while (existe_aberto(abertos, tamanho)){
        int u = menor_distancia(g, abertos, distancia);

        if (u >= 0){
            abertos[u] = 0;
            for(int i = 0; i < g->grau[u]; i++)
                relaxa(g, distancia, predecessores, u, g->aresta[u][i], g->pesos[u][i]);
        }
    }
    free(abertos);
}


void mostraCaminho(int aux, int *predecessores){
    if (aux>-1){
        mostraCaminho(predecessores[aux], predecessores);
        printf("%d ", aux+1);
    }
}

int main(){
	grafo *g = NULL;
    int *predecessores=NULL;
    int num_vertices;
    // predecessores = (int*) malloc(sizeof(int)*4);
    // g = cria_grafo(4);
    // insereAresta(g, 1, 2,2);
    // insereAresta(g, 1, 3,1);
    // insereAresta(g, 2, 4,1);
    // insereAresta(g, 3, 4,1);

    printf("Digite a quantidade de vertices: ");
    scanf(" %d", &num_vertices);

    g = cria_grafo(num_vertices);
    predecessores = (int*) malloc(sizeof(int)*num_vertices);

    printf("Seus vertices são numeradas entre 1 e %d\n", num_vertices);

    int status = 1;
	int vertice1, vertice2;
	float peso;

	while (status == 1){
        printf("Digite as ligações entres os vertices. \nEx.: 1 5 ligam os vertices 1 e 5\n");
        scanf(" %d %d", &vertice1, &vertice2);
        printf("Qual o valor que indica a confiabilidade do canal? ");
		scanf(" %f", &peso);
        insereAresta(g, vertice1, vertice2,peso);
        printf("Deseja inserir mais? 1-sim/2-nao \n");
		scanf(" %d", &status);	
	};
    printf("Digite os vertices para saber o caminho mais confiavel(ex: 1 2) ");
    scanf("%d", &vertice1);
    scanf("%d", &vertice2);

    dijkstra(g, g->pesos, vertice1-1, vertice2-1, predecessores);
    // dijkstra(g, g->pesos, 1-1, 4-1, predecessores);

    libera_grafo(g);
    printf("\nO melhor caminho é: ");
    // mostraCaminho(4-1, predecessores);
    mostraCaminho(vertice2-1, predecessores);
    printf("\n");
    free(predecessores);
    return 0;
}