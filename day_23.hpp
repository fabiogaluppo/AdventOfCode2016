#ifndef DAY_23_HPP
#define DAY_23_HPP

#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <type_traits>

namespace Day23
{
	enum Opcode { Cpy, Inc, Dec, Jnz, Tgl, Mul };

	struct instruction
	{
		instruction(int& pc) : pc(pc) {}
		virtual ~instruction() {}
		virtual void exec() = 0;
		virtual instruction* morph() = 0;
		int& pc;
	};

	using program = std::vector<std::unique_ptr<instruction>>;

	template<typename SourceOperand, typename DestinationOperand>
	struct cpy_instruction final : public instruction
	{
		cpy_instruction(SourceOperand s, DestinationOperand d, int& pc)
			: instruction(pc), s(s), d(d) {}
		void exec() override
		{
			if (std::is_reference<DestinationOperand>::value)
				d = s;
			//else nop
			++pc;
		}
		instruction* morph() override; //definition below jnz_instruction		
	private:
		SourceOperand s;
		DestinationOperand d;
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
		instruction* morph() override; //definition below dec_instruction		
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
		instruction* morph() override
		{
			return new inc_instruction(d, pc);
		}
	private:
		int& d;
	};

	instruction* inc_instruction::morph()
	{
		return new dec_instruction(d, pc);
	}

	template<typename SourceOperand, typename DestinationOperand>
	struct jnz_instruction final : public instruction
	{
		jnz_instruction(SourceOperand s, DestinationOperand d, int& pc)
			: instruction(pc), s(s), d(d) {}
		void exec() override
		{
			if (s != 0)
				pc += d;
			else
				++pc;
		}
		instruction* morph() override
		{
			return new cpy_instruction<SourceOperand, DestinationOperand>(s, d, pc);
		}
	private:
		SourceOperand s;
		DestinationOperand d;
	};

	template<typename SourceOperand, typename DestinationOperand>
	instruction* cpy_instruction<SourceOperand, DestinationOperand>::morph()
	{
		return new jnz_instruction<SourceOperand, DestinationOperand>(s, d, pc);
	}

	struct tgl_instruction final : public instruction
	{
		tgl_instruction(int& d, int& pc, program& prg)
			: instruction(pc), d(d), prg(prg) {}
		void exec() override
		{
			size_t i = pc + d;
			if (i < prg.size())
			{
				std::unique_ptr<instruction>& si = prg[i]; //selected instruction
				si.reset(si->morph());
			}
			//else invalid

			++pc;
			return;
		}
		instruction* morph() override
		{
			return new inc_instruction(d, pc);
		}
	private:
		int& d;
		program& prg;
	};

	template<typename SourceOperand, typename DestinationOperand>
	struct mul_instruction final : public instruction
	{
		mul_instruction(SourceOperand s, DestinationOperand d, int& pc)
			: instruction(pc), s(s), d(d) {}
		void exec() override
		{
			if (std::is_reference<DestinationOperand>::value)
				d *= s;
			//else nop
			++pc;
		}
		instruction* morph() override
		{
			return new jnz_instruction<SourceOperand, DestinationOperand>(s, d, pc);
		}
	private:
		SourceOperand s;
		DestinationOperand d;
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
		if (std::strcmp("tgl", opcode) == 0)
			return Tgl;
		if (std::strcmp("mul", opcode) == 0)
			return Mul;
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
		return std::unique_ptr<instruction>(new cpy_instruction<int, int&>(s, d, pc));
	}

	static inline std::unique_ptr<instruction> make_cpy(by_ref<int> s, int& d, int& pc)
	{
		return std::unique_ptr<instruction>(new cpy_instruction<int&, int&>(s, d, pc));
	}

	static inline std::unique_ptr<instruction> make_inc(int& d, int& pc)
	{
		return std::unique_ptr<instruction>(new inc_instruction(d, pc));
	}

	static inline std::unique_ptr<instruction> make_dec(int& d, int& pc)
	{
		return std::unique_ptr<instruction>(new dec_instruction(d, pc));
	}

	static inline std::unique_ptr<instruction> make_jnz(by_val<int> s, by_val<int> d, int& pc)
	{
		return std::unique_ptr<instruction>(new jnz_instruction<int, int>(s, d, pc));
	}

	static inline std::unique_ptr<instruction> make_jnz(by_ref<int> s, by_val<int> d, int& pc)
	{
		return std::unique_ptr<instruction>(new jnz_instruction<int&, int>(s, d, pc));
	}

	static inline std::unique_ptr<instruction> make_jnz(by_val<int> s, by_ref<int> d, int& pc)
	{
		return std::unique_ptr<instruction>(new jnz_instruction<int, int&>(s, d, pc));
	}

	static inline std::unique_ptr<instruction> make_jnz(by_ref<int> s, by_ref<int> d, int& pc)
	{
		return std::unique_ptr<instruction>(new jnz_instruction<int&, int&>(s, d, pc));
	}

	static inline std::unique_ptr<instruction> make_tgl(int& d, int& pc, program& prg)
	{
		return std::unique_ptr<instruction>(new tgl_instruction(d, pc, prg));
	}

	static inline std::unique_ptr<instruction> make_mul(by_val<int> s, int& d, int& pc)
	{
		return std::unique_ptr<instruction>(new mul_instruction<int, int&>(s, d, pc));
	}

	static inline std::unique_ptr<instruction> make_mul(by_ref<int> s, int& d, int& pc)
	{
		return std::unique_ptr<instruction>(new mul_instruction<int&, int&>(s, d, pc));
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
			if (opc == Tgl)
			{
				int& dst = select(p[0], rs);
				prg.push_back(make_tgl(dst, rs.pc, prg));
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
				if (is_register(p[0]))
				{
					by_ref<int> dst = select(p[0], rs);
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
				else
				{
					by_val<int> dst = std::atoi(p);
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
			}
			if (opc == Mul)
			{
				int& dst = select(p[0], rs);
				if (is_register(reg_or_const[0]))
				{
					by_ref<int> src = select(reg_or_const[0], rs);
					prg.push_back(make_mul(src, dst, rs.pc));
					continue;
				}
				else
				{
					by_val<int> src = std::stoi(reg_or_const);
					prg.push_back(make_mul(src, dst, rs.pc));
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
			//<< " pc = " << rs.pc << "\n"; //starting in 0
			<< " pc = " << (rs.pc + 1) << "\n"; //starting in 1
	}

	void day_23_part_1()
	{
		program prg;
		registers rs;

		read_program(prg, rs);

		//run program
		rs.reset();
		rs.a = 7;
		while (rs.pc < prg.size()) prg[rs.pc]->exec();

		display_registers(rs);
		std::cout << "What value is left in register a? " << rs.a << "\n";
	}

	void day_23_part_2()
	{
		//program
		/*
		cpy a b
		dec b
		cpy a d
		cpy 0 a
		cpy b c
		inc a
		dec c
		jnz c -2
		dec d
		jnz d -5
		dec b
		cpy b c
		cpy c d
		dec d
		inc c
		jnz d -2
		tgl c
		cpy -16 c
		jnz 1 c
		cpy 78 c
		jnz 99 d
		inc a
		inc d
		jnz d -2
		inc c
		jnz c -5
		*/

		//program rewritten with mul instruction
		/*
		cpy a b
		dec b
		mul b a
		cpy b c
		dec c
		jnz c -4
		cpy 78 c
		cpy 99 d
		mul d c
		inc a
		dec c
		jnz c -2
		*/

		program prg;
		registers rs;

		read_program(prg, rs);

		//run program
		rs.reset();
		rs.a = 12;
		//display_registers(rs); //DBG
		while (rs.pc < prg.size())
		{
			prg[rs.pc]->exec();
			//display_registers(rs); //DBG
		}

		display_registers(rs);
		std::cout << "What value is left in register a? " << rs.a << "\n";
	}
}

int day_23_main(unsigned int part)
{
	if (part == 1)
		Day23::day_23_part_1(); //Your puzzle answer was 12762.
	else
		Day23::day_23_part_2(); //Your puzzle answer was 479009322.
	return 0;
}

#endif /* DAY_23_HPP */