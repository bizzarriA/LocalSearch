:-lib(fd).
:-lib(fd_global).
:-lib(listut).
:-lib(propia).
:-lib(edge_finder).

	
/*---IS_PERCORSO-se-l'arco-considerato-è-valido-restituisce-false-altrimenti-true----------------------------------------------------------------------------------------------------*/

is_percorso([_,N1,N2,_],ListaNodi):-
	nth1(N1,ListaNodi,Val1),
	nth1(N2,ListaNodi,Val2),
	((Val1==1)	/*se entrambi i nodi sono già selezionati esiste per forza già un percorso non orientato da N1 a N2, se nessuno dei due è selezionato l'albero non è connesso*/
		->(Val2==1 -> true; false)
		;(Val2==1 -> false; true)
	).

/*--CONTROLLO-NODI-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
	
nodi_presi([_,N1,N2,_],ListaNodi):-
	nth1(N1,ListaNodi,Val1,_),
	nth1(N2,ListaNodi,Val2,_),
	(ground(Val1)
		-> true
		; Val1 is 1
	),
	(ground(Val2)
		-> true
		; Val2 is 1
	).

/*---CONTROLLO-GRADO-E-SE-NODO-PRESO----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
	
is_in_spanning([Id,_,_,_], ArchiSpanning, X):-
	nth1(Id, ArchiSpanning, X,_).

controllo_grado_loop(_,[],_,0).	
controllo_grado_loop(N,[[Id,N1,N2,_]|ListaArchi], ArchiSpanning,C):-
	nth1(Id, ArchiSpanning, Val,_),
	((not ground(Val))
		-> controllo_grado_loop(N,ListaArchi,ArchiSpanning,C)
		;((Val\== 1; (N\=N1, N\=N2) )
			-> controllo_grado_loop(N,ListaArchi,ArchiSpanning,C) /*se l'arco non è selezionato oppure se il valore di N non corrisponde a nessun estremo dell'arco considerato vado oltre*/
			; C is C1+1, controllo_grado_loop(N,ListaArchi,ArchiSpanning,C1) /*se invece l'arco è selezionato ed il nodo corrisponde ad almeno uno degli estremi incremento C */
		)
	).

controllo_grado([_,N1,N2,_], ListaArchi, ArchiSpanning, K):-
	controllo_grado_loop(N1, ListaArchi, ArchiSpanning, C1),
	controllo_grado_loop(N2, ListaArchi, ArchiSpanning, C2),
	((C1>=K; C2>=K)
		->true
		;false
	).
	
/*---CERCO-ARCO----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/	

cerca_arco_aux([],[],_,Min_attuale,Id_min_attuale,Id_min_definitivo,Min_definitivo):-!, /*se arrivo alla fine della lista, il minimo definitivo è l'ultimo minimo trovato*/
	Id_min_definitivo is Id_min_attuale,
	Min_definitivo is Min_attuale. 
cerca_arco_aux([[_,_,_,Costo]|Tarchi],[Bool1|Tbool],Id_attuale,Min_attuale,Id_min_attuale,Id_min_definitivo,Min_definitivo):-
	Id_prossimo is Id_attuale+1, /*per il controllo l'arco successivo*/
	(not ground(Bool1)
		-> (Costo<Min_attuale
				-> Id_new_min is Id_attuale, New_min is Costo, cerca_arco_aux(Tarchi,Tbool,Id_prossimo,New_min,Id_new_min,Id_min_definitivo,Min_definitivo)
				; cerca_arco_aux(Tarchi,Tbool,Id_prossimo,Min_attuale,Id_min_attuale,Id_min_definitivo,Min_definitivo)
			)
		; cerca_arco_aux(Tarchi,Tbool,Id_prossimo,Min_attuale,Id_min_attuale,Id_min_definitivo,Min_definitivo)
	).
		
cerca_arco(ListaArchi,ArchiSpanning,Arco,CostoMax):-
	cerca_arco_aux(ListaArchi,ArchiSpanning,1,CostoMax,1,Id_min_definitivo,Min_definitivo),	/*CostoMax è il minimo iniziale in quanto upper bound del minimo e 1 è il primo arco che si controlla*/
	element(Id_min_definitivo,ListaArchi,Arco).

/*---DEFINE_TREE-definisce-l'albero--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

define_tree(_,_,ListaNodi,_,_):-
	ground(ListaNodi),!.
define_tree(ListaArchi,ArchiSpanning,ListaNodi,CostoMax,N):-
	cerca_arco(ListaArchi,ArchiSpanning,Arco,CostoMax), /*Arco è una lista che ha Id,N1,N2,Costo*/
	(N\=0 /*se non sono al primo giro*/
		->((is_percorso(Arco,ListaNodi); grado(K),controllo_grado(Arco,ListaArchi,ArchiSpanning,K)) /*se Arco va da N1 a N2, controlla che non ci sia già un percorso e che il grado dei nodi sia rispettato*/
			-> is_in_spanning(Arco,ArchiSpanning,0)	/*se c'è il percorso, nella lista spanning l'arco va a 0;*/
			; is_in_spanning(Arco,ArchiSpanning,1), nodi_presi(Arco,ListaNodi)/*setta i nodi e gli archi presi*/
		)
		;is_in_spanning(Arco,ArchiSpanning,1), nodi_presi(Arco,ListaNodi) /*se sono al primo nodo setto i nodi e gli archi presi*/
	),
	define_tree(ListaArchi,ArchiSpanning,ListaNodi,CostoMax,1).

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/	

calcola_costo([],[],0):-!.		
calcola_costo([[_,_,_,C]|T],[Bool|Tbool],Costo):-
	(Bool==1
		-> Costo#=C+Costo1
		; Costo#=Costo1
	),
	calcola_costo(T,Tbool,Costo1).
	
spanning_tree(ArchiSpanning,Costo):-
	findall([Id,N1,N2,C],arco(Id,N1,N2,C),ListaArchi),
	findall(C,arco(_,_,_,C),ListaCosti),
	maxlist(ListaCosti,CostoMax),
	length(ListaArchi,Narchi),
	length(ArchiSpanning,Narchi),
	ArchiSpanning::[0,1],
	nodi(Nnodi),
	length(ListaNodi,Nnodi),
	ListaNodi::[0,1],
	define_tree(ListaArchi,ArchiSpanning,ListaNodi,CostoMax,0),
	calcola_costo(ListaArchi,ArchiSpanning,Costo).
