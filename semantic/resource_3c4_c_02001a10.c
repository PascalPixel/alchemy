/*
 * resource_3c4 @ 0x02001a10 (136 bytes: 130 code + alignment + one pool word).
 *
 * Brings slot 9 up: four state writes, clear bit 1 of the byte at +35, publish
 * selector 0x204 (0x81 << 2), pin an overlay at slot 9's 12.20 grid cell, then
 * install the same handler on slots 9 and 8 and run one more service call.
 *
 * The pool word 0x02008ec9 is an odd (Thumb) code pointer.  This overlay is
 * linked at 0x02008000 while the inventory prints a 0x02000000 base, so it is
 * file offset 0xec8 plus the Thumb bit: the handler is Func_02000ec8.
 *
 * Func_02004b84 is called twice with different argument shapes — first with
 * slot 9, then at the tail with whatever pointer the preceding fetch left in
 * r0.  It is therefore declared without a prototype, which is what the two
 * call sites jointly establish and no more.
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
void Func_080091c0();
void Func_080770c8();
void Func_0808a018();
void Func_0808a020();
Slot_02001a10 *Func_0808a080();
void Func_0808a100();
void Func_0808a158();
void Func_0808a1e0();
s32 Func_02000ec8(struct Record_02000ec8 *record);

typedef s32 (*Handler_02001a10)(struct Record_02000ec8 *record);


void Func_02001a10(void)
{
    u8 *descriptor;
    s32 column;
    s32 row;

    Func_0808a018();
    Func_0808a1e0(9, 1);
    Func_0808a100(9, 1);
    Func_0808a158(9, 0);
    Func_0808a100(9, 2);

    descriptor = Func_0808a080(9);
    descriptor[35] &= (u8)0xfd;

    Func_080770c8(0x204);

    column = Func_0808a080(9)->column;
    row = Func_0808a080(9)->row >> 20;
    Func_080091c0(26, 8, 1, 1, column >> 20, row);

    descriptor = Func_0808a080(9);
    *(Handler_02001a10 *)(descriptor + 108) = Func_02000ec8;

    descriptor = Func_0808a080(8);
    *(Handler_02001a10 *)(descriptor + 108) = Func_02000ec8;

    Func_0808a020(descriptor);
}
