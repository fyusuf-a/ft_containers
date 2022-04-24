#include <iostream>
#include <vector>
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
	srand(time(NULL));
	ft::map<K, V> tree;

	for (size_t k = 0; k < 50; k++) {
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


		//std::cerr << "ALL INSERTS ALL RIGHT" << std::endl;
		for (size_t i = 0; i < TEST_SIZE; i++) {
			K key = rand() % MAX_VALUE;
			//std::cout << "Test " << i << ": deleting: " << key << std::endl;
			wrapper_erase(tree, key);
		}
	}
	std::cout << "OK" << std::endl;
}
