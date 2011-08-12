#include <alien.h>

namespace odc {

AlienPiece::AlienPiece(const char * const _data, const size_t _len): data(_data), len(_len) {}

std::string AlienPiece::toString() {
	return std::string("AlienPiece");
}

AlienPart::AlienPart(Store * const _store): store(_store) {}

std::string AlienPart::toString() {
	if (store != 0)
		return store->toString();
	else
		return "NULL";
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

}
