#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

#include "arvores.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"portuguese"); //para imprimir corretamente na tela os caracteres acentuados

    clock_t start, end; //para contar o tempo decorrido

    start = clock(); // inicia a contagem do tempo

    FILE * textoA;
    FILE * textoB;
    FILE * stopwords;

    char *palavra, linha[1000]; // linhas a serem lidas do arquivo
    char separador[] = {" 0123456789-.,&*%\?!;/'@\"$#=~><()][}{:\n\t_"}; // separadores para stringtok

    // arvores
    pNodoA *AVL_A;
    pNodoA *AVL_B;
    pNodoA *AVL_SW;

    // flags para uso da arvore
    int ok;
    int nivel = 1;

    // variaveis de controle
    int distintasA, distintasB, distintasAB;
    distintasA = distintasB = distintasAB = 0;

    if (argc != 4)
    {
        printf ("Numero incorreto de parametros.\n Para chamar o programa digite: exemplo <texto1> <texto2> <texto_stopwords>\n");
        return 1;
    }
    else
    {
        textoA = fopen (argv[1], "r"); // abre textoA para leitura
        textoB = fopen (argv[2], "r"); // abre textoB para leitura
        stopwords = fopen (argv[3], "r"); // abre texto stopwords para leitura

        if (!textoA || !textoB || !stopwords) // se nao conseguiu abrir algum dos textos
        {
            printf ("Erro ao abrir pelo menos um dos arquivos");
            return 1;
        }
        else // textoA ok
        {

            // *******************************************
            // inicializa árvore de stopwords
            AVL_SW = cria_AVL();

            //percorre todo o arquivo lendo linha por linha
            while (fgets(linha, 1000, stopwords))
            {
                palavra = strtok (linha, separador); //considera qquer caractere nao alfabetico como separador
                while (palavra != NULL)
                {
                    // preenche AVL_A com palavras distintas do textoA
                    AVL_SW = InsereAVL (AVL_SW, strlwr(palavra), &ok);

                    // atualiza palavra para próxima iteração
                    palavra = strtok (NULL, separador);
                }
            }

            // *******************************************
            // inicializa árvore AVL_A (palavras do textoA)
            AVL_A = cria_AVL();

            //percorre todo o arquivo lendo linha por linha
            while (fgets(linha, 1000, textoA))
            {
                palavra = strtok (linha, separador); //considera qquer caractere nao alfabetico como separador
                while (palavra != NULL)
                {
                    palavra = strlwr(palavra);
                    // se palavra nao é stopword e ainda nao foi inserida em A
                    if (!consulta(AVL_SW, palavra) && !consulta(AVL_A, palavra)){
                        AVL_A = InsereAVL (AVL_A, palavra, &ok); // insere na arvore em lowercase
                        distintasA++;
                    }

                    // atualiza palavra para próxima iteração
                    palavra = strtok (NULL, separador);
                }
            }

            // *********************************************
            // inicializa árvore AVL_B (palavras do textoB)
            AVL_B = cria_AVL();

            //percorre todo o arquivo lendo linha por linha
            while (fgets(linha, 1000, textoB))
            {
                palavra = strtok (linha, separador); //considera qquer caractere nao alfabetico como separador
                while (palavra != NULL)
                {
                    palavra = strlwr(palavra);
                    int consultaB = consulta(AVL_B, palavra);
                    // se palavra ja esta em A e ainda nao foi inserida em B, entao é intersecção
                    if (consulta(AVL_A, palavra) && !consultaB)
                        distintasAB++;

                    // se palavra nao é stopword e ainda nao foi inserida em B
                    if (!consulta(AVL_SW, palavra) && !consultaB){
                        AVL_B = InsereAVL (AVL_B, palavra, &ok); // insere na arvore em lowercase
                        distintasB++;
                    }

                    // atualiza palavra para próxima iteração
                    palavra = strtok (NULL, separador);
                }
            }

            end = clock(); // finaliza contagem do tempo
            float miliseconds = (float)(end - start) / CLOCKS_PER_SEC * 1000; //calcula o tempo decorrido

            // realiza impressão das saídas
            printf("%s = %d palavras distintas\n", argv[1], distintasA);
            printf("%s = %d palavras distintas\n", argv[2], distintasB);
            printf("Interseção = %d\n", distintasAB);
            printf("União = %d\n", distintasA + distintasB - distintasAB);
            printf("Jaccard = %.2f\n", (float) distintasAB / (float) (distintasA + distintasB - distintasAB));
            printf("Tempo: %.5f ms\n\n", miliseconds);
        }

        // fecha arquivos
        fclose(textoA);
        fclose(textoB);
        fclose(stopwords);

        // desaloca arvores
        destroi(&AVL_A);
        destroi(&AVL_B);
        destroi(&AVL_SW);
    }

    return 0;
}
