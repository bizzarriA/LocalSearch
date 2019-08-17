#include <stdio.h>
#include <mem.h>
#include <stdlib.h>
#include <time.h>

#define NUMERONODI 7
#define NUMEROARCHI 21
#define KMASSIMO 3
#define TABUSIZE 3
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
void stampaLista(arco* ListaArchi) {
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
void localSearch(arco* SoluzioneCandidata, int Id, int Nodi[], int* IdNuovo) {  //ritorna anche l'arco che ha appena rimosso
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

void main() {
    int Nodi[NUMERONODI], NodiTemporanei[NUMERONODI];
    int ListaId[NUMEROARCHI];
    arco ListaArchi[NUMEROARCHI], SoluzioneTemporanea[NUMEROARCHI],TabuList[TABUSIZE];
    int IdArcoMigliore, IdRim, IdAggiunto, IdRimosso;
    int CostoMiglioreAttuale, CostoAttuale, CostoPrecedente;
    int VincoliInfrantiPrec=0,VincoliInfrantiOra=0;
    int scan = 0, i = 0, k = 0;
    int Stallo = 0, NoImprovement=0;
    int n;

    //APRO FILE E LEGGO ISTANZE
    FILE* fd;
    //fd = fopen("C:\\Users\\alice\\OneDrive\\Documents\\GitHub\\LocalSearch\\LocalSearch\\istanze2.txt", "r");
    fd=fopen("C:\\Users\\Sara\\Documents\\GitHub\\LocalSearch\\LocalSearch\\istanze2.txt", "r");
    if (fd == NULL) {
        printf("Errore apertura file");
        exit(1);
    }

    while (scan != EOF) {
        if (i < NUMERONODI) {
            for (i = 0; i < NUMERONODI; i++) {
                scan = fscanf(fd, "%d", &Nodi[i]);
                if (Nodi[i] > KMASSIMO) {
                    printf("ERRORE: grado nodo %d maggiore di %d, istanza non ammissibile", i + 1, KMASSIMO);
                    exit(-1);
                }
            }
        }
        scan = fscanf(fd, "%d/%d/%d/%d/%d", &ListaArchi[i - NUMERONODI].Id, &ListaArchi[i - NUMERONODI].N1, &ListaArchi[i - NUMERONODI].N2, &ListaArchi[i - NUMERONODI].Costo, &ListaArchi[i - NUMERONODI].Selected);
        i++;
    }
    fclose(fd);

    printf("Soluzione iniziale:\n");
    stampaLista(ListaArchi);

    VincoliInfrantiPrec=vincoliInfranti(Nodi);    //guardo quanti sono i nodi che non rispettano il vincolo sul grado
    CostoPrecedente = calcolaCosto(ListaArchi)+VincoliInfrantiPrec*PENALIZZAZIONE; //costo della soluzione iniziale

    individuaId(ListaArchi, ListaId);
    n = sizeof(ListaId);

    while (!Stallo) {   //finchè non raggiungo lo stallo, continuo a cercare una soluzione migliore
        permuta(ListaId, n);
        CostoMiglioreAttuale=100000; //valore fittizio per partire. A differenza della local search non può essere inizializzato al valore della soluzione iniziale perchè qui devo considerare solo l'intorno tranne la soluzione iniziale
        Stallo = 1;
        NoImprovement++;
        IdAggiunto = -1;
        IdRimosso = -2;
        for (int j = 0; j <= NUMEROARCHI; j++) {
            if (ListaArchi[ListaId[j] - 1].Selected == 0) {
                memcpy(SoluzioneTemporanea, ListaArchi, sizeof(ListaArchi));  //serve una copia perchè devo poter controllare arco per arco. La soluzione iniziale viene modificata solo dopo aver esplorato tutto l'intorno
                memcpy(NodiTemporanei, Nodi, sizeof(Nodi));
                localSearch(SoluzioneTemporanea, ListaId[j], NodiTemporanei, &IdRim);
                VincoliInfrantiOra=vincoliInfranti(NodiTemporanei);
                if (ListaId[j] != IdRim) { //se la soluzione è diversa da quella corrente, controlla se è la migliore dell'intorno
                    CostoAttuale = CostoPrecedente+SoluzioneTemporanea[ListaId[j] - 1].Costo+SoluzioneTemporanea[IdRim - 1].Costo+(VincoliInfrantiOra-VincoliInfrantiPrec)*PENALIZZAZIONE;  //il nuovo costo dopo l'inserimento di un arco e la rimozione di un altro è pari al costo precedente + il costo dell'arco aggiunto - costo arco rimosso + le penalizzazioni aggiunte (se si infrangono nuovi vincoli) o rimosse (se vincoli prima infranti ora sono rispettati)
                    if (CostoAttuale < CostoMiglioreAttuale) {
                        CostoMiglioreAttuale = CostoAttuale;
                        IdAggiunto = ListaId[j];
                        IdRimosso = IdRim;
                        NoImprovement=0;
                    }
                }
            }
        }
        k++;
        if(NoImprovement<MAXITERAZIONI)
            Stallo=0;
        if (!Stallo) {   //aggiungo e tolgo la coppia di archi che mi permette di avere la soluzione migliore per quell'intorno, diversa dalla soluzione corrente (anche se peggiore di quest'ultima)
            ListaArchi[IdAggiunto - 1].Selected = 1;
            ListaArchi[IdRimosso - 1].Selected = 0;
            Nodi[ListaArchi[IdAggiunto - 1].N1 - 1]++;
            Nodi[ListaArchi[IdAggiunto - 1].N2 - 1]++;
            Nodi[ListaArchi[IdRimosso - 1].N1 - 1]--;
            Nodi[ListaArchi[IdRimosso - 1].N2 - 1]--;
            CostoPrecedente=CostoMiglioreAttuale;
            VincoliInfrantiPrec=VincoliInfrantiOra;
            printf("Iterazione %d:\nAggiunto arco %d e rimosso arco %d.\n", k, IdAggiunto, IdRimosso);
            stampaLista(ListaArchi);
        }
        if (Stallo) {
            printf("All'iterazione %d non e' stata trovata soluzione migliore.\nL'ottimo locale trovato risulta quindi:\n", k);
            stampaLista(ListaArchi);
        }
    }

}