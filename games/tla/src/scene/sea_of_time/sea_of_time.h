#ifndef TLA_SEA_OF_TIME_H
#define TLA_SEA_OF_TIME_H

#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "dma.h"
#include "event_inline.h"
#include "frame.h"
#include "actor.h"
#include "system/field_effect.h"

struct SeaOfTimeSprite {
  u8 unk00[0x1a];
  u8 visible;
};

struct SeaOfTimeMapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct SeaOfTimeMapWork {
  u8 unk000[0x108];
  struct SeaOfTimeMapLayer layers[3];
};

struct SeaOfTimeEventWork {
  u8 unk000[0x16a];
  s16 alarm;
  u8 unk16c[0x40];
  s32 screenEffect;
};

extern volatile u32 gSeaOfTime_WaveStartFrame;
extern u32 gSeaOfTime_WaveState;
extern u32 gSceneState[];
extern u8 gMapCellBuffer[];

void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetSpriteFlags(struct Actor *object, u32 flags);
s32 Trig_Cos(s32 angle);
void Event_End(void);
void FieldCamera_WaitForArrival(void);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
s32 SeaOfTime_GetCellCode(u32 layer, s32 x, s32 y);
s32 SeaOfTime_SetCellCode(u32 layer, s32 x, s32 y, s32 value);
s32 Position_FlatDistance(s32 *a, s32 *b);
void SeaOfTime_RemoveRock(s32 x, s32 z, s32 actor);
void SeaOfTime_PlaceRock(s32 x, s32 z, s32 actor);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline s32 SeaOfTime_Cos(s32 angle) { return Trig_Cos(angle); }

#endif
