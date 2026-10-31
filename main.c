#include "estruturas.h"

void limparBufferMain(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main(void)
{
    Fabrica fabrica;
    int opcao = -1;
    fabrica.produtos = NULL;
    fabrica.clientes = NULL;

    while (opcao != 0) {
        printf("\n========== SISTEMA DA FABRICA ==========\n");
        printf("1 - Cadastrar produto\n");
        printf("2 - Buscar produto\n");
        printf("3 - Remover produto\n");
        printf("4 - Listar produtos\n");
        printf("5 - Cadastrar cliente/revendedor\n");
        printf("6 - Buscar cliente/revendedor\n");
        printf("7 - Remover cliente/revendedor\n");
        printf("8 - Listar clientes/revendedores\n");
        printf("9 - Registrar compra de cliente\n");
        printf("10 - Consultar compra de cliente\n");
        printf("11 - Remover compra de cliente\n");
        printf("12 - Listar compras de um cliente\n");
        printf("13 - Executar experimentos com CNPJs\n");
        printf("0 - Sair\n");

        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            limparBufferMain();
            opcao = -1;
        } else limparBufferMain();

        if (opcao == 1) {
            Produto *p = cadastrarProduto();
            if (p != NULL) {
                if (inserir23(&fabrica.produtos, p, INFO_PRODUTO) == 1)
                    printf("Produto cadastrado.\n");
                else { 
                    printf("Produto repetido ou erro de memoria.\n"); 
                    free(p); }
            }
        } else if (opcao == 2) {
            Produto chave, *p;

            printf("Codigo: "); 
            scanf("%d", &chave.codigo); 
            limparBufferMain();

            p = (Produto *)buscar23(fabrica.produtos, &chave, INFO_PRODUTO);
            if (p == NULL) 
                printf("Produto nao encontrado.\n"); 

            else listarProduto(p);

        } else if (opcao == 3) {
            Produto chave, *p;
            printf("Codigo: "); 
            scanf("%d", &chave.codigo); 
            limparBufferMain();

            p = (Produto *)buscar23(fabrica.produtos, &chave, INFO_PRODUTO);
            if (p == NULL) 
                printf("Produto nao encontrado.\n"); 

            else if (remover23(&fabrica.produtos, &chave, INFO_PRODUTO) == 1) {
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
                    printf("CNPJ repetido ou erro de memoria.\n"); 
                    free(c); }
            }
        } else if (opcao == 6) {
            Cliente chave, *c;
            printf("CNPJ: "); 
            fgets(chave.cnpj, TAM_CNPJ, stdin);

            chave.cnpj[strcspn(chave.cnpj, "\n")] = '\0';

            c = (Cliente *)buscar23(fabrica.clientes, &chave, INFO_CLIENTE);

            if (c == NULL) 
                printf("Cliente nao encontrado.\n"); 
            else listarCliente(c);

        } else if (opcao == 7) {
            Cliente chave, *c;
            printf("CNPJ: "); 
            fgets(chave.cnpj, TAM_CNPJ, stdin);

            chave.cnpj[strcspn(chave.cnpj, "\n")] = '\0';
            c = (Cliente *)buscar23(fabrica.clientes, &chave, INFO_CLIENTE);

            if (c == NULL) 
                printf("Cliente nao encontrado.\n");
            else if (c->compras != NULL) 
                printf("Nao e possivel remover: cliente possui compras.\n");
            else if (remover23(&fabrica.clientes, &chave, INFO_CLIENTE) == 1) {
                free(c); 
                printf("Cliente removido.\n");
            }
        } else if (opcao == 8) 
            listarClientes(fabrica.clientes);

        else if (opcao == 9) 
            registrarCompra(&fabrica);

        else if (opcao == 10 || opcao == 11 || opcao == 12) {
            Cliente chave, *c;
            printf("CNPJ: "); 
            fgets(chave.cnpj, TAM_CNPJ, stdin);

            chave.cnpj[strcspn(chave.cnpj, "\n")] = '\0';
            c = (Cliente *)buscar23(fabrica.clientes, &chave, INFO_CLIENTE);

            if (c == NULL) 
                printf("Cliente nao encontrado.\n");

            else if (opcao == 10) 
                consultarCompra(c);

            else if (opcao == 11) 
                removerCompra(c);

            else listarCompras(c->compras);

        } else if (opcao == 13)
            menu_experimento();

        else if (opcao != 0)
            printf("Opcao invalida.\n");
    }

    liberarArvore23(fabrica.produtos, INFO_PRODUTO);
    liberarArvore23(fabrica.clientes, INFO_CLIENTE);
    printf("Memoria liberada. Programa encerrado.\n");
    return 0;
}
