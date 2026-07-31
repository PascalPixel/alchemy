#include "types.h"

/*
 * Core-drive row 0x080f9a80, 64 bytes (census kind nonstandard_thumb_call_module
 * for the row's own entry point, but the tracked asm/080f9a80.s packs four
 * functions sharing tails, per that file's own comment: "080f9a98 and
 * 080f9a9a share a tail, so keep as one source unit"). Semantic-only: the
 * reference uses `mov ip,lr` / `bx ip` to dodge a stack push for the return
 * address (an ordinary compiler optimisation on a leaf-ish function that
 * still calls out), which an ordinary C function compiles differently but
 * behaves identically to -- not a cannot-express case, just not yet
 * byte-exact.
 *
 * Func_080f9a9a(candidate, passthrough): the shared validator. Transcribed
 * branch-for-branch rather than algebraically simplified, even though the
 * middle and final arms turn out to be reachable only when the first two
 * tests already imply the same outcome (candidate >= Data_080fb7a0 forces
 * candidate >> 14 nonzero, so the `== 0` arm can never fire once the
 * too-small clamp above it has already been skipped) -- keeping the
 * literal shape rather than asserting that proof in code.
 *   - if the candidate's top 7 bits (>> 25) are set, it looks like a real
 *     ROM/EWRAM address: return passthrough unchanged.
 *   - else if candidate is below Data_080fb7a0's own address, it is too
 *     small to be a real pointer: clear passthrough to 0.
 *   - else (mathematically the fall-through above is unreachable, but
 *     transcribed as the reference has it): candidate >> 14 == 0 returns
 *     passthrough unchanged; otherwise clears it to 0.
 *
 * Func_080f9a98(ptr): reads the byte at ptr, then runs the validator with
 * (ptr, that byte).
 *
 * Func_080f9a80(dest): copies 36 words from the fixed table Data_080fb7a0
 * into dest, each one routed through the validator with the TABLE ELEMENT'S
 * OWN ADDRESS as the candidate (always >= Data_080fb7a0 and always with a
 * set top-7-bits, since the table itself lives at a real ROM address, so in
 * practice every element passes through unchanged) -- transcribed as
 * written rather than simplified to a plain copy.
 *
 * Func_080f9ab4(object): reads the byte cursor at object+64, advances it by
 * one, then runs the validator with (old cursor, byte at old cursor) -- the
 * same "read and advance a byte cursor at +64" idiom the sibling
 * core-drive row 0x080f9b74 performs inline rather than by calling this.
 * Reached by a tail branch (`b.n Func_080f9a9a`, no `bl`), which an
 * ordinary C `return` compiles equivalently for behaviour, if not for the
 * exact branch encoding.
 */

extern u32 Data_080fb7a0[36];

static s32 Validate(u32 candidate, s32 passthrough)
{
    if ((candidate >> 25) != 0) {
        return passthrough;
    }
    if (candidate < (u32)Data_080fb7a0) {
        return 0;
    }
    if ((candidate >> 14) == 0) {
        return passthrough;
    }
    return 0;
}

s32 Func_080f9a9a(u32 candidate, s32 passthrough)
{
    return Validate(candidate, passthrough);
}

s32 Func_080f9a98(u8 *ptr)
{
    return Validate((u32)ptr, (s32)*ptr);
}

void Func_080f9a80(s32 *dest)
{
    s32 i;

    for (i = 0; i < 36; i++) {
        dest[i] = Validate((u32)&Data_080fb7a0[i], (s32)Data_080fb7a0[i]);
    }
}

s32 Func_080f9ab4(u8 *object)
{
    u8 **cursor = (u8 **)(object + 64);
    u8 *old = *cursor;

    *cursor = old + 1;

    return Validate((u32)old, (s32)*old);
}
