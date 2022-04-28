#ifndef MAP_HPP
#define MAP_HPP

#include "tree.hpp"
#include "utility.hpp"

namespace ft {

template<class K, class V, class Compare, class Alloc>
class map;

// a tree, but with iterators
template<class K, class V, class Compare = std::less<K>, class Alloc = std::allocator<ft::pair<const K, V> > >
class map : public BinaryTree<ft::pair<const K, V> >
{
	public:

	typedef K										key_type;
	typedef V										mapped_type;
	typedef ft::pair<const K, V>					value_type;
	typedef std::size_t								size_type;
	typedef std::ptrdiff_t							difference_type;
	typedef Compare									key_compare;
	typedef Alloc									allocator_type;
	typedef value_type&								reference;
	typedef const value_type&	 					const_reference;
	typedef typename Alloc::pointer					pointer;
	typedef typename Alloc::const_pointer			const_pointer;
	typedef NodeIterator<value_type>				iterator;
	typedef NodeConstIterator<value_type>			const_iterator;
	typedef ft::reverse_iterator<iterator>			reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

#ifdef FT_TEST
	public:
#else
	private:
#endif
	typedef Node<value_type>										node_type;

	private:
	typedef typename Alloc::template rebind<node_type>::other		node_allocator;

	size_type														_size;
	node_allocator													_allocator;
	Compare															_compare;


	// attention, does not update balance_factor
	node_type* update_data(node_type* node, const K& k, const V& v) {
		node_type* new_node = _allocator.allocate(1, node);
		new (new_node) node_type(value_type(k, v), node->parent,
				node->left, node->right, node->balance_factor);
		MY_ASSERT(new_node->data.first == k);
		MY_ASSERT(new_node->data.second == v);
		MY_ASSERT(new_node->parent == node->parent);
		MY_ASSERT(new_node->left == node->left);
		MY_ASSERT(new_node->right == node->right);
		if (node_type::is_left_child(node))
			node->parent->left = new_node;
		else if (node_type::is_right_child(node))
			node->parent->right = new_node;
		else
			this->_root = new_node;
		if (node->left)
			node->left->parent = new_node;
		if (node->right)
			node->right->parent = new_node;
		_allocator.destroy(node);
		_allocator.deallocate(node, 1);
		return new_node;
	}

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
	map()
		: BinaryTree<value_type>()
		, _size(0)
		, _allocator(node_allocator())
		, _compare(Compare())
	{}

	explicit map(const Compare& comp, const Alloc& alloc = Alloc())
		: BinaryTree<value_type>()
		, _size(0)
		, _allocator(node_allocator(alloc))
		, _compare(comp)
	{}

	private:
	// the next function is used to optimize a constructor of map
	node_type* insert_if_sorted(node_type* where, const K& k, const V& v) {
		node_type* new_node = this->_allocator.allocate(1, where);
		new (new_node) node_type(value_type(k, v), where);
		if (where)
			where->right = new_node;
		update_balance_insert(new_node);
		this->_size += 1;
		this->update_first_and_last();
		return new_node;
	}

	// TODO: test
	public:
	template<class InputIt>
	map(InputIt first, InputIt last, const Compare& comp = Compare(), const Alloc& alloc = Alloc())
		: BinaryTree<ft::pair<const K, V> >()
		, _size(0)
		, _allocator(node_allocator(alloc))
		, _compare(comp)
	{
		InputIt it = first;
		K this_key;
		K last_key;
		node_type* this_node = nullptr;
		for (; it != last; ++it) {
			this_key = it->first;
			this_node = this->_insert(this_node, this_key, it->second);
			if (it != first && (comp(this_key, last_key) || this_key == last_key))
				break;
			last_key = this_key;
		}
		if (it == last) {
			this->update_first_and_last();
			return;
		}
		it++;
		for (; it != last; ++it)
			insert(*it);
	}

	node_type* _copy_node(node_type* current, node_type* other) {
		if (!other)
			return nullptr;
		node_type* new_node = _allocator.allocate(1, current);
		new (new_node) node_type(value_type(other->data.first, other->data.second),
				current, 0, 0, other->balance_factor);
		new_node->left = _copy_node(new_node, other->left);
		new_node->right = _copy_node(new_node, other->right);
		return new_node;
	}

	map(const map& other)
		: BinaryTree<value_type>()
		, _size(other._size)
		, _allocator(node_allocator(other._allocator))
		, _compare(other._compare)
	{
		this->_root = _copy_node(nullptr, other._root);
		this->update_first_and_last();
	}

	// Destructor
	virtual ~map() {
		clear();
	}

	// Assignment
	map& operator=(const map& other)
	{
		if (this != &other) {
			this->clear();
			this->_root = _copy_node(nullptr, other._root);
			_size = other._size;
			this->update_first_and_last();
		}
		return *this;
	}

	allocator_type get_allocator() const
	{
		return this->_allocator;
	}

	// Element access
	
	V& at(const key_type& key)
	{
		node_type* searched_node = _find(key);
		if (searched_node == nullptr)
			throw std::out_of_range("ft::map::at");
		return searched_node->data.second;
	}

	const V& at(const key_type& key) const
	{
		node_type* searched_node = this->_find(key);
		if (searched_node == nullptr)
			throw std::out_of_range("ft::map::at");
		return searched_node->data.second;
	}

	V& operator[](const key_type& key)
	{
		return insert(ft::make_pair(key, V())).first->second;
	}

	// Iterators

	iterator begin() {
		return iterator(this, this->_first);
	}
	const_iterator begin() const {
		return const_iterator(this, this->_first);
	}
	iterator end() {
		return iterator(this, nullptr);
	}
	const_iterator end() const {
		return const_iterator(this, nullptr);
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

	size_type size() const {
		return _size;
	}

	size_type max_size() const {
		return this->_allocator.max_size();
	}

	// Modifiers
	private:
	void _delete_node_rec(node_type* node) {
		if (node == nullptr)
			return;
		if (node->left)
			_delete_node_rec(node->left);
		if (node->right)
			_delete_node_rec(node->right);
		_allocator.destroy(node);
		_allocator.deallocate(node, 1);
	}

	public:
		void clear() {
			_delete_node_rec(this->_root);
			this->_root = nullptr;
			_size = 0;
			this->_first = nullptr;
			this->_last = nullptr;
		}
	private:
	void update_balance_insert(node_type* node) {
		bool is_root = node_type::is_root(node);
		if (node->balance_factor > 1 || node->balance_factor < -1) {
			// the recursion must stop after rebalancing
			// as the height of the node is unchanged
			node = this->rebalance(node);
			this->_root = is_root ? node : this->_root;
			return;
		}

		if (node->parent) {
			if (node_type::is_left_child(node))
				node->parent->balance_factor--;
			else if (node_type::is_right_child(node))
				node->parent->balance_factor++;
			if (node->parent->balance_factor != 0)
				return update_balance_insert(node->parent);
			// the recursion must stop as the height of the parent is unchanged
			return;
		}
		this->_root = node;
		return;
	}

	// insert a key-value pair
#ifdef FT_TEST
	public:
#else
	private:
#endif

	node_type* _insert(const K& key, const V& value) {
		node_type* node = _insert(this->_root, key, value);
		this->update_first_and_last();
		return node;
	}

	node_type* _insert(node_type* node, const K& k, const V& v) {
		if (!node)
		{
			_size += 1;
			node_type* new_node = _allocator.allocate(1, node);
			new (new_node) node_type(value_type(k, v), nullptr);
			this->_root = new_node;
			return new_node;
		}
		if (Compare()(k, node->data.first)) {
			if (node_type::has_left_child(node))
				return _insert(node->left, k, v);
			else {
				node_type* new_node = _allocator.allocate(1, node);
				new (new_node) node_type(value_type(k, v), node);
				node->left = new_node; 
				MY_ASSERT(node->left->parent == node);
				_size += 1;
				MY_ASSERT(node->left != 0);
				update_balance_insert(node->left);
				return new_node;
			}
		}
		else if (Compare()(node->data.first, k)) {
			if (node_type::has_right_child(node))
				return _insert(node->right, k, v);
			else {
				node_type* new_node = _allocator.allocate(1, node);
				new (new_node) node_type(value_type(k, v), node);
				node->right = new_node;
				MY_ASSERT(node->right->parent == node);
				_size += 1;
				MY_ASSERT(node->right != 0);
				update_balance_insert(node->right);
				return new_node;
			}
		}
		return node;
	}

	public:
	ft::pair<iterator, bool> insert(const value_type& value) {
		size_type size = this->size();
		node_type* node = _insert(value.first, value.second);
		this->update_first_and_last();
		if (this->size() == size + 1)
			return ft::make_pair(iterator(this, node), true);
		else
			return ft::make_pair(iterator(this, node), false);
	}

	iterator insert(iterator hint, const value_type& value) {
		if (this->_find(value.first) != nullptr)
			return end();
		node_type* ret_node = _insert(hint._node, value.first, value.second);
		this->update_first_and_last();
		return iterator(this, ret_node);
	}

	template<class InputIt>
	void insert(InputIt first, InputIt last) {
		for (; first != last; ++first)
			insert(*first);
		this->update_first_and_last();
	}

#ifdef FT_TEST
public:
#else
private:
#endif
	size_type _erase(const K& key) {
		node_type* node_to_delete = nullptr;
		begin_erase(this->_root, key, &node_to_delete);
		if (node_to_delete) { // if the key appears and a node has to be deleted
			_size -= 1;
			end_erase(node_to_delete);
			this->update_first_and_last();
			return 1;
		}
		return 0;
	}
	private:
	node_type* begin_erase(node_type* node, K key, node_type** node_to_delete) {
		if (!node)
			return nullptr;
		if (Compare()(node->data.first, key)) {
			node->right = begin_erase(node->right, key, node_to_delete);
			return node;
		}
		else if (node->data.first > key) {
			node->left = begin_erase(node->left, key, node_to_delete);
			return node;
		}
		if (!node->left && !node->right) {
			if (!node->parent)
				this->_root = nullptr;
			*node_to_delete = node;
			return node;
		}
		else if (!node->left || !node->right) {
			node_type *ret = node->left ? node->left : node->right;
			ret->parent = node->parent;
			if (!node->parent)
				this->_root = ret;
			node->parent = ret; // necessary for update_balance_delete to work correctly
			*node_to_delete = node;
			return ret;
		}
		node_type* temp = node->right;
		while (temp->left)
			temp = temp->left;
		this->swap_nodes(node, temp);
		temp->right = begin_erase(temp->right, node->data.first, node_to_delete);
		return temp;
	}
	// returns a possibly new root for the tree or nullptr
	// this function takes a node to be deleted and recursively balances its
	// ancestors
	// please note that node_to_delete, if not null, does not have any children
	void end_erase(node_type* node_to_delete) {
		if (!node_to_delete->parent)
			this->_root = nullptr;
		else
			update_balance_delete(node_to_delete);
		if (node_type::is_left_child(node_to_delete))
			node_to_delete->parent->left = nullptr;
		else if (node_type::is_right_child(node_to_delete))
			node_to_delete->parent->right = nullptr;
		delete node_to_delete;
	}
	void update_balance_delete(node_type* node) {
		if (!node)
			return;
		if (node->balance_factor > 1 || node->balance_factor < -1) {
			// one has to analyze the child on the other side of the recursion
			// to know if the height of the node changes after rebalancing
			node_type* other_child = node->balance_factor == -2 ? node->left : node->right;
			bool stop_recursion = other_child->balance_factor == 0;
			node = this->rebalance(node);
			if (!node->parent)
				this->_root = node;
			if (stop_recursion)
			{
				this->_root = node->parent ? this->_root : node;
				return ;
			}
		}
		if (node->parent) {
			if (node_type::is_left_child(node))
				node->parent->balance_factor++;
			else if (node_type::is_right_child(node))
				node->parent->balance_factor--;
			if (node->parent->balance_factor == -1 || node->parent->balance_factor == 1)
				// the recursion must stop as the height of the parent is unchanged
				return;
			return update_balance_delete(node->parent);
		}
		return;
	}
	public:
	void erase(iterator pos)
	{
		_erase(pos->first);
		/*node_type* node_to_delete = nullptr;
		begin_erase(pos._node, pos._node->data.first, &node_to_delete);
		assert(node_to_delete != nullptr);
		this->_size -= 1;
		this->end_erase(node_to_delete);
		this->update_first_and_last();*/
	}
	// TODO: wrong complexity ?
	void erase(iterator first, iterator last)
	{
		iterator next;
		while (first != last)
		{
			next = first;
			next++;
			erase(first);
			first = next;
		}
	}
	size_type erase(const key_type& key)
	{
		size_type ret = this->_erase(key);
		this->update_first_and_last();
		return ret;
	}

	void swap(map& other)
	{
		ft::swap(this->_compare, other._compare);
		ft::swap(this->_root, other._root);
		ft::swap(this->_size, other._size);
		ft::swap(this->_first, other._first);
		ft::swap(this->_last, other._last);
	}

	// Lookup
	
	size_t count(const K& key) const {
		return this->_find(key) == nullptr ? 0 : 1;
	}

	private:
	static node_type* _find(node_type* node, const K& key) {
		if (node == nullptr) {
			return nullptr;
		}
		else if (Compare()(key, node->data.first)) {
			return _find(node->left, key);
		}
		else if (Compare()(node->data.first, key)) {
			return _find(node->right, key);
		}
		else
			return node;
	}
	// find a key-value pair
#ifdef FT_TEST
	public:
#else
	private:
#endif
	node_type* _find(const K& key) const {
		return _find(this->_root, key);
	}
	public:
	iterator find(const K& key) {
		return iterator(this, this->_find(key));
	}
	const_iterator find(const K& key) const {
		return const_iterator(this, this->_find(key));
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

#ifdef FT_TEST
	static void pretty_print(node_type* node, int depth, std::ostream& os) {
		if (node == nullptr) return;
		pretty_print(node->right, depth + 1, os);
		for (int i = 0; i < depth; i++) std::cout << "  ";
		std::ostringstream oss;
		if (node->parent)
			oss << node->parent->data.first;
		else
			oss << "nullptr";
		os << node->data.first << " " << node->data.second << " (" << static_cast<int>(node->balance_factor) << ')' << " ---> " << oss.str() << "(" << node << ")" << std::endl;
		pretty_print(node->left, depth + 1, os);
	}
	// print the tree
	void pretty_print(std::ostream& os) const {
		pretty_print(this->_root, 0, os);
	}
	static bool check_order(node_type* node) {
		if (node == nullptr)
			return true;
		if (node->left != nullptr && !Compare()(node->left->data.first, node->data.first))
			return false;
		if (node->right != nullptr && !Compare()(node->data.first, node->right->data.first))
			return false;
		return check_order(node->left) && check_order(node->right);
	}
#endif

};

template<class K, class V, class Compare, class Alloc>
bool operator==(const map<K, V, Compare, Alloc>& lhs, const map<K, V, Compare, Alloc>& rhs) {
	if (lhs.size() != rhs.size())
		return false;
	typename ft::map<K, V, Compare, Alloc>::const_iterator it = lhs.begin();
	for (typename ft::map<K, V, Compare, Alloc>::const_iterator jt = rhs.begin(); jt != rhs.end(); ++it, ++jt) {
		if (*it != *jt)
			return false;
	}
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


#ifdef FT_TEST
template<class K, class V, class Compare, class Alloc>
std::ostream& operator<<(std::ostream& os, const ft::map<K, V, Compare, Alloc>& tree) {
	tree.pretty_print(os);
	return os;
}
#endif

}
#endif
