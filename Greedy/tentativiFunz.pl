
is_arco(N1,N2,ListaArchi,ArchiSpanning):-
	(N1<N2
		-> arco(Id,N1,N2,_) infers most, nth1(Id,ArchiSpanning,Bool),Bool==1
		; (N1\=N2
			-> arco(Id,N2,N1,_) infers most, nth1(Id,ArchiSpanning,Bool),Bool==1
			; false
		)	
	).
/*--------------------------------------------------------------------------------------------------------------------------------------------------*/
/*
is_percorso_aux(N1,N2,ListaArchi,ArchiSpanning):-
	is_arco(N1,N2,ListaArchi,ArchiSpanning),!.
is_percorso_aux(N1,N2,ListaArchi,ArchiSpanning):-
	N_1 is N1+1,
	nodi(Nnodi),
	N::[1..Nnodi],
	is_percorso_aux(N1,N,ListaArchi,ArchiSpanning),is_arco(N,N2).

is_percorso([_,N1,N2,_],ListaArchi,ArchiSpanning):-
	is_percorso_aux(N1,N2,ListaArchi,ArchiSpanning).
	*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/	
/*TENTATIVO 2*/
define_treep(_,_,_,ListaNodi,_):-
	ground(ListaNodi),!.
define_treep(ListaArchi,ArchiSpanning,Lista_presi_old,ListaNodi,CostoMax):- /*lista presi old all'inizio è vuota, poi mantiene gli archi presi nell'ordine*/
	cerca_arco(ListaArchi,ArchiSpanning,Arco,CostoMax), /*Arco è una lista che ha Id,N1,N2,Costo*/
	prova_grafo(Lista_presi_old,Arco,Lista_presi_new,Grafo_prova),
	((not graph_is_acyclic(Grafo_prova); grado(K),controllo_grado(Arco,ListaArchi,ArchiSpanning,K)) /*se Arco va da N1 a N2, controlla che non ci sia già un percorso e che il grado dei nodi sia rispettato*/
		-> is_in_spanning(Arco,ArchiSpanning,0),Lista_presi=Lista_presi_old	/*se c'è il percorso, nella lista spanning l'arco va a 0;*/
		; is_in_spanning(Arco,ArchiSpanning,1), nodi_presi(Arco,ListaNodi),Lista_presi=Lista_presi_new/*setta i nodi e gli archi presi*/
	),
	define_treep(ListaArchi,ArchiSpanning,Lista_presi,ListaNodi,CostoMax).
	
	prova_grafo(Lista_presi_old,[Id,N1,N2,C],Lista_presi_new,Graph):-
		nodi(Nnodi),
		append(Lista_presi_old,[e(N1,N2,C),e(N2,N1,C)],Lista_presi_new),
		make_graph(Nnodi,Lista_presi_new,Graph).

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/	
