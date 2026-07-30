typedef signed int s32;

/*
 * resource_3bb owner at 0x020002e8, 28 bytes (0x020002e8-0x02000303):
 * 24 bytes of code plus the one-word literal pool at 0x02000300 that the
 * body loads from.
 *
 * Prologue `push {lr}` at 0x020002e8, epilogue `pop {r0} / bx r0` at
 * 0x020002fc: r0 holds the popped return address, so the owner is void.
 *
 * The single `bl` is resolved with tools/overlay_call_targets.ts (target
 * offset = stored displacement + 2, never the disassembler's annotation).
 * It reaches the import veneer at file offset 0x3fb0, whose trailing word
 * names Func_0808a140.
 *
 * The pool word 0x02000240 is below this overlay's 0x02008000 link base -
 * confirmed by the byte-exact sibling assets/code/resource_3bb_c_02000950.c,
 * whose 0x02008715 is this image's offset 0x714 plus the Thumb bit - so it
 * is a RAM global, not in-image data.  The scaled index is built as
 * 250 << 1 = 500, giving the word at 0x02000434.
 */

/* Import veneer, named by the main-image function it reaches.  Old-style
 * declaration: arities vary between call sites in this overlay. */
void Func_0808a140();

void Func_020002e8(void)
{
    s32 subject;

    subject = *(s32 *)(0x02000240 + 500);
    Func_0808a140(subject, 6, 0);
}
