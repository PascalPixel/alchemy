#include "types.h"

#define SceneState_RunSlot8OffsetStep Func_0200029c
#define OverlayObject_AdvancePositionByDelta Func_02001bdc
/* Advance both coordinate triplets by their corresponding deltas. */
struct MovingObject_02001bdc {
    u8 unknown00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown14[4];
    s32 sub_x;
    s32 sub_y;
    u8 unknown20[0x10];
    s32 sub_dx;
    s32 sub_dy;
    u8 unknown38[0x0c];
    s32 dx;
    s32 dy;
    s32 dz;
};

void Func_020026b6(s32);
void Func_02000430(s32, s32, s32);
void Func_0200043a(s32, s32, s32);
void Func_020026d0(s32);
void Func_020025ee(s32);
void Func_0200252c(s32);
void Func_02001320(void);

void SceneState_RunSlot8OffsetStep(void)
{
    s32 offset = 112;

    offset = -offset;
    Func_020026b6(0xF1);
    Func_02000430(8, offset, 0);
    Func_0200043a(8, offset, 0);
    Func_020026d0(0x121);
    Func_020025ee(0x301);
    Func_0200252c(2);
    Func_02001320();
}

void OverlayObject_AdvancePositionByDelta(struct MovingObject_02001bdc *object)
{
    object->x += object->dx;
    object->y += object->dy;
    object->z += object->dz;
    object->sub_x += object->sub_dx;
    object->sub_y += object->sub_dy;
}
