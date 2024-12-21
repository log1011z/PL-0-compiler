# pl0-compiler
> This is a compiler for pl0 programming language writen in C++.

## What I Do
>Add annotations and array identification
Syntax tree presentation

### Grammer
```
program ::= block "." .
block ::= constPart varPart procPart statement .
constPart ::= ["const" ident "=" number {"," ident "=" number} ";"] .
varPart ::= ["var" identdec {"," identdec} ";"] .
procPart ::= {"procedure" ident ";" block ";"} .
identdec ::= ident idrange .
idrange ::= "(" range ":" range ")" | ε .
range ::= ident | number .
statement ::= assignStmt | callStmt | readStmt | writeStmt | compoundStmt | ifStmt | whileStmt | ε .
assignStmt ::= identuse ":=" expression .
callStmt ::= "call" identuse .
readStmt ::= "read" "(" identuse {"," identuse} ")" .
writeStmt ::= "write" "(" expression {"," expression} ")" .
compoundStmt ::= "begin" statement {";" statement} "end" .
ifStmt ::= "if" condition "then" statement [else statement] .
whileStmt ::= "while" condition "do" statement .
condition ::= "odd" expression | expression relOp expression .
relOp ::= "=" | "#" | "<" | "<=" | ">" | ">=" .
expression ::= ["+"|"-"] term { ("+"|"-") term } .
term ::= factor {("*"|"/") factor} .
factor ::= identuse | number | "(" expression ")" .
identuse ::= ident idadd .
idadd ::= "(" expression ")" | ε .

```

## Usage
- make
- ./pl0 "filepath"
- "exec the program"
- make clean

## LICENSE
MIT © [duduscript](https://github.com/duduscript)
