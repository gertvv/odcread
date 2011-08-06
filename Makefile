HEADERS=oberon.h store.h reader.h domain.h

odcread: odcread.o reader.o store.o util.o
	g++ -o $@ $^

%.o: %.cc $(HEADERS)
	g++ -c -I. -o $@ $<
