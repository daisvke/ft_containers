namespace ft {
	
	template <class T, class Alloc = std::allocator<T> >
	
	class vector {
	
	public:
		// types:
		typedef Alloc										allocator_type;
		typedef typename allocator_type::reference			reference;
		typedef typename allocator_type::const_reference	const_reference;
		typedef T*											iterator;
		typedef const T*									const_iterator;
		typedef std::size_t									size_type;
		typedef std::ptrdiff_t								difference_type;
		typedef T											value_type;
		typedef typename allocator_type::pointer			pointer;
		typedef typename allocator_type::const_pointer		const_pointer;
		typedef std::reverse_iterator<iterator>				reverse_iterator;
		typedef std::reverse_iterator<const_iterator>		const_reverse_iterator;
