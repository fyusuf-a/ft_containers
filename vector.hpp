#ifndef FT_VECTOR_HPP
#define FT_VECTOR_HPP

#include <memory>
#include <stdexcept>
#include "iterator.hpp"
#include "type_traits.hpp"
#include "algorithm.hpp"

namespace ft {
template<class T, class Allocator = std::allocator<T> >
class vector {
	public:
		typedef T										value_type;
		typedef Allocator								allocator_type;
		typedef std::size_t								size_type;
		typedef std::ptrdiff_t							difference_type;
		typedef value_type&								reference;
		typedef const value_type& 						const_reference;
		typedef typename Allocator::pointer				pointer;
		typedef typename Allocator::const_pointer		const_pointer;
		typedef pointer									iterator;
		typedef const_pointer							const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

		// constructors
		vector()
			: _allocator()
			, _start(_allocator.allocate(0))
			, _end(_start)
			, _end_capacity(_end)
		{}

		explicit vector(const Allocator& alloc)
			: _allocator(alloc)
			, _start(_allocator.allocate(0))
			, _end(_start)
			, _end_capacity(_end)
		{}

		explicit vector(size_type count, const T& value = T(), const Allocator& alloc = Allocator())
			: _allocator(alloc)
			, _start(_allocator.allocate(0))
			, _end(_start)
			, _end_capacity(_end)
		{
			assign(count, value);
		}

		template<class InputIt>
		vector(InputIt first, typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type last, const Allocator& alloc = Allocator())
			: _allocator(alloc)
			, _start(_allocator.allocate(0))
			, _end(_start)
			, _end_capacity(_end) {
			assign(first, last);
		}

		vector(const vector& other)
			: _allocator(other._allocator)
			, _start(_allocator.allocate(0))
			, _end(_start)
			, _end_capacity(_end) {
			assign(other.begin(), other.end());
		}

		// destructor
		~vector() {
			for (size_type i = 0; i < size(); i++)
				_allocator.destroy(_start + i);
			_allocator.deallocate(_start, capacity());
			_start = 0;
		}

		// operator=
		vector& operator=(const vector& other) {
			if (this != &other) {
				resize(other.size());
				for (size_type i = 0; i < size(); i++)
					_start[i] = other._start[i];
			}
			return *this;
		}

		// assign
	private:
		inline void _assign(size_type count, const T& value) {
			clear();
			_insert(begin(), count, value);
		}
	public:
		void assign(size_type count, const T& value) {
			if (_start <= &value && &value <= _end) {
				T v = value;
				_assign(count, v);
			}
			else
				_assign(count, value);
		}
		template<class InputIt>
		void assign(InputIt first, typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type last) {
			clear();
			insert(begin(), first, last);
		}

		// get_allocator
		allocator_type get_allocator() const {
			return _allocator;
		}

		// element access
		reference at(size_type pos) {
			if (pos >= size())
				throw std::out_of_range("ft::vector::at");
			return _start[pos];
		}
		const_reference at(size_type pos) const {
			if (pos >= size())
				throw std::out_of_range("ft::vector::at");
			return _start[pos];
		}
		reference operator[](size_type pos) {
			return _start[pos];
		}
		const_reference operator[](size_type pos) const {
			return _start[pos];
		}
		reference front() {
			return _start[0];
		}
		const_reference front() const {
			return _start[0];
		}
		reference back() {
			return _start[size() - 1];
		}
		const_reference back() const {
			return _start[size() - 1];
		}

		// iterators
		iterator begin() {
			return _start;
		}
		const_iterator begin() const {
			return _start;
		}
		iterator end() {
			return _end;
		}
		const_iterator end() const {
			return _end;
		}
		reverse_iterator rbegin() {
			return reverse_iterator(end());
		}
		const_reverse_iterator rbegin() const {
			return const_reverse_iterator(end());
		}
		reverse_iterator rend() {
			return reverse_iterator(begin());
		}
		const_reverse_iterator rend() const {
			return const_reverse_iterator(begin());
		}

		// capacity
		bool empty() const {
			return _start == _end;
		}
		size_type size() const {
			return _end - _start;
		}
		size_type max_size() const {
			return _allocator.max_size();
		}
		void reserve(size_type new_cap) {
			if (new_cap >= max_size())
				throw std::length_error("vector::reserve");
			if (new_cap > capacity()) {
				size_t old_size = size();
				pointer new_start = _allocator.allocate(new_cap);
				for (size_type i = 0; i < size(); i++) {
					_allocator.construct(new_start + i, _start[i]);
					_allocator.destroy(_start + i);
				}
				_allocator.deallocate(_start, capacity());
				_start = new_start;
				_end = _start + old_size;
				_end_capacity = _start + new_cap;
			}
		}
		size_type capacity() const {
			return _end_capacity - _start;
		}

		// modifiers
		void clear() {
			erase(begin(), end());
		}

		iterator insert(iterator pos, const T& value) {
			if (pos <= &value && &value <= _end) {
				T v = value;
				return _insert(pos, 1, v);
			}
			return _insert(pos, 1, value);
		}

		void insert(iterator pos, size_type count, const T& value) {
			if (pos <= &value && &value <= _end) {
				T v = value;
				_insert(pos, count, v);
			}
			_insert(pos, count, value);
		}

		private:
		iterator _insert(iterator pos, size_type count, const T& value) {
			difference_type index = pos - begin();
			if (size() + count > capacity())
				reserve(ft::max(2 * size(), size() + count));
			iterator it;
			for (it = _start + size() + count - 1; it >= _start + index + count; it--) {
				if (it >= _start + size())
					_allocator.construct(it, *(it - count));
				else
					*it = *(it - count);
			}
			for (; it >= _start + index; it--) {
				if (it >= _start + size())
					_allocator.construct(it, value);
				else
					*it = value;
			}
			_end = _end + count;
			return _start + index;
		}

		public:
		template<class InputIt>
		void insert(iterator pos, typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type first, InputIt last) {
			difference_type index = pos - begin();
			size_t count = ft::distance(first, last);
			if (size() + count > capacity())
				reserve(ft::max(2 * size(), size() + count));
			iterator it;
			for (it = _start + size() + count - 1; it >= _start + index + count; it--)
			{
				if (it >= _start + size())
					_allocator.construct(it, *(it - count));
				else
					*it = *(it - count);
			}
			last--;
			for (; it >= _start + index; it--)
			{
				if (it >= _start + size())
					_allocator.construct(it, *(last--));
				else
					*it = *(last--);
			}
			_end = _end + count;
		}

		iterator erase(iterator position) {
			return erase(position, position + 1);
		}

		iterator erase(iterator first, iterator last) {
			difference_type number_of_deleted = last - first;
			ft::copy(last, _end, first);
			for (pointer it = _end - number_of_deleted; it != _end; it++)
				_allocator.destroy(it);
			_end -= number_of_deleted;
			return first;
		}

		void push_back(const T& value) {
			if (size() + 1 > capacity()) {
				T v = value; // because reserve can change the reference value (e.g if value = front())
				reserve(ft::max<size_type>(1, 2 * capacity()));
				_allocator.construct(_end, v);
			}
			else
				_allocator.construct(_end, value);
			++_end;
		}

		void pop_back() {
			_allocator.destroy(_end - 1);
			--_end;
		}

		void resize(size_type count, T c = T()) {
			size_type old_size = size();
			if (count > old_size) {
				reserve(ft::max<size_type>(2 * size(), count));
				/*if (2 * count <= max_size())
					reserve(2 * count);
				else
					reserve(count);*/
				for (size_type i = old_size; i < count; i++)	
					_allocator.construct(_start + i, c);
				_end = _start + count;
			}
			else if (count < old_size) {
				for (size_type i = count; i < old_size; i++)
					_allocator.destroy(_start + i);
				_end = _start + count;
			}
		}

		void swap(vector& other) {
			ft::swap(_start, other._start);
			ft::swap(_end, other._end);
			ft::swap(_end_capacity, other._end_capacity);
		}

		
	private:
		Allocator	_allocator;
		pointer		_start;
		pointer		_end;
		pointer 	_end_capacity;
};

template<class T>
bool operator==(const vector<T>& lhs, const vector<T>& rhs) {
	if (lhs.size() != rhs.size())
		return false;
	for (size_t i = 0; i < lhs.size(); i++)
		if (lhs[i] != rhs[i])
			return false;
	return true;
}

template<class T>
bool operator!=(const vector<T>& lhs, const vector<T>& rhs) {
	return !(lhs == rhs);
}

template<class T>
bool operator<(const vector<T>& lhs, const vector<T>& rhs) {
	return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T>
bool operator<=(const vector<T>& lhs, const vector<T>& rhs) {
	return !(rhs < lhs);
}

template<class T>
bool operator>(const vector<T>& lhs, const vector<T>& rhs) {
	return rhs < lhs;
}

template<class T>
bool operator>=(const vector<T>& lhs, const vector<T>& rhs) {
	return !(lhs < rhs);
}

template<class T, class Allocator>
void swap(vector<T, Allocator>& lhs, vector<T, Allocator>& rhs) {
	lhs.swap(rhs);
}

}


#endif
