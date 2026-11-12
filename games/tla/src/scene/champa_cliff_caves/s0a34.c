#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "version.h"

#if TLA_PATCH_4
#define END_THROW_EFFECT Field_EndActorSpriteEffect
#else
#define END_THROW_EFFECT Field_StartPaletteTransition
#endif

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

struct ThrowAnimations {
  s32 animations[3];
};

extern const struct ThrowAnimations gData_02003634;

struct Actor *Actor_GetById(u32 objectId);
void Func_0200345c(void);
void Event_ResolvePendingActions(s32 mode);
void Event_SetMessageId(s32 message);
void Func_020005e8(s32 value);
s32 Func_080d47b4(s32 actorId, s32 emote, s32 frames);
void Event_ShowMessageThenWait(s32 speaker, s32 a, s32 b);
void Func_0200067c(s32 mode);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_ArmCallbackAndRefresh(s32 id, s32 angle);
void Event_Wait(s32 frames);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 follow);
void Func_02003564(void);
void Func_080d3b28(s32 id, s32 animation);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Actor_SetSpeedParameters(s32 actorId, s32 speed, s32 acceleration);
void Actor_OffsetPositionAndResetMotion(s32 actorId, s32 dx, s32 dz);
void Sound_Play(s32 cue);
void Func_02000734(u16 *from, u16 *to, s32 frames);
void Actor_CommitPositionAndActivate(u32 objectId, s32 x_offset, s32 z_offset);
void Actor_StartRepeatedMotion(u32 objectId, s32 variant);
void Func_080d489c(s32 id, s32 animation);
void Func_0200360c(s32 a, s32 b);
void Map_SetScreenShake(s32 x, s32 y, s32 z);
void Func_02003164(s32 *position);
void Func_0200320c(s32 *position);
void Field_StartPaletteTransition(s32 mode);
void Field_EndActorSpriteEffect(s32 mode);
void Func_020032a4(void);
void wait(s32 frames);
void Func_020035bc(void);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
void Func_02003464(void);
void Func_02000038(void);

void Func_02000a34(s32 value) {
  /* FAKEMATCH(wrapper): calls forwarded through call.h's CallN and ValueN; as
   * direct calls, 146 of 529 instructions differ in tla-en. */
  struct Actor *thrown = Actor_GetById(10);
  u16 bright[5];
  u16 dim[5];
  s32 glow[3];
  s32 i;
  struct Actor *object;
  u16 *palette;

  glow[0] = 0x2960000;
  glow[1] = 0;
  glow[2] = 0xee0000;
  palette = (u16 *)0x050000e4;
  bright[0] = *palette++;
  bright[1] = *palette++;
  bright[2] = *palette++;
  bright[3] = *palette++;
  bright[4] = *palette;
  palette = (u16 *)0x050000c4;
  dim[0] = *palette++;
  dim[1] = *palette++;
  dim[2] = *palette++;
  dim[3] = *palette++;
  dim[4] = *palette;
  Func_0200345c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgWellDoneYou);
  Func_020005e8(value);
  Value3((s32 (*)())Func_080d47b4, 8, 0x100, 30);
  Event_ShowMessageThenWait(8, 0, 5);
  Value3((s32 (*)())Func_080d47b4, 8, 0x108, 30);
  Event_ShowMessageThenWait(8, 0, 5);
  Func_0200067c(1);
  Call3((void (*)())Actor_SetPositionAndReset, 8, 0x298, 200);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 8, 0x3000);
  Event_Wait(10);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 8, 0x8000);
  Event_Wait(10);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 8, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(20);
    Event_ShowMessageThenWait(8, 0, 5);
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 452)++;
  } else {
    Event_Wait(40);
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 452)++;
    Event_ShowMessageThenWait(8, 0, 5);
  }
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner, 0x2000,
        0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 8, 0x5000);
  Event_Wait(10);
  Camera_MoveWithinMapBounds(0x2980000, -1, 0xe80000, 1);
  Func_02003564();
  Event_Wait(10);
  Value3((s32 (*)())Func_080d47b4, 8, 0x107, 40);
  Event_ShowMessageThenWait(0x2008, 0, 5);
  for (i = 0; i <= 2; i++) {
    struct ThrowAnimations animations;
    s32 landing[3];
    animations = gData_02003634;
    Actor_SetModeById(8, 9);
    Event_Wait(3);
    Func_080d3b28(10, animations.animations[i]);
    Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 10, 0x29c0000,
          0xc80000);
    thrown->gravity = 0x4000;
    Actor_Jump(10, 4, 2);
    Actor_SetSpritePriority(10, 1);
    Call3((void (*)())Actor_SetSpeedParameters, 10, 0x10000, 0x8000);
    Actor_OffsetPositionAndResetMotion(10, 0, 40);
    Sound_Play(0x136);
    Event_Wait(34);
    landing[0] = thrown->x;
    landing[1] = thrown->y;
    landing[2] = thrown->z;
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Event_Wait(20);
    Sound_Play(134);
    Func_02000734(bright, dim, 12);
    Func_02000734(dim, bright, 48);
    Event_Wait(10);
  }
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  object->actionFlags &= ~1;
  Actor_CommitPositionAndActivate(gMainRoutineWork.currentOwner, 12, 12);
  Event_Wait(1);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, 0x2000);
  Event_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  Event_Wait(20);
  Actor_SetModeById(8, 7);
  Event_Wait(60);
  Sound_Play(78);
  Value3((s32 (*)())Func_080d47b4, 8, 0x101, 60);
  Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
  Event_Wait(20);
  Sound_Play(220);
  Func_02000734(bright, dim, 48);
  Actor_StartRepeatedMotion(gMainRoutineWork.currentOwner, 3);
  Call2((void (*)())Func_080d489c, gMainRoutineWork.currentOwner, 0x102);
  Event_Wait(50);
  Func_080d47b4(8, 0x101, 60);
  Call2((void (*)())Func_0200360c, 16, 0x1090);
  Call3((void (*)())Map_SetScreenShake, 0x20000, 0x20000, 0x10000);
  Func_02003164(glow);
  Actor_SetModeById(8, 1);
  Actor_Jump(4, 6, 0);
  Sound_Play(194);
  Actor_GetById(4)->actionFlags &= ~1;
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, -12, -12);
  Event_Wait(1);
  Actor_GetById(4)->actionFlags |= 1;
  Event_Wait(10);
  Actor_StartRepeatedMotion(8, 3);
  Func_080d489c(8, 0x102);
  Event_Wait(30);
  Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
  Call3((void (*)())Map_SetScreenShake, 0x40000, 0x40000, 0x10000);
  Func_0200320c(glow);
  Sound_Play(194);
  Event_Wait(10);
  Actor_Jump(8, 6, 15);
  Actor_Jump(8, 6, 23);
  Sound_Play(194);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x2008, 0, 5);
  Call2((void (*)())Func_080d3b28, 10, 0x146);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 10, 0x2980000,
        0xe80000);
  Actor_SetSpritePriority(10, 1);
  thrown->motionFlags = 2;
  thrown->y = 0x180000;
  thrown->update = Func_02000038;
  thrown->terrainHeight = 0;
  thrown->scaleX = 0;
  thrown->scaleY = 0;
  Actor_Jump(10, 5, 0);
  END_THROW_EFFECT(8);
  Func_020032a4();
  Call3((void (*)())Map_SetScreenShake, -1, -1, 0xe666);
  while (thrown->y > 0)
    wait(1);
  thrown->gravity = 0x10000;
  thrown->y = 0;
  thrown->motionFlags = 4;
  Func_020035bc();
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(5);
  Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
  Actor_ArmCallbackAndRefresh(8, 0x8000);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x2008, 0, 5);
  Actor_ArmCallbackAndRefresh(8, 0x5000);
  Event_Wait(10);
  /* FAKEMATCH(dead): the call in a once-only block, which orders the arguments
   * r0 first; as a plain call, 2 of 529 instructions differ in tla-en. */
  do {
    Actor_SetSpritePriority(10, 2);
  } while (0);
  Func_02003464();
}

#include "event_inline.h"
#include "scene.h"

struct EventFlags685 {
  u8 unk000[0x15a];
  u16 field15a;
};

void PartyInventory_Remove(s32 item);
void Func_0200343c(s32 x, s32 z, s32 a, s32 b);
void Func_02000804(s32 item);
void Func_02000978(s32 item);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 GameFlag_Test_(s32 flag) { return GameFlag_Test(flag); }

static __inline__ void Func_0200343c_(s32 x, s32 z, s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarded through this inline; as a direct call, 32 of
   * 613 instructions differ in tla-en. */
  Func_0200343c(x, z, a, b);
}

static __inline__ void Actor_ArmCallback_(s32 actor, s32 angle, s32 frames) {
  Actor_ArmCallback(actor, angle, frames);
}

void Func_02000f48(s32 actor) {
  s32 item;
  /* FAKEMATCH(dead): each owner stored and never read; as the bare tests,
   * 68 of 205 instructions differ in tla-en. */
  s32 owner;

  if ((Actor_GetById(gMainRoutineWork.currentOwner)->x >> 20) <= 41)
    goto first;
  ((struct EventFlags685 *)Ram_HeapSlots->eventWork)->field15a = 1;
  return;
remove:
  PartyInventory_Remove(item);
  goto chosen;
last:
  item = 441;
  if ((owner = PartyInventory_FindOwner(item)) != -1)
    goto remove;
  item = -1;
  goto chosen;
first:
  item = 439;
  if ((owner = PartyInventory_FindOwner(item)) != -1)
    goto remove;
  item++;
  if ((owner = PartyInventory_FindOwner(item)) == -1)
    goto last;
  PartyInventory_Remove(item);
chosen:
  if (item != -1) {
    if (!GameFlag_Test_(0x975)) {
      GameFlag_SetBit(0x975);
      Func_02000804(item);
    } else if (!GameFlag_Test_(0x976)) {
      GameFlag_SetBit(0x976);
      Func_02000978(item);
    } else if (!GameFlag_Test_(0x977)) {
      GameFlag_SetBit(0x977);
      Func_0200343c_(164 << 18, 208 << 16, 0, -4);
      Func_0200343c_(164 << 18, 224 << 16, 0, -4);
      Func_0200343c_(164 << 18, 240 << 16, 0, -4);
      Func_0200343c_(164 << 18, 128 << 17, 0, -4);
      Func_02000a34(item);
      Func_0200343c_(164 << 18, 208 << 16, 0, 0);
      Func_0200343c_(164 << 18, 224 << 16, 0, 0);
      Func_0200343c_(164 << 18, 240 << 16, 0, 0);
      Func_0200343c_(164 << 18, 128 << 17, 0, 0);
    }
  } else {
    Actor_SetAngleToward(actor, gMainRoutineWork.currentOwner, 5);
    if (!GameFlag_Test_(0x975))
      Event_SetMessageId((s32)MsgIMeanIt);
    else if (!GameFlag_Test_(0x976))
      Event_SetMessageId((s32)MsgTheLegendSaysThat);
    else if (!GameFlag_Test_(0x977))
      Event_SetMessageId((s32)MsgYesThereS);
    else
      Event_SetMessageId((s32)MsgTheTridentSeems);
    Event_ShowMessageAndWait(actor, 0);
    Actor_ArmCallback_(actor, 0x5000, 0);
  }
}

void Func_02001138(struct RiseObject *object) {
  object->scale += 0xc00;
  if (object->scale >= 0x10000) {
    object->scale = 0x10000;
    object->update = 0;
  }
  object->scaleCopy = object->scale;
}

#include "scene_ids.h"
#include "flag_call.h"

#define PALETTE ((u16 *)0x05000000)
#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct EventCounters {
  u8 unk000[0x1c4];
  u16 count;
};

struct Position {
  s32 x;
  s32 y;
  s32 z;
};

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);
void Func_0200357c(s32 actor, s32 value);
void Func_02002f58(struct Position *position);

static __inline__ void Func_0200351c_(s32 actor, s32 value, s32 frames) {
  /* FAKEMATCH(wrapper): forwarded through this inline; as a direct call, 44 of
   * 613 instructions differ in tla-en. */
  Event_ShowMessageThenWait(actor, value, frames);
}

static __inline__ void Func_02003544_(s32 actor, s32 value, s32 frames) {
  /* FAKEMATCH(wrapper): forwarded through this inline; as a direct call, 41 of
   * 613 instructions differ in tla-en. */
  Func_080d47b4(actor, value, frames);
}

static __inline__ void Func_020034d4_(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarded through this inline; as a direct call, 2 of
   * 613 instructions differ in tla-en. */
  Actor_SetHorizontalPositionWithTerrain(actor, x, z);
}

static __inline__ void Func_02003434_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): forwarded through this inline; as a direct call, 58 of
   * 613 instructions differ in tla-en. */
  Map_SetScreenShake(a, b, c);
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
  u16 current[5];
  u16 target[5];
  struct Position position;
  const u16 *colour;
  struct MainRoutineWork *party;
  struct Actor *object = Actor_GetById(13);

  if (Flag_IsSet(0x97e))
    return;
  Func_0200345c();
  Event_ResolvePendingActions(0);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 584, 248);
  if (Flag_IsSet(0x974)) {
    Func_02003534_(gMainRoutineWork.currentOwner, 0);
    Event_SetMessageId((s32)MsgBackForMore);
    Func_0200351c_(11, 0, 5);
    Sound_Play(36);
    Camera_MoveWithinMapBounds(664 << 16, -1, 0x1000000, 1);
    Func_02003564();
    Event_Wait(5);
    Motion_SetModeAndWaitAnimation(8, 4);
    Func_0200351c_(0x2008, 0, 5);
    Func_02003544_(8, 265, 30);
    Func_0200351c_(0x2008, 0, 5);
  } else {
    Flag_Set(0x974);
    Event_SetMessageId((s32)MsgHoldItRight);
    Func_0200351c_(11, 0, 5);
    Sound_Play(36);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
    Camera_MoveWithinMapBounds(664 << 16, -1, 0x1000000, 1);
    Func_02003564();
    Event_Wait(5);
    Actor_Jump(11, 6, 15);
    Actor_Jump(11, 6, 23);
    Func_0200351c_(11, 0, 5);
    Func_02003544_(11, 263, 30);
    Func_0200351c_(11, 0, 5);
    Func_02003544_(8, 257, 30);
    Func_02003534_(8, 0x3000);
    Event_Wait(10);
    Func_0200351c_(8, 0, 5);
    Func_02003544_(11, 256, 30);
    Func_02003534_(11, 0xb000);
    Event_Wait(10);
    Func_0200351c_(11, 0, 5);
    Func_02003544_(11, 263, 30);
    Func_0200351c_(11, 0, 5);
    Motion_SetModeAndWaitAnimation(8, 3);
    Func_0200351c_(8, 0, 5);
    Func_02003534_(8, 0x8000);
    Event_Wait(5);
    Motion_SetModeAndWaitAnimation(8, 4);
    Func_0200351c_(8, 0, 5);
    Func_02003534_(11, 0x8000);
    Event_Wait(30);
    Func_02003534_(11, 0xb000);
    Event_Wait(10);
    Func_02003544_(11, 258, 30);
    Func_0200351c_(11, 0, 5);
    Func_02003534_(11, 0x8000);
    Event_Wait(20);
    Motion_SetVarCbAndRefresh(11, 2);
    Event_Wait(5);
    Func_0200351c_(11, 0, 5);
    Func_02003544_(8, 257, 30);
    Func_0200351c_(8, 0, 5);
    Func_02003534_(11, 0xb000);
    Event_Wait(10);
    Func_02003544_(11, 265, 45);
    Func_02003534_(11, 0x300a);
    Event_Wait(10);
    Func_0200351c_(11, 0, 5);
    Func_02003544_(8, 258, 30);
    Func_02003534_(8, 0x3000);
    Event_Wait(10);
    Func_0200351c_(8, 0, 5);
    Func_02003534_(11, 0xb000);
    Event_Wait(10);
    Motion_SetModeAndWaitAnimation(8, 4);
    Event_Wait(5);
    Func_02003534_(8, 0x8000);
    Event_Wait(5);
    Func_0200351c_(8, 0, 5);
    Motion_SetModeAndWaitAnimation(11, 3);
    Event_Wait(10);
    Motion_SetModeAndWaitAnimation(11, 3);
    Event_Wait(5);
    Func_0200351c_(11, 0, 5);
    Func_02003534_(11, 0x8000);
    Event_Wait(5);
  }
  Event_SetMessageId((s32)MsgOhAllRightListen);
  Func_02003534_(8, 0xb000);
  Event_Wait(20);
  Actor_GetById(8)->facing = 0x5000;
  Actor_SetModeById(8, 5);
  Event_Wait(30);
  Actor_SetModeById(8, 6);
  Func_020034d4_(12, 724 << 16, 252 << 16);
  WORD_AT(Actor_GetById(12), 24) = 0x13333;
  WORD_AT(Actor_GetById(12), 28) = 0x13333;
  Actor_SetSingleChildFlags(Actor_GetById(12), 0);
  FieldMessage_ShowAtActor_(0x2008, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Func_020035bc();
    Event_Wait(15);
    Func_020034d4_(12, 0, 0);
    Actor_SetModeById(8, 1);
    Actor_GetById(8)->facing = 0x8000;
    Func_0200351c_(0x2008, 0, 5);
    ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
    Actor_CommitPositionAndActivate(4, -16, 0);
    Event_Wait(5);
  } else {
    Event_Wait(25);
    ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
    Func_0200351c_(0x2008, 0, 5);
    Func_02003544_(8, 263, 30);
    Func_0200351c_(0x2008, 0, 5);
    Sound_Play(78);
    Func_0200343c_(656 << 16, 208 << 16, 0, -4);
    Func_0200343c_(656 << 16, 224 << 16, 0, -4);
    Func_0200343c_(656 << 16, 240 << 16, 0, -4);
    Func_0200343c_(656 << 16, 256 << 16, 0, -4);
    WORD_AT(object, 72) = 0x4000;
    object->motionFlags = 2;
    object->terrainHeight = 0xfc190000;
    Actor_Jump(12, 10, 0);
    Sound_Play(146);
    Actor_SetSpritePriority(12, 1);
    Actor_SetSpeedParameters_(12, 0x30000, 0x18000);
    Actor_ResetAndSetPositionInMode2(12, 644, 248);
    Event_Wait(21);
    Func_020034d4_(12, 0, 0);
    Event_Wait(50);
    Func_02003434_(0x20000, 0x20000, 0x10000);
    Event_Wait(70);
    Sound_Play(148);
    Event_Wait(25);
    colour = (const u16 *)0x050000e4;
    current[0] = *colour++;
    current[1] = *colour++;
    current[2] = *colour++;
    current[3] = *colour++;
    current[4] = *colour++;
    colour = (const u16 *)0x050001a4;
    target[0] = *colour++;
    target[1] = *colour++;
    target[2] = *colour++;
    target[3] = *colour++;
    target[4] = *colour++;
    Func_02000734(current, target, 48);
    Event_Wait(20);
    Sound_Play(194);
    position.x = 0x02960000;
    position.y = 0;
    position.z = 0xee0000;
    Func_02002f58(&position);
    Sound_Play(147);
    Func_020034d4_(13, 664 << 16, 248 << 16);
    WORD_AT(object, 72) = 0x10000;
    object->motionFlags = 2;
    object->terrainHeight = 0;
    object->y = 0x200000;
    WORD_AT(object, 24) = 0x4000;
    WORD_AT(object, 108) = (s32)Func_02001138;
    WORD_AT(object, 28) = 0x4000;
    Actor_Jump(13, 11, 0);
    Sound_Play(147);
    Actor_SetSpeedParameters_(13, 0x33333, 0x19999);
    Actor_SetPositionAndReset_(13, 584, 248);
    Party_SetFields1f2And1f4((s32)&SceneId_Ov685Room4, 98);
    Party_SetFields1eeAnd1f0((s32)&SceneId_Ov685Room4, 99);
    party = &gMainRoutineWork;
    /* FAKEMATCH(dead): holds the 2's store after the party pointer; as no
     * block, 10 of 613 instructions differ in tla-en. */
    do {
    } while (0);
    party->battleRule24b = 2;
    Func_0200357c(12, 5);
  }
  Func_02003464();
}

#include "scene_tables.h"

struct EventTransition {
  u8 unk000[0x1ac];
  s32 type;
  u8 unk1b0[4];
  s32 frames;
};

void Func_0200356c(s32 scene, s32 entrance);

// The scene's own event inline: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Func_02001774(void) {
  struct EventTransition *work;

  Func_0200345c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgHaveYouLearned);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,
                                         568 << 16, 248 << 16);
  Func_020034d4_(8, 584 << 16, 224 << 16);
  Func_020034d4_(13, 600 << 16, 240 << 16);
  Func_020034d4_(11, 584 << 16, 264 << 16);
  Actor_ArmCallback_(8, 0x5000, 0);
  Actor_ArmCallback_(11, 0xb000, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 41);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  Func_080d47b4(11, 264, 30);
  Event_ShowMessageThenWait(11, 0, 5);
  Motion_SetModeAndWaitAnimation(8, 4);
  Event_ShowMessageThenWait(8, 0, 20);
  work = Ram_HeapSlots->eventWork;
  work->frames = 40;
  work->type = 521;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_0200356c((s16)gMainRoutineWork.savedScene,
                (s16)gMainRoutineWork.savedEntrance);
}

extern const u8 gData_020041e8[];
extern const u8 gData_02004254[];
extern const u8 gData_0200432c[];
extern const u8 gData_020042d4[];

void Func_020029f8(s32 id, s32 mode);

/// The scene that opens with MsgMySalamanderYou: persons 8 and 11 talk and
/// move with the party through three yes-or-no answers (each counted in the
/// event work), person 14 is brought in and the others follow it, then
/// persons 5, 6, 7 and 15 join the leader and flag 0x97e is set.
void Func_02001868(void) {
  const u8 *scriptA, *scriptB, *scriptC, *scriptD;
  s32 *owner;
  u8 *party;
  struct Actor *leader;

  Func_0200345c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgMySalamanderYou);
  owner = (s32 *)&gMainRoutineWork;
  owner = (s32 *)((u8 *)owner + 532);
  Actor_SetHorizontalPositionWithTerrain(*owner, PIXELS(568), PIXELS(256));
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 13, PIXELS(592),
        PIXELS(240));
  Actor_SetModeById(13, 3);
  Actor_ArmCallback(*owner, FACING_EAST, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(656), -1, PIXELS(248),
        0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 5);
  Motion_SetVarCbAndRefresh(13, 2);
  Event_Wait(5);
  Func_020029f8(13, 1);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x10000, 0x8000);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -12, -18);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -10, -14);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -18, -12);
  Call3((void (*)())Actor_SetPositionAndReset, 8, 664, 200);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 8, 0x5000);
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 5);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -16, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -16, 8);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -16, 16);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -8, 16);
  Call3((void (*)())Actor_SetPositionAndReset, 8, 608, 248);
  Actor_ArmCallbackAndRefresh(8, FACING_WEST);
  Event_Wait(5);
  Motion_SetModeAndWaitAnimation(8, 4);
  Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
  Func_02003544_(11, 0x102, 40);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x1b333, 0xd999);
  Call3((void (*)())Actor_CommitPositionAndActivate, 11, -16, 0);
  Event_Wait(5);
  Event_ShowMessageThenWait(11, 0, 5);
  Actor_ArmCallbackAndRefresh(8, FACING_EAST);
  Event_Wait(5);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_Jump(11, 6, 15);
  Actor_Jump(11, 6, 23);
  Event_ShowMessageThenWait(11, 0, 5);
  Func_080d47b4(8, 0x107, 30);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_StartRepeatedMotion(11, 3);
  Call2((void (*)())Func_080d489c, 11, 0x102);
  Event_Wait(38);
  Event_ShowMessageThenWait(11, 0, 5);
  Motion_SetModeAndWaitAnimation(8, 4);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_StartRepeatedMotion(11, 3);
  Call2((void (*)())Func_080d489c, 11, 0x102);
  Event_Wait(37);
  Event_ShowMessageThenWait(11, 0, 5);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 8, 0x7ff6);
  Event_Wait(25);
  Actor_ArmCallbackAndRefresh(8, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 5);
  Motion_SetVarCbAndRefresh(11, 2);
  Event_Wait(30);
  Func_080d47b4(11, 0x106, 30);
  Event_ShowMessageThenWait(11, 0, 10);
  Func_02003544_(8, 0x102, 40);
  Event_ShowMessageThenWait(8, 0, 0);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_ArmCallbackAndRefresh(8, 0x7ff6);
  Event_Wait(10);
  Func_080d47b4(8, 0x101, 40);
  Call4((void (*)())Func_080d3070, 7, 0, -16, 0);
  Call4((void (*)())Func_080d3070, 5, -16, 0, 0);
  Call4((void (*)())Func_080d3070, 6, -16, -16, 0);
  Call4((void (*)())Func_080d3070, 15, 16, -16, 0);
  Actor_WaitForMoveById(15);
  Event_Wait(10);
  Camera_MoveWithinMapBounds(PIXELS(640), -1, PIXELS(248), 1);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(6, 4);
  Call3((void (*)())Event_ShowMessageThenWait, 0x1006, 0, 5);
  Actor_StartRepeatedMotion(8, 3);
  Call2((void (*)())Func_080d489c, 8, 0x102);
  Actor_StartRepeatedMotion(11, 3);
  Call2((void (*)())Func_080d489c, 11, 0x102);
  Event_Wait(40);
  Event_ShowMessageThenWait(0x2008, 0, 5);
  Motion_SetModeAndWaitAnimation(5, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x1005, 0, 5);
  Actor_ArmCallbackAndRefresh(8, FACING_EAST);
  Event_Wait(5);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_StartRepeatedMotion(11, 3);
  Func_080d489c(11, 0x102);
  Event_Wait(35);
  Event_ShowMessageThenWait(11, 0, 0);
  Motion_SetModeAndWaitAnimation(7, 4);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_ArmCallbackAndRefresh(15, 0x5000);
  Event_Wait(5);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 0x200f, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(20);
    Actor_ArmCallbackAndRefresh(6, FACING_SOUTHEAST);
    Event_Wait(5);
    Motion_SetModeAndWaitAnimation(6, 3);
    Event_Wait(10);
    Event_ShowMessageThenWait(0x1006, 0, 5);
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 452)++;
  } else {
    Event_Wait(35);
    Actor_ArmCallbackAndRefresh(6, FACING_SOUTHEAST);
    Event_Wait(5);
    Motion_SetModeAndWaitAnimation(6, 4);
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 452)++;
    Event_ShowMessageThenWait(0x1006, 0, 5);
  }
  Actor_ArmCallback(15, FACING_EAST, 0);
  Actor_ArmCallbackAndRefresh(6, FACING_EAST);
  Event_ShowMessageThenWait(8, 0, 5);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x2cccc, 0x16666);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x36666, 0x1b333);
  Actor_StartRepeatedMotion(11, 3);
  Call2((void (*)())Func_080d489c, 11, 0x102);
  scriptA = gData_020041e8;
  Actor_SetMovementTypeById(8, scriptA);
  Event_Wait(20);
  Actor_ResetAndSetPositionInMode2(11, 664, 290);
  Actor_RefreshSelectorById(8);
  scriptB = gData_02004254;
  Actor_SetMovementTypeById(8, scriptB);
  Actor_WaitForMoveById(11);
  scriptC = gData_0200432c;
  Actor_SetMovementTypeById(11, scriptC);
  Actor_RefreshSelectorById(8);
  scriptD = gData_020042d4;
  Actor_SetMovementTypeById(8, scriptD);
  Actor_RefreshSelectorById(11);
  Actor_SetMovementTypeById(11, scriptA);
  Actor_RefreshSelectorById(8);
  Call3((void (*)())Actor_ArmCallback, 8, 0xb000, 0);
  Actor_RefreshSelectorById(11);
  Call3((void (*)())Actor_ArmCallback, 11, 0x3000, 0);
  Event_ShowMessageThenWait(11, 0, 5);
  Motion_SetModeAndWaitAnimation(8, 4);
  Event_ShowMessageThenWait(8, 0, 5);
  Func_02003544_(11, 0x102, 30);
  Event_ShowMessageThenWait(11, 0, 5);
  Func_080d47b4(8, 0x103, 30);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_StartRepeatedMotion(11, 3);
  Call2((void (*)())Func_080d489c, 11, 0x102);
  Actor_SetMovementTypeById(8, scriptC);
  Event_Wait(20);
  Actor_WaitForMoveById(11);
  Actor_SetMovementTypeById(11, scriptB);
  Actor_RefreshSelectorById(8);
  Actor_SetMovementTypeById(8, scriptA);
  Actor_RefreshSelectorById(11);
  Actor_SetMovementTypeById(11, scriptD);
  Actor_RefreshSelectorById(8);
  Call3((void (*)())Actor_ArmCallback, 8, 0x3000, 0);
  Actor_RefreshSelectorById(11);
  Call3((void (*)())Actor_ArmCallback, 11, 0xb000, 0);
  Motion_SetModeAndWaitAnimation(11, 4);
  Event_ShowMessageThenWait(11, 0, 5);
  Motion_SetModeAndWaitAnimation(8, 4);
  Event_ShowMessageThenWait(8, 0, 5);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_SetSpritePriority(4, 3);
  Actor_SetSpritePriority(5, 3);
  Actor_SetSpritePriority(6, 3);
  Actor_SetSpritePriority(7, 3);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_NORTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_NORTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_NORTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 15, FACING_WEST, 0);
  Call3((void (*)())Actor_ArmCallback, 11, FACING_WEST, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 8, FACING_WEST);
  Event_Wait(10);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(568), -1, PIXELS(248),
        1);
  Func_02003564();
  Event_Wait(10);
  Actor_SetSpritePriority(14, 2);
  Actor_GetById(14)->facing = 0x3000;
  Actor_GetById(14)->motionFlags = 0;
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 14, PIXELS(520),
        PIXELS(200));
  Call3((void (*)())Actor_SetSpeedParameters, 14, 0x10000, 0x8000);
  Actor_CommitPositionAndActivate(14, 0, 16);
  Actor_ArmCallback(14, 0x3000, 0);
  Event_ShowMessageThenWait(11, 0, 5);
  Actor_WatchAndSetScript(4, 14);
  Actor_WatchAndSetScript(7, 14);
  Actor_WatchAndSetScript(5, 14);
  Actor_WatchAndSetScript(6, 14);
  Actor_WatchAndSetScript(15, 14);
  Actor_WatchAndSetScript(8, 14);
  Actor_CommitPositionAndActivate(14, 0, 16);
  Actor_SetSpritePriority(14, 3);
  Actor_CommitPositionAndActivate(14, 0, 32);
  Actor_CommitPositionAndActivate(14, 8, 8);
  Actor_CommitPositionAndActivate(14, 64, 0);
  Call3((void (*)())Actor_ArmCallback, 8, 0x5000, 0);
  Camera_MoveWithinMapBounds(PIXELS(600), -1, PIXELS(256), 1);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x10000, 0x8000);
  Call3((void (*)())Actor_CommitPositionAndActivate, 11, -32, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 11, -16, -16);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 11, FACING_WEST);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(11, 0, 5);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 14, 0xb000);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x200e, 0, 5);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x10000, 0x8000);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -16, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -16, 8);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -16, 16);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -8, 8);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 8, 0x5000);
  Event_ShowMessageThenWait(8, 0, 5);
  Call3((void (*)())Actor_ArmCallback, 11, 0xb000, 0);
  Actor_ArmCallbackAndRefresh(14, 0xd000);
  Event_Wait(15);
  Motion_SetModeAndWaitAnimation(8, 4);
  Event_ShowMessageThenWait(8, 0, 5);
  Motion_SetModeAndWaitAnimation(14, 4);
  Event_ShowMessageThenWait(14, 0, 5);
  Func_080d47b4(8, 0x109, 30);
  Event_ShowMessageThenWait(8, 0, 5);
  Motion_SetModeAndWaitAnimation(11, 4);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 11, 0);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTHEAST, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 8, 0x5000);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(20);
    Motion_SetVarCbAndRefresh(8, 2);
    Event_Wait(5);
    Event_ShowMessageThenWait(8, 0, 5);
    Actor_ArmCallbackAndRefresh(11, FACING_WEST);
    Event_Wait(10);
    Func_080d47b4(11, 0x108, 30);
    Event_ShowMessageThenWait(11, 0, 5);
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 452) += 2;
  } else {
    Event_Wait(35);
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 452) += 2;
    Event_ShowMessageThenWait(8, 0, 5);
    Actor_ArmCallbackAndRefresh(11, FACING_WEST);
    Event_Wait(5);
    Func_080d47b4(11, 0x107, 30);
    Event_ShowMessageThenWait(11, 0, 5);
    Actor_Jump(11, 6, 15);
    Actor_Jump(11, 6, 23);
  }
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 4, FACING_SOUTHEAST);
  Event_Wait(15);
  Func_080d47b4(14, 0x109, 30);
  Event_ShowMessageThenWait(14, 0, 5);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 14, 0xb000);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(14, 2);
  Event_Wait(5);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200e, 0, 5);
  Func_02003544_(8, 0x102, 30);
  Event_ShowMessageThenWait(8, 0, 5);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 11, 0xb000);
  Event_Wait(10);
  Event_ShowMessageThenWait(11, 0, 5);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 14, 0xd000);
  Event_Wait(5);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_ArmCallbackAndRefresh(14, FACING_EAST);
  Event_Wait(5);
  Event_ShowMessageThenWait(14, 0, 5);
  Func_02003544_(11, 0x105, 60);
  Func_080d47b4(14, 0x101, 30);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_StartRepeatedMotion(11, 3);
  Func_080d489c(11, 0x102);
  Event_Wait(40);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 11, FACING_WEST);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(11, 0, 5);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_Wait(10);
  Actor_ArmCallbackAndRefresh(14, 0xd000);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Func_080d47b4(8, 0x105, 50);
  Event_ShowMessageThenWait(8, 0, 5);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 14, 0xb000);
  Event_Wait(10);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 0x200e, 0);
  Call3((void (*)())Actor_ArmCallback, 8, 0x5000, 0);
  Call3((void (*)())Actor_ArmCallback, 11, FACING_WEST, 0);
  Call3((void (*)())Actor_ArmCallback, 14, 0xb000, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTH, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTHEAST, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 15, 0x5000);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(15);
    Motion_SetModeAndWaitAnimation(15, 3);
    Event_Wait(10);
    Event_ShowMessageThenWait(0x200f, 0, 5);
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 452)++;
  } else {
    Event_Wait(35);
    Motion_SetModeAndWaitAnimation(15, 3);
    Event_Wait(10);
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 452)++;
    Event_ShowMessageThenWait(0x200f, 0, 5);
  }
  Call3((void (*)())Actor_ArmCallback, 8, 0x5000, 0);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 15, 0x3000, 0);
  Actor_ArmCallbackAndRefresh(14, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 10);
  Actor_StartRepeatedMotion(11, 3);
  Call2((void (*)())Func_080d489c, 11, 0x102);
  Event_Wait(50);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(11, 0, 5);
  Func_02003544_(14, 0x101, 30);
  Event_ShowMessageThenWait(14, 0, 5);
  Func_080d47b4(11, 0x105, 30);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 11, 0x5000);
  Event_Wait(10);
  Event_ShowMessageThenWait(11, 0, 15);
  Func_080d47b4(14, 0x106, 30);
  Event_ShowMessageThenWait(14, 0, 5);
  Func_02003544_(11, 0x101, 35);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 11, FACING_WEST);
  Event_Wait(5);
  Event_ShowMessageThenWait(11, 0, 5);
  Motion_SetModeAndWaitAnimation(14, 4);
  Event_ShowMessageThenWait(14, 0, 5);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 11, 0xb000);
  Event_Wait(10);
  Event_ShowMessageThenWait(11, 0, 5);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_ShowMessageThenWait(14, 0, 5);
  Call3((void (*)())Actor_CommitPositionAndActivate, 14, -8, 0);
  Call3((void (*)())Actor_ArmCallback, 11, FACING_WEST, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 14, -8, 0);
  Func_080d47b4(11, 0x102, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 14, -56, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 14, 0xb000);
  Func_02003544_(14, 0x101, 30);
  Actor_ArmCallbackAndRefresh(14, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Motion_SetVarCbAndRefresh(11, 2);
  Event_Wait(5);
  Actor_Jump(11, 6, 20);
  Actor_WatchAndSetScript(4, 11);
  Actor_WatchAndSetScript(7, 11);
  Actor_WatchAndSetScript(5, 11);
  Actor_WatchAndSetScript(6, 11);
  Actor_WatchAndSetScript(15, 11);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x20000, 0x10000);
  Actor_CommitPositionAndActivate(11, -80, 0);
  Event_Wait(10);
  Actor_SetSpritePriority(14, 3);
  Actor_SetSpritePriority(11, 3);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x10000, 0x8000);
  Call3((void (*)())Actor_OffsetPositionAndReset, 14, 0, -64);
  Call3((void (*)())Actor_CommitPositionAndActivate, 11, -16, 0);
  Actor_OffsetPositionAndReset(11, 0, -64);
  Event_Wait(15);
  Actor_SetSpritePriority(14, 2);
  Actor_SetSpritePriority(11, 2);
  Actor_WaitForMoveById(14);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Actor_WaitForMoveById(11);
  Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  Actor_ArmCallback(4, FACING_NORTHWEST, 0);
  Event_Wait(10);
  Event_Wait(60);
  Actor_CommitPositionAndActivate(8, 0, 16);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 8, FACING_WEST);
  Event_ShowMessageThenWait(8, 0, 5);
  Func_02003544_(4, 0x101, 0);
  Func_02003544_(7, 0x101, 0);
  Func_02003544_(5, 0x101, 0);
  Func_02003544_(6, 0x101, 0);
  Func_080d47b4(15, 0x101, 30);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_ArmCallbackAndRefresh(15, FACING_EAST);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTH, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTH, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 15, 0x5000);
  Event_Wait(40);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_ArmCallbackAndRefresh(15, FACING_EAST);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(8, 4);
  Event_ShowMessageThenWait(8, 0, 5);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0xb333, 0x5999);
  Actor_SetMovementTypeById(8, gData_020041e8);
  Event_Wait(30);
  Actor_Stop(8);
  Call3((void (*)())Actor_SetPositionAndReset, 8, 648, 188);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 8, 0x5000);
  Event_Wait(10);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTH, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTHEAST, 0);
  Actor_ArmCallbackAndRefresh(15, 0x5000);
  Event_Wait(20);
  Actor_SetSpritePriority(4, 2);
  Actor_GetById(4)->priorityFlags |= 1;
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0x13333, 0x9999);
  Actor_SetModeById(5, 2);
  party = (u8 *)&gMainRoutineWork;
  owner = (s32 *)(party + 532);
  leader = Actor_GetById(*owner);
  if (leader != 0)
    Actor_ResetAndSetPosition(5, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(5);
  Actor_SetHorizontalPositionWithTerrain(5, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 6, 0x13333, 0x9999);
  Actor_SetModeById(6, 2);
  leader = Actor_GetById(*owner);
  if (leader != 0)
    Actor_ResetAndSetPosition(6, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(6);
  Actor_SetHorizontalPositionWithTerrain(6, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 7, 0x13333, 0x9999);
  Actor_SetModeById(7, 2);
  leader = Actor_GetById(*owner);
  if (leader != 0)
    Actor_ResetAndSetPosition(7, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(7);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 15, 0x13333, 0x9999);
  Actor_SetModeById(15, 2);
  leader = Actor_GetById(*owner);
  if (leader != 0)
    Actor_ResetAndSetPosition(15, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(15);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  GameFlag_SetBit(0x97e);
  Func_02003464();
}

#include "system/dma.h"
#include "field.h"

extern const u16 Field_Palette685[];
extern char ResourceId_FieldEffectExplosion;

struct FlashSprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 x;
  s32 z;
  s32 scaleX;
  s32 scaleY;
  u8 unk1c[2];
  u16 unk1e;
};

struct FlashWork {
  u8 tiles[0x400];
  struct FlashSprite sprite;
};

struct FlashWork *HeapSlot_AllocFar(s32 slot, s32 size);
void *Resource_GetTableEntry(s32 resourceId);
void Func_020033dc(void *entry, void *destination);
s32 Resource_FindFreeEntry(void);
void Func_020035fc(struct FlashSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);
void Func_020035c4(s32 *position);
void Func_02003604(struct FlashSprite *sprite);
s32 Resource_ResetEntry(u32 slot);
void HeapSlot_Release(s32 slot);

static __inline__ void Place(struct FlashSprite *a, s32 b, s32 c, u32 d,
                             s32 e) {
  /* FAKEMATCH(wrapper): forwarded inline; as a direct call, 4 of 181
   * instructions differ in tla-en. */
  Func_020035fc(a, b, c, d, e);
}

void Func_020029f8(s32 id, s32 mode) {
  struct FlashWork *work = HeapSlot_AllocFar(220, 0x428);
  struct Actor *object = Actor_GetById(id);
  s32 slot;
  s32 tile;
  struct FlashSprite *sprite;
  s32 position[3];
  s32 i;
  s32 z;

  if (mode == 0) {
    for (i = 15; i >= 0; i--) {
      s32 angle = object->facing;
      angle += 0x2000;
      object->facing = angle;
      wait(1);
    }
  }
  wait(2);
  object->scaleX = 0;
  Actor_SetSingleChildFlags(object, 0);
  Dma_Set(Field_Palette685, (void *)0x050003e0, 0x80000010,
          (volatile u32 *)0x040000d4);
  Func_020033dc(Resource_GetTableEntry((s32)&ResourceId_FieldEffectExplosion),
                work);
  slot = Resource_FindFreeEntry();
  tile = Vram_Load(slot, 0x400, work);
  sprite = &work->sprite;
  Place(sprite, 8, 16, 0x40000000, tile);
  sprite->unk1e = 240;
  sprite->priority = 1;
  sprite->fullColor = 0;
  sprite->palette = 15;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  Func_020035c4(position);
  sprite->x = position[0];
  sprite->z = z = position[2];
  if (mode == 1) {
    sprite->scaleX = 0x20000;
    sprite->scaleY = 0x20000;
    sprite->z = z + 0xa0000;
  }
  Sound_Play(145);
  for (i = 0; i <= 71; i++) {
    sprite = &work->sprite;
    switch (i) {
    case 6:
      sprite->tile = tile + 4;
      break;
    case 12:
      sprite->tile = tile + 8;
      break;
    case 22:
      sprite->tile = tile + 12;
      break;
    case 32:
      sprite->tile = tile + 16;
      break;
    case 42:
      sprite->tile = tile + 20;
      break;
    case 52:
      sprite->tile = tile + 24;
      break;
    case 62:
      sprite->tile = tile + 28;
      break;
    }
    Func_02003604(sprite);
    wait(1);
  }
  Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
  Resource_ResetEntry(slot);
  HeapSlot_Release(220);
}
