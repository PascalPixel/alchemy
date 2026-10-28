// Draft, differ 202 of 687: the Islets' trade of item 453 with object 10: with
// it in the party, if flag 513 is set (asked before) the camera moves and
// object 10 asks again (MsgAreYouGiving), else it notices the item
// (MsgOhThatRed), the leader steps up and shows it (object type 489, its
// picture set to the item by Func_02003e3c) and object 10 asks; a yes
// hands the item across step by step, takes item 453 and gives item 454
// (object 10 hands it back over its head) and sets flag 0xaa3, counting
// the answer in the event work's halfword 0x1c4 the first time; a no sets
// flag 513. Without the item object 10 says MsgYupANice after the trade or
// MsgIVeBeenFeeling before it.
// Left: register choice through the body (the native keeps
// &gAdventure.currentOwner in r7, -1 in r5, the messages' bases in r9 and r10
// and the bit masks in r5 and r2), and the first answer's refusal sits after
// its trade.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "call.h"

extern const u8 MsgAreYouGiving[], MsgOhThatRed[], MsgYupANice[],
    MsgIVeBeenFeeling[];
void Func_02003e64(void);
void Func_02003e6c(void);
void Func_02003fa4(s32 skip);
s32 PartyInventory_FindOwner(s32 item);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02003f64(s32 a, s32 b);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void Func_02003f24(const u8 *message);
void Func_02003f2c(s32 speaker, s32 flags);
void Func_02003f74(void);
void Func_02003fc4(void);
void Func_02003fbc(void);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
struct ObjectRuntime *Object_GetById(u32 id);
void Object_LinkObjectAndSetCallback(s32 actor, s32 target);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Func_02003f3c(s32 actor, s32 angle);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
struct ObjectRuntime *Func_02003df4(s32 type, s32 x, s32 y, s32 z);
void Func_02003e3c(struct ObjectRuntime *object, s32 item);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        u32 value);
void FieldEvent_WaitFrames(s32 frames);
void ObjectMotion_EnableActionAndResetMotion(s32 actor);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
s32 Func_02003f54(s32 actor, s32 emote, s32 frames);
void PartyInventory_Remove(s32 item);
void PartyInventory_GiveItem(s32 item, s32 a);
void Func_02003dfc(struct ObjectRuntime *object);
void FieldMessage_Show(const u8 *message, s32 a);

#define STEP(actor, x, z)                                                      \
  (Object_GetById(actor)->actionFlags &= ~1,                                   \
   Call3(ObjectMotion_SetPositionAndReset, actor, x, z),                       \
   FieldEvent_WaitFrames(1), Object_GetById(actor)->actionFlags |= 1)

/// Object 10 walks round and hands item 454 back over the leader's head.
#define HAND_BACK()                                                            \
  do {                                                                         \
    Call3(ObjectMotion_SetSpeedParameters, 10, 0xcccc, 0x6666);                \
    Call3(ObjectMotion_SetPositionAndReset, 10, 0x80, 164);                    \
    Func_02003f3c(10, 0x8000);                                                 \
    FieldEvent_WaitFrames(20);                                                 \
    Call3(ObjectMotion_SetSpeedParameters, 10, 0x19999, 0xcccc);               \
    STEP(10, 108, 168);                                                        \
    trader = Object_GetById(10);                                               \
    item = (struct ObjectRuntime *)Value4(Func_02003df4, 489, trader->x,       \
                                          trader->y + 0x180000, trader->z);    \
    if (item != 0) {                                                           \
      Object_SetModeById(gAdventure.currentOwner, 28);                        \
      item->motionFlags = 0;                                                   \
      Func_02003e3c(item, 454);                                                \
      FieldEvent_WaitFrames(20);                                               \
    }                                                                          \
    STEP(10, 116, 164);                                                        \
    Func_02003fc4();                                                           \
    PartyInventory_Remove(453);                                                \
    PartyInventory_GiveItem(454, 0);                                           \
  } while (0)

void Func_020006f4(void) {
  struct ObjectRuntime *item, *trader;

  Func_02003e64();
  Func_02003fa4(0);
  if (PartyInventory_FindOwner(453) != -1) {
    if (GameFlag_Test(513)) {
      Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
      Func_02003f64(0xcccc, 0x1999);
      Call4(Camera_MoveWithinMapBounds, 0x8c0000, -1, 0xbe0000, 1);
      ObjectMotion_SetAngleToward(10, gAdventure.currentOwner, 0);
      Func_02003f24(MsgAreYouGiving);
      Func_02003f2c(10, 0);
      Func_02003f74();
      Func_02003fc4();
      if (Event_AskYesNoWithActorAnimation(gAdventure.currentOwner, 0) == 0) {
        Object_GetById(gAdventure.currentOwner);
        Object_LinkObjectAndSetCallback(10, gAdventure.currentOwner);
        Call3(ObjectMotion_SetSpeedParameters, gAdventure.currentOwner, 0xcccc,
              0x6666);
        Call3(ObjectMotion_SetPositionAndReset, gAdventure.currentOwner, 98,
              168);
        Func_02003f3c(gAdventure.currentOwner, 0xc000);
        ObjectMotion_Launch(gAdventure.currentOwner, 4, 10);
        item = (struct ObjectRuntime *)Value4(Func_02003df4, 489, 0x680000,
                                              0x180000, 0xa00000);
        if (item != 0) {
          Func_02003e3c(item, 453);
          ObjectDispatch_SetSingleChildFlags(item, 0);
          item->motionFlags = 0;
          item->unk5c[0] = 1;
        }
        FieldEvent_WaitFrames(10);
        Func_02003fbc();
        ObjectMotion_EnableActionAndResetMotion(10);
        ObjectMotion_ArmCallback(gAdventure.currentOwner, 0, 0);
        Func_02003f54(10, 264, 20);
        Func_02003f24(MsgAreYouGiving + 1);
        Func_02003f2c(0x800a, 0);
        HAND_BACK();
        goto given;
      }
      Func_02003fbc();
      Func_02003f54(10, 265, 20);
      Func_02003f24(MsgAreYouGiving + 4);
      Func_02003f2c(0x800a, 0);
    } else {
      Func_02003f24(MsgOhThatRed);
      Func_02003f2c(10, 0);
      Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
      Func_02003f64(0xcccc, 0x1999);
      Call4(Camera_MoveWithinMapBounds, 0x8c0000, -1, 0xbe0000, 1);
      ObjectMotion_ArmCallback(10, 0x5000, 0);
      Call3(ObjectMotion_SetSpeedParameters, gAdventure.currentOwner, 0xcccc,
            0x6666);
      Call3(ObjectMotion_SetPositionAndReset, gAdventure.currentOwner, 120,
            190);
      Func_02003f3c(gAdventure.currentOwner, 0);
      STEP(gAdventure.currentOwner, 132, 190);
      item = (struct ObjectRuntime *)Value4(Func_02003df4, 489, 0x8c0000, 0,
                                            0xbe0000);
      if (item != 0) {
        Func_02003e3c(item, 453);
        ObjectDispatch_SetSingleChildFlags(item, 0);
        item->motionFlags = 0;
        item->unk5c[0] = 1;
      }
      STEP(gAdventure.currentOwner, 120, 190);
      ObjectMotion_ArmCallback(10, 0x3000, 0);
      ObjectMotion_Launch(10, 4, 10);
      Func_02003f2c(10, 0);
      Object_LinkObjectAndSetCallback(10, gAdventure.currentOwner);
      STEP(gAdventure.currentOwner, 132, 190);
      Object_SetPositionAndResetMotion(item, 0xc30000, 0, 0x730000);
      STEP(gAdventure.currentOwner, 120, 190);
      FieldEvent_WaitFrames(20);
      Call3(ObjectMotion_SetPositionAndReset, gAdventure.currentOwner, 98,
            168);
      ObjectMotion_Launch(gAdventure.currentOwner, 4, 10);
      Object_SetPositionAndResetMotion(item, 0x680000, 0x180000, 0xa00000);
      FieldEvent_WaitFrames(10);
      Func_02003fc4();
      Func_02003f3c(gAdventure.currentOwner, 0x4000);
      FieldMessage_Show(MsgOhThatRed + 2, 1);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
      Func_02003fbc();
      ObjectMotion_Launch(10, 6, 10);
      Func_02003f24(MsgOhThatRed + 3);
      Func_02003f2c(10, 0);
      ObjectMotion_EnableActionAndResetMotion(10);
      Func_02003f3c(gAdventure.currentOwner, 0);
      Func_02003fc4();
      if (Event_AskYesNoWithActorAnimation(gAdventure.currentOwner, 0) == 0) {
        Object_GetById(gAdventure.currentOwner);
        Func_02003fbc();
        Func_02003f54(10, 264, 20);
        Func_02003f24(MsgOhThatRed + 4);
        Func_02003f2c(0x800a, 0);
        HAND_BACK();
      given:
        if (item != 0) {
          Func_02003dfc(item);
          Object_SetModeById(gAdventure.currentOwner, 1);
        }
        Func_02003fbc();
        GameFlag_SetBit(0xaa3);
      } else {
        Func_02003fbc();
        Func_02003f54(10, 265, 20);
        Func_02003f24(MsgOhThatRed + 7);
        Func_02003f2c(0x800a, 0);
        Func_02003f3c(gAdventure.currentOwner, 0xc000);
        ObjectMotion_Launch(gAdventure.currentOwner, 4, 10);
        if (item != 0)
          Func_02003dfc(item);
        Func_02003f3c(gAdventure.currentOwner, 0x4000);
        GameFlag_SetBit(513);
      }
    }
  } else {
    if (GameFlag_Test(0xaa3))
      Func_02003f24(MsgYupANice);
    else
      Func_02003f24(MsgIVeBeenFeeling);
    Func_02003f2c(10, 0);
  }
  Object_SetModeById(10, 4);
  Func_02003e6c();
}
