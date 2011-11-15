#include "alien/alien.ih"

namespace odc {

AlienPiece::AlienPiece(char const * const _data, const size_t _len): data(_data), len(_len) {}

std::string AlienPiece::toString() {
	return std::string("AlienPiece");
}

void AlienPiece::accept(Visitor &visitor) const {
}

} // namespace odc
