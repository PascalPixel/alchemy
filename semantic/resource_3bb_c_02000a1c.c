#include "types.h"

/*
 * resource_3bb owner at 0x02000a1c, 276 bytes (0x02000a1c-0x02000b2f):
 * 262 bytes of code plus the three-word literal pool at 0x02000b24 (with two
 * bytes of alignment after it).
 *
 * Prologue `push {r5, r6, lr}`, epilogue `pop {r5, r6} / pop {r0} / bx r0`:
 * r0 holds the popped return address, so the owner is void.  r6 keeps the
 * result of Func_02003344 live across twenty calls, which is the only reason
 * it is saved.
 *
 * All 25 `bl` sites are placed and reach 14 distinct callees, matching the
 * inventory row's calls=25.  Targets come from tools/overlay_call_targets.ts
 * (target offset = stored displacement + 2), never from the disassembler's
 * annotations, which print each repeated callee as a different function.
 * 22 sites reach the import veneer table (0x3f38 -> Func_0808a018, 0x3f68 ->
 * Func_0808a0a0, 0x3f60 -> Func_0808a090, 0x3f78 -> Func_0808a0c8, 0x3f80 ->
 * Func_0808a0d0, 0x3f98 -> Func_0808a100, 0x3fc0 -> Func_0808a150, 0x3f30 ->
 * Func_0808a010, 0x3fa0 -> Func_0808a110, 0x4038 -> Func_0808a250, 0x4040 ->
 * Func_0808a260, 0x4048 -> Func_0808a268, 0x3ef0 -> Func_080770c8) and three
 * reach this overlay's own code at file offsets 0x3228, 0x3344 and 0x3238.
 *
 * Func_02003228 is a two-byte routine consisting of `bx lr` alone - an empty
 * hook, not a `call_via` slot.  tools/overlay_call_targets.ts classifies it as
 * `call_via` because the bank is recognised by exactly that `bx rN` shape;
 * here the register is lr and the site sets no target register, so the call
 * genuinely does nothing.  It is kept because the call is real.
 *
 * The pool words 0x02000240 and 0x00000090 are below the overlay's 0x02008000
 * link base (base witnessed by 0x02008715 = Func_02000714 + 1 in the
 * byte-exact sibling assets/code/resource_3bb_c_02000950.c), so
 * 0x02000240 + 0x22b = 0x0200046b is a RAM byte - the same byte the converted
 * resource_383 owner at 0x02001348 writes with 3 at the end of its scene.
 *
 * Shape: a scene entry.  It runs the empty hook, opens the scene, asks
 * Func_02003344 for a choice between 77 and 89, plays a ten-frame settle,
 * then places and poses participants 0 and 8 twice before closing out - the
 * outcome is folded into Func_0808a250's second argument as 3 - choice, the
 * completion byte is set to 3, entries 4 and 5 of record 0x90 are posted and
 * story id 282 is recorded.
 *
 * Uncertainty: what Func_02003344's returned value means is not established
 * here, only that it is biased by 3 - value before being handed on.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_0808a018();
void Func_0808a0a0();
void Func_0808a090();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a100();
void Func_0808a150();
void Func_0808a010();
void Func_0808a110();
void Func_0808a250();
void Func_0808a260();
void Func_0808a268();
void Func_080770c8();

/* This overlay's own routines. 0x02003228 is `bx lr` and does nothing. */
void Func_02003228(void);
s32 Func_02003344();
void Func_02003238(void);

void Func_02000a1c(void)
{
    s32 choice;
    s32 frames;

    Func_02003228();
    Func_0808a018();

    choice = Func_02003344(77, 89);
    Func_02003238();

    frames = 9;
    do {
        frames = frames - 1;
        Func_0808a0a0(8);
    } while (frames >= 0);

    Func_0808a090(8, 0x10000, 0x8000);          /* 128 << 9, 128 << 8 */
    Func_0808a0c8(8, 88, 256);                  /* 128 << 1 */
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a0d0(0, 120, 256);
    Func_0808a100(8, 1);
    Func_0808a150(0, 8, 0);
    Func_0808a010(10);
    Func_0808a100(8, 3);
    Func_0808a110(0, 3);
    Func_0808a010(20);

    Func_0808a090(0, 0x20000, 0x10000);         /* 128 << 10, 128 << 9 */
    Func_0808a090(8, 0x20000, 0x10000);
    Func_0808a0c8(0, 112, 256);
    Func_0808a0d0(8, 96, 256);
    Func_0808a100(0, 16);
    Func_0808a100(8, 9);
    Func_0808a010(10);

    Func_0808a250(72, 3 - choice);

    *(u8 *)(0x02000240 + 0x22b) = 3;

    Func_0808a260(0x90, 4);
    Func_0808a268(0x90, 5);

    Func_080770c8(282);                         /* 141 << 1 */
}
