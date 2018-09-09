//compile with clang/clang++:
//clang -O3 -c md5c.c -o ./bin/md5c.o
//clang++ -std=c++14 -O3 -Wno-write-strings ./bin/md5c.o program.cpp -o ./bin/AdventOfCode2016.exe

//compile with msvc:
//cl.exe /c /Fo.\bin\ /Ox md5c.c
//cl.exe /Fo.\bin\ /std:c++14 /EHsc /Ox .\bin\md5c.obj program.cpp /link /out:.\bin\AdventOfCode2016.exe'

//compile with gcc/g++:
//gcc -O3 -c md5c.c -o ./bin/md5c.o
//g++ -std=c++14 -O3 -Wno-write-strings ./bin/md5c.o program.cpp -o ./bin/AdventOfCode2016.exe

#include "day_1.hpp"
#include "day_2.hpp"
#include "day_3.hpp"
#include "day_4.hpp"
#include "day_5.hpp"
#include "day_6.hpp"
#include "day_7.hpp"
#include "day_8.hpp"
#include "day_9.hpp"
#include "day_10.hpp"
#include "day_11.hpp"
#include "day_12.hpp"
#include "day_13.hpp"
#include "day_14.hpp"
#include "day_15.hpp"
#include "day_16.hpp"
#include "day_17.hpp"
#include "day_18.hpp"
#include "day_19.hpp"
#include "day_20.hpp"
#include "day_21.hpp"
#include "day_22.hpp"
#include "day_23.hpp"
#include "day_24.hpp"
#include "day_25.hpp"

#include <clocale>
#include <cstdlib>
#include <iostream>

#include "getopt/getopt.h"

void usage(char* process_name)
{
	std::cerr << "usage: << " << process_name << " -d:N -p:N" << std::endl;
	std::cerr << "where" << std::endl;
	std::cerr << "  d: day from 1 to 25" << std::endl;
	std::cerr << "  p: part 1 or 2" << std::endl;
	std::exit(1);
}

void get_args(int& argc, char* argv[], unsigned int& day, unsigned int& part)
{
	int ch;
	std::setlocale(LC_CTYPE, "");
	while ((ch = getopt(argc, argv, "d:p:")) != -1)
	{
		switch (static_cast<char>(ch))
		{
		case 'd':
			++optarg;
			day = static_cast<int>(std::atol(optarg));
			break;
		case 'p':
			++optarg;
			part = static_cast<int>(std::atol(optarg));
			break;
		case '?':
			default:
			usage(argv[0]);
		}
	}

	if (day && (!(1 <= day && day <= 25) || !(1 <= part && part <= 2)))
		usage(argv[0]);
	
	argv += optind;
	argc -= optind;
}

#include "md5/md5.hpp"

static void md5_test(const std::string& text)
{
	//string version
	std::cout << md5_string(text) << "\n";

	//digest version
	unsigned char digest[16];
	md5_digest(text, digest);
	for (size_t i = 0; i < 16; ++i)
		std::printf("%02x ", digest[i]);
	std::cout << "\n";
}

static int run_tests()
{
	md5_test(""); //d41d8cd98f00b204e9800998ecf8427e
	md5_test("The quick brown fox jumps over the lazy dog"); //9e107d9d372bb6826bd81d3542a419d6
	md5_test("The quick brown fox jumps over the lazy dog."); //e4d909c290d0fb1ca068ffaddf22cbd0
	md5_test("njfxhljp"); //2b63f63658ec7f39df9877eea56d9d42
	return 0;
}

int main(int argc, char* argv[])
{
	unsigned int day = 0, part = 0;
	get_args(argc, argv, day, part);
	switch (day)
	{
		case  1: return  day_1_main(part);
		case  2: return  day_2_main(part);
		case  3: return  day_3_main(part);
		case  4: return  day_4_main(part);
		case  5: return  day_5_main(part);
		case  6: return  day_6_main(part);
		case  7: return  day_7_main(part);
		case  8: return  day_8_main(part);
		case  9: return  day_9_main(part);
		case 10: return day_10_main(part);
		case 11: return day_11_main(part);
		case 12: return day_12_main(part);
		case 13: return day_13_main(part);
		case 14: return day_14_main(part);
		case 15: return day_15_main(part);
		case 16: return day_16_main(part);
		case 17: return day_17_main(part);
		case 18: return day_18_main(part);
		case 19: return day_19_main(part);
		case 20: return day_20_main(part);
		case 21: return day_21_main(part);
		case 22: return day_22_main(part);
		case 23: return day_23_main(part);
		case 24: return day_24_main(part);
		case 25: return day_25_main(part);
		case  0: return run_tests();
		default: usage(argv[0]);
	}
	return 2;
}