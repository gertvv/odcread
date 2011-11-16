#include "store/module.ih"

namespace odc {

const std::string Store::TYPENAME("Stores.Store^");
const TopTypeProxy<Store> Store::PROXY;

Store::Store(INTEGER id): d_id(id) {}

INTEGER Store::getId() {
	return d_id;
}

const std::string &Store::getTypeName() const {
	return Store::TYPENAME;
}

TypePath Store::getTypePath() const {
	TypePath out;
	calcTypePath(&out, getTypeName());
	return out;
}

void Store::calcTypePath(TypePath *path, const std::string &name) const {
	const std::string *super = TypeRegister::getInstance().get(name)->getSuper();
	if (super != 0) {
		calcTypePath(path, *super);
	}
	path->push_back(name);
}

void Store::internalize(Reader &reader) {
	reader.readVersion(0, 0);
}

std::string Store::toString() {
	return getTypeName(); 
}

void Store::accept(Visitor &visitor) const {}

} // namespace odc
