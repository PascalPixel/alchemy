/*
 * resource_384 owner at 0x02000094, 124 bytes: a scripted scene, written out
 * as a straight-line script between the scene bracket.
 *
 * Complete owner: `push {lr}` at 0x02000094 through `pop {r0} / bx r0` at
 * 0x02000108-0x0200010a.  The return address is popped into r0, so the owner
 * is `void` (HANDOVER section 0, epilogue rule).  No argument register is read
 * before being written, so it takes none.  The 4-byte literal pool at
 * 0x0200010c holds 0x0000138a and is inside the advertised span.
 *
 * Link base.  Image offset 0 is the exported-entry veneer table, and its six
 * entry words plus two constant-loader words are all in the 0x02008xxx band
 * and resolve at `word - 0x8000` onto function starts: 0x020081e5 -> 0x01e4,
 * 0x02008031 -> 0x0030, 0x0200803d -> 0x003c, 0x02008045 -> 0x0044,
 * 0x0200806d -> 0x006c, 0x02008039 -> 0x0038.  Two of those (0x0044, 0x006c)
 * are already-banked byte-exact siblings in assets/code, so the 0x02008000
 * base is proven here against banked material.  This owner's only pool word,
 * 0x0000138a, is far below the band and is a plain constant.
 *
 * Sixteen call sites over nine distinct targets, all import veneers, resolved
 * with `bun tools/overlay_call_targets.ts resource_384 0094`:
 *   0x0268 -> Func_0808a010 x5      0x0290 -> Func_0808a130 x1
 *   0x0270 -> Func_0808a018 x1      0x0298 -> Func_0808a138 x2
 *   0x0278 -> Func_0808a020 x1      0x02a0 -> Func_0808a148 x2
 *   0x02a8 -> Func_0808a150 x1      0x02b0 -> Func_0808a170 x1
 *   0x02b8 -> Func_0808a180 x2
 * The inventory advertises calls=14; that field is a floor, and the true
 * figure is 16.  Every printed `bl` annotation in the listing is wrong by the
 * usual amount -- each stored displacement is the target's image offset minus
 * two -- which is why sites two apart in the listing print callees hundreds of
 * bytes apart.
 *
 * The bracket settles the shape without any dataflow work: the owner opens
 * with Func_0808a018 and closes with Func_0808a020, the scripted-scene
 * bracket this project has established across a dozen overlays.  Everything
 * between them is the script, in order, with no branches at all -- the body
 * has no conditional or backward branch of any kind, so there is nothing to
 * fold and no arm to merge.
 *
 * Argument reading is direct: each site's r0/r1/r2 are set by the immediately
 * preceding `movs` instructions and by nothing else.  The one non-immediate is
 * the pooled 0x138a handed to Func_0808a170, the established "show a dialogue
 * line by id" import.
 *
 * Uncertainties.  The slot ids 8, 9 and 10 and the mode values 1 and 2 are
 * reproduced as written; this owner gives no evidence for what they name.
 * Func_0808a148's and Func_0808a150's third argument is 0 at every site here,
 * so its role is not observable.  The frame counts handed to Func_0808a010
 * (40, 40, 20, 20, 20) are the script's pacing and are exact.
 */

/* Overlay imports (via the veneer table).  Old-style declarations are
 * mandatory in overlay sources: one import name is reached with different
 * argument counts at different sites. */
void Func_0808a010();   /* wait n frames */
void Func_0808a018();   /* scripted-scene bracket: open */
void Func_0808a020();   /* scripted-scene bracket: close */
void Func_0808a130();   /* select a slot's presentation */
void Func_0808a138();   /* select a slot's presentation */
void Func_0808a148();   /* per-slot scene request */
void Func_0808a150();   /* per-slot scene request */
void Func_0808a170();   /* show a dialogue line by id */
void Func_0808a180();   /* act on the slot / wait for its action */

void Func_02000094(void)
{
    Func_0808a018();

    Func_0808a148(8, 9, 0);
    Func_0808a010(40);

    Func_0808a148(8, 10, 0);
    Func_0808a010(40);

    Func_0808a170(0x138a);
    Func_0808a180(8, 0);

    Func_0808a130(9, 2);
    Func_0808a138(10, 2);
    Func_0808a010(20);

    Func_0808a150(8, 0, 0);
    Func_0808a010(20);

    Func_0808a138(8, 1);
    Func_0808a010(20);

    Func_0808a180(8, 0);

    Func_0808a020();
}
