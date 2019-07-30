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

int findEstremo(arco* SoluzioneCandidata,int i){
    int IdNodo=i+1;
    int Estremo;
    for(int j=0;j<NUMEROARCHI;j++){
        if(SoluzioneCandidata[j].N1==IdNodo) {
            Estremo = SoluzioneCandidata[j].N2;
            break;
        }
        if(SoluzioneCandidata[j].N2==IdNodo) {
            Estremo = SoluzioneCandidata[j].N1;
            break;
        }
    }
    return Estremo;
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
                Estremo = findEstremo(SoluzioneCandidata,i);
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

void stampaLista(arco* ListaArchi){
    int Costo;
    for(int i=0;i++;i<NUMEROARCHI) {
        if (ListaArchi[i].Selected == 1) {
            printf("Soluzione: arco n %d, nodi %d %d, costo %d\n", ListaArchi[i].Id, ListaArchi[i].N1,
                   ListaArchi[i].N2, ListaArchi[i].Costo);
        }
    }
    Costo=calcolaCosto(ListaArchi);
    printf("Costo spanning tree: %d\n",Costo);
}



void localSearch(arco* SoluzioneCandidata,int Id,int Nodi[]){
    int NodiCiclo[NUMERONODI];
    int NumeroNodiCiclo;
    int NuovoCosto;
    int IdNuovo;
    SoluzioneCandidata[Id-1].Selected=1;
    NumeroNodiCiclo=individuaCiclo(SoluzioneCandidata,NodiCiclo,Nodi);
    IdNuovo=arcoDaRimuovere(NodiCiclo,SoluzioneCandidata,NumeroNodiCiclo);
    SoluzioneCandidata[IdNuovo-1].Selected=0;
    stampaLista(SoluzioneCandidata);
}

void main() {
    int Nodi[NUMERONODI];
    arco ListaArchi[NUMEROARCHI];
    int IdArcoMigliore;

    stampaLista(ListaArchi);
    //IdArcoMigliore=individuaArco(ListaArchi);
    for(IdArcoMigliore=1;IdArcoMigliore<=NUMEROARCHI;IdArcoMigliore++) {
        if(ListaArchi[IdArcoMigliore].Selected==0)
        localSearch(ListaArchi, IdArcoMigliore, Nodi);
    }



}