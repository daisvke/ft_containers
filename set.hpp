#ifndef _FT_SET_HPP
# define _FT_SET_HPP

# include <functional>
# include <memory>

# include "algorithm.hpp"
# include "type_traits.hpp"
# include "iterator.hpp"
# include "utility.hpp"
# include "tree.hpp"

namespace ft {
				
	template <typename Key, typename Compare = std::less<Key>,
		typename Alloc = std::allocator<Key> >
	class set {

	public:

		/*************************************************************
		 * Types
		*************************************************************/
		typedef Key		key_type;
		typedef Key		value_type;
		typedef Compare	key_compare;
		typedef Compare value_compare;
		typedef Alloc	allocator_type;

		typedef typename __gnu_cxx::__alloc_traits<Alloc>::template
		rebind<Key>::other _Key_alloc_type;

		typedef rb_tree<key_type, value_type, std::_Identity<value_type>,
				key_compare, _Key_alloc_type> rep_type;

		rep_type _t;  // Red-black tree representing set.

		typedef __gnu_cxx::__alloc_traits<_Key_alloc_type> alloc_traits;

		typedef typename alloc_traits::pointer				pointer;
		typedef typename alloc_traits::const_pointer		const_pointer;
		typedef typename alloc_traits::reference			reference;
		typedef typename alloc_traits::const_reference		const_reference;
		typedef typename rep_type::const_iterator			iterator;
		typedef typename rep_type::const_iterator			const_iterator;
		typedef typename rep_type::size_type				size_type;
		typedef typename rep_type::difference_type			difference_type;
		typedef typename rep_type::const_reverse_iterator	reverse_iterator;
		typedef typename rep_type::const_reverse_iterator	const_reverse_iterator;

		/*************************************************************
		 * Construct/Copy/Destroy
		*************************************************************/
		// Default constructor creates no elements
	    set() : _t() {}

		explicit set(const Compare& comp,
			const allocator_type& alloc = allocator_type())
			: _t(comp), _Key_alloc_type(alloc) {}

		set(const set& x) : _t(x._t) {};
		
		// Builds a set from a range
		// Create a set consisting of copies of the elements from
    	// [first,last).  This is linear in N if the range is
    	// already sorted, and NlogN otherwise (where N is distance(first,last)).
		template <typename InputIterator>
		set(InputIterator first, InputIterator last) : _t()
		{ _t.insert_range_unique(first, last); }

		template <typename InputIterator>
		set(InputIterator first, InputIterator last, const Compare& comp,
			const allocator_type& alloc = allocator_type())
			: _t(comp, allocator_type(alloc))
		{ _t.insert_range_unique(first, last); }

		/*************************************************************
		 *  Assignment operator
		*************************************************************/
		set&	operator=(const set& x)
		{ _t = x._t; return *this; }

		/*************************************************************
		 *  Accessors
		*************************************************************/
		///  Returns the comparison object with which the set was constructed.
		key_compare		key_comp() const { return _t.key_comp(); }
		///  Returns the comparison object with which the sset was constructed.
		value_compare	value_comp() const { return _t.key_comp(); }
		///  Returns the allocator object with which the set was constructed.
		allocator_type	get_allocator() const
		{ return allocator_type(_t.get_allocator()); }

		/*************************************************************
		 * Iterators
		*************************************************************/
		// Return a iterator that points to the first element in the set.
    	// Iteration is done in ascending order according to the keys.
		iterator				begin(void) const { return _t.begin(); }

		// Return a iterator that points to the one past the last element in the set.
    	// Iteration is done in ascending order according to the keys.
		iterator				end(void) const { return _t.end(); }

		// Return a reverse iterator that points to the last element in the set.
    	// Iteration is done in descending order according to the keys.
		reverse_iterator		rbegin(void )const  { return _t.rbegin(); }

		// Return a reverse iterator that points to the one before
		// the first element in the set.
    	// Iteration is done in descending order according to the keys.
		reverse_iterator		rend(void) const { return _t.rend(); }

		/*************************************************************
		 * Capacity
		*************************************************************/
	    // Returns true if the set is empty
		bool 		empty() const { return _t.empty(); }
		// Returns the size of the set
		size_type 	size() const { return _t.size(); }
		// Returns the maximum size of the set
		size_type	max_size() const { return _t.max_size(); }

		/*************************************************************
		 * Modifiers
		*************************************************************/

		// Swaps data with another set
		void		swap(set& x) { _t.swap(x._t); }

    	// This function attempts to insert an element into the set. A set
    	// relies on unique keys and thus an element is only inserted if it is
    	// not already present in the set.
		ft::pair<iterator, bool>	insert(const value_type& x)
		{
			ft::pair<typename rep_type::iterator, bool> p =
			_t.insert_unique(x);
			return ft::pair<iterator, bool>(p.first, p.second);
		}

		iterator	insert(const_iterator position, const value_type& x)
		{ return _t.insert_unique(position, x); }

		// A template function that attempts to insert a range of elements.
		template<typename _InputIterator>
		void	insert(_InputIterator first, _InputIterator last)
		{ _t.insert_range_unique(first, last); }
		
		// Erases an element from a set
		void	erase(iterator position)
		{ _t.erase(position); }

		// Erases elements according to the provided key
		size_type	erase(const key_type& x)
		{ return _t.erase(x); }

		// Erases a [first,last) range of elements from a set
		void	erase(iterator first, iterator last)
		{ _t.erase(first, last); }

		// Erases all elements in a set
		void		clear() { _t.clear(); }

		/*************************************************************
		 * Map operations
		*************************************************************/
		// These functions take a key and tries to locate the element with which
    	// the key matches.  If successful the functions return a constant
    	// iterator pointing to the sought after pair. If unsuccessful they
    	// return the past-the-end
		iterator		find(const key_type& x) { return _t.find(x); }
		const_iterator	find(const key_type& x) const { return _t.find(x); }

       	// This function only makes sense for multisets; for set the result will
       	// either be 0 (not present) or 1 (present).
		size_type		count(const key_type& x) const
		{ return _t.find(x) == _t.end() ? 0 : 1; }

    	// These functions return the first element of a subsequence of elements
    	// that matches the given key.  If unsuccessful they return an iterator
    	// pointing to the first element that has a greater value than given key
    	// or end() if no such element exists.
		iterator 		lower_bound(const key_type& x) { return _t.lower_bound(x); }
		const_iterator	lower_bound(const key_type& x) const
		{ return _t.lower_bound(x); }

		// Theses functions return an iterator pointing to the first element
    	//   greater than key, or end().
		iterator		upper_bound(const key_type& x) { return _t.upper_bound(x); }
		const_iterator	upper_bound(const key_type& x) const
		{ return _t.upper_bound(x); }

		// Find a subsequence matching given key.
		// Return a pair of iterators that possibly points to the subsequence
    	//	matching given key.
		// This function probably only makes sense for multisets.
		pair<iterator,iterator>				equal_range(const key_type& x)
		{ return _t.equal_range(x); }
		pair<const_iterator,const_iterator>	equal_range(const key_type& x) const
		{ return _t.equal_range(x); }

		/*************************************************************
		 *  Relational operators
		*************************************************************/

		template<typename _K1, typename _C1, typename _A1>
		friend bool	operator==(const set<_K1, _C1, _A1>&,
			const set<_K1, _C1, _A1>&);

		template<typename _K1, typename _C1, typename _A1>
		friend bool	operator<(const set<_K1, _C1, _A1>&,
			const set<_K1, _C1, _A1>&);
		  		  
	}; // Map


	/*************************************************************
	 *  Relational operators
	*************************************************************/
   	// This is an equivalence relation.  It is linear in the size of the
   	// sets.  Maps are considered equivalent if their sizes are equal,
   	// and if corresponding elements compare equal.
	template <typename Key, typename key_compare, typename allocator_type>
	bool operator==(const set<Key,key_compare,allocator_type>& x,
		const set<Key,key_compare,allocator_type>& y)
	{ return x._t == y._t; }

	template <typename Key, typename key_compare, typename allocator_type>
	bool operator< (const set<Key,key_compare,allocator_type>& x,
		const set<Key,key_compare,allocator_type>& y)
	{ return x._t < y._t; }

	template <typename Key, typename key_compare, typename allocator_type>
	bool operator!=(const set<Key,key_compare,allocator_type>& x,
		const set<Key,key_compare,allocator_type>& y)
	{ return !(x._t == y._t); }

	template <typename Key, typename key_compare, typename allocator_type>
	bool operator> (const set<Key,key_compare,allocator_type>& x,
		const set<Key,key_compare,allocator_type>& y)
	{ return y < x; }

	template <typename Key, typename key_compare, typename allocator_type>
	bool operator>=(const set<Key,key_compare,allocator_type>& x,
		const set<Key,key_compare,allocator_type>& y)
	{ return !(x < y); }

	template <typename Key, typename key_compare, typename allocator_type>
	bool operator<=(const set<Key,key_compare,allocator_type>& x,
		const set<Key,key_compare,allocator_type>& y)
	{ return !(y < x); }

	// specialized algorithms:
	template <typename Key, typename key_compare, typename allocator_type>
	void swap(set<Key,key_compare,allocator_type>& x,
		set<Key,key_compare,allocator_type>& y)
	{ x.swap(y); }

} // namespace ft


#endif /* _FT_SET_HPP */
