#include "fold/module.ih"

namespace odc {

void Fold::internalize(Reader &reader) {
	View::internalize(reader);
	if (reader.isCancelled()) return;
	reader.readVersion(0, 0);
	if (reader.isCancelled()) return;

	// indicates left/right side, but this is also determined by the presense
	// of a hidden part (so this field unused by BlackBox)
	reader.readSInt();

	// whether the view is collapsed
	SHORTINT c = reader.readSInt();
	d_collapsed = (c == 0);

	// label of the fold (not sure of its use)
	d_label = new SHORTCHAR[32];
	reader.readSString(d_label);

	// the hidden part (NULL indicates this is a right side)
	d_hidden = reader.readStore();
}

} // namespace odc
