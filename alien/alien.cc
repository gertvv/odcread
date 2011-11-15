#include <alien.h>

namespace odc {

AlienPiece::AlienPiece(const char * const _data, const size_t _len): data(_data), len(_len) {}

std::string AlienPiece::toString() {
	return std::string("AlienPiece");
}

void AlienPiece::accept(Visitor &visitor) const {
}

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

}
