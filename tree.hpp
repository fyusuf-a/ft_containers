#include "utility.hpp"
#include "algorithm.hpp"
#include <iostream>
#include <cassert>
#include <sstream>

namespace ft {

template <class K, class V>
struct Node {
public:
	typedef char	int8_t;
	ft::pair<K, V> data;
	
	Node* left;
	Node* right;
	Node* parent;
	int8_t balance_factor;
	Node(K k, V v, Node<K, V>* my_parent = 0) : data(k, v), left(nullptr), right(nullptr), parent(my_parent), balance_factor(0) {
	}

	static void clear(Node<K, V>* node)
	{
		if (node == nullptr)
			return;
		if (node->left) {
			clear(node->left);
		}
		if (node->right) {
			clear(node->right);
		}
		delete node;
	}

	static Node<K, V>* minimum(Node<K, V>* node) {
		while (node->left)
			node = node->left;
		return node;
	}
	static bool has_left_child(const Node<K, V>* node) {
		return node->left != nullptr;
	}
	static bool has_right_child(const Node<K, V>* node) {
		return node->right != nullptr;
	}
	static bool is_left_child(const Node<K, V>* node) {
		return node->parent != nullptr && node->parent->left == node;
	}
	static bool is_right_child(const Node<K, V>* node) {
		return node->parent != nullptr && node->parent->right == node;
	}
	static bool is_root(const Node<K, V>* node) {
		return node->parent == nullptr;
	}

	static Node<K, V>* find(Node<K, V>* node, const K& key) {
		if (node == nullptr) {
			return nullptr;
		}
		else if (key < node->data.first) {
			return find(node->left, key);
		}
		else if (key > node->data.first) {
			return find(node->right, key);
		}
		else
			return node;
	}

	static Node<K, V>* next(Node<K, V>* node) {
		if (node->right != nullptr) {
			return minimum(node->right);
		}
		Node<K, V>* parent = node->parent;
		while (parent != nullptr && node == parent->right) {
			node = parent;
			parent = parent->parent;
		}
		return parent;
	}

	// returns a possibly new root for the tree or nullptr
	static Node<K, V>* update_balance_insert(Node<K, V>* node) {
		if (node->balance_factor > 1 || node->balance_factor < -1) {
			// the recursion must stop after rebalancing
			// as the height of the node is unchanged
			return rebalance(node);
		}
		if (node->parent) {
			if (is_left_child(node))
				node->parent->balance_factor--;
			else if (is_right_child(node))
				node->parent->balance_factor++;
			if (node->parent->balance_factor != 0)
				return update_balance_insert(node->parent);
			// the recursion must stop as the height of the parent is unchanged
			return nullptr;
		}
		// the parent is the root
		return node;
	}

	// returns a possibly new root for the tree or nullptr
	static Node<K, V>* update_balance_delete(Node<K, V>* node) {
		//pretty_print(node, 0, std::cout);
		Node<K, V>* newRoot = node;
		if (node->balance_factor > 1 || node->balance_factor < -1) {
			// one has to analyze the child on the other side of the recursion
			// to know if the height of the node changes after rebalancing
			Node<K, V>* other_child = node->balance_factor == -2 ? node->left : node->right;
			bool stop_recursion = other_child->balance_factor == 0;
			std::cout << "before rebalance" << std::endl;
			pretty_print(node, 0, std::cout);
			newRoot = rebalance(node);
			std::cout << "after rebalance: " << (newRoot ? "new root" : "no new root") <<  std::endl;
			pretty_print(newRoot ? newRoot : node, 0, std::cout);
			node = newRoot ? newRoot : node;
			if (stop_recursion)
				return node->parent ? nullptr : node;
		}
		if (node->parent) {
			if (is_left_child(node))
				node->parent->balance_factor++;
			else if (is_right_child(node))
				node->parent->balance_factor--;
			if (node->parent->balance_factor == -1 || node->parent->balance_factor == 1)
				// the recursion must stop as the height of the parent is unchanged
				return nullptr;
			return update_balance_delete(node->parent);
		}
		return node;
		//return newRoot ? newRoot : node;
	}

	// returns a possibly new root for the tree or nullptr
	static Node<K, V>* rotateLeft(Node<K, V>* root) {
		std::cout << "rotateLeft" << std::endl;
		Node<K, V>* newRoot = root->right;
		root->right = newRoot->left;
		if (newRoot->left)
			newRoot->left->parent = root;
		newRoot->parent = root->parent;
		int return_a_new_root = 0;
		if (is_root(root))
		{
			std::cout << "returns a new root" << std::endl;
			return_a_new_root = 1; } else if (is_left_child(root))
			root->parent->left = newRoot;
		else
			root->parent->right = newRoot;
		newRoot->left = root;
		root->parent = newRoot;
		root->balance_factor = root->balance_factor - 1 - ft::max<char>(newRoot->balance_factor, 0);
		newRoot->balance_factor = newRoot->balance_factor - 1 + ft::min<char>(root->balance_factor, 0);
		return return_a_new_root ? newRoot : nullptr;
	}

	// returns a possibly new root for the tree or nullptr
	//static Node<K, V>* rotateRight(Node<K, V>* root) {
	static Node<K, V>* rotateRight(Node<K, V>* root) {
		std::cout << "rotateRight" << std::endl;
		Node<K, V>* newRoot = root->left;
		root->left = newRoot->right;
		if (newRoot->right)
			newRoot->right->parent = root;
		newRoot->parent = root->parent;
		int return_a_new_root = 0;
		if (is_root(root))
		{
			std::cout << "returns a new root" << std::endl;
			return_a_new_root = 1;
		}
		else if (is_left_child(root))
			root->parent->left = newRoot;
		else
			root->parent->right = newRoot;
		newRoot->right = root;
		root->parent = newRoot;
		root->balance_factor = root->balance_factor + 1 - ft::min<char>(newRoot->balance_factor, 0);
		newRoot->balance_factor = newRoot->balance_factor + 1 + ft::max<char>(root->balance_factor, 0);
		assert(root->parent->right == root);
		return return_a_new_root ? newRoot : nullptr;
	}

	// returns a possibly new root for the tree or nullptr
	static Node<K, V>* rebalance(Node<K, V>* node) {
		Node<K, V>* newRoot;
		if (node->balance_factor == 2) {
			if (node->right->balance_factor == -1) {
				newRoot = rotateRight(node->right);
				node->right = newRoot ? newRoot : node->right;
			}
			return rotateLeft(node);
		}
		else if (node->balance_factor == -2) {
			if (node->left->balance_factor == 1) {
				newRoot = rotateLeft(node->left);
				node->left = newRoot ? newRoot : node->left;
			}
			return rotateRight(node);
		}
		return nullptr;
	}

	// Modifiers

	// returns a possibly new root for the tree or nullptr
	static Node<K, V>* insert(Node<K, V>* node, K k, V v, bool* is_size_incremented, bool test=false) {
		if (!node)
		{
			*is_size_incremented = 1;
			return new Node<K, V>(k, v, nullptr);
		}
		if (k < node->data.first) {
			if (has_left_child(node))
				return insert(node->left, k, v, is_size_incremented);
			else {
				node->left = new Node<K, V>(k, v, node);
				assert(node->left->parent == node);
				*is_size_incremented = 1;
				assert(node->left != 0);
				if (!test)
					return update_balance_insert(node->left);
				else
					return nullptr;
			}
		}
		else if (k > node->data.first) {
			if (has_right_child(node))
				return insert(node->right, k, v, is_size_incremented);
			else {
				node->right = new Node<K, V>(k, v, node);
				assert(node->right->parent == node);
				*is_size_incremented = 1;
				assert(node->right != 0);
				if (!test)
					return update_balance_insert(node->right);
				else
					return nullptr;
			}
		}
		else {
			node->data.second = v;
			*is_size_incremented = 0;
			return nullptr;
		}
	}

	// return value is only for recursion, changes the value of *newRoot
	static Node<K, V>* erase(Node<K, V>* node, K key, size_t* removed, Node<K, V>** newRoot) {
		if (!node)
			return nullptr;
		if (key < node->data.first) {
			node->left = erase(node->left, key, removed, newRoot);
			return node;
		}
		else if (key > node->data.first) {
			node->right = erase(node->right, key, removed, newRoot);
			return node;
		}
		else {
			*removed = 1;
			if (!node->left && !node->right) {
				std::cout << "here" << std::endl;
				if (node->parent) {
					Node<K, V> *possibly_new_root = update_balance_delete(node);
					std::cout << (possibly_new_root ? "possibly_new_root" : "no_new_root") << std::endl;
					*newRoot = possibly_new_root ? possibly_new_root : *newRoot;
				}
				else
				{
					std::cout << "here2" << std::endl;
					*newRoot = nullptr;
				}
				delete node;
				return nullptr;
			}
			else if (!node->left || !node->right) {
				Node<K, V> tmp = *node;
				Node<K, V> *possibly_new_root = update_balance_delete(node);
				*newRoot = possibly_new_root ? possibly_new_root : *newRoot;
				delete node;
				Node<K, V> *ret = tmp.left ? tmp.left : tmp.right;
				ret->parent = tmp.parent;
				*newRoot = ret->parent ? *newRoot : ret;
				return ret;
			}
			Node<K, V>* temp = node->right;
			while (temp->left)
				temp = temp->left;
			node->data = temp->data;
			node->right = erase(node->right, temp->data.first, removed, newRoot);
			return node;
		}
	}

	static void pretty_print(Node<K, V>* node, int depth, std::ostream& os) {
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

};

template <class K, class V>
class BinaryTree {
public:
	typedef size_t	size_type;

private:
	Node<K, V>* _root;
	int _size;

public:
	size_type size() const {
		return _size;
	}
	
	Node<K, V>* getRoot() const {
		return _root;
	}

private:

public:
	void print(Node<K, V>* n, std::ostream& s) const {
		s << "(" << n->data.first << " at " << n << ") left: " << n->left << " right: " << n->right << " parent: " << n->parent << " (balance factor: " << (int)n->balance_factor << ")" << std::endl;
	}
public:
	// constructor
	BinaryTree(): _root(nullptr), _size(0) {}

	// destructor
	~BinaryTree() {
		delete _root;
	}

	void clear() {
		Node<K, V>::clear(_root);
		_root = nullptr;
		_size = 0;
	}

	bool erase(const K& key) {
		// as the newRoot is detected through a change to a Node pointer which
		// is null at the beginning, we have to treat the special case where
		// the root is deleted and the tree becomes empty
		if (_root && _root->data.first == key && !_root->left && !_root->right) {
			delete _root;
			_root = nullptr;
			_size--;
			return true;
		}
		size_type removed = 0;
		Node<K, V>* newRoot = nullptr;
		Node<K, V>::erase(_root, key, &removed, &newRoot);
		_root = newRoot ? newRoot : _root;
		_size -= static_cast<size_type>(removed);
		return removed;
	}

	// insert a key-value pair
	bool insert(const K& key, const V& value, bool test = false) {
		bool size_incremented = 0;
		Node<K, V>* newRoot;
		newRoot = Node<K, V>::insert(_root, key, value, &size_incremented, test);
		if (newRoot)
			_root = newRoot;
		_size += static_cast<size_type>(size_incremented);
		return size_incremented;
	}

	// find a key-value pair
	Node<K, V>* find(const K& key) const {
		return Node<K, V>::find(_root, key);
	}

	// print the tree
	void print(std::ostream& os) const {
		print(_root, os);
	}
	// print the tree
	void pretty_print(std::ostream& os) const {
		Node<K, V>::pretty_print(_root, 0, os);
	}
};

}

template<class K, class V>
std::ostream& operator<<(std::ostream& os, const ft::BinaryTree<K, V>& tree) {
	tree.pretty_print(os);
	return os;
}

