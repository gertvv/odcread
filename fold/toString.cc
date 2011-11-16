#include "fold/module.ih"

namespace odc {

std::string Fold::toString() {
	if (d_hidden == 0) {
		return std::string("Fold(right)");
	}
	return std::string("Fold(left) \"") + std::string(d_label) + std::string("\" { ") + d_hidden->toString() + std::string("  }");
}

} // namespace odc
