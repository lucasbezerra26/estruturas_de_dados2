#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NUM_VETOR 150

typedef struct {
    char matricula[6];
    char nome[200];
    char funcao[200];
    float salario;
}funcionario;

typedef struct {
    int pos;
    char mat[6];
}hash;

funcionario *gerar_funcionarios_arquivo(){
    FILE *arquivo;
    funcionario *f = NULL;
    arquivo = fopen("entradaHASHING-05.txt", "r");
    f = (funcionario *)malloc(sizeof(funcionario) * 1000);
    int cont = 0;
    if (arquivo != NULL){
        while (!feof(arquivo)){
            fscanf(arquivo, "%s", f[cont].matricula);
            cont++;
        }
    }
    return f;
}

hash *gerar_hash(){
    hash *h = NULL;
    h = (hash * ) malloc(sizeof(hash) * NUM_VETOR);
    for (int i = 0; i < NUM_VETOR; i++){
        h[i].pos = -1;
    }
    return h;
}

char *digitosA(char matricula[]){
    char *d = (char *) malloc(4 * sizeof(char));
    d[0] = matricula[5];
    d[1] = matricula[1];
    d[2] = matricula[3];
    d[3] = '\0';
    return d;
}

int digitosB(char matricula[]){
    int num1 = (
        (matricula[0]-'0'*100) +
        (matricula[2]-'0'*10) +
        (matricula[4]-'0'*1)
    );
    int num2 = (
        (matricula[1]-'0'*100) +
        (matricula[3]-'0'*10) +
        (matricula[5]-'0'*1)
    );
    return ((num1+num2)%1000)%NUM_VETOR;
}

int colisaoA(char mat[], int modulo, int *flag){
    int primeiro_digito = mat[4] - '0';
    if(primeiro_digito == 0)
        *flag = 1;
    return modulo + primeiro_digito;
}

int colisaoB(int modulo){
    return modulo + 7;
}

int pode_gravar(hash *h, int pos){
    int controle = 1;
    for (int i = 0; i < NUM_VETOR; i++){
        if(h[i].pos == pos){
            controle = 0;
            break;
        }
    }
    return controle;
}

int ocupados(hash *h){
    int c = 0;
    for (int i = 0; i < NUM_VETOR; i++){
        if (h[i].pos != -1){
            c++;
        }
    }
    return c;
}

int grava_hashA(hash *h, funcionario *f, char mat[], int atual, int *num_colisao){
    int controle = 1;
    
    if(atual == NUM_VETOR - 1){
        *num_colisao = *num_colisao + 1;
        controle = 0;
    }else{
        char *num_digito = digitosA(mat);
        int pos = atoi(num_digito)%NUM_VETOR;
        int flag = 0;

        while(!pode_gravar(h, pos) && pos < NUM_VETOR){
            *num_colisao += 1;
            pos = colisaoA(mat, pos, &flag);
            if(flag){
                break;
            }
        }

        atual++;
        h[atual].pos = pos;

    }
    return controle;

}

int grava_hashB(hash *h, funcionario *f, char mat[], int atual, int *num_colisao){
    int controle = 1;
    
    if(atual == NUM_VETOR - 1){
        *num_colisao = *num_colisao + 1;
        controle = 0;
    }else{
        int pos = digitosB(mat);

        while(!pode_gravar(h, pos) && pos < NUM_VETOR){
            *num_colisao += 1;
            pos = colisaoB(pos);
        }

        atual++;
        h[atual].pos = pos;

    }
    return controle;

}

int main(){
    srand(time(NULL));

    funcionario *funcionarios;

    funcionarios = gerar_funcionarios_arquivo();
    hash *hashesA = gerar_hash();
    hash *hashesB = gerar_hash();
    
    int num_colisoesA = 0;
    int num_colisoesB = 0;

    for (int i = 0; i < 1000; i++){
        grava_hashA(hashesA, funcionarios, funcionarios[i].matricula, ocupados(hashesA), &num_colisoesA);
        grava_hashB(hashesB, funcionarios, funcionarios[i].matricula, ocupados(hashesB), &num_colisoesB);
    }

    // printf("num atual %d\n", ocupados(hashes));

    printf("num colisoesA = %d\n", num_colisoesA);
    printf("num colisoesB = %d\n", num_colisoesB);
    
    return 0;

}