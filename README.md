# Bare Metal Raspberry Pi Elf Workshop
This project is for a mechanical elf workshop that we're building. Run the 
build script to compile the project.

## Makefile Info
Below are some important compiler flags and what they mean.

 - `-ffreestanding`: Tells the compiler that this code isn't running on a 
   normal operating system. Therefore, it will not assume things such as 
   `int  main()` as the entry point of the program. 

 - `-nostdlib`: Tells the compiler to not link the c standard library. This 
   means we won't have access to things like `printf()` or `malloc()`

 - `-nostartfiles`: Prevents the compiler from linking startup code (`crt0.o`)
   that eventually call `main()`. We don't want this, because `boot.S` is the 
   startup code


## Assembly Code in boot.S File
Below is a description of some of the commands in the assembly code, and what 
they're doing
<table>
  <thead>
    <th>Command</th>
    <th>Meaning</th>
    <th>Function in this Code</th>
  </thead>

  <tbody>
    <tr>
      <td>ldr x0, =label</td>
      <td>Load Address</td>
      <td>
        Loads the memory address of a symbol (like __stack_top) into a 
        register.
      </td>
    </tr>
    <tr>
      <td>mov sp, x0</td>
      <td>Move</td>
      <td>Sets the Stack Pointer to the top of the stack memory.</td>
    </tr>
    <tr>
      <td>cmp</td>
      <td>Compare</td>
      <td>
        Checks if the memory pointer has reached the end of the .bss section.
      </td>
    </tr>
    <tr>
      <td>str xzr, [x0], #8</td>
      <td>Store Zero (Post-index)</td>
      <td>
        Writes 0 to memory and increments the pointer by 8 bytes efficiently.
      </td>
    </tr>
    <tr>
      <td>bl</td>
      <td>Branch with Link</td>
      <td>Calls the kernel_main C function.</td>
    </tr>
    <tr>
      <td>wfe</td>
      <td>Wait For Event</td>
      <td>Pauses the CPU to save power in the infinite loop.</td>
    </tr>
</table>

### Understanding the .bss clearing loop
When the linker says:
```
__bss_start = 0x81000
__bass_end  = 0x81200
```
The assembly instruction do this:
```
x0 = 0x81000
x1 = 0x81200
```
Which makes the loop function like this:
```
store 0
advance 8 bytes
store 0
advance 8 bytes
store 0
advance 8 bytes
...
```
until `x0 == x1`. At that point, every uninitialized global variable contains
a 0 value instead of whatever random value already existed at the memory 
address.


## UART Drivers
**UART** stands for *Universal Asynchronous Receiver/Transmitter*. The source 
code file `src/uart.c` allows us to talk directly to the hardware, and write 
data one byte at a time. The Raspberry Pi exposes hardware peripherals 
through memory addresses. Normally, when you write:
```c
int x = 5;
```
you are using RAM. But ARM processors allow certain addresses to be mapped to 
hardware devices. For example:

<table>
  <thead>
    <th>Address</th>
    <th>Meaning</th>
  </thead>
  <tbody>
    <tr>
      <td>0x00000000</td>
      <td>RAM</td> 
    </tr>
    <tr>
      <td>0x3F201000</td>
      <td>UART Registers</td> 
    </tr>
    <tr>
      <td>0x3F200000</td>
      <td>GPIO registers</td> 
    </tr>
    <tr>
      <td>0x3F300000</td>
      <td>SPI Registers</td> 
    </tr>
  </tbody>
</table>

Writing to the addresses above doesn't wrote a value in RAM. Instead it 
changes the hardware behavior. The following line 
```c 
*address = value;
```
usually means `RAM address -> Store some value`, but here
`UART register address -> UART hardware -> Send a byte over serial`. The CPU 
thinks it's writing memory. The hardware interprets it as a command.

The Pi documentation says:
> The PL011 UART registers begin at offset 0x201000 from the peripheral base.
So we can get the starting address of UART0 like this:
```
0x3F000000
+
0x00201000
----------
0x3F201000
```

### The Registers:
Now we define specific UART registers:
```c
#define UART0_DR ((volatile unsigned int *)(UART0_BASE + 0x00))
```
DR means Data Register. This is where bytes are sent and received.
So this:
```c
*UART0_DR = 'A';
```
is equivalent to:
```
Put the ASCII value for A into the UART transmit register
```
and the UART sends it. 


