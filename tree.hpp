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

	public:
	typedef bidirectional_iterator_tag			iterator_category;
	typedef T									value_type;
	typedef ptrdiff_t							difference_type;
	typedef const T*							pointer;
	typedef const T&							reference;
	typedef ft::Node<T>							node_type;
	typedef ft::BinaryTree<T>					tree_type;

	protected:
	const tree_type*							_tree;
	node_type*									_node;


	public:
	/*operator NodeIterator<T>() {
		return NodeIterator<T>(_tree, _node);
	}*/

	//explicit NodeConstIterator(const tree_type* t, const node_type* n)
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

	NodeConstIterator operator++() {
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

	NodeConstIterator operator--() {
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

	public:
	typedef bidirectional_iterator_tag			iterator_category;
	typedef T									value_type;
	typedef ptrdiff_t							difference_type;
	typedef T*									pointer;
	typedef T&									reference;
	typedef ft::Node<T>							node_type;
	typedef ft::BinaryTree<T>					tree_type;

	protected:
	tree_type*							_tree;
	node_type*							_node;


	public:
	operator NodeConstIterator<T>() {
		return NodeConstIterator<T>(_tree, _node);
	}

	//explicit NodeIterator(tree_type* t, node_type* n)
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

	reference operator*() {
		return _node->data;
	}
	
	pointer operator->() {
		return &(operator*());
	}

	NodeIterator operator++() {
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

	NodeIterator operator--() {
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

template<class T>
class Node {

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
	//operator Node<const T>() {
		//return Node<const T>(data, left, right, parent, balance_factor);
	//}

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
		ASSERT(root->parent->right == root);
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

/*template<class K, class V, class Compare, class Alloc>
bool operator==(const ft::BinaryTree<K, V, Compare, Alloc>& lhs, const ft::BinaryTree<K, V, Compare, Alloc>& rhs) {
	if ((lhs.size() != rhs.size()))
		return false;
	if (lhs.size() == 0)
		return true;
	return *lhs.getRoot() == *rhs.getRoot();
}*/

}
