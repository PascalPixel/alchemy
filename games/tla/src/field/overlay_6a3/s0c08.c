#include "ov6a3.h"

extern u32 Data_02005524[];

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

/* FAKEMATCH: the angle passes through a block temporary: the native holds each run of equal angles in r5 (0x2000, then 0xc000), building the second while the third call is set up (a plain argument: differ 31 of 57). */
#define PLACE(id, x, z, angle) \
    { \
        s32 angle_ = (angle); \
        ObjPlace(id, (x) << 16, (z) << 16, angle_); \
    }

void Func_02000c08(void)
{
    PLACE(2, 757, 216, 0x2000);
    PLACE(0, 761, 219, 0x2000);
    PLACE(3, 816, 285, 0x2000);
    PLACE(1, 824, 302, 0xc000);
    PLACE(29, 821, 302, 0xc000);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(1), 0);
    ObjectMotion_EnableActionAndSetCallback(1, Data_02005524);
    {
        /* FAKEMATCH: the priority passes through a block temporary, so 0x480 is built before the callback's address is loaded (as an argument: differ 2 of 57; tried an unprototyped call, an s32 or u16 priority, a callback temporary and a forwarding inline). */
        s32 priority = 0x480;

        Scheduler_AddOrUpdateCallback((void (*)(void))Func_02000a78, priority);
    }
    Object_SetModeById(0, 38);
    Object_SetModeById(2, 31);
    Object_SetModeById(3, 9);
}
