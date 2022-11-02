#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

namespace ft {

template<class value_type>
void swap(value_type& a, value_type& b) { value_type c(a); a = b; b = c; }

}

#endif
