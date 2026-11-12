/// @file
/// Scene code of overlay 0x672, in ROM order, from s0f00's offset up to the next object the overlay links.

#include "text/msg_ids.h"

#include "gabomba_statue_chamber.h"
#include "event_inline.h"

extern struct GabombaStatueChamberObject *gData_02002058;
void *HeapSlot_AllocFar(s32 slot, s32 size);
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
void Func_02001ef0(void);

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
    sprite = statue->sprite;
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

void Func_02001d40(void);
s32 Func_02001f08();
extern u8 gData_02001f4c[];

struct GroupWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
  u8 unk1b0[0x1c4 - 0x1b0];
  u16 unk1c4;
};

void Func_02001710(void) {
  s32 *owner;
  s32 facing;
  s32 angleC;
  s32 zero;
  s32 turn;
  struct HeapSlots *heap;
  Func_02001da0();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  System_Wait(1);
  Func_02001010();
  Func_02001d40();
  System_Wait(1);
  owner = (s32 *)&gMainRoutineWork;
  owner += 133;
  {
    s32 angleA = 0xe000;

    Actor_SetPositionAndFacing(*owner, 0x1680000, 0xd80000, angleA);
    Actor_SetPositionAndFacing(7, 0x1780000, 0xd80000, angleA);
  }
  facing = 0xa000;
  zero = 0;
  Actor_SetPositionAndFacing(11, 0x1680000, 0xc80000, zero);
  Actor_SetPositionAndFacing(5, 0x1980000, 0xd80000, facing);
  angleC = 0x8000;
  Actor_SetPositionAndFacing(6, 0x1a80000, 0xc80000, angleC);
  {
    s32 angleB = 0xb000;

    Actor_SetPositionAndFacing(12, 0x1880000, 0xc80000, angleB);
  }
  heap = Ram_HeapSlots;
  System_Wait(1);
  {
    s32 *effect = &((struct GroupWork *)heap->eventWork)->screenEffect;

    turn = 0x100;
    *effect = turn;
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  Actor_FaceDirection(12, 0xd000, 20);
  Actor_FaceDirectionAndWait(12, 0x5000);
  Func_080d47b4(12, 0x106, 40);
  Actor_FaceDirection(7, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(11, 0xd000, 0);
  Actor_FaceDirection(6, facing, 0);
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_WalkToAndWait(12, 0x188, 168);
  Event_PlaySound(210);
  Func_02000f00();
  FieldEvent_Wait(40);
  Event_PlaySound(185);
  {
    struct Actor *guard = Actor_GetById(8);

    guard->motionFlags = zero;
  }
  Func_02001064();
  Actor_SetSpeed(8, 0x6666, 0x3333);
  Actor_MoveToAndWait(8, 0x1a8, 152);
  Actor_FaceDirectionAndWait(12, 0x5000);
  FieldEvent_SetMessageId((s32)MsgIMightBe);
  Event_ShowMessage(0x400c, 0);
  Actor_SetAnimationAndWait(12, 3);
  Actor_WalkToAndWait(12, 0x188, 140);
  System_Wait(1);
  Actor_SetPosition(12, 0, 0);
  System_Wait(1);
  Func_080d47b4(5, turn, 20);
  Actor_FaceDirection(5, angleC, 0);
  FieldMessage_ShowAtActor(0x4005, 0);
  if (Event_AskYesNoWithActorAnimation(*owner, 0) == 0) {
    Actor_FaceDirectionAndWait(7, 0);
    Actor_SetAnimation(7, 3);
    Event_ShowMessage(0x4007, 0);
    ((struct GroupWork *)heap->eventWork)->unk1c4++;
  } else {
    ((struct GroupWork *)heap->eventWork)->unk1c4++;
    Actor_FaceDirectionAndWait(7, angleC);
    Event_ShowMessage(0x4007, 0);
    Func_02001f08();
  }
  Actor_FaceDirection(6, 0x8000, 0);
  Actor_SetAttachedEffect(6, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(0x4006, 0);
  Actor_FaceDirectionAndWait(11, 0);
  Actor_SetAnimation(11, 4);
  Event_ShowMessage(0x400b, 0);
  Actor_FaceDirectionAndWait(11, 0xd000);
  Event_ShowMessage(0x400b, 0);
  Actor_FaceDirectionAndWait(11, 0x3000);
  Actor_SetAnimationAndWait(11, 3);
  Event_ShowMessage(0x400b, 0);
  Actor_FaceDirection(7, 0x8000, 0);
  Actor_FaceDirection(7, 0x8000, 20);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(6, 3);
  Actor_SetSpeed(11, 0xcccc, 0x6666);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  {
    s32 path = (s32)gData_02001f4c;

    Actor_SetScript(11, path);
    Actor_SetScript(7, path);
    Actor_SetScript(5, path);
    Actor_RunScriptAndWait(6, path);
  }
  Event_SetFlag(0x8fd);
  Func_02001da8();
}

void Func_02001b14(void);
void Func_02001ba8(void);
void Func_02001c44(void);

s32 Func_02001aa4(void) {
  struct GabombaStatueChamberEventWork *work;
  s16 scene;

  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 521;
  ((struct GabombaStatueChamberObject *)Actor_GetById(Party_GetLeaderActor()))
      ->priorityFlags |= 0x20;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov672Room1)
    Func_02001b14();
  else if (scene == (s32)&SceneId_Ov672Room2)
    Func_02001ba8();
  else if (scene == (s32)&SceneId_Ov672Room3)
    Func_02001c44();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_02001b14(void) {
  Func_02001e80(8);
  Func_02001e80(9);
  Func_02001e80(10);
  wait(1);
  if (GameFlag_Test(0x8fd)) {
    Func_02001010();
    Func_02001064();
    Actor_SetPosition(8, PIXELS(424), PIXELS(152));
    wait(1);
    Func_02000f00();
  } else {
    s32 entrance;

    entrance = gMainRoutineWork.entrance;
    if (entrance == 5) {
      Func_020010d4();
      return;
    }
    if (entrance == 6) {
      Func_02001710();
      return;
    }
  }
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->entrance == 3) {
      GameFlag_ClearBit(0xa27);
      GameFlag_SetBit(0xa28);
    }
  }
}

#include "../../../include/system/io_reg.h"

struct GabombaStatueChamberLayer {
  u8 unk00[12];
  s32 y;
  u8 unk10[0x28];
};

struct GabombaStatueChamberMapWork {
  u8 unk000[0x108];
  struct GabombaStatueChamberLayer layers[3];
};

void Func_02001d60(void);
void Func_02000be8(void);

void Func_02001ba8(void) {
  if (GameFlag_Test(265)) {
    if (GameFlag_Test(519)) {
      struct GabombaStatueChamberMapWork *map =
          (struct GabombaStatueChamberMapWork *)Ram_HeapSlots->mapWork;
      struct GabombaStatueChamberLayer *front = &map->layers[0];
      struct GabombaStatueChamberLayer *back = &map->layers[2];
      s32 blend = 1;
      s32 i;

      Func_02001d60();
      i = 0;
    loop:
      if (i >= 17 && i <= 79) {
        u32 alpha = ((blend + 1) / 4) << 8;

        alpha |= 16 - (blend + 1) / 4;
        REG_BLDALPHA = alpha;
        blend++;
      }
      front->y += 0x10000;
      back->y += 0x10000;
      i++;
      if (i < 352)
        goto loop;
      Func_02001d40();
      wait(1);
    }
  } else if (gMainRoutineWork.entrance == 2) {
    Func_02000be8();
  }
}

#include "version.h"

/// Gives actors 15 to 32 action variant 2, runs Func_02001e80 for actor 8 and
/// grounds it, then Func_02001094 with actor 10 placed when flag 0x8ff is set,
/// else Func_02000f88; arriving by entrance 2 it sets the retreat to
/// Ov671Room7's entrance 1 (flag 0xa27) or Ov672Room1's entrance 3 (flag
/// 0xa28).
void Func_02001c44(void) {
  u32 i;
  char *scene;

  for (i = 0; i <= 17; i++)
    Actor_SetSpritePriority(i + 15, 2);
  Func_02001e80(8);
  Actor_GetById(8)->y = 0;
  if (GameFlag_Test(0x8ff)) {
    Func_02001094();
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  } else {
    Func_02000f88();
  }
#if TLA_PATCH_1 /* The Japanese edition does not set the retreat. */
  if (gMainRoutineWork.entrance == 2) {
    if (GameFlag_Test(0xa27)) {
      /* FAKEMATCH(temporary): the scene id through a local, which issues its
       * literal load first; as plain stores, 4 of 71 instructions differ in
       * tla-en. */
      scene = &SceneId_Ov671Room7;
      gMainRoutineWork.retreatScene = (s32)scene;
        gMainRoutineWork.retreatEntrance = 1;
    } else if (GameFlag_Test(0xa28)) {
      /* FAKEMATCH(temporary): the scene id through a local, as in the first
       * branch; as plain stores, 4 of 71 instructions differ in tla-en. */
      scene = &SceneId_Ov672Room1;
      gMainRoutineWork.retreatScene = (s32)scene;
        gMainRoutineWork.retreatEntrance = 3;
    }
  }
#endif
}
