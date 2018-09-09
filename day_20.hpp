#ifndef DAY_20_HPP
#define DAY_20_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <tuple>
#include <queue>
#include <cstdlib>
#include <vector>
#include <functional>
#include <algorithm>

namespace Day20
{
	template<typename T>
	using min_priority_queue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

	static inline std::tuple<long long, long long> parse(const char* s)
	{
		char* p = strtok(const_cast<char*>(s), "-");
		long long a = static_cast<long long>(std::atoll(p));
		p = strtok(nullptr, "-");
		long long b = static_cast<long long>(std::atoll(p));
		return std::make_tuple(a, b);
	}

	static inline bool has_intersection(std::tuple<long long, long long> left, std::tuple<long long, long long> right)
	{
		return !(std::get<1>(right) < std::get<0>(left) || std::get<1>(left) < std::get<0>(right));
	}

	static inline bool has_connection(std::tuple<long long, long long> left, std::tuple<long long, long long> right)
	{
		return std::get<0>(right) - std::get<1>(left) <= 1;
	}

	static inline std::tuple<long long, long long> concat(std::tuple<long long, long long> left, std::tuple<long long, long long> right)
	{
		long long temp[4]{ std::get<0>(left), std::get<1>(left), std::get<0>(right), std::get<1>(right) };
		std::sort(temp, temp + 4);
		return std::make_tuple(temp[0], temp[3]);
	}

	void day_20_part_1()
	{
		min_priority_queue<long long> left_pq, right_pq;
		std::string s;
		while (std::getline(std::cin, s))
		{
			auto x = parse(s.c_str());
			left_pq.push(std::get<0>(x));
			right_pq.push(std::get<1>(x));
		}
		long long free_address = -1;
		if (left_pq.size() > 0)
		{
			auto left = std::make_tuple(left_pq.top(), right_pq.top());
			left_pq.pop();
			right_pq.pop();
			while (!left_pq.empty())
			{
				auto right = std::make_tuple(left_pq.top(), right_pq.top());
				//std::cout << std::get<0>(left) << "-" << std::get<1>(left) << ".." << std::get<0>(right) << "-" << std::get<1>(right) << "\n"; //DBG
				if (!has_intersection(left, right) && !has_connection(left, right))
					break;
				left = concat(left, right);
				left_pq.pop();
				right_pq.pop();
			}
			//std::cout << std::get<0>(left) << "-" << std::get<1>(left) << "\n"; //DBG
			free_address = std::get<0>(left) > 0 ? std::get<0>(left) - 1 : std::get<1>(left) + 1;
		}
		std::cout << "What is the lowest-valued IP that is not blocked? " << free_address << "\n";
	}

	void day_20_part_2()
	{
		min_priority_queue<long long> left_pq, right_pq;
		std::string s;
		while (std::getline(std::cin, s))
		{
			auto x = parse(s.c_str());
			left_pq.push(std::get<0>(x));
			right_pq.push(std::get<1>(x));
		}
		const long long MAX_ADDRESSES_COUNT = 4294967296;
		long long free_addresses_count = -1;
		if (left_pq.size() > 0)
		{
			long long blacklist_count = 0;
			auto left = std::make_tuple(left_pq.top(), right_pq.top());
			left_pq.pop();
			right_pq.pop();
			while (!left_pq.empty())
			{
				auto right = std::make_tuple(left_pq.top(), right_pq.top());
				if (!has_intersection(left, right) && !has_connection(left, right))
				{
					blacklist_count += (1 + std::get<1>(left) - std::get<0>(left));
					left = right;
				}
				else
				{
					left = concat(left, right);
				}
				left_pq.pop();
				right_pq.pop();
			}
			blacklist_count += (1 + std::get<1>(left) - std::get<0>(left));
			free_addresses_count = MAX_ADDRESSES_COUNT - blacklist_count;
		}
		std::cout << "How many IPs are allowed by the blacklist? " << free_addresses_count << "\n";
	}
};

int day_20_main(unsigned int part)
{
	if (part == 1)
		Day20::day_20_part_1(); //Your puzzle answer was 31053880.
	else
		Day20::day_20_part_2(); //Your puzzle answer was 117.
	return 0;
}

#endif /* DAY_20_HPP */