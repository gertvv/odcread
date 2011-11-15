#include "textmodel/textmodel.ih"

namespace odc {

const std::string TextModel::TYPENAME("TextModels.Model^");
const TypeProxy<TextModel, ContainerModel> TextModel::PROXY;

TextModel::TextModel(INTEGER id) : ContainerModel(id) {}

const std::string &TextModel::getTypeName() const {
	return TYPENAME;
}

void TextModel::internalize(Reader &reader) {
	ContainerModel::internalize(reader);
	if (reader.isCancelled()) return;
	reader.readVersion(0, 0);
}

const std::string StdTextModel::TYPENAME("TextModels.StdModel^");
const TypeProxy<StdTextModel, TextModel> StdTextModel::PROXY;

StdTextModel::StdTextModel(INTEGER id) : TextModel(id), d_pieces() {}

const std::string &StdTextModel::getTypeName() const {
	return TYPENAME;
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

	std::vector<Store *> dict; // attribute dictionary

	// reads the meta-data
	INTEGER len = reader.readInt(); // lenght of meta-data section
	BYTE ano = reader.readByte();
	while (ano != -1) {
		// This part reads the piece's attributes. These are ignored for now,
		// but may be needed in the future.
		if (ano == dict.size()) {
			dict.push_back(reader.readStore());
		}
		Store *attr = dict[ano];

		INTEGER pieceLen = reader.readInt();
		if (pieceLen > 0) { // shortchar piece
			d_pieces.push_back(new ShortPiece(pieceLen));
		} else if (pieceLen < 0) { // longchar piece
			assert(pieceLen % 2 == 0);
			d_pieces.push_back(new LongPiece(-pieceLen / 2));
		} else { // embedded view
			reader.readInt(); reader.readInt(); // view width + height: ignore
			Store *view = reader.readStore();
			d_pieces.push_back(new ViewPiece(view));
		}
		ano = reader.readByte();
	}

	// reads the pieces described in the meta-data
	for (int i = 0; i < d_pieces.size(); ++i) {
		d_pieces[i]->read(reader);
	}

	// free memory from stores in the dictionary
	for (int i = 0; i < dict.size(); ++i) {
		delete dict[i];
	}
}

std::string StdTextModel::toString() {
	std::string sofar = "StdTextModel { ";
	for (int i = 0; i < d_pieces.size(); ++i) {
		sofar += d_pieces[i]->toString() + " ";
	}
	return sofar + "}";
}

void StdTextModel::accept(Visitor &visitor) const {
	visitor.partStart();
	for (int i = 0; i < d_pieces.size(); ++i) {
		d_pieces[i]->accept(visitor);
	}
	visitor.partEnd();
}

TextPiece::TextPiece(size_t len): d_len(len) {}

unsigned TextPiece::size() const {
	return d_len;
}

LongPiece::LongPiece(size_t len): TextPiece(len * 2) {}

LongPiece::~LongPiece() {
	delete d_buf;
}

void LongPiece::read(Reader &reader) {
	d_buf = new CHAR[d_len / 2 + 1];
	reader.readLChar(d_buf, d_len / 2);
	d_buf[d_len / 2] = 0;
}

std::string LongPiece::toString() const {
	return std::string("LongPiece(FIXME)");
}

CHAR* LongPiece::getBuffer() const {
	return d_buf;
}

void LongPiece::accept(Visitor &visitor) const {
	visitor.textLongPiece(this);
}

ShortPiece::ShortPiece(size_t len): TextPiece(len) {}

ShortPiece::~ShortPiece() {
	delete d_buf;
}

void ShortPiece::read(Reader &reader) {
	d_buf = new SHORTCHAR[d_len + 1];
	reader.readSChar(d_buf, d_len);
	d_buf[d_len] = 0;
}

std::string ShortPiece::toString() const {
	return std::string("ShortPiece(") + std::string(d_buf) + std::string(")");
}

SHORTCHAR* ShortPiece::getBuffer() const {
	return d_buf;
}

void ShortPiece::accept(Visitor &visitor) const {
	visitor.textShortPiece(this);
}

ViewPiece::ViewPiece(Store *view): TextPiece(1), d_view(view) {}

ViewPiece::~ViewPiece() {
	delete d_view;
}

void ViewPiece::read(Reader &reader) {
	reader.readByte();
}

std::string ViewPiece::toString() const {
	return std::string("ViewPiece { ") + d_view->toString() + " }";
}

void ViewPiece::accept(Visitor &visitor) const {
	return d_view->accept(visitor);
}

} // namespace odc
