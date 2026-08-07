#include "types.h"

/*
 * resource_3a4 owner at 0x02002310, 280 bytes: the post-twin scene
 * branch -- everything here keys off flag 0x8fe, the flag the
 * sink-and-flicker twin Func_020007e8 sets.
 *
 * Flag 0x8fe set: clear bit 9 of the scene-state +20 halfword (same
 * mask as resource_3a4_c_020025c0.c) and park actor 9 at the origin.
 * Flag clear: run the local Func_02003a44 helper, then either hand off
 * to the local Func_02002e1c (flag 0x109 clear AND scene selector 99,
 * the same selector byte rows 0x2934/0x25c0 read), or pop two panels
 * and -- 0x8fe checked a second time, still clear -- configure actor 9
 * (Func_0808a158(9, 2), Func_0808a100(9, 3)) and call the local
 * four-argument mover Func_02002bb0(238 << 16, 0, 209 << 17, 0x8000).
 * Either way it closes with a flag-0x323 choice between two
 * panel/window pairs differing only in the leading id (0 vs 2).
 *
 * Complete owner: `push {lr}` at 0x02002310, `sub sp, #8`, through
 * `add sp, #8 / pop {r0} / bx r0` at 0x200240a-0x200240e, trailing
 * literal pool at 0x2002410-0x2002427, immediately followed by the
 * drafted owner at 0x02002428 (resource_3a4_c_02002428.c).  No
 * arguments, void.
 *
 * All 16 bl sites resolve with the `+2` stored-displacement rule; 13
 * veneers, three overlay-local:
 *   0x2002316 (0x8fe), 0x200233c (0x109), 0x2002384 (0x8fe),
 *     0x20023b0 (0x323) -> Func_080770c0   test story flag
 *   0x2002330 -> Func_0808a0f0             place entity (id, x, z)
 *   0x2002336 -> image offset 0x3a44       local helper (not yet drafted:
 *                zeroes 0x0200d25c/0x0200d260 and installs the local
 *                0x39c8|1 callback via Func_080000d0)
 *   0x2002354 -> image offset 0x2e1c       local Func_02002e1c, drafted
 *   0x200236a, 0x200237e, 0x20023c8, 0x20023f2 -> Func_080091c0
 *   0x2002390 -> Func_0808a158             (id, n), established
 *   0x2002398 -> Func_0808a100             (id, n)
 *   0x20023aa -> image offset 0x2bb0       local Func_02002bb0, drafted
 *   0x20023dc, 0x2002406 -> Func_08009180  six-arg window call
 */

s32 Func_080770c0();           /* test story flag, established */
void Func_0808a0f0();          /* place entity (id, x, z), established */
void Func_080091c0();          /* six-argument panel call */
void Func_08009180();          /* six-argument window call */
void Func_0808a158();          /* (id, n), established */
void Func_0808a100();          /* (id, n) */
void Func_02003a44();          /* local, image offset 0x3a44 */
void Func_02002e1c(void);      /* local, resource_3a4_c_02002e1c.c */
void Func_02002bb0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void Func_02002310(void)
{
    if (Func_080770c0(0x8fe) != 0) {
        *(u16 *)(*(u8 **)0x03001e70 + 20) &= 0xfdff;
        Func_0808a0f0(9, 0, 0);
    } else {
        Func_02003a44();
        if (Func_080770c0(0x109) == 0
            && *(s16 *)((u8 *)0x02000240 + 450) == 99) {
            Func_02002e1c();
        } else {
            Func_080091c0(38, 24, 1, 2, 37, 24);
            Func_080091c0(44, 23, 1, 2, 45, 23);
            if (Func_080770c0(0x8fe) == 0) {
                Func_0808a158(9, 2);
                Func_0808a100(9, 3);
                Func_02002bb0(238 << 16, 0, 209 << 17, 0x8000);
            }
        }
    }

    if (Func_080770c0(0x323) != 0) {
        Func_080091c0(0, 0, 1, 1, 24, 80);
        Func_08009180(0, 1, 24, 11, 1, 2);
    } else {
        Func_080091c0(2, 0, 1, 1, 24, 80);
        Func_08009180(2, 1, 24, 11, 1, 2);
    }
}
