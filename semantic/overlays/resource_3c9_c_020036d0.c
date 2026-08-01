typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_3c9 owner at 0x020036d0, 244 bytes: the SPAWNER for the
 * orbiting actor that Func_02003600 drives. Behind two gates it asks
 * the spawner for one record, attaches a data blob, randomises the
 * record's starting angle and radius, and installs Func_02003600 into
 * the record's +108 callback slot -- which is what makes 0x02003600
 * and its counter-driven twin 0x02003660 published rather than called.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + `mov r7, r8 / push {r7}` at
 * 0x020036d0 through the single epilogue `pop {r3} / mov r8, r3 / pop
 * {r5, r6, r7} / pop {r0} / bx r0` at 0x020037b8-0x020037c0, with TWO
 * interior literal pools -- 0x0200370a-0x02003713 and
 * 0x020037a2-0x020037b7 -- both branched over. Next owner's prologue at
 * 0x020037c4. This is exactly the case the span rules name: a pool
 * proves nothing about where a function ends, so the span was measured
 * to the epilogue and the interior pools were STEPPED OVER, never
 * walked. Reading them as instructions would have handed the reader
 * writes to r0-r3 that the hardware never performs.
 *
 * `arg0` is never read before being overwritten (r0 is loaded with the
 * flag id at 0x020036d6 as the first data instruction), so `void` in,
 * void out.
 *
 * Call sites resolved with `bun tools/overlay_call_targets.ts
 * resource_3c9 36d0 37c4`'s `+2` rule: `sites=12 distinct_targets=8
 * veneer=12`. Twelve are transcribed here and twelve is the tool's own
 * count -- the invariant is asserted on the output, not on the reading.
 *
 * Published, not called: this owner's address appears as the in-image
 * spelling 0x0200b6d0 (offset + 0x8000, Thumb bit set) in three words,
 * at overlay offsets 0x307c, 0x3e94 and 0x4778. The first sits inside
 * the still-parked 4,708-byte owner 0x02002360, the other two inside
 * the already drafted 0x02003bc8 and 0x0200423c. So three separate
 * scene drivers can start this effect.
 *
 * The callback install is the structural find, and it corroborates the
 * sibling draft rather than resting on it: the word stored to +108 is
 * 0x0200b601, which is the same published spelling -- 0x3600 + 0x8000
 * with the Thumb bit -- so the slot receives Func_02003600. Every field
 * this row initialises is a field that callback reads: +100 the u16
 * binary angle, +48 the s32 radius base. It does NOT touch +98, and
 * Func_02003600 is precisely the variant of the pair that does not read
 * +98. The counter-driven twin Func_02003660 is installed elsewhere.
 *
 * Uncertainties, all flagged rather than smoothed:
 *   - Func_080000f8 is taken as the argument-free pseudo-random source
 *     it is at every site here (no register is set up before any of the
 *     four calls). Other drafts in the tree show a three-argument site
 *     for the same address; that disagreement is NOT resolved here and
 *     the declaration is left old-style deliberately.
 *   - Func_030003e0 is an IWRAM-relocated routine of unknown purpose,
 *     called with a global loaded from 0x03001e40 and the constant 3.
 *     Nothing here establishes what it decides.
 *   - the masks and shifts are transcribed exactly and NOT rounded to a
 *     tidier equivalent: the angle seed is `random & 0x0ffff000` stored
 *     into a u16, so only the low nibble-aligned bits survive, and that
 *     truncation is in the hardware, not in this reading.
 *   - 0x0200e16c is an in-image data pointer (offset 0x616c) in the
 *     established base + 0x8000 spelling; its contents are unread.
 */

extern s32 Func_080770c0();            /* scene/event flag test */
extern s32 Func_030003e0();            /* IWRAM-relocated, unestablished */
extern u8 *Func_0808a080(s32 index);   /* scene-record accessor */
extern s32 Func_080000f8();            /* pseudo-random source (see above) */
extern u8 *Func_080090c8(s32 kind, s32 a, s32 b, s32 c);   /* spawner */
extern void Func_08009098(u8 *record, const void *blob);
extern void Func_0808a160(u8 *record, s32 mode);
extern s32 Func_08000118(s32 angle);   /* sine of a binary angle */

void Func_020036d0(void)
{
    u8 *anchor;
    u8 *record;
    u8 *attached;
    u32 seed;
    s32 placed;

    if (Func_080770c0(0x236) == 0
        && Func_030003e0(*(s32 *)0x03001e40, 3) != 0) {
        return;
    }

    anchor = Func_0808a080(24);

    if (Func_080770c0(0x236) != 0) {
        seed = (u32)Func_080000f8() << 8;
    } else {
        seed = (u32)Func_080000f8() << 6;
    }

    record = Func_080090c8(142 << 1,
                           *(s32 *)(anchor + 8),
                           (s32)((seed >> 16) << 16)
                               + *(s32 *)(anchor + 12)
                               + (s32)0xffe40000,
                           *(s32 *)(anchor + 16));
    if (record == 0) {
        return;
    }

    attached = *(u8 **)(record + 80);

    Func_08009098(record, (const void *)0x0200e16c);
    Func_0808a160(record, 1);
    *(u8 *)(record + 85) = 0;

    *(u16 *)(record + 100) = (u16)(Func_080000f8() & 0x0ffff000);
    *(u16 *)(record + 102) = 0;

    *(s32 *)(record + 108) = 0x0200b601;   /* -> Func_02003600 */

    placed = Func_080000f8();
    placed = Func_08000118((s32)((u32)((placed << 16) - placed) >> 20));
    *(s32 *)(record + 48) = (placed * 24) >> 16;

    *(u8 *)(attached + 38) = 0;
    *(u8 *)(attached + 9) = (u8)((*(u8 *)(attached + 9) & -13) | 4);
}
