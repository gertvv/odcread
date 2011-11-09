#ifndef _FOLD_H_
#define _FOLD_H_

#include <oberon.h>
#include <typeregister.h>
#include <store.h>

namespace odc {

	class View : public Store {
		private:
		static const TypeProxy<View, Store> PROXY;

		public:
		static const std::string TYPENAME;
		virtual const std::string &getTypeName() const;

		View(INTEGER id);
		virtual void internalize(Reader &reader);
	};

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
		virtual void internalize(Reader &reader);

		virtual std::string toString();
		virtual void accept(Visitor &visitor) const;
	};

}

#endif // _FOLD_H_
