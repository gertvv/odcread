#include "reader/module.ih"

namespace odc {

void Reader::addPathComponent(bool first, const std::string &typeName) {
	int next = d_typeList.size();
	int curr = next - 1;
	if (!first) {
		d_typeList[curr]->baseId = next;
	}
	d_typeList.push_back(new TypeEntry(typeName));
}

} // namespace odc
