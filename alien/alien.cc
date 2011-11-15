#include "alien/module.ih"

namespace odc {

Alien::Alien(INTEGER id, const TypePath &path): Store(id), d_path(path), d_comps() {}

std::vector<AlienComponent*> & Alien::getComponents() {
	return d_comps;
}

std::string Alien::toString() {
	std::string sofar = d_path.toString() + "{ ";
	for (int i = 0; i < d_comps.size(); ++i) {
		sofar += d_comps[i]->toString() + " ";
	}
	return sofar + "}";
}

void Alien::accept(Visitor &visitor) const {
	for (int i = 0; i < d_comps.size(); ++i) {
		d_comps[i]->accept(visitor);
	}
}

} // namespace odc
