#ifndef GABOMBA_CATACOMBS_LOCAL_H
#define GABOMBA_CATACOMBS_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "scene_tables.h"
#include "frame.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Func_0200114c(struct Actor *object, s32 mode);
void Func_02001154(struct Actor *object, const void *script);
struct Actor *Func_0200115c(s32 kind, s32 x, s32 y, s32 z);
void Func_02001164(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0200116c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
struct GabombaCatacombsVec;
s32 Func_02001174(struct Actor *object,
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
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001164(a, b, c, d, e, f);
}

static __inline__ void Func_0200116c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200116c(a, b, c, d, e, f);
}

static __inline__ void Func_020011cc_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020011cc(id, x, z);
}

static __inline__ void Func_02001274_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001274(a, b, c, d, e, f, g);
}

static __inline__ void Func_0200127c_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200127c(a, b, c);
}

static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 id, s32 x,
                                                                  s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_OffsetPositionAndResetMotion(id, x, z);
}

static __inline__ void Func_0200125c_(s32 a) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200125c(a);
}

static __inline__ void Func_02001204_(s32 id, s32 emote) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
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
