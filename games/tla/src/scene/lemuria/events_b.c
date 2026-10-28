#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"

struct WarpTarget {
  s32 scene;
  u16 x;
  u16 z;
};

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
s32 ArcTan2(s32, s32);
struct Actor *Actor_GetById(s32 id);
void Animation_SetIndexAndInitializeObjects(struct Actor *object, s32 index);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_CenterOnColumn(s32 id, s32 column, s32 offset);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Actor_TurnToFaceEachOther(s32 id, s32 other, s32 mode);
void Actor_SetModeById(u32 id, s32 mode);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001b7c(struct Actor *object, s32 mode);
void Func_02001b84(s32 scene, s32 x, s32 z);
void Func_02001ba4(struct Actor *object, s32 value);
void Func_02001bcc(void);
void Func_02001bd4(void);
void Func_02001c2c(s32 id, s32 x, s32 z);
void Func_02001c6c(s32 message);
void Func_02001c7c(s32 id, s32 a, s32 b);
void Func_02001c84(s32 a, s32 b);
void Func_02001cac(s32 id, s32 a);
void Func_02001cd4(s32 a);
void Func_02001cf4(s32 a, s32 b, s32 c);
void Func_02001d04(s32 a);
void Func_02001d44(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
s32 Func_02001d54(struct Actor *object);
s32 Func_02001d5c(s32 x, s32 z, s32 layer);
void Func_02001d64(s32 x, s32 z, s32 layer, s32 value);
void Func_02001d7c(s32 a);

extern const struct WarpTarget gData_020021bc;
extern const struct WarpTarget gData_020021c4;
extern const struct WarpTarget gData_020021cc;
extern const struct WarpTarget gData_020021d4;
extern const struct WarpTarget gData_020021dc;
extern const struct WarpTarget gData_020021e4;
extern const struct WarpTarget gData_020021ec;

extern u8 MsgTheDoorIs;
extern u8 MsgItHasBeen;
extern u8 MsgEveryoneSpoilsMe;
extern u8 MsgWeAreThe;
extern u8 MsgAreThereOther;

static __inline__ s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static __inline__ void GameFlag_SetBit_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static __inline__ void wait_(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  wait(frames);
}

static __inline__ void FieldEvent_Wait_(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  FieldEvent_Wait(frames);
}

static __inline__ s32 ArcTan2_(s32 a0, s32 a1) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return ArcTan2(a0, a1);
}

static __inline__ struct Actor *Actor_GetById_(s32 id) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Actor_GetById(id);
}

static __inline__ void
Animation_SetIndexAndInitializeObjects_(struct Actor *object, s32 index) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Animation_SetIndexAndInitializeObjects(object, index);
}

static __inline__ void Actor_ArmCallback_(s32 id, s32 angle, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ArmCallback(id, angle, flags);
}

static __inline__ void Actor_SetSpeedParameters_(s32 id, s32 speed,
                                                        s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_CenterOnColumn_(s32 id, s32 column,
                                                    s32 offset) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_CenterOnColumn(id, column, offset);
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

static __inline__ void Actor_TurnToFaceEachOther_(s32 id, s32 other, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_TurnToFaceEachOther(id, other, mode);
}

static __inline__ void Actor_SetModeById_(u32 id, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetModeById(id, mode);
}

static __inline__ void Event_FinishScreenTransition_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Event_FinishScreenTransition();
}

static __inline__ void Event_WaitTransitionFrames_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Event_WaitTransitionFrames();
}

static __inline__ void Func_02001b7c_(struct Actor *object, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001b7c(object, mode);
}

static __inline__ void Func_02001b84_(s32 scene, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001b84(scene, x, z);
}

static __inline__ void Func_02001ba4_(struct Actor *object, s32 value) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001ba4(object, value);
}

static __inline__ void Func_02001bcc_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001bcc();
}

static __inline__ void Func_02001bd4_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001bd4();
}

static __inline__ void Func_02001c2c_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001c2c(id, x, z);
}

static __inline__ void Func_02001c6c_(s32 message) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001c6c(message);
}

static __inline__ void Func_02001c7c_(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001c7c(id, a, b);
}

static __inline__ void Func_02001c84_(s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001c84(a, b);
}

static __inline__ void Func_02001cac_(s32 id, s32 a) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001cac(id, a);
}

static __inline__ void Func_02001cd4_(s32 a) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001cd4(a);
}

static __inline__ void Func_02001cf4_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001cf4(a, b, c);
}

static __inline__ void Func_02001d04_(s32 a) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001d04(a);
}

static __inline__ void Func_02001d44_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001d44(a, b, c, d, e, f, g);
}

static __inline__ s32 Func_02001d54_(struct Actor *object) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Func_02001d54(object);
}

static __inline__ s32 Func_02001d5c_(s32 x, s32 z, s32 layer) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Func_02001d5c(x, z, layer);
}

static __inline__ void Func_02001d64_(s32 x, s32 z, s32 layer, s32 value) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001d64(x, z, layer, value);
}

static __inline__ void Func_02001d7c_(s32 a) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001d7c(a);
}

void Func_020005ac(s32 a) { Func_02001cf4_(a, 64, 0x300); }

void Func_020005bc(s32 a) { Func_02001cf4_(a, 65, 0x301); }

void Func_020005cc(s32 a) { Func_02001cf4_(a, 66, 0x302); }

void Func_020005dc(s32 a) { Func_02001cf4_(a, 67, 0x303); }

void Func_020005ec(s32 a) { Func_02001cf4_(a, 68, 0x304); }

void Func_020005fc(s32 a) { Func_02001cf4_(a, 70, 0x306); }

void Func_0200060c(s32 a) { Func_02001cf4_(a, 71, 0x307); }

void Func_0200061c(s32 a) { Func_02001cf4_(a, 72, 0x308); }
