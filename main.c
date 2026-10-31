#include "estruturas.h"

void limparBufferMain(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int arvoreComprasTemProduto(No23 *r, int codigo)
{
    int achou = 0;
    if (r != NULL) {
        achou = arvoreComprasTemProduto(r->esq, codigo);
        if (!achou) {
            Compra *c = r->info1;
            ItemCompra *it = c->itens;
            while (it != NULL && !achou) {
                if (it->codigoProduto == codigo) 
                    achou = 1;
                it = it->prox;
            }
        }
        if (!achou) 
            achou = arvoreComprasTemProduto(r->centro, codigo);

        if (r->quantidadeInfos == 2 && !achou) {
            Compra *c = r->info2;
            ItemCompra *it = c->itens;

            while (it != NULL && !achou) {
                if (it->codigoProduto == codigo) 
                    achou = 1;
                it = it->prox;
            }
            if (!achou) 
                achou = arvoreComprasTemProduto(r->dir, codigo);
        }
    }
    return achou;
}

int produtoPossuiCompra(No23 *r, int codigo)
{
    int achou = 0;
    if (r != NULL) {
        achou = produtoPossuiCompra(r->esq, codigo);
        if (!achou) 
            achou = arvoreComprasTemProduto(((Cliente *)r->info1)->compras, codigo);

        if (!achou) 
            achou = produtoPossuiCompra(r->centro, codigo);

        if (r->quantidadeInfos == 2 && !achou) {
            achou = arvoreComprasTemProduto(((Cliente *)r->info2)->compras, codigo);
            if (!achou) 
                achou = produtoPossuiCompra(r->dir, codigo);
        }
    }
    return achou;
}

int main(void)
{
    Fabrica fabrica;
    int opcao = -1;
    fabrica.produtos = NULL; 
    fabrica.clientes = NULL;

    while (opcao != 0) {
        printf("\n========== SISTEMA DA FABRICA ==========\n");
        printf("1 - Cadastrar produto\n2 - Buscar produto\n3 - Remover produto\n4 - Listar produtos\n");
        printf("5 - Cadastrar cliente/revendedor\n6 - Buscar cliente/revendedor\n7 - Remover cliente/revendedor\n8 - Listar clientes/revendedores\n");
        printf("9 - Registrar compra de cliente\n10 - Consultar compra de cliente\n11 - Remover compra de cliente\n12 - Listar compras de um cliente\n13 - Executar experimentos com CNPJs\n0 - Sair\nEscolha: ");

        if (scanf("%d", &opcao) != 1) { 
            limparBufferMain(); 
            opcao = -1; 
        }
        else limparBufferMain();

        if (opcao == 1) {
            Produto *p = cadastrarProduto();
            if (p != NULL) { 
                if (inserir23(&fabrica.produtos, p, INFO_PRODUTO) == 1) 
                    printf("Produto cadastrado.\n"); 
                else { 
                    printf("Codigo repetido ou falha de memoria.\n"); 
                    free(p); 
                } 
            }

        } else if (opcao == 2) {
            Produto chave, *p;
            printf("Codigo: "); 
            if (scanf("%d", &chave.codigo) != 1) 
                chave.codigo = -1; 
                limparBufferMain();

            p = buscar23(fabrica.produtos, &chave, INFO_PRODUTO);

            if (p == NULL) 
                printf("Produto nao encontrado.\n"); 
            else listarProduto(p);

        } else if (opcao == 3) {
            Produto chave, *p;
            printf("Codigo: "); 
            if (scanf("%d", &chave.codigo) != 1) 
                chave.codigo = -1; 
                limparBufferMain();

            p = buscar23(fabrica.produtos, &chave, INFO_PRODUTO);

            if (p == NULL) 
                printf("Produto nao encontrado.\n");

            else if (produtoPossuiCompra(fabrica.clientes, chave.codigo)) 
                printf("Produto nao pode ser removido: consta em uma compra registrada.\n");

            else if (remover23(&fabrica.produtos, &chave, INFO_PRODUTO)) { 
                free(p); 
                printf("Produto removido.\n"); 
            }

        } else if (opcao == 4) 
            listarProdutos(fabrica.produtos);

        else if (opcao == 5) {
            Cliente *c = cadastrarCliente();

            if (c != NULL) { 
                if (inserir23(&fabrica.clientes, c, INFO_CLIENTE) == 1) 
                    printf("Cliente cadastrado.\n"); 
                
                else { 
                    printf("CNPJ repetido ou falha de memoria.\n"); 
                    free(c); 
                } 
            }

        } else if (opcao == 6 || opcao == 7 || opcao == 10 || opcao == 11 || opcao == 12) {
            Cliente chave, *c;
            printf("CNPJ: "); 
            fgets(chave.cnpj, TAM_CNPJ, stdin); 
            chave.cnpj[strcspn(chave.cnpj, "\n")] = '\0';

            c = buscar23(fabrica.clientes, &chave, INFO_CLIENTE);

            if (c == NULL) 
                printf("Cliente nao encontrado.\n");

            else if (opcao == 6) 
                listarCliente(c);

            else if (opcao == 7) {
                if (c->compras != NULL) 
                    printf("Nao e possivel remover: cliente possui compras.\n");

                else if (remover23(&fabrica.clientes, &chave, INFO_CLIENTE)) { 
                    free(c); 
                    printf("Cliente removido.\n"); 
                }

            } else if (opcao == 10) 
                consultarCompra(c);

            else if (opcao == 11) 
                removerCompra(c, &fabrica);

            else listarCompras(c->compras);

        } else if (opcao == 8) 
            listarClientes(fabrica.clientes);

        else if (opcao == 9) 
            registrarCompra(&fabrica);

        else if (opcao == 13) 
            menu_experimento();

        else if (opcao != 0) 
            printf("Opcao invalida.\n");
    }
    liberarArvore23(fabrica.produtos, INFO_PRODUTO);
    liberarArvore23(fabrica.clientes, INFO_CLIENTE);
    printf("Memoria liberada. Programa encerrado.\n");
    return 0;
}
