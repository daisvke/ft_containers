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
	  
	 * The concept random_access_iterator refines bidirectional_iterator
	   by adding support for constant time advancement with the
	   +=, +, -=, and - operators, constant time computation of
	   distance with -, and array notation with subscripting.
	*************************************************************/
	template <class T>
	class	random_access_iterator {

	public:

		/*************************************************************
		 * Types
		*************************************************************/
		typedef std::ptrdiff_t					difference_type;
		typedef T								value_type;
		typedef value_type*						pointer;
		typedef value_type&						reference;
		typedef std::random_access_iterator_tag	iterator_category;

		typedef random_access_iterator<T>		self;

		/*************************************************************
		 * Construct/Copy/Destroy
		*************************************************************/
		random_access_iterator() : _current() {}
		explicit random_access_iterator(pointer it) : _current(it) {}
		random_access_iterator(const random_access_iterator &it)
			: _current(it.base()) {}
		~random_access_iterator() {}

		/*************************************************************
		 * Getter
		*************************************************************/
		pointer	base(void) const { return _current; }

		/*************************************************************
		 * Assigning operator
		*************************************************************/
	//	random_access_iterator& operator= (const random_access_iterator& x) { _current = x.base(); return  *this; }

		/*************************************************************
		 * Accessing operators
		*************************************************************/
		reference	operator*(void) const { return *_current; }
		pointer		operator->(void) const { return &(operator*()); }
		reference	operator[](difference_type n) const { return _current[n]; }

		/*************************************************************
		 * Incrementing operators
		*************************************************************/
		self& operator++(void) { ++_current; return *this; }
		self  operator++(int) {
			self	tmp(*this);
			++(*this);
			return tmp;
		}
		self& operator--(void) { --_current; return *this; }
		self  operator--(int) {
			self	tmp(*this);
			--(*this);
			return tmp;
		}

		/*************************************************************
		 * Arithmetic operators
		*************************************************************/
		self operator+(difference_type n) const {
			return self(_current + n);
		}
		self operator-(difference_type n) const {
			return *this + (-n);
		}
		difference_type operator+(const self &b) const {
			return _current + b._current;
		}
		difference_type operator-(const self &b) const {
			return _current - b._current;
		}
		self& operator+=(const difference_type &n) { _current += n; return *this; }
		self& operator-=(const difference_type &n) { *this += -n; return *this; }

		/*************************************************************
		 * Boolean operators
		*************************************************************/
		bool operator==(random_access_iterator const& r) const { return (_current == r._current); }
		bool operator!=(random_access_iterator const& r) const { return (_current != r._current); }
		// bool operator>(random_access_iterator const& r) const { return (_current > r._current); }
		// bool operator<(random_access_iterator const& r) const { return (_current < r._current); }
		// bool operator>=(random_access_iterator const& r) const { return (_current >= r._current); }
		// bool operator<=(random_access_iterator const& r) const { return (_current <= r._current); }


	private:

		pointer	_current;
	}; // random_access_iterator



	/*************************************************************
	 * random_access_const_iterator
	*************************************************************/
	template <class T>
	class	random_access_const_iterator {

	public:

		/*************************************************************
		 * Types
		*************************************************************/
		typedef std::ptrdiff_t					difference_type;
		typedef T								value_type;
		typedef const value_type*				pointer;
		typedef const value_type&				reference;
		typedef std::random_access_iterator_tag	iterator_category;

		typedef random_access_iterator<T>		iterator;

		typedef random_access_const_iterator<T>	self;


		/*************************************************************
		 * Construct/Copy/Destroy
		*************************************************************/
		random_access_const_iterator() : _current() {}
		explicit random_access_const_iterator(pointer it) : _current(it) {}
		random_access_const_iterator(const iterator &it) : _current(it.base()) {}
		~random_access_const_iterator() {}

		/*************************************************************
		 * Getter
		*************************************************************/
		pointer	base(void) const { return _current; }

		/*************************************************************
		 * Assigning operator
		*************************************************************/
	// //	random_access_const_iterator& operator=
	// 	(const random_access_const_iterator& x)
	// 	{ _current = x.base(); return  *this; }

		/*************************************************************
		 * Accessing operators
		*************************************************************/
		reference	operator*(void) const { return *_current; }
		pointer		operator->(void) const { return &(operator*()); }
	//	reference	operator[](difference_type n) const { return _current[n]; }

		/*************************************************************
		 * Incrementing operators
		*************************************************************/
		self& operator++(void) { ++_current; return *this; }
		self  operator++(int) {
			self	tmp(*this);
			++(*this);
			return tmp;
		}
		self& operator--(void) { --_current; return *this; }
		self  operator--(int) {
			self	tmp(*this);
			--(*this);
			return tmp;
		}

		/*************************************************************
		 * Arithmetic operators
		*************************************************************/
		self operator+(difference_type n) const {
			return self(_current + n);
		}
		self operator-(difference_type n) const {
			return *this + (-n);
		}
		difference_type operator+(const self &b) const {
			return _current + b._current;
		}
		difference_type operator-(const self &b) const {
			return _current - b._current;
		}
		self& operator+=(const difference_type &n) { _current += n; return *this; }
		self& operator-=(const difference_type &n) { *this += -n; return *this; }

		/*************************************************************
		 * Boolean operators
		*************************************************************/
		bool operator==(self const& r) const { return (_current == r._current); }
		bool operator!=(self const& r) const { return (_current != r._current); }
		// bool operator>(self const& r) const { return (_current > r._current); }
		// bool operator<(self const& r) const { return (_current < r._current); }
		// bool operator>=(self const& r) const { return (_current >= r._current); }
		// bool operator<=(self const& r) const { return (_current <= r._current); }


	private:

		pointer	_current;
	}; // random_access_const_iterator


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
		explicit reverse_iterator(iterator_type x) : _current(x) {}

		reverse_iterator(const reverse_iterator& x) : _current(x.base()) {}

		// A reverse_iterator across other types can be copied if the
		// underlying iterator can be converted to the type of current
		template <class Iter>
		reverse_iterator(const reverse_iterator<Iter>& x) : _current(x.base()) {}
		
		~reverse_iterator() {}

		/*************************************************************
		 * Getter
		*************************************************************/
		iterator_type	base(void) const { return _current; }

		/*************************************************************
		 * Assigning operator
		*************************************************************/
		template <class Iter>
		reverse_iterator& operator=(const reverse_iterator<Iter>& x)
		{ _current = x.base(); return  *this; }

		/*************************************************************
		 * Accessing operators
		*************************************************************/
    	// Returns a reference to the value at --current	
    	// This requires that --current is dereferenceable.		
		reference	operator*(void) const
		{ iterator tmp = _current; return *--tmp; }

    	// Returns a pointer to the value at --current
    	// This requires that --current is dereferenceable.
		pointer		operator->(void) const
		{ iterator	tmp = _current; return &(*--tmp); }

      	// Random access iterator requirements
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
	bool operator==(const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r)
	{ return l.base() == r.base(); }

	template <class Iter1, class Iter2>
	bool operator!=(const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r)
	{ return l.base() != r.base(); }

	template <class Iter1, class Iter2>
	bool operator<(const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r)
	{ return  l.base() > r.base(); }

	template <class Iter1, class Iter2>
	bool operator<=(const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r)
	{ return l.base() >= r.base(); }

	template <class Iter1, class Iter2>
	bool operator>(const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r)
	{ return l.base() < r.base(); }

	template <class Iter1, class Iter2>
	bool operator>=(const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r)
	{ return l.base() <= r.base(); }

	template <typename Iter>
	reverse_iterator<Iter> operator+(typename reverse_iterator<Iter>::difference_type n,
		const reverse_iterator<Iter>& it) { return it + n; }

	template <typename Iter1, typename Iter2>
	typename reverse_iterator<Iter1>::difference_type
	operator-(const reverse_iterator<Iter1>& l, const reverse_iterator<Iter2>& r)
	{ return r.base() - l.base(); }

} // namespace ft

#endif /* _FT_ITERATOR_HPP */
