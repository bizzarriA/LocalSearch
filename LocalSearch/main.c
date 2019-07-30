#include <stdio.h>

#define NUMERONODI 5
#define NUMEROARCHI 8

typedef struct s_arco{
    int Id;
    int N1;
    int N2;
    int Costo;
    int Selected;
}arco;

arco individuaArco(arco ListaArchi[]){
    int min_costo=100;
    int min_id=0;
    for(int i=0; i <NUMEROARCHI;i++){
        if (ListaArchi[i].Costo<min_costo && ListaArchi[i].Selected==0){
            min_costo=ListaArchi[i].Costo;
            min_id=ListaArchi[i].Id;
        }
    }
}

void main() {
    int Nodi[NUMERONODI];
    arco ListaArchi[NUMEROARCHI];




}