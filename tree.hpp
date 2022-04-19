#include "iterator.hpp"
#include "utility.hpp"
#include "algorithm.hpp"
#include <iostream>
#include <cassert>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#define nullptr 0
#ifdef FT_TEST
#define ASSERT(x) assert(x)
#else
#define ASSERT(x)
#endif

namespace ft {

template<class K, class V, class Compare, class Alloc>
class BinaryTree;

template<class K, class V, class Compare, class Alloc>
class NodeIterator {
	friend class BinaryTree<K, V, Compare, Alloc>;

	public:
	typedef bidirectional_iterator_tag						iterator_category;
	typedef ft::pair<const K, V>							value_type;
	typedef ptrdiff_t										difference_type;
	typedef ft::pair<const K, V>*							pointer;
	typedef ft::pair<const K, V>&							reference;
	typedef typename BinaryTree<K, V, Compare, Alloc>::Node	node_type;
	//typedef node_type*										iterator;

	protected:
	node_type*							_node;

	public:
	NodeIterator() : _node(nullptr) {}

	explicit NodeIterator(node_type* n) : _node(n) {}

	template<class K2, class V2, class Compare2>
	NodeIterator(const NodeIterator<K2, V2, Compare2, Alloc>& other) : _node(other.node) {}

	template<class K2, class V2, class Compare2>
	NodeIterator& operator=(const NodeIterator<K2, V2, Compare2, Alloc>& other) {
		_node = other.node;
		return *this;
	}

	reference operator*() const {
		//return ft::pair<const K, V>(_node->data.first, _node->data.second);
		//return static_cast<std::pair<const K, V> >(_node->data);
		return _node->data;
	}
	
	pointer operator->() const {
		return &(operator*());
	}

	NodeIterator& operator++() {
		_node = _node->next(_node);
		return *this;
	}

	NodeIterator& operator--() {
		_node = _node->prev(_node);
		return *this;
	}

	NodeIterator operator++(int) {
		NodeIterator tmp = *this;
		operator++();
		return tmp;
	}

	NodeIterator operator--(int) {
		NodeIterator tmp = *this;
		operator--();
		return tmp;
	}

	friend bool operator==(const NodeIterator& lhs, const NodeIterator& rhs) {
		return lhs._node == rhs._node;
	}

	friend bool operator!=(const NodeIterator& lhs, const NodeIterator& rhs) {
		return !(lhs == rhs);
	}
};

template<class K, class V, class Compare, class Alloc>
class map;

template <class K, class V, class Compare = std::less<K>, class Alloc = std::allocator<ft::pair<K, V> > >
class BinaryTree {
	friend class NodeIterator<K, V, Compare, Alloc>;
	friend class map<K, V, Compare, Alloc>;

	class Node {
		friend class NodeIterator<K, V, Compare, Alloc>;
		friend class BinaryTree;
		friend class map<K, V, Compare, Alloc>;


	private:
		typedef char	int8_t;
		
#ifdef FT_TEST
	public:
#else
	protected:
#endif
		ft::pair<const K, V> data;
		Node* left;
		Node* right;
		Node* parent;
		int8_t balance_factor;

	public:

		Node(K k, V v, Node* my_parent = 0, Node* my_left = 0, Node* my_right = 0, int8_t my_balance_factor = 0)
			: data(k, v)
			, left(my_left)
			, right(my_right)
			, parent(my_parent)
			, balance_factor(my_balance_factor)
		{}

		static Node* minimum(Node* node) {
			while (node->left)
				node = node->left;
			return node;
		}

		static Node* maximum(Node* node) {
			while (node->right)
				node = node->right;
			return node;
		}

		static bool has_left_child(const Node* node) {
			return node->left != nullptr;
		}
		static bool has_right_child(const Node* node) {
			return node->right != nullptr;
		}
		static bool is_left_child(const Node* node) {
			return node->parent != nullptr && node->parent->left == node; }
		static bool is_right_child(const Node* node) {
			return node->parent != nullptr && node->parent->right == node;
		}
		static bool is_root(const Node* node) {
			return node->parent == nullptr;
		}

		static Node* find(Node* node, const K& key) {
			if (node == nullptr) {
				return nullptr;
			}
			else if (Compare()(key, node->data.first)) {
				return find(node->left, key);
			}
			else if (Compare()(node->data.first, key)) {
				return find(node->right, key);
			}
			else
				return node;
		}

		static Node* next(Node* node) {
			if (node->right != nullptr) {
				return minimum(node->right);
			}
			Node* parent = node->parent;
			while (parent != nullptr && node == parent->right) {
				node = parent;
				parent = parent->parent;
			}
			return parent;
		}

		static Node* prev(Node* node) {
			if (node->left != nullptr) {
				return maximum(node->left);
			}
			Node* parent = node->parent;
			while (parent != nullptr && node == parent->left) {
				node = parent;
				parent = parent->parent;
			}
			return parent;
		}

		// returns a new root for the tree
		static Node* rotateLeft(Node* root) {
			Node* newRoot = root->right;
			root->right = newRoot->left;
			if (newRoot->left)
				newRoot->left->parent = root;
			newRoot->parent = root->parent;
			if (is_left_child(root))
				root->parent->left = newRoot;
			else if (is_right_child(root))
				root->parent->right = newRoot;
			newRoot->left = root;
			root->parent = newRoot;
			root->balance_factor = root->balance_factor - 1 - ft::max<char>(newRoot->balance_factor, 0);
			newRoot->balance_factor = newRoot->balance_factor - 1 + ft::min<char>(root->balance_factor, 0);
			return newRoot;
		}

		// returns a new root for the tree
		static Node* rotateRight(Node* root) {
			Node* newRoot = root->left;
			root->left = newRoot->right;
			if (newRoot->right)
				newRoot->right->parent = root;
			newRoot->parent = root->parent;
			if (is_left_child(root))
				root->parent->left = newRoot;
			else if (is_right_child(root))
				root->parent->right = newRoot;
			newRoot->right = root;
			root->parent = newRoot;
			root->balance_factor = root->balance_factor + 1 - ft::min<char>(newRoot->balance_factor, 0);
			newRoot->balance_factor = newRoot->balance_factor + 1 + ft::max<char>(root->balance_factor, 0);
			ASSERT(root->parent->right == root);
			return newRoot;
		}

		// returns a new root for the tree, this function should only be called if
		// the node's balance factor is 2 or -2
		static Node* rebalance(Node* node) {
			if (node->balance_factor == 2) {
				if (node->right->balance_factor == -1)
					rotateRight(node->right);
				return rotateLeft(node);
			}
			else if (node->balance_factor == -2) {
				if (node->left->balance_factor == 1)
					rotateLeft(node->left);
				return rotateRight(node);
			}
			throw std::runtime_error("rebalance: node has wrong balance factor");
		}

		static void pretty_print(Node* node, int depth, std::ostream& os) {
			if (node == nullptr) return;
			pretty_print(node->right, depth + 1, os);
			for (int i = 0; i < depth; i++) std::cout << "  ";
			std::ostringstream oss;
			if (node->parent)
				oss << node->parent->data.first;
			else
				oss << "nullptr";
			os << node->data.first << " " << node->data.second << " (" << static_cast<int>(node->balance_factor) << ')' << " ---> " << oss.str() << std::endl;
			pretty_print(node->left, depth + 1, os);
		}

#ifdef FT_TEST
		public:
		static size_t length(Node* node) {
			if (node == nullptr)
				return 0;
			return 1 + std::max(length(node->left) , length(node->right));
		}

		static bool check_parent(Node* node) {
			if (node == nullptr)
				return true;
			if (node->left)
				ASSERT(node->left->parent == node);
			if (node->right)
				ASSERT(node->right->parent == node);
			return check_parent(node->left) && check_parent(node->right);
		}

		static bool check_balance(Node* node) {
			if (node == nullptr)
				return true;
			ASSERT(static_cast<char>(static_cast<int>(length(node->right)) - static_cast<int>(length(node->left))) == node->balance_factor);
			if (!(node->balance_factor == 0 || node->balance_factor == 1 || node->balance_factor == -1))
				return false;
			return check_balance(node->left) && check_balance(node->right);
		}

		static bool check_order(Node* node) {
			if (node == nullptr)
				return true;
			//if (node->left != nullptr && node->left->data.first >= node->data.first)
			if (node->left != nullptr && !Compare()(node->left->data.first, node->data.first))
				return false;
			if (node->right != nullptr && !Compare()(node->data.first, node->right->data.first))
				return false;
			return check_order(node->left) && check_order(node->right);
		}

		friend bool operator==(const Node& lhs, const Node& rhs) {
			if ((lhs.left == nullptr) == !(rhs.left == nullptr))
				return false;
			if ((lhs.right == nullptr) == !(rhs.right == nullptr))
				return false;
			if (!lhs.left) {
				if (!lhs.right)
					return true;
				return *lhs.right == *rhs.right;
			}
			else {
				if (!lhs.right)
					return *lhs.left == *rhs.left;
				return *lhs.left == *rhs.left && *lhs.right == *rhs.right;
			}
		}
#endif
	};

	public:
		typedef std::size_t											size_type;
		typedef Node												node_type;
	
	private:
		typedef typename Alloc::template rebind<node_type>::other	node_allocator;
		typedef NodeIterator<K, V, Compare, Alloc>					iterator;

	protected:
		node_type*				_root;
		size_type				_size;
		node_allocator			_allocator;
		Compare					_compare;

	public:
		// attention, does not update balance_factor
		node_type* update_data(node_type* node, const K& k, const V& v) {
			node_type* new_node = _allocator.allocate(1, node);
			new (new_node) node_type(k, v, node->parent, node->left, node->right, node->balance_factor);
			ASSERT(new_node->data.first == k);
			ASSERT(new_node->data.second == v);
			ASSERT(new_node->parent == node->parent);
			ASSERT(new_node->left == node->left);
			ASSERT(new_node->right == node->right);
			if (node_type::is_left_child(node))
				node->parent->left = new_node;
			else if (node_type::is_right_child(node))
				node->parent->right = new_node;
			else
				_root = new_node;
			if (node->left)
				node->left->parent = new_node;
			if (node->right)
				node->right->parent = new_node;
			_allocator.destroy(node);
			_allocator.deallocate(node, 1);
			return new_node;
		}

		protected:

		node_type* _insert(node_type* node, const K& k, const V& v) {
			if (!node)
			{
				_size += 1;
				node_type* new_node = _allocator.allocate(1, node);
				new (new_node) node_type(k, v, nullptr);
				_root = new_node;
				return new_node;
			}
			if (Compare()(k, node->data.first)) {
				if (node_type::has_left_child(node))
					return _insert(node->left, k, v);
				else {
					node_type* new_node = _allocator.allocate(1, node);
					new (new_node) node_type(k, v, node);
					node->left = new_node; 
					ASSERT(node->left->parent == node);
					_size += 1;
					ASSERT(node->left != 0);
					update_balance_insert(node->left);
					return new_node;
				}
			}
			else if (Compare()(node->data.first, k)) {
				if (node_type::has_right_child(node))
					return _insert(node->right, k, v);
				else {
					node_type* new_node = _allocator.allocate(1, node);
					new (new_node) node_type(k, v, node);
					node->right = new_node;
					ASSERT(node->right->parent == node);
					_size += 1;
					ASSERT(node->right != 0);
					update_balance_insert(node->right);
					return new_node;
				}
			}
			return node;
		}

		void update_balance_insert(node_type* node) {
			bool is_root = node_type::is_root(node);
			if (node->balance_factor > 1 || node->balance_factor < -1) {
				// the recursion must stop after rebalancing
				// as the height of the node is unchanged
				node = node_type::rebalance(node);
				_root = is_root ? node : _root;
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
			_root = node;
			return;
		}

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
					_root = nullptr;
				*node_to_delete = node;
				return node;
			}
			else if (!node->left || !node->right) {
				node_type *ret = node->left ? node->left : node->right;
				ret->parent = node->parent;
				if (!node->parent)
					_root = ret;
				node->parent = ret; // necessary for update_balance_delete to work correctly
				*node_to_delete = node;
				return ret;
			}
			node_type* temp = node->right;
			while (temp->left)
				temp = temp->left;
			node = update_data(node, temp->data.first, temp->data.second);
			node->right = begin_erase(node->right, temp->data.first, node_to_delete);
			return node;
		}

		// returns a possibly new root for the tree or nullptr
		// this function takes a node to be deleted and recursively balances its
		// ancestors
		// please note that node_to_delete, if not null, does not have any children
		void end_erase(node_type* node_to_delete) {
			if (!node_to_delete->parent)
				_root = nullptr;
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
				node = node_type::rebalance(node);
				if (!node->parent)
					_root = node;
				if (stop_recursion)
				{
					_root = node->parent ? _root : node;
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
		size_type size() const {
			return _size;
		}
		
		node_type* getRoot() const {
			return _root;
		}

	public:
		void print(node_type n, std::ostream& s) const {
			s << "(" << n->data.first << " at " << n << ") left: " << n->left << " right: " << n->right << " parent: " << n->parent << " (balance factor: " << (int)n->balance_factor << ")" << std::endl;
		}
	public:
		// constructor
		BinaryTree():
			_root(nullptr),
			_size(0),
			_allocator(node_allocator()),
			_compare(Compare())
		{}

		explicit BinaryTree(const Compare& comp, const Alloc& alloc = Alloc()):
			_root(nullptr),
			_size(0),
			_allocator(node_allocator(alloc)),
			_compare(comp)
		{}

		BinaryTree(const BinaryTree& other):
			_root(nullptr),
			_size(other._size),
			_allocator(node_allocator(other._allocator)),
			_compare(other._compare)
		{
			_root = copy_node(nullptr, other._root);
		}

		BinaryTree& operator=(const BinaryTree& other) {
			if (this != &other) {
				clear();
				_size = other._size;
				_root = copy_node(nullptr, other._root);
			}
			return *this;
		}

		node_type* copy_node(node_type* current, node_type* other) {
			if (!other)
				return nullptr;
			node_type* new_node = _allocator.allocate(1, current);
			new (new_node) node_type(other->data.first, other->data.second, current, 0, 0, other->balance_factor);
			new_node->left = copy_node(new_node, other->left);
			new_node->right = copy_node(new_node, other->right);
			return new_node;
		}

	public:
		// destructor
		virtual ~BinaryTree() {
			clear();
		}

		void delete_node_rec(node_type* node) {
			if (node == nullptr)
				return;
			if (node->left)
				delete_node_rec(node->left);
			if (node->right)
				delete_node_rec(node->right);
			_allocator.destroy(node);
			_allocator.deallocate(node, 1);
		}

	public:
		void clear() {
			delete_node_rec(_root);
			_root = nullptr;
			_size = 0;
		}

#ifdef FT_TEST
	public:
#else
	protected:
#endif
		size_type erase(const K& key) {
			node_type* node_to_delete = nullptr;
			begin_erase(_root, key, &node_to_delete);
			if (node_to_delete) { // if the key appears and a node has to be deleted
				_size -= 1;
				end_erase(node_to_delete);
				return 1;
			}
			return 0;
		}

		// insert a key-value pair
		node_type* _insert(const K& key, const V& value) {
			node_type* node = _insert(_root, key, value);
			return node;
		}

		// find a key-value pair
		node_type* _find(const K& key) const {
			return node_type::find(_root, key);
		}

#ifdef FT_TEST
	public:
		// print the tree
		void print(std::ostream& os) const {
			print(_root, os);
		}
		// print the tree
		void pretty_print(std::ostream& os) const {
			node_type::pretty_print(_root, 0, os);
		}
#endif

};

template<class K, class V, class Compare, class Alloc>
std::ostream& operator<<(std::ostream& os, const ft::BinaryTree<K, V, Compare, Alloc>& tree) {
	tree.pretty_print(os);
	return os;
}

template<class K, class V, class Compare, class Alloc>
bool operator==(const ft::BinaryTree<K, V, Compare, Alloc>& lhs, const ft::BinaryTree<K, V, Compare, Alloc>& rhs) {
	if ((lhs.size() != rhs.size()))
		return false;
	if (lhs.size() == 0)
		return true;
	return *lhs.getRoot() == *rhs.getRoot();
}

}
