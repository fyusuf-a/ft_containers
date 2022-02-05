#ifndef FT_VECTOR_HPP
#define FT_VECTOR_HPP

#include <cstddef>
#include <memory>

#define FT_VECTOR_INITIAL_SIZE 50

namespace ft {
template<class T, class Allocator = std::allocator<T> >
class Vector {
	public:
		typedef T value_type;
		typedef Allocator allocator_type;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;
	private:
		size_type	m_official_size;
		size_type	m_true_size;
		pointer 	m_p;
};
}

#ifndef FT_VECTOR_TPP
#include "Vector.tpp"
#endif

#endif
