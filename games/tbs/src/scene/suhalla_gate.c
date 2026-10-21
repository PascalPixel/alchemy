/// @file
/// Suhalla Gate (SuhallaGate1 and SuhallaGate2, scenes 169 and 170) and
/// Gateway Cave (GatewayCave, scene 171), the pass that Babi's soldiers
/// guard on the way to Babi Lighthouse.
///
/// The party comes here after the Suhalla Desert, and the three scenes are
/// the pass in the order it is crossed:
/// - Suhalla Gate (scene 169), the gate itself. The world map leads in from
///   the south (exit 1, entrance 1) and from the east (exit 5, entrance 5),
///   each way into its own part of the map; beside the gate stands a house
///   whose door (exit 3) leads into Gate to Suhalla, the house's own area
///   name (scene 105, entrance 10). The passage north (exit 2) and the
///   eastern part (exit 4) both lead into scene 170.
///
///   Before Babi's audience after Colosso (FLAG_BABI_SPOKE_AFTER_COLOSSO), two
///   of Babi's soldiers keep the passage: only those with travel papers may
///   pass, and the party has none (Event_TalkToEastGuard). Whenever the
///   leader comes near it (cell code 10) they step together in front of it
///   and the cells there change (Event_GuardsBlockPassage).
///
///   At that audience Babi sent the party to Venus Lighthouse with Iodem, who
///   commands his soldiers and now travels with the party. Afterwards the
///   scene places the two guards lying wounded and a third soldier east of
///   the gate, and the same cell runs a scripted scene once
///   (Event_IodemQuestionsGuards, FLAG_SUHALLA_GATE_ATTACK_SEEN): Iodem steps
///   out of the party, `What is the meaning of this!?`, and the guards tell
///   of a strange group without papers whose strange powers beat them. He
///   asks whether it was Felix, Jenna's brother, who travels with the band
///   that raided Sol Sanctum; if he has already questioned the wounded
///   soldiers in Suhalla (FLAG_IODEM_MET_WOUNDED_SOLDIERS), he adds that they
///   must find Sheba, the girl from Lalivero the soldiers were escorting home,
///   before tracking down the monsters. The guards send them on after the
///   attackers, and Iodem rejoins the party. The third soldier, talked to or
///   read with Mind Read, gasps `Venus Lighthouse was... attacked by
///   strange... wanderers...`, flickers and vanishes for good
///   (Event_SoldierVanishes, FLAG_SUHALLA_GATE_SOLDIER_VANISHED). Once Iodem
///   has questioned the guards, a defeat with no defeat point returns the
///   party to the house beside the gate.
/// - Suhalla Gate (scene 170), beyond the passage, in two parts that exits 2
///   and 3 join. Dew, a Mercury Djinni, waits here and fights when spoken to
///   (Event_FightDjinni), and a chest holds a Mint. Exit 5 leads into Gateway
///   Cave.
/// - Gateway Cave (scene 171), entered from scene 170's exit 5; its exit 4
///   leads out onto the world map (the world map's entrance 33). Searching in
///   it finds a Psynergy Stone, kept by a place flag (0x350) and so back on
///   every new visit.
///
/// Scene_Initialize also remembers, in FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH
/// and _EAST, which way the party came in from the world map, so that Retreat
/// from the house leads back out that way. In the Spanish, French and Italian
/// editions the wounded soldiers lie a few pixels apart without collision, and
/// the gate's cell attributes are patched around them and the vanishing
/// soldier.

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
#include "djinn_ids.h"
#include "types.h"
#include "field_event.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

#if TBS_PATCH_3
#define SUHALLA_GATE_RETREAT_SCENE(currentScene) ((s32)&SceneId_SuhallaGate1)
#else
#define SUHALLA_GATE_RETREAT_SCENE(currentScene) (currentScene)
#endif

/// The actors this scene's code names, by their place in the placement tables.
enum {
  /// At the gate itself.
  GUARD_WEST = 8,
  GUARD_EAST = 9,
  /// After Babi's audience: the soldier east of the gate who tells of the
  /// attack on Venus Lighthouse and vanishes, and Iodem.
  VANISHING_SOLDIER = 10,
  IODEM = 11,
  /// In the second scene: Dew, a Mercury Djinni.
  DJINNI = 13
};

/// Arrivals in the first scene: from the world map south of the gate, out of
/// the house beside it, and from the world map to the east.
enum { ENTRANCE_SOUTH = 1, ENTRANCE_FROM_HOUSE = 3, ENTRANCE_EAST = 5 };

/// The house beside the gate, Gate to Suhalla, is entrance 10 of scene 105
/// (LunpaSuhalla).
enum { GATE_TO_SUHALLA_ENTRANCE = 10 };

/// Suhalla Gate's second scene (170): talking to Dew, a Mercury Djinni,
/// starts its battle.
static void Event_FightDjinni(void) {
  Field_RequestDjinnBattle(DJINNI, DJINNI_DEW);
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_SuhallaGate2) {
    return gSuhallaGateEntrances2;
  }
  if (scene == (s32)&SceneId_GatewayCave) {
    return gSuhallaGateEntrances3;
  }
  return gSuhallaGateEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gSuhallaGateExits; }

/// Suhalla Gate's guards, standing or, after Babi's audience, lying wounded
/// beside the vanishing soldier and Iodem; Dew and the second scene's sprouts.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_SuhallaGate2) {
    return gSuhallaGatePlacements2;
  }
  if (scene == (s32)&SceneId_SuhallaGate1) {
    if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
      return gSuhallaGatePlacements1AfterAudience;
    }
    return gSuhallaGatePlacements1;
  }
  return gSuhallaGatePlacementsOther;
}

/// The events of Suhalla Gate's second scene (170), of Gateway Cave (171), or
/// of the gate itself (169) for any other scene.
const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_SuhallaGate2) {
    return gSuhallaGateEvents2;
  }
  if (scene == (s32)&SceneId_GatewayCave) {
    return gSuhallaGateEvents3;
  }
  return gSuhallaGateEventsOther;
}

/// Sets up whichever of the pass's scenes is entered. Once Iodem has
/// questioned the guards, a defeat returns the party to the house beside the
/// gate. In Suhalla Gate (169) the vanished soldier stays hidden, the side the
/// party came in by is remembered for Retreat, and arriving from the south
/// before Babi's audience changes cells by the passage; the Spanish, French
/// and Italian editions also change cells around the wounded guards. In the
/// second scene (170) the first five actors take their frames, actor 15 is
/// stretched and one cell changes.
s32 Scene_Initialize(void) {
  s32 scene;

  if (Engine_GameFlagIsSet(FLAG_SUHALLA_GATE_ATTACK_SEEN)) {
    gGameState.savedScene = (s32)&SceneId_LunpaSuhalla;
    gGameState.savedEntrance = GATE_TO_SUHALLA_ENTRANCE;
  }
  scene = gGameState.scene;
  if (scene == (s32)&SceneId_SuhallaGate1) {
    if (Engine_GameFlagIsSet(FLAG_SUHALLA_GATE_SOLDIER_VANISHED))
      Engine_ActorSetPosition(VANISHING_SOLDIER, 0, 0);
#if TBS_PATCH_3
    else if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO))
      Map_CopyCellAttributes(0, 0, 2, 1, 29, 18);
    if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
      Map_CopyCellAttributes(0, 0, 2, 1, 8, 25);
      Map_CopyCellAttributes(0, 0, 2, 1, 9, 26);
    }
#endif
    // Out of the house, Retreat leads back to the side the party came in by,
    // and the place's name is not shown again.
    if (gGameState.entrance == ENTRANCE_FROM_HOUSE) {
      if (Engine_GameFlagIsSet(FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH)) {
        gGameState.retreatScene = SUHALLA_GATE_RETREAT_SCENE(scene);
        gGameState.retreatEntrance = ENTRANCE_SOUTH;
      }
      if (Engine_GameFlagIsSet(FLAG_SUHALLA_GATE_ENTERED_FROM_EAST)) {
        gGameState.retreatScene = SUHALLA_GATE_RETREAT_SCENE(scene);
        gGameState.retreatEntrance = ENTRANCE_EAST;
      }
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    }
    if (gGameState.entrance == ENTRANCE_SOUTH) {
      Engine_GameFlagSet(FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH);
      if (!Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO))
        Map_CopyCellAttributes(6, 0, 2, 1, 8, 27);
    }
    if (gGameState.entrance == ENTRANCE_EAST)
      Engine_GameFlagSet(FLAG_SUHALLA_GATE_ENTERED_FROM_EAST);
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

/// In Suhalla Gate (169), walking north into the door of the house beside it:
/// the door opens in two frames and the leader steps into Gate to Suhalla.
static void Event_EnterHouse(void) {
  s32 exitTrigger;
  s32 cellSize;

  exitTrigger = gEventWork->touchedTrigger;
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  cellSize = 2;
  Event_PlayCue(SE_DOOR_OPEN);
  Map_CopyCellsTo(66, 36, 71, 8, cellSize, cellSize);
  Engine_TaskWait(4);
  Map_CopyCellsTo(68, 36, 71, 8, cellSize, cellSize);
  Engine_TaskWait(4);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  Engine_EventRequestExit(exitTrigger);
}

/// In Suhalla Gate (169), the party leaves for the world map by trigger 1
/// (south) or 5 (east), forgetting the side it came in by.
static void Event_LeaveForWorldMap(void) {
  s16 exitTrigger = gEventWork->touchedTrigger;

  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_GameFlagClear(FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH);
  Engine_GameFlagClear(FLAG_SUHALLA_GATE_ENTERED_FROM_EAST);
  Engine_EventRequestExit(exitTrigger);
}

/// In Suhalla Gate (169) before Babi's audience, each time the party nears the
/// passage north (cell code 10): the two guards step together in front of it,
/// and the map cells around them change.
static void Event_GuardsBlockPassage(void) {
  Engine_EventBegin();
  Actor_SetSpeed(GUARD_WEST, 65536, 32768);
  Actor_SetSpeed(GUARD_EAST, 65536, 32768);
  Actor_WalkTo(GUARD_WEST, 136, 384);
  Actor_WalkToAndWait(GUARD_EAST, 152, 384);
  Actor_FaceDirection(GUARD_WEST, FACING_SOUTH, 0);
  Actor_FaceDirection(GUARD_EAST, FACING_SOUTH, 0);
  Engine_ActorSetAnimation(GUARD_WEST, 1);
  Map_CopyCellAttributes(6, 27, 1, 1, 7, 27);
#if TBS_PATCH_3
  Map_CopyCellAttributes(11, 26, 1, 1, 7, 26);
  Map_CopyCellAttributes(11, 26, 1, 1, 8, 26);
#else
  Map_CopyCellAttributes(9, 26, 2, 1, 7, 26);
#endif
  Engine_EventEnd();
}

/// In Suhalla Gate (169) after Babi's audience, on reaching the passage (cell
/// code 10), once: Iodem steps out of the party and demands to know what
/// happened. "What is the meaning of this!?" The wounded guards tell of a
/// strange group without papers whose strange powers beat them; Iodem asks
/// whether it was Felix, the party's foe, and the guards send the party on
/// after the attackers, toward Babi Lighthouse. Iodem rejoins the party. Sets
/// FLAG_SUHALLA_GATE_ATTACK_SEEN.
static void Event_IodemQuestionsGuards(void) {
  struct FieldActor *leader;
#if !TBS_PATCH_3
  s32 destinationColumn;
#endif

  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 0x1b6);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(IODEM, leader->x.fixed, leader->z.fixed);
  }
  Engine_TaskWait(1);
  Actor_SetSpeed(IODEM, 0x19999, 0xcccc);
  Actor_WalkToAndWait(IODEM, 108, 0x1af);
  Actor_FaceDirection(IODEM, FACING_NORTH + FACING_STEP, 10);
  Actor_ShowEmote(IODEM, EMOTE_IN_FRONT, 20);
  Actor_FaceDirection(IODEM, FACING_NORTH + FACING_STEP, 20);
  Actor_FaceDirection(IODEM, FACING_EAST, 40);
  Actor_FaceDirection(IODEM, FACING_NORTH + FACING_STEP, 40);
  Actor_FaceDirection(IODEM, FACING_EAST, 20);
  Engine_ActorStartRepeatedMotion(IODEM, 2);
  Engine_EventSetMessage(MsgSuhallaMeaning);
  Event_ShowMessageAndWait(IODEM, 0, 40);
  // The guards describe the attack before Iodem asks who was responsible.
  Actor_ShowEmote(GUARD_WEST, EMOTE_IN_FRONT, 0);
  Engine_ActorRunRepeatedMotion(GUARD_WEST, 2);
  Event_ShowMessageAndWait(GUARD_WEST, 0, 10);
  Actor_WalkToAndWait(IODEM, 132, 0x1a4);
  Actor_FaceDirection(IODEM, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_WalkToAndWait(IODEM, 138, 0x1a0);
  Actor_FaceDirection(IODEM, FACING_NORTHWEST + FACING_STEP, 10);
  Engine_ActorStartRepeatedMotion(IODEM, 2);
  Event_ShowMessageAndWait(IODEM, 0, 40);
  Engine_ActorRunRepeatedMotion(GUARD_WEST, 2);
  Event_ShowMessageAndWait(GUARD_WEST, 0, 40);
  Actor_ShowEmote(GUARD_EAST, EMOTE_IN_FRONT, 20);
  Engine_ActorRunRepeatedMotion(GUARD_EAST, 2);
  Event_ShowMessageAndWait(GUARD_EAST, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_WalkToAndWait(IODEM, 144, 0x1a4);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(GUARD_EAST, 2);
  Event_ShowMessageAndWait(GUARD_EAST, 0, 20);
  Actor_SetAttachedEffect(GUARD_EAST, 0x102);
  Engine_ActorStartRepeatedMotion(GUARD_EAST, 3);
  Event_ShowMessageAndWait(GUARD_EAST, 0, 20);
  Actor_FaceDirection(IODEM, FACING_SOUTH + FACING_STEP, 20);
  // Iodem: "It must have been Felix!", and the party answers yes or no. Once he
  // has heard the wounded soldiers in Suhalla he adds that they must find
  // Sheba, then track down the monsters that did this.
  Event_AskYesNo(IODEM, 0);
  if (GameFlag_IsSet(FLAG_IODEM_MET_WOUNDED_SOLDIERS) != 0) {
    Actor_FaceDirection(IODEM, FACING_NORTH + FACING_STEP, 40);
    Actor_SetAttachedEffect(IODEM, 0x102);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(IODEM, 0, 10);
  } else {
    gEventWork->message += 1;
  }
  Actor_FaceDirection(IODEM, FACING_SOUTH + FACING_STEP, 10);
  Event_ShowMessageAndWait(IODEM, 0, 40);
  Actor_ShowEmote(IODEM, EMOTE_IN_FRONT, 40);
  Actor_FaceDirection(IODEM, FACING_NORTHWEST + FACING_STEP, 10);
  Event_ShowMessageAndWait(IODEM, 0, 10);
  Actor_WalkToAndWait(IODEM, 138, 0x1a0);
  Actor_FaceDirection(IODEM, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorStartRepeatedMotion(GUARD_WEST, 2);
  Event_ShowMessageAndWait(GUARD_WEST, 0, 10);
  Actor_SetAttachedEffect(IODEM, 0x102);
  Engine_ActorRunRepeatedMotion(IODEM, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(IODEM, 0, 20);
  Engine_ActorRunRepeatedMotion(GUARD_EAST, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 10);
  Engine_ActorRunRepeatedMotion(GUARD_EAST, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(GUARD_EAST, 0, 10);
  Engine_ActorRunRepeatedMotion(GUARD_WEST, 2);
  Event_ShowMessageAndWait(GUARD_WEST, 0, 20);
  Engine_ActorRunRepeatedMotion(IODEM, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(IODEM, 0, 20);
  Actor_FaceDirection(IODEM, FACING_SOUTH + FACING_STEP, 10);
  Event_ShowMessageAndWait(IODEM, 0, 10);
  Engine_ActorSetAnimation(IODEM, 2);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(IODEM, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(IODEM);
  Actor_SetPosition(IODEM, 0, 0);
#if TBS_PATCH_3
  Map_CopyCellAttributes(6, 27, 1, 1, 7, 27);
  Map_CopyCellAttributes(11, 26, 1, 1, 7, 26);
  Map_CopyCellAttributes(11, 26, 1, 1, 8, 26);
#else
  destinationColumn = 7;
  Map_CopyCellAttributes(6, 27, 1, 1, destinationColumn, 27);
  Map_CopyCellAttributes(9, 26, 2, 1, destinationColumn, 26);
#endif
  GameFlag_Set(FLAG_SUHALLA_GATE_ATTACK_SEEN);
  Engine_EventEnd();
}

/// In Suhalla Gate (169), talking to the east guard. Until Iodem has questioned
/// the guards he asks whether the party is bound for Babi Lighthouse, `Do you
/// want to go to Babi Lighthouse?`: yes, and the party must show travel papers
/// it does not have; no, and he calls them strange. Afterwards, wounded, he
/// vows to settle the score.
static void Event_TalkToEastGuard(void) {
  Engine_EventBegin();

  if (Engine_GameFlagIsSet(FLAG_SUHALLA_GATE_ATTACK_SEEN) != 0) {
    Engine_EventSetMessage(MsgSuhallaPityColossoVictor);
    Engine_EventShowMessage(GUARD_EAST, 0);
  } else {
    Engine_EventSetMessage(MsgSuhallaWantGoBabi);
    {
      s32 mode = 0;
      s32 no = GUARD_EAST;

      Engine_EventOpenMessage(no, mode);
    }

    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      Engine_EventShowMessage(GUARD_EAST, 0);
      Engine_ActorSetAnimationAndWait(GUARD_EAST, 4);
      Engine_EventShowMessage(GUARD_EAST, 0);
    } else {
      gEventWork->message += 2;
      Engine_EventShowMessage(GUARD_EAST, 0);
    }
  }

  Engine_EventEnd();
}

/// In Suhalla Gate (169) after Babi's audience, talking to the soldier east of
/// the gate or reading his mind, until he has vanished: he gasps that Venus
/// Lighthouse was attacked by strange wanderers, then flickers, slowly and
/// then fast, and vanishes. Sets FLAG_SUHALLA_GATE_SOLDIER_VANISHED.
static void Event_SoldierVanishes(void) {
  unsigned int beat;

  Engine_EventBegin();

  Engine_EventSetMessage(MsgFieldVenusLighthouseWasAttackedBy);
  Event_ShowMessageAndWait(VANISHING_SOLDIER, 0, 10);

  beat = 0;
  // Six slow blinks are followed by twelve fast ones.
  do {
    Actor_SetChildValue(VANISHING_SOLDIER, 0);
    Engine_ActorSetSpriteFlags(Actor_Get(VANISHING_SOLDIER), 1);
    Engine_TaskWait(4);

    Actor_SetChildValue(VANISHING_SOLDIER, 15);
    Engine_ActorSetSpriteFlags(Actor_Get(VANISHING_SOLDIER), 0);
    beat++;
    Engine_TaskWait(4);
  } while (beat <= 5);

  beat = 0;
  do {
    Actor_SetChildValue(VANISHING_SOLDIER, 0);
    Engine_ActorSetSpriteFlags(Actor_Get(VANISHING_SOLDIER), 1);
    Engine_TaskWait(2);

    Actor_SetChildValue(VANISHING_SOLDIER, 15);
    Engine_ActorSetSpriteFlags(Actor_Get(VANISHING_SOLDIER), 0);
    beat++;
    Engine_TaskWait(2);
  } while (beat <= 11);

  Actor_SetPosition(VANISHING_SOLDIER, 0, 0);

  GameFlag_Set(FLAG_SUHALLA_GATE_SOLDIER_VANISHED);

#if TBS_PATCH_3
  Map_CopyCellAttributes(29, 19, 2, 1, 29, 18);
#endif
  Engine_EventEnd();
}

// How the wounded soldiers lie: animation argument 5, the west guard
// mirrored. In the Spanish, French and Italian editions each script first
// turns collision off and runs on into the one after it.
#if TBS_PATCH_3
s32 gSuhallaGateWoundedNoCollision[] = {
    SC_SET(SW_COLLISION, 0),
};
#endif

s32 gSuhallaGateWounded[] = {
    SC_ARG(5),
    SC_END,
};

#if TBS_PATCH_3
s32 gSuhallaGateWoundedFlippedNoCollision[] = {
    SC_SET(SW_COLLISION, 0),
};
#endif

s32 gSuhallaGateWoundedFlipped[] = {
    SC_SET(SW_SCALE_X, -0x10000),
    SC_ARG(5),
    SC_END,
};

struct SceneEntrance gSuhallaGateEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 152, 0, 504, FACING_SOUTH, 0, 32, 32, 272, 544, 0},
    {SUHALLA_GATE_1_FROM_WORLD_MAP_WEST, CONDITION_ALWAYS, 152, 0, 520,
     FACING_NORTH, 0, 32, 32, 272, 544, 0},
    {SUHALLA_GATE_1_FROM_SUHALLA_GATE_2_WEST, CONDITION_ALWAYS, 248, 0, 280,
     FACING_WEST, 0, 32, 32, 272, 544, 0},
    {SUHALLA_GATE_1_FROM_LUNPA_SUHALLA, CONDITION_ALWAYS, 136, 0, 304,
     FACING_SOUTH, 0, 32, 32, 272, 544, 0},
    {SUHALLA_GATE_1_FROM_SUHALLA_GATE_2_EAST, CONDITION_ALWAYS, 360, 0, 296,
     FACING_EAST, 0, 336, 32, 736, 496, 0},
    {SUHALLA_GATE_1_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 664, 0, 424,
     FACING_WEST, 0, 336, 32, 736, 496, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaGateEntrances2[] = {
    {SUHALLA_GATE_2_FROM_SUHALLA_GATE_1_WEST, CONDITION_ALWAYS, 40, 0, 304,
     FACING_EAST, 0, 16, 32, 416, 480, 0},
    {2, CONDITION_ALWAYS, 392, 0, 368, FACING_WEST, 0, 16, 32, 416, 480, 0},
    {3, CONDITION_ALWAYS, 40, 0, 912, FACING_EAST, 0, 16, 608, 720, 1008, 0},
    {SUHALLA_GATE_2_FROM_SUHALLA_GATE_1_EAST, CONDITION_ALWAYS, 680, 0, 904,
     FACING_WEST, 0, 16, 608, 720, 1008, 0},
    {SUHALLA_GATE_2_FROM_GATEWAY_CAVE, CONDITION_ALWAYS, 616, 0, 920,
     FACING_SOUTH, 0, 16, 608, 720, 1008, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaGateEntrances3[] = {
    {GATEWAY_CAVE_FROM_SUHALLA_GATE_2, CONDITION_ALWAYS, 72, 0, 264,
     FACING_NORTH, 0, 24, 24, 280, 256, 0},
    {2, CONDITION_ALWAYS, 184, 0, 168, FACING_SOUTH, 0, 24, 24, 280, 256, 0},
    {3, CONDITION_ALWAYS, 280, 0, 344, FACING_SOUTH, 0, 208, 288, 704, 640, 0},
    {GATEWAY_CAVE_FROM_WORLD_MAP, CONDITION_ALWAYS, 664, 0, 616, FACING_NORTH,
     0, 208, 288, 704, 640, 0},
    {SCENE_TABLE_END},
};

u32 gSuhallaGateExits[] = {
    SCENE_EXITS(SuhallaGate1),
    SCENE_EXIT(SUHALLA_GATE_1_TO_WORLD_MAP_EAST, WorldMap,
               WORLD_MAP_FROM_SUHALLA_GATE_1_EAST),
    SCENE_EXIT(SUHALLA_GATE_1_TO_SUHALLA_GATE_2_WEST, SuhallaGate2,
               SUHALLA_GATE_2_FROM_SUHALLA_GATE_1_WEST),
    SCENE_EXIT(SUHALLA_GATE_1_TO_LUNPA_SUHALLA, LunpaSuhalla,
               LUNPA_SUHALLA_FROM_SUHALLA_GATE_1),
    SCENE_EXIT(SUHALLA_GATE_1_TO_SUHALLA_GATE_2_EAST, SuhallaGate2,
               SUHALLA_GATE_2_FROM_SUHALLA_GATE_1_EAST),
    SCENE_EXIT(SUHALLA_GATE_1_TO_WORLD_MAP_WEST, WorldMap,
               WORLD_MAP_FROM_SUHALLA_GATE_1_WEST),
    SCENE_EXITS(SuhallaGate2),
    SCENE_EXIT(SUHALLA_GATE_2_TO_SUHALLA_GATE_1_WEST, SuhallaGate1,
               SUHALLA_GATE_1_FROM_SUHALLA_GATE_2_WEST),
    SCENE_EXIT(2, SuhallaGate2, 3),
    SCENE_EXIT(3, SuhallaGate2, 2),
    SCENE_EXIT(SUHALLA_GATE_2_TO_SUHALLA_GATE_1_EAST, SuhallaGate1,
               SUHALLA_GATE_1_FROM_SUHALLA_GATE_2_EAST),
    SCENE_EXIT(SUHALLA_GATE_2_TO_GATEWAY_CAVE, GatewayCave,
               GATEWAY_CAVE_FROM_SUHALLA_GATE_2),
    SCENE_EXITS(GatewayCave),
    SCENE_EXIT(GATEWAY_CAVE_TO_SUHALLA_GATE_2, SuhallaGate2,
               SUHALLA_GATE_2_FROM_GATEWAY_CAVE),
    SCENE_EXIT(2, GatewayCave, 3),
    SCENE_EXIT(3, GatewayCave, 2),
    SCENE_EXIT(GATEWAY_CAVE_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_GATEWAY_CAVE),
    SCENE_EXITS_END,
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

struct ScenePlacement gSuhallaGatePlacements1AfterAudience[] = {
#if !TBS_PATCH_3
    { 0x98, CONDITION_ALWAYS, (s32)gSuhallaGateWoundedFlipped,
        PIXELS(136), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, (s32)gSuhallaGateWounded,
        PIXELS(168), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, (s32)gSuhallaGateWounded,
        PIXELS(472), 0, PIXELS(304), FACING_SOUTH, TALK_FACE_PARTY, 0 },
#else
    { 0x98, CONDITION_ALWAYS, (s32)gSuhallaGateWoundedFlippedNoCollision,
        PIXELS(140), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, (s32)gSuhallaGateWoundedNoCollision,
        PIXELS(164), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, (s32)gSuhallaGateWoundedNoCollision,
        PIXELS(476), 0, PIXELS(300), FACING_SOUTH, TALK_FACE_PARTY, 0 },
#endif
    { 0x39, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gSuhallaGateEventsOther[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveForWorldMap) },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveForWorldMap) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterHouse) },
    { EVENT_TOUCH, 10, FLAG_BABI_SPOKE_AFTER_COLOSSO, EVENT_SCRIPT(Event_GuardsBlockPassage) },
    { EVENT_TOUCH, 10, FLAG_SUHALLA_GATE_ATTACK_SEEN, EVENT_SCRIPT(Event_IodemQuestionsGuards) },
    { EVENT_TALK, 8, FLAG_SUHALLA_GATE_ATTACK_SEEN, MSGNO(MsgSuhallaTravelPapersRequired) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSuhallaSendHelpFromTower) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToEastGuard) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_SUHALLA_GATE_ATTACK_SEEN, MSGNO(MsgSuhallaObeyBabiOrders) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgSuhallaAshamedBeforeIodem) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_SUHALLA_GATE_ATTACK_SEEN, MSGNO(MsgSuhallaSeeColossoNextYear) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSuhallaSurvivedAttack) },
    { EVENT_TALK, 10, FLAG_SUHALLA_GATE_SOLDIER_VANISHED, EVENT_SCRIPT(Event_SoldierVanishes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40a, FLAG_SUHALLA_GATE_SOLDIER_VANISHED, EVENT_SCRIPT(Event_SoldierVanishes) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gSuhallaGateEvents2[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
    {EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_FightDjinni)},
    {0xc413, 100, 0xfb4, FIND_ITEM(ITEM_MINT)},
    {0x413, 100, 0xfb4, FIND_ITEM(ITEM_MINT)},
    {0x8413, 100, 0xfb4, FIND_ITEM(ITEM_MINT)},
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
