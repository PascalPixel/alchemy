typedef int s32;

/*
 * resource_3b9 owner at 0x0200055c, 148 bytes: the eighth published
 * handler in the bank at 0x020002a0-0x020006bc, driving actors 16 and
 * 17, and the bank's second once-only grant.
 *
 * PUBLISHED, NOT CALLED. Found by
 * `bun tools/overlay_published.ts resource_3b9`. The bank is described
 * in resource_3b9_c_020002a0.c.
 *
 * ONCE-ONLY ON FLAG 0x3c1, and the polarity puts the body in the ELSE.
 * `cmp r0,#0 / beq` past the short arm means the flag-SET case is the
 * two-instruction one (wait 20 frames and leave), and the long
 * first-time sequence runs when the flag is clear, ending with
 * Func_080770c8(0x3c1) to set it. Read from the mnemonic, not carried
 * from 0x0200039c, which is the bank's other grant.
 *
 * THE TWO GRANT FLAGS ARE ADJACENT AND SPELLED DIFFERENTLY. 0x0200039c
 * guards on 960 (0x3c0), built as `movs r0,#240 / lsls r0,#2`; this one
 * guards on 0x3c1, loaded from a pool word. Consecutive ids, two
 * different encodings, in two members of the same bank. The id
 * adjacency is worth recording; the encoding difference is just the
 * compiler picking the cheaper form for each value.
 *
 * The handler opens on actor 16 and then does all its first-time work
 * on actor 17, so the pairing is deliberate rather than a
 * transcription slip -- the `movs r0,#16` at 0x02000564 and the
 * `movs r0,#17` from 0x0200058c onward were read individually.
 *
 * Complete owner: `push {lr}` at 0x0200055c through `pop {r0} / bx r0`
 * at 0x020005de-0x020005e0, a zero alignment halfword at 0x020005e2,
 * then the three-word literal pool 0x020005e4-0x020005ef (line 0x211b,
 * flag 0x3c1, and 0x105); the next owner's prologue is at 0x020005f0.
 *
 * All `bl` targets resolved through the import-veneer table under the
 * +2 rule; the Func_02001c48 and Func_02001c5c calls are direct
 * intra-image branches.
 *
 * Uncertainty: Func_0808a188's arguments and the 0x105 passed to
 * Func_0808a1e8 are unread.
 */

extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a150(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_02001c48(s32 arg0);
extern void Func_02001c5c(s32 arg0, s32 arg1);

void Func_0200055c(void)
{
    Func_0808a018();
    Func_0808a138(16, 2);
    Func_0808a170(0x211b);
    Func_0808a188(16, 0, 20);
    if (Func_080770c0(0x3c1)) {
        Func_0808a010(20);
    } else {
        Func_02001c5c(17, 0);
        Func_0808a138(17, 1);
        Func_02001c48(17);
        Func_0808a150(17, 0, 20);
        Func_0808a100(17, 4);
        Func_02001c48(17);
        Func_0808a1e8(17, 0x105, 40);
        Func_02001c48(17);
        Func_02001c5c(17, 0x5000);
        Func_080770c8(0x3c1);
    }
    Func_0808a020();
}
