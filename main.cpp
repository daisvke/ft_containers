#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <deque>


#if 0 //CREATE A REAL STL EXAMPLE
	#include <map>
	#include <stack>
	#include <vector>
	namespace ft = std;
#else
//	#include "map.hpp"
//	#include "stack.hpp"
	#include "vector.hpp"
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
#define COUNT 4

/*

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
*/

void print_testname(const char *name) {
	static int	i;
	++i;
	std::cout << std::endl;
	std::cout << "\033[4;33m" << i << "> " << name << "\033[0m" << std::endl;
}

int main(int argc, char** argv) {
	/*
	if (argc != 2)
	{
		std::cerr << "Usage: ./test seed" << std::endl;
		std::cerr << "Provide a seed please" << std::endl;
		std::cerr << "Count value:" << COUNT << std::endl;
		return 1;
	}
	const size_t seed = atoi(argv[1]);
	srand(seed);
*/
	ft::vector<std::string> vector_str;
	ft::vector<int> vector_int;
//	ft::stack<size_t> stack_size_t;
	ft::vector<Buffer> vector_buffer;
//	ft::stack<Buffer, std::deque<Buffer> > stack_deq_buffer;
//	ft::map<size_t, size_t> map_size_t;
	
	print_testname("vector_str: push_back(hello 'i') x 4");
	for (size_t i = 0; i < 3; i++)
	{
		std::stringstream	res;
		res << "hello " << i;
		vector_str.push_back(res.str());
	}

	std::cout << "size: " << vector_str.size() << ", cap: " << vector_str.capacity() << std::endl;
	for (size_t i = 0; i < vector_str.size(); ++i)
		std::cout << vector_str[i] << std::endl;

//	std::cout << "count: " << COUNT << std::endl;
	print_testname("vector_buffer: push_back(Buffer()) x 4");
	for (size_t i = 0; i < 4; i++)
	{
		vector_buffer.push_back(Buffer());
	}

	for (size_t i = 0; i < vector_str.size(); ++i)
		std::cout << vector_buffer[i].idx << std::endl;

	print_testname("vector_buffer: assign value with =op x 4");
	for (size_t i = 0; i < COUNT; i++)
	{
		vector_buffer[i].idx = i;
	}
	for (size_t i = 0; i < vector_str.size(); ++i)
		std::cout << vector_buffer[i].idx << std::endl;

	print_testname("vector_buffer: assign value with =op x 4");
	for (size_t i = 0; i < COUNT; i++)
	{
		std::stringstream	res;
		res << "buffer " << i;
		const char *str = res.str().c_str();
		strcpy(vector_buffer[i].buff, str);
	}
	for (size_t i = 0; i < vector_str.size(); ++i)
		std::cout << vector_buffer[i].buff << std::endl;

	print_testname("vector_buffer: begin(), end()");
	ft::vector<Buffer>::iterator	begin = vector_buffer.begin();
	ft::vector<Buffer>::iterator	end = vector_buffer.end() - 1;
	std::cout << "begin: " << begin->idx << ", end: " << end->idx << std::endl;
	
/*
	std::cout << std::endl;
	std::cout << "vector_int: assign() '8' x 4" << std::endl;
	vector_int.assign(4, 8);
	for (size_t i = 0; i < 4; ++i)
		std::cout << vector_int[i] << std::endl;
*/

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
	vect_to_swap.push_back("ZERO"); vect_to_swap.push_back("ONE");
	ft::swap(vect_to_swap, vector_str);
	for (size_t i = 0; i < vector_str.size(); ++i)
		std::cout << vector_str[i] << std::endl;



	/*
	for (size_t i = 0; i < COUNT; i++)
	{
		const size_t idx = rand() % COUNT;
		vector_buffer[idx].idx = 5;
	}

	ft::vector<Buffer>().swap(vector_buffer);

	try
	{
		for (size_t i = 0; i < COUNT; i++)
		{
			const size_t idx = rand() % COUNT;
			vector_buffer.at(idx);
			std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" <<std::endl;
		}
	}
	catch(const std::exception& e)
	{
		//NORMAL ! :P
	}

	for (size_t i = 0; i < COUNT; ++i)
	{
		map_size_t.insert(ft::make_pair(rand(), rand()));
	}

	size_t sum = 0;
	for (size_t i = 0; i < 10000; i++)
	{
		size_t access = rand();
		sum += map_size_t[access];
	}
	std::cout << "should be constant with the same seed: " << sum << std::endl;

	{
		ft::map<size_t, size_t> copy = map_size_t;
	}
	MutantStack<char> iterable_stack;
	for (char letter = 'a'; letter <= 'z'; letter++)
		iterable_stack.push(letter);
	for (MutantStack<char>::iterator it = iterable_stack.begin(); it != iterable_stack.end(); it++)
	{
		std::cout << *it;
	}
	*/
	std::cout << std::endl;
	return (0);
}
