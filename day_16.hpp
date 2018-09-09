#ifndef DAY_16_HPP
#define DAY_16_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <iostream>

namespace Day16
{
	static inline std::vector<bool> to_bitseq(const std::string& s)
	{
		std::vector<bool> temp;
		for (char ch : s)
			temp.push_back(ch == '1');
		return temp;
	}

	static inline std::string to_string(const std::vector<bool>& xs)
	{
		std::string temp;
		for (bool x : xs)
			temp += x ? '1' : '0';
		return temp;
	}

	static inline void morph_bitseq(std::vector<bool>& xs)
	{
		if (xs.size() > 0)
		{
			std::vector<bool> xs_ = xs;
			std::reverse(xs_.begin(), xs_.end());
			xs.push_back(false);
			//for (bool x : xs_)
			//	xs.push_back(!x);
			////or
			std::transform(xs_.begin(), xs_.end(), std::back_inserter(xs), [](bool x) { return !x; });
		}
	}

	static inline std::vector<bool> fill_disk(const std::string& init, size_t length)
	{
		std::vector<bool> xs = to_bitseq(init);
		while (xs.size() < length)
			morph_bitseq(xs);
		xs.resize(length);
		return xs;
	}

	static inline std::vector<bool> compute_checksum(const std::vector<bool>& xs)
	{
		std::vector<bool> result = xs;
		do
		{
			std::vector<bool> inspected = result;
			result.clear();
			for (size_t i = 0; i < inspected.size(); i += 2)
				result.push_back(inspected[i] == inspected[i + 1]);
		} while (!(result.size() & 0x1));
		return result;
	}

	void day_16_part_1()
	{
		std::string init;
		size_t length;
		std::cin >> init >> length;
		std::vector<bool> checksum = compute_checksum(fill_disk(init, length));
		std::cout << "What is the correct checksum? " << to_string(checksum) << "\n";

		//test1:
		//10000 20
		//what is the correct checksum? 01100
		//test2:
		//110010110100 12
		//what is the correct checksum ? 100
		//part1:
		//10001110011110000 272
		//what is the correct checksum? 10010101010011101
	}

	void day_16_part_2()
	{
		day_16_part_1();

		//part2:
		//10001110011110000 35651584
		//what is the correct checksum? 01100111101101111
	}
};

int day_16_main(unsigned int part)
{
	if (part == 1)
		Day16::day_16_part_1(); //Your puzzle answer was 10010101010011101.
	else
		Day16::day_16_part_2(); //Your puzzle answer was 01100111101101111.
	return 0;
}

#endif /* DAY_16_HPP */