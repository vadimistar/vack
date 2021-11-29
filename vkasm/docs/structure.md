# Vkasm Structure

Assembly consists of sections, and sections are consist of instructions and labels:

## Sections

Sections are specified like this:
```
sec <section_name>

; Section content

sec <section_name2> ; Another section

; Another section content

```

Sections can't be declared multiple times.

### `.consts`

Used to specify [runtime constants](../../vack/docs/Bytecode.md).
For example:
```
sec .consts

.c0:
	10 4.3 90.0 150.6 ; array of some values
.c3: 
	"Hello, World!" ; array of byte chars
```

Labels before values are used to use them in other sections.  
When you pass these labels into instructions as arguments, the index of first value is passed.

### `.code`

Used to specify instructions, e.g.: 
```
sec .code

push 10
; ...
goto 0
```

Labels in this section are used to get instructions indexes. 

## Instructions
Instructions are single-line machine commands, they are executed at runtime.

For example:
```
push 10
push -15
addu
```

## Labels:
Labels are translation-time known symbols, which are used to get indexes more easily.

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
are identical for the stack machine, but the first one has more beautiful syntax, as labels are used there.
