#include "types.h"
#include "version.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "event_inline.h"

#define EVENT_WORK ((struct EventWork *)Ram_HeapSlots->eventWork)

extern const u8 MsgWeDidItAgatio[];
extern u32 gData_02004a6c[];
extern u32 gData_02004a90[];
extern u32 gData_02004ab4[];
extern u32 gData_02004ae8[];
extern u32 gData_02004b1c[];
extern u32 gData_02005548[];
extern u32 gData_020059a4[];
extern u32 gData_02005a00[];

void Func_020047d4(void);
void Party_RemoveActiveOwner(s32 owner);
s32 Party_AddActiveOwner(s32 owner);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020048f4(void);
void PartyInventory_Remove(s32 a);
s32 PartyInventory_Add(s32 item);
void *Unit_Get(s32 owner);
s32 Func_020047a4();
s32 Event_ClearInvalidPackedValues();
void Func_020047dc(void);

#define ACTOR_SET_MODE_AND_WAIT(id, mode, frames)                              \
  do {                                                                         \
    Actor_SetModeById(id, mode);                                               \
    Event_Wait(frames);                                                        \
  } while (0)

// The Japanese form differs and is still on its listing.

static __inline__ void Walk(s32 id, s32 speed, s32 frames) {
  /* FAKEMATCH(wrapper): the walk forwarded inline, so its constants stay out of
   * CSE; as direct calls, 103 of 1328 instructions differ in tla-en. */
  ((struct Actor *)Func_080d47b4(id, speed, frames));
}

void Func_02001ed0(void) {
  s32 index;
  s32 offset;
  s32 answer;

  Func_020047d4();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(0x2e10000, 0, 0xd10000, 0);
  gMainRoutineWork.currentOwner = 4;
  Party_RemoveActiveOwner(0);
  Party_RemoveActiveOwner(2);
  Party_RemoveActiveOwner(5);
  Party_RemoveActiveOwner(6);
  Party_RemoveActiveOwner(7);
  Party_RemoveActiveOwner(3);
  Party_RemoveActiveOwner(1);
  Party_AddActiveOwner(4);
  Actor_SetSpeed(4, 0x10000, 0x8000);
  {
    s32 angle = 0x4000;

    Actor_SetPositionAndFacing(4, 0x2980000, 0x580000, angle);
  }
  {
    s32 angle = 0x2000;

    Actor_SetPositionAndFacing(0x1b, 0x2cd0000, 0xd00000, angle);
  }
  Actor_SetPositionAndFacing(0x1a, 0x2e10000, 0xc30000, 0x2000);
  {
    s32 angle = 0xa000;

    Actor_SetPositionAndFacing(0, 0x2f90000, 0xdb0000, angle);
  }
  Actor_SetPositionAndFacing(2, 0x2e50000, 0xe80000, 0xa000);
  Actor_SetMovementTypeById(3, (s32)gData_02005548);
  Actor_SetSingleChildFlags(Actor_GetById(0), 0);
  Actor_SetModeById(0, 0x27);
  Actor_SetSingleChildFlags(Actor_GetById(2), 0);
  Actor_SetModeById(2, 0x20);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId((s32)MsgWeDidItAgatio);
  Event_ShowMessageThenWait(0x1b, 0, 5);
  Actor_SetModeById(0x1a, 8);
  Event_Wait(0x28);
  Event_ShowMessageThenWait(0x1a, 0, 5);
  Actor_ArmCallbackAndRefresh(0x1b, 0);
  Event_Wait(0xa);
  Event_ShowMessageThenWait(0x1b, 0, 5);
  Actor_StartRepeatedMotion(0, 2);
  Motion_SetVarCbAndRefresh(2, 2);
  Event_Wait(5);
  Walk(0x1a, 0x100, 0x1e);
  Event_ShowMessageThenWait(0x1a, 0, 5);
  Actor_ArmCallbackAndRefresh(0x1b, 0xe000);
  Event_Wait(0xa);
  Event_ShowMessageThenWait(0x1b, 0, 5);
  Actor_SetModeById(0x1a, 1);
  Event_Wait(0x1e);
  Actor_ArmCallbackAndRefresh(0x1a, 0x5000);
  Event_Wait(0x14);
  ACTOR_SET_MODE_AND_WAIT(0x1a, 3, 0x14);
  Event_Wait(0xa);
  Func_080d4568()->motionFlags = 0;
  Camera_FollowActor(4, 1);
  Event_Wait(0x14);
  Func_080d3070(6, 0, 0, 0x4000);
  Camera_FollowActor(6, 1);
  Func_080d3070(7, 0x10, 0, 0x4000);
  FieldActor_LaunchFromLeader(0x1c, -16, 0x18, 0x2000);
  FieldActor_LaunchFromLeader(5, -16, 0, 0x2000);
  Actor_CommitPositionAndActivate(4, 0x10, 0x18);
  Actor_ArmCallback(4, 0x4000, 0);
  ((struct Actor *)Func_080d47b4(0x1c, 0x100, 0x1e));
  Event_ShowMessageThenWait(0x1c, 0, 5);
  Actor_ArmCallbackAndRefresh(0x1c, 0);
  Event_Wait(0xa);
  Actor_FaceDirection(4, 0x8000, 0);
  Event_ShowMessageThenWait(0x1c, 0, 0x23);
  Actor_SetModeById(4, 3);
  ACTOR_SET_MODE_AND_WAIT(0x1c, 3, 0x14);
  Event_Wait(0xa);
  Actor_WalkTo(4, 0x2ab, 0xba);
  Actor_WalkTo(0x1c, 0x29b, 0xca);
  Actor_SetMovementTypeById(4, (s32)gData_02004a6c);
  Actor_SetMovementTypeById(0x1c, (s32)gData_02004a90);
  Actor_FaceDirection(0x1a, 0x8000, 0);
  Actor_FaceDirection(0x1b, 0x8000, 0);
  Walk(6, 0x102, 0x1e);
  Event_ShowMessageThenWait(6, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(5, 4, 0x21);
  Event_ShowMessageThenWait(5, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(7, 3, 0x14);
  Event_Wait(0xa);
  Actor_SetAngleToward(5, 7, 0);
  Actor_SetAngleToward(6, 7, 0);
  Actor_SetAngleToward(7, 5, 0);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(5, 3);
  ACTOR_SET_MODE_AND_WAIT(7, 3, 0x14);
  Event_Wait(0x14);
  Actor_OffsetPositionAndReset(7, 0, 0x28);
  Actor_OffsetPositionAndReset(6, 0, 0x28);
  Actor_OffsetPositionAndReset(5, 0, 0x28);
  Actor_SetMovementTypeById(7, (s32)gData_02004ab4);
  Actor_SetMovementTypeById(6, (s32)gData_02004ae8);
  Actor_SetMovementTypeById(5, (s32)gData_02004b1c);
  Camera_FollowActor(4, 1);
  Func_020048f4();
  Event_Wait(0x14);
  Walk(0x1a, 0x107, 0x1e);
  Event_ShowMessageThenWait(0x1a, 0, 5);
  Walk(0x1b, 0x107, 0x1e);
  Event_ShowMessageThenWait(0x1b, 0, 5);
  Actor_FaceDirectionAndWait(0x1a, 0xc000);
  Event_Wait(0xa);
  Actor_FaceDirectionAndWait(0x1a, 0x8000);
  Event_Wait(0xa);
  Actor_ArmCallbackAndRefresh(0x1a, 0x4000);
  Event_Wait(0xa);
  Actor_FaceDirectionAndWait(0x1a, 0x8000);
  Event_Wait(0xa);
  Actor_ArmCallbackAndRefresh(0x1a, 0xc000);
  Event_Wait(0xa);
  Actor_FaceDirectionAndWait(0x1a, 0x8000);
  Event_ShowMessageThenWait(0x1a, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(0x1b, 4, 0x21);
  Event_ShowMessageThenWait(0x1b, 0, 5);
  Motion_SetVarCbAndRefresh(0x1c, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(0x1c, 0, 5);
  ((struct Actor *)Func_080d47b4(0x1a, 0x103, 0x1e));
  Event_ShowMessageThenWait(0x1a, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(6, 4, 0x21);
  Event_ShowMessageThenWait(6, 0, 5);
  ((struct Actor *)Func_080d47b4(0x1b, 0x107, 0x1e));
  FieldMessage_ShowAtActor(0x1b, 0);
  Actor_SetAngleToward(0x1c, 4, 0);
  Actor_SetAngleToward(5, 4, 0);
  Actor_SetAngleToward(6, 4, 0);
  Actor_SetAngleToward(7, 4, 0);
  Event_Wait(0xa);
  answer = Event_AskYesNoWithActorAnimation(4, 0);
  if (answer == 0) {
    ((struct Actor *)Func_080d47b4(0x1b, 0x102, 0x1e));
    Event_ShowMessageThenWait(0x1b, 0, 5);
    EVENT_WORK->messageId++;
  } else {
    ((struct Actor *)Func_080d47b4(0x1b, 0x102, 0x1e));
    EVENT_WORK->messageId++;
    Event_ShowMessageThenWait(0x1b, 0, 5);
  }
  Actor_SetAngleToward(7, 0x1a, 0);
  ((struct Actor *)Func_080d47b4(7, 0x107, 0x1e));
  Event_ShowMessageThenWait(7, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(0x1a, 4, 0x21);
  Event_ShowMessageThenWait(0x1a, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(0x1b, 3, 0x14);
  Event_Wait(0xa);
  Event_ShowMessageThenWait(0x1b, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(5, 4, 0x21);
  Event_ShowMessageThenWait(5, 0, 5);
  Actor_TurnToFaceEachOther(0x1a, 0x1b, 0);
  Actor_RefreshSelectorById(0x1a);
  Actor_RefreshSelectorById(0x1b);
  Walk(0x1a, 0x105, 0);
  ((struct Actor *)Func_080d47b4(0x1b, 0x105, 0x1e));
  Actor_SetAngleToward(0x1c, 0x1b, 0);
  ACTOR_SET_MODE_AND_WAIT(0x1c, 4, 0x21);
  Actor_SetAngleToward(0x1a, 0x1c, 0);
  Actor_SetAngleToward(0x1b, 0x1c, 0);
  Event_ShowMessageThenWait(0x1c, 0, 5);
  Event_Wait(0x3c);
  ACTOR_SET_MODE_AND_WAIT(0x1b, 3, 0x14);
  Event_Wait(0xa);
  Event_ShowMessageThenWait(0x1b, 0, 5);
  Motion_SetVarCbAndRefresh(0x1a, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(0x1a, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(0x1b, 4, 0x21);
  Event_ShowMessageThenWait(0x1b, 0, 5);
  Walk(0x1a, 0x108, 0x1e);
  Event_ShowMessageThenWait(0x1a, 0, 5);
  Walk(0x1b, 0x108, 0x1e);
  Event_ShowMessageThenWait(0x1b, 0, 5);
  Event_Wait(0x14);
  Motion_SetVarCbAndRefresh(0x1a, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(0x1a, 0, 5);
  Actor_ArmCallbackAndRefresh(0x1a, 0x8000);
  Event_Wait(0xa);
  ACTOR_SET_MODE_AND_WAIT(0x1a, 3, 0x14);
  Event_Wait(0xa);
  Event_ShowMessageThenWait(0x1a, 0, 5);
  Actor_ArmCallbackAndRefresh(0x1b, 0xa000);
  Event_Wait(0xa);
  Event_ShowMessageThenWait(0x1b, 0, 5);
  Actor_SetAngleToward(7, 4, 0);
  Actor_TurnToFaceEachOther(6, 5, 0);
  Actor_TurnToFaceEachOther(4, 0x1c, 0);
  Actor_RefreshSelectorById(4);
  Actor_RefreshSelectorById(0x1c);
  Walk(4, 0x108, 0);
  Walk(6, 0x108, 0);
  Walk(5, 0x108, 0);
  Walk(7, 0x108, 0);
  ((struct Actor *)Func_080d47b4(0x1c, 0x108, 0x28));
  Actor_SetAngleToward(4, 0x1a, 0);
  Actor_SetAngleToward(5, 0x1a, 0);
  Actor_SetAngleToward(6, 0x1a, 0);
  Actor_SetAngleToward(7, 0x1a, 0);
  Actor_SetAngleToward(0x1c, 0x1a, 0);
  ACTOR_SET_MODE_AND_WAIT(0x1a, 3, 0x14);
  Event_Wait(0xa);
  FieldMessage_ShowAtActor(0x1a, 0);
  answer = Event_AskYesNoWithActorAnimation(4, 0);
  if (answer == 0) {
    ACTOR_SET_MODE_AND_WAIT(0x1a, 4, 0x21);
    Event_ShowMessageThenWait(0x1a, 0, 5);
    EVENT_WORK->messageId++;
  } else {
    ACTOR_SET_MODE_AND_WAIT(0x1a, 4, 0x21);
    EVENT_WORK->messageId++;
    Event_ShowMessageThenWait(0x1a, 0, 5);
  }
  Actor_TurnToFaceEachOther(0x1c, 4, 0);
  Actor_RefreshSelectorById(0x1c);
  Event_Wait(0xa);
  ACTOR_SET_MODE_AND_WAIT(0x1c, 3, 0x14);
  Event_Wait(0xa);
  Event_ShowMessageThenWait(0x1c, 0, 5);
  Actor_TurnToFaceEachOther(4, 0x1c, 0);
  Actor_RefreshSelectorById(4);
  Event_Wait(0xa);
  ((struct Actor *)Func_080d47b4(4, 0x102, 0x1e));
  ACTOR_SET_MODE_AND_WAIT(0x1c, 3, 0x14);
  Event_Wait(0x28);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 0x14);
  Event_Wait(0x1e);
  Actor_TurnToFaceEachOther(0x1a, 0x1b, 0);
  Actor_RefreshSelectorById(0x1a);
  Actor_RefreshSelectorById(0x1b);
  Walk(0x1b, 0x108, 0);
  Walk(0x1a, 0x108, 0x1e);
  Actor_SetAngleToward(0x1a, 4, 0);
  Actor_SetAngleToward(4, 0x1b, 0);
  Actor_WalkToAndWait(0x1b, 0x2bb, 0xca);
  ACTOR_SET_MODE_AND_WAIT(0x1b, 3, 0x14);
  Event_Wait(0xa);
  Event_ShowMessageThenWait(0x1b, 0, 5);
  Walk(0, 0x100, 0);
  Walk(4, 0x100, 0);
  Walk(2, 0x100, 0);
  Walk(5, 0x100, 0);
  ((struct Actor *)Func_080d47b4(0x1c, 0x100, 0x1e));
  ((struct Actor *)Func_080d47b4(0x1c, 0x109, 0x1e));
  Actor_ArmCallback(0x1c, 0, 0);
  Event_ShowMessageThenWait(0x1c, 0, 5);
  ((struct Actor *)Func_080d47b4(0x1a, 0x108, 0x1e));
  Event_ShowMessageThenWait(0x1a, 0, 5);
  Actor_TurnToFaceEachOther(4, 7, 0);
  Actor_TurnToFaceEachOther(6, 5, 0);
  Actor_RefreshSelectorById(6);
  Actor_RefreshSelectorById(5);
  Event_Wait(0x28);
  Actor_SetAngleToward(4, 0x1a, 0);
  Actor_SetAngleToward(7, 0x1a, 0);
  Actor_SetAngleToward(6, 0x1a, 0);
  Actor_SetAngleToward(5, 0x1a, 0);
  ((struct Actor *)Func_080d47b4(0x1a, 0x107, 0x1e));
  Actor_ArmCallback(0x1b, 0, 0);
  Event_ShowMessageThenWait(0x1b, 0, 5);
  Actor_SetSpeed(7, 0x40000, 0x20000);
  Actor_SetSpeed(0x1b, 0x20000, 0x10000);
  Actor_OffsetPositionAndReset(7, 8, 0x18);
  Actor_CommitPositionAndActivate(0x1b, 8, 4);
  Actor_WaitForMoveById(7);
  Event_Wait(5);
  ((struct Actor *)Func_080d47b4(0x1a, 0x103, 0x1e));
  Actor_SetAngleToward(0x1a, 7, 0);
  Actor_SetAngleToward(0x1b, 7, 0);
  Event_ShowMessageThenWait(0x1a, 0, 5);
  Motion_SetVarCbAndRefresh(0, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(0, 0, 5);
  Actor_SetAngleToward(0x1a, 0, 0);
  Actor_SetAngleToward(0x1b, 0, 0);
  Motion_SetVarCbAndRefresh(0, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(0, 0, 5);
  Event_Wait(0x28);
  Actor_ArmCallbackAndRefresh(4, 0x2000);
  Event_Wait(0x14);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 0x14);
  Event_Wait(0xa);
  Actor_WatchAndSetScript(0x1a, 4);
  Actor_WatchAndSetScript(0x1b, 4);
  Actor_CommitPositionAndActivate(4, 5, 0x20);
  Actor_CommitPositionAndActivate(4, 0x38, -2);
  Event_Wait(0x1e);
  Actor_StartRepeatedMotion(0, 2);
  Event_Wait(4);
  Motion_SetVarCbAndRefresh(4, 2);
  PartyInventory_Remove(0xdf);
  PartyInventory_Add(0xf5);
  FieldMessage_Show((s16)EVENT_WORK->messageId++, 5);
  index = 0;
  offset = 0xd8;
  for (index = 0; index <= 14; offset += 2, index++) {
    if ((*(u16 *)((u8 *)Unit_Get(4) + offset) & 0x1ff) == 245) {
      Func_020047a4(4, index);
      break;
    }
  }
  ACTOR_SET_MODE_AND_WAIT(0x1b, 3, 0x14);
  Event_Wait(0xa);
  Event_ShowMessageThenWait(0x1b, 0, 5);
  Actor_SetAngleToward(4, 0x1b, 0);
  Event_Wait(0x14);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 0x14);
  Event_Wait(0x14);
  Actor_SetSpeed(7, 0x20000, 0x10000);
  Actor_SetSpeed(0x1a, 0x8000, 0x4000);
  Actor_SetSpeed(0x1b, 0x8000, 0x4000);
  Actor_SetSpeed(4, 0x8000, 0x4000);
  Actor_SetMovementTypeById(0x1a, (s32)gData_020059a4);
  Event_Wait(0x28);
  Actor_OffsetPositionAndResetMotion(7, -16, 0);
  Event_Wait(0xa);
  Actor_SetMovementTypeById(0x1b, (s32)gData_020059a4);
  Event_Wait(0x6e);
  Actor_SetMovementTypeById(4, (s32)gData_020059a4);
  Actor_RefreshSelectorById(0x1a);
  Actor_SetMovementTypeById(0x1a, (s32)gData_02005a00);
  Actor_RefreshSelectorById(0x1b);
  Actor_SetMovementTypeById(0x1b, (s32)gData_02005a00);
  Actor_RefreshSelectorById(4);
  Actor_SetModeById(4, 1);
  Actor_StartRepeatedMotion(7, 2);
  Actor_FaceDirection(7, 0xc000, 0);
  Actor_FaceDirection(0x1c, 0xc000, 0);
  Event_ShowMessageThenWait(7, 0, 5);
  Camera_FollowActor(6, 1);
  Actor_FaceDirectionAndWait(4, 0x4000);
  Event_Wait(0xa);
  ((struct Actor *)Func_080d47b4(4, 0x101, 0x1e));
  ((struct Actor *)Func_080d47b4(7, 0x109, 0x1e));
  Event_ShowMessageThenWait(7, 0, 5);
  ((struct Actor *)Func_080d47b4(4, 0x100, 0x1e));
  ((struct Actor *)Func_080d47b4(6, 0x106, 0x1e));
  Event_ShowMessageThenWait(6, 0, 5);
  Actor_FaceDirectionAndWait(4, 0x4000);
  Event_Wait(0xa);
  ACTOR_SET_MODE_AND_WAIT(5, 3, 0x14);
  Event_Wait(0xa);
  Actor_FaceDirection(5, 0xc000, 0);
  Event_ShowMessageThenWait(5, 0, 5);
  Actor_ArmCallbackAndRefresh(4, 0x4000);
  Event_Wait(0xa);
  ACTOR_SET_MODE_AND_WAIT(0x1c, 3, 0x14);
  Event_Wait(0xa);
  Event_ShowMessageThenWait(0x1c, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 0x14);
  Event_Wait(0xa);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_SetMovementTypeById(7, (s32)gData_020059a4);
  Actor_RefreshSelectorById(7);
  Party_AddActiveOwner(7);
  Camera_FollowActor(4, 1);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  Event_Wait(0x14);
#if TLA_PATCH_1
  Event_ClearInvalidPackedValues();
#endif
  Actor_WalkToAndWait(4, 0x298, 0x46);
  Func_020047dc();
}
