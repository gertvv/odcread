#include "store/module.ih"

namespace odc {

const std::string Model::TYPENAME("Models.Model^");
const TypeProxy<Model, Elem> Model::PROXY;

Model::Model(INTEGER id) : Elem(id) {}

const std::string &Model::getTypeName() const {
	return TYPENAME;
}

void Model::internalize(Reader &reader) {
	Elem::internalize(reader);
	if (reader.isCancelled()) return;
	reader.readVersion(0, 0);
}

} // namespace odc
