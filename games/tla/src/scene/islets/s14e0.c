#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "version.h"
#include "event_inline.h"

// The value stored at the event work's 0x1ac: 521 in the Japanese release,
// 513 from patch 1.
#if TLA_PATCH_1
#define STORED_AT_1AC 513
#else
#define STORED_AT_1AC 521
#endif

extern const u8 MsgOhYouRe[], MsgIMSoVery[], MsgWantToGo[], MsgIMSoVeryVery[];
extern const u8 gData_020044f0[];
void Func_02003e64(void);
void Func_02003e6c(void);
void Func_02003fc4(void);
void Func_02003fbc(void);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
void Func_02003de4(struct Actor *object, s32 a);
void Func_02003dec(struct Actor *object, const u8 *script);
void Func_02003dfc(struct Actor *object);
void Func_0200129c(struct Actor *object);
void Func_02000df8(void);
void Func_02001278(void);
void *HeapSlot_AllocEwram(s32 slot, s32 size);
void Func_020034ec(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

#define STEP(actor, x, z)                                                      \
  (Actor_GetById(actor)->actionFlags &= ~1,                                    \
   Actor_WalkToAndWait(actor, x, z),                  \
   FieldEvent_Wait(1), Actor_GetById(actor)->actionFlags |= 1)

#define SPAWN(a, b, answer)                                                    \
  {                                                                            \
    a = FieldActor_Create(163, 0x1280000, \
                               0, 0x1a80000);                                  \
    b = FieldActor_Create(14, 0x1280000, \
                               0, 0x1a80000);                                  \
    if (a != 0) {                                                              \
      a->motionFlags = answer;                                                 \
      Actor_SetSingleChildFlags(a, 0);                                         \
    }                                                                          \
    if (b != 0) {                                                              \
      b->scaleY = 0x20000;                                                     \
      Actor_SetSingleChildFlags(b, 0);                                         \
      Func_02003de4(b, 1);                                                     \
      Func_02003dec(b, gData_020044f0);                                        \
    }                                                                          \
  }

void Func_020014e0(void) {
  struct Actor *leader, *a, *b, *other;
  const u8 *message;
  s32 answer;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  if (PartyInventory_FindOwner(455) != -1) {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (GameFlag_Test(513) != 0) {
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
      /* FAKEMATCH(temporary): the form kept; as leader->update = Func_02000df8,
       * 94 of 569 instructions differ in tla-en. */
      a = (struct Actor *)Func_02000df8;
      leader->update = (void (*)(void))a;
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 318, 424);
      leader->update = 0;
      Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x4000, 0);
      Actor_FaceDirectionAndWait(9, 0xd000);
      message = MsgOhYouRe;
      Event_SetMessageId(message);
      Event_ShowMessage(9, 0);
      Func_02003fc4();
      answer =
          Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
      if (answer == 0) {
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
        Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
        leader->update = (void (*)(void))a;
        STEP(gMainRoutineWork.currentOwner, 302, 424);
        SPAWN(a, b, answer)
        STEP(gMainRoutineWork.currentOwner, 318, 424);
        leader->update = 0;
        Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x6000, 0);
        Func_0200129c(a);
      } else {
        Func_02003fbc();
        Event_SetMessageId(message - 1);
        goto refused;
      }
    } else {
      Event_SetMessageId(MsgIMSoVery);
      Event_ShowMessage(9, 0);
      Func_080d47b4(gMainRoutineWork.currentOwner, 262, 20);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
      leader->update = Func_02000df8;
      // Default: a is set for real when object 163 is made.
      a = 0;
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 318, 424);
      leader->update = 0;
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
      leader->update = Func_02000df8;
      STEP(gMainRoutineWork.currentOwner, 302, 424);
      SPAWN(a, b, 0)
      STEP(gMainRoutineWork.currentOwner, 318, 424);
      leader->update = 0;
      Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x4000, 0);
      Func_080d47b4(9, 256, 20);
      Actor_FaceDirectionAndWait(9, 0xb000);
      other = Actor_GetById(9);
      other->update = Func_02000df8;
      FieldActor_Jump(9, 4, 20);
      other->update = 0;
      Event_ShowMessage(9, 0);
      Actor_FaceDirectionAndWait(9, 0xd000);
      Event_ShowMessage(9, 0);
      Func_02003fc4();
      if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
          0) {
        Func_0200129c(a);
      } else {
        Func_02003fbc();
        Func_080d47b4(9, 265, 0);
        Event_ShowMessage(9, 0);
        Actor_FaceDirection(9, 0x5000, 0);
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
        leader->update = Func_02000df8;
        Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 302, 424);
        leader->update = 0;
        if (a != 0)
          Func_02003dfc(a);
        Event_SetFlag(513);
      }
    }
  } else if (GameFlag_Test(0xaa5)) {
    Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
    Event_SetMessageId(MsgWantToGo);
    Event_ShowMessage(9, 0);
    Func_02003fc4();
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
      Func_02003fbc();
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
      Event_ShowMessage(9, 0);
      ((u8 *)HeapSlot_AllocEwram(88, 664))[0x295] = 99;
      FieldActor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
      Actor_FaceDirectionAndWait(9, 0x5000);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
      FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
      Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 296, 452);
      leader->update = Func_02001278;
      Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 15);
      Actor_SetAnimation(9, 3);
      FieldEvent_Wait(20);
      Event_ShowMessage(9, 0);
      Func_02003fc4();
      Actor_SetSpeed(9, 0xcccc, 0x6666);
      Actor_SetSpeed(10, 0xcccc, 0x6666);
      Actor_GetById(9)->update = Func_02000df8;
      Actor_SetDestination(9, 288, 538);
      FieldEvent_Wait(40);
      Func_02003fc4();
      Func_020034ec();
      Actor_GetById(10)->update = Func_02000df8;
      Actor_WalkTo(10, 288, 538);
      FieldEvent_Wait(20);
      *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = STORED_AT_1AC;
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      Func_02003fbc();
      FieldEvent_SetExitRequest(3);
    } else {
      Func_02003fbc();
    refused:
      Event_ShowMessage(9, 0);
      Actor_FaceDirectionAndWait(9, 0x5000);
    }
  } else {
    Event_SetMessageId(MsgIMSoVeryVery);
    Event_ShowMessage(9, 0);
  }
  Func_02003e6c();
}
