#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char matricula[6];
    char *nome;
    char *funcao;
    float salario;
}funcionario;

char rotaciona(char *matricula){
    char numero1 = matricula[0];
    char numero2 = matricula[1];
    matricula[0] = matricula[4];
    matricula[1] = matricula[5];
}


int main(){
    return 0;
}