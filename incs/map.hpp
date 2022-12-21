#ifndef MAP_HPP
# define MAP_HPP

# include <functional>
# include <memory>

# include "rbTree.hpp"

namespace ft {
				
	template <class Key, class T, class Compare = std::less<Key>,
		class Alloc = std::allocator< ft::pair<const Key, T> > >
	class map {

	public:


		/*************************************************************
		 * Types
		*************************************************************/
		typedef Key														key_type;
		typedef T														mapped_type;
		typedef ft::pair<const key_type, mapped_type>					value_type;
		typedef ft::rbNode<const key_type, mapped_type>					node_type;
		typedef Compare													key_compare;
		typedef Alloc													allocator_type;
		typedef typename allocator_type::reference						reference;
		typedef typename allocator_type::const_reference				const_reference;
		typedef ft::rbIterator<const Key, T>							iterator;
		typedef ft::rbIterator<const Key, const T>						const_iterator;
		typedef typename ft::rbIterator<const Key, T>::size_type		size_type;
		typedef typename ft::rbIterator<const Key, T>::difference_type	difference_type;
		typedef typename allocator_type::pointer						pointer;
		typedef typename allocator_type::const_pointer					const_pointer;
		typedef ft::reverse_iterator<iterator>							reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>					const_reverse_iterator;



		// in C++98, it is required to inherit binary_function<value_type,value_type,bool>
		class value_compare : public std::binary_function<value_type, value_type, bool> {   
			friend class map;

			protected:
				Compare comp;
				value_compare (Compare c) : comp(c) {} // constructed with map's comparison object

			public:
				typedef bool result_type;
				typedef value_type first_argument_type;
				typedef value_type second_argument_type;

				bool operator() (const value_type& x, const value_type& y) const
				{ return comp(x.first, y.first); }
		};

		/*************************************************************
		 * Construct/Copy/Destroy
		*************************************************************/
		explicit map(const key_compare& comp = key_compare(),
			const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _root(), _comp(comp, _size()) {}
		
		template <class InputIterator>
		map(InputIterator first, InputIterator last,
			const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _root(), _comp(comp, _size()) {
			insert(first, last);
		}

		map(const map<Key,T,key_compare,allocator_type>& x) {};

		~map() {};

		map<Key,T,key_compare,allocator_type>&
		operator=(const map<Key,T,key_compare,allocator_type>& x);

		/*************************************************************
		 * Iterators
		*************************************************************/
		iterator				begin() { return iterator(iterator(_root).leftmost()); }
		const_iterator			begin() const { return const_iterator(const_iterator(_root).leftmost()); }
		iterator				end() { ; }

		const_iterator end() const;
		reverse_iterator rbegin();
		const_reverse_iterator rbegin() const;
		reverse_iterator rend();
		const_reverse_iterator rend() const;

		/*************************************************************
		 * Capacity
		*************************************************************/
		bool empty() const { return _size == 0; }

		size_type size() const { return _size; }
		size_type max_size() const { return _alloc.max_size(); }

		/*************************************************************
		 * Element access
		*************************************************************/
		T& operator[](const key_type& x);

		/*************************************************************
		 * Modifiers
		*************************************************************/
		ft::pair<iterator, bool> insert(const value_type& x) {
			
		}

		iterator insert(iterator position, const value_type& x);

		template <class InputIterator>
		void insert(InputIterator first, InputIterator last);

		void erase(iterator position);

		size_type erase(const key_type& x);

		void erase(iterator first, iterator last);

		void swap(map<Key,T,Compare,allocator_type>& x) {
			ft::swap(x._root, _root);
			ft::swap(x._size, _size);
		}

		void clear() {
			if (_size > 0)
				erase(begin(), end());
			_root = 0;
		}

		/*************************************************************
		 * Observers
		*************************************************************/
		key_compare		key_comp() const { return key_compare(); }
		value_compare	value_comp() const { return value_compare(_comp); }

		/*************************************************************
		 * Map operations
		*************************************************************/
		iterator		find(const key_type& x) {
			
		}

		const_iterator	find(const key_type& x) const;

		size_type		count(const key_type& x) const {
			size_type	n = 0;
			for (const_iterator it = begin(); it != end(); ++it)
			{
			}
		}

		iterator lower_bound(const key_type& x);
		const_iterator lower_bound(const key_type& x) const;
		iterator upper_bound(const key_type& x);
		const_iterator upper_bound(const key_type& x) const;
		pair<iterator,iterator>
		equal_range(const key_type& x);
		pair<const_iterator,const_iterator>
		equal_range(const key_type& x) const;


	private:

		/*************************************************************
		 * The past-the-end element is the theoretical element that would
		 	follow the last element in the map container. It does not 
			point to any element, and thus shall not be dereferenced.
		*************************************************************/
		iterator	past_the_end(void)
		{
			node_type	pastTheEndNode = new node_type();
			iterator	rightmostNode(_root); rightmostNode.rightmost();

			pastTheEndNode.parent = rightmostNode.base();
			rightmostNode.base().right = pastTheEndNode;
			return iterator(pastTheEndNode);
		}

		allocator_type	_alloc;
		value_type		_root;
		key_compare		_comp;
		size_type		_size;

	}; // End of Map


	template <class Key, class T, class key_compare, class allocator_type>
	bool operator==(const map<Key,T,key_compare,allocator_type>& x,
	const map<Key,T,key_compare,allocator_type>& y);
	template <class Key, class T, class key_compare, class allocator_type>
	bool operator< (const map<Key,T,key_compare,allocator_type>& x,
	const map<Key,T,key_compare,allocator_type>& y);
	template <class Key, class T, class key_compare, class allocator_type>
	bool operator!=(const map<Key,T,key_compare,allocator_type>& x,
	const map<Key,T,key_compare,allocator_type>& y);
	template <class Key, class T, class key_compare, class allocator_type>
	bool operator> (const map<Key,T,key_compare,allocator_type>& x,
	const map<Key,T,key_compare,allocator_type>& y);
	template <class Key, class T, class key_compare, class allocator_type>
	bool operator>=(const map<Key,T,key_compare,allocator_type>& x,
	const map<Key,T,key_compare,allocator_type>& y);
	template <class Key, class T, class key_compare, class allocator_type>
	bool operator<=(const map<Key,T,key_compare,allocator_type>& x,
	const map<Key,T,key_compare,allocator_type>& y);

	// specialized algorithms:
	template <class Key, class T, class key_compare, class allocator_type>
	void swap(map<Key,T,key_compare,allocator_type>& x,
	map<Key,T,key_compare,allocator_type>& y);
}


#endif
