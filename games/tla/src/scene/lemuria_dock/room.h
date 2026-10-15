#ifndef TLA_LEMURIA_DOCK_ROOM_H
#define TLA_LEMURIA_DOCK_ROOM_H

#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "flag_call.h"

#define S16_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))

struct SceneWork
{
    u8  unk000[0x166];
    s16 unk166;
    u8  unk168[0x44];
    s32 unk1ac;
    u8  unk1b0[0x14];
    u16 unk1c4;
};

struct WarpTarget
{
    s32 scene;
    u16 x;
    u16 z;
};

#define SCENE_WORK ((struct SceneWork *)Ram_HeapSlots->eventWork)

extern u32                     gFrameCount;
extern u8                      MsgAreWeSetting[];
extern const u8                gData_0200057c[];
extern const u8                gData_0200059c[];
extern const struct WarpTarget gData_02000610;

struct ObjectRuntime *Object_GetById(s32 id);
void                  Battle_WaitMode0(s32 frames);
void                  Event_FinishScreenTransition(void);
void                  Event_WaitTransitionFrames(void);
void                  Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void                  ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void                  ObjectMotion_CenterOnColumn(s32 id, s32 column, s32 offset);
void                  ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void                  ObjectMotion_CommitPositionAndActivate(s32 id, s32 dx, s32 dz);
void                  ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void                  ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void                  ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void                  ObjectMotion_SetAngleToward(s32 id, s32 target, s32 frames);
void                  ObjectMotion_Launch(s32 id, s32 speed, s32 frames);
void                  UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32                   Inventory_PromptAndSetObjectMode(s32 actor, s32 flags);

void Func_02000254(const struct WarpTarget *warp, s32 door, s32 mode);
void Func_020002f8(void);
void Func_02000454(s32 scene, s32 x, s32 z);
void Func_0200045c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_0200046c(void);
void Func_02000474(void);
void Func_020004bc(s32 id, s32 x, s32 z);
void Func_020004dc(s32 message);
void Func_020004ec(s32 id, s32 a1);
void Func_020004f4(s32 id, s32 a1);
void Func_02000504(s32 id, s32 a1, s32 a2);
void Func_0200050c(s32 door);
void Func_02000524(s32 skip);
void Func_0200052c(s32 id, s32 a1, s32 a2, s32 a3);
s32  Func_0200053c(void);
void Func_02000544(s32 cue);

static inline void ObjectMotion_CenterOnColumn_(s32 id, s32 column, s32 offset)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_CenterOnColumn(id, column, offset);
}

static inline void ObjectMotion_OffsetPositionAndResetMotion_(s32 id, s32 dx, s32 dz)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_OffsetPositionAndResetMotion(id, dx, dz);
}

static inline void ObjectMotion_CommitPositionAndActivate_(s32 id, s32 dx, s32 dz)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_CommitPositionAndActivate(id, dx, dz);
}

static inline void ObjectMotion_SetSpeedParameters_(s32 id, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static inline void ObjectMotion_SetPositionAndReset_(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetPositionAndReset(id, x, z);
}

static inline void Func_0200052c_(s32 id, s32 a1, s32 a2, s32 a3)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200052c(id, a1, a2, a3);
}

static inline void Func_020004f4_(s32 id, s32 a1)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020004f4(id, a1);
}

static inline void Func_02000504_(s32 id, s32 a1, s32 a2)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02000504(id, a1, a2);
}

#endif
