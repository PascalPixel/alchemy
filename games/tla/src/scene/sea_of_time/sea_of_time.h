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

extern volatile u32 gData_020022c8;
extern u32 gData_020022cc;
extern u32 gSceneState[];
extern u8 gMapCellBuffer[];

void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);
s32 Math_Cosine(s32 angle);
void Func_02001fb4(void);
void Func_02001ffc(void);

void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
s32 Func_0200034c(u32 layer, s32 x, s32 y);
s32 Func_02000388(u32 layer, s32 x, s32 y, s32 value);
s32 Func_020003d0(s32 *a, s32 *b);
void Func_0200141c(s32 x, s32 z, s32 actor);
void Func_020018a8(s32 x, s32 z, s32 actor);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline s32 SeaOfTime_Cos(s32 angle) { return Math_Cosine(angle); }

#endif
