#ifndef RBTREE_HPP
# define RBTREE_HPP

# include "utility.hpp"

namespace ft {

	enum	rbTreeColor { _RED, _BLACK };
			
	/*************************************************************
	* rb tree
	*************************************************************/
	template < class Key, class T, class Compare = std::less<Key>, class Alloc = std::allocator< ft::pair<const Key, T> > >
	class	rbTree
	{
	public:

		/*************************************************************
		* node
		*************************************************************/
		struct	node
		{
			typedef ft::pair<const Key, T>	valuetype;

			node		*parent, *left, *right;
			rbTreeColor	color;
			valuetype	data;

			node()	: data(), parent(), left(), right(), color(BLACK) {}

			node(const valuetype &data, node *parent)
					: data(data), parent(parent), left(), right(), color(BLACK) {}

			node	&operator=(const node &obj)
			{
				data = obj.data;
				parent = obj.parent; left = obj.left; right = obj.right;
				color = obj.color;
				return *this;
			}

			const Key	&getKey(void) { return _data.first; }
			T			&getValue(void) { return _data.second; }
		};


		/*************************************************************
		* iterator
		*************************************************************/
		class	iterator
		{
		public:
			
			// Types
			typedef std::ptrdiff_t					difference_type;
			typedef ft::pair<const Key, T>			value_type;
			typedef value_type*						pointer;
			typedef const value_type*				const_pointer;
			typedef value_type&						reference;
			typedef const value_type&				const_reference;
			typedef std::bidirectional_iterator_tag	iterator_category;

			iterator() : _node() {}
			iterator(const node *node) : _node(node) {}
			iterator(const iterator &it) : _node(it.base()) {}
			~iterator() {}

			// Getter
			pointer	base(void) const { return _node; }

			// Assigning operator
			iterator& 	operator=(const iterator& x) { _node = x.base(void); return  *this; }

			// Accessing operators
			reference	operator*(void) const { return _node->data; }
			pointer		operator->(void) const { return &_node->value; }

			// Incrementing operators
			iterator&	operator++(void) {
				if (_node->right != _node->right->left)
				{
					_node = _node->right;
					while (_node->left != _node->left->left)
						_node = _node->left;
				}
				else
				{
					while (_node == _node->parent->right && _node != _node->parent)
						_node = _node->parent;
					_node = _node->parent;
				}	
				return *this;
			}
			iterator	operator++(int) {
				iterator	tmp(*this);
				++(*this);
				return tmp;
			}
			iterator&	operator--(void) { --_node; return *this; }
			iterator	operator--(int) {
				iterator	tmp(*this);
				--(*this);
				return tmp;
			}


		private:

			node	*_node;
		};


	private:

		node	*root;
	};

};

#endif
