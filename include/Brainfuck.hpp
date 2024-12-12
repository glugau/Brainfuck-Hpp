/* Brainfuck.hpp */

#ifndef BRAINFUCK_HPP
#define BRAINFUCK_HPP

#include <cstdint>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stack>

namespace bfi
{
	namespace impl
	{
		template<class CellType=char>
		class Program;
	}

	using Brainfuck		= impl::Program<char>;
	using u8Brainfuck	= impl::Program<uint8_t>;
	using i8Brainfuck	= impl::Program<int8_t>;
	using u16Brainfuck	= impl::Program<uint16_t>;
	using i16Brainfuck	= impl::Program<int16_t>;
	using u32Brainfuck	= impl::Program<uint32_t>;
	using i32Brainfuck	= impl::Program<int32_t>;
	using u64Brainfuck	= impl::Program<uint64_t>;
	using i64Brainfuck	= impl::Program<int64_t>;

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

		template<class CellType = char>
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
			void run(std::istream& inputSstream, size_t maxTokens = 0);
			void run(std::istream& inputStream, std::ostream& outputStream, size_t maxTokens = 0);
			void run(std::ostream& outputStream, size_t maxTokens = 0);
			void run(const std::vector<CellType>& inputData, std::vector<CellType>& outputData, size_t maxTokens = 0);
		private:
			void compile(const std::string& code);
			std::string genCompileMessage(const std::string& code, size_t ierr);
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
			memSize{ memCells }
		{
			mem = new CellType[memCells];
			resetState();
			compile(code);
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
		void Program<CellType>::compile(const std::string& code)
		{
			compileSuccess = true;
			instructions.clear();
			std::stack<size_t> jmpStack;
			std::stack<size_t> dbgJmpStack; // Used to display where the misbehaving brackets are.

			for (size_t i = 0; i < code.size(); ++i)
			{
				char c = code[i];
				if (c == '+' || c == '-' || c == '>' || c == '<' || c == '.' || c == ',')
				{
					// combine consecutive operations
					size_t count = 1;
					while (i + 1 < code.size() && code[i + 1] == c)
					{
						++count;
						++i;
					}
					instructions.push_back({ c, count });
				}
				else if (c == '[')
				{
					instructions.push_back({ c, 0 }); // The jump target will be defined by ]

					jmpStack.push(instructions.size()); // Jump to the NEXT instruction
					dbgJmpStack.push(i);
				}
				else if (c == ']')
				{
					if (!jmpStack.empty())
					{
						size_t jt = jmpStack.top();
						jmpStack.pop();
						dbgJmpStack.pop();

						instructions.push_back({ c, jt });
						instructions[jt - 1].value = instructions.size(); // Jump to the next instruction on [
					}
					else if (compileSuccess) // Only return the first error message
					{
						// Ignore the misbehaving bracket, but notify that a compile
						// error has occured. Let the programmer determine if they
						// still want to run the code or not.
						compileSuccess = false;
						compileMessage = genCompileMessage(code, i);
					}
				}

				if (compileSuccess)
					compileMessage = "Compilation completed successfully without any errors.";
			}

			// Error: [ with no closing ]
			if (!jmpStack.empty())
			{
				if (compileSuccess)
				{
					compileSuccess = false;
					compileMessage = genCompileMessage(code, dbgJmpStack.top());
					;
				}

				// Ignore all unclosed [
				while (!jmpStack.empty())
				{
					size_t ji = jmpStack.top();
					instructions[ji].value = ji + 1; // Skip the [
					jmpStack.pop();
					dbgJmpStack.pop();
				}
			}
		}

		template<class CellType>
		std::string Program<CellType>::genCompileMessage(const std::string& code, size_t ierr)
		{
			size_t line = 1;
			size_t column = 1;

			for (size_t i = 0; i < ierr; ++i)
			{
				if (code[i] == '\n')
				{
					++line;
					column = 1;
				}
				else
				{
					++column;
				}
			}

			std::stringstream ss;
			ss << "Compilation failed: misplaced '" << code[ierr] << "' on line " << line << ", column " << column << " (no matching bracket found).";
			return ss.str();
		}

		template<class CellType>
		void Program<CellType>::run(size_t maxTokens)
		{

		}

		template<class CellType>
		void Program<CellType>::run(std::istream& is, size_t maxTokens)
		{

		}

		template<class CellType>
		void Program<CellType>::run(std::ostream& os, size_t maxTokens)
		{

		}

		template<class CellType>
		void Program<CellType>::run(std::istream& is, std::ostream& os, size_t maxTokens)
		{

		}

		template<class CellType>
		void Program<CellType>::run(const std::vector<CellType>& in, std::vector<CellType>& out, size_t maxTokens)
		{

		}
	}
}

#endif // BRAINFUCK_HPP