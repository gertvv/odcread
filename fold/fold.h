#ifndef _FOLD_H_
#define _FOLD_H_

#include "oberon.h"
#include "typeregister/typeregister.h"
#include "store/store.h"

namespace odc {

	/**
	 * Supertype for views (in MVC framework).
	 * Included because a Fold is a View and not a Model, for some reason.
	 */
	class View : public Store {
		private:
		static const TypeProxy<View, Store> PROXY;

		public:
		static const std::string TYPENAME;
		virtual const std::string &getTypeName() const;

		View(INTEGER id);
		/**
		 * Just calls super and reads the version and checks that its in the allowed range.
		 */
		virtual void internalize(Reader &reader);
	};

	/**
	 * Folds are collapsible components in a text document.
	 */
	class Fold : public View {
		private:
		static const TypeProxy<Fold, View> PROXY;

		Store *d_hidden;
		SHORTCHAR *d_label;
		bool d_collapsed;

		public:
		static const std::string TYPENAME;
		virtual const std::string &getTypeName() const;

		Fold(INTEGER id);
		~Fold();
		/**
		 * Calls super and reads the version and checks that its in the allowed range.
		 * Then reads the state of the Fold, including the hidden part.
		 */
		virtual void internalize(Reader &reader);

		virtual std::string toString();
		virtual void accept(Visitor &visitor) const;
	};

}

#endif // _FOLD_H_
