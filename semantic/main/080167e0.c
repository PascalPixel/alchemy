/*
 * Correctness fix, veneer audit (mars, 2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * Every site here dispatches to the relocated IWRAM fill at 0x03000168.
 * Its signature is established, not guessed: semantic/main/080e15e8.c
 * documents it as (destination, size, value), and this file's own call
 * corroborates the third parameter directly.
 */
typedef signed int s32;
typedef unsigned int u32;

typedef void (*ArmFill)(void *destination, u32 size, u32 value);

struct DmaTransfer {
    const void *source;
    void *target;
    u32 control;
};


void Func_080167e0(s32 arg0)
{
    s32 source = 0x06002520;
    s32 width = arg0 * 6;
    s32 offset = arg0 * 24;
    s32 shifted = source + offset;
    s32 target = 0x06002500;
    s32 row = 29;

    do {
        struct DmaTransfer *transfer = (struct DmaTransfer *)0x040000d4;
        transfer->source = (const void *)shifted;
        transfer->target = (void *)source;
        transfer->control = (24 - width) | 0x84000000;
        ((ArmFill)0x03000168)(target + (32 - width) * 4, offset, 0);
        row--;
        target += 128;
        source += 128;
        shifted += 128;
    } while (row >= 0);
}
