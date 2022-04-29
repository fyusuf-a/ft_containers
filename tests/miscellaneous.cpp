#include <iostream>
#include <iterator>
#include <cassert>
#include <vector>

#ifndef STD 
# define NAMESPACE std
# include <algorithm>
# include <iterator>
# include <utility>
#else
# define NAMESPACE ft
# include "../algorithm.hpp"
# include "../iterator.hpp"
# include "../utility.hpp"
#endif

void test_pair() {
	NAMESPACE::pair<int, int> p1(1, 2);
	NAMESPACE::pair<int, int> p2(1, 3);
	NAMESPACE::pair<int, int> p3(2, 3);
	assert(p1 < p2);
	assert(p2 < p3);
	assert(p1 < p3);
	assert(p1 <= p2);
	assert(p2 <= p3);
	assert(p1 <= p3);
	assert(p1 != p2);
	assert(p2 != p3);
	assert(p1 != p3);
	assert(!(p1 >= p2));
	assert(!(p2 >= p3));
	assert(!(p1 >= p3));
	assert(!(p1 > p2));
	assert(!(p2 > p3));
	assert(!(p1 > p3));
	NAMESPACE::pair<int, int> p4(p1);
	assert(p1 == p4);
}

void test_reverse_iterator() {
	int tab[6] = {5, 4, 3, 2, 1, 0};
	std::vector<int> x;
	for (int i = 0; i < 6; i++)
		x.push_back(tab[i]);

	std::vector<int>::reverse_iterator reverse;

	std::vector<int>::iterator first = x.end();
	NAMESPACE::reverse_iterator<std::vector<int>::iterator > first2(first);
	std::vector<int>::iterator end = x.begin();
	NAMESPACE::reverse_iterator<std::vector<int>::iterator > end2(end);

	for(reverse = x.rbegin(); first2 != end2; ++first2, ++reverse)
		assert(*reverse == *first2);
}

bool is_palindrome(std::string s) {
	return NAMESPACE::equal(s.begin(), s.end(), s.rbegin());
}

void test_equal() {
	assert(is_palindrome((char*)""));
	assert(is_palindrome((char*)"a"));
	assert(is_palindrome((char*)"aa"));
	assert(is_palindrome((char*)"aba"));
	assert(is_palindrome((char*)"abba"));
	assert(is_palindrome((char*)"abcdcba"));
	assert(!is_palindrome((char*)"abcdcbaa"));
}

void test_compare() {
	std::string s1 = "a";
	std::string s2 = "b";
	std::string s3 = "ab";
	assert(std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end()) == std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end()));
	assert(!std::lexicographical_compare(s2.begin(), s2.end(), s1.begin(), s1.end()));
	assert(!std::lexicographical_compare(s2.begin(), s2.end(), s3.begin(), s3.end()));
	assert(std::lexicographical_compare(s3.begin(), s3.end(), s2.begin(), s2.end()));
	assert(std::lexicographical_compare(s1.begin(), s1.end(), s3.begin(), s3.end()));
	assert(!std::lexicographical_compare(s3.begin(), s3.end(), s1.begin(), s1.end()));
	assert(NAMESPACE::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end()) == std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end()));
	assert(NAMESPACE::lexicographical_compare(s2.begin(), s2.end(), s1.begin(), s1.end()) == std::lexicographical_compare(s2.begin(), s2.end(), s1.begin(), s1.end()));
	assert(NAMESPACE::lexicographical_compare(s2.begin(), s2.end(), s3.begin(), s3.end()) == std::lexicographical_compare(s2.begin(), s2.end(), s3.begin(), s3.end()));
	assert(NAMESPACE::lexicographical_compare(s3.begin(), s3.end(), s2.begin(), s2.end()) == std::lexicographical_compare(s3.begin(), s3.end(), s2.begin(), s2.end()));
	assert(NAMESPACE::lexicographical_compare(s1.begin(), s1.end(), s3.begin(), s3.end()) == std::lexicographical_compare(s1.begin(), s1.end(), s3.begin(), s3.end()));
	assert(NAMESPACE::lexicographical_compare(s3.begin(), s3.end(), s1.begin(), s1.end()) == std::lexicographical_compare(s3.begin(), s3.end(), s1.begin(), s1.end()));
}

int main()
{
	clock_t t = clock();

	int i = 1;

	/*static_assert(std::is_same< NAMESPACE::iterator_traits<std::istream_iterator<char> >::iterator_category, std::input_iterator_tag>::value, "An istream_iterator should be an input iterator");*/
    
	int* distance = &i;
	std::iterator_traits<int*>::difference_type dist = NAMESPACE::distance(distance + 2, distance + 12);
	assert(dist == 10);

	/*static_assert(std::is_same< NAMESPACE::iterator_traits<char*>::value_type, char>::value, "Value type for char* should be char");

	static_assert(!std::is_same< NAMESPACE::iterator_traits<char*>::value_type, int>::value, "Value type for char* should not be int");*/

	test_reverse_iterator();

	test_equal();

	test_compare();

	test_pair();

	t = clock() - t;
	std::cout << "Time: " << ((float)t) / CLOCKS_PER_SEC << " seconds" << std::endl;
    return (0);
}

