#include "alien/alien.ih"

namespace odc {

AlienPart::AlienPart(Store * const store): d_store(store) {}

std::string AlienPart::toString() {
	if (d_store != 0)
		return d_store->toString();
	else
		return "NULL";
}

void AlienPart::accept(Visitor &visitor) const {
	if (d_store != 0) {
		d_store->accept(visitor);
	}
}

} // namespace odc
