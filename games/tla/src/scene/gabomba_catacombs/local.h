#ifndef GABOMBA_CATACOMBS_LOCAL_H
#define GABOMBA_CATACOMBS_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_scene.h"
#include "frame.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void Func_0200114c(struct ObjectRuntime *object, s32 mode);
void Func_02001154(struct ObjectRuntime *object, const void *script);
struct ObjectRuntime *Func_0200115c(s32 kind, s32 x, s32 y, s32 z);
void Func_02001164(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0200116c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
struct GabombaCatacombsVec;
s32 Func_02001174(struct ObjectRuntime *object,
                  struct GabombaCatacombsVec *position);
void Func_0200119c(void);
void Func_020011a4(void);
void Func_020011cc(s32 id, s32 x, s32 z);
void Func_02001204(s32 id, s32 emote);
void Func_0200121c(s32 frames);
void Func_02001224(s32 a, s32 b);
void Func_0200122c(s32 a, s32 b);
void Func_02001234(s32 a);
void Func_0200125c(s32 a);
void Func_02001274(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_0200127c(s32 a, s32 b, s32 c);
void Func_02001284(s32 cue);

extern const u8 gData_0200128c[];

extern char SceneId_Ov673Room1;
extern char SceneId_Ov673Room2;
extern char SceneId_Ov673Room3;
extern char SceneId_Ov673Room4;
extern char SceneId_Ov673Room5;

static __inline__ void Func_02001164_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02001164(a, b, c, d, e, f);
}

static __inline__ void Func_0200116c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_0200116c(a, b, c, d, e, f);
}

static __inline__ void Func_020011cc_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_020011cc(id, x, z);
}

static __inline__ void Func_02001274_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02001274(a, b, c, d, e, f, g);
}

static __inline__ void Func_0200127c_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_0200127c(a, b, c);
}

static __inline__ void ObjectMotion_OffsetPositionAndResetMotion_(s32 id, s32 x,
                                                                  s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_OffsetPositionAndResetMotion(id, x, z);
}

static __inline__ void Func_0200125c_(s32 a) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_0200125c(a);
}

static __inline__ void Func_02001204_(s32 id, s32 emote) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02001204(id, emote);
}

struct GabombaCatacombsVec {
  s32 x;
  s32 y;
  s32 z;
};

struct GabombaCatacombsFollower {
  u8 unk00[0x10];
  s32 z;
};

struct GabombaCatacombsEventWork {
  u8 unk000[0x1cc];
  struct GabombaCatacombsFollower *follower;
};

struct GabombaCatacombsParty {
  u8 unk000[0x254];
  u16 value254;
};

#endif
