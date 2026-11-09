#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"
#include "event_inline.h"

extern const u8 MsgSniffSniffI[], MsgTradedHisMilk[], MsgIDonTLike[],
    MsgIDonTWantTo[], MsgTheWaterHere[];
extern const u8 gData_020043f8[], gData_02004300[];
void Func_02003e64(void);
void Func_02003e6c(void);
void Func_02000df8(void);
struct Actor *Func_02000da0(struct Actor *holder, s32 item);
void Func_02003fc4(void);
void Func_02003fbc(void);
void PartyInventory_Remove(s32 item);
void Func_02003dfc(struct Actor *object);

#define STEP(actor, x, z)                                                      \
  (Actor_GetById(actor)->actionFlags &= ~1, Actor_WalkToAndWait(actor, x, z),  \
   FieldEvent_Wait(1), Actor_GetById(actor)->actionFlags |= 1)

void Func_02000e38(void) {
  struct Actor *leader, *trader, *item, *gift;
  s32 right;
  s32 giftItem;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  if (FieldPartyInventory_FindOwner(454) != -1) {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    trader = Actor_GetById(11);
    // Default: the trader's zero update stores read it.
    gift = 0;
    right = 0;
    Actor_FaceActor(11, gMainRoutineWork.currentOwner, 0);
    trader->update = Func_02000df8;
    FieldActor_Jump(11, 4, 10);
    trader->update = 0;
    Event_SetMessageId(MsgSniffSniffI);
    Event_ShowMessage(11, 0);
    if (leader->x > trader->x)
      right = 1;
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_SetSpeed(11, 0x19999, 0xcccc);
    if (right) {
      Actor_FaceDirection(11, 0, 0);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 296, 856);
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
      item = (struct Actor *)Value2((s32 (*)())Func_02000da0, (s32)leader, 454);
      trader->update = Func_02000df8;
      Actor_RunScriptAndWait(11, gData_020043f8);
      trader->update = 0;
    } else {
      Actor_FaceDirection(11, 0x8000, 0);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 856);
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
      item = (struct Actor *)Value2((s32 (*)())Func_02000da0, (s32)leader, 454);
      trader->update = Func_02000df8;
      Actor_RunScriptAndWait(11, gData_02004300);
      trader->update = 0;
    }
    Event_ShowMessage(11, 0);
    FieldActor_WatchAndSetScript(11, gMainRoutineWork.currentOwner);
    if (item != 0) {
      void (*hook2)(void);
      item->collisionFlags = 9;
      FieldActor_SetPositionAndResetMotion(item, 0x680000, 0, 0x3480000);
      hook2 = Func_02000df8;
      if (right) {
        leader->update = hook2;
        Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 296, 840);
        leader->update = 0;
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
        leader->update = hook2;
        STEP(gMainRoutineWork.currentOwner, 0x120, 840);
        FieldActor_SetPositionAndResetMotion(item, 0x1180000, 0, 0x3480000);
        STEP(gMainRoutineWork.currentOwner, 296, 840);
        leader->update = 0;
        FieldActor_Stop(gMainRoutineWork.currentOwner);
        System_Wait(1);
        trader->update = hook2;
        STEP(11, 292, 844);
      } else {
        leader->update = hook2;
        Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 840);
        leader->update = 0;
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
        leader->update = hook2;
        STEP(gMainRoutineWork.currentOwner, 0x110, 840);
        FieldActor_SetPositionAndResetMotion(item, 0x1180000, 0, 0x3480000);
        STEP(gMainRoutineWork.currentOwner, 264, 840);
        leader->update = 0;
        FieldActor_Stop(gMainRoutineWork.currentOwner);
        System_Wait(1);
        trader->update = hook2;
        STEP(11, 268, 844);
      }
    }
    giftItem = 455;
    gift =
        (struct Actor *)Value2((s32 (*)())Func_02000da0, (s32)leader, giftItem);
    STEP(11, 280, 856);
    trader->update = 0;
    Func_02003fc4();
    Message_ShowCentered((s32)MsgTradedHisMilk, 1);
    PartyInventory_Remove(454);
    Party_GiveItem(giftItem, 0);
    Func_02003fbc();
    Actor_GetById(gMainRoutineWork.currentOwner)->angle = 0x4000;
    if (gift != 0) {
      Func_02003dfc(gift);
      Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
    }
    Actor_FaceDirectionAndWait(11, 0xc000);
    Event_SetFlag(0xaa1);
    Event_SetFlag(534);
  } else {
    if (Event_GetFlag(0xaa1)) {
      FieldActor_WatchAndSetScript(11, gMainRoutineWork.currentOwner);
      if (Event_GetFlag(534))
        Event_SetMessageId(MsgIDonTLike);
      else
        Event_SetMessageId(MsgIDonTWantTo);
    } else {
      Event_SetMessageId(MsgTheWaterHere);
    }
    Event_ShowMessage(11, 0);
  }
  Func_02003e6c();
}
