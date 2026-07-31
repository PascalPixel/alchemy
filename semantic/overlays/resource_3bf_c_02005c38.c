typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned long long u64;

/*
 * Resource 3bf soft-float double PACKER at 0x02005c38.
 *
 * This is the routine the sibling conversions call to turn the library's
 * 20-byte unpacked record back into a packed double.  Its record layout is
 * exactly the one 0x02005af0 builds and 0x02005b6c consumes:
 *   +0  class, +4 sign, +8 exponent, +12 significand low, +16 significand high
 * and it uses the same word order those files established: the packed value
 * leaves in r0 = HIGH word, r1 = LOW word, which is NOT the native
 * little-endian layout of a C `double`.  It is therefore carried here as
 * SoftDouble (a u64 whose low half is the r0 word), matching 0x02005a78,
 * 0x02005af0 and 0x02005b6c.
 *
 * The owner is complete: `push {r4-r7, lr} / mov r7,sl / mov r6,r8 /
 * push {r6,r7}` at 0x02005c38 through `pop {r3,r4} / mov r8,r3 / mov sl,r4 /
 * pop {r4,r5,r6,r7,pc}` at 0x02005dcc..0x02005dd3, i.e. exactly the row's 412
 * bytes.  Two literal pools sit inside the span (0x02005d38..0x02005d4f and
 * 0x02005db8..0x02005dcb); both are branched over and are data.  The second
 * one is why the listing appears to run past the epilogue — `overlay_show`
 * decodes 0x02005dca/0x02005dcc as one 32-bit instruction, but 0x02005dca is
 * the top half of the pool word 0xffff800f and 0x02005dcc is `pop {r3,r4}`.
 *
 * Call targets came from `bun tools/overlay_call_targets.ts resource_3bf 5c38`
 * (4 sites, 4 distinct targets, all ordinary prologues of this overlay), never
 * from the disassembler's pc-relative `bl` annotations:
 *   0x02005dd4  class predicate -> quiet-NaN result
 *   0x02005de4  class predicate -> infinity result
 *   0x02005df4  class predicate -> zero result
 *   0x02005c08  the already-converted 64-bit logical right shift
 * The three predicates are named by offset; only their truthiness and the
 * result each selects are evidence here, so no name is invented for them.
 *
 * Shape, in assembly order:
 *   - NaN:      exponent field 0x7ff, significand kept and forced quiet by
 *               OR-ing 0x00080000 into the high word (mantissa bit 51);
 *   - infinity: exponent field 0x7ff, significand zeroed;
 *   - zero:     exponent field 0, significand zeroed (sign still applied);
 *   - a zero significand that reached the arithmetic path also yields zero;
 *   - exponent < -1022 is the subnormal path: the value is right-shifted by
 *     (-1022 - exponent), with a sticky OR of every bit shifted out, and a
 *     shift of more than 56 flushes to zero;
 *   - exponent >= 1024 overflows to infinity;
 *   - otherwise the biased exponent is exponent + 0x3ff.
 *   Both paths then round and renormalise, and the common tail packs.
 *
 * Rounding is round-to-nearest-even on the low 8 guard bits: when
 * (low & 0xff) == 0x80 exactly, 0x80 is added only if bit 8 is already set
 * (odd), otherwise nothing is added; every other case adds 0x7f, which carries
 * only when the guard bits exceed half.  In the assembly a second operand
 * register is compared against zero at both tie tests and is a `movs r2,#0`
 * constant in this build, so those two branches can never be taken; they are
 * folded away here and recorded rather than transcribed as dead code.
 *
 * Renormalisation after rounding differs between the two paths, and that is
 * deliberate in the original: the subnormal path only PROMOTES (a carry past
 * 0x0fffffff means the value became the smallest normal, so the exponent field
 * goes from 0 to 1), while the normal path SHIFTS DOWN by one and increments
 * the biased exponent when the significand carried past 0x1fffffff.
 *
 * The final assembly is done in the original through an 8-byte stack scratch
 * area with a word store, a halfword store at +6 and a byte store at +7.
 * Every bit of the high word is defined by that sequence before it is read
 * back (bits 0-19 by the word store, 20-30 by the halfword store, 31 by the
 * byte store), so nothing uninitialised reaches the result and the same value
 * is produced here by plain bit assembly.
 *
 * Uncertainties:
 *  - `ldr r1,[pc]` at 0x02005c58 loads the pool zero at 0x02005d38 into r1 on
 *    the NaN path and r1 is never read again; it is dead in the original and
 *    is not modelled.  Likewise `movs r4,#0 / orrs r6,r4` at 0x02005cbc and
 *    0x02005cc6 is a no-op.
 *  - the shift helper is called with the raw subnormal distance, which the
 *    guard has already limited to 1..56, so 0x02005c08's undefined-for->=64
 *    region is never reached from here.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exponent;
    u32 low;
    u32 high;
} SoftFloatRecord;

/* Old-style declarations: overlay imports may be reached with different
 * argument counts, so arities are left open. */
s32 Func_02005c08();
s32 Func_02005dd4();
s32 Func_02005de4();
s32 Func_02005df4();

SoftDouble Func_02005c38(SoftFloatRecord *record)
{
    u32 sign = record->sign;
    u32 low = record->low;
    u32 high = record->high;
    u32 biasedExponent = 0;
    u32 packedHigh;

    if (Func_02005dd4(record) != 0) {
        /* Quiet NaN: keep the payload, force mantissa bit 51. */
        high |= 0x00080000u;
        biasedExponent = 0x7ffu;
    } else if (Func_02005de4(record) != 0) {
        biasedExponent = 0x7ffu;
        low = 0u;
        high = 0u;
    } else if (Func_02005df4(record) != 0) {
        low = 0u;
        high = 0u;
    } else if ((high | low) == 0u) {
        low = 0u;
        high = 0u;
    } else {
        s32 exponent = record->exponent;

        if (exponent < -1022) {
            s32 distance = -1022 - exponent;

            if (distance > 56) {
                low = 0u;
                high = 0u;
            } else {
                /* `movs r3,#1 / lsls r3,r2 / subs r3,#1` — a Thumb shift of 32
                 * or more yields 0, so the mask saturates to all ones.  Spelled
                 * out here because `1u << 32` is undefined in C. */
                u32 lowMask = distance >= 32 ? 0xffffffffu
                                             : ((1u << distance) - 1u);
                u32 highMask = (u32)((s32)lowMask >> 31);
                u32 sticky = ((highMask & high) | (lowMask & low)) != 0u ? 1u : 0u;
                u64 shifted;

                shifted = Func_02005c08(((u64)high << 32) | (u64)low,
                                        (u32)distance);
                low = (u32)shifted | sticky;
                high = (u32)(shifted >> 32);
            }

            /* Round to nearest even on the low 8 bits. */
            if ((low & 0xffu) == 0x80u) {
                if ((low & 0x100u) != 0u) {
                    u32 rounded = low + 0x80u;

                    if (rounded < low) {
                        high += 1u;
                    }
                    low = rounded;
                }
            } else {
                u32 rounded = low + 0x7fu;

                if (rounded < low) {
                    high += 1u;
                }
                low = rounded;
            }

            /* Carrying out of the subnormal range makes this the smallest
             * normal, so the exponent field becomes 1 rather than 0. */
            if (high > 0x0fffffffu) {
                biasedExponent = 1u;
            }
        } else if (exponent >= 1024) {
            biasedExponent = 0x7ffu;
            low = 0u;
            high = 0u;
        } else {
            biasedExponent = (u32)(exponent + 0x3ff);

            if ((low & 0xffu) == 0x80u) {
                if ((low & 0x100u) != 0u) {
                    u32 rounded = low + 0x80u;

                    if (rounded < low) {
                        high += 1u;
                    }
                    low = rounded;
                }
            } else {
                u32 rounded = low + 0x7fu;

                if (rounded < low) {
                    high += 1u;
                }
                low = rounded;
            }

            if (high > 0x1fffffffu) {
                low = (high << 31) | (low >> 1);
                high >>= 1;
                biasedExponent += 1u;
            }
        }

        /* Common tail for every arithmetic path: drop the 8 guard bits. */
        low = (high << 24) | (low >> 8);
        high >>= 8;
    }

    packedHigh = high & 0x000fffffu;
    packedHigh |= (biasedExponent & 0x7ffu) << 20;
    packedHigh |= sign << 31;

    /* r0 = high word, r1 = low word. */
    return (SoftDouble)packedHigh | ((SoftDouble)low << 32);
}
