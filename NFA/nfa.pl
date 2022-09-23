%%% -*- Mode: Prolog -*-

%%%% nfa.pl

%%% is_regexp/2

% caso in cui RE è atomico

is_regexp(RE) :-
    atomic(RE),
    !.

% caso in cui RE è composto, controllo il "tipo" di RE

is_regexp(RE) :-
    compound(RE),
    !,
    check_regexp(RE).

%%% reserved_op/2

% caso star e plus, operatori con arità 1

reserved_op(star, 1).

reserved_op(plus, 1).

% caso seq e or, operatori con arità diversa da 0

reserved_op(seq, Arity) :-
    Arity \= 0.

reserved_op(or, Arity) :-
    Arity \= 0.

%%% check_regexp/1

check_regexp([]) :-
    !.

% caso di una lista, controlla poi ricorsivamente la coda

check_regexp([Head | Tail]) :-
    !,
    is_regexp(Head),
    check_regexp(Tail).

% caso di un operatore riservato (seq, or, star e plus)

check_regexp(RE) :-
    compound_name_arity(RE, Name, Arity),
    reserved_op(Name, Arity),
    !,
    compound_name_arguments(RE, Name, Args),
    is_regexp(Args).

% caso di un operatore non riservato, ad es. foo(42)

check_regexp(RE) :-
    check_functor_name(RE).

%%% nfa_regexp_comp/2

nfa_regexp_comp(FA_Id, RE) :-
    reset_gensym(q),
    id_existing(FA_Id),
    is_regexp(RE),
    assert(nfa_initial(FA_Id, q0)),
    assert(nfa_final(FA_Id, qf)),
    nfa_builder(FA_Id, RE, q0, qf),
    reset_gensym(q),
    !.

%%% nfa_builder/4

% caso atomic, gestisce anche il caso epsilon

nfa_builder(FA_Id, RE, Initial, Final) :-
    atomic(RE),
    assert(nfa_delta(FA_Id, Initial, RE, Final)).

% caso compound senza funtore riservato

nfa_builder(FA_Id, RE, Initial, Final) :-
    check_functor_name(RE),
    assert(nfa_delta(FA_Id, Initial, RE, Final)).

% caso star

nfa_builder(FA_Id, star(RE), Initial, Final) :-
    assert(nfa_delta(FA_Id, Initial, epsilon, Final)),
    gensym(q, REInitial),
    assert(nfa_delta(FA_Id, Initial, epsilon, REInitial)),
    gensym(q, REFinal),
    assert(nfa_delta(FA_Id, REFinal, epsilon, REInitial)),
    assert(nfa_delta(FA_Id, REFinal, epsilon, Final)),
    nfa_builder(FA_Id, RE, REInitial, REFinal).

% caso plus

nfa_builder(FA_Id, plus(RE), Initial, Final) :-
    gensym(q, REInitial),
    assert(nfa_delta(FA_Id, Initial, epsilon, REInitial)),
    gensym(q, REFinal),
    assert(nfa_delta(FA_Id, REFinal, epsilon, REInitial)),
    assert(nfa_delta(FA_Id, REFinal, epsilon, Final)),
    nfa_builder(FA_Id, RE, REInitial, REFinal).

% caso or con un solo argomento

nfa_builder(FA_Id, RE, Initial, Final) :-
    RE =.. [or, R],
    nfa_builder(FA_Id, R, Initial, Final).

% caso or con più argomenti

nfa_builder(FA_Id, RE, Initial, Final) :-
    RE =.. [or, Arg | Args],
    or_builder(FA_Id, [Arg | Args], Initial, Final).

% caso seq

nfa_builder(FA_Id, RE, Initial, Final) :-
    RE =.. [seq, Arg | Args],
    seq_builder(FA_Id, [Arg | Args], Initial, Final).

%%% or_builder/4

% caso lista argomenti vuota

or_builder(_, [], _, _).

% caso lista argomenti non vuota

or_builder(FA_Id, [Arg | Args], Initial, Final) :-
    gensym(q, REInitial),
    assert(nfa_delta(FA_Id, Initial, epsilon, REInitial)),
    gensym(q, REFinal),
    assert(nfa_delta(FA_Id, REFinal, epsilon, Final)),
    nfa_builder(FA_Id, Arg, REInitial, REFinal),
    or_builder(FA_Id, Args, Initial, Final).

%%% seq_builder/4

% caso in cui la coda della lista è vuota

seq_builder(FA_Id, [Arg | Args], Initial, Final) :-
    Args = [],
    nfa_builder(FA_Id, Arg, Initial, Final).

% caso in cui la coda della lista non è vuota

seq_builder(FA_Id, [Arg | Args], Initial, Final) :-
    gensym(q, REFinal),
    nfa_builder(FA_Id, Arg, Initial, REFinal),
    gensym(q, REInitial),
    assert(nfa_delta(FA_Id, REFinal, epsilon, REInitial)),
    seq_builder(FA_Id, Args, REInitial, Final).

%%% check_functor_name/1

% per verificare la non riservatezza del nome del funtore

check_functor_name(Input) :-
    compound_name_arity(Input, Name, _),
    Name \= star,
    Name \= plus,
    Name \= or,
    Name \= seq.

%%% id_existing/1

id_existing(FA_Id) :-
    nonvar(FA_Id),
    not(nfa_initial(FA_Id, _)).

%%% nfa_test/2

nfa_test(FA_Id, Input) :-
    is_list(Input),
    check_input(Input),
    nfa_initial(FA_Id, Initial),
    nfa_forward(FA_Id, Input, Initial),
    !.

%%% nfa_forward/3

% la lista input è vuota e lo stato corrente è quello finale

nfa_forward(FA_Id, [], State) :-
    nfa_final(FA_Id, State).

% la lista input è vuota, si avanza al nuovo stato con una epsilon-mossa

nfa_forward(FA_Id, [], State) :-
    nfa_delta(FA_Id, State, epsilon, NewState),
    nfa_forward(FA_Id, [], NewState).

% esiste una mossa con il simbolo Sym

nfa_forward(FA_Id, [Sym | Rest], State) :-
    nfa_delta(FA_Id, State, Sym, NewState),
    nfa_forward(FA_Id, Rest, NewState).

% esiste una epsilon-mossa (non si consuma Sym)

nfa_forward(FA_Id, [Sym | Rest], State) :-
    nfa_delta(FA_Id, State, epsilon, NewState),
    nfa_forward(FA_Id, [Sym | Rest], NewState).

%%% check_input/1

% caso lista vuota

check_input([]) :-
    !.

% caso testa della lista atomica, controllo ricorsivo sulla coda

check_input([H | T]) :-
    atomic(H),
    !,
    check_input(T).

% caso testa della lista compound, controllo ricorsivo sulla coda

check_input([H | T]) :-
    compound(H),
    check_input(T).

%%% nfa_clear/0

% elimina l'intera base di conoscenze

nfa_clear :-
    retractall(nfa_initial(_, _)),
    retractall(nfa_delta(_, _, _, _)),
    retractall(nfa_final(_, _)).

%%% nfa_clear/1

% elimina le informazioni di un automa, dato il suo id

nfa_clear(FA_Id) :-
    retractall(nfa_initial(FA_Id, _)),
    retractall(nfa_delta(FA_Id, _, _, _)),
    retractall(nfa_final(FA_Id, _)).

%%% nfa_list/0

% mostra l'intera base di conoscenze

nfa_list :-
    listing(nfa_initial(_, _)),
    listing(nfa_delta(_, _, _, _)),
    listing(nfa_final(_, _)).

%%% nfa_list/1

% mostra le informazioni di un automa, dato il suo id

nfa_list(FA_Id) :-
    listing(nfa_initial(FA_Id, _)),
    listing(nfa_delta(FA_Id, _, _, _)),
    listing(nfa_final(FA_Id, _)).

% definizione dei predicati dynamic per le informazioni sugli automi

:- dynamic nfa_initial/2.
:- dynamic nfa_final/2.
:- dynamic nfa_delta/4.

%%%% End of file -- nfa.pl --
