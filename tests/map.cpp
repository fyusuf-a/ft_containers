#define FT_TEST
#include "../map.hpp"
#include "../utility.hpp"
#include <functional>
#include <stdexcept>

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

	// Testing iterators
	ft::map<int, size_t> my_map;
	for (int i = 0; i < 10; i++) {
		my_map.insert(ft::pair<const int, size_t>(i, i));
	}

	ft::map<int, size_t>::const_iterator my_it = my_map.begin();
	for (my_it = my_map.begin(); my_it != my_map.end(); ++my_it) {
		assert(static_cast<size_t>(my_it->first) == my_it->second);
	}

	ft::map<int, size_t> m1;
	for (int i = 0; i < 10; i++) {
		m1.insert(ft::pair<const int, size_t>(i, i));
	}
	ft::map<int, size_t, std::greater<size_t> > m2;
	for (int i = 0; i < 10; i++) {
		m2.insert(ft::pair<const int, size_t>(i, i));
	}
	ft::map<int, size_t>::reverse_iterator it = m1.rbegin();
	ft::map<int, size_t, std::greater<size_t> >::iterator jt = m2.begin();
	for (; it != m1.rend(); it++, jt++) {
		assert(jt != m2.end());
		assert(it->first == jt->first);
		assert(it->second == jt->second);
	}
	assert(jt == m2.end());

	// Testing empty
	assert(!m1.empty());
	assert(!m2.empty());

	// Testing erase
	assert(m1.size() == 10);
	assert(m2.size() == 10);
	m1.erase(0);
	assert(m1.find(0) == m1.end());
	m2.erase(0);
	assert(m2.find(0) == m2.end());
	assert(m1.size() == 9);

	it = m1.rbegin();
	jt = m2.begin();
	for (; it != m1.rend(); it++, jt++) {
		assert(jt != m2.end());
		assert(it->first == jt->first);
		assert(it->second == jt->second);
	}

	// Testing clear
	m1.erase(1);
	assert(m1.find(1) == m1.end());
	assert(m1.size() == 8);
	m1.clear();
	assert(m1.size() == 0);
	for (ft::map<int, size_t>::iterator it = m1.begin(); it != m1.end(); it++) {
		assert(false);
	}

	// Testing overload of insert
	m1.insert(m2.begin(), m2.end());
	assert(m1.size() == m2.size());

	it = m1.rbegin();
	jt = m2.begin();
	for (; it != m1.rend(); it++, jt++) {
		assert(jt != m2.end());
		assert(it->first == jt->first);
		assert(it->second == jt->second);
	}
	
}

void test_swap_and_operator_equal() {
	ft::map<int, size_t> m1;
	ft::map<int, size_t> m2;
	m1.insert(ft::pair<const int, size_t>(0, 6));
	m1.swap(m2);
	assert(m1.size() == 0);
	assert(m2.size() == 1);
	assert(m2.find(0)->second == 6);

	m2.clear();
	m2.insert(ft::pair<const int, size_t>(1, 1));
	m2.insert(ft::pair<const int, size_t>(2, 2));
	m2.insert(ft::pair<const int, size_t>(3, 3));
	m1 = m2;
	assert(m1.size() == 3);
	assert(m1.find(1)->second == 1);
	assert(m1.find(2)->second == 2);
	assert(m1.find(3)->second == 3);
	assert(m1 == m2);
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
		assert(static_cast<int>(m.at(i)) == i);
		assert(static_cast<int>(m[i]) == i);
	}
	int i = 0;
	for (ft::map<int, size_t>::iterator it = m.begin(); it != m.end(); i++) {
		assert(static_cast<int>(it->second) == i);
		it++;
	}

	ft::map<int, size_t, std::greater<int> > m2;
	assert(m2.size() == 0);

	for (int i = 0; i < 10; i++) {
		m2.insert(ft::pair<int, size_t>(i, i));
	}
	assert(m2.size() == 10);
	for (int i = 0; i < 10; i++) {
		assert(static_cast<int>(m2.at(i)) == i);
	}
	i = 0;
	for (ft::map<int, size_t, std::greater<int> >::iterator it = m2.begin(); it != m2.end(); it++, i++) {
		assert(static_cast<int>(it->second) == 9 - i);
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

void test_iterator() {
	ft::map<int, size_t> my_map;
	for (int i = 0; i < 10; i++) {
		my_map.insert(ft::pair<int, size_t>(i, i));
	}

	ft::map<int, size_t>::iterator my_it = my_map.begin();
	for (my_it = my_map.begin(); my_it != my_map.end(); ++my_it) {
		assert(static_cast<size_t>(my_it->first) == my_it->second);
		my_it->second = my_it->second + 1;
	}
}

void test_count_and_bounds() {
	ft::map <int, size_t> m;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 15; j++) {
			if (j < i) {
				assert(m.count(j) == 1);
				assert(m.find(j) != m.end());
			}
			else {
				assert(m.count(j) == 0);
				assert(m.find(j) == m.end());
			}
		}
		m.insert(ft::pair<int, size_t>(i, i));
	}
	assert(m.lower_bound(5) == m.find(5));
	assert(m.upper_bound(5) == m.find(6));
	assert(m.lower_bound(9) == m.find(9));
	assert(m.upper_bound(9) == m.end());
}

int main() {
	clock_t t = clock();

	test_constructor();
	test_miscellaneous();
	test_iterator();
	test_swap_and_operator_equal();
	test_count_and_bounds();

	t = clock() - t;
	std::cout << "Time: " << ((float)t) / CLOCKS_PER_SEC << " seconds" << std::endl;
    return 0;
}


