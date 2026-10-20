/// @file
/// Vault's jail, where the thieves who stole Hammet's Shaman's Rod are held; on
/// the party's return a lone prisoner takes their place.

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

enum JailEntrance {
  JAIL_ENTRANCE_FROM_VAULT = 1,
  JAIL_ENTRANCE_FROM_LUNPA = 2,
  JAIL_ENTRANCE_FROM_VAULT_REVISIT = 10
};

enum JailExit {
  JAIL_EXIT_TO_VAULT = 1,
  JAIL_EXIT_TO_VAULT_REVISIT = 2,
  JAIL_EXIT_TO_LUNPA = 3
};

enum { ENTRANCE_FROM_JAIL = 12 };

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

enum JailActor {
  ACTOR_THIEF_LEADER = ACTOR_FIRST_PLACED,
  ACTOR_LEFT_THIEF,
  ACTOR_RIGHT_THIEF,
  ACTOR_PRISONER = ACTOR_FIRST_PLACED
};

enum JailSprite {
  SPRITE_THIEF_LEADER = 0x3f,
  SPRITE_THIEF = 0x40,
  SPRITE_PRISONER = 0x74
};

enum { CELL_COLLISION_FLAGS = 0x14 };

/// Returns the entrances from Vault, Lunpa and the revisited Vault.
const struct SceneEntrance *Scene_GetEntrances(void) { return gJailEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

/// Returns the exits to Vault, to the revisited Vault and to Lunpa.
const u32 *Scene_GetExits(void) { return gJailExits; }

/// Places the three jailed thieves, or, entered from the revisited Vault
/// (entrance 10, after they have broken out), the lone prisoner.
const struct ScenePlacement *Scene_GetPlacements(void) {
  if (gGameState.entrance == JAIL_ENTRANCE_FROM_VAULT_REVISIT) {
    return gJailPrisonerPlacements;
  }
  return gJailThiefPlacements;
}

/// Returns the thieves' events, or the prisoner's when the party came in from
/// the revisited Vault, matching Scene_GetPlacements.
const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.entrance == JAIL_ENTRANCE_FROM_VAULT_REVISIT) {
    return gJailPrisonerEvents;
  }
  return gJailThiefEvents;
}

/// The jailed leader glares at each of his men and snaps at them, then turns to
/// the party: they got us this time, but you'll pay for this. "Shut yer yaps!"
///
/// Reached by talking to the thieves' leader (actor 8) while
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

/// The left thief wishes they had escaped to the cave northeast of the Goma
/// Range.
///
/// Reached by talking to the left thief (actor 9) while
/// FLAG_VAULT_THIEVES_JAILED is set.
static void Event_LeftThiefRegretsEscape(void) {
  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(ACTOR_LEFT_THIEF, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgVaultLeftThiefRegretsEscape);
  Engine_EventShowMessage(ACTOR_LEFT_THIEF, 0);
  Engine_EventEnd();
}

/// The right thief shakes his head: they should have skipped town sooner.
///
/// Reached by talking to the right thief (actor 10) while
/// FLAG_VAULT_THIEVES_JAILED is set.
static void Event_RightThiefRegretsStaying(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimationAndWait(ACTOR_RIGHT_THIEF, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgVaultRightThiefRegretsStaying);
  Engine_EventShowMessage(ACTOR_RIGHT_THIEF, 0);
  Engine_EventEnd();
}

/// Mind Read on the leader: he'll be out of this jail before you know it.
///
/// Reached by Mind Read on actor 8 while FLAG_VAULT_THIEVES_JAILED is set; the
/// two below are reached the same way on actors 9 and 10.
static void Event_ReadLeaderMind(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultThiefLeaderThoughts);
  Engine_EventShowMessage(ACTOR_THIEF_LEADER, 0);
  Engine_EventEnd();
}

/// Mind Read on the left thief: will he ever run free again?
static void Event_ReadLeftThiefMind(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultLeftThiefThoughts);
  Engine_EventShowMessage(ACTOR_LEFT_THIEF, 0);
  Engine_EventEnd();
}

/// Mind Read on the right thief: is this for stealing Hammet's Shaman's Rod?
static void Event_ReadRightThiefMind(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultRightThiefThoughts);
  Engine_EventShowMessage(ACTOR_RIGHT_THIEF, 0);
  Engine_EventEnd();
}

/// The party steps out of the jail into Vault.
///
/// Reached by stepping on the Vault door (cell code 1) while the thieves are
/// held; takes exit 1 to Vault's entrance 12. The prisoner's events leave by
/// exit 2 instead, to the revisited Vault.
static void Event_LeaveForVault(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(JAIL_EXIT_TO_VAULT);
}

/// Opens the scene through a window transition and makes the occupants solid.
///
/// Called by the engine once the actors are placed. Coming up from Lunpa
/// (entrance 2) clears FLAG_SHOW_LOCATION_NAME, so no place name is shown;
/// otherwise it adds collision flags 0x14 to the prisoner or to the three
/// thieves.
s32 Scene_Initialize(void) {
  s16 entrance;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  entrance = gGameState.entrance;

  if (entrance == JAIL_ENTRANCE_FROM_LUNPA) {
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  } else if (entrance == JAIL_ENTRANCE_FROM_VAULT_REVISIT) {
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
    { JAIL_ENTRANCE_FROM_VAULT, CONDITION_ALWAYS, 80, 0, 232, FACING_NORTH, 0, 16, 48, 304, 256, 0 },
    { JAIL_ENTRANCE_FROM_LUNPA, CONDITION_ALWAYS, 544, 0, 280, FACING_NORTH, 0, 440, 48, 680, 312, 0 },
    { JAIL_ENTRANCE_FROM_VAULT_REVISIT, CONDITION_ALWAYS, 80, 0, 232, FACING_NORTH, 0, 16, 48, 304, 256, 0 },
    { SCENE_TABLE_END },
};

const u32 gJailExits[] = {
    SCENE_EXITS(SCENE_VAULT_JAIL),
    SCENE_EXIT(JAIL_EXIT_TO_VAULT, SCENE_VAULT, ENTRANCE_FROM_JAIL),
    SCENE_EXIT(JAIL_EXIT_TO_VAULT_REVISIT, SCENE_VAULT_REVISIT, ENTRANCE_FROM_JAIL),
    SCENE_EXIT(JAIL_EXIT_TO_LUNPA, SCENE_LUNPA, ENTRANCE_FROM_JAIL),
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
    { EVENT_EXIT, TRIGGER_LUNPA_DOOR, CONDITION_ALWAYS, JAIL_EXIT_TO_LUNPA },
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
     JAIL_EXIT_TO_VAULT_REVISIT},
    {EVENT_TALK, ACTOR_PRISONER, CONDITION_ALWAYS,
     MsgVaultPrisonerRegretsFight},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRISONER, CONDITION_ALWAYS,
     MsgVaultPrisonerThoughts},
    {SCENE_EVENTS_END},
};
