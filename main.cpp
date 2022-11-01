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
	int idx;
	char buff[BUFFER_SIZE];
};


//#define COUNT (MAX_RAM / (int)sizeof(Buffer))
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

int main(int argc, char** argv) {
	/*
	if (argc != 2)
	{
		std::cerr << "Usage: ./test seed" << std::endl;
		std::cerr << "Provide a seed please" << std::endl;
		std::cerr << "Count value:" << COUNT << std::endl;
		return 1;
	}
	const int seed = atoi(argv[1]);
	srand(seed);
*/
	ft::vector<std::string> vector_str;
	ft::vector<int> vector_int;
//	ft::stack<int> stack_int;
	ft::vector<Buffer> vector_buffer;
//	ft::stack<Buffer, std::deque<Buffer> > stack_deq_buffer;
//	ft::map<int, int> map_int;
	
	std::cout << "vector_str: push_back(hello 'i') x 4" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		std::stringstream	res;
		res << "hello " << i;
		vector_str.push_back(res.str());
	}

	for (int i = 0; i < 4; ++i)
		std::cout << vector_str[i] << std::endl;

//	std::cout << "count: " << COUNT << std::endl;
	std::cout << std::endl;
	std::cout << "vector_buffer: push_back(Buffer()) x 4" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		vector_buffer.push_back(Buffer());
	}

	for (int i = 0; i < 4; ++i)
		std::cout << vector_buffer[i].idx << std::endl;

	std::cout << std::endl;
	std::cout << "vector_buffer: assign value with =op x 4" << std::endl;
	for (int i = 0; i < COUNT; i++)
	{
		vector_buffer[i].idx = i;
	}
	for (int i = 0; i < 4; ++i)
		std::cout << vector_buffer[i].idx << std::endl;

	std::cout << std::endl;
	std::cout << "vector_buffer: assign value with =op x 4" << std::endl;
	for (int i = 0; i < COUNT; i++)
	{
		std::stringstream	res;
		res << "buffer " << i;
		const char *str = res.str().c_str();
		strcpy(vector_buffer[i].buff, str);
	}
	for (int i = 0; i < 4; ++i)
		std::cout << vector_buffer[i].buff << std::endl;

	std::cout << std::endl;
	std::cout << "vector_buffer: begin(), end()" << std::endl;
	Buffer	*begin = vector_buffer.begin();
	Buffer	*end = vector_buffer.end() - 1;
	std::cout << "begin: " << begin->idx << ", end: " << end->idx << std::endl;
	
/*
	std::cout << std::endl;
	std::cout << "vector_int: assign() '8' x 4" << std::endl;
	vector_int.assign(4, 8);
	for (int i = 0; i < 4; ++i)
		std::cout << vector_int[i] << std::endl;
*/





	/*
	for (int i = 0; i < COUNT; i++)
	{
		const int idx = rand() % COUNT;
		vector_buffer[idx].idx = 5;
	}

	ft::vector<Buffer>().swap(vector_buffer);

	try
	{
		for (int i = 0; i < COUNT; i++)
		{
			const int idx = rand() % COUNT;
			vector_buffer.at(idx);
			std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" <<std::endl;
		}
	}
	catch(const std::exception& e)
	{
		//NORMAL ! :P
	}

	for (int i = 0; i < COUNT; ++i)
	{
		map_int.insert(ft::make_pair(rand(), rand()));
	}

	int sum = 0;
	for (int i = 0; i < 10000; i++)
	{
		int access = rand();
		sum += map_int[access];
	}
	std::cout << "should be constant with the same seed: " << sum << std::endl;

	{
		ft::map<int, int> copy = map_int;
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
