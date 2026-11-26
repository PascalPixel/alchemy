/// @file
/// Every piece of assembly the C holds: a few macros and inlines written once
/// and used by name, each a fake match. No other file has an asm statement.
///
/// A file that owns one of the inlines defines its ASM_PART_ name and includes
/// this file where the inline stood, so that each unit still meets its
/// definitions in the order it did; the macros come once.

#ifdef ASM_PART_DMA
#undef ASM_PART_DMA
/// Starts a DMA transfer: stores the source, destination and control words
/// into `channel`'s registers with one stmia.
static __inline__ void Dma_Set(const volatile void *source,
                               volatile void *destination, u32 control,
                               volatile u32 *channel) {
  /* FAKEMATCH(asm): dma pinned to r3 by an asm register name. */
  register volatile u32 *dma __asm__("r3") = channel;
  /* FAKEMATCH(asm): src pinned to r0 by an asm register name. */
  register u32 src __asm__("r0") = (u32)source;
  /* FAKEMATCH(asm): dst pinned to r1 by an asm register name. */
  register u32 dst __asm__("r1") = (u32)destination;
  /* FAKEMATCH(asm): cnt pinned to r2 by an asm register name. */
  register u32 cnt __asm__("r2") = control;
  /* FAKEMATCH(asm): handwritten assembly: stmia of three registers with no
   * paired ldmia, which no agscc pattern emits. */
  __asm__ volatile("stmia %2!, {%0, %3, %1}\n\tsub %2, #12"
                   : "+r"(src), "+r"(cnt)
                   : "r"(dma), "r"(dst)
                   : "memory", "cc");
}
#endif

#ifdef ASM_PART_IWRAM_CALL
#undef ASM_PART_IWRAM_CALL
#ifdef MODERN
/// Calls an IWRAM routine that returns through ip instead of lr, with left and
/// right in r0 and r1, and returns its r0.
/* FAKEMATCH(asm): handwritten assembly: a call that returns through ip, which
 * no procedure-call standard produces. */
static inline s32 Iwram_CallReturnIp(s32 left, s32 right, void *routine) {
  s32 result;

  __asm__ volatile("mov r0, %1\n\t"
                   "mov r1, %2\n\t"
                   ".align 2\n\t"
                   "mov ip, pc\n\t"
                   "bx %3\n\t"
                   "mov %0, r0"
                   : "=r"(result)
                   : "r"(left), "r"(right), "r"(routine)
                   : "r0", "r1", "r2", "ip", "cc");
  return result;
}
#else
static __inline__ s32 Iwram_CallReturnIp(s32 left, s32 right, void *routine) {
  /* FAKEMATCH(asm): result pinned to r0 by an asm register name. */
  register s32 result __asm__("r0") = left;
  /* FAKEMATCH(asm): factor pinned to r1 by an asm register name. */
  register s32 factor __asm__("r1") = right;

  /* FAKEMATCH(asm): handwritten assembly: a call that returns through ip, which
   * no procedure-call standard produces. */
  __asm__ volatile(".align 2\n\t"
                   "mov ip, pc\n\t"
                   "bx %2\n\t"
                   : "+r"(result)
                   : "r"(factor), "r"(routine)
                   : "r1", "r2", "ip", "cc");
  return result;
}
#endif
#endif

#ifdef ASM_PART_MATRIX
#undef ASM_PART_MATRIX
/// Sets a twelve-word matrix to the unit matrix: FIXED_ONE (1.0) on the
/// rotation's diagonal, the rest 0.
static __inline__ void Matrix_SetUnit(void *matrix) {
  /* FAKEMATCH(asm): handwritten assembly: three stmia no C store or copy
   * compiles to. */
  __asm__ volatile("mov r0, %0\n\tmov r1, #128\n\tmov r2, #0\n\tmov r3, #0\n\t"
                   "mov r4, #0\n\tlsl r1, r1, #9\n\t"
                   "stmia r0!, {r1, r2, r3, r4}\n\t"
                   "stmia r0!, {r1, r2, r3, r4}\n\t"
                   "stmia r0!, {r1, r2, r3, r4}"
                   :
                   : "r"(matrix)
                   : "r0", "r1", "r2", "r3", "r4", "memory");
}

/// Matrix_SetUnit's register setup, with the zero register left to the
/// caller's operand.
#define MATRIX_UNIT_REGISTER_SETUP                                             \
  "mov r0, %1\n\tmov r1, #128\n\tmov r2, #0\n\tmov %0, #0\n\t"                 \
  "mov r4, #0\n\tlsl r1, r1, #9\n\t"

/// Resets the current matrix to the unit matrix.
static __inline__ void Matrix_SetCurrentUnit(void) {
  s32 zero;

  /* FAKEMATCH(asm): handwritten assembly: Matrix_SetUnit's three stmia, applied
   * to the current matrix. */
  __asm__ volatile(MATRIX_UNIT_REGISTER_SETUP "stmia r0!, {r1, r2, %0, r4}\n\t"
                                              "stmia r0!, {r1, r2, %0, r4}\n\t"
                                              "stmia r0!, {r1, r2, %0, r4}"
                   : "=l"(zero)
                   : "r"(gCurrentMatrix)
                   : "r0", "r1", "r2", "r4", "memory");
}

#undef MATRIX_UNIT_REGISTER_SETUP
#endif

#ifdef ASM_PART_MUL_FX
#undef ASM_PART_MUL_FX
/// Multiplies two 16.16 fixed-point values, returning the middle 32 bits of
/// the product: IwramMulFX written out where it is called.
static __inline__ FIXED MulFX(FIXED left, FIXED right) {
  u32 low;
  s32 high;
  s32 result;

  /* FAKEMATCH(asm): handwritten assembly: smull with RdLo and RdHi in any two
   * registers, even over its inputs, where mulsidi3 pairs them (RdHi = RdLo +
   * 1) apart from the inputs and smulsi3_highpart discards RdLo; its orr takes
   * a fresh output reload register beside two dying inputs and nothing is
   * scheduled across it, as only asm statements do. */
  __asm__ volatile("smull %1, %0, %3, %2"
                   : "=r"(high), "=r"(low)
                   : "r"(left), "r"(right));
  high = INTtoFIXED(high);
  /* FAKEMATCH(asm): handwritten assembly: MulFX's next instruction. */
  __asm__ volatile("orr %0, %1, %2, lsr #16"
                   : "=r"(result)
                   : "r"(high), "r"(low));
  return result;
}
#endif

#ifndef ALCHEMY_ASM_MACROS
#define ALCHEMY_ASM_MACROS

/// Halts the CPU until an interrupt (the BIOS Halt call).
#define BIOS_HALT()                                                            \
  /* FAKEMATCH(asm): handwritten assembly: Halt, which no C compiles to. */    \
  __asm__ volatile("swi 2")

/// Stops the CPU and the screen until an enabled interrupt wakes them (the BIOS
/// Stop call).
#define BIOS_STOP()                                                            \
  /* FAKEMATCH(asm): handwritten assembly: Stop, which no C compiles to. */    \
  __asm__ volatile("swi 3")

/// Reads the hardware stack pointer into `variable`.
#define STACK_POINTER_GET(variable)                                            \
  /* FAKEMATCH(asm): handwritten assembly: a read of sp. */                    \
  __asm__ volatile("mov %0, sp" : "=r"(variable))

/// Moves the hardware stack pointer to `value`.
#define STACK_POINTER_SET(value)                                               \
  /* FAKEMATCH(asm): handwritten assembly: a write to sp no C can express. */  \
  __asm__ volatile("mov sp, %0" : : "r"(value) : "memory")

#endif
