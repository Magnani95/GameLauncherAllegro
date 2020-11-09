/**@file Modulo per l'inizializzazione strutturale del programma e il suo debug.
*Le funzioni servono a inizializzare lo stato della partita e a de-allocare la memoria dinamica
*al termine. Contiene anche le funzioni di debug per il controllo della correttezza dello stato partita.
*/

#include<cstdlib>
#include<iostream>
using namespace std;
#include"../strutt_dati.h"
#include"struttura_dati.h"
#include"gestione_gioco.h"

//Prototipi funzioni
st_carta* genera_distributore(void);
void init_file(st_carta *&distributore);
st_carta* estrai_distributore(st_carta *&distributore);
void init_mazzo(st_carta *&distributore);
void delete_partita();


//------------------------INIZIO DEFINIZIONE-----------------------
//--------------------------FUNZIONI INTERFACCIA-------------------
/**Inizializza la partita.
*Inizializza tutte le strutture dati e distribuisce le carte in modo casuale per
*avviare una partita. <br>Gestisce anche il caso ci sia già una partita inizializzata.
*/
void init_partita()
{
	int scelta=-2;
	if(partita!=NULL){
		do{
			scelta=al_show_native_message_box(display, "Attenzione!","Una partita è in corso!",
				"L'attuale partita verrà cancellata. Vuoi iniziarne un'altra?", NULL, ALLEGRO_MESSAGEBOX_YES_NO);
		}while(scelta==0);

		if(scelta==1)	//Yes
			delete_partita();
		else //(scelta==2)	//no
			return;
	}

	partita= new st_partita;

	st_carta *distributore = genera_distributore();

	init_file(distributore);
	init_mazzo(distributore);

	for(int i=0; i!=4;i++){
		partita->uscita[i]=NULL;
	}


	dblista(distributore,)
}

/**Dealloca tutte le carte e cancella la partita.
*Rimuove la memoria dinamica distribuita tra le varie liste circolari.
*/
void delete_partita()
{
	step(inizializzazione.cc, delete_partita, inizio)

	st_carta *attuale, *successivo, *testa;

	//distruttore carte file;
	step(, , distruttore file)
	for(int i=0; i!=7; i++){
		stampa(i, )
		testa=attuale= partita->fila[i].prima_carta;
		successivo=NULL;

		if(partita->fila[i].prima_carta!=NULL){
			for(int i=0; (attuale!=testa)||(i!=0); i++){
				assert(attuale!=NULL);
				successivo=attuale->next;
				delete attuale;
				attuale=successivo;
			}

		}

	}
	//distruttore mazzo;
	step(,,Distruttore mazzo)
	if(partita->mazzo!=NULL){
		testa=attuale=partita->mazzo;
		successivo=NULL;
		for(int i=0; (attuale!=testa)||(i!=0); i++){
			assert(attuale!=NULL);
			successivo=attuale->next;
			delete attuale;
			attuale=successivo;
		}
	}
	//distruttore mazzi di uscita
	step(,,Distruttore mazzi uscita)
	for(int i=0; i!=4; i++){
		stampa(i,)
		testa=attuale= partita->uscita[i];
		successivo=NULL;

		stampadd(partita->uscita[i],)
		if(partita->uscita[i]!=NULL){
			for(int i=0; (attuale!=testa)||(i!=0); i++){
				assert(attuale!=NULL);
				successivo=attuale->next;
				delete attuale;
				attuale=successivo;
			}

		}

	}

	delete partita;
	partita=NULL;
	step(,,fine distruttore)

}
//-----------DEBUG-----------------
/**Funzione di debug; stampa lo stato attuale della partita.
*Viene stampato a cout lo stato attuale della partita e quindi della file, mazzo e uscite.
*il formato della stampa è di [seme(valore)];
*/
void stampa_stato_partita()
{
	step(inizializzazione.cc, stampa_stato_partita, inizio)

	st_carta *attuale, *successivo, *testa;

	//file
	step(, , file)
	for(int i=0; i!=7; i++){
		stampa(i, )
		testa=attuale= partita->fila[i].prima_carta;
		successivo=NULL;
		stampadd(partita->fila[i].prima_carta,)
		cout<<"\nFila n\t"<<i<<flush;
		cout<<"\nn_carte\t"<<static_cast<int>(partita->fila[i].n_carte)<<flush;
		cout<<"\nn_coperte\t"<<static_cast<int>(partita->fila[i].n_coperte)<<endl<<flush;
		if(partita->fila[i].prima_carta!=NULL){
			for(int i=0; (attuale!=testa)||(i==0); i++){
				assert(attuale!=NULL);
				successivo=attuale->next;
				cout<<attuale->seme<<"("<<static_cast<int>(attuale->val)<<")  "<<flush;
				attuale=successivo;
			}
		}

	}
	//mazzo;
	cout<<"\n\nMAZZO\n"<<flush;

	if(partita->mazzo!=NULL){
		testa=attuale=partita->mazzo;
		successivo=NULL;
		for(int i=0; (attuale!=testa)||(i==0); i++){
			assert(attuale!=NULL);
			successivo=attuale->next;
			cout<<attuale->seme<<"("<<static_cast<int>(attuale->val)<<")  "<<flush;
			attuale=successivo;
		}
	}
	//mazzi di uscita
	cout<<"\n\nUSCITA\n"<<flush;
	for(int i=0; i!=4; i++){
		stampa(i,)
		testa=attuale= partita->uscita[i];
		successivo=NULL;

		stampadd(partita->uscita[i],)
		cout<<"\n";
		if(partita->uscita[i]!=NULL){
			for(int i=0; (attuale!=testa)||(i==0); i++){
				assert(attuale!=NULL);
				successivo=attuale->next;
				cout<<attuale->seme<<"("<<static_cast<int>(attuale->val)<<")  "<<flush;
				attuale=successivo;
			}
		}

	}
	step(,,fine stampa)


}
/**Funzione di debug: controlla la correttezza della partita.
*Controlla che tutte le carte siano presenti in gioco e che siano raggiungibili.<br>
*In caso contrario stampa le carte non raggiungibili.
*@return true se lo stato della partita è corretto.
*@return false se ci sono carte non raggiungibili.
*/
bool check_carte()
{
	step(inizializzazione.cc, check_carte, inizio)
	int carte [4][14];
	for(int i=0; i!=4; i++)
		for(int j=0;j!=13; j++)
			carte[i][j]=-1;


	st_carta *attuale, *successivo, *testa;

	//file
	step(, , file)
	for(int i=0; i!=7; i++){
		stampa(i, )
		testa=attuale= partita->fila[i].prima_carta;
		successivo=NULL;

		if(partita->fila[i].prima_carta!=NULL){
			for(int i=0; (attuale!=testa)||(i==0); i++){
				assert(attuale!=NULL);
				successivo=attuale->next;
				carte[attuale->seme][attuale->val]=i;
				attuale=successivo;
			}
		}

	}
	//mazzo;

	if(partita->mazzo!=NULL){
		testa=attuale=partita->mazzo;
		successivo=NULL;
		for(int i=0; (attuale!=testa)||(i==0); i++){
			assert(attuale!=NULL);
			successivo=attuale->next;
			carte[attuale->seme][attuale->val]=8;
			attuale=successivo;
		}
	}
	//mazzi di uscita
	for(int i=0; i!=4; i++){
		stampa(i,)
		testa=attuale= partita->uscita[i];
		successivo=NULL;

	stampadd(partita->uscita[i],)
	if(partita->uscita[i]!=NULL){
		for(int i=0; (attuale!=testa)||(i==0); i++){
			assert(attuale!=NULL);
			successivo=attuale->next;
			carte[attuale->seme][attuale->val]=10+i;
			attuale=successivo;
		}
	}

	}
	bool correttezza=true;
	for(int i=0; i!=4; i++)
		for(int j=1; j!=14; j++)
			if(carte[i][j]==-1){
				cout<<"\nERRORE CARTA "<<i<<"("<<j<<")\n";
				correttezza=false;
			}


	return correttezza;
	step(,check_carte, fine)
}

//---------------------FUNZIONI INTERNE------------------------

/**Genera 52 carte in lista doppia circolare
*Per la distribuzione casuale delle carte, viene generata una lista circolare contente le 52 carte in ordine di seme e valore; <br>
*Tale implementazione permette più rapidità negli spostamenti di carte e un metodo a minor "failure-rate" di un
*array o una matrice durante la distribuzione iniziale.
*@return indirizzo del puntatore alla prima carta.
*/
st_carta* genera_distributore(void)
{
	step(inizializzazione.cc, genera_distributore, inizio)

	en_seme semi[4]={cuori, quadri, fiori, picche};
	st_carta *distributore= new st_carta;
	st_carta *attuale= distributore;

	step(inizializzazione.cc, genera_distributore, ciclo-for carte)
	attuale->seme=cuori;
	attuale->val=1;
	for(int i=1; i!=52; i++){
		stampa(i, )
		step(,,001)
		attuale->next=new st_carta;
		step(,,002)
		attuale->next->prev=attuale;
		step(,,003)
		attuale=attuale->next;

		attuale->seme=semi[i/13];
		attuale->val=1+(i%13);
	}
	step(inizializzazione.cc, genera_distributore, post-ciclo)
	attuale->next=distributore;
	distributore->prev=attuale;

	return distributore;

}

/**Inizializza le file del Solitario.
*Estrae le carte dal distributore per assegnarle alle file.
*/
void init_file(st_carta *&distributore)
{
	step(inizializzazione.cc, init_file, inizio funzione)
	st_carta *attuale;
	for(int i=0; i!=7; i++){
		stampa(i, init_file )
		partita->fila[i].n_coperte=i;
		partita->fila[i].prima_carta=estrai_distributore(distributore);
		attuale=partita->fila[i].prima_carta;

		for(partita->fila[i].n_carte=1; partita->fila[i].n_carte!=i+1; partita->fila[i].n_carte++){
			stampa(partita->fila[i].n_carte, init_file )
			attuale->next=estrai_distributore(distributore);
			attuale->next->prev=attuale;
			attuale=attuale->next;
		}
		stampa(partita->fila[i].n_carte,)
		assert(partita->fila[i].n_carte==i+1);
		attuale->next=partita->fila[i].prima_carta;
		partita->fila[i].prima_carta->prev= attuale;
	}

}
/**Estrae una carta casuale dal distributore.
*La carta estratta non ha alcun collegamento next o prev.<br>
*Cambia al next l'indirizzo di distributore nel caso la carta estratta sia quella puntata da
*distributore. Nel caso sia estratta l'ultima carta, distributore è portato a NULL.<br>
*Utilizza la funzione ::estrai in @link gestione_gioco.cc @endlink.
*@return Indirizzo della carta estratta.
*@return NULL se il distributore è vuoto.
*/
st_carta* estrai_distributore(st_carta *&distributore)
{
	step(inizializzazione.cc, estrai_distributore, inizio)
	if(distributore==NULL)
		return NULL;

	int scelta=rand()%104;
	stampa(scelta, )
	st_carta *attuale=distributore;
	for(int i=0; i!=scelta; i++)
		attuale=attuale->next;

	assert(attuale!=NULL);
	if (attuale==distributore){		//se l'estratta è la prima carta
		step( , , estratta carta testa puntata dal distributore)
		distributore=attuale->next;
	}
	if(attuale==attuale->next){		//Ultima carta del distributore
		assert(attuale==distributore);
		assert(attuale==attuale->next);
		assert(attuale->prev==attuale->next);
		step(, , caso ultima carta del distributore)
		distributore=NULL;
	}
	estrai(attuale);

	return attuale;
}

/**Inizializza il mazzo in lista circolare doppia.
*Estrae dal distributore le carte restanti fino a svuotarlo.
*c_mazzo viene fatto puntare alla prima carta del mazzo.
*/
void init_mazzo(st_carta *&distributore)
{
	step(inizializzazione.cc, init_mazzo, inizio)
	st_carta *attuale, *estratta;
	int n_carte=1;
	partita->mazzo=attuale=estrai_distributore(distributore);
	estratta=estrai_distributore(distributore);

	while(estratta!=NULL){
		n_carte++;
		assert(estratta!=NULL);
		attuale->next=estratta;
		estratta->prev=attuale;
		attuale=attuale->next;
		estratta=estrai_distributore(distributore);
	}
	stampa(n_carte,)
	assert(n_carte==24);
	assert(attuale!=NULL);
	attuale->next=partita->mazzo;
	partita->mazzo->prev=attuale;

	partita->c_mazzo=NULL;
	step(,init_mazzo, fine)
}
