#ifndef OVERLAY_679_LOCAL_H
#define OVERLAY_679_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_scene.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "field_sprite.h"

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ScenePlacement gScenePlacements[];
extern const struct SceneEvent gSceneEvents[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetModeById(u32 id, s32 mode);
struct ObjectRuntime *Engine_ObjectCreate(s32 kind, s32 x, s32 y, s32 z);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 attribute);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void Animation_ApplyChildArgument(void *animation, s32 value);
u8 *ResourceMetadata_Register(void *animation, s32 id);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_CenterOnColumn(s32 id, s32 column, s32 offset);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void ObjectMotion_SetAngleToward(s32 id, s32 target, s32 flags);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_Launch(s32 id, s32 a, s32 b);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void Motion_CamBounds(s32 x, s32 a, s32 z, s32 b);
void UiText_OpenMessageAtObject(s32 id, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 id, s32 flags);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
void Scheduler_RemoveCallbackFar(void *callback);
s32 Random16Far(void);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32 entry, s32 size, void *data);
void *Runtime_AllocateBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 Map_GetTerrainHeight(s32 x, s32 z);
void Func_02001bf4(void *callback);
void Func_02001bfc(void *callback);
void Func_02001c0c(s32 a, s32 b, void *callback);
void Func_02001c24(const void *source, void *destination);
void Func_02001c6c(struct ObjectRuntime *object, s32 mode);
void Func_02001c74(struct ObjectRuntime *object, const void *data);
void Func_02001c84(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02001c8c(struct ObjectRuntime *object);
s32 Func_02001c9c(struct ObjectRuntime *object, s32 *position);
void Func_02001cc4(void);
void Func_02001ccc(void);
void Func_02001d04(s32 id, s32 x, s32 z);
void Func_02001d2c(s32 id, s32 a, s32 b);
void Func_02001d44(s32 message);
void Func_02001d54(s32 id, s32 a);
void Func_02001d6c(s32 id, s32 emote, s32 frames);
void Func_02001d74(s32 id, s32 emote);
void Func_02001d7c(s32 a, s32 b);
void Func_02001d8c(void);
void Func_02001d94(s32 a);
void Func_02001d9c(void);
void Func_02001da4(s32 a, s32 b);
void Func_02001dac(s32 a);
void Func_02001dcc(s32 a);
void Func_02001dd4(s32 a, s32 b, s32 c);
void Func_02001de4(void);
void Func_02001dec(s32 a);
void Func_02001df4(void);
void Func_02001dfc(void);
void Func_02001e04(s32 a, s32 b, s32 c, s32 d);
void Func_02001e0c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
s32 Func_02001e14(void);
void Func_02001e1c(s32 a, s32 b);
void Func_02001e24(s32 a, s32 b);
void Func_02001e2c(s32 cue);

void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 flags, s32 *params);
void Func_020003e8(void);
void Func_02000540(void);
void Func_020006bc(void);
void Func_02000770(struct ObjectRuntime *object);
void Func_02000780(struct ObjectRuntime *object);
void Func_02000974(void);
void Func_02000ef8(void);
void Func_02000f58(void);
void Func_02000f98(void);
void Func_02001054(void);
void Func_020010b4(void);
void Func_020010d8(struct ObjectRuntime *object);
s32 Func_020013d8(struct ObjectRuntime *object);
void Func_02001500(void);
void Func_020015f8(void);
void Func_020017bc(void);
void Func_020017f8(s32 a, s32 b, s32 c);
void Func_02001940(void);
void Func_02001ad8(void);
void Func_02001ba0(s32 id);

extern s32 Data_02002a98;
extern u16 Data_02002a9c;
extern u16 Data_02002a9e;
extern u16 Data_02002aa0[];
extern const u8 Data_02001ee8[];
extern const u8 Data_02001ef4[];
extern const u8 Field_Tiles679[];
extern u32 gFrameCount;

extern u8 MsgWhatWhatJust;
extern u8 MsgWhyDoesMy;
extern u8 MsgICanTEver;
extern u8 MsgWowYouLook;
extern u8 MsgYouGuysAren;
extern u8 MsgItDBe;

static __inline__ s32 GameFlag_Test_(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return GameFlag_Test(flag);
}

static __inline__ void GameFlag_SetBit_(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    GameFlag_SetBit(flag);
}

static __inline__ void GameFlag_ClearBit_(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    GameFlag_ClearBit(flag);
}

static __inline__ void WaitFrames_(s32 frames)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    WaitFrames(frames);
}

static __inline__ void Battle_WaitMode0_(s32 frames)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Battle_WaitMode0(frames);
}

static __inline__ struct ObjectRuntime * Object_GetById_(s32 id)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Object_GetById(id);
}

static __inline__ void Object_SetModeById_(u32 id, s32 mode)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Object_SetModeById(id, mode);
}

static __inline__ struct ObjectRuntime * Engine_ObjectCreate_(s32 kind, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Engine_ObjectCreate(kind, x, y, z);
}

static __inline__ void Object_SetPartAttribute_(struct ObjectRuntime *object, s32 attribute)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Object_SetPartAttribute(object, attribute);
}

static __inline__ void ObjectDispatch_SetSingleChildFlags_(struct ObjectRuntime *object, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectDispatch_SetSingleChildFlags(object, flags);
}

static __inline__ void Animation_ApplyChildArgument_(void *animation, s32 value)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Animation_ApplyChildArgument(animation, value);
}

static __inline__ void ObjectMotion_SetSpeedParameters_(s32 id, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void ObjectMotion_CenterOnColumn_(s32 id, s32 column, s32 offset)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_CenterOnColumn(id, column, offset);
}

static __inline__ void ObjectMotion_OffsetPositionAndResetMotion_(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_OffsetPositionAndResetMotion(id, x, z);
}

static __inline__ void ObjectMotion_CommitCurrentPositionAndActivate_(s32 id)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_CommitCurrentPositionAndActivate(id);
}

static __inline__ void ObjectMotion_CommitPositionAndActivate_(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_CommitPositionAndActivate(id, x, z);
}

static __inline__ void ObjectMotion_SetActionVariant_(u32 id, s32 variant)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetActionVariant(id, variant);
}

static __inline__ void ObjectMotion_SetAngleToward_(s32 id, s32 target, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetAngleToward(id, target, flags);
}

static __inline__ void ObjectMotion_ArmCallback_(s32 id, s32 angle, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_ArmCallback(id, angle, flags);
}

static __inline__ void ObjectMotion_Launch_(s32 id, s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_Launch(id, a, b);
}

static __inline__ void Motion_SetModeAndWaitAnimation_(s32 id, s32 mode)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Motion_SetModeAndWaitAnimation(id, mode);
}

static __inline__ void Motion_SetVarCbAndRefresh_(s32 id, s32 variant)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Motion_SetVarCbAndRefresh(id, variant);
}

static __inline__ void Motion_CamBounds_(s32 x, s32 a, s32 z, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Motion_CamBounds(x, a, z, b);
}

static __inline__ void UiText_OpenMessageAtObject_(s32 id, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    UiText_OpenMessageAtObject(id, flags);
}

static __inline__ s32 Inventory_PromptAndSetObjectMode_(s32 id, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Inventory_PromptAndSetObjectMode(id, flags);
}

static __inline__ void Event_StartScreenTransition_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Event_StartScreenTransition();
}

static __inline__ void Event_FinishScreenTransition_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Event_FinishScreenTransition();
}

static __inline__ void Event_WaitTransitionFrames_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Event_WaitTransitionFrames();
}

static __inline__ s32 Random16Far_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Random16Far();
}

static __inline__ s32 Map_GetTerrainHeight_(s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Map_GetTerrainHeight(x, z);
}

static __inline__ void Func_02001c6c_(struct ObjectRuntime *object, s32 mode)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001c6c(object, mode);
}

static __inline__ void Func_02001c84_(struct ObjectRuntime *object, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001c84(object, x, y, z);
}

static __inline__ void Func_02001d04_(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001d04(id, x, z);
}

static __inline__ void Func_02001d44_(s32 message)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001d44(message);
}

static __inline__ void Func_02001d54_(s32 id, s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001d54(id, a);
}

static __inline__ void Func_02001d7c_(s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001d7c(a, b);
}

static __inline__ void Func_02001e0c_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001e0c(a, b, c, d, e, f, g);
}

static __inline__ s32 VramBlock_LoadCached_(s32 entry, s32 size, void *data)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return VramBlock_LoadCached(entry, size, data);
}

static __inline__ void Scheduler_AddOrUpdateCallback_(void *callback, s32 priority)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Scheduler_AddOrUpdateCallback(callback, priority);
}

static __inline__ void Func_02001e04_(s32 a, s32 b, s32 c, s32 d)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001e04(a, b, c, d);
}

static __inline__ void Func_02001c74_(struct ObjectRuntime *object, const void *data)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001c74(object, data);
}

#endif
