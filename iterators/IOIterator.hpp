#ifndef FT_IOITERATOR_HPP
#define FT_IOITERATOR_HPP

#include <cstddef>
#include <memory>

namespace ft {
template<typename T>
class IOIterator {
	public:
		IOIterator();
		virtual ~IOIterator();
		IOIterator(const IOIterator&);
		IOIterator& operator=(const IOIterator&);
		virtual bool operator==(const IOIterator&) = 0;
		virtual bool operator!=(const IOIterator&);
		virtual T operator*();
};
}

#ifndef FT_IOITERATOR_TPP
#include "IOIterator.tpp"
#endif

#endif
