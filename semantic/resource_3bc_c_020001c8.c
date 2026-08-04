typedef signed int s32;

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
 * one-argument frame-yield import (10 frames on entry, then 1 per poll).
 *
 * Epilogue `pop {r5} / pop {r0} / bx r0` — r0 is the popped return address,
 * so the owner is void.  r5 is the poll counter and is the reason r5 is saved.
 *
 * Control flow detail worth preserving: the counter is bumped *before* the
 * yield and the `cmp r5,#119 / bgt` test is applied *after* it, so the loop
 * performs at most 120 polling yields.
 */

extern s32 Data_0200d480;
extern s32 Data_0200d484;

void Func_02004a0e();
void Func_02004a1e();

void Func_020001c8(void)
{
    s32 polls;

    Func_02004a0e(10);

    polls = 0;
    while (Data_0200d480 != 3 || Data_0200d484 != 1) {
        polls++;
        Func_02004a1e(1);
        if (polls > 119) {
            break;
        }
    }
}
