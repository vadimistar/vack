# Instructions

To read more about what are instructions and what they do, go [here](../../vack/docs/Bytecode.md)

## `addf`

Stack state:	Before: ... -> value -> value 
				After:	... -> value

Used for float values.
Pops two numbers, adds them, pushes result to the top

## `addi`

Stack state:	Before: ... -> value -> value 
				After:	... -> value

Used for signed integer values.
Pops two numbers, adds them, pushes result to the top

## `addu`

Stack state:	Before: ... -> value -> value 
				After:	... -> value

Used for unsigned integer values.
Pops two numbers, adds them, pushes result to the top

## `call` <op>

Stack state:	Before: ... 
				After:	... -> value

Pushed the index of the current instruction to the stack, then jumps unconditionally to 
the instruction/label 'op'.

It's used for simulating function calls.

## `dup`

Stack state:	Before: ... -> value 
				After:	... -> value -> value

Duplicates the value on the top of the stack.

## `getconst`

Stack state:	Before: ... -> value 
				After:	... -> value

Pushes runtime constant with specific index, which is on the top of the stack. 

## `goto` <op>

Stack state:	Before: ...  
				After:	... 

Jumps unconditionally to the instruction/label 'op'.

## `halt`

Stack state:	Before: ...  
				After:	... 

Stops virtual machine.

## `if_eq` <op>

Stack state:	Before: ... -> value -> value 
				After:	... 

Jumps to the instruction/label 'op', if two values on the top of the stack are equal.

## `logb`

( Will be deprecated )

Stack state:	Before: ... -> value 
				After:	... -> value

Used for byte chars.
Writes the top of the stack to stdout.

## `logf`

( Will be deprecated )

Stack state:	Before: ... -> value 
				After:	... -> value

Used for floats.
Writes the top of the stack to stdout.

## `logi`

( Will be deprecated )

Stack state:	Before: ... -> value 
				After:	... -> value

Used for signed integer values.
Writes the top of the stack to stdout.

## `logp`

( Will be deprecated )

Stack state:	Before: ... -> value 
				After:	... -> value

Used for hex values (e.g. pointers).
Writes the top of the stack to stdout.

## `logu`

( Will be deprecated )

Stack state:	Before: ... -> value 
				After:	... -> value

Used for unsigned integer values.
Writes the top of the stack to stdout.

## `nop`

Stack state:	Before: ... 
				After:	...

Does nothing.

## `pop`

Stack state:	Before: ... -> value
				After:	...

Drops the value on the top of the stack

## `push` <op>

Stack state:	Before: ...
				After:	... -> value

Puts the 'op' value on the top of the stack.

## `ret`

Stack state:	Before: ... -> value
				After:	...

Jumps to the instruction/label, which is on the top of the stack.

## `subf`

Stack state:	Before: ... -> value -> value
				After:	... -> value

Used for float values.
Pops two numbers, subtracts them, pushes result to the top

## `subi`

Stack state:	Before: ... -> value -> value
				After:	... -> value

Used for signed integer values.
Pops two numbers, subtracts them, pushes result to the top

## `subu`

Stack state:	Before: ... -> value -> value
				After:	... -> value

Used for unsigned integer values.
Pops two numbers, subtracts them, pushes result to the top

## `swp`

Stack state:	Before: ... -> value -> value
				After:	... -> value -> value

Swops two values on the top of the stack.




