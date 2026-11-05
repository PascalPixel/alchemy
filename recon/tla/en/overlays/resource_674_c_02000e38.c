// Under -Os, 5 November: differ 58 of 445 (tla-en); older figures below predate it.
// Draft, differ 73 of 445, skeleton gap 10 removed, 11 added (tla-en, 2026-11-01)
// The Islets' trade of the milk (item 454) with object 11: with the milk in the
// party, object 11 turns and hops toward the leader (MsgSniffSniffI), the two
// meet on the side object 11 stood (its action script gData_020043f8 or
// gData_02004300), the milk is held up (Func_02000da0) and passed across, step
// by step, then object 11 hands back item 455 (MsgTradedHisMilk), which the
// party is given, and flags 0xaa1 and 534 are set. Without the milk object 11
// says MsgTheWaterHere, or after the trade MsgIDonTLike or MsgIDonTWantTo.
// Calls go through call.h as the matched Islets scene does (games/tla/src/scene/
// islets): called directly, nearby calls share 454 and 0x19999 (gap 57, 64).
// Left: the native holds a zero in r9 from before Actor_SetAngleToward (it copies
// the side flag's r3) for the trader's two update stores, with Func_02000df8 in
// r7 for its three; this compile has Func_02000df8 in r9 and builds the zeros.
// gift = 0 is there for that: the first store takes it, without it the store
// reads the side flag's slot. An explicit store of gift at the second site gives
// r9's shape but makes every leader->update = 0 take gift too and costs r11
// (differ 154); a separate zero local is propagated as a constant (differ 146).
// The native also keeps 455 in r7 for both uses; declared beside the call it goes
// to r8 (differ 70, gap 12, 14), declared here it is built twice.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

extern const u8 MsgSniffSniffI[], MsgTradedHisMilk[], MsgIDonTLike[],
    MsgIDonTWantTo[], MsgTheWaterHere[];
extern const u8 gData_020043f8[], gData_02004300[];
void Func_02003e64(void);
void Func_02003e6c(void);
void Func_02003fa4(s32 skip);
s32 PartyInventory_FindOwner(s32 item);
struct Actor *Actor_GetById(u32 id);
void Actor_SetAngleToward(s32 actor, s32 target, s32 frames);
void Func_02000df8(void);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Func_02003f24(const u8 *message);
void Func_02003f2c(s32 speaker, s32 flags);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Func_02003f3c(s32 actor, s32 angle);
struct Actor *Func_02000da0(struct Actor *holder, s32 item);
void Actor_SetMovementTypeAndWaitById(s32 actor, const void *callback);
void Actor_WatchAndSetScript(s32 actor, s32 target);
void FieldEvent_Wait(s32 frames);
void Actor_Stop(s32 actor);
void wait(s32 frames);
void Func_02003fc4(void);
void Func_02003fbc(void);
void FieldMessage_Show(const u8 *message, s32 a);
void PartyInventory_Remove(s32 item);
void PartyInventory_GiveItem(s32 item, s32 a);
void Func_02003dfc(struct Actor *object);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);

#define STEP(actor, x, z)                                                       \
  (Actor_GetById(actor)->actionFlags &= ~1,                                    \
   Call3((void (*)())Actor_SetPositionAndReset, actor, x, z),                  \
   Call1((void (*)())FieldEvent_Wait, 1),                                      \
   Actor_GetById(actor)->actionFlags |= 1)

void Func_02000e38(void) {
  struct Actor *leader, *trader, *item, *gift;
  s32 right;
  s32 giftItem = 455;
  const u8 *script;

  Func_02003e64();
  Call1((void (*)())Func_02003fa4, 0);
  if (Value1((s32 (*)())PartyInventory_FindOwner, 454) != -1) {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    trader = Actor_GetById(11);
    // Default: the trader's first update store reads it.
    gift = 0;
    trader->update = Func_02000df8;
    Call3((void (*)())Actor_SetAngleToward, 11, gMainRoutineWork.currentOwner, 0);
    right = 0;
    Call3((void (*)())Actor_Jump, 11, 4, 10);
    trader->update = 0;
    Func_02003f24(MsgSniffSniffI);
    Call2((void (*)())Func_02003f2c, 11, 0);
    if (leader->x > trader->x)
      right = 1;
    Call3((void (*)())Actor_SetSpeedParameters, gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Call3((void (*)())Actor_SetSpeedParameters, 11, 0x19999, 0xcccc);
    if (right) {
      Call3((void (*)())Actor_ArmCallback, 11, 0, 0);
      Call3((void (*)())Actor_SetPositionAndReset, gMainRoutineWork.currentOwner, 296, 856);
      Call2((void (*)())Func_02003f3c, gMainRoutineWork.currentOwner, 0x8000);
      item = (struct Actor *)Value2((s32 (*)())Func_02000da0, (s32)leader, 454);
      trader->update = Func_02000df8;
      script = gData_020043f8;
    } else {
      Call3((void (*)())Actor_ArmCallback, 11, 0x8000, 0);
      Call3((void (*)())Actor_SetPositionAndReset, gMainRoutineWork.currentOwner, 264, 856);
      Call2((void (*)())Func_02003f3c, gMainRoutineWork.currentOwner, 0);
      item = (struct Actor *)Value2((s32 (*)())Func_02000da0, (s32)leader, 454);
      trader->update = Func_02000df8;
      script = gData_02004300;
    }
    Call2((void (*)())Actor_SetMovementTypeAndWaitById, 11, (s32)script);
    trader->update = 0;
    Call2((void (*)())Func_02003f2c, 11, 0);
    Call2((void (*)())Actor_WatchAndSetScript, 11, gMainRoutineWork.currentOwner);
    if (item != 0) {
      void (*hook2)(void);
      item->collisionFlags = 9;
      Call4((void (*)())Actor_SetPositionAndResetMotion, (s32)item, 0x680000, 0, 0x3480000);
      hook2 = Func_02000df8;
      if (right) {
        leader->update = hook2;
        Call3((void (*)())Actor_SetPositionAndReset, gMainRoutineWork.currentOwner, 296, 840);
        leader->update = 0;
        Call2((void (*)())Func_02003f3c, gMainRoutineWork.currentOwner, 0x8000);
        leader->update = hook2;
        STEP(gMainRoutineWork.currentOwner, 0x120, 840);
        Call4((void (*)())Actor_SetPositionAndResetMotion, (s32)item, 0x1180000, 0, 0x3480000);
        STEP(gMainRoutineWork.currentOwner, 296, 840);
        leader->update = 0;
        Actor_Stop(gMainRoutineWork.currentOwner);
        Call1((void (*)())wait, 1);
        trader->update = hook2;
        STEP(11, 292, 844);
      } else {
        leader->update = hook2;
        Call3((void (*)())Actor_SetPositionAndReset, gMainRoutineWork.currentOwner, 264, 840);
        leader->update = 0;
        Call2((void (*)())Func_02003f3c, gMainRoutineWork.currentOwner, 0);
        leader->update = hook2;
        STEP(gMainRoutineWork.currentOwner, 0x110, 840);
        Call4((void (*)())Actor_SetPositionAndResetMotion, (s32)item, 0x1180000, 0, 0x3480000);
        STEP(gMainRoutineWork.currentOwner, 264, 840);
        leader->update = 0;
        Actor_Stop(gMainRoutineWork.currentOwner);
        Call1((void (*)())wait, 1);
        trader->update = hook2;
        STEP(11, 268, 844);
      }
    }
    gift = (struct Actor *)Value2((s32 (*)())Func_02000da0, (s32)leader, giftItem);
    STEP(11, 280, 856);
    trader->update = 0;
    Func_02003fc4();
    Call2((void (*)())FieldMessage_Show, (s32)MsgTradedHisMilk, 1);
    Call1((void (*)())PartyInventory_Remove, 454);
    Call2((void (*)())PartyInventory_GiveItem, (s32)giftItem, 0);
    Func_02003fbc();
    Actor_GetById(gMainRoutineWork.currentOwner)->angle = 0x4000;
    if (gift != 0) {
      Func_02003dfc(gift);
      Call2((void (*)())Actor_SetModeById, gMainRoutineWork.currentOwner, 1);
    }
    Call2((void (*)())Func_02003f3c, 11, 0xc000);
    Call1((void (*)())GameFlag_SetBit, 0xaa1);
    Call1((void (*)())GameFlag_SetBit, 534);
  } else {
    if (Value1((s32 (*)())GameFlag_Test, 0xaa1)) {
      Call2((void (*)())Actor_WatchAndSetScript, 11, gMainRoutineWork.currentOwner);
      if (Value1((s32 (*)())GameFlag_Test, 534))
        Func_02003f24(MsgIDonTLike);
      else
        Func_02003f24(MsgIDonTWantTo);
    } else {
      Func_02003f24(MsgTheWaterHere);
    }
    Call2((void (*)())Func_02003f2c, 11, 0);
  }
  Func_02003e6c();
}
