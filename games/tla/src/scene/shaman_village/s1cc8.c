#include "shaman_village.h"
#include "event_inline.h"
#include "scene_tables.h"

void Func_02001cc8(void) {
  struct ShamanVillageObject *object =
      ((struct ShamanVillageObject *)Actor_GetById(14));

  if (object->x >> 20 == 32 && object->z >> 20 == 11) {
    Func_020048c4();
    Event_ResolvePendingActions(0);
    object->motionFlags = 3;
    do {
      wait(1);
    } while (object->velocityY != 0);
    Sound_Play(188);
    wait(10);
    object->motionFlags = 0;
    MapGrid_CopyCellAttributes(30, 11, 1, 1, 32, 11);
    wait(1);
    GameFlag_SetBit(0x94e);
    Func_020048cc();
  }
}

void Func_02001d3c(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Sound_Play(158);
  MapGrid_CopyCellsTo(43, 40, 7, 6, 1, 2);
  Event_Wait(20);
  Actor_SetPosition(15, PIXELS(120), PIXELS(194));
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x26666, 0x13333);
  Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
  ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->actionFlags &= ~1;
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 120, 220);
  Event_Wait(1);
  {
    u8 *flags = &((struct ShamanVillageObject *)Actor_GetById(
                      gMainRoutineWork.currentOwner))
                     ->actionFlags;

    *flags |= 1;
  }
  Actor_SetSpeed(15, 0x6666, 0x3333);
  Actor_SetPositionAndReset(15, 120, 208);
  Func_02004944(16, 15);
  Func_02004944(17, 15);
  wait(1);
  Actor_SetSpeed(16, 0xcccc, 0x6666);
  Actor_SetSpeed(17, 0xcccc, 0x6666);
  Actor_ResetAndSetPositionInMode2(16, 104, 212);
  Actor_SetPositionAndReset(17, 136, 212);
  Actor_SetModeById(16, 1);
  Actor_FaceDirection(16, FACING_SOUTH, 0);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Actor_StartRepeatedMotion(15, 1);
  Event_SetMessageId((s32)MsgShamanMoapaGiveMeRoom);
  Event_ShowMessageAndWait(15, 0);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  ((struct ShamanVillageObject *)Func_080d4568())->motionFlags = 0;
  Camera_MoveWithinMapBounds(PIXELS(120), 0x320000, PIXELS(224), 1);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 120, 256);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_NORTH);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x6666, 0x3333);
  ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->actionFlags &= ~1;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 120, 264);
  Event_Wait(1);
  ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->actionFlags |= 1;
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call, 13 of 246
   * instructions differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 259, 20)
  Event_ShowMessage(0x800f, 0);
  Actor_StartRepeatedMotion(17, 1);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call, 8 of 246
   * instructions differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 259, 0)
  Event_ShowMessageAndWait(0x8011, 0);
  Actor_StartRepeatedMotion(16, 1);
  Func_080d47b4(16, 259, 0);
  Event_ShowMessageAndWait(0x8010, 0);
  Motion_SetModeAndWaitAnimation(15, 3);
  Event_ShowMessageAndWait(0x800f, 0);
  GameFlag_SetBit(0x94f);
  Func_020048cc();
}
