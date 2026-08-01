/*
 * Correctness fix, veneer audit (mars, 2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * UNCERTAINTY, and it is deliberate.  What 0x03000164 DOES is not
 * established.  semantic/main/080c1ffc.c calls it a resident two-argument
 * owner initializer; across the tree it is reached with two arguments at
 * some sites and three at others, and where a third is passed it is almost
 * always zero.  It also sits four bytes -- one ARM instruction -- from the
 * fill at 0x03000168, the way the sin/cos pair at 0x0800231c/0x08002322
 * does.  That is suggestive of two entry points into one routine and it is
 * NOT asserted here: the evidence is recorded so the exact lane can settle
 * it, and the type below says only what this call site proves.
 */
#include "types.h"

typedef void (*Resident_03000164)(void *destination, u32 size, u32 value);

struct DeferredWrite_080cdbc0 {
    u32 value;
    u32 address;
    u32 mask;
};

void Func_080f9010(s32 sound);
void Func_08004278(void *callback);
void Func_080030f8(u32 frames);
void Func_080b5038(s32 mode, u16 object, s32 arg2);
void Func_080b5048(u16 object, s32 position);

static void QueueDisplayControl_080cdbc0(u32 value)
{
    volatile u16 *interrupt_master = (volatile u16 *)0x04000208;
    volatile u16 *count = (volatile u16 *)0x02002090;
    u16 saved = *interrupt_master;

    *interrupt_master = (u16)(u32)interrupt_master;
    if (*count <= 31) {
        struct DeferredWrite_080cdbc0 *write =
            (struct DeferredWrite_080cdbc0 *)
                (0x02002090 + *count * sizeof(*write));

        (*count)++;
        write->value = value;
        write->address = 0x04000000;
        write->mask = 0x00020000;
    }
    *interrupt_master = saved;
}

void Func_080cdbc0(void)
{
    u8 *scene = *(u8 **)0x03001eec;
    u8 *runtime = *(u8 **)0x03001e74;
    u16 object = *(u16 *)(runtime + 0x648);
    s32 index;

    Func_080f9010(0x121);
    *(s16 *)0x03001ad4 = *(s32 *)(scene + 0x77a0);
    *(s16 *)0x03001ad6 = *(s32 *)(scene + 0x77a4);
    *(s32 *)0x03001cec = 0x78;
    *(s32 *)0x03001cf0 = 0x78;
    *(volatile u16 *)0x0400000c = 0x0787;
    ((Resident_03000164)0x03000164)((void *)0x06004000, 0x4000, 0x0400000c);
    Func_08004278((void *)0x080cd4b5);
    *(s16 *)0x03001ad6 = 0x20;

    QueueDisplayControl_080cdbc0(0x7341);
    *(volatile u16 *)0x04000050 = 0;
    Func_080030f8(1);
    Func_080b5038(2, object, 7);
    Func_080030f8(1);

    for (index = 0; index < 8; index++) {
        Func_080b5048(object, 0x15 - index * 3);
        Func_080030f8(1);
    }

    QueueDisplayControl_080cdbc0(0x7541);
    Func_080030f8(1);
}
