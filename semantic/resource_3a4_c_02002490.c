#include "types.h"

/*
 * resource_3a4 owner at 0x02002490, 304 bytes: the flag-0x907 SIBLING
 * of resource_3a4_c_02002310.c -- the same post-scene reconciliation
 * shape keyed to the third sink-and-flicker sibling (0x020008d4 sets
 * 0x907) instead of the twin's 0x8fe, acting on actor 10 instead of 9,
 * with its own local handoff and closing-flag pair.
 *
 * Side-by-side with 0x2310, each read from its own bytes:
 *   - flag set: same `+20 &= 0xfdff` scene-state clear, park actor 10
 *     (twin parks 9);
 *   - flag clear: the selector-99 handoff goes to local Func_02002e84
 *     (twin: Func_02002e1c), and here it runs BEFORE the shared
 *     Func_02003a44 helper where 0x2310 calls 0x3a44 first;
 *   - the guarded configure block is Func_0808a158(10, 2) /
 *     Func_0808a100(10, 3) / Func_02002bb0(187 << 18, 128 << 12,
 *     140 << 17, 0x8000) -- the mover gets a non-zero second argument
 *     here, unlike 0x2310's;
 *   - then, unconditionally, Func_02003460(9) plus a flag-0x200 block
 *     that 0x2310 lacks entirely (0x25c0 carries the twin of THAT
 *     block, with different panel coordinates);
 *   - the closing choice tests flag 0x325 (0x2310: 0x323) between
 *     window ids 49 vs 48 (panel first id 10 vs 12).
 *
 * Complete owner: `push {lr}` at 0x02002490, `sub sp, #8`, through
 * `add sp, #8 / pop {r0} / bx r0` at 0x20025a0-0x20025a4, trailing
 * literal pool at 0x20025a6-0x20025bf, immediately followed by the
 * drafted owner at 0x020025c0 (resource_3a4_c_020025c0.c).  No
 * arguments, void.
 *
 * All 19 bl sites resolve with the `+2` stored-displacement rule; 15
 * veneers, four overlay-local:
 *   0x2002496 (0x907), 0x20024b8 (0x109), 0x20024da (0x907),
 *     0x2002510 (0x200), 0x2002546 (0x325) -> Func_080770c0
 *   0x20024b0 -> Func_0808a0f0             place entity (id, x, z)
 *   0x20024d0 -> image offset 0x2e84       local Func_02002e84, drafted
 *   0x20024d4 -> image offset 0x3a44       local helper (see 0x2310's note)
 *   0x20024e6 -> Func_0808a158, 0x20024ee, 0x200251c -> Func_0808a100
 *   0x2002502 -> image offset 0x2bb0       local Func_02002bb0, drafted
 *   0x2002508 -> image offset 0x3460       local Func_02003460(9)
 *   0x2002530, 0x200255e, 0x2002588 -> Func_080091c0
 *   0x2002536 -> Func_0808a080             record accessor
 *   0x2002572, 0x200259c -> Func_08009180  six-arg window call
 */

s32 Func_080770c0();           /* test story flag, established */
u8 *Func_0808a080();           /* scene-record accessor, established */
void Func_0808a0f0();          /* place entity (id, x, z), established */
void Func_0808a158();          /* (id, n), established */
void Func_0808a100();          /* (id, n) */
void Func_080091c0();          /* six-argument panel call */
void Func_08009180();          /* six-argument window call */
void Func_02002e84(void);      /* local, resource_3a4_c_02002e84.c */
void Func_02003a44();          /* local, image offset 0x3a44 */
void Func_02002bb0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02003460(s32 selector);   /* local, resource_3a4_c_02003460.c */

void Func_02002490(void)
{
    if (Func_080770c0(0x907) != 0) {
        *(u16 *)(*(u8 **)0x03001e70 + 20) &= 0xfdff;
        Func_0808a0f0(10, 0, 0);
    } else {
        if (Func_080770c0(0x109) == 0
            && *(s16 *)((u8 *)0x02000240 + 450) == 99) {
            Func_02002e84();
        }
        Func_02003a44();
        if (Func_080770c0(0x907) == 0) {
            Func_0808a158(10, 2);
            Func_0808a100(10, 3);
            Func_02002bb0(187 << 18, 128 << 12, 140 << 17, 0x8000);
        }
    }

    Func_02003460(9);

    if (Func_080770c0(0x200) != 0) {
        Func_0808a100(9, 5);
        Func_080091c0(23, 13, 1, 1, 25, 13);
        Func_0808a080(9)[0x23] |= 2;
    }

    if (Func_080770c0(0x325) != 0) {
        Func_080091c0(10, 72, 1, 1, 11, 73);
        Func_08009180(49, 32, 11, 4, 1, 2);
    } else {
        Func_080091c0(12, 72, 1, 1, 11, 73);
        Func_08009180(48, 32, 11, 4, 1, 2);
    }
}
