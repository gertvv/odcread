#include "reader/module.ih"

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
		return buf - 0x8000;
	} else {
		CHAR out;
		char *outPtr = (char *)&out;
		outPtr[0] = bufPtr[1]; outPtr[1] = bufPtr[0];
		return out - 0x8000;
	}
}

void Reader::readLChar(CHAR *buf, size_t len) {
	for (int i = 0; i < len; ++i) {
		buf[i] = readLChar();
	}
	/*
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
	*/
}

BYTE Reader::readByte() {
	BYTE out;
	d_rider.read((char*)&out, 1);
	return out;
}

SHORTINT Reader::readSInt() {
	SHORTINT buf;
	char *bufPtr = (char*)&buf;
	d_rider.read(bufPtr, 2);
	if (isLittleEndian()) {
		return buf;
	} else {
		SHORTINT out;
		char *outPtr = (char *)&out;
		outPtr[0] = bufPtr[1]; outPtr[1] = bufPtr[0];
		return out;
	}
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

} // namespace odc
