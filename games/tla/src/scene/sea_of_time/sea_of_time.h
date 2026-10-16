#ifndef TLA_SEA_OF_TIME_H
#define TLA_SEA_OF_TIME_H

#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "dma.h"

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

struct SeaOfTimeObject *Object_GetById(s32 id);
struct SeaOfTimeObject *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
void Object_SetSpritePriority(struct SeaOfTimeObject *object, s32 priority);
void ObjectDispatch_SetSingleChildFlags(struct SeaOfTimeObject *object,
                                        s32 value);
void Object_SetPartAttribute(struct SeaOfTimeObject *object, s32 value);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
u32 Random16Far(void);
s32 Math_Sine(s32 angle);
s32 Math_Cosine(s32 angle);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void FieldEvent_WaitFrames(s32 frames);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001f8c(s32 a, s32 b, s32 c);
void Func_02001fac(void);
void Func_02001fb4(void);
void Func_02001fdc(s32 actor, s32 x, s32 z);
void Func_02001fec(s32 speed, s32 acceleration);
void Func_02001ffc(void);
void Func_02002004(s32 countdown);
void Func_02002024(s32 a, s32 b);
void Func_02002044(s32 skip);
void Func_02002054(s32 cue);

void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 d,
                   struct SeaOfTimeSpark *spark);
s32 Func_0200034c(u32 layer, s32 x, s32 y);
s32 Func_02000388(u32 layer, s32 x, s32 y, s32 value);
s32 Func_020003d0(s32 *a, s32 *b);
void Func_0200141c(s32 x, s32 z, s32 actor);
void Func_020018a8(s32 x, s32 z, s32 actor);

static inline s32 SeaOfTime_FlagCheck(s32 flag) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  return GameFlag_Test(flag);
}

static inline void SeaOfTime_FlagSet(s32 flag) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  GameFlag_SetBit(flag);
}

static inline void SeaOfTime_CamMove(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static inline void SeaOfTime_Tint(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_02001f8c(a, b, c);
}

static inline s32 SeaOfTime_Cos(s32 angle) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  return Math_Cosine(angle);
}

static inline s32 SeaOfTime_Sin(s32 angle) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  return Math_Sine(angle);
}

#endif
