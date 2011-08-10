#include <store.h>
#include <reader.h>

#include <iostream>


namespace odc {

template<class T, class A> T join(const A & begin, const A & end, const T &sep) {
	T result;

	if (begin != end) {
		A it = begin;
		result.append(*it);
		for (++it; it != end; ++it) {
			result.append(sep).append(*it);
		}
	}

	return result;
}

std::string TypePath::toString() const {
	return join(begin(), end(), std::string("->"));
}

const std::string Store::TYPENAME("Stores.Store^");
const TypeProxy<Store> Store::PROXY;
TypePath *Store::s_typePath = 0;

Store::Store(INTEGER id): d_id(id) {}

INTEGER Store::getId() {
	return d_id;
}

const std::string &Store::getType() {
	return Store::TYPENAME;
}

const std::string *Store::getSuper() {
	return 0;
}

const std::string &Store::getTypeName() const {
	return Store::getType();
}

const TypePath &Store::getTypePath() const {
	if (s_typePath == 0) {
		s_typePath = calcTypePath(getTypeName());
	}
	return *s_typePath;
}

TypePath *Store::calcTypePath(const std::string &name) const {
	const std::string *super = TypeRegister::getInstance().get(name)->getSuper();
	TypePath *path;
	if (super != 0) {
		path = calcTypePath(*super);
	} else {
		path = new TypePath();
	}
	path->push_back(name);
	return path;
}

void Store::internalize(Reader &reader) {
	reader.readVersion(0, 0);
}

std::string Store::toString() {
	return getTypeName(); 
}

const std::string Elem::TYPENAME("Stores.Elem^");
const TypeProxy<Elem> Elem::PROXY;

Elem::Elem(INTEGER id) : Store(id) {}

const std::string &Elem::getType() {
	return TYPENAME;
}

const std::string *Elem::getSuper() {
	return &Store::getType();
}

const std::string &Elem::getTypeName() const {
	return getType();
}

void Elem::internalize(Reader &reader) {
	Store::internalize(reader); // just being explicit
}

const std::string Model::TYPENAME("Models.Model^");
const TypeProxy<Model> Model::PROXY;

Model::Model(INTEGER id) : Elem(id) {}

const std::string &Model::getType() {
	return TYPENAME;
}

const std::string *Model::getSuper() {
	return &Elem::getType();
}

const std::string &Model::getTypeName() const {
	return getType();
}

void Model::internalize(Reader &reader) {
	Elem::internalize(reader);
	reader.readVersion(0, 0);
}

const std::string ContainerModel::TYPENAME("Containers.Model^");
const TypeProxy<ContainerModel> ContainerModel::PROXY;

ContainerModel::ContainerModel(INTEGER id) : Model(id) {}

const std::string &ContainerModel::getType() {
	return TYPENAME;
}

const std::string *ContainerModel::getSuper() {
	return &Model::getType();
}

const std::string &ContainerModel::getTypeName() const {
	return getType();
}

void ContainerModel::internalize(Reader &reader) {
	Model::internalize(reader);
	reader.readVersion(0, 0);
}

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

void StdTextModel::internalize(Reader &reader) {
	TextModel::internalize(reader);
	reader.readVersion(0, 1);

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
}



} // namespace odc
