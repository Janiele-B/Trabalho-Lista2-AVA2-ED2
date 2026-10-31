#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_CNPJ 19
#define TAM_NOME 100
#define TAM_ENDERECO 150
#define TAM_CONTATO 50
#define TAM_PRODUTO 100
#define MAX_FAIXAS_PRECO 10

typedef enum { 
    SHAMPOO, 
    CONDICIONADOR, 
    FINALIZADOR, 
    MASCARA_HIDRATANTE, 
    OLEO_REPARADOR 
} TipoProduto;

typedef enum { 
    CABELO_LISO, 
    CABELO_ONDULADO, 
    CABELO_CRESPO 
} TipoCabelo;

typedef enum { 
    CABELO_OLEOSO, 
    CABELO_NORMAL, 
    CABELO_SECO, 
    CABELO_POS_QUIMICA 
} CondicaoCabelo;

typedef enum { 
    TAMANHO_300ML, 
    TAMANHO_500ML, 
    TAMANHO_1L 
} TamanhoProduto;

typedef enum { 
    INFO_PRODUTO, 
    INFO_CLIENTE, 
    INFO_COMPRA 
} TipoInfo;

typedef struct { 
    int quantidadeMin; 
    int quantidadeMax; 
    float precoUnitario; 
} FaixaPreco;

typedef struct Produto {
    int codigo;
    char nome[TAM_PRODUTO];
    TipoProduto tipo;
    TipoCabelo tipoCabelo;
    CondicaoCabelo condicaoCabelo;
    TamanhoProduto tamanho;
    int estoque;
    FaixaPreco faixasPreco[MAX_FAIXAS_PRECO];
    int quantidadeFaixas;
} Produto;

typedef struct { 
    int dia; 
    int mes; 
    int ano; 
} Data;

typedef struct ItemCompra {
    int codigoProduto;
    int quantidade;
    float valorUnitario;
    float subtotal;
    struct ItemCompra *prox;
} ItemCompra;

typedef struct Compra {
    int codigo;
    Data dataCompra;
    ItemCompra *itens;
    float valorTotal;
} Compra;

typedef struct No23 No23;

typedef struct Cliente {
    char cnpj[TAM_CNPJ];
    char nome[TAM_NOME];
    char endereco[TAM_ENDERECO];
    char contato[TAM_CONTATO];
    No23 *compras;
} Cliente;

struct No23 {
    void *info1;
    void *info2;
    No23 *esq;
    No23 *centro;
    No23 *dir;
    int quantidadeInfos;
};
typedef struct { 
    No23 *produtos; 
    No23 *clientes; 
} Fabrica;

int compararInfo(void *a, void *b, int tipo);

int inserir23(No23 **raiz, void *info, int tipo);

void *buscar23(No23 *raiz, void *chave, int tipo);

int buscar23ComPassos(No23 *raiz, void *chave, int tipo, int *passos);

int remover23(No23 **raiz, void *chave, int tipo);

void liberarArvore23(No23 *raiz, int tipo);

void imprimirArvore23(No23 *raiz, int tipo);

int compararDatas(Data a, Data b);

int adicionarItemCompra(Compra *compra, int codigoProduto, int quantidade, float valorUnitario);

void liberarItensCompra(ItemCompra *itens);

Produto *cadastrarProduto(void);

Cliente *cadastrarCliente(void);

Compra *cadastrarCompra(void);

void listarProduto(Produto *produto);

void listarCliente(Cliente *cliente);

void listarCompra(Compra *compra);

void listarProdutos(No23 *raiz);

void listarClientes(No23 *raiz);

void listarCompras(No23 *raiz);

void registrarCompra(Fabrica *fabrica);

void consultarCompra(Cliente *cliente);

void removerCompra(Cliente *cliente, Fabrica *fabrica);

void menu_experimento(void);

void gerarCNPJ(char *cnpj, int numero);

void embaralhar(int valores[], int quantidade);

void executarOrdem(int ordem[], int quantidade, int modo, int buscas[]);

void limparBufferMain(void);

#endif
