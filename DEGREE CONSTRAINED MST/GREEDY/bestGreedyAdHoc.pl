:-lib(fd).
:-lib(fd_global).
:-lib(listut).
:-lib(propia).
:-lib(edge_finder).

/*----OBSOLETO-------------------------------------------------------------------------------------------------------------------------------------*/
is_arco(N1,N2,ListaArchi,ArchiSpanning):-
	(N1<N2
		-> arco(Id,N1,N2,_) infers most, nth1(Id,ArchiSpanning,Bool),Bool==1
		; (N1\=N2
			-> arco(Id,N2,N1,_) infers most, nth1(Id,ArchiSpanning,Bool),Bool==1
			; false
		)	
	).
	
/*-VALIDITA'-PERCORSO-----------------------------------------------------------------------------------------------------------------------------*/

is_percorso([_,N1,N2,_],ListaNodi):-
	nth1(N1,ListaNodi,Val1),
	nth1(N2,ListaNodi,Val2),
	((Val1==1,Val2==1)	/*se entrambi i nodi sono già selezionati esiste per forza già un percorso non orientato da N1 a N2*/
		->true
		;false
	).

/*---SEGNA I NODI N1 E N2 COME PRESI-----------------------------------------------------------------------------------------------------------------------------------------------*/

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

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
	
is_in_spanning([Id,_,_,_], ArchiSpanning, X):-	/*assegna 0 o 1 all'arco, 0 se non può essere preso, 1 se è stato preso*/
	nth1(Id, ArchiSpanning, X,_).

controllo_grado_loop(_,[],_,0).	
controllo_grado_loop(N,[[Id,N1,N2,_]|ListaArchi], ArchiSpanning,C):-
	K1 is K-1,
	nth1(Id, ArchiSpanning, Val,_),
	((not ground(Val))
		-> controllo_grado_loop(N,ListaArchi,ArchiSpanning,C)
		;((Val\= 1; (N\=N1, N\=N2) )
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
	
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/	

gia_controllato(_,[]):-!,false.	/*controlla se è nella lista degli archi accettabili ma non già presi perchè nessuno dei loro nodi era già stato selezionato*/
gia_controllato(Arco,[H|T]):-
	(Arco==H
		->true
		; gia_controllato(Arco,T)
	).

/*----CERCA L'ARCO DI COSTO MINIMO NON ANCORA SELEZIONATO-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

cerca_arco_aux([],[],Id_attuale,Min_attuale,Id_min_attuale,Id_min_definitivo,Min_definitivo,_):-!, /*se arrivo alla fine della lista, il minimo definitivo è l'ultimo minimo trovato*/
	Id_min_definitivo is Id_min_attuale,
	Min_definitivo is Min_attuale. 
cerca_arco_aux([[Id,N1,N2,Costo]|Tarchi],[Bool1|Tbool],Id_attuale,Min_attuale,Id_min_attuale,Id_min_definitivo,Min_definitivo,ListaControllati):-
	Id_prossimo is Id_attuale+1, /*per il controllo l'arco successivo*/
	(not ground(Bool1)
		-> (Costo<Min_attuale, not gia_controllato([Id,N1,N2,Costo],ListaControllati)
				-> Id_new_min is Id_attuale, New_min is Costo, cerca_arco_aux(Tarchi,Tbool,Id_prossimo,New_min,Id_new_min,Id_min_definitivo,Min_definitivo,ListaControllati)
				; cerca_arco_aux(Tarchi,Tbool,Id_prossimo,Min_attuale,Id_min_attuale,Id_min_definitivo,Min_definitivo,ListaControllati)
			)
		; cerca_arco_aux(Tarchi,Tbool,Id_prossimo,Min_attuale,Id_min_attuale,Id_min_definitivo,Min_definitivo,ListaControllati)
	).
		
cerca_arco(ListaArchi,ArchiSpanning,Arco,CostoMax,ListaControllati):-
	cerca_arco_aux(ListaArchi,ArchiSpanning,1,CostoMax,1,Id_min_definitivo,Min_definitivo,ListaControllati),	/*CostoMax è il minimo iniziale in quanto upper bound del minimo e 1 è il primo arco che si controlla*/
	element(Id_min_definitivo,ListaArchi,Arco).

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

is_empty([]):-!.	/*ritorna true se nessun nodo fa ancora parte dell'MST, ovvero se ci si trova alla prima scelta della greedy*/
is_empty([Nodo|Tnodi]):-
	(ground(Nodo)
		-> false
		; is_empty(Tnodi)
	).
	
has_node_selected([_,N1,N2,_],ListaNodi):-	/*controlla se almeno uno dei due nodi dell'arco fa già parte del MST, ovvero se l'arco esaminato è adiacente a un arco appartenente al MST*/
	nth1(N1,ListaNodi,Val1,_),
	nth1(N2,ListaNodi,Val2,_),
	((Val1==1;Val2==1)
		->true
		;false
	).

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

define_tree(_,_,ListaNodi,_,_):-
	ground(ListaNodi),!.
define_tree(ListaArchi,ArchiSpanning,ListaNodi,CostoMax,ListaControllati):-
	cerca_arco(ListaArchi,ArchiSpanning,Arco,CostoMax,ListaControllati), /*Arco è una lista che ha Id,N1,N2,Costo. La funzione prende l'arco di costo minore non selezionato*/
	((is_percorso(Arco,ListaNodi); grado(K),controllo_grado(Arco,ListaArchi,ArchiSpanning,K)) /*se Arco va da N1 a N2, controlla che non ci sia già un percorso e che il grado dei nodi sia rispettato*/
		-> is_in_spanning(Arco,ArchiSpanning,0),Lista_presi=Lista_presi_old,Lista_controllati_new=ListaControllati	/*se c'è il percorso, nella lista spanning l'arco va a 0;*/
		; ((is_empty(ListaNodi); has_node_selected(Arco,ListaNodi)) /*primo giro o adiacenza*/
			-> is_in_spanning(Arco,ArchiSpanning,1), nodi_presi(Arco,ListaNodi),Lista_presi=Lista_presi_new,Lista_controllati_new=[]/*setta i nodi e gli archi presi*/
			; append(ListaControllati,[Arco],Lista_controllati_new)
		)
	),
	define_tree(ListaArchi,ArchiSpanning,ListaNodi,CostoMax,Lista_controllati_new).

/*SPIEGAZIONE: prende l'arco che costa meno e controlla se prendendolo non si generano cicli e se è rispettato il grado massimo di ogni nodo. Se questa condizione è vera, l'arco è accettabile
ma può essere preso solo se è adiacente ad un arco già preso (ovvero se uno dei suoi nodi è già stato selezionato) in modo che sia garantita la creazione di un albero.
Se l'arco è ammissibile e anche adiacente, viene preso, altrimenti, se è solo ammissibile, viene inserito in una lista controllati che viene passata alla funzione cerca_arco, in modo che essa sappia di aver già
controllato quell'arco, ma al momento non va bene e quindi non lo deve considerare.
Non appena un nuovo arco viene preso, la lista controllati si svuota perchè gli archi che prima erano ammissibili ma non adiacenti ad archi già selezionati potrebbero ora essere adiacenti al nuovo arco*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/	

calcola_costo([],[],0):-!.		
calcola_costo([[_,_,_,C]|T],[Bool|Tbool],Costo):-
	(Bool==1
		-> Costo#=C+Costo1
		; Costo#=Costo1
	),
	calcola_costo(T,Tbool,Costo1).
	
spanning_tree(ArchiSpanning,Costo,L):-
	findall([Id,N1,N2,C],arco(Id,N1,N2,C),ListaArchi),
	findall(C,arco(_,_,_,C),ListaCosti),
	maxlist(ListaCosti,CostoMax),
	length(ListaArchi,Narchi),
	length(ArchiSpanning,Narchi),	/*ArchiSpanning è la lista di elementi binari che indicano se l'arco i fa parte del MST o no*/
	ArchiSpanning::[0,1],
	nodi(Nnodi),
	length(ListaNodi,Nnodi),
	ListaNodi::[0,1],
	define_tree(ListaArchi,ArchiSpanning,ListaNodi,CostoMax,[]),
	calcola_costo(ListaArchi,ArchiSpanning,Costo),
	sistema_lista(ArchiSpanning,L).	/*serve solo per rendere più leggibile la lista degli archi del MST*/
	
/*NB: gli elementi non ground di ArchiSpanning, indicano archi che non sono stati esaminati o, se lo sono stati, non sono mai risultati non ammissibili o i migliori*/
