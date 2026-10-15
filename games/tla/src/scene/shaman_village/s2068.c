#include "shaman_village.h"

struct ShamanVillageObject *Func_02001fec(void);
void DriftScene_SpawnObject(s32 x, s32 y, s32 z, u16 value);
void Object_LinkObjectAndSetCallback(s32 actor, s32 target);

extern const u8 gData_02004cfc[];
extern const u8 gData_02004d40[];
extern const u8 gData_02004d84[];
extern const u8 gData_02004dc8[];
extern const u8 gData_02004e0c[];
extern const u8 gData_02004e5c[];
extern const u8 gData_02004eac[];
extern const u8 gData_02004f08[];

void Func_02002068(void) {
  struct ShamanVillageObject *rod;
  struct ShamanVillageObject *object;
  s32 angle;

  Func_020048c4();
  Func_02004a24(0);
  Engine_AudioPlayCue(78);
  angle = 0xc000;
  Object_GetById(gPartyState.currentOwner)->facing = angle;
  WaitFrames(1);
  ShamanVillage_CamSpeed(0xcccc, 0x1999);
  Func_020049fc()->motionFlags = 0;
  ShamanVillage_CamMove(0x780000, 0x280000, 0xf60000, 1);
  ShamanVillage_ObjSpeed(gPartyState.currentOwner, 0x13333, 0x9999);
  ShamanVillage_ObjWalk(gPartyState.currentOwner, 120, 264);
  Object_SetModeById(gPartyState.currentOwner, 28);
  rod = Func_02001fec();
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(15, 256, 20)
  ShamanVillage_ObjSpeed(15, 0x26666, 0x13333);
  ShamanVillage_ObjWalk(15, 120, 242);
  Engine_AudioPlayCue(29);
  Func_0200498c((s32)MsgShamanRecognizeShamanRod);
  ShamanVillage_MessagePut(15, 0);
  Battle_WaitMode0(20);
  if (rod != NULL)
    Func_0200481c(rod);
  Object_SetModeById(gPartyState.currentOwner, 1);
  ShamanVillage_ObjTurn(gPartyState.currentOwner, angle);
  Func_02004944(18, gPartyState.currentOwner);
  Func_02004944(7, gPartyState.currentOwner);
  Func_02004944(5, gPartyState.currentOwner);
  Func_02004944(6, gPartyState.currentOwner);
  WaitFrames(1);
  ShamanVillage_ObjSpeed(18, 0x19999, 0xcccc);
  ShamanVillage_ObjSpeed(7, 0x19999, 0xcccc);
  ShamanVillage_ObjSpeed(5, 0x19999, 0xcccc);
  ShamanVillage_ObjSpeed(6, 0x19999, 0xcccc);
  ObjectMotion_EnableActionAndSetCallback(18, gData_02004cfc);
  ObjectMotion_EnableActionAndSetCallback(7, gData_02004d40);
  ObjectMotion_EnableActionAndSetCallback(5, gData_02004d84);
  ObjectMotion_EnableActionAndSetCallback(6, gData_02004dc8);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(17, 256, 0)
  ShamanVillage_ObjSpeed(17, 0x13333, 0x9999);
  ShamanVillage_ObjWalk(17, 136, 232);
  ShamanVillage_MessagePut(17, 0);
  ShamanVillage_ObjSpeed(16, 0x13333, 0x9999);
  ShamanVillage_ObjWalk(16, 104, 232);
  ShamanVillage_MessagePut(16, 0);
  Motion_SetModeAndWaitAnimation(15, 3);
  ShamanVillage_MessagePut(15, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(15, 257, 20)
  ShamanVillage_MessagePut(15, 0);
  ShamanVillage_ObjEmoteWait(18, 258);
  Battle_WaitMode0(20);
  ShamanVillage_MessagePut(18, 0);
  ShamanVillage_ObjSpeed(15, 0xcccc, 0x6666);
  ShamanVillage_ObjWalk(15, 120, 252);
  Battle_WaitMode0(40);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(15, 257, 20)
  ShamanVillage_ObjWalk(15, 136, 252);
  ShamanVillage_ObjArm(15, 0x4000, 20);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(15, 257, 40)
  ShamanVillage_ObjWalk(15, 104, 252);
  ShamanVillage_ObjArm(15, 0x4000, 20);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(15, 257, 20)
  ShamanVillage_ObjWalk(15, 152, 252);
  ShamanVillage_ObjArm(15, 0x4000, 20);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(15, 257, 40)
  ShamanVillage_ObjWalk(15, 88, 252);
  ShamanVillage_ObjArm(15, 0x4000, 40);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(15, 257, 80)
  ShamanVillage_ObjWalk(15, 120, 252);
  ShamanVillage_ObjWalk(15, 120, 242);
  ShamanVillage_ObjTurn(15, 0x4000);
  Object_SetModeById(15, 4);
  ShamanVillage_MessagePut(15, 0);
  ShamanVillage_ObjArm(6, 0, 0);
  ShamanVillage_ObjArm(5, 0x8000, 0);
  ShamanVillage_ObjArm(gPartyState.currentOwner, 0, 0);
  ShamanVillage_ObjArm(7, 0x8000, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(18, 256, 20)
  ObjectMotion_Launch(18, 4, 0);
  ShamanVillage_MessagePut(18, 0);
  ShamanVillage_ObjArm(15, 0x2000, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(15, 265, 20)
  ShamanVillage_MessagePut(15, 0);
  ShamanVillage_ObjArm(6, 0xc000, 0);
  ShamanVillage_ObjArm(5, 0xc000, 0);
  ShamanVillage_ObjArm(gPartyState.currentOwner, 0xc000, 0);
  ShamanVillage_ObjArm(7, 0xc000, 0);
  ShamanVillage_ObjTurn(15, 0x4000);
  Motion_SetModeAndWaitAnimation(15, 4);
  ShamanVillage_MessagePut(15, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(6, 259, 0)
  ShamanVillage_MessagePut(6, 0);
  ShamanVillage_ObjArm(15, 0x6000, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(5, 257, 0)
  ShamanVillage_MessagePut(5, 0);
  Motion_SetModeAndWaitAnimation(15, 3);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(7, 265, 20)
  ShamanVillage_MessagePut(7, 0);
  ShamanVillage_ObjArm(15, 0x2000, 0);
  ShamanVillage_ObjEmoteWait(18, 258);
  ShamanVillage_MessagePut(18, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(15, 265, 40)
  ShamanVillage_MessagePut(15, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(17, 262, 20)
  ShamanVillage_ObjTurn(17, 0x6000);
  ShamanVillage_MessagePut(17, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(15, 257, 0)
  ShamanVillage_ObjArm(15, 0xe000, 20);
  Object_SetModeById(17, 3);
  ShamanVillage_MessagePut(17, 0);
  {
    s32 y = 0x500000;
    s32 z = 0xf20000;

    DriftScene_SpawnObject(0x740000, y, z, 0xe000);
    Battle_WaitMode0(10);
    DriftScene_SpawnObject(0x7c0000, y, z, 0x2000);
    Battle_WaitMode0(30);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see
     * SHAMAN_VILLAGE_EMOTE. */
    SHAMAN_VILLAGE_EMOTE(16, 262, 20)
    ShamanVillage_ObjTurn(16, 0);
    ShamanVillage_MessagePut(16, 0);
    ShamanVillage_ObjTurn(17, 0x8000);
    Motion_SetModeAndWaitAnimation(17, 3);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see
     * SHAMAN_VILLAGE_EMOTE. */
    SHAMAN_VILLAGE_EMOTE(15, 264, 40)
    Motion_SetModeAndWaitAnimation(15, 3);
    ShamanVillage_ObjArm(17, 0x4000, 0);
    ShamanVillage_ObjArm(16, 0x4000, 0);
    ShamanVillage_ObjTurn(15, 0x4000);
    ShamanVillage_MessagePut(15, 0);
    Object_SetModeById(15, 3);
    ShamanVillage_MessagePut(15, 0);
    ShamanVillage_CamSpeed(0x6666, 0xccc);
    ShamanVillage_CamMove(0xb40000, 0x200000, z, 1);
  }
  object = Object_GetById(16);
  object->unk63 = 0;
  Object_LinkObjectAndSetCallback(gPartyState.currentOwner, 15);
  Object_LinkObjectAndSetCallback(5, 15);
  Object_LinkObjectAndSetCallback(6, 15);
  Object_LinkObjectAndSetCallback(7, 15);
  Object_LinkObjectAndSetCallback(18, 15);
  ShamanVillage_ObjSpeed(15, 0x19999, 0xcccc);
  ShamanVillage_ObjSpeed(16, 0x19999, 0xcccc);
  ShamanVillage_ObjSpeed(17, 0x19999, 0xcccc);
  ObjectMotion_EnableActionAndSetCallback(15, gData_02004e0c);
  Battle_WaitMode0(20);
  ObjectMotion_EnableActionAndSetCallback(17, gData_02004e5c);
  ObjectMotion_EnableActionAndSetCallback(16, gData_02004eac);
  Engine_AudioPlayCue(78);
  do {
    WaitFrames(1);
  } while (object->unk63 == 0);
  ObjectMotion_EnableActionAndResetMotion(gPartyState.currentOwner);
  ObjectMotion_EnableActionAndResetMotion(18);
  ObjectMotion_EnableActionAndResetMotion(7);
  ObjectMotion_EnableActionAndResetMotion(5);
  ObjectMotion_EnableActionAndResetMotion(6);
  WaitFrames(1);
  Func_02004a2c();
  ShamanVillage_CamSpeed(0x33333, 0x6666);
  ShamanVillage_CamMove(0x780000, -1, 0x1040000, 1);
  Func_020049f4();
  Func_02004934(15, 0, 0);
  Func_02004934(17, 0, 0);
  Func_02004934(16, 0, 0);
  ShamanVillage_ObjTurn(6, 0);
  ShamanVillage_MessagePut(6, 0);
  ShamanVillage_ObjTurn(5, 0x8000);
  ShamanVillage_MessagePut(0x2005, 0);
  ShamanVillage_MessagePut(18, 0);
  ShamanVillage_ObjTurn(18, 0x8000);
  ShamanVillage_MessagePut(18, 0);
  ShamanVillage_ObjArm(7, 0, 0);
  ShamanVillage_ObjArm(5, 0, 0);
  ShamanVillage_ObjTurn(gPartyState.currentOwner, 0);
  Motion_SetModeAndWaitAnimation(gPartyState.currentOwner, 3);
  ShamanVillage_ObjSpeed(6, 0x10000, 0x8000);
  ShamanVillage_ObjSpeed(5, 0x10000, 0x8000);
  ShamanVillage_ObjSpeed(7, 0x10000, 0x8000);
  ShamanVillage_ObjSpeed(18, 0x10000, 0x8000);
  ObjectMotion_EnableActionAndSetCallback(6, gData_02004f08);
  ObjectMotion_EnableActionAndSetCallback(5, gData_02004f08);
  ObjectMotion_EnableActionAndSetCallback(7, gData_02004f08);
  Object_SetActionCallbackAndRefreshById(18, gData_02004f08);
  Battle_WaitMode0(20);
  GameFlag_SetBit(514);
  GameFlag_SetBit(0x9e1);
  Func_020048cc();
}
