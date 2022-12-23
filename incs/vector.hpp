#ifndef _FT_VECTOR_HPP
# define _FT_VECTOR_HPP

# include <memory>
# include <iterator>

# include "algorithm.hpp"
# include "type_traits.hpp"
# include "iterator.hpp"
# include "utility.hpp"

namespace ft {

	/*************************************************************
	 * A standard container which offers fixed time access to
		individual elements in any order.

	 * In some terminology a %vector can be described as a dynamic
	 	C-style array, it offers fast and efficient access to individual
		elements in any order and saves the user from worrying about
		memory and size allocation. 
	*************************************************************/
	template < class T, class Alloc = std::allocator<T> >
	class vector {
	
	public:

		/*************************************************************
		 * Types
		*************************************************************/
		typedef Alloc										allocator_type;
		typedef typename allocator_type::reference			reference;
		typedef typename allocator_type::const_reference	const_reference;
		typedef ft::random_access_iterator<T>				iterator;
		typedef ft::random_access_iterator<const T>			const_iterator;
		typedef std::size_t									size_type;
		typedef std::ptrdiff_t								difference_type;
		typedef T											value_type;
		typedef typename allocator_type::pointer			pointer;
		typedef typename allocator_type::const_pointer		const_pointer;
		typedef ft::reverse_iterator<iterator>				reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;

		/*************************************************************
		 * Construct/Copy/Destroy
		*************************************************************/
		// Empty container constructor (default constructor)
		// Creates a vector with no elements
		explicit vector(const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _capacity(0), _array(0), _size(0) {}

		// Fill constructor
		// This constructor fills the vector with n copies of value.
		explicit vector(size_type n, const value_type& val = value_type(),
			const allocator_type& alloc = allocator_type())
			:  _alloc(alloc), _capacity(0), _array(0), _size(0) {
			_capacity = n;
			_array = _alloc.allocate(n);
			
			for (size_type i = 0; i < n; ++i)
				_alloc.construct(_array + i, val);
		}

		// Range constructor
		// Create a vector consisting of copies of the elements from [first,last)
		//	with logN memory reallocations.
		template <class InputIterator>
		vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _capacity(0), _array(0), _size(0) {
			while (first != last) { push_back(*first++); }
		}

		// Copy constructor
		// All the elements of x are copied, but any unused capacity in x will not be copied
        // (i.e. capacity() == size() in the new %vector).
		vector(const vector& x): _alloc(x._alloc), _capacity(x._capacity),
			_array(_alloc.allocate(x._capacity)), _size(x._size) { assign(x.begin(), x.end()); }

		// Destructor
		// This only erases the elements. If the elements themselves are pointers, the pointed-to
        // memory is not touched in any way. Managing the pointer is the user's responsibility.
		~vector() {
			clear();
			_alloc.deallocate(_array, _capacity);
		}

		/*************************************************************
		 * Assigning operator
		*************************************************************/
		// All the elements of @a __x are copied, but any unused capacity
    	// in x will not be copied.
		vector& operator=(const vector& x) {
			if (this != &x) assign(x.begin(), x.end());
			return *this;
		}

		// This function fills a vector with copies of the elements in the range [__first,__last). 
		// Note that the assignment completely changes the vector and that
    	// the resulting vector's size is the same as the number of elements assigned.
		template <class InputIterator>
		void assign(InputIterator first, InputIterator last,
			typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = 0) {
			// Get size needed by the range
			size_type	n = 0;
			for (InputIterator it(first); it != last; ++it)
				++n;
			// Increase capacity if not enough
			if (n > _capacity) reserve(n);
			// Fill the vector with the range
			for (size_type i(0); i < n; ++i)
				_alloc.construct(_array + i, *first++);
			// Erase at end if n is smaller than the vector size
			if (n < _size)
				for (size_type i(n); i < _size; ++i)
					_alloc.destroy(_array + i);
			_size = n;
		}

		// This function fills a vector with n copies of the given
    	// value. Note that the assignment completely changes the vector and that
    	// the resulting vector's size is the same as the number of elements assigned.
		void assign(size_type n, const T& u) {
			// Increase capacity if not enough
			if (n > _capacity) reserve(n);
			// Fill vector with u
			for (size_type i(0); i < n; ++i)
				_alloc.construct(_array + i, u);
			// Erase at end if n is smaller than the vector size
			if (n < _size)
				for (size_type i(n); i < _size; ++i)
					_alloc.destroy(_array + i);
			// Update vector size
			_size = n;
		}

		/*************************************************************
		 * Get a copy of the memory allocation object
		*************************************************************/
		allocator_type get_allocator() const { return _alloc; }

		/*************************************************************
		 * Iterators
		*************************************************************/
		// Return an iterator that points to the first element in the vector
		iterator begin(void) { return iterator(_array); }
		const_iterator begin(void) const { return const_iterator(_array); }
		// Return an iterator that points one past the last element in the vector
		iterator end(void) { return iterator(_array + _size); }
		const_iterator end() const { return const_iterator(_array + _size); }
		// Return a reverse iterator that points to the last element in the vector.
		reverse_iterator rbegin(void) { return reverse_iterator(end()); }
		const_reverse_iterator rbegin(void) const { return const_reverse_iterator(end()); }
		// Return a reverse iterator that points to one before the first element in the vector
		reverse_iterator rend(void) { return reverse_iterator(begin()); }
		const_reverse_iterator rend(void) const { return const_reverse_iterator(begin()); }

		/*************************************************************
		 * Capacity
		*************************************************************/
    	// Returns the number of elements in the vector
		size_type size(void) const { return _size; }
		// Returns the size() of the largest possible vector
		size_type max_size(void) const { return _alloc.max_size(); }

		// This function will resize the vector to the specified
        // number of elements.  If the number is smaller than the
        // vector's current size the vector is truncated, otherwise
        // the vector is extended and new elements are populated with given data.
		void resize(size_type sz, value_type c = value_type()) {
			while (sz > _size) push_back(c);
			while (sz < _size) pop_back();
		}

		// Returns the total number of elements that the vector can
    	// hold before needing to allocate more memory
		size_type capacity(void) const { return _capacity; }

		// Returns true if the vector is empty
		bool empty(void) const { return _size == 0; }

		// This function attempts to reserve enough memory for the
    	// vector to hold the specified number of elements. If the
		// number requested is more than max_size(), length_error is
    	// thrown.	
    	// The advantage of this function is that if optimal code is a
    	// necessity and the user can determine the number of elements
    	// that will be required, the user can reserve the memory in
    	// advance, and thus prevent a possible reallocation of memory
    	// and copying of vector data.
		void reserve(size_type n) {
			if (n > max_size())
				throw std::length_error("Reserve size exceeds allocation max size");
			if (n > _capacity) {
				value_type	*new_array = _alloc.allocate(n);
				for (size_type i(0);  i < _size; ++i)
				{
					_alloc.construct(new_array + i, _array[i]);
					_alloc.destroy(_array + i);
				}
				_alloc.deallocate(_array, _capacity);
				_array = new_array;
				_capacity = n;
			}
		}

		/*************************************************************
		 * Operator overloads
		*************************************************************/
		// These two operators allow for easy, array-style, data access.
    	// Note that data access with this operator is unchecked and
    	// out_of_range lookups are not defined. (For checked lookups
    	// see at().)
		reference operator[](size_type n) { return _array[n]; }
		const_reference operator[](size_type n) const { return _array[n]; }

		// This function provides for safer data access.  The parameter
		// is first checked that it is in the range of the vector.  The
    	// function throws out_of_range if the check fails.
		const_reference at(size_type n) const {
			if (n < _size) return _array[n];
			throw std::out_of_range("Unvalid index is out of range");
		}
		reference at(size_type n) {
			if (n < _size) return _array[n];
			throw std::out_of_range("Unvalid index is out of range");
		}

		//  Return a reference to the data at the first element of the vector
		reference front(void) { return *begin(); }
		const_reference front() const { return *begin; }
		//  Return a reference to the data at the last element of the vector
		reference back(void) { return *(end() - 1); }
		const_reference back(void) const { return *(end() - 1); }

		/*************************************************************
		 * Modifiers
		*************************************************************/
		// Creates an element at the end of the vector and assigns the given data to it. 
		void push_back(const value_type& x) {
			if (_size == _capacity)
				reserve(new_cap(_size + 1));
			_alloc.construct(_array + _size, x);
			++_size;
		}

		// Removes last element
		void pop_back(void) { _alloc.destroy(&_array[--_size]); } 

		iterator insert(iterator position, const value_type& x) {
			difference_type	pos = position - begin();
			insert(position, 1, x);
			return begin() + pos;
		}

    	// This function will insert a copy of the given value before
    	// the specified location.  Note that this kind of operation
    	// could be expensive for a vector and if it is frequently
    	// used the user should consider using std::list.
		void insert(iterator position, size_type n, const value_type& x) {
			difference_type	pos = position - begin();

			if (_size + n > _capacity)
				reserve(new_cap(_size + n));
 			for (size_type i(_size - 1 + n); i >= pos + n; --i)
			{
				_alloc.construct(_array + i, _array[i - 1]);
				_alloc.destroy(&_array[i - 1]);
			}
			for (size_type j(pos); j < pos + n; ++j)
				_alloc.construct(_array + j, x);
			_size += n;
		}

		// This function will insert copies of the data in the range
    	// [__first,__last) into the vector before the location specified by position
		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = 0) {
			difference_type	pos = position - begin();
			size_type	n = 0;
			for (InputIterator it(first); it != last; ++it)
				++n;

			if (_size + n > _capacity)
				reserve(new_cap(_size + n));
 			for (size_type i(_size - 1 + n); i >= pos + n; --i)
			{
				_alloc.construct(_array + i, _array[i - 1]);
				_alloc.destroy(&_array[i - 1]);
			}
			for (size_type j(pos); j < pos + n; ++j)
			{
				_alloc.construct(_array + j, *first);
				++first;
			}
			_size += n;
		}

    	// This function will erase the element at the given position and thus
    	// shorten the %vector by one.	
    	// Note This operation could be expensive and if it is
    	// frequently used the user should consider using std::list.
    	// The user is also cautioned that this function only erases
    	// the element, and that if the element is itself a pointer,
    	// the pointed-to memory is not touched in any way.  Managing
    	// the pointer is the user's responsibility.
		iterator erase(iterator position) {
			difference_type	pos = position - begin();
			
			_alloc.destroy(_array + pos);
			--_size;
			for (size_type i(pos); i < _size; ++i)
			{
				_alloc.construct(_array + i, _array[i + 1]);
				_alloc.destroy(&_array[i + 1]);
			}
			return begin() + pos;
		}

		iterator erase(iterator first, iterator last)
		{
			difference_type	n = last - first;

			while (first != end() - n)
			{
				*first = first[n];
				++first;
			}
			while (first != end())
			{
				_alloc.destroy(first.base());
				++first;
			}
			_size -= n;
			return last - n; 
		}

		// Swaps data with another vector
		void swap(vector& x)
		{
			ft::swap(_alloc, x._alloc);
			ft::swap(_capacity, x._capacity);
			ft::swap(_array, x._array);
			ft::swap(_size, x._size);
		}

		// Erases all the elements. Note that this function only erases the
    	// elements, and that if the elements themselves are pointers, the
		// pointed-to memory is not touched in any way. Managing the pointer is
    	// the user's responsibility.
		void clear(void) {
			for (size_type i(0); i < _size; ++i)
				_alloc.destroy(_array + i);
			_size = 0;
		}


	private:

		allocator_type	_alloc;
		size_type		_capacity;
		value_type		*_array;
		size_type		_size;

		// When using assign() or insert(), vectors from STL can allocate more than needed
		size_type		new_cap(size_type n)
		{
			size_type	i;

			i = 1;
			while (i < n) { i *= 2;	}
			return i;
		}
	}; // End of vector


	/*************************************************************
	 *  Relational operators
	*************************************************************/
	template <class T, class Alloc>
	bool	operator== (const vector<T,Alloc>& l, const vector<T,Alloc>& r)
	{
		if (l.size() != r.size()) return false;
		return ft::equal(l.begin(), l.end(), r.begin());
	}

	template <class T, class Alloc>
	bool	operator!= (const vector<T,Alloc>& l, const vector<T,Alloc>& r) { return !(l == r); }

	template <class T, class Alloc>
	bool	operator< (const vector<T,Alloc>& l, const vector<T,Alloc>& r ) {
		return ft::lexicographical_compare(l.begin(), l.end(), r.begin(), r.end());
	}

	template <class T, class Alloc>
	bool	operator<= (const vector<T,Alloc>& l, const vector<T,Alloc>& r ) { return !(r < l); }

	template <class T, class Alloc>
	bool	operator> (const vector<T,Alloc>& l, const vector<T,Alloc>& r )	{ return r < l; }

	template <class T, class Alloc>
	bool	operator>= (const vector<T,Alloc>& l, const vector<T,Alloc>& r )	 { return !(l < r); }

	// See ft::vector::swap().
	template <class T, class Alloc>
	void swap(vector<T,Alloc>& x, vector<T,Alloc>& y) { x.swap(y); }

} // namespace ft

#endif /* _FT_VECTOR_HPP */
