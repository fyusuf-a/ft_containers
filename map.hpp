#ifndef MAP_HPP
#define MAP_HPP

#include "tree.hpp"
#include "utility.hpp"

namespace ft {

template<class K, class V, class Compare = std::less<K>, class Alloc = std::allocator<ft::pair<const K, V> > >
class map : public BinaryTree<K, V, Compare, Alloc>
{
	public:

	typedef K														key_type;
	typedef V														mapped_type;
	typedef ft::pair<const K, V>									value_type;
	typedef typename BinaryTree<K, V, Compare, Alloc>::size_type	size_type;
	typedef std::ptrdiff_t											difference_type;
	typedef Compare													key_compare;
	typedef Alloc													allocator_type;
	typedef value_type&												reference;
	typedef const value_type&	 									const_reference;
	typedef typename Alloc::pointer									pointer;
	typedef typename Alloc::const_pointer							const_pointer;
	typedef NodeIterator<K, V, Compare, Alloc>						iterator;
	typedef NodeIterator<K, const V, Compare, Alloc>				const_iterator;
	typedef ft::reverse_iterator<iterator>							reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>					const_reverse_iterator;

	private:
	typedef typename BinaryTree<K, V, Compare, Alloc>::node_type	node_type;

	public:
	class value_compare
	{
		//friend class map;
		protected:
		Compare comp;

		public:
		value_compare(const Compare& comp) : comp(comp) {}

		bool operator()(const value_type& lhs, const value_type& rhs) const
		{
			return comp(lhs.first, rhs.first);
		}
	};

	// Constructors
	map() : BinaryTree<K, V, Compare, Alloc>() {}

	explicit map(const Compare& comp, const Alloc& alloc = Alloc()) : BinaryTree<K, V, Compare, Alloc>(comp, alloc) {}

	private:
	// the next function is used to optimize a constructor of map
	node_type* insert_if_sorted(node_type* where, const K& k, const V& v) {
		node_type* new_node = this->_allocator.allocate(1, where);
		new (new_node) node_type(k, v, where);
		if (where)
			where->right = new_node;
		update_balance_insert(new_node);
		this->_size += 1;
		return new_node;
	}

	// TODO: test
	public:
	template<class InputIt>
	map(InputIt first, InputIt last, const Compare& comp = Compare(), const Alloc& alloc = Alloc()) : BinaryTree<K, V, Compare, Alloc>(comp, alloc)
	{
		InputIt it = first;
		K this_key;
		K last_key;
		V this_value;
		V last_value;
		node_type* this_node = nullptr;
		for (; it != last; ++it) {
			this_key = it->first;
			this_value = it->second;
			this_node = this->_insert(this_node, this_key, this_value);
			if (it != first && (comp(this_key, last_key) || this_key == last_key))
				break;
			last_key = this_key;
			last_value = this_value;
		}
		if (it == last)
			return;
		it++;
		for (; it != last; ++it)
			insert(*it);
	}

	map(const map& other) : BinaryTree<K, V, Compare, Alloc>(other) {}

	// Destructor
	virtual ~map() {}

	/*// Assignment
	map& operator=(const map& other)
	{
		BinaryTree<K, V, Compare, Alloc>::operator=(other);
		return *this;
	}*/

	allocator_type get_allocator() const
	{
		return this->_allocator;
	}

	// Element access
	
	V& at(const key_type& key)
	{
		node_type* searched_node = this->_find(key);
		if (searched_node == nullptr)
			throw std::out_of_range("ft::map::at");
		return searched_node->data.second;
	}

	const V& at(const key_type& key) const
	{
		node_type* searched_node = _find(key);
		if (searched_node == nullptr)
			throw std::out_of_range("ft::map::at");
		return searched_node->_value.second;
	}

	V& operator[](const key_type& key)
	{
		return insert(ft::make_pair(key, V())).first->second;
	}

	// Iterators

	iterator begin() {
		return iterator(ft::BinaryTree<K, V, Compare, Alloc>::Node::minimum(this->_root));
	}
	const_iterator begin() const {
		return const_iterator(this->minimum(this->_root));
	}
	iterator end() {
		return iterator(nullptr);
	}
	const_iterator end() const {
		return const_iterator(nullptr);
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

	// Capacity

	bool empty() const {
		return this->_size == 0;
	}

	// size already defined in parent class

	size_type max_size() const {
		return this->_allocator.max_size();
	}

	// Modifiers
	// clear already defined in parent class

	ft::pair<iterator, bool> insert(const value_type& value) {
		size_type size = this->size();
		node_type* node = this->_insert(value.first, value.second);
		if (this->size() == size + 1)
			return ft::make_pair(iterator(node), true);
		else
			return ft::make_pair(iterator(node), false);
	}

	iterator insert(iterator hint, const value_type& value) {
		if (_find(value.first) != nullptr)
			return end();
		return iterator(_insert(value.first, value.second, hint._node));
	}

	template<class InputIt>
	void insert(InputIt first, InputIt last) {
		for (; first != last; ++first)
			insert(*first);
	}

	void erase(iterator pos)
	{
		node_type* node_to_delete = nullptr;
		begin_erase(pos._node, pos->first, &node_to_delete);
		ASSERT(node_to_delete != nullptr);
		this->_size -= 1;
		end_erase(node_to_delete);
	}

	// TODO: wrong complexity ?
	void erase(iterator first, iterator last)
	{
		while (first != last)
			erase(first++);
	}

	void swap(map& other)
	{
		ft::swap(this->_compare, other._compare);
		ft::swap(this->_root, other._root);
		ft::swap(this->_size, other.size);
	}

	// Lookup
	
	size_t count(const K& key) const {
		return _find(key) == nullptr ? 0 : 1;
	}

	iterator find(const K& key) {
		return iterator(_find(key));
	}

	const_iterator find(const K& key) const {
		return const_iterator(_find(key));
	}

	ft::pair<iterator, iterator> equal_range(const K& key) {
		return ft::make_pair(lower_bound(key), upper_bound(key));
	}

	ft::pair<const_iterator, const_iterator> equal_range(const K& key) const {
		return ft::make_pair(lower_bound(key), upper_bound(key));
	}
	
	iterator lower_bound(const K& key) {
		for (iterator it = begin(); it != end(); ++it) {
			if (!this->_compare(it->first, key))
				return it;
		}
		return end();
	}

	const_iterator lower_bound(const K& key) const {
		for (const_iterator it = begin(); it != end(); ++it) {
			if (!this->_compare(it->first, key))
				return it;
		}
		return end();
	}

	iterator upper_bound(const K& key) {
		for (iterator it = begin(); it != end(); ++it) {
			if (this->_compare(key, it->first))
				return it;
		}
		return end();
	}

	const_iterator upper_bound(const K& key) const {
		for (const_iterator it = begin(); it != end(); ++it) {
			if (this->_compare(key, it->first))
				return it;
		}
		return end();
	}
	
	// Observers
	
	key_compare key_comp() const {
		return key_compare(this->_compare);
	}

	value_compare value_comp() const {
		return value_compare(this->_compare);
	}
};

template<class K, class V, class Compare, class Alloc>
bool operator==(const map<K, V, Compare, Alloc>& lhs, const map<K, V, Compare, Alloc>& rhs) {
	if (lhs.size() != rhs.size())
		return false;
	for (size_t i = 0; i < lhs.size(); i++)
		if (lhs[i] != rhs[i])
			return false;
	return true;
}

template<class K, class V, class Compare, class Alloc>
bool operator!=(const map<K, V, Compare, Alloc>& lhs, const map<K, V, Compare, Alloc>& rhs) {
	return !(lhs == rhs);
}

template<class K, class V, class Compare, class Alloc>
bool operator<(const map<K, V, Compare, Alloc>& lhs, const map<K, V, Compare, Alloc>& rhs) {
	return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class K, class V, class Compare, class Alloc>
bool operator<=(const map<K, V, Compare, Alloc>& lhs, const map<K, V, Compare, Alloc>& rhs) {
	return !(rhs < lhs);
}

template<class K, class V, class Compare, class Alloc>
bool operator>(const map<K, V, Compare, Alloc>& lhs, const map<K, V, Compare, Alloc>& rhs) {
	return rhs < lhs;
}

template<class K, class V, class Compare, class Alloc>
bool operator>=(const map<K, V, Compare, Alloc>& lhs, const map<K, V, Compare, Alloc>& rhs) {
	return !(lhs < rhs);
}

template<class K, class V, class Compare, class Alloc>
void swap(map<K, V, Compare, Alloc>& lhs, map<K, V, Compare, Alloc>& rhs) {
	lhs.swap(rhs);
}

}


#endif
