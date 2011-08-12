#include <reader.h>
#include <alien.h>

#include <string>
#include <assert.h>

namespace odc {

Reader::Reader(std::istream &rider): d_rider(rider), d_cancelled(false), d_readAlien(false), d_typeList(),
	d_state(new ReaderState()) {}

SHORTCHAR Reader::readSChar() {
	SHORTCHAR out;
	d_rider.read(&out, 1);
	return out;
}

void Reader::readSChar(SHORTCHAR *buf, size_t len) {
	d_rider.read(buf, len);
}

CHAR Reader::readLChar() {
	CHAR buf;
	char *bufPtr = (char *)&buf;
	d_rider.read(bufPtr, 2);
	if (isLittleEndian()) {
		return buf;
	} else {
		CHAR out;
		char *outPtr = (char *)&out;
		outPtr[0] = bufPtr[1]; outPtr[1] = bufPtr[0];
		return out;
	}
}

void Reader::readLChar(CHAR *buf, size_t len) {
	char *bufPtr = (char *)buf;
	int len2 = len * 2;
	d_rider.read(bufPtr, len2);
	if (isBigEndian()) {
		char tmp;
		for (int i = 0; i < len2; i += 2) {
			tmp = bufPtr[i];
			bufPtr[i] = bufPtr[i + 1];
			bufPtr[i + 1] = tmp;
		}
	}
}

BYTE Reader::readByte() {
	BYTE out;
	d_rider.read((char*)&out, 1);
	return out;
}

INTEGER Reader::readInt() {
	INTEGER buf;
	char *bufPtr = (char*)&buf;
	d_rider.read(bufPtr, 4);
	if (isLittleEndian()) {
		return buf;
	} else {
		INTEGER out;
		char *outPtr = (char *)&out;
		outPtr[0] = bufPtr[3]; outPtr[1] = bufPtr[2]; outPtr[2] = bufPtr[1]; outPtr[3] = bufPtr[0];
		return out;
	}
}

void Reader::readSString(SHORTCHAR *out) {
	while (*out = readSChar()) {
		++out;
	}
}

void Reader::turnIntoAlien(int cause) {
	assert(cause > 0);
	d_cancelled = true;
	d_cause = cause;
	d_readAlien = true;
}

bool Reader::isCancelled() {
	return d_cancelled;
}

INTEGER Reader::readVersion(INTEGER min, INTEGER max) {
	INTEGER version = readByte();
	if (version < min || version > max) {
		turnIntoAlien(ALIENVERSION);
	}
	return version;
}

Store* Reader::readStore() {
	SHORTCHAR kind = readSChar();
	if (kind == Store::NIL) {
		std::cout << "NIL STORE" << std::endl;
		return readNilStore();
	} else if (kind == Store::LINK) {
		std::cout << "LINK STORE" << std::endl;
		return readLinkStore();
	} else if (kind == Store::NEWLINK) {
		std::cout << "NEWLINK STORE" << std::endl;
		return readNewLinkStore();
	} else if (kind == Store::STORE) {
		std::cout << "STORE STORE" << std::endl;
		return readStoreOrElemStore(false);
	} else if (kind == Store::ELEM) {
		std::cout << "ELEM STORE" << std::endl;
		return readStoreOrElemStore(true);
	} else {
		std::cout << std::hex << (unsigned int)kind << std::endl;
		throw 20;
	}
}
//	PROCEDURE (VAR rd: Reader) ReadStore* (OUT x: Store), NEW;
//		VAR a: Alien; t: Kernel.Type;
//			len, pos, pos1, id, comment, next, down, downPos, nextTypeId, nextElemId, nextStoreId: INTEGER;
//			kind: SHORTCHAR; path: TypePath; type: TypeName;
//			save: ReaderState;
Store *Reader::readNilStore() {
	INTEGER comment = readInt();
	std::streamoff next = readInt();
	d_state->end = d_rider.tellg();
	if (next > 0 || (next == 0 && comment % 2 == 1)) {
		d_state->next = d_state->end + next;
	} else {
		d_state->next = 0;
	}
	return 0;
}
//		IF kind = nil THEN
//			rd.ReadInt(comment); rd.ReadInt(next);
//			rd.st.end := rd.Pos();
//			IF (next > 0) OR ((next = 0) & ODD(comment)) THEN rd.st.next := rd.st.end + next ELSE rd.st.next := 0 END;
//			x := NIL
Store *Reader::readLinkStore() {
	return 0;
}
//		ELSIF kind = link THEN
//			rd.ReadInt(id); rd.ReadInt(comment); rd.ReadInt(next);
//			rd.st.end := rd.Pos();
//			IF (next > 0) OR ((next = 0) & ODD(comment)) THEN rd.st.next := rd.st.end + next ELSE rd.st.next := 0 END;
//			x := ThisStore(rd.eDict, id)
Store *Reader::readNewLinkStore() {
	return 0;
}
//		ELSIF kind = newlink THEN
//			rd.ReadInt(id); rd.ReadInt(comment); rd.ReadInt(next);
//			rd.st.end := rd.Pos();
//			IF (next > 0) OR ((next = 0) & ODD(comment)) THEN rd.st.next := rd.st.end + next ELSE rd.st.next := 0 END;
//			x := ThisStore(rd.sDict, id)

Store *Reader::readStoreOrElemStore(bool isElem) {
	INTEGER id = isElem ? d_elemList.size() : d_storeList.size();
	TypePath path = readPath();
	std::cout << path.toString() << std::endl;
	const std::string &type = path[0];
	INTEGER comment = readInt();
	std::streampos pos1 = d_rider.tellg();
	std::streamoff next = readInt();
	std::streamoff down = readInt();
	std::streamoff len = readInt();
	std::streampos pos = d_rider.tellg();
	if (next > 0) {
		d_state->next = pos1 + next + (std::streamoff)4;
	} else {
		d_state->next = 0;
	}
	int downPos = 0;
	if (down > 0) {
		downPos = pos1 + down + (std::streamoff)8;
	}
	d_state->end = pos + len;
	d_cause = 0;
	assert(len >= 0);
	if (next != 0) {
		assert(d_state->next > pos1);
		if (down != 0) {
			assert(downPos < d_state->next);
		}
	}
	if (down > 0) {
		assert(downPos > pos1);
		assert(downPos < d_state->end);
	}

	const TypeProxyBase *t = TypeRegister::getInstance().get(type); // FIXME type lookup here
	Store *x = 0;
	if (t != 0) {
		x = t->newInstance(id);
	} else {
		d_cause = TYPENOTFOUND;
	}

	if (x != 0) { // IF READING SUCCEEDS, INSERT MORE CHECKS HERE
		if (true) { // samePath(x, path)
			ReaderState *save = d_state;
			d_state = new ReaderState();
			x->internalize(*this);
			delete d_state;
			d_state = save;

			if (d_cause != 0) {
				x = 0;
			}
			assert(d_rider.tellg() == d_state->end);
			assert(!d_rider.eof());
		} else {
//					rd.cause := inconsistentType; AlienTypeReport(rd.cause, type);
			x = 0;
		}
	}

	if (x != 0) {
		if (d_store == 0) {
			d_store = x;
		} else {
			// join(d_store, x)
			std::cout << "Man, should have written join(.,.)" << std::endl;
		}
		if (isElem) {
			d_elemList.push_back(x);
		} else {
			d_storeList.push_back(x);
		}
	} else {
		d_rider.seekg(pos); // x->internalize() could have left us anywhere
		assert(d_cause != 0);
		Alien *alien = new Alien(id, path); //, d_cause); //,file
		if (d_store == 0) {
			d_store = alien;
		} else {
			// join(d_store, alien)
			std::cout << "Man, should have written join(.,.)" << std::endl;
		}
		if (isElem) {
			d_elemList.push_back(alien);
		} else {
			d_storeList.push_back(alien);
		}
		ReaderState *save = d_state;
		d_state = new ReaderState();
		internalizeAlien(alien, downPos, save->end);
		delete d_state;
		d_state = save;
		assert(d_rider.tellg() == d_state->end);

		// we've just read the alien, so reset the state
		d_cause = 0;
		d_cancelled = false;
		d_readAlien = true;
		return alien;
	}

	return x;
}


void Reader::internalizeAlien(Alien *alien, std::streampos down, std::streampos end) {
	std::streampos next = down != 0 ? down : end;
	while (d_rider.tellg() < end) {
		if (d_rider.tellg() < next) { // for some reason, this means its a piece (unstructured)
			std::cout << "Alien Piece" << std::endl;
			size_t len = next - d_rider.tellg();
			char *buf = new char[len];
			d_rider.read(buf, len);
			AlienComponent *comp = new AlienPiece(buf, len);
			alien->getComponents().push_back(comp);
		} else { // that means we've got a store
			std::cout << "Alien Store" << std::endl;
			d_rider.seekg(next);
			AlienComponent *comp = new AlienPart(readStore());
			alien->getComponents().push_back(comp);
			next = d_state->next > 0 ? d_state->next : end;
		}
	}
}

std::string &Reader::fixTypeName(std::string &name) {
	size_t pos = name.size() - 4;
	if (pos > 0 && name.substr(pos, 4).compare("Desc") == 0) {
		return name.replace(pos, 4, "^");
	}
	return name;
}

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

void Reader::addPathComponent(bool first, const std::string &typeName) {
	int next = d_typeList.size();
	int curr = next - 1;
	if (!first) {
		d_typeList[curr]->baseId = next;
	}
	d_typeList.push_back(new TypeEntry(typeName));
}

} // namespace odc
