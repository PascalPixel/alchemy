#include "types.h"

/*
 * resource_3b2 owner at 0x020013ac, 232 bytes.  Its executable ranges are
 * 0x020013ac-0x020013f1 and 0x02001418-0x02001479; between them sits the
 * owner's 9-entry jump table at 0x020013f4-0x02001417 (preceded by a two-byte
 * alignment `movs r0, r0`), and after them the six-word literal pool
 * 0x0200147c-0x02001493.  The pool map was derived by walking the owner's
 * control flow from its prologue, not from the "pool words referenced" list.
 *
 * Complete owner: `push {r5, r6, r7, lr}` prologue and the single epilogue at
 * 0x02001474, `pop {r5, r6, r7} / pop {r0} / bx r0` — so the owner is **void**.
 *
 * THIRD independent confirmation of the 0x02008000 link base for this overlay:
 * the dispatcher loads its table base as the pool word 0x020093f4 while the
 * table physically sits at file offset 0x13f4, and its nine entries
 * 0x02009418, 0x0200941c, 0x02009420, 0x02009424, 0x02009428, 0x02009432,
 * 0x0200943c, 0x02009446, 0x02009450 are exactly the nine case bodies at
 * 0x1418, 0x141c, 0x1420, 0x1424, 0x1428, 0x1432, 0x143c, 0x1446, 0x1450.
 * None carries the Thumb bit, as expected for a `mov pc, rN` table.
 *
 * Call targets resolved with `bun tools/lib/overlay_call_targets.ts resource_3b2
 * 13ac --json` (12 sites, 8 distinct targets, all import veneers):
 *   0x020013c2 -> 0x02003058 -> Func_080770c0
 *   0x020013ca -> 0x02003070 -> Func_0808a018
 *   0x020013d4 -> 0x020030e8 -> Func_0808a260
 *   0x0200142c, 0x02001436, 0x02001440, 0x0200144a, 0x02001454
 *              -> 0x020030e0 -> Func_0808a250
 *   0x02001458 -> 0x02003078 -> Func_0808a020
 *   0x02001464 -> 0x02003020 -> Func_08009178
 *   0x0200146a -> 0x02003118 -> Func_080f9010
 *   0x02001470 -> 0x020030d8 -> Func_0808a248
 *
 * Behaviour: the overlay's scene-entry dispatcher.  `Data_02000240[224]` is the
 * scene id; the code derives a companion selector from it (id + 0x8c8 - 0x7e)
 * and a nine-way case index (id - 0x7e).  When the companion is ALREADY in the
 * party the whole scene is skipped and a canned message
 * (`Data_0200b2bc`, file offset 0x32bc) is shown instead, with a sound cue.
 * Otherwise the scene is armed: the table byte at +555 is set to 3 and the case
 * index selects one of two cutscene groups (63 for indices 0..3, 84 for 4..8)
 * together with a variant number 0..4.
 *
 * NOTE on call sites: the nine jump-table arms funnel into only FIVE `bl`
 * instructions — arms 0/4 share one, 1/5 another, 2/6, 3/7, and 8 has its own —
 * differing only in the group number the arm left in r0.  Writing one call per
 * arm would inject four phantom calls, so the C spells exactly five.
 *
 * UNCERTAINTY: the byte at `(u8 *)Data_02000240 + 555` is set to 3 and never
 * read here; the byte-exact siblings write neighbouring elements of the same
 * table, so it is left as a raw offset rather than named.
 */

extern s16 Data_02000240[];             /* the overlay's scene table */
extern u8 Data_0200b2bc[];              /* in-image message data, offset 0x32bc */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_080770c0();                    /* companion entity by selector, or 0 */
void Func_0808a018();                   /* entity hook, r0 residue */
void Func_0808a020();                   /* common exit hook, no arguments */
void Func_0808a248();                   /* scene teardown */
void Func_0808a250();                   /* run cutscene (group, variant) */
void Func_0808a260();                   /* arm scene (id, mode) */
void Func_08009178();                   /* show a message block */
void Func_080f9010();                   /* play a sound cue */

void Func_020013ac(void)
{
    s32 scene;
    s32 group;

    if (Func_080770c0(Data_02000240[224] + (0x8c8 - 0x7e)) != 0) {
        goto already_recruited;
    }

    Func_0808a018();
    Func_0808a260(Data_02000240[224], 5);

    ((u8 *)Data_02000240)[555] = 3;

    scene = Data_02000240[224] - 0x7e;
    if ((u32)scene <= 8) {
        group = (scene <= 3) ? 63 : 84;

        /* Five shared call sites; arms 0/4, 1/5, 2/6 and 3/7 pair up. */
        switch (scene) {
        case 0:
        case 4:
            Func_0808a250(group, 0);
            break;
        case 1:
        case 5:
            Func_0808a250(group, 1);
            break;
        case 2:
        case 6:
            Func_0808a250(group, 2);
            break;
        case 3:
        case 7:
            Func_0808a250(group, 3);
            break;
        default:
            Func_0808a250(group, 4);
            break;
        }
    }

    Func_0808a020();
    return;

already_recruited:
    /* Already recruited: show the canned line and leave. */
    Func_08009178(Data_0200b2bc, 44, 7);
    Func_080f9010(183);
    Func_0808a248(3);
}
