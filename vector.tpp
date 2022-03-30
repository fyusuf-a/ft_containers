#ifndef FT_VECTOR_TPP
#define FT_VECTOR_TPP

#include "vector.hpp"


template<typename T, class Allocator>
ft::Vector<T, Allocator>::Vector()
	: _allocator()
	, _start(0)
	, _end(0)
	, _end_capacity(0)
{
}

template<typename T, class Allocator>
ft::Vector<T, Allocator>::Vector(const Allocator& alloc)
	: _allocator(alloc)
	, _start(0)
	, _end(0)
	, _end_capacity(0)
{
}

template<typename T, class Allocator>
ft::Vector<T, Allocator>::Vector(size_type count, const T& value, const Allocator& alloc)
	: _allocator(alloc)
	, _start(_allocator.allocate(count))
	, _end(_start + count)
	, _end_capacity(_end)
{
	for (size_type i = 0; i < count; ++i)
		_allocator.construct(_start + i, value);
}

template<typename T, class Allocator>
ft::Vector<T, Allocator>::~Vector() {
	_allocator.deallocate(_start, capacity());
}

template<typename T, class Allocator>
bool ft::Vector<T, Allocator>::empty() const {
	return _start == _end;
}

template<typename T, class Allocator>
ft::Vector<T,Allocator>::size_type ft::Vector<T, Allocator>::size() const {
	return _end - _start;
}

template<typename T, class Allocator>
ft::Vector::size_type ft::Vector<T, Allocator>::capacity() const {
	return _end_capacity - _start;
}

#endif
