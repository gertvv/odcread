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
 * Used by Store.internalize() to read from file.
 */ 
class Reader {
private:
	/**
	 * The store was read as an Alien because its type is not registered.
	 * @see TypeRegister.
	 */
	static const unsigned int TYPENOTFOUND = 1;
	/**
	 * The store was read as an Alien because its version is not in the accepted range.
	 */
	static const unsigned int ALIENVERSION = 2;

	/**
	 * The input stream associated with this reader.
	 */ 
	std::istream &d_rider;

	/**
	 * Whether the currently executing Store.internalize() has been cancelled.
	 * If so, it will be read as an Alien.
	 */
	bool d_cancelled;

	/**
	 * Cause of current read being an Alien.
	 */
	unsigned int d_cause;

	/**
	 * Whether any alien has been read from the input stream.
	 */
	bool d_readAlien;

	/**
	 * The TypeList should be used for consistency checking on type paths.
	 * Currently unused.
	 */
	std::vector<TypeEntry*> d_typeList;

	/**
	 * List of stores that have been read already, to enable repeated occurences
	 * of a single store to reference them.
	 * Elem type stores have IDs that are separate from the IDs of other stores.
	 */
	std::vector<Store*> d_elemList;
	/**
	 * List of stores that have been read already, to enable repeated occurences
	 * of a single store to reference them.
	 * Elem type stores have IDs that are separate from the IDs of other stores.
	 */
	std::vector<Store*> d_storeList;

	/**
	 * Stores the reader state so that we can rewind a failed Store.internalize().
	 */
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

	/**
	 * The store that is currently being read.
	 */
	Store *d_store;

	/**
	 * Reader state at the start of reading the current store (before calling Store.internalize())....
	 */
	ReaderState *d_state;

	public:
	/**
	 * Construct a reader from the istream rider.
	 * @param rider An istream (binary mode).
	 */
	Reader(std::istream &rider);

	/* Omitted reading methods:
	 *
	 * There are a number of ReadX* methods that read a SHORT type but return a LONG type. Those have been omitted.
	 *
	 * PROCEDURE (VAR rd: Reader) ReadBool (OUT x: BOOLEAN)
	 * NEW
	 * Reads a Boolean value.
	 *
	 * PROCEDURE (VAR rd: Reader) ReadLong (OUT x: LONGINT)
	 * NEW
	 * Reads a long integer (-9223372036854775808..9223372036854775807).
	 * 
	 * PROCEDURE (VAR rd: Reader) ReadSReal (OUT x: SHORTREAL)
	 * NEW
	 * Reads a short real (32-bit IEEE number).
	 * 
	 * PROCEDURE (VAR rd: Reader) ReadReal (OUT x: REAL)
	 * NEW
	 * Reads a real (64-bit IEEE number).
	 * 
	 * PROCEDURE (VAR rd: Reader) ReadSet (OUT x: SET)
	 * NEW
	 * Reads a set (32 elements).
	 *
	 * PROCEDURE (VAR rd: Reader) ReadString (OUT x: ARRAY OF CHAR)
	 * NEW
	 * Reads a 0X-terminated string.
	 */ 
	/**
	 * Reads a short character (00X..0FFX).
	 */ 
	SHORTCHAR readSChar();
	/**
	 * Reads a string of short characters (00X..0FFX).
	 */ 
	void readSChar(SHORTCHAR *buf, size_t len);
	/** 
	 * Reads a character (0000X..0FFFFX).
	 */ 
	CHAR readLChar();
	/** 
	 * Reads a string of characters (0000X..0FFFFX).
	 */ 
	void readLChar(CHAR *buf, size_t len);
	/**
	 * Reads a very short integer (-128..127).
	 */ 
	BYTE readByte();
	/**
	 * Reads a short integer (-32768..32767).
	 */
	SHORTINT readSInt();
	/**
	 * Reads an integer (-2147483648..2147483647).
	 */
	INTEGER readInt();
	/**
	 * Reads a 0X-terminated short string.
	 * Used to read string that have a known maximum length.
	 */
	void readSString(SHORTCHAR *out);
	/** 
	 * (Explanation from the BlackBox source.)
	 * Reads a store's type, allocates it, and then reads its contents, by calling the store's Internalize procedure. x may also be NIL, or an alien if the store's module cannot be loaded, or if internalization has been cancelled by the Internalize procedure.
	 * If the store has already been read in, a pointer to the same store is returned instead of allocating a new one. This means that arbitrary graphs that have been written with WriteStore are reconstructed correctly, including alias pointers to the same store, cycles, etc.
	 * If the file on which the reader operates does not contain correct input, then an assertion trap will be caused (traps 101 to trap 106).
	 */
	Store *readStore();
	/**
	 * (Explanation from the BlackBox source.)
	 * Read a version byte and return it in version. If version is not in the
	 * specified range [min .. max], the store currently being read is turned
	 * into an alien, with cause = alienVersion.
	 */ 
	INTEGER readVersion(INTEGER min, INTEGER max);
	/**
	 * (Explanation from the BlackBox source.)
	 * A store which is currently being internalized can turn itself into an
	 * alien, e.g., if it has read a component store which is an alien.
	 */
	void turnIntoAlien(int cause);

	/**
	 * @return Whether the current read has been cancelled.
	 */
	bool isCancelled();

	private:
	/**
	 * Read a store.
	 */
	Store *readStoreOrElemStore(bool isElem);
	/**
	 * Read a Nil store. A Nil store doesn't contain anything, but may require us to skip some bytes.
	 */
	Store *readNilStore();
	/**
	 * Read a link to an Elem-type store.
	 */
	Store *readLinkStore();
	/**
	 * Read a link to a non-Elem-type store.
	 */
	Store *readNewLinkStore();
	/**
	 * Read an alien store.
	 */
	void internalizeAlien(Alien *alien, std::streampos down, std::streampos end);
	
	/**
	 * Make store name consistent with names found in BlackBox source.
	 */
	std::string &fixTypeName(std::string &name);

	/**
	 * Read a type path.
	 */
	TypePath readPath();
	/**
	 * Add another component to the current path. If first==true, start a new path.
	 */
	void addPathComponent(bool first, const std::string &typeName);
};

} // namespace odc

#endif // _READER_H_
