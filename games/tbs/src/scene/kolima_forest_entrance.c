/* Kolima Forest (SceneIdKolimaForestEntrance), the first of the forest's five
   scenes, entered from the World Map (KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP).

   If the party comes here before it has been to Kolima, the curse's script
   plays here instead of in the village
   (scKolimaForestEntranceWitnessForestCurse, until
   FLAG_KOLIMA_CURSE_WITNESSED). Sparkles of Tret's glamour rain down, fields of
   the party's own Psynergy shield it, and Tret and Laurel, unseen, speak of the
   dying forest. Five sparkles lie on the ground until Tret lifts the curse
   (FLAG_KOLIMA_CURSE_LIFTED).

   The fields flicker. The leader's and Mia's show on even ticks and Ivan's and
   Garet's on odd ones, each lifted out of sight on the others
   (scKolimaForestEntranceFlickerPsynergyFields). A sparkle bursts where it
   lands only inside the boxes scKolimaForestEntranceBurstOnPsynergyField tests,
   two with Mia in the party and three without her.

   Three logs lie here, rolled by walking into them. The one pushed west to the
   water's edge tips into the river and floats there, metatiles the party can
   cross. From the bank the leader can hop east (step cue 11,
   scKolimaForestHopEast).

   KOLIMA_FOREST_ENTRANCE_TO_KOLIMA_FOREST_GLADE leads on into the forest
   (SceneIdKolimaForestGlade), and KOLIMA_FOREST_ENTRANCE_TO_WORLD_MAP back to
   the World Map.

   FLAG_KOLIMA_FOREST_ENTRANCE_LOG_IN_RIVER: the log is in the river. Like every
   scene temp flag it is cleared when the party next comes in, so the log is
   back on the bank, but a battle keeps it. */

#include "flags.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script.h"
#include "graphics/graphics.h"
#include "scene/kolima.h"
#include "scene/kolima_curse_dust.h"
#include "text/msg_ids.h"

extern Sint32		  KolimaForestEntranceGaretAction[];
extern Sint32		  KolimaForestEntranceIvanAction[];
extern Sint32		  KolimaForestEntranceMiaAction[];
extern Sint32		  KolimaForestEntrancePsynergyFieldGrowAction[];
extern Sint32		  KolimaForestEntrancePsynergyFieldShrinkAction[];
extern Sint32		  KolimaForestEntranceGroundSparkleAction[];
extern SCENE_ENTRANCE KolimaForestEntranceEntrances[];
extern Uint32		  KolimaForestEntranceExits[];
extern ACTOR_PLACE	  KolimaForestEntranceActorPlacements[];
extern CUE			  KolimaForestEntranceCues[];
extern Sint32		  KolimaForestEntranceSparkleScript[];

/* The scene's actors after Garet's, Ivan's and Mia's party slots, numbering the
   rows of KolimaForestEntranceActorPlacements from ACTOR_FIRST_PLACED. */
enum {
	/* The three logs. The last is the one that can reach the river. */
	ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
	ACTOR_RIVER_LOG = ACTOR_FIRST_LOG + 2,
	/* Tret's wrathful side, unseen while the curse falls. */
	ACTOR_TRET,
	/* Unseen while the curse falls. */
	ACTOR_LAUREL,
	/* The fields of Psynergy, one over each party member. Mia's only with
	   Mia. */
	ACTOR_LEADER_PSYNERGY_FIELD,
	ACTOR_IVAN_PSYNERGY_FIELD,
	ACTOR_MIA_PSYNERGY_FIELD,
	ACTOR_GARET_PSYNERGY_FIELD,
	/* The sparkles of Tret's glamour on the ground, gone once the curse is
	   lifted. */
	ACTOR_FIRST_GROUND_SPARKLE,
	ACTOR_LAST_GROUND_SPARKLE = ACTOR_FIRST_GROUND_SPARKLE + 4
};

static void scKolimaForestEntranceWitnessForestCurse(void);
static void scKolimaForestEntranceShowMessageAndWait(
	Sint32 speaker, Sint32 ticks);
static void scKolimaForestEntranceFaceActorAndWait(
	Sint32 actorId, Sint32 facing, Sint32 waitTicks);
static void scKolimaForestEntranceSpawnCurseSparkle(void);
static void scKolimaForestEntranceFlickerPsynergyFields(void);

#include "shared/pushed_actor.c"

/* The river log has tipped into the river and floats there as ground the party
   can cross. */
#define FLAG_KOLIMA_FOREST_ENTRANCE_LOG_IN_RIVER FLAG_SCENE_TEMP_00

/* The column at the water's edge, where the river log pushed west tips in. */
#define RIVER_EDGE_COLUMN 20

/* The end of a field's growing: if a sparkle has burst on a field, the field
   takes its next pose. */
static Sint32 scKolimaForestEntranceFlashHitPsynergyFields(ACTOR *actor)
{
	Sint32 *pending = &KolimaForestEntrancePsynergyFieldHit;
	if(*pending) {
		fdActorSelectAnimation(actor, 2);
		*pending = 0;
	}
	return 1;
}

/* Each tick, a ground sparkle's timer runs on at random. */
static Sint32 scKolimaForestEntranceTwinkleGroundSparkle(ACTOR *actor)
{
	actor->timer = (actor->timer + (syRandomBelow(100)));
	if(actor->timer > 1000)
		fdActorSetSpriteColor(actor, SPRITE_COLOR_WHITE);
	else
		fdActorSetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
	if(actor->timer > 1200) {
		actor->timer = 0;
	}
	return 1;
}

/* Where the party enters, from the next scene of the forest
   (SceneIdKolimaForestGlade) and from the World Map. */
const SCENE_ENTRANCE *scGetEntrances(void)
{
	return KolimaForestEntranceEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return 0;
}

/* The ways on into the forest (SceneIdKolimaForestGlade) and back to the World
   Map. */
const Uint32 *scGetExits(void)
{
	return KolimaForestEntranceExits;
}

/* The party's companions, the logs, Tret, Laurel, the Psynergy fields and the
   ground sparkles. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	return KolimaForestEntranceActorPlacements;
}

/* In Kolima Forest (SceneIdKolimaForestEntrance), walking into a log rolls it
   ahead until something stops it. */
static void scKolimaForestEntrancePushLog(void)
{
	PUSHED_ACTOR_PROBE result;
	fdScriptStart();

	if(scPushedActorFindClearPos(&result)) {
		scPushedActorRoll(result);
		if(result.actorSlot == ACTOR_RIVER_LOG &&
			(FIELD_METATILE(result.pos.x)) == RIVER_EDGE_COLUMN) {
			fdActorSetAnimation(ACTOR_RIVER_LOG, LOG_ANIMATION_ROLL_WEST);
			fdActorSetDestinationOffset(ACTOR_RIVER_LOG, -18, 6);
			fdScriptWait(30);
			sySoundPlay(SE_LOG_FALL);
			fdActorSetAnimation(ACTOR_RIVER_LOG, LOG_ANIMATION_DROP_IN);
			/* It floats under the party's feet, and the river's metatiles under
			   it become ground to cross on. */
			fdActorGet(ACTOR_RIVER_LOG)->priorityFlags =
				ACTOR_PRIORITY_UNDERFOOT;
			fdMapGridCopyMetatileAttributes(0, 17, 2, 4, 19, 17);
			scMapGridFillCollisionRectangle(
				2, 20, 17, 1, 4, METATILE_BEHAVIOR_NORMAL);
			gmFlagSet(FLAG_KOLIMA_FOREST_ENTRANCE_LOG_IN_RIVER);
			fdActorSetSpriteFlags(fdActorGet(ACTOR_RIVER_LOG), 0);
		}
	}

	fdScriptEnd();
}

#include "shared/kolima_jump.c"

/* The logs, the hop east from the bank and the two ways out. */
const CUE *scGetCues(void)
{
	return KolimaForestEntranceCues;
}

/* On entry a log already in the river floats there again and the logs mark the
   metatiles they cover. */
Sint32 scInit(void)
{
	scPushedActorBlockFootprint(ACTOR_RIVER_LOG);
	if(gmFlagGet(FLAG_KOLIMA_FOREST_ENTRANCE_LOG_IN_RIVER) != 0) {
		fdActorGet(ACTOR_RIVER_LOG)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		fdMapGridCopyMetatileAttributes(0, 17, 2, 4, 19, 17);
		scMapGridFillCollisionRectangle(
			2, 20, 17, 1, 4, METATILE_BEHAVIOR_NORMAL);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_RIVER_LOG), 0);
	}
	scPushedActorBlockFootprint(ACTOR_FIRST_LOG);
	scPushedActorBlockFootprint(ACTOR_FIRST_LOG + 1);
	if(MainRoutineStateWork.entrance == KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP) {
		if(gmFlagGet(FLAG_KOLIMA_CURSE_WITNESSED) == 0) {
			scKolimaForestEntranceWitnessForestCurse();
		}
	}
	if(gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
		fdActorSetPos(ACTOR_FIRST_GROUND_SPARKLE, 0, 0);
		fdActorSetPos(ACTOR_FIRST_GROUND_SPARKLE + 1, 0, 0);
		fdActorSetPos(ACTOR_FIRST_GROUND_SPARKLE + 2, 0, 0);
		fdActorSetPos(ACTOR_FIRST_GROUND_SPARKLE + 3, 0, 0);
		fdActorSetPos(ACTOR_LAST_GROUND_SPARKLE, 0, 0);
	}
	return 0;
}

/* In Kolima Forest, on the first entry from the World Map before the curse has
   been seen in Kolima (FLAG_KOLIMA_CURSE_WITNESSED clear), the party sees the
   curse fall. It is the same script as scKolimaWitnessForestCurse without the
   trees that were people. */
static void scKolimaForestEntranceWitnessForestCurse(void)
{
	Sint32		 *party;
	const Sint32 *sparkleAction;
	const Sint32 *squashAction;
	Sint32		 *miaInParty;
	const Sint32 *restoreScaleAction;
	const Sint32 *growFieldAction;
	const Sint32 *shrinkFieldAction;

	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdCameraMoveTo(246, CAMERA_KEEP, 604, 0);
	party = &KolimaForestEntranceMiaInParty;
	*party = gmFlagGet(FLAG_IN_PARTY(MIA));
	fdActorSetSpriteFlags(fdActorGet(ACTOR_LEADER_PSYNERGY_FIELD), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_IVAN_PSYNERGY_FIELD), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_MIA_PSYNERGY_FIELD), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_GARET_PSYNERGY_FIELD), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_GROUND_SPARKLE), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_GROUND_SPARKLE + 1), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_GROUND_SPARKLE + 2), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_GROUND_SPARKLE + 3), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_LAST_GROUND_SPARKLE), 0);
	sparkleAction = KolimaForestEntranceGroundSparkleAction;
	fdActorSetScript(ACTOR_FIRST_GROUND_SPARKLE, sparkleAction);
	fdActorSetScript(ACTOR_FIRST_GROUND_SPARKLE + 1, sparkleAction);
	fdActorSetScript(ACTOR_FIRST_GROUND_SPARKLE + 2, sparkleAction);
	fdActorSetScript(ACTOR_FIRST_GROUND_SPARKLE + 3, sparkleAction);
	fdActorSetScript(ACTOR_LAST_GROUND_SPARKLE, sparkleAction);
	fdActorSetPos(ACTOR_PARTY_LEADER, 116, 602);
	sySynch(1);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 254, 593);
	fdActorSetSpeed(ACTOR_GARET, 0.6);
	fdActorSetSpeed(ACTOR_IVAN, 0.6);
	{
		ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

		if(leader != NULL) {
			fdActorSetPosFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
		}
	}
	{
		ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

		if(leader != NULL) {
			fdActorSetPosFX(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
		}
	}
	fdActorSetScript(ACTOR_GARET, KolimaForestEntranceGaretAction);
	fdActorSetScript(ACTOR_IVAN, KolimaForestEntranceIvanAction);
	if(*party != 0) {
		fdActorSetSpeed(ACTOR_MIA, 0.6);
		{
			ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

			if(leader != NULL) {
				fdActorSetPosFX(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
			}
		}
		fdActorSetScript(ACTOR_MIA, KolimaForestEntranceMiaAction);
	}
	fdActorWaitForScriptEndById(ACTOR_IVAN);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_IVAN, FACING_SOUTHEAST, 40);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_IVAN, FACING_WEST, 20);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_IVAN, FACING_SOUTH, 40);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	scKolimaForestEntranceFaceActorAndWait(
		ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 60);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_MIA, FACING_SOUTHEAST, 10);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	scKolimaForestEntranceFaceActorAndWait(
		ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	scKolimaForestEntranceFaceActorAndWait(
		ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptSetMessageId(MsgKolimaMatterIvan);
	/* "What is the matter, Ivan?" */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_GARET, 10);
	fdScriptSetMessageId(MsgKolimaSparklyStuffOnGround);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	/* "That sparkly stuff on the ground... What is it?" */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 20);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_GARET, FACING_EAST, 20);
	scKolimaForestEntranceFaceActorAndWait(
		ACTOR_PARTY_LEADER, FACING_NORTHWEST, 40);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 20);
	scKolimaForestEntranceFaceActorAndWait(
		ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 30);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_GARET, FACING_SOUTHWEST, 20);
	scKolimaForestEntranceFaceActorAndWait(
		ACTOR_PARTY_LEADER, FACING_NORTHEAST, 30);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	scKolimaForestEntranceFaceActorAndWait(
		ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 20);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
	sySoundPlay(SOUND_FADE_OUT_BGM);
	sySoundPlay(SE_CURSE_FALL);
	fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(1);
	sySynch(1);
	KolimaForestEntranceSparkleSoundOn = 1;
	sySetTickEvent(scKolimaForestEntranceSpawnCurseSparkle, EVENT_PASS_TICK);
	sySynch(20);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(16, 16, 20), PALETTES_BG);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_SPRITES);
	fdPaletteFadeStartInterpolation(120);
	sySynch(60);
	squashAction = KolimaPartySquashScript;
	fdActorSetScript(ACTOR_PARTY_LEADER, squashAction);
	fdActorSetScript(ACTOR_GARET, squashAction);
	fdActorSetScript(ACTOR_IVAN, squashAction);
	fdActorSetScript(ACTOR_MIA, squashAction);
	fdScriptWait(100);
	/* "Aaah! What's happening!?" */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_GARET, 20);
	/* "My mind!!! Someone is attacking my-" */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 40);
	if(KolimaForestEntranceMiaInParty != 0) {
		fdScriptWait(40);
		fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
		fdScriptWait(40);
		/* "I... I can't... I can't even stand up!" */
		scKolimaForestEntranceShowMessageAndWait(ACTOR_MIA, 40);
	} else {
		FieldStateWork->msg += 1;
	}
	fdScriptWait(20);
	if(KolimaForestEntranceMiaInParty != 0) {
		fdActorGet(ACTOR_MIA)->velocityY = toFIXED(2);
		fdScriptWait(10);
		fdActorSetSpeedAndAcceleration(ACTOR_MIA, 2, 2);
		fdActorSetDestinationOffset(ACTOR_MIA, -2, 0);
		fdActorSetScript(ACTOR_MIA, KolimaPartyRestoreScaleScript);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_MIA), 0);
		fdActorSetAnimation(ACTOR_MIA, ANIM_COLLAPSE);
		fdScriptWait(10);
	}
	fdActorGet(ACTOR_PARTY_LEADER)->velocityY = toFIXED(2);
	fdScriptWait(10);
	fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 2, 2);
	restoreScaleAction = KolimaPartyRestoreScaleScript;
	fdActorSetScript(ACTOR_PARTY_LEADER, restoreScaleAction);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_COLLAPSE);
	fdScriptWait(20);
	fdActorGet(ACTOR_GARET)->velocityY = toFIXED(2);
	fdScriptWait(10);
	fdActorSetSpeedAndAcceleration(ACTOR_GARET, 2, 2);
	fdActorSetScript(ACTOR_GARET, restoreScaleAction);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_GARET), 0);
	fdActorSetAnimation(ACTOR_GARET, ANIM_COLLAPSE);
	fdScriptWait(40);
	fdActorGet(ACTOR_IVAN)->velocityY = toFIXED(2);
	fdScriptWait(10);
	fdActorSetScript(ACTOR_IVAN, restoreScaleAction);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_IVAN), 0);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_COLLAPSE);
	KolimaForestEntranceSparkleSoundOn = 0;
	fdScriptWait(160);
	syCloseEvent(scKolimaForestEntranceSpawnCurseSparkle);
	fdScriptWait(120);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
	fdPaletteFadeStartInterpolation(60);
	sySynch(60);
	KolimaCurseDustWidth = 0;
	KolimaCurseDustOffset = toFIXED(128);
	KolimaCurseDustState = KOLIMA_DUST_CLOSING;
	sySetTickEvent(scKolimaUpdateCurseDust, EVENT_PASS_TICK);
	fdScriptWait(180);
	sySoundPlay(MUS_FOREST_THEME);
	/* "What is that? Something's falling from the sky!" */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_GARET, 80);
	/* "It's sparkling! Could this be what turned those people into..." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 40);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	/* "We've got to get out of here!" */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 20);
	KolimaCurseDustState = KOLIMA_DUST_CLOSING_WIDE;
	fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	fdScriptWait(40);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_MIA, 1);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 3);
	fdScriptWait(40);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	/* "I can't! I can't move!!" */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_GARET, 20);
	if(KolimaForestEntranceMiaInParty != 0) {
		fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
		/* "Oh, no... If it touches us, we'll turn into trees!" */
		scKolimaForestEntranceShowMessageAndWait(ACTOR_MIA, 10);
	} else {
		FieldStateWork->msg += 1;
	}
	KolimaCurseDustState = KOLIMA_DUST_DRIFTING;
	fdActorGet(ACTOR_PARTY_LEADER)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_GARET)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_IVAN)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
	fdActorSetSpritePriority(ACTOR_GARET, 3);
	fdActorSetSpritePriority(ACTOR_IVAN, 3);
	fdActorSetSpritePriority(ACTOR_MIA, 3);
	KolimaForestEntrancePsynergyFieldHit = 0;
	sySetTickEvent(
		scKolimaForestEntranceFlickerPsynergyFields, EVENT_PASS_TICK);
	sySoundPlay(SE_SHIMMER);
	fdActorGet(ACTOR_LEADER_PSYNERGY_FIELD)->priorityFlags &=
		~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSpritePriority(ACTOR_LEADER_PSYNERGY_FIELD, 2);
	fdActorSetPos(ACTOR_LEADER_PSYNERGY_FIELD, 253, 603);
	growFieldAction = KolimaForestEntrancePsynergyFieldGrowAction;
	fdActorSetScript(ACTOR_LEADER_PSYNERGY_FIELD, growFieldAction);
	fdActorGet(ACTOR_IVAN_PSYNERGY_FIELD)->priorityFlags &=
		~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSpritePriority(ACTOR_IVAN_PSYNERGY_FIELD, 2);
	fdActorSetPos(ACTOR_IVAN_PSYNERGY_FIELD, 233, 629);
	fdActorSetScript(ACTOR_IVAN_PSYNERGY_FIELD, growFieldAction);
	if(KolimaForestEntranceMiaInParty != 0) {
		fdActorGet(ACTOR_MIA_PSYNERGY_FIELD)->priorityFlags &=
			~ACTOR_PRIORITY_AUTOMATIC;
		fdActorSetSpritePriority(ACTOR_MIA_PSYNERGY_FIELD, 2);
		fdActorSetPos(ACTOR_MIA_PSYNERGY_FIELD, 207, 609);
		fdActorSetScript(ACTOR_MIA_PSYNERGY_FIELD, growFieldAction);
	}
	fdActorGet(ACTOR_GARET_PSYNERGY_FIELD)->priorityFlags &=
		~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSpritePriority(ACTOR_GARET_PSYNERGY_FIELD, 2);
	fdActorSetPos(ACTOR_GARET_PSYNERGY_FIELD, 227, 580);
	fdActorSetScript(ACTOR_GARET_PSYNERGY_FIELD, growFieldAction);
	while(KolimaCurseDustState != KOLIMA_DUST_CENTRED) {
		sySynch(1);
	}
	fdScriptWait(300);
	syCloseEvent(scKolimaUpdateCurseDust);
	fdScriptWait(120);
	sySoundPlay(SOUND_FADE_OUT_BGM);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
	fdPaletteFadeStartInterpolation(60);
	sySynch(60);
	fdActorStop(ACTOR_LEADER_PSYNERGY_FIELD);
	fdActorStop(ACTOR_IVAN_PSYNERGY_FIELD);
	if(KolimaForestEntranceMiaInParty != 0) {
		fdActorStop(ACTOR_MIA_PSYNERGY_FIELD);
	}
	fdActorStop(ACTOR_GARET_PSYNERGY_FIELD);
	sySynch(1);
	shrinkFieldAction = KolimaForestEntrancePsynergyFieldShrinkAction;
	fdActorSetScript(ACTOR_LEADER_PSYNERGY_FIELD, shrinkFieldAction);
	fdActorSetScript(ACTOR_IVAN_PSYNERGY_FIELD, shrinkFieldAction);
	if(KolimaForestEntranceMiaInParty != 0) {
		fdActorSetScript(ACTOR_MIA_PSYNERGY_FIELD, shrinkFieldAction);
	}
	fdActorRunScriptAndWait(ACTOR_GARET_PSYNERGY_FIELD, shrinkFieldAction);
	fdScriptWait(80);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(40);
	/* "Isaac... What just happened to us? That barrier... Did it save..." */
	fdScriptOpenMessage(ACTOR_GARET, 0);
	fdActorSetPos(ACTOR_TRET, 220, 494);
	fdActorSetPos(ACTOR_LAUREL, 220, 494);
	sySynch(1);
	/* Tret, unseen, takes the nod or the shake of the head. */
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_TRET, 0) == 1) {
		FieldStateWork->msg += 1;
	}
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	/* Yes: "I think so, too, Isaac. I think I know what those domes..." No:
	   "No, I think Garet is right. I think I know what those..." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 20);
	if(KolimaForestEntranceMiaInParty != 0) {
		fdActorRunRepeatedMotion(ACTOR_MIA, 2);
		fdScriptWait(10);
		fdScriptSetMessageId(MsgKolimaKnowThoseFieldsWere);
		/* "You know what those fields were?" */
		scKolimaForestEntranceShowMessageAndWait(ACTOR_MIA, 40);
	}
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(80);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptSetMessageId(MsgKolimaWasOurPsynergy);
	/* "It was our Psynergy." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 40);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdScriptWait(40);
	fdActorSetSpritePriority(ACTOR_GARET, 2);
	fdActorGet(ACTOR_GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSpriteFlags(fdActorGet(ACTOR_GARET), SPRITE_FLAG_SHADOW);
	fdActorJump(ACTOR_GARET, 6, 0);
	fdActorSetDestinationOffset(ACTOR_GARET, -3, 0);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 60);
	/* "What are you, crazy?" */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_GARET, 20);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	/* "I didn't use any Psynergy!" */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_GARET, 10);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(40);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_GARET, FACING_SOUTHWEST, 40);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 20);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_GARET, FACING_SOUTHWEST, 20);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
	fdActorJump(ACTOR_GARET, 2, 0);
	fdScriptWait(40);
	fdActorJump(ACTOR_GARET, 2, 0);
	fdScriptWait(10);
	fdActorJump(ACTOR_GARET, 4, 0);
	fdScriptWait(20);
	/* "Hey... I can stand... I can move again!" */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_GARET, 20);
	if(KolimaForestEntranceMiaInParty != 0) {
		fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdScriptWait(60);
		fdActorRunRepeatedMotion(ACTOR_MIA, 2);
		fdScriptWait(80);
		fdActorSetSpritePriority(ACTOR_MIA, 2);
		fdActorGet(ACTOR_MIA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
		fdActorSetSpriteFlags(fdActorGet(ACTOR_MIA), SPRITE_FLAG_SHADOW);
		fdActorJump(ACTOR_MIA, 4, 0);
		fdActorSetDestinationOffset(ACTOR_MIA, -2, 0);
		fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
		scKolimaForestEntranceFaceActorAndWait(ACTOR_MIA, FACING_NORTHEAST, 60);
		fdActorRunRepeatedMotion(ACTOR_MIA, 2);
		fdScriptWait(20);
		/* "You're right. I can stand, too..." */
		scKolimaForestEntranceShowMessageAndWait(ACTOR_MIA, 20);
	} else {
		FieldStateWork->msg += 1;
	}
	fdActorJump(ACTOR_GARET, 2, 0);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
	/* "Ivan, it's okay! Go ahead! Stand up!" */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_GARET, 20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(20);
	fdActorSetSpritePriority(ACTOR_IVAN, 2);
	fdActorGet(ACTOR_IVAN)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSpriteFlags(fdActorGet(ACTOR_IVAN), SPRITE_FLAG_SHADOW);
	fdActorJump(ACTOR_IVAN, 4, 0);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(10);
	fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
	fdActorGet(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSpriteFlags(fdActorGet(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
	fdActorJump(ACTOR_PARTY_LEADER, 4, 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	scKolimaForestEntranceFaceActorAndWait(
		ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 60);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdScriptWait(60);
	scKolimaForestEntranceFaceActorAndWait(
		ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	scKolimaForestEntranceFaceActorAndWait(
		ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 10);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	/* "Those barriers..." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	fdScriptWait(20);
	/* "We thought that Psynergy was only used when we concentrate,..." */
	fdScriptOpenMessage(ACTOR_IVAN, 0);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
		fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		FieldStateWork->msg += 1;
	} else {
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		scKolimaForestEntranceFaceActorAndWait(
			ACTOR_GARET, FACING_SOUTHEAST, 10);
		fdActorStartRepeatedMotion(ACTOR_GARET, 2);
		/* "You're such a liar, Isaac!" */
		fdScriptShowMessage(ACTOR_GARET, 0);
	}
	scKolimaForestEntranceFaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 10);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	/* "When I was training, I'd focus so hard, I thought my head..." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_GARET, 20);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	/* "Well, maybe it's just easier for you, but when I use Psynergy..." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 10);
	if(KolimaForestEntranceMiaInParty != 0) {
		fdActorRunRepeatedMotion(ACTOR_MIA, 2);
		scKolimaForestEntranceFaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
		scKolimaForestEntranceFaceActorAndWait(ACTOR_MIA, FACING_SOUTHEAST, 10);
		fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
		/* "But what? The only way you can use Psynergy is by concentrating!" */
		scKolimaForestEntranceShowMessageAndWait(ACTOR_MIA, 10);
	} else {
		FieldStateWork->msg += 1;
	}
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	scKolimaForestEntranceFaceActorAndWait(
		ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(80);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 10);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
	/* "Anyway, I'm certain that was Psynergy we were projecting." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	scKolimaForestEntranceFaceActorAndWait(
		ACTOR_PARTY_LEADER, FACING_NORTHWEST, 40);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	scKolimaForestEntranceFaceActorAndWait(
		ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	/* "That must mean that some Psynergy acts only when it's needed." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 10);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	/* "You mean, like when we're in danger?" */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_GARET, 20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(40);
	/* "I think it's like when we strike critical hits in battle..." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_GARET, 20);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	/* "I can agree with what Garet is saying. It is definitely..." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
	/* "Wouldn't it be something if we could use this power at any..." */
	fdScriptOpenMessage(ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	} else {
		fdScriptWait(20);
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		fdScriptWait(40);
		FieldStateWork->msg += 1;
	}
	/* Yes: "Yeah! Let's start trying to master this power!" No: "You... You're
	   not really into this, are you?" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	sySoundPlay(MUS_FOREST_THEME);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
	fdPaletteFadeStartInterpolation(60);
	sySynch(60);
	KolimaCurseDustWidth = 0;
	KolimaCurseDustOffset = toFIXED(128);
	KolimaCurseDustState = KOLIMA_DUST_CLOSING;
	sySetTickEvent(scKolimaUpdateCurseDust, EVENT_PASS_TICK);
	fdScriptWait(80);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(60);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
	fdScriptSetMessageId(MsgKolimaWatchOutItsHappeningAgain);
	/* "Watch out! It's happening again!" */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 10);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_GARET, FACING_NORTH, 10);
	scKolimaForestEntranceFaceActorAndWait(
		ACTOR_PARTY_LEADER, FACING_NORTH, 10);
	if(KolimaForestEntranceMiaInParty != 0) {
		scKolimaForestEntranceFaceActorAndWait(ACTOR_MIA, FACING_NORTH, 10);
	}
	fdActorGet(ACTOR_PARTY_LEADER)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_GARET)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_IVAN)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
	fdActorSetSpritePriority(ACTOR_GARET, 3);
	fdActorSetSpritePriority(ACTOR_IVAN, 3);
	fdActorSetSpritePriority(ACTOR_MIA, 3);
	KolimaCurseDustState = KOLIMA_DUST_CLOSING_WIDE;
	sySoundPlay(SE_SHIMMER);
	fdActorSetPos(ACTOR_LEADER_PSYNERGY_FIELD, 253, 603);
	growFieldAction = KolimaForestEntrancePsynergyFieldGrowAction;
	fdActorSetScript(ACTOR_LEADER_PSYNERGY_FIELD, growFieldAction);
	fdActorSetPos(ACTOR_IVAN_PSYNERGY_FIELD, 233, 629);
	fdActorSetScript(ACTOR_IVAN_PSYNERGY_FIELD, growFieldAction);
	if(KolimaForestEntranceMiaInParty != 0) {
		fdActorSetPos(ACTOR_MIA_PSYNERGY_FIELD, 207, 609);
		fdActorSetScript(ACTOR_MIA_PSYNERGY_FIELD, growFieldAction);
	}
	fdActorSetPos(ACTOR_GARET_PSYNERGY_FIELD, 227, 580);
	fdActorSetScript(ACTOR_GARET_PSYNERGY_FIELD, growFieldAction);
	fdScriptWait(120);
	KolimaCurseDustState = KOLIMA_DUST_DRIFTING;
	do {
		sySynch(1);
	} while(KolimaCurseDustState != KOLIMA_DUST_CENTRED);
	/* "Hroom! Why? Why don't they turn into trees...?" */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_TRET, 80);
	/* "Who are they?" */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_LAUREL, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(60);
	/* "What people are these, who are immune to Tret's glamour?" */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_LAUREL, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	/* "Have they come to save that village with their strange powers?" */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_LAUREL, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	/* "It will not be enough to save the people of Kolima." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_LAUREL, 10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdScriptWait(40);
	/* "They have earned their fate...Kolima took their axes to me,..." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_TRET, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_EAST, 0);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 40);
	/* "You children may not know this...but we are dying." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_LAUREL, 10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_MIA, FACING_NORTH, 80);
	/* "And Kolima Forest will wither and die when we are gone." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_LAUREL, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_MIA, FACING_EAST, 40);
	/* "We are not the only ones who will die, Laurel. Once we are..." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_TRET, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_MIA, FACING_NORTH, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdScriptWait(60);
	/* "Tret was once a kindly forest king." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_LAUREL, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	/* "But he is slowly dying, and his heart has torn in two..." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_LAUREL, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
	/* "His wrathful side turned the people of Kolima into trees." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_LAUREL, 10);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	/* "The kind Tret speaks no more." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_LAUREL, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
	/* "Hroom! The kindly heart invites destruction... I need no..." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_TRET, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(40);
	/* "If you wish to save those whom Tret turned into trees, you..." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_LAUREL, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_MIA, FACING_NORTH, 10);
	/* "But...if you cannot stop Tret from withering, we all will..." */
	scKolimaForestEntranceShowMessageAndWait(ACTOR_LAUREL, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(60);
	/* "Look for Tret deep within the forest." */
	fdScriptShowMessage(ACTOR_LAUREL, 0);
	/* "It's no use, Laurel. Humans cannot be trusted! I shall die..." */
	fdScriptShowMessage(ACTOR_TRET, 0);
	syCloseEvent(scKolimaUpdateCurseDust);
	fdScriptWait(80);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
	fdPaletteFadeStartInterpolation(60);
	sySynch(80);
	fdActorStop(ACTOR_LEADER_PSYNERGY_FIELD);
	fdActorStop(ACTOR_IVAN_PSYNERGY_FIELD);
	miaInParty = &KolimaForestEntranceMiaInParty;
	fdActorStop(ACTOR_MIA_PSYNERGY_FIELD);
	fdActorStop(ACTOR_GARET_PSYNERGY_FIELD);
	sySynch(1);
	shrinkFieldAction = KolimaForestEntrancePsynergyFieldShrinkAction;
	fdActorSetScript(ACTOR_LEADER_PSYNERGY_FIELD, shrinkFieldAction);
	fdActorSetScript(ACTOR_IVAN_PSYNERGY_FIELD, shrinkFieldAction);
	if(*miaInParty != 0) {
		fdActorSetScript(ACTOR_MIA_PSYNERGY_FIELD, shrinkFieldAction);
	}
	fdActorRunScriptAndWait(ACTOR_GARET_PSYNERGY_FIELD, shrinkFieldAction);
	fdScriptWait(20);
	fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
	fdActorSetSpritePriority(ACTOR_GARET, 2);
	fdActorSetSpritePriority(ACTOR_IVAN, 2);
	fdActorSetSpritePriority(ACTOR_MIA, 2);
	fdActorGet(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_IVAN)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_MIA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	scKolimaForestEntranceFaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 10);
	/* "Isaac... Isn't there anything we can do?" */
	fdScriptOpenMessage(ACTOR_IVAN, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_EAST, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		fdScriptWait(10);
		/* "What can we do? Do you have any idea?" */
		fdScriptOpenMessage(ACTOR_GARET, 0);
		if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
			scKolimaForestEntranceFaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
			fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
			fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
			fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
			fdScriptWait(40);
			scKolimaForestEntranceFaceActorAndWait(
				ACTOR_GARET, FACING_SOUTH, 20);
			/* "It looks like Isaac has something in mind." */
			scKolimaForestEntranceShowMessageAndWait(ACTOR_GARET, 10);
			scKolimaForestEntranceFaceActorAndWait(
				ACTOR_IVAN, FACING_NORTH, 20);
			scKolimaForestEntranceFaceActorAndWait(
				ACTOR_IVAN, FACING_NORTHEAST, 20);
			fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
			/* "We'll leave it up to you, Isaac. Let's go save everyone." */
			scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 20);
			scKolimaForestEntranceFaceActorAndWait(
				ACTOR_GARET, FACING_SOUTHEAST, 20);
		} else {
			scKolimaForestEntranceFaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
			fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
			fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
			fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
			fdScriptWait(40);
			scKolimaForestEntranceFaceActorAndWait(
				ACTOR_GARET, FACING_SOUTH, 20);
			fdScriptSetMessageId(MsgKolimaHesAsStumpedAsWe);
			/* "He's as stumped as we are, but he says he'll help..." */
			scKolimaForestEntranceShowMessageAndWait(ACTOR_GARET, 20);
			fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
			/* "I trust Isaac. He'll come up with an idea." */
			scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 20);
		}
		fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	} else {
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdScriptWait(10);
		fdScriptSetMessageId(MsgKolimaYoureIsaacThereIsntMuch);
		/* "You're right, Isaac... There isn't much we can do now." */
		scKolimaForestEntranceShowMessageAndWait(ACTOR_GARET, 10);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
		scKolimaForestEntranceFaceActorAndWait(
			ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 20);
		fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
		/* "So the people of Kolima will just remain trees until they..." */
		fdScriptOpenMessage(ACTOR_IVAN, 0);
		if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
			fdScriptWait(20);
			fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
			fdScriptWait(40);
			scKolimaForestEntranceFaceActorAndWait(
				ACTOR_IVAN, FACING_NORTHEAST, 10);
			/* "I'm disappointed in you, Isaac... How can you give up so..." */
			scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 10);
			if(*miaInParty != 0) {
				scKolimaForestEntranceFaceActorAndWait(
					ACTOR_MIA, FACING_EAST, 10);
				fdActorStartRepeatedMotion(ACTOR_MIA, 3);
				/* "That's right... We can't just leave them to rot!" */
				scKolimaForestEntranceShowMessageAndWait(ACTOR_MIA, 20);
			} else {
				FieldStateWork->msg += 1;
			}
			fdActorShowEmote(
				ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
			fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
			fdScriptWait(40);
			fdActorRunRepeatedMotion(ACTOR_GARET, 2);
			/* "But how can we bring back the nice Tret and save the
			   village?" */
			scKolimaForestEntranceShowMessageAndWait(ACTOR_GARET, 20);
			fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
			fdScriptWait(120);
			/* "I guess I really don't know what to do." */
			scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 40);
			if(KolimaForestEntranceMiaInParty != 0) {
				scKolimaForestEntranceFaceActorAndWait(
					ACTOR_MIA, FACING_SOUTHEAST, 10);
				fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
				/* "Ivan, you can't give up! Please, convince him to help!" */
				scKolimaForestEntranceShowMessageAndWait(ACTOR_MIA, 10);
			} else {
				FieldStateWork->msg += 1;
			}
			fdScriptWait(60);
			fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
			if(KolimaForestEntranceMiaInParty != 0) {
				scKolimaForestEntranceFaceActorAndWait(
					ACTOR_IVAN, FACING_NORTHWEST, 40);
				scKolimaForestEntranceFaceActorAndWait(
					ACTOR_IVAN, FACING_NORTHEAST, 20);
			}
			/* "Just promise me this one thing..." */
			scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 10);
			fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
			fdActorRunRepeatedMotion(ACTOR_GARET, 2);
			fdScriptWait(40);
			/* "Promise you will do everything in your power to save Kolima." */
			scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 20);
			fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
			fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
			fdScriptWait(20);
			fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
			fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		} else {
			fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
			fdScriptWait(40);
			fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
			fdScriptSetMessageId(MsgKolimaThatsReliefIsaacThoughtYoud);
			/* "That's a relief, Isaac. I thought you'd given up on
			   Kolima..." */
			scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 20);
			if(*miaInParty != 0) {
				scKolimaForestEntranceFaceActorAndWait(
					ACTOR_MIA, FACING_EAST, 10);
				fdActorStartRepeatedMotion(ACTOR_MIA, 1);
				/* "I was beginning to lose faith in you, Isaac." */
				scKolimaForestEntranceShowMessageAndWait(ACTOR_MIA, 20);
			} else {
				FieldStateWork->msg += 1;
			}
			fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
			fdActorShowEmote(
				ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
			fdScriptWait(40);
			fdActorRunRepeatedMotion(ACTOR_GARET, 2);
			/* "But what can we do? How can we bring back the good Tret
			   and..." */
			scKolimaForestEntranceShowMessageAndWait(ACTOR_GARET, 20);
			fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
			fdScriptWait(80);
			/* "I really don't have any idea right now..." */
			scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 40);
			if(KolimaForestEntranceMiaInParty != 0) {
				scKolimaForestEntranceFaceActorAndWait(
					ACTOR_MIA, FACING_SOUTHEAST, 20);
				fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
				/* "I'm not sure... What should we do?" */
				scKolimaForestEntranceShowMessageAndWait(ACTOR_MIA, 40);
			} else {
				FieldStateWork->msg += 1;
			}
			fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
			fdScriptWait(20);
			/* "I don't know just yet... But I'm sure we'll think of
			   something." */
			scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 20);
			fdActorStartRepeatedMotion(ACTOR_GARET, 2);
			fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
			fdScriptWait(40);
			/* "Well, let's get on with it!" */
			scKolimaForestEntranceShowMessageAndWait(ACTOR_IVAN, 20);
		}
	}
	sySoundPlay(SOUND_FADE_OUT_BGM);
	fdActorSetSpeed(ACTOR_GARET, 1.2);
	fdActorSetSpeed(ACTOR_IVAN, 1.2);
	fdActorSetSpeed(ACTOR_MIA, 1.2);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	{
		ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

		if(leader != NULL) {
			fdActorSetDestination(ACTOR_GARET, leader->x.part[0].integer,
				leader->z.part[0].integer);
		}
	}
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
	{
		ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

		if(leader != NULL) {
			fdActorSetDestination(ACTOR_IVAN, leader->x.part[0].integer,
				leader->z.part[0].integer);
		}
	}
	fdActorWaitForMoveById(ACTOR_IVAN);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
	if(KolimaForestEntranceMiaInParty != 0) {
		fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
		{
			ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

			if(leader != NULL) {
				fdActorSetDestination(ACTOR_MIA, leader->x.part[0].integer,
					leader->z.part[0].integer);
			}
		}
		fdActorWaitForMoveById(ACTOR_MIA);
		fdActorSetPos(ACTOR_MIA, 0, 0);
	}
	gmFlagSet(FLAG_KOLIMA_CURSE_WITNESSED);
	fdActorSetPos(ACTOR_TRET, 0, 0);
	fdActorSetPos(ACTOR_LAUREL, 0, 0);
	gmPlayMusic();
	fdScriptEnd();
}

/* Shows the script's next message at speaker, then waits ticks. */
static void scKolimaForestEntranceShowMessageAndWait(
	Sint32 speaker, Sint32 ticks)
{
	fdScriptShowMessage(speaker, 0);
	fdScriptWait(ticks);
}

/* Turns an actor at once, then leaves time for the next beat. */
static void scKolimaForestEntranceFaceActorAndWait(
	Sint32 actorId, Sint32 facing, Sint32 waitTicks)
{
	fdActorFaceDirection(actorId, facing, 0);
	fdScriptWait(waitTicks);
}

#include "shared/kolima_grow.c"

/* Every eighth tick a sparkle of Tret's glamour (SPRITE_GOLDEN_RINGS) falls
   toward the party, with its tinkle while the rain is at its height. */
static void scKolimaForestEntranceSpawnCurseSparkle(void)
{
	ACTOR *sparkle;
	SPR	  *sprite;
	Sint32 framePhase;

	framePhase = TickCount & 7;
	if(framePhase != 0)
		return;
	if(KolimaForestEntranceSparkleSoundOn != 0)
		sySoundPlay(SE_SPARKLE_FALL);
	sparkle =
		syFieldActorCreate(SPRITE_GOLDEN_RINGS, toFIXED(231), 0, toFIXED(460));
	if(sparkle == NULL)
		return;
	sprite = sparkle->sprite;
	sprite->flags = framePhase;
	sparkle->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
	sprite->image.priority = 1;
	sparkle->scaleX = toFIXED(1) / 10;
	sparkle->speedLimit = toFIXED(8);
	sparkle->acceleration = toFIXED(8);
	sparkle->motionFlags = framePhase;
	fdActorSelectAnimation(sparkle, 2);
	syFieldActorSetMoveTargetFX(sparkle, toFIXED(231), 0, toFIXED(624));
	fdActorStartScript(sparkle, KolimaForestEntranceSparkleScript);
}

#include "shared/kolima_blink.c"

/* The end of a sparkle's fall: one that lands on a field (the Psynergy fields'
   area, which differs with Mia) bursts with a sound, and the Psynergy fields
   are told. */
static Sint32 scKolimaForestEntranceBurstOnPsynergyField(ACTOR *sparkle)
{
	FIXED x;

	if(!(KolimaForestEntranceMiaInParty != 0
			   ? (x = sparkle->x.fixed,
					 (x > toFIXED(192) && x < toFIXED(274) &&
						 sparkle->z.fixed > toFIXED(566) &&
						 sparkle->z.fixed < toFIXED(612)) ||
						 (x > toFIXED(202) && x < toFIXED(255) &&
							 sparkle->z.fixed > toFIXED(549) &&
							 sparkle->z.fixed < toFIXED(632)))
			   : (x = sparkle->x.fixed,
					 (x > toFIXED(192) && x < toFIXED(244) &&
						 sparkle->z.fixed > toFIXED(549) &&
						 sparkle->z.fixed < toFIXED(585)) ||
						 (x > toFIXED(244) && x < toFIXED(274) &&
							 sparkle->z.fixed > toFIXED(571) &&
							 sparkle->z.fixed < toFIXED(605)) ||
						 (x > toFIXED(211) && x < toFIXED(255) &&
							 sparkle->z.fixed > toFIXED(596) &&
							 sparkle->z.fixed < toFIXED(632))))) {
		return 0;
	}
	sySoundPlay(SE_DRIP);
	/* UB: this function returns void; call it through its declared type. */
#if FIX_FUNCTION_SIGNATURES
	fdActorStartScript(sparkle, KolimaSparkleBurstScript);
#else
	((Sint32 (*)())fdActorStartScript)(
		(Sint32)sparkle, (Sint32)KolimaSparkleBurstScript);
#endif
	KolimaForestEntrancePsynergyFieldHit = 1;
	return 0;
}

#include "shared/kolima_curse_dust.c"
Sint32 KolimaForestEntrancePsynergyFieldHit BSS_DATA;
Sint32 KolimaForestEntranceMiaInParty		BSS_DATA;
Sint32 KolimaForestEntranceSparkleSoundOn	BSS_DATA;

/* Each tick the Psynergy fields flicker: two show on even ticks and two on
   odd, the hidden ones lifted 500 pixels out of sight. */
static void scKolimaForestEntranceFlickerPsynergyFields(void)
{
	ACTOR *p;

	p = fdActorGet(ACTOR_LEADER_PSYNERGY_FIELD);
	if(p != NULL) {
		p->motionFlags = 0;
		if((TickCount & 1) == 0) {
			p->y.fixed = 0;
		} else {
			p->y.fixed = toFIXED(500);
		}
	}
	p = fdActorGet(ACTOR_IVAN_PSYNERGY_FIELD);
	if(p != NULL) {
		p->motionFlags = 0;
		if((TickCount & 1) != 0) {
			p->y.fixed = 0;
		} else {
			p->y.fixed = toFIXED(500);
		}
	}
	p = fdActorGet(ACTOR_MIA_PSYNERGY_FIELD);
	if(p != NULL) {
		p->motionFlags = 0;
		if((TickCount & 1) == 0) {
			p->y.fixed = 0;
		} else {
			p->y.fixed = toFIXED(500);
		}
	}
	p = fdActorGet(ACTOR_GARET_PSYNERGY_FIELD);
	if(p != NULL) {
		p->motionFlags = 0;
		if((TickCount & 1) != 0) {
			p->y.fixed = 0;
		} else {
			p->y.fixed = toFIXED(500);
		}
	}
}

#include "shared/pushed_actor_tables.c"

/* Garet, Ivan and Mia walk to their places beside the leader. */
Sint32 KolimaForestEntranceGaretAction[] = {
	SC_ANIMATION(2),
	SC_MOVE(234, 0, 576),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_NORTHEAST),
	SC_ANIMATION(1),
	SC_END,
};

Sint32 KolimaForestEntranceIvanAction[] = {
	SC_ANIMATION(2),
	SC_MOVE(234, 0, 623),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_EAST),
	SC_ANIMATION(1),
	SC_END,
};

Sint32 KolimaForestEntranceMiaAction[] = {
	SC_ANIMATION(2),
	SC_MOVE(211, 0, 602),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_NORTH),
	SC_ANIMATION(1),
	SC_END,
};

#include "shared/kolima_squash.c"

/* A field grows from a tenth of its size over 70 ticks. */
Sint32 KolimaForestEntrancePsynergyFieldGrowAction[] = {
	SC_SET(SW_SCALE_X, toFIXED(1) / 10),
	SC_SET(SW_SCALE_Y, toFIXED(1) / 10),
	SC_WAIT(1),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 50),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 50),
	SC_WAIT(1),
	SC_LOOP(70, SC_TO(1)),
	SC_CALL(scKolimaForestEntranceFlashHitPsynergyFields),
	SC_END,
};

/* A field sinks and shrinks away over 90 ticks. */
Sint32 KolimaForestEntrancePsynergyFieldShrinkAction[] = {
	SC_ANIMATION(1),
	SC_ADD(SW_Z, -toFIXED(1) / 10),
	SC_ADD(SW_SCALE_X, -toFIXED(3) / 200),
	SC_ADD(SW_SCALE_Y, -toFIXED(3) / 200),
	SC_WAIT(1),
	SC_LOOP(90, SC_TOP),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

Sint32 KolimaForestEntranceGroundSparkleAction[] = {
	SC_SET(SW_SCALE_X, toFIXED(3) / 4),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 4),
	SC_SET(SW_COLLISION, 0),
	SC_SET(SW_TIMER, 0),
	SC_LABEL(1),
	SC_CALL(scKolimaForestEntranceTwinkleGroundSparkle),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

SCENE_ENTRANCE KolimaForestEntranceEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 384, 0, 312, FACING_NORTH, 0, -1, -1,
		-1, -1, 0},
	{KOLIMA_FOREST_ENTRANCE_FROM_KOLIMA_FOREST_GLADE, CONDITION_ALWAYS, 24, 0,
		128, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP, CONDITION_ALWAYS, 172, 0, 602,
		FACING_EAST, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

/* On into the forest (SceneIdKolimaForestGlade) and out to the World Map
   (SceneIdWorldMap), with two exits no cue uses back into this scene. */
Uint32 KolimaForestEntranceExits[] = {
	SCENE_EXITS(KolimaForestEntrance),
	SCENE_EXIT(KOLIMA_FOREST_ENTRANCE_TO_KOLIMA_FOREST_GLADE, KolimaForestGlade,
		KOLIMA_FOREST_GLADE_FROM_KOLIMA_FOREST_ENTRANCE),
	SCENE_EXIT(KOLIMA_FOREST_ENTRANCE_UNUSED_EXIT_A, KolimaForestEntrance,
		KOLIMA_FOREST_ENTRANCE_UNUSED),
	SCENE_EXIT(KOLIMA_FOREST_ENTRANCE_UNUSED_EXIT_B, KolimaForestEntrance,
		KOLIMA_FOREST_ENTRANCE_UNUSED),
	SCENE_EXIT(KOLIMA_FOREST_ENTRANCE_TO_WORLD_MAP, WorldMap,
		WORLD_MAP_FROM_KOLIMA_FOREST_ENTRANCE),
	SCENE_EXITS_END,
};

/* Garet, Ivan and Mia for the curse; the three logs; Tret and Laurel; the four
   Psynergy fields; and the five ground sparkles. */
/* clang-format off */
ACTOR_PLACE KolimaForestEntranceActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		232, 0, 128, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		208, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		360, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TRET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_LAUREL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY,
		ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY,
		ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY,
		ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		232, 0, 655, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		166, 0, 576, FACING_SOUTH, TALK_FACING_PARTY,
		ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		189, 0, 532, FACING_SOUTH, TALK_FACING_PARTY,
		ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		288, 0, 570, FACING_SOUTH, TALK_FACING_PARTY,
		ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		331, 0, 659, FACING_SOUTH, TALK_FACING_PARTY,
		ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{ SCENE_TABLE_END },
};
/* clang-format on */

CUE KolimaForestEntranceCues[] = {
	{CUE_STEP | STEP_CUE_FLAG_WALKING, 10, CONDITION_ALWAYS,
		SCRIPT_REF(scKolimaForestEntrancePushLog)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS,
		SCRIPT_REF(scKolimaForestHopEast)},
	{CUE_EXIT, 1, CONDITION_ALWAYS,
		KOLIMA_FOREST_ENTRANCE_TO_KOLIMA_FOREST_GLADE},
	{CUE_EXIT, 4, CONDITION_ALWAYS, KOLIMA_FOREST_ENTRANCE_TO_WORLD_MAP},
	{CUE_TABLE_END},
};

Sint32 KolimaForestEntranceSparkleScript[] = {
	SC_CALL(scFieldEffectStretchUntilArrived),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

#include "shared/kolima_sparkle_burst_script.c"

Sint32 KolimaCurseDustScript[] = {
	SC_ADD(SW_Y, -toFIXED(2)),
	SC_CALL(scKolimaFlickerCurseDust),
	SC_WAIT(1),
	SC_ADD(SW_Y, -toFIXED(2)),
	SC_CALL(scKolimaFlickerCurseDust),
	SC_WAIT(1),
	SC_LOOP(30, SC_TOP),
	SC_LABEL(1),
	SC_ADD(SW_Y, -toFIXED(1)),
	SC_CALL(scKolimaFlickerCurseDust),
	SC_WAIT(1),
	SC_LOOP(40, SC_TO(1)),
	SC_CALL(scKolimaForestEntranceBurstOnPsynergyField),
	SC_RELEASE,
};
