#include "reader/reader.ih"

namespace odc {

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

} // namespace odc
