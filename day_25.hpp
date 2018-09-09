#ifndef DAY_25_HPP
#define DAY_25_HPP

#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <type_traits>

namespace Day25
{
	enum Opcode { Cpy, Inc, Dec, Jnz, Tgl, Mul, Out };

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

	template<typename SourceOperand>
	struct out_instruction final : public instruction
	{
		out_instruction(SourceOperand s, void(**out_interruption)(void*, void*), int& pc, void* context)
			: instruction(pc), s(s), context(context), out_interruption(out_interruption) {}
		void exec() override
		{
			if (out_interruption != nullptr && *out_interruption != nullptr) (**out_interruption)(context, &s);
			++pc;
		}
		instruction* morph() override
		{
			throw std::runtime_error("morph is invalid");
		}
	private:
		SourceOperand s;
		void(**out_interruption)(void*, void*);
		void* context;
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
		if (std::strcmp("out", opcode) == 0)
			return Out;
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

	static inline std::unique_ptr<instruction> make_out(by_val<int> s, void(**out_interruption)(void*, void*), int& pc, void* context)
	{
		return std::unique_ptr<instruction>(new out_instruction<int>(s, out_interruption, pc, context));
	}

	static inline std::unique_ptr<instruction> make_out(by_ref<int> s, void(**out_interruption)(void*, void*), int& pc, void* context)
	{
		return std::unique_ptr<instruction>(new out_instruction<int&>(s, out_interruption, pc, context));
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

		void(*out_interruption)(void*, void*);
		bool out_to_stdout;

		static const size_t VECTOR_SIZE = 2;
		int v[VECTOR_SIZE];
		size_t vi;

		void reset()
		{
			a = b = c = d = pc = 0;
			out_interruption = nullptr;
			for (size_t i = 0; i < VECTOR_SIZE; ++i) v[i] = 0;
			vi = 0;
			out_to_stdout = false;
		}
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
			if (opc == Out)
			{
				if (is_register(reg_or_const[0]))
				{
					by_ref<int> src = select(reg_or_const[0], rs);
					prg.push_back(make_out(src, &rs.out_interruption, rs.pc, &rs));
					continue;
				}
				else
				{
					by_val<int> src = std::stoi(reg_or_const);
					prg.push_back(make_out(src, &rs.out_interruption, rs.pc, &rs));
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
		std::cout << "vi = " << rs.vi << " v = ";
		for (size_t i = 0; i < registers::VECTOR_SIZE; ++i)
			std::cout << rs.v[i] << " ";
		std::cout << "\n";
	}

	static void out_display(void* context, void* s)
	{
		registers* rs = reinterpret_cast<registers*>(context);
		int* r = reinterpret_cast<int*>(s);
		rs->vi = rs->vi % registers::VECTOR_SIZE;
		rs->v[rs->vi] = *r;
		++rs->vi;
		if (rs->out_to_stdout) std::cout << *r << " ";
	}

	static inline int find_0_1_sequence(const program& prg, registers& rs)
	{
		const size_t MATCHES_THRESHOLD = 100; //100000;
		for (int i = 0; i < std::numeric_limits<int>::max(); ++i)
		{
			rs.reset();
			//rs.out_to_stdout = true; //DBG
			rs.out_interruption = out_display;
			rs.a = i;
			size_t match_counter = 0;
			while (rs.pc < prg.size())
			{
				prg[rs.pc]->exec();
				if (rs.vi == registers::VECTOR_SIZE)
				{
					rs.vi = 0;
					if (rs.v[0] == 0 && rs.v[1] == 1)
					{
						if (++match_counter == MATCHES_THRESHOLD)
							break; //success
					}
					else
					{
						//std::cout << "\n" << i << " -----------------------------------------------------------\n"; //DBG
						break; //fail
					}
				}
			}
			//std::cout << "\n"; //DBG
			if (match_counter == MATCHES_THRESHOLD)
				return i;
		}
		return -1; //not found
	}

	void day_25_part_1()
	{
		//program
		/*
		cpy a d
		cpy 9 c
		cpy 282 b
		inc d
		dec b
		jnz b -2
		dec c
		jnz c -5
		cpy d a
		jnz 0 0
		cpy a b
		cpy 0 a
		cpy 2 c
		jnz b 2
		jnz 1 6
		dec b
		dec c
		jnz c -4
		inc a
		jnz 1 -7
		cpy 2 b
		jnz c 2
		jnz 1 4
		dec b
		dec c
		jnz 1 -4
		jnz 0 0
		out b
		jnz a -19
		jnz 1 -21
		*/

		program prg;
		registers rs;

		read_program(prg, rs);
		int a = find_0_1_sequence(prg, rs);

		display_registers(rs);
		std::cout << "What is the lowest positive integer that can be used to initialize register a "
			<< "and cause the code to output a clock signal of 0, 1, 0, 1... repeating forever? " << a << "\n";
	}
}

int day_25_main(unsigned int)
{
	Day25::day_25_part_1();	//Your puzzle answer was 192.
	return 0;
}

#endif /* DAY_25_HPP */