#include <store.h>

namespace odc {

template<class T, class A> T join(const A & begin, const A & end, const T &sep) {
	T result;

	if (begin != end) {
		A it = begin;
		result.append(*it);
		for (++it; it != end; ++it) {
			result.append(sep).append(*it);
		}
	}

	return result;
}

std::string TypePath::toString() const {
	return join(begin(), end(), std::string("->"));
}

Store::Store(INTEGER id): d_id(id) {}

INTEGER Store::getId() {
	return d_id;
}

Domain* Store::getDomain() {
	return 0;
}

}
