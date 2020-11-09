/**@file Modulo per la gestione della grafica e della lettura dell'input mouse sul display
*/

#include<cstring>
using namespace std;
#include"../allegro_header.h"
#include"../strutt_dati.h"
#include"struttura_dati.h"
#include"grafica.h"
//Variabili di file

/**Struttura contenente 14 puntatori ad immagine di tipo ALLEGRO_BITMAP
*/
struct st_figure{
	ALLEGRO_BITMAP *img[14];	/**<Contiene all'indice i la carta di valore i; 0 è carta coperta*/
} figure[4];				/**<Array di immagini-carta per seme*/


int carta_w=150,
carta_h=200,

carta_coperta_w=150,
carta_coperta_h=30,

mazzo_distanza=25,

fila_w=200,
fila_h=700,

p_alto_x=0,
p_alto_y=50,
p_alto_w=200,

p_uscita_x=0,
p_uscita_y=height-250,
p_uscita_w=200,
p_uscita_h=height-p_uscita_y,

menu_w=150,
menu_h=75,
menu_x=wight/2+150,
menu_y=height-(50+menu_h);

float spessore_selezione=3;

sezione mazzo, uscita[4], fila[7], menu;

//prototipi funzioni interne
bool inside(sezione, int, int);
void carica_figure();
char* crea_nome_file(char nome_file[50], int, int);
ALLEGRO_BITMAP *load_bitmap_at_size(const char *filename, int w, int h);
void disegna_selezione(int);

//------------------------INIZIO DEFINIZIONE-----------------------
//--------------------------FUNZIONI INTERFACCIA-------------------
/**Inizializza le sezioni per la gestione delle aree di input e grafiche.
*/
void init_grafica()
{
	int X=p_alto_x, Y=p_alto_y;
	mazzo.x=X;
	mazzo.y=Y;
	mazzo.w=p_alto_w;
	mazzo.h=carta_h*2+mazzo_distanza;

	 X+=p_alto_w;
	for(int i=0; i!=7; i++, X+=p_alto_w){
		fila[i].x=X;
		fila[i].y=Y;
		fila[i].w=p_alto_w;
		fila[i].h=p_uscita_y-p_alto_y;
	}
	X=p_uscita_x;
	Y=p_uscita_y;

	for(int i=0; i!=4; i++, X+=p_uscita_w){
		uscita[i].x=X;
		uscita[i].y=Y;
		uscita[i].w=p_uscita_w;
		uscita[i].h=p_uscita_h;
	}

	assert(menu_x+menu_w<=wight);
	assert(menu_y+menu_h<=height);
	menu.x=menu_x;
	menu.y=menu_y;
	menu.w=menu_w;
	menu.h=menu_h;

	carica_figure();
}
/**Disegna la schermata secondo lo stato della partita al momento della chiamata.
*/
void disegna_schermata (int selezione)
{
	int X, Y, bordo_x, bordo_y;
	st_carta *carta;

	ALLEGRO_COLOR 	col_sfondo=al_map_rgb(0,102,0),
				col_sottocarte=al_map_rgb(0,72,0),
				col_menu=al_map_rgb(151,11,161),
				col_f_menu=al_map_rgb(2,2,2);

	ALLEGRO_FONT *f_menu=al_load_ttf_font(path_main_font, 50 ,0);

	al_clear_to_color(col_sfondo);

	//mazzo
	X=mazzo.x+25; Y=mazzo.y+25;
	bordo_x= (mazzo.w-carta_w)/2;
	bordo_y=25;
	if(partita->mazzo!=NULL)
		al_draw_bitmap(figure[0].img[0], X, Y, 0);
	else
		al_draw_filled_rounded_rectangle(X, Y, X+carta_w, Y+carta_h, 10, 10, col_sottocarte);


	Y+=carta_h+mazzo_distanza;

	if(partita->c_mazzo!=NULL){
		carta=partita->c_mazzo;
		al_draw_bitmap(figure[carta->seme].img[carta->val], X, Y, 0);
	}else{
		al_draw_filled_rounded_rectangle(X, Y, X+carta_w, Y+carta_h, 10, 10, col_sottocarte);
	}

	//file
	bordo_x=(fila_w-carta_w)/2;
	bordo_y=25;

	for(int i=0; i!=7; i++){
		X=fila[i].x+bordo_x;
		Y=fila[i].y+bordo_y;
		int j;
		carta=partita->fila[i].prima_carta;
		if(partita->fila[i].n_carte!=0){

			for(j=1; j<=partita->fila[i].n_coperte; j++){
				al_draw_bitmap(figure[0].img[0], X, Y, 0);
				pausa_debug(1.0)
				Y+=carta_coperta_h;
				carta=carta->next;
			}
			for(; j<=partita->fila[i].n_carte; j++){
				al_draw_bitmap(figure[carta->seme].img[carta->val], X, Y, 0);
				pausa_debug(1.0)
				Y+=carta_coperta_h;
				carta=carta->next;
			}
		}

	}

	//uscita
	bordo_x=(p_uscita_w-carta_w)/2;
	bordo_y=25;

	for(int i=0; i!=4; i++){
		X=uscita[i].x+bordo_x;
		Y=uscita[i].y+bordo_y;
		if(partita->uscita[i]!=NULL){
			carta=partita->uscita[i]->prev;
			al_draw_bitmap(figure[carta->seme].img[carta->val], X, Y, 0);
		}else{
			al_draw_filled_rounded_rectangle(X, Y, X+carta_w, Y+carta_h, 10, 10, col_sottocarte);
			pausa_debug(1.0);
		}
	}

	//menu
	al_draw_filled_rectangle(menu.x, menu.y, menu.x+menu.w, menu.y+menu.h, col_menu);
	al_draw_text(f_menu, col_f_menu, menu.x+menu.w/2, menu.y+menu.h/2-40, ALLEGRO_ALIGN_CENTRE, "MENU");

	//selezione
	if( !((selezione==8)||(selezione==15)||(selezione==-1)) )
		disegna_selezione(selezione);

	pausa_debug(5.0)

	al_destroy_font(f_menu);
	al_flip_display();
}
/**Distrugge le immagini caricare e libera la memoria.
*/
void delete_grafica(){

	for(int i=0; i!=4; i++){
		for(int j=0; j!=14; j++){
			al_destroy_bitmap(figure[i].img[j]);

		}
	}
}


/**Gestisce la posizione del click mouse.
*Valuta se le coordinate passate sono all'interno di qualche sezione; in tal caso
*Restituisce l'indice di tale sezione.
*Utilizza la funzione ::inside in appoggio.
*@param[in] mouse_x le coordinate x del mouse
*@param[in] mouse_y le coordinate y del mouse

*@return -1 selezione non valida.
*@return 0-6 indice della fila.
*@return 8-9 mazzo parte sopra e sotto.
*@return 10-13 indice del mazzo d'uscita.
*@return 15 menu.
*/
int mouse_to_sezione (int mouse_x, int mouse_y)
{
	//controllo file
	for(int i=0; i!=7; i++){
		if(inside(fila[i], mouse_x, mouse_y))
			return i;
	}
	//controllo mazzo
	if(inside(mazzo, mouse_x, mouse_y)){
		if(mouse_y<=mazzo.y+mazzo.h/2)	//parte superiore
			return 8;
		else						//parte inferiore
			return 9;
	}
	//mazzi di uscita
	for(int i=10; i!=14; i++){
		if(inside(uscita[i%10], mouse_x, mouse_y))
			return i;
	}
	//menu
	if(inside(menu, mouse_x, mouse_y))
		return 15;
	//Nessuna sezione valida.
	return -1;
}
//-----------------------DEBUG
/**Disegna su schermo le sezioni
*Tali sezioni corrispondono alle aree rilevate al click del mouse.
*/
void debug_sezioni()
{
	al_clear_to_color(al_map_rgb(0,0,0));

	float spessore= 3;
	ALLEGRO_COLOR c_mazzo=al_map_rgb(0,0,204),
				c_fila=al_map_rgb(255,0,0),
				c_uscita=al_map_rgb(0,204,0),
				c_menu=al_map_rgb(76,0,153);

	al_draw_rectangle(mazzo.x, mazzo.y, mazzo.x+mazzo.w, mazzo.y+mazzo.h, c_mazzo, spessore);
	for(int i=0; i!=7; i++)
		al_draw_rectangle(fila[i].x, fila[i].y, fila[i].x+fila[i].w, fila[i].y+fila[i].h, c_fila, spessore);

	for(int i=0; i!=4; i++)
		al_draw_rectangle(uscita[i].x, uscita[i].y, uscita[i].x+uscita[i].w, uscita[i].y+uscita[i].h, c_uscita, spessore);

	al_draw_rectangle(menu.x, menu.y, menu.x+menu.w, menu.y+menu.h, c_menu, spessore);
	pausa_debug(5);
}

//--------------------------------_FUNZIONI INTERNE-----------------------------
//------------------------------------------------------------------------------

/**Check se le coordinate x e y sono all'interno della sezione
*@param[in] sez	La sezione da controllare
*@param[in] x	La coordinata x da controllare
*@param[in] y	La coordinata y da controllare
*
*@return true 	Se le coordinate sono all'interno della sezione
*@return false 	Se le coordinate sono esterne alla sezione
*/
bool inside(sezione sez, int x, int y)
{
	if((x>=sez.x)&&(x<=sez.x+sez.w)
		&&(y>=sez.y)&&(y<=sez.y+sez.h) ){
			return true;
		}
	return false;
}

/**Carica le immagini da mostrare a schermo.
*/
void carica_figure()
{
	step(,carica_figure,inizio)
	char	nome_file[50];

	for(int s=0; s!=4; s++){
		for(int v=0; v!=14; v++){

			figure[s].img[v]=load_bitmap_at_size(crea_nome_file(nome_file,s,v), carta_w, carta_h);
		}
	}
}

/**Restituisce il path della carta da caricare.
*Ricostruisce il path e il nome del file da caricare in base ai parametri passati.
*@return Puntatore alla stringa del path.
*/
char* crea_nome_file(char nome_file[50], int seme, int valore)
{
	step(,crea_nome_file, inizio)
	stampa(seme,) stampa(valore, )
	nome_file[0]='\0';
	char pref[]={"Solitario/data/Carte/"}, val[6]={'\0'}, intra[]={"_of_"}, sem[9]={'\0'}, end[]={".png"};

	switch(valore){
		case 0: strcpy(nome_file,"Solitario/data/Carte/coperta.png");
			return nome_file;
		case 1: strcpy(val,"ace");
			break;
		case 2: strcpy(val,"2");
			break;
		case 3: strcpy(val,"3");
			break;
		case 4: strcpy(val,"4");
			break;
		case 5: strcpy(val,"5");
			break;
		case 6: strcpy(val,"6");
			break;
		case 7: strcpy(val,"7");
			break;
		case 8: strcpy(val,"8");
			break;
		case 9: strcpy(val,"9");
			break;
		case 10: strcpy(val,"10");
			break;
		case 11: strcpy(val,"jack");
			break;
		case 12: strcpy(val,"queen");
			break;
		case 13: strcpy(val,"king");
			break;

		default: assert(false);

	}
	switch(seme){
		case cuori:	strcpy(sem, "hearts");
			break;
		case quadri:strcpy(sem, "diamonds");
			break;
		case fiori:strcpy(sem, "clubs");
			break;
		case picche:strcpy(sem, "spades");
			break;
		default: assert(false);
	}
	strcat(nome_file, pref);
	strcat(nome_file, val);
	strcat(nome_file, intra);
	strcat(nome_file, sem);
	strcat(nome_file, end);
	function_debug(cout<<endl<<nome_file<<flush;)
	return nome_file;
}
/**Carica e ridimensiona l'immagine del nome passato.
*La funzione carica la foto in dimensioni originali e la renderizza in un ALLEGRO_BITMAP
* delle dimensioni volute.
*Nel caso non esista il file, il programma si arresta per assert o ritorna NULL.<br>
*Si ringrazia Matthew di StackOverflow.
*@return il puntatore ALLEGRO_BITMAP dell'immagine.
*/
ALLEGRO_BITMAP *load_bitmap_at_size(const char *nome, int w, int h)
{
	ALLEGRO_BITMAP *ridimensionata, *originale, *prev_target;

	ridimensionata = al_create_bitmap(w, h);
  	if (!ridimensionata){
		function_debug(cout<<"\nERRORE 00 IN\t"<<nome<<flush;)
		assert(false);
		return NULL;
	}

	originale= al_load_bitmap(nome);
	if (!originale){
		function_debug(cout<<"\nERRORE 01 IN\t"<<nome<<flush;)
		assert(false);
		return NULL;
	}


	prev_target = al_get_target_bitmap();
	al_set_target_bitmap(ridimensionata);


	al_draw_scaled_bitmap(originale, 0, 0, al_get_bitmap_width(originale), al_get_bitmap_height(originale), 0, 0, w, h, 0);

	al_set_target_bitmap(prev_target);
	al_destroy_bitmap(originale);

	return ridimensionata;
}
/**Disegna il contorno della selezione corrente
*/
void disegna_selezione(int selezione)
{
	step( ,disegna_selezione, Inizio)
	stampa(selezione, )
	sezione s;
	switch(selezione){
		case 0:	case 1:	case 2:
		case 3:	case 4:	case 5:
		case 6:
				s=fila[selezione];
					break;

		case 10:	case 11:	case 12:
		case 13:
				s=uscita[selezione%10];
					break;
		case 9:
				s=mazzo;
				s.x+=12;
				s.w+= -12;
				s.y+= carta_h+37;
				s.h= s.h/2+20;
						break;
		default: step(, , errore default-switch) assert(false); return;
	}

	al_draw_rectangle(s.x, s.y, s.x+s.w, s.y+s.h, al_map_rgb(255, 0, 0), spessore_selezione);
	return;
}
