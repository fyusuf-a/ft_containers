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
#define MY_ASSERT(x) assert(x)
#else
#define MY_ASSERT(x)
#endif

namespace ft {

template<class T>
class BinaryTree;

template<class T>
class Node;

template<class T>
class NodeIterator;

template<class T>
class BinaryTree;

template<class T>
class NodeConstIterator {
	template<class T2>
	friend class Node;
	template<class K, class V, class Compare, class Alloc>
	friend class map;
	friend class BinaryTree<T>;

	public:
	typedef bidirectional_iterator_tag			iterator_category;
	typedef T									value_type;
	typedef ptrdiff_t							difference_type;
	typedef const T*							pointer;
	typedef const T&							reference;
	typedef ft::Node<T>							node_type;
	typedef ft::BinaryTree<T>					tree_type;

#ifdef FT_TEST
	public:
#else
	protected:
#endif
	const tree_type*							_tree;
	node_type*									_node;


	public:
	NodeConstIterator() : _tree(nullptr), _node(nullptr) {}

	NodeConstIterator(const tree_type* t, node_type* n)
		: _tree(t)
		, _node(n)
	{}

	template<class T2>
	NodeConstIterator(const NodeConstIterator<T2>& other)
		: _tree(other._tree)
		, _node(other._node)
	{}

	template<class T2>
	NodeConstIterator& operator=(const NodeConstIterator<T2>& other) {
		if (this != &other) {
			_tree = other._tree;
			_node = other._node;
		}
		return *this;
	}

	reference operator*() const {
		return _node->data;
	}
	
	pointer operator->() const {
		return &(operator*());
	}

	NodeConstIterator& operator++() {
		if (!_node) {
			_node = _tree->_first;
			return *this;
		}
		if (_node == _tree->_last) {
			_node = nullptr;
			return *this;
		}
		_node = _node->next(_node);
		return *this;
	}

	NodeConstIterator& operator--() {
		if (!_node) {
			_node = _tree->_last;
			return *this;
		}
		if (_node == _tree->_first) {
			_node = nullptr;
			return *this;
		}
		_node = _node->prev(_node);
		return *this;
	}

	NodeConstIterator operator++(int) {
		NodeConstIterator tmp = *this;
		operator++();
		return tmp;
	}

	NodeConstIterator operator--(int) {
		NodeConstIterator tmp = *this;
		operator--();
		return tmp;
	}

	friend bool operator==(const NodeConstIterator<T>& lhs, const NodeConstIterator<T>& rhs) {
		return lhs._node == rhs._node;
	}

	friend bool operator!=(const NodeConstIterator<T>& lhs, const NodeConstIterator<T>& rhs) {
		return !(lhs == rhs);
	}
};

template<class T>
class NodeIterator {
	template<class T2>
	friend class Node;
	template<class K, class V, class Compare, class Alloc>
	friend class map;
	friend class BinaryTree<T>;

	public:
	typedef bidirectional_iterator_tag			iterator_category;
	typedef T									value_type;
	typedef ptrdiff_t							difference_type;
	typedef T*									pointer;
	typedef T&									reference;
	typedef ft::Node<T>							node_type;
	typedef ft::BinaryTree<T>					tree_type;

#ifdef FT_TEST
	public:
#else
	protected:
#endif
	tree_type*							_tree;
	node_type*							_node;


	public:
	operator NodeConstIterator<T>() {
		return NodeConstIterator<T>(_tree, _node);
	}

	NodeIterator() : _tree(nullptr), _node(nullptr) {}

	NodeIterator(tree_type* t, node_type* n)
		: _tree(t)
		, _node(n)
	{}

	template<class T2>
	NodeIterator(const NodeIterator<T2>& other)
		: _tree(other._tree)
		, _node(other._node)
	{}

	template<class T2>
	NodeIterator& operator=(const NodeIterator<T2>& other) {
		if (this != &other) {
			_tree = other._tree;
			_node = other._node;
		}
		return *this;
	}

	reference operator*() const {
		return _node->data;
	}
	
	pointer operator->() const {
		return &(operator*());
	}

	NodeIterator& operator++() {
		if (!_node) {
			_node = _tree->_first;
			return *this;
		}
		if (_node == _tree->_last) {
			_node = nullptr;
			return *this;
		}
		_node = _node->next(_node);
		return *this;
	}

	NodeIterator& operator--() {
		if (!_node) {
			_node = _tree->_last;
			return *this;
		}
		if (_node == _tree->_first) {
			_node = nullptr;
			return *this;
		}
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

	friend bool operator==(const NodeIterator<T>& lhs, const NodeIterator<T>& rhs) {
		return lhs._node == rhs._node;
	}

	friend bool operator!=(const NodeIterator<T>& lhs, const NodeIterator<T>& rhs) {
		return !(lhs == rhs);
	}
};

template<class K, class V, class Compare, class Alloc>
class map;

template<class T>
class Node {
	friend class BinaryTree<T>;
	template<class K, class V, class Compare, class Alloc>
	friend class ft::map;
	friend class NodeIterator<T>;
	friend class NodeConstIterator<T>;


private:
	typedef char	int8_t;
	
#ifdef FT_TEST
public:
#else
protected:
#endif
	T data;
	Node* left;
	Node* right;
	Node* parent;
	int8_t balance_factor;

public:
	Node(T t, Node* my_parent = 0, Node* my_left = 0, Node* my_right = 0, int8_t my_balance_factor = 0)
		: data(t)
		, left(my_left)
		, right(my_right)
		, parent(my_parent)
		, balance_factor(my_balance_factor)
	{}

	static Node* minimum(Node* node) {
		if (!node)
			return nullptr;
		while (node->left)
			node = node->left;
		return node;
	}

	static Node* maximum(Node* node) {
		if (!node)
			return nullptr;
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
			MY_ASSERT(node->left->parent == node);
		if (node->right)
			MY_ASSERT(node->right->parent == node);
		return check_parent(node->left) && check_parent(node->right);
	}

	static bool check_balance(Node* node) {
		if (node == nullptr)
			return true;
		MY_ASSERT(static_cast<char>(static_cast<int>(length(node->right)) - static_cast<int>(length(node->left))) == node->balance_factor);
		if (!(node->balance_factor == 0 || node->balance_factor == 1 || node->balance_factor == -1))
			return false;
		return check_balance(node->left) && check_balance(node->right);
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

template <class T>
class BinaryTree {
	friend class NodeConstIterator<T>;
	friend class NodeIterator<T>;

	typedef Node<T>			node_type;
	
	protected:
	node_type*				_root;
	node_type*				_first;
	node_type*				_last;
	

	static void _swap_neighbors(node_type* n1, node_type* n2) {
		node_type* n1_parent = n1->parent;
		bool n1_parent_is_left = n1_parent ? n1_parent->left == n1 : false;
		node_type* n2_parent = n2->parent;
		bool n2_parent_is_left = n2->parent ? n2_parent->left == n2 : false;
		node_type* n1_left = n1->left;
		node_type* n1_right = n1->right;
		node_type* n2_left = n2->left;
		node_type* n2_right = n2->right;
		if (n1_parent) {
			if (n1_parent_is_left)
				n1_parent->left = n2;
			else
				n1_parent->right = n2;
		}
		if (n2_parent) {
			if (n2_parent_is_left)
				n2_parent->left = n1;
			else
				n2_parent->right = n1;
		}
		if (n1_left)
			n1_left->parent = n2;
		if (n1_right)
			n1_right->parent = n2;
		if (n2_left)
			n2_left->parent = n1;
		if (n2_right)
			n2_right->parent = n1;
	}

	public:
	// changes the position of two not-null nodes
	void swap_nodes(node_type* n1, node_type* n2) {
		if (node_type::is_root(n1) || node_type::is_root(n2))
			_root = node_type::is_root(n1) ? n2 : n1;
		_swap_neighbors(n1, n2);
		ft::swap(n1->parent, n2->parent);
		ft::swap(n1->left, n2->left);
		ft::swap(n1->right, n2->right);
		ft::swap(n1->balance_factor, n2->balance_factor);
	}

	
	// returns a new root for the tree
	static node_type* rotateLeft(node_type* root) {
		node_type* newRoot = root->right;
		root->right = newRoot->left;
		if (newRoot->left)
			newRoot->left->parent = root;
		newRoot->parent = root->parent;
		if (node_type::is_left_child(root))
			root->parent->left = newRoot;
		else if (node_type::is_right_child(root))
			root->parent->right = newRoot;
		newRoot->left = root;
		root->parent = newRoot;
		root->balance_factor = root->balance_factor - 1 - ft::max<char>(newRoot->balance_factor, 0);
		newRoot->balance_factor = newRoot->balance_factor - 1 + ft::min<char>(root->balance_factor, 0);
		return newRoot;
	}
	// returns a new root for the tree
	static node_type* rotateRight(node_type* root) {
		node_type* newRoot = root->left;
		root->left = newRoot->right;
		if (newRoot->right)
			newRoot->right->parent = root;
		newRoot->parent = root->parent;
		if (node_type::is_left_child(root))
			root->parent->left = newRoot;
		else if (node_type::is_right_child(root))
			root->parent->right = newRoot;
		newRoot->right = root;
		root->parent = newRoot;
		root->balance_factor = root->balance_factor + 1 - ft::min<char>(newRoot->balance_factor, 0);
		newRoot->balance_factor = newRoot->balance_factor + 1 + ft::max<char>(root->balance_factor, 0);
		MY_ASSERT(root->parent->right == root);
		return newRoot;
	}
	// returns a new root for the tree, this function should only be called if
	// the node's balance factor is 2 or -2
	static node_type* rebalance(node_type* node) {
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
	void update_first_and_last() {
		_first = node_type::minimum(_root);
		_last = node_type::maximum(_root);
	}

#ifdef FT_TEST
	public:
#else
	protected:
#endif
	node_type* getRoot() const {
		return _root;
	}
	// constructor
	BinaryTree()
		: _root(nullptr)
		, _first(nullptr)
		, _last(nullptr)
	{}
	// destructor
	virtual ~BinaryTree() {}
};
}
