# DNF conversion

## \<Input step>

### Step 1. Execute source file
> How to build?<br/>

`gcc hw2.c -o file <or> make`<br/>

> How to run?<br/>

`./file`

### Step 2. Input a propositional formula
When you execute program, you must be input a propositional formula. 
Otherwise, program will return a error message "Invalid form!" and be terminated itself.
```
ex) (or a1 (not (or (not (or a2 a3)) a4)))
```
## \<Output step>

### First output 
> The line before '0' prints the result value in units of clause based on DNF form.
```
ex) 1
    2 -4
    3 -4
```
### Second output
> The line after '0' prints the result value whether the formula is satisfiable or not.
```
ex) 1 2 3 -4
```
### Full output
```
ex) 1
    2 -4
    3 -4
    0
    1 2 3 -4
```

## \<Reference>
When you input a propositional formula, the following rules must be observed.<br/>
Otherwise, Program will be terminated or return Error message.

1. Each word must have a blank space excepting "(" or ")".
    
2. The form of propositional variable must be a combination of one alphabet and a number. ex) a1, a2, a3 ...
