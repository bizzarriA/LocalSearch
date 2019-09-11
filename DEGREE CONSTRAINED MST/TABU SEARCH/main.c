#include <stdio.h>
#include <mem.h>
#include <stdlib.h>
#include <time.h>

#define NUMERONODI 5
#define NUMEROARCHI 10
#define KMASSIMO 3              //grado massimo dei nodi
#define TABUSIZE 3              //dimensione massima lista Tabu
#define ARCHIDIVERSIFICARE 3    //numero archi da modificare durante diversificazione
#define MAXITERAZIONI 3         //numero di iterazioni di seguito in cui non vi sono miglioramenti
#define DIVERSIFICAZIONI 3      //numero massimo di diversificazioni
#define PENALIZZAZIONE 100      //valore di cui viene penalizzata l'accettazione di alberi non ammissibili

typedef struct s_arco {
    int Id;
    int N1;
    int N2;
    int Costo;
    int Selected;
}arco;

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
//Idea: rimuovo nodi con grado 1 e aggiorno i gradi dei nodi rimanenti, itero fino a che non ho più nodi di grado 1
//      i nodi rimanenti sono quelli che appartengono al ciclo
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
}

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
    int IdArco=-1, N;
    if(N1>N2){
        N=N1;
        N1=N2;
        N2=N;
    }
    for (int i = 0; i < NUMEROARCHI; i++) {
        if (SoluzioneCandidata[i].N1 == N1 && SoluzioneCandidata[i].N2 == N2 && SoluzioneCandidata[i].Selected==1) {
            IdArco = SoluzioneCandidata[i].Id;
            break;
        }
    }
    return IdArco;
}

//VERIFICO SE L'ARCO PASSATO è O NO IN UNA LISTA, 0 SE PRESENTE 1 SE ASSENTE
int notInLista(int Id,int* Lista,int Length){   //ritorna 1 se l'elemento id non appartiene all'array Lista
    for(int i=0;i<Length;i++){
        if(Lista[i]==Id)
            return 0;
    }
    return 1;
}

//VERIFICO SE L'ARCO PASSATO è O NO IN UNA LISTA, 1 SE PRESENTE 0 SE ASSENTE
int inLista(int Id,int* Lista,int Length){   //ritorna 1 se l'elemento id appartiene all'array Lista
    for(int i=0;i<Length;i++){
        if(Lista[i]==Id)
            return 1;
    }
    return 0;
}

//DATI I NODI DI UN CICLO INDIVIDUO GLI ARCHI DEL CICLO STESSO
int trovaArchiCiclo(arco ListaArchi[], int NodiCiclo[], int NumeroNodiCiclo, int IdArchiCiclo[]){
    int N1, N2, Id, k=0;
    for(int i=0; i<NumeroNodiCiclo-1; i++){
        N1=NodiCiclo[i];
        for(int j=i+1; j<NumeroNodiCiclo; j++){
            N2=NodiCiclo[j];
            Id=trovaArco(N1,N2,ListaArchi);
            if(Id!=-1) {
                IdArchiCiclo[k] = Id;
                k++;
            }
        }
    }
    return k;
}

//ESEGUO LOCAL SEARCH CON LIMITE GRADO NODI <= K
void localSearch(arco* SoluzioneCandidata, int Id, int Nodi[], int* IdNuovo,int* TabuList,int t, int TabuOn) {  //ritorna anche l'arco che ha appena rimosso
    int NodiCiclo[NUMERONODI], IdArchiCiclo[NUMEROARCHI];
    int NumeroNodiCiclo, NumeroArchiCiclo, Mindelta=1000,delta, minArco=-1;
    SoluzioneCandidata[Id - 1].Selected = 1;    /*aggiungi l'arco al ciclo*/
    Nodi[SoluzioneCandidata[Id - 1].N1 - 1]++;  /*aumenta il grado dei nodi, in seguito all'aggiunta*/
    Nodi[SoluzioneCandidata[Id - 1].N2 - 1]++;
    NumeroNodiCiclo = individuaCiclo(SoluzioneCandidata, NodiCiclo, Nodi);  /*trova i nodi connessi da un ciclo*/
    NumeroArchiCiclo = trovaArchiCiclo(SoluzioneCandidata,NodiCiclo,NumeroNodiCiclo,IdArchiCiclo);
    if(!TabuOn) {   //se la localsearch sta guardando solo le mosse non tabu dell'intorno
        for (int i = 0; i < NumeroArchiCiclo; i++) {
            //provo a rimuovere un arco alla volta e calcolo delta
            //se l'arco rimosso non è tabu e se non è lo stesso appena aggiunto eseguo il calcolo
            if (notInLista(IdArchiCiclo[i], TabuList, t) && IdArchiCiclo[i] != Id) {
                Nodi[SoluzioneCandidata[IdArchiCiclo[i] - 1].N1 - 1]--;
                Nodi[SoluzioneCandidata[IdArchiCiclo[i] - 1].N2 - 1]--;
                delta = SoluzioneCandidata[Id - 1].Costo - SoluzioneCandidata[IdArchiCiclo[i] - 1].Costo +
                        vincoliInfranti(Nodi) * PENALIZZAZIONE;
                //se ho un delta migliore di quelli trovati fino ad ora aggiorno l'arco
                if (delta < Mindelta) {
                    Mindelta = delta;
                    minArco = IdArchiCiclo[i];
                }
                //ripristino il ciclo prima di guardare gli altri delta
                Nodi[SoluzioneCandidata[IdArchiCiclo[i] - 1].N1 - 1]++;
                Nodi[SoluzioneCandidata[IdArchiCiclo[i] - 1].N2 - 1]++;
            }
        }
        //Ottenuto il delta minore aggiorno la soluzione candidata
        if(minArco!=-1) {
            SoluzioneCandidata[minArco - 1].Selected = 0;
            Nodi[SoluzioneCandidata[minArco - 1].N1 - 1]--;
            Nodi[SoluzioneCandidata[minArco - 1].N2 - 1]--;
        }

    }else{  //se la local search sta guardando le mosse tabu
        for (int i = 0; i < NumeroArchiCiclo; i++) {
            //provo a rimuovere un arco alla volta e calcolo delta
            //se l'arco rimosso è tabu e se non è lo stesso appena aggiunto eseguo il calcolo
            if (inLista(IdArchiCiclo[i], TabuList, t) && IdArchiCiclo[i] != Id) {
                Nodi[SoluzioneCandidata[IdArchiCiclo[i] - 1].N1 - 1]--;
                Nodi[SoluzioneCandidata[IdArchiCiclo[i] - 1].N2 - 1]--;
                delta = SoluzioneCandidata[Id - 1].Costo - SoluzioneCandidata[IdArchiCiclo[i] - 1].Costo +
                        vincoliInfranti(Nodi) * PENALIZZAZIONE;
                //se ho un delta migliore di quelli trovati fino ad ora aggiorno l'arco
                if (delta < Mindelta) {
                    Mindelta = delta;
                    minArco = IdArchiCiclo[i];
                }
                //ripristino il ciclo prima di guardare gli altri delta
                Nodi[SoluzioneCandidata[IdArchiCiclo[i] - 1].N1 - 1]++;
                Nodi[SoluzioneCandidata[IdArchiCiclo[i] - 1].N2 - 1]++;
            }
        }
            //Ottenuto il delta minore aggiorno la soluzione candidata
            if(minArco!=-1) {
                SoluzioneCandidata[minArco - 1].Selected = 0;
                Nodi[SoluzioneCandidata[minArco - 1].N1 - 1]--;
                Nodi[SoluzioneCandidata[minArco - 1].N2 - 1]--;
            }
    }
    //restituisco l'id del miglior arco trovato nell'intorno
    *IdNuovo=minArco;
}

//CREA UNA LISTA DEI SOLI ID
void individuaId(arco* ListaArchi, int* ListaId) {
    for (int i = 0; i < NUMEROARCHI; i++) {
        ListaId[i] = ListaArchi[i].Id;
    }
}

//----------------------------DIVERSIFICAZIONE--------------------------------------------------------------------------------------------------------------

//idea generele, aggiungo un arco casuale e ne rimuovo un altro nel ciclo che si viene a creare, così per N volte (N stabilito in define).

//RIMUOVO ARCO CASUALE DA CICLO
int rimuoviArco(arco ListaArchi[],int NodiCiclo[], int NumeroNodiCiclo){
    int N1, N2, IdArco=-1, i;
    //permuto la lista dei nodi del ciclo e ne scelgo uno casuale
    permuta(NodiCiclo,NumeroNodiCiclo);
    i= rand() % NumeroNodiCiclo;
    N1=NodiCiclo[i];
    //cerco finchè non trovo un altro nodo del ciclo che è collegato con un arco selezionato a N1
    do {
        i = rand() % NumeroNodiCiclo;
        N2=NodiCiclo[i];
        if(N1!=N2)
            IdArco=trovaArco(N1,N2, ListaArchi);
        if(ListaArchi[IdArco -1].Selected==0){
            IdArco=-1;
        }
    }while(N1==N2 || IdArco==-1);
    //restituisco l'id dell'arco da rimuovere dal ciclo
    return IdArco;
}

//AGGIUNGO ARCO CASUALE AL GRAFO E RIMUOVO ALTRO NODO DAL CICLO FORMATOSI
void modificaSoluzione(arco* ListaArchi, int* Nodi, int* ListaId){
    int NodiCiclo[NUMERONODI];
    int ArchiRimossi[ARCHIDIVERSIFICARE]={0};   //lo inizializza per essere sicura che, riempiendolo con numeri a caso, non metta Id esistenti
    int nNodi=0;
    int idArcoRemove;
    for(int i=0, j=0; i<NUMERONODI && j<ARCHIDIVERSIFICARE; i++){
        //Seleziono un arco a caso non selezionato e non fra quelli già rimossi dalla diversificazione e lo aggiungo
        if(ListaArchi[ListaId[i]-1].Selected==0 && notInLista(ListaId[i], ArchiRimossi, i)){
            ListaArchi[ListaId[i]-1].Selected=1;
            Nodi[ListaArchi[ListaId[i]-1].N1-1]++;
            Nodi[ListaArchi[ListaId[i]-1].N2-1]++;
            memset(NodiCiclo,0,sizeof(NodiCiclo));  //pulisco l'array
            //individuo il ciclo creatosi
            nNodi=individuaCiclo(ListaArchi,NodiCiclo,Nodi);
            //cerco l'arco da rimuovere fra quelli del ciclo e aggiorno il grafo
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
    int IdRim, IdAggiunto=-1, IdRimosso=-2,IdAggiuntoT=-1, IdRimossoT=-2;
    int trovato=0, TabuOn=0, cicloTabu=0, solTabu=0;
    int poche=1; //poche serve per sistemare le stampe a seconda che l'istanza sia piccola o grande
    int VincoliInfrantiPrec=0,VincoliInfrantiOra=0,VincoliInfrantiSolMigliore=0;
    int scan = 0, i = 0, k = 0, t=0, n;
    int Stallo = 0, NoImprovement=0, Repeat=0;
    int CostoMiglioreAttuale, CostoAttuale, CostoPrecedente,CostoMiglioreAssoluto;
    /* il primo rappresenta in ogni ciclo il costo migliore fin'ora trovato per singolo intorno,
     * il secondo rappresenta il costo della soluzione temporanea che si sta esaminando in quel momento,
     * il terzo è il costo della soluzione migliore dell'intorno precedente,
     * il quarto è il costo della miglior soluzione in assoluto trovata    */


    //APRO FILE E LEGGO ISTANZE
    FILE* fd;
    //fd = fopen("C:\\Users\\Sara\\Documents\\GitHub\\LocalSearch\\DEGREE CONSTRAINED MST\\TABU SEARCH\\100nodi.txt", "r");
    //fd = fopen("C:\\Users\\Sara\\Documents\\GitHub\\LocalSearch\\DEGREE CONSTRAINED MST\\TABU SEARCH\\50nodi.txt", "r");
    fd=fopen("C:\\Users\\Sara\\Documents\\GitHub\\LocalSearch\\DEGREE CONSTRAINED MST\\TABU SEARCH\\5nodi.txt", "r");
    //fd=fopen("C:\\Users\\Sara\\Documents\\GitHub\\LocalSearch\\DEGREE CONSTRAINED MST\\TABU SEARCH\\4nodi.txt", "r");
    if (fd == NULL) {
        printf("Errore apertura file");
        exit(1);
    }

    while (scan != EOF) {
        if (i < NUMERONODI) {
            for (i = 0; i < NUMERONODI; i++) {
                scan = fscanf(fd, "%d", &Nodi[i]);
            }
        }
        scan = fscanf(fd, "%d/%d/%d/%d/%d", &ListaArchi[i - NUMERONODI].Id, &ListaArchi[i - NUMERONODI].N1, &ListaArchi[i - NUMERONODI].N2, &ListaArchi[i - NUMERONODI].Costo, &ListaArchi[i - NUMERONODI].Selected);
        i++;
    }
    fclose(fd);
    //guardo quanti sono i nodi nella soluzione iniziale che non rispettano il vincolo sul grado e stampo soluzione inziale con rispettivo costo
    VincoliInfrantiPrec=vincoliInfranti(Nodi);
    printf("Soluzione iniziale:\n");
    stampaLista(ListaArchi);

    //setto i costi iniziali
    //Calcolo costo aggiungendo penalizzazioni dovute a non ammissibilità
    CostoPrecedente = calcolaCosto(ListaArchi)+VincoliInfrantiPrec*PENALIZZAZIONE; //costo della soluzione iniziale
    printf("Costo spanning tree: %d\n",CostoPrecedente);
    CostoMiglioreAssoluto=CostoPrecedente; //al primo giro, la miglior soluzione è l'iniziale quindi ne inizializzo il costo
    memcpy(SoluzioneMigliore,ListaArchi,sizeof(ListaArchi));    //la soluzione migliore viene inizializzata a quella iniziale

    individuaId(ListaArchi, ListaId);   //trovo la lista degli id degli archi per permutarla successivamente
    n = sizeof(ListaId);    //dato necessario per effettuare la permutazione

    while (!Stallo) {   //finchè non raggiungo lo stallo, continuo a cercare una soluzione migliore
        permuta(ListaId, n);
        //Calcolo costo aggiungendo penalizzazioni dovute a non ammissibilità
        CostoPrecedente=calcolaCosto(ListaArchi)+VincoliInfrantiPrec*PENALIZZAZIONE;
        CostoMiglioreAttuale=100000000;
        /* valore fittizio per partire nel singolo intorno.
         * A differenza della local search non può essere inizializzato al valore della soluzione iniziale
         * perchè qui devo considerare solo l'intorno tranne la soluzione iniziale*/
        printf("\nITERAZIONE %d\nEsploro l'intorno\n",k+1);
        for (int j = 0; j < NUMEROARCHI; j++) {    //ciclo sugli archi, nell'ordine dato dalla ListaId permutata
            TabuOn = 0;
            if (ListaArchi[ListaId[j] - 1].Selected == 0) {
                memcpy(SoluzioneTemporanea, ListaArchi, sizeof(ListaArchi));
                /* Serve una copia perchè devo poter controllare arco per arco.
                 * La soluzione iniziale viene modificata solo dopo aver esplorato tutto l'intorno*/
                memcpy(NodiTemporanei, Nodi, sizeof(Nodi));
                //Guardo solo mosse non tabu e restituisco IdRim=-1 se non ci sono mosse non tabu.
                localSearch(SoluzioneTemporanea, ListaId[j], NodiTemporanei, &IdRim, TabuList, t, TabuOn);
                VincoliInfrantiOra = vincoliInfranti(NodiTemporanei); //vincoli infranti dalla nuova soluzione trovata
                if (IdRim != -1) {
                    //Se ha trovato un arco non tabu da rimuovere, controlla se la soluzione creatasi così è la migliore dell'intorno.
                    //Calcolo nuovo costo
                    CostoAttuale =
                            CostoPrecedente                                          //costo della soluzione precedente
                            + SoluzioneTemporanea[ListaId[j] -1].Costo                     //+ il costo dell'arco aggiunto
                            - SoluzioneTemporanea[IdRim - 1].Costo                          //- costo arco rimosso
                            +(VincoliInfrantiOra - VincoliInfrantiPrec) * PENALIZZAZIONE;  //+ le penalizzazioni aggiunte
                    //Stampo per ogni possibile soluzione arco IN e arco OUT e relativo costo
                    if(poche==1)
                        printf("IN:%d\tOUT:%d\tCOSTO ATTUALE:%d\n", ListaId[j], IdRim, CostoAttuale);
                    //Se è migliorativo nell'intorno mi salvo la soluzione e il suo costo
                    if (CostoAttuale < CostoMiglioreAttuale) {
                        trovato = 1;
                        CostoMiglioreAttuale = CostoAttuale;
                        IdAggiunto = ListaId[j];
                        IdRimosso = IdRim;
                        solTabu = 0;
                    }
                }

            }
        }
        //cerco fra le soluzioni tabu per criterio aspirazione solo se non ho trovato soluzioni migliori
        if (IdRim == -1) //Se non ho trovato sluzione non Tabu prendo per forza fra una soluzione Tabu.
            cicloTabu = 1;
        if (IdRim == -1 || CostoMiglioreAttuale <= CostoAttuale) {
            for (int j = 0; j < NUMEROARCHI; j++) {
                //se non esistono mosse non tabu o se esistevano ma non erano migliorative, controlla le tabu
                TabuOn = 1;
                //serve una copia perchè devo poter controllare arco per arco. La soluzione iniziale viene modificata solo dopo aver esplorato tutto l'intorno
                if (ListaArchi[ListaId[j] - 1].Selected == 0) {
                    memcpy(SoluzioneTemporaneaTabu, ListaArchi, sizeof(ListaArchi));
                    memcpy(NodiTemporaneiTabu, Nodi, sizeof(Nodi));
                    if (t > 0) { //se la lista tabu è vuota, non ha senso fare una local search
                        localSearch(SoluzioneTemporaneaTabu, ListaId[j], NodiTemporaneiTabu, &IdRim, TabuList, t,
                                    TabuOn);//guarda solo mosse tabu
                        VincoliInfrantiOra = vincoliInfranti(
                                NodiTemporaneiTabu); //vincoli infranti dalla nuova soluzione trovata
                    } else
                        IdRim = -1;
                    if (IdRim != -1) { //se esistono mosse tabu, controlla se la soluzione così generata è la migliore dell'intorno
                        CostoAttuale = CostoPrecedente +
                                       SoluzioneTemporaneaTabu[ListaId[j] - 1].Costo -
                                       SoluzioneTemporaneaTabu[IdRim - 1].Costo +
                                       (VincoliInfrantiOra - VincoliInfrantiPrec) * PENALIZZAZIONE;
                        //Stampo per ogni possibile soluzione arco IN e arco OUT e relativo costo
                        if(poche==1)
                            printf("IN:%d\tOUT:%d\tCOSTO ATTUALE:%d\tTABU\n", ListaId[j], IdRim, CostoAttuale);
                        //Se il costo della TABU è migliorativo o se non ho altra soluzione possibile aggiorno costi e soluzione.
                        if (CostoAttuale < CostoMiglioreAttuale || cicloTabu == 1) {
                            CostoMiglioreAttuale = CostoAttuale;
                            IdAggiuntoT = ListaId[j];
                            IdRimossoT = IdRim;
                            solTabu = 1;
                        }
                    } else TabuOn = 0; //Se non ho mosse tabu da eseguire, allora esco dallo stato tabu
                }
            }
        }
        //Se ho un opzione tabu la considero solo se migliora la precedente
        if(solTabu==1 && CostoMiglioreAttuale<CostoMiglioreAssoluto){
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
        //Se non sono in stallo, aggiungo e tolgo la coppia di archi che mi permette di avere la soluzione migliore per quell'intorno,
        //diversa dalla soluzione corrente (anche se peggiore di quest'ultima)
        if (!Stallo) {
            if(t<TABUSIZE){ //aggiunta dell'arco appena aggiunto alla tabu list di archi da non rimuovere
                TabuList[t]=IdAggiunto;
            }else if(t==TABUSIZE){  //quando è piena, si riazzera così agisce FIFO
                t=0;
                TabuList[t]=IdAggiunto;
            }
            t++;
            //Se ho trovato una soluzione mi sposto su di essa
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
                VincoliInfrantiSolMigliore=vincoliInfranti(Nodi);
            }
            //tengo traccia dei vincoli infranti
            VincoliInfrantiPrec=VincoliInfrantiOra;
            //eseguo le varie stampe
            if(NoImprovement==0) {
                if(TabuOn)
                    printf("\nCriterio di aspirazione: accettata soluzione Tabu\n");
                printf("Soluzione iterazione %d:\nAggiunto arco %d e rimosso arco %d.\n", k, IdAggiunto, IdRimosso);
                stampaLista(ListaArchi);
                printf("COSTO MIGLIORE ASSOLUTO: %d\nCOSTO MIGLIORE ATTUALE: %d\n",CostoMiglioreAssoluto,CostoMiglioreAttuale);
                printf("Numero di iterazioni senza miglioramenti: %d\n\n", NoImprovement);
            }else {
                printf("Iterazione %d: non ha trovato miglioramenti\n", k);
                printf("Mi sposto comunque alla soluzione migliore dell'intorno:\n");
                if(poche==1)
                    stampaLista(ListaArchi);
                printf("Numero di iterazioni senza miglioramenti: %d\n", NoImprovement);
                printf("COSTO MIGLIORE ASSOLUTO: %d\n",CostoMiglioreAssoluto);
            }
        }
        //Se sono in situazione di stallo eseguo diversificazione fino ad un MAXITERAZIONI
        if (Stallo) {
            if (Repeat < DIVERSIFICAZIONI) { //se sono in stallo modifico in maniera casuale la soluzione temporanea e ricomincio
                modificaSoluzione(ListaArchi,Nodi,ListaId);
                NoImprovement = 0;
                Stallo=0;
                Repeat++;
                t=0; //svuoto la lista tabu
                printf("\nHo raggiunto una soluzione di stallo...\nDiversifico...\n\n");
                VincoliInfrantiPrec=vincoliInfranti(Nodi);
                CostoPrecedente=calcolaCosto(ListaArchi)+VincoliInfrantiPrec*PENALIZZAZIONE;
                stampaLista(ListaArchi);
                printf("Costo spanning tree: %d\n",CostoPrecedente);
            } else {
                printf("\nAll'iterazione %d e' stata raggiunta la situazione di stallo.\nL'ottimo locale trovato risulta quindi:\n",
                       k);
                stampaLista(SoluzioneMigliore);    //deve stampare la migliore
                printf("Costo spanning tree: %d\n",CostoMiglioreAssoluto);
            }
        }
    }

}