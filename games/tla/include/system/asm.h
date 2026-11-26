#ifndef ALCHEMY_ASM_H
#define ALCHEMY_ASM_H

/// @file
/// Every piece of assembly the C holds, one macro to each instruction sequence
/// and one for a local kept in a register, each a fake match. The helpers that
/// use them are ordinary inlines in their own headers. No other file has an
/// asm statement.

/// Jumps to `entry` and does not return: the cartridge restart.
#define ASM_CART_JUMP(entry)                                                   \
  /* FAKEMATCH(asm): the cartridge restart transfers                           \
   * control without returning; the target is in r4 because the jump lists r0  \
   * to r2 as changed while r3 holds REG_IME, which no C call gives. */        \
  __asm__ volatile("bx %0" : : "r"(entry) : "r0", "r1", "r2", "r3", "ip", "lr")

/// Calls a far-call table slot in Thumb through Matrix_CallInThumb, with its
/// target in ip, and leaves the low half of what it returns in `angle`.
#define ASM_THUMB_CALL_ARCTAN(angle, xValue, yValue, slot)                     \
  /* FAKEMATCH(asm): x pinned to r0 by an asm register name. */                \
  register s32 x __asm__("r0") = (xValue);                                     \
  /* FAKEMATCH(asm): y pinned to r1 by an asm register name. */                \
  register s32 y __asm__("r1") = (yValue);                                     \
  /* FAKEMATCH(asm): result pinned to r0 by an asm register name. */           \
  register s32 result __asm__("r0");                                           \
                                                                               \
  /* FAKEMATCH(asm): ldr ip from the table and bl                              \
   * Matrix_CallInThumb, a call through ip that returns in Thumb, which no C   \
   * call compiles to; its list is the call's: r2 to r4, ip, lr, flags and     \
   * memory. */                                                                \
  __asm__ volatile("ldr\tip, %3\n\tbl\tMatrix_CallInThumb"                     \
                   : "=r"(result)                                              \
                   : "r"(x), "r"(y), "m"(slot)                                 \
                   : "r2", "r3", "r4", "ip", "lr", "cc", "memory");            \
  (angle) = (u16)result

/// Keeps a local in the register named.
#define ASM_REGISTER(declaration, name)                                        \
  /* FAKEMATCH(asm): a local pinned to a register by an asm register name. */  \
  register declaration __asm__(name)

/// Stores source, destination and control into a DMA channel's three registers
/// with one stmia, and steps the channel pointer back.
#define ASM_DMA_STORE(source, control, channel, destination)                   \
  /* FAKEMATCH(asm): stmia of three registers with no                          \
   * paired ldmia, which no agscc pattern emits. */                            \
  __asm__ volatile("stmia %2!, {%0, %3, %1}\n\tsub %2, #12"                    \
                   : "+r"(source), "+r"(control)                               \
                   : "r"(channel), "r"(destination)                            \
                   : "memory", "cc")

/// Stores the unit matrix at a pointer: three runs of 1.0 and three zeros.
#define ASM_MATRIX_UNIT(matrix)                                                \
  /* FAKEMATCH(asm): three stmia no C store or copy                            \
   * compiles to. */                                                           \
  __asm__ volatile(                                                            \
      "mov r0, %0\n\tmov r1, #128\n\tmov r2, #0\n\tmov r3, #0\n\t"             \
      "mov r4, #0\n\tlsl r1, r1, #9\n\t"                                       \
      "stmia r0!, {r1, r2, r3, r4}\n\t"                                        \
      "stmia r0!, {r1, r2, r3, r4}\n\t"                                        \
      "stmia r0!, {r1, r2, r3, r4}"                                            \
      :                                                                        \
      : "r"(matrix)                                                            \
      : "r0", "r1", "r2", "r3", "r4", "memory")

/// ASM_MATRIX_UNIT with its third zero in a register the compiler picks.
#define ASM_MATRIX_UNIT_CURRENT(zero, matrix)                                  \
  /* FAKEMATCH(asm): Matrix_SetUnit's three stmia,                             \
   * applied to the current matrix. */                                         \
  __asm__ volatile(                                                            \
      "mov r0, %1\n\tmov r1, #128\n\tlsl r1, r1, #9\n\tmov r2, #0\n\t"         \
      "mov %0, #0\n\tmov r4, #0\n\t"                                           \
      "stmia r0!, {r1, r2, %0, r4}\n\t"                                        \
      "stmia r0!, {r1, r2, %0, r4}\n\t"                                        \
      "stmia r0!, {r1, r2, %0, r4}"                                            \
      : "=l"(zero)                                                             \
      : "r"(matrix)                                                            \
      : "r0", "r1", "r2", "r4", "memory")

/// The 64-bit product of two words, its halves in any two registers.
#define ASM_SMULL(high, low, left, right)                                      \
  /* FAKEMATCH(asm): smull with RdLo and RdHi in any two                       \
   * registers, even over its inputs, where mulsidi3 pairs them (RdHi = RdLo + \
   * 1) apart from the inputs and smulsi3_highpart discards RdLo; its orr      \
   * takes a fresh output reload register beside two dying inputs and nothing  \
   * is scheduled across it, as only asm statements do. */                     \
  __asm__ volatile("smull %1, %0, %3, %2"                                      \
                   : "=r"(high), "=r"(low)                                     \
                   : "r"(left), "r"(right))

/// The middle 32 bits of a 64-bit product, from its two halves.
#define ASM_ORR_SHIFTED(result, high, low)                                     \
  /* FAKEMATCH(asm): MulFX's next instruction. */                              \
  __asm__ volatile("orr %0, %1, %2, lsr #16"                                   \
                   : "=r"(result)                                              \
                   : "r"(high), "r"(low))

/// Stops the CPU and the screen until an enabled interrupt wakes them (the BIOS
/// Stop call).
#define ASM_BIOS_STOP()                                                        \
  /* FAKEMATCH(asm): BIOS Stop has no C instruction. */                        \
  __asm__ volatile("swi 3")

/// Copies four words with one block load and store.
#define ASM_COPY_WORDS_4(source, destination)                                  \
  /* FAKEMATCH(asm): ldmia and stmia of four registers:                        \
   * agscc's block moves stop at three, so a 16-byte struct copy compiles to   \
   * 12 and 4. */                                                              \
  __asm__ volatile(                                                            \
      "ldmia %0!, {r2, r3, r4, r5}\n\tstmia %1!, {r2, r3, r4, r5}"             \
      : "+r"(source), "+r"(destination)                                        \
      :                                                                        \
      : "r2", "r3", "r4", "r5", "memory")

/// ASM_MATRIX_UNIT with 1.0 built before the zeros.
#define ASM_MATRIX_UNIT_STORE(matrix)                                          \
  /* FAKEMATCH(asm): three stmia no C store or copy                            \
   * compiles to. */                                                           \
  __asm__ volatile(                                                            \
      "mov r0, %0\n\tmov r1, #128\n\tlsl r1, r1, #9\n\tmov r2, #0\n\t"         \
      "mov r3, #0\n\tmov r4, #0\n\t"                                           \
      "stmia r0!, {r1, r2, r3, r4}\n\t"                                        \
      "stmia r0!, {r1, r2, r3, r4}\n\t"                                        \
      "stmia r0!, {r1, r2, r3, r4}"                                            \
      :                                                                        \
      : "r"(matrix)                                                            \
      : "r0", "r1", "r2", "r3", "r4", "memory")

/// Copies two words with one block load and store.
#define ASM_COPY_WORDS_2(source, destination)                                  \
  /* FAKEMATCH(asm): ldmia and stmia of two registers                          \
   * from r0 to r1 with scratch from r2 up, in the shape of Dma_Set: agscc's   \
   * block moves leave r0 and r1 to the allocator. */                          \
  __asm__ volatile("ldmia %0!, {r2, r3}\n\tstmia %1!, {r2, r3}"                \
                   : "+r"(source), "+r"(destination)                           \
                   :                                                           \
                   : "r2", "r3", "memory")

/// Copies three words with one block load and store.
#define ASM_COPY_WORDS_3(source, destination)                                  \
  /* FAKEMATCH(asm): ldmia and stmia of three registers                        \
   * from r0 to r1 with scratch from r2 up, in the shape of Dma_Set: agscc's   \
   * block moves leave r0 and r1 to the allocator. */                          \
  __asm__ volatile("ldmia %0!, {r2, r3, r4}\n\tstmia %1!, {r2, r3, r4}"        \
                   : "+r"(source), "+r"(destination)                           \
                   :                                                           \
                   : "r2", "r3", "r4", "memory")

/// Declares `symbol` as the bytes at the linked name `name`, whatever type the
/// name has elsewhere: how the modern build reaches a fixed address.
#define ASM_LINKED_NAME(symbol, name)                                          \
  /* FAKEMATCH(asm): another name for a linked symbol, by an asm label. */     \
  extern char symbol[] __asm__(name)

/// Hides what `value` holds from the modern compiler, so that it reads it
/// through a register.
#define ASM_OPAQUE(value)                                                      \
  /* FAKEMATCH(barrier): an empty asm that rewrites value; as no asm. */       \
  __asm__("" : "+r"(value))

#endif
