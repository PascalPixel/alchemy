#include "../../../../../../games/tla/src/scene/champa_cliff_caves/scene.h"
#include "scene_ids.h"
#include "flag_call.h"

#define PALETTE ((u16 *)0x05000000)
#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct EventCounters {
  u8 unk000[0x1c4];
  u16 count;
};

struct Palette5 {
  u16 colour[5];
};

struct Position {
  s32 x;
  s32 y;
  s32 z;
};

void Actor_SetSingleChildFlags(struct FieldActor *object, s32 flags);
void Motion_SetVarCbAndRefresh(s32 actor, s32 variant);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Actor_CommitPositionAndActivate(s32 actor, s32 dx, s32 dz);
void Actor_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);
void Party_SetFields1f2And1f4(s32 scene, s32 entrance);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Func_020034d4(s32 actor, s32 x, s32 z);
s32 Func_02003544(s32 actor, s32 value, s32 frames);
void Func_0200351c(s32 actor, s32 value, s32 frames);
void Func_02003564(void);
void Func_020035bc(void);
void Func_0200343c(s32 x, s32 z, s32 a, s32 b);
void Func_02003434(s32 a, s32 b, s32 c);
void Func_0200357c(s32 actor, s32 value);
void Func_02000734(struct Palette5 *from, struct Palette5 *to, s32 frames);
void Func_02002f58(struct Position *position);
void Func_02001138(void);

static __inline__ void Func_0200351c_(s32 actor, s32 value, s32 frames) {
  /* FAKEMATCH(wrapper): forwarded through this inline; as a direct call, 44 of
   * 613 instructions differ in tla-en. */
  Func_0200351c(actor, value, frames);
}

static __inline__ void Func_02003544_(s32 actor, s32 value, s32 frames) {
  /* FAKEMATCH(wrapper): forwarded through this inline; as a direct call, 41 of
   * 613 instructions differ in tla-en. */
  Func_02003544(actor, value, frames);
}

static __inline__ void Func_020034d4_(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarded through this inline; as a direct call, 2 of
   * 613 instructions differ in tla-en. */
  Func_020034d4(actor, x, z);
}

static __inline__ void Func_0200343c_(s32 x, s32 z, s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarded through this inline; as a direct call, 32 of
   * 613 instructions differ in tla-en. */
  Func_0200343c(x, z, a, b);
}

static __inline__ void Func_02003434_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): forwarded through this inline; as a direct call, 58 of
   * 613 instructions differ in tla-en. */
  Func_02003434(a, b, c);
}

static __inline__ void Actor_SetPositionAndReset_(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarded through this inline; as a direct call, 2 of
   * 613 instructions differ in tla-en. */
  Actor_SetPositionAndReset(actor, x, z);
}

static __inline__ void Actor_SetSpeedParameters_(s32 id, s32 speed,
                                                 s32 acceleration) {
  /* FAKEMATCH(wrapper): forwarded through this inline; as a direct call, 6 of
   * 613 instructions differ in tla-en. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void FieldMessage_ShowAtActor_(s32 speaker, s32 flags) {
  /* FAKEMATCH(wrapper): forwarded through this inline; as a direct call, 35 of
   * 613 instructions differ in tla-en. */
  ((s32 (*)(s32, s32))FieldMessage_ShowAtActor)(speaker, flags);
}

void Func_0200115c(void) {
  struct Palette5 current;
  struct Palette5 target;
  struct Position position;
  const u16 *colour;
  struct MainRoutineWork *party;
  struct FieldActor *object = Actor_GetById(13);

  if (Flag_IsSet(0x97e))
    return;
  Func_0200345c();
  Func_020035b4(0);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 584, 248);
  if (Flag_IsSet(0x974)) {
    Func_02003534_(gMainRoutineWork.currentOwner, 0);
    Func_0200350c((s32)MsgBackForMore);
    Func_0200351c_(11, 0, 5);
    Func_0200362c(36);
    Camera_MoveWithinMapBounds(664 << 16, -1, 0x1000000, 1);
    Func_02003564();
    FieldEvent_Wait(5);
    Motion_SetModeAndWaitAnimation(8, 4);
    Func_0200351c_(0x2008, 0, 5);
    Func_02003544_(8, 265, 30);
    Func_0200351c_(SPLIT_CONSTANT(0x2000, 8), 0, 5);
  } else {
    Flag_Set(SPLIT_CONSTANT(0x900, 116));
    Func_0200350c((s32)MsgHoldItRight);
    Func_0200351c_(11, 0, 5);
    Func_0200362c(36);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
    Camera_MoveWithinMapBounds(664 << 16, -1, 0x1000000, 1);
    Func_02003564();
    FieldEvent_Wait(5);
    Actor_Jump(11, 6, 15);
    Actor_Jump(11, 6, 23);
    Func_0200351c_(11, 0, 5);
    Func_02003544_(11, 263, 30);
    Func_0200351c_(11, 0, 5);
    Func_02003544_(8, 257, 30);
    Func_02003534_(8, 0x3000);
    FieldEvent_Wait(10);
    Func_0200351c_(8, 0, 5);
    Func_02003544_(11, 256, 30);
    Func_02003534_(11, 0xb000);
    FieldEvent_Wait(10);
    Func_0200351c_(11, 0, 5);
    Func_02003544_(11, 263, 30);
    Func_0200351c_(11, 0, 5);
    Motion_SetModeAndWaitAnimation(8, 3);
    Func_0200351c_(8, 0, 5);
    Func_02003534_(8, 0x8000);
    FieldEvent_Wait(5);
    Motion_SetModeAndWaitAnimation(8, 4);
    Func_0200351c_(8, 0, 5);
    Func_02003534_(11, 0x8000);
    FieldEvent_Wait(30);
    Func_02003534_(11, 0xb000);
    FieldEvent_Wait(10);
    Func_02003544_(11, 258, 30);
    Func_0200351c_(11, 0, 5);
    Func_02003534_(11, 0x8000);
    FieldEvent_Wait(20);
    Motion_SetVarCbAndRefresh(11, 2);
    FieldEvent_Wait(5);
    Func_0200351c_(11, 0, 5);
    Func_02003544_(8, 257, 30);
    Func_0200351c_(8, 0, 5);
    Func_02003534_(11, 0xb000);
    FieldEvent_Wait(10);
    Func_02003544_(11, 265, 45);
    Func_02003534_(11, 0x300a);
    FieldEvent_Wait(10);
    Func_0200351c_(11, 0, 5);
    Func_02003544_(8, 258, 30);
    Func_02003534_(8, 0x3000);
    FieldEvent_Wait(10);
    Func_0200351c_(8, 0, 5);
    Func_02003534_(11, 0xb000);
    FieldEvent_Wait(10);
    Motion_SetModeAndWaitAnimation(8, 4);
    FieldEvent_Wait(5);
    Func_02003534_(8, 0x8000);
    FieldEvent_Wait(5);
    Func_0200351c_(8, 0, 5);
    Motion_SetModeAndWaitAnimation(11, 3);
    FieldEvent_Wait(10);
    Motion_SetModeAndWaitAnimation(11, 3);
    FieldEvent_Wait(5);
    Func_0200351c_(11, 0, 5);
    Func_02003534_(11, 0x8000);
    FieldEvent_Wait(5);
  }
  Func_0200350c((s32)MsgOhAllRightListen);
  Func_02003534_(8, 0xb000);
  FieldEvent_Wait(20);
  Actor_GetById(8)->facing = 0x5000;
  Actor_SetModeById(8, 5);
  FieldEvent_Wait(30);
  Actor_SetModeById(8, 6);
  Func_020034d4_(12, 724 << 16, 252 << 16);
  WORD_AT(Actor_GetById(12), 24) = 0x13333;
  WORD_AT(Actor_GetById(12), 28) = 0x13333;
  Actor_SetSingleChildFlags(Actor_GetById(12), 0);
  FieldMessage_ShowAtActor_(0x2008, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Func_020035bc();
    FieldEvent_Wait(15);
    Func_020034d4_(12, 0, 0);
    Actor_SetModeById(8, 1);
    Actor_GetById(8)->facing = 0x8000;
    Func_0200351c_(SPLIT_CONSTANT(0x2000, 8), 0, 5);
    ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
    Actor_CommitPositionAndActivate(4, -16, 0);
    FieldEvent_Wait(5);
  } else {
    FieldEvent_Wait(25);
    ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
    Func_0200351c_(0x2008, 0, 5);
    Func_02003544_(8, 263, 30);
    Func_0200351c_(SPLIT_CONSTANT(0x2000, 8), 0, 5);
    Func_0200362c(78);
    Func_0200343c_(656 << 16, 208 << 16, 0, -4);
    Func_0200343c_(656 << 16, 224 << 16, 0, -4);
    Func_0200343c_(656 << 16, 240 << 16, 0, -4);
    Func_0200343c_(656 << 16, 256 << 16, 0, -4);
    WORD_AT(object, 72) = 0x4000;
    object->motionFlags = 2;
    object->terrainHeight = 0xfc190000;
    Actor_Jump(12, 10, 0);
    Func_0200362c(146);
    Actor_SetSpritePriority(12, 1);
    Actor_SetSpeedParameters_(12, 0x30000, 0x18000);
    Actor_ResetAndSetPositionInMode2(12, 644, 248);
    FieldEvent_Wait(21);
    Func_020034d4_(12, 0, 0);
    FieldEvent_Wait(50);
    Func_02003434_(0x20000, 0x20000, 0x10000);
    FieldEvent_Wait(70);
    Func_0200362c(148);
    FieldEvent_Wait(25);
    colour = (const u16 *)0x050000e4;
    current.colour[0] = *colour++;
    current.colour[1] = *colour++;
    current.colour[2] = *colour++;
    current.colour[3] = *colour++;
    current.colour[4] = *colour++;
    colour = (const u16 *)0x050001a4;
    target.colour[0] = *colour++;
    target.colour[1] = *colour++;
    target.colour[2] = *colour++;
    target.colour[3] = *colour++;
    target.colour[4] = *colour++;
    Func_02000734(&current, &target, 48);
    FieldEvent_Wait(20);
    Func_0200362c(194);
    position.x = 0x02960000;
    position.y = 0;
    position.z = 0xee0000;
    Func_02002f58(&position);
    Func_0200362c(147);
    Func_020034d4_(13, 664 << 16, 248 << 16);
    WORD_AT(object, 72) = 0x10000;
    object->motionFlags = 2;
    object->terrainHeight = 0;
    object->y = 0x200000;
    WORD_AT(object, 24) = 0x4000;
    WORD_AT(object, 108) = (s32)Func_02001138;
    WORD_AT(object, 28) = 0x4000;
    Actor_Jump(13, 11, 0);
    Func_0200362c(147);
    Actor_SetSpeedParameters_(13, 0x33333, 0x19999);
    Actor_SetPositionAndReset_(13, 584, 248);
    Party_SetFields1f2And1f4((s32)&SceneId_Ov685Room4, 98);
    Party_SetFields1eeAnd1f0((s32)&SceneId_Ov685Room4, 99);
    party = &gMainRoutineWork;
    /* FAKEMATCH(barrier): holds the 2's store after the party pointer; as no
     * asm, 10 of 613 instructions differ in tla-en. */
    __asm__("");
    party->battleRule24b = 2;
    Func_0200357c(12, 5);
  }
  Func_02003464();
}
