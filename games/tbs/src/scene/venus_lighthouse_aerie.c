/// @file
/// Lighthouse Aerie (SceneId_VenusLighthouseAerie), the top of Venus Lighthouse
/// and the only map of this overlay: one scripted sequence cut by two battles
/// and two trips to the world map. Each part starts in
/// VenusLighthouseAerie_Initialize from the entrance the part before
/// sent the party to, and every arrival blocks Retreat and sets
/// FLAG_VENUS_LIGHTHOUSE_LIT. The parts, in the order the entrances
/// chain them:
/// - The arrival (VENUS_LIGHTHOUSE_AERIE_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR,
///   VenusLighthouseAerie_OverhearSaturosAndMenardi): a platform carries the
///   leader up and a conversation plays, with one yes-or-no question that
///   changes only the reply.
/// - Six floating platforms lie between the party and the beacon
///   (MB_VENUS_LIGHTHOUSE_AERIE_PLATFORM,
///   MB_VENUS_LIGHTHOUSE_AERIE_FLOOR_BY_PLATFORMS): the leader jumps from one
///   to the next, and each stops bobbing and dips under him as he lands
///   (FLAG_VENUS_AERIE_ON_PLATFORM while he stands on one).
/// - The confrontation (MB_VENUS_LIGHTHOUSE_AERIE_BEFORE_BEACON,
///   VenusLighthouseAerie_ConfrontSaturosAndMenardi): the Shaman's Rod is
///   handed over, remembered in FLAG_SHAMANS_ROD_HOLDER plus the member who
///   carried it, and the first battle follows (ENCOUNTER_ZONE_SCRIPTED, slot 1,
///   FLAG_AERIE_FIRST_BATTLE_BEGUN).
/// - After it (VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE,
///   VenusLighthouseAerie_CombineSaturosAndMenardi): Saturos throws the Venus
///   Star into the beacon (ACTOR_VENUS_STAR_SHADOW), Felix and Sheba leave,
///   and Saturos and Menardi fuse. Party_RestorePpOfLitElements refills the PP
///   of the party's Adepts whose lighthouses are lit, and the second battle
///   follows (slot 0, FLAG_AERIE_SECOND_BATTLE_BEGUN).
/// - After that (VENUS_LIGHTHOUSE_AERIE_AFTER_SECOND_BATTLE,
///   VenusLighthouseAerie_FinishSaturosAndMenardiBattle): Saturos and Menardi
///   fall into the lighthouse, and the beacon erupts: its beam
///   (ACTOR_BEAM_BOTTOM to ACTOR_BEAM_TOP) rises, the aerie splits and its
///   statues move apart, and the light lifts the party away to the world map's
///   lighthouse transition (WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT). The beam's
///   five segments are sprites stacked over the beacon, with particles rising
///   round them, and the split is drawn by copying the map grid's cells
///   (MapGrid_CopyCellsTo in VenusLighthouseAerie_EruptBeacon)
///   (https://alchemy.pascalpixel.com/places/venus-lighthouse-aerie/eruption.gif:
///   eight seconds from just before the segments stack, each numbered 1 to 5
///   from the bottom;
///   https://alchemy.pascalpixel.com/places/venus-lighthouse-aerie/eruption-layers-3d.gif:
///   five and a half seconds in, pulled apart into its layers, the sky at the
///   back, BG2, BG1 and BG0, and the sprites in front).
/// - A look back (VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT,
///   VenusLighthouseAerie_ShowShebaInDanger): Sheba hangs from the broken
///   aerie, and VENUS_LIGHTHOUSE_AERIE_TO_WORLD_MAP returns to the world map
///   (WORLD_MAP_BEFORE_SHEBAS_FALL) with its music kept.
/// - Sheba's fall (VENUS_LIGHTHOUSE_AERIE_FROM_WORLD_MAP,
///   VenusLighthouseAerie_StageShebasFall): Sheba and Felix fall, and the party
///   is taken to Lalivero (VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_C)
///   with FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED set.
///
/// A lost battle returns to VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT, which gives
/// the Shaman's Rod back to whoever carried it and leaves for Lalivero Sanctum
/// (VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_B).

#include "action_script.h"
#include "save_block.h"
#include "vram_cache.h"
#include "unit/import.h"
#include "unit/scene/venus_lighthouse_aerie.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/scene/common/object_nearest.h"
#include "unit/scene/common/effect_psynergy.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
#include "unit/runtime/runtime.h"
#include "macros.h"

#define NULL ((void *)0)
#include "actor.h"
#include "types.h"
#include "animspr.h"
#include "venus_lighthouse_aerie.h"
#include "field_effect.h"
#include "script_layer.h"
#include "scene_tables.h"
#include "songs.h"
#include "audio_engine_symbols.h"
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
#include "task.h"
#include "call.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "heap_slots.h"
#include "scene_ids.h"
#include "pushed_object.h"
#include "map_grid_scroll.h"
#include "tbs_edition.h"
#include "io_reg.h"
#include "window.h"
#include "heap.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "fx_work.h"
#include "palbuf.h"
#include "flags.h"
#include "item_ids.h"

// Area flags: Scene_Arrive clears 0x300 to 0x3ff when the
// party enters another area, and other areas use the numbers for
// their own.

/// Plus the party member (0 to 3) who carried the Shaman's Rod when it was
/// handed over before the aerie's first battle; a defeat gives it back.
/// ★ Named from the game: "Shaman's Rod" (MsgItemName)
#define FLAG_SHAMANS_ROD_HOLDER 0x345

/// Area flags that choose the aerie's field music after each battle, by
/// the map's field music rules (gScene_MusicRules,
/// overworld/map_rules.s): MUS_SILENCE before the first battle, MUS_BABI once
/// the first has begun and MUS_ELEMENTAL_STARS once the second has.
/// VenusLighthouseFirstFloor and VenusLighthouseFourthFloor, in the same
/// area, keep their Psynergy Stones in the same two flags, which no one goes
/// back down to see.
enum {
  /// ★ Named from its area name: Lighthouse Aerie
  FLAG_AERIE_FIRST_BATTLE_BEGUN = FLAG_AREA_TEMP_50,
  /// ★ Named from its area name: Lighthouse Aerie
  FLAG_AERIE_SECOND_BATTLE_BEGUN = FLAG_AREA_TEMP_51
};

static void VenusLighthouseAerie_FloatPlatforms(void);
static void VenusLighthouseAerie_OverhearSaturosAndMenardi(void);
static void VenusLighthouseAerie_ShowPartyMember(s32 actorId, s32 shown);
static void VenusLighthouseAerie_FlashActorPalette(struct Actor *actor);
static void VenusLighthouseAerie_SpawnLinkedPairEffects(struct Actor *parent);
static void VenusLighthouseAerie_UpdateShebaPalette(void);
static void VenusLighthouseAerie_CombineSaturosAndMenardi(void);
static void VenusLighthouseAerie_FlashScreen(void);
static void VenusLighthouseAerie_SpawnUpperOrbitParticles(void);
static void VenusLighthouseAerie_UpdateBeamActors(void);
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

/// The metatile behaviors the aerie's step triggers answer.
enum VenusLighthouseAerieMetatileBehavior {
  MB_VENUS_LIGHTHOUSE_AERIE_BEFORE_BEACON =
      5, ///< where the party confronts Saturos and Menardi
  MB_VENUS_LIGHTHOUSE_AERIE_PLATFORM = 10, ///< a floating platform's cells
  /// The floor beside the platforms, from which the leader jumps
  /// onto them.
  MB_VENUS_LIGHTHOUSE_AERIE_FLOOR_BY_PLATFORMS = 11
};

/// A child value that makes an actor blink rather than set its colour
/// mode (Actor_ConfigureSpriteColor).
enum { CHILD_VALUE_BLINK = 0x100 };

/// Animations the aerie plays by number beyond enum ActorAnimation, from
/// each sprite's animation table (graphics/sprite/spr_<number>.tsv; the party's
/// through their animation lists).
enum {
  /// Saturos standing, two pictures alternating at long intervals
  /// (spr_032.tsv).
  SATUROS_ANIMATION_BREATHE = 8,
  SATUROS_ANIMATION_LIE = 9, ///< lying on his side
  /// Saturos falling into the lighthouse: upright, then half his height, then
  /// only his top above the floor.
  SATUROS_ANIMATION_TIP = 10,
  SATUROS_ANIMATION_SLUMP = 11,
  SATUROS_ANIMATION_SINK = 12,
  /// Menardi standing, her hair blown back (spr_033.tsv).
  MENARDI_ANIMATION_STAND_WINDSWEPT = 5,
  MENARDI_ANIMATION_LIE = 7, ///< lying on her side
  /// Menardi falling into the lighthouse, as SATUROS_ANIMATION_TIP to _SINK.
  MENARDI_ANIMATION_TIP = 8,
  MENARDI_ANIMATION_SLUMP = 9,
  MENARDI_ANIMATION_SINK = 10,
  /// Garet on his knees, Ivan and Mia lying down, as the party lies on the
  /// broken aerie (spr_001.tsv to spr_003.tsv).
  ANIM_PARTY_KNOCKED_DOWN = 18,
  ANIM_LEADER_KNOCKED_DOWN = 19, ///< Isaac lying on his back (spr_000.tsv)
  FELIX_ANIMATION_KNEEL = 5,     ///< Felix crouched on one knee (spr_035.tsv)
  /// Sheba hanging from the broken aerie by one hand, seen from the side
  /// (spr_006.tsv).
  SHEBA_ANIMATION_HANG_ONE_HAND = 5,
  SHEBA_ANIMATION_HANG = 6, ///< hanging by both hands, seen from behind
  /// Her grip slipping for a moment, then SHEBA_ANIMATION_HANG again.
  SHEBA_ANIMATION_SLIP = 7
};

// The aerie's map flags: FLAG_VENUS_AERIE_FUSION_SLIDING: Saturos and Menardi
// slide together as they fuse. FLAG_VENUS_AERIE_FUSION_GROWING: they grow as
// they fuse. FLAG_VENUS_AERIE_BEACON_ERUPTED: the beacon has erupted; its
// particles rise higher. FLAG_VENUS_AERIE_PARTY_LIFTED: the light has lifted
// the party and flashed white. FLAG_VENUS_AERIE_ON_PLATFORM: the leader stands
// on a platform.

#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Pulses the actor scale around the countdown, then chooses its next
/// pause.
static s32 VenusLighthouseAerie_PulseActorScale(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
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
    *counter = Math_RemainderUnsigned(Import_RandomNext(), 80) + 80;
    break;
  }
  {
    s32 t = (u16)actor->timer;
    t--;
    actor->timer = t;
  }
  return 1;
}

/// Moves a particle, damps its horizontal velocity and updates its scale and
/// spin.
static void VenusLighthouseAerie_UpdateDriftingParticle(struct Actor *actor) {
  s32 velocityX = ((struct FieldEffect *)actor)->velocity.x;
  s32 velocityZ;

  ((struct FieldEffect *)actor)->x += velocityX;
  ((struct FieldEffect *)actor)->y += ((struct FieldEffect *)actor)->velocity.y;
  velocityZ = ((struct FieldEffect *)actor)->velocity.z;
  ((struct FieldEffect *)actor)->z += velocityZ;
  ((struct FieldEffect *)actor)->velocity.x = velocityX - velocityX / 22;
  ((struct FieldEffect *)actor)->velocity.z = velocityZ - velocityZ / 20;
  ((struct FieldEffect *)actor)->scaleX +=
      ((struct FieldEffect *)actor)->scaleRateX;
  ((struct FieldEffect *)actor)->scaleY +=
      ((struct FieldEffect *)actor)->scaleRateY;
  ((struct FieldEffect *)actor)->sprite->rotation +=
      ((struct FieldEffect *)actor)->spin;
}

/// Turns the falling actor sprite until its rotation leaves the negative
/// half-turn.
static void VenusLighthouseAerie_StepSpriteRotation(struct Actor *updated) {
  struct Actor *actor = (struct Actor *)updated;
  struct Sprite *sprite = actor->sprite;
  u16 rotation = sprite->rotation;

  if ((s16)(rotation - 1) < 0) {
    {
      s32 nextRotation = rotation - 0xa00;
      sprite->rotation = nextRotation;
    }
  }
}

#include "common/object_heading.h"

const struct SceneEntrance *VenusLighthouseAerie_GetEntrances(void) {
  return gVenusLighthouseAerieEntrances;
}

const struct SceneExitRegion *VenusLighthouseAerie_GetRegions(void) {
  return 0;
}

const u32 *VenusLighthouseAerie_GetExits(void) {
  return gVenusLighthouseAerieExits;
}

/// Returns the beacon's statues, the platforms, Saturos and Menardi, Felix and
/// Sheba, the party and the beacon effects.
const struct ActorTemplate *VenusLighthouseAerie_GetActorTemplates(void) {
  return gVenusLighthouseAerieActorTemplates;
}

#include "common/object_distance.h"
#include "common/object_nearest.h"

/// Saturos and Menardi slide together as they fuse.
/// ★ Named from the game: "Fusion Dragon" (MsgEnemyName)
#define FLAG_VENUS_AERIE_FUSION_SLIDING FLAG_SCENE_TEMP_34
/// Saturos and Menardi grow as they fuse.
/// ★ Named from the game: "Fusion Dragon" (MsgEnemyName)
#define FLAG_VENUS_AERIE_FUSION_GROWING FLAG_SCENE_TEMP_35
/// The beacon has erupted, and its particles rise higher.
/// ★ Named from the game: "At long last, the time has come to light Venus
/// Lighthouse's beacon." (MsgVenusLongLastTime)
#define FLAG_VENUS_AERIE_BEACON_ERUPTED FLAG_SCENE_TEMP_36
/// The beacon's light has lifted the party and flashed white.
/// ★ Named from the game: "At long last, the time has come to light Venus
/// Lighthouse's beacon." (MsgVenusLongLastTime)
#define FLAG_VENUS_AERIE_PARTY_LIFTED FLAG_SCENE_TEMP_37
/// The leader stands on one of the floating platforms.
/// ★ Named from what the player sees: round pale blue platforms floating in the
/// air between the lighthouse and the towers beside it (its render)
#define FLAG_VENUS_AERIE_ON_PLATFORM FLAG_SCENE_TEMP_50

/// Walking off the edge of a platform: if the ground two cells ahead is clear,
/// the leader jumps across to it.
static void VenusLighthouseAerie_JumpToNextPlatform(void) {
  struct Actor *actor;
  union {
    struct FieldPosition position;
    s32 coordinates[3];
  } destination;
  s32 angle;
  u8 flags;

  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  angle = (actor->facing + 0x1000) & 0xe000;
  flags = actor->motionFlags;
  SET_VECTOR(destination.coordinates, (actor->x.fixed & 0xfff00000) + PIXELS(8),
             actor->y.fixed, (actor->z.fixed & 0xfff00000) + PIXELS(8));
  Vector_AddPolarOffset(PIXELS(32), angle, destination.coordinates);
  if (FieldActor_CheckMovementCollision(actor, &destination.position) == 0) {
    GameFlag_Clear(FLAG_VENUS_AERIE_ON_PLATFORM);
    VenusLighthouseAerie_FloatPlatforms();
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    WaitFrames(6);
    Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);
    actor->speedLimit = PIXELS(3);
    actor->acceleration = PIXELS(2);
    Sound_PlaySong(SE_JUMP);
    actor->velocityY = PIXELS(4);
    actor->motionFlags &= 0x7e;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetPositionAndCommit(ACTOR_PARTY_LEADER,
                               destination.coordinates[0] >> 16,
                               destination.coordinates[2] >> 16);
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(actor, 1);
    actor->motionFlags = flags;
  }
}

/// Landing on a platform: it dips under the leader and springs back up.
/// FLAG_VENUS_AERIE_ON_PLATFORM is set while the leader stands on a platform,
/// so this runs once per landing.
static void VenusLighthouseAerie_LandOnPlatform(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  struct Actor *platform;
  s32 platformId;
  s32 reboundStep;

  Event_Start();
  platformId = Actor_FindNearestFloatingPlatform();
  gVenusLighthouseAerie_LandedPlatform = platformId;
  if (platformId != 0) {
    GameFlag_Set(FLAG_VENUS_AERIE_ON_PLATFORM);
    platform = Actor_GetById(gVenusLighthouseAerie_LandedPlatform);
    platform->motionFlags = 0;
    leader->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
    platform->y.fixed += -PIXELS(3);
    leader->y.fixed += -PIXELS(3);
    leader->groundHeight += -PIXELS(3);
    WaitFrames(2);
    platform->y.fixed += -PIXELS(2);
    leader->y.fixed += -PIXELS(2);
    leader->groundHeight += -PIXELS(2);
    WaitFrames(10);
    reboundStep = PIXELS(2);
    platform->y.fixed += reboundStep;
    leader->y.fixed += reboundStep;
    leader->groundHeight += reboundStep;
    WaitFrames(4);
    platform->y.fixed += reboundStep;
    leader->y.fixed += reboundStep;
    leader->groundHeight += reboundStep;
    WaitFrames(4);
    platform->y.fixed += PIXELS(1);
    leader->y.fixed += PIXELS(1);
    leader->groundHeight += PIXELS(1);
  }
  Event_End();
}

/// Stepping back onto the floor, and before each jump: the leader walks and
/// falls normally again, and all six platforms float, bobbing, as they do
/// until the leader lands on one (VenusLighthouseAerie_LandOnPlatform).
static void VenusLighthouseAerie_FloatPlatforms(void) {
  s32 flags;

  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  flags = ACTOR_FLOATS;
  Actor_GetById(ACTOR_FIRST_PLATFORM)->motionFlags = flags;
  Actor_GetById(ACTOR_FIRST_PLATFORM + 1)->motionFlags = flags;
  Actor_GetById(ACTOR_FIRST_PLATFORM + 2)->motionFlags = flags;
  Actor_GetById(ACTOR_FIRST_PLATFORM + 3)->motionFlags = flags;
  Actor_GetById(ACTOR_FIRST_PLATFORM + 4)->motionFlags = flags;
  Actor_GetById(ACTOR_LAST_PLATFORM)->motionFlags = flags;
}

/// Returns the platform and confrontation step triggers.
const struct Trigger *VenusLighthouseAerie_GetTriggers(void) {
  return gVenusLighthouseAerieTriggers;
}

/// Sets up the beacon's statues and the six platforms.
/// It then runs the part of the sequence the entrance selects (the
/// arrival only when not back from a battle), or, after a lost battle, gives
/// back the Shaman's Rod and leaves for Lalivero Sanctum. Back from a battle,
/// the platform nearest the leader stands still.
s32 VenusLighthouseAerie_Initialize(void) {
  s32 mirroredScale;
  u32 platformId;
  s32 clear;
  s16 entrance;
  struct Actor *platform;

  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  WaitFrames(1);
  GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_LIT);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_BEACON_STATUE_SOUTHWEST), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_BEACON_STATUE_NORTHWEST), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_BEACON_STATUE_NORTHEAST), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_BEACON_STATUE_SOUTHEAST), 0);
  mirroredScale = -Q16_ONE;
  Actor_GetById(ACTOR_BEACON_STATUE_NORTHEAST)->scaleX = mirroredScale;
  Actor_GetById(ACTOR_BEACON_STATUE_SOUTHEAST)->scaleX = mirroredScale;
  platformId = ACTOR_FIRST_PLATFORM;
  clear = 0;
  do {
    platform = Actor_GetById(platformId);
    Actor_SetSpriteFlags(Actor_GetById(platformId), clear);
    Actor_SetSpritePriority(platformId, 1);
    platform->motionFlags = ACTOR_FLOATS;
    platform->priorityFlags |= 2;
    platform->y.fixed = PIXELS(1) / 2;
    platformId++;
  } while (platformId <= ACTOR_LAST_PLATFORM);

  entrance = gSaveBlock.entrance;

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
    if (GameFlag_Get(FLAG_SHAMANS_ROD_HOLDER) != 0) {
      Inventory_AddItem(0, ITEM_SHAMANS_ROD);
    } else if (GameFlag_Get(FLAG_SHAMANS_ROD_HOLDER + 1) != 0) {
      Inventory_AddItem(1, ITEM_SHAMANS_ROD);
    } else if (GameFlag_Get(FLAG_SHAMANS_ROD_HOLDER + 2) != 0) {
      Inventory_AddItem(2, ITEM_SHAMANS_ROD);
    } else {
      Inventory_AddItem(3, ITEM_SHAMANS_ROD);
    }
    Import_EventSetExitRequest(VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_B);
    break;
  }

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    s32 slot = Actor_FindNearestFloatingPlatform();

    if (slot != 0) {
      struct Actor *nearestPlatform = Actor_GetById(slot);
      if (nearestPlatform != 0) {
        nearestPlatform->motionFlags = 0;
      }
    }
  }
  return clear;
}

static void VenusLighthouseAerie_ShowMessage(s32 speaker) {
  Event_ShowMessageAndWait(speaker, 0);
  Event_WaitFrames(10);
}

/// Turns one actor and waits ten frames.
static void VenusLighthouseAerie_FaceActor(s32 actorId, s32 facing) {
  Actor_ArmCallback(actorId, facing, 10);
}

/// Brings the party onto the aerie and reveals the argument over Sheba.
static void VenusLighthouseAerie_OverhearSaturosAndMenardi(void) {
  struct Actor *arrivalPlatform;
  struct Actor *leader;

  // Bring the leader across on the moving platform, then reveal the party.
  arrivalPlatform = Actor_GetById(ACTOR_RISING_PLATFORM);
  Event_Start();
  VenusLighthouseAerie_ShowPartyMember(1, 0);
  VenusLighthouseAerie_ShowPartyMember(2, 0);
  VenusLighthouseAerie_ShowPartyMember(3, 0);
  FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
  WaitFrames(1);
  arrivalPlatform->motionFlags = 0;
  arrivalPlatform->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_RISING_PLATFORM), 0);
  Actor_SetSpritePriority(ACTOR_RISING_PLATFORM, 1);
  Import_ActorSetPosition(ACTOR_RISING_PLATFORM, PIXELS(580), PIXELS(338));
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  leader->motionFlags = 0;
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
  Import_ActorSetPosition(ACTOR_PARTY_LEADER, PIXELS(581), PIXELS(288));
  WaitFrames(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(20);
  Actor_SetSpeed(ACTOR_RISING_PLATFORM, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_RISING_PLATFORM, 580, 221);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 581, 171);
  Actor_SetDestination(ACTOR_RISING_PLATFORM, 530, 211);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 531, 161);
  Actor_SetDestination(ACTOR_RISING_PLATFORM, 520, 191);
  Actor_SetPositionAndCommit(ACTOR_PARTY_LEADER, 521, 141);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_RISING_PLATFORM), 1);
  Actor_SetDestination(ACTOR_RISING_PLATFORM, 515, 171);
  Actor_SetPositionAndCommit(ACTOR_PARTY_LEADER, 516, 121);
  Sound_PlaySong(SE_STOP_ACTION_SE);
  Event_WaitFrames(20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_SPRING);
  Actor_WaitForAnimationChange(ACTOR_PARTY_LEADER);
  leader->x.fixed = PIXELS(516);
  leader->y.fixed = PIXELS(8);
  leader->z.fixed = PIXELS(148);
  leader->facing = FACING_WEST;
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Sound_PlaySong(SE_JUMP);
  leader->velocityY = PIXELS(4);
  Sound_PlaySong(SE_JUMP);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PARTY_LEADER), 1);
  Actor_SetPositionAndCommit(ACTOR_PARTY_LEADER, 504, 148);
  Event_WaitFrames(10);
  Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  {
    u16 shown = FACING_SOUTH;

    leader->y.fixed = -PIXELS(32);
    leader->facing = shown;
  }
  Event_WaitFrames(20);
  Sound_PlaySong(SE_MATERIALIZE);
  Actor_SetPositionAndCommit(ACTOR_RISING_PLATFORM, 524, 191);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_RISING_PLATFORM), 0);
  Actor_MoveToAndWait(ACTOR_RISING_PLATFORM, 530, 211);
  Actor_MoveToAndWait(ACTOR_RISING_PLATFORM, 580, 221);
  Actor_ResetAndSetPosition(ACTOR_RISING_PLATFORM, 580, 338);
  Actor_GetById(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  VenusLighthouseAerie_ShowPartyMember(1, 1);
  VenusLighthouseAerie_ShowPartyMember(2, 1);
  VenusLighthouseAerie_ShowPartyMember(3, 1);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 492, 164);
  Actor_ResetAndSetPositionInMode2(ACTOR_GARET, 514, 164);
  Actor_WalkTo(ACTOR_IVAN, 492, 140);
  Actor_SetPositionAndReset(ACTOR_MIA, 514, 140);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_ArmCallback(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_ArmCallback(ACTOR_IVAN, FACING_SOUTH, 0);
  Actor_ArmCallback(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_WaitForMoveById(ACTOR_RISING_PLATFORM);
  Import_ActorSetPosition(ACTOR_RISING_PLATFORM, 0, 0);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  // Reveal Saturos and Menardi arguing with Felix about Sheba.
  Event_SetMessageId(MsgVenusLongLastTime);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MENARDI);
  Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_ArmCallback(ACTOR_GARET, FACING_WEST, 0);
  Actor_ArmCallback(ACTOR_IVAN, FACING_WEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_WEST);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeedParameters(PIXELS(24) / 5, PIXELS(3) / 5);
  FieldCamera_MoveWithinMapBounds(PIXELS(304), PIXELS(32), PIXELS(158), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(20);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH + FACING_STEP);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Sound_PlaySong(MUS_SATUROS_AND_MENARDI);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Import_ActorSetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MENARDI);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldEffect_SpawnAttached(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                            20);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 20);
  FieldCamera_SetSpeedParameters(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(294), -1, PIXELS(194), 1);
  Event_WaitFrames(20);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_FELIX, 272, 200);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_WaitFrames(20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 40);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_WEST);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ArmCallback(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ArmCallback(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 40);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTH + FACING_STEP);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Event_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  Actor_ArmCallback(ACTOR_SATUROS, FACING_WEST, 40);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 40);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  Actor_ArmCallback(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 20);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_StartRepeatedMotion(ACTOR_MENARDI, 1);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_NORTHWEST + FACING_STEP);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  VenusLighthouseAerie_FaceActor(ACTOR_SHEBA, FACING_SOUTHEAST + FACING_STEP);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_WaitFrames(20);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_NORTH + FACING_STEP);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ArmCallback(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_ArmCallback(ACTOR_MENARDI, FACING_WEST, 0);
  Actor_ArmCallback(ACTOR_SATUROS, FACING_WEST, 0);
  FieldCamera_SetSpeedParameters(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveTo(PIXELS(294), -1, PIXELS(180), 1);
  Actor_WalkToAndWait(ACTOR_FELIX, 262, 176);
  Actor_ArmCallback(ACTOR_FELIX, FACING_WEST, 40);
  Actor_ArmCallback(ACTOR_FELIX, FACING_EAST, 20);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FELIX);
  FieldEffect_SpawnAttached(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                            20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 20);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FELIX);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Actor_ArmCallback(ACTOR_FELIX, FACING_WEST, 20);
  Event_ShowMessageThenWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 40);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_ArmCallback(ACTOR_FELIX, FACING_EAST, 40);
  Import_ActorSetAnimation(ACTOR_MENARDI, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_FELIX, 0, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  FieldEffect_SpawnAttached(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                            40);
  Import_ActorSetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  FieldEffect_SpawnAttached(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  FieldEffect_SpawnAttached(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                            20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_WaitFrames(20);
  Actor_StartRepeatedMotion(ACTOR_SATUROS, 2);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Import_ActorSetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveWithinMapBounds(PIXELS(504), -PIXELS(24), PIXELS(168), 0);
  WaitFrames(1);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GARET);
  FieldEffect_SpawnAttached(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0, 40);
  Actor_Jump(ACTOR_GARET, 2, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GARET);
  FieldMessage_ShowAtActor(SPEAKER_WINDOW_BELOW | ACTOR_GARET, 0);
  Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_ArmCallback(ACTOR_IVAN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    gFieldWork->message += 1;
  }
  Event_WaitFrames(20);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_ArmCallback(ACTOR_IVAN, FACING_EAST, 0);
  Actor_ArmCallback(ACTOR_MIA, FACING_WEST, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Actor_SetScript(ACTOR_GARET, gVenusLighthouseAerie_JoinLeaderAction);
  Actor_SetScript(ACTOR_IVAN, gVenusLighthouseAerie_JoinLeaderAction);
  Actor_RunScriptAndWait(ACTOR_MIA, gVenusLighthouseAerie_JoinLeaderAction);
  Event_WaitFrames(20);
  Event_End();
}

/// Shows or hides one companion, setting its movement speed when shown.
static void VenusLighthouseAerie_ShowPartyMember(s32 actorId, s32 shown) {
  struct Actor *member;

  if (shown != 0) {
    Actor_SetSpriteColorById(actorId, SPRITE_COLOR_NORMAL);
    member = Actor_Get(actorId);
    Actor_SetSpriteFlags(member, 1);
    Actor_SetSpeed(actorId, PIXELS(4) / 5, PIXELS(2) / 5);
  } else {
    Actor_SetSpriteColorById(actorId, SPRITE_COLOR_HIDDEN);
    member = Actor_Get(actorId);
    Actor_SetSpriteFlags(member, 0);
  }
}

/// Walking up to Saturos and Menardi (MB_VENUS_LIGHTHOUSE_AERIE_BEFORE_BEACON):
/// Felix refuses to be rescued, Sheba reads the party's minds for the rod that
/// reaches Jupiter Lighthouse, they promise not to harm her for the Shaman's
/// Rod, Felix carries it over, they will not let her go, and the first battle
/// begins. It sets FLAG_SHAMANS_ROD_HOLDER plus the member who carried the rod,
/// and FLAG_AERIE_FIRST_BATTLE_BEGUN as the battle starts, which changes the
/// aerie's music.
static void VenusLighthouseAerie_ConfrontSaturosAndMenardi(void) {
  s32 rodHolder;
  struct Actor *actor;
  s32 answerAccepted;
  s32 frame;
  s32 receiptMessage;

  Event_Start();
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(332), PIXELS(32), PIXELS(180), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 340, 184);
  VenusLighthouseAerie_FaceActor(ACTOR_PARTY_LEADER, FACING_WEST);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_SetMessageId(MsgVenusIsaac);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_LEFT | ACTOR_FELIX);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 1);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(7) / 5, PIXELS(7) / 10);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
  }
  Actor_WalkTo(ACTOR_GARET, 328, 168);
  Actor_WalkTo(ACTOR_IVAN, 340, 196);
  Actor_WalkToAndWait(ACTOR_MIA, 326, 204);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_ArmCallback(ACTOR_GARET, FACING_WEST, 0);
  Actor_ArmCallback(ACTOR_IVAN, FACING_WEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_WEST);
  Actor_ArmCallback(ACTOR_SATUROS, FACING_EAST, 0);
  Actor_ArmCallback(ACTOR_MENARDI, FACING_EAST, 40);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_WEST);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  Actor_ArmCallback(ACTOR_FELIX, FACING_NORTH + FACING_STEP, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_EAST);
  FieldEvent_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_WEST);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Actor_ArmCallback(ACTOR_SATUROS, FACING_EAST, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  Actor_ArmCallback(ACTOR_FELIX, FACING_WEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_ArmCallback(ACTOR_SATUROS, FACING_WEST, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_ArmCallback(ACTOR_MENARDI, FACING_WEST, 0);
  Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_ArmCallback(ACTOR_GARET, FACING_WEST, 0);
  Actor_ArmCallback(ACTOR_IVAN, FACING_WEST, 0);
  Actor_ArmCallback(ACTOR_MIA, FACING_WEST, 0);
  Actor_ArmCallback(ACTOR_FELIX, FACING_EAST, 20);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_SHEBA);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_WaitFrames(20);
  Import_ActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(ACTOR_SHEBA);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetSpeed(ACTOR_SHEBA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_SHEBA, 260, 186);
  Actor_ArmCallback(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WalkToAndWait(ACTOR_SHEBA, 276, 192);
  actor = Actor_GetById(ACTOR_MENARDI);
  actor->facing = FACING_SOUTH + FACING_STEP;
  WaitFrames(1);
  Actor_StartRepeatedMotion(ACTOR_MENARDI, 2);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_Jump(ACTOR_SHEBA, 2, 20);
  Actor_SetSpeed(ACTOR_SHEBA, PIXELS(12) / 5, PIXELS(6) / 5);
  Actor_WalkToAndWait(ACTOR_SHEBA, 260, 186);
  Actor_ArmCallback(ACTOR_FELIX, FACING_WEST, 0);
  Actor_WalkToAndWait(ACTOR_SHEBA, 248, 172);
  Actor_ArmCallback(ACTOR_MENARDI, FACING_WEST, 0);
  Actor_ArmCallback(ACTOR_FELIX, FACING_WEST, 0);
  Actor_ArmCallback(ACTOR_SHEBA, FACING_EAST, 20);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_WaitFrames(40);
  Scene_StartEventPsynergy();
  WaitFrames(1);
  frame = 0;
  do {
    actor = Actor_GetById(ACTOR_SHEBA);
    VenusLighthouseAerie_FlashActorPalette(actor);
    frame = (frame + 1);
    WaitFrames(1);
  } while ((u32)frame <= 39);
  Task_AddFrame(VenusLighthouseAerie_UpdateShebaPalette, TASK_PASS_FRAME);
  Event_WaitFrames(80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
  Actor_ArmCallback(ACTOR_FELIX, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_WEST);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 20);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Actor_StartRepeatedMotion(ACTOR_MENARDI, 2);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Task_Remove(VenusLighthouseAerie_UpdateShebaPalette);
  WaitFrames(1);
  Actor_SetSpriteColorById(ACTOR_SHEBA, SPRITE_COLOR_NORMAL);
  WaitFrames(10);
  Scene_EndEventPsynergy();
  Actor_Jump(ACTOR_SHEBA, 2, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_SHEBA);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Actor_GetById(ACTOR_SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_SHEBA, 250, 176);
  Event_WaitFrames(1);
  Actor_GetById(ACTOR_SHEBA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_ArmCallback(ACTOR_FELIX, FACING_EAST, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_WaitFrames(40);
  Actor_SetSpeed(ACTOR_SHEBA, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_GetById(ACTOR_SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_SHEBA, 248, 172);
  Event_WaitFrames(1);
  Actor_GetById(ACTOR_SHEBA)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(ACTOR_SHEBA);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Import_ActorSetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 100);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_WaitFrames(20);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_EAST);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  Event_OpenMessage(ACTOR_SATUROS, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
    answerAccepted = 1;
  } else {
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
    gFieldWork->message += 1;
    answerAccepted = 0;
  }
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  if (answerAccepted != 0) {
    gFieldWork->message += 1;
  }
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Event_OpenMessage(ACTOR_SATUROS, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_WaitFrames(20);
    Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    answerAccepted = 1;
  } else {
    Event_WaitFrames(20);
    Import_ActorSetAnimation(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    gFieldWork->message += 1;
    answerAccepted = 0;
  }
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  if (answerAccepted != 0) {
    gFieldWork->message += 1;
  }
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTH);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP);
  Import_ActorSetAnimation(ACTOR_MENARDI, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  Event_WaitFrames(40);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_WEST);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_FELIX, 0, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_WEST);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_FELIX, 288, 192);
  Actor_ArmCallback(ACTOR_MENARDI, FACING_EAST, 0);
  Actor_ArmCallback(ACTOR_SATUROS, FACING_EAST, 0);
  Actor_WalkToAndWait(ACTOR_FELIX, 310, 192);
  Actor_WalkToAndWait(ACTOR_FELIX, 328, 186);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);

  // Complete the Shaman's Rod exchange before the confrontation resumes.
  receiptMessage = MsgVenusIsaacHandedOverShamansRod;
  FieldMessage_Show(receiptMessage, 1);
  Actor_WalkToAndWait(ACTOR_FELIX, 310, 192);
  Actor_ArmCallback(ACTOR_MENARDI, FACING_WEST, 0);
  Actor_ArmCallback(ACTOR_SATUROS, FACING_WEST, 0);
  Actor_WalkToAndWait(ACTOR_FELIX, 288, 192);
  Actor_WalkToAndWait(ACTOR_FELIX, 262, 176);
  Actor_ArmCallback(ACTOR_FELIX, FACING_EAST, 40);
  Event_SetMessageId((receiptMessage + 1));
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_EAST);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetSpeed(ACTOR_SHEBA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_SHEBA, 260, 186);
  Actor_ArmCallback(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WalkToAndWait(ACTOR_SHEBA, 276, 192);
  Event_PlaySong(MUS_SILENCE);
  actor = Actor_GetById(ACTOR_MENARDI);
  actor->facing = FACING_SOUTH + FACING_STEP;
  WaitFrames(1);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Actor_ArmCallback(ACTOR_FELIX, FACING_EAST, 0);
  Actor_ArmCallback(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH + FACING_STEP, 20);
  Actor_Jump(ACTOR_MIA, 2, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_NORTHWEST);
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_ArmCallback(ACTOR_FELIX, FACING_EAST, 0);
  Actor_ArmCallback(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_ArmCallback(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 80);
  Actor_ArmCallback(ACTOR_MENARDI, FACING_EAST, 0);
  Actor_ArmCallback(ACTOR_SATUROS, FACING_EAST, 40);
  Event_PlaySong(MUS_CONFRONTATION);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_ArmCallback(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_ArmCallback(ACTOR_FELIX, FACING_EAST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_SHEBA, FACING_NORTH + FACING_STEP);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_ArmCallback(ACTOR_SATUROS, FACING_SOUTHEAST + FACING_STEP, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_ArmCallback(ACTOR_SHEBA, FACING_EAST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_EAST);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_EAST);
  Import_ActorSetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_ArmCallback(ACTOR_SHEBA, FACING_NORTH + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP);
  Import_ActorSetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 40);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_NORTHWEST);
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_ArmCallback(ACTOR_GARET, FACING_WEST, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_ArmCallback(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_ArmCallback(ACTOR_IVAN, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_StartRepeatedMotion(ACTOR_SATUROS, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  // Save the rod owner for the defeat return path, then enter battle.
  rodHolder = PartyInventory_FindMember(ITEM_SHAMANS_ROD);
  GameFlag_Set(rodHolder + FLAG_SHAMANS_ROD_HOLDER);
  PartyInventory_Remove(ITEM_SHAMANS_ROD);
  Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_IVAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_MIA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_MENARDI)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_SATUROS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetScript(ACTOR_PARTY_LEADER, gVenusLighthouseAerie_StepBackWestAction);
  Actor_SetScript(ACTOR_GARET, gVenusLighthouseAerie_StepBackWestAction);
  Actor_SetScript(ACTOR_IVAN, gVenusLighthouseAerie_StepBackWestAction);
  Actor_SetScript(ACTOR_MIA, gVenusLighthouseAerie_StepBackWestAction);
  Actor_SetScript(ACTOR_MENARDI, gVenusLighthouseAerie_StepBackEastAction);
  Actor_SetScript(ACTOR_SATUROS, gVenusLighthouseAerie_StepBackEastAction);
  Actor_SetScript(ACTOR_FELIX, gVenusLighthouseAerie_StepBackWestAction);
  Actor_RunScriptAndWait(ACTOR_SHEBA, gVenusLighthouseAerie_StepBackWestAction);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  {
    s32 actorId = (s32)&SceneId_VenusLighthouseAerie;

    Event_SetAfterBattleDestination(actorId,
                                    VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE);
    Event_SetDefeatDestination(actorId, VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT);
  }
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 1);
  GameFlag_Set(FLAG_AERIE_FIRST_BATTLE_BEGUN);
}

/// Flashes an actor palette and emits paired effects every fifteenth
/// frame.
static void VenusLighthouseAerie_FlashActorPalette(struct Actor *actor) {
  if (*(s32 *)&gFrameCount & 2) {
    Actor_SetColorMode(actor, SPRITE_COLOR_WHITE);
  } else {
    Actor_SetColorMode(actor, SPRITE_COLOR_NORMAL);
  }
  if (Math_RemainderUnsigned(*(s32 *)&gFrameCount, 15) == 0) {
    VenusLighthouseAerie_SpawnLinkedPairEffects(actor);
  }
}

/// Cycles the actor palette and emits paired effects every fifteenth
/// frame.
static void VenusLighthouseAerie_CycleActorPalette(struct Actor *actor) {
  if (gFrameCount & 1) {
    Actor_SetColorMode(actor, Math_RemainderUnsigned(gFrameCount >> 1, 6));
  }
  if (Math_RemainderUnsigned(gFrameCount, 15) == 0) {
    VenusLighthouseAerie_SpawnLinkedPairEffects(actor);
  }
}

#include "common/effect_vertical.h"

/// Spawns two vertical effects using the parent position and shared cached
/// tiles.
static void VenusLighthouseAerie_SpawnLinkedPairEffects(struct Actor *parent) {
  struct Actor *pair[2];
  struct Actor *child;
  struct Sprite *sprite;
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  s32 effectIndex;

  Sound_PlaySong(SE_PSYNERGY_SPARKLE);
  for (effectIndex = 0; effectIndex < 2; ++effectIndex) {
    child = FieldActor_Create(SPRITE_GOLDEN_RINGS, parent->x.fixed,
                              parent->y.fixed, parent->z.fixed);
    pair[effectIndex] = child;
    if (child != NULL) {
      child->groundHeight = parent->groundHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->linkedActor = parent;
      if (sprite != NULL) {
        Sprite_StartAnim(sprite, 0);
        sprite->flags = 0;
        VramCache_FreeEntry(sprite->slot);
        sprite->slot = effectWork->tileSlot;
        sprite->retainCacheEntry = 1;
        sprite->image.tile =
            (gVramCacheEntries[sprite->slot].offset >> 5) & 0x3ff;
        sprite->image.fullColor = 0;
        sprite->image.shape = 1;
        sprite->image.size = 2;
        sprite->entries[0]->frame = 0;
      }
    }
  }
  {
    struct Actor *effect = pair[0];
    struct Sprite *effectSprite = effect->sprite;

    effect->update = FieldEffect_AnimateVerticalNegative;
    effectSprite->image.priority = parent->sprite->image.priority;
  }
  {
    struct Actor *effect = pair[1];
    struct Sprite *effectSprite = effect->sprite;

    effectSprite->image.priority = parent->sprite->image.priority;
    effect->update = FieldEffect_AnimateVerticalPositive;
    effect->priorityFlags = 2;
  }
}

#include "common/effect_psynergy.h"

static void VenusLighthouseAerie_UpdateShebaPalette(void) {
  VenusLighthouseAerie_CycleActorPalette(Actor_GetById(ACTOR_SHEBA));
}

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

/// Back from the first battle (VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE):
/// Felix will still have the beacons lit, Saturos throws the Venus Star into
/// the lighthouse, its light restores Saturos and Menardi, Felix leaves with
/// Sheba, and the two fuse. Refills the PP of the party's Adepts whose
/// lighthouses are lit, starts the second battle and sets
/// FLAG_AERIE_SECOND_BATTLE_BEGUN, which changes the aerie's music.
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
  u8 *iconPixels;
  // Reuse the dialogue answer word for the later radial burst directions.
  u32 answeredYes;

  Event_Start();
  MapGrid_CopyCellAttributes(17, 10, 4, 2, 17, 8);
  Actor_Get(ACTOR_GARET)->facing = FACING_WEST;
  Actor_SetPosition(ACTOR_GARET, PIXELS(328), PIXELS(168));
  Actor_Get(ACTOR_IVAN)->facing = FACING_WEST;
  Actor_SetPosition(ACTOR_IVAN, PIXELS(340), PIXELS(196));
  Actor_Get(ACTOR_MIA)->facing = FACING_WEST;
  Actor_SetPosition(ACTOR_MIA, PIXELS(326), PIXELS(204));
  Actor_Get(ACTOR_SHEBA)->facing = FACING_SOUTHEAST + FACING_STEP;
  Actor_SetPosition(ACTOR_SHEBA, PIXELS(268), PIXELS(154));
  Actor_Get(ACTOR_FELIX)->facing = FACING_SOUTHEAST + FACING_STEP;
  Actor_SetPosition(ACTOR_FELIX, PIXELS(268), PIXELS(164));
  actor = Actor_Get(ACTOR_SATUROS);
  actor->timer = 10;
  actor->facing = FACING_NORTH + FACING_STEP;
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(294), PIXELS(212));
  Actor_SetAnimation(ACTOR_SATUROS, SATUROS_ANIMATION_LIE);
  Actor_SetScript(ACTOR_SATUROS, gVenusLighthouseAerie_ScaleStepAction);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SATUROS), 0);
  actor = Actor_Get(ACTOR_MENARDI);
  actor->timer = 10;
  actor->facing = FACING_EAST;
  Actor_SetPosition(ACTOR_MENARDI, PIXELS(286), PIXELS(192));
  Actor_SetAnimation(ACTOR_MENARDI, MENARDI_ANIMATION_LIE);
  Actor_SetScript(ACTOR_MENARDI, gVenusLighthouseAerie_ScaleStepAction);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MENARDI), 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 0);
  WaitFrames(1);
  MapGrid_Redraw();
  WaitFrames(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(80);

  Actor_Jump(ACTOR_GARET, 2, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTHEAST);
  Event_SetMessageId(MsgVenusBeatEm);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_NORTHWEST);
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_IVAN, FACING_NORTHWEST);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_Get(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_FELIX, 280, 164);
  Event_WaitFrames(1);
  Actor_Get(ACTOR_FELIX)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_EAST);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_Jump(ACTOR_IVAN, 2, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTHEAST);
  Event_OpenMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    answeredYes = TRUE;
  } else {
    gFieldWork->message++;
    answeredYes = FALSE;
  }
  WaitFrames(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_NORTHWEST);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_EAST);
  Event_ShowMessage(ACTOR_FELIX, 0);
  Event_PlaySong(SONG_FADE_OUT_BGM);
  Event_WaitFrames(40);
  if (answeredYes) {
    gFieldWork->message++;
  }

  // Saturos and Menardi recover from their defeat before using the Venus Star.
  Actor_Stop(ACTOR_SATUROS);
  Actor_Stop(ACTOR_MENARDI);
  WaitFrames(1);
  actor = Actor_Get(ACTOR_SATUROS);
  actor->scaleX = Q16_ONE;
  actor->scaleY = Q16_ONE;
  actor = Actor_Get(ACTOR_MENARDI);
  actor->scaleX = Q16_ONE;
  actor->scaleY = Q16_ONE;
  WaitFrames(1);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 40);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SATUROS), 1);
  Event_WaitFrames(20);
  Actor_SetSpeed(ACTOR_SATUROS, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_WalkToAndWait(ACTOR_SATUROS, 300, 206);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_WaitFrames(20);
  Actor_Get(ACTOR_SATUROS)->scaleX = -Q16_ONE;
  Event_PlaySong(SE_RATTLE);
  Actor_SetAnimation(ACTOR_SATUROS, SATUROS_ANIMATION_BREATHE);
  Event_WaitFrames(20);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MENARDI), 1);
  Actor_Jump(ACTOR_MENARDI, 4, 40);
  Actor_SetSpeed(ACTOR_MENARDI, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_Get(ACTOR_MENARDI)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_MENARDI, 296, 186);
  Actor_SetSpeed(ACTOR_MENARDI, PIXELS(1) / 10, PIXELS(1) / 20);
  Actor_WalkToAndWait(ACTOR_MENARDI, 292, 186);
  Actor_Get(ACTOR_MENARDI)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_MENARDI)->scaleX = -Q16_ONE;
  Event_PlaySong(SE_RATTLE);
  Actor_SetAnimation(ACTOR_MENARDI, MENARDI_ANIMATION_STAND_WINDSWEPT);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_WaitFrames(80);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
  Event_WaitFrames(40);
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
  actor->facing = FACING_SOUTHEAST + FACING_STEP;
  actor->scaleX = Q16_ONE;
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_STAND);
  Event_WaitFrames(10);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH + FACING_STEP);
  Actor_Jump(ACTOR_SATUROS, 6, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  star = (struct Actor *)FieldActor_Create(
      SPRITE_BLANK, actor->x.fixed, actor->y.fixed + PIXELS(8), actor->z.fixed);
  if (star != NULL) {
    sprite = star->sprite;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    sprite->image.priority = 0;
    star->priorityFlags &= ~1;
    star->motionFlags = 0;
    star->retainSpriteCache = 1;
    star->speedLimit = PIXELS(8) / 5;
    star->acceleration = 0xcccc;
    iconPixels = Heap_Allocate(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
    // Display the Venus Star icon before sending it toward the beacon.
    Item_LoadIcon(ITEM_VENUS_STAR);
    Vram_Load(sprite->slot, 128, iconPixels + 0x400);
    Heap_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
  Actor_SetSpritePriority(ACTOR_VENUS_STAR_SHADOW, 1);
  starShadow = Actor_Get(ACTOR_VENUS_STAR_SHADOW);
  starShadow->x.fixed = actor->x.fixed;
  starShadow->y.fixed = PIXELS(32);
  starShadow->z.fixed = actor->z.fixed;
  starShadow->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  starShadow->speedLimit = PIXELS(8) / 5;
  starShadow->acceleration = PIXELS(4) / 5;
  starShadow->scaleX = 3 * Q16_ONE / 4;
  starShadow->scaleY = 3 * Q16_ONE / 4;
  if (star != NULL) {
    star->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    ((struct FieldEffect *)star)->velocity.y = PIXELS(3) / 5;
    ((struct FieldEffect *)star)->velocity.x = PIXELS(4) / 5;
    star->velocityY = PIXELS(8);
    FieldActor_SetMoveTarget(star, PIXELS(308), PIXELS(32), PIXELS(164));
  }
  Actor_MoveToAndWait(ACTOR_VENUS_STAR_SHADOW, 308, 164);
  Actor_SetPosition(ACTOR_VENUS_STAR_SHADOW, 0, 0);
  Actor_SetSpritePriority(ACTOR_FELIX, 0);
  if (star != NULL) {
    Event_PlaySong(SE_STAR_BOUNCE);
    Actor_SetSpriteFlags(star, 0);
    star->velocityY = PIXELS(4);
    FieldActor_SetMoveTarget(star, PIXELS(314), PIXELS(32), PIXELS(137));
    Actor_WaitForMove(star);
    Event_PlaySong(SE_STAR_BOUNCE);
    star->sprite->image.priority = 1;
    star->velocityY = PIXELS(6);
    FieldActor_SetMoveTarget(star, PIXELS(285), PIXELS(32), PIXELS(146));
    Actor_WaitForMove(star);
    Event_PlaySong(SE_STAR_BOUNCE);
    star->velocityY = PIXELS(5);
    FieldActor_SetMoveTarget(star, PIXELS(300), PIXELS(32), PIXELS(154));
    Actor_WaitForMove(star);
    WaitFrames(6);
    star->x.fixed = 0;
    star->y.fixed = 0;
    star->z.fixed = 0;
    VenusLighthouseAerie_StopActorMotion(star);
  }
  // Let the group react as the Venus Star reaches the beacon.
  VenusLighthouseAerie_ShowGroupEmote(EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_SetSpritePriority(ACTOR_FELIX, 1);
  Actor_Get(ACTOR_FELIX)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_WaitFrames(20);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST + FACING_STEP, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP);
  VenusLighthouseAerie_ShowGroupEmote(EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 40);
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    answeredYes = TRUE;
  } else {
    gFieldWork->message++;
    Actor_SetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
    answeredYes = FALSE;
  }
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  if (answeredYes) {
    gFieldWork->message++;
  }
  Event_WaitFrames(20);

  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEAM_BOTTOM), 0);
  Actor_SetSpriteColorById(ACTOR_BEAM_BOTTOM, SPRITE_COLOR_WHITE);
  actor = Actor_Get(ACTOR_BEAM_BOTTOM);
  actor->scaleY = -Q16_ONE;
  actor->scaleX = Q16_ONE / 10;
  actor->motionFlags = 0;
  actor->y.fixed = PIXELS(64);
  actor->x.fixed = PIXELS(304);
  actor->z.fixed = PIXELS(158);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEAM_LOWER), 0);
  Actor_SetSpriteColorById(ACTOR_BEAM_LOWER, SPRITE_COLOR_WHITE);
  actor = Actor_Get(ACTOR_BEAM_LOWER);
  actor->scaleY = -Q16_ONE;
  actor->scaleX = Q16_ONE / 10;
  actor->motionFlags = 0;
  actor->y.fixed = PIXELS(96);
  actor->x.fixed = PIXELS(304);
  actor->z.fixed = PIXELS(158);
  VenusLighthouseAerie_ShowGroupEmote(EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_SetScript(ACTOR_BEAM_BOTTOM, gVenusLighthouseAerie_BeamOpenAction);
  Actor_SetScript(ACTOR_BEAM_LOWER, gVenusLighthouseAerie_BeamOpenAction);
  Event_PlaySong(SE_CRASH);
  MapGrid_SetShake(PIXELS(6), PIXELS(6), Q16_ONE);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
  PaletteFade_Interpolate(16);
  WaitFrames(20);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(24);
  WaitFrames(60);
  Task_AddFrame(VenusLighthouseAerie_SpawnUpperOrbitParticles, TASK_PASS_FRAME);
  Event_PlaySong(SE_RUMBLE);
  MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
  PaletteFade_Interpolate(120);
  Actor_SetScript(ACTOR_BEAM_BOTTOM, gVenusLighthouseAerie_BeamNarrowAction);
  Actor_SetScript(ACTOR_BEAM_LOWER, gVenusLighthouseAerie_BeamNarrowAction);
  WaitFrames(120);
  MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 16, 12), PALETTES_ALL);
  PaletteFade_Interpolate(120);
  WaitFrames(120);
  Event_PlaySong(MUS_ERUPTION);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_Interpolate(120);
  WaitFrames(120);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);

  Actor_SetAnimation(ACTOR_MENARDI, ANIM_STAND);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 0);
  Actor_Get(ACTOR_MENARDI)->scaleX = Q16_ONE;
  Actor_Jump(ACTOR_MENARDI, 4, 40);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  FieldEvent_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_WEST);
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_StartRepeatedMotion(ACTOR_MENARDI, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTHWEST);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH + FACING_STEP);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
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
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 40);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST + FACING_STEP, 60);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH + FACING_STEP, 40);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 80);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_WaitFrames(20);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST + FACING_STEP, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX);
  Actor_SetSpeed(ACTOR_SHEBA, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(1), PIXELS(1) / 2);
  Actor_SetScript(ACTOR_FELIX, gVenusLighthouseAerie_WalkOutAction);
  Event_WaitFrames(20);
  Actor_SetScript(ACTOR_SHEBA, gVenusLighthouseAerie_WalkOutAction);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTHWEST);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Event_PlaySong(SONG_FADE_OUT_BGM);
  // Flash Saturos and Menardi and release their matching radial bursts.
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(13, 8, 9), PALETTES_BG);
  PaletteFade_Interpolate(40);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH + FACING_STEP);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  VenusLighthouseAerie_FlashScreen();
  Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_WHITE);
  Actor_SetSpriteColorById(ACTOR_MENARDI, SPRITE_COLOR_WHITE);
  Event_WaitFrames(20);
  Actor_SetSpriteColorById(ACTOR_SATUROS, CHILD_VALUE_BLINK);
  Actor_SetSpriteColorById(ACTOR_MENARDI, CHILD_VALUE_BLINK);
  Event_WaitFrames(20);
  VenusLighthouseAerie_FlashScreen();
  Actor_Jump(ACTOR_MIA, 2, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  VenusLighthouseAerie_FlashScreen();

  menardi = Actor_Get(ACTOR_MENARDI);
  menardiBurst.palette = 7;
  menardiBurst.update = VenusLighthouseAerie_UpdateDriftingParticle;
  menardiBurst.startScaleX = Q16_ONE;
  menardiBurst.startScaleY = Q16_ONE;
  for (answeredYes = 0; answeredYes <= 16; answeredYes++) {
    VenusLighthouseAerie_AimBurst(velocity, answeredYes << 12);
    FieldEffect_Spawn(menardi->x.fixed, menardi->y.fixed, menardi->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE,
                      &menardiBurst);
  }
  Event_PlaySong(SE_POWER_UP);
  WaitFrames(6);
  VenusLighthouseAerie_FlashScreen();
  saturos = Actor_Get(ACTOR_SATUROS);
  saturosBurst.palette = 7;
  saturosBurst.update = VenusLighthouseAerie_UpdateDriftingParticle;
  saturosBurst.startScaleX = Q16_ONE;
  saturosBurst.startScaleY = Q16_ONE;
  for (answeredYes = 0; answeredYes <= 16; answeredYes++) {
    VenusLighthouseAerie_AimBurst(velocity, answeredYes << 12);
    FieldEffect_Spawn(saturos->x.fixed, saturos->y.fixed, saturos->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE,
                      &saturosBurst);
  }
  Event_PlaySong(SE_POWER_UP);
  Actor_Jump(ACTOR_IVAN, 6, 20);
  Event_PlaySong(MUS_FUSION_DRAGON_BATTLE);
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  VenusLighthouseAerie_FlashScreen();
  Actor_SetSpeed(ACTOR_SATUROS, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_SetSpeed(ACTOR_MENARDI, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_Get(ACTOR_SATUROS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_MENARDI)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_SATUROS, 294, 196);
  Actor_SetDestination(ACTOR_MENARDI, 294, 196);
  // Slide the two transformed actors together, then start the fusion
  // battle.
  Task_AddFrame(VenusLighthouseAerie_UpdateBeamActors, TASK_PASS_FRAME);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Event_ShowMessage(ACTOR_GARET, 0);
  GameFlag_Set(FLAG_VENUS_AERIE_FUSION_SLIDING);
  Event_ShowMessage(ACTOR_IVAN, 0);
  GameFlag_Set(FLAG_VENUS_AERIE_FUSION_GROWING);
  VenusLighthouseAerie_FlashScreen();
  Event_WaitFrames(20);
  VenusLighthouseAerie_FlashScreen();
  Event_WaitFrames(20);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination((s32)&SceneId_VenusLighthouseAerie,
                                  VENUS_LIGHTHOUSE_AERIE_AFTER_SECOND_BATTLE);
  Event_SetDefeatDestination((s32)&SceneId_VenusLighthouseAerie,
                             VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 0);
  Party_RestorePpOfLitElements();
  GameFlag_Set(FLAG_AERIE_SECOND_BATTLE_BEGUN);
}

/// Flashes the object palette white, then restores the transformation colour.
static void VenusLighthouseAerie_FlashScreen(void) {
  Sound_PlaySong(SE_FALLING_LIGHT);
  PaletteFade_SetTarget(RGB(31, 31, 31), PALETTES_BG);
  PaletteFade_StartInterpolation(1);
  WaitFrames(4);
  PaletteFade_SetTarget(PALETTE_TINT_BLEND | RGB(13, 8, 9), PALETTES_BG);
  PaletteFade_StartInterpolation(1);
  WaitFrames(1);
}

/// Orbits a particle around the upper beam and keeps its target at that
/// position.
static void VenusLighthouseAerie_UpdateUpperOrbit(struct Actor *actor) {
  struct Actor *effect = actor;
  struct Actor *center = Actor_GetById(ACTOR_BEAM_BOTTOM);
  u16 angle = effect->timer;

  effect->x.fixed =
      center->x.fixed + Trig_Cos(angle) * (effect->speedLimit + 3);
  effect->z.fixed = center->z.fixed + Trig_Sin(angle) * 2;
  effect->targetX = effect->x.fixed;
  effect->targetZ = effect->z.fixed;
  effect->timer -= 0x800;
}

/// Orbits a particle around the rising beam using its rise counter as a radius.
static void VenusLighthouseAerie_UpdateRisingOrbit(struct Actor *updated) {
  struct Actor *actor = updated;
  struct Actor *anchor = Actor_GetById(ACTOR_BEACON_ORB);
  s32 angle = (u16)actor->timer;
  s32 cosine;
  s32 sine;
  s32 along;
  s32 across;

  cosine = Trig_Cos(angle);
  along =
      anchor->x.fixed + cosine * (actor->speedLimit + actor->riseCounter + 6);
  actor->x.fixed = along;

  sine = Trig_Sin(angle);
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

/// Emits rising particles around the upper beam at the current beacon stage.
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
                               (s32)((u32)(Random_Next() << 8) >> 16 << 16) +
                                   center->y.fixed - PIXELS(28),
                               center->z.fixed);
  } else {
    effect = FieldActor_Create(SPRITE_PARTICLE, center->x.fixed,
                               (s32)((u32)(Random_Next() << 6) >> 16 << 16) +
                                   center->y.fixed - PIXELS(28),
                               center->z.fixed);
  }
  if (effect != 0) {
    sprite = effect->sprite;
    Actor_StartScript(effect, gVenusLighthouseAerie_OrbitParticleScript);
    Actor_SetSpriteColor(effect, SPRITE_COLOR_GRB);
    effect->motionFlags = 0;
    angle = Random_Next() & 0xffff000;
    effect->timer = angle;
    effect->userValue = 0;
    effect->update = VenusLighthouseAerie_UpdateUpperOrbit;
    effect->speedLimit =
        Trig_Sin((u32)(Random_Next() * 0xffff) >> 20) * 24 >> 16;
    sprite->flags = 0;
    sprite->image.priority = 1;
  }
}

/// Tests whether all three movement targets are unset.
static __inline__ s32 Actor_IsParked(struct Actor *actor) {
  if (actor->targetX == ACTOR_NO_TARGET && actor->targetY == actor->targetX &&
      actor->targetZ == actor->targetY) {
    return 1;
  }
  return 0;
}

/// Pulses and slides Saturos and Menardi while their beam flags are active.
static void VenusLighthouseAerie_UpdateBeamActors(void) {
  struct Actor *saturos;
  struct Actor *menardi;
  s32 pairParked;

  saturos = Actor_GetById(ACTOR_SATUROS);
  menardi = Actor_GetById(ACTOR_MENARDI);
  pairParked = Actor_IsParked(saturos); // Overwritten; only Saturos's
                                        // actor's test loads remain in the ROM.
  pairParked = Actor_IsParked(menardi);
  if (pairParked != 0) {
    saturos->facing = 0;
    menardi->facing = 0;
    if (GameFlag_Get(FLAG_VENUS_AERIE_FUSION_GROWING) != 0) {
      Actor_ConfigureSpriteColor(ACTOR_SATUROS, SPRITE_COLOR_WHITE);
      Actor_ConfigureSpriteColor(ACTOR_MENARDI, SPRITE_COLOR_WHITE);
      if (saturos->scaleX < 5 * Q16_ONE / 4) {
        saturos->scaleX += Q16_ONE / 128;
        saturos->scaleY += Q16_ONE / 128;
        menardi->scaleX += Q16_ONE / 128;
        menardi->scaleY += Q16_ONE / 128;
      }
    } else {
      if ((*(s32 *)&gFrameCount & 2) != 0) {
        Actor_ConfigureSpriteColor(ACTOR_SATUROS, SPRITE_COLOR_HIDDEN);
        Actor_ConfigureSpriteColor(ACTOR_MENARDI, SPRITE_COLOR_NORMAL);
      } else {
        Actor_ConfigureSpriteColor(ACTOR_SATUROS, SPRITE_COLOR_NORMAL);
        Actor_ConfigureSpriteColor(ACTOR_MENARDI, SPRITE_COLOR_HIDDEN);
      }
    }
    if (GameFlag_Get(FLAG_VENUS_AERIE_FUSION_SLIDING) != 0) {
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

/// Plays Saturos and Menardi's defeat, the party's discussion and the beacon's
/// eruption.
static void VenusLighthouseAerie_FinishSaturosAndMenardiBattle(void) {
  Event_Start();
  VenusLighthouseAerie_ShowSaturosAndMenardiFall();
  VenusLighthouseAerie_DiscussSaturosAndMenardiDefeat();
  VenusLighthouseAerie_EruptBeacon();
  Event_End();
}

/// A look back from the world map (VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT,
/// after the world map's lighthouse transition): Felix cries out as Sheba
/// clings to the broken aerie, then the world map returns with its music still
/// playing (VENUS_LIGHTHOUSE_AERIE_TO_WORLD_MAP).
static void VenusLighthouseAerie_ShowShebaInDanger(void) {
  Event_Start();
  VenusLighthouseAerie_RestorePartyOnBrokenAerie();
  VenusLighthouseAerie_TryToPreventFall();
  GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  gFieldWork->transitionFrames = 24;

  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(VENUS_LIGHTHOUSE_AERIE_TO_WORLD_MAP);
  Event_End();
}

/// Back from the world map (VENUS_LIGHTHOUSE_AERIE_FROM_WORLD_MAP): stages the
/// broken aerie for Sheba's fall and the party's escape, plays them, sets
/// FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED and leaves for Lalivero
/// (VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_C).
static void VenusLighthouseAerie_StageShebasFall(void) {
  struct Actor *leader;
  struct Actor *actor;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  Event_Start();
  FieldCamera_GetActor()->motionFlags = 0;
  MapGrid_CopyCellsTo(102, 4, 74, 4, 18, 23);
  MapGrid_CopyCellsTo(39, 72, 11, 72, 16, 20);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 22, 6);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 13, 6);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 22, 13);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 13, 13);
  WaitFrames(1);
  FieldCamera_MoveTo(PIXELS(192), -PIXELS(64), PIXELS(238), 0);
  WaitFrames(1);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  Actor_ReleaseById(ACTOR_SATUROS);
  Actor_ReleaseById(ACTOR_MENARDI);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_KNOCKED_DOWN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  leader->x.fixed = PIXELS(346);
  leader->z.fixed = PIXELS(205);
  leader->y.fixed = PIXELS(32);
  leader->facing = FACING_SOUTHWEST;
  VenusLighthouseAerie_StopActorMotion(leader);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_PARTY_KNOCKED_DOWN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  actor = Actor_Get(ACTOR_GARET);
  actor->x.fixed = PIXELS(356);
  actor->z.fixed = PIXELS(192);
  actor->facing = FACING_NORTHWEST;
  actor->y.fixed = PIXELS(32);
  VenusLighthouseAerie_StopActorMotion(actor);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_PARTY_KNOCKED_DOWN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN), 0);
  actor = Actor_Get(ACTOR_IVAN);
  actor->x.fixed = PIXELS(360);
  actor->facing = FACING_SOUTHEAST;
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(222);
  VenusLighthouseAerie_StopActorMotion(actor);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_PARTY_KNOCKED_DOWN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MIA), 0);
  actor = Actor_Get(ACTOR_MIA);
  actor->x.fixed = PIXELS(334);
  actor->facing = FACING_WEST;
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(222);
  VenusLighthouseAerie_StopActorMotion(actor);
  Actor_SetPosition(ACTOR_FELIX, PIXELS(196), PIXELS(220));
  Import_ActorSetAnimation(ACTOR_FELIX, FELIX_ANIMATION_KNEEL);
  Actor_SetPosition(ACTOR_SHEBA, PIXELS(188), PIXELS(316));
  Import_ActorSetAnimation(ACTOR_SHEBA, SHEBA_ANIMATION_HANG_ONE_HAND);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SHEBA), 0);
  actor = Actor_Get(ACTOR_BEACON_STATUE_SOUTHWEST);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_BEACON_STATUE_NORTHWEST);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_BEACON_STATUE_NORTHEAST);
  actor->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_BEACON_STATUE_SOUTHEAST);
  actor->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEACON_ORB), 0);
  Actor_GetById(ACTOR_BEACON_ORB)->motionFlags = ACTOR_FLOATS;
  Actor_SetSpriteColorById(ACTOR_BEACON_ORB, SPRITE_COLOR_GBR);
  Actor_Get(ACTOR_BEACON_ORB)->y.fixed = PIXELS(40);
  Task_AddFrame(VenusLighthouseAerie_SpawnSideOrbitParticles, TASK_PASS_FRAME);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  gFieldWork->transitionFrames = 24;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(40);
  VenusLighthouseAerie_PlayShebasFall();
  GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED);
  Import_EventSetExitRequest(VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_C);
}

/// Shows Saturos and Menardi falling into the beacon after their final defeat.
static void VenusLighthouseAerie_ShowSaturosAndMenardiFall(void) {
  struct Actor *actor;

  Event_PlaySong(SE_RUMBLE);
  MapGrid_CopyCellAttributes(17, 10, 4, 2, 17, 8);
  Actor_Get(ACTOR_PARTY_LEADER)->facing = FACING_NORTHWEST;
  Actor_Get(ACTOR_GARET)->facing = FACING_NORTHWEST;
  Actor_SetPosition(ACTOR_GARET, PIXELS(328), PIXELS(168));
  Actor_Get(ACTOR_IVAN)->facing = FACING_NORTHWEST;
  Actor_SetPosition(ACTOR_IVAN, PIXELS(340), PIXELS(196));
  Actor_Get(ACTOR_MIA)->facing = FACING_NORTHWEST;
  Actor_SetPosition(ACTOR_MIA, PIXELS(326), PIXELS(204));
  Actor_Get(ACTOR_FELIX)->facing = FACING_NORTH + FACING_STEP;
  Actor_SetPosition(ACTOR_FELIX, PIXELS(200), PIXELS(216));
  Actor_Get(ACTOR_SHEBA)->facing = FACING_NORTH + FACING_STEP;
  Actor_SetPosition(ACTOR_SHEBA, PIXELS(200), PIXELS(216));
  Actor_Get(ACTOR_SATUROS)->facing = FACING_SOUTHEAST + FACING_STEP;
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(310), PIXELS(158));
  Actor_Get(ACTOR_MENARDI)->facing = FACING_SOUTHEAST + FACING_STEP;
  Actor_SetPosition(ACTOR_MENARDI, PIXELS(292), PIXELS(158));

  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEAM_BOTTOM), 0);
  Actor_SetSpriteColorById(ACTOR_BEAM_BOTTOM, SPRITE_COLOR_WHITE);
  Actor_SetSpritePriority(ACTOR_BEAM_BOTTOM, 1);
  actor = Actor_Get(ACTOR_BEAM_BOTTOM);
  actor->scaleY = -Q16_ONE;
  actor->scaleX = Q16_ONE / 5;
  actor->motionFlags = 0;
  actor->x.fixed = PIXELS(304);
  actor->y.fixed = PIXELS(2);
  actor->z.fixed = PIXELS(96);

  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEAM_LOWER), 0);
  Actor_SetSpriteColorById(ACTOR_BEAM_LOWER, SPRITE_COLOR_WHITE);
  Actor_SetSpritePriority(ACTOR_BEAM_LOWER, 1);
  actor = Actor_Get(ACTOR_BEAM_LOWER);
  actor->scaleY = -Q16_ONE;
  actor->scaleX = Q16_ONE / 5;
  actor->motionFlags = 0;
  actor->x.fixed = PIXELS(304);
  actor->y.fixed = PIXELS(34);
  actor->z.fixed = PIXELS(96);

  Task_AddFrame(VenusLighthouseAerie_SpawnUpperOrbitParticles, TASK_PASS_FRAME);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 0);
  WaitFrames(1);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(40);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_WaitFrames(10);
  // Give Saturos and Menardi their final exchange before they fall.
  Event_SetMessageId(MsgVenusPairDefeated);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 3);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MENARDI, 0, 40);
  Event_PlaySong(SONG_FADE_OUT_BGM);

  actor = Actor_Get(ACTOR_SATUROS);
  actor->x.fixed = PIXELS(308);
  actor->y.fixed = PIXELS(28);
  actor->z.fixed = PIXELS(152);
  Import_ActorSetAnimation(ACTOR_SATUROS, SATUROS_ANIMATION_TIP);
  Event_WaitFrames(20);
  actor->x.fixed = PIXELS(306);
  actor->y.fixed = PIXELS(28);
  actor->z.fixed = PIXELS(152);
  Import_ActorSetAnimation(ACTOR_SATUROS, SATUROS_ANIMATION_SLUMP);
  Event_WaitFrames(12);
  actor->x.fixed = PIXELS(304);
  actor->y.fixed = PIXELS(21);
  actor->z.fixed = PIXELS(152);
  Import_ActorSetAnimation(ACTOR_SATUROS, SATUROS_ANIMATION_SINK);
  Event_WaitFrames(8);
  Actor_ReleaseById(ACTOR_SATUROS);

  actor = Actor_Get(ACTOR_MENARDI);
  actor->x.fixed = PIXELS(294);
  actor->y.fixed = PIXELS(28);
  actor->z.fixed = PIXELS(152);
  Import_ActorSetAnimation(ACTOR_MENARDI, MENARDI_ANIMATION_TIP);
  Event_WaitFrames(20);
  actor->x.fixed = PIXELS(300);
  actor->y.fixed = PIXELS(27);
  actor->z.fixed = PIXELS(152);
  Import_ActorSetAnimation(ACTOR_MENARDI, MENARDI_ANIMATION_SLUMP);
  Event_WaitFrames(12);
  actor->x.fixed = PIXELS(304);
  actor->y.fixed = PIXELS(17);
  actor->z.fixed = PIXELS(152);
  Import_ActorSetAnimation(ACTOR_MENARDI, MENARDI_ANIMATION_SINK);
  Event_WaitFrames(8);
  Actor_ReleaseById(ACTOR_MENARDI);
  Event_WaitFrames(160);
}

/// Lets the party discuss the battle while Felix and Sheba remain on the aerie.
static void VenusLighthouseAerie_DiscussSaturosAndMenardiDefeat(void) {
  s32 answerAccepted;
  s32 garetSpeaker;
  s32 miaSpeaker;

  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_WaitFrames(20);
  Event_OpenMessage(ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH);
  answerAccepted = 0;
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    answerAccepted = 1;
  } else {
    Event_WaitFrames(20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    gFieldWork->message += 1;
  }
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  if (answerAccepted != 0) {
    gFieldWork->message += 1;
  }
  Actor_SetSpeed(ACTOR_GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  garetSpeaker = SPEAKER_PORTRAIT_LEFT | ACTOR_GARET;
  Actor_WalkToAndWait(ACTOR_GARET, 321, 174);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTHEAST);
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_NORTH);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTH);
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 40);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_GARET, 0, 40);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  Event_PlaySong(SONG_FADE_OUT_BGM);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
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
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 40);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_Jump(ACTOR_GARET, 4, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetDestination(ACTOR_FELIX, 200, 188);
  Actor_SetDestination(ACTOR_SHEBA, 200, 204);
  FieldCamera_SetSpeed(PIXELS(16) / 5, PIXELS(2) / 5);
  FieldCamera_MoveTo(PIXELS(252), 0, PIXELS(190), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(40);
  Event_PlaySong(MUS_DANGER);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_WaitFrames(20);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  miaSpeaker = SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA;
  Event_WaitFrames(20);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  VenusLighthouseAerie_ShowMessage(miaSpeaker);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  Event_WaitFrames(20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_NORTHEAST);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  FieldEvent_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  VenusLighthouseAerie_ShowMessage(miaSpeaker);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN);
  Import_ActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Import_ActorSetAnimation(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH + FACING_STEP, 20);
  Actor_Jump(ACTOR_MIA, 4, 20);
  FieldEvent_ShowMessageThenWait(miaSpeaker, 0, 20);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
}

static s32 sTransitionTimer;
static s32 sTransitionStep;

/// Raises the beacon effects and splits the aerie beneath the group.
static void VenusLighthouseAerie_EruptBeacon(void) {
  struct Actor *lower;
  struct Actor *middle;
  struct Actor *upper;
  struct Actor *actor;

  Sound_PlaySong(MUS_SILENCE);
  Sound_PlaySong(SE_STOP_ACTION_SE);
  MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
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
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH + FACING_STEP, 0);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Event_WaitFrames(10);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  FieldCamera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 1);
  FieldCamera_WaitForArrival();
  // Open the beacon beam before splitting the surrounding platform.
  Actor_GetById(ACTOR_BEAM_BOTTOM)->scaleX = Q16_ONE / 10;
  Actor_GetById(ACTOR_BEAM_LOWER)->scaleX = Q16_ONE / 10;
  Actor_SetScript(ACTOR_BEAM_BOTTOM, gVenusLighthouseAerie_BeamOpenAction);
  Actor_SetScript(ACTOR_BEAM_LOWER, gVenusLighthouseAerie_BeamOpenAction);
  Sound_PlaySong(SE_CRASH);
  MapGrid_SetShake(PIXELS(6), PIXELS(6), Q16_ONE);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
  PaletteFade_StartInterpolation(16);
  WaitFrames(20);
  PaletteFade_SetTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_StartInterpolation(24);
  WaitFrames(60);
  Sound_PlaySong(SE_RUMBLE);
  GameFlag_Set(FLAG_VENUS_AERIE_BEACON_ERUPTED);
  Actor_GetById(ACTOR_BEAM_BOTTOM)->y.fixed = -PIXELS(96);
  Actor_GetById(ACTOR_BEAM_LOWER)->y.fixed = -PIXELS(64);
  Actor_ConfigureSpriteColor(ACTOR_BEAM_MIDDLE, SPRITE_COLOR_WHITE);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_BEAM_MIDDLE), 0);
  // Stack three more beam segments under the rising upper pair.
  lower = Actor_GetById(ACTOR_BEAM_MIDDLE);
  lower->scaleY = -Q16_ONE;
  lower->scaleX = Actor_GetById(ACTOR_BEAM_BOTTOM)->scaleX;
  lower->motionFlags = 0;
  lower->x.fixed = PIXELS(304);
  lower->y.fixed = -PIXELS(32);
  lower->z.fixed = PIXELS(96);
  Actor_ConfigureSpriteColor(ACTOR_BEAM_UPPER, SPRITE_COLOR_WHITE);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_BEAM_UPPER), 0);
  middle = Actor_GetById(ACTOR_BEAM_UPPER);
  middle->scaleY = -Q16_ONE;
  middle->scaleX = Actor_GetById(ACTOR_BEAM_BOTTOM)->scaleX;
  middle->motionFlags = 0;
  middle->x.fixed = PIXELS(304);
  middle->y.fixed = 0;
  middle->z.fixed = PIXELS(96);
  Actor_ConfigureSpriteColor(ACTOR_BEAM_TOP, SPRITE_COLOR_WHITE);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_BEAM_TOP), 0);
  upper = Actor_GetById(ACTOR_BEAM_TOP);
  upper->scaleY = -Q16_ONE;
  upper->scaleX = Actor_GetById(ACTOR_BEAM_BOTTOM)->scaleX;
  upper->motionFlags = 0;
  upper->x.fixed = PIXELS(304);
  upper->y.fixed = PIXELS(32);
  upper->z.fixed = PIXELS(96);
  MapGrid_CopyCellsTo(102, 4, 74, 4, 18, 23);
  MapGrid_CopyCellsTo(39, 72, 11, 72, 16, 21);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 22, 6);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 13, 6);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 22, 13);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 13, 13);
  WaitFrames(1);
  actor = Actor_GetById(ACTOR_BEACON_STATUE_SOUTHWEST);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_BEACON_STATUE_NORTHWEST);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_BEACON_STATUE_NORTHEAST);
  actor->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_BEACON_STATUE_SOUTHEAST);
  actor->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  actor->x.fixed += PIXELS(16);
  actor->z.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_GARET);
  actor->x.fixed += PIXELS(16);
  actor->z.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_IVAN);
  actor->x.fixed += PIXELS(16);
  actor->z.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_MIA);
  actor->x.fixed += PIXELS(16);
  actor->z.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  Actor_SetPosition(ACTOR_FELIX, PIXELS(196), PIXELS(220));
  Import_ActorSetAnimation(ACTOR_FELIX, FELIX_ANIMATION_KNEEL);
  Actor_SetPosition(ACTOR_SHEBA, PIXELS(188), PIXELS(316));
  Import_ActorSetAnimation(ACTOR_SHEBA, SHEBA_ANIMATION_HANG_ONE_HAND);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SHEBA), 0);
  MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  PaletteFade_SetTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
  PaletteFade_StartInterpolation(120);
  Actor_SetScript(ACTOR_BEAM_BOTTOM, gVenusLighthouseAerie_BeamRiseAction);
  Actor_SetScript(ACTOR_BEAM_LOWER, gVenusLighthouseAerie_BeamRiseAction);
  Actor_SetScript(ACTOR_BEAM_MIDDLE, gVenusLighthouseAerie_BeamRiseAction);
  Actor_SetScript(ACTOR_BEAM_UPPER, gVenusLighthouseAerie_BeamRiseAction);
  Actor_SetScript(ACTOR_BEAM_TOP, gVenusLighthouseAerie_BeamRiseAction);
  WaitFrames(120);
  MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | RGB(16, 16, 12), PALETTES_ALL);
  PaletteFade_StartInterpolation(120);
  WaitFrames(120);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(120);
  WaitFrames(120);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Actor_GetById(ACTOR_BEAM_BOTTOM)->scaleY = Q16_ONE / 50;
  Actor_SetScript(ACTOR_BEAM_LOWER, gVenusLighthouseAerie_BeamCloseAction);
  Actor_SetScript(ACTOR_BEAM_MIDDLE, gVenusLighthouseAerie_BeamCloseAction);
  Actor_SetScript(ACTOR_BEAM_UPPER, gVenusLighthouseAerie_BeamCloseAction);
  Actor_RunScriptAndWait(ACTOR_BEAM_TOP, gVenusLighthouseAerie_BeamCloseAction);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  Actor_ConfigureSpriteColor(ACTOR_BEAM_BOTTOM, SPRITE_COLOR_HIDDEN);
  Event_WaitFrames(20);
  Actor_RunScriptAndWait(ACTOR_BEAM_BOTTOM,
                         gVenusLighthouseAerie_BeamEndAction);
  Task_Remove(VenusLighthouseAerie_SpawnUpperOrbitParticles);
  Actor_Jump(ACTOR_IVAN, 2, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(20);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPositionAndReset(ACTOR_MIA, 326, 220);
  Event_WaitFrames(40);
  FieldEffect_SetAttached(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->riseCounter = 0;
  actor->riseEnabled = 1;
  actor->riseBaseY = actor->y.fixed;
  actor->facing = FACING_NORTHWEST;
  actor = Actor_Get(ACTOR_GARET);
  actor->riseCounter = 0;
  actor->riseEnabled = 1;
  actor->facing = FACING_NORTHWEST;
  actor->riseBaseY = actor->y.fixed;
  actor = Actor_Get(ACTOR_IVAN);
  actor->riseCounter = 0;
  actor->riseEnabled = 1;
  actor->facing = FACING_NORTHWEST;
  actor->riseBaseY = actor->y.fixed;
  actor = Actor_Get(ACTOR_MIA);
  actor->riseCounter = 0;
  actor->riseEnabled = 1;
  actor->facing = FACING_NORTHWEST;
  actor->riseBaseY = actor->y.fixed;
  actor = Actor_Get(ACTOR_FELIX);
  actor->riseCounter = 0;
  actor->riseEnabled = 1;
  actor->riseBaseY = actor->y.fixed;
  actor = Actor_Get(ACTOR_SHEBA);
  actor->riseCounter = 0;
  actor->riseEnabled = 1;
  actor->riseBaseY = actor->y.fixed;
  Actor_GetById(ACTOR_BEACON_ORB)->motionFlags = 0;
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_BEACON_ORB), 0);
  Actor_ConfigureSpriteColor(ACTOR_BEACON_ORB, SPRITE_COLOR_WHITE);
  Actor_SetSpritePriority(ACTOR_BEACON_ORB, 2);
  sTransitionStep = 0;
  sTransitionTimer = 240;
  Task_AddFrame(VenusLighthouseAerie_UpdateBeaconRise, TASK_PASS_FRAME);
  do {
    WaitFrames(1);
  } while (GameFlag_Get(FLAG_VENUS_AERIE_PARTY_LIFTED) == 0);
  GameFlag_Set(FLAG_SKIP_SCENE_CHANGE_SOUND);
  Event_WaitFrames(30);
  GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
  Actor_Snapshot();
  Event_SetDestination((s32)&SceneId_WorldMap,
                       WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT);
  {
    u16 white = 0x7fff;

    PLTT_BACKDROP = white;
  }
  gFieldWork->transitionFrames = 1;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

/// Places the party on the broken platform and resumes its falling effects.
static void VenusLighthouseAerie_RestorePartyOnBrokenAerie(void) {
  struct Actor *actor;

  Actor_ReleaseById(ACTOR_SATUROS);
  Actor_ReleaseById(ACTOR_MENARDI);
  Event_PlaySong(SE_RUMBLE);
  MapGrid_CopyCellAttributes(17, 10, 4, 2, 17, 8);
  MapGrid_CopyCellsTo(102, 4, 74, 4, 18, 23);
  MapGrid_CopyCellsTo(39, 72, 11, 72, 16, 21);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 22, 6);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 13, 6);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 22, 13);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 13, 13);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_Restore();
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  Event_PlaySong(SE_ITEM_BREAK);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_KNOCKED_DOWN);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_PARTY_KNOCKED_DOWN);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_PARTY_KNOCKED_DOWN);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_PARTY_KNOCKED_DOWN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MIA), 0);

  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->x.fixed = PIXELS(346);
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(205);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor->riseEnabled = 0;
  actor->velocityY = PIXELS(2);

  actor = Actor_Get(ACTOR_GARET);
  actor->x.fixed = PIXELS(356);
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(192);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor->riseEnabled = 0;
  actor->velocityY = PIXELS(2);

  actor = Actor_Get(ACTOR_IVAN);
  actor->x.fixed = PIXELS(360);
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(222);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor->riseEnabled = 0;
  actor->velocityY = PIXELS(2);

  actor = Actor_Get(ACTOR_MIA);
  actor->x.fixed = PIXELS(334);
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(222);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor->riseEnabled = 0;
  actor->velocityY = PIXELS(2);

  Actor_Get(ACTOR_FELIX)->riseCounter = 0;
  Actor_Get(ACTOR_SHEBA)->riseCounter = 0;
  Actor_Get(ACTOR_BEACON_ORB)->motionFlags |= 4;
  Actor_SetSpriteColorById(ACTOR_BEACON_ORB, SPRITE_COLOR_GBR);
  Task_AddFrame(VenusLighthouseAerie_SpawnSideOrbitParticles, TASK_PASS_FRAME);
  gFieldWork->transitionFrames = 1;
  Event_StartScreenTransition();
  PaletteFade_ApplySource(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(40);
  WaitFrames(60);
}

/// Shows Felix urging Sheba to hold on at the edge of the platform.
static void VenusLighthouseAerie_TryToPreventFall(void) {
  Event_SetMessageId(MsgVenusNoooo);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Event_PlaySong(MUS_SHEBA_HANGS_ON);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  FieldCamera_SetSpeed(PIXELS(24) / 5, PIXELS(3) / 5);
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  FieldCamera_MoveTo(PIXELS(192), -PIXELS(64), PIXELS(238), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(40);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 3);
  VenusLighthouseAerie_ShowMessage(ACTOR_SHEBA);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(60);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 80);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(40);
  Actor_StartRepeatedMotion(ACTOR_SHEBA, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_SHEBA);
}

/// Shows Sheba falling, Felix jumping after her and the party escaping: it
/// resolves to find Jenna and the others at Idejima, and a long search finds
/// no sign of Jenna.
static void VenusLighthouseAerie_PlayShebasFall(void) {
  struct Actor *actor;
  s32 stepPending;

  stepPending = 0;
  // Sheba loses her grip while Felix tries to keep her on the platform.
  Event_SetMessageId(MsgVenusWhyHappeningProtectVenusLighthouse);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 20);
  Import_ActorSetAnimation(ACTOR_SHEBA, SHEBA_ANIMATION_HANG);
  Event_WaitFrames(10);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 40);
  Import_ActorSetAnimation(ACTOR_SHEBA, SHEBA_ANIMATION_SLIP);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 20);
  Event_PlaySong(SONG_FADE_OUT_BGM);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Actor_SetScript(ACTOR_SHEBA, gVenusLighthouseAerie_StepBackAction);
  Event_WaitFrames(20);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_RefreshSelectorById(ACTOR_SHEBA);
  Event_WaitFrames(160);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_WaitFrames(20);
  Actor_GetById(ACTOR_FELIX)->facing = FACING_SOUTH + FACING_STEP;
  Import_ActorSetAnimation(ACTOR_FELIX, 0);
  Event_WaitFrames(80);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  Event_WaitFrames(40);
  FieldEvent_ShowMessageThenWait(ACTOR_FELIX, 0, 40);
  Import_ActorSetAnimation(ACTOR_FELIX, FELIX_ANIMATION_KNEEL);
  Event_WaitFrames(10);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FELIX), 0);
  Import_ActorSetAnimation(ACTOR_FELIX, 0);
  Actor_Jump(ACTOR_FELIX, 6, 0);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(3), PIXELS(3) / 2);
  Actor_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  actor = Actor_GetById(ACTOR_FELIX);
  // Felix jumps after Sheba; the remaining party watches from the far side.
  actor->update = VenusLighthouseAerie_StepSpriteRotation;
  actor->facing = FACING_WEST;
  Actor_MoveToAndWait(ACTOR_FELIX, 184, 237);
  Actor_RunScriptAndWait(ACTOR_FELIX, gVenusLighthouseAerie_LeapAction);
  Event_WaitFrames(120);
  VenusLighthouseAerie_ShowMessage(ACTOR_PARTY_LEADER);
  Event_PlaySong(MUS_FELIX_LEAPS_AFTER_SHEBA);
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  FieldCamera_MoveTo(PIXELS(342), PIXELS(32), PIXELS(212), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_WaitFrames(40);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PARTY_LEADER), 1);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 60);
  Actor_GetById(ACTOR_PARTY_LEADER)->timer = 10;
  Actor_SetScript(ACTOR_PARTY_LEADER, gVenusLighthouseAerie_ScaleStepAction);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_WaitFrames(20);
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_GARET), 1);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_Jump(ACTOR_GARET, 6, 60);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_WaitFrames(20);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_IVAN), 1);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_Jump(ACTOR_IVAN, 6, 40);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_MIA), 1);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_STAND);
  Actor_Jump(ACTOR_MIA, 6, 60);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 60);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  VenusLighthouseAerie_FaceActor(ACTOR_MIA, FACING_SOUTHWEST);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 80);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTH);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 40);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 40);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTH);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_GARET, 333, 194);
  Actor_WalkToAndWait(ACTOR_GARET, 333, 206);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 80);
  Actor_Stop(ACTOR_PARTY_LEADER);
  WaitFrames(1);
  actor->scaleX = Q16_ONE;
  actor->scaleY = Q16_ONE;
  Event_WaitFrames(40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_SOUTHEAST);
  Actor_Jump(ACTOR_GARET, 4, 0);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_NORTHEAST);
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
    Import_ActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
    VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
    stepPending = 1;
  } else {
    gFieldWork->message += 3;
    Event_WaitFrames(60);
    VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
    Import_ActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
    VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  }
  if (stepPending != 0) {
    gFieldWork->message += 3;
  }
  VenusLighthouseAerie_FaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(80);
  Event_PlaySong(SONG_FADE_OUT_BGM);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 40);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(60);
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Event_PlaySong(SE_RUMBLE);
  MapGrid_SetShake(PIXELS(6), PIXELS(6), Q16_ONE);
  Event_WaitFrames(40);
  Event_PlaySong(SE_CRASH);
  MapGrid_CopyCellsTo(110, 105, 74, 4, 18, 23);
  MapGrid_CopyCellsTo(92, 86, 83, 4, 8, 23);
  MapGrid_CopyCellsTo(75, 28, 75, 4, 8, 23);
  MapGrid_CopyCellsTo(92, 86, 11, 72, 16, 20);
  MapGrid_CopyCellsTo(19, 92, 19, 68, 8, 21);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  actor->z.fixed -= PIXELS(32);
  actor->userValue = 0;
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_GARET);
  actor->x.fixed -= PIXELS(4);
  actor->z.fixed -= PIXELS(32);
  actor->userValue = 0;
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_IVAN);
  actor->x.fixed -= PIXELS(4);
  actor->z.fixed -= PIXELS(32);
  actor->userValue = 0;
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_MIA);
  actor->x.fixed -= PIXELS(4);
  actor->z.fixed -= PIXELS(18);
  actor->userValue = 0;
  VenusLighthouseAerie_StopActorMotion(actor);
  Actor_GetById(ACTOR_BEACON_ORB)->y.fixed = PIXELS(56);
  FieldCamera_MoveTo(PIXELS(338), PIXELS(32), PIXELS(180), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Actor_Jump(ACTOR_IVAN, 6, 0);
  Actor_Jump(ACTOR_MIA, 6, 0);
  // Shake the party and split away the remaining central floor tiles.
  Actor_SetScript(ACTOR_PARTY_LEADER, gVenusLighthouseAerie_PartyHeadingAction);
  Actor_SetScript(ACTOR_GARET, gVenusLighthouseAerie_PartyHeadingAction);
  Actor_SetScript(ACTOR_IVAN, gVenusLighthouseAerie_PartyHeadingAction);
  Actor_SetScript(ACTOR_MIA, gVenusLighthouseAerie_PartyHeadingAction);
  MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Event_WaitFrames(80);
  MapGrid_SetShake(PIXELS(6), PIXELS(6), Q16_ONE);
  Event_WaitFrames(40);
  Event_PlaySong(SE_CRASH);
  MapGrid_CopyCellAttributes(110, 106, 18, 14, 10, 5);
  MapGrid_CopyCellsTo(110, 105, 74, 4, 18, 23);
  MapGrid_CopyCellsTo(92, 86, 11, 68, 16, 20);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_GARET);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_IVAN);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_MIA);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_BEACON_STATUE_SOUTHWEST);
  actor->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_BEACON_STATUE_NORTHWEST);
  actor->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_BEACON_STATUE_NORTHEAST);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_GetById(ACTOR_BEACON_STATUE_SOUTHEAST);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  FieldCamera_MoveTo(PIXELS(322), PIXELS(32), PIXELS(180), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
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
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Event_WaitFrames(80);
  Event_PlaySong(SONG_FADE_OUT_ACTION_SE);
  Event_WaitFrames(40);
  Actor_Stop(ACTOR_PARTY_LEADER);
  Actor_Stop(ACTOR_GARET);
  Actor_Stop(ACTOR_IVAN);
  Actor_Stop(ACTOR_MIA);
  Event_WaitFrames(120);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_MIA);
  Actor_Jump(ACTOR_IVAN, 2, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_IVAN);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(20);
  VenusLighthouseAerie_FaceActor(ACTOR_GARET, FACING_EAST);
  VenusLighthouseAerie_ShowMessage(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_SetScript(ACTOR_GARET, gVenusLighthouseAerie_PartyExitAction);
  Actor_SetScript(ACTOR_IVAN, gVenusLighthouseAerie_PartyExitAction);
  Actor_SetScript(ACTOR_MIA, gVenusLighthouseAerie_PartyExitAction);
  Event_WaitFrames(60);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 272, 216);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 272, 254);
  Event_WaitFrames(80);
  ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))->transitionFrames = 16;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(80);
  ActorSystem_SuspendProjectionAndShowSprites();
  ((struct WindowWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_WINDOW))->voiceSound = 0;
  ((struct WindowWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_WINDOW))->voiceCountdown = 0;
  FieldMessage_ShowCentered(MsgVenusDespiteLongTiring, 0, 0);
  ActorSystem_ResumeProjectionAndHideLayers();
  Event_WaitFrames(80);
}

/// Clears all movement targets and velocities and resets the actor
/// timer.
static void VenusLighthouseAerie_StopActorMotion(struct Actor *actor) {
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;

  SET_VELOCITY(actor, 0, 0, 0);

  actor->timer = 0;
}

/// Advances the beacon rise sequence, its flashes and orbiting particles.
static void VenusLighthouseAerie_UpdateBeaconRise(void) {
  struct Actor *center;
  struct Actor *effect;
  struct Sprite *sprite;
  s32 spawnParticles;
  u32 randomHeight;
  s32 angle;

  center = Actor_Get(ACTOR_BEACON_ORB);
  spawnParticles = 0;
  switch (sTransitionStep) {
  case 0:
    Event_PlaySong(SE_SHIMMER);
    MapGrid_SetShake(PIXELS(6), PIXELS(6), Q16_ONE);
    PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(31, 31, 24), PALETTES_BG);
    PaletteFade_StartInterpolation(8);
    break;
  case 8:
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
    PaletteFade_StartInterpolation(8);
    break;
  case 16:
    MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    break;
    // Start the lower beam beneath the floor, then pause each rise stage.

  case 24:
    center->x.fixed = PIXELS(304);
    center->y.fixed = -PIXELS(360);
    center->z.fixed = PIXELS(164);
    center->scaleX = Q16_ONE;
    center->scaleY = Q16_ONE;
    VenusLighthouseAerie_StopActorMotion(center);
    Actor_SetScript(ACTOR_BEACON_ORB, gVenusLighthouseAerie_RiseActionScript);
    break;
  case 25:
    sTransitionStep--;
    if (center->y.fixed > 0) {
      PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 16, 12),
                              PALETTES_ALL);
      PaletteFade_StartInterpolation(16);
      sTransitionStep++;
      Actor_Get(ACTOR_PARTY_LEADER)->riseCounter = 1;
      Actor_Get(ACTOR_GARET)->riseCounter = 1;
      Actor_Get(ACTOR_IVAN)->riseCounter = 1;
      Actor_Get(ACTOR_MIA)->riseCounter = 1;
      Actor_Get(ACTOR_FELIX)->riseCounter = 1;
      Actor_Get(ACTOR_SHEBA)->riseCounter = 1;
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlaySong(SE_GLITTER);
      spawnParticles = 1;
      center->y.fixed += PIXELS(9) / 4;
    }
    break;
  case 26:
    sTransitionStep--;
    if (center->y.fixed > (PIXELS(40))) {
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
      PaletteFade_StartInterpolation(40);
      sTransitionStep++;
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlaySong(SE_GLITTER);
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
    Event_PlaySong(SE_FALLING_LIGHT);
    PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
    PaletteFade_StartInterpolation(12);
    break;
  case 48:
    Actor_Stop(ACTOR_BEACON_ORB);
    Event_SetFlag(FLAG_VENUS_AERIE_PARTY_LIFTED);
    break;
  }
  if (spawnParticles) {
    randomHeight = ((u32)(Random_Next() * 80) >> 16) << 16;
    effect = FieldActor_Create(SPRITE_PARTICLE, center->x.fixed,
                               center->y.fixed - randomHeight + -PIXELS(8),
                               center->z.fixed);
    if (effect != 0) {
      sprite = effect->sprite;
      Actor_StartScript(effect, gVenusLighthouseAerie_RiseParticleScript);
      Actor_SetSpriteColor(effect, SPRITE_COLOR_GRB);
      effect->motionFlags = 0;
      angle = Random_Next() & 0xffff000;
      effect->timer = angle;
      effect->userValue = 0;
      effect->riseCounter = (u32)Random_Next() >> 13;
      effect->update = VenusLighthouseAerie_UpdateRisingOrbit;
      effect->speedLimit = Trig_Sin((u32)(Random_Next() * 0xffff) >> 20) * 24;
      effect->speedLimit = center->speedLimit >> 16;
      sprite->flags = 0;
      sprite->image.priority = 1;
    }
  }
  sTransitionStep++;
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(ACTOR_PARTY_LEADER));
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(ACTOR_GARET));
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(ACTOR_IVAN));
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(ACTOR_MIA));
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(ACTOR_FELIX));
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(ACTOR_SHEBA));
}

/// Raises an enabled actor according to its gated rise counter.
static void VenusLighthouseAerie_AdvanceActorRise(struct Actor *actor) {
  if (actor->riseEnabled != 0) {
    u8 counter = actor->riseCounter;

    actor->y.fixed = actor->riseBaseY + ((counter >> 2) << 16);

    VenusLighthouseAerie_StopActorMotion(actor);

    if (actor->riseCounter != 0) {
      if (actor->riseCounter <= 31) {
        ++actor->riseCounter;
      }
    }
  }
}

/// Emits particles beside the lower beam and links each to its source
/// actor.
static void VenusLighthouseAerie_SpawnSideOrbitParticles(void) {
  struct Actor *center;
  struct Actor *effect;
  struct Sprite *sprite;
  struct MapGridScrollWork *grid;
  s32 drift;
  s32 phase;
  s32 angle;

  center = ((struct Actor *)Actor_GetById(23));
  grid = gMapGridWork[0];
  drift = (u32)(Random_Next() * 48) >> 16 << 16;
  if ((s16)(grid->viewY >> 16) <= 129) {
    if (gFrameCount & 1) {
      Actor_SetPosition(ACTOR_BEACON_ORB, PIXELS(304), PIXELS(164));
      Actor_Get(ACTOR_BEACON_ORB)->scaleX = Q16_ONE;
      Actor_Get(ACTOR_BEACON_ORB)->scaleY = Q16_ONE;
    } else {
      Actor_SetPosition(ACTOR_BEACON_ORB, PIXELS(304), PIXELS(171));
      Actor_Get(ACTOR_BEACON_ORB)->scaleX = 13 * Q16_ONE / 10;
      Actor_Get(ACTOR_BEACON_ORB)->scaleY = 13 * Q16_ONE / 10;
    }
  } else {
    Actor_SetPosition(ACTOR_BEACON_ORB, 0, 0);
  }
  if (center == 0)
    return;
  phase = gFrameCount & 15;
  if (phase != 0)
    return;
  effect =
      FieldActor_Create(SPRITE_PARTICLE, center->x.fixed + PIXELS(8),
                        center->y.fixed + drift + PIXELS(8), center->z.fixed);
  drift /= PIXELS(6);
  drift <<= 16;
  if (effect == 0)
    return;
  sprite = effect->sprite;
  Actor_StartScript(effect, gVenusLighthouseAerie_BesideParticleScript);
  Actor_SetSpriteColor(effect, SPRITE_COLOR_BRG);
  effect->motionFlags = phase;
  angle = Random_Next() & 0xffff000;
  effect->timer = angle;
  effect->userValue = phase;
  effect->linkedActor = center;
  effect->update = VenusLighthouseAerie_UpdateOrbitingEffect;
  effect->speedLimit = Trig_Sin((drift & 0xfffff) >> 4) * 24 >> 16;
  sprite->flags = 0;
  sprite->image.priority = center->sprite->image.priority;
}

/// Moves a linked particle along its horizontal ellipse and preserves its
/// target.
static void VenusLighthouseAerie_UpdateOrbitingEffect(struct Actor *actor) {
  struct Actor *anchor;
  s32 spin;

  anchor = actor->linkedActor;
  spin = ((struct FieldEffect *)actor)->spin;
  ((struct FieldEffect *)actor)->x =
      anchor->x.fixed + Trig_Cos(spin) * (actor->speedLimit + 28);
  ((struct FieldEffect *)actor)->z = Trig_Sin(spin) * 16 + PIXELS(164);
  actor->targetX = ((struct FieldEffect *)actor)->x;
  actor->targetZ = ((struct FieldEffect *)actor)->z;
  ((struct FieldEffect *)actor)->spin -= 0x200;
}

s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_RELEASE,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gVenusLighthouseAerie_JoinLeaderAction[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(492), 0, PIXELS(164)),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gVenusLighthouseAerie_StepBackWestAction[] = {
    SC_SET(SW_SPEED, PIXELS(1) / 5),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 10),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(-PIXELS(8), 0, 0),
    SC_SYNC,
    SC_END,
};

s32 gVenusLighthouseAerie_StepBackEastAction[] = {
    SC_SET(SW_SPEED, PIXELS(1) / 5),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 10),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(PIXELS(8), 0, 0),
    SC_SYNC,
    SC_END,
};

s32 gVenusLighthouseAerie_ScaleStepAction[] = {
    SC_CALL(VenusLighthouseAerie_PulseActorScale),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gVenusLighthouseAerie_BeamOpenAction[] = {
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_WAIT(1),
    SC_LOOP(32, SC_TOP),
    SC_END,
};

s32 gVenusLighthouseAerie_BeamNarrowAction[] = {
    SC_ADD(SW_SCALE_X, -7 * Q16_ONE / 512),
    SC_WAIT(1),
    SC_LOOP(140, SC_TOP),
    SC_END,
};

s32 gVenusLighthouseAerie_BeamRiseAction[] = {
    SC_ADD(SW_SCALE_X, -7 * Q16_ONE / 512),
    SC_WAIT(1),
    SC_LOOP(160, SC_TOP),
    SC_END,
};

s32 gVenusLighthouseAerie_BeamCloseAction[] = {
    SC_ADD(SW_Y, PIXELS(4)), SC_WAIT(1), SC_LOOP(30, SC_TOP),
    SC_POSITION(0, 0, 0),    SC_WAIT(1), SC_END,
};

s32 gVenusLighthouseAerie_BeamEndAction[] = {
    SC_ADD(SW_Y, PIXELS(2)), SC_WAIT(1), SC_LOOP(160, SC_TOP),
    SC_POSITION(0, 0, 0),    SC_WAIT(1), SC_END,
};

s32 gVenusLighthouseAerie_OrbitParticleScript[] = {
    SC_SET(SW_SCALE_X, 4 * Q16_ONE / 5),
    SC_SET(SW_SCALE_Y, 4 * Q16_ONE / 5),
    SC_ANIMATION(1),
    SC_LABEL(1),
    SC_ADD(SW_Y, PIXELS(1)),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

s32 gVenusLighthouseAerie_RiseParticleScript[] = {
    SC_SET(SW_SCALE_X, 4 * Q16_ONE / 5),
    SC_SET(SW_SCALE_Y, 4 * Q16_ONE / 5),
    SC_ANIMATION(1),
    SC_LABEL(1),
    SC_ADD(SW_Y, -PIXELS(1)),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

s32 gVenusLighthouseAerie_WalkOutAction[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(268), 0, PIXELS(186)),
    SC_SYNC,
    SC_MOVE(PIXELS(268), 0, PIXELS(252)),
    SC_SYNC,
    SC_MOVE(PIXELS(232), 0, PIXELS(222)),
    SC_SYNC,
    SC_MOVE(PIXELS(232), 0, PIXELS(136)),
    SC_SYNC,
    SC_MOVE(PIXELS(200), 0, PIXELS(136)),
    SC_SYNC,
    SC_MOVE(PIXELS(200), 0, PIXELS(200)),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gVenusLighthouseAerie_RiseActionScript[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -3 * Q16_ONE / 128),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, 1536),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gVenusLighthouseAerie_StepBackAction[] = {
    SC_ADD(SW_Y, -(PIXELS(3) / 4)), SC_WAIT(1), SC_LOOP(120, SC_TOP),
    SC_POSITION(0, 0, 0),           SC_WAIT(1), SC_END,
};

s32 gVenusLighthouseAerie_LeapAction[] = {
    SC_ADD(SW_Y, -(PIXELS(9) / 2)), SC_WAIT(1), SC_LOOP(80, SC_TOP),
    SC_POSITION(0, 0, 0),           SC_WAIT(1), SC_END,
};

s32 gVenusLighthouseAerie_PartyExitAction[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(330), 0, PIXELS(173)),
    SC_SYNC,
    SC_RELEASE,
    SC_END,
};

s32 gVenusLighthouseAerie_PartyHeadingAction[] = {
    SC_CALL(Scene_LookAboutRandomly),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gVenusLighthouseAerieEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 304, 0, 216,
     FACING_SOUTHEAST + FACING_STEP, 0, 0, 0, 600, 420, 0},
    {VENUS_LIGHTHOUSE_AERIE_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR,
     CONDITION_ALWAYS, 504, 0, 168, FACING_SOUTHEAST + FACING_STEP, 0, 0, 0,
     600, 420, 0},
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
struct ActorTemplate gVenusLighthouseAerieActorTemplates[] = {
    { SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(440), 0, PIXELS(252), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(472), 0, PIXELS(252), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(504), 0, PIXELS(252), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(104), 0, PIXELS(252), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(136), 0, PIXELS(252), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(168), 0, PIXELS(252), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(504), 0, PIXELS(148), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(504), 0, PIXELS(148), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(504), 0, PIXELS(148), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_MENARDI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(164), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(180), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { ACTOR_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(172), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_FELIX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(252), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLUE_ORB, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};
// clang-format on

s32 gVenusLighthouseAerie_LandedPlatform = 0;

struct Trigger gVenusLighthouseAerieTriggers[] = {
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     MB_VENUS_LIGHTHOUSE_AERIE_FLOOR_BY_PLATFORMS, CONDITION_ALWAYS,
     EVENT(VenusLighthouseAerie_JumpToNextPlatform)},
    {TRIGGER_STEP, MB_VENUS_LIGHTHOUSE_AERIE_FLOOR_BY_PLATFORMS,
     CONDITION_ALWAYS, EVENT(VenusLighthouseAerie_FloatPlatforms)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     MB_VENUS_LIGHTHOUSE_AERIE_PLATFORM, CONDITION_ALWAYS,
     EVENT(VenusLighthouseAerie_JumpToNextPlatform)},
    {TRIGGER_STEP, MB_VENUS_LIGHTHOUSE_AERIE_PLATFORM,
     FLAG_VENUS_AERIE_ON_PLATFORM, EVENT(VenusLighthouseAerie_LandOnPlatform)},
    {TRIGGER_STEP, MB_VENUS_LIGHTHOUSE_AERIE_BEFORE_BEACON, CONDITION_ALWAYS,
     EVENT(VenusLighthouseAerie_ConfrontSaturosAndMenardi)},
    {TRIGGERS_END},
};

s32 gVenusLighthouseAerie_BesideParticleScript[] = {
    SC_SET(SW_SCALE_Y, 7 * Q16_ONE / 5),
    SC_SET(SW_SCALE_X, 7 * Q16_ONE / 5),
    SC_ANIMATION(4),
    SC_WAIT(30),
    SC_RELEASE,
};
