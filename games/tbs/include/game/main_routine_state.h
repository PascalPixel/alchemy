#ifndef ALCHEMY_MAIN_ROUTINE_STATE_H
#define ALCHEMY_MAIN_ROUTINE_STATE_H

/* @file */
/* The main routine's state: the transition data the main routine reads */
/* between sequences and the state every sequence shares, which a save file */
/* keeps. */

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "battle/battle_unit.h"

/* A scene's type (SCENE.sceneType, copied to MAIN_ROUTINE_STATE.sceneType). */
typedef enum SCENE_TYPE {
	SCENE_TYPE_OTHER = 1, /* every other scene: dungeons, the world map */
	/* Towns and their rooms, where walking restores no PP, recovers no */
	/* Djinn and does no poison damage, Retreat is blocked, Avoid does not wear
	 */
	/* off and FLAG_IN_TOWN is set. */
	SCENE_TYPE_TOWN = 2
} SCENE_TYPE;

/* MainRoutineStateWork.hazardMode: what advances the hazard count */
/* (fdUpdateWalking). */
typedef enum HAZARD_MODE {
	/* The distance the leader walks on CELL_BEHAVIOR_DESERT_SAND ground. */
	HAZARD_COUNT_ON_CELLS = 0,
	/* One each frame the field sequence runs; no code in the game sets it. */
	HAZARD_COUNT_FRAMES = 1
} HAZARD_MODE;

/* MainRoutineStateWork.hazardDamage's bit 8: its low byte is a percentage of */
/* maximum HP rather than HP. */
#define HAZARD_DAMAGE_PERCENT 0x100

/* MainRoutineStateWork.battleType: how the next battle starts (btSetType); */
/* it is set back to BATTLE_TYPE_NORMAL as the battle ends. */
typedef enum BATTLE_TYPE {
	BATTLE_TYPE_NORMAL = 0,
	/* A battle the party cannot run from: a Mimic's */
	/* (field/field_sequence_triggers.c) and the travelers' fight in Vale */
	/* (scValePlazaFightTravelers, scene/vale_plaza.c). */
	BATTLE_TYPE_NO_ESCAPE = 2,
	/* The battles an event starts. */
	BATTLE_TYPE_SCRIPTED = 3,
	BATTLE_TYPE_LINK = 4, /* a link battle (btRun, scene/battle_arena.c) */
	/* The debug battle menu's mark (btDebugBattleRunMenu), which no code tests
	 */
	/* by value; it still lets the party run. */
	BATTLE_TYPE_DEBUG = 5
} BATTLE_TYPE;

/* The most coins the party carries, and the most shop credit. */
#define COIN_MAX 999999

/* The most Game Tickets counted in PARTY_HOLDINGS.shopGifts, the last of the */
/* Game Ticket's credit costs that is used. */
#define SHOP_GIFTS_MAX 28

/* What the party holds apart from its items: its summons, coins and Djinn, */
/* and what Tolbi's shops and games keep for it. gmDjinnGetRecoveryTable */
/* returns it as the party's DJINN_RECOVERY_TABLE, and Lucky Wheels */
/* writes the prizes it wins through it. */
typedef struct PARTY_HOLDINGS {
	/* The summons the party can call, bit n for summon n: the */
	/* DJINN_RECOVERY_TABLE's summonMask. */
	u32 summonMask;
	u32 coins; /* 0 to COIN_MAX (gmPartyAdjustCoins) */
	/* The party's Djinn recovery list (DJINN_RECOVERY_LIST), read through
	 */
	/* gmDjinnGetRecoveryTable. */
	u8 djinnRecovery[260];
	s32 shopCredit; /* spent on a Game Ticket */
	s8 shopGifts;	/* indexes the Game Ticket's credit cost */
	/* The spring game's prize weights, three prizes to each of 4, 8, 16, 32 */
	/* and 64 on a new game. */
	s8 prizeWeights[15];
	/* Lucky Wheels: 0 to 7 the prize categories won, ended by -1 (-2 once */
	/* awarded); 8 on the last variant given of each category. */
	s8 wonPrizes[16];
} PARTY_HOLDINGS;

/* The main routine's state: the transition data the main routine reads between
 */
/* sequences (the scene and entrance it runs next, sequenceResult, the return */
/* point, the after-battle and defeat destinations and the restart point), and
 */
/* what every sequence shares (the party, coins, Djinn, play time and options).
 */
typedef struct MAIN_ROUTINE_STATE {
	/* gmBuildStampGetTime's value when the game was last saved; its low */
	/* half goes into the file's summary. */
	u32 buildStamp;
	u32 playTime; /* frames, copied from PlayTimeFrames when saving */
	u8 unused[4];
	PARTY_HOLDINGS holdings;
	u8 unusedAfterPrizes[132];
	/* What the main routine runs next: a scene (a SceneId_ row of ScenesWork),
	 */
	/* which means the field sequence running that scene, or a SEQUENCE_ value
	 */
	/* above the table's rows for another sequence. */
	s16 next;
	/* The destination's entrance; in SEQUENCE_BATTLE, the formation to fight.
	 */
	s16 entrance;
	/* The restart point, usually the last Sanctum: read after a defeat with no
	 */
	/* defeat destination, and by the File Screen. */
	s16 restartScene;
	s16 restartEntrance; /* the restart point's entrance */
	/* The return point: the scene that was running when a battle or minigame */
	/* started, where scReturnFromSequence returns unless an after-battle */
	/* destination is set. */
	s16 returnScene;
	s16 returnEntrance; /* the return point's entrance */
	s16 area; /* the current scene's area; a new one resets its flags */
	/* The after-battle destination, -1 for none: read by */
	/* scReturnFromSequence when the party was not defeated, before the */
	/* return point (a minigame's return reads it too), and cleared as each */
	/* scene starts. */
	s16 afterBattleScene;
	s16 afterBattleEntrance; /* its entrance; -1 for none */
	/* The defeat destination, -1 for none: read by scReturnFromSequence */
	/* after a defeat, before the restart point. */
	s16 defeatScene;
	s16 defeatEntrance; /* its entrance; -1 for none */
	s16 battleBg;		/* the battle background chosen for the scene or */
						/* battle environment */
	u8 unusedAfterBg[2];
	/* The scene's map (SCENE.mapIdx); the World Map's is MAP_INDEX_WORLD. */
	s16 mapId;
	FIXED x;		   /* 16.16 fixed-point pixels */
	FIXED y;		   /* 16.16 fixed-point pixels */
	FIXED z;		   /* 16.16 fixed-point pixels */
	u32 heading;	   /* 1/65536 turns */
	u16 layer;		   /* the leader's layer, kept with its position */
	s16 battleMusic;   /* the battle music chosen for the encounter */
	s16 sceneMusic;	   /* the current scene's field music */
	u8 movementMode;   /* 1 while the leader is climbing, 0 otherwise */
	s32 leaderActorId; /* the leader: its unit and its actor */
	u8 party[8];	   /* the party in order */
	u8 unusedAfterParty[5];
	u8 winHue;		  /* the "Window color" setting, 0 to 23 */
	u8 winBrightness; /* the "Window brightness" setting, 0 to 14 */
	u8 unusedAfterBrightness[3];
	/* The voice sound option: 1, as on a new game, plays each speaker's voice
	 */
	/* sound as their message types (fdSpeakerGetVoice). */
	u8 speech;
	u8 unusedAfterSpeech; /* 1 on a new game; nothing reads it */
	u8 msgSpeed;		  /* 0 to 2; 1 on a new game */
	u8 unusedAfterMsgSpeed[2];
	/* Set on saving at the end of the game, marking the file cleared: its data
	 */
	/* can be sent. Copied to the file's summary as sendFlg. */
	u8 sendFlg;
	/* The buttons for each field command, as KEY_ masks; read with KeysPressed
	 */
	/* except dashKeys, held. */
	/* @{ */
	u16 aKeys; /* examine; KEY_A */
	u16 bKeys;
	u16 selectKeys; /* the menu; KEY_SELECT */
	u16 startKeys;
	u16 firstShortcutKeys;
	u16 secondShortcutKeys;
	u16 dashKeys;
	/* @} */
	s16 returnMusic; /* music playing when the field left; -1 for none */
	/* The shortcuts' assignments. bits: 15..14 kind (0 Psynergy, 1 an item) |
	 */
	/* 13..0 which one. */
	/* @{ */
	u16 firstShortcut;
	u16 secondShortcut;
	/* @} */
	u8 perspectiveMap; /* set to 1 on the world map */
	u8 unusedAfterPerspective[5];
	u8 idleSleepEnabled; /* the option, saved from IdleSleepEnabled */
	u8 battleType;		 /* an BATTLE_TYPE */
	/* Hazardous ground (the Lamakan Desert's heat): when hazardSteps reaches */
	/* hazardLimit the party takes hazardDamage and the count starts again. */
	/* @{ */
	s16 hazardLimit; /* pixels walked on hazard cells, or frames, by hazardMode
					  */
	s16 hazardMode;	 /* an HAZARD_MODE */
	/* bits: 8 the amount is a percentage of maximum HP | 7..0 the amount */
	s16 hazardDamage;
	s16 hazardSteps; /* reset on entering a new area */
	/* @} */
	/* The Djinni battle pending (DJINNI_BATTLE_EVENT_FIRST plus its number, */
	/* flags in bits 15 to 11), resolved on returning to the field. */
	s16 pendingDjinni;
	s16 sequenceResult; /* the last battle's result; -1 for a defeat */
	s32 encounterSteps; /* the encounter count, reset with a new area */
	u16 pendingItem; /* given on returning to the field if the party has room */
	s16 sceneType; /* the current scene's variant from its row in ScenesWork */
	/* Where Retreat goes: the scene and entrance the party came into the area
	 */
	/* by. */
	/* @{ */
	s16 retreatScene;
	s16 retreatEntrance;
	/* @} */
	FIXED avoidDistance; /* distance left of Avoid, 16.16 pixels; 0 when off */
	s8 avoidByItem;		 /* Avoid came from an item (the Sacred Feather) */
	/* Halt: the stopped actor's sprite colour mode, which Halt flickers and */
	/* puts back when the actor resumes. */
	s8 haltedColMode;
	s16 haltedActor; /* Halt: the stopped actor; -1 for none */
	s16 cloaked;	 /* 1 under Cloak, which prevents battles */
	/* Halt: the stopped actor's per-frame update, put back when it resumes. */
	u32 haltedUpdate;
	u8 unusedAfterHalt[80];
	/* The Battle Arena's tallies: 4 the total wins and 7 the consecutive wins
	 * it */
	/* reports, 3 cleared on choosing a monster battle; the others not named by
	 */
	/* the game. */
	u16 linkTallies[8];
	u8 unusedAfterTallies[4];
	u32 startStamp; /* gmBuildStampGetTime's value at a new game */
	u8 unusedAfterStartStamp[4];
} MAIN_ROUTINE_STATE;

STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, buildStamp) == 0);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, playTime) == 4);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, idleSleepEnabled) == 554);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, holdings.coins) == 16);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, holdings.shopCredit) == 280);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, holdings.shopGifts) == 284);
STATIC_ASSERT(sizeof(MAIN_ROUTINE_STATE) == 704);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, holdings.wonPrizes) == 300);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, area) == 460);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, dashKeys) == 540);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, avoidDistance) == 580);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, entrance) == 450);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, battleBg) == 470);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, x) == 476);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, movementMode) == 498);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, leaderActorId) == 500);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, party) == 504);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, speech) == 522);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, msgSpeed) == 524);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, aKeys) == 528);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, firstShortcutKeys) == 536);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, secondShortcutKeys) == 538);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, firstShortcut) == 544);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, retreatEntrance) == 578);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, cloaked) == 588);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, linkTallies) == 676);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, startStamp) == 696);

/* The game's state (system/ewram.s). */
extern MAIN_ROUTINE_STATE MainRoutineStateWork;
/* The eight characters' units, which follow it (gmUnitGet). */
extern UNIT CharacterUnits[8];

/* Says what kind of battle comes next (a BATTLE_TYPE_), as an event or a */
/* battle trigger does before starting one. */
#define btSetType(mode)                                                        \
	do {                                                                       \
		MainRoutineStateWork.battleType = (mode);                              \
	} while(0)

#endif
