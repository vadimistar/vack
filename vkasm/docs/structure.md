# Vkasm Structure

Assembly consists of instructions and labels:

## Instructions
Instructions are single-line machine commands, they are executed at runtime.

For example:
```
push 10
push -15
addu
```

## Labels:
Labels are translation-time known symbols, which are used to get instruction indexes more easily.

For example, this code:
```
goto start
goto end
start:
  push 1
  goto 1
end:
  pop
  halt
```
and this code:
```
goto 2
goto 4
push 1
goto 1
pop
halt
```
are identical to the stack machine, but the first one has more beautiful syntax, as labels are used there.
