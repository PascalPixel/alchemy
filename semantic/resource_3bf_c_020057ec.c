#include "types.h"

/*
 * Resource 3bf soft-float ADD/SUBTRACT CORE at 0x020057ec.
 *
 * This is the routine the packed-level subtraction at 0x02005a78 calls after
 * unpacking both operands: it takes two unpacked records and a destination
 * record and returns a pointer to whichever record holds the answer.  The
 * layout is the one 0x02005af0 builds, 0x02005b6c consumes and 0x02005c38
 * packs:
 *   +0 class, +4 sign, +8 exponent, +12 significand low, +16 significand high
 * and the class values are now pinned by this owner's three predicates:
 *   0x02005ac0  class <= 1  (not-a-number)
 *   0x02005ad0  class == 4  (infinity)
 *   0x02005ae0  class == 2  (zero)
 * with class 3 = finite, which is what 0x02005af0 already wrote.  All three
 * predicates are four-instruction leaves visible in full at their addresses.
 *
 * Complete owner: `push {r4-r7,lr} / mov r7,fp / mov r6,sl / mov r5,r9 /
 * mov r4,r8 / push {r4-r7} / sub sp,#8` at 0x020057ec through
 * `add sp,#8 / pop {r3,r4,r5,r6} / mov r8,r3 / mov r9,r4 / mov sl,r5 /
 * mov fp,r6 / pop {r4,r5,r6,r7,pc}` at 0x02005a2a..0x02005a37.  The two pool
 * words at 0x02005a38 and 0x02005a3c close the row's 596 bytes; the next
 * owner, 0x02005a40, starts immediately after.  All four callee-saved high
 * registers are saved and restored explicitly, so r8/r9/sl/fp are live for the
 * whole body: sl = first operand, r9 = second operand, r8 = destination, and
 * lr/ip are used as ordinary scratch for the two exponents.
 *
 * Call targets came from `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3bf 57ec`
 * (9 sites, 4 distinct targets), never from the disassembler's pc-relative
 * `bl` annotations. Eight are the three class predicates. The ninth call, at
 * 0x02005836, targets a prologue-less constant loader and is worth spelling
 * out because it was previously omitted from the semantic source-call order:
 *
 *     02005ab8:  ldr r0,[pc,#0]   ; loads the following word
 *     02005aba:  bx  lr
 *     02005abc:  .word 0x0200df90
 *
 * There is no `push`, so `bx lr` returns straight to the instruction after the
 * `bl` — a complete two-instruction CONSTANT-LOADER leaf. Its effect is
 * exactly `r0 = 0x0200df90`, and the site immediately branches to the
 * epilogue, so the infinity-minus-infinity case returns that fixed record.
 * 0x0200df90 is inside this overlay's proven in-image band, i.e. the module's
 * own data at file offset 0x5f90 (the same neighbourhood as the 0x0200dfa4
 * word that 0x02004da0 writes), so it is a shared static NaN record rather
 * than a RAM global.  The tree's convention is kept: the raw pool value is
 * used as the data address while symbol names stay on printed offsets.
 *
 * Algorithm, in assembly order:
 *   - either operand not-a-number returns that operand unchanged;
 *   - infinity + infinity returns the first if the signs agree and the shared
 *     record at 0x0200df90 if they differ; infinity in only one operand
 *     returns that operand;
 *   - zero + zero copies the whole 20-byte first record to the destination and
 *     ANDs the two sign words (so -0 + -0 is -0 and every other combination is
 *     +0); zero in only one operand returns the other;
 *   - otherwise the exponents are aligned.  An absolute difference above 63
 *     flushes the smaller operand's significand to zero outright; otherwise the
 *     smaller one is shifted right one bit at a time with a STICKY OR of the
 *     bit shifted out into bit 0, which is what keeps the later rounding in
 *     0x02005c38 correct;
 *   - equal signs add the significands and keep the sign; different signs
 *     subtract, choosing the operand order from the first operand's sign word,
 *     then take the sign from the 64-bit result being negative and negate the
 *     magnitude;
 *   - the difference is renormalised UP (shift left, exponent down) until the
 *     64-bit significand exceeds 0x0fffffffffffffff, and every path finally
 *     sets class 3 and renormalises DOWN once (shift right with sticky,
 *     exponent up) if the high word carried past 0x1fffffff.
 *
 * Uncertainties:
 *  - the up-normalisation is written as `(value - 1) <= 0x0fffffffffffffff`
 *    rather than `value <= 0x1000000000000000` because that is literally what
 *    the assembly computes: it adds the 64-bit constant -1 and compares the
 *    result against 0x0fffffff / 0xfffffffe.  The two differ at value == 0,
 *    which is exactly the case a difference of equal magnitudes produces, and
 *    the assembly's form is the one that terminates there.  It is kept.
 *  - the shift loops read and rewrite the second operand's significand through
 *    the 8-byte stack slot the prologue reserves; that is a register-allocation
 *    artefact with no observable effect and is modelled as a local.
 *  - the destination record's class is only written on the arithmetic path;
 *    the early returns hand back one of the inputs (or the shared record) and
 *    never touch the destination at all.
 */
typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exponent;
    u32 low;
    u32 high;
} SoftFloatRecord;

/* Old-style declarations: overlay imports may be reached with different
 * argument counts, so arities are left open. */
s32 Func_02005ac0();   /* class <= 1  -> not a number */
s32 Func_02005ad0();   /* class == 4  -> infinity */
s32 Func_02005ae0();   /* class == 2  -> zero */
u8 *Func_02005ab8();   /* shared NaN record getter */

SoftFloatRecord *Func_020057ec(SoftFloatRecord *a, SoftFloatRecord *b,
                               SoftFloatRecord *result)
{
    s32 exponentA;
    s32 exponentB;
    u64 valueA;
    u64 valueB;
    u64 magnitude;
    u32 sign;

    if (Func_02005ac0(a) != 0) {
        return a;
    }
    if (Func_02005ac0(b) != 0) {
        return b;
    }

    if (Func_02005ad0(a) != 0) {
        if (Func_02005ad0(b) == 0) {
            return a;
        }
        if (a->sign == b->sign) {
            return a;
        }
        /* The two-instruction constant loader described above returns the
         * overlay's shared NaN record for infinity minus infinity. */
        return (SoftFloatRecord *)Func_02005ab8();
    }

    if (Func_02005ad0(b) != 0) {
        return b;
    }

    if (Func_02005ae0(b) != 0) {
        if (Func_02005ae0(a) == 0) {
            return a;
        }
        *result = *a;                     /* ldmia/stmia, all five words */
        result->sign = a->sign & b->sign;
        return result;
    }

    if (Func_02005ae0(a) != 0) {
        return b;
    }

    exponentA = a->exponent;
    exponentB = b->exponent;
    valueA = ((u64)a->high << 32) | (u64)a->low;
    valueB = ((u64)b->high << 32) | (u64)b->low;

    {
        s32 difference = exponentA - exponentB;

        if (difference < 0) {
            difference = -difference;
        }

        if (difference > 63) {
            if (exponentA > exponentB) {
                valueB = 0;
            } else {
                exponentA = exponentB;
                valueA = 0;
            }
        } else {
            s32 count = exponentA - exponentB;

            while (count > 0) {
                valueB = (valueB >> 1) | (valueB & 1u);   /* sticky */
                count -= 1;
            }
            if (exponentA > exponentB) {
                exponentB = exponentA;
            }
            while (exponentB > exponentA) {
                valueA = (valueA >> 1) | (valueA & 1u);   /* sticky */
                exponentA += 1;
            }
        }
    }

    if (a->sign == b->sign) {
        result->sign = a->sign;
        result->exponent = exponentA;
        magnitude = valueA + valueB;
        result->low = (u32)magnitude;
        result->high = (u32)(magnitude >> 32);
    } else {
        u64 difference = valueB - valueA;

        if (a->sign == 0u) {
            difference = valueA - valueB;
        }

        if ((s32)(u32)(difference >> 32) < 0) {
            sign = 1u;
            difference = (u64)0 - difference;
        } else {
            sign = 0u;
        }
        result->sign = sign;
        result->exponent = exponentA;
        result->low = (u32)difference;
        result->high = (u32)(difference >> 32);

        /* Renormalise up.  See the note above on the `- 1` form. */
        magnitude = ((u64)result->high << 32) | (u64)result->low;
        while ((magnitude - 1u) <= 0x0fffffffffffffffULL) {
            magnitude <<= 1;
            result->low = (u32)magnitude;
            result->high = (u32)(magnitude >> 32);
            result->exponent -= 1;
        }
    }

    result->cls = 3u;

    if (result->high > 0x1fffffffu) {
        magnitude = ((u64)result->high << 32) | (u64)result->low;
        magnitude = (magnitude >> 1) | (magnitude & 1u);   /* sticky */
        result->low = (u32)magnitude;
        result->high = (u32)(magnitude >> 32);
        result->exponent += 1;
    }

    return result;
}
