#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

namespace ft {
	template<bool B, class T = void>
	struct enable_if {};

	template<class T>
	struct enable_if<true, T> {
		typedef T type;
	};

	template<class T, T v>
	struct integral_constant {
		typedef T						value_type;
		typedef integral_constant<T, v>	type;
		const static T value = v;
		operator T() {
			return v;
		}
	};

	template<class T>
	struct is_integral : public integral_constant<bool, false> {
		static const bool value = false;
	};

	template<>
	struct is_integral<char> : public integral_constant<bool, true> {
		static const bool value = true;
	};

	template<>
	struct is_integral<unsigned char> : public integral_constant<bool, true> {
		static const bool value = true;
	};

	template<>
	struct is_integral<wchar_t> : public integral_constant<bool, true> {
		static const bool value = true;
	};

	template<>
	struct is_integral<unsigned short> : public integral_constant<bool, true> {
		static const bool value = true;
	};

	template<>
	struct is_integral<unsigned int> : public integral_constant<bool, true> {
		static const bool value = true;
	};

	template<>
	struct is_integral<unsigned long> : public integral_constant<bool, true> {
		static const bool value = true;
	};

	template<>
	struct is_integral<unsigned long long> : public integral_constant<bool, true> {
		static const bool value = true;
	};

	template<>
	struct is_integral<signed char> : public integral_constant<bool, true> {
		static const bool value = true;
	};

	template<>
	struct is_integral<signed short> : public integral_constant<bool, true> {
		static const bool value = true;
	};

	template<>
	struct is_integral<signed int> : public integral_constant<bool, true> {
		static const bool value = true;
	};

	template<>
	struct is_integral<signed long> : public integral_constant<bool, true> {
		static const bool value = true;
	};

	template<>
	struct is_integral<signed long long> : public integral_constant<bool, true> {
		static const bool value = true;
	};
} /* namespace ft */

#endif

