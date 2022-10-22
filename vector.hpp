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
			:  _array(0), _size(0), _capacity(0), _alloc(alloc) {}

		// fill constructor
		explicit vector(size_type n, const value_type& val = value_type(),
			const allocator_type& alloc = allocator_type())
			:  _array(0), _size(0), _capacity(0), _alloc(alloc) {
			_array = _alloc.allocate(n);
			_size = 0;
			_capacity = n;
			
			for (size_type i = 0; i < n; ++i)
				_alloc.construct(_array + i, val);
		}

		// range constructor
		template <class InputIterator>
		vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type()),
			:  _array(0), _size(0), _capacity(0), _alloc(alloc) {
			
		}

		// copy constructor
		vector(const vector<T,Allocator>& x);

		~vector();
		vector<T,Allocator>& operator=(const vector<T,Allocator>& x);
		template <class InputIterator>
		void assign(InputIterator first, InputIterator last);
		void assign(size_type n, const T& u);
		allocator_type get_allocator() const;
		// iterators:
		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;
		reverse_iterator rbegin();
		const_reverse_iterator rbegin() const;
		reverse_iterator rend();
		const_reverse_iterator rend() const;
		// 23.2.4.2 capacity:
		size_type size() const;
		size_type max_size() const;
		void resize(size_type sz, T c = T());
		size_type capacity() const;
		bool empty() const;
		void reserve(size_type n);
		// element access:
		reference operator[](size_type n);
		const_reference operator[](size_type n) const;
		const_reference at(size_type n) const;
		reference at(size_type n);
		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;
		// 23.2.4.3 modifiers:
		void push_back(const T& x);
		void pop_back();
		iterator insert(iterator position, const T& x);
		void insert(iterator position, size_type n, const T& x);
		template <class InputIterator>
		void insert(iterator position,
		InputIterator first, InputIterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		void swap(vector<T,Allocator>&);
		void clear();
		};
