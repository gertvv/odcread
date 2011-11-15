#include "typeregister/module.ih"

namespace odc {

TypeRegister::TypeRegister(): d_map() {}

TypeRegister *TypeRegister::s_instance = 0;

TypeRegister &TypeRegister::getInstance() {
	if (s_instance == 0) {
		s_instance = new TypeRegister();
	}
	return *s_instance;
}

void TypeRegister::add(const std::string &name, TypeProxyBase *proxy) {
	d_map[name] = proxy;
}

const TypeProxyBase *TypeRegister::get(const std::string &name) {
	return d_map[name];
}

TypeProxyBase::TypeProxyBase(const std::string &name) {
	TypeRegister::getInstance().add(name, this);
}

}
