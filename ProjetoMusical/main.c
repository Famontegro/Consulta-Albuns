
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 200
#define CAMPO 2000
#define LINHA 5000

typedef struct{
    char nome[300], banda[300], genero[200], link[1000];
    int musicas, duracao, ano;
    long long views;
}Album;

Album albuns[MAX], copia[MAX];
int qtd = 0, ordem;

int lerCSV(char *linha, char campos[][CAMPO]){
    int c = 0, p = 0, aspas = 0;

    for(int i = 0; linha[i]; i++){
        char ch = linha[i];

        if(ch == '"'){
            if(aspas && linha[i + 1] == '"'){
                campos[c][p++] = '"';
                i++;
            }else 
                aspas = !aspas;
        }else if(ch == ';' && !aspas){
            campos[c++][p] = '\0';
            p = 0;

            if(c >= 9) 
                return -1;

        }else if(ch != '\r' && ch != '\n'){
            if(p < CAMPO - 1)
                campos[c][p++] = ch;
        }
    }

    campos[c][p] = '\0';
    return c + 1;
}

long long numero(char *s){
    char *fim;
    long long n = strtoll(s, &fim, 10);

    if(fim == s || *fim != '\0' || n < 0)
        return -1;
    else
        return n;
}

int comparar(const char *a, const char *b){
    while(*a && *b){
        int x = tolower((unsigned char)*a);
        int y = tolower((unsigned char)*b);

        if(x != y) 
            return x - y;

        a++;
        b++;
    }

    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

int buscarTexto(char *texto, char *busca){
    int n = strlen(busca);

    for(; *texto; texto++){
        int i;

        for(i=0; i < n; i++){
            if(!texto[i] || tolower((unsigned char)texto[i]) != tolower((unsigned char)busca[i]))
                break;
        }

        if(i == n) 
            return 1;
    }

    return n == 0;
}

void carregar(){
    FILE *f = fopen("Albums.txt", "r");

    if(!f){
        printf("Erro ao abrir arquivo TXT!\n");
        return;
    }

    char linha[LINHA], c[9][CAMPO];

    fgets(linha, LINHA, f);

    while(fgets(linha, LINHA, f) && qtd < MAX){
        if(lerCSV(linha, c) != 9) 
            continue;

        long long m = numero(c[3]);
        long long d = numero(c[4]);
        long long a = numero(c[7]);

        if(m < 0 || d < 0 || a < 0) 
            continue;

        strcpy(albuns[qtd].nome, c[0]);
        strcpy(albuns[qtd].banda, c[1]);
        strcpy(albuns[qtd].genero, c[2]);
        strcpy(albuns[qtd].link, c[8]);

        albuns[qtd].musicas = (int)m;
        albuns[qtd].duracao = (int)d;
        albuns[qtd].views = numero(c[5]);
        albuns[qtd].ano = (int)a;

        qtd++;
    }

    fclose(f);
    printf("\n%d albuns carregados!\n", qtd);
}

int ordenar(const void *a, const void *b){
    const Album x = *(Album *)a;
    const Album y = *(Album *)b;

    if (ordem == 0)
        return comparar(x.nome, y.nome);

    if(x.views < 0){
        if(y.views < 0)
            return comparar(x.nome, y.nome);
        else
            return 1;
    }

    if(y.views < 0)
        return -1;

    if(x.views == y.views)
        return comparar(x.nome, y.nome);

    if(ordem == 1){
        if(x.views < y.views)
            return 1;
        else
            return -1;
    }

    if(x.views > y.views)
        return 1;
    else
        return -1;
}

void mostrar(Album a){
    printf("\n%s | %s | %s\n", a.nome, a.banda, a.genero);

    printf("Musicas: %d | Duracao: %d min | Ano: %d\n",
           a.musicas, a.duracao, a.ano);

    if (a.views < 0) 
        printf("Views: N/D\n");
    else 
        printf("Views: %lld\n", a.views);

    printf("Link: %s\n", a.link);
}

void ranking(int tipo){
    int i;
    memcpy(copia, albuns, qtd * sizeof(Album));

    ordem = tipo;
    qsort(copia, qtd, sizeof(Album), ordenar);

    for(i=0; i < qtd; i++){
        printf("%d. %s | %s | ", i + 1, copia[i].nome, copia[i].banda);

        if(tipo == 0)
            printf("%s\n", copia[i].genero);
        else{
            if(copia[i].views < 0)
                printf("N/D\n");
            else
                printf("Visualizacoes\n");
        }
    }

    if(tipo != 0){
        for(i=0; i < qtd; i++)
            if(copia[i].views >= 0)
                printf("   %lld views\n", copia[i].views);
    }
}

void generos(){
    int i, j;
    char lista[MAX][200];
    int cont[MAX] = {0}, total = 0;

    for(i=0; i < qtd; i++){
        for (j=0; j < total; j++)
            if(!comparar(albuns[i].genero, lista[j])) 
                break;

        if(j == total){
            strcpy(lista[total], albuns[i].genero);
            total++;
        }

        cont[j]++;
    }

    for(i = 0; i < total; i++)
        printf("%s: %d albuns\n", lista[i], cont[i]);
}

void pesquisar(int tipo){
    char busca[300];
    int encontrados = 0, i;

    printf("Digite sua busca: ");
    scanf(" %299[^\n]", busca);

    for(i=0; i < qtd; i++){
        char *campo;

        if(tipo == 1)
            campo = albuns[i].banda;
        else if(tipo == 2)
            campo = albuns[i].genero;
        else
            campo = albuns[i].nome;

        if(buscarTexto(campo, busca)){
            mostrar(albuns[i]);
            encontrados++;
        }
    }

    printf("\nEncontrados: %d\n", encontrados);
}

void pesquisarAno() {
    int ano, encontrados = 0, i;

    printf("Digite o ano: ");
    if(scanf("%d", &ano) != 1) 
        return;

    for(i=0; i < qtd; i++)
        if(albuns[i].ano == ano){
            mostrar(albuns[i]);
            encontrados++;
        }

    printf("\nEncontrados: %d\n", encontrados);
}

void estatisticas(){
    long long views = 0, musicas = 0, duracao = 0;
    int conhecidos = 0, i;

    for(i=0; i < qtd; i++){
        musicas += albuns[i].musicas;
        duracao += albuns[i].duracao;

        if(albuns[i].views >= 0){
            views += albuns[i].views;
            conhecidos++;
        }
    }

    printf("\nAlbuns: %d\n", qtd);
    printf("Musicas: %lld\n", musicas);
    printf("Views: %lld\n", views);
    printf("Albuns sem views: %d\n", qtd - conhecidos);

    if(conhecidos)
        printf("Media de views: %.2f\n", (double)views / conhecidos);

    printf("Duracao total: %lld minutos\n", duracao);

    if(qtd)
        printf("Duracao media: %.2f minutos\n", (double)duracao / qtd);
}

int main(){
    int opcao;

    carregar();

    if(!qtd) 
        return 1;

    do{
        printf("\nCONSULTA MUSICAL\n");
        printf("(1): Ordem alfabetica\n");
        printf("(2): Albuns por genero\n");
        printf("(3): Mais visualizados\n");
        printf("(4): Menos visualizados\n");
        printf("(5): Pesquisar banda\n");
        printf("(6): Pesquisar genero\n");
        printf("(7): Pesquisar ano\n");
        printf("(8): Pesquisar album\n");
        printf("(9): Estatisticas\n");
        printf("(0): Sair\n");
        printf("Escolha uma opcao: ");

        if(scanf("%d", &opcao) != 1) 
            break;

        switch(opcao){
            case 1: 
                ranking(0); 
                break;
            case 2: 
                generos(); 
                break;
            case 3: 
                ranking(1); 
                break;
            case 4: 
                ranking(2); 
                break;
            case 5: 
                pesquisar(1); 
                break;
            case 6: 
                pesquisar(2); 
                break;
            case 7: 
                pesquisarAno(); 
                break;
            case 8: 
                pesquisar(0); 
                break;
            case 9: 
                estatisticas(); 
                break;
            case 0: 
                printf("\nObrigado por utilizar nosso programa!\n"); 
                break;
            default: 
                printf("Opcao invalida!\n");
        }

    }while(opcao != 0);

    return 0;
}
