#include "reader/reader.ih"

namespace odc {

Store *Reader::readNewLinkStore() {
	throw "Reader::readNewLinkStore() not implemented";
}
//		ELSIF kind = newlink THEN
//			rd.ReadInt(id); rd.ReadInt(comment); rd.ReadInt(next);
//			rd.st.end := rd.Pos();
//			IF (next > 0) OR ((next = 0) & ODD(comment)) THEN rd.st.next := rd.st.end + next ELSE rd.st.next := 0 END;
//			x := ThisStore(rd.sDict, id)

} // namespace odc
