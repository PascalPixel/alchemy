#include "shaman_village.h"

extern const u8 gData_02004c5c[];
extern const u8 gData_02004cac[];

void Func_02001880(void) {
  struct ShamanVillageObject *owner;
  struct ShamanVillageObject *shaman;
  struct ShamanVillageObject *target;

  owner = Actor_GetById(gAdventure.currentOwner);
  shaman = Actor_GetById(15);
  Func_020048c4();
  Func_02004a24(0);
  Actor_Stop(16);
  Actor_Stop(17);
  Actor_Stop(15);
  ShamanVillage_ObjArm(15, 0xa000, 0);
  ShamanVillage_ObjTurn(16, 0x2000);
  Motion_SetModeAndWaitAnimation(16, 3);
  ShamanVillage_ObjSpeed(16, 0x19999, 0xcccc);
  Actor_SetModeById(16, 2);
  target = Actor_GetById(15);
  if (target != NULL)
    Actor_ResetAndSetPosition(16, ((s16 *)target)[5],
                                     ((s16 *)target)[9]);
  Actor_WaitForMoveById(16);
  Func_02004934(16, 0, 0);
  Actor_SetModeById(15, 3);
  Func_0200498c((s32)MsgShamanGuardsTakeOtherPath);
  Func_020049a4(15, 0);
  Actor_SetAngleToward(gAdventure.currentOwner, 15, 0);
  ShamanVillage_ObjArm(17, 0x6000, 0);
  ShamanVillage_ObjTurn(15, 0xe000);
  Motion_SetModeAndWaitAnimation(17, 3);
  Actor_SetModeById(17, 2);
  target = Actor_GetById(15);
  if (target != NULL)
    Actor_ResetAndSetPosition(17, ((s16 *)target)[5],
                                     ((s16 *)target)[9]);
  Actor_WaitForMoveById(17);
  Func_02004934(17, 0, 0);
  ShamanVillage_ObjTurn(15, 0xa000);
  Func_020049a4(15, 0);
  ShamanVillage_CamMove(-1, -1, -1, 0);
  ShamanVillage_CamSpeed(0x19999, 0x3333);
  ShamanVillage_CamMove(0xe00000, -1, 0xc80000, 1);
  owner->unk63 = 0;
  shaman->unk63 = 0;
  Actor_SetMovementTypeById(gAdventure.currentOwner,
                                          gData_02004c5c);
  Actor_SetMovementTypeById(15, gData_02004cac);
  do {
    wait(1);
  } while (owner->unk63 == 0 || shaman->unk63 == 0);
  /* FAKEMATCH: one more use of the shaman's flag gives its address r5 and the
   * owner's r6; as plain C, 36 instructions differ in tla-en. */
  __asm__("" : : "m"(shaman->unk63));
  Func_02003040();
  ShamanVillage_CamSpeed(0xcccc, 0x1999);
  ShamanVillage_CamMove(0xe00000, -1, 0xa80000, 1);
  ShamanVillage_ObjSpeed(gAdventure.currentOwner, 0x19999, 0xcccc);
  ShamanVillage_ObjSpeed(15, 0x19999, 0xcccc);
  Actor_ResetAndSetPositionInMode2(gAdventure.currentOwner, 184, 128);
  ShamanVillage_ObjWalk(15, 264, 128);
  Actor_SetModeById(gAdventure.currentOwner, 1);
  wait(1);
  Func_02004934(gAdventure.currentOwner, 0, 0);
  Func_02004934(15, 0, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020048b4();
  Func_02004a04(11);
}

void Func_02001aa4(void) {
  struct ShamanVillageObject *owner;
  struct ShamanVillageObject *shaman;
  struct ShamanVillageObject *target;

  owner = Actor_GetById(gAdventure.currentOwner);
  shaman = Actor_GetById(15);
  Func_020048c4();
  Func_02004a24(0);
  Actor_Stop(16);
  Actor_Stop(17);
  Actor_Stop(15);
  ShamanVillage_ObjArm(15, 0xe000, 0);
  ShamanVillage_ObjTurn(17, 0x6000);
  Motion_SetModeAndWaitAnimation(17, 3);
  ShamanVillage_ObjSpeed(17, 0x19999, 0xcccc);
  Actor_SetModeById(17, 2);
  target = Actor_GetById(15);
  if (target != NULL)
    Actor_ResetAndSetPosition(17, ((s16 *)target)[5],
                                     ((s16 *)target)[9]);
  Actor_WaitForMoveById(17);
  Func_02004934(17, 0, 0);
  Actor_SetModeById(15, 3);
  Func_0200498c((s32)MsgShamanGuardsTakeOtherPath);
  Func_020049a4(15, 0);
  Actor_SetAngleToward(gAdventure.currentOwner, 15, 0);
  ShamanVillage_ObjArm(16, 0x2000, 0);
  ShamanVillage_ObjTurn(15, 0xa000);
  Motion_SetModeAndWaitAnimation(16, 3);
  Actor_SetModeById(16, 2);
  target = Actor_GetById(15);
  if (target != NULL)
    Actor_ResetAndSetPosition(16, ((s16 *)target)[5],
                                     ((s16 *)target)[9]);
  Actor_WaitForMoveById(16);
  Func_02004934(16, 0, 0);
  ShamanVillage_ObjTurn(15, 0xe000);
  Func_020049a4(15, 0);
  ShamanVillage_CamMove(-1, -1, -1, 0);
  ShamanVillage_CamSpeed(0x19999, 0x3333);
  ShamanVillage_CamMove(0xe00000, -1, 0xc80000, 1);
  owner->unk63 = 0;
  shaman->unk63 = 0;
  Actor_SetMovementTypeById(15, gData_02004c5c);
  Actor_SetMovementTypeById(gAdventure.currentOwner,
                                          gData_02004cac);
  do {
    wait(1);
  } while (owner->unk63 == 0 || shaman->unk63 == 0);
  /* FAKEMATCH: one more use of the shaman's flag ranks its address ahead of the
   * owner's (r5, then r6), as in Func_02001880 above. */
  __asm__("" : : "m"(shaman->unk63));
  Func_02003040();
  ShamanVillage_CamSpeed(0xcccc, 0x1999);
  ShamanVillage_CamMove(0xe00000, -1, 0xa80000, 1);
  ShamanVillage_ObjSpeed(gAdventure.currentOwner, 0x19999, 0xcccc);
  ShamanVillage_ObjSpeed(15, 0x19999, 0xcccc);
  Actor_ResetAndSetPositionInMode2(15, 184, 128);
  ShamanVillage_ObjWalk(gAdventure.currentOwner, 264, 128);
  Actor_SetModeById(15, 1);
  wait(1);
  Func_02004934(15, 0, 0);
  Func_02004934(gAdventure.currentOwner, 0, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020048b4();
  Func_02004a04(12);
}
