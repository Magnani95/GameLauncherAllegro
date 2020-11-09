export
OBJ = 	main.o 	main_grafica.o
SUBDIRS = Solitario
SOLITARIO =	Solitario/solitario.o 	Solitario/gestione_gioco.o	Solitario/grafica.o	Solitario/inizializzazione.o
DEB = -g -D DEBUG
CXXFLAGS=-Wall

LDFLAGS = `pkg-config --cflags --libs \
		allegro-5 allegro_dialog-5 allegro_primitives-5 \
		allegro_font-5 allegro_ttf-5 allegro_image-5`

Progetto-Allegro: $(OBJ) all
	g++ $(OBJ) $(LDFLAGS)  $(SOLITARIO) -o $@

#scorpire come fare assegnamenti di variabili senza avere un target
deb: CXXFLAGS+=$(DEB)
deb: cleanall all Progetto-Allegro

-include dependencies.txt

#-------------------PHONY target
#compilazione di tutte le sottocartelle
.PHONY: all $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

#phony comuni
.PHONY: clean cleanall

depend:
	g++ -MM *.cc > dependencies.txt
	 $(MAKE) -C $(SUBDIRS) depend

clean:
	rm -f -v *.o
	$(MAKE) -C $(SUBDIRS) clean
cleanall:
	rm -f -v Progetto-Allegro *.o
	$(MAKE) -C $(SUBDIRS) cleanall
