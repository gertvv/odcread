#include <alien.h>

namespace odc {

AlienPiece::AlienPiece(const char * const _data, const size_t _len): data(_data), len(_len) {}

std::string AlienPiece::toString() {
	return std::string("AlienPiece");
}

std::string AlienPiece::toPlainText() {
	return std::string();
}

AlienPart::AlienPart(Store * const store): d_store(store) {}

std::string AlienPart::toString() {
	if (d_store != 0)
		return d_store->toString();
	else
		return "NULL";
}

std::string AlienPart::toPlainText() {
	if (d_store != 0)
		return d_store->toPlainText();
	else
		return std::string();
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

std::string Alien::toPlainText() {
	std::string sofar = std::string();
	for (int i = 0; i < d_comps.size(); ++i) {
		sofar += d_comps[i]->toPlainText();
	}
	return sofar;
}

}
