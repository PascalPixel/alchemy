#include "types.h"

/*
 * resource_3c5 owner at 0x02000d4c, 352 bytes: code 0x02000d4c-0x02000e95, a
 * two-byte alignment halfword at 0x02000e96 and the five pool words
 * 0x02008cf1 / 0x0000cccc / 0x01090001 / 0x00000101 / 0x0000e666 at
 * 0x02000e98-0x02000eab.  The overlay's set-piece: it takes the camera and
 * subject into a fixed pose, fires a seventeen-step ring of effects, then
 * restores the presentation.
 *
 * Frame: `push {r5,r6,r7,lr}` plus `mov r7,sl / mov r6,r8 / push {r6,r7}` and
 * `sub sp, #68`.  The 68 bytes are three separate areas, and keeping them
 * apart is what makes the loop readable:
 *   sp+0  .. sp+15   the four outgoing stack words of Func_02000ae8;
 *   sp+16 .. sp+27   a 12-byte velocity vector built each iteration;
 *   sp+28 .. sp+67   the 40-byte options record handed to Func_02000ae8.
 * The epilogue is `add sp, #68 / pop {r3,r5} / mov r8,r3 / mov sl,r5 /
 * pop {r5,r6,r7} / pop {r0} / bx r0` -- `pop {r0} ; bx r0`, so r0 is the
 * popped return address and the owner is **void**.  It also takes no
 * arguments: every incoming register is overwritten before any use.
 *
 * All 28 call sites were resolved with `bun tools/overlay_call_targets.ts
 * resource_3c5 0d4c`, which reports sites=28 / distinct_targets=17 (27 veneer,
 * 1 prologue).  Site-by-site, in program order, so the accounting is complete:
 *
 *   0d56 Func_0808a018    0d68 Func_0808a210    0d6c Func_08009128
 *   0d72 Func_080000c0    0d78 Func_0808a080    0d84 Func_0808a080
 *   0d90 Func_0808a080    0d9a Func_0808a080    0da2 Func_0808a360
 *   0da6 Func_0808a370    0dac Func_0808a010    0db2 Func_080f9010
 *   0db8 Func_0808a080    0dc4 Func_0808a010    0dca Func_0808a080
 *   0dea Func_08000120    0df6 Func_08000118    0e22 Func_02000ae8
 *   0e2c Func_080f9010    0e34 Func_0808a1f0    0e3c Func_0808a100
 *   0e4c Func_080091f0    0e5a Func_080091f0    0e5e Func_080091f8
 *   0e68 Func_0808a1f0    0e6e Func_0808a080    0e7a Func_0808a080
 *   0e84 Func_0808a020
 * That is 28 sites against the inventory's `calls=28`; the multiset of names
 * below matches this list exactly.  The listing's printed `bl` targets are the
 * usual overlay decoding artefact and are not used anywhere here.
 *
 * The options record is the strongest evidence in the file, because its four
 * initialised fields are exactly the four the flag word selects.  The overlay's
 * own Func_02000ae8 (semantic/overlays/resource_3c5_c_02000ae8.c) reads
 * options+4 under bit 0x10000, options+8/+12 under bit 0x80000, and options+36
 * under bit 0x1000000; the flags passed here are 0x01090001, whose set bits are
 * 0, 0x10000, 0x80000 and 0x1000000.  Nothing else in the 40-byte record is
 * written, and nothing else is read.  The low nibble 1 additionally selects
 * descriptor Data_0200b134[1].
 *
 * The pool word 0x02008cf1 is Func_02000cf0 + the Thumb bit under this
 * overlay's proven 0x02008000 link base (file offset 0x0cf0), i.e. the
 * per-frame handler installed into each effect at +108.  Its byte-exact source
 * assets/code/resource_3c5_c_02000cf0.c integrates position from velocity and
 * decays the velocity, which is precisely what the ring needs.
 *
 * Func_08000120 and Func_08000118 are ROM dispatch entries 12 and 11
 * (-> Func_0800231c / Func_08002322), the same pair resource_39f's cousin loop
 * at 0x02000e18 uses to turn `step << 12` into a coordinate pair.  Both loops
 * run the counter to 16 *inclusive*, i.e. seventeen iterations covering a full
 * turn plus a repeat of the zero step -- kept as written rather than
 * "corrected" to sixteen.
 *
 * Uncertainties, recorded rather than guessed:
 *  - Func_0808a018 and the trailing Func_0808a020 / Func_080091f8 are called
 *    with no argument register set; they are spelled with no arguments.
 *  - The third velocity slot (vector[2]) is stored and never reloaded; the
 *    same value reaches the call in r0.  Kept as a store for fidelity.
 *  - The two Func_0808a080(0) pairs at the end write +72 then +68, the same
 *    two fields the opening block writes, so the tail restores what the head
 *    set; the exact meaning of the fields is not established here.
 */

/* Old-style declarations are mandatory in overlay sources: an import name can
 * be reached with different argument counts at different sites. */
        /* record by slot index */











       /* frame wait */
       /* sound cue */
        /* ROM dispatch[12] -> Func_0800231c */
        /* ROM dispatch[11] -> Func_08002322 */
       /* four register arguments plus four stack words */

extern void Func_02003b90();
extern void Func_02003c8a();
extern void Func_02003b1e();
extern void Func_02003af4();
extern u8 * Func_02003bca();
extern u8 * Func_02003bd6();
extern u8 * Func_02003be2();
extern u8 * Func_02003bec();
extern void Func_02003cf4();
extern void Func_02003d08();
extern void Func_02003bde();
extern void Func_02003d44();
extern u8 * Func_02003c0a();
extern void Func_02003bf6();
extern u8 * Func_02003c1c();
extern s32 Func_02003b7c();
extern s32 Func_02003b80();
extern void Func_0200190c();
extern void Func_02003dbe();
extern void Func_02003d3e();
extern void Func_02003cce();
extern void Func_02003c4e();
extern void Func_02003c5c();
extern void Func_02003c68();
extern void Func_02003d72();
extern u8 * Func_02003cc0();
extern u8 * Func_02003ccc();
extern void Func_02003cc6();
void Func_02000d4c(void)
{
    u8 *record;
    u8 *subject;
    s32 velocity[3];        /* sp+16 */
    u8 options[40];         /* sp+28 */
    u32 step;

    Func_02003b90();
    Func_02003c8a(-1, -1, -1, 0);
    Func_02003b1e();
    Func_02003af4(1);

    record = Func_02003bca(0);
    *(s32 *)(record + 12) = 0x00820000;         /* 130 << 16 */

    record = Func_02003bd6(0);
    *(s32 *)(record + 72) = 0x00008000;         /* 128 << 8 */

    record = Func_02003be2(0);
    *(s32 *)(record + 68) = 0;

    record = Func_02003bec(0);
    record[85] = 0;

    Func_02003cf4();
    Func_02003d08();
    Func_02003bde(30);
    Func_02003d44(204);

    record = Func_02003c0a(0);
    record[85] = 3;

    Func_02003bf6(24);

    subject = Func_02003c1c(0);

    /* The four fields the 0x01090001 flag word selects, and only those. */
    *(s32 *)(options + 4) = 7;
    *(u32 *)(options + 36) = 0x02008cf1;        /* Func_02000cf0 + Thumb bit */
    *(s32 *)(options + 8) = 0x0000cccc;
    *(s32 *)(options + 12) = 0x0000cccc;

    for (step = 0; step <= 16; step++) {
        s32 key = step << 12;
        s32 lateral = Func_02003b7c(key);

        velocity[0] = lateral;
        velocity[1] = 0;
        velocity[2] = Func_02003b80(key);

        /* One and a half times the dispatch value, halved with C's
         * round-toward-zero division (`lsrs #31 / adds / asrs #1`). */
        velocity[0] = lateral + lateral / 2;

        Func_0200190c(*(s32 *)(subject + 8),
                      *(s32 *)(subject + 12),
                      *(s32 *)(subject + 16),
                      velocity[0],
                      velocity[1],
                      velocity[2],
                      0x01090001,
                      options);
    }

    Func_02003dbe(188);
    Func_02003d3e(0, 0x00000101);
    Func_02003cce(0, 22);
    Func_02003c4e(0x00050000, 0x00050000, 0x00010000);
    Func_02003c5c(-1, -1, 0x0000e666);
    Func_02003c68();
    Func_02003d72(0, 256);

    record = Func_02003cc0(0);
    *(s32 *)(record + 72) = 0x00010000;         /* 128 << 9 */

    record = Func_02003ccc(0);
    *(s32 *)(record + 68) = 0x00004000;         /* 128 << 7 */

    Func_02003cc6();
}
