#ifndef ALCHEMY_BATTLE_H
#define ALCHEMY_BATTLE_H

/* The battle module's functions and globals, as the other modules see them. */

#include "fixed_math.h"
#include "types.h"
#include "edition.h"
#include "system.h"
#include "field/speaker.h"
#include "stubs.h"

struct BATTLE_ACTION;
struct BATTLE_EFFECT_PARM;
struct ACTOR;

/* Battlefield (x, y) pixel pairs by place: 0 to 3 the party's column at x 80,
   32 pixels apart, and 5 to 10 a column at x -80, 20 apart
   (battle_tables.c). */
extern const Sint8 BattlePlacement_Spots[];
/* Each weapon's kind: bits 15..9 the kind, which indexes the
   BattleUnit_WeaponAnimations tables, and 8..0 the item id; ended by 0xffff
   (btUnitGetWeaponKind). */
extern const Uint16 BattleUnit_WeaponKinds[];

struct SPR_ANIMATION;
struct SPR;
struct BATTLE_SIDE_ORDER;
struct BATTLE_BACKGROUND_STATE;
struct SPR_SCALE;
struct MIND_READ_STATE;
struct BATTLE_OBJECT;

/* The battle module's functions in the order of its stub table
   (src/battle/stubs.s). */
void   btDebugBattleRunMenu(void);
Sint32 btTransitionRunMosaicFadeOut(void);
void   btObjectSpawnObjectsForList(Sint16 *unitIds, Sint32 refreshPoses);
void   btBackgroundFadeAlphaOut(void);
void   btBackgroundFadeAlphaIn(void);
void   btCameraSetView(Sint32 x, Sint32 depth, Sint32 y, Sint32 zoom);
void   btCameraSetViewAtPairMidpoint(
	  Sint32 firstUnitId, Sint32 secondUnitId, Sint32 zoom);
void   btBackgroundSetMode(Sint32 mode, Sint32 unusedResource, Sint32 fade);
void   btBackgroundLoad(Sint32 mode, Sint32 resourceId, Sint32 level);
void   btBackgroundSetPaletteLevel(Sint32 unused, Sint32 level);
Sint32 btSprGetEffectHeight(Sint32 index);
Sint32 btSprHasHeavyImpact(Sint32 index);
Uint32 btSprGetAttackHop(Uint32 index);
Sint32 btSprIsSmall(Sint32 index);
Sint32 btUnitGetEffectHeight(Sint32 unitId);
void   btMotionApproachTarget(
	  Sint32 unitId, Sint32 targetId, Sint32 frames, Sint32 initialVelocityY);
void				  btMotionLeapBack(Sint32 unitId);
void				  btMotionApplyKnockback(Sint32 unitId, Sint32 variant);
Sint32				  btUnitListIds(Sint32 sideMask, Uint16 *unitIds);
struct BATTLE_OBJECT *btObjectGet(Sint32 unitId);
Sint32				  btRun(Sint32 enemyGroup);
Sint32				  btMotionProjectPos(Sint32 unitId, Sint32 *projected);
Sint32 btMotionProjectScaledPos(Sint32 unitId, Sint32 *projected);
Sint32 btMotionProjectCursorPoint(Sint32 unitId, Sint32 *projected);
Sint32 btUnitListLiving(Sint32 sideMask, Uint16 *unitIds);
Sint32 btPartyPrepareMembers(Uint16 *unitIds);
void   btCameraSetCommandView(Sint32 verticalOffset);
void  *btActorGetSpr(struct ACTOR *actor, Sint32 spriteIndex);
void   btHighlightSetModes(Uint16 *selectedUnits, Sint32 mode);
void   btPlaybackStart(Sint32 snd);
Sint32 btGraphicsScaleRGB555Clamped(
	Uint16 *source, Uint16 *destination, FIXED scale, Sint32 count);
Sint32		  btDjinnRecoveryCountDown(void);
Sint32		  btObjectGetPalette(Sint32 id);
Sint32		  btDjinnRecoveryCountDownTwenty(void);
Sint32		  btDebugParmSetLoad(Sint32 set);
void		  btObjectPut(void);
void		  btObjectDestroySprs(Sint32 unitId);
UNREAD_RESULT btMessageWaitForAdvanceAt(Sint32 x, Sint32 y);
Sint32		  btDjinnCountStandby(Uint32 unitId, Uint8 *counts);

/* One of the star field's three rings, moving outward on the effect canvas. */
typedef struct RING {
	Sint32 x;	 /* 1/1024 canvas pixels */
	Sint32 y;	 /* 1/1024 canvas pixels */
	Sint32 velX; /* 1/1024 canvas pixels per frame */
	Sint32 velY; /* 1/1024 canvas pixels per frame */
	Sint32 age;	 /* frames */
} RING;

/* The weights an enemy picks among its eight actions by, one table for each
   weighted action strategy (ENEMY_ACTION_STRATEGY, battle_internal.h):
   even, falling by 6, or falling steeply from 113 to 5. */
extern const Uint8 BattleAI_ActionWeights[3][8];
/* The knockback's speed gained each frame by variant (BATTLE_KNOCKBACK), 16.16
   pixels a frame: 1, but 8 for KNOCKBACK_HURL. */
extern FIXED BattleKnockback_Acceleration[];
/* The knockback's reach by variant, in percent of the unit's distance from the
   centre: 140 to 200, 100 for KNOCKBACK_BOUNCE and KNOCKBACK_JOLT. */
extern Sint32 BattleKnockback_DistancePercent[];
/* The knockback's top speed by variant, 16.16 pixels a frame: 4, but 20 for
   KNOCKBACK_HURL. */
extern FIXED BattleKnockback_SpeedLimit[];
/* The knockback's hop by variant, 16.16 pixels a frame upward: 2 to 10. */
extern FIXED BattleKnockback_VelocityY[];
/* A fixed formation's places by order, as offsets from the middle place:
   0, -1, 1, -2, 2 and so on out to 4. */
extern const Sint8 BattleFormation_CenteredOffsets[];
/* Turn-end effects go to the party (BATTLE_SIDE_MASK_PARTY), then the enemies
   (BATTLE_SIDE_MASK_ENEMIES). */
extern const struct BATTLE_SIDE_ORDER Battle_TurnEndSideOrder;
/* A lone SC_END: a battle unit's actor script, which btObjectMake starts. */
extern const Sint32 BattleObject_EmptyScript[];
/* The advance arrow beside a battle message: eight 128-byte pictures of a
   256-colour sprite, one every four frames. */
extern Uint8 BattleMessage_AdvanceArrowTiles[];
/* The battle transition's curtain tiles, 32 bytes (one 4-bit tile) each. */
extern const Uint8 BattleTransition_CurtainTiles[];
/* Eight curtain tiles, 32 bytes each, of which the transition copies one at a
   time into the curtain's first tile. */
extern const Uint8 BattleTransition_CurtainVariants[];
/* Garet's weapon sprite by weapon kind (btUnitGetWeaponSpr). */
extern const Uint16 BattleUnit_WeaponAnimationsGaret[];
/* Ivan's weapon sprite by weapon kind. */
extern const Uint16 BattleUnit_WeaponAnimationsIvan[];
/* Mia's weapon sprite by weapon kind. */
extern const Uint16 BattleUnit_WeaponAnimationsMia[];
/* Jenna's weapon sprite by weapon kind; Felix draws none. */
extern const Uint16 BattleUnit_WeaponAnimationsJenna[];
/* The size of btBitDecoderDecodeImage, which btBackgroundLoad copies to
   HEAP_SLOT_RAM_CODE_TEMP; ld_script.ld defines it as an address equal to the
   size. */
extern Uint8 BitDecoder_Size[];
/* The extra matrix the battle camera's projection applies first while commands
   are chosen (FLAG_CHOOSING_COMMANDS), in 16.16 words. */
extern const FIXED BattleCamera_CommandMatrix[];
/* Where SPARK_STATE.sheet holds the spark's square picture for each frame of
   life left, 0 to 6: byte offsets, a byte a pixel. */
extern Sint32 ElementGlow_SparkPictures[];
/* The spark's width and height in pixels by frames of life left: 2 to 4, 4 from
   three; a spark with no life left is not drawn. */
extern Uint8 ElementGlow_SparkSizes[];
/* Where SPARK_STATE.sheet holds the ring's four 32 by 32 frames: byte offsets,
   1,024 apart. */
extern Sint32 ElementGlow_RingPictures[];
/* The highlighted units' BLDY lightening: 15 down to 0 once, then the second
   sixteen (a pulse up to 8 and back) over and over. */
extern Sint8 BattleHighlight_BlendRamp[];
/* The 104 encounter zones (encounter_zones.c), read as ENCOUNTER_ZONE rows. */
extern Uint16 EncounterZones[];
/* The HEAP_SLOT_MIND_READ heap slot: Mind Read's wave state while its effect
   runs. */
#define MindReadStateWork \
	(*(struct MIND_READ_STATE **)&HeapSlots[HEAP_SLOT_MIND_READ])
/* The heap slots from HEAP_SLOT_BATTLE_EFFECT, then the canvas and sprite
   sheet: the running battle effect's BATTLE_EFFECT_STATE first. */
#define BattleEffectStateWork \
	(*(void *(*)[]) & HeapSlots[HEAP_SLOT_BATTLE_EFFECT])
/* The HEAP_SLOT_ENEMY_UNITS heap slot: the six enemy battle units,
   BATTLE_UNIT_SIZE bytes each, that gmUnitGet returns for unit ids 128 to 133
   during a battle. */
#define EnemyUnits (*(Uint8 **)&HeapSlots[HEAP_SLOT_ENEMY_UNITS])
/* The HEAP_SLOT_BATTLE_BACKGROUND heap slot: the camera's turn, which
   battle_staging.c, battle_auto_battle.c and battle_turn.c set for each action,
   and the background's display mode. */
#define BattleBackgroundStateWork \
	(*(struct BATTLE_BACKGROUND_STATE * \
			*)&HeapSlots[HEAP_SLOT_BATTLE_BACKGROUND])
/* Isaac's weapon sprite by weapon kind (btUnitGetWeaponSpr). */
extern const Uint16 BattleUnit_WeaponAnimationsIsaac[];
/* The ARM decoder for a battle background's image, which btBackgroundLoad
   copies to RAM and runs as a BIT_DECODER. */
void btBitDecoderDecodeImage(void);

/* btMotionApplyKnockback's knockbacks, the columns of the BattleKnockback_*
   tables in knockback_tables.c. Each is named by how far and how high it sends
   the unit, as percent of its spot's distance from the centre and its hop in
   pixels a frame; all but KNOCKBACK_HURL accelerate by 1 pixel a frame to at
   most 4. */
typedef enum BATTLE_KNOCKBACK {
	KNOCKBACK_NUDGE,  /* 140 percent, a hop of 3 */
	KNOCKBACK_HOP,	  /* 140 percent, a hop of 6 */
	KNOCKBACK_LEAP,	  /* 150 percent, a hop of 7 */
	KNOCKBACK_LAUNCH, /* 200 percent, a hop of 10 */
	/* The hardest: 200 percent at 8 pixels a frame more each frame, to at most
	   20, with a hop of 5. */
	KNOCKBACK_HURL,
	/* 150 percent, a hop of 2. From here on a unit already in the air takes the
	   new hop too. */
	KNOCKBACK_SKID,
	KNOCKBACK_BOUNCE, /* the spot itself, a hop of 3 */
	KNOCKBACK_JOLT	  /* the spot itself, a hop of 5.5 */
} BATTLE_KNOCKBACK;

/* The battle sprites' animations (fdSprStartAnimation numbers) that the battle
   code picks by name. */
typedef enum BATTLE_POSE {
	/* The first standing picture held still, without POSE_STANDING's breathing:
	   how an enemy asleep, stunned or held stands. */
	POSE_STILL = 0,
	POSE_STANDING = 1,
	POSE_LEAP = 2, /* running up to a target and leaping */
	POSE_CAST = 3, /* a hand raised to cast or use an item */
	/* asleep, stunned or held; also a downed party member */
	POSE_PARTY_HELD = 4,
	POSE_DAMAGE = 5,	/* taking damage */
	POSE_ENEMY_DOWN = 5 /* a downed enemy stays in its damage pose */
} BATTLE_POSE;

/* A battle unit's sprites' OBJ mode, as btHighlightSetSprMode sets it: faded
   units are semi-transparent, and the battle's blends act on them. */
typedef enum BATTLE_SPR_MODE {
	SPRITE_OPAQUE,
	SPRITE_SEMI_TRANSPARENT
} BATTLE_SPR_MODE;

/* btHighlightSetModes' mode, kept in BATTLE_STATE.highlightMode:
   whether the selected units are highlighted, every other unit faded. */
typedef enum BATTLE_HIGHLIGHT {
	HIGHLIGHT_OFF,
	HIGHLIGHT_SELECTED
} BATTLE_HIGHLIGHT;

/* The glows btElementGlowPlay plays about a unit in its element. */
typedef enum BATTLE_ELEMENT_GLOW {
	/* The star field gathering on the unit while the background darkens: a
	   cast. */
	GLOW_STAR_FIELD,
	GLOW_STREAKS, /* the element's streaks: an enemy's cast */
	GLOW_PILLAR,  /* the element's pillar, at a strength: a summon's boost */
	GLOW_BURST	  /* the element's burst: a Djinni set */
} BATTLE_ELEMENT_GLOW;

struct BATTLE_ACTION;
struct BATTLE_PLAYBACK_STATE;
struct BATTLE_EFFECT_PARM;
struct DJINN_RECOVERY_TABLE;
struct BATTLE_TRANSITION_ENTRY;
struct ABILITY_DEFINITION;
struct BATTLE_OBJECT;
struct BATTLE_ACTION_RUN;
struct POS;
struct ACTOR;

void						 btDebugBattleViewMessages(void);
void						 btCameraUpdate(void);
void						 btIntroAnnounceEncounter(Sint32 enemyCount);
void						 btUnitCollect(void);
Sint32						 btShiftVRAMTiles(void);
void						 btClearBg0Vofs(void);
void						 btPartyBattleSetupNoOperation(Sint32 unused);
UNREAD_RESULT				 btPartyClearBattleStatuses(void);
UNREAD_RESULT				 btDjinnRecoveryReconcile(void);
struct DJINN_RECOVERY_TABLE *btSummonUpdateAvailability(void);
void						 btSummonReservedNoOperation(void);
Sint32						 btLinkBattleReceiveParty(void);
UNREAD_RESULT				 btLinkBattleSendParty(void);
Uint8						*btTextCopyFourCharsUnderscored(
						  Uint32 unusedA, Uint32 unusedB, Uint16 *source);
Sint32		  btLinkBattleWaitSync(void);
Sint32		  btLinkBattleSyncTurn(void);
UNREAD_RESULT btPartyAssignOpponentSlots(void);
Sint32		  btClearSerialTransferPayload(void);
/* btRun's results besides the number of enemies defeated. */
#define BATTLE_OUTCOME_LOST -1
#define BATTLE_OUTCOME_FLED 999
void   btCameraAdjustByShoulderKeys(void);
void   btCameraAdjustByShoulderKeysAlt(void);
Sint32 btUnitCountDownDefenseStage(Sint32 unitId);
Sint32 btUnitCountDownResistanceStage(Sint32 unitId);
Sint32 btUnitCountDownDelusion(Sint32 unitId);
Sint32 btUnitCountDownCharm(Sint32 unitId);
Sint32 btUnitCountDownStun(Sint32 unitId);
Sint32 btUnitCountDownSleep(Sint32 unitId);

/* Pushes a message to show, waiting for the player to read it. */
#define PUSH_MESSAGE(message) btPlaybackPush(PLAYBACK_MESSAGE, (message))

/* Defines a function that turns the battle camera a 128th of a turn each frame
   L or R is held, and centres the background's zoom unless a view holds it. Two
   such functions exist, one each in battle_auto_battle.c and battle_turn.c. */
#define DEFINE_ADJUST_CAMERA(name) \
	void name(void) \
	{ \
		struct BATTLE_CAMERA		   *cam = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA); \
		struct BATTLE_BACKGROUND_STATE *view = \
			HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND); \
\
		if((PerData & KEY_L) != 0) { \
			cam->yaw += 512; \
		} \
		if((PerData & KEY_R) != 0) { \
			cam->yaw -= 512; \
		} \
		if(view->keepRange == 0) { \
			btCameraSetRange(toFIXED(120), toFIXED(120), 0, 0, toFIXED(1)); \
		} \
	}

Sint32		  btEnemyList(Sint16 *unitIds);
void		  btObjectPutKeepingPoses(void);
Sint32		  btEnemyHasSprRoom(Sint32 enemyId);
Sint32		  btEnemySprLoadIntoSlots(Sint32 unitId);
UNREAD_RESULT btObjectReleaseSprSlots(Sint32 unitId);
Sint32		  btUnitGetWeaponKind(Sint32 itemId);
Sint32		  btUnitGetWeaponSpr(Sint32 unitId);
void		  btObjectMake(
			 struct BATTLE_OBJECT *battleObject, Sint32 unitId, Sint32 x, Sint32 y);
void btPlacementGetStepPair(Sint32 partyPlace, Sint32 *x, Sint32 *y);
void btPlacementLayoutEnemies(
	Uint16 *unitIds, Sint32 count, Sint32 *xPositions, Sint32 *zPositions);
Sint32		  btEnemyGetFreeUnitId(void);
UNREAD_RESULT btPlacementRelayoutEnemies(void);
void		  btUnitRefreshPlacement(void);
Bool		  btPlacementContainsId(Sint16 *list, Sint32 place);
void		  btObjectUpdateSprPriorities(void);
UNREAD_RESULT btUnitBuildStatFlags(
	Sint32 unitId, struct BATTLE_OBJECT *battleObject);
UNREAD_RESULT btStatIconCycle(struct BATTLE_OBJECT *battleObject);
UNREAD_RESULT btObjectUpdatePose(Sint32 unitId);
void		  btSprDetachScripts(void *recordGroup);
void		  btActorDetachSprScripts(struct ACTOR *actor);
Sint32		  btObjectMarkForRelease(Sint32 unitId);
#if FIX_FUNCTION_SIGNATURES
void btObjectDestroyAll(void);
#else
void *btObjectDestroyAll(void);
#endif
Sint32		  btObjectGetAnchor(Sint32 place, POS *position);
UNREAD_RESULT btCameraApplyTransform(void);
void		  btCameraSetDefaultView(void);
void		  btMotionReturnToSpot(Sint32 unitId);
void		  btMotionSetupEscapeObject(Sint32 unitId);
void		  btMotionLeapToward(struct BATTLE_OBJECT *startSlot,
			 struct BATTLE_OBJECT *endSlot, Sint32 progress);
void		  btMotionReturnObjectToSpot(struct BATTLE_OBJECT *battleObject);
void		  btMotionHopBack(Sint32 unitId);
void		  btMotionRestartLeap(Sint32 unitId);
void		  btMotionMidpointNoOperation();
void		  btMotionApplyPairMidpoint(Sint32 firstId, Sint32 secondId);
void		  btMotionPlayHitFlash(Sint32 unitId);

Sint32 btAutoBattleBuildActions(struct BATTLE_ACTION *entries);
Sint32 btAutoBattleRunAction(struct BATTLE_ACTION *action);
Sint32 btObjectIsValidId(Uint32 unitId);
Bool   btFleePlayRun(struct BATTLE_ACTION *action);
Sint32 btAutoBattleShowDefend(const struct BATTLE_ACTION *action);
Sint32 btAutoBattleRunPsynergy(struct BATTLE_ACTION *action);
Sint32 btAutoBattleRunApproachAttack(struct BATTLE_ACTION *input);
Sint32 btAutoBattleRunAttackOrItem(struct BATTLE_ACTION *action);
void   btAutoBattleApplyDamage(
	  Uint32 unitId, Sint32 damage, Sint32 showMessage, Uint8 *context);
void   btMotionPlayFall(Sint32 unitId);
Sint32 btAbilityReplaceDefeated(const struct BATTLE_ACTION *action);

void   btPaletteUpdatePulseBrightness(void);
void   btUnitResetGuardLevels(void);
Sint32 btFleeCheckSuccess(void);
Sint32 btBuildPartyActions(struct BATTLE_ACTION *entries);
Sint32 btBuildEnemyActions(struct BATTLE_ACTION *entries);
void   btActionOrderSort(struct BATTLE_ACTION *entries, Sint32 count);
Sint32 btAppendLinkedActions(struct BATTLE_ACTION *actions, Sint32 count);
void   btPaletteCopyWithBrightnessOffset(Sint32 offset);
Sint32 btGatherActions(struct BATTLE_ACTION *actions);
Sint32 btGetUnitAtPlace(Sint32 taggedPlace);
Sint32 btFindUnitPlace(Uint32 unitId);
void   btTurnStartNoOperation(void);

Sint32 btStagingDispatchAction(struct BATTLE_ACTION *request, Sint32 delay);
UNREAD_RESULT btStagingBuildTargetList(
	struct BATTLE_ACTION_RUN *plan, struct BATTLE_EFFECT_PARM *args);
Sint32 btStagingRunFlee(struct BATTLE_ACTION_RUN *plan);
Sint32 btStagingRunFullScreen(struct BATTLE_ACTION_RUN *plan, Sint32 mode);
UNREAD_RESULT btStagingRunMessages(
	struct BATTLE_ACTION_RUN *plan, Sint32 unused);
Sint32 btStagingRunAttack(struct BATTLE_ACTION_RUN *plan, Sint32 mode);
Sint32 btStagingRunPlainAttack(struct BATTLE_ACTION_RUN *input, Sint32 flags);
Sint32 btStagingRunItem(struct BATTLE_ACTION_RUN *plan, Sint32 unused,
	struct BATTLE_ACTION *selection);
UNREAD_RESULT btObjectSetColMode(struct ACTOR *actor, Sint32 palette);
Sint32 btStagingRunPsynergyCast(struct BATTLE_ACTION_RUN *plan, Sint32 mode);

void   btMotionFlashHit(Sint32 unitId, Sint32 unusedA, Sint32 unusedB);
void   btObjectRemoveFromLists(Sint32 unitId);
void   btMotionShowDowned(Sint32 unitId);
Sint32 btAIChooseRandomTarget(Sint32 requireLivingUnit);
Sint32 btAIChooseTarget(Sint32 userId, struct ABILITY_DEFINITION *action);
UNREAD_RESULT btUnitClearStatuses(Sint32 unitId);
UNREAD_RESULT btMessageWaitForAdvance(void);
void		  btPlaybackMarkMessage(void);
UNREAD_RESULT btUnitRemoveEnemy(Sint32 unitId);
void		  btPlaybackGrantDjinnBonus(
			 struct BATTLE_PLAYBACK_STATE *runtime, Uint32 unused);
#if FIX_FUNCTION_SIGNATURES
void btPlaybackRunAll(void);
#else
Uint32 btPlaybackRunAll(void);
#endif
Uint32		  btPlaybackPush(Uint32 opcode, Uint32 operand);
Bool		  btAbilityEffectAppliesToDowned(Sint32 effect);
UNREAD_RESULT btAbilityApplyToTarget(
	struct BATTLE_ACTION_RUN *plan, Sint32 slot);
Uint32		  btAbilityIsPsynergy(Sint32 abilityId);
UNREAD_RESULT btAIChooseAction(struct BATTLE_ACTION *request, Sint32 acting);
void		  btDMAStopDMA3(void);
Sint32		  btPlaybackSchedulePhase(Sint32 frames);
void		  btPlaybackRun(void);
void		  btPlaybackReset(void);
UNREAD_RESULT btPlaybackWaitForReady(void);
Uint32		  btUnitIsListed(Uint32 unitId);
Sint32		  btActionRunBuild(
		   struct BATTLE_ACTION *request, struct BATTLE_ACTION_RUN *plan);

Bool   btUnitRollCounterEarlyEnd(Sint32 unitId, Sint32 turns, Sint32 bias);
Bool   btUnitCountDownAttackStage(Sint32 unitId);
Sint32 btUnitCountDownConfusion(Sint32 unitId);
Bool   btUnitCountDownPsynergySeal(Sint32 unitId);
Bool   btUnitCountDownRegenerate(Sint32 unitId);
Bool   btUnitCountDownReflect(Sint32 unitId);
Bool   btUnitCountDownAgilityStage(Sint32 unitId);
void   btTurnEndNoOperation(void);
UNREAD_RESULT btProcessTurnEnd(void);
UNREAD_RESULT btUnitProcessActionEnd(struct BATTLE_ACTION_RUN *plan);

void btTransitionBuildAffineTilemap(void *tilemap);
void btTransitionBuildFlatTilemap(void *tilemap);
void btBackgroundShowZoomedPage(void);
void btTransitionUploadTileVariant(void);
void btTransitionAdvanceOpening(void);
void btTransitionDrawRows(void);
void btTransitionClearBg0Vofs(void);
void btTransitionRun(Sint32 enemyCount, Sint32 encounterId);
void btBackgroundInit(void);
void btBackgroundFree(void);
void btCameraSetRange(
	FIXED centerX, FIXED centerY, Sint32 offsetX, Sint32 offsetY, FIXED scale);
#if FIX_UNINITIALIZED_READ
void btCameraSetViewAtDepth(Sint32 x, Sint32 depth, Sint32 y, Sint32 zoom);
#else
void btCameraSetViewAtDepth(Sint32 x, Sint32 depth, Sint32 y);
#endif
/* BG1's map during a battle, a screen block: the battle background's, where the
   command row also places its icons' tiles. */
#define BATTLE_BG1_SCREEN_BLOCK	   31
/* BG1's vertical scroll that sets the battle background in the battle view. */
#define BATTLE_BACKGROUND_SCROLL_Y 32
/* btCameraSetView's zoom, in percent, for the battle's usual view. */
#define BATTLE_ZOOM_NORMAL		   100
void   btBackgroundSetBlendCtrl(void);
void   btTransitionInitEntries(struct BATTLE_TRANSITION_ENTRY *entries);
Sint32 btBackgroundDivideBy16(Sint32 value);
Sint32 btAdjustDebugValue(Sint32 selectedValue);
void   btHighlightSetSprMode(Sint32 unitId, Sint32 mode);
void   btHighlightSetOtherSprModes(Sint32 selectedUnitId);
void   btHighlightClearAllSprModes(void);
void   btBackgroundStepHighlightBlend(void);

UNREAD_RESULT btElementGlowUpdateStarField(void);
#if FIX_FUNCTION_SIGNATURES
void btElementGlowUploadStarField(void);
#else
Uint32 btElementGlowUploadStarField(void);
#endif
Sint32 btElementGlowInitStarField(Sint32 element);
Sint32 btElementGlowEnd(Sint32 unused);
/* UB: without UBFIX, btElementGlowPlay's callers see no prototype and take it
   to return int; it is defined returning nothing. */
#if FIX_FUNCTION_SIGNATURES
void btElementGlowPlay(
	Sint32 unitId, Sint32 element, Sint32 mode, Sint32 variant);
#endif
void btBackgroundRestorePalette(void);
void btIRQRemoveVCountHandler(void);

Sint32 btFormationAverageLevel(Sint32 formationId);
Sint32 btFormationSelectLevelMatchedCandidate(Sint32 *outMargin);
#if FIX_FUNCTION_SIGNATURES
void btEnemyAdjustLevel(Sint32 unitId, Sint32 levels);
#else
Sint32 btEnemyAdjustLevel(Sint32 unitId, Sint32 levels);
#endif
Sint32		  btEnemyClaimNameLetter(Sint32 enemyId, Sint32 unused);
UNREAD_RESULT btEnemyReleaseNameLetter(Sint32 unitId);
UNREAD_RESULT btEnemyLetterFirstOfKind(Sint32 enemyId);
Sint32		  btFormationGetRandomMember(Sint32 formationId);
Sint32		  btFormationBuildEnemyList(Sint32 formationId);

Uint32 btSprGetDefeatSnd(Sint32 index);
Sint32 btSprGetSpr(Sint32 index);
Sint32 btSprGetPalette(Sint32 index);
Uint32 btSprGetAttackEffect(Uint32 index);

Uint32 btSpoilsRankItem(Uint32 inventoryEntry);
void   btSpoilsClear(void);
Sint32 btEnemyRecordDefeat(Sint32 unitId, Sint32 djinnBonus);
void   btAwardSpoils(void);
void   btDebugSelectNoOperation(void);

#endif
