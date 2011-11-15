#ifndef _OBERON_H_
#define _OBERON_H_

#include <stdint.h>

namespace odc {
	/**
	 * BOOLEAN: 1 byte (0 = FALSE, 1 = TRUE)
	 */
	typedef bool BOOLEAN;

	/**
	 * SHORTCHAR 1 byte in the Latin-1 character set (i.e., Unicode page 0; 00X..0FFX)
	 */
	typedef char SHORTCHAR;

	/**
	 * CHAR 2 byte in the Unicode character set (0000X..0FFFFX)
	 * (cannot use wchar_t as its width is not fixed)
	 */
	typedef uint16_t CHAR;

	/**
	 * BYTE: 1 byte (-128..127)
	 */
	typedef int8_t BYTE;

	/**
	 * SHORTINT: 2 bytes (-32768..32767)
	 */
	typedef int16_t SHORTINT;


	/**
	 * INTEGER: 4 bytes (-2147483648..2147483647)
	 */
	typedef int32_t INTEGER;

	/**
	 * LONGINT: 8 bytes (-9223372036854775808..9223372036854775807)
	 */
	typedef int64_t LONGINT;

	/**
	 * SHORTREAL: 4 bytes IEEE format
	 */
	typedef float SHORTREAL;

	/**
	 * REAL: 8 bytes IEEE format
	 */
	typedef double REAL;

	/**
	 * SET: 4 bytes (least significant bit = element 0)
	 */
	typedef uint32_t SET;

	/**
	 * Short String: string in the Latin-1 character set, followed by a 00X
	 * i.e. SHORTCHAR*
	 * String: string in the Unicode character set, followed by a 0000X
	 * i.e. CHAR*
	 */
}

#endif
