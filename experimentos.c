#include "estruturas.h"
#include <time.h>

void gerarCNPJ(char *cnpj, int numero){
    sprintf(cnpj, "%014d", numero);
}

void embaralhar(int valores[], int quantidade)
{
    int i;
    for (i = quantidade - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int aux = valores[i];
        valores[i] = valores[j];
        valores[j] = aux;
    }
}

void executarOrdem(int ordem[], int quantidade, int modo)
{
    No23 *raiz = NULL;
    int i, passosTotal = 0, encontrados = 0;
    for (i = 0; i < quantidade; i++) {
        Cliente *c = (Cliente *)calloc(1, sizeof(Cliente));

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
        printf("Insercao com elemento central primeiro e demais aleatorios\n");
    else 
        printf("Insercao em ordem totalmente aleatoria\n");

    for (i = 0; i < 10; i++) {
        Cliente chave;

        int passos = 0;
        int numero = (i * 10) + 1;

        gerarCNPJ(chave.cnpj, numero);
        
        if (buscar23ComPassos(raiz, &chave, INFO_CLIENTE, &passos) == 1) 
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
    if (encontrados > 0) 
        printf("Media de nos visitados nas buscas existentes: %.2f\n", (float)passosTotal / 10.0f);
    liberarArvore23(raiz, INFO_CLIENTE);
}

void menu_experimento(void)
{
    int crescente[100], decrescente[100], central[100], aleatoria[100];
    int i, pos = 0;
    srand((unsigned int)time(NULL));

    for (i = 0; i < 100; i++) {
        crescente[i] = i + 1;
        decrescente[i] = 100 - i;
        aleatoria[i] = i + 1;
    }

    central[pos++] = 50;
    for (i = 1; i <= 100; i++) if (i != 50) central[pos++] = i;
    embaralhar(&central[1], 99);
    embaralhar(aleatoria, 100);

    printf("\n===== EXPERIMENTOS DO ITEM (D) =====\n");
    executarOrdem(crescente, 100, 1);
    executarOrdem(decrescente, 100, 2);
    executarOrdem(central, 100, 3);
    executarOrdem(aleatoria, 100, 4);
}
