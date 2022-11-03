#ifndef ENABLE_IF
# define ENABLE_IF

// enable_if
namespace ft {

	template <bool B, class T = void>
	struct enable_if {};
	 
	template <class T>
	struct enable_if<true, T> { typedef T type; };

// is_integral
	template <class T>
	struct is_integral
}

#endif
