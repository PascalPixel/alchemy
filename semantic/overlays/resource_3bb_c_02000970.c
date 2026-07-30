typedef signed int s32;

/*
 * resource_3bb owner at 0x02000970, 64 bytes (0x02000970-0x020009af):
 * 54 bytes of code, two alignment bytes at 0x020009a6, and the two-word
 * literal pool at 0x020009a8 holding the addresses of the two status words
 * this owner polls.
 *
 * Prologue `push {r5, lr}`, epilogue `pop {r5} / pop {r0} / bx r0`: r0 holds
 * the popped return address, so the owner is void.  r5 is the timeout
 * counter and is the only callee-saved register used.
 *
 * Both `bl` sites reach the same import veneer at file offset 0x3da0, whose
 * trailing word names Func_080000c0 (targets resolved with
 * tools/overlay_call_targets.ts: target offset = stored displacement + 2,
 * never the disassembler's annotation - it reports the two identical
 * halfwords as different callees).  That accounts for the row's calls=2.
 *
 * The pool words 0x0200c834 and 0x0200c838 lie in this overlay's 0x02008000
 * link band (link base confirmed by the byte-exact sibling
 * assets/code/resource_3bb_c_02000950.c, whose 0x02008715 is this image's
 * offset 0x714 plus the Thumb bit), so they are in-image words at file
 * offsets 0x4834 and 0x4838 rather than RAM globals.
 *
 * Shape: request the wait once with 10, then spin until the first word
 * reaches 0 with the second word equal to 75, giving up after 600 polls.
 * The words are read afresh on every pass, so they are declared volatile;
 * only the first is reloaded when the pair test fails on the second.
 *
 * Uncertainty: what the two words mean is not established here - only that
 * some other task publishes them while this owner spins.
 */

/* Import veneer, named by the main-image function it reaches.  Old-style
 * declaration: arities vary between call sites in this overlay. */
void Func_080000c0();

/* In-image status words at file offsets 0x4834 and 0x4838. */
extern volatile s32 Data_0200c834;
extern volatile s32 Data_0200c838;

void Func_02000970(void)
{
    s32 polls;

    Func_080000c0(10);

    polls = 0;
    while (Data_0200c834 != 0 || Data_0200c838 != 75) {
        Func_080000c0(1);
        polls = polls + 1;
        if (polls >= 600) break;
    }
}
