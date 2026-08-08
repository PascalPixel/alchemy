#include "types.h"

/*
 * resource_3c9 owner at 0x02005b90, 92 bytes: the THIRD orbit step in
 * this overlay, and the one installed by Func_02005a28. It reads the
 * binary angle from its actor's own +100 halfword, places the actor on
 * a circle around the anchor the spawner parked in the actor's +104
 * slot, mirrors the placement into +56/+64, and advances the angle by
 * -0x200 (1/128 turn).
 *
 * Complete owner: `push {r5, r6, lr}` + `mov r6, sl / mov r5, r8 /
 * push {r5, r6}` at 0x02005b90 through the single epilogue `pop {r3,
 * r5} / mov r8, r3 / mov sl, r5 / pop {r5, r6} / pop {r0} / bx r0` at
 * 0x02005bda-0x02005be4, one alignment halfword at 0x02005be6, then a
 * one-word literal pool at 0x02005be8 (0xfffffe00). Measured to the
 * epilogue.
 *
 * WHAT FOLLOWS IT IS NOT AN OWNER, and that is worth recording because
 * this is the LAST owner on resource_3c9. At 0x02005bec the overlay's
 * `ldr r4, [pc, #0] / bx r4` VENEER BANK begins -- 0x03000380,
 * 0x030003e0, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9 and on --
 * followed by this overlay's data tables out to the image end at
 * 0x02006760. So the span recorded here stops at 0x02005bec.
 *
 * `arg0` is read (the +100 halfword) before being overwritten, so it is
 * a real argument; void return. Call sites resolved with `bun
 * cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3c9 5b90 5c1c`'s `+2` rule:
 * `sites=2 veneer=2`, and two are transcribed.
 *
 * Published, not called: the address appears as the in-image spelling
 * 0x0200db91 (offset + 0x8000, Thumb bit set) in exactly one word, at
 * overlay offset 0x5b70 -- inside the span of Func_02005a28, which is
 * therefore its installer, and which writes that word into a spawned
 * record's +108 callback slot.
 *
 * THREE-WAY DIFF, done before this row was written. resource_3c9 now
 * holds three copies of one orbit step -- exact-C Func_02003600,
 * semantic Func_02003660, and this one -- and none of them should be
 * folded into the others:
 *   - the ANCHOR: 0x3600 and 0x3660 fetch it with Func_0808a080(24)
 *     and (23); this one takes it from the actor's own +104 field,
 *     which is exactly the field its spawner writes. That is what makes
 *     this the per-instance variant rather than a fixed-slot one.
 *   - the +8 arm's radius: (+48) + 3, (+48) + (+98 byte) + 6, and here
 *     (+48) + 28.
 *   - the +16 arm: the other two add the anchor's own +16 field; this
 *     one does NOT read the anchor at all, and writes an absolute
 *     (sine << 4) + 0xa40000. Its actor rides at a fixed height.
 *   - the angle step: -0x800 on both of the others, -0x200 here.
 * Identical shape, four genuinely different behaviours.
 *
 * Uncertainties: the anchor pointer in +104 is trusted to be an object
 * of the same family as the Func_0808a080 records only because +8 is
 * read off it at the same offset the twins use; nothing here proves the
 * layouts are the same. The constant 0xa40000 is transcribed as
 * 164 << 16 because that is how the row builds it.
 */

extern s32 Func_08000118(s32 angle);   /* sine of a binary angle */
extern s32 Func_08000120(s32 angle);   /* cosine of a binary angle */

void Func_02005b90(u8 *actor)
{
    u16 *anglePtr = (u16 *)(actor + 100);
    s32 angle = *anglePtr;
    u8 *anchor = *(u8 **)(actor + 104);
    s32 mirror;
    s32 across;

    *(s32 *)(actor + 8) = *(s32 *)(anchor + 8)
                        + Func_08000120(angle) * (*(s32 *)(actor + 48) + 28);

    across = (Func_08000118(angle) << 4) + (164 << 16);
    *(s32 *)(actor + 16) = across;
    *(s32 *)(actor + 56) = *(s32 *)(actor + 8);
    *(s32 *)(actor + 64) = across;

    {
        s32 next = *anglePtr;
        next = next + (s32)0xfffffe00;
        *anglePtr = (u16)next;
    }
}
