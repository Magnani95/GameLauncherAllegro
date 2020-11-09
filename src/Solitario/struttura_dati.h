/**@file Il file contiene le strutture dati utili al modulo Solitario per funzionare
*/

/**Indica il seme delle carte*/
enum en_seme {cuori, quadri, fiori, picche};

/**Struttura carta.
*Indica il valore e il seme della carta. È pensata per l'uso in liste doppie.
*/
struct st_carta{
	en_seme seme;
	unsigned char val;
	st_carta *next;
	st_carta *prev;
};
/**Struttura fila
*Indica una fila di carte.
*/
struct st_fila{
	char n_carte;
	char n_coperte;
	st_carta *prima_carta; /**<Indica la prima carta; è pensata per essere una lista doppia circolare*/
};

/**Structo partita.
*Continene tutti i puntatori e strutture necessarie per gestire una partita.
*/
struct st_partita {
	st_fila fila[7];
	st_carta *mazzo;		/**<Indica la prima carta; è pensata per essere una lista doppia circolare*/
	st_carta *c_mazzo;	/**<Punta la carta attuale mostrata dal mazzo*/
	st_carta *uscita[4]; 	/**<Ogni elemento indica la prima carta; è per seme. pensata per essere una lista doppia circolare*/
};

//VARIABILI GLOBALI DI MODULO
/**Puntatore alla partita attualmente inizializzata*/
extern st_partita *partita;
