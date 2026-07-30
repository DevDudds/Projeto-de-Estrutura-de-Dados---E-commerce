## Cadastro de Clientes e Histórico de Compras (E-commerce)

Um sistema de gerenciamento de usuários para uma loja virtual.

    Chave da Árvore: ID do Cliente ou Email.

    Dados do Nó: Nome, endereço de entrega, telefone e pontos de fidelidade.


# dividirFilho()
Divide um filho que está cheio.
Cria um novo nó.
Copia metade das chaves para esse novo nó.
Move a chave central para o pai.
Ajusta os ponteiros dos filhos.
Atualiza a quantidade de chaves.

# inserirNaoCheio()

Se for folha, insere mantendo a ordenação.
Caso contrário:
    escolhe o filho correto;
    divide o filho caso esteja cheio;
    continua a inserção recursivamente.

# inserir()
Função principal de inserção.

Responsabilidades:

verificar se a raiz está cheia;
criar uma nova raiz quando necessário;
chamar a inserção recursiva.

# remover()

Remove um cliente da Árvore B.

Casos tratados:

remoção em folha;
remoção em nó interno usando predecessor;
remoção usando sucessor;
empréstimo do irmão esquerdo;
empréstimo do irmão direito;
fusão de nós.

Durante toda a operação são preservadas as propriedades da Árvore B.

# removerArvore()

Responsável por iniciar a remoção e verificar se a raiz ficou vazia após a operação.

Caso isso aconteça, a árvore reduz sua altura, promovendo o filho adequado como nova raiz.
