#ifndef FT_IOITERATOR_TPP
#define FT_IOITERATOR_TPP

#include "IOIterator.hpp"

namespace ft {

template<typename T>
IOIterator<T>::IOIterator() {
}

template<typename T>
IOIterator<T>::~IOIterator() {
}

template<typename T>
IOIterator<T>::IOIterator(const IOIterator&) {
}

template<typename T>
IOIterator<T>::IOIterator& operator=(const IOIterator&) {
}

template<typename T>
bool IOIterator<T>::operator!=(const IOIterator& other) {
	return !operator==(other);
}

}

#endif
