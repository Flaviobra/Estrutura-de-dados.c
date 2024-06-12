#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cidade.h"

int comp(const void *a, const void *b) {
    Cidade *cidadeA = (Cidade *)a;
    Cidade *cidadeB = (Cidade *)b;
    return cidadeA->Posicao - cidadeB->Posicao;
}

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    Estrada *estrada = (Estrada *)malloc(sizeof(Estrada));
    if (estrada == NULL) {
        printf("Erro ao alocar memória para a estrada.\n");
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "%d", &estrada->T) != 1) {
        printf("Erro ao ler o tamanho da estrada.\n");
        free(estrada);
        fclose(file);
        return NULL;
    }
    if (fscanf(file, "%d", &estrada->N) != 1) {
        printf("Erro ao ler o número de cidades.\n");
        free(estrada);
        fclose(file);
        return NULL;
    }

    estrada->C = (Cidade *)malloc(estrada->N * sizeof(Cidade));
    if (estrada->C == NULL) {
        printf("Erro ao alocar memória para as cidades.\n");
        free(estrada);
        fclose(file);
        return NULL;
    }

    int i;
    for (i = 0; i < estrada->N; i++) {
        if (fscanf(file, "%d", &estrada->C[i].Posicao) != 1) {
            printf("Erro ao ler a posição da cidade.\n");
            free(estrada->C);
            free(estrada);
            fclose(file);
            return NULL;
        }
        fgetc(file); // Consome o espaço ou o caractere de nova linha
        if (fgets(estrada->C[i].Nome, sizeof(estrada->C[i].Nome), file) == NULL) {
            printf("Erro ao ler o nome da cidade.\n");
            free(estrada->C);
            free(estrada);
            fclose(file);
            return NULL;
        }
        // Remove o caractere de nova linha no final do nome da cidade, se houver
        size_t len = strlen(estrada->C[i].Nome);
        if (len > 0 && estrada->C[i].Nome[len - 1] == '\n') {
            estrada->C[i].Nome[len - 1] = '\0';
        }
    }

    fclose(file);

    qsort(estrada->C, estrada->N, sizeof(Cidade), comp);

    return estrada;
}

double calcularvizinho(int posicao1, int posicao2, int N) {
    return fabs(posicao1 - posicao2) / 2.0;
}

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (estrada == NULL) {
        return -1; // Erro ao obter a estrada
    }

    int final = estrada->T;
    double vizinhacaMedia = calcularvizinho(estrada->C[0].Posicao, estrada->C[1].Posicao, estrada->N);
    double vizinhacaPrimeiro = estrada->C[0].Posicao + vizinhacaMedia;
    double menorVizi = vizinhacaPrimeiro;

    if (estrada->N > 2) {
        double vizinhancaAtual;
        int i;
        for (i = 1; i < estrada->N - 1; i++) {
            vizinhancaAtual = calcularvizinho(estrada->C[i].Posicao, estrada->C[i + 1].Posicao, estrada->T);
            double temp = vizinhancaAtual;
            vizinhancaAtual += vizinhacaMedia;
            if (vizinhancaAtual < menorVizi) {
                menorVizi = vizinhancaAtual;
            }
            vizinhacaMedia = temp;
        }
        vizinhancaAtual = (final - estrada->C[i].Posicao) + vizinhacaMedia;
        if (vizinhancaAtual < menorVizi) {
            menorVizi = vizinhancaAtual;
        }
    } else {
        double vizinhancaSegundo = (final - estrada->C[1].Posicao) + vizinhacaMedia;
        menorVizi = (vizinhacaPrimeiro < vizinhancaSegundo) ? vizinhacaPrimeiro : vizinhancaSegundo;
    }

    free(estrada->C);
    free(estrada);

    return menorVizi;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (estrada == NULL) {
        return NULL;
    }

    int final = estrada->T;
    int iMenorV = 0;
    double vizinhacaMedia = calcularvizinho(estrada->C[0].Posicao, estrada->C[1].Posicao, estrada->N);
    double vizinhacaPrimeiro = estrada->C[0].Posicao + vizinhacaMedia;
    double menorVizi = vizinhacaPrimeiro;

    if (estrada->N > 2) {
        double vizinhancaAtual;
        int i;
        for (i = 1; i < estrada->N - 1; i++) {
            vizinhancaAtual = calcularvizinho(estrada->C[i].Posicao, estrada->C[i + 1].Posicao, estrada->T);
            double temp = vizinhancaAtual;
            vizinhancaAtual += vizinhacaMedia;
            if (vizinhancaAtual < menorVizi) {
                menorVizi = vizinhancaAtual;
                iMenorV = i;
            }
            vizinhacaMedia = temp;
        }
        vizinhancaAtual = (final - estrada->C[i].Posicao) + vizinhacaMedia;
        if (vizinhancaAtual < menorVizi) {
            menorVizi = vizinhancaAtual;
            iMenorV = i;
        }
    } else {
        double vizinhancaSegundo = (final - estrada->C[1].Posicao) + vizinhacaMedia;
        if (vizinhacaPrimeiro > vizinhancaSegundo) {
            menorVizi = vizinhancaSegundo;
            iMenorV = 1;
        } else {
            menorVizi = vizinhacaPrimeiro;
            iMenorV = 0;
        }
    }

    char *cidadeMenorViz = strdup(estrada->C[iMenorV].Nome);

    free(estrada->C);
    free(estrada);

    return cidadeMenorViz;
}
