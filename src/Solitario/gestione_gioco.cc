/** @file Modulo per la gestione del gioco.
*Il modulo gestisce le mosse possibili in base all'input passato dal chiamante.
*Solitamente gli errori sono gestiti internamente, altrimenti è ritornato il valore
*di errore generico.
*/
#include<cstdlib>
using namespace std;
#include"../strutt_dati.h"
#include"struttura_dati.h"
#include"inizializzazione.h"
#include"gestione_gioco.h"

//variabili di file

/**Enumeratore della sezione per gli spostamenti di carte*/
enum en_caso {fila, mazzo_sup, mazzo_inf, uscita, menu,
			errore /**<Valore di debug, non ha significato per le altre funzioni*/};


//prototipi funzioni interne
en_caso assegna_caso(int);

int c_menu();
int mazzo_carta();
int mazzo_to_fila(int);
int mazzo_to_uscita(int);
int fila_to_fila(int, int);
int fila_to_uscita(int, int);
int uscita_to_fila(int, int);

bool compatibile(st_carta*, st_carta*);
int inserisci (st_carta *source, st_carta *&destinazione);
void dividi (st_carta *&testa, st_carta *nuova);

//------------------------INIZIO DEFINIZIONE-----------------------
//--------------------------FUNZIONI INTERFACCIA-------------------
/**Prende in ingresso l'indice di due sezioni e gestisce le azioni da intraprendere.
*restituisce l'esito dell'operazione per la scelta delle azioni da intraprendere
*@return 0 tutto ok OR errore gestito internamente.
*@return -1 segnale di chiusura gioco.
*@return -2 azione non lecita non gestita (errore comunicato dal chiamante).
*@return -99 in caso di errore critico.
*/
int gestore_mosse(int src, int dest)
{
	step(gestione_gioco.cc, gestore_mosse, inizio)
	stampa(src,) stampa(dest,)

	en_caso s, d;
	s=assegna_caso(src);
	d=assegna_caso(dest);

	if((s==errore)||(d==errore)){			//errore nell'assegnamento casi
		step(,,s|d==errore)
		assert(false);
		return -99;

	}else if ((partita->mazzo==NULL)		//controllo che non vengano eseguite operazioni sul mazzo quando è vuoto.
		&& ((d==mazzo_sup)||(d==mazzo_inf)||(s==mazzo_sup)||(s==mazzo_inf)) ){

		al_show_native_message_box(display, "Errore","Il mazzo è vuoto","Scegli un'altra azione", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;

	}else if((s==menu)||(d==menu)){
		return c_menu();

	}else if((s==mazzo_sup|| d==mazzo_sup)){
		if(partita->mazzo!=NULL)
			return mazzo_carta();
		else{

			return 0;
		}
	}else{

		switch(s){
			case mazzo_inf:
				switch(d){
					case fila: return mazzo_to_fila(dest);
					case uscita: return mazzo_to_uscita(dest);
					default: return -2;
				}

			case fila:
				switch(d){
					case fila: return fila_to_fila(src, dest);
					case uscita: return fila_to_uscita(src, dest);
					default: return -2;
				}

			case uscita:
				switch(d){
					case fila: return uscita_to_fila(src, dest);
					default: return -2;
				}

			default: return -2;
			}
		}
	step(, gestore_mosse, Dopo-switch)
	assert(false);
	return -99;
}

/**Estrae la carta passata da una lista doppia.
*Rimuove i campi next e prev a NULL della carta passata; chiude inoltre il "buco"
*collegando il prev e next coi rispettivi campi.
*@return Indirizzo della carta estratta;
*/
void estrai(st_carta *carta)
{

	step(gestione_gioco.cc, estrai, inizio estrazione)
	stampa(carta->seme,)
	stampa(carta->val, )
	st_carta *precedente= carta->prev,
			*successivo= carta->next;

	assert(successivo!=NULL);
	assert(precedente!=NULL);
	precedente->next=successivo;
	successivo->prev=precedente;
	carta->next=carta->prev=NULL;
	step(,,fine estrazione)
}
//-------------------FUNZIONI INTERNE-----------------------------
//----------------------------------------------------------------

/** Restituisce il caso dell'indice-sezione passato.
*/
en_caso assegna_caso(int a)
{
	switch (a) {
		case 0: case 1:
		case 2: case 3:
		case 4: case 5:
		case 6:		return fila;

		case 8:		return mazzo_sup;
		case 9:		return mazzo_inf;

		case 10: case 11:
		case 12: case 13:	return uscita;

		case 15: return menu;

		default: step(gestione_gioco.cc, assegna_caso, errore in switch_case) stampa(a,) assert(false) ; return errore;
	}
}

/**Stampa e gestice il menu a schermo.
*Richiede se si vuole iniziare una nuova partita o uscire dal gioco.
*Nel caso di nuova partita inizializza in automatico il nuovo stato. <br>
*Vedere documentazione di ::gestore_mosse per i valori di ritorno
*/
int c_menu()
{
	step(gestione_gioco.cc, menu, inizio)
	int scelta;
	scelta=al_show_native_message_box(display, "MENU", "Cosa vuoi fare?", "Clicca sull'azione desiderata.",
						"Nuova partita | Esci dal gioco | Annulla", ALLEGRO_MESSAGEBOX_QUESTION);

	stampa(scelta, menu)
	switch(scelta){
		case 1: init_partita(); return 0;
		case 2: return -1;
		case 3:
		case 0: return 0;
		default:step(,menu, errore in switch_default); assert(false); return -99;
	}
}

/**Gestice il cambio della carta visibile dal mazzo_carta
*Vedere documentazione di ::gestore_mosse per i valori di ritorno
*/
int mazzo_carta()
{
	if(partita->c_mazzo!=NULL){
		partita->c_mazzo= partita->c_mazzo->next;
		return 0;
	}else{//estraggo per la prima volta
		partita->c_mazzo=partita->mazzo;
		return 0;
	}
	step(,mazzo_carta, errore impossibile O.o)
	assert(false);
	return -99;
}

/**Gestice lo spostamento dal mazzo alla fila indicata.
*Controlla che lo spostamento rispetti le regole del gioco e in tal caso gestice
*i casi possibili per evitare segmentation-fault. <br>
*Vedere documentazione di ::gestore_mosse per i valori di ritorno
*/
int mazzo_to_fila(int i)
{
	step(, mazzo_to_fila, Inizio)

	st_carta *estratta= partita->c_mazzo,
			*destinazione= partita->fila[i].prima_carta ? partita->fila[i].prima_carta->prev : NULL;

	if(compatibile(estratta, destinazione)){
		if(estratta==partita->mazzo){		//carta puntata dal mazzo
			assert((estratta==partita->mazzo) && (estratta==partita->c_mazzo)		//le tre carte DEVONO essere uguali
				&& (partita->c_mazzo==partita->mazzo) );

			partita->mazzo= partita->mazzo->next;
			partita->c_mazzo=partita->c_mazzo->next;

			if(partita->mazzo==estratta){		//ultima carta rimasta nel mazzo se l'incremento rimane uguale
				partita->mazzo=NULL;
				partita->c_mazzo=NULL;
			}

		}else{	//carta qualsiasi del mazzo
			partita->c_mazzo= partita->c_mazzo->next;
		}
	}else{			//non sono compatibili
		al_show_native_message_box(display, "ATTENTO!", "Le due carte non sono concatenabili",
	 						"Scegli un'altra azione o avvia un'altra partita", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}

	partita->fila[i].n_carte++;
	estrai(estratta);
	return inserisci(estratta, partita->fila[i].prima_carta);
}

/**Gestisce lo spostamento dal mazzo all'uscita
*Controlla che lo spostamento rispetti le regole. In caso di irregolarità lo comunica
*al giocatore tramite pop-up.
*Il puntatore destinazione può venire modificato se nullo<br>
*Vedere documentazione di ::gestore_mosse per i valori di ritorno.
*/
int mazzo_to_uscita(int i)
{
	step(, mazzo_to_uscita, inizio)
	i=i%10;
	st_carta 	*estratta = partita->c_mazzo,
			*destinazione = partita->uscita[i] ? partita->uscita[i]->prev : NULL;

	//Uscita non corretta
	if(estratta->seme != i){
		al_show_native_message_box(display, "ATTENTO", "Impossibile far uscire la carta",
							"Controlla che sia il mazzo giusto o scegli un'altra azione", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}
	stampadd(destinazione,)
	if(destinazione==NULL){

		if(estratta->val==1){
			partita->c_mazzo= partita->c_mazzo->next;
			if(estratta==partita->mazzo){
				partita->mazzo= partita->mazzo->next;
				if(partita->mazzo==estratta){		//ultima carta rimasta nel mazzo se l'incremento rimane uguale
					partita->mazzo=NULL;
					partita->c_mazzo=NULL;
				}
			}
			estrai(estratta);
			return inserisci(estratta, partita->uscita[i]);
		}else{
			al_show_native_message_box(display, "ATTENTO", "Impossibile far uscire la carta",
								"La prima carta deve essere un asso", 0, ALLEGRO_MESSAGEBOX_ERROR);
			return 0;
		}
	}else{	//uscita non vuota

		if(destinazione->val+1 == estratta->val ){
			step(,,inserimento con carte presenti)
			assert(destinazione->seme == estratta->seme);

			partita->c_mazzo= partita->c_mazzo->next;
			if(estratta==partita->mazzo){
				partita->mazzo= partita->mazzo->next;
				if(partita->mazzo==estratta){		//ultima carta rimasta nel mazzo se l'incremento rimane uguale
					partita->mazzo=NULL;
					partita->c_mazzo=NULL;
				}
			}
			estrai(estratta);
			return inserisci(estratta, partita->uscita[i]);

		}else{
			al_show_native_message_box(display, "ATTENTO", "Impossibile far uscire la carta",
								"Le carte devono essere in ordine crescente", 0, ALLEGRO_MESSAGEBOX_ERROR);
			return 0;
		}
	}

	assert(false);
	return -99;
}

/**Gestisce lo spostamento da una fila all'altra.
*Controlla che lo spostamento rispetti le regole e in caso di irregolarità lo
*comunica al giocatore. Effettua in automatico la ricerca del possibile spostamento <br>
*Vedere documentazione di ::gestore_mosse per i valori di ritorno.
*/
int fila_to_fila(int i, int j)
{
	step(, fila_to_fila, inizio)

	if(partita->fila[i].prima_carta==NULL){
		al_show_native_message_box(display, "ATTENTO", "Impossibile spostare la carta",
							"la fila di partenza è vuota", 0 , ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}

	st_carta *testa=partita->fila[i].prima_carta->prev,
		*destinazione= (partita->fila[j].prima_carta) ? partita->fila[j].prima_carta->prev : NULL;

	int n_cicli_max= partita->fila[i].n_carte - partita->fila[i].n_coperte;

	for(int c=1; c<=n_cicli_max; c++){
		if(!compatibile(testa, destinazione)){
			testa=testa->prev;
			stampa(c, )
		}
		else{
			stampa(c, in else)
			partita->fila[j].n_carte+=c;
			partita->fila[i].n_carte-=c;

			step(,,spostamento)
			dividi(partita->fila[i].prima_carta, testa);
			inserisci(testa, partita->fila[j].prima_carta);		//ci pensa inserisci() a portare a NULL il puntatore della fila nel caso


			if(partita->fila[i].n_carte==0)
				partita->fila[i].prima_carta=NULL;

			if((partita->fila[i].n_coperte!=0)&&(partita->fila[i].n_carte==partita->fila[i].n_coperte))
				partita->fila[i].n_coperte--;

			step(,,fine da ciclo for)
			return 0;
		}
	}

	al_show_native_message_box(display, "ATTENTO", "Impossibile spostare le carte",
						"Non è stato rilevato alcuno spostamento possibile", 0, ALLEGRO_MESSAGEBOX_ERROR);

	return 0;

}

/**Gestisce lo spostamento da una fila all'uscita.
*Controlla che lo spostamento rispetti le regole e in caso di irregolarità lo
*comunica al giocatore.<br>
*Vedere documentazione di ::gestore_mosse per i valori di ritorno.
*/
int fila_to_uscita(int i, int j)
{
	step(, fila_to_uscita, inizio)
	j= j%10;

	if(partita->fila[i].prima_carta==NULL){
		al_show_native_message_box(display, "ATTENTO", "Impossibile spostare la carta",
							"La fila di partenza è vuota", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}
	step(,,pre-assegnamento)
	st_carta *uscente = partita->fila[i].prima_carta->prev,
			*destinazione= (partita->uscita[j])? partita->uscita[j]->prev : NULL;
	stampa(uscente->val,)
	stampa(uscente->seme,)
	stampadd(destinazione,)

	if(uscente->seme!=j){
		al_show_native_message_box(display, "ATTENTO", "Mazzo di uscita col seme sbagliato",
								"I mazzi sono nell'ordine Cuori-Quadri-Fiori-Picche", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}
	if(destinazione==NULL){
		if(uscente->val==1){
			assert(uscente->seme==j);
			estrai(uscente);
			inserisci(uscente, partita->uscita[j]);
			partita->fila[i].n_carte--;

			if(partita->fila[i].n_carte==0)
				partita->fila[i].prima_carta=NULL;
			partita->fila[i].n_coperte-= (partita->fila[i].n_carte==partita->fila[i].n_coperte)? 1:0;
			return 0;
		}else{
			al_show_native_message_box(display, "ATTENTO", "Impossibile far uscire la carta",
								"La prima carta in uscita deve essere un asso", 0, ALLEGRO_MESSAGEBOX_ERROR);
			return 0;
		}
	}else{	//mazzo di uscita con carte
		if(uscente->val==destinazione->val+1){
			assert(uscente->seme==destinazione->seme);
			estrai(uscente);
			inserisci(uscente, partita->uscita[j]);
			partita->fila[i].n_carte--;

			if(partita->fila[i].n_carte==0)
				partita->fila[i].prima_carta=NULL;
			partita->fila[i].n_coperte-= (partita->fila[i].n_carte==partita->fila[i].n_coperte)? 1:0;

			return 0;
		}else{
			al_show_native_message_box(display, "ATTENTO", "Impossibile far uscire la carta",
								"Le carte devono uscire in valore crescente", 0, ALLEGRO_MESSAGEBOX_ERROR);
			return 0;
		}
	}
	step(,,errore impossibile)
	assert(false);
	return -99;


}

/**Gestisce il rientro in gioco di una carta.
*Controlla che lo spostamento rispetti le regole e in caso di irregolarità lo
*comunica al giocatore.<br>
*Vedere documentazione di ::gestore_mosse per i valori di ritorno.
*/
int uscita_to_fila(int i, int j)
{
	step(,uscita_to_fila, inizio)

	i=i%10;
	if(partita->uscita[i]==NULL){
		al_show_native_message_box(display, "ATTENTO", "Impossibile far tornare in gioco la carta",
							"Il mazzo di uscita è vuoto", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}
	st_carta *carta=partita->uscita[i]->prev,
			*destinazione= (partita->fila[j].prima_carta)? partita->fila[j].prima_carta->prev : NULL;

	if(compatibile(carta, destinazione)){
		estrai(carta);
		inserisci(carta, partita->fila[j].prima_carta);
		partita->fila[j].n_carte++;
		return 0;
	}else{
		al_show_native_message_box(display, "ATTENTO", "Impossibile far tornare in gioco la carta",
							"Le due carte non sono compatibili", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}
	step(,,errore impossibile)
	assert(false);
	return -99;
}

/**Afferma se la carta source può essere sotto alla carta destinazione nelle <b>FILE</b>.
*Lasciare prima il controllo valore poichè è leggermente meno costoso da effettuare.
*@return true se la source può essere spostata sotto la destinazione
*@return false se lo spostamento è contrario alle regole
*/
bool compatibile(st_carta *source, st_carta *destinazione)
{
	step(,compatibile, inizio)
	stampa(source->seme,)
	stampa(source->val,)

	if(source==NULL){
		assert(source!=NULL);
		return false;
	}


	//fila vuota
	if(destinazione==NULL){
		step(,,destinazione vuota)
		if(source->val==13)
			return true;
		else
			return false;
	}

	stampa(destinazione->seme,)
	stampa(destinazione->val,)
	//fila non vuota
	//controllo valore

	if( (source->val)!=(destinazione->val-1) ){
		step(,, valore non compatibile)
		return false;
	}

	//controllo colore
	enum en_colore {rosso, nero};
	en_colore c_source, c_dest;


	c_source= ((source->seme==cuori) || (source->seme==quadri)) ? rosso :nero;
	c_dest= ((destinazione->seme==cuori) || (destinazione->seme==quadri)) ? rosso :nero;

	stampa(c_source,)
	stampa(c_dest,)

	if(c_source==c_dest)
		return false;


	return true;
}

/**Inserisce una doppia lista chiusa in coda a un'altra.
*I parametri passati si intendono come puntatori alla testa della lista;
*al suo precedente sarà attaccata la lista source. <br>
*Nel caso la source sia una carta singola (ovvero senza linker), viene creata una lista con un elemento.
*Nel caso la destinazione sia vuota, viene cambiato il puntatore e punterà alla source. <br>
*La funzone non effettua nessun controllo di correttezza sulle regole del gioco.
*@return 0  in caso di esito positivo
@return -99 in caso di source passata vuota.
*/
int inserisci (st_carta *source, st_carta *&destinazione)
{
	step(gestione_gioco.cc, inserisci, inizio)

	if(source==NULL){
		step(,, puntatore source vuoto)
		return -99;
	}else{
		stampa(source->val,)
		stampa(source->seme,)
		assert( ((source->next==NULL)&&(source->prev==NULL))			//controllo che sia o carta singola o una lista non-corrotta
	 		|| ((source->next!=NULL)&&(source->prev!=NULL)) );

		//rendo la carta singola una lista doppia chiusa per accorpare tutto al caso singolo
		if(source->next==NULL)
			source->next=source;
		if(source->prev==NULL)
			source->prev=source;
	}

	st_carta	*s_coda=source->prev, *d_coda;

	if(destinazione==NULL){
		destinazione=source;
	}else{	//destinazione non vuota
		d_coda=destinazione->prev;

		destinazione->prev=s_coda;
		d_coda->next=source;
		source->prev=d_coda;
		s_coda->next=destinazione;
	}
	step(, inserisci, fine)
	return 0;
}
/**Divide una lista doppia circolare in due.
*Nel caso che i due parametri coincidato, il primo puntatore è portato a NULL.
*Non è effettuato alcun controllo per verificare che entrambi i puntatori puntino alla
*stessa lista.
*@param[in] nuovo		Indica il primo elemento della nuova lista da creare.
*/
void dividi (st_carta *&testa, st_carta *nuovo)
{
	step(,dividi, inizio)
	stampa(testa->seme,)	stampa(testa->val,)
	stampa(nuovo->seme,)	stampa(nuovo->val,)

	assert(nuovo!=NULL);

	if(testa==nuovo){
		testa=NULL;
		return;
	}

	st_carta *t_coda, *n_coda;
	t_coda=nuovo->prev;
	n_coda=testa->prev;

	nuovo->prev=n_coda;
	n_coda->next=nuovo;

	testa->prev=t_coda;
	t_coda->next=testa;

	step(, dividi, fine)
	return;
}
