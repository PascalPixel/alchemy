#include "types.h"

/*
 * resource_3a4 owner at 0x02002934, 168 bytes: scene-entry fixup step --
 * either park actors 8 and 9 at the origin (story flag 0x909 set) or
 * seat them visibly (flag clear), then a scene-selector dispatch off the
 * shared scene table's +450 halfword with a flag-0x109 gate on both
 * arms.
 *
 * Complete owner: `push {r5, lr}` at 0x02002934 through `pop {r5} /
 * pop {r0} / bx r0` at 0x20029ca-0x20029ce, trailing three-word literal
 * pool at 0x20029d0-0x20029db, immediately followed by the recorded
 * owner at 0x020029dc (resource_3a4_c_020029dc.c).  No arguments, void.
 *
 * 0x02000240 is the shared runtime scene table; its +450 (225 << 1)
 * signed halfword is the same selector field resource_3b1_c_0200037c.c
 * reads.  Selector 1 or 98: unless flag 0x109 is set, spawn a fresh
 * entity at slot 0, raise its +12 (y) to 0x100000 (16.0) inside a
 * Func_0808a018/Func_0808a020 bracket.  Selector 99: unless flag 0x109
 * is set, run this overlay's own Func_02003028 (the 938-byte unindexed
 * row, declared old-style here; drafted separately).
 *
 * In the flag-0x909-clear arm the strb into actor 9's +0x59 writes r5,
 * which is provably 0 there (it holds Func_080770c0's zero result) --
 * the same +89 flags byte resource_3a4_c_02003460.c masks.
 *
 * All 15 bl sites resolve with the `+2` stored-displacement rule; 14
 * land in the veneer table, one is overlay-local:
 *   0x2002938, 0x2002998, 0x20029be -> Func_080770c0   test story flag
 *   0x2002948, 0x2002952 -> Func_0808a0f0   place entity
 *   0x200295a, 0x200296e, 0x200297a, 0x20029a2 -> Func_0808a080   record accessor
 *   0x2002960, 0x2002974 -> Func_080091e0   (record, n), established
 *   0x2002968 -> Func_0808a1e0              (slot, n), established
 *   0x20029a8 -> Func_0808a018              open bracket on the entity
 *   0x20029b2 -> Func_0808a020              close bracket
 *   0x20029c6 -> image offset 0x3028        local Func_02003028
 */

           /* test story flag, established */
           /* scene-record accessor, established */
           /* place entity (id, x, z), established */
          /* (record, n), established */
          /* (slot, n), established */
          /* open bracket, established */
          /* close bracket, established */
          /* overlay-local, image offset 0x3028 */

extern s32 Func_0200648a();
extern u8 * Func_02006532();
extern u8 * Func_0200653c();
extern void Func_020064f4();
extern u8 * Func_02006492();
extern void Func_020065c2();
extern void Func_02006508();
extern u8 * Func_020064a6();
extern u8 * Func_02006514();
extern s32 Func_020064ea();
extern void Func_0200651a();
extern void Func_0200652c();
extern s32 Func_02006510();
extern void Func_020059f0();
void Func_02002934(void)
{
    s32 flag;
    u8 *entity;
    s16 selector;

    flag = Func_0200648a(0x909);
    if (flag != 0) {
        Func_02006532(8, 0, 0);
        Func_0200653c(9, 0, 0);
    } else {
        Func_020064f4(Func_02006492(8), 0);
        Func_020065c2(9, 3);
        Func_02006508(Func_020064a6(9), 0);
        Func_02006514(9)[0x59] = flag;      /* provably 0 here */
    }

    selector = *(s16 *)((u8 *)0x02000240 + 450);
    if (selector == 1 || selector == 98) {
        if (Func_020064ea(0x109) == 0) {
            entity = Func_0200653c(0);
            *(s32 *)(entity + 12) = 0x100000;   /* 128 << 13 */
            Func_0200651a(entity);
            Func_0200652c();
        }
    } else if (selector == 99) {
        if (Func_02006510(0x109) == 0) {
            Func_020059f0();
        }
    }
}
