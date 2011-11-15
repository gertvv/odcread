#include "typepath/typepath.ih"

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

} // namespace odc
