#ifndef _CLASSREGISTER_H_
#define _CLASSREGISTER_H_

#include <oberon.h>
#include <map>
#include <string>

namespace odc {
	class Store;
	class TypeProxyBase; // forward declaration

	class TypeRegister {
		static TypeRegister *s_instance;

		std::map<std::string, TypeProxyBase *> d_map;

		TypeRegister();
		TypeRegister(const TypeRegister &other); // NI
		TypeRegister &operator=(const TypeRegister &other); // NI

		public:
		static TypeRegister &getInstance();

		void add(const std::string &name, TypeProxyBase *proxy);
		const TypeProxyBase *get(const std::string &name);
	};

	class TypeProxyBase {
		public:
		TypeProxyBase(const std::string &name);
		virtual const std::string &getName() const = 0;
		virtual const std::string *getSuper() const = 0;
		virtual Store *newInstance(INTEGER id) const = 0;
	};

	template <class T> class TypeProxy : public TypeProxyBase {
		public:
		TypeProxy(): TypeProxyBase(T::getType()) {}
		virtual const std::string &getName() const {
			return T::getType();
		}
		virtual const std::string *getSuper() const {
			return T::getSuper();
		}
		virtual Store *newInstance(INTEGER id) const {
			return new T(id);
		}
	};
}

#endif // _CLASSREGISTER_H_
