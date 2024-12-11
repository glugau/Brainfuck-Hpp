/* Brainfuck.hpp */

#ifndef BRAINFUCK_HPP
#define BRAINFUCK_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

namespace bfi
{
	namespace impl 
	{
		struct Instruction
		{
			char instr; // char representing the instruction
			size_t value; // n times the instruction shall be executed
			
			static inline bool isValid(char c)
			{
				constexpr char valid[] = "+-][,.><";
				for (int i = 0; i < 8; ++i)
					if (valid[i] == c) return true;
				return false;
			}
		};
	}

	template<class CellType=uint8_t>
	class Program
	{
	public:
        Program(const std::string& code, size_t memCells = 30000);
		~Program();

		void resetMemory();
		void resetState();
		bool getCompileResult(std::string& out) const;
		CellType getMemory(size_t i) const;

        void run(size_t maxTokens = 0);
        void run(std::istream& inpuSstream, size_t maxTokens = 0);
        void run(std::istream& inputStream, std::ostream& outputStream, size_t maxTokens = 0);
		void run(std::ostream& outputStream, size_t maxTokens = 0);
		void run(const std::vector<CellType>& inputData, std::vector<CellType>& outputData, size_t maxTokens = 0);
		void run(std::vector<CellType>& outputData, size_t maxTokens = 0);
		void run(const std::vector<CellType>& inputData, size_t maxTokens = 0);
	private:
		void compile(const std::string& code);
	private:
		bool compileSuccess = false;
		std::string compileMessage;
		size_t ip = 0;
		size_t mp = 0;
		const size_t memSize;
		CellType* mem;
		std::vector<impl::Instruction> instructions;
	};

	template<class CellType>
	Program<CellType>::Program(const std::string& code, size_t memCells) :
		memSize{memCells}
	{
		mem = new CellType[memCells];
		resetState();
		compile(code);
	}

	template<class CellType>
	void Program<CellType>::compile(const std::string& code)
	{
		compileSuccess = true;
	}

	template<class CellType>
	Program<CellType>::~Program()
	{
		delete[] mem;
	}

	template<class CellType>
	void Program<CellType>::resetMemory()
	{
		std::fill(mem, mem + memSize, 0);
	}

	template<class CellType>
	void Program<CellType>::resetState()
	{
		resetMemory();
		ip = 0;
		mp = 0;
	}

	template<class CellType>
	CellType Program<CellType>::getMemory(size_t i) const
	{
		return mem[i % memSize];
	}

	template<class CellType>
	bool Program<CellType>::getCompileResult(std::string& out) const
	{
		out = compileMessage;
		return compileSuccess;
	}

	template<class CellType>
	void Program<CellType>::run()
	{

	}

	template<class CellType>
	Program<CellType>::~Program()
	{

	}

	template<class CellType>
	Program<CellType>::~Program()
	{

	}

	template<class CellType>
	Program<CellType>::~Program()
	{

	}

	template<class CellType>
	Program<CellType>::~Program()
	{

	}

	template<class CellType>
	Program<CellType>::~Program()
	{

	}
}

#endif // BRAINFUCK_HPP