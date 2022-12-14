#ifndef RBTREE_HPP
# define RBTREE_HPP

# include "Utility.hpp"

namespace ft {

	enum	rbTreeColor { _RED, _BLACK };

	/*************************************************************
	* rb tree
	*************************************************************/
	template <class T, class Compare = std::less<Key>, class Alloc = std::allocator<T> >
	class	rbTree
	{
		public:

			/*************************************************************
			* node
			*************************************************************/
			struct	node
			{
				typedef ft::pair<const key, T>	value_type;

				node		*_parent, *_left, *_right;
				rbTreeColor	_color;
				value_type	_data;

				node()	: data(), parent(), left(), right(), color(_BLACK)
				node(const value_type &data, node *parent)
						: _data(data), _parent(parent) {}

				node	&operator=(const node &obj)
				{
					_data = obj.data;
					_parent = obj.parent; _left = obj.left; _right = obj.right;
					_color = obj.color;
					return *this;
				}
			}


			/*************************************************************
			* iterator
			*************************************************************/
			class	iterator
			{
				public:
					
					// Types
					typedef std::ptrdiff_t					difference_type;
					typedef ft::pair<const key, T>			value_type;
					typedef value_type*						pointer;
					typedef const value_type*				const_pointer;
					typedef value_type&						reference;
					typedef const value_type&				const_reference;
					typedef std::bidirectional_iterator_tag	iterator_category;

					iterator() : _node() {}
					iterator(node *node) : _node(node) {}
					iterator(const iterator &it) : _node(it.base()) {}
					~iterator() {}

					// Getter
					pointer	base() const { return _node; }

					// Assigning operator
					iterator& operator= (const iterator& x) { _node = x.base(); return  *this; }

					// Accessing operators
					reference	operator*() const { return _node->data; }
					pointer		operator->() const { return &_node->value; }

				private:

				node	*_node;
			}

			
			/*************************************************************
			* rb tree
			*************************************************************/
			class	rbTree
			{
				private:

				node	*root;
			}
	}

}

#endif
