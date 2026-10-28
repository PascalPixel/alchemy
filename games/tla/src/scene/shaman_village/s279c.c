#include "shaman_village.h"

void Func_0200279c(void) {
  Func_020048c4();
  Func_02004a24(0);
  Func_0200498c((s32)MsgShamanShowMeSomething);
  Func_020049a4(15, 0);
  Func_020048cc();
}

void Func_020027c0(void) {
  Func_020048c4();
  Func_02004a24(0);
  Func_0200498c((s32)MsgShamanCannotBuyGift);
  Func_020049a4(16, 0);
  Func_020048cc();
}

void Func_020027e4(void) {
  Func_020048c4();
  Func_02004a24(0);
  Func_0200498c((s32)MsgShamanShowItToMoapa);
  Func_020049a4(17, 0);
  Func_020048cc();
}

void Func_02002808(void) {
  if (GameFlag_Test(0x9fe)) {
    Import_SoundPlay(0);
    Func_020048c4();
    Func_02004a24(0);
    Func_0200498c((s32)MsgShamanFortuneGreeting);
    Actor_SetModeById(17, 5);
    Func_020049a4(17, 0);
    ShamanVillage_ObjArm(gMainRoutineWork.currentOwner, 0xc000, 20);
    Func_020011f8();
    Actor_SetModeById(17, 1);
    GameFlag_SetBit(0x203);
    Func_020048cc();
  }
}

void Func_02002878(void) {
  ShamanVillage_BlendStart(0, 0, 1, 2, 2, 17);
  GameFlag_SetBit(0x205);
}

void Func_0200289c(void) {
  ShamanVillage_ObjSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  ShamanVillage_ObjWalk(gMainRoutineWork.currentOwner, 168, 304);
  ShamanVillage_ObjArm(16, 0x4000, 0);
  ShamanVillage_ObjArm(17, 0x4000, 0);
  ShamanVillage_ObjArm(15, 0x4000, 0);
  ShamanVillage_ObjWalk(gMainRoutineWork.currentOwner, 168, 368);
  GameFlag_ClearBit(0x201);
}
