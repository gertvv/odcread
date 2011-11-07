#ifndef _READER_H_
#define _READER_H_

#include <iostream>
#include <vector>

#include <oberon.h>
#include <store.h>
#include <alien.h>

namespace odc {

struct TypeEntry {
	const std::string name;
	INTEGER baseId;

	TypeEntry(const std::string &typeName) : name(typeName), baseId(-1) {}
};

/**
 * TYPE Reader
 * Reader for Component Pascal values like integers, reals, or sets. A reader contains a Files.Reader, to which it forwards most operations.
 * Readers are used in the Store.Internalize procedure.
 * Readers are not extensible.
 */ 
class Reader {
private:
	static const unsigned int TYPENOTFOUND = 1;
	static const unsigned int ALIENVERSION = 2;

	/*
	 * rider-: Files.Reader
	 * The file rider which links a Reader to a file.
	 */ 
	std::istream &d_rider;

	/*
	 * cancelled-: BOOLEAN	valid during a Store.Internalize call
	 * Tells whether the currently executing Internalize has been cancelled by ReadVersion or TurnIntoAlien.
	 */
	bool d_cancelled;

	/**
	 * readAlien-: BOOLEAN
	 * Tells whether any alien has been read since the last ConnectTo.
	 */
	bool d_readAlien;

	/**
	 * Cause of current read being alien.
	 */
	unsigned int d_cause;

	std::vector<TypeEntry*> d_typeList;

	std::vector<Store*> d_elemList; // FIXME: WTH, why are these different?
	std::vector<Store*> d_storeList;

	Store *d_store;

	struct ReaderState {
		/**
		 * Position of the next store in the current level
		 */
		std::streampos next;
		/**
		 * Position just after the last read store
		 */
		std::streampos end;
	};
	ReaderState *d_state;

	public:
	/**
	 * Construct a reader from the istream rider.
	 * @param rider An istream (binary mode).
	 */
	Reader(std::istream &rider);

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
	 */ 
	/**
	 * PROCEDURE (VAR rd: Reader) ReadSChar (OUT x: SHORTCHAR)
	 * NEW
	 * Reads a short character (00X..0FFX).
	 */ 
	SHORTCHAR readSChar();
	void readSChar(SHORTCHAR *buf, size_t len);
	/* PROCEDURE (VAR rd: Reader) ReadXChar (OUT x: CHAR)
	 * NEW
	 * Same as ReadSChar, but has a CHAR-type parameter.
	 * This procedure is provided to simplify migration from Release 1.2 to 1.3.
	 */
	/** 
	 * PROCEDURE (VAR rd: Reader) ReadChar (OUT x: CHAR)
	 * NEW
	 * Reads a character (0000X..0FFFFX).
	 */ 
	CHAR readLChar();
	void readLChar(CHAR *buf, size_t len);
	/**
	 * PROCEDURE (VAR rd: Reader) ReadByte (OUT x: BYTE)
	 * NEW
	 * Reads a very short integer (-128..127).
	 */ 
	BYTE readByte();
	/**
	 * PROCEDURE (VAR rd: Reader) ReadSInt (OUT x: SHORTINT)
	 * NEW
	 * Reads a short integer (-32768..32767).
	 */
	SHORTINT readSInt();
	/** 
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
	INTEGER readInt();
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
	 */
	/**
	 * PROCEDURE (VAR rd: Reader) ReadSString (OUT x: ARRAY OF SHORTCHAR)
	 * NEW
	 * Reads a 0X-terminated short string.
	 * 
	 * Pre
	 * invalid index	 LEN(x) > Length(string)
	 */
	void readSString(SHORTCHAR *out);
	/** 
	 * PROCEDURE (VAR rd: Reader) ReadXString (OUT x: ARRAY OF CHAR)
	 * NEW
	 * Same as ReadSString, but has a string-type parameter.
	 * This procedure is provided to simplify migration from Release 1.2 to 1.3.
	 */
	//void readXString(CHAR *out);
	/** 
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
	Store *readStore();
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
	 */ 
	INTEGER readVersion(INTEGER min, INTEGER max);
	/*
	 * PROCEDURE (VAR rd: Reader) TurnIntoAlien (cause: INTEGER)
	 * NEW
	 * A store which is currently being internalized can turn itself into an alien, e.g., if it has read a component store which is an alien.
	 * 
	 * Pre
	 * 20	cause > 0
	 */
	void turnIntoAlien(int cause);

	bool isCancelled();

	private:
	Store *readStoreOrElemStore(bool isElem);
	Store *readNilStore();
	Store *readLinkStore();
	Store *readNewLinkStore();
	void internalizeAlien(Alien *alien, std::streampos down, std::streampos end);
	
	std::string &fixTypeName(std::string &name);
	TypePath readPath();
	/**
	 * Add another component to the current path. If first==true, start a new path.
	 */
	void addPathComponent(bool first, const std::string &typeName);
};

} // namespace odc

#endif // _READER_H_
