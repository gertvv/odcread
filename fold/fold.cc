#include "fold/module.ih"

namespace odc {

const std::string Fold::TYPENAME("StdFolds.Fold^");
const TypeProxy<Fold, View> Fold::PROXY;

Fold::Fold(INTEGER id) : View(id) {}

Fold::~Fold() {
	if (d_hidden != 0) {
		delete d_hidden;
	}
	delete d_label;
}

const std::string &Fold::getTypeName() const {
	return TYPENAME;
}

} // namespace odc
