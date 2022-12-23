#ifndef _FTrb_TREE_HPP
# define _FTrb_TREE_HPP

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
		typedef rb_tree_node_base::const_base_ptr		base_ptr;
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
	template<typename Key, typename Val, typename _KeyOfValue,
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
			typedef const rb_tree_node<Val>*	constlink_type;


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

	};
	
}; // namespace ft

#endif /* _FTrb_TREE_HPP */
