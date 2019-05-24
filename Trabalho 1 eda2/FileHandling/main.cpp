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
            scanf("%d", &chaveAux);
            consultaChave(chaveAux);
            fclose(f);
        }

        if (entrada=='i'){
            //struct Registro reg;
            scanf("%d\n",&reg.chave);

            int i=0;
            for (i=0; i<=20; i++){
                scanf("%c", &reg.nome[i]);
                if (reg.nome[i] == '\n')
                    break;
            }
            reg.nome[i]='\0';

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

            scanf("%d", &chaveAux);
            removeChave(chaveAux);

            fclose(f);
        }

        if(entrada == 'p'){
            printFile();
        }

      } while (entrada!='e');

    return 0;
}
