#include "game_state.h"
#include "unit/import.h"
#include "unit/scene/xian.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"

#define NULL ((void *)0)
#include "field_obj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "scene_effect_sequence.h"
#include "iwram_call.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "call.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "flags.h"

struct PeriodicEffectConfig {
  s32 kind;
  s32 variant;
  u8 pad08[16];
  u16 id;
  u8 pad1a[2];
  u8 *data;
  u8 pad20[8];
};

void Effect_Spawn();

// Shared source is included here in ROM order.
#include "common/effect_create.h"
#include "common/effect_spawn.h"

// Temporary flags 00-03 track spilled water, frozen water,
// the kung fu demonstration and Force used on the training tree.
#define WATER_CARRIER 18
#define TRAINING_TREE 19
#define WATER_PUDDLE 20
#define FORCE_TARGET 21
#define VILLAGE_DJINNI 22
#define KUNG_FU_STUDENT 14
#define WATER_CARRIER_WAIT_TRIGGER 200
#define WATER_CARRIER_WAIT_FRAMES 120

struct SpawnParams {
  s32 count;
  u8 unk04[20];
  u16 sprite;
  u16 pad1a;
  s32 callback;
  u16 facing;
  u8 unk22[6];
};

// Recruit the village Djinni when the party reaches it.
static void Xian_RecruitVillageDjinni(void) {
  Djinn_JoinParty(VILLAGE_DJINNI, 1, 2);
}

// Select underfoot layering for the frozen puddle when the leader is raised.
static void Xian_UpdateFrozenPuddleLayer(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader->y.part.pixel > 31) {
    Object_GetById(WATER_PUDDLE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  } else {
    Object_GetById(WATER_PUDDLE)->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  }
}

// Rotate the log during the student's strike and Force animation.
static void Xian_RotateTrainingTree(void) {
  struct FieldActor *tree = Object_GetById(TRAINING_TREE);
  struct FieldSprite *sprite = tree->sprite;
  sprite->rotation += 0x1400;
}

// Emit particles beside the student every fourth frame.
// The existing byte view keeps the Z load before the options-data store;
// typed FieldActor reads reorder three instructions in the English build.
static void Xian_SpawnTrainingParticles(void) {
  u8 *studentActor = Object_GetById(KUNG_FU_STUDENT);

  if ((gFrameCount & 3) == 0) {
    struct PeriodicEffectConfig config;
    config.kind = 1;
    config.variant = 9;
    config.id = 169;
    config.data = gXian_EffectConfig;
    Effect_Spawn(*(s32 *)(studentActor + 8), *(s32 *)(studentActor + 12),
                 *(s32 *)(studentActor + 16) - 0x10000, 0, -0x10000, -0x10000,
                 0x330000, &config);
  }
}

// Shared source is included here in ROM order.
#include "common/object_distance.h"

// Track a nearby target only within the actor's current facing sector.
static s32 Xian_UpdateCarrierTracking(struct FieldActor *actor,
                                      struct FieldActor *target,
                                      s32 distanceLimit, s32 forceTracking) {
  s32 isTracking = 0;

  // This route uses paused/timer as its tracking latch and mode.
  if (actor->paused == 1 && actor->timer == 0) {
    Object_SetMode(actor, 1);
    return 1;
  }

  if (FixedPoint_Distance(&target->x.fixed, &actor->x.fixed) < distanceLimit ||
      forceTracking != 0) {
    u16 angle = ArcTan2(target->z.fixed - actor->z.fixed,
                        target->x.fixed - actor->x.fixed);
    s32 previousHeading = (angle - 0x1000) & 0xf000;
    s32 nextHeading = (angle + 0x1000) & 0xf000;
    s32 targetHeading = angle & 0xf000;
    s32 currentHeading = actor->facing & 0xf000;

    if (targetHeading == currentHeading || nextHeading == currentHeading ||
        previousHeading == currentHeading || forceTracking != 0) {
      actor->paused = 1;
      Object_SetMode(actor, 1);
      isTracking = 1;
      actor->timer = isTracking;
    } else {
      actor->paused = 0;
      Object_SetMode(actor, 2);
      actor->timer = 0;
    }
  } else {
    actor->paused = 0;
    Object_SetMode(actor, 2);
    actor->timer = 0;
  }

  return isTracking;
}

// Ask the carrier to move after the leader blocks her route for two seconds.
// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 154 to 150 bytes in tbs-en.
#ifdef UBFIX
static void Xian_WatchWaterCarrierPath(union FieldObject *fieldObject) {
#else
static s32 Xian_WatchWaterCarrierPath(union FieldObject *fieldObject) {
#endif
  struct FieldActor *self = (struct FieldActor *)fieldObject;
  struct FieldActor *leader;
  s32 near = 0;

  if (self->targetX == ACTOR_NO_TARGET && self->targetZ == ACTOR_NO_TARGET)
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  leader = Object_GetById(0);
  if ((u32)((leader->x.fixed >> 20) - 17) <= 1 && leader->z.fixed >> 20 == 14 &&
      self->x.fixed >> 20 <= 19 && self->velocityX <= 0) {
    if (leader->x.fixed <= self->x.fixed) {
      self->riseCounter++;
      near = 1;
    }
  } else {
    self->riseCounter = 0;
  }
  if (near && self->riseCounter > WATER_CARRIER_WAIT_FRAMES - 1) {
    gEventWork->raisedTrigger = WATER_CARRIER_WAIT_TRIGGER;
    self->riseCounter = 0;
  }
  Xian_UpdateCarrierTracking(self, leader, 18, near);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

// Keep the native callback representation beside its modern signature choice.
#ifdef UBFIX
#define XIAN_WATER_CARRIER_UPDATE Xian_WatchWaterCarrierPath
#else
#define XIAN_WATER_CARRIER_UPDATE ((void *)Xian_WatchWaterCarrierPath)
#endif

// Return the village entrance table through the overlay entry interface.
u8 *SceneEffect_GetPrimaryData(void) { return gXianEntrances; }

// This scene has no additional region table.
s32 SceneEffect_GetInitialValue(void) { return 0; }

// Return the village exit table through the overlay entry interface.
u8 *SceneEffect_GetSecondaryData(void) { return gXianExits; }

// Prepare actor placements after the party hears that Hsu is trapped.
s32 SceneEffect_PrepareState(void) {
  if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) != 0)
    gXianPlacements[7].talkFacing = 0;
  return (s32)gXianPlacements;
}

// Ask the visitors whether they are young warriors.
static void Xian_AskAboutYoungWarriors(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgXianYoungWarriorsVeryGallantCame);
  Engine_EventAskYesNo(9, 0);
  Engine_EventEnd();
}

// Have the water carrier spill her bucket and return to refill it.
static void Xian_SpillCarriedWater(void) {
  struct FieldActor *puddle;
  struct FieldActor *carrier;
  struct FieldActor *leader;
  s32 leaderMustStepAside;
  s32 savedFacing;
  s32 carrierX;
  s32 carrierCellZ;
  s32 carrierColumnX;
  s32 scaleY;
  s32 scaleX;

  puddle = Object_GetById(WATER_PUDDLE);
  Engine_EventBegin();
  leaderMustStepAside = 0;
  carrier = Actor_Get(WATER_CARRIER);
  carrier->update = NULL;
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) != 0 ||
      (Object_GetById(WATER_CARRIER)->x.fixed >> 20) <= 19) {
    carrier = Actor_Get(WATER_CARRIER);
    savedFacing = carrier->facing;
    Actor_FaceActor(WATER_CARRIER, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgXianNowMustGetWaterAgain);
    if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) == 0) {
      gEventWork->message++;
      Event_ShowMessage(WATER_CARRIER, 0);
      Object_GetById(WATER_CARRIER)->timer = 0;
      carrier = Object_GetById(WATER_CARRIER);
      carrier->facing = savedFacing;
    } else {
      Event_ShowMessage(WATER_CARRIER, 0);
      Actor_FaceDirection(WATER_CARRIER, 0x8000, 20);
    }
    carrier = Actor_Get(WATER_CARRIER);
    carrier->update = XIAN_WATER_CARRIER_UPDATE;
    Engine_EventEnd();
    return;
  }
  // Make room for the carrier before spilling the water.
  if ((Object_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 19) > 27 &&
      (Object_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 19) <= 29 &&
      (Object_GetById(ACTOR_PARTY_LEADER)->x.fixed >> 20) != 26) {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Engine_ActorFaceActor(0, WATER_CARRIER, 0);
    Engine_EventWait(5);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    carrier = Actor_Get(WATER_CARRIER);
    if (leader->x.fixed < carrier->x.fixed) {
      Object_GetById(0)->steeringFlags &= ~ACTOR_FACES_MOTION;
      carrier = Object_GetById(WATER_CARRIER);
      Actor_WalkTo(ACTOR_PARTY_LEADER, ((carrier->x.fixed >> 20) << 4) - 8,
                   232);
      leaderMustStepAside = 1;
    } else {
      Object_GetById(0)->steeringFlags &= ~ACTOR_FACES_MOTION;
      carrier = Object_GetById(WATER_CARRIER);
      Actor_WalkTo(ACTOR_PARTY_LEADER, ((carrier->x.fixed >> 20) << 4) + 24,
                   232);
    }
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  }
  carrier = Actor_Get(WATER_CARRIER);
  carrier->targetX = ACTOR_NO_TARGET;
  carrier = Object_GetById(WATER_CARRIER);
  carrier->targetY = ACTOR_NO_TARGET;
  carrier = Actor_Get(WATER_CARRIER);
  carrier->targetZ = ACTOR_NO_TARGET;
  Actor_SetBuiltinAction(WATER_CARRIER, ACTOR_ACTION_IDLE);
  Engine_ActorSetAnimation(WATER_CARRIER, 1);
  Engine_ActorRunRepeatedMotion(WATER_CARRIER, 2);
  Engine_EventWait(10);
  Event_PlayCue(228);
  // Copy the wet terrain cell and grow the puddle into view.
  puddle->scaleX = 0x4ccc;
  puddle->scaleY = 0x4ccc;
  carrier = Object_GetById(WATER_CARRIER);
  carrierX = carrier->x.fixed;
  carrier = Object_GetById(WATER_CARRIER);
  carrierCellZ = carrier->z.fixed >> 20;
  Actor_SetPosition(WATER_PUDDLE, ((carrierX >> 20) << 20) + (8 << 16),
                    (carrierCellZ << 20) + (8 << 16));
  carrier = Object_GetById(WATER_CARRIER);
  carrierColumnX = carrier->x.fixed;
  carrier = Object_GetById(WATER_CARRIER);
  Map_CopyCellAttributes(16, 16, 1, 1, carrierColumnX >> 20,
                         carrier->z.fixed >> 20);
  Engine_ActorSetSpritePriority(WATER_PUDDLE, 2);
  puddle->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  do {
    Engine_TaskWait(3);
    scaleY = puddle->scaleY;
    scaleX = puddle->scaleX;
    puddle->scaleY = scaleY + 0x1999;
    scaleX += 0x1999;
    puddle->scaleX = scaleX;
  } while (scaleX <= 0xffff);
  Actor_ShowEmote(WATER_CARRIER, 0x105, 70);
  Actor_FaceActor(WATER_CARRIER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(WATER_CARRIER, 0x103, 0);
  Engine_ActorStartRepeatedMotion(WATER_CARRIER, 2);
  Engine_EventWait(70);
  Engine_EventSetMessage(MsgXianDoingMadeMeSpillMy);
  Event_ShowMessageAndWait(WATER_CARRIER, 0, 20);
  BattleFx_PlayQueuedSound();
  if ((Object_GetById(ACTOR_PARTY_LEADER)->x.fixed >> 20) == 26 &&
      (Object_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 20) > 13) {
    leaderMustStepAside = 1;
  }
  // Clear the carrier's route back to the water supply.
  if (leaderMustStepAside != 0) {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 10);
    Object_GetById(0)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  }
  Actor_SetSpeed(WATER_CARRIER, 0xcccc, 0x6666);
  carrier = Object_GetById(WATER_CARRIER);
  if ((carrier->z.fixed >> 20) != 14) {
    carrier = Actor_Get(WATER_CARRIER);
    Actor_WalkToAndWait(WATER_CARRIER, carrier->x.part.pixel, 232);
  }
  Actor_WalkToAndWait(WATER_CARRIER, 0x118, 232);
  GameFlag_Set(FLAG_SCENE_TEMP_00);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventEnd();
}

// Set puddle sprite priority when the leader approaches from the north.
static void Xian_UpdatePuddlePriority(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  if ((leader->z.fixed >> 20) <= 13)
    Engine_ActorSetSpritePriority(WATER_PUDDLE, 1);
}

// Make the frozen puddle solid and keep its foreground visibility updated.
static void Xian_FreezeSpilledWater(void) {
  struct FieldActor *puddle;
  s32 stationaryMotionFlags;
  s32 puddleX;

  Engine_EventBegin();
  Object_GetById(WATER_PUDDLE)->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  stationaryMotionFlags = 0;
  Object_GetById(WATER_PUDDLE)->motionFlags = stationaryMotionFlags;
  puddle = Object_GetById(WATER_PUDDLE);
  puddleX = puddle->x.fixed;
  puddle = Object_GetById(WATER_PUDDLE);
  Map_CopyCellAttributes(3, 17, 1, 1, (puddleX >> 20), (puddle->z.fixed >> 20));
  FieldScheduler_AddCallback(Xian_UpdateFrozenPuddleLayer, 0xc80);
  GameFlag_Set(FLAG_SCENE_TEMP_01);
  Engine_ActorSetSpritePriority(WATER_PUDDLE, 2);
  Engine_EventEnd();
}

// Play the kung fu demonstration once, then discuss the student's strength.
static void Xian_WatchKungFuDemonstration(void) {
  s32 demonstrationSeen;
  struct FieldActor *leader;
  SchedulerCallback particleCallback;
  s32 reboundVelocity;
  s32 stepBackOffset[3];
  struct SpawnParams effectParams;

  Engine_EventBegin();
  demonstrationSeen = Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02);
  if (demonstrationSeen != 0) {
    Engine_ActorFaceActor(KUNG_FU_STUDENT, 0, 0);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgXianKungFuStrong);
    Engine_EventShowMessage(KUNG_FU_STUDENT, 0);
    Engine_ActorFaceDirection(KUNG_FU_STUDENT, 0, 10);
    Engine_EventEnd();
  } else {
    // Step back, then watch the student jump and strike the tree.
    Engine_EventSetMessage(MsgXianHooWhaaaHachaaa);
    Engine_EventShowMessage(KUNG_FU_STUDENT, 0);
    Engine_ActorRunRepeatedMotion(0, 2);
    Object_GetById(0)->steeringFlags &= ~ACTOR_FACES_MOTION;
    stepBackOffset[0] = demonstrationSeen;
    stepBackOffset[1] = demonstrationSeen;
    stepBackOffset[2] = demonstrationSeen;
    leader = Object_GetById(0);
    FieldVector_AddPolarOffset(-0x80000, leader->facing,
                               (struct FieldPosition *)stepBackOffset);
    Engine_ActorSetAnimation(0, 2);
    Engine_ActorSetDestinationOffset(0, stepBackOffset[0] / 0x10000,
                                     stepBackOffset[2] / 0x10000);
    Engine_ActorWaitForMove(0);
    Object_GetById(0)->steeringFlags |= ACTOR_FACES_MOTION;
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(KUNG_FU_STUDENT, 2);
    effectParams.count = 1;
    Effect_Spawn(0xc00000, 0, 0x1380000, 0x1999, 0x3333, 0, 0x20001, 0);
    Effect_Spawn(0xc00000, 0, 0x1380000, 0x3333, 0x1999, 0, 0x20001, 0);
    Engine_AudioPlayCue(132);
    particleCallback = Xian_SpawnTrainingParticles;
    Task_AddCallback(particleCallback, 0xc80);
    Object_GetById(KUNG_FU_STUDENT)->velocityY = 0x60000;
    Object_GetById(KUNG_FU_STUDENT)->gravity = 0x10000;
    Object_GetById(KUNG_FU_STUDENT)->bounce = 0;
    Actor_SetSpeed(KUNG_FU_STUDENT, 0x30000, 0x18000);
    Actor_SetDestination(KUNG_FU_STUDENT, 168, 0x138);
    Engine_ActorWaitForMove(KUNG_FU_STUDENT);
    Engine_AudioPlayCue(134);
    Actor_SetActionScript(TRAINING_TREE, gXian_Actor19Motion);
    FieldScheduler_AddCallback(Xian_RotateTrainingTree, 0xc80);
    effectParams.sprite = 0x11b;
    effectParams.callback = (s32)gXian_EffectScript;
    effectParams.facing = 0x4000;
    Effect_Spawn(0xa80000, 0, 0x14c0000, 0, 0, 0, 0x720000, &effectParams);
    Actor_SetDestination(KUNG_FU_STUDENT, 146, 0x138);
    Engine_ActorWaitForMove(KUNG_FU_STUDENT);
    // Finish the impact effects and leave the student in the final pose.
    Engine_TaskRemoveCallback((void (*)(void))particleCallback);
    Effect_Spawn(0x900000, 0, 0x1380000, 0, 0, 0, 0x20001, 0);
    Effect_Spawn(0x900000, 0, 0x1380000, -0x3333, 0x1999, 0, 0x20001, 0);
    reboundVelocity = -0x8000;
    Effect_Spawn(0x900000, 0, 0x1380000, reboundVelocity, 0, 0, 0x20001, 0);
    Object_RefreshSelectorById(TRAINING_TREE);
    Engine_AudioPlayCue(124);
    Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0, 0, 0, 0x20001, 0);
    Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0x3333, 0, 0, 0x20001, 0);
    Effect_Spawn(0xa80000, 0x80000, 0x1380000, -0x3333, 0, 0, 0x20001, 0);
    Engine_TaskRemoveCallback(Xian_RotateTrainingTree);
    Object_GetById(TRAINING_TREE)->sprite->rotation = reboundVelocity;
    Object_GetById(KUNG_FU_STUDENT)->bounce = 0x4000;
    Object_GetById(KUNG_FU_STUDENT)->gravity = 0x10000;
    Engine_EventWait(30);
    Engine_ActorFaceDirection(KUNG_FU_STUDENT, 0, 20);
    Engine_ActorRunRepeatedMotion(KUNG_FU_STUDENT, 2);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(KUNG_FU_STUDENT, 0, 20);
    Engine_GameFlagSet(FLAG_SCENE_TEMP_02);
    BattleFx_PlayQueuedSound();
    Engine_EventEnd();
  }
}

// Ask whether the visitors are warriors from the school.
static void Xian_AskAboutSchoolWarriors(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgXianWarriorsFromSchoolStrongWarriors);
  Engine_EventAskYesNo(17, 0);
  Engine_EventEnd();
}

// Hop across the frozen puddle when the destination is unobstructed.
static void Xian_HopLeaderAhead(void) {
  s32 savedMotionFlags;
  struct FieldActor *leader;
  union FieldCoordinate landingPosition[3];
  union FieldCoordinate *landing;

  leader = Object_GetById(0);
  savedMotionFlags = leader->motionFlags;
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00) != 0) {
    landing = landingPosition;
    landing[0].fixed = (leader->x.fixed & -0x100000) + 0x80000;
    landing[1].fixed = leader->y.fixed;
    landing[2].fixed = (leader->z.fixed & -0x100000) + 0x80000;
    FieldVector_AddPolarOffset(0x200000, (leader->facing + 0x2000) & 0xc000,
                               (struct FieldPosition *)landing);
    if (FieldObject_CheckMovementCollision(leader, landing) == 0) {
      Engine_EventBegin();
      Object_SetMode(leader, 6);
      Engine_TaskWait(6);
      Engine_AudioPlayCue(152);
      Object_SetMode(leader, 7);
      leader->speed = 0x30000;
      leader->acceleration = 0x20000;
      leader->velocityY = 0x40000;
      leader->motionFlags &= 126;
      Engine_ActorSetSpriteFlags(leader, 0);
      Engine_ObjectMotionSetPositionAndCommit(0, landing[0].part.pixel,
                                              landing[2].part.pixel);
      Object_SetMode(leader, 6);
      Engine_ActorSetSpriteFlags(leader, 1);
      leader->motionFlags = savedMotionFlags;
      Engine_EventEnd();
    }
  }
}

// Walk through a village doorway and request its exit.
static void Xian_WalkThroughExit(s32 exitId) {
  Object_GetById(0)->motionFlags = 0;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  if (exitId == 6) {
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  } else {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  }
  gEventWork->transitionFrames = 16;
  Engine_EventRequestExit(exitId);
}

// Animate the selected door before the party walks through it.
static void Xian_OpenDoorAndLeave(void) {
  struct EventWork *event = gEventWork;

  Engine_EventBegin();

  switch (event->touchedTrigger) {
  case 1:
    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(gXian_GateSteps1, 81, 18);
    break;
  case 2:
    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(gXian_GateSteps2, 83, 11);
    break;
  case 3:
    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(gXian_GateSteps2, 86, 11);
    break;
  case 4:
    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(gXian_GateSteps3, 84, 24);
    break;
  case 5:
    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(gXian_GateSteps3, 72, 7);
    break;
  case 6:
    Engine_AudioPlayCue(188);
    Engine_MapAnimateCells(gXian_GateSteps4, 69, 11);
    break;
  case 7:
    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(gXian_GateSteps5, 83, 7);
    break;
  default:
    break;
  }

  Xian_WalkThroughExit(event->touchedTrigger);
  Engine_EventEnd();
}

// Let Force strike the training tree and surprise the student.
static void Xian_StrikeTrainingTreeWithForce(void) {
  SchedulerCallback rotationCallback;

  Engine_EventBegin();
  Actor_SetActionScript(TRAINING_TREE, gXian_Actor19Motion);
  rotationCallback = Xian_RotateTrainingTree;
  FieldScheduler_AddCallback(rotationCallback, 0xc80);
  Object_RefreshSelectorById(TRAINING_TREE);
  Engine_AudioPlayCue(124);
  Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0, 0, 0, 0x20001, 0);
  Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0x3333, 0, 0, 0x20001, 0);
  Effect_Spawn(0xa80000, 0x80000, 0x1380000, -0x3333, 0, 0, 0x20001, 0);
  Engine_TaskRemoveCallback(rotationCallback);
  Object_GetById(TRAINING_TREE)->sprite->rotation = 0x8000;
  Engine_ActorSetPosition(FORCE_TARGET, 0xa80000, 0x1380000);
  Engine_EventWait(20);
  Engine_ActorFaceActor(KUNG_FU_STUDENT, TRAINING_TREE, 0);
  Engine_ActorRunRepeatedMotion(KUNG_FU_STUDENT, 2);
  Engine_EventWait(10);
  Engine_EventSetMessage(MsgXianWhaWhaHappened);
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_03)) {
    gEventWork->message++;
  }
  Engine_EventShowMessage(KUNG_FU_STUDENT, 0);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_03);
  BattleFx_PlayQueuedSound();
  Engine_EventEnd();
}

// Move the party and carrier aside, then restart the carrier's route.
static void Xian_MoveAsideForWaterCarrier(void) {
  struct FieldActor *carrier;

  Engine_EventBegin();
  Actor_SetBuiltinAction(WATER_CARRIER, ACTOR_ACTION_IDLE);
  carrier = Actor_Get(WATER_CARRIER);
  carrier->update = 0;
  carrier = Actor_Get(WATER_CARRIER);
  carrier->targetX = ACTOR_NO_TARGET;
  carrier = Object_GetById(WATER_CARRIER);
  carrier->targetZ = ACTOR_NO_TARGET;
  carrier = Object_GetById(WATER_CARRIER);
  carrier->velocityX = 0;
  carrier = Object_GetById(WATER_CARRIER);
  carrier->velocityZ = 0;
  carrier = Object_GetById(WATER_CARRIER);
  carrier->speed = 0;
  carrier = Actor_Get(WATER_CARRIER);
  carrier->acceleration = 0;
  Actor_ShowEmote(WATER_CARRIER, 0x103, 0);
  Engine_ActorStartRepeatedMotion(WATER_CARRIER, 2);
  Engine_EventWait(60);
  Actor_SetSpeed(WATER_CARRIER, 0x18000, 0xc000);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
  Actor_WalkTo(WATER_CARRIER, 0x118, 232);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x128, 232);
  Engine_ActorWaitForMove(WATER_CARRIER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
  Actor_SetActionScript(WATER_CARRIER, gXian_ActionTable);
  carrier = Actor_Get(WATER_CARRIER);
  carrier->update = XIAN_WATER_CARRIER_UPDATE;
  Engine_EventEnd();
}

// Return the village event table through the overlay entry interface.
u8 *SceneEffect_GetTertiaryData(void) { return gXianEvents; }

// Restore story-dependent actors, puzzle state and training-tree transforms.
s32 Xian_SetupScene(void) {
  struct FieldActor *actor;
  u32 decorationIndex;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  BattleFx_SetQueuedSoundAndPlay(169);
  if (gGameState.entrance > 9) {
    Engine_GameFlagClear(0x12f);
  }
  if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED)) {
    Actor_FaceDirection(13, 0x8000, 0);
    Actor_SetPosition(KUNG_FU_STUDENT, 0x920000, 0x1380000);
    Engine_ActorFaceDirection(KUNG_FU_STUDENT, 0, 0);
    if (Engine_GameFlagIsSet(FLAG_FEIZHI_WENT_TO_HSU)) {
      Engine_ActorSetPosition(17, 0, 0);
    }
  }
  if (Engine_GameFlagIsSet(0x8b0)) {
    Engine_ActorSetPosition(17, 0, 0);
  }
  // Keep the three decorative actors below the moving characters.
  for (decorationIndex = 0; decorationIndex <= 2; decorationIndex++) {
    struct FieldActor *actor;
    actor = Object_GetById(decorationIndex + 23);
    actor->sprite->priority = 1;
    actor->motionFlags = 0;
    actor->collisionFlags = 8;
    Engine_ActorSetSpriteFlags(actor, 0);
    ObjectGroup_SetChildValue(actor, 15);
    actor->priorityFlags = (actor->priorityFlags & 254) | 2;
  }
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02)) {
    Actor_SetPosition(KUNG_FU_STUDENT, 0x920000, 0x1380000);
    Engine_ActorFaceDirection(KUNG_FU_STUDENT, 0, 0);
  }
  // Restore the frozen puddle and callbacks when re-entering the village.
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01)) {
    Engine_ActorSetAnimation(WATER_PUDDLE, 5);
    {
      s32 puddleX = Object_GetById(WATER_PUDDLE)->x.fixed;

      Engine_MapCopyCellAttributes(3, 17, 1, 1, puddleX >> 20,
                                   Object_GetById(WATER_PUDDLE)->z.fixed >> 20);
    }
    FieldScheduler_AddCallback(Xian_UpdateFrozenPuddleLayer, 0xc80);
  }
  Engine_ActorSetChildValue(WATER_CARRIER, 2);
  Object_GetById(WATER_CARRIER)->update = XIAN_WATER_CARRIER_UPDATE;
  actor = Object_GetById(TRAINING_TREE);
  actor->motionFlags = 0;
  actor->y.fixed = 0x100000;
  actor->targetY = 0x100000;
  actor->scaleX = 0x8ccc;
  actor->scaleY = 0x6666;
  actor->sprite->rotation = 0x8000;
  Engine_ActorSetSpriteFlags(Object_GetById(FORCE_TARGET), 0);
  Object_GetById(FORCE_TARGET)->motionFlags = 0;
  Object_GetById(FORCE_TARGET)->y.fixed = 0;
  Object_GetById(FORCE_TARGET)->targetY = -0x80000000;
  return 0;
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

s32 gXian_ActionTable[] = {
    SC_SET(SW_SPEED, 0x6666),
    SC_SET(SW_ACCEL, 0x1999),
    SC_WAIT(60),
    SC_LABEL(1),
    SC_ARG(3),
    SC_WAIT(60),
    SC_ARG(3),
    SC_WAIT(180),
    SC_ARG(2),
    SC_MOVE(PIXELS(424), 0, PIXELS(232)),
    SC_SYNC,
    SC_MOVE(PIXELS(424), 0, PIXELS(216)),
    SC_SYNC,
    SC_WAIT(10),
    SC_SET(SW_FACING, 0x1000),
    SC_WAIT(10),
    SC_ARG(3),
    SC_WAIT(60),
    SC_ARG(3),
    SC_WAIT(120),
    SC_ARG(2),
    SC_MOVE(PIXELS(424), 0, PIXELS(232)),
    SC_SYNC,
    SC_MOVE(PIXELS(280), 0, PIXELS(232)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gXian_Actor19Motion[] = {
    SC_SET(SW_SNAP, 0),
    SC_SET(SW_SPEED, 0x30000),
    SC_SET(SW_ACCEL, 0x18000),
    SC_MOVE(PIXELS(168), PIXELS(48), PIXELS(312)),
    SC_SYNC,
    SC_SET(SW_SPEED, 0x30000),
    SC_SET(SW_ACCEL, 0x1999),
    SC_SET(SW_SNAP, 1),
    SC_MOVE(PIXELS(168), PIXELS(16), PIXELS(312)),
    SC_SYNC,
    SC_END,
};

s32 gXian_EffectScript[] = {
    SC_WAIT(18), SC_WAIT(1), SC_POS(0, 0, 0), SC_WAIT(1), SC_FREE,
};

s32 gXian_EffectConfig[] = {
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(16),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

u16 gXian_GateSteps1[] = {
    MAP_CELL_STEP(64, 48, 1, 2, 4),
    MAP_CELL_STEP(63, 48, 1, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gXian_GateSteps2[] = {
    MAP_CELL_STEP(69, 50, 2, 2, 4),
    MAP_CELL_STEP(65, 50, 2, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gXian_GateSteps3[] = {
    MAP_CELL_STEP(66, 46, 3, 2, 4),
    MAP_CELL_STEP(63, 46, 3, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gXian_GateSteps4[] = {
    MAP_CELL_STEP(67, 50, 4, 2, 4),
    MAP_CELL_STEP(63, 50, 4, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gXian_GateSteps5[] = {
    MAP_CELL_STEP(66, 44, 3, 2, 4),
    MAP_CELL_STEP(63, 44, 3, 2, 4),
    MAP_CELL_STEPS_END,
};

struct SceneEntrance gXianEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 376, 0, 504, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 344, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 376, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 424, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 408, 0, 424, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 216, 0, 184, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 392, 0, 184, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 176, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gXianExits[] = {
    SCENE_EXITS(72),      SCENE_EXIT(1, 73, 1), SCENE_EXIT(2, 73, 2),
    SCENE_EXIT(3, 73, 3), SCENE_EXIT(4, 73, 4), SCENE_EXIT(5, 73, 6),
    SCENE_EXIT(6, 9, 32), SCENE_EXIT(7, 61, 2), SCENE_EXIT(8, 2, 14),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gXianPlacements[] = {
    { 0x9c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(448), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x9f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0xa0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(420), 0, PIXELS(304), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xa4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(144), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xa6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(200), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xa8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(184), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(312), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xaa, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(428), 0, PIXELS(432), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x9f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(440), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x27, IF_FLAG_SET(FLAG_FEIZHI_WENT_TO_HSU), ACTOR_STAND,
        PIXELS(368), 0, PIXELS(496), FACING_SOUTHEAST + FACING_STEP, 3, 0 },
    { 0x9f, CONDITION_ALWAYS, (s32)gXian_ActionTable,
        PIXELS(280), 0, PIXELS(232), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(168), 0, PIXELS(312), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(312), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x5b, 0x46, ACTOR_STAND,
        PIXELS(448), 0, PIXELS(328), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0xdf, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(426), 0, PIXELS(428), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0xdf, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(299), 0, PIXELS(222), FACING_WEST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xdf, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(280), FACING_WEST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct SceneEvent gXianEvents[] = {
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 20, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Xian_HopLeaderAhead) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 21, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Xian_HopLeaderAhead) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 2, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Xian_HopLeaderAhead) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 3, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Xian_HopLeaderAhead) },
    { EVENT_TALK, 8, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianThisIsXianTravelersAreWelcome) },
    { EVENT_TALK, 9, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(Xian_AskAboutYoungWarriors) },
    { EVENT_TALK, 10, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianXianIsTheVillageOfKinu) },
    { EVENT_TALK, 11, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMartialArtsAreVeryPopularIn) },
    { EVENT_TALK, 12, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianYouWarriorsShouldVisitMasterFehs) },
    { EVENT_TALK, 13, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMyClothesAreNotSilkSilk) },
    { EVENT_TALK, KUNG_FU_STUDENT, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(Xian_WatchKungFuDemonstration) },
    { EVENT_TALK, 15, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianDisastersFromTheSkyManyMany) },
    { EVENT_TALK, 16, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianThisIsTheMulberryOrchardWhen) },
    { EVENT_TALK, 17, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(Xian_AskAboutSchoolWarriors) },
    { EVENT_TALK, WATER_CARRIER, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_SpillCarriedWater) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgXianXianIsAPeacefulVillageBut) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgXianThereWasARockslideOnSilk) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgXianIWentToHelpMoveBoulders) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgXianThePeopleWhoClearedTheRockslide) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgXianHsuIsVeryGoodAtKung) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgXianThePeopleOfXianLoveTheir) },
    { EVENT_TALK, KUNG_FU_STUDENT, CONDITION_ALWAYS, MSGNO(MsgXianAltinWasFloodedWithWaterBut) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgXianWeClearedTheRocksFromSilk) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgXianManyStrangeThingsHappenedAllAt) },
    { (1u << 28) | PSYNERGY_ON_ACTOR(ABILITY_FROST), WATER_PUDDLE, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Xian_UpdatePuddlePriority) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), WATER_PUDDLE, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Xian_FreezeSpilledWater) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), FORCE_TARGET, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_StrikeTrainingTreeWithForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianTheseChildrenMustHaveComeTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianYouYoungWarriorsLookStrongBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianIHopeManyPeopleComeTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianFightingWithKicksAndPunchesIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMasterFehsSchoolIsTrainingIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianSilkIsShinyAndSmoothIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KUNG_FU_STUDENT, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianBreakingTreesWithMyHandHurts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMasterFehSaidYahAndThen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianTheOrchardGivesTheVillageLife) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianGoingWestAloneIsVeryDangerous) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), WATER_CARRIER, CONDITION_ALWAYS, MSGNO(MsgXianIMustCarryWaterEveryDay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgXianHsuWasCaughtUnderABoulder) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgXianTheRocksThereWereVeryHard) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgXianSomePeopleDidNotCarryRocks) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgXianSandIsTheColorOfThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgXianHsuIsStrongHeadedHeWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgXianEveryoneStaysInXianTheyDo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KUNG_FU_STUDENT, CONDITION_ALWAYS, MSGNO(MsgXianThisWarriorSavedAltinVillageNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgXianItIsBadForXianIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgXianIWasHappyToSeeGems) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgXianWhereAreTheMerchantsNoOne) },
    { SEARCH(SEARCH_JAR), 101, 0xf6e, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EVENT_TALK, VILLAGE_DJINNI, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_RecruitVillageDjinni) },
    { EVENT_RAISED, WATER_CARRIER_WAIT_TRIGGER, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_MoveAsideForWaterCarrier) },
    { SCENE_EVENTS_END },
};
// clang-format on
