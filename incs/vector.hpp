#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
# include <iterator>

# include "algorithm.hpp"
# include "type_traits.hpp"
# include "iterator.hpp"
# include "utility.hpp"

namespace ft {
	
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
		* construct/copy/destroy
		*************************************************************/
		// empty container constructor (default constructor)
		explicit vector(const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _capacity(0), _array(0), _size(0) {}

		// fill constructor
		explicit vector(size_type n, const value_type& val = value_type(),
			const allocator_type& alloc = allocator_type())
			:  _alloc(alloc), _capacity(0), _array(0), _size(0) {
			_capacity = n;
			_array = _alloc.allocate(n);
			
			for (size_type i = 0; i < n; ++i)
				_alloc.construct(_array + i, val);
		}

		// range constructor
		template <class InputIterator>
		vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _capacity(0), _array(0), _size(0) {
				
		}

		// copy constructor
		vector(const vector& x): _alloc(x._alloc), _capacity(x._capacity), _array(_alloc.allocate(x._capacity)), _size(x._size) { assign(x.begin(), x.end()); }

		// destructor
		~vector() {
			clear();
			_alloc.deallocate(_array, _capacity);
		}


		/*************************************************************
		* Assigning operator
		*************************************************************/
		vector& operator=(const vector& x) {
			if (this != &x) assign(x.begin(), x.end());
			return *this;
		}

		template <class InputIterator>
		void assign(InputIterator first, InputIterator last,
			typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = 0) {
			size_type	n = 0;
			for (InputIterator it(first); it != last; ++it)
				++n;

			clear();
			if (n > _capacity) reserve(n);
			for (size_type i(0); i < n; ++i)
				_alloc.construct(_array + i, *first++);
			_size = n;
		}

		void assign(size_type n, const T& u) {
			clear();
			if (n > _capacity) reserve(n);
			for (size_type i(0); i < n; ++i)
				_alloc.construct(_array + i, u);
			_size = n;
		}

		allocator_type get_allocator() const { return _alloc; }


		/*************************************************************
		* Iterators
		*************************************************************/
		iterator begin(void) { return iterator(_array); }
		const_iterator begin(void) const { return const_iterator(_array); }

		iterator end(void) { return iterator(_array + _size); }
		const_iterator end() const { return const_iterator(_array + _size); }

		reverse_iterator rbegin(void) { return reverse_iterator(end()); }
		const_reverse_iterator rbegin(void) const { return const_reverse_iterator(end()); }

		reverse_iterator rend(void) { return reverse_iterator(begin()); }
		const_reverse_iterator rend(void) const { return const_reverse_iterator(begin()); }


		/*************************************************************
		* Capacity
		*************************************************************/
		size_type size(void) const {
			return _size;
		}

		size_type max_size(void) const {
			return _alloc.max_size();
		}

		void resize(size_type sz, value_type c = value_type()) {
			while (sz > _size) push_back(c);
			while (sz < _size) pop_back();
		}

		size_type capacity(void) const {
			return _capacity;
		}

		bool empty(void) const {
			return _size == 0;
		}

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
		reference operator[](size_type n) { return _array[n]; }
		const_reference operator[](size_type n) const { return _array[n]; }

		const_reference at(size_type n) const {
			if (n < _size) return _array[n];
			throw std::out_of_range("Unvalid index is out of range");
		}
		reference at(size_type n) {
			if (n < _size) return _array[n];
			throw std::out_of_range("Unvalid index is out of range");
		}

		reference front(void) { return _array[0]; }
		const_reference front() const { return _array[0]; }

		reference back(void) { return _array[_size - 1]; }
		const_reference back(void) const { return _array[_size - 1]; }

		/*************************************************************
		* Modifiers
		*************************************************************/
		void push_back(const value_type& x) {
			if (_size == _capacity)
				reserve(new_cap(_size + 1));
			_alloc.construct(_array + _size, x);
			++_size;
		}

		void pop_back(void) { _alloc.destroy(&_array[--_size]); } 

		iterator insert(iterator position, const value_type& x) {
			difference_type	pos = position - begin();
			insert(position, 1, x);
			return begin() + pos;
		}

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

		void swap(vector& x)
		{
			ft::swap(_alloc, x._alloc);
			ft::swap(_capacity, x._capacity);
			ft::swap(_array, x._array);
			ft::swap(_size, x._size);
		}

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
	* Relational operators
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

	// swap
	template <class T, class Alloc>
	void swap(vector<T,Alloc>& x, vector<T,Alloc>& y) { x.swap(y); }

} // End of namespace ft

#endif