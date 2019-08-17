#include <stdio.h>
#include <mem.h>
#include <stdlib.h>
#include <time.h>

#define NUMERONODI 7
#define NUMEROARCHI 21
#define KMASSIMO 3
#define NUMEROITERAZIONI 10
#define KMIGLIORI 4

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

//--------------------FUNZIONI PER IMPLEMENTAZIONE GREEDY-----------------------------------------------------------------------------------------------------------------------

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

//------------------------------FINE PARTE GREEDY-------------------------------------------------------------------------------------------------------------------------------
//-----------------------------FUNZIONI PER IMPLEMENTAZIONE LOCAL SEARCH--------------------------------------------------------------------------------------------------------

//---Funzioni per la generazione delle permutazioni della lista Id per la local search------------------------------------------------------------------------

void changeValues(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void shuffleRandom(int arr1[], int n) {
    srand(time(NULL));
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        changeValues(&arr1[i], &arr1[j]);
    }
}

void permuta(int* ListaId, int n) {     //bisogna passargli la lista id appena usata, perchè shuffleRandom la modifica direttamente
    int m = n / sizeof(ListaId[0]);
    int i;
    shuffleRandom(ListaId, m);
}

//----------------------------------------------------------------------------------------------------------------------------------------------

//CALCOLA IL COSTO DELA SOLUZIONE CONSIDERATA
int calcolaCosto(arco ListaArchi[]) {
    int CostoTot = 0;
    for (int i = 0; i < NUMEROARCHI; i++) {
        if (ListaArchi[i].Selected == 1)
            CostoTot = CostoTot + ListaArchi[i].Costo;
    }
    return CostoTot;
}

//HO UN ESTREMO E L'ARCO RESTITUISCO L'ALTRO ESTREMO
int findEstremo(arco* SoluzioneCandidata, int i, int* Nodi) {
    int IdNodo = i + 1;
    int Estremo;
    for (int j = 0; j < NUMEROARCHI; j++) {
        if (SoluzioneCandidata[j].N1 == IdNodo && SoluzioneCandidata[j].Selected == 1 && Nodi[SoluzioneCandidata[j].N2 - 1] != 0) {
            Estremo = SoluzioneCandidata[j].N2;
            break;
        }
        if (SoluzioneCandidata[j].N2 == IdNodo && SoluzioneCandidata[j].Selected == 1 && Nodi[SoluzioneCandidata[j].N1 - 1] != 0) {
            Estremo = SoluzioneCandidata[j].N1;
            break;
        }
    }
    return Estremo;
}

//CERCO TUTTI I NODI CON GRADO UNO E LI RIMUOVO, ITERO IL PROCEDIMENTO FINO A CHE NON HO PIù NODI DI GRADO 1. IL RISULTATO SARà IL CICLO CERCATO
int individuaCiclo(arco* SoluzioneCandidata, int* NodiCiclo, int* Nodi) {
    int* NodiNew = malloc(NUMERONODI * sizeof(int));
    memcpy(NodiNew, Nodi, NUMERONODI * sizeof(int));
    int trovato = 1;
    int Estremo;
    while (trovato == 1) {
        trovato = 0;
        for (int i = 0; i < NUMERONODI; i++) {
            if (NodiNew[i] == 1) {
                NodiNew[i] = 0;
                Estremo = findEstremo(SoluzioneCandidata, i, NodiNew);
                NodiNew[Estremo - 1]--;
                trovato = 1;
            }
        }
    }
    int j = 0;
    for (int i = 0; i < NUMERONODI; i++) {
        if (NodiNew[i] != 0) {
            NodiCiclo[j] = i + 1; /*perchè i nodi partono da 1, non da 0*/
            j++;
        }
    }
    return j;
}

//STAMPA LA LISTA DEGLI ARCHI SELEZIONATI E IL COSTO TOTALE
void stampaListaz(arco* ListaArchi) {
    int Costo, i;
    for (i = 0; i < NUMEROARCHI; i++) {
        if (ListaArchi[i].Selected == 1) {
            printf("Arco n %d, nodi %d-%d, costo %d\n", ListaArchi[i].Id, ListaArchi[i].N1,
                   ListaArchi[i].N2, ListaArchi[i].Costo);
        }
    }
    Costo = calcolaCosto(ListaArchi);
    printf("Costo spanning tree: %d\n\n", Costo);
}

//RESTITUISCO ID ARCO CHE VA DA N1 A N2
int trovaArco(int N1, int N2, arco* SoluzioneCandidata) {
    int IdArco;
    for (int i = 0; i < NUMEROARCHI; i++) {
        if (SoluzioneCandidata[i].N1 == N1 && SoluzioneCandidata[i].N2 == N2) {
            IdArco = SoluzioneCandidata[i].Id;
            break;
        }
    }
    return IdArco;
}

//RESTITUISCO L'ID DELL'ARCO DI COSTO MAGGIORE TRA QUELLI DI UN CICLO DATO
int arcoDaRimuovere(int* NodiCiclo, arco* SoluzioneCandidata, int NumeroNodiCiclo) {
    int CostoMax = 0;
    int N1, N2, IdArco, IdMax;
    for (int i = 0; i < NumeroNodiCiclo; i++) {
        N1 = NodiCiclo[i];
        for (int j = i + 1; j < NumeroNodiCiclo; j++) {
            N2 = NodiCiclo[j];
            IdArco = trovaArco(N1, N2, SoluzioneCandidata);
            if (SoluzioneCandidata[IdArco - 1].Selected == 1 && SoluzioneCandidata[IdArco - 1].Costo > CostoMax) {
                CostoMax = SoluzioneCandidata[IdArco - 1].Costo;
                IdMax = IdArco;
            }
        }
    }
    return IdMax;
}

//RESTITUISCO L'ARCO ASSOCIATO AD UN NODO OBBLIGATO E DI COSTO MAGGIORE TRA QUELLI DI UN CICLO
int arcoObbligato(int* NodiCiclo, arco* SoluzioneCandidata, int NumeroNodiCiclo, int NodoObbligato) {
    int CostoMax = 0;
    int N1, N2, IdArco, IdMax;
    N1 = NodoObbligato;
    for (int j = 0; j < NumeroNodiCiclo; j++) {
        if (N1 != NodiCiclo[j]) {
            N2 = NodiCiclo[j];
            if (N1 < N2)
                IdArco = trovaArco(N1, N2, SoluzioneCandidata);
            else
                IdArco = trovaArco(N2, N1, SoluzioneCandidata);
            if (SoluzioneCandidata[IdArco - 1].Selected == 1 && SoluzioneCandidata[IdArco - 1].Costo > CostoMax) {
                CostoMax = SoluzioneCandidata[IdArco - 1].Costo;
                IdMax = IdArco;
            }
        }
    }
    return IdMax;
}

//ESEGUO LOCAL SEARCH CON LIMITE GRADO NODI <= K
void esploraIntorno(arco* SoluzioneCandidata, int Id, int Nodi[], int* IdNuovo) {  //ritorna anche l'arco che ha appena rimosso
    int NodiCiclo[NUMERONODI];
    int NumeroNodiCiclo;
    //int CostoIniziale, CostoFinale;       NELLA NUOVA VERSIONE DI LOCAL SEARCH NON SERVE, VIENE FATTO FUORI DALLA FUNZIONE
    //CostoIniziale=calcolaCosto(SoluzioneCandidata);
    SoluzioneCandidata[Id - 1].Selected = 1;    /*aggiungi l'arco al ciclo*/
    Nodi[SoluzioneCandidata[Id - 1].N1 - 1]++;  /*aumenta il grado dei nodi, in seguito all'aggiunta*/
    Nodi[SoluzioneCandidata[Id - 1].N2 - 1]++;
    NumeroNodiCiclo = individuaCiclo(SoluzioneCandidata, NodiCiclo, Nodi);  /*trova i nodi connessi da un ciclo*/
    /*cerco i due archi del ciclo che sono associati al nodo con grado maggiore del massimo se c'è
     * (ovvero l'arco appena inserito e quello che c'era già) ed elimina quello che tra i due ha costo maggiore*/
    if (Nodi[SoluzioneCandidata[Id - 1].N1 - 1] > KMASSIMO)
        * IdNuovo = arcoObbligato(NodiCiclo, SoluzioneCandidata, NumeroNodiCiclo, SoluzioneCandidata[Id - 1].N1);
    else if (Nodi[SoluzioneCandidata[Id - 1].N2 - 1] > KMASSIMO)
        * IdNuovo = arcoObbligato(NodiCiclo, SoluzioneCandidata, NumeroNodiCiclo, SoluzioneCandidata[Id - 1].N2);
    else
        *IdNuovo = arcoDaRimuovere(NodiCiclo, SoluzioneCandidata, NumeroNodiCiclo);
    SoluzioneCandidata[*IdNuovo - 1].Selected = 0;
    Nodi[SoluzioneCandidata[*IdNuovo - 1].N1 - 1]--;  /*riduce il grado dei nodi, in seguito alla rimozione*/
    Nodi[SoluzioneCandidata[*IdNuovo - 1].N2 - 1]--;
    //CostoFinale=calcolaCosto(SoluzioneCandidata);  NELLA NUOVA VERSIONE DI LOCAL SEARCH NON SERVE, VIENE FATTO FUORI DALLA FUNZIONE
    //if(CostoFinale<CostoIniziale)
    //   stampaLista(SoluzioneCandidata);
}

//CREA UNA LISTA DEI SOLI ID
void individuaId(arco* ListaArchi, int* ListaId) {
    for (int i = 0; i < NUMEROARCHI; i++) {
        ListaId[i] = ListaArchi[i].Id;
    }
}

int localSearch(arco* SoluzioneIniziale, int* NodiIniziali) {
    int NodiTemporanei[NUMERONODI];
    int ListaId[NUMEROARCHI];
    arco SoluzioneTemporanea[NUMEROARCHI];
    int CostoMiglioreAttuale, CostoAttuale, IdRim, IdAggiunto, IdRimosso;
    int k = 0;
    int FindBest = 1;
    int n;

    CostoMiglioreAttuale = calcolaCosto(SoluzioneIniziale);

    individuaId(SoluzioneIniziale, ListaId);
    n = sizeof(ListaId);

    while (FindBest) {   //se ad un ciclo non trova una soluzione migliore della precedente, vuol dire che ha raggiunto l'ottimo locale
        permuta(ListaId, n);
        FindBest = 0;
        IdAggiunto = -1;
        IdRimosso = -2;
        for (int j = 0; j <= NUMEROARCHI; j++) {
            if (SoluzioneIniziale[ListaId[j] - 1].Selected == 0) {
                /*effettua la local search solo se l'arco da controllare non va ad attaccarsi su nodi che hanno grado minore di K altrimenti scarta l'arco e prendine un'altro*/
                if (NodiIniziali[SoluzioneIniziale[ListaId[j] - 1].N1 - 1] < KMASSIMO ||
                    NodiIniziali[SoluzioneIniziale[ListaId[j] - 1].N2 - 1] < KMASSIMO) {
                    memcpy(SoluzioneTemporanea, SoluzioneIniziale, sizeof(SoluzioneTemporanea));  //serve una copia perchè devo poter controllare arco per arco. La soluzione iniziale viene modificata solo dopo aver esplorato tutto l'intorno
                    memcpy(NodiTemporanei, NodiIniziali, sizeof(NodiTemporanei));
                    esploraIntorno(SoluzioneTemporanea, ListaId[j], NodiTemporanei, &IdRim);
                    CostoAttuale = calcolaCosto(SoluzioneTemporanea);
                    if (CostoAttuale < CostoMiglioreAttuale && IdAggiunto != IdRimosso) { //se la soluzione è migliore, si segna quale arco è stato aggiunto e quale rimosso per raggiungerla
                        CostoMiglioreAttuale = CostoAttuale;
                        IdAggiunto = ListaId[j];
                        IdRimosso = IdRim;
                        FindBest = 1;
                    }
                }
            }
        }
        k++;
        if (FindBest) {   //aggiungo e toglo la coppia di archi che mi permette di avere la soluzione migliore per quell'intorno
            SoluzioneIniziale[IdAggiunto - 1].Selected = 1;
            SoluzioneIniziale[IdRimosso - 1].Selected = 0;
            NodiIniziali[SoluzioneIniziale[IdAggiunto - 1].N1 - 1]++;
            NodiIniziali[SoluzioneIniziale[IdAggiunto - 1].N2 - 1]++;
            NodiIniziali[SoluzioneIniziale[IdRimosso - 1].N1 - 1]--;
            NodiIniziali[SoluzioneIniziale[IdRimosso - 1].N2 - 1]--;
            printf("Iterazione %d:\nAggiunto arco %d e rimosso arco %d.\n", k, IdAggiunto, IdRimosso);
            stampaListaz(SoluzioneIniziale);
        }
        if (!FindBest) {
            printf("All'iterazione %d non e' stata trovata soluzione migliore.\nL'ottimo locale trovato risulta quindi:\n", k);
            stampaListaz(SoluzioneIniziale);
        }
    }
    return CostoMiglioreAttuale;
}

//----------------------------FINE PARTE LS-------------------------------------------------------------------------------------------------------------------------------------

void main() {
    arco ListaArchi[NUMEROARCHI];
    solution ListaSoluzioni[NUMEROITERAZIONI]; //lista delle soluzioni di partenza create dalla greedy
    int IdSoluzioneMigliore=-1;
    int scan=0, i=0, CostoMigliore=0;
    int k;

    //APRO FILE E LEGGO ISTANZE
    FILE *fd;
    // fd=fopen("C:\\Users\\alice\\OneDrive\\Documents\\GitHub\\LocalSearch\\LocalSearch\\istanze2.txt", "r");
    fd=fopen("C:\\Users\\Sara\\Documents\\GitHub\\LocalSearch\\GraspConLS\\istanzeGrasp.txt", "r");
    if(fd==NULL){
        printf("Errore apertura file");
        exit(1);
    }

    while(scan!=EOF){   //questa scanf legge solo gli archi perchè l'array con i gradi dei nodi se lo calcola la greedy
        scan=fscanf(fd,"%d/%d/%d/%d",&ListaArchi[i].Id,&ListaArchi[i].N1,&ListaArchi[i].N2,&ListaArchi[i].Costo);
        ListaArchi[i].Selected=0;   //all'inizio nessun arco è selezionato
        ListaArchi[i].Ammissibile=1;    //all'inizio della greedy, tutti gli archi possono essere ammissibili
        CostoMigliore=CostoMigliore+ListaArchi[i].Costo;
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
        ListaSoluzioni[j].Costo=calcolaCosto(ListaSoluzioni[j].ListaArchi);
        printf("Costo spanning tree iniziale: %d\n\n",ListaSoluzioni[j].Costo);
        ListaSoluzioni[j].Costo=localSearch(ListaSoluzioni[j].ListaArchi,ListaSoluzioni[j].ListaNodi);
        if(ListaSoluzioni[j].Costo<CostoMigliore){
            IdSoluzioneMigliore=j;
            CostoMigliore=ListaSoluzioni[j].Costo;
        }
    }

    printf("\nMigliore ottimo locale trovato\n");
    stampaLista(ListaSoluzioni[IdSoluzioneMigliore].ListaArchi);
    printf("Costo spanning tree: %d\n",ListaSoluzioni[IdSoluzioneMigliore].Costo);
}
