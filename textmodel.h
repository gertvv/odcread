#ifndef _TEXTMODEL_H_
#define _TEXTMODEL_H_

#include <oberon.h>
#include <typeregister.h>
#include <store.h>

namespace odc {

	class TextModel : public ContainerModel {
		private:
		static const std::string TYPENAME;
		static const TypeProxy<TextModel> PROXY;

		public:
		TextModel(INTEGER id);
		static const std::string &getType();
		static const std::string *getSuper();
		virtual const std::string &getTypeName() const;
		virtual void internalize(Reader &reader);
	};

	class StdTextModel : public TextModel {
		private:
		static const std::string TYPENAME;
		static const TypeProxy<StdTextModel> PROXY;

		public:
		StdTextModel(INTEGER id);
		static const std::string &getType();
		static const std::string *getSuper();
		virtual const std::string &getTypeName() const;
		virtual void internalize(Reader &reader);
	};

} // namespace odc

#endif // _TEXTMODEL_H_
