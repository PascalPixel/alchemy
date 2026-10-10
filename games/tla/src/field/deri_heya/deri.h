#ifndef TLA_DERI_HEYA_DERI_H
#define TLA_DERI_HEYA_DERI_H

#include "types.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Battle_WaitMode0(s32 frames);
void Engine_EventSetMessage(s32 message);
void Engine_EventShowMessage(s32 actor, s32 flags);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_02000d4c(s32 actor, s32 x, s32 z);
void Func_02000d94(s32 actor, s32 jump, s32 frames);
void Func_02000cac(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height);

static inline s32 Deri_FlagChk(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    return GameFlag_Test(flag);
}

static inline void Deri_FlagSet(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    GameFlag_SetBit(flag);
}

static inline void Deri_FlagClr(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    GameFlag_ClearBit(flag);
}

static inline void Deri_Wait(s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Battle_WaitMode0(frames);
}

static inline void Deri_MesSet(s32 message)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Engine_EventSetMessage(message);
}

static inline void Deri_MesPut(s32 actor, s32 flags)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Engine_EventShowMessage(actor, flags);
}

static inline void Deri_ObjLaunch(s32 actor, s32 speed, s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_Launch(actor, speed, frames);
}

static inline void Deri_ObjWalk(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetPositionAndReset(actor, x, z);
}

static inline void Deri_ObjTurn(s32 actor, s32 angle, s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void Deri_ObjPlace(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02000d4c(actor, x, z);
}

static inline void Deri_Jump(s32 actor, s32 height, s32 frames)
{
    /* FAKEMATCH: the native loads the actor after the height and frames. */
    register s32 h __asm__("r1") = height;
    /* FAKEMATCH: the frames stay fixed in r2 for the same reason. */
    register s32 f __asm__("r2") = frames;
    s32 a = actor;

    Func_02000d94(a, h, f);
}

static inline void Deri_CopyCells(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02000cac(src_x, src_y, dest_x, dest_y, width, height);
}

#endif
