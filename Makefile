HEADERS=oberon.h store.h reader.h domain.h alien.h

odcread: odcread.o reader.o store.o util.o alien.o
	g++ -o $@ $^

%.o: %.cc $(HEADERS)
	g++ -c -I. -o $@ $<
