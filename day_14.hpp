#ifndef DAY_14_HPP
#define DAY_14_HPP

#include <string>
#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <iostream>

#include "md5/md5.hpp"

namespace Day14
{
	static inline void md5(std::string& text)
	{
		text = md5_string(text);
	}

	static inline size_t three_in_a_row(const std::string& s)
	{
		for (size_t i = 0; i < s.size() - 2; ++i)
		{
			if (s[i] == s[i + 1] && s[i + 1] == s[i + 2])
				return i;
		}
		return s.size();
	}

	static inline size_t five_in_a_row(const std::string& s)
	{
		for (size_t i = 0; i < s.size() - 4; ++i)
		{
			if (s[i] == s[i + 1] && s[i + 1] == s[i + 2] &&
				s[i + 2] == s[i + 3] && s[i + 3] == s[i + 4])
				return i;
		}
		return s.size();
	}

	static inline std::string cached_get_or_set(std::unordered_map<size_t, std::string>& cache, const char* salt, size_t key_stretching_size, size_t index)
	{
		std::string s;
		auto iter = cache.find(index);
		if (iter == cache.end())
		{
			s = salt + std::to_string(index);
			for (size_t i = 0; i <= key_stretching_size; ++i)
				md5(s);
			cache.emplace(index, s);
		}
		else
		{
			s = iter->second;
		}
		return s;
	}

	static inline void cached_del(std::unordered_map<size_t, std::string>& cache, size_t index)
	{
		auto iter = cache.find(index);
		if (iter != cache.end())
			cache.erase(iter->first);
	}

	static inline size_t find_kth_key(const char* salt, size_t kth, size_t key_stretching_size = 0)
	{
		std::unordered_map<size_t, std::string> cache;
		cache.reserve(1001);
		size_t count = 0, index = 0;
		while (index != -1)
		{
			std::string s3 = cached_get_or_set(cache, salt, key_stretching_size, index);
			size_t i = three_in_a_row(s3);
			if (i != s3.size())
			{
				for (size_t k = index + 1; k < index + 1 + 1000; ++k)
				{
					std::string s5 = cached_get_or_set(cache, salt, key_stretching_size, k);
					size_t j = five_in_a_row(s5);
					if (j != s5.size())
					{
						if (s3[i] == s5[j])
						{
							if (++count == kth)
								return index;
							else
								break;
						}
					}
				}
			}
			cached_del(cache, index);
			++index;
		}
		return -1;
	}

	static char* SALT;
	static std::string salt;

	void day_14_part_1()
	{
		std::cin >> salt;
		SALT = const_cast<char*>(salt.c_str());

		size_t index = find_kth_key(SALT, 64);
		std::cout << "What index produces your 64th one-time pad key? " << index << "\n";
	}

	void day_14_part_2()
	{
		std::cin >> salt;
		SALT = const_cast<char*>(salt.c_str());

		size_t index = find_kth_key(SALT, 64, 2016);
		std::cout << "What index produces your 64th one-time pad key with 2016 additional hashings? " << index << "\n";
	}
};

int day_14_main(unsigned int part)
{
	if (part == 1)
		Day14::day_14_part_1(); //Your puzzle answer was 16106.
	else
		Day14::day_14_part_2(); //Your puzzle answer was 22423.	
	return 0;
}

#endif /* DAY_14_HPP */