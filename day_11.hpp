#ifndef DAY_11_HPP
#define DAY_11_HPP

#include <array>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <queue>
#include <memory>
#include <set>

namespace Day11
{
	static inline size_t count_bits_on(unsigned int x)
	{
		//Counting 1-bits using 'divide and conquer' strategy 
		//from Hacker's Delight, Henry S. Warren, Jr.
		x = x - ((x >> 1) & 0x55555555);
		x = (x & 0x33333333) + ((x >> 0x2) & 0x33333333);
		x = (x + (x >> 0x4)) & 0x0F0F0F0F;
		x = x + (x >> 0x8);
		x = x + (x >> 0x10);
		return x & 0x0000003F;
	}

	template<typename T, size_t M, size_t N> /* M rows, N columns */
	struct table
	{
		table(const table<T, M, N>& that) :
			Data_(that.Data_) {}

		table() {}

		table(const T& val)
		{
			std::fill(Data_.begin(), Data_.end(), val);
		}

		inline T cell(size_t m, size_t n) const
		{
			return Data_[_2d_to_1d(m, n)];
		}

		inline void set(size_t m, size_t n, const T& x)
		{
			Data_[_2d_to_1d(m, n)] = x;
		}

		inline std::array<T, N> row(size_t m) const
		{
			std::array<T, N> temp;
			for (size_t n = 1; n <= N; ++n)
				temp[n - 1] = cell(m, n);
			return temp;
		}

		inline std::array<T, M> column(size_t n) const
		{
			std::array<T, M> temp;
			for (size_t m = 1; m <= M; ++m)
				temp[m - 1] = cell(m, n);
			return temp;
		}

		inline const std::array<T, M * N>&  data() const 
		{ 
			return Data_;  
		}

	private:
		static inline size_t _2d_to_1d(size_t m, size_t n) 
		{ 
			return (m - 1) * N + (n - 1); 
		}
		std::array<T, M * N> Data_;
	};

	enum component_enum : char { NONE = '-', MICROCHIP = 'M', GENERATOR = 'G' };
	using component = std::tuple<char, component_enum>;
	template<size_t M, size_t N>
	using component_table = table<component, M, N>;

	const static component Elevator = std::make_tuple('E', NONE);
	const static component Empty = std::make_tuple('-', NONE);
	static inline component make_microchip(char c) { return std::make_tuple(c, MICROCHIP); }
	static inline component make_generator(char c) { return std::make_tuple(c, GENERATOR); }
	static inline bool is_elevator(component c) { return std::get<0>(c) == 'E'; }
	static inline bool is_empty(component c) { return std::get<1>(c) == NONE && std::get<0>(c) == '-'; }
	static inline bool is_microchip(component c) { return std::get<1>(c) == MICROCHIP; }
	static inline bool is_generator(component c) { return std::get<1>(c) == GENERATOR; }
	static inline bool is_component(component c) { return is_microchip(c) || is_generator(c); }

	template<size_t M, size_t N>
	static inline std::string to_string(const component_table<M, N>& ct)
	{
		std::stringstream ss;
		for (size_t m = M; m > 0; --m)
		{
			ss << "F" << m;
			for (size_t n = 1; n <= N; ++n)
			{
				auto c = ct.cell(m, n);
				auto ch = std::get<1>(c);
				ss << std::setw(3) << std::setfill(' ') 
					<< std::get<0>(c) << (ch == NONE ? '-' : static_cast<char>(ch)) << " ";
			}
			ss << "\n";
		}
		return ss.str();
	}

	//DBG
	/*template<size_t N>
	static inline std::string to_string(size_t row_index, std::array<component, N>& row)
	{
		std::stringstream ss;
		ss << "F" << row_index;
		for (component c : row)		
			ss << std::setw(3) << std::setfill(' ') 
			<< std::get<0>(c) << (char)std::get<1>(c) << " ";		
		return ss.str();
	}*/

	template<size_t N> /* N columns */
	static inline bool is_valid(size_t row_index, std::array<component, N>& row)
	{
		//DBG
		//std::cout << to_string(row_index, row) << " = ";		
		bool not_has_generators = !(std::find_if(row.begin(), row.end(), is_generator) != row.end());
		auto non_empty = std::remove_if(row.begin(), row.end(), 
			[](component c) { return is_empty(c) || is_elevator(c); });
		auto boundary = std::partition(row.begin(), non_empty, is_generator);
		
		auto generators_begin = row.begin();
		auto generators_end = boundary;
		auto microchips_begin = boundary;
		auto microchips_end = non_empty;
		std::sort(generators_begin, generators_end); //generators
		std::sort(microchips_begin, microchips_end); //microchips
		
		std::vector<component> diff;
		if (std::distance(generators_begin, generators_end) > 0 && 
			std::distance(microchips_begin, microchips_end) > 0)
		{
			std::set_difference(microchips_begin, microchips_end, 
				generators_begin, generators_end, std::back_inserter(diff),
				[](component a, component b) {
					return std::get<0>(a) < std::get<0>(b); 
				});
		}

		bool result = not_has_generators || diff.size() == 0;
		//DBG
		//std::cout << std::boolalpha << result << "\n";
		return result;
	}

	template<size_t M, size_t N> /* M rows, N columns */
	static std::vector<component_table<M, N>> next(const component_table<M, N>& ct)
	{
		//get elevator floor
		auto es = ct.column(1);
		size_t e = std::distance(es.begin(), std::find_if(es.begin(), es.end(), is_elevator)) + 1;
		
		//select components in floor
		std::vector<size_t> in_floor;
		for (size_t n = 2; n <= N; ++n)
			if (!is_empty(ct.cell(e, n)))
				in_floor.push_back(n);
		size_t combo = std::pow(2, in_floor.size());
			
		//generate combinations and select candidates
		std::vector<component_table<M, N>> candidates;
		for (size_t m : {e - 1, e + 1})
		{
			if (1 <= m && m <= M)
			{
				for (size_t i = 1; i < combo; ++i)				
				{
					if (count_bits_on(i) <= 2)
					{
						size_t j = i, k = 0;
						component_table<M, N> candidate(ct);
						while (j > 0)						
						{
							if ((j & 0x1) == 0x1)
							{
								size_t col = in_floor[k];
								component c = ct.cell(e, col);
								candidate.set(m, col, c);
								candidate.set(e, col, Empty);
							}
							j >>= 1;
							++k;
						}
						candidate.set(m, 1, Elevator);
						candidate.set(e, 1, Empty);
						std::array<component, N> row = candidate.row(m);
						if (is_valid<N>(m, row))
							candidates.push_back(candidate);
					}
				}
			}
		}
		return candidates;
	}

	template<size_t M, size_t N>
	static inline std::vector<bool> fingerprint(const component_table<M, N>& current)
	{
		std::vector<bool> temp;
		std::transform(current.data().begin(), current.data().end(), std::back_inserter(temp),
			[](component c) { return !is_empty(c); });
		return temp;
	}

	static inline std::string to_string(std::vector<bool> bits)
	{
		std::stringstream ss;
		std::copy(bits.begin(), bits.end(), std::ostream_iterator<bool>(ss, ""));
		return ss.str();
	}

	template<size_t M, size_t N>
	static inline double score(const component_table<M, N>& ct)
	{
		size_t total = 0;
		for (size_t m = 1; m <= M; ++m)
		{
			size_t count = 0;
			for (size_t n = 2; n <= N; ++n)
				if (!is_empty(ct.cell(m, n)))
					++count;
			total += count * (M - m);
		}
		return total / 2.0;
	}

	template<typename T>
	struct state
	{
		state(const T& value, size_t depth, std::shared_ptr<state<T>> parent) :
			value(value),
			depth(depth),
			parent(parent) {}

		T value;		
		size_t depth;
		std::shared_ptr<state<T>> parent;
	};

	template<size_t M, size_t N>
	struct state_min
	{
		using state_type = state<component_table<M, N>>;
		using state_ptr = std::shared_ptr<state_type>;

		bool operator()(const state_ptr& lhs, const state_ptr& rhs) const
		{
			return score(lhs->value) > score(rhs->value);
		}
	};

	template<size_t M, size_t N>
	static size_t solving_with_priority_queue(const component_table<M, N>& start, const component_table<M, N>& goal)
	{
		using state_type = state<component_table<M, N>>;
		using state_ptr = std::shared_ptr<state_type>;
		std::priority_queue<state_ptr, std::vector<state_ptr>, state_min<M, N>> pq;
		std::set<std::vector<bool>> visited;
		visited.insert(fingerprint(start));
		pq.push(state_ptr(new state_type{ start, 0, nullptr }));
		while (!pq.empty())
		{
			auto best = pq.top();
			pq.pop();
			//DBG
			/*std::cout << "************************************\n";
			std::cout << "fingerprint = " << to_string(fingerprint(best->value))
				<< " depth = " << best->depth 
				<< " score = " << score(best->value) << "\n";
			std::cout << to_string(best->value);
			std::cout << "************************************\n";*/
			if (score(best->value) == 0.0)
			{
				std::cout << "************************************\n";
				std::cout << "found\n";
				std::cout << "************************************\n";
				std::vector<std::string> path;
				while (best != nullptr)
				{
					path.push_back(to_string(best->value));
					best = best->parent;
				}
				std::copy(path.rbegin(), path.rend(), std::ostream_iterator<std::string>(std::cout, "\n"));				
				std::cout << "path size = " << path.size() << "\n";
				std::cout << "visited size = " << visited.size() << "\n";
				std::cout << "************************************\n";
				return path.size() - 1;
			}

			auto moves = next<M, N>(best->value);
			//DBG
			/*std::cout << "------------------------------------\n";
			for (auto& m : moves)
				std::cout << to_string(m) << "=======================================\n";*/
			for (auto& x : moves)
			{
				auto f = fingerprint(x);
				if (visited.find(f) == visited.end())
				{
					visited.insert(f);					
					pq.push(state_ptr(new state_type{ x, best->depth + 1, best }));
				}
			}
		}

		std::cout << "************************************\n";
		std::cout << "not found\n";
		std::cout << "************************************\n";
		return -1;
	}

	template<size_t M, size_t N>
	static size_t solving_with_queue(const component_table<M, N>& start, const component_table<M, N>& goal)
	{
		using state_type = state<component_table<M, N>>;
		using state_ptr = std::shared_ptr<state_type>;
		std::queue<state_ptr> q;
		std::set<std::vector<bool>> visited;
		visited.insert(fingerprint(start));
		q.push(state_ptr(new state_type{ start, 0, nullptr }));
		while (!q.empty())
		{
			auto best = q.front();
			q.pop();
			//DBG
			/*std::cout << "************************************\n";
			std::cout << "fingerprint = " << to_string(fingerprint(best->value))
				<< " depth = " << best->depth << "\n";
			std::cout << to_string(best->value);
			std::cout << "************************************\n";*/
			if (fingerprint(best->value) == fingerprint(goal))
			{
				std::cout << "************************************\n";
				std::cout << "found\n";
				std::cout << "************************************\n";
				std::vector<std::string> path;				
				while (best != nullptr)
				{
					path.push_back(to_string(best->value));
					best = best->parent;
				}
				std::copy(path.rbegin(), path.rend(), std::ostream_iterator<std::string>(std::cout, "\n"));
				std::cout << "path size = " << path.size() << "\n";
				std::cout << "visited size = " << visited.size() << "\n";
				std::cout << "************************************\n";
				return path.size() - 1;
			}			

			auto moves = next(best->value);
			//DBG
			/*std::cout << "------------------------------------\n";
			for (auto& m : moves)
				std::cout << to_string(m) << "=======================================\n";*/
			for (auto& x : moves)
			{
				auto f = fingerprint(x);
				if (visited.find(f) == visited.end())
				{
					visited.insert(f);
					q.push(state_ptr(new state_type{ x, best->depth + 1, best }));
				}
			}	
		}
		
		std::cout << "************************************\n";
		std::cout << "not found\n";
		std::cout << "************************************\n";
		return -1;
	}

	static component_table<4, 3> start0()
	{
		component_table<4, 3> t(Empty);
		t.set(1, 1, Elevator);
		t.set(1, 2, make_microchip('R'));
		t.set(3, 3, make_generator('R'));
		return t;
	}

	static component_table<4, 3> goal0()
	{
		component_table<4, 3> t(Empty);
		t.set(4, 1, Elevator);
		t.set(4, 2, make_microchip('R'));
		t.set(4, 3, make_generator('R'));
		return t;
	}

	static component_table<4, 5> start1()
	{
		component_table<4, 5> t(Empty);
		t.set(1, 1, Elevator);
		t.set(1, 3, make_microchip('H'));
		t.set(1, 5, make_microchip('L'));
		t.set(2, 2, make_generator('H'));
		t.set(3, 4, make_generator('L'));
		return t;
	}

	static component_table<4, 5> goal1()
	{
		component_table<4, 5> t(Empty);
		t.set(4, 1, Elevator);
		t.set(4, 3, make_microchip('H'));
		t.set(4, 5, make_microchip('L'));
		t.set(4, 2, make_generator('H'));
		t.set(4, 4, make_generator('L'));
		return t;
	}

	static component_table<4, 11> start2()
	{
		component_table<4, 11> t(Empty);
		t.set(1,  1, Elevator);
		t.set(1,  2, make_generator('T'));
		t.set(1,  3, make_microchip('T'));
		t.set(1,  4, make_generator('P'));
		t.set(1,  5, make_generator('S'));
		t.set(2,  6, make_microchip('P'));
		t.set(2,  7, make_microchip('S'));
		t.set(3,  8, make_microchip('X'));
		t.set(3,  9, make_generator('X'));
		t.set(3, 10, make_microchip('R'));
		t.set(3, 11, make_generator('R'));
		return t;
	}

	static component_table<4, 11> goal2()
	{
		component_table<4, 11> t(Empty);
		t.set(4,  1, Elevator);
		t.set(4,  2, make_generator('T'));
		t.set(4,  3, make_microchip('T'));
		t.set(4,  4, make_generator('P'));
		t.set(4,  5, make_generator('S'));
		t.set(4,  6, make_microchip('P'));
		t.set(4,  7, make_microchip('S'));
		t.set(4,  8, make_microchip('X'));
		t.set(4,  9, make_generator('X'));
		t.set(4, 10, make_microchip('R'));
		t.set(4, 11, make_generator('R'));
		return t;
	}

	static component_table<4, 11> start3()
	{
		component_table<4, 11> t(Empty);
		t.set(1,  1, Elevator);
		t.set(1,  2, make_generator('S'));
		t.set(1,  3, make_microchip('S'));
		t.set(1,  4, make_generator('P'));
		t.set(1,  5, make_microchip('P'));
		t.set(2,  6, make_generator('T'));
		t.set(2,  7, make_generator('R'));
		t.set(2,  8, make_microchip('R'));
		t.set(2,  9, make_generator('C'));
		t.set(2, 10, make_microchip('C'));
		t.set(3, 11, make_microchip('T'));
		return t;
	}

	static component_table<4, 11> goal3()
	{
		component_table<4, 11> t(Empty);
		t.set(4, 1, Elevator);
		t.set(4, 2, make_generator('S'));
		t.set(4, 3, make_microchip('S'));
		t.set(4, 4, make_generator('P'));
		t.set(4, 5, make_microchip('P'));
		t.set(4, 6, make_generator('T'));
		t.set(4, 7, make_generator('R'));
		t.set(4, 8, make_microchip('R'));
		t.set(4, 9, make_generator('C'));
		t.set(4, 10, make_microchip('C'));
		t.set(4, 11, make_microchip('T'));
		return t;
	}

	void day_11_part_1()
	{
		size_t path_size;
		//path_size = solving_with_queue(start0(), goal0());
		//path_size = solving_with_queue(start1(), goal1());
		
		//path_size = solving_with_priority_queue(start0(), goal0());
		//path_size = solving_with_priority_queue(start1(), goal1());
		//path_size = solving_with_priority_queue(start2(), goal2());
		path_size = solving_with_priority_queue(start3(), goal3());

		std::cout << "What is the minimum number of steps required to bring all of the objects to the fourth floor? ";
		std::cout << path_size << "\n";
	}

	static component_table<4, 15> start4()
	{
		component_table<4, 15> t(Empty);
		t.set(1,  1, Elevator);
		t.set(1,  2, make_generator('S'));
		t.set(1,  3, make_microchip('S'));
		t.set(1,  4, make_generator('E'));
		t.set(1,  5, make_microchip('E'));
		t.set(1,  6, make_generator('D'));
		t.set(1,  7, make_microchip('D'));
		t.set(1,  8, make_generator('P'));
		t.set(1,  9, make_microchip('P'));
		t.set(2, 10, make_generator('T'));
		t.set(2, 11, make_generator('R'));
		t.set(2, 12, make_microchip('R'));
		t.set(2, 13, make_generator('C'));
		t.set(2, 14, make_microchip('C'));
		t.set(3, 15, make_microchip('T'));
		return t;
	}

	static component_table<4, 15> goal4()
	{
		component_table<4, 15> t(Empty);
		t.set(4, 1, Elevator);
		t.set(4, 2, make_generator('S'));
		t.set(4, 3, make_microchip('S'));
		t.set(4, 4, make_generator('E'));
		t.set(4, 5, make_microchip('E'));
		t.set(4, 6, make_generator('D'));
		t.set(4, 7, make_microchip('D'));
		t.set(4, 8, make_generator('P'));
		t.set(4, 9, make_microchip('P'));
		t.set(4, 10, make_generator('T'));
		t.set(4, 11, make_generator('R'));
		t.set(4, 12, make_microchip('R'));
		t.set(4, 13, make_generator('C'));
		t.set(4, 14, make_microchip('C'));
		t.set(4, 15, make_microchip('T'));
		return t;
	}

	void day_11_part_2()
	{
		size_t path_size = solving_with_priority_queue(start4(), goal4());
		std::cout << "What is the minimum number of steps required to bring all of the objects, including these four new ones, to the fourth floor? ";
		std::cout << path_size << "\n";
	}
}

int day_11_main(unsigned int part)
{
	if (part == 1)
		Day11::day_11_part_1(); //Your puzzle answer was 37.
	else
		Day11::day_11_part_2(); //Your puzzle answer was 61.
	return 0;
}

#endif /* DAY_11_HPP */