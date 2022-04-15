#include "utility.hpp"
#include "algorithm.hpp"
#include <iostream>
#include <cassert>
#include <sstream>
#include <stdexcept>

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
		bool is_root = Node<K,V>::is_root(node);
		if (node->balance_factor > 1 || node->balance_factor < -1) {
			// the recursion must stop after rebalancing
			// as the height of the node is unchanged
			node = rebalance(node);
			return is_root ? node : nullptr;
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
		return node;
	}

	// returns a possibly new root for the tree or nullptr
	static Node<K, V>* update_balance_delete(Node<K, V>* node) {
		if (!node)
			return nullptr;
		Node<K, V>* newRoot = node;
		if (node->balance_factor > 1 || node->balance_factor < -1) {
			// one has to analyze the child on the other side of the recursion
			// to know if the height of the node changes after rebalancing
			bool other_child_is_left = node->balance_factor == -2;
			Node<K, V>* other_child = node->balance_factor == -2 ? node->left : node->right;
			bool stop_recursion = other_child->balance_factor == 0;
			newRoot = rebalance(node);
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

	// returns a new root for the tree
	static Node<K, V>* rotateLeft(Node<K, V>* root) {
		Node<K, V>* newRoot = root->right;
		root->right = newRoot->left;
		if (newRoot->left)
			newRoot->left->parent = root;
		newRoot->parent = root->parent;
		int return_a_new_root = 0;
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
	static Node<K, V>* rotateRight(Node<K, V>* root) {
		Node<K, V>* newRoot = root->left;
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
		assert(root->parent->right == root);
		return newRoot;
	}

	// returns a new root for the tree, this function should only be called if
	// the node's balance factor is 2 or -2
	static Node<K, V>* rebalance(Node<K, V>* node) {
		Node<K, V>* newRoot;
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

	// Modifiers

	// returns a possibly new root for the tree or nullptr
	static Node<K, V>* insert(Node<K, V>* node, K k, V v, bool* is_size_incremented) {
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
				return update_balance_insert(node->left);
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
				return update_balance_insert(node->right);
			}
		}
		else {
			node->data.second = v;
			*is_size_incremented = 0;
			return nullptr;
		}
	}

	// returns a possibly new root for the tree or nullptr
	// this function takes a node to be deleted and recursively balances its
	// ancestors
	// please note that node_to_delete, if not null, does not have any children
	static void end_erase(Node<K, V>* node_to_delete, bool* has_new_root, Node<K, V>** new_root) {
		if (!node_to_delete->parent) {
			*new_root = nullptr;
			*has_new_root = true;
		}
		else {
			Node<K, V>* possibly_new_root = update_balance_delete(node_to_delete);
			*has_new_root = possibly_new_root;
			*new_root = possibly_new_root ? possibly_new_root : *new_root;
		}
		if (is_left_child(node_to_delete))
			node_to_delete->parent->left = nullptr;
		else if (is_right_child(node_to_delete))
			node_to_delete->parent->right = nullptr;
		delete node_to_delete;
	}
	

	// return value is only for recursion, changes the value of *newRoot, this
	// function is not to be called in the case where there is only one node in the tree
	static Node<K, V>* begin_erase(Node<K, V>* node, K key, Node<K, V>** node_to_delete, bool* has_new_root, Node<K,V>** new_root) {
		if (!node)
			return nullptr;
		if (node->data.first < key) {
			node->right = begin_erase(node->right, key, node_to_delete, has_new_root, new_root);
			return node;
		}
		else if (node->data.first > key) {
			node->left = begin_erase(node->left, key, node_to_delete, has_new_root, new_root);
			return node;
		}
		if (!node->left && !node->right) {
			if (!node->parent) {
				*has_new_root = true;
				*new_root = nullptr;
			}
			*node_to_delete = node;
			return node;
		}
		else if (!node->left || !node->right) {
			Node<K, V> *ret = node->left ? node->left : node->right;
			ret->parent = node->parent;
			node->parent = ret;
			*node_to_delete = node;
			if (!node->parent) {
				*has_new_root = true;
				*new_root = ret;
			}
			return ret;
		}
		Node<K, V>* temp = node->right;
		while (temp->left)
			temp = temp->left;
		node->data = temp->data;
		node->right = begin_erase(node->right, temp->data.first, node_to_delete, has_new_root, new_root);
		return node;
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
		/*if (_root && _root->data.first == key && !_root->left && !_root->right) {
			delete _root;
			_root = nullptr;
			_size--;
			return true;
		}*/
		Node<K, V>* node_to_delete = nullptr;
		Node<K, V>* possibly_new_root = nullptr;
		bool has_new_root = false;
		Node<K, V>::begin_erase(_root, key, &node_to_delete, &has_new_root, &possibly_new_root);
		_root = has_new_root ? possibly_new_root : _root;
		bool ret = false;
		if (node_to_delete) { // if the key appears and a node has to be deleted
			_size -= 1;
			possibly_new_root = nullptr;
			has_new_root = false;
			Node<K, V>::end_erase(node_to_delete, &has_new_root, &possibly_new_root);
			if (possibly_new_root)
				_root = possibly_new_root;
		}
		//_root = newRoot ? newRoot : _root;
		return ret;
	}

	// insert a key-value pair
	bool insert(const K& key, const V& value) {
		bool size_incremented = 0;
		Node<K, V>* newRoot;
		newRoot = Node<K, V>::insert(_root, key, value, &size_incremented);
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

