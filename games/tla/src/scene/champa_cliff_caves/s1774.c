#include "scene_tables.h"
#include "scene.h"
#include "call.h"

struct EventTransition {
  u8 unk000[0x1ac];
  s32 type;
  u8 unk1b0[4];
  s32 frames;
};

void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Camera_FollowActor(s32 actor, s32 target);
void Event_StartScreenTransition(void);
void Func_020034d4(s32 actor, s32 x, s32 z);
s32 Func_02003544(s32 actor, s32 value, s32 frames);
void Func_0200351c(s32 actor, s32 value, s32 frames);
void Func_0200356c(s32 scene, s32 entrance);

static __inline__ void Func_020034d4_(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020034d4(actor, x, z);
}

static __inline__ void Actor_ArmCallback_(s32 actor, s32 angle,
                                                 s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ArmCallback(actor, angle, frames);
}

void Func_02001774(void) {
  struct EventTransition *work;

  Func_0200345c();
  Func_020035b4(0);
  Func_0200350c((s32)MsgHaveYouLearned);
  Func_020034d4_(gMainRoutineWork.currentOwner, 568 << 16, 248 << 16);
  Func_020034d4_(8, 584 << 16, 224 << 16);
  Func_020034d4_(13, 600 << 16, 240 << 16);
  Func_020034d4_(11, 584 << 16, 264 << 16);
  Actor_ArmCallback_(8, 0x5000, 0);
  Actor_ArmCallback_(11, 0xb000, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 41);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(10);
  Func_02003544(11, 264, 30);
  Func_0200351c(11, 0, 5);
  Motion_SetModeAndWaitAnimation(8, 4);
  Func_0200351c(8, 0, 20);
  work = Ram_HeapSlots->eventWork;
  work->frames = 40;
  work->type = 521;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_0200356c((s16)gMainRoutineWork.savedScene, (s16)gMainRoutineWork.savedEntrance);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
extern const u8 gData_020041e8[];
extern const u8 gData_02004254[];
extern const u8 gData_0200432c[];
extern const u8 gData_020042d4[];

void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 follow);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void Func_020029f8(s32 id, s32 mode);
void Actor_CommitPositionAndActivate(s32 id, s32 dx, s32 dz);
void Actor_Jump(s32 id, s32 speed, s32 frames);
void Actor_StartRepeatedMotion(s32 id, s32 variant);
void Func_0200354c(s32 id, s32 animation);
void Func_020035cc(s32 id, s32 dx, s32 dz, s32 frames);
void Actor_WaitForMoveById(s32 id);
void Actor_SetMovementTypeById(s32 id, const u8 *script);
void Actor_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void Func_02003564(void);
void Actor_WatchAndSetScript(s32 id, s32 linkedId);
void Actor_OffsetPositionAndReset(s32 id, s32 dx, s32 dz);
void Actor_Stop(s32 id);
void Actor_ResetAndSetPosition(s32 id, s32 x, s32 z);

static __inline__ void Func_02003544_(s32 actor, s32 value, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003544(actor, value, frames);
}

/// The scene that opens with MsgMySalamanderYou: persons 8 and 11 talk and
/// move with the party through three yes-or-no answers (each counted in the
/// event work), person 14 is brought in and the others follow it, then
/// persons 5, 6, 7 and 15 join the leader and flag 0x97e is set.
void Func_02001868(void) {
  s32 scriptA, scriptB, scriptC, scriptD;
  s32 *owner;
  u8 *party;
  struct FieldActor *leader;

  Func_0200345c();
  Call1((void (*)())Func_020035b4, 0);
  Func_0200350c((s32)MsgMySalamanderYou);
  owner = (s32 *)&gMainRoutineWork;
  owner = (s32 *)((u8 *)owner + 532);
  Call3((void (*)())Func_020034d4, *owner, PIXELS(568), PIXELS(256));
  Call3((void (*)())Func_020034d4, 13, PIXELS(592), PIXELS(240));
  Call2((void (*)())Actor_SetModeById, 13, 3);
  Call3((void (*)())Actor_ArmCallback, *owner, FACING_EAST, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(656), -1, PIXELS(248),
        0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 13, 2);
  Call1((void (*)())FieldEvent_Wait, 5);
  Call2((void (*)())Func_020029f8, 13, 1);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x10000, 0x8000);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -12, -18);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -10, -14);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -18, -12);
  Call3((void (*)())Actor_SetPositionAndReset, 8, 664, 200);
  Call2((void (*)())Func_02003534, 8, 0x5000);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -16, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -16, 8);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -16, 16);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -8, 16);
  Call3((void (*)())Actor_SetPositionAndReset, 8, 608, 248);
  Call2((void (*)())Func_02003534, 8, FACING_WEST);
  Call1((void (*)())FieldEvent_Wait, 5);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 8, 4);
  Call3((void (*)())Func_0200351c, 0x2008, 0, 5);
  Func_02003544_(11, 0x102, 40);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x1b333, 0xd999);
  Call3((void (*)())Actor_CommitPositionAndActivate, 11, -16, 0);
  Call1((void (*)())FieldEvent_Wait, 5);
  Call3((void (*)())Func_0200351c, 11, 0, 5);
  Call2((void (*)())Func_02003534, 8, FACING_EAST);
  Call1((void (*)())FieldEvent_Wait, 5);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call3((void (*)())Actor_Jump, 11, 6, 15);
  Call3((void (*)())Actor_Jump, 11, 6, 23);
  Call3((void (*)())Func_0200351c, 11, 0, 5);
  Func_02003544_(8, 0x107, 30);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call2((void (*)())Actor_StartRepeatedMotion, 11, 3);
  Call2((void (*)())Func_0200354c, 11, 0x102);
  Call1((void (*)())FieldEvent_Wait, 38);
  Call3((void (*)())Func_0200351c, 11, 0, 5);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 8, 4);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call2((void (*)())Actor_StartRepeatedMotion, 11, 3);
  Call2((void (*)())Func_0200354c, 11, 0x102);
  Call1((void (*)())FieldEvent_Wait, 37);
  Call3((void (*)())Func_0200351c, 11, 0, 5);
  Call2((void (*)())Func_02003534, 8, 0x7ff6);
  Call1((void (*)())FieldEvent_Wait, 25);
  Call2((void (*)())Func_02003534, 8, FACING_EAST);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 11, 2);
  Call1((void (*)())FieldEvent_Wait, 30);
  Func_02003544_(11, 0x106, 30);
  Call3((void (*)())Func_0200351c, 11, 0, 10);
  Func_02003544_(8, 0x102, 40);
  Call3((void (*)())Func_0200351c, 8, 0, 0);
  Call3((void (*)())Func_0200351c, 15, 0, 5);
  Call2((void (*)())Func_02003534, 8, 0x7ff6);
  Call1((void (*)())FieldEvent_Wait, 10);
  Func_02003544_(8, 0x101, 40);
  Call4((void (*)())Func_020035cc, 7, 0, -16, 0);
  Call4((void (*)())Func_020035cc, 5, -16, 0, 0);
  Call4((void (*)())Func_020035cc, 6, -16, -16, 0);
  Call4((void (*)())Func_020035cc, 15, 16, -16, 0);
  Call1((void (*)())Actor_WaitForMoveById, 15);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(640), -1, PIXELS(248),
        1);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 6, 4);
  Call3((void (*)())Func_0200351c, 0x1006, 0, 5);
  Call2((void (*)())Actor_StartRepeatedMotion, 8, 3);
  Call2((void (*)())Func_0200354c, 8, 0x102);
  Call2((void (*)())Actor_StartRepeatedMotion, 11, 3);
  Call2((void (*)())Func_0200354c, 11, 0x102);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call3((void (*)())Func_0200351c, 0x2008, 0, 5);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200351c, 0x1005, 0, 5);
  Call2((void (*)())Func_02003534, 8, FACING_EAST);
  Call1((void (*)())FieldEvent_Wait, 5);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call2((void (*)())Actor_StartRepeatedMotion, 11, 3);
  Call2((void (*)())Func_0200354c, 11, 0x102);
  Call1((void (*)())FieldEvent_Wait, 35);
  Call3((void (*)())Func_0200351c, 11, 0, 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
  Call3((void (*)())Func_0200351c, 7, 0, 5);
  Call2((void (*)())Func_02003534, 15, 0x5000);
  Call1((void (*)())FieldEvent_Wait, 5);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 0x200f, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Call1((void (*)())FieldEvent_Wait, 20);
    Call2((void (*)())Func_02003534, 6, FACING_SOUTHEAST);
    Call1((void (*)())FieldEvent_Wait, 5);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 6, 3);
    Call1((void (*)())FieldEvent_Wait, 10);
    Call3((void (*)())Func_0200351c, 0x1006, 0, 5);
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 452)++;
  } else {
    Call1((void (*)())FieldEvent_Wait, 35);
    Call2((void (*)())Func_02003534, 6, FACING_SOUTHEAST);
    Call1((void (*)())FieldEvent_Wait, 5);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 6, 4);
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 452)++;
    Call3((void (*)())Func_0200351c, 0x1006, 0, 5);
  }
  Call3((void (*)())Actor_ArmCallback, 15, FACING_EAST, 0);
  Call2((void (*)())Func_02003534, 6, FACING_EAST);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x2cccc, 0x16666);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x36666, 0x1b333);
  Call2((void (*)())Actor_StartRepeatedMotion, 11, 3);
  Call2((void (*)())Func_0200354c, 11, 0x102);
  scriptA = (s32)gData_020041e8;
  Call2((void (*)())Actor_SetMovementTypeById, 8, scriptA);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ResetAndSetPositionInMode2, 11, 664, 290);
  Call1((void (*)())Actor_RefreshSelectorById, 8);
  scriptB = (s32)gData_02004254;
  Call2((void (*)())Actor_SetMovementTypeById, 8, scriptB);
  Call1((void (*)())Actor_WaitForMoveById, 11);
  scriptC = (s32)gData_0200432c;
  Call2((void (*)())Actor_SetMovementTypeById, 11, scriptC);
  Call1((void (*)())Actor_RefreshSelectorById, 8);
  scriptD = (s32)gData_020042d4;
  Call2((void (*)())Actor_SetMovementTypeById, 8, scriptD);
  Call1((void (*)())Actor_RefreshSelectorById, 11);
  Call2((void (*)())Actor_SetMovementTypeById, 11, scriptA);
  Call1((void (*)())Actor_RefreshSelectorById, 8);
  Call3((void (*)())Actor_ArmCallback, 8, 0xb000, 0);
  Call1((void (*)())Actor_RefreshSelectorById, 11);
  Call3((void (*)())Actor_ArmCallback, 11, 0x3000, 0);
  Call3((void (*)())Func_0200351c, 11, 0, 5);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 8, 4);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Func_02003544_(11, 0x102, 30);
  Call3((void (*)())Func_0200351c, 11, 0, 5);
  Func_02003544_(8, 0x103, 30);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call2((void (*)())Actor_StartRepeatedMotion, 11, 3);
  Call2((void (*)())Func_0200354c, 11, 0x102);
  Call2((void (*)())Actor_SetMovementTypeById, 8, scriptC);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call1((void (*)())Actor_WaitForMoveById, 11);
  Call2((void (*)())Actor_SetMovementTypeById, 11, scriptB);
  Call1((void (*)())Actor_RefreshSelectorById, 8);
  Call2((void (*)())Actor_SetMovementTypeById, 8, scriptA);
  Call1((void (*)())Actor_RefreshSelectorById, 11);
  Call2((void (*)())Actor_SetMovementTypeById, 11, scriptD);
  Call1((void (*)())Actor_RefreshSelectorById, 8);
  Call3((void (*)())Actor_ArmCallback, 8, 0x3000, 0);
  Call1((void (*)())Actor_RefreshSelectorById, 11);
  Call3((void (*)())Actor_ArmCallback, 11, 0xb000, 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 11, 4);
  Call3((void (*)())Func_0200351c, 11, 0, 5);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 8, 4);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call3((void (*)())Func_0200351c, 14, 0, 5);
  Call2((void (*)())Actor_SetSpritePriority, 4, 3);
  Call2((void (*)())Actor_SetSpritePriority, 5, 3);
  Call2((void (*)())Actor_SetSpritePriority, 6, 3);
  Call2((void (*)())Actor_SetSpritePriority, 7, 3);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_NORTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_NORTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_NORTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 15, FACING_WEST, 0);
  Call3((void (*)())Actor_ArmCallback, 11, FACING_WEST, 0);
  Call2((void (*)())Func_02003534, 8, FACING_WEST);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(568), -1, PIXELS(248),
        1);
  Func_02003564();
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Actor_SetSpritePriority, 14, 2);
  Actor_GetById(14)->facing = 0x3000;
  Actor_GetById(14)->motionFlags = 0;
  Call3((void (*)())Func_020034d4, 14, PIXELS(520), PIXELS(200));
  Call3((void (*)())Actor_SetSpeedParameters, 14, 0x10000, 0x8000);
  Call3((void (*)())Actor_CommitPositionAndActivate, 14, 0, 16);
  Call3((void (*)())Actor_ArmCallback, 14, 0x3000, 0);
  Call3((void (*)())Func_0200351c, 11, 0, 5);
  Call2((void (*)())Actor_WatchAndSetScript, 4, 14);
  Call2((void (*)())Actor_WatchAndSetScript, 7, 14);
  Call2((void (*)())Actor_WatchAndSetScript, 5, 14);
  Call2((void (*)())Actor_WatchAndSetScript, 6, 14);
  Call2((void (*)())Actor_WatchAndSetScript, 15, 14);
  Call2((void (*)())Actor_WatchAndSetScript, 8, 14);
  Call3((void (*)())Actor_CommitPositionAndActivate, 14, 0, 16);
  Call2((void (*)())Actor_SetSpritePriority, 14, 3);
  Call3((void (*)())Actor_CommitPositionAndActivate, 14, 0, 32);
  Call3((void (*)())Actor_CommitPositionAndActivate, 14, 8, 8);
  Call3((void (*)())Actor_CommitPositionAndActivate, 14, 64, 0);
  Call3((void (*)())Actor_ArmCallback, 8, 0x5000, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(600), -1, PIXELS(256),
        1);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x10000, 0x8000);
  Call3((void (*)())Actor_CommitPositionAndActivate, 11, -32, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 11, -16, -16);
  Call2((void (*)())Func_02003534, 11, FACING_WEST);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 14, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 11, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200351c, 11, 0, 5);
  Call2((void (*)())Func_02003534, 14, 0xb000);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200351c, 0x200e, 0, 5);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x10000, 0x8000);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -16, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -16, 8);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -16, 16);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -8, 8);
  Call2((void (*)())Func_02003534, 8, 0x5000);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call3((void (*)())Actor_ArmCallback, 11, 0xb000, 0);
  Call2((void (*)())Func_02003534, 14, 0xd000);
  Call1((void (*)())FieldEvent_Wait, 15);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 8, 4);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 14, 4);
  Call3((void (*)())Func_0200351c, 14, 0, 5);
  Func_02003544_(8, 0x109, 30);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 11, 4);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 11, 0);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTHEAST, 0);
  Call2((void (*)())Func_02003534, 8, 0x5000);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Call1((void (*)())FieldEvent_Wait, 20);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
    Call1((void (*)())FieldEvent_Wait, 5);
    Call3((void (*)())Func_0200351c, 8, 0, 5);
    Call2((void (*)())Func_02003534, 11, FACING_WEST);
    Call1((void (*)())FieldEvent_Wait, 10);
    Func_02003544_(11, 0x108, 30);
    Call3((void (*)())Func_0200351c, 11, 0, 5);
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 452) += 2;
  } else {
    Call1((void (*)())FieldEvent_Wait, 35);
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 452) += 2;
    Call3((void (*)())Func_0200351c, 8, 0, 5);
    Call2((void (*)())Func_02003534, 11, FACING_WEST);
    Call1((void (*)())FieldEvent_Wait, 5);
    Func_02003544_(11, 0x107, 30);
    Call3((void (*)())Func_0200351c, 11, 0, 5);
    Call3((void (*)())Actor_Jump, 11, 6, 15);
    Call3((void (*)())Actor_Jump, 11, 6, 23);
  }
  Call2((void (*)())Func_02003534, 4, FACING_SOUTHEAST);
  Call1((void (*)())FieldEvent_Wait, 15);
  Func_02003544_(14, 0x109, 30);
  Call3((void (*)())Func_0200351c, 14, 0, 5);
  Call2((void (*)())Func_02003534, 14, 0xb000);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 14, 2);
  Call1((void (*)())FieldEvent_Wait, 5);
  Call3((void (*)())Func_0200351c, 0x200e, 0, 5);
  Func_02003544_(8, 0x102, 30);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call2((void (*)())Func_02003534, 11, 0xb000);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200351c, 11, 0, 5);
  Call2((void (*)())Func_02003534, 14, 0xd000);
  Call1((void (*)())FieldEvent_Wait, 5);
  Call3((void (*)())Func_0200351c, 14, 0, 5);
  Call2((void (*)())Func_02003534, 14, FACING_EAST);
  Call1((void (*)())FieldEvent_Wait, 5);
  Call3((void (*)())Func_0200351c, 14, 0, 5);
  Func_02003544_(11, 0x105, 60);
  Func_02003544_(14, 0x101, 30);
  Call3((void (*)())Func_0200351c, 14, 0, 5);
  Call2((void (*)())Actor_StartRepeatedMotion, 11, 3);
  Call2((void (*)())Func_0200354c, 11, 0x102);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call2((void (*)())Func_02003534, 11, FACING_WEST);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 11, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200351c, 11, 0, 5);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 14, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Func_02003534, 14, 0xd000);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200351c, 14, 0, 5);
  Func_02003544_(8, 0x105, 50);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call2((void (*)())Func_02003534, 14, 0xb000);
  Call1((void (*)())FieldEvent_Wait, 10);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 0x200e, 0);
  Call3((void (*)())Actor_ArmCallback, 8, 0x5000, 0);
  Call3((void (*)())Actor_ArmCallback, 11, FACING_WEST, 0);
  Call3((void (*)())Actor_ArmCallback, 14, 0xb000, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTH, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTHEAST, 0);
  Call2((void (*)())Func_02003534, 15, 0x5000);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Call1((void (*)())FieldEvent_Wait, 15);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 15, 3);
    Call1((void (*)())FieldEvent_Wait, 10);
    Call3((void (*)())Func_0200351c, 0x200f, 0, 5);
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 452)++;
  } else {
    Call1((void (*)())FieldEvent_Wait, 35);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 15, 3);
    Call1((void (*)())FieldEvent_Wait, 10);
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 452)++;
    Call3((void (*)())Func_0200351c, 0x200f, 0, 5);
  }
  Call3((void (*)())Actor_ArmCallback, 8, 0x5000, 0);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 15, 0x3000, 0);
  Call2((void (*)())Func_02003534, 14, FACING_EAST);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200351c, 14, 0, 10);
  Call2((void (*)())Actor_StartRepeatedMotion, 11, 3);
  Call2((void (*)())Func_0200354c, 11, 0x102);
  Call1((void (*)())FieldEvent_Wait, 50);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 11, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200351c, 11, 0, 5);
  Func_02003544_(14, 0x101, 30);
  Call3((void (*)())Func_0200351c, 14, 0, 5);
  Func_02003544_(11, 0x105, 30);
  Call2((void (*)())Func_02003534, 11, 0x5000);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200351c, 11, 0, 15);
  Func_02003544_(14, 0x106, 30);
  Call3((void (*)())Func_0200351c, 14, 0, 5);
  Func_02003544_(11, 0x101, 35);
  Call2((void (*)())Func_02003534, 11, FACING_WEST);
  Call1((void (*)())FieldEvent_Wait, 5);
  Call3((void (*)())Func_0200351c, 11, 0, 5);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 14, 4);
  Call3((void (*)())Func_0200351c, 14, 0, 5);
  Call2((void (*)())Func_02003534, 11, 0xb000);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200351c, 11, 0, 5);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 14, 3);
  Call3((void (*)())Func_0200351c, 14, 0, 5);
  Call3((void (*)())Actor_CommitPositionAndActivate, 14, -8, 0);
  Call3((void (*)())Actor_ArmCallback, 11, FACING_WEST, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 14, -8, 0);
  Func_02003544_(11, 0x102, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 14, -56, 0);
  Call2((void (*)())Func_02003534, 14, 0xb000);
  Func_02003544_(14, 0x101, 30);
  Call2((void (*)())Func_02003534, 14, FACING_EAST);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200351c, 14, 0, 5);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 11, 2);
  Call1((void (*)())FieldEvent_Wait, 5);
  Call3((void (*)())Actor_Jump, 11, 6, 20);
  Call2((void (*)())Actor_WatchAndSetScript, 4, 11);
  Call2((void (*)())Actor_WatchAndSetScript, 7, 11);
  Call2((void (*)())Actor_WatchAndSetScript, 5, 11);
  Call2((void (*)())Actor_WatchAndSetScript, 6, 11);
  Call2((void (*)())Actor_WatchAndSetScript, 15, 11);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x20000, 0x10000);
  Call3((void (*)())Actor_CommitPositionAndActivate, 11, -80, 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Actor_SetSpritePriority, 14, 3);
  Call2((void (*)())Actor_SetSpritePriority, 11, 3);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x10000, 0x8000);
  Call3((void (*)())Actor_OffsetPositionAndReset, 14, 0, -64);
  Call3((void (*)())Actor_CommitPositionAndActivate, 11, -16, 0);
  Call3((void (*)())Actor_OffsetPositionAndReset, 11, 0, -64);
  Call1((void (*)())FieldEvent_Wait, 15);
  Call2((void (*)())Actor_SetSpritePriority, 14, 2);
  Call2((void (*)())Actor_SetSpritePriority, 11, 2);
  Call1((void (*)())Actor_WaitForMoveById, 14);
  Call3((void (*)())Func_020034d4, 14, 0, 0);
  Call1((void (*)())Actor_WaitForMoveById, 11);
  Call3((void (*)())Func_020034d4, 11, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTHWEST, 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call1((void (*)())FieldEvent_Wait, 60);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, 0, 16);
  Call2((void (*)())Func_02003534, 8, FACING_WEST);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Func_02003544_(4, 0x101, 0);
  Func_02003544_(7, 0x101, 0);
  Func_02003544_(5, 0x101, 0);
  Func_02003544_(6, 0x101, 0);
  Func_02003544_(15, 0x101, 30);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_EAST, 0);
  Call2((void (*)())Func_02003534, 15, FACING_EAST);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 8, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200351c, 8, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTH, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTH, 0);
  Call2((void (*)())Func_02003534, 15, 0x5000);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_EAST, 0);
  Call2((void (*)())Func_02003534, 15, FACING_EAST);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 8, 4);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0xb333, 0x5999);
  Call2((void (*)())Actor_SetMovementTypeById, 8,
        (s32)gData_020041e8);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call1((void (*)())Actor_Stop, 8);
  Call3((void (*)())Actor_SetPositionAndReset, 8, 648, 188);
  Call2((void (*)())Func_02003534, 8, 0x5000);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTH, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTHEAST, 0);
  Call2((void (*)())Func_02003534, 15, 0x5000);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Actor_SetSpritePriority, 4, 2);
  Actor_GetById(4)->priorityFlags |= 1;
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 5, 2);
  party = (u8 *)&gMainRoutineWork;
  owner = (s32 *)(party + 532);
  leader = Actor_GetById(*owner);
  if (leader != 0)
    Call3((void (*)())Actor_ResetAndSetPosition, 5,
          ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Call1((void (*)())Actor_WaitForMoveById, 5);
  Call3((void (*)())Func_020034d4, 5, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 6, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 6, 2);
  leader = Actor_GetById(*owner);
  if (leader != 0)
    Call3((void (*)())Actor_ResetAndSetPosition, 6,
          ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Call1((void (*)())Actor_WaitForMoveById, 6);
  Call3((void (*)())Func_020034d4, 6, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 7, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 7, 2);
  leader = Actor_GetById(*owner);
  if (leader != 0)
    Call3((void (*)())Actor_ResetAndSetPosition, 7,
          ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Call1((void (*)())Actor_WaitForMoveById, 7);
  Call3((void (*)())Func_020034d4, 7, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 15, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 15, 2);
  leader = Actor_GetById(*owner);
  if (leader != 0)
    Call3((void (*)())Actor_ResetAndSetPosition, 15,
          ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Call1((void (*)())Actor_WaitForMoveById, 15);
  Call3((void (*)())Func_020034d4, 15, 0, 0);
  Call1((void (*)())GameFlag_SetBit, 0x97e);
  Func_02003464();
}
