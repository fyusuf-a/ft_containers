#ifndef FT_VECTOR_TPP
#define FT_VECTOR_TPP

#include "Vector.hpp"


template<class T, class Allocator>
ft::Vector<T, Allocator>::Vector()
	: m_official_size(0)
	, m_true_size(FT_VECTOR_INITIAL_SIZE)
	, m_p(Allocator::allocate(FT_VECTOR_INITIAL_SIZE))
{
}

template<class T, class Allocator>
ft::Vector<T, Allocator>::~Vector() {
	//for (
	Allocator::deallocate(p, m_true_size);
}



/*explicit vector(const Allocator& alloc)
	: m_official_size(0)
	, m_true_size(FT_VECTOR_INITIAL_SIZE)
{

}*/

#endif
