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


## UART Driver
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

Writing to the addresses above doesn't write a value in RAM. Instead it 
changes the hardware behavior. The following line 
```c 
*address = value;
```
usually means `RAM address -> Store some value`, but here
`UART register address -> UART hardware -> Send a byte over serial`. The CPU 
thinks it's writing memory. The hardware interprets it as a command.

### The Registers:

#### Memory Mapped IO Register
The **memory mapped IO** registers on a Raspberry Pi 3 and up, begin at 
register `0x3F000000`. We create a reference to this register on this line:
```
// Memory mapped IO base address
#define MMIO_BASE 0x3F000000
```

#### UART Base Register
The Pi documentation says:
> The PL011 UART registers begin at offset 0x201000 from the peripheral base.

So we can get the starting address of `UART0` like this:
```
0x3F000000
+
0x00201000
----------
0x3F201000
```
That's where the register address for `UART0_BASE` come from on the following
line:
```c
// UART register addresses
#define UART0_BASE (MMIO_BASE + 0x201000)
```

#### UART Data Register
Now we define specific UART registers, starting with the **data register**.
This is where bytes of data are sent and received.
```c
#define UART0_DR (UART0_BASE + 0x00)
```
So this:
```c
*UART0_DR = 'A';
```
is equivalent to:
```
Put the ASCII value for A 
into the UART transmit register
```
and the UART sends it.

#### UART Flag Register
This register tells the current UART state. For example:
```
Bit 5 = TX FIFO Full
Bit 4 = RX FIFO Empty
Bit 3 = Busy
...
```
The flag register is defined on this line:
```c
#define UART0_FR (UART0_BASE + 0x18)
```

#### UART Transmit FIFO Flag
We create a bit mask on this line:
```
#define UART_FR_TXFF (1 << 5)
```
Binary:
```
00000000 00100000
             ^
             bit 5
```
If it is full...
```
TXFF = 1 
```
which means that the buffer cannot accept another byte of data yet.

### UART Initialization
The `PL011` UART has a handful of useful registers
<table>
  <thead>
    <tr>
      <th>UART Register</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>UART_CR</td>
      <td>Control Register</td>
    </tr>
    <tr>
      <td>UART_IBRD</td>
      <td>Integer Baud Rate</td>
    </tr>
    <tr>
      <td>UART_FBRD</td>
      <td>Fractional Baud Rate</td>
    </tr>
    <tr>
      <td>UART_LCRH</td>
      <td>Line Control</td>
    </tr>
    <tr>
      <td>UART_IMSC</td>
      <td>Interrupt Mask</td>
    </tr>
  </tbody>
</table>

#### IMPORTANT 
There's a complication that's specific to the Raspberry Pi. The UART isn't 
just the UART. Before it can send data on physical pins, GPIO pins 14 and 15 
have to be switched into an alternate function (ALT0). By default, they're 
just ordinary GPIO pins.

## GPIO Driver
The GPIO peripheral starts at `0x3F200000`. Within that peripheral are 
several registers.
<table>
  <thead>
    <tr>
      <th>Offset</th>
      <th>Register</th>
      <th>Purpose</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>0x00</td>
      <td>GPFSEL0</td>
      <td>Pin Function Select (0-9)</td>
    </tr>
    <tr>
      <td>0x04</td>
      <td>GPFSEL1</td>
      <td>Pin Function Select (10-19)</td>
    </tr>
    <tr>
      <td>0x08</td>
      <td>GPFSEL2</td>
      <td>Pin Function Select (20-29)</td>
    </tr>
    <tr>
      <td>...</td>
      <td>...</td>
      <td>...</td>
    </tr>
    <tr>
      <td>0x1C</td>
      <td>GPSET0</td>
      <td>Set output HIGH</td>
    </tr>
    <tr>
      <td>0x28</td>
      <td>GPCLR0</td>
      <td>Set output LOW</td>
    </tr>
    <tr>
      <td>0x34</td>
      <td>GPLEV0</td>
      <td>Read pin state</td>
    </tr>
  </tbody>
</table>   

Notice that there isn't a register for every GPIO pin. Instead, one register 
controls multiple pins. Each GPIO pin gets 3 bits.
```
GPIO 0  -> bits 0-2
GPIO 1  -> bits 3-5
GPIO 2  -> bits 6-8
...
GPIO 9  -> bits 27-29
```
So one 32-bit register controls ten pins. 
```
GPFSEL1

31                              0
|---|---|---|---|---|---|---|---|

 GPIO19
 GPIO18
 GPIO17
 GPIO16
 GPIO15
 GPIO14
 GPIO13
 GPIO12
 GPIO11
 GPIO10
 ...
```
Each field is 3-bits wide. Those 3 bits determine what the pin does.
```
000 = Input
001 = Output
100 = ALT0
101 = ALT1
110 = ALT2
111 = ALT3
011 = ALT4
010 = ALT5
```
The alternative functions connect peripherals like UART, SPI, I^2C, PWM, etc.

### GPIO API
Our GPIO API is setup to be used like this:
```c
gpio_set_function(17, GPIO_OUTPUT);

gpio_set_high(17);

gpio_set_low(17);

bool state = gpio_read(17);
```

Determining which register should be called to interact with a GPIO pin can be 
a bit tricky. To know which register should be modified when calling 
`gpio_set_function(17, GPIO_OUTPUT);`, we can do some floor division:
```
floor(17 / 10) = 1 
```
This means that register 1 (`GPFSEL1`) controls pins 10 through 19, and by 
extension, pin 17. We also need to determine which of the 3 bits of register
1 are controlled by GPIO17.
```
17 % 10 = 7
```
Pin 17 is the **8th field** inside **GPFSEL1** (don't forget that 0 is a valid
field as well, which is why 7 is the 8th field). Each field is 3 bits, so:
```
7 x 3 = 21 
```
which means that GPIO17 occupies bits 21-23. This logic is implemented in 
`gpio.c` as: 
```c
reg = pin / 10;
shift = (pin % 10) * 3;
```


