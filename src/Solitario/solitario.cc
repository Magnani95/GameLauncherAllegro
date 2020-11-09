/**@file File contenente il main del modulo ::Solitario
*Il file gestisce le chiamate agl altri moduli e funge da centro di comunicazione tra i moduli.
*/
using namespace std;

#include"../allegro_header.h"
#include"../strutt_dati.h"
#include"struttura_dati.h"

#include"inizializzazione.h"
#include"grafica.h"
#include"gestione_gioco.h"

//variabili globali di sottomodulo
/**Puntatore alla partita in corso*/
st_partita *partita=NULL;

//variabili di file
/**Coda eventi di Allegro*/
ALLEGRO_EVENT_QUEUE *coda_eventi;
/**Variabile per raccogliere l'evento di input*/
ALLEGRO_EVENT evento;

//prototipi funzioni interne
int leggi_input();

//---------------------------------------INIZIO INTERFACCIA---------------------
//-------------------------------------------------------------------------------
/**Funzione principale del modulo solitario
*La funzione gestisce l'inizializzazione delle componenti al lancio e la de-allocazione
*al termine. È il fulcro delle comunicazioni tra modulo grafico/input e gestione gioco.
*/
void solitario ()
{
	//inizializzazioni
	init_partita();
	function_debug(stampa_stato_partita();)
	assert(check_carte());

	init_grafica();
	function_debug(debug_sezioni();)

	coda_eventi = al_create_event_queue();

	al_register_event_source(coda_eventi, al_get_mouse_event_source());

	bool fine=false;
	int s1, s2, stato;

	while(!fine){
		function_debug(stampa_stato_partita();)
		disegna_schermata(-1);
		assert(check_carte());

		s1=leggi_input();
		if (!(s1==8 || s1==15)){
			disegna_schermata(s1);
			s2=leggi_input();
			stato=gestore_mosse(s1, s2);
		}else{
			stato=gestore_mosse(s1, 0);
		}
		switch(stato){
			case 0:
				break;
			case -1:
				fine=true; break;
			case -2:
				al_show_native_message_box(display, "ATTENTO", "Sembra che la mossa non sia possibile",
									"Controlla di aver fatto una scelta valida", 0, ALLEGRO_MESSAGEBOX_WARN);
				break;
			case -99:
				al_show_native_message_box(display, "ERRORE", "Errore Critico",
									"Attiva la modalità di debug per avere più informazioni sull'errore", 0, ALLEGRO_MESSAGEBOX_ERROR);
				break;

			default:
				al_show_native_message_box(display, "ERRORE", "Errore Critico in switch",
									"l'errore è nello switch del solitario.cc", 0, ALLEGRO_MESSAGEBOX_ERROR); assert(false); return;
		}
		s1=s2=-1;
	}

	delete_partita();
	delete_grafica();
	al_unregister_event_source(coda_eventi, al_get_mouse_event_source());
	return;
}
//---------------------------FUNZIONI INTERNE-------------------------
//--------------------------------------------------------------------
/**Funzione per la raccolta dell'input mouse.
*La funzione rimane in attesa di click sinistro del mouse e restituisce l'indice
*della sezione del click. Guardare ::mouse_to_sezione per i valori di ritorno.
*/
int leggi_input()
{
	int selezione=-1;
	while(true){
		al_wait_for_event(coda_eventi, &evento);
		if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
			selezione= mouse_to_sezione(evento.mouse.x, evento.mouse.y);
			stampa(selezione, leggi_input)
		}
		if((selezione==-1)||((selezione==9)&&(partita->c_mazzo==NULL)) )
			continue;
		else
			return selezione;
	}

}
