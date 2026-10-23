/// @file
/// Vale on the stormy night (SceneId_ValeStorm), the prologue, when Isaac and
/// Garet are children: a map of its own that stands in for SceneId_Vale while
/// FLAG_VALE_STORM is set, so that the doors that lead to Vale by day lead
/// here. ValeStorm_Initialize sets the flag on every arrival, and vale_gate.c
/// clears it when the night ends.
///
/// The night is a chain of story flags. Each is set by one event and read by
/// ValeStorm_Initialize, which rebuilds the scene at that stage on the next
/// arrival:
///
/// - FLAG_VALE_STORM_SENT_TO_PLAZA: Event_DoraSendsIsaacToThePlaza, played on
///   the first arrival by VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA. Dora
///   and Kyle send Isaac on to the plaza and go to help; until the flag is set
///   the lightning waits for the cutscene.
/// - FLAG_VALE_STORM_FIRST_ROCK_FELL to FLAG_VALE_STORM_FOURTH_ROCK_FELL, with
///   the place flags FLAG_VALE_STORM_FIRST_ROCK_PASSED to
///   FLAG_VALE_STORM_FOURTH_ROCK_PASSED: Event_FirstRockFalls to
///   Event_FourthRockFalls drop a rock across a path at four spots. The _FELL
///   flag puts the rock and the collision cells it blocks back; the _PASSED
///   flag, cleared once the party leaves Vale, keeps the event from running
///   again.
/// - FLAG_VALE_STORM_BUMPED_INTO_VILLAGER: ValeStorm_BumpIntoShovingMan, the
///   first time Isaac tries the door of
///   VALE_STORM_TO_VALE_INTERIORS_SHOPS_SOUTH.
/// - FLAG_VALE_STORM_HEARD_GARET, then FLAG_VALE_STORM_GARET_JOINED: Garet is
///   trying to save his chest of valuables; talking to him, or walking off
///   past him, brings him into the party (ValeStorm_GaretLeavesHisChest) and
///   leaves the chest by his door. Event_UnusedGaretJoinsIsaac is a third way
///   that never plays. With Garet along, a fallen rock and the path north to
///   Mt. Aleph have him turn Isaac back.
/// - FLAG_VALE_BOULDER_FELL: Event_WatchBoulderFall, the elders holding back
///   the Boulder with their Psynergy as it slips; from then on it rests where
///   it slipped to.
/// - FLAG_VALE_STORM_SAW_FELIX_HANGING_ON: Event_WatchAttemptToSaveFelix and
///   ValeStorm_DoraGoesForHelp, Felix clinging on in the river. The plaza
///   (vale_gate.c) then sets FLAG_VALE_STORM_SENT_TO_AID_ELDERS and sends the
///   boys back.
/// - FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY: Event_BoulderSweepsThemAway and
///   ValeStorm_ShowBoulderSweepingDownriver, the Boulder carrying Felix, his
///   parents and Kyle away down the river.
/// - FLAG_VALE_STORM_GARET_LEFT: ValeStorm_GaretFollowsIsaac, as Isaac leaves
///   for the plaza again; the night ends in vale_gate.c.
///
/// Every way out goes through Vale_RequestBackdropExit (common/vale.h), which
/// stops the lightning first, and each door plays its opening cells before it;
/// gValeStorm_Exits gives each exit's destination. Two cells start a battle
/// once each (slots 2 and 3 of ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT).
///
/// Open questions:
/// - None.

#include "action_script.h"
#include "game_state.h"
#include "unit/import.h"
#include "unit/map/vale_storm.h"
#include "unit/map/common/effect_psynergy.h"
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
#include "script_layer.h"
#include "map_tables.h"
#include "call.h"
#include "field_service.h"
#include "object_event.h"
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
#include "audio_engine_symbols.h"
#include "io_reg.h"
#include "fx_work.h"

static void Event_DoraSendsIsaacToThePlaza(void);
static void ValeStorm_ApplyFirstRockCollision(void);
static void ValeStorm_ApplySecondRockCollision(void);
static void ValeStorm_ApplyThirdRockCollision(void);
static void ValeStorm_ApplyFourthRockCollision(void);
static void ValeStorm_BumpIntoShovingMan(void);
static void ValeStorm_GaretLeavesHisChest(void);
static void ValeStorm_GaretCommentsOnBlockedPath(s32 x, s32 y);
static void ValeStorm_DoraGoesForHelp(void);
static void ValeStorm_ShowBoulderSweepingDownriver(void);
static void ValeStorm_FlashLightning(void);
static void ValeStorm_UpdateBoulderShadow(void);
static void ValeStorm_FlickerBoulderShadow(void);
static void ValeStorm_GaretFollowsIsaac(void);
static void ValeStorm_RebuildRiverbank(void);
static void ValeStorm_FlashPaletteAndSpawnEffects(struct FieldActor *actor);
static void ValeStorm_SpawnEffectPair(struct FieldActor *parent);
static void ValeStorm_UpdateBoulderSceneEffects(void);
static void ValeStorm_CycleBoulderPalette(void);
static void ValeStorm_CopyLeaderSpritePriority(void);

/// The placed actors, numbered in the order of gValeStorm_Placements; the
/// elders holding back the Boulder (gValeStorm_BoulderWatchers) follow them.
enum StormNightActor {
  /// The strongman who comes back to the river with Jenna, his Psynergy
  /// restored at the Psynergy Stone (vale_gate.c); after the Boulder has
  /// passed he tries to go for help and cannot. "I... I'll go!", then "It's no
  /// use. I can't move..."
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
  ACTOR_GARET,
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
  ACTOR_JENNA,
  /// The Boulder's shadow (sprite 0x120, a black blob), drawn on alternate
  /// frames under the Boulder as it comes down the river, swelling and
  /// shrinking with its bounds (ValeStorm_UpdateBoulderShadow), and twice its
  /// size during the frozen flashes (ValeStorm_FlickerBoulderShadow).
  /// ★ Named from what the player sees: a flickering black shadow under the
  /// Boulder, Event_BoulderSweepsThemAway.gif (its render)
  ACTOR_BOULDER_SHADOW,
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
  /// The westernmost elder, drawn with the sprite of the young man with brown
  /// hair (as is ACTOR_ELDER_FAR_EAST), tells the boys to run. "Hurry, boys!
  /// Run!!! It's just a little farther!"
  /// ★ Named from the game: "Hurry, boys!" (MsgValeTheBoulderIsFalling)
  ACTOR_ELDER_FAR_WEST,
  /// The easternmost elder, the same sprite drawn mirrored. "We have to hold on
  /// until the villagers are safe!"
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

/// Animations of the night's own sprites beyond enum ActorAnimation, as each
/// sprite's animation table and frames under graphics/sprite draw them.
enum StormNightAnimation {
  /// Garet leaning into his chest to drag it (spr_019.png).
  ANIMATION_GARET_DRAG = 5,
  /// Kyle crouched at the bank, reaching out to Felix (spr_053.png).
  ANIMATION_KYLE_REACH_OUT = 5,
  /// Jenna's father and mother crouched at the bank, reaching out
  /// (spr_059.png, spr_060.png).
  ANIMATION_PARENT_REACH_OUT = 6,
  /// The strongman fallen back on the ground (spr_116.png).
  ANIMATION_STRONGMAN_SPRAWLED = 6,
  /// Only Felix's hands above the water as he goes under (spr_036.png).
  ANIMATION_FELIX_UNDER_WATER = 6,
  /// Felix clinging on in the river, one arm reaching out (spr_036.png).
  ANIMATION_FELIX_CLING = 7,
  /// A bearded elder seen from behind, both arms raised against the Boulder
  /// (spr_122.png).
  ANIMATION_BEARDED_ELDER_HOLD_BACK = 5,
  /// The young man seen from behind, both arms raised against the Boulder
  /// (spr_108.png).
  ANIMATION_YOUNG_ELDER_HOLD_BACK = 8,
  /// The young man facing the player, his hands raised to his mouth to call
  /// out (spr_108.png).
  ANIMATION_YOUNG_ELDER_CALL_OUT = 9,
  /// The strongman propped up on one hand, the two facing opposite ways; the
  /// leader's heading picks one (spr_116.png).
  ANIMATION_STRONGMAN_PROPPED_UP = 7,
  ANIMATION_STRONGMAN_PROPPED_UP_OTHER_WAY = 8,
  // The poses frozen in the lightning as the Boulder comes down the river
  // (ValeStorm_ShowBoulderSweepingDownriver).
  ANIMATION_STRONGMAN_RUN = 5,    ///< in mid-stride (spr_116.png)
  ANIMATION_DORA_RUN = 9,         ///< in mid-stride (spr_031.png)
  ANIMATION_JENNA_RUN = 5,        ///< in mid-stride (spr_048.png)
  ANIMATION_ISAAC_RECOIL = 17,    ///< arms flung out (spr_018.png)
  ANIMATION_PARENT_RECOIL = 5,    ///< seen from behind, arms flung out
  ANIMATION_FELIX_CLING_STILL = 8 ///< ANIMATION_FELIX_CLING's first frame held
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

/// The step of gValeStorm_LookAroundScript: the actor faces a random way every
/// 20 to 39 frames, as everyone looks about once the Boulder has gone by.
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
  Engine_MapAnimateCells(gValeStorm_SanctumDoorOpens, 45, 11);
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 257, 420);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(
      VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SANCTUM_DOOR);
}

/// Leaves through an exit the exit table doesn't list. In Vale on the stormy
/// night, stepping on cell code 2 requests exit 1, the one number the exit
/// table leaves out; Scene_TakeExit therefore falls back to the first exit's
/// scene, Vale's SceneId_ValeGate, at ENTRANCE_DEFAULT.
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
  Engine_MapAnimateCells(gValeStorm_IsaacsDoorOpens, 54, 32);
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 406, 727);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(VALE_STORM_TO_VALE_INTERIORS_SHOPS_EAST);
}

/// A stout man runs out of a house and shoves Isaac. In Vale on the stormy
/// night, this is Isaac's first time at the door of
/// VALE_STORM_TO_VALE_INTERIORS_SHOPS_SOUTH; afterwards the door opens as
/// usual. "Huh? Isaac?" Reached by walking north into cell code 7. The shove
/// plays until FLAG_VALE_STORM_BUMPED_INTO_VILLAGER or
/// FLAG_VALE_STORM_SECOND_ROCK_FELL is set, and sets FLAG_SCENE_TEMP_06;
/// otherwise goes into a house of Vale (SceneId_ValeInteriorsShops, at
/// VALE_INTERIORS_SHOPS_FROM_VALE_SOUTH).
static void Event_LeaveToValeInteriorsShops(void) {
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_06) == 0) {
    Event_PlayCue(SE_DOOR_OPEN);
    Map_AnimateCells(gValeStorm_GrandparentsDoorOpens, 45, 39);
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

/// Goes back into Isaac's house on the stormy night. It's the door he and Dora
/// came out of in Vale, and it opens unless they left it open.
/// Reached by walking north into cell code 8; takes
/// VALE_STORM_TO_VALE_INTERIORS_INN_ISAACS_HOUSE, to
/// VALE_INTERIORS_INN_FROM_VALE in SceneId_ValeInteriorsInn, where Vale's
/// VALE_TO_VALE_INTERIORS_INN leads by day.
static void Event_LeaveToValeInteriorsInn(void) {
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_05) == 0) {
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapAnimateCells(gValeStorm_IsaacsDoorOpens, 50, 44);
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
  Engine_MapAnimateCells(gValeStorm_GrandparentsDoorOpens, 49, 69);
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
  Engine_MapAnimateCells(gValeStorm_WestHouseDoorOpens, 52, 76);
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 374, 1238);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST);
}

/// Unused: its row answers cell code 11, which no cell of the stormy night's
/// map (layout/layout_000) carries. It would have opened Isaac's door, as
/// Event_LeaveToValeInteriorsInnWest does for code 12, walked the leader to the
/// same spot and taken VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SECOND_DOOR into
/// the house Event_LeaveToValeInteriorsShopsWest leads to
/// (VALE_INTERIORS_SHOPS_FROM_VALE_WEST).
static void Event_UnusedLeaveToValeInteriorsShopsWestBySecondDoor(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_IsaacsDoorOpens, 35, 74);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 1206);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SECOND_DOOR);
}

/// In Vale on the stormy night, opens a door and goes in.
/// Reached by walking north into cell code 12; arrives in another part of Vale
/// at VALE_INTERIORS_INN_FROM_VALE_STORM_WEST.
static void Event_LeaveToValeInteriorsInnWest(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_IsaacsDoorOpens, 35, 73);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 1206);
  Engine_EventWait(3);
  Vale_RequestBackdropExit(VALE_STORM_TO_VALE_INTERIORS_INN_WEST);
}

/// In Vale on the stormy night, opens a door and goes in.
/// Reached by walking north into cell code 13; arrives in another part of Vale
/// at VALE_INTERIORS_INN_FROM_VALE_STORM_EAST.
static void Event_LeaveToValeInteriorsInnEast(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_GrandparentsDoorOpens, 38, 72);
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
  FieldEffect_SetQueuedSoundAndPlay(SE_170);
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
  // The rocks are big, so the leader can talk to them from further off: the
  // talk target search counts their distance at 10/13 (ACTOR_NEAR_TALK).
  Object_GetById(ACTOR_FIRST_ROCK)->collisionFlags |= ACTOR_NEAR_TALK;
  Object_GetById(ACTOR_FIRST_ROCK + 1)->collisionFlags |= ACTOR_NEAR_TALK;
  Object_GetById(ACTOR_FIRST_ROCK + 2)->collisionFlags |= ACTOR_NEAR_TALK;
  Object_GetById(ACTOR_FIRST_ROCK + 3)->collisionFlags |= ACTOR_NEAR_TALK;
  Object_GetById(ACTOR_FIRST_ROCK + 4)->collisionFlags |= ACTOR_NEAR_TALK;
  Object_GetById(ACTOR_FIRST_ROCK + 5)->collisionFlags |= ACTOR_NEAR_TALK;
  Object_GetById(ACTOR_FIRST_ROCK + 6)->collisionFlags |= ACTOR_NEAR_TALK;
  Object_GetById(ACTOR_FIRST_ROCK + 7)->collisionFlags |= ACTOR_NEAR_TALK;
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
    Actor_SetActionScript(ACTOR_BOULDER, gValeStorm_BoulderWaits);
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
    Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                             ANIMATION_STRONGMAN_SPRAWLED);
    Object_GetById(ACTOR_GARET)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    Object_GetById(ACTOR_EXHAUSTED_STRONGMAN)->priorityFlags &=
        ~ACTOR_PRIORITY_AUTOMATIC;
    FieldScheduler_AddCallback(ValeStorm_CopyLeaderSpritePriority,
                               TASK_PRIORITY_SCENE);
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
    Engine_ActorSetAnimation(ACTOR_KYLE, ANIMATION_KYLE_REACH_OUT);
    {
      struct FieldActor *actor;

      actor = Object_GetById(ACTOR_KYLE);
      actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    }
    table = gValeStorm_SquashAndStretchScript;
    Actor_SetActionScript(ACTOR_KYLE, table);
    Actor_SetPosition(ACTOR_JENNAS_FATHER, PIXELS(227), PIXELS(1214));
    Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_SOUTH, 0);
    Engine_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIMATION_PARENT_REACH_OUT);
    {
      struct FieldActor *actor;

      actor = Object_GetById(ACTOR_JENNAS_FATHER);
      actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    }
    Actor_SetActionScript(ACTOR_JENNAS_FATHER, table);
    Actor_SetPosition(ACTOR_JENNAS_MOTHER, PIXELS(247), PIXELS(1214));
    Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_SOUTH, 0);
    Engine_ActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIMATION_PARENT_REACH_OUT);
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
    FieldEffect_StartGroundParticles();
    FieldEffect_StartLightning();
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
    actor->collisionFlags = ACTOR_NOT_TARGETED;
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
  Engine_ActorSetAnimation(ACTOR_FELIX, ANIMATION_FELIX_CLING);
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_JOINED) == 0) {
    Engine_EventStart();
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    Actor_SetPosition(ACTOR_GARET, PIXELS(400), PIXELS(611));
    Actor_SetPosition(ACTOR_GARETS_CHEST, PIXELS(424), PIXELS(627));
    Actor_WalkTo(ACTOR_GARET, 400, 619);
    Actor_WalkToAndWait(ACTOR_GARETS_CHEST, 424, 619);
    Engine_ActorSetAnimation(ACTOR_GARETS_CHEST, ANIMATION_WALK);
    Engine_ActorSetAnimation(ACTOR_GARET, ANIMATION_GARET_DRAG);
    Engine_EventEnd();
  } else {
    Engine_EventStart();
    Actor_SetPosition(ACTOR_GARETS_CHEST, PIXELS(424), PIXELS(627));
    Actor_WalkToAndWait(ACTOR_GARETS_CHEST, 424, 619);
    Engine_ActorSetAnimation(ACTOR_GARETS_CHEST, ANIMATION_NOD);
    Engine_EventEnd();
  }
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 24;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  FieldEffect_ResumeLightning();
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

  Engine_EventStart();
  FieldEffect_StartGroundParticles();
  FieldEffect_StartLightning();
  FieldEffect_LastLightning();
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
  FieldEffect_ResumeLightning();
  // Dora and Isaac come out of the door.
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_IsaacsDoorOpens, 50, 44);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_KYLE, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_DORA, PIXELS(342), PIXELS(890));
  Actor_WalkToAndWait(ACTOR_DORA, 342, 905);
  FieldEffect_PlayQueuedSound();
  Actor_WalkTo(ACTOR_DORA, 296, 905);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(342), PIXELS(890));
  Actor_WalkTo(ACTOR_PARTY_LEADER, 342, 890);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 342, 905);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 318, 905);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIMATION_STAND);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 60);
  // Dora: "Kyle... Will they be able to stop the Boulder?" Kyle: "I don't think
  // so..." and tells the two to take refuge in the plaza.
  message = MsgValeKyleAbleStop;
  Engine_EventSetMessage(message);
  Event_ShowMessage(ACTOR_DORA, 0);
  Actor_WalkToAndWait(ACTOR_KYLE, 294, 838);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_KYLE, ANIMATION_SHAKE_HEAD);
  Event_ShowMessage(ACTOR_KYLE, 0);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_KYLE, FACING_SOUTH, 0);
  Event_ShowMessageAndWait(ACTOR_KYLE, 0, 20);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 10);
  // "Aren't you coming?" "I need to help evacuate the other villagers."
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_KYLE, ANIMATION_SHAKE_HEAD);
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
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIMATION_SHAKE_HEAD);
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
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIMATION_NOD);
  // "You know the way--just go south to get to the plaza. Be careful!"
  Engine_EventSetMessage(MsgValeKnowWayGo);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  // Kyle and Dora run north toward the Boulder and out of sight.
  Actor_SetSpeed(ACTOR_KYLE, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_WalkTo(ACTOR_KYLE, 297, 750);
  Engine_EventWait(10);
  Actor_WalkToAndWait(ACTOR_DORA, 297, 750);
  Actor_SetPosition(ACTOR_DORA, 0, 0);
  Actor_SetPosition(ACTOR_KYLE, 0, 0);
  Engine_ActorSetAnimation(ACTOR_KYLE, ANIMATION_STAND);
  Engine_ActorSetAnimation(ACTOR_GARETS_CHEST, ANIMATION_WALK);
  Engine_ActorSetAnimation(ACTOR_GARET, ANIMATION_GARET_DRAG);
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
    Engine_EventStart();
    if (GameFlag_IsSet(FLAG_VALE_STORM_FIRST_ROCK_FELL) == 0) {
      rock = Object_GetById(ACTOR_FIRST_ROCK);
      rockSprite = rock->sprite;
      // A rumble (SE_141, then SE_145), and the rock bounces down from the
      // east.
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
      // It lands: the rumble on player 3 fades and the shaking dies down.
      Engine_AudioPlayCue(AUDIO_CUE_FADE_SLOT3);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      Engine_MapWaitWorkValuesBelow256();
      FieldEffect_PlayQueuedSound();
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
    Engine_EventStart();
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
      // It lands: the rumble on player 3 fades and the shaking dies down.
      Engine_AudioPlayCue(AUDIO_CUE_FADE_SLOT3);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      Engine_MapWaitWorkValuesBelow256();
      FieldEffect_PlayQueuedSound();
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
    Engine_EventStart();
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
      // It lands: the rumble fades, and Isaac is drawn as before.
      Event_PlayCue(AUDIO_CUE_FADE_SLOT3);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      Engine_MapWaitWorkValuesBelow256();
      FieldEffect_PlayQueuedSound();
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
    Engine_EventStart();
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
      // It lands: the rumble on player 3 fades and the shaking dies down.
      Engine_AudioPlayCue(AUDIO_CUE_FADE_SLOT3);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      Engine_MapWaitWorkValuesBelow256();
      FieldEffect_PlayQueuedSound();
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

  Engine_EventStart();
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 262, 810);
  Actor_SetPosition(ACTOR_SHOVING_MAN, PIXELS(262), PIXELS(805));
  Actor_WalkTo(ACTOR_SHOVING_MAN, 262, 825);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 282, 855);
  Engine_ActorSetAnimation(ACTOR_SHOVING_MAN, ANIMATION_STAND);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_SHOVING_MAN, 0);
  FieldEffect_PlayQueuedSound();
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_ShowEmote(ACTOR_SHOVING_MAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
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
      Engine_EventStart();
      Engine_EventSetMessage(MsgValeUghHrnghhh);
      Event_ShowMessageAndWait(ACTOR_GARET, 0, 20);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
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
/// Garet joins; ValeStorm_GaretLeavesHisChest brings him along.
static void Event_GaretCallsAfterIsaac(void) {
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_JOINED) == 0 &&
      Engine_GameFlagIsSet(FLAG_VALE_STORM_HEARD_GARET) != 0) {
    Engine_EventStart();
    Engine_ActorRunRepeatedMotion(ACTOR_GARET, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgValeDontLeaveMeHere);
    ValeStorm_GaretLeavesHisChest();
    Engine_EventEnd();
  }
}

/// In Vale on the stormy night, talking to Garet: whether to leave his things
/// behind, or, after the river, the Boulder.
/// Reached by talking to Garet: before he joins, ValeStorm_GaretLeavesHisChest
/// brings him along.
static void Event_TalkToGaret(void) {
  if (GameFlag_IsSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) != 0) {
    Engine_EventStart();
    Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgValeBoulderNeedGet);
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
    Engine_EventEnd();
  } else {
    if (GameFlag_IsSet(FLAG_VALE_STORM_GARET_JOINED) == 0) {
      Engine_EventStart();
      Engine_EventSetMessage(MsgValeWantDumpStuff);
      ValeStorm_GaretLeavesHisChest();
      Engine_EventEnd();
    }
  }
}

/// Garet asks the question the caller chose and, on either answer, drops his
/// chest and joins the party. A yes has him agree to forget his things, a no
/// has him decide a rock would cost him chest and life alike. Called by
/// Event_GaretCallsAfterIsaac and Event_TalkToGaret; sets
/// FLAG_VALE_STORM_GARET_JOINED.
static void ValeStorm_GaretLeavesHisChest(void) {
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
  Engine_ActorSetAnimation(ACTOR_GARETS_CHEST, ANIMATION_NOD);
  Engine_ActorSetAnimation(ACTOR_GARET, ANIMATION_STAND);
  Engine_EventWait(40);
  Engine_ActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_GARET, ANIMATION_NOD);
  if (agreedToLeaveBelongings != 0) {
    Engine_EventSetMessage(MsgValeKnowRightOk);
  } else {
    Engine_EventSetMessage(MsgValeRightDitchStuff);
  }
  Engine_EventShowMessage(ACTOR_GARET, 0);
  Engine_ActorSetAnimation(ACTOR_GARET, ANIMATION_WALK);
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
    Engine_EventStart();
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
    message = MsgValeHey;
    Engine_EventSetMessage(message);
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
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
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_GARET, ANIMATION_NOD);
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
    Engine_ActorSetAnimation(ACTOR_GARET, ANIMATION_WALK);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Actor_SetDestination(ACTOR_GARET, actor->x.part.pixel,
                           actor->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_GARET);
    Actor_SetPosition(ACTOR_GARET, 0, 0);
    Event_PrepareObjectAndApplyValue(1, 1);
    Engine_ActorSetAnimation(ACTOR_GARETS_CHEST, ANIMATION_NOD);
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

  Engine_EventStart();
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
  Engine_ActorSetAnimation(ACTOR_GARET, ANIMATION_WALK);
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
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimation(ACTOR_GARET, ANIMATION_WALK);
  leaderAtEnd = Object_GetById(ACTOR_PARTY_LEADER);
  if (leaderAtEnd != 0) {
    Engine_ActorSetDestination(ACTOR_GARET, leaderAtEnd->x.part.pixel,
                               leaderAtEnd->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_GARET);
  Engine_ActorSetPosition(ACTOR_GARET, 0, 0);
}

/// The Boulder slips toward the village and the elders tell the boys to run.
/// In Vale on the stormy night, the elders hold the Boulder with their
/// Psynergy as it slips.
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
    Engine_EventStart();
    Event_CallWithLastActiveObjectId(gValeStorm_BoulderWatchers);
    Scene_StartCutscenePsynergy();
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
    Actor_SetActionScript(ACTOR_PARTY_LEADER, gValeStorm_BoulderFallLeaderPath);
    Actor_RunActionScriptAndWait(ACTOR_GARET, gValeStorm_BoulderFallGaretPath);
    Object_RefreshSelectorById(ACTOR_PARTY_LEADER);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
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
    Engine_ActorSetAnimation(ACTOR_ELDER_EAST,
                             ANIMATION_BEARDED_ELDER_HOLD_BACK);
    Engine_ActorSetAnimation(ACTOR_ELDER_WEST,
                             ANIMATION_BEARDED_ELDER_HOLD_BACK);
    Engine_ActorSetAnimation(ACTOR_ELDER_FAR_EAST,
                             ANIMATION_YOUNG_ELDER_HOLD_BACK);
    Engine_ActorSetAnimation(ACTOR_ELDER_FAR_WEST,
                             ANIMATION_YOUNG_ELDER_HOLD_BACK);
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
    FieldScheduler_AddOrUpdateCallback(updateEffects, TASK_PRIORITY_SCENE);
    cycleBoulderPalette = ValeStorm_CycleBoulderPalette;
    FieldScheduler_AddCallback(cycleBoulderPalette, TASK_PRIORITY_SCENE);
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
    Engine_ActorSetAnimation(ACTOR_BOULDER, ANIMATION_WALK);
    Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Event_PlayCue(SE_145);
    *shakeShift = 0;
    Actor_SetSpeed(ACTOR_BOULDER, PIXELS(2) / 5, PIXELS(1) / 5);
    Actor_MoveToAndWait(ACTOR_BOULDER, 114, 300);
    Engine_ActorSetAnimation(ACTOR_BOULDER, ANIMATION_WALK);
    Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
    Event_PlayCue(SE_145);
    *shakeShift = 2;
    Actor_SetSpeed(ACTOR_BOULDER, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_MoveToAndWait(ACTOR_BOULDER, 114, 333);
    Engine_ActorSetAnimation(ACTOR_BOULDER, ANIMATION_WALK);
    Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Event_PlayCue(SE_145);
    *shakeShift = 0;
    Actor_SetSpeed(ACTOR_BOULDER, PIXELS(2) / 5, PIXELS(1) / 5);
    Actor_MoveToAndWait(ACTOR_BOULDER, 114, 300);
    Engine_ActorSetAnimation(ACTOR_BOULDER, ANIMATION_WALK);
    Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
    Event_PlayCue(SE_145);
    *shakeShift = 2;
    Actor_SetSpeed(ACTOR_BOULDER, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_MoveToAndWait(ACTOR_BOULDER, 114, 333);
    Engine_ActorSetAnimation(ACTOR_BOULDER, ANIMATION_WALK);
    Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Event_PlayCue(SE_145);
    *shakeShift = 1;
    Engine_EventWait(20);
    Actor_SetAttachedEffect(ACTOR_ELDER_EAST,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ActorRunRepeatedMotion(ACTOR_ELDER_EAST, 2);
    Event_ShowMessage(ACTOR_ELDER_WEST_STAND_IN, 0);
    Actor_ShowEmote(ACTOR_ELDER_WEST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_ELDER_WEST, 2);
    Event_ShowMessageAndWait(ACTOR_ELDER_EAST_STAND_IN, 0, 40);
    Actor_SetAttachedEffect(ACTOR_ELDER_FAR_EAST,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ActorRunRepeatedMotion(ACTOR_ELDER_FAR_EAST, 2);
    Event_ShowMessage(ACTOR_ELDER_FAR_EAST, 0);
    // The westernmost elder stops straining, looks about and calls to the
    // boys, and the camera returns to Isaac.
    sShakeDone = 1;
    Engine_ActorSetAnimation(ACTOR_ELDER_FAR_WEST, ANIMATION_STAND);
    Engine_TaskWait(1);
    Actor_SetChildValue(ACTOR_ELDER_FAR_WEST, 0);
    Actor_ShowEmote(ACTOR_ELDER_FAR_WEST, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
    Actor_FaceDirection(ACTOR_ELDER_FAR_WEST, FACING_WEST, 40);
    Actor_FaceDirection(ACTOR_ELDER_FAR_WEST, FACING_EAST, 20);
    Actor_FaceDirection(ACTOR_ELDER_FAR_WEST, FACING_WEST, 20);
    Actor_FaceDirection(ACTOR_ELDER_FAR_WEST, FACING_SOUTH, 40);
    Actor_ShowEmote(ACTOR_ELDER_FAR_WEST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    0);
    Engine_ActorRunRepeatedMotion(ACTOR_ELDER_FAR_WEST, 2);
    Engine_ActorJump(ACTOR_ELDER_FAR_WEST, 4, 40);
    Engine_ActorSetAnimation(ACTOR_ELDER_FAR_WEST,
                             ANIMATION_YOUNG_ELDER_CALL_OUT);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_ELDER_FAR_WEST, 0, 20);
    Event_PlayCue(AUDIO_CUE_FADE_SLOT3);
    Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
    Camera_SetSpeed(PIXELS(6), PIXELS(3) / 4);
    Camera_MoveTo(PIXELS(84), -1, PIXELS(564), 1);
    Engine_CameraWaitForMove();
    FieldEffect_PlayQueuedSound();
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
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_GARET, ANIMATION_NOD);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_GARET, ANIMATION_WALK);
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

/// Sets the animation parameter of each entry of a one-sprite actor: mode - 1,
/// or for mode 0 a value from gValeStorm_FrameModes by the frame count. Only
/// ValeStorm_FlashBoulderSceneAnimationEntries calls it.
static void ValeStorm_SetActorFrameMode(struct FieldActor *actor, s32 mode) {
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
          entry->colorMode = animationParameter;
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
    ValeStorm_SetActorFrameMode(Object_GetById(ACTOR_ELDER_EAST), 1);
    ValeStorm_SetActorFrameMode(Object_GetById(ACTOR_ELDER_WEST), 1);
    ValeStorm_SetActorFrameMode(Object_GetById(ACTOR_ELDER_FAR_EAST), 1);
    ValeStorm_SetActorFrameMode(Object_GetById(ACTOR_ELDER_FAR_WEST), 1);
  } else {
    ValeStorm_SetActorFrameMode(Object_GetById(ACTOR_ELDER_EAST), 8);
    ValeStorm_SetActorFrameMode(Object_GetById(ACTOR_ELDER_WEST), 8);
    ValeStorm_SetActorFrameMode(Object_GetById(ACTOR_ELDER_FAR_EAST), 8);
    ValeStorm_SetActorFrameMode(Object_GetById(ACTOR_ELDER_FAR_WEST), 8);
  }
}

/// Jenna cries out for Felix as her family tries to save him at the river. In
/// Vale on the stormy night, her brother Felix has fallen in and clings on as
/// Kyle and her parents try to save him. "Oh no! My brother!" The rope will not
/// reach and no one has Psynergy left, so they send for help. Reached by
/// stepping on cell code 35 until FLAG_VALE_STORM_SAW_FELIX_HANGING_ON is set,
/// which this sets after ValeStorm_DoraGoesForHelp.
static void Event_WatchAttemptToSaveFelix(void) {
  const s32 *pulseScript;

  if (GameFlag_IsSet(FLAG_VALE_STORM_SAW_FELIX_HANGING_ON) != 0) {
    return;
  }
  Engine_EventStart();
  Actor_SetPosition(ACTOR_KYLE, PIXELS(192), PIXELS(1214));
  Actor_FaceDirection(ACTOR_KYLE, FACING_SOUTHEAST, 0);
  Engine_ActorSetAnimation(ACTOR_KYLE, ANIMATION_KYLE_REACH_OUT);
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
  Engine_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIMATION_PARENT_REACH_OUT);
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
  Engine_ActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIMATION_PARENT_REACH_OUT);
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
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
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
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIMATION_NOD);
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 10);
  Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
  Camera_MoveTo(PIXELS(232), -1, PIXELS(1253), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_AudioPlayCue(SE_HEAVY_IMPACT);
  Engine_ActorJump(ACTOR_FELIX, 4, 0);
  Engine_ActorSetAnimation(ACTOR_FELIX, ANIMATION_FELIX_UNDER_WATER);
  Engine_EventWait(10);
  Engine_ActorSetPosition(ACTOR_FELIX, 0, 0);
  Engine_EventWait(60);
  FieldEffect_PlayQueuedSound();
  Engine_ActorSetAnimation(ACTOR_KYLE, ANIMATION_STAND);
  {
    struct FieldActor *actor;
    actor = Object_GetById(ACTOR_KYLE);
    actor->scaleX = Q16_ONE;
    actor->scaleY = Q16_ONE;
  }
  Engine_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIMATION_STAND);
  {
    struct FieldActor *actor;
    actor = Object_GetById(ACTOR_JENNAS_FATHER);
    actor->scaleX = Q16_ONE;
    actor->scaleY = Q16_ONE;
  }
  Engine_ActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIMATION_STAND);
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
  Engine_ActorSetAnimation(ACTOR_FELIX, ANIMATION_FELIX_CLING);
  Engine_EventWait(20);
  FieldEffect_PlayQueuedSound();
  Engine_EventWait(20);
  Engine_CameraSetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Engine_CameraMoveTo(PIXELS(216), -1, PIXELS(1232), 1);
  Engine_CameraWaitForMove();
  Engine_ActorRunRepeatedMotion(ACTOR_JENNAS_FATHER, 2);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_JENNAS_FATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
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
  Engine_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIMATION_SHAKE_HEAD);
  Engine_EventShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNAS_FATHER, 0,
                                 10);
  Actor_FaceDirection(ACTOR_KYLE, FACING_NORTH, 20);
  Engine_ActorFaceDirection(ACTOR_KYLE, FACING_EAST, 10);
  Engine_ActorSetAnimation(ACTOR_KYLE, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KYLE, 0, 10);
  Actor_ShowEmote(ACTOR_JENNAS_FATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_KYLE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 20);
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
  Engine_ActorSetAnimation(ACTOR_DORA, ANIMATION_SHAKE_HEAD);
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_ShowEmote(ACTOR_KYLE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_JENNAS_FATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_JENNAS_MOTHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Engine_ActorFaceDirection(ACTOR_DORA, FACING_EAST, 10);
  Engine_ActorFaceEachOther(ACTOR_JENNAS_FATHER, ACTOR_JENNAS_MOTHER, 0);
  Engine_EventWait(20);
  Engine_ActorFaceDirection(ACTOR_DORA, FACING_EAST, 0);
  Engine_ActorFaceDirection(ACTOR_KYLE, FACING_EAST, 10);
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_WEST, 10);
  Engine_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_JENNAS_MOTHER, ANIMATION_NOD);
  Engine_ActorFaceEachOther(ACTOR_KYLE, ACTOR_DORA, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_KYLE, 1);
  Engine_EventShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KYLE, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIMATION_NOD);
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_NORTH + FACING_STEP, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_JENNAS_FATHER, 1);
  Engine_EventShowMessageAndWait(ACTOR_JENNAS_FATHER, 0, 10);
  Engine_ActorFaceDirection(ACTOR_KYLE, FACING_EAST, 0);
  Engine_ActorFaceDirection(ACTOR_DORA, FACING_EAST, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_JENNA, 1);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_JENNAS_MOTHER, ANIMATION_NOD);
  Engine_EventShowMessageAndWait(ACTOR_JENNAS_MOTHER, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_JENNA, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIMATION_NOD);
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_JENNA, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 10);
  ValeStorm_DoraGoesForHelp();
  Engine_GameFlagSet(FLAG_VALE_STORM_SAW_FELIX_HANGING_ON);
  Engine_EventEnd();
}

/// Dora and Jenna run through the house by the river; Dora goes north for help
/// and sends Jenna to the plaza, then asks Isaac and Garet to help: a yes
/// sends them after Jenna, a no to the plaza all the same. Called by
/// Event_WatchAttemptToSaveFelix.
static void ValeStorm_DoraGoesForHelp(void) {
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
  Engine_ActorSetAnimation(ACTOR_JENNA, ANIMATION_NOD);
  Engine_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIMATION_NOD);
  Engine_ActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIMATION_NOD);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_JENNAS_MOTHER, ANIMATION_NOD);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(134), -1, PIXELS(1195), 1);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetActionScript(ACTOR_JENNA, gValeStorm_JennaRunsIntoRiversideHouse);
  Actor_RunActionScriptAndWait(ACTOR_DORA,
                               gValeStorm_DoraRunsIntoRiversideHouse);
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_GrandparentsDoorOpens, 38, 72);
  Engine_EventWait(10);
  Actor_WalkToAndWait(ACTOR_DORA, 149, 1175);
  Actor_SetPosition(ACTOR_DORA, 0, 0);
  Actor_WalkToAndWait(ACTOR_JENNAS_MOTHER, 250, 1214);
  FieldEffect_PlayQueuedSound();
  Actor_FaceDirection(ACTOR_KYLE, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorSetAnimation(ACTOR_KYLE, ANIMATION_KYLE_REACH_OUT);
  Engine_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIMATION_PARENT_REACH_OUT);
  Engine_ActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIMATION_PARENT_REACH_OUT);
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
  Map_AnimateCells(gValeStorm_GrandparentsDoorCloses, 38, 72);
  Engine_EventWait(30);
  FieldEffect_PlayQueuedSound();
  Camera_MoveTo(PIXELS(112), -1, PIXELS(1225), 1);
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeStorm_IsaacsDoorOpens, 35, 73);
  Engine_EventWait(20);
  FieldEffect_PlayQueuedSound();
  Actor_SetActionScript(ACTOR_DORA, gValeStorm_DoraComesOutOfRiversideHouse);
  Engine_EventWait(20);
  Actor_SetActionScript(ACTOR_JENNA, gValeStorm_JennaComesOutOfRiversideHouse);
  Engine_EventWait(40);
  Event_PlayCue(SE_159);
  Map_AnimateCells(gValeStorm_IsaacsDoorCloses, 35, 73);
  Object_RefreshSelectorById(ACTOR_JENNA);
  FieldEffect_PlayQueuedSound();
  Engine_EventWait(40);
  northMessage = MsgValeGoLookNorth;
  Engine_EventSetMessage(northMessage);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_JENNA, ANIMATION_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 40);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_JENNA, ANIMATION_NOD);
  Engine_EventWait(30);
  Actor_SetActionScript(ACTOR_DORA, gValeStorm_DoraHeadsNorth);
  Actor_SetActionScript(ACTOR_JENNA, gValeStorm_JennaHeadsForPlaza);
  Engine_EventWait(40);
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(105), -1, PIXELS(1086), 1);
  Object_RefreshSelectorById(ACTOR_DORA);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 10);
  Actor_WalkToAndWait(ACTOR_DORA, 105, 1086);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIMATION_NOD);
    Engine_EventSetMessage((northMessage + 4));
  } else {
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventSetMessage((northMessage + 5));
  }
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  helpMessage = MsgValeDontSupposeTwo;
  Engine_EventSetMessage(helpMessage);
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIMATION_NOD);
    Engine_EventSetMessage((helpMessage + 1));
    Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0, 30);
    Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
    Engine_ActorSetAnimation(ACTOR_GARET, ANIMATION_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIMATION_NOD);
    Engine_EventWait(40);
  } else {
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 90);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Engine_ActorSetAnimation(ACTOR_DORA, ANIMATION_SHAKE_HEAD);
    Engine_EventSetMessage((helpMessage + 2));
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
  }
  Actor_SetActionScript(ACTOR_DORA, gValeStorm_DoraLeavesForHelp);
  Engine_EventWait(90);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_GARET, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_GARET, ANIMATION_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
}

/// Kyle tells Isaac and Garet to find help for Felix. In Vale on the stormy
/// night, at the river, he tells them they don't have enough Psynergy to save
/// Felix.
/// Reached by talking to Kyle; the first time he greets Isaac and sets
/// FLAG_VALE_STORM_KYLE_GREETED_ISAAC.
static void Event_TalkAboutSavingFelix(void) {
  s32 facing = FACING_SOUTHEAST;
  const s32 *pulseScript;

  Engine_EventStart();
  Engine_ActorSetAnimation(ACTOR_KYLE, ANIMATION_STAND);
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
  Engine_ActorSetAnimation(ACTOR_KYLE, ANIMATION_KYLE_REACH_OUT);
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

/// The Boulder sweeps Felix, his parents and Kyle away. In Vale on the stormy
/// night, Jenna comes back to the river with Isaac, Garet and the man whose
/// Psynergy is restored ("Mom! Dad! We're back!"), and as Dora calls everyone
/// down to save Felix the Boulder comes down the river and sweeps away Felix,
/// his parents and Kyle. Reached by stepping on cell code 36 once
/// FLAG_VALE_STORM_SENT_TO_AID_ELDERS is set (vale_gate.c), until
/// FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY is set, which this sets.
static void Event_BoulderSweepsThemAway(void) {
  struct FieldActor *leader;
  struct FieldActor *actor;
  s32 automaticPriority;
  struct AnimationObject *sprite;

  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) == 0) {
  } else {
    if (GameFlag_IsSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) != 0) {
    } else {
      Engine_EventStart();
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
      Actor_SetActionScript(ACTOR_DORA, gValeStorm_DoraRunsToRiver);
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
      Engine_ActorSetAnimationAndWait(ACTOR_EXHAUSTED_STRONGMAN, ANIMATION_NOD);
      Engine_EventWait(10);
      Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
      Engine_ActorFaceActor(ACTOR_GARET, ACTOR_DORA, 0);
      Engine_ActorFaceActor(ACTOR_JENNA, ACTOR_DORA, 0);
      Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIMATION_NOD);
      Engine_EventShowMessage(ACTOR_DORA, 0);
      Engine_ActorRunRepeatedMotion(ACTOR_JENNA, 2);
      Engine_EventShowMessageAndWait(ACTOR_JENNA, 0, 10);
      Actor_FaceDirection(ACTOR_DORA, FACING_NORTHEAST, 40);
      Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 20);
      Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIMATION_NOD);
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
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
      Engine_ActorSetAnimation(ACTOR_JENNA, ANIMATION_NOD);
      Engine_ActorSetAnimation(ACTOR_GARET, ANIMATION_NOD);
      automaticPriority = ACTOR_PRIORITY_AUTOMATIC;
      Engine_ActorSetAnimationAndWait(ACTOR_EXHAUSTED_STRONGMAN, ANIMATION_NOD);
      leader->priorityFlags |= automaticPriority;
      actor = Object_GetById(ACTOR_EXHAUSTED_STRONGMAN);
      actor->priorityFlags |= automaticPriority;
      ValeStorm_ShowBoulderSweepingDownriver();
      Engine_GameFlagSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY);
      Engine_EventEnd();
    }
  }
}

/// Places an actor and its targets at once, x doubling as its height.
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

/// One crash of the Boulder coming down: a shake, SE_145 and a wait.
static __inline__ void ValeStorm_PlayImpactBeat(s32 shakeAmplitude,
                                                s32 frames) {
  Map_SetShake(shakeAmplitude, shakeAmplitude, Q16_ONE);
  Event_PlayCue(SE_145);
  Event_Wait(frames);
}

/// The Boulder comes down the river and carries Felix, his parents and Kyle
/// away. The party gathers on the bank and hears it coming; the screen fades
/// to the river upstream, where the camera follows the Boulder down
/// (gValeStorm_BoulderRollsDownriver), and two frozen lightning flashes show
/// the family and Kyle in its path. Then the bank is redrawn without them,
/// with the rocks the Boulder left, and the party stands at the spot of
/// VALE_STORM_FROM_BOULDER_PASSED. Called by
/// Event_BoulderSweepsThemAway, whose recording on the docs site
/// (places/vale-storm/events/Event_BoulderSweepsThemAway.gif) shows the
/// descent.
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
  groupActor = Actor_Get(ACTOR_BOULDER_SHADOW);
  actorVisual = actor->sprite;
  groupVisual = groupActor->sprite;
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(110), -1, PIXELS(1419), 1);
  Actor_SetSpeed(ACTOR_EXHAUSTED_STRONGMAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  // Bring the group to the riverbank as the lightning subsides.
  entryActions = gValeStorm_ToRiverbankPath;
  Actor_SetActionScript(ACTOR_EXHAUSTED_STRONGMAN, entryActions);
  Event_Wait(10);
  Actor_SetActionScript(ACTOR_JENNA, entryActions);
  FieldEffect_LastLightning();
  Event_Wait(10);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, entryActions);
  Event_Wait(10);
  FieldEffect_StopLightning();
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
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_EXHAUSTED_STRONGMAN, EMOTE_IN_FRONT | EMOTE_QUESTION,
                  0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
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
  Event_PlayCue(AUDIO_CUE_FADE_BGM);
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
  FieldEffect_StopLightning();
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_Wait(1);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Camera_SetSpeed(PIXELS(8), PIXELS(8));
  Camera_MoveTo(PIXELS(217), -1, PIXELS(1084), 1);
  ColorBuffer_ApplyTarget(RGB(0, 0, 0), PALETTE_HALVES_BOTH);
  Engine_ColorBufferInterpolate(40);
  Task_Wait(40);
  // Stage the boulder and its shadow for the downriver camera shot.
  Actor_SetChildValue(ACTOR_BOULDER, 0);
  currentActor = Actor_Get(ACTOR_BOULDER);
  Actor_SetSpriteFlags(currentActor, 0);
  currentActor = Actor_Get(ACTOR_BOULDER_SHADOW);
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
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_WARM, PALETTE_HALF_BG);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, PALETTE_HALF_OBJ);
  Engine_ColorBufferInterpolate(30);
  Task_Wait(30);
  // Follow the boulder script until its eighth movement phase.
  FieldScheduler_AddCallback(ValeStorm_UpdateBoulderShadow,
                             TASK_PRIORITY_SCENE);
  Actor_SetActionScript(ACTOR_BOULDER, gValeStorm_BoulderRollsDownriver);
  Camera_SetSpeed(PIXELS(2), PIXELS(3) / 100);
  Camera_MoveTo(PIXELS(175), PIXELS(96), PIXELS(1086), 1);
  do {
    Task_Wait(1);
  } while ((s16)actor->work != 8);
  ColorBuffer_ApplyTarget(RGB(0, 0, 0), PALETTE_HALVES_BOTH);
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
  Scheduler_RemoveCallback(ValeStorm_UpdateBoulderShadow);
  Actor_Stop(ACTOR_BOULDER);
  Task_Wait(1);
  Actor_SetAnimation(ACTOR_BOULDER, ANIMATION_STILL);
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
  Actor_SetAnimation(ACTOR_FELIX, ANIMATION_FELIX_CLING_STILL);
  Actor_SetPosition(ACTOR_DORA, PIXELS(169), PIXELS(1264));
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 0);
  Actor_SetAnimation(ACTOR_DORA, ANIMATION_DORA_RUN);
  Actor_SetPosition(ACTOR_JENNA, PIXELS(151), PIXELS(1292));
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_SetAnimation(ACTOR_JENNA, ANIMATION_JENNA_RUN);
  Actor_SetPosition(ACTOR_EXHAUSTED_STRONGMAN, PIXELS(170), PIXELS(1313));
  Actor_FaceDirection(ACTOR_EXHAUSTED_STRONGMAN, FACING_SOUTHWEST, 0);
  Actor_SetAnimation(ACTOR_EXHAUSTED_STRONGMAN, ANIMATION_STRONGMAN_RUN);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(185), PIXELS(1333));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIMATION_ISAAC_RECOIL);
  Actor_SetPosition(ACTOR_GARET, PIXELS(169), PIXELS(1384));
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_SetAnimation(ACTOR_GARET, ANIMATION_STILL);
  Camera_MoveTo(PIXELS(166), 0, PIXELS(1337), 0);
  Engine_MapRedraw();
  actor->motionFlags = resetValue;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  ValeStorm_FlashLightning();
  Actor_SetPosition(ACTOR_BOULDER_SHADOW, PIXELS(218), PIXELS(1176));
  Camera_MoveTo(PIXELS(210), 0, PIXELS(1196), 0);
  Engine_MapRedraw();
  groupActor->scaleX = 2 * Q16_ONE;
  groupActor->scaleY = 2 * Q16_ONE;
  FieldScheduler_AddCallback(ValeStorm_FlickerBoulderShadow,
                             TASK_PRIORITY_SCENE);
  Actor_Stop(ACTOR_KYLE);
  Actor_Stop(ACTOR_JENNAS_FATHER);
  Actor_Stop(ACTOR_JENNAS_MOTHER);
  Task_Wait(1);
  groupActor = Actor_Get(ACTOR_KYLE);
  groupVisual = groupActor->sprite;
  groupActor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  groupActor->scaleX = Q16_ONE;
  groupActor->scaleY = Q16_ONE;
  groupActor->facing = FACING_NORTH + FACING_STEP;
  groupVisual->image.priority = 0;
  Actor_SetAnimation(ACTOR_KYLE, ANIMATION_STILL);
  groupActor = Actor_Get(ACTOR_JENNAS_FATHER);
  groupVisual = groupActor->sprite;
  groupActor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  groupActor->scaleX = Q16_ONE;
  groupActor->scaleY = Q16_ONE;
  groupActor->facing = FACING_NORTHWEST + FACING_STEP;
  groupVisual->image.priority = 0;
  Actor_SetAnimation(ACTOR_JENNAS_FATHER, ANIMATION_PARENT_RECOIL);
  groupActor = Actor_Get(ACTOR_JENNAS_MOTHER);
  groupVisual = groupActor->sprite;
  groupActor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  groupActor->scaleX = Q16_ONE;
  groupActor->scaleY = Q16_ONE;
  groupActor->facing = FACING_NORTHWEST + FACING_STEP;
  groupVisual->image.priority = 0;
  Actor_SetAnimation(ACTOR_JENNAS_MOTHER, ANIMATION_PARENT_RECOIL);
  groupActor = Actor_Get(ACTOR_BOULDER_SHADOW);
  groupVisual = groupActor->sprite;
  ValeStorm_FlashLightning();
  actor->y.fixed = PIXELS(48);
  actor->x.fixed = PIXELS(214);
  actor->z.fixed = PIXELS(1216);
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  groupVisual->image.priority = 1;
  Actor_SetPosition(ACTOR_BOULDER_SHADOW, PIXELS(214), PIXELS(1216));
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_NORTH, 20);
  GameFlag_Set(FLAG_WEATHER_HELD);
  Map_SetLayerEntryFlag(0);
  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);
  Map_SetLayerEntryFlag(3);
  Map_SetLayerEntryFlag(4);
  Map_SetLayerEntryFlag(5);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_WARM, PALETTE_HALF_BG);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, PALETTE_HALF_OBJ);
  Engine_ColorBufferInterpolate(120);
  Task_Wait(160);
  ColorBuffer_ApplyTarget(RGB(31, 31, 31), PALETTE_HALF_BG);
  ColorBuffer_ApplyTarget(RGB(31, 31, 31), PALETTE_HALF_OBJ);
  Engine_ColorBufferInterpolate(80);
  Event_Wait(80);
  Event_Wait(100);
  Scheduler_RemoveCallback(ValeStorm_FlickerBoulderShadow);
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
  Actor_SetAnimation(ACTOR_DORA, ANIMATION_STAND);
  actor = Actor_Get(ACTOR_DORA);
  actor->facing = FACING_NORTHEAST;
  random = Random_Next();
  actor->timer = Math_RemainderUnsigned(random, 90) + 60;
  departureActions = gValeStorm_SquashAndStretchScript;
  actor->work = 1;
  Actor_SetActionScript(ACTOR_DORA, departureActions);
  Actor_SetPosition(ACTOR_JENNA, PIXELS(165), PIXELS(1254));
  Actor_SetAnimation(ACTOR_JENNA, ANIMATION_STAND);
  actor = Actor_Get(ACTOR_JENNA);
  actor->facing = FACING_NORTHEAST;
  random = Random_Next();
  actor->timer = Math_RemainderUnsigned(random, 90) + 60;
  actor->work = 2;
  Actor_SetActionScript(ACTOR_JENNA, departureActions);
  Actor_SetPosition(ACTOR_GARET, PIXELS(152), PIXELS(1285));
  Actor_SetAnimation(ACTOR_GARET, ANIMATION_STAND);
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
  Actor_SetAnimation(ACTOR_EXHAUSTED_STRONGMAN, ANIMATION_STRONGMAN_SPRAWLED);
  Object_GetById(ACTOR_GARET)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Object_GetById(ACTOR_EXHAUSTED_STRONGMAN)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  FieldScheduler_AddCallback(ValeStorm_CopyLeaderSpritePriority,
                             TASK_PRIORITY_SCENE);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(181), PIXELS(1273));
  currentActor = Actor_Get(ACTOR_PARTY_LEADER);
  currentActor->facing = FACING_NORTHEAST;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIMATION_STAND);
  Camera_MoveTo(PIXELS(181), 0, PIXELS(1273), 0);
  Engine_MapRedraw();
  Actor_SetPosition(ACTOR_KYLE, 0, 0);
  Actor_SetPosition(ACTOR_BOULDER, 0, 0);
  Actor_SetPosition(ACTOR_JENNAS_FATHER, 0, 0);
  Actor_SetPosition(ACTOR_JENNAS_MOTHER, 0, 0);
  Actor_SetPosition(ACTOR_FELIX, 0, 0);
  Actor_SetPosition(ACTOR_BOULDER_SHADOW, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_ROCK + 6, PIXELS(144), PIXELS(1070));
  Actor_SetPosition(ACTOR_FIRST_ROCK + 7, PIXELS(276), PIXELS(1270));
  Task_Wait(60);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_WARM, PALETTE_HALF_BG);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, PALETTE_HALF_OBJ);
  Engine_ColorBufferInterpolate(80);
  Event_Wait(60);
  FieldEffect_PlayQueuedSound();
  Event_Wait(60);
  Party_RemoveOwnerRestored(1);
  FieldEffect_ResumeLightning();
}

/// A lightning flash that freezes the picture: holds the rain
/// (FLAG_WEATHER_HELD) and pauses the map's tile animation channels 0 to 5,
/// shows the scene in PALETTE_TRANSFORM_WARM for two seconds, fades it to
/// black, then lets the rain and the animations run again.
static void ValeStorm_FlashLightning(void) {
  Engine_TaskWait(20);
  GameFlag_Set(FLAG_WEATHER_HELD);
  Map_SetLayerEntryFlag(0);
  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);
  Map_SetLayerEntryFlag(3);
  Map_SetLayerEntryFlag(4);
  Map_SetLayerEntryFlag(5);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_WARM, PALETTE_HALF_BG);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, PALETTE_HALF_OBJ);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(120);
  ColorBuffer_ApplyTarget(RGB(0, 0, 0), PALETTE_HALVES_BOTH);
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

/// The callback that keeps ACTOR_BOULDER_SHADOW on the Boulder as it comes down
/// the river: the shadow swells or shrinks by the step of
/// gValeStorm_BoulderRollsDownriver the Boulder is on (its work value), shows
/// on odd frames only, and the map shakes as the step's timer counts down.
static void ValeStorm_UpdateBoulderShadow(void) {
  struct FieldActor *source;
  struct FieldActor *shadow;
  struct AnimationObject *sprite;
  s16 *timer;

  source = Object_GetById(ACTOR_BOULDER);
  shadow = Object_GetById(ACTOR_BOULDER_SHADOW);
  sprite = shadow->sprite;
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
  shadow->x.fixed = source->x.fixed;
  shadow->z.fixed = source->z.fixed;
  shadow->targetX = shadow->x.fixed;
  sprite->offsetY = 10;
  if (gFrameCount & 1) {
    switch ((s16)source->work) {
    case 1:
    case 5:
      shadow->scaleX += 0xa3d;
      shadow->scaleY += 0xa3d;
      break;
    case 4:
      shadow->scaleX += 0x51e;
      shadow->scaleY += 0x51e;
      break;
    case 2:
    case 3:
    case 6:
    case 7:
    case 8:
    case 9:
      shadow->scaleX += -0x7ae;
      shadow->scaleY += -0x7ae;
      break;
    }
    sprite->scale = shadow->scaleX;
  } else {
    sprite->scale = 0;
  }
}

/// The callback that moves ACTOR_BOULDER_SHADOW between two heights on
/// alternate frames, during the frozen flashes.
static void ValeStorm_FlickerBoulderShadow(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_BOULDER_SHADOW);
  u32 frame = gFrameCount;
  struct AnimationObject *sprite = actor->sprite;

  if (frame & 1) {
    sprite->offsetY = 2;
  } else {
    sprite->offsetY = 64;
  }
}

/// An exhausted strongman tries to go for help and can't move. In Vale on the
/// stormy night, after the Boulder, the strongman who came to help tries to go
/// for help himself. "I can't move..." Reached by talking to him
/// (ACTOR_EXHAUSTED_STRONGMAN); the first time sets
/// FLAG_VALE_STORM_STRONGMAN_TRIED_TO_GO.
static void Event_StrongmanTriesToGoForHelp(void) {
  struct FieldActor *villager;
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  villager = Object_GetById(ACTOR_EXHAUSTED_STRONGMAN);
  Engine_EventStart();
  if (GameFlag_IsSet(FLAG_VALE_STORM_STRONGMAN_TRIED_TO_GO) != 0) {
    Engine_ActorStop(ACTOR_EXHAUSTED_STRONGMAN);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_EXHAUSTED_STRONGMAN, 2);
    Engine_EventWait(40);
    if ((s16)leader->facing >= 0) {
      Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                               ANIMATION_STRONGMAN_PROPPED_UP);
    } else {
      Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                               ANIMATION_STRONGMAN_PROPPED_UP_OTHER_WAY);
    }
    Engine_ActorRunRepeatedMotion(ACTOR_EXHAUSTED_STRONGMAN, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgValeICantMoveGetHelp);
    Event_ShowMessage(ACTOR_EXHAUSTED_STRONGMAN, 0);
    Actor_SetActionScript(ACTOR_EXHAUSTED_STRONGMAN,
                          gValeStorm_SquashAndStretchScript);
    Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                             ANIMATION_STRONGMAN_SPRAWLED);
  } else {
    Engine_ActorStop(ACTOR_EXHAUSTED_STRONGMAN);
    villager->scaleX = Q16_ONE;
    villager->scaleY = Q16_ONE;
    Actor_FaceDirection(ACTOR_EXHAUSTED_STRONGMAN, FACING_EAST + FACING_STEP,
                        0);
    if ((s16)leader->facing >= 0) {
      Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                               ANIMATION_STRONGMAN_PROPPED_UP);
    } else {
      Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                               ANIMATION_STRONGMAN_PROPPED_UP_OTHER_WAY);
    }
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgValeIllGo);
    Event_ShowMessageAndWait(ACTOR_EXHAUSTED_STRONGMAN, 0, 20);
    Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, ANIMATION_STAND);
    Engine_ActorJump(ACTOR_EXHAUSTED_STRONGMAN, 4, 0);
    Engine_EventWait(80);
    Engine_ActorRunRepeatedMotion(ACTOR_EXHAUSTED_STRONGMAN, 2);
    Engine_EventWait(40);
    if ((s16)leader->facing >= 0) {
      Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                               ANIMATION_STRONGMAN_PROPPED_UP);
    } else {
      Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                               ANIMATION_STRONGMAN_PROPPED_UP_OTHER_WAY);
    }
    Engine_EventWait(2);
    Engine_ActorJump(ACTOR_EXHAUSTED_STRONGMAN, 2, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_EXHAUSTED_STRONGMAN, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_EXHAUSTED_STRONGMAN, 0);
    Actor_SetActionScript(ACTOR_EXHAUSTED_STRONGMAN,
                          gValeStorm_SquashAndStretchScript);
    Engine_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                             ANIMATION_STRONGMAN_SPRAWLED);
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
  Engine_EventStart();
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
  Engine_ActorSetAnimationAndWait(ACTOR_GARET, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_GARET, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_GARET, ANIMATION_NOD);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(ACTOR_GARET, 165, 1300);
  Actor_WalkToAndWait(ACTOR_GARET, 195, 1432);
  GameFlag_Set(FLAG_VALE_STORM_GARET_LEFT);
}

/// Redraws the riverbank as the Boulder left it, pictures and collision.
/// Called by ValeStorm_ShowBoulderSweepingDownriver and, on later arrivals, by
/// ValeStorm_Initialize.
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

  Engine_EventStart();
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

  Engine_EventStart();
  state = gEventWork;
  state->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  state->transitionFrames = 64;
  Engine_GameFlagSet(FLAG_VALE_STORM_FOURTH_MONSTER_FOUGHT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 3);
  Engine_GameFlagSet(FLAG_VALE_STORM_MONSTER_FOUGHT);
  Engine_EventEnd();
}

/// An elder calling up Psynergy, for 40 frames once the camera reaches the
/// Boulder: the sprite shows palette 7 for two frames in four, and a pair of
/// Psynergy sparks rises and falls every 16 frames plus 8 for each step of
/// sShakeShift.
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

/// An elder holding the Boulder: the sprite cycles through palettes 0 to 5,
/// and sparks rise and fall as in ValeStorm_FlashPaletteAndSpawnEffects.
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

/// Cycles the sprite through palettes 0 to 5, a step every second frame.
static void ValeStorm_CycleObjectPalettes(struct FieldActor *actor) {
  s32 paletteFrame;

  if ((gFrameCount & 1) != 0) {
    paletteFrame = (gFrameCount >> 1);
    Engine_ObjectSetPartPalettes(actor,
                                 Math_RemainderUnsigned(paletteFrame, 6));
  }
}

#include "common/effect_vertical.h"

/// Spawns a pair of Psynergy sparks on an elder, one rising and one falling
/// (common/effect_vertical.h), with SE_JUMP.
static void ValeStorm_SpawnEffectPair(struct FieldActor *parent) {
  struct FieldActor *pair[2];
  struct FieldActor *child;
  struct AnimationObject *sprite;
  struct FieldEffectScene *work = gEffectWork;
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

/// The callback that cycles the Boulder's palettes while the elders hold it.
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
  Engine_EventStart();
  Engine_MessageShowCentered(MsgValeDoorWontOpen, 1);
  Engine_EventEnd();
}

/// In Vale on the stormy night, searching the chest Garet was trying to save.
/// "It's Garet's chest of valuables."
static void Event_SearchValuablesChest(void) {
  Engine_EventStart();
  Engine_MessageShowCentered(MsgValeSChestValuables, 1);
  Engine_EventEnd();
}

/// ValeStorm_SetActorFrameMode's parameter for mode 0, by the frame count.
u8 gValeStorm_FrameModes[] = {7, 0, 0, 0};

// The actors' object scripts, positions in pixels.

/// The stout man runs off toward the plaza after shoving Isaac.
s32 gValeStorm_ShovingManScript[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(185), 0, PIXELS(824)),
    SC_SYNC,         SC_MOVE(PIXELS(179), 0, PIXELS(836)),
    SC_SYNC,         SC_MOVE(PIXELS(226), 0, PIXELS(840)),
    SC_SYNC,         SC_MOVE(PIXELS(226), 0, PIXELS(944)),
    SC_SYNC,         SC_POSITION(0, 0, 0),
    SC_ANIMATION(1), SC_END,
};

/// Garet runs up to Isaac (Event_UnusedGaretJoinsIsaac).
s32 gValeStorm_UnusedGaretRunsUpScript[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(345), 0, PIXELS(612)),
    SC_SYNC,         SC_MOVE(PIXELS(311), 0, PIXELS(638)),
    SC_SYNC,         SC_MOVE(PIXELS(256), 0, PIXELS(608)),
    SC_SYNC,         SC_ANIMATION(1),
    SC_END,
};

/// Garet heaves for breath: his sprite stretches over 24 frames and settles
/// over 12, for ever.
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

/// The Boulder on the mountainside, in animation 2 until it slips.
s32 gValeStorm_BoulderWaits[] = {
    SC_ANIMATION(2),
    SC_END,
};

/// Isaac and Garet run to where they see the elders hold the Boulder
/// (Event_WatchBoulderFall), and face each other.
s32 gValeStorm_BoulderFallLeaderPath[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(94), 0, PIXELS(564)), SC_SYNC,
    SC_ANIMATION(1), SC_SET(SW_FACING, FACING_WEST),      SC_WAIT(30),
    SC_END,
};

s32 gValeStorm_BoulderFallGaretPath[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(76), 0, PIXELS(564)), SC_SYNC,
    SC_ANIMATION(1), SC_SET(SW_FACING, FACING_EAST),      SC_WAIT(30),
    SC_END,
};

// ValeStorm_DoraGoesForHelp: Jenna and Dora run into the house by the river by
// its east door and come out by its west door; Jenna then heads for the plaza
// and Dora north.

s32 gValeStorm_DoraRunsIntoRiversideHouse[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(178), 0, PIXELS(1190)),
    SC_SYNC,         SC_MOVE(PIXELS(149), 0, PIXELS(1194)),
    SC_SYNC,         SC_ANIMATION(1),
    SC_END,
};

s32 gValeStorm_JennaRunsIntoRiversideHouse[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(198), 0, PIXELS(1176)),
    SC_SYNC,         SC_MOVE(PIXELS(170), 0, PIXELS(1195)),
    SC_SYNC,         SC_MOVE(PIXELS(149), 0, PIXELS(1194)),
    SC_SYNC,         SC_MOVE(PIXELS(149), 0, PIXELS(1175)),
    SC_SYNC,         SC_POSITION(0, 0, 0),
    SC_ANIMATION(1), SC_END,
};

s32 gValeStorm_DoraComesOutOfRiversideHouse[] = {
    SC_ANIMATION(2),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(30),
    SC_POSITION(PIXELS(102), 0, PIXELS(1189)),
    SC_WAIT(1),
    SC_MOVE(PIXELS(102), 0, PIXELS(1218)),
    SC_SYNC,
    SC_MOVE(PIXELS(84), 0, PIXELS(1228)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_END,
};

s32 gValeStorm_JennaComesOutOfRiversideHouse[] = {
    SC_ANIMATION(2),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(30),
    SC_POSITION(PIXELS(102), 0, PIXELS(1189)),
    SC_SYNC,
    SC_MOVE(PIXELS(102), 0, PIXELS(1218)),
    SC_SYNC,
    SC_MOVE(PIXELS(120), 0, PIXELS(1228)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_END,
};

s32 gValeStorm_DoraHeadsNorth[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(50), 0, PIXELS(1214)),
    SC_SYNC,         SC_MOVE(PIXELS(46), 0, PIXELS(1130)),
    SC_SYNC,         SC_MOVE(PIXELS(67), 0, PIXELS(1100)),
    SC_SYNC,         SC_ANIMATION(1),
    SC_END,
};

s32 gValeStorm_JennaHeadsForPlaza[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(153), 0, PIXELS(1251)),
    SC_SYNC,         SC_MOVE(PIXELS(177), 0, PIXELS(1357)),
    SC_SYNC,         SC_POSITION(0, 0, 0),
    SC_ANIMATION(1), SC_END,
};

/// Dora leaves to the west once she has asked the boys to help.
s32 gValeStorm_DoraLeavesForHelp[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(123), 0, PIXELS(1078)),
    SC_SYNC,         SC_MOVE(PIXELS(148), 0, PIXELS(1078)),
    SC_SYNC,         SC_MOVE(PIXELS(148), 0, PIXELS(1064)),
    SC_SYNC,         SC_MOVE(PIXELS(35), 0, PIXELS(1064)),
    SC_SYNC,         SC_POSITION(0, 0, 0),
    SC_ANIMATION(1), SC_END,
};

/// Dora runs down to the river when Jenna comes back
/// (Event_BoulderSweepsThemAway).
s32 gValeStorm_DoraRunsToRiver[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(102), 0, PIXELS(1236)),
    SC_SYNC,         SC_MOVE(PIXELS(148), 0, PIXELS(1256)),
    SC_SYNC,         SC_MOVE(PIXELS(177), 0, PIXELS(1303)),
    SC_SYNC,         SC_ANIMATION(1),
    SC_END,
};

/// The way down to the riverbank that each of the party and the strongman
/// takes in turn as the Boulder comes, then each one's place there.
s32 gValeStorm_ToRiverbankPath[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(137), 0, PIXELS(1379)),
    SC_SYNC,         SC_MOVE(PIXELS(114), 0, PIXELS(1416)),
    SC_SYNC,         SC_MOVE(PIXELS(95), 0, PIXELS(1416)),
    SC_SYNC,         SC_MOVE(PIXELS(83), 0, PIXELS(1427)),
    SC_SYNC,         SC_MOVE(PIXELS(154), 0, PIXELS(1430)),
    SC_SYNC,         SC_MOVE(PIXELS(179), 0, PIXELS(1398)),
    SC_SYNC,         SC_MOVE(PIXELS(179), 0, PIXELS(1361)),
    SC_SYNC,         SC_END,
};

// Where each one stops on the bank to hear the Boulder coming.

s32 gValeStorm_ExhaustedStrongmanGatherPath[] = {
    SC_MOVE(PIXELS(184), 0, PIXELS(1309)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_END,
};

s32 gValeStorm_JennaGatherPath[] = {
    SC_MOVE(PIXELS(165), 0, PIXELS(1306)), SC_SYNC, SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTHWEST),   SC_END,
};

s32 gValeStorm_LeaderGatherPath[] = {
    SC_MOVE(PIXELS(183), 0, PIXELS(1325)), SC_SYNC, SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTHWEST),   SC_END,
};

s32 gValeStorm_GaretGatherPath[] = {
    SC_MOVE(PIXELS(166), 0, PIXELS(1325)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_END,
};

/// Everyone on the bank looks about (ValeStorm_RerollHeadingOnCountdown).
s32 gValeStorm_LookAroundScript[] = {
    SC_CALL(ValeStorm_RerollHeadingOnCountdown),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// A squash and stretch every 60 to 149 frames (Scene_SquashAndStretch), for
/// the people straining at the river and, after the Boulder, those on the
/// bank.
s32 gValeStorm_SquashAndStretchScript[] = {
    SC_CALL(Scene_SquashAndStretch),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The Boulder bounds down the river in nine steps. Each sets its step in
/// SW_WORK, which ValeStorm_UpdateBoulderShadow reads and
/// ValeStorm_ShowBoulderSweepingDownriver waits on, and two of its landings
/// start a shake countdown in SW_TIMER.
s32 gValeStorm_BoulderRollsDownriver[] = {
    SC_ANIMATION(2),
    SC_SET(SW_SPEED, PIXELS(3)),
    SC_SET(SW_ACCELERATION, PIXELS(1)),
    SC_SET(SW_TIMER, 0),
    SC_SET(SW_WORK, 1),
    SC_SET(SW_SCALE_X, 2 * Q16_ONE),
    SC_SET(SW_SCALE_Y, 2 * Q16_ONE),
    SC_MOVE(PIXELS(256), PIXELS(128), PIXELS(931)),
    SC_SYNC,
    SC_SE(SE_145),
    SC_SET(SW_TIMER, 60),
    SC_SET(SW_WORK, 2),
    SC_MOVE(PIXELS(228), PIXELS(149), PIXELS(957)),
    SC_SYNC,
    SC_SET(SW_WORK, 3),
    SC_MOVE(PIXELS(201), PIXELS(160), PIXELS(984)),
    SC_SYNC,
    SC_SET(SW_WORK, 4),
    SC_MOVE(PIXELS(173), PIXELS(138), PIXELS(1011)),
    SC_SYNC,
    SC_SET(SW_WORK, 5),
    SC_MOVE(PIXELS(146), PIXELS(96), PIXELS(1038)),
    SC_SYNC,
    SC_SE(SE_145),
    SC_SET(SW_SPEED, PIXELS(3) / 2),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 2),
    SC_SET(SW_TIMER, 60),
    SC_SET(SW_WORK, 6),
    SC_MOVE(PIXELS(161), PIXELS(107), PIXELS(1050)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCELERATION, PIXELS(3) / 5),
    SC_SET(SW_WORK, 7),
    SC_MOVE(PIXELS(177), PIXELS(96), PIXELS(1062)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(5) / 2),
    SC_SET(SW_ACCELERATION, PIXELS(4) / 5),
    SC_SET(SW_WORK, 8),
    SC_MOVE(PIXELS(201), PIXELS(74), PIXELS(1074)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(3)),
    SC_SET(SW_ACCELERATION, PIXELS(1)),
    SC_SET(SW_WORK, 9),
    SC_MOVE(PIXELS(225), -PIXELS(64), PIXELS(1086)),
    SC_SYNC,
    SC_SET(SW_TIMER, 0),
    SC_SET(SW_WORK, 0),
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
    { SPRITE_ROCK, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_ROCK, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_ROCK, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_ROCK, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_ROCK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(437), 0, PIXELS(762), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_ROCK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(322), 0, PIXELS(1433), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_ROCK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(144), 0, PIXELS(1070), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_ROCK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(276), 0, PIXELS(1270), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_ROCK, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_CHEST_OF_VALUABLES, CONDITION_ALWAYS, ACTOR_STAND,
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
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(85), 0, PIXELS(361), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
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
     EVENT_SCRIPT(Event_UnusedLeaveToValeInteriorsShopsWestBySecondDoor)},
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

// The doors' cell animations: two pictures of a door opening, from the map's
// cells below the village, each shown for two frames. The four kinds of door
// are the day map's (vale.c), and each table takes the name of a door it
// opens.

/// The Sanctum's door (ValeStorm_UnusedOpenSanctumDoor).
u16 gValeStorm_SanctumDoorOpens[] = {
    MAP_CELL_STEP(0, 98, 2, 2, 2),
    MAP_CELL_STEP(2, 98, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

/// The door of Isaac's house, and of Garet's and the house by the river.
u16 gValeStorm_IsaacsDoorOpens[] = {
    MAP_CELL_STEP(0, 96, 2, 2, 2),
    MAP_CELL_STEP(2, 96, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

/// The door of the house of Jenna's grandparents, and two more of the kind.
u16 gValeStorm_GrandparentsDoorOpens[] = {
    MAP_CELL_STEP(4, 98, 2, 2, 2),
    MAP_CELL_STEP(6, 98, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

/// The door VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST goes through.
u16 gValeStorm_WestHouseDoorOpens[] = {
    MAP_CELL_STEP(4, 96, 2, 2, 2),
    MAP_CELL_STEP(6, 96, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

/// Shuts a door of gValeStorm_IsaacsDoorOpens's kind, with the shut picture
/// of Isaac's door, after Dora and Jenna come out of the house by the river.
u16 gValeStorm_IsaacsDoorCloses[] = {
    MAP_CELL_STEP(0, 96, 2, 2, 2),
    MAP_CELL_STEP(50, 44, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

/// Shuts a door of gValeStorm_GrandparentsDoorOpens's kind behind Dora and
/// Jenna as they go into the house by the river.
u16 gValeStorm_GrandparentsDoorCloses[] = {
    MAP_CELL_STEP(4, 98, 2, 2, 2),
    MAP_CELL_STEP(44, 108, 2, 2, 2),
    MAP_CELL_STEPS_END,
};
