# TopFlight
Top Flight langage implementation and the virtual machine YoloVM. Learn it to become a Top Flight developer. The langage is very basic, not really coherent but I'm having a lot of fun creating it.  
It does not aim to be fast or usable in a real project, quite the opposite, this language is literally a joke a bit inspired by the [Rockstar language](https://github.com/RockstarLang/rockstar).

# Top flight overview
## Misc
A program is composed of instruction and comments, each instruction and command are on their own lines.
Empty lines are discarded without errors, but lines with spaces/tabulations are not consireded empty.

## Comments
The line begins with a `#` without any space or tab before

## Instruction
Write the instruction type followed by its arguments, everything is seperated by space, list of instruction can be seen in Instruction.hpp
- `INSTRUCTION_TYPE ARG1 ARG2`

## Values
The values can be arguments has the following syntaxe for single values:
* `TYPE(value)`

If the type is an array the values are separated by a comma without any space. Like this:
* `TYPE(value_1,value_2)`

Strings are delimited by `""`

`"` can by escapted with a backslash `\`

Examples:
* `INTEGER(3)` 
* `NUMBER(5.9)`
* `BOOLEAN(true)`
* `ARRAY_OF_STRING("Hello"," ","World","!\n")`

The list of types availables in the file Types.hpp

## Routine
A routine is a set of instruction you can call later. The syntaxe to declare a routine is a line with the name of the routine between <> like example:
* `<MyAwesomeRoutine>`

A routine end with a line beginning by </ followed by the name of the routine and ending with a >:
* `</MyAwesomeRoutine>`

```
<MyRoutine>
STORE str STRING("this is a string\n")
PRINT str
FREE str
</MyRoutine>
```

A routine cannot contain another routine.

To call a routine the instruction is `CALL` followed by the routine name example:
```
<MyRoutine>
STORE str STRING("this is a string\n")
PRINT str
FREE str
</MyRoutine>
CALL MyRoutine
```
A routine name can only contain alphanumeric character only or `_` 

## Condition
With the instruction `CALL_IF` you can call only if a `BOOLEAN` is `true`. Example:
```
<hello_world>
STORE hi STRING("hello world")
PRINT hi
</hello_world>

STORE b_true BOOLEAN(true)
STORE b_false BOOLEAN(false)

# This call happends
CALL_IF hello_world b_true
# This call does not happend
CALL_IF hello_world b_false
```

## Example of a basic program
```
STORE my_first_var INTEGER(5)
STORE my_second_var INTEGER(9)
ADD my_first_var my_second_var result_var
PRINT result_var
```
There are more example in the folder TopFlightExamples of this repository.

## Notes
* The arguments (after the path of the script to execute) are store into an ARRAY_OF_STRING named "args";
* It is possible to overwrite values or routines
* It is possible to have routine and values with the same name

## Why is there so few unit tests ?
It is a pet project for fun, unit tests are very important but not fun to write most of the time and I want to have fun during my free time else there's no point doing my pet projects.

# Play with it online:
You can play with it online at: https://topflightonline.baduit.eu/

You can put a colored pixel in the canvas if you print a string looking like this `__PIXEL_PUT__ x y color` where x and y are numbers and colors and string which is a valid color for the web canvas.
And if you print a string looking like this `__CANVAS_RESIZE__ x y` it resizes the canvas.

There is an example in the file _canvas.tpf_.

# How to build
## How to build the interpreter ?
`cargo build`

## How to build the wasm code for the website
`wasm-pack build topflight_wasm  --target web --out-dir ../website/pkg`

# Things that would be nice but I'm pretty sure I won't have the time to do it
- A real doc for the langage instead/in addition of this readme
- Make a cool interactive interpreter
- Add functionnal tests would be nice, less boring to do than unit test (I know I know I should also do unit tests) and still usefull
- Improve the online editor (https://www.baduit.eu/topflight/)

