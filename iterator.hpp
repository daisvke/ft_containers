#ifndef _FT_ITERATOR_HPP
# define _FT_ITERATOR_HPP

# include <iterator>

/*************************************************************
 * Implementation of three classes from the iterator library:
	1. iterator_traits
	2. random_access_iterator
	3. reverse_iterator
*************************************************************/

namespace ft {

	/*************************************************************
	 * iterator_traits
	 
	 * This class does nothing but define nested typedefs
	*************************************************************/
	template <typename Iterator>
	struct iterator_traits {
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
		typedef typename Iterator::iterator_category	iterator_category;
	};

  	// Partial specialization for pointer types
	template <typename T>
	struct iterator_traits<T*>
	{
		typedef std::ptrdiff_t							difference_type;
		typedef T										value_type;
		typedef T*										pointer;
		typedef T&										reference;
		typedef std::random_access_iterator_tag			iterator_category;
	};

	// Partial specialization for const pointer types
	template <typename T>
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

		/*************************************************************
		 * Types
		*************************************************************/
		typedef std::ptrdiff_t		difference_type;
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef std::random_access_iterator_tag	iterator_category;

		/*************************************************************
		 * Construct/Copy/Destroy
		*************************************************************/
		random_access_iterator() : _ptr() {}
		random_access_iterator(pointer it) : _ptr(it) {}
		random_access_iterator(const random_access_iterator &it) : _ptr(it.base()) {}
		~random_access_iterator() {}

		/*************************************************************
		 * Getter
		*************************************************************/
		pointer	base(void) const { return _ptr; }

		/*************************************************************
		 * Assigning operator
		*************************************************************/
		random_access_iterator& operator= (const random_access_iterator& x) { _ptr = x.base(); return  *this; }

		/*************************************************************
		 * Accessing operators
		*************************************************************/
		reference	operator*(void) const { return *_ptr; }
		pointer		operator->(void) const { return &(operator*()); }
		reference	operator[](difference_type n) const { return _ptr[n]; }

		/*************************************************************
		 * Incrementing operators
		*************************************************************/
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

		/*************************************************************
		 * Arithmetic operators
		*************************************************************/
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

		/*************************************************************
		 * Boolean operators
		*************************************************************/
		bool operator==(random_access_iterator const& r) const { return (_ptr == r._ptr); }
		bool operator!=(random_access_iterator const& r) const { return (_ptr != r._ptr); }
		bool operator>(random_access_iterator const& r) const { return (_ptr > r._ptr); }
		bool operator<(random_access_iterator const& r) const { return (_ptr < r._ptr); }
		bool operator>=(random_access_iterator const& r) const { return (_ptr >= r._ptr); }
		bool operator<=(random_access_iterator const& r) const { return (_ptr <= r._ptr); }


	private:

		pointer	_ptr;
	}; // random_access_iterator


	/*************************************************************
	 * reverse_iterator
	
	 * Bidirectional and random access iterators have corresponding
		reverse iterator adaptors that iterate through the data
		structure in the opposite direction.
	*************************************************************/
	template <typename iterator>
	class reverse_iterator {

	public:

		/*************************************************************
		 * Types
		*************************************************************/
		typedef 		 iterator										iterator_type;
		typedef typename iterator_traits<iterator>::difference_type		difference_type;
		typedef typename iterator_traits<iterator>::value_type			value_type;
		typedef typename iterator_traits<iterator>::pointer				pointer;
		typedef typename iterator_traits<iterator>::reference			reference;
		typedef typename iterator_traits<iterator>::iterator_category	iterator_category;

		/*************************************************************
		 * Construct/Copy/Destroy
		*************************************************************/
		reverse_iterator() : _current() {}
		reverse_iterator(iterator_type it) : _current(it) {}

		// A reverse_iterator across other types can be copied if the
		// underlying iterator can be converted to the type of current
		template <class Iter>
		reverse_iterator(const reverse_iterator<Iter>& it) : _current(it.base()) {}
		
		~reverse_iterator() {}

		/*************************************************************
		 * Getter
		*************************************************************/
		iterator_type	base(void) const { return _current; }

		/*************************************************************
		 * Assigning operator
		*************************************************************/
		template <class Iter>
		reverse_iterator& operator=(const reverse_iterator<Iter>& x) { _current = x.base(); return  *this; }

		/*************************************************************
		 * Accessing operators
		*************************************************************/
		// A reference to the value at _current
		reference	operator*(void) const { return *_current; }
		// A pointer to the value at _current
		pointer		operator->(void) const { return &(operator*()); }
		reference	operator[](difference_type n) const { return _current[n]; }

		/*************************************************************
		 * Arithmetic operators
		*************************************************************/
		reverse_iterator operator+(difference_type n) const { return  reverse_iterator(_current - n); }

		// Decrement the underlying iterator
		reverse_iterator& operator++(void) { --_current; return *this; }
		reverse_iterator operator++(int) { reverse_iterator tmp = *this; --_current; return  tmp; }

		reverse_iterator& operator+=(difference_type n) { _current -= n;	return *this; }
		reverse_iterator operator-(difference_type n) const { return reverse_iterator(_current + n); }

		// Increment the underlying iterator
		reverse_iterator& operator--(void) { ++_current; return *this; }
		reverse_iterator operator--(int) { reverse_iterator tmp = *this; ++_current; return tmp; }

		reverse_iterator operator-=(difference_type n) { _current += n; return *this; }


	protected:

		iterator _current;

	}; // reverse_iterator


	/*************************************************************
	 * Boolean operators for reverse iterators
	*************************************************************/
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

	template <typename Iter>
	reverse_iterator<Iter> operator+(typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter>& it) {
		return it + n;
	}

	template <typename Iter1, typename Iter2>
	typename reverse_iterator<Iter1>::difference_type
	operator-(const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r) {return r.base() - l.base(); }

} // namespace ft

#endif /* _FT_ITERATOR_HPP */
