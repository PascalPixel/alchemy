#include "TYPES.H"

struct Obj {
    u8 unknown_00[24];
    s32 a;
    s32 b;
    u8 unknown_20[68];
    s16 frame;
};

extern s32 ArutamiraDou_PulseScales[];

void ArutamiraDou_UpdateScalePulse(struct Obj *obj)
{
    s32 v = ArutamiraDou_PulseScales[(u16)(obj->frame >> 2) & 3];

    obj->a = v;
    obj->b = v;
    obj->frame++;
    obj->frame &= 15;
}
