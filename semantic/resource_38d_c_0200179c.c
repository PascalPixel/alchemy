/*
 * resource_38d owner at 0x0200179c, 444 bytes: code 0x0200179c-0x02001935
 * (0x02001936 is the two-byte alignment `movs r0, r0`) and the eight-word
 * literal pool at 0x02001938-0x02001957, which is exactly the end of the row.
 * The owner contains no branch of any kind - it is one straight-line block
 * from `push {lr}` to `bx r0` - so the control-flow walk is trivial and every
 * word past 0x02001936 is pool.
 *
 * `overlay_show.ts` lists a ninth "pool word referenced", 0x02001c70, because
 * the real pool word 0x00004ccc at 0x0200193c decodes as `ldr r4, [pc, #816]`.
 * That address is not part of this owner; the same false entry appears in the
 * owner at 0x020005f4, which pools the same constant.
 *
 * One long scripted scene on slot 19, called from the router at 0x020006e0
 * when the scene selector is 9 and story flag 0x321 is set.  It turns and
 * scales the group, plays a cue with channels 1 and 2 muted across one frame
 * wait, repositions, shows a dialogue line, runs two animation-and-wait
 * passes, then rescales, replaces and repositions slot 19 before clearing
 * story flag 0x12f and setting 0x84f.
 *
 * Call accounting: 46 `bl` sites over 24 distinct targets, all resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_38d 179c --json`, which reports
 * exactly `sites=46 distinct_targets=24 veneer=46` - every target is an import
 * veneer, there is no intra-overlay call and no `call_via` slot (r3 is written
 * once, as Func_0808a210's fourth argument, never with an IWRAM code address).
 * Per-target multiset: Func_0808a0d0 x5, Func_0808a1b8 x4, Func_0808a090 x3,
 * Func_0808a010 x3, Func_0808a138 x3, Func_0808a188 x3, Func_0808a110 x3,
 * Func_0808a180 x2, Func_0808a0f0 x2, Func_0808a1e8 x2, Func_08009188 x2,
 * Func_08009190 x2, Func_080770c8 x2, and one each of Func_0808a018,
 * Func_0808a360, Func_0808a370, Func_0808a210, Func_080f9010, Func_080000c0,
 * Func_0808a170, Func_0808a190, Func_0808a1f0, Func_080770d0, Func_0808a020.
 * Each appears below the same number of times.  The inventory row says
 * calls=40, the documented lower bound.
 *
 * The epilogue is `pop {r0} / bx r0`, so r0 holds the popped return address
 * and the owner is void.  The prologue is `push {lr}` and the first branch is
 * taken with no argument register written, so the owner takes no arguments.
 *
 * Immediates are written as the assembly builds them; the shifted forms are
 * 192 << 6 = 0x3000, 128 << 2 = 512, 128 << 1 = 256, 165 << 2 = 660,
 * 161 << 2 = 644, 128 << 17 = 0x1000000, 158 << 18, 132 << 1 = 264,
 * 128 << 8 = 0x8000, 128 << 5 = 0x1000, 129 << 1 = 258 and 193 << 2 = 772.
 * -1 is built three times by `movs rN, #1 / negs rN, rN`.
 *
 * UNCERTAINTIES:
 *  - Func_0808a360 and Func_0808a370 are called with no argument register
 *    written at all; the caller's r0-r3 reach them unchanged.  They are
 *    spelled with no arguments, like the Func_0808a018 bracket.
 *  - Func_0808a210(-1, -1, -1, 0) is its only site in the overlay; the three
 *    -1s read as "leave unchanged" but nothing here proves that.
 *  - The two scale pairs are the same 16.16 fractions the owner at 0x020005f4
 *    uses (0.6/0.3 then 0.8/0.4).
 *  - Func_0808a190 and Func_0808a1f0 have no established interface; their
 *    argument registers are set explicitly at each site and asserted as such.
 */

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
void Func_080000c0();           /* wait n frames */
void Func_08009188();           /* channel control, entering the cue */
void Func_08009190();           /* channel control, leaving the cue */
void Func_0808a010();           /* wait n frames */
void Func_0808a018();           /* scripted-scene bracket: open */
void Func_0808a020();           /* scripted-scene bracket: close */
void Func_0808a090();           /* 16.16 scale pair */
void Func_0808a0d0();           /* place an entity */
void Func_0808a0f0();           /* place slot at (x, z) */
void Func_0808a110();           /* scene-presentation request */
void Func_0808a138();           /* select an entity presentation */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a180();           /* wait for the slot's action to finish */
void Func_0808a188();           /* wait for the slot's action, with a duration */
void Func_0808a190();           /* scene request; interface not established */
void Func_0808a1b8();           /* face slot along heading */
void Func_0808a1e8();           /* play animation id for n frames */
void Func_0808a1f0();           /* scene request; interface not established */
void Func_0808a210();           /* scene request; interface not established */
void Func_0808a360();           /* scene bracket helper; no arguments set */
void Func_0808a370();           /* scene bracket helper; no arguments set */
void Func_080770c8();           /* set a story flag */
void Func_080770d0();           /* clear a story flag */
void Func_080f9010();           /* play a sound cue */

void Func_0200179c(void)
{
    Func_0808a018();
    Func_0808a360();
    Func_0808a370();

    Func_0808a1b8(19, 0x3000, 0);
    Func_0808a090(0, 0x9999, 0x4ccc);
    Func_0808a0d0(0, 256, 660);
    Func_0808a010(20);
    Func_0808a210(-1, -1, -1, 0);

    Func_080770c8(512);
    Func_080f9010(188);

    Func_08009188(1);
    Func_08009188(2);
    Func_0808a0f0(19, 0x1000000, 158 << 18);
    Func_080000c0(1);
    Func_0808a090(19, 0x9999, 0x4ccc);
    Func_0808a0d0(19, 256, 644);
    Func_08009190(1);
    Func_08009190(2);

    Func_0808a010(20);
    Func_0808a138(19, 2);
    Func_0808a170(0x145e);
    Func_0808a188(19, 0, 10);

    Func_0808a1e8(0, 256, 40);
    Func_0808a0d0(0, 264, 660);
    Func_0808a1b8(0, 0x8000, 0);
    Func_0808a0d0(19, 248, 660);
    Func_0808a1b8(19, 0x1000, 40);
    Func_0808a110(19, 4);
    Func_0808a180(19, 0);
    Func_0808a110(19, 3);
    Func_0808a190(19, 0);
    Func_0808a138(19, 2);
    Func_0808a188(19, 0, 10);

    Func_0808a1e8(0, 0x101, 60);
    Func_0808a1f0(19, 258);
    Func_0808a010(60);
    Func_0808a138(19, 1);
    Func_0808a188(19, 0, 10);
    Func_0808a110(19, 3);
    Func_0808a180(19, 0);

    Func_0808a090(19, 0xcccc, 0x6666);
    Func_0808a0d0(19, 248, 772);
    Func_0808a0f0(19, 0, 0);

    Func_080770d0(0x12f);
    Func_080770c8(0x84f);

    Func_0808a020();
}
