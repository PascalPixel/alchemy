/// @file
/// The Gabomba Statue's inner chamber, in three rooms of this overlay: the way
/// down by the lift, and the chamber where the party finds the Black Crystal
/// gone (GabombaStatueChamber_WhereIsTheOrb), Akafubu offers to retrieve it
/// (GabombaStatueChamber_AkafubuOffersToHelp), reaches the chamber
/// (GabombaStatueChamber_AkafubuReachesChamber), and the Great Gabomba speaks,
/// naming him witch doctor (GabombaStatueChamber_GabombaNamesWitchDoctor).
#include "flags.h"
#include "item_ids.h"

#define FLAG_AKAFUBU_REACHED_CHAMBER 0x8fe
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "gamepak/far_call/system.h"
#include "scene/scene.h"
#include "system/system_bank.h"
#include "shared/gabomba_statue_chamber.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "system/input.h"
#include "field/field_sprite.h"
#include "system/io_reg.h"
#include "version.h"
#include "shared/tables.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "text/msg_ids.h"

void GabombaStatueChamber_OpenTheLap(void);

void GabombaStatueChamber_KeepItemWithStatue();
void GabombaStatueChamber_InitializeRoom1(void);
void GabombaStatueChamber_InitializeRoom2(void);

void GabombaStatueChamber_InitializeRoom3(void);

void GabombaStatueChamber_ArriveByLift(void);
void GabombaStatueChamber_GabombaNamesWitchDoctor(void);
void GabombaStatueChamber_StepIntoFirstHole(void);
void GabombaStatueChamber_StepIntoSecondHole(void);
void GabombaStatueChamber_RideLiftUp(void);

extern s32 gGabombaStatueChamber_MirroredScript[];

void ActorSystem_SuspendProjectionAndShowSprites(void);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

u8 *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_GABOMBA_STATUE_CHAMBER_1)
    return gGabombaStatueChamber_Room1Regions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_GABOMBA_STATUE_CHAMBER_1)
    return gGabombaStatueChamber_Room1Placements;
  if (scene == SCENE_GABOMBA_STATUE_CHAMBER_3)
    return gGabombaStatueChamber_Room3Placements;
  return gGabombaStatueChamber_Placements;
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_GABOMBA_STATUE_CHAMBER_1)
    return gGabombaStatueChamber_Room1Triggers;
  if (scene == SCENE_GABOMBA_STATUE_CHAMBER_2)
    return gGabombaStatueChamber_Room2Triggers;
  if (scene == SCENE_GABOMBA_STATUE_CHAMBER_3)
    return gGabombaStatueChamber_Room3Triggers;
  return gGabombaStatueChamber_Triggers;
}

void GabombaStatueChamber_StatueSpeaks(void) {
  volatile u16 *color = (volatile u16 *)&OBJ_PLTT_COLOR(0, 15);
  struct FieldWork *work;

  Actor_SetAnimationById(8, 2);
  PaletteFade_ApplySource(0x10000, 2);
  PaletteFade_SetTarget(0x10003, 2);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  work = Ram_HeapSlots->fieldWork;
  FieldMessage_ShowCentered((s16)work->message++, 0, 2);
  PaletteFade_SetTarget(0x10000, 2);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Actor_SetAnimationById(8, 1);
}

#define EVENT_WORK ((u8 *)Ram_HeapSlots->fieldWork)

/// The item shown above the statue, while it is there.
extern struct Actor *gGabombaStatueChamber_ShownItem;
extern u32 gGabombaStatueChamber_SpiritRisesScript[];
extern s32 gGabombaStatueChamber_AkafubuStepsUpScript[];
extern u32 gGabombaStatueChamber_AkafubuLeavesScript[];
extern u32 gGabombaStatueChamber_PartyLeavesScript[];
#define OBJECT(id) (Actor_Get((id)))
void ActorSystem_ResumeProjectionAndHideLayers(void);

void GabombaStatueChamber_GabombaNamesWitchDoctor(void) {
  struct Actor *object, *leader;
  u32 i;
  volatile u16 *color;

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(MUS_JUPITER_LIGHTHOUSE);
  Sound_Play(SE_POWER_UP);
  Actor_SetAnimationById(8, 3);
  PaletteFade_ApplySource(0x10000, 2);
  PaletteFade_ApplyTarget(0x10000, 2);
  PaletteFade_SetSource(0x10000, 1);
  PaletteFade_SetTarget(0x403108, 1);
  PaletteFade_StartInterpolation(40);
  wait(40);
  Sound_Play(SE_197);
  wait(1);
  if (gGabombaStatueChamber_ShownItem != 0) {
    i = 0;
  rise:
    if (i == 20)
      Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    gGabombaStatueChamber_ShownItem->y.fixed += 0x8000;
    wait(1);
    if (++i <= 79)
      goto rise;
    gGabombaStatueChamber_ShownItem->motionFlags = 4;
  }
  Actor_SetAnimationById(8, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 358, 108);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_MoveToObject(9, gMainRoutineWork.leaderActorId);
  wait(1);
  Actor_SetSpeed(9, 0.8);
  Actor_WalkToAndWait(9, 364, 124);
  Actor_FaceDirection(9, FACING_NORTH_NORTHEAST, 0);
  Event_SetMessageId(MsgOhYoungWitch);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirectionById(10, FACING_EAST, 20);
  Actor_FaceDirection(10, FACING_WEST, 40);
  Actor_FaceDirectionById(10, FACING_EAST, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 10, 0);
  Actor_SetAttachedEffect(9, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageAndWait(9, 0);
  Actor_FaceDirectionAndWait(10, FACING_NORTH_NORTHWEST);
  Actor_Jump(10, 4, 20);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 10, 0);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHWEST, 40);
  GabombaStatueChamber_StatueSpeaks();
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(9, FACING_SOUTH_SOUTHEAST, 0);
  Actor_SetAnimationById(8, 3);
  Actor_FaceDirectionById(10, FACING_SOUTH_SOUTHEAST, 20);
  object = OBJECT(14);
  object->facing = OBJECT(10)->facing;
  wait(1);
  Actor_MoveToObject(14, 10);
  Actor_SetPosition(10, 0, 0);
  Sound_Play(SE_CHIME);
  Actor_SetAnimationById(14, 5);
  Event_Wait(60);
  Event_Wait(20);
  Actor_FaceDirection(14, FACING_NORTH_NORTHWEST, 0);
  Actor_SetAnimationById(8, 1);
  Event_Wait(40);
  Actor_SetAnimationById(8, 3);
  Sound_Play(SE_CHIME);
  Actor_SetAnimationById(14, 6);
  Event_Wait(60);
  Actor_SetAnimationById(14, 1);
  Event_Wait(20);
  Actor_SetAnimationById(8, 1);
  Event_Wait(40);
  GabombaStatueChamber_StatueSpeaks();
  Actor_Jump(14, 2, 10);
  Actor_Jump(14, 4, 10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 14, 0);
  Actor_SetMovementTypeAndWaitById(14, gGabombaStatueChamber_SpiritRisesScript);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 14, 0);
  Actor_SetSpeed(14, 1.6);
  Actor_WalkToAndWait(14, 392, 164);
  Actor_ShowEmoteById(14, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_Jump(14, 6, 20);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | 14,
                           0);
  Actor_WalkToAndWait(14, 392, 220);
  Actor_SetPosition(14, 0, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(9, FACING_EAST, 0);
  OBJECT(12)->motionFlags = 0;
  Actor_SetMovementTypeAndWaitById(12,
                                   gGabombaStatueChamber_AkafubuStepsUpScript);
  Event_Wait(20);
  GabombaStatueChamber_StatueSpeaks();
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHEAST, 80);
  GabombaStatueChamber_StatueSpeaks();
  Event_Wait(20);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 20);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_WEST, 40);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 20);
  Actor_SetAnimationById(8, 3);
  PaletteFade_ApplySource(0x10000, 2);
  PaletteFade_SetTarget(0x10003, 2);
  /* FAKEMATCH(temporary): the palette entry's address taken before the fade's
   * wait. */
  color = &OBJ_PLTT_COLOR(0, 15);
  PaletteFade_StartInterpolation(10);
  wait(10);
  *color = 0x7fff;
  wait(1);
  FieldMessage_ShowCentered((s16)(((struct FieldWork *)EVENT_WORK)->message)++,
                            0, 2);
  PaletteFade_ApplyTarget(0x10000, 2);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Actor_SetAnimationById(8, 1);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHWEST, 20);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHEAST, 0);
  GabombaStatueChamber_StatueSpeaks();
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(9, FACING_EAST, 0);
  Actor_SetMovementTypeAndWaitById(12,
                                   gGabombaStatueChamber_AkafubuLeavesScript);
  GabombaStatueChamber_StatueSpeaks();
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(9, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHEAST, 20);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHWEST, 20);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirectionAndWait(9, FACING_NORTH_NORTHEAST);
  Event_ShowMessageAndWait(9, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_ShowEmoteById(13, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  GabombaStatueChamber_StatueSpeaks();
  Event_ShowMessageThenWait(9, 0, 40);
  GabombaStatueChamber_StatueSpeaks();
  Actor_Jump(9, 4, 10);
  Event_ShowMessageAndWait(9, 0);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTH_NORTHWEST);
  Actor_SetAnimationAndWait(9, 3);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTH_NORTHEAST);
  Actor_SetAnimationById(9, 4);
  Event_ShowMessageAndWait(9, 0);
  GabombaStatueChamber_StatueSpeaks();
  Sound_Play(MUS_SILENCE);
  Sound_Play(SE_SHIMMER);
  Actor_SetAnimationById(8, 3);
  Actor_SetSpriteColor(((s32)Actor_GetById(13)), 7);
  Actor_SetPosition(13, 424, 104);
  PaletteFade_SetTarget(0x7fff, 0);
  PaletteFade_StartInterpolation(40);
  wait(40);
  GabombaStatueChamber_OpenTheLap();
  Actor_SetPosition(13, 0, 0);
  Event_Wait(1);
  PaletteFade_ApplyTarget(0x10000, 2);
  PaletteFade_SetTarget(0x403108, 1);
  PaletteFade_StartInterpolation(60);
  wait(60);
  GabombaStatueChamber_StatueSpeaks();
  PaletteFade_SetTarget(0x10000, 1);
  PaletteFade_StartInterpolation(20);
  wait(20);
  gGabombaStatueChamber_ShownItem->motionFlags = 3;
  Event_Wait(10);
  Sound_Play(SE_STAR_BOUNCE);
  Event_Wait(40);
  Sound_Play(MUS_GABOMBA_STATUE);
  Event_Wait(40);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTH_NORTHWEST);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_FaceDirection(9, FACING_NORTH_NORTHEAST, 0);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 392, 104);
  Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                    FACING_NORTH);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 28);
  if (gGabombaStatueChamber_ShownItem != 0) {
    leader = OBJECT(gMainRoutineWork.leaderActorId);
    object = gGabombaStatueChamber_ShownItem;
    object->motionFlags = 0;
    Actor_SetMoveTarget((s32)object, leader->x.fixed, leader->y.fixed + px(36),
                        leader->z.fixed);
    Actor_WaitForMove((s32)gGabombaStatueChamber_ShownItem);
  }
  PartyInventory_GiveItem(ITEM_BLACK_CRYSTAL, 0);
  if (gGabombaStatueChamber_ShownItem != 0)
    Actor_Release((s32)gGabombaStatueChamber_ShownItem);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_STAND);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHEAST, 40);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Actor_SetAnimationById(9, 4);
    Event_ShowMessageAndWait(9, 0);
    ((struct FieldWork *)EVENT_WORK)->message += 1;
  } else {
    ((struct FieldWork *)EVENT_WORK)->message += 1;
    Actor_SetAnimationById(9, 3);
    Event_ShowMessageAndWait(9, 0);
  }
  Actor_WalkToAndWaitById(9, 376, 124);
  Event_Wait(20);
  Event_ShowMessageAndWait(9, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTH_NORTHEAST);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Event_ShowMessageAndWait(9, 0);
    ((struct FieldWork *)EVENT_WORK)->message += 1;
  } else {
    ((struct FieldWork *)EVENT_WORK)->message += 1;
    Event_ShowMessageAndWait(9, 0);
  }
  Actor_SetAnimationById(9, 4);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetAnimationAndWait(9, 3);
  Event_ShowMessageAndWait(9, 0);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetMovementTypeById(9, gGabombaStatueChamber_PartyLeavesScript);
  Event_Wait(20);
  Actor_SetMovementTypeById(gMainRoutineWork.leaderActorId,
                            gGabombaStatueChamber_PartyLeavesScript);
  Event_Wait(40);
  PaletteFade_SetTarget(0, 0);
  PaletteFade_StartInterpolation(60);
  wait(64);
  ((struct FieldWork *)EVENT_WORK)->transition = 513;
  ((struct FieldWork *)EVENT_WORK)->transitionFrames = 10;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  PaletteFade_SetTarget(0x10000, 0);
  ((struct FieldWork *)EVENT_WORK)->transitionFrames = 1;
  PaletteFade_StartInterpolation(1);
  ActorSystem_SuspendProjectionAndShowSprites();
  FieldMessage_ShowCentered(MsgCarriedTheMessage, 0, 3);
  ActorSystem_ResumeProjectionAndHideLayers();
  GameFlag_Set(FLAG_GABOMBA_MESSAGE_CARRIED);
  GameFlag_Clear(0x86b);
  GameFlag_Clear(FLAG_GABOMBA_CEREMONY_UNDER_WAY);
  gMainRoutineWork.retreatScene = SCENE_KIBOMBO_1;
  gMainRoutineWork.retreatEntrance = 1;
  gMainRoutineWork.restartScene = SCENE_KIBOMBO_3;
  gMainRoutineWork.restartEntrance = 6;
  Import_EventSetExitRequest(3);
}

void GabombaStatueChamber_DropIntoHole(s32 x, s32 z) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct GabombaStatueChamberObject *object =
      ((struct GabombaStatueChamberObject *)Actor_GetById(
          gMainRoutineWork.leaderActorId));

  Event_Start();
  Event_ResolvePendingActions(0);
  object->motionFlags = 0;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, x, z);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_WaitForScriptEndById(gMainRoutineWork.leaderActorId);
  Actor_SetSpriteFlags(((struct GabombaStatueChamberObject *)Actor_GetById(
                           gMainRoutineWork.leaderActorId)),
                       0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 13);
  FieldActor_SetMoveTarget(object, x, -12, z);
  Actor_WaitForMove(object);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 10);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->stepTrigger);
}

void GabombaStatueChamber_StepIntoFirstHole(void) {
  GabombaStatueChamber_DropIntoHole(312, 144);
}

void GabombaStatueChamber_StepIntoSecondHole(void) {
  struct GabombaStatueChamberObject *leader =
      ((struct GabombaStatueChamberObject *)Actor_GetById(
          gMainRoutineWork.leaderActorId));
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
  GabombaStatueChamber_DropIntoHole(424, 100);
}

void BlendScript_Suspend(void);

void GabombaStatueChamber_ArriveByLift(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapGrid;
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
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 328, 184);
  Event_Wait(20);
  Sound_Play(SE_MATERIALIZE);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
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
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  Sound_Play(SE_STOP_MUSIC_PLAYER_5);
  wait(1);
  Sound_Play(SE_CHEST_CLOSE);
  Event_Wait(20);
  GameFlag_Set(FLAG_SCENE_TEMP_06);
  Event_End();
}

void GabombaStatueChamber_RideLiftUp(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapGrid;
  u8 *lower = map + 0x108;
  u8 *upper = map + 0x178;
  s32 i, step;

  Event_Start();
  Event_ResolvePendingActions(0);
  BlendScript_Suspend();
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_SetDestinationOffsetById(gMainRoutineWork.leaderActorId, 0, -8);
  Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                    FACING_SOUTH);
  Sound_Play(SE_MATERIALIZE);
  step = 1;
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
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
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  Sound_Play(SE_STOP_MUSIC_PLAYER_5);
  wait(1);
  Sound_Play(SE_CHEST_CLOSE);
  Event_Wait(20);
  Actor_GetById(gMainRoutineWork.leaderActorId)->layer = 1;
  GameFlag_Set(FLAG_SCENE_TEMP_06);
  GameFlag_Set(FLAG_SCENE_TEMP_07);
  Event_End();
}

void GabombaStatueChamber_StepOffLift(void) {
  GameFlag_Clear(FLAG_SCENE_TEMP_06);
}

void GabombaStatueChamber_AkafubuReachesChamber(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 120, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Event_SetMessageId((s32)MsgIHaveFollowed);
  Actor_SetAttachedEffect(10, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "I have followed Oeia's instructions and entered the final chamber!"
  Event_ShowMessageAndWait(10, 0);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  // "He told me that if I reached this chamber, I would become Kibombo's..."
  Event_ShowMessageAndWait(10, 0);
  Actor_FaceDirectionAndRefreshById(10, FACING_EAST);
  // "But what must I do here to become a witch doctor?"
  Event_ShowMessageAndWait(10, 0);
  Actor_FaceDirectionAndWait(10, FACING_NORTH_NORTHEAST);
  Actor_SetAnimationAndWait(10, 4);
  // "I do not know..."
  Event_ShowMessageAndWait(10, 0);
  GameFlag_Set(FLAG_AKAFUBU_REACHED_CHAMBER);
  Event_End();
}

void GabombaStatueChamber_KeepItemWithStatue(struct Actor *object) {
  object->x.fixed = Actor_GetById(8)->x.fixed;
}
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
    if ((gGabombaStatueChamber_ShownItem = object =                            \
             ((struct GabombaStatueChamberObject *)Import_ActorCreate(         \
                 489, x, px(18), z))) != NULL) {                               \
      struct FieldSprite *sprite = object->sprite;                             \
      u8 *block;                                                               \
                                                                               \
      sprite->fullColor = 0;                                                   \
      sprite->palette = 0;                                                     \
      sprite->priority = 2;                                                    \
      object->motionFlags = zero;                                              \
      object->field5c = 1;                                                     \
      object->update = GabombaStatueChamber_KeepItemWithStatue;                \
      sprite->nextFrame = zero;                                                \
      sprite->unk1b[0] = zero;                                                 \
      block = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 0x608);                 \
      Icon_DrawItem(n1);                                                       \
      VramCache_Load(sprite->vramBlock, 128, block + 0x400);                   \
      HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);                              \
    }                                                                          \
  }

DEFINE_GABOMBA_STATUE_CHAMBER_SHOW_ITEM(GabombaStatueChamber_ShowDragonsEye,
                                        230)

DEFINE_GABOMBA_STATUE_CHAMBER_SHOW_ITEM(GabombaStatueChamber_ShowBlackCrystal,
                                        242)

void GabombaStatueChamber_OpenThePassage(void) {
  MapGrid_CopyCellPictureIndicesRect(85, 78, 22, 79, 5, 11);
  MapGrid_CopyCellAttributes(85, 78, 5, 11, 22, 15);
  MapGrid_CopyCellsTo(86, 30, 86, 20, 5, 2);
  MapGrid_CopyCellPictureIndicesRect(86, 30, 84, 22, 9, 2);
}

void GabombaStatueChamber_SetPassageCells(void) {
  Map_CopyCellAttributeRect(23, 1, 3, 1, 23, 9);
  Map_CopyCellAttributeRect(23, 0, 3, 1, 25, 9);
}

void GabombaStatueChamber_OpenTheLap(void) {
  MapGrid_CopyCellPictureIndicesRect(0, 1, 26, 6, 1, 1);
  MapGrid_CopyCellPictureIndicesRect(0, 65, 26, 70, 1, 1);
  MapGrid_CopyCellAttributes(0, 2, 3, 3, 25, 5);
}

#define EVENT_WORK ((u8 *)Ram_HeapSlots->fieldWork)

extern u32 gGabombaStatueChamber_WalkOffScript[];

/// In the chamber the party finds the Black Crystal gone: "The black orb...
/// Where is it?"
void GabombaStatueChamber_WhereIsTheOrb(void) {
  struct Actor *statue = (struct Actor *)((s32)Actor_GetById(13));
  struct FieldSprite *sprite;
  u8 *block;
  s32 turnA, turnB;
  s32 owner;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldActor_SetPositionAndResetMotion(((s32)Actor_GetById(13)), px(392), 0,
                                       px(268));
  wait(1);
  /* FAKEMATCH(temporary): the leader read before the first turn is built. */
  owner = gMainRoutineWork.leaderActorId;
  Actor_SetPositionAndFacing(owner, px(360), px(184), turnA = FACING_NORTHEAST);
  Actor_SetPositionAndFacing(ACTOR_PIERS, px(376), px(184), turnA);
  Actor_SetPositionAndFacing(11, px(360), px(168), turnA);
  Actor_SetPositionAndFacing(ACTOR_SHEBA, px(423), px(168),
                             turnB = FACING_NORTHWEST);
  Actor_SetPositionAndFacing(ACTOR_JENNA, px(408), px(184), turnB);
  if (statue != 0) {
    sprite = statue->sprite;
    sprite->nextFrame = 0;
    sprite->unk1b[0] = 0;
    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 1;
    ((u8 *)statue)[0x5c] = 1;
    block = (u8 *)HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
    Icon_DrawItem(ITEM_BLACK_CRYSTAL);
    VramCache_Load(sprite->vramBlock, 0x80, (s32)(block + 0x400));
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
  GabombaStatueChamber_OpenThePassage();
  ((struct FieldWork *)EVENT_WORK)->transition = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Actor_Jump(ACTOR_PIERS, 4, 10);
  Event_SetMessageId(MsgTheBlackOrb);
  // "The black orb... Where is it?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  // "I saw it roll in there..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  Actor_ShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "It came in through there and rolled along this groove in the floor..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  FieldActor_SetPositionAndResetMotion(((s32)Actor_GetById(13)), px(392), 0,
                                       px(340));
  Sound_Play(SOUND_FADE_OUT_BGM);
  PaletteFade_SetTarget(0x10002, 0);
  PaletteFade_StartInterpolation(40);
  wait(40);
  Actor_SetAnimationById(9, 2);
  Actor_SetAnimationById(10, 2);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, 13);
  Actor_WatchAndSetScript(ACTOR_JENNA, 13);
  Actor_WatchAndSetScript(ACTOR_SHEBA, 13);
  Actor_WatchAndSetScript(ACTOR_PIERS, 13);
  Actor_WatchAndSetScript(11, 13);
  Event_Wait(10);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 246, 1);
  Actor_SetSpeed(13, 0.8);
  Actor_MoveToAndWait(13, 392, 246);
  Actor_SetAnimationById(9, 1);
  Actor_SetAnimationById(10, 1);
  statue->velocityY = px(3);
  statue->gravity = px(1) / 2;
  Actor_SetSpeed(13, 1.6);
  Actor_MoveToAndWait(13, 392, 234);
  Event_Wait(10);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 168, 1);
  Actor_SetSpeed(13, 0.8);
  Actor_MoveToAndWaitById(13, 392, 160);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_SHEBA);
  Actor_Stop(ACTOR_PIERS);
  Actor_Stop(11);
  wait(1);
  Actor_SetPosition(13, 0, 0);
  Event_Wait(20);
  PaletteFade_SetTarget(0x10000, 0);
  PaletteFade_StartInterpolation(40);
  wait(40);
  Scene_PlayMusic();
  Actor_ShowEmoteById(11, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "It vanished into that hole!"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 11, 0);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  // "It looks pretty deep..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_JENNA, turnA);
  // "Can you see it? Do you think you can reach it?"
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_WalkToAndWait(ACTOR_SHEBA, 400, 172);
  Actor_SetAnimationById(ACTOR_SHEBA, 9);
  Event_Wait(20);
  // "Hm... No, I can't even see it."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "It's no use... Now, I'll never see Lemuria again... I am lost..."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  // "Wait, hold on... I can see light on the other side of this hole..."
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  Actor_ShowEmoteById(11, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  // "A light?"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 11, 0);
  Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHEAST);
  // "There must be a room on the other side of this statue..."
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 11, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Sound_Play(MUS_SILENCE);
  {
    s32 turn = 0xd000;
    Actor_SetHorizontalPositionAndAngleWithTerrain(12, px(400), px(320), turn);
  }
  wait(1);
  // "Who... Who are you!?"
  Event_ShowMessageAndWait(12, 0);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_STAND);
  wait(1);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(11, FACING_SOUTH_SOUTHEAST, 0);
  FieldCamera_SetSpeed(1.6);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 200, 1);
  Actor_SetSpeed(12, 1.2);
  Actor_WalkToAndWait(12, 404, 220);
  Actor_Jump(12, 4, 10);
  // "What are you doing here? Are you Kibombo!?" Yes: "What? Do you take me for
  // a naive fool!?" No: "Then what are you doing inside the Great..."
  FieldMessage_ShowAtActor(12, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    FieldEffect_SetAttached(12,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(20);
    // "What? Do you take me for a naive fool!?"
    Event_ShowMessageAndWait(12, 0);
    ((struct FieldWork *)EVENT_WORK)->message += 1;
  } else {
    ((struct FieldWork *)EVENT_WORK)->message += 1;
    Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
    // "Then what are you doing inside the Great Gabomba!?"
    Event_ShowMessageAndWait(12, 0);
  }
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
  // "We've come to reclaim the black orb you stole!"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_ShowEmoteById(12, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "Do you really believe I would permit you to take it!?"
  Event_ShowMessageAndWait(12, 0);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
  // "Hey, watch it! You wouldn't even be in here if it weren't for us!"
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  FieldEffect_SetAttached(12, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  // "What is that supposed to mean?"
  Event_ShowMessageAndWait(12, 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationAndWait(11, 3);
  Actor_SetAnimationById(12, 4);
  // "Why should I believe you? I am Akafubu, the chosen witch doctor of..."
  Event_ShowMessageAndWait(12, 0);
  Actor_ShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_WalkToAndWait(ACTOR_JENNA, 420, 228);
  Actor_FaceDirectionAndRefreshById(ACTOR_JENNA, FACING_NORTHWEST);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_JENNA, 1.6);
  Actor_WalkToAndWait(ACTOR_JENNA, 412, 222);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(12, 1);
  Actor_SetMovementTypeById(ACTOR_JENNA, gGabombaStatueChamber_WalkOffScript);
  Event_Wait(10);
  Actor_SetMovementTypeById(12, gGabombaStatueChamber_WalkOffScript);
  Event_Wait(60);
  ((struct FieldWork *)EVENT_WORK)->transition = 514;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  Import_EventSetExitRequest(2);
}

extern u32 gGabombaStatueChamber_AkafubuPath[];

void GabombaStatueChamber_AkafubuOffersToHelp(void) {
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
  GabombaStatueChamber_OpenThePassage();
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  owner = (s32 *)&gMainRoutineWork;
  owner += 133;
  {
    s32 angleA = FACING_NORTHEAST;

    Actor_SetPositionAndFacing(*owner, px(360), px(216), angleA);
    Actor_SetPositionAndFacing(ACTOR_PIERS, px(376), px(216), angleA);
  }
  facing = FACING_NORTHWEST;
  zero = 0;
  Actor_SetPositionAndFacing(11, px(360), px(200), zero);
  Actor_SetPositionAndFacing(ACTOR_JENNA, px(408), px(216), facing);
  angleC = FACING_WEST;
  Actor_SetPositionAndFacing(ACTOR_SHEBA, px(424), px(200), angleC);
  {
    s32 angleB = FACING_NORTH_NORTHWEST;

    Actor_SetPositionAndFacing(12, px(392), px(200), angleB);
  }
  heap = Ram_HeapSlots;
  System_Wait(1);
  {
    s32 *effect = &((struct FieldWork *)heap->fieldWork)->transition;

    turn = 0x100;
    *effect = turn;
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  Actor_FaceDirection(12, FACING_NORTH_NORTHEAST, 20);
  Actor_FaceDirectionAndWait(12, FACING_SOUTH_SOUTHWEST);
  Actor_ShowEmoteById(12, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(11, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, facing, 0);
  Actor_SetSpeed(12, 0.8);
  Actor_WalkToAndWait(12, 392, 168);
  Event_PlaySound(SE_HIDDEN_PASSAGE_OPEN);
  GabombaStatueChamber_ShowDragonsEye();
  FieldEvent_Wait(40);
  Event_PlaySound(SE_PUSH);
  {
    struct Actor *guard = Actor_GetById(8);

    guard->motionFlags = zero;
  }
  GabombaStatueChamber_SetPassageCells();
  Actor_SetSpeed(8, 0.4);
  Actor_MoveToAndWait(8, 424, 152);
  Actor_FaceDirectionAndWait(12, FACING_SOUTH_SOUTHWEST);
  FieldEvent_SetMessageId((s32)MsgIMightBe);
  // "I might be able to retrieve the orb."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 12, 0);
  Actor_SetAnimationAndWait(12, 3);
  Actor_WalkToAndWait(12, 392, 140);
  System_Wait(1);
  Actor_SetPosition(12, 0, 0);
  System_Wait(1);
  Actor_ShowEmoteById(ACTOR_JENNA, turn, 20);
  Actor_FaceDirection(ACTOR_JENNA, angleC, 0);
  // "Do you think Akafubu will really return the orb to us?" Yes: "Maybe we had
  // him all wrong..." No: "I think Jenna is right. It sounds like he's..."
  FieldMessage_ShowAtActor(SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0);
  if (Event_AskYesNoWithActorAnimation(*owner, 0) == 0) {
    Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
    Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
    // "Maybe we had him all wrong..."
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
    ((struct FieldWork *)heap->fieldWork)->message++;
  } else {
    ((struct FieldWork *)heap->fieldWork)->message++;
    Actor_FaceDirectionAndWait(ACTOR_PIERS, angleC);
    // "I think Jenna is right. It sounds like he's going to return what he..."
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
    GabombaStatueChamber_CountAnswer();
  }
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "I thought Akafubu was an evil man, but I guess we were wrong..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(11, FACING_EAST);
  Actor_SetAnimation(11, 4);
  // "You never can tell, Sheba..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 11, 0);
  Actor_FaceDirectionAndWait(11, FACING_NORTH_NORTHEAST);
  // "We have little choice but to trust that he will return it. Come on!"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 11, 0);
  Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(11, 3);
  // "Let's go, Felix!"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 11, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 20);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetSpeed(11, 0.8);
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  {
    s32 path = (s32)gGabombaStatueChamber_AkafubuPath;

    Actor_SetScript(11, path);
    Actor_SetScript(ACTOR_PIERS, path);
    Actor_SetScript(ACTOR_JENNA, path);
    Actor_RunScriptAndWait(ACTOR_SHEBA, path);
  }
  Event_SetFlag(FLAG_AKAFUBU_OFFERED_HELP);
  Event_End();
}

s32 Scene_Initialize(void) {
  struct FieldWork *work;
  s16 scene;

  work = Ram_HeapSlots->fieldWork;
  work->transition = 521;
  ((struct GabombaStatueChamberObject *)Actor_GetById(Party_GetLeaderActor()))
      ->priorityFlags |= 0x20;
  scene = gMainRoutineWork.next;
  if (scene == SCENE_GABOMBA_STATUE_CHAMBER_1)
    GabombaStatueChamber_InitializeRoom1();
  else if (scene == SCENE_GABOMBA_STATUE_CHAMBER_2)
    GabombaStatueChamber_InitializeRoom2();
  else if (scene == SCENE_GABOMBA_STATUE_CHAMBER_3)
    GabombaStatueChamber_InitializeRoom3();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void GabombaStatueChamber_InitializeRoom1(void) {
  Actor_DisableById(8);
  Actor_DisableById(9);
  Actor_DisableById(10);
  wait(1);
  if (GameFlag_Get(FLAG_AKAFUBU_OFFERED_HELP)) {
    GabombaStatueChamber_OpenThePassage();
    GabombaStatueChamber_SetPassageCells();
    Actor_SetPosition(8, 424, 152);
    wait(1);
    GabombaStatueChamber_ShowDragonsEye();
  } else {
    s32 entrance;

    entrance = gMainRoutineWork.entrance;
    if (entrance == 5) {
      GabombaStatueChamber_WhereIsTheOrb();
      return;
    }
    if (entrance == 6) {
      GabombaStatueChamber_AkafubuOffersToHelp();
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
          (struct GabombaStatueChamberMapWork *)Ram_HeapSlots->mapGrid;
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
      front->y += px(1);
      back->y += px(1);
      i++;
      if (i < 352)
        goto loop;
      MapGrid_CenterOnFollowedPoint();
      wait(1);
    }
  } else if (gMainRoutineWork.entrance == 2) {
    GabombaStatueChamber_ArriveByLift();
  }
}

/// Sets up the chamber: its lap open once the message of the Gabomba is
/// carried, or else the place of the Black Crystal; arriving by entrance 2,
/// Retreat leads back to the statue's top or to the chamber's first room, by
/// the way the party came.
void GabombaStatueChamber_InitializeRoom3(void) {
  u32 i;
  char *scene;

  for (i = 0; i <= 17; i++)
    Actor_SetSpritePriority(i + 15, 2);
  Actor_DisableById(8);
  Actor_GetById(8)->y.fixed = 0;
  if (GameFlag_Get(FLAG_GABOMBA_MESSAGE_CARRIED)) {
    GabombaStatueChamber_OpenTheLap();
    Actor_SetPosition(10, 0, 0);
  } else {
    GabombaStatueChamber_ShowBlackCrystal();
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
#include "text/msg_ids.h"

void GabombaStatueChamber_StepOffLift(void);
void GabombaStatueChamber_AkafubuReachesChamber(void);

// clang-format off
u32 gGabombaStatueChamber_WalkOffScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(428, 0, 234),
    SC_SYNC,
    SC_MOVE(488, 0, 234),
    SC_SYNC,
    SC_END,
};

u32 gGabombaStatueChamber_AkafubuPath[] = {
    SC_ANIMATION(2),
    SC_MOVE(360, 0, 216),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION_ORIGIN,
    SC_END,
};

u32 gGabombaStatueChamber_SpiritRisesScript[] = {
    SC_ADD(SW_FACING, 0x800),
    SC_WAIT(1),
    SC_LOOP(32, SC_TOP),
    SC_END,
};

s32 gGabombaStatueChamber_AkafubuStepsUpScript[] = {
    SC_POSITION(392, 64, 124),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_Y, px(-1)),
    SC_WAIT(1),
    SC_LOOP(64, SC_TO(1)),
    SC_WAIT(1),
    SC_END,
};

u32 gGabombaStatueChamber_AkafubuLeavesScript[] = {
    SC_ADD(SW_Y, px(2)),
    SC_WAIT(1),
    SC_LOOP(80, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_END,
};

u32 gGabombaStatueChamber_PartyLeavesScript[] = {                                                               
      SC_ANIMATION(2), SC_MOVE(392, 0, 148),                           
      SC_SYNC,   SC_MOVE(392, 0, 236),                           
      SC_SYNC,   SC_ANIMATION(1),                                                    
      SC_END,                                                                  
  };

s32 gGabombaStatueChamber_MirroredScript[] = {
    SC_SET(SW_SCALE_X, -0x10000),
    SC_END,
};

struct Actor *gGabombaStatueChamber_ShownItem = NULL;

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 392, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gGabombaStatueChamber_Room1Regions[] = {
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

struct ActorPlacement gGabombaStatueChamber_Room1Placements[] = {
    ACTOR_PLACEMENT(375, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(361, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 256, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(361, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 256, FACING_EAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(7, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(6, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(5, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(26, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaStatueChamber_Room3Placements[] = {
    ACTOR_PLACEMENT(375, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(26, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 136, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 392, 0, 84, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(487, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(27, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 336, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 416, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 432, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 448, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 336, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 416, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 432, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 448, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 336, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 416, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 432, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 448, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaStatueChamber_Placements[] = {
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gGabombaStatueChamber_Room1Triggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS,
     EVENT(GabombaStatueChamber_StepIntoFirstHole)},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaStatueChamber_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 3, FLAG_SCENE_TEMP_06,
     EVENT(GabombaStatueChamber_RideLiftUp)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS,
     EVENT(GabombaStatueChamber_StepOffLift)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaStatueChamber_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING, 2, CONDITION_ALWAYS,
     EVENT(GabombaStatueChamber_StepIntoSecondHole)},
    {TRIGGER_STEP, 10, FLAG_AKAFUBU_REACHED_CHAMBER,
     EVENT(GabombaStatueChamber_AkafubuReachesChamber)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgYourBlackOrb)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgOeiaWhatAm)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 11, FLAG_GABOMBA_MESSAGE_CARRIED,
     EVENT(GabombaStatueChamber_GabombaNamesWitchDoctor)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaStatueChamber_Triggers[] = {
    {TRIGGER_TABLE_END},
};
