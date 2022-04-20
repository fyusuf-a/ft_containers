#include "map.hpp"
#include "utility.hpp"
#include <functional>
#include <stdexcept>

/*T container(char* tab) {
	T c;
	for (int i = 0; tab[i] != '\0'; i++) {
		c.push_back(tab[i]);
	}
	return c;
}*/

void test_miscellaneous() {
	ft::map<int, std::string> m;
	m.insert(ft::pair<const int, std::string>(0, "hello"));
	try {
		m.at(1);
		throw std::out_of_range("Index out of bounds");
	}
	catch(std::exception& e) {
	}
	m[1];
	assert(m.at(1) == "");

	ft::map<int, size_t> m1;
	for (int i = 0; i < 10; i++) {
		m1.insert(ft::pair<const int, size_t>(i, i));
	}
	ft::map<int, size_t, std::greater<size_t> > m2;
	for (int i = 0; i < 10; i++) {
		m1.insert(ft::pair<const int, size_t>(i, i));
	}
	/*ft::map<int, size_t>::reverse_iterator it = m1.rend();
	ft::map<int, size_t, std::greater<size_t> >::iterator jt = m2.begin();
	std::cout << "reverse iterator: " << std::endl;
	for (ft::map<int, size_t>::reverse_iterator it = m1.rbegin(); it != m1.rend(); it++) {
		std::cout << it->first << " " << it->second << std::endl;
	}
	for (; it != m1.rbegin(); it++, jt++) {
		assert(jt != m2.end());
		assert(it->first == jt->first);
		assert(it->second == jt->second);
	}
	assert(jt == m2.end());*/

}


void test_constructor() {
	// Testing constructor...
	ft::map<int, size_t> m;
	assert(m.size() == 0);

	for (int i = 0; i < 10; i++) {
		m.insert(ft::pair<int, size_t>(i, i));
	}
	assert(m.size() == 10);
	for (int i = 0; i < 10; i++) {
		assert(m.at(i) == i);
		assert(m[i] == i);
	}
	int i = 0;
	for (ft::map<int, size_t>::iterator it = m.begin(); it != m.end(); it++, i++) {
		assert(it->second == i);
	}

	ft::map<int, size_t, std::greater<int> > m2;
	assert(m2.size() == 0);

	for (int i = 0; i < 10; i++) {
		m2.insert(ft::pair<int, size_t>(i, i));
	}
	assert(m2.size() == 10);
	for (int i = 0; i < 10; i++) {
		assert(m2.at(i) == i);
	}
	i = 0;
	for (ft::map<int, size_t, std::greater<int> >::iterator it = m2.begin(); it != m2.end(); it++, i++) {
		assert(it->second == 9 - i);
	}

	// Testing constructor with iterators
	ft::map<int, size_t> m3(m.begin(), m.end());

	assert(m3.size() == m.size());
	for (ft::map<int, size_t>::iterator it = m3.begin(), jt = m.begin(); it != m3.end(); it++, jt++) {
		assert(it->second == jt->second);
		assert(it->first == jt->first);
		assert(&*it != &*jt);
	}

	// Testing copy constructor
	ft::map<int, size_t> m4(m);
	assert(m4.size() == m.size());
	for (ft::map<int, size_t>::iterator it = m4.begin(), jt = m.begin(); it != m4.end(); it++, jt++) {
		assert(it->second == jt->second);
		assert(it->first == jt->first);
		assert(&*it != &*jt);
	}
}

int main() {

	ft::map<int, std::string> m;

	test_constructor();
	test_miscellaneous();
	std::cout << "OK" << std::endl;
}


