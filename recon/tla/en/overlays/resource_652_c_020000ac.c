/* Pin-free Deri_Jump caller trial: 62 native, 62 built, 61 common, differ 2.
 * The actor copy to r0 occurs before the height shift and frame load; native
 * performs the copy after both. Removing either height or frame binding
 * separately leaves the same swap. One-pass initialization, call and whole
 * helper blocks, XOR cancellation, add/sub cancellation and a redundant
 * copy guard also leave two differences. Linked helper remains unchanged. */
/* Reordering the inline helper's height, frame and actor parameters (and
 * its caller arguments together) retains differ 2 in three orders. Branching
 * on actor == height differs 10; actor == frames differs 8, with eight extra
 * instructions. Neither conditional call form is adopted. */
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
    s32 h = height;
    s32 f = frames;
    s32 a = actor;

    Func_02000d94(a, h, f);
}

static inline void Deri_CopyCells(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02000cac(src_x, src_y, dest_x, dest_y, width, height);
}

#endif
#include "types.h"
#include "field_event.h"
#include "party_state.h"
#include "scene_call.h"
#include "text/msg_ids.h"
#include "field_scene.h"
#include "scene_ids.h"

void Func_020000ac(s32 unused, s32 actor)
{
    Deri_Jump(actor, 0x100, 50);
    Deri_ObjLaunch(actor, 6, 15);
    Deri_ObjLaunch(actor, 6, 15);
    Deri_Wait(10);
    if (!Deri_FlagChk(0x200)) {
        Deri_MesSet((s32)MsgDeriHeyaGah);
        Deri_MesPut(actor, 0);
        Deri_ObjWalk(actor, 360, 72);
        Deri_ObjTurn(actor, 0x4000, 0);
        Deri_FlagSet(0x200);
    } else {
        Deri_MesSet((s32)MsgDeriHeyaYarg);
        Deri_MesPut(actor, 0);
        Deri_ObjWalk(actor, 392, 104);
        Deri_ObjTurn(actor, 0x8000, 0);
        Deri_FlagClr(0x200);
    }
    Deri_Wait(20);
}
