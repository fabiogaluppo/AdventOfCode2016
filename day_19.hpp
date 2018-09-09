#ifndef DAY_19_HPP
#define DAY_19_HPP

#include <cmath>
#include <vector>
#include <numeric>
#include <iostream>

namespace Day19
{
	static inline size_t josephus(size_t n)
	{
		//1.3 in Concrete Mathematics, 2nd
		//Eq (1.9)
		size_t m = static_cast<size_t>(log(static_cast<double>(n)) / log(2.0));
		size_t l = n - static_cast<size_t>(pow(2.0, static_cast<double>(m)));
		return 2 * l + 1;
	}

	struct node
	{
		node(size_t elf) :
			elf(elf), next(nullptr) {}
		size_t elf;
		node* next;
	};

	void day_19_part_1()
	{
		size_t n;
		std::cin >> n;
		std::cout << "Which Elf gets all the presents? " << josephus(n) << "\n";
	}

	static inline size_t brute_force_part_2(size_t n)
	{
		std::vector<node> elfs;
		for (size_t i = 0; i < n; ++i)
			elfs.push_back(node(i + 1));
		for (size_t i = 0; i < n - 1; ++i)
			elfs[i].next = &elfs[i + 1];
		elfs[n - 1].next = &elfs[0];

		node* current = &elfs[0];
		while (n > 2)
		{
			//advance
			node* across_previous = current;
			for (size_t i = 0, mid = (n / 2) - 1; i < mid; ++i)
				across_previous = across_previous->next;
			node* across = across_previous->next;

			across_previous->next = across->next;
			across->next = nullptr;

			current = current->next;
			--n;
		}
		return current->elf;
	}

	//DBG
	/*static inline void print_table(size_t first, size_t last)
	{
		if (first < last)
		{
			size_t W = 1 + static_cast<size_t>(log10(last));

			for (size_t i = first; i <= last; ++i)
			{
				std::cout.width(W);
				std::cout << std::right << i << " ";
			}

			std::cout << "\n";
			for (size_t i = first; i <= last; ++i)
			{
				std::cout.width(W);
				std::cout << std::right << brute_force_part_2(i) << " ";
			}
			std::cout << "\n";
		}
	}*/

	void day_19_part_2()
	{
		size_t n;
		std::cin >> n;

		//TODO: Needs improvement
		size_t elf = brute_force_part_2(n);
		std::cout << "Which Elf gets all the presents? " << elf << "\n";
	}
};

int day_19_main(unsigned int part)
{
	if (part == 1)
		Day19::day_19_part_1(); //Your puzzle answer was 1808357.
	else
		Day19::day_19_part_2(); //Your puzzle answer was 1407007.
	return 0;
}

#endif /* DAY_19_HPP */