/* Brainfuck.hpp */

#ifndef BRAINFUCK_HPP
#define BRAINFUCK_HPP

#include <cstdint>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <stack>
#include <type_traits>

namespace bfi
{
	namespace _impl
	{
		template<class CellType=char>
		class Program;
	}

	using Brainfuck		= _impl::Program<char>;
	using u8Brainfuck	= _impl::Program<uint8_t>;
	using i8Brainfuck	= _impl::Program<int8_t>;
	using u16Brainfuck	= _impl::Program<uint16_t>;
	using i16Brainfuck	= _impl::Program<int16_t>;
	using u32Brainfuck	= _impl::Program<uint32_t>;
	using i32Brainfuck	= _impl::Program<int32_t>;
	using u64Brainfuck	= _impl::Program<uint64_t>;
	using i64Brainfuck	= _impl::Program<int64_t>;

	namespace _impl
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
			Program(const std::string& code, size_t memCells = 30000, bool wrapMemory = true);
			~Program();

			void resetMemory();
			void resetState();
			void resetInstrPtr();
			void resetMemPtr();
			bool hasCompiled(std::string& out) const;
			
			CellType& operator[](size_t i) const;

			void run(const std::vector<CellType>* inputData = nullptr, std::vector<CellType>* outputData = nullptr, size_t maxTokens = 0);
			void run(std::istream* inputStream, std::ostream* outputStream, size_t maxTokens = 0);
			void run(const std::string* inputString, std::string* outputString, size_t maxTokens = 0);
		private:
			template<class ItIn, class ItOut>
			void runIt(ItIn inBegin, ItIn inEnd, ItOut out, size_t maxTokens = 0);
			void compile(const std::string& code);
			std::string genCompileMessage(const std::string& code, size_t ierr);
		private:
			const bool wrap;
			bool compileSuccess = false;
			std::string compileMessage;
			size_t ip = 0;
			size_t mp = 0;
			const size_t memSize;
			CellType* mem;
			std::vector<Instruction> instructions;
		};

		template<class CellType>
		Program<CellType>::Program(const std::string& code, size_t memCells, bool wrapMemory) :
			memSize{ memCells }, wrap{wrapMemory}
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
			resetInstrPtr();
			resetMemPtr();
		}

		template<class CellType>
		void Program<CellType>::resetInstrPtr()
		{
			ip = 0;
		}

		template<class CellType>
		void Program<CellType>::resetMemPtr()
		{
			mp = 0;
		}

		template<class CellType>
		CellType& Program<CellType>::operator[](size_t i) const
		{
			return wrap ? mem[i % memSize] : mem[i];
		}

		template<class CellType>
		bool Program<CellType>::hasCompiled(std::string& out) const
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
			}

			if (compileSuccess)
				compileMessage = "Compilation completed successfully without any errors.";

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
		template<class ItIn, class ItOut>
		void Program<CellType>::runIt(ItIn inBegin, ItIn inEnd, ItOut out, size_t maxTokens)
		{
			size_t start = ip;
			while ((ip - start < maxTokens || maxTokens == 0) && ip < instructions.size())
			{
				Instruction instr = instructions[ip];
				switch (instr.instr)
				{
				case '+':
					mem[mp] += instr.value;
					break;
				case '-':
					mem[mp] -= instr.value;
					break;
				case '<':
					mp = wrap ? ((mp + memSize - instr.value % memSize) % memSize) : (mp - 1);
					break;
				case '>':
					mp = wrap ? (mp + instr.value) % memSize : mp + 1;
					break;
				case '.':
					if constexpr (!std::is_same_v<ItOut, std::nullptr_t>)
					{
						for (size_t i = 0; i < instr.value; ++i)
							*out++ = mem[mp];
					}
					break;
				case ',':
					if constexpr (!std::is_same_v<ItIn, std::nullptr_t>)
					{
						for (size_t i = 0; inBegin != inEnd && i < instr.value; ++i, ++inBegin)
							mem[mp] = *inBegin;
					}
					break;
				case '[':
					ip = mem[mp] ? ip : instr.value - 1;
					break;
				case ']':
					ip = mem[mp] ? instr.value - 1 : ip;
					break;
				}
				ip++;
			}
		}

		template<class CellType>
		void Program<CellType>::run(const std::vector<CellType>* in, std::vector<CellType>* out, size_t maxTokens)
		{
			if (in && out)
				runIt(in->begin(), in->end(), std::back_inserter(*out), maxTokens);
			else if (in)
				runIt(in->begin(), in->end(), nullptr, maxTokens);
			else if (out)
				runIt(nullptr, nullptr, std::back_inserter(*out), maxTokens);
			else
				runIt(nullptr, nullptr, nullptr, maxTokens);
		}

		template<class CellType>
		void Program<CellType>::run(const std::string* in, std::string* out, size_t maxTokens)
		{
			// Cast the input into the valid integer type, into a vector.
			std::vector<CellType> vin;
			std::vector<CellType>* pvin = in ? &vin : nullptr;
			std::vector<CellType> vout;
			std::vector<CellType>* pvout = out ? &vout : nullptr;

			if (in)
			{
				vin.reserve(in->size());
				for (char c : *in)
					vin.push_back((CellType)c);
			}

			run(pvin, pvout, maxTokens);

			if (out)
			{
				for (CellType v : vout)
					*out += (char)v;
			}
		}

		template<class CellType>
		void Program<CellType>::run(std::istream* is, std::ostream* os, size_t maxTokens)
		{
			if (is && os)
				runIt(std::istream_iterator<CellType>(*is), std::istream_iterator<CellType>(), std::ostream_iterator<CellType>(*os), maxTokens);
			else if (is)
				runIt(std::istream_iterator<CellType>(*is), std::istream_iterator<CellType>(), nullptr, maxTokens);
			else if (os)
				runIt(nullptr, nullptr, std::ostream_iterator<CellType>(*os), maxTokens);
			else
				runIt(nullptr, nullptr, nullptr, maxTokens);
		}
	}
}

#endif // BRAINFUCK_HPP