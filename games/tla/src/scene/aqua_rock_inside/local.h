#ifndef AQUA_ROCK_INSIDE_LOCAL_H
#define AQUA_ROCK_INSIDE_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "field_sprite.h"

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ActorPlacement gScenePlacements[];
extern const struct Trigger gSceneEvents[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_SetModeById(u32 id, s32 mode);
struct Actor *Import_ActorCreate(s32 kind, s32 x, s32 y, s32 z);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Animation_ApplyChildArgument(void *animation, s32 value);
u8 *ResourceMetadata_Register(void *animation, s32 id);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_CenterOnColumn(s32 id, s32 column, s32 offset);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Actor_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void Actor_SetSpritePriority(u32 id, s32 variant);
void Actor_SetAngleToward(s32 id, s32 target, s32 flags);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_Jump(s32 id, s32 a, s32 b);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void Camera_MoveWithinMapBounds(s32 x, s32 a, s32 z, s32 b);
void FieldMessage_ShowAtActor(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Task_Add(void *callback, s32 priority);
void Task_RemoveFar(void *callback);
s32 Random16Far(void);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
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
  u8 unk18[24];
};

struct PillarWork {
  u16 vramEntry;
  u16 count;
  struct PillarEntry entries[32];
};

struct BeamEntry {
  s32 x;
  s32 y;
  s32 z;
  s16 unk0c;
  s16 kind;
};

struct BeamWork {
  u16 vramEntry;
  u16 unk02;
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

extern u8 gData_020023c4[];
#define VENT_WORK ((struct VentWork *)(gData_020023c4 + 0x188))

struct VentRow {
  u8 unk00[8];
  s16 active;
  s16 timer;
  s32 x;
  s32 y;
  s32 z;
  s32 value;
};

#define VENT_ROW(index) ((struct VentRow *)((u8 *)VENT_WORK + (index) * 20))

struct SceneEventWork {
  u8 unk000[0x15a];
  s16 value15a;
  u8 unk15c[0xe];
  s16 value16a;
};

struct FieldSpriteFlags {
  u8 unk00[0x11];
  u8 flag0 : 1;
};

s32 Math_Sine(s32 angle);
s32 Math_Cosine(s32 angle);
void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void FieldMessage_Show(const u8 *message, s32 mode);
s32 PartyInventory_FindOwner(s32 item);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Func_02003c7c(const void *source, void *destination);
void Func_02003cc4(struct Actor *object, s32 mode);
void Func_02003ccc(struct Actor *object, const void *data);
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
void Func_02002bd4(struct Actor *object);
s32 Func_02002ed4(struct Actor *object);
void Func_02003ddc(s32 id, s32 x, s32 z);
void Func_02003e5c(s32 a);
void Func_02003ed4(void);

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
void Func_0200389c(struct Actor *object);
void Func_02003994(struct Actor *object, s32 priority);
void Func_020039f0(s32 *position, s32 base);
s32 Func_02003bb8(s32 index, s32 value);
s32 Func_02003bcc(void);
s32 Func_02003bd8(s32 limit);

extern s32 gData_02004d84;
extern struct PillarWork gData_0200536c;
extern struct BeamWork gData_02005970;
extern const u8 gData_02003fd8[];
extern const u8 gData_020040dc[];
extern const u8 gData_0200411c[];
extern const u8 gData_02004128[];
extern const u8 gData_0200414c[];
extern const u8 gData_02004351[];
extern const u8 gData_020044c8[];

extern const u8 MsgCheckedTheColumn[];
extern const u8 MsgCheckedThePsynergy[];
extern const u8 MsgCheckedTheTabletIt[];

static __inline__ s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static __inline__ void GameFlag_SetBit_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static __inline__ void GameFlag_ClearBit_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_ClearBit(flag);
}

static __inline__ void WaitFrames_(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  WaitFrames(frames);
}

static __inline__ void FieldEvent_WaitFrames_(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  FieldEvent_WaitFrames(frames);
}

static __inline__ struct Actor *Actor_GetById_(s32 id) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Actor_GetById(id);
}

static __inline__ void Actor_SetModeById_(u32 id, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetModeById(id, mode);
}

static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x,
                                                             s32 y, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Import_ActorCreate(kind, x, y, z);
}

static __inline__ void Actor_SetPartAttribute_(struct Actor *object,
                                                s32 attribute) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPartAttribute(object, attribute);
}

static __inline__ void
Actor_SetSingleChildFlags_(struct Actor *object, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSingleChildFlags(object, flags);
}

static __inline__ void Animation_ApplyChildArgument_(void *animation,
                                                     s32 value) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Animation_ApplyChildArgument(animation, value);
}

static __inline__ void Actor_SetSpeedParameters_(s32 id, s32 speed,
                                                        s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_CenterOnColumn_(s32 id, s32 column,
                                                    s32 offset) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_CenterOnColumn(id, column, offset);
}

static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 id, s32 x,
                                                                  s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_OffsetPositionAndResetMotion(id, x, z);
}

static __inline__ void Actor_WaitForMoveById_(s32 id) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_WaitForMoveById(id);
}

static __inline__ void Actor_CommitPositionAndActivate_(s32 id, s32 x,
                                                               s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_CommitPositionAndActivate(id, x, z);
}

static __inline__ void Actor_SetSpritePriority_(u32 id, s32 variant) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSpritePriority(id, variant);
}

static __inline__ void Actor_SetAngleToward_(s32 id, s32 target,
                                                    s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetAngleToward(id, target, flags);
}

static __inline__ void Actor_ArmCallback_(s32 id, s32 angle, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ArmCallback(id, angle, flags);
}

static __inline__ void Actor_Jump_(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_Jump(id, a, b);
}

static __inline__ void Motion_SetModeAndWaitAnimation_(s32 id, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Motion_SetModeAndWaitAnimation(id, mode);
}

static __inline__ void Motion_SetVarCbAndRefresh_(s32 id, s32 variant) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Motion_SetVarCbAndRefresh(id, variant);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 a, s32 z, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(x, a, z, b);
}

static __inline__ void FieldMessage_ShowAtActor_(s32 id, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  FieldMessage_ShowAtActor(id, flags);
}

static __inline__ s32 Event_AskYesNoWithActorAnimation_(s32 id, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Event_AskYesNoWithActorAnimation(id, flags);
}

static __inline__ void Event_StartScreenTransition_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Event_StartScreenTransition();
}

static __inline__ void Event_FinishScreenTransition_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Event_FinishScreenTransition();
}

static __inline__ void Event_WaitTransitionFrames_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Event_WaitTransitionFrames();
}

static __inline__ s32 Random16Far_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Random16Far();
}

static __inline__ s32 Map_GetTerrainHeight_(s32 layer, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Map_GetTerrainHeight(layer, x, z);
}

static __inline__ void Func_02003d14_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003d14(a, b, c, d, e, f);
}

static __inline__ void Func_02003d1c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003d1c(a, b, c, d, e, f);
}

static __inline__ void Func_02003d24_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003d24(a, b, c, d, e, f);
}

static __inline__ void *MemAlloc_(s32 size) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Heap_Alloc(size);
}

static __inline__ s32 VramBlock_LoadCached_(s32 entry, s32 size, void *data) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return VramCache_Load(entry, size, data);
}

static __inline__ void Func_02003e44_(s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003e44(a, b);
}

static __inline__ void Task_Add_(void *callback,
                                                      s32 priority) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Task_Add(callback, priority);
}

#endif
