#include "estruturas.h"
#include <time.h>

void gerarCNPJ(char *cnpj, int numero)
{
    sprintf(cnpj, "%014d", numero);
}

void embaralhar(int valores[], int quantidade)
{
    int i;
    for (i = quantidade - 1; i > 0; i--) {
        int j = rand() % (i + 1), aux = valores[i];

        valores[i] = valores[j]; 
        valores[j] = aux;
    }
}

void executarOrdem(int ordem[], int quantidade, int modo, int buscas[])
{
    No23 *raiz = NULL;
    int i, passosTotal = 0, encontrados = 0;

    for (i = 0; i < quantidade; i++) {
        Cliente *c = calloc(1, sizeof(Cliente));
        if (c != NULL) {
            gerarCNPJ(c->cnpj, ordem[i]);

            if (inserir23(&raiz, c, INFO_CLIENTE) != 1) 
                free(c);
        }
    }
    printf("\n---------------------------------------------\n");
    if (modo == 1) 
        printf("Insercao em ordem crescente\n");

    else if (modo == 2) 
        printf("Insercao em ordem decrescente\n");

    else if (modo == 3) 
        printf("Elemento central primeiro e demais aleatorios\n");

    else printf("Insercao em ordem totalmente aleatoria\n");

    for (i = 0; i < 10; i++) {
        Cliente chave;
        int passos = 0;
        gerarCNPJ(chave.cnpj, buscas[i]);

        if (buscar23ComPassos(raiz, &chave, INFO_CLIENTE, &passos)) 
            encontrados++;

        passosTotal += passos;
        printf("Busca do CNPJ %s: %d nos visitados\n", chave.cnpj, passos);
    }
    {
        Cliente ausente;
        int passos = 0;
        gerarCNPJ(ausente.cnpj, 99999999);
        buscar23ComPassos(raiz, &ausente, INFO_CLIENTE, &passos);
        printf("Busca do CNPJ nao cadastrado %s: %d nos visitados\n", ausente.cnpj, passos);
    }
    printf("Total de CNPJs encontrados nas 10 buscas: %d\n", encontrados);

    if (encontrados == 10) 
        printf("Media de nos visitados nas buscas existentes: %.2f\n", (float)passosTotal / 10.0f);

    else printf("Media nao calculada: nem todos os CNPJs foram inseridos/encontrados.\n");

    liberarArvore23(raiz, INFO_CLIENTE);
}

void menu_experimento(void)
{
    int crescente[100], decrescente[100], central[100], aleatoria[100], buscas[10];
    int i, pos = 0;
    srand((unsigned int)time(NULL));

    for (i = 0; i < 100; i++) { 
        crescente[i] = i + 1; 
        decrescente[i] = 100 - i; 
        aleatoria[i] = i + 1; 
    }

    central[pos++] = 50;

    for (i = 1; i <= 100; i++) 
        if (i != 50) 
            central[pos++] = i;

    embaralhar(&central[1], 99);
    embaralhar(aleatoria, 100);

    for (i = 0; i < 10; i++) 
        buscas[i] = i + 1;

    embaralhar(buscas, 10);
    
    printf("\n===== EXPERIMENTOS DO ITEM (D) =====\n");
    printf("Os mesmos dez CNPJs aleatorios serao pesquisados nas quatro arvores.\n");
    executarOrdem(crescente, 100, 1, buscas);
    executarOrdem(decrescente, 100, 2, buscas);
    executarOrdem(central, 100, 3, buscas);
    executarOrdem(aleatoria, 100, 4, buscas);
}
