#ifndef DAY_9_HPP
#define DAY_9_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

namespace Day9
{
	void day_9_part_1()
	{
		std::string input;
		while (std::getline(std::cin, input)) 
		{
			char* p = const_cast<char*>(input.c_str());
			unsigned long long counter = 0;
			while (*p != '\0')
			{
				if (*p == '(')
				{
					++p;
					p = std::strtok(p, "x");
					size_t skip = std::atoi(p);					
					p = std::strtok(nullptr, ")");
					size_t len = std::strlen(p) + 1;
					size_t times = std::atoi(p);
					counter += (skip * times);
					p += (skip + len);
				}
				else
				{
					++p;
					++counter;
				}
			}
			std::cout << "What is the decompressed length? " << counter << "\n";
		}
		return;
	}

	static unsigned long long rec_count_dbg(char* first, char* last, size_t depth)
	{
		unsigned long long counter = 0;
		while (first != last)
		{
			if (*first == '(')
			{
				++first;
				first = std::strtok(first, "x");
				size_t skip = std::atoi(first);
				first = std::strtok(nullptr, ")");
				size_t len = std::strlen(first) + 1;
				size_t times = std::atoi(first);
				first += len;
				for (size_t i = 0; i < depth; ++i) std::cout << "  ";
				std::cout << "(" << skip << "x" << times << ")" << "\n";
				counter += (times * rec_count_dbg(first, first + skip, depth + 1));
				first += skip;
			}
			else
			{
				std::cout << *first;
				++first;
				++counter;
			}
		}
		for (size_t i = 0; i < depth; ++i) std::cout << "  ";
		std::cout << counter << "\n";
		return counter;
	}

	static unsigned long long rec_count(char* first, char* last)
	{
		unsigned long long counter = 0;
		while (first != last)
		{
			if (*first == '(')
			{
				++first;
				first = std::strtok(first, "x");
				size_t skip = std::atoi(first);
				first = std::strtok(nullptr, ")");
				size_t len = std::strlen(first) + 1;
				size_t times = std::atoi(first);
				first += len;
				counter += (times * rec_count(first, first + skip));
				first += skip;
			}
			else
			{
				++first;
				++counter;
			}
		}
		return counter;
	}

	void day_9_part_2()
	{
		std::string input;
		while (std::getline(std::cin, input))
		{
			char* p = const_cast<char*>(input.c_str());
			//unsigned long long counter = rec_count_dbg(p, p + input.size(), 0);
			unsigned long long counter = rec_count(p, p + input.size());
			std::cout << "What is the decompressed length? " << counter << "\n";
		}
		return;
	}
};

int day_9_main(unsigned int part)
{
	if (part == 1)
		Day9::day_9_part_1(); //Your puzzle answer was 98135.
	else
		Day9::day_9_part_2(); //Your puzzle answer was 10964557606.
	return 0;
}

#endif //DAY_9_HPP