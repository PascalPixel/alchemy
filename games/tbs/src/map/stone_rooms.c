
/// @file
/// Vault's jail (MapId_StoneRooms), one map of two rooms whose area names
/// differ by warp destination. The warp destination also picks the tables: the
/// thieves' for STONE_ROOMS_FROM_VAULT and STONE_ROOMS_FROM_LUNPA, the
/// prisoner's for STONE_ROOMS_FROM_VAULT_REVISIT.
///
/// - Vault (STONE_ROOMS_FROM_VAULT and STONE_ROOMS_FROM_VAULT_REVISIT), the
/// cell
///   behind Vault's jail door. Once the party has beaten the three thieves in
///   the inn's loft (FLAG_VAULT_THIEVES_JAILED, vault_interiors.c), coming in
///   from Vault finds them behind the bars, each with his own talk and Mind
///   Read. Once Vault has become its later map (vault_revisit.c), whose
///   villagers say the thieves have broken out, its jail door comes in by
///   STONE_ROOMS_FROM_VAULT_REVISIT and finds a lone prisoner instead.
/// - Lunpa (STONE_ROOMS_FROM_LUNPA), the map's other room, which Lunpa's hidden
///   passage reaches (lunpa.c). Its six chests are each opened once
///   (FLAG_LUNPA_HIDDEN_ROOM_CHEST_*), MB_LUNPA_HIDDEN_ROOM_DOOR leads back up
///   into Lunpa, and arriving this way clears FLAG_SHOW_LOCATION_NAME, so no
///   area name is shown. The room uses the thieves' tables, so its chests and
///   warp sit in the same map event table as the thieves' talk and the Vault
///   door.

#include "save_block.h"
#include "unit/map/stone_rooms.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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
#include "ability_ids.h"
#include "script_layer.h"
#include "map_tables.h"
#include "flags.h"
#include "item_ids.h"
#include "songs.h"
#include "text/msg_ids.h"

/// The metatile behaviors the jail's map events answer.
enum StoneRoomsMetatileBehavior {
  MB_VAULT_JAIL_DOOR = 1,
  MB_LUNPA_HIDDEN_ROOM_DOOR = 2,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  MB_LUNPA_HIDDEN_ROOM_CHEST_WATER_OF_LIFE = 100,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  MB_LUNPA_HIDDEN_ROOM_CHEST_SMOKE_BOMB,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  MB_LUNPA_HIDDEN_ROOM_CHEST_LUCKY_MEDAL,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  MB_LUNPA_HIDDEN_ROOM_CHEST_ANTIDOTE,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  MB_LUNPA_HIDDEN_ROOM_CHEST_COINS,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  MB_LUNPA_HIDDEN_ROOM_CHEST_ELIXIR
};

/// The placed object events, numbering the rows of the table
/// Map_GetObjectEventTemplates returns: the three thieves of
/// gStoneRooms_ThiefObjectEventTemplates, or the prisoner of
/// gStoneRooms_PrisonerObjectEventTemplates in the same first slot.
enum JailLocalId {
  /// The thieves' leader (SPRITE_MAN_WITH_LONG_DARK_HAIR), in the middle of the
  /// cell.
  /// ★ Named from the game: "Shut yer yaps!"
  /// (MsgVaultThiefLeaderSilencesThieves)
  LOCALID_THIEF_LEADER = LOCALID_FIRST_PLACED,
  /// The thief on the left (SPRITE_PERSON_IN_PURPLE).
  /// ★ Named from the game: "If only we had escaped to the cave northeast of
  /// Goma Range." (MsgVaultLeftThiefRegretsEscape)
  LOCALID_LEFT_THIEF,
  /// The thief on the right (SPRITE_PERSON_IN_PURPLE).
  /// ★ Named from the game: "Blast! I knew we should have skipped town sooner."
  /// (MsgVaultRightThiefRegretsStaying)
  LOCALID_RIGHT_THIEF,
  /// The lone prisoner after the thieves have broken out (SPRITE_STRONGMAN).
  /// ★ Named from the game: "I hit somebody, so they put me in here."
  /// (MsgVaultPrisonerRegretsFight)
  LOCALID_PRISONER = LOCALID_FIRST_PLACED
};

/// The jail's occupants count as nearer than they stand for talk and
/// Psynergy targets (Field_FindNearbyTalkTarget scales their distance by
/// 10/13), so the party reaches them across the bars.
enum { JAIL_REACH_ACROSS_BARS = OBJ_EVENT_NEAR_TALK | OBJ_EVENT_NEAR_PSYNERGY };

/// Returns the warp destinations from Vault, from Lunpa and from Vault's
/// revisit map.
const struct MapWarpDest *Map_GetWarpDests(void) {
  return gStoneRooms_WarpDests;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

/// Returns the warps to Vault, to Vault's revisit map and to Lunpa, each to
/// the warp destination that map names for the jail (VAULT_FROM_STONE_ROOMS,
/// VAULT_REVISIT_FROM_STONE_ROOMS and LUNPA_FROM_STONE_ROOMS).
const u32 *Map_GetWarps(void) { return gStoneRooms_Warps; }

/// Places the three jailed thieves (each only once FLAG_VAULT_THIEVES_JAILED
/// is set), or, entered from Vault's revisit map
/// (STONE_ROOMS_FROM_VAULT_REVISIT, after they have broken out), the lone
/// prisoner.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  if (gSaveBlock.warpDest == STONE_ROOMS_FROM_VAULT_REVISIT) {
    return gStoneRooms_PrisonerObjectEventTemplates;
  }
  return gStoneRooms_ThiefObjectEventTemplates;
}

/// Returns the thieves' map events, which also hold the Lunpa room's chests and
/// warp, or the prisoner's when the party came in from Vault's revisit map,
/// matching Map_GetObjectEventTemplates.
const struct MapEvent *Map_GetEvents(void) {
  if (gSaveBlock.warpDest == STONE_ROOMS_FROM_VAULT_REVISIT) {
    return gStoneRooms_PrisonerEvents;
  }
  return gStoneRooms_ThiefEvents;
}

/// In Vault's jail, the thieves' leader glares at each of his men and snaps
/// "Shut yer yaps!", so that both start, then turns to the party: they got us
/// this time, but next time they'll pay for this.
///
/// Reached by talking to the leader (LOCALID_THIEF_LEADER) while
/// FLAG_VAULT_THIEVES_JAILED is set.
static void EventScript_LeaderSilencesThieves(void) {
  FieldScript_Start();

  ObjectMotion_SetAngleToward(LOCALID_THIEF_LEADER, LOCALID_LEFT_THIEF, 0);
  FieldScript_WaitFrames(40);

  ObjectMotion_SetAngleToward(LOCALID_THIEF_LEADER, LOCALID_RIGHT_THIEF, 0);
  FieldScript_WaitFrames(40);

  // "Shut yer yaps!"; the two thieves start.
  FieldScript_SetMessageId(MsgVaultThiefLeaderSilencesThieves);
  FieldScript_ShowMessageAndWait(LOCALID_THIEF_LEADER, 0);

  ObjectMotion_SetVariantCallback(LOCALID_LEFT_THIEF, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_RIGHT_THIEF, 2);
  FieldScript_WaitFrames(20);

  // Turn from the two thieves to the party for the remaining dialogue.
  ObjectEvent_LinkPair(LOCALID_THIEF_LEADER, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(20);

  ObjectMotion_SetVarCbAndRefresh(LOCALID_THIEF_LEADER, 1);
  FieldScript_WaitFrames(20);

  FieldScript_ShowMessageAndWait(LOCALID_THIEF_LEADER, 0);

  FieldScript_End();
}

/// In Vault's jail, the thief on the left wishes they had escaped to the cave
/// northeast of Goma Range.
///
/// Reached by talking to him (LOCALID_LEFT_THIEF) while
/// FLAG_VAULT_THIEVES_JAILED is set.
static void EventScript_LeftThiefRegretsEscape(void) {
  FieldScript_Start();
  ObjectMotion_SetVarCbAndRefresh(LOCALID_LEFT_THIEF, 2);
  FieldScript_WaitFrames(20);
  FieldScript_SetMessageId(MsgVaultLeftThiefRegretsEscape);
  FieldScript_ShowMessageAndWait(LOCALID_LEFT_THIEF, 0);
  FieldScript_End();
}

/// In Vault's jail, the thief on the right shakes his head: he knew they
/// should have skipped town sooner.
///
/// Reached by talking to him (LOCALID_RIGHT_THIEF) while
/// FLAG_VAULT_THIEVES_JAILED is set.
static void EventScript_RightThiefRegretsStaying(void) {
  FieldScript_Start();
  ObjectMotion_SetAnimationAndWait(LOCALID_RIGHT_THIEF, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  FieldScript_SetMessageId(MsgVaultRightThiefRegretsStaying);
  FieldScript_ShowMessageAndWait(LOCALID_RIGHT_THIEF, 0);
  FieldScript_End();
}

/// Mind Read on the thieves' leader in Vault's jail: he'll be out of this jail
/// before you know it.
///
/// Reached by Mind Read on LOCALID_THIEF_LEADER while FLAG_VAULT_THIEVES_JAILED
/// is set; the two below are reached the same way on LOCALID_LEFT_THIEF and
/// LOCALID_RIGHT_THIEF.
static void EventScript_ReadLeaderMind(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgVaultThiefLeaderThoughts);
  FieldScript_ShowMessageAndWait(LOCALID_THIEF_LEADER, 0);
  FieldScript_End();
}

/// Mind Read on the thief on the left: will he ever run free again?
static void EventScript_ReadLeftThiefMind(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgVaultLeftThiefThoughts);
  FieldScript_ShowMessageAndWait(LOCALID_LEFT_THIEF, 0);
  FieldScript_End();
}

/// Mind Read on the thief on the right: is this because they stole Hammet's
/// Shaman's Rod?
static void EventScript_ReadRightThiefMind(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgVaultRightThiefThoughts);
  FieldScript_ShowMessageAndWait(LOCALID_RIGHT_THIEF, 0);
  FieldScript_End();
}

/// The party steps out of Vault's jail into Vault, with the warp sound.
///
/// Reached by stepping on the door's cell (MB_VAULT_JAIL_DOOR) under the
/// thieves' table, that is, entered from Vault or Lunpa; takes
/// STONE_ROOMS_TO_VAULT to VAULT_FROM_STONE_ROOMS. The prisoner's table leaves
/// by a plain warp instead, STONE_ROOMS_TO_VAULT_REVISIT.
static void EventScript_LeaveForVault(void) {
  Sound_PlaySong(SE_MAP_EXIT);
  Import_ScriptRequestWarp(STONE_ROOMS_TO_VAULT);
}

/// Opens the map through a window transition and lets the party reach the
/// jail's occupants across the bars.
///
/// Called by the engine once the object events are placed. Coming in through
/// Lunpa's hidden passage (STONE_ROOMS_FROM_LUNPA) clears
/// FLAG_SHOW_LOCATION_NAME, so no area name is shown; otherwise it adds
/// JAIL_REACH_ACROSS_BARS to the prisoner or to the three thieves.
s32 Map_Initialize(void) {
  s16 warpDest;

  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  warpDest = gSaveBlock.warpDest;

  if (warpDest == STONE_ROOMS_FROM_LUNPA) {
    // XXX (no evidence): why, when gMapSec_NameRules gives "Lunpa" for this
    // warp destination.
    GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  } else if (warpDest == STONE_ROOMS_FROM_VAULT_REVISIT) {
    ObjectEvent_GetById(LOCALID_PRISONER)->collisionFlags |=
        JAIL_REACH_ACROSS_BARS;
  } else {
    ObjectEvent_GetById(LOCALID_THIEF_LEADER)->collisionFlags |=
        JAIL_REACH_ACROSS_BARS;
    ObjectEvent_GetById(LOCALID_LEFT_THIEF)->collisionFlags |=
        JAIL_REACH_ACROSS_BARS;
    ObjectEvent_GetById(LOCALID_RIGHT_THIEF)->collisionFlags |=
        JAIL_REACH_ACROSS_BARS;
  }

  return 0;
}

// clang-format off
const struct MapWarpDest gStoneRooms_WarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { STONE_ROOMS_FROM_VAULT, CONDITION_ALWAYS, 80, 0, 232, FACING_NORTH, 0, 16, 48, 304, 256, 0 },
    { STONE_ROOMS_FROM_LUNPA, CONDITION_ALWAYS, 544, 0, 280, FACING_NORTH, 0, 440, 48, 680, 312, 0 },
    { STONE_ROOMS_FROM_VAULT_REVISIT, CONDITION_ALWAYS, 80, 0, 232, FACING_NORTH, 0, 16, 48, 304, 256, 0 },
    { MAP_TABLE_END },
};

const u32 gStoneRooms_Warps[] = {
    MAP_WARPS(StoneRooms),
    MAP_WARP(STONE_ROOMS_TO_VAULT, Vault, VAULT_FROM_STONE_ROOMS),
    MAP_WARP(STONE_ROOMS_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_STONE_ROOMS),
    MAP_WARP(STONE_ROOMS_TO_LUNPA, Lunpa, LUNPA_FROM_STONE_ROOMS),
    MAP_WARPS_END,
};

const struct ObjectEventTemplate gStoneRooms_ThiefObjectEventTemplates[] = {
    { SPRITE_MAN_WITH_LONG_DARK_HAIR, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), MOVEMENT_TYPE_STAND,
      PIXELS(168), 0, PIXELS(144), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PERSON_IN_PURPLE, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), MOVEMENT_TYPE_STAND,
      PIXELS(88), 0, PIXELS(144), FACING_SOUTH - FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_PERSON_IN_PURPLE, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), MOVEMENT_TYPE_STAND,
      PIXELS(248), 0, PIXELS(144), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

const struct ObjectEventTemplate gStoneRooms_PrisonerObjectEventTemplates[] = {
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(168), 0, PIXELS(144), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

const struct MapEvent gStoneRooms_ThiefEvents[] = {
    { MAP_EVENT_COORD, MB_VAULT_JAIL_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveForVault) },
    { MAP_EVENT_WARP, MB_LUNPA_HIDDEN_ROOM_DOOR, CONDITION_ALWAYS, STONE_ROOMS_TO_LUNPA },
    { MAP_EVENT_TALK, LOCALID_THIEF_LEADER, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT_SCRIPT(EventScript_LeaderSilencesThieves) },
    { MAP_EVENT_TALK, LOCALID_LEFT_THIEF, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT_SCRIPT(EventScript_LeftThiefRegretsEscape) },
    { MAP_EVENT_TALK, LOCALID_RIGHT_THIEF, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT_SCRIPT(EventScript_RightThiefRegretsStaying) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_THIEF_LEADER,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT_SCRIPT(EventScript_ReadLeaderMind) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LEFT_THIEF,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT_SCRIPT(EventScript_ReadLeftThiefMind) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_RIGHT_THIEF,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT_SCRIPT(EventScript_ReadRightThiefMind) },
    { BG_EVENT(BG_EVENT_CHEST), MB_LUNPA_HIDDEN_ROOM_CHEST_WATER_OF_LIFE, FLAG_LUNPA_HIDDEN_ROOM_CHEST_WATER_OF_LIFE,
      FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { BG_EVENT(BG_EVENT_CHEST), MB_LUNPA_HIDDEN_ROOM_CHEST_SMOKE_BOMB, FLAG_LUNPA_HIDDEN_ROOM_CHEST_SMOKE_BOMB,
      FIND_ITEM(ITEM_SMOKE_BOMB) },
    { BG_EVENT(BG_EVENT_CHEST), MB_LUNPA_HIDDEN_ROOM_CHEST_LUCKY_MEDAL, FLAG_LUNPA_HIDDEN_ROOM_CHEST_LUCKY_MEDAL,
      FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_CHEST), MB_LUNPA_HIDDEN_ROOM_CHEST_ANTIDOTE, FLAG_LUNPA_HIDDEN_ROOM_CHEST_ANTIDOTE,
      FIND_ITEM(ITEM_ANTIDOTE) },
    { BG_EVENT(BG_EVENT_CHEST), MB_LUNPA_HIDDEN_ROOM_CHEST_COINS, FLAG_LUNPA_HIDDEN_ROOM_CHEST_COINS, FIND_COINS(44) },
    { BG_EVENT(BG_EVENT_CHEST), MB_LUNPA_HIDDEN_ROOM_CHEST_ELIXIR, FLAG_LUNPA_HIDDEN_ROOM_CHEST_ELIXIR,
      FIND_ITEM(ITEM_ELIXIR) },
    { MAP_EVENTS_END },
};
// clang-format on

const struct MapEvent gStoneRooms_PrisonerEvents[] = {
    {MAP_EVENT_WARP, MB_VAULT_JAIL_DOOR, CONDITION_ALWAYS,
     STONE_ROOMS_TO_VAULT_REVISIT},
    {MAP_EVENT_TALK, LOCALID_PRISONER, CONDITION_ALWAYS,
     MsgVaultPrisonerRegretsFight},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_PRISONER,
     CONDITION_ALWAYS, MsgVaultPrisonerThoughts},
    {MAP_EVENTS_END},
};
