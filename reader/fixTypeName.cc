#include "reader/reader.ih"

namespace odc {

std::string &Reader::fixTypeName(std::string &name) {
	size_t pos = name.size() - 4;
	if (pos > 0 && name.substr(pos, 4).compare("Desc") == 0) {
		return name.replace(pos, 4, "^");
	}
	return name;
}

} // namespace odc
