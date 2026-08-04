#include "types.h"

/*
 * resource_3a4 owner at 0x020008d4, 280 bytes: the THIRD sibling of the
 * sink-and-flicker family (0x020006dc / 0x020007e8) -- same scene beat,
 * bracketed by Func_08009180 window calls instead of panels or the
 * actor-8 cameo, ending on story flag 0x907 (the flag
 * resource_3a4_c_020025c0.c later tests to clear the scene-state bit).
 *
 * Verified against this row's own bytes with both siblings open:
 * byte-for-byte the same sink loop as 0x7e8 (down-counting r6 = 23..0,
 * `-0x10000` on the record's +12 at four frames per step), the same
 * flicker bracket (install 0x020086a1 = Func_020006a0 | 1, zero
 * 0x0200d238, step to 100, one extra frame, uninstall), the same
 * Func_080f9010(230) opener, 0x121 sting and
 * `Func_080091f0(-1, -1, 0xe666) / Func_0808a010(30)` fade tail.
 * Deltas: THREE opening windows -- Func_08009180(113, 31, 103, 17, 1,
 * 1), (111, 32, 104, 18, 3, 2), (64, 32, 103, 18, 1, 2) -- one closing
 * window (103, 14, 103, 17, 4, 3), and flag 0x907 against the twins'
 * 0x8fd/0x8fe.
 *
 * Complete owner: `push {r5, r6, lr}` plus the r8 save at 0x020008d4,
 * `sub sp, #8`, through `add sp, #8 / pop {r3} / mov r8, r3 /
 * pop {r5, r6} / pop {r0} / bx r0` at 0x20009d6-0x20009e0.  The `b.n
 * 0x20009ac` at 0x2000992 hops the interior literal pool; the tail and
 * the closing pool words at 0x20009e2-0x20009eb are this owner's own.
 * No arguments, void.
 *
 * All 16 bl sites resolve with the `+2` stored-displacement rule:
 *   0x20008f0, 0x2000904, 0x2000914, 0x20009cc -> Func_08009180
 *   0x200091a (230), 0x200098a (0x121) -> Func_080f9010
 *   0x200092a, 0x20009b2 -> Func_080091f0   camera zoom triple
 *   0x2000930, 0x20009b8 -> Func_0808a010   frame wait
 *   0x2000948, 0x2000964, 0x200097a -> Func_080000c0
 *   0x2000956, 0x2000984 -> Func_08000130   per-frame callback hook
 *   0x20009d2 -> Func_080770c8              set a story flag
 */

void Func_08009180();          /* six-argument window call */
void Func_080f9010();          /* sound/sting, established */
void Func_080091f0();          /* camera zoom triple, established */
void Func_0808a010();          /* frame wait, established */
void Func_080000c0();          /* wait n frames, established */
void Func_08000130();          /* per-frame callback install/clear */
void Func_080770c8();          /* set a story flag, established */

void Func_020008d4(void)
{
    u8 *record;
    u16 *counter;
    s32 i;

    record = *(u8 **)0x03001e70 + 0x164;

    Func_08009180(113, 31, 103, 17, 1, 1);
    Func_08009180(111, 32, 104, 18, 3, 2);
    Func_08009180(64, 32, 103, 18, 1, 2);

    Func_080f9010(230);
    Func_080091f0(0x20000, 0x20000, 0x10000);   /* 128<<10, 128<<9 */
    Func_0808a010(10);

    for (i = 23; i >= 0; i--) {
        *(s32 *)(record + 12) += -0x10000;      /* pool 0xffff0000 */
        Func_080000c0(4);
    }

    Func_08000130(1, 0, 0x020086a1);            /* Func_020006a0 | 1 */

    counter = (u16 *)0x0200d238;
    *counter = 0;
    do {
        Func_080000c0(1);
        *counter += 1;
    } while ((u32)(*counter << 16) <= (u32)(200 << 15));

    Func_080000c0(1);
    Func_08000130(1, 0, 0);
    Func_080f9010(0x121);

    Func_080091f0(-1, -1, 0xe666);              /* 0.9 in 16.16 */
    Func_0808a010(30);
    Func_08009180(103, 14, 103, 17, 4, 3);
    Func_080770c8(0x907);
}
