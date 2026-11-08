#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200577c(void);
void Func_020057b4(void);
void Func_02005814(void);
void Func_02005924(void);
extern const u8 MsgWeHaveIgnited[];
s32 Func_0200592c(void);
void Func_02005984(s32 a);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 b);
void wait(s32 frames);
void Func_020058c4(const void *message);
void Func_0200588c(s32 id, s32 x, s32 z, s32 angle);
void Func_020059f4(s32 sound);
void Func_020057ac(s32 a, s32 b, s32 c);
void FieldEvent_Wait(s32 frames);
void Func_02005914(s32 a, s32 b);
void Actor_SetModeById(u32 id, s32 mode);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Func_020058ec(s32 id, s32 angle);
void Actor_OffsetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void Func_020058d4(s32 id, s32 a, s32 b);
void *Func_020058fc(s32 id, s32 emote, s32 frames);
s32 FieldMessage_ShowAtActor(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Actor_CommitPositionAndActivate(s32 id, s32 x, s32 z);
struct Actor *Actor_GetById(s32 id);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 x, s32 z);
void FieldMessage_Show(s32 message, s32 position);
void PartyInventory_Add(s32 item);
void *Unit_Get(s32 id);
void Func_020057ec(s32 id, s32 slot);
void Func_02005904(s32 id, s32 emote);
void Actor_StartRepeatedMotion(s32 id, s32 motion);
void Party_SetFields1eeAnd1f0(s32 a, s32 b);
void Party_SetFields1f2And1f4(s32 a, s32 b);
void GameFlag_SetBit(s32 flag);
void Func_0200593c(s32 a, s32 b);
static __inline__ s32 Func_0200592c_(void) { return Func_0200592c(); }
static __inline__ void Func_02005984_(s32 a) { Func_02005984(a); }
static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z, s32 b) {
  Camera_MoveWithinMapBounds(x, y, z, b);
}
static __inline__ void wait_(s32 frames) { wait(frames); }
static __inline__ void Func_020058c4_(const void *message) {
  Func_020058c4(message);
}
static __inline__ void Func_0200588c_(s32 id, s32 x, s32 z, s32 angle) {
  Func_0200588c(id, x, z, angle);
}
static __inline__ void Func_020059f4_(s32 sound) { Func_020059f4(sound); }
static __inline__ void Func_020057ac_(s32 a, s32 b, s32 c) {
  Func_020057ac(a, b, c);
}
static __inline__ void FieldEvent_Wait_(s32 frames) { FieldEvent_Wait(frames); }
static __inline__ void Func_02005914_(s32 a, s32 b) { Func_02005914(a, b); }
static __inline__ void Actor_ArmCallback_(s32 id, s32 angle, s32 flags) {
  Actor_ArmCallback(id, angle, flags);
}
static __inline__ void Func_020058ec_(s32 id, s32 angle) {
  Func_020058ec(id, angle);
}
static __inline__ void Actor_OffsetPositionAndReset_(s32 id, s32 x, s32 z) {
  Actor_OffsetPositionAndReset(id, x, z);
}
static __inline__ void Actor_WaitForMoveById_(s32 id) {
  Actor_WaitForMoveById(id);
}
static __inline__ void Motion_SetVarCbAndRefresh_(s32 id, s32 variant) {
  Motion_SetVarCbAndRefresh(id, variant);
}
static __inline__ void Func_020058d4_(s32 id, s32 a, s32 b) {
  Func_020058d4(id, a, b);
}
static __inline__ void *Func_020058fc_(s32 id, s32 emote, s32 frames) {
  return Func_020058fc(id, emote, frames);
}
static __inline__ s32 FieldMessage_ShowAtActor_(s32 id, s32 flags) {
  return FieldMessage_ShowAtActor(id, flags);
}
static __inline__ s32 Event_AskYesNoWithActorAnimation_(s32 id, s32 flags) {
  return Event_AskYesNoWithActorAnimation(id, flags);
}
static __inline__ void Actor_CommitPositionAndActivate_(s32 id, s32 x, s32 z) {
  Actor_CommitPositionAndActivate(id, x, z);
}
static __inline__ struct Actor *Actor_GetById_(s32 id) {
  return Actor_GetById(id);
}
static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 id, s32 x,
                                                           s32 z) {
  Actor_OffsetPositionAndResetMotion(id, x, z);
}
static __inline__ void FieldMessage_Show_(s32 message, s32 position) {
  FieldMessage_Show(message, position);
}
static __inline__ void PartyInventory_Add_(s32 item) {
  PartyInventory_Add(item);
}
static __inline__ void *Unit_Get_(s32 id) { return Unit_Get(id); }
static __inline__ void Func_020057ec_(s32 id, s32 slot) {
  Func_020057ec(id, slot);
}
static __inline__ void Func_02005904_(s32 id, s32 emote) {
  Func_02005904(id, emote);
}
static __inline__ void Actor_StartRepeatedMotion_(s32 id, s32 motion) {
  Actor_StartRepeatedMotion(id, motion);
}
static __inline__ void GameFlag_SetBit_(s32 flag) { GameFlag_SetBit(flag); }
static __inline__ void Func_0200593c_(s32 a, s32 b) { Func_0200593c(a, b); }

struct UnitItems {
  u8 unk00[0xd8];
  u16 items[15];
};

#define ACTOR_SET_MODE_AND_WAIT(id, mode, frames)                              \
  do {                                                                         \
    Actor_SetModeById(id, mode);                                               \
    FieldEvent_Wait(frames);                                                   \
  } while (0)

void Func_02000cd0(void) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 612 of 691 instructions differ in
   * tla-en. */
  s32 t2, t4, t75, t77, t78, t79, t80;
  s32 t101, t102, t135, t139, t140, t143, t144;
  s32 slot;
  s32 facing;

  Func_02005814();
  Func_02005984_(0);
  t2 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t2, s32, 428) = 0x100;
  FIELD_AT(t2, s32, 436) = 120;
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  t4 = Func_0200592c_();
  FIELD_AT(t4, u8, 85) = 0;
  Camera_MoveWithinMapBounds_(0x2000000, 0x600000, 0xf00000, 0);
  wait_(1);
  Func_0200577c();
  wait_(1);
  Func_020058c4_(MsgWeHaveIgnited);
  Func_0200588c_(4, 0x1fc0000, 0x1160000, facing = 0xc000);
  Func_0200588c_(7, 0x2140000, 0x1160000, facing);
  Func_020059f4_(141);
  Func_020057ac_(0x40000, 0x40000, 0x10000);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_020057ac_(0x30000, 0x30000, 0x10000);
  FieldEvent_Wait_(60);
  Func_020057ac_(0x20000, 0x20000, 0x10000);
  FieldEvent_Wait_(60);
  Func_020059f4_(0x186);
  Func_020057ac_(-1, -1, 0xe666);
  Func_020057b4();
  Func_02005914_(SPLIT_CONSTANT(SPLIT_CONSTANT(0x6600, 102)), 0xccc);
  Camera_MoveWithinMapBounds_(0x2000000, 0x600000, 0x1280000, 1);
  Func_02005924();
  FieldEvent_Wait_(20);
  ACTOR_SET_MODE_AND_WAIT(7, 3, 20);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 20);
  Actor_ArmCallback_(7, 0x4000, 0);
  Func_020058ec_(4, 0x4000);
  FieldEvent_Wait_(20);
  Actor_OffsetPositionAndReset_(4, 0, 8);
  Actor_OffsetPositionAndReset_(7, 0, 8);
  Actor_OffsetPositionAndReset_(27, 32, -16);
  Actor_OffsetPositionAndReset_(28, 48, -16);
  Actor_WaitForMoveById_(4);
  Actor_WaitForMoveById_(7);
  Actor_WaitForMoveById_(27);
  Actor_WaitForMoveById_(28);
  Actor_ArmCallback_(4, 0x2000, 0);
  Actor_ArmCallback_(7, 0x4000, 0);
  Actor_ArmCallback_(27, 0xa000, 0);
  Actor_ArmCallback_(28, 0xb000, 0);
  Motion_SetVarCbAndRefresh_(7, 2);
  FieldEvent_Wait_(5);
  Func_020059f4_(78);
  Func_020058d4_(7, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(28, 4, 33);
  Func_020059f4_(34);
  Func_020058d4_(0x101c, 0, 0);
  Func_020058fc(7, 0x100, 0);
  Func_020058fc_(4, 0x100, 30);
  ACTOR_SET_MODE_AND_WAIT(27, 4, 33);
  Func_020058d4_(27, 0, 0);
  Actor_ArmCallback_(4, 0, 0);
  Func_020058ec_(7, 0x8000);
  FieldEvent_Wait_(10);
  ACTOR_SET_MODE_AND_WAIT(28, 4, 33);
  Func_020058d4_(0x101c, 0, 0);
  Actor_ArmCallback_(4, 0x2000, 0);
  Func_020058ec_(7, 0x4000);
  FieldEvent_Wait_(10);
  ACTOR_SET_MODE_AND_WAIT(27, 3, 20);
  Func_020058d4_(27, 0, 0);
  Func_020058fc(7, 0x102, 30);
  Motion_SetVarCbAndRefresh_(28, 2);
  FieldEvent_Wait_(5);
  FieldMessage_ShowAtActor_(0x101c, 0);
  t75 = Event_AskYesNoWithActorAnimation_(4, 0);
  if (t75 != 0)
    goto L_02008fc4;
  Func_020058d4_(SPLIT_CONSTANT(0x1000, 28), 0, 0);
  t77 = FIELD_AT(0x3000000, s32, 108);
  t78 = FIELD_AT(t77, u16, 452);
  FIELD_AT(t77, u16, 452) = (t78 + 1);
  goto L_02008fe0;
L_02008fc4:
  t79 = FIELD_AT(0x3000000, s32, 108);
  t80 = FIELD_AT(t79, u16, 452);
  FIELD_AT(t79, u16, 452) = (t80 + 1);
  Func_020058d4_(SPLIT_CONSTANT(0x1000, 28), 0, 0);
L_02008fe0:
  Func_020058fc_(7, 0x100, 0);
  Func_020058fc(4, 0x100, 30);
  Actor_ArmCallback_(7, 0x8000, 0);
  Func_020058ec_(4, 0);
  FieldEvent_Wait_(10);
  Actor_CommitPositionAndActivate_(28, -16, -0x18);
  Actor_ArmCallback_(28, 0xd000, 0);
  Actor_ArmCallback_(4, 0x4000, 0);
  Func_020058fc_(7, 0x102, 0);
  Actor_GetById_(28)->actionFlags &= 0xfe;
  Actor_GetById_(28)->velocityY = 0x40000;
  Actor_OffsetPositionAndResetMotion_(28, 0, 24);
  Actor_WaitForMoveById_(28);
  Actor_GetById_(28)->actionFlags |= 1;
  Actor_ArmCallback_(28, 0xa000, 0);
  Actor_ArmCallback_(7, 0x4000, 0);
  Func_020058d4_(7, 0, 0);
  t101 = FIELD_AT(0x3000000, s32, 108);
  t102 = FIELD_AT(t101, u16, 452);
  FIELD_AT(t101, u16, 452) = (t102 + 1);
  FieldMessage_Show_(((t102 << 16) >> 16), 5);
  PartyInventory_Add_(224);
  for (slot = 0; slot < 15; slot++) {
    if ((((struct UnitItems *)Unit_Get_(4))->items[slot] & 0x1ff) == 224) {
      Func_020057ec_(4, slot);
      break;
    }
  }
  Motion_SetVarCbAndRefresh_(4, 2);
  FieldEvent_Wait_(5);
  Actor_OffsetPositionAndReset_(4, 0, 8);
  Actor_CommitPositionAndActivate_(27, -0x18, -8);
  Actor_GetById_(4)->actionFlags &= 0xfe;
  Actor_OffsetPositionAndResetMotion_(4, 0, -8);
  Func_020058fc_(28, 0x108, 30);
  Func_020058d4_(28, 0, 0);
  Actor_WaitForMoveById_(4);
  Actor_GetById_(4)->actionFlags &= 0xfe;
  Func_020058fc(4, 0x109, 30);
  Func_020058ec_(7, 0x8000);
  Func_020058d4_(7, 0, 0);
  Func_020058fc_(27, 0x108, 30);
  Actor_ArmCallback_(27, 0xd000, 0);
  Func_020058d4_(0x201b, 0, 0);
  Func_020058ec_(7, 0x4000);
  FieldEvent_Wait_(10);
  Func_02005904_(7, 0x102);
  Func_02005904_(gMainRoutineWork.currentOwner, 0x102);
  FieldEvent_Wait_(30);
  Func_020058fc(28, 0x108, 30);
  Actor_ArmCallback_(28, 0xb000, 0);
  FieldMessage_ShowAtActor(28, 0);
  t135 = Event_AskYesNoWithActorAnimation_(4, 0);
  if (t135 != 0)
    goto L_02009236;
  ACTOR_SET_MODE_AND_WAIT(28, 3, 20);
  Func_020058d4_(28, 0, 0);
  t139 = FIELD_AT(0x3000000, s32, 108);
  t140 = FIELD_AT(t139, u16, 452);
  FIELD_AT(t139, u16, 452) = (t140 + 1);
  goto L_02009260;
L_02009236:
  ACTOR_SET_MODE_AND_WAIT(28, 3, 20);
  t143 = FIELD_AT(0x3000000, s32, 108);
  t144 = FIELD_AT(t143, u16, 452);
  FIELD_AT(t143, u16, 452) = (t144 + 1);
  Func_020058d4_(28, 0, 0);
L_02009260:
  Motion_SetVarCbAndRefresh_(7, 2);
  FieldEvent_Wait_(5);
  Func_020058d4_(7, 0, 0);
  Func_020058fc_(27, 0x109, 30);
  Actor_ArmCallback_(27, 0xd000, 0);
  Func_020058d4_(0x201b, 0, 0);
  Func_020058fc(4, 0x108, 30);
  Actor_ArmCallback_(28, 0xd000, 0);
  ACTOR_SET_MODE_AND_WAIT(28, 4, 33);
  Func_020058d4_(28, 0, 0);
  Func_020058fc(4, 0x101, 30);
  Actor_ArmCallback_(27, 0xb000, 0);
  ACTOR_SET_MODE_AND_WAIT(27, 4, 33);
  Func_020058d4_(0x201b, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(28, 3, 20);
  Func_020058d4_(28, 0, 0);
  Actor_StartRepeatedMotion_(4, 2);
  Motion_SetVarCbAndRefresh_(7, 2);
  FieldEvent_Wait_(5);
  ACTOR_SET_MODE_AND_WAIT(27, 3, 20);
  Func_020058d4_(SPLIT_CONSTANT(0x2000, 27), 0, 0);
  /* FAKEMATCH(one-pass): the byte store kept in a once-only block, whose end
   * holds the scene id load behind it, as s16e8.c does with the same store; as
   * the plain store, 4 of 691 instructions differ in tla-en. */
  do {
    gMainRoutineWork.battleRule24b = 2;
  } while (0);
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov6a5, 10);
  Party_SetFields1f2And1f4((s32)&SceneId_Ov6a5, 10);
  GameFlag_SetBit_(0x11b);
  Func_0200593c_(13, 5);
  return;
}
