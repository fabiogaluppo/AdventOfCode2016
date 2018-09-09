#ifndef DAY_2_HPP
#define DAY_2_HPP

#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <tuple>

namespace Day2
{
	static short KEYPAD[3][3] = { { 1, 2, 3 },
								  { 4, 5, 6 },
								  { 7, 8, 9 } };
	const short KEYPAD_LIMIT_LEFT = -1, KEYPAD_LIMIT_RIGHT = 3;
	const short KEYPAD_LIMIT_UP = -1, KEYPAD_LIMIT_DOWN = 3;

	static short KEYPAD2[5][5] = { { 0x0, 0x0, 0x1, 0x0, 0x0 },
								   { 0x0, 0x2, 0x3, 0x4, 0x0 },
								   { 0x5, 0x6, 0x7, 0x8, 0x9 },
								   { 0x0, 0xA, 0xB, 0xC, 0x0 },
								   { 0x0, 0x0, 0xD, 0x0, 0x0 } };
	const short KEYPAD2_LIMIT_LEFT = -1, KEYPAD2_LIMIT_RIGHT = 5;
	const short KEYPAD2_LIMIT_UP = -1, KEYPAD2_LIMIT_DOWN = 5;

	enum Move : short { U, D, L, R };

	using Position2D = std::tuple<short, short>;

	static inline Position2D KEYPAD_move(Position2D pos, Move m)
	{
		switch (m)
		{
		case U:
			--std::get<1>(pos);
			if (std::get<1>(pos) == KEYPAD_LIMIT_UP)
				++std::get<1>(pos);
			break;
		case D:
			++std::get<1>(pos);
			if (std::get<1>(pos) == KEYPAD_LIMIT_DOWN)
				--std::get<1>(pos);
			break;
		case L:
			--std::get<0>(pos);
			if (std::get<0>(pos) == KEYPAD_LIMIT_LEFT)
				++std::get<0>(pos);
			break;
		case R:
			++std::get<0>(pos);
			if (std::get<0>(pos) == KEYPAD_LIMIT_RIGHT)
				--std::get<0>(pos);
			break;
		}
		return pos;
	}

	static inline Position2D KEYPAD2_move(Position2D pos, Move m)
	{
		short x, y;
		std::tie(x, y) = pos;
		switch (m)
		{
		case U:
			--y;
			if (y == KEYPAD2_LIMIT_UP || KEYPAD2[y][x] == 0x0)
				++y;
			break;
		case D:
			++y;
			if (y == KEYPAD2_LIMIT_DOWN || KEYPAD2[y][x] == 0x0)
				--y;
			break;
		case L:
			--x;
			if (x == KEYPAD2_LIMIT_LEFT || KEYPAD2[y][x] == 0x0)
				++x;
			break;
		case R:
			++x;
			if (x == KEYPAD2_LIMIT_RIGHT || KEYPAD2[y][x] == 0x0)
				--x;
			break;
		}
		return std::make_tuple(x, y);
	}

	static inline Move translate(char m)
	{
		switch (m)
		{
		case 'U': return U;
		case 'D': return D;
		case 'L': return L;
		case 'R': return R;
		default: throw std::runtime_error("invalid move");
		}
	}

	void day_2_part_1()
	{
		Position2D state = { 1, 1 };
		std::string str;
		std::cout << "What is the bathroom code? ";
		while (std::getline(std::cin, str))
		{
			for (char m : str)
				state = KEYPAD_move(state, translate(m));
			short x, y;
			std::tie(x, y) = state;
			std::cout << KEYPAD[y][x];
		}
		std::cout << "\n";
		return;
	}

	void day_2_part_2()
	{
		Position2D state = { 0, 2 };
		std::string str;
		std::cout << "what is the correct bathroom code? ";
		while (std::getline(std::cin, str))
		{
			for (char m : str)
				state = KEYPAD2_move(state, translate(m));
			short x, y;
			std::tie(x, y) = state;
			std::cout << std::hex << std::uppercase << KEYPAD2[y][x];
		}
		std::cout << "\n";
		return;
	}
};

int day_2_main(unsigned int part)
{
	if (part == 1)
		Day2::day_2_part_1(); //Your puzzle answer was 82958.
	else
		Day2::day_2_part_2(); //Your puzzle answer was B3DB8.
	return 0;
}

#endif //DAY_2_HPP