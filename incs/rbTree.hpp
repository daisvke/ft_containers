#ifndef RBTREE_HPP
# define RBTREE_HPP

# include "utility.hpp"

/*************************************************************
 * A Red-Black Tree implementation.

 * This is used in Map and Set classes
*************************************************************/

namespace ft {

	enum	rbTreeColor { _RED, _BLACK };
		

	/*************************************************************
	|* node
	*************************************************************/
	template < class Key, class T>
	typedef struct rbNode	s_rbNode
	{
		typedef ft::pair<const Key, T>	value_type;

		rbNode		*parent, *left, *right;
		rbTreeColor	color;
		value_type	data;

		rbNode() : data(), parent(), left(), right(), color(BLACK) {}

		rbNode(value_type &data, rbNode *parent)
				: data(data), parent(parent), left(), right(), color(BLACK) {}

		rbNode	&operator=(const rbNode &obj)
		{
			data = obj.data;
			parent = obj.parent; left = obj.left; right = obj.right;
			color = obj.color;
			return *this;
		}

		const Key	&key(void) { return data.first; }
		T			&value(void) { return data.second; }
	}	rbNode;


	/*************************************************************
	|* iterator
	*************************************************************/
	template < class Key, class T, class Compare = std::less<Key> >
	class	rbIterator
	{
		private:

			node	*_node;


		public:
			
			// Types
			typedef std::ptrdiff_t					difference_type;
			typedef ft::pair<const Key, T>			value_type;
			typedef value_type*						pointer;
			typedef const value_type*				const_pointer;
			typedef value_type&						reference;
			typedef const value_type&				const_reference;
		//	typedef std::bidirectional_iterator_tag	iterator_category;

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

			// Go as far left from the node as possible = find the min node in subtree
			void leftmost(void) { while (_node->left) _node = _node->left; }

			// Go as far right from the node as possible = find the max node in subtree
			void leftmost(void) { while (_node->left) _node = _node->left; }
			// Incrementing operators

			// Get the next node in key value order
			iterator&	operator++(void) {
				// If there is a right subtree, go to its leftmost (=minimal) node
				if (_node->right)
				{
					_node = _node->right;
					leftmost();
				}
				else
				{
					// Otherwise go up the tree, looking for a node
					//  that is its parent's left child.
					while (_node->parent && _node != parent->left)
						_node = _node->parent;
					_node = _node->parent;
				}	
				return *this;
			}
	/*		iterator	operator++(int) {
				iterator	tmp(*this);
				++(*this);
				return tmp;
			}
	*/
			iterator&	operator--(void) { --_node; return *this; }
	/*
			iterator	operator--(int) {
				iterator	tmp(*this);
				--(*this);
				return tmp;
			}
	*/

	};

};

#endif
