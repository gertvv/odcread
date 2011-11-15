#include "reader/reader.ih"

namespace odc {

TypePath Reader::readPath() {
	TypePath path;
	SHORTCHAR kind = readSChar();
	SHORTCHAR *buf = new SHORTCHAR[64]; // TypeName has a maximum of length 64 (including terminator).
	int i;
	for (i = 0; kind == Store::NEWEXT; ++i) {
		readSString(buf);
		std::string name(buf);
		path.push_back(fixTypeName(name));
		addPathComponent(i == 0, path[i]);
//			IF path[i] # elemTName THEN INC(i) END;
		kind = readSChar();
	}

	if (kind == Store::NEWBASE) {
		readSString(buf);
		std::string name(buf);
		path.push_back(fixTypeName(name));
		addPathComponent(i == 0, path[i]);
		++i;
	} else if (kind == Store::OLDTYPE) {
		int id = readInt();
		if (i > 0) {
			d_typeList[d_typeList.size() - 1]->baseId = id;
		}
		while (id != -1) {
			path.push_back(d_typeList[id]->name);
			id = d_typeList[id]->baseId;
//				IF path[i] # elemTName THEN INC(i) END
			++i;
		}
	} else {
		throw 100;
	}
	return path;
}

} // namespace odc
