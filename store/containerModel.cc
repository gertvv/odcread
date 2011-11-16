#include "store/module.ih"

namespace odc {

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
