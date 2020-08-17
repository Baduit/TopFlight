# TopFlight
Top Flight langage specification and interpreter. Learn it to become a Top Flight developer.

# Todo:
- Make the instruction serializable easily (format to define)
- Add a way to store instructions (an instructions set) and call them
- Add the boolean type
- Add a way to call condionnaly instructions set
- Make an interpreter
- Separate clearly the c++ library (header only, full controll) from the C library (just take a string or a buffer representing the instruction) and the interpreter (basically the same as the C library but it takes a file), also make an interactive interpreter (read from stdin)
