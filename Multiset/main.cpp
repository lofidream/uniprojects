/**
	@file main.cpp

	@brief Test della classe MultiSet su tipi primitivi e custom

	@description
	File sorgente con funzione main(). Contiene la dichiarazione e definizione di
	tipi custom e funzioni di test per verificare la correttezza dei metodi presenti
	nella classe MultiSet del file header multiset.h.
*/

// Direttive pre-compilatore

#include <iostream> // std::cout
#include <cassert> // assert
#include <string> // uso di oggetti std::string e relative funzioni associate
#include <ostream> // std::ostream
#include "multiset.h" // Classe MultiSet, iteratori e funzioni globali associate

/**
	@brief Struttura che definisce l'uguaglianza tra due interi, tramite funtore

	@param a primo intero
	@param b secondo intero

	@return True se a e b sono uguali, false altrimenti
*/
struct equal_int {
	bool operator()(int a, int b) const {
		return a==b;
	}
};

/**
	@brief Struttura che definisce l'uguaglianza tra due numeri con la virgola (double), tramite funtore

	@param a primo double
	@param b secondo double

	@return True se a e b sono uguali, false altrimenti
*/
struct equal_double {
	bool operator()(double a, double b) const {
		return a==b;
	}
};

/**
	@brief Struttura che definisce l'uguaglianza tra due oggetti std::string, tramite funtore

	@param s1 reference costante della prima stringa
	@param s2 reference costante della seconda stringa

	@return True se la funzione "compare"(std::string) applicata a s1 con argomento s2 restituisce 0, false altrimenti
*/
struct equal_string {
	bool operator()(const std::string &s1, const std::string &s2) const {
		return (s1.compare(s2) == 0);
	}
};

/**
	@brief Struttura che definisce un punto 2D
*/
struct point {
	int x; ///< Ascissa di un punto
	int y; ///< Ordinata di un punto

	/**
		@brief Costruttore di default per un punto
	*/
	point() {}

	/**
		@brief Costruttore secondario per un punto

		@description
		Istanzia un punto, inizializzando la sua ascissa e la sua ordinata.

		@param a ascissa del punto
		@param b ordinata del punto
	*/
	point(int a, int b) : x(a), y(b) {}

	/**
		@brief Operatore di uguaglianza per punti

		@description
		Ridefinizione dell'operator== per verificare l'uguaglianza tra punti.
		Due punti sono uguali se le ascisse coincidono e le ordinate coincidono.

		@param other punto da confrontare con quello corrente

		@return true se i punti sono uguali, false altrimenti
	*/
	bool operator==(const point &other) {
		return ((this->x == other.x) && (this->y == other.y));
	}
};

/**
	@brief Struttura che definisce l'uguaglianza tra due punti, tramite funtore
	
	@description
	Due punti sono uguali se le ascisse coincidono e le ordinate coincidono.

	@param p1 primo punto
	@param p2 secondo punto

	@return True se p1 e p2 sono uguali, false altrimenti
*/
struct equal_point {
	bool operator()(const point &p1, const point &p2) const {
		return ((p1.x == p2.x) && (p1.y == p2.y));
	}
};

/**
	@brief Ridefinizione dell'operatore di stream << per un punto

	@description
	La ridefinizione è necessaria per l'operator << di stream nella classe MultiSet.

	@param os oggetto di stream output
	@param p punto da stampare

	@return riferimento allo stream di output
*/
std::ostream &operator<<(std::ostream &os, const point &p) {
		std::cout << "(" << p.x << ", " << p.y << ")";
		return os;
}

/**
	@brief Struttura che definisce una persona
*/
struct person {
	std::string name; ///< Nome della persona
	std::string surname; ///< Cognome della persona
	unsigned int age; ///< Età della persona

	/**
		@brief Costruttore di default di person
	*/
	person() {}

	/**
		@brief Costruttore secondario di person

		@description
		Istanzia una persona, inizializzando il nome, il cognome e l'età.

		@param n nome della persona
		@param s cognome della persona
		@param a età della persona
	*/
	person(std::string n, std::string s, unsigned int a) : name(n), surname(s), age(a) {}

	/**
		@brief Operatore di uguaglianza per persone

		@description
		Ridefinizione dell'operator== per verificare l'uguaglianza tra persone.
		Due persone sono uguali se i nomi, i cognomi e le età coincidono.

		@param other persona da confrontare con quella corrente

		@return true se le persone sono uguali, false altrimenti
	*/
	bool operator==(const person &other) {
		return ((this->name.compare(other.name) == 0) && (this->surname.compare(other.surname) == 0) && (this->age == other.age));
	}
};

/**
	@brief Struttura che definisce l'uguaglianza tra due persone, tramite funtore
	
	@description
	Due persone sono uguali se i nomi, i cognomi e le età coincidono.

	@param p1 prima persona
	@param p2 seconda persona

	@return True se p1 e p2 sono uguali, false altrimenti
*/
struct equal_person {
	bool operator()(const person &p1, const person &p2) const {
		return ((p1.name.compare(p2.name) == 0) && (p1.surname.compare(p2.surname) == 0) && (p1.age == p2.age));
	}
};

/**
	@brief Ridefinizione dell'operatore di stream << per una persona

	@description
	La ridefinizione è necessaria per l'operator << di stream nella classe MultiSet.

	@param os oggetto di stream output
	@param p persona da stampare

	@return riferimento allo stream di output
*/
std::ostream &operator<<(std::ostream &os, const person &p) {
	std::cout << "[" << p.name << " " << p.surname << ", " << p.age << "]";
	return os;
}

/**
	@brief Struttura templata che definisce l'uguaglianza tra MultiSet, tramite funtore
	
	@description
	Il funtore sfrutta l'operator== definito nella classe MultiSet.

	@tparam T tipo del valore degli elementi del MultiSet
	@tparam E funtore di uguaglianza tra due elementi

	@param ms1 primo MultiSet
	@param ms2 secondo MultiSet

	@return True se ms1 ed ms2 sono uguali, false altrimenti
*/
template <typename T, typename E>
struct equal_multiset {
	bool operator()(const MultiSet<T,E> &ms1, const MultiSet<T,E> &ms2) const {
		return (ms1 == ms2);
	}
};

// Typedef per testare la classe MultiSet

typedef MultiSet<int, equal_int> msint; // MultiSet di int
typedef MultiSet<double, equal_double> msdouble; // MultiSet di double
typedef MultiSet<std::string, equal_string> msstr; // MultiSet di std::string
typedef MultiSet<point, equal_point> mspoint; // MultiSet di point
typedef MultiSet<person, equal_person> msperson; // MultiSet di person
typedef MultiSet<MultiSet<point, equal_point>, equal_multiset<point, equal_point>> ms_mspoint; // MultiSet di MultiSet di point

/**
	@brief Test della classe MultiSet su tipi int

	@description
	Questa funzione globale si occupa di effettuare alcuni test delle funzionalità della
	classe MultiSet su tipi interi (MultiSet di interi).
*/
void test_multiset_int() {
	std::cout << "!!!### TEST DELLA CLASSE MULTISET SU INTERI ###!!!" << std::endl;
	std::cout << std::endl;

	msint ms1; // Test costruttore di default

	std::cout << "Inserisco degli interi nel MultiSet ms1: 5 4 3 3 8 4 20" << std::endl;
	std::cout << std::endl;

	// Test metodo add
	ms1.add(5);
	ms1.add(4);
	ms1.add(3);
	ms1.add(3);
	ms1.add(8);
	ms1.add(4);
	ms1.add(20);

	std::cout << "Stampo il MultiSet ms1" << std::endl;
	std::cout << ms1 << std::endl; // Test operator<<
	std::cout << std::endl;

	msint ms2(ms1); // Test copy constructor

	std::cout << "Stampo il Multiset ms2, creato con il copy constructor a partire dal MultiSet ms1" << std::endl;
	std::cout << ms2 << std::endl; // Test operator<<
	std::cout << std::endl;

	msint ms3;
	ms3 = ms1; // Test operatore di assegnamento

	std::cout << "Stampo il MultiSet ms3, creato con l'operatore di assegnamento, a partire dal MultiSet ms1" << std::endl;
	std::cout << ms3 << std::endl; // Test operator<<
	std::cout << std::endl;

	std::cout << "Test di size() su ms1 (7): " << std::endl;
	assert(ms1.size() == 7);
	std::cout << "Risposta: " << ms1.size() << std::endl; // Test size
	std::cout << std::endl;

	std::cout << "Test di size() su ms2 (7): " << std::endl;
	assert(ms2.size() == 7);
	std::cout << "Risposta: " << ms2.size() << std::endl; // Test size
	std::cout << std::endl;

	std::cout << "Test di size() su ms3 (7): " << std::endl;
	assert(ms3.size() == 7);
	std::cout << "Risposta: " << ms3.size() << std::endl; // Test size
	std::cout << std::endl;

	std::cout << "Aggiungo ad ms1 il numero 35 (non presente)" << std::endl;
	assert(ms1.nocc(35) == 0);
	ms1.add(35); // Test add
	assert(ms1.size() == 8);
	assert(ms1.nocc(35) == 1);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;
	
	std::cout << "Aggiungo ad ms1 il numero 20 (presente 1 volta)" << std::endl;
	assert(ms1.nocc(20) == 1);
	ms1.add(20); // Test add
	assert(ms1.size() == 9);
	assert(ms1.nocc(20) == 2);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;

	std::cout << "Elimino da ms1 il numero 5 (presente 1 volta)" << std::endl;
	assert(ms1.nocc(5) == 1);
	ms1.remove(5); // Test remove
	assert(ms1.size() == 8);
	assert(ms1.nocc(5) == 0);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;
	
	std::cout << "Elimino da ms1 il numero 11 (non presente)" << std::endl;
	assert(ms1.nocc(11) == 0);
	try {
		ms1.remove(11); // Test remove		
	}
	catch(multiset_value_not_found &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: impossibile cancellare un valore non esistente!" << std::endl;
		std::cout << std::endl;
	}
	assert(ms1.size() == 8); // La dimensione è rimasta la stessa dall'ultima operazione valida
	std::cout << "Stampa di ms1 dopo il tentativo di eliminazione proibito" << std::endl;
	std::cout << ms1 << std::endl;
	std::cout << std::endl;

	msint msempty; // Creo un oggetto MultiSet senza aggiungere valori

	std::cout << "Elimino da un MultiSet vuoto il numero 11 (non presente)" << std::endl;
	assert(msempty.size() == 0);
	assert(msempty.nocc(11) == 0);
	try {
		msempty.remove(11); // Test remove		
	}
	catch(multiset_value_not_found &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: impossibile cancellare un valore non esistente!" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "Test su ms1 di nocc() sul numero 5 (non presente)" << std::endl;
	assert(ms1.nocc(5) == 0);
	std::cout << "Occorrenze di 5: " << ms1.nocc(5) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di nocc() sul numero 4 (presente 2 volte)" << std::endl;
	assert(ms1.nocc(4) == 2);
	std::cout << "Occorrenze di 4: " << ms1.nocc(4) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di contains() sul numero 5 (non presente)" << std::endl;
	assert(ms1.contains(5) == false); //Test contains
	std::cout << "Risposta: " << ms1.contains(5) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di contains() sul numero 35 (presente 1 volta)" << std::endl;
	assert(ms1.contains(35) == true); // Test contains
	std::cout << "Risposta: " << ms1.contains(35) << std::endl;
	std::cout << std::endl;

	// Sequenza di numeri
	int a[15] = {1, 23, 44, 66, 45, 7, 44, 90, 2, 1, 7, 44, 23, 3, 90};

	msint msiter(a, a + 15); // Test creazione MultiSet da una seq. generica identificata da due iteratori generici

	std::cout << "Stampa di msiter, creato da una seq. generica identificata da una coppia di iteratori" << std::endl;
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Test di size() su msiter (size = 15)" << std::endl;
	assert(msiter.size() == 15);
	std::cout << "Risposta: " << msiter.size() << std::endl;
	std::cout << std::endl;

	std::cout << "Test su msiter di nocc() sul numero 44 (presente 3 volte)" << std::endl;
	assert(msiter.nocc(44) == 3);
	std::cout << "Occorrenze di 44: " << msiter.nocc(44) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su msiter di contains() sul numero 46 (non presente)" << std::endl;
	assert(msiter.contains(46) == false);
	std::cout << "Risposta: " << msiter.contains(46) << std::endl;
	std::cout << std::endl;

	std::cout << "Aggiungo ad msiter il numero 45 (presente 1 volta)" << std::endl;
	assert(msiter.nocc(45) == 1);
	msiter.add(45); // Test add
	assert(msiter.size() == 16);
	assert(msiter.nocc(45) == 2);
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Elimino da msiter il numero 7 (presente 2 volte)" << std::endl;
	assert(msiter.nocc(7) == 2);
	msiter.remove(7); // Test remove
	assert(msiter.size() == 15);
	assert(msiter.nocc(7) == 1);
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Stampo ms1 ed ms2" << std::endl;
	std::cout << ms1 << std::endl;
	std::cout << std::endl;
	std::cout << ms2 << std::endl;
	std::cout << std::endl;

	std::cout << "Confronto ms1 con ms2 (diversi)" << std::endl;
	bool ans = (ms1 == ms2); // Test operator==
	assert(ans == false);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	msint mscopy(ms1); // Copy constructor

	std::cout << "Confronto ms1 con un MultiSet creato con il copy constructor (uguali)" << std::endl;
	ans = (ms1 == mscopy); // Test operator==
	assert(ans == true);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	msint ms4; // Inserisco gli stessi elementi di ms1, con le stesse occorrenze
	ms4.add(20);
	ms4.add(3);
	ms4.add(3);
	ms4.add(8);
	ms4.add(35);
	ms4.add(20);
	ms4.add(4);
	ms4.add(4);

	std::cout << "Confronto ms1 con ms4, popolato tramite add di elementi (uguali)" << std::endl;
	ans = (ms1 == ms4); // Test operator==
	assert(ans == true);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	
	typename MultiSet<int, equal_int>::const_iterator i = msiter.begin(), ie = msiter.end();

	std::cout << "Stampa dei valori di msiter tramite iteratore costante" << std::endl;

	while(i != ie) {
		std::cout << *i << " "; // Test stampa con iteratore costante
		++i;
	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Test su pre-incremento iteratore illegale" << std::endl;

	try {
		++i;
	}
	catch(multiset_iterator_out_of_bounds &e) { // Test eccezione custom 
		std::cout << "Eccezione verificata: incremento iteratore fuori dai limiti" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "Test su post-incremento iteratore illegale" << std::endl;

	try {
		i++;
	}
	catch(multiset_iterator_out_of_bounds &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: incremento iteratore fuori dai limiti" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "!!!### FINE TEST DELLA CLASSE MULTISET SU INTERI ###!!!" << std::endl;
	std::cout << std::endl;
}

/**
	@brief Test della classe MultiSet su tipi double

	@description
	Questa funzione globale si occupa di effettuare alcuni test delle funzionalità della
	classe MultiSet su tipi double (MultiSet di double).
*/
void test_multiset_double() {
	std::cout << "!!!### TEST DELLA CLASSE MULTISET SU DOUBLE ###!!!" << std::endl;
	std::cout << std::endl;

	msdouble ms1; // Test costruttore di default

	std::cout << "Inserisco dei valori double nel MultiSet ms1: 1.1 2.2 3.4 1.1 17.68 17.54 3.4" << std::endl;
	std::cout << std::endl;

	// Test metodo add
	ms1.add(1.1);
	ms1.add(2.2);
	ms1.add(3.4);
	ms1.add(1.1);
	ms1.add(17.68);
	ms1.add(17.54);
	ms1.add(3.4);

	std::cout << "Stampo il MultiSet ms1" << std::endl;
	std::cout << ms1 << std::endl; // Test operator<<
	std::cout << std::endl;

	msdouble ms2(ms1); // Test copy constructor

	std::cout << "Stampo il Multiset ms2, creato con il copy constructor a partire dal MultiSet ms1" << std::endl;
	std::cout << ms2 << std::endl; // Test operator<<
	std::cout << std::endl;

	msdouble ms3 = ms1; // Test operatore di assegnamento

	std::cout << "Stampo il MultiSet ms3, creato con l'operatore di assegnamento, a partire dal MultiSet ms1" << std::endl;
	std::cout << ms3 << std::endl; // Test operator<<
	std::cout << std::endl;

	std::cout << "Test di size() su ms1 (7): " << std::endl;
	assert(ms1.size() == 7);
	std::cout << "Risposta: " << ms1.size() << std::endl; // Test size
	std::cout << std::endl;

	std::cout << "Test di size() su ms2 (7): " << std::endl;
	assert(ms2.size() == 7);
	std::cout << "Risposta: " << ms2.size() << std::endl; // Test size
	std::cout << std::endl;

	std::cout << "Test di size() su ms3 (7): " << std::endl;
	assert(ms3.size() == 7);
	std::cout << "Risposta: " << ms3.size() << std::endl; // Test size
	std::cout << std::endl;

	std::cout << "Aggiungo ad ms1 il numero 35.99 (non presente)" << std::endl;
	assert(ms1.nocc(35.99) == 0);
	ms1.add(35.99); // Test add
	assert(ms1.size() == 8);
	assert(ms1.nocc(35.99) == 1);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;
	
	std::cout << "Aggiungo ad ms1 il numero 17.54 (presente 1 volta)" << std::endl;
	assert(ms1.nocc(17.54) == 1);
	ms1.add(17.54); // Test add
	assert(ms1.size() == 9);
	assert(ms1.nocc(17.54) == 2);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;

	std::cout << "Elimino da ms1 il numero 17.68 (presente 1 volta)" << std::endl;
	assert(ms1.nocc(17.68) == 1);
	ms1.remove(17.68); // Test remove
	assert(ms1.size() == 8);
	assert(ms1.nocc(17.68) == 0);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;

	std::cout << "Elimino da ms1 il numero 11.1 (non presente)" << std::endl;
	assert(ms1.nocc(11.1) == 0);
	try {
		ms1.remove(11.1); // Test remove		
	}
	catch(multiset_value_not_found &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: impossibile cancellare un valore non esistente!" << std::endl;
		std::cout << std::endl;
	}
	assert(ms1.size() == 8); // La dimensione è rimasta la stessa dall'ultima operazione valida
	std::cout << "Stampa di ms1 dopo il tentativo di eliminazione proibito" << std::endl;
	std::cout << ms1 << std::endl;
	std::cout << std::endl;

	msdouble msempty; // Creo un oggetto MultiSet senza aggiungere valori

	std::cout << "Elimino da un MultiSet vuoto il numero 11.1 (non presente)" << std::endl;
	assert(msempty.size() == 0);
	assert(msempty.nocc(11.1) == 0);
	try {
		msempty.remove(11.1); // Test remove		
	}
	catch(multiset_value_not_found &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: impossibile cancellare un valore non esistente!" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "Test su ms1 di nocc() sul numero 5.17 (non presente)" << std::endl;
	assert(ms1.nocc(5.17) == 0);
	std::cout << "Occorrenze di 5.17: " << ms1.nocc(5.17) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di nocc() sul numero 3.4 (presente 2 volte)" << std::endl;
	assert(ms1.nocc(3.4) == 2);
	std::cout << "Occorrenze di 3.4: " << ms1.nocc(3.4) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di contains() sul numero 1.2 (non presente)" << std::endl;
	assert(ms1.contains(1.2) == false); //Test contains
	std::cout << "Risposta: " << ms1.contains(1.2) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di contains() sul numero 35.99 (presente 1 volta)" << std::endl;
	assert(ms1.contains(35.99) == true); // Test contains
	std::cout << "Risposta: " << ms1.contains(35.99) << std::endl;
	std::cout << std::endl;

	// Sequenza di numeri
	double a[5] = {3.14, 4.99, 67.88, 1.1, 3.14};

	msdouble msiter(a, a + 5); // Test creazione MultiSet da una seq. generica identificata da due iteratori generici

	std::cout << "Stampa di msiter, creato da una seq. generica identificata da una coppia di iteratori" << std::endl;
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Test di size() su msiter (size = 5)" << std::endl;
	assert(msiter.size() == 5);
	std::cout << "Risposta: " << msiter.size() << std::endl;
	std::cout << std::endl;

	std::cout << "Test su msiter di nocc() sul numero 3.14 (presente 2 volte)" << std::endl;
	assert(msiter.nocc(3.14) == 2);
	std::cout << "Occorrenze di 3.14: " << msiter.nocc(3.14) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su msiter di contains() sul numero 67.45 (non presente)" << std::endl;
	assert(msiter.contains(67.45) == false);
	std::cout << "Risposta: " << msiter.contains(67.45) << std::endl;
	std::cout << std::endl;

	std::cout << "Aggiungo ad msiter il numero 67.88 (presente 1 volta)" << std::endl;
	assert(msiter.nocc(67.88) == 1);
	msiter.add(67.88); // Test add
	assert(msiter.size() == 6);
	assert(msiter.nocc(67.88) == 2);
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Elimino da msiter il numero 3.14 (presente 2 volte)" << std::endl;
	assert(msiter.nocc(3.14) == 2);
	msiter.remove(3.14); // Test remove
	assert(msiter.size() == 5);
	assert(msiter.nocc(3.14) == 1);
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Stampo ms1 ed ms2" << std::endl;
	std::cout << ms1 << std::endl;
	std::cout << std::endl;
	std::cout << ms2 << std::endl;
	std::cout << std::endl;

	std::cout << "Confronto ms1 con ms2 (diversi)" << std::endl;
	bool ans = (ms1 == ms2); // Test operator==
	assert(ans == false);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	msdouble mscopy(ms1); // Copy constructor

	std::cout << "Confronto ms1 con un MultiSet creato con il copy constructor (uguali)" << std::endl;
	ans = (ms1 == mscopy); // Test operator==
	assert(ans == true);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	msdouble ms4; // Inserisco gli stessi elementi di ms1, con le stesse occorrenze
	ms4.add(2.2);
	ms4.add(3.4);
	ms4.add(1.1);
	ms4.add(1.1);
	ms4.add(3.4);
	ms4.add(17.54);
	ms4.add(17.54);
	ms4.add(35.99);

	std::cout << "Confronto ms1 con ms4, popolato tramite add di elementi (uguali)" << std::endl;
	ans = (ms1 == ms4); // Test operator==
	assert(ans == true);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	typename MultiSet<double, equal_double>::const_iterator i = msiter.begin(), ie = msiter.end();

	std::cout << "Stampa dei valori di msiter tramite iteratore costante" << std::endl;

	while(i != ie) {
		std::cout << *i << " "; // Stampa con iteratore costante
		++i;
	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Test su pre-incremento iteratore illegale" << std::endl;

	try {
		++i;
	}
	catch(multiset_iterator_out_of_bounds &e) { // Test su eccezione custom
		std::cout << "Eccezione verificata: incremento iteratore fuori dai limiti" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "Test su post-incremento iteratore illegale" << std::endl;

	try {
		i++;
	}
	catch(multiset_iterator_out_of_bounds &e) { // Test su eccezione custom
		std::cout << "Eccezione verificata: incremento iteratore fuori dai limiti" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "!!!### FINE TEST DELLA CLASSE MULTISET SU DOUBLE ###!!!" << std::endl;
	std::cout << std::endl;
}

/**
	@brief Test della classe MultiSet su tipi std::string

	@description
	Questa funzione globale si occupa di effettuare alcuni test delle funzionalità della
	classe MultiSet su tipi std::string (MultiSet di std::string).
*/
void test_multiset_string() {
	std::cout << "!!!### TEST DELLA CLASSE MULTISET SU STRINGHE ###!!!" << std::endl;
	std::cout << std::endl;

	msstr ms1; // Test costruttore di default

	std::cout << "Inserisco delle stringhe nel MultiSet ms1: ciao hello salut nihao ciao hello hola" << std::endl;
	std::cout << std::endl;

	// Test metodo add
	ms1.add("ciao");
	ms1.add("hello");
	ms1.add("salut");
	ms1.add("nihao");
	ms1.add("ciao");
	ms1.add("hello");
	ms1.add("hola");

	std::cout << "Stampo il MultiSet ms1" << std::endl;
	std::cout << ms1 << std::endl; // Test operator<<
	std::cout << std::endl;

	msstr ms2(ms1); // Test copy constructor

	std::cout << "Stampo il Multiset ms2, creato con il copy constructor a partire dal MultiSet ms1" << std::endl;
	std::cout << ms2 << std::endl; // Test operator<<
	std::cout << std::endl;

	msstr ms3 = ms1; // Test operatore di assegnamento

	std::cout << "Stampo il MultiSet ms3, creato con l'operatore di assegnamento, a partire dal MultiSet ms1" << std::endl;
	std::cout << ms3 << std::endl; // Test operator<<
	std::cout << std::endl;

	std::cout << "Test di size() su ms1 (7): " << std::endl;
	assert(ms1.size() == 7);
	std::cout << "Risposta: " << ms1.size() << std::endl; // Test size
	std::cout << std::endl;

	std::cout << "Test di size() su ms2 (7): " << std::endl;
	assert(ms2.size() == 7);
	std::cout << "Risposta: " << ms2.size() << std::endl; // Test size
	std::cout << std::endl;

	std::cout << "Test di size() su ms3 (7): " << std::endl;
	assert(ms3.size() == 7);
	std::cout << "Risposta: " << ms3.size() << std::endl; // Test size
	std::cout << std::endl;

	std::cout << "Aggiungo ad ms1 la stringa hallo (non presente)" << std::endl;
	assert(ms1.nocc("hallo") == 0);
	ms1.add("hallo"); // Test add
	assert(ms1.size() == 8);
	assert(ms1.nocc("hallo") == 1);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;
	
	std::cout << "Aggiungo ad ms1 la stringa nihao (presente 1 volta)" << std::endl;
	assert(ms1.nocc("nihao") == 1);
	ms1.add("nihao"); // Test add
	assert(ms1.size() == 9);
	assert(ms1.nocc("nihao") == 2);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;

	std::cout << "Elimino da ms1 la stringa hola (presente 1 volta)" << std::endl;
	assert(ms1.nocc("hola") == 1);
	ms1.remove("hola"); // Test remove
	assert(ms1.size() == 8);
	assert(ms1.nocc("hola") == 0);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;

	std::cout << "Elimino da ms1 la stringa hola (non presente)" << std::endl;
	assert(ms1.nocc("hola") == 0);
	try {
		ms1.remove("hola"); // Test remove		
	}
	catch(multiset_value_not_found &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: impossibile cancellare un valore non esistente!" << std::endl;
		std::cout << std::endl;
	}
	assert(ms1.size() == 8); // La dimensione è rimasta la stessa dall'ultima operazione valida
	std::cout << "Stampa di ms1 dopo il tentativo di eliminazione proibito" << std::endl;
	std::cout << ms1 << std::endl;
	std::cout << std::endl;

	msstr msempty; // Creo un oggetto MultiSet senza aggiungere valori

	std::cout << "Elimino da un MultiSet vuoto la stringa hola (non presente)" << std::endl;
	assert(msempty.size() == 0);
	assert(msempty.nocc("hola") == 0);
	try {
		msempty.remove("hola"); // Test remove		
	}
	catch(multiset_value_not_found &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: impossibile cancellare un valore non esistente!" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "Test su ms1 di nocc() della stringa hola (non presente)" << std::endl;
	assert(ms1.nocc("hola") == 0);
	std::cout << "Occorrenze di hola: " << ms1.nocc("hola") << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di nocc() della stringa ciao (presente 2 volte)" << std::endl;
	assert(ms1.nocc("ciao") == 2);
	std::cout << "Occorrenze di ciao: " << ms1.nocc("ciao") << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di contains() della stringa ave (non presente)" << std::endl;
	assert(ms1.contains("ave") == false); //Test contains
	std::cout << "Risposta: " << ms1.contains("ave") << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di contains() della stringa salut (presente 1 volta)" << std::endl;
	assert(ms1.contains("salut") == true); // Test contains
	std::cout << "Risposta: " << ms1.contains("salut") << std::endl;
	std::cout << std::endl;

	// Sequenza di stringhe
	std::string a[10] = {"rosa", "giallo", "blu", "rosso", "rosa", "giallo", "bianco", "nero", "nero", "blu"};

	msstr msiter(a, a + 10); // Test creazione MultiSet da una seq. generica identificata da due iteratori generici

	std::cout << "Stampa di msiter, creato da una seq. generica identificata da una coppia di iteratori" << std::endl;
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Test di size() su msiter (size = 10)" << std::endl;
	assert(msiter.size() == 10);
	std::cout << "Risposta: " << msiter.size() << std::endl;
	std::cout << std::endl;

	std::cout << "Test su msiter di nocc() della stringa rosa (presente 2 volte)" << std::endl;
	assert(msiter.nocc("rosa") == 2);
	std::cout << "Occorrenze di rosa: " << msiter.nocc("rosa") << std::endl;
	std::cout << std::endl;

	std::cout << "Test su msiter di contains() della stringa verde (non presente)" << std::endl;
	assert(msiter.contains("verde") == false);
	std::cout << "Risposta: " << msiter.contains("verde") << std::endl;
	std::cout << std::endl;

	std::cout << "Aggiungo ad msiter la stringa bianco (presente 1 volta)" << std::endl;
	assert(msiter.nocc("bianco") == 1);
	msiter.add("bianco"); // Test add
	assert(msiter.size() == 11);
	assert(msiter.nocc("bianco") == 2);
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Elimino da msiter la stringa rosa (presente 2 volte)" << std::endl;
	assert(msiter.nocc("rosa") == 2);
	msiter.remove("rosa"); // Test remove
	assert(msiter.size() == 10);
	assert(msiter.nocc("rosa") == 1);
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Stampo ms1 ed ms2" << std::endl;
	std::cout << ms1 << std::endl;
	std::cout << std::endl;
	std::cout << ms2 << std::endl;
	std::cout << std::endl;

	std::cout << "Confronto ms1 con ms2 (diversi)" << std::endl;
	bool ans = (ms1 == ms2); // Test operator==
	assert(ans == false);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	msstr mscopy(ms1); // Copy constructor

	std::cout << "Confronto ms1 con un MultiSet creato con il copy constructor (uguali)" << std::endl;
	ans = (ms1 == mscopy); // Test operator==
	assert(ans == true);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	msstr ms4; // Inserisco gli stessi elementi di ms1, con le stesse occorrenze
	ms4.add("ciao");
	ms4.add("ciao");
	ms4.add("hello");
	ms4.add("salut");
	ms4.add("hello");
	ms4.add("hallo");
	ms4.add("nihao");
	ms4.add("nihao");

	std::cout << "Confronto ms1 con ms4, popolato tramite add di elementi (uguali)" << std::endl;
	ans = (ms1 == ms4); // Test operator==
	assert(ans == true);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	typename MultiSet<std::string, equal_string>::const_iterator i = msiter.begin(), ie = msiter.end();

	std::cout << "Stampa dei valori di msiter tramite iteratore costante" << std::endl;

	while(i != ie) {
		std::cout << *i << " "; // Stampa con iteratore costante
		++i;
	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Test su pre-incremento iteratore illegale" << std::endl;

	try {
		++i;
	}
	catch(multiset_iterator_out_of_bounds &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: incremento iteratore fuori dai limiti" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "Test su post-incremento iteratore illegale" << std::endl;

	try {
		i++;
	}
	catch(multiset_iterator_out_of_bounds &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: incremento iteratore fuori dai limiti" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "!!!### FINE TEST DELLA CLASSE MULTISET SU STRINGHE ###!!!" << std::endl;
	std::cout << std::endl;
}

/**
	@brief Test della classe MultiSet su tipi point

	@description
	Questa funzione globale si occupa di effettuare alcuni test delle funzionalità della
	classe MultiSet su tipi custom point (MultiSet di point).
*/
void test_multiset_point() {
	std::cout << "!!!### TEST DELLA CLASSE MULTISET SU POINT ###!!!" << std::endl;
	std::cout << std::endl;

	mspoint ms1; // Test costruttore di default

	std::cout << "Inserisco dei point nel MultiSet ms1: (1,1) (0,0) (1,2) (0,0) (2,4) (0,20) (2,4)" << std::endl;
	std::cout << std::endl;

	// Test metodo add
	ms1.add(point(1,1));
	ms1.add(point(0,0));
	ms1.add(point(1,2));
	ms1.add(point(0,0));
	ms1.add(point(2,4));
	ms1.add(point(0,20));
	ms1.add(point(2,4));

	std::cout << "Stampo il MultiSet ms1" << std::endl;
	std::cout << ms1 << std::endl; // Test operator<<
	std::cout << std::endl;

	mspoint ms2(ms1); // Test copy constructor

	std::cout << "Stampo il Multiset ms2, creato con il copy constructor a partire dal MultiSet ms1" << std::endl;
	std::cout << ms2 << std::endl; // Test operator<<
	std::cout << std::endl;

	mspoint ms3 = ms1; // Test operatore di assegnamento

	std::cout << "Stampo il MultiSet ms3, creato con l'operatore di assegnamento, a partire dal MultiSet ms1" << std::endl;
	std::cout << ms3 << std::endl; // Test operator<<
	std::cout << std::endl;

	std::cout << "Test di size() su ms1 (7): " << std::endl;
	assert(ms1.size() == 7);
	std::cout << "Risposta: " << ms1.size() << std::endl; // Test size
	std::cout << std::endl;

	std::cout << "Test di size() su ms2 (7): " << std::endl;
	assert(ms2.size() == 7);
	std::cout << "Risposta: " << ms2.size() << std::endl; // Test size
	std::cout << std::endl;

	std::cout << "Test di size() su ms3 (7): " << std::endl;
	assert(ms3.size() == 7);
	std::cout << "Risposta: " << ms3.size() << std::endl; // Test size
	std::cout << std::endl;

	std::cout << "Aggiungo ad ms1 il punto (4,4) (non presente)" << std::endl;
	assert(ms1.nocc(point(4,4)) == 0);
	ms1.add(point(4,4)); // Test add
	assert(ms1.size() == 8);
	assert(ms1.nocc(point(4,4)) == 1);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;
	
	std::cout << "Aggiungo ad ms1 il punto (0,20) (presente 1 volta)" << std::endl;
	assert(ms1.nocc(point(0,20)) == 1);
	ms1.add(point(0,20)); // Test add
	assert(ms1.size() == 9);
	assert(ms1.nocc(point(0,20)) == 2);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;

	std::cout << "Elimino da ms1 il punto (2,4) (presente 2 volte)" << std::endl;
	assert(ms1.nocc(point(2,4)) == 2);
	ms1.remove(point(2,4)); // Test remove
	assert(ms1.size() == 8);
	assert(ms1.nocc(point(2,4)) == 1);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;

	std::cout << "Elimino da ms1 il punto (8,8) (non presente)" << std::endl;
	assert(ms1.nocc(point(8,8)) == 0);
	try {
		ms1.remove(point(8,8)); // Test remove		
	}
	catch(multiset_value_not_found &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: impossibile cancellare un valore non esistente!" << std::endl;
		std::cout << std::endl;
	}
	assert(ms1.size() == 8); // La dimensione è rimasta la stessa dall'ultima operazione valida
	std::cout << "Stampa di ms1 dopo il tentativo di eliminazione proibito" << std::endl;
	std::cout << ms1 << std::endl;
	std::cout << std::endl;

	mspoint msempty; // Creo un oggetto MultiSet senza aggiungere valori

	std::cout << "Elimino da un MultiSet vuoto il punto (8,8) (non presente)" << std::endl;
	assert(msempty.size() == 0);
	assert(msempty.nocc(point(8,8)) == 0);
	try {
		msempty.remove(point(8,8)); // Test remove		
	}
	catch(multiset_value_not_found &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: impossibile cancellare un valore non esistente!" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "Test su ms1 di nocc() del punto (17,0) (non presente)" << std::endl;
	assert(ms1.nocc(point(17,0)) == 0);
	std::cout << "Occorrenze di (17,0): " << ms1.nocc(point(17,0)) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di nocc() del punto (0,20) (presente 2 volte)" << std::endl;
	assert(ms1.nocc(point(0,20)) == 2);
	std::cout << "Occorrenze di (0,20): " << ms1.nocc(point(0,20)) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di contains() del punto (33,17) (non presente)" << std::endl;
	assert(ms1.contains(point(33,17)) == false); //Test contains
	std::cout << "Risposta: " << ms1.contains(point(33,17)) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di contains() del punto (0,20) (presente 2 volte)" << std::endl;
	assert(ms1.contains(point(0,20)) == true); // Test contains
	std::cout << "Risposta: " << ms1.contains(point(0,20)) << std::endl;
	std::cout << std::endl;

	// Sequenza di point
	point a[5] = {point(0,1), point(0,2), point(0,3), point(0,1), point(0,0)};

	mspoint msiter(a, a + 5); // Test creazione MultiSet da una seq. generica identificata da due iteratori generici

	std::cout << "Stampa di msiter, creato da una seq. generica identificata da una coppia di iteratori" << std::endl;
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Test di size() su msiter (size = 5)" << std::endl;
	assert(msiter.size() == 5);
	std::cout << "Risposta: " << msiter.size() << std::endl;
	std::cout << std::endl;

	std::cout << "Test su msiter di nocc() del punto (0,1) (presente 2 volte)" << std::endl;
	assert(msiter.nocc(point(0,1)) == 2);
	std::cout << "Occorrenze di (0,1): " << msiter.nocc(point(0,1)) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su msiter di contains() del punto (0,5) (non presente)" << std::endl;
	assert(msiter.contains(point(0,5)) == false);
	std::cout << "Risposta: " << msiter.contains(point(0,5)) << std::endl;
	std::cout << std::endl;

	std::cout << "Aggiungo ad msiter il punto (0,0) (presente 1 volta)" << std::endl;
	assert(msiter.nocc(point(0,0)) == 1);
	msiter.add(point(0,0)); // Test add
	assert(msiter.size() == 6);
	assert(msiter.nocc(point(0,0)) == 2);
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Elimino da msiter il punto (0,0) (presente 2 volte)" << std::endl;
	assert(msiter.nocc(point(0,0)) == 2);
	msiter.remove(point(0,0)); // Test remove
	assert(msiter.size() == 5);
	assert(msiter.nocc(point(0,0)) == 1);
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Stampo ms1 ed ms2" << std::endl;
	std::cout << ms1 << std::endl;
	std::cout << std::endl;
	std::cout << ms2 << std::endl;
	std::cout << std::endl;

	std::cout << "Confronto ms1 con ms2 (diversi)" << std::endl;
	bool ans = (ms1 == ms2); // Test operator==
	assert(ans == false);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	mspoint mscopy(ms1); // Copy constructor

	std::cout << "Confronto ms1 con un MultiSet creato con il copy constructor (uguali)" << std::endl;
	ans = (ms1 == mscopy); // Test operator==
	assert(ans == true);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	mspoint ms4; // Inserisco gli stessi elementi di ms1, con le stesse occorrenze
	ms4.add(point(1,1));
	ms4.add(point(0,0));
	ms4.add(point(1,2));
	ms4.add(point(0,0));
	ms4.add(point(2,4));
	ms4.add(point(0,20));
	ms4.add(point(4,4));
	ms4.add(point(0,20));

	std::cout << "Confronto ms1 con ms4, popolato tramite add di elementi (uguali)" << std::endl;
	ans = (ms1 == ms4); // Test operator==
	assert(ans == true);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	typename MultiSet<point, equal_point>::const_iterator i = msiter.begin(), ie = msiter.end();

	std::cout << "Stampa dei valori di msiter tramite iteratore costante" << std::endl;

	while(i != ie) {
		std::cout << *i << " "; // Stampa con iteratore costante
		++i;
	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Test su pre-incremento iteratore illegale" << std::endl;

	try {
		++i;
	}
	catch(multiset_iterator_out_of_bounds &e) { // Test su eccezione custom
		std::cout << "Eccezione verificata: incremento iteratore fuori dai limiti" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "Test su post-incremento iteratore illegale" << std::endl;

	try {
		i++;
	}
	catch(multiset_iterator_out_of_bounds &e) { // Test su eccezione custom
		std::cout << "Eccezione verificata: incremento iteratore fuori dai limiti" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "!!!### FINE TEST DELLA CLASSE MULTISET SU POINT ###!!!" << std::endl;
	std::cout << std::endl;
}

/**
	@brief Test della classe MultiSet su tipi person

	@description
	Questa funzione globale si occupa di effettuare alcuni test delle funzionalità della
	classe MultiSet su tipi custom person (MultiSet di person).
*/
void test_multiset_person() {
	std::cout << "!!!### TEST DELLA CLASSE MULTISET SU PERSON ###!!!" << std::endl;
	std::cout << std::endl;

	msperson ms1; // Test costruttore di default

	std::cout << "Inserisco delle persone nel MultiSet ms1: " << std::endl;
	std::cout << "[Andrea Bianchi, 35], [Giovanni Verdi, 50], [Mario Rossi, 45], [Giovanni Verdi, 50], [Giovanni Verdi, 17], [Mario Rossi, 45], [Filippo Neri, 20]" << std::endl;
	std::cout << std::endl;

	// Test metodo add
	ms1.add(person("Andrea", "Bianchi", 35));
	ms1.add(person("Giovanni", "Verdi", 50));
	ms1.add(person("Mario", "Rossi", 45));
	ms1.add(person("Giovanni", "Verdi", 50));
	ms1.add(person("Giovanni", "Verdi", 17));
	ms1.add(person("Mario", "Rossi", 45));
	ms1.add(person("Filippo", "Neri", 20));

	std::cout << "Stampo il MultiSet ms1" << std::endl;
	std::cout << ms1 << std::endl; // Test operator<<
	std::cout << std::endl;

	msperson ms2(ms1); // Test copy constructor

	std::cout << "Stampo il Multiset ms2, creato con il copy constructor a partire dal MultiSet ms1" << std::endl;
	std::cout << ms2 << std::endl; // Test operator<<
	std::cout << std::endl;

	msperson ms3 = ms1; // Test operatore di assegnamento

	std::cout << "Stampo il MultiSet ms3, creato con l'operatore di assegnamento, a partire dal MultiSet ms1" << std::endl;
	std::cout << ms3 << std::endl; // Test operator<<
	std::cout << std::endl;

	std::cout << "Test di size() su ms1 (7): " << std::endl;
	assert(ms1.size() == 7);
	std::cout << "Risposta: " << ms1.size() << std::endl; // Test size
	std::cout << std::endl;

	std::cout << "Test di size() su ms2 (7): " << std::endl;
	assert(ms2.size() == 7);
	std::cout << "Risposta: " << ms2.size() << std::endl; // Test size
	std::cout << std::endl;

	std::cout << "Test di size() su ms3 (7): " << std::endl;
	assert(ms3.size() == 7);
	std::cout << "Risposta: " << ms3.size() << std::endl; // Test size
	std::cout << std::endl;

	std::cout << "Aggiungo ad ms1 la persona [Andrea Bianchi, 37] (non presente)" << std::endl;
	assert(ms1.nocc(person("Andrea", "Bianchi", 37)) == 0);
	ms1.add(person("Andrea", "Bianchi", 37)); // Test add
	assert(ms1.size() == 8);
	assert(ms1.nocc(person("Andrea", "Bianchi", 37)) == 1);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;
	
	std::cout << "Aggiungo ad ms1 la persona [Giovanni Verdi, 17] (presente 1 volta)" << std::endl;
	assert(ms1.nocc(person("Giovanni", "Verdi", 17)) == 1);
	ms1.add(person("Giovanni", "Verdi", 17)); // Test add
	assert(ms1.size() == 9);
	assert(ms1.nocc(person("Giovanni", "Verdi", 17)) == 2);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;

	std::cout << "Elimino da ms1 la persona [Giovanni Verdi, 50] (presente 2 volte)" << std::endl;
	assert(ms1.nocc(person("Giovanni", "Verdi", 50)) == 2);
	ms1.remove(person("Giovanni", "Verdi", 50)); // Test remove
	assert(ms1.size() == 8);
	assert(ms1.nocc(person("Giovanni", "Verdi", 50)) == 1);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;

	std::cout << "Elimino da ms1 la persona [Pinco Pallino, 90] (non presente)" << std::endl;
	assert(ms1.nocc(person("Pinco", "Pallino", 90)) == 0);
	try {
		ms1.remove(person("Pinco", "Pallino", 90)); // Test remove		
	}
	catch(multiset_value_not_found &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: impossibile cancellare un valore non esistente!" << std::endl;
		std::cout << std::endl;
	}
	assert(ms1.size() == 8); // La dimensione è rimasta la stessa dall'ultima operazione valida
	std::cout << "Stampa di ms1 dopo il tentativo di eliminazione proibito" << std::endl;
	std::cout << ms1 << std::endl;
	std::cout << std::endl;

	msperson msempty; // Creo un oggetto MultiSet senza aggiungere valori

	std::cout << "Elimino da un MultiSet vuoto la persona [Pinco Pallino, 90] (non presente)" << std::endl;
	assert(msempty.size() == 0);
	assert(msempty.nocc(person("Pinco", "Pallino", 90)) == 0);
	try {
		msempty.remove(person("Pinco", "Pallino", 90)); // Test remove		
	}
	catch(multiset_value_not_found &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: impossibile cancellare un valore non esistente!" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "Test su ms1 di nocc() della persona [Pinco Pallino, 90] (non presente)" << std::endl;
	assert(ms1.nocc(person("Pinco", "Pallino", 90)) == 0);
	std::cout << "Occorrenze di [Pinco Pallino, 90]: " << ms1.nocc(person("Pinco", "Pallino", 90)) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di nocc() della persona [Giovanni Verdi, 17] (presente 2 volte)" << std::endl;
	assert(ms1.nocc(person("Giovanni", "Verdi", 17)) == 2);
	std::cout << "Occorrenze di [Giovanni Verdi, 17]: " << ms1.nocc(person("Giovanni", "Verdi", 17)) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di contains() della persona [Andrea Neri, 21] (non presente)" << std::endl;
	assert(ms1.contains(person("Andrea", "Neri", 21)) == false); //Test contains
	std::cout << "Risposta: " << ms1.contains(person("Andrea", "Neri", 21)) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di contains() della persona [Filippo Neri, 20] (presente 1 volta)" << std::endl;
	assert(ms1.contains(person("Filippo", "Neri", 20)) == true); // Test contains
	std::cout << "Risposta: " << ms1.contains(person("Filippo", "Neri", 20)) << std::endl;
	std::cout << std::endl;

	// Sequenza di point
	person a[5] = {person("Henry", "Ford", 17), person("Albert", "Einstein", 45), person("David", "Bowie", 10), person("Albert", "Einstein", 45), person("David", "Bowie", 10)};

	msperson msiter(a, a + 5); // Test creazione MultiSet da una seq. generica identificata da due iteratori generici

	std::cout << "Stampa di msiter, creato da una seq. generica identificata da una coppia di iteratori" << std::endl;
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Test di size() su msiter (size = 5)" << std::endl;
	assert(msiter.size() == 5);
	std::cout << "Risposta: " << msiter.size() << std::endl;
	std::cout << std::endl;

	std::cout << "Test su msiter di nocc() della persona [David Bowie, 10] (presente 2 volte)" << std::endl;
	assert(msiter.nocc(person("David", "Bowie", 10)) == 2);
	std::cout << "Occorrenze di [David Bowie, 10]: " << msiter.nocc(person("David", "Bowie", 10)) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su msiter di contains() della persona [Pinco Pallino, 90] (non presente)" << std::endl;
	assert(msiter.contains(person("Pinco", "Pallino", 90)) == false);
	std::cout << "Risposta: " << msiter.contains(person("Pinco", "Pallino", 90)) << std::endl;
	std::cout << std::endl;

	std::cout << "Aggiungo ad msiter la persona [Henry Ford, 17] (presente 1 volta)" << std::endl;
	assert(msiter.nocc(person("Henry", "Ford", 17)) == 1);
	msiter.add(person("Henry", "Ford", 17)); // Test add
	assert(msiter.size() == 6);
	assert(msiter.nocc(person("Henry", "Ford", 17)) == 2);
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Elimino da msiter la persona [David Bowie, 10] (presente 2 volte)" << std::endl;
	assert(msiter.nocc(person("David", "Bowie", 10)) == 2);
	msiter.remove(person("David", "Bowie", 10)); // Test remove
	assert(msiter.size() == 5);
	assert(msiter.nocc(person("David", "Bowie", 10)) == 1);
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Stampo ms1 ed ms2" << std::endl;
	std::cout << ms1 << std::endl;
	std::cout << std::endl;
	std::cout << ms2 << std::endl;
	std::cout << std::endl;

	std::cout << "Confronto ms1 con ms2 (diversi)" << std::endl;
	bool ans = (ms1 == ms2); // Test operator==
	assert(ans == false);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	msperson mscopy(ms1); // Copy constructor

	std::cout << "Confronto ms1 con un MultiSet creato con il copy constructor (uguali)" << std::endl;
	ans = (ms1 == mscopy); // Test operator==
	assert(ans == true);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	msperson ms4; // Inserisco gli stessi elementi di ms1, con le stesse occorrenze
	ms4.add(person("Andrea", "Bianchi", 37));
	ms4.add(person("Andrea", "Bianchi", 35));
	ms4.add(person("Giovanni", "Verdi", 50));
	ms4.add(person("Mario", "Rossi", 45));
	ms4.add(person("Mario", "Rossi", 45));
	ms4.add(person("Giovanni", "Verdi", 17));
	ms4.add(person("Giovanni", "Verdi", 17));
	ms4.add(person("Filippo", "Neri", 20));

	std::cout << "Confronto ms1 con ms4, popolato tramite add di elementi (uguali)" << std::endl;
	ans = (ms1 == ms4); // Test operator==
	assert(ans == true);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	typename MultiSet<person, equal_person>::const_iterator i = msiter.begin(), ie = msiter.end();

	std::cout << "Stampa dei valori di msiter tramite iteratore costante" << std::endl;

	while(i != ie) {
		std::cout << *i << " "; // Stampa con iteratore costante
		++i;
	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Test su pre-incremento iteratore illegale" << std::endl;

	try {
		++i;
	}
	catch(multiset_iterator_out_of_bounds &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: incremento iteratore fuori dai limiti" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "Test su post-incremento iteratore illegale" << std::endl;

	try {
		i++;
	}
	catch(multiset_iterator_out_of_bounds &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: incremento iteratore fuori dai limiti" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "!!!### FINE TEST DELLA CLASSE MULTISET SU PERSON ###!!!" << std::endl;
	std::cout << std::endl;
}

/**
	@brief Test della classe MultiSet su tipi MultiSet di point

	@description
	Questa funzione globale si occupa di effettuare alcuni test delle funzionalità della
	classe MultiSet su tipi custom MultiSet di point (MultiSet di MultiSet di point).
*/
void test_multiset_multiset_point() {
	std::cout << "!!!### TEST DELLA CLASSE MULTISET SU MULTISET DI POINT ###!!!" << std::endl;
	std::cout << std::endl;

	ms_mspoint ms1; // Test costruttore di default

	mspoint msa, msb, msc; // MultiSet di point da aggiungere

	msa.add(point(1,1));
	msa.add(point(0,0));
	msa.add(point(1,2));

	msb.add(point(0,0));
	msb.add(point(2,4));
	msb.add(point(0,20));

	msc.add(point(2,4));
	msc.add(point(2,4));

	mspoint msd(msa); // Copy constructor
	msd.add(point(0,0));


	std::cout << "Inserisco dei MultiSet point nel MultiSet ms1:" << std::endl;
	std::cout << "{(1,1), (0,0), (1,2)}" << std::endl; // msa
	std::cout << "{(0,0), (2,4), (0,20)}" << std::endl; // msb
	std::cout << "{(2,4), (2,4)}" << std::endl; // msc
	std::cout << "{(1,1), (0,0), (1,2), (0,0)}" << std::endl; // msd
	std::cout << "{(2,4), (2,4)}" << std::endl; // msc
	std::cout << "{(2,4), (2,4)}" << std::endl; // msc
	std::cout << "{(1,1), (0,0), (1,2)}" << std::endl; // msa
	std::cout << std::endl;

	// Test metodo add
	ms1.add(msa);
	ms1.add(msb);
	ms1.add(msc);
	ms1.add(msd);
	ms1.add(msc);
	ms1.add(msc);
	ms1.add(msa);

	std::cout << "Stampo il MultiSet ms1" << std::endl;
	std::cout << ms1 << std::endl; // Test operator<<
	std::cout << std::endl;

	ms_mspoint ms2(ms1); // Test copy constructor

	std::cout << "Stampo il Multiset ms2, creato con il copy constructor a partire dal MultiSet ms1" << std::endl;
	std::cout << ms2 << std::endl; // Test operator<<
	std::cout << std::endl;

	ms_mspoint ms3 = ms1; // Test operatore di assegnamento

	std::cout << "Stampo il MultiSet ms3, creato con l'operatore di assegnamento, a partire dal MultiSet ms1" << std::endl;
	std::cout << ms3 << std::endl; // Test operator<<
	std::cout << std::endl;

	std::cout << "Test di size() su ms1 (7): " << std::endl;
	assert(ms1.size() == 7);
	std::cout << "Risposta: " << ms1.size() << std::endl; // Test size
	std::cout << std::endl;

	std::cout << "Test di size() su ms2 (7): " << std::endl;
	assert(ms2.size() == 7);
	std::cout << "Risposta: " << ms2.size() << std::endl; // Test size
	std::cout << std::endl;

	std::cout << "Test di size() su ms3 (7): " << std::endl;
	assert(ms3.size() == 7);
	std::cout << "Risposta: " << ms3.size() << std::endl; // Test size
	std::cout << std::endl;

	mspoint mstemp;
	mstemp.add(point(4,4));
	mstemp.add(point(8,8));

	std::cout << "Aggiungo ad ms1 il MultiSet {(4,4), (8,8)} (non presente)" << std::endl;
	assert(ms1.nocc(mstemp) == 0);
	ms1.add(mstemp); // Test add
	assert(ms1.size() == 8);
	assert(ms1.nocc(mstemp) == 1);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;
	
	std::cout << "Aggiungo ad ms1 il MultiSet {(2,4), (2,4)} (presente 3 volte)" << std::endl;
	assert(ms1.nocc(msc) == 3);
	ms1.add(msc); // Test add
	assert(ms1.size() == 9);
	assert(ms1.nocc(msc) == 4);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;

	std::cout << "Elimino da ms1 il MultiSet {(1,1), (0,0), (1,2)} (presente 2 volte)" << std::endl;
	assert(ms1.nocc(msa) == 2);
	ms1.remove(msa); // Test remove
	assert(ms1.size() == 8);
	assert(ms1.nocc(msa) == 1);
	std::cout << ms1 << std::endl;
	std::cout << std::endl;

	mspoint mstemp2;
	mstemp2.add(point(0,0));
	mstemp2.add(point(1,3));
	mstemp2.add(point(12,21));

	std::cout << "Elimino da ms1 il MultiSet {(0,0), (1,3), (12,21)} (non presente)" << std::endl;
	assert(ms1.nocc(mstemp2) == 0);
	try {
		ms1.remove(mstemp2); // Test remove		
	}
	catch(multiset_value_not_found &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: impossibile cancellare un valore non esistente!" << std::endl;
		std::cout << std::endl;
	}
	assert(ms1.size() == 8); // La dimensione è rimasta la stessa dall'ultima operazione valida
	std::cout << "Stampa di ms1 dopo il tentativo di eliminazione proibito" << std::endl;
	std::cout << ms1 << std::endl;
	std::cout << std::endl;

	ms_mspoint msempty; // Creo un oggetto MultiSet senza aggiungere valori

	std::cout << "Elimino da un MultiSet vuoto il MultiSet {(0,0), (1,3), (12,21)} (non presente)" << std::endl;
	assert(msempty.size() == 0);
	assert(msempty.nocc(mstemp2) == 0);
	try {
		msempty.remove(mstemp2); // Test remove		
	}
	catch(multiset_value_not_found &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: impossibile cancellare un valore non esistente!" << std::endl;
		std::cout << std::endl;
	}

	mspoint mstemp3;
	mstemp3.add(point(18,18));
	mstemp3.add(point(21,21));
	mstemp3.add(point(1,2));

	std::cout << "Test su ms1 di nocc() del MultiSet {(18,18), (21,21), (1,2)} (non presente)" << std::endl;
	assert(ms1.nocc(mstemp3) == 0);
	std::cout << "Occorrenze di {(18,18), (21,21), (1,2)}: " << ms1.nocc(mstemp3) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di nocc() del MultiSet {(1,1), (0,0), (1,2)} (presente 1 volta)" << std::endl;
	assert(ms1.nocc(msa) == 1);
	std::cout << "Occorrenze di {(1,1), (0,0), (1,2)}: " << ms1.nocc(msa) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di contains() del Multiset {(18,18), (21,21), (1,2)} (non presente)" << std::endl;
	assert(ms1.contains(mstemp3) == false); //Test contains
	std::cout << "Risposta: " << ms1.contains(mstemp3) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su ms1 di contains() del MultiSet {(4,4), (8,8)} (presente 1 volta)" << std::endl;
	assert(ms1.contains(mstemp) == true); // Test contains
	std::cout << "Risposta: " << ms1.contains(mstemp) << std::endl;
	std::cout << std::endl;

	// Sequenza di MultiSet di point
	MultiSet<point, equal_point> a[5] = {msa, msc, mstemp, msa, msc};

	ms_mspoint msiter(a, a + 5); // Test creazione MultiSet da una seq. generica identificata da due iteratori generici

	std::cout << "Stampa di msiter, creato da una seq. generica identificata da una coppia di iteratori" << std::endl;
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Test di size() su msiter (size = 5)" << std::endl;
	assert(msiter.size() == 5);
	std::cout << "Risposta: " << msiter.size() << std::endl;
	std::cout << std::endl;

	std::cout << "Test su msiter di nocc() del MultiSet {(2,4), (2,4)} (presente 2 volte)" << std::endl;
	assert(msiter.nocc(msc) == 2);
	std::cout << "Occorrenze di {(2,4), (2,4)}: " << msiter.nocc(msc) << std::endl;
	std::cout << std::endl;

	std::cout << "Test su msiter di contains() del MultiSet {(0,0), (2,4), (0,20)} (non presente)" << std::endl;
	assert(msiter.contains(msb) == false);
	std::cout << "Risposta: " << msiter.contains(msb) << std::endl;
	std::cout << std::endl;

	std::cout << "Aggiungo ad msiter il MultiSet {(4,4), (8,8)} (presente 1 volta)" << std::endl;
	assert(msiter.nocc(mstemp) == 1);
	msiter.add(mstemp); // Test add
	assert(msiter.size() == 6);
	assert(msiter.nocc(mstemp) == 2);
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Elimino da msiter il MultiSet {(2,4), (2,4)} (presente 2 volte)" << std::endl;
	assert(msiter.nocc(msc) == 2);
	msiter.remove(msc); // Test remove
	assert(msiter.size() == 5);
	assert(msiter.nocc(msc) == 1);
	std::cout << msiter << std::endl;
	std::cout << std::endl;

	std::cout << "Stampo ms1 ed ms2" << std::endl;
	std::cout << ms1 << std::endl;
	std::cout << std::endl;
	std::cout << ms2 << std::endl;
	std::cout << std::endl;

	std::cout << "Confronto ms1 con ms2 (diversi)" << std::endl;
	bool ans = (ms1 == ms2); // Test operator==
	assert(ans == false);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	ms_mspoint mscopy(ms1); // Copy constructor

	std::cout << "Confronto ms1 con un MultiSet creato con il copy constructor (uguali)" << std::endl;
	ans = (ms1 == mscopy); // Test operator==
	assert(ans == true);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	ms_mspoint ms4; // Inserisco gli stessi elementi di ms1, con le stesse occorrenze
	ms4.add(msa);
	ms4.add(mstemp);
	ms4.add(msd);
	ms4.add(msc);
	ms4.add(msb);
	ms4.add(msc);
	ms4.add(msc);
	ms4.add(msc);

	std::cout << "Confronto ms1 con ms4, popolato tramite add di elementi (uguali)" << std::endl;
	ans = (ms1 == ms4); // Test operator==
	assert(ans == true);
	std::cout << "Risposta: " << ans << std::endl;
	std::cout << std::endl;

	typename MultiSet<MultiSet<point, equal_point>, equal_multiset<point, equal_point>>::const_iterator i = msiter.begin(), ie = msiter.end();

	std::cout << "Stampa dei valori di msiter tramite iteratore costante" << std::endl;

	while(i != ie) {
		std::cout << *i << " "; // Stampa con iteratore costante
		++i;
	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Test su pre-incremento iteratore illegale" << std::endl;

	try {
		++i;
	}
	catch(multiset_iterator_out_of_bounds &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: incremento iteratore fuori dai limiti" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "Test su post-incremento iteratore illegale" << std::endl;

	try {
		i++;
	}
	catch(multiset_iterator_out_of_bounds &e) { // Test eccezione custom
		std::cout << "Eccezione verificata: incremento iteratore fuori dai limiti" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "!!!### FINE TEST DELLA CLASSE MULTISET SU MULTISET DI POINT ###!!!" << std::endl;
	std::cout << std::endl;
}

int main () {

	test_multiset_int();
	test_multiset_double();
	test_multiset_string();
	test_multiset_point();
	test_multiset_person();
	test_multiset_multiset_point();

	return 0;
}
