#include <iostream>
#include <vector>
#define FT_TEST
#include "../tree.hpp"
#define TEST_SIZE 50
#define MAX_VALUE 50

using namespace std;
typedef int K;
typedef string V;

namespace ft {

	BinaryTree<K, V>::size_type node_size(const BinaryTree<K, V>::node_type* node) {
		if (node == nullptr)
			return 0;
		return 1 + node_size(node->left) + node_size(node->right);
	}

	void check_size(const BinaryTree<K, V>& tree) {
		assert(tree.size() == node_size(tree.getRoot()));
	}

	void check_properties(const BinaryTree<K, V>& tree) {
		BinaryTree<K, V>::node_type::check_parent(tree.getRoot());
		BinaryTree<K, V>::node_type::check_balance(tree.getRoot());
		BinaryTree<K, V>::node_type::check_order(tree.getRoot());
		check_size(tree);
	}

	void wrapper_insert(BinaryTree<K, V>& tree, const K& key, const V& value) {
		BinaryTree<K, V>::size_type size_before = tree.size();
		tree.insert(key, value);
		check_properties(tree);
		if (tree.size() != size_before)
			assert(tree.size() == size_before + 1);
		else
			assert(tree.find(key)->data.second == value);
	}

	void wrapper_erase(BinaryTree<K, V>& tree, K key) {
		BinaryTree<K, V>::size_type size_before = tree.size();
		BinaryTree<K, V>::node_type* node_present = tree.find(key);
		tree.erase(key);
		check_properties(tree);
		if (node_present)
			assert(tree.size() == size_before - 1);
		else
			assert(tree.size() == size_before);
	}
}


int main() {
	srand(time(NULL));
	ft::BinaryTree<K, V> tree;
	//tree.insert(5, "");
	//tree.insert(6, "");
	//tree.insert(4, "");
	//tree.insert(0, "");

	////std::cout << tree << std::endl;

	//tree.erase(6);
	////std::cout << tree << std::endl;
	//check_properties(tree);

	for (size_t k = 0; k < 50; k++) {
		tree.clear();
		assert(tree.size() == 0);
		for (size_t i = 0; i < TEST_SIZE; i++) {
			K key = rand() % MAX_VALUE;
			V value = "";
			std::cout << "Test " << i << ": inserting: " << key << " with value: " << value << std::endl;
			wrapper_insert(tree, key, value);
		}
		std::cerr << "ALL INSERTS ALL RIGHT" << std::endl;
		for (size_t i = 0; i < TEST_SIZE; i++) {
			//std::cout << tree;
			K key = rand() % MAX_VALUE;
			std::cout << "Test " << i << ": deleting: " << key << std::endl;
			wrapper_erase(tree, key);
		}
	}
}
