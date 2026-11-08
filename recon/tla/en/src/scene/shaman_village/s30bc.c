#include "../../../../../../games/tla/src/scene/shaman_village/shaman_village.h"
#include "scene_tables.h"

#define EVENT_WORK ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)

void Actor_OffsetPositionAndResetMotion(s32 actor, s32 x, s32 z);
void Actor_WaitForAnimationChange(s32 actor);
void Actor_WatchAndSetScript(s32 actor, s32 target);
void Func_02003b24(void);

extern const u8 gData_02004f3c[];
extern const u8 gData_02004f80[];
extern const u8 gData_02004fc4[];
extern const u8 gData_02005008[];
extern const u8 gData_0200504c[];
extern const u8 gData_020050bc[];

void Func_020030bc(void) {
  u8 *work = Ram_HeapSlots->effectWork;
  struct ShamanVillageObject *object = Actor_GetById(16);

  s32 result;
  s32 owner;
  s32 angle;
  u8 *done;

  work[34] = 0;
  work[35] = 0;
  Func_020048c4();
  Func_02004a24(0);
  Import_SoundPlay(0);
  ShamanVillage_CamMove(-1, -1, -1, 0);
  ShamanVillage_CamSpeed(0x6666, 0xccc);
  ShamanVillage_CamMove(PIXELS(168), -1, PIXELS(528), 1);
  ShamanVillage_ObjSpeed(gMainRoutineWork.currentOwner, SPLIT_CONSTANT(0xcc00, 204), 0x6666);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 168, 536);
  Func_02004944(18, gMainRoutineWork.currentOwner);
  Func_02004944(7, gMainRoutineWork.currentOwner);
  Func_02004944(5, gMainRoutineWork.currentOwner);
  Func_02004944(7, gMainRoutineWork.currentOwner);
  Func_02004944(6, gMainRoutineWork.currentOwner);
  wait(1);
  ShamanVillage_ObjArm(6, FACING_NORTH, 0);
  ShamanVillage_ObjSpeed(18, 0x10000, 0x8000);
  ShamanVillage_ObjSpeed(7, 0x10000, 0x8000);
  ShamanVillage_ObjSpeed(5, 0x10000, 0x8000);
  ShamanVillage_ObjSpeed(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner,
                                          gData_02004f80);
  Actor_SetMovementTypeById(5, gData_02004fc4);
  Actor_SetMovementTypeById(18, gData_02004f3c);
  Actor_SetMovementTypeAndWaitById(7, gData_02005008);
  Func_020049bc(5, 0);
  Func_0200498c((s32)MsgShamanCheerForCompanion);
  Func_020049a4(5, 0);
  ShamanVillage_ObjTurn(6, FACING_WEST);
  Actor_SetModeById(6, 3);
  Func_020049a4(0x2006, 0);
  ShamanVillage_ObjTurn(6, FACING_NORTH);
  Func_020049bc(6, 0);
  Motion_SetModeAndWaitAnimation(6, 3);
  ShamanVillage_ObjSpeed(6, 0x6666, 0x3333);
  ShamanVillage_ObjWalk(6, 168, 524);
  ShamanVillage_ObjArm(15, FACING_NORTH, 0);
  ShamanVillage_ObjArm(17, FACING_NORTH, 0);
  ShamanVillage_ObjArm(16, FACING_NORTH, 0);
  Func_020049e4(0x19999, 0x3333);
  Func_02003b24();
  Import_SoundPlay(29);
  Func_020049e4(0x4cccc, SPLIT_CONSTANT(0x9900, 153));
  ShamanVillage_CamMove(PIXELS(168), -1, PIXELS(540), 1);
  Func_020049f4();
  FieldEvent_Wait(20);
  ShamanVillage_ObjEmoteWait(15, 258);
  FieldEvent_Wait(20);
  Func_020049a4(15, 0);
  ShamanVillage_ObjEmoteWait(17, 258);
  ShamanVillage_MessagePut(0x8011, 0);
  ShamanVillage_ObjEmoteWait(16, 258);
  ShamanVillage_MessageWait(0x8010, 0, 20);
  Motion_SetVarCbAndRefresh(15, 1);
  ShamanVillage_ObjTurn(15, FACING_NORTHWEST);
  ShamanVillage_MessagePut(0x200f, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(15, 256, 0)
  ShamanVillage_ObjTurn(15, FACING_NORTH);
  Func_020049a4(15, 0);
  Actor_Jump(6, 4, 0);
  ShamanVillage_ObjTurn(6, FACING_SOUTH);
  Actor_SetModeById(6, 3);
  Func_020049a4(6, 0);
  ShamanVillage_ObjTurn(7, FACING_SOUTHWEST);
  Func_020049a4(0x4007, 0);
  ShamanVillage_ObjTurn(15, FACING_NORTHEAST);
  Actor_SetModeById(15, 4);
  Func_020049a4(15, 0);
  ShamanVillage_ObjArm(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_ArmCallback(18, 0, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(5, 256, 20)
  ShamanVillage_ObjTurn(5, FACING_SOUTHEAST);
  Func_020049a4(5, 0);
  ShamanVillage_ObjTurn(15, FACING_NORTHWEST);
  Func_020049a4(0x200f, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(18, 257, 40)
  Func_020049a4(0x4012, 0);
  Motion_SetModeAndWaitAnimation(15, 3);
  ShamanVillage_ObjTurn(15, FACING_NORTH);
  Func_020049a4(15, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(17, 256, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(16, 256, 0)
  ShamanVillage_ObjArm(16, FACING_SOUTHEAST, 0);
  ShamanVillage_ObjTurn(17, FACING_SOUTHWEST);
  Actor_StartRepeatedMotion(17, 1);
  Func_020049a4(17, 0);
  Actor_StartRepeatedMotion(16, 1);
  Func_020049a4(16, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(6, 259, 0)
  ShamanVillage_ObjArm(6, FACING_SOUTHWEST, 20);
  ShamanVillage_ObjTurn(6, FACING_SOUTH);
  Func_020049a4(6, 0);
  ShamanVillage_ObjEmoteWait(15, 258);
  Func_020049a4(15, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(6, 259, 0)
  Actor_Jump(6, 2, 10);
  Actor_Jump(6, 4, 10);
  Func_020049a4(6, 0);
  ShamanVillage_ObjTurn(17, FACING_NORTHWEST);
  Actor_SetModeById(17, 4);
  Func_020049a4(0x8011, 0);
  ShamanVillage_ObjTurn(16, FACING_NORTHEAST);
  Actor_SetModeById(16, 4);
  Func_020049a4(0x8010, 0);
  Actor_ArmCallback(16, 0, 0);
  ShamanVillage_ObjTurn(17, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_SetModeById(16, 3);
  Motion_SetModeAndWaitAnimation(17, 3);
  Func_020049e4(SPLIT_CONSTANT(0x6600, 102), 0xccc);
  Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(560), 1);
  ShamanVillage_ObjArm(17, FACING_SOUTH, 0);
  ShamanVillage_ObjTurn(16, FACING_SOUTH);
  ShamanVillage_ObjSpeed(17, 0x10000, 0x8000);
  ShamanVillage_ObjSpeed(16, 0x10000, 0x8000);
  ShamanVillage_ObjWalk2(16, 120, 560);
  ShamanVillage_ObjWalk(17, 216, 560);
  Actor_SetModeById(17, 1);
  ShamanVillage_ObjArm(17, FACING_WEST, 0);
  Func_020049bc(16, 0);
  ShamanVillage_ObjWalk2(16, 152, 560);
  Actor_SetPositionAndReset(17, 184, 560);
  Actor_SetModeById(16, 1);
  ShamanVillage_ObjArm(17, FACING_SOUTH, 0);
  ShamanVillage_ObjArm(16, FACING_SOUTH, 0);
  ShamanVillage_ObjTurn(15, FACING_SOUTH);
  ShamanVillage_ObjSpeed(15, 0x10000, 0x8000);
  Actor_SetModeById(15, 2);
  Actor_SetModeById(17, 2);
  Actor_SetModeById(16, 2);
  Actor_OffsetPositionAndResetMotion(15, 0, 16);
  Actor_OffsetPositionAndResetMotion(17, 0, 16);
  Actor_OffsetPositionAndResetMotion(16, 0, 16);
  Actor_WaitForMoveById(15);
  Actor_SetModeById(15, 1);
  Actor_SetModeById(17, 1);
  Actor_SetModeById(16, 1);
  ShamanVillage_ObjTurn(18, 0x3000);
  Func_020049a4(18, 0);
  ShamanVillage_ObjArm(15, FACING_NORTHWEST, 0);
  ShamanVillage_ObjArm(17, FACING_NORTHWEST, 0);
  ShamanVillage_ObjArm(16, FACING_NORTHWEST, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(18, 265, 20)
  Func_020049a4(18, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(17, 257, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(16, 257, 20)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(5, 264, 40)
  Func_020049a4(5, 0);
  Actor_ArmCallback(16, 0, 0);
  ShamanVillage_ObjTurn(17, FACING_WEST);
  Motion_SetModeAndWaitAnimation(7, 3);
  Func_020049a4(7, 0);
  ShamanVillage_ObjArm(15, FACING_NORTHEAST, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(15, 259, 20)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(6, 259, 20)
  Func_020049a4(6, 0);
  ShamanVillage_ObjArm(15, FACING_NORTH, 20);
  Motion_SetModeAndWaitAnimation(15, 4);
  FieldEvent_Wait(20);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(17, 262, 0)
  ShamanVillage_ObjTurn(17, FACING_SOUTHWEST);
  Func_020049a4(17, 0);
  ShamanVillage_ObjTurn(15, FACING_NORTHEAST);
  ShamanVillage_ObjTurn(16, FACING_SOUTHEAST);
  Func_020049a4(16, 0);
  ShamanVillage_ObjEmoteWait(15, 258);
  ShamanVillage_ObjArm(15, FACING_NORTHWEST, 20);
  ShamanVillage_ObjTurn(17, FACING_WEST);
  Actor_Jump(17, 4, 10);
  Func_020049a4(17, 0);
  Func_020049bc(16, 0);
  Motion_SetModeAndWaitAnimation(16, 3);
  FieldEvent_Wait(10);
  ShamanVillage_ObjTurn(16, FACING_SOUTHEAST);
  Func_020049a4(16, 0);
  ShamanVillage_ObjArm(6, FACING_SOUTHWEST, 0);
  ShamanVillage_ObjArm(5, FACING_NORTHEAST, 40);
  ShamanVillage_ObjArm(6, FACING_SOUTH, 0);
  ShamanVillage_ObjTurn(5, FACING_SOUTH);
  Func_020049a4(5, 0);
  Actor_SetModeById(6, 3);
  Func_020049a4(6, 0);
  ShamanVillage_ObjEmoteWait(15, 258);
  ShamanVillage_ObjArm(15, FACING_NORTH, 40);
  Func_020049bc(15, FACING_NORTHEAST);
  FieldEvent_Wait(20);
  Motion_SetModeAndWaitAnimation(17, 3);
  FieldEvent_Wait(20);
  ShamanVillage_ObjTurn(15, FACING_NORTHWEST);
  FieldEvent_Wait(20);
  Motion_SetModeAndWaitAnimation(16, 3);
  FieldEvent_Wait(20);
  ShamanVillage_ObjTurn(15, FACING_NORTH);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(15, 265, 40)
  Func_020049a4(15, 0);
  Actor_SetModeById(15, 4);
  FieldMessage_ShowAtActor(15, 0);
  ShamanVillage_ObjArm(6, FACING_SOUTHEAST, 0);
  Actor_ArmCallback(5, 0, 0);
  Actor_ArmCallback(18, 0, 0);
  ShamanVillage_ObjArm(7, FACING_NORTHWEST, 0);
  result = Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  done = &object->unk63;
  owner = gMainRoutineWork.currentOwner;
  if (result == 0) {
    Actor_WaitForAnimationChange(owner);
    Motion_SetModeAndWaitAnimation(15, 4);
    Func_020049a4(15, 0);
  } else {
    Actor_WaitForAnimationChange(owner);
    Motion_SetModeAndWaitAnimation(15, 4);
    EVENT_WORK->field1c4++;
    Func_020049a4(15, 0);
    Actor_SetModeById(15, 3);
    Func_020049a4(15, 0);
  }
  ShamanVillage_ObjWalk(15, 212, SPLIT_CONSTANT(0x200, 70));
  Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(500), 1);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 15);
  Actor_WatchAndSetScript(18, 15);
  Actor_WatchAndSetScript(5, 15);
  Actor_WatchAndSetScript(7, 15);
  Actor_WatchAndSetScript(6, 15);
  *done = 0;
  Actor_SetMovementTypeById(15, gData_0200504c);
  FieldEvent_Wait(20);
  Actor_SetMovementTypeById(17, gData_0200504c);
  Actor_SetMovementTypeById(16, gData_0200504c);
  Import_SoundPlay(78);
  do {
    wait(1);
  } while (*done == 0);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(18);
  Actor_Stop(5);
  Actor_Stop(7);
  Actor_Stop(6);
  Func_02004a2c();
  Func_020049e4(0x4cccc, SPLIT_CONSTANT(0x9900, 153));
  Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(540), 1);
  FieldEvent_Wait(20);
  angle = 0x4000;
  ShamanVillage_ObjPut(15, PIXELS(224), PIXELS(280), angle);
  ShamanVillage_ObjPut(16, PIXELS(208), PIXELS(264), angle);
  ShamanVillage_ObjPut(17, PIXELS(240), PIXELS(264), angle);
  ShamanVillage_ObjArm(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_ArmCallback(18, 0, 0);
  Actor_ArmCallback(5, 0, 0);
  ShamanVillage_ObjArm(7, FACING_NORTHWEST, 0);
  Func_020049bc(6, FACING_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_SetModeById(18, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(7, 3);
  Motion_SetModeAndWaitAnimation(6, 3);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(18);
  Actor_Stop(5);
  Actor_Stop(7);
  Actor_Stop(6);
  Actor_GetById(gMainRoutineWork.currentOwner)->update = NULL;
  Actor_GetById(18)->update = NULL;
  Actor_GetById(5)->update = NULL;
  Actor_GetById(7)->update = NULL;
  Actor_GetById(6)->update = NULL;
  wait(1);
  ShamanVillage_ObjSpeed(6, 0x10000, 0x8000);
  Actor_SetMovementTypeById(7, gData_020050bc);
  Actor_SetMovementTypeById(6, gData_020050bc);
  Actor_SetMovementTypeById(5, gData_020050bc);
  Actor_SetMovementTypeAndWaitById(18, gData_020050bc);
  GameFlag_SetBit(SPLIT_CONSTANT(0x900, 76));
  Func_020048cc();
}
