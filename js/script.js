const container = document.getElementById("container");
let id = 0;
fetch("./clientes.json")
    .then(resp => resp.json())
    .then(clientes => {
        id++;
        container.innerHTML = clientes.map((pessoa, index) => `
            <div class="" id="card">
                <p>Nome: ${pessoa.nome}<p>
                <p>CPF: ${pessoa.cpf}</p>
                <p>Idade: ${pessoa.idade}</p>
            </div>
        `).join("");
    })
    .catch(erro => console.error(erro));