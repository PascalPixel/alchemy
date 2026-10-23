/// @file
/// Vale on the stormy night (MapId_ValeStorm), the prologue, when Isaac and
/// Garet are children: a map of its own that stands in for MapId_Vale while
/// FLAG_VALE_STORM is set, so that the doors that lead to Vale by day lead
/// here. ValeStorm_Initialize sets the flag on every arrival, and vale_gate.c
/// clears it when the night ends.
///
/// The night is a chain of story flags. Each is set by one event and read by
/// ValeStorm_Initialize, which rebuilds the map at that stage on the next
/// arrival:
///
/// - FLAG_VALE_STORM_SENT_TO_PLAZA: EventScript_DoraSendsIsaacToThePlaza,
/// played on
///   the first arrival by VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA. Dora
///   and Kyle send Isaac on to the plaza and go to help; until the flag is set
///   the lightning waits for the cutscene.
/// - FLAG_VALE_STORM_FIRST_ROCK_FELL to FLAG_VALE_STORM_FOURTH_ROCK_FELL, with
///   the map section flags FLAG_VALE_STORM_FIRST_ROCK_PASSED to
///   FLAG_VALE_STORM_FOURTH_ROCK_PASSED: EventScript_FirstRockFalls to
///   EventScript_FourthRockFalls drop a rock across a path at four spots. The
///   _FELL flag puts back the rock and the metatile behaviors of the cells it
///   blocks; the _PASSED flag, cleared once the party leaves Vale, keeps the
///   script from running again.
/// - FLAG_VALE_STORM_BUMPED_INTO_VILLAGER: ValeStorm_BumpIntoShovingMan, the
///   first time Isaac tries the door of
///   VALE_STORM_TO_VALE_INTERIORS_SHOPS_SOUTH.
/// - FLAG_VALE_STORM_HEARD_GARET, then FLAG_VALE_STORM_GARET_JOINED: Garet is
///   trying to save his chest of valuables; talking to him, or walking off
///   past him, brings him into the party (ValeStorm_GaretLeavesHisChest) and
///   leaves the chest by his door. EventScript_UnusedGaretJoinsIsaac is a third
///   way that never plays. With Garet along, a fallen rock and the path north
///   to Mt. Aleph have him turn Isaac back.
/// - FLAG_VALE_BOULDER_FELL: EventScript_WatchBoulderFall, the elders holding
/// back
///   the Boulder with their Psynergy as it slips; from then on it rests where
///   it slipped to.
/// - FLAG_VALE_STORM_SAW_FELIX_HANGING_ON: EventScript_WatchAttemptToSaveFelix
/// and
///   ValeStorm_DoraGoesForHelp, Felix clinging on in the river. The plaza
///   (vale_gate.c) then sets FLAG_VALE_STORM_SENT_TO_AID_ELDERS and sends the
///   boys back.
/// - FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY: EventScript_BoulderSweepsThemAway
/// and
///   ValeStorm_ShowBoulderSweepingDownriver, the Boulder carrying Felix, his
///   parents and Kyle away down the river.
/// - FLAG_VALE_STORM_GARET_LEFT: ValeStorm_GaretFollowsIsaac, as Isaac leaves
///   for the plaza again; the night ends in vale_gate.c.
///
/// Every way out goes through Vale_RequestBackdropWarp (common/vale.h), which
/// stops the lightning first, and each door plays its opening cells before it;
/// gValeStorm_Warps gives each warp's destination. Two cells start a battle
/// once each (slots 2 and 3 of ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT).
///
/// Open questions:
/// - None.

#include "action_script.h"
#include "game_state.h"
#include "unit/import.h"
#include "unit/map/vale_storm.h"
#include "unit/map/common/effect_psynergy.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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
#include "task.h"
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
#include "objcmd.h"
#include "heap_slots.h"
#include "vram_cache.h"
#include "fixed_math.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "io_reg.h"
#include "fx_work.h"

static void EventScript_DoraSendsIsaacToThePlaza(void);
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
static void
ValeStorm_FlashPaletteAndSpawnEffects(struct ObjectEvent *objectEvent);
static void ValeStorm_SpawnEffectPair(struct ObjectEvent *parent);
static void ValeStorm_UpdateBoulderMapEffects(void);
static void ValeStorm_CycleBoulderPalette(void);
static void ValeStorm_CopyLeaderSpritePriority(void);

/// The placed object events, numbered in the order of
/// gValeStorm_ObjectEventTemplates; the elders holding back the Boulder
/// (gValeStorm_BoulderWatchers) follow them.
enum StormNightLocalId {
  /// The strongman who comes back to the river with Jenna, his Psynergy
  /// restored at the Psynergy Stone (vale_gate.c); after the Boulder has
  /// passed he tries to go for help and cannot. "I... I'll go!", then "It's no
  /// use. I can't move..."
  /// ★ Named from the game: "I'll go!" (MsgValeIllGo)
  LOCALID_EXHAUSTED_STRONGMAN = LOCALID_FIRST_PLACED,
  /// ★ Named from the game: "Dora, hurry!" (MsgValeDoraHurryBoulder)
  LOCALID_DORA,
  /// ★ Named from the game: "Let me help you, Kyle!" (MsgValeKyleAbleStop)
  LOCALID_KYLE,
  /// Eight huge rocks that Isaac "can't even budge": LOCALID_FIRST_ROCK to
  /// LOCALID_FIRST_ROCK + 3 fall across the paths (EventScript_FirstRockFalls
  /// to
  /// EventScript_FourthRockFalls, FLAG_VALE_STORM_FIRST_ROCK_FELL on),
  /// LOCALID_FIRST_ROCK + 4 and + 5 already lie there, and LOCALID_FIRST_ROCK +
  /// 6
  /// and + 7 lie there only once the Boulder has passed down the river
  /// (ValeStorm_ShowBoulderSweepingDownriver).
  /// ★ Named from the game: "can't even budge" (MsgValeRockCannotBeMoved)
  LOCALID_FIRST_ROCK,
  /// ★ Named from the game: "The Mt. Aleph Boulder is going to fall!"
  /// (MsgValeWake)
  LOCALID_BOULDER = LOCALID_FIRST_PLACED + 11, ///< drawn at twice its size
  /// A stout man who runs out of his house and shoves Isaac. "Huh? Isaac?"
  /// ★ Named from the game: "Sorry to shove you like that." (MsgValeHuh)
  LOCALID_SHOVING_MAN,
  /// The things Garet struggles to save. "If a rock hits me now, I'll lose
  /// both my chest and my life."
  /// ★ Named from the game: "both my chest and my life" (MsgValeRockHitsLose)
  LOCALID_GARETS_CHEST,
  /// ★ Named from the game: "Garet (child)" (MsgDebugGlyphName)
  LOCALID_GARET,
  /// ★ Named from the game: "Felix (child)" (MsgDebugGlyphName)
  LOCALID_FELIX, ///< in the river
  /// Jenna's father, a man with brown hair in a green shirt; Jenna calls him
  /// and her mother "Mom! Dad!". "Hang on, Felix! Help is on the way!"
  /// ★ Named from the game: "Mom! Dad!" (MsgValeMomDadBack)
  LOCALID_JENNAS_FATHER,
  /// Jenna's mother, a woman with pink hair in a white apron. "Don't let go,
  /// Felix!"
  /// ★ Named from the game: "Mom! Dad!" (MsgValeMomDadBack)
  LOCALID_JENNAS_MOTHER,
  /// ★ Named from the game: "Oh no! My brother!" (MsgValeNoBrother)
  LOCALID_JENNA,
  /// The Boulder's shadow (sprite 0x120, a black blob), drawn on alternate
  /// frames under the Boulder as it comes down the river, swelling and
  /// shrinking with its bounds (ValeStorm_UpdateBoulderShadow), and twice its
  /// size during the frozen flashes (ValeStorm_FlickerBoulderShadow).
  /// ★ Named from what the player sees: a flickering black shadow under the
  /// Boulder, EventScript_BoulderSweepsThemAway.gif (its render)
  LOCALID_BOULDER_SHADOW,
  // The elders holding back the Boulder with their Psynergy ("Go aid the
  // elders!", vale_gate.c), from gValeStorm_BoulderWatchers, which
  // EventScript_WatchBoulderFall places after the map's own object events and
  // removes.
  // Two invisible object events stand on the spots of LOCALID_ELDER_EAST and
  // LOCALID_ELDER_WEST, and some of the elders' lines come from them.
  /// The invisible object event on LOCALID_ELDER_EAST's spot that speaks for
  /// that elder,
  /// the elders' first and third lines. "Ahhh! The Boulder! It's falling!",
  /// then "If our Psynergy runs out, Vale will be destroyed!"
  /// ★ Named from the game: "It's falling!" (MsgValeTheBoulderIsFalling)
  LOCALID_ELDER_EAST_STAND_IN,
  /// The westernmost elder, drawn with the sprite of the young man with brown
  /// hair (as is LOCALID_ELDER_FAR_EAST), tells the boys to run. "Hurry, boys!
  /// Run!!! It's just a little farther!"
  /// ★ Named from the game: "Hurry, boys!" (MsgValeTheBoulderIsFalling)
  LOCALID_ELDER_FAR_WEST,
  /// The easternmost elder, the same sprite drawn mirrored. "We have to hold on
  /// until the villagers are safe!"
  /// ★ Named from the game: "hold on until the villagers are safe"
  /// (MsgValeTheBoulderIsFalling)
  LOCALID_ELDER_FAR_EAST,
  /// The invisible object event on LOCALID_ELDER_WEST's spot that speaks for
  /// that elder,
  /// the elders' second line. "It's too heavy! We can't hold it for long!"
  /// ★ Named from the game: "We can't hold it for long"
  /// (MsgValeTheBoulderIsFalling)
  LOCALID_ELDER_WEST_STAND_IN,
  /// The elders between the two are men with long brown hair and beards in
  /// long robes.
  /// ★ Named from the game: "Go aid the elders!" (MsgValeGoAidElders)
  LOCALID_ELDER_EAST, ///< east of LOCALID_ELDER_WEST, and drawn as that elder
                      ///< is
  /// ★ Named from the game: "Go aid the elders!" (MsgValeGoAidElders)
  LOCALID_ELDER_WEST
};

/// Animations of the night's own sprites beyond enum ObjectEventAnimation, as
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
  // (ValeStorm_ShowBoulderSweepingDownriver).
  ANIM_STRONGMAN_RUN = 5,    ///< in mid-stride (spr_116.png)
  ANIM_DORA_RUN = 9,         ///< in mid-stride (spr_031.png)
  ANIM_JENNA_RUN = 5,        ///< in mid-stride (spr_048.png)
  ANIM_ISAAC_RECOIL = 17,    ///< arms flung out (spr_018.png)
  ANIM_PARENT_RECOIL = 5,    ///< seen from behind, arms flung out
  ANIM_FELIX_CLING_STILL = 8 ///< ANIM_FELIX_CLING's first frame held
};

/// Vale's map section flags this map keeps while the party is in Vale.
enum StormNightMapSecFlag {
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
  /// The party has passed each rock's spot since coming to Vale, so its script
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

// Map flags, cleared on every ordinary arrival:
// - FLAG_MAP_TEMP_05: the door of Isaac's house, from which Dora and Isaac
//   came, stands open.
// - FLAG_MAP_TEMP_06: the stout man has run out of the door of
//   VALE_STORM_TO_VALE_INTERIORS_SHOPS_SOUTH, leaving it open.
// - FLAG_MAP_TEMP_10: the leader is on the upper path, whose metatile behaviors
//   are swapped until the leader steps off it.

static s32 sShakeDone;
static s32 sShakeShift;

/// The step of gValeStorm_LookAroundScript: the object event faces a random way
/// every 20 to 39 frames, as everyone looks about once the Boulder has gone by.
static s32
ValeStorm_RerollHeadingOnCountdown(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  s32 timer = objectEvent->timer;
  s32 countdown = (s16)timer;

  if (timer == 0) {
    objectEvent->facing = Engine_RandomNext();
    countdown = Math_RemainderUnsigned(Engine_RandomNext(), 20) + 20;
    objectEvent->timer = countdown;
  }
  objectEvent->timer = countdown - 1;
  return 1;
}

#include "common/object_pulse.h"

/// Returns where the party appears;
/// VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA is where Dora and Isaac come
/// out of the door of their house, from MapId_ValeInteriorsInn's
/// VALE_INTERIORS_INN_TO_VALE_STORM_WITH_DORA.
const struct MapWarpDest *ValeStorm_GetWarpDests(void) {
  return gValeStorm_WarpDests;
}

const struct MapWarpRegion *ValeStorm_GetRegions(void) { return NULL; }

const u32 *ValeStorm_GetWarps(void) { return gValeStorm_Warps; }

const struct ObjectEventTemplate *ValeStorm_GetObjectEventTemplates(void) {
  return gValeStorm_ObjectEventTemplates;
}

const struct MapEvent *ValeStorm_GetEvents(void) { return gValeStorm_Events; }

/// In Vale on the stormy night, swaps the upper path's metatile behaviors as
/// the leader steps onto it.
/// Reached by stepping on metatile behavior 15. Sets FLAG_MAP_TEMP_10,
/// which ValeStorm_Initialize reads to swap them again.
static void EventScript_SetPathCellOverride(void) {
  Engine_GameFlagSet(FLAG_MAP_TEMP_10);
  MapGrid_CopyCellAttributes(40, 84, 7, 4, 10, 84);
}

/// In Vale on the stormy night, restores the upper path's metatile behaviors as
/// the leader steps off it.
/// Reached by stepping on metatile behavior 14. Clears FLAG_MAP_TEMP_10.
static void EventScript_ClearPathCellOverride(void) {
  GameFlag_ClearBit(FLAG_MAP_TEMP_10);
  MapGrid_CopyCellAttributes(40, 89, 7, 4, 10, 84);
}

#include "common/vale.h"

/// Unused: on the stormy night, the Sanctum's door opens and the leader goes
/// into a house of Vale.
///
/// It takes VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SANCTUM_DOOR
/// (MapId_ValeInteriorsShops, at VALE_INTERIORS_SHOPS_FROM_VALE_WEST, the
/// house VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST and its second door lead to
/// as well). No map event names it, and nothing
/// else takes that warp.
UNUSED static void ValeStorm_UnusedOpenSanctumDoor(void) {
  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gValeStorm_SanctumDoorOpens, 45, 11);
  ObjectMotion_ResetAndSetPositionInMode2(LOCALID_PARTY_LEADER, 257, 420);
  FieldScript_WaitFrames(3);
  Vale_RequestBackdropWarp(
      VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SANCTUM_DOOR);
}

/// Leaves through a warp the warp table doesn't list. In Vale on the stormy
/// night, stepping on metatile behavior 2 requests warp 1, the one number the
/// warp table leaves out; Map_TakeWarp therefore falls back to the first warp's
/// map, Vale's MapId_ValeGate, at WARP_DEST_DEFAULT.
static void EventScript_LeaveByMissingWarp(void) {
  Sound_PlaySong(SE_MAP_EXIT);
  Vale_RequestBackdropWarp(1);
}

/// In Vale on the stormy night, leaves for the part of Vale around Kraden's
/// cottage.
/// Reached by stepping on metatile behavior 3; arrives at
/// VALE_KRADEN_FROM_VALE_NORTH.
static void EventScript_LeaveToValeKradenNorth(void) {
  Sound_PlaySong(SE_MAP_EXIT);
  Vale_RequestBackdropWarp(VALE_STORM_TO_VALE_KRADEN_NORTH);
}

/// In Vale on the stormy night, leaves for the part of Vale around Kraden's
/// cottage by its second path.
/// Reached by stepping on metatile behavior 4; arrives at
/// VALE_KRADEN_FROM_VALE_SOUTH.
static void EventScript_LeaveToValeKradenSouth(void) {
  Sound_PlaySong(SE_MAP_EXIT);
  Vale_RequestBackdropWarp(VALE_STORM_TO_VALE_KRADEN_SOUTH);
}

/// In Vale on the stormy night, leaves for the plaza, where the villagers have
/// gathered (MapId_ValeGate).
/// Reached by stepping on metatile behavior 5; arrives at VALE_GATE_FROM_VALE.
/// After the Boulder swept them away, Garet first resolves to go after Isaac
/// (ValeStorm_GaretFollowsIsaac).
static void EventScript_LeaveToValeGate(void) {
  Sound_PlaySong(SE_MAP_EXIT);
  if (GameFlag_Test(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) != 0 &&
      GameFlag_Test(FLAG_VALE_STORM_GARET_LEFT) == 0) {
    ValeStorm_GaretFollowsIsaac();
  }
  Vale_RequestBackdropWarp(VALE_STORM_TO_VALE_GATE);
}

/// In Vale on the stormy night, opens the door of Garet's house and goes in.
/// Reached by walking north into metatile behavior 6; arrives at
/// VALE_INTERIORS_SHOPS_FROM_VALE_EAST, the house where Garet's family lives.
static void EventScript_LeaveToValeInteriorsShopsEast(void) {
  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gValeStorm_IsaacsDoorOpens, 54, 32);
  ObjectMotion_ResetAndSetPositionInMode2(LOCALID_PARTY_LEADER, 406, 727);
  FieldScript_WaitFrames(3);
  Vale_RequestBackdropWarp(VALE_STORM_TO_VALE_INTERIORS_SHOPS_EAST);
}

/// A stout man runs out of a house and shoves Isaac. In Vale on the stormy
/// night, this is Isaac's first time at the door of
/// VALE_STORM_TO_VALE_INTERIORS_SHOPS_SOUTH; afterwards the door opens as
/// usual. "Huh? Isaac?" Reached by walking north into metatile behavior 7. The
/// shove plays until FLAG_VALE_STORM_BUMPED_INTO_VILLAGER or
/// FLAG_VALE_STORM_SECOND_ROCK_FELL is set, and sets FLAG_MAP_TEMP_06;
/// otherwise goes into a house of Vale (MapId_ValeInteriorsShops, at
/// VALE_INTERIORS_SHOPS_FROM_VALE_SOUTH).
static void EventScript_LeaveToValeInteriorsShops(void) {
  if (GameFlag_IsSet(FLAG_MAP_TEMP_06) == 0) {
    Script_PlaySong(SE_DOOR_OPEN);
    MapGrid_AnimateCells(gValeStorm_GrandparentsDoorOpens, 45, 39);
  }
  if (GameFlag_IsSet(FLAG_VALE_STORM_BUMPED_INTO_VILLAGER) == 0 &&
      GameFlag_IsSet(FLAG_VALE_STORM_SECOND_ROCK_FELL) == 0) {
    ValeStorm_BumpIntoShovingMan();
    GameFlag_Set(FLAG_MAP_TEMP_06);
  } else {
    ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 262, 805);
    FieldScript_WaitFrames(3);
    Vale_RequestBackdropWarp(VALE_STORM_TO_VALE_INTERIORS_SHOPS_SOUTH);
  }
}

/// Goes back into Isaac's house on the stormy night. It's the door he and Dora
/// came out of in Vale, and it opens unless they left it open.
/// Reached by walking north into metatile behavior 8; takes
/// VALE_STORM_TO_VALE_INTERIORS_INN_ISAACS_HOUSE, to
/// VALE_INTERIORS_INN_FROM_VALE in MapId_ValeInteriorsInn, where Vale's
/// VALE_TO_VALE_INTERIORS_INN leads by day.
static void EventScript_LeaveToValeInteriorsInn(void) {
  if (GameFlag_Test(FLAG_MAP_TEMP_05) == 0) {
    Sound_PlaySong(SE_DOOR_OPEN);
    MapGrid_PlayMetatileCopySequence(gValeStorm_IsaacsDoorOpens, 50, 44);
  }
  ObjectMotion_ResetAndSetPositionInMode2(LOCALID_PARTY_LEADER, 340, 888);
  FieldScript_WaitFrames(3);
  Vale_RequestBackdropWarp(VALE_STORM_TO_VALE_INTERIORS_INN_ISAACS_HOUSE);
}

/// In Vale on the stormy night, opens a door and goes in.
/// Reached by walking north into metatile behavior 9; arrives in a house of
/// Vale at VALE_INTERIORS_SHOPS_FROM_VALE_NORTH.
static void EventScript_LeaveToValeInteriorsShopsNorth(void) {
  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gValeStorm_GrandparentsDoorOpens, 49, 69);
  ObjectMotion_ResetAndSetPositionInMode2(LOCALID_PARTY_LEADER, 326, 1126);
  FieldScript_WaitFrames(3);
  Vale_RequestBackdropWarp(VALE_STORM_TO_VALE_INTERIORS_SHOPS_NORTH);
}

/// In Vale on the stormy night, opens a door and goes in.
/// Reached by walking north into metatile behavior 10; takes
/// VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST, to
/// VALE_INTERIORS_SHOPS_FROM_VALE_WEST in MapId_ValeInteriorsShops, where
/// Vale's VALE_TO_VALE_INTERIORS_SHOPS_WEST leads by day.
static void EventScript_LeaveToValeInteriorsShopsWest(void) {
  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gValeStorm_WestHouseDoorOpens, 52, 76);
  ObjectMotion_ResetAndSetPositionInMode2(LOCALID_PARTY_LEADER, 374, 1238);
  FieldScript_WaitFrames(3);
  Vale_RequestBackdropWarp(VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST);
}

/// Unused: its row answers metatile behavior 11, which no cell of the stormy
/// night's layout (layout/layout_000) carries. It would have opened Isaac's
/// door, as EventScript_LeaveToValeInteriorsInnWest does for behavior 12,
/// walked the leader to the same spot and taken
/// VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SECOND_DOOR into the house
/// EventScript_LeaveToValeInteriorsShopsWest leads to
/// (VALE_INTERIORS_SHOPS_FROM_VALE_WEST).
static void EventScript_UnusedLeaveToValeInteriorsShopsWestBySecondDoor(void) {
  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gValeStorm_IsaacsDoorOpens, 35, 74);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 102, 1206);
  FieldScript_WaitFrames(3);
  Vale_RequestBackdropWarp(VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SECOND_DOOR);
}

/// In Vale on the stormy night, opens a door and goes in.
/// Reached by walking north into metatile behavior 12; arrives in another part
/// of Vale at VALE_INTERIORS_INN_FROM_VALE_STORM_WEST.
static void EventScript_LeaveToValeInteriorsInnWest(void) {
  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gValeStorm_IsaacsDoorOpens, 35, 73);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 102, 1206);
  FieldScript_WaitFrames(3);
  Vale_RequestBackdropWarp(VALE_STORM_TO_VALE_INTERIORS_INN_WEST);
}

/// In Vale on the stormy night, opens a door and goes in.
/// Reached by walking north into metatile behavior 13; arrives in another part
/// of Vale at VALE_INTERIORS_INN_FROM_VALE_STORM_EAST.
static void EventScript_LeaveToValeInteriorsInnEast(void) {
  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gValeStorm_GrandparentsDoorOpens, 38, 72);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 146, 1182);
  FieldScript_WaitFrames(3);
  Vale_RequestBackdropWarp(VALE_STORM_TO_VALE_INTERIORS_INN_EAST);
}

/// Arriving in Vale on the stormy night: restores the fallen rocks, the river
/// scene and the Boulder from their flags, starts the storm, and plays the
/// scene with Dora and Kyle on the first arrival by
/// VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA. Called by Map_Run once
/// the object events are placed. Sets FLAG_VALE_STORM; after a battle
/// (FLAG_KEEP_PARTY_POSITION) the two doors close again. Garet, until he joins,
/// is shown struggling with his things.
s32 ValeStorm_Initialize(void) {
  Map_QueueMusic(SE_170);
  Engine_ObjectEventSetPosition(LOCALID_FELIX, 0, 0);
  if (GameFlag_Test(FLAG_KEEP_PARTY_POSITION) != 0) {
    GameFlag_ClearBit(FLAG_MAP_TEMP_05);
    GameFlag_ClearBit(FLAG_MAP_TEMP_06);
  }
  // Restore the rocks that have already fallen.
  if (GameFlag_Test(FLAG_VALE_STORM_FIRST_ROCK_FELL) != 0) {
    ObjectEvent_SetPosition(LOCALID_FIRST_ROCK, PIXELS(334), PIXELS(932));
    ValeStorm_ApplyFirstRockCollision();
  }
  if (GameFlag_Test(FLAG_VALE_STORM_SECOND_ROCK_FELL) != 0) {
    ObjectEvent_SetPosition(LOCALID_FIRST_ROCK + 1, PIXELS(224), PIXELS(872));
    ValeStorm_ApplySecondRockCollision();
  }
  if (GameFlag_Test(FLAG_VALE_STORM_THIRD_ROCK_FELL) != 0) {
    ObjectEvent_SetPosition(LOCALID_FIRST_ROCK + 2, PIXELS(64), PIXELS(703));
    ValeStorm_ApplyThirdRockCollision();
  }
  if (GameFlag_Test(FLAG_VALE_STORM_FOURTH_ROCK_FELL) != 0) {
    ObjectEvent_SetPosition(LOCALID_FIRST_ROCK + 3, PIXELS(432), PIXELS(1147));
    ValeStorm_ApplyFourthRockCollision();
  }
  // The rocks are big, so the leader can talk to them from further off: the
  // talk target search counts their distance at 10/13 (OBJ_EVENT_NEAR_TALK).
  ObjectEvent_GetById(LOCALID_FIRST_ROCK)->collisionFlags |=
      OBJ_EVENT_NEAR_TALK;
  ObjectEvent_GetById(LOCALID_FIRST_ROCK + 1)->collisionFlags |=
      OBJ_EVENT_NEAR_TALK;
  ObjectEvent_GetById(LOCALID_FIRST_ROCK + 2)->collisionFlags |=
      OBJ_EVENT_NEAR_TALK;
  ObjectEvent_GetById(LOCALID_FIRST_ROCK + 3)->collisionFlags |=
      OBJ_EVENT_NEAR_TALK;
  ObjectEvent_GetById(LOCALID_FIRST_ROCK + 4)->collisionFlags |=
      OBJ_EVENT_NEAR_TALK;
  ObjectEvent_GetById(LOCALID_FIRST_ROCK + 5)->collisionFlags |=
      OBJ_EVENT_NEAR_TALK;
  ObjectEvent_GetById(LOCALID_FIRST_ROCK + 6)->collisionFlags |=
      OBJ_EVENT_NEAR_TALK;
  ObjectEvent_GetById(LOCALID_FIRST_ROCK + 7)->collisionFlags |=
      OBJ_EVENT_NEAR_TALK;
  // Garet walks with the party once he has joined.
  if (GameFlag_Test(FLAG_VALE_STORM_GARET_JOINED) != 0) {
    Engine_ObjectEventSetPosition(LOCALID_GARET, 0, 0);
  }
  {
    struct ObjectEvent *objectEvent;

    objectEvent = ObjectEvent_GetById(LOCALID_BOULDER);
    objectEvent->scaleX = 2 * Q16_ONE;
    objectEvent->scaleY = 2 * Q16_ONE;
  }
  // The Boulder lies where it fell, or waits on the mountainside.
  if (GameFlag_Test(FLAG_VALE_BOULDER_FELL) != 0) {
    ObjectEvent_SetPosition(LOCALID_BOULDER, PIXELS(114), PIXELS(333));
  } else {
    ObjectEvent_SetActionScript(LOCALID_BOULDER, gValeStorm_BoulderWaits);
  }
  // Restore the riverbank cast at the saved stage of the storm.
  if (GameFlag_Test(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) != 0) {
    s32 facing;
    const s32 *table;
    ValeStorm_RebuildRiverbank();
    ObjectEvent_SetPosition(LOCALID_DORA, PIXELS(165), PIXELS(1229));
    {
      struct ObjectEvent *objectEvent;

      objectEvent = ObjectEvent_GetById(LOCALID_DORA);
      facing = FACING_NORTHEAST;
      objectEvent->facing = facing;
      objectEvent->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
      table = gValeStorm_SquashAndStretchScript;
      objectEvent->work = 1;
      ObjectEvent_SetActionScript(LOCALID_DORA, table);
    }
    Engine_ObjectEventSetPosition(LOCALID_JENNA, PIXELS(165), PIXELS(1254));
    {
      struct ObjectEvent *objectEvent;

      objectEvent = ObjectEvent_GetById(LOCALID_JENNA);
      objectEvent->facing = facing;
      objectEvent->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
      objectEvent->work = 2;
      ObjectEvent_SetActionScript(LOCALID_JENNA, table);
    }
    Engine_ObjectEventSetPosition(LOCALID_GARET, PIXELS(152), PIXELS(1285));
    {
      struct ObjectEvent *objectEvent;

      objectEvent = ObjectEvent_GetById(LOCALID_GARET);
      objectEvent->facing = facing;
      objectEvent->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
      objectEvent->work = 3;
      ObjectEvent_SetActionScript(LOCALID_GARET, table);
    }
    Engine_ObjectEventSetPosition(LOCALID_EXHAUSTED_STRONGMAN, PIXELS(184),
                                  PIXELS(1304));
    {
      struct ObjectEvent *objectEvent;

      objectEvent = ObjectEvent_GetById(LOCALID_EXHAUSTED_STRONGMAN);
      objectEvent->facing = facing;
      objectEvent->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
      objectEvent->work = 4;
      ObjectEvent_SetActionScript(LOCALID_EXHAUSTED_STRONGMAN, table);
    }
    Engine_ObjectEventSetAnimation(LOCALID_EXHAUSTED_STRONGMAN,
                                   ANIM_STRONGMAN_SPRAWLED);
    ObjectEvent_GetById(LOCALID_GARET)->priorityFlags &=
        ~OBJ_EVENT_PRIORITY_AUTOMATIC;
    ObjectEvent_GetById(LOCALID_EXHAUSTED_STRONGMAN)->priorityFlags &=
        ~OBJ_EVENT_PRIORITY_AUTOMATIC;
    FieldScheduler_AddCallback(ValeStorm_CopyLeaderSpritePriority,
                               TASK_PRIORITY_MAP);
    Engine_ObjectEventSetPosition(LOCALID_JENNAS_FATHER, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_JENNAS_MOTHER, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_FELIX, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_BOULDER, 0, 0);
    if (GameFlag_Test(FLAG_VALE_STORM_GARET_LEFT) != 0) {
      Engine_ObjectEventSetPosition(LOCALID_GARET, 0, 0);
    }
    // While they try to save Felix: Kyle and Jenna's parents at the river,
    // Felix in it.
  } else if (GameFlag_Test(FLAG_VALE_STORM_SAW_FELIX_HANGING_ON) != 0) {
    const s32 *table;
    ObjectEvent_SetPosition(LOCALID_KYLE, PIXELS(192), PIXELS(1214));
    ObjectEvent_FaceDirection(LOCALID_KYLE, FACING_SOUTHEAST, 0);
    Engine_ObjectEventSetAnimation(LOCALID_KYLE, ANIM_KYLE_REACH_OUT);
    {
      struct ObjectEvent *objectEvent;

      objectEvent = ObjectEvent_GetById(LOCALID_KYLE);
      objectEvent->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    }
    table = gValeStorm_SquashAndStretchScript;
    ObjectEvent_SetActionScript(LOCALID_KYLE, table);
    ObjectEvent_SetPosition(LOCALID_JENNAS_FATHER, PIXELS(227), PIXELS(1214));
    ObjectEvent_FaceDirection(LOCALID_JENNAS_FATHER, FACING_SOUTH, 0);
    Engine_ObjectEventSetAnimation(LOCALID_JENNAS_FATHER,
                                   ANIM_PARENT_REACH_OUT);
    {
      struct ObjectEvent *objectEvent;

      objectEvent = ObjectEvent_GetById(LOCALID_JENNAS_FATHER);
      objectEvent->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    }
    ObjectEvent_SetActionScript(LOCALID_JENNAS_FATHER, table);
    ObjectEvent_SetPosition(LOCALID_JENNAS_MOTHER, PIXELS(247), PIXELS(1214));
    ObjectEvent_FaceDirection(LOCALID_JENNAS_MOTHER, FACING_SOUTH, 0);
    Engine_ObjectEventSetAnimation(LOCALID_JENNAS_MOTHER,
                                   ANIM_PARENT_REACH_OUT);
    {
      struct ObjectEvent *objectEvent;

      objectEvent = ObjectEvent_GetById(LOCALID_JENNAS_MOTHER);
      objectEvent->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    }
    ObjectEvent_SetActionScript(LOCALID_JENNAS_MOTHER, table);
    ObjectEvent_SetPosition(LOCALID_FELIX, PIXELS(243), PIXELS(1277));
    ObjectMotion_ArmCallback(LOCALID_FELIX, FACING_NORTH, 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_FELIX), 0);
    Engine_ObjectEventSetPosition(LOCALID_FIRST_ROCK + 6, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_FIRST_ROCK + 7, 0, 0);
  } else {
    Engine_ObjectEventSetPosition(LOCALID_FIRST_ROCK + 6, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_FIRST_ROCK + 7, 0, 0);
  }
  // The lightning starts at once, except before Dora and Kyle's scene.
  if (gGameState.warpDest != VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA ||
      GameFlag_Test(FLAG_VALE_STORM_SENT_TO_PLAZA) != 0) {
    Weather_StartRain();
    Weather_StartLightning();
  }
  if (GameFlag_Test(FLAG_MAP_TEMP_10) != 0) {
    EventScript_SetPathCellOverride();
  }
  Engine_GameFlagSet(FLAG_VALE_STORM);
  MapGrid_CopyCellAttributes(29, 24, 1, 2, 26, 46);
  MapGrid_CopyCellAttributes(29, 25, 1, 1, 27, 46);
  MapGrid_CopyCellAttributes(29, 25, 1, 1, 28, 46);
  MapGrid_CopyCellAttributes(19, 90, 1, 1, 20, 88);
  MapGrid_CopyCellAttributes(19, 90, 1, 1, 20, 89);
  {
    struct ObjectEvent *objectEvent;

    objectEvent = ObjectEvent_GetById(LOCALID_GARETS_CHEST);
    objectEvent->motionFlags = 0;
    objectEvent->y.fixed = PIXELS(192);
    objectEvent->collisionFlags = OBJ_EVENT_NOT_TARGETED;
    ObjectEvent_SetSpriteFlags(objectEvent, 0);
  }
  WaitFrames(1);
  if (GameFlag_Test(FLAG_VALE_STORM_SENT_TO_PLAZA) == 0 &&
      gGameState.warpDest == VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA) {
    EventScript_DoraSendsIsaacToThePlaza();
    return 0;
  }
  // Garet drags his chest out of his house until he joins, then the chest
  // stands by the door.
  Engine_ObjectEventSetAnimation(LOCALID_FELIX, ANIM_FELIX_CLING);
  if (GameFlag_Test(FLAG_VALE_STORM_GARET_JOINED) == 0) {
    FieldScript_Start();
    ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_ATTACH);
    ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(400), PIXELS(611));
    ObjectEvent_SetPosition(LOCALID_GARETS_CHEST, PIXELS(424), PIXELS(627));
    ObjectEvent_WalkTo(LOCALID_GARET, 400, 619);
    ObjectEvent_WalkToAndWait(LOCALID_GARETS_CHEST, 424, 619);
    Engine_ObjectEventSetAnimation(LOCALID_GARETS_CHEST, ANIM_WALK);
    Engine_ObjectEventSetAnimation(LOCALID_GARET, ANIM_GARET_DRAG);
    FieldScript_End();
  } else {
    FieldScript_Start();
    ObjectEvent_SetPosition(LOCALID_GARETS_CHEST, PIXELS(424), PIXELS(627));
    ObjectEvent_WalkToAndWait(LOCALID_GARETS_CHEST, 424, 619);
    Engine_ObjectEventSetAnimation(LOCALID_GARETS_CHEST, ANIM_NOD);
    FieldScript_End();
  }
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 24;
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  Weather_ResumeLightning();
  return 0;
}

/// In Vale on the stormy night, Dora comes out into the storm with Isaac to
/// Kyle, and the two send Isaac on to the plaza while they go to help.
/// "Kyle... Will they be able to stop the Boulder?"
/// Called by ValeStorm_Initialize on arrival by
/// VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA until
/// FLAG_VALE_STORM_SENT_TO_PLAZA is set. Sets it and
/// FLAG_MAP_TEMP_05, clears FLAG_SHOW_LOCATION_NAME, and
/// removes Dora and Kyle.
static void EventScript_DoraSendsIsaacToThePlaza(void) {
  s32 message;

  FieldScript_Start();
  Weather_StartRain();
  Weather_StartLightning();
  Weather_LastLightning();
  WaitFrames(60);
  // The camera looks down on the door of Isaac's house; Kyle waits outside.
  FieldCamera_SetSpeed(PIXELS(1) / 4, PIXELS(1) / 32);
  FieldCamera_MoveTo(PIXELS(316), PIXELS(160), PIXELS(880), 1);
  ObjectEvent_SetPosition(LOCALID_KYLE, PIXELS(294), PIXELS(868));
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 16;
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  Weather_ResumeLightning();
  // Dora and Isaac come out of the door.
  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gValeStorm_IsaacsDoorOpens, 50, 44);
  ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ATTACH);
  ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_KYLE, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetPosition(LOCALID_DORA, PIXELS(342), PIXELS(890));
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 342, 905);
  Map_PlayQueuedMusic();
  ObjectEvent_WalkTo(LOCALID_DORA, 296, 905);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(342), PIXELS(890));
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 342, 890);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 342, 905);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 318, 905);
  Engine_ObjectEventSetAnimation(LOCALID_DORA, ANIM_STAND);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 1);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTH, 60);
  // Dora: "Kyle... Will they be able to stop the Boulder?" Kyle: "I don't think
  // so..." and tells the two to take refuge in the plaza.
  message = MsgValeKyleAbleStop;
  FieldScript_SetMessageId(message);
  Script_ShowMessage(LOCALID_DORA, 0);
  ObjectEvent_WalkToAndWait(LOCALID_KYLE, 294, 838);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetAnimationAndWait(LOCALID_KYLE, ANIM_SHAKE_HEAD);
  Script_ShowMessage(LOCALID_KYLE, 0);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_KYLE, FACING_SOUTH, 0);
  Script_ShowMessageAndWait(LOCALID_KYLE, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTH, 10);
  // "Aren't you coming?" "I need to help evacuate the other villagers."
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_KYLE, ANIM_SHAKE_HEAD);
  Script_ShowMessage(LOCALID_KYLE, 0);
  ObjectEvent_SetAttachedEffect(LOCALID_DORA, ATTACHED_EFFECT_IN_FRONT |
                                                  ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_EAST, 50);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTH, 10);
  // Dora runs to Kyle: "Let me help you, Kyle!" "It's too dangerous, Dora."
  ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 289, 883);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTHEAST, 0);
  Script_ShowMessage(LOCALID_DORA, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KYLE, 2);
  Script_ShowMessage(LOCALID_KYLE, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_DORA, ANIM_SHAKE_HEAD);
  // "Isaac is old enough to get to the plaza on his own."
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST, 10);
  // "You can find your way, can't you, Isaac?" She asks again on every no:
  // "You're a big boy now, Isaac."
  FieldScript_SetMessageId((message + 8));
  Script_OpenMessage(LOCALID_DORA, 0);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 302, 905);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  while (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER,
                                                      0) == 1) {
    ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 1);
    FieldScript_SetMessageId(MsgValeBigBoyWhy);
    Script_OpenMessage(LOCALID_DORA, 0);
  }
  ObjectMotion_SetAnimationAndWait(LOCALID_DORA, ANIM_NOD);
  // "You know the way--just go south to get to the plaza. Be careful!"
  FieldScript_SetMessageId(MsgValeKnowWayGo);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  // Kyle and Dora run north toward the Boulder and out of sight.
  ObjectEvent_SetSpeed(LOCALID_KYLE, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_WalkTo(LOCALID_KYLE, 297, 750);
  FieldScript_WaitFrames(10);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 297, 750);
  ObjectEvent_SetPosition(LOCALID_DORA, 0, 0);
  ObjectEvent_SetPosition(LOCALID_KYLE, 0, 0);
  Engine_ObjectEventSetAnimation(LOCALID_KYLE, ANIM_STAND);
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_CHEST, ANIM_WALK);
  Engine_ObjectEventSetAnimation(LOCALID_GARET, ANIM_GARET_DRAG);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  GameFlag_Set(FLAG_VALE_STORM_SENT_TO_PLAZA);
  GameFlag_Set(FLAG_MAP_TEMP_05);
  FieldScript_End();
}

/// In Vale on the stormy night, walking past: the first rock
/// (LOCALID_FIRST_ROCK) falls across the path, and Garet, once with Isaac, says
/// they cannot get around it.
/// Reached by stepping on metatile behavior 41 until
/// FLAG_VALE_STORM_FIRST_ROCK_PASSED is set, which this sets. Sets
/// FLAG_VALE_STORM_FIRST_ROCK_FELL as the rock falls; with Garet along, before
/// the Boulder swept Felix, his parents and Kyle away, his first comment on any
/// rock sets FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH.
static void EventScript_FirstRockFalls(void) {
  struct ObjectEvent *rock;
  struct ObjectEvent *leader;
  s32 targetHeight;
  s32 walkZ;
  struct Sprite *rockSprite;

  if (GameFlag_IsSet(FLAG_VALE_STORM_FIRST_ROCK_PASSED) != 0) {
  } else {
    FieldScript_Start();
    if (GameFlag_IsSet(FLAG_VALE_STORM_FIRST_ROCK_FELL) == 0) {
      rock = ObjectEvent_GetById(LOCALID_FIRST_ROCK);
      rockSprite = rock->sprite;
      // A rumble (SE_RUMBLE, then SE_145), and the rock bounces down from the
      // east.
      MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      Sound_PlaySong(SE_RUMBLE);
      WaitFrames(40);
      Sound_PlaySong(SE_145);
      rock->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
      rockSprite->image.priority = 1;
      ObjectEvent_SetPosition(LOCALID_FIRST_ROCK, PIXELS(473), PIXELS(932));
      rock->speed = PIXELS(3) / 2;
      rock->acceleration = PIXELS(3) / 2;
      targetHeight = (rock->y.fixed + PIXELS(240));
      rock->y.fixed += PIXELS(240);
      rock->targetY = targetHeight;
      rock->bounce = 0x6666;
      ObjectEvent_WalkToAndWait(LOCALID_FIRST_ROCK, 344, 932);
      rockSprite->image.priority = 3;
      rock->priorityFlags |= OBJ_EVENT_PRIORITY_AUTOMATIC;
      FieldScript_WaitFrames(40);
      // It lands: the rumble on player 3 fades and the shaking dies down.
      Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
      MapGrid_WaitForShakeToSettle();
      Map_PlayQueuedMusic();
      Engine_GameFlagSet(FLAG_VALE_STORM_FIRST_ROCK_FELL);
    }
    // The rock now blocks the path.
    ValeStorm_ApplyFirstRockCollision();
    Engine_GameFlagSet(FLAG_VALE_STORM_FIRST_ROCK_PASSED);
    if (GameFlag_Test(FLAG_VALE_STORM_GARET_JOINED) != 0) {
      if (GameFlag_Test(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) == 0) {
        if (GameFlag_Test(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH) == 0) {
          // Garet comes out beside Isaac on whichever side of the rock he
          // stands.
          leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
          if (leader->y.fixed > PIXELS(128)) {
            walkZ = 0x396;
            ValeStorm_GaretCommentsOnBlockedPath(0x146, walkZ);
            ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 291, walkZ);
          } else {
            ValeStorm_GaretCommentsOnBlockedPath(0x14f, 0x3bd);
          }
          Engine_GameFlagSet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH);
        }
      }
    }
    FieldScript_End();
  }
}

/// Applies the metatile behaviors left by the first rock (LOCALID_FIRST_ROCK).
/// Called by EventScript_FirstRockFalls and, once the rock has fallen, by
/// ValeStorm_Initialize.
static void ValeStorm_ApplyFirstRockCollision(void) {
  MapGrid_CopyCellAttributes(29, 64, 1, 1, 21, 57);
  MapGrid_CopyCellAttributes(29, 64, 1, 1, 21, 58);
  MapGrid_CopyCellAttributes(29, 64, 1, 1, 22, 58);
  MapGrid_CopyCellAttributes(29, 64, 1, 1, 20, 58);
  MapGrid_CopyCellAttributes(28, 20, 1, 1, 20, 57);
}

/// In Vale on the stormy night, walking past: the second rock
/// (LOCALID_FIRST_ROCK + 1) falls down the path.
/// Reached by stepping on metatile behavior 42; as EventScript_FirstRockFalls
/// does the first, with FLAG_VALE_STORM_SECOND_ROCK_FELL and
/// FLAG_VALE_STORM_SECOND_ROCK_PASSED.
static void EventScript_SecondRockFalls(void) {
  struct ObjectEvent *rock;
  struct ObjectEvent *leader;
  s32 targetHeight;

  if (GameFlag_IsSet(FLAG_VALE_STORM_SECOND_ROCK_PASSED) != 0) {
  } else {
    FieldScript_Start();
    if (GameFlag_IsSet(FLAG_VALE_STORM_SECOND_ROCK_FELL) == 0) {
      rock = ObjectEvent_GetById(LOCALID_FIRST_ROCK + 1);
      // A rumble, then the rock rolls down the slope from the north-east.
      MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      Sound_PlaySong(SE_RUMBLE);
      WaitFrames(40);
      Sound_PlaySong(SE_145);
      ObjectEvent_SetPosition(LOCALID_FIRST_ROCK + 1, PIXELS(381), PIXELS(808));
      rock->speed = PIXELS(3) / 2;
      rock->acceleration = PIXELS(3) / 2;
      targetHeight = (rock->y.fixed + PIXELS(256));
      rock->y.fixed += PIXELS(256);
      rock->targetY = targetHeight;
      rock->bounce = 0x8000;
      ObjectEvent_WalkToAndWait(LOCALID_FIRST_ROCK + 1, 290, 833);
      ObjectMotion_SetActionVariant(LOCALID_FIRST_ROCK + 1, 1);
      ObjectEvent_WalkToAndWait(LOCALID_FIRST_ROCK + 1, 258, 852);
      ObjectMotion_SetActionVariant(LOCALID_FIRST_ROCK + 1, 2);
      ObjectEvent_WalkToAndWait(LOCALID_FIRST_ROCK + 1, 224, 872);
      FieldScript_WaitFrames(40);
      // It lands: the rumble on player 3 fades and the shaking dies down.
      Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
      MapGrid_WaitForShakeToSettle();
      Map_PlayQueuedMusic();
      Engine_GameFlagSet(FLAG_VALE_STORM_SECOND_ROCK_FELL);
    }
    ValeStorm_ApplySecondRockCollision();
    Engine_GameFlagSet(FLAG_VALE_STORM_SECOND_ROCK_PASSED);
    if (GameFlag_Test(FLAG_VALE_STORM_GARET_JOINED) != 0) {
      if (GameFlag_Test(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) == 0) {
        if (GameFlag_Test(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH) == 0) {
          // Garet comes out beside Isaac on whichever side of the rock he
          // stands.
          leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
          if (leader->y.fixed > PIXELS(128)) {
            ValeStorm_GaretCommentsOnBlockedPath(219, 0x34b);
            ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 179, 829);
          } else {
            ValeStorm_GaretCommentsOnBlockedPath(214, 0x38c);
            ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 219, 911);
          }
          Engine_GameFlagSet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH);
        }
      }
    }
    FieldScript_End();
  }
}

/// Applies the metatile behaviors left by the second rock (LOCALID_FIRST_ROCK +
/// 1). Called by EventScript_SecondRockFalls and, once the rock has fallen, by
/// ValeStorm_Initialize.
static void ValeStorm_ApplySecondRockCollision(void) {
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
/// (LOCALID_FIRST_ROCK + 2) falls down the path.
/// Reached by stepping on metatile behavior 43; as EventScript_FirstRockFalls
/// does the first, with FLAG_VALE_STORM_THIRD_ROCK_FELL and
/// FLAG_VALE_STORM_THIRD_ROCK_PASSED.
static void EventScript_ThirdRockFalls(void) {
  if (GameFlag_IsSet(FLAG_VALE_STORM_THIRD_ROCK_PASSED) == 0) {
    FieldScript_Start();
    if (GameFlag_IsSet(FLAG_VALE_STORM_THIRD_ROCK_FELL) == 0) {
      struct ObjectEvent *objectEvent = ObjectEvent_Get(LOCALID_FIRST_ROCK + 2);
      struct ObjectEvent *leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
      u16 priority = leader->sprite->image.priority;
      u8 flags = leader->priorityFlags;

      // A rumble, then the rock drops in from the west while Isaac is drawn
      // behind it.
      MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      Script_PlaySong(SE_RUMBLE);
      WaitFrames(40);
      Script_PlaySong(SE_145);
      ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 3);
      ObjectEvent_Get(LOCALID_PARTY_LEADER)->priorityFlags |=
          OBJ_EVENT_PRIORITY_UNDERFOOT;
      ObjectEvent_SetPosition(LOCALID_FIRST_ROCK + 2, 0, PIXELS(703));
      objectEvent->speed = PIXELS(3) / 2;
      objectEvent->acceleration = PIXELS(3) / 2;
      objectEvent->y.fixed += PIXELS(80);
      objectEvent->targetY = objectEvent->y.fixed;
      objectEvent->bounce = 0x8000;
      ObjectEvent_WalkToAndWait(LOCALID_FIRST_ROCK + 2, 64, 703);
      FieldScript_WaitFrames(40);
      // It lands: the rumble fades, and Isaac is drawn as before.
      Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
      MapGrid_WaitForShakeToSettle();
      Map_PlayQueuedMusic();
      GameFlag_Set(FLAG_VALE_STORM_THIRD_ROCK_FELL);
      ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, priority);
      ObjectEvent_Get(LOCALID_PARTY_LEADER)->priorityFlags |=
          OBJ_EVENT_PRIORITY_AUTOMATIC;
      leader->priorityFlags = flags;
    }
    ValeStorm_ApplyThirdRockCollision();
    GameFlag_Set(FLAG_VALE_STORM_THIRD_ROCK_PASSED);
    if (GameFlag_IsSet(FLAG_VALE_STORM_GARET_JOINED) != 0) {
      if (GameFlag_IsSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) == 0) {
        if (GameFlag_IsSet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH) == 0) {
          // Garet comes out beside Isaac on whichever side of the rock he
          // stands.
          if (ObjectEvent_Get(LOCALID_PARTY_LEADER)->z.fixed <= 0x2b4ffff) {
            ValeStorm_GaretCommentsOnBlockedPath(62, 0x29d);
            ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 27, 627);
          } else {
            ValeStorm_GaretCommentsOnBlockedPath(75, 0x2cb);
            ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 67, 757);
          }
          GameFlag_Set(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH);
        }
      }
    }
    FieldScript_End();
  }
}

/// Applies the metatile behaviors left by the third rock (LOCALID_FIRST_ROCK +
/// 2). Called by EventScript_ThirdRockFalls and, once the rock has fallen, by
/// ValeStorm_Initialize.
static void ValeStorm_ApplyThirdRockCollision(void) {
  MapGrid_CopyCellAttributes(29, 22, 1, 1, 3, 42);
  MapGrid_CopyCellAttributes(29, 21, 1, 1, 2, 42);
  MapGrid_CopyCellAttributes(29, 21, 1, 1, 4, 42);
  MapGrid_CopyCellAttributes(23, 20, 3, 1, 2, 43);
}

/// In Vale on the stormy night, walking past: the fourth rock
/// (LOCALID_FIRST_ROCK + 3) falls down the path.
/// Reached by stepping on metatile behavior 44; as EventScript_FirstRockFalls
/// does the first, with FLAG_VALE_STORM_FOURTH_ROCK_FELL and
/// FLAG_VALE_STORM_FOURTH_ROCK_PASSED.
static void EventScript_FourthRockFalls(void) {
  struct ObjectEvent *rock;
  struct ObjectEvent *leader;
  s32 targetHeight;

  if (GameFlag_IsSet(FLAG_VALE_STORM_FOURTH_ROCK_PASSED) != 0) {
  } else {
    FieldScript_Start();
    if (GameFlag_IsSet(FLAG_VALE_STORM_FOURTH_ROCK_FELL) == 0) {
      rock = ObjectEvent_GetById(LOCALID_FIRST_ROCK + 3);
      // A rumble, then the rock rolls in from the east.
      MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      Sound_PlaySong(SE_RUMBLE);
      WaitFrames(40);
      Sound_PlaySong(SE_145);
      ObjectEvent_SetPosition(LOCALID_FIRST_ROCK + 3, PIXELS(474),
                              PIXELS(1147));
      rock->speed = PIXELS(1);
      rock->acceleration = PIXELS(1);
      targetHeight = (rock->y.fixed + PIXELS(72));
      rock->y.fixed += PIXELS(72);
      rock->targetY = targetHeight;
      rock->bounce = 0x8000;
      ObjectMotion_SetPositionAndReset(LOCALID_FIRST_ROCK + 3, 432, 1147);
      FieldScript_WaitFrames(40);
      // It lands: the rumble on player 3 fades and the shaking dies down.
      Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
      MapGrid_WaitForShakeToSettle();
      Map_PlayQueuedMusic();
      Engine_GameFlagSet(FLAG_VALE_STORM_FOURTH_ROCK_FELL);
    }
    ValeStorm_ApplyFourthRockCollision();
    Engine_GameFlagSet(FLAG_VALE_STORM_FOURTH_ROCK_PASSED);
    if (GameFlag_Test(FLAG_VALE_STORM_GARET_JOINED) != 0) {
      if (GameFlag_Test(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) == 0) {
        if (GameFlag_Test(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH) == 0) {
          // Garet comes out beside Isaac on whichever side of the rock he
          // stands.
          leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
          if (leader->z.fixed <= 0x479ffff) {
            ValeStorm_GaretCommentsOnBlockedPath(0x19c, 0x460);
            ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 414, 1068);
          } else {
            ValeStorm_GaretCommentsOnBlockedPath(0x1bd, 0x494);
            ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 447, 1227);
          }
          Engine_GameFlagSet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH);
        }
      }
    }
    FieldScript_End();
  }
}

/// Applies the metatile behaviors left by the fourth rock (LOCALID_FIRST_ROCK +
/// 3). Called by EventScript_FourthRockFalls and, once the rock has fallen, by
/// ValeStorm_Initialize.
static void ValeStorm_ApplyFourthRockCollision(void) {
  MapGrid_CopyCellAttributes(29, 20, 1, 1, 26, 71);
  MapGrid_CopyCellAttributes(29, 20, 1, 1, 26, 70);
  MapGrid_CopyCellAttributes(29, 20, 1, 1, 27, 70);
  MapGrid_CopyCellAttributes(28, 21, 1, 1, 28, 71);
  MapGrid_CopyCellAttributes(28, 22, 1, 1, 27, 72);
}

/// Stops the leader at the door of VALE_STORM_TO_VALE_INTERIORS_SHOPS_SOUTH for
/// the first conversation of the night. A stout man runs out of the door,
/// knocks Isaac back and runs off to the plaza. "Huh? Isaac?" Called by
/// EventScript_LeaveToValeInteriorsShops. Sets
/// FLAG_VALE_STORM_BUMPED_INTO_VILLAGER.
static void ValeStorm_BumpIntoShovingMan(void) {
  s32 message;

  FieldScript_Start();
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 262, 810);
  ObjectEvent_SetPosition(LOCALID_SHOVING_MAN, PIXELS(262), PIXELS(805));
  ObjectEvent_WalkTo(LOCALID_SHOVING_MAN, 262, 825);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(1));
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 2, 0);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 282, 855);
  Engine_ObjectEventSetAnimation(LOCALID_SHOVING_MAN, ANIM_STAND);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 4, 0);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_SHOVING_MAN, 0);
  Map_PlayQueuedMusic();
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  ObjectEvent_ShowEmote(LOCALID_SHOVING_MAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        20);
  message = MsgValeHuh;
  FieldScript_SetMessageId(message);
  Script_ShowMessage(LOCALID_SHOVING_MAN, 0);
  FieldScript_WaitFrames(20);
  Script_AskYesNo(LOCALID_SHOVING_MAN, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SHOVING_MAN, 2);
  FieldScript_SetMessageId((message + 4));
  Script_ShowMessageAndWait(LOCALID_SHOVING_MAN, 0, 20);
  ObjectEvent_RunActionScriptAndWait(LOCALID_SHOVING_MAN,
                                     gValeStorm_ShovingManScript);
  GameFlag_Set(FLAG_VALE_STORM_BUMPED_INTO_VILLAGER);
  FieldScript_End();
}

/// In Vale on the stormy night, Isaac hears Garet struggling with his things.
/// "Ugh... Hrnghhh..."
/// Reached by stepping on metatile behavior 30, until Garet has joined. Sets
/// FLAG_VALE_STORM_HEARD_GARET.
static void EventScript_HearGaretStruggling(void) {
  if (GameFlag_IsSet(FLAG_VALE_STORM_HEARD_GARET) == 0) {
    if (GameFlag_IsSet(FLAG_VALE_STORM_GARET_JOINED) == 0) {
      FieldScript_Start();
      FieldScript_SetMessageId(MsgValeUghHrnghhh);
      Script_ShowMessageAndWait(LOCALID_GARET, 0, 20);
      ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                            EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 382, 619);
      ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
      FieldScript_WaitFrames(30);
      Script_ShowMessage(LOCALID_GARET, 0);
      GameFlag_Set(FLAG_VALE_STORM_HEARD_GARET);
      FieldScript_End();
    }
  }
}

/// In Vale on the stormy night, walking away from him: Garet calls after
/// Isaac, asking whether he will just leave him there.
/// Reached by stepping on metatile behavior 31 once Isaac has heard Garet and
/// before Garet joins; ValeStorm_GaretLeavesHisChest brings him along.
static void EventScript_GaretCallsAfterIsaac(void) {
  if (GameFlag_Test(FLAG_VALE_STORM_GARET_JOINED) == 0 &&
      GameFlag_Test(FLAG_VALE_STORM_HEARD_GARET) != 0) {
    FieldScript_Start();
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
    FieldScript_WaitFrames(20);
    FieldScript_SetMessageId(MsgValeDontLeaveMeHere);
    ValeStorm_GaretLeavesHisChest();
    FieldScript_End();
  }
}

/// In Vale on the stormy night, talking to Garet: whether to leave his things
/// behind, or, after the river, the Boulder.
/// Reached by talking to Garet: before he joins, ValeStorm_GaretLeavesHisChest
/// brings him along.
static void EventScript_TalkToGaret(void) {
  if (GameFlag_IsSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) != 0) {
    FieldScript_Start();
    ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
    FieldScript_WaitFrames(20);
    FieldScript_SetMessageId(MsgValeBoulderNeedGet);
    Script_ShowMessage(LOCALID_GARET, 0);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 10);
    FieldScript_End();
  } else {
    if (GameFlag_IsSet(FLAG_VALE_STORM_GARET_JOINED) == 0) {
      FieldScript_Start();
      FieldScript_SetMessageId(MsgValeWantDumpStuff);
      ValeStorm_GaretLeavesHisChest();
      FieldScript_End();
    }
  }
}

/// Garet asks the question the caller chose and, on either answer, drops his
/// chest and joins the party. A yes has him agree to forget his things, a no
/// has him decide a rock would cost him chest and life alike. Called by
/// EventScript_GaretCallsAfterIsaac and EventScript_TalkToGaret; sets
/// FLAG_VALE_STORM_GARET_JOINED.
static void ValeStorm_GaretLeavesHisChest(void) {
  struct ObjectEvent *leader;
  s32 agreedToLeaveBelongings;

  FieldMessage_ShowAtObject(LOCALID_GARET, 0);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
  agreedToLeaveBelongings = 0;
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    FieldScript_SetMessageId(MsgValeThinkForgetThings);
    agreedToLeaveBelongings = 1;
  } else {
    FieldScript_SetMessageId(MsgValeRockHitsLose);
  }
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageThenWait(LOCALID_GARET, 0, 40);
  ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_REMOVE);
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_CHEST, ANIM_NOD);
  Engine_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  if (agreedToLeaveBelongings != 0) {
    FieldScript_SetMessageId(MsgValeKnowRightOk);
  } else {
    FieldScript_SetMessageId(MsgValeRightDitchStuff);
  }
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  Engine_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectMotion_ResetAndSetPosition(LOCALID_GARET, leader->x.part.pixel,
                                     leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  Engine_ObjectEventSetPosition(LOCALID_GARET, 0, 0);
  FieldScript_PrepareObjectAndApplyValue(1, 1);
  Engine_GameFlagSet(FLAG_VALE_STORM_GARET_JOINED);
}

/// Unused: an earlier version of Garet joining Isaac on the stormy night,
/// which the game never plays.
///
/// Its row answers metatile behavior 32, but every way there crosses the cells
/// where Isaac hears Garet struggling (behavior 30,
/// EventScript_HearGaretStruggling) and where Garet calls after him and joins
/// (behavior 31, EventScript_GaretCallsAfterIsaac), so Garet has always joined
/// by then and the scene's first test, FLAG_VALE_STORM_GARET_JOINED, ends it.
/// Its lines appear nowhere else: Garet runs up
/// (gValeStorm_UnusedGaretRunsUpScript), heaves for breath, `*Hah* *Hah*`,
/// while gValeStorm_UnusedGaretPantingScript swells and shrinks his sprite, and
/// asks `Are you trapped, too?` before they leave together.
static void EventScript_UnusedGaretJoinsIsaac(void) {
  struct ObjectEvent *objectEvent;
  s32 message;

  if (GameFlag_IsSet(FLAG_VALE_STORM_GARET_JOINED) == 0) {
    FieldScript_Start();
    ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_REMOVE);
    message = MsgValeHey;
    FieldScript_SetMessageId(message);
    Script_ShowMessage(LOCALID_GARET, 0);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
    FieldCamera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
    FieldCamera_MoveTo(PIXELS(256), -1, PIXELS(588), 1);
    ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(2), PIXELS(1));
    ObjectEvent_RunActionScriptAndWait(LOCALID_GARET,
                                       gValeStorm_UnusedGaretRunsUpScript);
    ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
    FieldScript_WaitFrames(30);
    ObjectEvent_SetActionScript(LOCALID_GARET,
                                gValeStorm_UnusedGaretPantingScript);
    Script_ShowMessage(LOCALID_GARET, 0);
    objectEvent = ObjectEvent_Get(LOCALID_GARET);
    objectEvent->scaleY = Q16_ONE;
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
    FieldScript_WaitFrames(20);
    Script_AskYesNo(LOCALID_GARET, 0);
    FieldScript_WaitFrames(40);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
    FieldScript_SetMessageId((message + 5));
    Script_ShowMessageAndWait(LOCALID_GARET, 0, 20);
    ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
    Script_ShowMessage(LOCALID_GARET, 0);
    ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1), PIXELS(1) / 2);
    Engine_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
    objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (objectEvent != 0) {
      ObjectEvent_SetDestination(LOCALID_GARET, objectEvent->x.part.pixel,
                                 objectEvent->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
    ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
    FieldScript_PrepareObjectAndApplyValue(1, 1);
    Engine_ObjectEventSetAnimation(LOCALID_GARETS_CHEST, ANIM_NOD);
    GameFlag_Set(FLAG_VALE_STORM_GARET_JOINED);
    FieldScript_End();
  }
}

/// In Vale on the stormy night, Garet turns Isaac back from the path north,
/// which leads to Mt. Aleph.
/// Reached by stepping on metatile behavior 33, every time; Isaac is walked
/// back south.
static void EventScript_WarnAboutMtAleph(void) {
  struct ObjectEvent *leader;
  struct ObjectEvent *initialLeader;

  FieldScript_Start();
  initialLeader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (initialLeader != NULL) {
    ObjectEvent_SetPosition(LOCALID_GARET, initialLeader->x.fixed,
                            initialLeader->z.fixed);
  }
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 281, 507);
  ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(30);
  FieldScript_SetMessageId(MsgValeNorthLeadsToMtAleph);
  Script_ShowMessage(LOCALID_GARET, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH, 0);
  Script_ShowMessage(LOCALID_GARET, 0);
  Engine_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetDestination(LOCALID_GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 256, 517);
  FieldScript_End();
}

/// Brings Garet out to discuss a blocked path, then returns him to Isaac.
/// Called by the four rock scripts; x and y are the pixels where Garet stands.
static void ValeStorm_GaretCommentsOnBlockedPath(s32 x, s32 y) {
  struct ObjectEvent *leaderAtStart;
  s32 speed = PIXELS(1);
  s32 acceleration = (PIXELS(1) / 2);
  struct ObjectEvent *leaderAtEnd;

  leaderAtStart = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leaderAtStart != 0) {
    Engine_ObjectEventSetPosition(LOCALID_GARET, leaderAtStart->x.fixed,
                                  leaderAtStart->z.fixed);
  }
  ObjectMotion_SetSpeedParameters(LOCALID_GARET, speed, acceleration);
  ObjectMotion_SetPositionAndReset(LOCALID_GARET, x, y);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
  FieldScript_WaitFrames(20);
  FieldEffect_ConfigureLinkedObject(
      LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);
  FieldScript_SetMessageId(MsgValeCantGetAroundThisRock);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  leaderAtEnd = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leaderAtEnd != 0) {
    ObjectMotion_ResetAndSetPosition(LOCALID_GARET, leaderAtEnd->x.part.pixel,
                                     leaderAtEnd->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  Engine_ObjectEventSetPosition(LOCALID_GARET, 0, 0);
}

/// The Boulder slips toward the village and the elders tell the boys to run.
/// In Vale on the stormy night, the elders hold the Boulder with their
/// Psynergy as it slips.
/// "Ahhh! The Boulder! It's falling!"
/// Reached by stepping on metatile behavior 34 until FLAG_VALE_BOULDER_FELL is
/// set, which this sets. The elders and their two stand-ins
/// (gValeStorm_BoulderWatchers, LOCALID_ELDER_EAST_STAND_IN to
/// LOCALID_ELDER_WEST) are placed for the scene and removed after it.
static void EventScript_WatchBoulderFall(void) {
  u32 i;
  struct ObjectEvent *movingObjectEvent;
  s32 *shakeShift;
  struct ObjectEvent *objectEvent;
  TaskFunc updateEffects;
  TaskFunc cycleBoulderPalette;
  s32 boulderHeight;

  if (GameFlag_IsSet(FLAG_VALE_BOULDER_FELL) == 0) {
    FieldScript_Start();
    FieldScript_CallWithLastActiveObjectId(gValeStorm_BoulderWatchers);
    Map_StartCutscenePsynergy();
    WaitFrames(1);
    Script_PlaySong(SE_RUMBLE);
    MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    FieldScript_WaitFrames(30);
    MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
    Script_PlaySong(SE_145);
    FieldScript_WaitFrames(30);
    objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    if (objectEvent != NULL) {
      ObjectEvent_SetPosition(LOCALID_GARET, objectEvent->x.fixed,
                              objectEvent->z.fixed);
    }
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(1));
    ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(2), PIXELS(1));
    ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                                gValeStorm_BoulderFallLeaderPath);
    ObjectEvent_RunActionScriptAndWait(LOCALID_GARET,
                                       gValeStorm_BoulderFallGaretPath);
    ObjectEvent_RefreshSelectorById(LOCALID_PARTY_LEADER);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          30);
    MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
    Script_PlaySong(SE_145);
    FieldScript_WaitFrames(40);
    MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
    Script_PlaySong(SE_145);
    FieldScript_WaitFrames(20);
    ObjectEvent_SetAttachedEffect(
        LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(40);
    // Show the four elders holding back the Boulder.
    Engine_ObjectEventSetAnimation(LOCALID_ELDER_EAST,
                                   ANIM_BEARDED_ELDER_HOLD_BACK);
    Engine_ObjectEventSetAnimation(LOCALID_ELDER_WEST,
                                   ANIM_BEARDED_ELDER_HOLD_BACK);
    Engine_ObjectEventSetAnimation(LOCALID_ELDER_FAR_EAST,
                                   ANIM_YOUNG_ELDER_HOLD_BACK);
    Engine_ObjectEventSetAnimation(LOCALID_ELDER_FAR_WEST,
                                   ANIM_YOUNG_ELDER_HOLD_BACK);
    ObjectEvent_Get(LOCALID_ELDER_FAR_EAST)->scaleX = -Q16_ONE;
    ObjectMotion_SetActionVariant(LOCALID_ELDER_EAST, 2);
    ObjectMotion_SetActionVariant(LOCALID_ELDER_WEST, 2);
    ObjectMotion_SetActionVariant(LOCALID_ELDER_FAR_EAST, 3);
    ObjectMotion_SetActionVariant(LOCALID_ELDER_FAR_WEST, 3);
    FieldScript_SetMessageId(MsgValeTheBoulderIsFalling);
    Script_ShowMessageAndWait(LOCALID_ELDER_EAST_STAND_IN, 0, 20);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 20);
    FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
    FieldCamera_MoveTo(PIXELS(112), -1, PIXELS(331), 1);
    FieldCamera_WaitForArrival();
    for (i = 0; i < 40; i++) {
      ValeStorm_FlashPaletteAndSpawnEffects(
          ObjectEvent_Get(LOCALID_ELDER_EAST));
      ValeStorm_FlashPaletteAndSpawnEffects(
          ObjectEvent_Get(LOCALID_ELDER_WEST));
      ValeStorm_FlashPaletteAndSpawnEffects(
          ObjectEvent_Get(LOCALID_ELDER_FAR_EAST));
      ValeStorm_FlashPaletteAndSpawnEffects(
          ObjectEvent_Get(LOCALID_ELDER_FAR_WEST));
      WaitFrames(1);
    }
    // Run the palette effects while the boulder rocks back and forth.
    shakeShift = &sShakeShift;
    updateEffects = ValeStorm_UpdateBoulderMapEffects;
    sShakeDone = 0;
    *shakeShift = 0;
    FieldScheduler_AddOrUpdateCallback(updateEffects, TASK_PRIORITY_MAP);
    cycleBoulderPalette = ValeStorm_CycleBoulderPalette;
    FieldScheduler_AddCallback(cycleBoulderPalette, TASK_PRIORITY_MAP);
    FieldScript_WaitFrames(40);
    *shakeShift = 1;
    FieldScript_WaitFrames(30);
    ObjectEvent_SetPosition(LOCALID_BOULDER, PIXELS(114), PIXELS(290));
    movingObjectEvent = ObjectEvent_Get(LOCALID_BOULDER);
    boulderHeight = movingObjectEvent->y.fixed + PIXELS(64);
    movingObjectEvent->y.fixed = boulderHeight;
    movingObjectEvent->targetY = boulderHeight;
    ObjectEvent_SetSpeed(LOCALID_BOULDER, PIXELS(4) / 5, PIXELS(2) / 5);
    Script_PlaySong(SE_145);
    ObjectEvent_MoveToAndWait(LOCALID_BOULDER, 114, 333);
    Engine_ObjectEventSetAnimation(LOCALID_BOULDER, ANIM_WALK);
    MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Script_PlaySong(SE_145);
    *shakeShift = 0;
    ObjectEvent_SetSpeed(LOCALID_BOULDER, PIXELS(2) / 5, PIXELS(1) / 5);
    ObjectEvent_MoveToAndWait(LOCALID_BOULDER, 114, 300);
    Engine_ObjectEventSetAnimation(LOCALID_BOULDER, ANIM_WALK);
    MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
    Script_PlaySong(SE_145);
    *shakeShift = 2;
    ObjectEvent_SetSpeed(LOCALID_BOULDER, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_MoveToAndWait(LOCALID_BOULDER, 114, 333);
    Engine_ObjectEventSetAnimation(LOCALID_BOULDER, ANIM_WALK);
    MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Script_PlaySong(SE_145);
    *shakeShift = 0;
    ObjectEvent_SetSpeed(LOCALID_BOULDER, PIXELS(2) / 5, PIXELS(1) / 5);
    ObjectEvent_MoveToAndWait(LOCALID_BOULDER, 114, 300);
    Engine_ObjectEventSetAnimation(LOCALID_BOULDER, ANIM_WALK);
    MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
    Script_PlaySong(SE_145);
    *shakeShift = 2;
    ObjectEvent_SetSpeed(LOCALID_BOULDER, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_MoveToAndWait(LOCALID_BOULDER, 114, 333);
    Engine_ObjectEventSetAnimation(LOCALID_BOULDER, ANIM_WALK);
    MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Script_PlaySong(SE_145);
    *shakeShift = 1;
    FieldScript_WaitFrames(20);
    ObjectEvent_SetAttachedEffect(LOCALID_ELDER_EAST, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_ELDER_EAST, 2);
    Script_ShowMessage(LOCALID_ELDER_WEST_STAND_IN, 0);
    ObjectEvent_ShowEmote(LOCALID_ELDER_WEST,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_ELDER_WEST, 2);
    Script_ShowMessageAndWait(LOCALID_ELDER_EAST_STAND_IN, 0, 40);
    ObjectEvent_SetAttachedEffect(LOCALID_ELDER_FAR_EAST,
                                  ATTACHED_EFFECT_IN_FRONT |
                                      ATTACHED_EFFECT_ONCE);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_ELDER_FAR_EAST, 2);
    Script_ShowMessage(LOCALID_ELDER_FAR_EAST, 0);
    // The westernmost elder stops straining, looks about and calls to the
    // boys, and the camera returns to Isaac.
    sShakeDone = 1;
    Engine_ObjectEventSetAnimation(LOCALID_ELDER_FAR_WEST, ANIM_STAND);
    WaitFrames(1);
    ObjectEvent_SetChildValue(LOCALID_ELDER_FAR_WEST, 0);
    ObjectEvent_ShowEmote(LOCALID_ELDER_FAR_WEST,
                          EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
    ObjectEvent_FaceDirection(LOCALID_ELDER_FAR_WEST, FACING_WEST, 40);
    ObjectEvent_FaceDirection(LOCALID_ELDER_FAR_WEST, FACING_EAST, 20);
    ObjectEvent_FaceDirection(LOCALID_ELDER_FAR_WEST, FACING_WEST, 20);
    ObjectEvent_FaceDirection(LOCALID_ELDER_FAR_WEST, FACING_SOUTH, 40);
    ObjectEvent_ShowEmote(LOCALID_ELDER_FAR_WEST,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_ELDER_FAR_WEST, 2);
    ObjectMotion_Launch(LOCALID_ELDER_FAR_WEST, 4, 40);
    Engine_ObjectEventSetAnimation(LOCALID_ELDER_FAR_WEST,
                                   ANIM_YOUNG_ELDER_CALL_OUT);
    FieldScript_WaitFrames(10);
    Script_ShowMessageAndWait(LOCALID_ELDER_FAR_WEST, 0, 20);
    Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    FieldCamera_SetSpeed(PIXELS(6), PIXELS(3) / 4);
    FieldCamera_MoveTo(PIXELS(84), -1, PIXELS(564), 1);
    FieldCamera_WaitForArrival();
    Map_PlayQueuedMusic();
    ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
    FieldScript_WaitFrames(20);
    ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(30);
    Task_Remove(updateEffects);
    Task_Remove(cycleBoulderPalette);
    Script_ShowMessage(LOCALID_GARET, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
    FieldScript_WaitFrames(20);
    Map_EndCutscenePsynergy();
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
    FieldScript_WaitFrames(20);
    Engine_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
    movingObjectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (movingObjectEvent != 0) {
      ObjectEvent_SetDestination(LOCALID_GARET, movingObjectEvent->x.part.pixel,
                                 movingObjectEvent->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
    ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
    ObjectEvent_DestroyById(LOCALID_ELDER_WEST_STAND_IN);
    ObjectEvent_DestroyById(LOCALID_ELDER_EAST_STAND_IN);
    ObjectEvent_DestroyById(LOCALID_ELDER_FAR_EAST);
    ObjectEvent_DestroyById(LOCALID_ELDER_FAR_WEST);
    ObjectEvent_DestroyById(LOCALID_ELDER_EAST);
    ObjectEvent_DestroyById(LOCALID_ELDER_WEST);
    GameFlag_Set(FLAG_VALE_BOULDER_FELL);
    FieldScript_End();
  }
}

/// Sets the animation parameter of each entry of a one-sprite object event:
/// mode - 1, or for mode 0 a value from gValeStorm_FrameModes by the frame
/// count. Only ValeStorm_FlashBoulderMapAnimationEntries calls it.
static void ValeStorm_SetObjectEventFrameMode(struct ObjectEvent *objectEvent,
                                              s32 mode) {
  if ((objectEvent->spriteStorageKind & 0xf) == 1) {
    struct Sprite *animation = objectEvent->sprite;
    s32 animationParameter = mode - 1;
    u8 count;
    if (mode == 0) {
      animationParameter =
          gValeStorm_FrameModes[(gFrameCount >> 1) &
                                (objectEvent->spriteStorageKind & 0xf)];
    }
    count = animation->count;
    if (count != 0) {
      struct SpriteAnim **entries = animation->entries;
      s32 remainingEntries = count;
      do {
        struct SpriteAnim *entry = *entries++;
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
UNUSED static void ValeStorm_FlashBoulderMapAnimationEntries(void) {
  if (((gFrameCount >> sShakeShift) & 3) != 0) {
    ValeStorm_SetObjectEventFrameMode(ObjectEvent_GetById(LOCALID_ELDER_EAST),
                                      1);
    ValeStorm_SetObjectEventFrameMode(ObjectEvent_GetById(LOCALID_ELDER_WEST),
                                      1);
    ValeStorm_SetObjectEventFrameMode(
        ObjectEvent_GetById(LOCALID_ELDER_FAR_EAST), 1);
    ValeStorm_SetObjectEventFrameMode(
        ObjectEvent_GetById(LOCALID_ELDER_FAR_WEST), 1);
  } else {
    ValeStorm_SetObjectEventFrameMode(ObjectEvent_GetById(LOCALID_ELDER_EAST),
                                      8);
    ValeStorm_SetObjectEventFrameMode(ObjectEvent_GetById(LOCALID_ELDER_WEST),
                                      8);
    ValeStorm_SetObjectEventFrameMode(
        ObjectEvent_GetById(LOCALID_ELDER_FAR_EAST), 8);
    ValeStorm_SetObjectEventFrameMode(
        ObjectEvent_GetById(LOCALID_ELDER_FAR_WEST), 8);
  }
}

/// Jenna cries out for Felix as her family tries to save him at the river. In
/// Vale on the stormy night, her brother Felix has fallen in and clings on as
/// Kyle and her parents try to save him. "Oh no! My brother!" The rope will not
/// reach and no one has Psynergy left, so they send for help. Reached by
/// stepping on metatile behavior 35 until FLAG_VALE_STORM_SAW_FELIX_HANGING_ON
/// is set, which this sets after ValeStorm_DoraGoesForHelp.
static void EventScript_WatchAttemptToSaveFelix(void) {
  const s32 *pulseScript;

  if (GameFlag_IsSet(FLAG_VALE_STORM_SAW_FELIX_HANGING_ON) != 0) {
    return;
  }
  FieldScript_Start();
  ObjectEvent_SetPosition(LOCALID_KYLE, PIXELS(192), PIXELS(1214));
  ObjectEvent_FaceDirection(LOCALID_KYLE, FACING_SOUTHEAST, 0);
  Engine_ObjectEventSetAnimation(LOCALID_KYLE, ANIM_KYLE_REACH_OUT);
  {
    struct ObjectEvent *objectEvent;
    s32 pulseDelay;
    objectEvent = ObjectEvent_GetById(LOCALID_KYLE);
    pulseDelay = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    pulseScript = gValeStorm_SquashAndStretchScript;
    objectEvent->timer = pulseDelay;
    ObjectEvent_SetActionScript(LOCALID_KYLE, pulseScript);
  }
  ObjectEvent_SetPosition(LOCALID_DORA, PIXELS(192), PIXELS(1189));
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST, 0);
  ObjectEvent_SetPosition(LOCALID_JENNAS_FATHER, PIXELS(227), PIXELS(1214));
  ObjectEvent_FaceDirection(LOCALID_JENNAS_FATHER, FACING_SOUTH, 0);
  Engine_ObjectEventSetAnimation(LOCALID_JENNAS_FATHER, ANIM_PARENT_REACH_OUT);
  {
    struct ObjectEvent *objectEvent;
    s32 pulseDelay;
    objectEvent = ObjectEvent_GetById(LOCALID_JENNAS_FATHER);
    pulseDelay = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    objectEvent->timer = pulseDelay;
    ObjectEvent_SetActionScript(LOCALID_JENNAS_FATHER, pulseScript);
  }
  ObjectEvent_SetPosition(LOCALID_JENNAS_MOTHER, PIXELS(250), PIXELS(1214));
  ObjectEvent_FaceDirection(LOCALID_JENNAS_MOTHER, FACING_SOUTH, 0);
  Engine_ObjectEventSetAnimation(LOCALID_JENNAS_MOTHER, ANIM_PARENT_REACH_OUT);
  {
    struct ObjectEvent *objectEvent;
    s32 pulseDelay;
    objectEvent = ObjectEvent_GetById(LOCALID_JENNAS_MOTHER);
    pulseDelay = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    objectEvent->timer = pulseDelay;
    ObjectEvent_SetActionScript(LOCALID_JENNAS_MOTHER, pulseScript);
  }
  ObjectEvent_SetPosition(LOCALID_JENNA, PIXELS(227), PIXELS(1189));
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_SetPosition(LOCALID_FELIX, PIXELS(243), PIXELS(1277));
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_NORTH, 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_FELIX), 0);
  WaitFrames(3);
  FieldScript_SetMessageId(MsgValeNoBrother);
  FieldScript_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_JENNA, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 150, 1094);
  {
    struct ObjectEvent *leader;
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (leader != 0) {
      Engine_ObjectEventSetPosition(LOCALID_GARET, leader->x.fixed,
                                    leader->z.fixed);
    }
  }
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 132, 1094);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH, 20);
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  FieldCamera_MoveTo(PIXELS(216), -1, PIXELS(1232), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KYLE, 2);
  FieldScript_ShowMessageThenWait(LOCALID_KYLE, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 3);
  ObjectMotion_ArmCallback(LOCALID_DORA, FACING_EAST, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_DORA, ANIM_NOD);
  FieldScript_ShowMessageThenWait(LOCALID_DORA, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST + FACING_STEP, 10);
  FieldCamera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
  FieldCamera_MoveTo(PIXELS(232), -1, PIXELS(1253), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  Sound_PlaySong(SE_HEAVY_IMPACT);
  ObjectMotion_Launch(LOCALID_FELIX, 4, 0);
  Engine_ObjectEventSetAnimation(LOCALID_FELIX, ANIM_FELIX_UNDER_WATER);
  FieldScript_WaitFrames(10);
  Engine_ObjectEventSetPosition(LOCALID_FELIX, 0, 0);
  FieldScript_WaitFrames(60);
  Map_PlayQueuedMusic();
  Engine_ObjectEventSetAnimation(LOCALID_KYLE, ANIM_STAND);
  {
    struct ObjectEvent *objectEvent;
    objectEvent = ObjectEvent_GetById(LOCALID_KYLE);
    objectEvent->scaleX = Q16_ONE;
    objectEvent->scaleY = Q16_ONE;
  }
  Engine_ObjectEventSetAnimation(LOCALID_JENNAS_FATHER, ANIM_STAND);
  {
    struct ObjectEvent *objectEvent;
    objectEvent = ObjectEvent_GetById(LOCALID_JENNAS_FATHER);
    objectEvent->scaleX = Q16_ONE;
    objectEvent->scaleY = Q16_ONE;
  }
  Engine_ObjectEventSetAnimation(LOCALID_JENNAS_MOTHER, ANIM_STAND);
  {
    struct ObjectEvent *objectEvent;
    objectEvent = ObjectEvent_GetById(LOCALID_JENNAS_MOTHER);
    objectEvent->scaleX = Q16_ONE;
    objectEvent->scaleY = Q16_ONE;
  }
  ObjectMotion_SetVariantCallback(LOCALID_KYLE, 2);
  ObjectMotion_SetVariantCallback(LOCALID_DORA, 2);
  ObjectMotion_SetVariantCallback(LOCALID_JENNAS_FATHER, 2);
  ObjectMotion_SetVariantCallback(LOCALID_JENNAS_MOTHER, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
  FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  FieldCamera_MoveTo(PIXELS(216), -1, PIXELS(1232), 1);
  FieldCamera_WaitForArrival();
  ObjectEvent_SetAttachedEffect(LOCALID_JENNA, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  FieldEffect_ConfigureLinkedObject(LOCALID_DORA, ATTACHED_EFFECT_IN_FRONT |
                                                      ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
  ObjectMotion_SetVariantCallback(LOCALID_JENNA, 3);
  FieldScript_ShowMessageAndWait(LOCALID_JENNA, 0);
  ObjectMotion_Launch(LOCALID_JENNAS_MOTHER, 2, 0);
  ObjectEvent_SetDestination(LOCALID_JENNAS_MOTHER, 234, 1205);
  ObjectMotion_Launch(LOCALID_JENNA, 2, 0);
  ObjectEvent_SetDestination(LOCALID_JENNA, 227, 1201);
  FieldScript_WaitFrames(90);
  FieldCamera_MoveTo(PIXELS(232), -1, PIXELS(1253), 1);
  FieldCamera_WaitForArrival();
  Engine_ObjectEventSetPosition(LOCALID_FELIX, PIXELS(243), PIXELS(1277));
  WaitFrames(1);
  Sound_PlaySong(SE_106);
  {
    struct ObjectEvent *objectEvent;
    objectEvent = ObjectEvent_GetById(LOCALID_FELIX);
    objectEvent->velocityY = PIXELS(2);
  }
  FieldScript_WaitFrames(6);
  Engine_ObjectEventSetAnimation(LOCALID_FELIX, ANIM_FELIX_CLING);
  FieldScript_WaitFrames(20);
  Map_PlayQueuedMusic();
  FieldScript_WaitFrames(20);
  FieldCamera_SetSpeedParameters(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveWithinMapBounds(PIXELS(216), -1, PIXELS(1232), 1);
  FieldCamera_WaitForArrival();
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNAS_FATHER, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_JENNAS_FATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        40);
  ObjectEvent_LinkPair(LOCALID_JENNAS_FATHER, LOCALID_KYLE, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KYLE, 2);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_KYLE, 0);
  {
    struct ObjectEvent *objectEvent;
    objectEvent = ObjectEvent_GetById(LOCALID_JENNAS_MOTHER);
    objectEvent->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  }
  {
    struct ObjectEvent *objectEvent;
    objectEvent = ObjectEvent_GetById(LOCALID_JENNA);
    objectEvent->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  }
  ObjectEvent_SetSpeed(LOCALID_JENNAS_MOTHER, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetDestination(LOCALID_JENNAS_MOTHER, 247, 1210);
  ObjectEvent_MoveToAndWait(LOCALID_JENNA, 227, 1189);
  {
    struct ObjectEvent *objectEvent;
    objectEvent = ObjectEvent_GetById(LOCALID_JENNAS_MOTHER);
    objectEvent->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  }
  ObjectEvent_GetById(LOCALID_JENNA)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectMotion_ArmCallback(LOCALID_JENNA, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNAS_MOTHER, FACING_WEST, 10);
  Engine_ObjectEventSetAnimation(LOCALID_JENNAS_FATHER, ANIM_SHAKE_HEAD);
  FieldScript_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | LOCALID_JENNAS_FATHER,
                                  0, 10);
  ObjectEvent_FaceDirection(LOCALID_KYLE, FACING_NORTH, 20);
  ObjectMotion_ArmCallback(LOCALID_KYLE, FACING_EAST, 10);
  Engine_ObjectEventSetAnimation(LOCALID_KYLE, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_KYLE, 0, 10);
  ObjectEvent_ShowEmote(LOCALID_JENNAS_FATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  ObjectEvent_ShowEmote(LOCALID_KYLE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 20);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_WEST, 40);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTH, 20);
  ObjectMotion_ArmCallback(LOCALID_DORA, FACING_EAST, 30);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 10);
  FieldScript_ShowMessageThenWait(LOCALID_DORA, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_KYLE, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNAS_MOTHER, FACING_WEST + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_JENNAS_FATHER, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_WEST, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KYLE, 1);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_KYLE, 0, 10);
  Engine_ObjectEventSetAnimation(LOCALID_DORA, ANIM_SHAKE_HEAD);
  FieldScript_ShowMessageThenWait(LOCALID_DORA, 0, 10);
  ObjectEvent_ShowEmote(LOCALID_KYLE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(LOCALID_JENNAS_FATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  ObjectEvent_ShowEmote(LOCALID_JENNAS_MOTHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  ObjectMotion_ArmCallback(LOCALID_DORA, FACING_EAST, 10);
  ObjectEvent_LinkPair(LOCALID_JENNAS_FATHER, LOCALID_JENNAS_MOTHER, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_ArmCallback(LOCALID_DORA, FACING_EAST, 0);
  ObjectMotion_ArmCallback(LOCALID_KYLE, FACING_EAST, 10);
  ObjectEvent_FaceDirection(LOCALID_JENNAS_FATHER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNAS_MOTHER, FACING_WEST, 10);
  Engine_ObjectEventSetAnimation(LOCALID_JENNAS_FATHER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_JENNAS_MOTHER, ANIM_NOD);
  ObjectEvent_LinkPair(LOCALID_KYLE, LOCALID_DORA, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KYLE, 1);
  FieldScript_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | LOCALID_KYLE, 0, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_DORA, ANIM_NOD);
  ObjectEvent_FaceDirection(LOCALID_JENNAS_FATHER, FACING_NORTH + FACING_STEP,
                            10);
  ObjectMotion_SetVariantCallback(LOCALID_JENNAS_FATHER, 1);
  FieldScript_ShowMessageThenWait(LOCALID_JENNAS_FATHER, 0, 10);
  ObjectMotion_ArmCallback(LOCALID_KYLE, FACING_EAST, 0);
  ObjectMotion_ArmCallback(LOCALID_DORA, FACING_EAST, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 1);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST, 20);
  ObjectEvent_FaceDirection(LOCALID_JENNAS_MOTHER, FACING_NORTHWEST, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_JENNAS_MOTHER, ANIM_NOD);
  FieldScript_ShowMessageThenWait(LOCALID_JENNAS_MOTHER, 0, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_JENNA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 2);
  ObjectMotion_SetAnimationAndWait(LOCALID_DORA, ANIM_NOD);
  FieldScript_ShowMessageThenWait(LOCALID_DORA, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_WEST, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_JENNA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 1);
  FieldScript_ShowMessageThenWait(LOCALID_DORA, 0, 10);
  ValeStorm_DoraGoesForHelp();
  Engine_GameFlagSet(FLAG_VALE_STORM_SAW_FELIX_HANGING_ON);
  FieldScript_End();
}

/// Dora and Jenna run through the house by the river; Dora goes north for help
/// and sends Jenna to the plaza, then asks Isaac and Garet to help: a yes
/// sends them after Jenna, a no to the plaza all the same. Called by
/// EventScript_WatchAttemptToSaveFelix.
static void ValeStorm_DoraGoesForHelp(void) {
  struct ObjectEvent *villager;
  s32 random;
  const s32 *script;
  s32 northMessage;
  s32 helpMessage;
  struct ObjectEvent *leader;

  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNAS_FATHER, FACING_NORTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_JENNAS_MOTHER,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_KYLE, FACING_NORTH + FACING_STEP, 20);
  Engine_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_NOD);
  Engine_ObjectEventSetAnimation(LOCALID_JENNAS_FATHER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(LOCALID_JENNAS_MOTHER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(LOCALID_DORA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_JENNAS_MOTHER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(134), -1, PIXELS(1195), 1);
  ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_SetActionScript(LOCALID_JENNA,
                              gValeStorm_JennaRunsIntoRiversideHouse);
  ObjectEvent_RunActionScriptAndWait(LOCALID_DORA,
                                     gValeStorm_DoraRunsIntoRiversideHouse);
  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gValeStorm_GrandparentsDoorOpens, 38, 72);
  FieldScript_WaitFrames(10);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 149, 1175);
  ObjectEvent_SetPosition(LOCALID_DORA, 0, 0);
  ObjectEvent_WalkToAndWait(LOCALID_JENNAS_MOTHER, 250, 1214);
  Map_PlayQueuedMusic();
  ObjectEvent_FaceDirection(LOCALID_KYLE, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNAS_FATHER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNAS_MOTHER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ObjectEventSetAnimation(LOCALID_KYLE, ANIM_KYLE_REACH_OUT);
  Engine_ObjectEventSetAnimation(LOCALID_JENNAS_FATHER, ANIM_PARENT_REACH_OUT);
  Engine_ObjectEventSetAnimation(LOCALID_JENNAS_MOTHER, ANIM_PARENT_REACH_OUT);
  villager = ObjectEvent_Get(LOCALID_KYLE);
  random = Engine_RandomNext();
  villager->timer = (Math_RemainderUnsigned(random, 90) + 60);
  villager = ObjectEvent_Get(LOCALID_JENNAS_FATHER);
  random = Engine_RandomNext();
  villager->timer = (Math_RemainderUnsigned(random, 90) + 60);
  villager = ObjectEvent_Get(LOCALID_JENNAS_MOTHER);
  random = Engine_RandomNext();
  villager->timer = (Math_RemainderUnsigned(random, 90) + 60);
  script = gValeStorm_SquashAndStretchScript;
  ObjectEvent_SetActionScript(LOCALID_KYLE, script);
  ObjectEvent_SetActionScript(LOCALID_JENNAS_FATHER, script);
  ObjectEvent_SetActionScript(LOCALID_JENNAS_MOTHER, script);
  ObjectEvent_RefreshSelectorById(LOCALID_JENNA);
  FieldScript_WaitFrames(10);
  Script_PlaySong(SE_159);
  MapGrid_AnimateCells(gValeStorm_GrandparentsDoorCloses, 38, 72);
  FieldScript_WaitFrames(30);
  Map_PlayQueuedMusic();
  FieldCamera_MoveTo(PIXELS(112), -1, PIXELS(1225), 1);
  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gValeStorm_IsaacsDoorOpens, 35, 73);
  FieldScript_WaitFrames(20);
  Map_PlayQueuedMusic();
  ObjectEvent_SetActionScript(LOCALID_DORA,
                              gValeStorm_DoraComesOutOfRiversideHouse);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetActionScript(LOCALID_JENNA,
                              gValeStorm_JennaComesOutOfRiversideHouse);
  FieldScript_WaitFrames(40);
  Script_PlaySong(SE_159);
  MapGrid_AnimateCells(gValeStorm_IsaacsDoorCloses, 35, 73);
  ObjectEvent_RefreshSelectorById(LOCALID_JENNA);
  Map_PlayQueuedMusic();
  FieldScript_WaitFrames(40);
  northMessage = MsgValeGoLookNorth;
  FieldScript_SetMessageId(northMessage);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_JENNA, ANIM_NOD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_JENNA, 0, 40);
  Engine_ObjectEventSetAnimation(LOCALID_DORA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_JENNA, ANIM_NOD);
  FieldScript_WaitFrames(30);
  ObjectEvent_SetActionScript(LOCALID_DORA, gValeStorm_DoraHeadsNorth);
  ObjectEvent_SetActionScript(LOCALID_JENNA, gValeStorm_JennaHeadsForPlaza);
  FieldScript_WaitFrames(40);
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  FieldCamera_MoveTo(PIXELS(105), -1, PIXELS(1086), 1);
  ObjectEvent_RefreshSelectorById(LOCALID_DORA);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_EAST, 0);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 10);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 105, 1086);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 2);
  Script_OpenMessage(SPEAKER_WINDOW_BELOW | LOCALID_DORA, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    ObjectMotion_SetAnimationAndWait(LOCALID_DORA, ANIM_NOD);
    FieldScript_SetMessageId((northMessage + 4));
  } else {
    ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 2);
    FieldScript_SetMessageId((northMessage + 5));
  }
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_DORA, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 40);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  helpMessage = MsgValeDontSupposeTwo;
  FieldScript_SetMessageId(helpMessage);
  Script_OpenMessage(SPEAKER_WINDOW_BELOW | LOCALID_DORA, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    ObjectMotion_SetAnimationAndWait(LOCALID_DORA, ANIM_NOD);
    FieldScript_SetMessageId((helpMessage + 1));
    Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_DORA, 0, 30);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 20);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
    Engine_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_DORA, ANIM_NOD);
    FieldScript_WaitFrames(40);
  } else {
    ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 90);
    ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Engine_ObjectEventSetAnimation(LOCALID_DORA, ANIM_SHAKE_HEAD);
    FieldScript_SetMessageId((helpMessage + 2));
    Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_DORA, 0);
  }
  ObjectEvent_SetActionScript(LOCALID_DORA, gValeStorm_DoraLeavesForHelp);
  FieldScript_WaitFrames(90);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
  FieldScript_WaitFrames(40);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Engine_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(LOCALID_GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
}

/// Kyle tells Isaac and Garet to find help for Felix. In Vale on the stormy
/// night, at the river, he tells them they don't have enough Psynergy to save
/// Felix.
/// Reached by talking to Kyle; the first time he greets Isaac and sets
/// FLAG_VALE_STORM_KYLE_GREETED_ISAAC.
static void EventScript_TalkAboutSavingFelix(void) {
  s32 facing = FACING_SOUTHEAST;
  const s32 *pulseScript;

  FieldScript_Start();
  Engine_ObjectEventSetAnimation(LOCALID_KYLE, ANIM_STAND);
  FieldScript_WaitFrames(10);
  ObjectEvent_LinkPair(LOCALID_KYLE, LOCALID_PARTY_LEADER, 20);
  if (GameFlag_Test(FLAG_VALE_STORM_KYLE_GREETED_ISAAC) != 0) {
    FieldScript_SetMessageId(MsgValeTwoDontEnough);
    FieldScript_ShowMessageThenWait(LOCALID_KYLE, 0, 10);
  } else {
    FieldScript_SetMessageId(MsgValeOh);
    ObjectMotion_SetVariantCallback(LOCALID_KYLE, 1);
    FieldScript_ShowMessageThenWait(LOCALID_KYLE, 0, 10);
    ObjectMotion_SetVariantCallback(LOCALID_KYLE, 2);
    FieldScript_ShowMessageThenWait(LOCALID_KYLE, 0, 10);
  }
  ObjectMotion_ArmCallback(LOCALID_KYLE, facing, 20);
  Engine_ObjectEventSetAnimation(LOCALID_KYLE, ANIM_KYLE_REACH_OUT);
  FieldScript_WaitFrames(10);
  {
    struct ObjectEvent *villager;
    s32 pulseDelay;
    villager = ObjectEvent_GetById(LOCALID_KYLE);
    pulseDelay = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    pulseScript = gValeStorm_SquashAndStretchScript;
    villager->timer = pulseDelay;
    ObjectEvent_SetActionScript(LOCALID_KYLE, pulseScript);
  }
  FieldScript_WaitFrames(20);
  Engine_GameFlagSet(FLAG_VALE_STORM_KYLE_GREETED_ISAAC);
  FieldScript_End();
}

/// The Boulder sweeps Felix, his parents and Kyle away. In Vale on the stormy
/// night, Jenna comes back to the river with Isaac, Garet and the man whose
/// Psynergy is restored ("Mom! Dad! We're back!"), and as Dora calls everyone
/// down to save Felix the Boulder comes down the river and sweeps away Felix,
/// his parents and Kyle. Reached by stepping on metatile behavior 36 once
/// FLAG_VALE_STORM_SENT_TO_AID_ELDERS is set (vale_gate.c), until
/// FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY is set, which this sets.
static void EventScript_BoulderSweepsThemAway(void) {
  struct ObjectEvent *leader;
  struct ObjectEvent *objectEvent;
  s32 automaticPriority;
  struct Sprite *sprite;

  if (GameFlag_Test(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) == 0) {
  } else {
    if (GameFlag_IsSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) != 0) {
    } else {
      FieldScript_Start();
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1), PIXELS(1) / 2);
      ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(1), PIXELS(1) / 2);
      ObjectEvent_SetSpeed(LOCALID_EXHAUSTED_STRONGMAN, PIXELS(1),
                           PIXELS(1) / 2);
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 217, 1367);
      objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      if (objectEvent != 0) {
        Engine_ObjectEventSetPosition(LOCALID_GARET, objectEvent->x.fixed,
                                      objectEvent->z.fixed);
      }
      ObjectEvent_WalkToAndWait(LOCALID_GARET, 235, 1367);
      ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP,
                                0);
      objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      if (objectEvent != 0) {
        Engine_ObjectEventSetPosition(LOCALID_JENNA, objectEvent->x.fixed,
                                      objectEvent->z.fixed);
      }
      ObjectEvent_WalkToAndWait(LOCALID_JENNA, 199, 1367);
      ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTH + FACING_STEP, 0);
      ObjectEvent_SetPosition(LOCALID_JENNAS_MOTHER, PIXELS(247), PIXELS(1210));
      ObjectMotion_ArmCallback(LOCALID_JENNAS_MOTHER, FACING_SOUTHWEST, 0);
      objectEvent = ObjectEvent_GetById(LOCALID_EXHAUSTED_STRONGMAN);
      sprite = objectEvent->sprite;
      objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
      sprite->image.priority = 1;
      leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      sprite = leader->sprite;
      leader->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
      sprite->image.priority = 2;
      objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      if (objectEvent != 0) {
        Engine_ObjectEventSetPosition(LOCALID_EXHAUSTED_STRONGMAN,
                                      objectEvent->x.fixed,
                                      objectEvent->z.fixed);
      }
      ObjectEvent_WalkToAndWait(LOCALID_EXHAUSTED_STRONGMAN, 221, 1385);
      ObjectEvent_FaceDirection(LOCALID_EXHAUSTED_STRONGMAN,
                                FACING_NORTHWEST + FACING_STEP, 60);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
      FieldScript_SetMessageId(MsgValeMomDadBack);
      FieldScript_ShowMessageThenWait(LOCALID_JENNA, 0, 40);
      ObjectEvent_SetPosition(LOCALID_DORA, PIXELS(101), PIXELS(1197));
      ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST, 0);
      FieldScript_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | LOCALID_DORA, 0,
                                      10);
      ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST, 0);
      FieldCamera_SetSpeedParameters(PIXELS(6) / 5, PIXELS(3) / 20);
      FieldCamera_MoveTo(PIXELS(101), -1, PIXELS(1197), 1);
      ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(7) / 5, PIXELS(7) / 10);
      ObjectEvent_SetActionScript(LOCALID_DORA, gValeStorm_DoraRunsToRiver);
      FieldScript_WaitFrames(60);
      FieldCamera_SetSpeedParameters(PIXELS(3) / 5, PIXELS(3) / 40);
      FieldCamera_MoveTo(PIXELS(187), -1, PIXELS(1328), 1);
      FieldCamera_WaitForArrival();
      FieldScript_WaitFrames(40);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
      FieldScript_ShowMessageThenWait(LOCALID_JENNA, 0, 20);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 2);
      FieldScript_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | LOCALID_DORA, 0,
                                      20);
      FieldCamera_SetSpeedParameters(PIXELS(2), PIXELS(1) / 4);
      FieldCamera_MoveTo(PIXELS(221), -1, PIXELS(1385), 1);
      ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                                  LOCALID_EXHAUSTED_STRONGMAN, 0);
      ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_EXHAUSTED_STRONGMAN,
                                  0);
      ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST + FACING_STEP,
                                80);
      FieldCamera_MoveWithinMapBounds(PIXELS(182), -1, PIXELS(1360), 1);
      ObjectEvent_WalkToAndWait(LOCALID_EXHAUSTED_STRONGMAN, 182, 1384);
      ObjectMotion_SetAngleToward(LOCALID_EXHAUSTED_STRONGMAN, LOCALID_DORA, 0);
      FieldScript_WaitFrames(30);
      ObjectMotion_SetAnimationAndWait(LOCALID_EXHAUSTED_STRONGMAN, ANIM_NOD);
      FieldScript_WaitFrames(10);
      ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
      ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_DORA, 0);
      ObjectMotion_SetAngleToward(LOCALID_JENNA, LOCALID_DORA, 0);
      ObjectMotion_SetAnimationAndWait(LOCALID_DORA, ANIM_NOD);
      FieldScript_ShowMessageAndWait(LOCALID_DORA, 0);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
      FieldScript_ShowMessageThenWait(LOCALID_JENNA, 0, 10);
      ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTHEAST, 40);
      ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST + FACING_STEP,
                                20);
      ObjectMotion_SetAnimationAndWait(LOCALID_DORA, ANIM_NOD);
      FieldScript_ShowMessageAndWait(LOCALID_DORA, 0);
      ObjectEvent_LinkPair(LOCALID_JENNA, LOCALID_EXHAUSTED_STRONGMAN, 0);
      ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
      FieldScript_WaitFrames(40);
      ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
      ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_DORA, 0);
      ObjectMotion_SetAngleToward(LOCALID_JENNA, LOCALID_DORA, 0);
      ObjectMotion_SetAngleToward(LOCALID_EXHAUSTED_STRONGMAN, LOCALID_DORA, 0);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 2);
      FieldScript_WaitFrames(20);
      FieldScript_ShowMessageThenWait(LOCALID_DORA, 0, 10);
      Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
      Engine_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_NOD);
      Engine_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
      automaticPriority = OBJ_EVENT_PRIORITY_AUTOMATIC;
      ObjectMotion_SetAnimationAndWait(LOCALID_EXHAUSTED_STRONGMAN, ANIM_NOD);
      leader->priorityFlags |= automaticPriority;
      objectEvent = ObjectEvent_GetById(LOCALID_EXHAUSTED_STRONGMAN);
      objectEvent->priorityFlags |= automaticPriority;
      ValeStorm_ShowBoulderSweepingDownriver();
      Engine_GameFlagSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY);
      FieldScript_End();
    }
  }
}

/// Places an object event and its targets at once, x doubling as its height.
static __inline__ void
ValeStorm_PlaceObjectEvent(struct ObjectEvent *objectEvent,
                           s32 horizontalPosition, s32 groundPosition) {
  objectEvent->x.fixed = horizontalPosition;
  objectEvent->y.fixed = horizontalPosition;
  objectEvent->targetX = horizontalPosition;
  objectEvent->targetY = horizontalPosition;
  objectEvent->z.fixed = groundPosition;
  objectEvent->targetZ = groundPosition;
}

/// One crash of the Boulder coming down: a shake, SE_145 and a wait.
static __inline__ void ValeStorm_PlayImpactBeat(s32 shakeAmplitude,
                                                s32 frames) {
  MapGrid_SetShake(shakeAmplitude, shakeAmplitude, Q16_ONE);
  Script_PlaySong(SE_145);
  Script_Wait(frames);
}

/// The Boulder comes down the river and carries Felix, his parents and Kyle
/// away. The party gathers on the bank and hears it coming; the screen fades
/// to the river upstream, where the camera follows the Boulder down
/// (gValeStorm_BoulderRollsDownriver), and two frozen lightning flashes show
/// the family and Kyle in its path. Then the bank is redrawn without them,
/// with the rocks the Boulder left, and the party stands at the spot of
/// VALE_STORM_FROM_BOULDER_PASSED. Called by
/// EventScript_BoulderSweepsThemAway, whose recording on the docs site
/// (places/vale-storm/events/EventScript_BoulderSweepsThemAway.gif) shows the
/// descent.
static void ValeStorm_ShowBoulderSweepingDownriver(void) {
  struct Sprite *objectEventVisual;
  struct Sprite *groupVisual;
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *lookingObjectEvent;
  struct ObjectEvent *groupObjectEvent;
  struct ObjectEvent *currentObjectEvent;
  u32 random;
  const s32 *entryActions;
  s32 stoppedVelocity;
  u8 stoppedMotion;
  const s32 *lookAroundActions;
  s32 resetValue;
  const s32 *departureActions;

  objectEvent = ObjectEvent_Get(LOCALID_BOULDER);
  groupObjectEvent = ObjectEvent_Get(LOCALID_BOULDER_SHADOW);
  objectEventVisual = objectEvent->sprite;
  groupVisual = groupObjectEvent->sprite;
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(110), -1, PIXELS(1419), 1);
  ObjectEvent_SetSpeed(LOCALID_EXHAUSTED_STRONGMAN, PIXELS(6) / 5,
                       PIXELS(3) / 5);
  ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  // Bring the group to the riverbank as the lightning subsides.
  entryActions = gValeStorm_ToRiverbankPath;
  ObjectEvent_SetActionScript(LOCALID_EXHAUSTED_STRONGMAN, entryActions);
  Script_Wait(10);
  ObjectEvent_SetActionScript(LOCALID_JENNA, entryActions);
  Weather_LastLightning();
  Script_Wait(10);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, entryActions);
  Script_Wait(10);
  Weather_StopLightning();
  ObjectEvent_SetActionScript(LOCALID_GARET, entryActions);
  Script_Wait(128);
  EventScript_ClearPathCellOverride();
  FieldCamera_MoveTo(PIXELS(174), -1, PIXELS(1428), 1);
  Script_Wait(104);
  FieldCamera_MoveTo(PIXELS(153), -1, PIXELS(1325), 1);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 158, 1272);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST, 0);
  ObjectEvent_RefreshSelectorById(LOCALID_EXHAUSTED_STRONGMAN);
  ObjectEvent_SetActionScript(LOCALID_EXHAUSTED_STRONGMAN,
                              gValeStorm_ExhaustedStrongmanGatherPath);
  ObjectEvent_SetActionScript(LOCALID_JENNA, gValeStorm_JennaGatherPath);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                              gValeStorm_LeaderGatherPath);
  ObjectEvent_RunActionScriptAndWait(LOCALID_GARET, gValeStorm_GaretGatherPath);
  ValeStorm_PlayImpactBeat(PIXELS(4), 20);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Script_Wait(60);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_EXHAUSTED_STRONGMAN,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectEvent_FaceEachOther(LOCALID_JENNA, LOCALID_EXHAUSTED_STRONGMAN, 0);
  ObjectEvent_FaceEachOther(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  Script_Wait(20);
  lookingObjectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  random = Random_Next();
  random = Math_RemainderUnsigned(random, 20) + 20;
  lookingObjectEvent->timer = random;
  stoppedVelocity = 0;
  stoppedMotion = 0;
  lookingObjectEvent = ObjectEvent_Get(LOCALID_GARET);
  random = Random_Next();
  lookingObjectEvent->timer = (Math_RemainderUnsigned(random, 20) + 20);
  lookingObjectEvent = ObjectEvent_Get(LOCALID_JENNA);
  random = Random_Next();
  lookingObjectEvent->timer = (Math_RemainderUnsigned(random, 20) + 20);
  lookingObjectEvent = ObjectEvent_Get(LOCALID_EXHAUSTED_STRONGMAN);
  random = Random_Next();
  lookingObjectEvent->timer = (Math_RemainderUnsigned(random, 20) + 20);
  lookingObjectEvent = ObjectEvent_Get(LOCALID_DORA);
  random = Random_Next();
  lookingObjectEvent->timer = (Math_RemainderUnsigned(random, 20) + 20);
  lookAroundActions = gValeStorm_LookAroundScript;
  ObjectEvent_SetActionScript(LOCALID_DORA, lookAroundActions);
  Script_Wait(30);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, lookAroundActions);
  ObjectEvent_SetActionScript(LOCALID_JENNA, lookAroundActions);
  ObjectEvent_SetActionScript(LOCALID_GARET, lookAroundActions);
  ObjectEvent_SetActionScript(LOCALID_EXHAUSTED_STRONGMAN, lookAroundActions);
  Script_Wait(10);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  ValeStorm_PlayImpactBeat(PIXELS(3), 30);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Script_Wait(120);
  ValeStorm_PlayImpactBeat(PIXELS(3), 40);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Script_Wait(60);
  ValeStorm_PlayImpactBeat(PIXELS(4), 20);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Script_Wait(60);
  ValeStorm_PlayImpactBeat(PIXELS(3), 40);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Script_Wait(60);
  Weather_StopLightning();
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Script_Wait(1);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  FieldCamera_SetSpeed(PIXELS(8), PIXELS(8));
  FieldCamera_MoveTo(PIXELS(217), -1, PIXELS(1084), 1);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_StartInterpolation(40);
  Task_Wait(40);
  // Stage the boulder and its shadow for the downriver camera shot.
  ObjectEvent_SetChildValue(LOCALID_BOULDER, 0);
  currentObjectEvent = ObjectEvent_Get(LOCALID_BOULDER);
  ObjectEvent_SetSpriteFlags(currentObjectEvent, 0);
  currentObjectEvent = ObjectEvent_Get(LOCALID_BOULDER_SHADOW);
  ObjectEvent_SetSpriteFlags(currentObjectEvent, 0);
  groupObjectEvent->scaleX = 0xcccc;
  groupObjectEvent->scaleY = 0xcccc;
  groupObjectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  groupVisual->image.priority = 1;
  ValeStorm_PlaceObjectEvent(objectEvent, PIXELS(200), PIXELS(898));
  objectEvent->motionFlags = stoppedMotion;
  objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  objectEventVisual->image.priority = 0;
  currentObjectEvent = FieldCamera_GetScriptObject();
  currentObjectEvent->targetX = OBJ_EVENT_NO_TARGET;
  currentObjectEvent = FieldCamera_GetScriptObject();
  currentObjectEvent->targetY = OBJ_EVENT_NO_TARGET;
  currentObjectEvent = FieldCamera_GetScriptObject();
  currentObjectEvent->targetZ = OBJ_EVENT_NO_TARGET;
  currentObjectEvent = FieldCamera_GetScriptObject();
  currentObjectEvent->velocityX = stoppedVelocity;
  currentObjectEvent = FieldCamera_GetScriptObject();
  currentObjectEvent->velocityY = stoppedVelocity;
  currentObjectEvent = FieldCamera_GetScriptObject();
  currentObjectEvent->velocityZ = stoppedVelocity;
  Task_Wait(1);
  FieldCamera_MoveTo(PIXELS(247), PIXELS(128), PIXELS(917), 0);
  MapGrid_CenterOnFollowedPoint();
  Task_Wait(1);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(30);
  Task_Wait(30);
  // Follow the boulder script until its eighth movement phase.
  FieldScheduler_AddCallback(ValeStorm_UpdateBoulderShadow, TASK_PRIORITY_MAP);
  ObjectEvent_SetActionScript(LOCALID_BOULDER,
                              gValeStorm_BoulderRollsDownriver);
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(3) / 100);
  FieldCamera_MoveTo(PIXELS(175), PIXELS(96), PIXELS(1086), 1);
  do {
    Task_Wait(1);
  } while ((s16)objectEvent->work != 8);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  Task_Wait(60);
  MapGrid_WaitForShakeToSettle();
  currentObjectEvent = FieldCamera_GetScriptObject();
  currentObjectEvent->targetX = OBJ_EVENT_NO_TARGET;
  currentObjectEvent = FieldCamera_GetScriptObject();
  currentObjectEvent->targetY = OBJ_EVENT_NO_TARGET;
  currentObjectEvent = FieldCamera_GetScriptObject();
  currentObjectEvent->targetZ = OBJ_EVENT_NO_TARGET;
  resetValue = 0;
  currentObjectEvent = FieldCamera_GetScriptObject();
  currentObjectEvent->velocityX = resetValue;
  currentObjectEvent = FieldCamera_GetScriptObject();
  currentObjectEvent->velocityY = resetValue;
  currentObjectEvent = FieldCamera_GetScriptObject();
  currentObjectEvent->velocityZ = resetValue;
  Task_Remove(ValeStorm_UpdateBoulderShadow);
  ObjectEvent_Stop(LOCALID_BOULDER);
  Task_Wait(1);
  ObjectEvent_SetAnimation(LOCALID_BOULDER, ANIM_STILL);
  groupObjectEvent->scaleX = 0x14000;
  groupObjectEvent->scaleY = 0x14000;
  groupVisual->offsetY = 2;
  groupVisual->scale = 0x14000;
  objectEvent->scaleX = 2 * Q16_ONE;
  objectEvent->scaleY = 2 * Q16_ONE;
  objectEvent->x.fixed = resetValue;
  objectEvent->z.fixed = resetValue;
  objectEvent->targetX = resetValue;
  objectEvent->targetZ = resetValue;
  Task_Wait(1);
  // Stage the riverbank reactions between lightning flashes.
  ObjectEvent_SetAnimation(LOCALID_FELIX, ANIM_FELIX_CLING_STILL);
  ObjectEvent_SetPosition(LOCALID_DORA, PIXELS(169), PIXELS(1264));
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTH, 0);
  ObjectEvent_SetAnimation(LOCALID_DORA, ANIM_DORA_RUN);
  ObjectEvent_SetPosition(LOCALID_JENNA, PIXELS(151), PIXELS(1292));
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_WEST, 0);
  ObjectEvent_SetAnimation(LOCALID_JENNA, ANIM_JENNA_RUN);
  ObjectEvent_SetPosition(LOCALID_EXHAUSTED_STRONGMAN, PIXELS(170),
                          PIXELS(1313));
  ObjectEvent_FaceDirection(LOCALID_EXHAUSTED_STRONGMAN, FACING_SOUTHWEST, 0);
  ObjectEvent_SetAnimation(LOCALID_EXHAUSTED_STRONGMAN, ANIM_STRONGMAN_RUN);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(185), PIXELS(1333));
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_ISAAC_RECOIL);
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(169), PIXELS(1384));
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH, 0);
  ObjectEvent_SetAnimation(LOCALID_GARET, ANIM_STILL);
  FieldCamera_MoveTo(PIXELS(166), 0, PIXELS(1337), 0);
  MapGrid_CenterOnFollowedPoint();
  objectEvent->motionFlags = resetValue;
  objectEvent->targetX = OBJ_EVENT_NO_TARGET;
  objectEvent->targetY = OBJ_EVENT_NO_TARGET;
  objectEvent->targetZ = OBJ_EVENT_NO_TARGET;
  ValeStorm_FlashLightning();
  ObjectEvent_SetPosition(LOCALID_BOULDER_SHADOW, PIXELS(218), PIXELS(1176));
  FieldCamera_MoveTo(PIXELS(210), 0, PIXELS(1196), 0);
  MapGrid_CenterOnFollowedPoint();
  groupObjectEvent->scaleX = 2 * Q16_ONE;
  groupObjectEvent->scaleY = 2 * Q16_ONE;
  FieldScheduler_AddCallback(ValeStorm_FlickerBoulderShadow, TASK_PRIORITY_MAP);
  ObjectEvent_Stop(LOCALID_KYLE);
  ObjectEvent_Stop(LOCALID_JENNAS_FATHER);
  ObjectEvent_Stop(LOCALID_JENNAS_MOTHER);
  Task_Wait(1);
  groupObjectEvent = ObjectEvent_Get(LOCALID_KYLE);
  groupVisual = groupObjectEvent->sprite;
  groupObjectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  groupObjectEvent->scaleX = Q16_ONE;
  groupObjectEvent->scaleY = Q16_ONE;
  groupObjectEvent->facing = FACING_NORTH + FACING_STEP;
  groupVisual->image.priority = 0;
  ObjectEvent_SetAnimation(LOCALID_KYLE, ANIM_STILL);
  groupObjectEvent = ObjectEvent_Get(LOCALID_JENNAS_FATHER);
  groupVisual = groupObjectEvent->sprite;
  groupObjectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  groupObjectEvent->scaleX = Q16_ONE;
  groupObjectEvent->scaleY = Q16_ONE;
  groupObjectEvent->facing = FACING_NORTHWEST + FACING_STEP;
  groupVisual->image.priority = 0;
  ObjectEvent_SetAnimation(LOCALID_JENNAS_FATHER, ANIM_PARENT_RECOIL);
  groupObjectEvent = ObjectEvent_Get(LOCALID_JENNAS_MOTHER);
  groupVisual = groupObjectEvent->sprite;
  groupObjectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  groupObjectEvent->scaleX = Q16_ONE;
  groupObjectEvent->scaleY = Q16_ONE;
  groupObjectEvent->facing = FACING_NORTHWEST + FACING_STEP;
  groupVisual->image.priority = 0;
  ObjectEvent_SetAnimation(LOCALID_JENNAS_MOTHER, ANIM_PARENT_RECOIL);
  groupObjectEvent = ObjectEvent_Get(LOCALID_BOULDER_SHADOW);
  groupVisual = groupObjectEvent->sprite;
  ValeStorm_FlashLightning();
  objectEvent->y.fixed = PIXELS(48);
  objectEvent->x.fixed = PIXELS(214);
  objectEvent->z.fixed = PIXELS(1216);
  objectEvent->targetX = OBJ_EVENT_NO_TARGET;
  objectEvent->targetY = OBJ_EVENT_NO_TARGET;
  objectEvent->targetZ = OBJ_EVENT_NO_TARGET;
  groupVisual->image.priority = 1;
  ObjectEvent_SetPosition(LOCALID_BOULDER_SHADOW, PIXELS(214), PIXELS(1216));
  ObjectEvent_FaceDirection(LOCALID_JENNAS_FATHER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNAS_MOTHER, FACING_NORTH, 20);
  GameFlag_Set(FLAG_WEATHER_HELD);
  TilesetAnim_Pause(0);
  TilesetAnim_Pause(1);
  TilesetAnim_Pause(2);
  TilesetAnim_Pause(3);
  TilesetAnim_Pause(4);
  TilesetAnim_Pause(5);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(120);
  Task_Wait(160);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_BG);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(80);
  Script_Wait(80);
  Script_Wait(100);
  Task_Remove(ValeStorm_FlickerBoulderShadow);
  groupVisual->scale = groupObjectEvent->scaleX;
  GameFlag_Clear(FLAG_WEATHER_HELD);
  TilesetAnim_Resume(0);
  TilesetAnim_Resume(1);
  TilesetAnim_Resume(2);
  TilesetAnim_Resume(3);
  TilesetAnim_Resume(4);
  TilesetAnim_Resume(5);
  // Restore the riverbank and the idle object event scripts.
  ValeStorm_RebuildRiverbank();
  ObjectEvent_SetPosition(LOCALID_DORA, PIXELS(165), PIXELS(1229));
  ObjectEvent_SetAnimation(LOCALID_DORA, ANIM_STAND);
  objectEvent = ObjectEvent_Get(LOCALID_DORA);
  objectEvent->facing = FACING_NORTHEAST;
  random = Random_Next();
  objectEvent->timer = Math_RemainderUnsigned(random, 90) + 60;
  departureActions = gValeStorm_SquashAndStretchScript;
  objectEvent->work = 1;
  ObjectEvent_SetActionScript(LOCALID_DORA, departureActions);
  ObjectEvent_SetPosition(LOCALID_JENNA, PIXELS(165), PIXELS(1254));
  ObjectEvent_SetAnimation(LOCALID_JENNA, ANIM_STAND);
  objectEvent = ObjectEvent_Get(LOCALID_JENNA);
  objectEvent->facing = FACING_NORTHEAST;
  random = Random_Next();
  objectEvent->timer = Math_RemainderUnsigned(random, 90) + 60;
  objectEvent->work = 2;
  ObjectEvent_SetActionScript(LOCALID_JENNA, departureActions);
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(152), PIXELS(1285));
  ObjectEvent_SetAnimation(LOCALID_GARET, ANIM_STAND);
  objectEvent = ObjectEvent_Get(LOCALID_GARET);
  objectEvent->facing = FACING_NORTHEAST;
  random = Random_Next();
  objectEvent->timer = Math_RemainderUnsigned(random, 90) + 60;
  objectEvent->work = 3;
  ObjectEvent_SetActionScript(LOCALID_GARET, departureActions);
  ObjectEvent_SetPosition(LOCALID_EXHAUSTED_STRONGMAN, PIXELS(180),
                          PIXELS(1311));
  objectEvent = ObjectEvent_Get(LOCALID_EXHAUSTED_STRONGMAN);
  objectEvent->facing = FACING_NORTHEAST;
  random = Random_Next();
  objectEvent->timer = Math_RemainderUnsigned(random, 90) + 60;
  objectEvent->work = 4;
  ObjectEvent_SetActionScript(LOCALID_EXHAUSTED_STRONGMAN, departureActions);
  ObjectEvent_SetAnimation(LOCALID_EXHAUSTED_STRONGMAN,
                           ANIM_STRONGMAN_SPRAWLED);
  ObjectEvent_GetById(LOCALID_GARET)->priorityFlags &=
      ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  ObjectEvent_GetById(LOCALID_EXHAUSTED_STRONGMAN)->priorityFlags &=
      ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  FieldScheduler_AddCallback(ValeStorm_CopyLeaderSpritePriority,
                             TASK_PRIORITY_MAP);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(181), PIXELS(1273));
  currentObjectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  currentObjectEvent->facing = FACING_NORTHEAST;
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  FieldCamera_MoveTo(PIXELS(181), 0, PIXELS(1273), 0);
  MapGrid_CenterOnFollowedPoint();
  ObjectEvent_SetPosition(LOCALID_KYLE, 0, 0);
  ObjectEvent_SetPosition(LOCALID_BOULDER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_JENNAS_FATHER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_JENNAS_MOTHER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_FELIX, 0, 0);
  ObjectEvent_SetPosition(LOCALID_BOULDER_SHADOW, 0, 0);
  ObjectEvent_SetPosition(LOCALID_FIRST_ROCK + 6, PIXELS(144), PIXELS(1070));
  ObjectEvent_SetPosition(LOCALID_FIRST_ROCK + 7, PIXELS(276), PIXELS(1270));
  Task_Wait(60);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(80);
  Script_Wait(60);
  Map_PlayQueuedMusic();
  Script_Wait(60);
  Party_RemoveMemberRestored(1);
  Weather_ResumeLightning();
}

/// A lightning flash that freezes the picture: holds the rain
/// (FLAG_WEATHER_HELD) and pauses the map's tileset animations 0 to 5,
/// shows the scene in PALETTE_TINT_WARM for two seconds, fades it to
/// black, then lets the rain and the animations run again.
static void ValeStorm_FlashLightning(void) {
  WaitFrames(20);
  GameFlag_Set(FLAG_WEATHER_HELD);
  TilesetAnim_Pause(0);
  TilesetAnim_Pause(1);
  TilesetAnim_Pause(2);
  TilesetAnim_Pause(3);
  TilesetAnim_Pause(4);
  TilesetAnim_Pause(5);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(1);
  WaitFrames(120);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  WaitFrames(60);
  GameFlag_Clear(FLAG_WEATHER_HELD);
  TilesetAnim_Resume(0);
  TilesetAnim_Resume(1);
  TilesetAnim_Resume(2);
  TilesetAnim_Resume(3);
  TilesetAnim_Resume(4);
  TilesetAnim_Resume(5);
}

/// The callback that keeps LOCALID_BOULDER_SHADOW on the Boulder as it comes
/// down the river: the shadow swells or shrinks by the step of
/// gValeStorm_BoulderRollsDownriver the Boulder is on (its work value), shows
/// on odd frames only, and the map shakes as the step's timer counts down.
static void ValeStorm_UpdateBoulderShadow(void) {
  struct ObjectEvent *source;
  struct ObjectEvent *shadow;
  struct Sprite *sprite;
  s16 *timer;

  source = ObjectEvent_GetById(LOCALID_BOULDER);
  shadow = ObjectEvent_GetById(LOCALID_BOULDER_SHADOW);
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

/// The callback that moves LOCALID_BOULDER_SHADOW between two heights on
/// alternate frames, during the frozen flashes.
static void ValeStorm_FlickerBoulderShadow(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_BOULDER_SHADOW);
  u32 frame = gFrameCount;
  struct Sprite *sprite = objectEvent->sprite;

  if (frame & 1) {
    sprite->offsetY = 2;
  } else {
    sprite->offsetY = 64;
  }
}

/// An exhausted strongman tries to go for help and can't move. In Vale on the
/// stormy night, after the Boulder, the strongman who came to help tries to go
/// for help himself. "I can't move..." Reached by talking to him
/// (LOCALID_EXHAUSTED_STRONGMAN); the first time sets
/// FLAG_VALE_STORM_STRONGMAN_TRIED_TO_GO.
static void EventScript_StrongmanTriesToGoForHelp(void) {
  struct ObjectEvent *villager;
  struct ObjectEvent *leader;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  villager = ObjectEvent_GetById(LOCALID_EXHAUSTED_STRONGMAN);
  FieldScript_Start();
  if (GameFlag_IsSet(FLAG_VALE_STORM_STRONGMAN_TRIED_TO_GO) != 0) {
    ObjectMotion_EnableActionAndResetMotion(LOCALID_EXHAUSTED_STRONGMAN);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_EXHAUSTED_STRONGMAN, 2);
    FieldScript_WaitFrames(40);
    if ((s16)leader->facing >= 0) {
      Engine_ObjectEventSetAnimation(LOCALID_EXHAUSTED_STRONGMAN,
                                     ANIM_STRONGMAN_PROPPED_UP);
    } else {
      Engine_ObjectEventSetAnimation(LOCALID_EXHAUSTED_STRONGMAN,
                                     ANIM_STRONGMAN_PROPPED_UP_OTHER_WAY);
    }
    ObjectMotion_SetVarCbAndRefresh(LOCALID_EXHAUSTED_STRONGMAN, 2);
    FieldScript_WaitFrames(20);
    FieldScript_SetMessageId(MsgValeICantMoveGetHelp);
    Script_ShowMessage(LOCALID_EXHAUSTED_STRONGMAN, 0);
    ObjectEvent_SetActionScript(LOCALID_EXHAUSTED_STRONGMAN,
                                gValeStorm_SquashAndStretchScript);
    Engine_ObjectEventSetAnimation(LOCALID_EXHAUSTED_STRONGMAN,
                                   ANIM_STRONGMAN_SPRAWLED);
  } else {
    ObjectMotion_EnableActionAndResetMotion(LOCALID_EXHAUSTED_STRONGMAN);
    villager->scaleX = Q16_ONE;
    villager->scaleY = Q16_ONE;
    ObjectEvent_FaceDirection(LOCALID_EXHAUSTED_STRONGMAN,
                              FACING_EAST + FACING_STEP, 0);
    if ((s16)leader->facing >= 0) {
      Engine_ObjectEventSetAnimation(LOCALID_EXHAUSTED_STRONGMAN,
                                     ANIM_STRONGMAN_PROPPED_UP);
    } else {
      Engine_ObjectEventSetAnimation(LOCALID_EXHAUSTED_STRONGMAN,
                                     ANIM_STRONGMAN_PROPPED_UP_OTHER_WAY);
    }
    FieldScript_WaitFrames(20);
    FieldScript_SetMessageId(MsgValeIllGo);
    Script_ShowMessageAndWait(LOCALID_EXHAUSTED_STRONGMAN, 0, 20);
    Engine_ObjectEventSetAnimation(LOCALID_EXHAUSTED_STRONGMAN, ANIM_STAND);
    ObjectMotion_Launch(LOCALID_EXHAUSTED_STRONGMAN, 4, 0);
    FieldScript_WaitFrames(80);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_EXHAUSTED_STRONGMAN, 2);
    FieldScript_WaitFrames(40);
    if ((s16)leader->facing >= 0) {
      Engine_ObjectEventSetAnimation(LOCALID_EXHAUSTED_STRONGMAN,
                                     ANIM_STRONGMAN_PROPPED_UP);
    } else {
      Engine_ObjectEventSetAnimation(LOCALID_EXHAUSTED_STRONGMAN,
                                     ANIM_STRONGMAN_PROPPED_UP_OTHER_WAY);
    }
    FieldScript_WaitFrames(2);
    ObjectMotion_Launch(LOCALID_EXHAUSTED_STRONGMAN, 2, 0);
    FieldScript_WaitFrames(60);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_EXHAUSTED_STRONGMAN, 2);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_EXHAUSTED_STRONGMAN, 0);
    ObjectEvent_SetActionScript(LOCALID_EXHAUSTED_STRONGMAN,
                                gValeStorm_SquashAndStretchScript);
    Engine_ObjectEventSetAnimation(LOCALID_EXHAUSTED_STRONGMAN,
                                   ANIM_STRONGMAN_SPRAWLED);
    GameFlag_Set(FLAG_VALE_STORM_STRONGMAN_TRIED_TO_GO);
  }
  FieldScript_End();
}

/// As Isaac leaves the river to get help, Garet will not let him go alone and
/// goes after him. "I can't let him go alone..."
/// Called by EventScript_LeaveToValeGate after the Boulder swept Felix, his
/// parents and Kyle away; sets FLAG_VALE_STORM_GARET_LEFT, which has the two
/// meet the strangers on the way (vale_gate.c).
static void ValeStorm_GaretFollowsIsaac(void) {
  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_GARET);
  Task_Remove(ValeStorm_CopyLeaderSpritePriority);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 480, 1392);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST + FACING_STEP, 20);
  ObjectEvent_Get(LOCALID_GARET)->priorityFlags |= OBJ_EVENT_PRIORITY_AUTOMATIC;
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(249), PIXELS(1240));
  WaitFrames(1);
  FieldScript_SetMessageId(MsgValeIsaac);
  Script_ShowMessage(SPEAKER_PORTRAIT_LEFT | LOCALID_GARET, 0);
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(172), PIXELS(1278));
  WaitFrames(1);
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  FieldCamera_MoveTo(PIXELS(162), 0, PIXELS(1285), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(40);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | LOCALID_GARET, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | LOCALID_GARET, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST + FACING_STEP, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(2), PIXELS(1));
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 165, 1300);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 195, 1432);
  GameFlag_Set(FLAG_VALE_STORM_GARET_LEFT);
}

/// Redraws the riverbank as the Boulder left it, pictures and metatile
/// behaviors. Called by ValeStorm_ShowBoulderSweepingDownriver and, on later
/// arrivals, by ValeStorm_Initialize.
static void ValeStorm_RebuildRiverbank(void) {
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
/// Reached by stepping on metatile behavior 16 until
/// FLAG_VALE_STORM_THIRD_MONSTER_FOUGHT is set.
static void EventScript_FightThirdMonster(void) {
  struct EventWork *state;

  FieldScript_Start();
  state = gFieldWork;
  state->transition = MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  state->transitionFrames = 64;
  Engine_GameFlagSet(FLAG_VALE_STORM_THIRD_MONSTER_FOUGHT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 2);
  Engine_GameFlagSet(FLAG_VALE_STORM_MONSTER_FOUGHT);
  FieldScript_End();
}

/// In Vale on the stormy night, another monster attacks (slot 3 of
/// ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT).
/// Reached by stepping on metatile behavior 17 until
/// FLAG_VALE_STORM_FOURTH_MONSTER_FOUGHT is set.
static void EventScript_FightFourthMonster(void) {
  struct EventWork *state;

  FieldScript_Start();
  state = gFieldWork;
  state->transition = MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  state->transitionFrames = 64;
  Engine_GameFlagSet(FLAG_VALE_STORM_FOURTH_MONSTER_FOUGHT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 3);
  Engine_GameFlagSet(FLAG_VALE_STORM_MONSTER_FOUGHT);
  FieldScript_End();
}

/// An elder calling up Psynergy, for 40 frames once the camera reaches the
/// Boulder: the sprite shows palette 7 for two frames in four, and a pair of
/// Psynergy sparks rises and falls every 16 frames plus 8 for each step of
/// sShakeShift.
static void
ValeStorm_FlashPaletteAndSpawnEffects(struct ObjectEvent *objectEvent) {
  s32 spawnInterval;

  if ((gFrameCount & 2) != 0) {
    ObjectEvent_SetColorMode(objectEvent, 7);
  } else {
    ObjectEvent_SetColorMode(objectEvent, 0);
  }
  {
    volatile u32 *frame = &gFrameCount;

    spawnInterval = (sShakeShift << 3) + 16;
    if (Math_RemainderUnsigned(*frame, spawnInterval) == 0) {
      ValeStorm_SpawnEffectPair(objectEvent);
    }
  }
}

/// An elder holding the Boulder: the sprite cycles through palettes 0 to 5,
/// and sparks rise and fall as in ValeStorm_FlashPaletteAndSpawnEffects.
static void
ValeStorm_CyclePaletteAndSpawnEffects(struct ObjectEvent *objectEvent) {
  s32 paletteFrame;
  s32 spawnInterval;

  if ((gFrameCount & 1) != 0) {
    paletteFrame = (gFrameCount >> 1);
    ObjectEvent_SetColorMode(objectEvent,
                             Math_RemainderUnsigned(paletteFrame, 6));
  }
  spawnInterval = (sShakeShift << 3) + 16;
  if (Math_RemainderUnsigned(gFrameCount, spawnInterval) == 0) {
    ValeStorm_SpawnEffectPair(objectEvent);
  }
}

/// Cycles the sprite through palettes 0 to 5, a step every second frame.
static void ValeStorm_CycleObjectPalettes(struct ObjectEvent *objectEvent) {
  s32 paletteFrame;

  if ((gFrameCount & 1) != 0) {
    paletteFrame = (gFrameCount >> 1);
    ObjectEvent_SetColorMode(objectEvent,
                             Math_RemainderUnsigned(paletteFrame, 6));
  }
}

#include "common/effect_vertical.h"

/// Spawns a pair of Psynergy sparks on an elder, one rising and one falling
/// (common/effect_vertical.h), with SE_JUMP.
static void ValeStorm_SpawnEffectPair(struct ObjectEvent *parent) {
  struct ObjectEvent *pair[2];
  struct ObjectEvent *child;
  struct Sprite *sprite;
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  s32 i;

  Sound_PlaySong(SE_JUMP);
  for (i = 0; i < 2; ++i) {
    child = ObjectEvent_Create(26, parent->x.fixed, parent->y.fixed,
                               parent->z.fixed);
    pair[i] = child;
    if (child != NULL) {
      child->groundHeight = parent->groundHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->linkedObject = parent;
      if (sprite != NULL) {
        Sprite_StartAnim(sprite, 0);
        sprite->flags = 0;
        VramCache_FreeEntry(sprite->slot);
        sprite->slot = effectWork->tileSlot;
        sprite->retainCacheEntry = 1;
        sprite->image.tile =
            (gVramCacheEntries[sprite->slot].offset >> 5) & 0x3ff;
        sprite->image.fullColor = 0;
        sprite->image.shape = 1;
        sprite->image.size = 2;
        sprite->entries[0]->frame = 0;
      }
    }
  }
  {
    struct ObjectEvent *effect = pair[0];
    struct Sprite *effectSprite = effect->sprite;

    effect->update = FieldEffect_AnimateVerticalNegative;
    effectSprite->image.priority = parent->sprite->image.priority;
  }
  {
    struct ObjectEvent *effect = pair[1];
    struct Sprite *effectSprite = effect->sprite;

    effectSprite->image.priority = parent->sprite->image.priority;
    effect->update = FieldEffect_AnimateVerticalPositive;
    effect->priorityFlags = 2;
  }
}

#include "common/effect_psynergy.h"

/// Cycles the four elders' palettes and effects while they hold the Boulder;
/// the westernmost elder's stop once that elder lets go (sShakeDone).
static void ValeStorm_UpdateBoulderMapEffects(void) {
  ValeStorm_CyclePaletteAndSpawnEffects(
      ObjectEvent_GetById(LOCALID_ELDER_EAST));
  ValeStorm_CyclePaletteAndSpawnEffects(
      ObjectEvent_GetById(LOCALID_ELDER_WEST));
  ValeStorm_CyclePaletteAndSpawnEffects(
      ObjectEvent_GetById(LOCALID_ELDER_FAR_EAST));
  if (sShakeDone == 0) {
    ValeStorm_CyclePaletteAndSpawnEffects(
        ObjectEvent_GetById(LOCALID_ELDER_FAR_WEST));
  }
}

/// The callback that cycles the Boulder's palettes while the elders hold it.
static void ValeStorm_CycleBoulderPalette(void) {
  ValeStorm_CycleObjectPalettes(ObjectEvent_GetById(LOCALID_BOULDER));
}

/// Keeps Garet and the man who came to help (LOCALID_EXHAUSTED_STRONGMAN) at
/// the leader's sprite priority while they stand by the river.
static void ValeStorm_CopyLeaderSpritePriority(void) {
  struct Sprite *sourceSprite;
#if !TBS_PATCH_3
  struct Sprite *targetSprite;
  struct Sprite *secondTargetSprite;
#endif

  sourceSprite = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->sprite;
#if TBS_PATCH_3
  ObjectMotion_SetActionVariant(LOCALID_GARET, sourceSprite->image.priority);
  ObjectMotion_SetActionVariant(LOCALID_EXHAUSTED_STRONGMAN,
                                sourceSprite->image.priority);
#else
  targetSprite = ObjectEvent_GetById(LOCALID_GARET)->sprite;
  targetSprite->image.priority = sourceSprite->image.priority;
  secondTargetSprite = ObjectEvent_GetById(LOCALID_EXHAUSTED_STRONGMAN)->sprite;
  secondTargetSprite->image.priority = sourceSprite->image.priority;
#endif
}

/// In Vale on the stormy night, searching a broken door: it won't open, the
/// impact having knocked it off its hinges.
static void EventScript_SearchBrokenDoor(void) {
  FieldScript_Start();
  FieldMessage_Show(MsgValeDoorWontOpen, 1);
  FieldScript_End();
}

/// In Vale on the stormy night, searching the chest Garet was trying to save.
/// "It's Garet's chest of valuables."
static void EventScript_SearchValuablesChest(void) {
  FieldScript_Start();
  FieldMessage_Show(MsgValeSChestValuables, 1);
  FieldScript_End();
}

/// ValeStorm_SetObjectEventFrameMode's parameter for mode 0, by the frame
/// count.
u8 gValeStorm_FrameModes[] = {7, 0, 0, 0};

// The object events' scripts, positions in pixels.

/// The stout man runs off toward the plaza after shoving Isaac.
s32 gValeStorm_ShovingManScript[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(185), 0, PIXELS(824)),
    SC_SYNC,         SC_MOVE(PIXELS(179), 0, PIXELS(836)),
    SC_SYNC,         SC_MOVE(PIXELS(226), 0, PIXELS(840)),
    SC_SYNC,         SC_MOVE(PIXELS(226), 0, PIXELS(944)),
    SC_SYNC,         SC_POSITION(0, 0, 0),
    SC_ANIMATION(1), SC_END,
};

/// Garet runs up to Isaac (EventScript_UnusedGaretJoinsIsaac).
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
/// (EventScript_WatchBoulderFall), and face each other.
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
/// (EventScript_BoulderSweepsThemAway).
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

/// A squash and stretch every 60 to 149 frames (Map_SquashAndStretch), for
/// the people straining at the river and, after the Boulder, those on the
/// bank.
s32 gValeStorm_SquashAndStretchScript[] = {
    SC_CALL(Map_SquashAndStretch),
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

struct MapWarpDest gValeStorm_WarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 167, 0, 1281, FACING_SOUTH, 0, -1, -1,
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
    {MAP_TABLE_END},
};

u32 gValeStorm_Warps[] = {
    MAP_WARPS(ValeStorm),
    MAP_WARP(VALE_STORM_TO_VALE_GATE, ValeGate, VALE_GATE_FROM_VALE),
    MAP_WARP(VALE_STORM_TO_VALE_KRADEN_NORTH, ValeKraden,
             VALE_KRADEN_FROM_VALE_NORTH),
    MAP_WARP(VALE_STORM_TO_VALE_KRADEN_SOUTH, ValeKraden,
             VALE_KRADEN_FROM_VALE_SOUTH),
    MAP_WARP(VALE_STORM_TO_VALE_INTERIORS_SHOPS_EAST, ValeInteriorsShops,
             VALE_INTERIORS_SHOPS_FROM_VALE_EAST),
    MAP_WARP(VALE_STORM_TO_VALE_INTERIORS_SHOPS_SOUTH, ValeInteriorsShops,
             VALE_INTERIORS_SHOPS_FROM_VALE_SOUTH),
    MAP_WARP(VALE_STORM_TO_VALE_INTERIORS_INN_ISAACS_HOUSE, ValeInteriorsInn,
             VALE_INTERIORS_INN_FROM_VALE),
    MAP_WARP(VALE_STORM_TO_VALE_INTERIORS_SHOPS_NORTH, ValeInteriorsShops,
             VALE_INTERIORS_SHOPS_FROM_VALE_NORTH),
    MAP_WARP(VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST, ValeInteriorsShops,
             VALE_INTERIORS_SHOPS_FROM_VALE_WEST),
    MAP_WARP(VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SECOND_DOOR,
             ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_WEST),
    MAP_WARP(VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SANCTUM_DOOR,
             ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_WEST),
    MAP_WARP(VALE_STORM_TO_VALE_INTERIORS_INN_WEST, ValeInteriorsInn,
             VALE_INTERIORS_INN_FROM_VALE_STORM_WEST),
    MAP_WARP(VALE_STORM_TO_VALE_INTERIORS_INN_EAST, ValeInteriorsInn,
             VALE_INTERIORS_INN_FROM_VALE_STORM_EAST),
    MAP_WARP(VALE_STORM_TO_BOULDER_PASSED, ValeStorm,
             VALE_STORM_FROM_BOULDER_PASSED),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gValeStorm_ObjectEventTemplates[] = {
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
        PIXELS(322), 0, PIXELS(1433), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(144), 0, PIXELS(1070), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(276), 0, PIXELS(1270), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
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
    { 0x120, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gValeStorm_BoulderWatchers[] = {
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
    { MAP_TABLE_END },
};
// clang-format on

struct MapEvent gValeStorm_Events[] = {
    {MAP_EVENT_COORD, 2, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_LeaveByMissingWarp)},
    {MAP_EVENT_COORD, 3, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_LeaveToValeKradenNorth)},
    {MAP_EVENT_COORD, 4, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_LeaveToValeKradenSouth)},
    {MAP_EVENT_COORD, 5, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_LeaveToValeGate)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_LeaveToValeInteriorsShopsEast)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_LeaveToValeInteriorsShops)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_LeaveToValeInteriorsInn)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_LeaveToValeInteriorsShopsNorth)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_LeaveToValeInteriorsShopsWest)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_UnusedLeaveToValeInteriorsShopsWestBySecondDoor)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_LeaveToValeInteriorsInnWest)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 13, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_LeaveToValeInteriorsInnEast)},
    {MAP_EVENT_TALK, LOCALID_GARET, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TalkToGaret)},
    {MAP_EVENT_TALK, LOCALID_KYLE, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TalkAboutSavingFelix)},
    {MAP_EVENT_TALK, LOCALID_JENNAS_FATHER, CONDITION_ALWAYS,
     MSGNO(MsgValeHangOnFelixHelpIsOn)},
    {MAP_EVENT_TALK, LOCALID_JENNAS_MOTHER, CONDITION_ALWAYS,
     MSGNO(MsgValeDontLetGoFelix)},
    {MAP_EVENT_TALK, LOCALID_DORA, CONDITION_ALWAYS,
     MSGNO(MsgValeNoItCantBeKyleIt)},
    {MAP_EVENT_TALK, LOCALID_JENNA, CONDITION_ALWAYS,
     MSGNO(MsgValeMomDadFelixPleaseDontLeave)},
    {MAP_EVENT_TALK, LOCALID_EXHAUSTED_STRONGMAN, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StrongmanTriesToGoForHelp)},
    {MAP_EVENT_TALK, LOCALID_FIRST_ROCK, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {MAP_EVENT_TALK, LOCALID_FIRST_ROCK + 1, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {MAP_EVENT_TALK, LOCALID_FIRST_ROCK + 2, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {MAP_EVENT_TALK, LOCALID_FIRST_ROCK + 3, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {MAP_EVENT_TALK, LOCALID_FIRST_ROCK + 4, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {MAP_EVENT_TALK, LOCALID_FIRST_ROCK + 5, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {MAP_EVENT_TALK, LOCALID_FIRST_ROCK + 6, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {MAP_EVENT_TALK, LOCALID_FIRST_ROCK + 7, CONDITION_ALWAYS,
     MSGNO(MsgValeRockCannotBeMoved)},
    {MAP_EVENT_COORD, 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_SetPathCellOverride)},
    {MAP_EVENT_COORD, 14, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ClearPathCellOverride)},
    {MAP_EVENT_COORD, 41, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_FirstRockFalls)},
    {MAP_EVENT_COORD, 42, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_SecondRockFalls)},
    {MAP_EVENT_COORD, 43, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ThirdRockFalls)},
    {MAP_EVENT_COORD, 44, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_FourthRockFalls)},
    {MAP_EVENT_COORD, 30, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_HearGaretStruggling)},
    {MAP_EVENT_COORD, 31, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_GaretCallsAfterIsaac)},
    {MAP_EVENT_COORD, 32, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_UnusedGaretJoinsIsaac)},
    {MAP_EVENT_COORD, 33, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_WarnAboutMtAleph)},
    {MAP_EVENT_COORD, 34, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_WatchBoulderFall)},
    {MAP_EVENT_COORD, 35, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_WatchAttemptToSaveFelix)},
    {MAP_EVENT_COORD, 36, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_BoulderSweepsThemAway)},
    {MAP_EVENT_COORD, 16, FLAG_VALE_STORM_THIRD_MONSTER_FOUGHT,
     EVENT_SCRIPT(EventScript_FightThirdMonster)},
    {MAP_EVENT_COORD, 17, FLAG_VALE_STORM_FOURTH_MONSTER_FOUGHT,
     EVENT_SCRIPT(EventScript_FightFourthMonster)},
    {BG_EVENT(BG_EVENT_SILENT), 24, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_SearchBrokenDoor)},
    {BG_EVENT(BG_EVENT_SILENT), 19, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_SearchValuablesChest)},
    {MAP_EVENTS_END},
};

// The doors' cell animations: two pictures of a door opening, from the map's
// cells below the village, each shown for two frames. The four kinds of door
// are the day map's (vale.c), and each table takes the name of a door it
// opens.

/// The Sanctum's door (ValeStorm_UnusedOpenSanctumDoor).
u16 gValeStorm_SanctumDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(0, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(2, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The door of Isaac's house, and of Garet's and the house by the river.
u16 gValeStorm_IsaacsDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(0, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(2, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The door of the house of Jenna's grandparents, and two more of the kind.
u16 gValeStorm_GrandparentsDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(4, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(6, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The door VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST goes through.
u16 gValeStorm_WestHouseDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(4, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(6, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};

/// Shuts a door of gValeStorm_IsaacsDoorOpens's kind, with the shut picture
/// of Isaac's door, after Dora and Jenna come out of the house by the river.
u16 gValeStorm_IsaacsDoorCloses[] = {
    MAPGRID_ANIMATION_STEP(0, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(50, 44, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};

/// Shuts a door of gValeStorm_GrandparentsDoorOpens's kind behind Dora and
/// Jenna as they go into the house by the river.
u16 gValeStorm_GrandparentsDoorCloses[] = {
    MAPGRID_ANIMATION_STEP(4, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(44, 108, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};
