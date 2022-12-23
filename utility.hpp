#ifndef _FT_UTILITY_HPP
# define _FT_UTILITY_HPP

/*************************************************************
 * A partial utility library implementation
*************************************************************/

namespace ft
{
	/*************************************************************
	 * PAIR 
	*************************************************************/
	template<class T1, class T2>
	struct pair
	{
		typedef T1	first_type;
		typedef T2	second_type;

		first_type	first;
		second_type	second;

		pair() : first(), second() {}
		// Two objects may be passed to a pair constructor to be copied
		pair(const first_type &a, const second_type &b) : first(a), second(b) {}

		// There is also a templated constructor to convert from other pairs
		template<class U1, class U2>
		pair(const pair<U1, U2> &p) : first(p.first), second(p.second) {}

		pair	&operator=(const pair &obj) {
			first = obj.first;
			second = obj.second;
			return *this;
		}
	}; // pair

	/*************************************************************
	 * Boolean operators
	*************************************************************/
	// Two pairs of the same type are equal if their members are equal
    template <class T1, class T2>
    bool operator==(const pair<T1,T2> & l, const pair<T1,T2> & r)
    { return (l.first == r.first && l.second == r.second); }

    template <class T1, class T2>
    bool operator<(const pair<T1,T2> & l, const pair<T1,T2> & r)
    { return (l.first < r.first || (l.first == r.first && l.second < r.second)); }

    template <class T1, class T2>
    bool operator!=(const pair<T1,T2> & l, const pair<T1,T2> & r)
    { return (!(l == r)); }

    template <class T1, class T2>
    bool operator<=(const pair<T1,T2> & l, const pair<T1,T2> & r)
    { return (!(r < l)); }

    template <class T1, class T2>
    bool operator>(const pair<T1,T2> & l, const pair<T1,T2> & r)
    { return (r < l); }

    template <class T1, class T2>
    bool operator>=(const pair<T1,T2> & l, const pair<T1,T2> & r)
    { return (!(l < r)); }


	/*************************************************************
	 * MAKE_PAIR 
	 
	 * A convenience wrapper for creating a pair from two objects.
	*************************************************************/
	template <class T1, class T2>
	pair<T1, T2> make_pair(const T1 t, const T2 u) { return pair<T1, T2>(t, u); }

} // namespace

#endif /* _FT_UTILITY_HPP */
