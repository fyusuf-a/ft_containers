#ifndef FT_IOITERATOR_TPP
#define FT_IOITERATOR_TPP

#include "IOIterator.hpp"

namespace ft::IOIterator {

IOIterator() {
}

~IOIterator()

IOIterator(const IOIterator&) {
}

IOIterator& operator=(const IOIterator&) {
}

bool operator!=(const IOIterator& other) {
	return !operator==(other);
}

#endif
