typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_3c9 owner at 0x02003660, 112 bytes: a per-frame orbit step
 * for one scene actor -- it reads a binary angle out of the actor's own
 * halfword at +100, places the actor on a circle around scene record
 * 23, mirrors the placement into a second pair of fields, and advances
 * the angle by -0x800 (1/32 turn) ready for the next frame.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + `mov r7, sl / mov r6, r8 /
 * push {r6, r7}` at 0x02003660 through the single epilogue `pop {r3,
 * r5} / mov r8, r3 / mov sl, r5 / pop {r5, r6, r7} / pop {r0} / bx r0`
 * at 0x020036be-0x020036c8, one alignment halfword at 0x020036ca, then
 * a one-word literal pool at 0x020036cc (0xfffff800). Next owner's
 * prologue at 0x020036d0. Span measured to the epilogue, not to the
 * pool. `arg0` is read (the +100 halfword) before being written, so it
 * is a real argument; void return.
 *
 * Not found by the structural inventory walk: this is a PUBLISHED
 * callback, not a `bl` target. Its address is stored as the in-image
 * spelling 0x0200b660 (offset + 0x8000, Thumb bit set) in exactly one
 * word, at overlay offset 0x5984 -- inside the span of the still
 * undrafted owner 0x020056a0, which is therefore its installer.
 *
 * Call sites resolved with `bun tools/overlay_call_targets.ts
 * resource_3c9 3660 36d0`'s `+2` rule: sites=3, all three veneers, and
 * the three transcribed here are those three. Never read a `bl` target
 * off the raw listing in this overlay (see the note in the 0x020037c4
 * draft for what that error looks like when it reaches prose).
 *
 * NEAR-TWIN, and it was diffed before either was written: the exact-C
 * owner immediately before it, Func_02003600 (assets/code/
 * resource_3c9_c_02003600.c), is the same body with the same field set
 * and the same -0x800 angle step. Three things genuinely differ and
 * none of them should be folded away:
 *   - scene-record index 23 here, 24 there;
 *   - the +8 arm's radius is (+48 field) + (+98 byte) + 6 here, and
 *     (+48 field) + 3 there -- this variant is driven by the +98 byte
 *     counter, that one is not;
 *   - the +16 arm's radius is (+98 byte) + 4 here, and a bare `<< 1`
 *     (i.e. a constant 2) there.
 * So the pair is a plain and a counter-driven version of one orbit
 * step. Sibling 0x020037c4 in the same cluster reads the +56/+64 pair
 * this row writes.
 *
 * Trig helper identification follows the established reading of the
 * main-image bank (resource_378's 0x020026f0 and resource_381's
 * 0x0200301c both pair them this way): Func_08000118 is the sine of a
 * binary angle and Func_08000120 the cosine, and here as there the
 * cosine result lands in the +8 field and the sine result in the +16
 * field, via record fields +8 and +16 of the anchor.
 *
 * Uncertainties: the actor layout is raw offsets only -- +8/+16 are the
 * live position pair, +56/+64 a mirror of it, +48 an s32 radius base,
 * +98 a u8 counter, +100 the u16 angle. Nothing establishes which of
 * +8/+16 is which world axis, so they are not named x/y/z here.
 */

extern u8 *Func_0808a080(s32 index);   /* scene-record accessor */
extern s32 Func_08000118(s32 angle);   /* sine of a binary angle */
extern s32 Func_08000120(s32 angle);   /* cosine of a binary angle */

void Func_02003660(u8 *actor)
{
    u8 *anchor = Func_0808a080(23);
    u16 *anglePtr = (u16 *)(actor + 100);
    s32 angle = *anglePtr;
    s32 cosine;
    s32 sine;
    s32 along;
    s32 across;

    cosine = Func_08000120(angle);
    along = *(s32 *)(anchor + 8)
          + (*(s32 *)(actor + 48) + *(u8 *)(actor + 98) + 6) * cosine;
    *(s32 *)(actor + 8) = along;

    sine = Func_08000118(angle);
    across = *(s32 *)(anchor + 16)
           + (*(u8 *)(actor + 98) + 4) * sine;
    *(s32 *)(actor + 16) = across;

    *(s32 *)(actor + 56) = *(s32 *)(actor + 8);
    *(s32 *)(actor + 64) = across;

    *anglePtr = (u16)(*anglePtr + 0xf800);
}
