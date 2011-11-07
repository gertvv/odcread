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

const std::string Fold::TYPENAME("Folds.Fold^");
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
	Store::internalize(reader);
	if (reader.isCancelled()) return;
	reader.readVersion(0, 0);
	if (reader.isCancelled()) return;
	reader.readSInt(); // FIXME IMPLEMENT
//		rd.ReadXInt(xint);fold.leftSide := xint = 0;
//		rd.ReadXInt(xint); fold.collapsed := xint = 0;
//		rd.ReadXString(fold.label);
//		rd.ReadStore(store);
//		IF store # NIL THEN fold.hidden := store(TextModels.Model); Stores.Join(fold.hidden, fold)
//		ELSE fold.hidden := NIL
//		END;
//		fold.leftSide := store # NIL
}

} // namespace odc
