#ifndef _STORE_H_
#define _STORE_H_

#include <string>
#include <vector>

#include "oberon.h"
#include "typeregister/typeregister.h"
#include "visitor/visitor.h"
#include "typepath/typepath.h"

namespace odc {
	class Reader; // forward decl

	/**
	 * TYPE Store
	 * ABSTRACT
	 * Storable extensible data types like Views.View or TextModels.Text are derived from Store.
	 * Stores are typically allocated by suitable directories, e.g., Views.Directory or TextModels.Directory.
	 * Stores are used as base types for all objects that must be both extensible and persistent.
	 */
	class Store {
	private:
		static const TopTypeProxy<Store> PROXY;

		INTEGER d_id;

	public: 
		static const std::string TYPENAME;
		static const SHORTCHAR NEWBASE = 0xF0; // (* new base type (level = 0), i.e. not yet in dict *)
		static const SHORTCHAR NEWEXT = 0xF1;  // (* new extension type (level = 1), i.e. not yet in dict *)
		static const SHORTCHAR OLDTYPE = 0xF2; // (* old type, i.e. already in dict *)
		static const SHORTCHAR NIL = 0x80;     // (* nil store *)
		static const SHORTCHAR LINK = 0x81;    // (* link to another elem in same file *)
		static const SHORTCHAR STORE = 0x82;   // (* general store *)
		static const SHORTCHAR ELEM = 0x83;    // (* elem store *)
		static const SHORTCHAR NEWLINK = 0x84; // (* link to another non-elem store in same file *)

		Store(INTEGER id);

		INTEGER getId();
		
		/**
		 * Get the TypeName for this object.
		 */
		virtual const std::string &getTypeName() const;
		/**
		 * Get the TypePath to this object's type.
		 * I'm not sure why this was necessary, I think BlackBox uses them in some code
		 * I've omitted.
		 * @see TypePath
		 */
		TypePath getTypePath() const;

		/**
		 * PROCEDURE (s: Store) Domain (): Domain
		 * NEW
		 * A store may be associated with a domain. This is done by the procedure InitDomain, which assigns a domain to the store.
		 * Domain may be called by arbitrary clients.
		 */
		//Domain* getDomain();

		/**
		 * Read the contents of "this" from reader.
		 * Just reads the version and checks that its in the allowed range.
		 */
		virtual void internalize(Reader &reader);

		// In BlackBox, a Store will also have an externalize(writer) method.
		// The internalize and externalize should be compatible (internalize
		// should be able to read what externalize writes).

		/**
		 * A debug "toString".
		 */
		virtual std::string toString();

		/**
		 * Receiving end of the Visitor pattern.
		 */
		virtual void accept(Visitor &visitor) const;

		private:
		void calcTypePath(TypePath *out, const std::string &name) const;
	};

	/**
	 * An "Elem" store. Some kind of legacy BlackBox type that has been rolled into Store.
	 * I actually found it easier to keep the two separate.
	 */
	class Elem : public Store {
		private:
		static const TypeProxy<Elem, Store> PROXY;

		public:
		static const std::string TYPENAME;
		virtual const std::string &getTypeName() const;

		Elem(INTEGER id);
		/**
		 * Just calls super and reads the version and checks that its in the allowed range.
		 */
		virtual void internalize(Reader &reader);
	};

	/**
	 * A "Model" store. The basis for all model objects (in MVC framework).
	 * Most objects of interest extend Model.
	 */
	class Model : public Elem {
		private:
		static const TypeProxy<Model, Elem> PROXY;

		public:
		static const std::string TYPENAME;
		virtual const std::string &getTypeName() const;

		Model(INTEGER id);
		/**
		 * Just calls super and reads the version and checks that its in the allowed range.
		 */
		virtual void internalize(Reader &reader);
	};

	/**
	 * Super type for models that contain other stuff (e.g. TextModel).
	 */
	class ContainerModel : public Model {
		private:
		static const TypeProxy<ContainerModel, Model> PROXY;

		public:
		static const std::string TYPENAME;
		virtual const std::string &getTypeName() const;

		ContainerModel(INTEGER id);
		/**
		 * Just calls super and reads the version and checks that its in the allowed range.
		 */
		virtual void internalize(Reader &reader);
	};
}

#endif // _STORE_H_
