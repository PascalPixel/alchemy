#ifndef TLA_SEA_OF_TIME_H
#define TLA_SEA_OF_TIME_H

#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "dma.h"
#include "event_inline.h"

struct SeaOfTimeSprite {
  u8 unk00[0x1a];
  u8 visible;
};

struct SeaOfTimeObject {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 height;
  s32 field18;
  s32 scale;
  u8 unk20[2];
  u8 layer;
  u8 priorityFlags;
  u8 unk24[0x2c];
  struct SeaOfTimeSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
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

struct SeaOfTimeSpark {
  s32 mode;
  s32 kind;
  s32 scaleX;
  s32 scaleY;
  u8 unk10[8];
  s16 life;
  u8 unk1a[0xe];
};

extern u32 gFrameCount;
extern volatile u32 gData_020022c8;
extern u32 gData_020022cc;
extern u32 gSceneState[];
extern u8 gMapCellBuffer[];

void Scene_SetActorSpritePriority(struct SeaOfTimeObject *object, s32 priority);
void Actor_SetSingleChildFlags(struct SeaOfTimeObject *object,
                                        s32 value);
u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001fac(void);
void Func_02001fb4(void);
void Func_02001ffc(void);

void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 d,
                   struct SeaOfTimeSpark *spark);
s32 Func_0200034c(u32 layer, s32 x, s32 y);
s32 Func_02000388(u32 layer, s32 x, s32 y, s32 value);
s32 Func_020003d0(s32 *a, s32 *b);
void Func_0200141c(s32 x, s32 z, s32 actor);
void Func_020018a8(s32 x, s32 z, s32 actor);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline s32 SeaOfTime_Cos(s32 angle) {
  return Math_Cosine(angle);
}

#endif
