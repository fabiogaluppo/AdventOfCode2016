#ifndef DAY_4_HPP
#define DAY_4_HPP

#include <regex>
#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

namespace Day4
{
	static void matches(const char* regex_pattern, std::string& s, std::vector<std::string>& result)
	{
		std::regex r(regex_pattern);
		auto _beg = std::sregex_iterator(s.begin(), s.end(), r);
		auto _end = std::sregex_iterator();
		for (std::sregex_iterator it = _beg; it != _end; ++it)
		{
			std::smatch m = *it;
			for (size_t i = 0; i < m.size(); ++i)
				result.push_back(m[i].str());
		}
	}

	static std::string decompose_str(std::string s, int& num, std::string& checksum)
	{
		//test pattern with https://regex101.com/
		const char* pattern = "(.+)\\-([\\d]+)\\[([a-z]{5})\\]";
		std::vector<std::string> results;
		matches(pattern, s, results);
		num = std::stoi(results[2]);
		checksum = results[3];
		return results[1];
	}

	static const int N = 'z' - 'a' + 1; //alphabet size

	void day_4_part_1()
	{
		std::string str;
		int total = 0;
		while (std::getline(std::cin, str))
		{
			int num;
			std::string checksum;
			std::string data = decompose_str(str, num, checksum);
			int count[N] = { 0 };
			for (char ch : data)
				if (ch != '-')
					++count[ch - 'a'];
			int checksum_count[5];
			int pos = 0;
			for (char ch : checksum)
				checksum_count[pos++] = count[ch - 'a'];
			std::sort(count, count + N);
			std::sort(checksum_count, checksum_count + 5);
			bool is_real =
				checksum_count[0] == count[N - 5] &&
				checksum_count[1] == count[N - 4] &&
				checksum_count[2] == count[N - 3] &&
				checksum_count[3] == count[N - 2] &&
				checksum_count[4] == count[N - 1];
			if (is_real)
				total += num;
		}
		std::cout << "What is the sum of the sector IDs of the real rooms? " << total << "\n";
		return;
	}

	static inline char shift_cipher(char ch, int num)
	{
		if (ch == '-')
			return ' ';
		return 'a' + (ch - 'a' + num) % N;
	}

	void day_4_part_2()
	{
		std::string str;
		while (std::getline(std::cin, str))
		{
			int num;
			std::string checksum;
			std::string data = decompose_str(str, num, checksum);
			//decode
			std::string decoded_str;
			for (char ch : str.substr(0, str.find_last_of('-')))
				decoded_str += shift_cipher(ch, num);
			if (decoded_str.find("northpole") != -1)
			{
				//std::cout << decoded_str << " " << num << "\n"; //DBG
				std::cout << "What is the sector ID of the room where North Pole objects are stored? " << num << "\n";
				return;
			}
			//dump
			/*for (char ch : str.substr(0, str.find_last_of('-')))
				std::cout << shift_cipher(ch, num);
			std::cout << " " << num << "\n";*/
		}
		return;
	}
}

int day_4_main(unsigned int part)
{
	if (part == 1)
		Day4::day_4_part_1(); //Your puzzle answer was 185371.
	else
		Day4::day_4_part_2(); //Your puzzle answer was 984.
	return 0;
}

#endif //DAY_4_HPP