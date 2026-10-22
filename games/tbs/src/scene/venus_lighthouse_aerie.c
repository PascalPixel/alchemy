/// @file
/// Lighthouse Aerie (SceneId_VenusLighthouseAerie), the top of Venus
/// Lighthouse and the only scene of this overlay: one scripted sequence cut by
/// two battles and two trips to the world map. Each part starts in
/// VenusLighthouseAerie_Initialize from the entrance the part before sent the
/// party to, and every arrival blocks Retreat and sets
/// FLAG_VENUS_LIGHTHOUSE_LIT. The parts, in the order the entrances chain
/// them:
/// - The arrival (VENUS_LIGHTHOUSE_AERIE_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR,
///   VenusLighthouseAerie_OverhearSaturosAndMenardi): a platform carries the
///   leader up and a conversation plays, with one yes-or-no question that
///   changes only the reply.
/// - Six floating platforms lie between the party and the beacon
///   (TRIGGER_PLATFORM, TRIGGER_FLOOR_BY_PLATFORMS): the leader jumps from one
///   to the next, and each stops bobbing and dips under him as he lands
///   (FLAG_SCENE_TEMP_50 while he stands on one).
/// - The confrontation (TRIGGER_BEFORE_BEACON,
///   Event_ConfrontSaturosAndMenardi): the Shaman's Rod is handed over,
///   remembered in FLAG_SHAMANS_ROD_OWNER plus the member who carried it, and
///   the first battle follows (ENCOUNTER_ZONE_SCRIPTED, slot 1,
///   FLAG_AERIE_FIRST_BATTLE_BEGUN).
/// - After it (VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE,
///   VenusLighthouseAerie_CombineSaturosAndMenardi): Saturos throws the Venus
///   Star into the beacon (ACTOR_STAR_CARRIER), Felix and Sheba leave, and
///   Saturos and Menardi fuse. Party_RestorePpOfLitElements refills the PP of
///   the party's Adepts whose lighthouses are lit, and the second battle
///   follows (slot 0, FLAG_AERIE_SECOND_BATTLE_BEGUN).
/// - After that (VENUS_LIGHTHOUSE_AERIE_AFTER_SECOND_BATTLE,
///   VenusLighthouseAerie_FinishSaturosAndMenardiBattle): Saturos and Menardi
///   fall into the lighthouse, and the beacon erupts: its beam
///   (ACTOR_BEAM_BOTTOM to ACTOR_BEAM_TOP) rises, the aerie splits and its
///   statues move apart, and the light lifts the party away to the world map's
///   lighthouse transition (WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT). The beam's
///   five segments are sprites stacked over the beacon, with particles
///   rising round them, and the split is drawn by copying the map's cells
///   (Map_CopyCellsTo in VenusLighthouseAerie_EruptBeacon)
///   (https://alchemy.pascalpixel.com/places/venus-lighthouse-aerie/eruption.gif:
///   eight seconds from just before the segments stack, each numbered 1 to 5
///   from the bottom;
///   https://alchemy.pascalpixel.com/places/venus-lighthouse-aerie/eruption-layers-3d.gif:
///   five and a half seconds in, pulled apart into its layers, the sky at
///   the back, BG2, BG1 and BG0, and the sprites in front).
/// - A look back (VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT,
///   VenusLighthouseAerie_ShowShebaInDanger): Sheba hangs from the broken
///   aerie, and VENUS_LIGHTHOUSE_AERIE_TO_WORLD_MAP returns to the world map
///   (WORLD_MAP_BEFORE_SHEBAS_FALL) with its music kept.
/// - Sheba's fall (VENUS_LIGHTHOUSE_AERIE_FROM_WORLD_MAP,
///   VenusLighthouseAerie_StageShebasFall): Sheba and Felix fall, and the party
///   is taken to Lalivero (VENUS_LIGHTHOUSE_AERIE_TO_LALIVERO_FARANS_HOUSE)
///   with FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE set.
///
/// A lost battle returns to VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT, which gives
/// the Shaman's Rod back to whoever carried it and leaves for Lalivero Sanctum
/// (VENUS_LIGHTHOUSE_AERIE_TO_LALIVERO_SHOPS).
///
/// Open questions: none.

#include "script.h"
#include "game_state.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/scene/venus_lighthouse_aerie.h"
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
#include "macros.h"

#define NULL ((void *)0)
#include "field_obj.h"
#include "types.h"
#include "animspr.h"
#include "venus_lighthouse_aerie.h"
#include "field_effect.h"
#include "event.h"
#include "field_event.h"
#include "field_scene.h"
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
#include "flags.h"
#include "item_ids.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.

/// Plus the party member (0 to 3) who carried the Shaman's Rod when it was
/// handed over before the aerie's first battle; a defeat gives it back.
/// ★ Named from the game: "Shaman's Rod" (MsgItemName)
#define FLAG_SHAMANS_ROD_OWNER 0x345

/// Place flags that choose the aerie's field music after each battle, by the
/// scene's field music rules (gScene_InteractionRuleTable,
/// object/scene_rules.s): MUS_019 before the first battle, MUS_030 once the
/// first has begun and MUS_022 once the second has. VenusLighthouseFirstFloor
/// and VenusLighthouseFourthFloor, in the same place group, keep their Psynergy
/// Stones in the same two flags, which no one goes back down to see.
enum {
  /// ★ Named from its area name: Lighthouse Aerie
  FLAG_AERIE_FIRST_BATTLE_BEGUN = FLAG_PLACE_TEMP_50,
  /// ★ Named from its area name: Lighthouse Aerie
  FLAG_AERIE_SECOND_BATTLE_BEGUN = FLAG_PLACE_TEMP_51
};

static void Event_FloatPlatforms(void);
static void VenusLighthouseAerie_OverhearSaturosAndMenardi(void);
static void VenusLighthouseAerie_ShowPartyMember(s32 actor, s32 shown);
static void VenusLighthouseAerie_FlashActorPalette(struct FieldActor *actor);
static void
VenusLighthouseAerie_SpawnLinkedPairEffects(struct FieldActor *parent);
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
static void VenusLighthouseAerie_StopActorMotion(struct FieldActor *actor);
static void VenusLighthouseAerie_UpdateBeaconRise(void);
static void VenusLighthouseAerie_AdvanceActorRise(struct FieldActor *actor);
static void VenusLighthouseAerie_SpawnSideOrbitParticles(void);
static void
VenusLighthouseAerie_UpdateOrbitingEffect(union FieldObject *object);

/// The aerie's event cells.
enum VenusLighthouseAerieTrigger {
  TRIGGER_BEFORE_BEACON = 5, ///< where the party confronts Saturos and Menardi
  TRIGGER_PLATFORM = 10,     ///< a floating platform's cells
  /// The floor beside the platforms, from which the leader jumps
  /// onto them.
  TRIGGER_FLOOR_BY_PLATFORMS = 11
};

/// A child value that makes an actor blink rather than set its colour mode
/// (Engine_ActorSetChildValue).
enum { CHILD_VALUE_BLINK = 0x100 };

/// Animations the aerie plays by number beyond enum ActorAnimation, from each
/// sprite's animation table (graphics/sprite/spr_<number>.tsv; the party's
/// through their animation lists).
enum {
  /// Saturos standing, two pictures alternating at long intervals
  /// (spr_032.tsv).
  SATUROS_ANIM_BREATHE = 8,
  SATUROS_ANIM_LIE = 9, ///< lying on his side
  /// Saturos falling into the lighthouse: upright, then half his height, then
  /// only his top above the floor.
  SATUROS_ANIM_TIP = 10,
  SATUROS_ANIM_SLUMP = 11,
  SATUROS_ANIM_SINK = 12,
  /// Menardi standing, her hair blown back (spr_033.tsv).
  MENARDI_ANIM_STAND_WINDSWEPT = 5,
  MENARDI_ANIM_LIE = 7, ///< lying on her side
  /// Menardi falling into the lighthouse, as SATUROS_ANIM_TIP to _SINK.
  MENARDI_ANIM_TIP = 8,
  MENARDI_ANIM_SLUMP = 9,
  MENARDI_ANIM_SINK = 10,
  /// Garet on his knees, Ivan and Mia lying down, as the party lies on the
  /// broken aerie (spr_001.tsv to spr_003.tsv).
  ANIMATION_PARTY_KNOCKED_DOWN = 18,
  ANIMATION_LEADER_KNOCKED_DOWN = 19, ///< Isaac lying on his back (spr_000.tsv)
  FELIX_ANIM_KNEEL = 5, ///< Felix crouched on one knee (spr_035.tsv)
  /// Sheba hanging from the broken aerie by one hand, seen from the side
  /// (spr_006.tsv).
  SHEBA_ANIM_HANG_ONE_HAND = 5,
  SHEBA_ANIM_HANG = 6, ///< hanging by both hands, seen from behind
  /// Her grip slipping for a moment, then SHEBA_ANIM_HANG again.
  SHEBA_ANIM_SLIP = 7
};

// The aerie's scene flags:
// FLAG_SCENE_TEMP_34: Saturos and Menardi slide together as they fuse.
// FLAG_SCENE_TEMP_35: they grow as they fuse.
// FLAG_SCENE_TEMP_36: the beacon has erupted; its particles rise higher.
// FLAG_SCENE_TEMP_37: the light has lifted the party and flashed white.
// FLAG_SCENE_TEMP_50: the leader stands on a platform.

#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Pulses the actor scale around the countdown, then chooses its next pause.
static s32
VenusLighthouseAerie_PulseActorScale(struct ScriptInterpreter *interpreter) {
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

/// Moves a particle, damps its horizontal velocity and updates its scale and
/// spin.
static void
VenusLighthouseAerie_UpdateDriftingParticle(union FieldObject *object) {
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

/// Turns the falling actor sprite until its rotation leaves the negative
/// half-turn.
static void
VenusLighthouseAerie_StepSpriteRotation(union FieldObject *fieldObject) {
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

#include "common/object_heading.h"

const struct SceneEntrance *VenusLighthouseAerie_GetEntrances(void) {
  return gVenusLighthouseAerieEntrances;
}

const struct SceneRegion *VenusLighthouseAerie_GetRegions(void) { return 0; }

const u32 *VenusLighthouseAerie_GetExits(void) {
  return gVenusLighthouseAerieExits;
}

/// Returns the beacon's statues, the platforms, Saturos and Menardi, Felix and
/// Sheba, the party and the beacon effects.
const struct ScenePlacement *VenusLighthouseAerie_GetPlacements(void) {
  return gVenusLighthouseAeriePlacements;
}

#include "common/object_distance.h"
#include "common/object_nearest.h"

/// Walking off the edge of a platform: if the ground two tiles ahead is clear,
/// the leader jumps across to it.
static void Event_JumpToNextPlatform(void) {
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
  SET_VECTOR(destination.coordinates, (actor->x.fixed & 0xfff00000) + PIXELS(8),
             actor->y.fixed, (actor->z.fixed & 0xfff00000) + PIXELS(8));
  Vector_AddPolarOffset(PIXELS(32), angle, destination.coordinates);
  if (FieldObject_CheckMovementCollision(actor, &destination.position) == 0) {
    Engine_GameFlagClear(FLAG_SCENE_TEMP_50);
    Event_FloatPlatforms();
    Object_SetMode(actor, ANIMATION_LEADER_SPRING);
    Engine_TaskWait(6);
    Object_SetMode(actor, ANIMATION_LEADER_LEAP);
    actor->speed = PIXELS(3);
    actor->acceleration = PIXELS(2);
    Engine_AudioPlayCue(SE_JUMP);
    actor->velocityY = PIXELS(4);
    actor->motionFlags &= 0x7e;
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER,
                              destination.coordinates[0] >> 16,
                              destination.coordinates[2] >> 16);
    Object_SetMode(actor, ANIMATION_LEADER_SPRING);
    Engine_ActorSetSpriteFlags(actor, 1);
    actor->motionFlags = flags;
  }
}

/// Landing on a platform: it dips under the leader and springs back up.
/// FLAG_SCENE_TEMP_50 is set while the leader stands on a platform, so this
/// runs once per landing.
static void Event_LandOnPlatform(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldActor *platform;
  s32 platformId;
  s32 reboundStep;

  Engine_EventBegin();
  platformId = SceneActor_FindNearestFloatingPlatform();
  gVenusLighthouseAerie_LandedPlatform = platformId;
  if (platformId != 0) {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_50);
    platform = Object_GetById(gVenusLighthouseAerie_LandedPlatform);
    platform->motionFlags = 0;
    leader->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
    platform->y.fixed += -PIXELS(3);
    leader->y.fixed += -PIXELS(3);
    leader->terrainHeight += -PIXELS(3);
    Engine_TaskWait(2);
    platform->y.fixed += -PIXELS(2);
    leader->y.fixed += -PIXELS(2);
    leader->terrainHeight += -PIXELS(2);
    Engine_TaskWait(10);
    reboundStep = PIXELS(2);
    platform->y.fixed += reboundStep;
    leader->y.fixed += reboundStep;
    leader->terrainHeight += reboundStep;
    Engine_TaskWait(4);
    platform->y.fixed += reboundStep;
    leader->y.fixed += reboundStep;
    leader->terrainHeight += reboundStep;
    Engine_TaskWait(4);
    platform->y.fixed += PIXELS(1);
    leader->y.fixed += PIXELS(1);
    leader->terrainHeight += PIXELS(1);
  }
  Engine_EventEnd();
}

/// Stepping back onto the floor, and before each jump: the leader walks and
/// falls normally again, and all six platforms float, bobbing, as they do
/// until the leader lands on one (Event_LandOnPlatform).
static void Event_FloatPlatforms(void) {
  s32 flags;

  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  flags = ACTOR_FLOATS;
  Object_GetById(ACTOR_FIRST_PLATFORM)->motionFlags = flags;
  Object_GetById(ACTOR_FIRST_PLATFORM + 1)->motionFlags = flags;
  Object_GetById(ACTOR_FIRST_PLATFORM + 2)->motionFlags = flags;
  Object_GetById(ACTOR_FIRST_PLATFORM + 3)->motionFlags = flags;
  Object_GetById(ACTOR_FIRST_PLATFORM + 4)->motionFlags = flags;
  Object_GetById(ACTOR_LAST_PLATFORM)->motionFlags = flags;
}

/// Returns the platform and confrontation triggers.
const struct SceneEvent *VenusLighthouseAerie_GetEvents(void) {
  return gVenusLighthouseAerieEvents;
}

/// Sets up the beacon's statues and the six platforms.
/// It then runs the part of the sequence the entrance selects (the arrival only
/// when not back from a battle), or, after a lost battle, gives back the
/// Shaman's Rod and leaves for Lalivero Sanctum. Back from a battle, the
/// platform nearest the leader stands still.
s32 VenusLighthouseAerie_Initialize(void) {
  s32 mirroredScale;
  u32 platformId;
  s32 clear;
  s16 entrance;
  struct FieldActor *platform;

  Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  Engine_TaskWait(1);
  Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_LIT);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_BEACON_STATUE_SOUTHWEST), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_BEACON_STATUE_NORTHWEST), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_BEACON_STATUE_NORTHEAST), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_BEACON_STATUE_SOUTHEAST), 0);
  mirroredScale = -Q16_ONE;
  Object_GetById(ACTOR_BEACON_STATUE_NORTHEAST)->scaleX = mirroredScale;
  Object_GetById(ACTOR_BEACON_STATUE_SOUTHEAST)->scaleX = mirroredScale;
  platformId = ACTOR_FIRST_PLATFORM;
  clear = 0;
  do {
    platform = Object_GetById(platformId);
    Engine_ActorSetSpriteFlags(Object_GetById(platformId), clear);
    Engine_ActorSetSpritePriority(platformId, 1);
    platform->motionFlags = ACTOR_FLOATS;
    platform->priorityFlags |= 2;
    platform->y.fixed = PIXELS(1) / 2;
    platformId++;
  } while (platformId <= ACTOR_LAST_PLATFORM);

  entrance = gGameState.entrance;

  switch (entrance) {
  case VENUS_LIGHTHOUSE_AERIE_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
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
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(FLAG_SHAMANS_ROD_OWNER) != 0) {
      Inventory_AddItem(0, ITEM_SHAMANS_ROD);
    } else if (Engine_GameFlagIsSet(FLAG_SHAMANS_ROD_OWNER + 1) != 0) {
      Inventory_AddItem(1, ITEM_SHAMANS_ROD);
    } else if (Engine_GameFlagIsSet(FLAG_SHAMANS_ROD_OWNER + 2) != 0) {
      Inventory_AddItem(2, ITEM_SHAMANS_ROD);
    } else {
      Inventory_AddItem(3, ITEM_SHAMANS_ROD);
    }
    Engine_EventRequestExit(VENUS_LIGHTHOUSE_AERIE_TO_LALIVERO_SHOPS);
    break;
  }

  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    s32 slot = SceneActor_FindNearestFloatingPlatform();

    if (slot != 0) {
      struct FieldActor *nearestPlatform = Object_GetById(slot);
      if (nearestPlatform != 0) {
        nearestPlatform->motionFlags = 0;
      }
    }
  }
  return clear;
}

static void VenusLighthouseAerie_ShowMessage(s32 speaker) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(10);
}

/// Turns one actor and waits ten frames.
static void VenusLighthouseAerie_FaceActor(s32 actor, s32 facing) {
  Engine_ActorFaceDirection(actor, facing, 10);
}

/// Brings the party onto the aerie and reveals the argument over Sheba.
static void VenusLighthouseAerie_OverhearSaturosAndMenardi(void) {
  struct FieldActor *arrivalPlatform;
  struct FieldActor *leader;

  // Bring the leader across on the moving platform, then reveal the party.
  arrivalPlatform = Object_GetById(ACTOR_RISING_PLATFORM);
  Engine_EventBegin();
  VenusLighthouseAerie_ShowPartyMember(1, 0);
  VenusLighthouseAerie_ShowPartyMember(2, 0);
  VenusLighthouseAerie_ShowPartyMember(3, 0);
  Engine_CameraMoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  arrivalPlatform->motionFlags = 0;
  arrivalPlatform->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_RISING_PLATFORM), 0);
  Engine_ActorSetSpritePriority(ACTOR_RISING_PLATFORM, 1);
  Engine_ActorSetPosition(ACTOR_RISING_PLATFORM, PIXELS(580), PIXELS(338));
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  leader->motionFlags = 0;
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetPosition(ACTOR_PARTY_LEADER, PIXELS(581), PIXELS(288));
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_RISING_PLATFORM, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_RISING_PLATFORM, 580, 221);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 581, 171);
  Actor_SetDestination(ACTOR_RISING_PLATFORM, 530, 211);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 531, 161);
  Actor_SetDestination(ACTOR_RISING_PLATFORM, 520, 191);
  Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, 521, 141);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_RISING_PLATFORM), 1);
  Actor_SetDestination(ACTOR_RISING_PLATFORM, 515, 171);
  Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, 516, 121);
  Engine_AudioPlayCue(SE_STOP_SLOT3);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_LEADER_SPRING);
  ObjectMotion_WaitForAnimationChange(0);
  leader->x.fixed = PIXELS(516);
  leader->y.fixed = PIXELS(8);
  leader->z.fixed = PIXELS(148);
  leader->facing = FACING_WEST;
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Engine_AudioPlayCue(SE_JUMP);
  leader->velocityY = PIXELS(4);
  Engine_AudioPlayCue(SE_JUMP);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 1);
  Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, 504, 148);
  Engine_EventWait(10);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  {
    u16 shown = FACING_SOUTH;

    leader->y.fixed = -PIXELS(32);
    leader->facing = shown;
  }
  Engine_EventWait(20);
  Engine_AudioPlayCue(SE_308);
  Engine_ActorMoveToAndWait(ACTOR_RISING_PLATFORM, 524, 191);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_RISING_PLATFORM), 0);
  Actor_MoveToAndWait(ACTOR_RISING_PLATFORM, 530, 211);
  Actor_MoveToAndWait(ACTOR_RISING_PLATFORM, 580, 221);
  Engine_ActorSetDestination(ACTOR_RISING_PLATFORM, 580, 338);
  Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  VenusLighthouseAerie_ShowPartyMember(1, 1);
  VenusLighthouseAerie_ShowPartyMember(2, 1);
  VenusLighthouseAerie_ShowPartyMember(3, 1);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 492, 164);
  Engine_ActorWalkTo(GARET, 514, 164);
  Actor_WalkTo(IVAN, 492, 140);
  Engine_ActorWalkToAndWait(MIA, 514, 140);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_STAND);
  Engine_ActorSetAnimation(GARET, ANIMATION_STAND);
  Engine_ActorSetAnimation(IVAN, ANIMATION_STAND);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Engine_ActorFaceDirection(GARET, FACING_SOUTH, 0);
  Engine_ActorFaceDirection(IVAN, FACING_SOUTH, 0);
  Engine_ActorFaceDirection(MIA, FACING_SOUTH, 0);
  Engine_ActorWaitForMove(ACTOR_RISING_PLATFORM);
  Engine_ActorSetPosition(ACTOR_RISING_PLATFORM, 0, 0);
  Engine_AudioPlayCue(AUDIO_CUE_FADE_SLOT3);
  // Reveal Saturos and Menardi arguing with Felix about Sheba.
  Engine_EventSetMessage(MsgVenusLongLastTime);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MENARDI);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Engine_ActorFaceDirection(GARET, FACING_WEST, 0);
  Engine_ActorFaceDirection(IVAN, FACING_WEST, 0);
  VenusLighthouseAerie_FaceActor(MIA, FACING_WEST);
  Engine_EventGetViewCenter()->motionFlags = 0;
  Engine_CameraSetSpeed(PIXELS(24) / 5, PIXELS(3) / 5);
  Engine_CameraMoveTo(PIXELS(304), PIXELS(32), PIXELS(158), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH + FACING_STEP);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  Engine_AudioPlayCue(MUS_061);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Engine_ActorSetAnimation(ACTOR_MENARDI, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MENARDI);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
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
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, ANIMATION_NOD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_EventShowMessageAndWait(ACTOR_MENARDI, 0, 40);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_WEST);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Engine_ActorFaceDirection(SHEBA, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTH + FACING_STEP);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Engine_EventShowMessageAndWait(ACTOR_FELIX, 0, 20);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_WEST, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIMATION_SHAKE_HEAD);
  Engine_EventShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, ANIMATION_NOD);
  Engine_EventShowMessageAndWait(ACTOR_FELIX, 0, 20);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, ANIMATION_NOD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Engine_ActorStartRepeatedMotion(ACTOR_MENARDI, 1);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, ANIMATION_SHAKE_HEAD);
  Engine_EventShowMessageAndWait(ACTOR_FELIX, 0, 20);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_EventShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_NORTHWEST + FACING_STEP);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  VenusLighthouseAerie_FaceActor(SHEBA, FACING_SOUTHEAST + FACING_STEP);
  Engine_ActorRunRepeatedMotion(SHEBA, 2);
  Engine_EventWait(20);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_NORTH + FACING_STEP);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Engine_ActorFaceDirection(SHEBA, FACING_EAST, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIMATION_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Engine_ActorFaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_WEST, 0);
  Engine_CameraSetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(294), -1, PIXELS(180), 1);
  Actor_WalkToAndWait(ACTOR_FELIX, 262, 176);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_WEST, 40);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_EAST, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 2);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FELIX);
  Engine_ActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 2);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Event_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 20);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FELIX);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, ANIMATION_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_WEST, 20);
  Engine_EventShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 40);
  Actor_ShowEmote(SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_EAST, 40);
  Engine_ActorSetAnimation(ACTOR_MENARDI, ANIMATION_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Engine_EventShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_FELIX, 0, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Engine_ActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Engine_ActorSetAnimation(ACTOR_MENARDI, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Engine_ActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Engine_ActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(ACTOR_SATUROS, 2);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Engine_ActorSetAnimation(ACTOR_MENARDI, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
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
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | GARET);
  Engine_ActorShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(MIA);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_EventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | IVAN, 0, 40);
  Engine_ActorJump(GARET, 2, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW | GARET);
  Engine_EventOpenMessage(SPEAKER_WINDOW_BELOW | GARET, 0);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Engine_ActorFaceDirection(IVAN, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message += 1;
  }
  Engine_EventWait(20);
  VenusLighthouseAerie_ShowMessage(GARET);
  Engine_ActorFaceDirection(IVAN, FACING_EAST, 0);
  Engine_ActorFaceDirection(MIA, FACING_WEST, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Actor_SetActionScript(GARET, gVenusLighthouseAerie_JoinLeaderAction);
  Actor_SetActionScript(IVAN, gVenusLighthouseAerie_JoinLeaderAction);
  Actor_RunActionScriptAndWait(MIA, gVenusLighthouseAerie_JoinLeaderAction);
  Engine_EventWait(20);
  Engine_EventEnd();
}

/// Shows or hides one companion, setting its movement speed when shown.
static void VenusLighthouseAerie_ShowPartyMember(s32 actorId, s32 shown) {
  struct FieldActor *member;

  if (shown != 0) {
    Actor_SetChildValue(actorId, SPRITE_COLOR_NORMAL);
    member = Actor_Get(actorId);
    Engine_ActorSetSpriteFlags(member, 1);
    Actor_SetSpeed(actorId, PIXELS(4) / 5, PIXELS(2) / 5);
  } else {
    Actor_SetChildValue(actorId, SPRITE_COLOR_HIDDEN);
    member = Actor_Get(actorId);
    Engine_ActorSetSpriteFlags(member, 0);
  }
}

/// Walking up to Saturos and Menardi (TRIGGER_BEFORE_BEACON): Felix refuses to
/// be rescued, Sheba reads the party's minds for the rod that reaches Jupiter
/// Lighthouse, they promise not to harm her for the Shaman's Rod, Felix
/// carries it over, they will not let her go, and the first battle begins. It
/// sets FLAG_SHAMANS_ROD_OWNER plus the member who carried the rod, and
/// FLAG_AERIE_FIRST_BATTLE_BEGUN as the battle starts, which changes the
/// aerie's music.
static void Event_ConfrontSaturosAndMenardi(void) {
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
  VenusLighthouseAerie_FaceActor(ACTOR_PARTY_LEADER, FACING_WEST);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Engine_EventSetMessage(MsgVenusIsaac);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_LEFT | ACTOR_FELIX);
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
  Engine_ActorSetAnimation(GARET, ANIMATION_STAND);
  Engine_ActorSetAnimation(IVAN, ANIMATION_STAND);
  Engine_ActorFaceDirection(GARET, FACING_WEST, 0);
  Engine_ActorFaceDirection(IVAN, FACING_WEST, 0);
  VenusLighthouseAerie_FaceActor(MIA, FACING_WEST);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_EAST, 0);
  Engine_ActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_WEST);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 2);
  Event_ShowMessageAndWait(ACTOR_FELIX, 0, 20);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_NORTH + FACING_STEP, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_EAST);
  Event_ShowMessageAndWait(ACTOR_FELIX, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_WEST);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  VenusLighthouseAerie_ShowMessage(MIA);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  VenusLighthouseAerie_ShowMessage(IVAN);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_EAST, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_WEST, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Engine_ActorFaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Engine_ActorFaceDirection(GARET, FACING_WEST, 0);
  Engine_ActorFaceDirection(IVAN, FACING_WEST, 0);
  Engine_ActorFaceDirection(MIA, FACING_WEST, 0);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_EAST, 20);
  Actor_ShowEmote(SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(SHEBA);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Engine_ActorRunRepeatedMotion(SHEBA, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(SHEBA, ANIMATION_NOD);
  VenusLighthouseAerie_ShowMessage(SHEBA);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIMATION_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetSpeed(SHEBA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(SHEBA, 260, 186);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WalkToAndWait(SHEBA, 276, 192);
  object = Object_GetById(ACTOR_MENARDI);
  object->facing = FACING_SOUTH + FACING_STEP;
  Engine_TaskWait(1);
  Engine_ActorStartRepeatedMotion(ACTOR_MENARDI, 2);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Engine_ActorJump(SHEBA, 2, 20);
  Actor_SetSpeed(SHEBA, PIXELS(12) / 5, PIXELS(6) / 5);
  Actor_WalkToAndWait(SHEBA, 260, 186);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_WalkToAndWait(SHEBA, 248, 172);
  Engine_ActorFaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Engine_ActorFaceDirection(SHEBA, FACING_EAST, 20);
  Engine_ActorSetAnimationAndWait(SHEBA, ANIMATION_NOD);
  Engine_EventWait(40);
  Scene_BeginCutscenePsynergy();
  Engine_TaskWait(1);
  frame = 0;
  do {
    object = Object_GetById(SHEBA);
    VenusLighthouseAerie_FlashActorPalette(object);
    frame = (frame + 1);
    Engine_TaskWait(1);
  } while ((u32)frame <= 39);
  FieldScheduler_AddCallback(VenusLighthouseAerie_UpdateShebaPalette,
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
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_WEST);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 20);
  Actor_ShowEmote(SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Engine_ActorStartRepeatedMotion(ACTOR_MENARDI, 2);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Scheduler_RemoveCallback(VenusLighthouseAerie_UpdateShebaPalette);
  Engine_TaskWait(1);
  Actor_SetChildValue(SHEBA, SPRITE_COLOR_NORMAL);
  Engine_TaskWait(10);
  Scene_EndCutscenePsynergy();
  Engine_ActorJump(SHEBA, 2, 40);
  VenusLighthouseAerie_ShowMessage(SHEBA);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Engine_ActorRunRepeatedMotion(SHEBA, 2);
  Object_GetById(SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(SHEBA, 250, 176);
  Engine_EventWait(1);
  Object_GetById(SHEBA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_EAST, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Engine_ActorRunRepeatedMotion(SHEBA, 2);
  Engine_EventWait(40);
  Actor_SetSpeed(SHEBA, PIXELS(3) / 5, PIXELS(3) / 10);
  Object_GetById(SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(SHEBA, 248, 172);
  Engine_EventWait(1);
  Object_GetById(SHEBA)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(SHEBA, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, ANIMATION_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Engine_ActorSetAnimationAndWait(SHEBA, ANIMATION_NOD);
  VenusLighthouseAerie_ShowMessage(SHEBA);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ActorSetAnimation(ACTOR_SATUROS, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 100);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  Engine_EventWait(20);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_EAST);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  Event_OpenMessage(ACTOR_SATUROS, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIMATION_NOD);
    answerAccepted = 1;
  } else {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIMATION_SHAKE_HEAD);
    gEventWork->message += 1;
    answerAccepted = 0;
  }
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  if (answerAccepted != 0) {
    gEventWork->message += 1;
  }
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 20);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Event_OpenMessage(ACTOR_SATUROS, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorSetAnimation(IVAN, ANIMATION_NOD);
    answerAccepted = 1;
  } else {
    Engine_EventWait(20);
    Engine_ActorSetAnimation(IVAN, ANIMATION_SHAKE_HEAD);
    gEventWork->message += 1;
    answerAccepted = 0;
  }
  VenusLighthouseAerie_ShowMessage(IVAN);
  if (answerAccepted != 0) {
    gEventWork->message += 1;
  }
  Engine_ActorRunRepeatedMotion(GARET, 2);
  VenusLighthouseAerie_FaceActor(GARET, FACING_SOUTH);
  VenusLighthouseAerie_ShowMessage(GARET);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  VenusLighthouseAerie_FaceActor(MIA, FACING_WEST);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIMATION_NOD);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP);
  Engine_ActorSetAnimation(ACTOR_MENARDI, ANIMATION_NOD);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIMATION_NOD);
  Engine_EventWait(40);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_WEST);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_FELIX, 0, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_WEST);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, ANIMATION_NOD);
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
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIMATION_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, ANIMATION_NOD);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_EAST);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_WEST);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(SHEBA, ANIMATION_NOD);
  Actor_SetSpeed(SHEBA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(SHEBA, 260, 186);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WalkToAndWait(SHEBA, 276, 192);
  Event_PlayCue(MUS_SILENCE);
  object = Object_GetById(ACTOR_MENARDI);
  object->facing = FACING_SOUTH + FACING_STEP;
  Engine_TaskWait(1);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Engine_ActorRunRepeatedMotion(SHEBA, 2);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(SHEBA, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorJump(MIA, 2, 20);
  VenusLighthouseAerie_FaceActor(MIA, FACING_NORTHWEST);
  VenusLighthouseAerie_ShowMessage(MIA);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Engine_ActorFaceDirection(SHEBA, FACING_EAST, 0);
  Engine_ActorFaceDirection(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 80);
  Engine_ActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 0);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_EAST, 40);
  Event_PlayCue(MUS_029);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(SHEBA, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  VenusLighthouseAerie_ShowMessage(IVAN);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, ANIMATION_NOD);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Engine_ActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  VenusLighthouseAerie_FaceActor(SHEBA, FACING_NORTH + FACING_STEP);
  Engine_ActorSetAnimation(MIA, ANIMATION_NOD);
  VenusLighthouseAerie_ShowMessage(MIA);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Engine_ActorFaceDirection(ACTOR_SATUROS, FACING_SOUTHEAST + FACING_STEP, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_ShowEmote(SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ActorFaceDirection(SHEBA, FACING_EAST, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_EAST);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  VenusLighthouseAerie_ShowMessage(GARET);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_EAST);
  Engine_ActorSetAnimation(ACTOR_SATUROS, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Engine_ActorFaceDirection(SHEBA, FACING_NORTH + FACING_STEP, 0);
  Actor_ShowEmote(SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageAndWait(MIA, 0, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP);
  Engine_ActorSetAnimation(ACTOR_MENARDI, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_SetAttachedEffect(IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIMATION_NOD);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 40);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  VenusLighthouseAerie_FaceActor(MIA, FACING_NORTHWEST);
  VenusLighthouseAerie_ShowMessage(MIA);
  Engine_ActorFaceDirection(GARET, FACING_WEST, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  VenusLighthouseAerie_ShowMessage(GARET);
  Engine_ActorFaceDirection(SHEBA, FACING_EAST, 0);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Engine_ActorFaceDirection(IVAN, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Engine_ActorStartRepeatedMotion(ACTOR_SATUROS, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  // Save the rod owner for the defeat return path, then enter battle.
  rodOwner = PartyInventory_FindOwner(ITEM_SHAMANS_ROD);
  GameFlag_Set(rodOwner + FLAG_SHAMANS_ROD_OWNER);
  PartyInventory_Remove(ITEM_SHAMANS_ROD);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(IVAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(MIA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(ACTOR_MENARDI)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(ACTOR_SATUROS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetActionScript(ACTOR_PARTY_LEADER,
                        gVenusLighthouseAerie_StepBackWestAction);
  Actor_SetActionScript(GARET, gVenusLighthouseAerie_StepBackWestAction);
  Actor_SetActionScript(IVAN, gVenusLighthouseAerie_StepBackWestAction);
  Actor_SetActionScript(MIA, gVenusLighthouseAerie_StepBackWestAction);
  Actor_SetActionScript(ACTOR_MENARDI,
                        gVenusLighthouseAerie_StepBackEastAction);
  Actor_SetActionScript(ACTOR_SATUROS,
                        gVenusLighthouseAerie_StepBackEastAction);
  Actor_SetActionScript(ACTOR_FELIX, gVenusLighthouseAerie_StepBackWestAction);
  Actor_RunActionScriptAndWait(SHEBA, gVenusLighthouseAerie_StepBackWestAction);
  Battle_SetStart(BATTLE_START_SCENE);
  {
    s32 actor = (s32)&SceneId_VenusLighthouseAerie;

    Event_SetNextDestination(actor, VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE);
    Event_SetDefeatDestination(actor, VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT);
  }
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 1);
  GameFlag_Set(FLAG_AERIE_FIRST_BATTLE_BEGUN);
}

/// Flashes an actor palette and emits paired effects every fifteenth frame.
static void VenusLighthouseAerie_FlashActorPalette(struct FieldActor *actor) {
  if (*(s32 *)&gFrameCount & 2) {
    Engine_ObjectSetPartPalettes(actor, SPRITE_COLOR_WHITE);
  } else {
    Engine_ObjectSetPartPalettes(actor, SPRITE_COLOR_NORMAL);
  }
  if (Math_RemainderUnsigned(*(s32 *)&gFrameCount, 15) == 0) {
    VenusLighthouseAerie_SpawnLinkedPairEffects(actor);
  }
}

/// Cycles the actor palette and emits paired effects every fifteenth frame.
static void VenusLighthouseAerie_CycleActorPalette(struct FieldActor *actor) {
  if (gFrameCount & 1) {
    Engine_ObjectSetPartPalettes(actor,
                                 Math_RemainderUnsigned(gFrameCount >> 1, 6));
  }
  if (Math_RemainderUnsigned(gFrameCount, 15) == 0) {
    VenusLighthouseAerie_SpawnLinkedPairEffects(actor);
  }
}

#include "common/effect_vertical.h"

/// Spawns two vertical effects using the parent position and shared cached
/// tiles.
static void
VenusLighthouseAerie_SpawnLinkedPairEffects(struct FieldActor *parent) {
  struct FieldActor *pair[2];
  struct FieldActor *child;
  struct AnimationObject *sprite;
  struct FieldFxScene *sceneWork = gEffectWork;
  s32 effectIndex;

  Engine_AudioPlayCue(SE_131);
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

    effect->update = FieldEffect_AnimateVerticalNegative;
    effectSprite->image.priority = parent->sprite->image.priority;
  }
  {
    struct FieldActor *effect = pair[1];
    struct AnimationObject *effectSprite = effect->sprite;

    effectSprite->image.priority = parent->sprite->image.priority;
    effect->update = FieldEffect_AnimateVerticalPositive;
    effect->priorityFlags = 2;
  }
}

#include "common/effect_psynergy.h"

static void VenusLighthouseAerie_UpdateShebaPalette(void) {
  VenusLighthouseAerie_CycleActorPalette(Object_GetById(SHEBA));
}

/// Builds the horizontal velocity for an elliptical radial particle burst.
static inline void VenusLighthouseAerie_AimBurst(s32 velocity[3], s32 angle) {
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

/// Gives Felix, Sheba and the party the same reaction, then waits after Mia.
static inline void VenusLighthouseAerie_ShowGroupEmote(s32 emote,
                                                       s32 waitFrames) {
  Actor_ShowEmote(ACTOR_FELIX, emote, 0);
  Actor_ShowEmote(SHEBA, emote, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, emote, 0);
  Actor_ShowEmote(GARET, emote, 0);
  Actor_ShowEmote(IVAN, emote, 0);
  Actor_ShowEmote(MIA, emote, waitFrames);
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
  Actor_SetAnimation(ACTOR_SATUROS, SATUROS_ANIM_LIE);
  Actor_SetActionScript(ACTOR_SATUROS, gVenusLighthouseAerie_ScaleStepAction);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SATUROS), 0);
  actor = Actor_Get(ACTOR_MENARDI);
  actor->timer = 10;
  actor->facing = FACING_EAST;
  Actor_SetPosition(ACTOR_MENARDI, PIXELS(286), PIXELS(192));
  Actor_SetAnimation(ACTOR_MENARDI, MENARDI_ANIM_LIE);
  Actor_SetActionScript(ACTOR_MENARDI, gVenusLighthouseAerie_ScaleStepAction);
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
  VenusLighthouseAerie_FaceActor(GARET, FACING_SOUTHEAST);
  Event_SetMessage(MsgVenusBeatEm);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_LEFT | GARET);
  VenusLighthouseAerie_FaceActor(MIA, FACING_NORTHWEST);
  VenusLighthouseAerie_ShowMessage(MIA);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  VenusLighthouseAerie_FaceActor(IVAN, FACING_NORTHWEST);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_Get(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_FELIX, 280, 164);
  Event_Wait(1);
  Actor_Get(ACTOR_FELIX)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  VenusLighthouseAerie_ShowMessage(IVAN);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_RunRepeatedMotion(GARET, 1);
  VenusLighthouseAerie_ShowMessage(GARET);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_EAST);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_StartRepeatedMotion(MIA, 2);
  VenusLighthouseAerie_ShowMessage(MIA);
  Actor_SetAnimation(MIA, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(MIA);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_Jump(IVAN, 2, 20);
  VenusLighthouseAerie_ShowMessage(IVAN);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIMATION_NOD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_RunRepeatedMotion(GARET, 1);
  VenusLighthouseAerie_FaceActor(GARET, FACING_SOUTHEAST);
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
  VenusLighthouseAerie_FaceActor(MIA, FACING_NORTHWEST);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_EAST);
  Event_ShowMessage(ACTOR_FELIX, 0);
  Event_PlayCue(AUDIO_CUE_FADE_BGM);
  Event_Wait(40);
  if (sceneStep) {
    gEventWork->message++;
  }

  // Saturos and Menardi recover from their defeat before using the Venus Star.
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
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 40);
  Actor_SetAnimation(ACTOR_SATUROS, ANIMATION_STAND);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SATUROS), 1);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_SATUROS, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_WalkToAndWait(ACTOR_SATUROS, 300, 206);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_Wait(20);
  Actor_Get(ACTOR_SATUROS)->scaleX = -Q16_ONE;
  Event_PlayCue(SE_161);
  Actor_SetAnimation(ACTOR_SATUROS, SATUROS_ANIM_BREATHE);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_MENARDI, ANIMATION_STAND);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MENARDI), 1);
  Actor_Jump(ACTOR_MENARDI, 4, 40);
  Actor_SetSpeed(ACTOR_MENARDI, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_Get(ACTOR_MENARDI)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_MENARDI, 296, 186);
  Actor_SetSpeed(ACTOR_MENARDI, PIXELS(1) / 10, PIXELS(1) / 20);
  Actor_WalkToAndWait(ACTOR_MENARDI, 292, 186);
  Actor_Get(ACTOR_MENARDI)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_MENARDI)->scaleX = -Q16_ONE;
  Event_PlayCue(SE_161);
  Actor_SetAnimation(ACTOR_MENARDI, MENARDI_ANIM_STAND_WINDSWEPT);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_Wait(80);
  Actor_SetAnimationAndWait(MIA, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
  Event_Wait(40);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
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
  Actor_SetAnimation(ACTOR_SATUROS, ANIMATION_STAND);
  Event_Wait(10);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH + FACING_STEP);
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
    star->actor.retainSpriteCache = 1;
    star->actor.speed = PIXELS(8) / 5;
    star->actor.acceleration = 0xcccc;
    iconPixels = Heap_Allocate(17, 0x608);
    // Display the Venus Star icon before sending it toward the beacon.
    Item_LoadIcon(ITEM_VENUS_STAR);
    Vram_Load(sprite->slot, 128, iconPixels + 0x400);
    Heap_Release(17);
  }
  Actor_SetSpritePriority(ACTOR_STAR_CARRIER, 1);
  starCarrier = Actor_Get(ACTOR_STAR_CARRIER);
  starCarrier->x.fixed = actor->x.fixed;
  starCarrier->y.fixed = PIXELS(32);
  starCarrier->z.fixed = actor->z.fixed;
  starCarrier->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  starCarrier->speed = PIXELS(8) / 5;
  starCarrier->acceleration = PIXELS(4) / 5;
  starCarrier->scaleX = 3 * Q16_ONE / 4;
  starCarrier->scaleY = 3 * Q16_ONE / 4;
  if (star != NULL) {
    star->actor.motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    star->effect.velocity.y = PIXELS(3) / 5;
    star->effect.velocity.x = PIXELS(4) / 5;
    star->actor.velocityY = PIXELS(8);
    FieldObject_SetPosition(&star->actor, PIXELS(308), PIXELS(32), PIXELS(164));
  }
  Actor_MoveToAndWait(ACTOR_STAR_CARRIER, 308, 164);
  Actor_SetPosition(ACTOR_STAR_CARRIER, 0, 0);
  Actor_SetSpritePriority(ACTOR_FELIX, 0);
  if (star != NULL) {
    Event_PlayCue(SE_STAR_BOUNCE);
    Actor_SetSpriteFlags(&star->actor, 0);
    star->actor.velocityY = PIXELS(4);
    FieldObject_SetPosition(&star->actor, PIXELS(314), PIXELS(32), PIXELS(137));
    FieldObject_WaitForMove(&star->actor);
    Event_PlayCue(SE_STAR_BOUNCE);
    star->actor.sprite->image.priority = 1;
    star->actor.velocityY = PIXELS(6);
    FieldObject_SetPosition(&star->actor, PIXELS(285), PIXELS(32), PIXELS(146));
    FieldObject_WaitForMove(&star->actor);
    Event_PlayCue(SE_STAR_BOUNCE);
    star->actor.velocityY = PIXELS(5);
    FieldObject_SetPosition(&star->actor, PIXELS(300), PIXELS(32), PIXELS(154));
    FieldObject_WaitForMove(&star->actor);
    Task_Wait(6);
    star->actor.x.fixed = 0;
    star->actor.y.fixed = 0;
    star->actor.z.fixed = 0;
    VenusLighthouseAerie_StopActorMotion(&star->actor);
  }
  // Let the group react as the Venus Star reaches the beacon.
  VenusLighthouseAerie_ShowGroupEmote(EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_SetSpritePriority(ACTOR_FELIX, 1);
  Actor_Get(ACTOR_FELIX)->priorityFlags |= 1;
  Actor_StartRepeatedMotion(IVAN, 2);
  VenusLighthouseAerie_ShowMessage(IVAN);
  Actor_RunRepeatedMotion(MIA, 1);
  Event_ShowMessageAndWait(MIA, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_Wait(20);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 0);
  Actor_FaceDirection(SHEBA, FACING_SOUTHEAST + FACING_STEP, 0);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP);
  VenusLighthouseAerie_ShowGroupEmote(EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 40);
  Event_OpenMessage(GARET, 0);
  if (Event_ChooseYesNo(0, 0) == 0) {
    Actor_SetAnimation(GARET, ANIMATION_NOD);
    sceneStep = TRUE;
  } else {
    gEventWork->message++;
    Actor_SetAnimation(GARET, ANIMATION_SHAKE_HEAD);
    sceneStep = FALSE;
  }
  VenusLighthouseAerie_ShowMessage(GARET);
  if (sceneStep) {
    gEventWork->message++;
  }
  Event_Wait(20);

  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEAM_BOTTOM), 0);
  Actor_SetChildValue(ACTOR_BEAM_BOTTOM, SPRITE_COLOR_WHITE);
  actor = Actor_Get(ACTOR_BEAM_BOTTOM);
  actor->scaleY = -Q16_ONE;
  actor->scaleX = Q16_ONE / 10;
  actor->motionFlags = 0;
  actor->y.fixed = PIXELS(64);
  actor->x.fixed = PIXELS(304);
  actor->z.fixed = PIXELS(158);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BEAM_LOWER), 0);
  Actor_SetChildValue(ACTOR_BEAM_LOWER, SPRITE_COLOR_WHITE);
  actor = Actor_Get(ACTOR_BEAM_LOWER);
  actor->scaleY = -Q16_ONE;
  actor->scaleX = Q16_ONE / 10;
  actor->motionFlags = 0;
  actor->y.fixed = PIXELS(96);
  actor->x.fixed = PIXELS(304);
  actor->z.fixed = PIXELS(158);
  VenusLighthouseAerie_ShowGroupEmote(EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(SHEBA, FACING_EAST, 0);
  Actor_SetActionScript(ACTOR_BEAM_BOTTOM,
                        gVenusLighthouseAerie_BeamOpenAction);
  Actor_SetActionScript(ACTOR_BEAM_LOWER, gVenusLighthouseAerie_BeamOpenAction);
  Event_PlayCue(SE_145);
  Map_SetShake(PIXELS(6), PIXELS(6), Q16_ONE);
  ColorBuffer_ApplyTarget(0x4063ff, 0);
  ColorBuffer_Interpolate(16);
  Task_Wait(20);
  ColorBuffer_ApplyTarget(RGB(31, 31, 31), 0);
  ColorBuffer_Interpolate(24);
  Task_Wait(60);
  Task_AddCallback(VenusLighthouseAerie_SpawnUpperOrbitParticles,
                   TASK_PRIORITY_SCENE);
  Event_PlayCue(SE_141);
  Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  ColorBuffer_ApplyTarget(0x4063ff, 0);
  ColorBuffer_Interpolate(120);
  Actor_SetActionScript(ACTOR_BEAM_BOTTOM,
                        gVenusLighthouseAerie_BeamNarrowAction);
  Actor_SetActionScript(ACTOR_BEAM_LOWER,
                        gVenusLighthouseAerie_BeamNarrowAction);
  Task_Wait(120);
  Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  ColorBuffer_ApplyTarget(0x203210, 0);
  ColorBuffer_Interpolate(120);
  Task_Wait(120);
  Event_PlayCue(MUS_063);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  ColorBuffer_Interpolate(120);
  Task_Wait(120);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);

  Actor_SetAnimation(ACTOR_MENARDI, ANIMATION_STAND);
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
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_RunRepeatedMotion(MIA, 1);
  VenusLighthouseAerie_FaceActor(MIA, FACING_WEST);
  VenusLighthouseAerie_ShowMessage(MIA);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_StartRepeatedMotion(ACTOR_MENARDI, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  VenusLighthouseAerie_FaceActor(GARET, FACING_SOUTHWEST);
  VenusLighthouseAerie_ShowMessage(GARET);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH + FACING_STEP);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIMATION_NOD);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
  Actor_RunRepeatedMotion(GARET, 1);
  VenusLighthouseAerie_ShowMessage(GARET);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(GARET, 1);
  Actor_StartRepeatedMotion(IVAN, 1);
  Actor_RunRepeatedMotion(MIA, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  VenusLighthouseAerie_FaceActor(MIA, FACING_NORTHWEST);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 40);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 20);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIMATION_NOD);
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
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIMATION_NOD);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_SetAnimation(ACTOR_MENARDI, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 80);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIMATION_NOD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAnimation(ACTOR_MENARDI, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Actor_SetAnimation(ACTOR_SATUROS, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_Wait(20);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_SetAnimation(ACTOR_MENARDI, ANIMATION_NOD);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIMATION_NOD);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST + FACING_STEP, 20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_WINDOW_BELOW |
                                   SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX);
  Actor_SetSpeed(SHEBA, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(1), PIXELS(1) / 2);
  Actor_SetActionScript(ACTOR_FELIX, gVenusLighthouseAerie_WalkOutAction);
  Event_Wait(20);
  Actor_SetActionScript(SHEBA, gVenusLighthouseAerie_WalkOutAction);
  VenusLighthouseAerie_FaceActor(GARET, FACING_SOUTHWEST);
  Actor_StartRepeatedMotion(GARET, 2);
  VenusLighthouseAerie_ShowMessage(GARET);
  Event_PlayCue(AUDIO_CUE_FADE_BGM);
  // Flash Saturos and Menardi and release their matching radial bursts.
  ColorBuffer_ApplyTarget(0x40250d, 1);
  ColorBuffer_Interpolate(40);
  VenusLighthouseAerie_FaceActor(ACTOR_SATUROS, FACING_NORTH + FACING_STEP);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 0);
  VenusLighthouseAerie_FlashScreen();
  Actor_SetChildValue(ACTOR_SATUROS, SPRITE_COLOR_WHITE);
  Actor_SetChildValue(ACTOR_MENARDI, SPRITE_COLOR_WHITE);
  Event_Wait(20);
  Actor_SetChildValue(ACTOR_SATUROS, CHILD_VALUE_BLINK);
  Actor_SetChildValue(ACTOR_MENARDI, CHILD_VALUE_BLINK);
  Event_Wait(20);
  VenusLighthouseAerie_FlashScreen();
  Actor_Jump(MIA, 2, 20);
  VenusLighthouseAerie_ShowMessage(MIA);
  VenusLighthouseAerie_FaceActor(ACTOR_MENARDI, FACING_EAST);
  VenusLighthouseAerie_ShowMessage(ACTOR_MENARDI);
  VenusLighthouseAerie_FlashScreen();

  menardi = Actor_Get(ACTOR_MENARDI);
  menardiBurst.palette = 7;
  menardiBurst.update = VenusLighthouseAerie_UpdateDriftingParticle;
  menardiBurst.startScaleX = Q16_ONE;
  menardiBurst.startScaleY = Q16_ONE;
  for (sceneStep = 0; sceneStep <= 16; sceneStep++) {
    VenusLighthouseAerie_AimBurst(velocity, sceneStep << 12);
    FieldEffect_Spawn(menardi->x.fixed, menardi->y.fixed, menardi->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE,
                      &menardiBurst);
  }
  Event_PlayCue(SE_212);
  Task_Wait(6);
  VenusLighthouseAerie_FlashScreen();
  saturos = Actor_Get(ACTOR_SATUROS);
  saturosBurst.palette = 7;
  saturosBurst.update = VenusLighthouseAerie_UpdateDriftingParticle;
  saturosBurst.startScaleX = Q16_ONE;
  saturosBurst.startScaleY = Q16_ONE;
  for (sceneStep = 0; sceneStep <= 16; sceneStep++) {
    VenusLighthouseAerie_AimBurst(velocity, sceneStep << 12);
    FieldEffect_Spawn(saturos->x.fixed, saturos->y.fixed, saturos->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE,
                      &saturosBurst);
  }
  Event_PlayCue(SE_212);
  Actor_Jump(IVAN, 6, 20);
  Event_PlayCue(MUS_FUSION_DRAGON_BATTLE);
  VenusLighthouseAerie_ShowMessage(IVAN);
  Actor_SetAnimation(ACTOR_SATUROS, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  VenusLighthouseAerie_FlashScreen();
  Actor_SetSpeed(ACTOR_SATUROS, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_SetSpeed(ACTOR_MENARDI, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_Get(ACTOR_SATUROS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_MENARDI)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_SATUROS, 294, 196);
  Actor_SetDestination(ACTOR_MENARDI, 294, 196);
  // Slide the two transformed actors together, then start the fusion battle.
  Task_AddCallback(VenusLighthouseAerie_UpdateBeamActors, TASK_PRIORITY_SCENE);
  Actor_StartRepeatedMotion(GARET, 2);
  Event_ShowMessage(GARET, 0);
  GameFlag_Set(FLAG_SCENE_TEMP_34);
  Event_ShowMessage(IVAN, 0);
  GameFlag_Set(FLAG_SCENE_TEMP_35);
  VenusLighthouseAerie_FlashScreen();
  Event_Wait(20);
  VenusLighthouseAerie_FlashScreen();
  Event_Wait(20);
  Battle_SetStart(BATTLE_START_SCENE);
  Event_SetNextDestination((s32)&SceneId_VenusLighthouseAerie,
                           VENUS_LIGHTHOUSE_AERIE_AFTER_SECOND_BATTLE);
  Event_SetDefeatDestination((s32)&SceneId_VenusLighthouseAerie,
                             VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 0);
  Party_RestorePpOfLitElements();
  GameFlag_Set(FLAG_AERIE_SECOND_BATTLE_BEGUN);
}

/// Flashes the object palette white, then restores the transformation colour.
static void VenusLighthouseAerie_FlashScreen(void) {
  Engine_AudioPlayCue(SE_187);
  Engine_ColorBufferApplyTarget(0x7fff, 1);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(4);
  Engine_ColorBufferApplyTarget(0x40250d, 1);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
}

/// Orbits a particle around the upper beam and keeps its target at that
/// position.
static void
VenusLighthouseAerie_UpdateUpperOrbit(union FieldObject *fieldObject) {
  struct FieldActor *effect = &fieldObject->actor;
  struct FieldActor *center = Object_GetById(ACTOR_BEAM_BOTTOM);
  u16 angle = effect->timer;

  effect->x.fixed =
      center->x.fixed + Engine_MathCos(angle) * (effect->speed + 3);
  effect->z.fixed = center->z.fixed + Engine_MathSin(angle) * 2;
  effect->targetX = effect->x.fixed;
  effect->targetZ = effect->z.fixed;
  effect->timer -= 0x800;
}

/// Orbits a particle around the rising beam using its rise counter as a radius.
static void
VenusLighthouseAerie_UpdateRisingOrbit(union FieldObject *fieldObject) {
  struct FieldActor *actor = &fieldObject->actor;
  struct FieldActor *anchor = Object_GetById(ACTOR_PARTICLE_SOURCE);
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

/// Emits rising particles around the upper beam at the current beacon stage.
static void VenusLighthouseAerie_SpawnUpperOrbitParticles(void) {
  struct FieldActor *center;
  struct FieldActor *effect;
  struct AnimationObject *sprite;
  s32 angle;

  if (GameFlag_IsSet(FLAG_SCENE_TEMP_36) == 0 &&
      Math_RemainderUnsigned(gFrameCount, 3) != 0) {
    return;
  }
  center = Actor_Get(ACTOR_BEAM_BOTTOM);
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_36) != 0) {
    effect = Object_Create(SPRITE_PARTICLE, center->x.fixed,
                           (s32)((u32)(Random_Next() << 8) >> 16 << 16) +
                               center->y.fixed - PIXELS(28),
                           center->z.fixed);
  } else {
    effect = Object_Create(SPRITE_PARTICLE, center->x.fixed,
                           (s32)((u32)(Random_Next() << 6) >> 16 << 16) +
                               center->y.fixed - PIXELS(28),
                           center->z.fixed);
  }
  if (effect != 0) {
    sprite = effect->sprite;
    Object_SetScript(effect, gVenusLighthouseAerie_OrbitParticleScript);
    ObjectGroup_SetChildValue(effect, SPRITE_COLOR_GRB);
    effect->motionFlags = 0;
    angle = Random_Next() & 0xffff000;
    effect->timer = angle;
    effect->work = 0;
    effect->update = VenusLighthouseAerie_UpdateUpperOrbit;
    effect->speed =
        Engine_MathSin((u32)(Random_Next() * 0xffff) >> 20) * 24 >> 16;
    sprite->flags = 0;
    sprite->image.priority = 1;
  }
}

/// Tests whether all three movement targets are unset.
static __inline__ s32 Actor_IsParked(struct FieldActor *actor) {
  if (actor->targetX == ACTOR_NO_TARGET && actor->targetY == actor->targetX &&
      actor->targetZ == actor->targetY) {
    return 1;
  }
  return 0;
}

/// Pulses and slides Saturos and Menardi while their beam flags are active.
static void VenusLighthouseAerie_UpdateBeamActors(void) {
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
    if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_35) != 0) {
      Engine_ActorSetChildValue(ACTOR_SATUROS, SPRITE_COLOR_WHITE);
      Engine_ActorSetChildValue(ACTOR_MENARDI, SPRITE_COLOR_WHITE);
      if (saturos->scaleX < 5 * Q16_ONE / 4) {
        saturos->scaleX += Q16_ONE / 128;
        saturos->scaleY += Q16_ONE / 128;
        menardi->scaleX += Q16_ONE / 128;
        menardi->scaleY += Q16_ONE / 128;
      }
    } else {
      if ((*(s32 *)&gFrameCount & 2) != 0) {
        Engine_ActorSetChildValue(ACTOR_SATUROS, SPRITE_COLOR_HIDDEN);
        Engine_ActorSetChildValue(ACTOR_MENARDI, SPRITE_COLOR_NORMAL);
      } else {
        Engine_ActorSetChildValue(ACTOR_SATUROS, SPRITE_COLOR_NORMAL);
        Engine_ActorSetChildValue(ACTOR_MENARDI, SPRITE_COLOR_HIDDEN);
      }
    }
    if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_34) != 0) {
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
  Engine_EventBegin();
  VenusLighthouseAerie_ShowSaturosAndMenardiFall();
  VenusLighthouseAerie_DiscussSaturosAndMenardiDefeat();
  VenusLighthouseAerie_EruptBeacon();
  Engine_EventEnd();
}

/// A look back from the world map (VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT,
/// after the world map's lighthouse transition): Felix cries out as Sheba
/// clings to the broken aerie, then the world map returns with its music still
/// playing (VENUS_LIGHTHOUSE_AERIE_TO_WORLD_MAP).
static void VenusLighthouseAerie_ShowShebaInDanger(void) {
  Engine_EventBegin();
  VenusLighthouseAerie_RestorePartyOnBrokenAerie();
  VenusLighthouseAerie_TryToPreventFall();
  Engine_GameFlagSet(FLAG_KEEP_MUSIC);

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  gEventWork->transitionFrames = 24;

  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(VENUS_LIGHTHOUSE_AERIE_TO_WORLD_MAP);
  Engine_EventEnd();
}

/// Back from the world map (VENUS_LIGHTHOUSE_AERIE_FROM_WORLD_MAP): stages the
/// broken aerie for Sheba's fall and the party's escape, plays them, sets
/// FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE and leaves for Lalivero
/// (VENUS_LIGHTHOUSE_AERIE_TO_LALIVERO_FARANS_HOUSE).
static void VenusLighthouseAerie_StageShebasFall(void) {
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
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_LEADER_KNOCKED_DOWN);
  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  leader->x.fixed = PIXELS(346);
  leader->z.fixed = PIXELS(205);
  leader->y.fixed = PIXELS(32);
  leader->facing = FACING_SOUTHWEST;
  VenusLighthouseAerie_StopActorMotion(leader);
  Engine_ActorSetAnimation(GARET, ANIMATION_PARTY_KNOCKED_DOWN);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 0);
  actor = Actor_Get(GARET);
  actor->x.fixed = PIXELS(356);
  actor->z.fixed = PIXELS(192);
  actor->facing = FACING_NORTHWEST;
  actor->y.fixed = PIXELS(32);
  VenusLighthouseAerie_StopActorMotion(actor);
  Engine_ActorSetAnimation(IVAN, ANIMATION_PARTY_KNOCKED_DOWN);
  Engine_ActorSetSpriteFlags(Actor_Get(IVAN), 0);
  actor = Actor_Get(IVAN);
  actor->x.fixed = PIXELS(360);
  actor->facing = FACING_SOUTHEAST;
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(222);
  VenusLighthouseAerie_StopActorMotion(actor);
  Engine_ActorSetAnimation(MIA, ANIMATION_PARTY_KNOCKED_DOWN);
  Engine_ActorSetSpriteFlags(Actor_Get(MIA), 0);
  actor = Actor_Get(MIA);
  actor->x.fixed = PIXELS(334);
  actor->facing = FACING_WEST;
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(222);
  VenusLighthouseAerie_StopActorMotion(actor);
  Actor_SetPosition(ACTOR_FELIX, PIXELS(196), PIXELS(220));
  Engine_ActorSetAnimation(ACTOR_FELIX, FELIX_ANIM_KNEEL);
  Actor_SetPosition(SHEBA, PIXELS(188), PIXELS(316));
  Engine_ActorSetAnimation(SHEBA, SHEBA_ANIM_HANG_ONE_HAND);
  Engine_ActorSetSpriteFlags(Actor_Get(SHEBA), 0);
  actor = Actor_Get(ACTOR_BEACON_STATUE_SOUTHWEST);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(ACTOR_BEACON_STATUE_NORTHWEST);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Actor_Get(ACTOR_BEACON_STATUE_NORTHEAST);
  actor->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(ACTOR_BEACON_STATUE_SOUTHEAST);
  actor->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTICLE_SOURCE), 0);
  Object_GetById(ACTOR_PARTICLE_SOURCE)->motionFlags = ACTOR_FLOATS;
  Actor_SetChildValue(ACTOR_PARTICLE_SOURCE, SPRITE_COLOR_GBR);
  Actor_Get(ACTOR_PARTICLE_SOURCE)->y.fixed = PIXELS(40);
  FieldScheduler_AddCallback(VenusLighthouseAerie_SpawnSideOrbitParticles,
                             TASK_PRIORITY_SCENE);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  gEventWork->transitionFrames = 24;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  VenusLighthouseAerie_PlayShebasFall();
  GameFlag_Set(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE);
  Engine_EventRequestExit(VENUS_LIGHTHOUSE_AERIE_TO_LALIVERO_FARANS_HOUSE);
}

/// Shows Saturos and Menardi falling into the beacon after their final defeat.
static void VenusLighthouseAerie_ShowSaturosAndMenardiFall(void) {
  struct FieldActor *actor;

  Event_PlayCue(SE_141);
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

  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_BEAM_BOTTOM), 0);
  Actor_SetChildValue(ACTOR_BEAM_BOTTOM, SPRITE_COLOR_WHITE);
  Engine_ActorSetSpritePriority(ACTOR_BEAM_BOTTOM, 1);
  actor = Actor_Get(ACTOR_BEAM_BOTTOM);
  actor->scaleY = -Q16_ONE;
  actor->scaleX = Q16_ONE / 5;
  actor->motionFlags = 0;
  actor->x.fixed = PIXELS(304);
  actor->y.fixed = PIXELS(2);
  actor->z.fixed = PIXELS(96);

  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_BEAM_LOWER), 0);
  Actor_SetChildValue(ACTOR_BEAM_LOWER, SPRITE_COLOR_WHITE);
  Engine_ActorSetSpritePriority(ACTOR_BEAM_LOWER, 1);
  actor = Actor_Get(ACTOR_BEAM_LOWER);
  actor->scaleY = -Q16_ONE;
  actor->scaleX = Q16_ONE / 5;
  actor->motionFlags = 0;
  actor->x.fixed = PIXELS(304);
  actor->y.fixed = PIXELS(34);
  actor->z.fixed = PIXELS(96);

  FieldScheduler_AddCallback(VenusLighthouseAerie_SpawnUpperOrbitParticles,
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
  VenusLighthouseAerie_ShowMessage(ACTOR_SATUROS);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 40);
  Event_PlayCue(AUDIO_CUE_FADE_BGM);

  actor = Actor_Get(ACTOR_SATUROS);
  actor->x.fixed = PIXELS(308);
  actor->y.fixed = PIXELS(28);
  actor->z.fixed = PIXELS(152);
  Engine_ActorSetAnimation(ACTOR_SATUROS, SATUROS_ANIM_TIP);
  Engine_EventWait(20);
  actor->x.fixed = PIXELS(306);
  actor->y.fixed = PIXELS(28);
  actor->z.fixed = PIXELS(152);
  Engine_ActorSetAnimation(ACTOR_SATUROS, SATUROS_ANIM_SLUMP);
  Engine_EventWait(12);
  actor->x.fixed = PIXELS(304);
  actor->y.fixed = PIXELS(21);
  actor->z.fixed = PIXELS(152);
  Engine_ActorSetAnimation(ACTOR_SATUROS, SATUROS_ANIM_SINK);
  Engine_EventWait(8);
  Engine_ActorDestroy(ACTOR_SATUROS);

  actor = Actor_Get(ACTOR_MENARDI);
  actor->x.fixed = PIXELS(294);
  actor->y.fixed = PIXELS(28);
  actor->z.fixed = PIXELS(152);
  Engine_ActorSetAnimation(ACTOR_MENARDI, MENARDI_ANIM_TIP);
  Engine_EventWait(20);
  actor->x.fixed = PIXELS(300);
  actor->y.fixed = PIXELS(27);
  actor->z.fixed = PIXELS(152);
  Engine_ActorSetAnimation(ACTOR_MENARDI, MENARDI_ANIM_SLUMP);
  Engine_EventWait(12);
  actor->x.fixed = PIXELS(304);
  actor->y.fixed = PIXELS(17);
  actor->z.fixed = PIXELS(152);
  Engine_ActorSetAnimation(ACTOR_MENARDI, MENARDI_ANIM_SINK);
  Engine_EventWait(8);
  Engine_ActorDestroy(ACTOR_MENARDI);
  Engine_EventWait(160);
}

/// Lets the party discuss the battle while Felix and Sheba remain on the aerie.
static void VenusLighthouseAerie_DiscussSaturosAndMenardiDefeat(void) {
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
  VenusLighthouseAerie_FaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH);
  answerAccepted = 0;
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD);
    answerAccepted = 1;
  } else {
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    gEventWork->message += 1;
  }
  VenusLighthouseAerie_ShowMessage(IVAN);
  if (answerAccepted != 0) {
    gEventWork->message += 1;
  }
  Actor_SetSpeed(GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  garetSpeaker = SPEAKER_PORTRAIT_LEFT | GARET;
  Actor_WalkToAndWait(GARET, 321, 174);
  VenusLighthouseAerie_FaceActor(GARET, FACING_SOUTHEAST);
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  VenusLighthouseAerie_FaceActor(MIA, FACING_NORTH);
  VenusLighthouseAerie_FaceActor(GARET, FACING_SOUTH);
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 20);
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 40);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | GARET, 0, 40);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 20);
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  Event_PlayCue(AUDIO_CUE_FADE_BGM);
  Engine_ActorSetAnimation(MIA, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(MIA);
  Actor_FaceDirection(GARET, FACING_SOUTH, 80);
  Actor_FaceDirection(MIA, FACING_SOUTHWEST, 80);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  VenusLighthouseAerie_FaceActor(IVAN, FACING_NORTH);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 40);
  Engine_ActorJump(IVAN, 4, 60);
  VenusLighthouseAerie_FaceActor(GARET, FACING_SOUTHEAST);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(garetSpeaker);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 40);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorJump(GARET, 4, 40);
  VenusLighthouseAerie_ShowMessage(GARET);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetDestination(ACTOR_FELIX, 200, 188);
  Actor_SetDestination(SHEBA, 200, 204);
  Camera_SetSpeed(PIXELS(16) / 5, PIXELS(2) / 5);
  Camera_MoveTo(PIXELS(252), 0, PIXELS(190), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Event_PlayCue(MUS_023);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Engine_EventWait(20);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 20);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAttachedEffect(SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  miaSpeaker = SPEAKER_PORTRAIT_RIGHT | MIA;
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  VenusLighthouseAerie_ShowMessage(miaSpeaker);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 2);
  Engine_EventWait(20);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | IVAN);
  VenusLighthouseAerie_FaceActor(ACTOR_FELIX, FACING_NORTHEAST);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  VenusLighthouseAerie_ShowMessage(GARET);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Event_ShowMessageAndWait(ACTOR_FELIX, 0, 20);
  VenusLighthouseAerie_ShowMessage(miaSpeaker);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_SetAttachedEffect(IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  VenusLighthouseAerie_ShowMessage(SPEAKER_PORTRAIT_RIGHT | IVAN);
  Engine_ActorSetAnimation(ACTOR_FELIX, ANIMATION_NOD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  VenusLighthouseAerie_ShowMessage(GARET);
  Engine_ActorSetAnimation(ACTOR_FELIX, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorJump(MIA, 4, 20);
  Event_ShowMessageAndWait(miaSpeaker, 0, 20);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
}

static s32 sTransitionTimer;
static s32 sTransitionStep;

/// Raises the beacon effects and splits the aerie beneath the group.
static void VenusLighthouseAerie_EruptBeacon(void) {
  struct FieldActor *lower;
  struct FieldActor *middle;
  struct FieldActor *upper;
  struct FieldActor *actor;

  Engine_AudioPlayCue(MUS_SILENCE);
  Engine_AudioPlayCue(SE_STOP_SLOT3);
  Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
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
  Object_GetById(ACTOR_BEAM_BOTTOM)->scaleX = Q16_ONE / 10;
  Object_GetById(ACTOR_BEAM_LOWER)->scaleX = Q16_ONE / 10;
  Actor_SetActionScript(ACTOR_BEAM_BOTTOM,
                        gVenusLighthouseAerie_BeamOpenAction);
  Actor_SetActionScript(ACTOR_BEAM_LOWER, gVenusLighthouseAerie_BeamOpenAction);
  Engine_AudioPlayCue(SE_145);
  Map_SetShake(PIXELS(6), PIXELS(6), Q16_ONE);
  ColorBuffer_ApplyTarget(0x4063ff, 0);
  Engine_ColorBufferInterpolate(16);
  Engine_TaskWait(20);
  Engine_ColorBufferApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(24);
  Engine_TaskWait(60);
  Engine_AudioPlayCue(SE_141);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_36);
  Object_GetById(ACTOR_BEAM_BOTTOM)->y.fixed = -PIXELS(96);
  Object_GetById(ACTOR_BEAM_LOWER)->y.fixed = -PIXELS(64);
  Engine_ActorSetChildValue(ACTOR_BEAM_MIDDLE, SPRITE_COLOR_WHITE);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_BEAM_MIDDLE), 0);
  // Stack three more beam segments under the rising upper pair.
  lower = Object_GetById(ACTOR_BEAM_MIDDLE);
  lower->scaleY = -Q16_ONE;
  lower->scaleX = Object_GetById(ACTOR_BEAM_BOTTOM)->scaleX;
  lower->motionFlags = 0;
  lower->x.fixed = PIXELS(304);
  lower->y.fixed = -PIXELS(32);
  lower->z.fixed = PIXELS(96);
  Engine_ActorSetChildValue(ACTOR_BEAM_UPPER, SPRITE_COLOR_WHITE);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_BEAM_UPPER), 0);
  middle = Object_GetById(ACTOR_BEAM_UPPER);
  middle->scaleY = -Q16_ONE;
  middle->scaleX = Object_GetById(ACTOR_BEAM_BOTTOM)->scaleX;
  middle->motionFlags = 0;
  middle->x.fixed = PIXELS(304);
  middle->y.fixed = 0;
  middle->z.fixed = PIXELS(96);
  Engine_ActorSetChildValue(ACTOR_BEAM_TOP, SPRITE_COLOR_WHITE);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_BEAM_TOP), 0);
  upper = Object_GetById(ACTOR_BEAM_TOP);
  upper->scaleY = -Q16_ONE;
  upper->scaleX = Object_GetById(ACTOR_BEAM_BOTTOM)->scaleX;
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
  actor = Object_GetById(ACTOR_BEACON_STATUE_SOUTHWEST);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(ACTOR_BEACON_STATUE_NORTHWEST);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(ACTOR_BEACON_STATUE_NORTHEAST);
  actor->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(ACTOR_BEACON_STATUE_SOUTHEAST);
  actor->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->x.fixed += PIXELS(16);
  actor->z.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(GARET);
  actor->x.fixed += PIXELS(16);
  actor->z.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(IVAN);
  actor->x.fixed += PIXELS(16);
  actor->z.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(MIA);
  actor->x.fixed += PIXELS(16);
  actor->z.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  Actor_SetPosition(ACTOR_FELIX, PIXELS(196), PIXELS(220));
  Engine_ActorSetAnimation(ACTOR_FELIX, FELIX_ANIM_KNEEL);
  Actor_SetPosition(SHEBA, PIXELS(188), PIXELS(316));
  Engine_ActorSetAnimation(SHEBA, SHEBA_ANIM_HANG_ONE_HAND);
  Engine_ActorSetSpriteFlags(Object_GetById(SHEBA), 0);
  Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  Engine_ColorBufferApplyTarget(0x4063ff, 0);
  Engine_ColorBufferInterpolate(120);
  Actor_SetActionScript(ACTOR_BEAM_BOTTOM,
                        gVenusLighthouseAerie_BeamRiseAction);
  Actor_SetActionScript(ACTOR_BEAM_LOWER, gVenusLighthouseAerie_BeamRiseAction);
  Actor_SetActionScript(ACTOR_BEAM_MIDDLE,
                        gVenusLighthouseAerie_BeamRiseAction);
  Actor_SetActionScript(ACTOR_BEAM_UPPER, gVenusLighthouseAerie_BeamRiseAction);
  Actor_SetActionScript(ACTOR_BEAM_TOP, gVenusLighthouseAerie_BeamRiseAction);
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
  Object_GetById(ACTOR_BEAM_BOTTOM)->scaleY = Q16_ONE / 50;
  Actor_SetActionScript(ACTOR_BEAM_LOWER,
                        gVenusLighthouseAerie_BeamCloseAction);
  Actor_SetActionScript(ACTOR_BEAM_MIDDLE,
                        gVenusLighthouseAerie_BeamCloseAction);
  Actor_SetActionScript(ACTOR_BEAM_UPPER,
                        gVenusLighthouseAerie_BeamCloseAction);
  Actor_RunActionScriptAndWait(ACTOR_BEAM_TOP,
                               gVenusLighthouseAerie_BeamCloseAction);
  Engine_AudioPlayCue(AUDIO_CUE_FADE_SLOT3);
  Engine_ActorSetChildValue(ACTOR_BEAM_BOTTOM, SPRITE_COLOR_HIDDEN);
  Engine_EventWait(20);
  Actor_RunActionScriptAndWait(ACTOR_BEAM_BOTTOM,
                               gVenusLighthouseAerie_BeamEndAction);
  Scheduler_RemoveCallback(VenusLighthouseAerie_SpawnUpperOrbitParticles);
  Engine_ActorJump(IVAN, 2, 20);
  VenusLighthouseAerie_ShowMessage(IVAN);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 20);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  VenusLighthouseAerie_ShowMessage(GARET);
  Actor_SetSpeed(MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  Engine_ActorWalkToAndWait(MIA, 326, 220);
  Engine_EventWait(40);
  Engine_ActorSetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT |
                                         ATTACHED_EFFECT_ONCE);
  VenusLighthouseAerie_ShowMessage(MIA);
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
  Object_GetById(ACTOR_PARTICLE_SOURCE)->motionFlags = 0;
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTICLE_SOURCE), 0);
  Engine_ActorSetChildValue(ACTOR_PARTICLE_SOURCE, SPRITE_COLOR_WHITE);
  Engine_ActorSetSpritePriority(ACTOR_PARTICLE_SOURCE, 2);
  sTransitionStep = 0;
  sTransitionTimer = 240;
  FieldScheduler_AddCallback(VenusLighthouseAerie_UpdateBeaconRise,
                             TASK_PRIORITY_SCENE);
  do {
    Engine_TaskWait(1);
  } while (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_37) == 0);
  Engine_GameFlagSet(FLAG_SKIP_SCENE_CHANGE_SOUND);
  Engine_EventWait(30);
  Engine_GameFlagSet(FLAG_KEEP_MUSIC);
  ObjectTable_Snapshot();
  Event_SetDestination((s32)&SceneId_WorldMap,
                       WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT);
  {
    u16 white = 0x7fff;

    PLTT_BACKDROP = white;
  }
  gEventWork->transitionFrames = 1;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
}

/// Places the party on the broken platform and resumes its falling effects.
static void VenusLighthouseAerie_RestorePartyOnBrokenAerie(void) {
  struct FieldActor *actor;

  Engine_ActorDestroy(ACTOR_SATUROS);
  Engine_ActorDestroy(ACTOR_MENARDI);
  Event_PlayCue(SE_141);
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
  Event_PlayCue(SE_ITEM_BREAK);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_LEADER_KNOCKED_DOWN);
  Engine_ActorSetAnimation(GARET, ANIMATION_PARTY_KNOCKED_DOWN);
  Engine_ActorSetAnimation(IVAN, ANIMATION_PARTY_KNOCKED_DOWN);
  Engine_ActorSetAnimation(MIA, ANIMATION_PARTY_KNOCKED_DOWN);
  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 0);
  Engine_ActorSetSpriteFlags(Actor_Get(IVAN), 0);
  Engine_ActorSetSpriteFlags(Actor_Get(MIA), 0);

  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->x.fixed = PIXELS(346);
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(205);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor->riseEnabled = 0;
  actor->velocityY = PIXELS(2);

  actor = Actor_Get(GARET);
  actor->x.fixed = PIXELS(356);
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(192);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor->riseEnabled = 0;
  actor->velocityY = PIXELS(2);

  actor = Actor_Get(IVAN);
  actor->x.fixed = PIXELS(360);
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(222);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor->riseEnabled = 0;
  actor->velocityY = PIXELS(2);

  actor = Actor_Get(MIA);
  actor->x.fixed = PIXELS(334);
  actor->y.fixed = PIXELS(32);
  actor->z.fixed = PIXELS(222);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor->riseEnabled = 0;
  actor->velocityY = PIXELS(2);

  Actor_Get(ACTOR_FELIX)->riseCounter = 0;
  Actor_Get(SHEBA)->riseCounter = 0;
  Actor_Get(ACTOR_PARTICLE_SOURCE)->motionFlags |= 4;
  Actor_SetChildValue(ACTOR_PARTICLE_SOURCE, SPRITE_COLOR_GBR);
  FieldScheduler_AddCallback(VenusLighthouseAerie_SpawnSideOrbitParticles,
                             TASK_PRIORITY_SCENE);
  gEventWork->transitionFrames = 1;
  Engine_EventOpenScreen();
  ColorBuffer_ApplySource(RGB(31, 31, 31), 0);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(40);
  Engine_TaskWait(60);
}

/// Shows Felix urging Sheba to hold on at the edge of the platform.
static void VenusLighthouseAerie_TryToPreventFall(void) {
  Engine_EventSetMessage(MsgVenusNoooo);
  VenusLighthouseAerie_ShowMessage(ACTOR_FELIX);
  Event_PlayCue(MUS_062);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Camera_SetSpeed(PIXELS(24) / 5, PIXELS(3) / 5);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(192), -PIXELS(64), PIXELS(238), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 40);
  Engine_ActorRunRepeatedMotion(SHEBA, 3);
  VenusLighthouseAerie_ShowMessage(SHEBA);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 80);
  Actor_SetAttachedEffect(SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(SHEBA, 2);
  VenusLighthouseAerie_ShowMessage(SHEBA);
}

/// Shows Sheba falling, Felix jumping after her and the party escaping: it
/// resolves to find Jenna and the others at Idejima, and a long search finds
/// no sign of Jenna.
static void VenusLighthouseAerie_PlayShebasFall(void) {
  struct FieldActor *actor;
  s32 stepPending;

  stepPending = 0;
  // Sheba loses her grip while Felix tries to keep her on the platform.
  Engine_EventSetMessage(MsgVenusWhyHappeningProtectVenusLighthouse);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 20);
  Engine_ActorRunRepeatedMotion(SHEBA, 2);
  Event_ShowMessageAndWait(SHEBA, 0, 20);
  Engine_ActorSetAnimation(SHEBA, SHEBA_ANIM_HANG);
  Engine_EventWait(10);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 40);
  Engine_ActorSetAnimation(SHEBA, SHEBA_ANIM_SLIP);
  Event_ShowMessageAndWait(SHEBA, 0, 20);
  Event_PlayCue(AUDIO_CUE_FADE_BGM);
  Engine_ActorRunRepeatedMotion(SHEBA, 2);
  Actor_SetActionScript(SHEBA, gVenusLighthouseAerie_StepBackAction);
  Engine_EventWait(20);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 3);
  Object_RefreshSelectorById(SHEBA);
  Engine_EventWait(160);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Engine_EventWait(20);
  Object_GetById(ACTOR_FELIX)->facing = FACING_SOUTH + FACING_STEP;
  Engine_ActorSetAnimation(ACTOR_FELIX, 0);
  Engine_EventWait(80);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, ANIMATION_SHAKE_HEAD);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(ACTOR_FELIX, 0, 40);
  Engine_ActorSetAnimation(ACTOR_FELIX, FELIX_ANIM_KNEEL);
  Engine_EventWait(10);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FELIX), 0);
  Engine_ActorSetAnimation(ACTOR_FELIX, 0);
  Engine_ActorJump(ACTOR_FELIX, 6, 0);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(3), PIXELS(3) / 2);
  Object_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  actor = Object_GetById(ACTOR_FELIX);
  // Felix jumps after Sheba; the remaining party watches from the far side.
  actor->update = VenusLighthouseAerie_StepSpriteRotation;
  actor->facing = FACING_WEST;
  Actor_MoveToAndWait(ACTOR_FELIX, 184, 237);
  Actor_RunActionScriptAndWait(ACTOR_FELIX, gVenusLighthouseAerie_LeapAction);
  Engine_EventWait(120);
  VenusLighthouseAerie_ShowMessage(ACTOR_PARTY_LEADER);
  Event_PlayCue(MUS_072);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(342), PIXELS(32), PIXELS(212), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(40);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 1);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_STAND);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 60);
  Object_GetById(ACTOR_PARTY_LEADER)->timer = 10;
  Actor_SetActionScript(ACTOR_PARTY_LEADER,
                        gVenusLighthouseAerie_ScaleStepAction);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  Event_ShowMessageAndWait(GARET, 0, 40);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(20);
  VenusLighthouseAerie_ShowMessage(IVAN);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  VenusLighthouseAerie_ShowMessage(MIA);
  Engine_ActorSetSpriteFlags(Object_GetById(GARET), 1);
  Engine_ActorSetAnimation(GARET, ANIMATION_STAND);
  Engine_ActorJump(GARET, 6, 60);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(20);
  Engine_ActorSetSpriteFlags(Object_GetById(IVAN), 1);
  Engine_ActorSetAnimation(IVAN, ANIMATION_STAND);
  Engine_ActorJump(IVAN, 6, 40);
  Engine_ActorSetSpriteFlags(Object_GetById(MIA), 1);
  Engine_ActorSetAnimation(MIA, ANIMATION_STAND);
  Engine_ActorJump(MIA, 6, 60);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 60);
  Engine_ActorSetAnimation(GARET, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(GARET);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 20);
  Engine_ActorSetAnimation(IVAN, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(IVAN);
  VenusLighthouseAerie_FaceActor(MIA, FACING_SOUTHWEST);
  Engine_ActorSetAnimation(MIA, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(MIA, 0, 80);
  VenusLighthouseAerie_FaceActor(GARET, FACING_SOUTH);
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
  VenusLighthouseAerie_ShowMessage(MIA);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  VenusLighthouseAerie_FaceActor(GARET, FACING_SOUTH);
  VenusLighthouseAerie_ShowMessage(GARET);
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
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  VenusLighthouseAerie_FaceActor(GARET, FACING_SOUTHEAST);
  Engine_ActorJump(GARET, 4, 0);
  VenusLighthouseAerie_ShowMessage(GARET);
  VenusLighthouseAerie_FaceActor(GARET, FACING_NORTHEAST);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
    VenusLighthouseAerie_ShowMessage(GARET);
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    VenusLighthouseAerie_ShowMessage(IVAN);
    Engine_ActorSetAnimation(MIA, ANIMATION_SHAKE_HEAD);
    VenusLighthouseAerie_ShowMessage(MIA);
    stepPending = 1;
  } else {
    gEventWork->message += 3;
    Engine_EventWait(60);
    VenusLighthouseAerie_ShowMessage(GARET);
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    VenusLighthouseAerie_ShowMessage(IVAN);
    Engine_ActorSetAnimation(MIA, ANIMATION_SHAKE_HEAD);
    VenusLighthouseAerie_ShowMessage(MIA);
  }
  if (stepPending != 0) {
    gEventWork->message += 3;
  }
  VenusLighthouseAerie_FaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(80);
  Event_PlayCue(AUDIO_CUE_FADE_BGM);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 20);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 40);
  Actor_FaceDirection(GARET, FACING_WEST, 20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  VenusLighthouseAerie_ShowMessage(IVAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 20);
  Engine_ActorSetAnimation(GARET, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(GARET);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 40);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 40);
  Actor_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  VenusLighthouseAerie_ShowMessage(MIA);
  Event_PlayCue(SE_141);
  Map_SetShake(PIXELS(6), PIXELS(6), Q16_ONE);
  Engine_EventWait(40);
  Event_PlayCue(SE_145);
  Map_CopyCellsTo(110, 105, 74, 4, 18, 23);
  Map_CopyCellsTo(92, 86, 83, 4, 8, 23);
  Map_CopyCellsTo(75, 28, 75, 4, 8, 23);
  Map_CopyCellsTo(92, 86, 11, 72, 16, 20);
  Map_CopyCellsTo(19, 92, 19, 68, 8, 21);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->z.fixed -= PIXELS(32);
  actor->work = 0;
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(GARET);
  actor->x.fixed -= PIXELS(4);
  actor->z.fixed -= PIXELS(32);
  actor->work = 0;
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(IVAN);
  actor->x.fixed -= PIXELS(4);
  actor->z.fixed -= PIXELS(32);
  actor->work = 0;
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(MIA);
  actor->x.fixed -= PIXELS(4);
  actor->z.fixed -= PIXELS(18);
  actor->work = 0;
  VenusLighthouseAerie_StopActorMotion(actor);
  Object_GetById(ACTOR_PARTICLE_SOURCE)->y.fixed = PIXELS(56);
  Camera_MoveTo(PIXELS(338), PIXELS(32), PIXELS(180), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
  Engine_ActorJump(GARET, 6, 0);
  Engine_ActorJump(IVAN, 6, 0);
  Engine_ActorJump(MIA, 6, 0);
  // Shake the party and split away the remaining central floor tiles.
  Actor_SetActionScript(ACTOR_PARTY_LEADER,
                        gVenusLighthouseAerie_PartyHeadingAction);
  Actor_SetActionScript(GARET, gVenusLighthouseAerie_PartyHeadingAction);
  Actor_SetActionScript(IVAN, gVenusLighthouseAerie_PartyHeadingAction);
  Actor_SetActionScript(MIA, gVenusLighthouseAerie_PartyHeadingAction);
  Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Engine_EventWait(80);
  Map_SetShake(PIXELS(6), PIXELS(6), Q16_ONE);
  Engine_EventWait(40);
  Event_PlayCue(SE_145);
  Map_CopyCellAttributes(110, 106, 18, 14, 10, 5);
  Map_CopyCellsTo(110, 105, 74, 4, 18, 23);
  Map_CopyCellsTo(92, 86, 11, 68, 16, 20);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(GARET);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(IVAN);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(MIA);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(ACTOR_BEACON_STATUE_SOUTHWEST);
  actor->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(ACTOR_BEACON_STATUE_NORTHWEST);
  actor->x.fixed += PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(ACTOR_BEACON_STATUE_NORTHEAST);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  actor = Object_GetById(ACTOR_BEACON_STATUE_SOUTHEAST);
  actor->x.fixed -= PIXELS(16);
  VenusLighthouseAerie_StopActorMotion(actor);
  Camera_MoveTo(PIXELS(322), PIXELS(32), PIXELS(180), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
  Engine_ActorJump(GARET, 6, 0);
  Engine_ActorJump(IVAN, 6, 0);
  Engine_ActorJump(MIA, 6, 0);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_EventWait(80);
  Event_PlayCue(AUDIO_CUE_FADE_SLOT3);
  Engine_EventWait(40);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(IVAN);
  Engine_ActorStop(MIA);
  Engine_EventWait(120);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  VenusLighthouseAerie_ShowMessage(IVAN);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  VenusLighthouseAerie_ShowMessage(GARET);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
  Engine_ActorSetAnimation(MIA, ANIMATION_SHAKE_HEAD);
  VenusLighthouseAerie_ShowMessage(MIA);
  Engine_ActorJump(IVAN, 2, 20);
  VenusLighthouseAerie_ShowMessage(IVAN);
  Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_EventWait(20);
  VenusLighthouseAerie_FaceActor(GARET, FACING_EAST);
  VenusLighthouseAerie_ShowMessage(GARET);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_EventWait(20);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_SetActionScript(GARET, gVenusLighthouseAerie_PartyExitAction);
  Actor_SetActionScript(IVAN, gVenusLighthouseAerie_PartyExitAction);
  Actor_SetActionScript(MIA, gVenusLighthouseAerie_PartyExitAction);
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
  ObjectSystem_SuspendProjectionAndShowSprites();
  ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->speakerVoiceSound = 0;
  ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->speakerVoiceCountdown =
      0;
  UiText_ShowCenteredMessage(MsgVenusDespiteLongTiring, 0, 0);
  ObjectSystem_ResumeProjectionAndHideLayers();
  Engine_EventWait(80);
}

/// Clears all movement targets and velocities and resets the actor timer.
static void VenusLighthouseAerie_StopActorMotion(struct FieldActor *actor) {
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;

  SET_VELOCITY(actor, 0, 0, 0);

  actor->timer = 0;
}

/// Advances the beacon rise sequence, its flashes and orbiting particles.
static void VenusLighthouseAerie_UpdateBeaconRise(void) {
  struct FieldActor *center;
  struct FieldActor *effect;
  struct AnimationObject *sprite;
  s32 spawnParticles;
  u32 randomHeight;
  s32 angle;

  center = Actor_Get(ACTOR_PARTICLE_SOURCE);
  spawnParticles = 0;
  switch (sTransitionStep) {
  case 0:
    Event_PlayCue(SE_220);
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
    VenusLighthouseAerie_StopActorMotion(center);
    Actor_SetActionScript(ACTOR_PARTICLE_SOURCE,
                          gVenusLighthouseAerie_RiseActionScript);
    break;
  case 25:
    sTransitionStep--;
    if (center->y.fixed > 0) {
      ColorBuffer_ApplyTarget(0x203210, 0);
      Engine_ColorBufferInterpolate(16);
      sTransitionStep++;
      Actor_Get(ACTOR_PARTY_LEADER)->riseCounter = 1;
      Actor_Get(GARET)->riseCounter = 1;
      Actor_Get(IVAN)->riseCounter = 1;
      Actor_Get(MIA)->riseCounter = 1;
      Actor_Get(ACTOR_FELIX)->riseCounter = 1;
      Actor_Get(SHEBA)->riseCounter = 1;
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlayCue(SE_246);
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
        Event_PlayCue(SE_246);
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
    Event_PlayCue(SE_187);
    ColorBuffer_ApplyTarget(RGB(31, 31, 31), 0);
    Engine_ColorBufferInterpolate(12);
    break;
  case 48:
    Engine_ActorStop(ACTOR_PARTICLE_SOURCE);
    GameFlag_Set(FLAG_SCENE_TEMP_37);
    break;
  }
  if (spawnParticles) {
    randomHeight = ((u32)(Random_Next() * 80) >> 16) << 16;
    effect = Object_Create(SPRITE_PARTICLE, center->x.fixed,
                           center->y.fixed - randomHeight + -PIXELS(8),
                           center->z.fixed);
    if (effect != 0) {
      sprite = effect->sprite;
      Object_SetScript(effect, gVenusLighthouseAerie_RiseParticleScript);
      ObjectGroup_SetChildValue(effect, SPRITE_COLOR_GRB);
      effect->motionFlags = 0;
      angle = Random_Next() & 0xffff000;
      effect->timer = angle;
      effect->work = 0;
      effect->riseCounter = (u32)Random_Next() >> 13;
      effect->update = VenusLighthouseAerie_UpdateRisingOrbit;
      effect->speed = Engine_MathSin((u32)(Random_Next() * 0xffff) >> 20) * 24;
      effect->speed = center->speed >> 16;
      sprite->flags = 0;
      sprite->image.priority = 1;
    }
  }
  sTransitionStep++;
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(ACTOR_PARTY_LEADER));
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(GARET));
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(IVAN));
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(MIA));
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(ACTOR_FELIX));
  VenusLighthouseAerie_AdvanceActorRise(Actor_Get(SHEBA));
}

/// Raises an enabled actor according to its gated rise counter.
static void VenusLighthouseAerie_AdvanceActorRise(struct FieldActor *actor) {
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

/// Emits particles beside the lower beam and links each to its source actor.
static void VenusLighthouseAerie_SpawnSideOrbitParticles(void) {
  union FieldObject *center;
  struct FieldActor *effect;
  struct AnimationObject *sprite;
  struct MapScrollWork *map;
  s32 drift;
  s32 phase;
  s32 angle;

  center = OBJECT_GET(23);
  map = gMapWork[0];
  drift = (u32)(Random_Next() * 48) >> 16 << 16;
  if ((s16)(map->viewY >> 16) <= 129) {
    if (gFrameCount & 1) {
      Actor_SetPosition(ACTOR_PARTICLE_SOURCE, PIXELS(304), PIXELS(164));
      Actor_Get(ACTOR_PARTICLE_SOURCE)->scaleX = Q16_ONE;
      Actor_Get(ACTOR_PARTICLE_SOURCE)->scaleY = Q16_ONE;
    } else {
      Actor_SetPosition(ACTOR_PARTICLE_SOURCE, PIXELS(304), PIXELS(171));
      Actor_Get(ACTOR_PARTICLE_SOURCE)->scaleX = 13 * Q16_ONE / 10;
      Actor_Get(ACTOR_PARTICLE_SOURCE)->scaleY = 13 * Q16_ONE / 10;
    }
  } else {
    Actor_SetPosition(ACTOR_PARTICLE_SOURCE, 0, 0);
  }
  if (center == 0)
    return;
  phase = gFrameCount & 15;
  if (phase != 0)
    return;
  effect = Object_Create(SPRITE_PARTICLE, center->actor.x.fixed + PIXELS(8),
                         center->actor.y.fixed + drift + PIXELS(8),
                         center->actor.z.fixed);
  drift /= PIXELS(6);
  drift <<= 16;
  if (effect == 0)
    return;
  sprite = effect->sprite;
  Object_SetScript(effect, gVenusLighthouseAerie_BesideParticleScript);
  ObjectGroup_SetChildValue(effect, SPRITE_COLOR_BRG);
  effect->motionFlags = phase;
  angle = Random_Next() & 0xffff000;
  effect->timer = angle;
  effect->work = phase;
  effect->linkedObject = &center->actor;
  effect->update = VenusLighthouseAerie_UpdateOrbitingEffect;
  effect->speed = Engine_MathSin((drift & 0xfffff) >> 4) * 24 >> 16;
  sprite->flags = 0;
  sprite->image.priority = center->actor.sprite->image.priority;
}

/// Moves a linked particle along its horizontal ellipse and preserves its
/// target.
static void
VenusLighthouseAerie_UpdateOrbitingEffect(union FieldObject *object) {
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

s32 gVenusLighthouseAerie_JoinLeaderAction[] = {
    SC_ARG(ANIMATION_WALK),
    SC_MOVE(PIXELS(492), 0, PIXELS(164)),
    SC_SYNC,
    SC_ARG(ANIMATION_STAND),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gVenusLighthouseAerie_StepBackWestAction[] = {
    SC_SET(SW_SPEED, PIXELS(1) / 5),
    SC_SET(SW_ACCEL, PIXELS(1) / 10),
    SC_ARG(ANIMATION_WALK),
    SC_MOVE_BY(-PIXELS(8), 0, 0),
    SC_SYNC,
    SC_END,
};

s32 gVenusLighthouseAerie_StepBackEastAction[] = {
    SC_SET(SW_SPEED, PIXELS(1) / 5),
    SC_SET(SW_ACCEL, PIXELS(1) / 10),
    SC_ARG(ANIMATION_WALK),
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
    SC_POS(0, 0, 0),         SC_WAIT(1), SC_END,
};

s32 gVenusLighthouseAerie_BeamEndAction[] = {
    SC_ADD(SW_Y, PIXELS(2)), SC_WAIT(1), SC_LOOP(160, SC_TOP),
    SC_POS(0, 0, 0),         SC_WAIT(1), SC_END,
};

s32 gVenusLighthouseAerie_OrbitParticleScript[] = {
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

s32 gVenusLighthouseAerie_RiseParticleScript[] = {
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

s32 gVenusLighthouseAerie_WalkOutAction[] = {
    SC_ARG(ANIMATION_WALK),
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
    SC_ARG(ANIMATION_STAND),
    SC_POS(0, 0, 0),
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
    SC_ADD(SW_Y, -(PIXELS(3) / 4)),
    SC_WAIT(1),
    SC_LOOP(120, SC_TOP),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gVenusLighthouseAerie_LeapAction[] = {
    SC_ADD(SW_Y, -(PIXELS(9) / 2)),
    SC_WAIT(1),
    SC_LOOP(80, SC_TOP),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gVenusLighthouseAerie_PartyExitAction[] = {
    SC_ARG(ANIMATION_WALK),
    SC_MOVE(PIXELS(330), 0, PIXELS(173)),
    SC_SYNC,
    SC_FREE,
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
    SCENE_EXIT(VENUS_LIGHTHOUSE_AERIE_TO_LALIVERO_FARANS_HOUSE,
               LaliveroFaransHouse,
               LALIVERO_FARANS_HOUSE_FROM_VENUS_LIGHTHOUSE_AERIE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_AERIE_TO_LALIVERO_SHOPS, LaliveroShops,
               LALIVERO_SHOPS_SANCTUM),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gVenusLighthouseAeriePlacements[] = {
    { SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(440), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(472), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(504), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(104), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(136), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(168), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(504), 0, PIXELS(148), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(504), 0, PIXELS(148), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(504), 0, PIXELS(148), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_MENARDI, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(164), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_SATUROS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(180), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SHEBA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(172), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_FELIX, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(252), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLUE_ORB, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};
// clang-format on

s32 gVenusLighthouseAerie_LandedPlatform = 0;

struct SceneEvent gVenusLighthouseAerieEvents[] = {
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, TRIGGER_FLOOR_BY_PLATFORMS,
     CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpToNextPlatform)},
    {EVENT_TOUCH, TRIGGER_FLOOR_BY_PLATFORMS, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloatPlatforms)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, TRIGGER_PLATFORM, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_JumpToNextPlatform)},
    {EVENT_TOUCH, TRIGGER_PLATFORM, FLAG_SCENE_TEMP_50,
     EVENT_SCRIPT(Event_LandOnPlatform)},
    {EVENT_TOUCH, TRIGGER_BEFORE_BEACON, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ConfrontSaturosAndMenardi)},
    {SCENE_EVENTS_END},
};

s32 gVenusLighthouseAerie_BesideParticleScript[] = {
    SC_SET(SW_SCALE_Y, 7 * Q16_ONE / 5),
    SC_SET(SW_SCALE_X, 7 * Q16_ONE / 5),
    SC_ARG(4),
    SC_WAIT(30),
    SC_FREE,
};
