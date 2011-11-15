#include "reader/reader.ih"

namespace odc {

void Reader::readAlien(Alien *alien, std::streampos down, std::streampos end) {
	std::streampos next = down != 0 ? down : end;
	while (d_rider.tellg() < end) {
		if (d_rider.tellg() < next) { // for some reason, this means its a piece (unstructured)
			size_t len = next - d_rider.tellg();
			char *buf = new char[len];
			d_rider.read(buf, len);
			AlienComponent *comp = new AlienPiece(buf, len);
			alien->getComponents().push_back(comp);
		} else { // that means we've got a store
			d_rider.seekg(next);
			AlienComponent *comp = new AlienPart(readStore());
			alien->getComponents().push_back(comp);
			next = d_state->next > 0 ? d_state->next : end;
		}
	}
}

} // namespace odc
