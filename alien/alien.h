#ifndef _ALIEN_H_
#define _ALIEN_H_

#include "oberon.h"
#include "store/store.h"
#include <vector>
#include <string>

namespace odc {

class Visitor;

/**
 * Part of an alien store
 */
struct AlienComponent {
	virtual std::string toString() = 0;
	virtual void accept(Visitor &visitor) const = 0;
};

/**
 * Totally unstructured part of an alien store
 */
struct AlienPiece : public AlienComponent {
	char const * const data;
	size_t const len;

	AlienPiece(const char * const data, const size_t len);

	virtual std::string toString();
	virtual void accept(Visitor &visitor) const;
};

/**
 * Store component of an alien store
 */
struct AlienPart : public AlienComponent {
	Store * const d_store;

	AlienPart(Store * const store);

	virtual std::string toString();
	virtual void accept(Visitor &visitor) const;
};

/**
 * Any type that's not registered is treated as an "Alien".
 * This allows us to read files even if they contain things we're not aware of.
 * The alien will consist of AlienComponents, some of which we may be able to read.
 */
class Alien : public Store {
	const TypePath d_path;
	std::vector<AlienComponent*> d_comps;

	public:

	Alien(INTEGER id, const TypePath &path);

	std::vector<AlienComponent*> & getComponents();

	// cause-: INTEGER;	(** # 0, the cause that turned this store into an alien **)
	// file-: Files.File;	(** base file holding alien pieces **)
	// comps-: AlienComp	(** the constituent components of this alien store **)

	virtual std::string toString();
	virtual void accept(Visitor &visitor) const;
};

}
#endif // _ALIEN_H_
