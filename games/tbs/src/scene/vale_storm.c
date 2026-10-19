#include "script.h"
#include "game_state.h"
#include "unit/import.h"
#include "unit/scene/vale_storm.h"
#include "unit/scene/common/effect_psynergy.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "callback_scheduler.h"
#include "field_event.h"
#include "field_scene.h"
#include "call.h"
#include "field_service.h"
#include "field_obj.h"
#include "animspr.h"
#include "text/msg_ids.h"
#include "resource.h"
#include "field_effect.h"
#include "tbs_edition.h"
#include "b5_context.h"
#include "objcmd.h"
#include "global_cells.h"
#include "vram_tab.h"
#include "fixed_math.h"
#include "flags.h"
#include "songs.h"

// Forward declarations.
static void Event_DoraSendsIsaacToThePlaza(void);
static void ValeStorm_SetStormCellAttributes(void);
static void ValeStorm_ApplyRock12Collision(void);
static void ValeStorm_ApplyRock13Collision(void);
static void ValeStorm_ApplyRock14Collision(void);
static void ValeStorm_MeetVillagerAtExit6(void);
static void ValeStorm_RunCallOutSequence(void);
static void ValeStorm_GaretCommentsOnBlockedPath(s32 x, s32 y);
static void ValeStorm_RunRiverSearch(void);
static void ValeStorm_ShowBoulderSweepingDownriver(void);
static void ValeStorm_FlashLightning(void);
static void ValeStorm_UpdatePulsingGlow(void);
static void ValeStorm_FlickerGlowOffset(void);
static void ValeStorm_GaretLeavesIsaac(void);
static void ValeStorm_RebuildRiverbank(void);
static void ValeStorm_FlashPaletteAndSpawnEffects(struct FieldActor *actor);
static void ValeStorm_SpawnEffectPair(struct FieldActor *parent);
static void ValeStorm_UpdateBoulderSceneEffects(void);
static void ValeStorm_CycleBoulderPalette(void);
static void ValeStorm_CopyLeaderSpritePriority(void);

enum { STORM_CALLBACK_PRIORITY = 3200 };

enum StormNightActor { ACTOR_DORA = ACTOR_FIRST_PLACED + 1, ACTOR_KYLE };

#define ACTOR_GARET 22

s32 Math_RemainderUnsigned(s32, s32);

static s32 sShakeDone;
static s32 sShakeShift;

// Choose a new random heading whenever the actor countdown expires.
static s32
ValeStorm_RerollHeadingOnCountdown(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  s32 timer = actor->timer;
  s32 countdown = (s16)timer;

  if (timer == 0) {
    actor->facing = Engine_RandomNext();
    countdown = Math_RemainderUnsigned(Engine_RandomNext(), 20) + 20;
    actor->timer = countdown;
  }
  actor->timer = countdown - 1;
  return 1;
}

// Shared source is included here in ROM order.
#include "common/object_pulse.h"

// Return the entrances to the paths outside on the stormy night.
struct SceneEntrance *ValeStorm_GetEntrances(void) {
  return gValeStorm_Entrances;
}

// The paths have no scene regions.
struct SceneRegion *ValeStorm_GetRegions(void) { return NULL; }

// Return the storm scene exit table.
u32 *ValeStorm_GetExits(void) { return gValeStorm_Exits; }

// Return the storm scene actor placements.
struct ScenePlacement *ValeStorm_GetPlacements(void) {
  return gValeStorm_Placements;
}

// Return the storm scene event bindings.
struct SceneEvent *ValeStorm_GetEvents(void) { return gValeStorm_Events; }

// Walking onto the upper path: its collision cells change, remembered in
// FLAG_SCENE_TEMP_10.
static void Event_SetPathCellOverride(void) {
  Engine_GameFlagSet(FLAG_SCENE_TEMP_10);
  Map_CopyCellAttributes(40, 84, 7, 4, 10, 84);
}

// Walking off the upper path restores its collision cells.
static void Event_ClearPathCellOverride(void) {
  Engine_GameFlagClear(FLAG_SCENE_TEMP_10);
  Map_CopyCellAttributes(40, 89, 7, 4, 10, 84);
}

// Shared source is included here in ROM order.
#include "common/vale.h"

// Animate the doorway before taking backdrop exit 11.
UNUSED static void ValeStorm_OpenBackdropExit11(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gValeStorm_CellSteps0, 45, 11);
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 257, 420);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(11);
}

// Play the exit cue and take backdrop exit 1.
static void Event_LeaveByExit1(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Vale_RequestBackdropExit(1);
}

// Play the exit cue and take backdrop exit 3.
static void Event_LeaveByExit3(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Vale_RequestBackdropExit(3);
}

// Play the exit cue and take backdrop exit 4.
static void Event_LeaveByExit4(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Vale_RequestBackdropExit(4);
}

// Let Garet leave before taking backdrop exit 2.
static void Event_LeaveByExit2(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_RIVER_GATHERING_SEEN) != 0 &&
      Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_LEFT) == 0) {
    ValeStorm_GaretLeavesIsaac();
  }
  Vale_RequestBackdropExit(2);
}

// Animate the doorway and take backdrop exit 5.
static void Event_LeaveByExit5(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gValeStorm_CellSteps1, 54, 32);
  Engine_ActorWalkTo(0, 406, 727);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(5);
}

// Isaac's first time at the door of exit 6: a villager runs out and shoves
// him ("Huh? Isaac?"); afterwards the door opens as usual.
static void Event_LeaveByExit6(void) {
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_06) == 0) {
    Event_PlayCue(SE_DOOR_OPEN);
    Map_AnimateCells(gValeStorm_CellSteps2, 45, 39);
  }
  if (GameFlag_IsSet(FLAG_VALE_STORM_BUMPED_INTO_VILLAGER) == 0 &&
      GameFlag_IsSet(FLAG_VALE_STORM_SECOND_ROCK_FELL) == 0) {
    ValeStorm_MeetVillagerAtExit6();
    GameFlag_Set(FLAG_SCENE_TEMP_06);
  } else {
    Actor_WalkTo(ACTOR_PARTY_LEADER, 262, 805);
    Engine_EventWait(3);
    Vale_RequestBackdropExit(6);
  }
}

// Animate the doorway and take backdrop exit 7.
static void Event_LeaveByExit7(void) {
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_05) == 0) {
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapAnimateCells(gValeStorm_CellSteps1, 50, 44);
  }
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 340, 888);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(7);
}

// Animate the doorway and take backdrop exit 8.
static void Event_LeaveByExit8(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gValeStorm_CellSteps2, 49, 69);
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 326, 1126);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(8);
}

// Animate the doorway and take backdrop exit 9.
static void Event_LeaveByExit9(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gValeStorm_CellSteps3, 52, 76);
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 374, 1238);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(9);
}

// Animate the doorway and take backdrop exit 10.
static void Event_LeaveByExit10(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_CellSteps1, 35, 74);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 1206);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(10);
}

// Animate the doorway and take backdrop exit 12.
static void Event_LeaveByExit12(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_CellSteps1, 35, 73);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 1206);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(12);
}

// Animate the doorway and take backdrop exit 13.
static void Event_LeaveByExit13(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_CellSteps2, 38, 72);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 146, 1182);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(13);
}

// Arriving on the stormy night: restore the fallen rocks, the river scene and
// the Boulder from their flags, start the storm, and play the scene with Dora
// and Kyle on the first arrival by entrance 15.
s32 ValeStorm_Initialize(void) {
  BattleFx_SetQueuedSoundAndPlay(SE_170);
  Engine_ActorSetPosition(23, 0, 0);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    Engine_GameFlagClear(FLAG_SCENE_TEMP_05);
    Engine_GameFlagClear(FLAG_SCENE_TEMP_06);
  }
  // Restore the rocks that have already fallen.
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_FIRST_ROCK_FELL) != 0) {
    Actor_SetPosition(11, PIXELS(334), PIXELS(932));
    ValeStorm_SetStormCellAttributes();
  }
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_SECOND_ROCK_FELL) != 0) {
    Actor_SetPosition(12, PIXELS(224), PIXELS(872));
    ValeStorm_ApplyRock12Collision();
  }
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_THIRD_ROCK_FELL) != 0) {
    Actor_SetPosition(13, PIXELS(64), PIXELS(703));
    ValeStorm_ApplyRock13Collision();
  }
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_FOURTH_ROCK_FELL) != 0) {
    Actor_SetPosition(14, PIXELS(432), PIXELS(1147));
    ValeStorm_ApplyRock14Collision();
  }
  Object_GetById(11)->collisionFlags |= 4;
  Object_GetById(12)->collisionFlags |= 4;
  Object_GetById(13)->collisionFlags |= 4;
  Object_GetById(14)->collisionFlags |= 4;
  Object_GetById(15)->collisionFlags |= 4;
  Object_GetById(16)->collisionFlags |= 4;
  Object_GetById(17)->collisionFlags |= 4;
  Object_GetById(18)->collisionFlags |= 4;
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_JOINED) != 0) {
    Engine_ActorSetPosition(22, 0, 0);
  }
  {
    struct FieldActor *actor;

    actor = Object_GetById(19);
    actor->scaleX = 2 * Q16_ONE;
    actor->scaleY = 2 * Q16_ONE;
  }
  if (Engine_GameFlagIsSet(FLAG_VALE_BOULDER_FELL) != 0) {
    Actor_SetPosition(19, PIXELS(114), PIXELS(333));
  } else {
    Actor_SetActionScript(19, gValeStorm_ActorNineteenScript);
  }
  // Restore the riverbank cast at the saved stage of the storm.
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_RIVER_GATHERING_SEEN) != 0) {
    s32 facing;
    const s32 *table;
    ValeStorm_RebuildRiverbank();
    Actor_SetPosition(9, PIXELS(165), PIXELS(1229));
    {
      struct FieldActor *actor;

      actor = Object_GetById(9);
      facing = FACING_NORTHEAST;
      actor->facing = facing;
      actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
      table = gValeStorm_ActorEightScript;
      actor->work = 1;
      Actor_SetActionScript(9, table);
    }
    Engine_ActorSetPosition(26, PIXELS(165), PIXELS(1254));
    {
      struct FieldActor *actor;

      actor = Object_GetById(26);
      actor->facing = facing;
      actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
      actor->work = 2;
      Actor_SetActionScript(26, table);
    }
    Engine_ActorSetPosition(22, PIXELS(152), PIXELS(1285));
    {
      struct FieldActor *actor;

      actor = Object_GetById(22);
      actor->facing = facing;
      actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
      actor->work = 3;
      Actor_SetActionScript(22, table);
    }
    Engine_ActorSetPosition(8, PIXELS(184), PIXELS(1304));
    {
      struct FieldActor *actor;

      actor = Object_GetById(8);
      actor->facing = facing;
      actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
      actor->work = 4;
      Actor_SetActionScript(8, table);
    }
    Engine_ActorSetAnimation(8, 6);
    Object_GetById(22)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    Object_GetById(8)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    FieldScheduler_AddCallback(ValeStorm_CopyLeaderSpritePriority,
                               STORM_CALLBACK_PRIORITY);
    Engine_ActorSetPosition(24, 0, 0);
    Engine_ActorSetPosition(25, 0, 0);
    Engine_ActorSetPosition(23, 0, 0);
    Engine_ActorSetPosition(19, 0, 0);
    if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_LEFT) != 0) {
      Engine_ActorSetPosition(22, 0, 0);
    }
  } else if (Engine_GameFlagIsSet(FLAG_VALE_STORM_SAW_RIVER_RESCUE) != 0) {
    const s32 *table;
    Actor_SetPosition(10, PIXELS(192), PIXELS(1214));
    Actor_FaceDirection(10, FACING_SOUTHEAST, 0);
    Engine_ActorSetAnimation(10, 5);
    {
      struct FieldActor *actor;

      actor = Object_GetById(10);
      actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    }
    table = gValeStorm_ActorEightScript;
    Actor_SetActionScript(10, table);
    Actor_SetPosition(24, PIXELS(227), PIXELS(1214));
    Actor_FaceDirection(24, FACING_SOUTH, 0);
    Engine_ActorSetAnimation(24, 6);
    {
      struct FieldActor *actor;

      actor = Object_GetById(24);
      actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    }
    Actor_SetActionScript(24, table);
    Actor_SetPosition(25, PIXELS(247), PIXELS(1214));
    Actor_FaceDirection(25, FACING_SOUTH, 0);
    Engine_ActorSetAnimation(25, 6);
    {
      struct FieldActor *actor;

      actor = Object_GetById(25);
      actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    }
    Actor_SetActionScript(25, table);
    Actor_SetPosition(23, PIXELS(243), PIXELS(1277));
    Engine_ActorFaceDirection(23, FACING_NORTH, 0);
    Engine_ActorSetSpriteFlags(Object_GetById(23), 0);
    Engine_ActorSetPosition(17, 0, 0);
    Engine_ActorSetPosition(18, 0, 0);
  } else {
    Engine_ActorSetPosition(17, 0, 0);
    Engine_ActorSetPosition(18, 0, 0);
  }
  if (gGameState.entrance != 15 ||
      Engine_GameFlagIsSet(FLAG_VALE_STORM_SENT_TO_PLAZA) != 0) {
    Effect_SoundAndFlash();
    FieldFx_StartLightning();
  }
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_10) != 0) {
    Event_SetPathCellOverride();
  }
  Engine_GameFlagSet(FLAG_VALE_STORM);
  Map_CopyCellAttributes(29, 24, 1, 2, 26, 46);
  Map_CopyCellAttributes(29, 25, 1, 1, 27, 46);
  Map_CopyCellAttributes(29, 25, 1, 1, 28, 46);
  Map_CopyCellAttributes(19, 90, 1, 1, 20, 88);
  Map_CopyCellAttributes(19, 90, 1, 1, 20, 89);
  {
    struct FieldActor *actor;

    actor = Object_GetById(21);
    actor->motionFlags = 0;
    actor->y.fixed = PIXELS(192);
    actor->collisionFlags = 8;
    Engine_ActorSetSpriteFlags(actor, 0);
  }
  Engine_TaskWait(1);
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_SENT_TO_PLAZA) == 0 &&
      gGameState.entrance == 15) {
    Event_DoraSendsIsaacToThePlaza();
    return 0;
  }
  Engine_ActorSetAnimation(23, 7);
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_JOINED) == 0) {
    Engine_EventBegin();
    Actor_SetAttachedEffect(22, 0x101);
    Actor_SetPosition(22, PIXELS(400), PIXELS(611));
    Actor_SetPosition(21, PIXELS(424), PIXELS(627));
    Actor_WalkTo(22, 400, 619);
    Actor_WalkToAndWait(21, 424, 619);
    Engine_ActorSetAnimation(21, ANIM_WALK);
    Engine_ActorSetAnimation(22, 5);
    Engine_EventEnd();
  } else {
    Engine_EventBegin();
    Actor_SetPosition(21, PIXELS(424), PIXELS(627));
    Actor_WalkToAndWait(21, 424, 619);
    Engine_ActorSetAnimation(21, ANIM_NOD);
    Engine_EventEnd();
  }
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 24;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  FieldFx_ResumeLightning();
  return 0;
}

// Dora and Kyle come out into the storm with Isaac ("Kyle... Will they be able
// to stop the Boulder?") and send him to the plaza while they go to help.
static void Event_DoraSendsIsaacToThePlaza(void) {
  s32 message;

  Engine_EventBegin();
  Effect_SoundAndFlash();
  FieldFx_StartLightning();
  FieldFx_LastLightning();
  Engine_TaskWait(60);
  Camera_SetSpeed(PIXELS(1) / 4, PIXELS(1) / 32);
  Camera_MoveTo(PIXELS(316), PIXELS(160), PIXELS(880), 1);
  Actor_SetPosition(ACTOR_KYLE, PIXELS(294), PIXELS(868));
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 16;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  FieldFx_ResumeLightning();
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_CellSteps1, 50, 44);
  Actor_SetAttachedEffect(22, 0x101);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_KYLE, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_DORA, PIXELS(342), PIXELS(890));
  Actor_WalkToAndWait(ACTOR_DORA, 342, 905);
  BattleFx_PlayQueuedSound();
  Actor_WalkTo(ACTOR_DORA, 296, 905);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(342), PIXELS(890));
  Actor_WalkTo(ACTOR_PARTY_LEADER, 342, 890);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 342, 905);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 318, 905);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_STAND);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 60);
  message = MsgValeKyleAbleStop;
  Engine_EventSetMessage(message);
  Event_ShowMessage(ACTOR_DORA, 0);
  Actor_WalkToAndWait(ACTOR_KYLE, 294, 838);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_KYLE, ANIM_SHAKE_HEAD);
  Event_ShowMessage(ACTOR_KYLE, 0);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_KYLE, FACING_SOUTH, 0);
  Event_ShowMessageAndWait(ACTOR_KYLE, 0, 20);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 1, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 10);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_KYLE, ANIM_SHAKE_HEAD);
  Event_ShowMessage(ACTOR_KYLE, 0);
  Actor_SetAttachedEffect(ACTOR_DORA, 0x102);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 50);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 10);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_WalkToAndWait(ACTOR_DORA, 289, 883);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTHEAST, 0);
  Event_ShowMessage(ACTOR_DORA, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_KYLE, 2);
  Event_ShowMessage(ACTOR_KYLE, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST, 10);
  Engine_EventSetMessage((message + 8));
  Event_OpenMessage(ACTOR_DORA, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 302, 905);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  while (Engine_EventChooseYesNo(0, 0) == 1) {
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
    Engine_EventSetMessage(MsgValeBigBoyWhy);
    Event_OpenMessage(ACTOR_DORA, 0);
  }
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventSetMessage(MsgValeKnowWayGo);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetSpeed(ACTOR_KYLE, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_WalkTo(ACTOR_KYLE, 297, 750);
  Engine_EventWait(10);
  Actor_WalkToAndWait(ACTOR_DORA, 297, 750);
  Actor_SetPosition(ACTOR_DORA, 0, 0);
  Actor_SetPosition(ACTOR_KYLE, 0, 0);
  Engine_ActorSetAnimation(ACTOR_KYLE, ANIM_STAND);
  Engine_ActorSetAnimation(21, ANIM_WALK);
  Engine_ActorSetAnimation(22, 5);
  GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
  GameFlag_Set(FLAG_VALE_STORM_SENT_TO_PLAZA);
  GameFlag_Set(FLAG_SCENE_TEMP_05);
  Engine_EventEnd();
}

// Walking past: rock 11 falls across the path, and Garet, once with Isaac,
// says they cannot get around it.
static void Event_Rock11Falls(void) {
  struct FieldActor *rock;
  struct FieldActor *leader;
  s32 targetHeight;
  s32 walkZ;
  struct AnimationObject *rockSprite;

  if (GameFlag_IsSet(0x310) != 0) {
  } else {
    Engine_EventBegin();
    if (GameFlag_IsSet(FLAG_VALE_STORM_FIRST_ROCK_FELL) == 0) {
      rock = Object_GetById(11);
      rockSprite = rock->sprite;
      Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      Engine_AudioPlayCue(SE_141);
      Engine_TaskWait(40);
      Engine_AudioPlayCue(SE_145);
      rock->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      rockSprite->image.priority = 1;
      Actor_SetPosition(11, PIXELS(473), PIXELS(932));
      rock->speed = PIXELS(3) / 2;
      rock->acceleration = PIXELS(3) / 2;
      targetHeight = (rock->y.fixed + PIXELS(240));
      rock->y.fixed += PIXELS(240);
      rock->targetY = targetHeight;
      rock->bounce = 0x6666;
      Actor_WalkToAndWait(11, 344, 932);
      rockSprite->image.priority = 3;
      rock->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      Engine_EventWait(40);
      Engine_AudioPlayCue(SE_289);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      Engine_MapWaitWorkValuesBelow256();
      BattleFx_PlayQueuedSound();
      Engine_GameFlagSet(FLAG_VALE_STORM_FIRST_ROCK_FELL);
    }
    ValeStorm_SetStormCellAttributes();
    Engine_GameFlagSet(0x310);
    if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_JOINED) != 0) {
      if (Engine_GameFlagIsSet(FLAG_VALE_STORM_RIVER_GATHERING_SEEN) == 0) {
        if (Engine_GameFlagIsSet(0x30c) == 0) {
          leader = Object_GetById(0);
          if (leader->y.fixed > PIXELS(128)) {
            walkZ = 0x396;
            ValeStorm_GaretCommentsOnBlockedPath(0x146, walkZ);
            Actor_WalkToAndWait(0, 291, walkZ);
          } else {
            ValeStorm_GaretCommentsOnBlockedPath(0x14f, 0x3bd);
          }
          Engine_GameFlagSet(0x30c);
        }
      }
    }
    Engine_EventEnd();
  }
}

// Apply the collision cells left by rock 11.
static void ValeStorm_SetStormCellAttributes(void) {
  Map_CopyCellAttributes(29, 64, 1, 1, 21, 57);
  Map_CopyCellAttributes(29, 64, 1, 1, 21, 58);
  Map_CopyCellAttributes(29, 64, 1, 1, 22, 58);
  Map_CopyCellAttributes(29, 64, 1, 1, 20, 58);
  Map_CopyCellAttributes(28, 20, 1, 1, 20, 57);
}

// Walking past: rock 12 falls down the path.
static void Event_Rock12Falls(void) {
  struct FieldActor *rock;
  struct FieldActor *leader;
  s32 targetHeight;

  if (GameFlag_IsSet(0x311) != 0) {
  } else {
    Engine_EventBegin();
    if (GameFlag_IsSet(FLAG_VALE_STORM_SECOND_ROCK_FELL) == 0) {
      rock = Object_GetById(12);
      Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      Engine_AudioPlayCue(SE_141);
      Engine_TaskWait(40);
      Engine_AudioPlayCue(SE_145);
      Actor_SetPosition(12, PIXELS(381), PIXELS(808));
      rock->speed = PIXELS(3) / 2;
      rock->acceleration = PIXELS(3) / 2;
      targetHeight = (rock->y.fixed + PIXELS(256));
      rock->y.fixed += PIXELS(256);
      rock->targetY = targetHeight;
      rock->bounce = 0x8000;
      Actor_WalkToAndWait(12, 290, 833);
      Engine_ActorSetSpritePriority(12, 1);
      Actor_WalkToAndWait(12, 258, 852);
      Engine_ActorSetSpritePriority(12, 2);
      Actor_WalkToAndWait(12, 224, 872);
      Engine_EventWait(40);
      Engine_AudioPlayCue(SE_289);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      Engine_MapWaitWorkValuesBelow256();
      BattleFx_PlayQueuedSound();
      Engine_GameFlagSet(FLAG_VALE_STORM_SECOND_ROCK_FELL);
    }
    ValeStorm_ApplyRock12Collision();
    Engine_GameFlagSet(0x311);
    if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_JOINED) != 0) {
      if (Engine_GameFlagIsSet(FLAG_VALE_STORM_RIVER_GATHERING_SEEN) == 0) {
        if (Engine_GameFlagIsSet(0x30c) == 0) {
          leader = Object_GetById(0);
          if (leader->y.fixed > PIXELS(128)) {
            ValeStorm_GaretCommentsOnBlockedPath(219, 0x34b);
            Actor_WalkToAndWait(0, 179, 829);
          } else {
            ValeStorm_GaretCommentsOnBlockedPath(214, 0x38c);
            Actor_WalkToAndWait(0, 219, 911);
          }
          Engine_GameFlagSet(0x30c);
        }
      }
    }
    Engine_EventEnd();
  }
}

// Apply the collision cells left by rock 12.
static void ValeStorm_ApplyRock12Collision(void) {
  Map_CopyCellAttributes(29, 23, 1, 1, 15, 53);
  Map_CopyCellAttributes(29, 23, 1, 1, 14, 53);
  Map_CopyCellAttributes(29, 23, 1, 1, 13, 53);
  Map_CopyCellAttributes(26, 20, 2, 1, 14, 52);
  Map_CopyCellAttributes(25, 21, 1, 1, 13, 54);
  Map_CopyCellAttributes(25, 21, 1, 1, 15, 54);
  Map_CopyCellAttributes(14, 53, 1, 1, 14, 54);
  Map_CopyCellAttributes(13, 55, 1, 1, 15, 55);
}

// Walking past: rock 13 falls down the path.
static void Event_Rock13Falls(void) {
  if (GameFlag_IsSet(0x312) == 0) {
    Engine_EventBegin();
    if (GameFlag_IsSet(FLAG_VALE_STORM_THIRD_ROCK_FELL) == 0) {
      struct FieldActor *actor = Actor_Get(13);
      struct FieldActor *leader = Actor_Get(ACTOR_PARTY_LEADER);
      u16 priority = leader->sprite->image.priority;
      u8 flags = leader->priorityFlags;

      Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      Event_PlayCue(SE_141);
      Engine_TaskWait(40);
      Event_PlayCue(SE_145);
      Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
      Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      Actor_SetPosition(13, 0, PIXELS(703));
      actor->speed = PIXELS(3) / 2;
      actor->acceleration = PIXELS(3) / 2;
      actor->y.fixed += PIXELS(80);
      actor->targetY = actor->y.fixed;
      actor->bounce = 0x8000;
      Actor_WalkToAndWait(13, 64, 703);
      Engine_EventWait(40);
      Event_PlayCue(SE_289);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      Engine_MapWaitWorkValuesBelow256();
      BattleFx_PlayQueuedSound();
      GameFlag_Set(FLAG_VALE_STORM_THIRD_ROCK_FELL);
      Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, priority);
      Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      leader->priorityFlags = flags;
    }
    ValeStorm_ApplyRock13Collision();
    GameFlag_Set(0x312);
    if (GameFlag_IsSet(FLAG_VALE_STORM_GARET_JOINED) != 0) {
      if (GameFlag_IsSet(FLAG_VALE_STORM_RIVER_GATHERING_SEEN) == 0) {
        if (GameFlag_IsSet(0x30c) == 0) {
          if (Actor_Get(ACTOR_PARTY_LEADER)->z.fixed <= 0x2b4ffff) {
            ValeStorm_GaretCommentsOnBlockedPath(62, 0x29d);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 27, 627);
          } else {
            ValeStorm_GaretCommentsOnBlockedPath(75, 0x2cb);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 67, 757);
          }
          GameFlag_Set(0x30c);
        }
      }
    }
    Engine_EventEnd();
  }
}

// Apply the collision cells left by rock 13.
static void ValeStorm_ApplyRock13Collision(void) {
  Map_CopyCellAttributes(29, 22, 1, 1, 3, 42);
  Map_CopyCellAttributes(29, 21, 1, 1, 2, 42);
  Map_CopyCellAttributes(29, 21, 1, 1, 4, 42);
  Map_CopyCellAttributes(23, 20, 3, 1, 2, 43);
}

// Walking past: rock 14 falls down the path.
static void Event_Rock14Falls(void) {
  struct FieldActor *rock;
  struct FieldActor *leader;
  s32 targetHeight;

  if (GameFlag_IsSet(0x313) != 0) {
  } else {
    Engine_EventBegin();
    if (GameFlag_IsSet(FLAG_VALE_STORM_FOURTH_ROCK_FELL) == 0) {
      rock = Object_GetById(14);
      Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      Engine_AudioPlayCue(SE_141);
      Engine_TaskWait(40);
      Engine_AudioPlayCue(SE_145);
      Actor_SetPosition(14, PIXELS(474), PIXELS(1147));
      rock->speed = PIXELS(1);
      rock->acceleration = PIXELS(1);
      targetHeight = (rock->y.fixed + PIXELS(72));
      rock->y.fixed += PIXELS(72);
      rock->targetY = targetHeight;
      rock->bounce = 0x8000;
      Engine_ActorWalkToAndWait(14, 432, 1147);
      Engine_EventWait(40);
      Engine_AudioPlayCue(SE_289);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      Engine_MapWaitWorkValuesBelow256();
      BattleFx_PlayQueuedSound();
      Engine_GameFlagSet(FLAG_VALE_STORM_FOURTH_ROCK_FELL);
    }
    ValeStorm_ApplyRock14Collision();
    Engine_GameFlagSet(0x313);
    if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_JOINED) != 0) {
      if (Engine_GameFlagIsSet(FLAG_VALE_STORM_RIVER_GATHERING_SEEN) == 0) {
        if (Engine_GameFlagIsSet(0x30c) == 0) {
          leader = Object_GetById(0);
          if (leader->z.fixed <= 0x479ffff) {
            ValeStorm_GaretCommentsOnBlockedPath(0x19c, 0x460);
            Actor_WalkToAndWait(0, 414, 1068);
          } else {
            ValeStorm_GaretCommentsOnBlockedPath(0x1bd, 0x494);
            Actor_WalkToAndWait(0, 447, 1227);
          }
          Engine_GameFlagSet(0x30c);
        }
      }
    }
    Engine_EventEnd();
  }
}

// Apply the collision cells left by rock 14.
static void ValeStorm_ApplyRock14Collision(void) {
  Map_CopyCellAttributes(29, 20, 1, 1, 26, 71);
  Map_CopyCellAttributes(29, 20, 1, 1, 26, 70);
  Map_CopyCellAttributes(29, 20, 1, 1, 27, 70);
  Map_CopyCellAttributes(28, 21, 1, 1, 28, 71);
  Map_CopyCellAttributes(28, 22, 1, 1, 27, 72);
}

// Stop the leader at exit 6 for the first villager conversation.
static void ValeStorm_MeetVillagerAtExit6(void) {
  s32 message;

  Engine_EventBegin();
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 262, 810);
  Actor_SetPosition(20, PIXELS(262), PIXELS(805));
  Actor_WalkTo(20, 262, 825);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 282, 855);
  Engine_ActorSetAnimation(20, ANIM_STAND);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 20, 0);
  BattleFx_PlayQueuedSound();
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_ShowEmote(20, EMOTE_IN_FRONT | 0, 20);
  message = MsgValeHuh;
  Engine_EventSetMessage(message);
  Event_ShowMessage(20, 0);
  Engine_EventWait(20);
  Event_AskYesNo(20, 0);
  Engine_ActorRunRepeatedMotion(20, 2);
  Engine_EventSetMessage((message + 4));
  Event_ShowMessageAndWait(20, 0, 20);
  Actor_RunActionScriptAndWait(20, gValeStorm_ActorTwentyScript);
  GameFlag_Set(FLAG_VALE_STORM_BUMPED_INTO_VILLAGER);
  Engine_EventEnd();
}

// Isaac hears Garet struggling with his things ("Ugh... Hrnghhh...").
static void Event_HearGaretStruggling(void) {
  if (GameFlag_IsSet(FLAG_VALE_STORM_HEARD_GARET) == 0) {
    if (GameFlag_IsSet(FLAG_VALE_STORM_GARET_JOINED) == 0) {
      Engine_EventBegin();
      Engine_EventSetMessage(MsgValeUghHrnghhh);
      Event_ShowMessageAndWait(22, 0, 20);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 40);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 382, 619);
      Actor_FaceActor(ACTOR_PARTY_LEADER, 22, 0);
      Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Engine_EventWait(30);
      Event_ShowMessage(22, 0);
      GameFlag_Set(FLAG_VALE_STORM_HEARD_GARET);
      Engine_EventEnd();
    }
  }
}

// Walking away from him, Garet calls after Isaac, asking whether he will just
// leave him there.
static void Event_GaretCallsAfterIsaac(void) {
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_JOINED) == 0 &&
      Engine_GameFlagIsSet(FLAG_VALE_STORM_HEARD_GARET) != 0) {
    Engine_EventBegin();
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgValeDontLeaveMeHere);
    ValeStorm_RunCallOutSequence();
    Engine_EventEnd();
  }
}

// Talking to Garet: whether to leave his things behind, or, after the river,
// the Boulder.
static void Event_TalkToGaret(void) {
  if (GameFlag_IsSet(FLAG_VALE_STORM_RIVER_GATHERING_SEEN) != 0) {
    Engine_EventBegin();
    Actor_FaceActor(22, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgValeBoulderNeedGet);
    Event_ShowMessage(22, 0);
    Actor_FaceDirection(22, FACING_NORTHEAST, 10);
    Engine_EventEnd();
  } else {
    if (GameFlag_IsSet(FLAG_VALE_STORM_GARET_JOINED) == 0) {
      Engine_EventBegin();
      Engine_EventSetMessage(MsgValeWantDumpStuff);
      ValeStorm_RunCallOutSequence();
      Engine_EventEnd();
    }
  }
}

// Resolve the belongings conversation and bring Garet into the party.
static void ValeStorm_RunCallOutSequence(void) {
  struct FieldActor *leader;
  s32 agreedToLeaveBelongings;

  Engine_EventOpenMessage(22, 0);
  Engine_ActorFaceEachOther(0, 22, 0);
  agreedToLeaveBelongings = 0;
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(MsgValeThinkForgetThings);
    agreedToLeaveBelongings = 1;
  } else {
    Engine_EventSetMessage(MsgValeRockHitsLose);
  }
  Engine_EventWait(20);
  Engine_EventShowMessageAndWait(22, 0, 40);
  Actor_SetAttachedEffect(22, 0x100);
  Engine_ActorSetAnimation(21, ANIM_NOD);
  Engine_ActorSetAnimation(22, ANIM_STAND);
  Engine_EventWait(40);
  Engine_ActorFaceActor(22, 0, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(22, ANIM_NOD);
  if (agreedToLeaveBelongings != 0) {
    Engine_EventSetMessage(MsgValeKnowRightOk);
  } else {
    Engine_EventSetMessage(MsgValeRightDitchStuff);
  }
  Engine_EventShowMessage(22, 0);
  Engine_ActorSetAnimation(22, ANIM_WALK);
  leader = Object_GetById(0);
  if (leader != 0) {
    Engine_ActorSetDestination(22, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(22);
  Engine_ActorSetPosition(22, 0, 0);
  Event_PrepareObjectAndApplyValue(1, 1);
  Engine_GameFlagSet(FLAG_VALE_STORM_GARET_JOINED);
}

// Garet catches up ("Hey! Isaac!") and joins Isaac.
static void Event_GaretJoinsIsaac(void) {
  struct FieldActor *actor;
  s32 message;

  if (GameFlag_IsSet(FLAG_VALE_STORM_GARET_JOINED) == 0) {
    Engine_EventBegin();
    Actor_SetAttachedEffect(22, 0x100);
    message = MsgValeHey;
    Engine_EventSetMessage(message);
    Event_ShowMessage(22, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
    Camera_MoveTo(PIXELS(256), -1, PIXELS(588), 1);
    Actor_SetSpeed(22, PIXELS(2), PIXELS(1));
    Actor_RunActionScriptAndWait(22, gValeStorm_ActorTwentyTwoScriptA);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
    Engine_EventWait(30);
    Actor_SetActionScript(22, gValeStorm_ActorTwentyTwoScriptB);
    Event_ShowMessage(22, 0);
    actor = Actor_Get(22);
    actor->scaleY = Q16_ONE;
    Engine_ActorRunRepeatedMotion(22, 1);
    Engine_EventWait(20);
    Event_AskYesNo(22, 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(22, 1);
    Engine_EventSetMessage((message + 5));
    Event_ShowMessageAndWait(22, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(22, ANIM_NOD);
    Event_ShowMessage(22, 0);
    Actor_SetSpeed(22, PIXELS(1), PIXELS(1) / 2);
    Engine_ActorSetAnimation(22, ANIM_WALK);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Actor_SetDestination(22, actor->x.part.pixel, actor->z.part.pixel);
    }
    Engine_ActorWaitForMove(22);
    Actor_SetPosition(22, 0, 0);
    Event_PrepareObjectAndApplyValue(1, 1);
    Engine_ActorSetAnimation(21, ANIM_NOD);
    GameFlag_Set(FLAG_VALE_STORM_GARET_JOINED);
    Engine_EventEnd();
  }
}

// Garet warns that the path north leads to Mt. Aleph.
static void Event_WarnAboutMtAleph(void) {
  struct FieldActor *leader;
  struct FieldActor *initialLeader;

  Engine_EventBegin();
  initialLeader = Actor_Get(ACTOR_PARTY_LEADER);
  if (initialLeader != NULL) {
    Actor_SetPosition(22, initialLeader->x.fixed, initialLeader->z.fixed);
  }
  Actor_SetSpeed(22, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(22, 281, 507);
  Engine_ActorFaceEachOther(22, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(30);
  Engine_EventSetMessage(MsgValeNorthLeadsToMtAleph);
  Event_ShowMessage(22, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 22, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(22, FACING_SOUTH, 0);
  Event_ShowMessage(22, 0);
  Engine_ActorSetAnimation(22, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(22, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(22);
  Actor_SetPosition(22, 0, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 256, 517);
  Engine_EventEnd();
}

// Bring Garet out to discuss a blocked path, then return him to Isaac.
static void ValeStorm_GaretCommentsOnBlockedPath(s32 x, s32 y) {
  struct FieldActor *leaderAtStart;
  s32 speed = PIXELS(1);
  s32 acceleration = (PIXELS(1) / 2);
  struct FieldActor *leaderAtEnd;

  leaderAtStart = Object_GetById(ACTOR_PARTY_LEADER);
  if (leaderAtStart != 0) {
    Engine_ActorSetPosition(22, leaderAtStart->x.fixed, leaderAtStart->z.fixed);
  }
  Engine_ActorSetSpeed(22, speed, acceleration);
  Engine_ActorWalkToAndWait(22, x, y);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
  Engine_EventWait(20);
  Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Engine_EventWait(40);
  Engine_EventSetMessage(MsgValeCantGetAroundThisRock);
  Engine_EventShowMessage(22, 0);
  Engine_ActorRunRepeatedMotion(22, 2);
  Engine_EventShowMessage(22, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(22, ANIM_WALK);
  leaderAtEnd = Object_GetById(ACTOR_PARTY_LEADER);
  if (leaderAtEnd != 0) {
    Engine_ActorSetDestination(22, leaderAtEnd->x.part.pixel,
                               leaderAtEnd->z.part.pixel);
  }
  Engine_ActorWaitForMove(22);
  Engine_ActorSetPosition(22, 0, 0);
}

// The Boulder falls toward Vale while the elders hold it with their Psynergy
// ("Ahhh! The Boulder! It's falling!").
static void Event_WatchBoulderFall(void) {
  u32 i;
  struct FieldActor *movingActor;
  s32 *shakeShift;
  struct FieldActor *actor;
  SchedulerCallback updateEffects;
  SchedulerCallback cycleBoulderPalette;
  s32 boulderHeight;

  if (GameFlag_IsSet(FLAG_VALE_BOULDER_FELL) == 0) {
    Engine_EventBegin();
    Event_CallWithLastActiveObjectId(gValeStorm_BoulderWatchers);
    SceneState_SetValue140Mode0();
    Engine_TaskWait(1);
    Event_PlayCue(SE_141);
    Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Engine_EventWait(30);
    Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
    Event_PlayCue(SE_145);
    Engine_EventWait(30);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetPosition(22, actor->x.fixed, actor->z.fixed);
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
    Actor_SetSpeed(22, PIXELS(2), PIXELS(1));
    Actor_SetActionScript(0, gValeStorm_LeaderScript);
    Actor_RunActionScriptAndWait(22, gValeStorm_ActorTwentyTwoScript);
    Object_RefreshSelectorById(0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 0, 0);
    Actor_ShowEmote(22, EMOTE_IN_FRONT | 0, 30);
    Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
    Event_PlayCue(SE_145);
    Engine_EventWait(40);
    Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
    Event_PlayCue(SE_145);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(22, 0x102);
    Engine_EventWait(40);
    // Show the actors holding back the boulder.
    Engine_ActorSetAnimation(32, 5);
    Engine_ActorSetAnimation(33, 5);
    Engine_ActorSetAnimation(30, 8);
    Engine_ActorSetAnimation(29, 8);
    Actor_Get(30)->scaleX = -Q16_ONE;
    Engine_ActorSetSpritePriority(32, 2);
    Engine_ActorSetSpritePriority(33, 2);
    Engine_ActorSetSpritePriority(30, 3);
    Engine_ActorSetSpritePriority(29, 3);
    Engine_EventSetMessage(MsgValeTheBoulderIsFalling);
    Event_ShowMessageAndWait(28, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(22, FACING_NORTH, 20);
    Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
    Camera_MoveTo(PIXELS(112), -1, PIXELS(331), 1);
    Engine_CameraWaitForMove();
    for (i = 0; i < 40; i++) {
      ValeStorm_FlashPaletteAndSpawnEffects(Actor_Get(32));
      ValeStorm_FlashPaletteAndSpawnEffects(Actor_Get(33));
      ValeStorm_FlashPaletteAndSpawnEffects(Actor_Get(30));
      ValeStorm_FlashPaletteAndSpawnEffects(Actor_Get(29));
      Engine_TaskWait(1);
    }
    // Run the palette effects while the boulder rocks back and forth.
    shakeShift = &sShakeShift;
    updateEffects = ValeStorm_UpdateBoulderSceneEffects;
    sShakeDone = 0;
    *shakeShift = 0;
    FieldScheduler_AddOrUpdateCallback(updateEffects, STORM_CALLBACK_PRIORITY);
    cycleBoulderPalette = ValeStorm_CycleBoulderPalette;
    FieldScheduler_AddCallback(cycleBoulderPalette, STORM_CALLBACK_PRIORITY);
    Engine_EventWait(40);
    *shakeShift = 1;
    Engine_EventWait(30);
    Actor_SetPosition(19, PIXELS(114), PIXELS(290));
    movingActor = Actor_Get(19);
    boulderHeight = movingActor->y.fixed + PIXELS(64);
    movingActor->y.fixed = boulderHeight;
    movingActor->targetY = boulderHeight;
    Actor_SetSpeed(19, PIXELS(4) / 5, PIXELS(2) / 5);
    Event_PlayCue(SE_145);
    Actor_MoveToAndWait(19, 114, 333);
    Engine_ActorSetAnimation(19, ANIM_WALK);
    Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Event_PlayCue(SE_145);
    *shakeShift = 0;
    Actor_SetSpeed(19, PIXELS(2) / 5, PIXELS(1) / 5);
    Actor_MoveToAndWait(19, 114, 300);
    Engine_ActorSetAnimation(19, ANIM_WALK);
    Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
    Event_PlayCue(SE_145);
    *shakeShift = 2;
    Actor_SetSpeed(19, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_MoveToAndWait(19, 114, 333);
    Engine_ActorSetAnimation(19, ANIM_WALK);
    Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Event_PlayCue(SE_145);
    *shakeShift = 0;
    Actor_SetSpeed(19, PIXELS(2) / 5, PIXELS(1) / 5);
    Actor_MoveToAndWait(19, 114, 300);
    Engine_ActorSetAnimation(19, ANIM_WALK);
    Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
    Event_PlayCue(SE_145);
    *shakeShift = 2;
    Actor_SetSpeed(19, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_MoveToAndWait(19, 114, 333);
    Engine_ActorSetAnimation(19, ANIM_WALK);
    Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Event_PlayCue(SE_145);
    *shakeShift = 1;
    Engine_EventWait(20);
    Actor_SetAttachedEffect(32, 0x102);
    Engine_ActorRunRepeatedMotion(32, 2);
    Event_ShowMessage(31, 0);
    Actor_ShowEmote(33, EMOTE_IN_FRONT | 0, 0);
    Engine_ActorRunRepeatedMotion(33, 2);
    Event_ShowMessageAndWait(28, 0, 40);
    Actor_SetAttachedEffect(30, 0x102);
    Engine_ActorRunRepeatedMotion(30, 2);
    Event_ShowMessage(30, 0);
    // Stop the final actor effect and return the camera to Isaac.
    sShakeDone = 1;
    Engine_ActorSetAnimation(29, ANIM_STAND);
    Engine_TaskWait(1);
    Actor_SetChildValue(29, 0);
    Actor_ShowEmote(29, EMOTE_IN_FRONT | 5, 20);
    Actor_FaceDirection(29, FACING_WEST, 40);
    Actor_FaceDirection(29, FACING_EAST, 20);
    Actor_FaceDirection(29, FACING_WEST, 20);
    Actor_FaceDirection(29, FACING_SOUTH, 40);
    Actor_ShowEmote(29, EMOTE_IN_FRONT | 0, 0);
    Engine_ActorRunRepeatedMotion(29, 2);
    Engine_ActorJump(29, 4, 40);
    Engine_ActorSetAnimation(29, 9);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(29, 0, 20);
    Event_PlayCue(SE_289);
    Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
    Camera_SetSpeed(PIXELS(6), PIXELS(3) / 4);
    Camera_MoveTo(PIXELS(84), -1, PIXELS(564), 1);
    Engine_CameraWaitForMove();
    BattleFx_PlayQueuedSound();
    Actor_FaceActor(22, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(22, 0x102);
    Engine_EventWait(30);
    Scheduler_RemoveCallback(updateEffects);
    Scheduler_RemoveCallback(cycleBoulderPalette);
    Event_ShowMessage(22, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 22, 0);
    Engine_EventWait(20);
    SceneEffect_CleanupSceneObjects();
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(22, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(22, ANIM_WALK);
    movingActor = Object_GetById(ACTOR_PARTY_LEADER);
    if (movingActor != 0) {
      Actor_SetDestination(22, movingActor->x.part.pixel,
                           movingActor->z.part.pixel);
    }
    Engine_ActorWaitForMove(22);
    Actor_SetPosition(22, 0, 0);
    Engine_ActorDestroy(31);
    Engine_ActorDestroy(28);
    Engine_ActorDestroy(30);
    Engine_ActorDestroy(29);
    Engine_ActorDestroy(32);
    Engine_ActorDestroy(33);
    GameFlag_Set(FLAG_VALE_BOULDER_FELL);
    Engine_EventEnd();
  }
}

// Apply an animation parameter to each active entry of a single sprite.
static void Storm_SetActorFrameMode(struct FieldActor *actor, s32 mode) {
  if ((actor->spriteStorageKind & 0xf) == 1) {
    struct AnimationObject *animation = actor->sprite;
    s32 animationParameter = mode - 1;
    u8 count;
    if (mode == 0) {
      animationParameter =
          gValeStorm_FrameModes[(gFrameCount >> 1) &
                                (actor->spriteStorageKind & 0xf)];
    }
    count = animation->count;
    if (count != 0) {
      struct AnimationEntry **entries = animation->entries;
      s32 remainingEntries = count;
      do {
        struct AnimationEntry *entry = *entries++;
        if (entry != NULL && entry->script != NULL) {
          entry->param = animationParameter;
        }
        remainingEntries--;
      } while (remainingEntries != 0);
    }
    animation->dirty = 1;
  }
}

// Alternate four actors between the normal and flashing parameters.
UNUSED static void ValeStorm_FlashBoulderSceneAnimationEntries(void) {
  if (((gFrameCount >> sShakeShift) & 3) != 0) {
    Storm_SetActorFrameMode(Object_GetById(32), 1);
    Storm_SetActorFrameMode(Object_GetById(33), 1);
    Storm_SetActorFrameMode(Object_GetById(30), 1);
    Storm_SetActorFrameMode(Object_GetById(29), 1);
  } else {
    Storm_SetActorFrameMode(Object_GetById(32), 8);
    Storm_SetActorFrameMode(Object_GetById(33), 8);
    Storm_SetActorFrameMode(Object_GetById(30), 8);
    Storm_SetActorFrameMode(Object_GetById(29), 8);
  }
}

// At the river, Jenna cries out for her brother as the villagers try to save
// Felix ("Oh no! My brother!").
static void Event_WitnessRiverRescue(void) {
  const s32 *pulseScript;

  if (GameFlag_IsSet(FLAG_VALE_STORM_SAW_RIVER_RESCUE) != 0) {
    return;
  }
  Engine_EventBegin();
  Actor_SetPosition(10, PIXELS(192), PIXELS(1214));
  Actor_FaceDirection(10, FACING_SOUTHEAST, 0);
  Engine_ActorSetAnimation(10, 5);
  {
    struct FieldActor *actor;
    s32 pulseDelay;
    actor = Object_GetById(10);
    pulseDelay = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    pulseScript = gValeStorm_ActorEightScript;
    actor->timer = pulseDelay;
    Actor_SetActionScript(10, pulseScript);
  }
  Actor_SetPosition(9, PIXELS(192), PIXELS(1189));
  Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
  Actor_SetPosition(24, PIXELS(227), PIXELS(1214));
  Actor_FaceDirection(24, FACING_SOUTH, 0);
  Engine_ActorSetAnimation(24, 6);
  {
    struct FieldActor *actor;
    s32 pulseDelay;
    actor = Object_GetById(24);
    pulseDelay = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    actor->timer = pulseDelay;
    Actor_SetActionScript(24, pulseScript);
  }
  Actor_SetPosition(25, PIXELS(250), PIXELS(1214));
  Actor_FaceDirection(25, FACING_SOUTH, 0);
  Engine_ActorSetAnimation(25, 6);
  {
    struct FieldActor *actor;
    s32 pulseDelay;
    actor = Object_GetById(25);
    pulseDelay = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    actor->timer = pulseDelay;
    Actor_SetActionScript(25, pulseScript);
  }
  Actor_SetPosition(26, PIXELS(227), PIXELS(1189));
  Actor_FaceDirection(26, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetPosition(23, PIXELS(243), PIXELS(1277));
  Actor_FaceDirection(23, FACING_NORTH, 0);
  Engine_ActorSetSpriteFlags(Object_GetById(23), 0);
  Engine_TaskWait(3);
  Engine_EventSetMessage(MsgValeNoBrother);
  Engine_EventShowMessage(0x201a, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 0, 20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 150, 1094);
  {
    struct FieldActor *leader;
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetPosition(22, leader->x.fixed, leader->z.fixed);
    }
  }
  Actor_WalkToAndWait(22, 132, 1094);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(22, FACING_SOUTH, 20);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(216), -1, PIXELS(1232), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(10, 2);
  Engine_EventShowMessageAndWait(10, 0, 10);
  Engine_ActorRunRepeatedMotion(23, 3);
  Engine_ActorFaceDirection(9, FACING_EAST, 10);
  Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
  Engine_EventShowMessageAndWait(9, 0, 10);
  Actor_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP, 10);
  Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
  Camera_MoveTo(PIXELS(232), -1, PIXELS(1253), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_AudioPlayCue(SE_HEAVY_IMPACT);
  Engine_ActorJump(23, 4, 0);
  Engine_ActorSetAnimation(23, 6);
  Engine_EventWait(10);
  Engine_ActorSetPosition(23, 0, 0);
  Engine_EventWait(60);
  BattleFx_PlayQueuedSound();
  Engine_ActorSetAnimation(10, ANIM_STAND);
  {
    struct FieldActor *actor;
    actor = Object_GetById(10);
    actor->scaleX = Q16_ONE;
    actor->scaleY = Q16_ONE;
  }
  Engine_ActorSetAnimation(24, ANIM_STAND);
  {
    struct FieldActor *actor;
    actor = Object_GetById(24);
    actor->scaleX = Q16_ONE;
    actor->scaleY = Q16_ONE;
  }
  Engine_ActorSetAnimation(25, ANIM_STAND);
  {
    struct FieldActor *actor;
    actor = Object_GetById(25);
    actor->scaleX = Q16_ONE;
    actor->scaleY = Q16_ONE;
  }
  Engine_ActorStartRepeatedMotion(10, 2);
  Engine_ActorStartRepeatedMotion(9, 2);
  Engine_ActorStartRepeatedMotion(24, 2);
  Engine_ActorStartRepeatedMotion(25, 2);
  Engine_ActorRunRepeatedMotion(26, 2);
  Camera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  Camera_MoveTo(PIXELS(216), -1, PIXELS(1232), 1);
  Engine_CameraWaitForMove();
  Actor_SetAttachedEffect(26, 0x102);
  Engine_ActorSetAttachedEffect(9, 0x102);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(26, 2);
  Engine_ActorStartRepeatedMotion(26, 3);
  Engine_EventShowMessage(26, 0);
  Engine_ActorJump(25, 2, 0);
  Actor_SetDestination(25, 234, 1205);
  Engine_ActorJump(26, 2, 0);
  Actor_SetDestination(26, 227, 1201);
  Engine_EventWait(90);
  Camera_MoveTo(PIXELS(232), -1, PIXELS(1253), 1);
  Engine_CameraWaitForMove();
  Engine_ActorSetPosition(23, PIXELS(243), PIXELS(1277));
  Engine_TaskWait(1);
  Engine_AudioPlayCue(SE_106);
  {
    struct FieldActor *actor;
    actor = Object_GetById(23);
    actor->velocityY = PIXELS(2);
  }
  Engine_EventWait(6);
  Engine_ActorSetAnimation(23, 7);
  Engine_EventWait(20);
  BattleFx_PlayQueuedSound();
  Engine_EventWait(20);
  Engine_CameraSetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Engine_CameraMoveTo(PIXELS(216), -1, PIXELS(1232), 1);
  Engine_CameraWaitForMove();
  Engine_ActorRunRepeatedMotion(24, 2);
  Engine_EventWait(20);
  Actor_ShowEmote(24, EMOTE_IN_FRONT | 5, 40);
  Engine_ActorFaceEachOther(24, 10, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(10, 2);
  Event_ShowMessage(0x800A, 0);
  {
    struct FieldActor *actor;
    actor = Object_GetById(25);
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  }
  {
    struct FieldActor *actor;
    actor = Object_GetById(26);
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  }
  Actor_SetSpeed(25, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(26, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetDestination(25, 247, 1210);
  Actor_MoveToAndWait(26, 227, 1189);
  {
    struct FieldActor *actor;
    actor = Object_GetById(25);
    actor->steeringFlags |= ACTOR_FACES_MOTION;
  }
  Object_GetById(26)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_ActorFaceDirection(26, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(25, FACING_WEST, 10);
  Engine_ActorSetAnimation(24, ANIM_SHAKE_HEAD);
  Engine_EventShowMessageAndWait(0x8018, 0, 10);
  Actor_FaceDirection(10, FACING_NORTH, 20);
  Engine_ActorFaceDirection(10, FACING_EAST, 10);
  Engine_ActorSetAnimation(10, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(0x800A, 0, 10);
  Actor_ShowEmote(24, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | 5, 60);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 6, 20);
  Actor_FaceDirection(9, FACING_WEST, 40);
  Actor_FaceDirection(9, FACING_NORTH, 20);
  Engine_ActorFaceDirection(9, FACING_EAST, 30);
  Actor_FaceDirection(9, FACING_SOUTH, 10);
  Engine_EventShowMessageAndWait(9, 0, 10);
  Actor_FaceDirection(10, FACING_NORTH, 0);
  Actor_FaceDirection(25, FACING_WEST + FACING_STEP, 0);
  Actor_FaceDirection(24, FACING_NORTHWEST, 0);
  Actor_FaceDirection(26, FACING_WEST, 10);
  Engine_ActorRunRepeatedMotion(10, 1);
  Event_ShowMessageAndWait(0x800A, 0, 10);
  Engine_ActorSetAnimation(9, ANIM_SHAKE_HEAD);
  Engine_EventShowMessageAndWait(9, 0, 10);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(24, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(25, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(26, EMOTE_IN_FRONT | 5, 40);
  Engine_ActorFaceDirection(9, FACING_EAST, 10);
  Engine_ActorFaceEachOther(24, 25, 0);
  Engine_EventWait(20);
  Engine_ActorFaceDirection(9, FACING_EAST, 0);
  Engine_ActorFaceDirection(10, FACING_EAST, 10);
  Actor_FaceDirection(24, FACING_WEST, 0);
  Actor_FaceDirection(25, FACING_WEST, 10);
  Engine_ActorSetAnimation(24, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(25, ANIM_NOD);
  Engine_ActorFaceEachOther(10, 9, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(10, 1);
  Engine_EventShowMessageAndWait(0x800A, 0, 10);
  Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
  Actor_FaceDirection(24, 0xD000, 10);
  Engine_ActorStartRepeatedMotion(24, 1);
  Engine_EventShowMessageAndWait(24, 0, 10);
  Engine_ActorFaceDirection(10, FACING_EAST, 0);
  Engine_ActorFaceDirection(9, FACING_EAST, 0);
  Engine_ActorRunRepeatedMotion(26, 1);
  Actor_FaceDirection(26, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(25, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimationAndWait(25, ANIM_NOD);
  Engine_EventShowMessageAndWait(25, 0, 10);
  Engine_ActorSetAnimationAndWait(26, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
  Engine_EventShowMessageAndWait(9, 0, 10);
  Actor_FaceDirection(26, FACING_WEST, 10);
  Engine_ActorSetAnimationAndWait(26, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(9, 1);
  Engine_EventShowMessageAndWait(9, 0, 10);
  // Search both river paths before asking the children to help.
  ValeStorm_RunRiverSearch();
  Engine_GameFlagSet(FLAG_VALE_STORM_SAW_RIVER_RESCUE);
  Engine_EventEnd();
}

// Send the searchers along the river and ask Isaac to help.
static void ValeStorm_RunRiverSearch(void) {
  struct FieldActor *villager;
  s32 random;
  const s32 *script;
  s32 northMessage;
  s32 helpMessage;
  struct FieldActor *leader;

  Actor_FaceDirection(26, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(24, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(25, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(10, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimation(26, ANIM_NOD);
  Engine_ActorSetAnimation(24, ANIM_NOD);
  Engine_ActorSetAnimation(25, ANIM_NOD);
  Engine_ActorSetAnimation(9, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(25, ANIM_NOD);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(134), -1, PIXELS(1195), 1);
  Actor_SetSpeed(26, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(9, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetActionScript(26, gValeStorm_ActorTwentySixScriptA);
  Actor_RunActionScriptAndWait(9, gValeStorm_ActorNineScriptA);
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_CellSteps2, 38, 72);
  Engine_EventWait(10);
  Actor_WalkToAndWait(9, 149, 1175);
  Actor_SetPosition(9, 0, 0);
  Actor_WalkToAndWait(25, 250, 1214);
  BattleFx_PlayQueuedSound();
  Actor_FaceDirection(10, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(24, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(25, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorSetAnimation(10, 5);
  Engine_ActorSetAnimation(24, 6);
  Engine_ActorSetAnimation(25, 6);
  villager = Actor_Get(10);
  random = Engine_RandomNext();
  villager->timer = (Math_RemainderUnsigned(random, 90) + 60);
  villager = Actor_Get(24);
  random = Engine_RandomNext();
  villager->timer = (Math_RemainderUnsigned(random, 90) + 60);
  villager = Actor_Get(25);
  random = Engine_RandomNext();
  villager->timer = (Math_RemainderUnsigned(random, 90) + 60);
  script = gValeStorm_ActorEightScript;
  Actor_SetActionScript(10, script);
  Actor_SetActionScript(24, script);
  Actor_SetActionScript(25, script);
  Object_RefreshSelectorById(26);
  Engine_EventWait(10);
  Event_PlayCue(SE_159);
  Map_AnimateCells(gValeStorm_CellSteps5, 38, 72);
  Engine_EventWait(30);
  BattleFx_PlayQueuedSound();
  Camera_MoveTo(PIXELS(112), -1, PIXELS(1225), 1);
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_CellSteps1, 35, 73);
  Engine_EventWait(20);
  BattleFx_PlayQueuedSound();
  Actor_SetActionScript(9, gValeStorm_ActorNineScriptB);
  Engine_EventWait(20);
  Actor_SetActionScript(26, gValeStorm_ActorTwentySixScriptB);
  Engine_EventWait(40);
  Event_PlayCue(SE_159);
  Map_AnimateCells(gValeStorm_CellSteps4, 35, 73);
  Object_RefreshSelectorById(26);
  BattleFx_PlayQueuedSound();
  Engine_EventWait(40);
  northMessage = MsgValeGoLookNorth;
  Engine_EventSetMessage(northMessage);
  Event_ShowMessageAndWait(9, 0, 20);
  Engine_ActorSetAnimationAndWait(26, ANIM_NOD);
  Event_ShowMessageAndWait(0x201a, 0, 40);
  Engine_ActorSetAnimation(9, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(26, ANIM_NOD);
  Engine_EventWait(30);
  Actor_SetActionScript(9, gValeStorm_ActorNineScriptC);
  Actor_SetActionScript(26, gValeStorm_ActorTwentySixScriptC);
  Engine_EventWait(40);
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(105), -1, PIXELS(1086), 1);
  Object_RefreshSelectorById(9);
  Actor_FaceDirection(9, FACING_EAST, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 0, 40);
  Event_ShowMessageAndWait(9, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(22, FACING_WEST, 10);
  Actor_WalkToAndWait(9, 105, 1086);
  Engine_ActorRunRepeatedMotion(9, 2);
  Event_OpenMessage(0x8009, 0);
  Actor_FaceDirection(22, FACING_EAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
    Engine_EventSetMessage((northMessage + 4));
  } else {
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventSetMessage((northMessage + 5));
  }
  Event_ShowMessage(0x8009, 0);
  Actor_FaceDirection(22, FACING_WEST, 40);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 0, 30);
  helpMessage = MsgValeDontSupposeTwo;
  Engine_EventSetMessage(helpMessage);
  Event_OpenMessage(0x8009, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
    Engine_EventSetMessage((helpMessage + 1));
    Event_ShowMessageAndWait(0x8009, 0, 30);
    Actor_FaceDirection(22, FACING_WEST, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(22, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
    Engine_EventWait(40);
  } else {
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 5, 90);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 3, 40);
    Engine_ActorSetAnimation(9, ANIM_SHAKE_HEAD);
    Engine_EventSetMessage((helpMessage + 2));
    Event_ShowMessage(0x8009, 0);
  }
  Actor_SetActionScript(9, gValeStorm_ActorNineScriptD);
  Engine_EventWait(90);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(22, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(22, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(22, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(22);
  Actor_SetPosition(22, 0, 0);
}

// A villager at the river talks of rescuing Felix.
static void Event_TalkAboutRescuingFelix(void) {
  s32 facing = FACING_SOUTHEAST;
  const s32 *pulseScript;

  Engine_EventBegin();
  Engine_ActorSetAnimation(10, ANIM_STAND);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(10, ACTOR_PARTY_LEADER, 20);
  if (Engine_GameFlagIsSet(0x30d) != 0) {
    Engine_EventSetMessage(MsgValeTwoDontEnough);
    Engine_EventShowMessageAndWait(10, 0, 10);
  } else {
    Engine_EventSetMessage(MsgValeOh);
    Engine_ActorStartRepeatedMotion(10, 1);
    Engine_EventShowMessageAndWait(10, 0, 10);
    Engine_ActorStartRepeatedMotion(10, 2);
    Engine_EventShowMessageAndWait(10, 0, 10);
  }
  Engine_ActorFaceDirection(10, facing, 20);
  Engine_ActorSetAnimation(10, 5);
  Engine_EventWait(10);
  {
    struct FieldActor *villager;
    s32 pulseDelay;
    villager = Object_GetById(10);
    pulseDelay = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    pulseScript = gValeStorm_ActorEightScript;
    villager->timer = pulseDelay;
    Actor_SetActionScript(10, pulseScript);
  }
  Engine_EventWait(20);
  Engine_GameFlagSet(0x30d);
  Engine_EventEnd();
}

// Jenna comes back to the river ("Mom! Dad! We're back!"), and the
// scene ends with the Boulder sweeping down it.
static void Event_GatherAfterRiverRescue(void) {
  struct FieldActor *leader;
  struct FieldActor *actor;
  s32 automaticPriority;
  struct AnimationObject *sprite;

  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) == 0) {
  } else {
    if (GameFlag_IsSet(FLAG_VALE_STORM_RIVER_GATHERING_SEEN) != 0) {
    } else {
      Engine_EventBegin();
      Actor_SetSpeed(0, PIXELS(1), PIXELS(1) / 2);
      Actor_SetSpeed(22, PIXELS(1), PIXELS(1) / 2);
      Actor_SetSpeed(26, PIXELS(1), PIXELS(1) / 2);
      Actor_SetSpeed(8, PIXELS(1), PIXELS(1) / 2);
      Actor_WalkToAndWait(0, 217, 1367);
      actor = Object_GetById(0);
      if (actor != 0) {
        Engine_ActorSetPosition(22, actor->x.fixed, actor->z.fixed);
      }
      Actor_WalkToAndWait(22, 235, 1367);
      Actor_FaceDirection(22, FACING_NORTHWEST + FACING_STEP, 0);
      actor = Object_GetById(0);
      if (actor != 0) {
        Engine_ActorSetPosition(26, actor->x.fixed, actor->z.fixed);
      }
      Actor_WalkToAndWait(26, 199, 1367);
      Actor_FaceDirection(26, FACING_NORTH + FACING_STEP, 0);
      Actor_SetPosition(25, PIXELS(247), PIXELS(1210));
      Engine_ActorFaceDirection(25, FACING_SOUTHWEST, 0);
      actor = Object_GetById(8);
      sprite = actor->sprite;
      actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      sprite->image.priority = 1;
      leader = Object_GetById(0);
      sprite = leader->sprite;
      leader->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      sprite->image.priority = 2;
      actor = Object_GetById(0);
      if (actor != 0) {
        Engine_ActorSetPosition(8, actor->x.fixed, actor->z.fixed);
      }
      Actor_WalkToAndWait(8, 221, 1385);
      Actor_FaceDirection(8, FACING_NORTHWEST + FACING_STEP, 60);
      Engine_ActorRunRepeatedMotion(26, 2);
      Engine_EventSetMessage(MsgValeMomDadBack);
      Engine_EventShowMessageAndWait(26, 0, 40);
      Actor_SetPosition(9, PIXELS(101), PIXELS(1197));
      Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
      Engine_EventShowMessageAndWait(0x1009, 0, 10);
      Actor_FaceDirection(26, FACING_NORTHWEST, 0);
      Engine_CameraSetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
      Camera_MoveTo(PIXELS(101), -1, PIXELS(1197), 1);
      Actor_SetSpeed(9, PIXELS(7) / 5, PIXELS(7) / 10);
      Actor_SetActionScript(9, gValeStorm_StormRunActions);
      Engine_EventWait(60);
      Engine_CameraSetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
      Camera_MoveTo(PIXELS(187), -1, PIXELS(1328), 1);
      Engine_CameraWaitForMove();
      Engine_EventWait(40);
      Engine_ActorRunRepeatedMotion(26, 2);
      Engine_EventShowMessageAndWait(26, 0, 20);
      Engine_ActorRunRepeatedMotion(9, 2);
      Engine_EventShowMessageAndWait(0x4009, 0, 20);
      Engine_CameraSetSpeed(PIXELS(2), PIXELS(1) / 4);
      Camera_MoveTo(PIXELS(221), -1, PIXELS(1385), 1);
      Engine_ActorFaceActor(0, 8, 0);
      Engine_ActorFaceActor(22, 8, 0);
      Actor_FaceDirection(26, FACING_SOUTHEAST + FACING_STEP, 80);
      Engine_CameraMoveTo(PIXELS(182), -1, PIXELS(1360), 1);
      Actor_WalkToAndWait(8, 182, 1384);
      Engine_ActorFaceActor(8, 9, 0);
      Engine_EventWait(30);
      Engine_ActorSetAnimationAndWait(8, ANIM_NOD);
      Engine_EventWait(10);
      Engine_ActorFaceActor(0, 9, 0);
      Engine_ActorFaceActor(22, 9, 0);
      Engine_ActorFaceActor(26, 9, 0);
      Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
      Engine_EventShowMessage(9, 0);
      Engine_ActorRunRepeatedMotion(26, 2);
      Engine_EventShowMessageAndWait(26, 0, 10);
      Actor_FaceDirection(9, FACING_NORTHEAST, 40);
      Actor_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP, 20);
      Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
      Engine_EventShowMessage(9, 0);
      Engine_ActorFaceEachOther(26, 8, 0);
      Engine_ActorFaceEachOther(22, 0, 0);
      Engine_EventWait(40);
      Engine_ActorFaceActor(0, 9, 0);
      Engine_ActorFaceActor(22, 9, 0);
      Engine_ActorFaceActor(26, 9, 0);
      Engine_ActorFaceActor(8, 9, 0);
      Engine_ActorRunRepeatedMotion(9, 2);
      Engine_EventWait(20);
      Engine_EventShowMessageAndWait(9, 0, 10);
      Engine_ActorSetAnimation(0, ANIM_NOD);
      Engine_ActorSetAnimation(26, ANIM_NOD);
      Engine_ActorSetAnimation(22, ANIM_NOD);
      automaticPriority = ACTOR_PRIORITY_AUTOMATIC;
      Engine_ActorSetAnimationAndWait(8, ANIM_NOD);
      leader->priorityFlags |= automaticPriority;
      actor = Object_GetById(8);
      actor->priorityFlags |= automaticPriority;
      ValeStorm_ShowBoulderSweepingDownriver();
      Engine_GameFlagSet(FLAG_VALE_STORM_RIVER_GATHERING_SEEN);
      Engine_EventEnd();
    }
  }
}

// Place an actor and its motion targets at the supplied coordinates.
static __inline__ void ValeStorm_PlaceActor(struct FieldActor *object,
                                            s32 horizontalPosition,
                                            s32 groundPosition) {
  object->x.fixed = horizontalPosition;
  object->y.fixed = horizontalPosition;
  object->targetX = horizontalPosition;
  object->targetY = horizontalPosition;
  object->z.fixed = groundPosition;
  object->targetZ = groundPosition;
}

// Shake the scene at the impact cue and wait for the beat.
static __inline__ void ValeStorm_PlayImpactBeat(s32 shakeAmplitude,
                                                s32 frames) {
  Map_SetShake(shakeAmplitude, shakeAmplitude, Q16_ONE);
  Event_PlayCue(SE_145);
  Event_Wait(frames);
}

// Follow the boulder downriver, then restore the riverbank actors.
static void ValeStorm_ShowBoulderSweepingDownriver(void) {
  struct AnimationObject *actorVisual;
  struct AnimationObject *groupVisual;
  struct FieldActor *actor;
  struct FieldActor *lookingActor;
  struct FieldActor *groupActor;
  struct FieldActor *currentActor;
  u32 random;
  const s32 *entryActions;
  s32 stoppedVelocity;
  u8 stoppedMotion;
  const s32 *lookAroundActions;
  s32 resetValue;
  const s32 *departureActions;

  actor = Actor_Get(19);
  groupActor = Actor_Get(27);
  actorVisual = actor->sprite;
  groupVisual = groupActor->sprite;
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(110), -1, PIXELS(1419), 1);
  Actor_SetSpeed(8, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(26, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(0, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(22, PIXELS(6) / 5, PIXELS(3) / 5);
  // Bring the group to the riverbank as the lightning subsides.
  entryActions = gValeStorm_ArriveScript;
  Actor_SetActionScript(8, entryActions);
  Event_Wait(10);
  Actor_SetActionScript(26, entryActions);
  FieldFx_LastLightning();
  Event_Wait(10);
  Actor_SetActionScript(0, entryActions);
  Event_Wait(10);
  FieldFx_StopLightning();
  Actor_SetActionScript(22, entryActions);
  Event_Wait(128);
  Event_ClearPathCellOverride();
  Camera_MoveTo(PIXELS(174), -1, PIXELS(1428), 1);
  Event_Wait(104);
  Camera_MoveTo(PIXELS(153), -1, PIXELS(1325), 1);
  Actor_WalkToAndWait(9, 158, 1272);
  Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
  Object_RefreshSelectorById(8);
  Actor_SetActionScript(8, gValeStorm_GatherPath8);
  Actor_SetActionScript(26, gValeStorm_GatherPath26);
  Actor_SetActionScript(0, gValeStorm_GatherPath0);
  Actor_RunActionScriptAndWait(22, gValeStorm_GatherPath22);
  ValeStorm_PlayImpactBeat(PIXELS(4), 20);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_Wait(60);
  Actor_ShowEmote(0, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(26, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(22, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(8, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 1, 60);
  Actor_FaceEachOther(26, 8, 0);
  Actor_FaceEachOther(22, 0, 0);
  Event_Wait(20);
  lookingActor = Actor_Get(0);
  random = Random_Next();
  random = Math_RemainderUnsigned(random, 20) + 20;
  lookingActor->timer = random;
  stoppedVelocity = 0;
  stoppedMotion = 0;
  lookingActor = Actor_Get(22);
  random = Random_Next();
  lookingActor->timer = (Math_RemainderUnsigned(random, 20) + 20);
  lookingActor = Actor_Get(26);
  random = Random_Next();
  lookingActor->timer = (Math_RemainderUnsigned(random, 20) + 20);
  lookingActor = Actor_Get(8);
  random = Random_Next();
  lookingActor->timer = (Math_RemainderUnsigned(random, 20) + 20);
  lookingActor = Actor_Get(9);
  random = Random_Next();
  lookingActor->timer = (Math_RemainderUnsigned(random, 20) + 20);
  lookAroundActions = gValeStorm_LookAroundScript;
  Actor_SetActionScript(9, lookAroundActions);
  Event_Wait(30);
  Actor_SetActionScript(0, lookAroundActions);
  Actor_SetActionScript(26, lookAroundActions);
  Actor_SetActionScript(22, lookAroundActions);
  Actor_SetActionScript(8, lookAroundActions);
  Event_Wait(10);
  Event_PlayCue(MUS_017);
  ValeStorm_PlayImpactBeat(PIXELS(3), 30);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_Wait(120);
  ValeStorm_PlayImpactBeat(PIXELS(3), 40);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Event_Wait(60);
  ValeStorm_PlayImpactBeat(PIXELS(4), 20);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_Wait(60);
  ValeStorm_PlayImpactBeat(PIXELS(3), 40);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_Wait(60);
  FieldFx_StopLightning();
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_Wait(1);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Camera_SetSpeed(PIXELS(8), PIXELS(8));
  Camera_MoveTo(PIXELS(217), -1, PIXELS(1084), 1);
  ColorBuffer_ApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(40);
  Task_Wait(40);
  // Stage the boulder and its glow for the downriver camera shot.
  Actor_SetChildValue(19, 0);
  currentActor = Actor_Get(19);
  Actor_SetSpriteFlags(currentActor, 0);
  currentActor = Actor_Get(27);
  Actor_SetSpriteFlags(currentActor, 0);
  groupActor->scaleX = 0xcccc;
  groupActor->scaleY = 0xcccc;
  groupActor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  groupVisual->image.priority = 1;
  ValeStorm_PlaceActor(actor, PIXELS(200), PIXELS(898));
  actor->motionFlags = stoppedMotion;
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  actorVisual->image.priority = 0;
  currentActor = Camera_GetEventObject();
  currentActor->targetX = ACTOR_NO_TARGET;
  currentActor = Camera_GetEventObject();
  currentActor->targetY = ACTOR_NO_TARGET;
  currentActor = Camera_GetEventObject();
  currentActor->targetZ = ACTOR_NO_TARGET;
  currentActor = Camera_GetEventObject();
  currentActor->velocityX = stoppedVelocity;
  currentActor = Camera_GetEventObject();
  currentActor->velocityY = stoppedVelocity;
  currentActor = Camera_GetEventObject();
  currentActor->velocityZ = stoppedVelocity;
  Task_Wait(1);
  Camera_MoveTo(PIXELS(247), PIXELS(128), PIXELS(917), 0);
  Engine_MapRedraw();
  Task_Wait(1);
  ColorBuffer_ApplyTarget(0x10003, 1);
  ColorBuffer_ApplyTarget(0x10000, 2);
  Engine_ColorBufferInterpolate(30);
  Task_Wait(30);
  // Follow the boulder script until its eighth movement phase.
  FieldScheduler_AddCallback(ValeStorm_UpdatePulsingGlow,
                             STORM_CALLBACK_PRIORITY);
  Actor_SetActionScript(19, gValeStorm_ActorNineteenRiseScript);
  Camera_SetSpeed(PIXELS(2), PIXELS(3) / 100);
  Camera_MoveTo(PIXELS(175), PIXELS(96), PIXELS(1086), 1);
  do {
    Task_Wait(1);
  } while ((s16)actor->work != 8);
  ColorBuffer_ApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(60);
  Task_Wait(60);
  Engine_MapWaitWorkValuesBelow256();
  currentActor = Camera_GetEventObject();
  currentActor->targetX = ACTOR_NO_TARGET;
  currentActor = Camera_GetEventObject();
  currentActor->targetY = ACTOR_NO_TARGET;
  currentActor = Camera_GetEventObject();
  currentActor->targetZ = ACTOR_NO_TARGET;
  resetValue = 0;
  currentActor = Camera_GetEventObject();
  currentActor->velocityX = resetValue;
  currentActor = Camera_GetEventObject();
  currentActor->velocityY = resetValue;
  currentActor = Camera_GetEventObject();
  currentActor->velocityZ = resetValue;
  Scheduler_RemoveCallback(ValeStorm_UpdatePulsingGlow);
  Actor_Stop(19);
  Task_Wait(1);
  Actor_SetAnimation(19, 0);
  groupActor->scaleX = 0x14000;
  groupActor->scaleY = 0x14000;
  groupVisual->offsetY = 2;
  groupVisual->scale = 0x14000;
  actor->scaleX = 2 * Q16_ONE;
  actor->scaleY = 2 * Q16_ONE;
  actor->x.fixed = resetValue;
  actor->z.fixed = resetValue;
  actor->targetX = resetValue;
  actor->targetZ = resetValue;
  Task_Wait(1);
  // Stage the riverbank reactions between lightning flashes.
  Actor_SetAnimation(23, 8);
  Actor_SetPosition(9, PIXELS(169), PIXELS(1264));
  Actor_FaceDirection(9, FACING_NORTH, 0);
  Actor_SetAnimation(9, 9);
  Actor_SetPosition(26, PIXELS(151), PIXELS(1292));
  Actor_FaceDirection(26, FACING_WEST, 0);
  Actor_SetAnimation(26, 5);
  Actor_SetPosition(8, PIXELS(170), PIXELS(1313));
  Actor_FaceDirection(8, FACING_SOUTHWEST, 0);
  Actor_SetAnimation(8, 5);
  Actor_SetPosition(0, PIXELS(185), PIXELS(1333));
  Actor_FaceDirection(0, FACING_SOUTHEAST, 0);
  Actor_SetAnimation(0, 17);
  Actor_SetPosition(22, PIXELS(169), PIXELS(1384));
  Actor_FaceDirection(22, FACING_SOUTH, 0);
  Actor_SetAnimation(22, 0);
  Camera_MoveTo(PIXELS(166), 0, PIXELS(1337), 0);
  Engine_MapRedraw();
  actor->motionFlags = resetValue;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  ValeStorm_FlashLightning();
  Actor_SetPosition(27, PIXELS(218), PIXELS(1176));
  Camera_MoveTo(PIXELS(210), 0, PIXELS(1196), 0);
  Engine_MapRedraw();
  groupActor->scaleX = 2 * Q16_ONE;
  groupActor->scaleY = 2 * Q16_ONE;
  FieldScheduler_AddCallback(ValeStorm_FlickerGlowOffset,
                             STORM_CALLBACK_PRIORITY);
  Actor_Stop(10);
  Actor_Stop(24);
  Actor_Stop(25);
  Task_Wait(1);
  groupActor = Actor_Get(10);
  groupVisual = groupActor->sprite;
  groupActor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  groupActor->scaleX = Q16_ONE;
  groupActor->scaleY = Q16_ONE;
  groupActor->facing = 0xd000;
  groupVisual->image.priority = 0;
  Actor_SetAnimation(10, 0);
  groupActor = Actor_Get(24);
  groupVisual = groupActor->sprite;
  groupActor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  groupActor->scaleX = Q16_ONE;
  groupActor->scaleY = Q16_ONE;
  groupActor->facing = 0xb000;
  groupVisual->image.priority = 0;
  Actor_SetAnimation(24, 5);
  groupActor = Actor_Get(25);
  groupVisual = groupActor->sprite;
  groupActor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  groupActor->scaleX = Q16_ONE;
  groupActor->scaleY = Q16_ONE;
  groupActor->facing = 0xb000;
  groupVisual->image.priority = 0;
  Actor_SetAnimation(25, 5);
  groupActor = Actor_Get(27);
  groupVisual = groupActor->sprite;
  ValeStorm_FlashLightning();
  actor->y.fixed = PIXELS(48);
  actor->x.fixed = PIXELS(214);
  actor->z.fixed = PIXELS(1216);
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  groupVisual->image.priority = 1;
  Actor_SetPosition(27, PIXELS(214), PIXELS(1216));
  Actor_FaceDirection(24, FACING_NORTH, 0);
  Actor_FaceDirection(25, FACING_NORTH, 20);
  GameFlag_Set(0x166);
  Map_SetLayerEntryFlag(0);
  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);
  Map_SetLayerEntryFlag(3);
  Map_SetLayerEntryFlag(4);
  Map_SetLayerEntryFlag(5);
  ColorBuffer_ApplyTarget(0x10003, 1);
  ColorBuffer_ApplyTarget(0x10000, 2);
  Engine_ColorBufferInterpolate(120);
  Task_Wait(160);
  ColorBuffer_ApplyTarget(0x7fff, 1);
  ColorBuffer_ApplyTarget(0x7fff, 2);
  Engine_ColorBufferInterpolate(80);
  Event_Wait(80);
  Event_Wait(100);
  Scheduler_RemoveCallback(ValeStorm_FlickerGlowOffset);
  groupVisual->scale = groupActor->scaleX;
  GameFlag_Clear(0x166);
  Map_ClearLayerEntryFlag(0);
  Map_ClearLayerEntryFlag(1);
  Map_ClearLayerEntryFlag(2);
  Map_ClearLayerEntryFlag(3);
  Map_ClearLayerEntryFlag(4);
  Map_ClearLayerEntryFlag(5);
  // Restore the riverbank and the idle actor scripts.
  ValeStorm_RebuildRiverbank();
  Actor_SetPosition(9, PIXELS(165), PIXELS(1229));
  Actor_SetAnimation(9, 1);
  actor = Actor_Get(9);
  actor->facing = FACING_NORTHEAST;
  random = Random_Next();
  actor->timer = Math_RemainderUnsigned(random, 90) + 60;
  departureActions = gValeStorm_ActorEightScript;
  actor->work = 1;
  Actor_SetActionScript(9, departureActions);
  Actor_SetPosition(26, PIXELS(165), PIXELS(1254));
  Actor_SetAnimation(26, 1);
  actor = Actor_Get(26);
  actor->facing = FACING_NORTHEAST;
  random = Random_Next();
  actor->timer = Math_RemainderUnsigned(random, 90) + 60;
  actor->work = 2;
  Actor_SetActionScript(26, departureActions);
  Actor_SetPosition(22, PIXELS(152), PIXELS(1285));
  Actor_SetAnimation(22, 1);
  actor = Actor_Get(22);
  actor->facing = FACING_NORTHEAST;
  random = Random_Next();
  actor->timer = Math_RemainderUnsigned(random, 90) + 60;
  actor->work = 3;
  Actor_SetActionScript(22, departureActions);
  Actor_SetPosition(8, PIXELS(180), PIXELS(1311));
  actor = Actor_Get(8);
  actor->facing = FACING_NORTHEAST;
  random = Random_Next();
  actor->timer = Math_RemainderUnsigned(random, 90) + 60;
  actor->work = 4;
  Actor_SetActionScript(8, departureActions);
  Actor_SetAnimation(8, 6);
  Object_GetById(22)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Object_GetById(8)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  FieldScheduler_AddCallback(ValeStorm_CopyLeaderSpritePriority,
                             STORM_CALLBACK_PRIORITY);
  Actor_SetPosition(0, PIXELS(181), PIXELS(1273));
  currentActor = Actor_Get(0);
  currentActor->facing = FACING_NORTHEAST;
  Actor_SetAnimation(0, 1);
  Camera_MoveTo(PIXELS(181), 0, PIXELS(1273), 0);
  Engine_MapRedraw();
  Actor_SetPosition(10, 0, 0);
  Actor_SetPosition(19, 0, 0);
  Actor_SetPosition(24, 0, 0);
  Actor_SetPosition(25, 0, 0);
  Actor_SetPosition(23, 0, 0);
  Actor_SetPosition(27, 0, 0);
  Actor_SetPosition(17, PIXELS(144), PIXELS(1070));
  Actor_SetPosition(18, PIXELS(276), PIXELS(1270));
  Task_Wait(60);
  ColorBuffer_ApplyTarget(0x10003, 1);
  ColorBuffer_ApplyTarget(0x10000, 2);
  Engine_ColorBufferInterpolate(80);
  Event_Wait(60);
  BattleFx_PlayQueuedSound();
  Event_Wait(60);
  Party_RemoveOwnerRestored(1);
  FieldFx_ResumeLightning();
}

struct PairWork {
  u8 unk00[70];
  u16 vramBlock;
};

// Flash the scene layers before fading back to the storm palette.
static void ValeStorm_FlashLightning(void) {
  Engine_TaskWait(20);
  GameFlag_Set(0x166);
  Map_SetLayerEntryFlag(0);
  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);
  Map_SetLayerEntryFlag(3);
  Map_SetLayerEntryFlag(4);
  Map_SetLayerEntryFlag(5);
  ColorBuffer_ApplyTarget(0x10003, 1);
  ColorBuffer_ApplyTarget(0x10000, 2);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(120);
  ColorBuffer_ApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  GameFlag_Clear(0x166);
  Map_ClearLayerEntryFlag(0);
  Map_ClearLayerEntryFlag(1);
  Map_ClearLayerEntryFlag(2);
  Map_ClearLayerEntryFlag(3);
  Map_ClearLayerEntryFlag(4);
  Map_ClearLayerEntryFlag(5);
}

// Track the boulder and pulse its glow with the movement script phases.
static void ValeStorm_UpdatePulsingGlow(void) {
  struct FieldActor *source;
  struct FieldActor *glow;
  struct AnimationObject *sprite;
  s16 *timer;

  source = Object_GetById(19);
  glow = Object_GetById(27);
  sprite = glow->sprite;
  timer = &source->timer;
  if (*timer != 0) {
    if (*timer == 60) {
      Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
    }
    if (*timer == 40) {
      Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
    }
    if (*timer == 30) {
      Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    }
    if (*timer == 20) {
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
    }
    (*timer)--;
  }
  glow->x.fixed = source->x.fixed;
  glow->z.fixed = source->z.fixed;
  glow->targetX = glow->x.fixed;
  sprite->offsetY = 10;
  if (gFrameCount & 1) {
    switch ((s16)source->work) {
    case 1:
    case 5:
      glow->scaleX += 0xa3d;
      glow->scaleY += 0xa3d;
      break;
    case 4:
      glow->scaleX += 0x51e;
      glow->scaleY += 0x51e;
      break;
    case 2:
    case 3:
    case 6:
    case 7:
    case 8:
    case 9:
      glow->scaleX += -0x7ae;
      glow->scaleY += -0x7ae;
      break;
    }
    sprite->scale = glow->scaleX;
  } else {
    sprite->scale = 0;
  }
}

// Alternate the glow vertical offset on successive frames.
static void ValeStorm_FlickerGlowOffset(void) {
  struct FieldActor *actor = Object_GetById(27);
  u32 frame = gFrameCount;
  struct AnimationObject *sprite = actor->sprite;

  if (frame & 1) {
    sprite->offsetY = 2;
  } else {
    sprite->offsetY = 64;
  }
}

// A villager who cannot move asks Isaac to get help ("I can't move...").
static void Event_VillagerAsksForHelp(void) {
  struct FieldActor *villager;
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  villager = Object_GetById(8);
  Engine_EventBegin();
  if (GameFlag_IsSet(0x305) != 0) {
    Engine_ActorStop(8);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(40);
    if ((s16)leader->facing >= 0) {
      Engine_ActorSetAnimation(8, 7);
    } else {
      Engine_ActorSetAnimation(8, 8);
    }
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgValeICantMoveGetHelp);
    Event_ShowMessage(8, 0);
    Actor_SetActionScript(8, gValeStorm_ActorEightScript);
    Engine_ActorSetAnimation(8, 6);
  } else {
    Engine_ActorStop(8);
    villager->scaleX = Q16_ONE;
    villager->scaleY = Q16_ONE;
    Actor_FaceDirection(8, FACING_EAST + FACING_STEP, 0);
    if ((s16)leader->facing >= 0) {
      Engine_ActorSetAnimation(8, 7);
    } else {
      Engine_ActorSetAnimation(8, 8);
    }
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgValeIllGo);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimation(8, ANIM_STAND);
    Engine_ActorJump(8, 4, 0);
    Engine_EventWait(80);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(40);
    if ((s16)leader->facing >= 0) {
      Engine_ActorSetAnimation(8, 7);
    } else {
      Engine_ActorSetAnimation(8, 8);
    }
    Engine_EventWait(2);
    Engine_ActorJump(8, 2, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Event_ShowMessage(8, 0);
    Actor_SetActionScript(8, gValeStorm_ActorEightScript);
    Engine_ActorSetAnimation(8, 6);
    GameFlag_Set(0x305);
  }
  Engine_EventEnd();
}

// Have Garet leave Isaac at the restored riverbank.
static void ValeStorm_GaretLeavesIsaac(void) {
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_ActorStop(ACTOR_GARET);
  Scheduler_RemoveCallback(ValeStorm_CopyLeaderSpritePriority);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 480, 1392);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_Get(ACTOR_GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetPosition(ACTOR_GARET, PIXELS(249), PIXELS(1240));
  Engine_TaskWait(1);
  Engine_EventSetMessage(MsgValeIsaac);
  Event_ShowMessage(0x1016, 0);
  Actor_SetPosition(ACTOR_GARET, PIXELS(172), PIXELS(1278));
  Engine_TaskWait(1);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(162), 0, PIXELS(1285), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(0x1016, 0, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_GARET, 2);
  Event_ShowMessageAndWait(0x1016, 0, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(ACTOR_GARET, 165, 1300);
  Actor_WalkToAndWait(ACTOR_GARET, 195, 1432);
  GameFlag_Set(FLAG_VALE_STORM_GARET_LEFT);
}

// Redraw the riverbank cells after the boulder has passed.
static void ValeStorm_RebuildRiverbank(void) {
  Map_CopyCellsTo(16, 96, 11, 73, 6, 3);
  Map_CopyCellsTo(16, 96, 34, 68, 14, 10);
  Map_CopyCellsTo(16, 96, 64, 68, 7, 7);
  Map_CopyCellsTo(9, 95, 11, 73, 6, 3);
  Map_CopyCellsTo(40, 94, 34, 68, 14, 10);
  Map_CopyCellsTo(54, 94, 64, 68, 8, 7);
  Map_CopyCellsTo(72, 75, 72, 76, 1, 1);
  Map_CopyCellsTo(72, 75, 74, 76, 1, 1);
  Map_CopyCellAttributes(7, 75, 1, 1, 6, 75);
  Map_CopyCellAttributes(8, 70, 3, 1, 8, 71);
  Map_CopyCellAttributes(8, 70, 2, 1, 9, 72);
  Map_CopyCellAttributes(8, 70, 2, 1, 9, 73);
  Map_CopyCellAttributes(11, 66, 1, 1, 8, 73);
  Map_CopyCellAttributes(12, 66, 1, 4, 11, 73);
  Map_CopyCellAttributes(25, 0, 1, 1, 6, 74);
  Engine_MapRedraw();
}

// On the stormy night, a monster attacks on the paths (encounter 2 of table
// 12).
static void Event_FightThirdMonster(void) {
  struct EventWork *state;

  Engine_EventBegin();
  state = gEventWork;
  state->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  state->transitionFrames = 64;
  Engine_GameFlagSet(FLAG_VALE_STORM_THIRD_MONSTER_FOUGHT);
  Field_RequestBattleFromEncounterTable(12, 2);
  Engine_GameFlagSet(FLAG_VALE_STORM_MONSTER_FOUGHT);
  Engine_EventEnd();
}

// On the stormy night, another monster attacks (encounter 3 of table 12).
static void Event_FightFourthMonster(void) {
  struct EventWork *state;

  Engine_EventBegin();
  state = gEventWork;
  state->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  state->transitionFrames = 64;
  Engine_GameFlagSet(FLAG_VALE_STORM_FOURTH_MONSTER_FOUGHT);
  Field_RequestBattleFromEncounterTable(12, 3);
  Engine_GameFlagSet(FLAG_VALE_STORM_MONSTER_FOUGHT);
  Engine_EventEnd();
}

// Flash the actor palette and emit paired effects at the shake interval.
static void ValeStorm_FlashPaletteAndSpawnEffects(struct FieldActor *actor) {
  s32 spawnInterval;

  if ((gFrameCount & 2) != 0) {
    Engine_ObjectSetPartPalettes(actor, 7);
  } else {
    Engine_ObjectSetPartPalettes(actor, 0);
  }
  {
    u32 *frame = &gFrameCount;

    spawnInterval = (sShakeShift << 3) + 16;
    if (Math_RemainderUnsigned(*frame, spawnInterval) == 0) {
      ValeStorm_SpawnEffectPair(actor);
    }
  }
}

// Cycle six palettes and emit paired effects at the shake interval.
static void ValeStorm_CyclePaletteAndSpawnEffects(struct FieldActor *actor) {
  s32 paletteFrame;
  s32 spawnInterval;

  if ((gFrameCount & 1) != 0) {
    paletteFrame = (gFrameCount >> 1);
    Engine_ObjectSetPartPalettes(actor,
                                 Math_RemainderUnsigned(paletteFrame, 6));
  }
  spawnInterval = (sShakeShift << 3) + 16;
  if (Math_RemainderUnsigned(gFrameCount, spawnInterval) == 0) {
    ValeStorm_SpawnEffectPair(actor);
  }
}

// Cycle six actor palettes on alternate frames.
static void ValeStorm_CycleObjectPalettes(struct FieldActor *actor) {
  s32 paletteFrame;

  if ((gFrameCount & 1) != 0) {
    paletteFrame = (gFrameCount >> 1);
    Engine_ObjectSetPartPalettes(actor,
                                 Math_RemainderUnsigned(paletteFrame, 6));
  }
}

// Shared source is included here in ROM order.
#include "common/effect_vertical.h"

// Create the two linked effects that travel in opposite vertical directions.
static void ValeStorm_SpawnEffectPair(struct FieldActor *parent) {
  struct FieldActor *pair[2];
  struct FieldActor *child;
  struct AnimationObject *sprite;
  struct PairWork *work = gEffectWork;
  s32 i;

  Engine_AudioPlayCue(SE_JUMP);
  for (i = 0; i < 2; ++i) {
    child = Engine_ObjectCreate(26, parent->x.fixed, parent->y.fixed,
                                parent->z.fixed);
    pair[i] = child;
    if (child != NULL) {
      child->terrainHeight = parent->terrainHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->linkedObject = parent;
      if (sprite != NULL) {
        Object_InitializeMode(sprite, 0);
        sprite->flags = 0;
        Resource_ResetEntry(sprite->slot);
        sprite->slot = work->vramBlock;
        sprite->retainCacheEntry = 1;
        sprite->image.tile =
            (gVramBlockCache[sprite->slot].offset >> 5) & 0x3ff;
        sprite->image.fullColor = 0;
        sprite->image.shape = 1;
        sprite->image.size = 2;
        sprite->entries[0]->frame = 0;
      }
    }
  }
  {
    struct FieldActor *effect = pair[0];
    struct AnimationObject *effectSprite = effect->sprite;

    effect->update = Effect_AnimateVerticalNegative;
    effectSprite->image.priority = parent->sprite->image.priority;
  }
  {
    struct FieldActor *effect = pair[1];
    struct AnimationObject *effectSprite = effect->sprite;

    effectSprite->image.priority = parent->sprite->image.priority;
    effect->update = Effect_AnimateVerticalPositive;
    effect->priorityFlags = 2;
  }
}

// Shared source is included here in ROM order.
#include "common/effect_psynergy.h"

// Update the four boulder scene actors until the final reaction stops.
static void ValeStorm_UpdateBoulderSceneEffects(void) {
  ValeStorm_CyclePaletteAndSpawnEffects(Object_GetById(32));
  ValeStorm_CyclePaletteAndSpawnEffects(Object_GetById(33));
  ValeStorm_CyclePaletteAndSpawnEffects(Object_GetById(30));
  if (sShakeDone == 0) {
    ValeStorm_CyclePaletteAndSpawnEffects(Object_GetById(29));
  }
}

// Cycle the boulder sprite palette.
static void ValeStorm_CycleBoulderPalette(void) {
  ValeStorm_CycleObjectPalettes(Object_GetById(19));
}

// Keep Garet and actor 8 at the leader sprite priority.
static void ValeStorm_CopyLeaderSpritePriority(void) {
  struct AnimationObject *sourceSprite;
  struct AnimationObject *targetSprite;
  struct AnimationObject *secondTargetSprite;

  sourceSprite = Object_GetById(0)->sprite;
#if TBS_PATCH_3
  Engine_ActorSetSpritePriority(22, sourceSprite->image.priority);
  Engine_ActorSetSpritePriority(8, sourceSprite->image.priority);
#else
  targetSprite = Object_GetById(22)->sprite;
  targetSprite->image.priority = sourceSprite->image.priority;
  secondTargetSprite = Object_GetById(8)->sprite;
  secondTargetSprite->image.priority = sourceSprite->image.priority;
#endif
}

// The door won't open: the impact knocked it off its hinges.
static void Event_SearchBrokenDoor(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgValeDoorWontOpen, 1);
  Engine_EventEnd();
}

// Searching the chest of valuables.
static void Event_SearchValuablesChest(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgValeSChestValuables, 1);
  Engine_EventEnd();
}

u8 gValeStorm_FrameModes[] = {7, 0, 0, 0};

s32 gValeStorm_ActorTwentyScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(185), 0, PIXELS(824)),
    SC_SYNC,   SC_MOVE(PIXELS(179), 0, PIXELS(836)),
    SC_SYNC,   SC_MOVE(PIXELS(226), 0, PIXELS(840)),
    SC_SYNC,   SC_MOVE(PIXELS(226), 0, PIXELS(944)),
    SC_SYNC,   SC_POS(0, 0, 0),
    SC_ARG(1), SC_END,
};

s32 gValeStorm_ActorTwentyTwoScriptA[] = {
    SC_ARG(2), SC_MOVE(PIXELS(345), 0, PIXELS(612)),
    SC_SYNC,   SC_MOVE(PIXELS(311), 0, PIXELS(638)),
    SC_SYNC,   SC_MOVE(PIXELS(256), 0, PIXELS(608)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeStorm_ActorTwentyTwoScriptB[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 192),
    SC_ADD(SW_SCALE_Y, 384),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, -384),
    SC_ADD(SW_SCALE_Y, -768),
    SC_WAIT(1),
    SC_LOOP(12, SC_TO(2)),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

s32 gValeStorm_ActorNineteenScript[] = {
    SC_ARG(2),
    SC_END,
};

s32 gValeStorm_LeaderScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(94), 0, PIXELS(564)), SC_SYNC,
    SC_ARG(1), SC_SET(SW_FACING, FACING_WEST),      SC_WAIT(30),
    SC_END,
};

s32 gValeStorm_ActorTwentyTwoScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(76), 0, PIXELS(564)), SC_SYNC,
    SC_ARG(1), SC_SET(SW_FACING, FACING_EAST),      SC_WAIT(30),
    SC_END,
};

s32 gValeStorm_ActorNineScriptA[] = {
    SC_ARG(2), SC_MOVE(PIXELS(178), 0, PIXELS(1190)),
    SC_SYNC,   SC_MOVE(PIXELS(149), 0, PIXELS(1194)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeStorm_ActorTwentySixScriptA[] = {
    SC_ARG(2), SC_MOVE(PIXELS(198), 0, PIXELS(1176)),
    SC_SYNC,   SC_MOVE(PIXELS(170), 0, PIXELS(1195)),
    SC_SYNC,   SC_MOVE(PIXELS(149), 0, PIXELS(1194)),
    SC_SYNC,   SC_MOVE(PIXELS(149), 0, PIXELS(1175)),
    SC_SYNC,   SC_POS(0, 0, 0),
    SC_ARG(1), SC_END,
};

s32 gValeStorm_ActorNineScriptB[] = {
    SC_ARG(2),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(30),
    SC_POS(PIXELS(102), 0, PIXELS(1189)),
    SC_WAIT(1),
    SC_MOVE(PIXELS(102), 0, PIXELS(1218)),
    SC_SYNC,
    SC_MOVE(PIXELS(84), 0, PIXELS(1228)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_END,
};

s32 gValeStorm_ActorTwentySixScriptB[] = {
    SC_ARG(2),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(30),
    SC_POS(PIXELS(102), 0, PIXELS(1189)),
    SC_SYNC,
    SC_MOVE(PIXELS(102), 0, PIXELS(1218)),
    SC_SYNC,
    SC_MOVE(PIXELS(120), 0, PIXELS(1228)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_END,
};

s32 gValeStorm_ActorNineScriptC[] = {
    SC_ARG(2), SC_MOVE(PIXELS(50), 0, PIXELS(1214)),
    SC_SYNC,   SC_MOVE(PIXELS(46), 0, PIXELS(1130)),
    SC_SYNC,   SC_MOVE(PIXELS(67), 0, PIXELS(1100)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeStorm_ActorTwentySixScriptC[] = {
    SC_ARG(2), SC_MOVE(PIXELS(153), 0, PIXELS(1251)),
    SC_SYNC,   SC_MOVE(PIXELS(177), 0, PIXELS(1357)),
    SC_SYNC,   SC_POS(0, 0, 0),
    SC_ARG(1), SC_END,
};

s32 gValeStorm_ActorNineScriptD[] = {
    SC_ARG(2), SC_MOVE(PIXELS(123), 0, PIXELS(1078)),
    SC_SYNC,   SC_MOVE(PIXELS(148), 0, PIXELS(1078)),
    SC_SYNC,   SC_MOVE(PIXELS(148), 0, PIXELS(1064)),
    SC_SYNC,   SC_MOVE(PIXELS(35), 0, PIXELS(1064)),
    SC_SYNC,   SC_POS(0, 0, 0),
    SC_ARG(1), SC_END,
};

s32 gValeStorm_StormRunActions[] = {
    SC_ARG(2), SC_MOVE(PIXELS(102), 0, PIXELS(1236)),
    SC_SYNC,   SC_MOVE(PIXELS(148), 0, PIXELS(1256)),
    SC_SYNC,   SC_MOVE(PIXELS(177), 0, PIXELS(1303)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeStorm_ArriveScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(137), 0, PIXELS(1379)),
    SC_SYNC,   SC_MOVE(PIXELS(114), 0, PIXELS(1416)),
    SC_SYNC,   SC_MOVE(PIXELS(95), 0, PIXELS(1416)),
    SC_SYNC,   SC_MOVE(PIXELS(83), 0, PIXELS(1427)),
    SC_SYNC,   SC_MOVE(PIXELS(154), 0, PIXELS(1430)),
    SC_SYNC,   SC_MOVE(PIXELS(179), 0, PIXELS(1398)),
    SC_SYNC,   SC_MOVE(PIXELS(179), 0, PIXELS(1361)),
    SC_SYNC,   SC_END,
};

s32 gValeStorm_GatherPath8[] = {
    SC_MOVE(PIXELS(184), 0, PIXELS(1309)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0xb000),
    SC_END,
};

s32 gValeStorm_GatherPath26[] = {
    SC_MOVE(PIXELS(165), 0, PIXELS(1306)), SC_SYNC, SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTHWEST),   SC_END,
};

s32 gValeStorm_GatherPath0[] = {
    SC_MOVE(PIXELS(183), 0, PIXELS(1325)), SC_SYNC, SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTHWEST),   SC_END,
};

s32 gValeStorm_GatherPath22[] = {
    SC_MOVE(PIXELS(166), 0, PIXELS(1325)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0xb000),
    SC_END,
};

s32 gValeStorm_LookAroundScript[] = {
    SC_CALL(ValeStorm_RerollHeadingOnCountdown),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValeStorm_ActorEightScript[] = {
    SC_CALL(SceneActor_UpdateScalePulse),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValeStorm_ActorNineteenRiseScript[] = {
    SC_ARG(2),
    SC_SET(SW_SPEED, PIXELS(3)),
    SC_SET(SW_ACCEL, PIXELS(1)),
    SC_SET(0x23, 0),
    SC_SET(0x24, 1),
    SC_SET(SW_SCALE_X, 2 * Q16_ONE),
    SC_SET(SW_SCALE_Y, 2 * Q16_ONE),
    SC_MOVE(PIXELS(256), PIXELS(128), PIXELS(931)),
    SC_SYNC,
    SC_SE(145),
    SC_SET(0x23, 60),
    SC_SET(0x24, 2),
    SC_MOVE(PIXELS(228), PIXELS(149), PIXELS(957)),
    SC_SYNC,
    SC_SET(0x24, 3),
    SC_MOVE(PIXELS(201), PIXELS(160), PIXELS(984)),
    SC_SYNC,
    SC_SET(0x24, 4),
    SC_MOVE(PIXELS(173), PIXELS(138), PIXELS(1011)),
    SC_SYNC,
    SC_SET(0x24, 5),
    SC_MOVE(PIXELS(146), PIXELS(96), PIXELS(1038)),
    SC_SYNC,
    SC_SE(145),
    SC_SET(SW_SPEED, PIXELS(3) / 2),
    SC_SET(SW_ACCEL, PIXELS(1) / 2),
    SC_SET(0x23, 60),
    SC_SET(0x24, 6),
    SC_MOVE(PIXELS(161), PIXELS(107), PIXELS(1050)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCEL, PIXELS(3) / 5),
    SC_SET(0x24, 7),
    SC_MOVE(PIXELS(177), PIXELS(96), PIXELS(1062)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(5) / 2),
    SC_SET(SW_ACCEL, PIXELS(4) / 5),
    SC_SET(0x24, 8),
    SC_MOVE(PIXELS(201), PIXELS(74), PIXELS(1074)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(3)),
    SC_SET(SW_ACCEL, PIXELS(1)),
    SC_SET(0x24, 9),
    SC_MOVE(PIXELS(225), -PIXELS(64), PIXELS(1086)),
    SC_SYNC,
    SC_SET(0x23, 0),
    SC_SET(0x24, 0),
    SC_END,
};

struct SceneEntrance gValeStorm_Entrances[] = {
    {0, CONDITION_ALWAYS, 167, 0, 1281, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 257, 0, 456, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 113, 0, 303, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 27, 0, 621, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 29, 0, 792, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 458, 0, 1393, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 406, 0, 743, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 262, 0, 821, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 340, 0, 904, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 326, 0, 1142, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 374, 0, 1254, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 102, 0, 1222, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 101, 0, 1216, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {13, CONDITION_ALWAYS, 146, 0, 1193, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {15, CONDITION_ALWAYS, 335, 0, 908, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {16, CONDITION_ALWAYS, 181, 0, 1273, FACING_NORTHEAST, 0, -1, -1, -1, -1,
     0},
    {SCENE_TABLE_END},
};

u32 gValeStorm_Exits[] = {
    SCENE_EXITS(3),        SCENE_EXIT(2, 5, 8),   SCENE_EXIT(3, 6, 1),
    SCENE_EXIT(4, 6, 2),   SCENE_EXIT(5, 7, 6),   SCENE_EXIT(6, 7, 5),
    SCENE_EXIT(7, 8, 6),   SCENE_EXIT(8, 7, 2),   SCENE_EXIT(9, 7, 1),
    SCENE_EXIT(10, 7, 1),  SCENE_EXIT(11, 7, 1),  SCENE_EXIT(12, 8, 11),
    SCENE_EXIT(13, 8, 12), SCENE_EXIT(20, 3, 16), SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gValeStorm_Placements[] = {
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x35, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(437), 0, PIXELS(762), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(322), 0, PIXELS(1433), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(144), 0, PIXELS(1070), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(276), 0, PIXELS(1270), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xd2, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(420), 0, PIXELS(619), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x13, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(396), 0, PIXELS(619), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x24, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x3b, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x3c, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x30, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x120, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeStorm_BoulderWatchers[] = {
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(124), 0, PIXELS(375), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(85), 0, PIXELS(361), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(135), 0, PIXELS(363), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(100), 0, PIXELS(375), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(124), 0, PIXELS(375), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(100), 0, PIXELS(375), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gValeStorm_Events[] = {
    {EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit1)},
    {EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit3)},
    {EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit4)},
    {EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit2)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveByExit5)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveByExit6)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveByExit7)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveByExit8)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveByExit9)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveByExit10)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveByExit12)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 13, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveByExit13)},
    {EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToGaret)},
    {EVENT_TALK, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_TalkAboutRescuingFelix)},
    {EVENT_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgValeHangOnFelixHelpIsOn)},
    {EVENT_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgValeDontLetGoFelix)},
    {EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgValeNoItCantBeKyleIt)},
    {EVENT_TALK, 26, CONDITION_ALWAYS,
     MSGNO(MsgValeMomDadFelixPleaseDontLeave)},
    {EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_VillagerAsksForHelp)},
    {EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved)},
    {EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved)},
    {EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved)},
    {EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved)},
    {EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved)},
    {EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved)},
    {EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved)},
    {EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved)},
    {EVENT_TOUCH, 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_SetPathCellOverride)},
    {EVENT_TOUCH, 14, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ClearPathCellOverride)},
    {EVENT_TOUCH, 41, CONDITION_ALWAYS, EVENT_SCRIPT(Event_Rock11Falls)},
    {EVENT_TOUCH, 42, CONDITION_ALWAYS, EVENT_SCRIPT(Event_Rock12Falls)},
    {EVENT_TOUCH, 43, CONDITION_ALWAYS, EVENT_SCRIPT(Event_Rock13Falls)},
    {EVENT_TOUCH, 44, CONDITION_ALWAYS, EVENT_SCRIPT(Event_Rock14Falls)},
    {EVENT_TOUCH, 30, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_HearGaretStruggling)},
    {EVENT_TOUCH, 31, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_GaretCallsAfterIsaac)},
    {EVENT_TOUCH, 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_GaretJoinsIsaac)},
    {EVENT_TOUCH, 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WarnAboutMtAleph)},
    {EVENT_TOUCH, 34, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WatchBoulderFall)},
    {EVENT_TOUCH, 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WitnessRiverRescue)},
    {EVENT_TOUCH, 36, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_GatherAfterRiverRescue)},
    {EVENT_TOUCH, 16, FLAG_VALE_STORM_THIRD_MONSTER_FOUGHT,
     EVENT_SCRIPT(Event_FightThirdMonster)},
    {EVENT_TOUCH, 17, FLAG_VALE_STORM_FOURTH_MONSTER_FOUGHT,
     EVENT_SCRIPT(Event_FightFourthMonster)},
    {SEARCH(SEARCH_UNNAMED), 24, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_SearchBrokenDoor)},
    {SEARCH(SEARCH_UNNAMED), 19, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_SearchValuablesChest)},
    {SCENE_EVENTS_END},
};

u16 gValeStorm_CellSteps0[] = {
    MAP_CELL_STEP(0, 98, 2, 2, 2),
    MAP_CELL_STEP(2, 98, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

u16 gValeStorm_CellSteps1[] = {
    MAP_CELL_STEP(0, 96, 2, 2, 2),
    MAP_CELL_STEP(2, 96, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

u16 gValeStorm_CellSteps2[] = {
    MAP_CELL_STEP(4, 98, 2, 2, 2),
    MAP_CELL_STEP(6, 98, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

u16 gValeStorm_CellSteps3[] = {
    MAP_CELL_STEP(4, 96, 2, 2, 2),
    MAP_CELL_STEP(6, 96, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

u16 gValeStorm_CellSteps4[] = {
    MAP_CELL_STEP(0, 96, 2, 2, 2),
    MAP_CELL_STEP(50, 44, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

u16 gValeStorm_CellSteps5[] = {
    MAP_CELL_STEP(4, 98, 2, 2, 2),
    MAP_CELL_STEP(44, 108, 2, 2, 2),
    MAP_CELL_STEPS_END,
};
