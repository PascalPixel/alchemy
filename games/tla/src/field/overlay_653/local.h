#ifndef OVERLAY_653_LOCAL_H
#define OVERLAY_653_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_scene.h"
#include "iwram_call.h"
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
u32 Random16Far(void);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetModeById(u32 id, s32 mode);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Func_020051c0(void);
void Func_020051c8(void);
void Func_02005230(s32 id, s32 a, s32 b);
void Func_02005280(s32 message);
void Func_02005298(s32 id, s32 a);
void Func_020052b0(s32 id, s32 a);
void Func_020052c8(s32 a, s32 b, s32 c);
void Func_02005308(s32 a, s32 b);
void Func_02005370(s32 a);
void Func_02005378(s32 id, s32 x, s32 z, s32 angle);
void Func_02000668(struct ObjectRuntime *object, s32 a);

extern u8 MsgScene653No11;

struct ObjectRuntime *Func_02005140(s32 type, s32 x, s32 y, s32 z);
void Func_02005138(struct ObjectRuntime *object, const void *data);
void Func_02005130(struct ObjectRuntime *object, s32 a);
s32 Func_02005180(struct ObjectRuntime *object, s32 *position);
void Animation_ApplyChildArgument(void *sprite, s32 a);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Func_020006b0(struct ObjectRuntime *object);
void Func_02000778(struct ObjectRuntime *object, s32 dz);
void Func_020052c0(s32 id);
void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02005348(s32 a, s32 b);
void Func_02005350(s32 a, s32 b);
void Func_02005358(void);
void Func_02005360(void);
void Func_02005368(void);
void Field_DispatchTypeHandler(s32 type);
void Func_0200098c(s32 *position, s32 value);
void Func_02000adc(void);
extern s32 Data_02006298;
extern s32 Data_0200629c;
extern s32 Data_02006294;
extern u32 gFrameCount;
extern const u8 Data_02005494[];
extern const u8 Data_020054e4[];
extern const u8 Data_020054f0[];

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

static __inline__ u32 Random16Far_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Random16Far();
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

static __inline__ void ObjectMotion_SetActionVariant_(u32 id, s32 variant)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetActionVariant(id, variant);
}

static __inline__ void ObjectMotion_SetPositionAndReset_(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetPositionAndReset(id, x, z);
}

static __inline__ void ObjectMotion_ResetAndSetPosition_(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_ResetAndSetPosition(id, x, z);
}

static __inline__ void ObjectMotion_ArmCallback_(s32 id, s32 angle, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_ArmCallback(id, angle, flags);
}

static __inline__ void ObjectMotion_CommitCurrentPositionAndActivate_(s32 id)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_CommitCurrentPositionAndActivate(id);
}

static __inline__ void ObjectMotion_SetSpeedParameters_(s32 id, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void ObjectDispatch_SetSingleChildFlags_(struct ObjectRuntime *object, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectDispatch_SetSingleChildFlags(object, flags);
}

static __inline__ void Motion_SetModeAndWaitAnimation_(s32 id, s32 mode)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Motion_SetModeAndWaitAnimation(id, mode);
}

static __inline__ void Func_020051c0_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020051c0();
}

static __inline__ void Func_020051c8_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020051c8();
}

static __inline__ void Func_02005230_(s32 id, s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005230(id, a, b);
}

static __inline__ void Func_02005280_(s32 message)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005280(message);
}

static __inline__ void Func_02005298_(s32 id, s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005298(id, a);
}

static __inline__ void Func_020052b0_(s32 id, s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020052b0(id, a);
}

static __inline__ void Func_020052c8_(s32 a, s32 b, s32 c)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020052c8(a, b, c);
}

static __inline__ void Func_02005308_(s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005308(a, b);
}

static __inline__ void Func_02005370_(s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005370(a);
}

static __inline__ void Func_02005378_(s32 id, s32 x, s32 z, s32 angle)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005378(id, x, z, angle);
}

static __inline__ void Func_02000668_(struct ObjectRuntime *object, s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02000668(object, a);
}

static __inline__ struct ObjectRuntime * Func_02005140_(s32 type, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Func_02005140(type, x, y, z);
}

static __inline__ void Func_02005138_(struct ObjectRuntime *object, const void *data)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005138(object, data);
}

static __inline__ void Func_02005130_(struct ObjectRuntime *object, s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005130(object, a);
}

static __inline__ s32 Func_02005180_(struct ObjectRuntime *object, s32 *position)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Func_02005180(object, position);
}

static __inline__ void Animation_ApplyChildArgument_(void *sprite, s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Animation_ApplyChildArgument(sprite, a);
}

static __inline__ void Object_SetPartAttribute_(struct ObjectRuntime *object, s32 value)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Object_SetPartAttribute(object, value);
}

static __inline__ void Func_020006b0_(struct ObjectRuntime *object)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020006b0(object);
}

static __inline__ void Func_02000778_(struct ObjectRuntime *object, s32 dz)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02000778(object, dz);
}

static __inline__ void Func_020052c0_(s32 id)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020052c0(id);
}

static __inline__ void Object_SetPositionAndResetMotion_(struct ObjectRuntime *object, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Object_SetPositionAndResetMotion(object, x, y, z);
}

static __inline__ void Func_02005348_(s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005348(a, b);
}

static __inline__ void Func_02005350_(s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005350(a, b);
}

static __inline__ void Func_02005358_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005358();
}

static __inline__ void Func_02005360_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005360();
}

static __inline__ void Func_02005368_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005368();
}

static __inline__ void Field_DispatchTypeHandler_(s32 type)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Field_DispatchTypeHandler(type);
}

static __inline__ void Func_0200098c_(s32 *position, s32 value)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200098c(position, value);
}

static __inline__ void Func_02000adc_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02000adc();
}

#endif
