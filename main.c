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
    for (int i = 0; i < MAX_FILHOS; i++)
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
        }
        inserirNaoCheio(no->filhos[c], cliente);
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

// Escreve uma string no arquivo já "escapando" aspas e barras invertidas,
// pra não gerar um JSON inválido caso o nome tenha algum caractere especial.
void escreverStringJSON(FILE *f, char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '"' || str[i] == '\\')
        {
            fputc('\\', f);
        }
        fputc(str[i], f);
    }
}

// Escreve um único cliente como um objeto JSON, ex: {"nome": "...", "cpf": "...", "idade": 30}
void escreverClienteJSON(FILE *f, Cliente cliente)
{
    fprintf(f, "  {\"nome\": \"");
    escreverStringJSON(f, cliente.nome);
    fprintf(f, "\", \"cpf\": \"");
    escreverStringJSON(f, cliente.cpf);
    fprintf(f, "\", \"idade\": %d}", cliente.idade);
}

// Percorre a árvore B em ordem (in-order), que é a forma de visitar
// todos os clientes na ordem correta (por CPF), e vai escrevendo cada um no arquivo.
// 'primeiro' controla se colocamos vírgula antes do próximo elemento ou não.
void percorrerEmOrdemJSON(FILE *f, NoCliente *no, bool *primeiro)
{
    if (no == NULL)
        return;

    for (int i = 0; i < no->num_chaves; i++)
    {
        if (!no->eh_folha)
        {
            percorrerEmOrdemJSON(f, no->filhos[i], primeiro);
        }

        if (!*primeiro)
        {
            fprintf(f, ",\n");
        }
        escreverClienteJSON(f, no->clientes[i]);
        *primeiro = false;
    }

    if (!no->eh_folha)
    {
        percorrerEmOrdemJSON(f, no->filhos[no->num_chaves], primeiro);
    }
}

// Reescreve o arquivo clientes.json inteiro com todos os clientes da árvore.
void salvarClientesJSON(NoCliente *raiz, const char *nomeArquivo)
{
    FILE *f = fopen(nomeArquivo, "w");
    if (f == NULL)
    {
        printf("\nErro ao abrir o arquivo '%s' para salvar os dados!\n", nomeArquivo);
        return;
    }

    fprintf(f, "[\n");

    bool primeiro = true;
    percorrerEmOrdemJSON(f, raiz, &primeiro);

    fprintf(f, "\n]\n");

    fclose(f);
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

void listarClientes(NoCliente *no){
    if (no == NULL) return;

    int i;
    for (int i = 0; i < no->num_chaves; i++){
        if (!no->eh_folha){
            listarClientes(no->filhos[i]);
        }

        printf("CPF: %-12s | Nome %-20s | Idade: %d\n",
               no->clientes[i].cpf,
               no->clientes[i].nome,
               no->clientes[i].idade);

        if (!no->eh_folha){
            listarClientes(no->filhos[i]);
        }
        }
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
            printf("\n--- LISTA DE CLIENTES (ORDENADOS POR CPF) ---\n");
            if (raiz == NULL || raiz ->num_chaves == 0){
                printf("Nenhum cliente cadastrado.");
            }else{
                listarClientes(raiz);
            }

            break;
        case 4:
            salvarClientesJSON(raiz, "clientes.json");
            return 0;
            break;
        default:
            continue;
            break;
        }
    } while (opcao != 4);
}