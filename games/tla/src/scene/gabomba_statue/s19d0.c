#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_020056b4(struct ObjectRuntime *object);
void Func_020056bc(void);
void Func_0200580c(s32 value);
struct ObjectRuntime *Func_020057bc(void);
void Func_020057a4(s32 speed, s32 acceleration);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_020057b4(void);
void Func_02005854(s32 sound);
void Func_020020f0(s32 value);
void Battle_WaitMode0(s32 frames);
void GameFlag_SetBit(s32 flag);

static __inline__ void Camera_SetBounds(s32 x, s32 y, s32 z, s32 mode)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Motion_CamBounds(x, y, z, mode);
}

void Func_020019d0(void)
{
    struct ObjectRuntime *actor = Object_GetById(26);

    Func_020056b4(actor);
    Func_0200580c(0);
    Func_020057bc()->motionFlags = 0;
    Func_020057a4(0x9999, 0x1333);
    Camera_SetBounds(180 << 17, 0, 248 << 17, 1);
    Func_020057b4();
    Func_02005854(586);
    Func_020020f0(0);
    Battle_WaitMode0(40);
    Func_02005854(582);
    actor->motionFlags = 0;
    actor->y = 0x80000;
    Battle_WaitMode0(10);
    actor->y = 0;
    Battle_WaitMode0(10);
    Func_020020f0(-1);
    GameFlag_SetBit(0x8cd);
    Battle_WaitMode0(20);
    Func_020056bc();
}
