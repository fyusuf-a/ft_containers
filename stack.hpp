#ifndef STACK_HPP
#define STACK_HPP

#include <deque>

namespace ft {

template<class T, class Container = std::deque<T> >
class stack {
	public:
	typedef Container						container_type;
	typedef typename Container::value_type	value_type;
	typedef typename Container::size_type	size_type;
	typedef typename Container::reference	reference;
	typedef typename Container::const_reference	const_reference;

	protected:
	container_type c;

	public:
	explicit stack(const Container& c = Container()) : c(c) {}
	stack(const stack& s) : c(s.c) {}

	stack& operator=(const stack& s) {
		if (this != &s) {
			c = s.c;
		}
		return *this;
	}

	// Element access
	reference top() {
		return c.back();
	}
	const_reference top() const {
		return c.back();
	}

	// Capacity
	bool empty() const {
		return c.empty();
	}
	size_type size() const {
		return c.size();
	}

	// Modifiers
	void push(const value_type& val) {
		c.push_back(val);
	}
	void pop() {
		c.pop_back();
	}

	friend bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
	{
		return lhs.c == rhs.c;
	}
	friend bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
	{
		return lhs.c != rhs.c;
	}
	friend bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
	{
		return lhs.c < rhs.c;
	}
	friend bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
	{
		return lhs.c <= rhs.c;
	}
	friend bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
	{
		return lhs.c > rhs.c;
	}
	friend bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
	{
		return lhs.c >= rhs.c;
	}
};

}


#endif /* end of include guard STACK_HPP */

