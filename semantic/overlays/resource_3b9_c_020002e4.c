typedef int s32;

/*
 * resource_3b9 owner at 0x020002e4, 80 bytes: the second published talk
 * handler in the contiguous bank at 0x020002a0-0x020006bc, this one for
 * actor 13.
 *
 * PUBLISHED, NOT CALLED. Found by
 * `bun tools/overlay_published.ts resource_3b9`. The bank as a whole is
 * described in resource_3b9_c_020002a0.c.
 *
 * NOT A COPY OF ITS NEIGHBOUR, though it is the same size class and
 * tests the same flag. Two differences, both read rather than inferred:
 *   - The flag-set arm opens with Func_0808a1e8(13, 258, 40) before the
 *     line, which the 0x020002a0 handler has no counterpart for.
 *   - BOTH arms here end with Func_0808a180. The 0x020002a0 handler
 *     ends its two arms with two DIFFERENT callees. So the asymmetry
 *     there is a property of that handler, not of the bank, and cannot
 *     be carried across.
 *
 * The second argument to Func_0808a1e8 is built as `movs r1,#129 /
 * lsls r1,#1`, so it is 258 and not the pool-looking 0x102. This is the
 * same transcription error previously seen on this shape.
 *
 * Shape:
 *     open scene
 *     if (flag 0x962 set) Func_0808a1e8(13, 258, 40),
 *                         line 0x2254, Func_0808a180(13, 0)
 *     else                line 0x205c, Func_0808a180(13, 0)
 *     close scene
 *
 * Complete owner: `push {lr}` at 0x020002e4 through `pop {r0} / bx r0`
 * at 0x02000322-0x02000324, a zero alignment halfword at 0x02000326,
 * then the three-word literal pool 0x02000328-0x02000333 (0x962,
 * 0x2254, 0x205c); the next owner's prologue is at 0x02000334.
 *
 * All `bl` targets resolved through the import-veneer table under the
 * +2 rule (tools/overlay_call_targets.ts).
 *
 * Uncertainty: Func_0808a1e8's arguments have no established meaning
 * beyond the actor id in the first.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a180(s32 id, s32 arg1);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern s32 Func_080770c0(s32 flagId);

void Func_020002e4(void)
{
    Func_0808a018();
    if (Func_080770c0(0x962)) {
        Func_0808a1e8(13, 258, 40);
        Func_0808a170(0x2254);
        Func_0808a180(13, 0);
    } else {
        Func_0808a170(0x205c);
        Func_0808a180(13, 0);
    }
    Func_0808a020();
}
