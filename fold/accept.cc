#include "fold/module.ih"

namespace odc {

void Fold::accept(Visitor &visitor) const {
	if (d_hidden == 0) { // right part
		visitor.foldRight();
	} else { // left part
		visitor.foldLeft(d_collapsed);
		d_hidden->accept(visitor);
	}
}

} // namespace odc
