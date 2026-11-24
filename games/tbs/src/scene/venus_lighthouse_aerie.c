/// @file
/// Lighthouse Aerie (SceneId_VenusLighthouseAerie), the top of Venus Lighthouse
/// and the only scene of this overlay.
///
/// What happens here is one chain of events cut by two battles and two trips
/// to the World Map. Each part starts in Scene_Initialize from
/// the entrance the part before sent the party to. Every arrival blocks
/// Retreat and sets FLAG_VENUS_LIGHTHOUSE_LIT.
///
/// The parts, in the order the entrances chain them:
/// - The arrival (VENUS_LIGHTHOUSE_AERIE_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR,
///   VenusLighthouseAerie_OverhearSaturosAndMenardi). A floating platform
///   carries Isaac up beside the tower in the north-east onto its top, and
///   the party overhears Saturos, Menardi, Felix and Sheba arguing at the
///   beacon, with one yes-or-no question that changes only the reply.
/// - Two rows of three floating platforms bob in the air, one between the
///   tower and the middle of the aerie and one west of the middle
///   (CELL_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_PLATFORM,
///   CELL_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_FLOOR_BY_PLATFORMS). Isaac jumps
///   from one to the next, and each stops bobbing and dips under him as he
///   lands. FLAG_VENUS_AERIE_ON_PLATFORM is set while he stands on one.
/// - The confrontation, when Isaac walks up to the beacon
///   (CELL_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_BEFORE_BEACON,
///   VenusLighthouseAerie_ConfrontSaturosAndMenardi). The Shaman's Rod is
///   handed over for Sheba's sake, remembered in FLAG_SHAMANS_ROD_CARRIED_BY
///   plus the party member who carried it. The first battle follows
///   (ENCOUNTER_ZONE_SCRIPTED, slot 1, FLAG_AERIE_FIRST_BATTLE_BEGUN).
/// - After it (VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE,
///   VenusLighthouseAerie_CombineSaturosAndMenardi). Saturos throws the Venus
///   Star into the beacon (ACTOR_VENUS_STAR_SHADOW), Felix and Sheba leave,
///   and Saturos and Menardi fuse. Party_RestorePpOfLitElements refills the PP
///   of Isaac and Garet, inside the lighthouse of their element group, and the
///   second battle follows (slot 0, FLAG_AERIE_SECOND_BATTLE_BEGUN).
/// - After that (VENUS_LIGHTHOUSE_AERIE_AFTER_SECOND_BATTLE,
///   VenusLighthouseAerie_FinishSaturosAndMenardiBattle). Saturos and Menardi
///   fall into the lighthouse, and the beacon erupts. Its beam
///   (ACTOR_BEAM_BOTTOM to ACTOR_BEAM_TOP) rises, the aerie splits and its
///   statues move apart. The light lifts the party away to the World Map's
///   lighthouse transition (WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT). The beam's
///   five segments are sprites stacked over the beacon, with particles rising
///   round them. The split is drawn by copying the map's cells
///   (MapGrid_CopyCellsTo in VenusLighthouseAerie_EruptBeacon).
/// - A look back (VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT,
///   VenusLighthouseAerie_ShowShebaInDanger). Sheba hangs from the broken
///   aerie, and VENUS_LIGHTHOUSE_AERIE_TO_WORLD_MAP returns to the World Map
///   (WORLD_MAP_BEFORE_SHEBAS_FALL) with its music kept.
/// - Sheba's fall (VENUS_LIGHTHOUSE_AERIE_FROM_WORLD_MAP,
///   VenusLighthouseAerie_StageShebasFall). Sheba and Felix fall, and the
///   party is taken to Lalivero (VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_C)
///   with FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED set.
///
/// A lost battle returns to VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT, which gives
/// the Shaman's Rod back to whoever carried it and leaves for Lalivero Sanctum
/// (VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_B).

#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor.h"
#include "field/create_configured_actor.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/field_psynergy_state.h"
#include "field/map_grid_scroll.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/actor_nearest.h"
#include "scene/event_psynergy.h"
#include "scene/field_effect_spawn.h"
#include "scene/import.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/heap_slots.h"
#include "system/io_reg.h"
#include "system/iwram_call.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "system/vram_cache.h"
#include "window/screens.h"
#include "window/window.h"
#include "text/msg_ids.h"

extern s32 gVenusLighthouseAerie_JoinLeaderScript[];
extern s32 gVenusLighthouseAerie_StepBackWestScript[];
extern s32 gVenusLighthouseAerie_StepBackEastScript[];
extern s32 gVenusLighthouseAerie_BreatheHeavilyScript[];
extern s32 gVenusLighthouseAerie_BeamOpenScript[];
extern s32 gVenusLighthouseAerie_BeamNarrowScript[];
extern s32 gVenusLighthouseAerie_BeamRiseScript[];
extern s32 gVenusLighthouseAerie_BeamCloseScript[];
extern s32 gVenusLighthouseAerie_BeamEndScript[];
extern s32 gVenusLighthouseAerie_OrbitParticleScript[];
extern s32 gVenusLighthouseAerie_RiseParticleScript[];
extern s32 gVenusLighthouseAerie_WalkOutScript[];
extern s32 gVenusLighthouseAerie_OrbPulseScript[];
extern s32 gVenusLighthouseAerie_ShebaFallScript[];
extern s32 gVenusLighthouseAerie_FelixDiveScript[];
extern s32 gVenusLighthouseAerie_PartyExitScript[];
extern s32 gVenusLighthouseAerie_LookAboutScript[];
extern struct SceneEntrance gVenusLighthouseAerieEntrances[];
extern u32 gVenusLighthouseAerieExits[];
extern struct ActorPlacement gVenusLighthouseAerieActorPlacements[];
extern s32 gVenusLighthouseAerie_LandedPlatform;
extern struct Trigger gVenusLighthouseAerieTriggers[];
extern s32 gBeacon_SparkScript[];

/// The aerie's actors, in the order of its actor placements (its entrances and
/// exits are in scene_exits.h);
/// Garet, Ivan, Mia and Sheba keep their character numbers.
enum {
  /// The four statues around the beacon (SPRITE_STATUE_WITH_WREATH), the east
  /// two drawn mirrored. When the beacon erupts and splits the aerie, the west
  /// two move west with their half and the east two east.
  ACTOR_BEACON_STATUE_SOUTHWEST = 8,
  ACTOR_BEACON_STATUE_NORTHWEST = 9,
  ACTOR_BEACON_STATUE_NORTHEAST = 10,
  ACTOR_BEACON_STATUE_SOUTHEAST = 11,
  /// Six round platforms (SPRITE_FLOATING_PLATFORM) the leader jumps across,
  /// each dipping under him as he lands, and the one that carries him up to
  /// the aerie.
  ACTOR_FIRST_PLATFORM = 12,
  ACTOR_LAST_PLATFORM = 17,
  ACTOR_RISING_PLATFORM = 18,
  ACTOR_MENARDI = 19,
  ACTOR_SATUROS = 20,
  ACTOR_FELIX = 21,
  /// A blank actor (SPRITE_BLANK) that runs across the floor under the Venus
  /// Star as Saturos throws it into the beacon, so that only its shadow shows,
  /// the star's shadow.
  ACTOR_VENUS_STAR_SHADOW = 22,
  /// The orb the rising particles circle as the party is lifted away:
  /// SPRITE_BLUE_ORB drawn in SPRITE_COLOR_GBR, which turns its blue to green,
  /// and later solid white (SPRITE_COLOR_WHITE).
  ACTOR_BEACON_ORB = 23,
  /// The beacon's beam, in five segments (SPRITE_PIPE_NORTH_SOUTH) from the
  /// bottom up; particles circle the lowest.
  ACTOR_BEAM_BOTTOM = 24,
  ACTOR_BEAM_LOWER = 25,
  ACTOR_BEAM_MIDDLE = 26,
  ACTOR_BEAM_UPPER = 27,
  ACTOR_BEAM_TOP = 28
};

/// An area flag, plus the party member (0 to 3) who carried the Shaman's Rod
/// when it was handed over before the aerie's first battle. A defeat gives it
/// back.
#define FLAG_SHAMANS_ROD_CARRIED_BY FLAG_AREA_TEMP_45

/// Area flags that choose the aerie's field music after each battle.
///
/// They work by the scene's field music rules (gScene_MusicRules,
/// field/scene_rules.s). The music is MUS_SILENCE before the first battle,
/// MUS_BABIS_THEME once the first has begun and MUS_ELEMENTAL_STARS once the
/// second has.
///
/// VenusLighthouseFirstFloor and VenusLighthouseFourthFloor, in the same
/// area, keep their Psynergy Stones in the same two flags, which no one goes
/// back down to see.
enum {
  FLAG_AERIE_FIRST_BATTLE_BEGUN = FLAG_AREA_TEMP_50,
  FLAG_AERIE_SECOND_BATTLE_BEGUN = FLAG_AREA_TEMP_51
};

static void VenusLighthouseAerie_FloatPlatforms(void);
static void VenusLighthouseAerie_OverhearSaturosAndMenardi(void);
static void VenusLighthouseAerie_ShowPartyMember(s32 actorId, s32 shown);
static void VenusLighthouseAerie_FlashActorPalette(struct Actor *actor);
static void Scene_SpawnPsynergyRings(struct Actor *parent);
static void VenusLighthouseAerie_UpdateShebaPalette(void);
static void VenusLighthouseAerie_CombineSaturosAndMenardi(void);
static void VenusLighthouseAerie_FlashScreen(void);
static void VenusLighthouseAerie_SpawnUpperOrbitParticles(void);
static void VenusLighthouseAerie_UpdateFusion(void);
static void VenusLighthouseAerie_FinishSaturosAndMenardiBattle(void);
static void VenusLighthouseAerie_ShowShebaInDanger(void);
static void VenusLighthouseAerie_StageShebasFall(void);
static void VenusLighthouseAerie_ShowSaturosAndMenardiFall(void);
static void VenusLighthouseAerie_DiscussSaturosAndMenardiDefeat(void);
static void VenusLighthouseAerie_EruptBeacon(void);
static void VenusLighthouseAerie_RestorePartyOnBrokenAerie(void);
static void VenusLighthouseAerie_TryToPreventFall(void);
static void VenusLighthouseAerie_PlayShebasFall(void);
static void VenusLighthouseAerie_StopActorMotion(struct Actor *actor);
static void VenusLighthouseAerie_UpdateBeaconRise(void);
static void VenusLighthouseAerie_AdvanceActorRise(struct Actor *actor);
static void VenusLighthouseAerie_SpawnSideOrbitParticles(void);
static void VenusLighthouseAerie_UpdateOrbitingEffect(struct Actor *actor);

/// The cell behaviors the aerie's step triggers answer.
enum VenusLighthouseAerieCellBehavior {
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_BEFORE_BEACON =
      5, ///< before the beacon, where Isaac confronts Saturos and Menardi
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_PLATFORM =
      10, ///< a floating platform's cells
  /// The floor beside the platforms, from which the leader jumps
  /// onto them.
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_FLOOR_BY_PLATFORMS = 11
};

/// Animations the aerie plays by number beyond enum ActorAnimation.
///
/// They come from each sprite's animation table
/// (graphics/sprite/<name>.tsv). The party's are reached through their
/// animation lists.
enum {
  /// Menardi standing, her hair blown back (menardi.tsv).
  MENARDI_ANIMATION_STAND_WINDSWEPT = 5,
  MENARDI_ANIMATION_LIE = 7, ///< lying on her side
  /// Menardi falling into the lighthouse, as SATUROS_ANIMATION_TIP to _SINK.
  MENARDI_ANIMATION_TIP = 8,
  MENARDI_ANIMATION_SLUMP = 9,
  MENARDI_ANIMATION_SINK = 10,
  /// Garet on his knees, Ivan and Mia lying down, as the party lies on the
  /// broken aerie (garet.tsv to mia.tsv).
  ANIM_PARTY_KNOCKED_DOWN = 18,
  ANIM_LEADER_KNOCKED_DOWN = 19 ///< Isaac lying on his back (isaac.tsv)
};

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/// The heavy breathing of someone worn out: Saturos and Menardi lying hurt
/// after the first battle, and Isaac as he gets up after Sheba's fall. The
/// sprite stretches taller and narrower and settles back over six frames,
/// then keeps still for 80 to 159 frames before the next breath.
static s32 VenusLighthouseAerie_PulseActorScale(struct Actor *actor) {
  switch (actor->timer) {
  case 6:
    actor->scaleX -= FIXED_ONE / 8;
    actor->scaleY += FIXED_ONE / 16;
    break;
  case 4:
    actor->scaleX += FIXED_ONE / 16;
    actor->scaleY -= FIXED_ONE / 32;
    break;
  case 2:
    actor->scaleX += FIXED_ONE / 32;
    actor->scaleY -= FIXED_ONE / 64;
    break;
  case 0:
    actor->scaleX += FIXED_ONE / 32;
    actor->scaleY -= FIXED_ONE / 64;
    actor->timer = Math_RemainderUnsigned(Random_Next(), 80) + 80;
    break;
  }
  {
    s32 frames = (u16)actor->timer;
    frames--;
    actor->timer = frames;
  }
  return 1;
}

/// A drifting particle loses a PARTICLE_DRAG_X-th of its x speed and a
/// PARTICLE_DRAG_Z-th of its z speed each frame.
enum { PARTICLE_DRAG_X = 22, PARTICLE_DRAG_Z = 20 };

/// A particle of the bursts that fly out of Saturos and Menardi as they draw
/// on the beacon: it slows as it drifts, turning and shrinking.
static void VenusLighthouseAerie_UpdateDriftingParticle(struct Actor *actor) {
  struct FieldEffect *effect = (struct FieldEffect *)actor;
  s32 velocityX = effect->velocity.x;
  s32 velocityZ;

  effect->x += velocityX;
  effect->y += effect->velocity.y;
  velocityZ = effect->velocity.z;
  effect->z += velocityZ;
  effect->velocity.x = velocityX - velocityX / PARTICLE_DRAG_X;
  effect->velocity.z = velocityZ - velocityZ / PARTICLE_DRAG_Z;
  effect->scaleX += effect->scaleRateX;
  effect->scaleY += effect->scaleRateY;
  effect->sprite->rotation += effect->spin;
}

/// Felix's sprite turns as he dives off the broken aerie after Sheba, until
/// its angle is out of the negative half-turn.
static void VenusLighthouseAerie_StepSpriteRotation(struct Actor *actor) {
  struct Sprite *sprite = actor->sprite;
  u16 rotation = sprite->rotation;

  if ((s16)(rotation - 1) < 0) {
    {
      s32 nextRotation = rotation - TRIG_TURN * 5 / 128;
      sprite->rotation = nextRotation;
    }
  }
}

#include "shared/actor_heading.c"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gVenusLighthouseAerieEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gVenusLighthouseAerieExits; }

/// Returns the beacon's statues, the platforms, Saturos and Menardi, Felix and
/// Sheba, the party and the beacon effects.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gVenusLighthouseAerieActorPlacements;
}

#include "shared/actor_distance.c"
#include "shared/actor_nearest.c"

/// Saturos and Menardi slide together as they fuse.
#define FLAG_VENUS_AERIE_FUSION_SLIDING FLAG_SCENE_TEMP_34
/// Saturos and Menardi grow as they fuse.
#define FLAG_VENUS_AERIE_FUSION_GROWING FLAG_SCENE_TEMP_35
/// The beacon has erupted, and its particles rise higher.
#define FLAG_VENUS_AERIE_BEACON_ERUPTED FLAG_SCENE_TEMP_36
/// The beacon's light has lifted the party and flashed white.
#define FLAG_VENUS_AERIE_PARTY_LIFTED FLAG_SCENE_TEMP_37
/// The leader stands on one of the floating platforms.
#define FLAG_VENUS_AERIE_ON_PLATFORM FLAG_SCENE_TEMP_50

/// Walking off the edge of the floor or of a platform toward the next one,
/// Isaac springs and leaps two cells across onto it, if there is somewhere
/// to land.
static void VenusLighthouseAerie_JumpToNextPlatform(void) {
  struct Actor *actor;
  union {
    struct FieldPosition position;
    s32 coordinates[3];
  } destination;
  s32 angle;
  u8 flags;

  actor = Actor_Get(ACTOR_PARTY_LEADER);
  angle = FACING_NEAREST_EIGHTH(actor->facing);
  flags = actor->motionFlags;
  SET_VECTOR(destination.coordinates,
             (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8), actor->y.fixed,
             (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8));
  Vector_AddPolarOffset(toFIXED(32), angle, destination.coordinates);
  if (FieldActor_CheckMovementCollision(actor, &destination.position) == 0) {
    GameFlag_Clear(FLAG_VENUS_AERIE_ON_PLATFORM);
    VenusLighthouseAerie_FloatPlatforms();
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    wait(6);
    Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);
    actor->speedLimit = toFIXED(3);
    actor->acceleration = toFIXED(2);
    Sound_Play(SE_JUMP);
    actor->velocityY = toFIXED(4);
    actor->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
    Actor_SetSpriteFlags(actor, 0);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER,
                        FIXEDtoINT(destination.coordinates[0]),
                        FIXEDtoINT(destination.coordinates[2]));
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    actor->motionFlags = flags;
  }
}

/// As Isaac lands on a platform, it stops bobbing, dips under him and springs
/// back up.
static void VenusLighthouseAerie_LandOnPlatform(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  struct Actor *platform;
  s32 platformId;
  FIXED reboundStep;

  Event_Start();
  platformId = Actor_FindNearestFloatingPlatform();
  gVenusLighthouseAerie_LandedPlatform = platformId;
  if (platformId != 0) {
    GameFlag_Set(FLAG_VENUS_AERIE_ON_PLATFORM);
    platform = Actor_Get(gVenusLighthouseAerie_LandedPlatform);
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
    reboundStep = toFIXED(2);
    platform->y.fixed += reboundStep;
    leader->y.fixed += reboundStep;
    leader->groundHeight += reboundStep;
    wait(4);
    platform->y.fixed += reboundStep;
    leader->y.fixed += reboundStep;
    leader->groundHeight += reboundStep;
    wait(4);
    platform->y.fixed += toFIXED(1);
    leader->y.fixed += toFIXED(1);
    leader->groundHeight += toFIXED(1);
  }
  Event_End();
}

/// Stepping onto the floor at either end of a row of platforms (cells
/// (12, 12) and (25, 12) beside the middle of the aerie, and (31, 10) on the
/// tower in the north-east), and just before each jump, sets all six
/// platforms bobbing in the air again, so the one Isaac stood on no longer
/// holds still.
static void VenusLighthouseAerie_FloatPlatforms(void) {
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_Get(ACTOR_FIRST_PLATFORM)->motionFlags = ACTOR_FLOATS;
  Actor_Get(ACTOR_FIRST_PLATFORM + 1)->motionFlags = ACTOR_FLOATS;
  Actor_Get(ACTOR_FIRST_PLATFORM + 2)->motionFlags = ACTOR_FLOATS;
  Actor_Get(ACTOR_FIRST_PLATFORM + 3)->motionFlags = ACTOR_FLOATS;
  Actor_Get(ACTOR_FIRST_PLATFORM + 4)->motionFlags = ACTOR_FLOATS;
  Actor_Get(ACTOR_LAST_PLATFORM)->motionFlags = ACTOR_FLOATS;
}

/// The aerie's events: the platforms, and the beacon where Saturos and
/// Menardi wait.
const struct Trigger *Scene_GetTriggers(void) {
  return gVenusLighthouseAerieTriggers;
}

/// Sets up the beacon's statues and the six platforms.
s32 Scene_Initialize(void) {
  FIXED mirroredScale;
  u32 platformId;
  s16 entrance;
  struct Actor *platform;

  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  wait(1);
  GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_LIT);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEACON_STATUE_SOUTHWEST), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEACON_STATUE_NORTHWEST), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEACON_STATUE_NORTHEAST), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEACON_STATUE_SOUTHEAST), 0);
  mirroredScale = -FIXED_ONE;
  Actor_Get(ACTOR_BEACON_STATUE_NORTHEAST)->scaleX = mirroredScale;
  Actor_Get(ACTOR_BEACON_STATUE_SOUTHEAST)->scaleX = mirroredScale;
  for (platformId = ACTOR_FIRST_PLATFORM; platformId <= ACTOR_LAST_PLATFORM;
       platformId++) {
    platform = Actor_Get(platformId);
    Actor_SetSpriteFlags(Actor_Get(platformId), 0);
    Actor_SetSpritePriority(platformId, 1);
    platform->motionFlags = ACTOR_FLOATS;
    platform->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    platform->y.fixed = toFIXED(1) / 2;
  }

  entrance = gMainRoutineState.entrance;

  switch (entrance) {
  case VENUS_LIGHTHOUSE_AERIE_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR:
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      VenusLighthouseAerie_OverhearSaturosAndMenardi();
    }
    break;
  case VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE:
    VenusLighthouseAerie_CombineSaturosAndMenardi();
    break;
  case VENUS_LIGHTHOUSE_AERIE_AFTER_SECOND_BATTLE:
    VenusLighthouseAerie_FinishSaturosAndMenardiBattle();
    break;
  case VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT:
    VenusLighthouseAerie_ShowShebaInDanger();
    break;
  case VENUS_LIGHTHOUSE_AERIE_FROM_WORLD_MAP:
    VenusLighthouseAerie_StageShebasFall();
    break;
  case VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT:
    Event_Start();
    if (GameFlag_Get(FLAG_SHAMANS_ROD_CARRIED_BY) != 0) {
      Inventory_AddItem(ISAAC, ITEM_SHAMANS_ROD);
    } else if (GameFlag_Get(FLAG_SHAMANS_ROD_CARRIED_BY + 1) != 0) {
      Inventory_AddItem(GARET, ITEM_SHAMANS_ROD);
    } else if (GameFlag_Get(FLAG_SHAMANS_ROD_CARRIED_BY + 2) != 0) {
      Inventory_AddItem(IVAN, ITEM_SHAMANS_ROD);
    } else {
      Inventory_AddItem(MIA, ITEM_SHAMANS_ROD);
    }
    Import_EventSetExitRequest(VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_B);
    break;
  }

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    s32 slot = Actor_FindNearestFloatingPlatform();

    if (slot != 0) {
      struct Actor *nearestPlatform = Actor_Get(slot);
      if (nearestPlatform != NULL) {
        nearestPlatform->motionFlags = 0;
      }
    }
  }
  return 0;
}

/// Shows the event's next line, then waits ten frames.
static void VenusLighthouseAerie_ShowMessage(s32 speaker) {
  Event_ShowMessage(speaker, 0);
  Event_Wait(10);
}

/// Turns one actor and waits ten frames.
static void VenusLighthouseAerie_FaceActor(s32 actorId, s32 facing) {
  Actor_FaceDirection(actorId, facing, 10);
}

/// A floating platform carries Isaac up beside the tower in the north-east of
/// the aerie onto its top, and the party looks across to the beacon, where
/// Saturos and Menardi argue with Felix over Sheba and the lighthouses still
/// to light.
static void VenusLighthouseAerie_OverhearSaturosAndMenardi(void) {
  struct Actor *arrivalPlatform;
  struct Actor *leader;

  arrivalPlatform = Actor_Get(ACTOR_RISING_PLATFORM);
  Event_Start();
  VenusLighthouseAerie_ShowPartyMember(ACTOR_GARET, 0);
  VenusLighthouseAerie_ShowPartyMember(ACTOR_IVAN, 0);
  VenusLighthouseAerie_ShowPartyMember(ACTOR_MIA, 0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  arrivalPlatform->motionFlags = 0;
  arrivalPlatform->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_RISING_PLATFORM), 0);
  Actor_SetSpritePriority(ACTOR_RISING_PLATFORM, 1);
  Actor_SetPosition(ACTOR_RISING_PLATFORM, 580, 338);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leader->motionFlags = 0;
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 581, 288);
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_RISING_PLATFORM, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_RISING_PLATFORM, 580, 221);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 581, 171);
  Actor_SetDestination(ACTOR_RISING_PLATFORM, 530, 211);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 531, 161);
  Actor_SetDestination(ACTOR_RISING_PLATFORM, 520, 191);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 521, 141);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_RISING_PLATFORM), SPRITE_FLAG_SHADOW);
  Actor_SetDestination(ACTOR_RISING_PLATFORM, 515, 171);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 516, 121);
  Sound_Play(SE_STOP_ACTION_SE);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_SPRING);
  Actor_WaitForAnimationChange(ACTOR_PARTY_LEADER);
  leader->x.fixed = toFIXED(516);
  leader->y.fixed = toFIXED(8);
  leader->z.fixed = toFIXED(148);
  leader->facing = FACING_WEST;
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Sound_Play(SE_JUMP);
  leader->velocityY = toFIXED(4);
  Sound_Play(SE_JUMP);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 504, 148);
  Event_Wait(10);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  leader->facing = FACING_SOUTH;
  leader->y.fixed = -toFIXED(32);
  Event_Wait(20);
  Sound_Play(SE_MATERIALIZE);
  Actor_MoveToAndWait(ACTOR_RISING_PLATFORM, 524, 191);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_RISING_PLATFORM), 0);
  Actor_MoveToAndWait(ACTOR_RISING_PLATFORM, 530, 211);
  Actor_MoveToAndWait(ACTOR_RISING_PLATFORM, 580, 221);
  Actor_SetDestination(ACTOR_RISING_PLATFORM, 580, 338);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  VenusLighthouseAerie_ShowPartyMember(ACTOR_GARET, 1);
  VenusLighthouseAerie_ShowPartyMember(ACTOR_IVAN, 1);
  VenusLighthouseAerie_ShowPartyMember(ACTOR_MIA, 1);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 492, 164);
  Actor_WalkTo(ACTOR_GARET, 514, 164);
  Actor_WalkTo(ACTOR_IVAN, 492, 140);
  Actor_WalkToAndWait(ACTOR_MIA, 514, 140);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_WaitForMoveById(ACTOR_RISING_PLATFORM);
  Actor_SetPosition(ACTOR_RISING_PLATFORM, 0, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Event_SetMessageId(MsgVenusLongLastTime);
  // "At long last, the time has come to light Venus Lighthouse's beacon."
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MENARDI);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_WEST);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(304, 32, 158, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH_NORTHEAST);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Sound_Play(MUS_CHANCE_MEETING_WITH_SWORN_ENEMIES);
  // "With the second beacon lit, only two will remain."
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  // "No. Lighting Jupiter is virtually the same as lighting Mars."
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MENARDI);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH_NORTHWEST);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "That is true. Jupiter is really all that remains."
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  // "That's right. Your dreams are within your grasp now."
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 20);
  FieldCamera_SetSpeed(1.6);
  FieldCamera_MoveTo(294, CAMERA_KEEP, 194, 1);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_FELIX, 0.8);
  Actor_WalkToAndWait(ACTOR_FELIX, 272, 200);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_Wait(20);
  // "Why are you still here, Felix?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  // "I thought I ordered you on ahead to the ship."
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  // "I've already sent Jenna and the others to the dock at Idejima."
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "Then why did you come back?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  // "Because I have a different promise to keep."
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "A promise, you say?"
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 40);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_WEST);
  // "You must mean Sheba..."
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  // "Yes, Sheba. Why haven't you released her yet?"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 40);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  // "You promised to free her once you got into Venus Lighthouse."
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTH_SOUTHWEST);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "And that's what brought you back here?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  // "You promised not to involve anyone who wasn't related to this matter."
  Event_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 40);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  // "Sheba is special."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 40);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  // "Of course..."
  Event_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 20);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  // "Because Sheba is a special Adept?"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  // "You guys learned something from the first two lighthouses."
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "And what would that be?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  // "In order to enter each lighthouse, you needed an Adept of that clan."
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_StartRepeatedMotion(ACTOR_MENARDI, 1);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  // "Right now, we have Fire, Earth, and Water Adepts, but no Wind Adept."
  Event_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  // "That does seem to be the case."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_NORTH_NORTHWEST);
  // "Do we need Sheba to get into the Jupiter Lighthouse?"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  VenusLighthouseAerie_FaceActor(ACTOR_SHEBA, FACING_SOUTH_SOUTHEAST);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_Wait(20);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_NORTH_NORTHEAST);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  // "I'm sorry, Felix, but we cannot even enter without a Wind Adept."
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  // "That being the case, I guess I understand why you can't set Sheba free."
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  // "It seems that you, too, have learned something. If that is all, then..."
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  // "No. I cannot leave Sheba behind!"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 0);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(294, CAMERA_KEEP, 180, 1);
  Actor_WalkToAndWait(ACTOR_FELIX, 262, 176);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 20);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  // "I'm taking Sheba to the ship!"
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "What are you talking about, Felix?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  // "We don't know what will happen when you light this beacon. I won't..."
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  // "We told you that we'd protect Sheba. Don't you trust us?"
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 20);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  // "That's not what I'm saying... It's just best to be safe."
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  // "Don't tell us what's best, Felix. You're not taking Sheba."
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "Why not? What? You don't trust me?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FELIX);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  // "How can we, after you just demanded we free Sheba?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  // "If we had trusted you a moment ago, Sheba would be free now."
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 20);
  // "Come with me, Sheba! I shall protect you!"
  Event_ShowMessageThenWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 40);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  // "Are you ready to do this, even though it means fighting us?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 40);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_NOD);
  // "...I believe he is! He would not have defied us if he lacked the will..."
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  // "If it means protecting Sheba from harm, then I must."
  Event_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_FELIX, 0, 20);
  // "You have seen what we can do. You know you can't stop us."
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  // "How can you protect Sheba, when you can't even protect yourself?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  // "This rebellious attitude is exactly why we've never let you fight."
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  // "Haven't you been given enough reason to fear us in these travels?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_SATUROS, 2);
  // "You're no fighter, Felix. How can you hope to stand against us?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  // "You've questioned us for the last time! Let's see what you can do!"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(504, -24, 168, 0);
  wait(1);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  // "It looks like they're having a falling out over Sheba."
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GARET);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "There's something more to it, though..."
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  // "It seems Saturos and Menardi no longer trust Felix."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0, 40);
  Actor_Jump(ACTOR_GARET, 2, 20);
  // "Looks like the perfect opportunity has just fallen into our laps!"
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GARET);
  // "Now's our only chance to prevent them from lighting the beacon!"
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    gFieldState->message += 1;
  }
  Event_Wait(20);
  // Yes: "There's not a moment to spare! Let's move!"
  // No: "Don't chicken out now! If we hesitate, they'll light the beacon!"
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Actor_SetScript(ACTOR_GARET, gVenusLighthouseAerie_JoinLeaderScript);
  Actor_SetScript(ACTOR_IVAN, gVenusLighthouseAerie_JoinLeaderScript);
  Actor_RunScriptAndWait(ACTOR_MIA, gVenusLighthouseAerie_JoinLeaderScript);
  Event_Wait(20);
  Event_End();
}

/// Shows or hides one companion, setting its movement speed when shown.
static void VenusLighthouseAerie_ShowPartyMember(s32 actorId, s32 shown) {
  if (shown != 0) {
    Actor_SetSpriteColorById(actorId, SPRITE_COLOR_NORMAL);
    Actor_SetSpriteFlags(Actor_Get(actorId), SPRITE_FLAG_SHADOW);
    Actor_SetSpeed(actorId, 0.8);
  } else {
    Actor_SetSpriteColorById(actorId, SPRITE_COLOR_HIDDEN);
    Actor_SetSpriteFlags(Actor_Get(actorId), 0);
  }
}

/// Isaac walks up to the beacon in the middle of the aerie (cells (20, 14) and
/// (21, 14)) and the party confronts Saturos and Menardi. Saturos offers a
/// deal for the Shaman's Rod, the party hands it over for Sheba's sake,
/// Saturos keeps her all the same, and the battle begins.
static void VenusLighthouseAerie_ConfrontSaturosAndMenardi(void) {
  s32 rodHolder;
  struct Actor *actor;
  s32 answeredYes;
  u32 frame;
  s32 receiptMessage;

  Event_Start();
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(332, 32, 180, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 340, 184);
  VenusLighthouseAerie_FaceActor(ACTOR_PARTY_LEADER, FACING_WEST);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_SetMessageId(MsgVenusIsaac);
  // "Isaac..."
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_LEFT | ACTOR_FELIX);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(304, 32, 180, 1);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_SetSpeed(ACTOR_MIA, 1.4);
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
  Actor_WalkTo(ACTOR_GARET, 328, 168);
  Actor_WalkTo(ACTOR_IVAN, 340, 196);
  Actor_WalkToAndWait(ACTOR_MIA, 326, 204);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_WEST);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 40);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  // "So! You've come to stop us?"
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_WEST);
  // "Look what your defiance has brought us, Felix!"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  // "I don't need you or anyone to rescue me, Isaac!"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  // "I was just about to settle things with Saturos and Menardi... You can..."
  Event_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHEAST, 40);
  // "And then we'll show you the true power of this beacon's light!"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_EAST);
  // "But...Sheba has nothing to do with this! So set Sheba free!"
  Event_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_WEST);
  // "Silence! You'd best tread lightly, Felix, or both you and Sheba will..."
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  // "If anything happens to Sheba, we will have failed Faran..."
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  // "We won't permit you to lay a hand on Sheba or Felix!"
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  // "Oh ho! So you're worried about Sheba, too?"
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_EAST, 20);
  // "Don't worry. We won't hurt Sheba...if you meet our conditions."
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "What are you doing, Saturos?"
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH_NORTHWEST);
  // "Leave this to me!"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 20);
  // "Sheba! Do you remember the rod connected to Hesperia?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 20);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "The Rod of Hesperia?"
  VenusLighthouseAerie_ShowMessage(ACTOR_SHEBA);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  // "The rod required to reach the Jupiter Lighthouse!"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  // "Ye-Yes..."
  VenusLighthouseAerie_ShowMessage(ACTOR_SHEBA);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  // "Those who pursue us bear a rod... Is it the one?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_WalkToAndWait(ACTOR_SHEBA, 260, 186);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 0);
  Actor_WalkToAndWait(ACTOR_SHEBA, 276, 192);
  Actor_Get(ACTOR_MENARDI)->facing = FACING_SOUTH_SOUTHWEST;
  wait(1);
  Actor_StartRepeatedMotion(ACTOR_MENARDI, 2);
  // "Sheba! Stay where you are! You can do it from there!"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_Jump(ACTOR_SHEBA, 2, 20);
  Actor_SetSpeed(ACTOR_SHEBA, 2.4);
  Actor_WalkToAndWait(ACTOR_SHEBA, 260, 186);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_WalkToAndWait(ACTOR_SHEBA, 248, 172);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 20);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(40);
  Scene_StartEventPsynergy();
  wait(1);
  for (frame = 0; frame <= 39; frame++) {
    VenusLighthouseAerie_FlashActorPalette(Actor_Get(ACTOR_SHEBA));
    wait(1);
  }
  Task_AddFrame(VenusLighthouseAerie_UpdateShebaPalette, TASK_PASS_FRAME);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_WEST);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "How about it, Sheba?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 20);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Actor_StartRepeatedMotion(ACTOR_MENARDI, 2);
  // "Well, Sheba?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Task_Remove(VenusLighthouseAerie_UpdateShebaPalette);
  wait(1);
  Actor_SetSpriteColorById(ACTOR_SHEBA, SPRITE_COLOR_NORMAL);
  wait(10);
  Scene_EndEventPsynergy();
  Actor_Jump(ACTOR_SHEBA, 2, 40);
  // "...Perhaps..."
  VenusLighthouseAerie_ShowMessage(ACTOR_SHEBA);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  // ""Perhaps..." Perhaps what!?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Actor_Get(ACTOR_SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_SHEBA, 250, 176);
  Event_Wait(1);
  Actor_Get(ACTOR_SHEBA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 20);
  // "Don't yell at her!"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  // "We're not yelling! We just want a straight answer."
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_Wait(40);
  Actor_SetSpeed(ACTOR_SHEBA, 0.6);
  Actor_Get(ACTOR_SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_SHEBA, 248, 172);
  Event_Wait(1);
  Actor_Get(ACTOR_SHEBA)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  // "So, Sheba?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  // "Their minds are veiled. I cannot see clearly...but I think...it's the..."
  VenusLighthouseAerie_ShowMessage(ACTOR_SHEBA);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 20);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  // "It's a waste to make her try again."
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "So, what do we do?"
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 100);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_Wait(20);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_EAST);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  // "Do you know the terms we will require before we agree not to harm Sheba?"
  Event_OpenMessage(ACTOR_SATUROS, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
    answeredYes = 1;
  } else {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
    gFieldState->message += 1;
    answeredYes = 0;
  }
  // Yes: "You're smarter than I thought if you've already figured it out."
  // No: "I want the rod that will get us into Jupiter Lighthouse."
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  if (answeredYes != 0) {
    gFieldState->message += 1;
  }
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  // "If you give us the Shaman's Rod, Sheba will not be harmed. Agreed?"
  Event_OpenMessage(ACTOR_SATUROS, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
    answeredYes = 1;
  } else {
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    gFieldState->message += 1;
    answeredYes = 0;
  }
  // Yes: "The Shaman's Rod? If it means saving Sheba, I will give it to you."
  // No: "He wants the Shaman's Rod. If it will save Sheba, I'll give it up."
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  if (answeredYes != 0) {
    gFieldState->message += 1;
  }
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTH);
  // "What? But Hammet gave that to you, Ivan!"
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  // "Don't you care what happens to Sheba?"
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  // "Then you will give us the rod!"
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_NOD);
  // "Well done, Saturos! We finally have the Shaman's Rod!"
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  Event_Wait(40);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_WEST);
  // "Go get it, Felix!"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  Event_Wait(20);
  // "Why me?"
  Event_ShowMessageThenWait(ACTOR_FELIX, 0, 40);
  // "To ensure your loyalty to us!"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_WEST);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  // "Go on, Felix!"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_FELIX, 0.8);
  Actor_WalkToAndWait(ACTOR_FELIX, 288, 192);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_EAST, 0);
  Actor_WalkToAndWait(ACTOR_FELIX, 310, 192);
  Actor_WalkToAndWait(ACTOR_FELIX, 328, 186);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);

  // Felix takes the Shaman's Rod from Isaac and carries it back to Saturos.
  receiptMessage = MsgVenusIsaacHandedOverShamansRod;
  // "Isaac handed over the Shaman's Rod."
  FieldMessage_Show(receiptMessage, 1);
  Actor_WalkToAndWait(ACTOR_FELIX, 310, 192);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 0);
  Actor_WalkToAndWait(ACTOR_FELIX, 288, 192);
  Actor_WalkToAndWait(ACTOR_FELIX, 262, 176);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 40);
  Event_SetMessageId(receiptMessage + 1);
  // "What should I do with the rod?"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  // "Keep it safe!"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_EAST);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_WalkToAndWait(ACTOR_SHEBA, 260, 186);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 0);
  Actor_WalkToAndWait(ACTOR_SHEBA, 276, 192);
  Sound_Play(MUS_SILENCE);
  Actor_Get(ACTOR_MENARDI)->facing = FACING_SOUTH_SOUTHWEST;
  wait(1);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  // "Where do you think you're going, Sheba?"
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH_NORTHEAST, 20);
  Actor_Jump(ACTOR_MIA, 2, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_NORTHWEST);
  // "We gave you the rod. Now, release Sheba!"
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 80);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_EAST, 40);
  Sound_Play(MUS_THE_SLEEPING_TREASURE_OF_ALTMILLER_CAVE);
  // "Whatever do you mean?"
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH_NORTHWEST, 20);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  // "You said you'd free Sheba if we gave you the rod."
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  // "Those were your terms."
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_SHEBA, FACING_NORTH_NORTHEAST);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  // "We gave you the rod, so release Sheba!"
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHEAST, 20);
  // "You seem to have misunderstood. I said we wouldn't harm Sheba... but..."
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_EAST);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  // "You're altering the bargain?"
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  // "Ha ha! We can't help it if you misinterpreted Saturos's terms!"
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_EAST);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  // "Besides, if we were to release Sheba, she'd be in quite a fix."
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH_NORTHEAST, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "What do you mean?"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  // "Sheba can't get back to Lalivero alone."
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "What do you mean? We're going to take Sheba... Oh no..."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  // "That's right. You won't be going back. This is it for you!"
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 40);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_NORTHWEST);
  // "Excuse me?"
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  // "Enough! You're the ones who aren't going anywhere!"
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_StartRepeatedMotion(ACTOR_SATUROS, 2);
  // "Bring it on, whelp! You won't live long enough to regret your stupidity!"
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  // "Prepare to experience first-hand the horror of a master Fire Adept!"
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  rodHolder = PartyInventory_FindMember(ITEM_SHAMANS_ROD);
  GameFlag_Set(rodHolder + FLAG_SHAMANS_ROD_CARRIED_BY);
  PartyInventory_Remove(ITEM_SHAMANS_ROD);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_IVAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_MIA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_MENARDI)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_SATUROS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetScript(ACTOR_PARTY_LEADER, gVenusLighthouseAerie_StepBackWestScript);
  Actor_SetScript(ACTOR_GARET, gVenusLighthouseAerie_StepBackWestScript);
  Actor_SetScript(ACTOR_IVAN, gVenusLighthouseAerie_StepBackWestScript);
  Actor_SetScript(ACTOR_MIA, gVenusLighthouseAerie_StepBackWestScript);
  Actor_SetScript(ACTOR_MENARDI, gVenusLighthouseAerie_StepBackEastScript);
  Actor_SetScript(ACTOR_SATUROS, gVenusLighthouseAerie_StepBackEastScript);
  Actor_SetScript(ACTOR_FELIX, gVenusLighthouseAerie_StepBackWestScript);
  Actor_RunScriptAndWait(ACTOR_SHEBA, gVenusLighthouseAerie_StepBackWestScript);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(SCENE_VENUS_LIGHTHOUSE_AERIE,
                                  VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE);
  Event_SetDefeatDestination(SCENE_VENUS_LIGHTHOUSE_AERIE,
                             VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 1);
  GameFlag_Set(FLAG_AERIE_FIRST_BATTLE_BEGUN);
}

/// The frames between the Psynergy rings round a flashing actor.
enum { PSYNERGY_RINGS_INTERVAL = 15 };

/// Flashes the actor white two frames in four, with Psynergy rings every
/// PSYNERGY_RINGS_INTERVAL frames.
static void VenusLighthouseAerie_FlashActorPalette(struct Actor *actor) {
  if (gFrameCount & 2) {
    Actor_SetColorMode(actor, SPRITE_COLOR_WHITE);
  } else {
    Actor_SetColorMode(actor, SPRITE_COLOR_NORMAL);
  }
  if (Math_RemainderUnsigned(gFrameCount, PSYNERGY_RINGS_INTERVAL) == 0) {
    Scene_SpawnPsynergyRings(actor);
  }
}

/// Cycles the actor through palettes 0 to 5, with Psynergy rings every
/// PSYNERGY_RINGS_INTERVAL frames.
static void VenusLighthouseAerie_CycleActorPalette(struct Actor *actor) {
  if (gFrameCount & 1) {
    Actor_SetColorMode(actor, Math_RemainderUnsigned(gFrameCount >> 1, 6));
  }
  if (Math_RemainderUnsigned(gFrameCount, PSYNERGY_RINGS_INTERVAL) == 0) {
    Scene_SpawnPsynergyRings(actor);
  }
}

/// The sound the rings of light play here (psynergy_rings.c).
#define PSYNERGY_RINGS_SOUND SE_PSYNERGY_SPARKLE
#include "shared/psynergy_rings.c"

#include "shared/event_psynergy.c"

/// Sheba glows through changing colours, with rings of Psynergy, each frame
/// while she reads the party's minds.
static void VenusLighthouseAerie_UpdateShebaPalette(void) {
  VenusLighthouseAerie_CycleActorPalette(Actor_Get(ACTOR_SHEBA));
}

/// A burst of particles flies out in BURST_DIRECTIONS directions round a
/// turn, the first one twice.
enum { BURST_DIRECTIONS = 16 };

/// Builds the horizontal velocity for an elliptical radial particle burst.
static inline void VenusLighthouseAerie_AimBurst(s32 velocity[3], s32 angle) {
  s32 sine;
  s32 cosine;

  velocity[0] = Trig_Cos(angle);
  velocity[1] = 0;
  sine = Trig_Sin(angle);
  cosine = velocity[0];
  cosine += cosine * 2;
  sine *= 2;
  velocity[2] = sine;
  velocity[0] = cosine;
}

/// Gives Felix, Sheba and the party the same reaction, then waits after Mia.
static inline void VenusLighthouseAerie_ShowGroupEmote(s32 emote,
                                                       s32 waitFrames) {
  Actor_ShowEmote(ACTOR_FELIX, emote, 0);
  Actor_ShowEmote(ACTOR_SHEBA, emote, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, emote, 0);
  Actor_ShowEmote(ACTOR_GARET, emote, 0);
  Actor_ShowEmote(ACTOR_IVAN, emote, 0);
  Actor_ShowEmote(ACTOR_MIA, emote, waitFrames);
}

/// After the first battle, Saturos and Menardi rise again. Saturos throws the
/// Venus Star into the beacon, Felix and Sheba leave, and the two fuse into
/// the Fusion Dragon for the second battle
/// (VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE).
static void VenusLighthouseAerie_CombineSaturosAndMenardi(void) {
  struct EffectOptions menardiBurst;
  s32 velocity[3];
  struct EffectOptions saturosBurst;
  struct Actor *actor;
  struct Actor *menardi;
  struct Actor *saturos;
  struct Actor *star = NULL;
  struct Actor *starShadow;
  struct Sprite *sprite;
  GlyphTransfer *transfer;
  u32 answeredYes;

  Event_Start();
  MapGrid_CopyCellAttributes(17, 10, 4, 2, 17, 8);
  Actor_Get(ACTOR_GARET)->facing = FACING_WEST;
  Actor_SetPosition(ACTOR_GARET, 328, 168);
  Actor_Get(ACTOR_IVAN)->facing = FACING_WEST;
  Actor_SetPosition(ACTOR_IVAN, 340, 196);
  Actor_Get(ACTOR_MIA)->facing = FACING_WEST;
  Actor_SetPosition(ACTOR_MIA, 326, 204);
  Actor_Get(ACTOR_SHEBA)->facing = FACING_SOUTH_SOUTHEAST;
  Actor_SetPosition(ACTOR_SHEBA, 268, 154);
  Actor_Get(ACTOR_FELIX)->facing = FACING_SOUTH_SOUTHEAST;
  Actor_SetPosition(ACTOR_FELIX, 268, 164);
  actor = Actor_Get(ACTOR_SATUROS);
  actor->timer = 10;
  actor->facing = FACING_NORTH_NORTHEAST;
  Actor_SetPosition(ACTOR_SATUROS, 294, 212);
  Actor_SetAnimation(ACTOR_SATUROS, SATUROS_ANIMATION_LIE);
  Actor_SetScript(ACTOR_SATUROS, gVenusLighthouseAerie_BreatheHeavilyScript);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SATUROS), 0);
  actor = Actor_Get(ACTOR_MENARDI);
  actor->timer = 10;
  actor->facing = FACING_EAST;
  Actor_SetPosition(ACTOR_MENARDI, 286, 192);
  Actor_SetAnimation(ACTOR_MENARDI, MENARDI_ANIMATION_LIE);
  Actor_SetScript(ACTOR_MENARDI, gVenusLighthouseAerie_BreatheHeavilyScript);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MENARDI), 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(304, 32, 180, 0);
  wait(1);
  MapGrid_Redraw();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(80);

  Actor_Jump(ACTOR_GARET, 2, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTHEAST);
  Event_SetMessageId(MsgVenusBeatEm);
  // "We did it! We beat 'em!"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_NORTHWEST);
  // "All right, Sheba... Let's go!"
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_IVAN, FACING_NORTHWEST);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  Actor_SetSpeed(ACTOR_FELIX, 0.8);
  Actor_Get(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_FELIX, 280, 164);
  Event_Wait(1);
  Actor_Get(ACTOR_FELIX)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Felix! What are you doing?"
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  // "I cannot give up Sheba..."
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  // "What!? But we just saved you!"
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_EAST);
  // "I never asked you to help us..."
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  // "Felix, you're not one of them. You're free to go now..."
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  // "So why are you doing this?"
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST);
  // "The four beacons must still be lit..."
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_Jump(ACTOR_IVAN, 2, 20);
  // "You're going to light the beacons!? That's ridiculous!"
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  // "I felt you'd say that. But I'm going to do it anyway!"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTHEAST);
  // "So, wait... If he means to light the beacons, then he's our enemy, right?"
  Event_OpenMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    answeredYes = TRUE;
  } else {
    gFieldState->message++;
    answeredYes = FALSE;
  }
  wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_NORTHWEST);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_EAST);
  // Yes: "Do what you will, Isaac, but if you try to stop me, I'll fight you."
  // No: "Friend or enemy, you won't stop me from taking Sheba with me!"
  Event_ShowMessage(ACTOR_FELIX, 0);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Event_Wait(40);
  if (answeredYes) {
    gFieldState->message++;
  }

  // Saturos and Menardi recover from their defeat before using the Venus Star.
  Actor_Stop(ACTOR_SATUROS);
  Actor_Stop(ACTOR_MENARDI);
  wait(1);
  actor = Actor_Get(ACTOR_SATUROS);
  actor->scaleX = FIXED_ONE;
  actor->scaleY = FIXED_ONE;
  actor = Actor_Get(ACTOR_MENARDI);
  actor->scaleX = FIXED_ONE;
  actor->scaleY = FIXED_ONE;
  wait(1);
  // "I hope you don't think you've finished us off."
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 40);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SATUROS), SPRITE_FLAG_SHADOW);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_SATUROS, 0.2);
  Actor_WalkToAndWait(ACTOR_SATUROS, 300, 206);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_Wait(20);
  Actor_Get(ACTOR_SATUROS)->scaleX = -FIXED_ONE;
  Sound_Play(SE_RATTLE);
  Actor_SetAnimation(ACTOR_SATUROS, SATUROS_ANIMATION_BREATHE);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MENARDI), SPRITE_FLAG_SHADOW);
  Actor_Jump(ACTOR_MENARDI, 4, 40);
  Actor_SetSpeed(ACTOR_MENARDI, 0.2);
  Actor_Get(ACTOR_MENARDI)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_MENARDI, 296, 186);
  Actor_SetSpeed(ACTOR_MENARDI, 0.1);
  Actor_WalkToAndWait(ACTOR_MENARDI, 292, 186);
  Actor_Get(ACTOR_MENARDI)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_MENARDI)->scaleX = -FIXED_ONE;
  Sound_Play(SE_RATTLE);
  Actor_SetAnimation(ACTOR_MENARDI, MENARDI_ANIMATION_STAND_WINDSWEPT);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_Wait(80);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  // "You may not be finished, but you can barely stand."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
  Event_Wait(40);
  // "Right now, yes... But we'll be back on our feet... as soon as we do THIS!"
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);

  actor = Actor_Get(ACTOR_SATUROS);
  actor->facing = FACING_SOUTH_SOUTHEAST;
  actor->scaleX = FIXED_ONE;
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_STAND);
  Event_Wait(10);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH_NORTHEAST);
  Actor_Jump(ACTOR_SATUROS, 6, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  star = (struct Actor *)FieldActor_Create(SPRITE_BLANK, actor->x.fixed,
                                           actor->y.fixed + toFIXED(8),
                                           actor->z.fixed);
  if (star != NULL) {
    sprite = star->sprite;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    sprite->image.priority = 0;
    star->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    star->motionFlags = 0;
    star->retainSpriteCache = 1;
    star->speedLimit = toFIXED(8) / 5;
    star->acceleration = toFIXED(4) / 5;
    transfer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
    // Display the Venus Star icon before sending it toward the beacon.
    Item_LoadIcon(ITEM_VENUS_STAR);
    Vram_Load(sprite->slot, ICON_TILE_BYTES, transfer->tiles);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
  Actor_SetSpritePriority(ACTOR_VENUS_STAR_SHADOW, 1);
  starShadow = Actor_Get(ACTOR_VENUS_STAR_SHADOW);
  starShadow->x.fixed = actor->x.fixed;
  starShadow->y.fixed = toFIXED(32);
  starShadow->z.fixed = actor->z.fixed;
  starShadow->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  starShadow->speedLimit = toFIXED(8) / 5;
  starShadow->acceleration = toFIXED(4) / 5;
  starShadow->scaleX = 3 * FIXED_ONE / 4;
  starShadow->scaleY = 3 * FIXED_ONE / 4;
  if (star != NULL) {
    star->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    ((struct FieldEffect *)star)->velocity.y = toFIXED(3) / 5;
    ((struct FieldEffect *)star)->velocity.x = toFIXED(4) / 5;
    star->velocityY = toFIXED(8);
    FieldActor_SetMoveTarget(star, 308, 32, 164);
  }
  Actor_MoveToAndWait(ACTOR_VENUS_STAR_SHADOW, 308, 164);
  Actor_SetPosition(ACTOR_VENUS_STAR_SHADOW, 0, 0);
  Actor_SetSpritePriority(ACTOR_FELIX, 0);
  if (star != NULL) {
    Event_PlaySound(SE_STAR_BOUNCE);
    Actor_SetSpriteFlags(star, 0);
    star->velocityY = toFIXED(4);
    FieldActor_SetMoveTarget(star, 314, 32, 137);
    Actor_WaitForMove(star);
    Event_PlaySound(SE_STAR_BOUNCE);
    star->sprite->image.priority = 1;
    star->velocityY = toFIXED(6);
    FieldActor_SetMoveTarget(star, 285, 32, 146);
    Actor_WaitForMove(star);
    Sound_Play(SE_STAR_BOUNCE);
    star->velocityY = toFIXED(5);
    FieldActor_SetMoveTarget(star, 300, 32, 154);
    Actor_WaitForMove(star);
    wait(6);
    star->x.fixed = 0;
    star->y.fixed = 0;
    star->z.fixed = 0;
    VenusLighthouseAerie_StopActorMotion(star);
  }
  VenusLighthouseAerie_ShowGroupEmote(EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_SetSpritePriority(ACTOR_FELIX, 1);
  Actor_Get(ACTOR_FELIX)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  // "Oh, no! He threw the Elemental Star into the lighthouse!"
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  // "How could this happen... We couldn't keep them from lighting the beacon!"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_Wait(20);
  // "That's not all..."
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH_SOUTHEAST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST);
  VenusLighthouseAerie_ShowGroupEmote(EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  // "The energy of the beacon will restore our power."
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 40);
  // "How can it restore their power? Venus is the earth elemental, right?"
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    answeredYes = TRUE;
  } else {
    gFieldState->message++;
    Actor_SetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
    answeredYes = FALSE;
  }
  // Yes: "Then why would Venus give power to Fire Adepts?"
  // No: "Have you forgotten everything we've learned? Venus is earth. It..."
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  if (answeredYes) {
    gFieldState->message++;
  }
  Event_Wait(20);

  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEAM_BOTTOM), 0);
  Actor_SetSpriteColorById(ACTOR_BEAM_BOTTOM, SPRITE_COLOR_WHITE);
  actor = Actor_Get(ACTOR_BEAM_BOTTOM);
  actor->scaleY = -FIXED_ONE;
  actor->scaleX = FIXED_ONE / 10;
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(64);
  actor->x.fixed = toFIXED(304);
  actor->z.fixed = toFIXED(158);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEAM_LOWER), 0);
  Actor_SetSpriteColorById(ACTOR_BEAM_LOWER, SPRITE_COLOR_WHITE);
  actor = Actor_Get(ACTOR_BEAM_LOWER);
  actor->scaleY = -FIXED_ONE;
  actor->scaleX = FIXED_ONE / 10;
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(96);
  actor->x.fixed = toFIXED(304);
  actor->z.fixed = toFIXED(158);
  VenusLighthouseAerie_ShowGroupEmote(EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_SetScript(ACTOR_BEAM_BOTTOM, gVenusLighthouseAerie_BeamOpenScript);
  Actor_SetScript(ACTOR_BEAM_LOWER, gVenusLighthouseAerie_BeamOpenScript);
  Sound_Play(SE_CRASH);
  MapGrid_SetShake(6, 6, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
  PaletteFade_Interpolate(16);
  wait(20);
  PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_Interpolate(24);
  wait(60);
  Task_AddFrame(VenusLighthouseAerie_SpawnUpperOrbitParticles, TASK_PASS_FRAME);
  Sound_Play(SE_RUMBLE);
  MapGrid_SetShake(4, 4, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
  PaletteFade_Interpolate(120);
  Actor_SetScript(ACTOR_BEAM_BOTTOM, gVenusLighthouseAerie_BeamNarrowScript);
  Actor_SetScript(ACTOR_BEAM_LOWER, gVenusLighthouseAerie_BeamNarrowScript);
  wait(120);
  MapGrid_SetShake(3, 3, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 16, 12), PALETTES_ALL);
  PaletteFade_Interpolate(120);
  wait(120);
  Sound_Play(MUS_MOUNT_ALEPH_ERUPTS);
  MapGrid_SetShake(2, 2, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_Interpolate(120);
  wait(120);
  MapGrid_SetShake(1, 1, 1);

  Actor_SetAnimation(ACTOR_MENARDI, ANIM_STAND);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 0);
  Actor_Get(ACTOR_MENARDI)->scaleX = FIXED_ONE;
  Actor_Jump(ACTOR_MENARDI, 4, 40);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  // "Fire and earth share a symbiotic relationship."
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH_SOUTHEAST, 20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  // "This light will restore our Psynergy!"
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_WEST);
  // "We'll fight you as many times as it takes. We won't lose."
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  // "You may have beaten the two of us..."
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_StartRepeatedMotion(ACTOR_MENARDI, 2);
  // "But now, you face the true might of Fire Clan of the north!"
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTHWEST);
  // "Fire Clan of the north?"
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH_NORTHEAST);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  // "Bear witness to the true strength of the northern wilds!"
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  // "Oh, give it up! We beat you once, we'll do it again!"
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  // "Go, Felix! Take Sheba and go!"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_NORTHWEST);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  // "I can...go?"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 40);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  // "We may not have the strength to protect Sheba after this."
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH_NORTHWEST);
  // "You must take Sheba to the ship without fail!"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHWEST, 60);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 40);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  // "What are you waiting for? Go! Now!"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 40);
  // "No. I cannot leave you to fight them alone."
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHEAST, 40);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH_NORTHWEST);
  // "Are you saying you don't want to lose to Isaac?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHWEST, 20);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "I wonder if that's true..."
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "What was that?"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 20);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  // "Wouldn't you be just a little sad to see your childhood friend perish?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 80);
  // "What are you saying?"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  // "I'm saying you're here to protect this brat from your past."
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  // "Do you really think that I would help them?"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  // "Let's just say that I wouldn't put it past you."
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  // "And you, Saturos? Do you agree?"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  // "I wouldn't say that... But it would be best not to raise our suspicion."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_Wait(20);
  // "Now, who's telling whom what's best? Fine! I'll take Sheba for you!"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST, 20);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHWEST, 20);
  // "Come on, Sheba!"
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX);
  Actor_SetSpeed(ACTOR_SHEBA, 1);
  Actor_SetSpeed(ACTOR_FELIX, 1);
  Actor_SetScript(ACTOR_FELIX, gVenusLighthouseAerie_WalkOutScript);
  Event_Wait(20);
  Actor_SetScript(ACTOR_SHEBA, gVenusLighthouseAerie_WalkOutScript);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTHWEST);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  // "Wait, Felix! I won't let you go!"
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Sound_Play(SOUND_FADE_OUT_BGM);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(13, 8, 9), PALETTES_BG);
  PaletteFade_Interpolate(40);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH_NORTHEAST);
  // "Heh heh... Felix is the least of your worries right now."
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  VenusLighthouseAerie_FlashScreen();
  Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_WHITE);
  Actor_SetSpriteColorById(ACTOR_MENARDI, SPRITE_COLOR_WHITE);
  Event_Wait(20);
  Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_BLINK);
  Actor_SetSpriteColorById(ACTOR_MENARDI, SPRITE_COLOR_BLINK);
  Event_Wait(20);
  VenusLighthouseAerie_FlashScreen();
  Actor_Jump(ACTOR_MIA, 2, 20);
  // "Oh my-they're glowing!"
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  // "It's time you learned what true power is!"
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  VenusLighthouseAerie_FlashScreen();

  menardi = Actor_Get(ACTOR_MENARDI);
  menardiBurst.palette = 7;
  menardiBurst.update = VenusLighthouseAerie_UpdateDriftingParticle;
  menardiBurst.startScaleX = FIXED_ONE;
  menardiBurst.startScaleY = FIXED_ONE;
  for (answeredYes = 0; answeredYes <= BURST_DIRECTIONS; answeredYes++) {
    VenusLighthouseAerie_AimBurst(velocity,
                                  answeredYes * (TRIG_TURN / BURST_DIRECTIONS));
    FieldEffect_Spawn(menardi->x.fixed, menardi->y.fixed, menardi->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE,
                      &menardiBurst);
  }
  Sound_Play(SE_POWER_UP);
  wait(6);
  VenusLighthouseAerie_FlashScreen();
  saturos = Actor_Get(ACTOR_SATUROS);
  saturosBurst.palette = 7;
  saturosBurst.update = VenusLighthouseAerie_UpdateDriftingParticle;
  saturosBurst.startScaleX = FIXED_ONE;
  saturosBurst.startScaleY = FIXED_ONE;
  for (answeredYes = 0; answeredYes <= BURST_DIRECTIONS; answeredYes++) {
    VenusLighthouseAerie_AimBurst(velocity,
                                  answeredYes * (TRIG_TURN / BURST_DIRECTIONS));
    FieldEffect_Spawn(saturos->x.fixed, saturos->y.fixed, saturos->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE,
                      &saturosBurst);
  }
  Sound_Play(SE_POWER_UP);
  Actor_Jump(ACTOR_IVAN, 6, 20);
  Sound_Play(MUS_BATTLE_WITH_THE_FUSION_DRAGON);
  // "Their Psynergy is overflowing!"
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  // "Hya ha ha! It's too late to run!"
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  VenusLighthouseAerie_FlashScreen();
  Actor_SetSpeed(ACTOR_SATUROS, 0.2);
  Actor_SetSpeed(ACTOR_MENARDI, 0.2);
  Actor_Get(ACTOR_SATUROS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_MENARDI)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_SATUROS, 294, 196);
  Actor_SetDestination(ACTOR_MENARDI, 294, 196);
  Task_AddFrame(VenusLighthouseAerie_UpdateFusion, TASK_PASS_FRAME);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  // "Uh-oh... They're fusing!"
  Event_ShowMessage(ACTOR_GARET, 0);
  GameFlag_Set(FLAG_VENUS_AERIE_FUSION_SLIDING);
  // "They've merged into one another! Everyone get back!"
  Event_ShowMessage(ACTOR_IVAN, 0);
  GameFlag_Set(FLAG_VENUS_AERIE_FUSION_GROWING);
  VenusLighthouseAerie_FlashScreen();
  Event_Wait(20);
  VenusLighthouseAerie_FlashScreen();
  Event_Wait(20);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(SCENE_VENUS_LIGHTHOUSE_AERIE,
                                  VENUS_LIGHTHOUSE_AERIE_AFTER_SECOND_BATTLE);
  Event_SetDefeatDestination(SCENE_VENUS_LIGHTHOUSE_AERIE,
                             VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 0);
  Party_RestorePpOfLitElements();
  GameFlag_Set(FLAG_AERIE_SECOND_BATTLE_BEGUN);
}

/// The aerie flashes white and goes back to the dark red it turns while a
/// thin beam of light rises from the beacon and Saturos and Menardi glow with
/// its power.
static void VenusLighthouseAerie_FlashScreen(void) {
  Sound_Play(SE_FALLING_LIGHT);
  PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_BG);
  PaletteFade_StartInterpolation(1);
  wait(4);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(13, 8, 9), PALETTES_BG);
  PaletteFade_StartInterpolation(1);
  wait(1);
}

/// A sparkle circling the beam of light over the beacon, a thirty-second of a
/// turn a frame, in a flat ellipse.
static void VenusLighthouseAerie_UpdateUpperOrbit(struct Actor *effect) {
  struct Actor *center = Actor_Get(ACTOR_BEAM_BOTTOM);
  u16 angle = effect->timer;

  effect->x.fixed =
      center->x.fixed + Trig_Cos(angle) * (effect->speedLimit + 3);
  effect->z.fixed = center->z.fixed + Trig_Sin(angle) * 2;
  effect->targetX = effect->x.fixed;
  effect->targetZ = effect->z.fixed;
  effect->timer -= TRIG_TURN / 32;
}

/// A sparkle circling the orb of light as it rises out of the beacon, in an
/// ellipse that widens as the orb climbs.
static void VenusLighthouseAerie_UpdateRisingOrbit(struct Actor *actor) {
  struct Actor *anchor = Actor_Get(ACTOR_BEACON_ORB);
  s32 angle = (u16)actor->timer;
  FIXED cosine;
  FIXED sine;
  FIXED along;
  FIXED across;

  cosine = Trig_Cos(angle);
  along =
      anchor->x.fixed + cosine * (actor->speedLimit + actor->sceneCounter + 6);
  actor->x.fixed = along;

  sine = Trig_Sin(angle);
  across = anchor->z.fixed + sine * (actor->sceneCounter + 4);
  actor->z.fixed = across;

  actor->targetX = actor->x.fixed;
  actor->targetZ = across;

  {
    s32 next = (u16)actor->timer;
    next -= TRIG_TURN / 32;
    actor->timer = next;
  }
}

/// Sparkles rise around the beam of light over the beacon, more of them and
/// higher once it has erupted.
static void VenusLighthouseAerie_SpawnUpperOrbitParticles(void) {
  struct Actor *center;
  struct Actor *effect;
  struct Sprite *sprite;
  s32 angle;

  if (Event_GetFlag(FLAG_VENUS_AERIE_BEACON_ERUPTED) == 0 &&
      Math_RemainderUnsigned(gFrameCount, 3) != 0) {
    return;
  }
  center = Actor_Get(ACTOR_BEAM_BOTTOM);
  if (GameFlag_Get(FLAG_VENUS_AERIE_BEACON_ERUPTED) != 0) {
    effect = FieldActor_Create(SPRITE_PARTICLE, center->x.fixed,
                               (s32)toFIXED(Random_Below(256)) +
                                   center->y.fixed - toFIXED(28),
                               center->z.fixed);
  } else {
    effect = FieldActor_Create(SPRITE_PARTICLE, center->x.fixed,
                               (s32)toFIXED(Random_Below(64)) +
                                   center->y.fixed - toFIXED(28),
                               center->z.fixed);
  }
  if (effect != NULL) {
    sprite = effect->sprite;
    Actor_StartScript(effect, gVenusLighthouseAerie_OrbitParticleScript);
    Actor_SetSpriteColor(effect, SPRITE_COLOR_GRB);
    effect->motionFlags = 0;
    angle = Random_Next() & 0xffff000;
    effect->timer = angle;
    effect->userValue = 0;
    effect->update = VenusLighthouseAerie_UpdateUpperOrbit;
    effect->speedLimit = Trig_Sin(Random_Below(TRIG_TURN - 1) >> 4) * 24 >> 16;
    sprite->flags = 0;
    sprite->image.priority = 1;
  }
}

/// Tests whether all three movement targets are unset.
static inline s32 Actor_IsParked(struct Actor *actor) {
  if (actor->targetX == ACTOR_NO_TARGET && actor->targetY == actor->targetX &&
      actor->targetZ == actor->targetY) {
    return 1;
  }
  return 0;
}

/// Each frame of the fusion: Saturos and Menardi, facing east, flicker in
/// turn, one shown while the other is hidden, so that they seem to merge.
/// While FLAG_VENUS_AERIE_FUSION_GROWING is set both glow white and grow to a
/// quarter larger, and while FLAG_VENUS_AERIE_FUSION_SLIDING is set they slide
/// together toward the beacon.
static void VenusLighthouseAerie_UpdateFusion(void) {
  struct Actor *saturos;
  struct Actor *menardi;
  s32 pairParked;

  saturos = Actor_Get(ACTOR_SATUROS);
  menardi = Actor_Get(ACTOR_MENARDI);
  // BUG: Saturos's test is overwritten by Menardi's, so only Menardi needs to
  // be parked.
#ifdef BUGFIX
  pairParked = Actor_IsParked(saturos) && Actor_IsParked(menardi);
#else
  pairParked = Actor_IsParked(saturos);
  pairParked = Actor_IsParked(menardi);
#endif
  if (pairParked != 0) {
    saturos->facing = FACING_EAST;
    menardi->facing = FACING_EAST;
    if (GameFlag_Get(FLAG_VENUS_AERIE_FUSION_GROWING) != 0) {
      Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_WHITE);
      Actor_SetSpriteColorById(ACTOR_MENARDI, SPRITE_COLOR_WHITE);
      if (saturos->scaleX < 5 * FIXED_ONE / 4) {
        saturos->scaleX += FIXED_ONE / 128;
        saturos->scaleY += FIXED_ONE / 128;
        menardi->scaleX += FIXED_ONE / 128;
        menardi->scaleY += FIXED_ONE / 128;
      }
    } else {
      if ((gFrameCount & 2) != 0) {
        Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_HIDDEN);
        Actor_SetSpriteColorById(ACTOR_MENARDI, SPRITE_COLOR_NORMAL);
      } else {
        Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_NORMAL);
        Actor_SetSpriteColorById(ACTOR_MENARDI, SPRITE_COLOR_HIDDEN);
      }
    }
    if (GameFlag_Get(FLAG_VENUS_AERIE_FUSION_SLIDING) != 0) {
      if (saturos->x.fixed < toFIXED(312)) {
        saturos->x.fixed += toFIXED(1) / 16;
        menardi->x.fixed += toFIXED(1) / 16;
      }
      if (saturos->z.fixed > toFIXED(182)) {
        saturos->z.fixed -= toFIXED(1) / 16;
        menardi->z.fixed -= toFIXED(1) / 16;
      }
    }
  }
}

/// Plays Saturos and Menardi's defeat, the party's discussion and the beacon's
/// eruption.
static void VenusLighthouseAerie_FinishSaturosAndMenardiBattle(void) {
  Event_Start();
  VenusLighthouseAerie_ShowSaturosAndMenardiFall();
  VenusLighthouseAerie_DiscussSaturosAndMenardiDefeat();
  VenusLighthouseAerie_EruptBeacon();
  Event_End();
}

/// A look back from the World Map shows Sheba in danger
/// (VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT, after the World Map's lighthouse
/// transition).
static void VenusLighthouseAerie_ShowShebaInDanger(void) {
  Event_Start();
  VenusLighthouseAerie_RestorePartyOnBrokenAerie();
  VenusLighthouseAerie_TryToPreventFall();
  GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  gFieldState->transitionFrames = 24;

  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(VENUS_LIGHTHOUSE_AERIE_TO_WORLD_MAP);
  Event_End();
}

/// Back from the World Map, stages the broken aerie for Sheba's fall and the
/// party's escape (VENUS_LIGHTHOUSE_AERIE_FROM_WORLD_MAP).
static void VenusLighthouseAerie_StageShebasFall(void) {
  struct Actor *leader;
  struct Actor *actor;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Start();
  FieldCamera_GetActor()->motionFlags = 0;
  MapGrid_CopyCellsTo(102, 4, 74, 4, 18, 23);
  MapGrid_CopyCellsTo(39, 72, 11, 72, 16, 20);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 22, 6);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 13, 6);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 22, 13);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 13, 13);
  wait(1);
  FieldCamera_MoveTo(192, -64, 238, 0);
  wait(1);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Actor_ReleaseById(ACTOR_SATUROS);
  Actor_ReleaseById(ACTOR_MENARDI);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_KNOCKED_DOWN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  leader->x.fixed = toFIXED(346);
  leader->z.fixed = toFIXED(205);
  leader->y.fixed = toFIXED(32);
  leader->facing = FACING_SOUTHWEST;
  VenusLighthouseAerie_StopActorMotion(leader);
  Actor_SetAnimation(ACTOR_GARET, ANIM_PARTY_KNOCKED_DOWN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  actor = Actor_Get(ACTOR_GARET);
  actor->x.fixed = toFIXED(356);
  actor->z.fixed = toFIXED(192);
  actor->facing = FACING_NORTHWEST;
  actor->y.fixed = toFIXED(32);
  VenusLighthouseAerie_StopActorMotion(actor);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_PARTY_KNOCKED_DOWN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN), 0);
  actor = Actor_Get(ACTOR_IVAN);
  actor->x.fixed = toFIXED(360);
  actor->facing = FACING_SOUTHEAST;
  actor->y.fixed = toFIXED(32);
  actor->z.fixed = toFIXED(222);
  VenusLighthouseAerie_StopActorMotion(actor);
  Actor_SetAnimation(ACTOR_MIA, ANIM_PARTY_KNOCKED_DOWN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MIA), 0);
  actor = Actor_Get(ACTOR_MIA);
  actor->x.fixed = toFIXED(334);
  actor->facing = FACING_WEST;
  actor->y.fixed = toFIXED(32);
  actor->z.fixed = toFIXED(222);
  VenusLighthouseAerie_StopActorMotion(actor);
  Actor_SetPosition(ACTOR_FELIX, 196, 220);
  Actor_SetAnimation(ACTOR_FELIX, FELIX_ANIMATION_KNEEL);
  Actor_SetPosition(ACTOR_SHEBA, 188, 316);
  Actor_SetAnimation(ACTOR_SHEBA, SHEBA_ANIMATION_HANG_ONE_HAND);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SHEBA), 0);
  actor = Actor_Get(ACTOR_BEACON_STATUE_SOUTHWEST);
  actor->x.fixed -= toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_BEACON_STATUE_NORTHWEST);
  actor->x.fixed -= toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_BEACON_STATUE_NORTHEAST);
  actor->x.fixed += toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_BEACON_STATUE_SOUTHEAST);
  actor->x.fixed += toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  MapGrid_SetShake(1, 1, 1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEACON_ORB), 0);
  Actor_Get(ACTOR_BEACON_ORB)->motionFlags = ACTOR_FLOATS;
  Actor_SetSpriteColorById(ACTOR_BEACON_ORB, SPRITE_COLOR_GBR);
  Actor_Get(ACTOR_BEACON_ORB)->y.fixed = toFIXED(40);
  Task_AddFrame(VenusLighthouseAerie_SpawnSideOrbitParticles, TASK_PASS_FRAME);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  gFieldState->transitionFrames = 24;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  VenusLighthouseAerie_PlayShebasFall();
  GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED);
  Import_EventSetExitRequest(VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_C);
}

/// Shows Saturos and Menardi falling into the beacon after their final defeat.
static void VenusLighthouseAerie_ShowSaturosAndMenardiFall(void) {
  struct Actor *actor;

  Sound_Play(SE_RUMBLE);
  MapGrid_CopyCellAttributes(17, 10, 4, 2, 17, 8);
  Actor_Get(ACTOR_PARTY_LEADER)->facing = FACING_NORTHWEST;
  Actor_Get(ACTOR_GARET)->facing = FACING_NORTHWEST;
  Actor_SetPosition(ACTOR_GARET, 328, 168);
  Actor_Get(ACTOR_IVAN)->facing = FACING_NORTHWEST;
  Actor_SetPosition(ACTOR_IVAN, 340, 196);
  Actor_Get(ACTOR_MIA)->facing = FACING_NORTHWEST;
  Actor_SetPosition(ACTOR_MIA, 326, 204);
  Actor_Get(ACTOR_FELIX)->facing = FACING_NORTH_NORTHEAST;
  Actor_SetPosition(ACTOR_FELIX, 200, 216);
  Actor_Get(ACTOR_SHEBA)->facing = FACING_NORTH_NORTHEAST;
  Actor_SetPosition(ACTOR_SHEBA, 200, 216);
  Actor_Get(ACTOR_SATUROS)->facing = FACING_SOUTH_SOUTHEAST;
  Actor_SetPosition(ACTOR_SATUROS, 310, 158);
  Actor_Get(ACTOR_MENARDI)->facing = FACING_SOUTH_SOUTHEAST;
  Actor_SetPosition(ACTOR_MENARDI, 292, 158);

  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEAM_BOTTOM), 0);
  Actor_SetSpriteColorById(ACTOR_BEAM_BOTTOM, SPRITE_COLOR_WHITE);
  Actor_SetSpritePriority(ACTOR_BEAM_BOTTOM, 1);
  actor = Actor_Get(ACTOR_BEAM_BOTTOM);
  actor->scaleY = -FIXED_ONE;
  actor->scaleX = FIXED_ONE / 5;
  actor->motionFlags = 0;
  actor->x.fixed = toFIXED(304);
  actor->y.fixed = toFIXED(2);
  actor->z.fixed = toFIXED(96);

  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEAM_LOWER), 0);
  Actor_SetSpriteColorById(ACTOR_BEAM_LOWER, SPRITE_COLOR_WHITE);
  Actor_SetSpritePriority(ACTOR_BEAM_LOWER, 1);
  actor = Actor_Get(ACTOR_BEAM_LOWER);
  actor->scaleY = -FIXED_ONE;
  actor->scaleX = FIXED_ONE / 5;
  actor->motionFlags = 0;
  actor->x.fixed = toFIXED(304);
  actor->y.fixed = toFIXED(34);
  actor->z.fixed = toFIXED(96);

  Task_AddFrame(VenusLighthouseAerie_SpawnUpperOrbitParticles, TASK_PASS_FRAME);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(304, 32, 180, 0);
  wait(1);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  MapGrid_SetShake(1, 1, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_Wait(10);
  Event_SetMessageId(MsgVenusPairDefeated);
  // "How... How... did we lose?"
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 3);
  Event_Wait(20);
  // "We are superior in every way, but still we were defeated..."
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 40);
  Sound_Play(SOUND_FADE_OUT_BGM);

  actor = Actor_Get(ACTOR_SATUROS);
  actor->x.fixed = toFIXED(308);
  actor->y.fixed = toFIXED(28);
  actor->z.fixed = toFIXED(152);
  Actor_SetAnimation(ACTOR_SATUROS, SATUROS_ANIMATION_TIP);
  Event_Wait(20);
  actor->x.fixed = toFIXED(306);
  actor->y.fixed = toFIXED(28);
  actor->z.fixed = toFIXED(152);
  Actor_SetAnimation(ACTOR_SATUROS, SATUROS_ANIMATION_SLUMP);
  Event_Wait(12);
  actor->x.fixed = toFIXED(304);
  actor->y.fixed = toFIXED(21);
  actor->z.fixed = toFIXED(152);
  Actor_SetAnimation(ACTOR_SATUROS, SATUROS_ANIMATION_SINK);
  Event_Wait(8);
  Actor_ReleaseById(ACTOR_SATUROS);

  actor = Actor_Get(ACTOR_MENARDI);
  actor->x.fixed = toFIXED(294);
  actor->y.fixed = toFIXED(28);
  actor->z.fixed = toFIXED(152);
  Actor_SetAnimation(ACTOR_MENARDI, MENARDI_ANIMATION_TIP);
  Event_Wait(20);
  actor->x.fixed = toFIXED(300);
  actor->y.fixed = toFIXED(27);
  actor->z.fixed = toFIXED(152);
  Actor_SetAnimation(ACTOR_MENARDI, MENARDI_ANIMATION_SLUMP);
  Event_Wait(12);
  actor->x.fixed = toFIXED(304);
  actor->y.fixed = toFIXED(17);
  actor->z.fixed = toFIXED(152);
  Actor_SetAnimation(ACTOR_MENARDI, MENARDI_ANIMATION_SINK);
  Event_Wait(8);
  Actor_ReleaseById(ACTOR_MENARDI);
  Event_Wait(160);
}

/// Lets the party discuss the battle while Felix and Sheba remain on the aerie.
static void VenusLighthouseAerie_DiscussSaturosAndMenardiDefeat(void) {
  s32 answeredYes;
  s32 garetSpeaker;
  s32 miaSpeaker;

  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  // "We did it! We won!"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(20);
  // "We beat them...but have we really won?"
  Event_OpenMessage(ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH);
  answeredYes = 0;
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    answeredYes = 1;
  } else {
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    gFieldState->message += 1;
  }
  // Yes: "Even though we couldn't stop the beacon from being lit?"
  // No: "All our efforts were for naught."
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  if (answeredYes != 0) {
    gFieldState->message += 1;
  }
  Actor_SetSpeed(ACTOR_GARET, 1.6);
  garetSpeaker = SPEAKER_PORTRAIT_LEFT | ACTOR_GARET;
  Actor_WalkToAndWait(ACTOR_GARET, 321, 174);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTHEAST);
  // "What are you talking about?"
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_NORTH);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTH);
  // "We did everything we could!"
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
  // "Sure we couldn't save the Venus Lighthouse, but..."
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 40);
  // "We beat them! They're gone! You saw them fall down into the pit..."
  Event_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_GARET, 0, 40);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
  // "So we don't have to worry about any more beacons being lit!"
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  // "But Felix is gone, and he's taken Sheba..."
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 80);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 80);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 60);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  VenusLighthouseAerie_FaceActor(ACTOR_IVAN, FACING_NORTH);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 40);
  Actor_Jump(ACTOR_IVAN, 4, 60);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTHEAST);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "What's with you guys? Isaac... What are you lookin' at?!?"
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 40);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_Jump(ACTOR_GARET, 4, 40);
  // "Felix!"
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_SetSpeed(ACTOR_FELIX, 0.8);
  Actor_SetDestination(ACTOR_FELIX, 200, 188);
  Actor_SetDestination(ACTOR_SHEBA, 200, 204);
  FieldCamera_SetSpeed(3.2);
  FieldCamera_MoveTo(252, 0, 190, 1);
  FieldCamera_WaitForMove();
  Event_Wait(40);
  Sound_Play(MUS_DUNGEON_THEME);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "Did you finish them?"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_Wait(20);
  // "No one knew their power better than I did. If you destroyed them..."
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  // "Then I am no match for you now, Isaac... I must go..."
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 20);
  // "Come, Sheba!"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  miaSpeaker = SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA;
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  // "Wait, Felix!"
  VenusLighthouseAerie_ShowMessage(miaSpeaker);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  Event_Wait(20);
  // "Why do you run from us?"
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_NORTHEAST);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  // "They fell into the lighthouse. It's all over!"
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  // "Poor fools... If you think it's over, you are sadly mistaken!"
  Event_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  // "If you plan to light the other beacons, you will still need..."
  VenusLighthouseAerie_ShowMessage(miaSpeaker);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  // "The Elemental Stars? We have them!"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "They're gone, Felix! You don't have to light the beacons anymore..."
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  // "Yes, I do. If I don't light the beacons..."
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  // "What will happen?"
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  // "It's no use talking about it... Just wait and see!"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHWEST, 20);
  Actor_Jump(ACTOR_MIA, 4, 20);
  // "If you won't release Sheba, we'll have to take her back!"
  Event_ShowMessageThenWait(miaSpeaker, 0, 20);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
}

static s32 sBeaconRiseTimer; ///< set as the light starts lifting the party
/// The frames VenusLighthouseAerie_UpdateBeaconRise has run, held while the
/// orb rises.
static s32 sBeaconRiseStep;

/// Raises the beacon effects and splits the aerie beneath the group.
static void VenusLighthouseAerie_EruptBeacon(void) {
  struct Actor *middle;
  struct Actor *upper;
  struct Actor *top;
  struct Actor *actor;
  u16 white;

  Sound_Play(MUS_SILENCE);
  Sound_Play(SE_STOP_ACTION_SE);
  MapGrid_SetShake(3, 3, 1);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH_NORTHEAST, 0);
  MapGrid_SetShake(2, 2, 1);
  Event_Wait(10);
  MapGrid_SetShake(1, 1, 1);
  FieldCamera_MoveTo(304, 32, 180, 1);
  FieldCamera_WaitForMove();
  Actor_Get(ACTOR_BEAM_BOTTOM)->scaleX = FIXED_ONE / 10;
  Actor_Get(ACTOR_BEAM_LOWER)->scaleX = FIXED_ONE / 10;
  Actor_SetScript(ACTOR_BEAM_BOTTOM, gVenusLighthouseAerie_BeamOpenScript);
  Actor_SetScript(ACTOR_BEAM_LOWER, gVenusLighthouseAerie_BeamOpenScript);
  Sound_Play(SE_CRASH);
  MapGrid_SetShake(6, 6, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
  PaletteFade_StartInterpolation(16);
  wait(20);
  PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_StartInterpolation(24);
  wait(60);
  Sound_Play(SE_RUMBLE);
  GameFlag_Set(FLAG_VENUS_AERIE_BEACON_ERUPTED);
  Actor_Get(ACTOR_BEAM_BOTTOM)->y.fixed = -toFIXED(96);
  Actor_Get(ACTOR_BEAM_LOWER)->y.fixed = -toFIXED(64);
  Actor_SetSpriteColorById(ACTOR_BEAM_MIDDLE, SPRITE_COLOR_WHITE);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEAM_MIDDLE), 0);
  // Stack three more beam segments above the lower two.
  middle = Actor_Get(ACTOR_BEAM_MIDDLE);
  middle->scaleY = -FIXED_ONE;
  middle->scaleX = Actor_Get(ACTOR_BEAM_BOTTOM)->scaleX;
  middle->motionFlags = 0;
  middle->x.fixed = toFIXED(304);
  middle->y.fixed = -toFIXED(32);
  middle->z.fixed = toFIXED(96);
  Actor_SetSpriteColorById(ACTOR_BEAM_UPPER, SPRITE_COLOR_WHITE);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEAM_UPPER), 0);
  upper = Actor_Get(ACTOR_BEAM_UPPER);
  upper->scaleY = -FIXED_ONE;
  upper->scaleX = Actor_Get(ACTOR_BEAM_BOTTOM)->scaleX;
  upper->motionFlags = 0;
  upper->x.fixed = toFIXED(304);
  upper->y.fixed = 0;
  upper->z.fixed = toFIXED(96);
  Actor_SetSpriteColorById(ACTOR_BEAM_TOP, SPRITE_COLOR_WHITE);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEAM_TOP), 0);
  top = Actor_Get(ACTOR_BEAM_TOP);
  top->scaleY = -FIXED_ONE;
  top->scaleX = Actor_Get(ACTOR_BEAM_BOTTOM)->scaleX;
  top->motionFlags = 0;
  top->x.fixed = toFIXED(304);
  top->y.fixed = toFIXED(32);
  top->z.fixed = toFIXED(96);
  MapGrid_CopyCellsTo(102, 4, 74, 4, 18, 23);
  MapGrid_CopyCellsTo(39, 72, 11, 72, 16, 21);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 22, 6);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 13, 6);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 22, 13);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 13, 13);
  wait(1);
  actor = Actor_Get(ACTOR_BEACON_STATUE_SOUTHWEST);
  actor->x.fixed -= toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_BEACON_STATUE_NORTHWEST);
  actor->x.fixed -= toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_BEACON_STATUE_NORTHEAST);
  actor->x.fixed += toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_BEACON_STATUE_SOUTHEAST);
  actor->x.fixed += toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->x.fixed += toFIXED(16);
  actor->z.fixed += toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_GARET);
  actor->x.fixed += toFIXED(16);
  actor->z.fixed += toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_IVAN);
  actor->x.fixed += toFIXED(16);
  actor->z.fixed += toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_MIA);
  actor->x.fixed += toFIXED(16);
  actor->z.fixed += toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  Actor_SetPosition(ACTOR_FELIX, 196, 220);
  Actor_SetAnimation(ACTOR_FELIX, FELIX_ANIMATION_KNEEL);
  Actor_SetPosition(ACTOR_SHEBA, 188, 316);
  Actor_SetAnimation(ACTOR_SHEBA, SHEBA_ANIMATION_HANG_ONE_HAND);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SHEBA), 0);
  MapGrid_SetShake(4, 4, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
  PaletteFade_StartInterpolation(120);
  Actor_SetScript(ACTOR_BEAM_BOTTOM, gVenusLighthouseAerie_BeamRiseScript);
  Actor_SetScript(ACTOR_BEAM_LOWER, gVenusLighthouseAerie_BeamRiseScript);
  Actor_SetScript(ACTOR_BEAM_MIDDLE, gVenusLighthouseAerie_BeamRiseScript);
  Actor_SetScript(ACTOR_BEAM_UPPER, gVenusLighthouseAerie_BeamRiseScript);
  Actor_SetScript(ACTOR_BEAM_TOP, gVenusLighthouseAerie_BeamRiseScript);
  wait(120);
  MapGrid_SetShake(3, 3, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 16, 12), PALETTES_ALL);
  PaletteFade_StartInterpolation(120);
  wait(120);
  MapGrid_SetShake(2, 2, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(120);
  wait(120);
  MapGrid_SetShake(1, 1, 1);
  Actor_Get(ACTOR_BEAM_BOTTOM)->scaleY = FIXED_ONE / 50;
  Actor_SetScript(ACTOR_BEAM_LOWER, gVenusLighthouseAerie_BeamCloseScript);
  Actor_SetScript(ACTOR_BEAM_MIDDLE, gVenusLighthouseAerie_BeamCloseScript);
  Actor_SetScript(ACTOR_BEAM_UPPER, gVenusLighthouseAerie_BeamCloseScript);
  Actor_RunScriptAndWait(ACTOR_BEAM_TOP, gVenusLighthouseAerie_BeamCloseScript);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Actor_SetSpriteColorById(ACTOR_BEAM_BOTTOM, SPRITE_COLOR_HIDDEN);
  Event_Wait(20);
  Actor_RunScriptAndWait(ACTOR_BEAM_BOTTOM,
                         gVenusLighthouseAerie_BeamEndScript);
  Task_Remove(VenusLighthouseAerie_SpawnUpperOrbitParticles);
  Actor_Jump(ACTOR_IVAN, 2, 20);
  // "The lighthouse...!"
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  // "Isaac! They're too far away! We can't reach Felix!"
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_SetSpeed(ACTOR_MIA, 0.8);
  Actor_WalkToAndWait(ACTOR_MIA, 326, 220);
  Event_Wait(40);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "That's not all! There's light coming from down below!"
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->sceneCounter = 0;
  actor->sceneByte = 1;
  actor->sceneWord = actor->y.fixed;
  actor->facing = FACING_NORTHWEST;
  actor = Actor_Get(ACTOR_GARET);
  actor->sceneCounter = 0;
  actor->sceneByte = 1;
  actor->facing = FACING_NORTHWEST;
  actor->sceneWord = actor->y.fixed;
  actor = Actor_Get(ACTOR_IVAN);
  actor->sceneCounter = 0;
  actor->sceneByte = 1;
  actor->facing = FACING_NORTHWEST;
  actor->sceneWord = actor->y.fixed;
  actor = Actor_Get(ACTOR_MIA);
  actor->sceneCounter = 0;
  actor->sceneByte = 1;
  actor->facing = FACING_NORTHWEST;
  actor->sceneWord = actor->y.fixed;
  actor = Actor_Get(ACTOR_FELIX);
  actor->sceneCounter = 0;
  actor->sceneByte = 1;
  actor->sceneWord = actor->y.fixed;
  actor = Actor_Get(ACTOR_SHEBA);
  actor->sceneCounter = 0;
  actor->sceneByte = 1;
  actor->sceneWord = actor->y.fixed;
  Actor_Get(ACTOR_BEACON_ORB)->motionFlags = 0;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEACON_ORB), 0);
  Actor_SetSpriteColorById(ACTOR_BEACON_ORB, SPRITE_COLOR_WHITE);
  Actor_SetSpritePriority(ACTOR_BEACON_ORB, 2);
  sBeaconRiseStep = 0;
  // DEAD: nothing reads sBeaconRiseTimer; the ROM keeps its store.
  sBeaconRiseTimer = 240;
  Task_AddFrame(VenusLighthouseAerie_UpdateBeaconRise, TASK_PASS_FRAME);
  do {
    wait(1);
  } while (GameFlag_Get(FLAG_VENUS_AERIE_PARTY_LIFTED) == 0);
  GameFlag_Set(FLAG_SKIP_SCENE_CHANGE_SOUND);
  Event_Wait(30);
  GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
  Actor_Snapshot();
  Event_SetDestination(SCENE_WORLD_MAP, WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT);
  white = RGB_WHITE;

  PLTT_BACKDROP = white;
  gFieldState->transitionFrames = 1;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

/// Places the party on the broken platform and resumes its falling effects.
static void VenusLighthouseAerie_RestorePartyOnBrokenAerie(void) {
  struct Actor *actor;

  Actor_ReleaseById(ACTOR_SATUROS);
  Actor_ReleaseById(ACTOR_MENARDI);
  Sound_Play(SE_RUMBLE);
  MapGrid_CopyCellAttributes(17, 10, 4, 2, 17, 8);
  MapGrid_CopyCellsTo(102, 4, 74, 4, 18, 23);
  MapGrid_CopyCellsTo(39, 72, 11, 72, 16, 21);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 22, 6);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 13, 6);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 22, 13);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 13, 13);
  MapGrid_SetShake(2, 2, 1);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_Restore();
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Sound_Play(SE_ITEM_BREAK);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_KNOCKED_DOWN);
  Actor_SetAnimation(ACTOR_GARET, ANIM_PARTY_KNOCKED_DOWN);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_PARTY_KNOCKED_DOWN);
  Actor_SetAnimation(ACTOR_MIA, ANIM_PARTY_KNOCKED_DOWN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MIA), 0);

  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->x.fixed = toFIXED(346);
  actor->y.fixed = toFIXED(32);
  actor->z.fixed = toFIXED(205);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor->sceneByte = 0;
  actor->velocityY = toFIXED(2);

  actor = Actor_Get(ACTOR_GARET);
  actor->x.fixed = toFIXED(356);
  actor->y.fixed = toFIXED(32);
  actor->z.fixed = toFIXED(192);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor->sceneByte = 0;
  actor->velocityY = toFIXED(2);

  actor = Actor_Get(ACTOR_IVAN);
  actor->x.fixed = toFIXED(360);
  actor->y.fixed = toFIXED(32);
  actor->z.fixed = toFIXED(222);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor->sceneByte = 0;
  actor->velocityY = toFIXED(2);

  actor = Actor_Get(ACTOR_MIA);
  actor->x.fixed = toFIXED(334);
  actor->y.fixed = toFIXED(32);
  actor->z.fixed = toFIXED(222);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor->sceneByte = 0;
  actor->velocityY = toFIXED(2);

  Actor_Get(ACTOR_FELIX)->sceneCounter = 0;
  Actor_Get(ACTOR_SHEBA)->sceneCounter = 0;
  Actor_Get(ACTOR_BEACON_ORB)->motionFlags |= ACTOR_FLOATS;
  Actor_SetSpriteColorById(ACTOR_BEACON_ORB, SPRITE_COLOR_GBR);
  Task_AddFrame(VenusLighthouseAerie_SpawnSideOrbitParticles, TASK_PASS_FRAME);
  gFieldState->transitionFrames = 1;
  Event_StartScreenTransition();
  PaletteFade_ApplySource(RGB_WHITE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(40);
  wait(60);
}

/// Shows Felix urging Sheba to hold on at the edge of the platform.
static void VenusLighthouseAerie_TryToPreventFall(void) {
  Event_SetMessageId(MsgVenusNoooo);
  // "Noooo! Sheba!"
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Sound_Play(MUS_SHEBA_HANGS_ON);
  MapGrid_SetShake(1, 1, 1);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_SetSpeed(4);
  FieldCamera_MoveTo(192, -64, 238, 1);
  FieldCamera_WaitForMove();
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  // "Hold on, Sheba! Take my hand!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 3);
  // "I can't..."
  VenusLighthouseAerie_ShowMessage(ACTOR_SHEBA);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  // "You must try! If you don't, you'll fall!"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 80);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_SHEBA, 2);
  // "Look down... The foundations of the lighthouse are crumbling!"
  VenusLighthouseAerie_ShowMessage(ACTOR_SHEBA);
}

/// Shows Sheba falling, Felix jumping after her and the party escaping.
static void VenusLighthouseAerie_PlayShebasFall(void) {
  struct Actor *actor;
  s32 answeredYes;

  answeredYes = 0;
  // Sheba loses her grip while Felix tries to keep her on the platform.
  Event_SetMessageId(MsgVenusWhyHappeningProtectVenusLighthouse);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  // "Why is this happening? Is it to protect Venus Lighthouse?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  // "I'm slipping... I can't hold on any longer."
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 20);
  Actor_SetAnimation(ACTOR_SHEBA, SHEBA_ANIMATION_HANG);
  Event_Wait(10);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  // "Nooo! Don't let go! You can't! You'll..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 40);
  Actor_SetAnimation(ACTOR_SHEBA, SHEBA_ANIMATION_SLIP);
  // "Good-bye, Felix, and thank you."
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 20);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Actor_SetScript(ACTOR_SHEBA, gVenusLighthouseAerie_ShebaFallScript);
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_WaitForScriptEndById(ACTOR_SHEBA);
  Event_Wait(160);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_Wait(20);
  Actor_Get(ACTOR_FELIX)->facing = FACING_SOUTH_SOUTHWEST;
  Actor_SetAnimation(ACTOR_FELIX, ANIM_STILL);
  Event_Wait(80);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  Event_Wait(40);
  // "Sheba!!! No! I won't let you die!"
  Event_ShowMessageThenWait(ACTOR_FELIX, 0, 40);
  Actor_SetAnimation(ACTOR_FELIX, FELIX_ANIMATION_KNEEL);
  Event_Wait(10);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FELIX), 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_STILL);
  Actor_Jump(ACTOR_FELIX, 6, 0);
  Actor_SetSpeed(ACTOR_FELIX, 3);
  Actor_Get(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  actor = Actor_Get(ACTOR_FELIX);
  // Felix jumps after Sheba; the remaining party watches from the far side.
  actor->update = VenusLighthouseAerie_StepSpriteRotation;
  actor->facing = FACING_WEST;
  Actor_MoveToAndWait(ACTOR_FELIX, 184, 237);
  Actor_RunScriptAndWait(ACTOR_FELIX, gVenusLighthouseAerie_FelixDiveScript);
  Event_Wait(120);
  // "!!!"
  VenusLighthouseAerie_ShowMessage(ACTOR_PARTY_LEADER);
  Sound_Play(MUS_FELIXS_RESOLVE);
  FieldCamera_SetSpeed(4);
  FieldCamera_MoveTo(342, 32, 212, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(40);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 60);
  Actor_Get(ACTOR_PARTY_LEADER)->timer = 10;
  Actor_SetScript(ACTOR_PARTY_LEADER,
                  gVenusLighthouseAerie_BreatheHeavilyScript);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  // "Sheba fell after that last tremor..."
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(20);
  // "Felix didn't dive into the sea after her, did he?"
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "I can't believe he did that from so high up!"
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), SPRITE_FLAG_SHADOW);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_Jump(ACTOR_GARET, 6, 60);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(20);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN), SPRITE_FLAG_SHADOW);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_Jump(ACTOR_IVAN, 6, 40);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MIA), SPRITE_FLAG_SHADOW);
  Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
  Actor_Jump(ACTOR_MIA, 6, 60);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 60);
  Actor_SetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
  // "You idiot, Felix! I can't believe he said he'd light the other beacons."
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  // "We would have had to fight him if this hadn't happened..."
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_SOUTHWEST);
  Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  // "But he sacrificed himself to save Sheba. He's not like Saturos."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 80);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTH);
  // "I'm at a loss, Isaac. We have to decide what to do next."
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 40);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
  // "What to do? What can we do?"
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 40);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
  // "What about Jenna and the others at Idejima?"
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTH);
  // "Oh yeah! I completely forgot about Jenna!"
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_SetSpeed(ACTOR_GARET, 1.6);
  Actor_WalkToAndWait(ACTOR_GARET, 333, 194);
  Actor_WalkToAndWait(ACTOR_GARET, 333, 206);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  // "Isaac! Listen up! We've got to find Jenna!"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 80);
  Actor_Stop(ACTOR_PARTY_LEADER);
  wait(1);
  actor->scaleX = FIXED_ONE;
  actor->scaleY = FIXED_ONE;
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTHEAST);
  Actor_Jump(ACTOR_GARET, 4, 0);
  // "Look at that! Isaac has finally come back to his senses!"
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_NORTHEAST);
  // "Shouldn't we go find Jenna and the others right away?"
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    // "It's about time you started agreeing with me!"
    VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    // "Isaac, you'll have to tell Jenna about Felix."
    VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
    Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
    // "I know it will be hard, but you'll be there to comfort her."
    VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
    answeredYes = 1;
  } else {
    gFieldState->message += 3;
    Event_Wait(60);
    // "After all this, you don't have the guts to tell Jenna about Felix?"
    VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    // "If you don't tell her what happened to Felix, then who will?"
    VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
    Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
    // "You're going to have to find a way to console Jenna..."
    VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  }
  if (answeredYes != 0) {
    gFieldState->message += 3;
  }
  VenusLighthouseAerie_FaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(80);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "What's the matter, Garet?"
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
  Actor_SetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
  // "I was just thinkin'... How the heck are we supposed to get down?"
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 40);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  // "Good point. This is going to make leaving very difficult..."
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Sound_Play(SE_RUMBLE);
  MapGrid_SetShake(6, 6, 1);
  Event_Wait(40);
  Sound_Play(SE_CRASH);
  MapGrid_CopyCellsTo(110, 105, 74, 4, 18, 23);
  MapGrid_CopyCellsTo(92, 86, 83, 4, 8, 23);
  MapGrid_CopyCellsTo(75, 28, 75, 4, 8, 23);
  MapGrid_CopyCellsTo(92, 86, 11, 72, 16, 20);
  MapGrid_CopyCellsTo(19, 92, 19, 68, 8, 21);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->z.fixed -= toFIXED(32);
  actor->userValue = 0;
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_GARET);
  actor->x.fixed -= toFIXED(4);
  actor->z.fixed -= toFIXED(32);
  actor->userValue = 0;
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_IVAN);
  actor->x.fixed -= toFIXED(4);
  actor->z.fixed -= toFIXED(32);
  actor->userValue = 0;
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_MIA);
  actor->x.fixed -= toFIXED(4);
  actor->z.fixed -= toFIXED(18);
  actor->userValue = 0;
  VenusLighthouseAerie_StopActorMotion(actor);
  Actor_Get(ACTOR_BEACON_ORB)->y.fixed = toFIXED(56);
  FieldCamera_MoveTo(338, 32, 180, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Actor_Jump(ACTOR_IVAN, 6, 0);
  Actor_Jump(ACTOR_MIA, 6, 0);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVenusLighthouseAerie_LookAboutScript);
  Actor_SetScript(ACTOR_GARET, gVenusLighthouseAerie_LookAboutScript);
  Actor_SetScript(ACTOR_IVAN, gVenusLighthouseAerie_LookAboutScript);
  Actor_SetScript(ACTOR_MIA, gVenusLighthouseAerie_LookAboutScript);
  MapGrid_SetShake(3, 3, 1);
  Event_Wait(80);
  MapGrid_SetShake(6, 6, 1);
  Event_Wait(40);
  Sound_Play(SE_CRASH);
  MapGrid_CopyCellAttributes(110, 106, 18, 14, 10, 5);
  MapGrid_CopyCellsTo(110, 105, 74, 4, 18, 23);
  MapGrid_CopyCellsTo(92, 86, 11, 68, 16, 20);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->x.fixed -= toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_GARET);
  actor->x.fixed -= toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_IVAN);
  actor->x.fixed -= toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_MIA);
  actor->x.fixed -= toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_BEACON_STATUE_SOUTHWEST);
  actor->x.fixed += toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_BEACON_STATUE_NORTHWEST);
  actor->x.fixed += toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_BEACON_STATUE_NORTHEAST);
  actor->x.fixed -= toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_BEACON_STATUE_SOUTHEAST);
  actor->x.fixed -= toFIXED(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  FieldCamera_MoveTo(322, 32, 180, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Actor_Jump(ACTOR_IVAN, 6, 0);
  Actor_Jump(ACTOR_MIA, 6, 0);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_Wait(80);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Event_Wait(40);
  Actor_Stop(ACTOR_PARTY_LEADER);
  Actor_Stop(ACTOR_GARET);
  Actor_Stop(ACTOR_IVAN);
  Actor_Stop(ACTOR_MIA);
  Event_Wait(120);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
  // "It's back to normal!"
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  // "That was odd... What's going on around here?"
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
  Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  // "I don't think we'll ever know."
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_Jump(ACTOR_IVAN, 2, 20);
  // "At least now, we can go search for Jenna and the others."
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_EAST);
  // "All right, let's go, Isaac! It's time to find Jenna."
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_SetSpeed(ACTOR_MIA, 1);
  Actor_SetScript(ACTOR_GARET, gVenusLighthouseAerie_PartyExitScript);
  Actor_SetScript(ACTOR_IVAN, gVenusLighthouseAerie_PartyExitScript);
  Actor_SetScript(ACTOR_MIA, gVenusLighthouseAerie_PartyExitScript);
  Event_Wait(60);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 272, 216);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 272, 254);
  Event_Wait(80);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  gFieldState->transitionFrames = MAP_TRANSITION_FRAMES;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(80);
  ActorSystem_SuspendProjectionAndShowSprites();
  ((struct WindowState *)gWindowState[0])->voiceSound = 0;
  ((struct WindowState *)gWindowState[0])->voiceCountdown = 0;
  // "But despite a long and tiring search, Jenna was nowhere to be found."
  FieldMessage_ShowCentered(MsgVenusDespiteLongTiring, 0, 0);
  ActorSystem_ResumeProjectionAndHideLayers();
  Event_Wait(80);
}

/// Stops an actor where it stands, with no move under way.
static void VenusLighthouseAerie_StopActorMotion(struct Actor *actor) {
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;

  SET_VELOCITY(actor, 0, 0, 0);

  actor->timer = 0;
}

/// Each frame as the beacon's light lifts the party away: a flash and a shake,
/// then from frame 24 the orb rises from below the aerie, held at frame 25
/// until it is above the floor and at 26 until it is 40 pixels up, with
/// particles circling it to frame 34; the screen fades to white at 36, and at
/// 48 the party is lifted (FLAG_VENUS_AERIE_PARTY_LIFTED).
static void VenusLighthouseAerie_UpdateBeaconRise(void) {
  struct Actor *center;
  struct Actor *effect;
  struct Sprite *sprite;
  s32 spawnParticles;
  u32 randomHeight;
  s32 angle;

  center = Actor_Get(ACTOR_BEACON_ORB);
  spawnParticles = 0;
  switch (sBeaconRiseStep) {
  case 0:
    Sound_Play(SE_SHIMMER);
    MapGrid_SetShake(6, 6, 1);
    PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(31, 31, 24), PALETTES_BG);
    PaletteFade_StartInterpolation(8);
    break;
  case 8:
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
    PaletteFade_StartInterpolation(8);
    break;
  case 16:
    MapGrid_SetShake(1, 1, 1);
    break;

  case 24:
    center->x.fixed = toFIXED(304);
    center->y.fixed = -toFIXED(360);
    center->z.fixed = toFIXED(164);
    center->scaleX = FIXED_ONE;
    center->scaleY = FIXED_ONE;
    VenusLighthouseAerie_StopActorMotion(center);
    Actor_SetScript(ACTOR_BEACON_ORB, gVenusLighthouseAerie_OrbPulseScript);
    break;
  case 25:
    sBeaconRiseStep--;
    if (center->y.fixed > 0) {
      PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 16, 12),
                              PALETTES_ALL);
      PaletteFade_StartInterpolation(16);
      sBeaconRiseStep++;
      Actor_Get(ACTOR_PARTY_LEADER)->sceneCounter = 1;
      Actor_Get(ACTOR_GARET)->sceneCounter = 1;
      Actor_Get(ACTOR_IVAN)->sceneCounter = 1;
      Actor_Get(ACTOR_MIA)->sceneCounter = 1;
      Actor_Get(ACTOR_FELIX)->sceneCounter = 1;
      Actor_Get(ACTOR_SHEBA)->sceneCounter = 1;
    } else {
      if ((gFrameCount & 7) == 0)
        Sound_Play(SE_GLITTER);
      spawnParticles = 1;
      center->y.fixed += toFIXED(9) / 4;
    }
    break;
  case 26:
    sBeaconRiseStep--;
    if (center->y.fixed > (toFIXED(40))) {
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
      PaletteFade_StartInterpolation(40);
      sBeaconRiseStep++;
    } else {
      if ((gFrameCount & 7) == 0)
        Sound_Play(SE_GLITTER);
      spawnParticles = 1;
      center->y.fixed += toFIXED(9) / 4;
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
    Sound_Play(SE_FALLING_LIGHT);
    PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
    PaletteFade_StartInterpolation(12);
    break;
  case 48:
    Actor_Stop(ACTOR_BEACON_ORB);
    Event_SetFlag(FLAG_VENUS_AERIE_PARTY_LIFTED);
    break;
  }
  if (spawnParticles) {
    randomHeight = toFIXED(Random_Below(80));
    effect = FieldActor_Create(SPRITE_PARTICLE, center->x.fixed,
                               center->y.fixed - randomHeight - toFIXED(8),
                               center->z.fixed);
    if (effect != NULL) {
      sprite = effect->sprite;
      Actor_StartScript(effect, gVenusLighthouseAerie_RiseParticleScript);
      Actor_SetSpriteColor(effect, SPRITE_COLOR_GRB);
      effect->motionFlags = 0;
      angle = Random_Next() & 0xffff000;
      effect->timer = angle;
      effect->userValue = 0;
      effect->sceneCounter = Random_Below(8);
      effect->update = VenusLighthouseAerie_UpdateRisingOrbit;
      effect->speedLimit = Trig_Sin(Random_Below(TRIG_TURN - 1) >> 4) * 24;
      effect->speedLimit = FIXEDtoINT(center->speedLimit);
      sprite->flags = 0;
      sprite->image.priority = 1;
    }
  }
  sBeaconRiseStep++;
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(ACTOR_PARTY_LEADER));
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(ACTOR_GARET));
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(ACTOR_IVAN));
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(ACTOR_MIA));
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(ACTOR_FELIX));
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(ACTOR_SHEBA));
}

/// An actor the light lifts rises this many quarter pixels, one a frame.
enum { RISE_QUARTER_PIXELS = 32 };

/// Lifts one of the party, Felix or Sheba off the aerie as the beacon's light
/// takes them, a quarter pixel a frame up to 8 pixels.
static void VenusLighthouseAerie_AdvanceActorRise(struct Actor *actor) {
  if (actor->sceneByte != 0) {
    u8 counter = actor->sceneCounter;

    actor->y.fixed = actor->sceneWord + toFIXED(counter >> 2);

    VenusLighthouseAerie_StopActorMotion(actor);

    if (actor->sceneCounter != 0) {
      if (actor->sceneCounter < RISE_QUARTER_PIXELS) {
        ++actor->sceneCounter;
      }
    }
  }
}

/// Sparkles circle the foot of the beam of light, each on its own flat
/// ellipse.
static void VenusLighthouseAerie_SpawnSideOrbitParticles(void) {
  struct Actor *center;
  struct Actor *effect;
  struct Sprite *sprite;
  struct MapGridScrollState *grid;
  s32 drift;
  s32 phase;
  s32 angle;

  center = Actor_Get(ACTOR_BEACON_ORB);
  grid = gMapGridState[0];
  drift = toFIXED(Random_Below(48));
  if ((s16)(FIXEDtoINT(grid->view[1])) <= 129) {
    if (gFrameCount & 1) {
      Actor_SetPosition(ACTOR_BEACON_ORB, 304, 164);
      Actor_Get(ACTOR_BEACON_ORB)->scaleX = FIXED_ONE;
      Actor_Get(ACTOR_BEACON_ORB)->scaleY = FIXED_ONE;
    } else {
      Actor_SetPosition(ACTOR_BEACON_ORB, 304, 171);
      Actor_Get(ACTOR_BEACON_ORB)->scaleX = 13 * FIXED_ONE / 10;
      Actor_Get(ACTOR_BEACON_ORB)->scaleY = 13 * FIXED_ONE / 10;
    }
  } else {
    Actor_SetPosition(ACTOR_BEACON_ORB, 0, 0);
  }
  if (center == NULL)
    return;
  phase = gFrameCount & 15;
  if (phase != 0)
    return;
  effect =
      FieldActor_Create(SPRITE_PARTICLE, center->x.fixed + toFIXED(8),
                        center->y.fixed + drift + toFIXED(8), center->z.fixed);
  drift /= toFIXED(6);
  drift = INTtoFIXED(drift);
  if (effect == NULL)
    return;
  sprite = effect->sprite;
  Actor_StartScript(effect, gBeacon_SparkScript);
  Actor_SetSpriteColor(effect, SPRITE_COLOR_BRG);
  effect->motionFlags = phase;
  angle = Random_Next() & 0xffff000;
  effect->timer = angle;
  effect->userValue = phase;
  effect->watchedActor = center;
  effect->update = VenusLighthouseAerie_UpdateOrbitingEffect;
  effect->speedLimit = FIXEDtoINT(Trig_Sin((drift & 0xfffff) >> 4) * 24);
  sprite->flags = 0;
  sprite->image.priority = center->sprite->image.priority;
}

/// Moves a sparkle round the foot of the beam on its ellipse.
static void VenusLighthouseAerie_UpdateOrbitingEffect(struct Actor *actor) {
  struct FieldEffect *effect = (struct FieldEffect *)actor;
  struct Actor *anchor;
  s32 spin;

  anchor = actor->watchedActor;
  spin = effect->spin;
  effect->x = anchor->x.fixed + Trig_Cos(spin) * (actor->speedLimit + 28);
  effect->z = Trig_Sin(spin) * 16 + toFIXED(164);
  actor->targetX = effect->x;
  actor->targetZ = effect->z;
  effect->spin -= TRIG_TURN / 128;
}

#include "shared/field_effect_duration_scripts.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// Garet, Ivan and Mia walk up to Isaac on the tower once the party has
/// agreed to stop Saturos and Menardi, and step out of sight into him.
s32 gVenusLighthouseAerie_JoinLeaderScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(492, 0, 164),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/// Everyone steps half a cell apart as the first battle begins: the party,
/// Felix and Sheba west, Saturos and Menardi east.
s32 gVenusLighthouseAerie_StepBackWestScript[] = {
    SC_SPEED(0.2),
    SC_ACCELERATION(0.1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(-8, 0, 0),
    SC_SYNC,
    SC_END,
};

s32 gVenusLighthouseAerie_StepBackEastScript[] = {
    SC_SPEED(0.2),
    SC_ACCELERATION(0.1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(8, 0, 0),
    SC_SYNC,
    SC_END,
};

/// Breathes heavily for as long as it runs
/// (VenusLighthouseAerie_PulseActorScale).
s32 gVenusLighthouseAerie_BreatheHeavilyScript[] = {
    SC_CALL(VenusLighthouseAerie_PulseActorScale),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Widens a beam segment from a sliver.
s32 gVenusLighthouseAerie_BeamOpenScript[] = {
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_WAIT(1),
    SC_LOOP(32, SC_TOP),
    SC_END,
};

/// Narrows a beam segment again as Saturos and Menardi take its power.
s32 gVenusLighthouseAerie_BeamNarrowScript[] = {
    SC_ADD(SW_SCALE_X, -7 * FIXED_ONE / 512),
    SC_WAIT(1),
    SC_LOOP(140, SC_TOP),
    SC_END,
};

/// Narrows each segment of the erupting beam, for longer than
/// gVenusLighthouseAerie_BeamNarrowScript.
s32 gVenusLighthouseAerie_BeamRiseScript[] = {
    SC_ADD(SW_SCALE_X, -7 * FIXED_ONE / 512),
    SC_WAIT(1),
    SC_LOOP(160, SC_TOP),
    SC_END,
};

/// Lifts a beam segment four pixels a frame, then takes it off the stage.
s32 gVenusLighthouseAerie_BeamCloseScript[] = {
    SC_ADD(SW_Y, toFIXED(4)), SC_WAIT(1), SC_LOOP(30, SC_TOP),
    SC_POSITION(0, 0, 0),     SC_WAIT(1), SC_END,
};

/// Lifts the beam's flattened bottom two pixels a frame, then takes it off
/// the stage.
s32 gVenusLighthouseAerie_BeamEndScript[] = {
    SC_ADD(SW_Y, toFIXED(2)), SC_WAIT(1), SC_LOOP(160, SC_TOP),
    SC_POSITION(0, 0, 0),     SC_WAIT(1), SC_END,
};

/// A sparkle round the beam over the beacon rises 16 pixels and is gone.
s32 gVenusLighthouseAerie_OrbitParticleScript[] = {
    SC_SET(SW_SCALE_X, 4 * FIXED_ONE / 5),
    SC_SET(SW_SCALE_Y, 4 * FIXED_ONE / 5),
    SC_ANIMATION(ANIM_STAND),
    SC_LABEL(1),
    SC_ADD(SW_Y, toFIXED(1)),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

/// A sparkle round the rising orb sinks 16 pixels and is gone.
s32 gVenusLighthouseAerie_RiseParticleScript[] = {
    SC_SET(SW_SCALE_X, 4 * FIXED_ONE / 5),
    SC_SET(SW_SCALE_Y, 4 * FIXED_ONE / 5),
    SC_ANIMATION(ANIM_STAND),
    SC_LABEL(1),
    SC_ADD(SW_Y, -toFIXED(1)),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

/// Felix, then Sheba, walk off down the aerie's stairs after the first
/// battle.
s32 gVenusLighthouseAerie_WalkOutScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(268, 0, 186),
    SC_SYNC,
    SC_MOVE(268, 0, 252),
    SC_SYNC,
    SC_MOVE(232, 0, 222),
    SC_SYNC,
    SC_MOVE(232, 0, 136),
    SC_SYNC,
    SC_MOVE(200, 0, 136),
    SC_SYNC,
    SC_MOVE(200, 0, 200),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/// The orb rising out of the beacon narrows and widens again, four frames
/// each way, as long as it shows.
s32 gVenusLighthouseAerie_OrbPulseScript[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -3 * FIXED_ONE / 128),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, 3 * FIXED_ONE / 128),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Sheba drops from the broken aerie, 90 pixels over two seconds, and is gone.
s32 gVenusLighthouseAerie_ShebaFallScript[] = {
    SC_ADD(SW_Y, -(toFIXED(3) / 4)),
    SC_WAIT(1),
    SC_LOOP(120, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/// Felix dives after her, much faster, and is gone.
s32 gVenusLighthouseAerie_FelixDiveScript[] = {
    SC_ADD(SW_Y, -(toFIXED(9) / 2)),
    SC_WAIT(1),
    SC_LOOP(80, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/// Garet, Ivan and Mia walk off ahead of Isaac as the party leaves to find
/// Jenna.
s32 gVenusLighthouseAerie_PartyExitScript[] = {
    SC_ANIMATION(ANIM_WALK), SC_MOVE(330, 0, 173), SC_SYNC, SC_RELEASE, SC_END,
};

/// The party looks about them at random.
s32 gVenusLighthouseAerie_LookAboutScript[] = {
    SC_CALL(Scene_LookAboutRandomly),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gVenusLighthouseAerieEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 304, 0, 216, FACING_SOUTH_SOUTHEAST, 0,
     0, 0, 600, 420, 0},
    {VENUS_LIGHTHOUSE_AERIE_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR,
     CONDITION_ALWAYS, 504, 0, 168, FACING_SOUTH_SOUTHEAST, 0, 0, 0, 600, 420,
     0},
    {VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE, CONDITION_ALWAYS, 340, 0, 184,
     FACING_WEST, 0, 0, 0, 600, 420, 0},
    {VENUS_LIGHTHOUSE_AERIE_AFTER_SECOND_BATTLE, CONDITION_ALWAYS, 340, 0, 184,
     FACING_WEST, 0, 0, 0, 600, 420, 0},
    {VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT, CONDITION_ALWAYS, 0, 0, 0,
     FACING_WEST, 0, 0, 0, 600, 420, 0},
    {SCENE_TABLE_END},
};

u32 gVenusLighthouseAerieExits[] = {
    SCENE_EXITS(VenusLighthouseAerie),
    SCENE_EXIT(VENUS_LIGHTHOUSE_AERIE_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_BEFORE_SHEBAS_FALL),
    SCENE_EXIT(VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_C, TimberRoomsC,
               TIMBER_ROOMS_C_FROM_VENUS_LIGHTHOUSE_AERIE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_B, TimberRoomsB,
               TIMBER_ROOMS_B_SANCTUM),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gVenusLighthouseAerieActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        328, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        440, 0, 252, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        472, 0, 252, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        504, 0, 252, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        104, 0, 252, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        136, 0, 252, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        168, 0, 252, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        504, 0, 148, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        504, 0, 148, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        504, 0, 148, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MENARDI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 164, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        296, 0, 180, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 172, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FELIX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 252, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};
// clang-format on

/// The floating platform the leader has landed on, 0 when none.
s32 gVenusLighthouseAerie_LandedPlatform = 0;

struct Trigger gVenusLighthouseAerieTriggers[] = {
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     CELL_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_FLOOR_BY_PLATFORMS, CONDITION_ALWAYS,
     EVENT(VenusLighthouseAerie_JumpToNextPlatform)},
    {TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_FLOOR_BY_PLATFORMS,
     CONDITION_ALWAYS, EVENT(VenusLighthouseAerie_FloatPlatforms)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     CELL_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_PLATFORM, CONDITION_ALWAYS,
     EVENT(VenusLighthouseAerie_JumpToNextPlatform)},
    {TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_PLATFORM,
     FLAG_VENUS_AERIE_ON_PLATFORM, EVENT(VenusLighthouseAerie_LandOnPlatform)},
    {TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_BEFORE_BEACON,
     CONDITION_ALWAYS, EVENT(VenusLighthouseAerie_ConfrontSaturosAndMenardi)},
    {TRIGGER_TABLE_END},
};

#include "shared/beacon_spark_script.c"
