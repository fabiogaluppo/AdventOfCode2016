#ifndef DAY_7_HPP
#define DAY_7_HPP

#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>

namespace Day7
{
	void day_7_part_1()
	{
		size_t count = 0;
		std::string input;
		while (std::getline(std::cin, input))
		{
			//std::cout << input << "\n";
			size_t j = 0;
			bool ignore = false;
			bool compute = false;
			for (size_t i = 0; i < input.size() - 3; i += j)
			{
				char a = input[i + 0], b = input[i + 1],
					 c = input[i + 2], d = input[i + 3];

				bool opened = a == '[' || b == '[' || c == '[' || d == '[';
				bool closed = a == ']' || b == ']' || c == ']' || d == ']';

				ignore = ignore ||  opened;
				ignore = ignore && !closed;

				if (opened || closed)
				{
					if      (a == '[' || a == ']') j = 1;
					else if (b == '[' || b == ']') j = 2;
					else if (c == '[' || c == ']') j = 3;
					//else if (d == '[' || d == ']') j = 4;
					else                           j = 4;
				} 
				else if (a == d && b == c && a != b)
				{
					if (ignore)
					{
						break;
					}
					else
					{
						compute = true;
						j = 4;
					}
				}
				else
				{
					if (b == c)
					{
						j = c == d ? 3 : 2;
					}
					else
					{
						j = 1;
					}
				}
			}
			if (compute && !ignore)
			{
				//std::cout << input << "\n"; //valid input
				++count;
			}
		}
		std::cout << "How many IPs? " << count << "\n";

		return;
	}

	void day_7_part_2()
	{
		size_t count = 0;
		std::string input;
		while (std::getline(std::cin, input))
		{
			//std::cout << input << "\n";
			std::vector<std::string> inside;			
			std::vector<std::string> outside;
			size_t j = 0;
			bool is_inside = false;			
			for (size_t i = 0; i < input.size() - 2; i += j)
			{
				char a = input[i + 0], b = input[i + 1], c = input[i + 2];

				bool opened = a == '[' || b == '[' || c == '[';
				bool closed = a == ']' || b == ']' || c == ']';

				is_inside = is_inside || opened;
				is_inside = is_inside && !closed;

				if (opened || closed)
				{
					if      (a == '[' || a == ']') j = 1;
					else if (b == '[' || b == ']') j = 2;
					//else if (c == '[' || c == ']') j = 3;
					else j = 3;
					continue;
				}
				else if (a != b && a == c)
				{
					if (is_inside)
					{
						//storing aba as bab inside
						std::string s(3, b);
						s[1] = a;
						//s[2] = b;
						inside.push_back(s);
					}
					else
					{
						//storing aba outside
						std::string s(3, a);
						s[1] = b;
						s[2] = c;
						outside.push_back(s);
					}
				}
				j = b == c ? 2 : 1;
			}

			if (outside.size() > 0 && inside.size() > 0)
			{
				//1 + 2 = O(2(N lg N))
				//1: 1.1 = O(N lg N)
				std::sort(outside.begin(), outside.end()); //1.1: O(N lg N)
				//2: 2.1 * 2.2 = O(N lg N)
				for (auto& s : inside) //2.1: O(N)
				{
					if (std::binary_search(outside.begin(), outside.end(), s)) //2.2: O(lg N)
					{
						//std::cout << input << "\n"; //valid input
						++count;
						break;
					}
				}
			}
		}
		std::cout << "How many IPs? " << count << "\n";

		return;
	}
};

int day_7_main(unsigned int part)
{
	if (part == 1)
		Day7::day_7_part_1(); //Your puzzle answer was 110.
	else
		Day7::day_7_part_2(); //Your puzzle answer was 242.
	return 0;
}

#endif //DAY_7_HPP