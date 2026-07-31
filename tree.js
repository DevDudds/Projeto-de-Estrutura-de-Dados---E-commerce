const T = 2;
const MAX_CLIENTES = (T * 2 - 1);

class No {  
    constructor(folha=true) {
        this.folha = folha;
        this.clientes = [];
        this.filhos = [];
    }

    get numChaves() {
        return this.clientes.length;
    }
}

class ArvoreB {
    constructor() {
        this.raiz = new No(true);
    }

    dividirFilho(pai, indice) {
        const filho = pai.filhos[indice];
        const novo = new No(filho.folha);

        for (let i = 0; i < T - 1; i++) {
            novo.clientes.push(filho.clientes[i + T]);
        }

        if (!filho.folha) {
            for (let i = 0; i < T; i++) {
                novo.filhos.push(filho.filhos[i + T]);
            }
        }

        const promovido = filho.clientes[T - 1];

        filho.clientes = filho.clientes.slice(0, T - 1);

        if (!filho.folha) {
            filho.filhos = filho.filhos.slice(0, T);
        }

        pai.filhos.splice(indice + 1, 0, novo);

        pai.clientes.splice(indice, 0, promovido);
    }

    inserirNaoCheio(no, cliente) {
        let i = no.numChaves - 1;

        if (no.folha) {
            while (i >= 0 && cliente.cpf.localeCompare(no.clientes[i].cpf) < 0) {
                i--;
            } 
            no.clientes.splice(i + 1, 0, cliente);
        } else {
            while(i >= 0 && cliente.cpf.localeCompare(no.clientes[i].cpf) < 0) {
                i--;
            }

            i++;

            if (no.filhos[i].numChaves === MAX_CLIENTES) {
                this.dividirFilho(no, i);

                if (cliente.cpf.localeCompare(no.clientes[i].cpf) > 0) {
                    i++;
                }
            }
            this.inserirNaoCheio(no.filhos[i], cliente);
        }
    }

    inserir(cliente) {
        if (this.raiz.numChaves === MAX_CLIENTES) {
            const novaRaiz = new No(false);

            novaRaiz.filhos.push(this.raiz);

            this.dividirFilho(novaRaiz, 0);
            this.raiz = novaRaiz;
        }

        this.inserirNaoCheio(this.raiz, cliente);
    }
}


async function carregarClientes() {
    const resposta = await fetch("clientes.json");
    const clientes = await resposta.json();
    return clientes;
}

async function iniciar() {
    const clientes = await carregarClientes();
    const arvore = new ArvoreB();

    clientes.forEach(cliente => {
        arvore.inserir(cliente);
    });

    renderizarArvore(arvore.raiz);

    console.log(arvore);
}

function criarNoHTML(no) {
    const container = document.createElement("div");
    container.className = "tree-node";

    const keys = document.createElement("div");
    keys.className = "keys";

    no.clientes.forEach(cliente => {
        const key = document.createElement("div");
        key.className = "key";
        key.textContent = cliente.cpf;

        keys.appendChild(key);
    });

    container.appendChild(keys);
    
    if(!no.folha){

        const children = document.createElement("div");
        children.className = "children";

        no.filhos.forEach(filho => {

            children.appendChild(
                criarNoHTML(filho)
            );

        });

        container.appendChild(children);

    }

    return container;

}

function renderizarArvore(raiz){

    const div = document.getElementById("arvore");

    div.innerHTML = "";

    div.appendChild(
        criarNoHTML(raiz)
    );

}

iniciar();

