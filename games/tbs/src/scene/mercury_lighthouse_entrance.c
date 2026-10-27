/// @file
/// Mercury Lighthouse (SceneId_MercuryLighthouseEntrance), the lighthouse's
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
///   steps into the leader's way too (metatile behavior 41, and back to the
///   middle on 40 and 42); talking to him, Mind Read on him or walking north
///   into him starts the battle (the second slot of the encounter zone of Tret
///   Tree's and Mercury Lighthouse's scripted battles). Back from it
///   (MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN), Mia nods to
///   the leader and goes on into the lighthouse
///   (FLAG_MERCURY_LIGHTHOUSE_ENTERED_BY_MIA); metatile behavior 10 by the
///   doorway brings the party back to
///   MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN, so this plays
///   even if the party wandered off. Six statues line the way to the doorway.
/// - The pool. An inscription (metatile behavior 1) reads `He who honors the
///   heart of the goddess shall stand at the center of all, like a swan in the
///   center of a rippling pond.` Ply cast on the invisible actor there
///   from one cell only, (12, 32), dims the room, raises sparks from the water,
///   starts the colours shimmering and opens the routes that need it
///   (FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED): it copies the metatile behaviors
///   of 13 by 7 cells over the pool at (6, 32), and entering the goddess's room
///   once it is set copies them again. Cells out in the water take
///   MB_MERCURY_LIGHTHOUSE_POOL_WATER, the heart
///   MB_MERCURY_LIGHTHOUSE_POOL_HEART and cells on the pool's rim
///   MB_MERCURY_LIGHTHOUSE_POOL_RIM, which the hops and lights below answer,
///   while the water between them stays a wall. From then on
///   the pool's heart
///   ( MB_MERCURY_LIGHTHOUSE_POOL_HEART) gives the leader up to three lights,
///   each hop into the water spends one, and landing in it without one sinks
///   the leader back to the heart (the helpers of
///   common/mercury_lighthouse_pillar.h). Dropping in from above
///   (MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE) the first
///   time, the leader lands in a burst, and until Ply has worked the
///   inscription's words appear.
/// - The way on. Three sparkling passages (metatile behaviors 67 to 69) carry
///   the leader between this floor's parts
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
///   SceneId_MercuryLighthouseLower, and
///   MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_EXTERIOR back out
///   through the door to the ground before it, Mercury Lighthouse's
///   SceneId_MercuryLighthouseExterior (mercury_lighthouse.c). Two chests hold
///   Psynergy Armor and a mimic.
///
/// Every entry sets FLAG_MERCURY_LIGHTHOUSE_LIT, under which Mercury Adepts
/// regain PP faster, and makes SceneId_MercuryLighthouseExterior's
/// MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP the Retreat point. The shimmering
/// colours (MercuryLighthouse_CyclePalette) run once Ply has worked, and always
/// in the goddess's room (MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_SIX_STATUES
/// and the two from the twin pools, and
/// MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE); elsewhere
/// they are black until then. The lights the leader carries come back from
/// their heap slot (common/mercury_lighthouse_lights.h) on a return after a
/// battle. MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_FROM_CHEST_FOUNTAIN,
/// MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTHEAST and
/// MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST
/// each have two rows, chosen by whether the channels upstairs run
/// (FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN).

#include "script_operands.h"
#include "animspr.h"
#include "save_block.h"
#include "heap_slots.h"
#include "heap.h"
#include "keystate.h"
#include "unit/import.h"
#include "unit/scene/mercury_lighthouse_entrance.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/scene/common/mercury_lighthouse_lights.h"
#include "unit/scene/common/mercury_lighthouse_pillar.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/field/field.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
#include "unit/runtime/runtime.h"
#include "iowrite.h"
#include "macros.h"

#define NULL ((void *)0)
#include "actor.h"
#include "mem.h"
#include "types.h"
#include "mercury.h"
#include "scene_tables.h"
#include "pushed_actor.h"
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
#include "scene_ids.h"
#include "field_effect.h"
#include "io_reg.h"
#include "event_layer.h"
#include "text/msg_ids.h"
#include "iwram_call.h"
#include "ability_ids.h"
#include "actor_script.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

/// This floor's actors, numbering its actor template rows from
/// ACTOR_FIRST_PLACED. Mia stands in her own party slot; the Lizard Man is
/// the monster she cannot get past (his scripted battle fights him as "Lizard
/// Man"); the Ply target is the invisible actor Ply is cast on.
enum MercuryLighthouseEntranceActorId {
  /// ★ Named from the game: "Lizard Man" (MsgEnemyName)
  ACTOR_LIZARD_MAN = ACTOR_FIRST_PLACED,
  /// Six statues line the way to the doorway: three on its west side (x 88),
  /// then three on its east (x 184).
  /// ★ Named from the game: "checked the stone statue..." (MsgCheckedGround)
  ACTOR_FIRST_STATUE,
  /// The east side's three statues, which the doorway's events draw mirrored.
  /// ★ Named from the game: "checked the stone statue..." (MsgCheckedGround)
  ACTOR_FIRST_EAST_STATUE = ACTOR_FIRST_STATUE + 3,
  /// ★ Named from the game: "used Ply." (MsgMercuryDidThat)
  ACTOR_PLY_TARGET = ACTOR_FIRST_PLACED + 8
};

/// Scene flags, kept while the party stays here.
enum {
  /// ★ Named from the game: "Mia" (MsgCharacterName)
  FLAG_MIA_GOES_IN_SEEN = FLAG_SCENE_TEMP_50,
  /// ★ Named from its area name: Mercury Lighthouse
  FLAG_DROP_FROM_ABOVE_SEEN = FLAG_SCENE_TEMP_51,
  /// The Lizard Man has stepped into the leader's column.
  /// ★ Named from the game: "Lizard Man" (MsgEnemyName)
  FLAG_LIZARD_MAN_STEPPED_ASIDE = FLAG_SCENE_TEMP_05
};

// Forward declarations.
static void MercuryLighthouseEntrance_LizardManBlocksMia(void);
static void MercuryLighthouseEntrance_MiaEntersLighthouse(void);

#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "common/staged_actor.h"
#include "common/mercury_lighthouse_sparks.h"

/// The sound of the Lizard Man's sidestep, played by all three step scripts
/// as he lands.
static s32 MercuryLighthouseEntrance_PlayStepSound(struct Actor *interpreter) {
  Sound_PlaySong(SE_FOOTSTEP);
  return 0;
}

// Mia's update while she waits by the Lizard Man: she turns to watch the
// leader while the leader is north of 184 pixels, and looks back north at
// the doorway otherwise.
// UB: the field updater invokes a void update routine. Plain void removes the
// zero result and changes the native return register (three instructions in
// EN).
#ifdef UBFIX
static void MercuryLighthouseEntrance_MiaWatchesLeader(struct Actor *updated) {
#else
static s32 MercuryLighthouseEntrance_MiaWatchesLeader(struct Actor *updated) {
#endif
  struct Actor *actor = (struct Actor *)updated;
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

  if ((leader->z.fixed >> 19) <= 22) {
    actor->facing = ArcTan2(leader->z.fixed - actor->z.fixed,
                            leader->x.fixed - actor->x.fixed);
  } else if (actor->facing != FACING_NORTH) {
    Actor_ArmCallback(ACTOR_MIA, FACING_NORTH, 0);
  }
#ifndef UBFIX
  return 0;
#endif
}

#include "common/mercury_lighthouse_palette_cycle.h"

/// Mercury Lighthouse, the pool, once Ply has worked: touching
/// MB_MERCURY_LIGHTHOUSE_POOL_WATER keeps blue ripples spreading from the
/// leader while there.
static void MercuryLighthouseEntrance_RippleAroundLeader(void) {
  MercuryLighthouse_RippleAroundLeader();
}

/// Mercury Lighthouse, the pool, once Ply has worked: touching
/// MB_MERCURY_LIGHTHOUSE_POOL_RIM takes the leader's lights.
static void MercuryLighthouseEntrance_LoseLights(void) {
  MercuryLighthouse_LoseLights();
}

/// Mercury Lighthouse, the pool, once Ply has worked: walking into
/// MB_MERCURY_LIGHTHOUSE_POOL_HEART, MB_MERCURY_LIGHTHOUSE_POOL_WATER or
/// MB_MERCURY_LIGHTHOUSE_POOL_RIM hops the leader two cells the pressed way,
/// spending a light on a hop into the water.
static void MercuryLighthouseEntrance_HopAcrossPool(void) {
  MercuryLighthouse_HopAcrossPool();
}

/// Mercury Lighthouse, the pool, once Ply has worked: touching its heart
/// ( MB_MERCURY_LIGHTHOUSE_POOL_HEART) gives the leader lights, up to three.
static void MercuryLighthouseEntrance_ChargeLights(void) {
  MercuryLighthouse_ChargeLights();
}

const struct SceneEntrance *MercuryLighthouseEntrance_GetEntrances(void) {
  return gMercuryLighthouseEntrance_Entrances;
}

const struct SceneExitRegion *MercuryLighthouseEntrance_GetRegions(void) {
  return NULL;
}

const u32 *MercuryLighthouseEntrance_GetExits(void) {
  return gMercuryLighthouseEntrance_Exits;
}

/// Returns Mia, the Lizard Man, the six statues and the Ply target.
const struct ActorTemplate *MercuryLighthouseEntrance_GetActorTemplates(void) {
  return gMercuryLighthouseEntrance_ActorTemplates;
}

/// Mercury Lighthouse: searching the inscription by the pool
/// (metatile behavior 1). "He who honors the heart of the goddess shall stand
/// at the center of all..."
static void MercuryLighthouseEntrance_ReadHeartInscription(void) {
  Event_Start();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldMessage_Show(MsgMercuryWhoHonorsHeart, 1);
  Event_End();
}

#include "common/mercury_lighthouse_passage.h"

/// Mercury Lighthouse: walking north into metatile behavior 67, a sparkling
/// passage, leaves by
/// MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_FOUNTAIN_TO_DRY_FOUNTAIN.
static void MercuryLighthouseEntrance_LeaveChestPoolByPassage(void) {
  MercuryLighthouse_LeaveThroughPassage(
      MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_FOUNTAIN_TO_DRY_FOUNTAIN);
}

/// Mercury Lighthouse: walking north into metatile behavior 68, a sparkling
/// passage, leaves by
/// MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_TWIN_FOUNTAINS.
static void MercuryLighthouseEntrance_LeaveEastWaterfallsByPassage(void) {
  MercuryLighthouse_LeaveThroughPassage(
      MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_TWIN_FOUNTAINS);
}

/// Mercury Lighthouse: walking north into metatile behavior 69, a sparkling
/// passage, leaves by
/// MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_TO_CHEST_FOUNTAIN.
static void MercuryLighthouseEntrance_LeaveDryPoolByPassage(void) {
  MercuryLighthouse_LeaveThroughPassage(
      MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_TO_CHEST_FOUNTAIN);
}

/// Returns the floor's coord, bg, talk and Psynergy triggers.
const struct Trigger *MercuryLighthouseEntrance_GetTriggers(void) {
  return gMercuryLighthouseEntrance_Triggers;
}

/// Mercury Lighthouse, the pool: Ply on the Ply target, from cell (12, 32)
/// only, as the inscription bids. The room dims, fifteen sparks rise from the
/// pool, its colours start to shimmer, and the routes that need it open, so
/// the pool's metatile behaviors come into play. Until
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

  leaderColumn = Actor_GetById(ACTOR_PARTY_LEADER)->x.fixed / PIXELS(16);
  leaderRow = Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed / PIXELS(16);
  if (leaderColumn == 12 && leaderRow == 32) {
    Event_Start();
    // Fade the event before releasing fifteen sparks above the pool.
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_StartInterpolation(60);
    Event_WaitFrames(120);
    PaletteFade_ApplyTarget(PALETTE_TINT_PALE, PALETTES_BG);
    PaletteFade_StartInterpolation(60);
    Event_WaitFrames(40);
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
      Sound_PlaySong(SE_GLITTER);
      sparkX = 208 - ((u32)(Random_Next() << 4) >> 16);
      sparkZ = 560 - ((u32)(Random_Next() << 4) >> 16);
      speedRoll = ((u32)(Import_RandomNext() << 2) >> 16);
      velocityY =
          Math_Divide((((speedRoll << 4) - speedRoll) << 16) + 0x3c0000, 100);
      FieldEffect_Spawn(sparkX << 16, 0, sparkZ << 16, 0, velocityY, velocityZ,
                        EFFECT_USE_PRIORITY | EFFECT_USE_TYPE |
                            EFFECT_USE_SCRIPT | EFFECT_SCRIPT_MEDIUM,
                        effectData);
      Event_WaitFrames(4);
      sparkIndex++;
    } while ((u32)sparkIndex <= 14);
    Sound_PlaySong(SE_SHIMMER);
    Event_WaitFrames(60);
    // Persist the activated state and replace the pool's map cells.
    GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED);
    Task_AddFrame(MercuryLighthouse_CyclePalette, TASK_PASS_FRAME);
    MapGrid_CopyCellsTo(37, 98, 10, 97, 5, 3);
    MapGrid_CopyCellAttributes(70, 32, 13, 7, 6, 32);
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_StartInterpolation(60);
    Event_WaitFrames(120);
    Event_End();
  }
}

/// Mercury Lighthouse, the doorway: touching metatile behavior 10 starts the
/// scene again at MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN,
/// where Mia goes in if she has not yet.
static void MercuryLighthouseEntrance_ReturnToDoorway(void) {
  Import_EventSetExitRequest(
      MERCURY_LIGHTHOUSE_ENTRANCE_TO_SIX_STATUES_AFTER_LIZARD_MAN);
}

/// Mercury Lighthouse, the doorway: touching metatile behavior 40 or 42 while
/// the Lizard Man stands aside: he steps back in front of the doorway.
static void MercuryLighthouseEntrance_LizardManReturnsToCentre(void) {
  GameFlag_Clear(FLAG_LIZARD_MAN_STEPPED_ASIDE);
  Actor_SetScript(ACTOR_LIZARD_MAN,
                  gMercuryLighthouseEntrance_LizardManStepCentre);
}

/// Mercury Lighthouse, the doorway: touching metatile behavior 41 while the
/// Lizard Man stands in the doorway: he steps into the leader's way, west from
/// column 7 and east otherwise.
static void MercuryLighthouseEntrance_LizardManBlocksLeader(void) {
  struct Actor *leader;
  s32 leaderX;
  s32 leaderColumn;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  leaderX = leader->x.fixed;
  leaderColumn = leaderX / PIXELS(16);
  GameFlag_Set(FLAG_LIZARD_MAN_STEPPED_ASIDE);
  if (leaderColumn == 7) {
    Actor_SetScript(ACTOR_LIZARD_MAN,
                    gMercuryLighthouseEntrance_LizardManStepWest);
  } else {
    Actor_SetScript(ACTOR_LIZARD_MAN,
                    gMercuryLighthouseEntrance_LizardManStepEast);
  }
}

/// Mercury Lighthouse, the doorway: talking to the Lizard Man, Mind Read on
/// him, or walking north into metatile behavior 40 or 41. He stirs and the
/// battle begins; afterwards the party comes back at
/// MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN.
static void MercuryLighthouseEntrance_FightLizardMan(void) {
  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_LIZARD_MAN, 2);
  Event_WaitFrames(20);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Event_SetAfterBattleDestination(
      (s32)&SceneId_MercuryLighthouseEntrance,
      MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  // The second slot of the encounter zone of Tret Tree's and Mercury
  // Lighthouse's scripted battles: the Lizard Man.
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY,
                                        1);
  Event_End();
}

#include "common/mercury_lighthouse_arrive.h"

/// Sets up Mercury Lighthouse's ground floor: the lights the leader carries,
/// FLAG_MERCURY_LIGHTHOUSE_LIT, the Retreat point, the pool's colours and the
/// routes Ply has opened, then the event of the entrance taken: the
/// Lizard Man event until Mia has gone in, the drop from above, the passages'
/// arrivals and the rise from the channels.
s32 MercuryLighthouseEntrance_Initialize(void) {
  struct EffectOptions options;
  s32 velocity[3];
  struct Actor *actor;
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
      MB_MERCURY_LIGHTHOUSE_POOL_WATER,
      (struct MercuryLighthouseLights *)gSceneLocalState);
  GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_LIT);
  gSaveBlock.retreatEntrance = MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP;
  gSaveBlock.retreatScene = (s32)&SceneId_MercuryLighthouseExterior;
  FieldEffect_StartFadeOverlay(0);
  if (GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED))
    Task_AddFrame(MercuryLighthouse_CyclePalette, TASK_PASS_FRAME);
  else
    MercuryLighthouse_ClearPalette();
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  // Each entrance restores its route or plays its one-time arrival.
  switch (gSaveBlock.entrance) {
  case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR:
    // Until Mia has gone in, arriving here plays the Lizard Man event.
    if (!GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_ENTERED_BY_MIA))
      Import_EventSetExitRequest(
          MERCURY_LIGHTHOUSE_ENTRANCE_TO_SIX_STATUES_BEFORE_LIZARD_MAN);
    // Both doorway entrances mirror the east side's statues.
    // Fall through.
  case MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_FROM_GODDESS:
    Actor_Get(ACTOR_FIRST_EAST_STATUE)->scaleX = -Q16_ONE;
    Actor_Get(ACTOR_FIRST_EAST_STATUE + 1)->scaleX = -Q16_ONE;
    Actor_Get(ACTOR_FIRST_EAST_STATUE + 2)->scaleX = -Q16_ONE;
    WaitFrames(1);
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_WEST_WATERFALLS:
  case MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_EAST:
  case MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_WEST:
  case MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_EAST_WATERFALLS:
  case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST:
  case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST:
    if (GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED)) {
      MapGrid_CopyCellAttributes(84, 5, 10, 7, 20, 5);
      MapGrid_CopyCellAttributes(101, 5, 12, 7, 37, 5);
    }
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_SIX_STATUES:
  case MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_WEST:
  case MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_EAST:
  case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE:
    Task_AddFrame(MercuryLighthouse_CyclePalette, TASK_PASS_FRAME);
    if (GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED)) {
      MapGrid_CopyCellsTo(37, 98, 10, 97, 5, 3);
      MapGrid_CenterOnFollowedPoint();
      WaitFrames(1);
      MapGrid_CopyCellAttributes(70, 32, 13, 7, 6, 32);
    }
    if (gSaveBlock.entrance !=
        MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE)
      break;
    arrivalSeen = Event_GetFlag(FLAG_DROP_FROM_ABOVE_SEEN);
    if (arrivalSeen != 0)
      break;
    // The first time, the leader drops from 130 pixels up and lands in a
    // burst; until Ply has been used, the inscription's words appear.
    GameFlag_Set(FLAG_DROP_FROM_ABOVE_SEEN);
    Event_Start();
    FieldCamera_MoveTo(-1, -1, -1, 0);
    MapGrid_CenterOnFollowedPoint();
    WaitFrames(1);
    Actor_Get(ACTOR_PARTY_LEADER)->y.fixed = PIXELS(130);
    Actor_Get(ACTOR_PARTY_LEADER)->gravity = 0x8000;
    Actor_Get(ACTOR_PARTY_LEADER)->bounce = arrivalSeen;
    Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = arrivalSeen;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_WaitFrames(30);
    Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 3;
    Sound_PlaySong(SE_DUST_PUFF);
    Event_WaitFrames(24);
    // Scatter seventeen effects around the leader before the landing.
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    options.palette = 7;
    for (sparkIndex = 0; sparkIndex <= 16; sparkIndex++) {
      angle = sparkIndex << 12;
      SET_VECTOR(velocity, Trig_Cos(angle), 0, Trig_Sin(angle));
      velocity[0] -= velocity[0] / 4;
      velocity[2] -= velocity[2] / 2;
      FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
                        velocity[0], velocity[1], velocity[2],
                        EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM, &options);
    }
    Sound_PlaySong(SE_THUMP);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
    MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    MapGrid_WaitForShakeToSettle();
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
    Actor_Get(ACTOR_PARTY_LEADER)->gravity = 0x10000;
    Actor_Get(ACTOR_PARTY_LEADER)->bounce = 0x4000;
    if (!GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED)) {
      PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
      PaletteFade_StartInterpolation(30);
      Event_WaitTransitionFrames();
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
      Event_WaitFrames(30);
      FieldMessage_ShowCentered(MsgMercuryWhoHonorsHeart, 0, 0);
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
      PaletteFade_StartInterpolation(30);
    }
    Event_End();
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_EAST_WATERFALLS:
  case MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_TWIN_FOUNTAINS:
  case MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_WEST_WATERFALLS:
    MercuryLighthouse_ArriveWithSparks();
    // All four arrivals play the same sound.
    // Fall through.
  case MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_TWIN_FOUNTAINS:
    Scene_QueueMusic(SE_AMBIENT_WATER);
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_PIPE:
    Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Event_Start();
    MapGrid_CenterOnFollowedPoint();
    WaitFrames(1);
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_WaitFrames(120);
    Import_EventSetExitRequest(
        MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_PIPE);
    Event_End();
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_BEFORE_LIZARD_MAN:
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
      MercuryLighthouseEntrance_LizardManBlocksMia();
    else
      MapGrid_CopyCellAttributes(0, 0, 3, 3, 7, 9);
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN:
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
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
  struct Actor *mia;
  struct Actor *actor;

  Event_Start();
  actor = Actor_GetById(ACTOR_FIRST_EAST_STATUE);
  actor->scaleX = -Q16_ONE;
  actor = Actor_GetById(ACTOR_FIRST_EAST_STATUE + 1);
  actor->scaleX = -Q16_ONE;
  actor = Actor_GetById(ACTOR_FIRST_EAST_STATUE + 2);
  actor->scaleX = -Q16_ONE;
  Actor_SetPosition(ACTOR_MIA, PIXELS(136), PIXELS(184));
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(136), PIXELS(296));
  Actor_SetPosition(ACTOR_LIZARD_MAN, PIXELS(136), PIXELS(152));
  Actor_SetSpeed(ACTOR_MIA, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_SetSpeed(ACTOR_LIZARD_MAN, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  FieldCamera_SetSpeedParameters(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveWithinMapBounds(PIXELS(136), -1, PIXELS(184), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Actor_GetById(ACTOR_LIZARD_MAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Event_WaitFrames(20);
  // Twice she tries the east side and then the west.
  for (paceCycle = 0; paceCycle < 2; paceCycle++) {
    Actor_ResetAndSetPositionInMode2(ACTOR_MIA, 152, 168);
    Event_WaitFrames(10);
    Actor_SetScript(ACTOR_LIZARD_MAN,
                    gMercuryLighthouseEntrance_LizardManStepEast);
    Actor_WaitForMoveById(ACTOR_MIA);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 30);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    mia = Actor_GetById(ACTOR_MIA);
    mia->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_ResetAndSetPositionInMode2(ACTOR_MIA, 136, 184);
    Event_WaitFrames(10);
    Actor_SetScript(ACTOR_LIZARD_MAN,
                    gMercuryLighthouseEntrance_LizardManStepCentre);
    Actor_WaitForMoveById(ACTOR_MIA);
    Actor_GetById(ACTOR_MIA)->steeringFlags |= ACTOR_FACES_MOTION;
    Event_WaitFrames(30);
    Actor_ResetAndSetPositionInMode2(ACTOR_MIA, 120, 168);
    Event_WaitFrames(5);
    Actor_SetScript(ACTOR_LIZARD_MAN,
                    gMercuryLighthouseEntrance_LizardManStepWest);
    Actor_WaitForMoveById(ACTOR_MIA);
    Actor_ArmCallback(ACTOR_MIA, FACING_NORTH, 30);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_WaitFrames(15);
    Actor_GetById(ACTOR_MIA)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_ResetAndSetPositionInMode2(ACTOR_MIA, 136, 184);
    Event_WaitFrames(15);
    Actor_SetScript(ACTOR_LIZARD_MAN,
                    gMercuryLighthouseEntrance_LizardManStepCentre);
    Actor_WaitForMoveById(ACTOR_MIA);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Actor_GetById(ACTOR_MIA)->steeringFlags |= ACTOR_FACES_MOTION;
  }
  Event_WaitFrames(20);
  FieldEffect_SpawnAttached(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  actor = Actor_GetById(ACTOR_MIA);
#ifdef UBFIX
  actor->update = MercuryLighthouseEntrance_MiaWatchesLeader;
#else
  actor->update =
      (void (*)(struct Actor *))MercuryLighthouseEntrance_MiaWatchesLeader;
#endif
  // The leader walks up behind her; the cells before the doorway become
  // walkable.
  Actor_PlaceCurrentWithinCameraBounds(ACTOR_PARTY_LEADER, 1);
  Event_WaitFrames(30);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 136, 264);
  FieldCamera_WaitForArrival();
  MapGrid_CopyCellAttributes(0, 0, 3, 3, 7, 9);
  Event_End();
}

/// Mercury Lighthouse, the doorway, at
/// MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN, back from the
/// Lizard Man battle, once: Mia nods to the leader and goes on into the
/// lighthouse. Sets FLAG_MERCURY_LIGHTHOUSE_ENTERED_BY_MIA.
static void MercuryLighthouseEntrance_MiaEntersLighthouse(void) {
  struct Actor *actor;

  if (Event_GetFlag(FLAG_MIA_GOES_IN_SEEN) == 0) {
    GameFlag_Set(FLAG_MIA_GOES_IN_SEEN);
    Event_Start();
    actor = Actor_Get(ACTOR_FIRST_EAST_STATUE);
    actor->scaleX = -Q16_ONE;
    actor = Actor_GetById(ACTOR_FIRST_EAST_STATUE + 1);
    actor->scaleX = -Q16_ONE;
    actor = Actor_Get(ACTOR_FIRST_EAST_STATUE + 2);
    actor->scaleX = -Q16_ONE;
    Actor_SetPosition(ACTOR_MIA, PIXELS(136), PIXELS(144));
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 10);
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_WaitFrames(60);
    Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_WaitFrames(30);
    Actor_WalkTo(ACTOR_MIA, 136, 72);
    Event_WaitFrames(40);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_WaitForMoveById(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_ENTERED_BY_MIA);
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    Event_End();
  }
}

#include "common/mercury_lighthouse_lights.h"

/// Spreads one blue ripple (SPRITE_RIPPLE) from an actor, for 30 frames.
static void MercuryLighthouse_SpawnRipple(struct Actor *source) {
  struct Actor *effect = FieldActor_Create(SPRITE_RIPPLE, source->x.fixed,
                                           source->y.fixed, source->z.fixed);

  if (effect != 0) {
    struct Sprite *sprite = (struct Sprite *)effect->sprite;

    Actor_StartScript(effect, gMercuryLighthouseEntrance_RippleScript);
    effect->motionFlags = 0;
    effect->layer = 1;
    effect->priorityFlags = 2;
    if (sprite != 0) {
      Sprite_StartAnim(sprite, 2);
      sprite->flags = 0;
      sprite->image.priority = 3;
    }
  }
}

#include "common/mercury_lighthouse_pillar.h"

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

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gPushedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gMercuryLighthouseEntrance_RippleScript[] = {
    SC_WAIT(30),
    SC_RELEASE,
};

s32 gMercuryLighthouse_SparkScript[] = {
    SC_CALL(MercuryLighthouse_AdvanceAnchoredRiseFrame),
    SC_RELEASE,
};

s32 gMercuryLighthouse_RampScript[] = {
    SC_CALL(MercuryLighthouse_GrowLight),
    SC_END,
};

s32 gMercuryLighthouse_ScaleCounterScript[] = {
    SC_CALL(Actor_AdvanceScaleCounter),
    SC_RELEASE,
};

s32 gMercuryLighthouse_PillarScript[] = {
    SC_CALL(MercuryLighthouse_FollowLeader),
    SC_END,
};

u16 gMercuryLighthouse_MoveAngles[] = {
    0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, 0xffff, 0xffff, FACING_NORTH,
    FACING_SOUTH, 0xffff, 0xffff, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
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

struct SceneEntrance gMercuryLighthouseEntrance_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
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
    // XXX (no evidence): what the next two entrances were for; no exit
    // or code leads to them.
    { MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_UNUSED_NORTH, CONDITION_ALWAYS, 136, 0, 152, FACING_SOUTH, 0, 16, 32, 256, 320, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_UNUSED_MIDDLE, CONDITION_ALWAYS, 136, 0, 232, FACING_EAST, 0, 16, 32, 256, 320, 0 },
    { SCENE_TABLE_END },
};

u32 gMercuryLighthouseEntrance_Exits[] = {
    SCENE_EXITS(MercuryLighthouseEntrance),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_EXTERIOR, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_ENTRANCE),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_TO_GODDESS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_SIX_STATUES),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_SIX_STATUES, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_FROM_GODDESS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_FOUNTAINS_WEST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_WEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_FOUNTAINS_EAST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_EAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_WEST_WATERFALLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_TWIN_FOUNTAINS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_GODDESS_WEST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_WEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_GODDESS_EAST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_EAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_EAST_WATERFALLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_TWIN_FOUNTAINS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTHWEST, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_SOUTHWEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_WEST_WATERFALLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_EAST_WATERFALLS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_EAST_WATERFALLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_WEST_WATERFALLS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_TWIN_FOUNTAINS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_WEST_WATERFALLS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_FOUNTAIN_TO_DRY_FOUNTAIN, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_FROM_CHEST_FOUNTAIN),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_TWIN_FOUNTAINS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_EAST_WATERFALLS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_TO_CHEST_FOUNTAIN, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_FOUNTAIN_FROM_DRY_FOUNTAIN),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_SIX_STATUES_BEFORE_LIZARD_MAN, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_BEFORE_LIZARD_MAN),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_PIPE, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_SIX_STATUES_AFTER_LIZARD_MAN, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN),
    SCENE_EXITS_END,
};

struct ActorTemplate gMercuryLighthouseEntrance_ActorTemplates[] = {
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LIZARD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(88), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(88), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(88), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(184), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(184), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(184), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(616), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(200), 0, PIXELS(512), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gMercuryLighthouseEntrance_Triggers[] = {
    { STEP_TRIGGER_WALKING, MB_MERCURY_LIGHTHOUSE_POOL_RIM, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT(MercuryLighthouseEntrance_HopAcrossPool) },
    { TRIGGER_STEP, MB_MERCURY_LIGHTHOUSE_POOL_RIM, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT(MercuryLighthouseEntrance_LoseLights) },
    { STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK, MB_MERCURY_LIGHTHOUSE_POOL_WATER, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT(MercuryLighthouseEntrance_HopAcrossPool) },
    { TRIGGER_STEP, MB_MERCURY_LIGHTHOUSE_POOL_WATER, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT(MercuryLighthouseEntrance_RippleAroundLeader) },
    { STEP_TRIGGER_WALKING, MB_MERCURY_LIGHTHOUSE_POOL_HEART, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT(MercuryLighthouseEntrance_HopAcrossPool) },
    { TRIGGER_STEP, MB_MERCURY_LIGHTHOUSE_POOL_HEART, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT(MercuryLighthouseEntrance_ChargeLights) },
    { TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(MercuryLighthouseEntrance_ReturnToDoorway) },
    { TRIGGER_STEP, 40, IF_FLAG_SET(FLAG_LIZARD_MAN_STEPPED_ASIDE), EVENT(MercuryLighthouseEntrance_LizardManReturnsToCentre) },
    { TRIGGER_STEP, 42, IF_FLAG_SET(FLAG_LIZARD_MAN_STEPPED_ASIDE), EVENT(MercuryLighthouseEntrance_LizardManReturnsToCentre) },
    { TRIGGER_STEP, 41, FLAG_LIZARD_MAN_STEPPED_ASIDE, EVENT(MercuryLighthouseEntrance_LizardManBlocksLeader) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 40, CONDITION_ALWAYS, EVENT(MercuryLighthouseEntrance_FightLizardMan) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 41, CONDITION_ALWAYS, EVENT(MercuryLighthouseEntrance_FightLizardMan) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 67, CONDITION_ALWAYS, EVENT(MercuryLighthouseEntrance_LeaveChestPoolByPassage) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 68, CONDITION_ALWAYS, EVENT(MercuryLighthouseEntrance_LeaveEastWaterfallsByPassage) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 69, CONDITION_ALWAYS, EVENT(MercuryLighthouseEntrance_LeaveDryPoolByPassage) },
    { TRIGGER_EXIT, 51, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_EXTERIOR },
    { TRIGGER_EXIT, 52, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_TO_GODDESS },
    { TRIGGER_EXIT, 53, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_SIX_STATUES },
    { TRIGGER_EXIT, 54, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_FOUNTAINS_WEST },
    { TRIGGER_EXIT, 55, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_FOUNTAINS_EAST },
    { TRIGGER_EXIT, 56, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_WEST_WATERFALLS },
    { TRIGGER_EXIT, 57, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_GODDESS_WEST },
    { TRIGGER_EXIT, 58, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_GODDESS_EAST },
    { TRIGGER_EXIT, 59, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_EAST_WATERFALLS },
    { TRIGGER_EXIT, 60, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST },
    { TRIGGER_EXIT, 61, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 62, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTHWEST },
    { TRIGGER_EXIT, 63, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST },
    { TRIGGER_EXIT, 64, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_WEST_WATERFALLS },
    { TRIGGER_EXIT, 65, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_EAST_WATERFALLS },
    { TRIGGER_EXIT, 66, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_TWIN_FOUNTAINS },
    { TRIGGER_TALK, ACTOR_MIA, CONDITION_ALWAYS, MSGNO(MsgMercuryMonsterInsideTower) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIA, CONDITION_ALWAYS, MSGNO(MsgMercuryLetMePass) },
    { TRIGGER_TALK, ACTOR_LIZARD_MAN, CONDITION_ALWAYS, EVENT(MercuryLighthouseEntrance_FightLizardMan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LIZARD_MAN, CONDITION_ALWAYS, EVENT(MercuryLighthouseEntrance_FightLizardMan) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 1, CONDITION_ALWAYS, EVENT(MercuryLighthouseEntrance_ReadHeartInscription) },
    { PSYNERGY_ON_ACTOR(ABILITY_PLY), ACTOR_PLY_TARGET, FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED, EVENT(MercuryLighthouseEntrance_ActivateWithPly) },
    { PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT(MercuryLighthouse_RemoveColumnOnRetreat) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_MERCURY_LIGHTHOUSE_PSYNERGY_ARMOR, FIND_ITEM(ITEM_PSYNERGY_ARMOR) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_MERCURY_LIGHTHOUSE_MIMIC_BEATEN, FIND_MIMIC },
    { TRIGGERS_END },
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
    SC_RELEASE,
};
// clang-format on
