# TopFlight
Top Flight langage implementation and the virtual machine YoloVM. Learn it to become a Top Flight developer. The langage is very basic, not really coherent but I'm having a lot of fun creating it.

# Top flight specifications
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

## Routine
A routine is a set of instruction you can call later. The syntaxe to declare a routine is a line with the name of the routine between <> like example:
* \<MyAwesomeRoutine>

A routine end with a line beginning by </ followed by the name of the routine and ending with a >:
* \</MyAwesomeRoutine>

```
<MyRoutine>
STORE str STRING("this is a string\n")
PRINT str
FREE str
</MyRoutine>
```

A routine cannot contain another routine.

To call a routine the instruction is CALL followed by the routine name example:
<MyRoutine>
STORE str STRING("this is a string\n")
PRINT str
FREE str
</MyRoutine>
CALL MyRoutine
```
A routine name can only contain alphanumeric character only or _ 

## Example of a basic program
```
STORE my_first_var INTEGER(5)
STORE my_second_var INTEGER(9)
ADD my_first_var my_second_var result_var
PRINT result_var
```
There are more example in the folder TopFlightExamples of this repository.

## Fun fact
Because the instruction and arguments are seprated by only 1 space, a space can be a valid variable name, same for a tabulation.

## Why is there no unit tests ?
It is a pet project for fun, unit tests are very important but not fun to write most of the time and I want to have fun during my free time else there's no point doing my pet projects.

# Todo:
- Add the boolean type
- Add a way to call condionnaly instructions set
- Make something usefull with arrays
- Make a cool interactive interpreter and a C library
- Proper exception and not just throw some std::runtime_error like a moron
- A LOT of refactoring, there a lot of ugly code right know and it will keep getting worse because I'm a lazy ass
