#ifndef _FT_TYPE_TRAITS_HPP
# define _FT_TYPE_TRAITS_HPP

/*************************************************************
 * A partial type_traits library implementation
*************************************************************/

namespace ft {

	/*************************************************************
	 * enable_if
	*************************************************************/
	// Primary template
	// Define a member typedef `type` only if a boolean constant is true
	template <bool B, typename T = void>
	struct enable_if {};
	 
	// Partial specialization for true
	template <typename T>
	struct enable_if<true, T> { typedef T type; };


	/*************************************************************
	 * integral_constant
	
	 * Wraps a static constant of specified type.
	 * It is the base class for the C++ type traits.
	 * Accepts static constant of type T with value v.
	*************************************************************/
	template <typename T, T v>
	struct integral_constant {
		static const T					value = v;
		typedef T 						value_type;
		typedef integral_constant<T, v> type;
		operator value_type() { return value; }
	};

	// The type used as a compile-time boolean with true value
	typedef integral_constant<bool, true>	true_type;
	// The type used as a compile-time boolean with false value
	typedef integral_constant<bool, false>	false_type;
	

	/*************************************************************
	 * is_integral
	
	 * Checks whether T is an integral type.
	 * Provides the member constant value which is equal to true,
		if T is the type bool, char, wchar_t, short, int, long, long long,
		or any implementation-defined extended integer types,
		including any signed, unsigned, and cv-qualified variants.
		Otherwise, value is equal to false.
	 * is_integral inherits members from integral_constant
	*************************************************************/
	template <class T>
	struct is_integral : public ft::false_type {};

	template <> struct is_integral<bool>			: public ft::true_type {};
	template <> struct is_integral<signed char>		: public ft::true_type {};
	template <> struct is_integral<unsigned char>	: public ft::true_type {};
	template <> struct is_integral<char>			: public ft::true_type {};
	template <> struct is_integral<wchar_t>			: public ft::true_type {};
	template <> struct is_integral<short>			: public ft::true_type {};
	template <> struct is_integral<unsigned short>	: public ft::true_type {};
	template <> struct is_integral<int>				: public ft::true_type {};
	template <> struct is_integral<unsigned int>	: public ft::true_type {};
	template <> struct is_integral<long>			: public ft::true_type {};
	template <> struct is_integral<unsigned long>	: public ft::true_type {};
	template <> struct is_integral<long long>		: public ft::true_type {};
} // namespace ft

#endif /* _FT_TYPE_TRAITS_HPP */
