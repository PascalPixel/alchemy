#ifndef TAOPO_SWAMP_LOCAL_H
#define TAOPO_SWAMP_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "field_sprite.h"

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ActorPlacement gActorPlacements[];

extern char SceneId_Ov678Room1;
extern char SceneId_Ov678Room2;
extern char SceneId_Ov678Room3;
extern char SceneId_Ov678Room4;
extern char SceneId_Ov678Room5;
extern char SceneId_Ov678Room6;
extern char SceneId_Ov678Room7;
extern char SceneId_Ov678Room8;
extern char SceneId_Ov678Room9;
extern char SceneId_Ov678Room10;
extern char SceneId_Ov678Room11;

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_SetModeById(u32 id, s32 mode);
void Actor_SetSpritePriority(u32 id, s32 variant);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_SetAngleToward(s32 id, s32 target, s32 flags);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Actor_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Func_02002764(s32 id, s32 a);
void Func_02002778(s32 a, s32 b);
void Func_02002840(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002898(void);
void Func_020028a0(void);
void Func_020028d0(s32 id, s32 x, s32 z);
void Func_020028e0(s32 id, s32 a, s32 b);
void Func_02002908(s32 id, s32 a, s32 b);
void Func_02002910(s32 id, s32 a);
void Func_02002928(void);
void Func_02002930(s32 a);
void Func_02002938(s32 a, s32 b);
void Func_02002940(s32 a, s32 b);
void Func_02002958(s32 a, s32 b, s32 c);
void Func_02002968(s32 a);
void Func_02002990(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_020029a8(s32 a, s32 b, s32 c);
void Func_020029b0(s32 a);
s32 Random16Far(void);
struct Actor *Func_02002808(s32 type, s32 x, s32 y, s32 z);
void Func_02002800(struct Actor *object, const void *data);
void Func_02002838(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Task_RemoveFar(void (*func)(void));

extern s32 gData_02003458;
extern s32 gData_0200345c;
extern s32 gData_02003460;
extern const u8 gData_020029c8[];
void Func_02002948(void);
void Func_02002084(s32 a);
void Func_020029a0(s32 message);
void Func_020027f8(struct Actor *object, s32 a);
void Func_02002880(struct Actor *object, s32 a);
s32 Func_02002730(void);
void Func_0200271c(s32 a, s32 b);
s32 Func_0200273c(s32 a);
void Func_02002978(s32 a);

struct SpriteWork {
  u8 unk00[17];
  u8 layer : 2;
  u8 level : 6;
  u8 unk12[7];
  u8 dirty;
  u8 mode;
  u8 unk1b[17];
  u8 *child;
};

struct SceneState {
  s16 mode;
  s16 timer;
  s32 x;
  s32 z;
};

extern struct SceneState gSceneState;

typedef volatile u16 vu16;

struct IoReg16 {
  u16 value;
};

#define REG_BLDALPHA ((struct IoReg16 *)0x04000052)
struct VentEntry {
  s16 active;
  s16 timer;
  s32 x;
  s32 y;
  s32 z;
  s32 value;
};

struct VentWork {
  s32 counter;
  s32 (*hook)(void);
  struct VentEntry entries[16];
  u16 ids[4];
};

extern u8 gData_020023c4[];
#define VENT_WORK ((struct VentWork *)(gData_020023c4 + 0x188))
#define DIFF(a, b) ((a) - (b) >= 0 ? (a) - (b) : (b) - (a))

s32 Math_Cosine(s32 angle);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Func_02002400(struct Actor *object);
extern const u8 gData_02002af4[];
extern s32 gData_020035f0;
extern u8 gMapShapeGrid[];
void Func_02002848(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002810(void);
void Func_02002818(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002820(struct Actor *object);
void Func_02002828(void);
s32 Func_02002998(void);
void Task_Add(void (*func)(void), s32 priority);
void Func_02001eb8(void);
void Func_02001ff0(s32 x, s32 z);
void Func_0200237c(void);
void Func_02000920(s32 a, s32 id);
extern const u8 gData_02002a60[];
extern const u8 gData_02002a6c[];
void Func_02000810(s32 id);
void Func_020008d8(s32 id);
extern u32 gFrameCount;
extern const u8 gData_02002a48[];
extern const u8 gData_02002a54[];
extern u8 MsgIThoughtI;
void Func_02000c24(void);
void Func_02000ca0(void);
void Func_02000cec(void);

static __inline__ s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static __inline__ void GameFlag_SetBit_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static __inline__ void GameFlag_ClearBit_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_ClearBit(flag);
}

static __inline__ s32 GameFlag_GetByte_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_GetByte(flag);
}

static __inline__ void GameFlag_SetByte_(s32 flag, s32 value) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetByte(flag, value);
}

static __inline__ void wait_(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  wait(frames);
}

static __inline__ void FieldEvent_Wait_(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  FieldEvent_Wait(frames);
}

static __inline__ struct Actor *Actor_GetById_(s32 id) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Actor_GetById(id);
}

static __inline__ void Actor_SetModeById_(u32 id, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetModeById(id, mode);
}

static __inline__ void Actor_SetSpritePriority_(u32 id, s32 variant) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSpritePriority(id, variant);
}

static __inline__ void Actor_ArmCallback_(s32 id, s32 angle, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ArmCallback(id, angle, flags);
}

static __inline__ void Actor_SetAngleToward_(s32 id, s32 target,
                                                    s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetAngleToward(id, target, flags);
}

static __inline__ void
Actor_OffsetPositionAndResetMotion_(s32 id, s32 dx, s32 dz) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void Actor_WaitForMoveById_(s32 id) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_WaitForMoveById(id);
}

static __inline__ void Actor_CommitPositionAndActivate_(s32 id, s32 x,
                                                               s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_CommitPositionAndActivate(id, x, z);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 a, s32 b, s32 c, s32 d) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(a, b, c, d);
}

static __inline__ void Func_02002764_(s32 id, s32 a) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002764(id, a);
}

static __inline__ void Func_02002840_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002840(a, b, c, d, e, f);
}

static __inline__ void Func_020028d0_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020028d0(id, x, z);
}

static __inline__ void Func_020028e0_(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020028e0(id, a, b);
}

static __inline__ void Func_02002910_(s32 id, s32 a) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002910(id, a);
}

static __inline__ void Func_02002940_(s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002940(a, b);
}

static __inline__ void Func_02002958_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002958(a, b, c);
}

static __inline__ void Func_02002968_(s32 a) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002968(a);
}

static __inline__ void Func_02002990_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002990(a, b, c, d, e, f, g);
}

static __inline__ void Func_020029b0_(s32 a) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020029b0(a);
}

static __inline__ s32 Random16Far_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Random16Far();
}

static __inline__ struct Actor *Func_02002808_(s32 type, s32 x, s32 y,
                                                       s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Func_02002808(type, x, y, z);
}

static __inline__ void Func_02002838_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002838(a, b, c, d, e, f);
}

static __inline__ void Actor_SetSpeedParameters_(s32 id, s32 speed,
                                                        s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void
Actor_SetSingleChildFlags_(struct Actor *object, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSingleChildFlags(object, flags);
}

static __inline__ void Actor_SetPartAttribute_(struct Actor *object,
                                                s32 value) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPartAttribute(object, value);
}

static __inline__ void Func_02002400_(struct Actor *object) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002400(object);
}

static __inline__ void Func_02002848_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002848(a, b, c, d, e, f);
}

static __inline__ void Func_02002818_(struct Actor *object, s32 x,
                                      s32 y, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002818(object, x, y, z);
}

static __inline__ void Func_02001ff0_(s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001ff0(x, z);
}

static __inline__ void Func_0200271c_(s32 index, s32 active) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200271c(index, active);
}

static __inline__ void Func_020029a8_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020029a8(a, b, c);
}

#endif
