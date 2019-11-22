#include <stdio.h>
#include <stdlib.h>

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
                // printf("-- %d %d %d %d\n", g->estados[x][0], g->estados[x][1], g->estados[x][2], g->estados[x][3]);
            }
        }
    }
    return g;
}

void BellmanFord(grafo *hanoi, int **pesos, int ini){
    grafo *gr = hanoi;
    int n1, n2, n3;
    float vetorCusto[81], vetorAnterior[81];
    for (n1 = 0; n1 < 81; n1++){
        vetorCusto[n1] = 4294967295;
    }

    vetorCusto[ini] = 0;
    vetorAnterior[ini] = ini;

    for (n1 = 0; n1 < 81 - 1; n1++){       //Pecorre todos as ITERAÇÕES possiveis até está tudo correto.
        for (n2 = 0; n2 < 81; n2++){         //Pecorre todos os VERTICES.
            if (vetorCusto[n2] != 4294967295){
                for (n3 = 0; n3 < gr->grau[n2]; n3++){        //Pecorre todas as ARESTAS dos VERTICES.
                    if (vetorCusto[gr->aresta[n2][n3]] > vetorCusto[n2] + pesos[n2][n3]){
                        vetorCusto[gr->aresta[n2][n3]] = vetorCusto[n2] + pesos[n2][n3];
                        vetorAnterior[gr->aresta[n2][n3]] = n2;
                    }
                }
            }
        }
    }

}

int existe_aberto(int v[]){
    for(int i = 0; i < 81; i++){
        if(v[i]){
            return 1;
        }
    }
    return 0;
}

int menor_distancia(grafo *g, int abertos[], int distancia[]){
    long int menor = 4294967295;
    for(int i = 0; i < 81; i++){
        if(abertos[i] && distancia[i] < menor){
            menor = distancia[i];
        }   
    }
    return menor;
}

void relaxa(grafo *g, int distancia[], int predecessores[], int u, int valor){
    if(distancia[valor] > distancia[u] + 1){
        distancia[valor] = distancia[u] + 1;
        predecessores[valor] = u;
    }
}

void dijkstra(grafo *g, int **pesos, int final){
    int distancia[81];
    int predecessores[81];
    int abertos[81];


    for (int i = 0; i < 81; i++){
        abertos[i] = 1;
    }

    while (existe_aberto(abertos)){
        int u = menor_distancia(g, abertos, distancia);
        abertos[u] = 0;

        for(int i = 0; i < g->grau[u]; i++){
            relaxa(g, distancia, predecessores, u, g->aresta[u][i]);
        }

    }
    
    printf("disrancia\n");
}

int main(){
    grafo *g = gerarGrafo();
    for(int i=0;i<81;i++){
        printf("%d- ",i);
        for (int x = 0; x < 3; x++)
            printf("%d ",g->aresta[i][x]);
        printf("\n");
    }

    dijkstra(g, g->pesos, 80);

    // BellmanFord(g, g->pesos, 80);

    return 0;
}