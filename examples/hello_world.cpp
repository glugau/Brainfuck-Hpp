#include "Brainfuck.hpp"

const std::string code = ">++++++++[<+++++++++>-]<."
						">++++[<+++++++>-]<+."
						"+++++++.."
						"+++."
						">>++++++[<+++++++>-]<++."
						"------------."
						">++++++[<+++++++++>-]<+."
						"<."
						"+++."
						"------."
						"--------."
						">>>++++[<++++++++>-]<+.";

int main()
{
	bfi::Brainfuck bf(code);
	std::string msg;
	if (bf.hasCompiled(msg))
	{
		std::vector<char> c;
		bf.run(nullptr, &c);
		for (char i : c)
			std::cout << i;
		std::cout << std::endl;
	}
	else
	{
		std::cerr << msg << std::endl;
	}

	return 0;
}