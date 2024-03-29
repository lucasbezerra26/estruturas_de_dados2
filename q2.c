#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

typedef struct{
    int **aresta; //as conexões
    int **estados;
    int *grau; //quantas aresta o vertice ja possui
    int **pesos;
}grafo;

int verificaQtdMovimento(int *linha1, int *linha2){
    int cont=0;
    for(int i=0;i<4;i++){
        if(linha1[i] != linha2[i]){
            cont++;
        }
    }
    if(cont==1){
        return 1;
    }
    return 0;
}

int verificaMoverOrigem(int *linha1, int *linha2){
    // se não há um disco menor em cima do disco que irá mover 
    int cont=0;
    for(int i=0;i<4;i++){
        if(linha1[i] != linha2[i]){
            for(int x=i-1;x>=0;x--){
                if(linha1[x]==linha1[i])
                    return 0;
            }
        }
    }
    return 1;
}

int verificaMoverDestino(int *linha1, int *linha2){
    //Se não existe um disco menor no topo da torre que será movido o disco atual
    for(int i=0;i<4;i++){
        if(linha1[i] != linha2[i]){
            for(int x=i-1;x>=0;x--){
                if(linha2[x]==linha2[i])
                    return 0;
            }
        }
    }
    return 1;
}

grafo *gerarGrafo(){
    grafo *g;
    g = (grafo*) malloc(sizeof(grafo));
    g->aresta = (int **)malloc(sizeof(int*) * 81);
    g->estados = (int **)malloc(sizeof(int*) * 81);
    g->grau = (int *)calloc(81, sizeof(int));
    g->pesos = (int **)malloc(81 * sizeof(int *));
    
    for (int i = 0; i < 81; i++){
        g->estados[i] = (int *)calloc(4, sizeof(int));
    }
    for (int i = 0; i < 81; i++){
        g->aresta[i] = (int *)calloc(3, sizeof(int));
    }
    
    for (int j = 0; j < 81; j++){
        g->pesos[j] = (int *)malloc(3 * sizeof(int)); //fazendo a mesma coisa com os pesos
    }
    
    for(int i=0;i<81;i++){
        for(int x=0;x<3;x++){
            g->aresta[i][x] = -1;
            g->pesos[i][x] = 1;
        }
    }

    int cont=0;
    for( int i=0; i<3; i++){    
        for( int z=0; z<3; z++){
            for( int x=0; x<3; x++){
                for( int y=0; y<3; y++){
                    // printf("eii");
                    g->estados[cont][0] += i+1;
                    g->estados[cont][1] += z+1;
                    g->estados[cont][2] += x+1;
                    g->estados[cont][3] += y+1;
                    cont++;
                }

            }

        }
    }
    for(int i=0;i<81;i++){
        for(int x=i+1;x<81;x++){
            if (verificaQtdMovimento(g->estados[i], g->estados[x]) && verificaMoverOrigem(g->estados[i], g->estados[x]) && verificaMoverDestino(g->estados[i], g->estados[x])){
                g->aresta[i][g->grau[i]] = x;
                g->aresta[x][g->grau[x]] = i;
                g->grau[i]++;
                g->grau[x]++;
            }
        }
    }
    return g;
}

void mostraCaminho(grafo *gr,int aux, int *predecessores){
    if (aux>-1){
        printf("Posicao: %d Estado: %d %d %d %d\n", aux, gr->estados[aux][0], gr->estados[aux][1], gr->estados[aux][2], gr->estados[aux][3]);
        mostraCaminho(gr, predecessores[aux], predecessores);
    }
}

int BellmanFord(grafo *gr, int **pesos, int fim, int posicao, int predecessores[]){
    int flag=1;
    long int distancia[81];

    for (int i = 0; i < 81; i++){  
        predecessores[i] = -1;
        distancia[i] = INT_MAX / 2;
    }

    distancia[posicao] = 0;
    predecessores[fim] = fim;

    for (int i = 0; i < 80 && flag;i++){ 
        flag = 0;
        for (int x = 0; x < 81; x++){
            if (distancia[x] != (INT_MAX/2)){
                for (int y = 0; y < gr->grau[x]; y++){ 
                    if (distancia[gr->aresta[x][y]] > distancia[x] + 1){
                        distancia[gr->aresta[x][y]] = distancia[x] + 1;
                        predecessores[gr->aresta[x][y]] = x;
                        flag = 1;
                    }
                }
            }
        }
    }
    return distancia[80]; //onde todos os discos estão na torre 4
}


int converteEstadoPosicao(int estado1, int estado2, int estado3, int estado4){
    int cont=0;
    for( int i=0; i<3; i++){    
        for( int z=0; z<3; z++){
            for( int x=0; x<3; x++){
                for( int y=0; y<3; y++){
                    if( i+1 == estado1 && z+1 == estado2 && x+1 == estado3 && y+1==estado4) 
                        return cont;
                    cont++;
                }

            }
        }
    }
    return cont;
}


long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

int main(){
    int ini, fim;
    int estado1, estado2, estado3, estado4, predecessores[81];
    grafo *g = gerarGrafo();

    // for(int i=0;i<81;i++){
    //     printf("%d- ",i);
    //     for (int x = 0; x < 3; x++)
    //         printf("%d ",g->aresta[i][x]);
    //     printf("\n");
    // }
    // for(int i=0;i<81;i++){
    //     printf("%d- %d%d%d%d\n ", i, g->estados[i][0], g->estados[i][1], g->estados[i][2], g->estados[i][3]);
    // }
    // int posicao = converteEstadoPosicao(1, 1, 3, 3);

    printf("entre com uma posição dos disco nas torres:\n");
    printf("ex: 1 1 1 1 significa que todos estão na torre 1.\n");
    printf("ex: 1 2 3 3 significa que o menor está na torre 1, o maior e o segundo maior na torre 3, e o outra na torre 2.\n");
    printf(">>>");
    setbuf(stdin,NULL);
    scanf("%d %d %d %d", &estado1, &estado2, &estado3, &estado4);
    setbuf(stdin,NULL);
    int posicao = converteEstadoPosicao(estado1, estado2, estado3, estado4);
    // printf("Posicao: %d\n", posicao);

    ini = getMicrotime();
    int retorno = BellmanFord(g, g->pesos, 80, posicao, predecessores);
    printf("A quantidade de movimentações e custo é %d\n", retorno);
    fim = getMicrotime();
    printf("Tempo: %d\n", (fim - ini));

    printf("Caminho:\n");
    mostraCaminho(g, 80, predecessores);
    printf("\n");

    return 0;
}