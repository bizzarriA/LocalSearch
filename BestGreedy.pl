:-lib(fd).
:-lib(fd_global).
:-lib(listut).
:-lib(propia).

is_arco(N1,N2,ListaArchi,ArchiSpanning):-
	(N1<N2
		-> arco(Id,N1,N2,_) infers most, nth1(Id,ArchiSpanning,Bool),Bool==1
		; (N1\=N2
			-> arco(Id,N2,N1,_) infers most, nth1(Id,ArchiSpanning,Bool),Bool==1
			; false
	).
	
is_percorso_aux(N1,N2,ListaArchi,ArchiSpanning):-
	is_arco(N1,N2,ListaArchi,ArchiSpanning),!.
is_percorso_aux(N1,N2,ListaArchi,ArchiSpanning):-
	N_1 is N1+1,
	N::[1..Nnodi],
	is_percorso_aux(N1,N,ListaArchi,ArchiSpanning),is_arco(N,N2).

is_percorso([_,N1,N2,_],ListaArchi,ArchiSpanning):-
	is_percorso_aux(N1,N2,ListaArchi,ArchiSpanning).
	
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
	
nodi_presi([_,N1,N2,_],ListaNodi):-
	nth1(N1,ListaNodi,Val1),
	nth1(N2,ListaNodi,Val2),
	(ground(Val1)
		-> true
		; Val1 is 1
	),
	(ground(Val2)
		-> true
		; Val2 is 1
	).

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
	
is_in_spanning([Id,_,_,_], ArchiSpanning, X):-
	nth1(Id, ArchiSpanning, X).

controllo_grado_loop(_,[],_,_,0).	
controllo_grado_loop(N,[[Id,N1,N2,_]|ListaArchi], ArchiSpanning,K,C):-
	K1 is K-1,
	nth1(Id, ArchiSpanning, Val),
	((C==K1)
		->false;	/*se raggiungo K-1 sono già al limite ed è inutile controllare altri archi */
	),
	( (Val!=1)
		-> controllo_grado_loop(N,ListaArchi,ArchiSpanning,C); /* se l'arco non è selezionato vado oltre*/
	),
	((N!=N1, N!=N2) 
		-> controllo_grado_loop(N,ListaArchi,ArchiSpanning,C) /*se il valore di N non corrisponde a nessun estremo dell'arco considerato vado oltre*/
		; C is C1+1, controllo_grado_loop(N,ListaArchi,ArchiSpanning,C1) /*se invece l'arco è selezionato ed il nodo corrisponde ad almeno uno degli estremi incremento C */
	).

controllo_grado([_,N1,N2,_], ListaArchi, ArchiSpanning, K):-
	controllo_grado_loop(N1, ListaArchi, ArchiSpanning, K, C1),
	controllo_grado_loop(N2, ListaArchi, ArchiSpanning, K, C2).
	
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/	

cerca_arco_aux([],[],Id_attuale,Min_attuale,Id_min_attuale,Id_min_definitivo,Min_definitivo):-!, /*se arrivo alla fine della lista, il minimo definitivo è l'ultimo minimo trovato*/
	Id_min_definitivo is Id_min_attuale,
	Min_definitivo is Min_attuale. 
cerca_arco_aux([[Id,N1,N2,Costo]|Tarchi],[Bool1|Tbool],Id_attuale,Min_attuale,Id_min_attuale,Id_min_definitivo,Min_definitivo):-
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
	nth1(Id_min_definitivo,ListaArchi,Arco).

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
	
define_tree(_,_,ListaNodi):-
	ground(ListaNodi),!.
define_tree(ListaArchi,ArchiSpanning,ListaNodi):-
	cerca_arco(ListaArchi,ArchiSpanning,Arco), /*Arco è una lista che ha Id,N1,N2,Costo*/
	((is_percorso(Arco,ListaArchi,ArchiSpanning); grado(K),controllo_grado(Arco,ListaArchi,ArchiSpanning,K)) /*se Arco va da N1 a N2, controlla che non ci sia già un percorso e che il grado dei nodi sia rispettato*/
		-> is_in_spanning(Arco,ArchiSpanning,0)	/*se c'è il percorso, nella lista spanning l'arco va a 0;*/
		; is_in_spanning(Arco,ArchiSpanning,1), nodi_presi(Arco,ListaNodi)/*setta i nodi e gli archi presi*/
	),
	define_tree(ListaArchi,ArchiSpanning,ListaNodi).
	
spanning_tree(ArchiSpanning,Costo):-
	findall([Id,N1,N2,C],arco(Id,N1,N2,C),ListaArchi),
	findall([_,_,_,C],arco(_,_,_,C),ListaCosti),
	maxlist(ListaCosti,CostoMax),
	length(ListaArchi,Narchi),
	length(ArchiSpanning,Narchi),
	ArchiSpanning::[0,1],
	nodi(Nnodi),
	length(ListaNodi,Nnodi),
	ListaNodi::[0,1],
	define_tree(ListaArchi,ArchiSpanning,ListaNodi,CostoMax),
	calcola_costo(ListaArchi,ArchiSpanning,Costo).
