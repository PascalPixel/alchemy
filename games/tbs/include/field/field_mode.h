#ifndef ALCHEMY_FIELD_MODE_H
#define ALCHEMY_FIELD_MODE_H

/* What the field mode's object (field_mode.c) defines for the code outside it:
   encounters, the field's actors, the leader's walking, the scene's cues,
   treasure spots, screen transitions and the palette fade. */

#include "types.h"
#include "fixed_math.h"
#include "field/field_particle.h"

struct SPEAKER_DEFINITION;
struct ACTOR;
struct SCREEN_TRANSITION_STATE;
struct CUE;
struct VECTOR3;
struct ACTOR_PLACE;
struct SPR;
struct WINDOW;

/* The exit request whose destination is already in MainRoutineStateWork.next
   and .entrance, so that gmTakeExit leaves them as they are. */
#define EXIT_KEEP_DESTINATION 999

/* MainRoutineStateWork.movementMode: how the field mode hands the party leader
   back to the player (fdModeRun). */
typedef enum LEADER_MOVEMENT_MODE {
	/* Walking: the pad moves the leader, on the World Map or a layered map. */
	LEADER_MOVEMENT_WALK = 0,
	LEADER_MOVEMENT_CLIMB = 1, /* on a climbable wall, moved 8 pixels a step */
	/* Carried through the air (Suhalla Desert's storms, the World Map's
	   airborne travel): only the configured keys are checked. */
	LEADER_MOVEMENT_AIRBORNE = 2
} LEADER_MOVEMENT_MODE;

/* A window transition's shape: SCENE_TRANSITION(TRANSITION_WINDOW, mode)'s low
   byte, which fdScreenTransitionUpdateScanlineTable draws. */
typedef enum WINDOW_TRANSITION_MODE {
	WINDOW_TRANSITION_BARS = 0,	  /* a bar on each line, at random */
	WINDOW_TRANSITION_SHAPE = 1,  /* a shape from ScreenTransitionShapes */
	WINDOW_TRANSITION_NOISE = 2,  /* ragged lines from the left edge */
	WINDOW_TRANSITION_CIRCLE = 3, /* a circle at the screen's centre */
	/* Alternate lines wiping in from the left and the right. */
	WINDOW_TRANSITION_INTERLACED_WIPE = 4,
	/* As WINDOW_TRANSITION_LEADER with inside and outside swapped and the
	   progress run the other way; it flickers. */
	WINDOW_TRANSITION_LEADER_INVERSE = 7,
	/* An ellipse around a point (the state's centre) whose outline ripples:
	   Reveal's. */
	WINDOW_TRANSITION_RIPPLE = 8,
	/* An ellipse wider than tall around the party leader, 16 pixels above its
	   feet. */
	WINDOW_TRANSITION_LEADER = 9,
	/* An ellipse a little taller than wide, 8 pixels above the leader's
	   feet. */
	WINDOW_TRANSITION_LEADER_LOW = 77
} WINDOW_TRANSITION_MODE;

/* The encounter table's zones (encounter_zones.c's entries) that code names:
   the World Map's, two to a region (its open ground, then its forest), and the
   rows no step on a map rolls. */
typedef enum ENCOUNTER_ZONE_ID {
	ENCOUNTER_ZONE_NONE = 0, /* no battles */
	ENCOUNTER_ZONE_WORLD_MAP_VALE_VAULT = 7,
	ENCOUNTER_ZONE_WORLD_MAP_VALE_VAULT_FOREST = 8,
	/* The script battles of Vale's stormy night (its monsters, and the
	   travelers who catch Isaac in Vale's plaza), Vault's thieves and the World
	   Map's Djinn tutorial battle. */
	ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT = 12,
	ENCOUNTER_ZONE_WORLD_MAP_BILIBIN_KOLIMA = 13,
	ENCOUNTER_ZONE_WORLD_MAP_BILIBIN_KOLIMA_FOREST = 14,
	ENCOUNTER_ZONE_WORLD_MAP_SNOWFIELD = 29,
	ENCOUNTER_ZONE_WORLD_MAP_SNOW_FOREST = 30,
	/* The script battles of Tret Tree (Tret) and Mercury Lighthouse (the Lizard
	   Man, then Saturos at the aerie). */
	ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY = 36,
	ENCOUNTER_ZONE_WORLD_MAP_FUCHIN = 37,
	ENCOUNTER_ZONE_WORLD_MAP_FUCHIN_FOREST = 38,
	ENCOUNTER_ZONE_WORLD_MAP_XIAN_ALTIN = 45,
	ENCOUNTER_ZONE_WORLD_MAP_XIAN_ALTIN_FOREST = 46,
	/* The script battles of Mogall Forest, Altin Peak and Lamakan Desert, whose
	   last two slots hold the battles with the Ant Lions and the Manticore. */
	ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN = 53,
	ENCOUNTER_ZONE_WORLD_MAP_KALAY = 54,
	ENCOUNTER_ZONE_WORLD_MAP_KALAY_FOREST = 55,
	/* The monster attacks on the Tolbi-bound ship, the Kraken's last. */
	ENCOUNTER_ZONE_TOLBI_SHIP = 62,
	/* The monsters behind the east doors of Crossbone Isle Cave's first four
	   floors, one slot a floor. */
	ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS = 63,
	ENCOUNTER_ZONE_WORLD_MAP_TOLBI = 64,
	ENCOUNTER_ZONE_WORLD_MAP_TOLBI_FOREST = 65,
	/* The battles that end the Colosso finals' matches, the slot chosen by how
	   the match went. */
	ENCOUNTER_ZONE_COLOSSO = 72,
	ENCOUNTER_ZONE_WORLD_MAP_SUHALLA = 80,
	ENCOUNTER_ZONE_WORLD_MAP_SUHALLA_FOREST = 81,
	/* The monsters behind the east doors of Crossbone Isle Cave's next five
	   floors, one slot a floor, then Deadbeard on the Pirate Ship. */
	ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS = 84,
	/* The script battles: Lunpa Fortress, Suhalla Desert, Venus Lighthouse
	   Aerie and the World Map; its first slot is the Aerie's second battle. */
	ENCOUNTER_ZONE_SCRIPTED = 98,
	ENCOUNTER_ZONE_MIMIC = 99, /* mimic chests, by the chest's slot */
	/* The Djinn battles, one zone for each element in element order. */
	ENCOUNTER_ZONE_VENUS_DJINN = 100,
	ENCOUNTER_ZONE_MERCURY_DJINN = 101,
	ENCOUNTER_ZONE_MARS_DJINN = 102,
	ENCOUNTER_ZONE_JUPITER_DJINN = 103
} ENCOUNTER_ZONE_ID;

/* The field-mode module's functions in the order of its stub table
   (src/field/mode_stubs.s). */
Sint32 fdModeRun(Sint32 entrance);
void   gmMainRoutineRun(Sint32 start);
void   fdScriptWait(Sint32 ticks);
void   fdScriptStart(void);
void   fdScriptEnd(void);
void   fdScriptRunActorUpdateAndWait(struct ACTOR_PLACE *tbl);
void   fdScriptAppendActorPlacements(struct ACTOR_PLACE *tbl);
void   fdAssignViewFlags(struct ACTOR_PLACE *entry);
void   fdScriptSetCues(const struct CUE *cues);
void   fdScriptAddPartyMember(Sint32 unit, Sint32 announce);
void   fdScriptAddPartyMemberPair(Sint32 first, Sint32 second);
void   fdPartyRemoveMemberRestored(Sint32 unit);
Sint32 fdPartyInventoryGiveItem(Sint32 item, Sint32 unused);
Sint32 fdInventoryTryAddAndReturnMember(
	Sint32 item, Sint32 unused, Sint32 unit);
Sint32 fdScriptAskYesNoWithActorAnimation(Sint32 actorId, Sint32 forceBottom);
void   fdPartyLearnAbility(Sint32 unit, Sint32 ability);
struct ACTOR *fdActorGetById(Sint32 actorId);
void		  fdActorReleaseById(Sint32 actorId);
void fdActorSetSpeedParms(Uint32 actorId, FIXED speedLimit, FIXED acceleration);
void fdActorSetMovementTypeById(Uint32 actorId, Sint32 movement);
void fdActorWaitForScriptEndById(Uint32 actorId);
void fdActorStop(Uint32 actorId);
void fdActorSetMovementTypeAndWaitById(Uint32 actorId, Sint32 movement);
void fdActorSetDestinationById(Uint32 actorId, Sint32 x, Sint32 z);
void fdActorMoveToAndWaitById(Uint32 actorId, Sint32 x, Sint32 z);
void fdActorWalkToById(Uint32 actorId, Sint32 x, Sint32 z);
void fdActorWalkToAndWaitById(Uint32 actorId, Sint32 x, Sint32 z);
void fdActorCenterOnColumn(Uint32 actorId, Sint32 pri, Sint32 zOfs);
void fdActorSetDestinationOffsetById(Uint32 actorId, Sint32 xOfs, Sint32 zOfs);
void fdActorWaitForMoveById(Uint32 actorId);
void fdActorSetHorizontalPosWithTerrain(Uint32 actorId, FIXED x, FIXED z);
void fdActorSetPosWithTerrain(Uint32 actorId, FIXED x, FIXED y, FIXED z);
void fdActorSetAnimationById(Uint32 actorId, Sint32 anm);
void fdActorSetAnimationSpeedById(Uint32 actorId, Sint32 steps);
void fdActorSetAnimationAndWait(Uint32 actorId, Sint32 anm);
void fdActorWaitForAnimationChange(Uint32 actorId);
void fdActorNoOperation(void);
void fdActorJump(Uint32 actorId, Sint32 speed, Sint32 ticks);
void fdActorStartRepeatedMotion(Uint32 actorId, Sint32 cnt);
void fdActorRunRepeatedMotion(Uint32 actorId, Sint32 cnt);
void fdActorJumpDownLedge(Sint32 actorId, Uint32 pose, Sint32 dustCol);
void fdActorFaceActorById(Sint32 firstId, Sint32 secondId, Sint32 ticks);
void fdActorTurnToFaceEachOther(Sint32 firstId, Sint32 secondId, Sint32 ticks);
void fdActorConfigureSprCol(Sint32 actorId, Sint32 col);
void fdActorSetSprCol(struct ACTOR *actor, Sint32 col);
void fdActorWatchAndStartScript(
	Uint32 actorId, Sint32 tgtId, const void *script);
void		   fdScriptSetMessageId(Sint32 msg);
struct WINDOW *fdMessageShowAtActor(Sint32 speaker, Sint32 unusedFlg);
void		   fdScriptShowMessageAndWait(Sint32 actorId, Sint32 flg);
void   fdScriptShowMessageThenWait(Sint32 actorId, Sint32 flg, Sint32 ticks);
Sint32 fdScriptAskYesNoAndShowResponse(Sint32 actorId, Sint32 flg);
void   fdScriptNoOperationCallback(void);
void   fdScriptShowSpeakerMessage(Sint32 speaker);
void   fdScriptForwardShowMessageAtPos(
	  Sint32 unusedA, Sint32 unusedB, Sint32 x, Sint32 y);
void fdScriptShowMessageAtPos(
	Sint32 unusedA, Sint32 unusedB, Sint32 x, Sint32 y);
void fdActorFaceDirectionById(Sint32 actorId, Sint32 ang, Sint32 ticks);
void fdActorClearDestinationAndStand(Uint32 actorId);
void fdActorClosePortrait(Sint32 actorId);
void fdScriptShowTwoMessagesAndWait(Sint32 first, Sint32 firstX, Sint32 firstY,
	Sint32 firstPortraitX, Sint32 firstPortraitY, Sint32 second, Sint32 secondX,
	Sint32 secondY, Sint32 secondPortraitX, Sint32 secondPortraitY);
void fdMessageSetVoiceFromActor(Sint32 actorId);
void fdActorSetSprPriority(Sint32 actorId, Sint32 pri);
void fdActorShowEmoteById(Sint32 actorId, Sint32 emote, Sint32 ticks);
void fdEffectSetAttached(Sint32 actorId, Sint32 flg);
void fdActorCopyGraphics(Uint32 tgtId, Uint32 srcId);
void fdCameraFollowActor(Sint32 actorId, Sint32 keepPos);
void fdCameraSetSpeedParms(FIXED speedLimit, FIXED acceleration);
void fdCameraMoveWithinMapBounds(
	FIXED requestedX, FIXED requestedY, FIXED requestedZ, Sint32 glide);
void		  fdCameraWaitForArrival(void);
void		  fdCameraMoveToActorById(Sint32 actorId, Sint32 glide);
struct ACTOR *fdCameraGetActor(void);
void		  fdMapGridSetCameraBounds(
			 Sint32 minX, Sint32 minY, Sint32 maxX, Sint32 maxY);
void   fdScriptSetDestination(Sint32 sceneId, Sint32 entrance);
void   fdScriptSetRestartPoint(Uint16 sceneId, Uint16 entrance);
void   fdScriptSetExitRequest(Uint16 request);
void   fdRequestBattleFromEncounterTable(Sint32 zone, Sint32 slot);
void   fdRequestDjinnBattle(Sint32 actorAndHop, Sint32 djinni);
void   fdScriptSetAfterBattleDestination(Sint32 sceneId, Sint32 entrance);
void   fdScriptSetDefeatDestination(Sint32 sceneId, Sint32 entrance);
void   fdScriptLeaveFor(Uint16 sceneId, Uint16 entrance);
Sint32 fdActorGetScreenPos(Sint32 actorId, Sint32 *pos);
Sint32 fdActorGetCueMetatileAheadOfCurrent(void);
Sint32 fdPartyGetCharacterSpr(Sint32 character);
void   fdPsynergySteerPushedTarget(Sint32 tgtId);
void   fdPsynergyStartCloak(void);
Sint32 fdTryJumpForward(void);
Sint32 fdClimbDown(void);
Sint32 fdClimbUp(void);
void   fdLeaderStopMoving(void);
void   fdWeatherStartRain(void);
void   fdWeatherStartLightning(void);
void   fdWeatherStopLightning(void);
void   fdWeatherLastLightning(void);
void   fdWeatherResumeLightning(void);
void   fdWeatherStartLightningFlash(Sint32 from, Sint32 to);
void   fdMotesStart(void);
void   fdEffectInitSparkles(void);
void   fdScreenTransitionStartHeldWindow(Sint32 mode);
void   fdScreenTransitionStartDissolveHeld(Sint32 mode);
void   fdScreenTransitionStart(Sint32 mode, Sint32 ticks);
void   fdScreenTransitionFinish(Sint32 mode, Sint32 ticks);
void   fdScreenTransitionHideBackgrounds(
	  Sint32 clearBg3, Sint32 clearBg2, Sint32 clearBg1);
void fdPaletteFadeInit(void);
void fdPaletteFadeSetTarget(Sint32 mode, Sint32 half);
void fdPaletteFadeSetSource(Sint32 mode, Sint32 half);
void fdPaletteFadeCalcSteps(
	Sint16 *src, Sint16 *tgt, Sint16 *step, Sint32 ticks);
void fdPaletteFadeStartInterpolation(Sint32 ticks);
void fdPaletteFadeRelease(void);
void fdPaletteFadeSetBackdrop(Uint32 col);
void fdScriptStartScreenTransition(void);
void fdScriptFinishScreenTransition(void);
void fdScriptWaitTransitionTicks(void);
/* Starts the ripple: for x and y, the wave's frequency, its step each tick and
   its amplitude. */
void   scLineScrollInitWave(Sint32 mode, FIXED freqX, Sint32 stepX, FIXED ampX,
	  FIXED freqY, Sint32 stepY, FIXED ampY);
void   fdLineScrollStop(void);
Sint32 fdChestOpenAndShowItem(Sint32 actorId, Sint32 tgt, Sint32 itemId);
void   fdItemDispShowPicture(void *object, Sint32 flg);
void   fdItemDispShowItem(Sint32 item, Sint32 flg);
struct ACTOR *fdTreasureSpotGetActor(Sint32 id);
void		  fdTreasureSpotOpenChest(Sint32 id);
void		  fdTreasureSpotCloseChest(Sint32 id);
void		  fdTreasureSpotShowOpened(Sint32 id);
void		  fdTreasureSpotSetActorPos(Sint32 id, FIXED x, FIXED z);
void		  fdTreasureSpotHide(Sint32 id);
void		  fdTreasureSpotMoveLeaderAway(void);
void		  fdWorldMapFreeze(void);
void		  fdWorldMapUnfreeze(void);
void		  fdActorResetForLeader(struct ACTOR_PLACE *tbl);
void   fdScriptSpawnActorPlacements(struct ACTOR_PLACE *entry, Sint32 slot);
void   fdActorReleaseAtIndex(Sint32 actorIdx);
void  *fdActorLookup(Uint32 actorIdx);
void   fdEffectStartGroundMask(void *object);
void   fdGroundMaskSuspend(void);
void   fdGroundMaskResume(void);
void   fdPsynergyRunHandler(Sint32 kind);
void   fdPsynergyLoadResources(Sint32 abilityId, Sint32 mode);
void   fdPsynergySetupActorPair(Sint32 userId, Sint32 tgtId);
void   fdPsynergyStopCasterUpdate(void);
void   fdPsynergyEnd(void);
void   fdPsynergyStartCast(void);
void   fdPsynergyFadeToCastCol(void);
void   fdPsynergyFadeFromCastCol(void);
void   fdEndBusyAndReveal(void);
void   fdPsynergyStartMindReadWave(Sint32 casterId, Sint32 tgtId);
void   fdPsynergyStopMindReadWave(void);
void   fdPsynergyGreyMindReadPalette(void);
void   fdPsynergyRestoreMindReadPalette(void);
Bool   fdHasPsynergyCue(Sint32 group);
Uint32 fdHasItemCue(Uint16 item);
struct SPR *fdSprShareVRAMCacheEntry(struct SPR *srcSpr, struct SPR *sharedSpr);
Sint16		fdBattleBackgroundGetSelected(void);
struct ACTOR *fdEffectSpawnMoveHand(struct ACTOR *caster);
void		  fdEffectGrowHandAndWait(struct ACTOR *actor);
void		  fdEffectBurstHand(struct ACTOR *hand);
void		  fdPartyAddPp(Sint32 amount, Sint32 unused);
void		  fdPartyAddHp(Sint32 amount, Sint32 percent);
Sint32		  fdVRAMCacheCountFreeBlocks(void);
Sint32		  fdYesNoAsk(Sint32 atBottom);
Uint16		  fdEncounterGetFormationAtSlot(Sint32 zone, Sint32 slot);
Sint16		  fdEncounterGetDjinnFormation(Sint32 djinni);
void		  gmPlayMusic(void);
void		  fdDjinnJoinParty(Sint32 actorId, Sint32 element, Sint32 djinn);
void		  fdParticleUpdate(FIELD_PARTICLE *effect);
Uint32		  fdParticleHasPendingTarget(FIELD_PARTICLE *effect);
void		  fdParticleSetPos(FIELD_PARTICLE *effect, Sint32 x, Sint32 z);
void		  fdParticleSetAnimation(FIELD_PARTICLE *effect, Sint32 mode);
void   fdParticleSetCallback(FIELD_PARTICLE *effect, EFFECT_CALLBACK func);
void   fdParticleInit(FIELD_PARTICLE *effect, Sint32 kind, Sint32 x, FIXED z);
void   fdEffectClearOwnedSlot(FIELD_PARTICLE *effect);
void   fdCameraWorldToScreen(struct VECTOR3 *pos);
Sint32 fdSpeakerGetVoice(Uint32 sprId);
void   fdPartyClearUnusableShortcuts(void);
void   fdEffectInitSlots(void);
void   fdEffectReleaseSlots(void);
void   fdActorClearOutOfBounds(void);
void   fdActorRespawn(void);
void   fdActorLaunchFromLeader(
	  Uint32 actorId, Sint32 xOfs, Sint32 zOfs, Sint32 facing);
void   fdActorWalkByById(Uint32 actorId, Sint32 xOfs, Sint32 zOfs);
void   fdActorWalkByAndWaitById(Uint32 actorId, Sint32 xOfs, Sint32 zOfs);
void   fdActorWatchAndSetScript(Uint32 actorId, Uint32 watchedActorId);
void   fdPsynergyHaltActor(Sint32 actorId);
void   fdPsynergyResumeHaltedActor(Sint32 actorId);
Sint32 fdActorFindBySpr(Sint32 spr);
Sint32 fdActorGetSprById(Sint32 actorId);
void   fdActorSnapshot(void);
void   fdActorRestore(void);
void   fdWorldMapStartZoom(FIXED zoom, Sint32 ticks);
void   fdScriptWaitForZoomTransition(void);
Sint32 fdAreaGetNameIndex(Sint32 sceneId, Sint32 entrance);
void   fdActorSetMovementType(struct ACTOR *actor, Sint32 movement);
void   fdQueueMusic(Sint32 sndId);
void   fdPlayQueuedMusic(void);
Sint32 fdPartyGetCharacterSprForChildhood(Sint32 character, Sint32 childhood);
#if FIELD_MENU_STOPS_REVEAL_SPARKLES
void fdEffectReleaseRevealSparkles(void);
#endif

void   fdEncounterSelectMapZones(Sint32 useRules);
Sint32 fdEncounterResolveWorldMapZoneAndBackdrop(void *pos);
Sint32 fdEncounterCountStepsAndSelectFormation(Sint32 zone, FIXED steps);
Sint32 fdEncounterRollMapStep(Uint32 terrain, FIXED steps);
Sint32 fdEncounterRollWorldMap(Sint32 pos, FIXED steps);
void   fdChooseMusic(void);
void   fdSetRestartPoint(void);
void   fdBattleBackgroundSelectForMap(void);
void   fdBattleBackgroundSelectForEnvironment(Sint32 behavior);
/* The field mode calls fdEncounterSelectBattleMusic without a prototype. */
#if FIX_FUNCTION_SIGNATURES
void fdEncounterSelectBattleMusic(Sint32 zone, Sint32 slot);
#endif
void fdEffectShrinkActorAndDestroySlow(struct ACTOR *actor);
void fdEffectShrinkActorAndDestroyFast(struct ACTOR *actor);
void fdPsynergyLift(void);
void fdPsynergyCarry(void);

/* A cue result below it is a message number, and from it on the address of the
   function to run: every function lies above it, so one word holds either. */
#define CUE_RESULT_MESSAGE_END 65536

/* These masks decode the cue and search-result words whose bit layouts and
   values scene_tables.h gives (CUE, CUE_KIND, FIND_). */

/* A cue's control bits 3..0, its kind (CUE_KIND). */
#define CUE_KIND_MASK		0xf
/* A Psynergy cue's control bits 30..28, its phase (PSYNERGY_PHASE). */
#define PSYNERGY_PHASE_MASK (7u << 28)
/* An examine cue's control bits 8..0, its kind and what it is on, as
   EXAMINE_CUE() packs them. */
#define EXAMINE_CUE_MASK	0x1ff
/* A cue's control bits 15..12: the heading CUE_FLAG_FACING compares, in
   sixteenths of a turn. */
#define CUE_FACING_MASK		0xf000

/* A search result's type, its top twelve bits (FIND_ITEM to FIND_MIMIC). */
#define FIND_TYPE_MASK		 0xfff00000
/* Bits 27..24 of a search result, set only in a function's address: the result
   is then a function to call. */
#define FIND_FUNCTION_BITS	 0x0f000000
/* Bits 19..16 of a chest's search result, which fdExamineMetatile compares only
   with FIND_CHEST_LOCKED. No chest of the game sets them, so none is locked. */
#define FIND_CHEST_LOCK_MASK 0xf0000
/* A chest that stays shut while a party member (up to ALLY_MAX) leads: "can't
   open it..." (MsgCantOpenIt). */
#define FIND_CHEST_LOCKED	 0x10000

/* A field Psynergy or item use, packed into one value, as FIELD_STATE.psynergy
   and FIELD_STATE.item hold it. bits: 13..10 the user (13 is ACT_PACKED) | 9..0
   the Psynergy or item. */
#define ACT_ID_MASK	   0x3ff			   /* bits 9..0 */
#define ACT_UNIT_SHIFT 10				   /* bits 13..10 */
#define ABILITY_ID(c)  ((c) & ACT_ID_MASK) /* the Psynergy */
#define ACT_ITEM_ID(c) ((c) & ACT_ID_MASK)
#define ACT_UNIT(c)	   (((c) >> ACT_UNIT_SHIFT) & 15) /* who uses it */
#define ACT_PACKED	   0x2000 /* use it without paying PP */
/* The Psynergy request that ends Cloak, without paying PP. */
#define CLOAK_END	   (ACT_PACKED | ABILITY_CLOAK)

/* A 16.16 coordinate's whole metatile: a map grid metatile is 16 pixels
   square. */
#define FIELD_METATILE_MASK		   0xfff00000
/* A 16.16 coordinate's shift to its map grid metatile: 16 bits of fraction and
   4 of the metatile's 16 pixels. */
#define FIELD_METATILE_SHIFT	   20
/* The map grid metatile a 16.16 coordinate lies in, counted from 0 along its
   axis. */
#define FIELD_METATILE(coordinate) ((coordinate) >> FIELD_METATILE_SHIFT)
/* The half metatile, 8 pixels, a 16.16 coordinate lies in, counted from 0 along
   its axis. */
#define FIELD_HALF_METATILE(coordinate) \
	((coordinate) >> (FIELD_METATILE_SHIFT - 1))
/* The 16.16 coordinate of a half metatile's first pixel, FIELD_HALF_METATILE
   undone. */
#define FIELD_HALF_METATILE_ORIGIN(halfMetatile) \
	((halfMetatile) << (FIELD_METATILE_SHIFT - 1))
/* A 16.16 coordinate's whole pixels, its fraction cleared. */
#define FIELD_PIXEL_MASK				0xffff0000
/* The 16.16 coordinate of a map grid metatile's first pixel, FIELD_METATILE
   undone. */
#define FIELD_METATILE_ORIGIN(metatile) ((metatile) << FIELD_METATILE_SHIFT)
/* The 16.16 coordinate of the middle of a map grid metatile. */
#define FIELD_METATILE_CENTER(metatile) \
	(FIELD_METATILE_ORIGIN(metatile) + toFIXED(8))

/* The ticks the leader pushes against a step cue, a ledge, a gap or a wall
   before it runs, jumps or climbs (FIELD_STATE.blockedSteps). */
#define FIELD_PUSH_TICKS 12

/* --------------------------------------------------------------------------
   The map's treasure spots (chests and Psynergy Stones)
   --------------------------------------------------------------------------
   The most treasure spots a map places. */
#define TREASURE_SPOT_MAX	   10
/* The ids a treasure spot's cue answers to; the map's list of spots ends at
   column and row 255. */
#define TREASURE_SPOT_FIRST_ID 100
#define TREASURE_SPOT_LAST_ID  239
#define TREASURE_SPOT_LIST_END 255

Sint32 fdActorFindLastActiveId(void);
void  *fdActorGetSlotAddress(Uint32 actorIdx);
void   fdActorClearAll(void);

void   fdClearRequests(void);
Sint32 fdCountRequests(void);
Sint32 fdGetCollisionAheadOfLeader(void);
void   fdCheckExitRegions(FIXED posX, FIXED posY, FIXED posZ);
void   fdUpdateWalking(Sint32 layer, FIXED x, FIXED y, FIXED z);
Sint32 fdPartyApplyPoisonDamage(void);
void   fdApplyEntrance(void);
void   fdPaletteEditorRun(void);

struct ACTOR_PLACE *fdFindActorPlacement(Sint32 id);
Sint32				fdGameFlagIsConditionActive(Sint32 condition);
Uint32				fdExamineCueIgnoresCondition(Sint32 flg, Sint32 result);
const struct CUE   *fdFindCue(Sint32 kind, Sint32 tgt);
void				fdOverlayCallInit(void);
const struct CUE   *fdFindActorTalkCue(Sint32 id);
Sint32				fdTalkToActor(Sint32 id);
Sint32				fdRunRaisedCue(Sint32 value);
Sint32				fdRunStepCueOrExit(Sint32 value);
Sint32				fdRunPushMetatile(Sint32 behavior);
Sint32				fdExamineMetatile(Sint32 tile);
Sint32				fdGetPsynergyTargetRange(Sint32 kind);
Sint32				fdFindNearbyTalkTarget(Sint32 id);
Sint32				fdFindNearbyPsynergyTarget(Sint32 id, Sint32 kind);
void fdActorSetLayersColMode(struct ACTOR *actor, Sint32 colModePlusOne);
const struct CUE *fdFindItemCue(Sint32 item);
Sint32			  fdUseItem(Sint32 packedItem, Sint32 slot);
void			 *fdFindPsynergyCue(Sint32 eventFlg, Sint32 group, Sint32 *tgt);
Sint32			  fdRunPsynergyEndScripts(Sint32 packed);
Sint32			  fdUsePsynergy(Uint32 code);

void   fdActorSnapToTerrain(void *object);
void   fdPlaceTreasureSpots(void);
Sint32 fdTreasureSpotFindSlot(Sint32 id);
void   fdTreasureSpotReservedNoOperation(void);
void   fdTreasureSpotLaunchMimic(Sint32 id);

void		  fdItemDispEmitSparkles(struct ACTOR *actor);
struct ACTOR *fdItemDispShowItemInFront(Sint32 actorId, Sint32 item);
void		  fdActorReleaseIfPresent(void *object);
void		  fdItemDispRaiseAboveLeader(struct ACTOR *actor);

/* fdItemDispShowPicture's and fdItemDispShowItem's flags. */
typedef enum ITEM_DISPLAY_FLAG {
	ITEM_DISPLAY_SPARKLES = 1, /* sparkles appear round the picture */
	ITEM_DISPLAY_RAISED = 2	   /* the leader holds it up over his head */
} ITEM_DISPLAY_FLAG;

void fdRevealEmitSparkles(struct ACTOR *actor);
Bool fdIsRevealActive(struct ACTOR *interpreter);
void fdEffectSpawnRevealMarkers(void);

void							fdScreenTransitionStartWindowHBlankDMA(void);
void							fdScreenTransitionUpdateScanlineTable(void);
void							fdScreenTransitionSuspendCallbacks(void);
struct SCREEN_TRANSITION_STATE *fdScreenTransitionAllocateAndClearState(void);
void							fdScreenTransitionUpdate(void);
void							fdScreenTransitionUpdateFromCenter(void);
void							fdScreenTransitionUpdateScanline(void);
void							fdScreenTransitionUpdateDissolve(void);
void fdScreenTransitionFillTilemapAndColTile(Sint32 col);

void fdPaletteFadeUpdate(void);
/* UB: script_commands.c calls fdPaletteFadeBuild with no declaration in scope,
   as a function returning int. UBFIX declares it. */
#if FIX_FUNCTION_SIGNATURES
void fdPaletteFadeBuild();
#endif
Sint32							 fdPaletteFadeClampChannel(Sint32 value);
Sint32							 fdPaletteFadeClampShiftedChannel(Sint32 value);
void							 fdGroundMaskDrawMarks(void);
const struct SPEAKER_DEFINITION *fdSpeakerFindDefinition(Uint32 id);
Sint32							 fdSpeakerGetActorSpr(Sint32 id);
Sint32							 fdSpeakerGetHaltedLines(Sint32 id);

/* Mind Read's caster's palette cycle, two ticks a step
   (field_mode_tables.c). */
extern const Uint8 MindReadCasterPaletteCycle[];

/* The World Map cursor's angle for each +Control Pad direction, and its
   markers' blend cycle (field_mode_tables.c). */
extern const Uint16 WorldMapCursorDirectionAngles[];
extern const Uint8	WorldMapMarkerBlendCycle[];

#endif
