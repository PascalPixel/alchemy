/// @file
/// Lighthouse Aerie (SceneId_MercuryLighthouseAerie), the top of Mercury
/// Lighthouse, where its beacon burns.
///
/// Isaac and Garet, with Ivan and Mia, have climbed the lighthouse after
/// Saturos and Menardi, who hold Jenna and Kraden and carry the Elemental
/// Stars. They come too late to stop the beacon being lit.
///
/// - The first arrival (MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT,
///   MercuryLighthouseAerie_PartyArrives). The Adepts of the lit element regain
///   their PP, the aerie shakes, and in a flash the party stands at the top.
///   Touching cell behavior 5 (MercuryLighthouseAerie_ConfrontSaturos), Garet
///   sees `It's too late! The lighthouse has already been lit!` Mia learns that
///   the party came from Vale to recover the Elemental Stars, and that Vale
///   protected Sol Sanctum as her clan protects the lighthouse. Jenna calls
///   out, and Felix, Menardi and Saturos face the party. Saturos sends Menardi
///   on to the next lighthouse with Jenna, Kraden and Felix, and fights the
///   party alone (ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, slot 2), the light of
///   Mercury weakening his Psynergy.
/// - Back from the battle (MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE,
///   MercuryLighthouseAerie_AlexAppears): `Isaac, we got him!` Alex, the
///   apprentice Mia lost, appears. He explains that Saturos, aligned with Mars,
///   the clan of fire, had his power bound by the lighthouse's water, which
///   lent Mia, of the Mercury Clan like Alex, endless Psynergy. Alex was only
///   stalling until Saturos recovered. Saturos rises, Alex asks whether the
///   party still holds the Mars Star, and the two exit over to the lift and
///   ride it down. Mia means to go with the party, since with the lighthouse
///   lit the fountain will run over with healing water again
///   (FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED, and
///   FLAG_SATUROS_BEATEN_AT_MERCURY, which Imil, Bilibin and the lighthouse
///   test).
/// - Getting about. Six platforms dip under the leader, who jumps two cells
///   from one to the next (cell behaviors 10 and 11;
///   places/mercury-lighthouse-aerie/events/MercuryLighthouseAerie_StepOntoPlatform.gif).
///   The lift leads down to Mercury Lighthouse's
///   SceneId_MercuryLighthouseExterior (mercury_lighthouse.c), arriving at
///   MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE by the Fountain
///   of Healing. Cell behavior 6 calls the lift up, and cell behavior 12 rides
///   it down (MERCURY_LIGHTHOUSE_AERIE_TO_MERCURY_LIGHTHOUSE_EXTERIOR).
///   Arriving by it (MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR)
///   leaves it up
///   (places/mercury-lighthouse-aerie/events/MercuryLighthouseAerie_RideLiftDown.gif).
///   A Psynergy Stone can be found here.
///
/// Every arrival sets FLAG_MERCURY_LIGHTHOUSE_LIT, under which Mercury Adepts
/// regain PP faster.
///
/// On the aerie Retreat is blocked, the beacon flickers and sheds sparks by
/// task (MercuryLighthouseAerie_FlickerBeacon), and the platforms start 50
/// pixels down.
///
/// Jenna, Kraden, Felix and Menardi are placed only on the first arrival. Every
/// other entrance sets FLAG_HOSTAGES_GONE before the actor placements are read.
///
/// Unused: SceneId_UnusedMercuryLighthouseAerieCopy, which the area names call
/// Mercury Lighthouse and the debug scene list labels `MTO07NG`, loads this
/// overlay and the same map. The setup does nothing for it, and no exit, table
/// or code leads there. Only the debug scene selector, which reaches every
/// scene, can.

#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "scene/import.h"
#include "types.h"
#include "field/scene_tables.h"

extern struct SceneEntrance gMercuryLighthouseAerieEntrances[];
extern u32 gMercuryLighthouseAerieExits[];
extern struct ActorPlacement gMercuryLighthouseAerieActorPlacements[];
extern struct Trigger gMercuryLighthouseAerieTriggers[];
extern s32 gMercuryLighthouseAerie_PlatformUnderfoot;
extern s32 gMercuryLighthouseAerie_SparkScript[];

#include "scene/field_effect_spawn.h"
#include "scene/actor_nearest.h"
#include "field/field_sequence.h"
#include "system/main_routine.h"
#include "field/event_commands.h"
#include "game/game.h"
#include "field/event.h"
#include "system/system_call.h"
#include "field/create_configured_actor.h"
#include "system/heap_slots.h"
#include "battle/battle.h"
#include "debug.h"
#include "field/field.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/iwram_call.h"
#include "field/map_grid_scroll.h"
#include "text/msg_ids.h"
#include "macros.h"
#include "flags.h"
#include "sound/sound_driver_symbols.h"
#include "system/io_reg.h"
#include "ability_ids.h"

/// The aerie's actors.
///
/// The party and Jenna stand in their own slots. The speakers are known from
/// the text: Kraden ("It's all right, Jenna."), Felix ("Jenna, please... Just
/// do as she says"), Saturos (Menardi: "Oh, Saturos..."), Menardi (Saturos:
/// "Menardi... Wait!") and Alex (Mia: "Alex!"). Saturos is fought as
/// ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, slot 2.
enum MercuryLighthouseAerieActorId {
  /// Flickers between two sizes and sheds sparks while in view
  /// (MercuryLighthouseAerie_FlickerBeacon).
  ACTOR_BEACON = ACTOR_FIRST_PLACED,
  /// A statue, drawn as the lighthouse's statues all are, standing on the lift.
  /// It rides up and down with it.
  ACTOR_LIFT_STATUE,
  /// Four more statues flank the beacon, two on each side of it. The two east
  /// of it, ACTOR_FIRST_BEACON_STATUE + 2 and + 3, are drawn mirrored.
  ACTOR_FIRST_BEACON_STATUE,
  /// Six of them.
  ACTOR_FIRST_PLATFORM = ACTOR_FIRST_PLACED + 6,
  /// The two platforms that stand on the lift and go up and down with it.
  ACTOR_FIRST_LIFT_PLATFORM = ACTOR_FIRST_PLATFORM + 3,
  ACTOR_SECOND_LIFT_PLATFORM,
  ACTOR_KRADEN = ACTOR_FIRST_PLACED + 12,
  ACTOR_FELIX,
  ACTOR_SATUROS,
  ACTOR_MENARDI,
  ACTOR_ALEX
};

/// Animations of the aerie's sprites beyond enum ActorAnimation, as their
/// animation tables and frames under graphics/sprite draw them.
enum MercuryLighthouseAerieAnimation {
  /// Mia seen side-on, her head bowed and her hands clasped at her chest
  /// (mia.png).
  ANIM_MIA_HEAD_BOWED = 16
};

/// Scene flags, kept while the party stays on the aerie.
enum {
  /// The leader stands on a platform, which has dipped under the weight.
  FLAG_ON_PLATFORM = FLAG_SCENE_TEMP_50,
  FLAG_LIFT_UP = FLAG_SCENE_TEMP_51,
  /// Set on every arrival but the first, because Jenna, Kraden, Felix and
  /// Menardi are placed only before it.
  FLAG_HOSTAGES_GONE = FLAG_SCENE_TEMP_53
};

enum { FLAG_PSYNERGY_STONE = FLAG_AREA_TEMP_50 };

static void MercuryLighthouseAerie_PartyArrives(void);
static void MercuryLighthouseAerie_SettlePlatforms(void);
static void MercuryLighthouseAerie_AlexAppears(void);
static void MercuryLighthouseAerie_SaturosAndAlexWarpOut(void);
static void MercuryLighthouseAerie_SaturosAndAlexWarpIn(void);
static void MercuryLighthouseAerie_LowerLift(void);
static void MercuryLighthouseAerie_RaiseLift(void);
static void MercuryLighthouseAerie_FlickerBeacon(void);

#include "shared/field_effect_create.c"

#include "shared/field_effect_spawn.c"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gMercuryLighthouseAerieEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

/// Returns the one exit, the lift down to SceneId_MercuryLighthouseExterior's
/// MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE, by the Fountain
/// of Healing.
const u32 *Scene_GetExits(void) { return gMercuryLighthouseAerieExits; }

/// Returns the actors, with Jenna, Kraden, Felix and Menardi placed only on the
/// first arrival (MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT).
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.entrance != MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT) {
    GameFlag_Set(FLAG_HOSTAGES_GONE);
  }
  return gMercuryLighthouseAerieActorPlacements;
}

const struct Trigger *Scene_GetTriggers(void) {
  return gMercuryLighthouseAerieTriggers;
}

/// Sets up the Lighthouse Aerie, then plays the event of the entrance taken.
s32 Scene_Initialize(void) {
  u32 i;
  s32 actorId;
  struct Actor *actor;

  GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_LIT);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gMainRoutineWork.next == SCENE_MERCURY_LIGHTHOUSE_AERIE) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    Task_AddFrame(MercuryLighthouseAerie_FlickerBeacon, TASK_PASS_FRAME);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
    Actor_SetSpritePriority(ACTOR_GARET, 1);
    Actor_SetSpritePriority(ACTOR_IVAN, 1);
    Actor_SetSpritePriority(ACTOR_MIA, 1);
    Actor_SetSpritePriority(ACTOR_JENNA, 1);
    Actor_SetSpritePriority(ACTOR_KRADEN, 1);
    Actor_SetSpritePriority(ACTOR_FELIX, 1);
    Actor_SetSpritePriority(ACTOR_SATUROS, 1);
    Actor_SetSpritePriority(ACTOR_MENARDI, 1);
    Actor_SetSpritePriority(ACTOR_ALEX, 1);
    Actor_SetSpritePriority(ACTOR_BEACON, 1);
    Actor_SetSpritePriority(ACTOR_LIFT_STATUE, 1);
    Actor_SetSpritePriority(ACTOR_FIRST_BEACON_STATUE, 1);
    Actor_SetSpritePriority(ACTOR_FIRST_BEACON_STATUE + 1, 1);
    Actor_SetSpritePriority(ACTOR_FIRST_BEACON_STATUE + 2, 1);
    Actor_SetSpritePriority(ACTOR_FIRST_BEACON_STATUE + 3, 1);
    for (i = ACTOR_FIRST_PLATFORM; i < ACTOR_FIRST_PLATFORM + 6; i++) {
      Actor_SetSpritePriority(i, 1);
      Actor_Get(i)->motionFlags = ACTOR_FLOATS;
      Actor_Get(i)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      Actor_Get(i)->y.fixed = -(px(50) + px(1) / 2);
    }
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
        (actorId = Actor_FindNearestFloatingPlatform()) != 0 &&
        (actor = Actor_Get(actorId)) != NULL) {
      actor->motionFlags = 0;
    }
    Actor_SetSpriteFlags(Actor_Get(ACTOR_LIFT_STATUE), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_BEACON_STATUE), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_BEACON_STATUE + 1), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_BEACON_STATUE + 2), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_BEACON_STATUE + 3), 0);
    Actor_Get(ACTOR_FIRST_BEACON_STATUE + 2)->scaleX = -Q16_ONE;
    Actor_Get(ACTOR_FIRST_BEACON_STATUE + 3)->scaleX = -Q16_ONE;
    if (gMainRoutineWork.entrance ==
        MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT) {
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        MercuryLighthouseAerie_PartyArrives();
      }
    } else if (gMainRoutineWork.entrance ==
               MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE) {
      if (!GameFlag_Get(FLAG_LIFT_UP)) {
        {
          struct MapGridLayerScroll *lift =
              &((struct MapGridScrollWork *)gMapGridWork[0])->layers[2];

          lift->offsetY = px(1024);
        }
        MapGrid_CenterOnFollowedPoint();
        wait(1);
        MapGrid_CopyCellsTo(4, 70, 4, 74, 5, 4);
        Actor_SetPosition(ACTOR_LIFT_STATUE, 0, 0);
        if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
          MercuryLighthouseAerie_AlexAppears();
        }
      }
    } else if (gMainRoutineWork.entrance ==
               MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR) {
      GameFlag_Set(FLAG_LIFT_UP);
    }
  }
  return 0;
}

/// Lighthouse Aerie, at MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT unless
/// returning after a battle: the party arrives at the top.
static void MercuryLighthouseAerie_PartyArrives(void) {
  struct Actor *leader;

  Event_Start();
  Party_RestorePpOfLitElements();
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MIA), 0);
  FieldCamera_MoveTo(304, CAMERA_KEEP, 120, 0);
  wait(1);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Sound_Play(SE_RUMBLE);
  MapGrid_SetShake(5, 5, 1);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  MapGrid_WaitForShakeToSettle();
  Event_Wait(30);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(512, -24, 160, 1);
  FieldCamera_WaitForArrival();
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_PALE, PALETTES_ALL);
  PaletteFade_StartInterpolation(50);
  Event_Wait(50);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_StartInterpolation(30);
  Event_Wait(30);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 504, 168);
  Actor_SetPosition(ACTOR_GARET, 528, 144);
  Actor_SetPosition(ACTOR_IVAN, 488, 144);
  Actor_SetPosition(ACTOR_MIA, 512, 152);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_COLLAPSE);
  Actor_SetAnimation(ACTOR_GARET, ANIM_COLLAPSE);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_COLLAPSE);
  Actor_SetAnimation(ACTOR_MIA, ANIM_COLLAPSE);
  Event_Wait(10);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(30);
  Event_Wait(30);
  Event_Wait(80);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(60);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), SPRITE_FLAG_SHADOW);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN), SPRITE_FLAG_SHADOW);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Event_Wait(40);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MIA), SPRITE_FLAG_SHADOW);
  Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_SetSpeed(ACTOR_IVAN, 0.8);
  Actor_SetSpeed(ACTOR_MIA, 0.8);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
  Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                         leader->z.part.pixel);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_SetDestination(ACTOR_IVAN, leader->x.part.pixel,
                         leader->z.part.pixel);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_SetDestination(ACTOR_MIA, leader->x.part.pixel, leader->z.part.pixel);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Event_End();
}

#include "shared/actor_distance.c"
#include "shared/actor_nearest.c"

/// Lighthouse Aerie: walking into cell behavior 10 or 11.
static void MercuryLighthouseAerie_JumpToPlatform(void) {
  struct Actor *actor;
  struct FieldPosition position;
  s32 angle;
  u8 flags;

  actor = Actor_Get(ACTOR_PARTY_LEADER);
  angle = (actor->facing + TRIG_TURN / 16) & 0xe000;
  flags = actor->motionFlags;
  SET_POSITION(&position, (actor->x.fixed & FIELD_CELL_MASK) + px(8),
               actor->y.fixed, (actor->z.fixed & FIELD_CELL_MASK) + px(8));
  Vector_AddPolarOffset(px(32), angle, &position);
  if (FieldActor_CheckMovementCollision(actor, &position) == 0) {
    GameFlag_Clear(FLAG_ON_PLATFORM);
    MercuryLighthouseAerie_SettlePlatforms();
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);
    actor->speedLimit = px(3);
    actor->acceleration = px(2);
    actor->velocityY = px(4);
    actor->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
    Actor_SetSpriteFlags(actor, 0);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, position.x >> 16, position.z >> 16);
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    actor->motionFlags = flags;
  }
}

/// Lighthouse Aerie: touching cell behavior 10 when not already on a platform.
static void MercuryLighthouseAerie_StepOntoPlatform(void) {
  struct Actor *leader;
  struct Actor *actor;
  s32 id;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Start();
  id = Actor_FindNearestFloatingPlatform();
  gMercuryLighthouseAerie_PlatformUnderfoot = id;
  if (id != 0) {
    GameFlag_Set(FLAG_ON_PLATFORM);
    actor = Actor_Get(gMercuryLighthouseAerie_PlatformUnderfoot);
    actor->motionFlags = 0;
    leader->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
    actor->y.fixed -= px(3);
    leader->y.fixed -= px(3);
    leader->groundHeight -= px(3);
    wait(2);
    actor->y.fixed -= px(2);
    leader->y.fixed -= px(2);
    leader->groundHeight -= px(2);
    wait(10);
    actor->y.fixed += px(2);
    leader->y.fixed += px(2);
    leader->groundHeight += px(2);
    wait(4);
    actor->y.fixed += px(2);
    leader->y.fixed += px(2);
    leader->groundHeight += px(2);
    wait(4);
    actor->y.fixed += px(1);
    leader->y.fixed += px(1);
    leader->groundHeight += px(1);
  }
  Event_End();
}

/// Lighthouse Aerie: touching cell behavior 11, off the platforms.
static void MercuryLighthouseAerie_SettlePlatforms(void) {
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_Get(ACTOR_FIRST_PLATFORM)->motionFlags = ACTOR_FLOATS;
  Actor_Get(ACTOR_FIRST_PLATFORM + 1)->motionFlags = ACTOR_FLOATS;
  Actor_Get(ACTOR_FIRST_PLATFORM + 2)->motionFlags = ACTOR_FLOATS;
  Actor_Get(ACTOR_FIRST_LIFT_PLATFORM)->motionFlags = ACTOR_FLOATS;
  Actor_Get(ACTOR_SECOND_LIFT_PLATFORM)->motionFlags = ACTOR_FLOATS;
  Actor_Get(ACTOR_FIRST_PLATFORM + 5)->motionFlags = ACTOR_FLOATS;
}

/// Lighthouse Aerie: touching cell behavior 5 before the confrontation.
static void MercuryLighthouseAerie_ConfrontSaturos(void) {
  struct Actor *actor;

  Event_Start();
  Actor_Get(ACTOR_FIRST_LIFT_PLATFORM)->motionFlags &=
      ~(ACTOR_FOLLOWS_TERRAIN | ACTOR_FLOATS);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_SetSpeed(ACTOR_IVAN, 0.8);
  Actor_SetSpeed(ACTOR_MIA, 0.8);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFixed(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFixed(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFixed(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
  }
  wait(1);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 344, 232);
  Actor_WalkTo(ACTOR_GARET, 328, 232);
  Actor_WalkTo(ACTOR_IVAN, 344, 248);
  Actor_WalkTo(ACTOR_MIA, 328, 248);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Event_Wait(50);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  FieldCamera_SetSpeed(1.5);
  FieldCamera_MoveTo(328, 40, 176, 1);
  FieldCamera_GetActor()->motionFlags = 0;
  Actor_SetSpeed(ACTOR_GARET, 1.5);
  Actor_WalkToAndWait(ACTOR_GARET, 328, 216);
  FieldCamera_WaitForArrival();
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Event_Wait(60);
  FieldCamera_MoveTo(344, 24, 232, 1);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
  FieldCamera_WaitForArrival();
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_SetMessageId(MsgMercuryLighthouseAlreadyLit);
  // "It's too late! The lighthouse has already been lit!"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_SetSpeed(ACTOR_MIA, 1.5);
  Actor_WalkToAndWait(ACTOR_MIA, 328, 232);
  Event_Wait(10);
  // "It... It can't be!"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "The beacon cannot be lit without the Mercury Star..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 30);
  Actor_TurnToFaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  Event_Wait(60);
  Actor_WalkToAndWait(ACTOR_IVAN, 336, 248);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Event_Wait(30);
  // "...The Elemental Stars were stolen from Sol Sanctum, on Mt...."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 30);
  // "Mt. Aleph... Sol Sanctum... Are you healers from Sol Sanctum?"
  Event_OpenMessage(ACTOR_MIA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 10);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    // "We came from a village called Vale to recover the three..."
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    // "But, unlike you, we are not healers."
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    gFieldWork->message += 2;
  } else {
    gFieldWork->message += 2;
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 10);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    // "We came from Mt. Aleph, but we're not healers."
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(20);
    // "We came from a village called Vale to recover the three..."
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  }
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "Did you say Vale?"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  // "Our village was there to protect Sol Sanctum, but the Stars..."
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  // "That sounds just like my clan..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 10);
  Actor_SetAnimation(ACTOR_MIA, ANIM_MIA_HEAD_BOWED);
  // "I have failed in the one duty placed upon me... This is..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 60);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_SetPosition(ACTOR_JENNA, 216, 200);
  // "Isaac! Garet!"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_SetPosition(ACTOR_JENNA, 120, 160);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 10);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 5);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 5);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 5);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  FieldCamera_SetSpeed(3);
  FieldCamera_MoveTo(120, -24, 168, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_Wait(20);
  Sound_Play(MUS_OMINOUS);
  // "So it's you, Isaac!"
  Event_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  Event_Wait(20);
  // "Heh heh! Those kids are still alive?"
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH, 60);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 20);
  // "You came all this way to save Jenna and Kraden?"
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_SetPosition(ACTOR_GARET, 264, 288);
  // "That's not all... We're after the Elemental Stars!"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_SetPosition(ACTOR_GARET, 328, 216);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "In other words, they've come to stop us."
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  Event_Wait(20);
  // "Then we have no choice... Let's take care of them now,..."
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTHWEST, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 10);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 30);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 10);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  Actor_SetSpeed(ACTOR_FELIX, 0.8);
  Actor_WalkToAndWait(ACTOR_FELIX, 104, 168);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  Actor_SetPosition(ACTOR_SATUROS, 264, 288);
  // "Menardi... Wait!"
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_SetPosition(ACTOR_SATUROS, 296, 120);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldCamera_SetSpeed(1.5);
  FieldCamera_MoveTo(232, 40, 152, 1);
  Actor_SetSpeed(ACTOR_SATUROS, 0.8);
  Actor_WalkToAndWait(ACTOR_SATUROS, 272, 128);
  Actor_WalkToAndWait(ACTOR_SATUROS, 264, 152);
  Actor_WalkToAndWait(ACTOR_SATUROS, 280, 168);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 20);
  Event_Wait(20);
  // "Oh, Saturos... They want to..."
  Event_ShowMessage(ACTOR_MENARDI, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  Event_Wait(20);
  // "I heard everything."
  Event_ShowMessage(ACTOR_SATUROS, 0);
  FieldCamera_MoveTo(296, 40, 216, 1);
  Actor_WalkToAndWait(ACTOR_SATUROS, 304, 176);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTHEAST, 0);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_Wait(20);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  Event_Wait(20);
  // "They were lucky to survive the eruption of Mt. Aleph."
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 70);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_Wait(30);
  // "You have endured so much, and now you want to throw your..."
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  Event_Wait(20);
  // "Fine. If that is your wish, prepare to have it granted."
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Event_Wait(20);
  // "Saturos... Are you going to fight them alone?"
  Event_ShowMessage(ACTOR_MENARDI, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(40);
  // "Menardi, I can't expect you to leave our hostages alone and..."
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Event_Wait(30);
  // "Umm... That's true..."
  Event_ShowMessage(ACTOR_MENARDI, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(30);
  // "Besides, I want to see how much their powers have developed."
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Event_Wait(20);
  Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkTo(ACTOR_GARET, 328, 224);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 344, 224);
  Actor_WalkTo(ACTOR_IVAN, 344, 232);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_Get(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetPosition(ACTOR_MENARDI, 168, 200);
  // "Are you sure about this, Saturos?"
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_SetPosition(ACTOR_MENARDI, 104, 200);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_Wait(20);
  // "Leave me be! Head for the next lighthouse-now!"
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  FieldCamera_SetSpeed(3);
  FieldCamera_MoveTo(120, -24, 168, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  Event_Wait(20);
  // "Come here, child..."
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 70);
  Actor_TurnToFaceEachOther(ACTOR_JENNA, ACTOR_KRADEN, 0);
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  // "Do you mean to defy me?"
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHEAST, 20);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  Event_Wait(20);
  // "I should teach you a lesson, child, but..."
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Felix, I'd get your sister to obey me, if you hope to keep..."
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Jenna, please... Just do as she says for now."
  Event_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  // "Felix..."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 40);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(20);
  // "It's all right, Jenna."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 30);
  Actor_SetSpeed(ACTOR_JENNA, 0.7);
  Actor_SetSpeed(ACTOR_KRADEN, 0.7);
  Actor_WalkTo(ACTOR_JENNA, 128, 144);
  Actor_WalkToAndWait(ACTOR_KRADEN, 120, 136);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 20);
  Actor_Get(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_FELIX, 88, 152);
  Actor_Get(ACTOR_FELIX)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHWEST, 20);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(40);
  Actor_SetSpeedAndAcceleration(ACTOR_MENARDI, 3, 2);
  Actor_Get(ACTOR_MENARDI)->velocityY = px(4);
  Sound_Play(SE_JUMP);
  Actor_Get(ACTOR_MENARDI)->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MENARDI), 0);
  Actor_Get(ACTOR_FIRST_LIFT_PLATFORM)->motionFlags = ACTOR_FLOATS;
  Actor_MoveToAndWait(ACTOR_MENARDI, 104, 168);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MENARDI), SPRITE_FLAG_SHADOW);
  Actor_Get(ACTOR_MENARDI)->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 10);
  // "Isaac and Garet shouldn't have come after us..."
  Event_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "I'm sorry, Isaac, Garet..."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  // "I'm counting on you!"
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 0);
  Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 0);
  MercuryLighthouseAerie_LowerLift();
  Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 1);
  Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
  FieldCamera_MoveTo(304, 32, 216, 1);
  FieldCamera_WaitForArrival();
  Actor_SetPosition(ACTOR_KRADEN, 272, 280);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_SetSpeed(ACTOR_GARET, 1.5);
  Actor_WalkToAndWait(ACTOR_GARET, 312, 216);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 10);
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_WalkToAndWait(ACTOR_SATUROS, 312, 184);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHEAST, 20);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_SetSpeed(ACTOR_GARET, 2);
  Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_GARET, 328, 224);
  Event_Wait(1);
  Actor_Get(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_OpenMessage(ACTOR_SATUROS, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    gFieldWork->message++;
  } else {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    gFieldWork->message++;
    Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  }
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_WalkToAndWait(ACTOR_SATUROS, 328, 200);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 20);
  Actor_Get(ACTOR_SATUROS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_SATUROS, 336, 208);
  Event_Wait(1);
  Actor_Get(ACTOR_SATUROS)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_WalkToAndWait(ACTOR_SATUROS, 336, 216);
  Event_SetAfterBattleDestination(SCENE_MERCURY_LIGHTHOUSE_AERIE, 2);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  // Saturos: ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, slot 2.
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY,
                                        2);
  Event_End();
}

/// Lighthouse Aerie, at MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE, back
/// from the battle, unless returning after another: Saturos is beaten, but not
/// for long.
static void MercuryLighthouseAerie_AlexAppears(void) {
  Event_Start();
  Actor_Get(ACTOR_LIFT_STATUE)->motionFlags = 0;
  Actor_SetPosition(ACTOR_PARTY_LEADER, 344, 224);
  Actor_SetPosition(ACTOR_GARET, 328, 224);
  Actor_SetPosition(ACTOR_IVAN, 344, 232);
  Actor_SetPosition(ACTOR_MIA, 328, 232);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_SetPosition(ACTOR_SATUROS, 336, 176);
  Actor_SetAnimation(ACTOR_SATUROS, SATUROS_ANIMATION_LIE);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SATUROS), 0);
  FieldCamera_MoveTo(336, CAMERA_KEEP, 208, 0);
  wait(1);
  MapGrid_CenterOnFollowedPoint();
  wait(1);

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 20);
  Event_SetMessageId(MsgMercuryGot);
  // "Isaac, we got him!"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_Wait(20);
  // "Who would have thought that Mercury Lighthouse held such..."
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "Saturos... You said you were weakened. What did you mean?"
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_Wait(20);
  // "I... I meant nothing..."
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 30);
  Actor_SetPosition(ACTOR_ALEX, 312, 112);
  Sound_Play(SE_STOP_ACTION_SE);
  // "I shall tell you what he means..."
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  // "That voice! Could it be?"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 30);
  Sound_Play(MUS_CONFRONTATION);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(336, CAMERA_KEEP, 168, 1);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_ALEX, 0.8);
  Actor_WalkToAndWait(ACTOR_ALEX, 344, 136);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 20);
  FieldCamera_WaitForArrival();
  // "Alex!"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 30);
  FieldCamera_MoveTo(336, CAMERA_KEEP, 184, 1);
  Actor_WalkToAndWait(ACTOR_ALEX, 344, 160);
  Actor_WalkToAndWait(ACTOR_ALEX, 328, 168);
  Actor_WalkToAndWait(ACTOR_ALEX, 312, 176);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 20);
  FieldCamera_WaitForArrival();
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  Event_Wait(20);
  // "It's been a while, Mia."
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  // "Alex, do you realize what you've done?"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Ha! Of course... I have freed a great power that has long..."
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 60);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
  // "Mercury, the lighthouse of Water... What a mighty force."
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Alex, you... Are you mad!?"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_ALEX, 2);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 20);
  // "No, Mia... Don't you understand?"
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "The lighthouse granted you great power during your battle..."
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  // "It's true... I could use my power without ever depleting it."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  Event_Wait(20);
  // "Don't you see? The Mercury Lighthouse supplied you with..."
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_Wait(20);
  // "Psynergy? Are you telling me my powers come from Psynergy?"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_WalkToAndWait(ACTOR_ALEX, 312, 184);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 20);
  // "You and I are both members of Mercury Clan... Masters of..."
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 20);
  // "Mia is an Adept too, isn't she?"
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    // "Geez, Isaac, you knew all along, and you didn't tell us?"
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    // "I know it's hard to believe, but it looks like Mia's one of us."
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  }
  // "Tell me, Alex... Why wasn't Saturos able to use his full power?"
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  Event_Wait(20);
  // "Ah, yes... I shall explain it all."
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHEAST, 20);
  // "Saturos is aligned with Mars, the clan of fire."
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Fire and water are opposing elements... His full power was..."
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "Didn't Saturos know that?"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Of course he knew."
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 20);
  // "However, he failed to appreciate the power of this lighthouse."
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  Event_Wait(30);
  // "As did I..."
  Event_ShowMessage(ACTOR_ALEX, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(30);
  // "Is that why we were able to defeat Saturos?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "No... There is more to it."
  Event_ShowMessage(ACTOR_ALEX, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  Event_Wait(30);
  // "I stood by, watching your battle from afar..."
  Event_ShowMessage(ACTOR_ALEX, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  // "You were watching us?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  Event_Wait(30);
  // "Yes. I did not help Saturos, because I was certain he would win."
  Event_ShowMessage(ACTOR_ALEX, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  Event_Wait(30);
  // "But I was wrong... You have become such great fighters in a..."
  Event_ShowMessage(ACTOR_ALEX, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_SATUROS, 8);
  Event_Wait(45);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SATUROS), SPRITE_FLAG_SHADOW);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  // "Saturos is up again!? But we beat him!"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  // "Me? Beaten? Never! I lost to this lighthouse, not to you!"
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  // "I was merely stalling you until Saturos regained his strength."
  Event_ShowMessage(ACTOR_ALEX, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(30);
  // "You were just buying time! That's not fair, Alex!"
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(ACTOR_ALEX, ACTOR_SATUROS, 0);
  Event_Wait(35);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  Event_Wait(20);
  Actor_WalkToAndWait(ACTOR_ALEX, 328, 176);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 20);
  Actor_SetAnimation(ACTOR_ALEX, 5);
  Actor_SetAnimation(ACTOR_SATUROS, 7);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  // "I won't let Saturos escape!"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "And just what are you going to do? Will you finish him off?"
  Event_OpenMessage(ACTOR_ALEX, 0);
  Event_Wait(30);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  Event_Wait(20);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(30);
    // "If that's the case, you will have to fight me, as well. I've..."
    Event_ShowMessage(ACTOR_ALEX, 0);
    gFieldWork->message++;
  } else {
    Event_Wait(30);

    gFieldWork->message++;
    // "Yes, Isaac. You seem like a smart warrior. You can't beat..."
    Event_ShowMessage(ACTOR_ALEX, 0);
  }
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Event_Wait(20);
  // "And I doubt you are the type to 'finish' anyone off!"
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_Wait(20);
  // "Of course, you are welcome to believe whatever you want."
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  MercuryLighthouseAerie_SaturosAndAlexWarpOut();
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 5);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 5);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 5);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 5);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 5);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 5);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 5);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 5);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(280, CAMERA_KEEP, 232, 1);
  FieldCamera_WaitForArrival();
  Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_HIDDEN);
  Actor_SetPosition(ACTOR_SATUROS, 240, 208);
  Actor_SetPosition(ACTOR_ALEX, 232, 208);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
  MercuryLighthouseAerie_SaturosAndAlexWarpIn();
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 15);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 328, 208);
  Actor_WalkTo(ACTOR_IVAN, 336, 224);
  Actor_WalkToAndWait(ACTOR_GARET, 312, 216);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 30);
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 20);
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 20);
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  FieldCamera_SetSpeed(3);
  {
    struct MapGridLayerScroll *lift;
    void **cells;
    FieldCamera_MoveTo(152, CAMERA_KEEP, 216, 1);
    lift = &((struct MapGridScrollWork *)gMapGridWork[0])->layers[2];
    lift->offsetY = px(896);
    cells = gMapGridWork;
    MapGrid_CenterOnFollowedPoint();
    wait(1);
    Actor_Get(ACTOR_LIFT_STATUE)->motionFlags = 0;
    Actor_SetPosition(ACTOR_LIFT_STATUE, 104, 264);
    Actor_Get(ACTOR_LIFT_STATUE)->y.fixed = -px(32);
    Actor_Get(ACTOR_LIFT_STATUE)->targetY = -px(32);
    MapGrid_CopyCellsTo(29, 74, 4, 74, 5, 4);
    Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 0);
    Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 0);
    MercuryLighthouseAerie_RaiseLift();
    Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 1);
    Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_ALEX, FACING_WEST, 20);
    Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
    FieldCamera_MoveTo(280, CAMERA_KEEP, 216, 1);
    FieldCamera_WaitForArrival();
    Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 20);
    Actor_SetAnimation(ACTOR_ALEX, 5);
    Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
    Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_Wait(20);
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    Event_OpenMessage(ACTOR_ALEX, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_Wait(20);
      Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
      Event_Wait(20);
      Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
      ((struct FieldWork *)cells[19])->message++;
    } else {
      ((struct FieldWork *)cells[19])->message++;
      Event_Wait(20);
      Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    }
  }
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_OpenMessage(ACTOR_ALEX, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
    Event_Wait(20);
    Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 1);
    Event_Wait(20);
    Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  }
  Event_Wait(20);
  MercuryLighthouseAerie_SaturosAndAlexWarpOut();
  Event_Wait(20);
  FieldCamera_MoveTo(128, CAMERA_KEEP, 200, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_STAND);
  Actor_SetAnimation(ACTOR_ALEX, ANIM_STAND);
  Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_HIDDEN);
  Actor_SetPosition(ACTOR_SATUROS, 120, 152);
  Actor_SetPosition(ACTOR_ALEX, 112, 160);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
  MercuryLighthouseAerie_SaturosAndAlexWarpIn();
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 20);
  Event_OpenMessage(ACTOR_ALEX, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
    Event_Wait(20);
    Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  }
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_EAST, 20);
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 20);
  Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 0);
  Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 0);
  MercuryLighthouseAerie_LowerLift();
  Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 1);
  Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 1);
  Sound_Play(SOUND_FADE_OUT_BGM);
  FieldCamera_MoveTo(336, CAMERA_KEEP, 216, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Scene_PlayMusic();
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 22);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
  Actor_SetAnimation(ACTOR_MIA, ANIM_MIA_HEAD_BOWED);
  Actor_Get(ACTOR_MIA)->scaleX = -Q16_ONE;
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_OpenMessage(ACTOR_IVAN, 0);
  Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
  Actor_Get(ACTOR_MIA)->scaleX = Q16_ONE;
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  }
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 30);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetSpeed(ACTOR_MIA, 0.8);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_WalkToAndWait(ACTOR_MIA, 328, 216);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkTo(ACTOR_GARET, 312, 200);
  Actor_WalkTo(ACTOR_MIA, 280, 216);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 20);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 30);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 40);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 312, 216);
  Actor_WalkTo(ACTOR_GARET, 312, 216);
  Actor_WalkTo(ACTOR_IVAN, 312, 216);
  Actor_WalkTo(ACTOR_MIA, 312, 216);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  {
    struct MapGridLayerScroll *lift =
        &((struct MapGridScrollWork *)gMapGridWork[0])->layers[2];

    lift->offsetY = px(1024);
  }
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  MapGrid_CopyCellsTo(4, 70, 4, 74, 5, 4);
  GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED);
  GameFlag_Set(FLAG_SATUROS_BEATEN_AT_MERCURY);
  Event_End();
}

/// Lighthouse Aerie: touching cell behavior 6 while the lift is down.
static void MercuryLighthouseAerie_CallLift(void) {
  struct MapGridLayerScroll *lift;

  lift = &((struct MapGridScrollWork *)gMapGridWork[0])->layers[2];
  Event_Start();
  lift->offsetY = px(896);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Actor_Get(ACTOR_LIFT_STATUE)->motionFlags = 0;
  Actor_SetPosition(ACTOR_LIFT_STATUE, 104, 264);
  Actor_Get(ACTOR_LIFT_STATUE)->y.fixed = -px(32);
  Actor_Get(ACTOR_LIFT_STATUE)->targetY = -px(32);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(128, CAMERA_KEEP, 184, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(30);
  MapGrid_CopyCellsTo(29, 74, 4, 74, 5, 4);
  Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 0);
  Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 0);
  MercuryLighthouseAerie_RaiseLift();
  Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 1);
  Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 1);
  Event_Wait(20);
  GameFlag_Set(FLAG_LIFT_UP);
  Event_End();
}

/// Lighthouse Aerie: touching cell behavior 12.
static void MercuryLighthouseAerie_RideLiftDown(void) {
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 104, 152);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 60);
  Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 0);
  Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 0);
  MercuryLighthouseAerie_LowerLift();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Import_EventSetExitRequest(
      MERCURY_LIGHTHOUSE_AERIE_TO_MERCURY_LIGHTHOUSE_EXTERIOR);
  Event_End();
}

/// Saturos and Alex exit out in a column of sparkles and vanish.
static void MercuryLighthouseAerie_SaturosAndAlexWarpOut(void) {
  struct EffectOptions options;
  struct Actor *left;
  struct Actor *right;
  u32 i;
  s32 x;
  s32 r;

  left = Actor_Get(ACTOR_SATUROS);
  right = Actor_Get(ACTOR_ALEX);
  Sound_Play(SE_WARP);
  Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_BLINK);
  Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_BLINK);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SATUROS), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), 0);
  options.priority = 1;
  options.palette = 5;
  options.type = SPRITE_PARTICLE;
  options.startScaleX = 2 * Q16_ONE / 5;
  options.startScaleY = 3 * Q16_ONE;
  for (i = 0; i <= 31; i++) {
    Event_Wait(1);
    if (i & 1) {
      r = ((Random_Below(24)) << 16);
      x = left->x.fixed;
      x += r;
      x -= px(12);
      FieldEffect_Spawn(x, left->y.fixed + ((Random_Below(32)) << 16) - px(16),
                        left->z.fixed, 0, px(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        &options);
    } else {
      r = ((Random_Below(24)) << 16);
      x = right->x.fixed;
      x += r;
      x -= px(12);
      FieldEffect_Spawn(x, right->y.fixed + ((Random_Below(32)) << 16) - px(16),
                        right->z.fixed, 0, px(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        &options);
    }
  }
  Actor_SetPosition(ACTOR_SATUROS, 0, 0);
  Actor_SetPosition(ACTOR_ALEX, 0, 0);
}

/// Saturos and Alex exit in where they have been placed.
static void MercuryLighthouseAerie_SaturosAndAlexWarpIn(void) {
  struct EffectOptions options;
  struct Actor *left;
  struct Actor *right;
  u32 i;
  s32 x;
  s32 r;

  left = Actor_Get(ACTOR_SATUROS);
  right = Actor_Get(ACTOR_ALEX);
  Sound_Play(SE_WARP);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SATUROS), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), 0);
  options.priority = 1;
  options.palette = 5;
  options.type = SPRITE_PARTICLE;
  options.startScaleX = 2 * Q16_ONE / 5;
  options.startScaleY = 3 * Q16_ONE;
  for (i = 0; i <= 31; i++) {
    Event_Wait(1);
    if (i & 1) {
      r = ((Random_Below(24)) << 16);
      x = left->x.fixed;
      x += r;
      x -= px(12);
      FieldEffect_Spawn(x, left->y.fixed + ((Random_Below(32)) << 16) + px(32),
                        left->z.fixed, 0, -px(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        &options);
    } else {
      r = ((Random_Below(24)) << 16);
      x = right->x.fixed;
      x += r;
      x -= px(12);
      FieldEffect_Spawn(x, right->y.fixed + ((Random_Below(32)) << 16) + px(32),
                        right->z.fixed, 0, -px(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        &options);
    }
    if (i == 20) {
      Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_BLINK);
      Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_BLINK);
    }
  }
  Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SATUROS), SPRITE_FLAG_SHADOW);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), SPRITE_FLAG_SHADOW);
}

/// MercuryLighthouseAerie_LowerLift passes its lift as a third
/// argument, which this function does not take.
static void MercuryLighthouseAerie_FindLiftRiders(s32 *out, s32 y) {
  struct FieldWork *work;
  u32 slot;

  work = gFieldWork;
  y = 64 - (y >> 20);
  for (slot = 0; slot < sizeof(work->actors) / sizeof(work->actors[0]);
       slot++) {
    struct Actor *actor = work->actors[slot];

    if (actor != NULL) {
      s32 cell = FIELD_CELL(actor->x.fixed);
      s32 row = FIELD_CELL(actor->z.fixed);

      if (cell >= 4 && cell <= 8 && y + 8 <= row && row < y + 11)
        *out++ = slot;
    }
  }
}

/// The lift sinks with everything standing on it, speeding up.
static void MercuryLighthouseAerie_LowerLift(void) {
  s32 ids[5];
  struct MapGridLayerScroll *lift;
  u32 i;
  u32 j;
  u32 n;
  s32 speed;
  struct Actor *actor;

  lift = &((struct MapGridScrollWork *)gMapGridWork[0])->layers[2];
  speed = px(1) / 10;
  n = 0;
  for (i = 0; i <= 4; i++)
    ids[i] = 66;
  // UB: the lift's layer is passed as a third argument to a routine that
  // takes two; UBFIX passes the two.
#ifdef UBFIX
  MercuryLighthouseAerie_FindLiftRiders(ids, lift->offsetY);
#else
  ((void (*)())MercuryLighthouseAerie_FindLiftRiders)(ids, lift->offsetY, lift);
#endif
  for (i = 0; i <= 4; i++) {
    if (ids[i] == 66)
      break;
    Actor_Get(ids[i])->motionFlags = 0;
    n++;
  }
  Sound_Play(SE_LIFT);
  for (i = 0; i <= 227; i++) {
    lift->offsetY -= speed;
    for (j = 0; j < n; j++) {
      Actor_Get(ids[j])->z.fixed += speed;
      actor = Actor_Get(ids[j]);
      actor->targetZ = Actor_Get(ids[j])->z.fixed;
    }
    if ((i & 3) == 3)
      speed += px(1) / 10;
    if (speed >= px(3) / 2)
      speed = px(3) / 2;
    wait(1);
  }
  for (i = 0; i < n; i++)
    Actor_Get(ids[i])->motionFlags = 0;
}

/// The lift rises with everything standing on it, slowing at the top.
static void MercuryLighthouseAerie_RaiseLift(void) {
  s32 ids[5];
  struct MapGridLayerScroll *lift;
  u32 i;
  u32 j;
  u32 n;
  s32 speed;
  struct Actor *actor;

  lift = &((struct MapGridScrollWork *)gMapGridWork[0])->layers[2];
  speed = px(3) / 2;
  n = 0;
  for (i = 0; i <= 4; i++)
    ids[i] = 66;
  MercuryLighthouseAerie_FindLiftRiders(ids, lift->offsetY);
  for (i = 0; i <= 4; i++) {
    if (ids[i] == 66)
      break;
    Actor_Get(ids[i])->motionFlags = 0;
    n++;
  }
  Sound_Play(SE_LIFT);
  for (i = 0; i <= 85; i++) {
    lift->offsetY += speed;
    for (j = 0; j < n; j++) {
      Actor_Get(ids[j])->z.fixed -= speed;
      actor = Actor_Get(ids[j]);
      actor->targetZ = Actor_Get(ids[j])->z.fixed;
    }
    if ((i & 3) == 3 && i > 75)
      speed -= px(1) / 5;
    if (speed < px(1) / 20)
      speed = px(1) / 20;
    wait(1);
  }
  lift->offsetY = px(1024);
  MapGrid_CenterOnFollowedPoint();
  wait(2);
}

/// Unused: Alex casts Ply on the lift's statue.
///
/// Nothing calls it. In the game, Alex and Saturos exit over to the lift and
/// ride it down without it (MercuryLighthouseAerie_AlexAppears).
UNUSED static void MercuryLighthouseAerie_UnusedAlexCastsPly(void) {
  // XXX (no evidence): what this was for; no event plays it.
  FieldPsynergy_LoadResources(ABILITY_PLY, 1);
  FieldPsynergy_SetupActorPair(ACTOR_ALEX, ACTOR_LIFT_STATUE);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  FieldPsynergy_StopCasterUpdate();
  FieldPsynergy_End();
}

/// A beacon spark's update, which swings it round the beacon it watches.
static void MercuryLighthouseAerie_UpdateBeaconSpark(struct Actor *spark) {
  struct Actor *beacon;
  u16 angle;

  beacon = spark->watchedActor;
  angle = spark->timer;
  spark->x.fixed = beacon->x.fixed + Trig_Cos(angle) * (spark->speedLimit + 28);
  spark->z.fixed = (Trig_Sin(angle) << 4) + px(144);
  spark->targetX = spark->x.fixed;
  spark->targetZ = spark->z.fixed;
  spark->timer -= 512;
}

/// Flickers the beacon and sheds its sparks, every frame.
static void MercuryLighthouseAerie_FlickerBeacon(void) {
  struct Actor *beacon;
  struct Actor *spark;
  struct Sprite *sprite;
  struct MapGridScrollWork *grid;
  s32 height;
  s32 scale;
  s32 cycleFrame;
  u32 angle;

  beacon = Actor_Get(ACTOR_BEACON);
  grid = gMapGridWork[0];
  height = (Random_Below(48)) << 16;
  if (((union FieldCoordinate *)&grid->view[1])->part.pixel <= 129) {
    if (gFrameCount & 1) {
      Import_ActorSetPosition(ACTOR_BEACON, px(304), px(144));
      scale = Q16_ONE;
      Actor_Get(ACTOR_BEACON)->scaleX = scale;
    } else {
      Import_ActorSetPosition(ACTOR_BEACON, px(304), px(151));
      scale = 13 * Q16_ONE / 10;
      Actor_Get(ACTOR_BEACON)->scaleX = scale;
    }
    Actor_Get(ACTOR_BEACON)->scaleY = scale;
  } else {
    Actor_SetPosition(ACTOR_BEACON, 8, 8);
  }
  if (beacon == NULL) {
    return;
  }
  cycleFrame = gFrameCount & 15;
  if (cycleFrame != 0) {
    return;
  }
  spark = FieldActor_Create(SPRITE_PARTICLE, beacon->x.fixed + px(8),
                            beacon->y.fixed + height + px(8), beacon->z.fixed);
  height = height / px(6);
  height <<= 16;
  if (spark == NULL) {
    return;
  }
  sprite = spark->sprite;
  Actor_StartScript(spark, gMercuryLighthouseAerie_SparkScript);
  Actor_SetSpriteColor(spark, SPRITE_COLOR_BGR);
  spark->motionFlags = cycleFrame;
  angle = Random_Next() & 0x0ffff000;
  spark->timer = angle;
  spark->userValue = cycleFrame;
  spark->watchedActor = beacon;
  spark->update = MercuryLighthouseAerie_UpdateBeaconSpark;
  spark->speedLimit = (Trig_Sin((height & 0xfffff) >> 4) * 24) >> 16;
  sprite->flags = 0;
  sprite->image.priority = beacon->sprite->image.priority;
}

#include "shared/field_effect_duration_scripts.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct SceneEntrance gMercuryLighthouseAerieEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT, CONDITION_ALWAYS, 8, 0, 8,
     FACING_SOUTH, 0, 0, 0, 608, 424, 0},
    {MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE, CONDITION_ALWAYS, 8, 0, 8,
     FACING_SOUTH, 0, 0, 0, 608, 424, 0},
    {MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR,
     CONDITION_ALWAYS, 504, 0, 152, FACING_SOUTH, 0, 0, 0, 608, 424, 0},
    {51, CONDITION_ALWAYS, 504, 0, 168, FACING_SOUTH, 0, 0, 0, 608, 424, 0},
    {SCENE_TABLE_END},
};

u32 gMercuryLighthouseAerieExits[] = {
    SCENE_EXITS(MercuryLighthouseAerie),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_AERIE_TO_MERCURY_LIGHTHOUSE_EXTERIOR,
               MercuryLighthouseExterior,
               MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gMercuryLighthouseAerieActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_JENNA, FLAG_HOSTAGES_GONE, MOVEMENT_TYPE_STAND,
        120, 0, 160, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        304, 40, 144, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        104, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        328, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        440, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        472, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        504, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        104, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        136, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        168, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_KRADEN, FLAG_HOSTAGES_GONE, MOVEMENT_TYPE_STAND,
        104, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FELIX, FLAG_HOSTAGES_GONE, MOVEMENT_TYPE_STAND,
        88, 0, 160, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MENARDI, FLAG_HOSTAGES_GONE, MOVEMENT_TYPE_STAND,
        104, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_EAST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};
// clang-format on

struct Trigger gMercuryLighthouseAerieTriggers[] = {
    {TRIGGER_STEP, 5, FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED,
     EVENT(MercuryLighthouseAerie_ConfrontSaturos)},
    {TRIGGER_STEP, 6, FLAG_LIFT_UP, EVENT(MercuryLighthouseAerie_CallLift)},
    {STEP_TRIGGER_WALKING, 11, CONDITION_ALWAYS,
     EVENT(MercuryLighthouseAerie_JumpToPlatform)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS,
     EVENT(MercuryLighthouseAerie_SettlePlatforms)},
    {STEP_TRIGGER_WALKING, 10, CONDITION_ALWAYS,
     EVENT(MercuryLighthouseAerie_JumpToPlatform)},
    {TRIGGER_STEP, 10, FLAG_ON_PLATFORM,
     EVENT(MercuryLighthouseAerie_StepOntoPlatform)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 100, FLAG_PSYNERGY_STONE,
     FIND_PSYNERGY_STONE},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS,
     EVENT(MercuryLighthouseAerie_RideLiftDown)},
    {TRIGGER_TABLE_END},
};

s32 gMercuryLighthouseAerie_PlatformUnderfoot = 0;

s32 gMercuryLighthouseAerie_SparkScript[] = {
    SC_SET(SW_SCALE_Y, 7 * Q16_ONE / 5),
    SC_SET(SW_SCALE_X, 7 * Q16_ONE / 5),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(30),
    SC_RELEASE,
};
