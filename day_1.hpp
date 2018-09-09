#ifndef DAY_1_HPP
#define DAY_1_HPP

#include <tuple>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <utility>
#include <set>

namespace Day1
{
	enum Face { N = 0, S = 1, E = 2, W = 3 };

	struct State
	{
		std::tuple<int, int> intersection;
		Face face;
	};

	template<typename T>
	static bool try_insert(std::set<T>& xs, const T& x)
	{
		if (xs.find(x) == xs.end())
		{
			xs.insert(x);
			return true;
		}
		return false;
	}
	
	template<typename T>
	static constexpr inline T manhattan_distance(std::tuple<T, T> p, std::tuple<T, T> q = { 0, 0 })
	{
		return std::abs(std::get<0>(p) - std::get<0>(q)) + std::abs(std::get<1>(p) - std::get<1>(q));
	}
	
	static inline std::tuple<int, int> add_x(std::tuple<int, int> intersection, int x)
	{
		std::get<0>(intersection) += x;
		return intersection;
	}

	static inline std::tuple<int, int> add_y(std::tuple<int, int> intersection, int y)
	{
		std::get<1>(intersection) += y;
		return intersection;
	}

	static inline State& turn(State& state, Face _to, int steps)
	{
		static int  direction[]{ 1, -1, 1, -1 }; //{ N, S, E, W }
		steps *= direction[_to];
		state.face = _to;
		state.intersection = _to == N || _to == S ?
			add_y(state.intersection, steps) :
			add_x(state.intersection, steps);
		return state;
	}

	static inline State turn_left(State state, int steps)
	{
		static Face _to[]{ W, E, N, S };
		return turn(state, _to[state.face], steps);

		/*State new_state;
		switch (state.face)
		{
		case N:
			new_state.face = W;
			new_state.intersection = add_x(state.intersection, -steps);
			break;
		case S:
			new_state.face = E;
			new_state.intersection = add_x(state.intersection, steps);
			break;
		case W:
			new_state.face = S;
			new_state.intersection = add_y(state.intersection, -steps);
			break;
		case E:
			new_state.face = N;
			new_state.intersection = add_y(state.intersection, steps);
			break;
		}
		return new_state;*/
	}

	static inline State turn_right(State state, int steps)
	{
		static Face _to[]{ E, W, S, N };
		return turn(state, _to[state.face], steps);

		/*State new_state;
		switch (state.face)
		{
		case N:
			new_state.face = E;
			new_state.intersection = add_x(state.intersection, steps);
			break;
		case S:
			new_state.face = W;
			new_state.intersection = add_x(state.intersection, -steps);
			break;
		case W:
			new_state.face = N;
			new_state.intersection = add_y(state.intersection, steps);
			break;
		case E:
			new_state.face = S;
			new_state.intersection = add_y(state.intersection, -steps);
			break;
		}
		return new_state;*/
	}

	static inline State parse(const char* cmd, State state)
	{
		char turn = cmd[0];
		//assert turn 'R' or 'L'
		int steps = std::atoi(++cmd);
		return turn == 'R' ? turn_right(state, steps) : turn_left(state, steps);
	}

	static bool test_1()
	{
		State initial = { { 0, 0 }, N };

		//R2, L3
		bool result1 = 5 == manhattan_distance(initial.intersection, turn_left(turn_right(initial, 2), 3).intersection);

		//R2, R2, R2
		bool result2 = 2 == manhattan_distance(initial.intersection, turn_right(turn_right(turn_right(initial, 2), 2), 2).intersection);

		//R5, L5, R5, R3
		bool result3 = 12 == manhattan_distance(initial.intersection, turn_right(turn_right(turn_left(turn_right(initial, 5), 5), 5), 3).intersection);

		return result1 && result2 && result3;
	}

	void day_1_part_1()
	{
		State initial = { { 0, 0 }, N };
		std::string str;
		std::getline(std::cin, str);
		char* moves = const_cast<char*>(str.c_str());
		const char* pattern = ", ";
		char* p = std::strtok(moves, pattern);
		State state = initial;
		while (p != nullptr)
		{
			state = parse(p, state);
			p = std::strtok(nullptr, pattern);
		}
		std::cout << "How many blocks away is Easter Bunny HQ? " << manhattan_distance(initial.intersection, state.intersection) << "\n";
	}

	static inline void adjust_order(int& a, int& b)
	{
		if (a > b)
		{
			std::swap(a, b);
			--b;
		}
		else
		{
			++a;
		}
	}

	static inline bool intersects(std::set<std::tuple<int, int>>& visited, int x, int y)
	{
		auto pos = std::make_tuple(x, y);
		if (!try_insert(visited, pos))
		{
			std::cout << manhattan_distance(pos) << "\n";
			return true;
		}
		return false;
	}

	void day_1_part_2()
	{
		std::cout << "How many blocks away is the first location you visit twice? ";
		State initial = { { 0, 0 }, N };
		std::string str;
		std::getline(std::cin, str);
		char* moves = const_cast<char*>(str.c_str());
		const char* pattern = ", ";
		char* p = std::strtok(moves, pattern);
		State state = initial;
		std::set<std::tuple<int, int>> visited;
		visited.insert(initial.intersection);
		while (p != nullptr)
		{
			int old_x, old_y, new_x, new_y;
			std::tie(old_x, old_y) = state.intersection;
			state = parse(p, state);
			std::tie(new_x, new_y) = state.intersection;
			if (new_x == old_x)
			{
				//y varying
				int x = new_x;
				adjust_order(old_y, new_y);
				for (int y = old_y; y <= new_y; ++y)
					if (intersects(visited, x, y))
						return;
			}
			else
			{
				//x varying
				int y = new_y;
				adjust_order(old_x, new_x);
				for (int x = old_x; x <= new_x; ++x)
					if (intersects(visited, x, y))
						return;
			}
			p = std::strtok(nullptr, pattern);
		}
		std::cout << "no intersection\n";
	}
};

int day_1_main(unsigned int part)
{
	if (part == 1)
		Day1::day_1_part_1(); //Your puzzle answer was 241.
	else
		Day1::day_1_part_2(); //Your puzzle answer was 116.
	return 0;
}

#endif //DAY_1_HPP