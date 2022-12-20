#ifndef UTILITY_HPP
# define UTILITY_HPP

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
		pair(const first_type &a, const second_type &b) : first(a), second(b) {}
/*
		template<class U1, class U2>
		pair(const pair<U1, U2> &p) : first(p.first), second(p.second) {}
*/
		pair	&operator=(const pair &obj) {
			first = obj.first;
			second = obj.second;
			return *this;
		}
	};

	/*************************************************************
	 * MAKE_PAIR 
	*************************************************************/
	template <class T1, class T2>
	pair<T1, T2> make_pair(const T1 t, const T2 u) { return pair<T1, T2>(t, u); }

/*
	// swap
	template< class T1, class T2 >
	void swap(pair<T1,T2> &x, pair<T1,T2> &y) {
		pair<T1,T2>	tmp = x;
		x = y; y = tmp;
	}
*/

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
}

#endif
