// Suhalla Gate's approach routes, wounded soldiers and Iodem's inspection of
// the attack. Observed map repairs stay beside the scenes that apply them.

#include "game_state.h"
#include "version.h"
#include "unit/scene/suhalla_gate.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "suhalla_gate.h"
#include "field_scene.h"
#include "types.h"
#include "field_event.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"

#if TBS_PATCH_3
#define SUHALLA_GATE_RETREAT_SCENE(currentScene) ((s32)&SceneId_SuhallaGate1)
#else
#define SUHALLA_GATE_RETREAT_SCENE(currentScene) (currentScene)
#endif

// Starts the encounter of the Djinni in actor slot 13, using Djinni ID 26.
static void SuhallaGate_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(13, 26);
}

// Selects the entrance table for the gate's current scene.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_SuhallaGate2) {
    return gSuhallaGateEntrances2;
  }
  if (scene == (s32)&SceneId_SuhallaGate3) {
    return gSuhallaGateEntrances3;
  }
  return gSuhallaGateEntrancesOther;
}

// These scenes have no exit-region table.
s32 SceneData_ReturnZero(void) { return 0; }

// Returns the gate's combined exit destination table.
u8 *SceneData_GetSceneTable(void) { return (u8 *)gSuhallaGate_SceneTable; }

// The first scene has a different guard arrangement after flag 0x96f is set.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_SuhallaGate2) {
    return gSuhallaGatePlacements2;
  }
  if (scene == (s32)&SceneId_SuhallaGate1) {
    if (Engine_GameFlagIsSet(0x96f) != 0) {
      return gSuhallaGatePlacements1Flagged;
    }
    return gSuhallaGatePlacements1;
  }
  return gSuhallaGatePlacementsOther;
}

// Selects the events for the approach, Djinni area or final gate scene.
const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_SuhallaGate2) {
    return gSuhallaGateEvents2;
  }
  if (scene == (s32)&SceneId_SuhallaGate3) {
    return gSuhallaGateEvents3;
  }
  return gSuhallaGateEventsOther;
}

// Restores route-dependent Retreat destinations, actors and gate tiles.
// The Spanish, French and Italian editions repair additional map cells.
s32 SuhallaGate_EnterScene(void) {
  s32 scene;

  if (Engine_GameFlagIsSet(0x89f)) {
    gGameState.savedScene = (s32)&SceneId_LunpaSuhalla;
    gGameState.savedEntrance = 10;
  }
  scene = gGameState.scene;
  if (scene == (s32)&SceneId_SuhallaGate1) {
    // Keep the vanished soldier hidden and restore localized gate geometry.
    if (Engine_GameFlagIsSet(0x897))
      Engine_ActorSetPosition(10, 0, 0);
#if TBS_PATCH_3
    else if (Engine_GameFlagIsSet(0x96f))
      Map_CopyCellAttributes(0, 0, 2, 1, 29, 18);
    if (Engine_GameFlagIsSet(0x96f)) {
      Map_CopyCellAttributes(0, 0, 2, 1, 8, 25);
      Map_CopyCellAttributes(0, 0, 2, 1, 9, 26);
    }
#endif
    // Retreat from the interior returns to the side used to enter the gate.
    if (gGameState.entrance == 3) {
      if (Engine_GameFlagIsSet(0x8fb)) {
        gGameState.retreatScene = SUHALLA_GATE_RETREAT_SCENE(scene);
        gGameState.retreatEntrance = 1;
      }
      if (Engine_GameFlagIsSet(0x8fc)) {
        gGameState.retreatScene = SUHALLA_GATE_RETREAT_SCENE(scene);
        gGameState.retreatEntrance = 5;
      }
      Engine_GameFlagClear(0x12f);
    }
    if (gGameState.entrance == 1) {
      Engine_GameFlagSet(0x8fb);
      if (!Engine_GameFlagIsSet(0x96f))
        Map_CopyCellAttributes(6, 0, 2, 1, 8, 27);
    }
    if (gGameState.entrance == 5)
      Engine_GameFlagSet(0x8fc);
  } else if (scene == (s32)&SceneId_SuhallaGate2) {
    Engine_ActorSetAnimation(8, 4);
    Engine_ActorSetAnimation(9, 4);
    Engine_ActorSetAnimation(10, 3);
    Engine_ActorSetAnimation(11, 4);
    Engine_ActorSetAnimation(12, 3);
    Object_GetById(15)->scaleY = 0x19999;
    Map_CopyCellAttributes(108, 38, 1, 1, 102, 56);
  }
  return 0;
}

// Animates two 2x2 map-cell frames before walking through exit trigger 3.
static void SuhallaGate_AnimateExit(void) {
  s32 exitTrigger;
  s32 cellSize;

  exitTrigger = gEventWork->touchedTrigger;
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  cellSize = 2;
  Event_PlayCue(158);
  Map_CopyCellsTo(66, 36, 71, 8, cellSize, cellSize);
  Engine_TaskWait(4);
  Map_CopyCellsTo(68, 36, 71, 8, cellSize, cellSize);
  Engine_TaskWait(4);
  Actor_CenterAndWalk(0, 3, -16);
  Engine_EventRequestExit(exitTrigger);
}

// Clears both remembered approach routes when leaving by trigger 1 or 5.
static void SuhallaGate_ClearRouteFlagsAndExit(void) {
  s16 exitTrigger = gEventWork->touchedTrigger;

  Engine_AudioPlayCue(123);
  Engine_GameFlagClear(0x8FB);
  Engine_GameFlagClear(0x8FC);
  Engine_EventRequestExit(exitTrigger);
}

// Moves the two guards to the passage and updates the tiles behind them.
static void SuhallaGate_RepositionGuards(void) {
  Engine_EventBegin();
  Actor_SetSpeed(8, 65536, 32768);
  Actor_SetSpeed(9, 65536, 32768);
  Actor_WalkTo(8, 136, 384);
  Actor_WalkToAndWait(9, 152, 384);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  Engine_ActorSetAnimation(8, 1);
  Map_CopyCellAttributes(6, 27, 1, 1, 7, 27);
#if TBS_PATCH_3
  Map_CopyCellAttributes(11, 26, 1, 1, 7, 26);
  Map_CopyCellAttributes(11, 26, 1, 1, 8, 26);
#else
  Map_CopyCellAttributes(9, 26, 2, 1, 7, 26);
#endif
  Engine_EventEnd();
}

// Iodem questions the wounded guards, then follows the party out of view.
static void SuhallaGate_InspectAttackWithIodem(void) {
  struct FieldActor *leader;
  s32 destinationColumn;

  Engine_EventBegin();
  Actor_SetSpeed(0, 0x19999, 0xcccc);
  Actor_WalkToAndWait(0, 120, 0x1b6);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(11, leader->x.fixed, leader->z.fixed);
  }
  Engine_TaskWait(1);
  Actor_SetSpeed(11, 0x19999, 0xcccc);
  Actor_WalkToAndWait(11, 108, 0x1af);
  Actor_FaceDirection(11, FACING_NORTH + FACING_STEP, 10);
  Actor_ShowEmote(11, EMOTE_IN_FRONT, 20);
  Actor_FaceDirection(11, FACING_NORTH + FACING_STEP, 20);
  Actor_FaceDirection(11, FACING_EAST, 40);
  Actor_FaceDirection(11, FACING_NORTH + FACING_STEP, 40);
  Actor_FaceDirection(11, FACING_EAST, 20);
  Engine_ActorStartRepeatedMotion(11, 2);
  Engine_EventSetMessage(MsgSuhallaMeaning);
  Event_ShowMessageAndWait(11, 0, 40);
  // The guards describe the attack before Iodem asks who was responsible.
  Actor_ShowEmote(8, EMOTE_IN_FRONT, 0);
  Engine_ActorRunRepeatedMotion(8, 2);
  Event_ShowMessageAndWait(8, 0, 10);
  Actor_WalkToAndWait(11, 132, 0x1a4);
  Actor_FaceDirection(11, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(0, FACING_NORTHEAST, 0);
  Actor_WalkToAndWait(11, 138, 0x1a0);
  Actor_FaceDirection(11, FACING_NORTHWEST + FACING_STEP, 10);
  Engine_ActorStartRepeatedMotion(11, 2);
  Event_ShowMessageAndWait(11, 0, 40);
  Engine_ActorRunRepeatedMotion(8, 2);
  Event_ShowMessageAndWait(8, 0, 40);
  Actor_ShowEmote(9, EMOTE_IN_FRONT, 20);
  Engine_ActorRunRepeatedMotion(9, 2);
  Event_ShowMessageAndWait(9, 0, 20);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Actor_WalkToAndWait(11, 144, 0x1a4);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(9, 2);
  Event_ShowMessageAndWait(9, 0, 20);
  Actor_SetAttachedEffect(9, 0x102);
  Engine_ActorStartRepeatedMotion(9, 3);
  Event_ShowMessageAndWait(9, 0, 20);
  Actor_FaceDirection(11, FACING_SOUTH + FACING_STEP, 20);
  Event_AskYesNo(11, 0);
  // Continue with the answer selected by the preceding yes/no question.
  if (GameFlag_IsSet(0x9b0) != 0) {
    Actor_FaceDirection(11, FACING_NORTH + FACING_STEP, 40);
    Actor_SetAttachedEffect(11, 0x102);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(11, 0, 10);
  } else {
    gEventWork->message += 1;
  }
  Actor_FaceDirection(11, FACING_SOUTH + FACING_STEP, 10);
  Event_ShowMessageAndWait(11, 0, 40);
  Actor_ShowEmote(11, EMOTE_IN_FRONT, 40);
  Actor_FaceDirection(11, FACING_NORTHWEST + FACING_STEP, 10);
  Event_ShowMessageAndWait(11, 0, 10);
  Actor_WalkToAndWait(11, 138, 0x1a0);
  Actor_FaceDirection(11, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorStartRepeatedMotion(8, 2);
  Event_ShowMessageAndWait(8, 0, 10);
  Actor_SetAttachedEffect(11, 0x102);
  Engine_ActorRunRepeatedMotion(11, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(11, 0, 20);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(0, FACING_NORTHEAST, 10);
  Engine_ActorRunRepeatedMotion(9, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(9, 0, 10);
  Engine_ActorRunRepeatedMotion(8, 2);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorRunRepeatedMotion(11, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(11, 0, 20);
  Actor_FaceDirection(11, FACING_SOUTH + FACING_STEP, 10);
  Event_ShowMessageAndWait(11, 0, 10);
  Engine_ActorSetAnimation(11, 2);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(11, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(11);
  Actor_SetPosition(11, 0, 0);
#if TBS_PATCH_3
  Map_CopyCellAttributes(6, 27, 1, 1, 7, 27);
  Map_CopyCellAttributes(11, 26, 1, 1, 7, 26);
  Map_CopyCellAttributes(11, 26, 1, 1, 8, 26);
#else
  destinationColumn = 7;
  Map_CopyCellAttributes(6, 27, 1, 1, destinationColumn, 27);
  Map_CopyCellAttributes(9, 26, 2, 1, destinationColumn, 26);
#endif
  GameFlag_Set(0x89f);
  Engine_EventEnd();
}

// The guard asks about Babi Lighthouse before Iodem's scene, then vows revenge.
static void SuhallaGate_TalkToGateGuard(void) {
  Engine_EventBegin();

  if (Engine_GameFlagIsSet(0x89f) != 0) {
    Engine_EventSetMessage(MsgSuhallaPityColossoVictor);
    Engine_EventShowMessage(9, 0);
  } else {
    Engine_EventSetMessage(MsgSuhallaWantGoBabi);
    {
      s32 mode = 0;
      s32 no = 9;

      Engine_EventOpenMessage(no, mode);
    }

    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventShowMessage(9, 0);
      Engine_ActorSetAnimationAndWait(9, 4);
      Engine_EventShowMessage(9, 0);
    } else {
      gEventWork->message += 2;
      Engine_EventShowMessage(9, 0);
    }
  }

  Engine_EventEnd();
}

// After describing the attack, the soldier blinks slowly, then faster, and
// disappears.
static void SuhallaGate_BlinkAndHideSoldier(void) {
  unsigned int beat;

  Engine_EventBegin();

  Engine_EventSetMessage(MsgFieldVenusLighthouseWasAttackedBy);
  Event_ShowMessageAndWait(10, 0, 10);

  beat = 0;
  // Six slow blinks are followed by twelve fast ones.
  do {
    Actor_SetChildValue(10, 0);
    Engine_ActorSetSpriteFlags(Actor_Get(10), 1);
    Engine_TaskWait(4);

    Actor_SetChildValue(10, 15);
    Engine_ActorSetSpriteFlags(Actor_Get(10), 0);
    beat++;
    Engine_TaskWait(4);
  } while (beat <= 5);

  beat = 0;
  do {
    Actor_SetChildValue(10, 0);
    Engine_ActorSetSpriteFlags(Actor_Get(10), 1);
    Engine_TaskWait(2);

    Actor_SetChildValue(10, 15);
    Engine_ActorSetSpriteFlags(Actor_Get(10), 0);
    beat++;
    Engine_TaskWait(2);
  } while (beat <= 11);

  Actor_SetPosition(10, 0, 0);

  GameFlag_Set(0x897);

#if TBS_PATCH_3
  Map_CopyCellAttributes(29, 19, 2, 1, 29, 18);
#endif
  Engine_EventEnd();
}

#if TBS_PATCH_3
s32 gSuhallaGate_PatrolStartup[] = {
    SC_SET(SW_COLLISION, 0),
};
#endif

s32 gSuhallaGatePlacements1Flagged1[] = {
    SC_ARG(5),
    SC_END,
};

#if TBS_PATCH_3
s32 gSuhallaGate_FirstGuardStartup[] = {
    SC_SET(SW_COLLISION, 0),
};
#endif

s32 gSuhallaGatePlacements1Flagged0[] = {
    SC_SET(SW_SCALE_X, -0x10000),
    SC_ARG(5),
    SC_END,
};

struct SceneEntrance gSuhallaGateEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 152, 0, 504, FACING_SOUTH, 0, 32, 32, 272, 544, 0},
    {1, CONDITION_ALWAYS, 152, 0, 520, FACING_NORTH, 0, 32, 32, 272, 544, 0},
    {2, CONDITION_ALWAYS, 248, 0, 280, FACING_WEST, 0, 32, 32, 272, 544, 0},
    {3, CONDITION_ALWAYS, 136, 0, 304, FACING_SOUTH, 0, 32, 32, 272, 544, 0},
    {4, CONDITION_ALWAYS, 360, 0, 296, FACING_EAST, 0, 336, 32, 736, 496, 0},
    {5, CONDITION_ALWAYS, 664, 0, 424, FACING_WEST, 0, 336, 32, 736, 496, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaGateEntrances2[] = {
    {1, CONDITION_ALWAYS, 40, 0, 304, FACING_EAST, 0, 16, 32, 416, 480, 0},
    {2, CONDITION_ALWAYS, 392, 0, 368, FACING_WEST, 0, 16, 32, 416, 480, 0},
    {3, CONDITION_ALWAYS, 40, 0, 912, FACING_EAST, 0, 16, 608, 720, 1008, 0},
    {4, CONDITION_ALWAYS, 680, 0, 904, FACING_WEST, 0, 16, 608, 720, 1008, 0},
    {5, CONDITION_ALWAYS, 616, 0, 920, FACING_SOUTH, 0, 16, 608, 720, 1008, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaGateEntrances3[] = {
    {1, CONDITION_ALWAYS, 72, 0, 264, FACING_NORTH, 0, 24, 24, 280, 256, 0},
    {2, CONDITION_ALWAYS, 184, 0, 168, FACING_SOUTH, 0, 24, 24, 280, 256, 0},
    {3, CONDITION_ALWAYS, 280, 0, 344, FACING_SOUTH, 0, 208, 288, 704, 640, 0},
    {4, CONDITION_ALWAYS, 664, 0, 616, FACING_NORTH, 0, 208, 288, 704, 640, 0},
    {SCENE_TABLE_END},
};

u32 gSuhallaGate_SceneTable[] = {
    SCENE_EXITS(169),       SCENE_EXIT(1, 2, 29),  SCENE_EXIT(2, 170, 1),
    SCENE_EXIT(3, 105, 10), SCENE_EXIT(4, 170, 4), SCENE_EXIT(5, 2, 42),
    SCENE_EXIT(0, 170, 0),  SCENE_EXIT(1, 169, 2), SCENE_EXIT(2, 170, 3),
    SCENE_EXIT(3, 170, 2),  SCENE_EXIT(4, 169, 4), SCENE_EXIT(5, 171, 1),
    SCENE_EXIT(0, 171, 0),  SCENE_EXIT(1, 170, 5), SCENE_EXIT(2, 171, 3),
    SCENE_EXIT(3, 171, 2),  SCENE_EXIT(4, 2, 33),  SCENE_EXITS_END,
};

struct ScenePlacement gSuhallaGatePlacementsOther[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gSuhallaGatePlacements2[] = {
    { 0xee, CONDITION_ALWAYS, 7,
        PIXELS(264), 0, PIXELS(400), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, 7,
        PIXELS(280), 0, PIXELS(368), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, 7,
        PIXELS(472), 0, PIXELS(912), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, 7,
        PIXELS(536), 0, PIXELS(912), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, 7,
        PIXELS(552), 0, PIXELS(880), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x5b, 0x4a, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(888), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSuhallaGatePlacements1[] = {
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(384), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(384), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSuhallaGatePlacements1Flagged[] = {
#if !TBS_PATCH_3
    { 0x98, CONDITION_ALWAYS, (s32)gSuhallaGatePlacements1Flagged0,
        PIXELS(136), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, (s32)gSuhallaGatePlacements1Flagged1,
        PIXELS(168), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, (s32)gSuhallaGatePlacements1Flagged1,
        PIXELS(472), 0, PIXELS(304), FACING_SOUTH, TALK_FACE_PARTY, 0 },
#else
    { 0x98, CONDITION_ALWAYS, (s32)gSuhallaGate_FirstGuardStartup,
        PIXELS(140), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, (s32)gSuhallaGate_PatrolStartup,
        PIXELS(164), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, (s32)gSuhallaGate_PatrolStartup,
        PIXELS(476), 0, PIXELS(300), FACING_SOUTH, TALK_FACE_PARTY, 0 },
#endif
    { 0x39, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gSuhallaGateEventsOther[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaGate_ClearRouteFlagsAndExit) },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaGate_ClearRouteFlagsAndExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaGate_AnimateExit) },
    { EVENT_TOUCH, 10, 0x96f, EVENT_SCRIPT(SuhallaGate_RepositionGuards) },
    { EVENT_TOUCH, 10, 0x89f, EVENT_SCRIPT(SuhallaGate_InspectAttackWithIodem) },
    { EVENT_TALK, 8, 0x89f, MSGNO(MsgSuhallaTravelPapersRequired) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSuhallaSendHelpFromTower) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaGate_TalkToGateGuard) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x89f, MSGNO(MsgSuhallaObeyBabiOrders) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgSuhallaAshamedBeforeIodem) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x89f, MSGNO(MsgSuhallaSeeColossoNextYear) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSuhallaSurvivedAttack) },
    { EVENT_TALK, 10, 0x897, EVENT_SCRIPT(SuhallaGate_BlinkAndHideSoldier) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40a, 0x897, EVENT_SCRIPT(SuhallaGate_BlinkAndHideSoldier) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gSuhallaGateEvents2[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
    {EVENT_TALK, 13, CONDITION_ALWAYS,
     EVENT_SCRIPT(SuhallaGate_StartDjinnBattle)},
    {0xc413, 100, 0xfb4, FIND_ITEM(0xc3)},
    {0x413, 100, 0xfb4, FIND_ITEM(0xc3)},
    {0x8413, 100, 0xfb4, FIND_ITEM(0xc3)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gSuhallaGateEvents3[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {0x21, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {SEARCH(SEARCH_UNNAMED), 100, 0x350, FIND_PSYNERGY_STONE},
    {SCENE_EVENTS_END},
};
