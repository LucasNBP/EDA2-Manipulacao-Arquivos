#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <string>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include "filehand.h"


using namespace std;

int main(){
    int chaveAux =0;
//////////////////////AQUI SE TRATA DA INICIALIZACAO DO ARQUIVO PRINCIPAL////////////////////////
    abreArquivoAux();
    int aux;
    if (!f){
        printf("Erro ao abrir arquivo auxiliar!\n\n");
        exit(-1);
    }
    if (! fread(&aux,sizeof(int),1,f) ){
        aux=1;
        if (!fwrite(&aux, sizeof(int),1,f))
            printf("erro na escrita da inicializacao");
        else{
            fclose(f);
            abreArquivo();
            if (!f){
                printf("Erro ao abrir arquivo!\n\n");
                exit(-1);
            }
            InitFile();
        }
    }

    fclose(f);
/////////////////////////////////////////////////////////////////////////////////////////////

    do{
        scanf("%c", &entrada);

        if(entrada == 'c'){
            abreArquivo();
            if (!f){
                printf("Erro ao abrir arquivo!\n\n");
                exit(-1);
            }
            fflush(stdin);
            char buffer1[21];
            fgets (buffer1, sizeof(buffer1), stdin);
            if(buffer1[strlen(buffer1)-1] == '\n'){
                buffer1[strlen(buffer1)-1] = '\0';
            }
            fflush(stdin);
            consultaChave(buffer1);
            fclose(f);
        }

        if (entrada=='i'){
            fflush(stdin);
            char buffer1[21];
            fgets (buffer1, sizeof(buffer1), stdin);
            if(buffer1[strlen(buffer1)-1] == '\n'){
                buffer1[strlen(buffer1)-1] = '\0';
            }
            strcpy(reg.chave, buffer1);
            fflush(stdin);

            char buffer2[51];
            fgets (buffer2, sizeof(buffer2), stdin);
            if(buffer2[strlen(buffer2)-1] == '\n'){
                buffer2[strlen(buffer2)-1] = '\0';
            }
            strcpy(reg.nome, buffer2);
            fflush(stdin);

            //inserindo
            abreArquivo();
            if (!f){
                printf("Erro ao abrir arquivo!\n\n");
                exit(-1);
            }
            insereChave();
            /*

            */

            fclose(f);
        }

        if(entrada == 'r'){
            abreArquivo();
            if (!f){
                printf("Erro ao abrir arquivo!\n\n");
                exit(-1);
            }

            fflush(stdin);
            char buffer1[21];
            fgets (buffer1, sizeof(buffer1), stdin);
            if(buffer1[strlen(buffer1)-1] == '\n'){
                buffer1[strlen(buffer1)-1] = '\0';
            }
            fflush(stdin);
            removeChave(buffer1);

            fclose(f);
        }

        if(entrada == 'p'){
            printFile();
        }

      } while (entrada!='e');

    return 0;
}
