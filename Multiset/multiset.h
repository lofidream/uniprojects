/**
	@headerfile multiset.h

	@brief Dichiarazione e definizione di una classe templata MultiSet,
	con ridefinizione dell'operatore di stream << per oggetti MultiSet.
*/

// Guardie

#ifndef MULTISET_H
#define MULTISET_H

// Direttive pre-compilatore

#include <ostream> // std::ostream
#include <algorithm> //std::swap
#include <iterator> // std::forward_iterator_tag
#include <cstddef> // std::ptrdiff_t
#include "multiset_exceptions.h" // multiset_iterator_out_of_bounds, multiset_value_not_found

/**
	@brief MultiSet templato su due parametri

	@tparam T tipo degli elementi di un MultiSet
	@tparam E funtore di uguaglianza tra elementi del MultiSet
*/
template <typename T, typename E>
class MultiSet {

	// Sezione privata della classe

	/**
		Struct che implementa un nodo di una linked list (struttura dati scelta per
		rappresentare internamente il MultiSet)
	*/
	struct node {
		const T value; ///< Valore dell'elemento nel nodo
		unsigned int nocc; ///< Numero di volte in cui un valore compare nel MultiSet
		node *next; ///< Puntatore al nodo successivo

		/**
			@brief Costruttore di default per un nodo

			@description
			Questo metodo crea un nodo vuoto, con puntatore next a nullptr.
		*/
		node() : next(nullptr) {}

		/**
			@brief Costruttore secondario per un nodo

			@description
			Questo metodo crea un nodo, con valore specificato v. Il numero di occorrenze
			è impostato ad 1 e non punta ad alcun nodo successivo.
			La keyword explicit è usata per impedire al compilatore di usare questo metodo
			per effettuare conversioni implicite.

			@param v reference costante al valore dell'elemento di un nodo
		*/
		explicit node(const T &v) : value(v), nocc(1), next(nullptr) {}

		/**
			@brief Costruttore secondario per un nodo

			@description
			Questo metodo crea un nodo, con valore specificato v e puntatore al nodo successivo
			determinato da n.

			@param v reference costante al valore dell'elemento di un nodo
			@param n puntatore al nodo successivo
		*/
		node(const T &v, node *n) : value(v), nocc(1), next(n) {}

		/**
			@brief Distruttore per un nodo

			@post Locazione di memoria dell'elemento non più valida
		*/
		~node() {
			next = nullptr;
		}

		// L'implementazione dei restanti metodi standard è lasciata al compilatore

	}; //struct node

	// Altri dati membro privati

	node *_head; ///< Puntatore al primo nodo della lista
	unsigned int _size; ///< Numero totale di elementi nella lista

	E _eql; ///< Istanza del funtore di uguaglianza

	// Altri metodi privati

	/**
		@brief Metodo di rimozione contenuto del MultiSet

		@description
		Metodo privato che rimuove tutto il contenuto di un MultiSet.
		Si appoggia ad un altro metodo privato, richiamato sul nodo di testa.

		@post La memoria allocata per il MultiSet è deallocata
	*/
	void clear() {
		clear_helper(_head);
		_head = nullptr;
	}

	/**
		@brief Metodo ausiliario ricorsivo di rimozione degli elementi

		@description
		Metodo privato ausiliario che, in modo ricorsivo, si occupa di eliminare ogni nodo
		della lista che rappresenta il MultiSet. Tramite la ricorsione, la funzione elimina i nodi
		successivi a quello passato come parametro.

		@param curr puntatore all'elemento da cui iniziare la rimozione degli elementi

		@post Il puntatore a curr si riferisce ad una locazione di memoria non più valida
		@post Il numero di elementi nel MultiSet è ridotto tenendo conto delle occorrenze di quelli eliminati
		@post La memoria allocata per gli elementi a partire da curr è deallocata
	*/
	void clear_helper(node *curr) {
		if (curr == nullptr)
			return;
		clear_helper(curr->next);
		_size = _size - curr->nocc;
		delete curr;
		curr = nullptr;
	}

	/**
		@brief Variante di ricerca di un elemento nel MultiSet

		@description
		Questo metodo è una variante del metodo contains(). La logica è la medesima, ma
		cambia il valore di ritorno. Metodo privato utilizzato negli altri metodi.

		@param v elemento da cercare nel MultiSet

		@return puntatore al nodo contenente l'elemento cercato, se presente, nullptr altrimenti
	*/
	node* contains_at(const T &v) const {
		node *curr = _head;

		while(curr != nullptr) {
			if(_eql(curr->value, v))
				return curr;
			curr = curr->next;
		}
		return nullptr;
	}

	/**
		@brief Metodo ausiliario di rimozione elemento dal MultiSet

		@description
		Questo metodo viene richiamato da remove() e, tramite l'uso dei puntatori, elimina un elemento
		dal MultiSet.
		Se l'elemento da eliminare è la testa della lista, allora la nuova testa sarà l'elemento successivo a quello
		che faceva da testa. Il nodo testa precedente viene eliminato.
		Altrimenti, si scorre la lista dall'inizio fino a quando non viene trovato il nodo precedente a quello da eliminare.
		Tramite l'uso dei puntatori, il nodo da eliminare viene "scollegato" sia dal precedente sia dal successivo. Il next del
		nodo precedente diviene quindi il next del nodo da eliminare. Il nodo da eliminare è eliminato.

		@param curr nodo da eliminare

		@post Il puntatore della testa viene eventualmente aggiornato al nodo che le faceva da successivo
		@post Il nodo precedente ed il successivo rispetto al nodo da eliminare vengono collegati tra di loro,
		attraverso i puntatori
		@post Il nodo da eliminare viene distrutto e la sua locazione di memoria viene deallocata
	*/
	void remove_helper(node *curr) {
		node *tmp = curr;
		if(tmp == _head) {
			_head = _head->next;
			delete tmp;
			tmp = nullptr;
		}
		else {
			tmp = _head;
			while(tmp->next != curr)
				tmp = tmp->next;
			tmp->next = curr->next;
			delete curr;
			curr = nullptr;
		}
	}

public:
	
	// Sezione pubblica della classe

	// Metodi pubblici fondamentali

	/**
		@brief Costruttore di default per MultiSet

		@description
		Questo metodo istanzia un MultiSet vuoto, tramite initialization list.
		Il puntatore alla testa della lista, che rappresenta il MultiSet, è inizializzato
		a nullptr. La dimensione del MultiSet è 0.
	*/
	MultiSet() : _head(nullptr), _size(0) {}

	/**
		@brief Costruttore di copia per MultiSet

		@description
		Questo metodo permette di creare un MultiSet a partire da un altro.
		Dei dati di default vengono inseriti tramite initialization list, poi vi è
		l'effettiva copia del MultiSet. Nel caso si verifichi un'eccezione, questa è gestita
		tramite il blocco try-catch ed il contenuto del MultiSet corrente è rimosso tramite il metodo
		clear(). L'eventuale eccezione viene propagata al chiamante.

		@param other MultiSet da copiare per istanziare quello corrente

		@throw eccezione di allocazione di memoria

	*/
	MultiSet(const MultiSet &other) : _head(nullptr), _size(0) {
		node *curr = other._head;

		try {
			while(curr != nullptr) {
				for(unsigned int i = curr->nocc; i > 0; --i) {
					add(curr->value);
				}
				curr = curr->next;
			}
		}
		catch(...) { // Eccezione di allocazione di memoria
			clear();
			throw;
		}
	}

	/**
		@brief Operatore di assegnamento per MultiSet

		@description
		Questo metodo permette la copia tra MultiSet, controllando l'eventuale auto-assegnamento.
		La copia avviene tramite copy-constructor ed utilizzo della funzione std::swap.
		L'uso del metodo copy-constructor comporta la possibile propagazione di eccezioni.

		@param other MultiSet "sorgente" da copiare

		@return Riferimento al MultiSet corrente

		@throw Eccezione di allocazione di memoria
	*/
	MultiSet& operator=(const MultiSet &other) {
		if(this != &other) {
			MultiSet tmp(other);
			std::swap(this->_head, tmp._head);
			std::swap(this->_size, tmp._size);
		}
		return *this;
	}

	/**
		@brief Distruttore per MultiSet

		@descrpition
		Il distruttore del MultiSet sfrutta il metodo privato clear(), che si occupa
		di distruggere tutti i nodi della lista, deallocando ricorsivamente la memoria
		da loro occupata.

		@post La memoria allocata da tutti gli elementi del MultiSet è deallocata
		@post Tutte le locazioni di memoria per il MultiSet ed i suoi elementi non sono ora più valide
	*/
	~MultiSet() {
		clear();
	}

	// Metodi pubblici non fondamentali

	/**
		@brief Ricerca di un elemento nel MultiSet

		@description
		Questo metodo si occupa di stabilire se un dato di tipo generico T è presente
		o meno nel MultiSet. La ricerca avviene scorrendo la lista fino in fondo, o finchè
		l'elemento viene trovato.

		@param v elemento da cercare nel MultiSet

		@return True se l'elemento è presenta, false altrimenti
	*/
	bool contains(const T &v) const {
		node *curr = _head;

		while(curr != nullptr) {
			if(_eql(curr->value, v))
				return true;
			curr = curr->next;
		}
		return false;
	}

	/**
		@brief Numero di elementi di un MultiSet

		@return numero totale di elementi di un MultiSet
	*/
	unsigned int size() const {
		return _size;
	}

	/**
		@brief Inserimento di un elemento nel MultiSet

		@description
		Questo metodo permette l'inserimento di un elemento nel MultiSet.
		Il valore passato come parametro viene cercato all'interno del MultiSet e, se presente,
		il numero di occorrenze nel nodo che lo contiene viene incrementato di 1.
		Se il valore non è presente, allora viene creato un nuovo nodo, con valore dato e puntatore
		al nodo successivo impostato a nullptr.
		Seguono due sottocasi: se la lista è vuota (ovvero la testa punta a nullptr), allora il nuovo nodo 
		diviene la testa della lista. Altrimenti, il metodo scorre la lista ed aggiunge il nuovo nodo alla fine
		della stessa, avendo cura di aggiornare il puntatore next del nodo prima di quello inserito.
		
		@param v valore da inserire nel MultiSet

		@post Se l'elemento era già presente nel MultiSet, il suo numero di occorrenze è incrementato di 1
		@post Se la lista era vuota (elemento non presente), il puntatore alla testa viene aggiornato con l'elemento inserito
		@post Se la lista non era vuota (elemento non presente), il nuovo elemento inserito è l'ultimo della lista
		@post Il numero totale di elementi è incrementato di 1
	*/
	void add(const T &v) {
		node *curr = this->contains_at(v);

		_size++;

		if(curr != nullptr) {
			curr->nocc++;
			return;
		}
		else {
			node *tmp = new node(v, nullptr);
			if(_head == nullptr)
				_head = tmp;
			else {
				curr = _head;
				while(curr->next != nullptr)
					curr = curr->next;
				curr->next = tmp;
			}
		}
	}
	
	/**
		@brief Numero di occorrenze di un elemento del MultiSet

		@description
		Questo metodo restituisce il numero di occorrenze di un elemento contenuto in un nodo.
		Dato il valore v, si ricerca l'elemento nella lista e, se presente, viene restituito il suo
		numero di occorrenze.

		@param v valore di cui sapere il numero di occorrenze

		@return numero di occorrenze se il valore è presente, 0 altrimenti (un elemento non presente ha 0 occorrenze)
	*/
	unsigned int nocc(const T &v) const {
		node *curr = this->contains_at(v);
		if(curr != nullptr)
			return curr->nocc;
		return 0;
	}

	/**
		@brief Rimozione di un elemento dal MultiSet

		@description
		Questo metodo si occupa di cercare l'elemento da rimuovere e, se presente, ne riduce
		il numero di occorrenze. Se il numero di occorrenze diventa 0, allora l'elemento viene
		effettivamente cancellato dal MultiSet, tramite la funzione remove_helper(). Viene poi decrementata
		la dimensione totale del MultiSet.
		In caso l'elemento da eliminare non sia presente, viene lanciata un'eccezione custom con un messaggio.
		
		@pre L'elemento dev'essere presente nella lista
		
		@param v valore da rimuovere

		@post Se l'elemento era presente, ne viene decrementato il numero di occorrenze
		@post Se il numero di occorrenze (dopo il decremento) di un elemento è 0, la sua rimozione è lasciata al
		metodo ausiliario remove_helper()
		@post Il numero totale di elementi del MultiSet è diminuito di 1

		@throw Eccezione custom per elemento non presente
	*/
	void remove(const T &v) {
		node *curr = this->contains_at(v);

		if(curr != nullptr) {
			curr->nocc--;
			if(curr->nocc == 0)
				remove_helper(curr);
			_size--;
			return;
		}
		else {
			throw multiset_value_not_found();
		}
	}

	/**
		@brief Creazione di un MultiSet a partire da un insieme di elementi presi da
		una sequenza identificata da due iteratori generici

		@description
		Questo metodo crea un Multiset, aggiungendo (tramite add()) degli elementi presi da una sequenza
		identificata da una coppia di iteratori, uno per l'inizio della sequenza ed uno
		per la fine.
		Gli elementi della sequenza non sono necessariamente di tipo T, pertanto al compilatore
		è affidato il compito di effettuare l'eventuale conversione, tramite uno static cast a T.
		L'inserimento degli elementi ed il casting sono racchiusi in un blocco try-catch, che si 
		occupa di intercettare un'eventuale eccezione, andando a svuotare il nuovo MultiSet creato e
		rilanciando l'eccezione al chiamante.

		@pre Il tipo dei valori della sequenza dev'essere castabile nel tipo T

		@tparam IterT tipo degli iteratori che identificano la sequenza generica da inserire nel MultiSet

		@param begin iteratore che punta all'inizio della sequenza
		@param end iteratore che punta alla fine della sequenza

		@post Il numero di elementi inseriti nel MultiSet viene incrementato tante volte quanti sono
		gli elementi che compongono la sequenza generica
		@post L'iteratore begin di inizio sequenza punta ora alla fine della sequenza

		@throw Eccezione di allocazione di memoria
	*/
	template <typename IterT>
	MultiSet(IterT begin, IterT end) : _head(nullptr), _size(0) {
		try {
			while(begin != end) {
				add(static_cast<T>(*begin));
				++begin;
			}
		}
		catch(...) { // Eccezione di allocazione di memoria
			clear();
			throw;
		}
	}

	/**
		@brief Operatore di uguaglianza tra due MultiSet

		@description
		Questo metodo si occupa di ridefinire l'operator==, per implementare un confronto
		di uguaglianza tra due MultiSet.
		Due MultiSet (dello stesso tipo) sono uguali se contengono i medesimi elementi, con lo stesso numero
		di occorrenze per ciascun elemento.
		Il controllo che entrambi i MultiSet contengano dati dello stesso tipo è affidata al compilatore.
		Il primo controllo effettuato dal metodo è sul numero totale di elementi. In caso i due MultiSet
		avessero lo stesso numero di elementi, si procede a scorrere la lista di elementi del primo MultiSet
		e la si confronta con il secondo MultiSet, tramite il valore dei nodi e del numero di occorrenze.
		Nel caso un elemento non sia trovato o il suo numero di occorrenze non sia uguale in entrambi i MultiSet,
		allora i due MultiSet non sono uguali.

		@pre Il tipo di dati presenti nei MultiSet dev'essere lo stesso, per garantirne il confronto

		@param other MultiSet con cui confrontare il primo

		@return True se i due MultiSet sono uguali, false altrimenti
	*/
	bool operator==(const MultiSet &other) const {
		if(this->size() == other.size()) {
			node *curr = this->_head;
			while(curr != nullptr) {
				node *tmp = other.contains_at(curr->value);
				if((tmp != nullptr) && (tmp->nocc == curr->nocc))
					curr = curr->next;
				else
					return false;
			}
			return true;
		}
		return false;
	}

	// Supporto agli iteratori per un MultiSet

	// Iteratore in sola lettura

	/**
		@brief Iteratore in lettura (costante) di tipo forward per un MultiSet
	*/
	class const_iterator {

	public:

		// Interfaccia pubblica dell'iteratore costante

		// Traits dell'iteratore costante

		typedef std::forward_iterator_tag iterator_category; ///< Categoria dell'iteratore
		typedef const T value_type; ///< Tipo dei dati puntati dall'iteratore costante
		typedef ptrdiff_t difference_type; ///< Tipo per rappresentare la differenza tra due puntatori
		typedef const T* pointer; ///< Tipo di puntatore ai dati puntati dall'iteratore costante
		typedef const T& reference; ///< Tipo di reference ai dati puntati dall'iteratore costante

		// Metodi fondamentali dell'iteratore costante

		/**
			@brief Costruttore di default dell'iteratore costante

			@description
			Il costruttore di default istanzia un iteratore costante che punta a nullptr.
		*/
		const_iterator() : ptr(nullptr) {}

		/**
			@brief Copy constructor dell'iteratore costante

			@description
			Questo metodo permette di istanziare un nuovo iteratore costante con i valori
			dei dati membro	presi da un altro iteratore costante.

			@param other iteratore costante da copiare
		*/
		const_iterator(const const_iterator &other) : ptr(other.ptr) {}

		/**
			@brief Operatore di assegnamento dell'iteratore costante

			@description
			Questo metodo permette la copia tra iteratori costanti.

			@param other iteratore costante "sorgente" da copiare

			@return riferimento all'iteratore costante corrente
		*/
		const_iterator& operator=(const const_iterator &other) {
			ptr = other.ptr;
			return *this;
		}

		/**
			@brief Distruttore per un iteratore costante
		*/
		~const_iterator() {}

		// Altri metodi pubblici dell'iteratore costante

		/**
			@brief Operatore di deferenziamento

			@description
			Permette l'accesso in sola lettura del valore del nodo puntato
			dall'iteratore costante.

			@return valore costante del nodo puntato dall'iteratore costante
		*/
		reference operator*() const {
			return ptr->value;
		}

		/**
			@brief Operatore di accesso tramite puntatore

			@description
			Permette l'accesso in sola lettura, tramite un puntatore, al valore
			del nodo puntato dall'iteratore costante.

			@return puntatore al valore costante del nodo puntato dall'iteratore costante
		*/
		pointer operator->() const {
			return &(ptr->value);
		}

		/**
			@brief Operatore di iterazione post-incremento

			@description
			Permette di incrementare l'iteratore costante di un'unità, puntando all'elemento
			successivo, e di usarlo allo stato precedente l'incremento.
			L'incremento dell'iteratore è condizionato all'esaurimento di occorrenze del valore
			presente nel nodo puntato dall'iteratore.

			@pre L'iteratore deve puntare ad una locazione di memoria interna al MultiSet

			@param int è un placeholder che permette di distinguere questo operatore di incremento con quello
			di pre-incremento 

			@return Copia dell'iteratore costante corrente prima dell'incremento, se il numero di occorrenze del valore
			presente nel nodo puntato dall'iteratore è stato consumato. Altrimenti, viene ritornato il riferimento
			all'iteratore costante corrente non incrementato.

			@throw multiset_iterator_out_of_bounds se l'iteratore costante, punta ad una locazione di memoria
			esterna al MultiSet
		*/
		const_iterator operator++(int) {
			if(ptr == nullptr)
				throw multiset_iterator_out_of_bounds();
			if(t == ptr->nocc) {
				t = 1;
				const_iterator tmp(*this);
				ptr = ptr->next;
				return tmp;
			}
			else
				t++;
			return *this;
		}

		/**
			@brief Operatore di iterazione pre-incremento

			@description
			Permette di incrementare l'iteratore costante di un'unità, puntando all'elemento
			successivo, e di usarlo allo stato successivo l'incremento.
			L'incremento dell'iteratore è condizionato all'esaurimento di occorrenze del valore
			presente nel nodo puntato dall'iteratore.

			@pre L'iteratore deve puntare ad una locazione di memoria interna al MultiSet

			@return Riferimento all'iteratore costante corrente, incrementato o meno a seconda del verificarsi
			delle condizioni richieste

			@throw multiset_iterator_out_of_bounds se l'iteratore costante, punta ad una locazione di memoria
			esterna al MultiSet
		*/
		const_iterator& operator++() {
			if(ptr == nullptr)
				throw multiset_iterator_out_of_bounds();
			if(t == ptr->nocc) {
				t = 1;
				ptr = ptr->next;
			}
			else
				t++;
			return *this;
		}

		/**
			@brief Operatore di uguaglianza

			@description
			Permette di confrontare due iteratori costanti, verificando se sono uguali, ovvero se
			puntano allo stesso elemento della lista.

			@param other iteratore costante con cui confrontare quello corrente

			@return true se i due iteratori costanti puntano allo stesso elemento della lista, false altrimenti
		*/
		bool operator==(const const_iterator &other) const {
			return(ptr == other.ptr);
		}

		/**
			@brief Operatore di disuguaglianza

			@description
			Permette di confrontare due iteratori costanti, verificando se sono diversi, ovvero se
			non puntano allo stesso elemento della lista.

			@param other iteratore costante con cui confrontare quello corrente

			@return true se i due iteratori costanti non puntano allo stesso elemento della lista, false altrimenti
		*/
		bool operator!=(const const_iterator &other) const {
			return(ptr != other.ptr);
		}

	private:

		// Dati privati dell'iteratore costante

		const node *ptr; ///< Puntatore ad un elemento costante della lista
		unsigned int t; ///< Intero che memorizza il numero di occorrenze di un elemento della lista, usato negli operatori di incremento

		friend class MultiSet; // La classe container che utilizza l'iteratore costante dev'essere friend della classe iteratore

		/**
			@brief Costruttore privato

			@description
			Questo costruttore privato inizializza un iteratore costante, impostando il puntatore
			ad un elemento n passato come parametro. Il dato membro t (che serve per contare le occorrenze)
			è posto ad 1.
			Il costruttore è explicit per evitare che il compilatore lo usi per effettuare conversioni implicite.

			@param n puntatore ad un elemento costante della lista
		*/
		explicit const_iterator(const node *n) : ptr(n), t(1) {}

	}; // class const_iterator

	// Funzioni membro per l'utilizzo di iteratori costanti

	/**
		@brief Iteratore costante che punta all'inizio del MultiSet

		@description
		L'iteratore punta all'elemento in testa.

		@return iteratore costante che punta all'inizio del MultiSet
	*/
	const_iterator begin() const {
		return const_iterator(_head);
	}

	/**
		@brief Iteratore costante che punta alla fine del MultiSet

		@description
		L'iteratore punta a nullptr.

		@return iteratore costante che punta alla fine del MultiSet
	*/
	const_iterator end() const {
		return const_iterator(nullptr);
	}

}; //class MultiSet

// Funzioni globali

/**
	@brief Ridefinizione dell'operatore di stream <<

	@description
	L'operatore è ridefinito per inviare su stream il contenuto di un MultiSet.
	L'invio in stream di un elemento del MultiSet è effettuata solo quando il suo numero di
	occorrenze è stato consumato. L'ultimo elemento del MultiSet viene inviato in stream
	quando l'incremento	dell'iteratore lo porterebbe alla fine del MultiSet.
	Il formato di invio su stream è {<X1, OccorrenzeX1>, <X2, OccorrenzeX2>, ..., <Xn, OccorrenzeXn>}.

	@tparam T tipo del valore degli elementi del MultiSet da stampare
	@tparam E funtore di uguaglianza del valore di due elementi di un MultiSet

	@param os oggetto di stream output
	@param ms MultiSet da stampare

	@return riferimento allo stream di output
*/
template <typename T, typename E>
std::ostream &operator<<(std::ostream &os, const MultiSet<T,E> &ms) {

	typename MultiSet<T,E>::const_iterator i = ms.begin(), ie = ms.end();
	T curr_val;
	unsigned int count;
	
	curr_val = *i;
	count = 0;

	os << "{";

	while(i != ie) {
		if(curr_val == *i)
			count++;
		else {
			os << "<" << curr_val << ", " << count << ">";
			if(i != ms.begin())
				os << ", ";
			curr_val = *i;
			count = 1;
		}
		i++;
		if(i == ms.end())
			os << "<" << curr_val << ", " << count << ">";
	}

	os << "}";

	return os;
}

#endif

// Fine multiset.h