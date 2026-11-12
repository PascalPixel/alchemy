#ifndef AQUA_ROCK_LOCAL_H
#define AQUA_ROCK_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "field_sprite.h"
#include "frame.h"

extern const u32 gSceneExits[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
void Event_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_SetModeById(u32 id, s32 mode);
struct Actor *Import_ActorCreate(s32 kind, s32 x, s32 y, s32 z);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);
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
void Task_Add(void *func, s32 priority);
void Task_RemoveFar(void *func);
s32 Random16Far(void);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

#include "scene_ids.h"

struct AquaRockWork {
  struct Actor *object;
  u16 vramEntry;
  u16 unk06;
  s16 direction;
};

s32 Math_Sine(s32 angle);
void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
void Func_020028b8(const void *source, void *destination);
void Func_02002910(struct Actor *object, s32 mode);
void Func_02002918(struct Actor *object, const void *data);
void Func_02002928(struct Actor *object);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Map_CopyCellsRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void BattleEffect_ApplyColorToTargetBuffer(s32 a, s32 b);
s32 Party_GetLeaderActor(void);
void Func_02002998(void);
void Func_020029a0(void);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void Import_EventSetExitRequest(s32 a);
void Event_ResolvePendingActions(s32 a);
void Func_02002a60(void);
void Sound_Play(s32 cue);
void Actor_ResetAndSetPosition(s32 id, s32 x, s32 z);
s32 Func_02002030(struct Actor *object);

void Func_02000468(struct Actor *object);
void Func_020004e4(struct Actor *object);
void Func_02002250(void);
void Func_020023c0(void);
void Func_02002534(void);

extern struct AquaRockWork gData_020034f0;
extern const u8 gData_02002c44[];
extern const u8 gData_02002c50[];
extern const u8 gData_02002c74[];
extern const u8 gData_02002e30[];
extern const u8 gData_02002e60[];
extern const u8 gData_02002eec[];
extern const u8 gData_02002f04[];
extern const u8 gData_0200312c[];
extern const u8 gData_02003214[];
extern const u8 gData_02003220[];
extern const u8 gData_020032d4[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 GameFlag_Test_(s32 flag) {
  return GameFlag_Test(flag);
}

static __inline__ void GameFlag_ClearBit_(s32 flag) {
  GameFlag_ClearBit(flag);
}

static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x,
                                                             s32 y, s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 a, s32 z, s32 b) {
  Camera_MoveWithinMapBounds(x, a, z, b);
}

static __inline__ void Func_02002960_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

static __inline__ void Func_02002958_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Map_CopyCellsRect(a, b, c, d, e, f);
}

static __inline__ void Func_02002a20_(s32 a, s32 b) {
  BattleEffect_ApplyColorToTargetBuffer(a, b);
}

static __inline__ void Actor_ResetAndSetPosition_(s32 id, s32 x, s32 z) {
  Actor_ResetAndSetPosition(id, x, z);
}

static __inline__ void *MemAlloc_(s32 size) {
  return Heap_Alloc(size);
}

#endif
