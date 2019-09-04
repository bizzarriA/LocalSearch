:-lib(fd).

stampa_lista([],[],_):-!.
stampa_lista([H|T],[Bool|Tbool],Count):-
	ground(Bool),Bool=1,!,
	H is Count,
	Count1 is Count+1,
	stampa_lista(T,Tbool,Count1).
stampa_lista(L,[_|Tbool],Count):-!,
	Count1 is Count+1,
	stampa_lista(L,Tbool,Count1).
	
sistema_lista(L,Per_lettura):-
	stampa_lista(Per_lettura,L,1),
	labeling(Per_lettura).