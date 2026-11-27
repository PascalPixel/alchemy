#ifndef ALCHEMY_BATTLE_H
#define ALCHEMY_BATTLE_H

/* @file */
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
 */
/* 32 pixels apart, and 5 to 10 a column at x -80, 20 apart (battle_tables.c).
 */
extern const s8 BattlePlacement_Spots[];
/* Each weapon's kind: bits 15..9 the kind, which indexes the */
/* BattleUnit_WeaponAnimations tables, and 8..0 the item id; ended by 0xffff */
/* (btUnitGetWeaponKind). */
extern const u16 BattleUnit_WeaponKinds[];

struct SPR_ANIMATION;
struct SPR;
struct BATTLE_SIDE_ORDER;
struct BATTLE_BACKGROUND_STATE;
struct SPR_SCALE;
struct MIND_READ_STATE;
struct BATTLE_OBJECT;

/* The battle module's functions in the order of its stub table */
/* (src/battle/stubs.s). */
void btDebugBattleRunMenu(void);
s32 btTransitionRunMosaicFadeOut(void);
void btObjectSpawnObjectsForList(s16 *unitIds, s32 refreshPoses);
void btBackgroundFadeAlphaOut(void);
void btBackgroundFadeAlphaIn(void);
void btCameraSetView(s32 x, s32 depth, s32 y, s32 zoom);
void btCameraSetViewAtPairMidpoint(s32 firstUnitId, s32 secondUnitId, s32 zoom);
void btBackgroundSetMode(s32 mode, s32 unusedResource, s32 fade);
void btBackgroundLoad(s32 mode, s32 resourceId, s32 level);
void btBackgroundSetPaletteLevel(s32 unused, s32 level);
s32 btSprGetEffectHeight(s32 index);
s32 btSprHasHeavyImpact(s32 index);
u32 btSprGetAttackHop(u32 index);
s32 btSprIsSmall(s32 index);
s32 btUnitGetEffectHeight(s32 unitId);
void btMotionApproachTarget(s32 unitId, s32 targetId, s32 frames,
							s32 initialVelocityY);
void btMotionLeapBack(s32 unitId);
void btMotionApplyKnockback(s32 unitId, s32 variant);
s32 btUnitListIds(s32 sideMask, u16 *unitIds);
struct BATTLE_OBJECT *btObjectGet(s32 unitId);
s32 btRun(s32 enemyGroup);
s32 btMotionProjectPos(s32 unitId, s32 *projected);
s32 btMotionProjectScaledPos(s32 unitId, s32 *projected);
s32 btMotionProjectCursorPoint(s32 unitId, s32 *projected);
s32 btUnitListLiving(s32 sideMask, u16 *unitIds);
s32 btPartyPrepareMembers(u16 *unitIds);
void btCameraSetCommandView(s32 verticalOffset);
void *btActorGetSpr(struct ACTOR *actor, s32 spriteIndex);
void btHighlightSetModes(u16 *selectedUnits, s32 mode);
void btPlaybackStart(s32 cue);
s32 btGraphicsScaleRgb555Clamped(u16 *source, u16 *destination, FIXED scale,
								 s32 count);
s32 btDjinnRecoveryCountDown(void);
s32 btObjectGetPalette(s32 id);
s32 btDjinnRecoveryCountDownTwenty(void);
s32 btDebugParmSetLoad(s32 set);
void btObjectPut(void);
void btObjectDestroySprs(s32 unitId);
UNREAD_RESULT btMessageWaitForAdvanceAt(s32 x, s32 y);
s32 btDjinnCountStandby(u32 unitId, u8 *counts);

/* One of the star field's three rings, moving outward on the effect canvas. */
typedef struct RING {
	s32 x;	  /* 1/1024 canvas pixels */
	s32 y;	  /* 1/1024 canvas pixels */
	s32 velX; /* 1/1024 canvas pixels per frame */
	s32 velY; /* 1/1024 canvas pixels per frame */
	s32 age;  /* frames */
} RING;
/* The weights an enemy picks among its eight actions by, one table for each */
/* weighted action strategy (ENEMY_ACTION_STRATEGY, battle_internal.h): */
/* even, falling by 6, or falling steeply from 113 to 5. */
extern const u8 BattleAI_ActionWeights[3][8];
/* The knockback's speed gained each frame by variant (BATTLE_KNOCKBACK), */
/* 16.16 pixels a frame: 1, but 8 for KNOCKBACK_HURL. */
extern FIXED BattleKnockback_Acceleration[];
/* The knockback's reach by variant, in percent of the unit's distance from */
/* the centre: 140 to 200, 100 for KNOCKBACK_BOUNCE and KNOCKBACK_JOLT. */
extern s32 BattleKnockback_DistancePercent[];
/* The knockback's top speed by variant, 16.16 pixels a frame: 4, but 20 for */
/* KNOCKBACK_HURL. */
extern FIXED BattleKnockback_SpeedLimit[];
/* The knockback's hop by variant, 16.16 pixels a frame upward: 2 to 10. */
extern FIXED BattleKnockback_VelocityY[];
/* A fixed formation's places by order, as offsets from the middle place: */
/* 0, -1, 1, -2, 2 and so on out to 4. */
extern const s8 BattleFormation_CenteredOffsets[];
/* Turn-end effects go to the party (BATTLE_SIDE_MASK_PARTY), then the enemies
 */
/* (BATTLE_SIDE_MASK_ENEMIES). */
extern const struct BATTLE_SIDE_ORDER Battle_TurnEndSideOrder;
/* A lone SC_END: a battle unit's actor script, which btObjectMake */
/* starts. */
extern const s32 BattleObject_EmptyScript[];
/* The advance arrow beside a battle message: eight 128-byte pictures of a */
/* 256-colour sprite, one every four frames. */
extern u8 BattleMessage_AdvanceArrowTiles[];
/* The battle transition's curtain tiles, 32 bytes (one 4-bit tile) each. */
extern const u8 BattleTransition_CurtainTiles[];
/* Eight curtain tiles, 32 bytes each, of which the transition copies one at a
 */
/* time into the curtain's first tile. */
extern const u8 BattleTransition_CurtainVariants[];
/* Garet's weapon sprite by weapon kind (btUnitGetWeaponSpr). */
extern const u16 BattleUnit_WeaponAnimationsGaret[];
/* Ivan's weapon sprite by weapon kind. */
extern const u16 BattleUnit_WeaponAnimationsIvan[];
/* Mia's weapon sprite by weapon kind. */
extern const u16 BattleUnit_WeaponAnimationsMia[];
/* Jenna's weapon sprite by weapon kind; Felix draws none. */
extern const u16 BattleUnit_WeaponAnimationsJenna[];
/* The size of btBitDecoderDecodeImage, which btBackgroundLoad copies to */
/* HEAP_SLOT_RAM_CODE_TEMP; ld_script.ld defines it as an address equal to */
/* the size. */
extern u8 BitDecoder_Size[];
/* The extra matrix the battle camera's projection applies first while */
/* commands are chosen (FLAG_CHOOSING_COMMANDS), in 16.16 words. */
extern const FIXED BattleCamera_CommandMatrix[];
/* Where SPARK_STATE.sheet holds the spark's square picture for each frame of */
/* life left, 0 to 6: byte offsets, a byte a pixel. */
extern s32 ElementGlow_SparkPictures[];
/* The spark's width and height in pixels by frames of life left: 2 to 4, 4 */
/* from three; a spark with no life left is not drawn. */
extern u8 ElementGlow_SparkSizes[];
/* Where SPARK_STATE.sheet holds the ring's four 32 by 32 frames: byte offsets,
 */
/* 1,024 apart. */
extern s32 ElementGlow_RingPictures[];
/* The highlighted units' BLDY lightening: 15 down to 0 once, then the second */
/* sixteen (a pulse up to 8 and back) over and over. */
extern s8 BattleHighlight_BlendRamp[];
/* The 104 encounter zones (encounter_zones.c), read as ENCOUNTER_ZONE */
/* rows. */
extern u16 EncounterZones[];
/* The HEAP_SLOT_MIND_READ heap slot: Mind Read's wave state while its effect */
/* runs. */
#define MindReadStateWork                                                      \
	(*(struct MIND_READ_STATE **)&HeapSlots[HEAP_SLOT_MIND_READ])
/* The heap slots from HEAP_SLOT_BATTLE_EFFECT, then the canvas and sprite */
/* sheet: the running battle effect's BATTLE_EFFECT_STATE first. */
#define BattleEffectStateWork                                                  \
	(*(void *(*)[]) & HeapSlots[HEAP_SLOT_BATTLE_EFFECT])
/* The HEAP_SLOT_ENEMY_UNITS heap slot: the six enemy battle units, */
/* BATTLE_UNIT_SIZE bytes each, that gmUnitGet returns for unit ids 128 to 133
 */
/* during a battle. */
#define EnemyUnits (*(u8 **)&HeapSlots[HEAP_SLOT_ENEMY_UNITS])
/* The HEAP_SLOT_BATTLE_BACKGROUND heap slot: the camera's turn, which */
/* battle_staging.c, battle_auto_battle.c and battle_turn.c set for each */
/* action, and the background's display mode. */
#define BattleBackgroundStateWork                                              \
	(*(struct BATTLE_BACKGROUND_STATE *                                        \
		   *)&HeapSlots[HEAP_SLOT_BATTLE_BACKGROUND])
/* Isaac's weapon sprite by weapon kind (btUnitGetWeaponSpr). */
extern const u16 BattleUnit_WeaponAnimationsIsaac[];
/* The ARM decoder for a battle background's image, which */
/* btBackgroundLoad copies to RAM and runs as a BIT_DECODER. */
void btBitDecoderDecodeImage(void);

/* btMotionApplyKnockback's knockbacks, the columns of the */
/* BattleKnockback_* tables in knockback_tables.c. Each is named by how far */
/* and how high it sends the unit, as percent of its spot's distance from the */
/* centre and its hop in pixels a frame; all but KNOCKBACK_HURL accelerate by */
/* 1 pixel a frame to at most 4. */
typedef enum BATTLE_KNOCKBACK {
	KNOCKBACK_NUDGE,  /* 140 percent, a hop of 3 */
	KNOCKBACK_HOP,	  /* 140 percent, a hop of 6 */
	KNOCKBACK_LEAP,	  /* 150 percent, a hop of 7 */
	KNOCKBACK_LAUNCH, /* 200 percent, a hop of 10 */
	/* The hardest: 200 percent at 8 pixels a frame more each frame, to at */
	/* most 20, with a hop of 5. */
	KNOCKBACK_HURL,
	/* 150 percent, a hop of 2. From here on a unit already in the air takes */
	/* the new hop too. */
	KNOCKBACK_SKID,
	KNOCKBACK_BOUNCE, /* the spot itself, a hop of 3 */
	KNOCKBACK_JOLT	  /* the spot itself, a hop of 5.5 */
} BATTLE_KNOCKBACK;

/* The battle sprites' animations (fdSprStartAnimation numbers) */
/* that the battle code picks by name. */
typedef enum BATTLE_POSE {
	/* The first standing picture held still, without POSE_STANDING's */
	/* breathing: how an enemy asleep, stunned or held stands. */
	POSE_STILL = 0,
	POSE_STANDING = 1,
	POSE_LEAP = 2, /* running up to a target and leaping */
	POSE_CAST = 3, /* a hand raised to cast or use an item */
	POSE_PARTY_HELD =
		4,			 /* asleep, stunned or held; also a downed party member */
	POSE_DAMAGE = 5, /* taking damage */
	POSE_ENEMY_DOWN = 5 /* a downed enemy stays in its damage pose */
} BATTLE_POSE;

/* A battle unit's sprites' OBJ mode, as btHighlightSetSprMode */
/* sets it: faded units are semi-transparent, and the battle's blends act on */
/* them. */
typedef enum BATTLE_SPR_MODE {
	SPRITE_OPAQUE,
	SPRITE_SEMI_TRANSPARENT
} BATTLE_SPR_MODE;

/* btHighlightSetModes' mode, kept in BATTLE_STATE.highlightMode: */
/* whether the selected units are highlighted, every other unit faded. */
typedef enum BATTLE_HIGHLIGHT {
	HIGHLIGHT_OFF,
	HIGHLIGHT_SELECTED
} BATTLE_HIGHLIGHT;

/* The glows btElementGlowPlay plays about a unit in its element. */
typedef enum BATTLE_ELEMENT_GLOW {
	/* The star field gathering on the unit while the background darkens: a */
	/* cast. */
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

void btDebugBattleViewMessages(void);
void btCameraUpdate(void);
void btIntroAnnounceEncounter(s32 enemyCount);
void btUnitCollect(void);
s32 btShiftVramTiles(void);
void btClearBg0Vofs(void);
void btPartyBattleSetupNoOp(s32 unused);
UNREAD_RESULT btPartyClearBattleStatuses(void);
UNREAD_RESULT btDjinnRecoveryReconcile(void);
struct DJINN_RECOVERY_TABLE *btSummonUpdateAvailability(void);
void btSummonReservedNoOp(void);
s32 btLinkBattleReceiveParty(void);
UNREAD_RESULT btLinkBattleSendParty(void);
u8 *btTextCopyFourCharsUnderscored(u32 unusedA, u32 unusedB, u16 *source);
s32 btLinkBattleWaitSync(void);
s32 btLinkBattleSyncTurn(void);
UNREAD_RESULT btPartyAssignOpponentSlots(void);
s32 btClearSerialTransferPayload(void);
/* btRun's results besides the number of enemies defeated. */
#define BATTLE_OUTCOME_LOST -1
#define BATTLE_OUTCOME_FLED 999
void btCameraAdjustByShoulderKeys(void);
void btCameraAdjustByShoulderKeysAlt(void);
s32 btUnitCountDownDefenseStage(s32 unitId);
s32 btUnitCountDownResistanceStage(s32 unitId);
s32 btUnitCountDownDelusion(s32 unitId);
s32 btUnitCountDownCharm(s32 unitId);
s32 btUnitCountDownStun(s32 unitId);
s32 btUnitCountDownSleep(s32 unitId);

/* Pushes a message to show, waiting for the player to read it. */
#define PUSH_MESSAGE(message) btPlaybackPush(PLAYBACK_MESSAGE, (message))

/* Defines a function that turns the battle camera a 128th of a turn each frame
 */
/* L or R is held, and centres the background's zoom unless a view holds it. */
/* Two such functions exist, one each in battle_auto_battle.c and */
/* battle_turn.c. */
#define DEFINE_ADJUST_CAMERA(name)                                             \
	void name(void) {                                                          \
		struct BATTLE_CAMERA *cam = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);         \
		struct BATTLE_BACKGROUND_STATE *view =                                 \
			HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);                      \
                                                                               \
		if((KeysHeld & KEY_L) != 0) {                                          \
			cam->yaw += 512;                                                   \
		}                                                                      \
		if((KeysHeld & KEY_R) != 0) {                                          \
			cam->yaw -= 512;                                                   \
		}                                                                      \
		if(view->keepRange == 0) {                                             \
			btCameraSetRange(toFIXED(120), toFIXED(120), 0, 0, FIXED_ONE);     \
		}                                                                      \
	}

s32 btEnemyList(s16 *unitIds);
void btObjectPutKeepingPoses(void);
s32 btEnemyHasSprRoom(s32 enemyId);
s32 btEnemySprLoadIntoSlots(s32 unitId);
UNREAD_RESULT btObjectReleaseSprSlots(s32 unitId);
s32 btUnitGetWeaponKind(s32 itemId);
s32 btUnitGetWeaponSpr(s32 unitId);
void btObjectMake(struct BATTLE_OBJECT *battleObject, s32 unitId, s32 x, s32 y);
void btPlacementGetStepPair(s32 partyPlace, s32 *x, s32 *y);
void btPlacementLayoutEnemies(u16 *unitIds, s32 count, s32 *xPositions,
							  s32 *zPositions);
s32 btEnemyGetFreeUnitId(void);
UNREAD_RESULT btPlacementRelayoutEnemies(void);
void btUnitRefreshPlacement(void);
s32 btPlacementContainsId(s16 *list, s32 place);
void btObjectUpdateSprPriorities(void);
UNREAD_RESULT btUnitBuildStatFlags(s32 unitId,
								   struct BATTLE_OBJECT *battleObject);
UNREAD_RESULT btStatIconCycle(struct BATTLE_OBJECT *battleObject);
UNREAD_RESULT btObjectUpdatePose(s32 unitId);
void btSprDetachScripts(void *recordGroup);
void btActorDetachSprScripts(struct ACTOR *actor);
s32 btObjectMarkForRelease(s32 unitId);
#ifdef UBFIX
void btObjectDestroyAll(void);
#else
void *btObjectDestroyAll(void);
#endif
s32 btObjectGetAnchor(s32 place, POS *position);
UNREAD_RESULT btCameraApplyTransform(void);
void btCameraSetDefaultView(void);
void btMotionReturnToSpot(s32 unitId);
void btMotionSetupEscapeObject(s32 unitId);
void btMotionLeapToward(struct BATTLE_OBJECT *startSlot,
						struct BATTLE_OBJECT *endSlot, s32 progress);
void btMotionReturnObjectToSpot(struct BATTLE_OBJECT *battleObject);
void btMotionHopBack(s32 unitId);
void btMotionRestartLeap(s32 unitId);
void btMotionMidpointNoOp();
void btMotionApplyPairMidpoint(s32 firstId, s32 secondId);
void btMotionPlayHitFlash(s32 unitId);

s32 btAutoBattleBuildActions(struct BATTLE_ACTION *entries);
s32 btAutoBattleRunAction(struct BATTLE_ACTION *action);
s32 btObjectIsValidId(u32 unitId);
s32 btFleePlayRun(struct BATTLE_ACTION *action);
s32 btAutoBattleShowDefend(const struct BATTLE_ACTION *action);
s32 btAutoBattleRunPsynergy(struct BATTLE_ACTION *action);
s32 btAutoBattleRunApproachAttack(struct BATTLE_ACTION *input);
s32 btAutoBattleRunAttackOrItem(struct BATTLE_ACTION *action);
void btAutoBattleApplyDamage(u32 unitId, s32 damage, s32 showMessage,
							 u8 *context);
void btMotionPlayFall(s32 unitId);
s32 btAbilityReplaceDefeated(const struct BATTLE_ACTION *action);

void btPaletteUpdatePulseBrightness(void);
void btUnitResetGuardLevels(void);
s32 btFleeCheckSuccess(void);
s32 btBuildPartyActions(struct BATTLE_ACTION *entries);
s32 btBuildEnemyActions(struct BATTLE_ACTION *entries);
void btActionOrderSort(struct BATTLE_ACTION *entries, s32 count);
s32 btAppendLinkedActions(struct BATTLE_ACTION *actions, s32 count);
void btPaletteCopyWithBrightnessOffset(s32 offset);
s32 btGatherActions(struct BATTLE_ACTION *actions);
s32 btGetUnitAtPlace(s32 taggedPlace);
s32 btFindUnitPlace(u32 unitId);
void btTurnStartNoOp(void);

s32 btStagingDispatchAction(struct BATTLE_ACTION *request, s32 delay);
UNREAD_RESULT btStagingBuildTargetList(struct BATTLE_ACTION_RUN *plan,
									   struct BATTLE_EFFECT_PARM *args);
s32 btStagingRunFlee(struct BATTLE_ACTION_RUN *plan);
s32 btStagingRunFullScr(struct BATTLE_ACTION_RUN *plan, s32 mode);
UNREAD_RESULT btStagingRunMessages(struct BATTLE_ACTION_RUN *plan, s32 unused);
s32 btStagingRunAttack(struct BATTLE_ACTION_RUN *plan, s32 mode);
s32 btStagingRunPlainAttack(struct BATTLE_ACTION_RUN *input, s32 flags);
s32 btStagingRunItem(struct BATTLE_ACTION_RUN *plan, s32 unused,
					 struct BATTLE_ACTION *selection);
UNREAD_RESULT btObjectSetColMode(struct ACTOR *actor, s32 palette);
s32 btStagingRunPsynergyCast(struct BATTLE_ACTION_RUN *plan, s32 mode);

void btMotionFlashHit(s32 unitId, s32 unusedA, s32 unusedB);
void btObjectRemoveFromLists(s32 unitId);
void btMotionShowDowned(s32 unitId);
s32 btAIChooseRandomTarget(s32 requireLivingUnit);
s32 btAIChooseTarget(s32 userId, struct ABILITY_DEFINITION *action);
UNREAD_RESULT btUnitClearStatuses(s32 unitId);
UNREAD_RESULT btMessageWaitForAdvance(void);
void btPlaybackMarkMessage(void);
UNREAD_RESULT btUnitRemoveEnemy(s32 unitId);
void btPlaybackGrantDjinnBonus(struct BATTLE_PLAYBACK_STATE *runtime,
							   u32 unused);
#ifdef UBFIX
void btPlaybackRunAll(void);
#else
u32 btPlaybackRunAll(void);
#endif
u32 btPlaybackPush(u32 opcode, u32 operand);
s32 btAbilityEffectAppliesToDowned(s32 effect);
UNREAD_RESULT btAbilityApplyToTarget(struct BATTLE_ACTION_RUN *plan, s32 slot);
u32 btAbilityIsPsynergy(s32 abilityId);
UNREAD_RESULT btAIChooseAction(struct BATTLE_ACTION *request, s32 acting);
void btDmaStopDma3(void);
s32 btPlaybackSchedulePhase(s32 frames);
void btPlaybackRun(void);
void btPlaybackReset(void);
UNREAD_RESULT btPlaybackWaitForReady(void);
u32 btUnitIsListed(u32 unitId);
s32 btActionRunBuild(struct BATTLE_ACTION *request,
					 struct BATTLE_ACTION_RUN *plan);

s32 btUnitRollCounterEarlyEnd(s32 unitId, s32 turns, s32 bias);
s32 btUnitCountDownAttackStage(s32 unitId);
s32 btUnitCountDownConfusion(s32 unitId);
s32 btUnitCountDownPsynergySeal(s32 unitId);
s32 btUnitCountDownRegenerate(s32 unitId);
s32 btUnitCountDownReflect(s32 unitId);
s32 btUnitCountDownAgilityStage(s32 unitId);
void btTurnEndNoOp(void);
UNREAD_RESULT btProcessTurnEnd(void);
UNREAD_RESULT btUnitProcessActionEnd(struct BATTLE_ACTION_RUN *plan);

void btTransitionBuildAffineTilemap(void *tilemap);
void btTransitionBuildFlatTilemap(void *tilemap);
void btBackgroundShowZoomedPage(void);
void btTransitionUploadTileVariant(void);
void btTransitionAdvanceOpening(void);
void btTransitionDrawRows(void);
void btTransitionClearBg0Vofs(void);
void btTransitionRun(s32 enemyCount, s32 encounterId);
void btBackgroundInit(void);
void btBackgroundFree(void);
void btCameraSetRange(FIXED centerX, FIXED centerY, s32 offsetX, s32 offsetY,
					  FIXED scale);
#ifdef UBFIX
void btCameraSetViewAtDepth(s32 x, s32 depth, s32 y, s32 zoom);
#else
void btCameraSetViewAtDepth(s32 x, s32 depth, s32 y);
#endif
/* BG1's map during a battle, a screen block: the battle background's, where */
/* the command row also places its icons' tiles. */
#define BATTLE_BG1_SCREEN_BLOCK 31
/* BG1's vertical scroll that sets the battle background in the battle view. */
#define BATTLE_BACKGROUND_SCROLL_Y 32
/* btCameraSetView's zoom, in percent, for the battle's usual view. */
#define BATTLE_ZOOM_NORMAL 100
void btBackgroundSetBlendCtrl(void);
void btTransitionInitEntries(struct BATTLE_TRANSITION_ENTRY *entries);
s32 btBackgroundDivideBy16(s32 value);
s32 btAdjustDebugValue(s32 selectedValue);
void btHighlightSetSprMode(s32 unitId, s32 mode);
void btHighlightSetOtherSprModes(s32 selectedUnitId);
void btHighlightClearAllSprModes(void);
void btBackgroundStepHighlightBlend(void);

UNREAD_RESULT btElementGlowUpdateStarField(void);
#ifdef UBFIX
void btElementGlowUploadStarField(void);
#else
u32 btElementGlowUploadStarField(void);
#endif
s32 btElementGlowInitStarField(s32 element);
s32 btElementGlowEnd(s32 unused);
/* UB: without UBFIX, btElementGlowPlay's callers see no prototype and take it
 */
/* to return int; it is defined returning nothing. */
#ifdef UBFIX
void btElementGlowPlay(s32 unitId, s32 element, s32 mode, s32 variant);
#endif
void btBackgroundRestorePalette(void);
void btIrqRemoveVCountHandler(void);

s32 btFormationAverageLevel(s32 formationId);
s32 btFormationSelectLevelMatchedCandidate(s32 *outMargin);
#ifdef UBFIX
void btEnemyAdjustLevel(s32 unitId, s32 levels);
#else
s32 btEnemyAdjustLevel(s32 unitId, s32 levels);
#endif
s32 btEnemyClaimNameLetter(s32 enemyId, s32 unused);
UNREAD_RESULT btEnemyReleaseNameLetter(s32 unitId);
UNREAD_RESULT btEnemyLetterFirstOfKind(s32 enemyId);
s32 btFormationGetRandomMember(s32 formationId);
s32 btFormationBuildEnemyList(s32 formationId);

u32 btSprGetDefeatSnd(s32 index);
s32 btSprGetSpr(s32 index);
s32 btSprGetPalette(s32 index);
u32 btSprGetAttackEffect(u32 index);

u32 btSpoilsRankItem(u32 inventoryEntry);
void btSpoilsClear(void);
s32 btEnemyRecordDefeat(s32 unitId, s32 djinnBonus);
void btAwardSpoils(void);
void btDebugSelectNoOp(void);

#endif
