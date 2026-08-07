#include "types.h"

/*
 * Resource 3b5, cutscene script step at 0x02000894 (192 bytes; 178 bytes of
 * code, two bytes of alignment, and the 12-byte literal pool at
 * 0x02000948-0x02000953).
 *
 * Complete owner: `push {lr}` at 0x02000894 and `pop {r0} / bx r0` at
 * 0x02000942.  r0 holds the popped return address: void.
 *
 * Role: named in advance by the cutscene script table, which holds 0x02008895
 * in the handler word of three 12-byte records — Func_02000894 + the Thumb bit
 * under this overlay's proven 0x02008000 link base.  No arguments.
 *
 * The eight-instruction sequence at 0x020008e0-0x020008ee is the project's
 * skip-beat counter, recognised by its constants (`movs r3, #236 / lsls #1`
 * off the 0x03001ebc workspace pointer, i.e. the u16 at workspace + 472).  It
 * sits on only one arm of the Func_0808a070 test, which is the usual shape:
 * the two arms are the played and the skipped variants of one beat.
 *
 * Call targets resolved with `tools/lib/overlay_call_targets.ts`; sixteen sites,
 * against the inventory's `calls=15`, which is a documented lower bound.
 */

/* Old-style declarations: overlay import arities are not fixed per name. */
void Func_020015f4();
void Func_02001642();
void Func_020016ce();
void Func_020016da();
void Func_02001618();
void Func_020016be();
s32 Func_0200164e();
void Func_020016fa();
void Func_02001688();
void Func_020016ae();
void Func_020016bc();
void Func_02001748();
void Func_0200167e();
void Func_02001740();
void Func_020016a4();

                     

                     

                     

void Func_02000894(void)
{
    u8 *workspace;
    u16 *skipped;

    Func_020015f4();

    Func_02001642(0, 304, 312);
    Func_020016ce(0, 0xc000, 0);
    Func_020016da(28, 0x4000, 0);

    Func_02001618(20);

    Func_020016be(0x0e3d);
    Func_020016ce(28, 0);

    if (Func_0200164e(0, 0) == 0) {
        /* Skip-beat counter: the u16 at workspace + 472. */
        workspace = *(u8 **)0x03001ebc;
        skipped = (u16 *)(workspace + 472);
        *skipped = (u16)(*skipped + 1);

        Func_020016fa(28, 0);
        Func_02001688(28, 0x10000, 0x8000);
        Func_020016ae(28, 320, 304);
        Func_020016bc(28, 316, 328);
        Func_02001748(28, 0xa000, 0);
        Func_0200167e(0x8c1);
    } else {
        Func_02001740(28, 0);
    }

    Func_020016a4();
}
