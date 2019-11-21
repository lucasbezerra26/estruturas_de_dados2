#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int **aresta; //as conexões
    int **estados;
    int *grau; //quantas aresta o vertice ja possui
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
    /*
    Se não existe um disco menor 
    */
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
    for (int i = 0; i < 81; i++){
        g->estados[i] = (int *)calloc(4, sizeof(int));
    }
    for (int i = 0; i < 81; i++){
        g->aresta[i] = (int *)calloc(3, sizeof(int));
    }
    for(int i=0;i<81;i++)
        for(int x=0;x<3;x++)
            g->aresta[i][x] = -1;
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

int main(){
    grafo *g = gerarGrafo();
    for(int i=0;i<81;i++){
        printf("%d- ",i);
        for (int x = 0; x < 3; x++)
            printf("%d ",g->aresta[i][x]);
        printf("\n");
    }
    return 0;
}