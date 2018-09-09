#ifndef DAY_13_HPP
#define DAY_13_HPP

#include <iostream>
#include <tuple>
#include <queue>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

namespace Day13
{
	static size_t N; //office designer's favorite number
	static size_t START_X_1;
	static size_t START_Y_1;
	static size_t GOAL_X_1;
	static size_t GOAL_Y_1;

	using coord2d = std::tuple<size_t, size_t>;

	static inline size_t count_bits_on(size_t x, size_t y) 
	{
		size_t n = x * x + 3 * x + 2 * x * y + y + y * y;
		n += N;
		size_t count = 0;
		while (n > 0) 
		{
			count += (n & 0x1) == 0x1 ? 1 : 0;
			n >>= 1;
		}
		return count;
	}

	static inline bool is_open(size_t x, size_t y)
	{
		return (count_bits_on(x, y) & 0x1) == 0x0;
	}

	struct coord2d_hash : public std::unary_function<coord2d, size_t>
	{
		std::size_t operator()(coord2d key) const
		{
			const int PRIME = 199;
			size_t h = std::get<0>(key) + PRIME * std::get<1>(key);
			return h;
		}
	};
	
	static std::unordered_map<coord2d, size_t, coord2d_hash> path_cost;
	static std::unordered_map<coord2d, coord2d, coord2d_hash> previous;

	struct path_cost_min
	{
		bool operator()(coord2d lhs, coord2d rhs) 
		{
			return path_cost[lhs] > path_cost[rhs];
		}
	};

	static std::priority_queue<coord2d, std::vector<coord2d>, path_cost_min> pq;

	static inline bool relax(coord2d source, coord2d target, size_t cost)
	{
		size_t new_cost = cost + 1;
		auto iter = path_cost.find(target);
		if (iter == path_cost.end() || new_cost < iter->second)
		{
			path_cost[target] = new_cost;
			previous[target] = source;
			pq.emplace(target);
			return true;
		}
		return false;
	}

	static size_t astar_shortest_path(coord2d start, coord2d goal)
	{
		path_cost.emplace(start, 0);
		
		pq.emplace(start);

		bool has_path = false;
		while (!pq.empty())
		{
			coord2d source = pq.top();
			pq.pop();
			if (source == goal)
			{
				has_path = true;
				break;
			}

			size_t x = std::get<0>(source), y = std::get<1>(source);
			size_t cost = path_cost[source];

			//up
			if (y >= 1 && is_open(x, y - 1))
			{
				coord2d target = std::make_tuple(x, y - 1);
				relax(source, target, cost);
			}
			//down (extend infinitely toward positive y)
			if (is_open(x, y + 1))
			{
				coord2d target = std::make_tuple(x, y + 1);
				relax(source, target, cost);
			}
			//left
			if (x >= 1 && is_open(x - 1, y))
			{
				coord2d target = std::make_tuple(x - 1, y);
				relax(source, target, cost);
			}
			//right (extend infinitely toward positive x) 
			if (is_open(x + 1, y))
			{
				coord2d target = std::make_tuple(x + 1, y);
				relax(source, target, cost);
			}
		}

		if (has_path)
		{
			//path count
			coord2d i = previous[goal];
			size_t steps = 1;
			while (i != start)
			{
				i = previous[i];
				++steps;
			}
			return steps;
		}
		return 0;
	}

	static std::queue<coord2d> q;
	static std::unordered_set<coord2d, coord2d_hash> visited;

	static inline void visit(coord2d target, size_t cost)
	{
		if (visited.find(target) == visited.end())
		{
			size_t new_cost = cost + 1;
			q.push(target);
			path_cost[target] = new_cost;
			visited.emplace(target);
		}
	}

	static size_t bfs_count(coord2d start, size_t at_most_k_steps)
	{
		path_cost.emplace(start, 0);
		
		q.push(start);
		visited.emplace(start);
		
		while (!q.empty())
		{
			coord2d source = q.front();
			q.pop();
			size_t x = std::get<0>(source), y = std::get<1>(source);
			size_t cost = path_cost[source];			
			//up
			if (y >= 1 && is_open(x, y - 1))
			{
				coord2d target = std::make_tuple(x, y - 1);
				if (cost < at_most_k_steps)
					visit(target, cost);
			}
			//down (extend infinitely toward positive y)
			if (is_open(x, y + 1))
			{
				coord2d target = std::make_tuple(x, y + 1);
				if (cost < at_most_k_steps)
					visit(target, cost);
			}
			//left
			if (x >= 1 && is_open(x - 1, y))
			{
				coord2d target = std::make_tuple(x - 1, y);
				if (cost < at_most_k_steps)
					visit(target, cost);
			}
			//right (extend infinitely toward positive x) 
			if (is_open(x + 1, y))
			{
				coord2d target = std::make_tuple(x + 1, y);
				if (cost < at_most_k_steps)
					visit(target, cost);
			}
		}

		return visited.size();
	}

	static void display_path(coord2d start, coord2d goal)
	{
		std::cout << "path: ";
		std::cout << "(" << std::get<0>(goal) << ", " << std::get<1>(goal) << ") <- ";
		coord2d i = previous[goal];
		while (i != start)
		{
			std::cout << "(" << std::get<0>(i) << ", " << std::get<1>(i) << ") <- ";
			i = previous[i];			
		}
		std::cout << "(" << std::get<0>(start) << ", " << std::get<1>(start) << ")\n";		
	}

	static std::unordered_set<coord2d, coord2d_hash> get_path(coord2d start, coord2d goal)
	{
		std::unordered_set<coord2d, coord2d_hash> path;
		path.emplace(goal);
		coord2d i = previous[goal];
		size_t steps = 1;
		while (i != start)
		{
			path.emplace(i);
			i = previous[i];
		}
		path.emplace(start);
		return path;
	}

	static void display_maze(coord2d start, coord2d goal)
	{
		std::unordered_set<coord2d, coord2d_hash> in_path = get_path(start, goal);
		
		size_t HEIGHT = 0, WIDTH = 0;
		for (coord2d path : in_path)
		{
			WIDTH = std::max(std::get<0>(path), WIDTH);
			HEIGHT = std::max(std::get<1>(path), HEIGHT);
		}
		++WIDTH; ++HEIGHT;

		std::cout << "\n";
		for (size_t y = 0; y <= HEIGHT; ++y)
		{
			for (size_t x = 0; x <= WIDTH; ++x)
			{
				if (in_path.find(std::make_tuple(x, y)) == in_path.end())
					std::cout << (is_open(x, y) ? '.' : '#');
				else
					std::cout << 'O';
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}

	void day_13_part_1()
	{
		std::cin >> N >> START_X_1 >> START_Y_1 >> GOAL_X_1 >> GOAL_Y_1;
		coord2d start = std::make_tuple(START_X_1, START_Y_1);
		coord2d goal = std::make_tuple(GOAL_X_1, GOAL_Y_1);
		size_t steps = astar_shortest_path(start, goal);
		//display_path(start, goal); //DBG
		std::cout << "What is the fewest number of steps required for you to reach 31,39? " << steps << "\n";
		//display_maze(start, goal); //DBG
	}

	void day_13_part_2()
	{
		std::cin >> N >> START_X_1 >> START_Y_1;
		coord2d start = std::make_tuple(START_X_1, START_Y_1);
		size_t count = bfs_count(start, 50);
		std::cout << "How many locations (distinct x,y coordinates, including your starting location) can you reach in at most 50 steps? " << count << "\n";
	}
};

int day_13_main(unsigned int part)
{
	if (part == 1)
		Day13::day_13_part_1(); //Your puzzle answer was 96.
	else
		Day13::day_13_part_2(); //Your puzzle answer was 141.
	return 0;
}

#endif /* DAY_13_HPP */