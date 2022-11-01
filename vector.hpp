#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <iostream>

# include <memory>
# include <iterator>

namespace ft {
	
	template <class T, class Alloc = std::allocator<T> >
	
	class vector {
	
	public:

		// types:
		typedef Alloc										allocator_type;
		typedef typename allocator_type::reference			reference;
		typedef typename allocator_type::const_reference	const_reference;
		typedef T*											iterator;
		typedef const T*									const_iterator;
		typedef std::size_t									size_type;
		typedef std::ptrdiff_t								difference_type;
		typedef T											value_type;
		typedef typename allocator_type::pointer			pointer;
		typedef typename allocator_type::const_pointer		const_pointer;
		typedef std::reverse_iterator<iterator>				reverse_iterator;
		typedef std::reverse_iterator<const_iterator>		const_reverse_iterator;


		// 23.2.4.1 construct/copy/destroy:
		
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
		vector(const vector& x): _alloc(x.alloc), _capacity(x._capacity), _array(_alloc.allocate(x._capacity)), _size(x._size) { assign(x.begin(), x.end()); }

		~vector() {
			clear();
			_alloc.deallocate(_array, _capacity);
		}

		vector& operator=(const vector& x) {
			if (this != &x) assign(x.begin(), x.end());
			return *this;
		}

		template <class InputIterator>
		void assign(InputIterator first, InputIterator last/*,
			typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = nullptr*/) {
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

		// iterators:
		iterator begin(void) { return iterator(_array); }
		const_iterator begin(void) const { return const_iterator(_array); }

		iterator end(void) { return iterator(_array + _size); }
		const_iterator end() const { return const_iterator(_array + _size); }

		reverse_iterator rbegin(void) { return reverse_iterator(end()); }
		const_reverse_iterator rbegin(void) const { return const_reverse_iterator(end()); }

		reverse_iterator rend(void) { return reverse_iterator(begin()); }
		const_reverse_iterator rend(void) const { return const_reverse_iterator(begin()); }


		// 23.2.4.2 capacity:
		size_type size(void) const {
			return _size;
		}

		size_type max_size(void) const {
			return _alloc.max_size();
		}

		void resize(size_type sz, T c = T());

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

		// element access:
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

		// 23.2.4.3 modifiers:
		void push_back(const value_type& x) {
			if (_size == _capacity)
				reserve(_size + 1);
			_alloc.construct(_array + _size, x);
			++_size;
		}

		void pop_back(void) { _alloc.destroy(&_array[--_size]); }

		iterator insert(iterator position, const value_type& x) {
			size_type	pos = position - begin();
			insert(position, 1, x);
			return begin() + pos;
		}
/*
		void insert(iterator position, size_type n, const value_type& x) {
			size_type	pos = position - begin();

			if (_size + n) > _capacity)
				reserve(_size + n);
 			for (size_type i(_size - 1 + n); i >= pos + n; --i)
				_array[i] = _array[i - 1];
			for (size_type j(pos); j < pos + n; ++j)
				_array[i] = x;
			_size += n;
		}
*/
		void insert(iterator position, size_type n, const value_type& x) {
			size_type	pos = position - begin();

			if (_size + n > _capacity)
				reserve(_size + n);
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
		void insert(iterator position, InputIterator first, InputIterator last/*,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = nullptr*/) {
			size_type	pos = position - begin();
			size_type	n = 0;
			for (InputIterator it(first); it != last; ++it)
				++n;

			if (_size + n > _capacity)
				reserve(_size + n);
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
			size_type	pos = position - begin();
			
			_alloc.destroy(_array + pos);
			for (size_type i(pos); i < _size; ++i)
			{
				_alloc.construct(_array + i, _array[i + 1]);
				_alloc.destroy(_array[i + 1]);
			}
			return _array + pos;
		}

		iterator erase(iterator first, iterator last)
		{
			size_type	n = last - first;

			for (; first != end() - n; ++first)
				*first = first[n];
			for (; first != end(); ++first)
				_alloc.destroy(first);

			return first; 
		}

		void swap(vector<T,Alloc>&);

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

	};
}

#endif
