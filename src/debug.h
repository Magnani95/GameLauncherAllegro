/**@file
*File contenente tutte le direttive al pre-processore comuni a tutto il programma.
*L'header è contenuto in @link strutt_dati.h @endlink, quindi non va incluso esplicitamente.
*/

//--------------DISATTIVATORI
//---------------IF AND ELSE
#ifdef DEBUG
	#define function_debug(a) 	a
	#include<iostream>

	#ifndef ND_GRAFICA
		#define DEB_GRAFICA	/**<Attiva le macro di debug per le componenti grafiche*/
	#endif
	#ifndef ND_VAL
		#define DEB_VAL		/**<Attiva le macro di debug per seguire il flusso del programma e stampare lo stato delle variabili*/
	#endif
#else
	#define function_debug(a)

	#define NDEBUG
	#undef DEB_GRAFICA
	#undef DEB_VAL
#endif

//----------------SOTTOMACRO MAGGIORI
//-----------------------------------
#ifdef DEB_GRAFICA		//Debug per la parte di grafica di Allegro
/**Aggiorna lo schermo e attende per i secondi specificati.*/
	#define pausa_debug(a) //{al_flip_display(); al_rest((a));}
#else
	#define pausa_debug(a)
#endif
//----------------------
#ifdef DEB_VAL	//debug per valori e stato variabili

	/*stampa in cout (a)=file, (b)=funzione, (c)=note.*/
	#define step(a, b, c) 	step_debug(#a, #b, #c)

	/**Stampa in cout il valore e il nome di (a); (c) per il file o note*/
	#define stampa(a, c) 	stampa_debug(a, #a, #c)

	/**Stampa in cout l'indirizzo e il nome del puntatore (a); (c) per file o note*/
	#define stampadd(a,c) 	stampadd_debug(a, #a, #c)

	/** Stampa in cout una lista di carte (a); (x) per file o note.*/
	#define dblista(a, c)	//dblista_debug(a, #a, #c)

#else
	#define step(a, b, c)
	#define stampa(a, c)
	#define stampadd(a, c)
	#define dblista(a, c)
#endif


//-----------SOTTOMACRO STRUTTURALI)
#define stampa_debug(a, b, c)		{cout<<"\n"<<c<<"\t\tValore di\t\t"<<(b)<<"\t"<<static_cast<int>(a)<<flush;}

#define step_debug(a, b, c) 		cout<<"\n\n---File\t\t"<<(a)<<"\t\t"<<(b)<<"\n"<<(c)<<"\n"<<flush;

#define stampadd_debug(a, b, c)	{cout<<"\n"<<c<<"\t\tindirizzo di\t\t"<<(b)<<"\t"<<(a)<<flush;}

#define dblista_debug(a, b, c)	{ cout<<"\n\nStampa lista di \t\t"<<(b)<<flush; \
						cout<<"\n"<<(c);\
						st_carta *testa, *attuale; \
						testa=attuale=(a); \
						stampadd(testa,) \
						cout<<attuale->seme<<"("<<static_cast<int>(attuale->val)<<")  "<<flush; \
						attuale=attuale->next; \
						while(attuale!=testa){ \
							cout<<attuale->seme<<"("<<static_cast<int>(attuale->val)<<")  "<<flush;\
							attuale=attuale->next; \
						}\
						}

#include<cassert>
