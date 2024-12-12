

# Brainfuck.hpp
A header-only brainfuck interpreter, for C++17.


## Quickstart
First, download the [Brainfuck.hpp](include/Brainfuck.hpp) file.
```cpp
#include "Brainfuck.hpp"

int main()
{
    const std::string code = /* Your brainfuck code */;
    bfi::Brainfuck bf(code); // Compile your code

    std::string message; // Get the compile error message
    if(bf.hasCompiled(message))
    {
        std::string input = /* Your program input*/;
        std::string output;
        bf.run(&input, &output);
        bf[30]; // Acces the 31th cell of the memory, after execution.
        bf.resetState(); // Reset the state (memory & pointers) of the interpreter.
        // Alternatively, bf.resetMemory() resets only the memory.
    }
    else
    {
        std::cerr << message << std::endl;
    }
}

```


## API Reference
All the code is contained in the `bfi` namespace (standing for **B**rain**f**uck **i**nterpreter).
#### Compile some Brainfuck
This Brainfuck interpreter "pre-compiles" code for faster execution. This is done in the constructor. The source code must be a `std::string`.
```C++
bfi::Brainfuck bf(source, 30000, true);
```
|Parameter|Default value|Description|
|--|--|--|
|source: `const std::string&`| **Required** |The Brainfuck source code
|memCells: `size_t`| `30000`| The number of memory cells
|wrapMemory: `bool`|`true`| If true, the memory pointer will wrap around. If false, going out of bounds will result in undefined behavior.

#### Brainfuck styles
If you wich to modify the cell size in bits, multiple Brainfuck interpreters are available.
| Name | Type |
|--|--|
| `bfi::Brainfuck` | `char`
| `bfi::u8Brainfuck` | `uint8_t`
| `bfi::i8Brainfuck` | `int8_t`
| `bfi::u16Brainfuck` | `uint16_t`
| `bfi::i16Brainfuck` | `int16_t`
| `bfi::u32Brainfuck` | `uint32_t`
| `bfi::i32Brainfuck` | `int32_t`
| `bfi::u64Brainfuck` | `uint64_t`
| `bfi::i64Brainfuck` | `int64_t`

#### Compile errors
In case of misplaced brackets, you will get a compile error. You can check it as follows.
```cpp
bfi::Brainfuck bf(code);
std::string msg;
if(bf.hasCompiled(msg)) {/* ... */}
else{ std::cerr << msg << std::endl; }
```
A compile error only occurs in case of misplaced brackets. The `msg` string is passed by reference, and will contain, in the event of a compile error, a message to locate the (first) misplaced bracket.

**Note:** you may still run Brainfuck code that has failed to compile. The interpreter will ignore misplaced brackets, as if they were comments.
#### Running your code
Once you've compiled the code, you can execute it.
```cpp
bf.run(&std::cin, &std::cout, 0); // Run with istream & ostream

std::string sInput = /* ... */;
std::string sOutput;
bf.run(&sInput , &sOutput, 0); // Run with strings

std::vector<char> vInput = /* ... */;
std::vector<char> vOutput;
bf.run(&vInput , &vOutput,); // Run with vectors
```
The `bfi::Brainfuck::run` method takes three optional arguments:
|Parameter|Type|Default Value|Description
|--|--|--|--|
|`input`|`std::vector<CellType>*`/`std::string*`/`std::istream*`|`nullptr`|Input source of the program. If `nullptr`, then `,` instructions are ignored.|
|`output`|`std::vector<CellType>*`/`std::string*`/`std::ostream*`|`nullptr`|Output destination of the program. If `nullptr`, then `.` instructions are ignored.|
|`maxTokens`|`size_t`|`0`|Maximum amount of tokens (*) to be executed before stopping.|

(*) a token isn't a single character. It is a grouping of characters defined internally by the interpreter.

Argument types **MUST** match! You can only use `vector` & `vector`, `istream` & `ostream`, or `string` & `string`.

#### After execution
Once a program finishes running, its state is **not** set back to default. This is so you can explore the memory, or possibly modify it.
```cpp
bf.resetState(); // Calls the following commands.

bf.resetMemory(); // Reset only the memory
bf.resetInstrPtr(); // Reset only the instruction pointer back to 0 (start of program).
bf.resetMemPtr(); // Reset only the memory pointer back to 0.

bf[i];
// Access the ith memory cell. This follows the wrapping
// rule set at the construction of the interpreter
// (if wrapMemory = true, i will wrap if too large).
```

## License

This project is licensed under the [MIT](https://choosealicense.com/licenses/mit/) license.