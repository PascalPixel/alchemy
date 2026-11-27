/* @file */
/* Stone rooms (SceneId_StoneRooms), a rooms map drawn in stone. */
/* */
/* The game packs rooms of several places that share a tile style onto one */
/* map. This one holds two, whose area names differ by entrance. */
/* */
/* The entrance also picks the tables. STONE_ROOMS_FROM_VAULT and */
/* STONE_ROOMS_FROM_LUNPA take the thieves' tables, and */
/* STONE_ROOMS_FROM_VAULT_REVISIT takes the prisoner's. */
/* */
/* The two rooms, in story order, by the exits that lead in: */
/* */
/* - Vault (STONE_ROOMS_FROM_VAULT and STONE_ROOMS_FROM_VAULT_REVISIT), the */
/*   cell behind Vault's jail door. Once the party has beaten the three */
/*   thieves in the inn's loft (FLAG_VAULT_THIEVES_JAILED, log_rooms_a.c), */
/*   coming in from Vault finds them behind the bars. Each has his own talk */
/*   and Mind Read. Later Vault becomes its later scene (vault_revisit.c), */
/*   whose villagers say the thieves have broken out. Its jail door then */
/*   comes in by STONE_ROOMS_FROM_VAULT_REVISIT and finds a lone prisoner */
/*   instead. */
/* - Lunpa (STONE_ROOMS_FROM_LUNPA), Lunpa's hidden room, which the town's */
/*   hidden passage reaches (lunpa.c). Its six chests are each opened once */
/*   (FLAG_LUNPA_HIDDEN_ROOM_CHEST_*), and */
/*   CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_DOOR leads back up into Lunpa. Arriving */
/*   this way clears FLAG_SHOW_AREA_NAME, so no area name is shown. The room */
/*   uses the thieves' tables, so its chests and exit sit in the same */
/*   trigger table as the thieves' talk and the Vault door. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

extern const SCENE_ENTRANCE StoneRooms_Entrances[];
extern const u32 StoneRooms_Exits[];
extern const ACTOR_PLACE StoneRooms_ThiefActorPlacements[];
extern const ACTOR_PLACE StoneRooms_PrisonerActorPlacements[];
extern const TRIGGER StoneRooms_ThiefTriggers[];
extern const TRIGGER StoneRooms_PrisonerTriggers[];

/* The cell behaviors the jail's triggers answer. */
typedef enum STONE_ROOMS_CELL_BEHAVIOR {
	CELL_BEHAVIOR_VAULT_JAIL_DOOR = 1,
	CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_DOOR = 2,
	CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_WATER_OF_LIFE = 100,
	CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_SMOKE_BOMB,
	CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_LUCKY_MEDAL,
	CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_ANTIDOTE,
	CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_COINS,
	CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_CHEST_ELIXIR
} STONE_ROOMS_CELL_BEHAVIOR;

/* The placed actors, numbering the rows of the table */
/* scGetActorPlacements returns. */
/* */
/* They are the three thieves of StoneRooms_ThiefActorPlacements, or the */
/* prisoner of StoneRooms_PrisonerActorPlacements in the same first slot. */
typedef enum JAIL_ACTOR_ID {
	/* The thieves' leader (SPRITE_MAN_WITH_LONG_DARK_HAIR), in the middle of
	   the */
	/* cell. */
	ACTOR_THIEF_LEADER = ACTOR_FIRST_PLACED,
	/* The thief on the left (SPRITE_PERSON_IN_PURPLE). */
	ACTOR_LEFT_THIEF,
	/* The thief on the right (SPRITE_PERSON_IN_PURPLE). */
	ACTOR_RIGHT_THIEF,
	/* The lone prisoner after the thieves have broken out (SPRITE_STRONGMAN).
	 */
	ACTOR_PRISONER = ACTOR_FIRST_PLACED
} JAIL_ACTOR_ID;

/* The jail's occupants count as nearer than they stand for talk and */
/* Psynergy targets, so the party reaches them across the bars. */
/* */
/* fdFindNearbyTalkTarget scales their distance by 10/13. */
enum { JAIL_REACH_ACROSS_BARS = ACTOR_NEAR_TALK | ACTOR_NEAR_PSYNERGY };

/* Returns the entrances from Vault, from Lunpa and from Vault's */
/* revisit scene. */
const SCENE_ENTRANCE *scGetEntrances(void) { return StoneRooms_Entrances; }

const SCENE_EXIT_REGION *scGetExitRegions(void) { return NULL; }

/* Returns the exits to Vault, to Vault's revisit scene and to Lunpa. */
const u32 *scGetExits(void) { return StoneRooms_Exits; }

/* Places the three jailed thieves, or the lone prisoner. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	if(MainRoutineStateWork.entrance == STONE_ROOMS_FROM_VAULT_REVISIT) {
		return StoneRooms_PrisonerActorPlacements;
	}
	return StoneRooms_ThiefActorPlacements;
}

/* Returns the thieves' triggers, or the prisoner's. */
const TRIGGER *scGetTriggers(void) {
	if(MainRoutineStateWork.entrance == STONE_ROOMS_FROM_VAULT_REVISIT) {
		return StoneRooms_PrisonerTriggers;
	}
	return StoneRooms_ThiefTriggers;
}

/* In Vault's jail, the thieves' leader silences his men and vows revenge. */
static void scStoneRoomsLeaderSilencesThieves(void) {
	fdEventStart();

	fdActorFaceActor(ACTOR_THIEF_LEADER, ACTOR_LEFT_THIEF, 0);
	fdEventWait(40);

	fdActorFaceActor(ACTOR_THIEF_LEADER, ACTOR_RIGHT_THIEF, 0);
	fdEventWait(40);

	fdEventSetMessageId(MsgVaultThiefLeaderSilencesThieves);
	/* "Shut yer yaps!" */
	fdEventShowMessage(ACTOR_THIEF_LEADER, 0);

	/* The two thieves start. */
	fdActorStartRepeatedMotion(ACTOR_LEFT_THIEF, 2);
	fdActorRunRepeatedMotion(ACTOR_RIGHT_THIEF, 2);
	fdEventWait(20);

	fdActorTurnToFaceEachOther(ACTOR_THIEF_LEADER, ACTOR_PARTY_LEADER, 0);
	fdEventWait(20);

	fdActorRunRepeatedMotion(ACTOR_THIEF_LEADER, 1);
	fdEventWait(20);

	/* "They got us this time, but next time... Oh, they'll pay for..." */
	fdEventShowMessage(ACTOR_THIEF_LEADER, 0);

	fdEventEnd();
}

/* In Vault's jail, the thief on the left wishes they had escaped to the cave */
/* northeast of Goma Range. */
static void scStoneRoomsLeftThiefRegretsEscape(void) {
	fdEventStart();
	fdActorRunRepeatedMotion(ACTOR_LEFT_THIEF, 2);
	fdEventWait(20);
	fdEventSetMessageId(MsgVaultLeftThiefRegretsEscape);
	/* "If only we had escaped to the cave northeast of Goma Range." */
	fdEventShowMessage(ACTOR_LEFT_THIEF, 0);
	fdEventEnd();
}

/* In Vault's jail, the thief on the right shakes his head and says he knew */
/* they should have skipped town sooner. */
static void scStoneRoomsRightThiefRegretsStaying(void) {
	fdEventStart();
	fdActorSetAnimationAndWait(ACTOR_RIGHT_THIEF, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	fdEventSetMessageId(MsgVaultRightThiefRegretsStaying);
	/* "Blast! I knew we should have skipped town sooner." */
	fdEventShowMessage(ACTOR_RIGHT_THIEF, 0);
	fdEventEnd();
}

/* Mind Read on the thieves' leader in Vault's jail shows him thinking he'll */
/* be out of this jail before you know it. */
static void scStoneRoomsReadLeaderMind(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultThiefLeaderThoughts);
	/* "I'll be out of this jail before you know it. You'll see!" */
	fdEventShowMessage(ACTOR_THIEF_LEADER, 0);
	fdEventEnd();
}

/* Mind Read on the thief on the left shows him wondering whether he will */
/* ever run free again. */
static void scStoneRoomsReadLeftThiefMind(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultLeftThiefThoughts);
	/* "Sigh... Will I ever run free again?" */
	fdEventShowMessage(ACTOR_LEFT_THIEF, 0);
	fdEventEnd();
}

/* Mind Read on the thief on the right shows him wondering whether this is */
/* because they stole Hammet's Shaman's Rod. */
static void scStoneRoomsReadRightThiefMind(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultRightThiefThoughts);
	/* "I wonder if this is because we stole Hammet's Shaman's Rod." */
	fdEventShowMessage(ACTOR_RIGHT_THIEF, 0);
	fdEventEnd();
}

/* The party steps out of Vault's jail into Vault, with the exit sound. */
static void scStoneRoomsLeaveForVault(void) {
	sySndPlay(SE_MAP_EXIT);
	syImportEventSetExitRequest(STONE_ROOMS_TO_VAULT);
}

/* Opens the map through a window transition and lets the party reach the */
/* jail's occupants across the bars. */
s32 scInit(void) {
	s16 entrance;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	entrance = MainRoutineStateWork.entrance;

	if(entrance == STONE_ROOMS_FROM_LUNPA) {
		/* The room is in Vault's area, so coming from Lunpa changes the area
		 * and */
		/* scArrive asks for the area name, which Area_NameRules gives as */
		/* "Lunpa" for this entrance: the town the party has just left. Clearing
		 */
		/* the flag keeps that banner from showing. */
		gmFlagClear(FLAG_SHOW_AREA_NAME);
	}
	else if(entrance == STONE_ROOMS_FROM_VAULT_REVISIT) {
		fdActorGet(ACTOR_PRISONER)->collisionFlg |= JAIL_REACH_ACROSS_BARS;
	}
	else {
		fdActorGet(ACTOR_THIEF_LEADER)->collisionFlg |= JAIL_REACH_ACROSS_BARS;
		fdActorGet(ACTOR_LEFT_THIEF)->collisionFlg |= JAIL_REACH_ACROSS_BARS;
		fdActorGet(ACTOR_RIGHT_THIEF)->collisionFlg |= JAIL_REACH_ACROSS_BARS;
	}

	return 0;
}

/* clang-format off */
const SCENE_ENTRANCE StoneRooms_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { STONE_ROOMS_FROM_VAULT, CONDITION_ALWAYS, 80, 0, 232, FACING_NORTH, 0, 16, 48, 304, 256, 0 },
    { STONE_ROOMS_FROM_LUNPA, CONDITION_ALWAYS, 544, 0, 280, FACING_NORTH, 0, 440, 48, 680, 312, 0 },
    { STONE_ROOMS_FROM_VAULT_REVISIT, CONDITION_ALWAYS, 80, 0, 232, FACING_NORTH, 0, 16, 48, 304, 256, 0 },
    { SCENE_TABLE_END },
};

const u32 StoneRooms_Exits[] = {
    SCENE_EXITS(StoneRooms),
    SCENE_EXIT(STONE_ROOMS_TO_VAULT, Vault, VAULT_FROM_STONE_ROOMS),
    SCENE_EXIT(STONE_ROOMS_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_STONE_ROOMS),
    SCENE_EXIT(STONE_ROOMS_TO_LUNPA, Lunpa, LUNPA_FROM_STONE_ROOMS),
    SCENE_EXITS_END,
};

const ACTOR_PLACE StoneRooms_ThiefActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_LONG_DARK_HAIR, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), MOVEMENT_TYPE_STAND,
      168, 0, 144, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_IN_PURPLE, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), MOVEMENT_TYPE_STAND,
      88, 0, 144, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_IN_PURPLE, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), MOVEMENT_TYPE_STAND,
      248, 0, 144, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

const ACTOR_PLACE StoneRooms_PrisonerActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      168, 0, 144, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

const TRIGGER StoneRooms_ThiefTriggers[] = {
    { TRIGGER_STEP, CELL_BEHAVIOR_VAULT_JAIL_DOOR, CONDITION_ALWAYS, EVENT(scStoneRoomsLeaveForVault) },
    { TRIGGER_EXIT, CELL_BEHAVIOR_LUNPA_HIDDEN_ROOM_DOOR, CONDITION_ALWAYS, STONE_ROOMS_TO_LUNPA },
    { TRIGGER_TALK, ACTOR_THIEF_LEADER, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT(scStoneRoomsLeaderSilencesThieves) },
    { TRIGGER_TALK, ACTOR_LEFT_THIEF, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT(scStoneRoomsLeftThiefRegretsEscape) },
    { TRIGGER_TALK, ACTOR_RIGHT_THIEF, IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED),
      EVENT(scStoneRoomsRightThiefRegretsStaying) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIEF_LEADER,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT(scStoneRoomsReadLeaderMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEFT_THIEF,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT(scStoneRoomsReadLeftThiefMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RIGHT_THIEF,
      IF_FLAG_SET(FLAG_VAULT_THIEVES_JAILED), EVENT(scStoneRoomsReadRightThiefMind) },
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
    { TRIGGER_TABLE_END },
};
/* clang-format on */

const TRIGGER StoneRooms_PrisonerTriggers[] = {
	{TRIGGER_EXIT, CELL_BEHAVIOR_VAULT_JAIL_DOOR, CONDITION_ALWAYS,
	 STONE_ROOMS_TO_VAULT_REVISIT},
	{TRIGGER_TALK, ACTOR_PRISONER, CONDITION_ALWAYS,
	 MsgVaultPrisonerRegretsFight},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRISONER, CONDITION_ALWAYS,
	 MsgVaultPrisonerThoughts},
	{TRIGGER_TABLE_END},
};
