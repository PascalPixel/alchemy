/// @file
/// Vale (SceneId_ValeStorm) on the night of the storm, the game's
/// prologue, when Isaac and Garet are children. A storm rages over the
/// village, and on Mt. Aleph above it the elders hold back a great Boulder,
/// "the Mt. Aleph Boulder", with their Psynergy. The scene stands in for the
/// village's main scene (SceneId_Vale) on that one night, on a map of its
/// own: the doors that lead to SceneId_Vale by day lead here instead.
/// ValeStorm_Initialize sets FLAG_VALE_STORM, which tells Vale's other scenes
/// the storm is on, until the night ends in vale_gate.c. The night runs:
///
/// - Isaac and his mother Dora come out of their house into the storm
///   (VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA) to find Kyle, who asks Dora
///   to take good care of Isaac; three years later a woman reminds Isaac
///   that he lost his father that night (vale_gate.c). Kyle means to help
///   evacuate the other villagers, Dora insists on going with him, and she
///   sends Isaac on alone to take refuge in the plaza
///   (Event_DoraSendsIsaacToThePlaza, FLAG_VALE_STORM_SENT_TO_PLAZA). This
///   first time, the scene's lightning starts with the cutscene rather than on
///   arrival.
/// - On the way, rocks fall across the paths at four spots
///   (Event_FirstRockFalls to Event_FourthRockFalls). Each rock's _FELL flag
///   puts it back on later visits, blocking the cells it covers, and its
///   _PASSED flag keeps the fall from replaying. The first time Isaac tries
///   the door of VALE_STORM_TO_VALE_INTERIORS_SHOPS_SOUTH, a stout man running
///   for the plaza bursts out and shoves him
///   (FLAG_VALE_STORM_BUMPED_INTO_VILLAGER). Garet is struggling to save his
///   chest of valuables (FLAG_VALE_STORM_HEARD_GARET once Isaac has heard him);
///   talking to him or walking off past him brings him along (a third way,
///   Event_UnusedGaretJoinsIsaac, never plays), and either way the chest is
///   left behind (FLAG_VALE_STORM_GARET_JOINED). With Garet along, the first
///   fallen rock has him say they must find another way, and the path north has
///   him turn Isaac back, since it leads to Mt. Aleph. Two cells start a
///   monster battle each, once (slots 2 and 3 of
///   ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT).
/// - Further on, the Boulder slips toward Vale while the elders strain to
///   hold it, and they tell the boys to run (Event_WatchBoulderFall,
///   FLAG_VALE_BOULDER_FELL; from then on it stays where it slipped to).
/// - At the river, Jenna's brother Felix has fallen in and is clinging on
///   while Kyle and Jenna's parents try to save him
///   (Event_WatchAttemptToSaveFelix, FLAG_VALE_STORM_SAW_FELIX_HANGING_ON). No
///   one has Psynergy left, so Dora goes north for help, sends Jenna to the
///   plaza and asks Isaac and Garet to help: either answer sends them to the
///   plaza, a yes after Jenna (ValeStorm_RunRiverSearch).
///   VALE_STORM_TO_VALE_GATE leads there (SceneId_ValeGate, at
///   VALE_GATE_FROM_VALE), where Garet's grandfather sends the boys back to
///   show Jenna and a man whose Psynergy the Psynergy Stone has restored the
///   way to the river (FLAG_VALE_STORM_SENT_TO_AID_ELDERS, vale_gate.c).
/// - Back at the river (Event_BoulderSweepsThemAway,
///   FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY), Jenna finds her parents again
///   and Dora calls everyone down to save Felix; then the Boulder comes down
///   the mountain into the river, and once it has passed, Kyle, Jenna's parents
///   and Felix are gone (ValeStorm_ShowBoulderSweepingDownriver). The man who
///   came to help can no longer move. Leaving by VALE_STORM_TO_VALE_GATE now,
///   Isaac goes for help and Garet resolves to go after him
///   (ValeStorm_GaretFollowsIsaac, FLAG_VALE_STORM_GARET_LEFT); on the way
///   Isaac overhears two strangers, Garet catches up, and the night ends
///   (vale_gate.c).
///
/// ValeStorm_Initialize rebuilds the night from those flags on every arrival:
/// the fallen rocks, the Boulder, Garet struggling with his chest until he
/// joins, and the people at the river at their stage of the scene. Every way
/// out leaves through Vale_RequestBackdropExit (common/vale.h), which stops the
/// lightning first, and each door plays its opening cells before it. The doors
/// lead into Garet's house and the other houses of SceneId_ValeInteriorsShops
/// and into Isaac's house and the rest of SceneId_ValeInteriorsInn; the paths
/// lead to the part of Vale around Kraden's cottage (SceneId_ValeKraden) and to
/// the gathered villagers (SceneId_ValeGate). gValeStorm_Exits gives each
/// exit's destination. FLAG_SCENE_TEMP_05 and _06 record the two doors left
/// open, and FLAG_SCENE_TEMP_10 the upper path, whose collision cells are
/// swapped while the leader is on it.

#include "script.h"
#include "game_state.h"
#include "unit/import.h"
#include "unit/scene/vale_storm.h"
#include "unit/scene/common/effect_psynergy.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "callback_scheduler.h"
#include "field_event.h"
#include "field_scene.h"
#include "call.h"
#include "field_service.h"
#include "field_obj.h"
#include "animspr.h"
#include "text/msg_ids.h"
#include "resource.h"
#include "field_effect.h"
#include "tbs_edition.h"
#include "b5_context.h"
#include "objcmd.h"
#include "global_cells.h"
#include "vram_tab.h"
#include "fixed_math.h"
#include "flags.h"
#include "songs.h"
#include "fx_scene.h"

static void Event_DoraSendsIsaacToThePlaza(void);
static void ValeStorm_ApplyFirstRockCollision(void);
static void ValeStorm_ApplySecondRockCollision(void);
static void ValeStorm_ApplyThirdRockCollision(void);
static void ValeStorm_ApplyFourthRockCollision(void);
static void ValeStorm_BumpIntoShovingMan(void);
static void ValeStorm_RunCallOutSequence(void);
static void ValeStorm_GaretCommentsOnBlockedPath(s32 x, s32 y);
static void ValeStorm_RunRiverSearch(void);
static void ValeStorm_ShowBoulderSweepingDownriver(void);
static void ValeStorm_FlashLightning(void);
static void ValeStorm_UpdatePulsingGlow(void);
static void ValeStorm_FlickerGlowOffset(void);
static void ValeStorm_GaretFollowsIsaac(void);
static void ValeStorm_RebuildRiverbank(void);
static void ValeStorm_FlashPaletteAndSpawnEffects(struct FieldActor *actor);
static void ValeStorm_SpawnEffectPair(struct FieldActor *parent);
static void ValeStorm_UpdateBoulderSceneEffects(void);
static void ValeStorm_CycleBoulderPalette(void);
static void ValeStorm_CopyLeaderSpritePriority(void);

enum { STORM_CALLBACK_PRIORITY = 3200 };

/// The placed actors, numbered in the order of gValeStorm_Placements; the
/// elders holding back the Boulder (gValeStorm_BoulderWatchers) follow them.
enum StormNightActor {
  /// The strongman who comes back to the river with Jenna, his Psynergy
  /// restored at
  /// the Psynergy Stone (vale_gate.c: "Enough to save Felix"); after the
  /// Boulder has passed he tries to go for help and cannot. "I... I'll go!",
  /// then "It's no use. I can't move..."
  /// ★ Named from the game: "I'll go!" (MsgValeIllGo)
  ACTOR_EXHAUSTED_STRONGMAN = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "Dora, hurry!" (MsgValeDoraHurryBoulder)
  ACTOR_DORA,
  /// ★ Named from the game: "Let me help you, Kyle!" (MsgValeKyleAbleStop)
  ACTOR_KYLE,
  /// Eight huge rocks that Isaac "can't even budge": ACTOR_FIRST_ROCK to
  /// ACTOR_FIRST_ROCK + 3 fall across the paths (Event_FirstRockFalls to
  /// Event_FourthRockFalls, FLAG_VALE_STORM_FIRST_ROCK_FELL on),
  /// ACTOR_FIRST_ROCK + 4 and + 5 already lie there, and ACTOR_FIRST_ROCK + 6
  /// and + 7 lie there only once the Boulder has passed down the river
  /// (ValeStorm_ShowBoulderSweepingDownriver).
  /// ★ Named from the game: "can't even budge" (MsgValeRockCannotBeMoved)
  ACTOR_FIRST_ROCK,
  /// ★ Named from the game: "The Mt. Aleph Boulder is going to fall!"
  /// (MsgValeWake)
  ACTOR_BOULDER = ACTOR_FIRST_PLACED + 11, ///< drawn at twice its size
  /// A stout man who runs out of his house and shoves Isaac. "Huh? Isaac?"
  /// ★ Named from the game: "Sorry to shove you like that." (MsgValeHuh)
  ACTOR_SHOVING_MAN,
  /// The things Garet struggles to save. "If a rock hits me now, I'll lose
  /// both my chest and my life."
  /// ★ Named from the game: "both my chest and my life" (MsgValeRockHitsLose)
  ACTOR_GARETS_CHEST,
  /// ★ Named from the game: "Garet (child)" (MsgDebugGlyphName)
  ACTOR_GARET, ///< the young Garet's sprite, 0x13
  /// ★ Named from the game: "Felix (child)" (MsgDebugGlyphName)
  ACTOR_FELIX, ///< in the river
  /// Jenna's father, a man with brown hair in a green shirt; Jenna calls him
  /// and her mother "Mom! Dad!". "Hang on, Felix! Help is on the way!"
  /// ★ Named from the game: "Mom! Dad!" (MsgValeMomDadBack)
  ACTOR_JENNAS_FATHER,
  /// Jenna's mother, a woman with pink hair in a white apron. "Don't let go,
  /// Felix!"
  /// ★ Named from the game: "Mom! Dad!" (MsgValeMomDadBack)
  ACTOR_JENNAS_MOTHER,
  /// ★ Named from the game: "Oh no! My brother!" (MsgValeNoBrother)
  ACTOR_JENNA,        ///< "Oh no! My brother!"
  ACTOR_BOULDER_GLOW, ///< the light around the Boulder as it rolls
  // The elders holding back the Boulder with their Psynergy ("Go aid the
  // elders!", vale_gate.c), from gValeStorm_BoulderWatchers, which
  // Event_WatchBoulderFall places after the scene's own actors and removes.
  // Two invisible actors stand on the spots of ACTOR_ELDER_EAST and
  // ACTOR_ELDER_WEST, and some of the elders' lines come from them.
  /// The invisible actor on ACTOR_ELDER_EAST's spot that speaks for that elder,
  /// the elders' first and third lines. "Ahhh! The Boulder! It's falling!",
  /// then "If our Psynergy runs out, Vale will be destroyed!"
  /// ★ Named from the game: "It's falling!" (MsgValeTheBoulderIsFalling)
  ACTOR_ELDER_EAST_STAND_IN,
  /// The westernmost elder, drawn with the sprite of the boy with brown hair
  /// (as is ACTOR_ELDER_FAR_EAST), tells the boys to run. "Hurry, boys! Run!!!
  /// It's
  /// just a little farther!"
  /// ★ Named from the game: "Hurry, boys!" (MsgValeTheBoulderIsFalling)
  ACTOR_ELDER_FAR_WEST,
  /// The easternmost elder, the same sprite drawn mirrored. "We have to hold on
  /// until the
  /// villagers are safe!"
  /// ★ Named from the game: "hold on until the villagers are safe"
  /// (MsgValeTheBoulderIsFalling)
  ACTOR_ELDER_FAR_EAST,
  /// The invisible actor on ACTOR_ELDER_WEST's spot that speaks for that elder,
  /// the elders' second line. "It's too heavy! We can't hold it for long!"
  /// ★ Named from the game: "We can't hold it for long"
  /// (MsgValeTheBoulderIsFalling)
  ACTOR_ELDER_WEST_STAND_IN,
  /// The elders between the two are men with long brown hair and beards in
  /// long robes.
  /// ★ Named from the game: "Go aid the elders!" (MsgValeGoAidElders)
  ACTOR_ELDER_EAST, ///< east of ACTOR_ELDER_WEST, and drawn as that elder is
  /// ★ Named from the game: "Go aid the elders!" (MsgValeGoAidElders)
  ACTOR_ELDER_WEST
};

/// Vale's place flags this scene keeps while the party is in Vale.
enum StormNightPlaceFlag {
  /// The strongman at the river tried to go for help and could not.
  /// ★ Named from the game: "I'll go!" (MsgValeIllGo)
  FLAG_VALE_STORM_STRONGMAN_TRIED_TO_GO = 0x305,
  /// Garet has said once that they cannot get around a fallen rock.
  /// ★ Named from the game: "We can't get around this rock!"
  /// (MsgValeCantGetAroundThisRock)
  FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH = 0x30c,
  /// Kyle has greeted Isaac at the river.
  /// ★ Named from the game: "Oh," (MsgValeOh)
  FLAG_VALE_STORM_KYLE_GREETED_ISAAC = 0x30d,
  /// The party has passed each rock's spot since coming to Vale, so its event
  /// does not play again.
  /// ★ Named from the game: "We can't get around this rock!"
  /// (MsgValeCantGetAroundThisRock)
  FLAG_VALE_STORM_FIRST_ROCK_PASSED = 0x310,
  /// ★ Named from the game: "We can't get around this rock!"
  /// (MsgValeCantGetAroundThisRock)
  FLAG_VALE_STORM_SECOND_ROCK_PASSED = 0x311,
  /// ★ Named from the game: "We can't get around this rock!"
  /// (MsgValeCantGetAroundThisRock)
  FLAG_VALE_STORM_THIRD_ROCK_PASSED = 0x312,
  /// ★ Named from the game: "We can't get around this rock!"
  /// (MsgValeCantGetAroundThisRock)
  FLAG_VALE_STORM_FOURTH_ROCK_PASSED = 0x313
};

// Scene flags, cleared on every ordinary entrance:
// - FLAG_SCENE_TEMP_05: the door of Isaac's house, from which Dora and Isaac
//   came, stands open.
// - FLAG_SCENE_TEMP_06: the stout man has run out of the door of
//   VALE_STORM_TO_VALE_INTERIORS_SHOPS_SOUTH, leaving it open.
// - FLAG_SCENE_TEMP_10: the leader is on the upper path, whose collision cells
//   are swapped until the leader steps off it.

static s32 sShakeDone;
static s32 sShakeShift;

/// Chooses a new random heading whenever the actor countdown expires.
static s32
ValeStorm_RerollHeadingOnCountdown(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  s32 timer = actor->timer;
  s32 countdown = (s16)timer;

  if (timer == 0) {
    actor->facing = Engine_RandomNext();
    countdown = Math_RemainderUnsigned(Engine_RandomNext(), 20) + 20;
    actor->timer = countdown;
  }
  actor->timer = countdown - 1;
  return 1;
}

#include "common/object_pulse.h"

/// Returns where the party appears;
/// VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA is where Dora and Isaac come
/// out of the door of their house, from SceneId_ValeInteriorsInn's
/// VALE_INTERIORS_INN_TO_VALE_STORM_WITH_DORA.
const struct SceneEntrance *ValeStorm_GetEntrances(void) {
  return gValeStorm_Entrances;
}

const struct SceneRegion *ValeStorm_GetRegions(void) { return NULL; }

const u32 *ValeStorm_GetExits(void) { return gValeStorm_Exits; }

const struct ScenePlacement *ValeStorm_GetPlacements(void) {
  return gValeStorm_Placements;
}

const struct SceneEvent *ValeStorm_GetEvents(void) { return gValeStorm_Events; }

/// In Vale on the stormy night, swaps the upper path's collision cells as the
/// leader steps onto it.
/// Reached by stepping on cell code 15. Sets FLAG_SCENE_TEMP_10,
/// which ValeStorm_Initialize reads to swap them again.
static void Event_SetPathCellOverride(void) {
  Engine_GameFlagSet(FLAG_SCENE_TEMP_10);
  Map_CopyCellAttributes(40, 84, 7, 4, 10, 84);
}

/// In Vale on the stormy night, restores the upper path's collision cells as
/// the leader steps off it.
/// Reached by stepping on cell code 14. Clears FLAG_SCENE_TEMP_10.
static void Event_ClearPathCellOverride(void) {
  Engine_GameFlagClear(FLAG_SCENE_TEMP_10);
  Map_CopyCellAttributes(40, 89, 7, 4, 10, 84);
}

#include "common/vale.h"

/// Unused: on the stormy night, the Sanctum's door opens and the leader goes
/// into a house of Vale.
///
/// It takes VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SANCTUM_DOOR
/// (SceneId_ValeInteriorsShops, at VALE_INTERIORS_SHOPS_FROM_VALE_WEST, the
/// house VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST and its second door lead to
/// as well). No event row names it, and nothing
/// else takes that exit.
UNUSED static void ValeStorm_UnusedOpenSanctumDoor(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gValeStorm_CellSteps0, 45, 11);
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 257, 420);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(
      VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SANCTUM_DOOR);
}

/// In Vale on the stormy night, stepping on cell code 2 requests exit 1, the
/// one number the exit table leaves out; Scene_TakeExit therefore falls back to
/// the first exit's scene, Vale's SceneId_ValeGate, at ENTRANCE_DEFAULT.
static void Event_LeaveByMissingExit(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Vale_RequestBackdropExit(1);
}

/// In Vale on the stormy night, leaves for the part of Vale around Kraden's
/// cottage.
/// Reached by stepping on cell code 3; arrives at VALE_KRADEN_FROM_VALE_NORTH.
static void Event_LeaveToValeKradenNorth(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Vale_RequestBackdropExit(VALE_STORM_TO_VALE_KRADEN_NORTH);
}

/// In Vale on the stormy night, leaves for the part of Vale around Kraden's
/// cottage by its second path.
/// Reached by stepping on cell code 4; arrives at VALE_KRADEN_FROM_VALE_SOUTH.
static void Event_LeaveToValeKradenSouth(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Vale_RequestBackdropExit(VALE_STORM_TO_VALE_KRADEN_SOUTH);
}

/// In Vale on the stormy night, leaves for the plaza, where the villagers have
/// gathered (SceneId_ValeGate).
/// Reached by stepping on cell code 5; arrives at VALE_GATE_FROM_VALE. After
/// the Boulder swept them away, Garet first resolves to go after Isaac
/// (ValeStorm_GaretFollowsIsaac).
static void Event_LeaveToValeGate(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) != 0 &&
      Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_LEFT) == 0) {
    ValeStorm_GaretFollowsIsaac();
  }
  Vale_RequestBackdropExit(VALE_STORM_TO_VALE_GATE);
}

/// In Vale on the stormy night, opens the door of Garet's house and goes in.
/// Reached by walking north into cell code 6; arrives at
/// VALE_INTERIORS_SHOPS_FROM_VALE_EAST, the house where Garet's family lives.
static void Event_LeaveToValeInteriorsShopsEast(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gValeStorm_CellSteps1, 54, 32);
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 406, 727);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(VALE_STORM_TO_VALE_INTERIORS_SHOPS_EAST);
}

/// In Vale on the stormy night, Isaac's first time at the door of
/// VALE_STORM_TO_VALE_INTERIORS_SHOPS_SOUTH: a stout man runs out and shoves
/// him; afterwards the door opens as usual. "Huh? Isaac?" Reached by walking
/// north into cell code 7. The shove plays until
/// FLAG_VALE_STORM_BUMPED_INTO_VILLAGER or FLAG_VALE_STORM_SECOND_ROCK_FELL is
/// set, and sets FLAG_SCENE_TEMP_06; otherwise goes into a house of Vale
/// (SceneId_ValeInteriorsShops, at VALE_INTERIORS_SHOPS_FROM_VALE_SOUTH).
static void Event_LeaveToValeInteriorsShops(void) {
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_06) == 0) {
    Event_PlayCue(SE_DOOR_OPEN);
    Map_AnimateCells(gValeStorm_CellSteps2, 45, 39);
  }
  if (GameFlag_IsSet(FLAG_VALE_STORM_BUMPED_INTO_VILLAGER) == 0 &&
      GameFlag_IsSet(FLAG_VALE_STORM_SECOND_ROCK_FELL) == 0) {
    ValeStorm_BumpIntoShovingMan();
    GameFlag_Set(FLAG_SCENE_TEMP_06);
  } else {
    Actor_WalkTo(ACTOR_PARTY_LEADER, 262, 805);
    Engine_EventWait(3);
    Vale_RequestBackdropExit(VALE_STORM_TO_VALE_INTERIORS_SHOPS_SOUTH);
  }
}

/// In Vale on the stormy night, goes back into Isaac's house through the door
/// he and Dora came out of, opening it unless they left it open.
/// Reached by walking north into cell code 8; takes
/// VALE_STORM_TO_VALE_INTERIORS_INN_ISAACS_HOUSE, to
/// VALE_INTERIORS_INN_FROM_VALE in SceneId_ValeInteriorsInn, where Vale's
/// VALE_TO_VALE_INTERIORS_INN leads by day.
static void Event_LeaveToValeInteriorsInn(void) {
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_05) == 0) {
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapAnimateCells(gValeStorm_CellSteps1, 50, 44);
  }
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 340, 888);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(VALE_STORM_TO_VALE_INTERIORS_INN_ISAACS_HOUSE);
}

/// In Vale on the stormy night, opens a door and goes in.
/// Reached by walking north into cell code 9; arrives in a house of Vale at
/// VALE_INTERIORS_SHOPS_FROM_VALE_NORTH.
static void Event_LeaveToValeInteriorsShopsNorth(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gValeStorm_CellSteps2, 49, 69);
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 326, 1126);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(VALE_STORM_TO_VALE_INTERIORS_SHOPS_NORTH);
}

/// In Vale on the stormy night, opens a door and goes in.
/// Reached by walking north into cell code 10; takes
/// VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST, to
/// VALE_INTERIORS_SHOPS_FROM_VALE_WEST in SceneId_ValeInteriorsShops, where
/// Vale's VALE_TO_VALE_INTERIORS_SHOPS_WEST leads by day.
static void Event_LeaveToValeInteriorsShopsWest(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gValeStorm_CellSteps3, 52, 76);
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 374, 1238);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST);
}

/// In Vale on the stormy night, opens a door and goes in, to the room
/// Event_LeaveToValeInteriorsShopsWest leads to.
/// Reached by walking north into cell code 11; takes
/// VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SECOND_DOOR, which also
/// arrives at VALE_INTERIORS_SHOPS_FROM_VALE_WEST. The leader walks to the spot
/// Event_LeaveToValeInteriorsInnWest walks him to.
static void Event_LeaveToValeInteriorsShopsWestBySecondDoor(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_CellSteps1, 35, 74);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 1206);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SECOND_DOOR);
}

/// In Vale on the stormy night, opens a door and goes in.
/// Reached by walking north into cell code 12; arrives in another part of Vale
/// at VALE_INTERIORS_INN_FROM_VALE_STORM_WEST.
static void Event_LeaveToValeInteriorsInnWest(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_CellSteps1, 35, 73);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 1206);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(VALE_STORM_TO_VALE_INTERIORS_INN_WEST);
}

/// In Vale on the stormy night, opens a door and goes in.
/// Reached by walking north into cell code 13; arrives in another part of Vale
/// at VALE_INTERIORS_INN_FROM_VALE_STORM_EAST.
static void Event_LeaveToValeInteriorsInnEast(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_CellSteps2, 38, 72);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 146, 1182);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(VALE_STORM_TO_VALE_INTERIORS_INN_EAST);
}

/// Arriving in Vale on the stormy night: restores the fallen rocks, the river
/// scene and the Boulder from their flags, starts the storm, and plays the
/// scene with Dora and Kyle on the first arrival by
/// VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA. Called by Field_RunScene once
/// the actors are placed. Sets FLAG_VALE_STORM; after a battle
/// (FLAG_KEEP_PARTY_POSITION) the two doors close again. Garet, until he joins,
/// is shown struggling with his things.
s32 ValeStorm_Initialize(void) {
  BattleFx_SetQueuedSoundAndPlay(SE_170);
  Engine_ActorSetPosition(ACTOR_FELIX, 0, 0);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    Engine_GameFlagClear(FLAG_SCENE_TEMP_05);
    Engine_GameFlagClear(FLAG_SCENE_TEMP_06);
  }
  // Restore the rocks that have already fallen.
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_FIRST_ROCK_FELL) != 0) {
    Actor_SetPosition(ACTOR_FIRST_ROCK, PIXELS(334), PIXELS(932));
    ValeStorm_ApplyFirstRockCollision();
  }
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_SECOND_ROCK_FELL) != 0) {
    Actor_SetPosition(ACTOR_FIRST_ROCK + 1, PIXELS(224), PIXELS(872));
    ValeStorm_ApplySecondRockCollision();
  }
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_THIRD_ROCK_FELL) != 0) {
    Actor_SetPosition(ACTOR_FIRST_ROCK + 2, PIXELS(64), PIXELS(703));
    ValeStorm_ApplyThirdRockCollision();
  }
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_FOURTH_ROCK_FELL) != 0) {
    Actor_SetPosition(ACTOR_FIRST_ROCK + 3, PIXELS(432), PIXELS(1147));
    ValeStorm_ApplyFourthRockCollision();
  }
  // All eight rocks block the way.
  Object_GetById(ACTOR_FIRST_ROCK)->collisionFlags |= 4;
  Object_GetById(ACTOR_FIRST_ROCK + 1)->collisionFlags |= 4;
  Object_GetById(ACTOR_FIRST_ROCK + 2)->collisionFlags |= 4;
  Object_GetById(ACTOR_FIRST_ROCK + 3)->collisionFlags |= 4;
  Object_GetById(ACTOR_FIRST_ROCK + 4)->collisionFlags |= 4;
  Object_GetById(ACTOR_FIRST_ROCK + 5)->collisionFlags |= 4;
  Object_GetById(ACTOR_FIRST_ROCK + 6)->collisionFlags |= 4;
  Object_GetById(ACTOR_FIRST_ROCK + 7)->collisionFlags |= 4;
  // Garet walks with the party once he has joined.
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_JOINED) != 0) {
    Engine_ActorSetPosition(ACTOR_GARET, 0, 0);
  }
  {
    struct FieldActor *actor;

    actor = Object_GetById(ACTOR_BOULDER);
    actor->scaleX = 2 * Q16_ONE;
    actor->scaleY = 2 * Q16_ONE;
  }
  // The Boulder lies where it fell, or waits on the mountainside.
  if (Engine_GameFlagIsSet(FLAG_VALE_BOULDER_FELL) != 0) {
    Actor_SetPosition(ACTOR_BOULDER, PIXELS(114), PIXELS(333));
  } else {
    Actor_SetActionScript(ACTOR_BOULDER, gValeStorm_BoulderScript);
  }
  // Restore the riverbank cast at the saved stage of the storm.
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) != 0) {
    s32 facing;
    const s32 *table;
    ValeStorm_RebuildRiverbank();
    Actor_SetPosition(ACTOR_DORA, PIXELS(165), PIXELS(1229));
    {
      struct FieldActor *actor;

      actor = Object_GetById(ACTOR_DORA);
      facing = FACING_NORTHEAST;
      actor->facing = facing;
      actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
      table = gValeStorm_SquashAndStretchScript;
      actor->work = 1;
      Actor_SetActionScript(ACTOR_DORA, table);
    }
    Engine_ActorSetPosition(ACTOR_JENNA, PIXELS(165), PIXELS(1254));
    {
      struct FieldActor *actor;

      actor = Object_GetById(ACTOR_JENNA);
      actor->facing = facing;
      actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
      actor->work = 2;
      Actor_SetActionScript(ACTOR_JENNA, table);
    }
    Engine_ActorSetPosition(ACTOR_GARET, PIXELS(152), PIXELS(1285));
    {
      struct FieldActor *actor;

      actor = Object_GetById(ACTOR_GARET);
      actor->facing = facing;
      actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
      actor->work = 3;
      Actor_SetActionScript(ACTOR_GARET, table);
    }
    Engine_ActorSetPosition(ACTOR_EXHAUSTED_STRONGMAN, PIXELS(184),
                            PIXELS(1304));
    {
      struct FieldActor *actor;

      actor = Object_GetById(ACTOR_EXHAUSTED_STRONGMAN);
      actor->facing = facing;
      actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
      actor->work = 4;
      Actor_SetActionScript(ACTOR_EXHAUSTED_STRONGMAN, table);
    }
    Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, 6);
    Object_GetById(ACTOR_GARET)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    Object_GetById(ACTOR_EXHAUSTED_STRONGMAN)->priorityFlags &=
        ~ACTOR_PRIORITY_AUTOMATIC;
    FieldScheduler_AddCallback(ValeStorm_CopyLeaderSpritePriority,
                               STORM_CALLBACK_PRIORITY);
    Engine_ActorSetPosition(ACTOR_JENNAS_FATHER, 0, 0);
    Engine_ActorSetPosition(ACTOR_JENNAS_MOTHER, 0, 0);
    Engine_ActorSetPosition(ACTOR_FELIX, 0, 0);
    Engine_ActorSetPosition(ACTOR_BOULDER, 0, 0);
    if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_LEFT) != 0) {
      Engine_ActorSetPosition(ACTOR_GARET, 0, 0);
    }
    // While they try to save Felix: Kyle and Jenna's parents at the river,
    // Felix in it.
  } else if (Engine_GameFlagIsSet(FLAG_VALE_STORM_SAW_FELIX_HANGING_ON) != 0) {
    const s32 *table;
    Actor_SetPosition(ACTOR_KYLE, PIXELS(192), PIXELS(1214));
    Actor_FaceDirection(ACTOR_KYLE, FACING_SOUTHEAST, 0);
    Engine_ActorSetAnimation(ACTOR_KYLE, 5);
    {
      struct FieldActor *actor;

      actor = Object_GetById(ACTOR_KYLE);
      actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    }
    table = gValeStorm_SquashAndStretchScript;
    Actor_SetActionScript(ACTOR_KYLE, table);
    Actor_SetPosition(ACTOR_JENNAS_FATHER, PIXELS(227), PIXELS(1214));
    Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_SOUTH, 0);
    Engine_ActorSetAnimation(ACTOR_JENNAS_FATHER, 6);
    {
      struct FieldActor *actor;

      actor = Object_GetById(ACTOR_JENNAS_FATHER);
      actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    }
    Actor_SetActionScript(ACTOR_JENNAS_FATHER, table);
    Actor_SetPosition(ACTOR_JENNAS_MOTHER, PIXELS(247), PIXELS(1214));
    Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_SOUTH, 0);
    Engine_ActorSetAnimation(ACTOR_JENNAS_MOTHER, 6);
    {
      struct FieldActor *actor;

      actor = Object_GetById(ACTOR_JENNAS_MOTHER);
      actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    }
    Actor_SetActionScript(ACTOR_JENNAS_MOTHER, table);
    Actor_SetPosition(ACTOR_FELIX, PIXELS(243), PIXELS(1277));
    Engine_ActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FELIX), 0);
    Engine_ActorSetPosition(ACTOR_FIRST_ROCK + 6, 0, 0);
    Engine_ActorSetPosition(ACTOR_FIRST_ROCK + 7, 0, 0);
  } else {
    Engine_ActorSetPosition(ACTOR_FIRST_ROCK + 6, 0, 0);
    Engine_ActorSetPosition(ACTOR_FIRST_ROCK + 7, 0, 0);
  }
  // The lightning starts at once, except before Dora and Kyle's scene.
  if (gGameState.entrance != VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA ||
      Engine_GameFlagIsSet(FLAG_VALE_STORM_SENT_TO_PLAZA) != 0) {
    Effect_SoundAndFlash();
    FieldFx_StartLightning();
  }
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_10) != 0) {
    Event_SetPathCellOverride();
  }
  Engine_GameFlagSet(FLAG_VALE_STORM);
  Map_CopyCellAttributes(29, 24, 1, 2, 26, 46);
  Map_CopyCellAttributes(29, 25, 1, 1, 27, 46);
  Map_CopyCellAttributes(29, 25, 1, 1, 28, 46);
  Map_CopyCellAttributes(19, 90, 1, 1, 20, 88);
  Map_CopyCellAttributes(19, 90, 1, 1, 20, 89);
  {
    struct FieldActor *actor;

    actor = Object_GetById(ACTOR_GARETS_CHEST);
    actor->motionFlags = 0;
    actor->y.fixed = PIXELS(192);
    actor->collisionFlags = 8;
    Engine_ActorSetSpriteFlags(actor, 0);
  }
  Engine_TaskWait(1);
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_SENT_TO_PLAZA) == 0 &&
      gGameState.entrance == VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA) {
    Event_DoraSendsIsaacToThePlaza();
    return 0;
  }
  // Garet drags his chest out of his house until he joins, then the chest
  // stands by the door.
  Engine_ActorSetAnimation(ACTOR_FELIX, 7);
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_JOINED) == 0) {
    Engine_EventBegin();
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    Actor_SetPosition(ACTOR_GARET, PIXELS(400), PIXELS(611));
    Actor_SetPosition(ACTOR_GARETS_CHEST, PIXELS(424), PIXELS(627));
    Actor_WalkTo(ACTOR_GARET, 400, 619);
    Actor_WalkToAndWait(ACTOR_GARETS_CHEST, 424, 619);
    Engine_ActorSetAnimation(ACTOR_GARETS_CHEST, ANIM_WALK);
    Engine_ActorSetAnimation(ACTOR_GARET, 5);
    Engine_EventEnd();
  } else {
    Engine_EventBegin();
    Actor_SetPosition(ACTOR_GARETS_CHEST, PIXELS(424), PIXELS(627));
    Actor_WalkToAndWait(ACTOR_GARETS_CHEST, 424, 619);
    Engine_ActorSetAnimation(ACTOR_GARETS_CHEST, ANIM_NOD);
    Engine_EventEnd();
  }
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 24;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  FieldFx_ResumeLightning();
  return 0;
}

/// In Vale on the stormy night, Dora comes out into the storm with Isaac to
/// Kyle, and the two send Isaac on to the plaza while they go to help.
/// "Kyle... Will they be able to stop the Boulder?"
/// Called by ValeStorm_Initialize on arrival by
/// VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA until
/// FLAG_VALE_STORM_SENT_TO_PLAZA is set. Sets it and
/// FLAG_SCENE_TEMP_05, clears FLAG_SHOW_LOCATION_NAME, and
/// removes Dora and Kyle.
static void Event_DoraSendsIsaacToThePlaza(void) {
  s32 message;

  Engine_EventBegin();
  Effect_SoundAndFlash();
  FieldFx_StartLightning();
  FieldFx_LastLightning();
  Engine_TaskWait(60);
  // The camera looks down on the door of Isaac's house; Kyle waits outside.
  Camera_SetSpeed(PIXELS(1) / 4, PIXELS(1) / 32);
  Camera_MoveTo(PIXELS(316), PIXELS(160), PIXELS(880), 1);
  Actor_SetPosition(ACTOR_KYLE, PIXELS(294), PIXELS(868));
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 16;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  FieldFx_ResumeLightning();
  // Dora and Isaac come out of the door.
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_CellSteps1, 50, 44);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_KYLE, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_DORA, PIXELS(342), PIXELS(890));
  Actor_WalkToAndWait(ACTOR_DORA, 342, 905);
  BattleFx_PlayQueuedSound();
  Actor_WalkTo(ACTOR_DORA, 296, 905);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(342), PIXELS(890));
  Actor_WalkTo(ACTOR_PARTY_LEADER, 342, 890);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 342, 905);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 318, 905);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_STAND);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 60);
  // Dora: "Kyle... Will they be able to stop the Boulder?" Kyle: "I don't think
  // so..." and tells the two to take refuge in the plaza.
  message = MsgValeKyleAbleStop;
  Engine_EventSetMessage(message);
  Event_ShowMessage(ACTOR_DORA, 0);
  Actor_WalkToAndWait(ACTOR_KYLE, 294, 838);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_KYLE, ANIM_SHAKE_HEAD);
  Event_ShowMessage(ACTOR_KYLE, 0);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_KYLE, FACING_SOUTH, 0);
  Event_ShowMessageAndWait(ACTOR_KYLE, 0, 20);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 1, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 10);
  // "Aren't you coming?" "I need to help evacuate the other villagers."
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_KYLE, ANIM_SHAKE_HEAD);
  Event_ShowMessage(ACTOR_KYLE, 0);
  Actor_SetAttachedEffect(ACTOR_DORA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 50);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 10);
  // Dora runs to Kyle: "Let me help you, Kyle!" "It's too dangerous, Dora."
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_WalkToAndWait(ACTOR_DORA, 289, 883);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTHEAST, 0);
  Event_ShowMessage(ACTOR_DORA, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_KYLE, 2);
  Event_ShowMessage(ACTOR_KYLE, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  // "Isaac is old enough to get to the plaza on his own."
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST, 10);
  // "You can find your way, can't you, Isaac?" She asks again on every no:
  // "You're a big boy now, Isaac."
  Engine_EventSetMessage((message + 8));
  Event_OpenMessage(ACTOR_DORA, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 302, 905);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  while (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
    Engine_EventSetMessage(MsgValeBigBoyWhy);
    Event_OpenMessage(ACTOR_DORA, 0);
  }
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  // "You know the way--just go south to get to the plaza. Be careful!"
  Engine_EventSetMessage(MsgValeKnowWayGo);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  // Kyle and Dora run north toward the Boulder and out of sight.
  Actor_SetSpeed(ACTOR_KYLE, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_WalkTo(ACTOR_KYLE, 297, 750);
  Engine_EventWait(10);
  Actor_WalkToAndWait(ACTOR_DORA, 297, 750);
  Actor_SetPosition(ACTOR_DORA, 0, 0);
  Actor_SetPosition(ACTOR_KYLE, 0, 0);
  Engine_ActorSetAnimation(ACTOR_KYLE, ANIM_STAND);
  Engine_ActorSetAnimation(ACTOR_GARETS_CHEST, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_GARET, 5);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  GameFlag_Set(FLAG_VALE_STORM_SENT_TO_PLAZA);
  GameFlag_Set(FLAG_SCENE_TEMP_05);
  Engine_EventEnd();
}

/// In Vale on the stormy night, walking past: the first rock
/// (ACTOR_FIRST_ROCK) falls across the path, and Garet, once with Isaac, says
/// they cannot get around it.
/// Reached by stepping on cell code 41 until FLAG_VALE_STORM_FIRST_ROCK_PASSED
/// is set, which this sets. Sets FLAG_VALE_STORM_FIRST_ROCK_FELL as the rock
/// falls; with Garet along, before the Boulder swept Felix, his parents and
/// Kyle away, his first comment on any rock sets
/// FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH.
static void Event_FirstRockFalls(void) {
  struct FieldActor *rock;
  struct FieldActor *leader;
  s32 targetHeight;
  s32 walkZ;
  struct AnimationObject *rockSprite;

  if (GameFlag_IsSet(FLAG_VALE_STORM_FIRST_ROCK_PASSED) != 0) {
  } else {
    Engine_EventBegin();
    if (GameFlag_IsSet(FLAG_VALE_STORM_FIRST_ROCK_FELL) == 0) {
      rock = Object_GetById(ACTOR_FIRST_ROCK);
      rockSprite = rock->sprite;
      // A rumble (SE_141, SE_145), then the rock bounces down from the east.
      Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      Engine_AudioPlayCue(SE_141);
      Engine_TaskWait(40);
      Engine_AudioPlayCue(SE_145);
      rock->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      rockSprite->image.priority = 1;
      Actor_SetPosition(ACTOR_FIRST_ROCK, PIXELS(473), PIXELS(932));
      rock->speed = PIXELS(3) / 2;
      rock->acceleration = PIXELS(3) / 2;
      targetHeight = (rock->y.fixed + PIXELS(240));
      rock->y.fixed += PIXELS(240);
      rock->targetY = targetHeight;
      rock->bounce = 0x6666;
      Actor_WalkToAndWait(ACTOR_FIRST_ROCK, 344, 932);
      rockSprite->image.priority = 3;
      rock->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      Engine_EventWait(40);
      // It lands (SE_289) and the shaking dies down.
      Engine_AudioPlayCue(SE_289);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      Engine_MapWaitWorkValuesBelow256();
      BattleFx_PlayQueuedSound();
      Engine_GameFlagSet(FLAG_VALE_STORM_FIRST_ROCK_FELL);
    }
    // The rock now blocks the path.
    ValeStorm_ApplyFirstRockCollision();
    Engine_GameFlagSet(FLAG_VALE_STORM_FIRST_ROCK_PASSED);
    if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_JOINED) != 0) {
      if (Engine_GameFlagIsSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) == 0) {
        if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH) == 0) {
          // Garet comes out beside Isaac on whichever side of the rock he
          // stands.
          leader = Object_GetById(ACTOR_PARTY_LEADER);
          if (leader->y.fixed > PIXELS(128)) {
            walkZ = 0x396;
            ValeStorm_GaretCommentsOnBlockedPath(0x146, walkZ);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 291, walkZ);
          } else {
            ValeStorm_GaretCommentsOnBlockedPath(0x14f, 0x3bd);
          }
          Engine_GameFlagSet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH);
        }
      }
    }
    Engine_EventEnd();
  }
}

/// Applies the collision cells left by the first rock (ACTOR_FIRST_ROCK).
/// Called by Event_FirstRockFalls and, once the rock has fallen, by
/// ValeStorm_Initialize.
static void ValeStorm_ApplyFirstRockCollision(void) {
  Map_CopyCellAttributes(29, 64, 1, 1, 21, 57);
  Map_CopyCellAttributes(29, 64, 1, 1, 21, 58);
  Map_CopyCellAttributes(29, 64, 1, 1, 22, 58);
  Map_CopyCellAttributes(29, 64, 1, 1, 20, 58);
  Map_CopyCellAttributes(28, 20, 1, 1, 20, 57);
}

/// In Vale on the stormy night, walking past: the second rock
/// (ACTOR_FIRST_ROCK + 1) falls down the path.
/// Reached by stepping on cell code 42; as Event_FirstRockFalls does the first,
/// with FLAG_VALE_STORM_SECOND_ROCK_FELL and
/// FLAG_VALE_STORM_SECOND_ROCK_PASSED.
static void Event_SecondRockFalls(void) {
  struct FieldActor *rock;
  struct FieldActor *leader;
  s32 targetHeight;

  if (GameFlag_IsSet(FLAG_VALE_STORM_SECOND_ROCK_PASSED) != 0) {
  } else {
    Engine_EventBegin();
    if (GameFlag_IsSet(FLAG_VALE_STORM_SECOND_ROCK_FELL) == 0) {
      rock = Object_GetById(ACTOR_FIRST_ROCK + 1);
      // A rumble, then the rock rolls down the slope from the north-east.
      Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      Engine_AudioPlayCue(SE_141);
      Engine_TaskWait(40);
      Engine_AudioPlayCue(SE_145);
      Actor_SetPosition(ACTOR_FIRST_ROCK + 1, PIXELS(381), PIXELS(808));
      rock->speed = PIXELS(3) / 2;
      rock->acceleration = PIXELS(3) / 2;
      targetHeight = (rock->y.fixed + PIXELS(256));
      rock->y.fixed += PIXELS(256);
      rock->targetY = targetHeight;
      rock->bounce = 0x8000;
      Actor_WalkToAndWait(ACTOR_FIRST_ROCK + 1, 290, 833);
      Engine_ActorSetSpritePriority(ACTOR_FIRST_ROCK + 1, 1);
      Actor_WalkToAndWait(ACTOR_FIRST_ROCK + 1, 258, 852);
      Engine_ActorSetSpritePriority(ACTOR_FIRST_ROCK + 1, 2);
      Actor_WalkToAndWait(ACTOR_FIRST_ROCK + 1, 224, 872);
      Engine_EventWait(40);
      // It lands (SE_289) and the shaking dies down.
      Engine_AudioPlayCue(SE_289);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      Engine_MapWaitWorkValuesBelow256();
      BattleFx_PlayQueuedSound();
      Engine_GameFlagSet(FLAG_VALE_STORM_SECOND_ROCK_FELL);
    }
    ValeStorm_ApplySecondRockCollision();
    Engine_GameFlagSet(FLAG_VALE_STORM_SECOND_ROCK_PASSED);
    if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_JOINED) != 0) {
      if (Engine_GameFlagIsSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) == 0) {
        if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH) == 0) {
          // Garet comes out beside Isaac on whichever side of the rock he
          // stands.
          leader = Object_GetById(ACTOR_PARTY_LEADER);
          if (leader->y.fixed > PIXELS(128)) {
            ValeStorm_GaretCommentsOnBlockedPath(219, 0x34b);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 179, 829);
          } else {
            ValeStorm_GaretCommentsOnBlockedPath(214, 0x38c);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 219, 911);
          }
          Engine_GameFlagSet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH);
        }
      }
    }
    Engine_EventEnd();
  }
}

/// Applies the collision cells left by the second rock (ACTOR_FIRST_ROCK + 1).
/// Called by Event_SecondRockFalls and, once the rock has fallen, by
/// ValeStorm_Initialize.
static void ValeStorm_ApplySecondRockCollision(void) {
  Map_CopyCellAttributes(29, 23, 1, 1, 15, 53);
  Map_CopyCellAttributes(29, 23, 1, 1, 14, 53);
  Map_CopyCellAttributes(29, 23, 1, 1, 13, 53);
  Map_CopyCellAttributes(26, 20, 2, 1, 14, 52);
  Map_CopyCellAttributes(25, 21, 1, 1, 13, 54);
  Map_CopyCellAttributes(25, 21, 1, 1, 15, 54);
  Map_CopyCellAttributes(14, 53, 1, 1, 14, 54);
  Map_CopyCellAttributes(13, 55, 1, 1, 15, 55);
}

/// In Vale on the stormy night, walking past: the third rock
/// (ACTOR_FIRST_ROCK + 2) falls down the path.
/// Reached by stepping on cell code 43; as Event_FirstRockFalls does the first,
/// with FLAG_VALE_STORM_THIRD_ROCK_FELL and
/// FLAG_VALE_STORM_THIRD_ROCK_PASSED.
static void Event_ThirdRockFalls(void) {
  if (GameFlag_IsSet(FLAG_VALE_STORM_THIRD_ROCK_PASSED) == 0) {
    Engine_EventBegin();
    if (GameFlag_IsSet(FLAG_VALE_STORM_THIRD_ROCK_FELL) == 0) {
      struct FieldActor *actor = Actor_Get(ACTOR_FIRST_ROCK + 2);
      struct FieldActor *leader = Actor_Get(ACTOR_PARTY_LEADER);
      u16 priority = leader->sprite->image.priority;
      u8 flags = leader->priorityFlags;

      // A rumble, then the rock drops in from the west while Isaac is drawn
      // behind it.
      Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      Event_PlayCue(SE_141);
      Engine_TaskWait(40);
      Event_PlayCue(SE_145);
      Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
      Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      Actor_SetPosition(ACTOR_FIRST_ROCK + 2, 0, PIXELS(703));
      actor->speed = PIXELS(3) / 2;
      actor->acceleration = PIXELS(3) / 2;
      actor->y.fixed += PIXELS(80);
      actor->targetY = actor->y.fixed;
      actor->bounce = 0x8000;
      Actor_WalkToAndWait(ACTOR_FIRST_ROCK + 2, 64, 703);
      Engine_EventWait(40);
      // It lands (SE_289); Isaac's drawing priority is put back.
      Event_PlayCue(SE_289);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      Engine_MapWaitWorkValuesBelow256();
      BattleFx_PlayQueuedSound();
      GameFlag_Set(FLAG_VALE_STORM_THIRD_ROCK_FELL);
      Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, priority);
      Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      leader->priorityFlags = flags;
    }
    ValeStorm_ApplyThirdRockCollision();
    GameFlag_Set(FLAG_VALE_STORM_THIRD_ROCK_PASSED);
    if (GameFlag_IsSet(FLAG_VALE_STORM_GARET_JOINED) != 0) {
      if (GameFlag_IsSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) == 0) {
        if (GameFlag_IsSet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH) == 0) {
          // Garet comes out beside Isaac on whichever side of the rock he
          // stands.
          if (Actor_Get(ACTOR_PARTY_LEADER)->z.fixed <= 0x2b4ffff) {
            ValeStorm_GaretCommentsOnBlockedPath(62, 0x29d);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 27, 627);
          } else {
            ValeStorm_GaretCommentsOnBlockedPath(75, 0x2cb);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 67, 757);
          }
          GameFlag_Set(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH);
        }
      }
    }
    Engine_EventEnd();
  }
}

/// Applies the collision cells left by the third rock (ACTOR_FIRST_ROCK + 2).
/// Called by Event_ThirdRockFalls and, once the rock has fallen, by
/// ValeStorm_Initialize.
static void ValeStorm_ApplyThirdRockCollision(void) {
  Map_CopyCellAttributes(29, 22, 1, 1, 3, 42);
  Map_CopyCellAttributes(29, 21, 1, 1, 2, 42);
  Map_CopyCellAttributes(29, 21, 1, 1, 4, 42);
  Map_CopyCellAttributes(23, 20, 3, 1, 2, 43);
}

/// In Vale on the stormy night, walking past: the fourth rock
/// (ACTOR_FIRST_ROCK + 3) falls down the path.
/// Reached by stepping on cell code 44; as Event_FirstRockFalls does the first,
/// with FLAG_VALE_STORM_FOURTH_ROCK_FELL and
/// FLAG_VALE_STORM_FOURTH_ROCK_PASSED.
static void Event_FourthRockFalls(void) {
  struct FieldActor *rock;
  struct FieldActor *leader;
  s32 targetHeight;

  if (GameFlag_IsSet(FLAG_VALE_STORM_FOURTH_ROCK_PASSED) != 0) {
  } else {
    Engine_EventBegin();
    if (GameFlag_IsSet(FLAG_VALE_STORM_FOURTH_ROCK_FELL) == 0) {
      rock = Object_GetById(ACTOR_FIRST_ROCK + 3);
      // A rumble, then the rock rolls in from the east.
      Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      Engine_AudioPlayCue(SE_141);
      Engine_TaskWait(40);
      Engine_AudioPlayCue(SE_145);
      Actor_SetPosition(ACTOR_FIRST_ROCK + 3, PIXELS(474), PIXELS(1147));
      rock->speed = PIXELS(1);
      rock->acceleration = PIXELS(1);
      targetHeight = (rock->y.fixed + PIXELS(72));
      rock->y.fixed += PIXELS(72);
      rock->targetY = targetHeight;
      rock->bounce = 0x8000;
      Engine_ActorWalkToAndWait(ACTOR_FIRST_ROCK + 3, 432, 1147);
      Engine_EventWait(40);
      // It lands (SE_289) and the shaking dies down.
      Engine_AudioPlayCue(SE_289);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      Engine_MapWaitWorkValuesBelow256();
      BattleFx_PlayQueuedSound();
      Engine_GameFlagSet(FLAG_VALE_STORM_FOURTH_ROCK_FELL);
    }
    ValeStorm_ApplyFourthRockCollision();
    Engine_GameFlagSet(FLAG_VALE_STORM_FOURTH_ROCK_PASSED);
    if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_JOINED) != 0) {
      if (Engine_GameFlagIsSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) == 0) {
        if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH) == 0) {
          // Garet comes out beside Isaac on whichever side of the rock he
          // stands.
          leader = Object_GetById(ACTOR_PARTY_LEADER);
          if (leader->z.fixed <= 0x479ffff) {
            ValeStorm_GaretCommentsOnBlockedPath(0x19c, 0x460);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 414, 1068);
          } else {
            ValeStorm_GaretCommentsOnBlockedPath(0x1bd, 0x494);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 447, 1227);
          }
          Engine_GameFlagSet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH);
        }
      }
    }
    Engine_EventEnd();
  }
}

/// Applies the collision cells left by the fourth rock (ACTOR_FIRST_ROCK + 3).
/// Called by Event_FourthRockFalls and, once the rock has fallen, by
/// ValeStorm_Initialize.
static void ValeStorm_ApplyFourthRockCollision(void) {
  Map_CopyCellAttributes(29, 20, 1, 1, 26, 71);
  Map_CopyCellAttributes(29, 20, 1, 1, 26, 70);
  Map_CopyCellAttributes(29, 20, 1, 1, 27, 70);
  Map_CopyCellAttributes(28, 21, 1, 1, 28, 71);
  Map_CopyCellAttributes(28, 22, 1, 1, 27, 72);
}

/// Stops the leader at the door of VALE_STORM_TO_VALE_INTERIORS_SHOPS_SOUTH for
/// the first conversation of the night. A stout man runs out of the door,
/// knocks Isaac back and runs off to the plaza. "Huh? Isaac?" Called by
/// Event_LeaveToValeInteriorsShops. Sets FLAG_VALE_STORM_BUMPED_INTO_VILLAGER.
static void ValeStorm_BumpIntoShovingMan(void) {
  s32 message;

  Engine_EventBegin();
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 262, 810);
  Actor_SetPosition(ACTOR_SHOVING_MAN, PIXELS(262), PIXELS(805));
  Actor_WalkTo(ACTOR_SHOVING_MAN, 262, 825);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 282, 855);
  Engine_ActorSetAnimation(ACTOR_SHOVING_MAN, ANIM_STAND);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_SHOVING_MAN, 0);
  BattleFx_PlayQueuedSound();
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_ShowEmote(ACTOR_SHOVING_MAN, EMOTE_IN_FRONT | 0, 20);
  message = MsgValeHuh;
  Engine_EventSetMessage(message);
  Event_ShowMessage(ACTOR_SHOVING_MAN, 0);
  Engine_EventWait(20);
  Event_AskYesNo(ACTOR_SHOVING_MAN, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_SHOVING_MAN, 2);
  Engine_EventSetMessage((message + 4));
  Event_ShowMessageAndWait(ACTOR_SHOVING_MAN, 0, 20);
  Actor_RunActionScriptAndWait(ACTOR_SHOVING_MAN, gValeStorm_ShovingManScript);
  GameFlag_Set(FLAG_VALE_STORM_BUMPED_INTO_VILLAGER);
  Engine_EventEnd();
}

/// In Vale on the stormy night, Isaac hears Garet struggling with his things.
/// "Ugh... Hrnghhh..."
/// Reached by stepping on cell code 30, until Garet has joined. Sets
/// FLAG_VALE_STORM_HEARD_GARET.
static void Event_HearGaretStruggling(void) {
  if (GameFlag_IsSet(FLAG_VALE_STORM_HEARD_GARET) == 0) {
    if (GameFlag_IsSet(FLAG_VALE_STORM_GARET_JOINED) == 0) {
      Engine_EventBegin();
      Engine_EventSetMessage(MsgValeUghHrnghhh);
      Event_ShowMessageAndWait(ACTOR_GARET, 0, 20);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 40);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 382, 619);
      Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
      Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Engine_EventWait(30);
      Event_ShowMessage(ACTOR_GARET, 0);
      GameFlag_Set(FLAG_VALE_STORM_HEARD_GARET);
      Engine_EventEnd();
    }
  }
}

/// In Vale on the stormy night, walking away from him: Garet calls after
/// Isaac, asking whether he will just leave him there.
/// Reached by stepping on cell code 31 once Isaac has heard Garet and before
/// Garet joins; ValeStorm_RunCallOutSequence brings him along.
static void Event_GaretCallsAfterIsaac(void) {
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_JOINED) == 0 &&
      Engine_GameFlagIsSet(FLAG_VALE_STORM_HEARD_GARET) != 0) {
    Engine_EventBegin();
    Engine_ActorRunRepeatedMotion(ACTOR_GARET, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgValeDontLeaveMeHere);
    ValeStorm_RunCallOutSequence();
    Engine_EventEnd();
  }
}

/// In Vale on the stormy night, talking to Garet: whether to leave his things
/// behind, or, after the river, the Boulder.
/// Reached by talking to Garet: before he joins, ValeStorm_RunCallOutSequence
/// brings him along.
static void Event_TalkToGaret(void) {
  if (GameFlag_IsSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) != 0) {
    Engine_EventBegin();
    Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgValeBoulderNeedGet);
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
    Engine_EventEnd();
  } else {
    if (GameFlag_IsSet(FLAG_VALE_STORM_GARET_JOINED) == 0) {
      Engine_EventBegin();
      Engine_EventSetMessage(MsgValeWantDumpStuff);
      ValeStorm_RunCallOutSequence();
      Engine_EventEnd();
    }
  }
}

/// Resolves the belongings conversation and brings Garet into the party.
/// Called by Event_GaretCallsAfterIsaac and Event_TalkToGaret. Sets
/// FLAG_VALE_STORM_GARET_JOINED and puts Garet in the party.
static void ValeStorm_RunCallOutSequence(void) {
  struct FieldActor *leader;
  s32 agreedToLeaveBelongings;

  Engine_EventOpenMessage(ACTOR_GARET, 0);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  agreedToLeaveBelongings = 0;
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventSetMessage(MsgValeThinkForgetThings);
    agreedToLeaveBelongings = 1;
  } else {
    Engine_EventSetMessage(MsgValeRockHitsLose);
  }
  Engine_EventWait(20);
  Engine_EventShowMessageAndWait(ACTOR_GARET, 0, 40);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  Engine_ActorSetAnimation(ACTOR_GARETS_CHEST, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Engine_EventWait(40);
  Engine_ActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  if (agreedToLeaveBelongings != 0) {
    Engine_EventSetMessage(MsgValeKnowRightOk);
  } else {
    Engine_EventSetMessage(MsgValeRightDitchStuff);
  }
  Engine_EventShowMessage(ACTOR_GARET, 0);
  Engine_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(ACTOR_GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_GARET);
  Engine_ActorSetPosition(ACTOR_GARET, 0, 0);
  Event_PrepareObjectAndApplyValue(1, 1);
  Engine_GameFlagSet(FLAG_VALE_STORM_GARET_JOINED);
}

/// Unused: an earlier version of Garet joining Isaac on the stormy night,
/// which the game never plays.
///
/// Its row answers cell code 32, but every way there crosses the cells where
/// Isaac hears Garet struggling (code 30, Event_HearGaretStruggling) and where
/// Garet calls after him and joins (code 31, Event_GaretCallsAfterIsaac), so
/// Garet has always joined by then and the scene's first test,
/// FLAG_VALE_STORM_GARET_JOINED, ends it. Its lines appear nowhere else: Garet
/// runs up (gValeStorm_UnusedGaretRunsUpScript), heaves for breath, `*Hah*
/// *Hah*`, while gValeStorm_UnusedGaretPantingScript swells and shrinks his
/// sprite, and asks `Are you trapped, too?` before they leave together.
static void Event_UnusedGaretJoinsIsaac(void) {
  struct FieldActor *actor;
  s32 message;

  if (GameFlag_IsSet(FLAG_VALE_STORM_GARET_JOINED) == 0) {
    Engine_EventBegin();
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
    message = MsgValeHey;
    Engine_EventSetMessage(message);
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
    Camera_MoveTo(PIXELS(256), -1, PIXELS(588), 1);
    Actor_SetSpeed(ACTOR_GARET, PIXELS(2), PIXELS(1));
    Actor_RunActionScriptAndWait(ACTOR_GARET,
                                 gValeStorm_UnusedGaretRunsUpScript);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    Engine_EventWait(30);
    Actor_SetActionScript(ACTOR_GARET, gValeStorm_UnusedGaretPantingScript);
    Event_ShowMessage(ACTOR_GARET, 0);
    actor = Actor_Get(ACTOR_GARET);
    actor->scaleY = Q16_ONE;
    Engine_ActorRunRepeatedMotion(ACTOR_GARET, 1);
    Engine_EventWait(20);
    Event_AskYesNo(ACTOR_GARET, 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_GARET, 1);
    Engine_EventSetMessage((message + 5));
    Event_ShowMessageAndWait(ACTOR_GARET, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
    Engine_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Actor_SetDestination(ACTOR_GARET, actor->x.part.pixel,
                           actor->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_GARET);
    Actor_SetPosition(ACTOR_GARET, 0, 0);
    Event_PrepareObjectAndApplyValue(1, 1);
    Engine_ActorSetAnimation(ACTOR_GARETS_CHEST, ANIM_NOD);
    GameFlag_Set(FLAG_VALE_STORM_GARET_JOINED);
    Engine_EventEnd();
  }
}

/// In Vale on the stormy night, Garet turns Isaac back from the path north,
/// which leads to Mt. Aleph.
/// Reached by stepping on cell code 33, every time; Isaac is walked back south.
static void Event_WarnAboutMtAleph(void) {
  struct FieldActor *leader;
  struct FieldActor *initialLeader;

  Engine_EventBegin();
  initialLeader = Actor_Get(ACTOR_PARTY_LEADER);
  if (initialLeader != NULL) {
    Actor_SetPosition(ACTOR_GARET, initialLeader->x.fixed,
                      initialLeader->z.fixed);
  }
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_GARET, 281, 507);
  Engine_ActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(30);
  Engine_EventSetMessage(MsgValeNorthLeadsToMtAleph);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Event_ShowMessage(ACTOR_GARET, 0);
  Engine_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 256, 517);
  Engine_EventEnd();
}

/// Brings Garet out to discuss a blocked path, then returns him to Isaac.
/// Called by the four rock events; x and y are the pixels where Garet stands.
static void ValeStorm_GaretCommentsOnBlockedPath(s32 x, s32 y) {
  struct FieldActor *leaderAtStart;
  s32 speed = PIXELS(1);
  s32 acceleration = (PIXELS(1) / 2);
  struct FieldActor *leaderAtEnd;

  leaderAtStart = Object_GetById(ACTOR_PARTY_LEADER);
  if (leaderAtStart != 0) {
    Engine_ActorSetPosition(ACTOR_GARET, leaderAtStart->x.fixed,
                            leaderAtStart->z.fixed);
  }
  Engine_ActorSetSpeed(ACTOR_GARET, speed, acceleration);
  Engine_ActorWalkToAndWait(ACTOR_GARET, x, y);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Engine_EventWait(20);
  Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                        ATTACHED_EFFECT_ONCE);
  Engine_EventWait(40);
  Engine_EventSetMessage(MsgValeCantGetAroundThisRock);
  Engine_EventShowMessage(ACTOR_GARET, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_GARET, 2);
  Engine_EventShowMessage(ACTOR_GARET, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  leaderAtEnd = Object_GetById(ACTOR_PARTY_LEADER);
  if (leaderAtEnd != 0) {
    Engine_ActorSetDestination(ACTOR_GARET, leaderAtEnd->x.part.pixel,
                               leaderAtEnd->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_GARET);
  Engine_ActorSetPosition(ACTOR_GARET, 0, 0);
}

/// In Vale on the stormy night, the Boulder slips toward the village while the
/// elders hold it with their Psynergy, and they tell the boys to run.
/// "Ahhh! The Boulder! It's falling!"
/// Reached by stepping on cell code 34 until FLAG_VALE_BOULDER_FELL is set,
/// which this sets. The elders and their two stand-ins
/// (gValeStorm_BoulderWatchers, ACTOR_ELDER_EAST_STAND_IN to ACTOR_ELDER_WEST)
/// are placed for the scene and removed after it.
static void Event_WatchBoulderFall(void) {
  u32 i;
  struct FieldActor *movingActor;
  s32 *shakeShift;
  struct FieldActor *actor;
  SchedulerCallback updateEffects;
  SchedulerCallback cycleBoulderPalette;
  s32 boulderHeight;

  if (GameFlag_IsSet(FLAG_VALE_BOULDER_FELL) == 0) {
    Engine_EventBegin();
    Event_CallWithLastActiveObjectId(gValeStorm_BoulderWatchers);
    Scene_BeginCutscenePsynergy();
    Engine_TaskWait(1);
    Event_PlayCue(SE_141);
    Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Engine_EventWait(30);
    Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
    Event_PlayCue(SE_145);
    Engine_EventWait(30);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetPosition(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
    Actor_SetSpeed(ACTOR_GARET, PIXELS(2), PIXELS(1));
    Actor_SetActionScript(ACTOR_PARTY_LEADER, gValeStorm_LeaderScript);
    Actor_RunActionScriptAndWait(ACTOR_GARET, gValeStorm_GaretScript);
    Object_RefreshSelectorById(ACTOR_PARTY_LEADER);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 0, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | 0, 30);
    Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
    Event_PlayCue(SE_145);
    Engine_EventWait(40);
    Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
    Event_PlayCue(SE_145);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_EventWait(40);
    // Show the four elders holding back the Boulder.
    Engine_ActorSetAnimation(ACTOR_ELDER_EAST, 5);
    Engine_ActorSetAnimation(ACTOR_ELDER_WEST, 5);
    Engine_ActorSetAnimation(ACTOR_ELDER_FAR_EAST, 8);
    Engine_ActorSetAnimation(ACTOR_ELDER_FAR_WEST, 8);
    Actor_Get(ACTOR_ELDER_FAR_EAST)->scaleX = -Q16_ONE;
    Engine_ActorSetSpritePriority(ACTOR_ELDER_EAST, 2);
    Engine_ActorSetSpritePriority(ACTOR_ELDER_WEST, 2);
    Engine_ActorSetSpritePriority(ACTOR_ELDER_FAR_EAST, 3);
    Engine_ActorSetSpritePriority(ACTOR_ELDER_FAR_WEST, 3);
    Engine_EventSetMessage(MsgValeTheBoulderIsFalling);
    Event_ShowMessageAndWait(ACTOR_ELDER_EAST_STAND_IN, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 20);
    Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
    Camera_MoveTo(PIXELS(112), -1, PIXELS(331), 1);
    Engine_CameraWaitForMove();
    for (i = 0; i < 40; i++) {
      ValeStorm_FlashPaletteAndSpawnEffects(Actor_Get(ACTOR_ELDER_EAST));
      ValeStorm_FlashPaletteAndSpawnEffects(Actor_Get(ACTOR_ELDER_WEST));
      ValeStorm_FlashPaletteAndSpawnEffects(Actor_Get(ACTOR_ELDER_FAR_EAST));
      ValeStorm_FlashPaletteAndSpawnEffects(Actor_Get(ACTOR_ELDER_FAR_WEST));
      Engine_TaskWait(1);
    }
    // Run the palette effects while the boulder rocks back and forth.
    shakeShift = &sShakeShift;
    updateEffects = ValeStorm_UpdateBoulderSceneEffects;
    sShakeDone = 0;
    *shakeShift = 0;
    FieldScheduler_AddOrUpdateCallback(updateEffects, STORM_CALLBACK_PRIORITY);
    cycleBoulderPalette = ValeStorm_CycleBoulderPalette;
    FieldScheduler_AddCallback(cycleBoulderPalette, STORM_CALLBACK_PRIORITY);
    Engine_EventWait(40);
    *shakeShift = 1;
    Engine_EventWait(30);
    Actor_SetPosition(ACTOR_BOULDER, PIXELS(114), PIXELS(290));
    movingActor = Actor_Get(ACTOR_BOULDER);
    boulderHeight = movingActor->y.fixed + PIXELS(64);
    movingActor->y.fixed = boulderHeight;
    movingActor->targetY = boulderHeight;
    Actor_SetSpeed(ACTOR_BOULDER, PIXELS(4) / 5, PIXELS(2) / 5);
    Event_PlayCue(SE_145);
    Actor_MoveToAndWait(ACTOR_BOULDER, 114, 333);
    Engine_ActorSetAnimation(ACTOR_BOULDER, ANIM_WALK);
    Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Event_PlayCue(SE_145);
    *shakeShift = 0;
    Actor_SetSpeed(ACTOR_BOULDER, PIXELS(2) / 5, PIXELS(1) / 5);
    Actor_MoveToAndWait(ACTOR_BOULDER, 114, 300);
    Engine_ActorSetAnimation(ACTOR_BOULDER, ANIM_WALK);
    Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
    Event_PlayCue(SE_145);
    *shakeShift = 2;
    Actor_SetSpeed(ACTOR_BOULDER, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_MoveToAndWait(ACTOR_BOULDER, 114, 333);
    Engine_ActorSetAnimation(ACTOR_BOULDER, ANIM_WALK);
    Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Event_PlayCue(SE_145);
    *shakeShift = 0;
    Actor_SetSpeed(ACTOR_BOULDER, PIXELS(2) / 5, PIXELS(1) / 5);
    Actor_MoveToAndWait(ACTOR_BOULDER, 114, 300);
    Engine_ActorSetAnimation(ACTOR_BOULDER, ANIM_WALK);
    Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
    Event_PlayCue(SE_145);
    *shakeShift = 2;
    Actor_SetSpeed(ACTOR_BOULDER, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_MoveToAndWait(ACTOR_BOULDER, 114, 333);
    Engine_ActorSetAnimation(ACTOR_BOULDER, ANIM_WALK);
    Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Event_PlayCue(SE_145);
    *shakeShift = 1;
    Engine_EventWait(20);
    Actor_SetAttachedEffect(ACTOR_ELDER_EAST,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ActorRunRepeatedMotion(ACTOR_ELDER_EAST, 2);
    Event_ShowMessage(ACTOR_ELDER_WEST_STAND_IN, 0);
    Actor_ShowEmote(ACTOR_ELDER_WEST, EMOTE_IN_FRONT | 0, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_ELDER_WEST, 2);
    Event_ShowMessageAndWait(ACTOR_ELDER_EAST_STAND_IN, 0, 40);
    Actor_SetAttachedEffect(ACTOR_ELDER_FAR_EAST,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ActorRunRepeatedMotion(ACTOR_ELDER_FAR_EAST, 2);
    Event_ShowMessage(ACTOR_ELDER_FAR_EAST, 0);
    // The westernmost elder stops straining, looks about and calls to the
    // boys, and the camera returns to Isaac.
    sShakeDone = 1;
    Engine_ActorSetAnimation(ACTOR_ELDER_FAR_WEST, ANIM_STAND);
    Engine_TaskWait(1);
    Actor_SetChildValue(ACTOR_ELDER_FAR_WEST, 0);
    Actor_ShowEmote(ACTOR_ELDER_FAR_WEST, EMOTE_IN_FRONT | 5, 20);
    Actor_FaceDirection(ACTOR_ELDER_FAR_WEST, FACING_WEST, 40);
    Actor_FaceDirection(ACTOR_ELDER_FAR_WEST, FACING_EAST, 20);
    Actor_FaceDirection(ACTOR_ELDER_FAR_WEST, FACING_WEST, 20);
    Actor_FaceDirection(ACTOR_ELDER_FAR_WEST, FACING_SOUTH, 40);
    Actor_ShowEmote(ACTOR_ELDER_FAR_WEST, EMOTE_IN_FRONT | 0, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_ELDER_FAR_WEST, 2);
    Engine_ActorJump(ACTOR_ELDER_FAR_WEST, 4, 40);
    Engine_ActorSetAnimation(ACTOR_ELDER_FAR_WEST, 9);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_ELDER_FAR_WEST, 0, 20);
    Event_PlayCue(SE_289);
    Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
    Camera_SetSpeed(PIXELS(6), PIXELS(3) / 4);
    Camera_MoveTo(PIXELS(84), -1, PIXELS(564), 1);
    Engine_CameraWaitForMove();
    BattleFx_PlayQueuedSound();
    Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_EventWait(30);
    Scheduler_RemoveCallback(updateEffects);
    Scheduler_RemoveCallback(cycleBoulderPalette);
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    Engine_EventWait(20);
    Scene_EndCutscenePsynergy();
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
    movingActor = Object_GetById(ACTOR_PARTY_LEADER);
    if (movingActor != 0) {
      Actor_SetDestination(ACTOR_GARET, movingActor->x.part.pixel,
                           movingActor->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_GARET);
    Actor_SetPosition(ACTOR_GARET, 0, 0);
    Engine_ActorDestroy(ACTOR_ELDER_WEST_STAND_IN);
    Engine_ActorDestroy(ACTOR_ELDER_EAST_STAND_IN);
    Engine_ActorDestroy(ACTOR_ELDER_FAR_EAST);
    Engine_ActorDestroy(ACTOR_ELDER_FAR_WEST);
    Engine_ActorDestroy(ACTOR_ELDER_EAST);
    Engine_ActorDestroy(ACTOR_ELDER_WEST);
    GameFlag_Set(FLAG_VALE_BOULDER_FELL);
    Engine_EventEnd();
  }
}

/// Applies an animation parameter to each active entry of a single sprite.
static void Storm_SetActorFrameMode(struct FieldActor *actor, s32 mode) {
  if ((actor->spriteStorageKind & 0xf) == 1) {
    struct AnimationObject *animation = actor->sprite;
    s32 animationParameter = mode - 1;
    u8 count;
    if (mode == 0) {
      animationParameter =
          gValeStorm_FrameModes[(gFrameCount >> 1) &
                                (actor->spriteStorageKind & 0xf)];
    }
    count = animation->count;
    if (count != 0) {
      struct AnimationEntry **entries = animation->entries;
      s32 remainingEntries = count;
      do {
        struct AnimationEntry *entry = *entries++;
        if (entry != NULL && entry->script != NULL) {
          entry->param = animationParameter;
        }
        remainingEntries--;
      } while (remainingEntries != 0);
    }
    animation->dirty = 1;
  }
}

/// Unused: alternates the four elders between their normal and flashing frame
/// modes; nothing calls it.
UNUSED static void ValeStorm_FlashBoulderSceneAnimationEntries(void) {
  if (((gFrameCount >> sShakeShift) & 3) != 0) {
    Storm_SetActorFrameMode(Object_GetById(ACTOR_ELDER_EAST), 1);
    Storm_SetActorFrameMode(Object_GetById(ACTOR_ELDER_WEST), 1);
    Storm_SetActorFrameMode(Object_GetById(ACTOR_ELDER_FAR_EAST), 1);
    Storm_SetActorFrameMode(Object_GetById(ACTOR_ELDER_FAR_WEST), 1);
  } else {
    Storm_SetActorFrameMode(Object_GetById(ACTOR_ELDER_EAST), 8);
    Storm_SetActorFrameMode(Object_GetById(ACTOR_ELDER_WEST), 8);
    Storm_SetActorFrameMode(Object_GetById(ACTOR_ELDER_FAR_EAST), 8);
    Storm_SetActorFrameMode(Object_GetById(ACTOR_ELDER_FAR_WEST), 8);
  }
}

/// In Vale on the stormy night, at the river, Jenna cries out for her brother
/// Felix, who has fallen in and clings on as Kyle and her parents try to save
/// him. "Oh no! My brother!" The rope will not reach and no one has Psynergy
/// left, so they send for help.
/// Reached by stepping on cell code 35 until
/// FLAG_VALE_STORM_SAW_FELIX_HANGING_ON is set, which this sets after
/// ValeStorm_RunRiverSearch.
static void Event_WatchAttemptToSaveFelix(void) {
  const s32 *pulseScript;

  if (GameFlag_IsSet(FLAG_VALE_STORM_SAW_FELIX_HANGING_ON) != 0) {
    return;
  }
  Engine_EventBegin();
  Actor_SetPosition(ACTOR_KYLE, PIXELS(192), PIXELS(1214));
  Actor_FaceDirection(ACTOR_KYLE, FACING_SOUTHEAST, 0);
  Engine_ActorSetAnimation(ACTOR_KYLE, 5);
  {
    struct FieldActor *actor;
    s32 pulseDelay;
    actor = Object_GetById(ACTOR_KYLE);
    pulseDelay = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    pulseScript = gValeStorm_SquashAndStretchScript;
    actor->timer = pulseDelay;
    Actor_SetActionScript(ACTOR_KYLE, pulseScript);
  }
  Actor_SetPosition(ACTOR_DORA, PIXELS(192), PIXELS(1189));
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST, 0);
  Actor_SetPosition(ACTOR_JENNAS_FATHER, PIXELS(227), PIXELS(1214));
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_SOUTH, 0);
  Engine_ActorSetAnimation(ACTOR_JENNAS_FATHER, 6);
  {
    struct FieldActor *actor;
    s32 pulseDelay;
    actor = Object_GetById(ACTOR_JENNAS_FATHER);
    pulseDelay = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    actor->timer = pulseDelay;
    Actor_SetActionScript(ACTOR_JENNAS_FATHER, pulseScript);
  }
  Actor_SetPosition(ACTOR_JENNAS_MOTHER, PIXELS(250), PIXELS(1214));
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_SOUTH, 0);
  Engine_ActorSetAnimation(ACTOR_JENNAS_MOTHER, 6);
  {
    struct FieldActor *actor;
    s32 pulseDelay;
    actor = Object_GetById(ACTOR_JENNAS_MOTHER);
    pulseDelay = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    actor->timer = pulseDelay;
    Actor_SetActionScript(ACTOR_JENNAS_MOTHER, pulseScript);
  }
  Actor_SetPosition(ACTOR_JENNA, PIXELS(227), PIXELS(1189));
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetPosition(ACTOR_FELIX, PIXELS(243), PIXELS(1277));
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FELIX), 0);
  Engine_TaskWait(3);
  Engine_EventSetMessage(MsgValeNoBrother);
  Engine_EventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 0, 20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 150, 1094);
  {
    struct FieldActor *leader;
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetPosition(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
    }
  }
  Actor_WalkToAndWait(ACTOR_GARET, 132, 1094);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 20);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(216), -1, PIXELS(1232), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_KYLE, 2);
  Engine_EventShowMessageAndWait(ACTOR_KYLE, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 3);
  Engine_ActorFaceDirection(ACTOR_DORA, FACING_EAST, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 10);
  Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
  Camera_MoveTo(PIXELS(232), -1, PIXELS(1253), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_AudioPlayCue(SE_HEAVY_IMPACT);
  Engine_ActorJump(ACTOR_FELIX, 4, 0);
  Engine_ActorSetAnimation(ACTOR_FELIX, 6);
  Engine_EventWait(10);
  Engine_ActorSetPosition(ACTOR_FELIX, 0, 0);
  Engine_EventWait(60);
  BattleFx_PlayQueuedSound();
  Engine_ActorSetAnimation(ACTOR_KYLE, ANIM_STAND);
  {
    struct FieldActor *actor;
    actor = Object_GetById(ACTOR_KYLE);
    actor->scaleX = Q16_ONE;
    actor->scaleY = Q16_ONE;
  }
  Engine_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_STAND);
  {
    struct FieldActor *actor;
    actor = Object_GetById(ACTOR_JENNAS_FATHER);
    actor->scaleX = Q16_ONE;
    actor->scaleY = Q16_ONE;
  }
  Engine_ActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIM_STAND);
  {
    struct FieldActor *actor;
    actor = Object_GetById(ACTOR_JENNAS_MOTHER);
    actor->scaleX = Q16_ONE;
    actor->scaleY = Q16_ONE;
  }
  Engine_ActorStartRepeatedMotion(ACTOR_KYLE, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_JENNAS_FATHER, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_JENNAS_MOTHER, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_JENNA, 2);
  Camera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  Camera_MoveTo(PIXELS(216), -1, PIXELS(1232), 1);
  Engine_CameraWaitForMove();
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ActorSetAttachedEffect(ACTOR_DORA, ATTACHED_EFFECT_IN_FRONT |
                                                ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_JENNA, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_JENNA, 3);
  Engine_EventShowMessage(ACTOR_JENNA, 0);
  Engine_ActorJump(ACTOR_JENNAS_MOTHER, 2, 0);
  Actor_SetDestination(ACTOR_JENNAS_MOTHER, 234, 1205);
  Engine_ActorJump(ACTOR_JENNA, 2, 0);
  Actor_SetDestination(ACTOR_JENNA, 227, 1201);
  Engine_EventWait(90);
  Camera_MoveTo(PIXELS(232), -1, PIXELS(1253), 1);
  Engine_CameraWaitForMove();
  Engine_ActorSetPosition(ACTOR_FELIX, PIXELS(243), PIXELS(1277));
  Engine_TaskWait(1);
  Engine_AudioPlayCue(SE_106);
  {
    struct FieldActor *actor;
    actor = Object_GetById(ACTOR_FELIX);
    actor->velocityY = PIXELS(2);
  }
  Engine_EventWait(6);
  Engine_ActorSetAnimation(ACTOR_FELIX, 7);
  Engine_EventWait(20);
  BattleFx_PlayQueuedSound();
  Engine_EventWait(20);
  Engine_CameraSetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Engine_CameraMoveTo(PIXELS(216), -1, PIXELS(1232), 1);
  Engine_CameraWaitForMove();
  Engine_ActorRunRepeatedMotion(ACTOR_JENNAS_FATHER, 2);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_JENNAS_FATHER, EMOTE_IN_FRONT | 5, 40);
  Engine_ActorFaceEachOther(ACTOR_JENNAS_FATHER, ACTOR_KYLE, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_KYLE, 2);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KYLE, 0);
  {
    struct FieldActor *actor;
    actor = Object_GetById(ACTOR_JENNAS_MOTHER);
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  }
  {
    struct FieldActor *actor;
    actor = Object_GetById(ACTOR_JENNA);
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  }
  Actor_SetSpeed(ACTOR_JENNAS_MOTHER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetDestination(ACTOR_JENNAS_MOTHER, 247, 1210);
  Actor_MoveToAndWait(ACTOR_JENNA, 227, 1189);
  {
    struct FieldActor *actor;
    actor = Object_GetById(ACTOR_JENNAS_MOTHER);
    actor->steeringFlags |= ACTOR_FACES_MOTION;
  }
  Object_GetById(ACTOR_JENNA)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_ActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_WEST, 10);
  Engine_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_SHAKE_HEAD);
  Engine_EventShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNAS_FATHER, 0,
                                 10);
  Actor_FaceDirection(ACTOR_KYLE, FACING_NORTH, 20);
  Engine_ActorFaceDirection(ACTOR_KYLE, FACING_EAST, 10);
  Engine_ActorSetAnimation(ACTOR_KYLE, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KYLE, 0, 10);
  Actor_ShowEmote(ACTOR_JENNAS_FATHER, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(ACTOR_KYLE, EMOTE_IN_FRONT | 5, 60);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 6, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 20);
  Engine_ActorFaceDirection(ACTOR_DORA, FACING_EAST, 30);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 10);
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_KYLE, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_WEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_KYLE, 1);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KYLE, 0, 10);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_ShowEmote(ACTOR_KYLE, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(ACTOR_JENNAS_FATHER, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(ACTOR_JENNAS_MOTHER, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | 5, 40);
  Engine_ActorFaceDirection(ACTOR_DORA, FACING_EAST, 10);
  Engine_ActorFaceEachOther(ACTOR_JENNAS_FATHER, ACTOR_JENNAS_MOTHER, 0);
  Engine_EventWait(20);
  Engine_ActorFaceDirection(ACTOR_DORA, FACING_EAST, 0);
  Engine_ActorFaceDirection(ACTOR_KYLE, FACING_EAST, 10);
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_WEST, 10);
  Engine_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_JENNAS_MOTHER, ANIM_NOD);
  Engine_ActorFaceEachOther(ACTOR_KYLE, ACTOR_DORA, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_KYLE, 1);
  Engine_EventShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KYLE, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, 0xD000, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_JENNAS_FATHER, 1);
  Engine_EventShowMessageAndWait(ACTOR_JENNAS_FATHER, 0, 10);
  Engine_ActorFaceDirection(ACTOR_KYLE, FACING_EAST, 0);
  Engine_ActorFaceDirection(ACTOR_DORA, FACING_EAST, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_JENNA, 1);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_JENNAS_MOTHER, ANIM_NOD);
  Engine_EventShowMessageAndWait(ACTOR_JENNAS_MOTHER, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 10);
  // Search both river paths before asking the children to help.
  ValeStorm_RunRiverSearch();
  Engine_GameFlagSet(FLAG_VALE_STORM_SAW_FELIX_HANGING_ON);
  Engine_EventEnd();
}

/// Dora goes north for help and sends Jenna to the plaza, then asks Isaac and
/// Garet to help: a yes sends them after Jenna, a no to the plaza all the
/// same.
static void ValeStorm_RunRiverSearch(void) {
  struct FieldActor *villager;
  s32 random;
  const s32 *script;
  s32 northMessage;
  s32 helpMessage;
  struct FieldActor *leader;

  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_KYLE, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_JENNAS_MOTHER, ANIM_NOD);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(134), -1, PIXELS(1195), 1);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetActionScript(ACTOR_JENNA, gValeStorm_JennaScriptA);
  Actor_RunActionScriptAndWait(ACTOR_DORA, gValeStorm_DoraScriptA);
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_CellSteps2, 38, 72);
  Engine_EventWait(10);
  Actor_WalkToAndWait(ACTOR_DORA, 149, 1175);
  Actor_SetPosition(ACTOR_DORA, 0, 0);
  Actor_WalkToAndWait(ACTOR_JENNAS_MOTHER, 250, 1214);
  BattleFx_PlayQueuedSound();
  Actor_FaceDirection(ACTOR_KYLE, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorSetAnimation(ACTOR_KYLE, 5);
  Engine_ActorSetAnimation(ACTOR_JENNAS_FATHER, 6);
  Engine_ActorSetAnimation(ACTOR_JENNAS_MOTHER, 6);
  villager = Actor_Get(ACTOR_KYLE);
  random = Engine_RandomNext();
  villager->timer = (Math_RemainderUnsigned(random, 90) + 60);
  villager = Actor_Get(ACTOR_JENNAS_FATHER);
  random = Engine_RandomNext();
  villager->timer = (Math_RemainderUnsigned(random, 90) + 60);
  villager = Actor_Get(ACTOR_JENNAS_MOTHER);
  random = Engine_RandomNext();
  villager->timer = (Math_RemainderUnsigned(random, 90) + 60);
  script = gValeStorm_SquashAndStretchScript;
  Actor_SetActionScript(ACTOR_KYLE, script);
  Actor_SetActionScript(ACTOR_JENNAS_FATHER, script);
  Actor_SetActionScript(ACTOR_JENNAS_MOTHER, script);
  Object_RefreshSelectorById(ACTOR_JENNA);
  Engine_EventWait(10);
  Event_PlayCue(SE_159);
  Map_AnimateCells(gValeStorm_CellSteps5, 38, 72);
  Engine_EventWait(30);
  BattleFx_PlayQueuedSound();
  Camera_MoveTo(PIXELS(112), -1, PIXELS(1225), 1);
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_CellSteps1, 35, 73);
  Engine_EventWait(20);
  BattleFx_PlayQueuedSound();
  Actor_SetActionScript(ACTOR_DORA, gValeStorm_DoraScriptB);
  Engine_EventWait(20);
  Actor_SetActionScript(ACTOR_JENNA, gValeStorm_JennaScriptB);
  Engine_EventWait(40);
  Event_PlayCue(SE_159);
  Map_AnimateCells(gValeStorm_CellSteps4, 35, 73);
  Object_RefreshSelectorById(ACTOR_JENNA);
  BattleFx_PlayQueuedSound();
  Engine_EventWait(40);
  northMessage = MsgValeGoLookNorth;
  Engine_EventSetMessage(northMessage);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 40);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Engine_EventWait(30);
  Actor_SetActionScript(ACTOR_DORA, gValeStorm_DoraScriptC);
  Actor_SetActionScript(ACTOR_JENNA, gValeStorm_JennaScriptC);
  Engine_EventWait(40);
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(105), -1, PIXELS(1086), 1);
  Object_RefreshSelectorById(ACTOR_DORA);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 0, 40);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 10);
  Actor_WalkToAndWait(ACTOR_DORA, 105, 1086);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventSetMessage((northMessage + 4));
  } else {
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventSetMessage((northMessage + 5));
  }
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 0, 30);
  helpMessage = MsgValeDontSupposeTwo;
  Engine_EventSetMessage(helpMessage);
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventSetMessage((helpMessage + 1));
    Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0, 30);
    Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(40);
  } else {
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 5, 90);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 3, 40);
    Engine_ActorSetAnimation(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Engine_EventSetMessage((helpMessage + 2));
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
  }
  Actor_SetActionScript(ACTOR_DORA, gValeStorm_DoraScriptD);
  Engine_EventWait(90);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
}

/// In Vale on the stormy night, Kyle, at the river, tells Isaac and Garet they
/// do not have enough Psynergy to save Felix and should find help.
/// Reached by talking to Kyle; the first time he greets Isaac and sets
/// FLAG_VALE_STORM_KYLE_GREETED_ISAAC.
static void Event_TalkAboutSavingFelix(void) {
  s32 facing = FACING_SOUTHEAST;
  const s32 *pulseScript;

  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_KYLE, ANIM_STAND);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(ACTOR_KYLE, ACTOR_PARTY_LEADER, 20);
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_KYLE_GREETED_ISAAC) != 0) {
    Engine_EventSetMessage(MsgValeTwoDontEnough);
    Engine_EventShowMessageAndWait(ACTOR_KYLE, 0, 10);
  } else {
    Engine_EventSetMessage(MsgValeOh);
    Engine_ActorStartRepeatedMotion(ACTOR_KYLE, 1);
    Engine_EventShowMessageAndWait(ACTOR_KYLE, 0, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_KYLE, 2);
    Engine_EventShowMessageAndWait(ACTOR_KYLE, 0, 10);
  }
  Engine_ActorFaceDirection(ACTOR_KYLE, facing, 20);
  Engine_ActorSetAnimation(ACTOR_KYLE, 5);
  Engine_EventWait(10);
  {
    struct FieldActor *villager;
    s32 pulseDelay;
    villager = Object_GetById(ACTOR_KYLE);
    pulseDelay = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    pulseScript = gValeStorm_SquashAndStretchScript;
    villager->timer = pulseDelay;
    Actor_SetActionScript(ACTOR_KYLE, pulseScript);
  }
  Engine_EventWait(20);
  Engine_GameFlagSet(FLAG_VALE_STORM_KYLE_GREETED_ISAAC);
  Engine_EventEnd();
}

/// In Vale on the stormy night, Jenna comes back to the river with Isaac,
/// Garet and the man whose Psynergy is restored ("Mom! Dad! We're back!"),
/// and as Dora calls everyone down to save Felix the Boulder comes down the
/// river and sweeps away Felix, his parents and Kyle.
/// Reached by stepping on cell code 36 once FLAG_VALE_STORM_SENT_TO_AID_ELDERS
/// is set (vale_gate.c), until FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY is set,
/// which this sets.
static void Event_BoulderSweepsThemAway(void) {
  struct FieldActor *leader;
  struct FieldActor *actor;
  s32 automaticPriority;
  struct AnimationObject *sprite;

  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) == 0) {
  } else {
    if (GameFlag_IsSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) != 0) {
    } else {
      Engine_EventBegin();
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
      Actor_SetSpeed(ACTOR_JENNA, PIXELS(1), PIXELS(1) / 2);
      Actor_SetSpeed(ACTOR_EXHAUSTED_STRONGMAN, PIXELS(1), PIXELS(1) / 2);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 217, 1367);
      actor = Object_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Engine_ActorSetPosition(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
      }
      Actor_WalkToAndWait(ACTOR_GARET, 235, 1367);
      Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST + FACING_STEP, 0);
      actor = Object_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Engine_ActorSetPosition(ACTOR_JENNA, actor->x.fixed, actor->z.fixed);
      }
      Actor_WalkToAndWait(ACTOR_JENNA, 199, 1367);
      Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH + FACING_STEP, 0);
      Actor_SetPosition(ACTOR_JENNAS_MOTHER, PIXELS(247), PIXELS(1210));
      Engine_ActorFaceDirection(ACTOR_JENNAS_MOTHER, FACING_SOUTHWEST, 0);
      actor = Object_GetById(ACTOR_EXHAUSTED_STRONGMAN);
      sprite = actor->sprite;
      actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      sprite->image.priority = 1;
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      sprite = leader->sprite;
      leader->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      sprite->image.priority = 2;
      actor = Object_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Engine_ActorSetPosition(ACTOR_EXHAUSTED_STRONGMAN, actor->x.fixed,
                                actor->z.fixed);
      }
      Actor_WalkToAndWait(ACTOR_EXHAUSTED_STRONGMAN, 221, 1385);
      Actor_FaceDirection(ACTOR_EXHAUSTED_STRONGMAN,
                          FACING_NORTHWEST + FACING_STEP, 60);
      Engine_ActorRunRepeatedMotion(ACTOR_JENNA, 2);
      Engine_EventSetMessage(MsgValeMomDadBack);
      Engine_EventShowMessageAndWait(ACTOR_JENNA, 0, 40);
      Actor_SetPosition(ACTOR_DORA, PIXELS(101), PIXELS(1197));
      Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST, 0);
      Engine_EventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_DORA, 0, 10);
      Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
      Engine_CameraSetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
      Camera_MoveTo(PIXELS(101), -1, PIXELS(1197), 1);
      Actor_SetSpeed(ACTOR_DORA, PIXELS(7) / 5, PIXELS(7) / 10);
      Actor_SetActionScript(ACTOR_DORA, gValeStorm_StormRunActions);
      Engine_EventWait(60);
      Engine_CameraSetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
      Camera_MoveTo(PIXELS(187), -1, PIXELS(1328), 1);
      Engine_CameraWaitForMove();
      Engine_EventWait(40);
      Engine_ActorRunRepeatedMotion(ACTOR_JENNA, 2);
      Engine_EventShowMessageAndWait(ACTOR_JENNA, 0, 20);
      Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
      Engine_EventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_DORA, 0, 20);
      Engine_CameraSetSpeed(PIXELS(2), PIXELS(1) / 4);
      Camera_MoveTo(PIXELS(221), -1, PIXELS(1385), 1);
      Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_EXHAUSTED_STRONGMAN, 0);
      Engine_ActorFaceActor(ACTOR_GARET, ACTOR_EXHAUSTED_STRONGMAN, 0);
      Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST + FACING_STEP, 80);
      Engine_CameraMoveTo(PIXELS(182), -1, PIXELS(1360), 1);
      Actor_WalkToAndWait(ACTOR_EXHAUSTED_STRONGMAN, 182, 1384);
      Engine_ActorFaceActor(ACTOR_EXHAUSTED_STRONGMAN, ACTOR_DORA, 0);
      Engine_EventWait(30);
      Engine_ActorSetAnimationAndWait(ACTOR_EXHAUSTED_STRONGMAN, ANIM_NOD);
      Engine_EventWait(10);
      Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
      Engine_ActorFaceActor(ACTOR_GARET, ACTOR_DORA, 0);
      Engine_ActorFaceActor(ACTOR_JENNA, ACTOR_DORA, 0);
      Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
      Engine_EventShowMessage(ACTOR_DORA, 0);
      Engine_ActorRunRepeatedMotion(ACTOR_JENNA, 2);
      Engine_EventShowMessageAndWait(ACTOR_JENNA, 0, 10);
      Actor_FaceDirection(ACTOR_DORA, FACING_NORTHEAST, 40);
      Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 20);
      Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
      Engine_EventShowMessage(ACTOR_DORA, 0);
      Engine_ActorFaceEachOther(ACTOR_JENNA, ACTOR_EXHAUSTED_STRONGMAN, 0);
      Engine_ActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
      Engine_EventWait(40);
      Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
      Engine_ActorFaceActor(ACTOR_GARET, ACTOR_DORA, 0);
      Engine_ActorFaceActor(ACTOR_JENNA, ACTOR_DORA, 0);
      Engine_ActorFaceActor(ACTOR_EXHAUSTED_STRONGMAN, ACTOR_DORA, 0);
      Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
      Engine_EventWait(20);
      Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 10);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
      Engine_ActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
      Engine_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
      automaticPriority = ACTOR_PRIORITY_AUTOMATIC;
      Engine_ActorSetAnimationAndWait(ACTOR_EXHAUSTED_STRONGMAN, ANIM_NOD);
      leader->priorityFlags |= automaticPriority;
      actor = Object_GetById(ACTOR_EXHAUSTED_STRONGMAN);
      actor->priorityFlags |= automaticPriority;
      ValeStorm_ShowBoulderSweepingDownriver();
      Engine_GameFlagSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY);
      Engine_EventEnd();
    }
  }
}

/// Places an actor and its motion targets at the supplied coordinates.
static __inline__ void ValeStorm_PlaceActor(struct FieldActor *object,
                                            s32 horizontalPosition,
                                            s32 groundPosition) {
  object->x.fixed = horizontalPosition;
  object->y.fixed = horizontalPosition;
  object->targetX = horizontalPosition;
  object->targetY = horizontalPosition;
  object->z.fixed = groundPosition;
  object->targetZ = groundPosition;
}

/// Shakes the scene at the impact cue and waits for the beat.
static __inline__ void ValeStorm_PlayImpactBeat(s32 shakeAmplitude,
                                                s32 frames) {
  Map_SetShake(shakeAmplitude, shakeAmplitude, Q16_ONE);
  Event_PlayCue(SE_145);
  Event_Wait(frames);
}

/// Follows the boulder downriver, then restores the riverbank actors.
static void ValeStorm_ShowBoulderSweepingDownriver(void) {
  struct AnimationObject *actorVisual;
  struct AnimationObject *groupVisual;
  struct FieldActor *actor;
  struct FieldActor *lookingActor;
  struct FieldActor *groupActor;
  struct FieldActor *currentActor;
  u32 random;
  const s32 *entryActions;
  s32 stoppedVelocity;
  u8 stoppedMotion;
  const s32 *lookAroundActions;
  s32 resetValue;
  const s32 *departureActions;

  actor = Actor_Get(ACTOR_BOULDER);
  groupActor = Actor_Get(ACTOR_BOULDER_GLOW);
  actorVisual = actor->sprite;
  groupVisual = groupActor->sprite;
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(110), -1, PIXELS(1419), 1);
  Actor_SetSpeed(ACTOR_EXHAUSTED_STRONGMAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  // Bring the group to the riverbank as the lightning subsides.
  entryActions = gValeStorm_ArriveScript;
  Actor_SetActionScript(ACTOR_EXHAUSTED_STRONGMAN, entryActions);
  Event_Wait(10);
  Actor_SetActionScript(ACTOR_JENNA, entryActions);
  FieldFx_LastLightning();
  Event_Wait(10);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, entryActions);
  Event_Wait(10);
  FieldFx_StopLightning();
  Actor_SetActionScript(ACTOR_GARET, entryActions);
  Event_Wait(128);
  Event_ClearPathCellOverride();
  Camera_MoveTo(PIXELS(174), -1, PIXELS(1428), 1);
  Event_Wait(104);
  Camera_MoveTo(PIXELS(153), -1, PIXELS(1325), 1);
  Actor_WalkToAndWait(ACTOR_DORA, 158, 1272);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST, 0);
  Object_RefreshSelectorById(ACTOR_EXHAUSTED_STRONGMAN);
  Actor_SetActionScript(ACTOR_EXHAUSTED_STRONGMAN,
                        gValeStorm_ExhaustedStrongmanGatherPath);
  Actor_SetActionScript(ACTOR_JENNA, gValeStorm_JennaGatherPath);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gValeStorm_LeaderGatherPath);
  Actor_RunActionScriptAndWait(ACTOR_GARET, gValeStorm_GaretGatherPath);
  ValeStorm_PlayImpactBeat(PIXELS(4), 20);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_Wait(60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(ACTOR_EXHAUSTED_STRONGMAN, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 1, 60);
  Actor_FaceEachOther(ACTOR_JENNA, ACTOR_EXHAUSTED_STRONGMAN, 0);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  lookingActor = Actor_Get(ACTOR_PARTY_LEADER);
  random = Random_Next();
  random = Math_RemainderUnsigned(random, 20) + 20;
  lookingActor->timer = random;
  stoppedVelocity = 0;
  stoppedMotion = 0;
  lookingActor = Actor_Get(ACTOR_GARET);
  random = Random_Next();
  lookingActor->timer = (Math_RemainderUnsigned(random, 20) + 20);
  lookingActor = Actor_Get(ACTOR_JENNA);
  random = Random_Next();
  lookingActor->timer = (Math_RemainderUnsigned(random, 20) + 20);
  lookingActor = Actor_Get(ACTOR_EXHAUSTED_STRONGMAN);
  random = Random_Next();
  lookingActor->timer = (Math_RemainderUnsigned(random, 20) + 20);
  lookingActor = Actor_Get(ACTOR_DORA);
  random = Random_Next();
  lookingActor->timer = (Math_RemainderUnsigned(random, 20) + 20);
  lookAroundActions = gValeStorm_LookAroundScript;
  Actor_SetActionScript(ACTOR_DORA, lookAroundActions);
  Event_Wait(30);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, lookAroundActions);
  Actor_SetActionScript(ACTOR_JENNA, lookAroundActions);
  Actor_SetActionScript(ACTOR_GARET, lookAroundActions);
  Actor_SetActionScript(ACTOR_EXHAUSTED_STRONGMAN, lookAroundActions);
  Event_Wait(10);
  Event_PlayCue(MUS_017);
  ValeStorm_PlayImpactBeat(PIXELS(3), 30);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_Wait(120);
  ValeStorm_PlayImpactBeat(PIXELS(3), 40);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Event_Wait(60);
  ValeStorm_PlayImpactBeat(PIXELS(4), 20);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_Wait(60);
  ValeStorm_PlayImpactBeat(PIXELS(3), 40);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_Wait(60);
  FieldFx_StopLightning();
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_Wait(1);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Camera_SetSpeed(PIXELS(8), PIXELS(8));
  Camera_MoveTo(PIXELS(217), -1, PIXELS(1084), 1);
  ColorBuffer_ApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(40);
  Task_Wait(40);
  // Stage the boulder and its glow for the downriver camera shot.
  Actor_SetChildValue(ACTOR_BOULDER, 0);
  currentActor = Actor_Get(ACTOR_BOULDER);
  Actor_SetSpriteFlags(currentActor, 0);
  currentActor = Actor_Get(ACTOR_BOULDER_GLOW);
  Actor_SetSpriteFlags(currentActor, 0);
  groupActor->scaleX = 0xcccc;
  groupActor->scaleY = 0xcccc;
  groupActor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  groupVisual->image.priority = 1;
  ValeStorm_PlaceActor(actor, PIXELS(200), PIXELS(898));
  actor->motionFlags = stoppedMotion;
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  actorVisual->image.priority = 0;
  currentActor = Camera_GetEventObject();
  currentActor->targetX = ACTOR_NO_TARGET;
  currentActor = Camera_GetEventObject();
  currentActor->targetY = ACTOR_NO_TARGET;
  currentActor = Camera_GetEventObject();
  currentActor->targetZ = ACTOR_NO_TARGET;
  currentActor = Camera_GetEventObject();
  currentActor->velocityX = stoppedVelocity;
  currentActor = Camera_GetEventObject();
  currentActor->velocityY = stoppedVelocity;
  currentActor = Camera_GetEventObject();
  currentActor->velocityZ = stoppedVelocity;
  Task_Wait(1);
  Camera_MoveTo(PIXELS(247), PIXELS(128), PIXELS(917), 0);
  Engine_MapRedraw();
  Task_Wait(1);
  ColorBuffer_ApplyTarget(0x10003, 1);
  ColorBuffer_ApplyTarget(0x10000, 2);
  Engine_ColorBufferInterpolate(30);
  Task_Wait(30);
  // Follow the boulder script until its eighth movement phase.
  FieldScheduler_AddCallback(ValeStorm_UpdatePulsingGlow,
                             STORM_CALLBACK_PRIORITY);
  Actor_SetActionScript(ACTOR_BOULDER, gValeStorm_BoulderRiseScript);
  Camera_SetSpeed(PIXELS(2), PIXELS(3) / 100);
  Camera_MoveTo(PIXELS(175), PIXELS(96), PIXELS(1086), 1);
  do {
    Task_Wait(1);
  } while ((s16)actor->work != 8);
  ColorBuffer_ApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(60);
  Task_Wait(60);
  Engine_MapWaitWorkValuesBelow256();
  currentActor = Camera_GetEventObject();
  currentActor->targetX = ACTOR_NO_TARGET;
  currentActor = Camera_GetEventObject();
  currentActor->targetY = ACTOR_NO_TARGET;
  currentActor = Camera_GetEventObject();
  currentActor->targetZ = ACTOR_NO_TARGET;
  resetValue = 0;
  currentActor = Camera_GetEventObject();
  currentActor->velocityX = resetValue;
  currentActor = Camera_GetEventObject();
  currentActor->velocityY = resetValue;
  currentActor = Camera_GetEventObject();
  currentActor->velocityZ = resetValue;
  Scheduler_RemoveCallback(ValeStorm_UpdatePulsingGlow);
  Actor_Stop(ACTOR_BOULDER);
  Task_Wait(1);
  Actor_SetAnimation(ACTOR_BOULDER, 0);
  groupActor->scaleX = 0x14000;
  groupActor->scaleY = 0x14000;
  groupVisual->offsetY = 2;
  groupVisual->scale = 0x14000;
  actor->scaleX = 2 * Q16_ONE;
  actor->scaleY = 2 * Q16_ONE;
  actor->x.fixed = resetValue;
  actor->z.fixed = resetValue;
  actor->targetX = resetValue;
  actor->targetZ = resetValue;
  Task_Wait(1);
  // Stage the riverbank reactions between lightning flashes.
  Actor_SetAnimation(ACTOR_FELIX, 8);
  Actor_SetPosition(ACTOR_DORA, PIXELS(169), PIXELS(1264));
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 0);
  Actor_SetAnimation(ACTOR_DORA, 9);
  Actor_SetPosition(ACTOR_JENNA, PIXELS(151), PIXELS(1292));
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_SetAnimation(ACTOR_JENNA, 5);
  Actor_SetPosition(ACTOR_EXHAUSTED_STRONGMAN, PIXELS(170), PIXELS(1313));
  Actor_FaceDirection(ACTOR_EXHAUSTED_STRONGMAN, FACING_SOUTHWEST, 0);
  Actor_SetAnimation(ACTOR_EXHAUSTED_STRONGMAN, 5);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(185), PIXELS(1333));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 17);
  Actor_SetPosition(ACTOR_GARET, PIXELS(169), PIXELS(1384));
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_SetAnimation(ACTOR_GARET, 0);
  Camera_MoveTo(PIXELS(166), 0, PIXELS(1337), 0);
  Engine_MapRedraw();
  actor->motionFlags = resetValue;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  ValeStorm_FlashLightning();
  Actor_SetPosition(ACTOR_BOULDER_GLOW, PIXELS(218), PIXELS(1176));
  Camera_MoveTo(PIXELS(210), 0, PIXELS(1196), 0);
  Engine_MapRedraw();
  groupActor->scaleX = 2 * Q16_ONE;
  groupActor->scaleY = 2 * Q16_ONE;
  FieldScheduler_AddCallback(ValeStorm_FlickerGlowOffset,
                             STORM_CALLBACK_PRIORITY);
  Actor_Stop(ACTOR_KYLE);
  Actor_Stop(ACTOR_JENNAS_FATHER);
  Actor_Stop(ACTOR_JENNAS_MOTHER);
  Task_Wait(1);
  groupActor = Actor_Get(ACTOR_KYLE);
  groupVisual = groupActor->sprite;
  groupActor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  groupActor->scaleX = Q16_ONE;
  groupActor->scaleY = Q16_ONE;
  groupActor->facing = 0xd000;
  groupVisual->image.priority = 0;
  Actor_SetAnimation(ACTOR_KYLE, 0);
  groupActor = Actor_Get(ACTOR_JENNAS_FATHER);
  groupVisual = groupActor->sprite;
  groupActor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  groupActor->scaleX = Q16_ONE;
  groupActor->scaleY = Q16_ONE;
  groupActor->facing = 0xb000;
  groupVisual->image.priority = 0;
  Actor_SetAnimation(ACTOR_JENNAS_FATHER, 5);
  groupActor = Actor_Get(ACTOR_JENNAS_MOTHER);
  groupVisual = groupActor->sprite;
  groupActor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  groupActor->scaleX = Q16_ONE;
  groupActor->scaleY = Q16_ONE;
  groupActor->facing = 0xb000;
  groupVisual->image.priority = 0;
  Actor_SetAnimation(ACTOR_JENNAS_MOTHER, 5);
  groupActor = Actor_Get(ACTOR_BOULDER_GLOW);
  groupVisual = groupActor->sprite;
  ValeStorm_FlashLightning();
  actor->y.fixed = PIXELS(48);
  actor->x.fixed = PIXELS(214);
  actor->z.fixed = PIXELS(1216);
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  groupVisual->image.priority = 1;
  Actor_SetPosition(ACTOR_BOULDER_GLOW, PIXELS(214), PIXELS(1216));
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_NORTH, 20);
  GameFlag_Set(FLAG_WEATHER_HELD);
  Map_SetLayerEntryFlag(0);
  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);
  Map_SetLayerEntryFlag(3);
  Map_SetLayerEntryFlag(4);
  Map_SetLayerEntryFlag(5);
  ColorBuffer_ApplyTarget(0x10003, 1);
  ColorBuffer_ApplyTarget(0x10000, 2);
  Engine_ColorBufferInterpolate(120);
  Task_Wait(160);
  ColorBuffer_ApplyTarget(0x7fff, 1);
  ColorBuffer_ApplyTarget(0x7fff, 2);
  Engine_ColorBufferInterpolate(80);
  Event_Wait(80);
  Event_Wait(100);
  Scheduler_RemoveCallback(ValeStorm_FlickerGlowOffset);
  groupVisual->scale = groupActor->scaleX;
  GameFlag_Clear(FLAG_WEATHER_HELD);
  Map_ClearLayerEntryFlag(0);
  Map_ClearLayerEntryFlag(1);
  Map_ClearLayerEntryFlag(2);
  Map_ClearLayerEntryFlag(3);
  Map_ClearLayerEntryFlag(4);
  Map_ClearLayerEntryFlag(5);
  // Restore the riverbank and the idle actor scripts.
  ValeStorm_RebuildRiverbank();
  Actor_SetPosition(ACTOR_DORA, PIXELS(165), PIXELS(1229));
  Actor_SetAnimation(ACTOR_DORA, 1);
  actor = Actor_Get(ACTOR_DORA);
  actor->facing = FACING_NORTHEAST;
  random = Random_Next();
  actor->timer = Math_RemainderUnsigned(random, 90) + 60;
  departureActions = gValeStorm_SquashAndStretchScript;
  actor->work = 1;
  Actor_SetActionScript(ACTOR_DORA, departureActions);
  Actor_SetPosition(ACTOR_JENNA, PIXELS(165), PIXELS(1254));
  Actor_SetAnimation(ACTOR_JENNA, 1);
  actor = Actor_Get(ACTOR_JENNA);
  actor->facing = FACING_NORTHEAST;
  random = Random_Next();
  actor->timer = Math_RemainderUnsigned(random, 90) + 60;
  actor->work = 2;
  Actor_SetActionScript(ACTOR_JENNA, departureActions);
  Actor_SetPosition(ACTOR_GARET, PIXELS(152), PIXELS(1285));
  Actor_SetAnimation(ACTOR_GARET, 1);
  actor = Actor_Get(ACTOR_GARET);
  actor->facing = FACING_NORTHEAST;
  random = Random_Next();
  actor->timer = Math_RemainderUnsigned(random, 90) + 60;
  actor->work = 3;
  Actor_SetActionScript(ACTOR_GARET, departureActions);
  Actor_SetPosition(ACTOR_EXHAUSTED_STRONGMAN, PIXELS(180), PIXELS(1311));
  actor = Actor_Get(ACTOR_EXHAUSTED_STRONGMAN);
  actor->facing = FACING_NORTHEAST;
  random = Random_Next();
  actor->timer = Math_RemainderUnsigned(random, 90) + 60;
  actor->work = 4;
  Actor_SetActionScript(ACTOR_EXHAUSTED_STRONGMAN, departureActions);
  Actor_SetAnimation(ACTOR_EXHAUSTED_STRONGMAN, 6);
  Object_GetById(ACTOR_GARET)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Object_GetById(ACTOR_EXHAUSTED_STRONGMAN)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  FieldScheduler_AddCallback(ValeStorm_CopyLeaderSpritePriority,
                             STORM_CALLBACK_PRIORITY);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(181), PIXELS(1273));
  currentActor = Actor_Get(ACTOR_PARTY_LEADER);
  currentActor->facing = FACING_NORTHEAST;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
  Camera_MoveTo(PIXELS(181), 0, PIXELS(1273), 0);
  Engine_MapRedraw();
  Actor_SetPosition(ACTOR_KYLE, 0, 0);
  Actor_SetPosition(ACTOR_BOULDER, 0, 0);
  Actor_SetPosition(ACTOR_JENNAS_FATHER, 0, 0);
  Actor_SetPosition(ACTOR_JENNAS_MOTHER, 0, 0);
  Actor_SetPosition(ACTOR_FELIX, 0, 0);
  Actor_SetPosition(ACTOR_BOULDER_GLOW, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_ROCK + 6, PIXELS(144), PIXELS(1070));
  Actor_SetPosition(ACTOR_FIRST_ROCK + 7, PIXELS(276), PIXELS(1270));
  Task_Wait(60);
  ColorBuffer_ApplyTarget(0x10003, 1);
  ColorBuffer_ApplyTarget(0x10000, 2);
  Engine_ColorBufferInterpolate(80);
  Event_Wait(60);
  BattleFx_PlayQueuedSound();
  Event_Wait(60);
  Party_RemoveOwnerRestored(1);
  FieldFx_ResumeLightning();
}

/// Flashes the scene layers before fading back to the storm palette.
static void ValeStorm_FlashLightning(void) {
  Engine_TaskWait(20);
  GameFlag_Set(FLAG_WEATHER_HELD);
  Map_SetLayerEntryFlag(0);
  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);
  Map_SetLayerEntryFlag(3);
  Map_SetLayerEntryFlag(4);
  Map_SetLayerEntryFlag(5);
  ColorBuffer_ApplyTarget(0x10003, 1);
  ColorBuffer_ApplyTarget(0x10000, 2);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(120);
  ColorBuffer_ApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  GameFlag_Clear(FLAG_WEATHER_HELD);
  Map_ClearLayerEntryFlag(0);
  Map_ClearLayerEntryFlag(1);
  Map_ClearLayerEntryFlag(2);
  Map_ClearLayerEntryFlag(3);
  Map_ClearLayerEntryFlag(4);
  Map_ClearLayerEntryFlag(5);
}

/// Tracks the boulder and pulses its glow with the movement script phases.
static void ValeStorm_UpdatePulsingGlow(void) {
  struct FieldActor *source;
  struct FieldActor *glow;
  struct AnimationObject *sprite;
  s16 *timer;

  source = Object_GetById(ACTOR_BOULDER);
  glow = Object_GetById(ACTOR_BOULDER_GLOW);
  sprite = glow->sprite;
  timer = &source->timer;
  if (*timer != 0) {
    if (*timer == 60) {
      Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
    }
    if (*timer == 40) {
      Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
    }
    if (*timer == 30) {
      Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    }
    if (*timer == 20) {
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
    }
    (*timer)--;
  }
  glow->x.fixed = source->x.fixed;
  glow->z.fixed = source->z.fixed;
  glow->targetX = glow->x.fixed;
  sprite->offsetY = 10;
  if (gFrameCount & 1) {
    switch ((s16)source->work) {
    case 1:
    case 5:
      glow->scaleX += 0xa3d;
      glow->scaleY += 0xa3d;
      break;
    case 4:
      glow->scaleX += 0x51e;
      glow->scaleY += 0x51e;
      break;
    case 2:
    case 3:
    case 6:
    case 7:
    case 8:
    case 9:
      glow->scaleX += -0x7ae;
      glow->scaleY += -0x7ae;
      break;
    }
    sprite->scale = glow->scaleX;
  } else {
    sprite->scale = 0;
  }
}

/// Alternates the glow vertical offset on successive frames.
static void ValeStorm_FlickerGlowOffset(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_BOULDER_GLOW);
  u32 frame = gFrameCount;
  struct AnimationObject *sprite = actor->sprite;

  if (frame & 1) {
    sprite->offsetY = 2;
  } else {
    sprite->offsetY = 64;
  }
}

/// In Vale on the stormy night, after the Boulder, the strongman who came to
/// help tries to go for help himself and cannot move. "I can't move..." Reached
/// by talking to him (ACTOR_EXHAUSTED_STRONGMAN); the first time sets
/// FLAG_VALE_STORM_STRONGMAN_TRIED_TO_GO.
static void Event_StrongmanTriesToGoForHelp(void) {
  struct FieldActor *villager;
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  villager = Object_GetById(ACTOR_EXHAUSTED_STRONGMAN);
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_VALE_STORM_STRONGMAN_TRIED_TO_GO) != 0) {
    Engine_ActorStop(ACTOR_EXHAUSTED_STRONGMAN);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_EXHAUSTED_STRONGMAN, 2);
    Engine_EventWait(40);
    if ((s16)leader->facing >= 0) {
      Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, 7);
    } else {
      Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, 8);
    }
    Engine_ActorRunRepeatedMotion(ACTOR_EXHAUSTED_STRONGMAN, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgValeICantMoveGetHelp);
    Event_ShowMessage(ACTOR_EXHAUSTED_STRONGMAN, 0);
    Actor_SetActionScript(ACTOR_EXHAUSTED_STRONGMAN,
                          gValeStorm_SquashAndStretchScript);
    Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, 6);
  } else {
    Engine_ActorStop(ACTOR_EXHAUSTED_STRONGMAN);
    villager->scaleX = Q16_ONE;
    villager->scaleY = Q16_ONE;
    Actor_FaceDirection(ACTOR_EXHAUSTED_STRONGMAN, FACING_EAST + FACING_STEP,
                        0);
    if ((s16)leader->facing >= 0) {
      Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, 7);
    } else {
      Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, 8);
    }
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgValeIllGo);
    Event_ShowMessageAndWait(ACTOR_EXHAUSTED_STRONGMAN, 0, 20);
    Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, ANIM_STAND);
    Engine_ActorJump(ACTOR_EXHAUSTED_STRONGMAN, 4, 0);
    Engine_EventWait(80);
    Engine_ActorRunRepeatedMotion(ACTOR_EXHAUSTED_STRONGMAN, 2);
    Engine_EventWait(40);
    if ((s16)leader->facing >= 0) {
      Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, 7);
    } else {
      Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, 8);
    }
    Engine_EventWait(2);
    Engine_ActorJump(ACTOR_EXHAUSTED_STRONGMAN, 2, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_EXHAUSTED_STRONGMAN, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_EXHAUSTED_STRONGMAN, 0);
    Actor_SetActionScript(ACTOR_EXHAUSTED_STRONGMAN,
                          gValeStorm_SquashAndStretchScript);
    Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, 6);
    GameFlag_Set(FLAG_VALE_STORM_STRONGMAN_TRIED_TO_GO);
  }
  Engine_EventEnd();
}

/// As Isaac leaves the river to get help, Garet will not let him go alone and
/// goes after him. "I can't let him go alone..."
/// Called by Event_LeaveToValeGate after the Boulder swept Felix, his parents
/// and Kyle away; sets FLAG_VALE_STORM_GARET_LEFT, which has the two meet the
/// strangers on the way (vale_gate.c).
static void ValeStorm_GaretFollowsIsaac(void) {
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_ActorStop(ACTOR_GARET);
  Scheduler_RemoveCallback(ValeStorm_CopyLeaderSpritePriority);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 480, 1392);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_Get(ACTOR_GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetPosition(ACTOR_GARET, PIXELS(249), PIXELS(1240));
  Engine_TaskWait(1);
  Engine_EventSetMessage(MsgValeIsaac);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0);
  Actor_SetPosition(ACTOR_GARET, PIXELS(172), PIXELS(1278));
  Engine_TaskWait(1);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(162), 0, PIXELS(1285), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_GARET, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(ACTOR_GARET, 165, 1300);
  Actor_WalkToAndWait(ACTOR_GARET, 195, 1432);
  GameFlag_Set(FLAG_VALE_STORM_GARET_LEFT);
}

/// Redraws the riverbank cells after the boulder has passed.
static void ValeStorm_RebuildRiverbank(void) {
  Map_CopyCellsTo(16, 96, 11, 73, 6, 3);
  Map_CopyCellsTo(16, 96, 34, 68, 14, 10);
  Map_CopyCellsTo(16, 96, 64, 68, 7, 7);
  Map_CopyCellsTo(9, 95, 11, 73, 6, 3);
  Map_CopyCellsTo(40, 94, 34, 68, 14, 10);
  Map_CopyCellsTo(54, 94, 64, 68, 8, 7);
  Map_CopyCellsTo(72, 75, 72, 76, 1, 1);
  Map_CopyCellsTo(72, 75, 74, 76, 1, 1);
  Map_CopyCellAttributes(7, 75, 1, 1, 6, 75);
  Map_CopyCellAttributes(8, 70, 3, 1, 8, 71);
  Map_CopyCellAttributes(8, 70, 2, 1, 9, 72);
  Map_CopyCellAttributes(8, 70, 2, 1, 9, 73);
  Map_CopyCellAttributes(11, 66, 1, 1, 8, 73);
  Map_CopyCellAttributes(12, 66, 1, 4, 11, 73);
  Map_CopyCellAttributes(25, 0, 1, 1, 6, 74);
  Engine_MapRedraw();
}

/// In Vale on the stormy night, a monster attacks on the paths (slot 2 of
/// ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT).
/// Reached by stepping on cell code 16 until
/// FLAG_VALE_STORM_THIRD_MONSTER_FOUGHT is set.
static void Event_FightThirdMonster(void) {
  struct EventWork *state;

  Engine_EventBegin();
  state = gEventWork;
  state->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  state->transitionFrames = 64;
  Engine_GameFlagSet(FLAG_VALE_STORM_THIRD_MONSTER_FOUGHT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 2);
  Engine_GameFlagSet(FLAG_VALE_STORM_MONSTER_FOUGHT);
  Engine_EventEnd();
}

/// In Vale on the stormy night, another monster attacks (slot 3 of
/// ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT).
/// Reached by stepping on cell code 17 until
/// FLAG_VALE_STORM_FOURTH_MONSTER_FOUGHT is set.
static void Event_FightFourthMonster(void) {
  struct EventWork *state;

  Engine_EventBegin();
  state = gEventWork;
  state->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  state->transitionFrames = 64;
  Engine_GameFlagSet(FLAG_VALE_STORM_FOURTH_MONSTER_FOUGHT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 3);
  Engine_GameFlagSet(FLAG_VALE_STORM_MONSTER_FOUGHT);
  Engine_EventEnd();
}

/// Flashes the actor palette and emits paired effects at the shake interval.
static void ValeStorm_FlashPaletteAndSpawnEffects(struct FieldActor *actor) {
  s32 spawnInterval;

  if ((gFrameCount & 2) != 0) {
    Engine_ObjectSetPartPalettes(actor, 7);
  } else {
    Engine_ObjectSetPartPalettes(actor, 0);
  }
  {
    volatile u32 *frame = &gFrameCount;

    spawnInterval = (sShakeShift << 3) + 16;
    if (Math_RemainderUnsigned(*frame, spawnInterval) == 0) {
      ValeStorm_SpawnEffectPair(actor);
    }
  }
}

/// Cycles six palettes and emits paired effects at the shake interval.
static void ValeStorm_CyclePaletteAndSpawnEffects(struct FieldActor *actor) {
  s32 paletteFrame;
  s32 spawnInterval;

  if ((gFrameCount & 1) != 0) {
    paletteFrame = (gFrameCount >> 1);
    Engine_ObjectSetPartPalettes(actor,
                                 Math_RemainderUnsigned(paletteFrame, 6));
  }
  spawnInterval = (sShakeShift << 3) + 16;
  if (Math_RemainderUnsigned(gFrameCount, spawnInterval) == 0) {
    ValeStorm_SpawnEffectPair(actor);
  }
}

/// Cycles six actor palettes on alternate frames.
static void ValeStorm_CycleObjectPalettes(struct FieldActor *actor) {
  s32 paletteFrame;

  if ((gFrameCount & 1) != 0) {
    paletteFrame = (gFrameCount >> 1);
    Engine_ObjectSetPartPalettes(actor,
                                 Math_RemainderUnsigned(paletteFrame, 6));
  }
}

#include "common/effect_vertical.h"

/// Creates the two linked effects that travel in opposite vertical directions.
static void ValeStorm_SpawnEffectPair(struct FieldActor *parent) {
  struct FieldActor *pair[2];
  struct FieldActor *child;
  struct AnimationObject *sprite;
  struct BattleFxScene *work = gEffectWork;
  s32 i;

  Engine_AudioPlayCue(SE_JUMP);
  for (i = 0; i < 2; ++i) {
    child = Engine_ObjectCreate(26, parent->x.fixed, parent->y.fixed,
                                parent->z.fixed);
    pair[i] = child;
    if (child != NULL) {
      child->terrainHeight = parent->terrainHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->linkedObject = parent;
      if (sprite != NULL) {
        Object_InitializeMode(sprite, 0);
        sprite->flags = 0;
        Resource_ResetEntry(sprite->slot);
        sprite->slot = work->tileSlot;
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

    effect->update = Effect_AnimateVerticalNegative;
    effectSprite->image.priority = parent->sprite->image.priority;
  }
  {
    struct FieldActor *effect = pair[1];
    struct AnimationObject *effectSprite = effect->sprite;

    effectSprite->image.priority = parent->sprite->image.priority;
    effect->update = Effect_AnimateVerticalPositive;
    effect->priorityFlags = 2;
  }
}

#include "common/effect_psynergy.h"

/// Cycles the four elders' palettes and effects while they hold the Boulder;
/// the westernmost elder's stop once that elder lets go (sShakeDone).
static void ValeStorm_UpdateBoulderSceneEffects(void) {
  ValeStorm_CyclePaletteAndSpawnEffects(Object_GetById(ACTOR_ELDER_EAST));
  ValeStorm_CyclePaletteAndSpawnEffects(Object_GetById(ACTOR_ELDER_WEST));
  ValeStorm_CyclePaletteAndSpawnEffects(Object_GetById(ACTOR_ELDER_FAR_EAST));
  if (sShakeDone == 0) {
    ValeStorm_CyclePaletteAndSpawnEffects(Object_GetById(ACTOR_ELDER_FAR_WEST));
  }
}

static void ValeStorm_CycleBoulderPalette(void) {
  ValeStorm_CycleObjectPalettes(Object_GetById(ACTOR_BOULDER));
}

/// Keeps Garet and the man who came to help (ACTOR_EXHAUSTED_STRONGMAN) at the
/// leader's sprite priority while they stand by the river.
static void ValeStorm_CopyLeaderSpritePriority(void) {
  struct AnimationObject *sourceSprite;
#if !TBS_PATCH_3
  struct AnimationObject *targetSprite;
  struct AnimationObject *secondTargetSprite;
#endif

  sourceSprite = Object_GetById(ACTOR_PARTY_LEADER)->sprite;
#if TBS_PATCH_3
  Engine_ActorSetSpritePriority(ACTOR_GARET, sourceSprite->image.priority);
  Engine_ActorSetSpritePriority(ACTOR_EXHAUSTED_STRONGMAN,
                                sourceSprite->image.priority);
#else
  targetSprite = Object_GetById(ACTOR_GARET)->sprite;
  targetSprite->image.priority = sourceSprite->image.priority;
  secondTargetSprite = Object_GetById(ACTOR_EXHAUSTED_STRONGMAN)->sprite;
  secondTargetSprite->image.priority = sourceSprite->image.priority;
#endif
}

/// In Vale on the stormy night, searching a broken door: it won't open, the
/// impact having knocked it off its hinges.
static void Event_SearchBrokenDoor(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgValeDoorWontOpen, 1);
  Engine_EventEnd();
}

/// In Vale on the stormy night, searching the chest Garet was trying to save.
/// "It's Garet's chest of valuables."
static void Event_SearchValuablesChest(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgValeSChestValuables, 1);
  Engine_EventEnd();
}

u8 gValeStorm_FrameModes[] = {7, 0, 0, 0};

s32 gValeStorm_ShovingManScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(185), 0, PIXELS(824)),
    SC_SYNC,   SC_MOVE(PIXELS(179), 0, PIXELS(836)),
    SC_SYNC,   SC_MOVE(PIXELS(226), 0, PIXELS(840)),
    SC_SYNC,   SC_MOVE(PIXELS(226), 0, PIXELS(944)),
    SC_SYNC,   SC_POS(0, 0, 0),
    SC_ARG(1), SC_END,
};

s32 gValeStorm_UnusedGaretRunsUpScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(345), 0, PIXELS(612)),
    SC_SYNC,   SC_MOVE(PIXELS(311), 0, PIXELS(638)),
    SC_SYNC,   SC_MOVE(PIXELS(256), 0, PIXELS(608)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeStorm_UnusedGaretPantingScript[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 192),
    SC_ADD(SW_SCALE_Y, 384),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, -384),
    SC_ADD(SW_SCALE_Y, -768),
    SC_WAIT(1),
    SC_LOOP(12, SC_TO(2)),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

s32 gValeStorm_BoulderScript[] = {
    SC_ARG(2),
    SC_END,
};

s32 gValeStorm_LeaderScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(94), 0, PIXELS(564)), SC_SYNC,
    SC_ARG(1), SC_SET(SW_FACING, FACING_WEST),      SC_WAIT(30),
    SC_END,
};

s32 gValeStorm_GaretScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(76), 0, PIXELS(564)), SC_SYNC,
    SC_ARG(1), SC_SET(SW_FACING, FACING_EAST),      SC_WAIT(30),
    SC_END,
};

s32 gValeStorm_DoraScriptA[] = {
    SC_ARG(2), SC_MOVE(PIXELS(178), 0, PIXELS(1190)),
    SC_SYNC,   SC_MOVE(PIXELS(149), 0, PIXELS(1194)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeStorm_JennaScriptA[] = {
    SC_ARG(2), SC_MOVE(PIXELS(198), 0, PIXELS(1176)),
    SC_SYNC,   SC_MOVE(PIXELS(170), 0, PIXELS(1195)),
    SC_SYNC,   SC_MOVE(PIXELS(149), 0, PIXELS(1194)),
    SC_SYNC,   SC_MOVE(PIXELS(149), 0, PIXELS(1175)),
    SC_SYNC,   SC_POS(0, 0, 0),
    SC_ARG(1), SC_END,
};

s32 gValeStorm_DoraScriptB[] = {
    SC_ARG(2),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(30),
    SC_POS(PIXELS(102), 0, PIXELS(1189)),
    SC_WAIT(1),
    SC_MOVE(PIXELS(102), 0, PIXELS(1218)),
    SC_SYNC,
    SC_MOVE(PIXELS(84), 0, PIXELS(1228)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_END,
};

s32 gValeStorm_JennaScriptB[] = {
    SC_ARG(2),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(30),
    SC_POS(PIXELS(102), 0, PIXELS(1189)),
    SC_SYNC,
    SC_MOVE(PIXELS(102), 0, PIXELS(1218)),
    SC_SYNC,
    SC_MOVE(PIXELS(120), 0, PIXELS(1228)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_END,
};

s32 gValeStorm_DoraScriptC[] = {
    SC_ARG(2), SC_MOVE(PIXELS(50), 0, PIXELS(1214)),
    SC_SYNC,   SC_MOVE(PIXELS(46), 0, PIXELS(1130)),
    SC_SYNC,   SC_MOVE(PIXELS(67), 0, PIXELS(1100)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeStorm_JennaScriptC[] = {
    SC_ARG(2), SC_MOVE(PIXELS(153), 0, PIXELS(1251)),
    SC_SYNC,   SC_MOVE(PIXELS(177), 0, PIXELS(1357)),
    SC_SYNC,   SC_POS(0, 0, 0),
    SC_ARG(1), SC_END,
};

s32 gValeStorm_DoraScriptD[] = {
    SC_ARG(2), SC_MOVE(PIXELS(123), 0, PIXELS(1078)),
    SC_SYNC,   SC_MOVE(PIXELS(148), 0, PIXELS(1078)),
    SC_SYNC,   SC_MOVE(PIXELS(148), 0, PIXELS(1064)),
    SC_SYNC,   SC_MOVE(PIXELS(35), 0, PIXELS(1064)),
    SC_SYNC,   SC_POS(0, 0, 0),
    SC_ARG(1), SC_END,
};

s32 gValeStorm_StormRunActions[] = {
    SC_ARG(2), SC_MOVE(PIXELS(102), 0, PIXELS(1236)),
    SC_SYNC,   SC_MOVE(PIXELS(148), 0, PIXELS(1256)),
    SC_SYNC,   SC_MOVE(PIXELS(177), 0, PIXELS(1303)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeStorm_ArriveScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(137), 0, PIXELS(1379)),
    SC_SYNC,   SC_MOVE(PIXELS(114), 0, PIXELS(1416)),
    SC_SYNC,   SC_MOVE(PIXELS(95), 0, PIXELS(1416)),
    SC_SYNC,   SC_MOVE(PIXELS(83), 0, PIXELS(1427)),
    SC_SYNC,   SC_MOVE(PIXELS(154), 0, PIXELS(1430)),
    SC_SYNC,   SC_MOVE(PIXELS(179), 0, PIXELS(1398)),
    SC_SYNC,   SC_MOVE(PIXELS(179), 0, PIXELS(1361)),
    SC_SYNC,   SC_END,
};

s32 gValeStorm_ExhaustedStrongmanGatherPath[] = {
    SC_MOVE(PIXELS(184), 0, PIXELS(1309)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0xb000),
    SC_END,
};

s32 gValeStorm_JennaGatherPath[] = {
    SC_MOVE(PIXELS(165), 0, PIXELS(1306)), SC_SYNC, SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTHWEST),   SC_END,
};

s32 gValeStorm_LeaderGatherPath[] = {
    SC_MOVE(PIXELS(183), 0, PIXELS(1325)), SC_SYNC, SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTHWEST),   SC_END,
};

s32 gValeStorm_GaretGatherPath[] = {
    SC_MOVE(PIXELS(166), 0, PIXELS(1325)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0xb000),
    SC_END,
};

s32 gValeStorm_LookAroundScript[] = {
    SC_CALL(ValeStorm_RerollHeadingOnCountdown),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValeStorm_SquashAndStretchScript[] = {
    SC_CALL(Scene_SquashAndStretch),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValeStorm_BoulderRiseScript[] = {
    SC_ARG(2),
    SC_SET(SW_SPEED, PIXELS(3)),
    SC_SET(SW_ACCEL, PIXELS(1)),
    SC_SET(0x23, 0),
    SC_SET(0x24, 1),
    SC_SET(SW_SCALE_X, 2 * Q16_ONE),
    SC_SET(SW_SCALE_Y, 2 * Q16_ONE),
    SC_MOVE(PIXELS(256), PIXELS(128), PIXELS(931)),
    SC_SYNC,
    SC_SE(145),
    SC_SET(0x23, 60),
    SC_SET(0x24, 2),
    SC_MOVE(PIXELS(228), PIXELS(149), PIXELS(957)),
    SC_SYNC,
    SC_SET(0x24, 3),
    SC_MOVE(PIXELS(201), PIXELS(160), PIXELS(984)),
    SC_SYNC,
    SC_SET(0x24, 4),
    SC_MOVE(PIXELS(173), PIXELS(138), PIXELS(1011)),
    SC_SYNC,
    SC_SET(0x24, 5),
    SC_MOVE(PIXELS(146), PIXELS(96), PIXELS(1038)),
    SC_SYNC,
    SC_SE(145),
    SC_SET(SW_SPEED, PIXELS(3) / 2),
    SC_SET(SW_ACCEL, PIXELS(1) / 2),
    SC_SET(0x23, 60),
    SC_SET(0x24, 6),
    SC_MOVE(PIXELS(161), PIXELS(107), PIXELS(1050)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCEL, PIXELS(3) / 5),
    SC_SET(0x24, 7),
    SC_MOVE(PIXELS(177), PIXELS(96), PIXELS(1062)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(5) / 2),
    SC_SET(SW_ACCEL, PIXELS(4) / 5),
    SC_SET(0x24, 8),
    SC_MOVE(PIXELS(201), PIXELS(74), PIXELS(1074)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(3)),
    SC_SET(SW_ACCEL, PIXELS(1)),
    SC_SET(0x24, 9),
    SC_MOVE(PIXELS(225), -PIXELS(64), PIXELS(1086)),
    SC_SYNC,
    SC_SET(0x23, 0),
    SC_SET(0x24, 0),
    SC_END,
};

struct SceneEntrance gValeStorm_Entrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 167, 0, 1281, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {1, CONDITION_ALWAYS, 257, 0, 456, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 113, 0, 303, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_STORM_FROM_VALE_KRADEN_NORTH, CONDITION_ALWAYS, 27, 0, 621,
     FACING_EAST, 0, -1, -1, -1, -1, 0},
    {VALE_STORM_FROM_VALE_KRADEN_SOUTH, CONDITION_ALWAYS, 29, 0, 792,
     FACING_EAST, 0, -1, -1, -1, -1, 0},
    {VALE_STORM_FROM_VALE_GATE, CONDITION_ALWAYS, 458, 0, 1393, FACING_WEST, 0,
     -1, -1, -1, -1, 0},
    {VALE_STORM_FROM_VALE_INTERIORS_SHOPS_EAST, CONDITION_ALWAYS, 406, 0, 743,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_STORM_FROM_VALE_INTERIORS_SHOPS_SOUTH, CONDITION_ALWAYS, 262, 0, 821,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_STORM_FROM_VALE_INTERIORS_INN_ISAACS_HOUSE, CONDITION_ALWAYS, 340, 0,
     904, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_STORM_FROM_VALE_INTERIORS_SHOPS_NORTH, CONDITION_ALWAYS, 326, 0, 1142,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_STORM_FROM_VALE_INTERIORS_SHOPS_WEST, CONDITION_ALWAYS, 374, 0, 1254,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 102, 0, 1222, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_STORM_FROM_VALE_INTERIORS_INN_WEST, CONDITION_ALWAYS, 101, 0, 1216,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_STORM_FROM_VALE_INTERIORS_INN_EAST, CONDITION_ALWAYS, 146, 0, 1193,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA, CONDITION_ALWAYS, 335, 0,
     908, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {VALE_STORM_FROM_BOULDER_PASSED, CONDITION_ALWAYS, 181, 0, 1273,
     FACING_NORTHEAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gValeStorm_Exits[] = {
    SCENE_EXITS(ValeStorm),
    SCENE_EXIT(VALE_STORM_TO_VALE_GATE, ValeGate, VALE_GATE_FROM_VALE),
    SCENE_EXIT(VALE_STORM_TO_VALE_KRADEN_NORTH, ValeKraden,
               VALE_KRADEN_FROM_VALE_NORTH),
    SCENE_EXIT(VALE_STORM_TO_VALE_KRADEN_SOUTH, ValeKraden,
               VALE_KRADEN_FROM_VALE_SOUTH),
    SCENE_EXIT(VALE_STORM_TO_VALE_INTERIORS_SHOPS_EAST, ValeInteriorsShops,
               VALE_INTERIORS_SHOPS_FROM_VALE_EAST),
    SCENE_EXIT(VALE_STORM_TO_VALE_INTERIORS_SHOPS_SOUTH, ValeInteriorsShops,
               VALE_INTERIORS_SHOPS_FROM_VALE_SOUTH),
    SCENE_EXIT(VALE_STORM_TO_VALE_INTERIORS_INN_ISAACS_HOUSE, ValeInteriorsInn,
               VALE_INTERIORS_INN_FROM_VALE),
    SCENE_EXIT(VALE_STORM_TO_VALE_INTERIORS_SHOPS_NORTH, ValeInteriorsShops,
               VALE_INTERIORS_SHOPS_FROM_VALE_NORTH),
    SCENE_EXIT(VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST, ValeInteriorsShops,
               VALE_INTERIORS_SHOPS_FROM_VALE_WEST),
    SCENE_EXIT(VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SECOND_DOOR,
               ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_WEST),
    SCENE_EXIT(VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SANCTUM_DOOR,
               ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_WEST),
    SCENE_EXIT(VALE_STORM_TO_VALE_INTERIORS_INN_WEST, ValeInteriorsInn,
               VALE_INTERIORS_INN_FROM_VALE_STORM_WEST),
    SCENE_EXIT(VALE_STORM_TO_VALE_INTERIORS_INN_EAST, ValeInteriorsInn,
               VALE_INTERIORS_INN_FROM_VALE_STORM_EAST),
    SCENE_EXIT(VALE_STORM_TO_BOULDER_PASSED, ValeStorm,
               VALE_STORM_FROM_BOULDER_PASSED),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gValeStorm_Placements[] = {
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_DORA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_KYLE, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(437), 0, PIXELS(762), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(322), 0, PIXELS(1433), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(144), 0, PIXELS(1070), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(276), 0, PIXELS(1270), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xd2, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(420), 0, PIXELS(619), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { GARET_CHILD, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(396), 0, PIXELS(619), FACING_EAST, TALK_FACE_PARTY, 0 },
    { FELIX_CHILD, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_GREEN_SHIRT, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WOMAN_WITH_PINK_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { JENNA_CHILD, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x120, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeStorm_BoulderWatchers[] = {
    { SPRITE_BLANK, CONDITION_ALWAYS, 7,
        PIXELS(124), 0, PIXELS(375), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_BOY_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(85), 0, PIXELS(361), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_BOY_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(135), 0, PIXELS(363), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, 7,
        PIXELS(100), 0, PIXELS(375), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(124), 0, PIXELS(375), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(100), 0, PIXELS(375), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gValeStorm_Events[] = {
    {EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByMissingExit)},
    {EVENT_TOUCH, 3, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveToValeKradenNorth)},
    {EVENT_TOUCH, 4, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveToValeKradenSouth)},
    {EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeGate)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveToValeInteriorsShopsEast)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveToValeInteriorsShops)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveToValeInteriorsInn)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveToValeInteriorsShopsNorth)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveToValeInteriorsShopsWest)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveToValeInteriorsShopsWestBySecondDoor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveToValeInteriorsInnWest)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 13, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LeaveToValeInteriorsInnEast)},
    {EVENT_TALK, ACTOR_GARET, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_TalkToGaret)},
    {EVENT_TALK, ACTOR_KYLE, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_TalkAboutSavingFelix)},
    {EVENT_TALK, ACTOR_JENNAS_FATHER, CONDITION_ALWAYS,
     MSGNO(MsgValeHangOnFelixHelpIsOn)},
    {EVENT_TALK, ACTOR_JENNAS_MOTHER, CONDITION_ALWAYS,
     MSGNO(MsgValeDontLetGoFelix)},
    {EVENT_TALK, ACTOR_DORA, CONDITION_ALWAYS, MSGNO(MsgValeNoItCantBeKyleIt)},
    {EVENT_TALK, ACTOR_JENNA, CONDITION_ALWAYS,
     MSGNO(MsgValeMomDadFelixPleaseDontLeave)},
    {EVENT_TALK, ACTOR_EXHAUSTED_STRONGMAN, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_StrongmanTriesToGoForHelp)},
    {EVENT_TALK, ACTOR_FIRST_ROCK, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {EVENT_TALK, ACTOR_FIRST_ROCK + 1, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {EVENT_TALK, ACTOR_FIRST_ROCK + 2, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {EVENT_TALK, ACTOR_FIRST_ROCK + 3, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {EVENT_TALK, ACTOR_FIRST_ROCK + 4, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {EVENT_TALK, ACTOR_FIRST_ROCK + 5, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {EVENT_TALK, ACTOR_FIRST_ROCK + 6, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {EVENT_TALK, ACTOR_FIRST_ROCK + 7, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {EVENT_TOUCH, 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_SetPathCellOverride)},
    {EVENT_TOUCH, 14, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ClearPathCellOverride)},
    {EVENT_TOUCH, 41, CONDITION_ALWAYS, EVENT_SCRIPT(Event_FirstRockFalls)},
    {EVENT_TOUCH, 42, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SecondRockFalls)},
    {EVENT_TOUCH, 43, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ThirdRockFalls)},
    {EVENT_TOUCH, 44, CONDITION_ALWAYS, EVENT_SCRIPT(Event_FourthRockFalls)},
    {EVENT_TOUCH, 30, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_HearGaretStruggling)},
    {EVENT_TOUCH, 31, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_GaretCallsAfterIsaac)},
    {EVENT_TOUCH, 32, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_UnusedGaretJoinsIsaac)},
    {EVENT_TOUCH, 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WarnAboutMtAleph)},
    {EVENT_TOUCH, 34, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WatchBoulderFall)},
    {EVENT_TOUCH, 35, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_WatchAttemptToSaveFelix)},
    {EVENT_TOUCH, 36, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_BoulderSweepsThemAway)},
    {EVENT_TOUCH, 16, FLAG_VALE_STORM_THIRD_MONSTER_FOUGHT,
     EVENT_SCRIPT(Event_FightThirdMonster)},
    {EVENT_TOUCH, 17, FLAG_VALE_STORM_FOURTH_MONSTER_FOUGHT,
     EVENT_SCRIPT(Event_FightFourthMonster)},
    {SEARCH(SEARCH_UNNAMED), 24, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_SearchBrokenDoor)},
    {SEARCH(SEARCH_UNNAMED), 19, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_SearchValuablesChest)},
    {SCENE_EVENTS_END},
};

u16 gValeStorm_CellSteps0[] = {
    MAP_CELL_STEP(0, 98, 2, 2, 2),
    MAP_CELL_STEP(2, 98, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

u16 gValeStorm_CellSteps1[] = {
    MAP_CELL_STEP(0, 96, 2, 2, 2),
    MAP_CELL_STEP(2, 96, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

u16 gValeStorm_CellSteps2[] = {
    MAP_CELL_STEP(4, 98, 2, 2, 2),
    MAP_CELL_STEP(6, 98, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

u16 gValeStorm_CellSteps3[] = {
    MAP_CELL_STEP(4, 96, 2, 2, 2),
    MAP_CELL_STEP(6, 96, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

u16 gValeStorm_CellSteps4[] = {
    MAP_CELL_STEP(0, 96, 2, 2, 2),
    MAP_CELL_STEP(50, 44, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

u16 gValeStorm_CellSteps5[] = {
    MAP_CELL_STEP(4, 98, 2, 2, 2),
    MAP_CELL_STEP(44, 108, 2, 2, 2),
    MAP_CELL_STEPS_END,
};
