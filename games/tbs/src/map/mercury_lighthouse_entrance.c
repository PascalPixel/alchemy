/// @file
/// Mercury Lighthouse (MapId_MercuryLighthouseEntrance), the lighthouse's
/// ground floor inside its door, which leads up to its floors
/// (mercury_lighthouse.c). Mia, Imil's healer and the last of the Mercury Clan
/// who guard the lighthouse, has sensed someone inside, where none but she and
/// Alex, her apprentice who disappeared, can enter; she has hurried in ahead of
/// the party to find out who.
///
/// - The doorway. Until Mia has gone on, coming in by the door
///   (MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR) starts the
///   scene again at MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_BEFORE_LIZARD_MAN,
///   where she tries again and again to get past a monster, the Lizard Man, who
///   steps across to block her, then waits, turning to watch the leader come up
///   (MercuryLighthouseEntrance_LizardManBlocksMia). Spoken to, she says `A
///   monster inside Mercury Lighthouse? How could this be!?` The Lizard Man
///   steps into the leader's way too (cell code 41, and back to the middle on
///   40 and 42); talking to him, Mind Read on him or walking north into him
///   starts the battle (the second slot of the encounter zone of Tret Tree's
///   and Mercury Lighthouse's scripted battles). Back from it
///   (MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN), Mia nods to
///   the leader and goes on into the lighthouse
///   (FLAG_MERCURY_LIGHTHOUSE_ENTERED_BY_MIA); cell code 10 by the doorway
///   brings the party back to
///   MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN, so this plays
///   even if the party wandered off. Six statues line the way to the doorway.
/// - The pool. An inscription (cell code 1) reads `He who honors the heart of
///   the goddess shall stand at the center of all, like a swan in the center of
///   a rippling pond.` Ply cast on the invisible actor there from one cell
///   only, (12, 32), dims the room, raises sparks from the water, starts the
///   colours shimmering and opens the routes that need it
///   (FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED): it copies the collision of 13
///   by 7 cells over the pool at (6, 32), and entering the goddess's room
///   once it is set copies them again. Cells out in the water take code 21,
///   the heart 11 and cells on the pool's rim 31, the codes the hops and
///   lights below answer, while the water between them stays a wall
///   (https://alchemy.pascalpixel.com/places/mercury-lighthouse-entrance/pool-ply-cells.png:
///   the pool's cells tinted by what each does, a wall, a hop, or the lights
///   given or taken, entered by the goddess's room before and after Ply, the
///   cells Ply changed outlined). From then on
///   the pool's heart
///   (cell code 11) gives the leader up to three lights, each hop into the
///   water spends one, and landing in it without one sinks the leader back to
///   the heart (the helpers of common/mercury_lighthouse_pillar.h). Dropping in
///   from above
///   (MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE) the first
///   time, the leader lands in a burst, and until Ply has worked the
///   inscription's words appear.
/// - The way on. Three sparkling passages (cell codes 67 to 69) carry the
///   leader between this floor's parts
///   (MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_FOUNTAIN_TO_DRY_FOUNTAIN,
///   MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_TWIN_FOUNTAINS and
///   MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_TO_CHEST_FOUNTAIN); four doors
///   (MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST,
///   MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST,
///   MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTHWEST and
///   MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST)
///   and the rise from the channels
///   (MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_PIPE, then
///   MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_PIPE) lead up to
///   MapId_MercuryLighthouseLower, and
///   MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_EXTERIOR back out
///   through the door to the ground before it, Mercury Lighthouse's
///   MapId_MercuryLighthouseExterior (mercury_lighthouse.c). Two chests hold
///   Psynergy Armor and a mimic.
///
/// Every entry sets FLAG_MERCURY_LIGHTHOUSE_LIT, under which Mercury Adepts
/// regain PP faster, and makes MapId_MercuryLighthouseExterior's
/// MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP the Retreat point. The shimmering
/// colours (MercuryLighthouse_CyclePalette) run once Ply has worked, and always
/// in the goddess's room (MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_SIX_STATUES
/// and the two from the twin pools, and
/// MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE); elsewhere
/// they are black until then. The lights the leader carries come back from
/// their work slot (common/mercury_lighthouse_lights.h) on a return after a
/// battle. MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_FROM_CHEST_FOUNTAIN,
/// MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTHEAST and
/// MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST
/// each have two rows, chosen by whether the channels upstairs run
/// (FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN).
///
/// Open questions:
/// - XXX (no evidence): what the two unused six-statue entrances
///   (MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_UNUSED_NORTH and _MIDDLE) were
///   for; no exit or code leads to them, so nothing in the ROM shows them in
///   use.

#include "action_script.h"
#include "animspr.h"
#include "game_state.h"
#include "global_cells.h"
#include "heap_state.h"
#include "keystate.h"
#include "unit/import.h"
#include "unit/map/mercury_lighthouse_entrance.h"
#include "unit/map/common/effect_spawn.h"
#include "unit/map/common/mercury_lighthouse_lights.h"
#include "unit/map/common/mercury_lighthouse_pillar.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/runtime/runtime.h"
#include "iowrite.h"
#include "macros.h"

#define NULL ((void *)0)
#include "object_event.h"
#include "mem.h"
#include "types.h"
#include "mercury.h"
#include "map_tables.h"
#include "staged_object_event.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "map_grid_copy.h"
#include "call.h"
#include "engine_call.h"
#include "map_ids.h"
#include "field_effect.h"
#include "io_reg.h"
#include "script_layer.h"
#include "text/msg_ids.h"
#include "iwram_call.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

/// This floor's actors, numbering its placement rows from LOCALID_FIRST_PLACED.
/// Mia stands in her own party slot; the Lizard Man is the monster she cannot
/// get past (his scripted battle fights him as "Lizard Man"); the Ply target is
/// the invisible actor Ply is cast on.
enum MercuryLighthouseEntranceLocalId {
  /// ★ Named from the game: "Lizard Man" (MsgEnemyName)
  LOCALID_LIZARD_MAN = LOCALID_FIRST_PLACED,
  /// Six statues line the way to the doorway: three on its west side (x 88),
  /// then three on its east (x 184).
  /// ★ Named from the game: "checked the stone statue..." (MsgCheckedGround)
  LOCALID_FIRST_STATUE,
  /// The east side's three statues, which the doorway's scenes draw mirrored.
  /// ★ Named from the game: "checked the stone statue..." (MsgCheckedGround)
  LOCALID_FIRST_EAST_STATUE = LOCALID_FIRST_STATUE + 3,
  /// ★ Named from the game: "used Ply." (MsgMercuryDidThat)
  LOCALID_PLY_TARGET = LOCALID_FIRST_PLACED + 8
};

/// Scene flags, kept while the party stays here.
enum {
  /// ★ Named from the game: "Mia" (MsgCharacterName)
  FLAG_MIA_GOES_IN_SEEN = FLAG_MAP_TEMP_50,
  /// ★ Named from its area name: Mercury Lighthouse
  FLAG_DROP_FROM_ABOVE_SEEN = FLAG_MAP_TEMP_51,
  /// The Lizard Man has stepped into the leader's column.
  /// ★ Named from the game: "Lizard Man" (MsgEnemyName)
  FLAG_LIZARD_MAN_STEPPED_ASIDE = FLAG_MAP_TEMP_05
};

// Forward declarations.
static void MercuryLighthouseEntrance_LizardManBlocksMia(void);
static void MercuryLighthouseEntrance_MiaEntersLighthouse(void);

#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "common/object_staged_object_event.h"
#include "common/mercury_lighthouse_sparks.h"

/// The sound of the Lizard Man's sidestep, played by all three step scripts
/// as he lands.
static s32
MercuryLighthouseEntrance_PlayStepSound(struct ScriptInterpreter *interpreter) {
  Engine_AudioPlayCue(SE_118);
  return 0;
}

// Mia's update while she waits by the Lizard Man: she turns to watch the
// leader while the leader is north of 184 pixels, and looks back north at
// the doorway otherwise.
// UB: the field updater invokes a void hook. Plain void removes the zero
// result and changes the native return register (three instructions in EN).
#ifdef UBFIX
static void
MercuryLighthouseEntrance_MiaWatchesLeader(union FieldObject *fieldObject) {
#else
static s32
MercuryLighthouseEntrance_MiaWatchesLeader(union FieldObject *fieldObject) {
#endif
  struct ObjectEvent *object = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  if ((leader->z.fixed >> 19) <= 22) {
    object->facing = ArcTan2(leader->z.fixed - object->z.fixed,
                             leader->x.fixed - object->x.fixed);
  } else if (object->facing != FACING_NORTH) {
    Engine_ObjectEventFaceDirection(MIA, FACING_NORTH, 0);
  }
#ifndef UBFIX
  return 0;
#endif
}

#include "common/mercury_lighthouse_palette_cycle.h"

/// Mercury Lighthouse, the pool, once Ply has worked: touching cell code 21
/// keeps blue ripples spreading from the leader while there.
static void MercuryLighthouseEntrance_RippleAroundLeader(void) {
  MercuryLighthouse_RippleAroundLeader();
}

/// Mercury Lighthouse, the pool, once Ply has worked: touching cell code 31
/// takes the leader's lights.
static void MercuryLighthouseEntrance_LoseLights(void) {
  MercuryLighthouse_LoseLights();
}

/// Mercury Lighthouse, the pool, once Ply has worked: walking into cell code
/// 11, 21 or 31 hops the leader two cells the pressed way, spending a light on
/// a hop into the water.
static void MercuryLighthouseEntrance_HopAcrossPool(void) {
  MercuryLighthouse_HopAcrossPool();
}

/// Mercury Lighthouse, the pool, once Ply has worked: touching its heart (cell
/// code 11) gives the leader lights, up to three.
static void MercuryLighthouseEntrance_ChargeLights(void) {
  MercuryLighthouse_ChargeLights();
}

const struct MapWarpDest *MercuryLighthouseEntrance_GetWarpDests(void) {
  return gMercuryLighthouseEntrance_WarpDests;
}

const struct MapWarpRegion *MercuryLighthouseEntrance_GetRegions(void) {
  return NULL;
}

const u32 *MercuryLighthouseEntrance_GetWarps(void) {
  return gMercuryLighthouseEntrance_Warps;
}

/// Returns Mia, the Lizard Man, the six statues and the Ply target.
const struct ObjectEventTemplate *
MercuryLighthouseEntrance_GetObjectEventTemplates(void) {
  return gMercuryLighthouseEntrance_ObjectEventTemplates;
}

/// Mercury Lighthouse: searching the inscription by the pool (cell code 1).
/// "He who honors the heart of the goddess shall stand at the center of
/// all..."
static void MercuryLighthouseEntrance_ReadHeartInscription(void) {
  Engine_ScriptStart();
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_MessageShowCentered(MsgMercuryWhoHonorsHeart, 1);
  Engine_ScriptEnd();
}

#include "common/mercury_lighthouse_passage.h"

/// Mercury Lighthouse: walking north into cell code 67, a sparkling passage,
/// leaves by MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_FOUNTAIN_TO_DRY_FOUNTAIN.
static void MercuryLighthouseEntrance_LeaveChestPoolByPassage(void) {
  MercuryLighthouse_LeaveThroughPassage(
      MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_FOUNTAIN_TO_DRY_FOUNTAIN);
}

/// Mercury Lighthouse: walking north into cell code 68, a sparkling passage,
/// leaves by MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_TWIN_FOUNTAINS.
static void MercuryLighthouseEntrance_LeaveEastWaterfallsByPassage(void) {
  MercuryLighthouse_LeaveThroughPassage(
      MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_TWIN_FOUNTAINS);
}

/// Mercury Lighthouse: walking north into cell code 69, a sparkling passage,
/// leaves by MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_TO_CHEST_FOUNTAIN.
static void MercuryLighthouseEntrance_LeaveDryPoolByPassage(void) {
  MercuryLighthouse_LeaveThroughPassage(
      MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_TO_CHEST_FOUNTAIN);
}

/// Returns the floor's touch, search, talk and Psynergy events.
const struct MapEvent *MercuryLighthouseEntrance_GetEvents(void) {
  return gMercuryLighthouseEntrance_Events;
}

/// Mercury Lighthouse, the pool: Ply on the Ply target, from cell (12, 32)
/// only, as the inscription bids. The room dims, fifteen sparks rise from the
/// pool, its colours start to shimmer, and the routes that need it open, so
/// the pool's cell codes come into play. Until
/// FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED is set, which it sets.
static void MercuryLighthouseEntrance_ActivateWithPly(void) {
  s32 leaderColumn;
  s32 leaderRow;
  s32 velocityZ;
  s32 sparkIndex;
  s32 sparkX;
  s32 sparkZ;
  s32 speedRoll;
  s32 velocityY;
  struct EffectOptions *effectData;
  struct EffectOptions options;

  leaderColumn =
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.fixed / PIXELS(16);
  leaderRow = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed / PIXELS(16);
  if (leaderColumn == 12 && leaderRow == 32) {
    Engine_ScriptStart();
    // Fade the scene before releasing fifteen sparks above the pool.
    ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_ScriptWait(120);
    ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PALE, 1);
    Engine_ColorBufferInterpolate(60);
    Engine_ScriptWait(40);
    sparkIndex = 0;
    effectData = &options;
    velocityZ = 0;
    do {
      effectData->priority = 1;
      {
        u16 effectType = SPRITE_GLINT;

        effectData->type = effectType;
      }
      effectData->script = gMercuryLighthouseEntrance_SparkBurstScript;
      Engine_AudioPlayCue(SE_246);
      sparkX = 208 - ((u32)(Random_Next() << 4) >> 16);
      sparkZ = 560 - ((u32)(Random_Next() << 4) >> 16);
      speedRoll = ((u32)(Engine_RandomNext() << 2) >> 16);
      velocityY =
          Math_Divide((((speedRoll << 4) - speedRoll) << 16) + 0x3c0000, 100);
      FieldEffect_Spawn(sparkX << 16, 0, sparkZ << 16, 0, velocityY, velocityZ,
                        EFFECT_USE_PRIORITY | EFFECT_USE_TYPE |
                            EFFECT_USE_SCRIPT | 1,
                        effectData);
      Engine_ScriptWait(4);
      sparkIndex++;
    } while ((u32)sparkIndex <= 14);
    Engine_AudioPlayCue(SE_220);
    Engine_ScriptWait(60);
    // Persist the activated state and replace the pool's map cells.
    GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED);
    FieldScheduler_AddCallback(MercuryLighthouse_CyclePalette,
                               TASK_PRIORITY_MAP);
    MapGrid_CopyCellsTo(37, 98, 10, 97, 5, 3);
    MapGrid_CopyCellAttributes(70, 32, 13, 7, 6, 32);
    ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_ScriptWait(120);
    Engine_ScriptEnd();
  }
}

/// Mercury Lighthouse, the doorway: touching cell code 10 starts the scene
/// again at MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN, where Mia
/// goes in if she has not yet.
static void MercuryLighthouseEntrance_ReturnToDoorway(void) {
  Engine_ScriptRequestWarp(
      MERCURY_LIGHTHOUSE_ENTRANCE_TO_SIX_STATUES_AFTER_LIZARD_MAN);
}

/// Mercury Lighthouse, the doorway: touching cell code 40 or 42 while the
/// Lizard Man stands aside: he steps back in front of the doorway.
static void MercuryLighthouseEntrance_LizardManReturnsToCentre(void) {
  Engine_GameFlagClear(FLAG_LIZARD_MAN_STEPPED_ASIDE);
  ObjectEvent_SetActionScript(LOCALID_LIZARD_MAN,
                              gMercuryLighthouseEntrance_LizardManStepCentre);
}

/// Mercury Lighthouse, the doorway: touching cell code 41 while the Lizard
/// Man stands in the doorway: he steps into the leader's way, west from column
/// 7 and east otherwise.
static void MercuryLighthouseEntrance_LizardManBlocksLeader(void) {
  struct ObjectEvent *leader;
  s32 leaderX;
  s32 leaderColumn;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  leaderX = leader->x.fixed;
  leaderColumn = leaderX / PIXELS(16);
  Engine_GameFlagSet(FLAG_LIZARD_MAN_STEPPED_ASIDE);
  if (leaderColumn == 7) {
    ObjectEvent_SetActionScript(LOCALID_LIZARD_MAN,
                                gMercuryLighthouseEntrance_LizardManStepWest);
  } else {
    ObjectEvent_SetActionScript(LOCALID_LIZARD_MAN,
                                gMercuryLighthouseEntrance_LizardManStepEast);
  }
}

/// Mercury Lighthouse, the doorway: talking to the Lizard Man, Mind Read on
/// him, or walking north into cell code 40 or 41. He stirs and the battle
/// begins; afterwards the party comes back at
/// MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN.
static void MercuryLighthouseEntrance_FightLizardMan(void) {
  Engine_ScriptStart();
  Engine_ObjectEventRunRepeatedMotion(LOCALID_LIZARD_MAN, 2);
  Engine_ScriptWait(20);
  gEventWork->startTransition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  FieldScript_SetNextDestination(
      (s32)&MapId_MercuryLighthouseEntrance,
      MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN);
  Battle_SetStart(BATTLE_START_SCRIPTED);
  // The second slot of the encounter zone of Tret Tree's and Mercury
  // Lighthouse's scripted battles: the Lizard Man.
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY,
                                        1);
  Engine_ScriptEnd();
}

#include "common/mercury_lighthouse_arrive.h"

/// Sets up Mercury Lighthouse's ground floor: the lights the leader carries,
/// FLAG_MERCURY_LIGHTHOUSE_LIT, the Retreat point, the pool's colours and the
/// routes Ply has opened, then the scene of the entrance taken: the Lizard Man
/// scene until Mia has gone in, the drop from above, the passages' arrivals
/// and the rise from the channels.
s32 MercuryLighthouseEntrance_Initialize(void) {
  struct EffectOptions options;
  s32 velocity[3];
  struct ObjectEvent *objectEvent;
  u32 sparkIndex;
  s32 angle;
  s32 arrivalSeen;

  Io_Write16(BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND, &REG_BLDCNT);
  {
    s32 alpha = BLEND_ALPHA(16, 16);
    REG_BLDALPHA = alpha;
  }
  // Restore persistent lighting and the Retreat destination first.
  MercuryLighthouse_RestoreLights(
      21, (struct MercuryLighthouseLights *)gMapLocalState);
  GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_LIT);
  gGameState.retreatWarpDest = MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP;
  gGameState.retreatMap = (s32)&MapId_MercuryLighthouseExterior;
  FieldEffect_StartFadeOverlay(0);
  if (GameFlag_IsSet(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED))
    FieldScheduler_AddCallback(MercuryLighthouse_CyclePalette,
                               TASK_PRIORITY_MAP);
  else
    MercuryLighthouse_ClearPalette();
  gEventWork->startTransition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  // Each entrance restores its route or plays its one-time arrival.
  switch (gGameState.warpDest) {
  case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR:
    // Until Mia has gone in, arriving here plays the Lizard Man scene.
    if (!GameFlag_IsSet(FLAG_MERCURY_LIGHTHOUSE_ENTERED_BY_MIA))
      Engine_ScriptRequestWarp(
          MERCURY_LIGHTHOUSE_ENTRANCE_TO_SIX_STATUES_BEFORE_LIZARD_MAN);
    // Both doorway entrances mirror the east side's statues.
    // Fall through.
  case MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_FROM_GODDESS:
    ObjectEvent_Get(LOCALID_FIRST_EAST_STATUE)->scaleX = -Q16_ONE;
    ObjectEvent_Get(LOCALID_FIRST_EAST_STATUE + 1)->scaleX = -Q16_ONE;
    ObjectEvent_Get(LOCALID_FIRST_EAST_STATUE + 2)->scaleX = -Q16_ONE;
    Engine_TaskWait(1);
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_WEST_WATERFALLS:
  case MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_EAST:
  case MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_WEST:
  case MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_EAST_WATERFALLS:
  case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST:
  case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST:
    if (GameFlag_IsSet(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED)) {
      MapGrid_CopyCellAttributes(84, 5, 10, 7, 20, 5);
      MapGrid_CopyCellAttributes(101, 5, 12, 7, 37, 5);
    }
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_SIX_STATUES:
  case MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_WEST:
  case MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_EAST:
  case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE:
    FieldScheduler_AddCallback(MercuryLighthouse_CyclePalette,
                               TASK_PRIORITY_MAP);
    if (GameFlag_IsSet(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED)) {
      MapGrid_CopyCellsTo(37, 98, 10, 97, 5, 3);
      Engine_MapGridRedraw();
      Engine_TaskWait(1);
      MapGrid_CopyCellAttributes(70, 32, 13, 7, 6, 32);
    }
    if (gGameState.warpDest !=
        MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE)
      break;
    arrivalSeen = GameFlag_IsSet(FLAG_DROP_FROM_ABOVE_SEEN);
    if (arrivalSeen != 0)
      break;
    // The first time, the leader drops from 130 pixels up and lands in a
    // burst; until Ply has been used, the inscription's words appear.
    GameFlag_Set(FLAG_DROP_FROM_ABOVE_SEEN);
    Engine_ScriptStart();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_MapGridRedraw();
    Engine_TaskWait(1);
    ObjectEvent_Get(LOCALID_PARTY_LEADER)->y.fixed = PIXELS(130);
    ObjectEvent_Get(LOCALID_PARTY_LEADER)->gravity = 0x8000;
    ObjectEvent_Get(LOCALID_PARTY_LEADER)->bounce = arrivalSeen;
    ObjectEvent_Get(LOCALID_PARTY_LEADER)->motionFlags = arrivalSeen;
    Engine_ScriptOpenScreen();
    Engine_ScriptWaitForScreen();
    Engine_ScriptWait(30);
    ObjectEvent_Get(LOCALID_PARTY_LEADER)->motionFlags = 3;
    Engine_AudioPlayCue(SE_204);
    Engine_ScriptWait(24);
    // Scatter seventeen effects around the leader before the landing.
    objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    options.palette = 7;
    for (sparkIndex = 0; sparkIndex <= 16; sparkIndex++) {
      angle = sparkIndex << 12;
      SET_VECTOR(velocity, Engine_MathCos(angle), 0, Engine_MathSin(angle));
      velocity[0] -= velocity[0] / 4;
      velocity[2] -= velocity[2] / 2;
      FieldEffect_Spawn(objectEvent->x.fixed, objectEvent->y.fixed,
                        objectEvent->z.fixed, velocity[0], velocity[1],
                        velocity[2], EFFECT_USE_PALETTE | 1, &options);
    }
    Engine_AudioPlayCue(SE_188);
    ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER,
                                  ATTACHED_EFFECT_IN_FRONT |
                                      ATTACHED_EFFECT_ATTACH);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                   ANIMATION_LEADER_LEAN_BACK);
    MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    Engine_MapGridRenderWaitForValues();
    ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER,
                                  ATTACHED_EFFECT_IN_FRONT |
                                      ATTACHED_EFFECT_REMOVE);
    ObjectEvent_Get(LOCALID_PARTY_LEADER)->gravity = 0x10000;
    ObjectEvent_Get(LOCALID_PARTY_LEADER)->bounce = 0x4000;
    if (!GameFlag_IsSet(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED)) {
      ColorBuffer_ApplySource(PALETTE_TRANSFORM_PLAIN, 0);
      ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_WARM, 1);
      Engine_ColorBufferInterpolate(30);
      Engine_ScriptWaitForScreen();
      Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
      Engine_ScriptWait(30);
      UiText_ShowCenteredMessage(MsgMercuryWhoHonorsHeart, 0, 0);
      ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
      Engine_ColorBufferInterpolate(30);
    }
    Engine_ScriptEnd();
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_EAST_WATERFALLS:
  case MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_TWIN_FOUNTAINS:
  case MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_WEST_WATERFALLS:
    MercuryLighthouse_ArriveWithSparks();
    // All four arrivals play the same sound.
    // Fall through.
  case MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_TWIN_FOUNTAINS:
    FieldEffect_SetQueuedSoundAndPlay(SE_170);
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_PIPE:
    ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, 15);
    Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(LOCALID_PARTY_LEADER), 0);
    Engine_ScriptStart();
    Engine_MapGridRedraw();
    Engine_TaskWait(1);
    gEventWork->startTransition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_ScriptOpenScreen();
    Engine_ScriptWaitForScreen();
    Engine_ScriptWait(120);
    Engine_ScriptRequestWarp(
        MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_PIPE);
    Engine_ScriptEnd();
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_BEFORE_LIZARD_MAN:
    if (!GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION))
      MercuryLighthouseEntrance_LizardManBlocksMia();
    else
      MapGrid_CopyCellAttributes(0, 0, 3, 3, 7, 9);
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN:
    if (!GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION))
      MercuryLighthouseEntrance_MiaEntersLighthouse();
    break;
  }
  return 0;
}

/// Mercury Lighthouse, the doorway, at
/// MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_BEFORE_LIZARD_MAN unless returning
/// after a battle: Mia tries to get past the Lizard Man, who steps across to
/// block her each time; then she waits, watching the leader come up, and the
/// cells before the doorway become walkable.
static void MercuryLighthouseEntrance_LizardManBlocksMia(void) {
  s32 paceCycle;
  struct ObjectEvent *mia;
  struct ObjectEvent *objectEvent;

  Engine_ScriptStart();
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_EAST_STATUE);
  objectEvent->scaleX = -Q16_ONE;
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_EAST_STATUE + 1);
  objectEvent->scaleX = -Q16_ONE;
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_EAST_STATUE + 2);
  objectEvent->scaleX = -Q16_ONE;
  ObjectEvent_SetPosition(MIA, PIXELS(136), PIXELS(184));
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(136), PIXELS(296));
  ObjectEvent_SetPosition(LOCALID_LIZARD_MAN, PIXELS(136), PIXELS(152));
  ObjectEvent_SetSpeed(MIA, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_SetSpeed(LOCALID_LIZARD_MAN, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Engine_CameraSetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Engine_CameraMoveTo(PIXELS(136), -1, PIXELS(184), 0);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ObjectEventRunRepeatedMotion(MIA, 1);
  ObjectEvent_GetById(LOCALID_LIZARD_MAN)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  Engine_ScriptWait(20);
  // Twice she tries the east side and then the west.
  for (paceCycle = 0; paceCycle < 2; paceCycle++) {
    Engine_ObjectEventWalkTo(MIA, 152, 168);
    Engine_ScriptWait(10);
    ObjectEvent_SetActionScript(LOCALID_LIZARD_MAN,
                                gMercuryLighthouseEntrance_LizardManStepEast);
    Engine_ObjectEventWaitForMove(MIA);
    ObjectEvent_FaceDirection(MIA, FACING_NORTH, 30);
    Engine_ObjectEventRunRepeatedMotion(MIA, 1);
    mia = ObjectEvent_GetById(MIA);
    mia->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
    Engine_ObjectEventWalkTo(MIA, 136, 184);
    Engine_ScriptWait(10);
    ObjectEvent_SetActionScript(LOCALID_LIZARD_MAN,
                                gMercuryLighthouseEntrance_LizardManStepCentre);
    Engine_ObjectEventWaitForMove(MIA);
    ObjectEvent_GetById(MIA)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
    Engine_ScriptWait(30);
    Engine_ObjectEventWalkTo(MIA, 120, 168);
    Engine_ScriptWait(5);
    ObjectEvent_SetActionScript(LOCALID_LIZARD_MAN,
                                gMercuryLighthouseEntrance_LizardManStepWest);
    Engine_ObjectEventWaitForMove(MIA);
    Engine_ObjectEventFaceDirection(MIA, FACING_NORTH, 30);
    Engine_ObjectEventRunRepeatedMotion(MIA, 1);
    Engine_ScriptWait(15);
    ObjectEvent_GetById(MIA)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
    Engine_ObjectEventWalkTo(MIA, 136, 184);
    Engine_ScriptWait(15);
    ObjectEvent_SetActionScript(LOCALID_LIZARD_MAN,
                                gMercuryLighthouseEntrance_LizardManStepCentre);
    Engine_ObjectEventWaitForMove(MIA);
    Engine_ObjectEventRunRepeatedMotion(MIA, 1);
    ObjectEvent_GetById(MIA)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  }
  Engine_ScriptWait(20);
  Engine_ObjectEventShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  objectEvent = ObjectEvent_GetById(MIA);
#ifdef UBFIX
  objectEvent->update = MercuryLighthouseEntrance_MiaWatchesLeader;
#else
  objectEvent->update =
      (void (*)(union FieldObject *))MercuryLighthouseEntrance_MiaWatchesLeader;
#endif
  // The leader walks up behind her; the cells before the doorway become
  // walkable.
  Engine_CameraMoveToObjectEvent(LOCALID_PARTY_LEADER, 1);
  Engine_ScriptWait(30);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 136, 264);
  Engine_CameraWaitForMove();
  MapGrid_CopyCellAttributes(0, 0, 3, 3, 7, 9);
  Engine_ScriptEnd();
}

/// Mercury Lighthouse, the doorway, at
/// MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN, back from the
/// Lizard Man battle, once: Mia nods to the leader and goes on into the
/// lighthouse. Sets FLAG_MERCURY_LIGHTHOUSE_ENTERED_BY_MIA.
static void MercuryLighthouseEntrance_MiaEntersLighthouse(void) {
  struct ObjectEvent *objectEvent;

  if (GameFlag_IsSet(FLAG_MIA_GOES_IN_SEEN) == 0) {
    GameFlag_Set(FLAG_MIA_GOES_IN_SEEN);
    Engine_ScriptStart();
    objectEvent = ObjectEvent_Get(LOCALID_FIRST_EAST_STATUE);
    objectEvent->scaleX = -Q16_ONE;
    objectEvent = ObjectEvent_GetById(LOCALID_FIRST_EAST_STATUE + 1);
    objectEvent->scaleX = -Q16_ONE;
    objectEvent = ObjectEvent_Get(LOCALID_FIRST_EAST_STATUE + 2);
    objectEvent->scaleX = -Q16_ONE;
    ObjectEvent_SetPosition(MIA, PIXELS(136), PIXELS(144));
    ObjectEvent_FaceDirection(MIA, FACING_SOUTH, 10);
    gEventWork->startTransition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
    Engine_ScriptOpenScreen();
    Engine_ScriptWaitForScreen();
    Engine_ScriptWait(60);
    ObjectEvent_FaceObjectEvent(MIA, LOCALID_PARTY_LEADER, 0);
    Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_NOD);
    Engine_ScriptWait(30);
    ObjectEvent_WalkTo(MIA, 136, 72);
    Engine_ScriptWait(40);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 1);
    Engine_ObjectEventWaitForMove(MIA);
    ObjectEvent_SetPosition(MIA, 0, 0);
    GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_ENTERED_BY_MIA);
    gEventWork->startTransition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    Engine_ScriptEnd();
  }
}

#include "common/mercury_lighthouse_lights.h"

/// Spreads one blue ripple (SPRITE_RIPPLE) from an actor, for 30 frames.
static void MercuryLighthouse_SpawnRipple(struct ObjectEvent *source) {
  struct ObjectEvent *effect = Engine_ObjectCreate(
      SPRITE_RIPPLE, source->x.fixed, source->y.fixed, source->z.fixed);

  if (effect != 0) {
    struct AnimationObject *sprite = (struct AnimationObject *)effect->sprite;

    Engine_ObjectSetScript(effect, gMercuryLighthouseEntrance_RippleScript);
    effect->motionFlags = 0;
    effect->terrainId = 1;
    effect->priorityFlags = 2;
    if (sprite != 0) {
      AnimationObject_SelectAnimation(sprite, 2);
      sprite->flags = 0;
      sprite->image.priority = 3;
    }
  }
}

#include "common/mercury_lighthouse_pillar.h"

s32 gEffectScript0[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gStagedObjectEvent_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedObjectEvent_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedObjectEvent_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gMercuryLighthouseEntrance_RippleScript[] = {
    SC_WAIT(30),
    SC_FREE,
};

s32 gMercuryLighthouse_SparkScript[] = {
    SC_CALL(MercuryLighthouse_AdvanceAnchoredRiseFrame),
    SC_FREE,
};

s32 gMercuryLighthouse_RampScript[] = {
    SC_CALL(OverlayObject_RampWords24And28Over16Frames),
    SC_END,
};

s32 gMercuryLighthouse_ScaleCounterScript[] = {
    SC_CALL(OverlayObject_AdvanceScaleCounter),
    SC_FREE,
};

s32 gMercuryLighthouse_PillarScript[] = {
    SC_CALL(MercuryLighthouse_FollowLeader),
    SC_END,
};

u16 gMercuryLighthouse_MoveAngles[] = {
    0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, 0xffff, 0xffff, FACING_NORTH,
    FACING_SOUTH, 0xffff, 0xffff, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gMercuryLighthouseEntrance_LizardManStepCentre[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCELERATION, PIXELS(2)),
    SC_SET(SW_STEERING, 0),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(136), 0, PIXELS(152)),
    SC_CALL(MercuryLighthouseEntrance_PlayStepSound),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gMercuryLighthouseEntrance_LizardManStepWest[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCELERATION, PIXELS(2)),
    SC_SET(SW_STEERING, 0),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(120), 0, PIXELS(152)),
    SC_CALL(MercuryLighthouseEntrance_PlayStepSound),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gMercuryLighthouseEntrance_LizardManStepEast[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCELERATION, PIXELS(2)),
    SC_SET(SW_STEERING, 0),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(152), 0, PIXELS(152)),
    SC_CALL(MercuryLighthouseEntrance_PlayStepSound),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gPaletteCycleRed = 0;

s32 gPaletteCycleGreen = 0;

s32 gPaletteCycleBlue = 0;

struct MapWarpDest gMercuryLighthouseEntrance_WarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR, CONDITION_ALWAYS, 136, 0, 280, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_FROM_GODDESS, CONDITION_ALWAYS, 136, 0, 120, FACING_SOUTH, 0, 16, 32, 256, 320, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_SIX_STATUES, CONDITION_ALWAYS, 200, 0, 648, FACING_NORTH, 0, 80, 432, 320, 688, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_WEST, CONDITION_ALWAYS, 120, 0, 520, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_EAST, CONDITION_ALWAYS, 280, 0, 520, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE, CONDITION_ALWAYS, 200, 0, 552, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_WEST_WATERFALLS, CONDITION_ALWAYS, 360, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_EAST, CONDITION_ALWAYS, 456, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_WEST, CONDITION_ALWAYS, 616, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_EAST_WATERFALLS, CONDITION_ALWAYS, 760, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST, CONDITION_ALWAYS, 344, 0, 88, FACING_SOUTH, 0, 304, 32, 800, 208, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST, CONDITION_ALWAYS, 536, 0, 88, FACING_SOUTH, 0, 304, 32, 800, 208, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_FROM_CHEST_FOUNTAIN, FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN, 744, 0, 680, FACING_NORTH, 0, 544, 512, 784, 720, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTHEAST, FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN, 584, 0, 568, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST, FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN, 744, 0, 568, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_FROM_CHEST_FOUNTAIN, CONDITION_ALWAYS, 584, 0, 424, FACING_NORTH, 0, 384, 256, 624, 448, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTHEAST, CONDITION_ALWAYS, 424, 0, 312, FACING_SOUTH, 0, 384, 256, 624, 448, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST, CONDITION_ALWAYS, 584, 0, 312, FACING_SOUTH, 0, 384, 256, 624, 448, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_FOUNTAIN_FROM_DRY_FOUNTAIN, CONDITION_ALWAYS, 744, 0, 424, FACING_NORTH, 0, 704, 256, 944, 448, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_TWIN_FOUNTAINS, CONDITION_ALWAYS, 344, 0, 872, FACING_SOUTH, 0, 48, 768, 400, 960, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_EAST_WATERFALLS, CONDITION_ALWAYS, 232, 0, 872, FACING_SOUTH, 0, 48, 768, 400, 960, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_TWIN_FOUNTAINS, CONDITION_ALWAYS, 568, 0, 872, FACING_SOUTH, 0, 464, 768, 816, 960, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_WEST_WATERFALLS, CONDITION_ALWAYS, 696, 0, 872, FACING_SOUTH, 0, 464, 768, 816, 960, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_PIPE, CONDITION_ALWAYS, 744, 0, 600, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_BEFORE_LIZARD_MAN, CONDITION_ALWAYS, 136, 0, 296, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN, CONDITION_ALWAYS, 136, 0, 168, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_UNUSED_NORTH, CONDITION_ALWAYS, 136, 0, 152, FACING_SOUTH, 0, 16, 32, 256, 320, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_UNUSED_MIDDLE, CONDITION_ALWAYS, 136, 0, 232, FACING_EAST, 0, 16, 32, 256, 320, 0 },
    { MAP_TABLE_END },
};

u32 gMercuryLighthouseEntrance_Warps[] = {
    MAP_WARPS(MercuryLighthouseEntrance),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_EXTERIOR, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_ENTRANCE),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_TO_GODDESS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_SIX_STATUES),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_SIX_STATUES, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_FROM_GODDESS),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_FOUNTAINS_WEST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_WEST),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_FOUNTAINS_EAST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_EAST),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_WEST_WATERFALLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_TWIN_FOUNTAINS),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_GODDESS_WEST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_WEST),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_GODDESS_EAST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_EAST),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_EAST_WATERFALLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_TWIN_FOUNTAINS),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTHWEST, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_SOUTHWEST),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_WEST_WATERFALLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_EAST_WATERFALLS),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_EAST_WATERFALLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_WEST_WATERFALLS),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_TWIN_FOUNTAINS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_WEST_WATERFALLS),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_FOUNTAIN_TO_DRY_FOUNTAIN, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_FROM_CHEST_FOUNTAIN),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_TWIN_FOUNTAINS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_EAST_WATERFALLS),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_TO_CHEST_FOUNTAIN, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_FOUNTAIN_FROM_DRY_FOUNTAIN),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_TO_SIX_STATUES_BEFORE_LIZARD_MAN, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_BEFORE_LIZARD_MAN),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_PIPE, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE),
    MAP_WARP(MERCURY_LIGHTHOUSE_ENTRANCE_TO_SIX_STATUES_AFTER_LIZARD_MAN, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gMercuryLighthouseEntrance_ObjectEventTemplates[] = {
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LIZARD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(88), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(88), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(88), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(184), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(184), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(184), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(616), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(200), 0, PIXELS(512), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gMercuryLighthouseEntrance_Events[] = {
    { COORD_EVENT_WALKING, 31, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouseEntrance_HopAcrossPool) },
    { MAP_EVENT_COORD, 31, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouseEntrance_LoseLights) },
    { COORD_EVENT_WALKING | COORD_EVENT_FLAG_QUICK, 21, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouseEntrance_HopAcrossPool) },
    { MAP_EVENT_COORD, 21, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouseEntrance_RippleAroundLeader) },
    { COORD_EVENT_WALKING, 11, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouseEntrance_HopAcrossPool) },
    { MAP_EVENT_COORD, 11, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouseEntrance_ChargeLights) },
    { MAP_EVENT_COORD, 10, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_ReturnToDoorway) },
    { MAP_EVENT_COORD, 40, IF_FLAG_SET(FLAG_LIZARD_MAN_STEPPED_ASIDE), EVENT_SCRIPT(MercuryLighthouseEntrance_LizardManReturnsToCentre) },
    { MAP_EVENT_COORD, 42, IF_FLAG_SET(FLAG_LIZARD_MAN_STEPPED_ASIDE), EVENT_SCRIPT(MercuryLighthouseEntrance_LizardManReturnsToCentre) },
    { MAP_EVENT_COORD, 41, FLAG_LIZARD_MAN_STEPPED_ASIDE, EVENT_SCRIPT(MercuryLighthouseEntrance_LizardManBlocksLeader) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 40, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_FightLizardMan) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 41, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_FightLizardMan) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 67, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_LeaveChestPoolByPassage) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 68, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_LeaveEastWaterfallsByPassage) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 69, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_LeaveDryPoolByPassage) },
    { MAP_EVENT_WARP, 51, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_EXTERIOR },
    { MAP_EVENT_WARP, 52, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_TO_GODDESS },
    { MAP_EVENT_WARP, 53, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_SIX_STATUES },
    { MAP_EVENT_WARP, 54, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_FOUNTAINS_WEST },
    { MAP_EVENT_WARP, 55, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_FOUNTAINS_EAST },
    { MAP_EVENT_WARP, 56, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_WEST_WATERFALLS },
    { MAP_EVENT_WARP, 57, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_GODDESS_WEST },
    { MAP_EVENT_WARP, 58, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_GODDESS_EAST },
    { MAP_EVENT_WARP, 59, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_EAST_WATERFALLS },
    { MAP_EVENT_WARP, 60, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST },
    { MAP_EVENT_WARP, 61, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST },
    { MAP_EVENT_WARP | WARP_STYLE_ANIMATED, 62, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTHWEST },
    { MAP_EVENT_WARP, 63, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST },
    { MAP_EVENT_WARP, 64, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_WEST_WATERFALLS },
    { MAP_EVENT_WARP, 65, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_EAST_WATERFALLS },
    { MAP_EVENT_WARP, 66, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_TWIN_FOUNTAINS },
    { MAP_EVENT_TALK, MIA, CONDITION_ALWAYS, MSGNO(MsgMercuryMonsterInsideTower) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), MIA, CONDITION_ALWAYS, MSGNO(MsgMercuryLetMePass) },
    { MAP_EVENT_TALK, LOCALID_LIZARD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_FightLizardMan) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LIZARD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_FightLizardMan) },
    { BG_EVENT(BG_EVENT_UNNAMED), 1, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_ReadHeartInscription) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_PLY), LOCALID_PLY_TARGET, FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED, EVENT_SCRIPT(MercuryLighthouseEntrance_ActivateWithPly) },
    { PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_RemoveColumnOnRetreat) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_MERCURY_LIGHTHOUSE_PSYNERGY_ARMOR, FIND_ITEM(ITEM_PSYNERGY_ARMOR) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_MERCURY_LIGHTHOUSE_MIMIC_BEATEN, FIND_MIMIC },
    { MAP_EVENTS_END },
};

/// Each spark pulses in size four times, then is released.
s32 gMercuryLighthouseEntrance_SparkBurstScript[] = {
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 5 * Q16_ONE / 8),
    SC_SET(SW_SCALE_X, 5 * Q16_ONE / 8),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, Q16_ONE / 4),
    SC_SET(SW_SCALE_X, Q16_ONE / 4),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 5 * Q16_ONE / 8),
    SC_SET(SW_SCALE_X, 5 * Q16_ONE / 8),
    SC_WAIT(6),
    SC_LOOP(4, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};
// clang-format on
