#include "TYPES.H"
#include "ABILITY_IDS.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FLAG_IDS.H"
#include "ITEM_IDS.H"
#include "SOUND_IDS.H"

/*
 * The jail cell on the underground way between the Vault and Lunpa. While
 * the Vault thieves are locked up, the three of them sit in the cell; after
 * the party returns from the later Vault, a lone prisoner has taken their
 * place.
 */

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

/* Each exit arrives at the destination scene's entrance from the jail. */
enum {
    ENTRANCE_FROM_JAIL = 12
};

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

enum JailChestFlag {
    FLAG_JAIL_CHEST_WATER_OF_LIFE = 0xf8c,
    FLAG_JAIL_CHEST_SMOKE_BOMB,
    FLAG_JAIL_CHEST_LUCKY_MEDAL,
    FLAG_JAIL_CHEST_ANTIDOTE,
    FLAG_JAIL_CHEST_COINS,
    FLAG_JAIL_CHEST_ELIXIR
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

enum JailMessage {
    MSG_LEFT_THIEF_REGRETS_ESCAPE = 0x1388,
    MSG_RIGHT_THIEF_REGRETS_STAYING = 0x1389,
    MSG_THIEF_LEADER_SILENCES_THIEVES = 0x138a,
    MSG_LEFT_THIEF_THOUGHTS = 0x138c,
    MSG_RIGHT_THIEF_THOUGHTS = 0x138d,
    MSG_THIEF_LEADER_THOUGHTS = 0x138e,
    MSG_PRISONER_REGRETS_FIGHT = 0x1cf6,
    MSG_PRISONER_THOUGHTS = 0x1cf7
};

/* The collision flags set on whoever sits in the cell. */
enum {
    CELL_COLLISION_FLAGS = 0x14
};

extern const struct SceneEntrance gJailEntrances[];
extern const u32 gJailExits[];
extern const struct ScenePlacement gJailThiefPlacements[];
extern const struct ScenePlacement gJailPrisonerPlacements[];
extern const struct SceneEvent gJailThiefEvents[];
extern const struct SceneEvent gJailPrisonerEvents[];

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gJailEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return NULL;
}

const u32 *Scene_GetExits(void)
{
    return gJailExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.entrance == JAIL_ENTRANCE_FROM_VAULT_REVISIT) {
        return gJailPrisonerPlacements;
    }
    return gJailThiefPlacements;
}

const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.entrance == JAIL_ENTRANCE_FROM_VAULT_REVISIT) {
        return gJailPrisonerEvents;
    }
    return gJailThiefEvents;
}

/* The leader looks at each of his men and silences them, then turns to the
   party and speaks again. */
void ThiefLeader_Talk(void)
{
    Event_Begin();

    Actor_FaceActor(ACTOR_THIEF_LEADER, ACTOR_LEFT_THIEF, 0);
    Event_Wait(40);

    Actor_FaceActor(ACTOR_THIEF_LEADER, ACTOR_RIGHT_THIEF, 0);
    Event_Wait(40);

    Event_SetMessage(MSG_THIEF_LEADER_SILENCES_THIEVES);
    Event_ShowMessage(ACTOR_THIEF_LEADER, 0);

    Actor_StartRepeatedMotion(ACTOR_LEFT_THIEF, 2);
    Actor_RunRepeatedMotion(ACTOR_RIGHT_THIEF, 2);
    Event_Wait(20);

    Actor_FaceEachOther(ACTOR_THIEF_LEADER, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);

    Actor_RunRepeatedMotion(ACTOR_THIEF_LEADER, 1);
    Event_Wait(20);

    Event_ShowMessage(ACTOR_THIEF_LEADER, 0);

    Event_End();
}

void LeftThief_Talk(void)
{
    Event_Begin();
    Actor_RunRepeatedMotion(ACTOR_LEFT_THIEF, 2);
    Event_Wait(20);
    Event_SetMessage(MSG_LEFT_THIEF_REGRETS_ESCAPE);
    Event_ShowMessage(ACTOR_LEFT_THIEF, 0);
    Event_End();
}

void RightThief_Talk(void)
{
    Event_Begin();
    Actor_SetAnimationAndWait(ACTOR_RIGHT_THIEF, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_SetMessage(MSG_RIGHT_THIEF_REGRETS_STAYING);
    Event_ShowMessage(ACTOR_RIGHT_THIEF, 0);
    Event_End();
}

void ThiefLeader_ReadMind(void)
{
    Event_Begin();
    Event_SetMessage(MSG_THIEF_LEADER_THOUGHTS);
    Event_ShowMessage(ACTOR_THIEF_LEADER, 0);
    Event_End();
}

void LeftThief_ReadMind(void)
{
    Event_Begin();
    Event_SetMessage(MSG_LEFT_THIEF_THOUGHTS);
    Event_ShowMessage(ACTOR_LEFT_THIEF, 0);
    Event_End();
}

void RightThief_ReadMind(void)
{
    Event_Begin();
    Event_SetMessage(MSG_RIGHT_THIEF_THOUGHTS);
    Event_ShowMessage(ACTOR_RIGHT_THIEF, 0);
    Event_End();
}

void VaultDoor_Leave(void)
{
    Audio_PlayCue(SOUND_MAP_EXIT);
    Event_RequestExit(JAIL_EXIT_TO_VAULT);
}

s32 Scene_Initialize(void)
{
    s16 entrance;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    entrance = gGameState.entrance;

    if (entrance == JAIL_ENTRANCE_FROM_LUNPA) {
        GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
    } else if (entrance == JAIL_ENTRANCE_FROM_VAULT_REVISIT) {
        Actor_Get(ACTOR_PRISONER)->collision_flags |= CELL_COLLISION_FLAGS;
    } else {
        Actor_Get(ACTOR_THIEF_LEADER)->collision_flags |= CELL_COLLISION_FLAGS;
        Actor_Get(ACTOR_LEFT_THIEF)->collision_flags |= CELL_COLLISION_FLAGS;
        Actor_Get(ACTOR_RIGHT_THIEF)->collision_flags |= CELL_COLLISION_FLAGS;
    }

    return 0;
}

const struct SceneEntrance gJailEntrances[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { JAIL_ENTRANCE_FROM_VAULT, CONDITION_ALWAYS, 80, 0, 232, FACING_NORTH, 0, 16, 48, 304, 256, 0 },
    { JAIL_ENTRANCE_FROM_LUNPA, CONDITION_ALWAYS, 544, 0, 280, FACING_NORTH, 0, 440, 48, 680, 312, 0 },
    { JAIL_ENTRANCE_FROM_VAULT_REVISIT, CONDITION_ALWAYS, 80, 0, 232, FACING_NORTH, 0, 16, 48, 304, 256, 0 },
    { SCENE_TABLE_END },
};

const u32 gJailExits[] = {
    SCENE_EXITS(SCENE_KUUPUAPPU_RUNPA),
    SCENE_EXIT(JAIL_EXIT_TO_VAULT, SCENE_KUUPUAPPU_MURA, ENTRANCE_FROM_JAIL),
    SCENE_EXIT(JAIL_EXIT_TO_VAULT_REVISIT, SCENE_KUUPUAPPU_MURA_SAI, ENTRANCE_FROM_JAIL),
    SCENE_EXIT(JAIL_EXIT_TO_LUNPA, SCENE_RUNPA_MURA, ENTRANCE_FROM_JAIL),
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
    { EVENT_TOUCH, TRIGGER_VAULT_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(VaultDoor_Leave) },
    { EVENT_EXIT, TRIGGER_LUNPA_DOOR, CONDITION_ALWAYS, JAIL_EXIT_TO_LUNPA },
    { EVENT_TALK, ACTOR_THIEF_LEADER, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT_SCRIPT(ThiefLeader_Talk) },
    { EVENT_TALK, ACTOR_LEFT_THIEF, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT_SCRIPT(LeftThief_Talk) },
    { EVENT_TALK, ACTOR_RIGHT_THIEF, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT_SCRIPT(RightThief_Talk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIEF_LEADER,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT_SCRIPT(ThiefLeader_ReadMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEFT_THIEF,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT_SCRIPT(LeftThief_ReadMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RIGHT_THIEF,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT_SCRIPT(RightThief_ReadMind) },
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

const struct SceneEvent gJailPrisonerEvents[] = {
    { EVENT_EXIT, TRIGGER_VAULT_DOOR, CONDITION_ALWAYS, JAIL_EXIT_TO_VAULT_REVISIT },
    { EVENT_TALK, ACTOR_PRISONER, CONDITION_ALWAYS, MSG_PRISONER_REGRETS_FIGHT },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRISONER, CONDITION_ALWAYS,
      MSG_PRISONER_THOUGHTS },
    { SCENE_EVENTS_END },
};
