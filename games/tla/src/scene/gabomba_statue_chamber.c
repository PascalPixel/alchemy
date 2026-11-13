/// @file
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "gamepak/far_call/system.h"
#include "module/scene/scene.h"
#include "module/system/system.h"
#include "shared/gabomba_statue_chamber.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "system/input.h"
#include "field_sprite.h"
#include "system/io_reg.h"
#include "version.h"
#include "shared/tables.h"
#include "objcmd.h"
#include "text/msg_ids.h"

void Func_02001094(void);

void Func_02000ef0();
void GabombaStatueChamber_InitializeRoom1(void);
void GabombaStatueChamber_InitializeRoom2(void);

void GabombaStatueChamber_InitializeRoom3(void);

void Func_02000be8(void);
void Func_02000184(void);
void Func_02000b60(void);
void Func_02000b70(void);
void Func_02000d3c(void);

extern s32 gData_02002048[];

void ActorSystem_SuspendProjectionAndShowSprites(void);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

u8 *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_GabombaStatueChamber1)
    return gData_0200a08c;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_GabombaStatueChamber1)
    return gData_020020f8;
  if (scene == (s32)&SceneId_GabombaStatueChamber3)
    return gData_020021e8;
  return gData_02002458;
}

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_GabombaStatueChamber1)
    return gData_02002470;
  if (scene == (s32)&SceneId_GabombaStatueChamber2)
    return gData_020024a0;
  if (scene == (s32)&SceneId_GabombaStatueChamber3)
    return gData_020024dc;
  return gData_02002530;
}

void Func_02000100(void) {
  volatile u16 *color = (volatile u16 *)&OBJ_PLTT_COLOR(0, 15);
  struct GabombaStatueChamberEventWork *work;

  Actor_SetModeById(8, 2);
  PaletteFade_ApplySource(0x10000, 2);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 2);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  work = Ram_HeapSlots->eventWork;
  FieldMessage_ShowCentered((s16)work->field1c4++, 0, 2);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 2);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Actor_SetModeById(8, 1);
}

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

/* VIEW(first): routine gData_02002058 as this part declared it; the joined file
 * has another; tried as the definition's extern u32 gData_02002058[], it does
 * not compile in tla-ja: games/tla/src/scene/gabomba_statue_chamber.c:129:
 * `gData_02002058' undeclared (first use in this function). */
extern struct Actor *gData_02002058__first __asm__("gData_02002058");
#define gData_02002058 gData_02002058__first
extern u32 gData_02001f78[];
extern s32 gData_02001f9c[];
extern u32 gData_02001fe4[];
extern u32 gData_0200200c[];
#define OBJECT(id) (Actor_Get((id)))
void ActorSystem_ResumeProjectionAndHideLayers(void);

void Func_02000184(void) {
  struct Actor *object, *leader;
  u32 i;
  volatile u16 *color;

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(MUS_026);
  Sound_Play(SE_POWER_UP);
  Actor_SetModeById(8, 3);
  PaletteFade_ApplySource(0x10000, 2);
  PaletteFade_ApplyTarget(0x10000, 2);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 1);
  BattleEffect_ApplyColorToTargetBuffer(0x403108, 1);
  PaletteFade_StartInterpolation(40);
  wait(40);
  Sound_Play(SE_197);
  wait(1);
  if (gData_02002058 != 0) {
    i = 0;
  rise:
    if (i == 20)
      Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    gData_02002058->y += 0x8000;
    wait(1);
    if (++i <= 79)
      goto rise;
    gData_02002058->motionFlags = 4;
  }
  Actor_SetModeById(8, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 358, 108);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(9, 0.8);
  Actor_WalkToAndWait(9, 364, 124);
  Actor_FaceDirection(9, FACING_NORTH_NORTHEAST, 0);
  Event_SetMessageId(MsgOhYoungWitch);
  Func_02000100();
  Actor_ArmCallback(10, 0, 20);
  Actor_FaceDirection(10, FACING_WEST, 40);
  Actor_ArmCallback(10, 0, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 10, 0);
  Actor_SetAttachedEffect(9, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageAndWait(9, 0);
  Actor_FaceDirectionAndWait(10, FACING_NORTH_NORTHWEST);
  Actor_Jump(10, 4, 20);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 10, 0);
  Func_02000100();
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHWEST, 40);
  Func_02000100();
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(9, FACING_SOUTH_SOUTHEAST, 0);
  Actor_SetModeById(8, 3);
  Actor_ArmCallback(10, 0x3000, 20);
  object = OBJECT(14);
  object->facing = OBJECT(10)->facing;
  wait(1);
  Actor_MoveToObject(14, 10);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  Sound_Play(SE_CHIME);
  Actor_SetModeById(14, 5);
  Event_Wait(60);
  Event_Wait(20);
  Actor_FaceDirection(14, FACING_NORTH_NORTHWEST, 0);
  Actor_SetModeById(8, 1);
  Event_Wait(40);
  Actor_SetModeById(8, 3);
  Sound_Play(SE_CHIME);
  Actor_SetModeById(14, 6);
  Event_Wait(60);
  Actor_SetModeById(14, 1);
  Event_Wait(20);
  Actor_SetModeById(8, 1);
  Event_Wait(40);
  Func_02000100();
  Actor_Jump(14, 2, 10);
  Actor_Jump(14, 4, 10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 14, 0);
  Actor_SetMovementTypeAndWaitById(14, gData_02001f78);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 14, 0);
  Actor_SetSpeed(14, 1.6);
  Actor_WalkToAndWait(14, 392, 164);
  Actor_ShowEmoteById(14, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_Jump(14, 6, 20);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | 14,
                           0);
  Actor_WalkToAndWait(14, 392, 220);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ArmCallback(9, 0, 0);
  OBJECT(12)->motionFlags = 0;
  Actor_SetMovementTypeAndWaitById(12, gData_02001f9c);
  Event_Wait(20);
  Func_02000100();
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHEAST, 80);
  Func_02000100();
  Event_Wait(20);
  Func_02000100();
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 20);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x8000, 40);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 20);
  Actor_SetModeById(8, 3);
  PaletteFade_ApplySource(0x10000, 2);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 2);
  /* FAKEMATCH(temporary): the palette entry's address taken before the fade's
   * wait. */
  color = &OBJ_PLTT_COLOR(0, 15);
  PaletteFade_StartInterpolation(10);
  wait(10);
  *color = 0x7fff;
  wait(1);
  FieldMessage_ShowCentered((s16)(*(u16 *)(EVENT_WORK + 0x1c4))++, 0, 2);
  PaletteFade_ApplyTarget(0x10000, 2);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Actor_SetModeById(8, 1);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Actor_ShowEmoteById(gMainRoutineWork.currentOwner,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHWEST, 20);
  Func_02000100();
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHEAST, 0);
  Func_02000100();
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Func_02000100();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ArmCallback(9, 0, 0);
  Actor_SetMovementTypeAndWaitById(12, gData_02001fe4);
  Func_02000100();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(9, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHEAST, 20);
  Func_02000100();
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHWEST, 20);
  Func_02000100();
  Actor_FaceDirectionAndWait(9, FACING_NORTH_NORTHEAST);
  Event_ShowMessageAndWait(9, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Actor_ShowEmoteById(13, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Func_02000100();
  Event_ShowMessageThenWait(9, 0, 40);
  Func_02000100();
  Actor_Jump(9, 4, 10);
  Event_ShowMessageAndWait(9, 0);
  Func_02000100();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x4000, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTH_NORTHWEST);
  Motion_SetModeAndWaitAnimation(9, 3);
  Func_02000100();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xe000, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTH_NORTHEAST);
  Actor_SetModeById(9, 4);
  Event_ShowMessageAndWait(9, 0);
  Func_02000100();
  Sound_Play(MUS_000);
  Sound_Play(SE_SHIMMER);
  Actor_SetModeById(8, 3);
  Actor_SetSpriteColor(((s32)Actor_GetById(13)), 7);
  Actor_SetPosition(13, 424, 104);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  PaletteFade_StartInterpolation(40);
  wait(40);
  Func_02001094();
  Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  Event_Wait(1);
  PaletteFade_ApplyTarget(0x10000, 2);
  BattleEffect_ApplyColorToTargetBuffer(0x403108, 1);
  PaletteFade_StartInterpolation(60);
  wait(60);
  Func_02000100();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
  PaletteFade_StartInterpolation(20);
  wait(20);
  gData_02002058->motionFlags = 3;
  Event_Wait(10);
  Sound_Play(SE_STAR_BOUNCE);
  Event_Wait(40);
  Sound_Play(MUS_025);
  Event_Wait(40);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x2000, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTH_NORTHWEST);
  Event_ShowMessageAndWait(9, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_FaceDirection(9, FACING_NORTH_NORTHEAST, 0);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 392, 104);
  Actor_FaceDirectionById(gMainRoutineWork.currentOwner, FACING_NORTH);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  if (gData_02002058 != 0) {
    leader = OBJECT(gMainRoutineWork.currentOwner);
    object = gData_02002058;
    object->motionFlags = 0;
    Actor_SetMoveTarget((s32)object, leader->x, leader->y + 0x240000,
                        leader->z);
    Actor_WaitForMove((s32)gData_02002058);
  }
  PartyInventory_GiveItem(ITEM_BLACK_CRYSTAL, 0);
  if (gData_02002058 != 0)
    Actor_Release((s32)gData_02002058);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x6000, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHEAST, 40);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetModeById(9, 4);
    Event_ShowMessageAndWait(9, 0);
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
  } else {
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
    Actor_SetModeById(9, 3);
    Event_ShowMessageAndWait(9, 0);
  }
  Actor_WalkToAndWaitById(9, 376, 124);
  Event_Wait(20);
  Event_ShowMessageAndWait(9, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTH_NORTHEAST);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Event_ShowMessageAndWait(9, 0);
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
  } else {
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
    Event_ShowMessageAndWait(9, 0);
  }
  Actor_SetModeById(9, 4);
  Event_ShowMessageAndWait(9, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Event_ShowMessageAndWait(9, 0);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_ShowMessageAndWait(9, 0);
  Event_ShowMessageAndWait(9, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetMovementTypeById(9, gData_0200200c);
  Event_Wait(20);
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner, gData_0200200c);
  Event_Wait(40);
  BattleEffect_ApplyColorToTargetBuffer(0, 0);
  PaletteFade_StartInterpolation(60);
  wait(64);
  *(s32 *)(EVENT_WORK + 0x1ac) = 513;
  *(s32 *)(EVENT_WORK + 0x1b4) = 10;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  *(s32 *)(EVENT_WORK + 0x1b4) = 1;
  PaletteFade_StartInterpolation(1);
  ActorSystem_SuspendProjectionAndShowSprites();
  FieldMessage_ShowCentered(MsgCarriedTheMessage, 0, 3);
  ActorSystem_ResumeProjectionAndHideLayers();
  GameFlag_Set(0x8ff);
  GameFlag_Clear(0x86b);
  GameFlag_Clear(0x8f2);
  gMainRoutineWork.retreatScene = (s32)&SceneId_Kibombo1;
  gMainRoutineWork.retreatEntrance = 1;
  gMainRoutineWork.savedScene = (s32)&SceneId_Kibombo3;
  gMainRoutineWork.savedEntrance = 6;
  Import_EventSetExitRequest(3);
}

void Func_02000a98(s32 x, s32 z) {
  struct GabombaStatueChamberEventWork *work = Ram_HeapSlots->eventWork;
  struct GabombaStatueChamberObject *object =
      ((struct GabombaStatueChamberObject *)Actor_GetById(
          gMainRoutineWork.currentOwner));

  Event_Start();
  Event_ResolvePendingActions(0);
  object->motionFlags = 0;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, x, z);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_RefreshSelectorById(gMainRoutineWork.currentOwner);
  Actor_SetSpriteFlags(((struct GabombaStatueChamberObject *)Actor_GetById(
                           gMainRoutineWork.currentOwner)),
                       0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 13);
  Actor_SetMoveTarget(object, x << 16, PIXELS(-12), z << 16);
  Actor_WaitForMove(object);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 10);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}

void Func_02000b60(void) { Func_02000a98(312, 144); }

void Func_02000b70(void) {
  struct GabombaStatueChamberObject *leader =
      ((struct GabombaStatueChamberObject *)Actor_GetById(
          gMainRoutineWork.currentOwner));
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (x == 26) {
    if (z == 5 && (gInput.held & KEY_DOWN) == 0)
      return;
    if (z == 7 && (gInput.held & KEY_UP) == 0)
      return;
  } else if (z == 6) {
    if (x == 25 && (gInput.held & KEY_RIGHT) == 0)
      return;
    if (x == 27 && (gInput.held & KEY_LEFT) == 0)
      return;
  }
  Func_02000a98(424, 100);
}

void BlendScript_Suspend(void);

void Func_02000be8(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *lower = map + 0x108;
  u8 *upper = map + 0x178;
  s32 i, step;

  Event_Start();
  Event_ResolvePendingActions(0);
  BlendScript_Suspend();
  step = 1;
  for (i = 0; i < 352; i++) {
    if (i >= 17 && i < 80) {
      s32 blend = ((step + 1) / 4) << 8;

      blend |= 16 - (step + 1) / 4;
      REG_BLDALPHA = blend;
      step++;
    }
    *(s32 *)(lower + 12) += 0x10000;
    *(s32 *)(upper + 12) += 0x10000;
  }
  Map_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 328, 184);
  Event_Wait(20);
  Sound_Play(SE_MATERIALIZE);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  for (; i != 0; i--) {
    if (i >= 33 && i < 96) {
      s32 blend = ((step + 1) / 4) << 8;

      blend |= 16 - (step + 1) / 4;
      REG_BLDALPHA = blend;
      step--;
    }
    *(s32 *)(lower + 12) += -0x10000;
    *(s32 *)(upper + 12) += -0x10000;
    wait(1);
  }
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Sound_Play(SE_298);
  wait(1);
  Sound_Play(SE_CHEST_CLOSE);
  Event_Wait(20);
  GameFlag_Set(FLAG_SCENE_TEMP_06);
  Event_End();
}

void Func_02000d3c(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *lower = map + 0x108;
  u8 *upper = map + 0x178;
  s32 i, step;

  Event_Start();
  Event_ResolvePendingActions(0);
  BlendScript_Suspend();
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetDestinationOffsetById(gMainRoutineWork.currentOwner, 0, -8);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_FaceDirectionById(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Sound_Play(SE_MATERIALIZE);
  step = 1;
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  for (i = 0; i <= 351; i++) {
    if ((u32)(i - 17) <= 62) {
      s32 blend = ((step + 1) / 4) << 8;

      blend |= 16 - (step + 1) / 4;
      REG_BLDALPHA = blend;
      step++;
    }
    *(s32 *)(lower + 12) += 0x10000;
    *(s32 *)(upper + 12) += 0x10000;
    wait(1);
  }
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Sound_Play(SE_298);
  wait(1);
  Sound_Play(SE_CHEST_CLOSE);
  Event_Wait(20);
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 1;
  GameFlag_Set(FLAG_SCENE_TEMP_06);
  GameFlag_Set(FLAG_SCENE_TEMP_07);
  Event_End();
}

void Func_02000e50(void) { GameFlag_Clear(FLAG_SCENE_TEMP_06); }

void Func_02000e60(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveWithinMapBounds(PIXELS(392), -1, PIXELS(120), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Event_SetMessageId((s32)MsgIHaveFollowed);
  Actor_SetAttachedEffect(10, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageAndWait(10, 0);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  Event_ShowMessageAndWait(10, 0);
  Actor_FaceDirectionById(10, FACING_EAST);
  Event_ShowMessageAndWait(10, 0);
  Actor_FaceDirectionAndWait(10, FACING_NORTH + FACING_STEP);
  Motion_SetModeAndWaitAnimation(10, 4);
  Event_ShowMessageAndWait(10, 0);
  GameFlag_Set(0x8fe);
  Event_End();
}

void Func_02000ef0(struct Actor *object) { object->x = Actor_GetById(8)->x; }
#undef EVENT_WORK
#undef OBJECT

#define DEFINE_GABOMBA_STATUE_CHAMBER_SHOW_ITEM(name, n1)                      \
  void name(void) {                                                            \
    struct GabombaStatueChamberObject *leader =                                \
        ((struct GabombaStatueChamberObject *)Actor_GetById(8));               \
    s32 x = leader->x;                                                         \
    s32 z = leader->z;                                                         \
    /* FAKEMATCH(temporary): a local holding the 0 for the three byte stores,  \
     * set before the null test. */                                            \
    s32 zero = 0;                                                              \
    struct GabombaStatueChamberObject *object;                                 \
                                                                               \
    if ((gData_02002058 = object =                                             \
             ((struct GabombaStatueChamberObject *)Import_ActorCreate(         \
                 489, x, 0x120000, z))) != NULL) {                             \
      struct FieldSprite *sprite = object->sprite;                             \
      u8 *block;                                                               \
                                                                               \
      sprite->fullColor = 0;                                                   \
      sprite->palette = 0;                                                     \
      sprite->priority = 2;                                                    \
      object->motionFlags = zero;                                              \
      object->field5c = 1;                                                     \
      object->update = Func_02000ef0;                                          \
      sprite->nextFrame = zero;                                                \
      sprite->unk1b[0] = zero;                                                 \
      block = HeapSlot_Alloc(68, 0x608);                                       \
      Icon_DrawItem(n1);                                                       \
      VramCache_Load(sprite->vramBlock, 128, block + 0x400);                   \
      HeapSlot_Release(68);                                                    \
    }                                                                          \
  }

DEFINE_GABOMBA_STATUE_CHAMBER_SHOW_ITEM(Func_02000f00, 230)

DEFINE_GABOMBA_STATUE_CHAMBER_SHOW_ITEM(Func_02000f88, 242)

void Func_02001010(void) {
  MapGrid_CopyCellPictureIndicesRect(85, 78, 22, 79, 5, 11);
  MapGrid_CopyCellAttributes(85, 78, 5, 11, 22, 15);
  MapGrid_CopyCellsTo(86, 30, 86, 20, 5, 2);
  MapGrid_CopyCellPictureIndicesRect(86, 30, 84, 22, 9, 2);
}

void Func_02001064(void) {
  Map_CopyCellAttributeRect(23, 1, 3, 1, 23, 9);
  Map_CopyCellAttributeRect(23, 0, 3, 1, 25, 9);
}

void Func_02001094(void) {
  MapGrid_CopyCellPictureIndicesRect(0, 1, 26, 6, 1, 1);
  MapGrid_CopyCellPictureIndicesRect(0, 65, 26, 70, 1, 1);
  MapGrid_CopyCellAttributes(0, 2, 3, 3, 25, 5);
}

// The scene after the statue is lifted in the Gabomba Statue's chamber.

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

extern u32 gData_02001f18[];

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

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldActor_SetPositionAndResetMotion(((s32)Actor_GetById(13)), PIXELS(392), 0,
                                       PIXELS(268));
  wait(1);
  /* FAKEMATCH(temporary): the leader read before the first turn is built. */
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
    block = (u8 *)HeapSlot_Alloc(68, 0x608);
    Icon_DrawItem(ITEM_BLACK_CRYSTAL);
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
  Actor_SetAttachedEffect(6, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirectionAndWait(6, FACING_WEST);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 6, 0);
  Actor_ShowEmoteById(5, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Event_ShowMessageAndWait(5, 0);
  FieldActor_SetPositionAndResetMotion(((s32)Actor_GetById(13)), PIXELS(392), 0,
                                       PIXELS(340));
  Sound_Play(SOUND_FADE_OUT_BGM);
  BattleEffect_ApplyColorToTargetBuffer(0x10002, 0);
  PaletteFade_StartInterpolation(40);
  wait(40);
  Actor_SetModeById(9, 2);
  Actor_SetModeById(10, 2);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 13);
  Actor_WatchAndSetScript(5, 13);
  Actor_WatchAndSetScript(6, 13);
  Actor_WatchAndSetScript(7, 13);
  Actor_WatchAndSetScript(11, 13);
  Event_Wait(10);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 246, 1);
  Actor_SetSpeed(13, 0.8);
  Actor_MoveToAndWait(13, 392, 246);
  Actor_SetModeById(9, 1);
  Actor_SetModeById(10, 1);
  statue->velocityY = PIXELS(3);
  statue->gravity = 0x8000;
  Actor_SetSpeed(13, 1.6);
  Actor_MoveToAndWait(13, 392, 234);
  Event_Wait(10);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 168, 1);
  Actor_SetSpeed(13, 0.8);
  Actor_MoveToAndWaitById(13, 392, 160);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(5);
  Actor_Stop(6);
  Actor_Stop(7);
  Actor_Stop(11);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  Event_Wait(20);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(40);
  wait(40);
  Scene_PlayMusic();
  Actor_ShowEmoteById(11, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 11, 0);
  Actor_ShowEmoteById(6, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 6, 0);
  Actor_FaceDirectionById(5, turnA);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 5, 0);
  Actor_ArmCallback(5, FACING_NORTHWEST, 0);
  Actor_SetSpeed(6, 0.8);
  Actor_WalkToAndWait(6, 400, 172);
  Actor_SetModeById(6, 9);
  Event_Wait(20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 6, 0);
  Actor_SetModeById(7, 4);
  Event_ShowMessageAndWait(7, 0);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 6, 0);
  Actor_ShowEmoteById(11, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 11, 0);
  Actor_FaceDirectionAndWait(11, FACING_SOUTHEAST + FACING_STEP);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 11, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Sound_Play(MUS_000);
  {
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
  FieldCamera_MoveWithinMapBounds(PIXELS(392), -1, PIXELS(200), 1);
  Actor_SetSpeed(12, 1.2);
  Actor_WalkToAndWait(12, 404, 220);
  Actor_Jump(12, 4, 10);
  FieldMessage_ShowAtActor(12, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    FieldEffect_SetAttached(12,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(20);
    Event_ShowMessageAndWait(12, 0);
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
  } else {
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
    Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
    Event_ShowMessageAndWait(12, 0);
  }
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
  Event_ShowMessageAndWait(7, 0);
  Actor_ShowEmoteById(12, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessageAndWait(12, 0);
  Actor_ShowEmoteById(6, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
  Event_ShowMessageAndWait(6, 0);
  FieldEffect_SetAttached(12, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_ShowMessageAndWait(12, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  Motion_SetModeAndWaitAnimation(11, 3);
  Actor_SetModeById(12, 4);
  Event_ShowMessageAndWait(12, 0);
  Actor_ShowEmoteById(5, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Actor_SetSpeed(5, 0.8);
  Actor_WalkToAndWait(5, 420, 228);
  Actor_FaceDirectionById(5, FACING_NORTHWEST);
  Event_Wait(10);
  Actor_SetSpeed(5, 1.6);
  Actor_WalkToAndWait(5, 412, 222);
  Actor_SetSpeed(5, 1);
  Actor_SetSpeed(12, 1);
  Actor_SetMovementTypeById(5, gData_02001f18);
  Event_Wait(10);
  Actor_SetMovementTypeById(12, gData_02001f18);
  Event_Wait(60);
  *(s32 *)(EVENT_WORK + 0x1ac) = 514;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  Import_EventSetExitRequest(2);
}

extern u32 gData_02001f4c[];

void Func_02001710(void) {
  s32 *owner;
  s32 facing;
  s32 angleC;
  s32 zero;
  s32 turn;
  struct HeapSlots *heap;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  System_Wait(1);
  Func_02001010();
  Map_CenterOnFollowedPoint();
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
  Actor_FaceDirection(12, FACING_NORTH_NORTHEAST, 20);
  Actor_FaceDirectionAndWait(12, FACING_SOUTH_SOUTHWEST);
  Actor_ShowEmoteById(12, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(11, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(6, facing, 0);
  Actor_SetSpeed(12, 0.8);
  Actor_WalkToAndWait(12, 392, 168);
  Event_PlaySound(SE_HIDDEN_PASSAGE_OPEN);
  Func_02000f00();
  FieldEvent_Wait(40);
  Event_PlaySound(SE_PUSH);
  {
    struct Actor *guard = Actor_GetById(8);

    guard->motionFlags = zero;
  }
  Func_02001064();
  Actor_SetSpeed(8, 0.4);
  Actor_MoveToAndWait(8, 424, 152);
  Actor_FaceDirectionAndWait(12, FACING_SOUTH_SOUTHWEST);
  FieldEvent_SetMessageId((s32)MsgIMightBe);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 12, 0);
  Actor_SetAnimationAndWait(12, 3);
  Actor_WalkToAndWait(12, 392, 140);
  System_Wait(1);
  Actor_SetPosition(12, 0, 0);
  System_Wait(1);
  Actor_ShowEmoteById(5, turn, 20);
  Actor_FaceDirection(5, angleC, 0);
  FieldMessage_ShowAtActor(SPEAKER_WINDOW_ABOVE | 5, 0);
  if (Event_AskYesNoWithActorAnimation(*owner, 0) == 0) {
    Actor_FaceDirectionAndWait(7, FACING_EAST);
    Actor_SetAnimation(7, 3);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 7, 0);
    ((struct GroupWork *)heap->eventWork)->unk1c4++;
  } else {
    ((struct GroupWork *)heap->eventWork)->unk1c4++;
    Actor_FaceDirectionAndWait(7, angleC);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 7, 0);
    Func_02001f08();
  }
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_SetAttachedEffect(6, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 6, 0);
  Actor_FaceDirectionAndWait(11, FACING_EAST);
  Actor_SetAnimation(11, 4);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 11, 0);
  Actor_FaceDirectionAndWait(11, FACING_NORTH_NORTHEAST);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 11, 0);
  Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(11, 3);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 11, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_FaceDirection(7, FACING_WEST, 20);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(6, 3);
  Actor_SetSpeed(11, 0.8);
  Actor_SetSpeed(7, 0.8);
  Actor_SetSpeed(5, 1);
  Actor_SetSpeed(6, 1.2);
  {
    s32 path = (s32)gData_02001f4c;

    Actor_SetScript(11, path);
    Actor_SetScript(7, path);
    Actor_SetScript(5, path);
    Actor_RunScriptAndWait(6, path);
  }
  Event_SetFlag(0x8fd);
  Event_End();
}

s32 Scene_Initialize(void) {
  struct GabombaStatueChamberEventWork *work;
  s16 scene;

  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 521;
  ((struct GabombaStatueChamberObject *)Actor_GetById(Party_GetLeaderActor()))
      ->priorityFlags |= 0x20;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_GabombaStatueChamber1)
    GabombaStatueChamber_InitializeRoom1();
  else if (scene == (s32)&SceneId_GabombaStatueChamber2)
    GabombaStatueChamber_InitializeRoom2();
  else if (scene == (s32)&SceneId_GabombaStatueChamber3)
    GabombaStatueChamber_InitializeRoom3();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void GabombaStatueChamber_InitializeRoom1(void) {
  Actor_DisableById(8);
  Actor_DisableById(9);
  Actor_DisableById(10);
  wait(1);
  if (GameFlag_Get(0x8fd)) {
    Func_02001010();
    Func_02001064();
    Actor_SetPosition(8, 424, 152);
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
      GameFlag_Clear(0xa27);
      GameFlag_Set(0xa28);
    }
  }
}

void GabombaStatueChamber_InitializeRoom2(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    if (GameFlag_Get(FLAG_SCENE_TEMP_07)) {
      struct GabombaStatueChamberMapWork *map =
          (struct GabombaStatueChamberMapWork *)Ram_HeapSlots->mapWork;
      struct GabombaStatueChamberLayer *front = &map->layers[0];
      struct GabombaStatueChamberLayer *back = &map->layers[2];
      s32 blend = 1;
      s32 i;

      BlendScript_Suspend();
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
      Map_CenterOnFollowedPoint();
      wait(1);
    }
  } else if (gMainRoutineWork.entrance == 2) {
    Func_02000be8();
  }
}

/// Gives actors 15 to 32 action variant 2, runs Actor_DisableById for actor 8
/// and grounds it, then Func_02001094 with actor 10 placed when flag 0x8ff is
/// set, else Func_02000f88; arriving by entrance 2 it sets the retreat to
/// GabombaStatue7's entrance 1 (flag 0xa27) or GabombaStatueChamber1's entrance
/// 3 (flag 0xa28).
void GabombaStatueChamber_InitializeRoom3(void) {
  u32 i;
  char *scene;

  for (i = 0; i <= 17; i++)
    Actor_SetSpritePriority(i + 15, 2);
  Actor_DisableById(8);
  Actor_GetById(8)->y = 0;
  if (GameFlag_Get(0x8ff)) {
    Func_02001094();
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  } else {
    Func_02000f88();
  }
#if TLA_PATCH_1 /* The Japanese edition does not set the retreat. */
  if (gMainRoutineWork.entrance == 2) {
    if (GameFlag_Get(0xa27)) {
      /* FAKEMATCH(temporary): the scene id through a local, which issues its
       * literal load first. */
      scene = &SceneId_GabombaStatue7;
      gMainRoutineWork.retreatScene = (s32)scene;
      gMainRoutineWork.retreatEntrance = 1;
    } else if (GameFlag_Get(0xa28)) {
      /* FAKEMATCH(temporary): the scene id through a local. */
      scene = &SceneId_GabombaStatueChamber1;
      gMainRoutineWork.retreatScene = (s32)scene;
      gMainRoutineWork.retreatEntrance = 3;
    }
  }
#endif
}
#undef EVENT_WORK

#undef EVENT_WORK
#undef OBJECT
#undef DEFINE_GABOMBA_STATUE_CHAMBER_SHOW_ITEM
#undef gData_02002058
#include "text/msg_ids.h"

void Func_02000e50(void);
void Func_02000e60(void);

// clang-format off
u32 gData_02001f18[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(428), 0, PIXELS(234)),
    SC_SYNC,
    SC_MOVE(PIXELS(488), 0, PIXELS(234)),
    SC_SYNC,
    SC_END,
};

u32 gData_02001f4c[] = {
    39, 2, 4, 0x1680000, 0, 0xd80000,
    1, 39, 1, 3, 17,
};

u32 gData_02001f78[] = {
    SC_ADD(SW_FACING, 0x800),
    SC_WAIT(1),
    SC_LOOP(32, SC_TOP),
    SC_END,
};

s32 gData_02001f9c[] = {
    SC_POSITION(PIXELS(392), PIXELS(64), PIXELS(124)),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_Y, PIXELS(-1)),
    SC_WAIT(1),
    SC_LOOP(64, SC_TO(1)),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02001fe4[] = {
    23, 6, 0x20000, 0, 1, 13,
    80, 0, 3, 17,
};

DEFINE_G_DATA_0200200C(gData_0200200c, 392, 148, 392, 236)

s32 gData_02002048[] = {
    SC_SET(SW_SCALE_X, -0x10000),
    SC_END,
};

u32 gData_02002058[] = {
    0,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 392, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gData_0200a08c[] = {
    0x1001d0, 0x1e00090, 0xa00020, 0x2ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(GabombaStatueChamber1),
    SCENE_EXIT_UNLESS(1, GabombaStatue5, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, GabombaStatue6, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Kibombo2, 4, 0x86b),
    SCENE_EXIT_UNLESS(3, Kibombo5, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, GabombaStatueChamber2, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(GabombaStatueChamber2),
    SCENE_EXIT(1, GabombaStatueChamber1, 4),
    SCENE_EXIT(2, GabombaStatueChamber3, 1),
    SCENE_EXITS(GabombaStatueChamber3),
    SCENE_EXIT(1, GabombaStatueChamber2, 2),
    SCENE_EXIT(2, GabombaCatacombs1, 1),
    SCENE_EXIT(3, Kibombo3, 9),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_020020f8[] = {
    {375, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {361, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(256), FACING_EAST, 2, 0},
    {361, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(256), FACING_EAST, 2, 1},
    {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {26, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020021e8[] = {
    {375, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(88), FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {26, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(136), FACING_NORTH + FACING_STEP, 2, 0},
    {489, CONDITION_ALWAYS, 7, PIXELS(392), 0, PIXELS(84), FACING_SOUTH, 2, 0},
    {487, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {337, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {27, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH + FACING_STEP, 2, 0},
    {330, CONDITION_ALWAYS, 1, PIXELS(336), 0, PIXELS(264), FACING_SOUTH, 2, 0},
    {330, CONDITION_ALWAYS, 1, PIXELS(352), 0, PIXELS(264), FACING_SOUTH, 2, 1},
    {330, CONDITION_ALWAYS, 1, PIXELS(368), 0, PIXELS(264), FACING_SOUTH, 2, 1},
    {330, CONDITION_ALWAYS, (s32)gData_02002048, PIXELS(416), 0, PIXELS(264), FACING_SOUTH, 2, 1},
    {330, CONDITION_ALWAYS, (s32)gData_02002048, PIXELS(432), 0, PIXELS(264), FACING_SOUTH, 2, 1},
    {330, CONDITION_ALWAYS, (s32)gData_02002048, PIXELS(448), 0, PIXELS(264), FACING_SOUTH, 2, 1},
    {330, CONDITION_ALWAYS, 1, PIXELS(336), 0, PIXELS(328), FACING_SOUTH, 2, 1},
    {330, CONDITION_ALWAYS, 1, PIXELS(352), 0, PIXELS(328), FACING_SOUTH, 2, 1},
    {330, CONDITION_ALWAYS, 1, PIXELS(368), 0, PIXELS(328), FACING_SOUTH, 2, 1},
    {330, CONDITION_ALWAYS, (s32)gData_02002048, PIXELS(416), 0, PIXELS(328), FACING_SOUTH, 2, 1},
    {330, CONDITION_ALWAYS, (s32)gData_02002048, PIXELS(432), 0, PIXELS(328), FACING_SOUTH, 2, 1},
    {330, CONDITION_ALWAYS, (s32)gData_02002048, PIXELS(448), 0, PIXELS(328), FACING_SOUTH, 2, 1},
    {330, CONDITION_ALWAYS, 1, PIXELS(336), 0, PIXELS(392), FACING_SOUTH, 2, 1},
    {330, CONDITION_ALWAYS, 1, PIXELS(352), 0, PIXELS(392), FACING_SOUTH, 2, 1},
    {330, CONDITION_ALWAYS, 1, PIXELS(368), 0, PIXELS(392), FACING_SOUTH, 2, 1},
    {330, CONDITION_ALWAYS, (s32)gData_02002048, PIXELS(416), 0, PIXELS(392), FACING_SOUTH, 2, 1},
    {330, CONDITION_ALWAYS, (s32)gData_02002048, PIXELS(432), 0, PIXELS(392), FACING_SOUTH, 2, 1},
    {330, CONDITION_ALWAYS, (s32)gData_02002048, PIXELS(448), 0, PIXELS(392), FACING_SOUTH, 2, 1},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02002458[] = {
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gData_02002470[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS,
     EVENT(Func_02000b60)},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_020024a0[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 3, 0x206, EVENT(Func_02000d3c)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Func_02000e50)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_020024dc[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING, 2, CONDITION_ALWAYS, EVENT(Func_02000b70)},
    {TRIGGER_STEP, 10, 0x8fe, EVENT(Func_02000e60)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgYourBlackOrb)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgOeiaWhatAm)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 11, 0x8ff, EVENT(Func_02000184)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02002530[] = {
    {TRIGGER_TABLE_END},
};
