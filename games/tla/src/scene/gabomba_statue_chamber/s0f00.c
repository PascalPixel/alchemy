/// @file
/// Scene code of overlay 0x672, in ROM order, from s0f00's offset up to the next object the overlay links.

#include "text/msg_ids.h"

#include "gabomba_statue_chamber.h"
#include "event_inline.h"

extern struct GabombaStatueChamberObject *gData_02002058;
void *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void Func_02000ef0();

void Func_02000f00(void) {
  struct GabombaStatueChamberObject *leader =
      ((struct GabombaStatueChamberObject *)Actor_GetById(8));
  s32 x = leader->x;
  s32 z = leader->z;
  /* FAKEMATCH(temporary): a local holding the 0 for the three byte stores, set
   * before the null test, so it takes r7; as the constant 0 in each store, 20
   * of 58 instructions differ in tla-en. */
  s32 zero = 0;
  struct GabombaStatueChamberObject *object;

  if ((gData_02002058 = object =
           ((struct GabombaStatueChamberObject *)Import_ActorCreate(
               489, x, 0x120000, z))) != NULL) {
    struct FieldSprite *sprite = object->sprite;
    u8 *block;

    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 2;
    object->motionFlags = zero;
    object->field5c = 1;
    object->update = Func_02000ef0;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    block = HeapSlot_AllocFar(68, 0x608);
    Icon_DrawItem(230);
    VramCache_Load(sprite->vramBlock, 128, block + 0x400);
    HeapSlot_Release(68);
  }
}

void Func_02000f88(void) {
  struct GabombaStatueChamberObject *leader =
      ((struct GabombaStatueChamberObject *)Actor_GetById(8));
  s32 x = leader->x;
  s32 z = leader->z;
  /* FAKEMATCH(temporary): a local holding the 0 for the three byte stores, set
   * before the null test, so it takes r7; as the constant 0 in each store, 20
   * of 58 instructions differ in tla-en. */
  s32 zero = 0;
  struct GabombaStatueChamberObject *object;

  if ((gData_02002058 = object =
           ((struct GabombaStatueChamberObject *)Import_ActorCreate(
               489, x, 0x120000, z))) != NULL) {
    struct FieldSprite *sprite = object->sprite;
    u8 *block;

    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 2;
    object->motionFlags = zero;
    object->field5c = 1;
    object->update = Func_02000ef0;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    block = HeapSlot_AllocFar(68, 0x608);
    Icon_DrawItem(242);
    VramCache_Load(sprite->vramBlock, 128, block + 0x400);
    HeapSlot_Release(68);
  }
}

void Func_02001010(void) {
  Map_CopyCellPictureIndicesRect(85, 78, 22, 79, 5, 11);
  MapGrid_CopyCellAttributes(85, 78, 5, 11, 22, 15);
  MapGrid_CopyCellsTo(86, 30, 86, 20, 5, 2);
  Map_CopyCellPictureIndicesRect(86, 30, 84, 22, 9, 2);
}

#include "types.h"
#include "actor.h"

void Func_02000a98(s32 a, s32 b);

void Func_02001064(void) {
  Map_CopyCellAttributeRect(23, 1, 3, 1, 23, 9);
  Map_CopyCellAttributeRect(23, 0, 3, 1, 25, 9);
}

void Func_02001094(void) {
  Map_CopyCellPictureIndicesRect(0, 1, 26, 6, 1, 1);
  Map_CopyCellPictureIndicesRect(0, 65, 26, 70, 1, 1);
  MapGrid_CopyCellAttributes(0, 2, 3, 3, 25, 5);
}

// The scene after the statue is lifted in the Gabomba Statue's chamber.

#include "ram_buffer.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "scene_tables.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

extern const u8 gData_02001f18[];
void Func_02001da0(void);
void HeapSlot_Release();
void Func_02001010(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001ef0(void);
void Event_FinishScreenTransition(void);

/// Places the party around the statue object (its sprite reloaded with item
/// 242's picture), raises and moves the statue while the camera follows, has
/// actor 12 ask a yes-or-no question (both answers count up the event work's
/// halfword 0x1c4), sends actors 5 and 12 off with their action scripts and
/// sets flag 265.
void Func_020010d4(void) {
  struct Actor *statue = (struct Actor *)((s32)Actor_GetById(13));
  struct FieldSprite *sprite;
  u8 *block;
  s32 turnA, turnB;
  s32 owner;

  Func_02001da0();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldActor_SetPositionAndResetMotion(((s32)Actor_GetById(13)), PIXELS(392), 0,
                                       PIXELS(268));
  wait(1);
  /* FAKEMATCH(temporary): the leader read before the first turn is built; as
   * gMainRoutineWork.currentOwner in the call, 34 of 640 instructions differ in
   * tla-en. */
  owner = gMainRoutineWork.currentOwner;
  Actor_SetPositionAndFacing(owner, PIXELS(360), PIXELS(184),
                             turnA = FACING_NORTHEAST);
  Actor_SetPositionAndFacing(7, PIXELS(376), PIXELS(184), turnA);
  Actor_SetPositionAndFacing(11, PIXELS(360), PIXELS(168), turnA);
  Actor_SetPositionAndFacing(6, PIXELS(423), PIXELS(168),
                             turnB = FACING_NORTHWEST);
  Actor_SetPositionAndFacing(5, PIXELS(408), PIXELS(184), turnB);
  if (statue != 0) {
    sprite = statue->animation;
    sprite->nextFrame = 0;
    sprite->unk1b[0] = 0;
    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 1;
    ((u8 *)statue)[0x5c] = 1;
    block = (u8 *)HeapSlot_AllocFar(68, 0x608);
    Icon_DrawItem(242);
    VramCache_Load(sprite->vramBlock, 0x80, (s32)(block + 0x400));
    HeapSlot_Release(68);
  }
  Func_02001010();
  *(s32 *)(EVENT_WORK + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Actor_Jump(7, 4, 10);
  Event_SetMessageId(MsgTheBlackOrb);
  Event_ShowMessageAndWait(7, 0);
  Actor_SetAttachedEffect(6, 258);
  Actor_FaceDirectionAndWait(6, FACING_WEST);
  Event_ShowMessage(0x4006, 0);
  Func_080d47b4(5, 265, 20);
  Event_ShowMessageAndWait(5, 0);
  FieldActor_SetPositionAndResetMotion(((s32)Actor_GetById(13)), PIXELS(392), 0,
                                       PIXELS(340));
  Sound_Play(78);
  BattleEffect_ApplyColorToTargetBuffer(0x10002, 0);
  BattleEffect_StartBufferInterpolation(40);
  wait(40);
  Actor_SetModeById(9, 2);
  Actor_SetModeById(10, 2);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 13);
  Actor_WatchAndSetScript(5, 13);
  Actor_WatchAndSetScript(6, 13);
  Actor_WatchAndSetScript(7, 13);
  Actor_WatchAndSetScript(11, 13);
  Event_Wait(10);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(392), -1, PIXELS(246), 1);
  Actor_SetSpeed(13, 0xcccc, 0x6666);
  Actor_MoveToAndWait(13, 392, 246);
  Actor_SetModeById(9, 1);
  Actor_SetModeById(10, 1);
  statue->velocityY = PIXELS(3);
  statue->gravity = 0x8000;
  Actor_SetSpeed(13, 0x19999, 0xcccc);
  Actor_MoveToAndWait(13, 392, 234);
  Event_Wait(10);
  FieldCamera_MoveTo(PIXELS(392), -1, PIXELS(168), 1);
  Actor_SetSpeed(13, 0xcccc, 0x6666);
  Actor_SetPositionAndCommit(13, 392, 160);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(5);
  Actor_Stop(6);
  Actor_Stop(7);
  Actor_Stop(11);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  Event_Wait(20);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(40);
  wait(40);
  Func_02001ef0();
  Func_080d47b4(11, 0x100, 40);
  Event_ShowMessage(0x400b, 0);
  Func_080d47b4(6, 261, 20);
  Event_ShowMessage(0x4006, 0);
  Actor_ArmCallbackAndRefresh(5, turnA);
  Event_ShowMessageAndWait(0x1005, 0);
  Actor_ArmCallback(5, FACING_NORTHWEST, 0);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  Actor_WalkToAndWait(6, 400, 172);
  Actor_SetModeById(6, 9);
  Event_Wait(20);
  Event_ShowMessage(0x4006, 0);
  Actor_SetModeById(7, 4);
  Event_ShowMessageAndWait(7, 0);
  Event_ShowMessageAndWait(0x4006, 0);
  Func_080d47b4(11, 0x100, 0);
  Event_ShowMessage(0x400b, 0);
  Actor_FaceDirectionAndWait(11, FACING_SOUTHEAST + FACING_STEP);
  Event_ShowMessageAndWait(0x400b, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Sound_Play(0);
  {
    /* FAKEMATCH(temporary): the turn built before the other arguments; as a
     * plain 0xd000 argument, 2 of 640 instructions differ in tla-en. */
    s32 turn = 0xd000;
    Actor_SetHorizontalPositionAndAngleWithTerrain(12, PIXELS(400), PIXELS(320),
                                                   turn);
  }
  wait(1);
  Event_ShowMessageAndWait(12, 0);
  Actor_SetModeById(6, 1);
  wait(1);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  Actor_FaceDirection(11, FACING_SOUTHEAST + FACING_STEP, 0);
  Camera_SetSpeedParameters(0x19999, 0x3333);
  Camera_MoveWithinMapBounds(PIXELS(392), -1, PIXELS(200), 1);
  Actor_SetSpeed(12, 0x13333, 0x9999);
  Actor_WalkToAndWait(12, 404, 220);
  Actor_Jump(12, 4, 10);
  FieldMessage_ShowAtActor(12, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Func_080d489c(12, 258);
    Event_Wait(20);
    Event_ShowMessageAndWait(12, 0);
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
  } else {
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
    Actor_ShowEmote(12, 259, 20);
    Event_ShowMessageAndWait(12, 0);
  }
  Actor_ShowEmote(7, 263, 20);
  Event_ShowMessageAndWait(7, 0);
  Func_080d47b4(12, 0x100, 40);
  Event_ShowMessageAndWait(12, 0);
  Func_080d47b4(6, 263, 20);
  Event_ShowMessageAndWait(6, 0);
  Func_080d489c(12, 258);
  Event_Wait(20);
  Event_ShowMessageAndWait(12, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  Motion_SetModeAndWaitAnimation(11, 3);
  Actor_SetModeById(12, 4);
  Event_ShowMessageAndWait(12, 0);
  Func_080d47b4(5, 261, 20);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_WalkToAndWait(5, 420, 228);
  Actor_ArmCallbackAndRefresh(5, FACING_NORTHWEST);
  Event_Wait(10);
  Actor_SetSpeed(5, 0x19999, 0xcccc);
  Actor_WalkToAndWait(5, 412, 222);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(12, 0x10000, 0x8000);
  Actor_SetMovementTypeById(5, gData_02001f18);
  Event_Wait(10);
  Actor_SetMovementTypeById(12, gData_02001f18);
  Event_Wait(60);
  *(s32 *)(EVENT_WORK + 0x1ac) = 514;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_SetBit(265);
  Import_EventSetExitRequest(2);
}

