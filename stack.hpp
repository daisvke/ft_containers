#ifndef _FT_STACK_HPP
# define _FT_STACK_HPP

# include "vector.hpp"

namespace ft {

	/*************************************************************
	 * stack
	
	 * stacks are implemented as container adaptors, which are
	 	classes that use an encapsulated object of a specific container
		class as its underlying container, providing a specific set of
		member functions to access its elements. 

	 * Elements are pushed/popped from the "back" of the specific
	 	container, which is known as the top of the stack.
	*************************************************************/
	template < class T, class Container = ft::vector<T> >
	class stack
	{
		public:
			/*************************************************************
			 * Types
			*************************************************************/
			typedef typename Container::value_type	value_type;
			typedef typename Container::size_type	size_type;
			typedef Container						container_type;
			
		
		protected:

			Container c;
		
		
		public:
		
			/*************************************************************
		 	* Construct/Copy/Destroy
			*************************************************************/
			explicit stack(const container_type& cntr = container_type())
				: c(cntr) {}
			
			~stack() {}

			/*************************************************************
			 * Member functions
			*************************************************************/
			bool		empty() const { return c.empty(); }
			
			size_type	size() const { return c.size(); }
			
			value_type&			top() { return c.back(); }
			
			const value_type&	top() const { return c.back(); }
			
			void	push(const value_type& x) { c.push_back(x); }
			
			void	pop() { c.pop_back(); }
			
	}; // stack
			

	/*************************************************************
	 * Boolean operators for reverse iterators
	*************************************************************/
	template <class T, class Container>
	bool operator==(const stack<T, Container>& x, const stack<T, Container>& y)
	{ return x.c == y.c; }
	
	template <class T, class Container>
	bool operator< (const stack<T, Container>& x, const stack<T, Container>& y)
	{ return x.c < y.c; }
	
	template <class T, class Container>
	bool operator!=(const stack<T, Container>& x, const stack<T, Container>& y)
	{ return !(x == y); }
	
	template <class T, class Container>
	bool operator> (const stack<T, Container>& x, const stack<T, Container>& y)
	{ return (y < x); }
	
	template <class T, class Container>
	bool operator>=(const stack<T, Container>& x, const stack<T, Container>& y)
	{ return !(x < y); }
	
	template <class T, class Container>
	bool operator<=(const stack<T, Container>& x, const stack<T, Container>& y)
	{ return !(y < x); }

} // namespace ft

#endif /* _FT_STACK_HPP */