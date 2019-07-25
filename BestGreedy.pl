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

define_tree(_,_,ListaNodi):-
	ground(ListaNodi),!.
define_tree(ListaArchi,ArchiSpanning,ListaNodi):-
	cerca_arco(ListaArchi,ArchiSpanning,Arco), /*Arco è una lista che ha Id,N1,N2,Costo*/
	((is_percorso(Arco,ListaArchi,ArchiSpanning); grado(K),controllo_grado(Arco,ListaArchi,ArchiSpanning,K)), /*se Arco va da N1 a N2, controlla che non ci sia già un percorso e che il grado dei nodi sia rispettato*/
		-> is_in_spanning(Arco,ArchiSpanning,0)	/*se c'è il percorso, nella lista spanning l'arco va a 0*/
		; is_in_spanning(Arco,ArchiSpanning,1), nodi_presi(Arco,ListaNodi)/*setta i nodi e gli archi presi*/
	),
	define_tree(ListaArchi,ArchiSpanning,ListaNodi).
	
spanning_tree(ArchiSpanning,Costo):-
	findall([Id,N1,N2,C],arco(Id,N1,N2,C),ListaArchi),
	length(ListaArchi,Narchi),
	length(ArchiSpanning,Narchi),
	ArchiSpanning::[0,1],
	nodi(Nnodi),
	length(ListaNodi,Nnodi),
	ListaNodi::[0,1],
	define_tree(ListaArchi,ArchiSpanning,ListaNodi),
	calcola_costo(ListaArchi,ArchiSpanning,Costo).
