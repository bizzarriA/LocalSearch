#include <stdio.h>
#include <mem.h>
#include <stdlib.h>

#define NUMERONODI 5
#define NUMEROARCHI 10
#define KMASSIMO 3

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
        if(SoluzioneCandidata[j].N1==IdNodo && SoluzioneCandidata[j].Selected==1) {
            Estremo = SoluzioneCandidata[j].N2;
            break;
        }
        if(SoluzioneCandidata[j].N2==IdNodo && SoluzioneCandidata[j].Selected==1) {
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
                NodiNew[Estremo-1]--;
                trovato = 1;
            }
        }
    }
    int j=0;
    for(int i=0;i<NUMERONODI;i++){
        if(NodiNew[i]!=0){
            NodiCiclo[j]=i+1; /*perchè i nodi partono da 1, non da 0*/
            j++;
        }
    }
    return j;
}

void stampaLista(arco* ListaArchi){
    int Costo,i;
    printf("Soluzione:\n");
    for(i=0;i<NUMEROARCHI;i++) {
        if (ListaArchi[i].Selected == 1) {
            printf("Arco n %d, nodi %d-%d, costo %d\n", ListaArchi[i].Id, ListaArchi[i].N1,
                   ListaArchi[i].N2, ListaArchi[i].Costo);
        }
    }
    Costo=calcolaCosto(ListaArchi);
    printf("Costo spanning tree: %d\n\n",Costo);
}

int trovaArco(int N1, int N2, arco* SoluzioneCandidata){
    int IdArco;
    for(int i=0; i<NUMEROARCHI; i++){
        if(SoluzioneCandidata[i].N1==N1 && SoluzioneCandidata[i].N2==N2){
            IdArco=SoluzioneCandidata[i].Id;
            break;
        }
    }
    return IdArco;
}

int arcoDaRimuovere(int* NodiCiclo, arco* SoluzioneCandidata, int NumeroNodiCiclo){
    int CostoMax=0;
    int N1, N2, IdArco, IdMax;
    for(int i=0; i<NumeroNodiCiclo; i++){
        N1=NodiCiclo[i];
        for(int j=i+1;j<NumeroNodiCiclo; j++){
            N2=NodiCiclo[j];
            IdArco=trovaArco(N1,N2,SoluzioneCandidata);
            if(SoluzioneCandidata[IdArco-1].Selected==1 && SoluzioneCandidata[IdArco-1].Costo > CostoMax) {
                CostoMax = SoluzioneCandidata[IdArco - 1].Costo;
                IdMax=IdArco;
            }
        }
    }
    return IdMax;
}

int arcoObbligato(int* NodiCiclo, arco* SoluzioneCandidata, int NumeroNodiCiclo, int NodoObbligato){
    int CostoMax=0;
    int N1, N2, IdArco, IdMax;
    N1=NodoObbligato;
    for(int j=0;j<NumeroNodiCiclo; j++){
        if(N1!=NodiCiclo[j]){
            N2=NodiCiclo[j];
            if(N1<N2)
                IdArco=trovaArco(N1,N2,SoluzioneCandidata);
            else
                IdArco=trovaArco(N2,N1,SoluzioneCandidata);
            if(SoluzioneCandidata[IdArco-1].Selected==1 && SoluzioneCandidata[IdArco-1].Costo > CostoMax) {
                CostoMax = SoluzioneCandidata[IdArco - 1].Costo;
                IdMax=IdArco;
            }
        }
    }
    return IdMax;
}

void localSearch(arco* SoluzioneCandidata,int Id,int Nodi[]){
    int NodiCiclo[NUMERONODI];
    int NumeroNodiCiclo;
    int IdNuovo;
    int CostoIniziale, CostoFinale;
    CostoIniziale=calcolaCosto(SoluzioneCandidata);
    SoluzioneCandidata[Id-1].Selected=1;    /*aggiungi l'arco al ciclo*/
    Nodi[SoluzioneCandidata[Id-1].N1-1]++;  /*aumenta il grado dei nodi, in seguito all'aggiunta*/
    Nodi[SoluzioneCandidata[Id-1].N2-1]++;
    NumeroNodiCiclo=individuaCiclo(SoluzioneCandidata,NodiCiclo,Nodi);  /*trova i nodi connessi da un ciclo*/
    if(Nodi[SoluzioneCandidata[Id-1].N1-1]>KMASSIMO)
        IdNuovo = arcoObbligato(NodiCiclo, SoluzioneCandidata, NumeroNodiCiclo, SoluzioneCandidata[Id - 1].N1); /*trova i due archi del ciclo che sono associati al nodo con grado maggiore del massimo (ovvero l'arco appena inserito e quello che c'era già) ed elimina quello che tra i due ha costo maggiore*/
    else if(Nodi[SoluzioneCandidata[Id-1].N2-1]>KMASSIMO)
        IdNuovo=arcoObbligato(NodiCiclo,SoluzioneCandidata,NumeroNodiCiclo,SoluzioneCandidata[Id-1].N2);
    else
        IdNuovo=arcoDaRimuovere(NodiCiclo,SoluzioneCandidata,NumeroNodiCiclo);
    SoluzioneCandidata[IdNuovo-1].Selected=0;
    Nodi[SoluzioneCandidata[IdNuovo-1].N1-1]--;  /*riduce il grado dei nodi, in seguito alla rimozione*/
    Nodi[SoluzioneCandidata[IdNuovo-1].N2-1]--;
    CostoFinale=calcolaCosto(SoluzioneCandidata);
    if(CostoFinale<CostoIniziale)
        stampaLista(SoluzioneCandidata);
}

void main() {
    int Nodi[NUMERONODI];
    arco ListaArchi[NUMEROARCHI];
    int IdArcoMigliore, scan=0, i=0;

    FILE *fd;
    fd=fopen("C:\\Users\\Sara\\Documents\\GitHub\\LocalSearch\\LocalSearch\\istanze.txt", "r");
    if(fd==NULL){
        printf("Errore apertura file");
        exit(1);
    }

    while(scan!=EOF){
       if(i<NUMERONODI) {
           for (i = 0; i < NUMERONODI; i++) {
               scan = fscanf(fd, "%d", &Nodi[i]);
           }
       }
        scan=fscanf(fd,"%d/%d/%d/%d/%d",&ListaArchi[i-NUMERONODI].Id,&ListaArchi[i-NUMERONODI].N1,&ListaArchi[i-NUMERONODI].N2,&ListaArchi[i-NUMERONODI].Costo,&ListaArchi[i-NUMERONODI].Selected);
        i++;
    }
    fclose(fd);

    stampaLista(ListaArchi);
    //IdArcoMigliore=individuaArco(ListaArchi);
    for(IdArcoMigliore=1;IdArcoMigliore<=NUMEROARCHI;IdArcoMigliore++) {
        if(ListaArchi[IdArcoMigliore-1].Selected==0)
            if(Nodi[ListaArchi[IdArcoMigliore-1].N1-1]<KMASSIMO || Nodi[ListaArchi[IdArcoMigliore-1].N2-1]<KMASSIMO) /*effettua la local search solo se l'arco da controllare non va ad attaccarsi su nodi che hanno entrambi grado pari al massimo consentito, altrimenti scarta l'arco e prendine un'altro*/
                localSearch(ListaArchi, IdArcoMigliore, Nodi);
    }



}