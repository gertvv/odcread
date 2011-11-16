#include "fold/module.ih"

namespace odc {

const std::string View::TYPENAME("Views.View^");
const TypeProxy<View, Store> View::PROXY;

View::View(INTEGER id) : Store(id) {}

const std::string &View::getTypeName() const {
	return TYPENAME;
}

void View::internalize(Reader &reader) {
	Store::internalize(reader);
	if (reader.isCancelled()) return;
	reader.readVersion(0, 0);
}

} // namespace odc
