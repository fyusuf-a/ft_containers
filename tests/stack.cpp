#ifdef STD 
# define NAMESPACE std
# include <stack>
#else
# define NAMESPACE ft
# include "../stack.hpp"
#endif

#include <cassert>
#include <iostream>

int main() {
	clock_t t = clock();

	NAMESPACE::stack<int> s;
	s.push(1);
	// s = {1}
	s.push(2);
	// s = {1, 2}
	s.push(3);
	// s = {1, 2, 3}
	assert(s.size() == 3);
	assert(s.top() == 3);
	s.pop();
	// s = {1, 2}
	assert(s.size() == 2);
	assert(s.top() == 2);
	NAMESPACE::stack<int> s2(s);
	// s2 = {1, 2}
	assert(s == s2);
	NAMESPACE::stack<int> s3(s);
	assert(s3 == s);
	s3.push(4);
	// s3 = {1, 2, 4}
	assert(s3 != s);
	assert(s3 != s2);
	assert(!s3.empty());
	s.pop();
	assert(s.size() == 1);
	// s = {1}
	s.pop();
	// s = {}
	assert(s.empty());

	t = clock() - t;
	std::cout << "Time: " << ((float)t) / CLOCKS_PER_SEC << " seconds" << std::endl;
}

