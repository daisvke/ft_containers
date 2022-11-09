#ifndef ITERATOR_HPP
# define ITERATOR_HPP

# include <iterator>

namespace ft {

	// iterator traits
	template <class Iterator>
	struct iterator_traits {
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
		typedef typename Iterator::iterator_category	iterator_category;
	};

	template <class T>
	struct iterator_traits<T*>
	{
		typedef std::ptrdiff_t							difference_type;
		typedef T										value_type;
		typedef T*										pointer;
		typedef T&										reference;
		typedef std::random_access_iterator_tag			iterator_category;
	};

	template <class T>
	struct iterator_traits<const T*>
	{
		typedef std::ptrdiff_t							difference_type;
		typedef T										value_type;
		typedef const T*								pointer;
		typedef const T&								reference;
		typedef std::random_access_iterator_tag			iterator_category;
	};

	// random access iterator class
	template <class T>
	class	random_access_iterator {

	public:
		typedef std::ptrdiff_t		difference_type;
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const	value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const	value_type&	const_reference;
		typedef std::random_access_iterator_tag	iterator_category;

		random_access_iterator() : _ptr() {}
		random_access_iterator(pointer it) : _ptr(it) {}
		random_access_iterator(const random_access_iterator& it) : _ptr(it.base()) {}
		~random_access_iterator() {}

		// Getter
		pointer	base() const { return _ptr; }

		// Accessing operators
		reference operator*() const { return *_ptr; }
		pointer operator->() const { return &(operator*()); }
		reference operator[] (difference_type n) const { return _ptr[n]; }

		// Incrementing operators
		random_access_iterator& operator++() { ++_ptr; return *this; }
		random_access_iterator  operator++(int) {
			random_access_iterator	tmp(*this);
			++(*this);
			return tmp;
		}
		random_access_iterator& operator--() { --_ptr; return *this; }
		random_access_iterator  operator--(int) {
			random_access_iterator	tmp(*this);
			--(*this);
			return tmp;
		}

		// Arithmetic operators
		random_access_iterator operator+ (difference_type n) const {
			return random_access_iterator(_ptr + n);
		}
		random_access_iterator operator- (difference_type n) const {
			return *this + (-n);
		}
		difference_type operator+ (const random_access_iterator &b) const {
			return _ptr + b._ptr;
		}
		difference_type operator- (const random_access_iterator &b) const {
			return _ptr - b._ptr;
		}
		random_access_iterator& operator+= (const difference_type &n) { _ptr += n; return *this; }
		random_access_iterator& operator-= (const difference_type &n) { *this += -n; return *this; }

		 // Boolean operators
		bool operator== (random_access_iterator const& r) const { return (_ptr == r._ptr); };
		bool operator!= (random_access_iterator const& r) const { return (_ptr != r._ptr); };
		bool operator> (random_access_iterator const& r) const { return (_ptr > r._ptr); };
		bool operator< (random_access_iterator const& r) const { return (_ptr < r._ptr); };
		bool operator>= (random_access_iterator const& r) const { return (_ptr >= r._ptr); };
		bool operator<= (random_access_iterator const& r) const { return (_ptr <= r._ptr); };

	private:
		pointer	_ptr;
	};


}

#endif
