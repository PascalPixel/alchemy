#include "shaman_village.h"
#include "heap_release.h"
#include "resource.h"
#include "dma.h"
#include "io_reg.h"
#include "edition.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "types.h"
#include "input.h"

// The European editions' lettering sheet has one more palette row.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||                      \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define LETTERING_PALETTE_ROWS 6
#else
#define LETTERING_PALETTE_ROWS 5
#endif

extern s16 gData_02005158[];
extern u8 gData_02004bfe[];
extern char ResourceId_LetteringBanner;

void Func_0200479c(const void *source, void *destination);

/// The decoded lettering sheet: its palette rows, then eight graphics of 32
/// tiles each.
struct LetteringSheet {
  u8 palettes[LETTERING_PALETTE_ROWS][32];
  u8 graphics[8][1024];
};

/// Decodes the lettering sheet, sends one graphic's palette to OBJ palette 15
/// and loads its tiles into a VRAM block of its own. Graphic 8 has a palette of
/// its own and graphic 4's tiles.
void Func_02002fb0(s32 graphic) {
  // BUG: this holds seven graphics of the sheet's eight, so decoding the sheet
  // writes 1024 bytes past the block.
#ifdef BUGFIX
  struct LetteringSheet *sheet = Heap_AllocEwram(sizeof(struct LetteringSheet));
#else
  struct LetteringSheet *sheet =
      Heap_AllocEwram((224 + LETTERING_PALETTE_ROWS) * 32);
#endif
  struct DmaRegisters volatile *dma;
  s32 palette;

  if (gData_02005158[0] == -1)
    gData_02005158[0] = Resource_FindFreeEntry();
  palette = gData_02004bfe[graphic];
  if (graphic == 8)
    graphic = 4;
  Func_0200479c(Resource_GetTableEntry((s32)&ResourceId_LetteringBanner),
                sheet);
  Dma_Set(sheet->palettes[0] + palette, (void *)&OBJ_PLTT_COLOR(15, 0),
          (DMA_ENABLE | DMA_32BIT) << 16 | 32 / 4, REG_DMA3);
  Vram_Load(gData_02005158[0], sizeof(sheet->graphics[0]),
            sheet->graphics[graphic]);
  dma = (struct DmaRegisters volatile *)REG_DMA3;
  while ((s32)dma->control < 0)
    ;
  Heap_Release(sheet);
}

extern u16 gData_0200515a[];

void Func_02002f20(s32 a, s32 b);
void Func_02002fb0(s32 a);

void Func_02003040(void) {
  gData_0200515a[15] = 180;
  Event_Wait(30);
  Sound_Play(78);
  Func_02002fb0(5);
  Func_02002f20(2, 0);
  Sound_Play(236);
  Event_Wait(60);
  Func_02002f20(2, 1);
  Sound_Play(236);
  Event_Wait(60);
  Func_02002fb0(6);
  Func_02002f20(2, 0);
  Sound_Play(236);
  Event_Wait(60);
  Func_02002fb0(7);
  Func_02002f20(4, 0);
  Sound_Play(237);
  Sound_Play(76);
}

#define EVENT_WORK ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)

void Actor_WaitForAnimationChange(s32 actor);
void Func_02003b24(void);

extern const u8 gData_02004f3c[];
extern const u8 gData_02004f80[];
extern const u8 gData_02004fc4[];
extern const u8 gData_02005008[];
extern const u8 gData_0200504c[];
extern const u8 gData_020050bc[];

void Func_020030bc(void) {
  u8 *work = Ram_HeapSlots->effectWork;
  struct ShamanVillageObject *object = ((struct ShamanVillageObject *)Actor_GetById(16));

  s32 result;
  s32 owner;
  s32 angle;
  u8 *done;

  work[34] = 0;
  work[35] = 0;
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Sound_Play(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_SetSpeed(0x6666, 0xccc);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(528), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 168, 536);
  Func_02004944(18, gMainRoutineWork.currentOwner);
  Func_02004944(7, gMainRoutineWork.currentOwner);
  Func_02004944(5, gMainRoutineWork.currentOwner);
  Func_02004944(7, gMainRoutineWork.currentOwner);
  Func_02004944(6, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_SetSpeed(18, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner,
                                          gData_02004f80);
  Actor_SetMovementTypeById(5, gData_02004fc4);
  Actor_SetMovementTypeById(18, gData_02004f3c);
  Actor_SetMovementTypeAndWaitById(7, gData_02005008);
  Actor_ArmCallbackAndRefresh(5, 0);
  Event_SetMessageId((s32)MsgShamanCheerForCompanion);
  Event_ShowMessageAndWait(5, 0);
  Actor_FaceDirectionAndWait(6, FACING_WEST);
  Actor_SetModeById(6, 3);
  Event_ShowMessageAndWait(0x2006, 0);
  Actor_FaceDirectionAndWait(6, FACING_NORTH);
  Actor_ArmCallbackAndRefresh(6, 0);
  Motion_SetModeAndWaitAnimation(6, 3);
  Actor_SetSpeed(6, 0x6666, 0x3333);
  Actor_WalkToAndWait(6, 168, 524);
  Actor_FaceDirection(15, FACING_NORTH, 0);
  Actor_FaceDirection(17, FACING_NORTH, 0);
  Actor_FaceDirection(16, FACING_NORTH, 0);
  Camera_SetSpeedParameters(0x19999, 0x3333);
  Func_02003b24();
  Sound_Play(29);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(540), 1);
  Func_020049f4();
  Event_Wait(20);
  Actor_SetAttachedEffect(15, 258);
  Event_Wait(20);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetAttachedEffect(17, 258);
  Event_ShowMessage(0x8011, 0);
  Actor_SetAttachedEffect(16, 258);
  FieldEvent_ShowMessageThenWait(0x8010, 0, 20);
  Motion_SetVarCbAndRefresh(15, 1);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Event_ShowMessage(0x200f, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 91 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 256, 0)
  Actor_FaceDirectionAndWait(15, FACING_NORTH);
  Event_ShowMessageAndWait(15, 0);
  Actor_Jump(6, 4, 0);
  Actor_FaceDirectionAndWait(6, FACING_SOUTH);
  Actor_SetModeById(6, 3);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Event_ShowMessageAndWait(0x4007, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHEAST);
  Actor_SetModeById(15, 4);
  Event_ShowMessageAndWait(15, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_ArmCallback(18, 0, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 88 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(5, 256, 20)
  Actor_FaceDirectionAndWait(5, FACING_SOUTHEAST);
  Event_ShowMessageAndWait(5, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Event_ShowMessageAndWait(0x200f, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 30 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(18, 257, 40)
  Event_ShowMessageAndWait(0x4012, 0);
  Motion_SetModeAndWaitAnimation(15, 3);
  Actor_FaceDirectionAndWait(15, FACING_NORTH);
  Event_ShowMessageAndWait(15, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 83 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 256, 0)
  Func_080d47b4(16, 256, 0);
  Actor_FaceDirection(16, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(17, FACING_SOUTHWEST);
  Actor_StartRepeatedMotion(17, 1);
  Event_ShowMessageAndWait(17, 0);
  Actor_StartRepeatedMotion(16, 1);
  Event_ShowMessageAndWait(16, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 29 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(6, 259, 0)
  Actor_FaceDirection(6, FACING_SOUTHWEST, 20);
  Actor_FaceDirectionAndWait(6, FACING_SOUTH);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetAttachedEffect(15, 258);
  Event_ShowMessageAndWait(15, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 30 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(6, 259, 0)
  Actor_Jump(6, 2, 10);
  Actor_Jump(6, 4, 10);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirectionAndWait(17, FACING_NORTHWEST);
  Actor_SetModeById(17, 4);
  Event_ShowMessageAndWait(0x8011, 0);
  Actor_FaceDirectionAndWait(16, FACING_NORTHEAST);
  Actor_SetModeById(16, 4);
  Event_ShowMessageAndWait(0x8010, 0);
  Actor_ArmCallback(16, 0, 0);
  Actor_FaceDirectionAndWait(17, FACING_WEST);
  Event_Wait(10);
  Actor_SetModeById(16, 3);
  Motion_SetModeAndWaitAnimation(17, 3);
  Camera_SetSpeedParameters(0x6666, 0xccc);
  Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(560), 1);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(16, FACING_SOUTH);
  Actor_SetSpeed(17, 0x10000, 0x8000);
  Actor_SetSpeed(16, 0x10000, 0x8000);
  Actor_WalkTo(16, 120, 560);
  Actor_WalkToAndWait(17, 216, 560);
  Actor_SetModeById(17, 1);
  Actor_FaceDirection(17, FACING_WEST, 0);
  Actor_ArmCallbackAndRefresh(16, 0);
  Actor_WalkTo(16, 152, 560);
  Actor_SetPositionAndReset(17, 184, 560);
  Actor_SetModeById(16, 1);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Actor_FaceDirection(16, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH);
  Actor_SetSpeed(15, 0x10000, 0x8000);
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
  Actor_FaceDirectionAndWait(18, 0x3000);
  Event_ShowMessageAndWait(18, 0);
  Actor_FaceDirection(15, FACING_NORTHWEST, 0);
  Actor_FaceDirection(17, FACING_NORTHWEST, 0);
  Actor_FaceDirection(16, FACING_NORTHWEST, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 23 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(18, 265, 20)
  Event_ShowMessageAndWait(18, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 83 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 257, 0)
  Func_080d47b4(16, 257, 20);
  Func_080d47b4(5, 264, 40);
  Event_ShowMessageAndWait(5, 0);
  Actor_ArmCallback(16, 0, 0);
  Actor_FaceDirectionAndWait(17, FACING_WEST);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_ShowMessageAndWait(7, 0);
  Actor_FaceDirection(15, FACING_NORTHEAST, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 83 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 259, 20)
  Func_080d47b4(6, 259, 20);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirection(15, FACING_NORTH, 20);
  Motion_SetModeAndWaitAnimation(15, 4);
  Event_Wait(20);
  Func_080d47b4(17, 262, 0);
  Actor_FaceDirectionAndWait(17, FACING_SOUTHWEST);
  Event_ShowMessageAndWait(17, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHEAST);
  Actor_FaceDirectionAndWait(16, FACING_SOUTHEAST);
  Event_ShowMessageAndWait(16, 0);
  Actor_SetAttachedEffect(15, 258);
  Actor_FaceDirection(15, FACING_NORTHWEST, 20);
  Actor_FaceDirectionAndWait(17, FACING_WEST);
  Actor_Jump(17, 4, 10);
  Event_ShowMessageAndWait(17, 0);
  Actor_ArmCallbackAndRefresh(16, 0);
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, FACING_SOUTHEAST);
  Event_ShowMessageAndWait(16, 0);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 40);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(5, FACING_SOUTH);
  Event_ShowMessageAndWait(5, 0);
  Actor_SetModeById(6, 3);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetAttachedEffect(15, 258);
  Actor_FaceDirection(15, FACING_NORTH, 40);
  Actor_ArmCallbackAndRefresh(15, FACING_NORTHEAST);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(15, FACING_NORTH);
  Func_080d47b4(15, 265, 40);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetModeById(15, 4);
  FieldMessage_ShowAtActor(15, 0);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_ArmCallback(5, 0, 0);
  Actor_ArmCallback(18, 0, 0);
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
  result = Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  done = &object->unk63;
  owner = gMainRoutineWork.currentOwner;
  if (result == 0) {
    Actor_WaitForAnimationChange(owner);
    Motion_SetModeAndWaitAnimation(15, 4);
    Event_ShowMessageAndWait(15, 0);
  } else {
    Actor_WaitForAnimationChange(owner);
    Motion_SetModeAndWaitAnimation(15, 4);
    EVENT_WORK->field1c4++;
    Event_ShowMessageAndWait(15, 0);
    Actor_SetModeById(15, 3);
    Event_ShowMessageAndWait(15, 0);
  }
  Actor_WalkToAndWait(15, 212, 582);
  Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(500), 1);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 15);
  Actor_WatchAndSetScript(18, 15);
  Actor_WatchAndSetScript(5, 15);
  Actor_WatchAndSetScript(7, 15);
  Actor_WatchAndSetScript(6, 15);
  *done = 0;
  Actor_SetMovementTypeById(15, gData_0200504c);
  Event_Wait(20);
  Actor_SetMovementTypeById(17, gData_0200504c);
  Actor_SetMovementTypeById(16, gData_0200504c);
  Sound_Play(78);
  do {
    wait(1);
  } while (*done == 0);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(18);
  Actor_Stop(5);
  Actor_Stop(7);
  Actor_Stop(6);
  Func_02004a2c();
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(540), 1);
  Event_Wait(20);
  angle = 0x4000;
  Actor_SetPositionAndFacing(15, PIXELS(224), PIXELS(280), angle);
  Actor_SetPositionAndFacing(16, PIXELS(208), PIXELS(264), angle);
  Actor_SetPositionAndFacing(17, PIXELS(240), PIXELS(264), angle);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_ArmCallback(18, 0, 0);
  Actor_ArmCallback(5, 0, 0);
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
  Actor_ArmCallbackAndRefresh(6, FACING_SOUTHEAST);
  Event_Wait(10);
  Actor_SetModeById(18, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(7, 3);
  Motion_SetModeAndWaitAnimation(6, 3);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(18);
  Actor_Stop(5);
  Actor_Stop(7);
  Actor_Stop(6);
  ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner))->update = NULL;
  ((struct ShamanVillageObject *)Actor_GetById(18))->update = NULL;
  ((struct ShamanVillageObject *)Actor_GetById(5))->update = NULL;
  ((struct ShamanVillageObject *)Actor_GetById(7))->update = NULL;
  ((struct ShamanVillageObject *)Actor_GetById(6))->update = NULL;
  wait(1);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetMovementTypeById(7, gData_020050bc);
  Actor_SetMovementTypeById(6, gData_020050bc);
  Actor_SetMovementTypeById(5, gData_020050bc);
  Actor_SetMovementTypeAndWaitById(18, gData_020050bc);
  GameFlag_SetBit(0x94c);
  Func_020048cc();
}

void Func_02003b24(void) {
  Actor_SetSpritePriority(6, 1);
  Sound_Play(131);
  Func_02004a54();
  Func_02004a6c(((struct ShamanVillageObject *)Actor_GetById(6)), 2);
  Event_Wait(40);
  Func_02004a6c(((struct ShamanVillageObject *)Actor_GetById(6)), 0);
  Func_02004a64();
  Func_02004a5c();
  Func_0200057c();
  Actor_SetSpritePriority(6, 2);
}

void Func_02003b6c(s32 a, s32 b, s32 c, s32 d, s32 mode, s32 f) {
  struct ShamanVillageEventWork *work = Ram_HeapSlots->eventWork;
  struct ShamanVillageObject *object;
  s32 cell;

  Func_020048c4();
  Event_ResolvePendingActions(0);
  if (mode == 2)
    Sound_Play(188);
  else
    Sound_Play(158);
  Map_CopyCellPictureIndicesRect(a, b, c, d, mode, f);
  Event_Wait(20);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  if (mode == 1) {
    object = ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner));
    cell = object->x >> 16;
    Actor_SetMoveTarget(object, ((8 - cell % 16) << 16) + object->x, object->y,
                  object->z);
    Script_WaitForEventTimeout(object);
    object->facing = 0xc000;
  }
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner,
                                          gData_02004c08);
  wait(12);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}

void Func_02003c58(void) { Func_02003b6c(43, 40, 8, 22, 1, 2); }

void Func_02003c74(void) { Func_02003b6c(43, 42, 4, 26, 1, 2); }

void Func_02003c90(void) { Func_02003b6c(43, 42, 27, 21, 1, 2); }

void Func_02003cac(void) { Func_02003b6c(43, 40, 28, 17, 1, 2); }

void Func_02003cc8(void) { Func_02003b6c(43, 42, 10, 8, 1, 2); }

void Func_02003b6c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02003ce4(void) {
  if (GameFlag_Test(0x202)) {
    Func_020048c4();
    Event_ResolvePendingActions(0);
    Sound_Play(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Import_EventSetExitRequest(13);
  } else {
    Func_02003b6c(43, 40, 7, 6, 1, 2);
  }
}

void Func_0200482c(void);

#define EVENT_WORK ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)

void Func_02003d30(void) {
  s32 angle;

  GameFlag_ClearBit(0x11b);
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  ((struct ShamanVillageObject *)Actor_GetById(15))->facing = 0xc000;
  FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(182), 0);
  wait(1);
  Func_0200482c();
  wait(1);
  angle = 0x4000;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetPositionAndFacing(16, PIXELS(184), PIXELS(128), angle);
  Actor_SetPositionAndFacing(17, PIXELS(264), PIXELS(128), angle);
  Actor_SetSpeed(16, 0x19999, 0xcccc);
  Actor_SetSpeed(17, 0x19999, 0xcccc);
  Camera_SetSpeedParameters(0xcccc, 0x1999);
  Camera_MoveWithinMapBounds(PIXELS(224), -1, PIXELS(280), 1);
  Actor_WalkTo(16, 184, 256);
  Actor_WalkToAndWait(17, 264, 256);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Actor_WalkTo(16, 208, 264);
  Actor_SetPositionAndReset(17, 240, 264);
  Actor_SetModeById(16, 1);
  Event_Wait(10);
  Actor_ArmCallback(16, angle, 0);
  Actor_ArmCallback(17, angle, 0);
  Actor_FaceDirectionAndWait(15, FACING_WEST);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Func_020049f4();
  Event_SetMessageId((s32)MsgShamanMustFinishTrialRoad);
  Event_ShowMessage(0x200f, 0);
  Actor_SetModeById(15, 3);
  Event_OpenMessage(0x200f, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
    Event_OpenMessage(0x200f, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
      Motion_SetModeAndWaitAnimation(15, 3);
      Sound_Play(76);
      Event_ShowMessageAndWait(0x200f, 0);
      GameFlag_SetBit(0x200);
      Func_02001380();
      return;
    }
    EVENT_WORK->field1c4++;
  } else {
    EVENT_WORK->field1c4 += 2;
  }
  Event_OpenMessage(0x200f, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
    Event_ShowMessageAndWait(0x200f, 0);
    Func_0200289c();
  } else {
    EVENT_WORK->field1c4++;
    Event_ShowMessageAndWait(0x200f, 0);
    Func_0200290c();
  }
  Func_02004a2c();
  GameFlag_ClearBit(0x12f);
  Func_020048cc();
}

void Func_0200482c(void);
void Func_02004060(void);
void Func_020041b8(void);
void Func_020043f8(void);
void Func_02004534(void);
void Func_020046ec(void);

void Func_02003f78(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanMoapaAdvisesLeaving);
  ((struct ShamanVillageObject *)Actor_GetById(15))->facing = 0x8000;
  ((struct ShamanVillageObject *)Actor_GetById(16))->facing = 0x8000;
  ((struct ShamanVillageObject *)Actor_GetById(17))->facing = 0x8000;
  Camera_MoveWithinMapBounds(PIXELS(224), -1, PIXELS(280), 0);
  wait(1);
  Func_0200482c();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Event_ShowMessageAndWait(0x200f, 0);
  Func_0200289c();
  GameFlag_ClearBit(0x12f);
  Func_020048cc();
}

s32 Func_02003ff8(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShamanVillage1)
    Func_02004060();
  else if (scene == (s32)&SceneId_ShamanVillage3)
    Func_020041b8();
  else if (scene == (s32)&SceneId_ShamanVillage2)
    Func_020043f8();
  else if (scene == (s32)&SceneId_ShamanVillage4)
    Func_02004534();
  else if (scene == (s32)&SceneId_ShamanVillage6)
    Func_020046ec();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_02003d30(void);
void Func_02003f78(void);

void Func_02004060(void) {
  if (GameFlag_Test(0x94c)) {
    MapGrid_CopyCellsTo(47, 19, 38, 19, 9, 11);
    MapGrid_CopyCellAttributes(1, 5, 9, 1, 6, 29);
    Actor_SetPartAttribute(((struct ShamanVillageObject *)Actor_GetById(14)), 6);
  } else if (!GameFlag_Test(0x109)) {
    s32 angle = 0x4000;

    Actor_SetPositionAndFacing(15, PIXELS(168), PIXELS(568), angle);
    Actor_SetPositionAndFacing(17, PIXELS(216), PIXELS(536), angle);
    Actor_SetHorizontalPositionAndAngleWithTerrain(16, PIXELS(120), PIXELS(536), angle);
    wait(1);
  }
  Actor_SetModeById(8, 2);
  Actor_SetModeById(9, 2);
  Actor_SetModeById(10, 2);
  Actor_SetModeById(11, 2);
  Actor_SetModeById(12, 2);
  Actor_SetModeById(13, 2);
  if (!GameFlag_Test(0x109)) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 11)
      Func_02003d30();
    else if (entrance == 9)
      Func_02003f78();
  }
  if (GameFlag_Test(0x9fe)) {
    Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
    Map_CopyCellAttributeRect(10, 12, 3, 1, 10, 13);
    Map_CopyCellAttributeRect(15, 12, 3, 1, 15, 13);
    MapGrid_CopyCellAttributes(9, 20, 3, 1, 9, 21);
  }
}

void Func_020041b8(void) {
  struct ShamanVillageObject *object;

  Actor_SetModeById(8, 2);
  Actor_SetModeById(9, 2);
  Actor_SetModeById(10, 2);
  Actor_SetModeById(11, 2);
  Actor_SetModeById(12, 2);
  Actor_SetModeById(13, 2);
  ((struct ShamanVillageObject *)Actor_GetById(14))->motionFlags = 0;
  if (GameFlag_Test(0x94e)) {
    Actor_SetPositionAndResetMotion(((struct ShamanVillageObject *)Actor_GetById(14)), PIXELS(520), PIXELS(32),
                                     PIXELS(184));
    wait(1);
    MapGrid_CopyCellAttributes(30, 11, 1, 1, 32, 11);
    wait(10);
  }
  if (GameFlag_Test(0x9e1)) {
    Map_CopyCellPictureIndicesRect(64, 0, 76, 2, 3, 3);
    MapGrid_CopyCellAttributes(12, 5, 3, 1, 12, 6);
  }
  if (GameFlag_Test(0x94d))
    MapGrid_CopyCellsTo(48, 40, 101, 11, 1, 2);
  else
    Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  object = ((struct ShamanVillageObject *)Actor_GetById(16));
  object->collisionFlags = 8;
  object->scaleX = 0xcccc;
  object->scaleY = 0xcccc;
  Actor_SetSingleChildFlags(((struct ShamanVillageObject *)Actor_GetById(16)), 0);
}

void Func_020042c8(struct ShamanVillageObject *object) {
  s32 step;

  object->x += object->field64 << 8;

  object->y += 0x8000;

  step = 0x51e;
  object->scaleX += step;
  object->scaleY += step;
  object->field64 += 2;
  if (--object->field68 == 0)
    Func_0200481c(object);
}

void Func_020042c8(struct ShamanVillageObject *object);

void Func_0200430c(s32 x, s32 y, s32 z) {
  struct ShamanVillageObject *object =
      ((struct ShamanVillageObject *)Import_ActorCreate(285, x, y, z));

  if (object != NULL) {
    object->field5c = 2;
    object->motionFlags = 0;
    object->field64 = 0;
    object->priorityFlags &= ~1;
    Actor_SetPartAttribute(object, 7);
    object->sprite->priority = 1;
    object->priorityFlags |= 2;
    Actor_SetSingleChildFlags(object, 0);
    object->scaleX = 0x8000;
    object->scaleY = 0x8000;
    object->field68 = 60;
    object->update = Func_020042c8;
    Import_ActorSetMode(object, 5);
    Animation_SetStateFlags(object, 1);
  }
}

void Func_0200430c(s32 x, s32 y, s32 z);

void Func_02004394(void) {
  if ((gFrameCount & 63) == 0) {
    Func_0200430c(0x480000, 0x420000, 0x1780000);
    Func_0200430c(0x680000, 0x620000, 0x980000);
  }
}

void Func_020043c8(void) {
  s32 variant = ((struct ShamanVillageObject *)Actor_GetById(
                     gMainRoutineWork.currentOwner))
                    ->sprite->priority;

  Actor_SetSpritePriority(21, variant);
  Actor_SetSpritePriority(14, variant);
}

void Func_020048ac(s32 mode);
void Func_02002878(void);
void Func_020043c8(void);
void Func_02004394(void);

struct IoReg16 {
  u16 value;
};

void Func_020043f8(void) {
  if (!GameFlag_Test(0x109)) {
    if (GameFlag_Test(0x9fe)) {
      Actor_SetPosition(16, PIXELS(104), PIXELS(200));
      Actor_SetPosition(17, PIXELS(136), PIXELS(200));
    } else if (GameFlag_Test(0x94f) && !GameFlag_Test(0x9e1)) {
      Actor_SetPosition(15, PIXELS(120), PIXELS(208));
      Actor_SetPosition(16, PIXELS(104), PIXELS(212));
      Actor_SetHorizontalPositionWithTerrain(17, PIXELS(136), PIXELS(212));
      wait(1);
    }
  }
  if (GameFlag_Test(0x202))
    MapGrid_CopyCellsTo(43, 40, 7, 6, 1, 2);
  if (GameFlag_Test(0x94d)) {
    s32 width = 1;
    s32 height = 2;

    Map_CopyCellPictureIndicesRect(48, 40, 77, 21, width, height);
    Map_CopyCellPictureIndicesRect(48, 40, 81, 25, width, height);
  }
  Func_02004a44();
  Func_02004a4c(0, 0x204, 19, 20);
  if (GameFlag_Test(0x205))
    Func_02002878();
  REG_BLDALPHA = 0xc08;
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  {
    s32 priority = 0x480;

    /* FAKEMATCH(barrier): the empty asm holds r0 until the priority is
     * complete, so the callback is loaded last as the native has; as
     * Task_Add(Func_020043c8, 0x480), 8 of 128 instructions differ in
     * tla-en. */
    __asm__("" : "+l"(priority) : : "r0");
    Task_Add(Func_020043c8, priority);
  }
  {
    s32 priority = 0x480;

    Task_Add(Func_02004394, priority);
  }
}

void Func_02004534(void) {
  u32 i;

  ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      521;
  ((struct ShamanVillageObject *)Actor_GetById(12))->collisionFlags = 8;
  if (GameFlag_Test(0x9fe)) {
    /* FAKEMATCH(temporary): the local holding one constant starts the facing
     * before the other arguments, as the native has; as the plain argument
     * 0x3000, 2 of 159 instructions differ in tla-en. */
    s32 angle = 0x3000;

    Actor_SetPositionAndFacing(8, PIXELS(88), PIXELS(92), angle);
    Actor_SetPosition(11, PIXELS(156), PIXELS(72));
  } else {
    Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
  }
  if ((gMainRoutineWork.entrance == 30 || gMainRoutineWork.entrance == 31) &&
      !GameFlag_Test(0x109)) {
    Func_020048ac(1);
    Func_020048c4();
    Event_ResolvePendingActions(0);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 41);
    Actor_SetSingleChildFlags(((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner)),
                                       0);
    wait(1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(20);
    i = 0;
    goto test_stand;
  next_stand:
    i++;
  test_stand:
    if (i < 60) {
      wait(1);
      if (gInput.held == 0)
        goto next_stand;
    }
    Actor_SetModeById(gMainRoutineWork.currentOwner, 42);
    Event_Wait(10);
    i = 0;
    goto test_turn;
  next_turn:
    i++;
  test_turn:
    if (i < 30) {
      wait(1);
      if (gInput.held == 0)
        goto next_turn;
    }
    Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 813, 120);
    Actor_SetSingleChildFlags(((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner)),
                                       1);
    Func_020048cc();
    if (gMainRoutineWork.entrance == 30) {
      GameFlag_SetBit(0x9fe);
      GameFlag_SetBit(0x94d);
    }
  }
}

void Func_020046bc(void) {
  s32 variant = ((struct ShamanVillageObject *)Actor_GetById(
                     gMainRoutineWork.currentOwner))
                    ->sprite->priority;

  Actor_SetSpritePriority(9, variant);
  Actor_SetSpritePriority(10, variant);
}

void Func_020046bc(void);

void Func_020046ec(void) {
  struct ShamanVillageEventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 521;
  if (GameFlag_Test(0x9fe)) {
    Task_Add(Func_020046bc, 0x480);
  } else {
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  }
}
