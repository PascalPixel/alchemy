#include "types.h"

/*
 * resource_3c9 owner at 0x02003e9c, 928 bytes: mid-length scripted scene
 * continuation (no Func_0808a018/020 bracket of its own -- it runs inside
 * one opened elsewhere; bracketed by resource_3c9_c_020038c0.c alongside
 * 0x02003bc8 and 0x0200423c). This row and 0x02004bec were queued as
 * "branchy"; the reality is milder -- exactly two conditional branches,
 * both belonging to ONE idiom instance around Func_0808a070(0, 0):
 *
 *   taken = 0;
 *   if (Func_0808a070(0, 0) == 0) { wait; Func_0808a110(2,4); taken = 1; }
 *   else { wait; Func_0808a138(2,1); skipBeatCounter++; }
 *   Func_02000894(2);
 *   if (taken) skipBeatCounter++;
 *
 * -- i.e. the u16 skip-beat counter at workspace+472 (named in
 * resource_38b_c_02000584.c) is incremented on EITHER path, before or
 * after the Func_02000894(2) call depending on the arm. The same
 * Func_0808a070==1 gate closes resource_3c9_c_020008b4.c; this confirms
 * the shared idiom an earlier note proposed, resolved here as
 * "advance the beat exactly once, arm-dependent ordering".
 *
 * Complete owner: `push {r5, r6, lr}` + the r8/sl spill at
 * 0x02003e9c-0x02003ea2 through `pop {r3, r5} / mov r8,r3 / mov sl,r5 /
 * pop {r5, r6} / pop {r0} / bx r0` at 0x02004200-0x200420a, trailing
 * pool 0x200420c-0x200423b; next owner prologue at 0x0200423c
 * (resource_3c9_c_0200423c.c, drafted in this reconstruction). No incoming
 * arguments read, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (bun tools/overlay_call_targets.ts resource_3c9 3e9c 423c's
 * +2 rule).
 *
 * Local callees are the same pre-owner helper pair as
 * resource_3c9_c_020008b4.c (Func_02000894 / Func_020008a8, declared
 * not drafted, mixed small-id/packed-constant first argument kept as
 * compiled). One packed-constant quirk unique to this row: the compiled
 * code caches 0x2000 (= 128 << 6) in r6 for Func_020008a8(1, 0x2000)
 * and later executes `adds r6, #2` to make the 0x2002 it passes to
 * Func_02000894 twice -- transcribed as the resulting constants.
 *
 * Uncertainty: callee roles, text/fade ids (0x101/0x103/0x105, 0x141,
 * 258, the 0x19999/0xcccc and 0x33333/0x6666 pairs), and the packed
 * constants (0x1001, 0x2002, 0x2003, 0x8001) are recorded by shape only.
 */

extern u8 *Data_03001ebc;

extern void Func_0808a010(s32 frames);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a128(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a188(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1f0(s32 id, s32 arg1);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern void Func_080f9010(s32 arg0);
extern void Func_02000894(s32 arg0);
extern void Func_020008a8(s32 arg0, s32 arg1);

void Func_02003e9c(void)
{
    s32 taken;

    Func_0808a138(1, 1);
    Func_0808a010(20);
    Func_0808a188(1, 0, 20);
    Func_0808a138(2, 1);
    Func_0808a010(20);
    Func_0808a178(2, 0);
    Func_0808a1b8(2, 192 << 8, 0);
    Func_020008a8(0, 128 << 7);

    taken = 0;
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(20);
        Func_0808a110(2, 4);
        taken = 1;
    } else {
        Func_0808a010(20);
        Func_0808a138(2, 1);
        *(u16 *)(Data_03001ebc + 472) += 1; /* skip-beat counter */
    }
    Func_02000894(2);
    if (taken != 0) {
        *(u16 *)(Data_03001ebc + 472) += 1;
    }

    Func_0808a090(1, 0x19999, 0xcccc);
    Func_0808a0d0(1, 0x141, 174);
    Func_020008a8(1, 128 << 6);
    Func_02000894(0x1001);
    Func_0808a1b8(0, 160 << 8, 0);
    Func_0808a1b8(2, 160 << 8, 0);
    Func_020008a8(3, 192 << 8);
    Func_020008a8(1, 128 << 7);
    Func_02000894(0x1001);
    Func_0808a1e8(1, 0x105, 40);
    Func_0808a1b8(1, 128 << 6, 20);
    Func_02000894(0x1001);
    Func_0808a1b8(1, 160 << 8, 40);
    Func_0808a188(0x8001, 0, 40);
    Func_0808a1b8(1, 128 << 6, 20);
    Func_02000894(0x1001);
    Func_080f9010(17);
    Func_0808a100(3, 4);
    Func_02000894(3);
    Func_0808a1b8(1, 128 << 7, 80);
    Func_0808a1b8(3, 192 << 7, 80);
    Func_0808a1e8(3, 128 << 1, 40);
    Func_0808a1b8(0, 192 << 7, 60);
    Func_0808a138(0, 2);
    Func_020008a8(2, 192 << 8);
    Func_0808a1e8(2, 0x101, 60);
    Func_0808a1b8(2, 192 << 7, 40);
    Func_0808a128(2, 4, 60);
    Func_020008a8(1, 128 << 6);
    Func_0808a1e8(1, 0x101, 40);
    Func_02000894(0x1001);
    Func_0808a1b8(1, 192 << 7, 40);
    Func_0808a1e8(1, 128 << 1, 0);
    Func_0808a128(1, 4, 40);
    Func_02000894(1);

    Func_0808a090(21, 0xcccc, 0x6666);
    Func_0808a0b8(21, 200, 188);
    Func_0808a0b8(6, 200, 204);
    Func_0808a208(0x33333, 0x6666);
    Func_0808a210(252 << 16, 0, 190 << 16, 1);
    Func_0808a218();
    Func_0808a010(40);
    Func_080f9010(23);
    Func_0808a1f0(21, 129 << 1);
    Func_02000894(21);
    Func_0808a138(21, 1);
    Func_0808a010(20);
    Func_02000894(21);
    Func_0808a110(21, 4);
    Func_02000894(21);
    Func_0808a1b8(21, 192 << 6, 20);
    Func_02000894(21);
    Func_0808a1f0(6, 129 << 1);
    Func_0808a010(20);
    Func_0808a130(3, 2);
    Func_02000894(0x2003);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_02000894(0x2002);
    Func_020008a8(21, 224 << 8);
    Func_0808a138(1, 1);
    Func_02000894(1);
    Func_0808a138(21, 1);
    Func_0808a188(21, 0, 20);
    Func_02000894(0x2003);
    Func_0808a110(21, 4);
    Func_02000894(21);
    Func_0808a1f0(2, 129 << 1);
    Func_02000894(0x2002);
    Func_0808a100(21, 3);
    Func_02000894(21);
    Func_0808a1e8(1, 0x103, 40);
    Func_0808a130(1, 2);
    Func_02000894(1);
    Func_0808a100(21, 4);
    Func_02000894(21);
    Func_0808a1b8(21, 160 << 7, 20);
    Func_0808a128(3, 4, 20);
    Func_0808a188(0x2003, 0, 20);
    Func_0808a1e8(21, 0x103, 40);
}
