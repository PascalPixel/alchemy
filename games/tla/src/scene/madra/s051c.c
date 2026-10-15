#include "types.h"
#include "object_runtime.h"
#include "flag_call.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void Func_02000404(void);
void Func_02000490(void);
void Func_02002b28(void);
void Func_02002b30(void);
void Func_02002b90(s32 id, s32 x, s32 z, s32 angle);
void Func_02002bf8(s32 id, s32 a);
void Func_02002c30(s32 a);
void Func_02002c58(s32 a);
void Func_02002c90(s32 a);

static __inline__ void ObjPlace(s32 id, s32 x, s32 z, s32 angle)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002b90(id, x, z, angle);
}

/* FAKEMATCH: the angle passes through a block temporary, as overlay 6a3's PLACE does: the native builds it first and shifts it before the positions (direct: differ 4 and 30 with shared constants). */
#define PLACE(id, x, z, angle) \
    { \
        s32 angle_ = (angle); \
        ObjPlace(id, x, z, angle_); \
    }

static __inline__ void ObjSpeed(s32 id, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void ObjSetPos(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetPositionAndReset(id, x, z);
}

static __inline__ void ObjCall2bf8(s32 id, s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002bf8(id, a);
}

void Func_0200051c(void)
{
    if (Flag_IsSet(0x910)) {
        Func_02002c90(123);
        Func_02002c30(16);
    } else if (!Flag_IsSet(0x280)) {
        Func_02002b28();
        Func_02002c58(0);
        PLACE(26, 744 << 16, 0x01010000, 0x4000);
        Object_GetById(4)->velocity_y = 0x50000;
        ObjSpeed(4, 0x20000, 0x10000);
        ObjSetPos(4, 720, 244);
        ObjCall2bf8(4, 0x2000);
        PLACE(4, 720 << 16, 244 << 16, 0x2000);
        Flag_Set(0x280);
        Func_02000404();
        Func_02002b30();
    }
}

void Func_020005b8(void)
{
    if (!Flag_IsSet(0x280)) {
        Func_02002b28();
        Func_02002c58(0);
        PLACE(26, 744 << 16, 256 << 16, 0x4000);
        Object_GetById(4)->velocity_y = 0x50000;
        ObjSpeed(4, 0x20000, 0x10000);
        ObjSetPos(4, 720, 244);
        ObjCall2bf8(4, 0x2000);
        PLACE(4, 720 << 16, 244 << 16, 0x2000);
        Flag_Set(0x280);
        Func_02000490();
        Func_02002b30();
    }
}
