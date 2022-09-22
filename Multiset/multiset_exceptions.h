/**
	@headerfile multiset_exceptions.h

	@brief Dichiarazione delle classi di eccezioni custom legate alla classe MultiSet.

*/

// Guardie

#ifndef MULTISET_EXCEPTIONS_H
#define MULTISET_EXCEPTIONS_H

/**
	@brief Eccezione valore non trovato

	@description
	Questa eccezione viene lanciata quando si tenta di eliminare un valore
	non presente nel MultiSet.
*/
class multiset_value_not_found {

};


/**
	@brief Eccezione di iteratore del MultiSet fuori dai limiti

	@description
	Questa eccezione viene lanciata quando l'iteratore prova ad accedere
	ad una locazione di memoria al di fuori del MultiSet.
*/
class multiset_iterator_out_of_bounds {

};

#endif

// Fine multiset_exceptions.h