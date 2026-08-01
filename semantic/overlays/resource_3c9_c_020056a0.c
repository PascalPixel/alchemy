typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_3c9 owner at 0x020056a0, 848 bytes: the overlay's SCENE
 * STATE MACHINE. It reads a step counter, dispatches through a
 * 49-entry jump table, runs one short arm, and then -- only for the
 * arms that ask for it -- runs a shared spawn tail and a fixed
 * six-record epilogue.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + `mov r7, sl / mov r6, r8 /
 * push {r6, r7}` at 0x020056a0 through the single epilogue `pop {r3,
 * r5} / mov r8, r3 / mov sl, r5 / pop {r5, r6, r7} / pop {r0} / bx r0`
 * at 0x020059e0-0x020059ea, then a one-word literal pool at
 * 0x020059ec. Next owner's prologue at 0x020059f0. FOUR interior data
 * regions, all branched or jumped over and every one of them STEPPED
 * OVER rather than walked: the 196-byte jump table at
 * 0x020056c4-0x02005787 and literal pools at 0x020058b0-0x020058cf,
 * 0x02005970-0x02005987 and 0x020059ec.
 *
 * `arg0` is never read before being overwritten (r0 takes the constant
 * 23 at 0x020056a8), so void in, void out.
 *
 * Call sites resolved with `bun tools/overlay_call_targets.ts
 * resource_3c9 56a0 59f0`'s `+2` rule: `sites=44 distinct_targets=15
 * veneer=37 unknown=1 prologue=6`. Forty-four `bl` lines are
 * transcribed and forty-four is the tool's count, with the single
 * `unknown` RULED rather than waved through: it is 0x02005688, the
 * push-less leaf helper this overlay's already drafted 0x020059f0
 * identified and declared for exactly this reason. 37 + 6 + 1 = 44.
 *
 * Published, not called: the address appears as the in-image spelling
 * 0x0200d6a0 in exactly one word, at overlay offset 0x48c0 -- inside
 * the already drafted 0x0200423c, which is therefore its installer.
 *
 * THE JUMP TABLE, decoded from the image bytes and not from a listing
 * of them. `ldr r2, =0x0200d6c4` is image offset 0x56c4 in the
 * base + 0x8000 spelling; each entry decodes the same way. 49 entries,
 * states 0 through 48, collapsing to TEN arms:
 *
 *   0x5788  state 0            0x584e  state 26
 *   0x57a2  state 8            0x5896  states 27-34
 *   0x57b4  state 16           0x589a  state 36
 *   0x57c6  state 24           0x58d0  state 48
 *   0x57ee  state 25           0x58dc  the other 33 states (fall through)
 *
 * A state above 48 takes the same fall-through path, so the table is
 * total. Only states 25, 26 and 27-34 can set the spawn flag; every
 * other state runs its arm and skips the tail.
 *
 * A DEAD STORE, transcribed as observed. At 0x0200594a-0x02005960 the
 * tail computes a sine-derived radius into +48 exactly as this
 * overlay's two other spawners do -- and then at 0x02005962-0x02005966
 * it immediately overwrites +48 with a signed halfword read from the
 * anchor's +50. Both stores are to the same field, two instructions
 * apart. The whole sine chain, four instructions and a call, reaches
 * nothing. It is written out here rather than folded away because the
 * folded version would silently claim this spawner has no sine call,
 * and it demonstrably does. Note also that this copy omits the
 * `asr #16` that Func_020036d0 and Func_02005a28 both apply to the
 * same expression, so the discarded value was not even the same shape
 * as theirs.
 *
 * THIRD SPAWNER, and the diff against the other two closes the loop on
 * this overlay's orbit-step family. Same skeleton, and it installs
 * 0x0200b661 at +108 -- offset 0x3660 published, i.e. Func_02003660,
 * the COUNTER-DRIVEN twin. Consistently, this is the only one of the
 * three spawners that writes the actor's +98 byte, which is the only
 * counter that twin reads and which neither of the other two touches.
 * The three-way summary:
 *   0x020036d0 -> installs 0x02003600, no +98, keeps the sine radius;
 *   0x02005a28 -> installs 0x02005b90, no +98, keeps the sine radius,
 *                 also writes a +104 anchor;
 *   0x020056a0 -> installs 0x02003660, WRITES +98, discards the sine
 *                 radius for the anchor's +50 halfword.
 *
 * Uncertainties:
 *   - the step counter lives at 0x0200e764, which is FOUR BYTES PAST
 *     the end of the 26,464-byte image. In the base + 0x8000 spelling
 *     the overlay occupies 0x02008000-0x0200e760, so this is
 *     overlay-adjacent RAM immediately after the image, not an in-image
 *     word. That is recorded rather than explained.
 *   - the six scene indices 0, 1, 2, 3, 21, 6 appear twice, in that
 *     order both times -- once in state 25's arm setting each record's
 *     +98 byte to 1, and once in the unconditional epilogue. Whatever
 *     that set is, it is a fixed cast list.
 *   - Func_0808a330 / Func_0808a348 / Func_0808a098 / Func_0808a0a8 /
 *     Func_080091f0 / Func_080770c8 / Func_080f9010 are transcribed by
 *     arity and argument only; none of their meanings is established.
 *   - the constants are left in the shifted form the row builds them.
 *   - 0x03001e40 in the settle block is the tree's free-running FRAME
 *     COUNTER (established in HANDOVER), so `& 7` fires the
 *     Func_080f9010(246) call one frame in eight rather than selecting
 *     a mode.
 *   - the `goto` shape below is the row's own control flow: states 25
 *     and 26 both jump into a shared settle block at 0x02005876, and
 *     state 48 falls straight through into the join. Restructuring
 *     that into nested conditionals would move code between arms.
 */

extern u8 *Func_0808a080(s32 index);   /* scene-record accessor */
extern void Func_080f9010(s32 id);
extern void Func_080091f0(s32 a, s32 b, s32 c);
extern void Func_0808a330(s32 a, s32 b);
extern void Func_0808a348(s32 a);
extern void Func_02005688(u8 *record);  /* push-less leaf, see 0x020059f0 */
extern void Func_0808a098(s32 index, const void *blob);
extern void Func_0808a0a8(s32 index);
extern void Func_080770c8(s32 id);
extern s32 Func_080000f8();            /* pseudo-random source */
extern u8 *Func_080090c8(s32 kind, s32 a, s32 b, s32 c);   /* spawner */
extern void Func_08009098(u8 *record, const void *blob);
extern void Func_0808a160(u8 *record, s32 mode);
extern s32 Func_08000118(s32 angle);   /* sine of a binary angle */
extern void Func_020059f0(u8 *record);

void Func_020056a0(void)
{
    s32 *step = (s32 *)0x0200e764;
    u8 *anchor = Func_0808a080(23);
    s32 spawn = 0;
    s32 held = 0;
    u8 *record;
    u8 *attached;
    u32 offset;
    s32 radius;

    switch (*step) {
    case 0:
        Func_080f9010(220);
        Func_080091f0(192 << 11, 192 << 11, 128 << 9);
        Func_0808a330(0x002063ff, 1);
        Func_0808a348(8);
        break;

    case 8:
        Func_0808a330(128 << 9, 1);
        Func_0808a348(8);
        break;

    case 16:
        Func_080091f0(128 << 9, 128 << 9, 128 << 9);
        break;

    case 24:
        *(s32 *)(anchor + 8) = 152 << 17;
        *(s32 *)(anchor + 12) = (s32)0xfe980000;
        *(s32 *)(anchor + 16) = 164 << 16;
        *(s32 *)(anchor + 24) = 128 << 9;
        *(s32 *)(anchor + 28) = 128 << 9;
        Func_02005688(anchor);
        Func_0808a098(23, (const void *)0x0200e2d0);
        break;

    case 25:
        *step -= 1;
        held = *(s32 *)(anchor + 12);
        if (held <= 0) {
            goto settle;
        }
        Func_0808a330(0x00203210, 0);
        Func_0808a348(16);
        *step += 1;
        *(u8 *)(Func_0808a080(0) + 98) = 1;
        *(u8 *)(Func_0808a080(1) + 98) = 1;
        *(u8 *)(Func_0808a080(2) + 98) = 1;
        *(u8 *)(Func_0808a080(3) + 98) = 1;
        *(u8 *)(Func_0808a080(21) + 98) = 1;
        *(u8 *)(Func_0808a080(6) + 98) = 1;
        break;

    case 26:
        *step -= 1;
        held = *(s32 *)(anchor + 12);
        if (held <= (160 << 14)) {
            goto settle;
        }
        Func_0808a330(128 << 9, 0);
        Func_0808a348(40);
        *step += 1;
        break;

    case 27: case 28: case 29: case 30:
    case 31: case 32: case 33: case 34:
        spawn = 1;
        break;

    case 36:
        Func_080f9010(187);
        Func_0808a330(0x00007fff, 0);
        Func_0808a348(12);
        break;

    case 48:
        Func_0808a0a8(23);
        Func_080770c8(0x237);
        break;

    default:
        break;
    }
    goto join;

settle:
    if ((*(s32 *)0x03001e40 & 7) == 0) {
        Func_080f9010(246);
        held = *(s32 *)(anchor + 12);
    }
    *(s32 *)(anchor + 12) = held + (144 << 10);
    spawn = 1;

join:
    if (spawn != 0) {
        offset = ((u32)(Func_080000f8() * 80) >> 16) << 16;

        record = Func_080090c8(142 << 1,
                               *(s32 *)(anchor + 8),
                               *(s32 *)(anchor + 12) - (s32)offset
                                   + (s32)0xfff80000,
                               *(s32 *)(anchor + 16));
        if (record != 0) {
            attached = *(u8 **)(record + 80);

            Func_08009098(record, (const void *)0x0200e1cc);
            Func_0808a160(record, 1);
            *(u8 *)(record + 85) = 0;

            *(u16 *)(record + 100) = (u16)(Func_080000f8() & 0x0ffff000);
            *(u16 *)(record + 102) = 0;
            *(u8 *)(record + 98) = (u8)((u32)Func_080000f8() >> 13);

            *(s32 *)(record + 108) = 0x0200b661;   /* -> Func_02003660 */

            radius = Func_080000f8();
            radius = Func_08000118(
                (s32)((u32)((radius << 16) - radius) >> 20));

            /* Written, then overwritten two instructions later. */
            *(s32 *)(record + 48) = radius * 24;
            *(s32 *)(record + 48) = *(s16 *)(anchor + 50);

            *(u8 *)(attached + 38) = 0;
            *(u8 *)(attached + 9) = (u8)((*(u8 *)(attached + 9) & -13) | 4);
        }
    }

    *step += 1;
    Func_020059f0(Func_0808a080(0));
    Func_020059f0(Func_0808a080(1));
    Func_020059f0(Func_0808a080(2));
    Func_020059f0(Func_0808a080(3));
    Func_020059f0(Func_0808a080(21));
    Func_020059f0(Func_0808a080(6));
}
