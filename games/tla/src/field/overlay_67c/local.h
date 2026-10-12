#ifndef OVERLAY_67C_LOCAL_H
#define OVERLAY_67C_LOCAL_H

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
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);


extern u32 gFrameCount;

#include "map.h"

#define DIFF(a, b) ((a) - (b) >= 0 ? (a) - (b) : (b) - (a))

struct PillarEntry {
    s32 x;
    s32 height;
    s32 z;
    s32 base;
    s16 kind;
    s16 state;
    s32 *position;
    u8 unknown_18[24];
};

struct PillarWork {
    u16 vram_entry;
    u16 count;
    struct PillarEntry entries[32];
};

struct BeamEntry {
    s32 x;
    s32 y;
    s32 z;
    s16 unknown_0c;
    s16 kind;
};

struct BeamWork {
    u16 vram_entry;
    u16 unknown_02;
    struct BeamEntry entries[2];
};

struct VentEntry {
    s16 active;
    s16 timer;
    s32 x;
    s32 y;
    s32 z;
    s32 value;
};

struct VentWork {
    s32 counter;
    s32 (*hook)(void);
    struct VentEntry entries[16];
    u16 ids[4];
};

extern u8 Data_020023c4[];
#define VENT_WORK ((struct VentWork *)(Data_020023c4 + 0x188))

struct VentRow {
    u8 unknown_00[8];
    s16 active;
    s16 timer;
    s32 x;
    s32 y;
    s32 z;
    s32 value;
};

#define VENT_ROW(index) ((struct VentRow *)((u8 *)VENT_WORK + (index) * 20))

struct SceneEventWork {
    u8 unknown_000[0x15a];
    s16 value_15a;
    u8 unknown_15c[0xe];
    s16 value_16a;
};

struct FieldSpriteFlags {
    u8 unknown_00[0x11];
    u8 flag_0 : 1;
};

s32 Math_Sine(s32 angle);
s32 Math_Cosine(s32 angle);
void *MemAlloc(s32 size);
void MemFree(void *block);
void Animation_SetStateFlags(struct ObjectRuntime *object, s32 flags);
void UiText_ShowPositionedMessageAndWait(const u8 *message, s32 mode);
s32 PartyInventory_FindOwner(s32 item);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void Func_02003c7c(const void *source, void *destination);
void Func_02003cc4(struct ObjectRuntime *object, s32 mode);
void Func_02003ccc(struct ObjectRuntime *object, const void *data);
void Func_02003cdc(void);
void Func_02003d14(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003d1c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003d24(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003d54(void);
void Func_02003d64(s32 item, s32 mode);
void Func_02003d7c(void);
void Func_02003d84(void);
void Func_02003df4(s32 id, s32 a, s32 b);
void Func_02003e44(s32 a, s32 b);
void Func_02003e54(void);
void Func_02003e64(s32 a, s32 b);
void Func_02003e6c(void);
void Func_02003eac(s32 a);
void Func_02003eb4(s32 a, s32 b);
void Func_02003ecc(s32 a);
void Func_02003eec(s32 a, s32 b);
void Func_02003ef4(s32 cue);

void Func_020006d0(void);
s32 Func_02000b24(s32 id, s32 value);
void Func_02000b5c(void);
void Func_02001a1c(void);
void Func_02001e78(s32 id);
void Func_020030f4(void);
void Func_02003460(s32 value);
void Func_020034fc(void);
void Func_02003628(void);
s32 Func_02003688(s32 x, s32 y, s32 z, s32 kind);
s32 Func_020036b8(s32 a, s32 b);
void Func_0200389c(void);
void Func_02003994(struct ObjectRuntime *object, s32 priority);
void Func_020039f0(s32 *position, s32 base);
s32 Func_02003bb8(s32 index, s32 value);
s32 Func_02003bcc(void);
s32 Func_02003bd8(s32 limit);

extern s32 Data_02004d84;
extern struct PillarWork Data_0200536c;
extern struct BeamWork Data_02005970;
extern const u8 Data_02003fd8[];
extern const u8 Data_020040dc[];
extern const u8 Data_0200411c[];
extern const u8 Data_02004128[];
extern const u8 Data_0200414c[];
extern const u8 Data_02004350[];
extern const u8 Data_020044c8[];

extern const u8 MsgCheckedTheColumn[];
extern const u8 MsgCheckedThePsynergy[];
extern const u8 MsgCheckedTheTabletIt[];


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

static __inline__ s32 Map_GetTerrainHeight_(s32 layer, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Map_GetTerrainHeight(layer, x, z);
}

static __inline__ void Func_02003d14_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02003d14(a, b, c, d, e, f);
}

static __inline__ void Func_02003d1c_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02003d1c(a, b, c, d, e, f);
}

static __inline__ void Func_02003d24_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02003d24(a, b, c, d, e, f);
}

static __inline__ void *MemAlloc_(s32 size)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return MemAlloc(size);
}

static __inline__ s32 VramBlock_LoadCached_(s32 entry, s32 size, void *data)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return VramBlock_LoadCached(entry, size, data);
}

static __inline__ void Func_02003e44_(s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02003e44(a, b);
}

static __inline__ void Scheduler_AddOrUpdateCallback_(void *callback, s32 priority)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Scheduler_AddOrUpdateCallback(callback, priority);
}

#endif
