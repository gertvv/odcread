#ifndef _TYPEPATH_H_
#define _TYPEPATH_H_

#include <vector>
#include <string>

namespace odc {

	class TypePath : public std::vector<std::string> {
		public:
		std::string toString() const;
	};

}

#endif // _TYPEPATH_H_
