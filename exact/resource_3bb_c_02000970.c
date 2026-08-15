#include "types.h"

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
 * cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --: target offset = stored displacement + 2,
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
 *
 * Routed with -fno-hoist-volatile-address (evidenced fork mode, witness is
 * this owner): stock gcc 2.96 hoists both pool addresses into r6/r7 across
 * the poll call, cross jumping merges the entry load with the loop-tail
 * reload, and the first scheduling pass drifts the counter clear between an
 * address load and its dereference.  The mode vetoes all three for volatile
 * dereferences only, and the reference then falls out instruction-exact.
 *
 * The goto spelling below is load-bearing: the reference places the test
 * block after the body with a preheader jump into it (entry load of the
 * first word, `b` to the test), and reloads only the first word on the
 * loop-tail path.  A structured while loop makes gcc emit the test block
 * first; the explicit preheader/body/test/done labels reproduce the
 * reference block order without any -freorder-blocks routing.
 */

/* Per-site veneers (raw sub_ symbols from the overlay .s), both ultimately
 * reaching the same main-image import but each is its own call-site symbol. */
void Func_02004716();
void Func_02004724();

/* In-image status words at file offsets 0x4834 and 0x4838. */
extern volatile s32 Data_0200c834;
extern volatile s32 Data_0200c838;

void Func_02000970(void)
{
    s32 polls;
    s32 first;

    Func_02004716(10);

    first = Data_0200c834;
    polls = 0;
    goto test;
body:
    Func_02004724(1);
    polls = polls + 1;
    if (polls >= 600) goto done;
    first = Data_0200c834;
test:
    if (first != 0) goto body;
    if (Data_0200c838 != 75) goto body;
done:
    return;
}
