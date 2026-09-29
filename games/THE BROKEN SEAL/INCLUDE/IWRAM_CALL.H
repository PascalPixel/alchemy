#ifndef ALCHEMY_IWRAM_CALL_H
#define ALCHEMY_IWRAM_CALL_H

#include "TYPES.H"

/* The IWRAM routines return through ip, so callers establish that link in
 * Thumb before entering their ARM bodies. The routine is the last argument:
 * GCC evaluates arguments right to left, so its address is loaded before the
 * operands are computed, as every call site in the ROM shows. The routine
 * may overwrite r1 like any call, so the ROM sets r1 again before a second
 * call sharing a factor; the template ends in a line break so GCC also counts
 * the halfword .align may insert when it places literal pools. */
static __inline__ s32 Iwram_Call2(s32 left, s32 right, void *routine)
{
    register s32 result __asm__("r0") = left;
    register s32 factor __asm__("r1") = right;

    __asm__ volatile(
        ".align 2\n\t"
        "mov ip, pc\n\t"
        "bx %2\n\t"
        : "+r"(result)
        : "r"(factor), "r"(routine)
        : "r1", "r2", "ip", "cc");
    return result;
}

/* The 16.16 multiply of SYSTEM/RUNTIME_MUL_Q16_IP.S returns through ip, so
 * C enters it only through Iwram_Call2. */
extern u8 IwramMulQ16ReturnIp[];

#define Iwram_MulQ16(left, right) Iwram_Call2((left), (right), IwramMulQ16ReturnIp)

/* The resident runtime's entries that ROM code calls. They run in IWRAM,
 * beyond a ROM bl's reach, and ROM code reaches them as Camelot's did:
 * through their fixed entry addresses, loaded before the arguments. A call
 * through a label compiles to a direct bl instead. The build checks each
 * entry against the routine named beside it. */
#define Iwram_RatioMulQ14 ((s32 (*)(s32 divisor, s32 value))0x0300013c) /* IwramRatioMulQ14 */
#define Iwram_ClearWords ((s32 (*)(void *destination, s32 size))0x03000164) /* IwramClearWords */
#define Iwram_FillWords ((s32 (*)(void *destination, s32 size, u32 value))0x03000168) /* IwramFillWords */
#define Iwram_Sqrt ((s32 (*)(s32 value))0x030001d8) /* IwramSqrt */
#define Iwram_ExpandBitRuns ((s32 (*)(const void *bits, void *destination, s32 value))0x03000214) /* IwramExpandBitRuns */
#define Iwram_TransformVector ((void (*)(const s32 *vector, s32 *result))0x03000250) /* IwramTransformVector */
#define Iwram_TransformMatrix ((void (*)(const s32 *matrix))0x030002c0) /* IwramTransformMatrix */
#define Iwram_SignedDivide ((s32 (*)(s32 dividend, s32 divisor))0x03000380) /* IwramSignedDivide */
#define Iwram_UnsignedDivide ((u32 (*)(u32 dividend, u32 divisor))0x030003f0) /* IwramUnsignedDivide */
#define Iwram_CopyWords ((s32 (*)(void *destination, const void *source, s32 size))0x03001388) /* IwramCopyWords */
#define Iwram_Sqrt ((s32 (*)(s32 value))0x030001d8) /* IwramSqrt */
#define Iwram_RatioMulQ14 ((s32 (*)(s32 divisor, s32 value))0x0300013c) /* IwramRatioMulQ14 */

/* The item menu reaches the copier from the bank's first routine instead,
 * its label plus the copier's offset, which GCC schedules as a symbol:
 * after the arguments. The build checks the offset against both labels. */
extern u8 IwramIrqMain[];
#define Iwram_CopyWordsOffset 0x1388 /* IwramCopyWords - IwramIrqMain */

#endif
