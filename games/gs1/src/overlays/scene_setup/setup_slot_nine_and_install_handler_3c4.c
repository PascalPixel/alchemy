/*
 * Slot nine setup for overlay resource_3c4.
 */
#include "types.h"

typedef struct Slot_02001a10 {
    s32 pad0;
    s32 pad4;
    s32 col;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_02001a10;

struct Record_02000ec8;

/*
 * Every call site branches through its own veneer, so the callees are named
 * per site. Func_02004b84 serves two sites with different argument shapes
 * and is declared without a prototype.
 */
void Func_02004b0a();
void Func_02004b8a_a();
void Func_02004b8a_b();
void Func_02004b6a();
void Func_02004b7a();
u8 *Func_02004b48();
void Func_02004b2a();
Slot_02001a10 *Func_02004b60();
Slot_02001a10 *Func_02004b68();
void Func_02004b0e();
u8 *Func_02004b84_a();
u8 *Func_02004b84_b();
u8 *Func_02004b8e();
s32 Func_02008ec8(struct Record_02000ec8 *record);

typedef s32 (*Handler_02001a10)(struct Record_02000ec8 *record);

/*
 * Brings slot 9 up: four state writes, clear bit 1 of the byte at +35,
 * publish selector 0x204, pin an overlay at slot 9's 12.20 grid cell, then
 * install one handler on slots 9 and 8. The 136-byte owner at 0x02001a10
 * includes its alignment halfword and its one pool word; that word is an
 * odd Thumb pointer, so the handler is Func_02008ec8. The bit-clear folds
 * +35 into the returned pointer through the address local, not into a copy.
 */
void SceneActor_SetupSlotNineAndInstallHandler(void)
{
    u8 *desc;
    s32 col;
    s32 row;

    Func_02004b0a();
    Func_02004b8a_a(9, 1);
    Func_02004b6a(9, 1);
    Func_02004b8a_b(9, 0);
    Func_02004b7a(9, 2);

    {
        u8 *flag = Func_02004b48(9) + 35;
        *flag &= (u8)0xfd;
    }

    Func_02004b2a(0x204);

    col = Func_02004b60(9)->col;
    row = Func_02004b68(9)->row >> 20;
    Func_02004b0e(26, 8, 1, 1, col >> 20, row);

    desc = Func_02004b84_a(9);
    *(Handler_02001a10 *)(desc + 108) = Func_02008ec8;

    desc = Func_02004b8e(8);
    *(Handler_02001a10 *)(desc + 108) = Func_02008ec8;

    Func_02004b84_b(desc);
}
