#include "estruturas.h"

int compararDatas(Data a, Data b)
{
    int r = 0;
    if (a.ano < b.ano) 
        r = -1;
    else if (a.ano > b.ano) 
        r = 1;
    else if (a.mes < b.mes) 
        r = -1;
    else if (a.mes > b.mes) 
        r = 1;
    else if (a.dia < b.dia) 
        r = -1;
    else if (a.dia > b.dia) 
        r = 1;
    return r;
}

int compararInfo(void *a, void *b, int tipo)
{
    int r = 0;
    if (tipo == INFO_PRODUTO) {
        Produto *x = a, *y = b;

        if (x->codigo < y->codigo) 
            r = -1;

        else if (x->codigo > y->codigo) 
            r = 1;

    } else if (tipo == INFO_CLIENTE) {
        Cliente *x = a, *y = b;
        r = strcmp(x->cnpj, y->cnpj);
    } else {
        Compra *x = a, *y = b;
        r = compararDatas(x->dataCompra, y->dataCompra);
        if (r == 0) {
            if (x->codigo < y->codigo) 
                r = -1;
            else if (x->codigo > y->codigo) 
                r = 1;
        }
    }
    return r;
}

No23 *criarNo23(void *info)
{
    No23 *n = malloc(sizeof(No23));
    if (n != NULL) {
        n->info1 = info; 
        n->info2 = NULL;
        n->esq = NULL; 
        n->centro = NULL; 
        n->dir = NULL;
        n->quantidadeInfos = 1;
    }
    return n;
}

int ehFolha(No23 *n)
{
    int r = 0;
    if (n != NULL && n->esq == NULL && n->centro == NULL && n->dir == NULL) 
        r = 1;
    return r;
}


int inserirRecursivo(No23 **raiz, void *info, int tipo, void **promovida, No23 **filhoDireito)
{
    int r = 1;
    *promovida = NULL;
    *filhoDireito = NULL;

    if (*raiz == NULL) {
        *raiz = criarNo23(info);

        if (*raiz == NULL) 
            r = 0;
    } else {
        No23 *n = *raiz;
        if (ehFolha(n)) {
            int c1 = compararInfo(info, n->info1, tipo);
            int c2 = 0;

            if (n->quantidadeInfos == 2) 
                c2 = compararInfo(info, n->info2, tipo);
            if (c1 == 0 || (n->quantidadeInfos == 2 && c2 == 0)) 
                r = -1;
            else if (n->quantidadeInfos == 1) {
                if (c1 < 0) { 
                    n->info2 = n->info1; 
                    n->info1 = info; 
                }
                else n->info2 = info;
                n->quantidadeInfos = 2;
            } else {
                void *k[3];
                No23 *novo;
                if (c1 < 0) { 
                    k[0] = info; 
                    k[1] = n->info1; 
                    k[2] = n->info2; 
                }
                else if (c2 < 0) {
                    k[0] = n->info1; 
                    k[1] = info; 
                    k[2] = n->info2; 
                }
                else { 
                    k[0] = n->info1; 
                    k[1] = n->info2; 
                    k[2] = info; 
                }
                novo = criarNo23(k[2]);

                if (novo == NULL) 
                    r = 0;
                else {
                    n->info1 = k[0]; 
                    n->info2 = NULL; 
                    n->quantidadeInfos = 1;
                    *promovida = k[1]; 
                    *filhoDireito = novo;
                }
            }
        } else {
            int indice;
            No23 **destino;
            void *subPromovida = NULL;
            No23 *subDireito = NULL;

            if (compararInfo(info, n->info1, tipo) < 0) { 
                indice = 0; 
                destino = &n->esq; 
            }
            else if (n->quantidadeInfos == 1 || compararInfo(info, n->info2, tipo) < 0) { 
                indice = 1; 
                destino = &n->centro; 
            }
            else { 
                indice = 2; 
                destino = &n->dir; 
            }
            r = inserirRecursivo(destino, info, tipo, &subPromovida, &subDireito);
            if (r == 1 && subPromovida != NULL) {
                void *k[3];
                No23 *c[4];
                int i;
                if (n->quantidadeInfos == 1) {
                    if (indice == 0) {
                        n->info2 = n->info1; 
                        n->info1 = subPromovida;
                        n->dir = n->centro; 
                        n->centro = subDireito;

                    } else { 
                        n->info2 = subPromovida; 
                        n->dir = subDireito; 
                    }
                    n->quantidadeInfos = 2;
                } else {
                    k[0] = n->info1; 
                    k[1] = n->info2;
                    c[0] = n->esq; 
                    c[1] = n->centro; 
                    c[2] = n->dir;

                    for (i = 2; i > indice; i--) 
                        k[i] = k[i - 1];

                    k[indice] = subPromovida;

                    for (i = 3; i > indice + 1; i--) 
                        c[i] = c[i - 1];

                    c[indice + 1] = subDireito;
                    {
                        No23 *direita = criarNo23(k[2]);
                        if (direita == NULL) 
                            r = 0;
                        else {
                            direita->esq = c[2]; 
                            direita->centro = c[3];
                            n->info1 = k[0]; 
                            n->info2 = NULL; 
                            n->quantidadeInfos = 1;
                            n->esq = c[0]; 
                            n->centro = c[1]; 
                            n->dir = NULL;
                            *promovida = k[1]; 
                            *filhoDireito = direita;
                        }
                    }
                }
            }
        }
    }
    return r;
}

int inserir23(No23 **raiz, void *info, int tipo)
{
    int r;
    void *promovida = NULL;
    No23 *direita = NULL;
    r = inserirRecursivo(raiz, info, tipo, &promovida, &direita);
    if (r == 1 && promovida != NULL) {
        No23 *nova = criarNo23(promovida);
        if (nova == NULL) 
            r = 0;
        else { 
            nova->esq = *raiz; 
            nova->centro = direita; 
            *raiz = nova; }
    }
    return r;
}

void *buscar23(No23 *raiz, void *chave, int tipo)
{
    void *r = NULL;
    No23 *n = raiz;
    while (n != NULL && r == NULL) {
        int c = compararInfo(chave, n->info1, tipo);

        if (c == 0) 
            r = n->info1;

        else if (c < 0) 
            n = n->esq;

        else if (n->quantidadeInfos == 1) 
            n = n->centro;
        else {
            c = compararInfo(chave, n->info2, tipo);
            if (c == 0) 
                r = n->info2;
            else if (c < 0) 
                n = n->centro;
            else 
                n = n->dir;
        }
    }
    return r;
}

int buscar23ComPassos(No23 *raiz, void *chave, int tipo, int *passos)
{
    int achou = 0;
    No23 *n = raiz;
    *passos = 0;
    while (n != NULL && achou == 0) {
        int c;
        (*passos)++;
        c = compararInfo(chave, n->info1, tipo);

        if (c == 0) 
            achou = 1;

        else if (c < 0) 
            n = n->esq;

        else if (n->quantidadeInfos == 1) 
            n = n->centro;
        else {
            c = compararInfo(chave, n->info2, tipo);
            if (c == 0) 
                achou = 1;
            else if (c < 0) 
                n = n->centro;
            else 
                n = n->dir;
        }
    }
    return achou;
}


void *buscarPredecessor(No23 *n)
{
    void *r = NULL;
    No23 *atual = n;
    while (atual != NULL && !ehFolha(atual)) {
        if (atual->quantidadeInfos == 2) 
            atual = atual->dir;
        else atual = atual->centro;
    }
    if (atual != NULL) {
        if (atual->quantidadeInfos == 2) 
            r = atual->info2;
        else r = atual->info1;
    }
    return r;
}

void removerChaveNo(No23 *n, int indice)
{
    if (indice == 0) {
        if (n->quantidadeInfos == 2) { 
            n->info1 = n->info2; 
            n->info2 = NULL; 
            n->quantidadeInfos = 1; 
        }
        else { 
            n->info1 = NULL; 
            n->quantidadeInfos = 0; 
        }
    } else { 
        n->info2 = NULL; 
        n->quantidadeInfos = 1; 
    }
}


void repararFilho23(No23 *pai, int indice)
{
    No23 *f[3], *v, *irm;
    void *k[2];
    int n, i;
    n = pai->quantidadeInfos;

    f[0] = pai->esq; 
    f[1] = pai->centro; 
    f[2] = pai->dir;

    k[0] = pai->info1; 
    k[1] = pai->info2;
    v = f[indice];

    if (indice > 0 && f[indice - 1] != NULL && f[indice - 1]->quantidadeInfos == 2) {
        irm = f[indice - 1];
        v->info1 = k[indice - 1];
        v->quantidadeInfos = 1;
        k[indice - 1] = irm->info2;

        if (!ehFolha(irm)) {
            v->centro = v->esq;
            v->esq = irm->dir;
            irm->dir = NULL;
        }
        irm->info2 = NULL;
        irm->quantidadeInfos = 1;
    }
    
    else if (indice < n && f[indice + 1] != NULL && f[indice + 1]->quantidadeInfos == 2) {
        irm = f[indice + 1];
        v->info1 = k[indice];
        v->quantidadeInfos = 1;
        k[indice] = irm->info1;

        if (!ehFolha(irm)) {
            v->centro = irm->esq;
            irm->esq = irm->centro;
            irm->centro = irm->dir;
            irm->dir = NULL;
        }
        irm->info1 = irm->info2;
        irm->info2 = NULL;
        irm->quantidadeInfos = 1;
    }

    else if (indice > 0) {
        irm = f[indice - 1];
        irm->info2 = k[indice - 1];
        irm->quantidadeInfos = 2;

        if (!ehFolha(irm)) 
            irm->dir = v->esq;
        free(v);

        for (i = indice - 1; i < n - 1; i++) 
            k[i] = k[i + 1];

        for (i = indice; i < n; i++) 
            f[i] = f[i + 1];
        n--;
    }
   
    else {
        irm = f[indice + 1];
        v->info1 = k[indice];
        v->info2 = irm->info1;
        v->quantidadeInfos = 2;

        if (!ehFolha(irm)) {
            v->centro = irm->esq;
            v->dir = irm->centro;
        }
        free(irm);

        for (i = indice; i < n - 1; i++) 
            k[i] = k[i + 1];

        for (i = indice + 1; i < n; i++) 
            f[i] = f[i + 1];
        n--;
    }

    if (n == 2) {
        pai->info1 = k[0]; 
        pai->info2 = k[1]; 
        pai->quantidadeInfos = 2;
        pai->esq = f[0]; 
        pai->centro = f[1]; 
        pai->dir = f[2];

    } else if (n == 1) {
        pai->info1 = k[0]; 
        pai->info2 = NULL; 
        pai->quantidadeInfos = 1;
        pai->esq = f[0]; 
        pai->centro = f[1]; 
        pai->dir = NULL;

    } else {
        pai->info1 = NULL; 
        pai->info2 = NULL; 
        pai->quantidadeInfos = 0;
        pai->esq = f[0]; 
        pai->centro = NULL; 
        pai->dir = NULL;
    }
}

int removerRecursivo(No23 **raiz, void *chave, int tipo)
{
    int r = 0, indice = 0, encontrado = 0;
    No23 *n = *raiz;
    if (n != NULL) {
        int c1 = compararInfo(chave, n->info1, tipo);
        int c2 = 1;

        if (n->quantidadeInfos == 2) 
            c2 = compararInfo(chave, n->info2, tipo);

        if (c1 == 0) { 
            encontrado = 1; 
            indice = 0; 
        }
        else if (n->quantidadeInfos == 2 && c2 == 0) { 
            encontrado = 1; 
            indice = 1; 
        }
        if (encontrado) {
            r = 1;
            if (!ehFolha(n)) {
                No23 **sub;
                void *pred;
                if (indice == 0) 
                    sub = &n->esq;

                else sub = &n->centro;

                pred = buscarPredecessor(*sub);

                if (indice == 0) 
                    n->info1 = pred;

                else n->info2 = pred;

                removerRecursivo(sub, pred, tipo);

                if (*sub != NULL && (*sub)->quantidadeInfos == 0) 
                    repararFilho23(n, indice);

            } else removerChaveNo(n, indice);
        } else {
            No23 **sub;
            if (c1 < 0) { 
                indice = 0; 
                sub = &n->esq; 
            }
            else if (n->quantidadeInfos == 1 || c2 < 0) { 
                indice = 1; 
                sub = &n->centro; 
            }
            else { 
                indice = 2; 
                sub = &n->dir; 
            }
            r = removerRecursivo(sub, chave, tipo);

            if (r == 1 && *sub != NULL && (*sub)->quantidadeInfos == 0) 
                repararFilho23(n, indice);
        }
    }
    return r;
}

int remover23(No23 **raiz, void *chave, int tipo)
{
    int r = removerRecursivo(raiz, chave, tipo);
    if (r == 1 && *raiz != NULL && (*raiz)->quantidadeInfos == 0) {
        No23 *antiga = *raiz;
        *raiz = antiga->esq;
        free(antiga);
    }
    return r;
}

void liberarItensCompra(ItemCompra *itens)
{
    while (itens != NULL) { 
        ItemCompra *p = itens->prox; 
        free(itens); 
        itens = p; }
}

void liberarArvore23(No23 *r, int tipo)
{
    if (r != NULL) {
        liberarArvore23(r->esq, tipo); 
        liberarArvore23(r->centro, tipo); 
        liberarArvore23(r->dir, tipo);

        if (tipo == INFO_PRODUTO) { 
            free(r->info1); 
            free(r->info2); 
        }
        else if (tipo == INFO_CLIENTE) {
            if (r->info1 != NULL) { 
                Cliente *c = r->info1; 
                liberarArvore23(c->compras, INFO_COMPRA); 
                free(c); 
            }
            if (r->info2 != NULL) { 
                Cliente *c = r->info2; 
                liberarArvore23(c->compras, INFO_COMPRA); 
                free(c); 
            }
        } else {
            if (r->info1 != NULL) { 
                Compra *c = r->info1; 
                liberarItensCompra(c->itens); 
                free(c); 
            }
            if (r->info2 != NULL) { 
                Compra *c = r->info2; 
                liberarItensCompra(c->itens); 
                free(c); 
            }
        }
        free(r);
    }
}

void imprimirArvore23(No23 *r, int tipo)
{
    if (r != NULL) {
        imprimirArvore23(r->esq, tipo);

        if (tipo == INFO_PRODUTO) 
            printf("%d ", ((Produto *)r->info1)->codigo);

        else if (tipo == INFO_CLIENTE) 
            printf("%s ", ((Cliente *)r->info1)->cnpj);

        else printf("%02d/%02d/%04d ", ((Compra *)r->info1)->dataCompra.dia, ((Compra *)r->info1)->dataCompra.mes, ((Compra *)r->info1)->dataCompra.ano);

        imprimirArvore23(r->centro, tipo);

        if (r->quantidadeInfos == 2) {
            if (tipo == INFO_PRODUTO) 
                printf("%d ", ((Produto *)r->info2)->codigo);

            else if (tipo == INFO_CLIENTE) 
                printf("%s ", ((Cliente *)r->info2)->cnpj);

            else printf("%02d/%02d/%04d ", ((Compra *)r->info2)->dataCompra.dia, ((Compra *)r->info2)->dataCompra.mes, ((Compra *)r->info2)->dataCompra.ano);

            imprimirArvore23(r->dir, tipo);
        }
    }
}

int adicionarItemCompra(Compra *c, int codigo, int quantidade, float preco)
{
    int r = 0;
    ItemCompra *novo = NULL;
    if (c != NULL && quantidade > 0) novo = malloc(sizeof(ItemCompra));
    if (novo != NULL) {
        novo->codigoProduto = codigo; 
        novo->quantidade = quantidade; 
        novo->valorUnitario = preco;
        novo->subtotal = quantidade * preco; 
        novo->prox = c->itens; 
        c->itens = novo;
        c->valorTotal += novo->subtotal; 
        r = 1;
    }
    return r;
}

void limparEntrada(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void lerTexto(char *destino, int tamanho)
{
    size_t n;
    if (fgets(destino, tamanho, stdin) != NULL) {
        n = strlen(destino);

        if (n > 0 && destino[n - 1] == '\n') 
            destino[n - 1] = '\0';

        else limparEntrada();

    } else destino[0] = '\0';
}

int lerInteiro(char *mensagem, int *valor)
{
    int r = 0;
    printf("%s", mensagem);    

    if (scanf("%d", valor) == 1) 
        r = 1;

    limparEntrada();
    return r;
}

Produto *cadastrarProduto(void)
{
    Produto *p = calloc(1, sizeof(Produto));
    int tipo = -1, cabelo = -1, condicao = -1, tamanho = -1, i, ok = 1;
    if (p != NULL) {
        ok = lerInteiro("Codigo: ", &p->codigo);

        printf("Nome: "); 
        lerTexto(p->nome, TAM_PRODUTO);

        if (lerInteiro("Tipo (0-Shampoo, 1-Condicionador, 2-Finalizador, 3-Mascara, 4-Oleo): ", &tipo) == 0) 
            ok = 0;

        if (lerInteiro("Tipo de cabelo (0-Liso, 1-Ondulado, 2-Crespo): ", &cabelo) == 0) 
            ok = 0;

        if (lerInteiro("Condicao (0-Oleoso, 1-Normal, 2-Seco, 3-Pos-quimica): ", &condicao) == 0) 
            ok = 0;

        if (lerInteiro("Tamanho (0-300ml, 1-500ml, 2-1L): ", &tamanho) == 0) 
            ok = 0;

        if (lerInteiro("Estoque inicial: ", &p->estoque) == 0) 
            ok = 0;

        if (lerInteiro("Quantidade de faixas de preco (1 a 10): ", &p->quantidadeFaixas) == 0) 
            ok = 0;

        if (tipo < 0 || tipo > 4 || cabelo < 0 || cabelo > 2 || condicao < 0 || condicao > 3 || tamanho < 0 || tamanho > 2 || p->codigo <= 0 || p->estoque < 0 || p->quantidadeFaixas < 1 || p->quantidadeFaixas > MAX_FAIXAS_PRECO) 
            ok = 0;

        if (ok) {
            p->tipo = tipo; 
            p->tipoCabelo = cabelo; 
            p->condicaoCabelo = condicao; 
            p->tamanho = tamanho;

            for (i = 0; i < p->quantidadeFaixas; i++) {
                printf("Faixa %d - quantidade minima, maxima e preco unitario: ", i + 1);

                if (scanf("%d%d%f", &p->faixasPreco[i].quantidadeMin, &p->faixasPreco[i].quantidadeMax, &p->faixasPreco[i].precoUnitario) != 3) 
                    ok = 0;

                limparEntrada();

                if (p->faixasPreco[i].quantidadeMin <= 0 || p->faixasPreco[i].quantidadeMax < p->faixasPreco[i].quantidadeMin || p->faixasPreco[i].precoUnitario <= 0) 
                    ok = 0;

                if (i > 0 && p->faixasPreco[i].quantidadeMin <= p->faixasPreco[i - 1].quantidadeMax) 
                    ok = 0;
            }
        }
        if (!ok) { 
            printf("Dados do produto invalidos. Cadastro cancelado.\n"); 
            free(p); 
            p = NULL; }
    }
    return p;
}

int cnpjValido(char *cnpj)
{
    int i, r = 1;
    if (strlen(cnpj) != 14) 
        r = 0;

    for (i = 0; cnpj[i] != '\0'; i++) 
    {
        if (cnpj[i] < '0' || cnpj[i] > '9') 
            r = 0;
    }
    return r;
}

Cliente *cadastrarCliente(void)
{
    Cliente *c = calloc(1, sizeof(Cliente));
    if (c != NULL) {
        printf("CNPJ (14 digitos, somente numeros): "); 
        lerTexto(c->cnpj, TAM_CNPJ);

        printf("Nome: "); 
        lerTexto(c->nome, TAM_NOME);

        printf("Endereco: "); 
        lerTexto(c->endereco, TAM_ENDERECO);

        printf("Contato: "); 
        lerTexto(c->contato, TAM_CONTATO);

        if (!cnpjValido(c->cnpj) || strlen(c->nome) == 0) { 
            printf("CNPJ ou nome invalido.\n"); 
            free(c); 
            c = NULL; 
        }
    }
    return c;
}

int dataValida(Data d)
{
    int dias = 31, r = 1;
    if (d.mes == 4 || d.mes == 6 || d.mes == 9 || d.mes == 11) 
        dias = 30;

    else if (d.mes == 2) {
        dias = 28;
        if ((d.ano % 400 == 0) || (d.ano % 4 == 0 && d.ano % 100 != 0)) 
            dias = 29;
    }
    if (d.ano < 1 || d.mes < 1 || d.mes > 12 || d.dia < 1 || d.dia > dias) 
        r = 0;
    return r;
}

Compra *cadastrarCompra(void)
{
    Compra *c = calloc(1, sizeof(Compra));
    if (c != NULL) {
        if (lerInteiro("Codigo da compra: ", &c->codigo) == 0 || c->codigo <= 0) { 
            free(c); 
            c = NULL; 
        }
        else {
            printf("Data (dia mes ano): ");
            if (scanf("%d%d%d", &c->dataCompra.dia, &c->dataCompra.mes, &c->dataCompra.ano) != 3) {
                c->dataCompra.dia = 0;
            }
            limparEntrada();
            if (!dataValida(c->dataCompra)) { 
                printf("Data invalida.\n"); 
                free(c); 
                c = NULL; }
        }
    }
    return c;
}

void listarProduto(Produto *p)
{
    int i;
    if (p != NULL) {
        printf("\nCodigo: %d | Nome: %s | Estoque: %d\n", p->codigo, p->nome, p->estoque);

        printf("Tipo: %d | Cabelo: %d | Condicao: %d | Tamanho: %d\n", p->tipo, p->tipoCabelo, p->condicaoCabelo, p->tamanho);

        for (i = 0; i < p->quantidadeFaixas; i++) 
        {
            printf("  De %d a %d unidades: R$ %.2f\n", p->faixasPreco[i].quantidadeMin, p->faixasPreco[i].quantidadeMax, p->faixasPreco[i].precoUnitario);
        }
    }
}

void listarCliente(Cliente *c)
{
    if (c != NULL) 
        printf("\nCNPJ: %s | Nome: %s | Endereco: %s | Contato: %s\n", c->cnpj, c->nome, c->endereco, c->contato);
}

void listarCompra(Compra *c)
{
    ItemCompra *it;
    if (c != NULL) {
        printf("\nCompra %d - %02d/%02d/%04d - Total: R$ %.2f\n", c->codigo, c->dataCompra.dia, c->dataCompra.mes, c->dataCompra.ano, c->valorTotal);

        it = c->itens;

        while (it != NULL) { 
            printf("  Produto %d | Quantidade %d | Unitario R$ %.2f | Subtotal R$ %.2f\n", it->codigoProduto, it->quantidade, it->valorUnitario, it->subtotal); 
            it = it->prox; }
    }
}

void listarProdutosRec(No23 *r)
{
    if (r != NULL) { 
        listarProdutosRec(r->esq); 
        listarProduto(r->info1); 
        listarProdutosRec(r->centro); 
        
        if (r->quantidadeInfos == 2) { 
            listarProduto(r->info2); 
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
        listarCliente(r->info1); 
        listarClientesRec(r->centro); 

        if (r->quantidadeInfos == 2) { 
            listarCliente(r->info2); 
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
        listarCompra(r->info1); 
        listarComprasRec(r->centro); 

        if (r->quantidadeInfos == 2) { 
            listarCompra(r->info2); 
            listarComprasRec(r->dir); 
        } 
    } 
}


void listarCompras(No23 *r) { 
    listarComprasRec(r); 
}

float obterPreco(Produto *p, int quantidade)
{
    int i;
    float preco = 0.0f;
    for (i = 0; i < p->quantidadeFaixas; i++) 
        if (quantidade >= p->faixasPreco[i].quantidadeMin && quantidade <= p->faixasPreco[i].quantidadeMax) { 
            preco = p->faixasPreco[i].precoUnitario; 
            break; 
        }
    return preco;
}

void registrarCompra(Fabrica *f)
{
    char cnpj[TAM_CNPJ];
    Cliente chave, *cliente;
    Compra *compra;
    int continuar = 1;

    printf("CNPJ do cliente: "); 
    lerTexto(cnpj, TAM_CNPJ);

    strcpy(chave.cnpj, cnpj);
    cliente = buscar23(f->clientes, &chave, INFO_CLIENTE);

    if (cliente == NULL) 
        printf("Cliente nao encontrado.\n");
    else {
        compra = cadastrarCompra();
        if (compra != NULL) {
            while (continuar) {
                int codigo, quantidade;
                Produto chaveP, *p;
                if (!lerInteiro("Codigo do produto (0 encerra): ", &codigo)) 
                    continuar = 0;

                else if (codigo == 0) 
                    continuar = 0;

                else if (!lerInteiro("Quantidade: ", &quantidade)) 
                    printf("Quantidade invalida.\n");

                else {
                    chaveP.codigo = codigo; 
                    p = buscar23(f->produtos, &chaveP, INFO_PRODUTO);

                    if (p == NULL) 
                        printf("Produto nao encontrado.\n");
                        
                    else if (quantidade <= 0 || quantidade > p->estoque) 
                        printf("Quantidade indisponivel.\n");

                    else {
                        float preco = obterPreco(p, quantidade);
                        if (preco <= 0.0f) 
                            printf("Nao existe faixa de preco para essa quantidade.\n");

                        else if (adicionarItemCompra(compra, codigo, quantidade, preco)) { 
                            p->estoque -= quantidade; 
                            printf("Item adicionado.\n"); 
                        }
                        else printf("Falha ao alocar item.\n");
                    }
                }
            }
            if (compra->itens == NULL) { 
                free(compra); 
                printf("Compra cancelada: nenhum item.\n"); 
            }
            else if (inserir23(&cliente->compras, compra, INFO_COMPRA) != 1) {
                ItemCompra *it = compra->itens;

                while (it != NULL) { 
                    Produto cp, *p; 
                    cp.codigo = it->codigoProduto; 
                    p = buscar23(f->produtos, &cp, INFO_PRODUTO); 
                    
                    if (p != NULL) 
                        p->estoque += it->quantidade; 
                        it = it->prox; 
                    }
                liberarItensCompra(compra->itens); 
                free(compra); 
                printf("Compra nao inserida: codigo/data repetidos ou memoria insuficiente.\n");

            } else printf("Compra registrada com sucesso.\n");
        }
    }
}

void consultarCompra(Cliente *cliente)
{
    Compra chave, *c;
    printf("Data da compra (dia mes ano): ");
    if (scanf("%d%d%d", &chave.dataCompra.dia, &chave.dataCompra.mes, &chave.dataCompra.ano) != 3) 
        chave.dataCompra.dia = 0;

    printf("Codigo da compra: "); 
    if (scanf("%d", &chave.codigo) != 1) 
        chave.codigo = -1;

    limparEntrada();
    c = buscar23(cliente->compras, &chave, INFO_COMPRA);

    if (c == NULL) 
        printf("Compra nao encontrada.\n"); 
    else listarCompra(c);
}

void restaurarEstoqueCompra(Compra *c, No23 *produtos)
{
    ItemCompra *it = c->itens;
    while (it != NULL) { 
        Produto chave, *p; 
        chave.codigo = it->codigoProduto; 
        p = buscar23(produtos, &chave, INFO_PRODUTO); 
        if (p != NULL) 
            p->estoque += it->quantidade; 
        it = it->prox; 
    }
}

void removerCompra(Cliente *cliente, Fabrica *fabrica)
{
    Compra chave, *c;
    printf("Data da compra (dia mes ano): ");
    if (scanf("%d%d%d", &chave.dataCompra.dia, &chave.dataCompra.mes, &chave.dataCompra.ano) != 3) 
        chave.dataCompra.dia = 0;

    printf("Codigo da compra: "); 
    if (scanf("%d", &chave.codigo) != 1) 
        chave.codigo = -1;

    limparEntrada();
    c = buscar23(cliente->compras, &chave, INFO_COMPRA);

    if (c == NULL) 
        printf("Compra nao encontrada.\n");

    else if (remover23(&cliente->compras, &chave, INFO_COMPRA)) {
        restaurarEstoqueCompra(c, fabrica->produtos);
        liberarItensCompra(c->itens); 
        free(c);
        printf("Compra removida e estoque restaurado.\n");
    }
}
