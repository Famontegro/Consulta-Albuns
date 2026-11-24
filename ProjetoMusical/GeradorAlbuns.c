#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LINHAS 1000
#define COLUNAS 10
#define TAMANHO 1024

int main(){
    FILE *entrada;
    FILE *saida;
    int i, c, j;

    static char dados[LINHAS][COLUNAS][TAMANHO];
    char texto[TAMANHO];
    char cabecalho[TAMANHO];
    char temp[TAMANHO];

    int total = 0;
    int colunas = 1;

    entrada = fopen("Albums.txt", "r");

    if(!entrada){
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }
    
    if(fgets(cabecalho, TAMANHO, entrada) == NULL){
        printf("Arquivo vazio!\n");
        fclose(entrada);
        return 1;
    }

    for(i=0; cabecalho[i] != '\0'; i++){
        if (cabecalho[i] == ';') {
            colunas++;
        }
    }

    if(colunas > COLUNAS){
        printf("Muitas colunas!\n");
        fclose(entrada);
        return 1;
    }

    while(total < LINHAS && fgets(texto, TAMANHO, entrada) != NULL){
        texto[strcspn(texto, "\r\n")] = '\0';

        int coluna = 0;
        int posicao = 0;

        for(i=0; ; i++){
            if(texto[i] == ';' || texto[i] == '\0'){
                dados[total][coluna][posicao] = '\0';

                coluna++;
                posicao = 0;

                if(texto[i] == '\0' || coluna >= colunas)
                    break;
            }else{
                dados[total][coluna][posicao] = texto[i];
                posicao++;
            }
        }

        while(coluna < colunas){
            dados[total][coluna][0] = '\0';
            coluna++;
        }
        total++;
    }

    fclose(entrada);

    srand(time(NULL));

    for(c=0; c < colunas; c++){
        for(i = total - 1; i > 0; i--){
            j = rand() % (i + 1);

            strcpy(temp, dados[i][c]);
            strcpy(dados[i][c], dados[j][c]);
            strcpy(dados[j][c], temp);
        }
    }

    saida = fopen("Albums_embaralhados.txt", "w");

    if(!saida){
        printf("Erro ao criar arquivo!\n");
        return 1;
    }

    fputs(cabecalho, saida);

    if (cabecalho[strlen(cabecalho) - 1] != '\n')
        fprintf(saida, "\n");

    for(i=0; i < total; i++){
        for(c=0; c < colunas; c++){
            fprintf(saida, "%s", dados[i][c]);

            if (c < colunas - 1) 
                fprintf(saida, ";");
        }

        fprintf(saida, "\n");
    }

    fclose(saida);

    printf("Arquivo embaralhado com sucesso!\n");
    printf("Linhas: %d\n", total);
    printf("Colunas: %d\n", colunas);

    return 0;
}
