#include <iostream>
#include <iterator>
#include <cassert>
#include <vector>
#include "../algorithm.hpp"
#include "../iterator.hpp"
#include "../utility.hpp"

#define TEST(i) std::cout << "Test no " << i << std::endl; i++;

void test_pair() {
	ft::pair<int, int> p1(1, 2);
	ft::pair<int, int> p2(1, 3);
	ft::pair<int, int> p3(2, 3);
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
	ft::pair<int, int> p4(p1);
	assert(p1 == p4);
}

void test_reverse_iterator() {
	int tab[6] = {5, 4, 3, 2, 1, 0};
	std::vector<int> x;
	for (int i = 0; i < 6; i++)
		x.push_back(tab[i]);

	std::vector<int>::reverse_iterator reverse;

	std::vector<int>::iterator first = x.end();
	ft::reverse_iterator<std::vector<int>::iterator > first2(first);
	std::vector<int>::iterator end = x.begin();
	ft::reverse_iterator<std::vector<int>::iterator > end2(end);

	for(reverse = x.rbegin(); first2 != end2; ++first2, ++reverse)
		assert(*reverse == *first2);
}

bool is_palindrome(std::string s) {
	return ft::equal(s.begin(), s.end(), s.rbegin());
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
	assert(ft::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end()) == std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end()));
	assert(ft::lexicographical_compare(s2.begin(), s2.end(), s1.begin(), s1.end()) == std::lexicographical_compare(s2.begin(), s2.end(), s1.begin(), s1.end()));
	assert(ft::lexicographical_compare(s2.begin(), s2.end(), s3.begin(), s3.end()) == std::lexicographical_compare(s2.begin(), s2.end(), s3.begin(), s3.end()));
	assert(ft::lexicographical_compare(s3.begin(), s3.end(), s2.begin(), s2.end()) == std::lexicographical_compare(s3.begin(), s3.end(), s2.begin(), s2.end()));
	assert(ft::lexicographical_compare(s1.begin(), s1.end(), s3.begin(), s3.end()) == std::lexicographical_compare(s1.begin(), s1.end(), s3.begin(), s3.end()));
	assert(ft::lexicographical_compare(s3.begin(), s3.end(), s1.begin(), s1.end()) == std::lexicographical_compare(s3.begin(), s3.end(), s1.begin(), s1.end()));
}

int main()
{
	int i = 1;

	/*TEST(i)
	static_assert(std::is_same< ft::iterator_traits<std::istream_iterator<char> >::iterator_category, std::input_iterator_tag>::value, "An istream_iterator should be an input iterator");*/
    
	TEST(i)
	int* distance = &i;
	std::iterator_traits<int*>::difference_type dist = ft::distance(distance + 2, distance + 12);
	assert(dist == 10);

	/*TEST(i)
	static_assert(std::is_same< ft::iterator_traits<char*>::value_type, char>::value, "Value type for char* should be char");

	TEST(i)
	static_assert(!std::is_same< ft::iterator_traits<char*>::value_type, int>::value, "Value type for char* should not be int");*/

	TEST(i)
	test_reverse_iterator();

	TEST(i)
	test_equal();

	TEST(i)
	test_compare();

	TEST(i)
	test_pair();

    return (0);
}

