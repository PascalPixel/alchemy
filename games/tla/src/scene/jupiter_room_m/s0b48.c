#include "jupiter_room_m.h"

extern u32 gData_02005524[];
extern u32 gData_02005548[];
extern u32 gData_0200558c[];
extern u32 gData_020055ec[];
extern u32 gData_02005644[];
extern u32 gData_0200569c[];

void Func_0200484c(s32 id, s32 x, s32 z, s32 angle);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
s32 Func_02000a78(void);

static __inline__ void ObjPlace(s32 id, s32 x, s32 z, s32 angle)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does (direct: differ 30 of 57). */
    Func_0200484c(id, x, z, angle);
}

static __inline__ void ObjSpeed(s32 id, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does (direct: the speeds are held in r5 and r6 across the calls, four rows fewer). */
    ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

/* FAKEMATCH: the angle passes through a block temporary: the native holds each run of equal angles in r5 (0x2000, then 0xc000), building the second while the third call is set up (a plain argument: differ 31 of 57). */
#define PLACE(id, x, z, angle) \
    { \
        s32 angle_ = (angle); \
        ObjPlace(id, (x) << 16, (z) << 16, angle_); \
    }

// Sets four actors' speeds, places them and starts their action scripts.
void Func_02000b48(void)
{
    ObjSpeed(26, 0x30000, 0x18000);
    ObjSpeed(27, 0x30000, 0x18000);
    ObjSpeed(0, 0x30000, 0x18000);
    ObjSpeed(2, 0x30000, 0x18000);
    PLACE(0, 776, 202, 0x8000);
    PLACE(2, 731, 232, 0xc000);
    PLACE(27, 731, 213, 0x4000);
    PLACE(26, 737, 202, 0);
    ObjectMotion_EnableActionAndSetCallback(0, gData_0200558c);
    ObjectMotion_EnableActionAndSetCallback(2, gData_02005644);
    ObjectMotion_EnableActionAndSetCallback(26, gData_020055ec);
    ObjectMotion_EnableActionAndSetCallback(27, gData_0200569c);
    ObjectMotion_EnableActionAndSetCallback(3, gData_02005548);
}

void Func_02000c08(void)
{
    PLACE(2, 757, 216, 0x2000);
    PLACE(0, 761, 219, 0x2000);
    PLACE(3, 816, 285, 0x2000);
    PLACE(1, 824, 302, 0xc000);
    PLACE(29, 821, 302, 0xc000);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(1), 0);
    ObjectMotion_EnableActionAndSetCallback(1, gData_02005524);
    {
        /* FAKEMATCH: the priority passes through a block temporary, so 0x480 is built before the callback's address is loaded (as an argument: differ 2 of 57; tried an unprototyped call, an s32 or u16 priority, a callback temporary and a forwarding inline). */
        s32 priority = 0x480;

        Scheduler_AddOrUpdateCallback((void (*)(void))Func_02000a78, priority);
    }
    Object_SetModeById(0, 38);
    Object_SetModeById(2, 31);
    Object_SetModeById(3, 9);
}
