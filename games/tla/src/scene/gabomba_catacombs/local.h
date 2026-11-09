#ifndef GABOMBA_CATACOMBS_LOCAL_H
#define GABOMBA_CATACOMBS_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "frame.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_0200114c(struct Actor *object, s32 mode);
void Func_02001154(struct Actor *object, const void *script);
struct GabombaCatacombsVec;
s32 Func_02001174(struct Actor *object,
                  struct GabombaCatacombsVec *position);
void Func_0200119c(void);
void Func_020011a4(void);
void Func_02001224(s32 a, s32 b);
void Func_02001234(s32 a);
void Func_02001274(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_0200127c(s32 a, s32 b, s32 c);

extern const u8 gData_0200128c[];

extern char SceneId_Ov673Room1;
extern char SceneId_Ov673Room2;
extern char SceneId_Ov673Room3;
extern char SceneId_Ov673Room4;
extern char SceneId_Ov673Room5;

static __inline__ void Func_02001274_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001274(a, b, c, d, e, f, g);
}

static __inline__ void Func_0200127c_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200127c(a, b, c);
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
