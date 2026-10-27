#include "shrine_of_the_sea_god.h"
#include "text/msg_ids.h"

void Func_020019dc(void) {
  ShrineOfTheSeaGod_CopyRect(42, 1, 15, 14, 1, 2);
  ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 15, 14);
}

void Func_02001a0c(void) {
  ShrineOfTheSeaGod_CopyRect(15, 9, 15, 14, 1, 2);
  ShrineOfTheSeaGod_BlendStart(14, 14, 1, 1, 15, 14);
}

void Func_02001a3c(void) {
  struct ShrineOfTheSeaGodObject *leader;

  Func_020039d8();
  Func_02003b08(0);
  Func_02003a70((s32)MsgFieldRopeThrownAgain);
  ShrineOfTheSeaGod_MessagePut(0x200c, 0);
  ShrineOfTheSeaGod_ObjSpeed(gPartyState.currentOwner, 0xcccc, 0x6666);
  ShrineOfTheSeaGod_ObjSpeed(14, 0xcccc, 0x6666);
  ShrineOfTheSeaGod_ObjWalk(gPartyState.currentOwner, 272, 336);
  ShrineOfTheSeaGod_ObjTurn(gPartyState.currentOwner, 0xc000, 0);
  ShrineOfTheSeaGod_ObjPlace(14, 272 << 16, 336 << 16);
  WaitFrames(1);
  ShrineOfTheSeaGod_ObjWalk(14, 256, 336);
  ShrineOfTheSeaGod_ObjTurn(14, 0, 0);
  ShrineOfTheSeaGod_ObjTurn(gPartyState.currentOwner, 0x8000, 40);
  ShrineOfTheSeaGod_ObjTurn(14, 0xd000, 0);
  ShrineOfTheSeaGod_ObjTurn(gPartyState.currentOwner, 0xc000, 0);
  ShrineOfTheSeaGod_CamMove(-1, -1, -1, 0);
  ShrineOfTheSeaGod_CamSpeed(0x4cccc, 0x9999);
  ShrineOfTheSeaGod_CamMove(288 << 16, -1, 126 << 16, 1);
  Func_02003ac8();
  ShrineOfTheSeaGod_ObjLaunch(13, 4, 20);
  ShrineOfTheSeaGod_MessagePut(0x800d, 0);
  ShrineOfTheSeaGod_ObjModeWait(12, 3);
  ShrineOfTheSeaGod_Wait(20);
  ShrineOfTheSeaGod_ObjVar(12, 2);
  ShrineOfTheSeaGod_ObjLaunch(12, 6, 80);
  ShrineOfTheSeaGod_MessagePut(0x800d, 0);
  ShrineOfTheSeaGod_ObjMode(12, 4);
  ShrineOfTheSeaGod_MessagePut(0x400c, 0);
  ShrineOfTheSeaGod_ObjTurn(13, 0x3000, 0);
  ShrineOfTheSeaGod_Jump(13, 265, 20);
  ShrineOfTheSeaGod_MessagePut(0x800d, 0);
  ShrineOfTheSeaGod_MessagePut(0x400c, 0);
  ShrineOfTheSeaGod_ObjTurn(13, 0, 0);
  ShrineOfTheSeaGod_ObjEmote(13, 258);
  ShrineOfTheSeaGod_Wait(20);
  ShrineOfTheSeaGod_MessagePut(0x800d, 0);
  ShrineOfTheSeaGod_ObjModeWait(12, 3);
  ShrineOfTheSeaGod_MessagePut(0x400c, 0);
  ShrineOfTheSeaGod_CamSpeed(0x33333, 0x6666);
  ShrineOfTheSeaGod_CamMove(420 << 16, -1, 200 << 16, 1);
  ShrineOfTheSeaGod_ObjSpeed(12, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(12, 346, 186);
  ShrineOfTheSeaGod_ObjWalk(12, 374, 192);
  ShrineOfTheSeaGod_ObjWalk(12, 392, 224);
  ShrineOfTheSeaGod_ObjWalk(12, 394, 246);
  ShrineOfTheSeaGod_ObjWalk(12, 420, 268);
  ShrineOfTheSeaGod_ObjWalk(12, 456, 268);
  ShrineOfTheSeaGod_ObjWalk(12, 488, 240);
  ShrineOfTheSeaGod_ObjWalk(12, 488, 226);
  ShrineOfTheSeaGod_ObjPlace(12, 0, 0);
  ShrineOfTheSeaGod_CamSpeed(0x4cccc, 0x9999);
  ShrineOfTheSeaGod_CamMove(264 << 16, -1, 320 << 16, 1);
  Func_02003ac8();
  ShrineOfTheSeaGod_ObjTurn(gPartyState.currentOwner, 0x8000, 0);
  ShrineOfTheSeaGod_ObjTurn(14, 0, 20);
  ShrineOfTheSeaGod_ObjMode(gPartyState.currentOwner, 3);
  ShrineOfTheSeaGod_ObjModeWait(14, 3);
  ShrineOfTheSeaGod_ObjMode(14, 2);
  leader = Object_GetById(gPartyState.currentOwner);
  if (leader != NULL)
    ObjectMotion_ResetAndSetPosition(14, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  ObjectMotion_CommitCurrentPositionAndActivate(14);
  ShrineOfTheSeaGod_ObjPlace(14, 0, 0);
  ShrineOfTheSeaGod_ObjPlace(13, 0, 0);
  GameFlag_SetBit(0x872);
  Func_020039e0();
}
