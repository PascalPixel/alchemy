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
///   scene again at MERCURY_LIGHTHOUSE_ENTRANCE_HALL_BEFORE_LIZARD_MAN, where
///   she tries again and again to get past a monster, the Lizard Man, who steps
///   across to block her, then waits, turning to watch the leader come up
///   (MercuryLighthouseEntrance_LizardManBlocksMia). Spoken to, she says `A
///   monster inside Mercury Lighthouse? How could this be!?` The Lizard Man
///   steps into the leader's way too (cell code 41, and back to the middle on
///   40 and 42); talking to him, Mind Read on him or walking north into him
///   starts the battle (the second slot of the encounter zone of Tret Tree's
///   and Mercury Lighthouse's scripted battles). Back from it
///   (MERCURY_LIGHTHOUSE_ENTRANCE_HALL_AFTER_LIZARD_MAN), Mia nods to the
///   leader and goes on into the lighthouse
///   (FLAG_MIA_ENTERED_MERCURY_LIGHTHOUSE); cell code 10 by the doorway brings
///   the party back to MERCURY_LIGHTHOUSE_ENTRANCE_HALL_AFTER_LIZARD_MAN, so
///   this plays even if the party wandered off. Six statues line the way to the
///   doorway.
/// - The pool. An inscription (cell code 1) reads `He who honors the heart of
///   the goddess shall stand at the center of all, like a swan in the center of
///   a rippling pond.` Ply cast on the invisible actor there from one cell
///   only, (12, 32), dims the room, raises sparks from the water, starts the
///   colours shimmering and opens the routes that need it
///   (FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED). From then on the pool's heart
///   (cell code 11) gives the leader up to three lights, each hop into the
///   water spends one, and landing in it without one sinks the leader back to
///   the heart (the helpers of common/mercury_lighthouse_pillar.h). Dropping in
///   from above
///   (MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE) the first
///   time, the leader lands in a burst, and until Ply has worked the
///   inscription's words appear.
/// - The way on. Three sparkling passages (cell codes 67 to 69) carry the
///   leader between this floor's parts
///   (MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_POOL_TO_DRY_POOL,
///   MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_TWIN_POOLS and
///   MERCURY_LIGHTHOUSE_ENTRANCE_DRY_POOL_TO_CHEST_POOL); four doors
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
/// in the goddess's room (MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_HALL and the
/// two from the twin pools, and
/// MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE); elsewhere
/// they are black until then. The lights the leader carries come back from
/// their work slot (common/mercury_lighthouse_lights.h) on a return after a
/// battle. MERCURY_LIGHTHOUSE_ENTRANCE_DRY_POOL_FROM_CHEST_POOL,
/// MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTHEAST and
/// MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST
/// each have two rows, chosen by whether the channels upstairs run
/// (FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN).

#include "script.h"
#include "animspr.h"
#include "game_state.h"
#include "global_cells.h"
#include "heap_state.h"
#include "keystate.h"
#include "unit/import.h"
#include "unit/scene/mercury_lighthouse_entrance.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/scene/common/mercury_lighthouse_lights.h"
#include "unit/scene/common/mercury_lighthouse_pillar.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/runtime/runtime.h"
#include "iowrite.h"
#include "macros.h"

#define NULL ((void *)0)
#include "field_obj.h"
#include "mem.h"
#include "types.h"
#include "mercury.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"
#include "call.h"
#include "engine_call.h"
#include "scene_ids.h"
#include "field_effect.h"
#include "io_reg.h"
#include "field_event.h"
#include "text/msg_ids.h"
#include "iwram_call.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

/// This floor's actors, numbering its placement rows from ACTOR_FIRST_PLACED.
/// Mia stands in her own party slot; the Lizard Man is the monster she cannot
/// get past (his scripted battle fights him as "Lizard Man"); the Ply target is
/// the invisible actor Ply is cast on.
enum MercuryLighthouseEntranceActor {
  /// ★ Named from the game: "Lizard Man" (MsgEnemyName)
  ACTOR_LIZARD_MAN = ACTOR_FIRST_PLACED,
  /// Six statues line the way to the doorway: three on its west side (x 88),
  /// then three on its east (x 184).
  ACTOR_FIRST_STATUE,
  /// The east side's three statues, which the doorway's scenes draw mirrored.
  ACTOR_FIRST_EAST_STATUE = ACTOR_FIRST_STATUE + 3,
  ACTOR_PLY_TARGET = ACTOR_FIRST_PLACED + 8
};

/// Scene flags, kept while the party stays here.
enum {
  FLAG_MIA_GOES_IN_SEEN = FLAG_SCENE_TEMP_50,
  FLAG_DROP_FROM_ABOVE_SEEN = FLAG_SCENE_TEMP_00 + 0x51,
  /// The Lizard Man has stepped into the leader's column.
  /// ★ Named from the game: "Lizard Man" (MsgEnemyName)
  FLAG_LIZARD_MAN_STEPPED_ASIDE = FLAG_SCENE_TEMP_05
};

// Forward declarations.
static void MercuryLighthouseEntrance_LizardManBlocksMia(void);
static void MercuryLighthouseEntrance_MiaEntersLighthouse(void);

#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "common/object_staged_actor.h"
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
  struct FieldActor *object = (struct FieldActor *)fieldObject;
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  if ((leader->z.fixed >> 19) <= 22) {
    object->facing = ArcTan2(leader->z.fixed - object->z.fixed,
                             leader->x.fixed - object->x.fixed);
  } else if (object->facing != FACING_NORTH) {
    Engine_ActorFaceDirection(MIA, FACING_NORTH, 0);
  }
#ifndef UBFIX
  return 0;
#endif
}

#include "common/mercury_lighthouse_palette_cycle.h"

/// Mercury Lighthouse, the pool, once Ply has worked: touching cell code 21
/// keeps sparkles rising from the leader while there.
static void MercuryLighthouseEntrance_SparkleAtLeader(void) {
  MercuryLighthouse_SparkleAtLeader();
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

const struct SceneEntrance *MercuryLighthouseEntrance_GetEntrances(void) {
  return gMercuryLighthouseEntrance_Entrances;
}

const struct SceneRegion *MercuryLighthouseEntrance_GetRegions(void) {
  return NULL;
}

const u32 *MercuryLighthouseEntrance_GetExits(void) {
  return gMercuryLighthouseEntrance_Exits;
}

/// Returns Mia, the Lizard Man, the six statues and the Ply target.
const struct ScenePlacement *MercuryLighthouseEntrance_GetPlacements(void) {
  return gMercuryLighthouseEntrance_Placements;
}

/// Mercury Lighthouse: searching the inscription by the pool (cell code 1).
/// "He who honors the heart of the goddess shall stand at the center of
/// all..."
static void MercuryLighthouseEntrance_ReadHeartInscription(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_MessageShowCentered(MsgMercuryWhoHonorsHeart, 1);
  Engine_EventEnd();
}

#include "common/mercury_lighthouse_risedrop.h"

/// Mercury Lighthouse: walking north into cell code 67, a sparkling passage,
/// leaves by MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_POOL_TO_DRY_POOL.
static void MercuryLighthouseEntrance_LeaveChestPoolByPassage(void) {
  MercuryLighthouse_LeaveThroughPassage(
      MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_POOL_TO_DRY_POOL);
}

/// Mercury Lighthouse: walking north into cell code 68, a sparkling passage,
/// leaves by MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_TWIN_POOLS.
static void MercuryLighthouseEntrance_LeaveEastWaterfallsByPassage(void) {
  MercuryLighthouse_LeaveThroughPassage(
      MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_TWIN_POOLS);
}

/// Mercury Lighthouse: walking north into cell code 69, a sparkling passage,
/// leaves by MERCURY_LIGHTHOUSE_ENTRANCE_DRY_POOL_TO_CHEST_POOL.
static void MercuryLighthouseEntrance_LeaveDryPoolByPassage(void) {
  MercuryLighthouse_LeaveThroughPassage(
      MERCURY_LIGHTHOUSE_ENTRANCE_DRY_POOL_TO_CHEST_POOL);
}

/// Returns the floor's touch, search, talk and Psynergy events.
const struct SceneEvent *MercuryLighthouseEntrance_GetEvents(void) {
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
  struct EffectOptions effectOptions;

  leaderColumn = Object_GetById(ACTOR_PARTY_LEADER)->x.fixed / 0x100000;
  leaderRow = Object_GetById(ACTOR_PARTY_LEADER)->z.fixed / 0x100000;
  if (leaderColumn == 12 && leaderRow == 32) {
    Engine_EventBegin();
    // Fade the scene before releasing fifteen sparks above the pool.
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_EventWait(120);
    ColorBuffer_ApplyTarget(0x10005, 1);
    Engine_ColorBufferInterpolate(60);
    Engine_EventWait(40);
    sparkIndex = 0;
    effectData = &effectOptions;
    velocityZ = 0;
    do {
      effectData->priority = 1;
      {
        u16 effectType = 0x11e;

        effectData->type = effectType;
      }
      effectData->script = gMercuryLighthouseEntrance_SparkBurstScript;
      Engine_AudioPlayCue(SE_246);
      sparkX = 208 - ((u32)(Random_Next() << 4) >> 16);
      sparkZ = 560 - ((u32)(Random_Next() << 4) >> 16);
      speedRoll = ((u32)(Engine_RandomNext() << 2) >> 16);
      velocityY =
          Math_Divide((((speedRoll << 4) - speedRoll) << 16) + 0x3c0000, 100);
      FieldFx_Spawn(sparkX << 16, 0, sparkZ << 16, 0, velocityY, velocityZ,
                    EFFECT_USE_PRIORITY | EFFECT_USE_TYPE | EFFECT_USE_SCRIPT |
                        1,
                    effectData);
      Engine_EventWait(4);
      sparkIndex++;
    } while ((u32)sparkIndex <= 14);
    Engine_AudioPlayCue(SE_220);
    Engine_EventWait(60);
    // Persist the activated state and replace the pool's map cells.
    GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED);
    FieldScheduler_AddCallback(MercuryLighthouse_CyclePalette,
                               TASK_PRIORITY_SCENE);
    Map_CopyCellsTo(37, 98, 10, 97, 5, 3);
    Map_CopyCellAttributes(70, 32, 13, 7, 6, 32);
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_EventWait(120);
    Engine_EventEnd();
  }
}

/// Mercury Lighthouse, the doorway: touching cell code 10 starts the scene
/// again at MERCURY_LIGHTHOUSE_ENTRANCE_HALL_AFTER_LIZARD_MAN, where Mia goes
/// in if she has not yet.
static void MercuryLighthouseEntrance_ReturnToDoorway(void) {
  Engine_EventRequestExit(MERCURY_LIGHTHOUSE_ENTRANCE_TO_HALL_AFTER_LIZARD_MAN);
}

/// Mercury Lighthouse, the doorway: touching cell code 40 or 42 while the
/// Lizard Man stands aside: he steps back in front of the doorway.
static void MercuryLighthouseEntrance_LizardManReturnsToCentre(void) {
  Engine_GameFlagClear(FLAG_LIZARD_MAN_STEPPED_ASIDE);
  Actor_SetActionScript(ACTOR_LIZARD_MAN,
                        gMercuryLighthouseEntrance_LizardManStepCentre);
}

/// Mercury Lighthouse, the doorway: touching cell code 41 while the Lizard
/// Man stands in the doorway: he steps into the leader's way, west from column
/// 7 and east otherwise.
static void MercuryLighthouseEntrance_LizardManBlocksLeader(void) {
  struct FieldActor *leader;
  s32 leaderX;
  s32 leaderColumn;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  leaderX = leader->x.fixed;
  leaderColumn = leaderX / 0x100000;
  Engine_GameFlagSet(FLAG_LIZARD_MAN_STEPPED_ASIDE);
  if (leaderColumn == 7) {
    Actor_SetActionScript(ACTOR_LIZARD_MAN,
                          gMercuryLighthouseEntrance_LizardManStepWest);
  } else {
    Actor_SetActionScript(ACTOR_LIZARD_MAN,
                          gMercuryLighthouseEntrance_LizardManStepEast);
  }
}

/// Mercury Lighthouse, the doorway: talking to the Lizard Man, Mind Read on
/// him, or walking north into cell code 40 or 41. He stirs and the battle
/// begins; afterwards the party comes back at
/// MERCURY_LIGHTHOUSE_ENTRANCE_HALL_AFTER_LIZARD_MAN.
static void MercuryLighthouseEntrance_FightLizardMan(void) {
  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(ACTOR_LIZARD_MAN, 2);
  Engine_EventWait(20);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Event_SetNextDestination((s32)&SceneId_MercuryLighthouseEntrance,
                           MERCURY_LIGHTHOUSE_ENTRANCE_HALL_AFTER_LIZARD_MAN);
  Battle_SetStart(BATTLE_START_SCENE);
  // The second slot of the encounter zone of Tret Tree's and Mercury
  // Lighthouse's scripted battles: the Lizard Man.
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY,
                                        1);
  Engine_EventEnd();
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
  struct FieldActor *actor;
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
      21, (struct MercuryLighthouseLights *)gSceneState);
  GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_LIT);
  gGameState.retreatEntrance = MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP;
  gGameState.retreatScene = (s32)&SceneId_MercuryLighthouseExterior;
  BattleFx_StartFadeOverlay(0);
  if (GameFlag_IsSet(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED))
    FieldScheduler_AddCallback(MercuryLighthouse_CyclePalette,
                               TASK_PRIORITY_SCENE);
  else
    MercuryLighthouse_ClearPalette();
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  // Each entrance restores its route or plays its one-time arrival.
  switch (gGameState.entrance) {
  case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR:
    // Until Mia has gone in, arriving here plays the Lizard Man scene.
    if (!GameFlag_IsSet(FLAG_MIA_ENTERED_MERCURY_LIGHTHOUSE))
      Engine_EventRequestExit(
          MERCURY_LIGHTHOUSE_ENTRANCE_TO_HALL_BEFORE_LIZARD_MAN);
    // Both doorway entrances mirror the east side's statues.
    // Fall through.
  case MERCURY_LIGHTHOUSE_ENTRANCE_HALL_FROM_GODDESS:
    Actor_Get(ACTOR_FIRST_EAST_STATUE)->scaleX = -0x10000;
    Actor_Get(ACTOR_FIRST_EAST_STATUE + 1)->scaleX = -0x10000;
    Actor_Get(ACTOR_FIRST_EAST_STATUE + 2)->scaleX = -0x10000;
    Engine_TaskWait(1);
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_FROM_WEST_WATERFALLS:
  case MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_FROM_GODDESS_EAST:
  case MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_FROM_GODDESS_WEST:
  case MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_FROM_EAST_WATERFALLS:
  case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST:
  case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST:
    if (GameFlag_IsSet(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED)) {
      Map_CopyCellAttributes(84, 5, 10, 7, 20, 5);
      Map_CopyCellAttributes(101, 5, 12, 7, 37, 5);
    }
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_HALL:
  case MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_POOLS_WEST:
  case MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_POOLS_EAST:
  case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE:
    FieldScheduler_AddCallback(MercuryLighthouse_CyclePalette,
                               TASK_PRIORITY_SCENE);
    if (GameFlag_IsSet(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED)) {
      Map_CopyCellsTo(37, 98, 10, 97, 5, 3);
      Engine_MapRedraw();
      Engine_TaskWait(1);
      Map_CopyCellAttributes(70, 32, 13, 7, 6, 32);
    }
    if (gGameState.entrance !=
        MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE)
      break;
    arrivalSeen = GameFlag_IsSet(FLAG_DROP_FROM_ABOVE_SEEN);
    if (arrivalSeen != 0)
      break;
    // The first time, the leader drops from 130 pixels up and lands in a
    // burst; until Ply has been used, the inscription's words appear.
    GameFlag_Set(FLAG_DROP_FROM_ABOVE_SEEN);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Actor_Get(ACTOR_PARTY_LEADER)->y.fixed = PIXELS(130);
    Actor_Get(ACTOR_PARTY_LEADER)->gravity = 0x8000;
    Actor_Get(ACTOR_PARTY_LEADER)->bounce = arrivalSeen;
    Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = arrivalSeen;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(30);
    Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 3;
    Engine_AudioPlayCue(SE_204);
    Engine_EventWait(24);
    // Scatter seventeen effects around the leader before the landing.
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    options.palette = 7;
    for (sparkIndex = 0; sparkIndex <= 16; sparkIndex++) {
      angle = sparkIndex << 12;
      SET_VECTOR(velocity, Engine_MathCos(angle), 0, Engine_MathSin(angle));
      velocity[0] -= velocity[0] / 4;
      velocity[2] -= velocity[2] / 2;
      FieldFx_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, velocity[0],
                    velocity[1], velocity[2], EFFECT_USE_PALETTE | 1, &options);
    }
    Engine_AudioPlayCue(SE_188);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 22);
    Map_SetShake(0x50000, 0x50000, 0x10000);
    Map_SetShake(-1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
    Actor_Get(ACTOR_PARTY_LEADER)->gravity = 0x10000;
    Actor_Get(ACTOR_PARTY_LEADER)->bounce = 0x4000;
    if (!GameFlag_IsSet(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED)) {
      ColorBuffer_ApplySource(0x10000, 0);
      ColorBuffer_ApplyTarget(0x10003, 1);
      Engine_ColorBufferInterpolate(30);
      Engine_EventWaitForScreen();
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
      Engine_EventWait(30);
      UiText_ShowCenteredMessage(MsgMercuryWhoHonorsHeart, 0, 0);
      ColorBuffer_ApplyTarget(0x10000, 0);
      Engine_ColorBufferInterpolate(30);
    }
    Engine_EventEnd();
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_EAST_WATERFALLS:
  case MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_TWIN_POOLS:
  case MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_WEST_WATERFALLS:
    MercuryLighthouse_ArriveWithSparks();
    // All four arrivals play the same sound.
    // Fall through.
  case MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_TWIN_POOLS:
    BattleFx_SetQueuedSoundAndPlay(SE_170);
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_PIPE:
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Engine_EventBegin();
    Engine_MapRedraw();
    Engine_TaskWait(1);
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(120);
    Engine_EventRequestExit(
        MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_PIPE);
    Engine_EventEnd();
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_HALL_BEFORE_LIZARD_MAN:
    if (!GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION))
      MercuryLighthouseEntrance_LizardManBlocksMia();
    else
      Map_CopyCellAttributes(0, 0, 3, 3, 7, 9);
    break;
  case MERCURY_LIGHTHOUSE_ENTRANCE_HALL_AFTER_LIZARD_MAN:
    if (!GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION))
      MercuryLighthouseEntrance_MiaEntersLighthouse();
    break;
  }
  return 0;
}

/// Mercury Lighthouse, the doorway, at
/// MERCURY_LIGHTHOUSE_ENTRANCE_HALL_BEFORE_LIZARD_MAN unless returning after a
/// battle: Mia tries to get past the Lizard Man, who steps across to block her
/// each time; then she waits, watching the leader come up, and the cells before
/// the doorway become walkable.
static void MercuryLighthouseEntrance_LizardManBlocksMia(void) {
  s32 paceCycle;
  struct FieldActor *mia;
  struct FieldActor *actor;

  Engine_EventBegin();
  actor = Object_GetById(ACTOR_FIRST_EAST_STATUE);
  actor->scaleX = -0x10000;
  actor = Object_GetById(ACTOR_FIRST_EAST_STATUE + 1);
  actor->scaleX = -0x10000;
  actor = Object_GetById(ACTOR_FIRST_EAST_STATUE + 2);
  actor->scaleX = -0x10000;
  Actor_SetPosition(MIA, PIXELS(136), PIXELS(184));
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(136), PIXELS(296));
  Actor_SetPosition(ACTOR_LIZARD_MAN, PIXELS(136), PIXELS(152));
  Actor_SetSpeed(MIA, 0x18000, 0xc000);
  Actor_SetSpeed(ACTOR_LIZARD_MAN, 0x18000, 0xc000);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  Engine_CameraSetSpeed(0xcccc, 0x1999);
  Engine_CameraMoveTo(PIXELS(136), -1, PIXELS(184), 0);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Object_GetById(ACTOR_LIZARD_MAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_EventWait(20);
  // Twice she tries the east side and then the west.
  for (paceCycle = 0; paceCycle < 2; paceCycle++) {
    Engine_ActorWalkTo(MIA, 152, 168);
    Engine_EventWait(10);
    Actor_SetActionScript(ACTOR_LIZARD_MAN,
                          gMercuryLighthouseEntrance_LizardManStepEast);
    Engine_ActorWaitForMove(MIA);
    Actor_FaceDirection(MIA, FACING_NORTH, 30);
    Engine_ActorRunRepeatedMotion(MIA, 1);
    mia = Object_GetById(MIA);
    mia->steeringFlags &= ~ACTOR_FACES_MOTION;
    Engine_ActorWalkTo(MIA, 136, 184);
    Engine_EventWait(10);
    Actor_SetActionScript(ACTOR_LIZARD_MAN,
                          gMercuryLighthouseEntrance_LizardManStepCentre);
    Engine_ActorWaitForMove(MIA);
    Object_GetById(MIA)->steeringFlags |= ACTOR_FACES_MOTION;
    Engine_EventWait(30);
    Engine_ActorWalkTo(MIA, 120, 168);
    Engine_EventWait(5);
    Actor_SetActionScript(ACTOR_LIZARD_MAN,
                          gMercuryLighthouseEntrance_LizardManStepWest);
    Engine_ActorWaitForMove(MIA);
    Engine_ActorFaceDirection(MIA, FACING_NORTH, 30);
    Engine_ActorRunRepeatedMotion(MIA, 1);
    Engine_EventWait(15);
    Object_GetById(MIA)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Engine_ActorWalkTo(MIA, 136, 184);
    Engine_EventWait(15);
    Actor_SetActionScript(ACTOR_LIZARD_MAN,
                          gMercuryLighthouseEntrance_LizardManStepCentre);
    Engine_ActorWaitForMove(MIA);
    Engine_ActorRunRepeatedMotion(MIA, 1);
    Object_GetById(MIA)->steeringFlags |= ACTOR_FACES_MOTION;
  }
  Engine_EventWait(20);
  Engine_ActorShowEmote(MIA, EMOTE_IN_FRONT | 2, 60);
  actor = Object_GetById(MIA);
#ifdef UBFIX
  actor->update = MercuryLighthouseEntrance_MiaWatchesLeader;
#else
  actor->update =
      (void (*)(union FieldObject *))MercuryLighthouseEntrance_MiaWatchesLeader;
#endif
  // The leader walks up behind her; the cells before the doorway become
  // walkable.
  Engine_CameraMoveToActor(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(30);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 136, 264);
  Engine_CameraWaitForMove();
  Map_CopyCellAttributes(0, 0, 3, 3, 7, 9);
  Engine_EventEnd();
}

/// Mercury Lighthouse, the doorway, at
/// MERCURY_LIGHTHOUSE_ENTRANCE_HALL_AFTER_LIZARD_MAN, back from the Lizard Man
/// battle, once: Mia nods to the leader and goes on into the lighthouse. Sets
/// FLAG_MIA_ENTERED_MERCURY_LIGHTHOUSE.
static void MercuryLighthouseEntrance_MiaEntersLighthouse(void) {
  struct FieldActor *actor;

  if (GameFlag_IsSet(FLAG_MIA_GOES_IN_SEEN) == 0) {
    GameFlag_Set(FLAG_MIA_GOES_IN_SEEN);
    Engine_EventBegin();
    actor = Actor_Get(ACTOR_FIRST_EAST_STATUE);
    actor->scaleX = -0x10000;
    actor = Object_GetById(ACTOR_FIRST_EAST_STATUE + 1);
    actor->scaleX = -0x10000;
    actor = Actor_Get(ACTOR_FIRST_EAST_STATUE + 2);
    actor->scaleX = -0x10000;
    Actor_SetPosition(MIA, PIXELS(136), PIXELS(144));
    Actor_FaceDirection(MIA, FACING_SOUTH, 10);
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(60);
    Actor_FaceActor(MIA, ACTOR_PARTY_LEADER, 0);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    Engine_EventWait(30);
    Actor_WalkTo(MIA, 136, 72);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorWaitForMove(MIA);
    Actor_SetPosition(MIA, 0, 0);
    GameFlag_Set(FLAG_MIA_ENTERED_MERCURY_LIGHTHOUSE);
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    Engine_EventEnd();
  }
}

#include "common/mercury_lighthouse_lights.h"

/// Spawns the short-lived rising effect at an actor's position.
static void OverlayObject_SpawnKind24AtObject(struct FieldActor *source) {
  struct FieldActor *effect = Engine_ObjectCreate(
      24, source->x.fixed, source->y.fixed, source->z.fixed);

  if (effect != 0) {
    struct AnimationObject *sprite = (struct AnimationObject *)effect->sprite;

    Engine_ObjectSetScript(effect, gMercuryLighthouseEntrance_RiseScript);
    effect->motionFlags = 0;
    effect->terrainId = 1;
    effect->priorityFlags = 2;
    if (sprite != 0) {
      AnimationObjects_SelectAnimation(sprite, 2);
      sprite->flags = 0;
      sprite->image.priority = 3;
    }
  }
}

#include "common/mercury_lighthouse_pillar.h"

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

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gMercuryLighthouseEntrance_RiseScript[] = {
    SC_WAIT(30),
    SC_FREE,
};

s32 gMercuryLighthouse_SparkScript[] = {
    SC_CALL(SceneEffect_AdvanceAnchoredRiseFrame),
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
    SC_SET(SW_SPEED, 0x20000),
    SC_SET(SW_ACCEL, 0x20000),
    SC_SET(SW_STEERING, 0),
    SC_ARG(2),
    SC_MOVE(PIXELS(136), 0, PIXELS(152)),
    SC_CALL(MercuryLighthouseEntrance_PlayStepSound),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gMercuryLighthouseEntrance_LizardManStepWest[] = {
    SC_SET(SW_SPEED, 0x20000),
    SC_SET(SW_ACCEL, 0x20000),
    SC_SET(SW_STEERING, 0),
    SC_ARG(2),
    SC_MOVE(PIXELS(120), 0, PIXELS(152)),
    SC_CALL(MercuryLighthouseEntrance_PlayStepSound),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gMercuryLighthouseEntrance_LizardManStepEast[] = {
    SC_SET(SW_SPEED, 0x20000),
    SC_SET(SW_ACCEL, 0x20000),
    SC_SET(SW_STEERING, 0),
    SC_ARG(2),
    SC_MOVE(PIXELS(152), 0, PIXELS(152)),
    SC_CALL(MercuryLighthouseEntrance_PlayStepSound),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gPaletteCycleRed = 0;

s32 gPaletteCycleGreen = 0;

s32 gPaletteCycleBlue = 0;

struct SceneEntrance gMercuryLighthouseEntrance_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR, CONDITION_ALWAYS, 136, 0, 280, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_HALL_FROM_GODDESS, CONDITION_ALWAYS, 136, 0, 120, FACING_SOUTH, 0, 16, 32, 256, 320, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_HALL, CONDITION_ALWAYS, 200, 0, 648, FACING_NORTH, 0, 80, 432, 320, 688, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_POOLS_WEST, CONDITION_ALWAYS, 120, 0, 520, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_POOLS_EAST, CONDITION_ALWAYS, 280, 0, 520, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE, CONDITION_ALWAYS, 200, 0, 552, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_FROM_WEST_WATERFALLS, CONDITION_ALWAYS, 360, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_FROM_GODDESS_EAST, CONDITION_ALWAYS, 456, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_FROM_GODDESS_WEST, CONDITION_ALWAYS, 616, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_FROM_EAST_WATERFALLS, CONDITION_ALWAYS, 760, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST, CONDITION_ALWAYS, 344, 0, 88, FACING_SOUTH, 0, 304, 32, 800, 208, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST, CONDITION_ALWAYS, 536, 0, 88, FACING_SOUTH, 0, 304, 32, 800, 208, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_DRY_POOL_FROM_CHEST_POOL, FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN, 744, 0, 680, FACING_NORTH, 0, 544, 512, 784, 720, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTHEAST, FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN, 584, 0, 568, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST, FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN, 744, 0, 568, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_DRY_POOL_FROM_CHEST_POOL, CONDITION_ALWAYS, 584, 0, 424, FACING_NORTH, 0, 384, 256, 624, 448, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTHEAST, CONDITION_ALWAYS, 424, 0, 312, FACING_SOUTH, 0, 384, 256, 624, 448, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST, CONDITION_ALWAYS, 584, 0, 312, FACING_SOUTH, 0, 384, 256, 624, 448, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_POOL_FROM_DRY_POOL, CONDITION_ALWAYS, 744, 0, 424, FACING_NORTH, 0, 704, 256, 944, 448, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_TWIN_POOLS, CONDITION_ALWAYS, 344, 0, 872, FACING_SOUTH, 0, 48, 768, 400, 960, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_EAST_WATERFALLS, CONDITION_ALWAYS, 232, 0, 872, FACING_SOUTH, 0, 48, 768, 400, 960, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_TWIN_POOLS, CONDITION_ALWAYS, 568, 0, 872, FACING_SOUTH, 0, 464, 768, 816, 960, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_WEST_WATERFALLS, CONDITION_ALWAYS, 696, 0, 872, FACING_SOUTH, 0, 464, 768, 816, 960, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_PIPE, CONDITION_ALWAYS, 744, 0, 600, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_HALL_BEFORE_LIZARD_MAN, CONDITION_ALWAYS, 136, 0, 296, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_HALL_AFTER_LIZARD_MAN, CONDITION_ALWAYS, 136, 0, 168, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_HALL_UNUSED_NORTH, CONDITION_ALWAYS, 136, 0, 152, FACING_SOUTH, 0, 16, 32, 256, 320, 0 },
    { MERCURY_LIGHTHOUSE_ENTRANCE_HALL_UNUSED_MIDDLE, CONDITION_ALWAYS, 136, 0, 232, FACING_EAST, 0, 16, 32, 256, 320, 0 },
    { SCENE_TABLE_END },
};

u32 gMercuryLighthouseEntrance_Exits[] = {
    SCENE_EXITS(MercuryLighthouseEntrance),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_EXTERIOR, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_ENTRANCE),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_HALL_TO_GODDESS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_HALL),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_HALL, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_HALL_FROM_GODDESS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_POOLS_WEST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_FROM_GODDESS_WEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_POOLS_EAST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_FROM_GODDESS_EAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_TO_WEST_WATERFALLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_TWIN_POOLS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_TO_GODDESS_WEST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_POOLS_WEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_TO_GODDESS_EAST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_POOLS_EAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_TO_EAST_WATERFALLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_TWIN_POOLS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTHWEST, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_SOUTHWEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_WEST_WATERFALLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_EAST_WATERFALLS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_EAST_WATERFALLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_WEST_WATERFALLS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_TWIN_POOLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_FROM_WEST_WATERFALLS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_POOL_TO_DRY_POOL, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_DRY_POOL_FROM_CHEST_POOL),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_TWIN_POOLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_FROM_EAST_WATERFALLS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_DRY_POOL_TO_CHEST_POOL, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_POOL_FROM_DRY_POOL),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_HALL_BEFORE_LIZARD_MAN, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_HALL_BEFORE_LIZARD_MAN),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_PIPE, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_HALL_AFTER_LIZARD_MAN, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_HALL_AFTER_LIZARD_MAN),
    SCENE_EXITS_END,
};

struct ScenePlacement gMercuryLighthouseEntrance_Placements[] = {
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0xc0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, 7,
        PIXELS(88), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, 7,
        PIXELS(88), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, 7,
        PIXELS(88), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, 7,
        PIXELS(184), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, 7,
        PIXELS(184), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, 7,
        PIXELS(184), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(616), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, 7,
        PIXELS(200), 0, PIXELS(512), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gMercuryLighthouseEntrance_Events[] = {
    { TOUCH_WALKING, 31, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouseEntrance_HopAcrossPool) },
    { EVENT_TOUCH, 31, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouseEntrance_LoseLights) },
    { 0xa02, 21, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouseEntrance_HopAcrossPool) },
    { EVENT_TOUCH, 21, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouseEntrance_SparkleAtLeader) },
    { TOUCH_WALKING, 11, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouseEntrance_HopAcrossPool) },
    { EVENT_TOUCH, 11, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouseEntrance_ChargeLights) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_ReturnToDoorway) },
    { EVENT_TOUCH, 40, IF_FLAG_SET(FLAG_LIZARD_MAN_STEPPED_ASIDE), EVENT_SCRIPT(MercuryLighthouseEntrance_LizardManReturnsToCentre) },
    { EVENT_TOUCH, 42, IF_FLAG_SET(FLAG_LIZARD_MAN_STEPPED_ASIDE), EVENT_SCRIPT(MercuryLighthouseEntrance_LizardManReturnsToCentre) },
    { EVENT_TOUCH, 41, FLAG_LIZARD_MAN_STEPPED_ASIDE, EVENT_SCRIPT(MercuryLighthouseEntrance_LizardManBlocksLeader) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 40, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_FightLizardMan) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 41, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_FightLizardMan) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 67, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_LeaveChestPoolByPassage) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 68, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_LeaveEastWaterfallsByPassage) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 69, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_LeaveDryPoolByPassage) },
    { EVENT_EXIT, 51, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_EXTERIOR },
    { EVENT_EXIT, 52, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_HALL_TO_GODDESS },
    { EVENT_EXIT, 53, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_HALL },
    { EVENT_EXIT, 54, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_POOLS_WEST },
    { EVENT_EXIT, 55, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_POOLS_EAST },
    { EVENT_EXIT, 56, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_TO_WEST_WATERFALLS },
    { EVENT_EXIT, 57, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_TO_GODDESS_WEST },
    { EVENT_EXIT, 58, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_TO_GODDESS_EAST },
    { EVENT_EXIT, 59, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_POOLS_TO_EAST_WATERFALLS },
    { EVENT_EXIT, 60, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST },
    { EVENT_EXIT, 61, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 62, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTHWEST },
    { EVENT_EXIT, 63, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST },
    { EVENT_EXIT, 64, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_WEST_WATERFALLS },
    { EVENT_EXIT, 65, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_EAST_WATERFALLS },
    { EVENT_EXIT, 66, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_TWIN_POOLS },
    { EVENT_TALK, MIA, CONDITION_ALWAYS, MSGNO(MsgMercuryMonsterInsideTower) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), MIA, CONDITION_ALWAYS, MSGNO(MsgMercuryLetMePass) },
    { EVENT_TALK, ACTOR_LIZARD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_FightLizardMan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LIZARD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_FightLizardMan) },
    { SEARCH(SEARCH_UNNAMED), 1, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_ReadHeartInscription) },
    { PSYNERGY_ON_ACTOR(ABILITY_PLY), ACTOR_PLY_TARGET, FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED, EVENT_SCRIPT(MercuryLighthouseEntrance_ActivateWithPly) },
    { PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_RemoveColumnOnRetreat) },
    { SEARCH(SEARCH_CHEST), 100, FLAG_MERCURY_LIGHTHOUSE_PSYNERGY_ARMOR, FIND_ITEM(ITEM_PSYNERGY_ARMOR) },
    { SEARCH(SEARCH_CHEST), 101, FLAG_MERCURY_LIGHTHOUSE_MIMIC_BEATEN, FIND_MIMIC },
    { SCENE_EVENTS_END },
};

/// Each spark pulses in size four times, then is released.
s32 gMercuryLighthouseEntrance_SparkBurstScript[] = {
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 0xa000),
    SC_SET(SW_SCALE_X, 0xa000),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 0x4000),
    SC_SET(SW_SCALE_X, 0x4000),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 0xa000),
    SC_SET(SW_SCALE_X, 0xa000),
    SC_WAIT(6),
    SC_LOOP(4, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};
// clang-format on
