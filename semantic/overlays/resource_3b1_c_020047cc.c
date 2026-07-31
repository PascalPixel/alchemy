typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x020047cc (160 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,lr}` at 0x020047cc, epilogue `pop {r5} / pop {r0} / bx r0` at
 * 0x02004862-0x02004866. `cmp r3,#8 / bhi` gates a 9-entry jump table
 * (`r3 = a0 - 18`, unsigned range 0..8, i.e. `a0` 18..26 inclusive;
 * anything outside that is a silent no-op). LINK BASE 0x02008000 (same
 * as the 0x020048e8 dispatcher documented in
 * work/claude/notes/resource_3b1_48e8.md and the 0x0200012c dispatcher
 * in assets/code/resource_3b1_c_0200012c.c): table pool word
 * 0x0200c7e0 resolves to file offset 0x47e0, immediately after `mov pc,
 * r3`, and its 9 entries' targets (each `value - 0x02008000`) all land
 * inside 0x02004804-0x02004862, confirming the base. The one-word pool
 * at 0x02004868 (the table-base address itself) is included per the
 * usual pool rule, immediately followed by the next owner's push {lr}
 * at 0x0200486c, already this overlay's row `0x0200486c` (already
 * adopted exact earlier this session), so the span is
 * 0x020047cc-0x0200486c, 160 bytes.
 *
 * Raw callee naming.
 */

void Func_0200ad60();
void Func_0200ad70();
void Func_0200ad72();
void Func_0200ad82();
void Func_0200ad84();
void Func_0200ad94();
void Func_0200ad96();
void Func_0200ada6();
void Func_0200ada8();
void Func_0200adb8();

void Func_020047cc(s32 a0)
{
    switch (a0) {
    case 18:
    case 20:
        Func_0200ad60(a0, 5);
        Func_0200ad70(a0, 16);
        break;
    case 19:
        Func_0200ad84(a0, 6);
        Func_0200ad94(a0, 8);
        break;
    case 24:
        Func_0200ad84(a0, 10);
        Func_0200ad94(a0, 8);
        break;
    case 21:
    case 25:
        Func_0200ad96(a0, 5);
        Func_0200ada6(a0, 4);
        break;
    case 22:
    case 23:
        Func_0200ad72(a0, 5);
        Func_0200ad82(a0, 20);
        break;
    case 26:
        Func_0200ada8(a0, 9);
        Func_0200adb8(a0, 4);
        break;
    default:
        break;
    }
}
