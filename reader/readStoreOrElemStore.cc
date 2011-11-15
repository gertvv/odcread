#include "reader/module.ih"

namespace odc {

Store *Reader::readStoreOrElemStore(bool isElem) {
	INTEGER id = isElem ? d_elemList.size() : d_storeList.size();
	TypePath path = readPath();
	//std::cout << path.toString() << std::endl;
	const std::string &type = path[0];
	INTEGER comment = readInt();
	std::streampos pos1 = d_rider.tellg();
	std::streamoff next = readInt();
	std::streamoff down = readInt();
	std::streamoff len = readInt();
	std::streampos pos = d_rider.tellg();
	if (next > 0) {
		d_state->next = pos1 + next + (std::streamoff)4;
	} else {
		d_state->next = 0;
	}
	int downPos = 0;
	if (down > 0) {
		downPos = pos1 + down + (std::streamoff)8;
	}
	d_state->end = pos + len;
	d_cause = 0;
	assert(len >= 0);
	if (next != 0) {
		assert(d_state->next > pos1);
		if (down != 0) {
			assert(downPos < d_state->next);
		}
	}
	if (down > 0) {
		assert(downPos > pos1);
		assert(downPos < d_state->end);
	}

	const TypeProxyBase *t = TypeRegister::getInstance().get(type); // FIXME type lookup here
	Store *x = 0;
	if (t != 0) {
		x = t->newInstance(id);
	} else {
		d_cause = TYPENOTFOUND;
	}

	if (x != 0) { // IF READING SUCCEEDS, INSERT MORE CHECKS HERE
		if (true) { // samePath(x, path)
			ReaderState *save = d_state;
			d_state = new ReaderState();
			x->internalize(*this);
			delete d_state;
			d_state = save;

			if (d_cause != 0) {
				x = 0;
			}
			assert(d_rider.tellg() == d_state->end);
			assert(!d_rider.eof());
		} else {
//					rd.cause := inconsistentType; AlienTypeReport(rd.cause, type);
			x = 0;
		}
	}

	if (x != 0) {
		if (d_store == 0) {
			d_store = x;
		} else {
			// join(d_store, x)
		}
		if (isElem) {
			d_elemList.push_back(x);
		} else {
			d_storeList.push_back(x);
		}
	} else {
		d_rider.seekg(pos); // x->internalize() could have left us anywhere
		assert(d_cause != 0);
		Alien *alien = new Alien(id, path); //, d_cause); //,file
		if (d_store == 0) {
			d_store = alien;
		} else {
			// join(d_store, alien)
		}
		if (isElem) {
			d_elemList.push_back(alien);
		} else {
			d_storeList.push_back(alien);
		}
		ReaderState *save = d_state;
		d_state = new ReaderState();
		readAlien(alien, downPos, save->end);
		delete d_state;
		d_state = save;
		assert(d_rider.tellg() == d_state->end);

		// we've just read the alien, so reset the state
		d_cause = 0;
		d_cancelled = false;
		d_readAlien = true;
		return alien;
	}

	return x;
}

} // namespace odc
