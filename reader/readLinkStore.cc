#include "reader/module.ih"

namespace odc {

Store *Reader::readLinkStore() {
	throw "Reader::readLinkStore() not implemented";
}
//		ELSIF kind = link THEN
//			rd.ReadInt(id); rd.ReadInt(comment); rd.ReadInt(next);
//			rd.st.end := rd.Pos();
//			IF (next > 0) OR ((next = 0) & ODD(comment)) THEN rd.st.next := rd.st.end + next ELSE rd.st.next := 0 END;
//			x := ThisStore(rd.eDict, id)

} // namespace odc
