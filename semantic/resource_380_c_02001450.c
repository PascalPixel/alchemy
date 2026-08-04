#include "types.h"

/*
 * resource_380 owner at 0x02001450, 828 bytes: a branch-free scripted
 * scene beat of roughly ninety calls -- actor placements for ids
 * 5/9/10/11/12, per-actor mode and animation runs, three
 * position-with-hold blocks around fixed-point targets (0x15a0000/
 * 0x1b80000, 0x1180000/0x1d60000, 0x14e0000 pair), two dialogue-style
 * pairs against ids 0x01050000/0x109b/0x01dd0000, and a long
 * choreographed tail that walks the same actor set through
 * mode-2/mode-3 transitions with 20-tick holds before the closing
 * placement.  Same linear-scene-bracket family as the 0x02000d5c and
 * 0x0200227c owners; the actor coordinates reuse that family's
 * vocabulary.
 *
 * Complete owner: `push {lr}` at 0x02001450 through `pop {r0} / bx r0`
 * at 0x2001778-0x200177c, alignment halfword, then the three-word
 * literal pool 0x02001780-0x0200178b (0x01050000, 0x109b, 0x01dd0000);
 * next owner's prologue at 0x0200178c.
 *
 * CORRECTION (name sweep): this file's callee names came from a naive
 * pc-relative decode and were wrong.  Resolved through the overlay's
 * import-veneer table under the +2 rule
 * (tools/overlay_call_targets.ts), every callee is main-ROM -- the
 * Func_0808a0xx scene vocabulary shared with the other item-28 drafts,
 * plus the local wrapper Func_02004248.  The earlier claim that six
 * callees were "called at two arities inside this one owner" was an
 * artifact of that decode: one phantom name stood for two genuinely
 * different functions, which is exactly why their arities disagreed.
 * No callee here is called at two arities, and none is declared
 * old-style.
 *
 * Uncertainty: callee roles beyond call shape remain open.
 */

extern void Func_02004248(s32 arg0, s32 arg1);
extern void Func_080000c0(s32 arg0);
extern void Func_08009128(void);
extern void Func_0808a010(s32 arg0);
extern void Func_0808a0f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 arg0, s32 arg1);
extern void Func_0808a110(s32 arg0, s32 arg1);
extern void Func_0808a130(s32 arg0, s32 arg1);
extern void Func_0808a138(s32 arg0, s32 arg1);
extern void Func_0808a148(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 arg0, s32 arg1);
extern void Func_0808a180(s32 arg0, s32 arg1);
extern void Func_0808a190(s32 arg0, s32 arg1);
extern void Func_0808a1b8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a1f0(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);

void Func_02001450(void)
{
    Func_0808a1b8(5, 0x6000, 0);
    Func_0808a1b8(9, 0x5000, 0);
    Func_0808a1b8(10, 0x8000, 0);
    Func_0808a1b8(12, 0x5000, 40);
    Func_0808a1b8(11, 0xd000, 40);
    Func_0808a138(11, 2);
    Func_02004248(11, 20);
    Func_0808a138(12, 2);
    Func_0808a1f0(12, 258);
    Func_0808a010(60);
    Func_02004248(12, 10);
    Func_0808a148(10, 12, 0);
    Func_0808a148(5, 12, 0);
    Func_0808a148(9, 12, 0);
    Func_0808a010(40);
    Func_0808a138(10, 1);
    Func_0808a1b8(10, 0x8000, 10);
    Func_0808a100(10, 3);
    Func_02004248(10, 10);
    Func_0808a1b8(11, 0, 10);
    Func_0808a110(11, 3);
    Func_0808a010(10);
    Func_0808a1b8(5, 0, 0);
    Func_0808a1b8(9, 0x8000, 20);
    Func_0808a130(5, 2);
    Func_0808a138(9, 2);
    Func_0808a010(10);
    Func_0808a1b8(5, 0xc000, 0);
    Func_0808a1b8(9, 0xb000, 10);
    Func_0808a1b8(10, 0xb000, 40);
    Func_0808a110(10, 3);
    Func_02004248(10, 10);
    Func_0808a1e8(5, 256, 20);
    Func_0808a1f0(12, 258);
    Func_0808a138(12, 3);
    Func_0808a010(40);
    Func_0808a0f0(1, 0x15a0000, 0x1b80000);
    Func_080000c0(1);
    Func_0808a180(1, 0);
    Func_0808a1b8(0, 0x2000, 0);
    Func_0808a1b8(1, 0xa000, 20);
    Func_0808a368();
    Func_0808a370();
    Func_0808a0f0(1, 0x1180000, 0x1d60000);
    Func_0808a210(0x01050000, -1, 0x1d20000, 0);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);
    Func_0808a1b8(0, 0x3000, 10);
    Func_0808a1e8(1, 258, 0);
    Func_0808a138(1, 3);
    Func_0808a190(1, 0);
    Func_0808a010(10);
    Func_0808a110(1, 4);
    Func_0808a010(10);
    Func_0808a190(1, 0);
    Func_0808a010(10);
    Func_0808a170(0x109b, 1);
    Func_0808a180(11, 0);
    Func_0808a1b8(11, 0xd000, 10);
    Func_0808a130(0, 2);
    Func_0808a138(0, 2);
    Func_0808a010(10);
    Func_0808a1b8(0, 0xe000, 0);
    Func_0808a1b8(1, 0xe000, 20);
    Func_0808a368();
    Func_0808a370();
    Func_0808a210(0x01dd0000, -1, 0x14e0000, 0);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);
    Func_0808a138(9, 2);
    Func_0808a010(10);
    Func_0808a1b8(9, 0x8000, 30);
    Func_02004248(9, 20);
    Func_0808a1b8(5, 0, 40);
    Func_0808a110(5, 4);
    Func_0808a010(20);
    Func_02004248(5, 10);
    Func_0808a138(12, 2);
    Func_0808a010(80);
    Func_02004248(12, 20);
    Func_0808a138(5, 2);
    Func_0808a1b8(5, 0xc000, 0);
    Func_0808a1b8(9, 0xb000, 20);
    Func_0808a110(12, 3);
    Func_02004248(12, 20);
    Func_0808a130(5, 2);
    Func_0808a138(9, 2);
    Func_0808a1b8(12, 0xb000, 40);
}
