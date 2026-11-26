/// @file
/// Every piece of assembly the C holds: a few macros and inlines written once
/// and used by name, each a fake match. No other file has an asm statement.
///
/// A file that owns one of the inlines defines its ASM_PART_ name and includes
/// this file where the inline stood, so that each unit still meets its
/// definitions in the order it did; the macros come once.

#ifdef ASM_PART_DMA
#undef ASM_PART_DMA
/// Starts a transfer on DMA channel `channel`: its source, destination and
/// control written in one store.
static __inline__ void Dma_Set(const void *source, void *destination,
                               u32 control, volatile u32 *channel) {
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

/// The unit matrix's three values set up in Matrix_SetCurrentUnit's order,
/// which differs from Matrix_SetUnit's.
#define MATRIX_UNIT_REGISTER_SETUP                                             \
  "mov r0, %1\n\tmov r1, #128\n\tlsl r1, r1, #9\n\tmov r2, #0\n\t"             \
  "mov %0, #0\n\tmov r4, #0\n\t"

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
/// Multiplies 16.16 fixed-point values, returning the middle 32 product bits:
/// IwramMulFX's three instructions written in place.
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

#ifdef ASM_PART_BIOS_STOP
#undef ASM_PART_BIOS_STOP
/// Stops the CPU and the screen until an enabled interrupt wakes them (the BIOS
/// Stop call): the sleep that L, R and Select end.
static __inline__ void Bios_Stop(void) {
  /* FAKEMATCH(asm): handwritten assembly: BIOS Stop has no C instruction. */
  __asm__ volatile("swi 3");
}
#endif

#ifdef ASM_PART_COPY_16
#undef ASM_PART_COPY_16
/// Copies 16 bytes from `source` to `destination` with one four-register load
/// and store.
static __inline__ void Copy16Bytes(const void *source,
                                   volatile void *destination) {
  /* FAKEMATCH(asm): src pinned to r0 by an asm register name. */
  register const void *src __asm__("r0") = source;
  /* FAKEMATCH(asm): dst pinned to r1 by an asm register name. */
  register volatile void *dst __asm__("r1") = destination;
  /* FAKEMATCH(asm): handwritten assembly: ldmia and stmia of four registers:
   * agscc's block moves stop at three, so a 16-byte struct copy compiles to 12
   * and 4. */
  __asm__ volatile("ldmia %0!, {r2, r3, r4, r5}\n\tstmia %1!, {r2, r3, r4, r5}"
                   : "+r"(src), "+r"(dst)
                   :
                   : "r2", "r3", "r4", "r5", "memory");
}
#endif

#ifdef ASM_PART_COPY_WORDS
#undef ASM_PART_COPY_WORDS
/// Copies two words from source to destination with one block load and store.
static __inline__ void CopyWords2(const void *source, void *destination) {
  /* FAKEMATCH(asm): from pinned to r0 by an asm register name. */
  register const void *from __asm__("r0") = source;
  /* FAKEMATCH(asm): to pinned to r1 by an asm register name. */
  register void *to __asm__("r1") = destination;
  /* FAKEMATCH(asm): handwritten assembly: ldmia and stmia of two registers from
   * r0 to r1 with scratch from r2 up, in the shape of Dma_Set: agscc's block
   * moves leave r0 and r1 to the allocator. */
  __asm__ volatile("ldmia %0!, {r2, r3}\n\tstmia %1!, {r2, r3}"
                   : "+r"(from), "+r"(to)
                   :
                   : "r2", "r3", "memory");
}

/// Copies three words from source to destination with one block load and store.
static __inline__ void CopyWords3(const void *source, void *destination) {
  /* FAKEMATCH(asm): from pinned to r0 by an asm register name. */
  register const void *from __asm__("r0") = source;
  /* FAKEMATCH(asm): to pinned to r1 by an asm register name. */
  register void *to __asm__("r1") = destination;
  /* FAKEMATCH(asm): handwritten assembly: ldmia and stmia of three registers
   * from r0 to r1 with scratch from r2 up, in the shape of Dma_Set: agscc's
   * block moves leave r0 and r1 to the allocator. */
  __asm__ volatile("ldmia %0!, {r2, r3, r4}\n\tstmia %1!, {r2, r3, r4}"
                   : "+r"(from), "+r"(to)
                   :
                   : "r2", "r3", "r4", "memory");
}

/// Copies four words from source to destination with one block load and store.
static __inline__ void CopyWords4(const void *source, void *destination) {
  /* FAKEMATCH(asm): from pinned to r0 by an asm register name. */
  register const void *from __asm__("r0") = source;
  /* FAKEMATCH(asm): to pinned to r1 by an asm register name. */
  register void *to __asm__("r1") = destination;
  /* FAKEMATCH(asm): handwritten assembly: ldmia and stmia of four registers:
   * agscc's block moves stop at three, so a 16-byte struct copy compiles to 12
   * and 4. */
  __asm__ volatile("ldmia %0!, {r2, r3, r4, r5}\n\tstmia %1!, {r2, r3, r4, r5}"
                   : "+r"(from), "+r"(to)
                   :
                   : "r2", "r3", "r4", "r5", "memory");
}
#endif

#ifdef ASM_PART_STORE_UNIT
#undef ASM_PART_STORE_UNIT
/// Stores the unit matrix at `matrix`: three runs of 1.0 and three zeros, the
/// rotation's diagonal and a translation of 0.
static __inline__ void Matrix_StoreUnit(void *matrix) {
  /* FAKEMATCH(asm): handwritten assembly: three stmia no C store or copy
   * compiles to. */
  __asm__ volatile(
      "mov r0, %0\n\tmov r1, #128\n\tlsl r1, r1, #9\n\tmov r2, #0\n\t"
      "mov r3, #0\n\tmov r4, #0\n\t"
      "stmia r0!, {r1, r2, r3, r4}\n\t"
      "stmia r0!, {r1, r2, r3, r4}\n\t"
      "stmia r0!, {r1, r2, r3, r4}"
      :
      : "r"(matrix)
      : "r0", "r1", "r2", "r3", "r4", "memory");
}
#endif

#ifndef ALCHEMY_ASM_MACROS
#define ALCHEMY_ASM_MACROS

/// Jumps to `entry` and does not return: the cartridge restart.
#define CART_JUMP(entry)                                                       \
  /* FAKEMATCH(asm): handwritten assembly: the cartridge restart transfers     \
   * control without returning; the target is in r4 because the jump lists r0  \
   * to r2 as changed while r3 holds REG_IME, which no C call gives. */        \
  __asm__ volatile("bx %0" : : "r"(entry) : "r0", "r1", "r2", "r3", "ip", "lr")

/// Calls a far-call table slot in Thumb through Matrix_CallInThumb, with its
/// target in ip, and leaves the low half of what it returns in `angle`.
#define THUMB_CALL_ARCTAN(angle, xValue, yValue, slot)                         \
  /* FAKEMATCH(asm): x pinned to r0 by an asm register name. */                \
  register s32 x __asm__("r0") = (xValue);                                     \
  /* FAKEMATCH(asm): y pinned to r1 by an asm register name. */                \
  register s32 y __asm__("r1") = (yValue);                                     \
  /* FAKEMATCH(asm): result pinned to r0 by an asm register name. */           \
  register s32 result __asm__("r0");                                           \
                                                                               \
  /* FAKEMATCH(asm): handwritten assembly: ldr ip from the table and bl        \
   * Matrix_CallInThumb, a call through ip that returns in Thumb, which no C   \
   * call compiles to; its list is the call's: r2 to r4, ip, lr, flags and     \
   * memory. */                                                                \
  __asm__ volatile("ldr\tip, %3\n\tbl\tMatrix_CallInThumb"                     \
                   : "=r"(result)                                              \
                   : "r"(x), "r"(y), "m"(slot)                                 \
                   : "r2", "r3", "r4", "ip", "lr", "cc", "memory");            \
  (angle) = (u16)result

#endif
