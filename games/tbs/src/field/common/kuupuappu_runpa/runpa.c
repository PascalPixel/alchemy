#include "game_state.h"
#include "unit/field/common/kuupuappu_runpa/runpa.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
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
#include "flag_ids.h"
#include "item_ids.h"
#include "sound_ids.h"
#include "text/msg_ids.h"

enum JailEntrance
{
	JAIL_ENTRANCE_FROM_VAULT = 1,
	JAIL_ENTRANCE_FROM_LUNPA = 2,
	JAIL_ENTRANCE_FROM_VAULT_REVISIT = 10
};

enum JailExit
{
	JAIL_EXIT_TO_VAULT = 1,
	JAIL_EXIT_TO_VAULT_REVISIT = 2,
	JAIL_EXIT_TO_LUNPA = 3
};

enum
{
	ENTRANCE_FROM_JAIL = 12
};

enum JailTrigger
{
	TRIGGER_VAULT_DOOR = 1,
	TRIGGER_LUNPA_DOOR = 2,
	TRIGGER_CHEST_WATER_OF_LIFE = 100,
	TRIGGER_CHEST_SMOKE_BOMB,
	TRIGGER_CHEST_LUCKY_MEDAL,
	TRIGGER_CHEST_ANTIDOTE,
	TRIGGER_CHEST_COINS,
	TRIGGER_CHEST_ELIXIR
};

enum JailChestFlag
{
	FLAG_JAIL_CHEST_WATER_OF_LIFE = 0xf8c,
	FLAG_JAIL_CHEST_SMOKE_BOMB,
	FLAG_JAIL_CHEST_LUCKY_MEDAL,
	FLAG_JAIL_CHEST_ANTIDOTE,
	FLAG_JAIL_CHEST_COINS,
	FLAG_JAIL_CHEST_ELIXIR
};

enum JailActor
{
	ACTOR_THIEF_LEADER = ACTOR_FIRST_PLACED,
	ACTOR_LEFT_THIEF,
	ACTOR_RIGHT_THIEF,
	ACTOR_PRISONER = ACTOR_FIRST_PLACED
};

enum JailSprite
{
	SPRITE_THIEF_LEADER = 0x3f,
	SPRITE_THIEF = 0x40,
	SPRITE_PRISONER = 0x74
};

enum
{
	CELL_COLLISION_FLAGS = 0x14
};

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
	if (gGameState.entrance == JAIL_ENTRANCE_FROM_VAULT_REVISIT)
	{
		return gJailPrisonerPlacements;
	}
	return gJailThiefPlacements;
}

const struct SceneEvent *Scene_GetEvents(void)
{
	if (gGameState.entrance == JAIL_ENTRANCE_FROM_VAULT_REVISIT)
	{
		return gJailPrisonerEvents;
	}
	return gJailThiefEvents;
}

static void EV08_TALK(void)
{
	Engine_EventBegin();

	Engine_ActorFaceActor(ACTOR_THIEF_LEADER, ACTOR_LEFT_THIEF, 0);
	Engine_EventWait(40);

	Engine_ActorFaceActor(ACTOR_THIEF_LEADER, ACTOR_RIGHT_THIEF, 0);
	Engine_EventWait(40);

	Engine_EventSetMessage((s32)MsgKuupuappuThiefLeaderSilencesThieves);
	Engine_EventShowMessage(ACTOR_THIEF_LEADER, 0);

	Engine_ActorStartRepeatedMotion(ACTOR_LEFT_THIEF, 2);
	Engine_ActorRunRepeatedMotion(ACTOR_RIGHT_THIEF, 2);
	Engine_EventWait(20);

	Engine_ActorFaceEachOther(ACTOR_THIEF_LEADER, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(20);

	Engine_ActorRunRepeatedMotion(ACTOR_THIEF_LEADER, 1);
	Engine_EventWait(20);

	Engine_EventShowMessage(ACTOR_THIEF_LEADER, 0);

	Engine_EventEnd();
}

static void EV09_TALK(void)
{
	Engine_EventBegin();
	Engine_ActorRunRepeatedMotion(ACTOR_LEFT_THIEF, 2);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgKuupuappuLeftThiefRegretsEscape);
	Engine_EventShowMessage(ACTOR_LEFT_THIEF, 0);
	Engine_EventEnd();
}

static void EV10_TALK(void)
{
	Engine_EventBegin();
	Engine_ActorSetAnimationAndWait(ACTOR_RIGHT_THIEF, ANIM_SHAKE_HEAD);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgKuupuappuRightThiefRegretsStaying);
	Engine_EventShowMessage(ACTOR_RIGHT_THIEF, 0);
	Engine_EventEnd();
}

static void EV08_PSYNERGY(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuThiefLeaderThoughts);
	Engine_EventShowMessage(ACTOR_THIEF_LEADER, 0);
	Engine_EventEnd();
}

static void EV09_PSYNERGY(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuLeftThiefThoughts);
	Engine_EventShowMessage(ACTOR_LEFT_THIEF, 0);
	Engine_EventEnd();
}

static void EV10_PSYNERGY(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuRightThiefThoughts);
	Engine_EventShowMessage(ACTOR_RIGHT_THIEF, 0);
	Engine_EventEnd();
}

static void EV01_TOUCH(void)
{
	Engine_AudioPlayCue(SOUND_MAP_EXIT);
	Engine_EventRequestExit(JAIL_EXIT_TO_VAULT);
}

s32 Scene_Initialize(void)
{
	s16 entrance;

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	entrance = gGameState.entrance;

	if (entrance == JAIL_ENTRANCE_FROM_LUNPA)
	{
		Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
	}
	else if (entrance == JAIL_ENTRANCE_FROM_VAULT_REVISIT)
	{
		Object_GetById(ACTOR_PRISONER)->collision_flags |= CELL_COLLISION_FLAGS;
	}
	else
	{
		Object_GetById(ACTOR_THIEF_LEADER)->collision_flags |= CELL_COLLISION_FLAGS;
		Object_GetById(ACTOR_LEFT_THIEF)->collision_flags |= CELL_COLLISION_FLAGS;
		Object_GetById(ACTOR_RIGHT_THIEF)->collision_flags |= CELL_COLLISION_FLAGS;
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
	{ EVENT_TOUCH, TRIGGER_VAULT_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_TOUCH) },
	{ EVENT_EXIT, TRIGGER_LUNPA_DOOR, CONDITION_ALWAYS, JAIL_EXIT_TO_LUNPA },
	{ EVENT_TALK, ACTOR_THIEF_LEADER, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT_SCRIPT(EV08_TALK) },
	{ EVENT_TALK, ACTOR_LEFT_THIEF, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT_SCRIPT(EV09_TALK) },
	{ EVENT_TALK, ACTOR_RIGHT_THIEF, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT_SCRIPT(EV10_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIEF_LEADER,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT_SCRIPT(EV08_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEFT_THIEF,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT_SCRIPT(EV09_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RIGHT_THIEF,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT_SCRIPT(EV10_PSYNERGY) },
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
	{ EVENT_TALK, ACTOR_PRISONER, CONDITION_ALWAYS,
      (s32)MsgKuupuappuPrisonerRegretsFight },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRISONER, CONDITION_ALWAYS,
      (s32)MsgKuupuappuPrisonerThoughts },
	{ SCENE_EVENTS_END },
};
