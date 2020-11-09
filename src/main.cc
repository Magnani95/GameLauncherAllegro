/**
*@mainpage Progetto-Allegro
*Programma per la gestione di più giochi da tavolo. <br>
*Il programma utilizza la libreria Allegro5; vedere tale documentazione sul sito. <br>
*Per aggiungere un gioco o sottomodulo bisogna: <br>
*-aggiungere il prototipo della funzione principale del modulo a @link giochi.h @endlink <br>
*-aggiungere il puntatore e il nome alla funzione ::inizializzazione_giochi in @link inizializzazione.cc @endlink. <br>
*-aggiungere agli header di ogni file @link strutt_dati.h @endlink poichè contiene i dati da propagare a tutto il programma.<br>
*-se il file usa componenti grafiche di allegro, includere anche @link allegro_header.h @endlink
*@author Andrea Magnani
*/
/**@file
*File contenente la funzione main.
*Gestisce l'interfaccia principale per il lancio dei vari giochi.
*/
#include<iostream>
#include<cstring>
#include<string>
#include<cstdlib>
#include<ctime>
using namespace std;

#include"allegro_header.h"
#include"strutt_dati.h"
#include"main_grafica.h"

//---Variabili globali di programma
extern const int wight=1600,	/**<Numero di pixel di larghezza della finestra di Allegro */
		height=900;		/**<Numero di pixel di altezza della finestra di Allegro */
ALLEGRO_DISPLAY *display= NULL;
extern const int n_giochi=1;

char path_main_font[]={"Data/Font/Oswald-Light.ttf"};		/**<Path al font base del programma*/

pagina_giochi pagina[1+ (n_giochi-1)/6];
void ( *giochi [(n_giochi-1)/6] )(void);	/**<Array di puntatori a funzione per le chiamate dei diversi giochi. Restituisce 0 se tutto ok, altrimenti il codice di errore*/


/**Funzione main del programma.
* Invoca le inizializzazioni utili al funzionamento del programma e di Allegro.<br>
* Gestisce la pagina di selezione dei giochi e gli eventi di selezione. <br>
* Per ulteriori informazioni sulle inizializzazioni guardare @link main_grafica.cc @endlink
*/
int main()
{
	srand(time(0));
	if(!inizializzazione_allegro())
		return -1;
	if(!al_install_mouse())
		return -2;
		//settaggio dei costrutti necessari per la gestione di Allegro
	display = al_create_display(wight, height);
	al_set_window_position(display, 100, 100);
	al_set_window_title(display, "Progetto Allegro");
	ALLEGRO_EVENT_QUEUE *coda_eventi = al_create_event_queue();
	ALLEGRO_EVENT evento;

	//registra gli eventi dalle fonti salvandoli nelle code.
	al_register_event_source(coda_eventi, al_get_mouse_event_source());
	al_register_event_source(coda_eventi, al_get_display_event_source(display));

	inizializzazione_giochi(pagina, giochi);

	int indice=0, //mostra l'indice di pagina da stampare a schermo
			selezione=-1; //Indica la sezione scelta dal click del mouse; 6 per il bottone di uscita. -1 se nessuna valida

	schermata_principale(pagina [0]);		//Schermata iniziale

	while(true){
		al_wait_for_event(coda_eventi, &evento);

		if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
			selezione= gestione_mouse(evento.mouse.x, evento.mouse.y);
			stampa(selezione, main)
		}
		if( (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)  || (selezione == 6) )
			break;
		else if((selezione == -1) ||( (selezione>n_giochi-1)&&(selezione<6) ))		//sezione non valida o gioco non presente.
			continue;
		else{//selezione indica l'indice del gioco da avviare

			giochi[selezione]();
			selezione=-1;
			//Scollego coda_eventi dall'input per evitare che siano registrati input dal sottomodulo lanciato.
			al_unregister_event_source(coda_eventi, al_get_mouse_event_source());
			al_unregister_event_source(coda_eventi, al_get_display_event_source(display));

			schermata_principale(pagina[indice]);

			al_register_event_source(coda_eventi, al_get_mouse_event_source());
			al_register_event_source(coda_eventi, al_get_display_event_source(display));

		}
	}

	//distruttori al termine del programma
//	al_rest(10.0);
	al_destroy_event_queue(coda_eventi);
	al_uninstall_mouse();
	al_destroy_display(display);
	return 0;
}
