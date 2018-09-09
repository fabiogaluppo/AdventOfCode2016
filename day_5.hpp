#ifndef DAY_5_HPP
#define DAY_5_HPP

#include <string>
#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>
#include <cstdlib>

#include "md5/md5.hpp"

namespace Day5
{
	struct MD5_RESULT
	{
		long long high;
		long long low;
	};

	MD5_RESULT md5(const std::string& text)
	{
		unsigned char digest[16];
		md5_digest(text, digest);

		MD5_RESULT result{ 0LL, 0LL };
		for (size_t i = 0; i < 8; ++i)
		{
			result.high <<= 8;
			result.high += static_cast<short>(digest[i]);
		}

		for (size_t i = 8; i < 16; ++i)
		{
			result.low <<= 8;
			result.low += static_cast<short>(digest[i]);
		}

		return result;
	}

	inline long long msb(long long value, unsigned short bits)
	{
		if (bits < 64)
			return value >> (64 - bits);
		return value;
	}

	void test1()
	{
		MD5_RESULT a = md5(std::string("abc3231928"));
		MD5_RESULT b = md5(std::string("abc3231929"));
		std::cout << std::hex << std::setw(16) << std::setfill('0') << a.high
			<< std::hex << std::setw(16) << std::setfill('0') << a.low << "\n";
		std::cout << std::hex << std::setw(16) << std::setfill('0') << b.high
			<< std::hex << std::setw(16) << std::setfill('0') << b.low << "\n";
		long long a5 = msb(a.high, 20);
		long long b5 = msb(b.high, 20);
		std::cout << a5 << "\n";
		std::cout << b5 << "\n";
		return;
	}

	void day_5_part_1()
	{
		std::string password;
		std::string input;
		if (std::getline(std::cin, input))
		{
			for (long long i = 0LL; i < std::numeric_limits<long long>::max(); ++i)
			{
				input += std::to_string(i);
				MD5_RESULT hash = md5(input);
				if (msb(hash.high, 20) == 0LL)
				{
					std::cout << std::hex << std::setw(16) << std::setfill('0') << hash.high
						<< std::hex << std::setw(16) << std::setfill('0') << hash.low << "\n";
					char ch[1];
					std::sprintf(ch, "%01x", static_cast<char>(0xF & msb(hash.high, 24)));
					password += ch[0];
					if (password.size() == 8)
						break;
				}
				input.erase(8);
			}
			std::cout << "What is the password? " << password << "\n";
		}
		return;
	}

	void day_5_part_2()
	{
		const char CH = '_';
		std::string password(8, CH);
		std::string input;
		if (std::getline(std::cin, input))
		{
			for (long long i = 0LL; i < std::numeric_limits<long long>::max(); ++i)
			{
				input += std::to_string(i);
				MD5_RESULT hash = md5(input);
				if (msb(hash.high, 20) == 0LL)
				{
					int pos = 0xF & msb(hash.high, 24);
					if (0 <= pos && pos <= 7 && password[pos] == CH)
					{
						std::cout << std::hex << std::setw(16) << std::setfill('0') << hash.high
							<< std::hex << std::setw(16) << std::setfill('0') << hash.low << "\n";

						char ch[1];
						std::sprintf(ch, "%01x", static_cast<char>(0xF & msb(hash.high, 28)));
						password[pos] = ch[0];
						if (password.find(CH) == -1)
							break;
					}
				}
				input.erase(8);
			}
			std::cout << "What is the password? " << password << "\n";
		}
		return;
	}
}

int day_5_main(unsigned int part)
{
	if (part == 1)
		Day5::day_5_part_1(); //Your puzzle answer was c6697b55.
	else
		Day5::day_5_part_2(); //Your puzzle answer was 8c35d1ab.
	return 0;
}

#endif //DAY_5_HPP