
#include <stdio.h>
#include <mem.h>
#include <stdlib.h>
#include <time.h>

#define NUMERONODI 5
#define NUMEROARCHI 10
#define KMASSIMO 3
#define TABUSIZE 4
#define ARCHIDIVERSIFICARE 2
#define MAXITERAZIONI 3     //numero di iterazioni di seguito in cui non vi sono miglioramenti, ovvero si va in stallo
#define PENALIZZAZIONE 100  //valore di cui viene penalizzata l'accettazione di alberi non ammissibili

typedef struct s_arco {
    int Id;
    int N1;
    int N2;
    int Costo;
    int Selected;
}arco;

//OBSOLETO
int individuaArco(arco ListaArchi[]) {
    int min_costo = 100;
    int ArcoMigliore;
    for (int i = 0; i < NUMEROARCHI; i++) {
        if (ListaArchi[i].Costo < min_costo&& ListaArchi[i].Selected == 0) {
            min_costo = ListaArchi[i].Costo;
            ArcoMigliore = ListaArchi[i].Id;
        }
    }
    return ArcoMigliore;
}

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
int calcolaCosto(arco ListaArchi[]) { //se la sol precedente era ammissibile, ogni cambio di arco non ammissibile penalizza di 100; se era non ammissibile, ogni cambio di arco che la rende ammissibile abbassa il costo precedente di 100
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
    int Estremo=-1;
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
void stampaLista(arco* ListaArchi) {
    int Costo, i;
    for (i = 0; i < NUMEROARCHI; i++) {
        if (ListaArchi[i].Selected == 1) {
            printf("Arco n %d, nodi %d-%d, costo %d\n", ListaArchi[i].Id, ListaArchi[i].N1,
                   ListaArchi[i].N2, ListaArchi[i].Costo);
        }
    }
    Costo = calcolaCosto(ListaArchi);
    printf("Costo spanning tree: %d\n", Costo);
}

//Stampa grado nodi

/*void stampaNodi(int Nodi[]){
    for (int i =0; i<NUMERONODI; i++){
        printf("Nodo%d ha grado %d\n",i, Nodi[i]);
    }
}*/

//RITORNA IL NUMERO DI NODI CON GRADO SUPERIORE AL MASSIMO
int vincoliInfranti(int *ListaNodi){
    int n=0;
    for(int i=0;i<NUMERONODI;i++){
        if(ListaNodi[i]>KMASSIMO)
            n++;
    }
    return n;
}

//RESTITUISCO ID ARCO CHE VA DA N1 A N2
int trovaArco(int N1, int N2, arco* SoluzioneCandidata) {
    int IdArco, N;
    if(N1>N2){
        N=N1;
        N1=N2;
        N2=N;
    }
    for (int i = 0; i < NUMEROARCHI; i++) {
        if (SoluzioneCandidata[i].N1 == N1 && SoluzioneCandidata[i].N2 == N2) {
            IdArco = SoluzioneCandidata[i].Id;
            break;
        }
    }
    return IdArco;
}

int notInLista(int Id,int* Lista,int Length){   //ritona 1 se l'elemento id non appartiene all'array Lista
    for(int i=0;i<Length;i++){
        if(Lista[i]==Id)
            return 0;
    }
    return 1;
}

int inLista(int Id,int* Lista,int Length){   //ritona 1 se l'elemento id appartiene all'array Lista
    for(int i=0;i<Length;i++){
        if(Lista[i]==Id)
            return 1;
    }
    return 0;
}

//trovo archi ciclo

int trovaArchiCiclo(arco ListaArchi[], int NodiCiclo[], int NumeroNodiCiclo, int IdArchiCiclo[]){
    int N1, N2, k=0;
    for(int i=0; i<NumeroNodiCiclo; i++){
        N1=NodiCiclo[i];
        for(int j=i+1; j<NumeroNodiCiclo; j++){
            N2=NodiCiclo[j];
            IdArchiCiclo[k]=trovaArco(N1,N2,ListaArchi);
            k++;
        }
    }
    return k;
}


//RESTITUISCO L'ID DELL'ARCO DI COSTO MAGGIORE TRA QUELLI DI UN CICLO DATO CHE NON FACCIA PARTE DELLA TABULIST
int arcoDaRimuovere(int* NodiCiclo, arco* SoluzioneCandidata, int NumeroNodiCiclo,int* TabuList,int t) {
    int CostoMax = 0;
    int N1, N2, IdArco, IdMax = -1;
    for (int i = 0; i < NumeroNodiCiclo; i++) {
        N1 = NodiCiclo[i];
        for (int j = i + 1; j < NumeroNodiCiclo; j++) {
            N2 = NodiCiclo[j];
            IdArco = trovaArco(N1, N2, SoluzioneCandidata);
            if (SoluzioneCandidata[IdArco - 1].Selected == 1 && SoluzioneCandidata[IdArco - 1].Costo > CostoMax &&
                notInLista(IdArco, TabuList, t)) {
                CostoMax = SoluzioneCandidata[IdArco - 1].Costo;
                IdMax = IdArco;
            }
        }
    }
    return IdMax;
}

int arcoDaRimuovereTabu(int* NodiCiclo, arco* SoluzioneCandidata, int NumeroNodiCiclo,int* TabuList,int t) {
    int CostoMax = 0;
    int N1, N2, IdArco, IdMax = -1;
    for (int i = 0; i < NumeroNodiCiclo; i++) {
        N1 = NodiCiclo[i];
        for (int j = i + 1; j < NumeroNodiCiclo; j++) {
            N2 = NodiCiclo[j];
            IdArco = trovaArco(N1, N2, SoluzioneCandidata);
            if (SoluzioneCandidata[IdArco - 1].Selected == 1 && SoluzioneCandidata[IdArco - 1].Costo > CostoMax &&
                inLista(IdArco, TabuList, t)) {
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
void localSearch(arco* SoluzioneCandidata, int Id, int Nodi[], int* IdNuovo,int* TabuList,int t, int TabuOn) {  //ritorna anche l'arco che ha appena rimosso
    int NodiCiclo[NUMERONODI], IdArchiCiclo[NUMEROARCHI];
    int NumeroNodiCiclo, NumeroArchiCiclo, Mindelta=1000,delta, minArco;
    SoluzioneCandidata[Id - 1].Selected = 1;    /*aggiungi l'arco al ciclo*/
    Nodi[SoluzioneCandidata[Id - 1].N1 - 1]++;  /*aumenta il grado dei nodi, in seguito all'aggiunta*/
    Nodi[SoluzioneCandidata[Id - 1].N2 - 1]++;
    NumeroNodiCiclo = individuaCiclo(SoluzioneCandidata, NodiCiclo, Nodi);  /*trova i nodi connessi da un ciclo*/
    NumeroArchiCiclo = trovaArchiCiclo(SoluzioneCandidata,NodiCiclo,NumeroNodiCiclo,IdArchiCiclo);
    for(int i=0; i<NumeroArchiCiclo;i++){
        //provo a rimuovere un arco alla volta e calcolo delta
        Nodi[SoluzioneCandidata[IdArchiCiclo[i]-1].N1 - 1]--;
        Nodi[SoluzioneCandidata[IdArchiCiclo[i]-1].N2 - 1]--;
        delta=SoluzioneCandidata[Id-1].Costo-SoluzioneCandidata[IdArchiCiclo[i]-1].Costo+vincoliInfranti(Nodi)*PENALIZZAZIONE;
        if(delta<Mindelta){
            Mindelta=delta;
            minArco=IdArchiCiclo[i];
        }
        //ripristino il ciclo prima di guardare gli altri delta
        Nodi[SoluzioneCandidata[IdArchiCiclo[i]-1].N1 - 1]++;
        Nodi[SoluzioneCandidata[IdArchiCiclo[i]-1].N2 - 1]++;
    }
    //Ottenuto il delta minore aggiorno la soluzione candidata
    SoluzioneCandidata[minArco-1].Selected=0;
    Nodi[SoluzioneCandidata[minArco-1].N1 - 1]--;
    Nodi[SoluzioneCandidata[minArco-1].N2 - 1]--;
    /*if(TabuOn)
        *IdNuovo = arcoDaRimuovereTabu(NodiCiclo, SoluzioneCandidata, NumeroNodiCiclo,TabuList,t);
    else *IdNuovo = arcoDaRimuovere(NodiCiclo, SoluzioneCandidata, NumeroNodiCiclo,TabuList,t);
    if(*IdNuovo==-1){ //Se non trovo nessun arco da rimuovere tolgo quello appena inserito
        SoluzioneCandidata[Id - 1].Selected = 0;
        Nodi[SoluzioneCandidata[Id - 1].N1 - 1]--;  //ripristina il grado dei nodi
        Nodi[SoluzioneCandidata[Id - 1].N2 - 1]--;
        *IdNuovo=Id;
    }else {
        SoluzioneCandidata[*IdNuovo - 1].Selected = 0;
        Nodi[SoluzioneCandidata[*IdNuovo - 1].N1 - 1]--;  //riduce il grado dei nodi, in seguito alla rimozione
        Nodi[SoluzioneCandidata[*IdNuovo - 1].N2 - 1]--;
    }*/
}

//CREA UNA LISTA DEI SOLI ID
void individuaId(arco* ListaArchi, int* ListaId) {
    for (int i = 0; i < NUMEROARCHI; i++) {
        ListaId[i] = ListaArchi[i].Id;
    }
}

//----------------------------DIVERSIFICAZIONE--------------------------------------------------------------------------------------------------------------

//IDEONA, Aggiungi prima un arco nel ciclo creato (che invividui con trovaCiclo) rimuovi un arco che non sia quello appena inserito e così in loop un numero N di volte.

int rimuoviArco(arco ListaArchi[],int NodiCiclo[], int NumeroNodiCiclo){
    int N1, N2, IdArco, i;
    permuta(NodiCiclo,NumeroNodiCiclo);
    i= rand() % NumeroNodiCiclo;
    N1=NodiCiclo[i];
    do {
        i = rand() % NumeroNodiCiclo;
        N2=NodiCiclo[i];
        if(N1!=N2)
            IdArco=trovaArco(N1,N2, ListaArchi);
    }while(N1==N2);
    return IdArco;
}

void modificaSoluzione(arco* ListaArchi, int* Nodi, int* ListaId){
    int NodiCiclo[NUMERONODI];
    int ArchiRimossi[ARCHIDIVERSIFICARE];
    int nNodi=0;
    int idArcoRemove;
    for(int i=0, j=0; i<NUMERONODI && j<ARCHIDIVERSIFICARE; i++){
        if(ListaArchi[ListaId[i]-1].Selected==0 && notInLista(ListaId[i], ArchiRimossi, i)){
            ListaArchi[ListaId[i]-1].Selected=1;
            Nodi[ListaArchi[ListaId[i]-1].N1-1]++;
            Nodi[ListaArchi[ListaId[i]-1].N2-1]++;
            nNodi=individuaCiclo(ListaArchi,NodiCiclo,Nodi);
            idArcoRemove=rimuoviArco(ListaArchi,NodiCiclo,nNodi);
            ListaArchi[idArcoRemove-1].Selected=0;
            Nodi[ListaArchi[idArcoRemove-1].N1-1]--;
            Nodi[ListaArchi[idArcoRemove-1].N2-1]--;
            ArchiRimossi[j]=idArcoRemove;
            j++;
        }
    }
}

void main() {
    int Nodi[NUMERONODI], NodiTemporanei[NUMERONODI],NodiTemporaneiTabu[NUMERONODI];
    int ListaId[NUMEROARCHI],TabuList[TABUSIZE]={0};
    arco ListaArchi[NUMEROARCHI], SoluzioneTemporanea[NUMEROARCHI],SoluzioneTemporaneaTabu[NUMEROARCHI],SoluzioneMigliore[NUMEROARCHI];
    int trovato, IdRim, IdAggiunto, IdRimosso;
    int CostoMiglioreAttuale, CostoAttuale, CostoPrecedente,CostoMiglioreAssoluto;    //il primo rappresenta in ogni ciclo il costo migliore fin'ora trovato per singolo intorno, il secondo rappresenta il costo della soluzione temporanea che si sta esaminando in quel momento, il terzo è il costo della soluzione migliore dell'intorno precedente, il quarto è il costo della miglior soluzione in assoluto trovata
    int VincoliInfrantiPrec=0,VincoliInfrantiOra=0;
    int scan = 0, i = 0, k = 0, t=0;
    int Stallo = 0, NoImprovement=0;
    int n, Repeat=0;
    int IdAggiuntoT=-1;
    int IdRimossoT=-2;
    IdAggiunto = -1;
    IdRimosso = -2;
    int TabuOn;

    //APRO FILE E LEGGO ISTANZE
    FILE* fd;
    fd=fopen("C:\\Users\\alice\\OneDrive\\Documents\\GitHub\\LocalSearch\\TabuSearch\\istanzeTest.txt", "r");
    //fd = fopen("C:\\Users\\alice\\OneDrive\\Documents\\GitHub\\LocalSearch\\LocalSearch\\istanze.txt", "r");
    //fd=fopen("C:\\Users\\Sara\\Documents\\GitHub\\LocalSearch\\LocalSearch\\istanze2.txt", "r");
    if (fd == NULL) {
        printf("Errore apertura file");
        exit(1);
    }

    while (scan != EOF) {
        if (i < NUMERONODI) {
            for (i = 0; i < NUMERONODI; i++) {
                scan = fscanf(fd, "%d", &Nodi[i]);
                /*if (Nodi[i] > KMASSIMO) { nella Tabu non ho vincoli sulla soluzione iniziale ma verrà penalizzata successivamente
                    printf("ERRORE: grado nodo %d maggiore di %d, istanza non ammissibile", i + 1, KMASSIMO);
                    exit(-1);
                }*/
            }
        }
        scan = fscanf(fd, "%d/%d/%d/%d/%d", &ListaArchi[i - NUMERONODI].Id, &ListaArchi[i - NUMERONODI].N1, &ListaArchi[i - NUMERONODI].N2, &ListaArchi[i - NUMERONODI].Costo, &ListaArchi[i - NUMERONODI].Selected);
        i++;
    }
    fclose(fd);

    printf("Soluzione iniziale:\n");
    stampaLista(ListaArchi);

    VincoliInfrantiPrec=vincoliInfranti(Nodi);    //guardo quanti sono i nodi nella soluzione iniziale che non rispettano il vincolo sul grado
    CostoPrecedente = calcolaCosto(ListaArchi)+VincoliInfrantiPrec*PENALIZZAZIONE; //costo della soluzione iniziale
    CostoMiglioreAssoluto=CostoPrecedente; //al primo giro, la miglior soluzione è l'iniziale quindi ne inizializzo il costo
    memcpy(SoluzioneMigliore,ListaArchi,sizeof(ListaArchi));    //la soluzione migliore viene inizializzata a quella iniziale

    individuaId(ListaArchi, ListaId);   //trovo la lista degli id degli archi per permutarla successivamente
    n = sizeof(ListaId);    //dato necessario per effettuare la permutazione

    while (!Stallo) {   //finchè non raggiungo lo stallo, continuo a cercare una soluzione migliore
        permuta(ListaId, n);
        CostoMiglioreAttuale=100000000; //valore fittizio per partire nel singolo intorno. A differenza della local search non può essere inizializzato al valore della soluzione iniziale perchè qui devo considerare solo l'intorno tranne la soluzione iniziale
        for (int j = 0; j < NUMEROARCHI; j++) {    //ciclo sugli archi, nell'ordine dato dalla ListaId permutata
            TabuOn=0;
            if (ListaArchi[ListaId[j] - 1].Selected == 0) {
                memcpy(SoluzioneTemporanea, ListaArchi, sizeof(ListaArchi));  //serve una copia perchè devo poter controllare arco per arco. La soluzione iniziale viene modificata solo dopo aver esplorato tutto l'intorno
                memcpy(NodiTemporanei, Nodi, sizeof(Nodi));
                localSearch(SoluzioneTemporanea, ListaId[j], NodiTemporanei, &IdRim, TabuList, t,
                            TabuOn);   //guarda solo mosse non tabu
                VincoliInfrantiOra = vincoliInfranti(NodiTemporanei); //vincoli infranti dalla nuova soluzione trovata
                if (ListaId[j] !=
                    IdRim) { //se la soluzione è diversa da quella corrente (ovvero se non ho aggiunto e tolto lo stesso arco), controlla se è la migliore dell'intorno
                    CostoAttuale = CostoPrecedente + SoluzioneTemporanea[ListaId[j] - 1].Costo -
                                   SoluzioneTemporanea[IdRim - 1].Costo + (VincoliInfrantiOra - VincoliInfrantiPrec) *
                                                                          PENALIZZAZIONE;  //il nuovo costo dopo l'inserimento di un arco e la rimozione di un altro è pari al costo della soluzione precedente + il costo dell'arco aggiunto - costo arco rimosso + le penalizzazioni aggiunte (se si infrangono nuovi vincoli) o rimosse (se vincoli prima infranti ora sono rispettati)
                    if (CostoAttuale < CostoMiglioreAttuale) {
                        trovato=1;
                        CostoMiglioreAttuale = CostoAttuale;
                        IdAggiunto = ListaId[j];
                        IdRimosso = IdRim;
                    }
                    if (CostoMiglioreAttuale < CostoAttuale) {
                        TabuOn = 1;
                        memcpy(SoluzioneTemporaneaTabu, ListaArchi,
                               sizeof(ListaArchi));  //serve una copia perchè devo poter controllare arco per arco. La soluzione iniziale viene modificata solo dopo aver esplorato tutto l'intorno
                        memcpy(NodiTemporaneiTabu, Nodi, sizeof(Nodi));
                        localSearch(SoluzioneTemporaneaTabu, ListaId[j], NodiTemporaneiTabu, &IdRim, TabuList, t,
                                    TabuOn);   //guarda solo mosse tabu
                        VincoliInfrantiOra = vincoliInfranti(
                                NodiTemporaneiTabu); //vincoli infranti dalla nuova soluzione trovata
                        if (ListaId[j] !=
                            IdRim) { //se la soluzione è diversa da quella corrente (ovvero se non ho aggiunto e tolto lo stesso arco), controlla se è la migliore dell'intorno

                            CostoAttuale = CostoPrecedente + SoluzioneTemporaneaTabu[ListaId[j] - 1].Costo -
                                           SoluzioneTemporaneaTabu[IdRim - 1].Costo +
                                           (VincoliInfrantiOra - VincoliInfrantiPrec) *
                                           PENALIZZAZIONE;  //il nuovo costo dopo l'inserimento di un arco e la rimozione di un altro è pari al costo della soluzione precedente + il costo dell'arco aggiunto - costo arco rimosso + le penalizzazioni aggiunte (se si infrangono nuovi vincoli) o rimosse (se vincoli prima infranti ora sono rispettati)
                            if (CostoAttuale < CostoMiglioreAttuale) {
                                CostoMiglioreAttuale = CostoAttuale;
                                IdAggiuntoT = ListaId[j];
                                IdRimossoT = IdRim;
                            }
                        }else TabuOn=0; //Se ho rimosso e inserito lo stesso arco non eseguo mosse Tabu
                    }
                }
            }
        }
        if(TabuOn==1 && CostoMiglioreAttuale<CostoMiglioreAssoluto){//Se ho un opzione tabu la considero solo se migliora la precedente
            IdAggiunto=IdAggiuntoT;
            IdRimosso=IdRimossoT;
        } else TabuOn=0;
        k++;    //mantiene il numero dell'iterazione corrente
        if(CostoMiglioreAttuale<CostoMiglioreAssoluto) //se il costo della soluzione appena trovata nell'intorno è minore della migliore trovata in assoluto
            NoImprovement = 0;    //lo azzero perchè ho appena trovato un miglioramento
        else {//altrimenti implemento fino a raggiungere stallo
            NoImprovement++;
        }
        if(NoImprovement==MAXITERAZIONI)
            Stallo=1;
        if (!Stallo) {   //aggiungo e tolgo la coppia di archi che mi permette di avere la soluzione migliore per quell'intorno, diversa dalla soluzione corrente (anche se peggiore di quest'ultima)
            if(t<TABUSIZE){ //aggiunta dell'arco appena aggiunto alla tabu list di archi da non rimuovere
                TabuList[t]=IdAggiunto;
            }else if(t==TABUSIZE){  //quando è piena, si riazzera così agisce FIFO
                t=0;
                TabuList[t]=IdAggiunto;
            }
            t++;
            if(trovato==1) {
                ListaArchi[IdAggiunto - 1].Selected = 1;
                ListaArchi[IdRimosso - 1].Selected = 0;
                Nodi[ListaArchi[IdAggiunto - 1].N1 - 1]++;
                Nodi[ListaArchi[IdAggiunto - 1].N2 - 1]++;
                Nodi[ListaArchi[IdRimosso - 1].N1 - 1]--;
                Nodi[ListaArchi[IdRimosso - 1].N2 - 1]--;
                trovato=0;
            }
            if(CostoMiglioreAttuale<CostoMiglioreAssoluto){
                CostoMiglioreAssoluto=CostoMiglioreAttuale;
                memcpy(SoluzioneMigliore,ListaArchi,sizeof(ListaArchi));//DEVO MEMORIZZARE LA SOLUZIONE MIGLIORE IN ASSOLUTO FIN'ORA TROVATA
            }
            CostoPrecedente=CostoMiglioreAttuale;   //siccome la modifica trovata diventa definitiva, aggiorno i costi
            VincoliInfrantiPrec=VincoliInfrantiOra;
            if(NoImprovement==0) {
                if(TabuOn)
                    printf("Criterio di aspirazione: accettata soluzione Tabu\n");
                printf("Iterazione %d:\nAggiunto arco %d e rimosso arco %d.\n", k, IdAggiunto, IdRimosso);
                stampaLista(ListaArchi);
                printf("Numero di iterazioni senza miglioramenti: %d\n\n", NoImprovement);
            }else {
                printf("Iterazione %d:non ha trovato migioramenti\n", k);
                stampaLista(ListaArchi);
                printf("Numero di iterazioni senza miglioramenti: %d\n\n", NoImprovement);
            }
        }
        if (Stallo) {
            if (Repeat < MAXITERAZIONI) { //se sono in stallo modifico in maniera casuale la soluzione temporanea e ricomincio
                //memcpy(ListaArchi, SoluzioneTemporanea,sizeof(ListaArchi));  //sovrascrivo la soluzione iniziale con quella in cui mi trovo e modifico
                //memcpy(Nodi,NodiTemporanei, sizeof(Nodi));
                modificaSoluzione(ListaArchi,Nodi,ListaId);
                NoImprovement = 0;
                Stallo=0;
                Repeat++;
                printf("Ho raggiunto sluzione di stallo... diversifico\n");
                stampaLista(ListaArchi);
            } else {
                printf("All'iterazione %d e' stata raggiunta la situazione di stallo.\nL'ottimo locale trovato risulta quindi:\n",
                       k);
                stampaLista(SoluzioneMigliore);    //deve stampare la migliore
            }
        }
    }

}