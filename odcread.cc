#include <iostream>
#include <fstream>
#include <stdint.h>

namespace odc {
	/**
	 * INTEGER: 4 bytes (-2147483648..2147483647)
	 */
	typedef int32_t INTEGER;

	bool isBigEndian() { // http://stackoverflow.com/questions/1001307/detecting-endianness-programmatically-in-a-c-program
		union {
			uint32_t i;
			uint8_t c[4];
		} test = {0x01020304};
		return test.c[0] == 1; 
	}
	bool isLittleEndian() {
		return !isBigEndian();
	}

	class Domain {
	};

	class Reader;

	/**
	 * TYPE Store
	 * ABSTRACT
	 * Storable extensible data types like Views.View or TextModels.Text are derived from Store.
	 * Stores are typically allocated by suitable directories, e.g., Views.Directory or TextModels.Directory.
	 * Stores are used as base types for all objects that must be both extensible and persistent.
	 */
	class Store {
	public: 
		/**
		 * PROCEDURE (s: Store) Domain (): Domain
		 * NEW
		 * A store may be associated with a domain. This is done by the procedure InitDomain, which assigns a domain to the store.
		 * Domain may be called by arbitrary clients.
		 */
		Domain* getDomain() {
			return 0;
		}

		/**
		 * PROCEDURE (s: Store) CopyFrom- (source: Store)
		 * NEW, EMPTY
		 * Copy the contents of source to s. Copying is a deep copy.
		 *
		 * Pre
		 * source # NIL	guaranteed
		 * TYP(source) = TYP(s)	guaranteed
		 * s.Domain() = NIL	guaranteed
		 * s is not yet initialized	guaranteed
		 */
		// FIXME
		/**
		 * PROCEDURE (s: Store) Internalize- (VAR rd: Reader)
		 * NEW, EMPTY
		 * (For backward compatibility, this method is actually still EXTENSIBLE. This may change in the future.)
		 * Reads the contents of s from reader rd. Internalize must read the same (amount of) data as is written by the corresponding Externalize procedure.
		 * Internalize is called locally.
		 * Internalize is extended by various persistent object types, e.g., models, views, and controllers.
		 *
		 * Pre
		 * source.Domain() = NIL	guaranteed
		 * source is not yet initialized	guaranteed
		 */
		 void internalize(Reader &reader) {
//	PROCEDURE (s: Store) Internalize- (VAR rd: Reader), NEW, EXTENSIBLE;
//		VAR thisVersion: INTEGER;
//	BEGIN
//		rd.ReadVersion(minVersion, maxStoreVersion, thisVersion);
//		IF ~rd.cancelled & s.isElem THEN
//			rd.ReadVersion(minVersion, maxStoreVersion, thisVersion)
//			(* works since maxStoreVersion = maxElemVersion = 0 in pre-1.3 *)
//		END	
//	END Internalize;	
		}

		/**
		 * PROCEDURE (s: Store) Externalize- (VAR wr: Writer)
		 * NEW, EMPTY
		 * (For backward compatibility, this method is actually still EXTENSIBLE. This may change in the future.)
		 * Write the contents of s to writer wr. Externalize must write the same (amount of) data as is read by the corresponding Internalize procedure.
		 * Externalize ist called locally.
		 * Externalize is extended by various persistent object types, e.g., models, views, and controllers.
		 */
		// FIXME

		/**
		 * PROCEDURE (s: Store) ExternalizeAs- (VAR s1: Store)
		 * NEW, EMPTY
		 * Before a store's Externalize procedure is called, its ExternalizeAs procedure is called, which gives the store the opportunity to denote another store that should be externalized in its place (a "proxy"). It is also possible to set s1 to NIL, which means that the store should not be externalized at all. This is used e.g. for compiler error markers, which are never stored.
		 * ExternalizeAs ist called locally.
		 * 
		 * Pre
		 * s1 = s	guaranteed
		 */
		// FIXME
	};

	/**
	 * TYPE Reader
	 * Reader for Component Pascal values like integers, reals, or sets. A reader contains a Files.Reader, to which it forwards most operations.
	 * Readers are used in the Store.Internalize procedure.
	 * Readers are not extensible.
	 */ 
	class Reader {
	private:
		/*
		 * rider-: Files.Reader
		 * The file rider which links a Reader to a file.
		 */ 
		std::istream &d_rider;

		/*
		 * cancelled-: BOOLEAN	valid during a Store.Internalize call
		 * Tells whether the currently executing Internalize has been called by ReadVersion or TurnIntoAlien.
		 */
		bool d_cancelled;

		/**
		 * readAlien-: BOOLEAN
		 * Tells whether any alien has been read since the last ConnectTo.
		 */
		bool d_readAlien;

		public:
		Reader(std::istream &rider): d_rider(rider), d_cancelled(false), d_readAlien(false) {}

		/**
		 * PROCEDURE (VAR rd: Reader) ConnectTo (f: Files.File)
		 * NEW
		 * Connect the reader to a file. All the following operations require connected readers, i.e., rd.rider # NIL. This precondition is not checked explicitly, however. After connecting, the reader's position is at the beginning of the file. If the same reader should be reused on another file, it must first be closed, by connecting it to NIL.
		 * ConnectTo is used internally.
		 * 
		 * Pre
		 * 20	(f = NIL) OR (rd.rider = NIL)
		 * 
		 * Post
		 * f = NIL
		 * 	rd.rider = NIL
		 * f # NIL
		 * 	(rd.rider # NIL) & (rd.rider.Base() = f)
		 * 	rd.Pos() = 0
		 */
		// FIXME

		/**
		 * PROCEDURE (VAR rd: Reader) Pos (): INTEGER
		 * NEW
		 * Returns the reader's current position.
		 * 
		 * Post
		 * 0 <= result <= rd.rider.Base().Length()
		 */
		// FIXME

		/**
		 * PROCEDURE (VAR rd: Reader) SetPos (pos: INTEGER)
		 * NEW
		 * Sets the reader's current position to pos.
		 * 
		 * Pre
		 * 20	pos >= 0
		 * 21	pos <= rd.rider.Base().Length()
		 * 
		 * Post
		 * rd.Pos() = pos
		 * ~rd.rider.eof
		 */
		// FIXME

		/**
		 * PROCEDURE (VAR rd: Reader) ReadBool (OUT x: BOOLEAN)
		 * NEW
		 * Reads a Boolean value.
		 * 
		 * PROCEDURE (VAR rd: Reader) ReadSChar (OUT x: SHORTCHAR)
		 * NEW
		 * Reads a short character (00X..0FFX).
		 * 
		 * PROCEDURE (VAR rd: Reader) ReadXChar (OUT x: CHAR)
		 * NEW
		 * Same as ReadSChar, but has a CHAR-type parameter.
		 * This procedure is provided to simplify migration from Release 1.2 to 1.3.
		 * 
		 * PROCEDURE (VAR rd: Reader) ReadChar (OUT x: CHAR)
		 * NEW
		 * Reads a character (0000X..0FFFFX).
		 * 
		 * PROCEDURE (VAR rd: Reader) ReadByte (OUT x: BYTE)
		 * NEW
		 * Reads a very short integer (-128..127).
		 * 
		 * PROCEDURE (VAR rd: Reader) ReadSInt (OUT x: SHORTINT)
		 * NEW
		 * Reads a short integer (-32768..32767).
		 * 
		 * PROCEDURE (VAR rd: Reader) ReadXInt (OUT x: INTEGER)
		 * NEW
		 * Same as ReadSInt, but has an INTEGER-type parameter.
		 * This procedure is provided to simplify migration from Release 1.2 to 1.3.
		 */ 

		/**
		 * PROCEDURE (VAR rd: Reader) ReadInt (OUT x: INTEGER)
		 * NEW
		 * Reads an integer (-2147483648..2147483647).
		 */
		INTEGER readInt() {
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

		/* 
		 * PROCEDURE (VAR rd: Reader) ReadLong (OUT x: LONGINT)
		 * NEW
		 * Reads a long integer (-9223372036854775808..9223372036854775807).
		 * 
		 * PROCEDURE (VAR rd: Reader) ReadSReal (OUT x: SHORTREAL)
		 * NEW
		 * Reads a short real (32-bit IEEE number).
		 * 
		 * PROCEDURE (VAR rd: Reader) ReadXReal (OUT x: REAL)
		 * NEW
		 * Same as ReadSReal, but has a REAL-type parameter.
		 * This procedure is provided to simplify migration from Release 1.2 to 1.3.
		 * 
		 * PROCEDURE (VAR rd: Reader) ReadReal (OUT x: REAL)
		 * NEW
		 * Reads a real (64-bit IEEE number).
		 * 
		 * PROCEDURE (VAR rd: Reader) ReadSet (OUT x: SET)
		 * NEW
		 * Reads a set (32 elements).
		 * 
		 * PROCEDURE (VAR rd: Reader) ReadSString (OUT x: ARRAY OF SHORTCHAR)
		 * NEW
		 * Reads a 0X-terminated short string.
		 * 
		 * Pre
		 * invalid index	 LEN(x) > Length(string)
		 * 
		 * PROCEDURE (VAR rd: Reader) ReadXString (OUT x: ARRAY OF CHAR)
		 * NEW
		 * Same as ReadSString, but has a string-type parameter.
		 * This procedure is provided to simplify migration from Release 1.2 to 1.3.
		 * 
		 * PROCEDURE (VAR rd: Reader) ReadString (OUT x: ARRAY OF CHAR)
		 * NEW
		 * Reads a 0X-terminated string.
		 * 
		 * Pre
		 * invalid index	 LEN(x) > Length(string)
		 * 
		 * PROCEDURE (VAR rd: Reader) ReadStore (OUT x: Store)
		 * NEW
		 * Reads a store's type, allocates it, and then reads its contents, by calling the store's Internalize procedure. x may also be NIL, or an alien if the store's module cannot be loaded, or if internalization has been cancelled by the Internalize procedure.
		 * If the store has already been read in, a pointer to the same store is returned instead of allocating a new one. This means that arbitrary graphs that have been written with WriteStore are reconstructed correctly, including alias pointers to the same store, cycles, etc.
		 * If the file on which the reader operates does not contain correct input, then an assertion trap will be caused (traps 101 to trap 106).
		 * 
		 * Pre
		 * 20	the reader is at the start position of a new store
		 * 
		 * Post
		 * empty store on file
		 * 	x = NIL
		 * non-empty store on file
		 * 	x # NIL
		 * 		x IS Alien
		 * 			x.cause # 0
		 * 			x.type # ""
		 * 			x.file # NIL
		 * 			x.pos >= 0	beginning of store's data
		 * 			x.len >= 0	length of store's data
		 * 			alien store contents are on x.file in the range [x.pos .. x.pos + x.len[.
		 * 			These data include only the store's contents, not its prefix
		 * 		~(x IS Alien)
		 * 			x was read successfully
		 */
		Store *readStore() {
			return new Store();
		}
//	PROCEDURE (VAR rd: Reader) ReadStore* (OUT x: Store), NEW;
//		VAR a: Alien; t: Kernel.Type;
//			len, pos, pos1, id, comment, next, down, downPos, nextTypeId, nextElemId, nextStoreId: INTEGER;
//			kind: SHORTCHAR; path: TypePath; type: TypeName;
//			save: ReaderState;
//	BEGIN
//		rd.ReadSChar(kind);
//		IF kind = nil THEN
//			rd.ReadInt(comment); rd.ReadInt(next);
//			rd.st.end := rd.Pos();
//			IF (next > 0) OR ((next = 0) & ODD(comment)) THEN rd.st.next := rd.st.end + next ELSE rd.st.next := 0 END;
//			x := NIL
//		ELSIF kind = link THEN
//			rd.ReadInt(id); rd.ReadInt(comment); rd.ReadInt(next);
//			rd.st.end := rd.Pos();
//			IF (next > 0) OR ((next = 0) & ODD(comment)) THEN rd.st.next := rd.st.end + next ELSE rd.st.next := 0 END;
//			x := ThisStore(rd.eDict, id)
//		ELSIF kind = newlink THEN
//			rd.ReadInt(id); rd.ReadInt(comment); rd.ReadInt(next);
//			rd.st.end := rd.Pos();
//			IF (next > 0) OR ((next = 0) & ODD(comment)) THEN rd.st.next := rd.st.end + next ELSE rd.st.next := 0 END;
//			x := ThisStore(rd.sDict, id)
//		ELSIF (kind = store) OR (kind = elem) THEN
//			IF kind = elem THEN
//				id := rd.nextElemId; INC(rd.nextElemId)
//			ELSE
//				id := rd.nextStoreId; INC(rd.nextStoreId)
//			END;
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
//		ELSE
//			pos := rd.Pos();
//			HALT(20)
//		END
//	END ReadStore;
		/**
		 * PROCEDURE (VAR rd: Reader) ReadVersion (min, max: INTEGER; OUT version: INTEGER)
		 * NEW
		 * Read a version byte and return it in version. If version is not in the specified range [min .. max], the store currently being read is turned into an alien, with cause = alienVersion.
		 * 
		 * Pre
		 * 20	0 <= min <= max
		 * 
		 * Post
		 * min <= version <= max
		 * 	legal version
		 * (version < min) OR (version > max)
		 * 	illegal version
		 * 	rd.cause = alienVersion
		 * 	rd.cancelled
		 * 	rd.readAlien
		 * 
		 * PROCEDURE (VAR rd: Reader) TurnIntoAlien (cause: INTEGER)
		 * NEW
		 * A store which is currently being internalized can turn itself into an alien, e.g., if it has read a component store which is an alien.
		 * 
		 * Pre
		 * 20	cause > 0
		 */
	};


	Store* importDocument(std::istream &is) {
		const INTEGER docTag = 0x6F4F4443;
		const INTEGER docVersion = 0;
		Reader r(is);
		INTEGER tag = r.readInt();
		if (tag == docTag) {
			INTEGER version = r.readInt();
			if (version != docVersion) {
				throw 100;
			}
			Store *s = r.readStore();
			return s;
		}
		return 0;
	}
}

int main(int argc, char *argv[]) {
	std::ifstream in(argv[1], std::ios::in | std::ios::binary);
	odc::Store* s = odc::importDocument(in);
	std::cout << s << std::endl;
	return 0;
}
