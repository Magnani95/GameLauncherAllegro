/**@file
*Interfaccia del modulo @link main_grafica.cc @endlink
*/
void schermata_principale(pagina_giochi pagina);
void inizializzazione_giochi(pagina_giochi *pagina, void ( (*giochi[]) (void)) );
bool inizializzazione_allegro();
int gestione_mouse(int x, int y);
