#include "store/module.ih"

namespace odc {

const std::string Elem::TYPENAME("Stores.Elem^");
const TypeProxy<Elem, Store> Elem::PROXY;

Elem::Elem(INTEGER id) : Store(id) {}

const std::string &Elem::getTypeName() const {
	return TYPENAME;
}

void Elem::internalize(Reader &reader) {
	Store::internalize(reader);
	if (reader.isCancelled()) return;
	reader.readVersion(0, 0);
}

} // namespace odc
