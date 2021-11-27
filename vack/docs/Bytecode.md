# Bytecode

Bytecode contains of 2 sections (strongly ordered):
 - runtime constants
 - machine instructions (code)

## Runtime constants

Runtime constants section consists of a sequence of [values](Values.md). 
It starts with 16-bit integer, which indicates the number of following runtime constants values.
Runtime constants are used for storing string literals and read-only arrays.

## Machine instructions (code)

Instructions are machine commands, they are executed, so it could do something.
They accept [values](Values.md) as arguments.

### Format:
```
<8-bit instruction code> [<[value](Values.md)>, <[value](Values.md)>, ...]
```

Instruction code is used for determining instruction kind. Each instruction has it's own required number of arguments.

