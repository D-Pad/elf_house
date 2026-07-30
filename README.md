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


## Assembly Instructions in boot.S File
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

