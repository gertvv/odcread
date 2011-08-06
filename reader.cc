#include <reader.h>

namespace odc {

Reader::Reader(std::istream &rider): d_rider(rider), d_cancelled(false), d_readAlien(false) {}

SHORTCHAR Reader::readSChar() {
	SHORTCHAR out;
	d_rider.read(&out, 1);
	return out;
}

INTEGER Reader::readInt() {
	char *buf = new char[4];
	d_rider.read(buf, 4);
	if (isLittleEndian()) {
		return *(INTEGER *)buf;
	} else {
		char *out = new char[4];
		out[0] = buf[3]; out[1] = buf[2]; out[2] = buf[1]; out[3] = buf[0];
		return *(INTEGER *)out;
	}
}

Store* Reader::readStore() {
	SHORTCHAR kind = readSChar();
	if (kind == Store::NIL) {
		return readNilStore();
	} else if (kind == Store::LINK) {
		return readLinkStore();
	} else if (kind == Store::NEWLINK) {
		return readNewLinkStore();
	} else if (kind == Store::STORE) {
		return readStoreOrElemStore(false);
	} else if (kind == Store::ELEM) {
		return readStoreOrElemStore(true);
	} else {
		throw 20;
	}
}
//	PROCEDURE (VAR rd: Reader) ReadStore* (OUT x: Store), NEW;
//		VAR a: Alien; t: Kernel.Type;
//			len, pos, pos1, id, comment, next, down, downPos, nextTypeId, nextElemId, nextStoreId: INTEGER;
//			kind: SHORTCHAR; path: TypePath; type: TypeName;
//			save: ReaderState;
Store *Reader::readNilStore() {
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
	INTEGER id = isElem ? d_nextElemId++ : d_nextStoreId++;
	CHAR** path = newTypePath();
	readPath(path);
	CHAR* type = path[0];
	std::cout << type << std::endl;
	INTEGER comment = readInt();
	return 0;
}
//			ReadPath(rd, path); type := path[0];
//			nextTypeId := rd.nextTypeId; nextElemId := rd.nextElemId; nextStoreId := rd.nextStoreId;
//			rd.ReadInt(comment);
//			pos1 := rd.Pos();
//			rd.ReadInt(next); rd.ReadInt(down); rd.ReadInt(len);
//			pos := rd.Pos();
//			IF next > 0 THEN rd.st.next := pos1 + next + 4 ELSE rd.st.next := 0 END;
//			IF down > 0 THEN downPos := pos1 + down + 8 ELSE downPos := 0 END;
//			rd.st.end := pos + len;
//			rd.cause := 0;
//			ASSERT(len >= 0, 101);
//			IF next # 0 THEN
//				ASSERT(rd.st.next > pos1, 102);
//				IF down # 0 THEN
//					ASSERT(downPos < rd.st.next, 103)
//				END
//			END;
//			IF down # 0 THEN
//				ASSERT(downPos > pos1, 104);
//				ASSERT(downPos < rd.st.end, 105)
//			END;
//			t := ThisType(type);
//			IF t # NIL THEN
//				x := NewStore(t); x.isElem := kind = elem
//			ELSE
//				rd.cause := thisTypeRes; AlienTypeReport(rd.cause, type);
//				x := NIL
//			END;
//			IF x # NIL THEN
//				IF SamePath(t, path) THEN
//					IF kind = elem THEN
//						x.id := id; AddStore(rd.eDict, rd.eHead, x)
//					ELSE
//						x.id := id; AddStore(rd.sDict, rd.sHead, x)
//					END;
//					save := rd.st; rd.cause := 0; rd.cancelled :=  FALSE;
//					x.Internalize(rd);
//					rd.st := save;
//					IF rd.cause # 0 THEN x := NIL
//					ELSIF (rd.Pos() # rd.st.end) OR rd.rider.eof THEN
//						rd.cause := inconsistentVersion; AlienReport(rd.cause);
//						x := NIL
//					END
//				ELSE
//					rd.cause := inconsistentType; AlienTypeReport(rd.cause, type);
//					x := NIL
//				END
//			END;
//			
//			IF x # NIL THEN
//				IF rd.noDomain THEN
//					rd.store := x;
//					rd.noDomain := FALSE
//				ELSE
//					Join(rd.store, x)
//				END
//			ELSE	(* x is an alien *)
//				rd.SetPos(pos);
//				ASSERT(rd.cause # 0, 107);
//				NEW(a); a.path := path; a.cause := rd.cause; a.file := rd.rider.Base();
//				IF rd.noDomain THEN
//					rd.store := a;
//					rd.noDomain := FALSE
//				ELSE
//					Join(rd.store, a)
//				END;
//				IF kind = elem THEN
//					a.id := id; AddStore(rd.eDict, rd.eHead, a)
//				ELSE
//					a.id := id; AddStore(rd.sDict, rd.sHead, a)
//				END;
//				save := rd.st;
//				rd.nextTypeId := nextTypeId; rd.nextElemId := nextElemId; rd.nextStoreId := nextStoreId;
//				InternalizeAlien(rd, a.comps, downPos, pos, len);
//				rd.st := save;
//				x := a;
//				ASSERT(rd.Pos() = rd.st.end, 108);
//				rd.cause := 0; rd.cancelled :=  FALSE; rd.readAlien := TRUE
//			END

void Reader::readPath(CHAR **path) {
	SHORTCHAR kind = readSChar();
	for (int i = 0; kind == Store::NEWEXT; ++i) {
		std::cout << "NEWEXT" << std::endl;
		//readXString(path[i]);
//			AddPathComp(rd); INC(rd.nextTypeId);
//			IF path[i] # elemTName THEN INC(i) END;
//			rd.ReadSChar(kind)
	}

	if (kind == Store::NEWBASE) {
		std::cout << "NEWBASE" << std::endl;
	} else if (kind == Store::OLDTYPE) {
		std::cout << "OLDTYPE" << std::endl;
	} else {
		throw 100;
	}

	// FIXME
	path[0][0] = 'H';
	path[0][1] = 'i';
	path[0][2] = 0;
}
//	PROCEDURE ReadPath (VAR rd: Reader; VAR path: TypePath);
//		VAR h: TypeDict; id, extId: INTEGER; i: INTEGER; kind: SHORTCHAR;
//
//		PROCEDURE AddPathComp (VAR rd: Reader);
//		BEGIN
//			IF h # NIL THEN AddBaseId(h, extId, rd.nextTypeId) END;
//			AddType(rd.tDict, rd.tHead, rd.nextTypeId, path[i]);
//			h := rd.tHead; extId := rd.nextTypeId
//		END AddPathComp;
//
//	BEGIN
//		h := NIL; i := 0; rd.ReadSChar(kind);
//		WHILE kind = newExt DO
//			rd.ReadXString(path[i]);
//			AddPathComp(rd); INC(rd.nextTypeId);
//			IF path[i] # elemTName THEN INC(i) END;
//			rd.ReadSChar(kind)
//		END;
//		IF kind = newBase THEN
//			rd.ReadXString(path[i]);
//			AddPathComp(rd); INC(rd.nextTypeId); INC(i)
//		ELSE
//			ASSERT(kind = oldType, 100);
//			rd.ReadInt(id);
//			IF h # NIL THEN AddBaseId(h, extId, id) END;
//			REPEAT
//				GetThisType(rd.tDict, id, path[i]); id := ThisBaseId(rd.tDict, id);
//				IF path[i] # elemTName THEN INC(i) END
//			UNTIL id = -1
//		END;
//		path[i] := ""
//	END ReadPath;

} // namespace odc
