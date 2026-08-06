/*
 * resource_3c4 @ 0x02001a10 (136 bytes: 130 code + alignment + one pool word).
 *
 * Brings slot 9 up: four state writes, clear bit 1 of the byte at +35, publish
 * selector 0x204 (0x81 << 2), pin an overlay at slot 9's 12.20 grid cell, then
 * install the same handler on slots 9 and 8 and run one more service call.
 *
 * Every call site bls its own veneer, so the callees are named at the decoded
 * per-site addresses (Func_02004b84 serves two sites with different argument
 * shapes and is declared without a prototype, which is what the two call
 * sites jointly establish and no more).
 *
 * The bit-clear works through a single-set address local so the +35 is folded
 * into the returned pointer in place (adds r0,#35) instead of staging a copy.
 *
 * The pool word 0x02008ec9 is an odd (Thumb) code pointer.  This overlay is
 * linked at 0x02008000 while the inventory prints a 0x02000000 base, so it is
 * file offset 0xec8 plus the Thumb bit: the handler is Func_02008ec8.
 *
 * `add sp,#8 ; pop {r5} ; pop {r0} ; bx r0` return: void.
 */
#include "types.h"

typedef struct Slot_02001a10 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_02001a10;

struct Record_02000ec8;

/* The installed handler; see the base note above. */
void Func_02004b0a();
void Func_02004b8a();
void Func_02004b6a();
void Func_02004b7a();
u8 *Func_02004b48();
void Func_02004b2a();
Slot_02001a10 *Func_02004b60();
Slot_02001a10 *Func_02004b68();
void Func_02004b0e();
u8 *Func_02004b84();
u8 *Func_02004b8e();
s32 Func_02008ec8(struct Record_02000ec8 *record);

typedef s32 (*Handler_02001a10)(struct Record_02000ec8 *record);


void Func_02001a10(void)
{
    u8 *descriptor;
    s32 column;
    s32 row;

    Func_02004b0a();
    Func_02004b8a(9, 1);
    Func_02004b6a(9, 1);
    Func_02004b8a(9, 0);
    Func_02004b7a(9, 2);

    {
        u8 *flag = Func_02004b48(9) + 35;
        *flag &= (u8)0xfd;
    }

    Func_02004b2a(0x204);

    column = Func_02004b60(9)->column;
    row = Func_02004b68(9)->row >> 20;
    Func_02004b0e(26, 8, 1, 1, column >> 20, row);

    descriptor = Func_02004b84(9);
    *(Handler_02001a10 *)(descriptor + 108) = Func_02008ec8;

    descriptor = Func_02004b8e(8);
    *(Handler_02001a10 *)(descriptor + 108) = Func_02008ec8;

    Func_02004b84(descriptor);
}
