#ifndef FT_VECTOR_HPP
#define FT_VECTOR_HPP

#include <cstddef>
#include <memory>
#include <stdexcept>
#include "iterator.hpp"
#include "type_traits.hpp"
#include "algorithm.hpp"

#define COPY_ELEMENTS 1
#define CHANGE_SIZE 1

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

		// constructors very ok
		vector()
			: _allocator()
			, _start(_allocator.allocate(0))
			, _end(_start)
			, _end_capacity(end)
		{}

		explicit vector(const Allocator& alloc)
			: _allocator(alloc)
			, _start(_allocator.allocate(0))
			, _end(_start)
			, _end_capacity(_end)
		{}

		explicit vector(size_type count, const T& value = T(), const Allocator& alloc = Allocator())
			: _allocator(alloc)
			, _start(_allocator.allocate(count))
			, _end(_start + count)
			, _end_capacity(_end)
		{
			assign(size(), value);
		}

		template<class InputIt, typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type* = 0>
		vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())
			: _allocator(alloc)
			, _start(_allocator.allocate(last - first))
			, _end(_start + (last - first))
			, _end_capacity(_end) {
			assign(first, last);
		}

		vector(const vector& other)
			: _allocator(other._allocator)
			, _start(_allocator.allocate(other._end - other._start))
			, _end(_start + (other._end - other._start))
			, _end_capacity(_end) {
			assign(other.begin(), other.end());
		}

		// destructor very ok
		~vector() {
			for (size_type i = 0; i < size(); i++)
				_allocator.destroy(_start + i);
			_allocator.deallocate(_start, capacity());
		}

		// operator= very ok
		vector& operator=(const vector& other) {
			if (this != &other) {
				_allocator.deallocate(_start, capacity());
				_start = _allocator.allocate(other._end - other._start);
				_end = _start + (other._end - other._start);
				_end_capacity = _end;
				for (size_type i = 0; i < size(); i++)
					_allocator.construct(_start + i, other._start[i]);
			}
			return *this;
		}

		// assign very ok
		void assign(size_type count, const T& value) {
			if (!_maybe_grow(count, !COPY_ELEMENTS, CHANGE_SIZE, value))
				for (size_type i = 0; i < count; i++)
					_allocator.construct(_start + i, value);
		}
		template<class InputIt, typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type* = 0>
		void assign(InputIt first, InputIt last) {
			_maybe_grow(last - first, !COPY_ELEMENTS, CHANGE_SIZE);
			for (InputIt it = first; it != last; it++)
				_allocator.construct(_start + (it - first), *it);
		}

		// get_allocator very ok
		allocator_type get_allocator() const {
			return _allocator;
		}

		// element access very ok
		reference at(size_type pos) {
			if (pos >= size())
				throw std::out_of_range("ft::vector::at");
			return _start[pos];
		}
		const_reference at(size_type pos) const {
			return at(pos);
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

		// iterators ok
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
				throw std::length_error("vector::reserve: new size is too big");
			_maybe_grow(new_cap, COPY_ELEMENTS, !CHANGE_SIZE);
		}
		size_type capacity() const {
			return _end_capacity - _start;
		}

		// modifiers
		// ok
		void clear() {
			erase(begin(), end());
		}

		iterator insert(iterator pos, const T& value) {
			insert(pos, 1, value);
			return pos;
		}

		void insert(iterator pos, size_type count, const T& value) {
			maybe_grow(size() + count - capacity(), COPY_ELEMENTS, !CHANGE_SIZE);
			iterator new_end = _end + count;
			ft::copy_backward(pos, _end, new_end);
			_end = new_end;
			for (iterator it = pos; it != pos + count; it++)
				_allocator.construct(it, value);
		}

		template<class InputIt>
		void insert(iterator pos, InputIt first, InputIt last) {
			size_t count = last - first;
			maybe_grow(size() + count - capacity());
			iterator new_end = _end + count;
			ft::copy_backward(pos, _end, new_end);
			ft::copy(pos, pos + count, first);
		}

		void erase(iterator position) {
			erase(position, position + 1);
		}

		void erase(iterator first, iterator last) {
			difference_type number_of_deleted = last - first;
			ft::copy(last, _end, first);
			ft::destroy(_end - number_of_deleted, number_of_deleted);
			_end -= number_of_deleted;
		}

		void push_back(const T& value) {
			maybe_grow(1);
			_allocator.construct(_end, value);
			++_end;
		}

		private:
		// changes the capacity of the vector to count. The resulting vector
		// contains constructed elements : the first size() elements are copied
		// if copy is true, otherwise, the value is used
		// if the new size is greater than the current capacity, the capacity is
		// increased, and the elements are copied to the new storage if copy is true
		// returns if the growth was done
		bool _maybe_grow(size_t count, bool copy, bool change_size, T& value = T()) {
			size_t old_size = size();
			if (count > capacity()) {
				pointer new_start = _allocator.allocate(count);
				pointer i = new_start;
				for (; i < size(); i++) {
					_allocator.construct(new_start + i, copy ? _start[i] : T());
					_allocator.destroy(_start + i);
				}
				_allocator.deallocate(_start, capacity());
				_start = new_start;
				_end = _start + (change_size ? count : old_size);
				for (; i < size(); i++)
					_allocator.construct(T());
				_end_capacity = _start + count;
				return true;
			}
			return false;
		}

		public:

		// ok
		void _resize(size_type count, T c = T()) {
			size_type old_size = size();
			_maybe_grow(true);
			if (count > old_size) {
				for (pointer i = old_size; i < count; i++)	
					_allocator.construct(_start + i, c);
			}
			else if (count < old_size) {
				for (size_type i = count; i < old_size; i++)
					_allocator.destroy(_start + i);
			}
		}

		
	private:
		Allocator	_allocator;
		pointer		_start;
		pointer		_end;
		pointer 	_end_capacity;
};
}

#endif
