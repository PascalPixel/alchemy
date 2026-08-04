typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_38d owner at 0x020005f4, 236 bytes: code 0x020005f4-0x020006ad
 * (0x020006ae is the two-byte alignment `movs r0, r0`) and the twelve-word
 * literal pool at 0x020006b0-0x020006df.  A control-flow walk from the
 * prologue reaches the pool only through `ldr rN, [pc, #imm]`: the single
 * conditional branch goes forward to 0x020006aa, which is the epilogue, and
 * the last instruction before the pool is `bx r0`.  Nothing falls into it.
 *
 * The pool is why the walk matters here.  `overlay_show.ts` lists a thirteenth
 * "pool word referenced", 0x020009ec, because the real pool word 0x00004ccc at
 * 0x020006b8 decodes as `ldr r4, [pc, #816]`.  That is the documented inverse
 * trap: 0x020009ec is not part of this owner at all.
 *
 * A gated scripted beat on slot 19, run only when story flag 0x84e is set.  It
 * scales, places and turns the slot, requests presentation 3 for slots 19 and
 * 17, waits, re-presents, shows a dialogue line, waits for the slot's action,
 * then scales and places the slot again, publishes a scene word into the
 * workspace and sets story flags 0x85e and 0x333.
 *
 * The workspace pointer is the word at 0x03001ebc (IWRAM).  The store at
 * 0x02000698 is the documented "one register is both value and displacement"
 * shape: r2 is built as 224 << 1 = 448 and used as the displacement, then
 * `adds r2, #73` makes it 521 and *that* is the word stored.  It is 521 at
 * workspace + 448, not 448 + 73 anywhere else.
 *
 * Call accounting: 20 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_38d 05f4 --json`: Func_0808a110
 * x3, Func_0808a090 x2, Func_0808a0d0 x2, Func_0808a010 x2, Func_0808a148 x2,
 * Func_080770c8 x2, and one each of Func_080770c0, Func_0808a018,
 * Func_0808a1b8, Func_0808a170, Func_0808a188, Func_0808a0f0, Func_0808a020 -
 * 13 distinct targets, matching the inventory's calls=20.  Every site appears
 * below exactly once and the per-target multisets match.  No intra-overlay
 * call and no `call_via` slot: r3 is written only as workspace scratch.
 *
 * The epilogue is `pop {r0} / bx r0`, so r0 holds the popped return address
 * and the owner is void.  The prologue is `push {lr}` and the first branch is
 * taken with only r0 written, so the owner takes no arguments.
 *
 * This owner is one of the two link-base witnesses for the overlay: the word
 * 0x020085f5 near the end of `assets/code/resource_38d_overlay.s` is this
 * entry plus the Thumb bit under the 0x02008000 base.
 *
 * Immediates are written as the assembly builds them: 240 << 8 = 0xf000 and
 * 191 << 2 = 764; the rest are pool words.
 *
 * UNCERTAINTIES:
 *  - Func_0808a148 is reached once as (0, 19, 0) and once as (19, 0, 0).  Both
 *    argument sets are explicit in the bytes; whether the first is a different
 *    selector convention or a swapped pair is not decidable here.
 *  - The two scale pairs read as 16.16 fractions (0x9999/0x4ccc = 0.6/0.3 and
 *    0xcccc/0x6666 = 0.8/0.4), consistent with the established use of
 *    Func_0808a090, but the unit is not proven from this owner.
 *  - The workspace word at +448 is written and never read here.
 */

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
void Func_0808a010();           /* wait n frames */
void Func_0808a018();           /* scripted-scene bracket: open */
void Func_0808a020();           /* scripted-scene bracket: close */
void Func_0808a090();           /* 16.16 scale pair */
void Func_0808a0d0();           /* place an entity */
void Func_0808a0f0();           /* place slot at (x, z) */
void Func_0808a110();           /* scene-presentation request */
void Func_0808a148();           /* scene request; interface not established */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a188();           /* wait for the slot's action, with a duration */
void Func_0808a1b8();           /* face slot along heading */
s32 Func_080770c0();            /* story-flag test; used in a condition */
void Func_080770c8();           /* set a story flag */

void Func_020005f4(void)
{
    u8 *workspace;

    if (Func_080770c0(0x84e) == 0) {
        return;
    }

    Func_0808a018();

    Func_0808a148(0, 19, 0);
    Func_0808a090(19, 0x9999, 0x4ccc);
    Func_0808a0d0(19, 0x26e, 764);
    Func_0808a1b8(19, 0xf000, 20);
    Func_0808a110(19, 3);
    Func_0808a110(17, 3);

    Func_0808a010(20);
    Func_0808a148(19, 0, 0);
    Func_0808a010(20);
    Func_0808a110(19, 3);

    Func_0808a170(0x1749);
    Func_0808a188(19, 0, 10);

    Func_0808a090(19, 0xcccc, 0x6666);
    Func_0808a0d0(19, 0x23a, 0x2f6);
    Func_0808a0f0(19, 0, 0);

    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 448) = 521;

    Func_080770c8(0x85e);
    Func_080770c8(0x333);

    Func_0808a020();
}
