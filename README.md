# Sistema de Gerenciamento de uma Fábrica de Produtos Capilares

## 1. Descrição do projeto

Este projeto consiste no desenvolvimento de um sistema em linguagem C para gerenciar os produtos, clientes e compras de uma fábrica de produtos capilares.

O sistema permite cadastrar, consultar, listar e remover produtos e clientes, além de registrar e consultar as compras realizadas pelos clientes. Também realiza experimentos para analisar o comportamento da árvore 2-3 em diferentes ordens de inserção de CNPJs.

## 2. Estruturas de dados utilizadas

O projeto utiliza as seguintes estruturas:

- **Árvore 2-3 de produtos:** armazena os produtos organizados pelo código.
- **Árvore 2-3 de clientes:** armazena os clientes organizados pelo CNPJ.
- **Árvore 2-3 de compras:** cada cliente possui sua própria árvore de compras, organizada pela data e pelo código da compra.
- **Lista encadeada:** utilizada para armazenar os produtos pertencentes a cada compra, incluindo quantidade, valor unitário e subtotal.

## 3. Funcionalidades

O sistema disponibiliza as seguintes operações:

1. Cadastrar produto.
2. Buscar produto.
3. Remover produto.
4. Listar produtos.
5. Cadastrar cliente/revendedor.
6. Buscar cliente/revendedor.
7. Remover cliente/revendedor.
8. Listar clientes/revendedores.
9. Registrar compra de cliente.
10. Consultar compra de cliente.
11. Remover compra de cliente.
12. Listar compras de um cliente.
13. Executar experimentos com CNPJs.

## 4. Organização dos arquivos

O projeto está dividido em quatro arquivos:

- **estruturas.h:** contém as definições das estruturas de dados, enumerações e protótipos das funções.
- **funcoes.c:** contém a implementação das operações das árvores 2-3, cadastros, consultas, remoções e gerenciamento das compras.
- **main.c:** contém a função principal e o menu de interação com o usuário.
- **experimentos.c:** contém as funções responsáveis pela geração dos CNPJs e pela execução dos experimentos de inserção e busca.

## 5. Compilação

Para compilar o programa utilizando o GCC, execute o seguinte comando no terminal, dentro da pasta do projeto:

```bash
gcc -Wall -Wextra -Wpedantic -std=c11 main.c funcoes.c experimentos.c -o programa
```

## 6. Execução

Após a compilação, execute o programa com o comando:

**Windows:**
```bash
.\programa.exe
```

**Linux:**
```bash
./programa
```

Em seguida, o menu principal será apresentado, permitindo selecionar a operação desejada.

## 7. Experimentos

O sistema realiza experimentos com 100 CNPJs, considerando quatro situações de inserção:

- Ordem crescente;
- Ordem decrescente;
- Elemento central inserido primeiro, seguido dos demais em ordem aleatória;
- Ordem totalmente aleatória.

Em cada situação, são realizadas buscas pelos mesmos dez CNPJs cadastrados, além de uma busca por um CNPJ inexistente. O programa contabiliza a quantidade de nós visitados durante cada busca, permitindo comparar os resultados obtidos nas diferentes ordens de inserção.

## 8. Considerações finais

O projeto permite aplicar os conceitos de árvores 2-3 e listas encadeadas na resolução de um problema prático. A organização das informações em estruturas adequadas facilita o gerenciamento dos dados e permite analisar o comportamento das operações de busca, inserção e remoção.