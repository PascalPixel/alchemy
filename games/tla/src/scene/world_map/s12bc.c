#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_work.h"
#include "event_inline.h"

/// gMainRoutineWork.savedScene and savedEntrance as this script reads them.
struct SavedDestination {
  u8 unk000[0x1e4];
  s16 scene;
  s16 entrance;
};

extern char MsgHeyWaitA[];
extern char MsgItLooksLike[];
extern char MsgWeLostAgain[];
extern char SceneId_WorldMap;

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000d4c(void);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004d38(s32 scene, s32 entrance);
void Func_02004d48(s32 a, s32 b);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004e00(void);

#define ARM_WEST(id)                                                           \
  do {                                                                         \
    /* FAKEMATCH(barrier): the native copies the angle before it loads the     \
     * actor id; as the plain Call3, 10 of 946 instructions differ in tla-en.  \
     */                                                                        \
    s32 a1 = west;                                                             \
                                                                               \
    asm volatile("" : "+l"(a1));                                               \
    Actor_ArmCallback(id, a1, 0);                                              \
  } while (0)

void Func_020012bc(s32 entrance) {
  s32 east, west, east74, eastB, westB;

  Func_02004c20();
  Event_ResolvePendingActions(0);
  if (entrance == 9) {
    west = 0xc000;
    Actor_SetPositionAndFacing(20, 0x27b80000, 0x23280000, west);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004da8();
    Event_SetMessageId((s32)MsgHeyWaitA);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Func_02004db0();
    Func_080d46a4(0x18ccc, 16);
    Actor_ShowEmote(4, 0x102, 30);
    Actor_WalkByAndWait(4, 0, 32);
    Actor_FaceDirection(4, west, 0);
    Func_02004e00();
    Actor_WaitForMoveById(4);
    Func_02004da8();
    {
      /* FAKEMATCH(barrier): the native copies the angle before the other
       * arguments; as the plain call, 2 of 946 instructions differ in tla-en.
       */
      s32 a3 = west;

      asm volatile("" : "+l"(a3));
      Func_080d3070(23, 16, 0, a3);
    }
    Actor_RefreshSelectorById(23);
    Actor_FaceDirectionAndWait(23, west);
    east = 0x4000;
    Event_Wait(10);
    Actor_SetPositionAndFacing(20, 0x27b80000, 0x25800000, east);
    Actor_SetPositionAndFacing(21, 0x27b80000, 0x25800000, east);
    Actor_SetPositionAndFacing(22, 0x27b80000, 0x25800000, east);
    Actor_WalkBy(20, 0, 16);
    Actor_WalkBy(21, -16, 16);
    Actor_WalkByAndWait(22, 16, 16);
    Actor_FaceDirection(20, 0x3000, 0);
    Actor_FaceDirection(21, 0x3000, 0);
    Actor_FaceDirection(22, 0x3000, 0);
    Actor_ShowEmote(23, 0x100, 30);
    Actor_WaitForMoveById(20);
    Actor_WaitForMoveById(21);
    Actor_WaitForMoveById(22);
    Func_02004da8();
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_TurnToFaceEachOther(4, 23, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    ARM_WEST(4);
    {
      /* FAKEMATCH(barrier): the native copies the angle, loads the 0 and then
       * the actor; as the plain Call3, 2 of 946 instructions differ in tla-en.
       */
      s32 a1 = west, a2;

      asm volatile("" : "+l"(a1));
      a2 = 0;
      asm volatile("" : "+l"(a2));
      Actor_ArmCallback(23, a1, a2);
    }
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_ShowEmote(23, 0x109, 30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_ShowEmote(22, 0x107, 30);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_ShowEmote(23, 0x101, 30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_ShowEmote(20, 0x107, 30);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_ShowEmote(23, 0x102, 30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_TurnToFaceEachOther(4, 23, 0);
    Event_Wait(30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_ShowEmote(22, 0x107, 30);
    ARM_WEST(4);
    ARM_WEST(23);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_WalkBy(4, 8, -8);
    Actor_WalkBy(23, -8, 8);
    Actor_WaitForMoveById(4);
    Actor_WaitForMoveById(23);
    ARM_WEST(4);
    ARM_WEST(23);
    {
      /* FAKEMATCH(barrier): the native starts 0x102, loads the 30, shifts, and
       * loads the actor last; as the plain Value3, 2 of 946 instructions differ
       * in tla-en. */
      s32 a1 = 129, a2;

      asm volatile("" : "+l"(a1));
      a2 = 30;
      asm volatile("" : "+l"(a2));
      a1 <<= 1;
      asm volatile("" : "+l"(a1));
      Actor_ShowEmote(4, a1, a2);
    }
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    gMainRoutineWork.battleRule24b = 2;
    /* FAKEMATCH(dead): the native loads the scene's id after this store; as
     * no block, 2 of 946 instructions differ in tla-en. */
    do {
    } while (0);
    {
      s32 worldMap = (s32)&SceneId_WorldMap;

      Event_SetAfterBattleDestination(worldMap, 73);
      Party_SetFields1f2And1f4(worldMap, 74);
    }
    Func_02004db0();
    FieldCamera_MoveTo(-1, -1, -1, 0);
    Func_02004d48(102, 3);
  } else if (entrance == 74) {
    Event_ClearFlag(0xa76);
    east74 = 0x4000;
    Actor_SetPositionAndFacing(20, 0x27b80000, 0x25900000, east74);
    Actor_SetPositionAndFacing(21, 0x27a80000, 0x25900000, east74);
    Actor_SetPositionAndFacing(22, 0x27c80000, 0x25900000, east74);
    {
      s32 a3 = 0xe000;

      Actor_SetPositionAndFacing(23, 0x27a80000, 0x25c00000, a3);
    }
    Actor_SetPositionAndFacing(4, 0x27b80000, 0x25b00000, 0xa000);
    FieldCamera_FollowActor(4, 0);
    Func_080d46a4(0x18ccc, 1);
    Actor_SetAnimation(4, 19);
    Actor_SetSingleChildFlags(Actor_Get(4), 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004da8();
    Event_SetMessageId((s32)MsgItLooksLike);
    Actor_RunRepeatedMotion(4, 2);
    Event_Wait(5);
    Actor_ShowEmote(20, 0x108, 30);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_FaceDirectionAndWait(21, east74);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_FaceDirectionAndWait(22, east74);
    Event_Wait(10);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_SetAnimationAndWait(21, 3);
    Event_Wait(10);
    Actor_FaceDirectionAndWait(20, east74);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_SetAnimationAndWait(21, 3);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_ShowEmote(22, 0x108, 30);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(10);
    Event_Wait(40);
    Func_02004d38(((struct SavedDestination *)&gMainRoutineWork)->scene,
                  ((struct SavedDestination *)&gMainRoutineWork)->entrance);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  } else {
    Event_ClearFlag(0xa76);
    eastB = 0x4000;
    Actor_SetPositionAndFacing(20, 0x27b80000, 0x25a00000, eastB);
    westB = 0xc000;
    Actor_SetPositionAndFacing(21, 0x27a80000, 0x25a00000, eastB);
    Actor_SetPositionAndFacing(22, 0x27c80000, 0x25a00000, eastB);
    Actor_SetPositionAndFacing(23, 0x27b00000, 0x25d00000, westB);
    Actor_SetPositionAndFacing(4, 0x27c00000, 0x25d00000, westB);
    FieldCamera_FollowActor(4, 0);
    Func_080d46a4(0x18000, 1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004da8();
    Event_SetMessageId((s32)MsgWeLostAgain);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(5);
    Actor_ShowEmote(20, 0x102, 30);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_ShowEmote(21, 0x102, 30);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_ShowEmote(22, 0x102, 30);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_FaceDirectionAndWait(21, 0);
    Event_Wait(10);
    Actor_ShowEmote(21, 0x101, 30);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_FaceDirectionAndWait(20, 0x8000);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_FaceDirectionAndWait(22, 0x8000);
    Event_Wait(10);
    Actor_ShowEmote(22, 0x101, 30);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_ShowEmote(23, 0x101, 30);
    Actor_FaceDirection(20, eastB, 0);
    Actor_FaceDirection(21, eastB, 0);
    Actor_FaceDirection(22, eastB, 0);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_SetAnimation(20, 3);
    Actor_SetAnimation(21, 3);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(10);
    Actor_ShowEmote(23, 0x109, 30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_ShowEmote(20, 0x100, 0);
    Actor_ShowEmote(21, 0x100, 0);
    Actor_ShowEmote(22, 0x100, 30);
    Event_Wait(40);
    Actor_TurnToFaceEachOther(20, 21, 0);
    Actor_FaceDirection(22, 0x8000, 0);
    Actor_ShowEmote(22, 0x105, 30);
    Actor_ShowEmote(20, 0x102, 40);
    Actor_FaceDirectionAndWait(20, eastB);
    Event_Wait(20);
    Event_OpenMessage(20, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
      Actor_ShowEmote(20, 0x109, 30);
      FieldEvent_ShowMessageThenWait(20, 0, 5);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    } else {
      Actor_ShowEmote(20, 0x109, 30);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
      FieldEvent_ShowMessageThenWait(20, 0, 5);
    }
    Actor_TurnToFaceEachOther(4, 23, 0);
    Event_Wait(40);
    Actor_FaceDirection(4, 0xc000, 0);
    Actor_FaceDirection(23, 0xc000, 0);
    Actor_ShowEmote(23, 0x109, 30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_RunRepeatedMotion(21, 2);
    Actor_FaceDirection(21, 0x3000, 0);
    Actor_FaceDirection(22, 0x5000, 0);
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_SetAnimationAndWait(22, 4);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_RunRepeatedMotion(20, 2);
    Func_02000d4c();
    Event_Wait(5);
    Actor_ShowEmote(20, 0x109, 30);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_TurnToFaceEachOther(20, 21, 0);
    Event_Wait(40);
    Actor_SetAnimation(21, 3);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(10);
    Actor_TurnToFaceEachOther(20, 22, 0);
    Event_Wait(40);
    Actor_SetAnimation(22, 3);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Actor_FaceDirection(21, 0x4000, 0);
    Actor_FaceDirection(22, 0x4000, 0);
    Actor_FaceDirectionAndWait(20, 0x4000);
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_WalkBy(20, -32, 0);
    Actor_WalkBy(21, -32, 0);
    Actor_WalkByAndWait(22, -32, 0);
    Actor_WalkBy(20, -64, -64);
    Actor_WalkBy(21, -64, -64);
    Actor_WalkByAndWait(22, -64, -64);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(22, 0, 0);
    Event_Wait(10);
    Actor_FaceDirectionAndWait(20, 0x4000);
    Event_Wait(10);
    Actor_FaceDirectionAndWait(23, 0);
    Actor_FaceDirectionAndWait(4, 0x8000);
    Actor_SetAnimationAndWait(23, 3);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_SetAnimation(4, 3);
    Actor_SetAnimationAndWait(23, 3);
    Event_Wait(10);
    Actor_SetSpeed(23, 0x13333, 0x9999);
    Actor_SetAnimation(23, 2);
    {
      struct Actor *object = Actor_Get(4);

      if (object != NULL)
        Actor_SetDestination(23, *(s16 *)((u8 *)object + 10),
                                  *(s16 *)((u8 *)object + 18));
    }
    Actor_WaitForMoveById(23);
    Actor_SetPosition(23, 0, 0);
    Func_02004db0();
    Func_080d46a4(0x10000, 15);
    Func_02004c28();
  }
}
