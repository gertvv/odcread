#include "store/store.ih"

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

const std::string ContainerModel::TYPENAME("Containers.Model^");
const TypeProxy<ContainerModel, Model> ContainerModel::PROXY;

ContainerModel::ContainerModel(INTEGER id) : Model(id) {}

const std::string &ContainerModel::getTypeName() const {
	return TYPENAME;
}

void ContainerModel::internalize(Reader &reader) {
	Model::internalize(reader);
	if (reader.isCancelled()) return;
	reader.readVersion(0, 0);
}

} // namespace odc
