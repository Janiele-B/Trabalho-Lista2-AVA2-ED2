#include "estruturas.h"


int compararDatas(Data a, Data b)
{
    int resultado = 0;
    if (a.ano < b.ano) 
        resultado = -1;
    else if (a.ano > b.ano) 
        resultado = 1;
    else if (a.mes < b.mes) 
        resultado = -1;
    else if (a.mes > b.mes) 
        resultado = 1;
    else if (a.dia < b.dia) 
        resultado = -1;
    else if (a.dia > b.dia) 
        resultado = 1;
    return resultado;
}

int compararInfo(void *a, void *b, int tipo)
{
    int resultado = 0;
    if (tipo == INFO_PRODUTO) {
        Produto *x = (Produto *)a, *y = (Produto *)b;

        if (x->codigo < y->codigo) 
            resultado = -1;
        else if (x->codigo > y->codigo) 
            resultado = 1;

    } else if (tipo == INFO_CLIENTE) {
        Cliente *x = (Cliente *)a, *y = (Cliente *)b;
        resultado = strcmp(x->cnpj, y->cnpj);

    } else {
        Compra *x = (Compra *)a, *y = (Compra *)b;
        resultado = compararDatas(x->dataCompra, y->dataCompra);

        if (resultado == 0) {
            if (x->codigo < y->codigo) 
                resultado = -1;
            else if (x->codigo > y->codigo) 
                resultado = 1;
        }
    }
    return resultado;
}

No23 *criarNo23(void *info)
{
    No23 *novo = (No23 *)malloc(sizeof(No23));
    if (novo != NULL) {
        novo->info1 = info;
        novo->info2 = NULL;
        novo->esq = NULL;
        novo->centro = NULL;
        novo->dir = NULL;
        novo->quantidadeInfos = 1;
    }
    return novo;
}

int ehFolha(No23 *no)
{
    int resultado = 0;
    if (no != NULL && no->esq == NULL && no->centro == NULL && no->dir == NULL)
        resultado = 1;
    return resultado;
}


int inserirRecursivo(No23 **raiz, void *info, int tipo, void **promovida, No23 **filhoDireito)
{
    int resultado = 1;
    *promovida = NULL;
    *filhoDireito = NULL;

    if (*raiz == NULL) {
        *raiz = criarNo23(info);

        if (*raiz == NULL) 
            resultado = 0;

    } else if (ehFolha(*raiz)) {
        No23 *no = *raiz;
        int cmp1 = compararInfo(info, no->info1, tipo);

        if (cmp1 == 0) 
            resultado = -1;

        else if (no->quantidadeInfos == 1) {
            if (cmp1 < 0) {
                no->info2 = no->info1;
                no->info1 = info;
            } else no->info2 = info;

            no->quantidadeInfos = 2;
        } else {
            void *a = no->info1, *b = no->info2, *menor, *meio, *maior;
            int c1 = compararInfo(info, a, tipo);
            int c2 = compararInfo(info, b, tipo);

            if (c1 < 0){
                menor = info; 
                meio = a; 
                maior = b; 
            }
            else if (c2 < 0) { 
                menor = a; 
                meio = info; 
                maior = b; 
            }
            else if (c2 == 0)
                resultado = -1;
            
            else { 
                menor = a; 
                meio = b; 
                maior = info; 
            }
            if (resultado == 1) {
                No23 *direita = criarNo23(maior);
                if (direita == NULL) resultado = 0;
                else {
                    no->info1 = menor;
                    no->info2 = NULL;
                    no->quantidadeInfos = 1;
                    *promovida = meio;
                    *filhoDireito = direita;
                }
            }
        }
    } else {
        No23 *no = *raiz;
        No23 **destino;

        if (compararInfo(info, no->info1, tipo) < 0) 
        destino = &no->esq;

        else if (no->quantidadeInfos == 1 || compararInfo(info, no->info2, tipo) < 0) 
            destino = &no->centro;

        else destino = &no->dir;

        void *subPromovida = NULL;
        No23 *subDireito = NULL;
        resultado = inserirRecursivo(destino, info, tipo, &subPromovida, &subDireito);

        if (resultado == 1 && subPromovida != NULL) {
            if (no->quantidadeInfos == 1) {
                if (destino == &no->esq) {
                    no->info2 = no->info1;
                    no->info1 = subPromovida;
                    no->dir = no->centro;
                    no->centro = subDireito;
                } else {
                    no->info2 = subPromovida;
                    no->dir = subDireito;
                }
                no->quantidadeInfos = 2;
            } else {
                void *k1 = no->info1, *k2 = no->info2;
                No23 *c0 = no->esq, *c1 = no->centro, *c2 = no->dir;
                void *menor, *meio, *maior;
                No23 *nEsq = NULL, *nDir = NULL;

                if (destino == &no->esq) {
                    menor = subPromovida; 
                    meio = k1; 
                    maior = k2;
                    nEsq = subDireito;
                    nDir = criarNo23(maior);

                    if (nDir != NULL) { 
                        nDir->esq = c1; 
                        nDir->centro = c2; 
                    }
                    no->info1 = menor;
                    no->info2 = NULL;
                    no->quantidadeInfos = 1;
                    no->esq = c0;
                    no->centro = nEsq;
                    no->dir = NULL;

                } else if (destino == &no->centro) {
                    menor = k1; 
                    meio = subPromovida; 
                    maior = k2;
                    nEsq = criarNo23(menor);
                    nDir = criarNo23(maior);

                    if (nEsq != NULL && nDir != NULL) {
                        nEsq->esq = c0; 
                        nEsq->centro = c1;
                        nDir->esq = subDireito; 
                        nDir->centro = c2;
                        no->info1 = meio; 
                        no->info2 = NULL; 
                        no->quantidadeInfos = 1;
                        no->esq = nEsq; 
                        no->centro = nDir; 
                        no->dir = NULL;
                    }
                } else {
                    menor = k1; 
                    meio = k2; 
                    maior = subPromovida;
                    nEsq = criarNo23(menor);

                    if (nEsq != NULL) { 
                        nEsq->esq = c0; 
                        nEsq->centro = c1; 
                    }
                    nDir = subDireito;
                    no->info1 = meio; 
                    no->info2 = NULL; 
                    no->quantidadeInfos = 1;
                    no->esq = nEsq; 
                    no->centro = nDir; 
                    no->dir = NULL;
                }
                if (nEsq == NULL || nDir == NULL) 
                    resultado = 0;
                else {
                    *promovida = meio;
                    *filhoDireito = nDir;
                    
                    if (destino == &no->esq) {
                        no->info1 = menor;
                        no->esq = c0;
                        no->centro = subDireito;
                        no->dir = NULL;
                    } else if (destino == &no->centro) {
                        no->info1 = menor;
                        no->esq = nEsq->esq;
                        no->centro = nEsq->centro;
                        no->dir = NULL;
                    } else {
                        no->info1 = menor;
                        no->esq = c0;
                        no->centro = c1;
                        no->dir = NULL;
                    }
                    no->info2 = NULL;
                    no->quantidadeInfos = 1;
                    if (destino == &no->centro) 
                        free(nEsq);
                }
            }
        }
    }
    return resultado;
}

int inserir23(No23 **raiz, void *info, int tipo)
{
    int resultado = 1;
    void *promovida = NULL;
    No23 *filhoDireito = NULL;

    resultado = inserirRecursivo(raiz, info, tipo, &promovida, &filhoDireito);

    if (resultado == 1 && promovida != NULL) {
        No23 *novaRaiz = criarNo23(promovida);

        if (novaRaiz == NULL) 
            resultado = 0;
        else {
            novaRaiz->esq = *raiz;
            novaRaiz->centro = filhoDireito;
            *raiz = novaRaiz;
        }
    }
    return resultado;
}

void *buscar23(No23 *raiz, void *chave, int tipo)
{
    void *resultado = NULL;
    No23 *atual = raiz;

    while (atual != NULL && resultado == NULL) {
        int c1 = compararInfo(chave, atual->info1, tipo);
        if (c1 == 0) 
            resultado = atual->info1;
        else if (c1 < 0) 
            atual = atual->esq;
        else if (atual->quantidadeInfos == 1) 
            atual = atual->centro;
        else {
            int c2 = compararInfo(chave, atual->info2, tipo);
            if (c2 == 0) 
                resultado = atual->info2;
            else if (c2 < 0) 
                atual = atual->centro;
            else atual = atual->dir;
        }
    }
    return resultado;
}

int buscar23ComPassos(No23 *raiz, void *chave, int tipo, int *passos)
{
    int encontrado = 0;
    No23 *atual = raiz;
    *passos = 0;

    while (atual != NULL && encontrado == 0) {
        (*passos)++;
        int c1 = compararInfo(chave, atual->info1, tipo);

        if (c1 == 0) 
            encontrado = 1;

        else if (c1 < 0)   
            atual = atual->esq;

        else if (atual->quantidadeInfos == 1) 
            atual = atual->centro;
        else {
            int c2 = compararInfo(chave, atual->info2, tipo);

            if (c2 == 0) 
                encontrado = 1;
            else if (c2 < 0) 
                atual = atual->centro;
            else atual = atual->dir;
        }
    }
    return encontrado;
}


void *buscarPredecessor(No23 *no)
{
    No23 *atual = no;
    while (atual->centro != NULL) 
        atual = atual->centro;

    if (atual->quantidadeInfos == 2) 
        return atual->info2;
    return atual->info1;
}

int removerFolha(No23 *no, void *chave, int tipo)
{
    int resultado = 0;
    if (compararInfo(no->info1, chave, tipo) == 0) {
        if (no->quantidadeInfos == 2) {
            no->info1 = no->info2;
            no->info2 = NULL;
            no->quantidadeInfos = 1;
        } else {
            no->info1 = NULL;
            no->quantidadeInfos = 0;
        }
        resultado = 1;
    } else if (no->quantidadeInfos == 2 && compararInfo(no->info2, chave, tipo) == 0) {
        no->info2 = NULL;
        no->quantidadeInfos = 1;
        resultado = 1;
    }
    return resultado;
}


int removerRecursivo(No23 **raiz, void *chave, int tipo)
{
    int resultado = 0;
    No23 *no = *raiz;
    if (no != NULL) {
        int c1 = compararInfo(chave, no->info1, tipo);
        int c2 = 1;
        if (no->quantidadeInfos == 2) 
            c2 = compararInfo(chave, no->info2, tipo);

        if (c1 == 0 || (no->quantidadeInfos == 2 && c2 == 0)) {
            resultado = 1;
            if (!ehFolha(no)) {
                int usarPrimeira = 0;

                if (c1 == 0) 
                    usarPrimeira = 1;

                void **alvo;
                No23 **sub;

                if (usarPrimeira) { 
                    alvo = &no->info1; 
                    sub = &no->esq; 
                }
                else { 
                    alvo = &no->info2; 
                    sub = &no->centro; 
                }
                void *pred = buscarPredecessor(*sub);
                *alvo = pred;
                removerRecursivo(sub, pred, tipo);
            } else removerFolha(no, chave, tipo);

        } else {
            No23 **sub;
            if (c1 > 0) 
                sub = &no->esq;

            else if (no->quantidadeInfos == 1 || c2 > 0) 
                sub = &no->centro;

            else sub = &no->dir;
            resultado = removerRecursivo(sub, chave, tipo);
        }


        if (resultado == 1 && no->quantidadeInfos > 0) {
            No23 **filhoVazio = NULL;

            if (no->esq != NULL && no->esq->quantidadeInfos == 0) 
                filhoVazio = &no->esq;
            
            else if (no->centro != NULL && no->centro->quantidadeInfos == 0) 
                filhoVazio = &no->centro;

            else if (no->dir != NULL && no->dir->quantidadeInfos == 0) 
                filhoVazio = &no->dir;
            

            if (filhoVazio != NULL) {
                if (no->quantidadeInfos == 2) {
                    if (filhoVazio == &no->esq) {
                        No23 *irmao = no->centro;
                        if (irmao->quantidadeInfos == 2) {
                            (*filhoVazio)->info1 = no->info1;
                            (*filhoVazio)->quantidadeInfos = 1;
                            no->info1 = irmao->info1;
                            irmao->info1 = irmao->info2;
                            irmao->info2 = NULL;
                            irmao->quantidadeInfos = 1;
                            (*filhoVazio)->esq = (*filhoVazio)->centro;
                            (*filhoVazio)->centro = irmao->esq;
                            irmao->esq = irmao->centro;
                            irmao->centro = irmao->dir;
                            irmao->dir = NULL;
                        } else {
                            No23 *v = *filhoVazio;
                            v->info1 = no->info1;
                            v->info2 = irmao->info1;
                            v->quantidadeInfos = 2;
                            v->esq = v->centro;
                            v->centro = irmao->esq;
                            v->dir = irmao->centro;
                            no->info1 = no->info2;
                            no->info2 = NULL;
                            no->quantidadeInfos = 1;
                            no->centro = no->dir;
                            no->dir = NULL;
                            free(irmao);
                        }
                    } else if (filhoVazio == &no->centro) {
                        No23 *irmao = no->esq;
                        if (irmao->quantidadeInfos == 2) {
                            (*filhoVazio)->info1 = no->info1;
                            (*filhoVazio)->quantidadeInfos = 1;
                            no->info1 = irmao->info2;
                            irmao->info2 = NULL;
                            irmao->quantidadeInfos = 1;
                            (*filhoVazio)->esq = irmao->dir;
                        } else {
                            irmao->info2 = no->info1;
                            irmao->quantidadeInfos = 2;
                            irmao->dir = (*filhoVazio)->centro;
                            no->info1 = no->info2;
                            no->info2 = NULL;
                            no->quantidadeInfos = 1;
                            no->esq = irmao;
                            no->centro = no->dir;
                            no->dir = NULL;
                            free(*filhoVazio);
                        }
                    } else {
                        No23 *irmao = no->centro;

                        if (irmao->quantidadeInfos == 2) {
                            (*filhoVazio)->info1 = no->info2;
                            (*filhoVazio)->quantidadeInfos = 1;
                            no->info2 = irmao->info2;
                            irmao->info2 = NULL;
                            irmao->quantidadeInfos = 1;
                            (*filhoVazio)->centro = (*filhoVazio)->esq;
                            (*filhoVazio)->esq = irmao->dir;
                        } else {
                            irmao->info2 = no->info2;
                            irmao->quantidadeInfos = 2;
                            irmao->dir = (*filhoVazio)->esq;
                            no->info2 = NULL;
                            no->quantidadeInfos = 1;
                            no->dir = NULL;
                            free(*filhoVazio);
                        }
                    }
                } else {
                    No23 *irmao = NULL;

                    if (filhoVazio == &no->esq) 
                        irmao = no->centro;
                    else irmao = no->esq;

                    if (irmao != NULL && irmao->quantidadeInfos == 2) {
                        if (filhoVazio == &no->esq) {
                            (*filhoVazio)->info1 = no->info1;
                            no->info1 = irmao->info1;
                            irmao->info1 = irmao->info2;
                            irmao->info2 = NULL;
                            irmao->quantidadeInfos = 1;
                            (*filhoVazio)->esq = (*filhoVazio)->centro;
                            (*filhoVazio)->centro = irmao->esq;
                            irmao->esq = irmao->centro;
                            irmao->centro = irmao->dir;
                            irmao->dir = NULL;
                        } else {
                            (*filhoVazio)->info1 = no->info1;
                            no->info1 = irmao->info2;
                            irmao->info2 = NULL;
                            irmao->quantidadeInfos = 1;
                            (*filhoVazio)->centro = (*filhoVazio)->esq;
                            (*filhoVazio)->esq = irmao->dir;
                        }
                        (*filhoVazio)->quantidadeInfos = 1;
                    } else {
                        if (filhoVazio == &no->esq) {
                            irmao->info2 = no->info1;
                            irmao->quantidadeInfos = 2;
                            irmao->dir = (*filhoVazio)->centro;
                            no->info1 = NULL;
                            no->quantidadeInfos = 0;
                            no->esq = irmao;
                            no->centro = NULL;
                        } else {
                            irmao->info2 = irmao->info1;
                            irmao->info1 = no->info1;
                            irmao->quantidadeInfos = 2;
                            irmao->dir = irmao->centro;
                            irmao->centro = irmao->esq;
                            irmao->esq = (*filhoVazio)->centro;
                            no->info1 = NULL;
                            no->quantidadeInfos = 0;
                            no->centro = NULL;
                        }
                        free(*filhoVazio);
                    }
                }
            }
        }
    }
    return resultado;
}

int remover23(No23 **raiz, void *chave, int tipo)
{
    int resultado = removerRecursivo(raiz, chave, tipo);
    if (resultado == 1 && *raiz != NULL && (*raiz)->quantidadeInfos == 0) {
        No23 *antiga = *raiz;

        if (antiga->esq != NULL) 
            *raiz = antiga->esq;

        else if (antiga->centro != NULL) 
            *raiz = antiga->centro;

        else *raiz = NULL;
        free(antiga);
    }
    return resultado;
}

void liberarItensCompra(ItemCompra *itens)
{
    ItemCompra *atual = itens;
    while (atual != NULL) {
        ItemCompra *prox = atual->prox;
        free(atual);
        atual = prox;
    }
}

void liberarArvore23(No23 *raiz, int tipo)
{
    if (raiz != NULL) {
        liberarArvore23(raiz->esq, tipo);
        liberarArvore23(raiz->centro, tipo);
        liberarArvore23(raiz->dir, tipo);
        if (tipo == INFO_COMPRA) {
            if (raiz->info1 != NULL) {
                Compra *c = (Compra *)raiz->info1;
                liberarItensCompra(c->itens);
                free(c);
            }
            if (raiz->info2 != NULL) {
                Compra *c = (Compra *)raiz->info2;
                liberarItensCompra(c->itens);
                free(c);
            }
        } else if (tipo == INFO_PRODUTO) {
            free(raiz->info1);
            free(raiz->info2);
        } else {
            if (raiz->info1 != NULL) {
                Cliente *c = (Cliente *)raiz->info1;
                liberarArvore23(c->compras, INFO_COMPRA);
                free(c);
            }
            if (raiz->info2 != NULL) {
                Cliente *c = (Cliente *)raiz->info2;
                liberarArvore23(c->compras, INFO_COMPRA);
                free(c);
            }
        }
        free(raiz);
    }
}

void imprimirArvore23(No23 *raiz, int tipo)
{
    if (raiz != NULL) {
        imprimirArvore23(raiz->esq, tipo);

        if (tipo == INFO_PRODUTO) 
            printf("%d ", ((Produto *)raiz->info1)->codigo);

        else if (tipo == INFO_CLIENTE) 
            printf("%s ", ((Cliente *)raiz->info1)->cnpj);
            
        else 
            printf("%02d/%02d/%04d ", ((Compra *)raiz->info1)->dataCompra.dia,((Compra *)raiz->info1)->dataCompra.mes, ((Compra *)raiz->info1)->dataCompra.ano);
                    
        imprimirArvore23(raiz->centro, tipo);

        if (raiz->quantidadeInfos == 2) {
            if (tipo == INFO_PRODUTO) 
                printf("%d ", ((Produto *)raiz->info2)->codigo);

            else if (tipo == INFO_CLIENTE) 
                printf("%s ", ((Cliente *)raiz->info2)->cnpj);

            else printf("%02d/%02d/%04d ", ((Compra *)raiz->info2)->dataCompra.dia,((Compra *)raiz->info2)->dataCompra.mes, ((Compra *)raiz->info2)->dataCompra.ano);
                        
            imprimirArvore23(raiz->dir, tipo);
        }
    }
}

int adicionarItemCompra(Compra *compra, int codigoProduto, int quantidade, float valorUnitario)
{
    int resultado = 0;
    ItemCompra *novo = (ItemCompra *)malloc(sizeof(ItemCompra));
    if (novo != NULL && quantidade > 0) {
        novo->codigoProduto = codigoProduto;
        novo->quantidade = quantidade;
        novo->valorUnitario = valorUnitario;
        novo->subtotal = quantidade * valorUnitario;
        novo->prox = compra->itens;
        compra->itens = novo;
        compra->valorTotal += novo->subtotal;
        resultado = 1;
    } else if (novo != NULL) free(novo);
    return resultado;
}

void lerTexto(char *destino, int tamanho)
{
    size_t n;
    if (fgets(destino, tamanho, stdin) != NULL) {
        n = strlen(destino);
        if (n > 0 && destino[n - 1] == '\n') destino[n - 1] = '\0';
    }
}

void limparEntrada(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

Produto *cadastrarProduto(void)
{
    Produto *p = (Produto *)calloc(1, sizeof(Produto));
    if (p != NULL) {
        printf("Codigo: "); 
        scanf("%d", &p->codigo); 
        limparEntrada();

        printf("Nome: "); 
        lerTexto(p->nome, TAM_PRODUTO);

        printf("Tipo (0-Shampoo, 1-Condicionador, 2-Finalizador, 3-Mascara, 4-Oleo): ");
        scanf("%d", (int *)&p->tipo);

        printf("Tipo de cabelo (0-Liso, 1-Ondulado, 2-Crespo): "); 
        scanf("%d", (int *)&p->tipoCabelo);
        printf("Condicao (0-Oleoso, 1-Normal, 2-Seco, 3-Pos-quimica): "); 
        scanf("%d", (int *)&p->condicaoCabelo);
        printf("Tamanho (0-300ml, 1-500ml, 2-1L): "); 
        scanf("%d", (int *)&p->tamanho);
        printf("Estoque inicial: "); 
        scanf("%d", &p->estoque);
        printf("Quantidade de faixas de preco (1 a %d): ", MAX_FAIXAS_PRECO);
        scanf("%d", &p->quantidadeFaixas);

        if (p->quantidadeFaixas < 1 || p->quantidadeFaixas > MAX_FAIXAS_PRECO || p->estoque < 0) {
            printf("Dados invalidos.\n"); free(p); p = NULL;
        } else {
            int i;
            for (i = 0; i < p->quantidadeFaixas; i++) {
                printf("Faixa %d - quantidade minima, maxima e preco unitario: ", i + 1);
                scanf("%d%d%f", &p->faixasPreco[i].quantidadeMin, &p->faixasPreco[i].quantidadeMax, &p->faixasPreco[i].precoUnitario);
                     
            }
        }
        limparEntrada();
    }
    return p;
}

Cliente *cadastrarCliente(void)
{
    Cliente *c = (Cliente *)calloc(1, sizeof(Cliente));
    if (c != NULL) {
        printf("CNPJ (14 digitos): "); 
        lerTexto(c->cnpj, TAM_CNPJ);

        printf("Nome: "); 
        lerTexto(c->nome, TAM_NOME);

        printf("Endereco: "); 
        lerTexto(c->endereco, TAM_ENDERECO);

        printf("Contato: "); 
        lerTexto(c->contato, TAM_CONTATO);

        if (strlen(c->cnpj) == 0) { 
            free(c); 
            c = NULL; }
    }
    return c;
}

Compra *cadastrarCompra(void)
{
    Compra *c = (Compra *)calloc(1, sizeof(Compra));
    if (c != NULL) {
        printf("Codigo da compra: "); 
        scanf("%d", &c->codigo);

        printf("Data (dia mes ano): "); 
        scanf("%d%d%d", &c->dataCompra.dia, &c->dataCompra.mes, &c->dataCompra.ano);

        if (c->dataCompra.dia < 1 || c->dataCompra.dia > 31 ||c->dataCompra.mes < 1 || c->dataCompra.mes > 12 || c->dataCompra.ano < 1) {
            
            printf("Data invalida.\n"); free(c); c = NULL;
        }
        limparEntrada();
    }
    return c;
}

void listarProduto(Produto *p)
{
    int i;
    if (p != NULL) {
        printf("\nCodigo: %d | Nome: %s | Estoque: %d\n", p->codigo, p->nome, p->estoque);
        printf("Tipo: %d | Cabelo: %d | Condicao: %d | Tamanho: %d\n",p->tipo, p->tipoCabelo, p->condicaoCabelo, p->tamanho);
               
        for (i = 0; i < p->quantidadeFaixas; i++)
            printf("  De %d a %d unidades: R$ %.2f\n", p->faixasPreco[i].quantidadeMin,p->faixasPreco[i].quantidadeMax, p->faixasPreco[i].precoUnitario);
                   
    }
}

void listarCliente(Cliente *c)
{
    if (c != NULL)
        printf("\nCNPJ: %s | Nome: %s | Endereco: %s | Contato: %s\n",c->cnpj, c->nome, c->endereco, c->contato);
               
}

void listarCompra(Compra *c)
{
    ItemCompra *it;
    if (c != NULL) {
        printf("\nCompra %d - %02d/%02d/%04d - Total: R$ %.2f\n", c->codigo,c->dataCompra.dia, c->dataCompra.mes, c->dataCompra.ano, c->valorTotal);
               
        it = c->itens;
        while (it != NULL) {
            printf("  Produto %d | Quantidade %d | Unitario R$ %.2f | Subtotal R$ %.2f\n",it->codigoProduto, it->quantidade, it->valorUnitario, it->subtotal);
                   
            it = it->prox;
        }
    }
}

void listarProdutosRec(No23 *r)
{
    if (r != NULL) {
        listarProdutosRec(r->esq); 
        listarProduto((Produto *)r->info1);
        listarProdutosRec(r->centro);

        if (r->quantidadeInfos == 2) { 
            listarProduto((Produto *)r->info2); 
            listarProdutosRec(r->dir); 
        }
    }
}
void listarProdutos(No23 *r) { 
    listarProdutosRec(r); 
}

void listarClientesRec(No23 *r)
{
    if (r != NULL) {
        listarClientesRec(r->esq); 
        listarCliente((Cliente *)r->info1);
        listarClientesRec(r->centro);

        if (r->quantidadeInfos == 2) { 
            listarCliente((Cliente *)r->info2); 
            listarClientesRec(r->dir); 
        }
    }
}
void listarClientes(No23 *r) { 
    listarClientesRec(r); 
}

void listarComprasRec(No23 *r)
{
    if (r != NULL) {
        listarComprasRec(r->esq); 
        listarCompra((Compra *)r->info1);
        listarComprasRec(r->centro);

        if (r->quantidadeInfos == 2) { 
            listarCompra((Compra *)r->info2); 
            listarComprasRec(r->dir); 
        }
    }
}
void listarCompras(No23 *r) { listarComprasRec(r); }

float obterPreco(Produto *p, int quantidade)
{
    int i;
    float preco = 0.0f;
    for (i = 0; i < p->quantidadeFaixas; i++) {
        if (quantidade >= p->faixasPreco[i].quantidadeMin &&quantidade <= p->faixasPreco[i].quantidadeMax) {
            preco = p->faixasPreco[i].precoUnitario;
            break;
        }
    }
    return preco;
}

void registrarCompra(Fabrica *fabrica)
{
    char cnpj[TAM_CNPJ];
    Cliente chaveCliente;
    Cliente *cliente;
    Compra *compra;
    int continuar = 1;

    printf("CNPJ do cliente: "); 
    lerTexto(cnpj, TAM_CNPJ);

    strcpy(chaveCliente.cnpj, cnpj);
    cliente = (Cliente *)buscar23(fabrica->clientes, &chaveCliente, INFO_CLIENTE);

    if (cliente == NULL) 
        printf("Cliente nao encontrado.\n");
    else {
        compra = cadastrarCompra();
        if (compra != NULL) {
            while (continuar == 1) {
                int codigo, quantidade;
                Produto chaveProduto, *produto;

                printf("Codigo do produto (0 encerra): "); 
                scanf("%d", &codigo);

                if (codigo == 0) 
                    continuar = 0;
                else {
                    printf("Quantidade: "); 
                    scanf("%d", &quantidade);

                    chaveProduto.codigo = codigo;
                    produto = (Produto *)buscar23(fabrica->produtos, &chaveProduto, INFO_PRODUTO);

                    if (produto == NULL) 
                        printf("Produto nao encontrado.\n");

                    else if (quantidade <= 0 || quantidade > produto->estoque) 
                        printf("Quantidade indisponivel.\n");

                    else {
                        float preco = obterPreco(produto, quantidade);

                        if (preco <= 0.0f) 
                            printf("Nao existe faixa de preco para essa quantidade.\n");
                        else if (adicionarItemCompra(compra, codigo, quantidade, preco) == 1) {
                            produto->estoque -= quantidade;
                            printf("Item adicionado.\n");
                        }
                    }
                }
            }
            limparEntrada();
            if (compra->itens == NULL) { 
                free(compra); 
                printf("Compra cancelada: nenhum item.\n");
            }
            else if (inserir23(&cliente->compras, compra, INFO_COMPRA) != 1) {
                printf("Compra nao inserida (codigo/data repetidos ou falta de memoria).\n");

                ItemCompra *it = compra->itens;

                while (it != NULL) {
                    Produto chave;
                    Produto *p;
                    chave.codigo = it->codigoProduto;
                    p = (Produto *)buscar23(fabrica->produtos, &chave, INFO_PRODUTO);
                    if (p != NULL) p->estoque += it->quantidade;
                    it = it->prox;
                }
                liberarItensCompra(compra->itens); 
                free(compra);

            } else printf("Compra registrada com sucesso.\n");
        }
    }
}

void consultarCompra(Cliente *cliente)
{
    Compra chave;
    Compra *encontrada;
    printf("Data da compra (dia mes ano): ");
    scanf("%d%d%d", &chave.dataCompra.dia, &chave.dataCompra.mes, &chave.dataCompra.ano);

    printf("Codigo da compra: "); 
    scanf("%d", &chave.codigo);

    encontrada = (Compra *)buscar23(cliente->compras, &chave, INFO_COMPRA);

    if (encontrada == NULL) 
        printf("Compra nao encontrada.\n");
    else listarCompra(encontrada);

    limparEntrada();
}

void removerCompra(Cliente *cliente)
{
    Compra chave;
    Compra *encontrada;

    printf("Data da compra (dia mes ano): ");
    scanf("%d%d%d", &chave.dataCompra.dia, &chave.dataCompra.mes, &chave.dataCompra.ano);

    printf("Codigo da compra: "); 
    scanf("%d", &chave.codigo);

    encontrada = (Compra *)buscar23(cliente->compras, &chave, INFO_COMPRA);

    if (encontrada == NULL) 
        printf("Compra nao encontrada.\n");
    else {
        if (remover23(&cliente->compras, &chave, INFO_COMPRA) == 1) {
            printf("Compra removida. Os itens e o registro foram liberados.\n");
            liberarItensCompra(encontrada->itens);
            free(encontrada);
        }
    }
    limparEntrada();
}
