//===---------------------------------------------------------------------===//
// WDC65816 backend.
//===---------------------------------------------------------------------===//

DISCLAIMER: I don't know what I'm doing.

Misc thoughts:

So, 65816 has 16-bit words, but addresses _should_ be 24-bits. Most addressing
modes would be selected as optimizations, effectively, either because the
instructions run faster, or to avoid juggling data through the accumulator?

ditto, usage for index registers.


Besides that, for compiling to SNES, some definition of the memory map must be created
-need to know, during linking (elf generation?) which regions are okay for ROM,
which for RAM, etc. Determines where to put 'text' and 'data' sections in address
space (And also where it should exist in an output ROM)

(effectively, we should be looking at the compiler/linker output being something that 
is conducted across the full 16M addr space, then doing an extra translation of that
into a single rom file)



Regarding addressing...

one late optimization may be for writing to addresses that are "$00xxyy". Setting
the DIRECT register to "$xx00" and changing "$yy" (part of the op) are the FASTEST
possible memory accesses! Would be useful for setting data such as a grip of dma 
registers
(might be the only practical app of "direct", when we KNOW that the addr is 16b)