#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define T 2

#define MAX_CLIENTES (T * 2 - 1)  
#define MAX_FILHOS (2 * T)   

typedef struct Cliente
{                  
    char nome[50]; 
    char cpf[12];  
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
        return NULL; 

    int i = 0; 

    while (i < raiz->num_chaves && strcmp(cpf, raiz->clientes[i].cpf) > 0){ 
        i++;
    }
    
   
    if ( i < raiz ->num_chaves && strcmp(cpf,raiz->clientes[i].cpf) == 0){
        *indice_encontrado = i;
        return raiz;
    }
   
    if (raiz->eh_folha){
        return NULL;
    }
    
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

    int indice = -1;

    if (buscarCliente(*raiz, cliente.cpf, &indice) != NULL){
        printf("Erro: CPF já cadastrado!\n");
        return;
    }

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

void escreverClienteJSON(FILE *f, Cliente cliente)
{
    fprintf(f, "  {\"nome\": \"");
    escreverStringJSON(f, cliente.nome);
    fprintf(f, "\", \"cpf\": \"");
    escreverStringJSON(f, cliente.cpf);
    fprintf(f, "\", \"idade\": %d}", cliente.idade);
}

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
    for ( i = 0; i < no->num_chaves; i++){
        if (!no->eh_folha){
            listarClientes(no->filhos[i]);
        }

        printf("CPF: %-12s | Nome %-20s | Idade: %d\n",
               no->clientes[i].cpf,
               no->clientes[i].nome,
               no->clientes[i].idade);
        }
        
        if (!no->eh_folha){
            listarClientes(no->filhos[i]);
        }
}

void remover(NoCliente *no, char *cpf) {
    int i = 0;
    while (i < no->num_chaves && strcmp(cpf, no->clientes[i].cpf) > 0) {
        i++;
    }

    if (i < no->num_chaves && strcmp(cpf, no->clientes[i].cpf) == 0) {
        if (no->eh_folha) {
            for (int j = i; j < no->num_chaves - 1; j++) {
                no->clientes[j] = no->clientes[j + 1];
            }

            no->num_chaves--;
        } else {
            if (no->filhos[i]->num_chaves >= T) {
                NoCliente *atual = no->filhos[i];

                while(!atual->eh_folha) {
                    atual = atual->filhos[atual->num_chaves];
                }

                Cliente pred = atual->clientes[atual->num_chaves - 1];
                no->clientes[i] = pred;
                remover(no->filhos[i], pred.cpf);

            } else if (no->filhos[i + 1]->num_chaves >= T) {
                NoCliente *atual = no->filhos[i + 1];

                while(!atual->eh_folha) {
                    atual = atual->filhos[0];
                }

                Cliente sucessor = atual->clientes[0];
                no->clientes[i] = sucessor;
                remover(no->filhos[i + 1], sucessor.cpf);

            } else {
                NoCliente *esquerdo = no->filhos[i];
                NoCliente *direito = no->filhos[i + 1];

                int antigas = esquerdo->num_chaves;

                esquerdo->clientes[antigas] = no->clientes[i];

                for (int j = 0; j < direito->num_chaves; j++) {
                    esquerdo->clientes[antigas + 1 + j] = direito->clientes[j];
                }

                esquerdo->num_chaves += direito->num_chaves + 1;

                if (!esquerdo->eh_folha) {
                    for (int j = 0; j <= direito->num_chaves; j++)
                    {
                        esquerdo->filhos[antigas + 1 + j] = direito->filhos[j];
                    }
                }

                for (int j = i; j < no->num_chaves - 1; j++) {
                    no->clientes[j] = no->clientes[j + 1];
                }

                for (int j = i + 1; j < no->num_chaves; j++) {
                    no->filhos[j] = no->filhos[j + 1];
                }

                no->num_chaves--;

                free(direito);

                remover(esquerdo, cpf);
            }
        }
    } else {
        if(no->eh_folha) {
            printf("Cliente não encontrado");
            return;
        } else {
            NoCliente *filho = no->filhos[i];
            if (filho->num_chaves == T - 1) {
                
                if(i > 0 && no->filhos[i - 1]->num_chaves >= T) {
                    NoCliente *irmao = no->filhos[i - 1];

                    for (int j = filho->num_chaves - 1; j >= 0; j--) {
                        filho->clientes[j + 1] = filho->clientes[j];
                    }

                    if (!filho->eh_folha) {
                        for (int j = filho->num_chaves; j >= 0; j--) {
                            filho->filhos[j + 1] = filho->filhos[j];
                        }
                        filho->filhos[0] = irmao->filhos[irmao->num_chaves];

                        irmao->filhos[irmao->num_chaves] = NULL;
                    }

                    filho->clientes[0] = no->clientes[i - 1];
                    no->clientes[i - 1] = irmao->clientes[irmao->num_chaves - 1];
                    
                    irmao->num_chaves--;
                    filho->num_chaves++;
                } 

                else if (i < no->num_chaves && no->filhos[i + 1]->num_chaves >= T) {
                    NoCliente *irmao = no->filhos[i + 1];

                    filho->clientes[filho->num_chaves] = no->clientes[i];

                    if (!filho->eh_folha) {
                        filho->filhos[filho->num_chaves + 1] = irmao->filhos[0];
                    }

                    no->clientes[i] = irmao->clientes[0];
                    
                    for (int j = 0; j < irmao->num_chaves - 1; j++) {
                        irmao->clientes[j] = irmao->clientes[j + 1];
                    }

                    if (!irmao->eh_folha) {
                        for (int j = 0; j < irmao->num_chaves; j++) {
                            irmao->filhos[j] = irmao->filhos[j + 1];
                        }

                        irmao->filhos[irmao->num_chaves] = NULL;
                    }

                    irmao->num_chaves--;
                    filho->num_chaves++;
                } else {
                    NoCliente *esquerdo = no->filhos[i];
                    NoCliente *direito = no->filhos[i + 1];

                    int antigas = esquerdo->num_chaves;
                    int chaves_direito = direito->num_chaves;

                    esquerdo->clientes[antigas] = no->clientes[i];

                    for (int j = 0; j < chaves_direito; j++) {
                        esquerdo->clientes[antigas + 1 + j] = direito->clientes[j];
                    }


                    esquerdo->num_chaves += direito->num_chaves + 1;

                    if(!esquerdo->eh_folha) {
                        for(int j = 0; j <= direito->num_chaves; j++) {
                            esquerdo->filhos[antigas + 1 + j] = direito->filhos[j];
                        }
                    }

                    for (int j = i; j < no->num_chaves - 1; j++) {
                        no->clientes[j] = no->clientes[j + 1];
                    }
                    
                    for (int j = i + 1; j < no->num_chaves; j++) {
                        no->filhos[j] = no->filhos[j + 1];
                    }

                    no->num_chaves--;
                    free(direito);
                    remover(esquerdo, cpf);
                    return;
                }
            }

            remover(filho, cpf);
        }
    }
}

void removerArvore(NoCliente **raiz, char *cpf)
{
    remover(*raiz, cpf);

    if((*raiz)->num_chaves == 0 && !(*raiz)->eh_folha)
    {
        NoCliente *aux = *raiz;
        *raiz = (*raiz)->filhos[0];
        free(aux);
    }
}

int main()
{
    int opcao;

    NoCliente *raiz = NoVazio(true);

    do
    {
        printf("\nDigite uma opção");
        printf("\n\t1 - Inserir");
        printf("\n\t2 - Buscar Cliente por CPF");
        printf("\n\t3 - Listar");
        printf("\n\t4 - Remover Cliente");
        printf("\n\t5 - Sair\n");

        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
        {
            Cliente cliente;

            printf("\n--- INSERIR CLIENTE ---\n");

            printf("Nome: ");
            scanf(" %[^\n]", cliente.nome);

            printf("CPF: ");
            scanf("%11s", cliente.cpf);

            printf("Idade: ");
            scanf("%d", &cliente.idade);

            inserir(&raiz, cliente);
            salvarClientesJSON(raiz, "clientes.json");
            break;
        }

        case 2:
        {
            char cpf_digitado[15];
            int indice = -1;

            printf("\n--- BUSCAR CLIENTE POR CPF ---\n");

            printf("Digite o CPF do cliente: ");
            scanf("%s", cpf_digitado);

            NoCliente *no_encontrado = buscarCliente(raiz, cpf_digitado, &indice);

            if (no_encontrado != NULL && indice != -1)
            {
                Cliente cliente = no_encontrado->clientes[indice];

                printf("\n--- Cliente encontrado ---\n");
                printf("Nome: %s\n", cliente.nome);
                printf("CPF: %s\n", cliente.cpf);
                printf("Idade: %d\n", cliente.idade);
            }
            else
            {
                printf("\nCliente com CPF [%s] não foi encontrado.\n", cpf_digitado);
            }

            break;
        }

        case 3:
        {
            printf("\n--- LISTA DE CLIENTES (ORDENADOS POR CPF) ---\n");

            if (raiz == NULL || raiz->num_chaves == 0)
            {
                printf("Nenhum cliente cadastrado.\n");
            }
            else
            {
                listarClientes(raiz);
            }

            break;
        }

        case 4:
        {
            char cpf_remover[15];

            printf("\n--- REMOVER CLIENTE ---\n");

            printf("Digite o CPF do cliente: ");
            scanf("%s", cpf_remover);

            removerArvore(&raiz, cpf_remover);

            printf("\nOperação de remoção concluída.\n");

            break;
        }

        case 5:
        {
            return 0;
        }

        default:
            printf("\nOpção inválida!\n");
            break;
        }

    } while (opcao != 5);
}