#include <store.h>
#include <reader.h>
#include <visitor.h>

#include <iostream>


namespace odc {

template<class T, class A> T join(const A & begin, const A & end, const T &sep) {
	T result;

	if (begin != end) {
		A it = begin;
		result.append(*it);
		for (++it; it != end; ++it) {
			result.append(sep).append(*it);
		}
	}

	return result;
}

std::string TypePath::toString() const {
	return join(begin(), end(), std::string("->"));
}

const std::string Store::TYPENAME("Stores.Store^");
const TypeProxy<Store> Store::PROXY;

Store::Store(INTEGER id): d_id(id) {}

INTEGER Store::getId() {
	return d_id;
}

const std::string &Store::getType() {
	return Store::TYPENAME;
}

const std::string *Store::getSuper() {
	return 0;
}

const std::string &Store::getTypeName() const {
	return Store::getType();
}

void Store::getTypePath(TypePath *out) const {
	return calcTypePath(out, getTypeName());
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
const TypeProxy<Elem> Elem::PROXY;

Elem::Elem(INTEGER id) : Store(id) {}

const std::string &Elem::getType() {
	return TYPENAME;
}

const std::string *Elem::getSuper() {
	return &Store::getType();
}

const std::string &Elem::getTypeName() const {
	return getType();
}

void Elem::internalize(Reader &reader) {
	Store::internalize(reader);
	if (reader.isCancelled()) return;
	reader.readVersion(0, 0);
}

const std::string Model::TYPENAME("Models.Model^");
const TypeProxy<Model> Model::PROXY;

Model::Model(INTEGER id) : Elem(id) {}

const std::string &Model::getType() {
	return TYPENAME;
}

const std::string *Model::getSuper() {
	return &Elem::getType();
}

const std::string &Model::getTypeName() const {
	return getType();
}

void Model::internalize(Reader &reader) {
	Elem::internalize(reader);
	if (reader.isCancelled()) return;
	reader.readVersion(0, 0);
}

const std::string ContainerModel::TYPENAME("Containers.Model^");
const TypeProxy<ContainerModel> ContainerModel::PROXY;

ContainerModel::ContainerModel(INTEGER id) : Model(id) {}

const std::string &ContainerModel::getType() {
	return TYPENAME;
}

const std::string *ContainerModel::getSuper() {
	return &Model::getType();
}

const std::string &ContainerModel::getTypeName() const {
	return getType();
}

void ContainerModel::internalize(Reader &reader) {
	Model::internalize(reader);
	if (reader.isCancelled()) return;
	reader.readVersion(0, 0);
}

} // namespace odc
