#include "arvores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pNodoA* cria_AVL ()
{
    return NULL;
}


pNodoA* rotacao_direita (pNodoA* p)
{
    pNodoA *u;
    u = p->esq;
    p->esq = u->dir;
    u->dir = p;
    p->FB = 0;
    p = u;

    return p;
}

pNodoA* rotacao_esquerda (pNodoA *p)
{
    pNodoA *z;
    z = p->dir;
    p->dir = z->esq;
    z->esq = p;
    p->FB = 0;
    p = z;

    return p;
}

pNodoA* rotacao_dupla_direita (pNodoA* p)
{
    pNodoA *u, *v;
    u = p->esq;
    v = u->dir;
    u->dir = v->esq;
    v->esq = u;
    p->esq = v->dir;
    v->dir = p;
    if (v->FB == 1) p->FB = -1;
    else p->FB = 0;
    if (v->FB == -1) u->FB = 1;
    else u->FB = 0;
    p = v;

    return p;
}

pNodoA* rotacao_dupla_esquerda (pNodoA *p)
{
    pNodoA *z, *y;
    z = p->dir;
    y = z->esq;
    z->esq = y->dir;
    y->dir = z;
    p->dir = y->esq;
    y->esq = p;
    if (y->FB == -1) p->FB = 1;
    else p->FB = 0;
    if (y->FB == 1) z->FB = -1;
    else z->FB = 0;
    p = y;

    return p;
}

pNodoA* InsereAVL (pNodoA *a, char palavraInput[], int *ok)
{
    /* Insere nodo em uma árvore AVL, onde A representa a raiz da árvore,
    x, a chave a ser inserida e h a altura da árvore */
    if (a == NULL)
    {
        a = (pNodoA*) malloc(sizeof(pNodoA));
        strcpy(a->palavra, palavraInput);
        a->esq = NULL;
        a->dir = NULL;
        a->FB = 0;
        *ok = 1;
    }
    // palavraInput tem caractere menor que a chave
    else if (strcmp(palavraInput, a->palavra) < 0)
    {
        a->esq = InsereAVL(a->esq, palavraInput, ok);
        if (*ok)
        {
            switch (a->FB)
            {
            case -1:
                a->FB = 0;
                *ok = 0;
                break;
            case 0:
                a->FB = 1;
                break;
            case 1:
                a = Caso1(a, ok);
                break;
            }
        }
    }
    else
    {
        // palavraInput tem caractere igual ou maior que a chave
        a->dir = InsereAVL(a->dir, palavraInput, ok);
        if (*ok)
        {
            switch (a->FB)
            {
            case 1:
                a->FB = 0;
                *ok = 0;
                break;
            case 0:
                a->FB = -1;
                break;
            case -1:
                a = Caso2(a,ok);
                break;
            }
        }
    }

    return a;
}

pNodoA* Caso1 (pNodoA *a, int *ok)
{
    pNodoA *z;
    z = a->esq;
    if (z->FB == 1)
        a = rotacao_direita(a);
    else
        a = rotacao_dupla_direita(a);
    a->FB = 0;
    *ok = 0;

    return a;
}

pNodoA* Caso2 (pNodoA *a, int *ok)
{
    pNodoA *z;
    z = a->dir;
    if (z->FB == -1)
        a = rotacao_esquerda(a);
    else
        a = rotacao_dupla_esquerda(a);
    a->FB = 0;
    *ok = 0;

    return a;
}


// funções de impressão (caminhamento e níveis)
void caminhamentoPreFixado(pNodoA *a)
{
    if (a!= NULL)
    {
        printf("%s\n", a->palavra);
        caminhamentoPreFixado(a->esq);
        caminhamentoPreFixado(a->dir);
    }
}

void imprimeNiveis(pNodoA *a, int nivel)
{
    int i;
    if (a != NULL)
    {
        for(i = 0; i < nivel; i++)
            printf("=");
        printf("%s\n", a->palavra);
        imprimeNiveis(a->esq, nivel+1);
        imprimeNiveis(a->dir, nivel+1);
    }
}

// contagem dos nodos
int contaNodos(pNodoA *a)
{
    if(a == NULL)
    {
        return 0;
    }
    else
    {
        return 1 + contaNodos(a->esq)
               + contaNodos(a->dir);
    }
}

int consulta(pNodoA *a, char palavra[])
{
    if (a == NULL)
        return 0;
    else if (strcmp(a->palavra, palavra) == 0)
        return 1;
    else if (strcmp(a->palavra, palavra) > 0)
        return consulta(a->esq, palavra);
    else
        return consulta(a->dir, palavra);
}

void destroi(pNodoA **a)
{
    if (*a == NULL)
        return;

    destroi(&(*a)->dir);
    destroi(&(*a)->esq);

    free(*a); //destroi de baixo para cima; pos-fixado
}

