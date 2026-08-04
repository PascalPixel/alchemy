#include "types.h"

struct Inner {
    u8 filler0[6];
    u16 field6;
    u16 field8;
    u8 filler10[10];
    u8 field20;
    u8 filler21;
    u16 bits : 9;
    u16 rest : 7;
};

struct Object {
    struct Inner *inner;
    u16 field4;
    u16 field6;
    u16 field8;
    u16 field10;
    u8 field12;
    u8 field13;
};

void Func_080b0a20(struct Object *object, u16 first, u16 second)
{
    struct Inner *inner = object->inner;

    object->field13 = 1;
    inner->field6 = first;
    object->field8 = first;
    object->field4 = first;
    object->field12 = 0;
    inner->bits = first;
    object->field10 = second;
    object->field6 = second;
    inner->field8 = second;
    inner->field20 = second;
}
