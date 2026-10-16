#include "script.h"
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

// Forward declarations.
static void SuhallaDesert_ApplyVisitFlagBlend(void);
static void SuhallaDesert_ApplyAltarFlagBlend(void);

struct SelectedActorId {
  u8 reserved000[500];
  s32 actorId;
};

struct SelectedActorFacing {
  u8 reserved00[6];
  u16 presentation;
};

struct StateMarkerWork {
  u8 reserved000[386];
  u16 stateOneMarker;
};

static u16 sSuhallaDesertSavedColor;

#define SUHALLA_SCENE_COMPANION 13

#define SUHALLA_REVEAL_PALETTE_COLOR (*(volatile u16 *)0x0500019e)

// Shared source is included here in ROM order.
#include "common/effect_create.h"
#include "common/effect_spawn.h"

struct FieldGlobals {
  u8 *map;
  u8 unk04[0x48];
  struct EventWork *event;
};

void QueueSceneSound(s32 cue);

// Starts the Djinn battle associated with actor 15.
static void SuhallaDesert_StartDjinnBattle(void) {
  BattleFx_SetPhaseRequest(15, 45);
}

// Clears this scripted actor's sprite and collision flags; returns the original
// zero status.
static s32 SuhallaDesert_ClearActorSpriteAndCollisionFlags(
    struct ScriptInterpreter *interpreter) {
  u8 *actorBytes = (u8 *)interpreter;
  Engine_ActorSetSpriteFlags(actorBytes, 0);
  ((struct FieldActor *)actorBytes)->collisionFlags = 0;
  return 0;
}

// Toggles the scripted actor's active bit and reports completion.
static s32
SuhallaDesert_ToggleActorActive(struct ScriptInterpreter *interpreter) {
  u8 *actorBytes = (u8 *)interpreter;
  ((struct FieldActor *)actorBytes)->active ^= 1;
  return 1;
}

// Selects the entrance table for the active desert room.
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

// Provides the original zero-result scene entry; its broader meaning is
// unknown.
s32 SceneData_ReturnZero(void) { return 0; }

// Returns the exit table shared by the desert rooms.
u8 *SuhallaDesert_GetExits(void) { return gSuhallaDesertExits; }

// Selects the storm and actor placements for the active desert room.
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

// Rotates the captured actor and counts down until the sandstorm raises
// event 99.
static void SuhallaDesert_UpdateCaptureTimer(void) {
  struct SelectedActorFacing *selectedActor;
  struct StateMarkerWork *requests;
  s32 remainingFrames;

  selectedActor =
      Object_GetById(((struct SelectedActorId *)((s16 *)&gGameState))->actorId);
  requests = *(struct StateMarkerWork **)((u8 *)&gEventWork);
  selectedActor->presentation = (gFrameCount << 12);

  remainingFrames = FieldGameFlag_GetByte(0x210);
  if (remainingFrames != 0) {
    if (remainingFrames == 1) {
      requests->stateOneMarker = 99;
    } else if (Engine_GameFlagIsSet(0x106) == 0) {
      remainingFrames -= 1;
    }
  }
  FieldGameFlag_SetByte(0x210, remainingFrames);
}

// Pulls the selected actor into a sandstorm and starts its capture timer.
static void SuhallaDesert_BeginSandstormCapture(s32 stormId) {
  s32 stormIdCopy;
  s32 captureActive;
  struct FieldActor *selectedActor;
  struct FieldActor *storm;
  u8 *selectedActorBits;
  u8 *gameBytes;

  // The event-selected actor is pulled toward the storm before being lifted.
  gameBytes = (u8 *)((s16 *)&gGameState);
  selectedActorBits = *(u8 **)(gameBytes + 500);
  stormIdCopy = stormId;
  selectedActor = Object_GetById((s32)selectedActorBits);
  Actor_Get(stormIdCopy);
  captureActive = GameFlag_IsSet(0x20f);
  if (captureActive == 0) {
    Engine_EventBegin();
    Actor_SetAttachedEffect((s32)selectedActorBits, 0x101);
    Engine_ActorSetAnimation((s32)selectedActorBits, 9);
    storm = Object_GetById(stormIdCopy);
    if (storm != 0) {
      Actor_SetDestination((s32)selectedActorBits, storm->x.part.pixel,
                           storm->z.part.pixel);
    }
    Engine_ActorWaitForMove((s32)selectedActorBits);
    // Start the capture countdown once the approach finishes.
    Event_PlayCue(244);
    FieldScheduler_AddCallback(SuhallaDesert_UpdateCaptureTimer,
                               TASK_PRIORITY_SCENE);
    selectedActor->motionFlags = captureActive;
    Engine_ObjectSetPosition((s32)selectedActor, selectedActor->x.fixed,
                             selectedActor->y.fixed + PIXELS(32),
                             selectedActor->z.fixed);
    Engine_ActorWaitForMove((s32)selectedActorBits);
    selectedActor->velocityY = captureActive;
    selectedActor->motionFlags = 4;
    *(gameBytes + 498) = 2;
    GameFlag_Set(0x20f);
    FieldGameFlag_SetByte(0x218, stormIdCopy);
    FieldGameFlag_SetByte(0x210, 180);
    Engine_EventEnd();
    gEventWork->encounterEnemyGroup = 0; // drop any battle this step rolled
  }
}

// Starts capture by the first sandstorm in either room's placement table.
static void SuhallaDesert_EnterFirstSandstorm(void) {
  SuhallaDesert_BeginSandstormCapture(8);
}

// Starts capture by the second sandstorm in either room's placement table.
static void SuhallaDesert_EnterSecondSandstorm(void) {
  SuhallaDesert_BeginSandstormCapture(9);
}

// Starts capture by the third sandstorm in either room's placement table.
static void SuhallaDesert_EnterThirdSandstorm(void) {
  SuhallaDesert_BeginSandstormCapture(10);
}

// Starts capture by the fourth sandstorm in the placement table.
static void SuhallaDesert_EnterFourthSandstorm(void) {
  SuhallaDesert_BeginSandstormCapture(11);
}

// Starts capture by the fifth sandstorm, also used by the following storm.
static void SuhallaDesert_EnterFifthSandstorm(void) {
  SuhallaDesert_BeginSandstormCapture(12);
}

// Restores a sandstorm's scale and alpha, then gives the first sandstorm
// warning when needed.
static void SuhallaDesert_RestoreSandstormAppearance(s32 stormId) {
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
  appearanceRestored = Engine_GameFlagIsSet(0x340);
  leaderX = selectedActor->x.part.pixel;
  leaderZ = selectedActor->z.part.pixel;
  Engine_EventBegin();
  Engine_AudioPlayCue(244);
  // Restore scale and, on the first restoration, fade in the storm layer.
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
  Engine_GameFlagSet((0x1fe + stormIdCopy));
  Engine_GameFlagSet(0x340);
  // The companion warns about the sandstorms once, when travelling with us.
  if (Engine_GameFlagIsSet(0x9a0) != 0 && GameFlag_IsSet(0x9b6) == 0) {
    Engine_GameFlagSet(0x9b6);
    Actor_SetSpeed(13, 0x10000, 0x8000);
    Actor_SetSpeed(0, 0x18000, 0xc000);
    leader = Object_GetById(0);
    if ((s32)leader != 0) {
      Engine_ActorSetPosition(13, leader->x.fixed, leader->z.fixed);
    }
    Actor_FaceActor(13, FACING_SOUTH, 0);
    Object_GetById(13)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Engine_ActorWalkTo(13, leaderX, (leaderZ - 16));
    Object_GetById(0)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Engine_ActorWalkToAndWait(0, (leaderX + 8), (leaderZ - 40));
    Engine_EventWait(1);
    Object_GetById(0)->steeringFlags |= ACTOR_FACES_MOTION;
    Engine_ActorSetAnimation(13, 1);
    Engine_ActorFaceDirection(0, FACING_SOUTH, 0);
    Engine_EventSetMessage(MsgSuhallaSandstorm);
    Actor_ShowEmote(13, EMOTE_IN_FRONT | 6, 60);
    Engine_EventShowMessage(13, 0);
    Actor_ShowEmote(13, EMOTE_IN_FRONT | 2, 60);
    Engine_EventShowMessage(13, 0);
    Actor_FaceDirection(13, FACING_SOUTHEAST, 0);
    Actor_ShowEmote(13, EMOTE_IN_FRONT | 1, 60);
    Engine_EventShowMessage(13, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(13, FACING_NORTH, 30);
    Engine_ActorRunRepeatedMotion(13, 2);
    Engine_EventShowMessage(13, 0);
    Engine_ActorSetAnimationAndWait(13, 3);
    Engine_EventShowMessage(13, 0);
    Actor_SetSpeed(13, 0x10000, 0x8000);
    Engine_ActorSetAnimation(13, 2);
    leader = Object_GetById(0);
    if ((s32)leader != 0) {
      Engine_ActorSetDestination(13, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(13);
    Engine_ActorSetPosition(13, 0, 0);
  }
  Engine_EventEnd();
}

// Restores the first sandstorm after its remembered encounter flag is set.
static void SuhallaDesert_RestoreFirstSandstorm(void) {
  SuhallaDesert_RestoreSandstormAppearance(8);
}

// Restores the second sandstorm after its remembered encounter flag is set.
static void SuhallaDesert_RestoreSecondSandstorm(void) {
  SuhallaDesert_RestoreSandstormAppearance(9);
}

// Restores the third sandstorm after its remembered encounter flag is set.
static void SuhallaDesert_RestoreThirdSandstorm(void) {
  SuhallaDesert_RestoreSandstormAppearance(10);
}

// Restores the fourth sandstorm after its remembered encounter flag is set.
static void SuhallaDesert_RestoreFourthSandstorm(void) {
  SuhallaDesert_RestoreSandstormAppearance(11);
}

// Restores the fifth sandstorm after its remembered encounter flag is set.
static void SuhallaDesert_RestoreFifthSandstorm(void) {
  SuhallaDesert_RestoreSandstormAppearance(12);
}

// Discusses the missing captive and whether to search again or continue to
// Lalivero.
static void SuhallaDesert_DiscussContinuingToLalivero(void) {
  struct FieldActor *leader;

  // Only show this conversation when the companion and story flags allow it.
  if (GameFlag_IsSet(0x9a0) != 0 && GameFlag_IsSet(0x1b7) == 0 &&
      GameFlag_IsSet(0x9b0) != 0) {
    GameFlag_Set(0x9b5);
    Engine_EventBegin();
    Engine_EventSetMessage(MsgSuhallaNowhereFound);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetPosition(SUHALLA_SCENE_COMPANION, leader->x.fixed,
                        leader->z.fixed);
    }
    Actor_FaceActor(SUHALLA_SCENE_COMPANION, FACING_NORTH, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 440, 1256);
    Actor_FaceDirection(SUHALLA_SCENE_COMPANION, FACING_SOUTH, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 444, 1240);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 30);
    Engine_ActorSetAnimationAndWait(SUHALLA_SCENE_COMPANION, 4);
    Event_ShowMessage(SUHALLA_SCENE_COMPANION, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 60);
    Actor_ShowEmote(SUHALLA_SCENE_COMPANION, EMOTE_IN_FRONT | 5, 60);
    Event_ShowMessage(SUHALLA_SCENE_COMPANION, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(SUHALLA_SCENE_COMPANION, 2);
    Event_ShowMessage(SUHALLA_SCENE_COMPANION, 0);
    Actor_FaceDirection(SUHALLA_SCENE_COMPANION, FACING_NORTH, 30);
    Event_AskYesNo(SUHALLA_SCENE_COMPANION, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(SUHALLA_SCENE_COMPANION, EMOTE_IN_FRONT | 6, 60);
    Event_ShowMessage(SUHALLA_SCENE_COMPANION, 0);
    Engine_ActorSetAnimationAndWait(SUHALLA_SCENE_COMPANION, 3);
    Event_ShowMessage(SUHALLA_SCENE_COMPANION, 0);
    Actor_SetSpeed(SUHALLA_SCENE_COMPANION, 0xb333, 0x5999);
    Actor_WalkToAndWait(SUHALLA_SCENE_COMPANION, 440, 1256);
    Event_ShowMessage(SUHALLA_SCENE_COMPANION, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(SUHALLA_SCENE_COMPANION, 2);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetDestination(SUHALLA_SCENE_COMPANION, leader->x.part.pixel,
                           leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(SUHALLA_SCENE_COMPANION);
    Actor_SetPosition(SUHALLA_SCENE_COMPANION, 0, 0);
    Engine_EventEnd();
  }
}

// Follows the selected actor, raising a contact event nearby and emitting
// sparks every eight frames.
static s32
SuhallaDesert_FollowLeaderWithSparks(struct ScriptInterpreter *interpreter) {
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
  actor->active ^= 1;
  // Chase the selected actor unless the current state suppresses pursuit.
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
  // Keep the original eight-frame emission and single random spin sample.
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

// Places the fifth storm and starts its following script when its encounter
// remains available.
static void SuhallaDesert_StartFollowingSandstorm(void) {
  if (GameFlag_IsSet(0x9b7) == 0) {
    GameFlag_Set(0x20e);
    Actor_SetPosition(12, PIXELS(120), PIXELS(824));
    Actor_SetActionScript(12, gSuhallaDesertActor12Action);
  }
}

// Enables the effect scene's child option; its remaining meaning is unknown.
static void SuhallaDesert_EnableEffectChildOption(void) {
  u8 *state = gEffectWork;
  ((struct BattleFxScene *)state)->childOption = 1;
}

// Ends sandstorm capture after Douse and selects the corresponding battle.
static void SuhallaDesert_DouseSandstormAndStartBattle(s32 unused,
                                                       s32 stormId) {
  struct GameState *state;

  // Cancel the pending ride before the Douse encounter begins.
  if (gEventWork->raisedTrigger == 99) {
    gEventWork->raisedTrigger = 0;
  }
  Engine_GameFlagClear(0x20f);
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert1) {
    Engine_GameFlagSet((stormId + 0x2f9));
  } else {
    if (gGameState.scene == (s32)&SceneId_SuhallaDesert2) {
      Engine_GameFlagSet((stormId + 0x309));
    }
  }
  FieldGameFlag_SetByte(0x210, 0);
  // Most storms use the same encounter; two second-room storms override it.
  BattleFx_SetWeightedResult(98, 5);
  Battle_SetStart(3);
  state = &gGameState;
  if (state->scene == (s32)&SceneId_SuhallaDesert2) {
    if (stormId == 11) {
      BattleFx_SetWeightedResult(98, 7);
    } else {
      if (stormId == 12) {
        BattleFx_SetWeightedResult(98, 6);
        Engine_ActorStop(12);
        Engine_ActorSetPosition(12, 0, 0);
      }
    }
  }
  Object_GetById(gGameState.selectedActor)->motionFlags = 3;
}

// Carries the selected actor upward with its storm, then returns to the world
// map.
static void SuhallaDesert_RideSandstormToWorldMap(void) {
  s32 stormId;
  struct FieldActor *selectedActor;
  struct FieldActor *storm;
  s32 frame;

  stormId = FieldGameFlag_GetByte(0x218);
  selectedActor = Object_GetById(gGameState.selectedActor);
  storm = Object_GetById(stormId);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_AudioPlayCue(219);
  Engine_ActorSetSpriteFlags((struct FieldActor *)gGameState.selectedActor, 0);
  storm->motionFlags = 0;
  selectedActor->motionFlags = 0;
  selectedActor->velocityY = 0;
  selectedActor->unk5d[4] = 1;
  storm->unk5d[4] = 1;
  // Accelerate both actors together for sixty frames before closing the screen.
  for (frame = 0; frame < 60; frame++) {
    selectedActor->velocityY += 0x3333;
    storm->velocityY += 0x3333;
    Engine_TaskWait(1);
  }
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
  Engine_GameFlagSet(0x122);
  // The fourth storm in the second room reaches a different world-map entrance.
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert2 &&
      FieldGameFlag_GetByte(0x218) == 11) {
    Event_SetDestination((s32)&SceneId_WorldMap, 77);
  } else {
    Event_SetDestination((s32)&SceneId_WorldMap, 27);
  }
}

// Pulses the Reveal palette color with a triangular frame-based brightness
// ramp.
static void SuhallaDesert_PulseRevealPalette(void) {
  u16 brightnessFrame = gFrameCount & 63;
  s32 color;

  if (brightnessFrame > 31)
    brightnessFrame = 64 - brightnessFrame;
  color = (brightnessFrame >> 1) + 7;
  color |= (color << 10) | (color << 5);
  SUHALLA_REVEAL_PALETTE_COLOR = ((u32)color << 16) >> 16;
}

// Hides the Reveal objects in the second room and restores the saved palette
// color.
static s32 SuhallaDesert_HideRevealObjects(void) {
  s32 removalResult;

  if (gGameState.scene == (s32)&SceneId_SuhallaDesert2) {
    Object_GetById(14)->priorityFlags = 2;
    Object_GetById(14)->motionFlags = 3;
    Actor_SetPosition(14, 0, 0);
    Map_CopyCellAttributes(16, 44, 1, 1, 15, 44);
    MapObject_SetPosition(100, 0, 0);
    Map_CopyCellAttributes(12, 71, 1, 1, 127, 127);
    Map_CopyCellAttributes(11, 71, 1, 1, 12, 71);
    removalResult = Engine_TaskRemoveCallback(SuhallaDesert_PulseRevealPalette);
    Io_Write16(sSuhallaDesertSavedColor, &SUHALLA_REVEAL_PALETTE_COLOR);
    return removalResult;
  }
}

// Shows the hidden actor and stone in the second room and starts their palette
// pulse.
static s32 SuhallaDesert_ShowRevealObjects(void) {
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert2) {
    Object_GetById(14)->priorityFlags = 2;
    Object_GetById(14)->motionFlags = 0;
    Actor_SetPosition(14, PIXELS(248), PIXELS(712));
    Map_CopyCellAttributes(31, 95, 1, 1, 15, 44);
    MapObject_SetPosition(100, -1, -1);
    Marker_MoveLeaderAway();
    Map_CopyCellAttributes(127, 127, 1, 1, 12, 71);
    return FieldScheduler_AddCallback(SuhallaDesert_PulseRevealPalette,
                                      TASK_PRIORITY_SCENE);
  }
}

// Uses the third room's exits or the shared sandstorm event table.
const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert3) {
    return gSuhallaDesertEvents3;
  }
  return gSuhallaDesertEventsOther;
}

// Restores capture, palette and storm appearance when entering a desert room.
s32 SuhallaDesert_RunSceneScript(void) {
  u8 *map;

  // Resume capture across room entry, and save the color Reveal will animate.
  map = (*(struct FieldGlobals *)gMapWork).map;
  (*(struct FieldGlobals *)gMapWork).event->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  if (FieldGameFlag_GetByte(0x210) != 0) {
    gGameState.movementMode = 2;
    FieldScheduler_AddCallback(SuhallaDesert_UpdateCaptureTimer,
                               TASK_PRIORITY_SCENE);
  }
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert1 ||
      gGameState.scene == (s32)&SceneId_SuhallaDesert2) {
    sSuhallaDesertSavedColor = SUHALLA_REVEAL_PALETTE_COLOR;
    SuhallaDesert_HideRevealObjects();
  }
  // Restore each room's remembered storms and initial blend state.
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert1) {
    SuhallaDesert_ApplyVisitFlagBlend();
  } else if (gGameState.scene == (s32)&SceneId_SuhallaDesert2) {
    SuhallaDesert_ApplyAltarFlagBlend();
  } else {
    Engine_AudioPlayCue(0x120);
  }
  if (gGameState.entrance == 0) {
    *(u16 *)(map + 20) &= ~0x200;
  }
  return 0;
}

// Restores first-room storm flags, scales and alpha blending on entry.
// Trial: sharing the blend tail through an ordinary inline shrinks this
// function from 0x144 to 0x138 bytes in tbs-en; retain the original body.
static void SuhallaDesert_ApplyVisitFlagBlend(void) {
  struct FieldActor *actor;
  s32 stormId;
  s32 stormAlpha = 0;

  if (Engine_GameFlagIsSet(0x301)) {
    Engine_GameFlagSet(0x206);
  }
  if (Engine_GameFlagIsSet(0x302)) {
    Engine_GameFlagSet(0x207);
  }
  if (Engine_GameFlagIsSet(0x303)) {
    Engine_GameFlagSet(0x208);
  }
  if (Engine_GameFlagIsSet(0x304)) {
    Engine_GameFlagSet(0x209);
  }
  if (Engine_GameFlagIsSet(0x305)) {
    Engine_GameFlagSet(0x20a);
  }
  for (stormId = 8; stormId <= 12; stormId++) {
    actor = Object_GetById(stormId);
    if (actor != 0) {
      if (!Engine_GameFlagIsSet(0x109)) {
        actor->scaleX = 0x800;
        actor->scaleY = 0x800;
      }
      actor->sprite->flags = 0;
    }
  }
  IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f42, 0x20000);
  if (Engine_GameFlagIsSet(0x340)) {
    stormAlpha = 16;
    QueueSceneSound(244);
  }
  IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - stormAlpha) << 8) | stormAlpha,
                      0x20000);
}

// Restores second-room storm flags, sprite settings and alpha blending on
// entry.
// Trial: the same blend-tail inline shrinks this function from 0x158 to
// 0x14c bytes in tbs-en; retain the original body.
static void SuhallaDesert_ApplyAltarFlagBlend(void) {
  struct FieldActor *actor;
  struct FieldSprite *sprite;
  s32 stormId;
  s32 stormAlpha = 0;

  if (Engine_GameFlagIsSet(0x311)) {
    Engine_GameFlagSet(0x206);
  }
  if (Engine_GameFlagIsSet(0x312)) {
    Engine_GameFlagSet(0x207);
  }
  if (Engine_GameFlagIsSet(0x313)) {
    Engine_GameFlagSet(0x208);
  }
  for (stormId = 8; stormId <= 10; stormId++) {
    actor = Object_GetById(stormId);
    if (actor != 0) {
      if (!Engine_GameFlagIsSet(0x109)) {
        actor->scaleX = 0x800;
        actor->scaleY = 0x800;
      }
      sprite = actor->sprite;
      sprite->flags = 0;
    }
  }
  actor = Object_GetById(11);
  if (actor != 0) {
    u8 *part;

    sprite = actor->sprite;
    part = *(u8 **)((u8 *)sprite + 40);
    if (part != 0) {
      part[5] = 10;
    }
    sprite->unk20[5] = 1;
    sprite->flags = 0;
  }
  if (Engine_GameFlagIsSet(0x315)) {
    Engine_GameFlagSet(0x9b7);
  }
  IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f42, 0x20000);
  if (Engine_GameFlagIsSet(0x340)) {
    stormAlpha = 16;
    QueueSceneSound(244);
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

s32 gSuhallaDesertPlacements1_Data[] = {
    SC_CALL(SuhallaDesert_ClearActorSpriteAndCollisionFlags),
    SC_CALL(SuhallaDesert_ToggleActorActive),
    SC_END,
};

s32 gSuhallaDesertPlacements2_1[] = {
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
    { 0x121, 0x301, (s32)gSuhallaDesertPlacements1_Data,
        PIXELS(56), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x121, 0x302, (s32)gSuhallaDesertPlacements1_Data,
        PIXELS(456), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x121, 0x303, (s32)gSuhallaDesertPlacements1_Data,
        PIXELS(120), 0, PIXELS(712), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x121, 0x304, (s32)gSuhallaDesertPlacements1_Data,
        PIXELS(264), 0, PIXELS(712), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x121, 0x305, (s32)gSuhallaDesertPlacements1_Data,
        PIXELS(440), 0, PIXELS(744), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x39, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSuhallaDesertPlacements2[] = {
    { 0x121, 0x311, (s32)gSuhallaDesertPlacements1_Data,
        PIXELS(216), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x121, 0x312, (s32)gSuhallaDesertPlacements1_Data,
        PIXELS(296), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x121, 0x313, (s32)gSuhallaDesertPlacements1_Data,
        PIXELS(408), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x121, 0x314, (s32)gSuhallaDesertPlacements2_1,
        PIXELS(376), 0, PIXELS(632), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x121, 0x315, 7,
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

s32 gSuhallaDesertActor12Action[] = {
    SC_CALL(SuhallaDesert_FollowLeaderWithSparks),
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
    { EVENT_TOUCH, 9, 0x9b5, EVENT_SCRIPT(SuhallaDesert_DiscussContinuingToLalivero) },
    { EVENT_TOUCH, 41, 0x301, EVENT_SCRIPT(SuhallaDesert_EnterFirstSandstorm) },
    { EVENT_TOUCH, 42, 0x302, EVENT_SCRIPT(SuhallaDesert_EnterSecondSandstorm) },
    { EVENT_TOUCH, 43, 0x303, EVENT_SCRIPT(SuhallaDesert_EnterThirdSandstorm) },
    { EVENT_TOUCH, 44, 0x304, EVENT_SCRIPT(SuhallaDesert_EnterFourthSandstorm) },
    { EVENT_TOUCH, 45, 0x305, EVENT_SCRIPT(SuhallaDesert_EnterFifthSandstorm) },
    { EVENT_TOUCH, 51, 0x311, EVENT_SCRIPT(SuhallaDesert_EnterFirstSandstorm) },
    { EVENT_TOUCH, 52, 0x312, EVENT_SCRIPT(SuhallaDesert_EnterSecondSandstorm) },
    { EVENT_TOUCH, 53, 0x313, EVENT_SCRIPT(SuhallaDesert_EnterThirdSandstorm) },
    { EVENT_TOUCH, 61, 0x206, EVENT_SCRIPT(SuhallaDesert_RestoreFirstSandstorm) },
    { EVENT_TOUCH, 62, 0x207, EVENT_SCRIPT(SuhallaDesert_RestoreSecondSandstorm) },
    { EVENT_TOUCH, 63, 0x208, EVENT_SCRIPT(SuhallaDesert_RestoreThirdSandstorm) },
    { EVENT_TOUCH, 64, 0x209, EVENT_SCRIPT(SuhallaDesert_RestoreFourthSandstorm) },
    { EVENT_TOUCH, 65, 0x20a, EVENT_SCRIPT(SuhallaDesert_RestoreFifthSandstorm) },
    { EVENT_TOUCH, 71, 0x206, EVENT_SCRIPT(SuhallaDesert_RestoreFirstSandstorm) },
    { EVENT_TOUCH, 72, 0x207, EVENT_SCRIPT(SuhallaDesert_RestoreSecondSandstorm) },
    { EVENT_TOUCH, 73, 0x208, EVENT_SCRIPT(SuhallaDesert_RestoreThirdSandstorm) },
    { EVENT_TOUCH, 54, 0x314, EVENT_SCRIPT(SuhallaDesert_EnterFourthSandstorm) },
    { EVENT_TOUCH, 81, 0x20e, EVENT_SCRIPT(SuhallaDesert_StartFollowingSandstorm) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaDesert_StartDjinnBattle) },
    { SEARCH(SEARCH_UNNAMED), 100, 0x351, FIND_PSYNERGY_STONE },
    { SEARCH(SEARCH_CHEST), 103, 0xfb1, FIND_ITEM(0x8d) },
    { SEARCH(SEARCH_CHEST), 104, 0xfb2, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_CHEST), 101, 0xfb3, FIND_ITEM(0xc0) },
    { SEARCH(SEARCH_CHEST), 102, 0xef6, 0x500006 },
    { 0x10002115, 8, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_EnableEffectChildOption) },
    { 0x10002115, 9, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_EnableEffectChildOption) },
    { 0x10002115, 10, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_EnableEffectChildOption) },
    { 0x10002115, 11, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_EnableEffectChildOption) },
    { 0x10002115, 12, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_EnableEffectChildOption) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 8, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_DouseSandstormAndStartBattle) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 9, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_DouseSandstormAndStartBattle) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 10, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_DouseSandstormAndStartBattle) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 11, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_DouseSandstormAndStartBattle) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 12, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_DouseSandstormAndStartBattle) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaDesert_ShowRevealObjects) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaDesert_HideRevealObjects) },
    { EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaDesert_RideSandstormToWorldMap) },
    { EVENT_RAISED, 55, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaDesert_EnterFifthSandstorm) },
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
