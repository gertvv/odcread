#ifndef _CLASSREGISTER_H_
#define _CLASSREGISTER_H_

#include <map>
#include <string>

#include "oberon.h"

namespace odc {
	class Store;
	class TypeProxyBase; // forward declaration

	/**
	 * Register of Oberon/BlackBox types.
	 * Each type has a registered name, which refers to the full BlackBox type name (including any modules).
	 * It also (optionally) has a supertype (also referred to by the full BlackBox type name).
	 */
	class TypeRegister {
		static TypeRegister *s_instance;

		std::map<std::string, TypeProxyBase *> d_map;

		TypeRegister();
		TypeRegister(const TypeRegister &other); // NI
		TypeRegister &operator=(const TypeRegister &other); // NI

		public:
		/**
		 * Get an instance of the type register (singleton pattern).
		 */
		static TypeRegister &getInstance();

		/**
		 * Register a new type.
		 */
		void add(const std::string &name, TypeProxyBase *proxy);

		/**
		 * Get a type's proxy.
		 */
		const TypeProxyBase *get(const std::string &name);
	};

	/**
	 * Proxy to represent a BlackBox type. Has a name and optional supertype name.
	 * Can instantiate new instances of the type.
	 */
	class TypeProxyBase {
		public:
		/**
		 * Create a new TypeProxy and register it with the TypeRegister.
		 */
		TypeProxyBase(const std::string &name);
		/**
		 * @return The full BlackBox type name (including modules).
		 */
		virtual const std::string &getName() const = 0;
		/**
		 * @return The full BlackBox type name of the supertype (if applicable, otherwise a 0-pointer).
		 */
		virtual const std::string *getSuper() const = 0;
		/**
		 * @return A new instance of the type, with the given ID.
		 */
		virtual Store *newInstance(INTEGER id) const = 0;
	};

	/**
	 * Proxy for a toplevel type (no supertype).
	 * T: the class to proxy for.
	 * Requires T to have a static const std::string TYPENAME.
	 */
	template <class T> class TopTypeProxy : public TypeProxyBase {
		public:
		TopTypeProxy(): TypeProxyBase(T::TYPENAME) {}
		virtual const std::string &getName() const {
			return T::TYPENAME;
		}
		virtual const std::string *getSuper() const {
			return 0;
		}
		virtual Store *newInstance(INTEGER id) const {
			return new T(id);
		}
	};

	/**
	 * Proxy for a derived type (with supertype).
	 * T: the class to proxy for.
	 * S: the supertype.
	 * Requires T, S to have a static const std::string TYPENAME.
	 */
	template <class T, class S> class TypeProxy : public TypeProxyBase {
		public:
		TypeProxy(): TypeProxyBase(T::TYPENAME) {}
		virtual const std::string &getName() const {
			return T::TYPENAME;
		}
		virtual const std::string *getSuper() const {
			return &S::TYPENAME;
		}
		virtual Store *newInstance(INTEGER id) const {
			return new T(id);
		}
	};
}

#endif // _CLASSREGISTER_H_
