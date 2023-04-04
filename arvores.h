struct TNodoA
{
    char palavra[30];
    int FB;
    struct TNodoA *esq;
    struct TNodoA *dir;
};
typedef struct TNodoA pNodoA;

// fun��es de opera��o/manuten��o
pNodoA* cria_AVL ();
pNodoA* rotacao_direita (pNodoA* p);
pNodoA* rotacao_esquerda (pNodoA *p);
pNodoA* rotacao_dupla_direita (pNodoA* p);
pNodoA* rotacao_dupla_esquerda (pNodoA *p);
pNodoA* InsereAVL (pNodoA *a, char palavraInput[], int *ok);
pNodoA* Caso1 (pNodoA *a, int *ok);
pNodoA* Caso2 (pNodoA *a, int *ok);
void destroi(pNodoA **a);

// fun��es de impress�o (caminhamento e niveis)
void caminhamentoPreFixado(pNodoA *a);
void imprimeNiveis(pNodoA *a, int nivel);

// extra��o de dados
int contaNodos(pNodoA *a);
int consulta(pNodoA *a, char palavra[]);
