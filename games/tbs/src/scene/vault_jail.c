/// @file
/// Vault's jail (VaultJail, scene 22), one map of two rooms that the area
/// names tell apart by entrance. The entrance also picks the tables: the
/// thieves' for VAULT_JAIL_FROM_VAULT and VAULT_JAIL_FROM_LUNPA, the
/// prisoner's for VAULT_JAIL_FROM_VAULT_REVISIT.
///
/// - Vault (VAULT_JAIL_FROM_VAULT and VAULT_JAIL_FROM_VAULT_REVISIT), the cell
///   behind Vault's jail door. In the inn's loft the party beats the three
///   thieves who stole Hammet's Shaman's Rod and the town's treasures during
///   the eruption, and the mayor has them locked up "for a long, long time"
///   (FLAG_VAULT_THIEVES_JAILED, set in vault_interiors.c). From then on,
///   coming in from Vault (VAULT_JAIL_FROM_VAULT), the party finds the three
///   behind the bars: their leader, who glares at his two men, tells them to
///   "Shut yer yaps!" and promises the party will pay, one who wishes they had
///   escaped to the cave northeast of Goma Range, and one who knew they should
///   have skipped town sooner. Mind Read hears each one's thoughts. Once the
///   party has reached Kalay, Vault becomes its revisit scene
///   (vault_revisit.c), where the villagers say the thieves have broken out.
///   Its jail door brings the party in by VAULT_JAIL_FROM_VAULT_REVISIT, which
///   places a lone prisoner instead, jailed for hitting somebody, with his own
///   talk and Mind Read and a plain exit back to that Vault.
/// - Lunpa (VAULT_JAIL_FROM_LUNPA), the map's other room, which Lunpa's hidden
///   passage reaches (lunpa.c). Its six chests hold a Water of Life, a Smoke
///   Bomb, a Lucky Medal, an Antidote, 44 coins and an Elixir, each opened
///   once (FLAG_JAIL_CHEST_*), and TRIGGER_LUNPA_DOOR leads back up into Lunpa
///   (VAULT_JAIL_TO_LUNPA). Arriving this way clears FLAG_SHOW_LOCATION_NAME,
///   so no place name is shown. The room uses the thieves' tables, so its
///   chests and its exit sit in the same event table as the thieves' talk and
///   the Vault door.

#include "game_state.h"
#include "unit/scene/vault_jail.h"
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
#include "field_event.h"
#include "field_scene.h"
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
/// returns: the three thieves of gJailThiefPlacements, or the prisoner of
/// gJailPrisonerPlacements in the same first slot.
enum JailActor {
  /// The thieves' leader (SPRITE_THIEF_LEADER), in the middle of the cell.
  /// "Shut yer yaps!"
  ACTOR_THIEF_LEADER = ACTOR_FIRST_PLACED,
  /// The thief on the left (SPRITE_THIEF). "If only we had escaped to the cave
  /// northeast of Goma Range."
  ACTOR_LEFT_THIEF,
  /// The thief on the right (SPRITE_THIEF). "Blast! I knew we should have
  /// skipped town sooner."
  ACTOR_RIGHT_THIEF,
  /// The lone prisoner after the thieves have broken out. "I hit somebody, so
  /// they put me in here."
  ACTOR_PRISONER = ACTOR_FIRST_PLACED
};

enum JailSprite {
  SPRITE_THIEF_LEADER = 0x3f,
  SPRITE_THIEF = 0x40,
  SPRITE_PRISONER = 0x74
};

enum { CELL_COLLISION_FLAGS = 0x14 };

/// Returns the entrances from Vault, from Lunpa and from Vault's revisit scene.
const struct SceneEntrance *Scene_GetEntrances(void) { return gJailEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

/// Returns the exits to Vault, to Vault's revisit scene and to Lunpa, each to
/// the entrance that scene names for the jail (VAULT_FROM_VAULT_JAIL,
/// VAULT_REVISIT_FROM_VAULT_JAIL and LUNPA_FROM_VAULT_JAIL).
const u32 *Scene_GetExits(void) { return gJailExits; }

/// Places the three jailed thieves (each only once FLAG_VAULT_THIEVES_JAILED
/// is set), or, entered from Vault's revisit scene
/// (VAULT_JAIL_FROM_VAULT_REVISIT, after they have broken out), the lone
/// prisoner.
const struct ScenePlacement *Scene_GetPlacements(void) {
  if (gGameState.entrance == VAULT_JAIL_FROM_VAULT_REVISIT) {
    return gJailPrisonerPlacements;
  }
  return gJailThiefPlacements;
}

/// Returns the thieves' events, which also hold the Lunpa room's chests and
/// exit, or the prisoner's when the party came in from Vault's revisit scene,
/// matching Scene_GetPlacements.
const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.entrance == VAULT_JAIL_FROM_VAULT_REVISIT) {
    return gJailPrisonerEvents;
  }
  return gJailThiefEvents;
}

/// In Vault's jail, the thieves' leader glares at each of his men and snaps
/// "Shut yer yaps!", so that both start, then turns to the party: they got us
/// this time, but next time they'll pay for this.
///
/// Reached by talking to the leader (ACTOR_THIEF_LEADER) while
/// FLAG_VAULT_THIEVES_JAILED is set.
static void Event_LeaderSilencesThieves(void) {
  Engine_EventBegin();

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
  Engine_EventBegin();
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
  Engine_EventBegin();
  Engine_ActorSetAnimationAndWait(ACTOR_RIGHT_THIEF, ANIM_SHAKE_HEAD);
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
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultThiefLeaderThoughts);
  Engine_EventShowMessage(ACTOR_THIEF_LEADER, 0);
  Engine_EventEnd();
}

/// Mind Read on the thief on the left: will he ever run free again?
static void Event_ReadLeftThiefMind(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultLeftThiefThoughts);
  Engine_EventShowMessage(ACTOR_LEFT_THIEF, 0);
  Engine_EventEnd();
}

/// Mind Read on the thief on the right: is this because they stole Hammet's
/// Shaman's Rod?
static void Event_ReadRightThiefMind(void) {
  Engine_EventBegin();
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

/// Opens the scene through a window transition and makes the jail's occupants
/// solid.
///
/// Called by the engine once the actors are placed. Coming in through Lunpa's
/// hidden passage (VAULT_JAIL_FROM_LUNPA) clears FLAG_SHOW_LOCATION_NAME, so no
/// place name is shown; otherwise it adds CELL_COLLISION_FLAGS to the prisoner
/// or to the three thieves.
s32 Scene_Initialize(void) {
  s16 entrance;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  entrance = gGameState.entrance;

  if (entrance == VAULT_JAIL_FROM_LUNPA) {
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  } else if (entrance == VAULT_JAIL_FROM_VAULT_REVISIT) {
    Object_GetById(ACTOR_PRISONER)->collisionFlags |= CELL_COLLISION_FLAGS;
  } else {
    Object_GetById(ACTOR_THIEF_LEADER)->collisionFlags |= CELL_COLLISION_FLAGS;
    Object_GetById(ACTOR_LEFT_THIEF)->collisionFlags |= CELL_COLLISION_FLAGS;
    Object_GetById(ACTOR_RIGHT_THIEF)->collisionFlags |= CELL_COLLISION_FLAGS;
  }

  return 0;
}

// clang-format off
const struct SceneEntrance gJailEntrances[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VAULT_JAIL_FROM_VAULT, CONDITION_ALWAYS, 80, 0, 232, FACING_NORTH, 0, 16, 48, 304, 256, 0 },
    { VAULT_JAIL_FROM_LUNPA, CONDITION_ALWAYS, 544, 0, 280, FACING_NORTH, 0, 440, 48, 680, 312, 0 },
    { VAULT_JAIL_FROM_VAULT_REVISIT, CONDITION_ALWAYS, 80, 0, 232, FACING_NORTH, 0, 16, 48, 304, 256, 0 },
    { SCENE_TABLE_END },
};

const u32 gJailExits[] = {
    SCENE_EXITS(VaultJail),
    SCENE_EXIT(VAULT_JAIL_TO_VAULT, Vault, VAULT_FROM_VAULT_JAIL),
    SCENE_EXIT(VAULT_JAIL_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_VAULT_JAIL),
    SCENE_EXIT(VAULT_JAIL_TO_LUNPA, Lunpa, LUNPA_FROM_VAULT_JAIL),
    SCENE_EXITS_END,
};

const struct ScenePlacement gJailThiefPlacements[] = {
    { SPRITE_THIEF_LEADER, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), ACTOR_STAND,
      PIXELS(168), 0, PIXELS(144), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_THIEF, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), ACTOR_STAND,
      PIXELS(88), 0, PIXELS(144), FACING_SOUTH - FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_THIEF, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), ACTOR_STAND,
      PIXELS(248), 0, PIXELS(144), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gJailPrisonerPlacements[] = {
    { SPRITE_PRISONER, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(168), 0, PIXELS(144), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

const struct SceneEvent gJailThiefEvents[] = {
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

const struct SceneEvent gJailPrisonerEvents[] = {
    {EVENT_EXIT, TRIGGER_VAULT_DOOR, CONDITION_ALWAYS,
     VAULT_JAIL_TO_VAULT_REVISIT},
    {EVENT_TALK, ACTOR_PRISONER, CONDITION_ALWAYS,
     MsgVaultPrisonerRegretsFight},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRISONER, CONDITION_ALWAYS,
     MsgVaultPrisonerThoughts},
    {SCENE_EVENTS_END},
};
