#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "iterator.hpp"
#include "type_traits.hpp"

namespace ft
{
	template<class InputIt1, class InputIt2>
	bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
	{
		while (first1 != last1)
		{
			if (*first1 != *first2)
				return false;
			++first1;
			++first2;
		}
		return true;
	}

	template<class InputIt1, class InputIt2, class BinaryPredicate>
	bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p)
	{
		while (first1 != last1)
		{
			if (!p(*first1, *first2))
				return false;
			++first1;
			++first2;
		}
		return true;
	}

	template<class InputIt1, class InputIt2>
	bool _equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, ft::input_iterator_tag, ft::input_iterator_tag)
	{
		while (first1 != last1)
		{
			if (first2 == last2)
				return false;
			if (*first1 != *first2)
				return false;
			++first1;
			++first2;
		}
		return first2 == last2;
	}

	template<class InputIt1, class InputIt2>
	bool _equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, ft::random_access_iterator_tag, ft::random_access_iterator_tag)
	{
		if (last2 - first2 != last1 - first1)
			return false;
		return _equal(first1, last1, first2, last2, ft::input_iterator_tag(), ft::input_iterator_tag());
	}

	template<class InputIt1, class InputIt2>
	bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2) {
		return _equal(first1, last1, first2, last2, typename ft::iterator_traits<InputIt1>::iterator_category(), typename ft::iterator_traits<InputIt2>::iterator_category());
	}

	template<class InputIt1, class InputIt2>
	bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
	{
		while (first1 != last1)
		{
			if (first2 == last2)
				return false;
			if (*first2 < *first1)
				return false;
			if (*first1 < *first2)
				return true;
			++first1;
			++first2;
		}
		return first2 != last2;
	}
};



#endif
