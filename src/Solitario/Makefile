LDFLAGS = `pkg-config --cflags --libs \
		allegro-5 allegro_dialog-5 allegro_primitives-5 \
		allegro_font-5 allegro_ttf-5 allegro_image-5`

solitario.o: gestione_gioco.o grafica.o inizializzazione.o

-include dependencies.txt

.PHONY: clean cleanall debug

depend:
	g++ -MM *.cc > dependencies.txt

clean:
	rm -f -v *.o
cleanall:
	rm -f -v *.o
