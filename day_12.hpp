#ifndef DAY_12_HPP
#define DAY_12_HPP

#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <memory>

namespace Day12
{
	enum Opcode { Cpy, Inc, Dec, Jnz };

	struct instruction
	{
		instruction(int& pc) : pc(pc) {}
		virtual ~instruction() {}
		virtual void exec() = 0;
		int& pc;
	};

	template<typename SourceOperand>
	struct cpy_instruction final : public instruction
	{
		cpy_instruction(SourceOperand s, int& d, int& pc)
			: instruction(pc), s(s), d(d) {}
		void exec() override
		{
			d = s;
			++pc;
		}
	private:
		SourceOperand s;
		int& d;
	};

	struct inc_instruction final : public instruction
	{
		inc_instruction(int& d, int& pc) 
			: instruction(pc), d(d) {}
		void exec() override
		{
			++d;
			++pc;
		}
	private:
		int& d;
	};

	struct dec_instruction final : public instruction
	{
		dec_instruction(int& d, int& pc) 
			: instruction(pc), d(d) {}
		void exec() override
		{
			--d;
			++pc;
		}
	private:
		int& d;
	};

	template<typename SourceOperand>
	struct jnz_instruction final : public instruction
	{
		jnz_instruction(SourceOperand s, int d, int& pc)
			: instruction(pc), s(s), d(d) {}
		void exec() override
		{
			if (s != 0)
				pc += d;
			else
				++pc;
		}
	private:
		SourceOperand s;
		int d;
	};

	static inline Opcode translate_opcode(const char* opcode)
	{
		if (std::strcmp("cpy", opcode) == 0)
			return Cpy;
		if (std::strcmp("inc", opcode) == 0)
			return Inc;
		if (std::strcmp("dec", opcode) == 0)
			return Dec;
		if (std::strcmp("jnz", opcode) == 0)
			return Jnz;
		throw std::runtime_error("invalid opcode");
	}

	template<typename T>
	struct by_val final
	{
		by_val(T value) : value(value) {}
		operator T() { return value; }
		T value;
	};

	template<typename T>
	struct by_ref final
	{
		by_ref(T& value) : value(value) {}
		operator T&() { return value; }
		T& value;
	};

	static inline std::unique_ptr<instruction> make_cpy(by_val<int> s, int& d, int& pc)
	{
		return std::unique_ptr<instruction>(new cpy_instruction<int>(s, d, pc));
	}

	static inline std::unique_ptr<instruction> make_cpy(by_ref<int> s, int& d, int& pc)
	{
		return std::unique_ptr<instruction>(new cpy_instruction<int&>(s, d, pc));
	}

	static inline std::unique_ptr<instruction> make_inc(int& d, int& pc)
	{
		return std::unique_ptr<instruction>(new inc_instruction(d, pc));
	}

	static inline std::unique_ptr<instruction> make_dec(int& d, int& pc)
	{
		return std::unique_ptr<instruction>(new dec_instruction(d, pc));
	}

	static inline std::unique_ptr<instruction> make_jnz(by_val<int> s, int d, int& pc)
	{
		return std::unique_ptr<instruction>(new jnz_instruction<int>(s, d, pc));
	}

	static inline std::unique_ptr<instruction> make_jnz(by_ref<int> s, int d, int& pc)
	{
		return std::unique_ptr<instruction>(new jnz_instruction<int&>(s, d, pc));
	}

	static inline bool is_register(char reg)
	{
		switch (reg)
		{
		case 'a':
		case 'b':
		case 'c':
		case 'd':
			return true;
		default:
			return false;
		}
	}

	using program = std::vector<std::unique_ptr<instruction>>;

	struct registers final
	{
		int a, b, c, d, pc;

		void reset() { a = b = c = d = pc = 0; }
	};

	static inline int& select(char reg, registers& rs)
	{
		switch (reg)
		{
		case 'a': return rs.a;
		case 'b': return rs.b;
		case 'c': return rs.c;
		case 'd': return rs.d;
		default:
			throw std::runtime_error("invalid register");
		}
	}

	static void read_program(program& prg, registers& rs)
	{
		std::string input;
		while (std::getline(std::cin, input))
		{
			char* p = const_cast<char*>(input.c_str());
			const char* pattern = " ";
			const char* opcode = p = std::strtok(p, pattern);
			auto opc = translate_opcode(opcode);
			p = std::strtok(nullptr, pattern);
			if (opc == Inc)
			{
				int& dst = select(p[0], rs);
				prg.push_back(make_inc(dst, rs.pc));
				continue;
			}
			if (opc == Dec)
			{
				int& dst = select(p[0], rs);
				prg.push_back(make_dec(dst, rs.pc));
				continue;
			}
			std::string reg_or_const = p;
			p = std::strtok(nullptr, " ");
			if (opc == Cpy)
			{
				int& dst = select(p[0], rs);
				if (is_register(reg_or_const[0]))
				{
					by_ref<int> src = select(reg_or_const[0], rs);
					prg.push_back(make_cpy(src, dst, rs.pc));
					continue;
				}
				else
				{
					by_val<int> src = std::stoi(reg_or_const);
					prg.push_back(make_cpy(src, dst, rs.pc));
					continue;
				}
			}
			if (opc == Jnz)
			{
				int dst = std::atoi(p);
				if (is_register(reg_or_const[0]))
				{
					by_ref<int> src = select(reg_or_const[0], rs);
					prg.push_back(make_jnz(src, dst, rs.pc));
					continue;
				}
				else
				{
					by_val<int> src = std::stoi(reg_or_const);
					prg.push_back(make_jnz(src, dst, rs.pc));
					continue;
				}
			}
			throw std::runtime_error("invalid instruction");
		}
	}

	static void display_registers(const registers& rs)
	{
		std::cout << "a = " << rs.a << " b = " << rs.b
			<< " c = " << rs.c << " d = " << rs.d
			<< " pc = " << rs.pc << "\n";
	}

	void day_12_part_1()
	{
		program prg;
		registers rs;

		read_program(prg, rs);

		//run program
		rs.reset();
		while (rs.pc < prg.size()) prg[rs.pc]->exec();

		display_registers(rs);
		std::cout << "What value is left in register a? " << rs.a << "\n";
	}

	void day_12_part_2()
	{
		program prg;
		registers rs;
		
		prg.push_back(make_cpy(by_val<int>(1), rs.c, rs.pc));
		read_program(prg, rs);

		//run program
		rs.reset();
		while (rs.pc < prg.size()) prg[rs.pc]->exec();

		display_registers(rs);
		std::cout << "What value is left in register a? " << rs.a << "\n";
	}
}

int day_12_main(unsigned int part)
{
	if (part == 1)
		Day12::day_12_part_1(); //Your puzzle answer was 317993.
	else
		Day12::day_12_part_2(); //Your puzzle answer was 9227647.	
	return 0;
}

#endif /* DAY_12_HPP */