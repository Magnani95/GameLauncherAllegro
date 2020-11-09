/**@file
*Strutture dati, variabili e header/librerie comuni a tutto il programma.
*Qualunque entità da propagarsi ovunque è consigliato aggiungerla qui.
*Per il debug guardare @link debug.h @endlink
*/
#include<allegro5/allegro.h>
#include<allegro5/allegro5.h>
#include<allegro5/allegro_native_dialog.h>
#include"debug.h"

/**Costante contentente larghezza della schermata di Allegro.*/
extern const int wight,
/**Costante contentente Altezza della schermata di Allegro.*/
		height;
/** Pointer al tipo ALLEGRO_DISPLAY*/
extern ALLEGRO_DISPLAY *display;

extern char path_main_font[];

/**Struct per indicare le sezioni nelle schermate.
*Indica le coordinate x e y in alto a sinistra della sezione;
*w e h indicano la larghezza(verso sx) e altezza(verso il basso).
*/
struct sezione{
	int x;
	int y;
	int w;
	int h;
};
/** Struct contente 6 puntatori a stringa, ovvero i 6 giochi di una pagina
*da disegnare a schermo.
*/
struct pagina_giochi{
	char nome_gioco[6][11];
};
