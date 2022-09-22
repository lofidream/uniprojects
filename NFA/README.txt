Bonacina Luca 813632
Colombo Dugoni Nubia 829729

202001 E1P NFA - COMPILAZIONE D'ESPRESSIONI REGOLARI IN AUTOMI NON DETERMINISTICI

Il progetto consisteva nel realizzare un compilatore da regexps ad NFA, con annesse eventuali operazioni ausiliarie.
I predicati principali da implementare erano is_regexp/1, nfa_regexp_comp/2, nfa_test/2, nfa_clear/0 ed nfa_clear/1.

SPIEGAZIONE DEI PREDICATI (inclusi gli "ausiliari")

1) is_regexp/1

Parametri: RE

Questo predicato risulta vero se l'argomento passato è un'espressione regolare. Risultano espressioni regolari i termini
che soddisfano il predicato built-in atomic/1 e compound/1. Per quanto riguarda i secondi, il controllo del contenuto viene
affidato al predicato check_regexp/1.

2) check_regexp/1

Parametri: RE

Questo predicato controlla che siano espressioni regolari anche i termini contenuti da tutto ciò che soddisfa compound/1
nel predicato is_regexp/1. In particolare, si distinguono alcuni casi:

	- Lista vuota;
	- Lista non vuota, dove abbiamo il controllo elemento per elemento;
	- Funtore riservato (star, plus, seq, or), con controllo sulla riservatezza e sull'arità, più il controllo degli argomenti;
	- Funtore non riservato.

3) reserved_op/2

Parametri: Name, Arity

Questo predicato controlla il nome e l'arità dei funtori riservati.

4) check_functor_name/1

Parametri: Input

Questo predicato controlla che il nome di un particolare funtore non rientri tra quelli riservati.

5) nfa_regexp_comp/2

Parametri: FA_Id, RE

E' il predicato principale del programma che, dati come argomenti un ID per un NFA e un'espressione regolare, compila la regexp in un automa,
che viene inserito nella base dati di Prolog.
Richiama il predicato id_existing/1, che permette di effettuare controlli sull'ID del NFA passato come argomento, successivamente popola la base
dati di Prolog con le informazioni sullo stato iniziale e finale dell'automa e richiama poi il predicato nfa_builder/4.

6) nfa_builder/4

Parametri: FA_Id, RE, Initial, Final

E' il predicato che si occupa di costruire e combinare i sottoautomi delle varie espressioni regolari, allo scopo di produrre l'automa complessivo.
Attraverso l'uso del predicato assert/1 è possibile andare ad aggiungere le informazioni sulle transizioni tra stati all'interno della base dati.
Il funzionamento del predicato nfa_builder/4 si basa sull'algoritmo di Thompson, ovvero l'algoritmo che definisce le regole di traduzione da regexp ad NFA.
Si distinguono diversi casi di costruzione di sottoautomi:

	- Caso di un'espressione regolare atomica o di epsilon;
	- Caso di un funtore non riservato, visto come simbolo;
	- Caso dell'operatore riservato star();
	- Caso dell'operatore riservato plus();
	- Caso dell'operatore riservato or(), con uno o più argomenti;
	- Caso dell'operatore riservato seq().

In particolare, per quanto riguarda gli operatori riservati or() e seq() sono stati definiti dei predicati ausiliari.

7) or_builder/4

Parametri: FA_Id, ArgList, Initial, Final

E' un predicato di appoggio per la costruzione dell'automa relativo all'operatore riservato or(). Riceve in input i dati relativi all'automa e la lista degli argomenti
contenuti nella porzione di espressione regolare relativa all'operatore or(...). Applica poi l'algoritmo di Thompson.

8) seq_builder/4

Parametri: FA_Id, ArgList, Initial, Final

E' un predicato di appoggio per la costruzione dell'automa relativo all'operatore riservato seq(). Il funzionamento è relativamente simile ad or_builder/4, ma analizza anche 
il caso in cui l'argomento corrente sia l'ultimo della sequenza, in modo da collegarsi con una transizione allo stato finale del sottoautoma.

9) id_existing/1

Parametri: FA_Id

E' un predicato che controlla se l'ID del NFA risulta un termine senza variabili. Inoltre, controlla la presenza di un altro automa con lo stesso ID nella base di dati.

10) nfa_test/2

Parametri: FA_Id, Input

E' il predicato che si occupa di stabilire se un determinato Input viene interamente consumato da un automa e quest'ultimo si trova in uno stato finale.
Effettua un controllo sull'Input e verifica lo stato iniziale dell'automa, per poi richiamare il predicato nfa_forward/3.

11) nfa_forward/3

Parametri: FA_Id, Input, State

E' il predicato che si occupa di verificare se è possibile effettuare una transizione tra stati, tenendo conto delle informazioni nella base dati e dell'Input corrente.

12) check_input/1

Parametri: Input

E' il predicato che si occupa di controllare la correttezza dell'Input passato al predicato nfa_test/2. In particolare, controlla ricorsivamente i singoli elementi della lista di Input.

13) nfa_clear/0 ed nfa_clear/1

Parametri: FA_Id

nfa_clear/0 è il predicato che si occupa di eliminare l'intera base di conoscenze, tramite retractall/1. Il secondo, invece, elimina solo le informazioni relative all'automa corrispondente
all'ID passato come argomento.

14) nfa_list/0 ed nfa_list/1

Parametri: FA_Id

In modo speculare ad nfa_clear/0 e nfa_clear/1, questi due predicati si occupano rispettivamente di mostrare l'intera base di dati o solo le informazioni relative all'automa corrispondente
all'ID passato come argomento.

Per rappresentare nella base di dati le informazioni relative agli automi vengono poi definiti dei predicati dynamic:

	- dynamic nfa_initial/2, Parametri: FA_Id, State;
	- dynamic nfa_final/2, Parametri: FA_Id, State;
	- dynamic nfa_delta/4, Parametri: FA_Id, State1, Symbol, State2.





