#include "script.h"
#include "game_state.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/scene/venus_aerie.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/scene/common/object_nearest.h"
#include "unit/scene/common/effect_psynergy.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/runtime/runtime.h"

#define NULL ((void *)0)
#include "field_obj.h"
#include "types.h"
#include "animspr.h"
#include "venus_aerie.h"
#include "field_effect.h"
#include "event.h"
#include "field_event.h"
#include "field_scene.h"
#include "sound_ids.h"
#include "create_configured_overlay_object.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "iwram_call.h"
#include "vram_block.h"
#include "callback_scheduler.h"
#include "call.h"
#include "field_service.h"
#include "b5_context.h"
#include "text/msg_ids.h"
#include "global_cells.h"
#include "scene_ids.h"
#include "staged_actor.h"
#include "map_scroll.h"
#include "tbs_edition.h"
#include "io_reg.h"
#include "window.h"
#include "heap_state.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "fx_scene.h"
#include "palbuf.h"

enum { ITEM_SHAMANS_ROD = 65, ITEM_VENUS_STAR = 220 };

// Forward declarations.
static void VenusAerie_EnablePillarGravity(void);
static void VenusAerie_EnterAndConfrontAntagonists(void);
static void VenusAerie_ShowPartyMember(s32 actor, s32 shown);
static void VenusAerie_FlashActorPalette(struct FieldActor *actor);
static void VenusAerie_SpawnLinkedPairEffects(struct FieldActor *parent);
static void VenusAerie_UpdateShebaPalette(void);
static void VenusAerie_CombineSaturosAndMenardi(void);
static void VenusAerie_FlashScreen(void);
static void VenusAerie_SpawnUpperOrbitParticles(void);
static void VenusAerie_UpdateBeamActors(void);
static void VenusAerie_FinishSaturosAndMenardiBattle(void);
static void VenusAerie_PlayFallWarningExit(void);
static void VenusAerie_RunLeapEntrance(void);
static void VenusAerie_ShowSaturosAndMenardiFall(void);
static void VenusAerie_DiscussSaturosAndMenardiDefeat(void);
static void VenusAerie_EruptBeacon(void);
static void VenusAerie_RestorePartyOnBrokenAerie(void);
static void VenusAerie_TryToPreventFall(void);
static void VenusAerie_PlayShebasFall(void);
static void VenusAerie_StopActorMotion(struct FieldActor *actor);
static void VenusAerie_UpdateBeaconRise(void);
static void VenusAerie_AdvanceActorRise(struct FieldActor *actor);
static void VenusAerie_SpawnSideOrbitParticles(void);
static void VenusAerie_UpdateOrbitingEffect(union FieldObject *object);

s32 Math_RemainderUnsigned(s32, s32);

// Shared source is included here in ROM order.
#include "common/effect_create.h"
#include "common/effect_spawn.h"

// Pulse the actor scale around the countdown, then choose its next pause.
static s32 VenusAerie_PulseActorScale(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  s16 *counter = &actor->timer;

  switch (*counter) {
  case 6:
    actor->scaleX += -Q16_ONE / 8;
    actor->scaleY += Q16_ONE / 16;
    break;
  case 4:
    actor->scaleX += Q16_ONE / 16;
    actor->scaleY += -Q16_ONE / 32;
    break;
  case 2:
    actor->scaleX += Q16_ONE / 32;
    actor->scaleY += -Q16_ONE / 64;
    break;
  case 0:
    actor->scaleX += Q16_ONE / 32;
    actor->scaleY += -Q16_ONE / 64;
    *counter = Math_RemainderUnsigned(Engine_RandomNext(), 80) + 80;
    break;
  }
  {
    s32 t = (u16)actor->timer;
    t--;
    actor->timer = t;
  }
  return 1;
}

// Move a particle, damp its horizontal velocity and update its scale and spin.
static void VenusAerie_UpdateDriftingParticle(union FieldObject *object) {
  s32 velocityX = object->effect.velocity.x;
  s32 velocityZ;

  object->effect.x += velocityX;
  object->effect.y += object->effect.velocity.y;
  velocityZ = object->effect.velocity.z;
  object->effect.z += velocityZ;
  object->effect.velocity.x = velocityX - velocityX / 22;
  object->effect.velocity.z = velocityZ - velocityZ / 20;
  object->effect.scaleX += object->effect.scaleRateX;
  object->effect.scaleY += object->effect.scaleRateY;
  object->effect.sprite->rotation += object->effect.spin;
}

// Turn the falling actor sprite until its rotation leaves the negative
// half-turn.
static void VenusAerie_StepSpriteRotation(union FieldObject *fieldObject) {
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct AnimationObject *sprite = actor->sprite;
  u16 rotation = sprite->rotation;

  if ((s16)(rotation - 1) < 0) {
    {
      s32 nextRotation = rotation - 0xa00;
      sprite->rotation = nextRotation;
    }
  }
}

// Shared source is included here in ROM order.
#include "common/object_heading.h"

// Return the aerie arrival positions.
const struct SceneEntrance *VenusAerie_GetEntrances(void) {
  return gVenusAerieEntrances;
}

// The aerie has no separate trigger-region table.
const struct SceneRegion *VenusAerie_GetRegions(void) { return 0; }

// Return the aerie scene destinations.
const u32 *VenusAerie_GetExits(void) { return gVenusAerieExits; }

// Return the platforms, antagonists, companions and beacon effects.
const struct ScenePlacement *VenusAerie_GetPlacements(void) {
  return gVenusAeriePlacements;
}

// Shared source is included here in ROM order.
#include "common/object_distance.h"
#include "common/object_nearest.h"

// Jump the leader to the next clear tile and restore the prior motion flags.
static void VenusAerie_TryLeaderJump(void) {
  struct FieldActor *actor;
  union {
    struct FieldPosition position;
    s32 coordinates[3];
  } destination;
  s32 angle;
  u8 flags;

  actor = Object_GetById(ACTOR_PARTY_LEADER);
  angle = (actor->facing + 0x1000) & 0xe000;
  flags = actor->motionFlags;
  destination.coordinates[0] = (actor->x.fixed & 0xfff00000) + PIXELS(8);
  destination.coordinates[1] = actor->y.fixed;
  destination.coordinates[2] = (actor->z.fixed & 0xfff00000) + PIXELS(8);
  Vector_AddPolarOffset(PIXELS(32), angle, destination.coordinates);
  if (FieldObject_CheckMovementCollision(actor, &destination.position) == 0) {
    Engine_GameFlagClear(592);
    VenusAerie_EnablePillarGravity();
    Object_SetMode(actor, 6);
    Engine_TaskWait(6);
    Object_SetMode(actor, 7);
    actor->speed = PIXELS(3);
    actor->acceleration = PIXELS(2);
    Engine_AudioPlayCue(152);
    actor->velocityY = PIXELS(4);
    actor->motionFlags &= 0x7e;
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER,
                              destination.coordinates[0] >> 16,
                              destination.coordinates[2] >> 16);
    Object_SetMode(actor, 6);
    Engine_ActorSetSpriteFlags(actor, 1);
    actor->motionFlags = flags;
  }
}

// Dip the nearest pillar with the leader, then rebound to its original height.
static void VenusAerie_BouncePillarUnderLeader(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldActor *pillar;
  s32 pillarId;
  s32 reboundStep;

  Engine_EventBegin();
  pillarId = SceneActor_FindNearestSlotOfKindF2();
  gVenusAerie_NearestActor = pillarId;
  if (pillarId != 0) {
    Engine_GameFlagSet(592);
    pillar = Object_GetById(gVenusAerie_NearestActor);
    pillar->motionFlags = 0;
    leader->motionFlags &= 0xfe;
    pillar->y.fixed += -PIXELS(3);
    leader->y.fixed += -PIXELS(3);
    leader->terrainHeight += -PIXELS(3);
    Engine_TaskWait(2);
    pillar->y.fixed += -PIXELS(2);
    leader->y.fixed += -PIXELS(2);
    leader->terrainHeight += -PIXELS(2);
    Engine_TaskWait(10);
    reboundStep = PIXELS(2);
    pillar->y.fixed += reboundStep;
    leader->y.fixed += reboundStep;
    leader->terrainHeight += reboundStep;
    Engine_TaskWait(4);
    pillar->y.fixed += reboundStep;
    leader->y.fixed += reboundStep;
    leader->terrainHeight += reboundStep;
    Engine_TaskWait(4);
    pillar->y.fixed += PIXELS(1);
    leader->y.fixed += PIXELS(1);
    leader->terrainHeight += PIXELS(1);
  }
  Engine_EventEnd();
}

// Let the leader and the six outer pillars fall with the terrain.
static void VenusAerie_EnablePillarGravity(void) {
  s32 flags;

  Object_GetById(0)->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  flags = 4;
  Object_GetById(12)->motionFlags = flags;
  Object_GetById(13)->motionFlags = flags;
  Object_GetById(14)->motionFlags = flags;
  Object_GetById(15)->motionFlags = flags;
  Object_GetById(16)->motionFlags = flags;
  Object_GetById(17)->motionFlags = flags;
}

// Return the pillar and confrontation triggers.
const struct SceneEvent *VenusAerie_GetEvents(void) {
  return gVenusAerieEvents;
}

// Restore the platforms and run the scene selected by the arrival.
s32 VenusAerie_Initialize(void) {
  s32 mirroredScale;
  u32 platformId;
  s32 clear;
  s16 entrance;
  struct FieldActor *platform;

  Engine_GameFlagSet(324);
  Engine_TaskWait(1);
  Engine_GameFlagSet(272);
  Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
  mirroredScale = -Q16_ONE;
  Object_GetById(10)->scaleX = mirroredScale;
  Object_GetById(11)->scaleX = mirroredScale;
  platformId = 12;
  clear = 0;
  do {
    platform = Object_GetById(platformId);
    Engine_ActorSetSpriteFlags(Object_GetById(platformId), clear);
    Engine_ActorSetSpritePriority(platformId, 1);
    platform->motionFlags = 4;
    platform->priorityFlags |= 2;
    platform->y.fixed = PIXELS(1) / 2;
    platformId++;
  } while (platformId <= 17);

  entrance = gGameState.entrance;

  switch (entrance) {
  case 1:
    if (Engine_GameFlagIsSet(0x109) == 0) {
      VenusAerie_EnterAndConfrontAntagonists();
    }
    break;
  case 2:
    VenusAerie_CombineSaturosAndMenardi();
    break;
  case 3:
    VenusAerie_FinishSaturosAndMenardiBattle();
    break;
  case 0x5d:
    VenusAerie_PlayFallWarningExit();
    break;
  case 4:
    VenusAerie_RunLeapEntrance();
    break;
  case 9:
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x345) != 0) {
      Inventory_AddItem(0, ITEM_SHAMANS_ROD);
    } else if (Engine_GameFlagIsSet(0x346) != 0) {
      Inventory_AddItem(1, ITEM_SHAMANS_ROD);
    } else if (Engine_GameFlagIsSet(0x347) != 0) {
      Inventory_AddItem(2, ITEM_SHAMANS_ROD);
    } else {
      Inventory_AddItem(3, ITEM_SHAMANS_ROD);
    }
    Engine_EventRequestExit(9);
    break;
  }

  if (Engine_GameFlagIsSet(0x109) != 0) {
    s32 slot = SceneActor_FindNearestSlotOfKindF2();

    if (slot != 0) {
      struct FieldActor *nearestPillar = Object_GetById(slot);
      if (nearestPillar != 0) {
        nearestPillar->motionFlags = 0;
      }
    }
  }
  return clear;
}

// Show the selected speaker message, then pause for ten frames.
static void VenusAerie_ShowMessage(s32 speaker) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(10);
}

// Turn one actor and wait ten frames.
static void VenusAerie_FaceActor(s32 actor, s32 facing) {
  Engine_ActorFaceDirection(actor, facing, 10);
}

// Bring the party onto the aerie and reveal the argument over Sheba.
static void VenusAerie_EnterAndConfrontAntagonists(void) {
  struct FieldActor *arrivalPlatform;
  struct FieldActor *leader;

  // Bring the leader across on the moving platform, then reveal the party.
  arrivalPlatform = Object_GetById(18);
  Engine_EventBegin();
  VenusAerie_ShowPartyMember(1, 0);
  VenusAerie_ShowPartyMember(2, 0);
  VenusAerie_ShowPartyMember(3, 0);
  Engine_CameraMoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  arrivalPlatform->motionFlags = 0;
  arrivalPlatform->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
  Engine_ActorSetSpritePriority(18, 1);
  Engine_ActorSetPosition(18, PIXELS(580), PIXELS(338));
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  leader->motionFlags = 0;
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetPosition(ACTOR_PARTY_LEADER, PIXELS(581), PIXELS(288));
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Actor_SetSpeed(18, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(18, 580, 221);
  Actor_MoveToAndWait(0, 581, 171);
  Actor_SetDestination(18, 530, 211);
  Actor_MoveToAndWait(0, 531, 161);
  Actor_SetDestination(18, 520, 191);
  Engine_ActorMoveToAndWait(0, 521, 141);
  Engine_ActorSetSpriteFlags(Object_GetById(18), 1);
  Actor_SetDestination(18, 515, 171);
  Engine_ActorMoveToAndWait(0, 516, 121);
  Engine_AudioPlayCue(0x120);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 6);
  ObjectMotion_WaitForAnimationChange(0);
  leader->x.fixed = PIXELS(516);
  leader->y.fixed = PIXELS(8);
  leader->z.fixed = PIXELS(148);
  leader->facing = FACING_WEST;
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Engine_AudioPlayCue(152);
  leader->velocityY = PIXELS(4);
  Engine_AudioPlayCue(152);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 1);
  Engine_ActorMoveToAndWait(0, 504, 148);
  Engine_EventWait(10);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  {
    u16 shown = FACING_SOUTH;

    leader->y.fixed = -PIXELS(32);
    leader->facing = shown;
  }
  Engine_EventWait(20);
  Engine_AudioPlayCue(0x134);
  Engine_ActorMoveToAndWait(18, 524, 191);
  Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
  Actor_MoveToAndWait(18, 530, 211);
  Actor_MoveToAndWait(18, 580, 221);
  Engine_ActorSetDestination(18, 580, 338);
  Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  VenusAerie_ShowPartyMember(1, 1);
  VenusAerie_ShowPartyMember(2, 1);
  VenusAerie_ShowPartyMember(3, 1);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 492, 164);
  Engine_ActorWalkTo(1, 514, 164);
  Actor_WalkTo(IVAN, 492, 140);
  Engine_ActorWalkToAndWait(3, 514, 140);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetAnimation(GARET, 1);
  Engine_ActorSetAnimation(IVAN, 1);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Engine_ActorFaceDirection(GARET, FACING_SOUTH, 0);
  Engine_ActorFaceDirection(IVAN, FACING_SOUTH, 0);
  Engine_ActorFaceDirection(MIA, FACING_SOUTH, 0);
  Engine_ActorWaitForMove(18);
  Engine_ActorSetPosition(18, 0, 0);
  Engine_AudioPlayCue(0x121);
  // Reveal Saturos and Menardi arguing with Felix about Sheba.
  Engine_EventSetMessage(MsgVenusLongLastTime);
  VenusAerie_ShowMessage(0x4013);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Engine_ActorFaceDirection(GARET, FACING_WEST, 0);
  Engine_ActorFaceDirection(IVAN, FACING_WEST, 0);
  VenusAerie_FaceActor(MIA, FACING_WEST);
  Engine_EventGetViewCenter()->motionFlags = 0;
  Engine_CameraSetSpeed(PIXELS(24) / 5, PIXELS(3) / 5);
  Engine_CameraMoveTo(PIXELS(304), PIXELS(32), PIXELS(158), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  VenusAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH + FACING_STEP);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  Engine_AudioPlayCue(61);
  VenusAerie_ShowMessage(0x2014);
  Engine_ActorSetAnimation(ACTOR_MENARDI, 4);
  VenusAerie_ShowMessage(0x4013);
  VenusAerie_FaceActor(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 5, 40);
  VenusAerie_ShowMessage(0x2014);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT, 0);
  Engine_ActorShowEmote(20, EMOTE_IN_FRONT, 20);
  Actor_FaceDirection(SHEBA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 20);
  Engine_CameraSetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Camera_MoveTo(PIXELS(294), -1, PIXELS(194), 1);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_FELIX, 272, 200);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  Engine_EventWait(20);
  VenusAerie_ShowMessage(0x2014);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | 3, 20);
  VenusAerie_ShowMessage(ACTOR_MENARDI);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 3);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 1, 40);
  VenusAerie_ShowMessage(0x2014);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 4);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | 1, 60);
  Engine_EventShowMessageAndWait(19, 0, 40);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | 6, 40);
  VenusAerie_FaceActor(ACTOR_MENARDI, FACING_WEST);
  VenusAerie_ShowMessage(0x2013);
  Engine_ActorFaceDirection(SHEBA, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 3, 40);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  VenusAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTH + FACING_STEP);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | 8, 20);
  VenusAerie_ShowMessage(0x2013);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 3, 20);
  Engine_EventShowMessageAndWait(21, 0, 20);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_WEST, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 4);
  Engine_EventShowMessageAndWait(0x2014, 0, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 3);
  Engine_EventShowMessageAndWait(21, 0, 20);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 2);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | 5, 80);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 2);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | 1, 60);
  VenusAerie_ShowMessage(0x2013);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 3);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Engine_ActorStartRepeatedMotion(ACTOR_MENARDI, 1);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 4);
  Engine_EventShowMessageAndWait(21, 0, 20);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 5, 60);
  Engine_EventShowMessageAndWait(0x2014, 0, 20);
  VenusAerie_FaceActor(ACTOR_FELIX, FACING_NORTHWEST + FACING_STEP);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  VenusAerie_FaceActor(SHEBA, FACING_SOUTHEAST + FACING_STEP);
  Engine_ActorRunRepeatedMotion(SHEBA, 2);
  Engine_EventWait(20);
  VenusAerie_FaceActor(ACTOR_FELIX, FACING_NORTH + FACING_STEP);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 4);
  VenusAerie_ShowMessage(0x2013);
  Engine_ActorFaceDirection(SHEBA, FACING_EAST, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
  VenusAerie_ShowMessage(0x2014);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 4);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | 1, 80);
  Engine_ActorFaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_WEST, 0);
  Engine_CameraSetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(294), -1, PIXELS(180), 1);
  Actor_WalkToAndWait(ACTOR_FELIX, 262, 176);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_WEST, 40);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_EAST, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 2);
  VenusAerie_ShowMessage(0x8015);
  Engine_ActorShowEmote(19, EMOTE_IN_FRONT, 20);
  VenusAerie_ShowMessage(0x2013);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 2);
  VenusAerie_ShowMessage(0x8015);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 3, 40);
  Event_ShowMessageAndWait(0xa014, 0, 20);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 5, 20);
  VenusAerie_ShowMessage(0x8015);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | 3, 20);
  VenusAerie_ShowMessage(0x2013);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 1, 40);
  VenusAerie_ShowMessage(0x8015);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 4);
  VenusAerie_ShowMessage(0xa014);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
  VenusAerie_ShowMessage(0x2013);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 3, 60);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_WEST, 20);
  Engine_EventShowMessageAndWait(0xa015, 0, 40);
  Actor_ShowEmote(SHEBA, EMOTE_IN_FRONT | 5, 120);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 5, 60);
  VenusAerie_ShowMessage(0xa014);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_EAST, 40);
  Engine_ActorSetAnimation(ACTOR_MENARDI, 3);
  VenusAerie_ShowMessage(0x2013);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Engine_EventShowMessageAndWait(0x8015, 0, 20);
  VenusAerie_ShowMessage(0xa014);
  Engine_ActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT, 40);
  Engine_ActorSetAnimation(ACTOR_MENARDI, 4);
  VenusAerie_ShowMessage(0x2013);
  Actor_ShowEmote(SHEBA, EMOTE_IN_FRONT | 5, 40);
  Engine_ActorShowEmote(20, EMOTE_IN_FRONT | 8, 40);
  VenusAerie_ShowMessage(0xa014);
  Engine_ActorShowEmote(19, EMOTE_IN_FRONT | 3, 20);
  VenusAerie_ShowMessage(0x2013);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(ACTOR_SATUROS, 2);
  VenusAerie_ShowMessage(0xa014);
  Engine_ActorSetAnimation(ACTOR_MENARDI, 4);
  VenusAerie_ShowMessage(0x2013);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_CameraMoveTo(PIXELS(504), -PIXELS(24), PIXELS(168), 0);
  Engine_TaskWait(1);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  VenusAerie_ShowMessage(0x8001);
  Engine_ActorShowEmote(3, EMOTE_IN_FRONT | 1, 40);
  VenusAerie_ShowMessage(MIA);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_EventShowMessageAndWait(0x1002, 0, 40);
  Engine_ActorJump(GARET, 2, 20);
  VenusAerie_ShowMessage(0x8001);
  Engine_EventOpenMessage(0x8001, 0);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Engine_ActorFaceDirection(IVAN, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message += 1;
  }
  Engine_EventWait(20);
  VenusAerie_ShowMessage(GARET);
  Engine_ActorFaceDirection(IVAN, FACING_EAST, 0);
  Engine_ActorFaceDirection(MIA, FACING_WEST, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Actor_SetActionScript(GARET, gVenusAerie_PairActionScript);
  Actor_SetActionScript(IVAN, gVenusAerie_PairActionScript);
  Actor_RunActionScriptAndWait(MIA, gVenusAerie_PairActionScript);
  Engine_EventWait(20);
  Engine_EventEnd();
}

// Show or hide one companion, setting its movement speed when shown.
static void VenusAerie_ShowPartyMember(s32 actorId, s32 shown) {
  struct FieldActor *member;

  if (shown != 0) {
    Actor_SetChildValue(actorId, 0);
    member = Actor_Get(actorId);
    Engine_ActorSetSpriteFlags(member, 1);
    Actor_SetSpeed(actorId, PIXELS(4) / 5, PIXELS(2) / 5);
  } else {
    Actor_SetChildValue(actorId, 15);
    member = Actor_Get(actorId);
    Engine_ActorSetSpriteFlags(member, 0);
  }
}

// Resolve the hostage argument and Shaman's Rod exchange, then start battle.
static void VenusAerie_ConfrontSaturosAndMenardi(void) {
  s32 rodOwner;
  struct FieldActor *object;
  s32 answerAccepted;
  s32 frame;
  s32 receiptMessage;

  Engine_EventBegin();
  Engine_EventGetViewCenter()->motionFlags = 0;
  Camera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Camera_MoveTo(PIXELS(332), PIXELS(32), PIXELS(180), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 340, 184);
  VenusAerie_FaceActor(ACTOR_PARTY_LEADER, FACING_WEST);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Engine_EventSetMessage(MsgVenusIsaac);
  VenusAerie_ShowMessage(0x9015);
  Engine_EventGetViewCenter()->motionFlags = 0;
  Camera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Camera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 1);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(MIA, PIXELS(7) / 5, PIXELS(7) / 10);
  object = Object_GetById(ACTOR_PARTY_LEADER);
  if (object != 0) {
    Actor_SetPosition(GARET, object->x.fixed, object->z.fixed);
  }
  object = Object_GetById(ACTOR_PARTY_LEADER);
  if (object != 0) {
    Actor_SetPosition(IVAN, object->x.fixed, object->z.fixed);
  }
  object = Object_GetById(ACTOR_PARTY_LEADER);
  if (object != 0) {
    Actor_SetPosition(MIA, object->x.fixed, object->z.fixed);
  }
  Actor_WalkTo(GARET, 328, 168);
  Actor_WalkTo(IVAN, 340, 196);
  Actor_WalkToAndWait(MIA, 326, 204);
  Engine_ActorSetAnimation(GARET, 1);
  Engine_ActorSetAnimation(IVAN, 1);
  Engine_ActorFaceDirection(GARET, FACING_WEST, 0);
  Engine_ActorFaceDirection(IVAN, FACING_WEST, 0);
  VenusAerie_FaceActor(MIA, FACING_WEST);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_EAST, 0);
  Engine_ActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
  VenusAerie_ShowMessage(ACTOR_SATUROS);
  VenusAerie_FaceActor(ACTOR_MENARDI, FACING_WEST);
  VenusAerie_ShowMessage(0x2013);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 3, 20);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 2);
  Event_ShowMessageAndWait(ACTOR_FELIX, 0, 20);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_NORTH + FACING_STEP, 40);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 4);
  VenusAerie_FaceActor(ACTOR_FELIX, FACING_EAST);
  Event_ShowMessageAndWait(ACTOR_FELIX, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  VenusAerie_FaceActor(ACTOR_SATUROS, FACING_WEST);
  VenusAerie_ShowMessage(0x2014);
  Actor_SetAttachedEffect(MIA, EMOTE_IN_FRONT | 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  VenusAerie_ShowMessage(MIA);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  VenusAerie_ShowMessage(IVAN);
  VenusAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  VenusAerie_ShowMessage(ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 6, 40);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_EAST, 20);
  VenusAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  VenusAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | 2, 40);
  VenusAerie_ShowMessage(ACTOR_MENARDI);
  VenusAerie_FaceActor(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP);
  VenusAerie_ShowMessage(0x2014);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_WEST, 20);
  VenusAerie_ShowMessage(0x2014);
  Engine_ActorFaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Engine_ActorFaceDirection(GARET, FACING_WEST, 0);
  Engine_ActorFaceDirection(IVAN, FACING_WEST, 0);
  Engine_ActorFaceDirection(MIA, FACING_WEST, 0);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_EAST, 20);
  Actor_ShowEmote(SHEBA, EMOTE_IN_FRONT | 1, 40);
  VenusAerie_ShowMessage(SHEBA);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 3, 20);
  VenusAerie_ShowMessage(0x2014);
  Engine_ActorRunRepeatedMotion(SHEBA, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(SHEBA, 3);
  VenusAerie_ShowMessage(SHEBA);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
  VenusAerie_ShowMessage(0x2014);
  Actor_SetSpeed(SHEBA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(SHEBA, 260, 186);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WalkToAndWait(SHEBA, 276, 192);
  object = Object_GetById(ACTOR_MENARDI);
  object->facing = FACING_SOUTH + FACING_STEP;
  Engine_TaskWait(1);
  Engine_ActorStartRepeatedMotion(ACTOR_MENARDI, 2);
  VenusAerie_ShowMessage(0x2013);
  Engine_ActorJump(SHEBA, 2, 20);
  Actor_SetSpeed(SHEBA, PIXELS(12) / 5, PIXELS(6) / 5);
  Actor_WalkToAndWait(SHEBA, 260, 186);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_WalkToAndWait(SHEBA, 248, 172);
  Engine_ActorFaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Engine_ActorFaceDirection(SHEBA, FACING_EAST, 20);
  Engine_ActorSetAnimationAndWait(SHEBA, 3);
  Engine_EventWait(40);
  SceneState_SetValue140Mode0();
  Engine_TaskWait(1);
  frame = 0;
  do {
    object = Object_GetById(SHEBA);
    VenusAerie_FlashActorPalette(object);
    frame = (frame + 1);
    Engine_TaskWait(1);
  } while ((u32)frame <= 39);
  FieldScheduler_AddCallback(VenusAerie_UpdateShebaPalette,
                             TASK_PRIORITY_SCENE);
  Engine_EventWait(80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 0);
  VenusAerie_FaceActor(ACTOR_FELIX, FACING_WEST);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessageAndWait(0x2014, 0, 20);
  Actor_ShowEmote(SHEBA, EMOTE_IN_FRONT | 5, 80);
  Engine_ActorStartRepeatedMotion(ACTOR_MENARDI, 2);
  VenusAerie_ShowMessage(0x2013);
  Scheduler_RemoveCallback(VenusAerie_UpdateShebaPalette);
  Engine_TaskWait(1);
  Actor_SetChildValue(SHEBA, 0);
  Engine_TaskWait(10);
  SceneEffect_CleanupSceneObjects();
  Engine_ActorJump(SHEBA, 2, 40);
  VenusAerie_ShowMessage(SHEBA);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 3, 20);
  VenusAerie_ShowMessage(0x2014);
  Engine_ActorRunRepeatedMotion(SHEBA, 2);
  Object_GetById(SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(SHEBA, 250, 176);
  Engine_EventWait(1);
  Object_GetById(SHEBA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 3, 20);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_EAST, 20);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 4);
  VenusAerie_ShowMessage(0x2013);
  Engine_ActorRunRepeatedMotion(SHEBA, 2);
  Engine_EventWait(40);
  Actor_SetSpeed(SHEBA, PIXELS(3) / 5, PIXELS(3) / 10);
  Object_GetById(SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(SHEBA, 248, 172);
  Engine_EventWait(1);
  Object_GetById(SHEBA)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(SHEBA, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
  VenusAerie_ShowMessage(0x2013);
  Engine_ActorSetAnimationAndWait(SHEBA, 3);
  VenusAerie_ShowMessage(SHEBA);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 5, 60);
  Engine_ActorSetAnimation(ACTOR_SATUROS, 4);
  VenusAerie_ShowMessage(0x2014);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | 1, 40);
  VenusAerie_ShowMessage(ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 5, 100);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  Engine_EventWait(20);
  VenusAerie_FaceActor(ACTOR_SATUROS, FACING_EAST);
  VenusAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  Event_OpenMessage(ACTOR_SATUROS, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
    answerAccepted = 1;
  } else {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 4);
    gEventWork->message += 1;
    answerAccepted = 0;
  }
  VenusAerie_ShowMessage(ACTOR_SATUROS);
  if (answerAccepted != 0) {
    gEventWork->message += 1;
  }
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 20);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 8, 40);
  Event_OpenMessage(ACTOR_SATUROS, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorSetAnimation(IVAN, 3);
    answerAccepted = 1;
  } else {
    Engine_EventWait(20);
    Engine_ActorSetAnimation(IVAN, 4);
    gEventWork->message += 1;
    answerAccepted = 0;
  }
  VenusAerie_ShowMessage(IVAN);
  if (answerAccepted != 0) {
    gEventWork->message += 1;
  }
  Engine_ActorRunRepeatedMotion(GARET, 2);
  VenusAerie_FaceActor(GARET, FACING_SOUTH);
  VenusAerie_ShowMessage(GARET);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  VenusAerie_ShowMessage(ACTOR_MENARDI);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  VenusAerie_FaceActor(MIA, FACING_WEST);
  Engine_ActorSetAnimationAndWait(GARET, 4);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  VenusAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP);
  Engine_ActorSetAnimation(ACTOR_MENARDI, 3);
  VenusAerie_ShowMessage(ACTOR_MENARDI);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
  Engine_EventWait(40);
  VenusAerie_FaceActor(ACTOR_SATUROS, FACING_WEST);
  VenusAerie_ShowMessage(0x2014);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_FELIX, 0, 40);
  VenusAerie_ShowMessage(0x2014);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 3, 60);
  VenusAerie_FaceActor(ACTOR_MENARDI, FACING_WEST);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  VenusAerie_ShowMessage(0x2013);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 5, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 3);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_FELIX, 288, 192);
  Engine_ActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 0);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_EAST, 0);
  Actor_WalkToAndWait(ACTOR_FELIX, 310, 192);
  Actor_WalkToAndWait(ACTOR_FELIX, 328, 186);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 2);

  // Complete the Shaman's Rod exchange before the confrontation resumes.
  receiptMessage = MsgVenusIsaacHandedOverShamansRod;
  Engine_MessageShowCentered(receiptMessage, 1);
  Actor_WalkToAndWait(ACTOR_FELIX, 310, 192);
  Engine_ActorFaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_WEST, 0);
  Actor_WalkToAndWait(ACTOR_FELIX, 288, 192);
  Actor_WalkToAndWait(ACTOR_FELIX, 262, 176);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_EAST, 40);
  Engine_EventSetMessage((receiptMessage + 1));
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
  VenusAerie_ShowMessage(0x2014);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 3);
  VenusAerie_FaceActor(ACTOR_SATUROS, FACING_EAST);
  VenusAerie_FaceActor(ACTOR_FELIX, FACING_WEST);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 3);
  Engine_ActorSetAnimationAndWait(SHEBA, 3);
  Actor_SetSpeed(SHEBA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(SHEBA, 260, 186);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WalkToAndWait(SHEBA, 276, 192);
  Event_PlayCue(19);
  object = Object_GetById(ACTOR_MENARDI);
  object->facing = FACING_SOUTH + FACING_STEP;
  Engine_TaskWait(1);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  VenusAerie_ShowMessage(ACTOR_MENARDI);
  Engine_ActorRunRepeatedMotion(SHEBA, 2);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(SHEBA, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorJump(MIA, 2, 20);
  VenusAerie_FaceActor(MIA, FACING_NORTHWEST);
  VenusAerie_ShowMessage(MIA);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Engine_ActorFaceDirection(SHEBA, FACING_EAST, 0);
  Engine_ActorFaceDirection(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 80);
  Engine_ActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 0);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_EAST, 40);
  Event_PlayCue(29);
  VenusAerie_ShowMessage(ACTOR_SATUROS);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(SHEBA, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  VenusAerie_ShowMessage(IVAN);
  VenusAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
  VenusAerie_ShowMessage(ACTOR_MENARDI);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  VenusAerie_FaceActor(SHEBA, FACING_NORTH + FACING_STEP);
  Engine_ActorSetAnimation(MIA, 3);
  VenusAerie_ShowMessage(MIA);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT, 20);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_SOUTHEAST + FACING_STEP, 20);
  VenusAerie_ShowMessage(ACTOR_SATUROS);
  Actor_ShowEmote(SHEBA, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 2, 60);
  Engine_ActorFaceDirection(SHEBA, FACING_EAST, 0);
  VenusAerie_FaceActor(ACTOR_FELIX, FACING_EAST);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 40);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  VenusAerie_ShowMessage(GARET);
  VenusAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 4);
  VenusAerie_ShowMessage(ACTOR_MENARDI);
  VenusAerie_FaceActor(ACTOR_SATUROS, FACING_EAST);
  Engine_ActorSetAnimation(ACTOR_SATUROS, 4);
  VenusAerie_ShowMessage(ACTOR_SATUROS);
  Engine_ActorFaceDirection(SHEBA, FACING_NORTH + FACING_STEP, 0);
  Actor_ShowEmote(SHEBA, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 60);
  Event_ShowMessageAndWait(MIA, 0, 20);
  VenusAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP);
  Engine_ActorSetAnimation(ACTOR_MENARDI, 4);
  VenusAerie_ShowMessage(ACTOR_MENARDI);
  Actor_SetAttachedEffect(IVAN, EMOTE_IN_FRONT | 2);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  VenusAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
  VenusAerie_ShowMessage(ACTOR_SATUROS);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 40);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  VenusAerie_FaceActor(MIA, FACING_NORTHWEST);
  VenusAerie_ShowMessage(MIA);
  Engine_ActorFaceDirection(GARET, FACING_WEST, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 20);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  VenusAerie_ShowMessage(GARET);
  Engine_ActorFaceDirection(SHEBA, FACING_EAST, 0);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Engine_ActorFaceDirection(IVAN, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 3, 40);
  Engine_ActorStartRepeatedMotion(ACTOR_SATUROS, 2);
  VenusAerie_ShowMessage(ACTOR_SATUROS);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  VenusAerie_ShowMessage(ACTOR_MENARDI);
  // Save the rod owner for the defeat return path, then enter battle.
  rodOwner = PartyInventory_FindOwner(ITEM_SHAMANS_ROD);
  GameFlag_Set(rodOwner + 0x345);
  PartyInventory_Remove(ITEM_SHAMANS_ROD);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(IVAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(MIA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(ACTOR_MENARDI)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(ACTOR_SATUROS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gSceneAction_EntryGroup);
  Actor_SetActionScript(GARET, gSceneAction_EntryGroup);
  Actor_SetActionScript(IVAN, gSceneAction_EntryGroup);
  Actor_SetActionScript(MIA, gSceneAction_EntryGroup);
  Actor_SetActionScript(ACTOR_MENARDI, gSceneAction_EntryPair);
  Actor_SetActionScript(ACTOR_SATUROS, gSceneAction_EntryPair);
  Actor_SetActionScript(ACTOR_FELIX, gSceneAction_EntryGroup);
  Actor_RunActionScriptAndWait(SHEBA, gSceneAction_EntryGroup);
  Battle_SetStart(3);
  {
    s32 actor = (s32)&SceneId_VenusAerie;

    Event_SetNextDestination(actor, 2);
    Event_SetDefeatDestination(actor, 9);
  }
  Field_RequestBattleFromEncounterTable(98, 1);
  GameFlag_Set(0x350);
}

// Flash an actor palette and emit paired effects every fifteenth frame.
static void VenusAerie_FlashActorPalette(struct FieldActor *actor) {
  if (*(s32 *)&gFrameCount & 2) {
    Engine_ObjectSetPartPalettes(actor, 7);
  } else {
    Engine_ObjectSetPartPalettes(actor, 0);
  }
  if (Math_RemainderUnsigned(*(s32 *)&gFrameCount, 15) == 0) {
    VenusAerie_SpawnLinkedPairEffects(actor);
  }
}

// Cycle the actor palette and emit paired effects every fifteenth frame.
static void VenusAerie_CycleActorPalette(struct FieldActor *actor) {
  if (gFrameCount & 1) {
    Engine_ObjectSetPartPalettes(actor,
                                 Math_RemainderUnsigned(gFrameCount >> 1, 6));
  }
  if (Math_RemainderUnsigned(gFrameCount, 15) == 0) {
    VenusAerie_SpawnLinkedPairEffects(actor);
  }
}

// Shared source is included here in ROM order.
#include "common/effect_vertical.h"

// Spawn two vertical effects using the parent position and shared cached tiles.
static void VenusAerie_SpawnLinkedPairEffects(struct FieldActor *parent) {
  struct FieldActor *pair[2];
  struct FieldActor *child;
  struct AnimationObject *sprite;
  struct BattleFxScene *sceneWork = gEffectWork;
  s32 effectIndex;

  Engine_AudioPlayCue(131);
  for (effectIndex = 0; effectIndex < 2; ++effectIndex) {
    child = Engine_ObjectCreate(26, parent->x.fixed, parent->y.fixed,
                                parent->z.fixed);
    pair[effectIndex] = child;
    if (child != NULL) {
      child->terrainHeight = parent->terrainHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->linkedObject = parent;
      if (sprite != NULL) {
        AnimationObjects_SelectAnimation(sprite, 0);
        sprite->flags = 0;
        Resource_ResetEntry(sprite->slot);
        sprite->slot = sceneWork->tileSlot;
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

// Advance Sheba's palette cycle.
static void VenusAerie_UpdateShebaPalette(void) {
  VenusAerie_CycleActorPalette(Object_GetById(SHEBA));
}

// Build the horizontal velocity for an elliptical radial particle burst.
static inline void VenusAerie_AimBurst(s32 velocity[3], s32 angle) {
  s32 sine;
  s32 cosine;

  velocity[0] = Engine_MathCos(angle);
  velocity[1] = 0;
  sine = Engine_MathSin(angle);
  cosine = velocity[0];
  cosine += cosine * 2;
  sine *= 2;
  velocity[2] = sine;
  velocity[0] = cosine;
}

// Give Felix, Sheba and the party the same reaction, then wait after Mia.
static inline void VenusAerie_ShowGroupEmote(s32 emote, s32 waitFrames) {
  Actor_ShowEmote(ACTOR_FELIX, emote, 0);
  Actor_ShowEmote(SHEBA, emote, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, emote, 0);
  Actor_ShowEmote(GARET, emote, 0);
  Actor_ShowEmote(IVAN, emote, 0);
  Actor_ShowEmote(MIA, emote, waitFrames);
}

// Show the Venus Star lighting the beacon and the antagonists combining.
static void VenusAerie_CombineSaturosAndMenardi(void) {
  struct EffectOptions menardiBurst;
  s32 velocity[3];
  struct EffectOptions saturosBurst;
  struct FieldActor *actor;
  struct FieldActor *menardi;
  struct FieldActor *saturos;
  union FieldObject *star = NULL;
  struct FieldActor *starCarrier;
  struct AnimationObject *sprite;
  u8 *iconPixels;
  // Reuse the dialogue answer word for the later radial burst directions.
  u32 sceneStep;

  Event_Begin();
  Map_CopyCellAttributes(17, 10, 4, 2, 17, 8);
  Actor_Get(GARET)->facing = FACING_WEST;
  Actor_SetPosition(GARET, PIXELS(328), PIXELS(168));
  Actor_Get(IVAN)->facing = FACING_WEST;
  Actor_SetPosition(IVAN, PIXELS(340), PIXELS(196));
  Actor_Get(MIA)->facing = FACING_WEST;
  Actor_SetPosition(MIA, PIXELS(326), PIXELS(204));
  Actor_Get(SHEBA)->facing = FACING_SOUTHEAST + FACING_STEP;
  Actor_SetPosition(SHEBA, PIXELS(268), PIXELS(154));
  Actor_Get(ACTOR_FELIX)->facing = FACING_SOUTHEAST + FACING_STEP;
  Actor_SetPosition(ACTOR_FELIX, PIXELS(268), PIXELS(164));
  actor = Actor_Get(ACTOR_SATUROS);
  actor->timer = 10;
  actor->facing = FACING_NORTH + FACING_STEP;
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(294), PIXELS(212));
  Actor_SetAnimation(ACTOR_SATUROS, 9);
  Actor_SetActionScript(ACTOR_SATUROS, gVenusAerie_ScaleStepAction);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SATUROS), 0);
  actor = Actor_Get(ACTOR_MENARDI);
  actor->timer = 10;
  actor->facing = FACING_EAST;
  Actor_SetPosition(ACTOR_MENARDI, PIXELS(286), PIXELS(192));
  Actor_SetAnimation(ACTOR_MENARDI, 7);
  Actor_SetActionScript(ACTOR_MENARDI, gVenusAerie_ScaleStepAction);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MENARDI), 0);
  Event_GetViewCenter()->motionFlags = 0;
  Camera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 0);
  Task_Wait(1);
  Map_Redraw();
  Task_Wait(1);
  Event_OpenScreen();
  Event_WaitForScreen();
  Event_Wait(80);

  Actor_Jump(GARET, 2, 20);
  VenusAerie_FaceActor(GARET, FACING_SOUTHEAST);
  Event_SetMessage(MsgVenusBeatEm);
  VenusAerie_ShowMessage(0x1001);
  VenusAerie_FaceActor(MIA, FACING_NORTHWEST);
  VenusAerie_ShowMessage(MIA);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  VenusAerie_FaceActor(IVAN, FACING_NORTHWEST);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_Get(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_FELIX, 280, 164);
  Event_Wait(1);
  Actor_Get(ACTOR_FELIX)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 40);
  VenusAerie_ShowMessage(IVAN);
  Actor_SetAnimationAndWait(ACTOR_FELIX, 4);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 20);
  Actor_RunRepeatedMotion(GARET, 1);
  VenusAerie_ShowMessage(GARET);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  VenusAerie_FaceActor(ACTOR_FELIX, FACING_EAST);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_StartRepeatedMotion(MIA, 2);
  VenusAerie_ShowMessage(MIA);
  Actor_SetAnimation(MIA, 4);
  VenusAerie_ShowMessage(MIA);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 5, 40);
  VenusAerie_FaceActor(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_Jump(IVAN, 2, 20);
  VenusAerie_ShowMessage(IVAN);
  Actor_SetAnimationAndWait(ACTOR_FELIX, 3);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_RunRepeatedMotion(GARET, 1);
  VenusAerie_FaceActor(GARET, FACING_SOUTHEAST);
  Event_OpenMessage(GARET, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
  if (Event_ChooseYesNo(0, 0) == 0) {
    sceneStep = TRUE;
  } else {
    gEventWork->message++;
    sceneStep = FALSE;
  }
  Task_Wait(20);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  VenusAerie_FaceActor(MIA, FACING_NORTHWEST);
  VenusAerie_FaceActor(ACTOR_FELIX, FACING_EAST);
  Event_ShowMessage(ACTOR_FELIX, 0);
  Event_PlayCue(17);
  Event_Wait(40);
  if (sceneStep) {
    gEventWork->message++;
  }

  // The defeated antagonists recover before using the Venus Star.
  Actor_Stop(ACTOR_SATUROS);
  Actor_Stop(ACTOR_MENARDI);
  Task_Wait(1);
  actor = Actor_Get(ACTOR_SATUROS);
  actor->scaleX = Q16_ONE;
  actor->scaleY = Q16_ONE;
  actor = Actor_Get(ACTOR_MENARDI);
  actor->scaleX = Q16_ONE;
  actor->scaleY = Q16_ONE;
  Task_Wait(1);
  VenusAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 40);
  Actor_SetAnimation(ACTOR_SATUROS, 1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SATUROS), 1);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_SATUROS, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_WalkToAndWait(ACTOR_SATUROS, 300, 206);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_Wait(20);
  Actor_Get(ACTOR_SATUROS)->scaleX = -Q16_ONE;
  Event_PlayCue(161);
  Actor_SetAnimation(ACTOR_SATUROS, 8);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_MENARDI, 1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MENARDI), 1);
  Actor_Jump(ACTOR_MENARDI, 4, 40);
  Actor_SetSpeed(ACTOR_MENARDI, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_Get(ACTOR_MENARDI)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_MENARDI, 296, 186);
  Actor_SetSpeed(ACTOR_MENARDI, PIXELS(1) / 10, PIXELS(1) / 20);
  Actor_WalkToAndWait(ACTOR_MENARDI, 292, 186);
  Actor_Get(ACTOR_MENARDI)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_MENARDI)->scaleX = -Q16_ONE;
  Event_PlayCue(161);
  Actor_SetAnimation(ACTOR_MENARDI, 5);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_Wait(80);
  Actor_SetAnimationAndWait(MIA, 4);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
  Event_Wait(40);
  VenusAerie_ShowMessage(ACTOR_MENARDI);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 20);

  actor = Actor_Get(ACTOR_SATUROS);
  actor->facing = FACING_SOUTHEAST + FACING_STEP;
  actor->scaleX = Q16_ONE;
  Actor_SetAnimation(ACTOR_SATUROS, 1);
  Event_Wait(10);
  VenusAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH + FACING_STEP);
  Actor_Jump(ACTOR_SATUROS, 6, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(SHEBA, FACING_EAST, 0);
  star = (union FieldObject *)Object_Create(
      22, actor->x.fixed, actor->y.fixed + PIXELS(8), actor->z.fixed);
  if (star != NULL) {
    sprite = star->actor.sprite;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    sprite->image.priority = 0;
    star->actor.priorityFlags &= ~1;
    star->actor.motionFlags = 0;
    star->actor.unk5c = 1;
    star->actor.speed = PIXELS(8) / 5;
    star->actor.acceleration = 0xcccc;
    iconPixels = Heap_Allocate(17, 0x608);
    // Display the Venus Star icon before sending it toward the beacon.
    Item_LoadIcon(ITEM_VENUS_STAR);
    Vram_Load(sprite->slot, 128, iconPixels + 0x400);
    Heap_Release(17);
  }
  Actor_SetSpritePriority(22, 1);
  starCarrier = Actor_Get(22);
  starCarrier->x.fixed = actor->x.fixed;
  starCarrier->y.fixed = PIXELS(32);
  starCarrier->z.fixed = actor->z.fixed;
  starCarrier->motionFlags = 3;
  starCarrier->speed = PIXELS(8) / 5;
  starCarrier->acceleration = PIXELS(4) / 5;
  starCarrier->scaleX = 3 * Q16_ONE / 4;
  starCarrier->scaleY = 3 * Q16_ONE / 4;
  if (star != NULL) {
    star->actor.motionFlags = 3;
    star->effect.velocity.y = PIXELS(3) / 5;
    star->effect.velocity.x = PIXELS(4) / 5;
    star->actor.velocityY = PIXELS(8);
    FieldObject_SetPosition(&star->actor, PIXELS(308), PIXELS(32), PIXELS(164));
  }
  Actor_MoveToAndWait(22, 308, 164);
  Actor_SetPosition(22, 0, 0);
  Actor_SetSpritePriority(ACTOR_FELIX, 0);
  if (star != NULL) {
    Event_PlayCue(0x135);
    Actor_SetSpriteFlags(&star->actor, 0);
    star->actor.velocityY = PIXELS(4);
    FieldObject_SetPosition(&star->actor, PIXELS(314), PIXELS(32), PIXELS(137));
    FieldObject_CommitPosition(&star->actor);
    Event_PlayCue(0x135);
    star->actor.sprite->image.priority = 1;
    star->actor.velocityY = PIXELS(6);
    FieldObject_SetPosition(&star->actor, PIXELS(285), PIXELS(32), PIXELS(146));
    FieldObject_CommitPosition(&star->actor);
    Event_PlayCue(0x135);
    star->actor.velocityY = PIXELS(5);
    FieldObject_SetPosition(&star->actor, PIXELS(300), PIXELS(32), PIXELS(154));
    FieldObject_CommitPosition(&star->actor);
    Task_Wait(6);
    star->actor.x.fixed = 0;
    star->actor.y.fixed = 0;
    star->actor.z.fixed = 0;
    VenusAerie_StopActorMotion(&star->actor);
  }
  // Let the group react as the Venus Star reaches the beacon.
  VenusAerie_ShowGroupEmote(EMOTE_IN_FRONT, 30);
  Actor_SetSpritePriority(ACTOR_FELIX, 1);
  Actor_Get(ACTOR_FELIX)->priorityFlags |= 1;
  Actor_StartRepeatedMotion(IVAN, 2);
  VenusAerie_ShowMessage(IVAN);
  Actor_RunRepeatedMotion(MIA, 1);
  Event_ShowMessageAndWait(MIA, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_Wait(20);
  VenusAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 0);
  Actor_FaceDirection(SHEBA, FACING_SOUTHEAST + FACING_STEP, 0);
  VenusAerie_FaceActor(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP);
  VenusAerie_ShowGroupEmote(EMOTE_IN_FRONT | 1, 60);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 8, 40);
  VenusAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 40);
  Event_OpenMessage(GARET, 0);
  if (Event_ChooseYesNo(0, 0) == 0) {
    Actor_SetAnimation(GARET, ANIM_NOD);
    sceneStep = TRUE;
  } else {
    gEventWork->message++;
    Actor_SetAnimation(GARET, ANIM_SHAKE_HEAD);
    sceneStep = FALSE;
  }
  VenusAerie_ShowMessage(GARET);
  if (sceneStep) {
    gEventWork->message++;
  }
  Event_Wait(20);

  Actor_SetSpriteFlags(Actor_Get(24), 0);
  Actor_SetChildValue(24, 7);
  actor = Actor_Get(24);
  actor->scaleY = -Q16_ONE;
  actor->scaleX = Q16_ONE / 10;
  actor->motionFlags = 0;
  actor->y.fixed = PIXELS(64);
  actor->x.fixed = PIXELS(304);
  actor->z.fixed = PIXELS(158);
  Actor_SetSpriteFlags(Actor_Get(25), 0);
  Actor_SetChildValue(25, 7);
  actor = Actor_Get(25);
  actor->scaleY = -Q16_ONE;
  actor->scaleX = Q16_ONE / 10;
  actor->motionFlags = 0;
  actor->y.fixed = PIXELS(96);
  actor->x.fixed = PIXELS(304);
  actor->z.fixed = PIXELS(158);
  VenusAerie_ShowGroupEmote(EMOTE_IN_FRONT, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(SHEBA, FACING_EAST, 0);
  Actor_SetActionScript(24, gVenusAerie_BeamOpenAction);
  Actor_SetActionScript(25, gVenusAerie_BeamOpenAction);
  Event_PlayCue(145);
  Map_SetShake(PIXELS(6), PIXELS(6), Q16_ONE);
  ColorBuffer_ApplyTarget(0x4063ff, 0);
  ColorBuffer_Interpolate(16);
  Task_Wait(20);
  ColorBuffer_ApplyTarget(0x7fff, 0);
  ColorBuffer_Interpolate(24);
  Task_Wait(60);
  Task_AddCallback(VenusAerie_SpawnUpperOrbitParticles, TASK_PRIORITY_SCENE);
  Event_PlayCue(141);
  Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  ColorBuffer_ApplyTarget(0x4063ff, 0);
  ColorBuffer_Interpolate(120);
  Actor_SetActionScript(24, gVenusAerie_BeamNarrowAction);
  Actor_SetActionScript(25, gVenusAerie_BeamNarrowAction);
  Task_Wait(120);
  Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  ColorBuffer_ApplyTarget(0x203210, 0);
  ColorBuffer_Interpolate(120);
  Task_Wait(120);
  Event_PlayCue(63);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  ColorBuffer_Interpolate(120);
  Task_Wait(120);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);

  Actor_SetAnimation(ACTOR_MENARDI, 1);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 0);
  Actor_Get(ACTOR_MENARDI)->scaleX = Q16_ONE;
  Actor_Jump(ACTOR_MENARDI, 4, 40);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(SHEBA, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  VenusAerie_ShowMessage(ACTOR_SATUROS);
  Actor_RunRepeatedMotion(MIA, 1);
  VenusAerie_FaceActor(MIA, FACING_WEST);
  VenusAerie_ShowMessage(MIA);
  VenusAerie_FaceActor(ACTOR_SATUROS, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, 4);
  VenusAerie_ShowMessage(ACTOR_SATUROS);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | 3, 20);
  Actor_StartRepeatedMotion(ACTOR_MENARDI, 2);
  VenusAerie_ShowMessage(ACTOR_MENARDI);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 20);
  VenusAerie_FaceActor(GARET, FACING_SOUTHWEST);
  VenusAerie_ShowMessage(GARET);
  VenusAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH + FACING_STEP);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, 3);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
  Actor_RunRepeatedMotion(GARET, 1);
  VenusAerie_ShowMessage(GARET);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  VenusAerie_ShowMessage(0x2013);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(GARET, 1);
  Actor_StartRepeatedMotion(IVAN, 1);
  Actor_RunRepeatedMotion(MIA, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  VenusAerie_FaceActor(MIA, FACING_NORTHWEST);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 1, 80);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 40);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  VenusAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 20);
  VenusAerie_FaceActor(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP);
  VenusAerie_ShowMessage(0x2014);
  Actor_SetAnimationAndWait(ACTOR_FELIX, 3);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST + FACING_STEP, 60);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  VenusAerie_ShowMessage(0x2013);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 40);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH + FACING_STEP, 40);
  VenusAerie_FaceActor(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP);
  VenusAerie_ShowMessage(0x2014);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_FELIX, 3);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | 1, 40);
  VenusAerie_ShowMessage(0x2013);
  VenusAerie_FaceActor(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 1, 20);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_SetAnimation(ACTOR_MENARDI, 4);
  VenusAerie_ShowMessage(0x2013);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 3, 80);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
  VenusAerie_ShowMessage(0x2013);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 3, 20);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimation(ACTOR_MENARDI, 4);
  VenusAerie_ShowMessage(0x2013);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 2, 60);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 8, 40);
  Actor_SetAnimation(ACTOR_SATUROS, 4);
  Event_ShowMessageAndWait(0x2014, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_Wait(20);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_SetAnimation(ACTOR_MENARDI, 3);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, 3);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST + FACING_STEP, 20);
  VenusAerie_ShowMessage(0xa015);
  Actor_SetSpeed(SHEBA, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(1), PIXELS(1) / 2);
  Actor_SetActionScript(ACTOR_FELIX, gVenusAerie_WalkOutAction);
  Event_Wait(20);
  Actor_SetActionScript(SHEBA, gVenusAerie_WalkOutAction);
  VenusAerie_FaceActor(GARET, FACING_SOUTHWEST);
  Actor_StartRepeatedMotion(GARET, 2);
  VenusAerie_ShowMessage(GARET);
  Event_PlayCue(17);
  // Flash the antagonists and release their matching radial bursts.
  ColorBuffer_ApplyTarget(0x40250d, 1);
  ColorBuffer_Interpolate(40);
  VenusAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH + FACING_STEP);
  VenusAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 0);
  VenusAerie_FlashScreen();
  Actor_SetChildValue(ACTOR_SATUROS, 7);
  Actor_SetChildValue(ACTOR_MENARDI, 7);
  Event_Wait(20);
  Actor_SetChildValue(ACTOR_SATUROS, 0x100);
  Actor_SetChildValue(ACTOR_MENARDI, 0x100);
  Event_Wait(20);
  VenusAerie_FlashScreen();
  Actor_Jump(MIA, 2, 20);
  VenusAerie_ShowMessage(MIA);
  VenusAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  VenusAerie_ShowMessage(ACTOR_MENARDI);
  VenusAerie_FlashScreen();

  menardi = Actor_Get(ACTOR_MENARDI);
  menardiBurst.palette = 7;
  menardiBurst.update = VenusAerie_UpdateDriftingParticle;
  menardiBurst.startScaleX = Q16_ONE;
  menardiBurst.startScaleY = Q16_ONE;
  for (sceneStep = 0; sceneStep <= 16; sceneStep++) {
    VenusAerie_AimBurst(velocity, sceneStep << 12);
    Effect_Spawn(menardi->x.fixed, menardi->y.fixed, menardi->z.fixed,
                 velocity[0], velocity[1], velocity[2],
                 EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                     EFFECT_USE_PALETTE,
                 &menardiBurst);
  }
  Event_PlayCue(212);
  Task_Wait(6);
  VenusAerie_FlashScreen();
  saturos = Actor_Get(ACTOR_SATUROS);
  saturosBurst.palette = 7;
  saturosBurst.update = VenusAerie_UpdateDriftingParticle;
  saturosBurst.startScaleX = Q16_ONE;
  saturosBurst.startScaleY = Q16_ONE;
  for (sceneStep = 0; sceneStep <= 16; sceneStep++) {
    VenusAerie_AimBurst(velocity, sceneStep << 12);
    Effect_Spawn(saturos->x.fixed, saturos->y.fixed, saturos->z.fixed,
                 velocity[0], velocity[1], velocity[2],
                 EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                     EFFECT_USE_PALETTE,
                 &saturosBurst);
  }
  Event_PlayCue(212);
  Actor_Jump(IVAN, 6, 20);
  Event_PlayCue(54);
  VenusAerie_ShowMessage(IVAN);
  Actor_SetAnimation(ACTOR_SATUROS, 4);
  VenusAerie_ShowMessage(ACTOR_SATUROS);
  VenusAerie_FlashScreen();
  Actor_SetSpeed(ACTOR_SATUROS, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_SetSpeed(ACTOR_MENARDI, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_Get(ACTOR_SATUROS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_MENARDI)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_SATUROS, 294, 196);
  Actor_SetDestination(ACTOR_MENARDI, 294, 196);
  // Slide the two transformed actors together, then start the fusion battle.
  Task_AddCallback(VenusAerie_UpdateBeamActors, TASK_PRIORITY_SCENE);
  Actor_StartRepeatedMotion(GARET, 2);
  Event_ShowMessage(GARET, 0);
  GameFlag_Set(FLAG_PAIR_SLIDING);
  Event_ShowMessage(IVAN, 0);
  GameFlag_Set(FLAG_PAIR_GROWING);
  VenusAerie_FlashScreen();
  Event_Wait(20);
  VenusAerie_FlashScreen();
  Event_Wait(20);
  Battle_SetStart(3);
  Event_SetNextDestination((s32)&SceneId_VenusAerie, 3);
  Event_SetDefeatDestination((s32)&SceneId_VenusAerie, 9);
  Field_RequestBattleFromEncounterTable(98, 0);
  Party_RestorePpOfLitElements();
  GameFlag_Set(0x351);
}

// Flash the object palette white, then restore the transformation colour.
static void VenusAerie_FlashScreen(void) {
  Engine_AudioPlayCue(187);
  Engine_ColorBufferApplyTarget(0x7fff, 1);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(4);
  Engine_ColorBufferApplyTarget(0x40250d, 1);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
}

enum { ORBIT_CENTER_ACTOR = 24 };

// Orbit a particle around the upper beam and keep its target at that position.
static void VenusAerie_UpdateUpperOrbit(union FieldObject *fieldObject) {
  struct FieldActor *effect = &fieldObject->actor;
  struct FieldActor *center = Object_GetById(ORBIT_CENTER_ACTOR);
  u16 angle = effect->timer;

  effect->x.fixed =
      center->x.fixed + Engine_MathCos(angle) * (effect->speed + 3);
  effect->z.fixed = center->z.fixed + Engine_MathSin(angle) * 2;
  effect->targetX = effect->x.fixed;
  effect->targetZ = effect->z.fixed;
  effect->timer -= 0x800;
}

// Orbit a particle around the rising beam using its rise counter as a radius.
static void VenusAerie_UpdateRisingOrbit(union FieldObject *fieldObject) {
  struct FieldActor *actor = &fieldObject->actor;
  struct FieldActor *anchor = Object_GetById(23);
  s32 angle = (u16)actor->timer;
  s32 cosine;
  s32 sine;
  s32 along;
  s32 across;

  cosine = Engine_MathCos(angle);
  along = anchor->x.fixed + cosine * (actor->speed + actor->riseCounter + 6);
  actor->x.fixed = along;

  sine = Engine_MathSin(angle);
  across = anchor->z.fixed + sine * (actor->riseCounter + 4);
  actor->z.fixed = across;

  actor->targetX = actor->x.fixed;
  actor->targetZ = across;

  {
    s32 next = (u16)actor->timer;
    next += 0xfffff800;
    actor->timer = next;
  }
}

// Emit rising particles around the upper beam at the current beacon stage.
static void VenusAerie_SpawnUpperOrbitParticles(void) {
  struct FieldActor *center;
  struct FieldActor *effect;
  struct AnimationObject *sprite;
  s32 angle;

  if (GameFlag_IsSet(0x236) == 0 &&
      Math_RemainderUnsigned(gFrameCount, 3) != 0) {
    return;
  }
  center = Actor_Get(ORBIT_CENTER_ACTOR);
  if (GameFlag_IsSet(0x236) != 0) {
    effect = Object_Create(284, center->x.fixed,
                           (s32)((u32)(Random_Next() << 8) >> 16 << 16) +
                               center->y.fixed - PIXELS(28),
                           center->z.fixed);
  } else {
    effect = Object_Create(284, center->x.fixed,
                           (s32)((u32)(Random_Next() << 6) >> 16 << 16) +
                               center->y.fixed - PIXELS(28),
                           center->z.fixed);
  }
  if (effect != 0) {
    sprite = effect->sprite;
    Object_SetScript(effect, gVenusAerie_OrbitParticleScript);
    ObjectGroup_SetChildValue(effect, 1);
    effect->motionFlags = 0;
    angle = Random_Next() & 0xffff000;
    effect->timer = angle;
    effect->work = 0;
    effect->update = VenusAerie_UpdateUpperOrbit;
    effect->speed =
        Engine_MathSin((u32)(Random_Next() * 0xffff) >> 20) * 24 >> 16;
    sprite->flags = 0;
    sprite->image.priority = 1;
  }
}

enum { PARTICLE_SOURCE_ACTOR = 23 };

// Test whether all three movement targets are unset.
static __inline__ s32 Actor_IsParked(struct FieldActor *actor) {
  if (actor->targetX == ACTOR_NO_TARGET && actor->targetY == actor->targetX &&
      actor->targetZ == actor->targetY) {
    return 1;
  }
  return 0;
}

// Pulse and slide Saturos and Menardi while their beam flags are active.
static void VenusAerie_UpdateBeamActors(void) {
  struct FieldActor *saturos;
  struct FieldActor *menardi;
  s32 pairParked;

  saturos = Object_GetById(ACTOR_SATUROS);
  menardi = Object_GetById(ACTOR_MENARDI);
  pairParked = Actor_IsParked(saturos); // Overwritten; only the saturos actor's
                                        // test loads remain in the ROM.
  pairParked = Actor_IsParked(menardi);
  if (pairParked != 0) {
    saturos->facing = 0;
    menardi->facing = 0;
    if (Engine_GameFlagIsSet(FLAG_PAIR_GROWING) != 0) {
      Engine_ActorSetChildValue(ACTOR_SATUROS, 7);
      Engine_ActorSetChildValue(ACTOR_MENARDI, 7);
      if (saturos->scaleX < 5 * Q16_ONE / 4) {
        saturos->scaleX += Q16_ONE / 128;
        saturos->scaleY += Q16_ONE / 128;
        menardi->scaleX += Q16_ONE / 128;
        menardi->scaleY += Q16_ONE / 128;
      }
    } else {
      if ((*(s32 *)&gFrameCount & 2) != 0) {
        Engine_ActorSetChildValue(ACTOR_SATUROS, 15);
        Engine_ActorSetChildValue(ACTOR_MENARDI, 0);
      } else {
        Engine_ActorSetChildValue(ACTOR_SATUROS, 0);
        Engine_ActorSetChildValue(ACTOR_MENARDI, 15);
      }
    }
    if (Engine_GameFlagIsSet(FLAG_PAIR_SLIDING) != 0) {
      if (saturos->x.fixed < PIXELS(312)) {
        saturos->x.fixed += PIXELS(1) / 16;
        menardi->x.fixed += PIXELS(1) / 16;
      }
      if (saturos->z.fixed > PIXELS(182)) {
        saturos->z.fixed += -PIXELS(1) / 16;
        menardi->z.fixed += -PIXELS(1) / 16;
      }
    }
  }
}

// Play the antagonists' defeat, the party discussion and the beacon eruption.
static void VenusAerie_FinishSaturosAndMenardiBattle(void) {
  Engine_EventBegin();
  VenusAerie_ShowSaturosAndMenardiFall();
  VenusAerie_DiscussSaturosAndMenardiDefeat();
  VenusAerie_EruptBeacon();
  Engine_EventEnd();
}

// Restore the broken aerie, show the fall warning and leave through exit one.
static void VenusAerie_PlayFallWarningExit(void) {
  Engine_EventBegin();
  VenusAerie_RestorePartyOnBrokenAerie();
  VenusAerie_TryToPreventFall();
  Engine_GameFlagSet(141 << 1);

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  gEventWork->transitionFrames = 24;

  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(1);
  Engine_EventEnd();
}

// Stage the broken aerie for the fall sequence and the party escape.
static void VenusAerie_RunLeapEntrance(void) {
  struct FieldActor *leader;
  struct FieldActor *actor;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Engine_EventGetViewCenter()->motionFlags = 0;
  Map_CopyCellsTo(102, 4, 74, 4, 18, 23);
  Map_CopyCellsTo(39, 72, 11, 72, 16, 20);
  Map_CopyCellAttributes(19, 6, 3, 7, 22, 6);
  Map_CopyCellAttributes(19, 6, 3, 7, 13, 6);
  Map_CopyCellAttributes(19, 6, 3, 7, 22, 13);
  Map_CopyCellAttributes(19, 6, 3, 7, 13, 13);
  Engine_TaskWait(1);
  Camera_MoveTo(PIXELS(192), -PIXELS(64), PIXELS(238), 0);
  Engine_TaskWait(1);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_ActorDestroy(ACTOR_SATUROS);
  Engine_ActorDestroy(ACTOR_MENARDI);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  leader->x.fixed = PIXELS(346);
  leader->z.fixed = PIXELS(205);
  leader->y.fixed = PIXELS(32);
  leader->facing = FACING_SOUTHWEST;
  VenusAerie_StopActorMotion(leader);
  Engine_ActorSetAnimation(GARET, 18);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 0);
  actor = Actor_Get(GARET);
  actor->x.fixed = PIXELS(356);
  actor->z.fixed = PIXELS(192);
  actor->facing = FACING_NORTHWEST;
  actor->y.fixed = PIXELS(32);
  VenusAerie_StopActorMotion(actor);
  Engine_ActorSetAnimation(IVAN, 18);
  Engine_ActorSetSpriteFlags(Actor_Get(IVAN), 0);
  actor = Actor_Get(IVAN);
  actor->x.fixed = PIXELS(360);
  actor->facing = FACING_SOUTHEAST;
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(222);
  VenusAerie_StopActorMotion(actor);
  Engine_ActorSetAnimation(MIA, 18);
  Engine_ActorSetSpriteFlags(Actor_Get(MIA), 0);
  actor = Actor_Get(MIA);
  actor->x.fixed = PIXELS(334);
  actor->facing = FACING_WEST;
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(222);
  VenusAerie_StopActorMotion(actor);
  Actor_SetPosition(ACTOR_FELIX, PIXELS(196), PIXELS(220));
  Engine_ActorSetAnimation(ACTOR_FELIX, 5);
  Actor_SetPosition(SHEBA, PIXELS(188), PIXELS(316));
  Engine_ActorSetAnimation(SHEBA, 5);
  Engine_ActorSetSpriteFlags(Actor_Get(SHEBA), 0);
  actor = Actor_Get(8);
  actor->x.fixed -= PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(9);
  actor->x.fixed -= PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  actor = Actor_Get(10);
  actor->x.fixed += PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(11);
  actor->x.fixed += PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Engine_ActorSetSpriteFlags(Actor_Get(PARTICLE_SOURCE_ACTOR), 0);
  Object_GetById(PARTICLE_SOURCE_ACTOR)->motionFlags = 4;
  Actor_SetChildValue(PARTICLE_SOURCE_ACTOR, 4);
  Actor_Get(PARTICLE_SOURCE_ACTOR)->y.fixed = PIXELS(40);
  FieldScheduler_AddCallback(VenusAerie_SpawnSideOrbitParticles,
                             TASK_PRIORITY_SCENE);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  gEventWork->transitionFrames = 24;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  VenusAerie_PlayShebasFall();
  GameFlag_Set(0x9a7);
  Engine_EventRequestExit(2);
}

// Show Saturos and Menardi falling into the beacon after their final defeat.
static void VenusAerie_ShowSaturosAndMenardiFall(void) {
  struct FieldActor *actor;

  Event_PlayCue(141);
  Map_CopyCellAttributes(17, 10, 4, 2, 17, 8);
  Actor_Get(ACTOR_PARTY_LEADER)->facing = FACING_NORTHWEST;
  Actor_Get(GARET)->facing = FACING_NORTHWEST;
  Actor_SetPosition(GARET, PIXELS(328), PIXELS(168));
  Actor_Get(IVAN)->facing = FACING_NORTHWEST;
  Actor_SetPosition(IVAN, PIXELS(340), PIXELS(196));
  Actor_Get(MIA)->facing = FACING_NORTHWEST;
  Actor_SetPosition(MIA, PIXELS(326), PIXELS(204));
  Actor_Get(ACTOR_FELIX)->facing = FACING_NORTH + FACING_STEP;
  Actor_SetPosition(ACTOR_FELIX, PIXELS(200), PIXELS(216));
  Actor_Get(SHEBA)->facing = FACING_NORTH + FACING_STEP;
  Actor_SetPosition(SHEBA, PIXELS(200), PIXELS(216));
  Actor_Get(ACTOR_SATUROS)->facing = FACING_SOUTHEAST + FACING_STEP;
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(310), PIXELS(158));
  Actor_Get(ACTOR_MENARDI)->facing = FACING_SOUTHEAST + FACING_STEP;
  Actor_SetPosition(ACTOR_MENARDI, PIXELS(292), PIXELS(158));

  Engine_ActorSetSpriteFlags(Actor_Get(24), 0);
  Actor_SetChildValue(24, 7);
  Engine_ActorSetSpritePriority(24, 1);
  actor = Actor_Get(24);
  actor->scaleY = -Q16_ONE;
  actor->scaleX = Q16_ONE / 5;
  actor->motionFlags = 0;
  actor->x.fixed = PIXELS(304);
  actor->y.fixed = PIXELS(2);
  actor->z.fixed = PIXELS(96);

  Engine_ActorSetSpriteFlags(Actor_Get(25), 0);
  Actor_SetChildValue(25, 7);
  Engine_ActorSetSpritePriority(25, 1);
  actor = Actor_Get(25);
  actor->scaleY = -Q16_ONE;
  actor->scaleX = Q16_ONE / 5;
  actor->motionFlags = 0;
  actor->x.fixed = PIXELS(304);
  actor->y.fixed = PIXELS(34);
  actor->z.fixed = PIXELS(96);

  FieldScheduler_AddCallback(VenusAerie_SpawnUpperOrbitParticles,
                             TASK_PRIORITY_SCENE);
  Engine_EventGetViewCenter()->motionFlags = 0;
  Camera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 0);
  Engine_TaskWait(1);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
  Engine_EventWait(10);
  // Give Saturos and Menardi their final exchange before they fall.
  Engine_EventSetMessage(MsgVenusPairDefeated);
  VenusAerie_ShowMessage(ACTOR_SATUROS);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 40);
  Event_PlayCue(17);

  actor = Actor_Get(ACTOR_SATUROS);
  actor->x.fixed = PIXELS(308);
  actor->y.fixed = PIXELS(28);
  actor->z.fixed = PIXELS(152);
  Engine_ActorSetAnimation(ACTOR_SATUROS, 10);
  Engine_EventWait(20);
  actor->x.fixed = PIXELS(306);
  actor->y.fixed = PIXELS(28);
  actor->z.fixed = PIXELS(152);
  Engine_ActorSetAnimation(ACTOR_SATUROS, 11);
  Engine_EventWait(12);
  actor->x.fixed = PIXELS(304);
  actor->y.fixed = PIXELS(21);
  actor->z.fixed = PIXELS(152);
  Engine_ActorSetAnimation(ACTOR_SATUROS, 12);
  Engine_EventWait(8);
  Engine_ActorDestroy(ACTOR_SATUROS);

  actor = Actor_Get(ACTOR_MENARDI);
  actor->x.fixed = PIXELS(294);
  actor->y.fixed = PIXELS(28);
  actor->z.fixed = PIXELS(152);
  Engine_ActorSetAnimation(ACTOR_MENARDI, 8);
  Engine_EventWait(20);
  actor->x.fixed = PIXELS(300);
  actor->y.fixed = PIXELS(27);
  actor->z.fixed = PIXELS(152);
  Engine_ActorSetAnimation(ACTOR_MENARDI, 9);
  Engine_EventWait(12);
  actor->x.fixed = PIXELS(304);
  actor->y.fixed = PIXELS(17);
  actor->z.fixed = PIXELS(152);
  Engine_ActorSetAnimation(ACTOR_MENARDI, 10);
  Engine_EventWait(8);
  Engine_ActorDestroy(ACTOR_MENARDI);
  Engine_EventWait(160);
}

// Let the party discuss the battle while Felix and Sheba remain on the aerie.
static void VenusAerie_DiscussSaturosAndMenardiDefeat(void) {
  s32 answerAccepted;
  s32 garetSpeaker;
  s32 miaSpeaker;

  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(20);
  Event_OpenMessage(IVAN, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  VenusAerie_FaceActor(0, FACING_SOUTH);
  answerAccepted = 0;
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(IVAN, 4);
    answerAccepted = 1;
  } else {
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    gEventWork->message += 1;
  }
  VenusAerie_ShowMessage(2);
  if (answerAccepted != 0) {
    gEventWork->message += 1;
  }
  Actor_SetSpeed(GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  garetSpeaker = 0x1001;
  Actor_WalkToAndWait(GARET, 321, 174);
  VenusAerie_FaceActor(1, FACING_SOUTHEAST);
  VenusAerie_ShowMessage(garetSpeaker);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  VenusAerie_FaceActor(3, FACING_NORTH);
  VenusAerie_FaceActor(1, FACING_SOUTH);
  VenusAerie_ShowMessage(garetSpeaker);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 5, 40);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 20);
  VenusAerie_ShowMessage(garetSpeaker);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 40);
  Event_ShowMessageAndWait(0x8001, 0, 40);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 20);
  VenusAerie_ShowMessage(garetSpeaker);
  Event_PlayCue(17);
  Engine_ActorSetAnimation(MIA, 4);
  VenusAerie_ShowMessage(3);
  Actor_FaceDirection(GARET, FACING_SOUTH, 80);
  Actor_FaceDirection(MIA, FACING_SOUTHWEST, 80);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  VenusAerie_FaceActor(2, FACING_NORTH);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 60);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 40);
  Engine_ActorJump(IVAN, 4, 60);
  VenusAerie_FaceActor(1, FACING_SOUTHEAST);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 40);
  VenusAerie_ShowMessage(garetSpeaker);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 40);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Engine_ActorJump(GARET, 4, 40);
  VenusAerie_ShowMessage(1);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetDestination(ACTOR_FELIX, 200, 188);
  Actor_SetDestination(SHEBA, 200, 204);
  Camera_SetSpeed(PIXELS(16) / 5, PIXELS(2) / 5);
  Camera_MoveTo(PIXELS(252), 0, PIXELS(190), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Event_PlayCue(23);
  Actor_SetAttachedEffect(ACTOR_FELIX, EMOTE_IN_FRONT | 2);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Engine_EventWait(20);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 4);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 20);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAttachedEffect(SHEBA, EMOTE_IN_FRONT | 2);
  miaSpeaker = 0x2003;
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  VenusAerie_ShowMessage(miaSpeaker);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 2);
  Engine_EventWait(20);
  VenusAerie_ShowMessage(0x2002);
  VenusAerie_FaceActor(ACTOR_FELIX, FACING_NORTHEAST);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  VenusAerie_ShowMessage(1);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Event_ShowMessageAndWait(ACTOR_FELIX, 0, 20);
  VenusAerie_ShowMessage(miaSpeaker);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 4);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAttachedEffect(IVAN, EMOTE_IN_FRONT | 2);
  VenusAerie_ShowMessage(0x2002);
  Engine_ActorSetAnimation(ACTOR_FELIX, 3);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 40);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  VenusAerie_ShowMessage(1);
  Engine_ActorSetAnimation(ACTOR_FELIX, 4);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorJump(MIA, 4, 20);
  Event_ShowMessageAndWait(miaSpeaker, 0, 20);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 3, 40);
}

static s32 sTransitionTimer;
static s32 sTransitionStep;

// Raise the beacon effects and split the aerie beneath the group.
static void VenusAerie_EruptBeacon(void) {
  struct FieldActor *lower;
  struct FieldActor *middle;
  struct FieldActor *upper;
  struct FieldActor *actor;

  Engine_AudioPlayCue(19);
  Engine_AudioPlayCue(0x120);
  Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(SHEBA, EMOTE_IN_FRONT, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(SHEBA, FACING_NORTH + FACING_STEP, 0);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Engine_EventWait(10);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Camera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 1);
  Engine_CameraWaitForMove();
  // Open the beacon beam before splitting the surrounding platform.
  Object_GetById(24)->scaleX = Q16_ONE / 10;
  Object_GetById(25)->scaleX = Q16_ONE / 10;
  Actor_SetActionScript(24, gVenusAerie_BeamOpenAction);
  Actor_SetActionScript(25, gVenusAerie_BeamOpenAction);
  Engine_AudioPlayCue(145);
  Map_SetShake(PIXELS(6), PIXELS(6), Q16_ONE);
  ColorBuffer_ApplyTarget(0x4063ff, 0);
  Engine_ColorBufferInterpolate(16);
  Engine_TaskWait(20);
  Engine_ColorBufferApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(24);
  Engine_TaskWait(60);
  Engine_AudioPlayCue(141);
  Engine_GameFlagSet(0x236);
  Object_GetById(24)->y.fixed = -PIXELS(96);
  Object_GetById(25)->y.fixed = -PIXELS(64);
  Engine_ActorSetChildValue(26, 7);
  Engine_ActorSetSpriteFlags(Object_GetById(26), 0);
  // Stack three more beam segments under the rising upper pair.
  lower = Object_GetById(26);
  lower->scaleY = -Q16_ONE;
  lower->scaleX = Object_GetById(24)->scaleX;
  lower->motionFlags = 0;
  lower->x.fixed = PIXELS(304);
  lower->y.fixed = -PIXELS(32);
  lower->z.fixed = PIXELS(96);
  Engine_ActorSetChildValue(27, 7);
  Engine_ActorSetSpriteFlags(Object_GetById(27), 0);
  middle = Object_GetById(27);
  middle->scaleY = -Q16_ONE;
  middle->scaleX = Object_GetById(24)->scaleX;
  middle->motionFlags = 0;
  middle->x.fixed = PIXELS(304);
  middle->y.fixed = 0;
  middle->z.fixed = PIXELS(96);
  Engine_ActorSetChildValue(28, 7);
  Engine_ActorSetSpriteFlags(Object_GetById(28), 0);
  upper = Object_GetById(28);
  upper->scaleY = -Q16_ONE;
  upper->scaleX = Object_GetById(24)->scaleX;
  upper->motionFlags = 0;
  upper->x.fixed = PIXELS(304);
  upper->y.fixed = PIXELS(32);
  upper->z.fixed = PIXELS(96);
  Map_CopyCellsTo(102, 4, 74, 4, 18, 23);
  Map_CopyCellsTo(39, 72, 11, 72, 16, 21);
  Map_CopyCellAttributes(19, 6, 3, 7, 22, 6);
  Map_CopyCellAttributes(19, 6, 3, 7, 13, 6);
  Map_CopyCellAttributes(19, 6, 3, 7, 22, 13);
  Map_CopyCellAttributes(19, 6, 3, 7, 13, 13);
  Engine_TaskWait(1);
  actor = Object_GetById(8);
  actor->x.fixed -= PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(9);
  actor->x.fixed -= PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(10);
  actor->x.fixed += PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(11);
  actor->x.fixed += PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->x.fixed += PIXELS(16);
  actor->z.fixed += PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(GARET);
  actor->x.fixed += PIXELS(16);
  actor->z.fixed += PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(IVAN);
  actor->x.fixed += PIXELS(16);
  actor->z.fixed += PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(MIA);
  actor->x.fixed += PIXELS(16);
  actor->z.fixed += PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  Actor_SetPosition(ACTOR_FELIX, PIXELS(196), PIXELS(220));
  Engine_ActorSetAnimation(ACTOR_FELIX, 5);
  Actor_SetPosition(SHEBA, PIXELS(188), PIXELS(316));
  Engine_ActorSetAnimation(SHEBA, 5);
  Engine_ActorSetSpriteFlags(Object_GetById(SHEBA), 0);
  Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  Engine_ColorBufferApplyTarget(0x4063ff, 0);
  Engine_ColorBufferInterpolate(120);
  Actor_SetActionScript(24, gVenusAerie_BeamRiseAction);
  Actor_SetActionScript(25, gVenusAerie_BeamRiseAction);
  Actor_SetActionScript(26, gVenusAerie_BeamRiseAction);
  Actor_SetActionScript(27, gVenusAerie_BeamRiseAction);
  Actor_SetActionScript(28, gVenusAerie_BeamRiseAction);
  Engine_TaskWait(120);
  Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Engine_ColorBufferApplyTarget(0x203210, 0);
  Engine_ColorBufferInterpolate(120);
  Engine_TaskWait(120);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(120);
  Engine_TaskWait(120);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Object_GetById(24)->scaleY = Q16_ONE / 50;
  Actor_SetActionScript(25, gVenusAerie_BeamCloseAction);
  Actor_SetActionScript(26, gVenusAerie_BeamCloseAction);
  Actor_SetActionScript(27, gVenusAerie_BeamCloseAction);
  Actor_RunActionScriptAndWait(28, gVenusAerie_BeamCloseAction);
  Engine_AudioPlayCue(0x121);
  Engine_ActorSetChildValue(24, 15);
  Engine_EventWait(20);
  Actor_RunActionScriptAndWait(24, gVenusAerie_BeamEndAction);
  Scheduler_RemoveCallback(VenusAerie_SpawnUpperOrbitParticles);
  Engine_ActorJump(IVAN, 2, 20);
  VenusAerie_ShowMessage(IVAN);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 20);
  Actor_SetAttachedEffect(GARET, EMOTE_IN_FRONT | 2);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  VenusAerie_ShowMessage(GARET);
  Actor_SetSpeed(MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  Engine_ActorWalkToAndWait(MIA, 326, 220);
  Engine_EventWait(40);
  Engine_ActorSetAttachedEffect(MIA, EMOTE_IN_FRONT | 2);
  VenusAerie_ShowMessage(MIA);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->riseCounter = 0;
  actor->riseEnabled = 1;
  actor->riseBaseY = actor->y.fixed;
  actor->facing = FACING_NORTHWEST;
  actor = Actor_Get(GARET);
  actor->riseCounter = 0;
  actor->riseEnabled = 1;
  actor->facing = FACING_NORTHWEST;
  actor->riseBaseY = actor->y.fixed;
  actor = Actor_Get(IVAN);
  actor->riseCounter = 0;
  actor->riseEnabled = 1;
  actor->facing = FACING_NORTHWEST;
  actor->riseBaseY = actor->y.fixed;
  actor = Actor_Get(MIA);
  actor->riseCounter = 0;
  actor->riseEnabled = 1;
  actor->facing = FACING_NORTHWEST;
  actor->riseBaseY = actor->y.fixed;
  actor = Actor_Get(ACTOR_FELIX);
  actor->riseCounter = 0;
  actor->riseEnabled = 1;
  actor->riseBaseY = actor->y.fixed;
  actor = Actor_Get(SHEBA);
  actor->riseCounter = 0;
  actor->riseEnabled = 1;
  actor->riseBaseY = actor->y.fixed;
  Object_GetById(PARTICLE_SOURCE_ACTOR)->motionFlags = 0;
  Engine_ActorSetSpriteFlags(Object_GetById(PARTICLE_SOURCE_ACTOR), 0);
  Engine_ActorSetChildValue(PARTICLE_SOURCE_ACTOR, 7);
  Engine_ActorSetSpritePriority(PARTICLE_SOURCE_ACTOR, 2);
  sTransitionStep = 0;
  sTransitionTimer = 240;
  FieldScheduler_AddCallback(VenusAerie_UpdateBeaconRise, TASK_PRIORITY_SCENE);
  do {
    Engine_TaskWait(1);
  } while (Engine_GameFlagIsSet(0x237) == 0);
  Engine_GameFlagSet(0x101);
  Engine_EventWait(30);
  Engine_GameFlagSet(0x11a);
  ObjectTable_Snapshot();
  Event_SetDestination((s32)&SceneId_WorldMap, 91);
  {
    u16 white = 0x7fff;

    PLTT_BACKDROP = white;
  }
  gEventWork->transitionFrames = 1;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
}

// Place the party on the broken platform and resume its falling effects.
static void VenusAerie_RestorePartyOnBrokenAerie(void) {
  struct FieldActor *actor;

  Engine_ActorDestroy(ACTOR_SATUROS);
  Engine_ActorDestroy(ACTOR_MENARDI);
  Event_PlayCue(141);
  Map_CopyCellAttributes(17, 10, 4, 2, 17, 8);
  Map_CopyCellsTo(102, 4, 74, 4, 18, 23);
  Map_CopyCellsTo(39, 72, 11, 72, 16, 21);
  Map_CopyCellAttributes(19, 6, 3, 7, 22, 6);
  Map_CopyCellAttributes(19, 6, 3, 7, 13, 6);
  Map_CopyCellAttributes(19, 6, 3, 7, 22, 13);
  Map_CopyCellAttributes(19, 6, 3, 7, 13, 13);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_ActorsRefresh();
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Event_PlayCue(SOUND_ITEM_BREAK);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
  Engine_ActorSetAnimation(GARET, 18);
  Engine_ActorSetAnimation(IVAN, 18);
  Engine_ActorSetAnimation(MIA, 18);
  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 0);
  Engine_ActorSetSpriteFlags(Actor_Get(IVAN), 0);
  Engine_ActorSetSpriteFlags(Actor_Get(MIA), 0);

  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->x.fixed = PIXELS(346);
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(205);
  VenusAerie_StopActorMotion(actor);
  actor->riseEnabled = 0;
  actor->velocityY = PIXELS(2);

  actor = Actor_Get(GARET);
  actor->x.fixed = PIXELS(356);
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(192);
  VenusAerie_StopActorMotion(actor);
  actor->riseEnabled = 0;
  actor->velocityY = PIXELS(2);

  actor = Actor_Get(IVAN);
  actor->x.fixed = PIXELS(360);
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(222);
  VenusAerie_StopActorMotion(actor);
  actor->riseEnabled = 0;
  actor->velocityY = PIXELS(2);

  actor = Actor_Get(MIA);
  actor->x.fixed = PIXELS(334);
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(222);
  VenusAerie_StopActorMotion(actor);
  actor->riseEnabled = 0;
  actor->velocityY = PIXELS(2);

  Actor_Get(ACTOR_FELIX)->riseCounter = 0;
  Actor_Get(SHEBA)->riseCounter = 0;
  Actor_Get(PARTICLE_SOURCE_ACTOR)->motionFlags |= 4;
  Actor_SetChildValue(PARTICLE_SOURCE_ACTOR, 4);
  FieldScheduler_AddCallback(VenusAerie_SpawnSideOrbitParticles,
                             TASK_PRIORITY_SCENE);
  gEventWork->transitionFrames = 1;
  Engine_EventOpenScreen();
  ColorBuffer_ApplySource(0x7fff, 0);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(40);
  Engine_TaskWait(60);
}

// Show Felix urging Sheba to hold on at the edge of the platform.
static void VenusAerie_TryToPreventFall(void) {
  Engine_EventSetMessage(MsgVenusNoooo);
  VenusAerie_ShowMessage(ACTOR_FELIX);
  Event_PlayCue(62);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Camera_SetSpeed(PIXELS(24) / 5, PIXELS(3) / 5);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(192), -PIXELS(64), PIXELS(238), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Event_ShowMessageAndWait(0x2015, 0, 40);
  Engine_ActorRunRepeatedMotion(SHEBA, 3);
  VenusAerie_ShowMessage(SHEBA);
  Actor_SetAttachedEffect(ACTOR_FELIX, EMOTE_IN_FRONT | 2);
  Engine_EventWait(60);
  Event_ShowMessageAndWait(0x2015, 0, 80);
  Actor_SetAttachedEffect(SHEBA, EMOTE_IN_FRONT | 2);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(SHEBA, 2);
  VenusAerie_ShowMessage(SHEBA);
}

// Show Sheba falling, Felix jumping after her and the party escaping.
static void VenusAerie_PlayShebasFall(void) {
  struct FieldActor *actor;
  s32 stepPending;

  stepPending = 0;
  // Sheba loses her grip while Felix tries to keep her on the platform.
  Engine_EventSetMessage(MsgVenusWhyHappeningProtectVenusLighthouse);
  Actor_SetAttachedEffect(ACTOR_FELIX, EMOTE_IN_FRONT | 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x2015, 0, 20);
  Engine_ActorRunRepeatedMotion(SHEBA, 2);
  Event_ShowMessageAndWait(SHEBA, 0, 20);
  Engine_ActorSetAnimation(SHEBA, 6);
  Engine_EventWait(10);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 2);
  Event_ShowMessageAndWait(0x2015, 0, 40);
  Engine_ActorSetAnimation(SHEBA, 7);
  Event_ShowMessageAndWait(SHEBA, 0, 20);
  Event_PlayCue(17);
  Engine_ActorRunRepeatedMotion(SHEBA, 2);
  Actor_SetActionScript(SHEBA, gVenusAerie_StepBackAction);
  Engine_EventWait(20);
  Actor_SetAttachedEffect(ACTOR_FELIX, EMOTE_IN_FRONT | 2);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 3);
  Object_RefreshSelectorById(SHEBA);
  Engine_EventWait(160);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Engine_EventWait(20);
  Object_GetById(ACTOR_FELIX)->facing = FACING_SOUTH + FACING_STEP;
  Engine_ActorSetAnimation(ACTOR_FELIX, 0);
  Engine_EventWait(80);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 4);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(ACTOR_FELIX, 0, 40);
  Engine_ActorSetAnimation(ACTOR_FELIX, 5);
  Engine_EventWait(10);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FELIX), 0);
  Engine_ActorSetAnimation(ACTOR_FELIX, 0);
  Engine_ActorJump(ACTOR_FELIX, 6, 0);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(3), PIXELS(3) / 2);
  Object_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  actor = Object_GetById(ACTOR_FELIX);
  // Felix jumps after Sheba; the remaining party watches from the far side.
  actor->update = VenusAerie_StepSpriteRotation;
  actor->facing = FACING_WEST;
  Actor_MoveToAndWait(ACTOR_FELIX, 184, 237);
  Actor_RunActionScriptAndWait(ACTOR_FELIX, gVenusAerie_LeapAction);
  Engine_EventWait(120);
  VenusAerie_ShowMessage(ACTOR_PARTY_LEADER);
  Event_PlayCue(72);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(342), PIXELS(32), PIXELS(212), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(40);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 1);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 60);
  Object_GetById(ACTOR_PARTY_LEADER)->timer = 10;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gVenusAerie_ScaleStepAction);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 80);
  Event_ShowMessageAndWait(GARET, 0, 40);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(20);
  VenusAerie_ShowMessage(IVAN);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 40);
  VenusAerie_ShowMessage(MIA);
  Engine_ActorSetSpriteFlags(Object_GetById(GARET), 1);
  Engine_ActorSetAnimation(GARET, 1);
  Engine_ActorJump(GARET, 6, 60);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(20);
  Engine_ActorSetSpriteFlags(Object_GetById(IVAN), 1);
  Engine_ActorSetAnimation(IVAN, 1);
  Engine_ActorJump(IVAN, 6, 40);
  Engine_ActorSetSpriteFlags(Object_GetById(MIA), 1);
  Engine_ActorSetAnimation(MIA, 1);
  Engine_ActorJump(MIA, 6, 60);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 60);
  Engine_ActorSetAnimation(GARET, 4);
  VenusAerie_ShowMessage(GARET);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 20);
  Engine_ActorSetAnimation(IVAN, 4);
  VenusAerie_ShowMessage(IVAN);
  VenusAerie_FaceActor(MIA, FACING_SOUTHWEST);
  Engine_ActorSetAnimation(MIA, 4);
  Event_ShowMessageAndWait(MIA, 0, 80);
  VenusAerie_FaceActor(GARET, FACING_SOUTH);
  Event_ShowMessageAndWait(GARET, 0, 40);
  Actor_FaceDirection(IVAN, FACING_NORTH, 20);
  Event_ShowMessageAndWait(IVAN, 0, 40);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 20);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
  VenusAerie_ShowMessage(MIA);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 20);
  VenusAerie_FaceActor(GARET, FACING_SOUTH);
  VenusAerie_ShowMessage(GARET);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_SetSpeed(GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(GARET, 333, 194);
  Actor_WalkToAndWait(GARET, 333, 206);
  Actor_FaceDirection(GARET, FACING_EAST, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Event_ShowMessageAndWait(GARET, 0, 80);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_TaskWait(1);
  actor->scaleX = Q16_ONE;
  actor->scaleY = Q16_ONE;
  Engine_EventWait(40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 80);
  VenusAerie_FaceActor(GARET, FACING_SOUTHEAST);
  Engine_ActorJump(GARET, 4, 0);
  VenusAerie_ShowMessage(GARET);
  VenusAerie_FaceActor(GARET, FACING_NORTHEAST);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(GARET, 3);
    VenusAerie_ShowMessage(GARET);
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    VenusAerie_ShowMessage(IVAN);
    Engine_ActorSetAnimation(MIA, 4);
    VenusAerie_ShowMessage(MIA);
    stepPending = 1;
  } else {
    gEventWork->message += 3;
    Engine_EventWait(60);
    VenusAerie_ShowMessage(GARET);
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    VenusAerie_ShowMessage(IVAN);
    Engine_ActorSetAnimation(MIA, 4);
    VenusAerie_ShowMessage(MIA);
  }
  if (stepPending != 0) {
    gEventWork->message += 3;
  }
  VenusAerie_FaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
  Engine_EventWait(80);
  Event_PlayCue(17);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 20);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 20);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 40);
  Actor_FaceDirection(GARET, FACING_WEST, 20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 40);
  VenusAerie_ShowMessage(IVAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 20);
  Engine_ActorSetAnimation(GARET, 4);
  VenusAerie_ShowMessage(GARET);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 40);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 40);
  Actor_SetAttachedEffect(MIA, EMOTE_IN_FRONT | 2);
  Engine_EventWait(60);
  VenusAerie_ShowMessage(MIA);
  Event_PlayCue(141);
  Map_SetShake(PIXELS(6), PIXELS(6), Q16_ONE);
  Engine_EventWait(40);
  Event_PlayCue(145);
  Map_CopyCellsTo(110, 105, 74, 4, 18, 23);
  Map_CopyCellsTo(92, 86, 83, 4, 8, 23);
  Map_CopyCellsTo(75, 28, 75, 4, 8, 23);
  Map_CopyCellsTo(92, 86, 11, 72, 16, 20);
  Map_CopyCellsTo(19, 92, 19, 68, 8, 21);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->z.fixed -= PIXELS(32);
  actor->work = 0;
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(GARET);
  actor->x.fixed -= PIXELS(4);
  actor->z.fixed -= PIXELS(32);
  actor->work = 0;
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(IVAN);
  actor->x.fixed -= PIXELS(4);
  actor->z.fixed -= PIXELS(32);
  actor->work = 0;
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(MIA);
  actor->x.fixed -= PIXELS(4);
  actor->z.fixed -= PIXELS(18);
  actor->work = 0;
  VenusAerie_StopActorMotion(actor);
  Object_GetById(23)->y.fixed = PIXELS(56);
  Camera_MoveTo(PIXELS(338), PIXELS(32), PIXELS(180), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 0);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
  Engine_ActorJump(GARET, 6, 0);
  Engine_ActorJump(IVAN, 6, 0);
  Engine_ActorJump(MIA, 6, 0);
  // Shake the party and split away the remaining central floor tiles.
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gVenusAerie_PartyHeadingAction);
  Actor_SetActionScript(GARET, gVenusAerie_PartyHeadingAction);
  Actor_SetActionScript(IVAN, gVenusAerie_PartyHeadingAction);
  Actor_SetActionScript(MIA, gVenusAerie_PartyHeadingAction);
  Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Engine_EventWait(80);
  Map_SetShake(PIXELS(6), PIXELS(6), Q16_ONE);
  Engine_EventWait(40);
  Event_PlayCue(145);
  Map_CopyCellAttributes(110, 106, 18, 14, 10, 5);
  Map_CopyCellsTo(110, 105, 74, 4, 18, 23);
  Map_CopyCellsTo(92, 86, 11, 68, 16, 20);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->x.fixed -= PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(GARET);
  actor->x.fixed -= PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(IVAN);
  actor->x.fixed -= PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(MIA);
  actor->x.fixed -= PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(8);
  actor->x.fixed += PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(9);
  actor->x.fixed += PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(10);
  actor->x.fixed -= PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  actor = Object_GetById(11);
  actor->x.fixed -= PIXELS(16);
  VenusAerie_StopActorMotion(actor);
  Camera_MoveTo(PIXELS(322), PIXELS(32), PIXELS(180), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
  Actor_SetAttachedEffect(GARET, EMOTE_IN_FRONT | 2);
  Actor_SetAttachedEffect(IVAN, EMOTE_IN_FRONT | 2);
  Actor_SetAttachedEffect(MIA, EMOTE_IN_FRONT | 2);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
  Engine_ActorJump(GARET, 6, 0);
  Engine_ActorJump(IVAN, 6, 0);
  Engine_ActorJump(MIA, 6, 0);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_EventWait(80);
  Event_PlayCue(0x121);
  Engine_EventWait(40);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(IVAN);
  Engine_ActorStop(MIA);
  Engine_EventWait(120);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 120);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  VenusAerie_ShowMessage(IVAN);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  VenusAerie_ShowMessage(GARET);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
  Engine_ActorSetAnimation(MIA, 4);
  VenusAerie_ShowMessage(MIA);
  Engine_ActorJump(IVAN, 2, 20);
  VenusAerie_ShowMessage(IVAN);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  VenusAerie_FaceActor(GARET, FACING_EAST);
  VenusAerie_ShowMessage(GARET);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(20);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_SetActionScript(GARET, gVenusAerie_PartyExitAction);
  Actor_SetActionScript(IVAN, gVenusAerie_PartyExitAction);
  Actor_SetActionScript(MIA, gVenusAerie_PartyExitAction);
  Engine_EventWait(60);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 272, 216);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 272, 254);
  Engine_EventWait(80);
  ((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  ((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->transitionFrames = 16;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(80);
  Graphics_EnableObjLayerAndCallbacks();
  ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->speakerVoiceSound = 0;
  ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->speakerVoiceCountdown =
      0;
  UiText_ShowCenteredMessage(MsgVenusDespiteLongTiring, 0, 0);
  ObjectDispatch_StopCallbacksAndHideLayers();
  Engine_EventWait(80);
}

// Clear all movement targets and velocities and reset the actor timer.
static void VenusAerie_StopActorMotion(struct FieldActor *actor) {
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;

  actor->velocityX = 0;
  actor->velocityY = 0;
  actor->velocityZ = 0;

  actor->timer = 0;
}

// Advance the beacon rise sequence, its flashes and orbiting particles.
static void VenusAerie_UpdateBeaconRise(void) {
  struct FieldActor *center;
  struct FieldActor *effect;
  struct AnimationObject *sprite;
  s32 spawnParticles;
  u32 randomHeight;
  s32 angle;

  center = Actor_Get(23);
  spawnParticles = 0;
  switch (sTransitionStep) {
  case 0:
    Event_PlayCue(220);
    Map_SetShake(PIXELS(6), PIXELS(6), Q16_ONE);
    ColorBuffer_ApplyTarget(0x2063ff, 1);
    Engine_ColorBufferInterpolate(8);
    break;
  case 8:
    ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
    Engine_ColorBufferInterpolate(8);
    break;
  case 16:
    Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    break;
    // Start the lower beam beneath the floor, then pause each rise stage.

  case 24:
    center->x.fixed = PIXELS(304);
    center->y.fixed = -PIXELS(360);
    center->z.fixed = PIXELS(164);
    center->scaleX = Q16_ONE;
    center->scaleY = Q16_ONE;
    VenusAerie_StopActorMotion(center);
    Actor_SetActionScript(23, gVenusAerie_RiseActionScript);
    break;
  case 25:
    sTransitionStep--;
    if (center->y.fixed > 0) {
      ColorBuffer_ApplyTarget(0x203210, 0);
      Engine_ColorBufferInterpolate(16);
      sTransitionStep++;
      Actor_Get(0)->riseCounter = 1;
      Actor_Get(1)->riseCounter = 1;
      Actor_Get(2)->riseCounter = 1;
      Actor_Get(3)->riseCounter = 1;
      Actor_Get(ACTOR_FELIX)->riseCounter = 1;
      Actor_Get(SHEBA)->riseCounter = 1;
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlayCue(246);
      spawnParticles = 1;
      center->y.fixed += PIXELS(9) / 4;
    }
    break;
  case 26:
    sTransitionStep--;
    if (center->y.fixed > (PIXELS(40))) {
      ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
      Engine_ColorBufferInterpolate(40);
      sTransitionStep++;
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlayCue(246);
      spawnParticles = 1;
      center->y.fixed += PIXELS(9) / 4;
    }
    break;
  case 27:
  case 28:
  case 29:
  case 30:
  case 31:
  case 32:
  case 33:
  case 34:
    spawnParticles = 1;
    break;
  case 36:
    Event_PlayCue(187);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    Engine_ColorBufferInterpolate(12);
    break;
  case 48:
    Engine_ActorStop(23);
    GameFlag_Set(0x237);
    break;
  }
  if (spawnParticles) {
    randomHeight = ((u32)(Random_Next() * 80) >> 16) << 16;
    effect = Object_Create(284, center->x.fixed,
                           center->y.fixed - randomHeight + -PIXELS(8),
                           center->z.fixed);
    if (effect != 0) {
      sprite = effect->sprite;
      Object_SetScript(effect, gVenusAerie_RiseParticleScript);
      ObjectGroup_SetChildValue(effect, 1);
      effect->motionFlags = 0;
      angle = Random_Next() & 0xffff000;
      effect->timer = angle;
      effect->work = 0;
      effect->riseCounter = (u32)Random_Next() >> 13;
      effect->update = VenusAerie_UpdateRisingOrbit;
      effect->speed = Engine_MathSin((u32)(Random_Next() * 0xffff) >> 20) * 24;
      effect->speed = center->speed >> 16;
      sprite->flags = 0;
      sprite->image.priority = 1;
    }
  }
  sTransitionStep++;
  VenusAerie_AdvanceActorRise(Actor_Get(0));
  VenusAerie_AdvanceActorRise(Actor_Get(1));
  VenusAerie_AdvanceActorRise(Actor_Get(2));
  VenusAerie_AdvanceActorRise(Actor_Get(3));
  VenusAerie_AdvanceActorRise(Actor_Get(ACTOR_FELIX));
  VenusAerie_AdvanceActorRise(Actor_Get(SHEBA));
}

// Raise an enabled actor according to its gated rise counter.
static void VenusAerie_AdvanceActorRise(struct FieldActor *actor) {
  if (actor->riseEnabled != 0) {
    u8 counter = actor->riseCounter;

    actor->y.fixed = actor->riseBaseY + ((counter >> 2) << 16);

    VenusAerie_StopActorMotion(actor);

    if (actor->riseCounter != 0) {
      if (actor->riseCounter <= 31) {
        ++actor->riseCounter;
      }
    }
  }
}

// Emit particles beside the lower beam and link each to its source actor.
static void VenusAerie_SpawnSideOrbitParticles(void) {
  union FieldObject *center;
  struct FieldActor *effect;
  struct AnimationObject *sprite;
  struct MapScrollWork *map;
  s32 drift;
  s32 phase;
  s32 angle;

  center = OBJ_GET(23);
  map = gMapWork[0];
  drift = (u32)(Random_Next() * 48) >> 16 << 16;
  if ((s16)(map->viewY >> 16) <= 129) {
    if (gFrameCount & 1) {
      Actor_SetPosition(23, PIXELS(304), PIXELS(164));
      Actor_Get(23)->scaleX = Q16_ONE;
      Actor_Get(23)->scaleY = Q16_ONE;
    } else {
      Actor_SetPosition(23, PIXELS(304), PIXELS(171));
      Actor_Get(23)->scaleX = 13 * Q16_ONE / 10;
      Actor_Get(23)->scaleY = 13 * Q16_ONE / 10;
    }
  } else {
    Actor_SetPosition(23, 0, 0);
  }
  if (center == 0)
    return;
  phase = gFrameCount & 15;
  if (phase != 0)
    return;
  effect = Object_Create(284, center->actor.x.fixed + PIXELS(8),
                         center->actor.y.fixed + drift + PIXELS(8),
                         center->actor.z.fixed);
  drift /= PIXELS(6);
  drift <<= 16;
  if (effect == 0)
    return;
  sprite = effect->sprite;
  Object_SetScript(effect, gVenusAerie_BesideParticleScript);
  ObjectGroup_SetChildValue(effect, 5);
  effect->motionFlags = phase;
  angle = Random_Next() & 0xffff000;
  effect->timer = angle;
  effect->work = phase;
  effect->linkedObject = &center->actor;
  effect->update = VenusAerie_UpdateOrbitingEffect;
  effect->speed = Engine_MathSin((drift & 0xfffff) >> 4) * 24 >> 16;
  sprite->flags = 0;
  sprite->image.priority = center->actor.sprite->image.priority;
}

// Move a linked particle along its horizontal ellipse and preserve its target.
static void VenusAerie_UpdateOrbitingEffect(union FieldObject *object) {
  struct FieldActor *anchor;
  s32 spin;

  anchor = object->actor.linkedObject;
  spin = object->effect.spin;
  object->effect.x =
      anchor->x.fixed + Engine_MathCos(spin) * (object->actor.speed + 28);
  object->effect.z = Engine_MathSin(spin) * 16 + PIXELS(164);
  object->actor.targetX = object->effect.x;
  object->actor.targetZ = object->effect.z;
  object->effect.spin -= 0x200;
}

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gVenusAerie_PairActionScript[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(492), 0, PIXELS(164)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_WAIT(1),
    SC_END,
};

s32 gSceneAction_EntryGroup[] = {
    SC_SET(SW_SPEED, PIXELS(1) / 5),
    SC_SET(SW_ACCEL, PIXELS(1) / 10),
    SC_ARG(2),
    SC_MOVE_BY(-PIXELS(8), 0, 0),
    SC_SYNC,
    SC_END,
};

s32 gSceneAction_EntryPair[] = {
    SC_SET(SW_SPEED, PIXELS(1) / 5),
    SC_SET(SW_ACCEL, PIXELS(1) / 10),
    SC_ARG(2),
    SC_MOVE_BY(PIXELS(8), 0, 0),
    SC_SYNC,
    SC_END,
};

s32 gVenusAerie_ScaleStepAction[] = {
    SC_CALL(VenusAerie_PulseActorScale),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gVenusAerie_BeamOpenAction[] = {
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_WAIT(1),
    SC_LOOP(32, SC_TOP),
    SC_END,
};

s32 gVenusAerie_BeamNarrowAction[] = {
    SC_ADD(SW_SCALE_X, -896),
    SC_WAIT(1),
    SC_LOOP(140, SC_TOP),
    SC_END,
};

s32 gVenusAerie_BeamRiseAction[] = {
    SC_ADD(SW_SCALE_X, -896),
    SC_WAIT(1),
    SC_LOOP(160, SC_TOP),
    SC_END,
};

s32 gVenusAerie_BeamCloseAction[] = {
    SC_ADD(SW_Y, PIXELS(4)), SC_WAIT(1), SC_LOOP(30, SC_TOP),
    SC_POS(0, 0, 0),         SC_WAIT(1), SC_END,
};

s32 gVenusAerie_BeamEndAction[] = {
    SC_ADD(SW_Y, PIXELS(2)), SC_WAIT(1), SC_LOOP(160, SC_TOP),
    SC_POS(0, 0, 0),         SC_WAIT(1), SC_END,
};

s32 gVenusAerie_OrbitParticleScript[] = {
    SC_SET(SW_SCALE_X, 4 * Q16_ONE / 5),
    SC_SET(SW_SCALE_Y, 4 * Q16_ONE / 5),
    SC_ARG(1),
    SC_LABEL(1),
    SC_ADD(SW_Y, PIXELS(1)),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(1)),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gVenusAerie_RiseParticleScript[] = {
    SC_SET(SW_SCALE_X, 4 * Q16_ONE / 5),
    SC_SET(SW_SCALE_Y, 4 * Q16_ONE / 5),
    SC_ARG(1),
    SC_LABEL(1),
    SC_ADD(SW_Y, -PIXELS(1)),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(1)),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gVenusAerie_WalkOutAction[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(268), 0, PIXELS(186)),
    SC_SYNC,         SC_MOVE(PIXELS(268), 0, PIXELS(252)),
    SC_SYNC,         SC_MOVE(PIXELS(232), 0, PIXELS(222)),
    SC_SYNC,         SC_MOVE(PIXELS(232), 0, PIXELS(136)),
    SC_SYNC,         SC_MOVE(PIXELS(200), 0, PIXELS(136)),
    SC_SYNC,         SC_MOVE(PIXELS(200), 0, PIXELS(200)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_WAIT(1),
    SC_END,
};

s32 gVenusAerie_RiseActionScript[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -1536),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, 1536),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gVenusAerie_StepBackAction[] = {
    SC_ADD(SW_Y, -(PIXELS(3) / 4)),
    SC_WAIT(1),
    SC_LOOP(120, SC_TOP),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gVenusAerie_LeapAction[] = {
    SC_ADD(SW_Y, -(PIXELS(9) / 2)),
    SC_WAIT(1),
    SC_LOOP(80, SC_TOP),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gVenusAerie_PartyExitAction[] = {
    SC_ARG(2), SC_MOVE(PIXELS(330), 0, PIXELS(173)), SC_SYNC, SC_FREE, SC_END,
};

s32 gVenusAerie_PartyHeadingAction[] = {
    SC_CALL(OverlayObject_UpdateHeadingTimer),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gVenusAerieEntrances[] = {
    {0, CONDITION_ALWAYS, 304, 0, 216, FACING_SOUTHEAST + FACING_STEP, 0, 0, 0,
     600, 420, 0},
    {1, CONDITION_ALWAYS, 504, 0, 168, FACING_SOUTHEAST + FACING_STEP, 0, 0, 0,
     600, 420, 0},
    {2, CONDITION_ALWAYS, 340, 0, 184, FACING_WEST, 0, 0, 0, 600, 420, 0},
    {3, CONDITION_ALWAYS, 340, 0, 184, FACING_WEST, 0, 0, 0, 600, 420, 0},
    {9, CONDITION_ALWAYS, 0, 0, 0, FACING_WEST, 0, 0, 0, 600, 420, 0},
    {SCENE_TABLE_END},
};

u32 gVenusAerieExits[] = {
    SCENE_EXITS(187),       SCENE_EXIT(1, 2, 73), SCENE_EXIT(2, 180, 99),
    SCENE_EXIT(9, 179, 12), SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gVenusAeriePlacements[] = {
    { 0x102, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x102, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x102, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x102, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf2, CONDITION_ALWAYS, 7,
        PIXELS(440), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf2, CONDITION_ALWAYS, 7,
        PIXELS(472), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf2, CONDITION_ALWAYS, 7,
        PIXELS(504), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf2, CONDITION_ALWAYS, 7,
        PIXELS(104), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf2, CONDITION_ALWAYS, 7,
        PIXELS(136), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf2, CONDITION_ALWAYS, 7,
        PIXELS(168), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf2, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(504), 0, PIXELS(148), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(504), 0, PIXELS(148), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(504), 0, PIXELS(148), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x21, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(164), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x20, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(180), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(172), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x23, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(252), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf4, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe5, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe5, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe5, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe5, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe5, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};
// clang-format on

s32 gVenusAerie_NearestActor = 0;

struct SceneEvent gVenusAerieEvents[] = {
    {0x202, 11, CONDITION_ALWAYS, EVENT_SCRIPT(VenusAerie_TryLeaderJump)},
    {EVENT_TOUCH, 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(VenusAerie_EnablePillarGravity)},
    {0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(VenusAerie_TryLeaderJump)},
    {EVENT_TOUCH, 10, 0x250, EVENT_SCRIPT(VenusAerie_BouncePillarUnderLeader)},
    {EVENT_TOUCH, 5, CONDITION_ALWAYS,
     EVENT_SCRIPT(VenusAerie_ConfrontSaturosAndMenardi)},
    {SCENE_EVENTS_END},
};

s32 gVenusAerie_BesideParticleScript[] = {
    SC_SET(SW_SCALE_Y, 7 * Q16_ONE / 5),
    SC_SET(SW_SCALE_X, 7 * Q16_ONE / 5),
    SC_ARG(4),
    SC_WAIT(30),
    SC_FREE,
};
