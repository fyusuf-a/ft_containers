#include <iostream>
#include <iterator>
#include <cassert>
#include "iterator.hpp"

#define TEST(i) std::cout << "Test no " << i << std::endl; i++;

namespace ft {
template<class BidirIt>
typename std::iterator_traits<BidirIt>::difference_type
distance (BidirIt first, BidirIt last)
{
	typename ft::iterator_traits<BidirIt>::difference_type result = 0;
	while (first != last) {
		++first;
		++result;
	}
	return result;
}

}


int main()
{
	int i = 1;
	
	TEST(i)
	static_assert(std::is_same< ft::iterator_traits<std::istream_iterator<char> >::iterator_category, std::input_iterator_tag>::value, "An istream_iterator should be an input iterator");
    
	TEST(i)
	int* distance;
	std::iterator_traits<int*>::difference_type dist = ft::distance(distance + 2, distance + 12);
	assert(dist == 10);

	TEST(i)
	static_assert(std::is_same< ft::iterator_traits<char*>::value_type, char>::value, "Value type for char* should be char");

	TEST(i)
	static_assert(!std::is_same< ft::iterator_traits<char*>::value_type, int>::value, "Value type for char* should not be int");

	int lil[3] = {6, 5, 4};
	int* lol = lil;

	printf("%p\n", lol);

	ft::reverse_iterator<int*> rev_lol(lol + 2);
	printf("%p\n", rev_lol.operator->());


    return 0;
}

