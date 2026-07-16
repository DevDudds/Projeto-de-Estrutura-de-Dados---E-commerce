#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_CLIENTES 2 // eduardo e erik, nossa árvore é de ordem 3, ou seja, duas chaves pois é sempre N-1
#define MAX_FILHOS 3   // e três filhos, pois é sempre total de chaves + 1

typedef struct Cliente
{                  // eduardo e erik, aprendi que se a gente colocar 'typedef', evitamos de por struct
    char nome[50]; // isso fala para o c++ que agora o Cliente é um tipo, como int, double, e tals.
    char cpf[12];  // isso evita colocar sempre "Struct Cliente, e colocamos somente "Cliente"."
    int idade;
} Cliente;

typedef struct NoCliente
{
    Cliente clientes[MAX_CLIENTES];
    struct NoCliente *filhos[MAX_FILHOS];
    int num_chaves;
    bool eh_folha;
} NoCliente;

NoCliente *NoVzio(bool eh_folha)
{
    NoCliente *novoCliente = (NoCliente *)malloc(sizeof(NoCliente));
    novoCliente->num_chaves = 0;
    novoCliente->eh_folha = eh_folha;
    for (size_t i = 0; i < MAX_FILHOS; i++)
    {
        novoCliente->filhos[i] = NULL;
    }
    return novoCliente;
};

NoCliente *buscarCliente(NoCliente *raiz, char *cpf, int *indice_encontrado)
{
    if (raiz == NULL)
        return NULL; // vê se a "caixinha" existe, se não, retorna null

    int i = 0; // esse i serve para "andar" procurando na aŕvore.

    while (i < raiz->num_chaves && strcmp(cpf, raiz->clientes[i].cpf) > 0){ // aqui faz que enquanto NÃO for maior que o número de chaves, e NÃO for o cpf desejado eu ando com i++
        i++;
    }
    
    // aqui se i for menor que o numero de chaves, e o cpf for igual ao que procuramos, nos usamos o *indice_encontrado = i; para apontar pra ele
    if ( i < raiz ->num_chaves && strcmp(cpf,raiz->clientes->cpf[i] == 0)){
        *indice_encontrado = i;
        return raiz; // aqui retornamos a raiz, pois achamos na raiz
    }
   // aqui se raiz for folha, ou seja, nao tenha mais nada para procurar,é pq aquele cpf não existe
    if (raiz->eh_folha){
        return NULL;
    }
    
    // aqui se a raiz tiver filhos, ele vai nos filhos procurando indice por indice com o raiz ->[i]
    return buscarCliente(raiz->filhos[i], cpf, indice_encontrado);
}

int main()
{

    int opcao;

    do
    {
        printf("\nDigite uma opção");
        printf("\n\t1 - Inserir\n\t2 - Buscar Cliente por CPF Remover\n\t3 - Listar\n\t4 - Sair\n");
        scanf("%d", &opcao);

        switch (opcao)
        {
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