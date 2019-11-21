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

void dijkstra(grafo *g){
    int n = 81;
    int visitados[n];

    for(int i = 1; i < n; i++){ // Começa em 1 pois não precisa comparar o vértice com ele mesmo

        int min = -1; // Variável que guarda a posição do menor valor, inicia em -1 pois é uma posição inválida
        unsigned long int MinValor = 4294967295; // Variável que guarda o menor valor encontrado, inicia com 'infinito', assim, sempre na primeira passada o valor será menor que esta variável

        // For que percorre todas as linhas na coluna [0]
        for(int j = 1; j < n; j++){
            // Se o vertice ainda não foi visitado e o valor for menor que o 'MinValor'
            if( visitados[j] == 0 && g->estados[j][0] < MinValor ){
                   min = j; // Guarda a posição do menor
                   MinValor = g->estados[j][0]; // Guarda o menor valor
            }
          }

          visitados[min] = 1; // Marca o valor a posição do minimo como visitado

        // For de 1 até n
          for(int j = 1; j < n; j++){
               // Se o valor da coluna [0] + o valor da coluna que está passando for menor que o valor da linha que está passando e coluna [0]
               // Atualiza a primeira coluna da matriz, que será utilizado para as próximas iterações
               if( (g->estados[min][0] + g->estados[min][j]) < g->estados[j][0] ){
                g->estados[j][0] = g->estados[min][0] + g->estados[min][j];
               }
          }
    }
}

int main(){
    grafo *g = gerarGrafo();
    for(int i=0;i<81;i++){
        printf("%d- ",i);
        for (int x = 0; x < 3; x++)
            printf("%d ",g->aresta[i][x]);
        printf("\n");
    }

    dijkstra(g);

    return 0;
}