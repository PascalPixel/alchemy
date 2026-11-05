#include "edition.h"

// The Japanese form differs and is still on its listing.
#if (LANGUAGE != LANGUAGE_JAPANESE)

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

extern const u8 MsgOhYouRe[], MsgIMSoVery[], MsgWantToGo[], MsgIMSoVeryVery[];
extern const u8 gData_020044f0[];
void Func_02003e64(void);
void Func_02003e6c(void);
void Func_02003fa4(s32 skip);
s32 PartyInventory_FindOwner(s32 item);
struct Actor *Actor_GetById(u32 id);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Func_02000df8(void);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_02003f3c(s32 actor, s32 angle);
void Func_02003f24(const u8 *message);
void Func_02003f2c(s32 speaker, s32 flags);
void Func_02003fc4(void);
void Func_02003fbc(void);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void FieldEvent_Wait(s32 frames);
struct Actor *Func_02003df4(s32 type, s32 x, s32 y, s32 z);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        u32 value);
void Func_02003de4(struct Actor *object, s32 mode);
void Func_02003dec(struct Actor *object, const u8 *script);
void Func_0200129c(struct Actor *item);
s32 Func_02003f54(s32 actor, s32 emote, s32 frames);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Func_02003dfc(struct Actor *object);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Actor_SetAngleToward(s32 actor, s32 target, s32 frames);
u8 *HeapSlot_AllocEwram(s32 slot, s32 size);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Actor_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void Func_02001278(void);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void Actor_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);
void Func_020034ec(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02003f7c(s32 a);

#define STEP(actor, x, z)                                                      \
  (Actor_GetById(actor)->actionFlags &= ~1,                                   \
   Call3(Actor_SetPositionAndReset, actor, x, z),                       \
   FieldEvent_Wait(1), Actor_GetById(actor)->actionFlags |= 1)

#define SHOW_ITEM(gift)                                                        \
  (gift = (struct Actor *)Value4(Func_02003df4, 163, 0x1280000, 0,     \
                                         0x1a80000),                           \
   item = (struct Actor *)Value4(Func_02003df4, 14, 0x1280000, 0,      \
                                         0x1a80000))

void Func_020014e0(void) {
  struct Actor *leader, *object, *leader2;
  struct Actor *giftAgain, *gift, *item;
  s32 answer;

  Func_02003e64();
  Func_02003fa4(0);
  if (PartyInventory_FindOwner(455) != -1) {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (GameFlag_Test(513)) {
      Call3(Actor_SetSpeedParameters, gMainRoutineWork.currentOwner, 0xcccc,
            0x6666);
      leader->update = Func_02000df8;
      Call3(Actor_SetPositionAndReset, gMainRoutineWork.currentOwner, 318,
            424);
      leader->update = 0;
      Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x4000, 0);
      Func_02003f3c(9, 0xd000);
      Func_02003f24(MsgOhYouRe);
      Func_02003f2c(9, 0);
      Func_02003fc4();
      answer = Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
      if (answer == 0) {
        Func_02003f3c(gMainRoutineWork.currentOwner, 0x8000);
        Call3(Actor_SetSpeedParameters, gMainRoutineWork.currentOwner,
              0x19999, 0xcccc);
        leader->update = Func_02000df8;
        STEP(gMainRoutineWork.currentOwner, 302, 424);
        SHOW_ITEM(giftAgain);
        if (giftAgain != 0) {
          giftAgain->motionFlags = 0;
          Actor_SetSingleChildFlags(giftAgain, 0);
        }
        if (item != 0) {
          item->scaleY = 0x20000;
          Actor_SetSingleChildFlags(item, 0);
          Func_02003de4(item, 1);
          Func_02003dec(item, gData_020044f0);
        }
        STEP(gMainRoutineWork.currentOwner, 318, 424);
        leader->update = 0;
        Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x6000, 0);
        Func_0200129c(giftAgain);
      } else {
        Func_02003fbc();
        Func_02003f24(MsgOhYouRe - 1);
        Func_02003f2c(9, 0);
        Call2(Func_02003f3c, 9, 0x5000);
      }
    } else {
      // Default: the 0 the first hook store reads.
      gift = 0;
      Func_02003f24(MsgIMSoVery);
      Func_02003f2c(9, 0);
      Func_02003f54(gMainRoutineWork.currentOwner, 262, 20);
      Call3(Actor_SetSpeedParameters, gMainRoutineWork.currentOwner, 0xcccc,
            0x6666);
      leader->update = Func_02000df8;
      Call3(Actor_SetPositionAndReset, gMainRoutineWork.currentOwner, 318,
            424);
      leader->update = 0;
      Func_02003f3c(gMainRoutineWork.currentOwner, 0x8000);
      Call3(Actor_SetSpeedParameters, gMainRoutineWork.currentOwner, 0x19999,
            0xcccc);
      leader->update = Func_02000df8;
      STEP(gMainRoutineWork.currentOwner, 302, 424);
      SHOW_ITEM(gift);
      if (gift != 0) {
        gift->motionFlags = 0;
        Actor_SetSingleChildFlags(gift, 0);
      }
      if (item != 0) {
        item->scaleY = 0x20000;
        Actor_SetSingleChildFlags(item, 0);
        Func_02003de4(item, 1);
        Func_02003dec(item, gData_020044f0);
      }
      STEP(gMainRoutineWork.currentOwner, 318, 424);
      leader->update = 0;
      Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x4000, 0);
      Func_02003f54(9, 0x100, 20);
      Func_02003f3c(9, 0xb000);
      object = Actor_GetById(9);
      object->update = Func_02000df8;
      Actor_Jump(9, 4, 20);
      object->update = 0;
      Func_02003f2c(9, 0);
      Call2(Func_02003f3c, 9, 0xd000);
      Func_02003f2c(9, 0);
      Func_02003fc4();
      if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
        Func_0200129c(gift);
      } else {
        Func_02003fbc();
        Func_02003f54(9, 265, 0);
        Func_02003f2c(9, 0);
        Call3(Actor_ArmCallback, 9, 0x5000, 0);
        Func_02003f3c(gMainRoutineWork.currentOwner, 0x8000);
        leader->update = Func_02000df8;
        Call3(Actor_SetPositionAndReset, gMainRoutineWork.currentOwner, 302,
              424);
        leader->update = 0;
        if (gift != 0)
          Func_02003dfc(gift);
        GameFlag_SetBit(513);
      }
    }
  } else if (GameFlag_Test(0xaa5)) {
    Actor_SetAngleToward(9, gMainRoutineWork.currentOwner, 0);
    Func_02003f24(MsgWantToGo);
    Func_02003f2c(9, 0);
    Func_02003fc4();
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
      leader2 = Actor_GetById(gMainRoutineWork.currentOwner);
      Func_02003fbc();
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
      Func_02003f2c(9, 0);
      ((struct {
         u8 unk000[0x295];
         u8 unk295;
       } *)HeapSlot_AllocEwram(88, 664))
          ->unk295 = 99;
      Call2(Motion_SetModeAndWaitAnimation, gMainRoutineWork.currentOwner, 3);
      Call2(Func_02003f3c, 9, 0x5000);
      Call3(Actor_SetSpeedParameters, gMainRoutineWork.currentOwner, 0x19999,
            0xcccc);
      Actor_Jump(gMainRoutineWork.currentOwner, 6, 0);
      Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 296, 452);
      leader2->update = Func_02001278;
      Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
      Actor_SetModeById(9, 3);
      FieldEvent_Wait(20);
      Func_02003f2c(9, 0);
      Func_02003fc4();
      Call3(Actor_SetSpeedParameters, 9, 0xcccc, 0x6666);
      Call3(Actor_SetSpeedParameters, 10, 0xcccc, 0x6666);
      Actor_GetById(9)->update = Func_02000df8;
      Call3(Actor_ResetAndSetPosition, 9, 0x120, 538);
      FieldEvent_Wait(40);
      Func_02003fc4();
      Func_020034ec();
      Actor_GetById(10)->update = Func_02000df8;
      Call3(Actor_ResetAndSetPositionInMode2, 10, 0x120, 538);
      FieldEvent_Wait(20);
      *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 513;
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      Func_02003fbc();
      Func_02003f7c(3);
    } else {
      Func_02003fbc();
      Func_02003f2c(9, 0);
      Call2(Func_02003f3c, 9, 0x5000);
    }
  } else {
    Func_02003f24(MsgIMSoVeryVery);
    Func_02003f2c(9, 0);
  }
  Func_02003e6c();
}

#endif
