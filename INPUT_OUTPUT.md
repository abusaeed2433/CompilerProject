<table>
<tr>
<td valign="top">

```
/* importing header */

import math;
import stdio;
import math;

// declaring function prototype
int add(int,int, float);
int scan();
void add(int, float, double);

/*
    multi-line comment
    /* support nested also */
*/


static void entryPoint(){

    // variable declaration
    int a;
    float b,c;
    
    println(a b c);
    // deleting variables


    discard a,b,c;


    /* variable initialization */
    int a = 10, b;
    int c = a <add> b, d = 10 <sub> a;
    println(a,b,c,d);

    double e = @max(2,3), f;
    println(e,f);



    discard c,d,e, /* valid comment */ f;



    // variable assignment
    a = 100;
    println("a: " a);

    b = a <rem> 11;
    println("b: " b);

    b = @max(a,101);
    println("b: " b);
    

    discard a,b;


    /* condition */
    float a = 10;
    float b = 100;
    
    justInCase(a <lt> b){
        println("Actually executed since " a "< " b);
        
        justInCase(10 <lt> 100){
            println("Also executed");
        }

        justInCase(10 <gt> b){
            println("Not executed since false");
        }
        otherwise{
            println("Inside else. Since 10" " is < than " b);
        }
    }

    println("------------- separator -----------------");

    a = 10; int c = 15;
    justInCase(5 <lt> 10 and a <lt> c){
        println("5 < 10 and " a " < " c);

        int j = 55;
        till(j <gt> 0){
            j = j <sub> 15;
        }
        
    }

    justInCase(5 <lt> 10 and a <lt> b and 10 <gt> 10 ){
        println("Condition is false");
    }
    otherwise{
        println("Executed else block");
    }
    
    
    
    
    
    int i = 0;
    till( i <lt> 100){
        i = i <add> 15;
    }


    /* library function */

    int n = @scanInt();

    println(n);


    float f = @scan();
    println("Value after scan is: " f);

    discard n,f;

    int m = 6, n = 4;
    int result = m <mul> n;
    @show(result);

    double mx = @max(2.5,5);
    @show(mx);



    float rootf = @sqrt(42);
    println("Square root of 42 is :" rootf "(float)");

    int toFind = -42;
    n = @sqrt(-42);
    println("Square root of " toFind " is :" n "(int)");

    n = @toInt(rootf);
    
    println(rootf " becomes " n " after toInt" );


    rootf = @toFloat(n);
    
    println(n " becomes " rootf " after toFloat" );

    rootf = @toDouble(rootf);
    
    println(rootf " becomes " rootf " after toDouble" );


    int mn = @show(n);


    // user defined function
    int n1, n2;
    float f1;


    int addRes = @add(n1,n1,f1);
    println(addRes);

    @add(m,f1,100);

    int dum = @add(m,f1,100);

    @test();
}
```
</td>

<td valign="top" >

```
Captured comment: /* importing header */

Imported math
Imported stdio
Import already exists

Single line comment: // declaring function prototype
Prototype inserted: int add(int,int,float)
Prototype inserted: int scan()
Prototype inserted: void add(int,float,double)


Captured comment: /*|    multi-line comment|    /* support nested also */|*/





    
Single line comment: // variable declaration



0 0.000000 0.000000  < - - - - - - - - - -
Single line comment: // deleting variables

Discarded variable c
Discarded variable b
Discarded variable a

Captured comment: /* variable initialization */


10 0 10 0  < - - - - - - - - - -

Calling function float max(any,any)
3.000000 0.000000  < - - - - - - - - - -
     
Captured comment: /* valid comment */
Discarded variable f
Discarded variable e
Discarded variable d
Discarded variable c
    
Single line comment: // variable assignment

a: 100  < - - - - - - - - - -
        

b: 1  < - - - - - - - - - -

    
Calling function float max(any,any)    
b: 101  < - - - - - - - - - -

Discarded variable b
Discarded variable a


Captured comment: /* condition */

                  
If condition is True        
Actually executed since 10.000000 < 100.000000  < - - - - - - - - - -

If condition is True
Also executed < - - - - - - - - - -
If-else processed

If condition is False


Else condition is True
Inside else. Since 10 is < than 100.000000  < - - - - - - - - - -
If-else processed
If-else processed

------------- separator ----------------- < - - - - - - - - - -
               

If condition is True
5 < 10 and 10.000000  < 15  < - - - - - - - - - -
                              
Loop matched
Iterating - 0, value: 55
Iterating - 1, value: 40
Iterating - 2, value: 25
Iterating - 3, value: 10
If-else processed

If condition is False
    

Else condition is True
Executed else block < - - - - - - - - - -
If-else processed
                       

Loop matched
Iterating - 0, value: 0
Iterating - 1, value: 15
Iterating - 2, value: 30
Iterating - 3, value: 45
Iterating - 4, value: 60
Iterating - 5, value: 75
Iterating - 6, value: 90

Captured comment: /* library function */

Calling function int scanInt()
Taking int input 0
    
0  < - - - - - - - - - -

Calling function int scan()
Value after scan is: 0.000000  < - - - - - - - - - -

Discarded variable f
Discarded variable n
            
Calling function void show(any)
From show function: 24.000000 
    
Calling function float max(any,any)
Calling function void show(any)
From show function: 5.000000 
    

Calling function double sqrt(any)
Square root of 42 is :6.480741 (float) < - - - - - - - - - -

Calling function double sqrt(any)
Stay in real world
Square root of -42  is :0 (int) < - - - - - - - - - -
    
Calling function int toInt(any)
Warning - library converter is not imported    
6.480741  becomes 6  after toInt < - - - - - - - - - -
    

Calling function float toFloat(any)
Warning - library converter is not imported
6  becomes 6.000000  after toFloat < - - - - - - - - - -
    
Calling function double toDouble(any)
Warning - library converter is not imported    
6.000000  becomes 6.000000  after toDouble < - - - - - - - - - -

Calling function void show(any)
From show function: 6.000000 
Invalid assignment from void to int
    
Single line comment: // user defined function

            


Calling function int add(int,int,float)
0  < - - - - - - - - - -

Calling function void add(int,float,double)

Calling function void add(int,float,double)
Invalid assignment from void to int

Function not found
program executed


----------Printing all variables----------
a(float) 10.000000 -> b(float) 100.000000 -> c(int) 15 -> j(int) -5 -> i(int) 105 -> n(int) 6 -> m(int) 6 -> result(int) 24 -> mx(double) 5.000000 -> rootf(float) 6.000000 -> toFind(int) -42 -> mn(int) 0 -> n2(int) 0 -> n1(int) 0 -> f1(float) 0.000000 -> addRes(int) 0 -> dum(int) 0 -> 


Printing all prototype
int add(int,int,float)
int scan()
void add(int,float,double)

All library functions are:
int scanInt()
float scan()
void show(any)
float max(any,any)
double sqrt(any)
int toInt(any)
float toFloat(any)
double toDouble(any)

Printing all imports:
math
stdio

```
</td>

</tr>
</table>

> Extra lines are added/removed to make the two files consistent.
