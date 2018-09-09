#ifndef DAY_18_HPP
#define DAY_18_HPP

#include <string>
#include <iostream>
#include <algorithm>

namespace Day18
{
	const char TILE_TRAP = '^';
	const char TILE_SAFE = '.';

	static inline bool is_trap(char left, char center, char right)
	{
		return left != right;
	}

	static inline char select_tile(char left, char center, char right)
	{
		return is_trap(left, center, right) ? TILE_TRAP : TILE_SAFE;
	}

	static inline size_t tile_safe_count(const std::string& input)
	{
		return std::count(input.begin(), input.end(), TILE_SAFE);
	}

	void day_18_part_1()
	{
		std::string input;
		std::cin >> input;
		size_t N;
		std::cin >> N;

		//std::cout << input << "\n"; //DBG
		size_t count_safe = tile_safe_count(input);
		size_t n = input.size();
		std::string temp(n, ' ');
		for (size_t i = 1; i < N; ++i)
		{
			temp[0] = select_tile(TILE_SAFE, input[0], input[1]);
			for (size_t j = 1, l = n - 1; j < l; ++j)
				temp[j] = select_tile(input[j - 1], input[j], input[j + 1]);
			temp[n - 1] = select_tile(input[n - 2], input[n - 1], TILE_SAFE);
			input.swap(temp);
			//std::cout << input << "\n"; //DBG
			count_safe += tile_safe_count(input);
		}

		std::cout << "How many safe tiles? " << count_safe << "\n";

		//part 1:
		//^.....^.^^^^^.^..^^.^.......^^..^^^..^^^^..^.^^.^.^....^^...^^.^^.^...^^.^^^^..^^.....^.^...^.^.^^.^
		//40
		//how many safe tiles ? 1974
	}

	void day_18_part_2()
	{
		day_18_part_1();

		//part 2:
		//^.....^.^^^^^.^..^^.^.......^^..^^^..^^^^..^.^^.^.^....^^...^^.^^.^...^^.^^^^..^^.....^.^...^.^.^^.^
		//400000
		//how many safe tiles ? 19991126
	}
};

int day_18_main(unsigned int part)
{
	if (part == 1)
		Day18::day_18_part_1(); //Your puzzle answer was 1974.
	else
		Day18::day_18_part_2(); //Your puzzle answer was 19991126.
	return 0;
}

#endif /* DAY_18_HPP */