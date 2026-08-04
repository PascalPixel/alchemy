// __call_via_rN veneer site, resolved per-site against the ROM.
//
// Both `bl Func_080072f8` sites (0x080068ba, 0x080068c6) are `__call_via_r5`,
// and r5 is written once, at 0x08006884: `mov r5, sp` then `adds r5, #1`.
// The callee is the 64-byte STACK BUFFER, with bit 0 set for Thumb -- the
// same stack-kernel idiom as the nine `__call_via_sp` files, differing only
// in that the address is parked in r5 and carries an explicit Thumb bit
// rather than being installed with `mov sp`.
//
// Func_08006ac0(sp) immediately above is what fills that buffer, so the
// routine being run is written by this function before it is called. The
// resolver reports these two sites as COMPUTED because sp is not a value it
// will invent an origin for; the origin is one instruction away and is read
// here rather than guessed.
//
// ARITY: one argument. Only r0 is set at each branch (0x080068b6 loads
// 0x0e000001 from the pool, 0x080068c2 materialises 0x0e000000), and the
// result is consumed as a byte both times (`lsls #24` then `lsrs`). The
// routine must run from RAM because it reads the flash chip at 0x0e000000
// while that chip is in ID mode, which is why it is copied to the stack at
// all.
#include "types.h"

/// The routine Func_08006ac0 copies into the stack buffer: reads one byte
/// from the flash chip. Entered through the veneer with the Thumb bit set.
typedef u8 (*FlashByteRead)(s32 address);

extern void Func_08006ac0(void *);

s32 Func_08006878(void)
{
    struct {
        u8 buffer[64];
        u16 delay;
    } locals;
    s32 value;
    s32 result;

    Func_08006ac0(locals.buffer);
    {
        volatile u8 *first = (volatile u8 *)0x0e005555;
        volatile u8 *second = (volatile u8 *)0x0e002aaa;
        *first = 0xaa;
        *second = 0x55;
        *first = 0x90;
    }

    value = 20000;
    goto first_write;
first_decrement:
    value = *(volatile u16 *)&locals.delay;
    value--;
first_write:
    *(volatile u16 *)&locals.delay = value;
    if (*(volatile u16 *)&locals.delay != 0)
        goto first_decrement;

    {
        FlashByteRead readByte = (FlashByteRead)((u32)locals.buffer | 1);

        result = (u16)readByte(0x0e000001) << 8;
        result |= readByte(0x0e000000);
    }

    {
        volatile u8 *first = (volatile u8 *)0x0e005555;
        volatile u8 *second = (volatile u8 *)0x0e002aaa;
        *first = 0xaa;
        *second = 0x55;
        *first = 0xf0;
    }

    value = 20000;
    goto second_write;
second_decrement:
    value = *(volatile u16 *)&locals.delay;
    value--;
second_write:
    *(volatile u16 *)&locals.delay = value;
    if (*(volatile u16 *)&locals.delay != 0)
        goto second_decrement;
    return result;
}
