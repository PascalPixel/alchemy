#ifndef AQUA_ROCK_LOCAL_H
#define AQUA_ROCK_LOCAL_H

#include "types.h"
#include "system.h"
#include "task.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "field_sprite.h"
#include "frame.h"

s32 GameFlag_Get(s32 flag);
void GameFlag_Clear(s32 flag);
void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void Animation_ApplyChildArgument(void *animation, s32 value);
u8 *ResourceMetadata_Register(void *animation, s32 id);
void FieldCamera_MoveWithinMapBounds(s32 x, s32 a, s32 z, s32 b);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 VramCache_FindFreeEntry(void);

#include "scene_ids.h"

struct Bubble {
  s32 x;
  s32 y;
  s32 z;
  // volatile: the listing reads age again after the OAM stores.
  volatile s32 age;
  u32 oam[3];
};

struct AquaRockWork {
  struct Actor *object;
  u16 vramEntry;
  u16 next;
  s16 direction;
  u8 unk0a[2];
  struct Bubble bubbles[64];
};

void Import_DecompressTaggedLz(const void *source, void *destination);
void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_StartScript(struct Actor *object, const void *data);
void Import_ActorRelease(struct Actor *object);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Map_CopyCellsRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void BattleEffect_ApplyColorToTargetBuffer(s32 a, s32 b);
void Event_End(void);
void Event_NoOp(void);
s32 Actor_EmitMist(struct Actor *object);

void Func_02000468(struct Actor *object);
void Func_020004e4(struct Actor *object);
void Func_02002250(void);
void Func_020023c0(void);
void Func_02002534(void);

extern struct AquaRockWork gData_020034f0;
extern u32 gData_02002c44[];
extern u32 gWaterDropScript[];
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
static __inline__ s32 GameFlag_Test_(s32 flag) { return GameFlag_Get(flag); }

static __inline__ void GameFlag_ClearBit_(s32 flag) { GameFlag_Clear(flag); }

static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x, s32 y,
                                                    s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 a, s32 z, s32 b) {
  FieldCamera_MoveWithinMapBounds(x, a, z, b);
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
  Actor_SetDestinationById(id, x, z);
}

static __inline__ void *MemAlloc_(s32 size) { return Heap_Alloc(size); }

#endif
