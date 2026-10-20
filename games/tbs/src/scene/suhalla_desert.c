/// @file
/// The Suhalla Desert in three scenes. Sandstorms rise as the party nears
/// them; one that catches the leader spins the party and, unless Douse stops
/// it and its monster is fought, carries the party off to the world map. In
/// the second scene one storm gives chase, and Reveal shows what the sand
/// hides. Travelling with the party, Iodem warns of the storms, and, once he
/// has heard the wounded soldiers in Suhalla, asks whether to search on for
/// Sheba or press on to Lalivero.

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

// FLAG_SCENE_TEMP_06 to _0A: storms 8 to 12 have risen during this visit.
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
  FLAG_STORMS_RISEN = 0x340
};

/// The actors this scene's code names, by their place in the placement tables.
enum {
  STORM_FIRST = 8, ///< the storms are actors 8 to 12
  STORM_FOURTH = 11,
  STORM_LAST = 12,
  STORM_CHASING = 12, ///< in the second scene the last storm gives chase
  IODEM = 13,
  REVEALED_OBJECT = 14, ///< in the second scene, seen with Reveal
  DJINNI = 15
};

/// Frames a caught party spins before the storm carries it away.
enum { STORM_CAPTURE_FRAMES = 180 };

/// Where a storm drops the party on the world map: the second scene's fourth
/// storm drops it elsewhere.
enum { WORLD_MAP_STORM_DROP = 27, WORLD_MAP_STORM_DROP_FOURTH = 77 };

static void SuhallaDesert_SetUpFirstSceneStorms(void);
static void SuhallaDesert_SetUpSecondSceneStorms(void);

static u16 sSuhallaDesertSavedColor;

#define SUHALLA_REVEAL_PALETTE_COLOR (*(volatile u16 *)0x0500019e)

#include "common/effect_create.h"
#include "common/effect_spawn.h"

void QueueSceneSound(s32 cue);

/// Talking to the Djinni starts its battle.
static void Event_FightDjinni(void) {
  Field_RequestDjinnBattle(DJINNI, DJINNI_FLASH);
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

  if (selector == (s32)&SceneId_SuhallaDesert1) {
    return gSuhallaDesertEntrances1;
  }
  if (selector == (s32)&SceneId_SuhallaDesert2) {
    return gSuhallaDesertEntrances2;
  }
  if (selector == (s32)&SceneId_SuhallaDesert3) {
    return gSuhallaDesertEntrances3;
  }
  return gSuhallaDesertEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

/// Where each exit of the three scenes leads.
const u32 *Scene_GetExits(void) { return gSuhallaDesertExits; }

/// The storms, Iodem, and in the second scene the Djinni and what Reveal shows.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_SuhallaDesert1) {
    return gSuhallaDesertPlacements1;
  }
  if (selector == (s32)&SceneId_SuhallaDesert2) {
    return gSuhallaDesertPlacements2;
  }
  if (selector == (s32)&SceneId_SuhallaDesert3) {
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
    Actor_SetAttachedEffect(selectedActorId, 0x101);
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
    gGameState.movementMode = 2;
    GameFlag_Set(FLAG_SCENE_TEMP_0F);
    FieldGameFlag_SetByte(FLAG_SCENE_TEMP_18, stormIdCopy);
    FieldGameFlag_SetByte(FLAG_SCENE_TEMP_10, STORM_CAPTURE_FRAMES);
    Engine_EventEnd();
    gEventWork->encounterEnemyGroup = 0; // drop any battle this step rolled
  }
}

/// The leader walks into the first storm of the scene.
static void Event_CaughtByFirstStorm(void) {
  SuhallaDesert_CatchInStorm(STORM_FIRST);
}

/// The leader walks into the second storm.
static void Event_CaughtBySecondStorm(void) {
  SuhallaDesert_CatchInStorm(STORM_FIRST + 1);
}

/// The leader walks into the third storm.
static void Event_CaughtByThirdStorm(void) {
  SuhallaDesert_CatchInStorm(STORM_FIRST + 2);
}

/// The leader walks into the fourth storm.
static void Event_CaughtByFourthStorm(void) {
  SuhallaDesert_CatchInStorm(STORM_FIRST + 3);
}

/// The leader walks into the first scene's fifth storm, or the chasing storm
/// reaches the moving leader.
static void Event_CaughtByFifthStorm(void) {
  SuhallaDesert_CatchInStorm(STORM_FIRST + 4);
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
  Engine_GameFlagSet((FLAG_SCENE_TEMP_06 - STORM_FIRST + stormIdCopy));
  Engine_GameFlagSet(FLAG_STORMS_RISEN);
  if (Engine_GameFlagIsSet(FLAG_IODEM_FOLLOWING_PARTY) != 0 &&
      GameFlag_IsSet(FLAG_SUHALLA_DESERT_IODEM_WARNED) == 0) {
    Engine_GameFlagSet(FLAG_SUHALLA_DESERT_IODEM_WARNED);
    Actor_SetSpeed(IODEM, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if ((s32)leader != 0) {
      Engine_ActorSetPosition(IODEM, leader->x.fixed, leader->z.fixed);
    }
    Actor_FaceActor(IODEM, FACING_SOUTH, 0);
    Object_GetById(IODEM)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Engine_ActorWalkTo(IODEM, leaderX, (leaderZ - 16));
    Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, (leaderX + 8),
                              (leaderZ - 40));
    Engine_EventWait(1);
    Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
    Engine_ActorSetAnimation(IODEM, 1);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Engine_EventSetMessage(MsgSuhallaSandstorm);
    Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 6, 60);
    Engine_EventShowMessage(IODEM, 0);
    Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 2, 60);
    Engine_EventShowMessage(IODEM, 0);
    Actor_FaceDirection(IODEM, FACING_SOUTHEAST, 0);
    Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 1, 60);
    Engine_EventShowMessage(IODEM, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(IODEM, FACING_NORTH, 30);
    Engine_ActorRunRepeatedMotion(IODEM, 2);
    Engine_EventShowMessage(IODEM, 0);
    Engine_ActorSetAnimationAndWait(IODEM, 3);
    Engine_EventShowMessage(IODEM, 0);
    Actor_SetSpeed(IODEM, 0x10000, 0x8000);
    Engine_ActorSetAnimation(IODEM, 2);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if ((s32)leader != 0) {
      Engine_ActorSetDestination(IODEM, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(IODEM);
    Engine_ActorSetPosition(IODEM, 0, 0);
  }
  Engine_EventEnd();
}

/// The party nears the first storm.
static void Event_FirstStormRises(void) {
  SuhallaDesert_RaiseStorm(STORM_FIRST);
}

/// The party nears the second storm.
static void Event_SecondStormRises(void) {
  SuhallaDesert_RaiseStorm(STORM_FIRST + 1);
}

/// The party nears the third storm.
static void Event_ThirdStormRises(void) {
  SuhallaDesert_RaiseStorm(STORM_FIRST + 2);
}

/// The party nears the fourth storm.
static void Event_FourthStormRises(void) {
  SuhallaDesert_RaiseStorm(STORM_FIRST + 3);
}

/// The party nears the fifth storm.
static void Event_FifthStormRises(void) {
  SuhallaDesert_RaiseStorm(STORM_FIRST + 4);
}

/// Near the second scene's way out to the world map, once Iodem has heard the
/// wounded soldiers: he steps out of the party. "Sheba is nowhere to be found.
/// Should we search the desert once more or press on to Lalivero?" Whatever the
/// answer, Iodem decides to go on to Tolbi's guard gate ahead and leave the
/// search to its soldiers, and rejoins the party.
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
      Actor_SetPosition(IODEM, leader->x.fixed, leader->z.fixed);
    }
    Actor_FaceActor(IODEM, FACING_NORTH, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 440, 1256);
    Actor_FaceDirection(IODEM, FACING_SOUTH, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 444, 1240);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 30);
    Engine_ActorSetAnimationAndWait(IODEM, 4);
    Event_ShowMessage(IODEM, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 60);
    Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 5, 60);
    Event_ShowMessage(IODEM, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(IODEM, 2);
    Event_ShowMessage(IODEM, 0);
    Actor_FaceDirection(IODEM, FACING_NORTH, 30);
    Event_AskYesNo(IODEM, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 6, 60);
    Event_ShowMessage(IODEM, 0);
    Engine_ActorSetAnimationAndWait(IODEM, 3);
    Event_ShowMessage(IODEM, 0);
    Actor_SetSpeed(IODEM, 0xb333, 0x5999);
    Actor_WalkToAndWait(IODEM, 440, 1256);
    Event_ShowMessage(IODEM, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(IODEM, 2);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetDestination(IODEM, leader->x.part.pixel, leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(IODEM);
    Actor_SetPosition(IODEM, 0, 0);
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
  // Flag 0x104 stops the chase.
  if (Engine_GameFlagIsSet(0x104)) {
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

/// In the second scene, until the chasing storm has been doused: it appears
/// behind the party and gives chase.
static void Event_StormGivesChase(void) {
  if (GameFlag_IsSet(FLAG_SUHALLA_DESERT_CHASING_STORM_DOUSED) == 0) {
    GameFlag_Set(FLAG_SCENE_TEMP_0E);
    Actor_SetPosition(STORM_CHASING, PIXELS(120), PIXELS(824));
    Actor_SetActionScript(STORM_CHASING, gSuhallaDesertChaseScript);
  }
}

/// Douse cast at the storm that caught the leader: sets the Psynergy effect's
/// child option (its meaning unknown).
static void Event_DouseCastAtStorm(void) {
  u8 *state = gEffectWork;
  ((struct BattleFxScene *)state)->childOption = 1;
}

/// Douse stops the storm that caught the leader: it is doused for good, and
/// the monster in it attacks. The second scene's fourth and chasing storms
/// hold other monsters.
static void Event_DouseStorm(s32 unused, s32 stormId) {
  struct GameState *state;

  // The storm no longer carries the party away.
  if (gEventWork->raisedTrigger == 99) {
    gEventWork->raisedTrigger = 0;
  }
  Engine_GameFlagClear(FLAG_SCENE_TEMP_0F);
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert1) {
    Engine_GameFlagSet((stormId + FLAG_FIRST_SCENE_STORM_DOUSED - STORM_FIRST));
  } else {
    if (gGameState.scene == (s32)&SceneId_SuhallaDesert2) {
      Engine_GameFlagSet(
          (stormId + FLAG_SECOND_SCENE_STORM_DOUSED - STORM_FIRST));
    }
  }
  FieldGameFlag_SetByte(FLAG_SCENE_TEMP_10, 0);
  Field_RequestBattleFromEncounterTable(98, 5);
  Battle_SetStart(3);
  state = &gGameState;
  if (state->scene == (s32)&SceneId_SuhallaDesert2) {
    if (stormId == STORM_FOURTH) {
      Field_RequestBattleFromEncounterTable(98, 7);
    } else {
      if (stormId == STORM_CHASING) {
        Field_RequestBattleFromEncounterTable(98, 6);
        Engine_ActorStop(STORM_CHASING);
        Engine_ActorSetPosition(STORM_CHASING, 0, 0);
      }
    }
  }
  Object_GetById(gGameState.selectedActor)->motionFlags = 3;
}

/// The spinning runs out: the storm rises into the sky with the party, the
/// screen closes, and the party lands on the world map.
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
  selectedActor->unk61 = 1;
  storm->unk61 = 1;
  // Both rise faster and faster for a second.
  for (frame = 0; frame < 60; frame++) {
    selectedActor->velocityY += 0x3333;
    storm->velocityY += 0x3333;
    Engine_TaskWait(1);
  }
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
  Engine_GameFlagSet(0x122);
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert2 &&
      FieldGameFlag_GetByte(FLAG_SCENE_TEMP_18) == STORM_FOURTH) {
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

/// Reveal ends, or a scene is entered: in the second scene, what Reveal showed
/// is hidden again and the pulsing color is put back.
// Native event return forms: see MATCHING.md.
#ifdef UBFIX
static void Event_RevealEnds(void) {
#else
static s32 Event_RevealEnds(void) {
  s32 removalResult;
#endif
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert2) {
    Object_GetById(REVEALED_OBJECT)->priorityFlags = 2;
    Object_GetById(REVEALED_OBJECT)->motionFlags = 3;
    Actor_SetPosition(REVEALED_OBJECT, 0, 0);
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

/// Reveal in the second scene: the hidden object appears at 248, 712, map
/// cells and the map object at 100 change, and a palette color pulses.
#ifdef UBFIX
static void Event_RevealStarts(void) {
#else
static s32 Event_RevealStarts(void) {
#endif
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert2) {
    Object_GetById(REVEALED_OBJECT)->priorityFlags = 2;
    Object_GetById(REVEALED_OBJECT)->motionFlags = 0;
    Actor_SetPosition(REVEALED_OBJECT, PIXELS(248), PIXELS(712));
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
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert3) {
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
      SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  if (FieldGameFlag_GetByte(FLAG_SCENE_TEMP_10) != 0) {
    gGameState.movementMode = 2;
    FieldScheduler_AddCallback(SuhallaDesert_UpdateCaptureTimer,
                               TASK_PRIORITY_SCENE);
  }
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert1 ||
      gGameState.scene == (s32)&SceneId_SuhallaDesert2) {
    sSuhallaDesertSavedColor = SUHALLA_REVEAL_PALETTE_COLOR;
    Event_RevealEnds();
  }
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert1) {
    SuhallaDesert_SetUpFirstSceneStorms();
  } else if (gGameState.scene == (s32)&SceneId_SuhallaDesert2) {
    SuhallaDesert_SetUpSecondSceneStorms();
  } else {
    Engine_AudioPlayCue(SE_288);
  }
  if (gGameState.entrance == 0) {
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
  for (stormId = STORM_FIRST; stormId <= STORM_LAST; stormId++) {
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
  for (stormId = STORM_FIRST; stormId < STORM_FOURTH; stormId++) {
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
  actor = Object_GetById(STORM_FOURTH);
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
    {0, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaDesertEntrances1[] = {
    {1, CONDITION_ALWAYS, 264, 0, 56, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 24, 0, 936, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaDesertEntrances2[] = {
    {1, CONDITION_ALWAYS, 488, 0, 456, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 488, 0, 1208, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 440, 0, 624, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 56, 0, 624, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaDesertEntrances3[] = {
    {1, CONDITION_ALWAYS, 72, 0, 248, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 440, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 632, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 648, 0, 280, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSuhallaDesertExits[] = {
    SCENE_EXITS(164),      SCENE_EXIT(1, 2, 28),  SCENE_EXIT(2, 165, 1),
    SCENE_EXIT(0, 165, 0), SCENE_EXIT(1, 164, 2), SCENE_EXIT(2, 2, 55),
    SCENE_EXIT(3, 166, 4), SCENE_EXIT(4, 166, 1), SCENE_EXIT(0, 166, 0),
    SCENE_EXIT(1, 165, 4), SCENE_EXIT(2, 166, 3), SCENE_EXIT(3, 166, 2),
    SCENE_EXIT(4, 165, 3), SCENE_EXITS_END,
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

struct SceneEvent gSuhallaDesertEventsOther[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
#if !TBS_PATCH_1
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
#else
    { 0xc401, 4, CONDITION_ALWAYS, 4 },
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
    { EVENT_TALK, DJINNI, CONDITION_ALWAYS, EVENT_SCRIPT(Event_FightDjinni) },
    { SEARCH(SEARCH_UNNAMED), 100, 0x351, FIND_PSYNERGY_STONE },
    { SEARCH(SEARCH_CHEST), 103, 0xfb1, FIND_ITEM(ITEM_VIRTUOUS_ARMLET) },
    { SEARCH(SEARCH_CHEST), 104, 0xfb2, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_CHEST), 101, 0xfb3, FIND_ITEM(ITEM_COOKIE) },
    { SEARCH(SEARCH_CHEST), 102, 0xef6, 0x500006 },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, 1), 8, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, 1), 9, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, 1), 10, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, 1), 11, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, 1), 12, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseCastAtStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 8, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 9, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 10, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 11, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 12, IF_FLAG_SET(FLAG_SCENE_TEMP_0F), EVENT_SCRIPT(Event_DouseStorm) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RevealStarts) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RevealEnds) },
    { EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StormCarriesPartyAway) },
    { EVENT_RAISED, 55, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CaughtByFifthStorm) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gSuhallaDesertEvents3[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {0x31, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {SCENE_EVENTS_END},
};
