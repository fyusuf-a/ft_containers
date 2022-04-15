#include <iostream>
#include <vector>
#include <tuple>
#include "tree.hpp"
#define TEST_SIZE 100
#define MAX_VALUE 100
#include <random>

using namespace std;
typedef int K;
typedef string V;

size_t length(ft::Node<K, V>* node) {
	if (node == nullptr)
		return 0;
	return 1 + std::max(length(node->left) , length(node->right));
}

bool check_parent(ft::Node<K, V>* node) {
	if (node == nullptr)
		return true;
	if (node->left)
		assert(node->left->parent == node);
	if (node->right)
		assert(node->right->parent == node);
	return check_parent(node->left) && check_parent(node->right);
}

bool check_balance(ft::Node<K, V>* node) {
	if (node == nullptr)
		return true;
	assert(static_cast<char>(static_cast<int>(length(node->right)) - static_cast<int>(length(node->left))) == node->balance_factor);
	if (!(node->balance_factor == 0 || node->balance_factor == 1 || node->balance_factor == -1))
		return false;
	return check_balance(node->left) && check_balance(node->right);
}

bool check_order(ft::Node<K, V>* node) {
	if (node == nullptr)
		return true;
	if (node->left != nullptr && node->left->data.first >= node->data.first)
		return false;
	if (node->right != nullptr && node->right->data.first <= node->data.first)
		return false;
	return check_order(node->left) && check_order(node->right);
}

ft::BinaryTree<K, V>::size_type size(ft::Node<K, V>* node) {
	if (node == nullptr)
		return 0;
	return 1 + size(node->left) + size(node->right);
}

void check_size(const ft::BinaryTree<K, V>& tree) {
	assert(tree.size() == size(tree.getRoot()));
}

void check_properties(const ft::BinaryTree<K, V>& tree) {
	check_parent(tree.getRoot());
	check_balance(tree.getRoot());
	check_order(tree.getRoot());
	check_size(tree);
}

void wrapper_insert(ft::BinaryTree<K, V>& tree, K key, V value) {
	ft::BinaryTree<K, V>::size_type size_before = tree.size();
	tree.insert(key, value);
	check_properties(tree);
	ft::Node<K, V>* maybe_found_node = tree.find(key);
	if (tree.size() != size_before)
		assert(tree.size() == size_before + 1);
	else
		assert(tree.find(key)->data.second == value);
}

void wrapper_erase(ft::BinaryTree<K, V>& tree, K key) {
	ft::BinaryTree<K, V>::size_type size_before = tree.size();
	ft::Node<K, V>* node_present = tree.find(key);
	tree.erase(key);
	check_properties(tree);
	if (node_present)
		assert(tree.size() == size_before - 1);
	else
		assert(tree.size() == size_before);
}

int main() {
	srand(time(NULL));
	ft::BinaryTree<K, V> tree;

	for (size_t k = 0; k < 500; k++) {
		tree.clear();
		for (size_t i = 0; i < TEST_SIZE; i++) {
			K key = rand() % MAX_VALUE;
			V value = "";
			std::cout << "Test " << i << ": inserting: " << key << " with value: " << value << std::endl;
			wrapper_insert(tree, key, value);
		}
		std::cerr << "ALL INSERTS ALL RIGHT" << std::endl;
		for (size_t i = 0; i < TEST_SIZE; i++) {
			K key = rand() % MAX_VALUE;
			std::cout << "Test " << i << ": deleting: " << key << std::endl;
			wrapper_erase(tree, key);
		}
	}
}
