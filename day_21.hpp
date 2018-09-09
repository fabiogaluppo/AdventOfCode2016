#ifndef DAY_21_HPP
#define DAY_21_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <vector>

namespace Day21
{
	static inline void swap_position(std::string& state, size_t x, size_t y)
	{
		std::swap(state[x], state[y]);
	}

	static inline void unswap_position(std::string& state, size_t x, size_t y)
	{
		swap_position(state, x, y);
	}

	static inline void swap_letter(std::string& state, char x, char y)
	{
		size_t i = state.find_first_of(x);
		size_t j = state.find_first_of(y);
		if (i != std::string::npos && j != std::string::npos)
			swap_position(state, i, j);
	}

	static inline void unswap_letter(std::string& state, char x, char y)
	{
		swap_letter(state, x, y);
	}

	static inline void rotate_left(std::string& state, size_t x)
	{
		std::rotate(state.begin(), state.begin() + (x % state.size()), state.end());
	}

	static inline void rotate_right(std::string& state, size_t x)
	{
		std::rotate(state.begin(), state.end() - (x % state.size()), state.end());
	}

	static inline void unrotate_left(std::string& state, size_t x)
	{
		rotate_right(state, x);
	}

	static inline void unrotate_right(std::string& state, size_t x)
	{
		rotate_left(state, x);
	}

	static inline void rotate_based_on_position_of(std::string& state, char x)
	{
		size_t i = state.find_first_of(x);
		if (i != std::string::npos)
			rotate_right(state, i < 4 ? i + 1 : i + 2);
	}

	static inline void unrotate_based_on_position_of(std::string& state, char x)
	{
		throw "rotate_based_on_position_of don't have inverse";
	}

	static inline void reverse_positions(std::string& state, size_t x, size_t y)
	{
		std::reverse(state.begin() + x, state.begin() + y + 1);
	}

	static inline void unreverse_positions(std::string& state, size_t x, size_t y)
	{
		reverse_positions(state, x, y);
	}

	static inline void move_position(std::string& state, size_t x, size_t y)
	{
		if (x > y)
			std::rotate(state.begin() + y, state.begin() + x, state.begin() + x + 1);
		else
			std::rotate(state.begin() + x, state.begin() + x + 1, state.begin() + y + 1);
	}

	static inline void unmove_position(std::string& state, size_t x, size_t y)
	{
		move_position(state, y, x);
	}

	static inline void execute_command(const std::string& input, std::string& state)
	{
		std::string input_copy = input;
		char* cmd = const_cast<char*>(input_copy.c_str());
		const char* pattern = " ";
		char* p = std::strtok(cmd, pattern);
		if (p != nullptr)
		{
			if (std::strcmp(p, "swap") == 0)
			{
				p = std::strtok(nullptr, pattern);
				if (std::strcmp(p, "position") == 0)
				{
					p = std::strtok(nullptr, pattern);
					size_t x = std::atoi(p);
					p = std::strtok(nullptr, pattern); //"with"
					p = std::strtok(nullptr, pattern); //"position"
					p = std::strtok(nullptr, pattern);
					size_t y = std::atoi(p);
					swap_position(state, x, y);
				}
				else if (std::strcmp(p, "letter") == 0)
				{
					p = std::strtok(nullptr, pattern);
					char x = *p;
					p = std::strtok(nullptr, pattern); //"with"
					p = std::strtok(nullptr, pattern); //"letter"
					p = std::strtok(nullptr, pattern);
					char y = *p;
					swap_letter(state, x, y);
				}
				//else ignore command
			}
			else if (std::strcmp(p, "rotate") == 0)
			{
				p = std::strtok(nullptr, pattern);
				if (std::strcmp(p, "left") == 0)
				{
					p = std::strtok(nullptr, pattern);
					size_t x = std::atoi(p);
					//skip " steps"
					rotate_left(state, x);
				}
				else if (std::strcmp(p, "right") == 0)
				{
					p = std::strtok(nullptr, pattern);
					size_t x = std::atoi(p);
					//skip " steps"
					rotate_right(state, x);
				}
				else if (std::strcmp(p, "based") == 0)
				{
					p = std::strtok(nullptr, pattern); //"on"
					p = std::strtok(nullptr, pattern); //"position"
					p = std::strtok(nullptr, pattern); //"of"
					p = std::strtok(nullptr, pattern); //"letter"
					p = std::strtok(nullptr, pattern);
					char x = *p;
					rotate_based_on_position_of(state, x);
				}
				//else ignore command
			}
			else if (std::strcmp(p, "reverse") == 0)
			{
				p = std::strtok(nullptr, pattern); //"positions"
				p = std::strtok(nullptr, pattern);
				size_t x = std::atoi(p);
				p = std::strtok(nullptr, pattern); //"through"
				p = std::strtok(nullptr, pattern);
				size_t y = std::atoi(p);
				reverse_positions(state, x, y);
			}
			else if (std::strcmp(p, "move") == 0)
			{
				p = std::strtok(nullptr, pattern); //"position"
				p = std::strtok(nullptr, pattern);
				size_t x = std::atoi(p);
				p = std::strtok(nullptr, pattern); //"to"
				p = std::strtok(nullptr, pattern); //"position"
				p = std::strtok(nullptr, pattern);
				size_t y = std::atoi(p);
				move_position(state, x, y);
			}
			//else ignore command
		}
	}

	static void test_1()
	{
		//scramble
		std::string state = "abcde";
		std::cout << state << "\n";
		execute_command("swap position 4 with position 0", state);
		std::cout << state << "\n";
		execute_command("swap letter d with letter b", state);
		std::cout << state << "\n";
		execute_command("reverse positions 0 through 4", state);
		std::cout << state << "\n";
		execute_command("rotate left 1 step", state);
		std::cout << state << "\n";
		execute_command("move position 1 to position 4", state);
		std::cout << state << "\n";
		execute_command("move position 3 to position 0", state);
		std::cout << state << "\n";
		execute_command("rotate based on position of letter b", state);
		std::cout << state << "\n";
		execute_command("rotate based on position of letter d", state);
		std::cout << state << "\n";
	}

	void day_21_part_1()
	{
		//test_1();

		std::string state = "abcdefgh";
		std::string input;
		while (std::getline(std::cin, input))
		{
			execute_command(input, state);
		}
		std::cout << "What is the result of scrambling abcdefgh? " << state << "\n";
	}

	void day_21_part_2()
	{
		//unscramble of functions seems impossible due to 'rotate_based_on_position_of'
		//so brute force

		std::string input;
		std::vector<std::string> commands;
		while (std::getline(std::cin, input))
		{
			commands.push_back(input);
		}

		std::string state = "fbgdceah";
		std::string expected = state;
		std::sort(state.begin(), state.end());
		do
		{
			std::string result = state;
			for (auto& command : commands)
				execute_command(command, result);
			if (result == expected)
				break;
		} while (std::next_permutation(state.begin(), state.end()));

		std::cout << "What is the un-scrambled version of the scrambled password fbgdceah? " << state << "\n";
	}
}

int day_21_main(unsigned int part)
{
	if (part == 1)
		Day21::day_21_part_1(); //Your puzzle answer was bfheacgd.
	else
		Day21::day_21_part_2(); //Your puzzle answer was gcehdbfa.
	return 0;
}

#endif /* DAY_21_HPP */