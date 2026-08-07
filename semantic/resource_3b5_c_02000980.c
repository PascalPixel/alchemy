#include "types.h"

/*
 * Resource 3b5, cutscene script step at 0x02000980 (808 bytes; 782 bytes of
 * code, two bytes of alignment, and the 24-byte literal pool at
 * 0x02000c90-0x02000ca7).  The pool map is from a control-flow walk: the owner
 * is straight-line apart from one two-armed test, nothing is branched over,
 * and everything from 0x02000c90 on is reached only as `ldr [pc]` data.
 *
 * Complete owner: `push {r5, lr}` at 0x02000980 and
 * `pop {r5} / pop {r0} / bx r0` at 0x02000c88-0x02000c8c.  r0 holds the popped
 * return address: void.
 *
 * Role: named in advance by the cutscene script table, which holds 0x02008981
 * in a record's handler word — Func_02000980 + the Thumb bit under this
 * overlay's proven 0x02008000 link base.  No arguments.  It is the overlay's
 * climax beat: it stages actors 29, 30 and 32, plays the effect, then splits
 * into a played and a skipped variant.
 *
 * r5 is a long-lived alias that is REASSIGNED mid-owner, which an argument
 * simulator gets silently wrong: it holds the text id 0x1fb6 from 0x020009b4
 * (bumped to 0x1fb9 by `adds r5, #3` at 0x02000a96, which is the id passed to
 * Func_08015038), and from 0x02000ab0 onward it holds the 0x03001ebc
 * workspace-pointer cell instead.  Tracked per use below, not as one variable.
 *
 * The store at 0x02000abc is the displacement/value trap: r2 is built as
 * 250 << 1 = 500 for the displacement, then reloaded with 32 as the value.
 * It is workspace[500] = 32.
 *
 * The eight-instruction sequence at 0x02000be6-0x02000bf2 is the project's
 * skip-beat counter (`movs r3, #236 / lsls #1` off the 0x03001ebc workspace
 * pointer, i.e. the u16 at workspace + 472).  It appears on only one arm.
 *
 * The two arms are NOT merged even where they run the same import sequence:
 * per project rule the multiset is over call SITES, not over distinct
 * behaviour, and folding them would deflate it by roughly twenty.
 *
 * Call targets resolved with `tools/lib/overlay_call_targets.ts`; the inventory's
 * `calls=80` is a documented lower bound on distinct targets.
 */

/* Old-style declarations: overlay import arities are not fixed per name. */
void Func_0808a018();
void Func_0808a210();
void Func_0808a090();
void Func_0808a170();
void Func_0808a0f0();
void Func_0808a158();
u8 *Func_0808a080();
void Func_080091e0();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a1b8();
void Func_0808a0e8();
void Func_0808a100();
void Func_0808a148();
void Func_0808a010();
void Func_0808a1f0();
void Func_0808a130();
void Func_0808a138();
void Func_0808a178();
void Func_080150f8();
void Func_08015038();
s32 Func_0808a070();
void Func_0808a180();
void Func_0808a110();
void Func_080770c8();
void Func_0808a020();

void Func_02000980(void)
{
    s32 text = 0x1fb6;
    u8 *workspace;
    u16 *skipped;

    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);

    Func_0808a090(29, 0x10000, 0x8000);
    Func_0808a090(30, 0x10000, 0x8000);

    Func_0808a170(text);

    Func_0808a0f0(29, 0x480000, 0xd00000);
    Func_0808a0f0(30, 0x380000, 0xd00000);

    Func_0808a158(32, 15);
    Func_080091e0(Func_0808a080(32), 0);
    Func_0808a0f0(32, 0x5f0000, 0x280000);

    Func_0808a0c8(29, 72, 248);
    Func_0808a0c8(30, 56, 248);
    Func_0808a0d0(0, 64, 264);
    Func_0808a1b8(0, 0xc000, 0);

    Func_0808a0e8(29);
    Func_0808a100(29, 1);
    Func_0808a100(30, 1);
    Func_0808a100(0, 1);
    Func_0808a148(29, 0, 0);
    Func_0808a148(30, 0, 0);

    Func_0808a010(20);
    Func_0808a1f0(29, 258);
    Func_0808a1f0(30, 258);
    Func_0808a130(29, 2);
    Func_0808a138(30, 2);
    Func_0808a010(20);
    Func_0808a178(29, 0);
    Func_0808a010(25);

    Func_080150f8(52, 0, 12, 7);
    text += 3;
    Func_08015038(text, 11, 12, 2);

    *(s32 *)(workspace + 500) = 32;
    workspace = *(u8 **)0x03001ebc;

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(20);
        Func_0808a138(30, 2);
        Func_0808a010(30);
        Func_0808a1b8(30, 0, 0);
        Func_0808a010(30);
        Func_0808a010(10);
        Func_0808a110(29, 3);
        Func_0808a010(20);
        Func_0808a1b8(29, 0, 0);
        Func_0808a010(30);
        Func_0808a180(29, 0);
        Func_0808a010(20);
        Func_0808a1b8(29, 0x4000, 0);
        Func_0808a1b8(30, 0x4000, 0);
        Func_0808a010(30);
        Func_0808a100(29, 3);
        Func_0808a110(30, 3);
        Func_0808a010(20);
        Func_0808a090(29, 0x1cccc, 0xe666);
        Func_0808a090(30, 0x1cccc, 0xe666);
        Func_0808a0c8(29, 232, 248);
        Func_0808a010(2);
        Func_0808a0c8(30, 232, 248);
        Func_0808a0e8(29);
        Func_0808a0c8(29, 248, 248);
        Func_0808a0d0(30, 248, 248);
    } else {
        Func_0808a010(20);
        Func_0808a138(30, 2);
        Func_0808a010(30);
        Func_0808a1b8(30, 0, 0);
        Func_0808a010(30);
        Func_0808a010(10);
        Func_0808a110(29, 4);
        Func_0808a010(20);
        Func_0808a1b8(29, 0, 0);
        Func_0808a010(30);

        /* Skip-beat counter: the u16 at workspace + 472. */
        *skipped = (u16)(*skipped + 1);
        skipped = (u16 *)(*(u8 **)0x03001ebc + 472);

        Func_0808a180(29, 0);
        Func_0808a010(20);
        Func_0808a1b8(29, 0x4000, 0);
        Func_0808a1b8(30, 0x4000, 0);
        Func_0808a010(30);
        Func_0808a100(29, 3);
        Func_0808a110(30, 3);
        Func_0808a010(20);
        Func_0808a090(29, 0x19999, 0xcccc);
        Func_0808a090(30, 0x19999, 0xcccc);
        Func_0808a0c8(29, 72, 184);
        Func_0808a0d0(30, 56, 184);
    }

    Func_0808a0f0(29, 0, 0);
    Func_0808a0f0(30, 0, 0);
    Func_0808a0f0(32, 0, 0);

    Func_080770c8(0x8c0);
    Func_0808a020();
}
