#ifndef FT_VECTOR_TPP
#define FT_VECTOR_TPP

#include "Vector.hpp"


template<typename T, class Allocator>
ft::Vector<T, Allocator>::Vector()
	: m_official_size(0)
	, m_true_size(FT_VECTOR_INITIAL_SIZE)
	, m_p(m_allocator.allocate(FT_VECTOR_INITIAL_SIZE))
{
}

template<typename T, class Allocator>
ft::Vector<T, Allocator>::~Vector() {
	m_allocator.deallocate(m_p, m_true_size);
}

template<typename T, class Allocator>
ft::Vector<T, Allocator>::Vector(const Allocator& alloc)
	: m_allocator(alloc)
	, m_official_size(0)
	, m_true_size(FT_VECTOR_INITIAL_SIZE)
	, m_p(m_allocator.allocate(m_true_size))
{
}

template<typename T, class Allocator>
ft::Vector<T, Allocator>::Vector(const Vector& other)
{
}

#endif
