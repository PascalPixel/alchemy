/// @file
/// Xian (scene 72), the village of kinu, silk, reached through Mogall Forest,
/// which sends its silk along Silk Road to Tolbi and where martial arts are
/// taught at Master Feh's school. Its houses, shops and inn are
/// xian_interiors.c; the school itself is a room of the Fuchin Temple overlay
/// (FuchinTemple2, which XIAN_TO_FUCHIN_TEMPLE_2 leads into; fuchin_temple.c).
///
/// The village's talk turns on FLAG_XIAN_HEARD_HSU_TRAPPED, which Master
/// Hama's Reveal lesson at Lama Temple sets: every villager's talk and Mind
/// Read has a row for each side of it. Before, they welcome the travelers who
/// came through Mogall Forest (one asks; a no, claiming Altin, is a lie,
/// since Silk Road is blocked) and talk of silk, kung fu and Master Feh's
/// school. Feizhi, Master Feh's daughter, stands by the village's south exit
/// once she has left the school sure that Hsu is in danger
/// (FLAG_FEIZHI_WENT_TO_HSU, set in fuchin_temple.c), asking whether the
/// party, "the warriors from the school", are strong, and hoping someone will
/// take her west; she is gone once she has found the road blocked at Alpine
/// Crossing (FLAG_FEIZHI_SAW_ROCKSLIDE) or Xian has heard the news.
/// After, the villagers say that Hsu, Master Feh's apprentice, was trapped
/// under a boulder in a rockslide on Silk Road, that everyone went to clear
/// the rocks, and that young warriors saved Altin. Xian_Initialize then also
/// turns ACTOR_VILLAGER_SILK_CLOTHES west and stands the kung fu student where
/// his strike leaves him, and Xian_GetPlacements turns
/// ACTOR_VILLAGER_DISASTERS to face the party.
///
/// Two small scenes run on scene flags, which last until the party next
/// enters a scene other than by returning from a battle:
/// - the water carrier walks her round (gXian_WaterCarrierRoute). Spoken to
///   on its eastern part, she spills her water (FLAG_SCENE_TEMP_00): a puddle
///   spreads at her feet, its cell takes a collision that walking into jumps
///   the leader over it (Xian_JumpOverSpilledWater), and she walks back for
///   more. Frost freezes the puddle (FLAG_SCENE_TEMP_01) into ice that can be
///   stood on. A leader who stands in her way for two seconds makes her raise
///   XIAN_TRIGGER_CARRIER_BLOCKED (Xian_WatchWaterCarrierPath), and both walk
///   back to the start of her round.
/// - the kung fu student shows his strike on the tree before him once
///   (FLAG_SCENE_TEMP_02), and Force on the tree makes it jump on its own, to
///   his surprise (FLAG_SCENE_TEMP_03 picks his second line).
///
/// Mist, a Mercury Djinni, waits in the village until she joins, which she
/// does when spoken to.
///
/// A jar holds a Lucky Medal. Five doors lead into the houses, shops and inn
/// (scene 73, XIAN_TO_HOUSE to XIAN_TO_WEAPON_AND_ARMOR_SHOP), XIAN_TO_SANCTUM
/// into Xian Sanctum, its sliding door playing SE_188, XIAN_TO_FUCHIN_TEMPLE_2
/// into Master Feh's school, and XIAN_TO_WORLD_MAP out to the world map.
/// Coming back from the school (XIAN_FROM_FUCHIN_TEMPLE_2) or the Sanctum
/// (XIAN_FROM_SANCTUM_WEST) clears FLAG_SHOW_LOCATION_NAME, so the name is not
/// shown again.

#include "game_state.h"
#include "unit/import.h"
#include "unit/scene/xian.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "macros.h"

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
#include "songs.h"

#include "common/effect_create.h"
#include "common/effect_spawn.h"

// Xian's temporary flags:
//   FLAG_SCENE_TEMP_00  the water carrier has spilled her water
//   FLAG_SCENE_TEMP_01  the spilled water is frozen
//   FLAG_SCENE_TEMP_02  the kung fu student has shown his strike on the tree
//   FLAG_SCENE_TEMP_03  Force has made the tree jump once

/// The actors, numbering the rows of gXian_Placements. Each villager is named
/// by what they talk of before Xian hears of Hsu.
enum XianActor {
  /// "This is Xian. Travelers are welcome here!"
  ACTOR_VILLAGER_WELCOME = ACTOR_FIRST_PLACED,
  /// "Young warriors, very gallant! You came through Mogall Forest?"
  /// (Xian_AskAboutYoungWarriors)
  ACTOR_VILLAGER_YOUNG_WARRIORS,
  /// "Xian is the village of kinu. Kinu means silk."
  ACTOR_VILLAGER_KINU,
  /// "Martial arts are very popular in Xian."
  ACTOR_VILLAGER_MARTIAL_ARTS,
  /// "You warriors should visit Master Feh's school. You will be surprised!"
  ACTOR_VILLAGER_FEHS_SCHOOL,
  /// "My clothes are not silk. Silk is very expensive."
  ACTOR_VILLAGER_SILK_CLOTHES,
  /// Strikes the tree before him with a shout. "Hoo-whaaa... Hachaaa!"
  ACTOR_KUNG_FU_STUDENT,
  /// "Disasters from the sky... Many, many thieves..."
  ACTOR_VILLAGER_DISASTERS,
  /// "This is the mulberry orchard."
  ACTOR_VILLAGER_MULBERRY_ORCHARD,
  /// Feizhi, as at Alpine Crossing: looking for someone to take her west.
  ACTOR_FEIZHI,
  /// Carries water along the street. "You made me spill my water!"
  ACTOR_WATER_CARRIER,
  /// The tree the student strikes; it jumps when hit.
  ACTOR_TRAINING_TREE,
  /// The spilled water, which Frost freezes.
  ACTOR_PUDDLE,
  /// Force's mark on the training tree.
  ACTOR_FORCE_TARGET,
  ACTOR_MIST, ///< Mist, a Mercury Djinni
  /// The first of three actors kept below the others.
  ACTOR_FIRST_DECORATION
};

enum { ANIM_PUDDLE_FROZEN = 5 };

/// The trigger the water carrier raises when the leader has stood in her way
/// for two seconds.
enum { XIAN_TRIGGER_CARRIER_BLOCKED = 200, XIAN_CARRIER_BLOCKED_FRAMES = 120 };

/// Talking to Mist in Xian: Mist, a Mercury Djinni (Mercury's Djinni 2),
/// joins the party.
static void Xian_MistJoins(void) {
  Djinn_JoinParty(ACTOR_MIST, ELEMENT_MERCURY, 2);
}

/// Draws the frozen puddle under the leader while the leader stands on top of
/// it.
static void Xian_UpdateFrozenPuddleLayer(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader->y.part.pixel > 31) {
    Object_GetById(ACTOR_PUDDLE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  } else {
    Object_GetById(ACTOR_PUDDLE)->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  }
}

/// Rotates the training tree during the student's strike and Force animation.
static void Xian_RotateTrainingTree(void) {
  struct FieldActor *tree = Object_GetById(ACTOR_TRAINING_TREE);
  struct AnimationObject *sprite = tree->sprite;
  sprite->rotation += 0x1400;
}

/// Emits particles beside the student every fourth frame.
static void Xian_SpawnTrainingParticles(void) {
  // The byte view keeps the Z load ahead of the options-script store.
  u8 *studentActor = (u8 *)Object_GetById(ACTOR_KUNG_FU_STUDENT);

  if ((gFrameCount & 3) == 0) {
    struct EffectOptions config;
    config.priority = 1;
    config.palette = 9;
    config.type = 169;
    config.script = gXian_TrainingParticleScript;
    Effect_Spawn(*(s32 *)(studentActor + offsetof(struct FieldActor, x)),
                 *(s32 *)(studentActor + offsetof(struct FieldActor, y)),
                 *(s32 *)(studentActor + offsetof(struct FieldActor, z)) -
                     PIXELS(1),
                 0, -PIXELS(1), -PIXELS(1),
                 EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE | EFFECT_USE_TYPE |
                     EFFECT_USE_SCRIPT,
                 &config);
  }
}

#include "common/object_distance.h"

/// Tracks a nearby target only within the actor's current facing sector.
static s32 Xian_UpdateCarrierTracking(struct FieldActor *actor,
                                      struct FieldActor *target,
                                      s32 distanceLimit, s32 forceTracking) {
  s32 isTracking = 0;

  // This route uses paused/timer as its tracking latch and mode.
  if (actor->paused == 1 && actor->timer == 0) {
    Object_SetMode(actor, 1);
    return 1;
  }

  if (FixedPoint_Distance((const u8 *)target + offsetof(struct FieldActor, x),
                          (const u8 *)actor + offsetof(struct FieldActor, x)) <
          distanceLimit ||
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
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if ((u32)((leader->x.fixed >> 20) - 17) <= 1 && leader->z.fixed >> 20 == 14 &&
      self->x.fixed >> 20 <= 19 && self->velocityX <= 0) {
    if (leader->x.fixed <= self->x.fixed) {
      self->riseCounter++;
      near = 1;
    }
  } else {
    self->riseCounter = 0;
  }
  if (near && self->riseCounter > XIAN_CARRIER_BLOCKED_FRAMES - 1) {
    gEventWork->raisedTrigger = XIAN_TRIGGER_CARRIER_BLOCKED;
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

const struct SceneEntrance *Xian_GetEntrances(void) { return gXian_Entrances; }

const struct SceneRegion *Xian_GetRegions(void) { return NULL; }

const u32 *Xian_GetExits(void) { return gXian_Exits; }

/// Once Xian has heard Hsu is trapped, ACTOR_VILLAGER_DISASTERS turns to face
/// the party.
const struct ScenePlacement *Xian_GetPlacements(void) {
  if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) != 0)
    gXian_Placements[ACTOR_VILLAGER_DISASTERS - ACTOR_FIRST_PLACED].talkFacing =
        TALK_FACE_PARTY;
  return gXian_Placements;
}

/// A villager, ACTOR_VILLAGER_YOUNG_WARRIORS, asks about the young warriors'
/// way here, before Xian hears of Hsu. "Young warriors, very gallant! You came
/// through Mogall Forest?" A yes earns respect; a no is a lie, since Silk Road
/// from Altin is blocked.
static void Xian_AskAboutYoungWarriors(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgXianYoungWarriorsVeryGallantCame);
  Engine_EventAskYesNo(ACTOR_VILLAGER_YOUNG_WARRIORS, 0);
  Engine_EventEnd();
}

/// Talking to the water carrier. While she carries her water east of x = 320
/// the first time, she spills it: a puddle spreads at her feet, she cries "You
/// made me spill my water!" and walks back west to fetch more, the leader
/// stepping aside if he is in her way. Otherwise she only answers: "Do not
/// speak to me. I might spill my water.", or once it is spilled, "Now I must
/// get water again."
static void Xian_TalkToWaterCarrier(void) {
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

  puddle = Object_GetById(ACTOR_PUDDLE);
  Engine_EventBegin();
  leaderMustStepAside = 0;
  carrier = Actor_Get(ACTOR_WATER_CARRIER);
  carrier->update = NULL;
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) != 0 ||
      (Object_GetById(ACTOR_WATER_CARRIER)->x.fixed >> 20) <= 19) {
    carrier = Actor_Get(ACTOR_WATER_CARRIER);
    savedFacing = carrier->facing;
    Actor_FaceActor(ACTOR_WATER_CARRIER, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgXianNowMustGetWaterAgain);
    if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) == 0) {
      gEventWork->message++;
      Event_ShowMessage(ACTOR_WATER_CARRIER, 0);
      Object_GetById(ACTOR_WATER_CARRIER)->timer = 0;
      carrier = Object_GetById(ACTOR_WATER_CARRIER);
      carrier->facing = savedFacing;
    } else {
      Event_ShowMessage(ACTOR_WATER_CARRIER, 0);
      Actor_FaceDirection(ACTOR_WATER_CARRIER, FACING_WEST, 20);
    }
    carrier = Actor_Get(ACTOR_WATER_CARRIER);
    carrier->update = XIAN_WATER_CARRIER_UPDATE;
    Engine_EventEnd();
    return;
  }
  // Make room for the carrier before spilling the water.
  if ((Object_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 19) > 27 &&
      (Object_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 19) <= 29 &&
      (Object_GetById(ACTOR_PARTY_LEADER)->x.fixed >> 20) != 26) {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_WATER_CARRIER, 0);
    Engine_EventWait(5);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    carrier = Actor_Get(ACTOR_WATER_CARRIER);
    if (leader->x.fixed < carrier->x.fixed) {
      Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
      carrier = Object_GetById(ACTOR_WATER_CARRIER);
      Actor_WalkTo(ACTOR_PARTY_LEADER, ((carrier->x.fixed >> 20) << 4) - 8,
                   232);
      leaderMustStepAside = 1;
    } else {
      Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
      carrier = Object_GetById(ACTOR_WATER_CARRIER);
      Actor_WalkTo(ACTOR_PARTY_LEADER, ((carrier->x.fixed >> 20) << 4) + 24,
                   232);
    }
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  }
  carrier = Actor_Get(ACTOR_WATER_CARRIER);
  carrier->targetX = ACTOR_NO_TARGET;
  carrier = Object_GetById(ACTOR_WATER_CARRIER);
  carrier->targetY = ACTOR_NO_TARGET;
  carrier = Actor_Get(ACTOR_WATER_CARRIER);
  carrier->targetZ = ACTOR_NO_TARGET;
  Actor_SetBuiltinAction(ACTOR_WATER_CARRIER, ACTOR_ACTION_IDLE);
  Engine_ActorSetAnimation(ACTOR_WATER_CARRIER, ANIM_STAND);
  Engine_ActorRunRepeatedMotion(ACTOR_WATER_CARRIER, 2);
  Engine_EventWait(10);
  Event_PlayCue(SE_228);
  // Copy the wet terrain cell and grow the puddle into view.
  puddle->scaleX = 0x4ccc;
  puddle->scaleY = 0x4ccc;
  carrier = Object_GetById(ACTOR_WATER_CARRIER);
  carrierX = carrier->x.fixed;
  carrier = Object_GetById(ACTOR_WATER_CARRIER);
  carrierCellZ = carrier->z.fixed >> 20;
  Actor_SetPosition(ACTOR_PUDDLE, ((carrierX >> 20) << 20) + PIXELS(8),
                    (carrierCellZ << 20) + PIXELS(8));
  carrier = Object_GetById(ACTOR_WATER_CARRIER);
  carrierColumnX = carrier->x.fixed;
  carrier = Object_GetById(ACTOR_WATER_CARRIER);
  Map_CopyCellAttributes(16, 16, 1, 1, carrierColumnX >> 20,
                         carrier->z.fixed >> 20);
  Engine_ActorSetSpritePriority(ACTOR_PUDDLE, 2);
  puddle->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  do {
    Engine_TaskWait(3);
    scaleY = puddle->scaleY;
    scaleX = puddle->scaleX;
    puddle->scaleY = scaleY + 0x1999;
    scaleX += 0x1999;
    puddle->scaleX = scaleX;
  } while (scaleX <= 0xffff);
  Actor_ShowEmote(ACTOR_WATER_CARRIER, EMOTE_IN_FRONT | 5, 70);
  Actor_FaceActor(ACTOR_WATER_CARRIER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_WATER_CARRIER, EMOTE_IN_FRONT | 3, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_WATER_CARRIER, 2);
  Engine_EventWait(70);
  Engine_EventSetMessage(MsgXianDoingMadeMeSpillMy);
  Event_ShowMessageAndWait(ACTOR_WATER_CARRIER, 0, 20);
  BattleFx_PlayQueuedSound();
  if ((Object_GetById(ACTOR_PARTY_LEADER)->x.fixed >> 20) == 26 &&
      (Object_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 20) > 13) {
    leaderMustStepAside = 1;
  }
  // Clear the carrier's route back to the water supply.
  if (leaderMustStepAside != 0) {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
    Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  }
  Actor_SetSpeed(ACTOR_WATER_CARRIER, 0xcccc, 0x6666);
  carrier = Object_GetById(ACTOR_WATER_CARRIER);
  if ((carrier->z.fixed >> 20) != 14) {
    carrier = Actor_Get(ACTOR_WATER_CARRIER);
    Actor_WalkToAndWait(ACTOR_WATER_CARRIER, carrier->x.part.pixel, 232);
  }
  Actor_WalkToAndWait(ACTOR_WATER_CARRIER, 280, 232);
  GameFlag_Set(FLAG_SCENE_TEMP_00);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventEnd();
}

/// Frost on the spilled water, before it freezes: the puddle is drawn in
/// front while the leader stands north of it.
static void Xian_UpdatePuddlePriority(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  if ((leader->z.fixed >> 20) <= 13)
    Engine_ActorSetSpritePriority(ACTOR_PUDDLE, 1);
}

/// Frost freezes the spilled water: the puddle turns to ice the leader can
/// stand on, and there is nothing left to jump over.
static void Xian_FreezeSpilledWater(void) {
  struct FieldActor *puddle;
  s32 stationaryMotionFlags;
  s32 puddleX;

  Engine_EventBegin();
  Object_GetById(ACTOR_PUDDLE)->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  stationaryMotionFlags = 0;
  Object_GetById(ACTOR_PUDDLE)->motionFlags = stationaryMotionFlags;
  puddle = Object_GetById(ACTOR_PUDDLE);
  puddleX = puddle->x.fixed;
  puddle = Object_GetById(ACTOR_PUDDLE);
  Map_CopyCellAttributes(3, 17, 1, 1, (puddleX >> 20), (puddle->z.fixed >> 20));
  FieldScheduler_AddCallback(Xian_UpdateFrozenPuddleLayer, TASK_PRIORITY_SCENE);
  GameFlag_Set(FLAG_SCENE_TEMP_01);
  Engine_ActorSetSpritePriority(ACTOR_PUDDLE, 2);
  Engine_EventEnd();
}

/// The kung fu student. The first time: "Hoo-whaaa... Hachaaa!", the leader
/// steps back, and the student leaps at the tree, which jumps under his strike;
/// "Heh heh heh..." Afterwards: "My kung fu is strong."
static void Xian_TalkToKungFuStudent(void) {
  s32 demonstrationSeen;
  struct FieldActor *leader;
  SchedulerCallback particleCallback;
  s32 reboundVelocity;
  s32 stepBackOffset[3];
  struct EffectOptions effectParams;

  Engine_EventBegin();
  demonstrationSeen = Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02);
  if (demonstrationSeen != 0) {
    Engine_ActorFaceActor(ACTOR_KUNG_FU_STUDENT, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgXianKungFuStrong);
    Engine_EventShowMessage(ACTOR_KUNG_FU_STUDENT, 0);
    Engine_ActorFaceDirection(ACTOR_KUNG_FU_STUDENT, FACING_EAST, 10);
    Engine_EventEnd();
  } else {
    // Step back, then watch the student jump and strike the tree.
    Engine_EventSetMessage(MsgXianHooWhaaaHachaaa);
    Engine_EventShowMessage(ACTOR_KUNG_FU_STUDENT, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    SET_VECTOR(stepBackOffset, demonstrationSeen, demonstrationSeen,
               demonstrationSeen);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    FieldVector_AddPolarOffset(-PIXELS(8), leader->facing,
                               (struct FieldPosition *)stepBackOffset);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER,
                                     stepBackOffset[0] / 0x10000,
                                     stepBackOffset[2] / 0x10000);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(ACTOR_KUNG_FU_STUDENT, 2);
    effectParams.priority = 1;
    Effect_Spawn(PIXELS(192), 0, PIXELS(312), 0x1999, 0x3333, 0, 0x20001, 0);
    Effect_Spawn(PIXELS(192), 0, PIXELS(312), 0x3333, 0x1999, 0, 0x20001, 0);
    Engine_AudioPlayCue(SE_132);
    particleCallback = Xian_SpawnTrainingParticles;
    Task_AddCallback(particleCallback, TASK_PRIORITY_SCENE);
    Object_GetById(ACTOR_KUNG_FU_STUDENT)->velocityY = 0x60000;
    Object_GetById(ACTOR_KUNG_FU_STUDENT)->gravity = 0x10000;
    Object_GetById(ACTOR_KUNG_FU_STUDENT)->bounce = 0;
    Actor_SetSpeed(ACTOR_KUNG_FU_STUDENT, 0x30000, 0x18000);
    Actor_SetDestination(ACTOR_KUNG_FU_STUDENT, 168, 312);
    Engine_ActorWaitForMove(ACTOR_KUNG_FU_STUDENT);
    Engine_AudioPlayCue(SE_HEAVY_IMPACT);
    Actor_SetActionScript(ACTOR_TRAINING_TREE, gXian_TrainingTreeJump);
    FieldScheduler_AddCallback(Xian_RotateTrainingTree, TASK_PRIORITY_SCENE);
    effectParams.type = 0x11b;
    effectParams.script = gXian_TreeImpactScript;
    effectParams.rotation = 0x4000;
    Effect_Spawn(PIXELS(168), 0, PIXELS(332), 0, 0, 0,
                 EFFECT_USE_PRIORITY | EFFECT_USE_TYPE | EFFECT_USE_SCRIPT |
                     EFFECT_USE_ROTATION,
                 &effectParams);
    Actor_SetDestination(ACTOR_KUNG_FU_STUDENT, 146, 312);
    Engine_ActorWaitForMove(ACTOR_KUNG_FU_STUDENT);
    // Finish the impact effects and leave the student in the final pose.
    Engine_TaskRemoveCallback((void (*)(void))particleCallback);
    Effect_Spawn(PIXELS(144), 0, PIXELS(312), 0, 0, 0, 0x20001, 0);
    Effect_Spawn(PIXELS(144), 0, PIXELS(312), -0x3333, 0x1999, 0, 0x20001, 0);
    reboundVelocity = -0x8000;
    Effect_Spawn(PIXELS(144), 0, PIXELS(312), reboundVelocity, 0, 0, 0x20001,
                 0);
    Object_RefreshSelectorById(ACTOR_TRAINING_TREE);
    Engine_AudioPlayCue(SE_124);
    Effect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), 0, 0, 0, 0x20001, 0);
    Effect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), 0x3333, 0, 0, 0x20001, 0);
    Effect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), -0x3333, 0, 0, 0x20001,
                 0);
    Engine_TaskRemoveCallback(Xian_RotateTrainingTree);
    Object_GetById(ACTOR_TRAINING_TREE)->sprite->rotation = reboundVelocity;
    Object_GetById(ACTOR_KUNG_FU_STUDENT)->bounce = 0x4000;
    Object_GetById(ACTOR_KUNG_FU_STUDENT)->gravity = 0x10000;
    Engine_EventWait(30);
    Engine_ActorFaceDirection(ACTOR_KUNG_FU_STUDENT, FACING_EAST, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_KUNG_FU_STUDENT, 2);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(ACTOR_KUNG_FU_STUDENT, 0, 20);
    Engine_GameFlagSet(FLAG_SCENE_TEMP_02);
    BattleFx_PlayQueuedSound();
    Engine_EventEnd();
  }
}

/// Talking to Feizhi by the south exit, after she has left Master Feh's school
/// and before Xian hears of Hsu. "You are the warriors from the school... Are
/// you strong warriors?" A yes: she hopes they will go west; a no: very few
/// warriors would call themselves weak.
static void Xian_TalkToFeizhi(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgXianWarriorsFromSchoolStrongWarriors);
  Engine_EventAskYesNo(ACTOR_FEIZHI, 0);
  Engine_EventEnd();
}

/// Walking into the spilled water before it is frozen: the leader jumps two
/// cells over it, where nothing stands in the way.
static void Xian_JumpOverSpilledWater(void) {
  s32 savedMotionFlags;
  struct FieldActor *leader;
  struct FieldPosition landingPosition;
  struct FieldPosition *landing;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  savedMotionFlags = leader->motionFlags;
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00) != 0) {
    landing = &landingPosition;
    landing->x = (leader->x.fixed & -0x100000) + PIXELS(8);
    landing->y = leader->y.fixed;
    landing->z = (leader->z.fixed & -0x100000) + PIXELS(8);
    FieldVector_AddPolarOffset(PIXELS(32), (leader->facing + 0x2000) & 0xc000,
                               landing);
    if (FieldObject_CheckMovementCollision(leader, landing) == 0) {
      Engine_EventBegin();
      Object_SetMode(leader, 6);
      Engine_TaskWait(6);
      Engine_AudioPlayCue(SE_JUMP);
      Object_SetMode(leader, 7);
      leader->speed = PIXELS(3);
      leader->acceleration = PIXELS(2);
      leader->velocityY = PIXELS(4);
      leader->motionFlags &= 126;
      Engine_ActorSetSpriteFlags(leader, 0);
      Engine_ObjectMotionSetPositionAndCommit(
          ACTOR_PARTY_LEADER, landing->x >> 16, landing->z >> 16);
      Object_SetMode(leader, 6);
      Engine_ActorSetSpriteFlags(leader, 1);
      leader->motionFlags = savedMotionFlags;
      Engine_EventEnd();
    }
  }
}

/// Walks the leader through a doorway and leaves by its exit; through the
/// Sanctum's door the leader walks straight in, without centering on it.
static void Xian_WalkThroughExit(s32 exitId) {
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  if (exitId == XIAN_TO_SANCTUM) {
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  } else {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  }
  gEventWork->transitionFrames = 16;
  Engine_EventRequestExit(exitId);
}

/// Walking north into one of Xian's doors: it opens and the party goes in.
/// Each door's trigger is the number of the exit it leads by: five lead into
/// the houses, shops and inn, XIAN_TO_SANCTUM into Xian Sanctum (SE_188) and
/// XIAN_TO_FUCHIN_TEMPLE_2 into Master Feh's school.
static void Xian_OpenDoorAndLeave(void) {
  struct EventWork *event = gEventWork;

  Engine_EventBegin();

  switch (event->touchedTrigger) {
  case XIAN_TO_HOUSE:
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapAnimateCells(gXian_DoorSteps1, 81, 18);
    break;
  case XIAN_TO_HSUS_HOUSE:
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapAnimateCells(gXian_DoorSteps2, 83, 11);
    break;
  case XIAN_TO_XIAN_INTERIORS_EAST:
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapAnimateCells(gXian_DoorSteps2, 86, 11);
    break;
  case XIAN_TO_INN:
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapAnimateCells(gXian_DoorSteps3, 84, 24);
    break;
  case XIAN_TO_WEAPON_AND_ARMOR_SHOP:
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapAnimateCells(gXian_DoorSteps3, 72, 7);
    break;
  case XIAN_TO_SANCTUM:
    Engine_AudioPlayCue(SE_188);
    Engine_MapAnimateCells(gXian_DoorSteps4, 69, 11);
    break;
  case XIAN_TO_FUCHIN_TEMPLE_2:
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapAnimateCells(gXian_DoorSteps5, 83, 7);
    break;
  default:
    break;
  }

  Xian_WalkThroughExit(event->touchedTrigger);
  Engine_EventEnd();
}

/// Force on the training tree: it jumps, and the student cries out.
/// "Wha-Wha-What happened? The tree jumped on its own!", the next time
/// "...It jumped again... I am too tired...".
static void Xian_StrikeTrainingTreeWithForce(void) {
  SchedulerCallback rotationCallback;

  Engine_EventBegin();
  Actor_SetActionScript(ACTOR_TRAINING_TREE, gXian_TrainingTreeJump);
  rotationCallback = Xian_RotateTrainingTree;
  FieldScheduler_AddCallback(rotationCallback, TASK_PRIORITY_SCENE);
  Object_RefreshSelectorById(ACTOR_TRAINING_TREE);
  Engine_AudioPlayCue(SE_124);
  Effect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), 0, 0, 0, 0x20001, 0);
  Effect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), 0x3333, 0, 0, 0x20001, 0);
  Effect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), -0x3333, 0, 0, 0x20001, 0);
  Engine_TaskRemoveCallback(rotationCallback);
  Object_GetById(ACTOR_TRAINING_TREE)->sprite->rotation = 0x8000;
  Engine_ActorSetPosition(ACTOR_FORCE_TARGET, PIXELS(168), PIXELS(312));
  Engine_EventWait(20);
  Engine_ActorFaceActor(ACTOR_KUNG_FU_STUDENT, ACTOR_TRAINING_TREE, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_KUNG_FU_STUDENT, 2);
  Engine_EventWait(10);
  Engine_EventSetMessage(MsgXianWhaWhaHappened);
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_03)) {
    gEventWork->message++;
  }
  Engine_EventShowMessage(ACTOR_KUNG_FU_STUDENT, 0);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_03);
  BattleFx_PlayQueuedSound();
  Engine_EventEnd();
}

/// The leader has stood in the water carrier's way for two seconds: both walk
/// to the start of her route, the leader turns west, and she sets off again.
static void Xian_MoveAsideForWaterCarrier(void) {
  struct FieldActor *carrier;

  Engine_EventBegin();
  Actor_SetBuiltinAction(ACTOR_WATER_CARRIER, ACTOR_ACTION_IDLE);
  carrier = Actor_Get(ACTOR_WATER_CARRIER);
  carrier->update = 0;
  carrier = Actor_Get(ACTOR_WATER_CARRIER);
  carrier->targetX = ACTOR_NO_TARGET;
  carrier = Object_GetById(ACTOR_WATER_CARRIER);
  carrier->targetZ = ACTOR_NO_TARGET;
  carrier = Object_GetById(ACTOR_WATER_CARRIER);
  carrier->velocityX = 0;
  carrier = Object_GetById(ACTOR_WATER_CARRIER);
  carrier->velocityZ = 0;
  carrier = Object_GetById(ACTOR_WATER_CARRIER);
  carrier->speed = 0;
  carrier = Actor_Get(ACTOR_WATER_CARRIER);
  carrier->acceleration = 0;
  Actor_ShowEmote(ACTOR_WATER_CARRIER, EMOTE_IN_FRONT | 3, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_WATER_CARRIER, 2);
  Engine_EventWait(60);
  Actor_SetSpeed(ACTOR_WATER_CARRIER, 0x18000, 0xc000);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
  Actor_WalkTo(ACTOR_WATER_CARRIER, 280, 232);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 296, 232);
  Engine_ActorWaitForMove(ACTOR_WATER_CARRIER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
  Actor_SetActionScript(ACTOR_WATER_CARRIER, gXian_WaterCarrierRoute);
  carrier = Actor_Get(ACTOR_WATER_CARRIER);
  carrier->update = XIAN_WATER_CARRIER_UPDATE;
  Engine_EventEnd();
}

const struct SceneEvent *Xian_GetEvents(void) { return gXian_Events; }

/// Queues and plays SE_169 and sets up the village as the story and this visit
/// left it: after the news of Hsu (or, back from a battle, after his strike)
/// the kung fu student stands where his strike leaves him; Feizhi is gone
/// after the news or once she has seen the rockslide; back from a battle, the
/// frozen puddle keeps its ice; the water carrier is hooked to
/// Xian_WatchWaterCarrierPath; and the training tree, its Force mark and the
/// three actors kept below the others are set in place.
s32 Xian_Initialize(void) {
  struct FieldActor *actor;
  u32 decorationIndex;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  BattleFx_SetQueuedSoundAndPlay(SE_169);
  // Back from the school (XIAN_FROM_FUCHIN_TEMPLE_2) or the Sanctum
  // (XIAN_FROM_SANCTUM_WEST), the last two entrances, the name is not shown.
  if (gGameState.entrance >= XIAN_FROM_FUCHIN_TEMPLE_2) {
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  }
  if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED)) {
    Actor_FaceDirection(ACTOR_VILLAGER_SILK_CLOTHES, FACING_WEST, 0);
    Actor_SetPosition(ACTOR_KUNG_FU_STUDENT, PIXELS(146), PIXELS(312));
    Engine_ActorFaceDirection(ACTOR_KUNG_FU_STUDENT, FACING_EAST, 0);
    if (Engine_GameFlagIsSet(FLAG_FEIZHI_WENT_TO_HSU)) {
      Engine_ActorSetPosition(ACTOR_FEIZHI, 0, 0);
    }
  }
  // Gone once she has found the road blocked (Alpine Crossing's
  // FLAG_FEIZHI_SAW_ROCKSLIDE).
  if (Engine_GameFlagIsSet(FLAG_FEIZHI_SAW_ROCKSLIDE)) {
    Engine_ActorSetPosition(ACTOR_FEIZHI, 0, 0);
  }
  // Keep the three decorative actors below the moving characters.
  for (decorationIndex = 0; decorationIndex <= 2; decorationIndex++) {
    struct FieldActor *actor;
    actor = Object_GetById(decorationIndex + ACTOR_FIRST_DECORATION);
    actor->sprite->image.priority = 1;
    actor->motionFlags = 0;
    actor->collisionFlags = 8;
    Engine_ActorSetSpriteFlags(actor, 0);
    ObjectGroup_SetChildValue(actor, 15);
    actor->priorityFlags = (actor->priorityFlags & 254) | 2;
  }
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02)) {
    Actor_SetPosition(ACTOR_KUNG_FU_STUDENT, PIXELS(146), PIXELS(312));
    Engine_ActorFaceDirection(ACTOR_KUNG_FU_STUDENT, FACING_EAST, 0);
  }
  // Restore the frozen puddle and callbacks when re-entering the village.
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01)) {
    Engine_ActorSetAnimation(ACTOR_PUDDLE, ANIM_PUDDLE_FROZEN);
    {
      s32 puddleX = Object_GetById(ACTOR_PUDDLE)->x.fixed;

      Engine_MapCopyCellAttributes(3, 17, 1, 1, puddleX >> 20,
                                   Object_GetById(ACTOR_PUDDLE)->z.fixed >> 20);
    }
    FieldScheduler_AddCallback(Xian_UpdateFrozenPuddleLayer,
                               TASK_PRIORITY_SCENE);
  }
  Engine_ActorSetChildValue(ACTOR_WATER_CARRIER, 2);
  Object_GetById(ACTOR_WATER_CARRIER)->update = XIAN_WATER_CARRIER_UPDATE;
  actor = Object_GetById(ACTOR_TRAINING_TREE);
  actor->motionFlags = 0;
  actor->y.fixed = PIXELS(16);
  actor->targetY = PIXELS(16);
  actor->scaleX = 0x8ccc;
  actor->scaleY = 0x6666;
  actor->sprite->rotation = 0x8000;
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FORCE_TARGET), 0);
  Object_GetById(ACTOR_FORCE_TARGET)->motionFlags = 0;
  Object_GetById(ACTOR_FORCE_TARGET)->y.fixed = 0;
  Object_GetById(ACTOR_FORCE_TARGET)->targetY = ACTOR_NO_TARGET;
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

/// The water carrier's round: from (280, 232) east to (424, 232), up to
/// (424, 216) and back, pausing at each end.
s32 gXian_WaterCarrierRoute[] = {
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

/// The struck tree jumps 32 pixels and drops back.
s32 gXian_TrainingTreeJump[] = {
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

/// The effect at the tree's foot when the student strikes it: 18 frames.
s32 gXian_TreeImpactScript[] = {
    SC_WAIT(18), SC_WAIT(1), SC_POS(0, 0, 0), SC_WAIT(1), SC_FREE,
};

/// Each particle beside the leaping student, facing west: 16 frames.
s32 gXian_TrainingParticleScript[] = {
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(16),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

/// The doors' opening frames, copied from map cells.
u16 gXian_DoorSteps1[] = {
    MAP_CELL_STEP(64, 48, 1, 2, 4),
    MAP_CELL_STEP(63, 48, 1, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gXian_DoorSteps2[] = {
    MAP_CELL_STEP(69, 50, 2, 2, 4),
    MAP_CELL_STEP(65, 50, 2, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gXian_DoorSteps3[] = {
    MAP_CELL_STEP(66, 46, 3, 2, 4),
    MAP_CELL_STEP(63, 46, 3, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gXian_DoorSteps4[] = {
    MAP_CELL_STEP(67, 50, 4, 2, 4),
    MAP_CELL_STEP(63, 50, 4, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gXian_DoorSteps5[] = {
    MAP_CELL_STEP(66, 44, 3, 2, 4),
    MAP_CELL_STEP(63, 44, 3, 2, 4),
    MAP_CELL_STEPS_END,
};

struct SceneEntrance gXian_Entrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {XIAN_FROM_HOUSE, CONDITION_ALWAYS, 376, 0, 504, FACING_NORTH, 0, -1, -1,
     -1, -1, 0},
    {XIAN_FROM_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, 344, 0, 328,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {XIAN_FROM_HSUS_HOUSE, CONDITION_ALWAYS, 376, 0, 216, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {XIAN_FROM_XIAN_INTERIORS_EAST, CONDITION_ALWAYS, 424, 0, 216, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {XIAN_FROM_XIAN_INTERIORS_SOUTH, CONDITION_ALWAYS, 408, 0, 424,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {XIAN_FROM_XIAN_INTERIORS_WEST, CONDITION_ALWAYS, 216, 0, 184, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {XIAN_FROM_FUCHIN_TEMPLE_2, CONDITION_ALWAYS, 392, 0, 184, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {XIAN_FROM_SANCTUM_WEST, CONDITION_ALWAYS, 176, 0, 216, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gXian_Exits[] = {
    SCENE_EXITS(Xian),
    SCENE_EXIT(XIAN_TO_HOUSE, XianInteriors, XIAN_INTERIORS_HOUSE),
    SCENE_EXIT(XIAN_TO_HSUS_HOUSE, XianInteriors, XIAN_INTERIORS_HSUS_HOUSE),
    SCENE_EXIT(XIAN_TO_XIAN_INTERIORS_EAST, XianInteriors,
               XIAN_INTERIORS_FROM_XIAN_EAST),
    SCENE_EXIT(XIAN_TO_INN, XianInteriors, XIAN_INTERIORS_INN_SOUTH),
    SCENE_EXIT(XIAN_TO_WEAPON_AND_ARMOR_SHOP, XianInteriors,
               XIAN_INTERIORS_WEAPON_AND_ARMOR_SHOP),
    SCENE_EXIT(XIAN_TO_SANCTUM, Sanctum, SANCTUM_FROM_XIAN),
    SCENE_EXIT(XIAN_TO_FUCHIN_TEMPLE_2, FuchinTemple2,
               FUCHIN_TEMPLE_2_FROM_XIAN),
    SCENE_EXIT(XIAN_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_XIAN),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gXian_Placements[] = {
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
    { 0x9f, CONDITION_ALWAYS, (s32)gXian_WaterCarrierRoute,
        PIXELS(280), 0, PIXELS(232), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(168), 0, PIXELS(312), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(312), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x5b, FLAG_DJINNI_0 + ELEMENT_MERCURY * 20 + 2, ACTOR_STAND,
        PIXELS(448), 0, PIXELS(328), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0xdf, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(426), 0, PIXELS(428), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0xdf, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(299), 0, PIXELS(222), FACING_WEST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xdf, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(280), FACING_WEST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct SceneEvent gXian_Events[] = {
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, XIAN_TO_WORLD_MAP },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 20, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Xian_JumpOverSpilledWater) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 21, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Xian_JumpOverSpilledWater) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 2, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Xian_JumpOverSpilledWater) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 3, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Xian_JumpOverSpilledWater) },
    { EVENT_TALK, ACTOR_VILLAGER_WELCOME, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianThisIsXianTravelersAreWelcome) },
    { EVENT_TALK, ACTOR_VILLAGER_YOUNG_WARRIORS, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(Xian_AskAboutYoungWarriors) },
    { EVENT_TALK, ACTOR_VILLAGER_KINU, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianXianIsTheVillageOfKinu) },
    { EVENT_TALK, ACTOR_VILLAGER_MARTIAL_ARTS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMartialArtsAreVeryPopularIn) },
    { EVENT_TALK, ACTOR_VILLAGER_FEHS_SCHOOL, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianYouWarriorsShouldVisitMasterFehs) },
    { EVENT_TALK, ACTOR_VILLAGER_SILK_CLOTHES, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMyClothesAreNotSilkSilk) },
    { EVENT_TALK, ACTOR_KUNG_FU_STUDENT, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(Xian_TalkToKungFuStudent) },
    { EVENT_TALK, ACTOR_VILLAGER_DISASTERS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianDisastersFromTheSkyManyMany) },
    { EVENT_TALK, ACTOR_VILLAGER_MULBERRY_ORCHARD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianThisIsTheMulberryOrchardWhen) },
    { EVENT_TALK, ACTOR_FEIZHI, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(Xian_TalkToFeizhi) },
    { EVENT_TALK, ACTOR_WATER_CARRIER, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_TalkToWaterCarrier) },
    { EVENT_TALK, ACTOR_VILLAGER_WELCOME, CONDITION_ALWAYS, MSGNO(MsgXianXianIsAPeacefulVillageBut) },
    { EVENT_TALK, ACTOR_VILLAGER_YOUNG_WARRIORS, CONDITION_ALWAYS, MSGNO(MsgXianThereWasARockslideOnSilk) },
    { EVENT_TALK, ACTOR_VILLAGER_KINU, CONDITION_ALWAYS, MSGNO(MsgXianIWentToHelpMoveBoulders) },
    { EVENT_TALK, ACTOR_VILLAGER_MARTIAL_ARTS, CONDITION_ALWAYS, MSGNO(MsgXianThePeopleWhoClearedTheRockslide) },
    { EVENT_TALK, ACTOR_VILLAGER_FEHS_SCHOOL, CONDITION_ALWAYS, MSGNO(MsgXianHsuIsVeryGoodAtKung) },
    { EVENT_TALK, ACTOR_VILLAGER_SILK_CLOTHES, CONDITION_ALWAYS, MSGNO(MsgXianThePeopleOfXianLoveTheir) },
    { EVENT_TALK, ACTOR_KUNG_FU_STUDENT, CONDITION_ALWAYS, MSGNO(MsgXianAltinWasFloodedWithWaterBut) },
    { EVENT_TALK, ACTOR_VILLAGER_DISASTERS, CONDITION_ALWAYS, MSGNO(MsgXianWeClearedTheRocksFromSilk) },
    { EVENT_TALK, ACTOR_VILLAGER_MULBERRY_ORCHARD, CONDITION_ALWAYS, MSGNO(MsgXianManyStrangeThingsHappenedAllAt) },
    { (1u << 28) | PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Xian_UpdatePuddlePriority) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Xian_FreezeSpilledWater) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FORCE_TARGET, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_StrikeTrainingTreeWithForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WELCOME, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianTheseChildrenMustHaveComeTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_YOUNG_WARRIORS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianYouYoungWarriorsLookStrongBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_KINU, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianIHopeManyPeopleComeTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_MARTIAL_ARTS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianFightingWithKicksAndPunchesIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_FEHS_SCHOOL, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMasterFehsSchoolIsTrainingIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SILK_CLOTHES, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianSilkIsShinyAndSmoothIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KUNG_FU_STUDENT, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianBreakingTreesWithMyHandHurts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DISASTERS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMasterFehSaidYahAndThen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_MULBERRY_ORCHARD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianTheOrchardGivesTheVillageLife) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianGoingWestAloneIsVeryDangerous) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WATER_CARRIER, CONDITION_ALWAYS, MSGNO(MsgXianIMustCarryWaterEveryDay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WELCOME, CONDITION_ALWAYS, MSGNO(MsgXianHsuWasCaughtUnderABoulder) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_YOUNG_WARRIORS, CONDITION_ALWAYS, MSGNO(MsgXianTheRocksThereWereVeryHard) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_KINU, CONDITION_ALWAYS, MSGNO(MsgXianSomePeopleDidNotCarryRocks) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_MARTIAL_ARTS, CONDITION_ALWAYS, MSGNO(MsgXianSandIsTheColorOfThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_FEHS_SCHOOL, CONDITION_ALWAYS, MSGNO(MsgXianHsuIsStrongHeadedHeWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SILK_CLOTHES, CONDITION_ALWAYS, MSGNO(MsgXianEveryoneStaysInXianTheyDo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KUNG_FU_STUDENT, CONDITION_ALWAYS, MSGNO(MsgXianThisWarriorSavedAltinVillageNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DISASTERS, CONDITION_ALWAYS, MSGNO(MsgXianItIsBadForXianIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_MULBERRY_ORCHARD, CONDITION_ALWAYS, MSGNO(MsgXianIWasHappyToSeeGems) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgXianWhereAreTheMerchantsNoOne) },
    { SEARCH(SEARCH_JAR), 101, FLAG_XIAN_LUCKY_MEDAL_JAR, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EVENT_TALK, ACTOR_MIST, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_MistJoins) },
    { EVENT_RAISED, XIAN_TRIGGER_CARRIER_BLOCKED, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_MoveAsideForWaterCarrier) },
    { SCENE_EVENTS_END },
};
// clang-format on
