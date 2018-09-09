#ifndef DAY_6_HPP
#define DAY_6_HPP

#include <string>
#include <iostream>
#include <vector>
#include <array>

namespace Day6
{
	constexpr size_t ALPHABET_SIZE = 26;
	using counting_table_type = std::vector<std::array<size_t, ALPHABET_SIZE>>;

	static inline void count(const std::string& input, counting_table_type& counting_table)
	{
		for (size_t i = 0; i < input.size(); ++i)
			++counting_table[i][input[i] - 'a'];
	}

	static void init_table(counting_table_type& counting_table)
	{
		size_t N = counting_table.size();
		for (size_t i = 0; i < N; ++i)
			for (size_t j = 0; j < ALPHABET_SIZE; ++j)
				counting_table[i][j] = 0;
	}

	static std::string get_most_common_char_by_column(const counting_table_type& counting_table)
	{
		size_t N = counting_table.size();
		std::string msg(N, ' ');
		for (size_t i = 0; i < N; ++i)
		{
			size_t max_index = 0;
			size_t max = counting_table[i][0];
			for (size_t j = 1; j < ALPHABET_SIZE; ++j)
			{
				if (max < counting_table[i][j])
				{
					max_index = j;
					max = counting_table[i][j];
				}
			}
			msg[i] = 'a' + max_index;
		}
		return msg;
	}

	static std::string get_least_common_char_by_column(const counting_table_type& counting_table)
	{
		size_t N = counting_table.size();
		std::string msg(N, ' ');
		for (size_t i = 0; i < N; ++i)
		{
			size_t min_index = 0;
			size_t min = counting_table[i][0];
			for (size_t j = 1; j < ALPHABET_SIZE; ++j)
			{
				if (counting_table[i][j] < min)
				{
					min_index = j;
					min = counting_table[i][j];
				}
			}
			msg[i] = 'a' + min_index;
		}
		return msg;
	}

	void day_6_part_1()
	{
		counting_table_type counting_table;

		std::string input;
		if (std::getline(std::cin, input))
		{
			size_t N = input.size(); //number of columns

			counting_table.resize(N);
			init_table(counting_table);

			count(input, counting_table);
			while (std::getline(std::cin, input))
				count(input, counting_table);

			std::string msg = get_most_common_char_by_column(counting_table);
			std::cout << "What is the error-corrected version of the message being sent? " << msg << "\n";
		}

		return;
	}

	void day_6_part_2()
	{
		counting_table_type counting_table;

		std::string input;
		if (std::getline(std::cin, input))
		{
			size_t N = input.size(); //number of columns

			counting_table.resize(N);
			init_table(counting_table);

			count(input, counting_table);
			while (std::getline(std::cin, input))
				count(input, counting_table);

			std::string msg = get_least_common_char_by_column(counting_table);
			std::cout << "What is the original message that Santa is trying to send? " << msg << "\n";
		}

		return;
	}
}

int day_6_main(unsigned int part)
{
	if (part == 1)
		Day6::day_6_part_1(); //Your puzzle answer was qqqluigu.
	else
		Day6::day_6_part_2(); //Your puzzle answer was lsoypmia.
	return 0;
}

#endif //DAY_6_HPP