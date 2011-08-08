#include <iostream>
#include <fstream>
#include <string>

#include <oberon.h>
#include <reader.h>
#include <store.h>

namespace odc {
	Store* importDocument(std::istream &is) {
		const INTEGER docTag = 0x6F4F4443;
		const INTEGER docVersion = 0;
		Reader r(is);
		INTEGER tag = r.readInt();
		if (tag == docTag) {
			INTEGER version = r.readInt();
			if (version != docVersion) {
				throw 100;
			}
			Store *s = r.readStore();
			return s;
		}
		return 0;
	}
}

int main(int argc, char *argv[]) {
	std::ifstream in(argv[1], std::ios::in | std::ios::binary);
	odc::Store* s = odc::importDocument(in);
	std::cout << s->toString() << std::endl;
	std::cout << in.tellg() << " " << in.eof() << std::endl;
	return 0;
}
