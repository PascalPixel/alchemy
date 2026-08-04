#include "types.h"

/*
 * Resource 370 six-bit payload encoder at 0x020011e4.
 *
 * Complete owner: the three-part prologue at 0x020011e4 (`push {r5, r6, r7,
 * lr}`, then fp/sl/r9 and r8 saved through r5-r7, then `sub sp, #8`) through
 * the matching unwind and `pop {r1} / bx r1` at 0x020012b2.  The return
 * address is popped into r1, so r0 survives and IS the result (HANDOVER
 * section 0, epilogue rule): the owner returns the number of symbols written.
 *
 * The span contains no literal pool -- every instruction from the prologue is
 * reached by the control-flow walk -- and no call sites at all, matching the
 * inventory's calls=0 and `bun tools/overlay_call_targets.ts resource_370`,
 * which reports no site in this range.
 *
 * Three phases:
 *  1. Whitening in place.  The last input byte is used as a key and XORed over
 *     the preceding length-1 bytes.  The key byte itself is left alone.
 *  2. Six-bit packing.  Bits are consumed most-significant-first across the
 *     whole (whitened) buffer, six at a time, each group written as one output
 *     byte.  A short final group is padded with the zero bits it never wrote.
 *     After every nine data symbols, a tenth symbol carrying the low six bits
 *     of their sum is inserted, and the running sum restarts.
 *  3. Position mixing.  Every emitted symbol, checksum symbols included, is
 *     replaced by `(symbol + index) & 63`.
 *
 * The alphabet is therefore 0..63 and the output is self-checking every ten
 * symbols, which together with the id-to-glyph table at 0x020002f4 (which maps
 * small ids onto printable ASCII) reads as a password/code generator.  That
 * pairing is an inference from the two owners' shapes and is not proven here.
 *
 * Notes and uncertainties.
 *  - `str r3, [sp, #0]` at 0x0200124e stores the shift amount to a stack slot
 *    that is never read.  It is a dead store and is not modelled.
 *  - The shift is `asrs`, but its operand is a zero-extended byte and the
 *    shift count is 0..7, so it is an ordinary logical shift.
 *  - A length of 0 is not handled by the code: the whitening loop would run
 *    with a count of -1.  The caller contract is length >= 1.
 *  - The buffer is modified in place by phase 1, so the caller's input is
 *    destroyed; that is what the stores at 0x0200121c do.
 */

s32 Func_020011e4(u8 *data, s32 length, u8 *out)
{
    u8 key;
    u8 *dst;
    s32 i;
    s32 produced;
    s32 group;
    s32 checksum;
    s32 bit;
    s32 consumed;
    s32 value;
    s32 n;

    key = data[length - 1];
    for (i = 0; i != length - 1; i++) {
        data[i] ^= key;
    }

    dst = out;
    produced = 0;
    checksum = 0;
    group = 0;
    bit = 0;
    consumed = 0;

    do {
        value = 0;
        n = 0;
        while (n != 6) {
            s32 sample = (data[consumed] >> (7 - bit)) & 1;

            bit++;
            if (bit == 8) {
                bit = 0;
                consumed++;
            }
            value |= sample << (5 - n);

            /* Input exhausted mid-group: the rest of the group stays zero. */
            if (consumed == length) {
                break;
            }
            n++;
        }

        group++;
        *dst++ = (u8)value;
        produced++;
        checksum += value;

        if (group == 9) {
            *dst++ = (u8)(checksum & 63);
            produced++;
            checksum = 0;
            group = 0;
        }
    } while (consumed != length);

    for (i = 0; i != produced; i++) {
        out[i] = (u8)((out[i] + i) & 63);
    }

    return produced;
}
