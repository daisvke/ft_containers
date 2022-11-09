#ifndef REVERSE_ITERATOR_HPP
# define REVERSE_ITERATOR_HPP

# include "iterator_traits.hpp"

namespace ft {

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
		iterator_type	base() const { return _ptr; }

		// Assigning operator
		template <class Iter>
		reverse_iterator& operator= (const reverse_iterator<Iter>& x) { _ptr = x.base(); return  *this; }

		// Accessing operators
		reference	operator*() const { return *_ptr; }
		pointer		operator->() const { return &(operator*()); }
		reference	operator[] (difference_type n) const { return _ptr[n]; }

		reverse_iterator operator+ (difference_type n) const { return  reverse_iterator(_ptr - n); }
		reverse_iterator& operator++ () { --_ptr; return *this; }
		reverse_iterator operator++ (int) { reverse_iterator tmp = *this; --_ptr; return  tmp; }
		reverse_iterator& operator+= (difference_type n) { _ptr -= n;	return *this; }
		reverse_iterator operator- (difference_type n) const { return reverse_iterator(_ptr + n); }
		reverse_iterator& operator-- () { ++_ptr; return *this; }
		reverse_iterator operator-- (int) { reverse_iterator tmp = *this; ++_ptr; return tmp; }
		reverse_iterator operator -= (difference_type n) { _ptr += n; return *this; }


	private:
		iterator _ptr;

	}; // End reverse_iterator class

	template <class Iter1, class Iter2>
	bool operator== (const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r) { return l.base() == r.base(); }

	template <class Iter1, class Iter2>
	bool operator!= (const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r) { return l.base() != r.base(); }

	template <class Iter1, class Iter2>
	bool operator< (const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r) { return  l.base() > r.base(); }

	template <class Iter1, class Iter2>
	bool operator<= (const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r) { return l.base() >= r.base(); }

	template <class Iter1, class Iter2>
	bool operator> (const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r) { return l.base() < r.base(); }

	template <class Iter1, class Iter2>
	bool operator>= (const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r) { return l.base() <= r.base(); }

	template <class Iter>
	reverse_iterator<Iter> operator+ (typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter>& it) {
		return it + n;
	}

	template <class Iter1, class Iter2>
	typename reverse_iterator<Iter1>::difference_type operator- (const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r) {return r.base() - l.base(); }

}

#endif
