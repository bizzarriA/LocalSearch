#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include <time.h>

#define NUMEROARCHI 10000
#define NUMERONODI 10000

/*-----------GREEDY COSTRUTTIVA----------------------------------------------------------------------------------------------------------------------------------*/
typedef struct s_arco{
    int Id;
    int N1;
    int N2;
    int Costo;
    int Selected;
    int Ammissibile;    //serve SOLO nella greedy e se posto a 0 significa che l'arco è già stato individuato come non accettabile (per grado o percorso creato) e non deve essere ricontrollato. Nel file deve essere =1
}arco;

int allNodes(int* NodiAttuali,int n){ //ritorna 1 se tutti i nodi sono stati presi (ovvero se nessuno ha grado 0)
    for(int i=0;i<n;i++){
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

int gradoEccessivo(arco ArcoAttuale, int* NodiAttuali,int GradoMassimo){ //ritorna 1 se almeno uno dei nodi in cui incide il nuovo arco ha già raggiunto il grado massimo consentito
    if(NodiAttuali[ArcoAttuale.N1-1]==GradoMassimo || NodiAttuali[ArcoAttuale.N2-1]==GradoMassimo)
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

void greedyCostruttiva(arco* SoluzioneAttuale, int* NodiAttuali, int Numero_nodi,int* IdArchi, int Numero_archi,int Grado_massimo){
    int i,IdArco;
    int j=0;
    int PrimoGiro=1;    //al posto della funzione che ogni volta controlla i gradi dei nodi per vedere se almeno uno è stato preso, setto una semplice variabile
    int DaRicontrollare[NUMEROARCHI]={0}; //contiene gli Id di archi che non potevano essere presi al momento perchè non adiacenti ad altri
    while(!allNodes(NodiAttuali,Numero_nodi)) {
        srand ( time(NULL) );
        i=rand() % (Numero_archi);
        i=rand() % (Numero_archi);
        IdArco=IdArchi[i];
        if(SoluzioneAttuale[IdArco-1].Selected!=1 && (is_percorso(SoluzioneAttuale[IdArco-1],NodiAttuali) || gradoEccessivo(SoluzioneAttuale[IdArco-1],NodiAttuali,Grado_massimo))) {
            SoluzioneAttuale[IdArco-1].Ammissibile=0;
        }
        else if(SoluzioneAttuale[IdArco-1].Selected!=1 && (PrimoGiro || almenoUnNodo(SoluzioneAttuale[IdArco-1],NodiAttuali))){
            PrimoGiro=0;
            SoluzioneAttuale[IdArco-1].Selected=1;
            NodiAttuali[SoluzioneAttuale[IdArco-1].N1-1]++;
            NodiAttuali[SoluzioneAttuale[IdArco-1].N2-1]++;
            memset(DaRicontrollare,0, (j+1)*sizeof(int));   //svuota la lista dei nodi che non potevano essere presi perchè non adiacenti
            j=0;
        }
        else if(SoluzioneAttuale[IdArco-1].Selected!=1){
            DaRicontrollare[j]=SoluzioneAttuale[IdArco-1].Id;
            j++;
        }
    }
}

void individuaId(arco* ListaArchi, int* ListaId) {
    for (int i = 0; i < NUMEROARCHI; i++) {
        ListaId[i] = ListaArchi[i].Id;
    }
}
/*------------FINE---------------------------------------------------------------------------------------------------------------------------------------------------*/
void main() {
    char c;
    int n,g,i,j,scan,count=1,costo;
    arco ListaArchi[NUMEROARCHI];
    int ListaId[NUMEROARCHI];
    int ListaNodi[NUMERONODI]={0};

  printf("Per quale tipo di ricerca si vuole generare l'istanza?\nG: greedy\nL: local search\nR: grasp\nT: tabu\n");
    scanf("%c",&c);
    printf("Quanti nodi deve avere il grafo?\n");
    scanf("%d",&n);
    printf("Qual e' il grado massimo consentito per ciascun nodo?\n");
    scanf("%d",&g);

    FILE *fp;
    FILE *fd;

    if(c=='G'){
        fp=fopen("C:\\Users\\Alice\\OneDrive\\Documents\\GitHub\\LocalSearch\\CreaIstanze\\nuova_istanza.txt", "w");
        //fp=fopen("C:\\Users\\Sara\\Documents\\GitHub\\LocalSearch\\CreaIstanze\\nuova_istanza.pl","w");
        if(fp==NULL){
            printf("Errore apertura file");
            exit(1);
        }
        fprintf(fp,"nodi(%d).\n",n);
        fprintf(fp,"grado(%d).\n",g);
        for(i=1;i<n;i++){
            for(j=i+1;j<=n;j++){
                costo=rand()%100;
                fprintf(fp,"arco(%d,%d,%d,%d).\n",count,i,j,costo);
                count++;
            }
        }
    }else if(c=='L'){
        //creo un file con archi, ma nessuno è preso
        //fp=fopen("C:\\Users\\Alice\\OneDrive\\Documents\\GitHub\\LocalSearch\\CreaIstanze\\nuova_istanza.txt", "w");
        fp=fopen("C:\\Users\\Sara\\Documents\\GitHub\\LocalSearch\\CreaIstanze\\nuova_istanza.txt","w");
        if(fp==NULL){
            printf("Errore apertura file");
            exit(1);
        }
        for (i = 1; i < n; i++) {
            for (j = i + 1; j <= n; j++) {
                costo = rand() % 100;
                fprintf(fp, "%d/%d/%d/%d\n", count, i, j, costo);
                count++;
            }
        }
        //chiudo file per salvare inserimento
        fclose(fp);
        //apro il file per leggere gli archi e darli in pasto alla greedy che calcolerà la soluzione iniziale
        fd=fopen("C:\\Users\\Sara\\Documents\\GitHub\\LocalSearch\\CreaIstanze\\nuova_istanza.txt", "r");
        //fd=fopen("C:\\Users\\Alice\\OneDrive\\Documents\\GitHub\\LocalSearch\\CreaIstanze\\nuova_istanza.txt", "r");
        if(fd==NULL){
            printf("Errore apertura file");
            exit(1);
        }
        i=0;
        scan=0;
        while(scan!=EOF){   //questa scanf legge solo gli archi perchè l'array con i gradi dei nodi se lo calcola la greedy
            scan=fscanf(fd,"%d/%d/%d/%d",&ListaArchi[i].Id,&ListaArchi[i].N1,&ListaArchi[i].N2,&ListaArchi[i].Costo);
            ListaArchi[i].Selected=0;
            ListaArchi[i].Ammissibile=1;
            i++;
        }
        fclose(fd);
        individuaId(ListaArchi,ListaId);
        greedyCostruttiva(ListaArchi,ListaNodi,n,ListaId,count-1,g);
        //fp=fopen("C:\\Users\\Alice\\OneDrive\\Documents\\GitHub\\LocalSearch\\CreaIstanze\\nuova_istanza.txt", "w");
        fp=fopen("C:\\Users\\Sara\\Documents\\GitHub\\LocalSearch\\CreaIstanze\\nuova_istanza.txt","w");
        if(fp==NULL){
            printf("Errore apertura file");
            exit(1);
        }
        for(i=0;i<n;i++)
            fprintf(fp,"%d\n",ListaNodi[i]);
        for(j=0;j<count-1;j++)
            fprintf(fp,"%d/%d/%d/%d/%d\n",ListaArchi[j].Id,ListaArchi[j].N1,ListaArchi[j].N2,ListaArchi[j].Costo,ListaArchi[j].Selected);
        fclose(fp);
    }else if(c=='R') {
        fp=fopen("C:\\Users\\Alice\\OneDrive\\Documents\\GitHub\\LocalSearch\\CreaIstanze\\nuova_istanza.txt", "w");
        //fp=fopen("C:\\Users\\Sara\\Documents\\GitHub\\LocalSearch\\CreaIstanze\\nuova_istanza.txt","w");
        if(fp==NULL){
            printf("Errore apertura file");
            exit(1);
        }
        for (i = 1; i < n; i++) {
            for (j = i + 1; j <= n; j++) {
                costo = rand() % 100;
                fprintf(fp, "%d/%d/%d/%d\n", count, i, j, costo);
                count++;
            }
        }
    }else if(c=='T'){
        printf("Hai selezionato T, ma ora non faccio nulla");
    }else{
        printf("Selezione errata");
    }
        fclose(fp);
}