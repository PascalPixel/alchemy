/// @file
/// The Islets' trade of item 453 with object 10. With the item in the party,
/// if flag 513 is set (asked before) the camera moves and object 10 asks again
/// (MsgAreYouGiving); otherwise it notices the item (MsgOhThatRed), the leader
/// steps up and shows it (object type 489, its picture set to the item by
/// Func_0802372c) and object 10 asks. A yes hands the item across step by
/// step, takes item 453 and gives item 454 (object 10 hands it back over the
/// leader's head), sets flag 0xaa3 and counts the answer in the event work's
/// halfword 0x1c4 the first time; a no sets flag 513. Without the item, object
/// 10 says MsgYupANice after the trade or MsgIVeBeenFeeling before it.

// zero is set only inside the first spawn check: set where CSE can see it, the
// store reuses the second answer, which then stays in r7. HAND_BACK is a plain
// block: a do-while(0) ends CSE's block and loses the owner address and
// message bases in the refusals.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern const u8 MsgAreYouGiving[], MsgOhThatRed[], MsgYupANice[],
    MsgIVeBeenFeeling[];
void Func_02003e64(void);
void Func_02003e6c(void);
void Func_02003f74(void);
void Func_02003fc4(void);
void Func_02003fbc(void);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
void PartyInventory_Remove(s32 item);
void Func_02003dfc(struct Actor *object);

#define STEP(actor, x, z)                                                      \
  (Actor_GetById(actor)->actionFlags &= ~1, Actor_WalkToAndWait(actor, x, z),  \
   Event_Wait(1), Actor_GetById(actor)->actionFlags |= 1)

/// Object 10 walks round and hands item 454 back over the leader's head.
#define HAND_BACK(leader, given, off)                                          \
  {                                                                            \
    Actor_SetSpeed(10, 0xcccc, 0x6666);                                        \
    Actor_WalkToAndWait(10, 0x80, 164);                                        \
    Actor_ArmCallbackAndRefresh(10, 0x8000);                                   \
    Event_Wait(20);                                                            \
    Actor_SetSpeed(10, 0x19999, 0xcccc);                                       \
    STEP(10, 108, 168);                                                        \
    given =                                                                    \
        FieldActor_Create(489, leader->x, leader->y + 0x180000, leader->z);    \
    if (given != 0) {                                                          \
      Actor_SetModeById(gMainRoutineWork.currentOwner, 28);                    \
      given->motionFlags = off;                                                \
      Func_0802372c(given, 454);                                               \
      Event_Wait(20);                                                          \
    }                                                                          \
    STEP(10, 116, 164);                                                        \
    Func_02003fc4();                                                           \
    PartyInventory_Remove(453);                                                \
    PartyInventory_GiveItem(454, 0);                                           \
    if (given != 0) {                                                          \
      Func_02003dfc(given);                                                    \
      Actor_SetModeById(gMainRoutineWork.currentOwner, 1);                     \
    }                                                                          \
    Func_02003fbc();                                                           \
    GameFlag_SetBit(0xaa3);                                                    \
  }

void Func_020006f4(void) {
  struct Actor *item, *leader, *given;
  const u8 *asked, *noticed;
  /* FAKEMATCH(temporary): zero wrapped in a struct of one u16, so it stays a
   * HImode pseudo and its 0 is loaded from the pool; as s32 zero, 25 of 687
   * instructions differ in tla-en. */
  struct {
    u16 v;
  } zero;

  Func_02003e64();
  Event_ResolvePendingActions(0);
  if (FieldPartyInventory_FindOwner(453) != -1) {
    if (Event_GetFlag(513)) {
      FieldCamera_MoveTo(-1, -1, -1, 0);
      FieldCamera_SetSpeed(0xcccc, 0x1999);
      FieldCamera_MoveTo(0x8c0000, -1, 0xbe0000, 1);
      Actor_SetAngleToward(10, gMainRoutineWork.currentOwner, 0);
      asked = MsgAreYouGiving;
      Event_SetMessageId(asked);
      Event_ShowMessageAndWait(10, 0);
      Func_02003f74();
      Func_02003fc4();
      if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
          0) {
        leader = Actor_GetById(gMainRoutineWork.currentOwner);
        Actor_WatchAndSetScript(10, gMainRoutineWork.currentOwner);
        Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
        Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 98, 168);
        Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, 0xc000);
        Actor_Jump(gMainRoutineWork.currentOwner, 4, 10);
        item = FieldActor_Create(489, 0x680000, 0x180000, 0xa00000);
        // Default: the object handed back is set for real in HAND_BACK.
        given = 0;
        if (item != 0) {
          Func_0802372c(item, 453);
          Actor_SetSingleChildFlags(item, 0);
          item->motionFlags = 0;
          item->unk5c[0] = 1;
        }
        Event_Wait(10);
        Func_02003fbc();
        Actor_Stop(10);
        Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
        Func_080d47b4(10, 264, 20);
        Event_SetMessageId(asked + 1);
        Event_ShowMessageAndWait(0x800a, 0);
        HAND_BACK(leader, given, 0)
      } else {
        Func_02003fbc();
        Func_080d47b4(10, 265, 20);
        Event_SetMessageId(asked + 4);
        Event_ShowMessageAndWait(0x800a, 0);
      }
    } else {
      noticed = MsgOhThatRed;
      Event_SetMessageId(noticed);
      Event_ShowMessageAndWait(10, 0);
      FieldCamera_MoveTo(-1, -1, -1, 0);
      FieldCamera_SetSpeed(0xcccc, 0x1999);
      FieldCamera_MoveTo(0x8c0000, -1, 0xbe0000, 1);
      Actor_FaceDirection(10, 0x5000, 0);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 120, 190);
      Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, 0);
      STEP(gMainRoutineWork.currentOwner, 132, 190);
      item = FieldActor_Create(489, 0x8c0000, 0, 0xbe0000);
      if (item != 0) {
        Func_0802372c(item, 453);
        Actor_SetSingleChildFlags(item, 0);
        item->motionFlags = 0;
        zero.v = 0;
        item->unk5c[0] = 1;
      }
      STEP(gMainRoutineWork.currentOwner, 120, 190);
      Actor_ArmCallback(10, 0x3000, 0);
      Actor_Jump(10, 4, 10);
      Event_ShowMessageAndWait(10, 0);
      Actor_WatchAndSetScript(10, gMainRoutineWork.currentOwner);
      STEP(gMainRoutineWork.currentOwner, 132, 190);
      FieldActor_SetPositionAndResetMotion(item, 0xc30000, 0, 0x730000);
      STEP(gMainRoutineWork.currentOwner, 120, 190);
      Event_Wait(20);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 98, 168);
      Actor_Jump(gMainRoutineWork.currentOwner, 4, 10);
      Actor_SetPositionAndResetMotion(item, 0x680000, 0x180000, 0xa00000);
      Event_Wait(10);
      Func_02003fc4();
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
      FieldMessage_Show(noticed + 2, 1);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
      Func_02003fbc();
      Actor_Jump(10, 6, 10);
      Event_SetMessageId(noticed + 3);
      Event_ShowMessageAndWait(10, 0);
      Actor_Stop(10);
      Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, 0);
      Func_02003fc4();
      if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner,
                                                0) == 0) {
        item = Actor_GetById(gMainRoutineWork.currentOwner);
        Func_02003fbc();
        Func_080d47b4(10, 264, 20);
        Event_SetMessageId(noticed + 4);
        Event_ShowMessageAndWait(0x800a, 0);
        HAND_BACK(item, item, zero.v)
      } else {
        Func_02003fbc();
        Func_080d47b4(10, 265, 20);
        Event_SetMessageId(noticed + 7);
        Event_ShowMessageAndWait(0x800a, 0);
        Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, 0xc000);
        Actor_Jump(gMainRoutineWork.currentOwner, 4, 10);
        if (item != 0)
          Func_02003dfc(item);
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
        Event_SetFlag(513);
      }
    }
  } else {
    if (Event_GetFlag(0xaa3))
      Event_SetMessageId(MsgYupANice);
    else
      Event_SetMessageId(MsgIVeBeenFeeling);
    Event_ShowMessageAndWait(10, 0);
  }
  Actor_SetModeById(10, 4);
  Func_02003e6c();
}
