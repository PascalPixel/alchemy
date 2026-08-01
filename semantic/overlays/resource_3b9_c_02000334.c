typedef int s32;

/*
 * resource_3b9 owner at 0x02000334, 104 bytes: the third published talk
 * handler in the bank at 0x020002a0-0x020006bc, for actor 14.
 *
 * PUBLISHED, NOT CALLED. Found by
 * `bun tools/overlay_published.ts resource_3b9`. The bank is described
 * in resource_3b9_c_020002a0.c.
 *
 * FIRST MEMBER TO USE THE OVERLAY'S OWN TRAMPOLINES. It calls
 * Func_02001c48(14) and Func_02001c5c(14, 0), both already tracked
 * byte-exact under assets/code. Their signatures are taken from those
 * files rather than re-derived: (s32) and (s32, s32).
 *
 * A NAMING CONFLICT WORTH FLAGGING, not fixing here. Some existing
 * files in the tree declare `Func_02001c5c(void)` alongside others
 * declaring `Func_02001c5c(s32, s32)`. The exact-C row
 * assets/code/resource_3b9_c_02001c5c.c settles it: it takes two
 * arguments and forwards them. The void spelling is wrong, but the
 * files carrying it belong to other rows and rewriting them mid-flight
 * would hand another reconstruction a merge conflict.
 *
 * Shape, and note the two arms are very different lengths:
 *     open scene
 *     if (flag 0x962 set) Func_0808a138(14, 2), line 0x2256,
 *                         Func_02001c48(14), Func_0808a150(14, 0, 0),
 *                         wait 20, Func_0808a190(14, 0),
 *                         Func_02001c5c(14, 0)
 *     else                line 0x205d, Func_0808a180(14, 0)
 *     close scene
 *
 * The long arm ends on Func_0808a190 and the short arm on
 * Func_0808a180 -- two different callees, the same asymmetry the first
 * handler in this bank has and the second does not. Read per arm.
 *
 * Complete owner: `push {lr}` at 0x02000334 through `pop {r0} / bx r0`
 * at 0x0200038c-0x0200038e, then the three-word literal pool
 * 0x02000390-0x0200039b (0x962, 0x2256, 0x205d); the next owner's
 * prologue is at 0x0200039c. No alignment halfword here -- the
 * epilogue already ends on a word boundary, unlike its two neighbours.
 *
 * All `bl` targets resolved through the import-veneer table under the
 * +2 rule; the two calls to this overlay's own trampolines are direct
 * intra-image branches, visible only in the resolved target.
 *
 * Uncertainty: Func_0808a138's second argument (2 here) is unread.
 */

extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a150(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a180(s32 id, s32 arg1);
extern void Func_0808a190(s32 id, s32 arg1);
extern s32 Func_080770c0(s32 flagId);
extern void Func_02001c48(s32 arg0);
extern void Func_02001c5c(s32 arg0, s32 arg1);

void Func_02000334(void)
{
    Func_0808a018();
    if (Func_080770c0(0x962)) {
        Func_0808a138(14, 2);
        Func_0808a170(0x2256);
        Func_02001c48(14);
        Func_0808a150(14, 0, 0);
        Func_0808a010(20);
        Func_0808a190(14, 0);
        Func_02001c5c(14, 0);
    } else {
        Func_0808a170(0x205d);
        Func_0808a180(14, 0);
    }
    Func_0808a020();
}
