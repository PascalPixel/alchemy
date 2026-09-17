#include "TYPES.H"
#include "OBJECT_EFX.H"

struct ArcObject {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[4];
    s32 scale_x;
    s32 scale_y;
    u8 pad20[0x44];
    u16 step;
    u8 pad66[2];
    struct ArcObject *link;
};

void BattleFx_UpdateScaledArcObjectA(struct ArcObject *obj)
{
    struct ArcObject *link;
    s32 v;

    link = obj->link;
    v = (s16)++obj->step;
    if (v > 31) {
        Object_SetCallback((s32)obj, Data_0809f0b0);
        return;
    }
    v = Func_08002322(v << 10);
    obj->scale_x = v;
    obj->scale_y = v;
    obj->x = link->x;
    obj->y += 0x10000;
    obj->z = link->z + (0x10000 - v) * 5 + 0x90000;
}
