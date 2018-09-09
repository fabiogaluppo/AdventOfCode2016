#ifndef DAY_22_HPP
#define DAY_22_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <tuple>
#include <memory>
#include <cmath>
#include <queue>
#include <unordered_set>

namespace Day22
{
	using node_info = std::tuple<std::string, int, int>;

	static inline const std::string& get_name(const node_info& x) { return std::get<0>(x); }

	static inline int get_used(const node_info& x) { return std::get<1>(x); }

	static inline int get_avail(const node_info& x) { return std::get<2>(x); }

	static inline node_info parse_info(const std::string& input)
	{
		std::string node;
		int size, used, avail, use_perc;

		char* info = const_cast<char*>(input.c_str());
		const char* pattern = " ";
		char* p = std::strtok(info, pattern);
		if (p != nullptr)
		{
			node = p;
			std::string temp;
			p = std::strtok(nullptr, pattern);
			temp = p; temp.resize(temp.size() - 1);
			size = std::atoi(temp.c_str());
			p = std::strtok(nullptr, pattern);
			temp = p; temp.resize(temp.size() - 1);
			used = std::atoi(temp.c_str());
			p = std::strtok(nullptr, pattern);
			temp = p; temp.resize(temp.size() - 1);
			avail = std::atoi(temp.c_str());
			p = std::strtok(nullptr, pattern);
			temp = p; temp.resize(temp.size() - 1);
			use_perc = std::atoi(temp.c_str());
		}

		return std::make_tuple(node, used, avail);
	}

	using coord_2d = std::tuple<size_t, size_t>;

	static inline coord_2d get_xy(std::string node_name /* by value due to strtok */)
	{
		char* info = const_cast<char*>(node_name.c_str());
		const char* pattern = "/";
		char* p = std::strtok(info, pattern);
		p = std::strtok(nullptr, pattern);
		p = std::strtok(nullptr, pattern);
		const char* pattern2 = "-";
		p = std::strtok(p, pattern2);
		p = std::strtok(nullptr, pattern2);
		size_t x = std::atoi(++p);
		p = std::strtok(nullptr, pattern2);
		size_t y = std::atoi(++p);
		return std::make_tuple(x, y);
	}

	static inline long long manhattan_distance(size_t px, size_t py, size_t qx, size_t qy)
	{
		auto dx = static_cast<long long>(px) - static_cast<long long>(qx);
		auto dy = static_cast<long long>(py) - static_cast<long long>(qy);
		return std::abs(dx) + std::abs(dy);
	}

	struct game_state final
	{
		enum class items : char
		{
			OPENED = '.', GOAL_DATA = 'G', CLOSED = '#', EMPTY = '_'
		};

		game_state(size_t width, size_t height, std::vector<coord_2d> opened, coord_2d empty)
			: gx(width - 1), gy(0), ex(std::get<0>(empty)), ey(std::get<1>(empty)),
			width(width), height(height), depth_count(0), data(width * height, items::CLOSED)
		{
			//eager game state
			for (auto xy : opened) set(std::get<0>(xy), std::get<1>(xy), game_state::items::OPENED);
			set(ex, ey, game_state::items::EMPTY);
			set(gx, gy, items::GOAL_DATA); //goal starts at top-right corner
		}

		game_state(const game_state&) = default;

		game_state& operator=(const game_state&) = default;

		long long score_to_finish() const { return manhattan_distance(0, 0, gx, gy); }

		long long score_to_move_goal() const { return manhattan_distance(ex, ey, gx, gy); }

		long long depth() const { return depth_count; }

		std::vector<std::shared_ptr<game_state>> move_next() const
		{
			std::vector<std::shared_ptr<game_state>> moves;

			//up
			if (ey > 0 && get(ex, ey - 1) != items::CLOSED)
			{
				std::unique_ptr<game_state> up(new game_state(*this));
				++up->depth_count; //increase move count
				if (ex == gx && ey - 1 == gy) { up->gy = ey; } //update goal
				up->swap(ex, ey, ex, ey - 1); //swap items
				--up->ey; //update empty						 
				moves.push_back(std::move(up));
			}

			//down
			if (ey + 1 < height && get(ex, ey + 1) != items::CLOSED)
			{
				std::unique_ptr<game_state> down(new game_state(*this));
				++down->depth_count; //increase move count
				if (ex == gx && ey + 1 == gy) { down->gy = ey; } //update goal
				down->swap(ex, ey, ex, ey + 1); //swap items
				++down->ey; //update empty						   
				moves.push_back(std::move(down));
			}

			//left
			if (ex > 0 && get(ex - 1, ey) != items::CLOSED)
			{
				std::unique_ptr<game_state> left(new game_state(*this));
				++left->depth_count; //increase move count
				if (ex - 1 == gx && ey == gy) { left->gx = ex; } //update goal
				left->swap(ex, ey, ex - 1, ey); //swap items
				--left->ex; //update empty						   
				moves.push_back(std::move(left));
			}

			//right
			if (ex + 1 < width && get(ex + 1, ey) != items::CLOSED)
			{
				std::unique_ptr<game_state> right(new game_state(*this));
				++right->depth_count; //increase move count
				if (ex + 1 == gx && ey == gy) { right->gx = ex; } //update goal
				right->swap(ex, ey, ex + 1, ey); //swap items
				++right->ex; //update empty							
				moves.push_back(std::move(right));
			}

			return std::move(moves);
		}

		std::string str() const
		{
			std::string temp;
			for (size_t y = 0; y < height; ++y)
			{
				for (size_t x = 0; x < width; ++x)
				{
					temp += static_cast<char>(get(x, y));
				}
				temp += '\n';
			}
			temp += std::to_string(score_to_finish());
			temp += ' ';
			temp += std::to_string(score_to_move_goal());
			temp += ' ';
			temp += std::to_string(depth());
			temp += '\n';
			return std::move(temp);
		}

		friend inline bool operator==(const std::shared_ptr<game_state>& lhs, const std::shared_ptr<game_state>& rhs)
		{
			return (lhs->gx == rhs->gx) && (lhs->gy == rhs->gy) && (lhs->ex == rhs->ex) && (lhs->ey == rhs->ey) &&
				(lhs->width == rhs->width) && (lhs->height == rhs->height);
		}

		friend struct game_state_hash;

	private:
		items get(size_t x, size_t y) const { return data[y * width + x]; }

		void set(size_t x, size_t y, items item) { data[y * width + x] = item; }

		void swap(size_t px, size_t py, size_t qx, size_t qy) { std::swap(data[py * width + px], data[qy * width + qx]); }

	private:
		size_t gx, gy; //goal xy
		size_t ex, ey; //empty xy
		size_t width, height;
		long long depth_count;
		std::vector<items> data;
	};

	struct game_state_hash final
	{
		size_t operator()(const std::shared_ptr<game_state>& state) const
		{
			const size_t PRIME = 61;
			return (((((state->gx) * PRIME + state->gy) * PRIME + state->ex) * PRIME + state->ey) * PRIME + state->width) * PRIME + state->height;
		}
	};

	static inline long long solving_with_astar(std::shared_ptr<game_state> start_state)
	{
		struct game_state_min
		{
			bool operator()(const std::shared_ptr<game_state>& lhs, const std::shared_ptr<game_state>& rhs) const
			{
				long long l = lhs->score_to_finish();
				long long r = rhs->score_to_finish();
				if (l == r)
				{
					l = lhs->score_to_move_goal();
					r = rhs->score_to_move_goal();
					return l > r;
				}
				return l > r;
			}
		};

		std::priority_queue<std::shared_ptr<game_state>, std::vector<std::shared_ptr<game_state>>, game_state_min> pq;
		std::unordered_set<std::shared_ptr<game_state>, game_state_hash> visited;

		visited.insert(start_state);
		pq.emplace(start_state);

		while (!pq.empty())
		{
			const std::shared_ptr<game_state>& state = pq.top();
			//std::cout << state->str() << std::endl; //DBG
			if (state->score_to_finish() == 0)
				return state->depth();
			std::vector<std::shared_ptr<game_state>> next_moves = state->move_next();
			pq.pop();
			for (std::shared_ptr<game_state> next_state : next_moves)
			{
				if (visited.find(next_state) == visited.end())
				{
					visited.insert(next_state);
					pq.emplace(std::move(next_state));
				}
			}
		}
		return -1;
	}

	void day_22_part_1()
	{
		std::string input;
		//ignoring header begin
		std::getline(std::cin, input);
		std::getline(std::cin, input);
		//ignoring header end
		std::vector<node_info> nodes_asc_by_used;
		while (std::getline(std::cin, input))
		{
			nodes_asc_by_used.push_back(parse_info(input));
		}
		//nodes -> ascending order by used
		std::sort(nodes_asc_by_used.begin(), nodes_asc_by_used.end(),
			[](const node_info& lhs, const node_info& rhs) { return get_used(lhs) < get_used(rhs); });
		std::vector<node_info> nodes_desc_by_avail(nodes_asc_by_used.begin(), nodes_asc_by_used.end());
		//nodes_desc_by_avail -> descending order by avail
		std::sort(nodes_desc_by_avail.begin(), nodes_desc_by_avail.end(),
			[](const node_info& lhs, const node_info& rhs) { return get_avail(lhs) > get_avail(rhs); });

		size_t viable_pairs_counter = 0;
		for (size_t i = 0; i < nodes_asc_by_used.size(); ++i)
		{
			if (get_used(nodes_asc_by_used[i]) > 0)
			{
				for (size_t j = 0; j < nodes_desc_by_avail.size() && get_used(nodes_asc_by_used[i]) <= get_avail(nodes_desc_by_avail[j]); ++j)
				{
					if (get_name(nodes_asc_by_used[i]) != get_name(nodes_desc_by_avail[j]))
						++viable_pairs_counter;
				}
			}
		}
		std::cout << "How many viable pairs of nodes are there? " << viable_pairs_counter << "\n";
	}

	void day_22_part_2()
	{
		std::string input;
		//ignoring header begin
		std::getline(std::cin, input);
		std::getline(std::cin, input);
		//ignoring header end
		std::vector<node_info> nodes_asc_by_used;
		while (std::getline(std::cin, input))
		{
			nodes_asc_by_used.push_back(parse_info(input));
		}
		auto last_node = get_xy(get_name(nodes_asc_by_used[nodes_asc_by_used.size() - 1]));
		size_t WIDTH = std::get<0>(last_node) + 1;
		size_t HEIGHT = std::get<1>(last_node) + 1;
		//nodes -> ascending order by used
		std::sort(nodes_asc_by_used.begin(), nodes_asc_by_used.end(),
			[](const node_info& lhs, const node_info& rhs) { return get_used(lhs) < get_used(rhs); });
		std::vector<node_info> nodes_desc_by_avail(nodes_asc_by_used.begin(), nodes_asc_by_used.end());
		//nodes_desc_by_avail -> descending order by avail
		std::sort(nodes_desc_by_avail.begin(), nodes_desc_by_avail.end(),
			[](const node_info& lhs, const node_info& rhs) { return get_avail(lhs) > get_avail(rhs); });

		std::unordered_set<std::string> node_names;
		for (size_t i = 0; i < nodes_asc_by_used.size(); ++i)
		{
			if (get_used(nodes_asc_by_used[i]) > 0)
			{
				for (size_t j = 0; j < nodes_desc_by_avail.size() && get_used(nodes_asc_by_used[i]) <= get_avail(nodes_desc_by_avail[j]); ++j)
				{
					node_names.emplace(get_name(nodes_asc_by_used[i]));
					node_names.emplace(get_name(nodes_desc_by_avail[j]));
				}
			}
		}
		std::vector<coord_2d> opened(node_names.size());
		std::transform(node_names.begin(), node_names.end(), opened.begin(),
			[](const std::string& node_name) { return get_xy(node_name); });

		auto empty = std::find_if(nodes_asc_by_used.begin(), nodes_asc_by_used.end(),
			[](const node_info& node) { return get_used(node) == 0; });

		std::shared_ptr<game_state> start_state(new game_state(WIDTH, HEIGHT, std::move(opened), get_xy(get_name(*empty))));
		long long number_of_steps = solving_with_astar(std::move(start_state));

		std::cout << "What is the fewest number of steps required to move your goal data to node-x0-y0? " << number_of_steps << "\n";
	}
}

int day_22_main(unsigned int part)
{
	if (part == 1)
		Day22::day_22_part_1(); //Your puzzle answer was 1043.
	else
		Day22::day_22_part_2(); //Your puzzle answer was 185.
	return 0;
}

#endif /* DAY_22_HPP */