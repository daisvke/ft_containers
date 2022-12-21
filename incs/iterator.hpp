#ifndef _FT_ITERATOR_HPP
# define _FT_ITERATOR_HPP

# include <iterator>

/*************************************************************
 * Implementation of three classes from header <iterator>:
	1. iterator_traits
	2. random_access_iterator
	3. reverse_iterator
*************************************************************/

namespace ft {

	/*************************************************************
	 * iterator_traits
	*************************************************************/
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
	

	/*************************************************************
	 * random_access_iterator
	*************************************************************/
	template <class T>
	class	random_access_iterator {

	public:

		typedef std::ptrdiff_t		difference_type;
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef std::random_access_iterator_tag	iterator_category;

		random_access_iterator() : _ptr() {}
		random_access_iterator(pointer it) : _ptr(it) {}
		random_access_iterator(const random_access_iterator &it) : _ptr(it.base()) {}
		~random_access_iterator() {}

		// Getter
		pointer	base(void) const { return _ptr; }

		// Assigning operator
		random_access_iterator& operator= (const random_access_iterator& x) { _ptr = x.base(); return  *this; }

		// Accessing operators
		reference	operator*(void) const { return *_ptr; }
		pointer		operator->(void) const { return &(operator*()); }
		reference	operator[](difference_type n) const { return _ptr[n]; }

		// Incrementing operators
		random_access_iterator& operator++(void) { ++_ptr; return *this; }
		random_access_iterator  operator++(int) {
			random_access_iterator	tmp(*this);
			++(*this);
			return tmp;
		}
		random_access_iterator& operator--(void) { --_ptr; return *this; }
		random_access_iterator  operator--(int) {
			random_access_iterator	tmp(*this);
			--(*this);
			return tmp;
		}

		// Arithmetic operators
		random_access_iterator operator+(difference_type n) const {
			return random_access_iterator(_ptr + n);
		}
		random_access_iterator operator-(difference_type n) const {
			return *this + (-n);
		}
		difference_type operator+(const random_access_iterator &b) const {
			return _ptr + b._ptr;
		}
		difference_type operator-(const random_access_iterator &b) const {
			return _ptr - b._ptr;
		}
		random_access_iterator& operator+=(const difference_type &n) { _ptr += n; return *this; }
		random_access_iterator& operator-=(const difference_type &n) { *this += -n; return *this; }

		// Boolean operators
		bool operator==(random_access_iterator const& r) const { return (_ptr == r._ptr); }
		bool operator!=(random_access_iterator const& r) const { return (_ptr != r._ptr); }
		bool operator>(random_access_iterator const& r) const { return (_ptr > r._ptr); }
		bool operator<(random_access_iterator const& r) const { return (_ptr < r._ptr); }
		bool operator>=(random_access_iterator const& r) const { return (_ptr >= r._ptr); }
		bool operator<=(random_access_iterator const& r) const { return (_ptr <= r._ptr); }


	private:

		pointer	_ptr;
	};


	/*************************************************************
	 * reverse_iterator
	*************************************************************/
	template <class iterator>
	class reverse_iterator
	{

	public:

		typedef iterator iterator_type;
		typedef typename iterator_traits<iterator>::difference_type		difference_type;
		typedef typename iterator_traits<iterator>::value_type			value_type;
		typedef typename iterator_traits<iterator>::pointer				pointer;
		typedef typename iterator_traits<iterator>::reference			reference;
		typedef typename iterator_traits<iterator>::iterator_category	iterator_category;

		reverse_iterator() : _ptr() {}
		reverse_iterator(iterator_type it) : _ptr(it) {}
		template <class Iter>
		reverse_iterator(const reverse_iterator<Iter>& it) : _ptr(it.base()) {}
		~reverse_iterator() {}

		// Getter
		iterator_type	base(void) const { return _ptr; }

		// Assigning operator
		template <class Iter>
		reverse_iterator& operator=(const reverse_iterator<Iter>& x) { _ptr = x.base(); return  *this; }

		// Accessing operators
		reference	operator*(void) const { return *_ptr; }
		pointer		operator->(void) const { return &(operator*()); }
		reference	operator[](difference_type n) const { return _ptr[n]; }

		reverse_iterator operator+(difference_type n) const { return  reverse_iterator(_ptr - n); }
		reverse_iterator& operator++(void) { --_ptr; return *this; }
		reverse_iterator operator++(int) { reverse_iterator tmp = *this; --_ptr; return  tmp; }
		reverse_iterator& operator+=(difference_type n) { _ptr -= n;	return *this; }
		reverse_iterator operator-(difference_type n) const { return reverse_iterator(_ptr + n); }
		reverse_iterator& operator--(void) { ++_ptr; return *this; }
		reverse_iterator operator--(int) { reverse_iterator tmp = *this; ++_ptr; return tmp; }
		reverse_iterator operator-=(difference_type n) { _ptr += n; return *this; }


	private:

		iterator _ptr;

	}; // End reverse_iterator class

	template <class Iter1, class Iter2>
	bool operator==(const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r) { return l.base() == r.base(); }

	template <class Iter1, class Iter2>
	bool operator!=(const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r) { return l.base() != r.base(); }

	template <class Iter1, class Iter2>
	bool operator<(const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r) { return  l.base() > r.base(); }

	template <class Iter1, class Iter2>
	bool operator<=(const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r) { return l.base() >= r.base(); }

	template <class Iter1, class Iter2>
	bool operator>(const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r) { return l.base() < r.base(); }

	template <class Iter1, class Iter2>
	bool operator>=(const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r) { return l.base() <= r.base(); }

	template <class Iter>
	reverse_iterator<Iter> operator+(typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter>& it) {
		return it + n;
	}

	template <class Iter1, class Iter2>
	typename reverse_iterator<Iter1>::difference_type operator-(const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r) {return r.base() - l.base(); }

}

#endif /* _FT_ITERATOR_HPP */
