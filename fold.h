#ifndef _TEXTMODEL_H_
#define _TEXTMODEL_H_

#include <oberon.h>
#include <typeregister.h>
#include <store.h>

namespace odc {

	class View : public Store {
		private:
		static const std::string TYPENAME;
		static const TypeProxy<View> PROXY;

		public:
		View(INTEGER id);
		static const std::string &getType();
		static const std::string *getSuper();
		virtual const std::string &getTypeName() const;
		virtual void internalize(Reader &reader);
	};

	class Fold : public View {
		private:
		static const std::string TYPENAME;
		static const TypeProxy<Fold> PROXY;

		public:
		Fold(INTEGER id);
		static const std::string &getType();
		static const std::string *getSuper();
		virtual const std::string &getTypeName() const;
		virtual void internalize(Reader &reader);
	};

}

#endif
