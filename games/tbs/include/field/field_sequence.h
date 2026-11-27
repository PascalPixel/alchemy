#ifndef ALCHEMY_FIELD_SEQUENCE_H
#define ALCHEMY_FIELD_SEQUENCE_H

/* @file */
/* What the field sequence's object (field_sequence.c) defines for the code */
/* outside it: encounters, the field's actors, the leader's walking, the */
/* scene's triggers, treasure spots, screen transitions and the palette fade. */

#include "types.h"
#include "fixed_math.h"
#include "field/field_particle.h"

struct SPEAKER_DEFINITION;
struct ACTOR;
struct SCR_TRANSITION_STATE;
struct TRIGGER;
struct VECTOR3;
struct ACTOR_PLACE;
struct SPR;
struct WINDOW;

/* The exit request whose destination is already in MainRoutineStateWork.next */
/* and .entrance, so that scTakeExit leaves them as they are. */
#define EXIT_KEEP_DESTINATION 999

/* MainRoutineStateWork.movementMode: how the field sequence hands the party */
/* leader back to the player (fdSequenceRun). */
typedef enum LEADER_MOVEMENT_MODE {
	/* Walking: the pad moves the leader, on the World Map or a layered map. */
	LEADER_MOVEMENT_WALK = 0,
	LEADER_MOVEMENT_CLIMB = 1, /* on a climbable wall, moved 8 pixels a step */
	/* Carried through the air (Suhalla Desert's storms, the World Map's */
	/* airborne travel): only the configured keys are checked. */
	LEADER_MOVEMENT_AIRBORNE = 2
} LEADER_MOVEMENT_MODE;

/* A window transition's shape: SCENE_TRANSITION(TRANSITION_WINDOW, mode)'s */
/* low byte, which fdScrTransitionUpdateScanlineTable draws. */
typedef enum WINDOW_TRANSITION_MODE {
	WINDOW_TRANSITION_BARS = 0,	  /* a bar on each line, at random */
	WINDOW_TRANSITION_SHAPE = 1,  /* a shape from ScreenTransition_Shapes */
	WINDOW_TRANSITION_NOISE = 2,  /* ragged lines from the left edge */
	WINDOW_TRANSITION_CIRCLE = 3, /* a circle at the screen's centre */
	/* Alternate lines wiping in from the left and the right. */
	WINDOW_TRANSITION_INTERLACED_WIPE = 4,
	/* As WINDOW_TRANSITION_LEADER with inside and outside swapped and the */
	/* progress run the other way; it flickers. */
	WINDOW_TRANSITION_LEADER_INVERSE = 7,
	/* An ellipse around a point (the state's centre) whose outline ripples: */
	/* Reveal's. */
	WINDOW_TRANSITION_RIPPLE = 8,
	/* An ellipse wider than tall around the party leader, 16 pixels above its
	 */
	/* feet. */
	WINDOW_TRANSITION_LEADER = 9,
	/* An ellipse a little taller than wide, 8 pixels above the leader's feet.
	 */
	WINDOW_TRANSITION_LEADER_LOW = 77
} WINDOW_TRANSITION_MODE;

/* The encounter table's zones (encounter_zones.c's entries) that code names: */
/* the World Map's, two to a region (its open ground, then its forest), and */
/* the rows no step on a map rolls. */
typedef enum ENCOUNTER_ZONE_ID {
	ENCOUNTER_ZONE_NONE = 0, /* no battles */
	ENCOUNTER_ZONE_WORLD_MAP_VALE_VAULT = 7,
	ENCOUNTER_ZONE_WORLD_MAP_VALE_VAULT_FOREST = 8,
	/* The event battles of Vale's stormy night (its monsters, and the */
	/* travelers who catch Isaac in Vale's plaza), Vault's thieves and the World
	 */
	/* Map's Djinn tutorial battle. */
	ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT = 12,
	ENCOUNTER_ZONE_WORLD_MAP_BILIBIN_KOLIMA = 13,
	ENCOUNTER_ZONE_WORLD_MAP_BILIBIN_KOLIMA_FOREST = 14,
	ENCOUNTER_ZONE_WORLD_MAP_SNOWFIELD = 29,
	ENCOUNTER_ZONE_WORLD_MAP_SNOW_FOREST = 30,
	/* The event battles of Tret Tree (Tret) and Mercury Lighthouse (the */
	/* Lizard Man, then Saturos at the aerie). */
	ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY = 36,
	ENCOUNTER_ZONE_WORLD_MAP_FUCHIN = 37,
	ENCOUNTER_ZONE_WORLD_MAP_FUCHIN_FOREST = 38,
	ENCOUNTER_ZONE_WORLD_MAP_XIAN_ALTIN = 45,
	ENCOUNTER_ZONE_WORLD_MAP_XIAN_ALTIN_FOREST = 46,
	/* The event battles of Mogall Forest, Altin Peak and Lamakan Desert, */
	/* whose last two slots hold the battles with the Ant Lions and the */
	/* Manticore. */
	ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN = 53,
	ENCOUNTER_ZONE_WORLD_MAP_KALAY = 54,
	ENCOUNTER_ZONE_WORLD_MAP_KALAY_FOREST = 55,
	/* The monster attacks on the Tolbi-bound ship, the Kraken's last. */
	ENCOUNTER_ZONE_TOLBI_SHIP = 62,
	/* The monsters behind the east doors of Crossbone Isle Cave's first four */
	/* floors, one slot a floor. */
	ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS = 63,
	ENCOUNTER_ZONE_WORLD_MAP_TOLBI = 64,
	ENCOUNTER_ZONE_WORLD_MAP_TOLBI_FOREST = 65,
	/* The battles that end the Colosso finals' matches, the slot chosen by */
	/* how the match went. */
	ENCOUNTER_ZONE_COLOSSO = 72,
	ENCOUNTER_ZONE_WORLD_MAP_SUHALLA = 80,
	ENCOUNTER_ZONE_WORLD_MAP_SUHALLA_FOREST = 81,
	/* The monsters behind the east doors of Crossbone Isle Cave's next five */
	/* floors, one slot a floor, then Deadbeard on the Pirate Ship. */
	ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS = 84,
	/* The event battles: Lunpa Fortress, Suhalla Desert, Venus Lighthouse */
	/* Aerie and the World Map; its first slot is the Aerie's second battle. */
	ENCOUNTER_ZONE_SCRIPTED = 98,
	ENCOUNTER_ZONE_MIMIC = 99, /* mimic chests, by the chest's slot */
	/* The Djinn battles, one zone for each element in element order. */
	ENCOUNTER_ZONE_VENUS_DJINN = 100,
	ENCOUNTER_ZONE_MERCURY_DJINN = 101,
	ENCOUNTER_ZONE_MARS_DJINN = 102,
	ENCOUNTER_ZONE_JUPITER_DJINN = 103
} ENCOUNTER_ZONE_ID;

/* The field-sequence module's functions in the order of its stub table */
/* (src/field/sequence_stubs.s). */
s32 fdSequenceRun(s32 entrance);
void gmMainRoutineRun(s32 mode);
void fdEventWait(s32 frames);
void fdEventStart(void);
void fdEventEnd(void);
void fdEventRunActorUpdateAndWait(struct ACTOR_PLACE *table);
void fdEventAppendActorPlacements(struct ACTOR_PLACE *table);
void scAssignViewFlags(struct ACTOR_PLACE *entry);
void fdEventSetTriggers(const struct TRIGGER *triggers);
void fdEventAddPartyMember(s32 unit, s32 announce);
void fdEventAddPartyMemberPair(s32 first, s32 second);
void fdPartyRemoveMemberRestored(s32 unit);
s32 fdPartyInventoryGiveItem(s32 item, s32 unused);
s32 fdInventoryTryAddAndReturnMember(s32 item, s32 unused, s32 unit);
s32 fdEventAskYesNoWithActorAnimation(s32 actorId, s32 forceBottom);
void fdPartyLearnAbility(s32 unit, s32 ability);
struct ACTOR *fdActorGetById(s32 actorId);
void fdActorReleaseById(s32 actorId);
void fdActorSetSpeedParms(u32 actorId, FIXED speedLimit, FIXED acceleration);
void fdActorSetMovementTypeById(u32 actorId, s32 movement);
void fdActorWaitForScriptEndById(u32 actorId);
void fdActorStop(u32 actorId);
void fdActorSetMovementTypeAndWaitById(u32 actorId, s32 movement);
void fdActorSetDestinationById(u32 actorId, s32 x, s32 z);
void fdActorMoveToAndWaitById(u32 actorId, s32 x, s32 z);
void fdActorWalkToById(u32 actorId, s32 x, s32 z);
void fdActorWalkToAndWaitById(u32 actorId, s32 x, s32 z);
void fdActorCenterOnColumn(u32 actorId, s32 priority, s32 zOffset);
void fdActorSetDestinationOffsetById(u32 actorId, s32 xOffset, s32 zOffset);
void fdActorWaitForMoveById(u32 actorId);
void fdActorSetHorizontalPosWithTerrain(u32 actorId, FIXED x, FIXED z);
void fdActorSetPosWithTerrain(u32 actorId, FIXED x, FIXED y, FIXED z);
void fdActorSetAnimationById(u32 actorId, s32 animation);
void fdActorSetAnimationSpeedById(u32 actorId, s32 steps);
void fdActorSetAnimationAndWait(u32 actorId, s32 animation);
void fdActorWaitForAnimationChange(u32 actorId);
void fdActorNoOp(void);
void fdActorJump(u32 actorId, s32 speed, s32 frames);
void fdActorStartRepeatedMotion(u32 actorId, s32 count);
void fdActorRunRepeatedMotion(u32 actorId, s32 count);
void fdActorJumpDownLedge(s32 actorId, u32 pose, s32 dustColor);
void fdActorFaceActorById(s32 firstId, s32 secondId, s32 frames);
void fdActorTurnToFaceEachOther(s32 firstId, s32 secondId, s32 frames);
void fdActorConfigureSprCol(s32 actorId, s32 color);
void fdActorSetSprCol(struct ACTOR *actor, s32 color);
void fdActorWatchAndStartScript(u32 actorId, s32 targetId, const void *script);
void fdEventSetMessageId(s32 message);
struct WINDOW *fdMessageShowAtActor(s32 speaker, s32 unusedFlags);
void fdEventShowMessageAndWait(s32 actorId, s32 flags);
void fdEventShowMessageThenWait(s32 actorId, s32 flags, s32 frames);
s32 fdEventAskYesNoAndShowResponse(s32 actorId, s32 flags);
void fdEventEmptyCallback(void);
void fdEventShowSpeakerMessage(s32 speaker);
void fdEventForwardShowMessageAtPos(s32 unusedA, s32 unusedB, s32 x, s32 y);
void fdEventShowMessageAtPos(s32 unusedA, s32 unusedB, s32 x, s32 y);
void fdActorFaceDirectionById(s32 actorId, s32 angle, s32 frames);
void fdActorClearDestinationAndStand(u32 actorId);
void fdActorClosePortrait(s32 actorId);
void fdEventShowTwoMessagesAndWait(s32 first, s32 firstX, s32 firstY,
								   s32 firstPortraitX, s32 firstPortraitY,
								   s32 second, s32 secondX, s32 secondY,
								   s32 secondPortraitX, s32 secondPortraitY);
void fdMessageSetVoiceFromActor(s32 actorId);
void fdActorSetSprPriority(s32 actorId, s32 priority);
void fdActorShowEmoteById(s32 actorId, s32 emote, s32 frames);
void fdEffectSetAttached(s32 actorId, s32 flags);
void fdActorCopyGraphics(u32 targetId, u32 sourceId);
void fdCameraFollowActor(s32 actorId, s32 keepPosition);
void fdCameraSetSpeedParms(FIXED speedLimit, FIXED acceleration);
void fdCameraMoveWithinMapBounds(FIXED requestedX, FIXED requestedY,
								 FIXED requestedZ, s32 glide);
void fdCameraWaitForArrival(void);
void fdCameraMoveToActorById(s32 actorId, s32 glide);
struct ACTOR *fdCameraGetActor(void);
void fdMapGridSetCameraBounds(s32 minX, s32 minY, s32 maxX, s32 maxY);
void fdEventSetDestination(s32 sceneId, s32 entrance);
void fdEventSetRestartPoint(u16 sceneId, u16 entrance);
void fdEventSetExitRequest(u16 request);
void fdRequestBattleFromEncounterTable(s32 zone, s32 slot);
void fdRequestDjinnBattle(s32 actorAndHop, s32 djinni);
void fdEventSetAfterBattleDestination(s32 sceneId, s32 entrance);
void fdEventSetDefeatDestination(s32 sceneId, s32 entrance);
void fdEventLeaveFor(u16 sceneId, u16 entrance);
s32 fdActorGetScrPos(s32 actorId, s32 *position);
s32 fdActorGetTriggerCellAheadOfCurrent(void);
s32 fdPartyGetCharacterSpr(s32 character);
void fdPsynergySteerPushedTarget(s32 targetId);
void fdPsynergyStartCloak(void);
s32 fdTryJumpForward(void);
s32 fdClimbDown(void);
s32 fdClimbUp(void);
void fdLeaderStopMoving(void);
void fdWeatherStartRain(void);
void fdWeatherStartLightning(void);
void fdWeatherStopLightning(void);
void fdWeatherLastLightning(void);
void fdWeatherResumeLightning(void);
void fdWeatherStartLightningFlash(s32 from, s32 to);
void fdMotesStart(void);
void fdEffectInitSparkles(void);
void fdScrTransitionStartHeldWindow(s32 mode);
void fdScrTransitionStartDissolveHeld(s32 mode);
void fdScrTransitionStart(s32 mode, s32 frames);
void fdScrTransitionFinish(s32 mode, s32 frames);
void fdScrTransitionHideBackgrounds(s32 clearBg3, s32 clearBg2, s32 clearBg1);
void fdPaletteFadeInit(void);
void fdPaletteFadeSetTarget(s32 mode, s32 half);
void fdPaletteFadeSetSource(s32 mode, s32 half);
void fdPaletteFadeCalcSteps(s16 *source, s16 *target, s16 *step, s32 frames);
void fdPaletteFadeStartInterpolation(s32 frames);
void fdPaletteFadeRelease(void);
void fdPaletteFadeSetBackdrop(u32 color);
void fdEventStartScrTransition(void);
void fdEventFinishScrTransition(void);
void fdEventWaitTransitionFrames(void);
/* Starts the ripple: for x and y, the wave's frequency, its step each frame */
/* and its amplitude. */
void scLineScrollInitWave(s32 mode, FIXED freqX, s32 stepX, FIXED ampX,
						  FIXED freqY, s32 stepY, FIXED ampY);
void fdLineScrollStop(void);
s32 fdChestOpenAndShowItem(s32 actorId, s32 target, s32 itemId);
void fdItemDispShowPicture(void *object, s32 flags);
void fdItemDispShowItem(s32 item, s32 flags);
struct ACTOR *fdTreasureSpotGetActor(s32 id);
void fdTreasureSpotOpenChest(s32 id);
void fdTreasureSpotCloseChest(s32 id);
void fdTreasureSpotShowOpened(s32 id);
void fdTreasureSpotSetActorPos(s32 id, FIXED x, FIXED z);
void fdTreasureSpotHide(s32 id);
void fdTreasureSpotMoveLeaderAway(void);
void fdWorldMapFreeze(void);
void fdWorldMapUnfreeze(void);
void fdActorResetForLeader(struct ACTOR_PLACE *table);
void fdEventSpawnActorPlacements(struct ACTOR_PLACE *entry, s32 slot);
void fdActorReleaseAtIndex(s32 actorIndex);
void *fdActorLookup(u32 actorIndex);
void fdEffectStartGroundMask(void *object);
void fdGroundMaskSuspend(void);
void fdGroundMaskResume(void);
void fdPsynergyRunHandler(s32 kind);
void fdPsynergyLoadResources(s32 abilityId, s32 mode);
void fdPsynergySetupActorPair(s32 userId, s32 targetId);
void fdPsynergyStopCasterUpdate(void);
void fdPsynergyEnd(void);
void fdPsynergyStartCast(void);
void fdPsynergyFadeToCastCol(void);
void fdPsynergyFadeFromCastCol(void);
void fdEndBusyAndReveal(void);
void fdPsynergyStartMindReadWave(s32 casterId, s32 targetId);
void fdPsynergyStopMindReadWave(void);
void fdPsynergyGreyMindReadPalette(void);
void fdPsynergyRestoreMindReadPalette(void);
s32 scHasPsynergyTrigger(s32 group);
u32 scHasItemTrigger(u16 item);
struct SPR *fdSprShareVramCacheEntry(struct SPR *sourceSprite,
									 struct SPR *sharedSprite);
s16 fdBattleBackgroundGetSelected(void);
struct ACTOR *fdEffectSpawnMoveHand(struct ACTOR *caster);
void fdEffectGrowHandAndWait(struct ACTOR *actor);
void fdEffectBurstHand(struct ACTOR *hand);
void fdPartyAddPp(s32 amount, s32 unused);
void fdPartyAddHp(s32 amount, s32 percent);
s32 fdVramCacheCountFreeBlocks(void);
s32 fdYesNoAsk(s32 atBottom);
u16 fdEncounterGetFormationAtSlot(s32 zone, s32 slot);
s16 fdEncounterGetDjinnFormation(s32 djinni);
void scPlayMusic(void);
void fdDjinnJoinParty(s32 actorId, s32 element, s32 djinn);
void fdParticleUpdate(FIELD_PARTICLE *effect);
u32 fdParticleHasPendingTarget(FIELD_PARTICLE *effect);
void fdParticleSetPos(FIELD_PARTICLE *effect, s32 x, s32 z);
void fdParticleSetAnimation(FIELD_PARTICLE *effect, s32 mode);
void fdParticleSetCallback(FIELD_PARTICLE *effect, EFFECT_CALLBACK callback);
void fdParticleInit(FIELD_PARTICLE *effect, s32 kind, s32 x, FIXED z);
void fdEffectClearOwnedSlot(FIELD_PARTICLE *effect);
void fdCameraWorldToScr(struct VECTOR3 *position);
s32 fdSpeakerGetVoice(u32 spriteId);
void fdPartyClearUnusableShortcuts(void);
void fdEffectInitSlots(void);
void fdEffectReleaseSlots(void);
void fdActorClearOutOfBounds(void);
void fdActorRespawn(void);
void fdActorLaunchFromLeader(u32 actorId, s32 xOffset, s32 zOffset, s32 facing);
void fdActorWalkByById(u32 actorId, s32 xOffset, s32 zOffset);
void fdActorWalkByAndWaitById(u32 actorId, s32 xOffset, s32 zOffset);
void fdActorWatchAndSetScript(u32 actorId, u32 watchedActorId);
void fdPsynergyHaltActor(s32 actorId);
void fdPsynergyResumeHaltedActor(s32 actorId);
s32 fdActorFindBySpr(s32 sprite);
s32 fdActorGetSprById(s32 actorId);
void fdActorSnapshot(void);
void fdActorRestore(void);
void fdWorldMapStartZoom(FIXED zoom, s32 frames);
void fdEventWaitForZoomTransition(void);
s32 fdAreaGetNameIndex(s32 sceneId, s32 entrance);
void fdActorSetMovementType(struct ACTOR *actor, s32 movement);
void scQueueMusic(s32 soundId);
void scPlayQueuedMusic(void);
s32 fdPartyGetCharacterSprForChildhood(s32 character, s32 childhood);
#if TBS_PATCH_2
void fdEffectReleaseRevealSparkles(void);
#endif

void fdEncounterSelectMapZones(s32 useRules);
s32 fdEncounterResolveWorldMapZoneAndBackdrop(void *pos);
s32 fdEncounterCountStepsAndSelectFormation(s32 zone, FIXED steps);
s32 fdEncounterRollMapStep(u32 terrain, FIXED steps);
s32 fdEncounterRollWorldMap(s32 pos, FIXED steps);
void scChooseMusic(void);
void scSetRestartPoint(void);
void fdBattleBackgroundSelectForMap(void);
void fdBattleBackgroundSelectForEnvironment(s32 behavior);
/* The field sequence calls fdEncounterSelectBattleMusic without a prototype. */
#ifdef UBFIX
void fdEncounterSelectBattleMusic(s32 zone, s32 slot);
#endif
void fdEffectShrinkActorAndDestroySlow(struct ACTOR *actor);
void fdEffectShrinkActorAndDestroyFast(struct ACTOR *actor);
void fdPsynergyLift(void);
void fdPsynergyCarry(void);

/* A trigger result below it is a message number, and from it on the address of
 */
/* the function to run: every function lies above it, so one word holds either.
 */
#define TRIGGER_RESULT_MESSAGE_END 65536

/* These masks decode the trigger and search-result words whose bit layouts */
/* and values scene_tables.h gives (TRIGGER, TRIGGER_KIND, FIND_). */

/* A trigger's control bits 3..0, its kind (TRIGGER_KIND). */
#define TRIGGER_KIND_MASK 0xf
/* A Psynergy trigger's control bits 30..28, its phase (PSYNERGY_PHASE). */
#define PSYNERGY_PHASE_MASK (7u << 28)
/* An examine trigger's control bits 8..0, its kind and what it is on, as */
/* EXAMINE_TRIGGER() packs them. */
#define EXAMINE_TRIGGER_MASK 0x1ff
/* A trigger's control bits 15..12: the heading TRIGGER_FLAG_FACING compares, */
/* in sixteenths of a turn. */
#define TRIGGER_FACING_MASK 0xf000

/* A search result's type, its top twelve bits (FIND_ITEM to FIND_MIMIC). */
#define FIND_TYPE_MASK 0xfff00000
/* Bits 27..24 of a search result, set only in a function's address: the result
 */
/* is then a function to call. */
#define FIND_FUNCTION_BITS 0x0f000000
/* Bits 19..16 of a chest's search result, which fdExamineCell compares */
/* only with FIND_CHEST_LOCKED. No chest of the game sets them, so none is */
/* locked. */
#define FIND_CHEST_LOCK_MASK 0xf0000
/* A chest that stays shut while a party member (up to ALLY_MAX) leads: "can't
 */
/* open it..." (MsgCantOpenIt). */
#define FIND_CHEST_LOCKED 0x10000

/* A field Psynergy or item use, packed into one value, as FIELD_STATE.psynergy
 */
/* and FIELD_STATE.item hold it. bits: 13..10 the user (13 is ACT_PACKED) | 9..0
 */
/* the Psynergy or item. */
#define ACT_ID_MASK	   0x3ff			   /* bits 9..0 */
#define ACT_UNIT_SHIFT 10				   /* bits 13..10 */
#define ABILITY_ID(c)  ((c) & ACT_ID_MASK) /* the Psynergy */
#define ACT_ITEM_ID(c) ((c) & ACT_ID_MASK)
#define ACT_UNIT(c)	   (((c) >> ACT_UNIT_SHIFT) & 15) /* who uses it */
#define ACT_PACKED	   0x2000 /* use it without paying PP */
/* The Psynergy request that ends Cloak, without paying PP. */
#define CLOAK_END (ACT_PACKED | ABILITY_CLOAK)

/* A 16.16 coordinate's whole cell: a map grid cell is 16 pixels square. */
#define FIELD_CELL_MASK 0xfff00000
/* A 16.16 coordinate's shift to its map grid cell: 16 bits of fraction and 4 */
/* of the cell's 16 pixels. */
#define FIELD_CELL_SHIFT 20
/* The map grid cell a 16.16 coordinate lies in, counted from 0 along its */
/* axis. */
#define FIELD_CELL(coordinate) ((coordinate) >> FIELD_CELL_SHIFT)
/* The half cell, 8 pixels, a 16.16 coordinate lies in, counted from 0 along */
/* its axis. */
#define FIELD_HALF_CELL(coordinate) ((coordinate) >> (FIELD_CELL_SHIFT - 1))
/* The 16.16 coordinate of a half cell's first pixel, FIELD_HALF_CELL undone. */
#define FIELD_HALF_CELL_ORIGIN(halfCell) ((halfCell) << (FIELD_CELL_SHIFT - 1))
/* A 16.16 coordinate's whole pixels, its fraction cleared. */
#define FIELD_PIXEL_MASK 0xffff0000
/* The 16.16 coordinate of a map grid cell's first pixel, FIELD_CELL undone. */
#define FIELD_CELL_ORIGIN(cell) ((cell) << FIELD_CELL_SHIFT)
/* The 16.16 coordinate of the middle of a map grid cell. */
#define FIELD_CELL_CENTER(cell) (FIELD_CELL_ORIGIN(cell) + toFIXED(8))

/* The frames the leader pushes against a step trigger, a ledge, a gap or a */
/* wall before it runs, jumps or climbs (FIELD_STATE.blockedSteps). */
#define FIELD_PUSH_FRAMES 12

/* -------------------------------------------------------------------------- */
/* The map's treasure spots (chests and Psynergy Stones) */
/* -------------------------------------------------------------------------- */
/* The most treasure spots a map places. */
#define TREASURE_SPOT_MAX 10
/* The ids a treasure spot's trigger answers to; the map's list of spots */
/* ends at column and row 255. */
#define TREASURE_SPOT_FIRST_ID 100
#define TREASURE_SPOT_LAST_ID  239
#define TREASURE_SPOT_LIST_END 255

s32 fdActorFindLastActiveId(void);
void *fdActorGetSlotAddress(u32 actorIndex);
void fdActorClearAll(void);

void fdClearRequests(void);
s32 fdCountRequests(void);
s32 fdGetCollisionAheadOfLeader(void);
void fdCheckExitRegions(FIXED positionX, FIXED positionY, FIXED positionZ);
void fdUpdateWalking(s32 layer, FIXED x, FIXED y, FIXED z);
s32 fdPartyApplyPoisonDamage(void);
void scApplyEntrance(void);
void fdPaletteEditorRun(void);

struct ACTOR_PLACE *scFindActorPlacement(s32 id);
s32 fdGameFlagIsConditionActive(s32 condition);
u32 scExamineTriggerIgnoresCondition(s32 flags, s32 result);
const struct TRIGGER *scFindTrigger(s32 kind, s32 target);
void fdOverlayCallInit(void);
const struct TRIGGER *scFindActorTalkTrigger(s32 id);
s32 scTalkToActor(s32 id);
s32 scRunRaisedTrigger(s32 value);
s32 scRunStepTriggerOrExit(s32 value);
s32 fdRunPushCell(s32 behavior);
s32 fdExamineCell(s32 tile);
s32 fdGetPsynergyTargetRange(s32 kind);
s32 fdFindNearbyTalkTarget(s32 id);
s32 fdFindNearbyPsynergyTarget(s32 id, s32 kind);
void fdActorSetLayersColMode(struct ACTOR *actor, s32 colorModePlusOne);
const struct TRIGGER *fdFindItemTrigger(s32 item);
s32 fdUseItem(s32 packedItem, s32 slot);
void *scFindPsynergyTrigger(s32 eventFlags, s32 group, s32 *target);
s32 fdRunPsynergyEndEvents(s32 packed);
s32 fdUsePsynergy(u32 code);

void fdActorSnapToTerrain(void *object);
void fdPlaceTreasureSpots(void);
s32 fdTreasureSpotFindSlot(s32 id);
void fdTreasureSpotReservedNoOp(void);
void fdTreasureSpotLaunchMimic(s32 id);

void fdItemDispEmitSparkles(struct ACTOR *actor);
struct ACTOR *fdItemDispShowItemInFront(s32 actorId, s32 item);
void fdActorReleaseIfPresent(void *object);
void fdItemDispRaiseAboveLeader(struct ACTOR *actor);

/* fdItemDispShowPicture's and fdItemDispShowItem's flags. */
typedef enum ITEM_DISPLAY_FLAG {
	ITEM_DISPLAY_SPARKLES = 1, /* sparkles appear round the picture */
	ITEM_DISPLAY_RAISED = 2	   /* the leader holds it up over his head */
} ITEM_DISPLAY_FLAG;

void fdRevealEmitSparkles(struct ACTOR *actor);
s32 fdIsRevealActive(struct ACTOR *interpreter);
void fdEffectSpawnRevealMarkers(void);

void fdScrTransitionStartWindowHBlankDma(void);
void fdScrTransitionUpdateScanlineTable(void);
void fdScrTransitionSuspendCallbacks(void);
struct SCR_TRANSITION_STATE *fdScrTransitionAllocateAndClearState(void);
void fdScrTransitionUpdate(void);
void fdScrTransitionUpdateFromCenter(void);
void fdScrTransitionUpdateScanline(void);
void fdScrTransitionUpdateDissolve(void);
void fdScrTransitionFillTilemapAndColTile(s32 color);

void fdPaletteFadeUpdate(void);
/* UB: event_commands.c calls fdPaletteFadeBuild with no declaration in */
/* scope, as a function returning int. UBFIX declares it. */
#ifdef UBFIX
void fdPaletteFadeBuild();
#endif
s32 fdPaletteFadeClampChannel(s32 value);
s32 fdPaletteFadeClampShiftedChannel(s32 value);
void fdGroundMaskDrawMarks(void);
const struct SPEAKER_DEFINITION *fdSpeakerFindDefinition(u32 id);
s32 fdSpeakerGetActorSpr(s32 id);
s32 fdSpeakerGetHaltedLines(s32 id);

/* Mind Read's caster's palette cycle, two frames a step */
/* (field_sequence_tables.c). */
extern const u8 MindRead_CasterPaletteCycle[];

/* The World Map cursor's angle for each +Control Pad direction, and its */
/* markers' blend cycle (field_sequence_tables.c). */
extern const u16 WorldMap_CursorDirectionAngles[];
extern const u8 WorldMap_MarkerBlendCycle[];

#endif
