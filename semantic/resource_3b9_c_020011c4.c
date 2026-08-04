#include "types.h"

/*
 * resource_3b9 owner at 0x020011c4, 212 bytes: a two-outcome scene
 * ending on actor 17, gated by story flag 0x8a4.
 *
 * PUBLISHED, NOT CALLED. Found by
 * `bun tools/overlay_published.ts resource_3b9`.
 *
 * THE FLAG RESULT IS REUSED AS A VALUE, not just as a condition. The
 * return of Func_080770c0(0x8a4) is kept in r5 across the whole
 * function, and on the flag-CLEAR path it is stored as the byte at
 * offset 85 of the record Func_0808a228 returns. On that path r5 is
 * necessarily zero -- the branch that reaches it is `cmp r5,#0 / beq`
 * -- so the store writes 0 and is written as 0 here. The register
 * reuse is the compiler noticing it already had a zero, not an
 * expression in the source.
 *
 * A WORKSPACE-WRITE VARIANT WORTH NAMING. The usual shape builds the
 * displacement 448 and then turns the same register into the value by
 * an add or subtract. Here it runs the other way and does it twice:
 *
 *     movs r3,#224 / lsls r3,#1   -> 448   (displacement)
 *     adds r2, r1, r3             -> &workspace[448]
 *     adds r3, #64                -> 512   (value)
 *     str  r3, [r2]                       workspace+448 = 512
 *     subs r3, #56                -> 456   (displacement, from the VALUE)
 *     adds r2, r1, r3             -> &workspace[456]
 *     movs r3, #32                -> 32    (value)
 *     str  r3, [r2]                       workspace+456 = 32
 *
 * So the second displacement is derived from the first slot's VALUE,
 * not from the first displacement. Anyone reading `subs r3,#56` as
 * 448-56 gets 392 and writes to the wrong field. Transcribe the whole
 * chain, never one instruction of it.
 *
 * The tail picks between Func_0808a248(70) and Func_0808a248(7) on a
 * second flag, 0x8a3 -- one below the gate flag 0x8a4, the same
 * adjacent-ids habit the 24-byte owner at 0x020011ac shows.
 *
 * Complete owner: `push {r5, lr}` at 0x020011c4 through
 * `pop {r5} / pop {r0} / bx r0` at 0x02001276-0x0200127a, then the
 * seven-word literal pool 0x0200127c-0x02001297; the next owner's
 * prologue is at 0x02001298. The r5 spill exists to carry the flag
 * result across the body.
 *
 * All `bl` targets resolved through the import-veneer table under the
 * +2 rule; the Func_02001c48 call is a direct intra-image branch.
 *
 * Uncertainty: record offset 85 is a byte by the `strb` width and its
 * meaning is unread; Func_0808a208's and Func_0808a210's arguments are
 * transcribed, not interpreted.
 */

extern void Func_080000c0(s32 arg0);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a150(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a180(s32 id, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern u8 *Func_0808a228(void);
extern void Func_0808a248(s32 arg0);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern s32 Func_080770c0(s32 flagId);
extern void Func_02001c48(s32 arg0);

extern u8 *Data_03001ebc;

void Func_020011c4(void)
{
    u8 *record;
    u8 *workspace;

    Func_0808a018();
    if (Func_080770c0(0x8a4)) {
        Func_0808a150(17, 0, 40);
        Func_0808a170(0x206f);
        Func_02001c48(17);
        Func_0808a1b8(17, 0x3000, 20);
    } else {
        Func_0808a130(17, 2);
        Func_0808a170(0x206d);
        Func_0808a180(17, 0);
        record = Func_0808a228();
        *(u8 *)(record + 85) = 0;
        Func_080000c0(1);
        Func_0808a208(0x66666, 0xcccc);
        Func_0808a210(0x21c0000, -1, 0xd00000, 1);
        Func_0808a218();
        workspace = Data_03001ebc;
        *(s32 *)(workspace + 448) = 512;
        *(s32 *)(workspace + 456) = 32;
        Func_0808a368();
        Func_0808a370();
        if (Func_080770c0(0x8a3))
            Func_0808a248(70);
        else
            Func_0808a248(7);
    }
    Func_0808a020();
}
