#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_Test(s32 a);
void Func_0200279c(s32 a, s32 b, s32 c, s32 d);
void WaitFrames(s32 a);
void ObjectMotion_EnableActionAndSetCallback(s32 a, s32 b);
void ObjectMotion_SetActionVariant(s32 a, s32 b);
s32 Object_GetById(s32 a);
void Object_SetPartAttribute(s32 a, s32 b);
extern u8 Data_020047ac[];
extern u8 Data_02004848[];
extern u8 Data_020048e4[];

void Func_020027e4(void)
{
    s32 t0;
    s32 t1;

    t0 = Value1(GameFlag_Test, 0xa3d);
    if (t0 != 0) {
        t1 = Value1(GameFlag_Test, 0x109);
        if (t1 == 0) {
            Func_0200279c(0x10, 0x218, 0x168, 0);
            Func_0200279c(0x11, 0x218, 0x1a8, 1);
            Call4(Func_0200279c, 0x12, 0x244, 0x1a4, 2);
            Call1(WaitFrames, 1);
            Call2(ObjectMotion_EnableActionAndSetCallback, 0x10, (s32)Data_020047ac);
            Call2(ObjectMotion_EnableActionAndSetCallback, 0x11, (s32)Data_02004848);
            Call2(ObjectMotion_EnableActionAndSetCallback, 0x12, (s32)Data_020048e4);
            Call1(WaitFrames, 1);
        }
    }
    Call2(ObjectMotion_SetActionVariant, 0x14, 1);
    Call2(Object_SetPartAttribute, Value1(Object_GetById, 0x14), 0xf);
}

