HEADERS=oberon.h store.h reader.h domain.h alien.h typeregister.h

odcread: odcread.o reader.o store.o util.o alien.o typeregister.o
	g++ -o $@ $^

%.o: %.cc $(HEADERS)
	g++ -c -I. -o $@ $<
