#ifndef OVERLAY_690_LOCAL_H
#define OVERLAY_690_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_scene.h"
#include "iwram_call.h"
#include "scene_ids.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void PartyInventory_GiveItem(s32 item, s32 mode);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
s32 Math_Cosine(s32 angle);

void UiText_OpenMessageAtObject(s32 id, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 id, s32 flags);
void ObjectMotion_Launch(s32 id, s32 a, s32 b);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);

extern u8 MsgScene690No01;

void Func_02002018(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002030(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020020e0(s32 message);
void Func_020020f0(s32 id, s32 a);
void Func_02002110(s32 a);
s32 Func_020021a8(void);
void Func_020021b0(s32 a);
void Func_0200052c(void);
void Func_02000e30(void);
void Func_02000e90(void);
void Func_02001fa8(s32 a, s32 b, void (*callback)(void));
void Func_02002068(void);
void Func_02002070(void);
void Func_020020c0(s32 id, s32 a, s32 b);
void Func_02002130(s32 a, s32 b);
void Func_02002150(s32 a);
void Func_02002158(s32 a, s32 b, s32 c);
void Func_020021a0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

extern const u8 Data_02002380[];
extern const u8 Data_020023a0[];

static __inline__ void CommitPos(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_CommitPositionAndActivate(id, x, z);
}

static __inline__ void Window(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020021a0(a, b, c, d, e, f, g);
}

static __inline__ void CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002018(a, b, c, d, e, f);
}

static __inline__ void CopyCells2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002030(a, b, c, d, e, f);
}

static __inline__ void SetPosCommit(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetPositionAndCommit(id, x, z);
}

#endif
