#ifndef ALCHEMY_BATTLE_PRESENTATION_H
#define ALCHEMY_BATTLE_PRESENTATION_H

/* @file */
/* The battle's staging and turn: building the enemies and the battle */
/* background, choosing and carrying out actions, the turn's end, and the */
/* main-image functions they call. */

#include "types.h"
#include "resource_ids.h"
#include "battle/battle_action.h"
#include "battle/battle_side.h"

struct BATTLE_ACTION_RECORD;
struct BATTLE_PLAN;

/* Returns 1 when the unit is among its side's units in the battle. */
u32 btUnitIsListed(u32 id);

/* Builds the battle's enemies from a formation; returns how many there are. */
s32 btFormationBuildEnemyList(s32 recordId);

/* The battle background: its scanline state made, its picture loaded, the */
/* state freed, and its display mode switched. */
void btBackgroundInit(void);

/* Opens the battle: the curtain, then the enemies announced. */
void btTransitionRun(s32 enemyCount, s32 kind);

/* Does nothing, at the end of each turn and of the battle. */
void btTurnEndNoOp(void);

void btBackgroundFree(void);

/* An enemy kind's battle sprite and its palette. */
s32 btSprGetSpr(s32 index);
s32 btSprGetPalette(s32 index);

/* Announces an action and lays it out target by target; 0 stages it. */
s32 btActionRunBuild(BATTLE_ACTION_RECORD *request, struct BATTLE_PLAN *plan);

/* Plays an attack: the run up to the target and the blow. */
s32 btStagingRunAttack(struct BATTLE_PLAN *plan, s32 mode);

/* Counts down one side's Djinn recovery at the turn's end. */
s32 btDjinnRecoveryCountDownSide(s32 side);

/* Chooses an enemy's action and its target. */
s32 btAIChooseAction(BATTLE_ACTION_RECORD *request, s32 retry);

/* The battle effect an enemy kind's plain attack shows, and its defeat sound.
 */
u32 btSprGetAttackEffect(u32 index);
u32 btSprGetDefeatSnd(s32 index);

/* Picks a random living target on the party's side or the enemies'. */
s32 btAIChooseRandomTarget(s32 party);

struct BATTLE_PRESENTATION_STATE;

/* The bit decoder btBackgroundLoad copies to RAM and runs. It returns */
/* the source after the last halfword it read. */
typedef const u8 *(*MODEL_DECODER)(const u8 *source, void *destination);

/* What btMotionCallBeast saves of the battle object a called beast */
/* borrows, for btMotionDismissBeast to put back. */
typedef struct SAVED_BATTLE_OBJECT {
	BATTLE_OBJECT obj; /* the borrowed object, copied whole */
	/* The beast's pictures: a heap block of its own, or ActorSpriteSlots when
	 */
	/* the object borrowed is another side's. */
	u32 *buf;
	u8 unitId; /* the unit whose battle object the beast borrows */
	/* 1 when the caster and its first target are on opposite sides: the beast
	 */
	/* takes the caster's own object. */
	u8 single;
	u8 reactivate; /* 1 when the borrowed object had to be made first */
	u16 classId;   /* the beast's enemy id (ENEMY_WILD_WOLF and so on) */
} SAVED_BATTLE_OBJECT;

/* One kept formation: its record and its average level, -1 while empty. */
typedef struct FORMATION_CANDIDATE {
	s16 recordId;
	s32 score : 16;
} FORMATION_CANDIDATE;

/* An ENEMY_DEFINITION as btAIChooseAction reads its abilities: through a */
/* pointer two bytes past its start. */
typedef struct AI_ENEMY_REST {
	u8 unused[44];	  /* the rest of the definition before its abilities */
	u16 abilities[8]; /* ENEMY_DEFINITION.abilities */
} AI_ENEMY_REST;

/* The bit decoder btBackgroundLoad copies to RAM to decode a battle */
/* background's tiles (model_decode.s). */
extern const u8 Model_DecodeCode[];
extern u8 Model_DecodeCodeSize[]; /* Model_DecodeCode's bytes */

/* The field engine's sprite functions the battle's motion calls: a sprite run
 */
/* on to its animation's stop, one of its animations removed, and an */
/* animation's length in frames. */
void fdSprSkipToStopStub(void *record);
void fdSprRemoveAnimationAtStub(void *record, s32 slot);
s32 fdSprAnimationSumCommandLengthsStub(s32 id, s32 animation, s32 timedCount);

/* Starts a message in the battle's message window. */
void mnBattleMessageOpenStub(s32 message);
/* One step of dissolving a sprite pixel by pixel. */
void fdSprDissolveStepStub(void *record, s32 step);
/* Levels the unit up once if its experience reaches the next level, filling */
/* gains, a LEVEL_UP_RESULT read as halfwords (LEVEL_UP_GAIN); its */
/* address, or 0 when the unit did not level up. */
s32 gmUnitTryLevelUpStub(s32 unitId, s16 *gains);

/* The advance arrow beside a battle message: eight 128-byte pictures of a */
/* 256-colour sprite, one every four frames. */
extern u8 BattleMessage_AdvanceArrowTiles[];

/* btAIChooseAction's rolls for the first three action strategies, a row */
/* each: the chance in 256 of each place in the enemy's actions. */
extern u8 BattleAI_ActionWeights[3][8];
/* The enemy definitions Dullahan acts as, one drawn at random. */
extern u16 BattleAI_DullahanForms[];

/* The battle transition's curtain tile in eight variants, 32 bytes (one 4-bit
 */
/* tile) each, a new one every four frames. */
extern const u8 BattleTransition_CurtainVariants[];
/* The battle transition's curtain tiles, 32 bytes (one 4-bit tile) each. */
extern u8 BattleTransition_CurtainTiles[];

/* The Doom Dragon's opening: the screen shatters over the Serpent's stars. */
void btEffectRunShatteringOpeningStub(struct BATTLE_PRESENTATION_STATE *state);

/* The party, then the enemies: the order the sides take their turn-end */
/* effects in. */
extern const BATTLE_SIDE_ORDER Battle_TurnEndSideOrder;

/* Puts a unit's set Djinn on standby, one at random or all. */
void btUnitDrainDjinn(s32 unitId, s32 all, s32 announce);

#endif
