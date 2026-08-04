#include "types.h"

/*
 * resource_3a4 owner at 0x020007e8, 236 bytes: the second half of the
 * sink-and-flicker twin pair -- same scene beat as 0x020006dc but
 * bracketed by a Func_080091b8 panel instead of the actor-8 cameo,
 * ending on story flag 0x8fe.
 *
 * TWIN of 0x020006dc (resource_3a4_c_020006dc.c); both drafted
 * together, each verified against its OWN disassembly.  Shared body
 * confirmed on this side: the same 24-step `-0x10000` sink on the
 * record's +12 field at four frames per step (here a down-counting
 * loop, r6 = 23..0, where the twin counts up -- same 24 iterations),
 * the same flicker bracket (install 0x020086a1 = Func_020006a0 | 1,
 * zero 0x0200d238, step to 100, one extra frame, uninstall), the same
 * Func_080f9010(0x121) sting and `Func_080091f0(-1, -1, 0xe666) /
 * Func_0808a010(30)` fade tail.  Real deltas, read from this row's own
 * bytes: an OPENING Func_080091b8(93, 41, 16, 4, 77, 28) panel the twin
 * lacks, no actor-8 block anywhere in the loop, a closing
 * Func_080091b8(77, 41, 16, 4, 77, 28) repeat (first argument 93 -> 77)
 * where the twin calls Func_080091c0(0, 0, 1, 2, 3, 14), and flag 0x8fe
 * against the twin's 0x8fd.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020007e8, `sub sp, #8`,
 * through `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0` at
 * 0x20008c2-0x20008c8.  The `b.n 0x2000898` at 0x200087e only hops the
 * literal pool at 0x2000880-0x2000896; the tail through the pool words
 * at 0x20008cc-0x20008d2 is this owner's own inline copy, not shared
 * with the twin.  No arguments, void.
 *
 * Record base, counter and callback identities as in the twin's
 * header: scene-state pointer at 0x03001e70 plus 0x164, runtime
 * halfword at 0x0200d238 just past the image end, callback constant
 * 0x020086a1 = Func_020006a0 | 1 at the 0x02008000 link base.
 *
 * All 14 bl sites resolve through the veneer table with the `+2`
 * stored-displacement rule:
 *   0x2000800, 0x20008b8 -> Func_080091b8    six-argument panel call
 *   0x2000806, 0x2000876 -> Func_080f9010    sound/sting (id)
 *   0x2000816, 0x200089e -> Func_080091f0    camera zoom triple
 *   0x200081c, 0x20008a4 -> Func_0808a010    frame wait, established
 *   0x2000834, 0x2000850, 0x2000866 -> Func_080000c0   wait n frames
 *   0x2000842, 0x2000870 -> Func_08000130    per-frame callback hook
 *   0x20008be -> Func_080770c8               set a story flag
 */

void Func_080091b8();          /* six-argument panel call */
void Func_080f9010();          /* sound/sting, established */
void Func_080091f0();          /* camera zoom triple, established */
void Func_0808a010();          /* frame wait, established */
void Func_080000c0();          /* wait n frames, established */
void Func_08000130();          /* per-frame callback install/clear */
void Func_080770c8();          /* set a story flag, established */

void Func_020007e8(void)
{
    u8 *record;
    u16 *counter;
    s32 i;

    record = *(u8 **)0x03001e70 + 0x164;

    Func_080091b8(93, 41, 16, 4, 77, 28);
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
    Func_080091b8(77, 41, 16, 4, 77, 28);
    Func_080770c8(0x8fe);
}
