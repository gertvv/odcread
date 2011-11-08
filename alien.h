#ifndef _ALIEN_H_
#define _ALIEN_H_

#include <oberon.h>
#include <store.h>
#include <iostream>
#include <vector>
#include <string>

namespace odc {

/**
 * Part of an alien store
 */
struct AlienComponent {
	virtual std::string toString() = 0;
	virtual std::string toPlainText() = 0;
};

/**
 * Totally unstructured part of an alien store
 */
struct AlienPiece : public AlienComponent {
	const char * const data;
	const size_t len;

	AlienPiece(const char * const data, const size_t len);

	virtual std::string toString();
	virtual std::string toPlainText();
};

/**
 * Store component of an alien store
 */
struct AlienPart : public AlienComponent {
	Store * const d_store;

	AlienPart(Store * const store);

	virtual std::string toString();
	virtual std::string toPlainText();
};

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
	virtual std::string toPlainText();
};

}
#endif // _ALIEN_H_
