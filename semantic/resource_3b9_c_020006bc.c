#include "types.h"

/*
 * resource_3b9 owner at 0x020006bc, 84 bytes: the eleventh and last
 * published handler in the bank at 0x020002a0-0x020006bc, for actor 8.
 *
 * PUBLISHED, NOT CALLED. Found by
 * `bun tools/overlay_published.ts resource_3b9`. The bank is described
 * in resource_3b9_c_020002a0.c.
 *
 * THE SKIP-BEAT GATE RUNS THE OPPOSITE WAY ROUND FROM 0x0200039c, and
 * this is the trap the bank sets twice over. Here `cmp r0,#0 / beq`
 * skips the counter bump, so the workspace+472 u16 advances when
 * Func_0808a070(0, 0) is NON-zero. In 0x0200039c the same call is
 * followed by `cmp r0,#0 / bne` past the body, so there the bump
 * happens when it is ZERO. Two members of one bank, one gate call, two
 * opposite senses. Neither can be derived from the other; both were
 * read from the mnemonic and its target.
 *
 * That makes three separate polarity traps now recorded in this
 * overlay: two arms ending on different callees (0x020002a0), a
 * once-only guard with the body in the else (0x0200055c), and this
 * inverted skip gate.
 *
 * The counter is the standard slot: `movs r3,#236 / lsls r3,#1` builds
 * displacement 472 against the shared workspace pointer at 0x03001ebc,
 * and the value is a u16 read, incremented and stored back.
 *
 * Complete owner: `push {lr}` at 0x020006bc through `pop {r0} / bx r0`
 * at 0x02000702-0x02000704, a zero alignment halfword at 0x02000706,
 * then the two-word literal pool 0x02000708-0x0200070f (line id 0x2125
 * and 0x03001ebc); the next owner's prologue is at 0x02000710, which is
 * the 1724-byte published owner that ends this bank's run.
 *
 * All `bl` targets resolved through the import-veneer table under the
 * +2 rule (tools/overlay_call_targets.ts).
 *
 * Uncertainty: what Func_0808a070(0, 0) reports is unestablished beyond
 * its role as the skip gate.
 */









extern u8 *Data_03001ebc;

extern void Func_020032b4(void);
extern void Func_0200334e(s32 id, s32 arg1, s32 arg2);
extern void Func_02003364(s32 dialogueId);
extern void Func_02003374(s32 id, s32 arg1);
extern s32 Func_020032e4(s32 arg0, s32 arg1);
extern void Func_020033a0(s32 id, s32 arg1);
extern void Func_020032fc(void);
void Func_020006bc(void)
{
    Func_020032b4();
    Func_0200334e(8, 0, 20);
    Func_02003364(0x2125);
    Func_02003374(8, 0);
    if (Func_020032e4(0, 0))
        *(u16 *)(Data_03001ebc + 472) += 1;
    Func_020033a0(8, 0);
    Func_020032fc();
}
