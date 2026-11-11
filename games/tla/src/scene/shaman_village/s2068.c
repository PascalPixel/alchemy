#include "shaman_village.h"
#include "scene_tables.h"
#include "event_inline.h"

struct ShamanVillageObject *Func_02001fec(void);
void DriftScene_SpawnObject(s32 x, s32 y, s32 z, u16 value);

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
  Event_ResolvePendingActions(0);
  Sound_Play(78);
  angle = 0xc000;
  ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->facing = angle;
  wait(1);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  ((struct ShamanVillageObject *)Func_080d4568())->motionFlags = 0;
  Camera_MoveWithinMapBounds(PIXELS(120), 0x280000, PIXELS(246), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x13333, 0x9999);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 120, 264);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  rod = Func_02001fec();
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 256, 20)
  Actor_SetSpeed(15, 0x26666, 0x13333);
  Actor_SetPositionAndReset(15, 120, 242);
  Sound_Play(29);
  Event_SetMessageId((s32)MsgShamanRecognizeShamanRod);
  Event_ShowMessageAndWait(15, 0);
  Event_Wait(20);
  if (rod != NULL)
    Func_0200481c(rod);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, angle);
  Func_02004944(18, gMainRoutineWork.currentOwner);
  Func_02004944(7, gMainRoutineWork.currentOwner);
  Func_02004944(5, gMainRoutineWork.currentOwner);
  Func_02004944(6, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(18, 0x19999, 0xcccc);
  Actor_SetSpeed(7, 0x19999, 0xcccc);
  Actor_SetSpeed(5, 0x19999, 0xcccc);
  Actor_SetSpeed(6, 0x19999, 0xcccc);
  Actor_SetMovementTypeById(18, gData_02004cfc);
  Actor_SetMovementTypeById(7, gData_02004d40);
  Actor_SetMovementTypeById(5, gData_02004d84);
  Actor_SetMovementTypeById(6, gData_02004dc8);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 256, 0)
  Actor_SetSpeed(17, 0x13333, 0x9999);
  Actor_SetPositionAndReset(17, 136, 232);
  Event_ShowMessageAndWait(17, 0);
  Actor_SetSpeed(16, 0x13333, 0x9999);
  Actor_SetPositionAndReset(16, 104, 232);
  Event_ShowMessageAndWait(16, 0);
  Motion_SetModeAndWaitAnimation(15, 3);
  Event_ShowMessageAndWait(15, 0);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 20)
  Event_ShowMessageAndWait(15, 0);
  Actor_SetAttachedEffect(18, 258);
  Event_Wait(20);
  Event_ShowMessageAndWait(18, 0);
  Actor_SetSpeed(15, 0xcccc, 0x6666);
  Actor_SetPositionAndReset(15, 120, 252);
  Event_Wait(40);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 20)
  Actor_SetPositionAndReset(15, 136, 252);
  Actor_FaceDirection(15, FACING_SOUTH, 20);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 40)
  Actor_SetPositionAndReset(15, 104, 252);
  Actor_FaceDirection(15, FACING_SOUTH, 20);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 20)
  Actor_SetPositionAndReset(15, 152, 252);
  Actor_FaceDirection(15, FACING_SOUTH, 20);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 40)
  Actor_SetPositionAndReset(15, 88, 252);
  Actor_FaceDirection(15, FACING_SOUTH, 40);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 80)
  Actor_SetPositionAndReset(15, 120, 252);
  Actor_SetPositionAndReset(15, 120, 242);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH);
  Actor_SetModeById(15, 4);
  Event_ShowMessageAndWait(15, 0);
  Actor_ArmCallback(6, 0, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(18, 256, 20)
  Actor_Jump(18, 4, 0);
  Event_ShowMessageAndWait(18, 0);
  Actor_FaceDirection(15, FACING_SOUTHEAST, 0);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 265, 20)
  Event_ShowMessageAndWait(15, 0);
  Actor_ArmCallback(6, FACING_NORTH, 0);
  Actor_ArmCallback(5, FACING_NORTH, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_ArmCallback(7, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH);
  Motion_SetModeAndWaitAnimation(15, 4);
  Event_ShowMessageAndWait(15, 0);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(6, 259, 0)
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirection(15, FACING_SOUTHWEST, 0);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(5, 257, 0)
  Event_ShowMessageAndWait(5, 0);
  Motion_SetModeAndWaitAnimation(15, 3);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(7, 265, 20)
  Event_ShowMessageAndWait(7, 0);
  Actor_FaceDirection(15, FACING_SOUTHEAST, 0);
  Actor_SetAttachedEffect(18, 258);
  Event_ShowMessageAndWait(18, 0);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 265, 40)
  Event_ShowMessageAndWait(15, 0);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 262, 20)
  Actor_FaceDirectionAndWait(17, FACING_SOUTHWEST);
  Event_ShowMessageAndWait(17, 0);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 0)
  Actor_FaceDirection(15, FACING_NORTHEAST, 20);
  Actor_SetModeById(17, 3);
  Event_ShowMessageAndWait(17, 0);
  {
    s32 y = 0x500000;
    s32 z = 0xf20000;

    DriftScene_SpawnObject(0x740000, y, z, 0xe000);
    Event_Wait(10);
    DriftScene_SpawnObject(0x7c0000, y, z, 0x2000);
    Event_Wait(30);
    /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
     * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
     * comes one row early (see the macro). */
    SHAMAN_VILLAGE_EMOTE(16, 262, 20)
    Actor_ArmCallbackAndRefresh(16, 0);
    Event_ShowMessageAndWait(16, 0);
    Actor_FaceDirectionAndWait(17, FACING_WEST);
    Motion_SetModeAndWaitAnimation(17, 3);
    /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
     * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
     * comes one row early (see the macro). */
    SHAMAN_VILLAGE_EMOTE(15, 264, 40)
    Motion_SetModeAndWaitAnimation(15, 3);
    Actor_FaceDirection(17, FACING_SOUTH, 0);
    Actor_FaceDirection(16, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(15, FACING_SOUTH);
    Event_ShowMessageAndWait(15, 0);
    Actor_SetModeById(15, 3);
    Event_ShowMessageAndWait(15, 0);
    Camera_SetSpeedParameters(0x6666, 0xccc);
    Camera_MoveWithinMapBounds(PIXELS(180), 0x200000, z, 1);
  }
  object = ((struct ShamanVillageObject *)Actor_GetById(16));
  object->unk63 = 0;
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 15);
  Actor_WatchAndSetScript(5, 15);
  Actor_WatchAndSetScript(6, 15);
  Actor_WatchAndSetScript(7, 15);
  Actor_WatchAndSetScript(18, 15);
  Actor_SetSpeed(15, 0x19999, 0xcccc);
  Actor_SetSpeed(16, 0x19999, 0xcccc);
  Actor_SetSpeed(17, 0x19999, 0xcccc);
  Actor_SetMovementTypeById(15, gData_02004e0c);
  Event_Wait(20);
  Actor_SetMovementTypeById(17, gData_02004e5c);
  Actor_SetMovementTypeById(16, gData_02004eac);
  Sound_Play(78);
  do {
    wait(1);
  } while (object->unk63 == 0);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(18);
  Actor_Stop(7);
  Actor_Stop(5);
  Actor_Stop(6);
  wait(1);
  Func_02004a2c();
  Camera_SetSpeedParameters(0x33333, 0x6666);
  Camera_MoveWithinMapBounds(PIXELS(120), -1, PIXELS(260), 1);
  Func_020049f4();
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  Actor_ArmCallbackAndRefresh(6, 0);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirectionAndWait(5, FACING_WEST);
  Event_ShowMessageAndWait(0x2005, 0);
  Event_ShowMessageAndWait(18, 0);
  Actor_FaceDirectionAndWait(18, FACING_WEST);
  Event_ShowMessageAndWait(18, 0);
  Actor_ArmCallback(7, 0, 0);
  Actor_ArmCallback(5, 0, 0);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_SetSpeed(18, 0x10000, 0x8000);
  Actor_SetMovementTypeById(6, gData_02004f08);
  Actor_SetMovementTypeById(5, gData_02004f08);
  Actor_SetMovementTypeById(7, gData_02004f08);
  Actor_SetMovementTypeAndWaitById(18, gData_02004f08);
  Event_Wait(20);
  GameFlag_SetBit(514);
  GameFlag_SetBit(0x9e1);
  Func_020048cc();
}
