/// @file
/// Suhalla Desert (SceneId_SuhallaDesertNorth, SceneId_SuhallaDesertSouth and
/// SceneId_SuhallaDesertCaves), the desert of
/// sandstorms between Suhalla and Suhalla Gate.
///
/// After Babi's audience the party crosses it with Iodem, who commands
/// Babi's soldiers and travels with the party, looking for Sheba, the holy
/// child of Lalivero: the soldiers escorting her home were caught in the
/// sandstorms, and in Suhalla the survivors said she had not been caught with
/// them. The three scenes, in the order the party meets them:
/// - Suhalla Desert (SceneId_SuhallaDesertNorth), entered from the world map
///   near Suhalla, where SUHALLA_DESERT_NORTH_TO_WORLD_MAP leads back out;
///   SUHALLA_DESERT_NORTH_TO_SUHALLA_DESERT_SOUTH leads on into
///   SuhallaDesertSouth. Five storms stand here.
/// - Suhalla Desert (SceneId_SuhallaDesertSouth), with four storms and a
///   fifth that gives chase. SUHALLA_DESERT_SOUTH_TO_WORLD_MAP leads out onto
///   the world map, and SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_CAVES_EAST and
///   SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_CAVES_WEST into SuhallaDesertCaves.
///   Flash, a Mars Djinni, waits here and fights when spoken to
///   (Event_FightDjinni). Near the way out, once Iodem has questioned the
///   soldiers (FLAG_IODEM_MET_WOUNDED_SOLDIERS), he steps out of the party:
///   `Sheba is nowhere to be found.` He asks whether to search on or press on
///   to Lalivero, and either way decides to head first for the guard gate ahead
///   and leave Sheba to its soldiers (Event_IodemAsksWhetherToPressOn, once,
///   FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA).
/// - Suhalla Desert (SceneId_SuhallaDesertCaves), two caves under the
///   desert, whose exits lead only back into SuhallaDesertSouth and between its
///   two caves; it places nothing and plays a sound on entry.
///
/// The storms are ACTOR_FIRST_STORM to ACTOR_LAST_STORM of the first two
/// scenes, and their logic is the desert's:
/// - As the party nears a storm (cell codes 61 to 65 and 71 to 73) it rises,
///   growing to full size, and the sand haze fades in the first time
///   (SuhallaDesert_RaiseStorm). The first storm to rise while Iodem travels
///   with the party brings him out to warn, `If we don't get past here, we'll
///   never make it to Babi Lighthouse.` (FLAG_SUHALLA_DESERT_IODEM_WARNED).
/// - Walking into a storm (cell codes 41 to 45 and 51 to 54) draws the leader
///   to its centre and lifts the party, spinning, for 180 frames
///   (SuhallaDesert_CatchInStorm). Douse cast at it in that time stops it,
///   and the monster in it attacks (Event_DouseStorm): the water the wounded
///   soldiers in Suhalla saw others use.
/// - When the time runs out the storm rises into the sky with the party and
///   drops it on the world map outside Suhalla (WORLD_MAP_FROM_SUHALLA, where
///   SUHALLA_TO_WORLD_MAP also leads), which is why everyone the storms catch
///   winds up there; the second scene's fourth storm drops it at
///   WORLD_MAP_STORM_DROP_FOURTH instead, one of the world map's entrances
///   that resume airborne travel.
/// - In the second scene, cell code 81 brings the fifth storm up behind the
///   party to chase it (SuhallaDesert_ChaseLeader); it catches a leader on
///   the move.
///
/// A doused storm's flag (five from FLAG_FIRST_SCENE_STORM_DOUSED and five
/// from FLAG_SECOND_SCENE_STORM_DOUSED) is a place flag:
/// the storm stays gone until the party leaves the desert's place group,
/// except the chasing storm, which once doused is gone for good
/// (FLAG_SUHALLA_DESERT_CHASING_STORM_DOUSED). In the second scene Reveal
/// shows an object the sand hides, at 248, 712, changes two cells and moves
/// map object 100 while one palette colour pulses; in Suhalla a villager
/// cannot remember a saying about the storms, and Mind Read finds half of it,
/// about eyes of truth. Chests hold a
/// Virtuous Armlet, a Lucky Medal and a Cookie, a fourth chest is a mimic,
/// and searching finds a Psynergy Stone (FLAG_SUHALLA_DESERT_PSYNERGY_STONE).

#include "script.h"
#include "flags.h"
#include "version.h"
#include "game_state.h"
#include "engine_call.h"
#include "unit/scene/suhalla_desert.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "field_obj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "djinn_ids.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fx_scene.h"
#include "global_cells.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "call.h"
#include "field_effect.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "objcmd.h"
#include "iowrite.h"
#include "songs.h"
#include "heap_state.h"
#include "map.h"

// FLAG_SCENE_TEMP_06 to _0A: storms ACTOR_FIRST_STORM to ACTOR_LAST_STORM have
// risen during this visit.
// FLAG_SCENE_TEMP_0E: the chasing storm has set off after the party.
// FLAG_SCENE_TEMP_0F: a storm has caught the leader.
// FLAG_SCENE_TEMP_10: a byte, the frames left before the storm carries the
// party away.
// FLAG_SCENE_TEMP_18: a byte, the storm that caught the leader.

/// Storms doused in the first and second scenes, one flag per storm from the
/// first; each storm stays away once doused.
enum {
  FLAG_FIRST_SCENE_STORM_DOUSED = 0x301,
  FLAG_SECOND_SCENE_STORM_DOUSED = 0x311,
  /// A storm has risen: the sand haze stays faded in.
  FLAG_STORMS_RISEN = 0x340,
  /// The Psynergy Stone has been found, a place flag.
  FLAG_SUHALLA_DESERT_PSYNERGY_STONE = FLAG_PLACE_TEMP_51
};

/// The actors, numbering the placement rows of the first two scenes from
/// ACTOR_FIRST_PLACED; the third scene places none.
enum {
  /// The storms, five in each scene.
  ACTOR_FIRST_STORM = ACTOR_FIRST_PLACED,
  ACTOR_FOURTH_STORM = ACTOR_FIRST_STORM + 3,
  ACTOR_LAST_STORM = ACTOR_FIRST_STORM + 4,
  /// In the second scene the last storm gives chase.
  ACTOR_CHASING_STORM = ACTOR_LAST_STORM,
  /// Iodem, who steps out of the party to talk and then rejoins it.
  ACTOR_IODEM,
  /// In the second scene, seen only while Reveal lasts, drawn as
  /// Goma Cave's stone pillars. XXX: no text names it here.
  ACTOR_REVEALED_OBJECT,
  ACTOR_FLASH ///< in the second scene, the Mars Djinni Flash
};

/// Frames a caught party spins before the storm carries it away.
enum { STORM_CAPTURE_FRAMES = 180 };

/// Where a storm drops the party on the world map: outside Suhalla, or, for
/// the second scene's fourth storm, at an entrance that resumes airborne
/// travel.
enum {
  WORLD_MAP_STORM_DROP = WORLD_MAP_FROM_SUHALLA,
  WORLD_MAP_STORM_DROP_FOURTH = 77
};

static void SuhallaDesert_SetUpFirstSceneStorms(void);
static void SuhallaDesert_SetUpSecondSceneStorms(void);

static u16 sSuhallaDesertSavedColor;

#define SUHALLA_REVEAL_PALETTE_COLOR (*(volatile u16 *)0x0500019e)

#include "common/effect_create.h"
#include "common/effect_spawn.h"

void QueueSceneSound(s32 cue);

/// In the Suhalla Desert's second scene (165), talking to Flash, a Mars
/// Djinni, starts its battle.
static void Event_FightDjinni(void) {
  Field_RequestDjinnBattle(ACTOR_FLASH, DJINNI_FLASH);
}

/// A storm's placement script: clears its sprite and collision flags.
static s32 SuhallaDesert_ClearActorSpriteAndCollisionFlags(
    struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  Engine_ActorSetSpriteFlags(actor, 0);
  actor->collisionFlags = 0;
  return 0;
}

/// A storm's placement script: toggles its single-sprite storage.
static s32
SuhallaDesert_ToggleActorActive(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  actor->spriteStorageKind ^= 1;
  return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_SuhallaDesertNorth) {
    return gSuhallaDesertEntrances1;
  }
  if (selector == (s32)&SceneId_SuhallaDesertSouth) {
    return gSuhallaDesertEntrances2;
  }
  if (selector == (s32)&SceneId_SuhallaDesertCaves) {
    return gSuhallaDesertEntrances3;
  }
  return gSuhallaDesertEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

/// Where each exit of the three scenes leads.
const u32 *Scene_GetExits(void) { return gSuhallaDesertExits; }

/// The storms, Iodem, and in the second scene Flash and what Reveal shows.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_SuhallaDesertNorth) {
    return gSuhallaDesertPlacements1;
  }
  if (selector == (s32)&SceneId_SuhallaDesertSouth) {
    return gSuhallaDesertPlacements2;
  }
  if (selector == (s32)&SceneId_SuhallaDesertCaves) {
    return gSuhallaDesertPlacements3;
  }
  return gSuhallaDesertPlacementsOther;
}

/// Each frame while caught: spins the leader, and counts down to raising event
/// 99, the storm carrying the party away.
static void SuhallaDesert_UpdateCaptureTimer(void) {
  struct FieldActor *selectedActor;
  struct EventWork *requests;
  s32 remainingFrames;

  selectedActor = Object_GetById(gGameState.selectedActor);
  requests = gEventWork;
  selectedActor->facing = (gFrameCount << 12);

  remainingFrames = FieldGameFlag_GetByte(FLAG_SCENE_TEMP_10);
  if (remainingFrames != 0) {
    if (remainingFrames == 1) {
      requests->raisedTrigger = 99;
    } else if (Engine_GameFlagIsSet(FLAG_MENU_OPEN) == 0) {
      remainingFrames -= 1;
    }
  }
  FieldGameFlag_SetByte(FLAG_SCENE_TEMP_10, remainingFrames);
}

/// The leader walks into a storm: drawn to its centre and lifted, the party
/// spins until Douse stops the storm or it carries them away.
static void SuhallaDesert_CatchInStorm(s32 stormId) {
  s32 stormIdCopy;
  s32 captureActive;
  struct FieldActor *selectedActor;
  struct FieldActor *storm;
  s32 selectedActorId;

  // The event-selected actor is pulled toward the storm before being lifted.
  selectedActorId = gGameState.selectedActor;
  stormIdCopy = stormId;
  selectedActor = Object_GetById(selectedActorId);
  Actor_Get(stormIdCopy);
  captureActive = GameFlag_IsSet(FLAG_SCENE_TEMP_0F);
  if (captureActive == 0) {
    Engine_EventBegin();
    Actor_SetAttachedEffect(selectedActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    Engine_ActorSetAnimation(selectedActorId, 9);
    storm = Object_GetById(stormIdCopy);
    if (storm != 0) {
      Actor_SetDestination(selectedActorId, storm->x.part.pixel,
                           storm->z.part.pixel);
    }
    Engine_ActorWaitForMove(selectedActorId);
    // Start the capture countdown once the approach finishes.
    Event_PlayCue(SE_244);
    FieldScheduler_AddCallback(SuhallaDesert_UpdateCaptureTimer,
                               TASK_PRIORITY_SCENE);
    selectedActor->motionFlags = captureActive;
    Engine_ObjectSetPosition(selectedActor, selectedActor->x.fixed,
                             selectedActor->y.fixed + PIXELS(32),
                             selectedActor->z.fixed);
    Engine_ActorWaitForMove(selectedActorId);
    selectedActor->velocityY = captureActive;
    selectedActor->motionFlags = 4;
    gGameState.movementMode = LEADER_MOVEMENT_AIRBORNE;
    GameFlag_Set(FLAG_SCENE_TEMP_0F);
    FieldGameFlag_SetByte(FLAG_SCENE_TEMP_18, stormIdCopy);
    FieldGameFlag_SetByte(FLAG_SCENE_TEMP_10, STORM_CAPTURE_FRAMES);
    Engine_EventEnd();
    gEventWork->encounterEnemyGroup = 0; // drop any battle this step rolled
  }
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the leader walks into the scene's first storm
/// (cell code 41 or 51).
static void Event_CaughtByFirstStorm(void) {
  SuhallaDesert_CatchInStorm(ACTOR_FIRST_STORM);
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the leader walks into the second storm (cell
/// code 42 or 52).
static void Event_CaughtBySecondStorm(void) {
  SuhallaDesert_CatchInStorm(ACTOR_FIRST_STORM + 1);
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the leader walks into the third storm (cell
/// code 43 or 53).
static void Event_CaughtByThirdStorm(void) {
  SuhallaDesert_CatchInStorm(ACTOR_FIRST_STORM + 2);
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the leader walks into the fourth storm (cell
/// code 44 or 54).
static void Event_CaughtByFourthStorm(void) {
  SuhallaDesert_CatchInStorm(ACTOR_FIRST_STORM + 3);
}

/// In Suhalla Desert, the leader walks into SuhallaDesertNorth's fifth storm
/// (cell code 45), or SuhallaDesertSouth's chasing storm reaches the moving
/// leader (event 55).
static void Event_CaughtByFifthStorm(void) {
  SuhallaDesert_CatchInStorm(ACTOR_FIRST_STORM + 4);
}

/// As the party nears a storm it rises, growing to full size with the sand
/// haze fading in the first time. The first time a storm rises with Iodem in
/// the party, he steps out and warns of the sandstorms: "If we don't get past
/// here, we'll never make it to Babi Lighthouse."
static void SuhallaDesert_RaiseStorm(s32 stormId) {
  s32 frame;
  s32 nextFrame;
  s32 stormIdCopy;
  s32 leaderX;
  s32 leaderZ;
  struct FieldActor *storm;
  s32 appearanceRestored;
  struct FieldActor *selectedActor;
  struct FieldActor *leader;

  stormIdCopy = stormId;
  selectedActor = Object_GetById(gGameState.selectedActor);
  storm = Object_GetById(stormIdCopy);
  appearanceRestored = Engine_GameFlagIsSet(FLAG_STORMS_RISEN);
  leaderX = selectedActor->x.part.pixel;
  leaderZ = selectedActor->z.part.pixel;
  Engine_EventBegin();
  Engine_AudioPlayCue(SE_244);
  // Grow over 16 frames and, the first time, fade the haze in with it.
  frame = 0;
  do {
    storm->scaleX = ((frame << 11) + 0x800);
    storm->scaleY = ((frame << 12) + 0x1000);
    if (appearanceRestored == 0) {
      nextFrame = frame + 1;
      IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - nextFrame) << 8) | nextFrame,
                          0x20000);
    } else {
      nextFrame = frame + 1;
    }
    Engine_TaskWait(1);
    frame = nextFrame;
  } while (frame <= 15);
  Engine_GameFlagSet((FLAG_SCENE_TEMP_06 - ACTOR_FIRST_STORM + stormIdCopy));
  Engine_GameFlagSet(FLAG_STORMS_RISEN);
  if (Engine_GameFlagIsSet(FLAG_IODEM_FOLLOWING_PARTY) != 0 &&
      GameFlag_IsSet(FLAG_SUHALLA_DESERT_IODEM_WARNED) == 0) {
    Engine_GameFlagSet(FLAG_SUHALLA_DESERT_IODEM_WARNED);
    Actor_SetSpeed(ACTOR_IODEM, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if ((s32)leader != 0) {
      Engine_ActorSetPosition(ACTOR_IODEM, leader->x.fixed, leader->z.fixed);
    }
    Actor_FaceActor(ACTOR_IODEM, FACING_SOUTH, 0);
    Object_GetById(ACTOR_IODEM)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Engine_ActorWalkTo(ACTOR_IODEM, leaderX, (leaderZ - 16));
    Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, (leaderX + 8),
                              (leaderZ - 40));
    Engine_EventWait(1);
    Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
    Engine_ActorSetAnimation(ACTOR_IODEM, 1);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Engine_EventSetMessage(MsgSuhallaSandstorm);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 6, 60);
    Engine_EventShowMessage(ACTOR_IODEM, 0);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 60);
    Engine_EventShowMessage(ACTOR_IODEM, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 1, 60);
    Engine_EventShowMessage(ACTOR_IODEM, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
    Engine_EventShowMessage(ACTOR_IODEM, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_IODEM, 3);
    Engine_EventShowMessage(ACTOR_IODEM, 0);
    Actor_SetSpeed(ACTOR_IODEM, 0x10000, 0x8000);
    Engine_ActorSetAnimation(ACTOR_IODEM, 2);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if ((s32)leader != 0) {
      Engine_ActorSetDestination(ACTOR_IODEM, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_IODEM);
    Engine_ActorSetPosition(ACTOR_IODEM, 0, 0);
  }
  Engine_EventEnd();
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the party nears the scene's first storm (cell
/// code 61 or 71).
static void Event_FirstStormRises(void) {
  SuhallaDesert_RaiseStorm(ACTOR_FIRST_STORM);
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the party nears the second storm (cell code 62
/// or 72).
static void Event_SecondStormRises(void) {
  SuhallaDesert_RaiseStorm(ACTOR_FIRST_STORM + 1);
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the party nears the third storm (cell code 63
/// or 73).
static void Event_ThirdStormRises(void) {
  SuhallaDesert_RaiseStorm(ACTOR_FIRST_STORM + 2);
}

/// In SuhallaDesertNorth, the party nears the fourth storm (cell code
/// 64).
static void Event_FourthStormRises(void) {
  SuhallaDesert_RaiseStorm(ACTOR_FIRST_STORM + 3);
}

/// In SuhallaDesertNorth, the party nears the fifth storm (cell code
/// 65).
static void Event_FifthStormRises(void) {
  SuhallaDesert_RaiseStorm(ACTOR_FIRST_STORM + 4);
}

/// In SuhallaDesertSouth, near its way out to the world map
/// (cell code 9), once Iodem has heard the wounded soldiers in Suhalla: he
/// steps out of the party. "Sheba is nowhere to be found. Should we search
/// the desert once more or press on to Lalivero?" Whatever the answer, Iodem
/// decides to head first for the guard gate ahead, Suhalla Gate, and leave
/// Sheba to the soldiers there, and rejoins the party. Sets
/// FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA.
static void Event_IodemAsksWhetherToPressOn(void) {
  struct FieldActor *leader;

  if (GameFlag_IsSet(FLAG_IODEM_FOLLOWING_PARTY) != 0 &&
      GameFlag_IsSet(0x1b7) == 0 &&
      GameFlag_IsSet(FLAG_IODEM_MET_WOUNDED_SOLDIERS) != 0) {
    GameFlag_Set(FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA);
    Engine_EventBegin();
    Engine_EventSetMessage(MsgSuhallaNowhereFound);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetPosition(ACTOR_IODEM, leader->x.fixed, leader->z.fixed);
    }
    Actor_FaceActor(ACTOR_IODEM, FACING_NORTH, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 440, 1256);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 444, 1240);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 30);
    Engine_ActorSetAnimationAndWait(ACTOR_IODEM, 4);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 60);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 5, 60);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 30);
    Event_AskYesNo(ACTOR_IODEM, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 6, 60);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_IODEM, 3);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Actor_SetSpeed(ACTOR_IODEM, 0xb333, 0x5999);
    Actor_WalkToAndWait(ACTOR_IODEM, 440, 1256);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_IODEM, 2);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetDestination(ACTOR_IODEM, leader->x.part.pixel,
                           leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_IODEM);
    Actor_SetPosition(ACTOR_IODEM, 0, 0);
    Engine_EventEnd();
  }
}

/// The chasing storm's script: it follows the leader, throwing up sand every
/// eight frames, and catches a moving leader it comes within 8 pixels of
/// (event 55).
static s32 SuhallaDesert_ChaseLeader(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  struct EffectOptions options;
  struct FieldActor *selectedActor;
  s32 dx;
  s32 dz;
  s32 sparkFrame;

  selectedActor = Object_GetById(gGameState.selectedActor);
  actor->acceleration = 0x4000;
  actor->speed = 0x18000;
  actor->motionFlags = 0;
  Engine_ActorSetSpriteFlags(actor, 0);
  actor->spriteStorageKind ^= 1;
  if (Engine_GameFlagIsSet(FLAG_FIELD_HANDLING_REQUESTS)) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
  } else {
    actor->targetX = selectedActor->x.fixed;
    actor->targetY = selectedActor->terrainHeight;
    actor->targetZ = selectedActor->z.fixed;
    dx = actor->x.fixed - selectedActor->x.fixed;
    if (dx < 0) {
      dx = selectedActor->x.fixed - actor->x.fixed;
    }
    dz = actor->z.fixed - selectedActor->z.fixed;
    if (dx + (dz >= 0 ? actor->z.fixed - selectedActor->z.fixed
                      : selectedActor->z.fixed - actor->z.fixed) <
        0x80000) {
      struct EventWork *work = gEventWork;

      if (selectedActor->motionFlags != 0) {
        work->raisedTrigger = 55;
      }
      actor->motionFlags = 3;
      actor->targetX = selectedActor->x.fixed;
      actor->targetY = selectedActor->y.fixed;
      actor->targetZ = selectedActor->z.fixed;
    }
  }
  // Sand every eight frames, with a random spin.
  sparkFrame = gFrameCount & 7;
  if (sparkFrame == 0) {
    options.startScaleX = 0xcccc;
    options.startScaleY = 0xcccc;
    options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, sparkFrame,
                 sparkFrame, 0x880001, &options);
  }
  return 1;
}

/// In SuhallaDesertSouth, until the chasing storm has been doused: it
/// appears behind the party and gives chase.
static void Event_StormGivesChase(void) {
  if (GameFlag_IsSet(FLAG_SUHALLA_DESERT_CHASING_STORM_DOUSED) == 0) {
    GameFlag_Set(FLAG_SCENE_TEMP_0E);
    Actor_SetPosition(ACTOR_CHASING_STORM, PIXELS(120), PIXELS(824));
    Actor_SetActionScript(ACTOR_CHASING_STORM, gSuhallaDesertChaseScript);
  }
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), Douse cast at the storm that caught the leader:
/// sets the Psynergy effect's child option (its meaning unknown).
static void Event_DouseCastAtStorm(void) {
  u8 *state = gEffectWork;
  ((struct BattleFxScene *)state)->childOption = 1;
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), Douse stops the storm that caught the leader:
/// it is doused for good, and the monster in it attacks.
/// SceneId_SuhallaDesertSouth's fourth and chasing storms hold other monsters.
static void Event_DouseStorm(s32 unused, s32 stormId) {
  struct GameState *state;

  // The storm no longer carries the party away.
  if (gEventWork->raisedTrigger == 99) {
    gEventWork->raisedTrigger = 0;
  }
  Engine_GameFlagClear(FLAG_SCENE_TEMP_0F);
  if (gGameState.scene == (s32)&SceneId_SuhallaDesertNorth) {
    Engine_GameFlagSet(
        (stormId + FLAG_FIRST_SCENE_STORM_DOUSED - ACTOR_FIRST_STORM));
  } else {
    if (gGameState.scene == (s32)&SceneId_SuhallaDesertSouth) {
      Engine_GameFlagSet(
          (stormId + FLAG_SECOND_SCENE_STORM_DOUSED - ACTOR_FIRST_STORM));
    }
  }
  FieldGameFlag_SetByte(FLAG_SCENE_TEMP_10, 0);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 5);
  Battle_SetStart(BATTLE_START_SCENE);
  state = &gGameState;
  if (state->scene == (s32)&SceneId_SuhallaDesertSouth) {
    if (stormId == ACTOR_FOURTH_STORM) {
      Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 7);
    } else {
      if (stormId == ACTOR_CHASING_STORM) {
        Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 6);
        Engine_ActorStop(ACTOR_CHASING_STORM);
        Engine_ActorSetPosition(ACTOR_CHASING_STORM, 0, 0);
      }
    }
  }
  Object_GetById(gGameState.selectedActor)->motionFlags = 3;
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the spinning runs out (event 99): the storm
/// rises into the sky with the party, the screen closes, and the party lands
/// falling (FLAG_ARRIVE_FALLING) on the world map outside Suhalla, or where the
/// second scene's fourth storm drops it.
static void Event_StormCarriesPartyAway(void) {
  s32 stormId;
  struct FieldActor *selectedActor;
  struct FieldActor *storm;
  s32 frame;

  stormId = FieldGameFlag_GetByte(FLAG_SCENE_TEMP_18);
  selectedActor = Object_GetById(gGameState.selectedActor);
  storm = Object_GetById(stormId);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_AudioPlayCue(SE_219);
  // BUG: the native passes an actor id where the sprite helper needs a pointer.
#ifdef BUGFIX
  Engine_ActorSetSpriteFlags(selectedActor, 0);
#else
  Engine_ActorSetSpriteFlags((struct FieldActor *)gGameState.selectedActor, 0);
#endif
  storm->motionFlags = 0;
  selectedActor->motionFlags = 0;
  selectedActor->velocityY = 0;
  selectedActor->frozen = 1;
  storm->frozen = 1;
  // Both rise faster and faster for a second.
  for (frame = 0; frame < 60; frame++) {
    selectedActor->velocityY += 0x3333;
    storm->velocityY += 0x3333;
    Engine_TaskWait(1);
  }
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
  Engine_GameFlagSet(FLAG_ARRIVE_FALLING);
  if (gGameState.scene == (s32)&SceneId_SuhallaDesertSouth &&
      FieldGameFlag_GetByte(FLAG_SCENE_TEMP_18) == ACTOR_FOURTH_STORM) {
    Event_SetDestination((s32)&SceneId_WorldMap, WORLD_MAP_STORM_DROP_FOURTH);
  } else {
    Event_SetDestination((s32)&SceneId_WorldMap, WORLD_MAP_STORM_DROP);
  }
}

/// While Reveal lasts: one palette color pulses, brightening and dimming over
/// 64 frames.
static void SuhallaDesert_PulseRevealPalette(void) {
  u16 brightnessFrame = gFrameCount & 63;
  s32 color;

  if (brightnessFrame > 31)
    brightnessFrame = 64 - brightnessFrame;
  color = (brightnessFrame >> 1) + 7;
  color |= (color << 10) | (color << 5);
  SUHALLA_REVEAL_PALETTE_COLOR = ((u32)color << 16) >> 16;
}

/// Reveal ends, or a scene is entered: in SuhallaDesertSouth, what
/// Reveal showed is hidden again and the pulsing color is put back.
// Native event return forms: see MATCHING.md.
#ifdef UBFIX
static void Event_RevealEnds(void) {
#else
static s32 Event_RevealEnds(void) {
  s32 removalResult;
#endif
  if (gGameState.scene == (s32)&SceneId_SuhallaDesertSouth) {
    Object_GetById(ACTOR_REVEALED_OBJECT)->priorityFlags = 2;
    Object_GetById(ACTOR_REVEALED_OBJECT)->motionFlags = 3;
    Actor_SetPosition(ACTOR_REVEALED_OBJECT, 0, 0);
    Map_CopyCellAttributes(16, 44, 1, 1, 15, 44);
    MapObject_SetPosition(100, 0, 0);
    Map_CopyCellAttributes(12, 71, 1, 1, 127, 127);
    Map_CopyCellAttributes(11, 71, 1, 1, 12, 71);
#ifdef UBFIX
    Engine_TaskRemoveCallback(SuhallaDesert_PulseRevealPalette);
#else
    removalResult = Engine_TaskRemoveCallback(SuhallaDesert_PulseRevealPalette);
#endif
    Io_Write16(sSuhallaDesertSavedColor, &SUHALLA_REVEAL_PALETTE_COLOR);
#ifndef UBFIX
    return removalResult;
#endif
  }
}

/// Reveal in SuhallaDesertSouth: the hidden object appears at 248, 712,
/// map cells and the map object at 100 change, and a palette color pulses.
#ifdef UBFIX
static void Event_RevealStarts(void) {
#else
static s32 Event_RevealStarts(void) {
#endif
  if (gGameState.scene == (s32)&SceneId_SuhallaDesertSouth) {
    Object_GetById(ACTOR_REVEALED_OBJECT)->priorityFlags = 2;
    Object_GetById(ACTOR_REVEALED_OBJECT)->motionFlags = 0;
    Actor_SetPosition(ACTOR_REVEALED_OBJECT, PIXELS(248), PIXELS(712));
    Map_CopyCellAttributes(31, 95, 1, 1, 15, 44);
    MapObject_SetPosition(100, -1, -1);
    Marker_MoveLeaderAway();
    Map_CopyCellAttributes(127, 127, 1, 1, 12, 71);
#ifdef UBFIX
    FieldScheduler_AddCallback(SuhallaDesert_PulseRevealPalette,
                               TASK_PRIORITY_SCENE);
#else
    return FieldScheduler_AddCallback(SuhallaDesert_PulseRevealPalette,
                                      TASK_PRIORITY_SCENE);
#endif
  }
}

/// The third scene's exits, or the storms, Iodem, treasure and Reveal of the
/// first two.
const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_SuhallaDesertCaves) {
    return gSuhallaDesertEvents3;
  }
  return gSuhallaDesertEventsOther;
}

/// Sets a scene up as the party enters: a capture goes on spinning, Reveal's
/// color is saved, and the first two scenes set up their storms.
s32 Scene_Initialize(void) {
  struct MapState *map;

  map = WORK_SLOT(HEAP_SLOT_MAP);
  ((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  if (FieldGameFlag_GetByte(FLAG_SCENE_TEMP_10) != 0) {
    gGameState.movementMode = LEADER_MOVEMENT_AIRBORNE;
    FieldScheduler_AddCallback(SuhallaDesert_UpdateCaptureTimer,
                               TASK_PRIORITY_SCENE);
  }
  if (gGameState.scene == (s32)&SceneId_SuhallaDesertNorth ||
      gGameState.scene == (s32)&SceneId_SuhallaDesertSouth) {
    sSuhallaDesertSavedColor = SUHALLA_REVEAL_PALETTE_COLOR;
    Event_RevealEnds();
  }
  if (gGameState.scene == (s32)&SceneId_SuhallaDesertNorth) {
    SuhallaDesert_SetUpFirstSceneStorms();
  } else if (gGameState.scene == (s32)&SceneId_SuhallaDesertSouth) {
    SuhallaDesert_SetUpSecondSceneStorms();
  } else {
    Engine_AudioPlayCue(SE_288);
  }
  if (gGameState.entrance == ENTRANCE_DEFAULT) {
    map->flags &= ~DISPCNT_BG1_ON;
  }
  return 0;
}

/// The first scene's storms on entry: a doused storm counts as risen, the
/// others start small (unless the party is coming back to where it stood), and
/// the sand haze is faded in if a storm has risen before.
static void SuhallaDesert_SetUpFirstSceneStorms(void) {
  // Trial: sharing the blend tail through an ordinary inline shrinks this
  // function from 0x144 to 0x138 bytes in tbs-en; retain the original body.
  struct FieldActor *actor;
  s32 stormId;
  s32 stormAlpha = 0;

  if (Engine_GameFlagIsSet(FLAG_FIRST_SCENE_STORM_DOUSED)) {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_06);
  }
  if (Engine_GameFlagIsSet(FLAG_FIRST_SCENE_STORM_DOUSED + 1)) {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_07);
  }
  if (Engine_GameFlagIsSet(FLAG_FIRST_SCENE_STORM_DOUSED + 2)) {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_08);
  }
  if (Engine_GameFlagIsSet(FLAG_FIRST_SCENE_STORM_DOUSED + 3)) {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_09);
  }
  if (Engine_GameFlagIsSet(FLAG_FIRST_SCENE_STORM_DOUSED + 4)) {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_0A);
  }
  for (stormId = ACTOR_FIRST_STORM; stormId <= ACTOR_LAST_STORM; stormId++) {
    actor = Object_GetById(stormId);
    if (actor != 0) {
      if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        actor->scaleX = 0x800;
        actor->scaleY = 0x800;
      }
      actor->sprite->flags = 0;
    }
  }
  IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f42, 0x20000);
  if (Engine_GameFlagIsSet(FLAG_STORMS_RISEN)) {
    stormAlpha = 16;
    QueueSceneSound(SE_244);
  }
  IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - stormAlpha) << 8) | stormAlpha,
                      0x20000);
}

/// The second scene's storms on entry, as the first scene's; the fourth storm's
/// first sprite part takes parameter 10, and once the chasing storm has been
/// doused it is gone for good.
static void SuhallaDesert_SetUpSecondSceneStorms(void) {
  // Trial: the same blend-tail inline shrinks this function from 0x158 to 0x14c
  // bytes in tbs-en; retain the original body.
  struct FieldActor *actor;
  struct AnimationObject *sprite;
  s32 stormId;
  s32 stormAlpha = 0;

  if (Engine_GameFlagIsSet(FLAG_SECOND_SCENE_STORM_DOUSED)) {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_06);
  }
  if (Engine_GameFlagIsSet(FLAG_SECOND_SCENE_STORM_DOUSED + 1)) {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_07);
  }
  if (Engine_GameFlagIsSet(FLAG_SECOND_SCENE_STORM_DOUSED + 2)) {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_08);
  }
  for (stormId = ACTOR_FIRST_STORM; stormId < ACTOR_FOURTH_STORM; stormId++) {
    actor = Object_GetById(stormId);
    if (actor != 0) {
      if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        actor->scaleX = 0x800;
        actor->scaleY = 0x800;
      }
      sprite = actor->sprite;
      sprite->flags = 0;
    }
  }
  actor = Object_GetById(ACTOR_FOURTH_STORM);
  if (actor != 0) {
    struct AnimationEntry *part;

    sprite = actor->sprite;
    part = sprite->entries[0];
    if (part != 0) {
      part->param = 10;
    }
    sprite->dirty = 1;
    sprite->flags = 0;
  }
  if (Engine_GameFlagIsSet(FLAG_SECOND_SCENE_STORM_DOUSED + 4)) {
    Engine_GameFlagSet(FLAG_SUHALLA_DESERT_CHASING_STORM_DOUSED);
  }
  IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f42, 0x20000);
  if (Engine_GameFlagIsSet(FLAG_STORMS_RISEN)) {
    stormAlpha = 16;
    QueueSceneSound(SE_244);
  }
  IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - stormAlpha) << 8) | stormAlpha,
                      0x20000);
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

s32 gSuhallaDesertStormScript[] = {
    SC_CALL(SuhallaDesert_ClearActorSpriteAndCollisionFlags),
    SC_CALL(SuhallaDesert_ToggleActorActive),
    SC_END,
};

s32 gSuhallaDesertFourthStormScript[] = {
    SC_CALL(SuhallaDesert_ClearActorSpriteAndCollisionFlags),
    SC_CALL(SuhallaDesert_ToggleActorActive),
    SC_END,
};

struct SceneEntrance gSuhallaDesertEntrancesOther[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaDesertEntrances1[] = {
    {SUHALLA_DESERT_NORTH_FROM_WORLD_MAP, CONDITION_ALWAYS, 264, 0, 56,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_NORTH_FROM_SUHALLA_DESERT_SOUTH, CONDITION_ALWAYS, 24, 0,
     936, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaDesertEntrances2[] = {
    {SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_NORTH, CONDITION_ALWAYS, 488, 0,
     456, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_SOUTH_FROM_WORLD_MAP, CONDITION_ALWAYS, 488, 0, 1208,
     FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_EAST, CONDITION_ALWAYS, 440,
     0, 624, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_WEST, CONDITION_ALWAYS, 56,
     0, 624, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaDesertEntrances3[] = {
    {SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_WEST, CONDITION_ALWAYS, 72,
     0, 248, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_CAVES_WEST_FROM_EAST, CONDITION_ALWAYS, 440, 0, 152,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_CAVES_EAST_FROM_WEST, CONDITION_ALWAYS, 632, 0, 152,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_EAST, CONDITION_ALWAYS, 648,
     0, 280, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSuhallaDesertExits[] = {
    SCENE_EXITS(SuhallaDesertNorth),
    SCENE_EXIT(SUHALLA_DESERT_NORTH_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_SUHALLA_DESERT_NORTH),
    SCENE_EXIT(SUHALLA_DESERT_NORTH_TO_SUHALLA_DESERT_SOUTH, SuhallaDesertSouth,
               SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_NORTH),
    SCENE_EXITS(SuhallaDesertSouth),
    SCENE_EXIT(SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_NORTH, SuhallaDesertNorth,
               SUHALLA_DESERT_NORTH_FROM_SUHALLA_DESERT_SOUTH),
    SCENE_EXIT(SUHALLA_DESERT_SOUTH_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_SUHALLA_DESERT_SOUTH),
    SCENE_EXIT(SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_CAVES_EAST,
               SuhallaDesertCaves,
               SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_EAST),
    SCENE_EXIT(SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_CAVES_WEST,
               SuhallaDesertCaves,
               SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_WEST),
    SCENE_EXITS(SuhallaDesertCaves),
    SCENE_EXIT(SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_WEST,
               SuhallaDesertSouth,
               SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_WEST),
    SCENE_EXIT(SUHALLA_DESERT_CAVES_WEST_TO_EAST, SuhallaDesertCaves,
               SUHALLA_DESERT_CAVES_EAST_FROM_WEST),
    SCENE_EXIT(SUHALLA_DESERT_CAVES_EAST_TO_WEST, SuhallaDesertCaves,
               SUHALLA_DESERT_CAVES_WEST_FROM_EAST),
    SCENE_EXIT(SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_EAST,
               SuhallaDesertSouth,
               SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_EAST),
    SCENE_EXITS_END,
};

struct ScenePlacement gSuhallaDesertPlacementsOther[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gSuhallaDesertPlacements1[] = {
    { 0x121, FLAG_FIRST_SCENE_STORM_DOUSED, (s32)gSuhallaDesertStormScript,
        PIXELS(56), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x121, FLAG_FIRST_SCENE_STORM_DOUSED + 1, (s32)gSuhallaDesertStormScript,
        PIXELS(456), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x121, FLAG_FIRST_SCENE_STORM_DOUSED + 2, (s32)gSuhallaDesertStormScript,
        PIXELS(120), 0, PIXELS(712), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x121, FLAG_FIRST_SCENE_STORM_DOUSED + 3, (s32)gSuhallaDesertStormScript,
        PIXELS(264), 0, PIXELS(712), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x121, FLAG_FIRST_SCENE_STORM_DOUSED + 4, (s32)gSuhallaDesertStormScript,
        PIXELS(440), 0, PIXELS(744), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x39, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSuhallaDesertPlacements2[] = {
    { 0x121, FLAG_SECOND_SCENE_STORM_DOUSED, (s32)gSuhallaDesertStormScript,
        PIXELS(216), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x121, FLAG_SECOND_SCENE_STORM_DOUSED + 1, (s32)gSuhallaDesertStormScript,
        PIXELS(296), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x121, FLAG_SECOND_SCENE_STORM_DOUSED + 2, (s32)gSuhallaDesertStormScript,
        PIXELS(408), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x121, FLAG_SECOND_SCENE_STORM_DOUSED + 3, (s32)gSuhallaDesertFourthStormScript,
        PIXELS(376), 0, PIXELS(632), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x121, FLAG_SECOND_SCENE_STORM_DOUSED + 4, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x39, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x5c, 0x5d, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSuhallaDesertPlacements3[] = {
    { SCENE_TABLE_END },
};

s32 gSuhallaDesertChaseScript[] = {
    SC_CALL(SuhallaDesert_ChaseLeader),
    SC_END,
};

/// The first two scenes' events, in one table. Its exits keep their numbers,
/// since each scene's exit table gives them its own ways out: exit 1 is
/// SUHALLA_DESERT_NORTH_TO_WORLD_MAP in the first and
/// SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_NORTH in the second.
struct SceneEvent gSuhallaDesertEventsOther[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
#if !TBS_PATCH_1
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
#else
    { EVENT_EXIT | EVENT_TOUCH_FACING | FACING_NORTH, 4, CONDITION_ALWAYS, 4 },
#endif
    { EVENT_TOUCH, 9, FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA, EVENT_SCRIPT(Event_IodemAsksWhetherToPressOn) },
    { EVENT_TOUCH, 41, FLAG_FIRST_SCENE_STORM_DOUSED, EVENT_SCRIPT(Event_CaughtByFirstStorm) },
    { EVENT_TOUCH, 42, FLAG_FIRST_SCENE_STORM_DOUSED + 1, EVENT_SCRIPT(Event_CaughtBySecondStorm) },
    { EVENT_TOUCH, 43, FLAG_FIRST_SCENE_STORM_DOUSED + 2, EVENT_SCRIPT(Event_CaughtByThirdStorm) },
    { EVENT_TOUCH, 44, FLAG_FIRST_SCENE_STORM_DOUSED + 3, EVENT_SCRIPT(Event_CaughtByFourthStorm) },
    { EVENT_TOUCH, 45, FLAG_FIRST_SCENE_STORM_DOUSED + 4, EVENT_SCRIPT(Event_CaughtByFifthStorm) },
    { EVENT_TOUCH, 51, FLAG_SECOND_SCENE_STORM_DOUSED, EVENT_SCRIPT(Event_CaughtByFirstStorm) },
    { EVENT_TOUCH, 52, FLAG_SECOND_SCENE_STORM_DOUSED + 1, EVENT_SCRIPT(Event_CaughtBySecondStorm) },
    { EVENT_TOUCH, 53, FLAG_SECOND_SCENE_STORM_DOUSED + 2, EVENT_SCRIPT(Event_CaughtByThirdStorm) },
    { EVENT_TOUCH, 61, FLAG_SCENE_TEMP_06, EVENT_SCRIPT(Event_FirstStormRises) },
    { EVENT_TOUCH, 62, FLAG_SCENE_TEMP_07, EVENT_SCRIPT(Event_SecondStormRises) },
    { EVENT_TOUCH, 63, FLAG_SCENE_TEMP_08, EVENT_SCRIPT(Event_ThirdStormRises) },
    { EVENT_TOUCH, 64, FLAG_SCENE_TEMP_09, EVENT_SCRIPT(Event_FourthStormRises) },
    { EVENT_TOUCH, 65, FLAG_SCENE_TEMP_0A, EVENT_SCRIPT(Event_FifthStormRises) },
    { EVENT_TOUCH, 71, FLAG_SCENE_TEMP_06, EVENT_SCRIPT(Event_FirstStormRises) },
    { EVENT_TOUCH, 72, FLAG_SCENE_TEMP_07, EVENT_SCRIPT(Event_SecondStormRises) },
    { EVENT_TOUCH, 73, FLAG_SCENE_TEMP_08, EVENT_SCRIPT(Event_ThirdStormRises) },
    { EVENT_TOUCH, 54, FLAG_SECOND_SCENE_STORM_DOUSED + 3, EVENT_SCRIPT(Event_CaughtByFourthStorm) },
    { EVENT_TOUCH, 81, FLAG_SCENE_TEMP_0E, EVENT_SCRIPT(Event_StormGivesChase) },
    { EVENT_TALK, ACTOR_FLASH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_FightDjinni) },
    { SEARCH(SEARCH_UNNAMED), 100, FLAG_SUHALLA_DESERT_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { SEARCH(SEARCH_CHEST), 103, FLAG_SUHALLA_DESERT_VIRTUOUS_ARMLET, FIND_ITEM(ITEM_VIRTUOUS_ARMLET) },
    { SEARCH(SEARCH_CHEST), 104, FLAG_SUHALLA_DESERT_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_CHEST), 101, FLAG_SUHALLA_DESERT_COOKIE, FIND_ITEM(ITEM_COOKIE) },
    { SEARCH(SEARCH_CHEST), 102, FLAG_SUHALLA_DESERT_MIMIC_BEATEN, FIND_MIMIC | 6 },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_STORM, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_STORM + 1, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_STORM + 2, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_STORM + 3, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_STORM + 4, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseCastAtStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_STORM, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_STORM + 1, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_STORM + 2, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_STORM + 3, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_STORM + 4, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseStorm) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RevealStarts) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RevealEnds) },
    { EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StormCarriesPartyAway) },
    { EVENT_RAISED, 55, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CaughtByFifthStorm) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gSuhallaDesertEvents3[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS,
     SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_WEST},
    {EVENT_EXIT | EXIT_STYLE_ANIMATED, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS,
     SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_EAST},
    {SCENE_EVENTS_END},
};
