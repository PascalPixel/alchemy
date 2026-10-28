// Draft, differ 122 of 946 (was 142): the three scripts run on entrances 9, 74
// and the rest after flag 0xa76 (actors 20 to 23 and 4). Calls go through
// call.h where the native rebuilds their constants and 0x4000 and 0xc000 are
// locals where it shares them. A greedy pass over every call, direct or through
// call.h, kept four changes (142 to 122): two Func_02004d08 calls and one
// Func_02004c90 call direct, one Actor_SetSpeedParameters call
// wrapped. Left: argument order at about 25 calls, where sched2 issues the
// first argument between the halves of a split constant and the native issues
// it last, and a few constants shared or rebuilt the other way.
#include "types.h"
#include "ram_buffer.h"
#include "adventure.h"
#include "actor.h"
#include "event_work.h"
#include "call.h"

extern char MsgHeyWaitA[];
extern char MsgItLooksLike[];
extern char MsgWeLostAgain[];
extern char SceneId_WorldMap;

void GameFlag_ClearBit(s32 flag);
void FieldEvent_WaitFrames(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Actor_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Actor_OffsetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_ArmCallback(s32 id, s32 angle, s32 mode);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_ResetAndSetPosition(s32 id, s32 x, s32 z);
void Motion_SetVarCbAndRefresh(s32 id, s32 value);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Actor_TurnToFaceEachOther(s32 id, s32 other, s32 mode);
void Camera_FollowActor(s32 id, s32 mode);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void FieldMessage_ShowAtActor(s32 id, s32 mode);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 mode);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Party_SetFields1f2And1f4(s32 scene, s32 entrance);
void Func_02000d4c(void);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004c88(s32 id, s32 x, s32 z);
void Func_02004c90(s32 id, s32 x, s32 z, s32 angle);
void Func_02004cd8(s32 message);
void Func_02004ce8(s32 id, s32 a, s32 b);
void Func_02004d00(s32 id, s32 angle);
void Func_02004d08(s32 id, s32 animation, s32 frames);
void Func_02004d38(s32 scene, s32 entrance);
void Func_02004d48(s32 a, s32 b);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004db8(s32 value);
void Func_02004dd8(s32 id, s32 a, s32 b, s32 angle);
void Func_02004df8(s32 a, s32 b);
void Func_02004e00(void);

void Func_020012bc(s32 entrance) {
  s32 east, west, east74, eastB, westB;

  Func_02004c20();
  Func_02004db8(0);
  if (entrance == 9) {
    west = 0xc000;
    Call4((void (*)())Func_02004c90, 20, 0x27b80000, 0x23280000, west);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004da8();
    Func_02004cd8((s32)MsgHeyWaitA);
    Call3((void (*)())Func_02004ce8, 20, 0, 5);
    Func_02004db0();
    Func_02004df8(0x18ccc, 16);
    Call3((void (*)())Func_02004d08, 4, 0x102, 30);
    Call3((void (*)())Actor_CommitPositionAndActivate, 4, 0, 32);
    Call3((void (*)())Actor_ArmCallback, 4, west, 0);
    Func_02004e00();
    Actor_WaitForMoveById(4);
    Func_02004da8();
    Func_02004dd8(23, 16, 0, west);
    Actor_RefreshSelectorById(23);
    Call2((void (*)())Func_02004d00, 23, west);
    east = 0x4000;
    FieldEvent_WaitFrames(10);
    Call4((void (*)())Func_02004c90, 20, 0x27b80000, 0x25800000, east);
    Call4((void (*)())Func_02004c90, 21, 0x27b80000, 0x25800000, east);
    Call4((void (*)())Func_02004c90, 22, 0x27b80000, 0x25800000, east);
    Call3((void (*)())Actor_OffsetPositionAndReset, 20, 0, 16);
    Call3((void (*)())Actor_OffsetPositionAndReset, 21, -16, 16);
    Call3((void (*)())Actor_CommitPositionAndActivate, 22, 16, 16);
    Call3((void (*)())Actor_ArmCallback, 20, 0x3000, 0);
    Call3((void (*)())Actor_ArmCallback, 21, 0x3000, 0);
    Call3((void (*)())Actor_ArmCallback, 22, 0x3000, 0);
    Call3((void (*)())Func_02004d08, 23, 0x100, 30);
    Actor_WaitForMoveById(20);
    Actor_WaitForMoveById(21);
    Actor_WaitForMoveById(22);
    Func_02004da8();
    Call3((void (*)())Func_02004ce8, 23, 0, 5);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 20, 2);
    FieldEvent_WaitFrames(5);
    Call3((void (*)())Func_02004ce8, 20, 0, 5);
    Actor_TurnToFaceEachOther(4, 23, 0);
    FieldEvent_WaitFrames(30);
    FieldEvent_WaitFrames(10);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 21, 2);
    Call3((void (*)())Actor_ArmCallback, 4, west, 0);
    Call3((void (*)())Actor_ArmCallback, 23, west, 0);
    FieldEvent_WaitFrames(5);
    Call3((void (*)())Func_02004ce8, 21, 0, 5);
    Call3((void (*)())Func_02004d08, 23, 0x109, 30);
    Call3((void (*)())Func_02004ce8, 23, 0, 5);
    Call3((void (*)())Func_02004d08, 22, 0x107, 30);
    Call3((void (*)())Func_02004ce8, 22, 0, 5);
    Call3((void (*)())Func_02004d08, 23, 0x101, 30);
    Call3((void (*)())Func_02004ce8, 23, 0, 5);
    Call3((void (*)())Func_02004d08, 20, 0x107, 30);
    Call3((void (*)())Func_02004ce8, 20, 0, 5);
    Call3((void (*)())Func_02004d08, 23, 0x102, 30);
    Call3((void (*)())Func_02004ce8, 23, 0, 5);
    Call3((void (*)())Func_02004ce8, 21, 0, 5);
    Actor_TurnToFaceEachOther(4, 23, 0);
    FieldEvent_WaitFrames(30);
    Call3((void (*)())Func_02004ce8, 23, 0, 5);
    Call3((void (*)())Func_02004d08, 22, 0x107, 30);
    Call3((void (*)())Actor_ArmCallback, 4, west, 0);
    Call3((void (*)())Actor_ArmCallback, 23, west, 0);
    Call3((void (*)())Func_02004ce8, 22, 0, 5);
    Call3((void (*)())Actor_OffsetPositionAndReset, 4, 8, -8);
    Call3((void (*)())Actor_OffsetPositionAndReset, 23, -8, 8);
    Actor_WaitForMoveById(4);
    Actor_WaitForMoveById(23);
    Call3((void (*)())Actor_ArmCallback, 4, west, 0);
    Call3((void (*)())Actor_ArmCallback, 23, west, 0);
    Func_02004d08(4, 0x102, 30);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 20, 3);
    FieldEvent_WaitFrames(10);
    Call3((void (*)())Func_02004ce8, 20, 0, 5);
    gAdventure.battleRule24b = 2;
    Party_SetFields1eeAnd1f0((s32)&SceneId_WorldMap, 73);
    Party_SetFields1f2And1f4((s32)&SceneId_WorldMap, 74);
    Func_02004db0();
    Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
    Func_02004d48(102, 3);
  } else if (entrance == 74) {
    east74 = 0x4000;
    GameFlag_ClearBit(0xa76);
    Call4((void (*)())Func_02004c90, 20, 0x27b80000, 0x25900000, east74);
    Call4((void (*)())Func_02004c90, 21, 0x27a80000, 0x25900000, east74);
    Call4((void (*)())Func_02004c90, 22, 0x27c80000, 0x25900000, east74);
    Call4((void (*)())Func_02004c90, 23, 0x27a80000, 0x25c00000, 0xe000);
    Call4((void (*)())Func_02004c90, 4, 0x27b80000, 0x25b00000, 0xa000);
    Camera_FollowActor(4, 0);
    Func_02004df8(0x18ccc, 1);
    Call2((void (*)())Actor_SetModeById, 4, 19);
    Actor_SetSingleChildFlags(Actor_GetById(4), 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004da8();
    Func_02004cd8((s32)MsgItLooksLike);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 4, 2);
    FieldEvent_WaitFrames(5);
    Call3((void (*)())Func_02004d08, 20, 0x108, 30);
    Call3((void (*)())Func_02004ce8, 20, 0, 5);
    Call2((void (*)())Func_02004d00, 21, east74);
    FieldEvent_WaitFrames(10);
    Call3((void (*)())Func_02004ce8, 21, 0, 5);
    Call2((void (*)())Func_02004d00, 22, east74);
    FieldEvent_WaitFrames(10);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 22, 2);
    FieldEvent_WaitFrames(5);
    Call3((void (*)())Func_02004ce8, 22, 0, 5);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 21, 3);
    FieldEvent_WaitFrames(10);
    Call2((void (*)())Func_02004d00, 20, east74);
    FieldEvent_WaitFrames(10);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 20, 2);
    FieldEvent_WaitFrames(5);
    Call3((void (*)())Func_02004ce8, 20, 0, 5);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 20, 3);
    FieldEvent_WaitFrames(10);
    Call3((void (*)())Func_02004ce8, 20, 0, 5);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 21, 3);
    FieldEvent_WaitFrames(10);
    Call3((void (*)())Func_02004ce8, 21, 0, 5);
    Call3((void (*)())Func_02004d08, 22, 0x108, 30);
    Call3((void (*)())Func_02004ce8, 22, 0, 5);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 20, 3);
    FieldEvent_WaitFrames(10);
    FieldEvent_WaitFrames(40);
    Func_02004d38(gAdventure.pair1ee[0], gAdventure.pair1ee[1]);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  } else {
    eastB = 0x4000;
    westB = 0xc000;
    GameFlag_ClearBit(0xa76);
    Func_02004c90(20, 0x27b80000, 0x25a00000, eastB);
    Call4((void (*)())Func_02004c90, 21, 0x27a80000, 0x25a00000, eastB);
    Call4((void (*)())Func_02004c90, 22, 0x27c80000, 0x25a00000, eastB);
    Call4((void (*)())Func_02004c90, 23, 0x27b00000, 0x25d00000, westB);
    Call4((void (*)())Func_02004c90, 4, 0x27c00000, 0x25d00000, westB);
    Camera_FollowActor(4, 0);
    Func_02004df8(0x18000, 1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004da8();
    Func_02004cd8((s32)MsgWeLostAgain);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 20, 2);
    FieldEvent_WaitFrames(5);
    Call3((void (*)())Func_02004d08, 20, 0x102, 30);
    Call3((void (*)())Func_02004ce8, 20, 0, 5);
    Call3((void (*)())Func_02004d08, 21, 0x102, 30);
    Call3((void (*)())Func_02004ce8, 21, 0, 5);
    Call3((void (*)())Func_02004d08, 22, 0x102, 30);
    Call3((void (*)())Func_02004ce8, 22, 0, 5);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 20, 2);
    FieldEvent_WaitFrames(5);
    Call3((void (*)())Func_02004ce8, 20, 0, 5);
    Call2((void (*)())Func_02004d00, 21, 0);
    FieldEvent_WaitFrames(10);
    Call3((void (*)())Func_02004d08, 21, 0x101, 30);
    Call3((void (*)())Func_02004ce8, 21, 0, 5);
    Call2((void (*)())Func_02004d00, 20, 0x8000);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 20, 3);
    FieldEvent_WaitFrames(10);
    Call3((void (*)())Func_02004ce8, 20, 0, 5);
    Call2((void (*)())Func_02004d00, 22, 0x8000);
    FieldEvent_WaitFrames(10);
    Call3((void (*)())Func_02004d08, 22, 0x101, 30);
    Call3((void (*)())Func_02004ce8, 22, 0, 5);
    Call3((void (*)())Func_02004d08, 23, 0x101, 30);
    Call3((void (*)())Actor_ArmCallback, 20, eastB, 0);
    Call3((void (*)())Actor_ArmCallback, 21, eastB, 0);
    Call3((void (*)())Actor_ArmCallback, 22, eastB, 0);
    Call3((void (*)())Func_02004ce8, 23, 0, 5);
    Call2((void (*)())Actor_SetModeById, 20, 3);
    Call2((void (*)())Actor_SetModeById, 21, 3);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 22, 3);
    FieldEvent_WaitFrames(10);
    Call3((void (*)())Func_02004d08, 23, 0x109, 30);
    Call3((void (*)())Func_02004ce8, 23, 0, 5);
    Call3((void (*)())Func_02004d08, 20, 0x100, 0);
    Call3((void (*)())Func_02004d08, 21, 0x100, 0);
    Call3((void (*)())Func_02004d08, 22, 0x100, 30);
    FieldEvent_WaitFrames(40);
    Actor_TurnToFaceEachOther(20, 21, 0);
    Call3((void (*)())Actor_ArmCallback, 22, 0x8000, 0);
    Call3((void (*)())Func_02004d08, 22, 0x105, 30);
    Func_02004d08(20, 0x102, 40);
    Call2((void (*)())Func_02004d00, 20, eastB);
    FieldEvent_WaitFrames(20);
    FieldMessage_ShowAtActor(20, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      Call3((void (*)())Func_02004d08, 20, 0x109, 30);
      Call3((void (*)())Func_02004ce8, 20, 0, 5);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    } else {
      Call3((void (*)())Func_02004d08, 20, 0x109, 30);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
      Call3((void (*)())Func_02004ce8, 20, 0, 5);
    }
    Actor_TurnToFaceEachOther(4, 23, 0);
    FieldEvent_WaitFrames(40);
    Call3((void (*)())Actor_ArmCallback, 4, 0xc000, 0);
    Call3((void (*)())Actor_ArmCallback, 23, 0xc000, 0);
    Call3((void (*)())Func_02004d08, 23, 0x109, 30);
    Call3((void (*)())Func_02004ce8, 23, 0, 5);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 21, 2);
    Call3((void (*)())Actor_ArmCallback, 21, 0x3000, 0);
    Call3((void (*)())Actor_ArmCallback, 22, 0x5000, 0);
    FieldEvent_WaitFrames(5);
    Call3((void (*)())Func_02004ce8, 21, 0, 5);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 22, 4);
    Call3((void (*)())Func_02004ce8, 22, 0, 5);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 20, 2);
    Func_02000d4c();
    FieldEvent_WaitFrames(5);
    Call3((void (*)())Func_02004d08, 20, 0x109, 30);
    Call3((void (*)())Func_02004ce8, 20, 0, 5);
    Actor_TurnToFaceEachOther(20, 21, 0);
    FieldEvent_WaitFrames(40);
    Call2((void (*)())Actor_SetModeById, 21, 3);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 20, 3);
    FieldEvent_WaitFrames(10);
    Actor_TurnToFaceEachOther(20, 22, 0);
    FieldEvent_WaitFrames(40);
    Call2((void (*)())Actor_SetModeById, 22, 3);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 20, 3);
    FieldEvent_WaitFrames(10);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 20, 2);
    Call3((void (*)())Actor_ArmCallback, 21, 0x4000, 0);
    Call3((void (*)())Actor_ArmCallback, 22, 0x4000, 0);
    Call2((void (*)())Func_02004d00, 20, 0x4000);
    FieldEvent_WaitFrames(5);
    Call3((void (*)())Func_02004ce8, 20, 0, 5);
    Call3((void (*)())Actor_OffsetPositionAndReset, 20, -32, 0);
    Call3((void (*)())Actor_OffsetPositionAndReset, 21, -32, 0);
    Call3((void (*)())Actor_CommitPositionAndActivate, 22, -32, 0);
    Call3((void (*)())Actor_OffsetPositionAndReset, 20, -64, -64);
    Call3((void (*)())Actor_OffsetPositionAndReset, 21, -64, -64);
    Call3((void (*)())Actor_CommitPositionAndActivate, 22, -64, -64);
    Call3((void (*)())Func_02004c88, 20, 0, 0);
    Call3((void (*)())Func_02004c88, 21, 0, 0);
    Call3((void (*)())Func_02004c88, 22, 0, 0);
    FieldEvent_WaitFrames(10);
    Call2((void (*)())Func_02004d00, 20, 0x4000);
    FieldEvent_WaitFrames(10);
    Call2((void (*)())Func_02004d00, 23, 0);
    Call2((void (*)())Func_02004d00, 4, 0x8000);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 23, 3);
    FieldEvent_WaitFrames(10);
    Call3((void (*)())Func_02004ce8, 23, 0, 5);
    Call2((void (*)())Actor_SetModeById, 4, 3);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 23, 3);
    FieldEvent_WaitFrames(10);
    Call3((void (*)())Actor_SetSpeedParameters, 23, 0x13333, 0x9999);
    Call2((void (*)())Actor_SetModeById, 23, 2);
    {
      struct Actor *object = Actor_GetById(4);

      if (object != NULL)
        Actor_ResetAndSetPosition(23, *(s16 *)((u8 *)object + 10),
                                         *(s16 *)((u8 *)object + 18));
    }
    Actor_WaitForMoveById(23);
    Call3((void (*)())Func_02004c88, 23, 0, 0);
    Func_02004db0();
    Func_02004df8(0x10000, 15);
    Func_02004c28();
  }
}
