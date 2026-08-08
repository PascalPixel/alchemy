#include "types.h"

/*
 * resource_38d owner at 0x02000568, 140 bytes: code 0x02000568-0x020005e3 and
 * the four-word literal pool at 0x020005e4-0x020005f3.  A control-flow walk
 * from the prologue reaches the pool only through `ldr rN, [pc, #imm]`; both
 * early exits branch to 0x020005e0, which is the epilogue, and the last
 * instruction before the pool is `bx r0`.  No pool word is decoded as an
 * instruction.
 *
 * A gated scripted beat on slot 19: it runs only when story flag 0x84e is
 * clear AND story flag 0x322 is set.  When it runs it brackets the body with
 * the Func_0808a018 / Func_0808a020 pair, plays an animation, turns the slot,
 * selects a presentation, waits, shows a dialogue line, waits for the slot's
 * action, then scales and places entity 0 before turning slot 19 again.
 *
 * This owner is one of the two link-base witnesses for the overlay: the word
 * 0x02008569 near the end of `assets/code/resource_38d_overlay.s` is this
 * function's entry plus the Thumb bit under the 0x02008000 base (the other is
 * 0x020085f5 = Func_020005f4 + 1).  So it is installed as a handler somewhere
 * in the overlay's own tables; nothing in the body depends on the base.
 *
 * Call accounting: 13 `bl` sites, all resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_38d 0568 --json`: Func_080770c0
 * x2, Func_0808a1b8 x2, and one each of Func_0808a018, Func_0808a1e8,
 * Func_0808a138, Func_0808a010, Func_0808a170, Func_0808a180, Func_0808a090,
 * Func_0808a0d0, Func_0808a020 - 11 distinct targets, matching the tool's
 * count for the owner.  Every site appears below exactly once and the
 * per-target multisets match.  No intra-overlay call and no `call_via` slot:
 * neither r3 nor r4 is ever written.
 *
 * The epilogue is `pop {r0} / bx r0`, so r0 holds the popped return address
 * and the owner is void.  The prologue is `push {lr}` and the first branch is
 * taken with only r0 written (the flag id), so the owner takes no arguments.
 *
 * Immediates are written as the assembly builds them: 128 << 1 = 256,
 * 224 << 7 = 0x7000, 128 << 9 = 0x10000, 128 << 8 = 0x8000, 154 << 2 = 616,
 * 208 << 8 = 0xd000, and 762 is the pool word 0x000002fa.
 *
 * UNCERTAINTY: the Func_0808a090 / Func_0808a0d0 pair addresses entity 0 while
 * everything else addresses slot 19; both selectors are literal in the bytes.
 */

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
           /* wait n frames */
           /* scripted-scene bracket: open */
           /* scripted-scene bracket: close */
           /* 16.16 scale pair */
           /* place an entity */
           /* select an entity presentation */
           /* show a dialogue line by id */
           /* wait for the slot's action to finish */
           /* face slot along heading */
           /* play animation id for n frames */
            /* story-flag test; used in a condition */

extern s32 Func_020029be();
extern s32 Func_020029c8();
extern void Func_020029f0();
extern void Func_02002af4();
extern void Func_02002af8();
extern void Func_02002ac0();
extern void Func_02002a0e();
extern void Func_02002ae4();
extern void Func_02002afc();
extern void Func_02002a62();
extern void Func_02002aa6();
extern void Func_02002b3a();
extern void Func_02002a56();
void Func_02000568(void)
{
    if (Func_020029be(0x84e) != 0) {
        return;
    }
    if (Func_020029c8(0x322) == 0) {
        return;
    }

    Func_020029f0();

    Func_02002af4(19, 256, 0);
    Func_02002af8(19, 0x7000, 10);
    Func_02002ac0(19, 2);
    Func_02002a0e(20);

    Func_02002ae4(0x1748);
    Func_02002afc(19, 0);

    Func_02002a62(0, 0x10000, 0x8000);
    Func_02002aa6(0, 616, 762);
    Func_02002b3a(19, 0xd000, 10);

    Func_02002a56();
}
