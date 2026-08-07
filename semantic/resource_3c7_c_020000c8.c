#include "types.h"

/*
 * Resource 3c7 examine handler at 0x020000c8.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020000c8 through the interworking
 * return `pop {r5, r6} / pop {r0} / bx r0` at 0x0200014a.  r0 receives the
 * popped return address, so the owner returns nothing (HANDOVER §0).
 *
 * Two interior literal pools, both established by a control-flow walk from the
 * prologue rather than from the "pool words referenced" listing:
 * 0x020000f2-0x020000f7 (alignment halfword plus the 0xffffc000 mask, hopped
 * by the `bne` at 0x020000e6) and 0x02000150-0x0200015b (the flag id and the
 * two message-id bases, past the epilogue).  Neither is ever executed.
 *
 * This is the long member of the five-owner family at 0x020000c8, 0x0200015c,
 * 0x020001c8, 0x02000234 and 0x0200029c.  All five open identically; this one
 * additionally splits its "not facing" branch on a second predicate, using a
 * message-id BASE held in r5 and offset by 0 / 1 / 2 — the same handle+0/1/2
 * idiom the byte-exact resource_3b8 sources use.
 *
 * `Func_0808a080(0)` is the scene-record accessor.  The halfword at record
 * offset 6 is the player's heading; adding 0x2000 and keeping bits 14-15
 * quantises it to a quadrant, and the `lsls #16` before the compare only moves
 * that field into the top half of the word, so the test is "quadrant ==
 * 0xc000".
 *
 * Call accounting, per target, against `overlay_call_targets.ts`
 * (12 sites, 8 distinct):
 *   Func_0808a080  1    Func_080770c0  1    Func_080b0008  1
 *   Func_0808a170  4    Func_0808a180  2    Func_0808a178  1
 *   Func_0808a070  1    Func_0808a010  1
 *
 * Uncertainties: 0x9a7 is a story-flag id whose event is not established here;
 * Func_0808a070(0, 0) is called with both arguments zero and only its truth
 * value is used, so what it queries is likewise unestablished.
 */

/* Import veneers.  Old-style declarations: arity is deliberately left open,
 * but the two imports used in conditions must declare a return type. */
u16 *Func_020012e4();
void Func_020013e2();
s32 Func_020012d8();
void Func_02001382();
void Func_0200139a();
void Func_02001394();
void Func_020013a4();
s32 Func_02001334();
void Func_02001326();
void Func_020013b4();
void Func_020013bc();
void Func_020013d4();

                     

                     

void Func_020000c8(s32 actor)
{
    u16 *scene = Func_020012e4(0);
    s32 message;

    if ((((s32)scene[3] + 0x2000) & 0xc000) == 0xc000) {
        Func_020013e2(0x20, actor);
        return;
    }

    if (Func_020012d8(0x9a7) != 0) {
        Func_02001382(0x28f0);
        Func_0200139a(actor, 0);
        return;
    }

    message = 0x26e3;
    Func_02001394(message);
    Func_020013a4(actor, 0);
    if (Func_02001334(0, 0) == 0) {
        Func_02001326(10);
        Func_020013b4(message + 1);
    } else {
        Func_020013bc(message + 2);
    }
    Func_020013d4(actor, 0);
}
