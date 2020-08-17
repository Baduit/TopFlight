# TopFlight
Top Flight langage specification and virtual machine. Learn it to become a Top Flight developer. The langage is very basic.

# Text format
## Misc
A program is composed of instruction and comments, each instruction and command are on their own lines.
Empty lines are discarded without errors, but lines with spaces/tabulations are not consireded empty.

## Comments
The line begins with a # without any space or tab before

## Instruction
Write the instruction type followed by its arguments, everything is seperated by space
- INSTRUCTION_TYPE ARG1 ARG2 

## Values
The values can be arguments has the following syntaxe for single values:
* TYPE(value)

If the type is an array the values are separated by a comma without any space. Like this:
* TYPE(value_1,value_2)

Strings are delimited by "", " can by escapted with a backslash \
Examples:
* INTEGER(3) 
* NUMBER(5.9)
* ARRAY_OF_STRING("Hello"," ","World","!\n")

## Example of a basic program
```
STORE my_first_var INTEGER(5)
STORE my_second_var INTEGER(9)
ADD my_first_var my_second_var result_var
PRINT result_var
```

## Fun fact
Because the instruction and arguments are seprated by only 1 space, a space can be a valid variable name, same for a tabulation.

# Todo:
- Make the instruction serializable easily
- Add a way to store instructions (an instructions set) and call them
- Add the boolean type
- Add a way to call condionnaly instructions set
- Make something usefull with arrays
- Make an interpreter
- Separate clearly the c++ library (header only, full controll) from the C library (just take a string or a buffer representing the instruction) and the interpreter (basically the same as the C library but it takes a file), also make an interactive interpreter (read from stdin)
- Handle in the parsing the \ for escaping the " and also for stuff like \n
