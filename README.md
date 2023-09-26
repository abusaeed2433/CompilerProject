# CompilerProject

## Structure

- Import section
    Using `import name;`

- Comment
    - single line comment
        - using `//`
    - multiline comment
        - Using `/* - */`
        - Supports `nested comment`

- Starting point of execution
    ```
    static void main(){ }
    ```

- Variable
    - Data type: `int`, `float`, `double`[ now int only]
    - Supports `declaration` & `initialization`
        - `int a=10;`
        - `int a,b, cd = 344;`
    - Supports `assignment`
        - `a = 234;`
        - `b = 40;`
    - Initialized to `0` by default,
    - Error if duplicate variable is declared,
    - Error if assign value to undeclared variable
    - All variables are stored until discarded explicitly
    - No scope

- Conditional operators
    - less than `lt`,
    - greater than `gt`,
    - equal `eq`,
    - not equals `neq`
    - less equal `le`,
    - greater equal `ge`

- `if` structure
    - vc = variable or constant
    ```
    if(vc @ vc){

    }
    ```
    - no else statment
