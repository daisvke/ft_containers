#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <deque>


#if 0 //CREATE A REAL STL EXAMPLE
	#define _NAMESPACE	"STL"
	#include <map>
	#include <stack>
	#include <vector>
	namespace ft = std;
#else
	#define _NAMESPACE	"FT"
	#include "stack.hpp"
	#include "vector.hpp"
	#include "map.hpp"
	#include "set.hpp"
#endif

#include <stdlib.h>

#define MAX_RAM 4294967296
#define BUFFER_SIZE 4096
struct Buffer
{
	size_t idx;
	char buff[BUFFER_SIZE];
};


//#define COUNT (MAX_RAM / (size_t)sizeof(Buffer))
#define COUNT 5



template<typename T>
class MutantStack : public ft::stack<T>
{
public:
	MutantStack() {}
	MutantStack(const MutantStack<T>& src) { *this = src; }
	MutantStack<T>& operator=(const MutantStack<T>& rhs) 
	{
		this->c = rhs.c;
		return *this;
	}
	~MutantStack() {}

	typedef typename ft::stack<T>::container_type::iterator iterator;

	iterator begin() { return this->c.begin(); }
	iterator end() { return this->c.end(); }
};


void print_testname(const char *name) {
	static int	i;
	++i;
	std::cout << std::endl;
	std::cout << "\033[4;33m" << i << "> " << name << "\033[0m" << std::endl;
}

int main(int argc, char** argv) {
	
	if (argc != 2)
	{
		std::cerr << "Usage: ./test seed" << std::endl;
		std::cerr << "Provide a seed please" << std::endl;
		std::cerr << "Count value:" << COUNT << std::endl;
		return 1;
	}
	const size_t seed = atoi(argv[1]);
	srand(seed);


	// Check which namespace will be used
	std::cout << std::endl <<
		">> \033[31mUSING NAMESPACE " << _NAMESPACE << "\033[0m <<"
		<< std::endl;

	ft::vector<std::string> vector_str;
	ft::vector<int> vector_int;
	ft::stack<size_t> stack_size_t;
	ft::vector<Buffer> vector_buffer;
	ft::stack<Buffer, std::deque<Buffer> > stack_deq_buffer;
	ft::map<int, int> map_int;
	
	std::cout << "count: " << COUNT << std::endl;

	print_testname("vector_buffer: push_back(Buffer()) x COUNT");
	for (size_t i = 0; i < COUNT; i++)
		vector_buffer.push_back(Buffer());

	for (size_t i = 0; i < vector_buffer.size(); ++i)
		std::cout << vector_buffer[i].idx << std::endl;

	print_testname("vector_buffer: assign value with =op x ");
	for (size_t i = 0; i < COUNT; i++)
	{
		const int idx = rand() % COUNT;
		vector_buffer[idx].idx = 5;
	}
	for (size_t i = 0; i < vector_buffer.size(); ++i)
		std::cout << vector_buffer[i].idx << std::endl;

	// print_testname("vector_buffer: swap with empty vector<Buffer>");
	// ft::vector<Buffer>().swap(vector_buffer);

	// try
	// {
	// 	for (size_t i = 0; i < COUNT; i++)
	// 	{
	// 		const size_t idx = rand() % COUNT;
	// 		vector_buffer.at(idx);
	// 		std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" <<std::endl;
	// 	}
	// }
	// catch(const std::exception& e)
	// {
	// 	//NORMAL ! :P
	// }

	std::cout << std::endl;

	print_testname("vector_buffer: assign value with =op x COUNT");
	for (size_t i = 0; i < 10 && i < COUNT; i++)
	{
		vector_buffer[i].buff[0] = 'B';
		vector_buffer[i].buff[1] = i + 48;
	}
	for (size_t i = 0; i < vector_buffer.size(); ++i)
		std::cout << vector_buffer[i].buff << std::endl;

	print_testname("vector_buffer: begin(), end()");
	ft::vector<Buffer>::iterator	begin = vector_buffer.begin();
	ft::vector<Buffer>::iterator	end = --vector_buffer.end();
	std::cout << "begin: " << begin->idx << ", end: " << end->idx << std::endl;

	print_testname("vector_buffer: rbegin(), rend()");
	ft::vector<Buffer>::reverse_iterator	rbegin = vector_buffer.rbegin();
	ft::vector<Buffer>::reverse_iterator	rend = --vector_buffer.rend();
	std::cout << "rbegin: " << rbegin->idx << ", rend: " << rend->idx << std::endl;

	std::cout << std::endl;
	print_testname("vector_int: assign() '8' x 4");
	vector_int.assign(4, 8);
	for (size_t i = 0; i < 4; ++i)
		std::cout << vector_int[i] << std::endl;

	print_testname("vector_str: push_back(hello 'i') x 3");
	for (size_t i = 0; i < 3; i++)
	{
		std::stringstream	res;
		res << "hello " << i;
		vector_str.push_back(res.str());
	}

	std::cout << "size: " << vector_str.size() << ", cap: " << vector_str.capacity() << std::endl;
	for (size_t i = 0; i < vector_str.size(); ++i)
		std::cout << vector_str[i] << std::endl;

	print_testname( "vector_str: insert 'HERE' at position 2");
	ft::vector<std::string>::iterator it_str = vector_str.begin() + 2;
	vector_str.insert(it_str, 1, "HERE"); 
	for (size_t i = 0; i < vector_str.size(); ++i)
		std::cout << vector_str[i] << std::endl;
	std::cout << "size: " << vector_str.size() << ", cap: " << vector_str.capacity() << std::endl;

	print_testname("vector_str: erase at position 2");
	it_str = vector_str.begin() + 2; it_str = vector_str.erase(it_str);
	std::cout << "returned: " << *it_str << std::endl;
	for (size_t i = 0; i < vector_str.size(); ++i)
		std::cout << vector_str[i] << std::endl;

	print_testname("vector_str: erase between pos [1-3)");
	ft::vector<std::string>::iterator	begin_str = vector_str.begin() + 1;
	ft::vector<std::string>::iterator	end_str = vector_str.begin() + 3;
	vector_str.erase(begin_str, end_str);
	for (size_t i = 0; i < vector_str.size(); ++i)
		std::cout << vector_str[i] << std::endl;
	
	print_testname("ft::swap() two vectors");
	ft::vector<std::string>	vect_to_swap;
	const std::string	s1 = "ZERO";
	vect_to_swap.push_back(s1); vect_to_swap.push_back("ONE");
	ft::swap(vect_to_swap, vector_str);
	for (size_t i = 0; i < vector_str.size(); ++i)
		std::cout << vector_str[i] << std::endl;

	print_testname( "vector_int:	v1: {1, 2, 3}	v2 = v1. Comparing the two using '=='");

	ft::vector<int>	v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	ft::vector<int>	v2 = v1;
	if (v1 == v2) {
		std::cout << "v1 equals v2" << std::endl;

		print_testname( "vector_int:	v1: {2, 2, 3}	v2 = {1, 2, 3}. Comparing the two using '>'");
		v1[0] = 2;
		if (v1 > v2) {
			std::cout << "v1 is bigger than v2" << std::endl;

			print_testname( "vector_int:	v1: {-1, 2, 3}	v2 = {1, 2, 3}. Comparing the two using '<'");
			v1[0] = -1;
			if (v1 < v2)
				std::cout << "v1 is smaller than v2" << std::endl;
		}
	}

	print_testname( "map_int:	insert ft::make_pair(i,i)");

	for (size_t i = 0; i < COUNT; ++i)
	{
		map_int.insert(ft::make_pair(i, i));
	}
	for (ft::map<int, int>::iterator it = map_int.begin(); it != map_int.end(); ++it)
		std::cout << "key: " << (*it).first << " => val: " << (*it).second << std::endl;


	print_testname( "map_int:	insert {0, 42}, {1, 42}, {2, 4242}");

	map_int[0] = 42;
	map_int[1] = 42;
	map_int[2] = 4242;

	for (ft::map<int, int>::iterator it = map_int.begin(); it != map_int.end(); ++it)
		std::cout << "key: " << (*it).first << " => val: " << (*it).second << std::endl;

	print_testname( "map_int:	erase key=1");
	map_int.erase(1);
	for (ft::map<int, int>::iterator it = map_int.begin(); it != map_int.end(); ++it)
		std::cout << "key: " << (*it).first << " => val: " << (*it).second << std::endl;


	print_testname( "map_int:	at key=2 | at key=3");
	std::cout << map_int.at(2) << " | " << map_int.at(3) << std::endl;

	ft::map<int, int>	map_int2;

	print_testname( "map_int2:	insert ft::make_pair(rand(),rand())");
	for (int i = 0; i < COUNT; ++i)
	{
		map_int2.insert(ft::make_pair(rand(), rand()));
	}
	for (ft::map<int, int>::iterator it = map_int2.begin(); it != map_int2.end(); ++it)
		std::cout << "key: " << (*it).first << " => val: " << (*it).second << std::endl;
 
 	ft::map<int, int>	map_int4;

	size_t sum = 0;
	for (size_t i = 0; i < 10; i++)
	{
		size_t access = rand();
		sum += map_int4[access];
	}
	std::cout << "should be constant with the same seed: " << sum << std::endl;

	ft::map<int, int> copy = map_int4;
	for (ft::map<int, int>::iterator it = copy.begin(); it != copy.end(); ++it)
		std::cout << "key: " << (*it).first << " => val: " << (*it).second << std::endl;

	print_testname("map_int2:	begin(), end()");
	
	std::cout << "[begin] key: " << (*map_int2.begin()).first
		<< " val: " << (*map_int2.begin()).second <<
		"	[end] key:" << (*--map_int2.end()).first <<
		" val: " << (*--map_int2.end()).second << std::endl;

	print_testname("map_int2:	rbegin(), rend()");

	std::cout << "[rbegin] key: " << (map_int2.rbegin())->first
		<< " val: " << (map_int2.rbegin())->second <<
		"	[rend] key:" << (--map_int2.rend())->first <<
		" val: " << (--map_int2.rend())->second << std::endl;

	print_testname("map_int5:	insert map_int.begin(), map_int.end()");

	ft::map<int, int>	map_int5(map_int.begin(), map_int.end());
	std::cout << map_int5[0] << std::endl;

	print_testname("map_int5:	assign map_int");

	map_int5 = map_int;
	for (ft::map<int, int>::iterator it = map_int5.begin(); it != map_int5.end(); ++it)
		std::cout << "key: " << (*it).first << " => val: " << (*it).second << std::endl;

	print_testname("map_int5:	swap map_int");
	map_int5.swap(map_int);
	for (ft::map<int, int>::iterator it = map_int5.begin(); it != map_int5.end(); ++it)
		std::cout << "key: " << (*it).first << " => val: " << (*it).second << std::endl;

	print_testname("vct:	assign operator");

	ft::vector<int>	vct;
	vct.push_back(1);
	vct.push_back(2);
	vct.push_back(3);
	vct.pop_back();
	vct.pop_back();
	vct.pop_back();
	for (unsigned long int i = 0; i < vct.size(); ++i)
		vct[i] = (vct.size() - i) * 3;

	for (size_t i = 0; i < vct.size(); ++i)
		std::cout << vct[i] << std::endl;
		
	print_testname("vct:	assign 53 x 5");

	vct.assign(7, 53);

	for (size_t i = 0; i < vct.size(); ++i)
		std::cout << vct[i] << std::endl;

	print_testname("vct:	assign vector_int.begin(), vector_int.end()");

	vct.assign(vector_int.begin(), vector_int.begin() + 3);

	for (size_t i = 0; i < vct.size(); ++i)
		std::cout << vct[i] << std::endl;

	print_testname("vct:	at()");

	ft::vector<int> myvector (10);   // 10 zero-initialized ints

	// assign some values:
	for (unsigned i=0; i<myvector.size(); i++)
		myvector.at(i)=i;

	std::cout << "myvector contains:";
	for (unsigned i=0; i<myvector.size(); i++)
		std::cout << ' ' << myvector.at(i);
	std::cout << std::endl;	

	print_testname("vct:	nbr + iterator");
 	ft::vector<int>::iterator	it2 = 2 + myvector.begin();
	std::cout << (*it2) << std::endl;

	print_testname("vct:	const_iterator - iterator");
	ft::vector<int>::const_reverse_iterator	const_it = vector_int.rbegin();
 	std::cout << const_it - (vector_int.rend() + 3) << std::endl;

	print_testname("vct:	const_reverse_iterator - reverse_iterator");


	#define TESTED_NAMESPACE ft
	#define TESTED_TYPE int
	const int size = 5;
	TESTED_NAMESPACE::vector<TESTED_TYPE> vct2(size);
	TESTED_NAMESPACE::vector<TESTED_TYPE>::reverse_iterator it = vct2.rbegin();
	TESTED_NAMESPACE::vector<TESTED_TYPE>::const_reverse_iterator ite = vct2.rbegin();

	for (int i = 0; i < size; ++i)
		it[i] = (size - i) * 5;

	it = it + 5;
	it = 1 + it;
	it = it - 4;
	 std::cout << *(it += 2) << std::endl;
	std::cout << *(it -= 1) << std::endl;

	*(it -= 2) = 42;
	*(it += 2) = 21;

	std::cout << "const_ite +=/-=: " << *(ite += 2) << " | " << *(ite -= 2) << std::endl;

	std::cout << "(it == const_it): " << (ite == it) << std::endl;
	std::cout << "(const_ite - it): " << (ite - it) << std::endl;
	std::cout << "(ite + 3 == it): " << (ite + 3 == it) << std::endl << std::endl;


	//std::cout << "begin: " << map_int3.begin() << "	end: " << map_int3.end() << std::endl;

	MutantStack<char> iterable_stack;
	for (char letter = 'a'; letter <= 'z'; letter++)
		iterable_stack.push(letter);
	for (MutantStack<char>::iterator it = iterable_stack.begin(); it != iterable_stack.end(); it++)
	{
		std::cout << *it;
	}

	int myints[]={12,75,10,32,20,25};
	ft::set<int> first (myints,myints+3);     // 10,12,75
	ft::set<int> second (myints+3,myints+6);  // 20,25,32

	first.swap(second);

	std::cout << "first contains:";
	for (ft::set<int>::iterator it4=first.begin(); it4!=first.end(); ++it4)
	std::cout << ' ' << *it4;
	std::cout << '\n';

	std::cout << "second contains:";
	for (ft::set<int>::iterator it4=second.begin(); it4!=second.end(); ++it4)
	std::cout << ' ' << *it4;
	std::cout << '\n';


	std::cout << std::endl;

	return (0);
}