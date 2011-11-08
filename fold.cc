#include <fold.h>
#include <reader.h>

namespace odc {

const std::string View::TYPENAME("Views.View^");
const TypeProxy<View> View::PROXY;

View::View(INTEGER id) : Store(id) {}

const std::string &View::getType() {
	return TYPENAME;
}

const std::string *View::getSuper() {
	return &Store::getType();
}

const std::string &View::getTypeName() const {
	return getType();
}

void View::internalize(Reader &reader) {
	Store::internalize(reader);
	if (reader.isCancelled()) return;
	reader.readVersion(0, 0);
}

const std::string Fold::TYPENAME("StdFolds.Fold^");
const TypeProxy<Fold> Fold::PROXY;

Fold::Fold(INTEGER id) : View(id) {}

const std::string &Fold::getType() {
	return TYPENAME;
}

const std::string *Fold::getSuper() {
	return &View::getType();
}

const std::string &Fold::getTypeName() const {
	return getType();
}

void Fold::internalize(Reader &reader) {
	View::internalize(reader);
	if (reader.isCancelled()) return;
	reader.readVersion(0, 0);
	if (reader.isCancelled()) return;
//		rd.ReadXInt(xint);fold.leftSide := xint = 0;
	reader.readSInt();
//		rd.ReadXInt(xint); fold.collapsed := xint = 0;
	SHORTINT c = reader.readSInt();
	d_collapsed = (c == 0);
//		rd.ReadXString(fold.label);
	d_label = new SHORTCHAR[32];
	reader.readSString(d_label); // the label
//		rd.ReadStore(store);
	d_hidden = reader.readStore(); // the hidden part
//		IF store # NIL THEN fold.hidden := store(TextModels.Model); Stores.Join(fold.hidden, fold)
//		ELSE fold.hidden := NIL
//		END;
//		fold.leftSide := store # NIL
}

std::string Fold::toString() {
	if (d_hidden == 0) {
		return std::string("Fold(right)");
	}
	return std::string("Fold(left) \"") + std::string(d_label) + std::string("\" { ") + d_hidden->toString() + std::string("  }");
}

void Fold::accept(Visitor &visitor) const {
	if (d_hidden == 0) { // right part
		visitor.foldRight();
	} else { // left part
		visitor.foldLeft(d_collapsed);
		d_hidden->accept(visitor);
	}
}

} // namespace odc
