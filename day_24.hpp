#ifndef DAY_24_HPP
#define DAY_24_HPP

#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <tuple>
#include <memory>
#include <stdexcept>
#include <limits>
#include <iostream>

namespace Day24
{
	//rectangular (2d-dimension) assumed
	using blueprint_type = std::vector<std::string>;
	using coord2d = std::tuple<size_t, size_t>;
	using points_type = std::vector<coord2d>;
	using count_type = std::vector<size_t>;

	struct coord2d_hash final : public std::unary_function<coord2d, size_t>
	{
		std::size_t operator()(coord2d key) const
		{
			const int PRIME = 199;
			size_t h = std::get<0>(key) + PRIME * std::get<1>(key);
			return h;
		}
	};

	using visited_type = std::unordered_set<coord2d, coord2d_hash>;
	using bfs_queue_type = std::queue<std::tuple<coord2d, size_t>>;

	static inline blueprint_type read_blueprint()
	{
		std::vector<std::string> blueprint;
		std::string str;
		while (std::getline(std::cin, str)) blueprint.push_back(str);
		return std::move(blueprint);
	}

	static inline coord2d get_2d_size(const blueprint_type& blueprint)
	{
		return std::make_tuple(blueprint.size(), blueprint[0].size());
	}

	static inline size_t char_to_index(char ch) { return ch - '0'; }

	static inline std::vector<coord2d> find_2d_points(const blueprint_type& blueprint)
	{
		size_t ROWS, COLS;
		std::tie(ROWS, COLS) = get_2d_size(blueprint);
		std::unordered_map<size_t, coord2d> points;
		for (size_t i = 0; i < ROWS; ++i)
		{
			for (size_t j = 0; j < COLS; ++j)
			{
				char ch = blueprint[i][j];
				switch (ch)
				{
				case '.':
				case '#':
					break;
				default:
					points.emplace(char_to_index(ch), std::make_tuple(i, j));
					break;
				}
			}
		}
		std::vector<coord2d> result;
		for (size_t i = 0; i < points.size(); ++i)
			result.push_back(points[i]);
		return result;
	}

	static inline bool is_blocked(char ch) { return ch == '#'; }

	static inline void visit(const blueprint_type& blueprint, visited_type& visited, bfs_queue_type& q, count_type& count,
		size_t row, size_t col, size_t dist)
	{
		char ch = blueprint[row][col];
		if (!is_blocked(ch))
		{
			coord2d p = std::make_tuple(row, col);
			if (visited.find(p) == visited.end())
			{
				q.push(std::make_tuple(p, dist));
				visited.emplace(p);
				if (ch != '.')
					count[char_to_index(ch)] = dist;
			}
		}
	}

	static inline count_type breadth_first_search(const blueprint_type& blueprint, size_t ROWS, size_t COLS, size_t N, coord2d starting_point)
	{
		bfs_queue_type q;
		visited_type visited;
		std::vector<size_t> count(N, 0);
		q.push(std::make_tuple(starting_point, 0));
		visited.emplace(starting_point);
		size_t row, col, dist;
		while (!q.empty())
		{
			std::tuple<coord2d, size_t> item = q.front();
			std::tie(row, col) = std::get<0>(item);
			dist = std::get<1>(item); 
			q.pop();
			//it can't move diagonally
			if (0 < row)
			{
				//goes up
				visit(blueprint, visited, q, count, row - 1, col, dist + 1);
			}
			if (row < ROWS)
			{
				//goes down
				visit(blueprint, visited, q, count, row + 1, col, dist + 1);
			}
			if (0 < col)
			{
				//goes left
				visit(blueprint, visited, q, count, row, col - 1, dist + 1);
			}
			if (col < COLS)
			{
				//goes right
				visit(blueprint, visited, q, count, row, col + 1, dist + 1);
			}
		}
		return count;
	}

	template <typename T>
	struct square_matrix final
	{
		square_matrix(size_t N)
			: N(N), data(N * N, T()) {}

		square_matrix(square_matrix&& that)
			: N(that.N), data(std::move(that.data))
		{
			that.N = 0;
		}

		square_matrix& operator=(square_matrix&& that)
		{
			if (this != &that)
			{
				N = that.N;
				data = std::move(that.data);
				that.N = 0;
			}
			return *this;
		}

		void set(size_t row, size_t col, const T& value)
		{
			data[row * N + col] = value;
		}

		void set(size_t row, const std::vector<T>& values)
		{
			if (values.size() == N)
			{
				std::copy(values.begin(), values.end(), data.begin() + (row * N));
				return;
			}
			throw std::runtime_error("incompatible row size");
		}

		const T& get(size_t row, size_t col) const
		{
			return data[row * N + col];
		}

		size_t side_size() const { return N; }

		square_matrix() = delete;
		~square_matrix() = default;
		square_matrix(const square_matrix&) = default;
		square_matrix& operator=(const square_matrix&) = default;

	private:
		size_t N;
		std::vector<T> data;
	};

	using distances_type = square_matrix<size_t>;
	using path_type = std::vector<size_t>;

	struct state_node final
	{
		size_t visited_count;
		size_t path_cost;
		path_type path;
		std::vector<bool> visited;

		state_node() = default;
		~state_node() = default;
		state_node(const state_node&) = default;
		state_node& operator=(const state_node&) = default;

		state_node(state_node&& that) :
			visited(std::move(that.visited)),
			visited_count(that.visited_count),
			path(std::move(that.path)),
			path_cost(that.path_cost)
		{
		}

		state_node& operator=(state_node&& that)
		{
			if (this != &that)
			{
				visited = std::move(that.visited);
				visited_count = that.visited_count;
				path = std::move(that.path);
				path_cost = that.path_cost;
			}
			return *this;
		}
	};

	static inline state_node make_state_node(const std::vector<bool>& visited, const path_type& path, size_t path_cost)
	{
		state_node node;
		node.visited = visited;
		node.visited_count = std::count(visited.begin(), visited.end(), true);
		node.path = path;
		node.path_cost = path_cost;
		return std::move(node);
	}

	struct state_node_minimize final
	{
		bool operator()(const state_node& lhs, const state_node& rhs) const
		{
			if (lhs.visited_count == rhs.visited_count)
				return lhs.path_cost > rhs.path_cost;
			return lhs.visited_count < rhs.visited_count;
		}
	};

	using state_space_type = std::priority_queue<state_node, std::vector<state_node>, state_node_minimize>;

	distances_type build_square_matrix_of_distances(const blueprint_type& blueprint, const points_type& points)
	{
		size_t ROWS, COLS;
		std::tie(ROWS, COLS) = get_2d_size(blueprint);
		size_t N = points.size();
		distances_type distances(N);
		for (size_t i = 0; i < N; ++i)
			distances.set(i, breadth_first_search(blueprint, ROWS, COLS, N, points[i]));
		return distances;
	}

	using path_cost_type = size_t;

	std::tuple<path_type, path_cost_type> best_first_search(const distances_type& distances, size_t start, size_t finish = -1)
	{
		size_t min_path_cost = std::numeric_limits<size_t>::max();
		path_type min_path;

		size_t N = distances.side_size();
		state_space_type pq;
		std::vector<bool> visited(N, false);
		visited[start] = true;
		pq.push(std::move(make_state_node(visited, path_type{ start }, path_cost_type(0))));
		while (!pq.empty())
		{
			state_node current_node = pq.top();
			pq.pop();
			if (current_node.visited_count != N || (!(finish == -1 || finish == current_node.path[current_node.path.size() - 1])))
			{
				size_t current_index = current_node.path[current_node.path.size() - 1];
				//add every node adjacent (except itself), because you can visit at least one each node (not TSP problem)
				for (size_t i = 0; i < N; ++i)
				{
					//avoid visit itself and visited neighbor
					if (i != current_index && (finish == -1 || !(i != finish && current_node.visited[i])))
					{
						path_cost_type new_path_cost = current_node.path_cost + distances.get(current_index, i);
						if (new_path_cost < min_path_cost)
						{
							path_type new_path{ current_node.path.begin(), current_node.path.end() };
							new_path.push_back(i);
							std::vector<bool> new_visited{ current_node.visited.begin(), current_node.visited.end() };
							new_visited[i] = true;
							pq.push(std::move(make_state_node(new_visited, new_path, new_path_cost)));
						}
					}
				}
			}
			else
			{
				if (current_node.path_cost < min_path_cost)
				{
					min_path = current_node.path;
					min_path_cost = std::move(current_node.path_cost);
				}
			}
		}
		return std::make_tuple(min_path, min_path_cost);
	}

	static inline std::tuple<path_type, path_cost_type> find_shortest_route(const distances_type& distances)
	{
		return best_first_search(distances, 0);
	}

	static inline void display(const distances_type& distances)
	{
		for (size_t row = 0; row < distances.side_size(); ++row)
		{
			for (size_t col = 0; col < distances.side_size(); ++col)
			{
				std::cout << distances.get(row, col) << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}

	static inline void display(const std::tuple<path_type, path_cost_type>& p)
	{
		std::cout << "cost = " << std::get<1>(p) << "\n";
		for (size_t k = 0; k < std::get<0>(p).size(); ++k)
			std::cout << std::get<0>(p)[k] << " ";
		std::cout << "\n";
	}

	void day_24_part_1()
	{
		blueprint_type blueprint = read_blueprint();
		points_type points = find_2d_points(blueprint);
		distances_type distances = build_square_matrix_of_distances(blueprint, points);
		//display(distances); //DBG
		std::tuple<path_type, path_cost_type> p = find_shortest_route(distances);
		//display(p); //DBG
		std::cout << "From location 0, what is the fewest number of steps required to visit every non-0 number marked on the map at least once? ";
		std::cout << std::get<1>(p) << "\n";
	}

	static inline std::tuple<path_type, path_cost_type> find_shortest_cycle(const distances_type& distances)
	{
		return best_first_search(distances, 0, 0);
	}

	void day_24_part_2()
	{
		blueprint_type blueprint = read_blueprint();
		points_type points = find_2d_points(blueprint);
		distances_type distances = build_square_matrix_of_distances(blueprint, points);
		//display(distances); //DBG
		std::tuple<path_type, path_cost_type> p = find_shortest_cycle(distances);
		//display(p); //DBG
		std::cout << "From location 0, what is the fewest number of steps required to visit every non-0 number marked on the map at least once, and then return to 0? ";
		std::cout << std::get<1>(p) << "\n";
	}
}

int day_24_main(unsigned int part)
{
	if (part == 1)
		Day24::day_24_part_1(); //Your puzzle answer was 456.
	else
		Day24::day_24_part_2(); //Your puzzle answer was 704.
	return 0;
}

#endif /* DAY_24_HPP */