#include "types.h"

/* Advance both coordinate triplets by their corresponding deltas. */
struct MovingObject_02001bdc {
    u8 unknown00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown14[4];
    s32 secondaryX;
    s32 secondaryY;
    u8 unknown20[0x10];
    s32 secondaryDeltaX;
    s32 secondaryDeltaY;
    u8 unknown38[0x0c];
    s32 deltaX;
    s32 deltaY;
    s32 deltaZ;
};

void Func_02001bdc(struct MovingObject_02001bdc *object)
{
    object->x += object->deltaX;
    object->y += object->deltaY;
    object->z += object->deltaZ;
    object->secondaryX += object->secondaryDeltaX;
    object->secondaryY += object->secondaryDeltaY;
}
