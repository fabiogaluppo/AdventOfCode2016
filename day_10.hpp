#ifndef DAY_10_HPP
#define DAY_10_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <tuple>
#include <stdexcept>
#include <vector>
#include <memory>
#include <utility>
#include <algorithm>
#include <typeinfo>

namespace Day10
{
	enum Tag { Bot, Value, Output };

	static inline Tag translate_tag(const char* tag)
	{
		if (std::strcmp("bot", tag) == 0)
			return Bot;
		if (std::strcmp("value", tag) == 0)
			return Value;
		if (std::strcmp("output", tag) == 0)
			return Output;
		throw std::runtime_error("invalid tag");
	}

	static inline std::tuple<char*, Tag, size_t> read_tag_value(char* p, const char* pattern)
	{
		const char* tag = p = std::strtok(p, pattern);
		p = std::strtok(nullptr, pattern);
		return std::make_tuple(p, translate_tag(tag), std::atoi(p));
	}

	static inline char* skip_n(char* p, const char* pattern, size_t n)
	{
		while (n-- > 0) p = std::strtok(nullptr, pattern);
		return p;
	}

	struct node
	{
		node(size_t index) : index(index) {}			
		virtual ~node() {}

		size_t index;
		bool visited = false;
	};

	struct output_node final : public node
	{
		output_node(size_t index) : 
			node(index), 
			value(-1) {}

		size_t value;

		void visit(size_t val)
		{
			value = val;
			visited = true;
		}
	};

	static inline bool is_output(node* x)
	{ 
		return typeid(*x) == typeid(output_node); 
	}

	static inline bool is_output(std::unique_ptr<node>& x)
	{ 
		return is_output(x.get());
	}

	static inline output_node* cast_output(node* x)
	{
		if (is_output(x))
			return static_cast<output_node*>(x);
		throw std::runtime_error("node is incompatible");
	}

	static inline output_node* cast_output(std::unique_ptr<node>& x)
	{
		return cast_output(x.get());
	}

	struct bot_node final : public node
	{
		bot_node(size_t index) :
			node(index),			
			low(-1),
			high(-1),
			low_edge(nullptr),
			high_edge(nullptr) {}

		size_t low;
		size_t high;

		std::unique_ptr<node> low_edge;
		std::unique_ptr<node> high_edge;

		void visit(size_t value, std::vector<std::tuple<size_t, size_t>>& visits)
		{
			if (low == -1)
			{
				low = value;
			}
			else if (high == -1)
			{
				high = value;
			}
			else
			{
				throw std::runtime_error("unexpected");
			}

			if (low != -1 && high != -1)
			{
				if (low > high)
					std::swap(low, high);
				visits[index] = std::make_tuple(low, high);
				visited = true;
			}
		}
	};

	static inline bool is_bot(node* x)
	{ 
		return typeid(*x) == typeid(bot_node);
	}

	static inline bool is_bot(std::unique_ptr<node>& x)
	{ 
		return is_bot(x.get()); 
	}

	static inline bot_node* cast_bot(node* x)
	{
		if (is_bot(x))
			return static_cast<bot_node*>(x);
		throw std::runtime_error("node is incompatible");
	}

	static inline bot_node* cast_bot(std::unique_ptr<node>& x)
	{
		return cast_bot(x.get());
	}

	struct dag /* M bots, N outputs */
	{
		dag(size_t M, size_t N) : bots(M), outputs(N), visits(M) {}

		std::vector<std::unique_ptr<bot_node>> bots;
		std::vector<std::unique_ptr<output_node>> outputs;
		std::vector<std::tuple<size_t, size_t>> visits;

		inline bot_node* get_or_add_bot(size_t index)
		{
			if (!bots[index])
			{
				bots[index].reset(new bot_node(index));
			}
			return bots[index].get();
		}

		void link(std::unique_ptr<node>& out_node, Tag tag, size_t index)
		{
			if (tag == Output)
			{
				outputs[index].reset(new output_node(index));
				out_node.reset(outputs[index].get());
			}
			else if (tag == Bot)
			{
				bot_node* out_bot = get_or_add_bot(index);
				out_node.reset(out_bot);
			}
			else
			{
				throw std::runtime_error("unexpected");
			}
		}
	};

	static void depth_first_propagation(bot_node* bot_src, std::vector<std::tuple<size_t, size_t>>& visits);

	static void propagate(size_t new_value, std::unique_ptr<node>& x, std::vector<std::tuple<size_t, size_t>>& visits)
	{
		if (is_output(x))
		{
			output_node* output = cast_output(x);
			output->visit(new_value);
		}
		else //is_bot(x)
		{
			bot_node* bot_dst = cast_bot(x);			
			if (!bot_dst->visited)			
				bot_dst->visit(new_value, visits);			
			depth_first_propagation(bot_dst, visits);
		}
		x.release();
	}

	static inline void depth_first_propagation(bot_node* bot_src, std::vector<std::tuple<size_t, size_t>>& visits)
	{
		if (bot_src->visited)
		{
			propagate(bot_src->low, bot_src->low_edge, visits);
			propagate(bot_src->high, bot_src->high_edge, visits);
		}
	}

	static void build_dag(dag& g)
	{
		std::string input;
		while (std::getline(std::cin, input))
		{
			char* p = const_cast<char*>(input.c_str());
			const char* pattern = " ";
			size_t value;
			Tag tag;
			std::tie(p, tag, value) = read_tag_value(p, pattern);
			switch (tag)
			{
			case Bot:
			{
				bot_node* bot = g.get_or_add_bot(value);
				p = skip_n(nullptr, pattern, 3);
				size_t index;
				//low
				std::tie(p, tag, index) = read_tag_value(nullptr, pattern);
				g.link(bot->low_edge, tag, index);
				p = skip_n(nullptr, pattern, 3);
				//high
				std::tie(p, tag, index) = read_tag_value(nullptr, pattern);
				g.link(bot->high_edge, tag, index);
			}
			break;
			case Value:
			{
				p = skip_n(nullptr, pattern, 2);
				size_t index;
				std::tie(p, tag, index) = read_tag_value(nullptr, pattern);
				bot_node* bot = g.get_or_add_bot(index);
				bot->visit(value, g.visits);
			}
			break;
			case Output:
			default:
				throw std::runtime_error("invalid format");
			}
		}
	}

	static inline void run_dag(dag& g)
	{
		auto bound = std::partition(g.bots.begin(), g.bots.end(),
			[](const std::unique_ptr<bot_node>& x) { return x ? x->visited : false; });

		//depth-search propagation
		std::for_each(g.bots.begin(), bound,
			[&](std::unique_ptr<bot_node>& x) { depth_first_propagation(x.get(), g.visits); });
	}

	static inline void print_dag(const dag& g)
	{
		//print visits
		for (size_t i = 0; i < g.visits.size(); ++i)
		{
			size_t lvalue = std::get<0>(g.visits[i]);
			size_t rvalue = std::get<1>(g.visits[i]);
			std::cout << i << ": " << lvalue << ", " << rvalue << "\n";
		}

		//print sinks
		for (const std::unique_ptr<output_node>& x : g.outputs)
			if (x) std::cout << x->index << ": " << x->value << "\n";
	}

	void day_10_part_1(size_t M = 210, size_t N = 21) /* M bots, N outputs */
	{
		dag g(M, N);

		build_dag(g);
		run_dag(g);
		//print_dag(g);

		auto iter = std::find(g.visits.begin(), g.visits.end(), std::make_tuple(17, 61));
		std::cout << "What is the number of the bot that is responsible for comparing value-61 microchips with value-17 microchips? ";
		if (iter != g.visits.end())
			std::cout << (iter - g.visits.begin()) << "\n";
		else
			std::cout << "not found\n";

		return;
	}

	void day_10_part_2(size_t M = 210, size_t N = 21) /* M bots, N outputs */
	{
		dag g(M, N);

		build_dag(g);
		run_dag(g);

		std::cout << "What do you get if you multiply together the values of one chip in each of outputs 0, 1, and 2? ";
		std::cout << (g.outputs[0]->value * g.outputs[1]->value * g.outputs[2]->value) << "\n";

		return;
	}
};

int day_10_main(unsigned int part)
{
	if (part == 1)
		Day10::day_10_part_1(); //Your puzzle answer was 73.
	else
		Day10::day_10_part_2(); //Your puzzle answer was 3965.
	return 0;
}

#endif //DAY_10_HPP