#include "reader/reader.ih"

namespace odc {

INTEGER Reader::readVersion(INTEGER min, INTEGER max) {
	INTEGER version = readByte();
	if (version < min || version > max) {
		turnIntoAlien(ALIENVERSION);
	}
	return version;
}

} // namespace odc
