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
			// Go as far left from this node as possible.
			// i.e. find the minimum node in this subtree
			Node* leftmost(Node* node)
			{
			    if (node == 0)
				return 0;
			    while (node->left != 0)
				node = node->left;
			    return node;
			}

			// Return the next node in key value order.
			iterator	nextNode() {

		    // If there is a right subtree, iterate over it,
		    // starting at its leftmost (=minimal) node

		    if (node->right != 0)
			return Leftmost(node->right);
		    
		    // Otherwise we must go up the tree

		    node *parent = node->parent;
		    if (parent == 0)
			return 0;

		    // If we came from the left subtree of a parent node, go back to parent

		    if (node == parent->left)
			return parent;

		    // So, we must be in the right subtree of the parent.
		    // In which case we need to go up again, looking for a node that is
		    // its parent's left child.

		    while (parent != 0 && node != parent->left)
		    {
			node = parent;
			parent = node->parent;
		    }
		    return parent;
}
			}


		private:

			node	*_node;
		};


	private:

		node	*root;
	};

};

#endif
