#ifndef TLA_ANKOHL_RUINS_H
#define TLA_ANKOHL_RUINS_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "field_sprite.h"

/// The overlay's view of the event work heap slot.
struct AnkohlRuinsEventWork {
  u8 unk000[0x154];
  s16 unk154; ///< 66 when Func_020010f4's step is refused; 40 lets it run
  u8 unk156[0x14];
  s16 alarm;
  u8 unk16c[0x60];
  /// Moved with the leader while the raft (object 9) carries them.
  struct Actor *unk1cc;
};

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSpritePriority(u32 id, s32 variant);
s32 Random16Far(void);

extern s16 gData_02003780;
extern s16 gData_02003782;
extern s16 gData_02003784;
extern s32 gData_02003788;
extern s32 gData_0200378c;

s32 Task_Add(void (*func)(void), s32 priority);
void FieldEvent_Wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Actor_OffsetPositionAndResetMotion(s32 actor, s32 x, s32 z);
void Actor_WaitForMoveById(s32 actor);
void Func_02000b30(s32 id, s32 cue);
void Func_0200160c(void);
void Func_020019a0(void);
void Func_02001f7c(void);
s32 Func_02002dcc(void (*callback)(void));
void Func_02002df4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002dfc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002e24(s32 a, s32 b, s32 c);
void Func_02002e44(void);
void Func_02002e4c(void);
void Func_02002e94(s32 actor, s32 a, s32 b);
void Func_02002e9c(s32 actor, s32 a);
void Func_02002edc(s32 skip);
void Func_02002eec(s32 cue);

static inline s32 AnkohlRuins_FlagCheck(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static inline void AnkohlRuins_FlagSet(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static inline void AnkohlRuins_FlagClear(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_ClearBit(flag);
}

static inline void AnkohlRuins_Call2e24(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002e24(a, b, c);
}

static inline void AnkohlRuins_CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static inline void AnkohlRuins_Call2df4(s32 a, s32 b, s32 c, s32 d, s32 e,
                                        s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002df4(a, b, c, d, e, f);
}

static inline void AnkohlRuins_Call2dfc(s32 a, s32 b, s32 c, s32 d, s32 e,
                                        s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002dfc(a, b, c, d, e, f);
}

static inline void AnkohlRuins_Call2e94(s32 actor, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002e94(actor, a, b);
}

static inline void AnkohlRuins_ObjOffset(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_OffsetPositionAndResetMotion(actor, x, z);
}

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Camera_FollowActor(s32 actor, s32 target);
void Func_02002e34(struct Actor *object, s32 a);
void Func_02002e6c(s32 id, s32 a, s32 b);
void Func_02002eb4(void);

#endif
