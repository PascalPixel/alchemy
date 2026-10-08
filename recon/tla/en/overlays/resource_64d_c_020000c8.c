#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 Func_020026f4();
s32 ObjectMotion_CommitPositionAndActivate();
s32 Battle_WaitMode0();
s32 VinasuIriguchi_StopSparks();
extern u8 VinasuIriguchi_UpdateSparkTrail[];

void Func_020000c8(s32 a0)
{
    s32 t0;

    t0 = Object_GetById();
    Func_020026f4();
    FIELD_AT(t0, s32, 108) = (s32)VinasuIriguchi_UpdateSparkTrail;
    FIELD_AT((t0 + 0x23), u8, 0) = 0;
    Call3(ObjectMotion_CommitPositionAndActivate, a0, 0x60, -96);
    Call3(ObjectMotion_CommitPositionAndActivate, a0, -96, 0x60);
    FIELD_AT(t0, s32, 108) = 0;
    FIELD_AT((t0 + 0x23), u8, 0) = 1;
    Call1(Battle_WaitMode0, 0x1e);
    VinasuIriguchi_StopSparks();
}

