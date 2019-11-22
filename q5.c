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

char *rotaciona(char matricula[]){
    char *matricula_final = (char *) malloc(6 * sizeof(char));
    char numero1 = matricula[0];
    char numero2 = matricula[1];
    char numero3 = matricula[2];
    char numero4 = matricula[3];
    char numero5 = matricula[4];
    char numero6 = matricula[5];

    matricula_final[0] = numero5;
    matricula_final[1] = numero6;
    matricula_final[2] = numero1;
    matricula_final[3] = numero2;
    matricula_final[4] = numero3;
    matricula_final[5] = numero4;
    
    return matricula_final;
}

char *digitos(char matricula[]){
    char *d = (char *) malloc(4 * sizeof(char));
    d[0] = matricula[1];
    d[1] = matricula[3];
    d[2] = matricula[5];
    d[3] = '\0';
    return d;
}

int colisao(char mat[], int modulo){
    int primeiro_digito = mat[0] - '0';
    return modulo + primeiro_digito;
}

int pode_gravar(hash *h, int pos, int cont){
    int controle = 1;
    for (int i = 0; i < cont; i++){
        if(h[i].pos == pos && pos != -1){
            controle = 0;
            break;
        }
    }
    return controle;
}

int grava_hash(hash *h, funcionario *f, char mat[], int atual, int *num_colisao){
    int controle = 1;
    
    if(atual == NUM_VETOR - 1){
        *num_colisao = *num_colisao + 1;
        controle = 0;
    }else{
        char *num_digito = digitos(mat);
        int pos = atoi(num_digito)%NUM_VETOR;

        while(!pode_gravar(h, pos, atual)){
            *num_colisao += 1;
            pos = colisao(mat, pos);
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
    hash *hashes = gerar_hash();
    int atual = 0;

    int num_colisoes = 0;

    for (int i = 0; i < 1000; i++){
        int x = grava_hash(hashes, funcionarios, funcionarios[i].matricula, atual, &num_colisoes);
        if(x){
            atual++;
        }
    }

    printf("num atual %d\n", atual);

    printf("num colisoes = %d\n", num_colisoes);
    
    return 0;

}