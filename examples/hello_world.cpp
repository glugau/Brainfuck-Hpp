#include "Brainfuck.hpp"

const std::string code = R"(
>++++++++[<+++++++++>-]<.
>++++[<+++++++>-]<+.
+++++++..
+++.
>>++++++[<+++++++>-]<++.
------------.
>++++++[<+++++++++>-]<+.
<.
+++.
------.
--------.
>>>++++[<++++++++>-]<+.
)";

int main()
{
	bfi::Brainfuck bf(code);
	
	std::string msg;
	if (bf.getCompileResult(msg))
	{
		bf.run(std::cout);
	}
	else
	{
		std::cerr << msg << std::endl;
	}

	return 0;
}