/// @file
/// Vale on the stormy night (SceneId_ValePrologue), the prologue, when Isaac
/// and Garet are children: a scene of its own that stands in for SceneId_Vale
/// while FLAG_VALE_STORM is set, so that the doors that lead to Vale by day
/// lead here. ValePrologue_Initialize sets the flag on every arrival, and
/// vale_plaza.c clears it when the night ends.
///
/// The night is a chain of story flags. Each is set by one event and read by
/// ValePrologue_Initialize, which rebuilds the scene at that stage on the
/// next arrival:
///
/// - FLAG_VALE_STORM_SENT_TO_PLAZA: ValePrologue_DoraSendsIsaacToThePlaza,
/// played on
///   the first arrival by VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WITH_DORA.
///   Dora and Kyle send Isaac on to the plaza and go to help; until the flag is
///   set the lightning waits for the event.
/// - FLAG_VALE_STORM_FIRST_ROCK_FELL to FLAG_VALE_STORM_FOURTH_ROCK_FELL, with
///   the area flags FLAG_VALE_STORM_FIRST_ROCK_PASSED to
///   FLAG_VALE_STORM_FOURTH_ROCK_PASSED: ValePrologue_FirstRockFalls to
///   ValePrologue_FourthRockFalls drop a rock across a path at four spots.
///   The _FELL flag puts back the rock and the cell behaviors of the cells
///   it blocks; the _PASSED flag, cleared once the party leaves Vale, keeps the
///   event from running again.
/// - FLAG_VALE_STORM_BUMPED_INTO_VILLAGER: ValePrologue_BumpIntoShovingMan,
/// the
///   first time Isaac tries the door of
///   VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_SOUTH.
/// - FLAG_VALE_STORM_HEARD_GARET, then FLAG_VALE_STORM_GARET_JOINED: Garet is
///   trying to save his chest of valuables; talking to him, or walking off
///   past him, brings him into the party (ValePrologue_GaretLeavesHisChest)
///   and leaves the chest by his door. ValePrologue_UnusedGaretJoinsIsaac is
///   a third way that never plays. With Garet along, a fallen rock and the path
///   north to Mt. Aleph have him turn Isaac back.
/// - FLAG_VALE_BOULDER_FELL: ValePrologue_WatchBoulderFall, the elders
/// holding back
///   the Boulder with their Psynergy as it slips; from then on it rests where
///   it slipped to.
/// - FLAG_VALE_STORM_SAW_FELIX_HANGING_ON:
/// ValePrologue_WatchAttemptToSaveFelix and
///   ValePrologue_DoraGoesForHelp, Felix clinging on in the river. The plaza
///   (vale_plaza.c) then sets FLAG_VALE_STORM_SENT_TO_AID_ELDERS and sends the
///   boys back.
/// - FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY:
/// ValePrologue_BoulderSweepsThemAway and
///   ValePrologue_ShowBoulderSweepingDownriver, the Boulder carrying Felix,
///   his parents and Kyle away down the river.
/// - FLAG_VALE_STORM_GARET_LEFT: ValePrologue_GaretFollowsIsaac, as Isaac
/// leaves
///   for the plaza again; the night ends in vale_plaza.c.
///
/// Every way out goes through Vale_RequestBackdropExit (shared/vale.h), which
/// stops the lightning first, and each door plays its opening cells before it;
/// gValePrologue_Exits gives each exit's destination. Two cells start a
/// battle once each (slots 2 and 3 of ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT).

#include "script_operands.h"
#include "main_routine_work.h"
#include "module/import.h"
#include "module/scene/vale_prologue.h"
#include "module/scene/shared/effect_psynergy.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
#include "module/field/field.h"
#include "module/runtime/runtime.h"
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
#include "task.h"
#include "event.h"
#include "scene_tables.h"
#include "call.h"
#include "event_inline.h"
#include "actor.h"
#include "animspr.h"
#include "text/msg_ids.h"
#include "resource.h"
#include "field_effect.h"
#include "tbs_edition.h"
#include "actor_script.h"
#include "heap_slots.h"
#include "vram_cache.h"
#include "fixed_math.h"
#include "flags.h"
#include "sounds.h"
#include "sound_driver_symbols.h"
#include "io_reg.h"
#include "fx_work.h"

static void ValePrologue_DoraSendsIsaacToThePlaza(void);
static void ValePrologue_ApplyFirstRockCollision(void);
static void ValePrologue_ApplySecondRockCollision(void);
static void ValePrologue_ApplyThirdRockCollision(void);
static void ValePrologue_ApplyFourthRockCollision(void);
static void ValePrologue_BumpIntoShovingMan(void);
static void ValePrologue_GaretLeavesHisChest(void);
static void ValePrologue_GaretCommentsOnBlockedPath(s32 x, s32 y);
static void ValePrologue_DoraGoesForHelp(void);
static void ValePrologue_ShowBoulderSweepingDownriver(void);
static void ValePrologue_FlashLightning(void);
static void ValePrologue_UpdateBoulderShadow(void);
static void ValePrologue_FlickerBoulderShadow(void);
static void ValePrologue_GaretFollowsIsaac(void);
static void ValePrologue_RebuildRiverbank(void);
static void ValePrologue_FlashPaletteAndSpawnEffects(struct Actor *actor);
static void ValePrologue_SpawnEffectPair(struct Actor *parent);
static void ValePrologue_UpdateBoulderMapEffects(void);
static void ValePrologue_CycleBoulderPalette(void);
static void ValePrologue_CopyLeaderSpritePriority(void);

/// The placed actors, numbered in the order of
/// gValePrologue_ActorPlacements; the elders holding back the Boulder
/// (gValePrologue_BoulderWatchers) follow them.
enum StormNightActorId {
  /// The strongman who comes back to the river with Jenna, his Psynergy
  /// restored at the Psynergy Stone (vale_plaza.c); after the Boulder has
  /// passed he tries to go for help and cannot. "I... I'll go!", then "It's no
  /// use. I can't move..."
  /// ★ Named from the game: "I'll go!" (MsgValeIllGo)
  ACTOR_EXHAUSTED_STRONGMAN = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "Dora, hurry!" (MsgValeDoraHurryBoulder)
  ACTOR_DORA,
  /// ★ Named from the game: "Let me help you, Kyle!" (MsgValeKyleAbleStop)
  ACTOR_KYLE,
  /// Eight huge rocks that Isaac "can't even budge": ACTOR_FIRST_ROCK to
  /// ACTOR_FIRST_ROCK + 3 fall across the paths (ValePrologue_FirstRockFalls
  /// to
  /// ValePrologue_FourthRockFalls, FLAG_VALE_STORM_FIRST_ROCK_FELL on),
  /// ACTOR_FIRST_ROCK + 4 and + 5 already lie there, and ACTOR_FIRST_ROCK +
  /// 6
  /// and + 7 lie there only once the Boulder has passed down the river
  /// (ValePrologue_ShowBoulderSweepingDownriver).
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
  ACTOR_GARET_CHILD,
  /// ★ Named from the game: "Felix (child)" (MsgDebugGlyphName)
  ACTOR_FELIX_CHILD, ///< in the river
  /// Jenna's father, a man with brown hair in a green shirt; Jenna calls him
  /// and her mother "Mom! Dad!". "Hang on, Felix! Help is on the way!"
  /// ★ Named from the game: "Mom! Dad!" (MsgValeMomDadBack)
  ACTOR_JENNAS_FATHER,
  /// Jenna's mother, a woman with pink hair in a white apron. "Don't let go,
  /// Felix!"
  /// ★ Named from the game: "Mom! Dad!" (MsgValeMomDadBack)
  ACTOR_JENNAS_MOTHER,
  /// ★ Named from the game: "Oh no! My brother!" (MsgValeNoBrother)
  ACTOR_JENNA_CHILD,
  /// The Boulder's shadow (sprite 0x120, a black blob), drawn on alternate
  /// frames under the Boulder as it comes down the river, swelling and
  /// shrinking with its bounds (ValePrologue_UpdateBoulderShadow), and twice
  /// its
  /// size during the frozen flashes (ValePrologue_FlickerBoulderShadow).
  /// ★ Named from what the player sees: a flickering black shadow under the
  /// Boulder, ValePrologue_BoulderSweepsThemAway.gif (its render)
  ACTOR_BOULDER_SHADOW,
  // The elders holding back the Boulder with their Psynergy ("Go aid the
  // elders!", vale_plaza.c), from gValePrologue_BoulderWatchers, which
  // ValePrologue_WatchBoulderFall places after the scene's own actors and
  // removes.
  // Two invisible actors stand on the spots of ACTOR_ELDER_EAST and
  // ACTOR_ELDER_WEST, and some of the elders' lines come from them.
  /// The invisible actor on ACTOR_ELDER_EAST's spot that speaks for
  /// that elder,
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
  /// The invisible actor on ACTOR_ELDER_WEST's spot that speaks for
  /// that elder,
  /// the elders' second line. "It's too heavy! We can't hold it for long!"
  /// ★ Named from the game: "We can't hold it for long"
  /// (MsgValeTheBoulderIsFalling)
  ACTOR_ELDER_WEST_STAND_IN,
  /// The elders between the two are men with long brown hair and beards in
  /// long robes.
  /// ★ Named from the game: "Go aid the elders!" (MsgValeGoAidElders)
  ACTOR_ELDER_EAST, ///< east of ACTOR_ELDER_WEST, and drawn as that elder
                    ///< is
  /// ★ Named from the game: "Go aid the elders!" (MsgValeGoAidElders)
  ACTOR_ELDER_WEST
};

/// Animations of the night's own sprites beyond enum ActorAnimation, as
/// each sprite's animation table and frames under graphics/sprite draw them.
enum StormNightAnimation {
  /// Garet leaning into his chest to drag it (spr_019.png).
  ANIM_GARET_DRAG = 5,
  /// Kyle crouched at the bank, reaching out to Felix (spr_053.png).
  ANIM_KYLE_REACH_OUT = 5,
  /// Jenna's father and mother crouched at the bank, reaching out
  /// (spr_059.png, spr_060.png).
  ANIM_PARENT_REACH_OUT = 6,
  /// The strongman fallen back on the ground (spr_116.png).
  ANIM_STRONGMAN_SPRAWLED = 6,
  /// Only Felix's hands above the water as he goes under (spr_036.png).
  ANIM_FELIX_UNDER_WATER = 6,
  /// Felix clinging on in the river, one arm reaching out (spr_036.png).
  ANIM_FELIX_CLING = 7,
  /// A bearded elder seen from behind, both arms raised against the Boulder
  /// (spr_122.png).
  ANIM_BEARDED_ELDER_HOLD_BACK = 5,
  /// The young man seen from behind, both arms raised against the Boulder
  /// (spr_108.png).
  ANIM_YOUNG_ELDER_HOLD_BACK = 8,
  /// The young man facing the player, his hands raised to his mouth to call
  /// out (spr_108.png).
  ANIM_YOUNG_ELDER_CALL_OUT = 9,
  /// The strongman propped up on one hand, the two facing opposite ways; the
  /// leader's heading picks one (spr_116.png).
  ANIM_STRONGMAN_PROPPED_UP = 7,
  ANIM_STRONGMAN_PROPPED_UP_OTHER_WAY = 8,
  // The poses frozen in the lightning as the Boulder comes down the river
  // (ValePrologue_ShowBoulderSweepingDownriver).
  ANIM_STRONGMAN_RUN = 5,    ///< in mid-stride (spr_116.png)
  ANIM_DORA_RUN = 9,         ///< in mid-stride (spr_031.png)
  ANIM_JENNA_RUN = 5,        ///< in mid-stride (spr_048.png)
  ANIM_ISAAC_RECOIL = 17,    ///< arms flung out (spr_018.png)
  ANIM_PARENT_RECOIL = 5,    ///< seen from behind, arms flung out
  ANIM_FELIX_CLING_STILL = 8 ///< ANIM_FELIX_CLING's first frame held
};

/// Vale's area flags this scene keeps while the party is in Vale.
enum StormNightAreaFlag {
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
  FLAG_VALE_STORM_FIRST_ROCK_PASSED = FLAG_AREA_TEMP_10,
  /// ★ Named from the game: "We can't get around this rock!"
  /// (MsgValeCantGetAroundThisRock)
  FLAG_VALE_STORM_SECOND_ROCK_PASSED = FLAG_AREA_TEMP_11,
  /// ★ Named from the game: "We can't get around this rock!"
  /// (MsgValeCantGetAroundThisRock)
  FLAG_VALE_STORM_THIRD_ROCK_PASSED = FLAG_AREA_TEMP_12,
  /// ★ Named from the game: "We can't get around this rock!"
  /// (MsgValeCantGetAroundThisRock)
  FLAG_VALE_STORM_FOURTH_ROCK_PASSED = FLAG_AREA_TEMP_13
};

// Scene flags, cleared on every ordinary arrival:
// - FLAG_VALE_ISAACS_DOOR_OPEN: the door of Isaac's house, from which Dora and
//   Isaac came, stands open.
// - FLAG_VALE_SHOVING_MANS_DOOR_OPEN: the stout man has run out of the door of
//   VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_SOUTH, leaving it open.
// - FLAG_VALE_ON_BRIDGE: the leader is on the rope bridge over the river.

static s32 sShakeDone;
static s32 sShakeShift;

/// The step of gValePrologue_LookAroundScript: the actor faces a
/// random way every 20 to 39 frames, as everyone looks about once the Boulder
/// has gone by.
static s32 ValePrologue_RerollHeadingOnCountdown(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  s32 timer = actor->timer;
  s32 countdown = (s16)timer;

  if (timer == 0) {
    actor->facing = Import_RandomNext();
    countdown = Math_RemainderUnsigned(Import_RandomNext(), 20) + 20;
    actor->timer = countdown;
  }
  actor->timer = countdown - 1;
  return 1;
}

#include "shared/actor_pulse.h"

/// The door of Isaac's house, from which Dora and Isaac came out into the
/// storm, stands open.
/// ★ Named from the game: "This old house will fall apart if you stay much
/// longer." (MsgValeAsStubbornAsYourFather)
#define FLAG_VALE_ISAACS_DOOR_OPEN FLAG_SCENE_TEMP_05
/// The stout man has run out of his door and shoved Isaac, leaving the door
/// open.
/// ★ Named from the game: "Oh... Sorry to shove you like that." (MsgValeHuh)
#define FLAG_VALE_SHOVING_MANS_DOOR_OPEN FLAG_SCENE_TEMP_06
/// The leader is on the rope bridge over the river. While it is set, the
/// bridge's cells take the deck's shapes and cell behaviors: its two middle
/// rows walkable at the deck's height and its rails walls; stepping off puts
/// back the river's, walled water between open banks.
/// ★ Named from what the player sees: a wooden bridge with rope rails over the
/// river, above the waterfall (its render)
#define FLAG_VALE_ON_BRIDGE FLAG_SCENE_TEMP_10

/// Returns where the party appears;
/// VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WITH_DORA is where Dora and Isaac
/// come out of the door of their house, from SceneId_CottageRoomsB's
/// COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_WITH_DORA.
const struct SceneEntrance *ValePrologue_GetEntrances(void) {
  return gValePrologue_Entrances;
}

const struct SceneExitRegion *ValePrologue_GetRegions(void) { return NULL; }

const u32 *ValePrologue_GetExits(void) { return gValePrologue_Exits; }

const struct ActorPlacement *ValePrologue_GetActorPlacements(void) {
  return gValePrologue_ActorPlacements;
}

const struct Trigger *ValePrologue_GetTriggers(void) {
  return gValePrologue_Triggers;
}

/// In Vale on the stormy night, swaps the bridge's cell behaviors as
/// the leader steps onto it. Reached by stepping on cell behavior 15. Sets
/// FLAG_VALE_ON_BRIDGE, which ValePrologue_Initialize reads to swap them
/// again.
static void ValePrologue_StepOntoBridge(void) {
  GameFlag_Set(FLAG_VALE_ON_BRIDGE);
  MapGrid_CopyCellAttributes(40, 84, 7, 4, 10, 84);
}

/// In Vale on the stormy night, restores the bridge's cell behaviors as
/// the leader steps off it. Reached by stepping on cell behavior 14. Clears
/// FLAG_VALE_ON_BRIDGE.
static void ValePrologue_StepOffBridge(void) {
  GameFlag_Clear(FLAG_VALE_ON_BRIDGE);
  MapGrid_CopyCellAttributes(40, 89, 7, 4, 10, 84);
}

#include "shared/vale.h"

/// Unused: on the stormy night, the Sanctum's door opens and the leader goes
/// into a house of Vale.
///
/// It takes VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST_SANCTUM_DOOR
/// (SceneId_CottageRoomsA, at COTTAGE_ROOMS_A_FROM_VALE_WEST, the
/// house VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST and its second door lead to
/// as well). No trigger names it, and nothing
/// else takes that exit.
UNUSED static void ValePrologue_UnusedOpenSanctumDoor(void) {
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(gValePrologue_SanctumDoorOpens, 45, 11);
  Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 257, 420);
  Event_Wait(3);
  Vale_RequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST_SANCTUM_DOOR);
}

/// Leaves through an exit the exit table doesn't list. In Vale on the stormy
/// night, stepping on cell behavior 2 requests exit 1, the one number the
/// exit table leaves out; Scene_TakeExit therefore falls back to the first
/// exit's scene, Vale's SceneId_ValePlaza, at ENTRANCE_DEFAULT.
static void ValePrologue_LeaveNorthwestToValePlaza(void) {
  Sound_Play(SE_MAP_EXIT);
  Vale_RequestBackdropExit(1);
}

/// In Vale on the stormy night, leaves for the part of Vale around Kraden's
/// cottage.
/// Reached by stepping on cell behavior 3; arrives at
/// VALE_KRADENS_COTTAGE_FROM_VALE_NORTH.
static void ValePrologue_LeaveToValeKradensCottageNorth(void) {
  Sound_Play(SE_MAP_EXIT);
  Vale_RequestBackdropExit(VALE_PROLOGUE_TO_VALE_KRADENS_COTTAGE_NORTH);
}

/// In Vale on the stormy night, leaves for the part of Vale around Kraden's
/// cottage by its second path.
/// Reached by stepping on cell behavior 4; arrives at
/// VALE_KRADENS_COTTAGE_FROM_VALE_SOUTH.
static void ValePrologue_LeaveToValeKradensCottageSouth(void) {
  Sound_Play(SE_MAP_EXIT);
  Vale_RequestBackdropExit(VALE_PROLOGUE_TO_VALE_KRADENS_COTTAGE_SOUTH);
}

/// In Vale on the stormy night, leaves for the plaza, where the villagers have
/// gathered (SceneId_ValePlaza).
/// Reached by stepping on cell behavior 5; arrives at VALE_PLAZA_FROM_VALE.
/// After the Boulder swept them away, Garet first resolves to go after Isaac
/// (ValePrologue_GaretFollowsIsaac).
static void ValePrologue_LeaveToValePlaza(void) {
  Sound_Play(SE_MAP_EXIT);
  if (GameFlag_Get(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) != 0 &&
      GameFlag_Get(FLAG_VALE_STORM_GARET_LEFT) == 0) {
    ValePrologue_GaretFollowsIsaac();
  }
  Vale_RequestBackdropExit(VALE_PROLOGUE_TO_VALE_PLAZA);
}

/// In Vale on the stormy night, opens the door of Garet's house and goes in.
/// Reached by walking north into cell behavior 6; arrives at
/// COTTAGE_ROOMS_A_FROM_VALE_EAST, the house where Garet's family lives.
static void ValePrologue_LeaveToCottageRoomsAEast(void) {
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(gValePrologue_IsaacsDoorOpens, 54, 32);
  Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 406, 727);
  Event_Wait(3);
  Vale_RequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_EAST);
}

/// A stout man runs out of a house and shoves Isaac. In Vale on the stormy
/// night, this is Isaac's first time at the door of
/// VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_SOUTH; afterwards the door opens as
/// usual. "Huh? Isaac?" Reached by walking north into cell behavior 7. The
/// shove plays until FLAG_VALE_STORM_BUMPED_INTO_VILLAGER or
/// FLAG_VALE_STORM_SECOND_ROCK_FELL is set, and sets
/// FLAG_VALE_SHOVING_MANS_DOOR_OPEN; otherwise goes into a house of Vale
/// (SceneId_CottageRoomsA, at COTTAGE_ROOMS_A_FROM_VALE_SOUTH).
static void ValePrologue_LeaveToCottageRoomsA(void) {
  if (Event_GetFlag(FLAG_VALE_SHOVING_MANS_DOOR_OPEN) == 0) {
    Event_PlaySound(SE_DOOR_OPEN);
    MapGrid_AnimateCells(gValePrologue_GrandparentsDoorOpens, 45, 39);
  }
  if (GameFlag_Get(FLAG_VALE_STORM_BUMPED_INTO_VILLAGER) == 0 &&
      GameFlag_Get(FLAG_VALE_STORM_SECOND_ROCK_FELL) == 0) {
    ValePrologue_BumpIntoShovingMan();
    GameFlag_Set(FLAG_VALE_SHOVING_MANS_DOOR_OPEN);
  } else {
    Actor_WalkTo(ACTOR_PARTY_LEADER, 262, 805);
    Event_Wait(3);
    Vale_RequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_SOUTH);
  }
}

/// Goes back into Isaac's house on the stormy night. It's the door he and Dora
/// came out of in Vale, and it opens unless they left it open.
/// Reached by walking north into cell behavior 8; takes
/// VALE_PROLOGUE_TO_COTTAGE_ROOMS_B_ISAACS_HOUSE, to
/// COTTAGE_ROOMS_B_FROM_VALE in SceneId_CottageRoomsB, where Vale's
/// VALE_TO_COTTAGE_ROOMS_B leads by day.
static void ValePrologue_LeaveToIsaacsHouse(void) {
  if (GameFlag_Get(FLAG_VALE_ISAACS_DOOR_OPEN) == 0) {
    Sound_Play(SE_DOOR_OPEN);
    MapGrid_PlayMapEdit(gValePrologue_IsaacsDoorOpens, 50, 44);
  }
  Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 340, 888);
  Event_Wait(3);
  Vale_RequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_B_ISAACS_HOUSE);
}

/// In Vale on the stormy night, opens a door and goes in.
/// Reached by walking north into cell behavior 9; arrives in a house of
/// Vale at COTTAGE_ROOMS_A_FROM_VALE_NORTH.
static void ValePrologue_LeaveToCottageRoomsANorth(void) {
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(gValePrologue_GrandparentsDoorOpens, 49, 69);
  Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 326, 1126);
  Event_Wait(3);
  Vale_RequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_NORTH);
}

/// In Vale on the stormy night, opens a door and goes in.
/// Reached by walking north into cell behavior 10; takes
/// VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST, to
/// COTTAGE_ROOMS_A_FROM_VALE_WEST in SceneId_CottageRoomsA, where
/// Vale's VALE_TO_COTTAGE_ROOMS_A_WEST leads by day.
static void ValePrologue_LeaveToCottageRoomsAWest(void) {
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(gValePrologue_WestHouseDoorOpens, 52, 76);
  Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 374, 1238);
  Event_Wait(3);
  Vale_RequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST);
}

/// Unused: its row answers cell behavior 11, which no cell of the stormy
/// night's layout (map/map_000) carries. It would have opened Isaac's
/// door, as ValePrologue_LeaveToCottageRoomsBWest does for behavior 12,
/// walked the leader to the same spot and taken
/// VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST_SECOND_DOOR into the house
/// ValePrologue_LeaveToCottageRoomsAWest leads to
/// (COTTAGE_ROOMS_A_FROM_VALE_WEST).
static void ValePrologue_UnusedLeaveToCottageRoomsAWestBySecondDoor(void) {
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gValePrologue_IsaacsDoorOpens, 35, 74);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 1206);
  Event_Wait(3);
  Vale_RequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST_SECOND_DOOR);
}

/// In Vale on the stormy night, opens a door and goes in.
/// Reached by walking north into cell behavior 12; arrives in the house
/// with two beds before a hearth, COTTAGE_ROOMS_B_FROM_VALE_PROLOGUE_WEST.
static void ValePrologue_LeaveToCottageRoomsBWest(void) {
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gValePrologue_IsaacsDoorOpens, 35, 73);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 1206);
  Event_Wait(3);
  Vale_RequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_B_WEST);
}

/// In Vale on the stormy night, opens a door and goes in.
/// Reached by walking north into cell behavior 13; arrives in the house
/// with two beds before a hearth, COTTAGE_ROOMS_B_FROM_VALE_PROLOGUE_EAST.
static void ValePrologue_LeaveToCottageRoomsBEast(void) {
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gValePrologue_GrandparentsDoorOpens, 38, 72);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 146, 1182);
  Event_Wait(3);
  Vale_RequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_B_EAST);
}

/// Arriving in Vale on the stormy night: restores the fallen rocks, the river
/// event and the Boulder from their flags, starts the storm, and plays the
/// event with Dora and Kyle on the first arrival by
/// VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WITH_DORA. Called by
/// FieldSequence_Run once the actors are placed. Sets FLAG_VALE_STORM; after a
/// battle (FLAG_KEEP_PARTY_POSITION) the two doors close again. Garet, until he
/// joins, is shown struggling with his things.
s32 ValePrologue_Initialize(void) {
  Scene_QueueMusic(SE_AMBIENT_WATER);
  Import_ActorSetPosition(ACTOR_FELIX_CHILD, 0, 0);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    GameFlag_Clear(FLAG_VALE_ISAACS_DOOR_OPEN);
    GameFlag_Clear(FLAG_VALE_SHOVING_MANS_DOOR_OPEN);
  }
  // Restore the rocks that have already fallen.
  if (GameFlag_Get(FLAG_VALE_STORM_FIRST_ROCK_FELL) != 0) {
    Actor_SetPosition(ACTOR_FIRST_ROCK, PIXELS(334), PIXELS(932));
    ValePrologue_ApplyFirstRockCollision();
  }
  if (GameFlag_Get(FLAG_VALE_STORM_SECOND_ROCK_FELL) != 0) {
    Actor_SetPosition(ACTOR_FIRST_ROCK + 1, PIXELS(224), PIXELS(872));
    ValePrologue_ApplySecondRockCollision();
  }
  if (GameFlag_Get(FLAG_VALE_STORM_THIRD_ROCK_FELL) != 0) {
    Actor_SetPosition(ACTOR_FIRST_ROCK + 2, PIXELS(64), PIXELS(703));
    ValePrologue_ApplyThirdRockCollision();
  }
  if (GameFlag_Get(FLAG_VALE_STORM_FOURTH_ROCK_FELL) != 0) {
    Actor_SetPosition(ACTOR_FIRST_ROCK + 3, PIXELS(432), PIXELS(1147));
    ValePrologue_ApplyFourthRockCollision();
  }
  // The rocks are big, so the leader can talk to them from further off: the
  // talk target search counts their distance at 10/13 (ACTOR_NEAR_TALK).
  Actor_GetById(ACTOR_FIRST_ROCK)->collisionFlags |= ACTOR_NEAR_TALK;
  Actor_GetById(ACTOR_FIRST_ROCK + 1)->collisionFlags |= ACTOR_NEAR_TALK;
  Actor_GetById(ACTOR_FIRST_ROCK + 2)->collisionFlags |= ACTOR_NEAR_TALK;
  Actor_GetById(ACTOR_FIRST_ROCK + 3)->collisionFlags |= ACTOR_NEAR_TALK;
  Actor_GetById(ACTOR_FIRST_ROCK + 4)->collisionFlags |= ACTOR_NEAR_TALK;
  Actor_GetById(ACTOR_FIRST_ROCK + 5)->collisionFlags |= ACTOR_NEAR_TALK;
  Actor_GetById(ACTOR_FIRST_ROCK + 6)->collisionFlags |= ACTOR_NEAR_TALK;
  Actor_GetById(ACTOR_FIRST_ROCK + 7)->collisionFlags |= ACTOR_NEAR_TALK;
  // Garet walks with the party once he has joined.
  if (GameFlag_Get(FLAG_VALE_STORM_GARET_JOINED) != 0) {
    Import_ActorSetPosition(ACTOR_GARET_CHILD, 0, 0);
  }
  {
    struct Actor *actor;

    actor = Actor_GetById(ACTOR_BOULDER);
    actor->scaleX = 2 * Q16_ONE;
    actor->scaleY = 2 * Q16_ONE;
  }
  // The Boulder lies where it fell, or waits on the mountainside.
  if (GameFlag_Get(FLAG_VALE_BOULDER_FELL) != 0) {
    Actor_SetPosition(ACTOR_BOULDER, PIXELS(114), PIXELS(333));
  } else {
    Actor_SetScript(ACTOR_BOULDER, gValePrologue_BoulderWaits);
  }
  // Restore the riverbank cast at the saved stage of the storm.
  if (GameFlag_Get(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) != 0) {
    s32 facing;
    const s32 *table;
    ValePrologue_RebuildRiverbank();
    Actor_SetPosition(ACTOR_DORA, PIXELS(165), PIXELS(1229));
    {
      struct Actor *actor;

      actor = Actor_GetById(ACTOR_DORA);
      facing = FACING_NORTHEAST;
      actor->facing = facing;
      actor->timer = Math_RemainderUnsigned(Import_RandomNext(), 90) + 60;
      table = gValePrologue_SquashAndStretchScript;
      actor->userValue = 1;
      Actor_SetScript(ACTOR_DORA, table);
    }
    Import_ActorSetPosition(ACTOR_JENNA_CHILD, PIXELS(165), PIXELS(1254));
    {
      struct Actor *actor;

      actor = Actor_GetById(ACTOR_JENNA_CHILD);
      actor->facing = facing;
      actor->timer = Math_RemainderUnsigned(Import_RandomNext(), 90) + 60;
      actor->userValue = 2;
      Actor_SetScript(ACTOR_JENNA_CHILD, table);
    }
    Import_ActorSetPosition(ACTOR_GARET_CHILD, PIXELS(152), PIXELS(1285));
    {
      struct Actor *actor;

      actor = Actor_GetById(ACTOR_GARET_CHILD);
      actor->facing = facing;
      actor->timer = Math_RemainderUnsigned(Import_RandomNext(), 90) + 60;
      actor->userValue = 3;
      Actor_SetScript(ACTOR_GARET_CHILD, table);
    }
    Import_ActorSetPosition(ACTOR_EXHAUSTED_STRONGMAN, PIXELS(184),
                            PIXELS(1304));
    {
      struct Actor *actor;

      actor = Actor_GetById(ACTOR_EXHAUSTED_STRONGMAN);
      actor->facing = facing;
      actor->timer = Math_RemainderUnsigned(Import_RandomNext(), 90) + 60;
      actor->userValue = 4;
      Actor_SetScript(ACTOR_EXHAUSTED_STRONGMAN, table);
    }
    Import_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                             ANIM_STRONGMAN_SPRAWLED);
    Actor_GetById(ACTOR_GARET_CHILD)->priorityFlags &=
        ~ACTOR_PRIORITY_AUTOMATIC;
    Actor_GetById(ACTOR_EXHAUSTED_STRONGMAN)->priorityFlags &=
        ~ACTOR_PRIORITY_AUTOMATIC;
    Task_AddFrame(ValePrologue_CopyLeaderSpritePriority, TASK_PASS_FRAME);
    Import_ActorSetPosition(ACTOR_JENNAS_FATHER, 0, 0);
    Import_ActorSetPosition(ACTOR_JENNAS_MOTHER, 0, 0);
    Import_ActorSetPosition(ACTOR_FELIX_CHILD, 0, 0);
    Import_ActorSetPosition(ACTOR_BOULDER, 0, 0);
    if (GameFlag_Get(FLAG_VALE_STORM_GARET_LEFT) != 0) {
      Import_ActorSetPosition(ACTOR_GARET_CHILD, 0, 0);
    }
    // While they try to save Felix: Kyle and Jenna's parents at the river,
    // Felix in it.
  } else if (GameFlag_Get(FLAG_VALE_STORM_SAW_FELIX_HANGING_ON) != 0) {
    const s32 *table;
    Actor_SetPosition(ACTOR_KYLE, PIXELS(192), PIXELS(1214));
    Actor_FaceDirection(ACTOR_KYLE, FACING_SOUTHEAST, 0);
    Import_ActorSetAnimation(ACTOR_KYLE, ANIM_KYLE_REACH_OUT);
    {
      struct Actor *actor;

      actor = Actor_GetById(ACTOR_KYLE);
      actor->timer = Math_RemainderUnsigned(Import_RandomNext(), 90) + 60;
    }
    table = gValePrologue_SquashAndStretchScript;
    Actor_SetScript(ACTOR_KYLE, table);
    Actor_SetPosition(ACTOR_JENNAS_FATHER, PIXELS(227), PIXELS(1214));
    Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_SOUTH, 0);
    Import_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_PARENT_REACH_OUT);
    {
      struct Actor *actor;

      actor = Actor_GetById(ACTOR_JENNAS_FATHER);
      actor->timer = Math_RemainderUnsigned(Import_RandomNext(), 90) + 60;
    }
    Actor_SetScript(ACTOR_JENNAS_FATHER, table);
    Actor_SetPosition(ACTOR_JENNAS_MOTHER, PIXELS(247), PIXELS(1214));
    Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_SOUTH, 0);
    Import_ActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIM_PARENT_REACH_OUT);
    {
      struct Actor *actor;

      actor = Actor_GetById(ACTOR_JENNAS_MOTHER);
      actor->timer = Math_RemainderUnsigned(Import_RandomNext(), 90) + 60;
    }
    Actor_SetScript(ACTOR_JENNAS_MOTHER, table);
    Actor_SetPosition(ACTOR_FELIX_CHILD, PIXELS(243), PIXELS(1277));
    Actor_ArmCallback(ACTOR_FELIX_CHILD, FACING_NORTH, 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FELIX_CHILD), 0);
    Import_ActorSetPosition(ACTOR_FIRST_ROCK + 6, 0, 0);
    Import_ActorSetPosition(ACTOR_FIRST_ROCK + 7, 0, 0);
  } else {
    Import_ActorSetPosition(ACTOR_FIRST_ROCK + 6, 0, 0);
    Import_ActorSetPosition(ACTOR_FIRST_ROCK + 7, 0, 0);
  }
  // The lightning starts at once, except before Dora and Kyle's event.
  if (gMainRoutineWork.entrance !=
          VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WITH_DORA ||
      GameFlag_Get(FLAG_VALE_STORM_SENT_TO_PLAZA) != 0) {
    Weather_StartRain();
    Weather_StartLightning();
  }
  if (GameFlag_Get(FLAG_VALE_ON_BRIDGE) != 0) {
    ValePrologue_StepOntoBridge();
  }
  GameFlag_Set(FLAG_VALE_STORM);
  MapGrid_CopyCellAttributes(29, 24, 1, 2, 26, 46);
  MapGrid_CopyCellAttributes(29, 25, 1, 1, 27, 46);
  MapGrid_CopyCellAttributes(29, 25, 1, 1, 28, 46);
  MapGrid_CopyCellAttributes(19, 90, 1, 1, 20, 88);
  MapGrid_CopyCellAttributes(19, 90, 1, 1, 20, 89);
  {
    struct Actor *actor;

    actor = Actor_GetById(ACTOR_GARETS_CHEST);
    actor->motionFlags = 0;
    actor->y.fixed = PIXELS(192);
    actor->collisionFlags = ACTOR_NOT_TARGETED;
    Actor_SetSpriteFlags(actor, 0);
  }
  wait(1);
  if (GameFlag_Get(FLAG_VALE_STORM_SENT_TO_PLAZA) == 0 &&
      gMainRoutineWork.entrance ==
          VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WITH_DORA) {
    ValePrologue_DoraSendsIsaacToThePlaza();
    return 0;
  }
  // Garet drags his chest out of his house until he joins, then the chest
  // stands by the door.
  Import_ActorSetAnimation(ACTOR_FELIX_CHILD, ANIM_FELIX_CLING);
  if (GameFlag_Get(FLAG_VALE_STORM_GARET_JOINED) == 0) {
    Event_Start();
    Actor_SetAttachedEffect(ACTOR_GARET_CHILD,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    Actor_SetPosition(ACTOR_GARET_CHILD, PIXELS(400), PIXELS(611));
    Actor_SetPosition(ACTOR_GARETS_CHEST, PIXELS(424), PIXELS(627));
    Actor_WalkTo(ACTOR_GARET_CHILD, 400, 619);
    Actor_WalkToAndWait(ACTOR_GARETS_CHEST, 424, 619);
    Import_ActorSetAnimation(ACTOR_GARETS_CHEST, ANIM_WALK);
    Import_ActorSetAnimation(ACTOR_GARET_CHILD, ANIM_GARET_DRAG);
    Event_End();
  } else {
    Event_Start();
    Actor_SetPosition(ACTOR_GARETS_CHEST, PIXELS(424), PIXELS(627));
    Actor_WalkToAndWait(ACTOR_GARETS_CHEST, 424, 619);
    Import_ActorSetAnimation(ACTOR_GARETS_CHEST, ANIM_NOD);
    Event_End();
  }
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 24;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Weather_ResumeLightning();
  return 0;
}

/// In Vale on the stormy night, Dora comes out into the storm with Isaac to
/// Kyle, and the two send Isaac on to the plaza while they go to help. "Kyle...
/// Will they be able to stop the Boulder?" Called by ValePrologue_Initialize
/// on arrival by VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WITH_DORA until
/// FLAG_VALE_STORM_SENT_TO_PLAZA is set. Sets it and
/// FLAG_VALE_ISAACS_DOOR_OPEN, clears FLAG_SHOW_AREA_NAME, and removes Dora
/// and Kyle.
static void ValePrologue_DoraSendsIsaacToThePlaza(void) {
  s32 message;

  Event_Start();
  Weather_StartRain();
  Weather_StartLightning();
  Weather_LastLightning();
  wait(60);
  // The camera looks down on the door of Isaac's house; Kyle waits outside.
  FieldCamera_SetSpeed(PIXELS(1) / 4, PIXELS(1) / 32);
  FieldCamera_MoveTo(PIXELS(316), PIXELS(160), PIXELS(880), 1);
  Actor_SetPosition(ACTOR_KYLE, PIXELS(294), PIXELS(868));
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 16;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Weather_ResumeLightning();
  // Dora and Isaac come out of the door.
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gValePrologue_IsaacsDoorOpens, 50, 44);
  Actor_SetAttachedEffect(ACTOR_GARET_CHILD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_KYLE, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_DORA, PIXELS(342), PIXELS(890));
  Actor_WalkToAndWait(ACTOR_DORA, 342, 905);
  Scene_PlayQueuedMusic();
  Actor_WalkTo(ACTOR_DORA, 296, 905);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(342), PIXELS(890));
  Actor_WalkTo(ACTOR_PARTY_LEADER, 342, 890);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 342, 905);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 318, 905);
  Import_ActorSetAnimation(ACTOR_DORA, ANIM_STAND);
  Actor_RunRepeatedMotion(ACTOR_DORA, 1);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 60);
  // Dora: "Kyle... Will they be able to stop the Boulder?" Kyle: "I don't think
  // so..." and tells the two to take refuge in the plaza.
  message = MsgValeKyleAbleStop;
  Event_SetMessageId(message);
  Event_ShowMessage(ACTOR_DORA, 0);
  Actor_WalkToAndWait(ACTOR_KYLE, 294, 838);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_KYLE, ANIM_SHAKE_HEAD);
  Event_ShowMessage(ACTOR_KYLE, 0);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_KYLE, FACING_SOUTH, 0);
  FieldEvent_ShowMessageThenWait(ACTOR_KYLE, 0, 20);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 10);
  // "Aren't you coming?" "I need to help evacuate the other villagers."
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KYLE, ANIM_SHAKE_HEAD);
  Event_ShowMessage(ACTOR_KYLE, 0);
  Actor_SetAttachedEffect(ACTOR_DORA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 50);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 10);
  // Dora runs to Kyle: "Let me help you, Kyle!" "It's too dangerous, Dora."
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_WalkToAndWait(ACTOR_DORA, 289, 883);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTHEAST, 0);
  Event_ShowMessage(ACTOR_DORA, 0);
  Actor_RunRepeatedMotion(ACTOR_KYLE, 2);
  Event_ShowMessage(ACTOR_KYLE, 0);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  // "Isaac is old enough to get to the plaza on his own."
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST, 10);
  // "You can find your way, can't you, Isaac?" She asks again on every no:
  // "You're a big boy now, Isaac."
  Event_SetMessageId((message + 8));
  Event_OpenMessage(ACTOR_DORA, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 302, 905);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  while (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    Actor_RunRepeatedMotion(ACTOR_DORA, 1);
    Event_SetMessageId(MsgValeBigBoyWhy);
    Event_OpenMessage(ACTOR_DORA, 0);
  }
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  // "You know the way--just go south to get to the plaza. Be careful!"
  Event_SetMessageId(MsgValeKnowWayGo);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  // Kyle and Dora run north toward the Boulder and out of sight.
  Actor_SetSpeed(ACTOR_KYLE, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_WalkTo(ACTOR_KYLE, 297, 750);
  Event_Wait(10);
  Actor_WalkToAndWait(ACTOR_DORA, 297, 750);
  Actor_SetPosition(ACTOR_DORA, 0, 0);
  Actor_SetPosition(ACTOR_KYLE, 0, 0);
  Import_ActorSetAnimation(ACTOR_KYLE, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_GARETS_CHEST, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_GARET_CHILD, ANIM_GARET_DRAG);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  GameFlag_Set(FLAG_VALE_STORM_SENT_TO_PLAZA);
  GameFlag_Set(FLAG_VALE_ISAACS_DOOR_OPEN);
  Event_End();
}

/// In Vale on the stormy night, walking past: the first rock
/// (ACTOR_FIRST_ROCK) falls across the path, and Garet, once with Isaac, says
/// they cannot get around it.
/// Reached by stepping on cell behavior 41 until
/// FLAG_VALE_STORM_FIRST_ROCK_PASSED is set, which this sets. Sets
/// FLAG_VALE_STORM_FIRST_ROCK_FELL as the rock falls; with Garet along, before
/// the Boulder swept Felix, his parents and Kyle away, his first comment on any
/// rock sets FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH.
static void ValePrologue_FirstRockFalls(void) {
  struct Actor *rock;
  struct Actor *leader;
  s32 targetHeight;
  s32 walkZ;
  struct Sprite *rockSprite;

  if (Event_GetFlag(FLAG_VALE_STORM_FIRST_ROCK_PASSED) != 0) {
  } else {
    Event_Start();
    if (Event_GetFlag(FLAG_VALE_STORM_FIRST_ROCK_FELL) == 0) {
      rock = Actor_GetById(ACTOR_FIRST_ROCK);
      rockSprite = rock->sprite;
      // A rumble (SE_RUMBLE, then SE_CRASH), and the rock bounces down from the
      // east.
      MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      Sound_Play(SE_RUMBLE);
      wait(40);
      Sound_Play(SE_CRASH);
      rock->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      rockSprite->image.priority = 1;
      Actor_SetPosition(ACTOR_FIRST_ROCK, PIXELS(473), PIXELS(932));
      rock->speedLimit = PIXELS(3) / 2;
      rock->acceleration = PIXELS(3) / 2;
      targetHeight = (rock->y.fixed + PIXELS(240));
      rock->y.fixed += PIXELS(240);
      rock->targetY = targetHeight;
      rock->bounce = 0x6666;
      Actor_WalkToAndWait(ACTOR_FIRST_ROCK, 344, 932);
      rockSprite->image.priority = 3;
      rock->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      Event_Wait(40);
      // It lands: the rumble on player 3 fades and the shaking dies down.
      Sound_Play(SOUND_FADE_OUT_ACTION_SE);
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
      MapGrid_WaitForShakeToSettle();
      Scene_PlayQueuedMusic();
      GameFlag_Set(FLAG_VALE_STORM_FIRST_ROCK_FELL);
    }
    // The rock now blocks the path.
    ValePrologue_ApplyFirstRockCollision();
    GameFlag_Set(FLAG_VALE_STORM_FIRST_ROCK_PASSED);
    if (GameFlag_Get(FLAG_VALE_STORM_GARET_JOINED) != 0) {
      if (GameFlag_Get(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) == 0) {
        if (GameFlag_Get(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH) == 0) {
          // Garet comes out beside Isaac on whichever side of the rock he
          // stands.
          leader = Actor_GetById(ACTOR_PARTY_LEADER);
          if (leader->y.fixed > PIXELS(128)) {
            walkZ = 918;
            ValePrologue_GaretCommentsOnBlockedPath(326, walkZ);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 291, walkZ);
          } else {
            ValePrologue_GaretCommentsOnBlockedPath(335, 957);
          }
          GameFlag_Set(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH);
        }
      }
    }
    Event_End();
  }
}

/// Applies the cell behaviors left by the first rock (ACTOR_FIRST_ROCK).
/// Called by ValePrologue_FirstRockFalls and, once the rock has fallen, by
/// ValePrologue_Initialize.
static void ValePrologue_ApplyFirstRockCollision(void) {
  MapGrid_CopyCellAttributes(29, 64, 1, 1, 21, 57);
  MapGrid_CopyCellAttributes(29, 64, 1, 1, 21, 58);
  MapGrid_CopyCellAttributes(29, 64, 1, 1, 22, 58);
  MapGrid_CopyCellAttributes(29, 64, 1, 1, 20, 58);
  MapGrid_CopyCellAttributes(28, 20, 1, 1, 20, 57);
}

/// In Vale on the stormy night, walking past: the second rock
/// (ACTOR_FIRST_ROCK + 1) falls down the path.
/// Reached by stepping on cell behavior 42; as
/// ValePrologue_FirstRockFalls does the first, with
/// FLAG_VALE_STORM_SECOND_ROCK_FELL and FLAG_VALE_STORM_SECOND_ROCK_PASSED.
static void ValePrologue_SecondRockFalls(void) {
  struct Actor *rock;
  struct Actor *leader;
  s32 targetHeight;

  if (Event_GetFlag(FLAG_VALE_STORM_SECOND_ROCK_PASSED) != 0) {
  } else {
    Event_Start();
    if (Event_GetFlag(FLAG_VALE_STORM_SECOND_ROCK_FELL) == 0) {
      rock = Actor_GetById(ACTOR_FIRST_ROCK + 1);
      // A rumble, then the rock rolls down the slope from the north-east.
      MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      Sound_Play(SE_RUMBLE);
      wait(40);
      Sound_Play(SE_CRASH);
      Actor_SetPosition(ACTOR_FIRST_ROCK + 1, PIXELS(381), PIXELS(808));
      rock->speedLimit = PIXELS(3) / 2;
      rock->acceleration = PIXELS(3) / 2;
      targetHeight = (rock->y.fixed + PIXELS(256));
      rock->y.fixed += PIXELS(256);
      rock->targetY = targetHeight;
      rock->bounce = 0x8000;
      Actor_WalkToAndWait(ACTOR_FIRST_ROCK + 1, 290, 833);
      Actor_SetSpritePriority(ACTOR_FIRST_ROCK + 1, 1);
      Actor_WalkToAndWait(ACTOR_FIRST_ROCK + 1, 258, 852);
      Actor_SetSpritePriority(ACTOR_FIRST_ROCK + 1, 2);
      Actor_WalkToAndWait(ACTOR_FIRST_ROCK + 1, 224, 872);
      Event_Wait(40);
      // It lands: the rumble on player 3 fades and the shaking dies down.
      Sound_Play(SOUND_FADE_OUT_ACTION_SE);
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
      MapGrid_WaitForShakeToSettle();
      Scene_PlayQueuedMusic();
      GameFlag_Set(FLAG_VALE_STORM_SECOND_ROCK_FELL);
    }
    ValePrologue_ApplySecondRockCollision();
    GameFlag_Set(FLAG_VALE_STORM_SECOND_ROCK_PASSED);
    if (GameFlag_Get(FLAG_VALE_STORM_GARET_JOINED) != 0) {
      if (GameFlag_Get(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) == 0) {
        if (GameFlag_Get(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH) == 0) {
          // Garet comes out beside Isaac on whichever side of the rock he
          // stands.
          leader = Actor_GetById(ACTOR_PARTY_LEADER);
          if (leader->y.fixed > PIXELS(128)) {
            ValePrologue_GaretCommentsOnBlockedPath(219, 843);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 179, 829);
          } else {
            ValePrologue_GaretCommentsOnBlockedPath(214, 908);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 219, 911);
          }
          GameFlag_Set(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH);
        }
      }
    }
    Event_End();
  }
}

/// Applies the cell behaviors left by the second rock (ACTOR_FIRST_ROCK +
/// 1). Called by ValePrologue_SecondRockFalls and, once the rock has fallen,
/// by ValePrologue_Initialize.
static void ValePrologue_ApplySecondRockCollision(void) {
  MapGrid_CopyCellAttributes(29, 23, 1, 1, 15, 53);
  MapGrid_CopyCellAttributes(29, 23, 1, 1, 14, 53);
  MapGrid_CopyCellAttributes(29, 23, 1, 1, 13, 53);
  MapGrid_CopyCellAttributes(26, 20, 2, 1, 14, 52);
  MapGrid_CopyCellAttributes(25, 21, 1, 1, 13, 54);
  MapGrid_CopyCellAttributes(25, 21, 1, 1, 15, 54);
  MapGrid_CopyCellAttributes(14, 53, 1, 1, 14, 54);
  MapGrid_CopyCellAttributes(13, 55, 1, 1, 15, 55);
}

/// In Vale on the stormy night, walking past: the third rock
/// (ACTOR_FIRST_ROCK + 2) falls down the path.
/// Reached by stepping on cell behavior 43; as
/// ValePrologue_FirstRockFalls does the first, with
/// FLAG_VALE_STORM_THIRD_ROCK_FELL and FLAG_VALE_STORM_THIRD_ROCK_PASSED.
static void ValePrologue_ThirdRockFalls(void) {
  if (Event_GetFlag(FLAG_VALE_STORM_THIRD_ROCK_PASSED) == 0) {
    Event_Start();
    if (Event_GetFlag(FLAG_VALE_STORM_THIRD_ROCK_FELL) == 0) {
      struct Actor *actor = Actor_Get(ACTOR_FIRST_ROCK + 2);
      struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
      u16 priority = leader->sprite->image.priority;
      u8 flags = leader->priorityFlags;

      // A rumble, then the rock drops in from the west while Isaac is drawn
      // behind it.
      MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      Event_PlaySound(SE_RUMBLE);
      wait(40);
      Event_PlaySound(SE_CRASH);
      Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
      Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      Actor_SetPosition(ACTOR_FIRST_ROCK + 2, 0, PIXELS(703));
      actor->speedLimit = PIXELS(3) / 2;
      actor->acceleration = PIXELS(3) / 2;
      actor->y.fixed += PIXELS(80);
      actor->targetY = actor->y.fixed;
      actor->bounce = 0x8000;
      Actor_WalkToAndWait(ACTOR_FIRST_ROCK + 2, 64, 703);
      Event_Wait(40);
      // It lands: the rumble fades, and Isaac is drawn as before.
      Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
      MapGrid_WaitForShakeToSettle();
      Scene_PlayQueuedMusic();
      GameFlag_Set(FLAG_VALE_STORM_THIRD_ROCK_FELL);
      Actor_SetSpritePriority(ACTOR_PARTY_LEADER, priority);
      Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      leader->priorityFlags = flags;
    }
    ValePrologue_ApplyThirdRockCollision();
    GameFlag_Set(FLAG_VALE_STORM_THIRD_ROCK_PASSED);
    if (GameFlag_Get(FLAG_VALE_STORM_GARET_JOINED) != 0) {
      if (GameFlag_Get(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) == 0) {
        if (GameFlag_Get(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH) == 0) {
          // Garet comes out beside Isaac on whichever side of the rock he
          // stands.
          if (Actor_Get(ACTOR_PARTY_LEADER)->z.fixed <= PIXELS(693) - 1) {
            ValePrologue_GaretCommentsOnBlockedPath(62, 669);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 27, 627);
          } else {
            ValePrologue_GaretCommentsOnBlockedPath(75, 715);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 67, 757);
          }
          GameFlag_Set(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH);
        }
      }
    }
    Event_End();
  }
}

/// Applies the cell behaviors left by the third rock (ACTOR_FIRST_ROCK +
/// 2). Called by ValePrologue_ThirdRockFalls and, once the rock has fallen,
/// by ValePrologue_Initialize.
static void ValePrologue_ApplyThirdRockCollision(void) {
  MapGrid_CopyCellAttributes(29, 22, 1, 1, 3, 42);
  MapGrid_CopyCellAttributes(29, 21, 1, 1, 2, 42);
  MapGrid_CopyCellAttributes(29, 21, 1, 1, 4, 42);
  MapGrid_CopyCellAttributes(23, 20, 3, 1, 2, 43);
}

/// In Vale on the stormy night, walking past: the fourth rock
/// (ACTOR_FIRST_ROCK + 3) falls down the path.
/// Reached by stepping on cell behavior 44; as
/// ValePrologue_FirstRockFalls does the first, with
/// FLAG_VALE_STORM_FOURTH_ROCK_FELL and FLAG_VALE_STORM_FOURTH_ROCK_PASSED.
static void ValePrologue_FourthRockFalls(void) {
  struct Actor *rock;
  struct Actor *leader;
  s32 targetHeight;

  if (Event_GetFlag(FLAG_VALE_STORM_FOURTH_ROCK_PASSED) != 0) {
  } else {
    Event_Start();
    if (Event_GetFlag(FLAG_VALE_STORM_FOURTH_ROCK_FELL) == 0) {
      rock = Actor_GetById(ACTOR_FIRST_ROCK + 3);
      // A rumble, then the rock rolls in from the east.
      MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      Sound_Play(SE_RUMBLE);
      wait(40);
      Sound_Play(SE_CRASH);
      Actor_SetPosition(ACTOR_FIRST_ROCK + 3, PIXELS(474), PIXELS(1147));
      rock->speedLimit = PIXELS(1);
      rock->acceleration = PIXELS(1);
      targetHeight = (rock->y.fixed + PIXELS(72));
      rock->y.fixed += PIXELS(72);
      rock->targetY = targetHeight;
      rock->bounce = 0x8000;
      Actor_SetPositionAndReset(ACTOR_FIRST_ROCK + 3, 432, 1147);
      Event_Wait(40);
      // It lands: the rumble on player 3 fades and the shaking dies down.
      Sound_Play(SOUND_FADE_OUT_ACTION_SE);
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
      MapGrid_WaitForShakeToSettle();
      Scene_PlayQueuedMusic();
      GameFlag_Set(FLAG_VALE_STORM_FOURTH_ROCK_FELL);
    }
    ValePrologue_ApplyFourthRockCollision();
    GameFlag_Set(FLAG_VALE_STORM_FOURTH_ROCK_PASSED);
    if (GameFlag_Get(FLAG_VALE_STORM_GARET_JOINED) != 0) {
      if (GameFlag_Get(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) == 0) {
        if (GameFlag_Get(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH) == 0) {
          // Garet comes out beside Isaac on whichever side of the rock he
          // stands.
          leader = Actor_GetById(ACTOR_PARTY_LEADER);
          if (leader->z.fixed <= PIXELS(1146) - 1) {
            ValePrologue_GaretCommentsOnBlockedPath(412, 1120);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 414, 1068);
          } else {
            ValePrologue_GaretCommentsOnBlockedPath(445, 1172);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 447, 1227);
          }
          GameFlag_Set(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH);
        }
      }
    }
    Event_End();
  }
}

/// Applies the cell behaviors left by the fourth rock (ACTOR_FIRST_ROCK +
/// 3). Called by ValePrologue_FourthRockFalls and, once the rock has fallen,
/// by ValePrologue_Initialize.
static void ValePrologue_ApplyFourthRockCollision(void) {
  MapGrid_CopyCellAttributes(29, 20, 1, 1, 26, 71);
  MapGrid_CopyCellAttributes(29, 20, 1, 1, 26, 70);
  MapGrid_CopyCellAttributes(29, 20, 1, 1, 27, 70);
  MapGrid_CopyCellAttributes(28, 21, 1, 1, 28, 71);
  MapGrid_CopyCellAttributes(28, 22, 1, 1, 27, 72);
}

/// Stops the leader at the door of VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_SOUTH
/// for the first conversation of the night. A stout man runs out of the door,
/// knocks Isaac back and runs off to the plaza. "Huh? Isaac?" Called by
/// ValePrologue_LeaveToCottageRoomsA. Sets
/// FLAG_VALE_STORM_BUMPED_INTO_VILLAGER.
static void ValePrologue_BumpIntoShovingMan(void) {
  s32 message;

  Event_Start();
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 262, 810);
  Actor_SetPosition(ACTOR_SHOVING_MAN, PIXELS(262), PIXELS(805));
  Actor_WalkTo(ACTOR_SHOVING_MAN, 262, 825);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 282, 855);
  Import_ActorSetAnimation(ACTOR_SHOVING_MAN, ANIM_STAND);
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_SHOVING_MAN, 0);
  Scene_PlayQueuedMusic();
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_ShowEmote(ACTOR_SHOVING_MAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  message = MsgValeHuh;
  Event_SetMessageId(message);
  Event_ShowMessage(ACTOR_SHOVING_MAN, 0);
  Event_Wait(20);
  Event_AskYesNo(ACTOR_SHOVING_MAN, 0);
  Actor_RunRepeatedMotion(ACTOR_SHOVING_MAN, 2);
  Event_SetMessageId((message + 4));
  FieldEvent_ShowMessageThenWait(ACTOR_SHOVING_MAN, 0, 20);
  Actor_RunScriptAndWait(ACTOR_SHOVING_MAN, gValePrologue_ShovingManScript);
  GameFlag_Set(FLAG_VALE_STORM_BUMPED_INTO_VILLAGER);
  Event_End();
}

/// In Vale on the stormy night, Isaac hears Garet struggling with his things.
/// "Ugh... Hrnghhh..."
/// Reached by stepping on cell behavior 30, until Garet has joined. Sets
/// FLAG_VALE_STORM_HEARD_GARET.
static void ValePrologue_HearGaretStruggling(void) {
  if (Event_GetFlag(FLAG_VALE_STORM_HEARD_GARET) == 0) {
    if (GameFlag_Get(FLAG_VALE_STORM_GARET_JOINED) == 0) {
      Event_Start();
      Event_SetMessageId(MsgValeUghHrnghhh);
      FieldEvent_ShowMessageThenWait(ACTOR_GARET_CHILD, 0, 20);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 382, 619);
      Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET_CHILD, 0);
      Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Event_Wait(30);
      Event_ShowMessage(ACTOR_GARET_CHILD, 0);
      GameFlag_Set(FLAG_VALE_STORM_HEARD_GARET);
      Event_End();
    }
  }
}

/// In Vale on the stormy night, walking away from him: Garet calls after
/// Isaac, asking whether he will just leave him there.
/// Reached by stepping on cell behavior 31 once Isaac has heard Garet and
/// before Garet joins; ValePrologue_GaretLeavesHisChest brings him along.
static void ValePrologue_GaretCallsAfterIsaac(void) {
  if (GameFlag_Get(FLAG_VALE_STORM_GARET_JOINED) == 0 &&
      GameFlag_Get(FLAG_VALE_STORM_HEARD_GARET) != 0) {
    Event_Start();
    Actor_RunRepeatedMotion(ACTOR_GARET_CHILD, 2);
    Event_Wait(20);
    Event_SetMessageId(MsgValeDontLeaveMeHere);
    ValePrologue_GaretLeavesHisChest();
    Event_End();
  }
}

/// In Vale on the stormy night, talking to Garet: whether to leave his things
/// behind, or, after the river, the Boulder.
/// Reached by talking to Garet: before he joins,
/// ValePrologue_GaretLeavesHisChest brings him along.
static void ValePrologue_TalkToGaret(void) {
  if (GameFlag_Get(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) != 0) {
    Event_Start();
    Actor_FaceActor(ACTOR_GARET_CHILD, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Event_SetMessageId(MsgValeBoulderNeedGet);
    Event_ShowMessage(ACTOR_GARET_CHILD, 0);
    Actor_FaceDirection(ACTOR_GARET_CHILD, FACING_NORTHEAST, 10);
    Event_End();
  } else {
    if (GameFlag_Get(FLAG_VALE_STORM_GARET_JOINED) == 0) {
      Event_Start();
      Event_SetMessageId(MsgValeWantDumpStuff);
      ValePrologue_GaretLeavesHisChest();
      Event_End();
    }
  }
}

/// Garet asks the question the caller chose and, on either answer, drops his
/// chest and joins the party. A yes has him agree to forget his things, a no
/// has him decide a rock would cost him chest and life alike. Called by
/// ValePrologue_GaretCallsAfterIsaac and ValePrologue_TalkToGaret; sets
/// FLAG_VALE_STORM_GARET_JOINED.
static void ValePrologue_GaretLeavesHisChest(void) {
  struct Actor *leader;
  s32 agreedToLeaveBelongings;

  FieldMessage_ShowAtActor(ACTOR_GARET_CHILD, 0);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET_CHILD, 0);
  agreedToLeaveBelongings = 0;
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_SetMessageId(MsgValeThinkForgetThings);
    agreedToLeaveBelongings = 1;
  } else {
    Event_SetMessageId(MsgValeRockHitsLose);
  }
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_GARET_CHILD, 0, 40);
  Actor_SetAttachedEffect(ACTOR_GARET_CHILD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  Import_ActorSetAnimation(ACTOR_GARETS_CHEST, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET_CHILD, ANIM_STAND);
  Event_Wait(40);
  Actor_SetAngleToward(ACTOR_GARET_CHILD, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_GARET_CHILD, ANIM_NOD);
  if (agreedToLeaveBelongings != 0) {
    Event_SetMessageId(MsgValeKnowRightOk);
  } else {
    Event_SetMessageId(MsgValeRightDitchStuff);
  }
  Event_ShowMessageAndWait(ACTOR_GARET_CHILD, 0);
  Import_ActorSetAnimation(ACTOR_GARET_CHILD, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_ResetAndSetPosition(ACTOR_GARET_CHILD, leader->x.part.pixel,
                              leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET_CHILD);
  Import_ActorSetPosition(ACTOR_GARET_CHILD, 0, 0);
  Event_PrepareActorAndApplyValue(1, 1);
  GameFlag_Set(FLAG_VALE_STORM_GARET_JOINED);
}

/// Unused: an earlier version of Garet joining Isaac on the stormy night,
/// which the game never plays.
///
/// Its row answers cell behavior 32, but every way there crosses the cells
/// where Isaac hears Garet struggling (behavior 30,
/// ValePrologue_HearGaretStruggling) and where Garet calls after him and
/// joins (behavior 31, ValePrologue_GaretCallsAfterIsaac), so Garet has
/// always joined by then and the event's first test,
/// FLAG_VALE_STORM_GARET_JOINED, ends it. Its lines appear nowhere else: Garet
/// runs up (gValePrologue_UnusedGaretRunsUpScript), heaves for breath,
/// `*Hah* *Hah*`, while gValePrologue_UnusedGaretPantingScript swells and
/// shrinks his sprite, and asks `Are you trapped, too?` before they leave
/// together.
static void ValePrologue_UnusedGaretJoinsIsaac(void) {
  struct Actor *actor;
  s32 message;

  if (Event_GetFlag(FLAG_VALE_STORM_GARET_JOINED) == 0) {
    Event_Start();
    Actor_SetAttachedEffect(ACTOR_GARET_CHILD,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
    message = MsgValeHey;
    Event_SetMessageId(message);
    Event_ShowMessage(ACTOR_GARET_CHILD, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    FieldCamera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
    FieldCamera_MoveTo(PIXELS(256), -1, PIXELS(588), 1);
    Actor_SetSpeed(ACTOR_GARET_CHILD, PIXELS(2), PIXELS(1));
    Actor_RunScriptAndWait(ACTOR_GARET_CHILD,
                           gValePrologue_UnusedGaretRunsUpScript);
    Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET_CHILD, 0);
    Event_Wait(30);
    Actor_SetScript(ACTOR_GARET_CHILD, gValePrologue_UnusedGaretPantingScript);
    Event_ShowMessage(ACTOR_GARET_CHILD, 0);
    actor = Actor_Get(ACTOR_GARET_CHILD);
    actor->scaleY = Q16_ONE;
    Actor_RunRepeatedMotion(ACTOR_GARET_CHILD, 1);
    Event_Wait(20);
    Event_AskYesNo(ACTOR_GARET_CHILD, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_GARET_CHILD, 1);
    Event_SetMessageId((message + 5));
    FieldEvent_ShowMessageThenWait(ACTOR_GARET_CHILD, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET_CHILD, ANIM_NOD);
    Event_ShowMessage(ACTOR_GARET_CHILD, 0);
    Actor_SetSpeed(ACTOR_GARET_CHILD, PIXELS(1), PIXELS(1) / 2);
    Import_ActorSetAnimation(ACTOR_GARET_CHILD, ANIM_WALK);
    actor = Actor_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Actor_SetDestination(ACTOR_GARET_CHILD, actor->x.part.pixel,
                           actor->z.part.pixel);
    }
    Actor_WaitForMoveById(ACTOR_GARET_CHILD);
    Actor_SetPosition(ACTOR_GARET_CHILD, 0, 0);
    Event_PrepareActorAndApplyValue(1, 1);
    Import_ActorSetAnimation(ACTOR_GARETS_CHEST, ANIM_NOD);
    GameFlag_Set(FLAG_VALE_STORM_GARET_JOINED);
    Event_End();
  }
}

/// In Vale on the stormy night, Garet turns Isaac back from the path north,
/// which leads to Mt. Aleph.
/// Reached by stepping on cell behavior 33, every time; Isaac is walked
/// back south.
static void ValePrologue_WarnAboutMtAleph(void) {
  struct Actor *leader;
  struct Actor *initialLeader;

  Event_Start();
  initialLeader = Actor_Get(ACTOR_PARTY_LEADER);
  if (initialLeader != NULL) {
    Actor_SetPosition(ACTOR_GARET_CHILD, initialLeader->x.fixed,
                      initialLeader->z.fixed);
  }
  Actor_SetSpeed(ACTOR_GARET_CHILD, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_GARET_CHILD, 281, 507);
  Actor_TurnToFaceEachOther(ACTOR_GARET_CHILD, ACTOR_PARTY_LEADER, 0);
  Event_Wait(30);
  Event_SetMessageId(MsgValeNorthLeadsToMtAleph);
  Event_ShowMessage(ACTOR_GARET_CHILD, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET_CHILD, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET_CHILD, FACING_SOUTH, 0);
  Event_ShowMessage(ACTOR_GARET_CHILD, 0);
  Import_ActorSetAnimation(ACTOR_GARET_CHILD, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_GARET_CHILD, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET_CHILD);
  Actor_SetPosition(ACTOR_GARET_CHILD, 0, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 256, 517);
  Event_End();
}

/// Brings Garet out to discuss a blocked path, then returns him to Isaac.
/// Called by the four rock events; x and y are the pixels where Garet stands.
static void ValePrologue_GaretCommentsOnBlockedPath(s32 x, s32 y) {
  struct Actor *leaderAtStart;
  s32 speed = PIXELS(1);
  s32 acceleration = (PIXELS(1) / 2);
  struct Actor *leaderAtEnd;

  leaderAtStart = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leaderAtStart != 0) {
    Import_ActorSetPosition(ACTOR_GARET_CHILD, leaderAtStart->x.fixed,
                            leaderAtStart->z.fixed);
  }
  Actor_SetSpeedParameters(ACTOR_GARET_CHILD, speed, acceleration);
  Actor_SetPositionAndReset(ACTOR_GARET_CHILD, x, y);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET_CHILD, 0);
  Event_Wait(20);
  FieldEffect_SetAttached(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Event_SetMessageId(MsgValeCantGetAroundThisRock);
  Event_ShowMessageAndWait(ACTOR_GARET_CHILD, 0);
  Actor_RunRepeatedMotion(ACTOR_GARET_CHILD, 2);
  Event_ShowMessageAndWait(ACTOR_GARET_CHILD, 0);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET_CHILD, ANIM_WALK);
  leaderAtEnd = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leaderAtEnd != 0) {
    Actor_ResetAndSetPosition(ACTOR_GARET_CHILD, leaderAtEnd->x.part.pixel,
                              leaderAtEnd->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET_CHILD);
  Import_ActorSetPosition(ACTOR_GARET_CHILD, 0, 0);
}

/// The Boulder slips toward the village and the elders tell the boys to run.
/// In Vale on the stormy night, the elders hold the Boulder with their
/// Psynergy as it slips.
/// "Ahhh! The Boulder! It's falling!"
/// Reached by stepping on cell behavior 34 until FLAG_VALE_BOULDER_FELL is
/// set, which this sets. The elders and their two stand-ins
/// (gValePrologue_BoulderWatchers, ACTOR_ELDER_EAST_STAND_IN to
/// ACTOR_ELDER_WEST) are placed for the event and removed after it.
static void ValePrologue_WatchBoulderFall(void) {
  u32 i;
  struct Actor *movingActor;
  s32 *shakeShift;
  struct Actor *actor;
  TaskFunc updateEffects;
  TaskFunc cycleBoulderPalette;
  s32 boulderHeight;

  if (GameFlag_Get(FLAG_VALE_BOULDER_FELL) == 0) {
    Event_Start();
    Event_CallWithLastActiveActorId(gValePrologue_BoulderWatchers);
    Scene_StartEventPsynergy();
    wait(1);
    Event_PlaySound(SE_RUMBLE);
    MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Event_Wait(30);
    MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
    Event_PlaySound(SE_CRASH);
    Event_Wait(30);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetPosition(ACTOR_GARET_CHILD, actor->x.fixed, actor->z.fixed);
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
    Actor_SetSpeed(ACTOR_GARET_CHILD, PIXELS(2), PIXELS(1));
    Actor_SetScript(ACTOR_PARTY_LEADER, gValePrologue_BoulderFallLeaderPath);
    Actor_RunScriptAndWait(ACTOR_GARET_CHILD,
                           gValePrologue_BoulderFallGaretPath);
    Actor_RefreshSelectorById(ACTOR_PARTY_LEADER);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_GARET_CHILD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
    Event_PlaySound(SE_CRASH);
    Event_Wait(40);
    MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
    Event_PlaySound(SE_CRASH);
    Event_Wait(20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_GARET_CHILD,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    // Show the four elders holding back the Boulder.
    Import_ActorSetAnimation(ACTOR_ELDER_EAST, ANIM_BEARDED_ELDER_HOLD_BACK);
    Import_ActorSetAnimation(ACTOR_ELDER_WEST, ANIM_BEARDED_ELDER_HOLD_BACK);
    Import_ActorSetAnimation(ACTOR_ELDER_FAR_EAST, ANIM_YOUNG_ELDER_HOLD_BACK);
    Import_ActorSetAnimation(ACTOR_ELDER_FAR_WEST, ANIM_YOUNG_ELDER_HOLD_BACK);
    Actor_Get(ACTOR_ELDER_FAR_EAST)->scaleX = -Q16_ONE;
    Actor_SetSpritePriority(ACTOR_ELDER_EAST, 2);
    Actor_SetSpritePriority(ACTOR_ELDER_WEST, 2);
    Actor_SetSpritePriority(ACTOR_ELDER_FAR_EAST, 3);
    Actor_SetSpritePriority(ACTOR_ELDER_FAR_WEST, 3);
    Event_SetMessageId(MsgValeTheBoulderIsFalling);
    FieldEvent_ShowMessageThenWait(ACTOR_ELDER_EAST_STAND_IN, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET_CHILD, FACING_NORTH, 20);
    FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
    FieldCamera_MoveTo(PIXELS(112), -1, PIXELS(331), 1);
    FieldCamera_WaitForArrival();
    for (i = 0; i < 40; i++) {
      ValePrologue_FlashPaletteAndSpawnEffects(Actor_Get(ACTOR_ELDER_EAST));
      ValePrologue_FlashPaletteAndSpawnEffects(Actor_Get(ACTOR_ELDER_WEST));
      ValePrologue_FlashPaletteAndSpawnEffects(Actor_Get(ACTOR_ELDER_FAR_EAST));
      ValePrologue_FlashPaletteAndSpawnEffects(Actor_Get(ACTOR_ELDER_FAR_WEST));
      wait(1);
    }
    // Run the palette effects while the boulder rocks back and forth.
    shakeShift = &sShakeShift;
    updateEffects = ValePrologue_UpdateBoulderMapEffects;
    sShakeDone = 0;
    *shakeShift = 0;
    Task_AddOrUpdateFrame(updateEffects, TASK_PASS_FRAME);
    cycleBoulderPalette = ValePrologue_CycleBoulderPalette;
    Task_AddFrame(cycleBoulderPalette, TASK_PASS_FRAME);
    Event_Wait(40);
    *shakeShift = 1;
    Event_Wait(30);
    Actor_SetPosition(ACTOR_BOULDER, PIXELS(114), PIXELS(290));
    movingActor = Actor_Get(ACTOR_BOULDER);
    boulderHeight = movingActor->y.fixed + PIXELS(64);
    movingActor->y.fixed = boulderHeight;
    movingActor->targetY = boulderHeight;
    Actor_SetSpeed(ACTOR_BOULDER, PIXELS(4) / 5, PIXELS(2) / 5);
    Event_PlaySound(SE_CRASH);
    Actor_MoveToAndWait(ACTOR_BOULDER, 114, 333);
    Import_ActorSetAnimation(ACTOR_BOULDER, ANIM_WALK);
    MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Event_PlaySound(SE_CRASH);
    *shakeShift = 0;
    Actor_SetSpeed(ACTOR_BOULDER, PIXELS(2) / 5, PIXELS(1) / 5);
    Actor_MoveToAndWait(ACTOR_BOULDER, 114, 300);
    Import_ActorSetAnimation(ACTOR_BOULDER, ANIM_WALK);
    MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
    Event_PlaySound(SE_CRASH);
    *shakeShift = 2;
    Actor_SetSpeed(ACTOR_BOULDER, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_MoveToAndWait(ACTOR_BOULDER, 114, 333);
    Import_ActorSetAnimation(ACTOR_BOULDER, ANIM_WALK);
    MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Event_PlaySound(SE_CRASH);
    *shakeShift = 0;
    Actor_SetSpeed(ACTOR_BOULDER, PIXELS(2) / 5, PIXELS(1) / 5);
    Actor_MoveToAndWait(ACTOR_BOULDER, 114, 300);
    Import_ActorSetAnimation(ACTOR_BOULDER, ANIM_WALK);
    MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
    Event_PlaySound(SE_CRASH);
    *shakeShift = 2;
    Actor_SetSpeed(ACTOR_BOULDER, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_MoveToAndWait(ACTOR_BOULDER, 114, 333);
    Import_ActorSetAnimation(ACTOR_BOULDER, ANIM_WALK);
    MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Event_PlaySound(SE_CRASH);
    *shakeShift = 1;
    Event_Wait(20);
    Actor_SetAttachedEffect(ACTOR_ELDER_EAST,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_RunRepeatedMotion(ACTOR_ELDER_EAST, 2);
    Event_ShowMessage(ACTOR_ELDER_WEST_STAND_IN, 0);
    Actor_ShowEmote(ACTOR_ELDER_WEST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_RunRepeatedMotion(ACTOR_ELDER_WEST, 2);
    FieldEvent_ShowMessageThenWait(ACTOR_ELDER_EAST_STAND_IN, 0, 40);
    Actor_SetAttachedEffect(ACTOR_ELDER_FAR_EAST,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_RunRepeatedMotion(ACTOR_ELDER_FAR_EAST, 2);
    Event_ShowMessage(ACTOR_ELDER_FAR_EAST, 0);
    // The westernmost elder stops straining, looks about and calls to the
    // boys, and the camera returns to Isaac.
    sShakeDone = 1;
    Import_ActorSetAnimation(ACTOR_ELDER_FAR_WEST, ANIM_STAND);
    wait(1);
    Actor_SetSpriteColorById(ACTOR_ELDER_FAR_WEST, SPRITE_COLOR_NORMAL);
    Actor_ShowEmote(ACTOR_ELDER_FAR_WEST, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
    Actor_FaceDirection(ACTOR_ELDER_FAR_WEST, FACING_WEST, 40);
    Actor_FaceDirection(ACTOR_ELDER_FAR_WEST, FACING_EAST, 20);
    Actor_FaceDirection(ACTOR_ELDER_FAR_WEST, FACING_WEST, 20);
    Actor_FaceDirection(ACTOR_ELDER_FAR_WEST, FACING_SOUTH, 40);
    Actor_ShowEmote(ACTOR_ELDER_FAR_WEST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    0);
    Actor_RunRepeatedMotion(ACTOR_ELDER_FAR_WEST, 2);
    Actor_Jump(ACTOR_ELDER_FAR_WEST, 4, 40);
    Import_ActorSetAnimation(ACTOR_ELDER_FAR_WEST, ANIM_YOUNG_ELDER_CALL_OUT);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(ACTOR_ELDER_FAR_WEST, 0, 20);
    Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    FieldCamera_SetSpeed(PIXELS(6), PIXELS(3) / 4);
    FieldCamera_MoveTo(PIXELS(84), -1, PIXELS(564), 1);
    FieldCamera_WaitForArrival();
    Scene_PlayQueuedMusic();
    Actor_FaceActor(ACTOR_GARET_CHILD, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Actor_SetAttachedEffect(ACTOR_GARET_CHILD,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(30);
    Task_Remove(updateEffects);
    Task_Remove(cycleBoulderPalette);
    Event_ShowMessage(ACTOR_GARET_CHILD, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET_CHILD, 0);
    Event_Wait(20);
    Scene_EndEventPsynergy();
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET_CHILD, ANIM_NOD);
    Event_Wait(20);
    Import_ActorSetAnimation(ACTOR_GARET_CHILD, ANIM_WALK);
    movingActor = Actor_GetById(ACTOR_PARTY_LEADER);
    if (movingActor != 0) {
      Actor_SetDestination(ACTOR_GARET_CHILD, movingActor->x.part.pixel,
                           movingActor->z.part.pixel);
    }
    Actor_WaitForMoveById(ACTOR_GARET_CHILD);
    Actor_SetPosition(ACTOR_GARET_CHILD, 0, 0);
    Actor_ReleaseById(ACTOR_ELDER_WEST_STAND_IN);
    Actor_ReleaseById(ACTOR_ELDER_EAST_STAND_IN);
    Actor_ReleaseById(ACTOR_ELDER_FAR_EAST);
    Actor_ReleaseById(ACTOR_ELDER_FAR_WEST);
    Actor_ReleaseById(ACTOR_ELDER_EAST);
    Actor_ReleaseById(ACTOR_ELDER_WEST);
    GameFlag_Set(FLAG_VALE_BOULDER_FELL);
    Event_End();
  }
}

/// Sets the animation parameter of each entry of a one-sprite actor:
/// mode - 1, or for mode 0 a value from gValePrologue_FrameModes by the
/// frame count. Only ValePrologue_FlashBoulderMapAnimationEntries calls it.
static void ValePrologue_SetActorFrameMode(struct Actor *actor, s32 mode) {
  if ((actor->spriteStorageKind & 0xf) == 1) {
    struct Sprite *animation = actor->sprite;
    s32 animationParameter = mode - 1;
    u8 count;
    if (mode == 0) {
      animationParameter =
          gValePrologue_FrameModes[(gFrameCount >> 1) &
                                   (actor->spriteStorageKind & 0xf)];
    }
    count = animation->count;
    if (count != 0) {
      struct SpriteAnimation **entries = animation->entries;
      s32 remainingEntries = count;
      do {
        struct SpriteAnimation *entry = *entries++;
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
UNUSED static void ValePrologue_FlashBoulderMapAnimationEntries(void) {
  if (((gFrameCount >> sShakeShift) & 3) != 0) {
    ValePrologue_SetActorFrameMode(Actor_GetById(ACTOR_ELDER_EAST), 1);
    ValePrologue_SetActorFrameMode(Actor_GetById(ACTOR_ELDER_WEST), 1);
    ValePrologue_SetActorFrameMode(Actor_GetById(ACTOR_ELDER_FAR_EAST), 1);
    ValePrologue_SetActorFrameMode(Actor_GetById(ACTOR_ELDER_FAR_WEST), 1);
  } else {
    ValePrologue_SetActorFrameMode(Actor_GetById(ACTOR_ELDER_EAST), 8);
    ValePrologue_SetActorFrameMode(Actor_GetById(ACTOR_ELDER_WEST), 8);
    ValePrologue_SetActorFrameMode(Actor_GetById(ACTOR_ELDER_FAR_EAST), 8);
    ValePrologue_SetActorFrameMode(Actor_GetById(ACTOR_ELDER_FAR_WEST), 8);
  }
}

/// Jenna cries out for Felix as her family tries to save him at the river. In
/// Vale on the stormy night, her brother Felix has fallen in and clings on as
/// Kyle and her parents try to save him. "Oh no! My brother!" The rope will not
/// reach and no one has Psynergy left, so they send for help. Reached by
/// stepping on cell behavior 35 until FLAG_VALE_STORM_SAW_FELIX_HANGING_ON
/// is set, which this sets after ValePrologue_DoraGoesForHelp.
static void ValePrologue_WatchAttemptToSaveFelix(void) {
  const s32 *pulseScript;

  if (Event_GetFlag(FLAG_VALE_STORM_SAW_FELIX_HANGING_ON) != 0) {
    return;
  }
  Event_Start();
  Actor_SetPosition(ACTOR_KYLE, PIXELS(192), PIXELS(1214));
  Actor_FaceDirection(ACTOR_KYLE, FACING_SOUTHEAST, 0);
  Import_ActorSetAnimation(ACTOR_KYLE, ANIM_KYLE_REACH_OUT);
  {
    struct Actor *actor;
    s32 pulseDelay;
    actor = Actor_GetById(ACTOR_KYLE);
    pulseDelay = Math_RemainderUnsigned(Import_RandomNext(), 90) + 60;
    pulseScript = gValePrologue_SquashAndStretchScript;
    actor->timer = pulseDelay;
    Actor_SetScript(ACTOR_KYLE, pulseScript);
  }
  Actor_SetPosition(ACTOR_DORA, PIXELS(192), PIXELS(1189));
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST, 0);
  Actor_SetPosition(ACTOR_JENNAS_FATHER, PIXELS(227), PIXELS(1214));
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_SOUTH, 0);
  Import_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_PARENT_REACH_OUT);
  {
    struct Actor *actor;
    s32 pulseDelay;
    actor = Actor_GetById(ACTOR_JENNAS_FATHER);
    pulseDelay = Math_RemainderUnsigned(Import_RandomNext(), 90) + 60;
    actor->timer = pulseDelay;
    Actor_SetScript(ACTOR_JENNAS_FATHER, pulseScript);
  }
  Actor_SetPosition(ACTOR_JENNAS_MOTHER, PIXELS(250), PIXELS(1214));
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_SOUTH, 0);
  Import_ActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIM_PARENT_REACH_OUT);
  {
    struct Actor *actor;
    s32 pulseDelay;
    actor = Actor_GetById(ACTOR_JENNAS_MOTHER);
    pulseDelay = Math_RemainderUnsigned(Import_RandomNext(), 90) + 60;
    actor->timer = pulseDelay;
    Actor_SetScript(ACTOR_JENNAS_MOTHER, pulseScript);
  }
  Actor_SetPosition(ACTOR_JENNA_CHILD, PIXELS(227), PIXELS(1189));
  Actor_FaceDirection(ACTOR_JENNA_CHILD, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetPosition(ACTOR_FELIX_CHILD, PIXELS(243), PIXELS(1277));
  Actor_FaceDirection(ACTOR_FELIX_CHILD, FACING_NORTH, 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FELIX_CHILD), 0);
  wait(3);
  Event_SetMessageId(MsgValeNoBrother);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA_CHILD, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 150, 1094);
  {
    struct Actor *leader;
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Import_ActorSetPosition(ACTOR_GARET_CHILD, leader->x.fixed,
                              leader->z.fixed);
    }
  }
  Actor_WalkToAndWait(ACTOR_GARET_CHILD, 132, 1094);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET_CHILD, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET_CHILD, FACING_SOUTH, 20);
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  FieldCamera_MoveTo(PIXELS(216), -1, PIXELS(1232), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_KYLE, 2);
  Event_ShowMessageThenWait(ACTOR_KYLE, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_FELIX_CHILD, 3);
  Actor_ArmCallback(ACTOR_DORA, FACING_EAST, 10);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 10);
  FieldCamera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
  FieldCamera_MoveTo(PIXELS(232), -1, PIXELS(1253), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Sound_Play(SE_HEAVY_IMPACT);
  Actor_Jump(ACTOR_FELIX_CHILD, 4, 0);
  Import_ActorSetAnimation(ACTOR_FELIX_CHILD, ANIM_FELIX_UNDER_WATER);
  Event_Wait(10);
  Import_ActorSetPosition(ACTOR_FELIX_CHILD, 0, 0);
  Event_Wait(60);
  Scene_PlayQueuedMusic();
  Import_ActorSetAnimation(ACTOR_KYLE, ANIM_STAND);
  {
    struct Actor *actor;
    actor = Actor_GetById(ACTOR_KYLE);
    actor->scaleX = Q16_ONE;
    actor->scaleY = Q16_ONE;
  }
  Import_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_STAND);
  {
    struct Actor *actor;
    actor = Actor_GetById(ACTOR_JENNAS_FATHER);
    actor->scaleX = Q16_ONE;
    actor->scaleY = Q16_ONE;
  }
  Import_ActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIM_STAND);
  {
    struct Actor *actor;
    actor = Actor_GetById(ACTOR_JENNAS_MOTHER);
    actor->scaleX = Q16_ONE;
    actor->scaleY = Q16_ONE;
  }
  Actor_StartRepeatedMotion(ACTOR_KYLE, 2);
  Actor_StartRepeatedMotion(ACTOR_DORA, 2);
  Actor_StartRepeatedMotion(ACTOR_JENNAS_FATHER, 2);
  Actor_StartRepeatedMotion(ACTOR_JENNAS_MOTHER, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA_CHILD, 2);
  FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  FieldCamera_MoveTo(PIXELS(216), -1, PIXELS(1232), 1);
  FieldCamera_WaitForArrival();
  Actor_SetAttachedEffect(ACTOR_JENNA_CHILD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEffect_SetAttached(ACTOR_DORA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_JENNA_CHILD, 2);
  Actor_StartRepeatedMotion(ACTOR_JENNA_CHILD, 3);
  Event_ShowMessageAndWait(ACTOR_JENNA_CHILD, 0);
  Actor_Jump(ACTOR_JENNAS_MOTHER, 2, 0);
  Actor_SetDestination(ACTOR_JENNAS_MOTHER, 234, 1205);
  Actor_Jump(ACTOR_JENNA_CHILD, 2, 0);
  Actor_SetDestination(ACTOR_JENNA_CHILD, 227, 1201);
  Event_Wait(90);
  FieldCamera_MoveTo(PIXELS(232), -1, PIXELS(1253), 1);
  FieldCamera_WaitForArrival();
  Import_ActorSetPosition(ACTOR_FELIX_CHILD, PIXELS(243), PIXELS(1277));
  wait(1);
  Sound_Play(SE_DRIP);
  {
    struct Actor *actor;
    actor = Actor_GetById(ACTOR_FELIX_CHILD);
    actor->velocityY = PIXELS(2);
  }
  Event_Wait(6);
  Import_ActorSetAnimation(ACTOR_FELIX_CHILD, ANIM_FELIX_CLING);
  Event_Wait(20);
  Scene_PlayQueuedMusic();
  Event_Wait(20);
  FieldCamera_SetSpeedParameters(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveWithinMapBounds(PIXELS(216), -1, PIXELS(1232), 1);
  FieldCamera_WaitForArrival();
  Actor_RunRepeatedMotion(ACTOR_JENNAS_FATHER, 2);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_JENNAS_FATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_TurnToFaceEachOther(ACTOR_JENNAS_FATHER, ACTOR_KYLE, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_KYLE, 2);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KYLE, 0);
  {
    struct Actor *actor;
    actor = Actor_GetById(ACTOR_JENNAS_MOTHER);
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  }
  {
    struct Actor *actor;
    actor = Actor_GetById(ACTOR_JENNA_CHILD);
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  }
  Actor_SetSpeed(ACTOR_JENNAS_MOTHER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(ACTOR_JENNA_CHILD, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetDestination(ACTOR_JENNAS_MOTHER, 247, 1210);
  Actor_MoveToAndWait(ACTOR_JENNA_CHILD, 227, 1189);
  {
    struct Actor *actor;
    actor = Actor_GetById(ACTOR_JENNAS_MOTHER);
    actor->steeringFlags |= ACTOR_FACES_MOTION;
  }
  Actor_GetById(ACTOR_JENNA_CHILD)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_ArmCallback(ACTOR_JENNA_CHILD, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_WEST, 10);
  Import_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNAS_FATHER, 0, 10);
  Actor_FaceDirection(ACTOR_KYLE, FACING_NORTH, 20);
  Actor_ArmCallback(ACTOR_KYLE, FACING_EAST, 10);
  Import_ActorSetAnimation(ACTOR_KYLE, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_KYLE, 0, 10);
  Actor_ShowEmote(ACTOR_JENNAS_FATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_KYLE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 20);
  Actor_ArmCallback(ACTOR_DORA, FACING_EAST, 30);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 10);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_KYLE, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_WEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA_CHILD, FACING_WEST, 10);
  Actor_RunRepeatedMotion(ACTOR_KYLE, 1);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_KYLE, 0, 10);
  Import_ActorSetAnimation(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_ShowEmote(ACTOR_KYLE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_JENNAS_FATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_JENNAS_MOTHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_JENNA_CHILD, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_ArmCallback(ACTOR_DORA, FACING_EAST, 10);
  Actor_TurnToFaceEachOther(ACTOR_JENNAS_FATHER, ACTOR_JENNAS_MOTHER, 0);
  Event_Wait(20);
  Actor_ArmCallback(ACTOR_DORA, FACING_EAST, 0);
  Actor_ArmCallback(ACTOR_KYLE, FACING_EAST, 10);
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_WEST, 10);
  Import_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNAS_MOTHER, ANIM_NOD);
  Actor_TurnToFaceEachOther(ACTOR_KYLE, ACTOR_DORA, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_KYLE, 1);
  Event_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_KYLE, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_NORTH + FACING_STEP, 10);
  Actor_StartRepeatedMotion(ACTOR_JENNAS_FATHER, 1);
  Event_ShowMessageThenWait(ACTOR_JENNAS_FATHER, 0, 10);
  Actor_ArmCallback(ACTOR_KYLE, FACING_EAST, 0);
  Actor_ArmCallback(ACTOR_DORA, FACING_EAST, 0);
  Actor_RunRepeatedMotion(ACTOR_JENNA_CHILD, 1);
  Actor_FaceDirection(ACTOR_JENNA_CHILD, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNAS_MOTHER, ANIM_NOD);
  Event_ShowMessageThenWait(ACTOR_JENNAS_MOTHER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA_CHILD, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA_CHILD, FACING_WEST, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA_CHILD, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_DORA, 1);
  Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  ValePrologue_DoraGoesForHelp();
  GameFlag_Set(FLAG_VALE_STORM_SAW_FELIX_HANGING_ON);
  Event_End();
}

/// Dora and Jenna run through the house by the river; Dora goes north for help
/// and sends Jenna to the plaza, then asks Isaac and Garet to help: a yes
/// sends them after Jenna, a no to the plaza all the same. Called by
/// ValePrologue_WatchAttemptToSaveFelix.
static void ValePrologue_DoraGoesForHelp(void) {
  struct Actor *villager;
  s32 random;
  const s32 *script;
  s32 northMessage;
  s32 helpMessage;
  struct Actor *leader;

  Actor_FaceDirection(ACTOR_JENNA_CHILD, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_KYLE, FACING_NORTH + FACING_STEP, 20);
  Import_ActorSetAnimation(ACTOR_JENNA_CHILD, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNAS_MOTHER, ANIM_NOD);
  Event_Wait(20);
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(134), -1, PIXELS(1195), 1);
  Actor_SetSpeed(ACTOR_JENNA_CHILD, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetScript(ACTOR_JENNA_CHILD, gValePrologue_JennaRunsIntoRiversideHouse);
  Actor_RunScriptAndWait(ACTOR_DORA, gValePrologue_DoraRunsIntoRiversideHouse);
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gValePrologue_GrandparentsDoorOpens, 38, 72);
  Event_Wait(10);
  Actor_WalkToAndWait(ACTOR_DORA, 149, 1175);
  Actor_SetPosition(ACTOR_DORA, 0, 0);
  Actor_WalkToAndWait(ACTOR_JENNAS_MOTHER, 250, 1214);
  Scene_PlayQueuedMusic();
  Actor_FaceDirection(ACTOR_KYLE, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_SOUTHEAST + FACING_STEP, 0);
  Import_ActorSetAnimation(ACTOR_KYLE, ANIM_KYLE_REACH_OUT);
  Import_ActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_PARENT_REACH_OUT);
  Import_ActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIM_PARENT_REACH_OUT);
  villager = Actor_Get(ACTOR_KYLE);
  random = Import_RandomNext();
  villager->timer = (Math_RemainderUnsigned(random, 90) + 60);
  villager = Actor_Get(ACTOR_JENNAS_FATHER);
  random = Import_RandomNext();
  villager->timer = (Math_RemainderUnsigned(random, 90) + 60);
  villager = Actor_Get(ACTOR_JENNAS_MOTHER);
  random = Import_RandomNext();
  villager->timer = (Math_RemainderUnsigned(random, 90) + 60);
  script = gValePrologue_SquashAndStretchScript;
  Actor_SetScript(ACTOR_KYLE, script);
  Actor_SetScript(ACTOR_JENNAS_FATHER, script);
  Actor_SetScript(ACTOR_JENNAS_MOTHER, script);
  Actor_RefreshSelectorById(ACTOR_JENNA_CHILD);
  Event_Wait(10);
  Event_PlaySound(SE_DOOR_CLOSE);
  MapGrid_AnimateCells(gValePrologue_GrandparentsDoorCloses, 38, 72);
  Event_Wait(30);
  Scene_PlayQueuedMusic();
  FieldCamera_MoveTo(PIXELS(112), -1, PIXELS(1225), 1);
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gValePrologue_IsaacsDoorOpens, 35, 73);
  Event_Wait(20);
  Scene_PlayQueuedMusic();
  Actor_SetScript(ACTOR_DORA, gValePrologue_DoraComesOutOfRiversideHouse);
  Event_Wait(20);
  Actor_SetScript(ACTOR_JENNA_CHILD,
                  gValePrologue_JennaComesOutOfRiversideHouse);
  Event_Wait(40);
  Event_PlaySound(SE_DOOR_CLOSE);
  MapGrid_AnimateCells(gValePrologue_IsaacsDoorCloses, 35, 73);
  Actor_RefreshSelectorById(ACTOR_JENNA_CHILD);
  Scene_PlayQueuedMusic();
  Event_Wait(40);
  northMessage = MsgValeGoLookNorth;
  Event_SetMessageId(northMessage);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA_CHILD, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA_CHILD, 0,
                                 40);
  Import_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA_CHILD, ANIM_NOD);
  Event_Wait(30);
  Actor_SetScript(ACTOR_DORA, gValePrologue_DoraHeadsNorth);
  Actor_SetScript(ACTOR_JENNA_CHILD, gValePrologue_JennaHeadsForPlaza);
  Event_Wait(40);
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  FieldCamera_MoveTo(PIXELS(105), -1, PIXELS(1086), 1);
  Actor_RefreshSelectorById(ACTOR_DORA);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET_CHILD, FACING_WEST, 10);
  Actor_WalkToAndWait(ACTOR_DORA, 105, 1086);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
  Actor_FaceDirection(ACTOR_GARET_CHILD, FACING_EAST, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Event_SetMessageId((northMessage + 4));
  } else {
    Actor_RunRepeatedMotion(ACTOR_DORA, 2);
    Event_SetMessageId((northMessage + 5));
  }
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
  Actor_FaceDirection(ACTOR_GARET_CHILD, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  helpMessage = MsgValeDontSupposeTwo;
  Event_SetMessageId(helpMessage);
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Event_SetMessageId((helpMessage + 1));
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0, 30);
    Actor_FaceDirection(ACTOR_GARET_CHILD, FACING_WEST, 20);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_GARET_CHILD, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Event_Wait(40);
  } else {
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 90);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Import_ActorSetAnimation(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Event_SetMessageId((helpMessage + 2));
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
  }
  Actor_SetScript(ACTOR_DORA, gValePrologue_DoraLeavesForHelp);
  Event_Wait(90);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET_CHILD, 0);
  Event_Wait(40);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET_CHILD, ANIM_NOD);
  Event_Wait(20);
  Import_ActorSetAnimation(ACTOR_GARET_CHILD, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_GARET_CHILD, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET_CHILD);
  Actor_SetPosition(ACTOR_GARET_CHILD, 0, 0);
}

/// Kyle tells Isaac and Garet to find help for Felix. In Vale on the stormy
/// night, at the river, he tells them they don't have enough Psynergy to save
/// Felix.
/// Reached by talking to Kyle; the first time he greets Isaac and sets
/// FLAG_VALE_STORM_KYLE_GREETED_ISAAC.
static void ValePrologue_TalkAboutSavingFelix(void) {
  s32 facing = FACING_SOUTHEAST;
  const s32 *pulseScript;

  Event_Start();
  Import_ActorSetAnimation(ACTOR_KYLE, ANIM_STAND);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(ACTOR_KYLE, ACTOR_PARTY_LEADER, 20);
  if (GameFlag_Get(FLAG_VALE_STORM_KYLE_GREETED_ISAAC) != 0) {
    Event_SetMessageId(MsgValeTwoDontEnough);
    Event_ShowMessageThenWait(ACTOR_KYLE, 0, 10);
  } else {
    Event_SetMessageId(MsgValeOh);
    Actor_StartRepeatedMotion(ACTOR_KYLE, 1);
    Event_ShowMessageThenWait(ACTOR_KYLE, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_KYLE, 2);
    Event_ShowMessageThenWait(ACTOR_KYLE, 0, 10);
  }
  Actor_ArmCallback(ACTOR_KYLE, facing, 20);
  Import_ActorSetAnimation(ACTOR_KYLE, ANIM_KYLE_REACH_OUT);
  Event_Wait(10);
  {
    struct Actor *villager;
    s32 pulseDelay;
    villager = Actor_GetById(ACTOR_KYLE);
    pulseDelay = Math_RemainderUnsigned(Import_RandomNext(), 90) + 60;
    pulseScript = gValePrologue_SquashAndStretchScript;
    villager->timer = pulseDelay;
    Actor_SetScript(ACTOR_KYLE, pulseScript);
  }
  Event_Wait(20);
  GameFlag_Set(FLAG_VALE_STORM_KYLE_GREETED_ISAAC);
  Event_End();
}

/// The Boulder sweeps Felix, his parents and Kyle away. In Vale on the stormy
/// night, Jenna comes back to the river with Isaac, Garet and the man whose
/// Psynergy is restored ("Mom! Dad! We're back!"), and as Dora calls everyone
/// down to save Felix the Boulder comes down the river and sweeps away Felix,
/// his parents and Kyle. Reached by stepping on cell behavior 36 once
/// FLAG_VALE_STORM_SENT_TO_AID_ELDERS is set (vale_plaza.c), until
/// FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY is set, which this sets.
static void ValePrologue_BoulderSweepsThemAway(void) {
  struct Actor *leader;
  struct Actor *actor;
  s32 automaticPriority;
  struct Sprite *sprite;

  if (GameFlag_Get(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) == 0) {
  } else {
    if (Event_GetFlag(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) != 0) {
    } else {
      Event_Start();
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      Actor_SetSpeed(ACTOR_GARET_CHILD, PIXELS(1), PIXELS(1) / 2);
      Actor_SetSpeed(ACTOR_JENNA_CHILD, PIXELS(1), PIXELS(1) / 2);
      Actor_SetSpeed(ACTOR_EXHAUSTED_STRONGMAN, PIXELS(1), PIXELS(1) / 2);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 217, 1367);
      actor = Actor_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Import_ActorSetPosition(ACTOR_GARET_CHILD, actor->x.fixed,
                                actor->z.fixed);
      }
      Actor_WalkToAndWait(ACTOR_GARET_CHILD, 235, 1367);
      Actor_FaceDirection(ACTOR_GARET_CHILD, FACING_NORTHWEST + FACING_STEP, 0);
      actor = Actor_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Import_ActorSetPosition(ACTOR_JENNA_CHILD, actor->x.fixed,
                                actor->z.fixed);
      }
      Actor_WalkToAndWait(ACTOR_JENNA_CHILD, 199, 1367);
      Actor_FaceDirection(ACTOR_JENNA_CHILD, FACING_NORTH + FACING_STEP, 0);
      Actor_SetPosition(ACTOR_JENNAS_MOTHER, PIXELS(247), PIXELS(1210));
      Actor_ArmCallback(ACTOR_JENNAS_MOTHER, FACING_SOUTHWEST, 0);
      actor = Actor_GetById(ACTOR_EXHAUSTED_STRONGMAN);
      sprite = actor->sprite;
      actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      sprite->image.priority = 1;
      leader = Actor_GetById(ACTOR_PARTY_LEADER);
      sprite = leader->sprite;
      leader->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      sprite->image.priority = 2;
      actor = Actor_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Import_ActorSetPosition(ACTOR_EXHAUSTED_STRONGMAN, actor->x.fixed,
                                actor->z.fixed);
      }
      Actor_WalkToAndWait(ACTOR_EXHAUSTED_STRONGMAN, 221, 1385);
      Actor_FaceDirection(ACTOR_EXHAUSTED_STRONGMAN,
                          FACING_NORTHWEST + FACING_STEP, 60);
      Actor_RunRepeatedMotion(ACTOR_JENNA_CHILD, 2);
      Event_SetMessageId(MsgValeMomDadBack);
      Event_ShowMessageThenWait(ACTOR_JENNA_CHILD, 0, 40);
      Actor_SetPosition(ACTOR_DORA, PIXELS(101), PIXELS(1197));
      Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST, 0);
      Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_DORA, 0, 10);
      Actor_FaceDirection(ACTOR_JENNA_CHILD, FACING_NORTHWEST, 0);
      FieldCamera_SetSpeedParameters(PIXELS(6) / 5, PIXELS(3) / 20);
      FieldCamera_MoveTo(PIXELS(101), -1, PIXELS(1197), 1);
      Actor_SetSpeed(ACTOR_DORA, PIXELS(7) / 5, PIXELS(7) / 10);
      Actor_SetScript(ACTOR_DORA, gValePrologue_DoraRunsToRiver);
      Event_Wait(60);
      FieldCamera_SetSpeedParameters(PIXELS(3) / 5, PIXELS(3) / 40);
      FieldCamera_MoveTo(PIXELS(187), -1, PIXELS(1328), 1);
      FieldCamera_WaitForArrival();
      Event_Wait(40);
      Actor_RunRepeatedMotion(ACTOR_JENNA_CHILD, 2);
      Event_ShowMessageThenWait(ACTOR_JENNA_CHILD, 0, 20);
      Actor_RunRepeatedMotion(ACTOR_DORA, 2);
      Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_DORA, 0, 20);
      FieldCamera_SetSpeedParameters(PIXELS(2), PIXELS(1) / 4);
      FieldCamera_MoveTo(PIXELS(221), -1, PIXELS(1385), 1);
      Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_EXHAUSTED_STRONGMAN, 0);
      Actor_SetAngleToward(ACTOR_GARET_CHILD, ACTOR_EXHAUSTED_STRONGMAN, 0);
      Actor_FaceDirection(ACTOR_JENNA_CHILD, FACING_SOUTHEAST + FACING_STEP,
                          80);
      FieldCamera_MoveWithinMapBounds(PIXELS(182), -1, PIXELS(1360), 1);
      Actor_WalkToAndWait(ACTOR_EXHAUSTED_STRONGMAN, 182, 1384);
      Actor_SetAngleToward(ACTOR_EXHAUSTED_STRONGMAN, ACTOR_DORA, 0);
      Event_Wait(30);
      Actor_SetAnimationAndWait(ACTOR_EXHAUSTED_STRONGMAN, ANIM_NOD);
      Event_Wait(10);
      Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
      Actor_SetAngleToward(ACTOR_GARET_CHILD, ACTOR_DORA, 0);
      Actor_SetAngleToward(ACTOR_JENNA_CHILD, ACTOR_DORA, 0);
      Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
      Event_ShowMessageAndWait(ACTOR_DORA, 0);
      Actor_RunRepeatedMotion(ACTOR_JENNA_CHILD, 2);
      Event_ShowMessageThenWait(ACTOR_JENNA_CHILD, 0, 10);
      Actor_FaceDirection(ACTOR_DORA, FACING_NORTHEAST, 40);
      Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 20);
      Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
      Event_ShowMessageAndWait(ACTOR_DORA, 0);
      Actor_TurnToFaceEachOther(ACTOR_JENNA_CHILD, ACTOR_EXHAUSTED_STRONGMAN,
                                0);
      Actor_TurnToFaceEachOther(ACTOR_GARET_CHILD, ACTOR_PARTY_LEADER, 0);
      Event_Wait(40);
      Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
      Actor_SetAngleToward(ACTOR_GARET_CHILD, ACTOR_DORA, 0);
      Actor_SetAngleToward(ACTOR_JENNA_CHILD, ACTOR_DORA, 0);
      Actor_SetAngleToward(ACTOR_EXHAUSTED_STRONGMAN, ACTOR_DORA, 0);
      Actor_RunRepeatedMotion(ACTOR_DORA, 2);
      Event_Wait(20);
      Event_ShowMessageThenWait(ACTOR_DORA, 0, 10);
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
      Import_ActorSetAnimation(ACTOR_JENNA_CHILD, ANIM_NOD);
      Import_ActorSetAnimation(ACTOR_GARET_CHILD, ANIM_NOD);
      automaticPriority = ACTOR_PRIORITY_AUTOMATIC;
      Actor_SetAnimationAndWait(ACTOR_EXHAUSTED_STRONGMAN, ANIM_NOD);
      leader->priorityFlags |= automaticPriority;
      actor = Actor_GetById(ACTOR_EXHAUSTED_STRONGMAN);
      actor->priorityFlags |= automaticPriority;
      ValePrologue_ShowBoulderSweepingDownriver();
      GameFlag_Set(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY);
      Event_End();
    }
  }
}

/// Places an actor and its targets at once, x doubling as its height.
static __inline__ void ValePrologue_PlaceActor(struct Actor *actor,
                                               s32 horizontalPosition,
                                               s32 groundPosition) {
  actor->x.fixed = horizontalPosition;
  actor->y.fixed = horizontalPosition;
  actor->targetX = horizontalPosition;
  actor->targetY = horizontalPosition;
  actor->z.fixed = groundPosition;
  actor->targetZ = groundPosition;
}

/// One crash of the Boulder coming down: a shake, SE_CRASH and a wait.
static __inline__ void ValePrologue_PlayImpactBeat(s32 shakeAmplitude,
                                                   s32 frames) {
  MapGrid_SetShake(shakeAmplitude, shakeAmplitude, Q16_ONE);
  Event_PlaySound(SE_CRASH);
  Event_Wait(frames);
}

/// The Boulder comes down the river and carries Felix, his parents and Kyle
/// away. The party gathers on the bank and hears it coming; the screen fades
/// to the river upstream, where the camera follows the Boulder down
/// (gValePrologue_BoulderRollsDownriver), and two frozen lightning flashes
/// show the family and Kyle in its path. Then the bank is redrawn without them,
/// with the rocks the Boulder left, and the party stands at the spot of
/// VALE_PROLOGUE_FROM_BOULDER_PASSED. Called by
/// ValePrologue_BoulderSweepsThemAway, whose recording on the docs site
/// (places/vale-storm/events/ValePrologue_BoulderSweepsThemAway.gif) shows
/// the descent.
static void ValePrologue_ShowBoulderSweepingDownriver(void) {
  struct Sprite *actorVisual;
  struct Sprite *boulderShadowSprite;
  struct Actor *actor;
  struct Actor *lookingActor;
  struct Actor *boulderShadow;
  struct Actor *currentActor;
  u32 random;
  const s32 *entryActions;
  s32 stoppedVelocity;
  u8 stoppedMotion;
  const s32 *lookAroundActions;
  s32 resetValue;
  const s32 *departureActions;

  actor = Actor_Get(ACTOR_BOULDER);
  boulderShadow = Actor_Get(ACTOR_BOULDER_SHADOW);
  actorVisual = actor->sprite;
  boulderShadowSprite = boulderShadow->sprite;
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(110), -1, PIXELS(1419), 1);
  Actor_SetSpeed(ACTOR_EXHAUSTED_STRONGMAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_JENNA_CHILD, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_GARET_CHILD, PIXELS(6) / 5, PIXELS(3) / 5);
  // Bring the group to the riverbank as the lightning subsides.
  entryActions = gValePrologue_ToRiverbankPath;
  Actor_SetScript(ACTOR_EXHAUSTED_STRONGMAN, entryActions);
  Event_Wait(10);
  Actor_SetScript(ACTOR_JENNA_CHILD, entryActions);
  Weather_LastLightning();
  Event_Wait(10);
  Actor_SetScript(ACTOR_PARTY_LEADER, entryActions);
  Event_Wait(10);
  Weather_StopLightning();
  Actor_SetScript(ACTOR_GARET_CHILD, entryActions);
  Event_Wait(128);
  ValePrologue_StepOffBridge();
  FieldCamera_MoveTo(PIXELS(174), -1, PIXELS(1428), 1);
  Event_Wait(104);
  FieldCamera_MoveTo(PIXELS(153), -1, PIXELS(1325), 1);
  Actor_WalkToAndWait(ACTOR_DORA, 158, 1272);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST, 0);
  Actor_RefreshSelectorById(ACTOR_EXHAUSTED_STRONGMAN);
  Actor_SetScript(ACTOR_EXHAUSTED_STRONGMAN,
                  gValePrologue_ExhaustedStrongmanGatherPath);
  Actor_SetScript(ACTOR_JENNA_CHILD, gValePrologue_JennaGatherPath);
  Actor_SetScript(ACTOR_PARTY_LEADER, gValePrologue_LeaderGatherPath);
  Actor_RunScriptAndWait(ACTOR_GARET_CHILD, gValePrologue_GaretGatherPath);
  ValePrologue_PlayImpactBeat(PIXELS(4), 20);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_Wait(60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA_CHILD, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET_CHILD, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_EXHAUSTED_STRONGMAN, EMOTE_IN_FRONT | EMOTE_QUESTION,
                  0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_FaceEachOther(ACTOR_JENNA_CHILD, ACTOR_EXHAUSTED_STRONGMAN, 0);
  Actor_FaceEachOther(ACTOR_GARET_CHILD, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  lookingActor = Actor_Get(ACTOR_PARTY_LEADER);
  random = Random_Next();
  random = Math_RemainderUnsigned(random, 20) + 20;
  lookingActor->timer = random;
  stoppedVelocity = 0;
  stoppedMotion = 0;
  lookingActor = Actor_Get(ACTOR_GARET_CHILD);
  random = Random_Next();
  lookingActor->timer = (Math_RemainderUnsigned(random, 20) + 20);
  lookingActor = Actor_Get(ACTOR_JENNA_CHILD);
  random = Random_Next();
  lookingActor->timer = (Math_RemainderUnsigned(random, 20) + 20);
  lookingActor = Actor_Get(ACTOR_EXHAUSTED_STRONGMAN);
  random = Random_Next();
  lookingActor->timer = (Math_RemainderUnsigned(random, 20) + 20);
  lookingActor = Actor_Get(ACTOR_DORA);
  random = Random_Next();
  lookingActor->timer = (Math_RemainderUnsigned(random, 20) + 20);
  lookAroundActions = gValePrologue_LookAroundScript;
  Actor_SetScript(ACTOR_DORA, lookAroundActions);
  Event_Wait(30);
  Actor_SetScript(ACTOR_PARTY_LEADER, lookAroundActions);
  Actor_SetScript(ACTOR_JENNA_CHILD, lookAroundActions);
  Actor_SetScript(ACTOR_GARET_CHILD, lookAroundActions);
  Actor_SetScript(ACTOR_EXHAUSTED_STRONGMAN, lookAroundActions);
  Event_Wait(10);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  ValePrologue_PlayImpactBeat(PIXELS(3), 30);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_Wait(120);
  ValePrologue_PlayImpactBeat(PIXELS(3), 40);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Event_Wait(60);
  ValePrologue_PlayImpactBeat(PIXELS(4), 20);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_Wait(60);
  ValePrologue_PlayImpactBeat(PIXELS(3), 40);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_Wait(60);
  Weather_StopLightning();
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_Wait(1);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  FieldCamera_SetSpeed(PIXELS(8), PIXELS(8));
  FieldCamera_MoveTo(PIXELS(217), -1, PIXELS(1084), 1);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_StartInterpolation(40);
  wait(40);
  // Stage the boulder and its shadow for the downriver camera shot.
  Actor_SetSpriteColorById(ACTOR_BOULDER, SPRITE_COLOR_NORMAL);
  currentActor = Actor_Get(ACTOR_BOULDER);
  Actor_SetSpriteFlags(currentActor, 0);
  currentActor = Actor_Get(ACTOR_BOULDER_SHADOW);
  Actor_SetSpriteFlags(currentActor, 0);
  boulderShadow->scaleX = 4 * Q16_ONE / 5;
  boulderShadow->scaleY = 4 * Q16_ONE / 5;
  boulderShadow->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  boulderShadowSprite->image.priority = 1;
  ValePrologue_PlaceActor(actor, PIXELS(200), PIXELS(898));
  actor->motionFlags = stoppedMotion;
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  actorVisual->image.priority = 0;
  currentActor = FieldCamera_GetActor();
  currentActor->targetX = ACTOR_NO_TARGET;
  currentActor = FieldCamera_GetActor();
  currentActor->targetY = ACTOR_NO_TARGET;
  currentActor = FieldCamera_GetActor();
  currentActor->targetZ = ACTOR_NO_TARGET;
  currentActor = FieldCamera_GetActor();
  currentActor->velocityX = stoppedVelocity;
  currentActor = FieldCamera_GetActor();
  currentActor->velocityY = stoppedVelocity;
  currentActor = FieldCamera_GetActor();
  currentActor->velocityZ = stoppedVelocity;
  wait(1);
  FieldCamera_MoveTo(PIXELS(247), PIXELS(128), PIXELS(917), 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(30);
  wait(30);
  // Follow the boulder script until its eighth movement phase.
  Task_AddFrame(ValePrologue_UpdateBoulderShadow, TASK_PASS_FRAME);
  Actor_SetScript(ACTOR_BOULDER, gValePrologue_BoulderRollsDownriver);
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(3) / 100);
  FieldCamera_MoveTo(PIXELS(175), PIXELS(96), PIXELS(1086), 1);
  do {
    wait(1);
  } while ((s16)actor->userValue != 8);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  wait(60);
  MapGrid_WaitForShakeToSettle();
  currentActor = FieldCamera_GetActor();
  currentActor->targetX = ACTOR_NO_TARGET;
  currentActor = FieldCamera_GetActor();
  currentActor->targetY = ACTOR_NO_TARGET;
  currentActor = FieldCamera_GetActor();
  currentActor->targetZ = ACTOR_NO_TARGET;
  resetValue = 0;
  currentActor = FieldCamera_GetActor();
  currentActor->velocityX = resetValue;
  currentActor = FieldCamera_GetActor();
  currentActor->velocityY = resetValue;
  currentActor = FieldCamera_GetActor();
  currentActor->velocityZ = resetValue;
  Task_Remove(ValePrologue_UpdateBoulderShadow);
  Actor_Stop(ACTOR_BOULDER);
  wait(1);
  Actor_SetAnimation(ACTOR_BOULDER, ANIM_STILL);
  boulderShadow->scaleX = 5 * Q16_ONE / 4;
  boulderShadow->scaleY = 5 * Q16_ONE / 4;
  boulderShadowSprite->offsetY = 2;
  boulderShadowSprite->scale = 5 * Q16_ONE / 4;
  actor->scaleX = 2 * Q16_ONE;
  actor->scaleY = 2 * Q16_ONE;
  actor->x.fixed = resetValue;
  actor->z.fixed = resetValue;
  actor->targetX = resetValue;
  actor->targetZ = resetValue;
  wait(1);
  // Stage the riverbank reactions between lightning flashes.
  Actor_SetAnimation(ACTOR_FELIX_CHILD, ANIM_FELIX_CLING_STILL);
  Actor_SetPosition(ACTOR_DORA, PIXELS(169), PIXELS(1264));
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 0);
  Actor_SetAnimation(ACTOR_DORA, ANIM_DORA_RUN);
  Actor_SetPosition(ACTOR_JENNA_CHILD, PIXELS(151), PIXELS(1292));
  Actor_FaceDirection(ACTOR_JENNA_CHILD, FACING_WEST, 0);
  Actor_SetAnimation(ACTOR_JENNA_CHILD, ANIM_JENNA_RUN);
  Actor_SetPosition(ACTOR_EXHAUSTED_STRONGMAN, PIXELS(170), PIXELS(1313));
  Actor_FaceDirection(ACTOR_EXHAUSTED_STRONGMAN, FACING_SOUTHWEST, 0);
  Actor_SetAnimation(ACTOR_EXHAUSTED_STRONGMAN, ANIM_STRONGMAN_RUN);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(185), PIXELS(1333));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_ISAAC_RECOIL);
  Actor_SetPosition(ACTOR_GARET_CHILD, PIXELS(169), PIXELS(1384));
  Actor_FaceDirection(ACTOR_GARET_CHILD, FACING_SOUTH, 0);
  Actor_SetAnimation(ACTOR_GARET_CHILD, ANIM_STILL);
  FieldCamera_MoveTo(PIXELS(166), 0, PIXELS(1337), 0);
  MapGrid_CenterOnFollowedPoint();
  actor->motionFlags = resetValue;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  ValePrologue_FlashLightning();
  Actor_SetPosition(ACTOR_BOULDER_SHADOW, PIXELS(218), PIXELS(1176));
  FieldCamera_MoveTo(PIXELS(210), 0, PIXELS(1196), 0);
  MapGrid_CenterOnFollowedPoint();
  boulderShadow->scaleX = 2 * Q16_ONE;
  boulderShadow->scaleY = 2 * Q16_ONE;
  Task_AddFrame(ValePrologue_FlickerBoulderShadow, TASK_PASS_FRAME);
  Actor_Stop(ACTOR_KYLE);
  Actor_Stop(ACTOR_JENNAS_FATHER);
  Actor_Stop(ACTOR_JENNAS_MOTHER);
  wait(1);
  boulderShadow = Actor_Get(ACTOR_KYLE);
  boulderShadowSprite = boulderShadow->sprite;
  boulderShadow->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  boulderShadow->scaleX = Q16_ONE;
  boulderShadow->scaleY = Q16_ONE;
  boulderShadow->facing = FACING_NORTH + FACING_STEP;
  boulderShadowSprite->image.priority = 0;
  Actor_SetAnimation(ACTOR_KYLE, ANIM_STILL);
  boulderShadow = Actor_Get(ACTOR_JENNAS_FATHER);
  boulderShadowSprite = boulderShadow->sprite;
  boulderShadow->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  boulderShadow->scaleX = Q16_ONE;
  boulderShadow->scaleY = Q16_ONE;
  boulderShadow->facing = FACING_NORTHWEST + FACING_STEP;
  boulderShadowSprite->image.priority = 0;
  Actor_SetAnimation(ACTOR_JENNAS_FATHER, ANIM_PARENT_RECOIL);
  boulderShadow = Actor_Get(ACTOR_JENNAS_MOTHER);
  boulderShadowSprite = boulderShadow->sprite;
  boulderShadow->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  boulderShadow->scaleX = Q16_ONE;
  boulderShadow->scaleY = Q16_ONE;
  boulderShadow->facing = FACING_NORTHWEST + FACING_STEP;
  boulderShadowSprite->image.priority = 0;
  Actor_SetAnimation(ACTOR_JENNAS_MOTHER, ANIM_PARENT_RECOIL);
  boulderShadow = Actor_Get(ACTOR_BOULDER_SHADOW);
  boulderShadowSprite = boulderShadow->sprite;
  ValePrologue_FlashLightning();
  actor->y.fixed = PIXELS(48);
  actor->x.fixed = PIXELS(214);
  actor->z.fixed = PIXELS(1216);
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  boulderShadowSprite->image.priority = 1;
  Actor_SetPosition(ACTOR_BOULDER_SHADOW, PIXELS(214), PIXELS(1216));
  Actor_FaceDirection(ACTOR_JENNAS_FATHER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNAS_MOTHER, FACING_NORTH, 20);
  Event_SetFlag(FLAG_WEATHER_HELD);
  TilesetAnim_Pause(0);
  TilesetAnim_Pause(1);
  TilesetAnim_Pause(2);
  TilesetAnim_Pause(3);
  TilesetAnim_Pause(4);
  TilesetAnim_Pause(5);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(120);
  wait(160);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_BG);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(80);
  Event_Wait(80);
  Event_Wait(100);
  Task_Remove(ValePrologue_FlickerBoulderShadow);
  boulderShadowSprite->scale = boulderShadow->scaleX;
  GameFlag_Clear(FLAG_WEATHER_HELD);
  TilesetAnim_Resume(0);
  TilesetAnim_Resume(1);
  TilesetAnim_Resume(2);
  TilesetAnim_Resume(3);
  TilesetAnim_Resume(4);
  TilesetAnim_Resume(5);
  // Restore the riverbank and the idle actor scripts.
  ValePrologue_RebuildRiverbank();
  Actor_SetPosition(ACTOR_DORA, PIXELS(165), PIXELS(1229));
  Actor_SetAnimation(ACTOR_DORA, ANIM_STAND);
  actor = Actor_Get(ACTOR_DORA);
  actor->facing = FACING_NORTHEAST;
  random = Random_Next();
  actor->timer = Math_RemainderUnsigned(random, 90) + 60;
  departureActions = gValePrologue_SquashAndStretchScript;
  actor->userValue = 1;
  Actor_SetScript(ACTOR_DORA, departureActions);
  Actor_SetPosition(ACTOR_JENNA_CHILD, PIXELS(165), PIXELS(1254));
  Actor_SetAnimation(ACTOR_JENNA_CHILD, ANIM_STAND);
  actor = Actor_Get(ACTOR_JENNA_CHILD);
  actor->facing = FACING_NORTHEAST;
  random = Random_Next();
  actor->timer = Math_RemainderUnsigned(random, 90) + 60;
  actor->userValue = 2;
  Actor_SetScript(ACTOR_JENNA_CHILD, departureActions);
  Actor_SetPosition(ACTOR_GARET_CHILD, PIXELS(152), PIXELS(1285));
  Actor_SetAnimation(ACTOR_GARET_CHILD, ANIM_STAND);
  actor = Actor_Get(ACTOR_GARET_CHILD);
  actor->facing = FACING_NORTHEAST;
  random = Random_Next();
  actor->timer = Math_RemainderUnsigned(random, 90) + 60;
  actor->userValue = 3;
  Actor_SetScript(ACTOR_GARET_CHILD, departureActions);
  Actor_SetPosition(ACTOR_EXHAUSTED_STRONGMAN, PIXELS(180), PIXELS(1311));
  actor = Actor_Get(ACTOR_EXHAUSTED_STRONGMAN);
  actor->facing = FACING_NORTHEAST;
  random = Random_Next();
  actor->timer = Math_RemainderUnsigned(random, 90) + 60;
  actor->userValue = 4;
  Actor_SetScript(ACTOR_EXHAUSTED_STRONGMAN, departureActions);
  Actor_SetAnimation(ACTOR_EXHAUSTED_STRONGMAN, ANIM_STRONGMAN_SPRAWLED);
  Actor_GetById(ACTOR_GARET_CHILD)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(ACTOR_EXHAUSTED_STRONGMAN)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Task_AddFrame(ValePrologue_CopyLeaderSpritePriority, TASK_PASS_FRAME);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(181), PIXELS(1273));
  currentActor = Actor_Get(ACTOR_PARTY_LEADER);
  currentActor->facing = FACING_NORTHEAST;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldCamera_MoveTo(PIXELS(181), 0, PIXELS(1273), 0);
  MapGrid_CenterOnFollowedPoint();
  Actor_SetPosition(ACTOR_KYLE, 0, 0);
  Actor_SetPosition(ACTOR_BOULDER, 0, 0);
  Actor_SetPosition(ACTOR_JENNAS_FATHER, 0, 0);
  Actor_SetPosition(ACTOR_JENNAS_MOTHER, 0, 0);
  Actor_SetPosition(ACTOR_FELIX_CHILD, 0, 0);
  Actor_SetPosition(ACTOR_BOULDER_SHADOW, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_ROCK + 6, PIXELS(144), PIXELS(1070));
  Actor_SetPosition(ACTOR_FIRST_ROCK + 7, PIXELS(276), PIXELS(1270));
  wait(60);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(80);
  Event_Wait(60);
  Scene_PlayQueuedMusic();
  Event_Wait(60);
  Party_RemoveMemberRestored(1);
  Weather_ResumeLightning();
}

/// A lightning flash that freezes the picture: holds the rain
/// (FLAG_WEATHER_HELD) and pauses the map's tileset animations 0 to 5,
/// shows the event in PALETTE_TINT_WARM for two seconds, fades it to
/// black, then lets the rain and the animations run again.
static void ValePrologue_FlashLightning(void) {
  wait(20);
  Event_SetFlag(FLAG_WEATHER_HELD);
  TilesetAnim_Pause(0);
  TilesetAnim_Pause(1);
  TilesetAnim_Pause(2);
  TilesetAnim_Pause(3);
  TilesetAnim_Pause(4);
  TilesetAnim_Pause(5);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(1);
  wait(120);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  wait(60);
  GameFlag_Clear(FLAG_WEATHER_HELD);
  TilesetAnim_Resume(0);
  TilesetAnim_Resume(1);
  TilesetAnim_Resume(2);
  TilesetAnim_Resume(3);
  TilesetAnim_Resume(4);
  TilesetAnim_Resume(5);
}

/// The task that keeps ACTOR_BOULDER_SHADOW on the Boulder as it comes
/// down the river: the shadow swells or shrinks by the step of
/// gValePrologue_BoulderRollsDownriver the Boulder is on (its work value),
/// shows on odd frames only, and the map shakes as the step's timer counts
/// down.
static void ValePrologue_UpdateBoulderShadow(void) {
  struct Actor *source;
  struct Actor *shadow;
  struct Sprite *sprite;
  s16 *timer;

  source = Actor_GetById(ACTOR_BOULDER);
  shadow = Actor_GetById(ACTOR_BOULDER_SHADOW);
  sprite = shadow->sprite;
  timer = &source->timer;
  if (*timer != 0) {
    if (*timer == 60) {
      MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
    }
    if (*timer == 40) {
      MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
    }
    if (*timer == 30) {
      MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    }
    if (*timer == 20) {
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    }
    (*timer)--;
  }
  shadow->x.fixed = source->x.fixed;
  shadow->z.fixed = source->z.fixed;
  shadow->targetX = shadow->x.fixed;
  sprite->offsetY = 10;
  if (gFrameCount & 1) {
    switch ((s16)source->userValue) {
    case 1:
    case 5:
      shadow->scaleX += Q16_ONE / 25;
      shadow->scaleY += Q16_ONE / 25;
      break;
    case 4:
      shadow->scaleX += Q16_ONE / 50;
      shadow->scaleY += Q16_ONE / 50;
      break;
    case 2:
    case 3:
    case 6:
    case 7:
    case 8:
    case 9:
      shadow->scaleX += -3 * Q16_ONE / 100;
      shadow->scaleY += -3 * Q16_ONE / 100;
      break;
    }
    sprite->scale = shadow->scaleX;
  } else {
    sprite->scale = 0;
  }
}

/// The task that moves ACTOR_BOULDER_SHADOW between two heights on
/// alternate frames, during the frozen flashes.
static void ValePrologue_FlickerBoulderShadow(void) {
  struct Actor *actor = Actor_GetById(ACTOR_BOULDER_SHADOW);
  u32 frame = gFrameCount;
  struct Sprite *sprite = actor->sprite;

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
static void ValePrologue_StrongmanTriesToGoForHelp(void) {
  struct Actor *villager;
  struct Actor *leader;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  villager = Actor_GetById(ACTOR_EXHAUSTED_STRONGMAN);
  Event_Start();
  if (GameFlag_Get(FLAG_VALE_STORM_STRONGMAN_TRIED_TO_GO) != 0) {
    Actor_Stop(ACTOR_EXHAUSTED_STRONGMAN);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_EXHAUSTED_STRONGMAN, 2);
    Event_Wait(40);
    if ((s16)leader->facing >= 0) {
      Import_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                               ANIM_STRONGMAN_PROPPED_UP);
    } else {
      Import_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                               ANIM_STRONGMAN_PROPPED_UP_OTHER_WAY);
    }
    Actor_RunRepeatedMotion(ACTOR_EXHAUSTED_STRONGMAN, 2);
    Event_Wait(20);
    Event_SetMessageId(MsgValeICantMoveGetHelp);
    Event_ShowMessage(ACTOR_EXHAUSTED_STRONGMAN, 0);
    Actor_SetScript(ACTOR_EXHAUSTED_STRONGMAN,
                    gValePrologue_SquashAndStretchScript);
    Import_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                             ANIM_STRONGMAN_SPRAWLED);
  } else {
    Actor_Stop(ACTOR_EXHAUSTED_STRONGMAN);
    villager->scaleX = Q16_ONE;
    villager->scaleY = Q16_ONE;
    Actor_FaceDirection(ACTOR_EXHAUSTED_STRONGMAN, FACING_EAST + FACING_STEP,
                        0);
    if ((s16)leader->facing >= 0) {
      Import_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                               ANIM_STRONGMAN_PROPPED_UP);
    } else {
      Import_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                               ANIM_STRONGMAN_PROPPED_UP_OTHER_WAY);
    }
    Event_Wait(20);
    Event_SetMessageId(MsgValeIllGo);
    FieldEvent_ShowMessageThenWait(ACTOR_EXHAUSTED_STRONGMAN, 0, 20);
    Import_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, ANIM_STAND);
    Actor_Jump(ACTOR_EXHAUSTED_STRONGMAN, 4, 0);
    Event_Wait(80);
    Actor_RunRepeatedMotion(ACTOR_EXHAUSTED_STRONGMAN, 2);
    Event_Wait(40);
    if ((s16)leader->facing >= 0) {
      Import_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                               ANIM_STRONGMAN_PROPPED_UP);
    } else {
      Import_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                               ANIM_STRONGMAN_PROPPED_UP_OTHER_WAY);
    }
    Event_Wait(2);
    Actor_Jump(ACTOR_EXHAUSTED_STRONGMAN, 2, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_EXHAUSTED_STRONGMAN, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_EXHAUSTED_STRONGMAN, 0);
    Actor_SetScript(ACTOR_EXHAUSTED_STRONGMAN,
                    gValePrologue_SquashAndStretchScript);
    Import_ActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
                             ANIM_STRONGMAN_SPRAWLED);
    GameFlag_Set(FLAG_VALE_STORM_STRONGMAN_TRIED_TO_GO);
  }
  Event_End();
}

/// As Isaac leaves the river to get help, Garet will not let him go alone and
/// goes after him. "I can't let him go alone..."
/// Called by ValePrologue_LeaveToValePlaza after the Boulder swept Felix,
/// his parents and Kyle away; sets FLAG_VALE_STORM_GARET_LEFT, which has the
/// two meet the strangers on the way (vale_plaza.c).
static void ValePrologue_GaretFollowsIsaac(void) {
  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_Stop(ACTOR_GARET_CHILD);
  Task_Remove(ValePrologue_CopyLeaderSpritePriority);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 480, 1392);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_FaceDirection(ACTOR_GARET_CHILD, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_Get(ACTOR_GARET_CHILD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetPosition(ACTOR_GARET_CHILD, PIXELS(249), PIXELS(1240));
  wait(1);
  Event_SetMessageId(MsgValeIsaac);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET_CHILD, 0);
  Actor_SetPosition(ACTOR_GARET_CHILD, PIXELS(172), PIXELS(1278));
  wait(1);
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  FieldCamera_MoveTo(PIXELS(162), 0, PIXELS(1285), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_GARET_CHILD, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET_CHILD, 0,
                                 10);
  Actor_FaceDirection(ACTOR_GARET_CHILD, FACING_NORTH, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET_CHILD, 2);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET_CHILD, 0,
                                 10);
  Actor_FaceDirection(ACTOR_GARET_CHILD, FACING_EAST + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET_CHILD, ANIM_NOD);
  Actor_SetSpeed(ACTOR_GARET_CHILD, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(ACTOR_GARET_CHILD, 165, 1300);
  Actor_WalkToAndWait(ACTOR_GARET_CHILD, 195, 1432);
  GameFlag_Set(FLAG_VALE_STORM_GARET_LEFT);
}

/// Redraws the riverbank as the Boulder left it, pictures and cell
/// behaviors. Called by ValePrologue_ShowBoulderSweepingDownriver and, on
/// later arrivals, by ValePrologue_Initialize.
static void ValePrologue_RebuildRiverbank(void) {
  MapGrid_CopyCellsTo(16, 96, 11, 73, 6, 3);
  MapGrid_CopyCellsTo(16, 96, 34, 68, 14, 10);
  MapGrid_CopyCellsTo(16, 96, 64, 68, 7, 7);
  MapGrid_CopyCellsTo(9, 95, 11, 73, 6, 3);
  MapGrid_CopyCellsTo(40, 94, 34, 68, 14, 10);
  MapGrid_CopyCellsTo(54, 94, 64, 68, 8, 7);
  MapGrid_CopyCellsTo(72, 75, 72, 76, 1, 1);
  MapGrid_CopyCellsTo(72, 75, 74, 76, 1, 1);
  MapGrid_CopyCellAttributes(7, 75, 1, 1, 6, 75);
  MapGrid_CopyCellAttributes(8, 70, 3, 1, 8, 71);
  MapGrid_CopyCellAttributes(8, 70, 2, 1, 9, 72);
  MapGrid_CopyCellAttributes(8, 70, 2, 1, 9, 73);
  MapGrid_CopyCellAttributes(11, 66, 1, 1, 8, 73);
  MapGrid_CopyCellAttributes(12, 66, 1, 4, 11, 73);
  MapGrid_CopyCellAttributes(25, 0, 1, 1, 6, 74);
  MapGrid_CenterOnFollowedPoint();
}

/// In Vale on the stormy night, a monster attacks on the paths (slot 2 of
/// ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT).
/// Reached by stepping on cell behavior 16 until
/// FLAG_VALE_STORM_THIRD_MONSTER_FOUGHT is set.
static void ValePrologue_FightThirdMonster(void) {
  struct FieldWork *state;

  Event_Start();
  state = gFieldWork;
  state->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  state->transitionFrames = 64;
  GameFlag_Set(FLAG_VALE_STORM_THIRD_MONSTER_FOUGHT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 2);
  GameFlag_Set(FLAG_VALE_STORM_MONSTER_FOUGHT);
  Event_End();
}

/// In Vale on the stormy night, another monster attacks (slot 3 of
/// ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT).
/// Reached by stepping on cell behavior 17 until
/// FLAG_VALE_STORM_FOURTH_MONSTER_FOUGHT is set.
static void ValePrologue_FightFourthMonster(void) {
  struct FieldWork *state;

  Event_Start();
  state = gFieldWork;
  state->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  state->transitionFrames = 64;
  GameFlag_Set(FLAG_VALE_STORM_FOURTH_MONSTER_FOUGHT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 3);
  GameFlag_Set(FLAG_VALE_STORM_MONSTER_FOUGHT);
  Event_End();
}

/// An elder calling up Psynergy, for 40 frames once the camera reaches the
/// Boulder: the sprite shows palette 7 for two frames in four, and a pair of
/// Psynergy sparks rises and falls every 16 frames plus 8 for each step of
/// sShakeShift.
static void ValePrologue_FlashPaletteAndSpawnEffects(struct Actor *actor) {
  s32 spawnInterval;

  if ((gFrameCount & 2) != 0) {
    Actor_SetColorMode(actor, 7);
  } else {
    Actor_SetColorMode(actor, 0);
  }
  {
    volatile u32 *frame = &gFrameCount;

    spawnInterval = (sShakeShift << 3) + 16;
    if (Math_RemainderUnsigned(*frame, spawnInterval) == 0) {
      ValePrologue_SpawnEffectPair(actor);
    }
  }
}

/// An elder holding the Boulder: the sprite cycles through palettes 0 to 5,
/// and sparks rise and fall as in ValePrologue_FlashPaletteAndSpawnEffects.
static void ValePrologue_CyclePaletteAndSpawnEffects(struct Actor *actor) {
  s32 paletteFrame;
  s32 spawnInterval;

  if ((gFrameCount & 1) != 0) {
    paletteFrame = (gFrameCount >> 1);
    Actor_SetColorMode(actor, Math_RemainderUnsigned(paletteFrame, 6));
  }
  spawnInterval = (sShakeShift << 3) + 16;
  if (Math_RemainderUnsigned(gFrameCount, spawnInterval) == 0) {
    ValePrologue_SpawnEffectPair(actor);
  }
}

/// Cycles the sprite through palettes 0 to 5, a step every second frame.
static void ValePrologue_CycleActorPalettes(struct Actor *actor) {
  s32 paletteFrame;

  if ((gFrameCount & 1) != 0) {
    paletteFrame = (gFrameCount >> 1);
    Actor_SetColorMode(actor, Math_RemainderUnsigned(paletteFrame, 6));
  }
}

#include "shared/effect_vertical.h"

/// Spawns a pair of Psynergy sparks on an elder, one rising and one falling
/// (shared/effect_vertical.h), with SE_JUMP.
static void ValePrologue_SpawnEffectPair(struct Actor *parent) {
  struct Actor *pair[2];
  struct Actor *child;
  struct Sprite *sprite;
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  s32 i;

  Sound_Play(SE_JUMP);
  for (i = 0; i < 2; ++i) {
    child = FieldActor_Create(SPRITE_GOLDEN_RINGS, parent->x.fixed,
                              parent->y.fixed, parent->z.fixed);
    pair[i] = child;
    if (child != NULL) {
      child->groundHeight = parent->groundHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->watchedActor = parent;
      if (sprite != NULL) {
        Sprite_StartAnimation(sprite, 0);
        sprite->flags = 0;
        VramCache_FreeEntry(sprite->slot);
        sprite->slot = effectWork->tileSlot;
        sprite->retainCacheEntry = 1;
        sprite->image.tile =
            (gVramCacheEntries[sprite->slot].offset >> 5) & 0x3ff;
        sprite->image.fullColor = 0;
        sprite->image.shape = 1;
        sprite->image.size = 2;
        sprite->entries[0]->picture = 0;
      }
    }
  }
  {
    struct Actor *effect = pair[0];
    struct Sprite *effectSprite = effect->sprite;

    effect->update = FieldEffect_AnimateVerticalNegative;
    effectSprite->image.priority = parent->sprite->image.priority;
  }
  {
    struct Actor *effect = pair[1];
    struct Sprite *effectSprite = effect->sprite;

    effectSprite->image.priority = parent->sprite->image.priority;
    effect->update = FieldEffect_AnimateVerticalPositive;
    effect->priorityFlags = 2;
  }
}

#include "shared/effect_psynergy.h"

/// Cycles the four elders' palettes and effects while they hold the Boulder;
/// the westernmost elder's stop once that elder lets go (sShakeDone).
static void ValePrologue_UpdateBoulderMapEffects(void) {
  ValePrologue_CyclePaletteAndSpawnEffects(Actor_GetById(ACTOR_ELDER_EAST));
  ValePrologue_CyclePaletteAndSpawnEffects(Actor_GetById(ACTOR_ELDER_WEST));
  ValePrologue_CyclePaletteAndSpawnEffects(Actor_GetById(ACTOR_ELDER_FAR_EAST));
  if (sShakeDone == 0) {
    ValePrologue_CyclePaletteAndSpawnEffects(
        Actor_GetById(ACTOR_ELDER_FAR_WEST));
  }
}

/// The task that cycles the Boulder's palettes while the elders hold it.
static void ValePrologue_CycleBoulderPalette(void) {
  ValePrologue_CycleActorPalettes(Actor_GetById(ACTOR_BOULDER));
}

/// Keeps Garet and the man who came to help (ACTOR_EXHAUSTED_STRONGMAN) at
/// the leader's sprite priority while they stand by the river.
static void ValePrologue_CopyLeaderSpritePriority(void) {
  struct Sprite *sourceSprite;
#if !TBS_PATCH_3
  struct Sprite *targetSprite;
  struct Sprite *secondTargetSprite;
#endif

  sourceSprite = Actor_GetById(ACTOR_PARTY_LEADER)->sprite;
#if TBS_PATCH_3
  Actor_SetSpritePriority(ACTOR_GARET_CHILD, sourceSprite->image.priority);
  Actor_SetSpritePriority(ACTOR_EXHAUSTED_STRONGMAN,
                          sourceSprite->image.priority);
#else
  targetSprite = Actor_GetById(ACTOR_GARET_CHILD)->sprite;
  targetSprite->image.priority = sourceSprite->image.priority;
  secondTargetSprite = Actor_GetById(ACTOR_EXHAUSTED_STRONGMAN)->sprite;
  secondTargetSprite->image.priority = sourceSprite->image.priority;
#endif
}

/// In Vale on the stormy night, searching a broken door: it won't open, the
/// impact having knocked it off its hinges.
static void ValePrologue_SearchBrokenDoor(void) {
  Event_Start();
  FieldMessage_Show(MsgValeDoorWontOpen, 1);
  Event_End();
}

/// In Vale on the stormy night, searching the chest Garet was trying to save.
/// "It's Garet's chest of valuables."
static void ValePrologue_SearchValuablesChest(void) {
  Event_Start();
  FieldMessage_Show(MsgValeSChestValuables, 1);
  Event_End();
}

/// ValePrologue_SetActorFrameMode's parameter for mode 0, by the frame
/// count.
u8 gValePrologue_FrameModes[] = {7, 0, 0, 0};

// The actors' scripts, positions in pixels.

/// The stout man runs off toward the plaza after shoving Isaac.
s32 gValePrologue_ShovingManScript[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(185), 0, PIXELS(824)),
    SC_SYNC,         SC_MOVE(PIXELS(179), 0, PIXELS(836)),
    SC_SYNC,         SC_MOVE(PIXELS(226), 0, PIXELS(840)),
    SC_SYNC,         SC_MOVE(PIXELS(226), 0, PIXELS(944)),
    SC_SYNC,         SC_POSITION(0, 0, 0),
    SC_ANIMATION(1), SC_END,
};

/// Garet runs up to Isaac (ValePrologue_UnusedGaretJoinsIsaac).
s32 gValePrologue_UnusedGaretRunsUpScript[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(345), 0, PIXELS(612)),
    SC_SYNC,         SC_MOVE(PIXELS(311), 0, PIXELS(638)),
    SC_SYNC,         SC_MOVE(PIXELS(256), 0, PIXELS(608)),
    SC_SYNC,         SC_ANIMATION(1),
    SC_END,
};

/// Garet heaves for breath: his sprite stretches over 24 frames and settles
/// over 12, for ever.
s32 gValePrologue_UnusedGaretPantingScript[] = {
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
s32 gValePrologue_BoulderWaits[] = {
    SC_ANIMATION(2),
    SC_END,
};

/// Isaac and Garet run to where they see the elders hold the Boulder
/// (ValePrologue_WatchBoulderFall), and face each other.
s32 gValePrologue_BoulderFallLeaderPath[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(94), 0, PIXELS(564)), SC_SYNC,
    SC_ANIMATION(1), SC_SET(SW_FACING, FACING_WEST),      SC_WAIT(30),
    SC_END,
};

s32 gValePrologue_BoulderFallGaretPath[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(76), 0, PIXELS(564)), SC_SYNC,
    SC_ANIMATION(1), SC_SET(SW_FACING, FACING_EAST),      SC_WAIT(30),
    SC_END,
};

// ValePrologue_DoraGoesForHelp: Jenna and Dora run into the house by the
// river by its east door and come out by its west door; Jenna then heads for
// the plaza and Dora north.

s32 gValePrologue_DoraRunsIntoRiversideHouse[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(178), 0, PIXELS(1190)),
    SC_SYNC,         SC_MOVE(PIXELS(149), 0, PIXELS(1194)),
    SC_SYNC,         SC_ANIMATION(1),
    SC_END,
};

s32 gValePrologue_JennaRunsIntoRiversideHouse[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(198), 0, PIXELS(1176)),
    SC_SYNC,         SC_MOVE(PIXELS(170), 0, PIXELS(1195)),
    SC_SYNC,         SC_MOVE(PIXELS(149), 0, PIXELS(1194)),
    SC_SYNC,         SC_MOVE(PIXELS(149), 0, PIXELS(1175)),
    SC_SYNC,         SC_POSITION(0, 0, 0),
    SC_ANIMATION(1), SC_END,
};

s32 gValePrologue_DoraComesOutOfRiversideHouse[] = {
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

s32 gValePrologue_JennaComesOutOfRiversideHouse[] = {
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

s32 gValePrologue_DoraHeadsNorth[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(50), 0, PIXELS(1214)),
    SC_SYNC,         SC_MOVE(PIXELS(46), 0, PIXELS(1130)),
    SC_SYNC,         SC_MOVE(PIXELS(67), 0, PIXELS(1100)),
    SC_SYNC,         SC_ANIMATION(1),
    SC_END,
};

s32 gValePrologue_JennaHeadsForPlaza[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(153), 0, PIXELS(1251)),
    SC_SYNC,         SC_MOVE(PIXELS(177), 0, PIXELS(1357)),
    SC_SYNC,         SC_POSITION(0, 0, 0),
    SC_ANIMATION(1), SC_END,
};

/// Dora leaves to the west once she has asked the boys to help.
s32 gValePrologue_DoraLeavesForHelp[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(123), 0, PIXELS(1078)),
    SC_SYNC,         SC_MOVE(PIXELS(148), 0, PIXELS(1078)),
    SC_SYNC,         SC_MOVE(PIXELS(148), 0, PIXELS(1064)),
    SC_SYNC,         SC_MOVE(PIXELS(35), 0, PIXELS(1064)),
    SC_SYNC,         SC_POSITION(0, 0, 0),
    SC_ANIMATION(1), SC_END,
};

/// Dora runs down to the river when Jenna comes back
/// (ValePrologue_BoulderSweepsThemAway).
s32 gValePrologue_DoraRunsToRiver[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(102), 0, PIXELS(1236)),
    SC_SYNC,         SC_MOVE(PIXELS(148), 0, PIXELS(1256)),
    SC_SYNC,         SC_MOVE(PIXELS(177), 0, PIXELS(1303)),
    SC_SYNC,         SC_ANIMATION(1),
    SC_END,
};

/// The way down to the riverbank that each of the party and the strongman
/// takes in turn as the Boulder comes, then each one's place there.
s32 gValePrologue_ToRiverbankPath[] = {
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

s32 gValePrologue_ExhaustedStrongmanGatherPath[] = {
    SC_MOVE(PIXELS(184), 0, PIXELS(1309)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_END,
};

s32 gValePrologue_JennaGatherPath[] = {
    SC_MOVE(PIXELS(165), 0, PIXELS(1306)), SC_SYNC, SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTHWEST),   SC_END,
};

s32 gValePrologue_LeaderGatherPath[] = {
    SC_MOVE(PIXELS(183), 0, PIXELS(1325)), SC_SYNC, SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTHWEST),   SC_END,
};

s32 gValePrologue_GaretGatherPath[] = {
    SC_MOVE(PIXELS(166), 0, PIXELS(1325)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_END,
};

/// Everyone on the bank looks about (ValePrologue_RerollHeadingOnCountdown).
s32 gValePrologue_LookAroundScript[] = {
    SC_CALL(ValePrologue_RerollHeadingOnCountdown),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// A squash and stretch every 60 to 149 frames (Scene_SquashAndStretch), for
/// the people straining at the river and, after the Boulder, those on the
/// bank.
s32 gValePrologue_SquashAndStretchScript[] = {
    SC_CALL(Scene_SquashAndStretch),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The Boulder bounds down the river in nine steps. Each sets its step in
/// SW_USER_VALUE, which ValePrologue_UpdateBoulderShadow reads and
/// ValePrologue_ShowBoulderSweepingDownriver waits on, and two of its
/// landings start a shake countdown in SW_TIMER.
s32 gValePrologue_BoulderRollsDownriver[] = {
    SC_ANIMATION(2),
    SC_SET(SW_SPEED, PIXELS(3)),
    SC_SET(SW_ACCELERATION, PIXELS(1)),
    SC_SET(SW_TIMER, 0),
    SC_SET(SW_USER_VALUE, 1),
    SC_SET(SW_SCALE_X, 2 * Q16_ONE),
    SC_SET(SW_SCALE_Y, 2 * Q16_ONE),
    SC_MOVE(PIXELS(256), PIXELS(128), PIXELS(931)),
    SC_SYNC,
    SC_SE(SE_CRASH),
    SC_SET(SW_TIMER, 60),
    SC_SET(SW_USER_VALUE, 2),
    SC_MOVE(PIXELS(228), PIXELS(149), PIXELS(957)),
    SC_SYNC,
    SC_SET(SW_USER_VALUE, 3),
    SC_MOVE(PIXELS(201), PIXELS(160), PIXELS(984)),
    SC_SYNC,
    SC_SET(SW_USER_VALUE, 4),
    SC_MOVE(PIXELS(173), PIXELS(138), PIXELS(1011)),
    SC_SYNC,
    SC_SET(SW_USER_VALUE, 5),
    SC_MOVE(PIXELS(146), PIXELS(96), PIXELS(1038)),
    SC_SYNC,
    SC_SE(SE_CRASH),
    SC_SET(SW_SPEED, PIXELS(3) / 2),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 2),
    SC_SET(SW_TIMER, 60),
    SC_SET(SW_USER_VALUE, 6),
    SC_MOVE(PIXELS(161), PIXELS(107), PIXELS(1050)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCELERATION, PIXELS(3) / 5),
    SC_SET(SW_USER_VALUE, 7),
    SC_MOVE(PIXELS(177), PIXELS(96), PIXELS(1062)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(5) / 2),
    SC_SET(SW_ACCELERATION, PIXELS(4) / 5),
    SC_SET(SW_USER_VALUE, 8),
    SC_MOVE(PIXELS(201), PIXELS(74), PIXELS(1074)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(3)),
    SC_SET(SW_ACCELERATION, PIXELS(1)),
    SC_SET(SW_USER_VALUE, 9),
    SC_MOVE(PIXELS(225), -PIXELS(64), PIXELS(1086)),
    SC_SYNC,
    SC_SET(SW_TIMER, 0),
    SC_SET(SW_USER_VALUE, 0),
    SC_END,
};

struct SceneEntrance gValePrologue_Entrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 167, 0, 1281, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {1, CONDITION_ALWAYS, 257, 0, 456, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 113, 0, 303, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_PROLOGUE_FROM_VALE_KRADENS_COTTAGE_NORTH, CONDITION_ALWAYS, 27, 0,
     621, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {VALE_PROLOGUE_FROM_VALE_KRADENS_COTTAGE_SOUTH, CONDITION_ALWAYS, 29, 0,
     792, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {VALE_PROLOGUE_FROM_VALE_PLAZA, CONDITION_ALWAYS, 458, 0, 1393, FACING_WEST,
     0, -1, -1, -1, -1, 0},
    {VALE_PROLOGUE_FROM_COTTAGE_ROOMS_A_EAST, CONDITION_ALWAYS, 406, 0, 743,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_PROLOGUE_FROM_COTTAGE_ROOMS_A_SOUTH, CONDITION_ALWAYS, 262, 0, 821,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_ISAACS_HOUSE, CONDITION_ALWAYS, 340, 0,
     904, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_PROLOGUE_FROM_COTTAGE_ROOMS_A_NORTH, CONDITION_ALWAYS, 326, 0, 1142,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_PROLOGUE_FROM_COTTAGE_ROOMS_A_WEST, CONDITION_ALWAYS, 374, 0, 1254,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 102, 0, 1222, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WEST, CONDITION_ALWAYS, 101, 0, 1216,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_EAST, CONDITION_ALWAYS, 146, 0, 1193,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WITH_DORA, CONDITION_ALWAYS, 335, 0,
     908, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {VALE_PROLOGUE_FROM_BOULDER_PASSED, CONDITION_ALWAYS, 181, 0, 1273,
     FACING_NORTHEAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gValePrologue_Exits[] = {
    SCENE_EXITS(ValePrologue),
    SCENE_EXIT(VALE_PROLOGUE_TO_VALE_PLAZA, ValePlaza, VALE_PLAZA_FROM_VALE),
    SCENE_EXIT(VALE_PROLOGUE_TO_VALE_KRADENS_COTTAGE_NORTH, ValeKradensCottage,
               VALE_KRADENS_COTTAGE_FROM_VALE_NORTH),
    SCENE_EXIT(VALE_PROLOGUE_TO_VALE_KRADENS_COTTAGE_SOUTH, ValeKradensCottage,
               VALE_KRADENS_COTTAGE_FROM_VALE_SOUTH),
    SCENE_EXIT(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_EAST, CottageRoomsA,
               COTTAGE_ROOMS_A_FROM_VALE_EAST),
    SCENE_EXIT(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_SOUTH, CottageRoomsA,
               COTTAGE_ROOMS_A_FROM_VALE_SOUTH),
    SCENE_EXIT(VALE_PROLOGUE_TO_COTTAGE_ROOMS_B_ISAACS_HOUSE, CottageRoomsB,
               COTTAGE_ROOMS_B_FROM_VALE),
    SCENE_EXIT(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_NORTH, CottageRoomsA,
               COTTAGE_ROOMS_A_FROM_VALE_NORTH),
    SCENE_EXIT(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST, CottageRoomsA,
               COTTAGE_ROOMS_A_FROM_VALE_WEST),
    SCENE_EXIT(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST_SECOND_DOOR, CottageRoomsA,
               COTTAGE_ROOMS_A_FROM_VALE_WEST),
    SCENE_EXIT(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST_SANCTUM_DOOR,
               CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_WEST),
    SCENE_EXIT(VALE_PROLOGUE_TO_COTTAGE_ROOMS_B_WEST, CottageRoomsB,
               COTTAGE_ROOMS_B_FROM_VALE_PROLOGUE_WEST),
    SCENE_EXIT(VALE_PROLOGUE_TO_COTTAGE_ROOMS_B_EAST, CottageRoomsB,
               COTTAGE_ROOMS_B_FROM_VALE_PROLOGUE_EAST),
    SCENE_EXIT(VALE_PROLOGUE_TO_BOULDER_PASSED, ValePrologue,
               VALE_PROLOGUE_FROM_BOULDER_PASSED),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gValePrologue_ActorPlacements[] = {
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_KYLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(437), 0, PIXELS(762), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(322), 0, PIXELS(1433), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(144), 0, PIXELS(1070), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(276), 0, PIXELS(1270), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEST_OF_VALUABLES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(420), 0, PIXELS(619), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { GARET_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(396), 0, PIXELS(619), FACING_EAST, TALK_FACING_PARTY, 0 },
    { FELIX_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_GREEN_SHIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WOMAN_WITH_PINK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { JENNA_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gValePrologue_BoulderWatchers[] = {
    { SPRITE_BLANK, CONDITION_ALWAYS, 7,
        PIXELS(124), 0, PIXELS(375), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(85), 0, PIXELS(361), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(135), 0, PIXELS(363), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, 7,
        PIXELS(100), 0, PIXELS(375), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(124), 0, PIXELS(375), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(100), 0, PIXELS(375), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct Trigger gValePrologue_Triggers[] = {
    {TRIGGER_STEP, 2, CONDITION_ALWAYS,
     EVENT(ValePrologue_LeaveNorthwestToValePlaza)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS,
     EVENT(ValePrologue_LeaveToValeKradensCottageNorth)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS,
     EVENT(ValePrologue_LeaveToValeKradensCottageSouth)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(ValePrologue_LeaveToValePlaza)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
     EVENT(ValePrologue_LeaveToCottageRoomsAEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS,
     EVENT(ValePrologue_LeaveToCottageRoomsA)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS,
     EVENT(ValePrologue_LeaveToIsaacsHouse)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS,
     EVENT(ValePrologue_LeaveToCottageRoomsANorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS,
     EVENT(ValePrologue_LeaveToCottageRoomsAWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS,
     EVENT(ValePrologue_UnusedLeaveToCottageRoomsAWestBySecondDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS,
     EVENT(ValePrologue_LeaveToCottageRoomsBWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 13, CONDITION_ALWAYS,
     EVENT(ValePrologue_LeaveToCottageRoomsBEast)},
    {TRIGGER_TALK, ACTOR_GARET_CHILD, CONDITION_ALWAYS,
     EVENT(ValePrologue_TalkToGaret)},
    {TRIGGER_TALK, ACTOR_KYLE, CONDITION_ALWAYS,
     EVENT(ValePrologue_TalkAboutSavingFelix)},
    {TRIGGER_TALK, ACTOR_JENNAS_FATHER, CONDITION_ALWAYS,
     MSGNO(MsgValeHangOnFelixHelpIsOn)},
    {TRIGGER_TALK, ACTOR_JENNAS_MOTHER, CONDITION_ALWAYS,
     MSGNO(MsgValeDontLetGoFelix)},
    {TRIGGER_TALK, ACTOR_DORA, CONDITION_ALWAYS,
     MSGNO(MsgValeNoItCantBeKyleIt)},
    {TRIGGER_TALK, ACTOR_JENNA_CHILD, CONDITION_ALWAYS,
     MSGNO(MsgValeMomDadFelixPleaseDontLeave)},
    {TRIGGER_TALK, ACTOR_EXHAUSTED_STRONGMAN, CONDITION_ALWAYS,
     EVENT(ValePrologue_StrongmanTriesToGoForHelp)},
    {TRIGGER_TALK, ACTOR_FIRST_ROCK, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {TRIGGER_TALK, ACTOR_FIRST_ROCK + 1, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {TRIGGER_TALK, ACTOR_FIRST_ROCK + 2, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {TRIGGER_TALK, ACTOR_FIRST_ROCK + 3, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {TRIGGER_TALK, ACTOR_FIRST_ROCK + 4, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {TRIGGER_TALK, ACTOR_FIRST_ROCK + 5, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {TRIGGER_TALK, ACTOR_FIRST_ROCK + 6, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {TRIGGER_TALK, ACTOR_FIRST_ROCK + 7, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {TRIGGER_STEP, 15, CONDITION_ALWAYS, EVENT(ValePrologue_StepOntoBridge)},
    {TRIGGER_STEP, 14, CONDITION_ALWAYS, EVENT(ValePrologue_StepOffBridge)},
    {TRIGGER_STEP, 41, CONDITION_ALWAYS, EVENT(ValePrologue_FirstRockFalls)},
    {TRIGGER_STEP, 42, CONDITION_ALWAYS, EVENT(ValePrologue_SecondRockFalls)},
    {TRIGGER_STEP, 43, CONDITION_ALWAYS, EVENT(ValePrologue_ThirdRockFalls)},
    {TRIGGER_STEP, 44, CONDITION_ALWAYS, EVENT(ValePrologue_FourthRockFalls)},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS,
     EVENT(ValePrologue_HearGaretStruggling)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS,
     EVENT(ValePrologue_GaretCallsAfterIsaac)},
    {TRIGGER_STEP, 32, CONDITION_ALWAYS,
     EVENT(ValePrologue_UnusedGaretJoinsIsaac)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(ValePrologue_WarnAboutMtAleph)},
    {TRIGGER_STEP, 34, CONDITION_ALWAYS, EVENT(ValePrologue_WatchBoulderFall)},
    {TRIGGER_STEP, 35, CONDITION_ALWAYS,
     EVENT(ValePrologue_WatchAttemptToSaveFelix)},
    {TRIGGER_STEP, 36, CONDITION_ALWAYS,
     EVENT(ValePrologue_BoulderSweepsThemAway)},
    {TRIGGER_STEP, 16, FLAG_VALE_STORM_THIRD_MONSTER_FOUGHT,
     EVENT(ValePrologue_FightThirdMonster)},
    {TRIGGER_STEP, 17, FLAG_VALE_STORM_FOURTH_MONSTER_FOUGHT,
     EVENT(ValePrologue_FightFourthMonster)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 24, CONDITION_ALWAYS,
     EVENT(ValePrologue_SearchBrokenDoor)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 19, CONDITION_ALWAYS,
     EVENT(ValePrologue_SearchValuablesChest)},
    {TRIGGERS_END},
};

// The doors' cell animations: two pictures of a door opening, from the map's
// cells below the village, each shown for two frames. The four kinds of door
// are the day map's (vale.c), and each table takes the name of a door it
// opens.

/// The Sanctum's door (ValePrologue_UnusedOpenSanctumDoor).
u16 gValePrologue_SanctumDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(0, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(2, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The door of Isaac's house, and of Garet's and the house by the river.
u16 gValePrologue_IsaacsDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(0, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(2, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The door of the house of Jenna's grandparents, and two more of the kind.
u16 gValePrologue_GrandparentsDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(4, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(6, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The door VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST goes through.
u16 gValePrologue_WestHouseDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(4, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(6, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};

/// Shuts a door of gValePrologue_IsaacsDoorOpens's kind, with the shut
/// picture of Isaac's door, after Dora and Jenna come out of the house by the
/// river.
u16 gValePrologue_IsaacsDoorCloses[] = {
    MAPGRID_ANIMATION_STEP(0, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(50, 44, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};

/// Shuts a door of gValePrologue_GrandparentsDoorOpens's kind behind Dora
/// and Jenna as they go into the house by the river.
u16 gValePrologue_GrandparentsDoorCloses[] = {
    MAPGRID_ANIMATION_STEP(4, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(44, 108, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};
