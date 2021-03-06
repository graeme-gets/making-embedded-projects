# Week Eight

### Output from program

Item|pointer
-|-
Stack Pointer|0x2001ff98
Heap Pointer|		0x2000054f
Initialised global variable |		0x20000000
Un-initialised global variable |		0x200002f4
Static variable |		0x200002f5
Function variable 	|	0x2001ffc3
Const variable 	|	0x800c918

## Linker File info
```
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 128K
  FLASH    (rx)    : ORIGIN = 0x8000000,   LENGTH = 512K
}
```


## Map file conformation



|.data.initGlobalVar|

                0x0000000020000000        0x1 ./Core/Cli/Src/consoleCommands.o
                0x0000000020000000                initGlobalVar

 |.bss.uninitGlobalVar|  

                0x00000000200002f4        0x1 ./Core/Cli/Src/consoleCommands.o
                0x00000000200002f4                uninitGlobalVar               

| bss.staticVar |  

                0x00000000200002f5        0x1 ./Core/Cli/Src/consoleCommands.o

|.rodata.constVar |  

                0x000000000800c918        0x1 ./Core/Cli/Src/consoleCommands.o
                0x000000000800c918                constVar                

Notes:
1. The Function variable is not within the map file as it is runtime and only exists for the life of the function. 
2. Variables are showing in RAM memory space 
3. Constant Variable is showing in FLASH memory space

### Initialised and Un-initialised variables 

```
/* Initialized data sections into "RAM" Ram type memory */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */

  } >RAM

  /* Uninitialized data section into "RAM" Ram type memory */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM
  ```