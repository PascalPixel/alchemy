// Draft, differ 253 of 445: the Islets' trade of the milk (item 454) with
// object 11: with the milk in the party, object 11 turns and hops toward
// the leader (MsgSniffSniffI), the two meet on the side object 11 stood
// (its action script gData_020043f8 or gData_02004300), the milk is held
// up (Func_02000da0) and passed across, step by step, then object 11 hands
// back item 455 (MsgTradedHisMilk), which the party is given, and flags
// 0xaa1 and 534 are set. Without the milk object 11 says
// MsgTheWaterHere, or after the trade MsgIDonTLike or MsgIDonTWantTo.
// Left: the native keeps the side flag on the stack and stores it as the zero for the update hooks, keeps the trader in r10, the item in r8, Func_02000df8 in r7 then r11, &gAdventure.currentOwner in r5 then r7 and the bit masks in r5 and r9, and shares the trader's last step's tail between the two sides.
#include "types.h"
#include "object_runtime.h"
#include "adventure.h"

extern const u8 MsgSniffSniffI[], MsgTradedHisMilk[], MsgIDonTLike[],
    MsgIDonTWantTo[], MsgTheWaterHere[];
extern const u8 gData_020043f8[], gData_02004300[];
void Func_02003e64(void);
void Func_02003e6c(void);
void Func_02003fa4(s32 skip);
s32 PartyInventory_FindOwner(s32 item);
struct ObjectRuntime *Object_GetById(u32 id);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void Func_02000df8(void);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void Func_02003f24(const u8 *message);
void Func_02003f2c(s32 speaker, s32 flags);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Func_02003f3c(s32 actor, s32 angle);
struct ObjectRuntime *Func_02000da0(struct ObjectRuntime *holder, s32 item);
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *callback);
void Object_LinkObjectAndSetCallback(s32 actor, s32 target);
void FieldEvent_WaitFrames(s32 frames);
void ObjectMotion_EnableActionAndResetMotion(s32 actor);
void WaitFrames(s32 frames);
void Func_02003fc4(void);
void Func_02003fbc(void);
void UiText_ShowPositionedMessageAndWait(const u8 *message, s32 a);
void PartyInventory_Remove(s32 item);
void PartyInventory_GiveItem(s32 item, s32 a);
void Func_02003dfc(struct ObjectRuntime *object);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);

#define STEP(actor, x, z)                                                      \
  (Object_GetById(actor)->actionFlags &= ~1,                                   \
   ObjectMotion_SetPositionAndReset(actor, x, z), FieldEvent_WaitFrames(1),    \
   Object_GetById(actor)->actionFlags |= 1)

void Func_02000e38(void) {
  struct ObjectRuntime *leader, *trader, *item, *gift;
  s32 right;

  Func_02003e64();
  Func_02003fa4(0);
  if (PartyInventory_FindOwner(454) != -1) {
    leader = Object_GetById(gAdventure.currentOwner);
    trader = Object_GetById(11);
    right = 0;
    ObjectMotion_SetAngleToward(11, gAdventure.currentOwner, 0);
    trader->update = Func_02000df8;
    ObjectMotion_Launch(11, 4, 10);
    trader->update = 0;
    Func_02003f24(MsgSniffSniffI);
    Func_02003f2c(11, 0);
    if (leader->x > trader->x)
      right = 1;
    ObjectMotion_SetSpeedParameters(gAdventure.currentOwner, 0x19999, 0xcccc);
    ObjectMotion_SetSpeedParameters(11, 0x19999, 0xcccc);
    if (right) {
      ObjectMotion_ArmCallback(11, 0, 0);
      ObjectMotion_SetPositionAndReset(gAdventure.currentOwner, 296, 856);
      Func_02003f3c(gAdventure.currentOwner, 0x8000);
      item = Func_02000da0(leader, 454);
      trader->update = Func_02000df8;
      Object_SetActionCallbackAndRefreshById(11, gData_020043f8);
    } else {
      ObjectMotion_ArmCallback(11, 0x8000, 0);
      ObjectMotion_SetPositionAndReset(gAdventure.currentOwner, 264, 856);
      Func_02003f3c(gAdventure.currentOwner, 0);
      item = Func_02000da0(leader, 454);
      trader->update = Func_02000df8;
      Object_SetActionCallbackAndRefreshById(11, gData_02004300);
    }
    trader->update = 0;
    Func_02003f2c(11, 0);
    Object_LinkObjectAndSetCallback(11, gAdventure.currentOwner);
    if (item != 0) {
      item->collisionFlags = 9;
      Object_SetPositionAndResetMotion(item, 0x680000, 0, 0x3480000);
      if (right) {
        leader->update = Func_02000df8;
        ObjectMotion_SetPositionAndReset(gAdventure.currentOwner, 296, 840);
        leader->update = 0;
        Func_02003f3c(gAdventure.currentOwner, 0x8000);
        leader->update = Func_02000df8;
        STEP(gAdventure.currentOwner, 0x120, 840);
        Object_SetPositionAndResetMotion(item, 0x1180000, 0, 0x3480000);
        STEP(gAdventure.currentOwner, 296, 840);
        leader->update = 0;
        ObjectMotion_EnableActionAndResetMotion(gAdventure.currentOwner);
        WaitFrames(1);
        trader->update = Func_02000df8;
        STEP(11, 292, 844);
      } else {
        leader->update = Func_02000df8;
        ObjectMotion_SetPositionAndReset(gAdventure.currentOwner, 264, 840);
        leader->update = 0;
        Func_02003f3c(gAdventure.currentOwner, 0);
        leader->update = Func_02000df8;
        STEP(gAdventure.currentOwner, 0x110, 840);
        Object_SetPositionAndResetMotion(item, 0x1180000, 0, 0x3480000);
        STEP(gAdventure.currentOwner, 264, 840);
        leader->update = 0;
        ObjectMotion_EnableActionAndResetMotion(gAdventure.currentOwner);
        WaitFrames(1);
        trader->update = Func_02000df8;
        STEP(11, 268, 844);
      }
    }
    gift = Func_02000da0(leader, 455);
    STEP(11, 280, 856);
    trader->update = 0;
    Func_02003fc4();
    UiText_ShowPositionedMessageAndWait(MsgTradedHisMilk, 1);
    PartyInventory_Remove(454);
    PartyInventory_GiveItem(455, 0);
    Func_02003fbc();
    Object_GetById(gAdventure.currentOwner)->angle = 0x4000;
    if (gift != 0) {
      Func_02003dfc(gift);
      Object_SetModeById(gAdventure.currentOwner, 1);
    }
    Func_02003f3c(11, 0xc000);
    GameFlag_SetBit(0xaa1);
    GameFlag_SetBit(534);
  } else {
    if (GameFlag_Test(0xaa1)) {
      Object_LinkObjectAndSetCallback(11, gAdventure.currentOwner);
      if (GameFlag_Test(534))
        Func_02003f24(MsgIDonTLike);
      else
        Func_02003f24(MsgIDonTWantTo);
    } else {
      Func_02003f24(MsgTheWaterHere);
    }
    Func_02003f2c(11, 0);
  }
  Func_02003e6c();
}
