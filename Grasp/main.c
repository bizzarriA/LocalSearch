#include <stdio.h>
#include <mem.h>
#include <stdlib.h>
#include <time.h>

#define NUMERONODI 7
#define NUMEROARCHI 21
#define KMASSIMO 3
#define NUMEROITERAZIONI 5
#define KMIGLIORI 3

typedef struct s_arco{
    int Id;
    int N1;
    int N2;
    int Costo;
    int Selected;
    int Ammissibile;    //serve SOLO nella greedy e se posto a 0 significa che l'arco è già stato individuato come non accettabile (per grado o percorso creato) e non deve essere ricontrollato. Nel file deve essere =1
}arco;

typedef struct s_soluzione{
    arco ListaArchi[NUMEROARCHI];
    int ListaNodi[NUMERONODI];
    int Costo;
}solution;

//STAMPA LA LISTA DEGLI ARCHI SELEZIONATI E IL COSTO TOTALE
void stampaLista(arco* ListaArchi){
    int Costo,i;
    printf("Soluzione:\n");
    for(i=0;i<NUMEROARCHI;i++) {
        if (ListaArchi[i].Selected == 1) {
            printf("Arco n %d, nodi %d-%d, costo %d\n", ListaArchi[i].Id, ListaArchi[i].N1,
                   ListaArchi[i].N2, ListaArchi[i].Costo);
        }
    }
}

int allNodes(int* NodiAttuali){ //ritorna 1 se tutti i nodi sono stati presi (ovvero se nessuno ha grado 0)
    for(int i=0;i<NUMERONODI;i++){
        if(NodiAttuali[i]==0)
            return 0;
    }
    return 1;
}

int almenoUnNodo(arco ArcoAttuale,int* NodiAttuali){    //ritona 1 se almeno uno dei due nodi in cui incide ArcoAttuale è già stato preso, ovvero se ArcoAttuale è adiacente ad un arco che fa già parte dell'albero
    if(NodiAttuali[ArcoAttuale.N1-1]!=0 || NodiAttuali[ArcoAttuale.N2-1]!=0)
        return 1;
    return 0;
}

int is_percorso(arco ArcoAttuale,int* NodiAttuali){ //ritorna 1 se entrambi i nodi in cui incide il nuovo arco sono già stati presi, ovvero se c'è già un percorso tra loro
    if(NodiAttuali[ArcoAttuale.N1-1]!=0 && NodiAttuali[ArcoAttuale.N2-1]!=0)
        return 1;
    return 0;
}

int gradoEccessivo(arco ArcoAttuale, int* NodiAttuali){ //ritorna 1 se almeno uno dei nodi in cui incide il nuovo arco ha già raggiunto il grado massimo consentito
    if(NodiAttuali[ArcoAttuale.N1-1]==KMASSIMO || NodiAttuali[ArcoAttuale.N2-1]==KMASSIMO)
        return 1;
    return 0;
}

int notInLista(int Id,int* Lista,int Length){   //ritona 1 se l'elemento id non appartiene all'array Lista
    for(int i=0;i<Length;i++){
        if(Lista[i]==Id)
            return 0;
    }
    return 1;
}

int trovaMigliore(arco* SoluzioneAttuale, int* DaRicontrollare,int j,int* IdArchiMigliori,int i){ //trova l'i-esimo arco migliore che non sia già stato preso tra i migliori e che non sia tra quelli momentaneamente non accettabili perchè non adiacenti
    int IdMin=0;
    int Min=10000;  //valore fittizzio, da stabilire, magari letto da file
    for(int j=0;j<NUMEROARCHI;j++){
        if(SoluzioneAttuale[j].Costo<Min && SoluzioneAttuale[j].Ammissibile==1 && SoluzioneAttuale[j].Selected!=1 && notInLista(SoluzioneAttuale[j].Id,DaRicontrollare,j) && notInLista(SoluzioneAttuale[j].Id,IdArchiMigliori,i)){
            Min=SoluzioneAttuale[j].Costo;
            IdMin=SoluzioneAttuale[j].Id;
        }
    }
    return IdMin;
}

void cercaArchi(arco* SoluzioneAttuale, int* DaRicontrollare,int j,int* IdArchiMigliori){
    int i=0;
    for(int j=0;j<KMIGLIORI;j++){
        IdArchiMigliori[j]=trovaMigliore(SoluzioneAttuale,DaRicontrollare,j,IdArchiMigliori,i);
        i++;
    }
}

void greedyCostruttiva(arco* SoluzioneAttuale, int* NodiAttuali, arco* ListaArchi){
    int IdArchiMigliori[KMIGLIORI];
    int i,IdArco;
    int j=0;
    int PrimoGiro=1;    //al posto della funzione che ogni volta controlla i gradi dei nodi per vedere se almeno uno è stato preso, setto una semplice variabile
    int DaRicontrollare[NUMEROARCHI]={0}; //contiene gli Id di archi che non potevano essere presi al momento perchè non adiacenti ad altri
    while(!allNodes(NodiAttuali)) {
        cercaArchi(SoluzioneAttuale,DaRicontrollare,j,IdArchiMigliori);
        srand ( time(NULL) );
        i=rand() % KMIGLIORI;
        IdArco=IdArchiMigliori[i];
        if(is_percorso(SoluzioneAttuale[IdArco-1],NodiAttuali) || gradoEccessivo(SoluzioneAttuale[IdArco-1],NodiAttuali)) {
            SoluzioneAttuale[IdArco-1].Ammissibile=0;
        }
        else if(PrimoGiro || almenoUnNodo(SoluzioneAttuale[IdArco-1],NodiAttuali)){
            PrimoGiro=0;
            SoluzioneAttuale[IdArco-1].Selected=1;
            NodiAttuali[SoluzioneAttuale[IdArco-1].N1-1]++;
            NodiAttuali[SoluzioneAttuale[IdArco-1].N2-1]++;
            memset(DaRicontrollare,0, (j+1)*sizeof(int));   //svuota la lista dei nodi che non potevano essere presi perchè non adiacenti
            j=0;
        }
        else {
            DaRicontrollare[j]=SoluzioneAttuale[IdArco-1].Id;
            j++;
        }
    }
}

void main() {
    arco ListaArchi[NUMEROARCHI];
    solution ListaSoluzioni[NUMEROITERAZIONI]; //lista delle soluzioni di partenza create dalla greedy
    int IdSoluzioneMigliore;
    solution SoluzioneMigliore;
    int scan=0, i=0;
    int k;

    //APRO FILE E LEGGO ISTANZE
    FILE *fd;
    // fd=fopen("C:\\Users\\alice\\OneDrive\\Documents\\GitHub\\LocalSearch\\LocalSearch\\istanze2.txt", "r");
    fd=fopen("C:\\Users\\Sara\\Documents\\GitHub\\LocalSearch\\Grasp\\istanzeGrasp.txt", "r");
    if(fd==NULL){
        printf("Errore apertura file");
        exit(1);
    }

    while(scan!=EOF){   //questa scanf legge solo gli archi perchè l'array con i gradi dei nodi se lo calcola la greedy
        scan=fscanf(fd,"%d/%d/%d/%d",&ListaArchi[i].Id,&ListaArchi[i].N1,&ListaArchi[i].N2,&ListaArchi[i].Costo);
        ListaArchi[i].Selected=0;   //all'inizio nessun arco è selezionato
        ListaArchi[i].Ammissibile=1;    //all'inizio della greedy, tutti gli archi possono essere ammissibili
        i++;
    }
    fclose(fd);

    for(int j=0;j<NUMEROITERAZIONI;j++){
        memset(ListaSoluzioni[j].ListaNodi,0, NUMERONODI*sizeof(int));
        memcpy(ListaSoluzioni[j].ListaArchi,ListaArchi,NUMEROARCHI*sizeof(arco));
        greedyCostruttiva(ListaSoluzioni[j].ListaArchi,ListaSoluzioni[j].ListaNodi,ListaArchi);
        k=j+1;
        printf("\nSoluzione iniziale n %d\n",k);
        stampaLista(ListaSoluzioni[j].ListaArchi);
        //localSearch(SoluzioneAttuale,NodiAttuali,CostoAttuale);
        //if(CostoAttuale<CostoMigliore){
            //la soluzione migliore(con relativi nodi) viene sostituita da quella attuale
    }

    printf("\nMigliore ottimo locale trovato\n");
    stampaLista(SoluzioneMigliore.ListaArchi);
    printf("Costo spanning tree: %d\n",SoluzioneMigliore.Costo);
}