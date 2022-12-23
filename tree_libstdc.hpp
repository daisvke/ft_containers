#ifndef _FT_TREE_CC_HPP
# define _FT_TREE_CC_HPP

/*************************************************************
 * A Red-Black Tree utilities implementation
*************************************************************/

namespace ft {

	enum	rb_tree_color { _red = false, _black = true };

	/*************************************************************
	 * Base node
	*************************************************************/
	struct rb_tree_node_base
	{
 		typedef rb_tree_node_base*			base_ptr;
		typedef const rb_tree_node_base*	const_base_ptr;

		rb_tree_color	_color;
		base_ptr		_parent;
		base_ptr		_left;
		base_ptr		_right;

		static base_ptr			minimum(base_ptr x)
		{ while (x->_left != 0) x = x->_left; return x; }

		static const_base_ptr	minimum(const_base_ptr x)
		{ while (x->_left != 0) x = x->_left; return x; }

		static base_ptr			maximum(base_ptr x)
		{ while (x->_right != 0) x = x->_right; return x; }

		static const_base_ptr	maximum(const_base_ptr x)
		{ while (x->_right != 0) x = x->_right; return x; }
	};

	
	/*************************************************************
	 * Incrementing functions
	*************************************************************/
	static rb_tree_node_base *rb_tree_increment(rb_tree_node_base *x) {
		// If there is a right subtree, go to its leftmost (=minimal) node
		if (x->_right != 0) 
			{
				x = x->_right;
				while (x->_left != 0)
					x = x->_left;
			}
		else {
			// Otherwise go up the tree, looking for a node
			//  that is its parent's left child
			rb_tree_node_base* y = x->_parent;
			while (x == y->_right) {
				x = y;
				y = y->_parent;
			}
			// Handle end() case
			if (x->_right != y)
				x = y;
		}
		return x;
	}

	const rb_tree_node_base*	rb_tree_increment(const rb_tree_node_base* x)
	{ return rb_tree_increment(const_cast<rb_tree_node_base*>(x)); }

	static rb_tree_node_base*	rb_tree_decrement(rb_tree_node_base* x)
	{
		// Handle end() case
		if (x->_color == _red && x->_parent->_parent == x)
			x = x->_right;
		else if (x->_left != 0) {
			// If there is a left subtree, go to its rightmost (=max) node
			rb_tree_node_base* y = x->_left;
			while (y->_right != 0)
				y = y->_right;
			x = y;
		}
		else {
			// Otherwise go up the tree, looking for a node
			//  that is its parent's right child.
			rb_tree_node_base* y = x->_parent;
			while (x == y->_left) {
				x = y;
				y = y->_parent;
			}
			x = y;
		}
		return x;
	}

	const rb_tree_node_base*	rb_tree_decrement(const rb_tree_node_base* x)
	{ return rb_tree_decrement(const_cast<rb_tree_node_base*>(x)); }


	/*************************************************************
	 * Rotation functions
	*************************************************************/
	static void	rb_tree_rotate_left(rb_tree_node_base* const x,
										  rb_tree_node_base*& root)
	{
		rb_tree_node_base* const y = x->_right;

		x->_right = y->_left;
		if (y->_left !=0)
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

	static void	rb_tree_rotate_right(rb_tree_node_base* const x,
									 rb_tree_node_base*& root)
	{
		rb_tree_node_base* const y = x->_left;

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
	void	rb_tree_insert_and_rebalance(const bool insert_left,
								rb_tree_node_base* x,
								rb_tree_node_base* p,
								rb_tree_node_base& header)
	{
		rb_tree_node_base *& _root = header._parent;

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
			rb_tree_node_base* const xpp = x->_parent->_parent;

			if (x->_parent == xpp->_left)
			{
				rb_tree_node_base* const y = xpp->_right;
				if (y && y->_color == _red)
				{
					x->_parent->_color = _black;
					y->_color = _black;
					xpp->_color = _red;
					x = xpp;
				}
				else
				{
				if (x == x->_parent->_right) {
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
				rb_tree_node_base* const y = xpp->_left;
				if (y && y->_color == _red)
				{
					x->_parent->_color = _black;
					y->_color = _black;
					xpp->_color = _red;
					x = xpp;
				}
				else
				{
					if (x == x->_parent->_left) {
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
	rb_tree_node_base*	rb_tree_rebalance_for_erase(rb_tree_node_base* const z,
													rb_tree_node_base& header)
	{
		rb_tree_node_base *& 	root = header._parent;
		rb_tree_node_base *& 	leftmost = header._left;
		rb_tree_node_base *& 	rightmost = header._right;
		rb_tree_node_base* 		y = z;
		rb_tree_node_base* 		x = 0;
		rb_tree_node_base* 		x_parent = 0;

		if (y->_left == 0)     // z has at most one non-null child. y == z.
			x = y->_right;     // x might be null.
		else
			if (y->_right == 0)  // z has exactly one non-null child. y == z.
				x = y->_left;    // x is not null.
		else
		{
			// z has two non-null children.  Set y to
			y = y->_right;   //   z's successor.  x might be null.
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
				if (x) x->_parent = y->_parent;
				y->_parent->_left = x;   // y must be a child of _left
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
		{   // y == z
			x_parent = y->_parent;
			if (x)
				x->_parent = y->_parent;
			if (root == z)
				root = x;
			else
				if (z->_parent->_left == z)
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
				else  // x == z->_left
					rightmost = rb_tree_node_base::maximum(x);
				}
		}
		if (y->_color != _red)
		{
			while (x != root && (x == 0 || x->_color == _black))
				if (x == x_parent->_left) {
					rb_tree_node_base* w = x_parent->_right;
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
						rb_tree_node_base* w = x_parent->_left;
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
			if (x) x->_color = _black;
		}
		return y;
	} // rb_tree_rebalance_for_erase

}; // namespace ft
  
#endif /* _FT_TREE_CC_HPP */