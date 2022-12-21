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
	struct rbNode
	{
		typedef ft::pair<const Key, T>	value_type;

		rbNode		*parent, *left, *right;
		rbTreeColor	color;
		value_type	data;

		rbNode() : data(), parent(), left(), right(), color(_BLACK) {}

		rbNode(value_type &data, rbNode *parent)
				: data(data), parent(parent), left(), right(), color(_BLACK) {}

		rbNode	&operator=(const rbNode &obj)
		{
			data = obj.data;
			parent = obj.parent; left = obj.left; right = obj.right;
			color = obj.color;
			return *this;
		}

		const Key	&key(void) { return data.first; }
		T			&value(void) { return data.second; }
	};


	/*************************************************************
	|* iterator
	*************************************************************/
	template < class Key, class T, class Compare = std::less<Key> >
	class	rbIterator
	{
		public:
			
			/*************************************************************
			 * Types
			*************************************************************/
			typedef std::ptrdiff_t					difference_type;
			typedef ft::pair<const Key, T>			value_type;
			typedef ft::rbNode<const Key, T>		node_type;
			typedef value_type*						pointer;
			typedef const value_type*				const_pointer;
			typedef value_type&						reference;
			typedef const value_type&				const_reference;
		//	typedef std::bidirectional_iterator_tag	iterator_category;
			
			/*************************************************************
			 * Construct/Copy/Destroy
			*************************************************************/
			rbIterator() : _node() {}
			rbIterator(const node_type *node) : _node(node) {}
			rbIterator(const rbIterator &it) : _node(it.base()) {}
			~rbIterator() {}

			/*************************************************************
			 * Getters
			*************************************************************/
			pointer	base(void) const { return _node; }

			// Go as far left from the node as possible = find the min node in subtree
			void leftmost(void) { while (_node->left) _node = _node->left; }
			// Go as far right from the node as possible = find the max node in subtree
			void rightmost(void) { while (_node->right) _node = _node->right; }

			/*************************************************************
			 * Assigning operator
			*************************************************************/
			rbIterator& 	operator=(const rbIterator& x) { _node = x.base(); return  *this; }

			/*************************************************************
			 * Accessing operators
			*************************************************************/
			reference	operator*(void) const { return _node->data; }
			pointer		operator->(void) const { return &_node->value; }

			/*************************************************************
			 * Incrementing operators
			*************************************************************/
			// Get the next node in key value order
			rbIterator&	operator++(void) {
				// If there is a right subtree, go to its leftmost (=minimal) node
				if (_node->right) { _node = _node->right; leftmost(); }
				else
				{
					// Otherwise go up the tree, looking for a node
					//  that is its parent's left child.
					while (_node->parent && _node != _node->parent->left)
						_node = _node->parent;
					_node = _node->parent;
				}	
				return *this;
			}
	/*		rbIterator	operator++(int) {
				rbIterator	tmp(*this);
				++(*this);
				return tmp;
			}
	*/
			// Get the previous node in key value order
			rbIterator&	operator--(void) {
				// If there is a left subtree, go to its rightmost (=maximal) node
				if (_node->left) { _node = _node->left; rightmost(); }
				else
				{
					// Otherwise go up the tree, looking for a node
					//  that is its parent's right child.
					while (_node->parent && _node != _node->parent->right)
						_node = _node->parent;
					_node = _node->parent;
				}	
				return *this;
			}
	/*
			rbIterator	operator--(int) {
				rbIterator	tmp(*this);
				--(*this);
				return tmp;
			}
	*/


		private:

			node_type	*_node;
	}; // End of rbIterator
	
}; // End of namespace ft

#endif
