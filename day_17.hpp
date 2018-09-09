#ifndef DAY_17_HPP
#define DAY_17_HPP

#include <string>
#include <iostream>
#include <tuple>
#include <queue>
#include <vector>
#include <stack>
#include <cstdlib>

#include "md5/md5.hpp"

namespace Day17
{
	static inline std::string md5_first_four_chars(const std::string& input)
	{
		std::string result = md5_string(input);
		result.resize(4);
		return result;
	}

	enum struct direction : char
	{
		UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3
	};

	static inline bool is_opened(const std::string& hash, direction door)
	{
		size_t idx = static_cast<size_t>(door);
		return 'b' <= hash[idx] && hash[idx] <= 'f';
	}

	static inline bool is_closed(const std::string& hash, direction door)
	{
		return !is_opened(hash, door);
	}

	using coord2d = std::tuple<size_t, size_t>;

	static inline bool not_bound(coord2d pos, direction d, size_t N)
	{
		switch (d)
		{
		case direction::UP:
			return std::get<0>(pos) > 0;
		case direction::DOWN:
			return std::get<0>(pos) < (N - 1);
		case direction::LEFT:
			return std::get<1>(pos) > 0;
		//case direction::RIGHT:
		default:
			return std::get<1>(pos) < (N - 1);
		}
	}

	static const direction directions[]{ direction::RIGHT, direction::LEFT, direction::DOWN, direction::UP };
	static const char direction_chars[]{ 'U', 'D', 'L', 'R' };

	struct node
	{
		coord2d pos;
		std::string str;

		node& update(direction d)
		{
			switch (d)
			{
			case direction::UP:
				--std::get<0>(pos);
				break;
			case direction::DOWN:
				++std::get<0>(pos);
				break;
			case direction::LEFT:
				--std::get<1>(pos);
				break;
			//case direction::RIGHT:
			default:
				++std::get<1>(pos);
				break;
			}
			str.push_back(direction_chars[static_cast<size_t>(d)]);
			return *this;
		}
	};

	struct node_cost_min
	{
		bool operator()(const node& lhs, const node& rhs) const
		{
			return lhs.str.size() > rhs.str.size();
		}
	};

	static inline std::string astar_shortest_path(coord2d start, const std::string& init, size_t SQUARE_BOUND)
	{
		const coord2d goal{ SQUARE_BOUND - 1, SQUARE_BOUND - 1 };

		std::priority_queue<node, std::vector<node>, node_cost_min> pq;
		pq.emplace(node{ start, init });

		while (!pq.empty())
		{
			node source = pq.top();
			pq.pop();
			if (source.pos == goal)
			{
				source.str.erase(0, init.size());
				return source.str;
			}
			std::cout << source.str << "\n"; //LIXO
			std::string hash = md5_first_four_chars(source.str);
			for (direction d : directions)
			{
				if (is_opened(hash, d) && not_bound(source.pos, d, SQUARE_BOUND))
				{
					pq.emplace(node(source).update(d));
				}
			}
		}

		return std::string();
	}

	static size_t dfs_count(coord2d start, const std::string& init, size_t SQUARE_BOUND)
	{
		const coord2d goal{ SQUARE_BOUND - 1, SQUARE_BOUND - 1 };

		std::stack<node> s;
		s.push(node{ start, init });

		size_t longest_path_size = 0;

		while (!s.empty())
		{
			node source = s.top();
			s.pop();
			if (source.pos == goal)
			{
				size_t temp = source.str.size() - init.size();
				if (temp > longest_path_size)
					longest_path_size = temp;
				continue;
			}

			std::string hash = md5_first_four_chars(source.str);
			for (direction d : directions)
			{
				if (is_opened(hash, d) && not_bound(source.pos, d, SQUARE_BOUND))
				{
					s.push(node(source).update(d));
				}
			}
		}

		return longest_path_size;
	}

	void day_17_part_1()
	{
		std::string init;
		std::cin >> init;
		const size_t SQUARE_BOUND = 4;
		std::string shortest_path = astar_shortest_path({ 0, 0 }, init, SQUARE_BOUND);
		std::cout << "What is the shortest path? " << shortest_path << "\n";
	}

	void day_17_part_2()
	{
		std::string init;
		std::cin >> init;

		const size_t SQUARE_BOUND = 4;
		size_t count = dfs_count({ 0, 0 }, init, SQUARE_BOUND);
		std::cout << "What is the length of the longest path? " << count << "\n";
	}
};

int day_17_main(unsigned int part)
{
	if (part == 1)
		Day17::day_17_part_1(); //Your puzzle answer was DURLDRRDRD.
	else
		Day17::day_17_part_2(); //Your puzzle answer was 650.
	return 0;
}

#endif /* DAY_17_HPP */