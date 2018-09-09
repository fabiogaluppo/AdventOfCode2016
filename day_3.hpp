#ifndef DAY_3_HPP
#define DAY_3_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

namespace Day3
{
	static inline bool is_triangle_inequality(int x, int y, int z)
	{
		return y + z > x && x + z > y && x + y > z;
	}
	
	void day_3_part_1()
	{
		std::string str;
		const char* pattern = " ";
		int possible = 0;
		while (std::getline(std::cin, str))
		{
			int values[3];
			int i = 0;
			char* p = std::strtok(const_cast<char*>(str.c_str()), pattern);
			while (p != nullptr)
			{
				values[i++] = std::atoi(p);
				p = std::strtok(nullptr, pattern);
			}
			if (is_triangle_inequality(values[0], values[1], values[2]))
				++possible;
		}
		std::cout << "How many of the listed triangles are possible? " << possible << "\n";
		return;
	}

	void day_3_part_2()
	{
		std::string str;
		const char* pattern = " ";
		int possible = 0;
		const int Y = 3;
		const int X = 3;
		int table[Y][X];
		int j = 0;
		while (std::getline(std::cin, str))
		{
			//inverted read
			int i = 0;
			char* p = std::strtok(const_cast<char*>(str.c_str()), pattern);
			while (p != nullptr)
			{
				table[i++][j] = std::atoi(p);
				p = std::strtok(nullptr, pattern);
			}
			//compute
			if (++j == Y)
			{
				for (int y = 0; y < Y; ++y)
					if (is_triangle_inequality(table[y][0], table[y][1], table[y][2])) 
						++possible;
				j = 0;
			}
		}
		std::cout << "How many of the listed triangles are possible? " << possible << "\n";
		return;
	}
};

int day_3_main(unsigned int part)
{
	if (part == 1)
		Day3::day_3_part_1(); //Your puzzle answer was 982.
	else
		Day3::day_3_part_2(); //Your puzzle answer was 1826.
	return 0;
}

#endif //DAY_3_HPP