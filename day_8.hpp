#ifndef DAY_8_HPP
#define DAY_8_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <bitset>
#include <sstream>
#include <algorithm>

namespace Day8
{
	template<size_t Width, size_t Height>
	struct Display
	{
		static const size_t width = Width;
		static const size_t height = Height;
		
		using Transposed = Display<height, width>;

		using Pixels = std::bitset<width>;

		Display() : pixels{ 0 } {}
		Pixels pixels[height];
	};

	template<size_t Width, size_t Height>
	static inline std::ostream& operator<<(std::ostream& os, const Display<Width, Height>& display)
	{
		for (size_t i = 0; i < display.height; ++i)
			os << display.pixels[i] << "\n";
		return os;
	}

	template<size_t Width, size_t Height>
	static inline std::string to_readable_string(const Display<Width, Height>& display)
	{
		std::stringstream ss;
		for (size_t i = 0; i < display.height; ++i)
		{
			std::stringstream temp;
			temp << display.pixels[i];
			std::string str = temp.str();
			std::replace(str.begin(), str.end(), '1', '#');
			std::replace(str.begin(), str.end(), '0', ' ');
			ss << str << "\n";
		}
		return ss.str();
	}
	
	template<typename TPixels>
	static inline TPixels rotate_right(TPixels pixels, size_t n)
	{
		constexpr size_t N = pixels.size();
		size_t r = n % N;
		TPixels right_shift = pixels >> r;
		TPixels left_shift = pixels << (N - r);
		return right_shift | left_shift;
	}
	
	template<size_t Width, size_t Height>
	static inline typename Display<Width, Height>::Transposed transpose(const Display<Width, Height>& display)
	{
		typename Display<Width, Height>::Transposed temp;
		for (size_t i = 0; i < display.height; ++i)
			for (size_t j = 0; j < display.width; ++j)
				temp.pixels[display.width - j - 1][display.height - i - 1] = display.pixels[i][j];
		return temp;
	}

	template<size_t Width, size_t Height>
	static inline void rect(Display<Width, Height>& display, size_t w, size_t h)
	{
		for (size_t i = 0; i < h; ++i)
			for (size_t j = display.width - w; j <= display.width - 1; ++j)
				display.pixels[i].set(j);
	}

	template<size_t Width, size_t Height>
	static inline void rotate_row(Display<Width, Height>& display, size_t y, size_t by)
	{
		display.pixels[y] = rotate_right(display.pixels[y], by);
	}

	template<size_t Width, size_t Height>
	static inline void rotate_column(Display<Width, Height>& display, size_t x, size_t by)
	{
		typename Display<Width, Height>::Transposed transposed_display = transpose(display);
		transposed_display.pixels[x] = rotate_right(transposed_display.pixels[x], by);
		display = transpose(transposed_display);
	}

	template<size_t Width, size_t Height>
	static inline size_t count_bits_on(const Display<Width, Height>& display)
	{
		size_t total = 0;
		for (size_t i = 0; i < display.height; ++i)
			total += display.pixels[i].count();
		return total;
	}

	template<size_t Width, size_t Height>
	static void execute_command(Display<Width, Height>& display, const std::string& input)
	{
		char* cmd = const_cast<char*>(input.c_str());
		const char* pattern = " ";
		char* p = std::strtok(cmd, pattern);
		if (p != nullptr)
		{
			if (std::strcmp(p, "rect") == 0)
			{
				p = std::strtok(nullptr, "x");
				size_t w = std::atoi(p);
				p = std::strtok(nullptr, "");
				size_t h = std::atoi(p);
				rect(display, w, h);
				return;
			}
			else if (std::strcmp(p, "rotate") == 0)
			{
				p = std::strtok(nullptr, pattern);
				if (std::strcmp(p, "column") == 0)
				{
					p = std::strtok(nullptr, "x= ");
					size_t x = std::atoi(p);
					p = std::strtok(nullptr, "by");
					size_t by = std::atoi(p);
					rotate_column(display, x, by);
					return;
				}
				else if (std::strcmp(p, "row") == 0)
				{
					p = std::strtok(nullptr, "y= ");
					size_t y = std::atoi(p);
					p = std::strtok(nullptr, "by");
					size_t by = std::atoi(p);
					rotate_row(display, y, by);
					return;
				}
			}
			//else ignore command
		}
	}

	static void test_1()
	{
		Display<7, 3> display;
		rect(display, 3, 2);
		std::cout << display << "\n";
		rotate_column(display, 1, 1);
		std::cout << display << "\n";
		rotate_row(display, 0, 4);
		std::cout << display << "\n";
		rotate_column(display, 1, 1);
		std::cout << display << "\n";
		std::cout << "# of bits on = " << count_bits_on(display) << "\n";
	}

	static void test_2()
	{
		Display<7, 3> display;
		rect(display, 3, 2);
		std::cout << display << "\n";
		rotate_column(display, 1, 1);
		std::cout << display << "\n";
		rotate_row(display, 0, 4);
		std::cout << display << "\n";
		rotate_column(display, 1, 1);
		std::cout << display << "\n";
		rect(display, 3, 3);
		std::cout << display << "\n";
		std::cout << "# of bits on = " << count_bits_on(display) << "\n";
	}

	static void test_3()
	{
		Display<7, 3> display;
		execute_command(display, "rect 3x2");
		std::cout << display << "\n";
		execute_command(display, "rotate column x=1 by 1");
		std::cout << display << "\n";
		execute_command(display, "rotate row y=0 by 4");
		std::cout << display << "\n";
		execute_command(display, "rotate column x = 1 by 1");
		std::cout << display << "\n";
	}
	
	void day_8_part_1()
	{
		//test_1();
		//test_2();
		//test_3();
		
		Display<50, 6> display;
		std::string input;
		while (std::getline(std::cin, input))
		{
			execute_command(display, input);
			//std::cout << display << "\n";
		}
		std::cout << "How many pixels should be lit? " << count_bits_on(display) << "\n";
		return;
	}

	void day_8_part_2()
	{
		Display<50, 6> display;
		std::string input;
		while (std::getline(std::cin, input))
			execute_command(display, input);
		std::cout << "What code is the screen trying to display?\n";
		std::cout << to_readable_string(display) << "\n";
		return;
	}
}

int day_8_main(unsigned int part)
{
	if (part == 1)
		Day8::day_8_part_1(); //Your puzzle answer was 106.
	else
		Day8::day_8_part_2(); //Your puzzle answer was CFLELOYFCS.
	return 0;
}

#endif //DAY_8_HPP