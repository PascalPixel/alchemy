typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_3c9 owner at 0x02005a28, 360 bytes: the SPAWNER for the
 * orbiting actor that Func_02005b90 drives. It first retargets scene
 * record 23 -- to one of three fixed destinations chosen by a signed
 * halfword at 0x03001e70+234 and a bit of the frame counter at
 * 0x03001e40 -- and then, one frame in sixteen, asks the spawner for a
 * record, attaches a blob, seeds the angle and radius,
 * parks record 23's own pointer in the new record's +104 anchor slot,
 * and installs Func_02005b90 at +108.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + `mov r7, sl / mov r6, r9 /
 * mov r5, r8 / push {r5, r6, r7}` at 0x02005a28 through the single
 * epilogue `pop {r3, r5, r6} / mov r8, r3 / mov r9, r5 / mov sl, r6 /
 * pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02005b80-0x02005b8c. Next
 * owner's prologue at 0x02005b90. TWO interior literal pools --
 * 0x02005aa0-0x02005aab and 0x02005b5c-0x02005b73 -- both branched
 * over, both STEPPED OVER rather than walked. Measured to the epilogue.
 *
 * `arg0` is never read before being overwritten (r0 takes the constant
 * 23 as the first data instruction), so void in, void out. Call sites
 * resolved with `bun tools/overlay_call_targets.ts resource_3c9 5a28
 * 5b90`'s `+2` rule: `sites=14 veneer=14`, and fourteen are
 * transcribed -- the tool's count and the transcription agree, which is
 * the invariant asserted on the output.
 *
 * Published, not called: this owner's address appears as the in-image
 * spelling 0x0200da28 in two words, at overlay offsets 0x3bbc and
 * 0x4b1c -- the trailing pools of the already drafted 0x02003924 and
 * 0x020048d8. Two drivers can start this effect.
 *
 * NEAR-TWIN OF Func_020036d0, and they were diffed against each other
 * before either header was written. Same skeleton -- flag gate, spawn
 * kind 142 << 1, attach blob, `Func_0808a160`, clear +85, random angle
 * masked with 0x0ffff000 into +100, zero +102, a sine-derived radius
 * `(x * 24) >> 16` into +48, install a callback at +108, then clear the
 * attached object's +38 byte and rewrite bits 2-3 of its +9 byte. The
 * real differences:
 *   - the callback installed: 0x0200b601 there (Func_02003600), and
 *     0x0200db91 here (Func_02005b90);
 *   - this one also writes +104 with record 23's pointer, giving its
 *     callback a per-instance anchor; 0x020036d0 writes no anchor and
 *     its callback uses a fixed scene index;
 *   - `Func_0808a160` mode 1 there, 5 here;
 *   - the +9 byte's new bits 2-3 are the CONSTANT 4 there, and here
 *     they are COPIED from record 23's own attached object. Folding the
 *     pair would have turned a copy into a constant.
 *   - this one carries the whole record-23 retarget prologue, which has
 *     no counterpart at all in 0x020036d0.
 *
 * Uncertainties, flagged rather than smoothed:
 *   - 0x03001e40 is the tree's free-running FRAME COUNTER, so `& 1`
 *     alternates the retarget every other frame and `& 15` is a
 *     one-frame-in-sixteen gate on the spawn, NOT a state test. That
 *     correction is taken from HANDOVER rather than guessed here.
 *     0x03001e70 and Func_03000380 stay unestablished;
 *     Func_03000380 is called with the
 *     masked random value and 0x60000 and its result is shifted left 16
 *     before use, which is consistent with a scaling helper but is not
 *     evidence of one.
 *   - Func_080000f8 is taken as the argument-free pseudo-random source
 *     it is at both sites here. The tree holds a three-argument site
 *     for the same address elsewhere; that disagreement is NOT resolved
 *     here and the declaration stays old-style deliberately.
 *   - Func_0808a0f0 is a three-argument retarget on a scene index; the
 *     meaning of 0x1300000 / 0xa40000 / 0xab0000 is not established.
 *   - the branch that fires when the +234 halfword exceeds 129 issues
 *     the retarget with both coordinates ZERO and skips the two field
 *     writes entirely; whether that is a cancel or a home position is
 *     not established.
 */

extern u8 *Func_0808a080(s32 index);   /* scene-record accessor */
extern s32 Func_080000f8();            /* pseudo-random source (see above) */
extern void Func_0808a0f0(s32 index, s32 a, s32 b);
extern u8 *Func_080090c8(s32 kind, s32 a, s32 b, s32 c);   /* spawner */
extern s32 Func_03000380();            /* IWRAM-relocated, unestablished */
extern void Func_08009098(u8 *record, const void *blob);
extern void Func_0808a160(u8 *record, s32 mode);
extern s32 Func_08000118(s32 angle);   /* sine of a binary angle */

void Func_02005a28(void)
{
    u8 *anchor = Func_0808a080(23);
    u8 *state = *(u8 **)0x03001e70;
    u32 spread;
    s32 reach;
    u8 *record;
    u8 *attached;
    u8 *positioned;
    s32 gate;
    s32 placed;

    spread = ((u32)(Func_080000f8() * 48) >> 16) << 16;
    state += 232;

    if (*(short *)(state + 2) > 129) {
        goto reset_position;
    }
    if ((*(s32 *)0x03001e40 & 1) != 0) {
        Func_0808a0f0(23, 152 << 17, 164 << 16);
        reach = 128 << 9;
        positioned = Func_0808a080(23);
    } else {
        Func_0808a0f0(23, 152 << 17, 171 << 16);
        reach = 0x00014ccc;
        positioned = Func_0808a080(23);
    }
    *(s32 *)(positioned + 24) = reach;
    *(s32 *)(Func_0808a080(23) + 28) = reach;
    goto position_done;

reset_position:
    Func_0808a0f0(23, 0, 0);

position_done:

    if (anchor == 0) {
        return;
    }

    gate = *(s32 *)0x03001e40 & 15;
    if (gate != 0) {
        return;
    }

    record = Func_080090c8(142 << 1,
                           *(s32 *)(anchor + 8) + (128 << 12),
                           *(s32 *)(anchor + 12) + (s32)spread + (128 << 12),
                           *(s32 *)(anchor + 16));

    spread = (u32)Func_03000380((s32)spread, 192 << 11) << 16;

    if (record == 0) {
        return;
    }

    attached = *(u8 **)(record + 80);

    Func_08009098(record, (const void *)0x0200e734);
    Func_0808a160(record, 5);
    *(u8 *)(record + 85) = (u8)gate;   /* gate is 0 on this path */

    *(u16 *)(record + 100) = (u16)(Func_080000f8() & 0x0ffff000);
    *(u16 *)(record + 102) = (u16)gate;

    *(s32 *)(record + 104) = (s32)anchor;
    *(s32 *)(record + 108) = 0x0200db91;   /* -> Func_02005b90 */

    placed = Func_08000118((s32)(spread & 0x000fffff) >> 4);
    *(s32 *)(record + 48) = (placed * 24) >> 16;

    *(u8 *)(attached + 38) = 0;
    *(u8 *)(attached + 9) =
        (u8)((*(u8 *)(attached + 9) & -13)
             | (*(u8 *)(*(u8 **)(anchor + 80) + 9) & 12));
}
