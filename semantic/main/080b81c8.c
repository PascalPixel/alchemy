#include "types.h"

/*
 * Reset one battle participant's object to its opening pose: the fixed scale
 * words at +0x28, +0x30 and +0x34, the 0x9999 marker at +0x48, then +0x44 and
 * the +0x5a byte cleared, then place it and start animation program 5.  The
 * placement's x comes from scaling the slot's +0x0c field by 0x14ccc.
 *
 * `ldr r3,[pc,#40]` loads 0x03000118 - the ARM fixed-point multiply relocated
 * into IWRAM - and `mov ip, pc ; bx r3` calls it.  `mov ip, pc` sets the
 * return address to the instruction after the `bx`, so the call returns into
 * the same frame with the live registers intact: an ordinary indirect call,
 * the same shape already resolved at 0x0800ebec.  The `movs r0, r0` before it
 * is alignment filler.
 *
 * Reached only as a published Thumb pointer at 0x080b5084.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_080b81c8)(s32 a, s32 b);

struct Object_080b81c8 {
    u8 unknown_00[0x28];
    s32 field_28;
    u8 unknown_2c[4];
    s32 field_30;
    s32 field_34;
    u8 unknown_38[0xc];
    s32 zero_44;
    s32 marker_48;
    u8 unknown_4c[0xe];
    u8 flag_5a;
};

struct Slot_080b81c8 {
    struct Object_080b81c8 *object;
    u8 unknown_04[8];
    s32 offset_0c;
    s32 depth_10;
};

struct Slot_080b81c8 *Func_080b7dd0(s32 id);
void Func_08009080(struct Object_080b81c8 *object, s32 program);
void Func_08009140(struct Object_080b81c8 *object);
void Func_08009150(struct Object_080b81c8 *object, s32 x, s32 y, s32 z);

void Func_080b81c8(s32 id)
{
    Multiply_080b81c8 multiply = (Multiply_080b81c8)0x03000118;
    struct Slot_080b81c8 *slot = Func_080b7dd0(id);
    struct Object_080b81c8 *object = slot->object;

    object->field_34 = 128 << 9;
    object->field_30 = 128 << 11;
    object->field_28 = 128 << 11;
    object->marker_48 = 0x9999;
    object->zero_44 = 0;
    object->flag_5a = 0;
    Func_08009140(object);
    Func_08009150(object, multiply(slot->offset_0c, 0x14ccc), 0,
                  slot->depth_10);
    Func_08009080(object, 5);
}
