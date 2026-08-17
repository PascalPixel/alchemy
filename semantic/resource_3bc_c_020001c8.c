#include "types.h"

/*
 * Resource 3bc, owner at 0x020001c8.  60 strict bytes: 50 bytes of code
 * (0x01c8-0x01f9), a 2-byte alignment `nop`, and a two-word literal pool at
 * 0x01fc-0x0203.
 *
 * Pool words 0x0200d480 / 0x0200d484 are in-image data at file offsets
 * 0x5480 / 0x5484 under the proven 0x02008000 link base — the same two state
 * words that 0x02000188 clears before installing the task at Func_0200004c.
 * This owner is that handshake's waiter: it spins until the task reports
 * state 3 with acknowledgement 1, giving up after 120 yields.
 *
 * Both call sites resolve through veneer 0x02004840 to Func_080000c0, the
 * one-argument frame-yield import (((s32) &Value_0000000a) frames on entry, then 1 per poll).
 *
 * Epilogue `pop {r5} / pop {r0} / bx r0` — r0 is the popped return address,
 * so the owner is void.  r5 is the poll counter and is the reason r5 is saved.
 *
 * Control flow detail worth preserving: the counter is bumped *before* the
 * yield and the `cmp r5,#119 / bgt` test is applied *after* it, so the loop
 * performs at most 120 polling yields.
 */

extern volatile s32 Data_0200d480;
extern volatile s32 Data_0200d484;

extern u8 Value_0000000a;
void Func_02004a0e();
void Func_02004a1e();

/*
 * WHERE THIS ROW STANDS.  The goto form below is SIZE-correct at 60 bytes and 21
 * differing halfwords.  Writing the wait as an ordinary loop --
 *
 *   polls = 0;
 *   while (Data_0200d480 != 3 || Data_0200d484 != 1) { ...; polls++; if (...) return; }
 *
 * -- cuts the residual to 9 halfwords but comes out 64 bytes, four over, and the
 * same is true keeping `cond` and using `while` instead of the goto.  So the
 * natural shape is much closer in content and carries one instruction or pool word
 * the reference does not; the guide's four-byte rule read backwards.  Whoever takes
 * this next should start from the loop form and hunt those four bytes, not from
 * here.
 *
 * Two things it is NOT.  `&Value_0000000a` is right: passing the literal 10
 * instead moves the row further out, 21 halfwords to 25.  And `volatile` on
 * Data_0200d480, or on both globals, changes nothing at all -- the reference
 * dereferences the pointer once before the loop where we drop that read, which
 * looks exactly like the guide's volatile tell and is not one.  The scorer was
 * checked for caching while establishing that: inserting a call moves the count
 * and removing it restores it, so these are real measurements.
 */
void Func_020001c8(void)
{
    s32 polls;
    s32 cond;

    Func_02004a0e(((s32) &Value_0000000a));

    cond = Data_0200d480;
    polls = 0;
    goto test;

loop_body:
    Func_02004a1e(1);
    polls++;
    if (polls > 119) {
        return;
    }
    cond = Data_0200d480;

test:
    if (cond != 3) goto loop_body;
    if (Data_0200d484 != 1) goto loop_body;
}
