#include "reader/reader.ih"

namespace odc {

Store* Reader::readStore() {
	SHORTCHAR kind = readSChar();
	if (kind == Store::NIL) {
		//std::cout << "NIL STORE" << std::endl;
		return readNilStore();
	} else if (kind == Store::LINK) {
		//std::cout << "LINK STORE" << std::endl;
		return readLinkStore();
	} else if (kind == Store::NEWLINK) {
		//std::cout << "NEWLINK STORE" << std::endl;
		return readNewLinkStore();
	} else if (kind == Store::STORE) {
		//std::cout << "STORE STORE" << std::endl;
		return readStoreOrElemStore(false);
	} else if (kind == Store::ELEM) {
		//std::cout << "ELEM STORE" << std::endl;
		return readStoreOrElemStore(true);
	} else {
		//std::cout << std::hex << (unsigned int)kind << std::endl;
		throw 20;
	}
}

} // namespace odc
