#ifndef _FT_TREE_HPP
#define _FT_TREE_HPP

#include <memory>

#include "utility.hpp"

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

namespace ft
{
	/*************************************************************
	 * Red-Black Tree class
	 *************************************************************/
	template <typename Key, typename Val, typename KeyOfValue,
			  typename Compare, typename Alloc = std::allocator<Val> >
	class rb_tree
	{

		enum rb_tree_color
		{
			_red = false,
			_black = true
		};

		/*************************************************************
		 * Base node
		 *************************************************************/
		struct rb_tree_node_base
		{
			typedef rb_tree_node_base *base_ptr;
			typedef const rb_tree_node_base *const_base_ptr;

			rb_tree_color _color;
			base_ptr _parent;
			base_ptr _left;
			base_ptr _right;

			static base_ptr minimum(base_ptr x)
			{
				while (x->_left != 0)
					x = x->_left;
				return x;
			}

			static const_base_ptr minimum(const_base_ptr x)
			{
				while (x->_left != 0)
					x = x->_left;
				return x;
			}

			static base_ptr maximum(base_ptr x)
			{
				while (x->_right != 0)
					x = x->_right;
				return x;
			}

			static const_base_ptr maximum(const_base_ptr x)
			{
				while (x->_right != 0)
					x = x->_right;
				return x;
			}
		}; // rb_tree_node_base

		/*************************************************************
		 * Helper type offering value initialization guarantee on the
		 compare functor.
		*************************************************************/
		template <typename Key_compare>
		struct rb_tree_key_compare
		{
			Key_compare _key_compare;

			rb_tree_key_compare() : _key_compare() {}

			rb_tree_key_compare(const Key_compare &comp) : _key_compare(comp) {}
		};

		/*************************************************************
		 * Tree header

		* Helper type to manage default initialization of node count and header
		*************************************************************/
		struct rb_tree_header
		{
			rb_tree_node_base _header;
			std::size_t _node_count; // Keeps track of size of tree.

			rb_tree_header()
			{
				_header._color = _red;
				reset();
			}

			void move_data(rb_tree_header &from)
			{
				_header._color = from._header._color;
				_header._parent = from._header._parent;
				_header._left = from._header._left;
				_header._right = from._header._right;
				_header._parent->_parent = &_header;
				_node_count = from._node_count;
				from.reset();
			}

			void reset()
			{
				_header._parent = 0;
				_header._left = &_header;
				_header._right = &_header;
				_node_count = 0;
			}

		}; // rb_tree_header

		/*************************************************************
		 * Red-Black Tree node
		 *************************************************************/
		template <typename T>
		struct rb_tree_node : public rb_tree_node_base
		{
			typedef rb_tree_node<T> *link_type;
			T Tue_field;

			T *valptr() { return &Tue_field; }
			const T *valptr() const { return &Tue_field; }
		};

		/*************************************************************
		|* Red-Black Tree iterator
		*************************************************************/
		template <typename T>
		struct rb_tree_iterator
		{
			/*************************************************************
			 * Types
			 *************************************************************/
			typedef std::ptrdiff_t difference_type;
			typedef T value_type;
			typedef value_type *pointer;
			typedef value_type &reference;
			typedef std::bidirectional_iterator_tag iterator_category;

			typedef rb_tree_iterator<T> self;
			typedef typename rb_tree_node_base::base_ptr base_ptr;
			typedef rb_tree_node<T> *link_type;

			/*************************************************************
			 * Construct/Copy/Destroy
			 *************************************************************/
			rb_tree_iterator() : _node() {}
			explicit rb_tree_iterator(base_ptr x) : _node(x) {}

			/*************************************************************
			 * Accessing operators
			 *************************************************************/
			reference operator*(void) const
			{
				return *static_cast<link_type>(_node)->valptr();
			}

			pointer operator->(void) const
			{
				return static_cast<link_type>(_node)->valptr();
			}

			/*************************************************************
			 * Incrementing operators
			 *************************************************************/
			// Get the next node in key value order
			self &operator++()
			{
				_node = rb_tree_increment(_node);
				return *this;
			}
			self operator++(int)
			{
				self tmp = *this;
				_node = rb_tree_increment(_node);
				return tmp;
			}

			// Get the previous node in key value order
			self &operator--()
			{
				_node = rb_tree_decrement(_node);
				return *this;
			}
			self operator--(int)
			{
				self tmp = *this;
				_node = rb_tree_decrement(_node);
				return tmp;
			}

			/*************************************************************
			 * Boolean operators
			 *************************************************************/
			friend bool operator==(const self &x, const self &y)
			{
				return x._node == y._node;
			}

			friend bool operator!=(const self &x, const self &y)
			{
				return x._node != y._node;
			}

			base_ptr _node;

		}; // rb_tree_iterator

		/*************************************************************
		|* Red-Black Tree const iterator
		*************************************************************/
		template <typename T>
		struct rb_tree_const_iterator
		{
			/*************************************************************
			 * Types
			 *************************************************************/
			typedef std::ptrdiff_t difference_type;
			typedef T value_type;
			typedef const value_type *pointer;
			typedef const value_type &reference;
			typedef std::bidirectional_iterator_tag iterator_category;

			typedef rb_tree_iterator<T> iterator;

			typedef rb_tree_const_iterator<T> self;
			typedef typename rb_tree_node_base::const_base_ptr base_ptr;
			typedef const rb_tree_node<T> *link_type;

			/*************************************************************
			 * Construct/Copy/Destroy
			 *************************************************************/
			rb_tree_const_iterator() : _node() {}
			explicit rb_tree_const_iterator(base_ptr x) : _node(x) {}
			rb_tree_const_iterator(const iterator &it) : _node(it._node) {}

			iterator _const_cast() const
			{
				return iterator(const_cast<typename iterator::base_ptr>(_node));
			}

			/*************************************************************
			 * Accessing operators
			 *************************************************************/
			reference operator*(void) const
			{
				return *static_cast<link_type>(_node)->valptr();
			}

			pointer operator->(void) const
			{
				return static_cast<link_type>(_node)->valptr();
			}

			/*************************************************************
			 * Incrementing operators
			 *************************************************************/
			// Get the next node in key value order
			self &operator++()
			{
				_node = rb_tree_increment((rb_tree_node_base *)_node);
				return *this;
			}
			self operator++(int)
			{
				self tmp = *this;
				_node = rb_tree_increment((rb_tree_node_base *)_node);
				return tmp;
			}

			// Get the previous node in key value order
			self &operator--()
			{
				_node = rb_tree_decrement((rb_tree_node_base *)_node);
				return *this;
			}
			self operator--(int)
			{
				self tmp = *this;
				_node = rb_tree_decrement((rb_tree_node_base *)_node);
				return tmp;
			}

			/*************************************************************
			 * Boolean operators
			 *************************************************************/
			friend bool operator==(const self &x, const self &y)
			{
				return x._node == y._node;
			}

			friend bool operator!=(const self &x, const self &y)
			{
				return x._node != y._node;
			}

			base_ptr _node;

		}; // rb_tree_const_iterator

		typedef typename __gnu_cxx::__alloc_traits<Alloc>::template rebind<rb_tree_node<Val> >::other node_allocator;
		typedef __gnu_cxx::__alloc_traits<node_allocator> alloc_traits;

	protected:
		typedef rb_tree_node_base *base_ptr;
		typedef const rb_tree_node_base *const_base_ptr;
		typedef rb_tree_node<Val> *link_type;
		typedef const rb_tree_node<Val> *const_link_type;

	private:
		// Functor recycling a pool of nodes and using allocation once the pool
		// is empty.
		struct reuse_or_alloc_node
		{
			reuse_or_alloc_node(rb_tree &__t)
				: _root(__t.root()), _nodes(__t.rightmost()), _t(__t)
			{
				if (_root)
				{
					_root->_parent = 0;

					if (_nodes->_left)
						_nodes = _nodes->_left;
				}
				else
					_nodes = 0;
			}

			~reuse_or_alloc_node()
			{
				_t.erase(static_cast<link_type>(_root));
			}

			template <typename _Arg>
			link_type operator()(const _Arg &__arg)
			{
				link_type node = static_cast<link_type>(extract());
				if (node)
				{
					_t.destroy_node(node);
					_t.construct_node(node, __arg);
					return node;
				}

				return _t.create_node(__arg);
			}

		private:
			base_ptr extract()
			{
				if (!_nodes)
					return _nodes;

				base_ptr node = _nodes;
				_nodes = _nodes->_parent;
				if (_nodes)
				{
					if (_nodes->_right == node)
					{
						_nodes->_right = 0;
						if (_nodes->_left)
						{
							_nodes = _nodes->_left;
							while (_nodes->_right)
								_nodes = _nodes->_right;
							if (_nodes->_left)
								_nodes = _nodes->_left;
						}
					}
					else // node is on the left.
						_nodes->_left = 0;
				}
				else
					_root = 0;
				return node;
			}

			base_ptr _root;
			base_ptr _nodes;
			rb_tree &_t;
		};

		struct alloc_node
		{
			alloc_node(rb_tree &t) : _t(t) {}

			template <typename Arg>
			link_type operator()(Arg arg) const { return _t.create_node(arg); }

		private:
			rb_tree &_t;
		};


	public:

		typedef Key					key_type;
		typedef Val					value_type;
		typedef value_type 			*pointer;
		typedef const value_type	*const_pointer;
		typedef value_type			&reference;
		typedef const value_type	&const_reference;
		typedef std::size_t			size_type;
		typedef std::ptrdiff_t		difference_type;
		typedef Alloc				allocator_type;

		node_allocator &get_node_allocator() { return _impl; }

		const node_allocator &get_node_allocator() const { return _impl; }

		allocator_type get_allocator() const
		{ return allocator_type(get_node_allocator()); }


	protected:

		link_type get_node()
		{ return alloc_traits::allocate(get_node_allocator(), 1); }

		void put_node(link_type p)
		{ alloc_traits::deallocate(get_node_allocator(), p, 1); }

		void construct_node(link_type node, const value_type &x)
		{
			try
			{ get_allocator().construct(node->valptr(), x); }
			catch (...)
			{ put_node(node); throw; }
		}

		link_type create_node(const value_type &x)
		{
			link_type tmp = get_node();
			construct_node(tmp, x);
			return tmp;
		}

		void destroy_node(link_type p)
		{ get_allocator().destroy(p->valptr()); }

		void drop_node(link_type p)
		{
			destroy_node(p);
			put_node(p);
		}

		template <bool MoveValue, typename NodeGen>
		link_type clone_node(link_type x, NodeGen &node_gen)
		{
			link_type tmp = node_gen(*x->valptr());

			tmp->_color = x->_color;
			tmp->_left = 0;
			tmp->_right = 0;
			return tmp;
		}

		template <typename Key_compare>
		struct rb_tree_impl
			: public node_allocator,
			  public rb_tree_key_compare<Key_compare>,
			  public rb_tree_header
		{
			typedef rb_tree_key_compare<Key_compare> base_key_compare;

			rb_tree_impl() : node_allocator() {}

			// rb_tree_impl(const rb_tree_impl& x)
			// : node_allocator(), base_key_compare(x._key_compare), rb_tree_header() {}

			rb_tree_impl(const Key_compare &comp, const node_allocator &a)
				: node_allocator(a), base_key_compare(comp) {}
		};

		rb_tree_impl<Compare> _impl;

		base_ptr &root() { return _impl._header._parent; }
		const_base_ptr root() const { return _impl._header._parent; }

		base_ptr &leftmost() { return _impl._header._left; }
		const_base_ptr leftmost() const { return _impl._header._left; }

		base_ptr &rightmost() { return _impl._header._right; }
		const_base_ptr rightmost() const { return _impl._header._right; }

		link_type _mbegin() const
		{ return static_cast<link_type>(_impl._header._parent); }

		link_type _begin() { return _mbegin(); }

		const_link_type _begin() const
		{ return static_cast<const_link_type>(_impl._header._parent); }

		base_ptr _end() { return &_impl._header; }

		const_base_ptr _end() const { return &_impl._header; }

		static const Key &key(const_link_type x)
		{ return KeyOfValue()(*x->valptr()); }

		static link_type left(base_ptr x)
		{ return static_cast<link_type>(x->_left); }

		static const_link_type left(const_base_ptr x)
		{ return static_cast<const_link_type>(x->_left); }

		static link_type right(base_ptr x)
		{ return static_cast<link_type>(x->_right); }

		static const_link_type right(const_base_ptr x)
		{ return static_cast<const_link_type>(x->_right); }

		static const Key &key(const_base_ptr x)
		{ return key(static_cast<const_link_type>(x)); }

		static base_ptr minimum(base_ptr x)
		{ return rb_tree_node_base::minimum(x); }

		static const_base_ptr minimum(const_base_ptr x)
		{ return rb_tree_node_base::minimum(x); }

		static base_ptr maximum(base_ptr x)
		{ return rb_tree_node_base::maximum(x); }

		static const_base_ptr maximum(const_base_ptr x)
		{ return rb_tree_node_base::maximum(x); }


	public:

		typedef rb_tree_iterator<value_type>			iterator;
		typedef rb_tree_const_iterator<value_type>		const_iterator;

		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

		ft::pair<base_ptr, base_ptr>	get_insert_unique_pos(const key_type &k);
		ft::pair<base_ptr, base_ptr>	get_insert_equal_pos(const key_type &k);
		ft::pair<base_ptr, base_ptr>	get_insert_hint_unique_pos(iterator pos,
																const key_type &k);
		ft::pair<base_ptr, base_ptr>	get_insert_hint_unique_pos(const_iterator pos,
																const key_type &k);
		ft::pair<base_ptr, base_ptr>	get_insert_hint_equal_pos(const_iterator pos,
															   const key_type &k);


	private:

		template <typename _NodeGen>
		iterator
		insert_(base_ptr x, base_ptr y, const value_type &v, _NodeGen &);

		// Insertion hints in associative containers.
		iterator insert_lower(base_ptr y, const value_type &v);
		iterator insert_equal_lower(const value_type &x);

		enum
		{
			__as_lvalue,
			__as_rvalue
		};

		template <bool MoveValues, typename NodeGen>
		link_type copy(link_type, base_ptr, NodeGen &);

		template <bool MoveValues, typename NodeGen>
		link_type copy(const rb_tree &x, NodeGen &gen)
		{
			link_type root = copy<MoveValues>(x._mbegin(), _end(), gen);
			leftmost() = minimum(root);
			rightmost() = maximum(root);
			_impl._node_count = x._impl._node_count;
			return root;
		}

		link_type copy(const rb_tree &x)
		{
			alloc_node an(*this);
			return copy<__as_lvalue>(x, an);
		}

		void erase(link_type x);

		iterator lower_bound(link_type x, base_ptr y, const Key &k);
		const_iterator lower_bound(const_link_type x, const_base_ptr y,
								   const Key &k) const;

		iterator upper_bound(link_type x, base_ptr y, const Key &k);
		const_iterator upper_bound(const_link_type x, const_base_ptr y,
								   const Key &k) const;


	public:

		rb_tree() {}

		rb_tree(const Compare &comp, const allocator_type &a = allocator_type())
			: _impl(comp, node_allocator(a)) {}

		rb_tree(const rb_tree &x) : _impl(x._impl)
		{ if (x.root() != 0) root() = copy(x); }

		~rb_tree() { erase(_begin()); }

		rb_tree &operator=(const rb_tree &x);

		// Accessors.
		Compare key_comp() const { return _impl._key_compare; }

		iterator begin()
		{ return iterator(_impl._header._left); }

		const_iterator begin() const
		{ return const_iterator(_impl._header._left); }

		iterator end() { return iterator(&_impl._header); }

		const_iterator end() const
		{ return const_iterator(&_impl._header); }

		reverse_iterator rbegin() { return reverse_iterator(end()); }

		const_reverse_iterator rbegin() const
		{ return const_reverse_iterator(end()); }

		reverse_iterator rend() { return reverse_iterator(begin()); }

		const_reverse_iterator rend() const
		{ return const_reverse_iterator(begin()); }

		bool empty() const { return _impl._node_count == 0; }

		size_type size() const { return _impl._node_count; }

		size_type max_size() const
		{ return alloc_traits::max_size(get_node_allocator()); }

		void swap(rb_tree &t);

		pair<iterator, bool> insert_unique(const value_type &x);
		iterator insert_equal(const value_type &x);

		template <typename NodeGen>
		iterator insert_unique(iterator pos, const value_type &x,
							   NodeGen &);
		template <typename NodeGen>
		iterator insert_unique(const_iterator pos, const value_type &x,
							   NodeGen &);
		iterator insert_unique(const_iterator pos, const value_type &x)
		{
			alloc_node an(*this);
			return insert_unique(pos, x, an);
		}

		template <typename NodeGen>
		iterator insert_equal_(iterator pos, const value_type &x,
							   NodeGen &);
		template <typename NodeGen>
		iterator insert_equal_(const_iterator pos, const value_type &x,
							   NodeGen &);
		iterator insert_equal_(const_iterator pos, const value_type &x)
		{
			alloc_node an(*this);
			return insert_equal_(pos, x, an);
		}

		template <typename _InputIterator>
		void insert_range_unique(_InputIterator first, _InputIterator last)
		{
			alloc_node an(*this);
			for (; first != last; ++first)
				insert_unique(end(), *first, an);
		}

		template <typename _InputIterator>
		void insert_range_equal(_InputIterator first, _InputIterator last)
		{
			alloc_node an(*this);
			for (; first != last; ++first)
				insert_equal_(end(), *first, an);
		}


	private:

		void erase_aux(iterator position);
		void erase_aux(iterator first, iterator last);
		void erase_aux(const_iterator position);
		void erase_aux(const_iterator first, const_iterator last);


	public:

		void erase(iterator position) { erase_aux(position); }

		void erase(const_iterator position) { erase_aux(position); }

		size_type erase(const key_type &x);

		void erase(iterator first, iterator last) { erase_aux(first, last); }

		void erase(const_iterator first, const_iterator last)
		{ erase_aux(first, last); }

		void clear()
		{ erase(_begin()); _impl.reset(); }

		// Set operations.
		iterator find(const key_type &k);
		const_iterator find(const key_type &k) const;

		size_type count(const key_type &k) const;

		iterator lower_bound(const key_type &k)
		{ return lower_bound(_begin(), _end(), k); }

		const_iterator lower_bound(const key_type &k) const
		{ return lower_bound(_begin(), _end(), k); }

		iterator upper_bound(const key_type &k)
		{ return upper_bound(_begin(), _end(), k); }

		const_iterator upper_bound(const key_type &k) const
		{ return upper_bound(_begin(), _end(), k); }

		pair<iterator, iterator> equal_range(const key_type &k);

		pair<const_iterator, const_iterator>
		equal_range(const key_type &k) const;

		friend bool operator==(const rb_tree &x, const rb_tree &y)
		{
			return x.size() == y.size()
				&& ft::equal(x.begin(), x.end(), y.begin());
		}

		friend bool operator<(const rb_tree &x, const rb_tree &y)
		{
			return ft::lexicographical_compare(x.begin(), x.end(),
											   y.begin(), y.end());
		}


		/*************************************************************
		 * From here we are implementing some functions from the libstdc
		 *************************************************************/

		/*************************************************************
		 * Incrementing functions
		 *************************************************************/
		static rb_tree_node_base *rb_tree_increment(rb_tree_node_base *x)
		{
			// If there is a right subtree, go to its leftmost (=minimal) node
			if (x->_right != 0)
			{
				x = x->_right;
				while (x->_left != 0)
					x = x->_left;
			}
			else
			{
				// Otherwise go up the tree, looking for a node
				//  that is its parent's left child
				rb_tree_node_base *y = x->_parent;
				while (x == y->_right)
				{
					x = y;
					y = y->_parent;
				}
				// Handle end() case
				if (x->_right != y)
					x = y;
			}
			return x;
		}

		const rb_tree_node_base *rb_tree_increment(const rb_tree_node_base *x)
		{ return rb_tree_increment(const_cast<rb_tree_node_base *>(x)); }

		static rb_tree_node_base *rb_tree_decrement(rb_tree_node_base *x)
		{
			// Handle end() case
			if (x->_color == _red && x->_parent->_parent == x)
				x = x->_right;
			else if (x->_left != 0)
			{
				// If there is a left subtree, go to its rightmost (=max) node
				rb_tree_node_base *y = x->_left;
				while (y->_right != 0)
					y = y->_right;
				x = y;
			}
			else
			{
				// Otherwise go up the tree, looking for a node
				//  that is its parent's right child.
				rb_tree_node_base *y = x->_parent;
				while (x == y->_left)
				{
					x = y;
					y = y->_parent;
				}
				x = y;
			}
			return x;
		}

		const rb_tree_node_base *rb_tree_decrement(const rb_tree_node_base *x)
		{ return rb_tree_decrement(const_cast<rb_tree_node_base *>(x)); }


		/*************************************************************
		 * Rotation functions
		 *************************************************************/
		static void rb_tree_rotate_left(rb_tree_node_base *const x,
										rb_tree_node_base *&root)
		{
			rb_tree_node_base *const y = x->_right;

			x->_right = y->_left;
			if (y->_left != 0)
				y->_left->_parent = x;
			y->_parent = x->_parent;

			if (x == root)
				root = y;
			else if (x == x->_parent->_left)
				x->_parent->_left = y;
			else
				x->_parent->_right = y;
			y->_left = x;
			x->_parent = y;
		}

		static void rb_tree_rotate_right(rb_tree_node_base *const x,
										 rb_tree_node_base *&root)
		{
			rb_tree_node_base *const y = x->_left;

			x->_left = y->_right;
			if (y->_right != 0)
				y->_right->_parent = x;
			y->_parent = x->_parent;

			if (x == root)
				root = y;
			else if (x == x->_parent->_right)
				x->_parent->_right = y;
			else
				x->_parent->_left = y;
			y->_right = x;
			x->_parent = y;
		}


		/*************************************************************
		 * Insert and rebalance
		 *************************************************************/
		static void rb_tree_insert_and_rebalance(const bool insert_left,
												 rb_tree_node_base *x,
												 rb_tree_node_base *p,
												 rb_tree_node_base &header)
		{
			rb_tree_node_base *&_root = header._parent;

			// Initialize fields in new node to insert.
			x->_parent = p;
			x->_left = 0;
			x->_right = 0;
			x->_color = _red;

			// Insert.
			// Make new node child of parent and maintain root, leftmost and
			// rightmost nodes.
			// N.B. First node is always inserted left.
			if (insert_left == true)
			{
				p->_left = x; // also makes leftmost = x when p == &header

				if (p == &header)
				{
					header._parent = x;
					header._right = x;
				}
				else if (p == header._left)
					header._left = x; // maintain leftmost pointing to min node
			}
			else
			{
				p->_right = x;

				if (p == header._right)
					header._right = x; // maintain rightmost pointing to max node
			}

			// Rebalance.
			while (x != _root && x->_parent->_color == _red)
			{
				rb_tree_node_base *const xpp = x->_parent->_parent;

				if (x->_parent == xpp->_left)
				{
					rb_tree_node_base *const y = xpp->_right;
					if (y && y->_color == _red)
					{
						x->_parent->_color = _black;
						y->_color = _black;
						xpp->_color = _red;
						x = xpp;
					}
					else
					{
						if (x == x->_parent->_right)
						{
							x = x->_parent;
							rb_tree_rotate_left(x, _root);
						}
						x->_parent->_color = _black;
						xpp->_color = _red;
						rb_tree_rotate_right(xpp, _root);
					}
				}
				else
				{
					rb_tree_node_base *const y = xpp->_left;
					if (y && y->_color == _red)
					{
						x->_parent->_color = _black;
						y->_color = _black;
						xpp->_color = _red;
						x = xpp;
					}
					else
					{
						if (x == x->_parent->_left)
						{
							x = x->_parent;
							rb_tree_rotate_right(x, _root);
						}
						x->_parent->_color = _black;
						xpp->_color = _red;
						rb_tree_rotate_left(xpp, _root);
					}
				}
			}
			_root->_color = _black;

		} // rb_tree_insert_and_rebalance


		/*************************************************************
		 * Rebalance for erase
		 *************************************************************/
		rb_tree_node_base *rb_tree_rebalance_for_erase(rb_tree_node_base *const z,
													   rb_tree_node_base &header)
		{
			rb_tree_node_base *&root = header._parent;
			rb_tree_node_base *&leftmost = header._left;
			rb_tree_node_base *&rightmost = header._right;
			rb_tree_node_base *y = z;
			rb_tree_node_base *x = 0;
			rb_tree_node_base *x_parent = 0;

			if (y->_left == 0)		 // z has at most one non-null child. y == z.
				x = y->_right;		 // x might be null.
			else if (y->_right == 0) // z has exactly one non-null child. y == z.
				x = y->_left;		 // x is not null.
			else
			{
				// z has two non-null children.  Set y to
				y = y->_right; //   z's successor.  x might be null.
				while (y->_left != 0)
					y = y->_left;
				x = y->_right;
			}
			if (y != z)
			{
				// relink y in place of z.  y is z's successor
				z->_left->_parent = y;
				y->_left = z->_left;
				if (y != z->_right)
				{
					x_parent = y->_parent;
					if (x)
						x->_parent = y->_parent;
					y->_parent->_left = x; // y must be a child of _left
					y->_right = z->_right;
					z->_right->_parent = y;
				}
				else
					x_parent = y;
				if (root == z)
					root = y;
				else if (z->_parent->_left == z)
					z->_parent->_left = y;
				else
					z->_parent->_right = y;
				y->_parent = z->_parent;
				ft::swap(y->_color, z->_color);
				y = z;
				// y now points to node to be actually deleted
			}
			else
			{ // y == z
				x_parent = y->_parent;
				if (x)
					x->_parent = y->_parent;
				if (root == z)
					root = x;
				else if (z->_parent->_left == z)
					z->_parent->_left = x;
				else
					z->_parent->_right = x;
				if (leftmost == z)
				{
					if (z->_right == 0) // z->_left must be null also
						leftmost = z->_parent;
					// makes leftmost == _header if z == root
					else
						leftmost = rb_tree_node_base::minimum(x);
				}
				if (rightmost == z)
				{
					if (z->_left == 0) // z->_right must be null also
						rightmost = z->_parent;
					// makes rightmost == _header if z == root
					else // x == z->_left
						rightmost = rb_tree_node_base::maximum(x);
				}
			}
			if (y->_color != _red)
			{
				while (x != root && (x == 0 || x->_color == _black))
					if (x == x_parent->_left)
					{
						rb_tree_node_base *w = x_parent->_right;
						if (w->_color == _red)
						{
							w->_color = _black;
							x_parent->_color = _red;
							rb_tree_rotate_left(x_parent, root);
							w = x_parent->_right;
						}
						if ((w->_left == 0 || w->_left->_color == _black) &&
							(w->_right == 0 || w->_right->_color == _black))
						{
							w->_color = _red;
							x = x_parent;
							x_parent = x_parent->_parent;
						}
						else
						{
							if (w->_right == 0 || w->_right->_color == _black)
							{
								w->_left->_color = _black;
								w->_color = _red;
								rb_tree_rotate_right(w, root);
								w = x_parent->_right;
							}
							w->_color = x_parent->_color;
							x_parent->_color = _black;
							if (w->_right)
								w->_right->_color = _black;
							rb_tree_rotate_left(x_parent, root);
							break;
						}
					}
					else
					{
						// same as above, with _right <-> _left.
						rb_tree_node_base *w = x_parent->_left;
						if (w->_color == _red)
						{
							w->_color = _black;
							x_parent->_color = _red;
							rb_tree_rotate_right(x_parent, root);
							w = x_parent->_left;
						}
						if ((w->_right == 0 || w->_right->_color == _black) &&
							(w->_left == 0 || w->_left->_color == _black))
						{
							w->_color = _red;
							x = x_parent;
							x_parent = x_parent->_parent;
						}
						else
						{
							if (w->_left == 0 || w->_left->_color == _black)
							{
								w->_right->_color = _black;
								w->_color = _red;
								rb_tree_rotate_left(w, root);
								w = x_parent->_left;
							}
							w->_color = x_parent->_color;
							x_parent->_color = _black;
							if (w->_left)
								w->_left->_color = _black;
							rb_tree_rotate_right(x_parent, root);
							break;
						}
					}
				if (x)
					x->_color = _black;
			}
			return y;
		} // rb_tree_rebalance_for_erase

	}; // rb_tree class

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	inline void
	swap(rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &x,
		 rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &y)
	{ x.swap(y); }

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
	operator=(const rb_tree &x)
	{
		if (this != &x)
		{
			reuse_or_alloc_node __roan(*this);
			_impl.reset();
			_impl._key_compare = x._impl._key_compare;
			if (x.root() != 0)
				root() = copy<__as_lvalue>(x, __roan);
		}
		return *this;
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	template <typename _NodeGen>
	typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		insert_(base_ptr x, base_ptr p, const _Val &v, _NodeGen &node_gen)
	{
		bool __insert_left = (x != 0 || p == _end()
			|| _impl._key_compare(_KeyOfValue()(v), key(p)));

		link_type __z = node_gen(v);

		rb_tree_insert_and_rebalance(__insert_left, __z, p, _impl._header);
		++_impl._node_count;
		return iterator(__z);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		insert_lower(base_ptr p, const _Val &v)
	{
		bool __insert_left = (p == _end()
			|| !_impl._key_compare(key(p), _KeyOfValue()(v)));

		link_type __z = _create_node(v);

		rb_tree_insert_and_rebalance(__insert_left, __z, p,
									 this->_impl._header);
		++_impl._node_count;

		return iterator(__z);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		insert_equal_lower(const _Val &v)
	{
		link_type x = _begin();
		base_ptr y = _end();
		while (x != 0)
		{
			y = x;
			x = !_impl._key_compare(key(x), _KeyOfValue()(v)) ? _(x) : right(x);
		}
		return _insert_lower(y, v);
	}

	template <typename _Key, typename _Val, typename _KoV,
			  typename _Compare, typename _Alloc>
	template <bool _MoveValues, typename _NodeGen>
	typename rb_tree<_Key, _Val, _KoV, _Compare, _Alloc>::link_type
	rb_tree<_Key, _Val, _KoV, _Compare, _Alloc>::
		copy(link_type x, base_ptr p, _NodeGen &node_gen)
	{
		// Structural copy. x and p must be non-null.
		link_type __top = clone_node<_MoveValues>(x, node_gen);
		__top->_parent = p;

		try
		{
			if (x->_right)
				__top->_right =
					copy<_MoveValues>(right(x), __top, node_gen);
			p = __top;
			x = left(x);

			while (x != 0)
			{
				link_type y = clone_node<_MoveValues>(x, node_gen);
				p->_left = y;
				y->_parent = p;
				if (x->_right)
					y->_right = copy<_MoveValues>(right(x),
												  y, node_gen);
				p = y;
				x = left(x);
			}
		}
		catch (...)
		{ erase(__top); throw; }

		return __top;
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	void rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		erase(link_type x)
	{
		// Erase without rebalancing.
		while (x != 0)
		{
			erase(right(x));
			link_type y = left(x);
			drop_node(x);
			x = y;
		}
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		lower_bound(link_type x, base_ptr y,
					const _Key &k)
	{
		while (x != 0)
			if (!_impl._key_compare(key(x), k))
				y = x, x = left(x);
			else
				x = right(x);
		return iterator(y);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::const_iterator
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		lower_bound(const_link_type x, const_base_ptr y,
					const _Key &k) const
	{
		while (x != 0)
			if (!_impl._key_compare(key(x), k))
				y = x, x = left(x);
			else
				x = right(x);
		return const_iterator(y);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		upper_bound(link_type x, base_ptr y, const _Key &k)
	{
		while (x != 0)
			if (_impl._key_compare(k, key(x)))
				y = x, x = left(x);
			else
				x = right(x);
		return iterator(y);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::const_iterator
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		upper_bound(const_link_type x, const_base_ptr y,
					const _Key &k) const
	{
		while (x != 0)
			if (_impl._key_compare(k, key(x)))
				y = x, x = left(x);
			else
				x = right(x);
		return const_iterator(y);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	pair<typename rb_tree<_Key, _Val, _KeyOfValue,
						  _Compare, _Alloc>::iterator,
		 typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator>
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		equal_range(const _Key &k)
	{
		link_type x = _begin();
		base_ptr y = _end();
		while (x != 0)
		{
			if (_impl._key_compare(key(x), k))
				x = right(x);
			else if (_impl._key_compare(k, key(x)))
				y = x, x = left(x);
			else
			{
				link_type xu(x);
				base_ptr yu(y);
				y = x, x = left(x);
				xu = right(xu);
				return pair<iterator,
							iterator>(lower_bound(x, y, k),
									  upper_bound(xu, yu, k));
			}
		}
		return pair<iterator, iterator>(iterator(y), iterator(y));
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	pair<typename rb_tree<_Key, _Val, _KeyOfValue,
						  _Compare, _Alloc>::const_iterator,
		 typename rb_tree<_Key, _Val, _KeyOfValue,
						  _Compare, _Alloc>::const_iterator>
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		equal_range(const _Key &k) const
	{
		const_link_type x = _begin();
		const_base_ptr y = _end();
		while (x != 0)
		{
			if (_impl._key_compare(key(x), k))
				x = right(x);
			else if (_impl._key_compare(k, key(x)))
				y = x, x = left(x);
			else
			{
				const_link_type xu(x);
				const_base_ptr yu(y);
				y = x, x = left(x);
				xu = right(xu);
				return pair<const_iterator,
							const_iterator>(lower_bound(x, y, k),
											upper_bound(xu, yu, k));
			}
		}
		return pair<const_iterator, const_iterator>(const_iterator(y),
													const_iterator(y));
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	void rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		swap(rb_tree &__t)
	{
		if (root() == 0)
		{
			if (__t.root() != 0)
				_impl.move_data(__t._impl);
		}
		else if (__t.root() == 0)
			__t._impl.move_data(_impl);
		else
		{
			ft::swap(root(), __t.root());
			ft::swap(leftmost(), __t.leftmost());
			ft::swap(rightmost(), __t.rightmost());

			root()->_parent = _end();
			__t.root()->_parent = __t._end();
			ft::swap(this->_impl._node_count, __t._impl._node_count);
		}
		// No need to swap header's color as it does not change.
		ft::swap(this->_impl._key_compare, __t._impl._key_compare);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	pair<typename rb_tree<_Key, _Val, _KeyOfValue,
						  _Compare, _Alloc>::base_ptr,
		 typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::base_ptr>
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		get_insert_unique_pos(const key_type &k)
	{
		typedef pair<base_ptr, base_ptr> _Res;
		link_type x = _begin();
		base_ptr y = _end();
		bool __comp = true;
		while (x != 0)
		{
			y = x;
			__comp = _impl._key_compare(k, key(x));
			x = __comp ? left(x) : right(x);
		}
		iterator j = iterator(y);
		if (__comp)
		{
			if (j == begin())
				return _Res(x, y);
			else
				--j;
		}
		if (_impl._key_compare(key(j._node), k))
			return _Res(x, y);
		return _Res(j._node, 0);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	pair<typename rb_tree<_Key, _Val, _KeyOfValue,
						  _Compare, _Alloc>::base_ptr,
		 typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::base_ptr>
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		get_insert_equal_pos(const key_type &k)
	{
		typedef pair<base_ptr, base_ptr> _Res;
		link_type x = _begin();
		base_ptr y = _end();
		while (x != 0)
		{
			y = x;
			x = _impl._key_compare(k, key(x)) ? left(x) : right(x);
		}
		return _Res(x, y);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	pair<typename rb_tree<_Key, _Val, _KeyOfValue,
						  _Compare, _Alloc>::iterator,
		 bool>
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		insert_unique(const _Val &v)
	{
		typedef pair<iterator, bool> _Res;
		pair<base_ptr, base_ptr> res = get_insert_unique_pos(_KeyOfValue()(v));

		if (res.second)
		{
			alloc_node an(*this);
			return _Res(insert_(res.first, res.second, v, an), true);
		}
		return _Res(iterator(res.first), false);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		insert_equal(const _Val &v)
	{
		pair<base_ptr, base_ptr> res = get_insert_equal_pos(_KeyOfValue()(v));
		alloc_node an(*this);
		return _insert_(res.first, res.second, v, an);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	pair<typename rb_tree<_Key, _Val, _KeyOfValue,
						  _Compare, _Alloc>::base_ptr,
		 typename rb_tree<_Key, _Val, _KeyOfValue,
						  _Compare, _Alloc>::base_ptr>
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		get_insert_hint_unique_pos(iterator position, const key_type &k)
	{
		iterator							pos = position;
		typedef pair<base_ptr, base_ptr>	_Res;

		// end()
		if (pos._node == _end())
		{
			if (size() > 0 && _impl._key_compare(key(rightmost()), k))
				return _Res(0, rightmost());
			else
				return get_insert_unique_pos(k);
		}
		else if (_impl._key_compare(k, key(pos._node)))
		{
			// First, try before...
			iterator before = pos;
			if (pos._node == leftmost()) // begin()
				return _Res(leftmost(), leftmost());
			else if (_impl._key_compare(key((--before)._node), k))
			{
				if (right(before._node) == 0)
					return _Res(0, before._node);
				else
					return _Res(pos._node, pos._node);
			}
			else
				return get_insert_unique_pos(k);
		}
		else if (_impl._key_compare(key(pos._node), k))
		{
			// ... then try after.
			iterator after = pos;
			if (pos._node == rightmost())
				return _Res(0, rightmost());
			else if (_impl._key_compare(k, key((++after)._node)))
			{
				if (right(pos._node) == 0)
					return _Res(0, pos._node);
				else
					return _Res(after._node, after._node);
			}
			else
				return get_insert_unique_pos(k);
		}
		else
			// Equivalent keys.
			return _Res(pos._node, 0);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	pair<typename rb_tree<_Key, _Val, _KeyOfValue,
						  _Compare, _Alloc>::base_ptr,
		 typename rb_tree<_Key, _Val, _KeyOfValue,
						  _Compare, _Alloc>::base_ptr>
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		get_insert_hint_unique_pos(const_iterator position,
								   const key_type &k)
	{
		iterator	pos = position._const_cast();
		typedef pair<base_ptr, base_ptr>	_Res;

		// end()
		if (pos._node == _end())
		{
			if (size() > 0 && _impl._key_compare(key(rightmost()), k))
				return _Res(0, rightmost());
			else
				return get_insert_unique_pos(k);
		}
		else if (_impl._key_compare(k, key(pos._node)))
		{
			// First, try before...
			iterator before = pos;
			if (pos._node == leftmost()) // begin()
				return _Res(leftmost(), leftmost());
			else if (_impl._key_compare(key((--before)._node), k))
			{
				if (right(before._node) == 0)
					return _Res(0, before._node);
				else
					return _Res(pos._node, pos._node);
			}
			else
				return get_insert_unique_pos(k);
		}
		else if (_impl._key_compare(key(pos._node), k))
		{
			// ... then try after.
			iterator	after = pos;
			if (pos._node == rightmost())
				return _Res(0, rightmost());
			else if (_impl._key_compare(k, key((++after)._node)))
			{
				if (right(pos._node) == 0)
					return _Res(0, pos._node);
				else
					return _Res(after._node, after._node);
			}
			else
				return get_insert_unique_pos(k);
		}
		else
			// Equivalent keys.
			return _Res(pos._node, 0);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	template <typename _NodeGen>
	typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		insert_unique(const_iterator position, const _Val &v,
					  _NodeGen &node_gen)
	{
		pair<base_ptr, base_ptr> res = get_insert_hint_unique_pos(
			position, _KeyOfValue()(v));

		if (res.second)
			return insert_(res.first, res.second, v, node_gen);
		return iterator(res.first);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	template <typename _NodeGen>
	typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		insert_unique(iterator position, const _Val &v, _NodeGen &node_gen)
	{
		pair<base_ptr, base_ptr> res = get_insert_hint_unique_pos(
			position, _KeyOfValue()(v));

		if (res.second)
			return insert_(res.first, res.second, v, node_gen);
		return iterator(res.first);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	pair<typename rb_tree<_Key, _Val, _KeyOfValue,
						  _Compare, _Alloc>::base_ptr,
		 typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::base_ptr>
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		get_insert_hint_equal_pos(const_iterator position, const key_type &k)
	{
		iterator pos = position._const_cast();
		typedef pair<base_ptr, base_ptr> _Res;

		// end()
		if (pos._node == _end())
		{
			if (size() > 0 && !_impl._key_compare(k, key(rightmost())))
				return _Res(0, rightmost());
			else
				return get_insert_equal_pos(k);
		}
		else if (!_impl._key_compare(key(pos._node), k))
		{
			// First, try before...
			iterator before = pos;
			if (pos._node == leftmost()) // begin()
				return _Res(leftmost(), leftmost());
			else if (!_impl._key_compare(k, key((--before)._node)))
			{
				if (right(before._node) == 0)
					return _Res(0, before._node);
				else
					return _Res(pos._node, pos._node);
			}
			else
				return get_insert_equal_pos(k);
		}
		else
		{
			// ... then try after.
			iterator after = pos;
			if (pos._node == rightmost())
				return _Res(0, rightmost());
			else if (!_impl._key_compare(key((++after)._node), k))
			{
				if (right(pos._node) == 0)
					return _Res(0, pos._node);
				else
					return _Res(after._node, after._node);
			}
			else
				return _Res(0, 0);
		}
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	template <typename _NodeGen>
	typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		insert_equal_(const_iterator position, const _Val &v,
					  _NodeGen &node_gen)
	{
		pair<base_ptr, base_ptr> res = get_insert_hint_equal_pos(position, _KeyOfValue()(v));

		if (res.second)
			return insert_(res.first, res.second, v, node_gen);
		return insert_equal_lower(v);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	template <typename _NodeGen>
	typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		insert_equal_(iterator position, const _Val &v,
					  _NodeGen &node_gen)
	{
		pair<base_ptr, base_ptr> res
			= get_insert_hint_equal_pos(position, _KeyOfValue()(v));

		if (res.second)
			return insert_(res.first, res.second, v, node_gen);
		return insert_equal_lower(v);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	void rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		erase_aux(iterator position)
	{
		link_type y =
			static_cast<link_type>(rb_tree_rebalance_for_erase(
				const_cast<base_ptr>(position._node),
				this->_impl._header));
		drop_node(y);
		--_impl._node_count;
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	void rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		erase_aux(const_iterator position)
	{
		link_type y =
			static_cast<link_type>(rb_tree_rebalance_for_erase(
				const_cast<base_ptr>(position._node),
				this->_impl._header));
		drop_node(y);
		--_impl._node_count;
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	void rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		erase_aux(iterator first, iterator last)
	{
		if (first == begin() && last == end())
			clear();
		else
			while (first != last)
				erase_aux(first++);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	void rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		erase_aux(const_iterator first, const_iterator last)
	{
		if (first == begin() && last == end())
			clear();
		else
			while (first != last)
				erase_aux(first++);
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::size_type
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		erase(const _Key &x)
	{
		pair<iterator, iterator> p = equal_range(x);
		const size_type __old_size = size();
		erase_aux(p.first, p.second);
		return __old_size - size();
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		find(const _Key &k)
	{
		iterator j = lower_bound(_begin(), _end(), k);
		return (j == end() || _impl._key_compare(k, key(j._node))) ? end() : j;
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	typename rb_tree<_Key, _Val, _KeyOfValue,
					 _Compare, _Alloc>::const_iterator
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		find(const _Key &k) const
	{
		const_iterator j = lower_bound(_begin(), _end(), k);
		return (j == end() || _impl._key_compare(k, key(j._node))) ? end() : j;
	}

	template <typename _Key, typename _Val, typename _KeyOfValue,
			  typename _Compare, typename _Alloc>
	typename rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::size_type
	rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
		count(const _Key &k) const
	{
		pair<const_iterator, const_iterator> p = equal_range(k);
		const size_type __n = std::distance(p.first, p.second);
		return __n;
	}

}; // namespace ft

#endif /* _FT_TREE_HPP */
