#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#define FT_TEST
#include "../map.hpp"
#define TEST_SIZE 100
#define MAX_VALUE 100

using namespace std;
typedef int K;
typedef unsigned int V;

namespace ft {

	map<K, V>::size_type node_size(const map<K, V>::node_type* node) {
		if (node == nullptr)
			return 0;
		return 1 + node_size(node->left) + node_size(node->right);
	}

	void check_size(const map<K, V>& tree) {
		assert(tree.size() == node_size(tree.getRoot()));
	}

	void check_properties(const map<K, V>& tree) {
		map<K, V>::node_type::check_parent(tree.getRoot());
		map<K, V>::node_type::check_balance(tree.getRoot());
		map<K, V>::check_order(tree.getRoot());
		check_size(tree);
	}

	void wrapper_insert(map<K, V>& tree, const K& key, const V& value) {
		map<K, V>::node_type* node_present = tree._find(key);
		map<K, V>::size_type size_before = tree.size();
		map<K, V>::node_type* node = tree._insert(key, value);
		check_properties(tree);

		assert(node && node->data.first == key);
		if (tree.size() != size_before) {
			assert(tree.size() == size_before + 1);
			assert(node != nullptr);
			assert(node->data.second == value);
		}
		else {
			assert(node_present != nullptr);
		}
	}

	bool custom_find(map<K, V>::node_type* node, const K& key) {
		if (node == nullptr)
			return false;
		return custom_find(node->left, key) || node->data.first == key || custom_find(node->right, key);
	}
	

	void wrapper_swap(map<K, V>& tree) {
		K key1;
		map<K, V>::node_type* node1 = nullptr;
		while (!node1) {
			key1 = rand() % MAX_VALUE;
			node1 = tree._find(key1);
		}
		K key2;
		map<K, V>::node_type* node2 = nullptr;
		while (!node2) {
			key2 = rand() % MAX_VALUE;
			node2 = tree._find(key2);
		}

		//std::cout << "swapping " << key1 << " and " << key2 << "..." << std::endl;

		map<K, V>::size_type size_before = tree.size();
		tree.swap_nodes(node1, node2);
		
		check_properties(tree);
		assert(custom_find(tree.getRoot(), key1));
		assert(custom_find(tree.getRoot(), key2));

		assert(tree.size() == size_before);
	}

	void wrapper_erase(map<K, V>& tree, K key) {
		map<K, V>::size_type size_before = tree.size();
		map<K, V>::node_type* node_present = tree._find(key);
		tree._erase(key);
		check_properties(tree);
		if (node_present)
			assert(tree.size() == size_before - 1);
		else
			assert(tree.size() == size_before);
	}
}


int main() {
	clock_t t = clock();

	srand(time(NULL));
	ft::map<K, V> tree;

	for (size_t k = 0; k < 50; k++) {
		// testing insert
		tree.clear();
		assert(tree.size() == 0);
		for (size_t i = 0; i < TEST_SIZE; i++) {
			K key = rand() % MAX_VALUE;
			V value = rand() % MAX_VALUE;
			//std::cout << "Test " << i << ": inserting: " << key << " with value: " << value << std::endl;
			wrapper_insert(tree, key, value);
		}

		// testing swap
		for (size_t i = 0; i < TEST_SIZE; i++) {
			//std::cout << "Test " << i << ": erasing: " << key << std::endl;
			wrapper_swap(tree);
		}

		// filling the tree because it can be unordered by the preceding swap
		// operations
		tree.clear();
		assert(tree.size() == 0);
		for (size_t i = 0; i < TEST_SIZE; i++) {
			K key = rand() % MAX_VALUE;
			V value = rand() % MAX_VALUE;
			//std::cout << "Test " << i << ": inserting: " << key << " with value: " << value << std::endl;
			wrapper_insert(tree, key, value);
		}

		{
			//std::cerr << "TESTING COPY CONSTRUCTOR" << std::endl;
			ft::map<K, V> tree2(tree);
			check_properties(tree2);
			assert(tree == tree2);
		}

		//std::cerr << "TESTING ERASE" << std::endl;
		for (size_t i = 0; i < TEST_SIZE; i++) {
			K key = rand() % MAX_VALUE;
			//std::cout << "Test " << i << ": deleting: " << key << std::endl;
			wrapper_erase(tree, key);
		}
	}

	t = clock() - t;
	std::cout << "Time: " << ((float)t) / CLOCKS_PER_SEC << " seconds" << std::endl;
}
