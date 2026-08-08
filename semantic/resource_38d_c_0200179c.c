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
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_38d 179c --json`, which reports
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
void Func_02003c10();
void Func_02003d3c();
void Func_02003d50();
void Func_02003d14();
void Func_02003c5e();
void Func_02003ca4();
void Func_02003c3a();
void Func_02003d64();
void Func_02003c44();
void Func_02003daa();
void Func_02003c20();
void Func_02003c26();
void Func_02003d04();
void Func_02003c1a();
void Func_02003cbc();
void Func_02003d02();
void Func_02003c60();
void Func_02003c66();
void Func_02003d82();
void Func_02003da4();
void Func_02003dc8();
void Func_02003d46();
void Func_02003dda();
void Func_02003d5e();
void Func_02003df2();
void Func_02003da2();
void Func_02003dea();
void Func_02003db2();
void Func_02003e0a();
void Func_02003e14();
void Func_02003e36();
void Func_02003e48();
void Func_02003e06();
void Func_02003e40();
void Func_02003e00();
void Func_02003dee();
void Func_02003e18();
void Func_02003d86();
void Func_02003d84();
void Func_02003da8();
                                /* wait n frames */
                                /* channel control, entering the cue */
                                /* channel control, leaving the cue */
                                /* wait n frames */
                                /* scripted-scene bracket: open */
                                /* scripted-scene bracket: close */
                                /* 16.16 scale pair */
                                /* place an entity */
                                /* place slot at (x, z) */
                                /* scene-presentation request */
                                /* select an entity presentation */
                                /* show a dialogue line by id */
                                /* wait for the slot's action to finish */
                                /* wait for the slot's action, with a duration */
                                /* scene request; interface not established */
                                /* face slot along heading */
                                /* play animation id for n frames */
                                /* scene request; interface not established */
                                /* scene request; interface not established */
                                /* scene bracket helper; no arguments set */
                                /* scene bracket helper; no arguments set */
                                /* set a story flag */
                                /* clear a story flag */
                                /* play a sound cue */

void Func_0200179c(void)
{
    Func_02003c10();
    Func_02003d3c();
    Func_02003d50();

    Func_02003d14(19, 0x3000, 0);
    Func_02003c5e(0, 0x9999, 0x4ccc);
    Func_02003ca4(0, 256, 660);
    Func_02003c3a(20);
    Func_02003d64(-1, -1, -1, 0);

    Func_02003c44(512);
    Func_02003daa(188);

    Func_02003c20(1);
    Func_02003c26(2);
    Func_02003d04(19, 0x1000000, 158 << 18);
    Func_02003c1a(1);
    Func_02003cbc(19, 0x9999, 0x4ccc);
    Func_02003d02(19, 256, 644);
    Func_02003c60(1);
    Func_02003c66(2);

    Func_02003ca4(20);
    Func_02003d64(19, 2);
    Func_02003d82(0x145e);
    Func_02003da4(19, 0, 10);

    Func_02003dc8(0, 256, 40);
    Func_02003d46(0, 264, 660);
    Func_02003dda(0, 0x8000, 0);
    Func_02003d5e(19, 248, 660);
    Func_02003df2(19, 0x1000, 40);
    Func_02003da2(19, 4);
    Func_02003dea(19, 0);
    Func_02003db2(19, 3);
    Func_02003e0a(19, 0);
    Func_02003dda(19, 2);
    Func_02003e14(19, 0, 10);

    Func_02003e36(0, 0x101, 60);
    Func_02003e48(19, 258);
    Func_02003d46(60);
    Func_02003e06(19, 1);
    Func_02003e40(19, 0, 10);
    Func_02003e00(19, 3);
    Func_02003e48(19, 0);

    Func_02003daa(19, 0xcccc, 0x6666);
    Func_02003dee(19, 248, 772);
    Func_02003e18(19, 0, 0);

    Func_02003d86(0x12f);
    Func_02003d84(0x84f);

    Func_02003da8();
}
