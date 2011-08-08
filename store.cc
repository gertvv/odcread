#include <store.h>

namespace odc {

Store::Store(INTEGER id): d_id(id) {}

INTEGER Store::getId() {
	return d_id;
}

Domain* Store::getDomain() {
	return 0;
}

}
