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

	}; // rb_tree class
	
}; // namespace ft

#endif /* _FT_TREE_HPP */
