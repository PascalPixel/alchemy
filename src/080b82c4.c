#include "types.h"

struct Object_080b82c4 {
    u8 padding00[8];
    s32 x;
    u8 padding0c[4];
    s32 y;
    u8 padding14[20];
    s32 offset;
    u8 padding2c[4];
    s32 speed_x;
    s32 speed_y;
    u8 padding38[12];
    s32 unknown44;
    s32 kind;
    u8 padding4c[9];
    u8 flags55;
    u8 padding56[2];
    u8 enabled;
    u8 padding59;
    u8 visible;
};

struct ObjectSlot_080b82c4 {
    struct Object_080b82c4 *object;
};

struct ObjectSlot_080b82c4 *Func_080b7dd0(s32);
s32 Func_080022ec(s32, s32);
void Func_08009140(struct Object_080b82c4 *);
void Func_08009150(struct Object_080b82c4 *, s32, s32, s32);
void Func_08009080(struct Object_080b82c4 *, s32);

void Func_080b82c4(s32 first, s32 second, s32 divisor, s32 offset)
{
    struct ObjectSlot_080b82c4 *first_slot = Func_080b7dd0(first);
    struct ObjectSlot_080b82c4 *second_slot = Func_080b7dd0(second);
    struct Object_080b82c4 *object = first_slot->object;
    struct Object_080b82c4 *target = second_slot->object;
    s32 scale = 75;
    s32 difference_x = target->x - object->x;
    s32 start_x = object->x;
    s32 delta_x = Func_080022ec(scale * difference_x, 100);
    s32 difference_y = target->y - object->y;
    s32 start_y = object->y;
    s32 delta_y = Func_080022ec(scale * difference_y, 100);
    s32 x = start_x + delta_x;
    s32 y = start_y + delta_y;
    s32 short_x = delta_x >> 8;
    s32 short_y = delta_y >> 8;
    s32 distance;

    distance = ((s32 (*)(s32))0x030001d8)(
        short_x * short_x + short_y * short_y);
    distance = Func_080022ec(distance << 8, divisor);
    object->speed_y = distance;
    object->speed_x = distance;
    object->enabled = 1;
    if (object->flags55 & 4)
        object->offset = offset;
    object->offset = offset;
    object->kind = 0xab85;
    object->visible = 1;
    Func_08009140(object);
    Func_08009150(object, x, 0, y);
    Func_08009080(object, 2);
}
