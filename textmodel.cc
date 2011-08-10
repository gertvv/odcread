#include <textmodel.h>
#include <reader.h>

#include <vector>

namespace odc {

const std::string TextModel::TYPENAME("TextModels.Model^");
const TypeProxy<TextModel> TextModel::PROXY;

TextModel::TextModel(INTEGER id) : ContainerModel(id) {}

const std::string &TextModel::getType() {
	return TYPENAME;
}

const std::string *TextModel::getSuper() {
	return &ContainerModel::getType();
}

const std::string &TextModel::getTypeName() const {
	return getType();
}

void TextModel::internalize(Reader &reader) {
	ContainerModel::internalize(reader);
	if (reader.isCancelled()) return;
	reader.readVersion(0, 0);
}

const std::string StdTextModel::TYPENAME("TextModels.StdModel^");
const TypeProxy<StdTextModel> StdTextModel::PROXY;

StdTextModel::StdTextModel(INTEGER id) : TextModel(id) {}

const std::string &StdTextModel::getType() {
	return TYPENAME;
}

const std::string *StdTextModel::getSuper() {
	return &TextModel::getType();
}

const std::string &StdTextModel::getTypeName() const {
	return getType();
}

/*
 * This is my current theory of how the StdTextModel storage format works:
 * byte 0: version
 * byte 1-5: len (length of piece descriptions)
 * byte 6-5+len: piece descriptions
 * byte 6+len-end: pieces (length of each defined in piece descriptions)
 */
void StdTextModel::internalize(Reader &reader) {
	TextModel::internalize(reader);
	if (reader.isCancelled()) return;
	reader.readVersion(0, 1);
	if (reader.isCancelled()) return;

	std::vector<Store *> dict;
	INTEGER len = reader.readInt();
	BYTE ano = reader.readByte();
	std::cout << "len " << len << " ano " << (int)ano << std::endl;
	while (ano != -1) {
		if (ano == dict.size()) {
			Store *attr = reader.readStore(); // readAttributes(); Stores.Join(t, attr)
			dict.push_back(attr);
		}
		INTEGER pieceLen = reader.readInt();
		if (pieceLen > 0) { // shortchar piece
			std::cout << "Found SChar piece" << std::endl;
//				NEW(sp); sp.len := len; sp.attr := attr;
//				sp.file := rd.rider.Base(); sp.org := org; un := sp;
//				INC(org, len) -- increment org by len ?
		} else if (pieceLen < 0) { // longchar piece
			std::cout << "Found LChar piece" << std::endl;
//				len := -len; ASSERT(~ODD(len), 100);
//				NEW(lp); lp.len := len DIV 2; lp.attr := attr;
//				lp.file := rd.rider.Base(); lp.org := org; un := lp;
//				INC(org, len) -- increment org by len ?
		} else { // embedded view
			reader.readInt(); reader.readInt();
			Store *view = reader.readStore(); // fixme: save somewhere
//				NEW(v); v.len := 1; v.attr := attr;
//				rd.ReadInt(v.w); rd.ReadInt(v.h); Views.ReadView(rd, v.view);
//				v.view.InitContext(NewContext(v, t));
//				un := v; INC(org) -- increment org by one?? WTH?
		}
		ano = reader.readByte();
	}
//		rd.SetPos(org);
}
//	PROCEDURE (t: StdModel) Internalize (VAR rd: Stores.Reader);
//		VAR u, un: Run; sp: Piece; lp: LPiece; v: ViewRef;
//			org, len: INTEGER; ano: BYTE; thisVersion: INTEGER;
//			attr: Attributes; dict: AttrDict;
//	BEGIN
//		ASSERT(t.Domain() = NIL, 20); ASSERT(t.len = 0, 21);
//		t.Internalize^(rd); IF rd.cancelled THEN RETURN END;
//		rd.ReadVersion(minVersion, maxStdModelVersion, thisVersion);
//		IF rd.cancelled THEN RETURN END;
//		StdInit(t);
//		dict.len := 0; u := t.trailer;
//		rd.ReadInt(len); org := rd.Pos() + len;
//		rd.ReadByte(ano);
//		WHILE ano # -1 DO
//			IF ano = dict.len THEN
//				ReadAttr(rd, attr); Stores.Join(t, attr);
//				IF dict.len < dictSize THEN dict.attr[dict.len] := attr; INC(dict.len) END
//			ELSE
//				attr := dict.attr[ano]
//			END;
//			rd.ReadInt(len);
//			IF len > 0 THEN	(* piece *)
//				NEW(sp); sp.len := len; sp.attr := attr;
//				sp.file := rd.rider.Base(); sp.org := org; un := sp;
//				INC(org, len)
//			ELSIF len < 0 THEN	(* longchar piece *)
//				len := -len; ASSERT(~ODD(len), 100);
//				NEW(lp); lp.len := len DIV 2; lp.attr := attr;
//				lp.file := rd.rider.Base(); lp.org := org; un := lp;
//				INC(org, len)
//			ELSE	(* len = 0  =>  embedded view *)
//				NEW(v); v.len := 1; v.attr := attr;
//				rd.ReadInt(v.w); rd.ReadInt(v.h); Views.ReadView(rd, v.view);
//				v.view.InitContext(NewContext(v, t));
//				un := v; INC(org)
//			END;
//			INC(t.len, un.len); u.next := un; un.prev := u; u := un;
//			rd.ReadByte(ano)
//		END;
//		rd.SetPos(org);
//		u.next := t.trailer; t.trailer.prev := u
//	END Internalize;
//

} // namespace odc
