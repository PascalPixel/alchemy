typedef signed int s32;

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
 * `bun tools/overlay_call_targets.ts resource_38d 0568 --json`: Func_080770c0
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
void Func_0808a010();           /* wait n frames */
void Func_0808a018();           /* scripted-scene bracket: open */
void Func_0808a020();           /* scripted-scene bracket: close */
void Func_0808a090();           /* 16.16 scale pair */
void Func_0808a0d0();           /* place an entity */
void Func_0808a138();           /* select an entity presentation */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a180();           /* wait for the slot's action to finish */
void Func_0808a1b8();           /* face slot along heading */
void Func_0808a1e8();           /* play animation id for n frames */
s32 Func_080770c0();            /* story-flag test; used in a condition */

void Func_02000568(void)
{
    if (Func_080770c0(0x84e) != 0) {
        return;
    }
    if (Func_080770c0(0x322) == 0) {
        return;
    }

    Func_0808a018();

    Func_0808a1e8(19, 256, 0);
    Func_0808a1b8(19, 0x7000, 10);
    Func_0808a138(19, 2);
    Func_0808a010(20);

    Func_0808a170(0x1748);
    Func_0808a180(19, 0);

    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a0d0(0, 616, 762);
    Func_0808a1b8(19, 0xd000, 10);

    Func_0808a020();
}
