#include "TYPES.H"

struct Obj {
    u8 unknown_00[24];
    s32 a;
    s32 b;
    u8 unknown_20[68];
    s16 frame;
};

extern s32 Data_0200c468[];

void ArutamiraDou_Func02000ee0(struct Obj *obj)
{
    s32 v = Data_0200c468[(u16)(obj->frame >> 2) & 3];

    obj->a = v;
    obj->b = v;
    obj->frame++;
    obj->frame &= 15;
}
