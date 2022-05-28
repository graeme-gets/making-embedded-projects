# Week Nine - Maths & Optimization



## Multiplication
While multiplication is a relatively quick function, it can be made more optimal by simply shifting the bits or by the use of a processor opcode to multiply iof the process has a maths unit. 

For Example
```
int multiplyByn(int num, int n)
{
    return num * n;
}
```
In Assembly, the result is obtained by a series of additions and subtractions as well as shift lefts. Teh compiler is not choosing to use the assembly 'mul' 
```
multiplyBy10:
        str     fp, [sp, #-4]!
        add     fp, sp, #0
        sub     sp, sp, #12
        str     r0, [fp, #-8]
        ldr     r2, [fp, #-8]
        mov     r3, r2
        lsl     r3, r3, #2
        add     r3, r3, r2
        lsl     r3, r3, #1
        mov     r0, r3
        add     sp, fp, #0
        ldr     fp, [sp], #4
        bx      lr
```

However, using the compiler optimisation, the code is reduced to 

```
        mul     r0, r1, r0
        bx      lr
```

## Taylor Series Cos function

Factorials are added in as precalculated const as this saved processor cycles and are stoic. 

The divide operations and floating point variables can be expensive on processor cycles

```
uint32_t F2 = 2;
uint32_t F4 = 24;
uint32_t F6 = 720;
uint32_t F8 = 40320;
float t_cos(float x)
{
    float ret = 1-(x*x)/F2 + (x*x*x*x)/F4 - (x*x*x*x*x*x)/F6;
    return ret;
}

float t_cos3(float x)
{

	float x2 = x * x;
	float x4 = x2*x2;
	float  x6 = x2 * x2 * x2;
	float ret = 1-x2/F2 + x4/F4 - x6/F6;
	ret = ret /10;
	return ret;
}
```
# TODO - outtatime!!!

Quadratic function for Cos