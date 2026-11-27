#ifndef ALCHEMY_SCRIPT_COMMANDS_H
#define ALCHEMY_SCRIPT_COMMANDS_H

/* What script_commands.c declares for the files that use it: the commands
   scripts run, moving on the field, weather and line scrolls, Djinn joining the
   party, each field Psynergy and its effects, and the map screen. */

#include "fixed_math.h"
#include "types.h"
#include "field/field_particle.h"
#include "field/field_mode.h"

struct SPR;
struct CUE;
struct ACTOR_PLACE;
struct ACTOR;

/* A step of the current field Psynergy (fdPsynergyRunHandler). */
typedef enum FIELD_PSYNERGY_HANDLER {
	/* Casts it with no target (fdPsynergyRun). */
	FIELD_PSYNERGY_HANDLER_UNTARGETED = 0,
	/* Casts it on its target, secondActorId (fdPsynergyDispatch). */
	FIELD_PSYNERGY_HANDLER_ON_TARGET = 1,
	FIELD_PSYNERGY_HANDLER_FINISH = 2,
	FIELD_PSYNERGY_HANDLER_EXTRA = 3 /* Runs its kind's extra handler. */
} FIELD_PSYNERGY_HANDLER;

/* A field Psynergy, as the field kind: the type column of
   games/tbs/src/game/ability_table.c, named for the abilities that have it. */
typedef enum FIELD_PSY {
	FIELD_PSYNERGY_MOVE = 1,
	FIELD_PSYNERGY_MIND_READ = 2,
	/* Ply, Ply Well and Pure Ply. */
	FIELD_PSYNERGY_PLY = 3,
	FIELD_PSYNERGY_FORCE = 4,
	FIELD_PSYNERGY_DOUSE = 5,
	FIELD_PSYNERGY_FROST = 6,
	FIELD_PSYNERGY_LIFT = 7,
	FIELD_PSYNERGY_REVEAL = 8,
	FIELD_PSYNERGY_HALT = 9,
	FIELD_PSYNERGY_CLOAK = 10,
	FIELD_PSYNERGY_CARRY = 11,
	FIELD_PSYNERGY_GROWTH = 12,
	FIELD_PSYNERGY_CATCH = 13,
	/* Whirlwind and Gale. */
	FIELD_PSYNERGY_WHIRLWIND = 14,
	FIELD_PSYNERGY_RETREAT = 15,
	FIELD_PSYNERGY_AVOID = 16
} FIELD_PSY;

/* One picture of a sparkle: its height offset and tile
   (field_mode_tables.c). */
typedef struct SPARKLE_PICTURE {
	Sint16 dy;
	Uint16 tile;
} SPARKLE_PICTURE;

extern const SPARKLE_PICTURE FieldEffectSparklePictures[32];

void   fdScriptUpdateDebugSkip(void);
void   fdScriptNoOperationUpdateA(void);
void   fdScriptNoOperationUpdate(void);
Sint32 fdPartyCheckShortcut(Uint32 shortcut);
Sint32 fdPartyCheckItemLimit(Sint32 item);
void   fdInventoryNoOperationCallback(void);
void   fdInventoryReservedNoOperation(void);
void   fdActorWaitForAnimationEnd(Sint32 actorId, Sint32 animation);
/* fdRequestDjinnBattle's actorAndHop: the Djinni does not hop first. */
#define DJINN_NO_HOP 0x800

void fdEffectUpdateDustPuff(struct ACTOR *actor);
void fdEffectSpawnDustPuff(struct ACTOR *src, Sint32 color);
void fdActorTurnPairToFaceEachOther(struct ACTOR *first, struct ACTOR *second);
void fdActorReleaseNoOperation(void);
void fdActorUpdateBlink(struct ACTOR *actor);
Sint32 fdActorStepAngle(struct ACTOR *actor);
void   fdActorReservedNoOperation(void);

struct WINDOW;

/* A camera move's axis that stays where the camera is. */
#define CAMERA_KEEP (-1)
void		  fdCameraFollow(struct ACTOR *target, Sint32 keepCurrentPos);
UNREAD_RESULT fdWorldMapStepZoom(void);
UNREAD_RESULT fdEffectCopyWatchedActorPos(struct ACTOR *actor);
/* fdActorShowEmoteById's emote flag: the bubble takes sprite priority 1 instead
   of its actor's. The low four bits pick the bubble (enum Emote). */
#define EMOTE_IN_FRONT 0x100

/* The bubbles fdActorShowEmoteById can show, by the animation they pick in the
   bubble's sprite sheet (EMOTE). */
typedef enum EMOTE {
	EMOTE_EXCLAMATION = 0, /* "!" */
	EMOTE_QUESTION = 1,	   /* "?" */
	/* A wincing face in English; a sweat drop in Japanese and a frown in
	   Italian. */
	EMOTE_WINCE = 2,
	EMOTE_ANGER_VEIN = 3, /* the cross-shaped vein of anger */
	EMOTE_HEART = 4,	  /* a red heart */
	EMOTE_ELLIPSIS = 5,	  /* "..." */
	EMOTE_LIGHT_BULB = 6, /* a lit bulb, with SE_CHIME */
	EMOTE_SCOWL = 7,	  /* a scowling face */
	EMOTE_SMILE = 8,	  /* a smiling face */
	/* A sweat drop; no scene shows it, and the Japanese sprite sheet has no
	   tenth bubble. */
	EMOTE_SWEAT_DROP = 9
} EMOTE;

/* fdEffectSetAttached's flags: a mode in the low two bits, and
   ATTACHED_EFFECT_IN_FRONT. */
typedef enum ATTACHED_EFFECT_FLAG {
	ATTACHED_EFFECT_REMOVE = 0, /* removes the attached actor */
	ATTACHED_EFFECT_ATTACH = 1, /* attaches one that stays until removed */
	ATTACHED_EFFECT_ONCE = 2,	/* shows a one-off one that ends by itself */
	/* The attached actor takes sprite priority 1, not the actor's. */
	ATTACHED_EFFECT_IN_FRONT = 0x100
} ATTACHED_EFFECT_FLAG;

Sint32		  fdActorClearSpriteAndCollisionFlags(struct ACTOR *actor);
Sint32		  fdActorMoveTowardTarget(struct ACTOR *actor);
Sint32		  fdActorTurnTowardWatchedActor(struct ACTOR *actor);
struct ACTOR *fdActorFindNearestFacingTarget(struct ACTOR *self, Sint32 sprite);
void		  fdExitNoOperation(void);
UNREAD_RESULT fdStartEncounterTransition(Sint32 unused);
void		  fdExitPrepareAnimation(Sint32 animation);
void		  fdExitStartStairsUp(void);
void		  fdExitStartStairsDown(void);
void		  fdEntrancePlayArrivalAnimation(Sint32 animation);
Sint32		  fdEntrancePlayArrival(void);

void fdLineScrollArmDMA(void);
void fdLineScrollBuildPage(void);
void fdWeatherUpdateRain(void);
void fdWeatherUpdateLightning(void);
void fdMotesUpdate(void);
void fdEffectUpdateSparkles(void);

struct FIELD_PARTICLE;

void fdActorMoveHalfwayTowardTarget(struct ACTOR *actor);
void fdDjinnRunBattleStartHop(Sint32 djinniId);
void fdDjinnRestoreActorAfterBattle(void);
/* Declared without a prototype: fdModeRun calls it with one argument and with
   two. */
void fdDjinnResolvePending();
void fdEffectUpdateAllSlots(void);
void fdEffectAdvanceSpinAngle(struct ACTOR *actor);
void fdEffectRunAngledApproachPhases(FIELD_PARTICLE *particle);
void fdDjinnRunJupiterJoinEffect(Sint32 djinniId);
void fdEffectPulseActorScale(struct ACTOR *actor);
void fdEffectAdvanceAngleWithRamp(struct ACTOR *actor);
void fdEffectUpdateMarsSpark(FIELD_PARTICLE *effect);
void fdDjinnRunMarsJoinEffect(Sint32 djinniId);
void fdEffectUpdateDescendingOrbitActor(struct ACTOR *actor);
void fdEffectUpdateRadialSpread(FIELD_PARTICLE *effect);
void fdDjinnRunVenusJoinEffect(Sint32 djinniId);
void fdEffectUpdateRandomTargetParticle(FIELD_PARTICLE *effect);
void fdEffectHalveDistanceToTarget(struct ACTOR *actor);
void fdDjinnRunMercuryJoinEffect(Sint32 djinniId);

struct VECTOR3;

void   fdPsynergyRun(void);
void   fdPsynergyDispatch(void);
void   fdPsynergyFinishMindRead(void);
void   fdPsynergyRunExtraHandler(void);
Sint32 fdRunPsynergyScript(const void *event, Sint32 param, Sint32 target);
void   fdActorCycleSpriteColors(struct ACTOR *actor);
struct ACTOR *fdActorSpawn(Sint32 kind, Sint32 x, Sint32 y, Sint32 z);
void		  fdActorSetOthersPaused(
			 struct ACTOR *except, Sint32 paused, Sint32 animationSpeed);
void fdEffectUpdateScaledArcActorA(struct ACTOR *actor);
void fdEffectUpdateScaledArcActorB(struct ACTOR *actor);
void fdEffectSpawnScaledArcActors(struct ACTOR *watched);
void fdEffectFlickerActorAndSpawnArcs(struct ACTOR *actor);
void fdEffectCycleActorAndSpawnArcs(struct ACTOR *actor);
void fdEffectUpdateAllEffectSlots(void);

void   fdPsynergyUpdateMindReadWave(void);
void   fdMindReadStartRipple(void);
void   fdMindReadAdvanceTint(void);
void   fdEffectCalculateHueChannels(FIXED hue, FIXED *r, FIXED *g, FIXED *b);
Sint32 fdEffectHueChannelRamp(Sint32 angle, Sint32 low, FIXED high);
FIXED  fdFixedPointRemainder(FIXED value, FIXED divisor);
void   fdMindReadUpdateCrescent(struct ACTOR *actor);
/* What fdInputGetDpadAngle returns when no direction, or a diagonal, is held:
   the blank entries of InputDpadAngles. */
#define DPAD_ANGLE_NONE 65535
Sint32		  fdInputGetDpadAngle(Uint32 unused);
void		  fdEffectUpdateMoveHand(struct ACTOR *actor);
void		  fdEffectUpdateOutAndBack(FIELD_PARTICLE *effect);
void		  fdPsynergyMove(void);
void		  fdActorSetRevealSpriteColor(Sint32 value);
void		  fdPsynergyWatchLeaderDistance(void);
void		  fdPsynergyReveal(void);
void		  fdPsynergyEndReveal(void);
void		  fdPsynergyGrowthOnTarget(Sint32 unused);
void		  fdPsynergyGrowth(void);
void		  fdEffectSpawnBurstParticleField(void);
void		  fdPsynergyLiftOnTarget(Sint32 unused);
struct ACTOR *fdEffectSpawnLiftHand(Sint32 x, Sint32 y, Sint32 z, Sint32 angle);
void		  fdEffectUpdateRadialBurst(FIELD_PARTICLE *effect);
Sint32		  fdEffectSpawnHeavyImpact(struct ACTOR *interpreter);
void		  fdPsynergyForceOnTarget(Sint32 unused);
void		  fdPsynergyForce(void);
void		  fdEffectUpdateOrbitingParticleFade(struct ACTOR *actor);
void		  fdEffectUpdateOrbitingParticleLeft(struct ACTOR *actor);
void		  fdEffectUpdateOrbitingParticleRight(struct ACTOR *actor);
void		  fdPsynergyFrostOnTarget(Sint32 unused);
void		  fdPsynergyFrost(void);
void		  fdEffectSwayParticle(struct ACTOR *actor);
void		  fdEffectUpdateShrinkingOrbitActor(struct ACTOR *actor);
void		  fdEffectRunSparkEmitter(struct ACTOR *actor);
void		  fdPsynergyPlyOnTarget(Sint32 unused);
void		  fdPsynergyPly(void);
void		  fdPsynergyCheckCloakTile(void);
void		  fdPsynergyEndCloak(void);
void		  fdPsynergyResumeCloakCheck(void);
void		  fdEffectUpdateDouseDrop(struct ACTOR *drop);
void		  fdEffectUpdateDriftingFallActor(struct ACTOR *actor);
void		  fdPsynergyDouseOnTarget(Sint32 unused);
void		  fdPsynergyDouse(void);
void		  fdEffectSpawnFallingParticles(void);
void		  fdPsynergySteerLiftedTarget(Sint32 targetId);
struct ACTOR *fdEffectSpawnCarryHand(
	Sint32 x, Sint32 y, Sint32 z, Sint32 angle);
void   fdEffectSpawnRadialParticleRing(struct ACTOR *origin);
void   fdEffectWanderAroundAnchor(struct ACTOR *actor);
void   fdEffectCircleAnchor(struct ACTOR *actor);
void   fdWhirlwindUpdateTarget(struct ACTOR *actor);
void   fdPsynergyWhirlwindOnTarget(Sint32 unused);
void   fdPsynergyWhirlwind(void);
void   fdEffectUpdateHaltParticle(FIELD_PARTICLE *effect);
void   fdPsynergyHaltOnTarget(Sint32 unused);
void   fdPsynergyHalt(void);
void   fdPsynergyUpdateHaltedActor(struct ACTOR *actor);
Sint32 fdPsynergyFilterHaltTarget(Sint32 id);
void   fdPsynergyCatchOnTarget(Sint32 unused);
void   fdPsynergyCatch(void);
void   fdEffectUpdateSpinAngle(struct ACTOR *actor);
void   fdEffectUpdateRetreatCaster(struct ACTOR *actor);
void   fdRetreatUpdateSpark(FIELD_PARTICLE *effect);
void   fdPsynergyRetreat(void);
void   fdEffectUpdateDescendingParticlePositiveArc(struct ACTOR *actor);
void   fdEffectUpdateDescendingParticleNegativeArc(struct ACTOR *actor);
void   fdEffectSpawnDescendingArcParticles(struct ACTOR *src);
void   fdEffectBlinkAvoidCaster(void);
void   fdEffectUpdateAvoidCaster(struct ACTOR *actor);
void   fdPsynergyShowAvoidWoreOff(void);
void   fdPsynergyAvoid(void);

void fdEffectDrawScaledActor(FIELD_PARTICLE *object);
void fdParticleUpdateMotion(FIELD_PARTICLE *effect);

/* The map screen's marker tiles: the party's dot, the two kinds of area a
   MAP_SCREEN_MARKER row (AREA_MARKER or LIGHTHOUSE_MARKER) asks for (a town or
   a place on the way; a lighthouse or Sol Sanctum), and the larger dot that
   blinks over the marker nearest the cursor. */
typedef enum MAP_SCREEN_MARKER_TILE {
	MAP_SCREEN_MARKER_PARTY,
	MAP_SCREEN_MARKER_AREA,
	MAP_SCREEN_MARKER_LIGHTHOUSE,
	MAP_SCREEN_MARKER_NEAREST
} MAP_SCREEN_MARKER_TILE;

/* The World Map's area markers, {tile << 16 | flag, area} for each area's actor
   id from 11, ended by -1; a zero row reserves an actor id with no marker. */
extern const Sint32 MapScreenAreaMarkers[];

void fdMapScreenInit(void);
void fdMapScreenRelease(void);
void fdMapScreenUpdateMarkers(void);
void fdMapScreenShow(Sint32 image);
void fdUpdateActorVisibilityBounds(void);
void fdWorldMapZoomOutWhileLHeld(void);

/* The message voice's rate by message speed option: 1, 3, then 10
   (fdMessageSetVoiceFromActor). The same values as MessageSpeedVoiceGaps,
   which the message reads, in a copy of its own. */
extern const Uint8 MessageSpeedVoiceRates[];

#endif
