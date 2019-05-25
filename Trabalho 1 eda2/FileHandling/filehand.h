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
    char chave[21];
    char nome[51];
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

void consultaChave(char str[21]){
    int cont = 1;
    int chaveAux = stringToInt(str);
    leRegistro(hash1(chaveAux));
    if (Registro.chave == chaveAux){
        printf("%s %s\n",Registro.chave, Registro.nome);
        resolveColisaoConsulta(chaveAux);
    }
    else
        resolveColisaoConsulta(chaveAux);
}

void resolveColisaoConsulta(int chaveAux){
    int conth2 = 1;
    leRegistro(( ((conth2 * hash2(chaveAux))+hash1(chaveAux)) % FSIZE ) ) ; //aqui é onde tratarei o arquivo em loop
    while(conth2 < FSIZE){
        conth2++;
        if (Registro.chave == chaveAux){
            printf("%d %s\n",Registro.chave, Registro.nome);
        }
        leRegistro(( ((conth2 * hash2(chaveAux))+hash1(chaveAux)) % FSIZE ) );

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
                        resolveColisaoInsercao();
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
    //if(((conth2 * hash2(reg.chave))+hash1(reg.chave)) % FSIZE == hash1(reg.chave)){
    //	return;
 	//}
    while(Registro.chave != -1){
        if (Registro.chave == -2){
            break;
        }
        else{
            conth2++;
            if (conth2 >= FSIZE)
            	break;
            leRegistro(( ((conth2 * hash2(reg.chave))+hash1(reg.chave)) % FSIZE ));
        }
    }
    if(conth2 < FSIZE){
        int aux = ( ((conth2 * hash2(reg.chave))+hash1(reg.chave)) % FSIZE );
        Registro=reg;
        escreveRegistro(aux);
    }
}

void removeChave(char str[21]){
        int chaveAux = stringToInt(str);
        leRegistro(hash1(chaveAux));
        if (stringToInt(Registro.chave) == chaveAux){
            Registro.chave = -2;
            Registro.nome[0] = '\0';
            escreveRegistro(hash1(chaveAux));
            resolveColisaoRemocao(chaveAux);
        }
        else {
            resolveColisaoRemocao(chaveAux);
        }
}

void resolveColisaoRemocao(int chaveAux){ //igual ao resolveColisaoInsercao, mas com as instruções do flag no final modificadas.
    int conth2 = 1;
    leRegistro(( ((conth2 * hash2(chaveAux))+hash1(chaveAux)) % FSIZE ) ) ;
    while(Registro.chave != -1){
        if (chaveAux == Registro.chave){
            Registro.chave = -2;
            Registro.nome[0] = '\0';
            int aux = ( ((conth2 * hash2(chaveAux))+hash1(chaveAux)) % FSIZE );
            escreveRegistro(aux);
        }
        conth2++;
        if (conth2 >= FSIZE){
            //printf("assssd\n");
            break;
        }
        leRegistro(( ((conth2 * hash2(chaveAux))+hash1(chaveAux)) % FSIZE ));
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
            printf("Chave: %d;Nome: %s;\n", Registro.chave, Registro.nome);
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
        escreveRegistro(x);
    }
    fclose(f);
}

int stringToInt(char str[21]){
    int key = 0;
    int count = 0;
    int potencia = 1;
    while(1){
        if(str[count] == '\0')
            break;
        key += ((int)str[count]) * pow(2, potencia);
        count++;
        potencia++;
    }
    return key;
}
