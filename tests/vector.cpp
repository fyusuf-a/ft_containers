#ifdef STD 
# define NAMESPACE std
# include <iterator>
# include <vector>
#else
# define NAMESPACE ft
# include "../iterator.hpp"
# include "../vector.hpp"
#endif

#include <cassert>
#include <iostream>

template<class T>
T container(char* tab) {
	T c;
	for (int i = 0; tab[i] != '\0'; i++) {
		c.push_back(tab[i]);
	}
	return c;
}

void test_constructor() {
	// Testing constructor...
	NAMESPACE::vector<int> empty_v;
	assert(empty_v.size() == 0);

	// Testing constructor with size...
	NAMESPACE::vector<int> v0(2);
	assert(v0.size() == 2);
	assert(v0.capacity() >= 2);

	// Testing constructor with size and value...
	NAMESPACE::vector<int> v1(2, -5);
	assert(v1.size() == 2);
	assert(v1[0] == -5);
	assert(v1[1] == -5);
	try {
		v1[2];
		throw std::out_of_range("Index out of bounds");
	}
	catch (std::exception &e) {
	}

	NAMESPACE::vector<int> v2(v1.begin(), v1.end());
	assert(v2.capacity() == 2);
	assert(v2.size() == 2);
	assert(v2[0] == -5);
	assert(v2[1] == -5);
	try {
		v2[2];
		throw std::out_of_range("Index out of bounds");
	}
	catch (std::exception &e) {
	}

	// Testing copy constructor
	NAMESPACE::vector<int> v3(v1.begin(), v1.end());
	assert(v3.capacity() == 2);
	assert(v3.size() == 2);
	assert(v3[0] == -5);
	assert(v3[1] == -5);
	try {
		v3[2];
		throw std::out_of_range("Index out of bounds");
	}
	catch (std::exception &e) {
	}
}

void test_various() {
	NAMESPACE::vector<int> v0(5, -1);
	assert(v0.size() == 5);
	for (NAMESPACE::vector<int>::iterator it = v0.begin(); it != v0.end(); ++it) {
		assert(*it == -1);
	}
	v0.push_back(2);
	assert(v0.size() == 6);
	NAMESPACE::vector<int>::iterator it;
	for (it = v0.begin(); it != v0.end() - 1; ++it) {
		assert(*it == -1);
	}
	assert(*it == 2);
	v0.pop_back();
	assert(v0.size() == 5);
	for (NAMESPACE::vector<int>::iterator it = v0.begin(); it != v0.end(); ++it) {
		assert(*it == -1);
	}

	v0.push_back(3);
	NAMESPACE::vector<int> v1 = v0;
	assert(v1.size() == v0.size());
	for (NAMESPACE::vector<int>::iterator it = v1.begin(), jt = v0.begin(); it != v1.end(); ++it, ++jt) {
		assert(*it == *jt);
	}
	assert(v1 == v0);
	assert(!(v1 != v0));
	NAMESPACE::vector<int> v3;
	assert(v3.size() == 0);
	assert(!(v1 == v3));

	NAMESPACE::vector<int> v4;
	v4.push_back(0);
	v4.push_back(1);
	v4.push_back(2);
	v4.push_back(3);
	assert(v4.size() == 4);
	int i = 3;
	for (NAMESPACE::vector<int>::reverse_iterator it = v4.rbegin(); it != v4.rend(); ++it, --i) {
		assert(*it == i);
	}

	v0.clear();
	assert(v0.size() == 0);
	for (NAMESPACE::vector<int>::iterator it = v0.begin(); it != v0.end(); ++it) {
		assert(0 == 1);
	}

}

void test_assign_and_reserve() {
	NAMESPACE::vector<int> v0;
	v0.assign(10, -2);
	assert(v0.size() == 10);
	for (NAMESPACE::vector<int>::iterator it = v0.begin(); it != v0.end(); ++it) {
		assert(*it == -2);
	}

	NAMESPACE::vector<int> v1;
	assert(v1.empty());
	v1.assign(v0.begin(), v0.end());
	assert(v1.size() == 10);
	for (NAMESPACE::vector<int>::iterator it = v1.begin(); it != v1.end(); ++it) {
		assert(*it == -2);
	}

	NAMESPACE::vector<int> v2(2, -7);
	assert(v2.size() == 2);
	assert(v2.capacity() >= 2);
	assert(!v2.empty());
	v2.reserve(3);
	assert(v2.size() == 2);
	assert(v2.capacity() >= 3);
	assert(v2[0] == -7 && v2[1] == -7);

	v2.assign(5, -12);
	for (NAMESPACE::vector<int>::iterator it = v2.begin(); it != v2.end(); ++it) {
		assert(*it == -12);
	}

	v2.clear();
	assert(v2.size() == 0);
}

void test_element_access() {
	NAMESPACE::vector<int> v0;
	v0.push_back(0);
	v0.push_back(1);
	v0.push_back(2);
	v0.push_back(3);

	try {
		(void)v0.at(4);
		throw std::out_of_range("Index out of bounds");
	}
	catch (std::out_of_range &e) {
	}
	NAMESPACE::vector<int>::const_reference first_value = v0.at(0);
	assert(first_value == 0);
	NAMESPACE::vector<int>::const_reference value1 = v0[2];
	assert(value1 == 2);
	assert(v0.front() == 0);
	assert(v0.back() == 3);
}

void test_insert_erase() {
	NAMESPACE::vector<int> v0;
	v0.push_back(0);
	v0.push_back(1);
	v0.push_back(3);
	v0.push_back(4);
	v0.push_back(5);
	assert(v0.size() == 5);
	v0.insert(v0.begin() + 2, 2);
	assert(v0.size() == 6);
	for (NAMESPACE::vector<int>::iterator it = v0.begin(); it != v0.end(); ++it) {
		assert(*it == it - v0.begin());
	}
	v0.erase(v0.begin() + 2, v0.begin() + 4);
	assert(v0.size() == 4);
	assert(v0[0] == 0 && v0[1] == 1 && v0[2] == 4 && v0[3] == 5);
	v0.insert(v0.begin() + 2, 2);
	v0.insert(v0.begin() + 3, 3);
	assert(v0.size() == 6);
	for (NAMESPACE::vector<int>::iterator it = v0.begin(); it != v0.end(); ++it) {
		assert(*it == it - v0.begin());
	}
	v0.erase(v0.end() - 3, v0.end());
	assert(v0.size() == 3);
	v0.insert(v0.begin(), v0.begin(), v0.end());
	NAMESPACE::vector<int>::iterator it;
	for (it = v0.begin(); it != v0.begin() + 3; ++it) {
		assert(*it == it - v0.begin());
	}
	for (; it != v0.end(); ++it) {
		assert(*it == it - v0.begin() - 3);
	}
}

void test_resize_swap() {
	NAMESPACE::vector<int> v0;
	v0.push_back(0);
	assert(v0.size() == 1);
	v0.resize(3, -1);
	assert(v0.size() == 3);
	assert(v0[0] == 0 && v0[1] == -1 && v0[2] == -1);
	v0.resize(2, -2);
	assert(v0.size() == 2);
	assert(v0[0] == 0 && v0[1] == -1);

	NAMESPACE::vector<int> v1;
	v1.push_back(3);
	v1.swap(v0);
	assert(v1.size() == 2);
	assert(v1[0] == 0 && v1[1] == -1);
	assert(v0.size() == 1);
	assert(v0[0] == 3);
}

void test_compare() {
	NAMESPACE::vector<char> v0 = container<NAMESPACE::vector<char> >((char*)"lol");
	NAMESPACE::vector<char> v1 = container<NAMESPACE::vector<char> >((char*)"lil");
	assert(v1 < v0);
	NAMESPACE::vector<char> v2 = container<NAMESPACE::vector<char> >((char*)"z");
	assert(v0 > v2);
	assert(v1 > v2);
	NAMESPACE::vector<char> v3 = container<NAMESPACE::vector<char> >((char*)"lop");
	assert(v0 < v3);
	assert(v0 <= v3);
}

int main()
{
	clock_t t = clock();

	test_constructor();
	test_assign_and_reserve();
	test_element_access();
	test_insert_erase();
	test_resize_swap();
	test_various();

	t = clock() - t;
	std::cout << "Time: " << ((float)t) / CLOCKS_PER_SEC << " seconds" << std::endl;
    return 0;
}

