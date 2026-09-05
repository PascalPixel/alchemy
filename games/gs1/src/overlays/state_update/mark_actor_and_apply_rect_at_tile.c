#include "types.h"

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[15];
    u8 unk23;
    u8 filler24[49];
    u8 unk55;
} Struct_1a14;

extern void Func_0200687a(s32, s32, s32, s32, s32, s32);

void SceneState_MarkActorAndApplyRectAtTile(Struct_1a14 *obj)
{
    s32 x;
    s32 z;

    obj->unk23 |= 2;
    obj->unk55 = 0;
    x = obj->unk8 >> 20;
    z = obj->unk10 >> 20;
    Func_0200687a(9, 24, 1, 1, x, z);
}
