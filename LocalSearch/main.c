#include <stdio.h>
#include <mem.h>
#include <stdlib.h>

#define NUMERONODI 5
#define NUMEROARCHI 8

typedef struct s_arco{
    int Id;
    int N1;
    int N2;
    int Costo;
    int Selected;
}arco;

int individuaArco(arco ListaArchi[]){
    int min_costo=100;
    int ArcoMigliore;
    for(int i=0; i <NUMEROARCHI;i++){
        if (ListaArchi[i].Costo<min_costo && ListaArchi[i].Selected==0){
            min_costo=ListaArchi[i].Costo;
            ArcoMigliore=ListaArchi[i].Id;
        }
    }
    return ArcoMigliore;
}

int calcolaCosto(arco ListaArchi[]){
    int CostoTot=0;
    for(int i=0;i<NUMEROARCHI;i++){
        if(ListaArchi[i].Selected==1)
           CostoTot=CostoTot+ListaArchi[i].Costo;
    }
    return CostoTot;
}

int individuaCiclo(arco* SoluzioneCandidata, int* NodiCiclo,int* Nodi){
    int* NodiNew=malloc(NUMERONODI*sizeof(int));
    memcpy(NodiNew,Nodi,NUMERONODI*sizeof(int));
    int trovato=1;
    int Estremo;
    while(trovato==1) {
        trovato=0;
        for (int i = 0; i < NUMERONODI; i++) {
            if (NodiNew[i] == 1) {
                NodiNew[i] = 0;
                Estremo = findEstremo(SoluzioneCandidata);
                NodiNew[Estremo]--;
                trovato = 1;
            }
        }
    }
    int j=0;
    for(int i=0;i<NUMERONODI;i++){
        if(NodiNew[i]!=0){
            NodiCiclo[j]=i;
            j++;
        }
    }
    return j+1;
}

void localSearch(arco* ListaArchi,int Id,int Nodi[]){
    arco SoluzioneCandidata[NUMEROARCHI];
    int NodiCiclo[NUMERONODI];
    int NumeroNodiCiclo;
    memcpy(SoluzioneCandidata,ListaArchi,NUMEROARCHI*sizeof(arco));
    SoluzioneCandidata[Id-1].Selected=1;
    NumeroNodiCiclo=individuaCiclo(SoluzioneCandidata,NodiCiclo,Nodi);

}

void main() {
    int Nodi[NUMERONODI];
    arco ListaArchi[NUMEROARCHI];
    int IdArcoMigliore;

    IdArcoMigliore=individuaArco(ListaArchi);
    localSearch(ListaArchi,IdArcoMigliore,Nodi);



}