#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <string>
#include <time.h>
#include <ctype.h>
#include <string.h>

#define FNAME "random.bin"
#define FSIZE 11

using namespace std;

FILE *f;

struct Registro{
    int chave;
    char nome[21];
    int idade;
}Registro;

char entrada;

struct Registro reg;

int hash1(int chaveAux);
int hash2(int chaveAux);
void abreArquivo();
void abreArquivoAux();
int escreveRegistro(int jump);
int leRegistro(int jump);
void consultaChave(int chaveAux);
void resolveColisaoConsulta(int chaveAux);
void insereChave();
void resolveColisaoInsercao();
void removeChave(int chaveAux);
void resolveColisaoRemocao(int chaveAux);
void printFile();
void InitFile();



int hash1(int chaveAux){
    return (chaveAux % FSIZE);
}

int hash2(int chaveAux){
    int resp = ((int)floor(chaveAux/FSIZE)) %FSIZE;
    if (resp == 0)
        return 1;
    else
        return resp;
}

void abreArquivo(){
    f = fopen(FNAME,"rb+");
    if (!f){
      //printf("erro ao abrir arquivo para leitura");
      f = fopen(FNAME,"wb+");
    }
}

void abreArquivoAux(){
    f = fopen("auxi.bin","rb+");
    if (!f){
      //printf("erro ao abrir arquivo para leitura");
      f = fopen("auxi.bin","wb+");
    }
}

int escreveRegistro(int jump){
    if( fseek(f,jump * sizeof(Registro), SEEK_SET) == 0 )
        if ( fwrite(&Registro,sizeof(Registro),1,f) )
            return 1;
    return 0;
}


int leRegistro(int jump){
    if( fseek(f, jump * sizeof(Registro), SEEK_SET) == 0 ){
        if ( fread(&Registro,sizeof(Registro),1,f) ){
            return 1;
        }
    }
    return 0;
}

void consultaChave(int chaveAux){
    leRegistro(hash1(chaveAux));
        //printf("Nao foi possivel ler a posição da chave: %d",Registro.chave);
        if (Registro.chave == chaveAux){
            printf("chave: %d %s %d",Registro.chave, Registro.nome, Registro.idade);
        }
        else {

            resolveColisaoConsulta(chaveAux);

        }
}

void resolveColisaoConsulta(int chaveAux){
    int conth2 = 1;
    leRegistro(( ((conth2 * hash2(chaveAux))+hash1(chaveAux)) % FSIZE ) ) ; //aqui é onde tratarei o arquivo em loop
    while(Registro.chave != chaveAux){
        conth2++;
        if (conth2 >= FSIZE)
            break;
        leRegistro(( ((conth2 * hash2(chaveAux))+hash1(chaveAux)) % FSIZE ) );

    }
    if (conth2 >= FSIZE){
            printf("nao ha registro com chave: %d",chaveAux);
    }
    else{
        //int aux = (conth2 * hash2(chaveAux))+hash1(chaveAux);
        printf("chave: %d %s %d",Registro.chave, Registro.nome, Registro.idade);
    }
}

void insereChave(){
    if ( leRegistro(hash1(reg.chave)) ){
            if ((Registro.chave != -1)){ //caso ja exista um registro na posição
                if (Registro.chave == -2){
                    Registro = reg;
                    escreveRegistro(hash1(Registro.chave));
                }
                else{
                    if (Registro.chave == reg.chave) //caso esse registro teja a mesma chave do que esta para ser inserido
                        printf("registro com chave existente");
                    else{ //houve colisão
                        resolveColisaoInsercao();
                    }
                }

            }
            else {
                Registro = reg;
                escreveRegistro(hash1(Registro.chave));

            }
    }
}

void resolveColisaoInsercao(){
    int conth2 = 1;
    bool flag = false;
    leRegistro(( ((conth2 * hash2(reg.chave))+hash1(reg.chave)) % FSIZE ) ) ;
    while(Registro.chave != -1){
        if (Registro.chave == -2){
            break;
        }
        else{
            if (reg.chave == Registro.chave){ //isso aqui testa pra ver se ja existe o elemento a ser inserido em alguma posição
                flag=true;
                break;
            }
            conth2++;
            if (conth2 >= FSIZE)
                break;
            leRegistro(( ((conth2 * hash2(reg.chave))+hash1(reg.chave)) % FSIZE ));
        }
    }
    if (flag){ //caso houve uma chave igual a ser inserida
        printf("registro com chave existente");
    }
    else if (!flag){ //podendo inserir
        int aux = ( ((conth2 * hash2(reg.chave))+hash1(reg.chave)) % FSIZE );
        //printf ("%d\n",aux); //TESTE IMPORTANTE PARA VER SE ESTA INSERINDO NA DEVIDA POSICAO
        /*if (conth2 >= FSIZE){ //se o aux ja deu uma volta em todas as posições do arquivo
            printf("arquivo cheio");
        }*/
        Registro=reg;
        escreveRegistro(aux);
    }
}

void resolveColisaoRemocao(int chaveAux){ //igual ao resolveColisaoInsercao, mas com as instruções do flag no final modificadas.
    int conth2 = 1;
    bool flag = false;
    leRegistro(( ((conth2 * hash2(reg.chave))+hash1(reg.chave)) % FSIZE ) ) ;
    while(Registro.chave != -1){
        if (reg.chave == Registro.chave){ //isso aqui testa pra ver se ja existe o elemento a ser inserido em alguma posição
            flag=true;
            break;
        }
        conth2++;
        if (conth2 >= FSIZE)
            break;
        leRegistro(( ((conth2 * hash2(reg.chave))+hash1(reg.chave)) % FSIZE ));
    }
    if (flag){ //caso houve uma chave igual a ser inserida
            Registro.chave = -2;
            Registro.nome[0] = '\0';
            Registro.idade = 0;
            int aux = ( ((conth2 * hash2(reg.chave))+hash1(reg.chave)) % FSIZE );
            escreveRegistro(aux);
    }
    else if (!flag){ //podendo inserir
        printf("nao existe registro com chave: %d",reg.chave);
    }
}


void removeChave(int chaveAux){
        leRegistro(hash1(chaveAux));
        if (Registro.chave == chaveAux){
            Registro.chave = -2;
            Registro.nome[0] = '\0';
            Registro.idade = 0;
            escreveRegistro(hash1(chaveAux));
        }
        else {
                resolveColisaoRemocao(chaveAux);
        }
}

void printFile(){
    abreArquivo();
    if (!f){
        //printf("Erro ao abrir arquivo!");
        exit(-1);
    }
    int i;
    for (i = 0; i < FSIZE; i++){
        leRegistro(i);
        printf("Posicao: %d - ",i);
        if(Registro.chave == -1 || Registro.chave == -2){
            printf ("Registro vazio\n");
        }
        else{
            printf("Chave: %d;Nome: %s;Idade: %d;\n", Registro.chave, Registro.nome, Registro.idade);
        }
    }

    fclose(f);
}

void InitFile(){
    abreArquivo();
    if (!f){
        //printf("Erro ao abrir arquivo!");
        exit(-1);
    }

    int x, y=0;
    memset(&Registro,sizeof(Registro),0);
    for (x = 0; x < FSIZE; x++){
        Registro.chave = -1;
        Registro.nome[y] =  'a';
        Registro.idade= 0;
        escreveRegistro(x);
    }
    fclose(f);
}
