#ifndef ALCHEMY_BATTLE_PRESENTATION_H
#define ALCHEMY_BATTLE_PRESENTATION_H

/* The battle's staging and turn: building the enemies and the battle
   background, choosing and carrying out actions, the turn's end, and the
   main-image functions they call. */

#include "types.h"
#include "resource_ids.h"
#include "battle/battle_action.h"
#include "battle/battle_side.h"

struct BATTLE_ACTION_RECORD;
struct BATTLE_PLAN;

/* Returns 1 when the unit is among its side's units in the battle. */
Uint32 btUnitIsListed(Uint32 id);

/* Builds the battle's enemies from a formation; returns how many there are. */
Sint32 btFormationBuildEnemyList(Sint32 recordId);

/* The battle background: its scanline state made, its picture loaded, the state
   freed, and its display mode switched. */
void btBackgroundInit(void);

/* Opens the battle: the curtain, then the enemies announced. */
void btTransitionRun(Sint32 enemyCnt, Sint32 kind);

/* Does nothing, at the end of each turn and of the battle. */
void btTurnEndNoOperation(void);

void btBackgroundFree(void);

/* An enemy kind's battle sprite and its palette. */
Sint32 btSprGetSpr(Sint32 idx);
Sint32 btSprGetPalette(Sint32 idx);

/* Announces an action and lays it out target by target; 0 stages it. */
Sint32 btActionRunBuild(
	BATTLE_ACTION_RECORD *request, struct BATTLE_PLAN *plan);

/* Plays an attack: the run up to the target and the blow. */
Sint32 btStagingRunAttack(struct BATTLE_PLAN *plan, Sint32 mode);

/* Counts down one side's Djinn recovery at the turn's end. */
Sint32 btDjinnRecoveryCountDownSide(Sint32 side);

/* Chooses an enemy's action and its target. */
Sint32 btAIChooseAction(BATTLE_ACTION_RECORD *request, Sint32 retry);

/* The battle effect an enemy kind's plain attack shows, and its defeat
   sound. */
Uint32 btSprGetAttackEffect(Uint32 idx);
Uint32 btSprGetDefeatSnd(Sint32 idx);

/* Picks a random living target on the party's side or the enemies'. */
Sint32 btAIChooseRandomTarget(Sint32 party);

struct BATTLE_PRESENTATION_STATE;

/* The bit decoder btBackgroundLoad copies to RAM and runs. It returns the
   source after the last halfword it read. */
typedef const Uint8 *(*MODEL_DECODER)(const Uint8 *source, void *destination);

/* What btMotionCallBeast saves of the battle object a called beast borrows, for
   btMotionDismissBeast to put back. */
typedef struct SAVED_BATTLE_OBJECT {
	BATTLE_OBJECT obj; /* the borrowed object, copied whole */
	/* The beast's pictures: a heap block of its own, or ActorSpriteSlots when
	   the object borrowed is another side's. */
	Uint32 *buf;
	Uint8	unitId; /* the unit whose battle object the beast borrows */
	/* 1 when the caster and its first target are on opposite sides: the beast
	   takes the caster's own object. */
	Uint8  single;
	Uint8  reactivate; /* 1 when the borrowed object had to be made first */
	Uint16 classId;	   /* the beast's enemy id (ENEMY_WILD_WOLF and so on) */
} SAVED_BATTLE_OBJECT;

/* One kept formation: its record and its average level, -1 while empty. */
typedef struct FORMATION_CANDIDATE {
	Sint16 recordId;
	Sint32 score : 16;
} FORMATION_CANDIDATE;

/* An ENEMY_DEFINITION as btAIChooseAction reads its abilities: through a
   pointer two bytes past its start. */
typedef struct AI_ENEMY_REST {
	Uint8  unused[44];	 /* the rest of the definition before its abilities */
	Uint16 abilities[8]; /* ENEMY_DEFINITION.abilities */
} AI_ENEMY_REST;

/* The bit decoder btBackgroundLoad copies to RAM to decode a battle
   background's tiles (model_decode.s). */
extern const Uint8 btModelDecodeCode[];
extern Uint8	   Model_DecodeCodeSize[]; /* btModelDecodeCode's bytes */

/* The field engine's sprite functions the battle's motion calls: a sprite run
   on to its animation's stop, one of its animations removed, and an animation's
   length in ticks. */
void   fdSprSkipToStopStub(void *record);
void   fdSprRemoveAnimationAtStub(void *record, Sint32 slot);
Sint32 fdSprAnimationSumCommandLengthsStub(
	Sint32 id, Sint32 anm, Sint32 timedCnt);

/* Starts a message in the battle's message window. */
void mnBattleMessageOpenStub(Sint32 msg);
/* One step of dissolving a sprite pixel by pixel. */
void fdSprDissolveStepStub(void *record, Sint32 step);
/* Levels the unit up once if its experience reaches the next level, filling
   gains, a LEVEL_UP_RESULT read as halfwords (LEVEL_UP_GAIN); its address, or 0
   when the unit did not level up. */
Sint32 gmUnitTryLevelUpStub(Sint32 unitId, Sint16 *gains);

/* The advance arrow beside a battle message: eight 128-byte pictures of a
   256-colour sprite, one every four ticks. */
extern Uint8 BattleMessage_AdvanceArrowTiles[];

/* btAIChooseAction's rolls for the first three action strategies, a row each:
   the chance in 256 of each place in the enemy's actions. */
extern Uint8 BattleAIActionWeights[3][8];
/* The enemy definitions Dullahan acts as, one drawn at random. */
extern Uint16 BattleAIDullahanForms[];

/* The battle transition's curtain tile in eight variants, 32 bytes (one 4-bit
   tile) each, a new one every four ticks. */
extern const Uint8 BattleTransition_CurtainVariants[];
/* The battle transition's curtain tiles, 32 bytes (one 4-bit tile) each. */
extern Uint8 BattleTransition_CurtainTiles[];

/* The Doom Dragon's opening: the screen shatters over the Serpent's stars. */
void btEffectRunShatteringOpeningStub(struct BATTLE_PRESENTATION_STATE *state);

/* The party, then the enemies: the order the sides take their turn-end effects
   in. */
extern const BATTLE_SIDE_ORDER BattleTurnEndSideOrder;

/* Puts a unit's set Djinn on standby, one at random or all. */
void btUnitDrainDjinn(Sint32 unitId, Sint32 all, Sint32 announce);

#endif
