// Draft, differ 205 of 569: the Islets' trade of item 455 with object 9: with
// it in the party, the leader steps up; if flag 513 is set (asked before)
// object 9 asks again (MsgOhYouRe), else it tells its story
// (MsgIMSoVery), the leader steps back and forth while the item (object
// type 14, gData_020044f0) appears, object 9 hops and asks; a yes runs
// Func_0200129c with the item, a no sets flag 513 and puts the item away.
// Without the item, after flag 0xaa5 object 9 offers to take the party
// along (MsgWantToGo): a yes counts in the event work's halfword 0x1c4,
// sets the boat block's byte 661 to 99, puts the leader aboard
// (Func_02001278) and sails off with objects 9 and 10 (Func_02003f7c(3));
// before it, it says MsgIMSoVeryVery.
// Left: the native keeps &gPartyState.currentOwner in r6 and the leader in r8,
// a 0 in r10 for the hooks and the glow's flags and the 0xfe mask in r11 (the
// draft swaps r6 and r7 and keeps other constants), and lays the first answer's
// refusal after the trade.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

extern const u8 MsgOhYouRe[], MsgIMSoVery[], MsgWantToGo[], MsgIMSoVeryVery[];
extern const u8 gData_020044f0[];
void Func_02003e64(void);
void Func_02003e6c(void);
void Func_02003fa4(s32 skip);
s32 PartyInventory_FindOwner(s32 item);
struct ObjectRuntime *Object_GetById(u32 id);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Func_02000df8(void);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_02003f3c(s32 actor, s32 angle);
void Func_02003f24(const u8 *message);
void Func_02003f2c(s32 speaker, s32 flags);
void Func_02003fc4(void);
void Func_02003fbc(void);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void FieldEvent_WaitTicks(s32 ticks);
struct ObjectRuntime *Func_02003df4(s32 type, s32 x, s32 y, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        u32 value);
void Func_02003de4(struct ObjectRuntime *object, s32 mode);
void Func_02003dec(struct ObjectRuntime *object, const u8 *script);
void Func_0200129c(struct ObjectRuntime *item);
s32 Func_02003f54(s32 actor, s32 emote, s32 frames);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void Func_02003dfc(struct ObjectRuntime *object);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
u8 *Runtime_AllocateBlock(s32 slot, s32 size);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void Func_02001278(void);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void ObjectMotion_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);
void Func_020034ec(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02003f7c(s32 a);

#define STEP(actor, x, z)                                                      \
  (Object_GetById(actor)->actionFlags &= ~1,                                   \
   Call3(ObjectMotion_SetPositionAndReset, actor, x, z),                       \
   FieldEvent_WaitTicks(1), Object_GetById(actor)->actionFlags |= 1)

#define SHOW_ITEM()                                                            \
  (glow = (struct ObjectRuntime *)Value4(Func_02003df4, 163, 0x1280000, 0,     \
                                         0x1a80000),                           \
   item = (struct ObjectRuntime *)Value4(Func_02003df4, 14, 0x1280000, 0,      \
                                         0x1a80000))

void Func_020014e0(void) {
  struct ObjectRuntime *leader, *object;
  struct ObjectRuntime *glow, *item;
  s32 answer;

  Func_02003e64();
  Func_02003fa4(0);
  if (PartyInventory_FindOwner(455) != -1) {
    leader = Object_GetById(gPartyState.currentOwner);
    if (GameFlag_Test(513)) {
      Call3(ObjectMotion_SetSpeedParameters, gPartyState.currentOwner, 0xcccc,
            0x6666);
      leader->update = Func_02000df8;
      Call3(ObjectMotion_SetPositionAndReset, gPartyState.currentOwner, 318,
            424);
      leader->update = 0;
      ObjectMotion_ArmCallback(gPartyState.currentOwner, 0x4000, 0);
      Func_02003f3c(9, 0xd000);
      Func_02003f24(MsgOhYouRe);
      Func_02003f2c(9, 0);
      Func_02003fc4();
      answer = Event_AskYesNoWithActorAnimation(gPartyState.currentOwner, 0);
      if (answer == 0) {
        Func_02003f3c(gPartyState.currentOwner, 0x8000);
        Call3(ObjectMotion_SetSpeedParameters, gPartyState.currentOwner,
              0x19999, 0xcccc);
        leader->update = Func_02000df8;
        STEP(gPartyState.currentOwner, 302, 424);
        SHOW_ITEM();
        if (glow != 0) {
          glow->motionFlags = answer;
          ObjectDispatch_SetSingleChildFlags(glow, 0);
        }
        if (item != 0) {
          item->scaleY = 0x20000;
          ObjectDispatch_SetSingleChildFlags(item, 0);
          Func_02003de4(item, 1);
          Func_02003dec(item, gData_020044f0);
        }
        STEP(gPartyState.currentOwner, 318, 424);
        leader->update = (void *)answer;
        ObjectMotion_ArmCallback(gPartyState.currentOwner, 0x6000, 0);
      } else {
        Func_02003fbc();
        Func_02003f24(MsgOhYouRe - 1);
        goto refuse;
      }
    } else {
      Func_02003f24(MsgIMSoVery);
      Func_02003f2c(9, 0);
      Func_02003f54(gPartyState.currentOwner, 262, 20);
      Call3(ObjectMotion_SetSpeedParameters, gPartyState.currentOwner, 0xcccc,
            0x6666);
      leader->update = Func_02000df8;
      Call3(ObjectMotion_SetPositionAndReset, gPartyState.currentOwner, 318,
            424);
      leader->update = 0;
      Func_02003f3c(gPartyState.currentOwner, 0x8000);
      Call3(ObjectMotion_SetSpeedParameters, gPartyState.currentOwner, 0x19999,
            0xcccc);
      leader->update = Func_02000df8;
      STEP(gPartyState.currentOwner, 302, 424);
      SHOW_ITEM();
      if (glow != 0) {
        glow->motionFlags = 0;
        ObjectDispatch_SetSingleChildFlags(glow, 0);
      }
      if (item != 0) {
        item->scaleY = 0x20000;
        ObjectDispatch_SetSingleChildFlags(item, 0);
        Func_02003de4(item, 1);
        Func_02003dec(item, gData_020044f0);
      }
      STEP(gPartyState.currentOwner, 318, 424);
      leader->update = 0;
      ObjectMotion_ArmCallback(gPartyState.currentOwner, 0x4000, 0);
      Func_02003f54(9, 0x100, 20);
      Func_02003f3c(9, 0xb000);
      object = Object_GetById(9);
      object->update = Func_02000df8;
      ObjectMotion_Launch(9, 4, 20);
      object->update = 0;
      Func_02003f2c(9, 0);
      Func_02003f3c(9, 0xd000);
      Func_02003f2c(9, 0);
      Func_02003fc4();
      if (Event_AskYesNoWithActorAnimation(gPartyState.currentOwner, 0) != 0) {
        Func_02003fbc();
        Func_02003f54(9, 265, 0);
        Func_02003f2c(9, 0);
        ObjectMotion_ArmCallback(9, 0x5000, 0);
        Func_02003f3c(gPartyState.currentOwner, 0x8000);
        leader->update = Func_02000df8;
        Call3(ObjectMotion_SetPositionAndReset, gPartyState.currentOwner, 302,
              424);
        leader->update = 0;
        if (item != 0)
          Func_02003dfc(item);
        GameFlag_SetBit(513);
        goto done;
      }
    }
    Func_0200129c(item);
  } else if (GameFlag_Test(0xaa5)) {
    ObjectMotion_SetAngleToward(9, gPartyState.currentOwner, 0);
    Func_02003f24(MsgWantToGo);
    Func_02003f2c(9, 0);
    Func_02003fc4();
    if (Event_AskYesNoWithActorAnimation(gPartyState.currentOwner, 0) == 0) {
      leader = Object_GetById(gPartyState.currentOwner);
      Func_02003fbc();
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
      Func_02003f2c(9, 0);
      Runtime_AllocateBlock(88, 664)[661] = 99;
      Motion_SetModeAndWaitAnimation(gPartyState.currentOwner, 3);
      Func_02003f3c(9, 0x5000);
      Call3(ObjectMotion_SetSpeedParameters, gPartyState.currentOwner, 0x19999,
            0xcccc);
      ObjectMotion_Launch(gPartyState.currentOwner, 6, 0);
      ObjectMotion_SetPositionAndCommit(gPartyState.currentOwner, 296, 452);
      leader->update = Func_02001278;
      Object_SetPartAttribute(Object_GetById(gPartyState.currentOwner), 15);
      Object_SetModeById(9, 3);
      FieldEvent_WaitTicks(20);
      Func_02003f2c(9, 0);
      Func_02003fc4();
      Call3(ObjectMotion_SetSpeedParameters, 9, 0xcccc, 0x6666);
      Call3(ObjectMotion_SetSpeedParameters, 10, 0xcccc, 0x6666);
      Object_GetById(9)->update = Func_02000df8;
      ObjectMotion_ResetAndSetPosition(9, 0x120, 538);
      FieldEvent_WaitTicks(40);
      Func_02003fc4();
      Func_020034ec();
      Object_GetById(10)->update = Func_02000df8;
      ObjectMotion_ResetAndSetPositionInMode2(10, 0x120, 538);
      FieldEvent_WaitTicks(20);
      *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 513;
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      Func_02003fbc();
      Func_02003f7c(3);
    } else {
      Func_02003fbc();
    refuse:
      Func_02003f2c(9, 0);
      Func_02003f3c(9, 0x5000);
    }
  } else {
    Func_02003f24(MsgIMSoVeryVery);
    Func_02003f2c(9, 0);
  }
done:
  Func_02003e6c();
}
