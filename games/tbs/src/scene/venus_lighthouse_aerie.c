/// @file
/// Lighthouse Aerie (SceneId_VenusLighthouseAerie), the top of Venus Lighthouse
/// and the only map of this overlay: one scripted sequence cut by two battles
/// and two trips to the world map. Each part starts in
/// VenusLighthouseAerie_Initialize from the warp destination the part before
/// sent the party to, and every arrival blocks Retreat and sets
/// FLAG_VENUS_LIGHTHOUSE_LIT. The parts, in the order the warp destinations
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
///   EventScript_ConfrontSaturosAndMenardi): the Shaman's Rod is handed over,
///   remembered in FLAG_SHAMANS_ROD_HOLDER plus the member who carried it, and
///   the first battle follows (ENCOUNTER_ZONE_SCRIPTED, slot 1,
///   FLAG_AERIE_FIRST_BATTLE_BEGUN).
/// - After it (VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE,
///   VenusLighthouseAerie_CombineSaturosAndMenardi): Saturos throws the Venus
///   Star into the beacon (LOCALID_VENUS_STAR_SHADOW), Felix and Sheba leave,
///   and Saturos and Menardi fuse. Party_RestorePpOfLitElements refills the PP
///   of the party's Adepts whose lighthouses are lit, and the second battle
///   follows (slot 0, FLAG_AERIE_SECOND_BATTLE_BEGUN).
/// - After that (VENUS_LIGHTHOUSE_AERIE_AFTER_SECOND_BATTLE,
///   VenusLighthouseAerie_FinishSaturosAndMenardiBattle): Saturos and Menardi
///   fall into the lighthouse, and the beacon erupts: its beam
///   (LOCALID_BEAM_BOTTOM to LOCALID_BEAM_TOP) rises, the aerie splits and its
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
#include "object_event.h"
#include "types.h"
#include "animspr.h"
#include "venus_lighthouse_aerie.h"
#include "field_effect.h"
#include "script_layer.h"
#include "map_tables.h"
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

// Map section flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the
// party enters another map section, and other map sections use the numbers for
// their own.

/// Plus the party member (0 to 3) who carried the Shaman's Rod when it was
/// handed over before the aerie's first battle; a defeat gives it back.
/// ★ Named from the game: "Shaman's Rod" (MsgItemName)
#define FLAG_SHAMANS_ROD_HOLDER 0x345

/// Map section flags that choose the aerie's field music after each battle, by
/// the map's field music rules (gMap_MusicRules,
/// overworld/map_rules.s): MUS_SILENCE before the first battle, MUS_BABI once
/// the first has begun and MUS_ELEMENTAL_STARS once the second has.
/// VenusLighthouseFirstFloor and VenusLighthouseFourthFloor, in the same map
/// section, keep their Psynergy Stones in the same two flags, which no one goes
/// back down to see.
enum {
  /// ★ Named from its area name: Lighthouse Aerie
  FLAG_AERIE_FIRST_BATTLE_BEGUN = FLAG_MAPSEC_TEMP_50,
  /// ★ Named from its area name: Lighthouse Aerie
  FLAG_AERIE_SECOND_BATTLE_BEGUN = FLAG_MAPSEC_TEMP_51
};

static void EventScript_FloatPlatforms(void);
static void VenusLighthouseAerie_OverhearSaturosAndMenardi(void);
static void VenusLighthouseAerie_ShowPartyMember(s32 localId, s32 shown);
static void
VenusLighthouseAerie_FlashObjectEventPalette(struct ObjectEvent *objectEvent);
static void
VenusLighthouseAerie_SpawnLinkedPairEffects(struct ObjectEvent *parent);
static void VenusLighthouseAerie_UpdateShebaPalette(void);
static void VenusLighthouseAerie_CombineSaturosAndMenardi(void);
static void VenusLighthouseAerie_FlashScreen(void);
static void VenusLighthouseAerie_SpawnUpperOrbitParticles(void);
static void VenusLighthouseAerie_UpdateBeamObjectEvents(void);
static void VenusLighthouseAerie_FinishSaturosAndMenardiBattle(void);
static void VenusLighthouseAerie_ShowShebaInDanger(void);
static void VenusLighthouseAerie_StageShebasFall(void);
static void VenusLighthouseAerie_ShowSaturosAndMenardiFall(void);
static void VenusLighthouseAerie_DiscussSaturosAndMenardiDefeat(void);
static void VenusLighthouseAerie_EruptBeacon(void);
static void VenusLighthouseAerie_RestorePartyOnBrokenAerie(void);
static void VenusLighthouseAerie_TryToPreventFall(void);
static void VenusLighthouseAerie_PlayShebasFall(void);
static void
VenusLighthouseAerie_StopObjectEventMotion(struct ObjectEvent *objectEvent);
static void VenusLighthouseAerie_UpdateBeaconRise(void);
static void
VenusLighthouseAerie_AdvanceObjectEventRise(struct ObjectEvent *objectEvent);
static void VenusLighthouseAerie_SpawnSideOrbitParticles(void);
static void VenusLighthouseAerie_UpdateOrbitingEffect(
    union ObjectEventOrEffect *fieldObject);

/// The metatile behaviors the aerie's coord events answer.
enum VenusLighthouseAerieMetatileBehavior {
  MB_VENUS_LIGHTHOUSE_AERIE_BEFORE_BEACON =
      5, ///< where the party confronts Saturos and Menardi
  MB_VENUS_LIGHTHOUSE_AERIE_PLATFORM = 10, ///< a floating platform's cells
  /// The floor beside the platforms, from which the leader jumps
  /// onto them.
  MB_VENUS_LIGHTHOUSE_AERIE_FLOOR_BY_PLATFORMS = 11
};

/// A child value that makes an object event blink rather than set its colour
/// mode (ObjectEvent_ConfigureSpriteColor).
enum { CHILD_VALUE_BLINK = 0x100 };

/// Animations the aerie plays by number beyond enum ObjectEventAnimation, from
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

/// Pulses the object event scale around the countdown, then chooses its next
/// pause.
static s32
VenusLighthouseAerie_PulseObjectEventScale(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  s16 *counter = &objectEvent->timer;

  switch (*counter) {
  case 6:
    objectEvent->scaleX += -Q16_ONE / 8;
    objectEvent->scaleY += Q16_ONE / 16;
    break;
  case 4:
    objectEvent->scaleX += Q16_ONE / 16;
    objectEvent->scaleY += -Q16_ONE / 32;
    break;
  case 2:
    objectEvent->scaleX += Q16_ONE / 32;
    objectEvent->scaleY += -Q16_ONE / 64;
    break;
  case 0:
    objectEvent->scaleX += Q16_ONE / 32;
    objectEvent->scaleY += -Q16_ONE / 64;
    *counter = Math_RemainderUnsigned(Import_RandomNext(), 80) + 80;
    break;
  }
  {
    s32 t = (u16)objectEvent->timer;
    t--;
    objectEvent->timer = t;
  }
  return 1;
}

/// Moves a particle, damps its horizontal velocity and updates its scale and
/// spin.
static void VenusLighthouseAerie_UpdateDriftingParticle(
    union ObjectEventOrEffect *fieldObject) {
  s32 velocityX = fieldObject->effect.velocity.x;
  s32 velocityZ;

  fieldObject->effect.x += velocityX;
  fieldObject->effect.y += fieldObject->effect.velocity.y;
  velocityZ = fieldObject->effect.velocity.z;
  fieldObject->effect.z += velocityZ;
  fieldObject->effect.velocity.x = velocityX - velocityX / 22;
  fieldObject->effect.velocity.z = velocityZ - velocityZ / 20;
  fieldObject->effect.scaleX += fieldObject->effect.scaleRateX;
  fieldObject->effect.scaleY += fieldObject->effect.scaleRateY;
  fieldObject->effect.sprite->rotation += fieldObject->effect.spin;
}

/// Turns the falling object event sprite until its rotation leaves the negative
/// half-turn.
static void VenusLighthouseAerie_StepSpriteRotation(
    union ObjectEventOrEffect *fieldObject) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  struct Sprite *sprite = objectEvent->sprite;
  u16 rotation = sprite->rotation;

  if ((s16)(rotation - 1) < 0) {
    {
      s32 nextRotation = rotation - 0xa00;
      sprite->rotation = nextRotation;
    }
  }
}

#include "common/object_heading.h"

const struct MapWarpDest *VenusLighthouseAerie_GetWarpDests(void) {
  return gVenusLighthouseAerieWarpDests;
}

const struct MapWarpRegion *VenusLighthouseAerie_GetRegions(void) { return 0; }

const u32 *VenusLighthouseAerie_GetWarps(void) {
  return gVenusLighthouseAerieWarps;
}

/// Returns the beacon's statues, the platforms, Saturos and Menardi, Felix and
/// Sheba, the party and the beacon effects.
const struct ObjectEventTemplate *
VenusLighthouseAerie_GetObjectEventTemplates(void) {
  return gVenusLighthouseAerieObjectEventTemplates;
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
static void EventScript_JumpToNextPlatform(void) {
  struct ObjectEvent *objectEvent;
  union {
    struct FieldPosition position;
    s32 coordinates[3];
  } destination;
  s32 angle;
  u8 flags;

  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  angle = (objectEvent->facing + 0x1000) & 0xe000;
  flags = objectEvent->motionFlags;
  SET_VECTOR(
      destination.coordinates, (objectEvent->x.fixed & 0xfff00000) + PIXELS(8),
      objectEvent->y.fixed, (objectEvent->z.fixed & 0xfff00000) + PIXELS(8));
  Vector_AddPolarOffset(PIXELS(32), angle, destination.coordinates);
  if (FieldObject_CheckMovementCollision(objectEvent, &destination.position) ==
      0) {
    GameFlag_Clear(FLAG_VENUS_AERIE_ON_PLATFORM);
    EventScript_FloatPlatforms();
    Object_SelectAnimation(objectEvent, ANIM_LEADER_SPRING);
    WaitFrames(6);
    Object_SelectAnimation(objectEvent, ANIM_LEADER_LEAP);
    objectEvent->speed = PIXELS(3);
    objectEvent->acceleration = PIXELS(2);
    Sound_PlaySong(SE_JUMP);
    objectEvent->velocityY = PIXELS(4);
    objectEvent->motionFlags &= 0x7e;
    ObjectEvent_SetSpriteFlags(objectEvent, 0);
    ObjectMotion_SetPositionAndCommit(LOCALID_PARTY_LEADER,
                                      destination.coordinates[0] >> 16,
                                      destination.coordinates[2] >> 16);
    Object_SelectAnimation(objectEvent, ANIM_LEADER_SPRING);
    ObjectEvent_SetSpriteFlags(objectEvent, 1);
    objectEvent->motionFlags = flags;
  }
}

/// Landing on a platform: it dips under the leader and springs back up.
/// FLAG_VENUS_AERIE_ON_PLATFORM is set while the leader stands on a platform,
/// so this runs once per landing.
static void EventScript_LandOnPlatform(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct ObjectEvent *platform;
  s32 platformId;
  s32 reboundStep;

  FieldScript_Start();
  platformId = ObjectEvent_FindNearestFloatingPlatform();
  gVenusLighthouseAerie_LandedPlatform = platformId;
  if (platformId != 0) {
    GameFlag_Set(FLAG_VENUS_AERIE_ON_PLATFORM);
    platform = ObjectEvent_GetById(gVenusLighthouseAerie_LandedPlatform);
    platform->motionFlags = 0;
    leader->motionFlags &= ~OBJ_EVENT_FOLLOWS_TERRAIN;
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
  FieldScript_End();
}

/// Stepping back onto the floor, and before each jump: the leader walks and
/// falls normally again, and all six platforms float, bobbing, as they do
/// until the leader lands on one (EventScript_LandOnPlatform).
static void EventScript_FloatPlatforms(void) {
  s32 flags;

  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags =
      OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
  flags = OBJ_EVENT_FLOATS;
  ObjectEvent_GetById(LOCALID_FIRST_PLATFORM)->motionFlags = flags;
  ObjectEvent_GetById(LOCALID_FIRST_PLATFORM + 1)->motionFlags = flags;
  ObjectEvent_GetById(LOCALID_FIRST_PLATFORM + 2)->motionFlags = flags;
  ObjectEvent_GetById(LOCALID_FIRST_PLATFORM + 3)->motionFlags = flags;
  ObjectEvent_GetById(LOCALID_FIRST_PLATFORM + 4)->motionFlags = flags;
  ObjectEvent_GetById(LOCALID_LAST_PLATFORM)->motionFlags = flags;
}

/// Returns the platform and confrontation coord events.
const struct MapEvent *VenusLighthouseAerie_GetEvents(void) {
  return gVenusLighthouseAerieEvents;
}

/// Sets up the beacon's statues and the six platforms.
/// It then runs the part of the sequence the warp destination selects (the
/// arrival only when not back from a battle), or, after a lost battle, gives
/// back the Shaman's Rod and leaves for Lalivero Sanctum. Back from a battle,
/// the platform nearest the leader stands still.
s32 VenusLighthouseAerie_Initialize(void) {
  s32 mirroredScale;
  u32 platformId;
  s32 clear;
  s16 warpDest;
  struct ObjectEvent *platform;

  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  WaitFrames(1);
  GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_LIT);
  ObjectEvent_SetSpriteFlags(
      ObjectEvent_GetById(LOCALID_BEACON_STATUE_SOUTHWEST), 0);
  ObjectEvent_SetSpriteFlags(
      ObjectEvent_GetById(LOCALID_BEACON_STATUE_NORTHWEST), 0);
  ObjectEvent_SetSpriteFlags(
      ObjectEvent_GetById(LOCALID_BEACON_STATUE_NORTHEAST), 0);
  ObjectEvent_SetSpriteFlags(
      ObjectEvent_GetById(LOCALID_BEACON_STATUE_SOUTHEAST), 0);
  mirroredScale = -Q16_ONE;
  ObjectEvent_GetById(LOCALID_BEACON_STATUE_NORTHEAST)->scaleX = mirroredScale;
  ObjectEvent_GetById(LOCALID_BEACON_STATUE_SOUTHEAST)->scaleX = mirroredScale;
  platformId = LOCALID_FIRST_PLATFORM;
  clear = 0;
  do {
    platform = ObjectEvent_GetById(platformId);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(platformId), clear);
    ObjectMotion_SetActionVariant(platformId, 1);
    platform->motionFlags = OBJ_EVENT_FLOATS;
    platform->priorityFlags |= 2;
    platform->y.fixed = PIXELS(1) / 2;
    platformId++;
  } while (platformId <= LOCALID_LAST_PLATFORM);

  warpDest = gSaveBlock.warpDest;

  switch (warpDest) {
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
    FieldScript_Start();
    if (GameFlag_Get(FLAG_SHAMANS_ROD_HOLDER) != 0) {
      Inventory_AddItem(0, ITEM_SHAMANS_ROD);
    } else if (GameFlag_Get(FLAG_SHAMANS_ROD_HOLDER + 1) != 0) {
      Inventory_AddItem(1, ITEM_SHAMANS_ROD);
    } else if (GameFlag_Get(FLAG_SHAMANS_ROD_HOLDER + 2) != 0) {
      Inventory_AddItem(2, ITEM_SHAMANS_ROD);
    } else {
      Inventory_AddItem(3, ITEM_SHAMANS_ROD);
    }
    Import_ScriptRequestWarp(VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_B);
    break;
  }

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    s32 slot = ObjectEvent_FindNearestFloatingPlatform();

    if (slot != 0) {
      struct ObjectEvent *nearestPlatform = ObjectEvent_GetById(slot);
      if (nearestPlatform != 0) {
        nearestPlatform->motionFlags = 0;
      }
    }
  }
  return clear;
}

static void VenusLighthouseAerie_ShowMessage(s32 speaker) {
  FieldScript_ShowMessageAndWait(speaker, 0);
  FieldScript_WaitFrames(10);
}

/// Turns one object event and waits ten frames.
static void VenusLighthouseAerie_FaceObjectEvent(s32 localId, s32 facing) {
  ObjectMotion_ArmCallback(localId, facing, 10);
}

/// Brings the party onto the aerie and reveals the argument over Sheba.
static void VenusLighthouseAerie_OverhearSaturosAndMenardi(void) {
  struct ObjectEvent *arrivalPlatform;
  struct ObjectEvent *leader;

  // Bring the leader across on the moving platform, then reveal the party.
  arrivalPlatform = ObjectEvent_GetById(LOCALID_RISING_PLATFORM);
  FieldScript_Start();
  VenusLighthouseAerie_ShowPartyMember(1, 0);
  VenusLighthouseAerie_ShowPartyMember(2, 0);
  VenusLighthouseAerie_ShowPartyMember(3, 0);
  FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
  WaitFrames(1);
  arrivalPlatform->motionFlags = 0;
  arrivalPlatform->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_RISING_PLATFORM), 0);
  ObjectMotion_SetActionVariant(LOCALID_RISING_PLATFORM, 1);
  Import_ObjectEventSetPosition(LOCALID_RISING_PLATFORM, PIXELS(580),
                                PIXELS(338));
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  leader->motionFlags = 0;
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 1);
  Import_ObjectEventSetPosition(LOCALID_PARTY_LEADER, PIXELS(581), PIXELS(288));
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_RISING_PLATFORM, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetDestination(LOCALID_RISING_PLATFORM, 580, 221);
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 581, 171);
  ObjectEvent_SetDestination(LOCALID_RISING_PLATFORM, 530, 211);
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 531, 161);
  ObjectEvent_SetDestination(LOCALID_RISING_PLATFORM, 520, 191);
  ObjectMotion_SetPositionAndCommit(LOCALID_PARTY_LEADER, 521, 141);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_RISING_PLATFORM), 1);
  ObjectEvent_SetDestination(LOCALID_RISING_PLATFORM, 515, 171);
  ObjectMotion_SetPositionAndCommit(LOCALID_PARTY_LEADER, 516, 121);
  Sound_PlaySong(SE_STOP_ACTION_SE);
  FieldScript_WaitFrames(20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_LEADER_SPRING);
  ObjectMotion_WaitForAnimationChange(LOCALID_PARTY_LEADER);
  leader->x.fixed = PIXELS(516);
  leader->y.fixed = PIXELS(8);
  leader->z.fixed = PIXELS(148);
  leader->facing = FACING_WEST;
  leader->motionFlags = OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
  Sound_PlaySong(SE_JUMP);
  leader->velocityY = PIXELS(4);
  Sound_PlaySong(SE_JUMP);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER), 1);
  ObjectMotion_SetPositionAndCommit(LOCALID_PARTY_LEADER, 504, 148);
  FieldScript_WaitFrames(10);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  {
    u16 shown = FACING_SOUTH;

    leader->y.fixed = -PIXELS(32);
    leader->facing = shown;
  }
  FieldScript_WaitFrames(20);
  Sound_PlaySong(SE_MATERIALIZE);
  ObjectMotion_SetPositionAndCommit(LOCALID_RISING_PLATFORM, 524, 191);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_RISING_PLATFORM), 0);
  ObjectEvent_MoveToAndWait(LOCALID_RISING_PLATFORM, 530, 211);
  ObjectEvent_MoveToAndWait(LOCALID_RISING_PLATFORM, 580, 221);
  ObjectMotion_ResetAndSetPosition(LOCALID_RISING_PLATFORM, 580, 338);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->priorityFlags |=
      OBJ_EVENT_PRIORITY_AUTOMATIC;
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  VenusLighthouseAerie_ShowPartyMember(1, 1);
  VenusLighthouseAerie_ShowPartyMember(2, 1);
  VenusLighthouseAerie_ShowPartyMember(3, 1);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 492, 164);
  ObjectMotion_ResetAndSetPositionInMode2(LOCALID_GARET, 514, 164);
  ObjectEvent_WalkTo(LOCALID_IVAN, 492, 140);
  ObjectMotion_SetPositionAndReset(LOCALID_MIA, 514, 140);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_STAND);
  ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectMotion_ArmCallback(LOCALID_GARET, FACING_SOUTH, 0);
  ObjectMotion_ArmCallback(LOCALID_IVAN, FACING_SOUTH, 0);
  ObjectMotion_ArmCallback(LOCALID_MIA, FACING_SOUTH, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_RISING_PLATFORM);
  Import_ObjectEventSetPosition(LOCALID_RISING_PLATFORM, 0, 0);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  // Reveal Saturos and Menardi arguing with Felix about Sheba.
  FieldScript_SetMessageId(MsgVenusLongLastTime);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_MENARDI);
  ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectMotion_ArmCallback(LOCALID_GARET, FACING_WEST, 0);
  ObjectMotion_ArmCallback(LOCALID_IVAN, FACING_WEST, 0);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MIA, FACING_WEST);
  FieldCamera_GetScriptObject()->motionFlags = 0;
  FieldCamera_SetSpeedParameters(PIXELS(24) / 5, PIXELS(3) / 5);
  FieldCamera_MoveWithinMapBounds(PIXELS(304), PIXELS(32), PIXELS(158), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_SATUROS,
                                       FACING_NORTH + FACING_STEP);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 1);
  Sound_PlaySong(MUS_SATUROS_AND_MENARDI);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  Import_ObjectEventSetAnimation(LOCALID_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_MENARDI);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_SATUROS,
                                       FACING_NORTHWEST + FACING_STEP);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldEffect_SpawnLinked(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          20);
  ObjectEvent_FaceDirection(LOCALID_SHEBA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_SOUTH + FACING_STEP, 20);
  FieldCamera_SetSpeedParameters(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(294), -1, PIXELS(194), 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_FELIX, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_FELIX, 272, 200);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 1);
  FieldScript_WaitFrames(20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  VenusLighthouseAerie_ShowMessage(LOCALID_MENARDI);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  FieldScript_ShowMessageThenWait(LOCALID_MENARDI, 0, 40);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MENARDI, FACING_WEST);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectMotion_ArmCallback(LOCALID_SHEBA, FACING_EAST, 0);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectMotion_ArmCallback(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 1);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MENARDI,
                                       FACING_SOUTH + FACING_STEP);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  FieldScript_ShowMessageThenWait(LOCALID_FELIX, 0, 20);
  ObjectMotion_ArmCallback(LOCALID_SATUROS, FACING_WEST, 40);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_SHAKE_HEAD);
  FieldScript_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS, 0,
                                  40);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_NOD);
  FieldScript_ShowMessageThenWait(LOCALID_FELIX, 0, 20);
  ObjectMotion_ArmCallback(LOCALID_SATUROS, FACING_SOUTH + FACING_STEP, 20);
  ObjectMotion_SetVariantCallback(LOCALID_FELIX, 2);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  ObjectMotion_SetVariantCallback(LOCALID_FELIX, 2);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectMotion_SetVariantCallback(LOCALID_MENARDI, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_SHAKE_HEAD);
  FieldScript_ShowMessageThenWait(LOCALID_FELIX, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  FieldScript_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS, 0,
                                  20);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_FELIX,
                                       FACING_NORTHWEST + FACING_STEP);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_SHEBA,
                                       FACING_SOUTHEAST + FACING_STEP);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SHEBA, 2);
  FieldScript_WaitFrames(20);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_FELIX,
                                       FACING_NORTH + FACING_STEP);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectMotion_ArmCallback(LOCALID_SHEBA, FACING_EAST, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 1);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  ObjectMotion_ArmCallback(LOCALID_MENARDI, FACING_WEST, 0);
  ObjectMotion_ArmCallback(LOCALID_SATUROS, FACING_WEST, 0);
  FieldCamera_SetSpeedParameters(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveTo(PIXELS(294), -1, PIXELS(180), 1);
  ObjectEvent_WalkToAndWait(LOCALID_FELIX, 262, 176);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_WEST, 40);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_EAST, 20);
  ObjectMotion_SetVariantCallback(LOCALID_FELIX, 2);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_FELIX);
  FieldEffect_SpawnLinked(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectMotion_SetVariantCallback(LOCALID_FELIX, 2);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_FELIX);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_FELIX);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_FELIX);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_WEST, 20);
  FieldScript_ShowMessageThenWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | LOCALID_FELIX, 0, 40);
  ObjectEvent_ShowEmote(LOCALID_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_EAST, 40);
  Import_ObjectEventSetAnimation(LOCALID_MENARDI, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 1);
  FieldScript_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | LOCALID_FELIX, 0, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  FieldEffect_SpawnLinked(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          40);
  Import_ObjectEventSetAnimation(LOCALID_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectEvent_ShowEmote(LOCALID_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  FieldEffect_SpawnLinked(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  FieldEffect_SpawnLinked(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                          20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(LOCALID_SATUROS, 2);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  Import_ObjectEventSetAnimation(LOCALID_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_WINDOW, 2);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldCamera_MoveWithinMapBounds(PIXELS(504), -PIXELS(24), PIXELS(168), 0);
  WaitFrames(1);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_GARET);
  FieldEffect_SpawnLinked(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | LOCALID_IVAN, 0, 40);
  ObjectMotion_Launch(LOCALID_GARET, 2, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_GARET);
  FieldMessage_ShowAtObject(SPEAKER_WINDOW_BELOW | LOCALID_GARET, 0);
  ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectMotion_ArmCallback(LOCALID_IVAN, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_SOUTHEAST, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 1) {
    gFieldWork->message += 1;
  }
  FieldScript_WaitFrames(20);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  ObjectMotion_ArmCallback(LOCALID_IVAN, FACING_EAST, 0);
  ObjectMotion_ArmCallback(LOCALID_MIA, FACING_WEST, 20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  ObjectEvent_SetActionScript(LOCALID_GARET,
                              gVenusLighthouseAerie_JoinLeaderAction);
  ObjectEvent_SetActionScript(LOCALID_IVAN,
                              gVenusLighthouseAerie_JoinLeaderAction);
  ObjectEvent_RunActionScriptAndWait(LOCALID_MIA,
                                     gVenusLighthouseAerie_JoinLeaderAction);
  FieldScript_WaitFrames(20);
  FieldScript_End();
}

/// Shows or hides one companion, setting its movement speed when shown.
static void VenusLighthouseAerie_ShowPartyMember(s32 localId, s32 shown) {
  struct ObjectEvent *member;

  if (shown != 0) {
    ObjectEvent_SetSpriteColorById(localId, SPRITE_COLOR_NORMAL);
    member = ObjectEvent_Get(localId);
    ObjectEvent_SetSpriteFlags(member, 1);
    ObjectEvent_SetSpeed(localId, PIXELS(4) / 5, PIXELS(2) / 5);
  } else {
    ObjectEvent_SetSpriteColorById(localId, SPRITE_COLOR_HIDDEN);
    member = ObjectEvent_Get(localId);
    ObjectEvent_SetSpriteFlags(member, 0);
  }
}

/// Walking up to Saturos and Menardi (MB_VENUS_LIGHTHOUSE_AERIE_BEFORE_BEACON):
/// Felix refuses to be rescued, Sheba reads the party's minds for the rod that
/// reaches Jupiter Lighthouse, they promise not to harm her for the Shaman's
/// Rod, Felix carries it over, they will not let her go, and the first battle
/// begins. It sets FLAG_SHAMANS_ROD_HOLDER plus the member who carried the rod,
/// and FLAG_AERIE_FIRST_BATTLE_BEGUN as the battle starts, which changes the
/// aerie's music.
static void EventScript_ConfrontSaturosAndMenardi(void) {
  s32 rodHolder;
  struct ObjectEvent *objectEvent;
  s32 answerAccepted;
  s32 frame;
  s32 receiptMessage;

  FieldScript_Start();
  FieldCamera_GetScriptObject()->motionFlags = 0;
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(332), PIXELS(32), PIXELS(180), 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 340, 184);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_PARTY_LEADER, FACING_WEST);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 1);
  FieldScript_SetMessageId(MsgVenusIsaac);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_LEFT | LOCALID_FELIX);
  FieldCamera_GetScriptObject()->motionFlags = 0;
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 1);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(7) / 5, PIXELS(7) / 10);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(LOCALID_GARET, objectEvent->x.fixed,
                            objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(LOCALID_IVAN, objectEvent->x.fixed,
                            objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(LOCALID_MIA, objectEvent->x.fixed,
                            objectEvent->z.fixed);
  }
  ObjectEvent_WalkTo(LOCALID_GARET, 328, 168);
  ObjectEvent_WalkTo(LOCALID_IVAN, 340, 196);
  ObjectEvent_WalkToAndWait(LOCALID_MIA, 326, 204);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_STAND);
  ObjectMotion_ArmCallback(LOCALID_GARET, FACING_WEST, 0);
  ObjectMotion_ArmCallback(LOCALID_IVAN, FACING_WEST, 0);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MIA, FACING_WEST);
  ObjectMotion_ArmCallback(LOCALID_SATUROS, FACING_EAST, 0);
  ObjectMotion_ArmCallback(LOCALID_MENARDI, FACING_EAST, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 2);
  VenusLighthouseAerie_ShowMessage(LOCALID_SATUROS);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MENARDI, FACING_WEST);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectMotion_SetVariantCallback(LOCALID_FELIX, 2);
  Script_ShowMessageAndWait(LOCALID_FELIX, 0, 20);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_NORTH + FACING_STEP, 40);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_FELIX, FACING_EAST);
  Script_ShowMessageAndWait(LOCALID_FELIX, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 1);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_SATUROS, FACING_WEST);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  ObjectEvent_SetAttachedEffect(LOCALID_MIA, ATTACHED_EFFECT_IN_FRONT |
                                                 ATTACHED_EFFECT_ONCE);
  ObjectMotion_SetVariantCallback(LOCALID_MIA, 2);
  VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
  VenusLighthouseAerie_ShowMessage(LOCALID_IVAN);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MENARDI, FACING_EAST);
  VenusLighthouseAerie_ShowMessage(LOCALID_MENARDI);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  ObjectMotion_ArmCallback(LOCALID_SATUROS, FACING_EAST, 20);
  VenusLighthouseAerie_ShowMessage(LOCALID_SATUROS);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTHEAST, 0);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_WEST, 0);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MENARDI,
                                       FACING_SOUTHEAST + FACING_STEP);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  VenusLighthouseAerie_ShowMessage(LOCALID_MENARDI);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_SATUROS,
                                       FACING_NORTHWEST + FACING_STEP);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  ObjectMotion_ArmCallback(LOCALID_SATUROS, FACING_WEST, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  ObjectMotion_ArmCallback(LOCALID_MENARDI, FACING_WEST, 0);
  ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectMotion_ArmCallback(LOCALID_GARET, FACING_WEST, 0);
  ObjectMotion_ArmCallback(LOCALID_IVAN, FACING_WEST, 0);
  ObjectMotion_ArmCallback(LOCALID_MIA, FACING_WEST, 0);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_EAST, 20);
  ObjectEvent_ShowEmote(LOCALID_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(LOCALID_SHEBA);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SHEBA, 2);
  FieldScript_WaitFrames(20);
  Import_ObjectEventSetAnimation(LOCALID_SHEBA, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(LOCALID_SHEBA);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  ObjectEvent_SetSpeed(LOCALID_SHEBA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_SHEBA, 260, 186);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_WalkToAndWait(LOCALID_SHEBA, 276, 192);
  objectEvent = ObjectEvent_GetById(LOCALID_MENARDI);
  objectEvent->facing = FACING_SOUTH + FACING_STEP;
  WaitFrames(1);
  ObjectMotion_SetVariantCallback(LOCALID_MENARDI, 2);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectMotion_Launch(LOCALID_SHEBA, 2, 20);
  ObjectEvent_SetSpeed(LOCALID_SHEBA, PIXELS(12) / 5, PIXELS(6) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_SHEBA, 260, 186);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_WEST, 0);
  ObjectEvent_WalkToAndWait(LOCALID_SHEBA, 248, 172);
  ObjectMotion_ArmCallback(LOCALID_MENARDI, FACING_WEST, 0);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_WEST, 0);
  ObjectMotion_ArmCallback(LOCALID_SHEBA, FACING_EAST, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_SHEBA, ANIM_NOD);
  FieldScript_WaitFrames(40);
  Scene_StartEventPsynergy();
  WaitFrames(1);
  frame = 0;
  do {
    objectEvent = ObjectEvent_GetById(LOCALID_SHEBA);
    VenusLighthouseAerie_FlashObjectEventPalette(objectEvent);
    frame = (frame + 1);
    WaitFrames(1);
  } while ((u32)frame <= 39);
  FieldScheduler_AddCallback(VenusLighthouseAerie_UpdateShebaPalette,
                             TASK_PRIORITY_MAP);
  FieldScript_WaitFrames(80);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTHEAST, 20);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_EAST, 40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_WEST, 0);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_FELIX, FACING_WEST);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  ObjectMotion_SetVariantCallback(LOCALID_MENARDI, 2);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  Task_Remove(VenusLighthouseAerie_UpdateShebaPalette);
  WaitFrames(1);
  ObjectEvent_SetSpriteColorById(LOCALID_SHEBA, SPRITE_COLOR_NORMAL);
  WaitFrames(10);
  Scene_EndEventPsynergy();
  ObjectMotion_Launch(LOCALID_SHEBA, 2, 40);
  VenusLighthouseAerie_ShowMessage(LOCALID_SHEBA);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SHEBA, 2);
  ObjectEvent_GetById(LOCALID_SHEBA)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(LOCALID_SHEBA, 250, 176);
  FieldScript_WaitFrames(1);
  ObjectEvent_GetById(LOCALID_SHEBA)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_EAST, 20);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SHEBA, 2);
  FieldScript_WaitFrames(40);
  ObjectEvent_SetSpeed(LOCALID_SHEBA, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_GetById(LOCALID_SHEBA)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(LOCALID_SHEBA, 248, 172);
  FieldScript_WaitFrames(1);
  ObjectEvent_GetById(LOCALID_SHEBA)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_SHEBA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectMotion_SetAnimationAndWait(LOCALID_SHEBA, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(LOCALID_SHEBA);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP,
                            20);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Import_ObjectEventSetAnimation(LOCALID_SATUROS, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(LOCALID_MENARDI);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 100);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 1);
  FieldScript_WaitFrames(20);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_SATUROS, FACING_EAST);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MENARDI, FACING_EAST);
  Script_OpenMessage(LOCALID_SATUROS, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTHEAST, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_NOD);
    answerAccepted = 1;
  } else {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_SHAKE_HEAD);
    gFieldWork->message += 1;
    answerAccepted = 0;
  }
  VenusLighthouseAerie_ShowMessage(LOCALID_SATUROS);
  if (answerAccepted != 0) {
    gFieldWork->message += 1;
  }
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_WEST, 20);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Script_OpenMessage(LOCALID_SATUROS, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTHEAST, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(20);
    Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
    answerAccepted = 1;
  } else {
    FieldScript_WaitFrames(20);
    Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_SHAKE_HEAD);
    gFieldWork->message += 1;
    answerAccepted = 0;
  }
  VenusLighthouseAerie_ShowMessage(LOCALID_IVAN);
  if (answerAccepted != 0) {
    gFieldWork->message += 1;
  }
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_GARET, FACING_SOUTH);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 1);
  VenusLighthouseAerie_ShowMessage(LOCALID_MENARDI);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MIA, FACING_WEST);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_NOD);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MENARDI,
                                       FACING_SOUTHEAST + FACING_STEP);
  Import_ObjectEventSetAnimation(LOCALID_MENARDI, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(LOCALID_MENARDI);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP,
                            20);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_NOD);
  FieldScript_WaitFrames(40);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_SATUROS, FACING_WEST);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_FELIX, 0, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MENARDI, FACING_WEST);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 1);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_FELIX, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_FELIX, 288, 192);
  ObjectMotion_ArmCallback(LOCALID_MENARDI, FACING_EAST, 0);
  ObjectMotion_ArmCallback(LOCALID_SATUROS, FACING_EAST, 0);
  ObjectEvent_WalkToAndWait(LOCALID_FELIX, 310, 192);
  ObjectEvent_WalkToAndWait(LOCALID_FELIX, 328, 186);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 2);

  // Complete the Shaman's Rod exchange before the confrontation resumes.
  receiptMessage = MsgVenusIsaacHandedOverShamansRod;
  FieldMessage_Show(receiptMessage, 1);
  ObjectEvent_WalkToAndWait(LOCALID_FELIX, 310, 192);
  ObjectMotion_ArmCallback(LOCALID_MENARDI, FACING_WEST, 0);
  ObjectMotion_ArmCallback(LOCALID_SATUROS, FACING_WEST, 0);
  ObjectEvent_WalkToAndWait(LOCALID_FELIX, 288, 192);
  ObjectEvent_WalkToAndWait(LOCALID_FELIX, 262, 176);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_EAST, 40);
  FieldScript_SetMessageId((receiptMessage + 1));
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_NOD);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_SATUROS, FACING_EAST);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_FELIX, FACING_WEST);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_SHEBA, ANIM_NOD);
  ObjectEvent_SetSpeed(LOCALID_SHEBA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_SHEBA, 260, 186);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_WalkToAndWait(LOCALID_SHEBA, 276, 192);
  Script_PlaySong(MUS_SILENCE);
  objectEvent = ObjectEvent_GetById(LOCALID_MENARDI);
  objectEvent->facing = FACING_SOUTH + FACING_STEP;
  WaitFrames(1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 1);
  VenusLighthouseAerie_ShowMessage(LOCALID_MENARDI);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SHEBA, 2);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_EAST, 0);
  ObjectMotion_ArmCallback(LOCALID_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SHEBA, FACING_NORTH + FACING_STEP, 20);
  ObjectMotion_Launch(LOCALID_MIA, 2, 20);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MIA, FACING_NORTHWEST);
  VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_EAST, 0);
  ObjectMotion_ArmCallback(LOCALID_SHEBA, FACING_EAST, 0);
  ObjectMotion_ArmCallback(LOCALID_MENARDI, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP,
                            80);
  ObjectMotion_ArmCallback(LOCALID_MENARDI, FACING_EAST, 0);
  ObjectMotion_ArmCallback(LOCALID_SATUROS, FACING_EAST, 40);
  Script_PlaySong(MUS_CONFRONTATION);
  VenusLighthouseAerie_ShowMessage(LOCALID_SATUROS);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SHEBA, FACING_NORTHWEST + FACING_STEP, 20);
  ObjectMotion_SetVariantCallback(LOCALID_IVAN, 2);
  VenusLighthouseAerie_ShowMessage(LOCALID_IVAN);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MENARDI,
                                       FACING_SOUTHEAST + FACING_STEP);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(LOCALID_MENARDI);
  ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_EAST, 0);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_SHEBA,
                                       FACING_NORTH + FACING_STEP);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        20);
  ObjectMotion_ArmCallback(LOCALID_SATUROS, FACING_SOUTHEAST + FACING_STEP, 20);
  VenusLighthouseAerie_ShowMessage(LOCALID_SATUROS);
  ObjectEvent_ShowEmote(LOCALID_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectMotion_ArmCallback(LOCALID_SHEBA, FACING_EAST, 0);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_FELIX, FACING_EAST);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MENARDI, FACING_EAST);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(LOCALID_MENARDI);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_SATUROS, FACING_EAST);
  Import_ObjectEventSetAnimation(LOCALID_SATUROS, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(LOCALID_SATUROS);
  ObjectMotion_ArmCallback(LOCALID_SHEBA, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_ShowEmote(LOCALID_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(LOCALID_MIA, 0, 20);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MENARDI,
                                       FACING_SOUTHEAST + FACING_STEP);
  Import_ObjectEventSetAnimation(LOCALID_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(LOCALID_MENARDI);
  ObjectEvent_SetAttachedEffect(LOCALID_IVAN, ATTACHED_EFFECT_IN_FRONT |
                                                  ATTACHED_EFFECT_ONCE);
  Script_ShowMessageAndWait(LOCALID_IVAN, 0, 20);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MENARDI, FACING_EAST);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(LOCALID_SATUROS);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  ObjectMotion_SetVariantCallback(LOCALID_IVAN, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 2);
  ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTHEAST, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 1);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MIA, FACING_NORTHWEST);
  VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
  ObjectMotion_ArmCallback(LOCALID_GARET, FACING_WEST, 20);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  ObjectMotion_ArmCallback(LOCALID_SHEBA, FACING_EAST, 0);
  ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectMotion_ArmCallback(LOCALID_IVAN, FACING_WEST, 40);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  ObjectMotion_SetVariantCallback(LOCALID_SATUROS, 2);
  VenusLighthouseAerie_ShowMessage(LOCALID_SATUROS);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 1);
  VenusLighthouseAerie_ShowMessage(LOCALID_MENARDI);
  // Save the rod owner for the defeat return path, then enter battle.
  rodHolder = PartyInventory_FindMember(ITEM_SHAMANS_ROD);
  GameFlag_Set(rodHolder + FLAG_SHAMANS_ROD_HOLDER);
  PartyInventory_Remove(ITEM_SHAMANS_ROD);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_GetById(LOCALID_GARET)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_GetById(LOCALID_IVAN)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_GetById(LOCALID_MIA)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_GetById(LOCALID_MENARDI)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_GetById(LOCALID_SATUROS)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_GetById(LOCALID_FELIX)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_GetById(LOCALID_SHEBA)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                              gVenusLighthouseAerie_StepBackWestAction);
  ObjectEvent_SetActionScript(LOCALID_GARET,
                              gVenusLighthouseAerie_StepBackWestAction);
  ObjectEvent_SetActionScript(LOCALID_IVAN,
                              gVenusLighthouseAerie_StepBackWestAction);
  ObjectEvent_SetActionScript(LOCALID_MIA,
                              gVenusLighthouseAerie_StepBackWestAction);
  ObjectEvent_SetActionScript(LOCALID_MENARDI,
                              gVenusLighthouseAerie_StepBackEastAction);
  ObjectEvent_SetActionScript(LOCALID_SATUROS,
                              gVenusLighthouseAerie_StepBackEastAction);
  ObjectEvent_SetActionScript(LOCALID_FELIX,
                              gVenusLighthouseAerie_StepBackWestAction);
  ObjectEvent_RunActionScriptAndWait(LOCALID_SHEBA,
                                     gVenusLighthouseAerie_StepBackWestAction);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  {
    s32 localId = (s32)&SceneId_VenusLighthouseAerie;

    FieldScript_SetNextDestination(localId,
                                   VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE);
    FieldScript_SetDefeatDestination(localId,
                                     VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT);
  }
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 1);
  GameFlag_Set(FLAG_AERIE_FIRST_BATTLE_BEGUN);
}

/// Flashes an object event palette and emits paired effects every fifteenth
/// frame.
static void
VenusLighthouseAerie_FlashObjectEventPalette(struct ObjectEvent *objectEvent) {
  if (*(s32 *)&gFrameCount & 2) {
    ObjectEvent_SetColorMode(objectEvent, SPRITE_COLOR_WHITE);
  } else {
    ObjectEvent_SetColorMode(objectEvent, SPRITE_COLOR_NORMAL);
  }
  if (Math_RemainderUnsigned(*(s32 *)&gFrameCount, 15) == 0) {
    VenusLighthouseAerie_SpawnLinkedPairEffects(objectEvent);
  }
}

/// Cycles the object event palette and emits paired effects every fifteenth
/// frame.
static void
VenusLighthouseAerie_CycleObjectEventPalette(struct ObjectEvent *objectEvent) {
  if (gFrameCount & 1) {
    ObjectEvent_SetColorMode(objectEvent,
                             Math_RemainderUnsigned(gFrameCount >> 1, 6));
  }
  if (Math_RemainderUnsigned(gFrameCount, 15) == 0) {
    VenusLighthouseAerie_SpawnLinkedPairEffects(objectEvent);
  }
}

#include "common/effect_vertical.h"

/// Spawns two vertical effects using the parent position and shared cached
/// tiles.
static void
VenusLighthouseAerie_SpawnLinkedPairEffects(struct ObjectEvent *parent) {
  struct ObjectEvent *pair[2];
  struct ObjectEvent *child;
  struct Sprite *sprite;
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  s32 effectIndex;

  Sound_PlaySong(SE_PSYNERGY_SPARKLE);
  for (effectIndex = 0; effectIndex < 2; ++effectIndex) {
    child = ObjectEvent_Create(SPRITE_GOLDEN_RINGS, parent->x.fixed,
                               parent->y.fixed, parent->z.fixed);
    pair[effectIndex] = child;
    if (child != NULL) {
      child->groundHeight = parent->groundHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->linkedObject = parent;
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
    struct ObjectEvent *effect = pair[0];
    struct Sprite *effectSprite = effect->sprite;

    effect->update = FieldEffect_AnimateVerticalNegative;
    effectSprite->image.priority = parent->sprite->image.priority;
  }
  {
    struct ObjectEvent *effect = pair[1];
    struct Sprite *effectSprite = effect->sprite;

    effectSprite->image.priority = parent->sprite->image.priority;
    effect->update = FieldEffect_AnimateVerticalPositive;
    effect->priorityFlags = 2;
  }
}

#include "common/effect_psynergy.h"

static void VenusLighthouseAerie_UpdateShebaPalette(void) {
  VenusLighthouseAerie_CycleObjectEventPalette(
      ObjectEvent_GetById(LOCALID_SHEBA));
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
  ObjectEvent_ShowEmote(LOCALID_FELIX, emote, 0);
  ObjectEvent_ShowEmote(LOCALID_SHEBA, emote, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, emote, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, emote, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, emote, 0);
  ObjectEvent_ShowEmote(LOCALID_MIA, emote, waitFrames);
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
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *menardi;
  struct ObjectEvent *saturos;
  union ObjectEventOrEffect *star = NULL;
  struct ObjectEvent *starShadow;
  struct Sprite *sprite;
  u8 *iconPixels;
  // Reuse the dialogue answer word for the later radial burst directions.
  u32 answeredYes;

  Script_Start();
  MapGrid_CopyCellAttributes(17, 10, 4, 2, 17, 8);
  ObjectEvent_Get(LOCALID_GARET)->facing = FACING_WEST;
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(328), PIXELS(168));
  ObjectEvent_Get(LOCALID_IVAN)->facing = FACING_WEST;
  ObjectEvent_SetPosition(LOCALID_IVAN, PIXELS(340), PIXELS(196));
  ObjectEvent_Get(LOCALID_MIA)->facing = FACING_WEST;
  ObjectEvent_SetPosition(LOCALID_MIA, PIXELS(326), PIXELS(204));
  ObjectEvent_Get(LOCALID_SHEBA)->facing = FACING_SOUTHEAST + FACING_STEP;
  ObjectEvent_SetPosition(LOCALID_SHEBA, PIXELS(268), PIXELS(154));
  ObjectEvent_Get(LOCALID_FELIX)->facing = FACING_SOUTHEAST + FACING_STEP;
  ObjectEvent_SetPosition(LOCALID_FELIX, PIXELS(268), PIXELS(164));
  objectEvent = ObjectEvent_Get(LOCALID_SATUROS);
  objectEvent->timer = 10;
  objectEvent->facing = FACING_NORTH + FACING_STEP;
  ObjectEvent_SetPosition(LOCALID_SATUROS, PIXELS(294), PIXELS(212));
  ObjectEvent_SetAnimation(LOCALID_SATUROS, SATUROS_ANIMATION_LIE);
  ObjectEvent_SetActionScript(LOCALID_SATUROS,
                              gVenusLighthouseAerie_ScaleStepAction);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_SATUROS), 0);
  objectEvent = ObjectEvent_Get(LOCALID_MENARDI);
  objectEvent->timer = 10;
  objectEvent->facing = FACING_EAST;
  ObjectEvent_SetPosition(LOCALID_MENARDI, PIXELS(286), PIXELS(192));
  ObjectEvent_SetAnimation(LOCALID_MENARDI, MENARDI_ANIMATION_LIE);
  ObjectEvent_SetActionScript(LOCALID_MENARDI,
                              gVenusLighthouseAerie_ScaleStepAction);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_MENARDI), 0);
  Script_GetViewCenter()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 0);
  Task_Wait(1);
  MapGrid_Redraw();
  Task_Wait(1);
  Script_OpenScreen();
  Script_WaitForScreen();
  Script_Wait(80);

  ObjectEvent_Jump(LOCALID_GARET, 2, 20);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_GARET, FACING_SOUTHEAST);
  Script_SetMessage(MsgVenusBeatEm);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_LEFT | LOCALID_GARET);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MIA, FACING_NORTHWEST);
  VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 0);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_IVAN, FACING_NORTHWEST);
  ObjectEvent_RunRepeatedMotion(LOCALID_FELIX, 2);
  ObjectEvent_SetSpeed(LOCALID_FELIX, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_Get(LOCALID_FELIX)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(LOCALID_FELIX, 280, 164);
  Script_Wait(1);
  ObjectEvent_Get(LOCALID_FELIX)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  Script_Wait(20);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  VenusLighthouseAerie_ShowMessage(LOCALID_IVAN);
  ObjectEvent_SetAnimationAndWait(LOCALID_FELIX, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  ObjectEvent_RunRepeatedMotion(LOCALID_GARET, 1);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  ObjectEvent_RunRepeatedMotion(LOCALID_FELIX, 1);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_FELIX, FACING_EAST);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_StartRepeatedMotion(LOCALID_MIA, 2);
  VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
  ObjectEvent_SetAnimation(LOCALID_MIA, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_FELIX,
                                       FACING_SOUTHEAST + FACING_STEP);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_Jump(LOCALID_IVAN, 2, 20);
  VenusLighthouseAerie_ShowMessage(LOCALID_IVAN);
  ObjectEvent_SetAnimationAndWait(LOCALID_FELIX, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_RunRepeatedMotion(LOCALID_GARET, 1);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_GARET, FACING_SOUTHEAST);
  Script_OpenMessage(LOCALID_GARET, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTHEAST, 0);
  if (Script_ChooseYesNo(0, 0) == 0) {
    answeredYes = TRUE;
  } else {
    gFieldWork->message++;
    answeredYes = FALSE;
  }
  Task_Wait(20);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHWEST, 0);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MIA, FACING_NORTHWEST);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_FELIX, FACING_EAST);
  Script_ShowMessage(LOCALID_FELIX, 0);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  Script_Wait(40);
  if (answeredYes) {
    gFieldWork->message++;
  }

  // Saturos and Menardi recover from their defeat before using the Venus Star.
  ObjectEvent_Stop(LOCALID_SATUROS);
  ObjectEvent_Stop(LOCALID_MENARDI);
  Task_Wait(1);
  objectEvent = ObjectEvent_Get(LOCALID_SATUROS);
  objectEvent->scaleX = Q16_ONE;
  objectEvent->scaleY = Q16_ONE;
  objectEvent = ObjectEvent_Get(LOCALID_MENARDI);
  objectEvent->scaleX = Q16_ONE;
  objectEvent->scaleY = Q16_ONE;
  Task_Wait(1);
  VenusLighthouseAerie_ShowMessage(LOCALID_SATUROS);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_WEST, 40);
  ObjectEvent_SetAnimation(LOCALID_SATUROS, ANIM_STAND);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_SATUROS), 1);
  Script_Wait(20);
  ObjectEvent_SetSpeed(LOCALID_SATUROS, PIXELS(1) / 5, PIXELS(1) / 10);
  ObjectEvent_WalkToAndWait(LOCALID_SATUROS, 300, 206);
  Script_Wait(20);
  ObjectEvent_RunRepeatedMotion(LOCALID_SATUROS, 2);
  Script_Wait(20);
  ObjectEvent_Get(LOCALID_SATUROS)->scaleX = -Q16_ONE;
  Script_PlaySong(SE_RATTLE);
  ObjectEvent_SetAnimation(LOCALID_SATUROS, SATUROS_ANIMATION_BREATHE);
  Script_Wait(20);
  ObjectEvent_SetAnimation(LOCALID_MENARDI, ANIM_STAND);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_MENARDI), 1);
  ObjectEvent_Jump(LOCALID_MENARDI, 4, 40);
  ObjectEvent_SetSpeed(LOCALID_MENARDI, PIXELS(1) / 5, PIXELS(1) / 10);
  ObjectEvent_Get(LOCALID_MENARDI)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(LOCALID_MENARDI, 296, 186);
  ObjectEvent_SetSpeed(LOCALID_MENARDI, PIXELS(1) / 10, PIXELS(1) / 20);
  ObjectEvent_WalkToAndWait(LOCALID_MENARDI, 292, 186);
  ObjectEvent_Get(LOCALID_MENARDI)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(LOCALID_MENARDI)->scaleX = -Q16_ONE;
  Script_PlaySong(SE_RATTLE);
  ObjectEvent_SetAnimation(LOCALID_MENARDI, MENARDI_ANIMATION_STAND_WINDSWEPT);
  Script_Wait(20);
  ObjectEvent_RunRepeatedMotion(LOCALID_MENARDI, 2);
  Script_Wait(20);
  ObjectEvent_RunRepeatedMotion(LOCALID_SATUROS, 1);
  Script_Wait(80);
  ObjectEvent_SetAnimationAndWait(LOCALID_MIA, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_MIA, 0, 20);
  ObjectEvent_RunRepeatedMotion(LOCALID_MENARDI, 2);
  Script_Wait(40);
  VenusLighthouseAerie_ShowMessage(LOCALID_MENARDI);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTHEAST, 40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_WEST, 20);

  objectEvent = ObjectEvent_Get(LOCALID_SATUROS);
  objectEvent->facing = FACING_SOUTHEAST + FACING_STEP;
  objectEvent->scaleX = Q16_ONE;
  ObjectEvent_SetAnimation(LOCALID_SATUROS, ANIM_STAND);
  Script_Wait(10);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_SATUROS,
                                       FACING_NORTH + FACING_STEP);
  ObjectEvent_Jump(LOCALID_SATUROS, 6, 0);
  Script_Wait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SHEBA, FACING_EAST, 0);
  star = (union ObjectEventOrEffect *)ObjectEvent_Create(
      SPRITE_BLANK, objectEvent->x.fixed, objectEvent->y.fixed + PIXELS(8),
      objectEvent->z.fixed);
  if (star != NULL) {
    sprite = star->objectEvent.sprite;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    sprite->image.priority = 0;
    star->objectEvent.priorityFlags &= ~1;
    star->objectEvent.motionFlags = 0;
    star->objectEvent.retainSpriteCache = 1;
    star->objectEvent.speed = PIXELS(8) / 5;
    star->objectEvent.acceleration = 0xcccc;
    iconPixels = Heap_Allocate(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
    // Display the Venus Star icon before sending it toward the beacon.
    Item_LoadIcon(ITEM_VENUS_STAR);
    Vram_Load(sprite->slot, 128, iconPixels + 0x400);
    Heap_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
  ObjectEvent_SetSpritePriority(LOCALID_VENUS_STAR_SHADOW, 1);
  starShadow = ObjectEvent_Get(LOCALID_VENUS_STAR_SHADOW);
  starShadow->x.fixed = objectEvent->x.fixed;
  starShadow->y.fixed = PIXELS(32);
  starShadow->z.fixed = objectEvent->z.fixed;
  starShadow->motionFlags = OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
  starShadow->speed = PIXELS(8) / 5;
  starShadow->acceleration = PIXELS(4) / 5;
  starShadow->scaleX = 3 * Q16_ONE / 4;
  starShadow->scaleY = 3 * Q16_ONE / 4;
  if (star != NULL) {
    star->objectEvent.motionFlags = OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
    star->effect.velocity.y = PIXELS(3) / 5;
    star->effect.velocity.x = PIXELS(4) / 5;
    star->objectEvent.velocityY = PIXELS(8);
    Object_SetPosition(&star->objectEvent, PIXELS(308), PIXELS(32),
                       PIXELS(164));
  }
  ObjectEvent_MoveToAndWait(LOCALID_VENUS_STAR_SHADOW, 308, 164);
  ObjectEvent_SetPosition(LOCALID_VENUS_STAR_SHADOW, 0, 0);
  ObjectEvent_SetSpritePriority(LOCALID_FELIX, 0);
  if (star != NULL) {
    Script_PlaySong(SE_STAR_BOUNCE);
    ObjectEvent_SetSpriteFlags(&star->objectEvent, 0);
    star->objectEvent.velocityY = PIXELS(4);
    Object_SetPosition(&star->objectEvent, PIXELS(314), PIXELS(32),
                       PIXELS(137));
    Object_WaitForMove(&star->objectEvent);
    Script_PlaySong(SE_STAR_BOUNCE);
    star->objectEvent.sprite->image.priority = 1;
    star->objectEvent.velocityY = PIXELS(6);
    Object_SetPosition(&star->objectEvent, PIXELS(285), PIXELS(32),
                       PIXELS(146));
    Object_WaitForMove(&star->objectEvent);
    Script_PlaySong(SE_STAR_BOUNCE);
    star->objectEvent.velocityY = PIXELS(5);
    Object_SetPosition(&star->objectEvent, PIXELS(300), PIXELS(32),
                       PIXELS(154));
    Object_WaitForMove(&star->objectEvent);
    Task_Wait(6);
    star->objectEvent.x.fixed = 0;
    star->objectEvent.y.fixed = 0;
    star->objectEvent.z.fixed = 0;
    VenusLighthouseAerie_StopObjectEventMotion(&star->objectEvent);
  }
  // Let the group react as the Venus Star reaches the beacon.
  VenusLighthouseAerie_ShowGroupEmote(EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  ObjectEvent_SetSpritePriority(LOCALID_FELIX, 1);
  ObjectEvent_Get(LOCALID_FELIX)->priorityFlags |= OBJ_EVENT_PRIORITY_AUTOMATIC;
  ObjectEvent_StartRepeatedMotion(LOCALID_IVAN, 2);
  VenusLighthouseAerie_ShowMessage(LOCALID_IVAN);
  ObjectEvent_RunRepeatedMotion(LOCALID_MIA, 1);
  Script_ShowMessageAndWait(LOCALID_MIA, 0, 40);
  ObjectEvent_RunRepeatedMotion(LOCALID_SATUROS, 1);
  Script_Wait(20);
  VenusLighthouseAerie_ShowMessage(LOCALID_SATUROS);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_SHEBA, FACING_SOUTHEAST + FACING_STEP, 0);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_FELIX,
                                       FACING_SOUTHEAST + FACING_STEP);
  VenusLighthouseAerie_ShowGroupEmote(EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  VenusLighthouseAerie_ShowMessage(LOCALID_SATUROS);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST, 40);
  Script_OpenMessage(LOCALID_GARET, 0);
  if (Script_ChooseYesNo(0, 0) == 0) {
    ObjectEvent_SetAnimation(LOCALID_GARET, ANIM_NOD);
    answeredYes = TRUE;
  } else {
    gFieldWork->message++;
    ObjectEvent_SetAnimation(LOCALID_GARET, ANIM_SHAKE_HEAD);
    answeredYes = FALSE;
  }
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  if (answeredYes) {
    gFieldWork->message++;
  }
  Script_Wait(20);

  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_BEAM_BOTTOM), 0);
  ObjectEvent_SetSpriteColorById(LOCALID_BEAM_BOTTOM, SPRITE_COLOR_WHITE);
  objectEvent = ObjectEvent_Get(LOCALID_BEAM_BOTTOM);
  objectEvent->scaleY = -Q16_ONE;
  objectEvent->scaleX = Q16_ONE / 10;
  objectEvent->motionFlags = 0;
  objectEvent->y.fixed = PIXELS(64);
  objectEvent->x.fixed = PIXELS(304);
  objectEvent->z.fixed = PIXELS(158);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_BEAM_LOWER), 0);
  ObjectEvent_SetSpriteColorById(LOCALID_BEAM_LOWER, SPRITE_COLOR_WHITE);
  objectEvent = ObjectEvent_Get(LOCALID_BEAM_LOWER);
  objectEvent->scaleY = -Q16_ONE;
  objectEvent->scaleX = Q16_ONE / 10;
  objectEvent->motionFlags = 0;
  objectEvent->y.fixed = PIXELS(96);
  objectEvent->x.fixed = PIXELS(304);
  objectEvent->z.fixed = PIXELS(158);
  VenusLighthouseAerie_ShowGroupEmote(EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SHEBA, FACING_EAST, 0);
  ObjectEvent_SetActionScript(LOCALID_BEAM_BOTTOM,
                              gVenusLighthouseAerie_BeamOpenAction);
  ObjectEvent_SetActionScript(LOCALID_BEAM_LOWER,
                              gVenusLighthouseAerie_BeamOpenAction);
  Script_PlaySong(SE_CRASH);
  MapGrid_SetShake(PIXELS(6), PIXELS(6), Q16_ONE);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
  PaletteFade_Interpolate(16);
  Task_Wait(20);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(24);
  Task_Wait(60);
  Task_AddCallback(VenusLighthouseAerie_SpawnUpperOrbitParticles,
                   TASK_PRIORITY_MAP);
  Script_PlaySong(SE_RUMBLE);
  MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
  PaletteFade_Interpolate(120);
  ObjectEvent_SetActionScript(LOCALID_BEAM_BOTTOM,
                              gVenusLighthouseAerie_BeamNarrowAction);
  ObjectEvent_SetActionScript(LOCALID_BEAM_LOWER,
                              gVenusLighthouseAerie_BeamNarrowAction);
  Task_Wait(120);
  MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 16, 12), PALETTES_ALL);
  PaletteFade_Interpolate(120);
  Task_Wait(120);
  Script_PlaySong(MUS_ERUPTION);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_Interpolate(120);
  Task_Wait(120);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);

  ObjectEvent_SetAnimation(LOCALID_MENARDI, ANIM_STAND);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_EAST, 0);
  ObjectEvent_Get(LOCALID_MENARDI)->scaleX = Q16_ONE;
  ObjectEvent_Jump(LOCALID_MENARDI, 4, 40);
  ObjectEvent_RunRepeatedMotion(LOCALID_MENARDI, 1);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SHEBA, FACING_SOUTHEAST + FACING_STEP, 20);
  ObjectEvent_RunRepeatedMotion(LOCALID_SATUROS, 1);
  VenusLighthouseAerie_ShowMessage(LOCALID_SATUROS);
  ObjectEvent_RunRepeatedMotion(LOCALID_MIA, 1);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MIA, FACING_WEST);
  VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_SATUROS, FACING_EAST);
  ObjectEvent_SetAnimationAndWait(LOCALID_SATUROS, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(LOCALID_SATUROS);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  ObjectEvent_StartRepeatedMotion(LOCALID_MENARDI, 2);
  VenusLighthouseAerie_ShowMessage(LOCALID_MENARDI);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_GARET, FACING_SOUTHWEST);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_SATUROS,
                                       FACING_NORTH + FACING_STEP);
  ObjectEvent_SetAnimationAndWait(LOCALID_SATUROS, ANIM_NOD);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
  ObjectEvent_RunRepeatedMotion(LOCALID_GARET, 1);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTHWEST + FACING_STEP,
                            20);
  ObjectEvent_RunRepeatedMotion(LOCALID_MENARDI, 1);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectEvent_StartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
  ObjectEvent_StartRepeatedMotion(LOCALID_GARET, 1);
  ObjectEvent_StartRepeatedMotion(LOCALID_IVAN, 1);
  ObjectEvent_RunRepeatedMotion(LOCALID_MIA, 1);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHWEST, 0);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MIA, FACING_NORTHWEST);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTHEAST, 40);
  ObjectEvent_RunRepeatedMotion(LOCALID_SATUROS, 1);
  VenusLighthouseAerie_ShowMessage(LOCALID_SATUROS);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_WEST, 20);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_SATUROS,
                                       FACING_NORTHWEST + FACING_STEP);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  ObjectEvent_SetAnimationAndWait(LOCALID_FELIX, ANIM_NOD);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_NORTHWEST + FACING_STEP, 60);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 40);
  ObjectEvent_RunRepeatedMotion(LOCALID_MENARDI, 1);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_EAST, 40);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTH + FACING_STEP, 40);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_SATUROS,
                                       FACING_NORTHWEST + FACING_STEP);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 40);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 20);
  ObjectEvent_SetAnimationAndWait(LOCALID_FELIX, ANIM_NOD);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_EAST, 40);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTHWEST + FACING_STEP,
                            20);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_FELIX,
                                       FACING_SOUTHEAST + FACING_STEP);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP,
                            20);
  ObjectEvent_SetAnimation(LOCALID_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 80);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_SetAnimationAndWait(LOCALID_MENARDI, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  ObjectEvent_StartRepeatedMotion(LOCALID_FELIX, 2);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_SetAnimation(LOCALID_MENARDI, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectEvent_RunRepeatedMotion(LOCALID_FELIX, 1);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  ObjectEvent_SetAnimation(LOCALID_SATUROS, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS, 0, 40);
  ObjectEvent_RunRepeatedMotion(LOCALID_FELIX, 1);
  Script_Wait(20);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTHWEST, 20);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_SOUTHEAST + FACING_STEP,
                            20);
  ObjectEvent_SetAnimation(LOCALID_MENARDI, ANIM_NOD);
  ObjectEvent_SetAnimationAndWait(LOCALID_SATUROS, ANIM_NOD);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_NORTHWEST + FACING_STEP, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | LOCALID_FELIX);
  ObjectEvent_SetSpeed(LOCALID_SHEBA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_FELIX, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetActionScript(LOCALID_FELIX,
                              gVenusLighthouseAerie_WalkOutAction);
  Script_Wait(20);
  ObjectEvent_SetActionScript(LOCALID_SHEBA,
                              gVenusLighthouseAerie_WalkOutAction);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_GARET, FACING_SOUTHWEST);
  ObjectEvent_StartRepeatedMotion(LOCALID_GARET, 2);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  // Flash Saturos and Menardi and release their matching radial bursts.
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(13, 8, 9), PALETTES_BG);
  PaletteFade_Interpolate(40);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_SATUROS,
                                       FACING_NORTH + FACING_STEP);
  VenusLighthouseAerie_ShowMessage(LOCALID_SATUROS);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_WEST, 0);
  VenusLighthouseAerie_FlashScreen();
  ObjectEvent_SetSpriteColorById(LOCALID_SATUROS, SPRITE_COLOR_WHITE);
  ObjectEvent_SetSpriteColorById(LOCALID_MENARDI, SPRITE_COLOR_WHITE);
  Script_Wait(20);
  ObjectEvent_SetSpriteColorById(LOCALID_SATUROS, CHILD_VALUE_BLINK);
  ObjectEvent_SetSpriteColorById(LOCALID_MENARDI, CHILD_VALUE_BLINK);
  Script_Wait(20);
  VenusLighthouseAerie_FlashScreen();
  ObjectEvent_Jump(LOCALID_MIA, 2, 20);
  VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MENARDI, FACING_EAST);
  VenusLighthouseAerie_ShowMessage(LOCALID_MENARDI);
  VenusLighthouseAerie_FlashScreen();

  menardi = ObjectEvent_Get(LOCALID_MENARDI);
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
  Script_PlaySong(SE_POWER_UP);
  Task_Wait(6);
  VenusLighthouseAerie_FlashScreen();
  saturos = ObjectEvent_Get(LOCALID_SATUROS);
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
  Script_PlaySong(SE_POWER_UP);
  ObjectEvent_Jump(LOCALID_IVAN, 6, 20);
  Script_PlaySong(MUS_FUSION_DRAGON_BATTLE);
  VenusLighthouseAerie_ShowMessage(LOCALID_IVAN);
  ObjectEvent_SetAnimation(LOCALID_SATUROS, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(LOCALID_SATUROS);
  VenusLighthouseAerie_FlashScreen();
  ObjectEvent_SetSpeed(LOCALID_SATUROS, PIXELS(1) / 5, PIXELS(1) / 10);
  ObjectEvent_SetSpeed(LOCALID_MENARDI, PIXELS(1) / 5, PIXELS(1) / 10);
  ObjectEvent_Get(LOCALID_SATUROS)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(LOCALID_MENARDI)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetDestination(LOCALID_SATUROS, 294, 196);
  ObjectEvent_SetDestination(LOCALID_MENARDI, 294, 196);
  // Slide the two transformed object events together, then start the fusion
  // battle.
  Task_AddCallback(VenusLighthouseAerie_UpdateBeamObjectEvents,
                   TASK_PRIORITY_MAP);
  ObjectEvent_StartRepeatedMotion(LOCALID_GARET, 2);
  Script_ShowMessage(LOCALID_GARET, 0);
  GameFlag_Set(FLAG_VENUS_AERIE_FUSION_SLIDING);
  Script_ShowMessage(LOCALID_IVAN, 0);
  GameFlag_Set(FLAG_VENUS_AERIE_FUSION_GROWING);
  VenusLighthouseAerie_FlashScreen();
  Script_Wait(20);
  VenusLighthouseAerie_FlashScreen();
  Script_Wait(20);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  FieldScript_SetNextDestination((s32)&SceneId_VenusLighthouseAerie,
                                 VENUS_LIGHTHOUSE_AERIE_AFTER_SECOND_BATTLE);
  FieldScript_SetDefeatDestination((s32)&SceneId_VenusLighthouseAerie,
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
static void
VenusLighthouseAerie_UpdateUpperOrbit(union ObjectEventOrEffect *fieldObject) {
  struct ObjectEvent *effect = &fieldObject->objectEvent;
  struct ObjectEvent *center = ObjectEvent_GetById(LOCALID_BEAM_BOTTOM);
  u16 angle = effect->timer;

  effect->x.fixed = center->x.fixed + Trig_Cos(angle) * (effect->speed + 3);
  effect->z.fixed = center->z.fixed + Trig_Sin(angle) * 2;
  effect->targetX = effect->x.fixed;
  effect->targetZ = effect->z.fixed;
  effect->timer -= 0x800;
}

/// Orbits a particle around the rising beam using its rise counter as a radius.
static void
VenusLighthouseAerie_UpdateRisingOrbit(union ObjectEventOrEffect *fieldObject) {
  struct ObjectEvent *objectEvent = &fieldObject->objectEvent;
  struct ObjectEvent *anchor = ObjectEvent_GetById(LOCALID_BEACON_ORB);
  s32 angle = (u16)objectEvent->timer;
  s32 cosine;
  s32 sine;
  s32 along;
  s32 across;

  cosine = Trig_Cos(angle);
  along = anchor->x.fixed +
          cosine * (objectEvent->speed + objectEvent->riseCounter + 6);
  objectEvent->x.fixed = along;

  sine = Trig_Sin(angle);
  across = anchor->z.fixed + sine * (objectEvent->riseCounter + 4);
  objectEvent->z.fixed = across;

  objectEvent->targetX = objectEvent->x.fixed;
  objectEvent->targetZ = across;

  {
    s32 next = (u16)objectEvent->timer;
    next += 0xfffff800;
    objectEvent->timer = next;
  }
}

/// Emits rising particles around the upper beam at the current beacon stage.
static void VenusLighthouseAerie_SpawnUpperOrbitParticles(void) {
  struct ObjectEvent *center;
  struct ObjectEvent *effect;
  struct Sprite *sprite;
  s32 angle;

  if (Script_GetFlag(FLAG_VENUS_AERIE_BEACON_ERUPTED) == 0 &&
      Math_RemainderUnsigned(gFrameCount, 3) != 0) {
    return;
  }
  center = ObjectEvent_Get(LOCALID_BEAM_BOTTOM);
  if (GameFlag_Get(FLAG_VENUS_AERIE_BEACON_ERUPTED) != 0) {
    effect = ObjectEvent_Create(SPRITE_PARTICLE, center->x.fixed,
                                (s32)((u32)(Random_Next() << 8) >> 16 << 16) +
                                    center->y.fixed - PIXELS(28),
                                center->z.fixed);
  } else {
    effect = ObjectEvent_Create(SPRITE_PARTICLE, center->x.fixed,
                                (s32)((u32)(Random_Next() << 6) >> 16 << 16) +
                                    center->y.fixed - PIXELS(28),
                                center->z.fixed);
  }
  if (effect != 0) {
    sprite = effect->sprite;
    ObjectEvent_SetScript(effect, gVenusLighthouseAerie_OrbitParticleScript);
    ObjectEvent_SetSpriteColor(effect, SPRITE_COLOR_GRB);
    effect->motionFlags = 0;
    angle = Random_Next() & 0xffff000;
    effect->timer = angle;
    effect->work = 0;
    effect->update = VenusLighthouseAerie_UpdateUpperOrbit;
    effect->speed = Trig_Sin((u32)(Random_Next() * 0xffff) >> 20) * 24 >> 16;
    sprite->flags = 0;
    sprite->image.priority = 1;
  }
}

/// Tests whether all three movement targets are unset.
static __inline__ s32 ObjectEvent_IsParked(struct ObjectEvent *objectEvent) {
  if (objectEvent->targetX == OBJ_EVENT_NO_TARGET &&
      objectEvent->targetY == objectEvent->targetX &&
      objectEvent->targetZ == objectEvent->targetY) {
    return 1;
  }
  return 0;
}

/// Pulses and slides Saturos and Menardi while their beam flags are active.
static void VenusLighthouseAerie_UpdateBeamObjectEvents(void) {
  struct ObjectEvent *saturos;
  struct ObjectEvent *menardi;
  s32 pairParked;

  saturos = ObjectEvent_GetById(LOCALID_SATUROS);
  menardi = ObjectEvent_GetById(LOCALID_MENARDI);
  pairParked =
      ObjectEvent_IsParked(saturos); // Overwritten; only Saturos's object
                                     // event's test loads remain in the ROM.
  pairParked = ObjectEvent_IsParked(menardi);
  if (pairParked != 0) {
    saturos->facing = 0;
    menardi->facing = 0;
    if (GameFlag_Get(FLAG_VENUS_AERIE_FUSION_GROWING) != 0) {
      ObjectEvent_ConfigureSpriteColor(LOCALID_SATUROS, SPRITE_COLOR_WHITE);
      ObjectEvent_ConfigureSpriteColor(LOCALID_MENARDI, SPRITE_COLOR_WHITE);
      if (saturos->scaleX < 5 * Q16_ONE / 4) {
        saturos->scaleX += Q16_ONE / 128;
        saturos->scaleY += Q16_ONE / 128;
        menardi->scaleX += Q16_ONE / 128;
        menardi->scaleY += Q16_ONE / 128;
      }
    } else {
      if ((*(s32 *)&gFrameCount & 2) != 0) {
        ObjectEvent_ConfigureSpriteColor(LOCALID_SATUROS, SPRITE_COLOR_HIDDEN);
        ObjectEvent_ConfigureSpriteColor(LOCALID_MENARDI, SPRITE_COLOR_NORMAL);
      } else {
        ObjectEvent_ConfigureSpriteColor(LOCALID_SATUROS, SPRITE_COLOR_NORMAL);
        ObjectEvent_ConfigureSpriteColor(LOCALID_MENARDI, SPRITE_COLOR_HIDDEN);
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
  FieldScript_Start();
  VenusLighthouseAerie_ShowSaturosAndMenardiFall();
  VenusLighthouseAerie_DiscussSaturosAndMenardiDefeat();
  VenusLighthouseAerie_EruptBeacon();
  FieldScript_End();
}

/// A look back from the world map (VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT,
/// after the world map's lighthouse transition): Felix cries out as Sheba
/// clings to the broken aerie, then the world map returns with its music still
/// playing (VENUS_LIGHTHOUSE_AERIE_TO_WORLD_MAP).
static void VenusLighthouseAerie_ShowShebaInDanger(void) {
  FieldScript_Start();
  VenusLighthouseAerie_RestorePartyOnBrokenAerie();
  VenusLighthouseAerie_TryToPreventFall();
  GameFlag_Set(FLAG_KEEP_MUSIC);

  gFieldWork->transition = MAP_TRANSITION(TRANSITION_WINDOW, 0);
  gFieldWork->transitionFrames = 24;

  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Import_ScriptRequestWarp(VENUS_LIGHTHOUSE_AERIE_TO_WORLD_MAP);
  FieldScript_End();
}

/// Back from the world map (VENUS_LIGHTHOUSE_AERIE_FROM_WORLD_MAP): stages the
/// broken aerie for Sheba's fall and the party's escape, plays them, sets
/// FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED and leaves for Lalivero
/// (VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_C).
static void VenusLighthouseAerie_StageShebasFall(void) {
  struct ObjectEvent *leader;
  struct ObjectEvent *objectEvent;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  FieldScript_Start();
  FieldCamera_GetScriptObject()->motionFlags = 0;
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
  ObjectEvent_DestroyById(LOCALID_SATUROS);
  ObjectEvent_DestroyById(LOCALID_MENARDI);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                 ANIM_LEADER_KNOCKED_DOWN);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_PARTY_LEADER), 0);
  leader->x.fixed = PIXELS(346);
  leader->z.fixed = PIXELS(205);
  leader->y.fixed = PIXELS(32);
  leader->facing = FACING_SOUTHWEST;
  VenusLighthouseAerie_StopObjectEventMotion(leader);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_PARTY_KNOCKED_DOWN);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_GARET), 0);
  objectEvent = ObjectEvent_Get(LOCALID_GARET);
  objectEvent->x.fixed = PIXELS(356);
  objectEvent->z.fixed = PIXELS(192);
  objectEvent->facing = FACING_NORTHWEST;
  objectEvent->y.fixed = PIXELS(32);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_PARTY_KNOCKED_DOWN);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_IVAN), 0);
  objectEvent = ObjectEvent_Get(LOCALID_IVAN);
  objectEvent->x.fixed = PIXELS(360);
  objectEvent->facing = FACING_SOUTHEAST;
  objectEvent->y.fixed = PIXELS(32);
  objectEvent->z.fixed = PIXELS(222);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_PARTY_KNOCKED_DOWN);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_MIA), 0);
  objectEvent = ObjectEvent_Get(LOCALID_MIA);
  objectEvent->x.fixed = PIXELS(334);
  objectEvent->facing = FACING_WEST;
  objectEvent->y.fixed = PIXELS(32);
  objectEvent->z.fixed = PIXELS(222);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  ObjectEvent_SetPosition(LOCALID_FELIX, PIXELS(196), PIXELS(220));
  Import_ObjectEventSetAnimation(LOCALID_FELIX, FELIX_ANIMATION_KNEEL);
  ObjectEvent_SetPosition(LOCALID_SHEBA, PIXELS(188), PIXELS(316));
  Import_ObjectEventSetAnimation(LOCALID_SHEBA, SHEBA_ANIMATION_HANG_ONE_HAND);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_SHEBA), 0);
  objectEvent = ObjectEvent_Get(LOCALID_BEACON_STATUE_SOUTHWEST);
  objectEvent->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_BEACON_STATUE_NORTHWEST);
  objectEvent->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_Get(LOCALID_BEACON_STATUE_NORTHEAST);
  objectEvent->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_BEACON_STATUE_SOUTHEAST);
  objectEvent->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_BEACON_ORB), 0);
  ObjectEvent_GetById(LOCALID_BEACON_ORB)->motionFlags = OBJ_EVENT_FLOATS;
  ObjectEvent_SetSpriteColorById(LOCALID_BEACON_ORB, SPRITE_COLOR_GBR);
  ObjectEvent_Get(LOCALID_BEACON_ORB)->y.fixed = PIXELS(40);
  FieldScheduler_AddCallback(VenusLighthouseAerie_SpawnSideOrbitParticles,
                             TASK_PRIORITY_MAP);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_WINDOW, 0);
  gFieldWork->transitionFrames = 24;
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(40);
  VenusLighthouseAerie_PlayShebasFall();
  GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED);
  Import_ScriptRequestWarp(VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_C);
}

/// Shows Saturos and Menardi falling into the beacon after their final defeat.
static void VenusLighthouseAerie_ShowSaturosAndMenardiFall(void) {
  struct ObjectEvent *objectEvent;

  Script_PlaySong(SE_RUMBLE);
  MapGrid_CopyCellAttributes(17, 10, 4, 2, 17, 8);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->facing = FACING_NORTHWEST;
  ObjectEvent_Get(LOCALID_GARET)->facing = FACING_NORTHWEST;
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(328), PIXELS(168));
  ObjectEvent_Get(LOCALID_IVAN)->facing = FACING_NORTHWEST;
  ObjectEvent_SetPosition(LOCALID_IVAN, PIXELS(340), PIXELS(196));
  ObjectEvent_Get(LOCALID_MIA)->facing = FACING_NORTHWEST;
  ObjectEvent_SetPosition(LOCALID_MIA, PIXELS(326), PIXELS(204));
  ObjectEvent_Get(LOCALID_FELIX)->facing = FACING_NORTH + FACING_STEP;
  ObjectEvent_SetPosition(LOCALID_FELIX, PIXELS(200), PIXELS(216));
  ObjectEvent_Get(LOCALID_SHEBA)->facing = FACING_NORTH + FACING_STEP;
  ObjectEvent_SetPosition(LOCALID_SHEBA, PIXELS(200), PIXELS(216));
  ObjectEvent_Get(LOCALID_SATUROS)->facing = FACING_SOUTHEAST + FACING_STEP;
  ObjectEvent_SetPosition(LOCALID_SATUROS, PIXELS(310), PIXELS(158));
  ObjectEvent_Get(LOCALID_MENARDI)->facing = FACING_SOUTHEAST + FACING_STEP;
  ObjectEvent_SetPosition(LOCALID_MENARDI, PIXELS(292), PIXELS(158));

  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_BEAM_BOTTOM), 0);
  ObjectEvent_SetSpriteColorById(LOCALID_BEAM_BOTTOM, SPRITE_COLOR_WHITE);
  ObjectMotion_SetActionVariant(LOCALID_BEAM_BOTTOM, 1);
  objectEvent = ObjectEvent_Get(LOCALID_BEAM_BOTTOM);
  objectEvent->scaleY = -Q16_ONE;
  objectEvent->scaleX = Q16_ONE / 5;
  objectEvent->motionFlags = 0;
  objectEvent->x.fixed = PIXELS(304);
  objectEvent->y.fixed = PIXELS(2);
  objectEvent->z.fixed = PIXELS(96);

  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_BEAM_LOWER), 0);
  ObjectEvent_SetSpriteColorById(LOCALID_BEAM_LOWER, SPRITE_COLOR_WHITE);
  ObjectMotion_SetActionVariant(LOCALID_BEAM_LOWER, 1);
  objectEvent = ObjectEvent_Get(LOCALID_BEAM_LOWER);
  objectEvent->scaleY = -Q16_ONE;
  objectEvent->scaleX = Q16_ONE / 5;
  objectEvent->motionFlags = 0;
  objectEvent->x.fixed = PIXELS(304);
  objectEvent->y.fixed = PIXELS(34);
  objectEvent->z.fixed = PIXELS(96);

  FieldScheduler_AddCallback(VenusLighthouseAerie_SpawnUpperOrbitParticles,
                             TASK_PRIORITY_MAP);
  FieldCamera_GetScriptObject()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 0);
  WaitFrames(1);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 2);
  FieldScript_WaitFrames(10);
  // Give Saturos and Menardi their final exchange before they fall.
  FieldScript_SetMessageId(MsgVenusPairDefeated);
  VenusLighthouseAerie_ShowMessage(LOCALID_SATUROS);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 3);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 40);
  Script_PlaySong(SONG_FADE_OUT_BGM);

  objectEvent = ObjectEvent_Get(LOCALID_SATUROS);
  objectEvent->x.fixed = PIXELS(308);
  objectEvent->y.fixed = PIXELS(28);
  objectEvent->z.fixed = PIXELS(152);
  Import_ObjectEventSetAnimation(LOCALID_SATUROS, SATUROS_ANIMATION_TIP);
  FieldScript_WaitFrames(20);
  objectEvent->x.fixed = PIXELS(306);
  objectEvent->y.fixed = PIXELS(28);
  objectEvent->z.fixed = PIXELS(152);
  Import_ObjectEventSetAnimation(LOCALID_SATUROS, SATUROS_ANIMATION_SLUMP);
  FieldScript_WaitFrames(12);
  objectEvent->x.fixed = PIXELS(304);
  objectEvent->y.fixed = PIXELS(21);
  objectEvent->z.fixed = PIXELS(152);
  Import_ObjectEventSetAnimation(LOCALID_SATUROS, SATUROS_ANIMATION_SINK);
  FieldScript_WaitFrames(8);
  ObjectEvent_DestroyById(LOCALID_SATUROS);

  objectEvent = ObjectEvent_Get(LOCALID_MENARDI);
  objectEvent->x.fixed = PIXELS(294);
  objectEvent->y.fixed = PIXELS(28);
  objectEvent->z.fixed = PIXELS(152);
  Import_ObjectEventSetAnimation(LOCALID_MENARDI, MENARDI_ANIMATION_TIP);
  FieldScript_WaitFrames(20);
  objectEvent->x.fixed = PIXELS(300);
  objectEvent->y.fixed = PIXELS(27);
  objectEvent->z.fixed = PIXELS(152);
  Import_ObjectEventSetAnimation(LOCALID_MENARDI, MENARDI_ANIMATION_SLUMP);
  FieldScript_WaitFrames(12);
  objectEvent->x.fixed = PIXELS(304);
  objectEvent->y.fixed = PIXELS(17);
  objectEvent->z.fixed = PIXELS(152);
  Import_ObjectEventSetAnimation(LOCALID_MENARDI, MENARDI_ANIMATION_SINK);
  FieldScript_WaitFrames(8);
  ObjectEvent_DestroyById(LOCALID_MENARDI);
  FieldScript_WaitFrames(160);
}

/// Lets the party discuss the battle while Felix and Sheba remain on the aerie.
static void VenusLighthouseAerie_DiscussSaturosAndMenardiDefeat(void) {
  s32 answerAccepted;
  s32 garetSpeaker;
  s32 miaSpeaker;

  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_GARET, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
  FieldScript_WaitFrames(20);
  Script_OpenMessage(LOCALID_IVAN, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_PARTY_LEADER, FACING_SOUTH);
  answerAccepted = 0;
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_SHAKE_HEAD);
    answerAccepted = 1;
  } else {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
    gFieldWork->message += 1;
  }
  VenusLighthouseAerie_ShowMessage(LOCALID_IVAN);
  if (answerAccepted != 0) {
    gFieldWork->message += 1;
  }
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  garetSpeaker = SPEAKER_PORTRAIT_LEFT | LOCALID_GARET;
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 321, 174);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_GARET, FACING_SOUTHEAST);
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHWEST, 0);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MIA, FACING_NORTH);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_GARET, FACING_SOUTH);
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST, 20);
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 40);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_GARET, 0, 40);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST, 20);
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH, 80);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_SOUTHWEST, 80);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_IVAN, FACING_NORTH);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTHWEST, 40);
  ObjectMotion_Launch(LOCALID_IVAN, 4, 60);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_GARET, FACING_SOUTHEAST);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST, 40);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectMotion_Launch(LOCALID_GARET, 4, 40);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  ObjectEvent_SetSpeed(LOCALID_FELIX, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetDestination(LOCALID_FELIX, 200, 188);
  ObjectEvent_SetDestination(LOCALID_SHEBA, 200, 204);
  FieldCamera_SetSpeed(PIXELS(16) / 5, PIXELS(2) / 5);
  FieldCamera_MoveTo(PIXELS(252), 0, PIXELS(190), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(40);
  Script_PlaySong(MUS_DANGER);
  ObjectEvent_SetAttachedEffect(LOCALID_FELIX, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 1);
  FieldScript_WaitFrames(20);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 20);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_SetAttachedEffect(LOCALID_SHEBA, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  miaSpeaker = SPEAKER_PORTRAIT_RIGHT | LOCALID_MIA;
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(LOCALID_MIA, 2);
  VenusLighthouseAerie_ShowMessage(miaSpeaker);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 2);
  FieldScript_WaitFrames(20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_IVAN);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_FELIX, FACING_NORTHEAST);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 1);
  Script_ShowMessageAndWait(LOCALID_FELIX, 0, 20);
  VenusLighthouseAerie_ShowMessage(miaSpeaker);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_SetAttachedEffect(LOCALID_IVAN, ATTACHED_EFFECT_IN_FRONT |
                                                  ATTACHED_EFFECT_ONCE);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_IVAN);
  Import_ObjectEventSetAnimation(LOCALID_FELIX, ANIM_NOD);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  Import_ObjectEventSetAnimation(LOCALID_FELIX, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTH + FACING_STEP, 20);
  ObjectMotion_Launch(LOCALID_MIA, 4, 20);
  Script_ShowMessageAndWait(miaSpeaker, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
}

static s32 sTransitionTimer;
static s32 sTransitionStep;

/// Raises the beacon effects and splits the aerie beneath the group.
static void VenusLighthouseAerie_EruptBeacon(void) {
  struct ObjectEvent *lower;
  struct ObjectEvent *middle;
  struct ObjectEvent *upper;
  struct ObjectEvent *objectEvent;

  Sound_PlaySong(MUS_SILENCE);
  Sound_PlaySong(SE_STOP_ACTION_SE);
  MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SHEBA, FACING_NORTH + FACING_STEP, 0);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  FieldScript_WaitFrames(10);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  FieldCamera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 1);
  FieldCamera_WaitForArrival();
  // Open the beacon beam before splitting the surrounding platform.
  ObjectEvent_GetById(LOCALID_BEAM_BOTTOM)->scaleX = Q16_ONE / 10;
  ObjectEvent_GetById(LOCALID_BEAM_LOWER)->scaleX = Q16_ONE / 10;
  ObjectEvent_SetActionScript(LOCALID_BEAM_BOTTOM,
                              gVenusLighthouseAerie_BeamOpenAction);
  ObjectEvent_SetActionScript(LOCALID_BEAM_LOWER,
                              gVenusLighthouseAerie_BeamOpenAction);
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
  ObjectEvent_GetById(LOCALID_BEAM_BOTTOM)->y.fixed = -PIXELS(96);
  ObjectEvent_GetById(LOCALID_BEAM_LOWER)->y.fixed = -PIXELS(64);
  ObjectEvent_ConfigureSpriteColor(LOCALID_BEAM_MIDDLE, SPRITE_COLOR_WHITE);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_BEAM_MIDDLE), 0);
  // Stack three more beam segments under the rising upper pair.
  lower = ObjectEvent_GetById(LOCALID_BEAM_MIDDLE);
  lower->scaleY = -Q16_ONE;
  lower->scaleX = ObjectEvent_GetById(LOCALID_BEAM_BOTTOM)->scaleX;
  lower->motionFlags = 0;
  lower->x.fixed = PIXELS(304);
  lower->y.fixed = -PIXELS(32);
  lower->z.fixed = PIXELS(96);
  ObjectEvent_ConfigureSpriteColor(LOCALID_BEAM_UPPER, SPRITE_COLOR_WHITE);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_BEAM_UPPER), 0);
  middle = ObjectEvent_GetById(LOCALID_BEAM_UPPER);
  middle->scaleY = -Q16_ONE;
  middle->scaleX = ObjectEvent_GetById(LOCALID_BEAM_BOTTOM)->scaleX;
  middle->motionFlags = 0;
  middle->x.fixed = PIXELS(304);
  middle->y.fixed = 0;
  middle->z.fixed = PIXELS(96);
  ObjectEvent_ConfigureSpriteColor(LOCALID_BEAM_TOP, SPRITE_COLOR_WHITE);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_BEAM_TOP), 0);
  upper = ObjectEvent_GetById(LOCALID_BEAM_TOP);
  upper->scaleY = -Q16_ONE;
  upper->scaleX = ObjectEvent_GetById(LOCALID_BEAM_BOTTOM)->scaleX;
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
  objectEvent = ObjectEvent_GetById(LOCALID_BEACON_STATUE_SOUTHWEST);
  objectEvent->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_BEACON_STATUE_NORTHWEST);
  objectEvent->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_BEACON_STATUE_NORTHEAST);
  objectEvent->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_BEACON_STATUE_SOUTHEAST);
  objectEvent->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  objectEvent->x.fixed += PIXELS(16);
  objectEvent->z.fixed += PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_GARET);
  objectEvent->x.fixed += PIXELS(16);
  objectEvent->z.fixed += PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_IVAN);
  objectEvent->x.fixed += PIXELS(16);
  objectEvent->z.fixed += PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_MIA);
  objectEvent->x.fixed += PIXELS(16);
  objectEvent->z.fixed += PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  ObjectEvent_SetPosition(LOCALID_FELIX, PIXELS(196), PIXELS(220));
  Import_ObjectEventSetAnimation(LOCALID_FELIX, FELIX_ANIMATION_KNEEL);
  ObjectEvent_SetPosition(LOCALID_SHEBA, PIXELS(188), PIXELS(316));
  Import_ObjectEventSetAnimation(LOCALID_SHEBA, SHEBA_ANIMATION_HANG_ONE_HAND);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_SHEBA), 0);
  MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  PaletteFade_SetTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
  PaletteFade_StartInterpolation(120);
  ObjectEvent_SetActionScript(LOCALID_BEAM_BOTTOM,
                              gVenusLighthouseAerie_BeamRiseAction);
  ObjectEvent_SetActionScript(LOCALID_BEAM_LOWER,
                              gVenusLighthouseAerie_BeamRiseAction);
  ObjectEvent_SetActionScript(LOCALID_BEAM_MIDDLE,
                              gVenusLighthouseAerie_BeamRiseAction);
  ObjectEvent_SetActionScript(LOCALID_BEAM_UPPER,
                              gVenusLighthouseAerie_BeamRiseAction);
  ObjectEvent_SetActionScript(LOCALID_BEAM_TOP,
                              gVenusLighthouseAerie_BeamRiseAction);
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
  ObjectEvent_GetById(LOCALID_BEAM_BOTTOM)->scaleY = Q16_ONE / 50;
  ObjectEvent_SetActionScript(LOCALID_BEAM_LOWER,
                              gVenusLighthouseAerie_BeamCloseAction);
  ObjectEvent_SetActionScript(LOCALID_BEAM_MIDDLE,
                              gVenusLighthouseAerie_BeamCloseAction);
  ObjectEvent_SetActionScript(LOCALID_BEAM_UPPER,
                              gVenusLighthouseAerie_BeamCloseAction);
  ObjectEvent_RunActionScriptAndWait(LOCALID_BEAM_TOP,
                                     gVenusLighthouseAerie_BeamCloseAction);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  ObjectEvent_ConfigureSpriteColor(LOCALID_BEAM_BOTTOM, SPRITE_COLOR_HIDDEN);
  FieldScript_WaitFrames(20);
  ObjectEvent_RunActionScriptAndWait(LOCALID_BEAM_BOTTOM,
                                     gVenusLighthouseAerie_BeamEndAction);
  Task_Remove(VenusLighthouseAerie_SpawnUpperOrbitParticles);
  ObjectMotion_Launch(LOCALID_IVAN, 2, 20);
  VenusLighthouseAerie_ShowMessage(LOCALID_IVAN);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST, 20);
  ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectMotion_SetPositionAndReset(LOCALID_MIA, 326, 220);
  FieldScript_WaitFrames(40);
  FieldEffect_ConfigureLinkedObject(LOCALID_MIA, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_ONCE);
  VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  objectEvent->riseCounter = 0;
  objectEvent->riseEnabled = 1;
  objectEvent->riseBaseY = objectEvent->y.fixed;
  objectEvent->facing = FACING_NORTHWEST;
  objectEvent = ObjectEvent_Get(LOCALID_GARET);
  objectEvent->riseCounter = 0;
  objectEvent->riseEnabled = 1;
  objectEvent->facing = FACING_NORTHWEST;
  objectEvent->riseBaseY = objectEvent->y.fixed;
  objectEvent = ObjectEvent_Get(LOCALID_IVAN);
  objectEvent->riseCounter = 0;
  objectEvent->riseEnabled = 1;
  objectEvent->facing = FACING_NORTHWEST;
  objectEvent->riseBaseY = objectEvent->y.fixed;
  objectEvent = ObjectEvent_Get(LOCALID_MIA);
  objectEvent->riseCounter = 0;
  objectEvent->riseEnabled = 1;
  objectEvent->facing = FACING_NORTHWEST;
  objectEvent->riseBaseY = objectEvent->y.fixed;
  objectEvent = ObjectEvent_Get(LOCALID_FELIX);
  objectEvent->riseCounter = 0;
  objectEvent->riseEnabled = 1;
  objectEvent->riseBaseY = objectEvent->y.fixed;
  objectEvent = ObjectEvent_Get(LOCALID_SHEBA);
  objectEvent->riseCounter = 0;
  objectEvent->riseEnabled = 1;
  objectEvent->riseBaseY = objectEvent->y.fixed;
  ObjectEvent_GetById(LOCALID_BEACON_ORB)->motionFlags = 0;
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_BEACON_ORB), 0);
  ObjectEvent_ConfigureSpriteColor(LOCALID_BEACON_ORB, SPRITE_COLOR_WHITE);
  ObjectMotion_SetActionVariant(LOCALID_BEACON_ORB, 2);
  sTransitionStep = 0;
  sTransitionTimer = 240;
  FieldScheduler_AddCallback(VenusLighthouseAerie_UpdateBeaconRise,
                             TASK_PRIORITY_MAP);
  do {
    WaitFrames(1);
  } while (GameFlag_Get(FLAG_VENUS_AERIE_PARTY_LIFTED) == 0);
  GameFlag_Set(FLAG_SKIP_MAP_CHANGE_SOUND);
  FieldScript_WaitFrames(30);
  GameFlag_Set(FLAG_KEEP_MUSIC);
  ObjectEvent_Snapshot();
  FieldScript_SetDestination((s32)&SceneId_WorldMap,
                             WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT);
  {
    u16 white = 0x7fff;

    PLTT_BACKDROP = white;
  }
  gFieldWork->transitionFrames = 1;
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
}

/// Places the party on the broken platform and resumes its falling effects.
static void VenusLighthouseAerie_RestorePartyOnBrokenAerie(void) {
  struct ObjectEvent *objectEvent;

  ObjectEvent_DestroyById(LOCALID_SATUROS);
  ObjectEvent_DestroyById(LOCALID_MENARDI);
  Script_PlaySong(SE_RUMBLE);
  MapGrid_CopyCellAttributes(17, 10, 4, 2, 17, 8);
  MapGrid_CopyCellsTo(102, 4, 74, 4, 18, 23);
  MapGrid_CopyCellsTo(39, 72, 11, 72, 16, 21);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 22, 6);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 13, 6);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 22, 13);
  MapGrid_CopyCellAttributes(19, 6, 3, 7, 13, 13);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  ObjectEvent_Restore();
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  Script_PlaySong(SE_ITEM_BREAK);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                 ANIM_LEADER_KNOCKED_DOWN);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_PARTY_KNOCKED_DOWN);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_PARTY_KNOCKED_DOWN);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_PARTY_KNOCKED_DOWN);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_PARTY_LEADER), 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_GARET), 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_IVAN), 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_MIA), 0);

  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  objectEvent->x.fixed = PIXELS(346);
  objectEvent->y.fixed = PIXELS(32);
  objectEvent->z.fixed = PIXELS(205);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent->riseEnabled = 0;
  objectEvent->velocityY = PIXELS(2);

  objectEvent = ObjectEvent_Get(LOCALID_GARET);
  objectEvent->x.fixed = PIXELS(356);
  objectEvent->y.fixed = PIXELS(32);
  objectEvent->z.fixed = PIXELS(192);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent->riseEnabled = 0;
  objectEvent->velocityY = PIXELS(2);

  objectEvent = ObjectEvent_Get(LOCALID_IVAN);
  objectEvent->x.fixed = PIXELS(360);
  objectEvent->y.fixed = PIXELS(32);
  objectEvent->z.fixed = PIXELS(222);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent->riseEnabled = 0;
  objectEvent->velocityY = PIXELS(2);

  objectEvent = ObjectEvent_Get(LOCALID_MIA);
  objectEvent->x.fixed = PIXELS(334);
  objectEvent->y.fixed = PIXELS(32);
  objectEvent->z.fixed = PIXELS(222);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent->riseEnabled = 0;
  objectEvent->velocityY = PIXELS(2);

  ObjectEvent_Get(LOCALID_FELIX)->riseCounter = 0;
  ObjectEvent_Get(LOCALID_SHEBA)->riseCounter = 0;
  ObjectEvent_Get(LOCALID_BEACON_ORB)->motionFlags |= 4;
  ObjectEvent_SetSpriteColorById(LOCALID_BEACON_ORB, SPRITE_COLOR_GBR);
  FieldScheduler_AddCallback(VenusLighthouseAerie_SpawnSideOrbitParticles,
                             TASK_PRIORITY_MAP);
  gFieldWork->transitionFrames = 1;
  FieldScript_StartScreenTransition();
  PaletteFade_ApplySource(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(40);
  WaitFrames(60);
}

/// Shows Felix urging Sheba to hold on at the edge of the platform.
static void VenusLighthouseAerie_TryToPreventFall(void) {
  FieldScript_SetMessageId(MsgVenusNoooo);
  VenusLighthouseAerie_ShowMessage(LOCALID_FELIX);
  Script_PlaySong(MUS_SHEBA_HANGS_ON);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  FieldCamera_SetSpeed(PIXELS(24) / 5, PIXELS(3) / 5);
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  FieldCamera_MoveTo(PIXELS(192), -PIXELS(64), PIXELS(238), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 1);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_FELIX, 0, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SHEBA, 3);
  VenusLighthouseAerie_ShowMessage(LOCALID_SHEBA);
  ObjectEvent_SetAttachedEffect(LOCALID_FELIX, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_FELIX, 0, 80);
  ObjectEvent_SetAttachedEffect(LOCALID_SHEBA, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVariantCallback(LOCALID_SHEBA, 2);
  VenusLighthouseAerie_ShowMessage(LOCALID_SHEBA);
}

/// Shows Sheba falling, Felix jumping after her and the party escaping: it
/// resolves to find Jenna and the others at Idejima, and a long search finds
/// no sign of Jenna.
static void VenusLighthouseAerie_PlayShebasFall(void) {
  struct ObjectEvent *objectEvent;
  s32 stepPending;

  stepPending = 0;
  // Sheba loses her grip while Felix tries to keep her on the platform.
  FieldScript_SetMessageId(MsgVenusWhyHappeningProtectVenusLighthouse);
  ObjectEvent_SetAttachedEffect(LOCALID_FELIX, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_FELIX, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SHEBA, 2);
  Script_ShowMessageAndWait(LOCALID_SHEBA, 0, 20);
  Import_ObjectEventSetAnimation(LOCALID_SHEBA, SHEBA_ANIMATION_HANG);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVariantCallback(LOCALID_FELIX, 2);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_FELIX, 0, 40);
  Import_ObjectEventSetAnimation(LOCALID_SHEBA, SHEBA_ANIMATION_SLIP);
  Script_ShowMessageAndWait(LOCALID_SHEBA, 0, 20);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SHEBA, 2);
  ObjectEvent_SetActionScript(LOCALID_SHEBA,
                              gVenusLighthouseAerie_StepBackAction);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetAttachedEffect(LOCALID_FELIX, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  ObjectMotion_SetVariantCallback(LOCALID_FELIX, 3);
  ObjectEvent_RefreshSelectorById(LOCALID_SHEBA);
  FieldScript_WaitFrames(160);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_GetById(LOCALID_FELIX)->facing = FACING_SOUTH + FACING_STEP;
  Import_ObjectEventSetAnimation(LOCALID_FELIX, 0);
  FieldScript_WaitFrames(80);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(40);
  Script_ShowMessageAndWait(LOCALID_FELIX, 0, 40);
  Import_ObjectEventSetAnimation(LOCALID_FELIX, FELIX_ANIMATION_KNEEL);
  FieldScript_WaitFrames(10);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_FELIX), 0);
  Import_ObjectEventSetAnimation(LOCALID_FELIX, 0);
  ObjectMotion_Launch(LOCALID_FELIX, 6, 0);
  ObjectEvent_SetSpeed(LOCALID_FELIX, PIXELS(3), PIXELS(3) / 2);
  ObjectEvent_GetById(LOCALID_FELIX)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  objectEvent = ObjectEvent_GetById(LOCALID_FELIX);
  // Felix jumps after Sheba; the remaining party watches from the far side.
  objectEvent->update = VenusLighthouseAerie_StepSpriteRotation;
  objectEvent->facing = FACING_WEST;
  ObjectEvent_MoveToAndWait(LOCALID_FELIX, 184, 237);
  ObjectEvent_RunActionScriptAndWait(LOCALID_FELIX,
                                     gVenusLighthouseAerie_LeapAction);
  FieldScript_WaitFrames(120);
  VenusLighthouseAerie_ShowMessage(LOCALID_PARTY_LEADER);
  Script_PlaySong(MUS_FELIX_LEAPS_AFTER_SHEBA);
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  FieldCamera_MoveTo(PIXELS(342), PIXELS(32), PIXELS(212), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  FieldScript_WaitFrames(40);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER), 1);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 6, 60);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->timer = 10;
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                              gVenusLighthouseAerie_ScaleStepAction);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  Script_ShowMessageAndWait(LOCALID_GARET, 0, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
  FieldScript_WaitFrames(20);
  VenusLighthouseAerie_ShowMessage(LOCALID_IVAN);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_GARET), 1);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  ObjectMotion_Launch(LOCALID_GARET, 6, 60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_IVAN), 1);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_STAND);
  ObjectMotion_Launch(LOCALID_IVAN, 6, 40);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_MIA), 1);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_STAND);
  ObjectMotion_Launch(LOCALID_MIA, 6, 60);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST, 60);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTHWEST, 20);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(LOCALID_IVAN);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_MIA, FACING_SOUTHWEST);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_MIA, 0, 80);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_GARET, FACING_SOUTH);
  Script_ShowMessageAndWait(LOCALID_GARET, 0, 40);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 20);
  Script_ShowMessageAndWait(LOCALID_IVAN, 0, 40);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTHWEST, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 1);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTHEAST, 20);
  VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_GARET, FACING_SOUTH);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 333, 194);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 333, 206);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  Script_ShowMessageAndWait(LOCALID_GARET, 0, 80);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_PARTY_LEADER);
  WaitFrames(1);
  objectEvent->scaleX = Q16_ONE;
  objectEvent->scaleY = Q16_ONE;
  FieldScript_WaitFrames(40);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_GARET, FACING_SOUTHEAST);
  ObjectMotion_Launch(LOCALID_GARET, 4, 0);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_GARET, FACING_NORTHEAST);
  Script_OpenMessage(LOCALID_GARET, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
    VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
    VenusLighthouseAerie_ShowMessage(LOCALID_IVAN);
    Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_SHAKE_HEAD);
    VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
    stepPending = 1;
  } else {
    gFieldWork->message += 3;
    FieldScript_WaitFrames(60);
    VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
    VenusLighthouseAerie_ShowMessage(LOCALID_IVAN);
    Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_SHAKE_HEAD);
    VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
  }
  if (stepPending != 0) {
    gFieldWork->message += 3;
  }
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_PARTY_LEADER, FACING_SOUTH);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(80);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 20);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 40);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 20);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(LOCALID_IVAN);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST, 20);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_WEST, 40);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 40);
  ObjectEvent_SetAttachedEffect(LOCALID_MIA, ATTACHED_EFFECT_IN_FRONT |
                                                 ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
  Script_PlaySong(SE_RUMBLE);
  MapGrid_SetShake(PIXELS(6), PIXELS(6), Q16_ONE);
  FieldScript_WaitFrames(40);
  Script_PlaySong(SE_CRASH);
  MapGrid_CopyCellsTo(110, 105, 74, 4, 18, 23);
  MapGrid_CopyCellsTo(92, 86, 83, 4, 8, 23);
  MapGrid_CopyCellsTo(75, 28, 75, 4, 8, 23);
  MapGrid_CopyCellsTo(92, 86, 11, 72, 16, 20);
  MapGrid_CopyCellsTo(19, 92, 19, 68, 8, 21);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  objectEvent->z.fixed -= PIXELS(32);
  objectEvent->work = 0;
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_GARET);
  objectEvent->x.fixed -= PIXELS(4);
  objectEvent->z.fixed -= PIXELS(32);
  objectEvent->work = 0;
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_IVAN);
  objectEvent->x.fixed -= PIXELS(4);
  objectEvent->z.fixed -= PIXELS(32);
  objectEvent->work = 0;
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_MIA);
  objectEvent->x.fixed -= PIXELS(4);
  objectEvent->z.fixed -= PIXELS(18);
  objectEvent->work = 0;
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  ObjectEvent_GetById(LOCALID_BEACON_ORB)->y.fixed = PIXELS(56);
  FieldCamera_MoveTo(PIXELS(338), PIXELS(32), PIXELS(180), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 6, 0);
  ObjectMotion_Launch(LOCALID_GARET, 6, 0);
  ObjectMotion_Launch(LOCALID_IVAN, 6, 0);
  ObjectMotion_Launch(LOCALID_MIA, 6, 0);
  // Shake the party and split away the remaining central floor tiles.
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                              gVenusLighthouseAerie_PartyHeadingAction);
  ObjectEvent_SetActionScript(LOCALID_GARET,
                              gVenusLighthouseAerie_PartyHeadingAction);
  ObjectEvent_SetActionScript(LOCALID_IVAN,
                              gVenusLighthouseAerie_PartyHeadingAction);
  ObjectEvent_SetActionScript(LOCALID_MIA,
                              gVenusLighthouseAerie_PartyHeadingAction);
  MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  FieldScript_WaitFrames(80);
  MapGrid_SetShake(PIXELS(6), PIXELS(6), Q16_ONE);
  FieldScript_WaitFrames(40);
  Script_PlaySong(SE_CRASH);
  MapGrid_CopyCellAttributes(110, 106, 18, 14, 10, 5);
  MapGrid_CopyCellsTo(110, 105, 74, 4, 18, 23);
  MapGrid_CopyCellsTo(92, 86, 11, 68, 16, 20);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  objectEvent->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_GARET);
  objectEvent->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_IVAN);
  objectEvent->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_MIA);
  objectEvent->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_BEACON_STATUE_SOUTHWEST);
  objectEvent->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_BEACON_STATUE_NORTHWEST);
  objectEvent->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_BEACON_STATUE_NORTHEAST);
  objectEvent->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  objectEvent = ObjectEvent_GetById(LOCALID_BEACON_STATUE_SOUTHEAST);
  objectEvent->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopObjectEventMotion(objectEvent);
  FieldCamera_MoveTo(PIXELS(322), PIXELS(32), PIXELS(180), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_IVAN, ATTACHED_EFFECT_IN_FRONT |
                                                  ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_MIA, ATTACHED_EFFECT_IN_FRONT |
                                                 ATTACHED_EFFECT_ONCE);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 6, 0);
  ObjectMotion_Launch(LOCALID_GARET, 6, 0);
  ObjectMotion_Launch(LOCALID_IVAN, 6, 0);
  ObjectMotion_Launch(LOCALID_MIA, 6, 0);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  FieldScript_WaitFrames(80);
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  FieldScript_WaitFrames(40);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_PARTY_LEADER);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_GARET);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_IVAN);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_MIA);
  FieldScript_WaitFrames(120);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHWEST, 20);
  VenusLighthouseAerie_ShowMessage(LOCALID_IVAN);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST, 0);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTHEAST, 20);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(LOCALID_MIA);
  ObjectMotion_Launch(LOCALID_IVAN, 2, 20);
  VenusLighthouseAerie_ShowMessage(LOCALID_IVAN);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  VenusLighthouseAerie_FaceObjectEvent(LOCALID_GARET, FACING_EAST);
  VenusLighthouseAerie_ShowMessage(LOCALID_GARET);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetActionScript(LOCALID_GARET,
                              gVenusLighthouseAerie_PartyExitAction);
  ObjectEvent_SetActionScript(LOCALID_IVAN,
                              gVenusLighthouseAerie_PartyExitAction);
  ObjectEvent_SetActionScript(LOCALID_MIA,
                              gVenusLighthouseAerie_PartyExitAction);
  FieldScript_WaitFrames(60);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 272, 216);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 272, 254);
  FieldScript_WaitFrames(80);
  ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, 1);
  ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))->transitionFrames = 16;
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(80);
  ObjectSystem_SuspendProjectionAndShowSprites();
  ((struct WindowWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_WINDOW))->voiceSound = 0;
  ((struct WindowWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_WINDOW))->voiceCountdown = 0;
  FieldMessage_ShowCentered(MsgVenusDespiteLongTiring, 0, 0);
  ObjectSystem_ResumeProjectionAndHideLayers();
  FieldScript_WaitFrames(80);
}

/// Clears all movement targets and velocities and resets the object event
/// timer.
static void
VenusLighthouseAerie_StopObjectEventMotion(struct ObjectEvent *objectEvent) {
  objectEvent->targetX = OBJ_EVENT_NO_TARGET;
  objectEvent->targetY = OBJ_EVENT_NO_TARGET;
  objectEvent->targetZ = OBJ_EVENT_NO_TARGET;

  SET_VELOCITY(objectEvent, 0, 0, 0);

  objectEvent->timer = 0;
}

/// Advances the beacon rise sequence, its flashes and orbiting particles.
static void VenusLighthouseAerie_UpdateBeaconRise(void) {
  struct ObjectEvent *center;
  struct ObjectEvent *effect;
  struct Sprite *sprite;
  s32 spawnParticles;
  u32 randomHeight;
  s32 angle;

  center = ObjectEvent_Get(LOCALID_BEACON_ORB);
  spawnParticles = 0;
  switch (sTransitionStep) {
  case 0:
    Script_PlaySong(SE_SHIMMER);
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
    VenusLighthouseAerie_StopObjectEventMotion(center);
    ObjectEvent_SetActionScript(LOCALID_BEACON_ORB,
                                gVenusLighthouseAerie_RiseActionScript);
    break;
  case 25:
    sTransitionStep--;
    if (center->y.fixed > 0) {
      PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 16, 12),
                              PALETTES_ALL);
      PaletteFade_StartInterpolation(16);
      sTransitionStep++;
      ObjectEvent_Get(LOCALID_PARTY_LEADER)->riseCounter = 1;
      ObjectEvent_Get(LOCALID_GARET)->riseCounter = 1;
      ObjectEvent_Get(LOCALID_IVAN)->riseCounter = 1;
      ObjectEvent_Get(LOCALID_MIA)->riseCounter = 1;
      ObjectEvent_Get(LOCALID_FELIX)->riseCounter = 1;
      ObjectEvent_Get(LOCALID_SHEBA)->riseCounter = 1;
    } else {
      if ((gFrameCount & 7) == 0)
        Script_PlaySong(SE_GLITTER);
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
        Script_PlaySong(SE_GLITTER);
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
    Script_PlaySong(SE_FALLING_LIGHT);
    PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
    PaletteFade_StartInterpolation(12);
    break;
  case 48:
    ObjectMotion_EnableActionAndResetMotion(LOCALID_BEACON_ORB);
    Script_SetFlag(FLAG_VENUS_AERIE_PARTY_LIFTED);
    break;
  }
  if (spawnParticles) {
    randomHeight = ((u32)(Random_Next() * 80) >> 16) << 16;
    effect = ObjectEvent_Create(SPRITE_PARTICLE, center->x.fixed,
                                center->y.fixed - randomHeight + -PIXELS(8),
                                center->z.fixed);
    if (effect != 0) {
      sprite = effect->sprite;
      ObjectEvent_SetScript(effect, gVenusLighthouseAerie_RiseParticleScript);
      ObjectEvent_SetSpriteColor(effect, SPRITE_COLOR_GRB);
      effect->motionFlags = 0;
      angle = Random_Next() & 0xffff000;
      effect->timer = angle;
      effect->work = 0;
      effect->riseCounter = (u32)Random_Next() >> 13;
      effect->update = VenusLighthouseAerie_UpdateRisingOrbit;
      effect->speed = Trig_Sin((u32)(Random_Next() * 0xffff) >> 20) * 24;
      effect->speed = center->speed >> 16;
      sprite->flags = 0;
      sprite->image.priority = 1;
    }
  }
  sTransitionStep++;
  VenusLighthouseAerie_AdvanceObjectEventRise(
      ObjectEvent_Get(LOCALID_PARTY_LEADER));
  VenusLighthouseAerie_AdvanceObjectEventRise(ObjectEvent_Get(LOCALID_GARET));
  VenusLighthouseAerie_AdvanceObjectEventRise(ObjectEvent_Get(LOCALID_IVAN));
  VenusLighthouseAerie_AdvanceObjectEventRise(ObjectEvent_Get(LOCALID_MIA));
  VenusLighthouseAerie_AdvanceObjectEventRise(ObjectEvent_Get(LOCALID_FELIX));
  VenusLighthouseAerie_AdvanceObjectEventRise(ObjectEvent_Get(LOCALID_SHEBA));
}

/// Raises an enabled object event according to its gated rise counter.
static void
VenusLighthouseAerie_AdvanceObjectEventRise(struct ObjectEvent *objectEvent) {
  if (objectEvent->riseEnabled != 0) {
    u8 counter = objectEvent->riseCounter;

    objectEvent->y.fixed = objectEvent->riseBaseY + ((counter >> 2) << 16);

    VenusLighthouseAerie_StopObjectEventMotion(objectEvent);

    if (objectEvent->riseCounter != 0) {
      if (objectEvent->riseCounter <= 31) {
        ++objectEvent->riseCounter;
      }
    }
  }
}

/// Emits particles beside the lower beam and links each to its source object
/// event.
static void VenusLighthouseAerie_SpawnSideOrbitParticles(void) {
  union ObjectEventOrEffect *center;
  struct ObjectEvent *effect;
  struct Sprite *sprite;
  struct MapGridScrollWork *grid;
  s32 drift;
  s32 phase;
  s32 angle;

  center = ((union ObjectEventOrEffect *)ObjectEvent_GetById(23));
  grid = gMapGridWork[0];
  drift = (u32)(Random_Next() * 48) >> 16 << 16;
  if ((s16)(grid->viewY >> 16) <= 129) {
    if (gFrameCount & 1) {
      ObjectEvent_SetPosition(LOCALID_BEACON_ORB, PIXELS(304), PIXELS(164));
      ObjectEvent_Get(LOCALID_BEACON_ORB)->scaleX = Q16_ONE;
      ObjectEvent_Get(LOCALID_BEACON_ORB)->scaleY = Q16_ONE;
    } else {
      ObjectEvent_SetPosition(LOCALID_BEACON_ORB, PIXELS(304), PIXELS(171));
      ObjectEvent_Get(LOCALID_BEACON_ORB)->scaleX = 13 * Q16_ONE / 10;
      ObjectEvent_Get(LOCALID_BEACON_ORB)->scaleY = 13 * Q16_ONE / 10;
    }
  } else {
    ObjectEvent_SetPosition(LOCALID_BEACON_ORB, 0, 0);
  }
  if (center == 0)
    return;
  phase = gFrameCount & 15;
  if (phase != 0)
    return;
  effect = ObjectEvent_Create(SPRITE_PARTICLE,
                              center->objectEvent.x.fixed + PIXELS(8),
                              center->objectEvent.y.fixed + drift + PIXELS(8),
                              center->objectEvent.z.fixed);
  drift /= PIXELS(6);
  drift <<= 16;
  if (effect == 0)
    return;
  sprite = effect->sprite;
  ObjectEvent_SetScript(effect, gVenusLighthouseAerie_BesideParticleScript);
  ObjectEvent_SetSpriteColor(effect, SPRITE_COLOR_BRG);
  effect->motionFlags = phase;
  angle = Random_Next() & 0xffff000;
  effect->timer = angle;
  effect->work = phase;
  effect->linkedObject = &center->objectEvent;
  effect->update = VenusLighthouseAerie_UpdateOrbitingEffect;
  effect->speed = Trig_Sin((drift & 0xfffff) >> 4) * 24 >> 16;
  sprite->flags = 0;
  sprite->image.priority = center->objectEvent.sprite->image.priority;
}

/// Moves a linked particle along its horizontal ellipse and preserves its
/// target.
static void VenusLighthouseAerie_UpdateOrbitingEffect(
    union ObjectEventOrEffect *fieldObject) {
  struct ObjectEvent *anchor;
  s32 spin;

  anchor = fieldObject->objectEvent.linkedObject;
  spin = fieldObject->effect.spin;
  fieldObject->effect.x =
      anchor->x.fixed + Trig_Cos(spin) * (fieldObject->objectEvent.speed + 28);
  fieldObject->effect.z = Trig_Sin(spin) * 16 + PIXELS(164);
  fieldObject->objectEvent.targetX = fieldObject->effect.x;
  fieldObject->objectEvent.targetZ = fieldObject->effect.z;
  fieldObject->effect.spin -= 0x200;
}

s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
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
    SC_CALL(VenusLighthouseAerie_PulseObjectEventScale),
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
    SC_FREE,
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
    SC_FREE,
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
    SC_FREE,
    SC_END,
};

s32 gVenusLighthouseAerie_PartyHeadingAction[] = {
    SC_CALL(Scene_LookAboutRandomly),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct MapWarpDest gVenusLighthouseAerieWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 304, 0, 216,
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
    {MAP_TABLE_END},
};

u32 gVenusLighthouseAerieWarps[] = {
    MAP_WARPS(VenusLighthouseAerie),
    MAP_WARP(VENUS_LIGHTHOUSE_AERIE_TO_WORLD_MAP, WorldMap,
             WORLD_MAP_BEFORE_SHEBAS_FALL),
    MAP_WARP(VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_C, TimberRoomsC,
             TIMBER_ROOMS_C_FROM_VENUS_LIGHTHOUSE_AERIE),
    MAP_WARP(VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_B, TimberRoomsB,
             TIMBER_ROOMS_B_SANCTUM),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gVenusLighthouseAerieObjectEventTemplates[] = {
    { SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(440), 0, PIXELS(252), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(472), 0, PIXELS(252), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(504), 0, PIXELS(252), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(104), 0, PIXELS(252), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(136), 0, PIXELS(252), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(168), 0, PIXELS(252), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(504), 0, PIXELS(148), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { LOCALID_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(504), 0, PIXELS(148), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { LOCALID_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(504), 0, PIXELS(148), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_MENARDI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(164), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(180), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { LOCALID_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
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
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};
// clang-format on

s32 gVenusLighthouseAerie_LandedPlatform = 0;

struct MapEvent gVenusLighthouseAerieEvents[] = {
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING,
     MB_VENUS_LIGHTHOUSE_AERIE_FLOOR_BY_PLATFORMS, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_JumpToNextPlatform)},
    {MAP_EVENT_COORD, MB_VENUS_LIGHTHOUSE_AERIE_FLOOR_BY_PLATFORMS,
     CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_FloatPlatforms)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING,
     MB_VENUS_LIGHTHOUSE_AERIE_PLATFORM, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_JumpToNextPlatform)},
    {MAP_EVENT_COORD, MB_VENUS_LIGHTHOUSE_AERIE_PLATFORM,
     FLAG_VENUS_AERIE_ON_PLATFORM, EVENT_SCRIPT(EventScript_LandOnPlatform)},
    {MAP_EVENT_COORD, MB_VENUS_LIGHTHOUSE_AERIE_BEFORE_BEACON, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ConfrontSaturosAndMenardi)},
    {MAP_EVENTS_END},
};

s32 gVenusLighthouseAerie_BesideParticleScript[] = {
    SC_SET(SW_SCALE_Y, 7 * Q16_ONE / 5),
    SC_SET(SW_SCALE_X, 7 * Q16_ONE / 5),
    SC_ANIMATION(4),
    SC_WAIT(30),
    SC_FREE,
};
