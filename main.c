#include <stdio.h>

struct Cliente {
    char nome[50];
    char cpf[12];
    int idade;
};

int main(){

    int opcao;

    do {
        printf("\nDigite uma opção");
        printf("\n\t1 - Inserir\n\t2 - Buscar Cliente por CPF Remover\n\t3 - Listar\n\t4 - Sair\n");
        scanf("%d", &opcao);

        switch(opcao){
            case 1:

            break;
            case 2:

            break;
            case 3:

            break;
            case 4:
                return 0;
            break;
            default:
                continue;
            break;
        }
    } while (opcao != 4);
} 