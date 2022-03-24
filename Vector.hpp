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
		typedef pointer iterator;
		typedef const_pointer const_iterator;

		Vector();
		Vector(const Allocator& alloc);
		explicit Vector(size_type count, const T& value = T(), const Allocator& alloc = Allocator())
			: m_allocator(alloc)
			, m_official_size(count)
			, m_true_size(FT_VECTOR_INITIAL_SIZE)
		{
			while (m_true_size < m_official_size)
				m_true_size *= 2;
			m_p = m_allocator.allocate(m_true_size);
			// TODO : use iterators
			for (size_type i = 0; i < m_official_size; i++)
				m_p[i] = value;
		}
		template<class InputIt>
		Vector(InputIt first, InputIt last, const Allocator& alloc = Allocator());
		Vector(const Vector&);
		~Vector();
	private:
		Allocator	m_allocator;
		pointer		m_start;
		pointer		m_finish;
		pointer 	m_end_of_storage;
};
}

#ifndef FT_VECTOR_TPP
#include "Vector.tpp"
#endif

#endif
