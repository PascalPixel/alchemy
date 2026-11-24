/// @file
/// Lighthouse Aerie (SceneId_MercuryLighthouseAerie), the top of Mercury
/// Lighthouse, where its beacon burns.
///
/// Isaac and Garet, with Ivan and Mia, have climbed the lighthouse after
/// Saturos and Menardi, who hold Jenna and Kraden and carry the Elemental
/// Stars. Ply on the statue below the rainbow turns the last waterfall to
/// stream upward, and the party comes out at the top. They come too late to
/// stop the beacon being lit.
///
/// The aerie is three tower tops in the sky. The party lands on the east one,
/// where a Psynergy Stone lies. The beacon stands on the middle one, at the
/// head of a wide flight of stairs, with two statues either side of it. The
/// west one is a lift, a tower top with a statue on it that sinks to the
/// ground and rises again. Three floating platforms bridge each gap, and the
/// leader jumps from one to the next.
///
/// - The arrival (MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT,
///   MercuryLighthouseAerie_PartyArrives). The Adepts of the lit element regain
///   their PP, the aerie shakes, and in a white flash the party lies on the
///   east tower top. They get up and gather round Isaac.
/// - The beacon (MercuryLighthouseAerie_ConfrontSaturos). Climbing the stairs
///   to it, Garet sees `It's too late! The lighthouse has already been lit!`
///   Mia learns that the party came from Vale to recover the Elemental Stars,
///   and that Vale protected Sol Sanctum as her clan protects the lighthouse.
///   Jenna calls out from the west tower top, where Kraden, Felix and Menardi
///   hold her. Saturos sends Menardi on to the next lighthouse with the others,
///   and Menardi leaps onto the lift, which sinks with them. Saturos fights the
///   party alone (ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, slot 2), the light of
///   Mercury weakening his Psynergy.
/// - Back from the battle (MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE,
///   MercuryLighthouseAerie_AlexAppears): `Isaac, we got him!` Alex, the
///   apprentice Mia lost, appears. He explains that Saturos, aligned with Mars,
///   the clan of fire, had his power bound by the lighthouse's water, which
///   lent Mia, of the Mercury Clan like Alex, endless Psynergy. Alex was only
///   stalling until Saturos recovered. Saturos rises, Alex asks whether the
///   party still holds the Mars Star, and the two vanish in sparkles,
///   reappear on the lift and ride it down. Mia means to go with the party,
///   since with the lighthouse lit the fountain will run over with healing
///   water again (FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED, and
///   FLAG_SATUROS_BEATEN_AT_MERCURY, which Imil, Bilibin and the lighthouse
///   test).
/// - Leaving. The stairs on the west side of the beacon's tower bring the
///   lift back up if it is down, and stepping onto it from the
///   westmost platform rides it down to the ground before Mercury Lighthouse,
///   by the Fountain of Healing (SceneId_MercuryLighthouseExterior,
///   mercury_lighthouse.c). Coming up that way
///   (MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR) leaves the
///   lift up.
///
/// Every arrival sets FLAG_MERCURY_LIGHTHOUSE_LIT, under which Mercury Adepts
/// regain PP faster.
///
/// On the aerie Retreat is blocked, and the beacon flickers and sheds sparks
/// (MercuryLighthouseAerie_FlickerBeacon).
///
/// Jenna, Kraden, Felix and Menardi are placed only on the first arrival. Every
/// other entrance sets FLAG_HOSTAGES_GONE before the actor placements are read.
///
/// Unused: SceneId_UnusedMercuryLighthouseAerieCopy, which the area names call
/// Mercury Lighthouse and the debug scene list labels `MTO07NG`, loads this
/// overlay and the same map. The setup does nothing for it, and no exit, table
/// or code leads there. Only the debug scene selector, which reaches every
/// scene, can.

#include "ability_ids.h"
#include "flags.h"
#include "macros.h"
#include "scene_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/create_configured_actor.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "field/map_grid_scroll.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/actor_nearest.h"
#include "scene/field_effect_spawn.h"
#include "scene/import.h"
#include "scene/sparkle_warp.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/heap_slots.h"
#include "system/io_reg.h"
#include "system/iwram_call.h"
#include "system/main_routine.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "system/system_call.h"
#include "text/msg_ids.h"

/// The cell behaviors the aerie's triggers answer.
enum MercuryLighthouseAerieCellBehavior {
  /// The stairs up to the beacon, cells (20, 14) and (21, 14).
  CELL_BEHAVIOR_BEACON_STAIRS = 5,
  /// The stairs on the west side of the beacon's tower, cell (12, 9), from
  /// which the lift is called up.
  CELL_BEHAVIOR_LIFT_LANDING = 6,
  /// The six floating platforms.
  CELL_BEHAVIOR_PLATFORM = 10,
  /// The ground beside the platforms: the foot of the stairs either side of
  /// the beacon's tower and the east tower top.
  CELL_BEHAVIOR_PLATFORM_EDGE = 11,
  /// The lift's floor, cell (6, 10), stepped onto from the westmost platform.
  CELL_BEHAVIOR_LIFT = 12,
  /// Where the Psynergy Stone lies on the east tower top, cell (31, 8).
  CELL_BEHAVIOR_PSYNERGY_STONE = 100
};

extern struct SceneEntrance gMercuryLighthouseAerieEntrances[];
extern u32 gMercuryLighthouseAerieExits[];
extern struct ActorPlacement gMercuryLighthouseAerieActorPlacements[];
extern struct Trigger gMercuryLighthouseAerieTriggers[];
extern s32 gMercuryLighthouseAerie_LandedPlatform;
extern s32 gBeacon_SparkScript[];

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
  /// The statue on the lift, the west tower top. It rides up and down with
  /// it.
  ACTOR_LIFT_STATUE,
  /// Four more statues flank the beacon, two on each side of it. The two east
  /// of it, ACTOR_FIRST_BEACON_STATUE + 2 and + 3, are drawn mirrored.
  ACTOR_FIRST_BEACON_STATUE,
  /// The six floating platforms, three in each gap between the towers.
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

/// The aerie's Psynergy Stone. It is an area temp flag, so it refills once the
/// party has been to another area.
enum { FLAG_PSYNERGY_STONE = FLAG_AREA_TEMP_50 };

static void MercuryLighthouseAerie_PartyArrives(void);
static void MercuryLighthouseAerie_FloatPlatforms(void);
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

/// Returns the one exit, the lift down to the ground before Mercury
/// Lighthouse, by the Fountain of Healing
/// (MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE).
const u32 *Scene_GetExits(void) { return gMercuryLighthouseAerieExits; }

/// Returns the actors, with Jenna, Kraden, Felix and Menardi placed only on the
/// first arrival (MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT).
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineState.entrance != MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT) {
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
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gMainRoutineState.next == SCENE_MERCURY_LIGHTHOUSE_AERIE) {
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
      Actor_Get(i)->y.fixed = -(toFIXED(50) + toFIXED(1) / 2);
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
    Actor_Get(ACTOR_FIRST_BEACON_STATUE + 2)->scaleX = -FIXED_ONE;
    Actor_Get(ACTOR_FIRST_BEACON_STATUE + 3)->scaleX = -FIXED_ONE;
    if (gMainRoutineState.entrance ==
        MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT) {
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        MercuryLighthouseAerie_PartyArrives();
      }
    } else if (gMainRoutineState.entrance ==
               MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE) {
      if (!GameFlag_Get(FLAG_LIFT_UP)) {
        {
          struct MapGridLayerScroll *lift =
              &((struct MapGridScrollState *)gMapGridState[0])->layers[2];

          lift->offsetY = toFIXED(1024);
        }
        MapGrid_CenterOnFollowedPoint();
        wait(1);
        MapGrid_CopyCellsTo(4, 70, 4, 74, 5, 4);
        Actor_SetPosition(ACTOR_LIFT_STATUE, 0, 0);
        if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
          MercuryLighthouseAerie_AlexAppears();
        }
      }
    } else if (gMainRoutineState.entrance ==
               MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR) {
      GameFlag_Set(FLAG_LIFT_UP);
    }
  }
  return 0;
}

/// The party arrives at the top: the aerie shakes, a white flash fills the
/// screen, and Isaac, Garet, Ivan and Mia lie on the east tower top, then get
/// up and gather round Isaac.
///
/// It plays at MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT, the entrance the
/// world map sends the party to after the waterfall below turns to stream
/// upward, but not on a return from a battle there.
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
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
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
  PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
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
    Actor_SetDestination(ACTOR_GARET, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_SetDestination(ACTOR_IVAN, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_SetDestination(ACTOR_MIA, leader->x.part[0].integer,
                         leader->z.part[0].integer);
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

/// Walking on from a platform, or from the tower edge beside one, makes Isaac
/// crouch and leap two cells to the next platform or tower top, if there is
/// ground there to land on.
///
/// The cells are the platforms in the gaps west and east of the beacon's
/// tower, cells (6, 12), (8, 12), (10, 12), (27, 12), (29, 12) and (31, 12),
/// and the ground beside them: the foot of the stairs either side of the
/// beacon's tower, (12, 12) and (25, 12), and the east tower top, (31, 10).
/// Before the leap every platform is set floating again.
static void MercuryLighthouseAerie_JumpToNextPlatform(void) {
  struct Actor *actor;
  struct FieldPosition position;
  s32 angle;
  u8 flags;

  actor = Actor_Get(ACTOR_PARTY_LEADER);
  angle = FACING_NEAREST_EIGHTH(actor->facing);
  flags = actor->motionFlags;
  SET_POSITION(&position, (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8),
               actor->y.fixed, (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8));
  Vector_AddPolarOffset(toFIXED(32), angle, &position);
  if (FieldActor_CheckMovementCollision(actor, &position) == 0) {
    GameFlag_Clear(FLAG_ON_PLATFORM);
    MercuryLighthouseAerie_FloatPlatforms();
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);
    actor->speedLimit = toFIXED(3);
    actor->acceleration = toFIXED(2);
    actor->velocityY = toFIXED(4);
    actor->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
    Actor_SetSpriteFlags(actor, 0);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, FIXEDtoINT(position.x),
                        FIXEDtoINT(position.z));
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    actor->motionFlags = flags;
  }
}

/// Landing on one of the floating platforms makes it dip under Isaac and
/// spring back up.
///
/// The platforms are cells (6, 12), (8, 12) and (10, 12) west of the beacon's
/// tower and (27, 12), (29, 12) and (31, 12) east of it. The one under him
/// stops floating and holds still while he stands on it (FLAG_ON_PLATFORM).
static void MercuryLighthouseAerie_LandOnPlatform(void) {
  struct Actor *leader;
  struct Actor *platform;
  s32 platformId;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Start();
  platformId = Actor_FindNearestFloatingPlatform();
  gMercuryLighthouseAerie_LandedPlatform = platformId;
  if (platformId != 0) {
    GameFlag_Set(FLAG_ON_PLATFORM);
    platform = Actor_Get(gMercuryLighthouseAerie_LandedPlatform);
    platform->motionFlags = 0;
    leader->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
    platform->y.fixed -= toFIXED(3);
    leader->y.fixed -= toFIXED(3);
    leader->groundHeight -= toFIXED(3);
    wait(2);
    platform->y.fixed -= toFIXED(2);
    leader->y.fixed -= toFIXED(2);
    leader->groundHeight -= toFIXED(2);
    wait(10);
    platform->y.fixed += toFIXED(2);
    leader->y.fixed += toFIXED(2);
    leader->groundHeight += toFIXED(2);
    wait(4);
    platform->y.fixed += toFIXED(2);
    leader->y.fixed += toFIXED(2);
    leader->groundHeight += toFIXED(2);
    wait(4);
    platform->y.fixed += toFIXED(1);
    leader->y.fixed += toFIXED(1);
    leader->groundHeight += toFIXED(1);
  }
  Event_End();
}

/// Nothing visible happens when Isaac steps off the platforms onto the foot of
/// the stairs either side of the beacon's tower, cells (12, 12) and (25, 12),
/// or onto the east tower top at cell (31, 10). The game sets every platform
/// floating again, the one he left included, which stopped under him when he
/// landed.
///
/// The leap from one platform to the next calls it too.
static void MercuryLighthouseAerie_FloatPlatforms(void) {
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_Get(ACTOR_FIRST_PLATFORM)->motionFlags = ACTOR_FLOATS;
  Actor_Get(ACTOR_FIRST_PLATFORM + 1)->motionFlags = ACTOR_FLOATS;
  Actor_Get(ACTOR_FIRST_PLATFORM + 2)->motionFlags = ACTOR_FLOATS;
  Actor_Get(ACTOR_FIRST_LIFT_PLATFORM)->motionFlags = ACTOR_FLOATS;
  Actor_Get(ACTOR_SECOND_LIFT_PLATFORM)->motionFlags = ACTOR_FLOATS;
  Actor_Get(ACTOR_FIRST_PLATFORM + 5)->motionFlags = ACTOR_FLOATS;
}

/// Climbing the wide stairs up to the beacon, at cells (20, 14) and (21, 14),
/// the party finds it already lit, Menardi rides the lift down with Jenna,
/// Kraden and Felix, and Saturos fights the party alone.
///
/// Garet sees `It's too late! The lighthouse has already been lit!` The
/// party gathers on the stairs and talks it over with Mia before Jenna calls
/// out from the west tower top. It plays until Saturos is beaten
/// (FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED).
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
    Actor_SetPositionFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
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
    gFieldState->message += 2;
  } else {
    gFieldState->message += 2;
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
  Sound_Play(MUS_CHANCE_MEETING_WITH_SWORN_ENEMIES);
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
  Actor_Get(ACTOR_MENARDI)->velocityY = toFIXED(4);
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
  // "Isaac! Garet! Don't die!"
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  // "Jenna... Kraden!"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_SetSpeed(ACTOR_GARET, 1.5);
  Actor_WalkToAndWait(ACTOR_GARET, 312, 216);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 10);
  // "If you want to save them, you'll have to beat me, first!"
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_WalkToAndWait(ACTOR_SATUROS, 312, 184);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHEAST, 20);
  // "No! Get back!"
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
  // "What's the matter!"
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  // "Saturos is extremely powerful. I could sense it when I faced him."
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  // "You have to be very careful when fighting him... We have to match..."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "Match me? Pah! You think you can match the great Saturos?"
  Event_OpenMessage(ACTOR_SATUROS, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    // "You overestimate your chances."
    Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    gFieldState->message++;
  } else {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    gFieldState->message++;
    // "And yet you still want to fight? Ship of fools..."
    Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  }
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_Wait(20);
  // "Fine then... I'll just teach you all a lesson!"
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
  // "What happened? Saturos is moving strangely."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  // "What's this!? The light of Mercury... It's weakening my Psynergy!"
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_Wait(20);
  // "I must finish this quickly... Come on, fools!"
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_WalkToAndWait(ACTOR_SATUROS, 336, 216);
  Event_SetAfterBattleDestination(
      SCENE_MERCURY_LIGHTHOUSE_AERIE,
      MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  // Saturos: ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, slot 2.
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY,
                                        2);
  Event_End();
}

/// Back from the battle, Saturos is beaten, but Alex appears, stalls until
/// Saturos recovers and leaves with him on the lift.
///
/// It plays at MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE, unless the party
/// returns there after another battle. If the lift is down it is raised for
/// them first.
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

  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
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
  Sound_Play(MUS_THE_SLEEPING_TREASURE_OF_ALTMILLER_CAVE);
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
    gFieldState->message++;
  } else {
    gFieldState->message++;
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
    gFieldState->message++;
  } else {
    Event_Wait(30);

    gFieldState->message++;
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
  // "He warped again!"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  // "Again? He's done that before?"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 20);
  // "Yeah, in Sol Sanctum..."
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
  // "That Psynergy...the warp. Does that mean Mia can use it?"
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "No, it's impossible. Even Alex didn't have this kind of power before."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 20);
  // "Ah, well... I can't stay the same Alex you knew forever..."
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  FieldCamera_SetSpeed(3);
  {
    struct MapGridLayerScroll *lift;
    FieldCamera_MoveTo(152, CAMERA_KEEP, 216, 1);
    lift = &((struct MapGridScrollState *)gMapGridState[0])->layers[2];
    lift->offsetY = toFIXED(896);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
    Actor_Get(ACTOR_LIFT_STATUE)->motionFlags = 0;
    Actor_SetPosition(ACTOR_LIFT_STATUE, 104, 264);
    Actor_Get(ACTOR_LIFT_STATUE)->y.fixed = -toFIXED(32);
    Actor_Get(ACTOR_LIFT_STATUE)->targetY = -toFIXED(32);
    MapGrid_CopyCellsTo(29, 74, 4, 74, 5, 4);
    Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 0);
    Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 0);
    MercuryLighthouseAerie_RaiseLift();
    Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 1);
    Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_ALEX, FACING_WEST, 20);
    // "Well, it seems that my ride has returned while we were speaking."
    Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
    FieldCamera_MoveTo(280, CAMERA_KEEP, 216, 1);
    FieldCamera_WaitForArrival();
    Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 20);
    Actor_SetAnimation(ACTOR_ALEX, 5);
    // "And so, I bid you adieu."
    Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
    // "Isaac, Saturos still has the Elemental Stars!"
    Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_Wait(20);
    // "That's right! We can't let them leave now!"
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    // "Oh? And you want to take the Elemental Stars from Saturos?"
    Event_OpenMessage(ACTOR_ALEX, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_Wait(20);
      Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
      Event_Wait(20);
      // "Ha! Too bad! Menardi has them. You'll have to ask her!"
      Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
      gFieldState->message++;
    } else {
      gFieldState->message++;
      Event_Wait(20);
      // "Ha! You're right... Menardi has the Elemental Stars!"
      Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    }
  }
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  Event_Wait(20);
  // "That reminds me..."
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "Do you still have the Mars Star?"
  Event_OpenMessage(ACTOR_ALEX, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
    Event_Wait(20);
    // "I appreciate your honesty."
    Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
    gFieldState->message++;
  } else {
    gFieldState->message++;
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 1);
    Event_Wait(20);
    // "Are you telling the truth? Well, I shall believe you for now."
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
  // "I suppose you'll be willing to pursue us for the Elemental Stars?"
  Event_OpenMessage(ACTOR_ALEX, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
    Event_Wait(20);
    // "I look forward to it. I shall see you soon, then."
    Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
    gFieldState->message++;
  } else {
    gFieldState->message++;
    Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    // "That's too bad... I shall look forward to seeing you again."
    Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  }
  // "Good-bye."
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_EAST, 20);
  // "I look forward to our next battle."
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
  // "They're gone..."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 22);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(10);
  // "Shoot... We couldn't even save Jenna..."
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  // "I don't deserve to be a healer..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
  // "We couldn't stop them from lighting the beacon."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
  Actor_SetAnimation(ACTOR_MIA, ANIM_MIA_HEAD_BOWED);
  Actor_Get(ACTOR_MIA)->scaleX = -FIXED_ONE;
  Event_Wait(20);
  // "I've failed my clan, failed in my duty..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "That's not true! We can still beat them, right, Isaac?"
  Event_OpenMessage(ACTOR_IVAN, 0);
  Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
  Actor_Get(ACTOR_MIA)->scaleX = FIXED_ONE;
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    // "That's right, Isaac! All we have to do is stop them from lighting..."
    Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
    gFieldState->message++;
  } else {
    gFieldState->message++;
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    // "You're not helping here! We're fine as long as the lighthouses..."
    Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  }
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  // "The beacon of Mercury Lighthouse has been lit, but we'll stop them..."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 30);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(10);
  // "That's right. We can save Jenna at the next lighthouse."
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  // "Yes. We won't let Alex get away with this..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  // "Let's go after them now!"
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
  // "What's the matter? We should hurry!"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  // "We? Mia, are you coming along with us?"
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
  // "But the people of Imil need you, Mia."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "It's okay."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
  // "'The fountain shall be filled as long as light fills the..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "I have mixed feelings about leaving, but Imil will be fine as long..."
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
  // "We should hurry if we're going to follow them."
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
        &((struct MapGridScrollState *)gMapGridState[0])->layers[2];

    lift->offsetY = toFIXED(1024);
  }
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  MapGrid_CopyCellsTo(4, 70, 4, 74, 5, 4);
  GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED);
  GameFlag_Set(FLAG_SATUROS_BEATEN_AT_MERCURY);
  Event_End();
}

/// Walking down the stairs on the west side of the beacon's tower, at cell
/// (12, 9), brings the lift back up while it is down.
///
/// The camera pans west to the empty space, the lift rises into it with its
/// statue and the two platforms it carries, and the camera comes back to
/// Isaac. It stays up (FLAG_LIFT_UP).
static void MercuryLighthouseAerie_CallLift(void) {
  struct MapGridLayerScroll *lift;

  lift = &((struct MapGridScrollState *)gMapGridState[0])->layers[2];
  Event_Start();
  lift->offsetY = toFIXED(896);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Actor_Get(ACTOR_LIFT_STATUE)->motionFlags = 0;
  Actor_SetPosition(ACTOR_LIFT_STATUE, 104, 264);
  Actor_Get(ACTOR_LIFT_STATUE)->y.fixed = -toFIXED(32);
  Actor_Get(ACTOR_LIFT_STATUE)->targetY = -toFIXED(32);
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

/// Stepping onto the lift from the westmost platform, at cell (6, 10), rides
/// it down to the ground before Mercury Lighthouse, by the Fountain of
/// Healing.
///
/// Isaac walks to the middle of the lift and turns to face south, and the lift
/// sinks with him before the party leaves by
/// MERCURY_LIGHTHOUSE_AERIE_TO_MERCURY_LIGHTHOUSE_EXTERIOR.
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

/// Saturos and Alex vanish in a column of sparkles.
static void MercuryLighthouseAerie_SaturosAndAlexWarpOut(void) {
  struct EffectOptions options;
  struct Actor *left;
  struct Actor *right;
  u32 frame;
  FIXED x;
  FIXED r;

  left = Actor_Get(ACTOR_SATUROS);
  right = Actor_Get(ACTOR_ALEX);
  Sound_Play(SE_WARP);
  Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_BLINK);
  Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_BLINK);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SATUROS), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), 0);
  options.priority = 1;
  options.palette = SPRITE_COLOR_BRG;
  options.type = SPRITE_PARTICLE;
  options.startScaleX = 2 * FIXED_ONE / 5;
  options.startScaleY = 3 * FIXED_ONE;
  for (frame = 0; frame < WARP_FRAMES; frame++) {
    Event_Wait(1);
    if (frame & 1) {
      r = toFIXED(Random_Below(24));
      x = left->x.fixed;
      x += r;
      x -= toFIXED(12);
      FieldEffect_Spawn(x,
                        left->y.fixed + toFIXED(Random_Below(32)) - toFIXED(16),
                        left->z.fixed, 0, toFIXED(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        &options);
    } else {
      r = toFIXED(Random_Below(24));
      x = right->x.fixed;
      x += r;
      x -= toFIXED(12);
      FieldEffect_Spawn(
          x, right->y.fixed + toFIXED(Random_Below(32)) - toFIXED(16),
          right->z.fixed, 0, toFIXED(4), 0,
          EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
              EFFECT_USE_PALETTE,
          &options);
    }
  }
  Actor_SetPosition(ACTOR_SATUROS, 0, 0);
  Actor_SetPosition(ACTOR_ALEX, 0, 0);
}

/// Saturos and Alex appear in a column of sparkles where they have been
/// placed.
static void MercuryLighthouseAerie_SaturosAndAlexWarpIn(void) {
  struct EffectOptions options;
  struct Actor *left;
  struct Actor *right;
  u32 frame;
  FIXED x;
  FIXED r;

  left = Actor_Get(ACTOR_SATUROS);
  right = Actor_Get(ACTOR_ALEX);
  Sound_Play(SE_WARP);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SATUROS), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), 0);
  options.priority = 1;
  options.palette = SPRITE_COLOR_BRG;
  options.type = SPRITE_PARTICLE;
  options.startScaleX = 2 * FIXED_ONE / 5;
  options.startScaleY = 3 * FIXED_ONE;
  for (frame = 0; frame < WARP_FRAMES; frame++) {
    Event_Wait(1);
    if (frame & 1) {
      r = toFIXED(Random_Below(24));
      x = left->x.fixed;
      x += r;
      x -= toFIXED(12);
      FieldEffect_Spawn(x,
                        left->y.fixed + toFIXED(Random_Below(32)) + toFIXED(32),
                        left->z.fixed, 0, -toFIXED(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        &options);
    } else {
      r = toFIXED(Random_Below(24));
      x = right->x.fixed;
      x += r;
      x -= toFIXED(12);
      FieldEffect_Spawn(
          x, right->y.fixed + toFIXED(Random_Below(32)) + toFIXED(32),
          right->z.fixed, 0, -toFIXED(4), 0,
          EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
              EFFECT_USE_PALETTE,
          &options);
    }
    if (frame == WARP_IN_BLINK_FRAME) {
      Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_BLINK);
      Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_BLINK);
    }
  }
  Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SATUROS), SPRITE_FLAG_SHADOW);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), SPRITE_FLAG_SHADOW);
}

/// Writes to out the ids of the actors standing on the lift, columns 4 to 8 and
/// three rows from its top, while its map layer is scrolled y pixels down.
static void MercuryLighthouseAerie_FindActorsOnLift(s32 *out, s32 y) {
  struct FieldState *state;
  u32 slot;

  state = gFieldState;
  y = 64 - FIELD_CELL(y);
  for (slot = 0; slot < sizeof(state->actors) / sizeof(state->actors[0]);
       slot++) {
    struct Actor *actor = state->actors[slot];

    if (actor != NULL) {
      s32 cell = FIELD_CELL(actor->x.fixed);
      s32 row = FIELD_CELL(actor->z.fixed);

      if (cell >= 4 && cell <= 8 && y + 8 <= row && row < y + 11)
        *out++ = slot;
    }
  }
}

/// How long the lift takes to sink, speeding up every fourth frame.
#define LIFT_DOWN_FRAMES 228

/// How long the lift takes to rise, and the last frames of it in which it
/// slows every fourth frame.
#define LIFT_UP_FRAMES 86
#define LIFT_SLOWING_FRAMES 10

/// The lift sinks with everything standing on it, speeding up.
static void MercuryLighthouseAerie_LowerLift(void) {
  s32 ids[5];
  struct MapGridLayerScroll *lift;
  u32 i;
  u32 j;
  u32 n;
  FIXED speed;

  lift = &((struct MapGridScrollState *)gMapGridState[0])->layers[2];
  speed = toFIXED(1) / 10;
  n = 0;
  for (i = 0; i <= 4; i++)
    ids[i] = ACTOR_ID_COUNT;
  MercuryLighthouseAerie_FindActorsOnLift(ids, lift->offsetY);
  for (i = 0; i <= 4; i++) {
    if (ids[i] == ACTOR_ID_COUNT)
      break;
    Actor_Get(ids[i])->motionFlags = 0;
    n++;
  }
  Sound_Play(SE_LIFT);
  for (i = 0; i < LIFT_DOWN_FRAMES; i++) {
    lift->offsetY -= speed;
    for (j = 0; j < n; j++) {
      Actor_Get(ids[j])->z.fixed += speed;
      Actor_Get(ids[j])->targetZ = Actor_Get(ids[j])->z.fixed;
    }
    if ((i & 3) == 3)
      speed += toFIXED(1) / 10;
    if (speed >= toFIXED(3) / 2)
      speed = toFIXED(3) / 2;
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
  FIXED speed;

  lift = &((struct MapGridScrollState *)gMapGridState[0])->layers[2];
  speed = toFIXED(3) / 2;
  n = 0;
  for (i = 0; i <= 4; i++)
    ids[i] = ACTOR_ID_COUNT;
  MercuryLighthouseAerie_FindActorsOnLift(ids, lift->offsetY);
  for (i = 0; i <= 4; i++) {
    if (ids[i] == ACTOR_ID_COUNT)
      break;
    Actor_Get(ids[i])->motionFlags = 0;
    n++;
  }
  Sound_Play(SE_LIFT);
  for (i = 0; i < LIFT_UP_FRAMES; i++) {
    lift->offsetY += speed;
    for (j = 0; j < n; j++) {
      Actor_Get(ids[j])->z.fixed -= speed;
      Actor_Get(ids[j])->targetZ = Actor_Get(ids[j])->z.fixed;
    }
    if ((i & 3) == 3 && i >= LIFT_UP_FRAMES - LIFT_SLOWING_FRAMES)
      speed -= toFIXED(1) / 5;
    if (speed < toFIXED(1) / 20)
      speed = toFIXED(1) / 20;
    wait(1);
  }
  lift->offsetY = toFIXED(1024);
  MapGrid_CenterOnFollowedPoint();
  wait(2);
}

/// Unused: Alex casts Ply on the lift's statue.
///
/// Nothing calls it. In the game, Alex and Saturos vanish, appear on the lift
/// and ride it down without it (MercuryLighthouseAerie_AlexAppears).
UNUSED static void MercuryLighthouseAerie_UnusedAlexCastsPly(void) {
  // XXX (no evidence): what this was for; no event plays it.
  FieldPsynergy_LoadResources(ABILITY_PLY, 1);
  FieldPsynergy_SetupActorPair(ACTOR_ALEX, ACTOR_LIFT_STATUE);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  FieldPsynergy_StopCasterUpdate();
  FieldPsynergy_End();
}

/// A beacon spark circles the beacon this many pixels out, and up to
/// SPARK_ORBIT_BULGE more the nearer it rose from the middle of the beacon's
/// height, so the sparks outline a ball.
#define SPARK_ORBIT_RADIUS 28
#define SPARK_ORBIT_BULGE 24

/// The beacon is in sight, and drawn, while the view's top is at most this
/// far down the map.
#define BEACON_SHOWN_VIEW_TOP 129

/// A beacon spark's update, which swings it round the beacon it watches.
static void MercuryLighthouseAerie_UpdateBeaconSpark(struct Actor *spark) {
  struct Actor *beacon;
  u16 angle;

  beacon = spark->watchedActor;
  angle = spark->timer;
  spark->x.fixed = beacon->x.fixed +
                   Trig_Cos(angle) * (spark->speedLimit + SPARK_ORBIT_RADIUS);
  spark->z.fixed = (Trig_Sin(angle) << 4) + toFIXED(144);
  spark->targetX = spark->x.fixed;
  spark->targetZ = spark->z.fixed;
  spark->timer -= 512;
}

/// Flickers the beacon and sheds its sparks, every frame.
static void MercuryLighthouseAerie_FlickerBeacon(void) {
  struct Actor *beacon;
  struct Actor *spark;
  struct Sprite *sprite;
  struct MapGridScrollState *grid;
  s32 height;
  FIXED scale;
  s32 cycleFrame;
  u32 angle;

  beacon = Actor_Get(ACTOR_BEACON);
  grid = gMapGridState[0];
  height = toFIXED(Random_Below(48));
  if (((struct FieldCoordinate *)&grid->view[1])->part[0].integer <=
      BEACON_SHOWN_VIEW_TOP) {
    if (gFrameCount & 1) {
      Import_ActorSetPosition(ACTOR_BEACON, toFIXED(304), toFIXED(144));
      scale = FIXED_ONE;
      Actor_Get(ACTOR_BEACON)->scaleX = scale;
    } else {
      Import_ActorSetPosition(ACTOR_BEACON, toFIXED(304), toFIXED(151));
      scale = 13 * FIXED_ONE / 10;
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
  spark =
      FieldActor_Create(SPRITE_PARTICLE, beacon->x.fixed + toFIXED(8),
                        beacon->y.fixed + height + toFIXED(8), beacon->z.fixed);
  height = height / toFIXED(6);
  height <<= 16;
  if (spark == NULL) {
    return;
  }
  sprite = spark->sprite;
  Actor_StartScript(spark, gBeacon_SparkScript);
  Actor_SetSpriteColor(spark, SPRITE_COLOR_BGR);
  spark->motionFlags = cycleFrame;
  angle = Random_Next() & 0x0ffff000;
  spark->timer = angle;
  spark->userValue = cycleFrame;
  spark->watchedActor = beacon;
  spark->update = MercuryLighthouseAerie_UpdateBeaconSpark;
  spark->speedLimit =
      FIXEDtoINT(Trig_Sin((height & 0xfffff) >> 4) * SPARK_ORBIT_BULGE);
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
    {TRIGGER_STEP, CELL_BEHAVIOR_BEACON_STAIRS,
     FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED,
     EVENT(MercuryLighthouseAerie_ConfrontSaturos)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LIFT_LANDING, FLAG_LIFT_UP,
     EVENT(MercuryLighthouseAerie_CallLift)},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_PLATFORM_EDGE, CONDITION_ALWAYS,
     EVENT(MercuryLighthouseAerie_JumpToNextPlatform)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PLATFORM_EDGE, CONDITION_ALWAYS,
     EVENT(MercuryLighthouseAerie_FloatPlatforms)},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_PLATFORM, CONDITION_ALWAYS,
     EVENT(MercuryLighthouseAerie_JumpToNextPlatform)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PLATFORM, FLAG_ON_PLATFORM,
     EVENT(MercuryLighthouseAerie_LandOnPlatform)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_PSYNERGY_STONE,
     FLAG_PSYNERGY_STONE, FIND_PSYNERGY_STONE},
    {TRIGGER_STEP, CELL_BEHAVIOR_LIFT, CONDITION_ALWAYS,
     EVENT(MercuryLighthouseAerie_RideLiftDown)},
    {TRIGGER_TABLE_END},
};

/// The floating platform the leader has landed on, 0 when none.
s32 gMercuryLighthouseAerie_LandedPlatform = 0;

#include "shared/beacon_spark_script.c"
