
/// @file
/// Vault's jail (SceneId_VaultJail), one map of two rooms whose area names
/// differ by entrance. The entrance also picks the tables: the thieves' for
/// VAULT_JAIL_FROM_VAULT and VAULT_JAIL_FROM_LUNPA, the prisoner's for
/// VAULT_JAIL_FROM_VAULT_REVISIT.
///
/// - Vault (VAULT_JAIL_FROM_VAULT and VAULT_JAIL_FROM_VAULT_REVISIT), the cell
///   behind Vault's jail door. Once the party has beaten the three thieves in
///   the inn's loft (FLAG_VAULT_THIEVES_JAILED, vault_interiors.c), coming in
///   from Vault finds them behind the bars, each with his own talk and Mind
///   Read. Once Vault has become its later scene (vault_revisit.c), whose
///   villagers say the thieves have broken out, its jail door comes in by
///   VAULT_JAIL_FROM_VAULT_REVISIT and finds a lone prisoner instead.
/// - Lunpa (VAULT_JAIL_FROM_LUNPA), the map's other room, which Lunpa's hidden
///   passage reaches (lunpa.c). Its six chests are each opened once
///   (FLAG_JAIL_CHEST_*), TRIGGER_LUNPA_DOOR leads back up into Lunpa, and
///   arriving this way clears FLAG_SHOW_LOCATION_NAME, so no place name is
///   shown. The room uses the thieves' tables, so its chests and exit sit in
///   the same event table as the thieves' talk and the Vault door.
///
/// Open questions:
/// - XXX (no evidence): why arriving from Lunpa hides the place name, which
///   gScene_AreaNameRules gives as "Lunpa" for that entrance; the code shows
///   only that it does.

#include "game_state.h"
#include "unit/map/vault_jail.h"
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
#include "types.h"
#include "ability_ids.h"
#include "script_layer.h"
#include "map_tables.h"
#include "flags.h"
#include "item_ids.h"
#include "songs.h"
#include "text/msg_ids.h"

enum JailTrigger {
  TRIGGER_VAULT_DOOR = 1,
  TRIGGER_LUNPA_DOOR = 2,
  TRIGGER_CHEST_WATER_OF_LIFE = 100,
  TRIGGER_CHEST_SMOKE_BOMB,
  TRIGGER_CHEST_LUCKY_MEDAL,
  TRIGGER_CHEST_ANTIDOTE,
  TRIGGER_CHEST_COINS,
  TRIGGER_CHEST_ELIXIR
};

/// The placed actors, numbering the rows of the table Scene_GetPlacements
/// returns: the three thieves of gVaultJail_ThiefPlacements, or the prisoner of
/// gVaultJail_PrisonerPlacements in the same first slot.
enum JailActor {
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

/// Returns the entrances from Vault, from Lunpa and from Vault's revisit scene.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gVaultJail_Entrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

/// Returns the exits to Vault, to Vault's revisit scene and to Lunpa, each to
/// the entrance that scene names for the jail (VAULT_FROM_VAULT_JAIL,
/// VAULT_REVISIT_FROM_VAULT_JAIL and LUNPA_FROM_VAULT_JAIL).
const u32 *Scene_GetExits(void) { return gVaultJail_Exits; }

/// Places the three jailed thieves (each only once FLAG_VAULT_THIEVES_JAILED
/// is set), or, entered from Vault's revisit scene
/// (VAULT_JAIL_FROM_VAULT_REVISIT, after they have broken out), the lone
/// prisoner.
const struct ScenePlacement *Scene_GetPlacements(void) {
  if (gGameState.entrance == VAULT_JAIL_FROM_VAULT_REVISIT) {
    return gVaultJail_PrisonerPlacements;
  }
  return gVaultJail_ThiefPlacements;
}

/// Returns the thieves' events, which also hold the Lunpa room's chests and
/// exit, or the prisoner's when the party came in from Vault's revisit scene,
/// matching Scene_GetPlacements.
const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.entrance == VAULT_JAIL_FROM_VAULT_REVISIT) {
    return gVaultJail_PrisonerEvents;
  }
  return gVaultJail_ThiefEvents;
}

/// In Vault's jail, the thieves' leader glares at each of his men and snaps
/// "Shut yer yaps!", so that both start, then turns to the party: they got us
/// this time, but next time they'll pay for this.
///
/// Reached by talking to the leader (ACTOR_THIEF_LEADER) while
/// FLAG_VAULT_THIEVES_JAILED is set.
static void Event_LeaderSilencesThieves(void) {
  Engine_EventStart();

  Engine_ActorFaceActor(ACTOR_THIEF_LEADER, ACTOR_LEFT_THIEF, 0);
  Engine_EventWait(40);

  Engine_ActorFaceActor(ACTOR_THIEF_LEADER, ACTOR_RIGHT_THIEF, 0);
  Engine_EventWait(40);

  // "Shut yer yaps!"; the two thieves start.
  Engine_EventSetMessage(MsgVaultThiefLeaderSilencesThieves);
  Engine_EventShowMessage(ACTOR_THIEF_LEADER, 0);

  Engine_ActorStartRepeatedMotion(ACTOR_LEFT_THIEF, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_RIGHT_THIEF, 2);
  Engine_EventWait(20);

  // Turn from the two thieves to the party for the remaining dialogue.
  Engine_ActorFaceEachOther(ACTOR_THIEF_LEADER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);

  Engine_ActorRunRepeatedMotion(ACTOR_THIEF_LEADER, 1);
  Engine_EventWait(20);

  Engine_EventShowMessage(ACTOR_THIEF_LEADER, 0);

  Engine_EventEnd();
}

/// In Vault's jail, the thief on the left wishes they had escaped to the cave
/// northeast of Goma Range.
///
/// Reached by talking to him (ACTOR_LEFT_THIEF) while FLAG_VAULT_THIEVES_JAILED
/// is set.
static void Event_LeftThiefRegretsEscape(void) {
  Engine_EventStart();
  Engine_ActorRunRepeatedMotion(ACTOR_LEFT_THIEF, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgVaultLeftThiefRegretsEscape);
  Engine_EventShowMessage(ACTOR_LEFT_THIEF, 0);
  Engine_EventEnd();
}

/// In Vault's jail, the thief on the right shakes his head: he knew they
/// should have skipped town sooner.
///
/// Reached by talking to him (ACTOR_RIGHT_THIEF) while
/// FLAG_VAULT_THIEVES_JAILED is set.
static void Event_RightThiefRegretsStaying(void) {
  Engine_EventStart();
  Engine_ActorSetAnimationAndWait(ACTOR_RIGHT_THIEF, ANIMATION_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgVaultRightThiefRegretsStaying);
  Engine_EventShowMessage(ACTOR_RIGHT_THIEF, 0);
  Engine_EventEnd();
}

/// Mind Read on the thieves' leader in Vault's jail: he'll be out of this jail
/// before you know it.
///
/// Reached by Mind Read on ACTOR_THIEF_LEADER while FLAG_VAULT_THIEVES_JAILED
/// is set; the two below are reached the same way on ACTOR_LEFT_THIEF and
/// ACTOR_RIGHT_THIEF.
static void Event_ReadLeaderMind(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MsgVaultThiefLeaderThoughts);
  Engine_EventShowMessage(ACTOR_THIEF_LEADER, 0);
  Engine_EventEnd();
}

/// Mind Read on the thief on the left: will he ever run free again?
static void Event_ReadLeftThiefMind(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MsgVaultLeftThiefThoughts);
  Engine_EventShowMessage(ACTOR_LEFT_THIEF, 0);
  Engine_EventEnd();
}

/// Mind Read on the thief on the right: is this because they stole Hammet's
/// Shaman's Rod?
static void Event_ReadRightThiefMind(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MsgVaultRightThiefThoughts);
  Engine_EventShowMessage(ACTOR_RIGHT_THIEF, 0);
  Engine_EventEnd();
}

/// The party steps out of Vault's jail into Vault, with the exit sound.
///
/// Reached by stepping on the door's cell (TRIGGER_VAULT_DOOR) under the
/// thieves' table, that is, entered from Vault or Lunpa; takes
/// VAULT_JAIL_TO_VAULT to VAULT_FROM_VAULT_JAIL. The prisoner's table leaves by
/// a plain exit instead, VAULT_JAIL_TO_VAULT_REVISIT.
static void Event_LeaveForVault(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(VAULT_JAIL_TO_VAULT);
}

/// Opens the scene through a window transition and lets the party reach the
/// jail's occupants across the bars.
///
/// Called by the engine once the actors are placed. Coming in through Lunpa's
/// hidden passage (VAULT_JAIL_FROM_LUNPA) clears FLAG_SHOW_LOCATION_NAME, so no
/// place name is shown; otherwise it adds JAIL_REACH_ACROSS_BARS to the
/// prisoner or to the three thieves.
s32 Scene_Initialize(void) {
  s16 entrance;

  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  entrance = gGameState.entrance;

  if (entrance == VAULT_JAIL_FROM_LUNPA) {
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  } else if (entrance == VAULT_JAIL_FROM_VAULT_REVISIT) {
    Object_GetById(ACTOR_PRISONER)->collisionFlags |= JAIL_REACH_ACROSS_BARS;
  } else {
    Object_GetById(ACTOR_THIEF_LEADER)->collisionFlags |=
        JAIL_REACH_ACROSS_BARS;
    Object_GetById(ACTOR_LEFT_THIEF)->collisionFlags |= JAIL_REACH_ACROSS_BARS;
    Object_GetById(ACTOR_RIGHT_THIEF)->collisionFlags |= JAIL_REACH_ACROSS_BARS;
  }

  return 0;
}

// clang-format off
const struct SceneEntrance gVaultJail_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VAULT_JAIL_FROM_VAULT, CONDITION_ALWAYS, 80, 0, 232, FACING_NORTH, 0, 16, 48, 304, 256, 0 },
    { VAULT_JAIL_FROM_LUNPA, CONDITION_ALWAYS, 544, 0, 280, FACING_NORTH, 0, 440, 48, 680, 312, 0 },
    { VAULT_JAIL_FROM_VAULT_REVISIT, CONDITION_ALWAYS, 80, 0, 232, FACING_NORTH, 0, 16, 48, 304, 256, 0 },
    { SCENE_TABLE_END },
};

const u32 gVaultJail_Exits[] = {
    SCENE_EXITS(VaultJail),
    SCENE_EXIT(VAULT_JAIL_TO_VAULT, Vault, VAULT_FROM_VAULT_JAIL),
    SCENE_EXIT(VAULT_JAIL_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_VAULT_JAIL),
    SCENE_EXIT(VAULT_JAIL_TO_LUNPA, Lunpa, LUNPA_FROM_VAULT_JAIL),
    SCENE_EXITS_END,
};

const struct ScenePlacement gVaultJail_ThiefPlacements[] = {
    { SPRITE_MAN_WITH_LONG_DARK_HAIR, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), ACTOR_STAND,
      PIXELS(168), 0, PIXELS(144), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_PERSON_IN_PURPLE, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), ACTOR_STAND,
      PIXELS(88), 0, PIXELS(144), FACING_SOUTH - FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_PERSON_IN_PURPLE, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), ACTOR_STAND,
      PIXELS(248), 0, PIXELS(144), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gVaultJail_PrisonerPlacements[] = {
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(168), 0, PIXELS(144), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

const struct SceneEvent gVaultJail_ThiefEvents[] = {
    { EVENT_TOUCH, TRIGGER_VAULT_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveForVault) },
    { EVENT_EXIT, TRIGGER_LUNPA_DOOR, CONDITION_ALWAYS, VAULT_JAIL_TO_LUNPA },
    { EVENT_TALK, ACTOR_THIEF_LEADER, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT_SCRIPT(Event_LeaderSilencesThieves) },
    { EVENT_TALK, ACTOR_LEFT_THIEF, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT_SCRIPT(Event_LeftThiefRegretsEscape) },
    { EVENT_TALK, ACTOR_RIGHT_THIEF, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT_SCRIPT(Event_RightThiefRegretsStaying) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIEF_LEADER,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT_SCRIPT(Event_ReadLeaderMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEFT_THIEF,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT_SCRIPT(Event_ReadLeftThiefMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RIGHT_THIEF,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT_SCRIPT(Event_ReadRightThiefMind) },
    { SEARCH(SEARCH_CHEST), TRIGGER_CHEST_WATER_OF_LIFE, FLAG_JAIL_CHEST_WATER_OF_LIFE,
      FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { SEARCH(SEARCH_CHEST), TRIGGER_CHEST_SMOKE_BOMB, FLAG_JAIL_CHEST_SMOKE_BOMB,
      FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_CHEST), TRIGGER_CHEST_LUCKY_MEDAL, FLAG_JAIL_CHEST_LUCKY_MEDAL,
      FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_CHEST), TRIGGER_CHEST_ANTIDOTE, FLAG_JAIL_CHEST_ANTIDOTE,
      FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_CHEST), TRIGGER_CHEST_COINS, FLAG_JAIL_CHEST_COINS, FIND_COINS(44) },
    { SEARCH(SEARCH_CHEST), TRIGGER_CHEST_ELIXIR, FLAG_JAIL_CHEST_ELIXIR,
      FIND_ITEM(ITEM_ELIXIR) },
    { SCENE_EVENTS_END },
};
// clang-format on

const struct SceneEvent gVaultJail_PrisonerEvents[] = {
    {EVENT_EXIT, TRIGGER_VAULT_DOOR, CONDITION_ALWAYS,
     VAULT_JAIL_TO_VAULT_REVISIT},
    {EVENT_TALK, ACTOR_PRISONER, CONDITION_ALWAYS,
     MsgVaultPrisonerRegretsFight},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRISONER, CONDITION_ALWAYS,
     MsgVaultPrisonerThoughts},
    {SCENE_EVENTS_END},
};
