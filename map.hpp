#ifndef _FT_MAP_HPP
# define _FT_MAP_HPP

# include <functional>
# include <memory>

# include "algorithm.hpp"
# include "type_traits.hpp"
# include "iterator.hpp"
# include "utility.hpp"
# include "tree.hpp"

namespace ft {
				
	template <typename Key, typename T, typename Compare = std::less<Key>,
		typename Alloc = std::allocator< ft::pair<const Key, T> > >
	class map {

	public:

		/*************************************************************
		 * Types
		*************************************************************/
		typedef Key														key_type;
		typedef T														mapped_type;
		typedef ft::pair<const key_type, mapped_type>					value_type;
		typedef Compare													key_compare;
		typedef Alloc													allocator_type;

		/*************************************************************
		 * value_compare
		 * In C++98, it is required to inherit binary_function<value_type,value_type,bool>
		*************************************************************/
		class value_compare : public std::binary_function<value_type, value_type, bool> {   
			friend class map;

			protected:
				Compare comp;
				value_compare (Compare c) : comp(c) {} // constructed with map's comparison object

			public:
				typedef bool		result_type;
				typedef value_type	first_argument_type;
				typedef value_type	second_argument_type;

				bool operator() (const value_type& x, const value_type& y) const
				{ return comp(x.first, y.first); }
		};

      	typedef typename __gnu_cxx::__alloc_traits<Alloc>::template
		rebind<value_type>::other							pair_alloc_type;

		typedef rb_tree<key_type, value_type, std::_Select1st<value_type>,
    					key_compare, pair_alloc_type>		rep_type;

    	/// The actual tree structure.
      	rep_type	_t;

		typedef __gnu_cxx::__alloc_traits<pair_alloc_type>	alloc_traits;


	public:

		typedef typename alloc_traits::pointer				pointer;
		typedef typename alloc_traits::const_pointer		const_pointer;
		typedef typename alloc_traits::reference			reference;
		typedef typename alloc_traits::const_reference		const_reference;
		typedef typename rep_type::iterator					iterator;
		typedef typename rep_type::const_iterator			const_iterator;
		typedef typename rep_type::size_type				size_type;
		typedef typename rep_type::difference_type			difference_type;
		typedef typename rep_type::reverse_iterator			reverse_iterator;
		typedef typename rep_type::const_reverse_iterator	const_reverse_iterator;

		/*************************************************************
		 * Construct/Copy/Destroy
		*************************************************************/
		// Default constructor creates no elements
	    map() : _t() {}

		explicit map(const key_compare& comp,
			const allocator_type& alloc = allocator_type())
			: _t(comp), pair_alloc_type(alloc) {}

		map(const map& x) : _t(x._t) {};
		
		// Builds a map from a range
		// Create a map consisting of copies of the elements from
    	// [__first,__last).  This is linear in N if the range is
    	// already sorted, and NlogN otherwise (where N is distance(__first,__last)).
		template <typename InputIterator>
		map(InputIterator first, InputIterator last) : _t()
		{ _t.insert_range_unique(first, last); }

		template <typename InputIterator>
		map(InputIterator first, InputIterator last, const Compare& comp,
			const allocator_type& alloc = allocator_type())
			: _t(comp, pair_alloc_type(alloc))
		{ _t.insert_range_unique(first, last); }

		/*************************************************************
		 *  Assignment operator
		*************************************************************/
		map&	operator=(const map& x)
		{ _t = x._t; return *this; }

		/*************************************************************
		 *  Get a copy of the memory allocation object
		*************************************************************/
		allocator_type	get_allocator() const
		{ return allocator_type(_t.get_allocator()); }

		/*************************************************************
		 * Iterators
		*************************************************************/
		// Return a iterator that points to the first pair in the map.
    	// Iteration is done in ascending order according to the keys.
		iterator				begin(void) { return _t.begin(); }
		const_iterator			begin(void) const  { return _t.begin(); }

		// Return a iterator that points to the one past the last pair in the map.
    	// Iteration is done in ascending order according to the keys.
		iterator				end(void) { return _t.end(); }
		const_iterator			end(void) const { return _t.end(); }

		// Return a reverse iterator that points to the last pair in the map.
    	// Iteration is done in descending order according to the keys.
		reverse_iterator		rbegin(void) { return _t.rbegin(); }
		const_reverse_iterator	rbegin(void) const { return _t.rbegin(); }

		// Return a reverse iterator that points to the one before
		// the first pair in the map.
    	// Iteration is done in descending order according to the keys.
		reverse_iterator		rend(void) { return _t.rend(); }
		const_reverse_iterator	rend(void) const { return _t.rend(); }

		/*************************************************************
		 * Capacity
		*************************************************************/
	    // Returns true if the map is empty. (Thus begin() would equal end().)
		bool 		empty() const { return _t.empty(); }
		// Returns the size of the map
		size_type 	size() const { return _t.size(); }
		// Returns the maximum size of the map
		size_type	max_size() const { return _t.max_size(); }

		/*************************************************************
		 * Element access
		*************************************************************/
		// Allows for easy lookup with the subscript ( [] )
    	// operator.  Returns data associated with the key specified in
    	// subscript.  If the key does not exist, a pair with that key
    	// is created using default values, which is then returned.
		mapped_type& operator[](const key_type& k)
		{
			iterator	i = lower_bound(k);
			// i->first is greater than or equivalent to k.
			if (i == end() || key_comp()(k, (*i).first))
				i = insert(const_iterator(i), value_type(k, mapped_type()));
			return (*i).second;
    	}

		// Access to %map data.
    	// k  The key for which data should be retrieved.
    	//  A reference to the data whose key is equivalent to k, if
    	//  such a data is present in the map.
    	// std::out_of_range  If no such data is present.
		mapped_type&		at(const key_type& k)
		{
			iterator i = lower_bound(k);
			if (i == end() || key_comp()(k, (*i).first))
				std::__throw_out_of_range(__N("map::at"));
			return (*i).second;
		}

		const mapped_type&	at(const key_type& k) const
		{
			const_iterator i = lower_bound(k);
			if (i == end() || key_comp()(k, (*i).first))
				std::__throw_out_of_range(__N("map::at"));
			return (*i).second;
		}

		/*************************************************************
		 * Modifiers
		*************************************************************/
	 	// Attempts to insert a ft::pair into the map.
		// returns a pair, of which the first element is an iterator that
    	        //  points to the possibly inserted pair, and the second is
    	        //  a bool that is true if the pair was actually inserted.	
    	// This function attempts to insert a (key, value) pair into the map.
    	// A %map relies on unique keys and thus a %pair is only inserted if its
    	// first element (the key) is not already present in the map.
		ft::pair<iterator, bool> insert(const value_type& x)
		{ return _t.insert_unique(x); }

		// position: An iterator that serves as a hint as to where the
                // pair should be inserted.
		// This function is not concerned about whether the insertion
		// took place, and thus does not return a boolean like the
		// single-argument insert() does.  Note that the first
		// parameter is only a hint and can potentially improve the
		// performance of the insertion process.  A bad hint would
		// cause no gains in efficiency.
		iterator	insert(const_iterator position, const value_type& x)
		{ return _t.insert_unique(position, x); }

		// Template function that attempts to insert a range of elements
		template <typename InputIterator>
		void		insert(InputIterator first, InputIterator last)
		{ return _t.insert_range_unique(first, last); }

    	// This function erases an element, pointed to by the given
    	// iterator, from a %map.  Note that this function only erases
    	// the element, and that if the element is itself a pointer,
    	// the pointed-to memory is not touched in any way.  Managing
    	// the pointer is the user's responsibility.
		void		erase(iterator position) { _t.erase(position); }

		// Erases elements according to the provided key
		size_type	erase(const key_type& x) { return _t.erase(x); }

		// Erases a [__first,__last) range of elements from a map
		void		erase(iterator first, iterator last) { _t.erase(first, last); }

		// Swaps data with another map
		void		swap(map& x) { _t.swap(x._t); }

		// Erases all elements in a map
		void		clear() { _t.clear(); }

		/*************************************************************
		 * Observers
		*************************************************************/
		// Returns the key comparison object out of which the map was constructed.
		key_compare		key_comp() const { return _t.key_comp(); }
		// Returns a value comparison object, built from the key comparison
    	//	object out of which the map was constructed.
		value_compare	value_comp() const { return value_compare(_t.key_comp()); }

		/*************************************************************
		 * Map operations
		*************************************************************/
		// These functions take a key and tries to locate the element with which
    	// the key matches.  If successful the functions return a constant
    	// iterator pointing to the sought after pair. If unsuccessful they
    	// return the past-the-end
		iterator		find(const key_type& x) { return _t.find(x); }
		const_iterator	find(const key_type& x) const { return _t.find(x); }

       	// This function only makes sense for multimaps; for map the result will
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
		// This function probably only makes sense for multimaps.
		pair<iterator,iterator>				equal_range(const key_type& x)
		{ return _t.equal_range(x); }
		pair<const_iterator,const_iterator>	equal_range(const key_type& x) const
		{ return _t.equal_range(x); }

		/*************************************************************
		 *  Relational operators
		*************************************************************/

		template<typename _K1, typename _T1, typename _C1, typename _A1>
		friend bool	operator==(const map<_K1, _T1, _C1, _A1>&,
			const map<_K1, _T1, _C1, _A1>&);

		template<typename _K1, typename _T1, typename _C1, typename _A1>
		friend bool	operator<(const map<_K1, _T1, _C1, _A1>&,
			const map<_K1, _T1, _C1, _A1>&);
		  		  
	}; // Map


	/*************************************************************
	 *  Relational operators
	*************************************************************/
   	// This is an equivalence relation.  It is linear in the size of the
   	// maps.  Maps are considered equivalent if their sizes are equal,
   	// and if corresponding elements compare equal.
	template <typename Key, typename T, typename key_compare, typename allocator_type>
	bool operator==(const map<Key,T,key_compare,allocator_type>& x,
		const map<Key,T,key_compare,allocator_type>& y)
	{ return x._t == y._t; }

	template <typename Key, typename T, typename key_compare, typename allocator_type>
	bool operator< (const map<Key,T,key_compare,allocator_type>& x,
		const map<Key,T,key_compare,allocator_type>& y)
	{ return x._t < y._t; }

	template <typename Key, typename T, typename key_compare, typename allocator_type>
	bool operator!=(const map<Key,T,key_compare,allocator_type>& x,
		const map<Key,T,key_compare,allocator_type>& y)
	{ return !(x._t == y._t); }

	template <typename Key, typename T, typename key_compare, typename allocator_type>
	bool operator> (const map<Key,T,key_compare,allocator_type>& x,
		const map<Key,T,key_compare,allocator_type>& y)
	{ return !(y < x); }

	template <typename Key, typename T, typename key_compare, typename allocator_type>
	bool operator>=(const map<Key,T,key_compare,allocator_type>& x,
		const map<Key,T,key_compare,allocator_type>& y)
	{ return !(y < x); }

	template <typename Key, typename T, typename key_compare, typename allocator_type>
	bool operator<=(const map<Key,T,key_compare,allocator_type>& x,
		const map<Key,T,key_compare,allocator_type>& y)
	{ return !(x < y); }

	// specialized algorithms:
	template <typename Key, typename T, typename key_compare, typename allocator_type>
	void swap(map<Key,T,key_compare,allocator_type>& x,
	map<Key,T,key_compare,allocator_type>& y)
	{ x.swap(y); }

} // namespace ft


#endif /* _FT_MAP_HPP */
