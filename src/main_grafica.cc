/**@file
*File di supporto per la grafica e l'inizializzazione del ::main. <br>
*Usato principalmente per lasciare il main il più pulito possibile.
*/

#include<cstring>
using namespace std;
#include "allegro_header.h"
#include"giochi.h"
#include "strutt_dati.h"


//Variabili globali file
sezione s_titolo, s_centrale, s_basso, pulsante;
//Prototipi funzioni interne
void init_sezioni_schermata_principale();

//------------------------INIZIO DEFINIZIONE-----------------------
//--------------------------FUNZIONI INTERFACCIA-------------------

/**Inizializza allegro e i relativi addon.
* Viene stampata una finestra nativa a schermo in caso di errore.
*@return TRUE se l'inizializzazione è andata a buon fine
*@return FALSE se c'è stato un errore
*/
bool inizializzazione_allegro()
{
	char errore_testo[]={"Hai provato a spegnere e riaccendere il computer? :)"};
	bool status=true;

	if(!al_init()){
		al_show_native_message_box(display,"Errore","Impossibile Inizializzare Allegro",
		errore_testo, NULL, ALLEGRO_MESSAGEBOX_ERROR);
		status=false;
	}
	if(!al_init_font_addon()){
		al_show_native_message_box(display,"Errore","Impossibile Inizializzare font_addon",
		errore_testo, NULL, ALLEGRO_MESSAGEBOX_ERROR);
	}
	if(!al_init_ttf_addon()){
		al_show_native_message_box(display,"Errore","Impossibile Inizializzare ttf_addon",
		errore_testo, NULL, ALLEGRO_MESSAGEBOX_ERROR);
		status=false;
	}
	if(!al_init_primitives_addon()){
		al_show_native_message_box(display,"Errore","Impossibile Inizializzare primitives_addon",
		errore_testo, NULL, ALLEGRO_MESSAGEBOX_ERROR);
		status=false;
	}
	if(!al_init_image_addon()){
		al_show_native_message_box(display,"Errore","Impossibile Inizializzare image_addon",
		errore_testo, NULL, ALLEGRO_MESSAGEBOX_ERROR);
		status=false;
	}
	return status;
}
/**Disegna i sei giochi contenuti nella pagina passata.
*Crea ed inizializza le ::sezioni utili al disegno della pagina iniziale.
*utile per le aggiunte future (più pagine, bottoni tutorial etc.)-<br>
*Al termine dell'esecuzione aggiorna automaticamente lo schermo.<br>
*Modificare qui per aggiustare la posizione del testo.
*@param [in] pagina Pagina da stampare a schermo.
*/

void schermata_principale(pagina_giochi pagina)
{
	static bool init=false;
	if(!init)
		init_sezioni_schermata_principale();

	ALLEGRO_COLOR col_bottone= al_map_rgb(255, 255, 0),
				col_testo= al_map_rgb(32,32,32),
				col_sfondo= al_map_rgb(0,102,0),
				col_uscita_bottone=al_map_rgb(255,0,0),
				col_uscita_testo=al_map_rgb(0,0,0);

	ALLEGRO_FONT *f_titolo= al_load_ttf_font(path_main_font, 80, 0),
			*f_giochi= al_load_ttf_font(path_main_font, 50, 0);

	al_clear_to_color(col_sfondo);

	//Sezione del titolo
	al_draw_filled_rounded_rectangle(s_titolo.x+s_titolo.w/2-500, s_titolo.y+s_titolo.h/2+10, s_titolo.x+s_titolo.w/2+500,  s_titolo.y+s_titolo.h/2+125, 5, 5, col_bottone);
	al_draw_text(f_titolo, col_testo, s_titolo.x+s_titolo.w/2, s_titolo.y+s_titolo.h/2, ALLEGRO_ALIGN_CENTER, "Progetto Allegro");
	pausa_debug(0.5)

	//sezione centrale - pulsanti per i giochi.
	for(int j=0, N=0, Y=s_centrale.y+s_centrale.h/4-pulsante.h/2; j!=2; j++, Y+=(s_centrale.h/2)){
		for(int i=0, X=s_centrale.x+s_centrale.w/4-pulsante.w/2; i!=3; i++, N++, X+=(s_centrale.w/4)){
			al_draw_filled_rounded_rectangle(X, Y, X+pulsante.w, Y+pulsante.h, 15, 15, col_bottone);
			if(strcmp(pagina.nome_gioco[N], "VUOTO") )
				al_draw_text(f_giochi, col_testo, X+pulsante.w/2, Y+pulsante.h/2-40, ALLEGRO_ALIGN_CENTRE, pagina.nome_gioco[N]);
			else
				al_draw_text(f_giochi, col_testo, X+pulsante.w/2, Y+pulsante.h/2-40, ALLEGRO_ALIGN_CENTRE, "COMING SOON!");
			pausa_debug(0.5)
		}
	}

	//sezione bassa - tasto uscita
	int X=s_basso.x+s_basso.w/4-pulsante.w/2, Y=s_basso.y+s_basso.h/2-pulsante.h/2;
	al_draw_filled_rectangle(X, Y , X+pulsante.w, Y+pulsante.h, col_uscita_bottone);
	al_draw_text(f_giochi, col_uscita_testo, X+pulsante.w/2, Y+pulsante.h/2, ALLEGRO_ALIGN_CENTRE, "USCIRE");

	al_destroy_font(f_titolo);
	al_destroy_font(f_giochi);

	al_flip_display();


}
/**Inizializza il nome dei giochi e i loro puntatori.
*Le stringhe contengono il nome del gioco mostrato a schermo durante la selezione
*del gioco. <br>
*Ogni pagina contiene 6 stringhe, quindi inizializzare i nomi a blocchi di 6.
*Inserire "VUOTO" se non c'è alcun gioco; serve per gestire correttamente le chiamate
*dei sottomoduli-gioco. <br>
*Viene anche inizializzato l'array di puntatori per la selezione del gioco.
*@param[in] array di pagina_giochi da inizializzare
@param[in] array di puntatori alle funzioni dei giochi
*/
void inizializzazione_giochi(pagina_giochi *pagina, void(*giochi[]) (void))
{
	strcpy(pagina[0].nome_gioco[0], "Solitario");
	strcpy(pagina[0].nome_gioco[1], "VUOTO");
	strcpy(pagina[0].nome_gioco[2], "VUOTO");
	strcpy(pagina[0].nome_gioco[3], "VUOTO");
	strcpy(pagina[0].nome_gioco[4], "VUOTO");
	strcpy(pagina[0].nome_gioco[5], "VUOTO");

	giochi[0]=solitario;
	giochi[1]=NULL;
	giochi[2]=NULL;
	giochi[3]=NULL;
	giochi[4]=NULL;
	giochi[5]=NULL;
}
/**Gestisce la posizione del click mouse.
*Valuta se le coordinate passate sono all'interno di qualche sezione; in tal caso
*Restituisce l'indice di tale sezione.
*@param[in] mouse_x le coordinate x del mouse
*@param[in] mouse_y le coordinate y del mouse
@return l'indice della sezione scelta. -1 se non è una sezione valida.
*/
int gestione_mouse(int mouse_x, int mouse_y)
{
	//controllo se è un pulsante dei giochi
	for(int j=0, N=0, Y=s_centrale.y+s_centrale.h/4-pulsante.h/2; j!=2; j++, Y+=(s_centrale.h/2))
		for(int i=0, X=s_centrale.x+s_centrale.w/4-pulsante.w/2; i!=3; i++, N++, X+=(s_centrale.w/4))
			if( 	((mouse_x>=X) && (mouse_x<=X+pulsante.w))
		  		&&((mouse_y>=Y) && (mouse_y<=Y+pulsante.h)) ){

				return N;	//0-5
			}


	//Se è stata premuta l'uscita.
	int X=s_basso.x+s_basso.w/4-pulsante.w/2, Y=s_basso.y+s_basso.h/2-pulsante.h/2;

	if(((mouse_x>=X) && (mouse_x<=X+pulsante.w))
		&&((mouse_y>=Y) && (mouse_y<=Y+pulsante.h)) ){

			return 6;
		}

	//Nessuna sezione valida.
	return -1;
}
//-----------------------------FUNZIONI INTERNE----------------------------------------

/**Inizializza le sezioni della funzione ::schermata_principale.
*Le coordinate hanno per origine l'angolo alto sinistro dello schermo.
*Usa le struct ::sezione
*/
void init_sezioni_schermata_principale()
{
	s_titolo.x=0;	s_titolo.y=0;	s_titolo.w=wight;		s_titolo.h=100;
	s_centrale.x=0;	s_centrale.y=200;	s_centrale.w=wight;	s_centrale.h=500;
	s_basso.x=0;	s_basso.y=750;	s_basso.w=wight;		s_basso.h=100;
	pulsante.x=0;	pulsante.y=0;	pulsante.w=300;		pulsante.h=150;
}
