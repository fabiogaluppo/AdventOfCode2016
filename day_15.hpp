#ifndef DAY_15_HPP
#define DAY_15_HPP

#include <vector>

//#define TRACE_BACKTRACKING

#ifdef TRACE_BACKTRACKING
#include <iostream>
#endif //TRACE_BACKTRACKING

namespace Day15
{
	//Solution in F# using "intersection and guess n" strategy
	/*
	let findFirstTime n layers =
		let nextPos size t0 checkpoint =
			seq{
			let mutable x = size - t0 + checkpoint
			if (x < 0) then x <- x + size
			while true do
			yield x
			x <- x + size }
		let xs = layers |> List.map (fun (size, t0, checkpoint) -> nextPos size t0 checkpoint |> Seq.take n |> Set.ofSeq)
		let h, t = xs |> Seq.head, xs |> Seq.tail
		t |> Seq.fold (fun acc x -> acc |> Set.intersect x) h |> Seq.map(fun x -> x - 1)

	> findFirstTime 100000 [(17, 5, 0); (19, 8, -1); (7, 1, -2); (13, 7, -3); (5, 1, -4); (3, 0, -5)];;
	val it : seq<int> = seq [16824]
	> findFirstTime 1190000 [(17, 5, 0); (19, 8, -1); (7, 1, -2); (13, 7, -3); (5, 1, -4); (3, 0, -5); (11, 0, -6)];;
	val it : seq<int> = seq [3543984]
	*/

	struct gen_next_pos
	{
		gen_next_pos(int size, int t0, int checkpoint) :
			size(size)
		{
			int temp = size - t0 + checkpoint;
			if (temp < 0) temp += size;
			val = temp;
		}

		size_t current() const { return val; }

		void next() { val += size; }

	private:
		size_t val, size;
	};

	using layers_type = std::vector<gen_next_pos>;

	static bool find_first_time(layers_type& layers, size_t idx, size_t value_to_match)
	{
		//backtracking strategy
		auto& gen = layers[idx];
		while (true) //infinity or constrained
		{
			if (value_to_match != -1)
			{
#ifdef TRACE_BACKTRACKING
				std::cout << value_to_match << " "; //DBG
#endif //TRACE_BACKTRACKING

				bool found = false;
				while (gen.current() < value_to_match)
					gen.next();
				if (gen.current() != value_to_match)
				{
#ifdef TRACE_BACKTRACKING
					std::cout << gen.current() << "\n"; //DBG
#endif //TRACE_BACKTRACKING

					return false;
				}
				else if (idx + 1 == layers.size())
				{
#ifdef TRACE_BACKTRACKING
					std::cout << gen.current() << "\n"; //DBG
#endif //TRACE_BACKTRACKING

					return true;
				}
			}

			if (find_first_time(layers, idx + 1, gen.current()))
				return true;
			gen.next();
		}
		return false;
	}

	/*
	State space from the first problem
	#1[12; 29; 46; 63; 80;  97; 114; 131; 148; ...]
	#2[10; 29; 48; 67; 86; 105; 124; 143; 162; ...]
	#3[4; 11; 18; 25; 32;  39;  46;  53;  60; ...]
	#4[3; 16; 29; 42; 55;  68;  81;  94; 107; ...]
	#5[0;  5; 10; 15; 20;  25;  30;  35;  40; ...]
	#6[1;  4;  7; 10; 13;  16;  19;  22;  25; ...]
	Backtracking trace:
	12 29
	29 29 32
	...
	352 352 354
	...
	675 675 676
	...
	998 998 998 1004
	...
	...
	2290 2290 2293
	...
	...
	3259 3259 3259 3266
	...
	...
	16825 16825 16825 16825 16825 16825
	*/
	void day_15_part_1()
	{
		layers_type layers {
			gen_next_pos(17, 5,  0),
			gen_next_pos(19, 8, -1),
			gen_next_pos(7, 1, -2),
			gen_next_pos(13, 7, -3),
			gen_next_pos(5, 1, -4),
			gen_next_pos(3, 0, -5),
		};

		if (find_first_time(layers, 0, -1))
		{
			size_t T = layers[0].current() - 1;
			std::cout << "What is the first time you can press the button to get a capsule? " << T << "\n";
		}
		else
		{
			std::cout << "Not found\n";
		}
	}

	void day_15_part_2()
	{
		layers_type layers {
			gen_next_pos(17, 5,  0),
			gen_next_pos(19, 8, -1),
			gen_next_pos(7, 1, -2),
			gen_next_pos(13, 7, -3),
			gen_next_pos(5, 1, -4),
			gen_next_pos(3, 0, -5),
			gen_next_pos(11, 0, -6)
		};

		if (find_first_time(layers, 0, -1))
		{
			size_t T = layers[0].current() - 1;
			std::cout << "What is the first time you can press the button to get another capsule? " << T << "\n";
		}
		else
		{
			std::cout << "Not found\n";
		}
	}
};

int day_15_main(unsigned int part)
{
	if (part == 1)
		Day15::day_15_part_1(); //Your puzzle answer was 16824.
	else
		Day15::day_15_part_2();	//Your puzzle answer was 3543984.
	return 0;
}

#endif /* DAY_15_HPP */