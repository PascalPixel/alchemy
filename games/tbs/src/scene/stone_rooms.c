
/// @file
/// Stone rooms (SceneId_StoneRooms), a sheet of rooms drawn in stone: the game
/// packs rooms of several places that share a tile style onto one map, and
/// this one holds two, whose area names differ by entrance. The
/// entrance also picks the tables: the thieves' for STONE_ROOMS_FROM_VAULT
/// and STONE_ROOMS_FROM_LUNPA, the prisoner's for
/// STONE_ROOMS_FROM_VAULT_REVISIT. In story order, by the exits that lead in:
///
/// - Vault (STONE_ROOMS_FROM_VAULT and STONE_ROOMS_FROM_VAULT_REVISIT), the
/// cell
///   behind Vault's jail door. Once the party has beaten the three thieves in
///   the inn's loft (FLAG_VAULT_THIEVES_JAILED, log_rooms_a.c), coming in
///   from Vault finds them behind the bars, each with his own talk and Mind
///   Read. Once Vault has become its later scene (vault_revisit.c), whose
///   villagers say the thieves have broken out, its jail door comes in by
///   STONE_ROOMS_FROM_VAULT_REVISIT and finds a lone prisoner instead.
/// - Lunpa (STONE_ROOMS_FROM_LUNPA), Lunpa's hidden room, which the town's
///   hidden passage reaches (lunpa.c). Its six chests are each opened once
///   (FLAG_LUNPA_HIDDEN_ROOM_CHEST_*), CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_DOOR
///   leads back up into Lunpa, and arriving this way clears
///   FLAG_SHOW_AREA_NAME, so no area name is shown. The room uses the thieves'
///   tables, so its chests and exit sit in the same trigger table as the
///   thieves' talk and the Vault door. ★ Named from what the player sees: a
///   hidden stone room of six chests behind barred windows (its render)

#include "main_routine_work.h"
#include "module/scene/stone_rooms.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
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
#include "event.h"
#include "scene_tables.h"
#include "flags.h"
#include "item_ids.h"
#include "sounds.h"
#include "text/msg_ids.h"

/// The cell behaviors the jail's triggers answer.
enum StoneRoomsCellBehavior {
  CELL_BEHAVIOR_VAULT_JAIL_DOOR = 1,
  CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_DOOR = 2,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_WATER_OF_LIFE = 100,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_SMOKE_BOMB,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_LUCKY_MEDAL,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_ANTIDOTE,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_COINS,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_ELIXIR
};

/// The placed actors, numbering the rows of the table
/// Scene_GetActorPlacements returns: the three thieves of
/// gStoneRooms_ThiefActorPlacements, or the prisoner of
/// gStoneRooms_PrisonerActorPlacements in the same first slot.
enum JailActorId {
  /// The thieves' leader (SPRITE_MAN_WITH_LONG_DARK_HAIR), in the middle of the
  /// cell.
  /// ★ Named from the game: "Shut yer yaps!"
  /// (MsgVaultThiefLeaderSilencesThieves)
  ACTOR_THIEF_LEADER = ACTOR_FIRST_PLACED,
  /// The thief on the left (SPRITE_PERSON_IN_PURPLE).
  /// ★ Named from the game: "If only we had escaped to the cave northeast of
  /// Goma Range." (MsgVaultLeftThiefRegretsEscape)
  ACTOR_LEFT_THIEF,
  /// The thief on the right (SPRITE_PERSON_IN_PURPLE).
  /// ★ Named from the game: "Blast! I knew we should have skipped town sooner."
  /// (MsgVaultRightThiefRegretsStaying)
  ACTOR_RIGHT_THIEF,
  /// The lone prisoner after the thieves have broken out (SPRITE_STRONGMAN).
  /// ★ Named from the game: "I hit somebody, so they put me in here."
  /// (MsgVaultPrisonerRegretsFight)
  ACTOR_PRISONER = ACTOR_FIRST_PLACED
};

/// The jail's occupants count as nearer than they stand for talk and
/// Psynergy targets (Field_FindNearbyTalkTarget scales their distance by
/// 10/13), so the party reaches them across the bars.
enum { JAIL_REACH_ACROSS_BARS = ACTOR_NEAR_TALK | ACTOR_NEAR_PSYNERGY };

/// Returns the entrances from Vault, from Lunpa and from Vault's
/// revisit scene.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gStoneRooms_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

/// Returns the exits to Vault, to Vault's revisit scene and to Lunpa, each to
/// the entrance that scene names for the jail (VAULT_FROM_STONE_ROOMS,
/// VAULT_REVISIT_FROM_STONE_ROOMS and LUNPA_FROM_STONE_ROOMS).
const u32 *Scene_GetExits(void) { return gStoneRooms_Exits; }

/// Places the three jailed thieves (each only once FLAG_VAULT_THIEVES_JAILED
/// is set), or, entered from Vault's revisit scene
/// (STONE_ROOMS_FROM_VAULT_REVISIT, after they have broken out), the lone
/// prisoner.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.entrance == STONE_ROOMS_FROM_VAULT_REVISIT) {
    return gStoneRooms_PrisonerActorPlacements;
  }
  return gStoneRooms_ThiefActorPlacements;
}

/// Returns the thieves' triggers, which also hold the Lunpa room's chests and
/// exit, or the prisoner's when the party came in from Vault's revisit scene,
/// matching Scene_GetActorPlacements.
const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.entrance == STONE_ROOMS_FROM_VAULT_REVISIT) {
    return gStoneRooms_PrisonerTriggers;
  }
  return gStoneRooms_ThiefTriggers;
}

/// In Vault's jail, the thieves' leader glares at each of his men and snaps
/// "Shut yer yaps!", so that both start, then turns to the party: they got us
/// this time, but next time they'll pay for this.
///
/// Reached by talking to the leader (ACTOR_THIEF_LEADER) while
/// FLAG_VAULT_THIEVES_JAILED is set.
static void StoneRooms_LeaderSilencesThieves(void) {
  Event_Start();

  Actor_SetAngleToward(ACTOR_THIEF_LEADER, ACTOR_LEFT_THIEF, 0);
  Event_Wait(40);

  Actor_SetAngleToward(ACTOR_THIEF_LEADER, ACTOR_RIGHT_THIEF, 0);
  Event_Wait(40);

  // "Shut yer yaps!"; the two thieves start.
  Event_SetMessageId(MsgVaultThiefLeaderSilencesThieves);
  Event_ShowMessageAndWait(ACTOR_THIEF_LEADER, 0);

  Actor_StartRepeatedMotion(ACTOR_LEFT_THIEF, 2);
  Actor_RunRepeatedMotion(ACTOR_RIGHT_THIEF, 2);
  Event_Wait(20);

  // Turn from the two thieves to the party for the remaining dialogue.
  Actor_TurnToFaceEachOther(ACTOR_THIEF_LEADER, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);

  Actor_RunRepeatedMotion(ACTOR_THIEF_LEADER, 1);
  Event_Wait(20);

  Event_ShowMessageAndWait(ACTOR_THIEF_LEADER, 0);

  Event_End();
}

/// In Vault's jail, the thief on the left wishes they had escaped to the cave
/// northeast of Goma Range.
///
/// Reached by talking to him (ACTOR_LEFT_THIEF) while
/// FLAG_VAULT_THIEVES_JAILED is set.
static void StoneRooms_LeftThiefRegretsEscape(void) {
  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_LEFT_THIEF, 2);
  Event_Wait(20);
  Event_SetMessageId(MsgVaultLeftThiefRegretsEscape);
  Event_ShowMessageAndWait(ACTOR_LEFT_THIEF, 0);
  Event_End();
}

/// In Vault's jail, the thief on the right shakes his head: he knew they
/// should have skipped town sooner.
///
/// Reached by talking to him (ACTOR_RIGHT_THIEF) while
/// FLAG_VAULT_THIEVES_JAILED is set.
static void StoneRooms_RightThiefRegretsStaying(void) {
  Event_Start();
  Actor_SetAnimationAndWait(ACTOR_RIGHT_THIEF, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_SetMessageId(MsgVaultRightThiefRegretsStaying);
  Event_ShowMessageAndWait(ACTOR_RIGHT_THIEF, 0);
  Event_End();
}

/// Mind Read on the thieves' leader in Vault's jail: he'll be out of this jail
/// before you know it.
///
/// Reached by Mind Read on ACTOR_THIEF_LEADER while FLAG_VAULT_THIEVES_JAILED
/// is set; the two below are reached the same way on ACTOR_LEFT_THIEF and
/// ACTOR_RIGHT_THIEF.
static void StoneRooms_ReadLeaderMind(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultThiefLeaderThoughts);
  Event_ShowMessageAndWait(ACTOR_THIEF_LEADER, 0);
  Event_End();
}

/// Mind Read on the thief on the left: will he ever run free again?
static void StoneRooms_ReadLeftThiefMind(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultLeftThiefThoughts);
  Event_ShowMessageAndWait(ACTOR_LEFT_THIEF, 0);
  Event_End();
}

/// Mind Read on the thief on the right: is this because they stole Hammet's
/// Shaman's Rod?
static void StoneRooms_ReadRightThiefMind(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultRightThiefThoughts);
  Event_ShowMessageAndWait(ACTOR_RIGHT_THIEF, 0);
  Event_End();
}

/// The party steps out of Vault's jail into Vault, with the exit sound.
///
/// Reached by stepping on the door's cell (CELL_BEHAVIOR_VAULT_JAIL_DOOR) under
/// the thieves' table, that is, entered from Vault or Lunpa; takes
/// STONE_ROOMS_TO_VAULT to VAULT_FROM_STONE_ROOMS. The prisoner's table leaves
/// by a plain exit instead, STONE_ROOMS_TO_VAULT_REVISIT.
static void StoneRooms_LeaveForVault(void) {
  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(STONE_ROOMS_TO_VAULT);
}

/// Opens the map through a window transition and lets the party reach the
/// jail's occupants across the bars.
///
/// Called by the field once the actors are placed. Coming in through
/// Lunpa's hidden passage (STONE_ROOMS_FROM_LUNPA) clears
/// FLAG_SHOW_AREA_NAME, so no area name is shown; otherwise it adds
/// JAIL_REACH_ACROSS_BARS to the prisoner or to the three thieves.
s32 Scene_Initialize(void) {
  s16 entrance;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  entrance = gMainRoutineWork.entrance;

  if (entrance == STONE_ROOMS_FROM_LUNPA) {
    // XXX (no evidence): why, when gArea_NameRules gives "Lunpa" for this
    // entrance.
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  } else if (entrance == STONE_ROOMS_FROM_VAULT_REVISIT) {
    Actor_GetById(ACTOR_PRISONER)->collisionFlags |= JAIL_REACH_ACROSS_BARS;
  } else {
    Actor_GetById(ACTOR_THIEF_LEADER)->collisionFlags |= JAIL_REACH_ACROSS_BARS;
    Actor_GetById(ACTOR_LEFT_THIEF)->collisionFlags |= JAIL_REACH_ACROSS_BARS;
    Actor_GetById(ACTOR_RIGHT_THIEF)->collisionFlags |= JAIL_REACH_ACROSS_BARS;
  }

  return 0;
}

// clang-format off
const struct SceneEntrance gStoneRooms_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { STONE_ROOMS_FROM_VAULT, CONDITION_ALWAYS, 80, 0, 232, FACING_NORTH, 0, 16, 48, 304, 256, 0 },
    { STONE_ROOMS_FROM_LUNPA, CONDITION_ALWAYS, 544, 0, 280, FACING_NORTH, 0, 440, 48, 680, 312, 0 },
    { STONE_ROOMS_FROM_VAULT_REVISIT, CONDITION_ALWAYS, 80, 0, 232, FACING_NORTH, 0, 16, 48, 304, 256, 0 },
    { SCENE_TABLE_END },
};

const u32 gStoneRooms_Exits[] = {
    SCENE_EXITS(StoneRooms),
    SCENE_EXIT(STONE_ROOMS_TO_VAULT, Vault, VAULT_FROM_STONE_ROOMS),
    SCENE_EXIT(STONE_ROOMS_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_STONE_ROOMS),
    SCENE_EXIT(STONE_ROOMS_TO_LUNPA, Lunpa, LUNPA_FROM_STONE_ROOMS),
    SCENE_EXITS_END,
};

const struct ActorPlacement gStoneRooms_ThiefActorPlacements[] = {
    { SPRITE_MAN_WITH_LONG_DARK_HAIR, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), MOVEMENT_TYPE_STAND,
      PIXELS(168), 0, PIXELS(144), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PERSON_IN_PURPLE, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), MOVEMENT_TYPE_STAND,
      PIXELS(88), 0, PIXELS(144), FACING_SOUTH - FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_PERSON_IN_PURPLE, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), MOVEMENT_TYPE_STAND,
      PIXELS(248), 0, PIXELS(144), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

const struct ActorPlacement gStoneRooms_PrisonerActorPlacements[] = {
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(168), 0, PIXELS(144), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

const struct Trigger gStoneRooms_ThiefTriggers[] = {
    { TRIGGER_STEP, CELL_BEHAVIOR_VAULT_JAIL_DOOR, CONDITION_ALWAYS, EVENT(StoneRooms_LeaveForVault) },
    { TRIGGER_EXIT, CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_DOOR, CONDITION_ALWAYS, STONE_ROOMS_TO_LUNPA },
    { TRIGGER_TALK, ACTOR_THIEF_LEADER, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT(StoneRooms_LeaderSilencesThieves) },
    { TRIGGER_TALK, ACTOR_LEFT_THIEF, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT(StoneRooms_LeftThiefRegretsEscape) },
    { TRIGGER_TALK, ACTOR_RIGHT_THIEF, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT(StoneRooms_RightThiefRegretsStaying) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIEF_LEADER,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT(StoneRooms_ReadLeaderMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEFT_THIEF,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT(StoneRooms_ReadLeftThiefMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RIGHT_THIEF,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT(StoneRooms_ReadRightThiefMind) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_WATER_OF_LIFE, FLAG_LUNPA_HIDDEN_ROOM_CHEST_WATER_OF_LIFE,
      FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_SMOKE_BOMB, FLAG_LUNPA_HIDDEN_ROOM_CHEST_SMOKE_BOMB,
      FIND_ITEM(ITEM_SMOKE_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_LUCKY_MEDAL, FLAG_LUNPA_HIDDEN_ROOM_CHEST_LUCKY_MEDAL,
      FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_ANTIDOTE, FLAG_LUNPA_HIDDEN_ROOM_CHEST_ANTIDOTE,
      FIND_ITEM(ITEM_ANTIDOTE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_COINS, FLAG_LUNPA_HIDDEN_ROOM_CHEST_COINS, FIND_COINS(44) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_ELIXIR, FLAG_LUNPA_HIDDEN_ROOM_CHEST_ELIXIR,
      FIND_ITEM(ITEM_ELIXIR) },
    { TRIGGERS_END },
};
// clang-format on

const struct Trigger gStoneRooms_PrisonerTriggers[] = {
    {TRIGGER_EXIT, CELL_BEHAVIOR_VAULT_JAIL_DOOR, CONDITION_ALWAYS,
     STONE_ROOMS_TO_VAULT_REVISIT},
    {TRIGGER_TALK, ACTOR_PRISONER, CONDITION_ALWAYS,
     MsgVaultPrisonerRegretsFight},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRISONER, CONDITION_ALWAYS,
     MsgVaultPrisonerThoughts},
    {TRIGGERS_END},
};
