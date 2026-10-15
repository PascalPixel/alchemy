#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void Func_02003e64();
void Func_02003e6c();
void Func_02003f24();
void Func_02003f2c();
void Func_02003fa4();
void GameFlag_Test();
void ObjectMotion_EnableActionAndResetMotion();
void ObjectMotion_SetAngleToward();
void Object_LinkObjectAndSetCallback();
void WaitFrames();
struct ObjectRuntime *Object_GetById();
void Func_02003ff4();
void Battle_WaitMode0();
void Func_02003f3c();
void Func_02003f5c();
void GameFlag_SetBit();
void ObjectMotion_ArmCallback();
void ObjectMotion_Launch();
void Func_02003df4();
void Func_02003dfc();
void Func_02003e3c();
void Func_02003fbc();
void Func_02003fc4();
void ObjectDispatch_SetSingleChildFlags();
void PartyInventory_FindOwner();
void PartyInventory_GiveItem();
void PartyInventory_Remove();
void UiText_ShowPositionedMessageAndWait();

void Func_02000210(void) {
  s32 result0;
  s32 result1;

  ((void (*)())Func_02003e64)();
  Call1((void (*)())Func_02003fa4, 0);
  result0 = Value1((s32 (*)())GameFlag_Test, 0xaa2);
  if (result0 == 0)
    goto L_02008260;
  result1 = Value1((s32 (*)())GameFlag_Test, 0x200);
  if (result1 == 0)
    goto L_02008250;
  Call3((void (*)())ObjectMotion_SetAngleToward, 9, gPartyState.currentOwner,
        0);
  Call1((void (*)())Func_02003f24, (s32)MsgPenPenPen);
  goto L_02008256;
L_02008250:
  Call1((void (*)())Func_02003f24, (s32)MsgPenPen2);
L_02008256:
  Call2((void (*)())Func_02003f2c, 9, 0);
  goto L_02008294;
L_02008260:
  Call1((void (*)())ObjectMotion_EnableActionAndResetMotion, 9);
  Call1((void (*)())WaitFrames, 1);
  Call3((void (*)())ObjectMotion_SetAngleToward, 9, gPartyState.currentOwner,
        0);
  Call1((void (*)())Func_02003f24, (s32)MsgPenPen3);
  Call2((void (*)())Func_02003f2c, 9, 0);
  Call2((void (*)())Object_LinkObjectAndSetCallback, 9, 19);
L_02008294:
  ((void (*)())Func_02003e6c)();
  return;
}

void Func_020002ac(void) {
  s32 result0;
  s32 result1;

  ((void (*)())Func_02003e64)();
  Call1((void (*)())Func_02003fa4, 0);
  result0 = Value1((s32 (*)())GameFlag_Test, 0xaa2);
  if (result0 == 0)
    goto L_020082ea;
  result1 = Value1((s32 (*)())GameFlag_Test, 0x200);
  if (result1 == 0)
    goto L_020082da;
  Call1((void (*)())Func_02003f24, (s32)MsgThereIsNothing);
  goto L_020082e0;
L_020082da:
  Call1((void (*)())Func_02003f24, (s32)MsgIMSoHappy);
L_020082e0:
  Call2((void (*)())Func_02003f2c, 9, 0);
  goto L_020082f8;
L_020082ea:
  Call1((void (*)())Func_02003f24, (s32)MsgMyDarlingPengulina);
  Call2((void (*)())Func_02003f2c, 9, 0);
L_020082f8:
  ((void (*)())Func_02003e6c)();
  return;
}

void Func_0200030c(void) {
  s32 result0;
  s32 result1;

  ((void (*)())Func_02003e64)();
  Call1((void (*)())Func_02003fa4, 0);
  result0 = Value1((s32 (*)())GameFlag_Test, 0xaa2);
  if (result0 == 0)
    goto L_0200835c;
  result1 = Value1((s32 (*)())GameFlag_Test, 0x200);
  if (result1 == 0)
    goto L_0200834c;
  Call3((void (*)())ObjectMotion_SetAngleToward, 19, gPartyState.currentOwner,
        0);
  Call1((void (*)())Func_02003f24, (s32)MsgPenPenPehnnnn);
  goto L_02008352;
L_0200834c:
  Call1((void (*)())Func_02003f24, (s32)MsgPenPen2);
L_02008352:
  Call2((void (*)())Func_02003f2c, 19, 0);
  goto L_0200836a;
L_0200835c:
  Call1((void (*)())Func_02003f24, (s32)MsgPenPenPehnnn);
  Call2((void (*)())Func_02003f2c, 19, 0);
L_0200836a:
  ((void (*)())Func_02003e6c)();
  return;
}

void Func_02000380(void) {
  s32 result0;
  s32 result1;

  ((void (*)())Func_02003e64)();
  Call1((void (*)())Func_02003fa4, 0);
  result0 = Value1((s32 (*)())GameFlag_Test, 0xaa2);
  if (result0 == 0)
    goto L_020083be;
  result1 = Value1((s32 (*)())GameFlag_Test, 0x200);
  if (result1 == 0)
    goto L_020083ae;
  Call1((void (*)())Func_02003f24, (s32)MsgPengusIsPretty);
  goto L_020083b4;
L_020083ae:
  Call1((void (*)())Func_02003f24, (s32)MsgIMSoGlad);
L_020083b4:
  Call2((void (*)())Func_02003f2c, 19, 0);
  goto L_020083cc;
L_020083be:
  Call1((void (*)())Func_02003f24, (s32)MsgOhhMyLeg);
  Call2((void (*)())Func_02003f2c, 19, 0);
L_020083cc:
  return;
}

s32 Func_020003dc(void) {
  u16 turn;
  s32 facing;

  turn = Object_GetById(gPartyState.currentOwner)->angle + 0x5fff;
  facing = 1;
  if (turn >= 0x3fff)
    facing = 0;
  return facing;
}

void Func_0200040c(void) {
  s32 result0;

  result0 = Value0((s32 (*)())Func_020003dc);
  if (result0 == 0)
    goto L_0200841e;
  Call1((void (*)())Func_02003ff4, 21);
  goto L_0200843a;
L_0200841e:
  ((void (*)())Func_02003e64)();
  Call1((void (*)())Func_02003fa4, 0);
  Call1((void (*)())Func_02003f24, (s32)MsgIAmHere);
  Call2((void (*)())Func_02003f2c, 21, 0);
  ((void (*)())Func_02003e6c)();
L_0200843a:
  return;
}

void Func_02000440(void) {
  s32 result0;

  ((void (*)())Func_02003e64)();
  Call1((void (*)())Func_02003fa4, 0);
  result0 = Value1((s32 (*)())GameFlag_Test, 0x214);
  if (result0 != 0)
    goto L_020084b6;
  Call1((void (*)())Func_02003f24, (s32)MsgWeLikeThis);
  Call2((void (*)())Func_02003f2c, 8, 0);
  Call2((void (*)())Func_02003f5c, 9, 0x102);
  Call3((void (*)())ObjectMotion_ArmCallback, 9, 0x8000, 0);
  Call2((void (*)())Func_02003f2c, 9, 0);
  Call2((void (*)())Func_02003f3c, 8, 0);
  Call3((void (*)())ObjectMotion_Launch, 9, 4, 10);
  Call2((void (*)())Func_02003f2c, 9, 0);
  Call2((void (*)())Func_02003f5c, 9, 0x102);
  Call1((void (*)())Battle_WaitMode0, 20);
  Call1((void (*)())GameFlag_SetBit, 0x214);
L_020084b6:
  Call3((void (*)())ObjectMotion_SetAngleToward, 8, gPartyState.currentOwner,
        0);
  Call1((void (*)())Func_02003f24, (s32)MsgLookWeRe);
  Call2((void (*)())Func_02003f2c, 8, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 9, 0x5000, 0);
  ((void (*)())Func_02003e6c)();
  return;
}

void Func_020004f4(void) {
  ((void (*)())Func_02003e64)();
  Call1((void (*)())Func_02003fa4, 0);
  Call1((void (*)())Func_02003f24, (s32)MsgTheEasternSea);
  Call2((void (*)())Func_02003f2c, 9, 0);
  Call2((void (*)())Func_02003f2c, 9, 0);
  ((void (*)())Func_02003e6c)();
  return;
}

void Func_02000520(void) {
  s32 result0;

  ((void (*)())Func_02003e64)();
  Call1((void (*)())Func_02003fa4, 0);
  result0 = Value1((s32 (*)())GameFlag_Test, 0xaa4);
  if (result0 == 0)
    goto L_0200854a;
  Call2((void (*)())Object_LinkObjectAndSetCallback, 10,
        gPartyState.currentOwner);
L_0200854a:
  Call1((void (*)())Func_02003f24, (s32)MsgChirpChirp2);
  Call2((void (*)())Func_02003f2c, 10, 0);
  ((void (*)())Func_02003e6c)();
  return;
}

// Object 10's talk, and its exchange of the party's item 0x1c4 for item 0x1c5.
void Func_02000568(void) {
  struct ObjectRuntime *leader;
  struct ObjectRuntime *object;
  struct ObjectRuntime *spawned;

  ((void (*)())Func_02003e64)();
  Call1((void (*)())Func_02003fa4, 0);
  if (Value1((s32 (*)())PartyInventory_FindOwner, 0x1c4) == -1) {
    if (Value1((s32 (*)())GameFlag_Test, 0xaa4)) {
      Call2((void (*)())Object_LinkObjectAndSetCallback, 10,
            gPartyState.currentOwner);
      Call1((void (*)())Func_02003f24, (s32)MsgWhatDoYou);
    } else {
      Call1((void (*)())Func_02003f24, (s32)MsgILoveFeeling);
    }
    Call2((void (*)())Func_02003f2c, 10, 0);
  } else {
    leader = Object_GetById(gPartyState.currentOwner);
    object = Object_GetById(10);
    Call3((void (*)())ObjectMotion_SetAngleToward, 10, gPartyState.currentOwner,
          0);
    Call3((void (*)())ObjectMotion_Launch, 10, 4, 10);
    Call1((void (*)())Func_02003f24, (s32)MsgOhMyThat);
    Call2((void (*)())Func_02003f2c, 10, 0);
    Call2((void (*)())Object_SetModeById, 10, 4);
    Call4((void (*)())Object_SetPositionAndResetMotion, (s32)object,
          (leader->x + object->x) / 2, leader->y, (leader->z + object->z) / 2);
    Call1((void (*)())WaitFrames, 20);
    Object_SetPositionAndResetMotion(object, 0x780000, 0x800000, 0x1380000);
    Call2((void (*)())Object_SetModeById, 10, 5);
    Call1((void (*)())WaitFrames, 10);
    Call2((void (*)())Func_02003f3c, gPartyState.currentOwner, 0x4000);
    spawned = (struct ObjectRuntime *)Value4((s32 (*)())Func_02003df4, 0x1e9,
                                             leader->x, 0x980000, leader->z);
    if (spawned != 0) {
      Call2((void (*)())Object_SetModeById, gPartyState.currentOwner, 28);
      Call2((void (*)())Func_02003e3c, (s32)spawned, 0x1c5);
      ObjectDispatch_SetSingleChildFlags(spawned, 0);
      spawned->motionFlags = 0;
    }
    Call1((void (*)())Battle_WaitMode0, 10);
    ((void (*)())Func_02003fc4)();
    Call2((void (*)())UiText_ShowPositionedMessageAndWait, (s32)MsgOhMyThat + 1,
          1);
    Call1((void (*)())PartyInventory_Remove, 0x1c4);
    Value2((s32 (*)())PartyInventory_GiveItem, 0x1c5, 0);
    ((void (*)())Func_02003fbc)();
    if (spawned != 0) {
      Func_02003dfc(spawned);
      Call2((void (*)())Object_SetModeById, gPartyState.currentOwner, 1);
    }
    Call2((void (*)())Func_02003f3c, 10, 0);
    Call1((void (*)())GameFlag_SetBit, 0xaa4);
  }
  ((void (*)())Func_02003e6c)();
}
