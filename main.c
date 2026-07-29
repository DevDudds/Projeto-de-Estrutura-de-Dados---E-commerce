#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define T 2

#define MAX_CLIENTES (T * 2 - 1) // nossa árvore é de ordem 3, ou seja, duas chaves pois é sempre N-1
#define MAX_FILHOS (2 * T)   // e três filhos, pois é sempre total de chaves + 1

typedef struct Cliente
{                  // aprendi que se a gente colocar 'typedef', evitamos de por struct
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

NoCliente *NoVazio(bool eh_folha)
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
    if ( i < raiz ->num_chaves && strcmp(cpf,raiz->clientes[i].cpf) == 0){
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

void dividirFilho(NoCliente *pai, int indice) {
    NoCliente *filho = pai->filhos[indice];
    NoCliente *novo = NoVazio(filho->eh_folha);

    for (int i = 0; i < T - 1; i++) {
        novo->clientes[i] = filho->clientes[i + T];
    }

    novo->num_chaves = T - 1;

    if (!filho->eh_folha) {
        for (int i = 0; i < T; i++) {
            novo->filhos[i] = filho->filhos[i + T];
        }
    }

    filho->num_chaves = T - 1;

    for (int i = pai->num_chaves; i>= indice + 1; i--) {
        pai->filhos[i + 1] = pai->filhos[i];
    }

    pai->filhos[indice + 1] = novo;

    for (int i = pai->num_chaves - 1; i>= indice; i--) {
        pai->clientes[i + 1] = pai->clientes[i];
    }

    pai->clientes[indice] = filho->clientes[T - 1];

    pai->num_chaves++;
}

void inserirNaoCheio(NoCliente *no, Cliente cliente) {
    int c = no->num_chaves - 1;
    if (no->eh_folha) {
        while(c >= 0 && strcmp(cliente.cpf, no->clientes[c].cpf) < 0) {
            no->clientes[c + 1] = no->clientes[c];
            c--;
        }

        no->clientes[c+1] = cliente;
        no->num_chaves++;
    } else {
        while(c >= 0 && strcmp(cliente.cpf, no->clientes[c].cpf) < 0) {
            c--;
        }
        c++;
        if (no->filhos[c]->num_chaves == MAX_CLIENTES) {
            dividirFilho(no, c);
            if (strcmp(cliente.cpf, no->clientes[c].cpf) > 0) {
                c++;
            }
            inserirNaoCheio(no->filhos[c], cliente);
        }
    }
}

void inserir(NoCliente **raiz, Cliente cliente) {
    if((*raiz)->num_chaves == MAX_CLIENTES) {
        NoCliente *novaRaiz = NoVazio(false);
        novaRaiz->filhos[0] = *raiz;
        dividirFilho(novaRaiz, 0);
        *raiz = novaRaiz;
        inserirNaoCheio(novaRaiz, cliente);
    } else {
        inserirNaoCheio(*raiz, cliente);
    }
}

void imprimirNo(NoCliente *no)
{
    printf("[ ");

    for (int i = 0; i < no->num_chaves; i++)
    {
        printf("%s ", no->clientes[i].cpf);
    }

    printf("]\n");
}

int main()
{

    int opcao;

    NoCliente *raiz = NoVazio(true);

    do
    {
        printf("\nDigite uma opção");
        printf("\n\t1 - Inserir\n\t2 - Buscar Cliente por CPF \n\t3 - Listar\n\t4 - Sair\n");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            Cliente cliente;
            printf("\n--- INSERIR CLIENTE ---\n");
            printf("Nome: ");
            scanf(" %[^\n]", cliente.nome);

            printf("CPF: ");
            scanf("%11s", cliente.cpf);

            printf("Idade: ");
            scanf("%d", &cliente.idade);

            inserir(&raiz, cliente);

            imprimirNo(raiz);

            printf("\nCliente inserido!\n");

            break;
        case 2:
            char cpf_digitado[15];
            int indice = -1;

            printf("\n--- BUSCAR CLIENTE POR CPF ---\n");
            printf("Digite o CPF do cliente: ");
            scanf("%s",cpf_digitado);

            NoCliente *no_encontrado = buscarCliente(raiz, cpf_digitado, &indice);

            if (no_encontrado !=NULL && indice != -1){
                Cliente cliente = no_encontrado->clientes[indice];

                printf("\n--- Cliente encontrado ---\n");
                printf("Nome: %s\n", cliente.nome);
                printf("CPF: %s\n", cliente.cpf);

            } else {

                printf("\n Cliente com o CPF [%s] não foi encontrado", cpf_digitado);
            }

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