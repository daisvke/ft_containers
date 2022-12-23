#ifndef _FT_TREE_HPP
# define _FT_TREE_HPP

# include <memory>

# include "utility.hpp"
# include "tree_cc.hpp"

/*************************************************************
 * A Red-Black Tree implementation.
 
 * Designed for use in implementing STL associative containers
 	(set and map, etc)

 * The header cell is maintained with links not only to the root
	but also to the leftmost node of the tree, to enable constant
	time begin(), and to the rightmost node of the tree, to enable
	linear time performance when used with the generic set algorithms

 * When a node being deleted has two children its successor node
	is relinked into its place, rather than copied, so that the only
	iterators invalidated are those referring to the deleted node.
*************************************************************/

namespace ft {

	  // Helper type offering value initialization guarantee on the compare functor.
  	template<typename Key_compare>
    struct	rb_tree_key_compare
    {
		Key_compare	_key_compare;

		rb_tree_key_compare() : _key_compare() {}

		rb_tree_key_compare(const Key_compare& comp) : _key_compare(comp) {}
	};

	
	/*************************************************************
	 * Tree header
	
	 * Helper type to manage default initialization of node count and header
	*************************************************************/
	struct	rb_tree_header
	{
		rb_tree_node_base	_header;
		std::size_t			_node_count; // Keeps track of size of tree.

		rb_tree_header() { _header._color = _red; reset(); }

		void	move_data(rb_tree_header& from) {
			_header._color = from._header._color;
			_header._parent =from._header._parent;
			_header._left = from._header._left;
			_header._right = from._header._right;
			_header._parent->_parent = &_header;
			_node_count = from._node_count;
			from.reset();
		}

		void	reset()
		{
			_header._parent = 0;
			_header._left = &_header;
			_header._right = &_header;
			_node_count = 0;
		}
	};

	/*************************************************************
	 * Red-Black Tree node
	*************************************************************/
	template<typename Val>
	struct rb_tree_node : public rb_tree_node_base {
		typedef rb_tree_node<Val>* link_type;
		Val	Value_field;

		Val			*valptr() { return &Value_field; }
		const Val	*valptr() const { return &Value_field; }
	};


	/*************************************************************
	|* Red-Black Tree iterator
	*************************************************************/
	template <typename T>
	struct	rb_tree_iterator
	{			
		/*************************************************************
		 * Types
		*************************************************************/
		typedef std::ptrdiff_t					difference_type;
		typedef T								value_type;
		typedef value_type*						pointer;
		typedef value_type&						reference;
		typedef std::bidirectional_iterator_tag	iterator_category;

		typedef rb_tree_iterator<T>				self;
		typedef rb_tree_node_base::base_ptr		base_ptr;
		typedef rb_tree_node<T>					*link_type;
		
		/*************************************************************
		 * Construct/Copy/Destroy
		*************************************************************/
		rb_tree_iterator() : _node() {}
		explicit rb_tree_iterator(base_ptr x) : _node(x) {}

		/*************************************************************
		 * Accessing operators
		*************************************************************/
		reference	operator*(void) const { return *static_cast<link_type>(_node)->valptr(); }
		reference	operator->(void) const { return static_cast<link_type>(_node)->valptr(); }

		/*************************************************************
		 * Incrementing operators
		*************************************************************/
		// Get the next node in key value order
		self		&operator++() { _node = rb_tree_increment(_node); return *this; }
		self		operator++(int) {
			self	tmp = *this;
			_node = rb_tree_increment(_node);
			return tmp;
		}
		
		// Get the previous node in key value order
		self		&operator--() { _node = rb_tree_decrement(_node); return *this; }
		self		operator--(int) {
			self	tmp = *this;
			_node = rb_tree_decrement(_node);
			return tmp;
		}

		/*************************************************************
		 * Boolean operators
		*************************************************************/
		friend bool	operator==(const self& x, const self& y)
		{ return x._node == y._node; }

		friend bool	operator!=(const self& x, const self& y)
		{ return x._node != y._node; }

		base_ptr	_node;
	}; // rb_tree_iterator


	/*************************************************************
	|* Red-Black Tree const iterator
	*************************************************************/
	template <typename T>
	struct	rb_tree_const_iterator
	{			
		/*************************************************************
		 * Types
		*************************************************************/
		typedef std::ptrdiff_t						difference_type;
		typedef T									value_type;
		typedef const value_type*					pointer;
		typedef const value_type&					reference;
		typedef std::bidirectional_iterator_tag		iterator_category;

		typedef rb_tree_iterator<T>					iterator;

		typedef rb_tree_const_iterator<T>			self;
		typedef rb_tree_node_base::const_base_ptr	base_ptr;
		typedef rb_tree_node<T>						*link_type;
		
		/*************************************************************
		 * Construct/Copy/Destroy
		*************************************************************/
		rb_tree_const_iterator() : _node() {}
		explicit rb_tree_const_iterator(base_ptr x) : _node(x) {}
		explicit rb_tree_const_iterator(const iterator& it) : _node(it._node) {}

		iterator	_const_cast() const
		{ return iterator(const_cast<typename iterator::base_ptr>(_node)); }

		/*************************************************************
		 * Accessing operators
		*************************************************************/
		reference	operator*(void) const { return *static_cast<link_type>(_node)->valptr(); }
		reference	operator->(void) const { return static_cast<link_type>(_node)->valptr(); }

		/*************************************************************
		 * Incrementing operators
		*************************************************************/
		// Get the next node in key value order
		self		&operator++() { _node = rb_tree_increment(_node); return *this; }
		self		operator++(int) {
			self	tmp = *this;
			_node = rb_tree_increment(_node);
			return tmp;
		}
		
		// Get the previous node in key value order
		self		&operator--() { _node = rb_tree_decrement(_node); return *this; }
		self		operator--(int) {
			self	tmp = *this;
			_node = rb_tree_decrement(_node);
			return tmp;
		}

		/*************************************************************
		 * Boolean operators
		*************************************************************/
		friend bool	operator==(const self& x, const self& y)
		{ return x._node == y._node; }

		friend bool	operator!=(const self& x, const self& y)
		{ return x._node != y._node; }

		base_ptr	_node;
	}; // rb_tree_const_iterator


	/*************************************************************
	 * Red-Black Tree class
	*************************************************************/
	template<typename Key, typename Val, typename KeyOfValue,
	   typename Compare, typename Alloc = std::allocator<Val> >
    class rb_tree
	{
    	typedef typename __gnu_cxx::__alloc_traits<Alloc>::template
		rebind<rb_tree_node<Val> >::other				node_allocator;

    	typedef __gnu_cxx::__alloc_traits<node_allocator>	alloc_traits;

		protected:

			typedef rb_tree_node_base* 			base_ptr;
			typedef const rb_tree_node_base* 	const_base_ptr;
			typedef rb_tree_node<Val>* 			link_type;
			typedef const rb_tree_node<Val>*	const_link_type;


		private:

			struct alloc_node
			{
				alloc_node(rb_tree& t) : _t(t) { }

				template<typename Arg>
				link_type	operator()(Arg arg) const { return _t._create_node(arg); }

				private:
					rb_tree& _t;
			};
		

		public:

			typedef Key 				key_type;
			typedef Val 				value_type;
			typedef value_type* 		pointer;
			typedef const value_type* 	const_pointer;
			typedef value_type& 		reference;
			typedef const value_type& 	const_reference;
			typedef std::size_t 		size_type;
			typedef std::ptrdiff_t 		difference_type;
			typedef Alloc 				allocator_type;

			node_allocator&		get_node_allocator() { return _impl; }

			const node_allocator&	get_node_allocator() const { return _impl; }

			allocator_type			get_allocator() const
			{ return allocator_type(get_node_allocator()); }


		protected:

      		link_type	get_node()
			{ return alloc_traits::allocate(get_node_allocator(), 1); }

			void		put_node(link_type p)
			{ alloc_traits::deallocate(get_node_allocator(), p, 1); }

			void		construct_node(link_type node, const value_type& x)
			{
				try
				{ get_allocator().construct(node->valptr(), x); }
				catch (...)
				{ put_node(node); throw; }
      		}

			link_type	create_node(const value_type& x)
      		{
				link_type tmp = get_node();
				construct_node(tmp, x);
				return tmp;
      		}

	     	void	destroy_node(link_type p)
			{ get_allocator().destroy(p->valptr()); }

			void	drop_node(link_type p)
			{
				destroy_node(p);
				put_node(p);
			}

			template<bool MoveValue, typename NodeGen>
			link_type	clone_node(link_type x, NodeGen& node_gen)
			{
				link_type	tmp = node_gen( *x->_valptr());

				tmp->_color = x->_color;
				tmp->_left = 0;
				tmp->_right = 0;
				return tmp;
			}
		
			template<typename Key_compare>
			struct rb_tree_impl
				: public node_allocator
				, public rb_tree_key_compare<Key_compare>
				, public rb_tree_header
			{
				typedef rb_tree_key_compare<Key_compare> base_key_compare;

				rb_tree_impl() : node_allocator() {}

				rb_tree_impl(const rb_tree_impl& x)
				: node_allocator(alloc_traits::select_on_copy(x))
				, base_key_compare(x._key_compare), rb_tree_header() {}

				rb_tree_impl(const Key_compare& comp, const node_allocator& a)
				: node_allocator(a), base_key_compare(comp) {}
			};
			
			rb_tree_impl<Compare> _impl;

			base_ptr&		root() { return _impl._header._parent; }
			const_base_ptr	root() const { return _impl._header._parent; }

			base_ptr&		leftmost() { return _impl._header._left; }
			const_base_ptr	leftmost() const { return _impl._header._left; }

			base_ptr&		rightmost() { return _impl._header._right; }
			const_base_ptr	rightmost() const { return _impl._header._right; }

			link_type		_mbegin() const 
			{ return static_cast<link_type>(_impl._header._parent); }
			link_type		_begin() { return _mbegin(); }
			const_link_type	_begin() const
			{ return static_cast<const_link_type>(_impl._header._parent); }

			base_ptr		_end() { return &_impl._header; }
			const_base_ptr	_end() const { return &_impl._header; }

			static const Key&	key(const_link_type x)
			{ return KeyOfValue()(*x->_valptr()); }

			static link_type		left(base_ptr x)
			{ return static_cast<link_type>(x->_left); }
			static const_link_type	left(const_base_ptr x)
			{ return static_cast<const_link_type>(x->_left); }

			static link_type	right(base_ptr x)
			{ return static_cast<link_type>(x->_right); }
			static const_link_type	right(const_base_ptr x)
			{ return static_cast<const_link_type>(x->_right); }

			static const Key&	key(const_base_ptr x)
			{ return key(static_cast<const_link_type>(x)); }

			static base_ptr	minimum(base_ptr x)
			{ return rb_tree_node_base::minimum(x); }

			static const_base_ptr	minimum(const_base_ptr x)
			{ return rb_tree_node_base::minimum(x); }

			static base_ptr	maximum(base_ptr x)
			{ return rb_tree_node_base::maximum(x); }

			static const_base_ptr	maximum(const_base_ptr x)
			{ return rb_tree_node_base::maximum(x); }


		public:

			typedef rb_tree_iterator<value_type>			iterator;
			typedef rb_tree_const_iterator<value_type>		const_iterator;

			typedef ft::reverse_iterator<iterator>			reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

			ft::pair<base_ptr, base_ptr> get_insert_unique_pos(const key_type& k);
			ft::pair<base_ptr, base_ptr> get_insert_equal_pos(const key_type& k);
			ft::pair<base_ptr, base_ptr> get_insert_hint_unique_pos(const_iterator pos,
							const key_type& k);
			ft::pair<base_ptr, base_ptr> get_insert_hint_equal_pos(const_iterator pos,
						const key_type& k);


		private:

		    template<typename _NodeGen>
			iterator
			insert_(base_ptr x, base_ptr y, const value_type& v, _NodeGen&);

			// Insertion hints in associative containers.
			iterator	insert_lower(base_ptr y, const value_type& v);
			iterator	insert_equal_lower(const value_type& x);

			enum { as_lvalue, as_rvalue };


			template<bool MoveValues, typename NodeGen>
			link_type	copy(link_type, base_ptr, NodeGen&);

			template<bool MoveValues, typename NodeGen>
			link_type	copy(const rb_tree& x, NodeGen& gen)
			{
				link_type root = copy<MoveValues>(x._mbegin(), _end(), gen);
				leftmost() = minimum(root);
				rightmost() = maximum(root);
				_impl._node_count = x._impl._node_count;
				return root;
			}

			link_type	copy(const rb_tree& x)
			{
				alloc_node	an(*this);
				return copy<as_lvalue>(x, an);
			}

			void	erase(link_type x);

			iterator		lower_bound(link_type x, base_ptr y, const Key& k);
			const_iterator	lower_bound(const_link_type x, const_base_ptr y,
					const Key& k) const;

			iterator		upper_bound(link_type x, base_ptr y, const Key& k);
			const_iterator	upper_bound(const_link_type x, const_base_ptr y,
					const Key& k) const;


		public:

		    rb_tree() {}

			rb_tree(const Compare& comp, const allocator_type& a = allocator_type())
			: _impl(comp, node_allocator(a)) { }

			rb_tree(const rb_tree& x) : _impl(x._impl)
			{ if (x.root() != 0) root() = copy(x); }

			~rb_tree() { erase(begin()); }

			rb_tree&	operator=(const rb_tree& x);

			// Accessors.
			Compare	key_comp() const { return _impl._key_compare; }

			iterator				begin()
			{ return iterator(_impl._header._left); }
			const_iterator			begin() const
			{ return const_iterator(this->_impl._header._left); }

			iterator				end() { return iterator(&_impl._header); }
			const_iterator			end() const
			{ return const_iterator(&_impl._header); }

			reverse_iterator		rbegin() { return reverse_iterator(end()); }
			const_reverse_iterator	rbegin() const
			{ return const_reverse_iterator(end()); }

			reverse_iterator		rend() { return reverse_iterator(begin()); }
			const_reverse_iterator	rend() const
			{ return const_reverse_iterator(begin()); }

			bool		empty() const { return _impl._node_count == 0; }

			size_type	size() const { return _impl._node_count; }

			size_type	max_size() const
			{ return alloc_traits::max_size(get_node_allocator()); }

			void		swap(rb_tree& t);

			pair<iterator, bool>	insert_unique(const value_type& x);
			iterator				insert_equal(const value_type& x);

			template<typename NodeGen>
			iterator	insert_unique_(const_iterator pos, const value_type& x,
						NodeGen&);
			iterator	insert_unique_(const_iterator pos, const value_type& x)
			{
				alloc_node an(*this);
				return insert_unique_(pos, x, an);
			}

			template<typename NodeGen>
			iterator	insert_equal_(const_iterator pos, const value_type& x,
						NodeGen&);
			iterator	insert_equal_(const_iterator pos, const value_type& x)
			{
				alloc_node an(*this);
				return insert_equal_(pos, x, an);
			}

			template<typename _InputIterator>
			void	insert_range_unique(_InputIterator first, _InputIterator last)
			{
				alloc_node an(*this);
				for (; first != last; ++first)
					insert_unique_(end(), *first, an);
			}

			template<typename _InputIterator>
			void	insert_range_equal(_InputIterator first, _InputIterator last)
			{
				alloc_node an(*this);
				for (; first != last; ++first)
					insert_equal_(end(), *first, an);
			}

		private:
			void		erase_aux(const_iterator position);
			void		erase_aux(const_iterator first, const_iterator last);

      		void		erase(iterator position)
			{ erase_aux(position); }

			void		erase(const_iterator position)
			{ erase_aux(position); }
			
			size_type	erase(const key_type& x);
			void		erase(iterator first, iterator last)
		    { erase_aux(first, last); }
			
			void		erase(const_iterator first, const_iterator last)
      		{ erase_aux(first, last); }

			void	clear()
      		{ erase(_begin()); _impl._reset(); }

			// Set operations.
			iterator		find(const key_type& __k);
			const_iterator	find(const key_type& __k) const;

			size_type		count(const key_type& __k) const;

			iterator		lower_bound(const key_type& __k)
			{ return _lower_bound(_begin(), _end(), __k); }

			const_iterator	lower_bound(const key_type& __k) const
			{ return _lower_bound(_begin(), _end(), __k); }

			iterator		upper_bound(const key_type& __k)
			{ return _upper_bound(_begin(), _end(), __k); }

			const_iterator	upper_bound(const key_type& __k) const
			{ return _upper_bound(_begin(), _end(), __k); }

			pair<iterator, iterator>	equal_range(const key_type& __k);
			
			pair<const_iterator, const_iterator>
			equal_range(const key_type& __k) const;

			friend bool	operator==(const rb_tree& __x, const rb_tree& __y)
			{
				return __x.size() == __y.size()
					&& ft::equal(__x.begin(), __x.end(), __y.begin());
			}

			friend bool	operator<(const rb_tree& __x, const rb_tree& __y)
			{ return ft::lexicographical_compare(__x.begin(), __x.end(),
								__y.begin(), __y.end()); }
			};

			template<typename _Key, typename _Val, typename _KeyOfValue,
			typename _Compare, typename _Alloc>
			inline void
			swap(rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
				rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y)
			{ __x.swap(__y); }
/*
			template<typename _Key, typename _Val, typename _KeyOfValue,
			typename _Compare, typename _Alloc>
			rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>&
			rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
			operator=(const rb_tree& __x)
			{
				if (this != &__x)
				{
					_Reuse_or_alloc_node __roan(*this);
					_impl._reset();
					_impl._key_compare = __x._impl._key_compare;
					if (__x._root() != 0)
						_root() = _copy<__as_lvalue>(__x, __roan);
				}

				return *this;
			}
*/

  			template<typename _Key, typename _Val, typename _KeyOfValue,
			typename _Compare, typename _Alloc>
			template<typename _NodeGen>
			typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
			rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
			insert_(base_ptr __x, base_ptr __p, const _Val& __v, _NodeGen& __node_gen)
			{
				bool __insert_left = (__x != 0 || __p == _end()
							|| _impl._key_compare(_KeyOfValue()(__v),
										key(__p)));

				link_type __z = __node_gen(__v);

				rb_tree_insert_and_rebalance(__insert_left, __z, __p, _impl._header);
				++_impl._node_count;
				return iterator(__z);
			}
/*
  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _insert_lower(_Base_ptr __p, const _Val& __v)
    {
      bool __insert_left = (__p == _end()
			    || !_impl._key_compare(_S_key(__p),
						       _KeyOfValue()(__v)));

      _Link_type __z = _create_node(_GLIBCXX_FORWARD(_Arg, __v));

      rb_tree_insert_and_rebalance(__insert_left, __z, __p,
				    this->_impl._header);
      ++_impl._node_count;
      return iterator(__z);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
#if __cplusplus >= 201103L
    template<typename _Arg>
#endif
    typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
#if __cplusplus >= 201103L
    _insert_equal_lower(_Arg&& __v)
#else
    _insert_equal_lower(const _Val& __v)
#endif
    {
      _Link_type __x = _begin();
      _Base_ptr __y = _end();
      while (__x != 0)
	{
	  __y = __x;
	  __x = !_impl._key_compare(_S_key(__x), _KeyOfValue()(__v)) ?
		_S_left(__x) : _S_right(__x);
	}
      return _insert_lower(__y, _GLIBCXX_FORWARD(_Arg, __v));
    }

  template<typename _Key, typename _Val, typename _KoV,
	   typename _Compare, typename _Alloc>
    template<bool _MoveValues, typename _NodeGen>
      typename rb_tree<_Key, _Val, _KoV, _Compare, _Alloc>::_Link_type
      rb_tree<_Key, _Val, _KoV, _Compare, _Alloc>::
      _copy(_Link_type __x, _Base_ptr __p, _NodeGen& __node_gen)
      {
	// Structural copy. __x and __p must be non-null.
	_Link_type __top = _clone_node<_MoveValues>(__x, __node_gen);
	__top->_parent = __p;

	__try
	  {
	    if (__x->_right)
	      __top->_right =
		_copy<_MoveValues>(_S_right(__x), __top, __node_gen);
	    __p = __top;
	    __x = _S_left(__x);

	    while (__x != 0)
	      {
		_Link_type __y = _clone_node<_MoveValues>(__x, __node_gen);
		__p->_left = __y;
		__y->_parent = __p;
		if (__x->_right)
		  __y->_right = _copy<_MoveValues>(_S_right(__x),
						       __y, __node_gen);
		__p = __y;
		__x = _S_left(__x);
	      }
	  }
	__catch(...)
	  {
	    _erase(__top);
	    __throw_exception_again;
	  }
	return __top;
      }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    void
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _erase(_Link_type __x)
    {
      // Erase without rebalancing.
      while (__x != 0)
	{
	  _erase(_S_right(__x));
	  _Link_type __y = _S_left(__x);
	  _drop_node(__x);
	  __x = __y;
	}
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    typename rb_tree<_Key, _Val, _KeyOfValue,
		      _Compare, _Alloc>::iterator
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _lower_bound(_Link_type __x, _Base_ptr __y,
		   const _Key& __k)
    {
      while (__x != 0)
	if (!_impl._key_compare(_S_key(__x), __k))
	  __y = __x, __x = _S_left(__x);
	else
	  __x = _S_right(__x);
      return iterator(__y);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    typename rb_tree<_Key, _Val, _KeyOfValue,
		      _Compare, _Alloc>::const_iterator
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _lower_bound(_Const_Link_type __x, _Const_Base_ptr __y,
		   const _Key& __k) const
    {
      while (__x != 0)
	if (!_impl._key_compare(_S_key(__x), __k))
	  __y = __x, __x = _S_left(__x);
	else
	  __x = _S_right(__x);
      return const_iterator(__y);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    typename rb_tree<_Key, _Val, _KeyOfValue,
		      _Compare, _Alloc>::iterator
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _upper_bound(_Link_type __x, _Base_ptr __y,
		   const _Key& __k)
    {
      while (__x != 0)
	if (_impl._key_compare(__k, _S_key(__x)))
	  __y = __x, __x = _S_left(__x);
	else
	  __x = _S_right(__x);
      return iterator(__y);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    typename rb_tree<_Key, _Val, _KeyOfValue,
		      _Compare, _Alloc>::const_iterator
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _upper_bound(_Const_Link_type __x, _Const_Base_ptr __y,
		   const _Key& __k) const
    {
      while (__x != 0)
	if (_impl._key_compare(__k, _S_key(__x)))
	  __y = __x, __x = _S_left(__x);
	else
	  __x = _S_right(__x);
      return const_iterator(__y);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    pair<typename rb_tree<_Key, _Val, _KeyOfValue,
			   _Compare, _Alloc>::iterator,
	 typename rb_tree<_Key, _Val, _KeyOfValue,
			   _Compare, _Alloc>::iterator>
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    equal_range(const _Key& __k)
    {
      _Link_type __x = _begin();
      _Base_ptr __y = _end();
      while (__x != 0)
	{
	  if (_impl._key_compare(_S_key(__x), __k))
	    __x = _S_right(__x);
	  else if (_impl._key_compare(__k, _S_key(__x)))
	    __y = __x, __x = _S_left(__x);
	  else
	    {
	      _Link_type __xu(__x);
	      _Base_ptr __yu(__y);
	      __y = __x, __x = _S_left(__x);
	      __xu = _S_right(__xu);
	      return pair<iterator,
			  iterator>(_lower_bound(__x, __y, __k),
				    _upper_bound(__xu, __yu, __k));
	    }
	}
      return pair<iterator, iterator>(iterator(__y),
				      iterator(__y));
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    pair<typename rb_tree<_Key, _Val, _KeyOfValue,
			   _Compare, _Alloc>::const_iterator,
	 typename rb_tree<_Key, _Val, _KeyOfValue,
			   _Compare, _Alloc>::const_iterator>
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    equal_range(const _Key& __k) const
    {
      _Const_Link_type __x = _begin();
      _Const_Base_ptr __y = _end();
      while (__x != 0)
	{
	  if (_impl._key_compare(_S_key(__x), __k))
	    __x = _S_right(__x);
	  else if (_impl._key_compare(__k, _S_key(__x)))
	    __y = __x, __x = _S_left(__x);
	  else
	    {
	      _Const_Link_type __xu(__x);
	      _Const_Base_ptr __yu(__y);
	      __y = __x, __x = _S_left(__x);
	      __xu = _S_right(__xu);
	      return pair<const_iterator,
			  const_iterator>(_lower_bound(__x, __y, __k),
					  _upper_bound(__xu, __yu, __k));
	    }
	}
      return pair<const_iterator, const_iterator>(const_iterator(__y),
						  const_iterator(__y));
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    void
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    swap(rb_tree& __t)
    _GLIBCXX_NOEXCEPT_IF(__is_nothrow_swappable<_Compare>::value)
    {
      if (_root() == 0)
	{
	  if (__t._root() != 0)
	    _impl._move_data(__t._impl);
	}
      else if (__t._root() == 0)
	__t._impl._move_data(_impl);
      else
	{
	  std::swap(_root(),__t._root());
	  std::swap(_leftmost(),__t._leftmost());
	  std::swap(_rightmost(),__t._rightmost());

	  _root()->_parent = _end();
	  __t._root()->_parent = __t._end();
	  std::swap(this->_impl._node_count, __t._impl._node_count);
	}
      // No need to swap header's color as it does not change.
      std::swap(this->_impl._key_compare, __t._impl._key_compare);

      _Alloc_traits::_S_on_swap(_get_Node_allocator(),
				__t._get_Node_allocator());
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    pair<typename rb_tree<_Key, _Val, _KeyOfValue,
			   _Compare, _Alloc>::_Base_ptr,
	 typename rb_tree<_Key, _Val, _KeyOfValue,
			   _Compare, _Alloc>::_Base_ptr>
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _get_insert_unique_pos(const key_type& __k)
    {
      typedef pair<_Base_ptr, _Base_ptr> _Res;
      _Link_type __x = _begin();
      _Base_ptr __y = _end();
      bool __comp = true;
      while (__x != 0)
	{
	  __y = __x;
	  __comp = _impl._key_compare(__k, _S_key(__x));
	  __x = __comp ? _S_left(__x) : _S_right(__x);
	}
      iterator __j = iterator(__y);
      if (__comp)
	{
	  if (__j == begin())
	    return _Res(__x, __y);
	  else
	    --__j;
	}
      if (_impl._key_compare(_S_key(__j._node), __k))
	return _Res(__x, __y);
      return _Res(__j._node, 0);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    pair<typename rb_tree<_Key, _Val, _KeyOfValue,
			   _Compare, _Alloc>::_Base_ptr,
	 typename rb_tree<_Key, _Val, _KeyOfValue,
			   _Compare, _Alloc>::_Base_ptr>
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _get_insert_equal_pos(const key_type& __k)
    {
      typedef pair<_Base_ptr, _Base_ptr> _Res;
      _Link_type __x = _begin();
      _Base_ptr __y = _end();
      while (__x != 0)
	{
	  __y = __x;
	  __x = _impl._key_compare(__k, _S_key(__x)) ?
		_S_left(__x) : _S_right(__x);
	}
      return _Res(__x, __y);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
#if __cplusplus >= 201103L
    template<typename _Arg>
#endif
    pair<typename rb_tree<_Key, _Val, _KeyOfValue,
			   _Compare, _Alloc>::iterator, bool>
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
#if __cplusplus >= 201103L
    _insert_unique(_Arg&& __v)
#else
    _insert_unique(const _Val& __v)
#endif
    {
      typedef pair<iterator, bool> _Res;
      pair<_Base_ptr, _Base_ptr> __res
	= _get_insert_unique_pos(_KeyOfValue()(__v));

      if (__res.second)
	{
	  _Alloc_node __an(*this);
	  return _Res(_insert_(__res.first, __res.second,
				 _GLIBCXX_FORWARD(_Arg, __v), __an),
		      true);
	}

      return _Res(iterator(__res.first), false);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
#if __cplusplus >= 201103L
    template<typename _Arg>
#endif
    typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
#if __cplusplus >= 201103L
    _insert_equal(_Arg&& __v)
#else
    _insert_equal(const _Val& __v)
#endif
    {
      pair<_Base_ptr, _Base_ptr> __res
	= _get_insert_equal_pos(_KeyOfValue()(__v));
      _Alloc_node __an(*this);
      return _insert_(__res.first, __res.second,
			_GLIBCXX_FORWARD(_Arg, __v), __an);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    pair<typename rb_tree<_Key, _Val, _KeyOfValue,
			   _Compare, _Alloc>::_Base_ptr,
	 typename rb_tree<_Key, _Val, _KeyOfValue,
			   _Compare, _Alloc>::_Base_ptr>
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _get_insert_hint_unique_pos(const_iterator __position,
				  const key_type& __k)
    {
      iterator __pos = __position._const_cast();
      typedef pair<_Base_ptr, _Base_ptr> _Res;

      // end()
      if (__pos._node == _end())
	{
	  if (size() > 0
	      && _impl._key_compare(_S_key(_rightmost()), __k))
	    return _Res(0, _rightmost());
	  else
	    return _get_insert_unique_pos(__k);
	}
      else if (_impl._key_compare(__k, _S_key(__pos._node)))
	{
	  // First, try before...
	  iterator __before = __pos;
	  if (__pos._node == _leftmost()) // begin()
	    return _Res(_leftmost(), _leftmost());
	  else if (_impl._key_compare(_S_key((--__before)._node), __k))
	    {
	      if (_S_right(__before._node) == 0)
		return _Res(0, __before._node);
	      else
		return _Res(__pos._node, __pos._node);
	    }
	  else
	    return _get_insert_unique_pos(__k);
	}
      else if (_impl._key_compare(_S_key(__pos._node), __k))
	{
	  // ... then try after.
	  iterator __after = __pos;
	  if (__pos._node == _rightmost())
	    return _Res(0, _rightmost());
	  else if (_impl._key_compare(__k, _S_key((++__after)._node)))
	    {
	      if (_S_right(__pos._node) == 0)
		return _Res(0, __pos._node);
	      else
		return _Res(__after._node, __after._node);
	    }
	  else
	    return _get_insert_unique_pos(__k);
	}
      else
	// Equivalent keys.
	return _Res(__pos._node, 0);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
#if __cplusplus >= 201103L
    template<typename _Arg, typename _NodeGen>
#else
    template<typename _NodeGen>
#endif
      typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
      rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
      _insert_unique_(const_iterator __position,
#if __cplusplus >= 201103L
			_Arg&& __v,
#else
			const _Val& __v,
#endif
			_NodeGen& __node_gen)
    {
      pair<_Base_ptr, _Base_ptr> __res
	= _get_insert_hint_unique_pos(__position, _KeyOfValue()(__v));

      if (__res.second)
	return _insert_(__res.first, __res.second,
			  _GLIBCXX_FORWARD(_Arg, __v),
			  __node_gen);
      return iterator(__res.first);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    pair<typename rb_tree<_Key, _Val, _KeyOfValue,
			   _Compare, _Alloc>::_Base_ptr,
	 typename rb_tree<_Key, _Val, _KeyOfValue,
			   _Compare, _Alloc>::_Base_ptr>
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _get_insert_hint_equal_pos(const_iterator __position, const key_type& __k)
    {
      iterator __pos = __position._const_cast();
      typedef pair<_Base_ptr, _Base_ptr> _Res;

      // end()
      if (__pos._node == _end())
	{
	  if (size() > 0
	      && !_impl._key_compare(__k, _S_key(_rightmost())))
	    return _Res(0, _rightmost());
	  else
	    return _get_insert_equal_pos(__k);
	}
      else if (!_impl._key_compare(_S_key(__pos._node), __k))
	{
	  // First, try before...
	  iterator __before = __pos;
	  if (__pos._node == _leftmost()) // begin()
	    return _Res(_leftmost(), _leftmost());
	  else if (!_impl._key_compare(__k, _S_key((--__before)._node)))
	    {
	      if (_S_right(__before._node) == 0)
		return _Res(0, __before._node);
	      else
		return _Res(__pos._node, __pos._node);
	    }
	  else
	    return _get_insert_equal_pos(__k);
	}
      else
	{
	  // ... then try after.
	  iterator __after = __pos;
	  if (__pos._node == _rightmost())
	    return _Res(0, _rightmost());
	  else if (!_impl._key_compare(_S_key((++__after)._node), __k))
	    {
	      if (_S_right(__pos._node) == 0)
		return _Res(0, __pos._node);
	      else
		return _Res(__after._node, __after._node);
	    }
	  else
	    return _Res(0, 0);
	}
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
#if __cplusplus >= 201103L
    template<typename _Arg, typename _NodeGen>
#else
    template<typename _NodeGen>
#endif
      typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
      rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
      _insert_equal_(const_iterator __position,
#if __cplusplus >= 201103L
		       _Arg&& __v,
#else
		       const _Val& __v,
#endif
		       _NodeGen& __node_gen)
      {
	pair<_Base_ptr, _Base_ptr> __res
	  = _get_insert_hint_equal_pos(__position, _KeyOfValue()(__v));

	if (__res.second)
	  return _insert_(__res.first, __res.second,
			    _GLIBCXX_FORWARD(_Arg, __v),
			    __node_gen);

	return _insert_equal_lower(_GLIBCXX_FORWARD(_Arg, __v));
      }

	  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    void
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _erase_aux(const_iterator __position)
    {
      _Link_type __y =
	static_cast<_Link_type>(rb_tree_rebalance_for_erase
				(const_cast<_Base_ptr>(__position._node),
				 this->_impl._header));
      _drop_node(__y);
      --_impl._node_count;
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    void
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _erase_aux(const_iterator __first, const_iterator __last)
    {
      if (__first == begin() && __last == end())
	clear();
      else
	while (__first != __last)
	  _erase_aux(__first++);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::size_type
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    erase(const _Key& __x)
    {
      pair<iterator, iterator> __p = equal_range(__x);
      const size_type __old_size = size();
      _erase_aux(__p.first, __p.second);
      return __old_size - size();
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    typename rb_tree<_Key, _Val, _KeyOfValue,
		      _Compare, _Alloc>::iterator
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    find(const _Key& __k)
    {
      iterator __j = _lower_bound(_begin(), _end(), __k);
      return (__j == end()
	      || _impl._key_compare(__k,
					_S_key(__j._node))) ? end() : __j;
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    typename rb_tree<_Key, _Val, _KeyOfValue,
		      _Compare, _Alloc>::const_iterator
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    find(const _Key& __k) const
    {
      const_iterator __j = _lower_bound(_begin(), _end(), __k);
      return (__j == end()
	      || _impl._key_compare(__k,
					_S_key(__j._node))) ? end() : __j;
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::size_type
    rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    count(const _Key& __k) const
    {
      pair<const_iterator, const_iterator> __p = equal_range(__k);
      const size_type __n = std::distance(__p.first, __p.second);
      return __n;
    }

  _GLIBCXX_PURE unsigned int
  rb_tree_black_count(const rb_tree_node_base* __node,
		       const rb_tree_node_base* __root) throw ();

  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    bool
    rb_tree<_Key,_Val,_KeyOfValue,_Compare,_Alloc>::__rb_verify() const
    {
      if (_impl._node_count == 0 || begin() == end())
	return _impl._node_count == 0 && begin() == end()
	       && this->_impl._header._left == _end()
	       && this->_impl._header._right == _end();

      unsigned int __len = rb_tree_black_count(_leftmost(), _root());
      for (const_iterator __it = begin(); __it != end(); ++__it)
	{
	  _Const_Link_type __x = static_cast<_Const_Link_type>(__it._node);
	  _Const_Link_type __L = _S_left(__x);
	  _Const_Link_type __R = _S_right(__x);

	  if (__x->_color == _S_red)
	    if ((__L && __L->_color == _S_red)
		|| (__R && __R->_color == _S_red))
	      return false;

	  if (__L && _impl._key_compare(_S_key(__x), _S_key(__L)))
	    return false;
	  if (__R && _impl._key_compare(_S_key(__R), _S_key(__x)))
	    return false;

	  if (!__L && !__R && rb_tree_black_count(__x, _root()) != __len)
	    return false;
	}

      if (_leftmost() != rb_tree_node_base::_S_minimum(_root()))
	return false;
      if (_rightmost() != rb_tree_node_base::_S_maximum(_root()))
	return false;
      return true;
    }
*/

//	}; // rb_tree class
	
}; // namespace ft

#endif /* _FT_TREE_HPP */
