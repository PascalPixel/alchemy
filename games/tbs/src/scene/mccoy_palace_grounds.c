/* The grounds of McCoy's Palace, at the top of Bilibin.

   The grounds are their own scene, SceneIdMcCoyPalaceGrounds, sharing the area
   name McCoy's Palace with the palace itself (mccoy_palace.c). Steps lead up
   from Bilibin to a sandy yard before the palace door, where two soldiers in
   spiked helmets stand guard. West of the yard, workers with pickaxes have been
   building Lady McCoy a new palace, stopped since the lumber from Kolima
   vanished, and Lady McCoy herself complains of it. A Nut hangs in the orange
   tree west of the steps for Catch, and a dry stone well stands by the palace's
   east corner.

   Talking to the east guard is the way in
   (scMcCoyPalaceGroundsTalkToEastGuard). He asks whether the party is up to
   meeting his lord; on yes the two guards confer, the west guard opens the door
   (scMcCoyPalaceGroundsOpenDoor) and goes in ahead, and walking north into the
   doorway takes the party inside (scMcCoyPalaceGroundsEnterPalace). After McCoy
   has turned it away, the guards only talk.

   The first time the party comes back once Kolima's curse is lifted, the guards
   stop it on the steps, ask whether it broke the curse of the holy tree until
   Isaac says yes, put it "under arrest" and march it straight into McCoy's room
   for its reward (scMcCoyPalaceGroundsGuardsArrestParty). After the curse, Lady
   McCoy stands at the west edge of the yard.

   Its people follow three story flags: FLAG_MCCOY_PALACE_ADMITTED,
   FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY (set once McCoy has turned the party
   away) and FLAG_KOLIMA_CURSE_LIFTED; scMcCoyPalaceGroundsRestoreStory places
   them by those flags.

   In the French, Spanish and Italian editions, a patch of ivy (SPRITE_IVY)
   grows at metatile (8, 24), over a dark opening at the foot of the cliff on
   the grounds' south edge. The earlier editions leave the opening bare. No exit
   or cue answers that opening, so the ivy only hides a way in that leads
   nowhere. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "graphics/graphics.h"
#include "scene/floating_nut.h"
#include "scene/catch_nut.h"
#include "text/msg_ids.h"

extern Uint16		  McCoyPalaceGroundsDoorOpenSteps[];
extern Sint32		  McCoyPalaceGroundsGuardWatchesParty[];
extern SCENE_ENTRANCE McCoyPalaceGroundsEntrances[];
extern Uint32		  McCoyPalaceGroundsExits[];
extern ACTOR_PLACE	  McCoyPalaceGroundsNoActorPlacements[];
extern ACTOR_PLACE	  McCoyPalaceGroundsActorPlacements[];
extern CUE			  McCoyPalaceGroundsNoCues[];
extern CUE			  McCoyPalaceGroundsCues[];

/* The placed actors, numbered in the order of
   McCoyPalaceGroundsActorPlacements. */
typedef enum MC_COY_PALACE_GROUNDS_ACTOR_ID {
	/* A worker with a pickaxe at the west edge of the yard, by the new palace.
	   Before the curse is lifted: "The lumber I ordered from Kolima disappeared
	   after the incident." */
	ACTOR_WEST_WORKER = ACTOR_FIRST_PLACED,
	/* The worker with a pickaxe who wanders by the new palace, north of the
	   other. "Have you seen the barricade we built on the road to Kolima?" */
	ACTOR_NORTH_WORKER,
	/* Lady McCoy, a woman with curly purple hair in a pink dress, at the foot
	   of the new palace. Once the curse is lifted she stands at the west edge
	   of the yard: "Hmph! My palace is not finished!" */
	ACTOR_LADY_MCCOY,
	/* The west guard at the palace door, a soldier in a spiked helmet, who goes
	   in ahead of the party. "Lord McCoy is so upset about this curse that he
	   can't even eat." */
	ACTOR_WEST_GUARD,
	/* The east guard at the palace door, who decides who may see McCoy. "The
	   lord will only meet with brave and mighty warriors." */
	ACTOR_EAST_GUARD,
	/* The Nut in the orange tree west of the steps, gone once Catch has taken
	   it (FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT). */
	ACTOR_FLOATING_NUT
} MC_COY_PALACE_GROUNDS_ACTOR_ID;

/* The stone well by the palace's east corner, metatiles (28, 16) to (29, 17),
   which the cues answer besides the exits, whose metatile behaviors are their
   exits' numbers. */
typedef enum MC_COY_PALACE_GROUNDS_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_MCCOY_PALACE_GROUNDS_WELL = 90
} MC_COY_PALACE_GROUNDS_METATILE_BEHAVIOR;

/* The grounds' temporary flags. */
enum {
	/* The palace door stands open (scMcCoyPalaceGroundsOpenDoor), until the
	   scene is left. */
	FLAG_DOOR_OPEN = FLAG_SCENE_TEMP_00,
	/* The east guard has scolded the party for keeping McCoy waiting. */
	FLAG_GUARD_ASKED_WHERE_YOU_WERE = FLAG_SCENE_TEMP_01,
	/* Set by Bilibin (bilibin.c) while the guards have let the party in but it
	   has not yet entered the palace. The grounds are in another area than
	   Bilibin, so entering them clears it before the guard can read it. */
	FLAG_KEEPING_MCCOY_WAITING = FLAG_AREA_TEMP_04
};

static void scMcCoyPalaceGroundsRestoreStory(void);
static void scMcCoyPalaceGroundsGuardsArrestParty(void);
static void scMcCoyPalaceGroundsOpenDoor(void);

#include "shared/actor_facing.c"

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return McCoyPalaceGroundsEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return McCoyPalaceGroundsExits;
}

/* The people outside McCoy's Palace. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(MainRoutineStateWork.next == SCENE_MC_COY_PALACE_GROUNDS) {
		if(gmFlagGet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) != 0)
			McCoyPalaceGroundsActorPlacements[ACTOR_EAST_GUARD -
				ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY_AND_BACK;
		if(gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) != 0)
			McCoyPalaceGroundsActorPlacements[ACTOR_LADY_MCCOY -
				ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY;
		return McCoyPalaceGroundsActorPlacements;
	}
	return McCoyPalaceGroundsNoActorPlacements;
}

/* Catch on the Nut in the orange tree west of the steps pulls it down and gives
   it to the party. */
static void scMcCoyPalaceGroundsCatchFloatingNut(void)
{
	scCatchNut(ACTOR_FLOATING_NUT, FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT);
}

/* Searching the stone well by the palace's east corner (metatiles (28, 16) to
   (29, 17)): "Isaac peered into the well... Upon closer inspection, it seems
   the well is dried up." Nothing is found. */
static void scMcCoyPalaceGroundsSearchWell(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgFieldPeeredWell, 1);
	mnFieldMessageShow(MsgBilibinUponCloserInspectionSeemsDried, 1);
	fdScriptEnd();
}

/* What the party can talk to, search and catch outside McCoy's Palace. */
const CUE *scGetCues(void)
{
	if(MainRoutineStateWork.next == SCENE_MC_COY_PALACE_GROUNDS) {
		return McCoyPalaceGroundsCues;
	}
	return McCoyPalaceGroundsNoCues;
}

/* Before the curse is lifted, the wandering worker asks whether the party has
   seen the barricade they built on the road to Kolima. On yes he admits it is
   pretty shabby, built on short notice; on no he says it is off to the east. */
static void scMcCoyPalaceGroundsTalkToNorthWorker(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinHaveYouSeenBarricadeWe);
	/* "Have you seen the barricade we built on the road to Kolima?" Yes: "It's
	   pretty shabby, because it was built on short notice......" No: "It's off
	   to the east, if you want to see it. It was a rush..." */
	fdScriptAskYesNo(ACTOR_NORTH_WORKER, 0);
	fdScriptEnd();
}

/* Before the curse is lifted, Lady McCoy asks "Are you telling me that I'm
   responsible for this?" On yes she owns that she had the tree cut down for her
   palace, but calls herself the victim; on no she calls the curse a foolish old
   superstition. */
static void scMcCoyPalaceGroundsTalkToLadyMcCoy(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinTellingMeImResponsibleFor);
	/* "Are you telling me that I'm responsible for this?" Yes: "Sure, I had
	   them cut the tree down so I could build my..." No: "That curse of the
	   tree spirit is a foolish old superstition!..." */
	fdScriptAskYesNo(ACTOR_LADY_MCCOY, 0);
	fdScriptEnd();
}

/* After the reward, the west guard asks "Do you think I can become as fine a
   warrior as you?" On yes he will try his best; on no he begs Isaac to just say
   yes. */
static void scMcCoyPalaceGroundsTalkToWestGuard(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinDoThinkCanBecomeAs);
	/* "Do you think I can become as fine a warrior as you?" Yes: "Really? Then
	   I will try my best!" No: "Don't be so harsh... Just say yes, even if you
	   don't mean it." */
	fdScriptAskYesNo(ACTOR_WEST_GUARD, 0);
	fdScriptEnd();
}

/* The east guard at the palace door asks whether the party is up to meeting his
   lord. On yes he and the west guard confer, decide nobody else seems up to the
   challenge, and the west guard says "All right. I shall let you pass. Follow
   me!", opens the door and goes in; on no the guard says he cannot send such
   inexperienced children. Once the party is let in, he warns that Lord McCoy is
   a very difficult man, and keeps turning to watch Isaac. After McCoy has
   turned the party away, he only talks.

   Unused: the branch for a party back from Bilibin never runs. In it he scolds
   the party once for keeping McCoy waiting (MsgBilibinWhere, `Where were you?`,
   and MsgBilibinMasterQuiteCranky). The branch is never reached because
   FLAG_KEEPING_MCCOY_WAITING is cleared on entering the grounds. */
static void scMcCoyPalaceGroundsTalkToEastGuard(void)
{
	fdScriptStart();
	/* Once the party is let in, the guard only warns it about McCoy. */
	if(fdScriptGetFlag(FLAG_MCCOY_PALACE_ADMITTED) != 0) {
		if(gmFlagGet(FLAG_KEEPING_MCCOY_WAITING) != 0) {
			if(fdScriptGetFlag(FLAG_GUARD_ASKED_WHERE_YOU_WERE) == 0) {
				fdScriptSetMessageId(MsgBilibinWhere);
				/* "Where were you?" */
				fdScriptShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
				fdActorShowEmote(
					ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
				/* "Lord McCoy is angry because he couldn't find you." */
				fdScriptShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
				fdActorRunRepeatedMotion(ACTOR_EAST_GUARD, 2);
				gmFlagSet(FLAG_GUARD_ASKED_WHERE_YOU_WERE);
			}
			fdScriptSetMessageId(MsgBilibinMasterQuiteCranky);
			/* "The master can be quite cranky. Hurry along, now!" */
			fdScriptShowMessage(ACTOR_EAST_GUARD, 0);
		} else {
			fdScriptSetMessageId(MsgBilibinLordMccoyDifficult);
			/* "Lord McCoy is a very difficult man. Be careful how you
			   act..." */
			fdScriptShowMessage(ACTOR_EAST_GUARD, 0);
			syFieldActorWatchAndStartScript(ACTOR_EAST_GUARD,
				ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
				McCoyPalaceGroundsGuardWatchesParty);
		}
	} else {
		/* On the first visit, the guards confer before opening the door. */
		fdScriptSetMessageId(MsgBilibinLordOnlyMeet);
		/* "The lord will only meet with brave and mighty warriors. You..." */
		fdScriptOpenMessage(ACTOR_EAST_GUARD, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			FieldStateWork->msg++;
			/* "Hmmm, I see... You seem pretty confident..." */
			fdScriptShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
			if(fdActorGet(ACTOR_PARTY_LEADER)->z.fixed < toFIXED(270)) {
				fdActorSetSpeed(ACTOR_EAST_GUARD, 0.8);
				fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 346, 274);
				fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 328, 282);
				fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
			}
			fdActorFaceDirection(ACTOR_WEST_GUARD, FACING_EAST_SOUTHEAST, 0);
			fdActorFaceDirection(ACTOR_EAST_GUARD, FACING_WEST_SOUTHWEST, 20);
			fdActorShowEmote(
				ACTOR_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
			fdActorStartRepeatedMotion(ACTOR_WEST_GUARD, 1);
			/* "What do you say? They look a little young to get involved
			   in..." */
			fdScriptShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
			fdActorShowEmote(
				ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
			fdActorStartRepeatedMotion(ACTOR_EAST_GUARD, 1);
			/* "Nobody else seems up to the challenge, though. What do we..." */
			fdScriptShowMessageThenWait(ACTOR_EAST_GUARD, 0, 20);
			fdActorSetAnimation(ACTOR_WEST_GUARD, ANIM_NOD);
			fdActorSetAnimationAndWait(ACTOR_EAST_GUARD, ANIM_NOD);
			fdActorFaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH_SOUTHEAST, 0);
			fdActorFaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH_SOUTHWEST, 10);
			fdActorStartRepeatedMotion(ACTOR_WEST_GUARD, 1);
			/* "All right. I shall let you pass. Follow me!" */
			fdScriptShowMessageThenWait(ACTOR_WEST_GUARD, 0, 20);
			fdActorFaceDirection(ACTOR_WEST_GUARD, FACING_EAST_NORTHEAST, 0);
			fdActorSetSpeed(ACTOR_EAST_GUARD, 1);
			fdActorGet(ACTOR_EAST_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
			fdActorWalkToAndWait(ACTOR_EAST_GUARD, 346, 263);
			fdScriptWait(1);
			fdActorGet(ACTOR_EAST_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
			fdActorSetSpeed(ACTOR_WEST_GUARD, 0.6);
			fdActorWalkToAndWait(ACTOR_WEST_GUARD, 328, 263);
			fdActorWalkToAndWait(ACTOR_WEST_GUARD, 328, 252);
			fdActorFaceDirection(ACTOR_WEST_GUARD, FACING_NORTH, 10);
			scMcCoyPalaceGroundsOpenDoor();
			fdActorWalkToAndWait(ACTOR_WEST_GUARD, 328, 246);
			fdActorSetPos(ACTOR_WEST_GUARD, 0, 0);
			gmFlagSet(FLAG_MCCOY_PALACE_ADMITTED);
			syFieldActorWatchAndStartScript(ACTOR_EAST_GUARD,
				ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
				McCoyPalaceGroundsGuardWatchesParty);
		} else {
			fdScriptShowMessage(ACTOR_EAST_GUARD, 0);
			fdActorFaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH_SOUTHEAST, 10);
		}
	}
	fdScriptEnd();
}

/* Walking north into the palace doorway (metatile (20, 16)), once the guards
   have let the party in and stepped aside: the door swings open if it is shut,
   Isaac walks through, and the palace's hall appears. */
static void scMcCoyPalaceGroundsEnterPalace(void)
{
	fdScriptStart();
	if(gmFlagGet(FLAG_DOOR_OPEN) == 0) {
		scMcCoyPalaceGroundsOpenDoor();
	}
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
	fdScriptWait(16);
	syImportScriptSetExitRequest(MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE);
	fdScriptEnd();
}

/* Sets up the grounds outside McCoy's Palace, and plays the guards' arrest once
   the curse is lifted. */
Sint32 scInit(void)
{
	FIELD_STATE *state = FieldStateWork;
	ACTOR		*actor;

	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

	actor = fdActorGet(ACTOR_WEST_WORKER);
	actor->priorityFlags = 0;
	actor->sprite->image.priority = 1;

	if(MainRoutineStateWork.next == SCENE_MC_COY_PALACE_GROUNDS) {
		scMcCoyPalaceGroundsRestoreStory();
	}

	return 0;
}

/* Places the people by the story. After the guards let the party in, the west
   guard is gone inside and the east guard stands aside, watching Isaac, until
   McCoy has turned the party away; then both stand either side of the door.
   Once the curse is lifted, Lady McCoy moves to the west edge of the yard and
   the worker by the new palace turns east, and the guards' arrest plays until
   McCoy has rewarded the party. The Nut is hung in its tree until it is
   caught. */
static void scMcCoyPalaceGroundsRestoreStory(void)
{
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
		gmFlagClear(FLAG_DOOR_OPEN);
	}
	if(gmFlagGet(FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT) == 0) {
		scFloatNut(ACTOR_FLOATING_NUT);
	}
	if(gmFlagGet(FLAG_MCCOY_PALACE_ADMITTED) != 0) {
		fdActorSetPos(ACTOR_WEST_GUARD, 308, 263);
		fdActorSetPos(ACTOR_EAST_GUARD, 346, 263);
		if(gmFlagGet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
			if(gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
				fdActorSetPos(ACTOR_WEST_GUARD, 0, 0);
				syFieldActorWatchAndStartScript(ACTOR_EAST_GUARD,
					ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
					McCoyPalaceGroundsGuardWatchesParty);
			}
		}
	}
	/* Lady McCoy's new place and the arrest come only after the curse lifts. */
	if(gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
		fdActorSetPos(ACTOR_LADY_MCCOY, 224, 292);
		fdActorFaceDirection(ACTOR_LADY_MCCOY, FACING_SOUTH, 0);
		fdActorFaceDirection(ACTOR_WEST_WORKER, FACING_EAST, 0);
		if(gmFlagGet(FLAG_MCCOY_PALACE_PARTY_REWARDED) == 0) {
			scMcCoyPalaceGroundsGuardsArrestParty();
		}
	}
}

/* The first time the party climbs the steps after lifting Kolima's curse, the
   guards cry "Oh! Sir!" and "We've been looking everywhere for you!", and ask
   "The curse of the holy tree... You're the ones who broke it, right?" Each no
   brings "That's not true. We have witnesses who say they saw you in the
   forest!" and the question again. On yes the west guard declares "You're under
   arrest!", opens the door and marches Isaac straight into McCoy's room. */
static void scMcCoyPalaceGroundsGuardsArrestParty(void)
{
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraMoveTo(320, CAMERA_KEEP, 320, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 320, 372);
	fdScriptStartScreenTransition();
	fdCameraSetSpeed(0.2);
	fdCameraMoveTo(320, CAMERA_KEEP, 290, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 320, 310);
	fdActorFaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH_SOUTHEAST, 10);
	fdActorStartRepeatedMotion(ACTOR_WEST_GUARD, 2);
	fdActorShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	fdScriptSetMessageId(MsgBilibinOhItS);
	/* "Oh! It's..." */
	fdScriptShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
	fdActorFaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH_SOUTHWEST, 10);
	fdActorStartRepeatedMotion(ACTOR_EAST_GUARD, 2);
	fdActorShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	/* "Oh! Sir!" */
	fdScriptShowMessageThenWait(ACTOR_EAST_GUARD, 0, 20);
	fdActorFaceDirection(ACTOR_WEST_GUARD, FACING_EAST_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_EAST_GUARD, FACING_WEST_SOUTHWEST, 40);
	fdActorFaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH_SOUTHWEST, 10);
	fdActorRunRepeatedMotion(ACTOR_WEST_GUARD, 1);
	/* "We've been looking everywhere for you!" */
	fdScriptShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_EAST_GUARD, 1);
	/* "The curse of the holy tree... You're the ones who broke it,..." */
	fdScriptOpenMessage(ACTOR_EAST_GUARD, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	/* Each No brings the accusation again, until Isaac says yes. */
	while(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
		fdActorShowEmote(
			ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
		fdScriptSetMessageId(MsgBilibinNotTrueWitnesses);
		/* "That's not true. We have witnesses who say they saw you in..." */
		fdScriptShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
		fdActorStartRepeatedMotion(ACTOR_EAST_GUARD, 2);
		/* "It was you, wasn't it?" */
		fdScriptOpenMessage(ACTOR_EAST_GUARD, 0);
	}
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH_SOUTHWEST, 20);
	fdActorSetAnimation(ACTOR_WEST_GUARD, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_EAST_GUARD, ANIM_NOD);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_WEST_GUARD, 1);
	fdScriptSetMessageId(MsgBilibinUnderArrest);
	/* "You're under arrest!" */
	fdScriptShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
	fdActorSetSpeed(ACTOR_WEST_GUARD, 1);
	fdActorWalkToAndWait(ACTOR_WEST_GUARD, 314, 280);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
	/* "Let's go. Lord McCoy is waiting." */
	fdScriptShowMessageThenWait(ACTOR_WEST_GUARD, 0, 40);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	if(gmFlagGet(FLAG_MCCOY_PALACE_ADMITTED) == 0) {
		fdActorSetSpeed(ACTOR_EAST_GUARD, 1);
		fdActorGet(ACTOR_EAST_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
		fdActorWalkToAndWait(ACTOR_EAST_GUARD, 346, 263);
		fdScriptWait(1);
		fdActorGet(ACTOR_EAST_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
	}
	fdActorSetSpeed(ACTOR_WEST_GUARD, 0.8);
	/* The west guard marches Isaac through the door into the palace. */
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorWalkTo(ACTOR_WEST_GUARD, 328, 262);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 328, 278);
	fdActorSetAnimation(ACTOR_WEST_GUARD, ANIM_STAND);
	scMcCoyPalaceGroundsOpenDoor();
	fdScriptWait(40);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 328, 242);
	fdActorWalkToAndWait(ACTOR_WEST_GUARD, 328, 242);
	fdActorSetPos(ACTOR_WEST_GUARD, 0, 0);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE_EAST);
	fdScriptEnd();
}

/* The palace door swings open with a thump. */
static void scMcCoyPalaceGroundsOpenDoor(void)
{
	sySoundPlay(SE_THUMP);
	fdMapGridPlayMapEdit(McCoyPalaceGroundsDoorOpenSteps, 52, 11);
	gmFlagSet(FLAG_DOOR_OPEN);
}

#include "shared/floating_nut.c"

/* clang-format off */
/* The door's opening, drawn in three steps of 5 ticks each from the spare */
/* metatiles at (34, 25) to (36, 27). */
Uint16 McCoyPalaceGroundsDoorOpenSteps[] = {
	MAPGRID_ANIMATION_STEP(34, 25, 1, 3, 5), MAPGRID_ANIMATION_STEP(35, 25, 1, 3, 5), MAPGRID_ANIMATION_STEP(36, 25, 1, 3, 5), MAPGRID_ANIMATION_STEPS_END,
};

/* A guard turns to keep facing Isaac. */
Sint32 McCoyPalaceGroundsGuardWatchesParty[] = {
	SC_CALL(scTurnTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

SCENE_ENTRANCE McCoyPalaceGroundsEntrances[] = {
	{ MCCOY_PALACE_GROUNDS_FROM_BILIBIN, CONDITION_ALWAYS, 320, 0, 328, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ MCCOY_PALACE_GROUNDS_FROM_MCCOY_PALACE, CONDITION_ALWAYS, 328, 0, 272, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

Uint32 McCoyPalaceGroundsExits[] = {
	SCENE_EXITS(McCoyPalaceGrounds),
	SCENE_EXIT(MCCOY_PALACE_GROUNDS_TO_BILIBIN, Bilibin, BILIBIN_FROM_MCCOY_PALACE_GROUNDS),
	SCENE_EXIT(MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE, McCoyPalace, MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_WEST),
	SCENE_EXIT(MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE_EAST, McCoyPalace, MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST),
	SCENE_EXITS_END,
};

/* Unused: the getters return it for any scene but */
/* SceneIdMcCoyPalaceGrounds, and the overlay serves no other. */
ACTOR_PLACE McCoyPalaceGroundsNoActorPlacements[] = {
	{ SCENE_TABLE_END },
};

ACTOR_PLACE McCoyPalaceGroundsActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		103, 0, 280, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		135, 0, 200, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		128, 0, 327, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		320, 0, 263, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		334, 0, 263, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
		216, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
#if MCCOY_PALACE_GROUNDS_IVY
	ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		132, 0, 388, FACING_SOUTH, TALK_FACING_KEEP, 0),
#endif
	{ SCENE_TABLE_END },
};

/* Unused, as McCoyPalaceGroundsNoActorPlacements is. */
CUE McCoyPalaceGroundsNoCues[] = {
	{ CUE_TABLE_END },
};

CUE McCoyPalaceGroundsCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, MCCOY_PALACE_GROUNDS_TO_BILIBIN },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE, CONDITION_ALWAYS, SCRIPT_REF(scMcCoyPalaceGroundsEnterPalace) },
	{ CUE_TALK, ACTOR_WEST_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheLumberIOrderedFromKolima) },
	{ CUE_TALK, ACTOR_WEST_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinNowThatTheCurseIsBroken) },
	{ CUE_TALK, ACTOR_NORTH_WORKER, FLAG_KOLIMA_CURSE_LIFTED, SCRIPT_REF(scMcCoyPalaceGroundsTalkToNorthWorker) },
	{ CUE_TALK, ACTOR_NORTH_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinItsOverOverMilordDecidedNot) },
	{ CUE_TALK, ACTOR_LADY_MCCOY, FLAG_KOLIMA_CURSE_LIFTED, SCRIPT_REF(scMcCoyPalaceGroundsTalkToLadyMcCoy) },
	{ CUE_TALK, ACTOR_LADY_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinHmphMyPalaceIsNotFinished) },
	{ CUE_TALK, ACTOR_WEST_GUARD, FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, MSGNO(MsgBilibinLordMccoyIsSoUpsetAbout) },
	{ CUE_TALK, ACTOR_WEST_GUARD, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinICantTellYouAnythingElse) },
	{ CUE_TALK, ACTOR_WEST_GUARD, CONDITION_ALWAYS, SCRIPT_REF(scMcCoyPalaceGroundsTalkToWestGuard) },
	{ CUE_TALK, ACTOR_EAST_GUARD, FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, SCRIPT_REF(scMcCoyPalaceGroundsTalkToEastGuard) },
	{ CUE_TALK, ACTOR_EAST_GUARD, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinWhatAShameHuhIGuess) },
	{ CUE_TALK, ACTOR_EAST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBilibinICouldNeverHaveGoneTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIfThingsLikeThisAreGoing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinWedStillBeWorkingIfWe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_NORTH_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIAmGladIDidntKnow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_NORTH_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinThatWasCloseBuildingACursed) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LADY_MCCOY, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIfAllThisTroubleWasCaused) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LADY_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinHeSaidHeStoppedConstructionSo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, FLAG_MCCOY_PALACE_ADMITTED, MSGNO(MsgBilibinNoOneBelievedTheyWouldTurn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinThatBarricadeIsPrettyShabbyIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBilibinIWillBeAReliableWarrior) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, FLAG_MCCOY_PALACE_ADMITTED, MSGNO(MsgBilibinIsThereNoOneWhoCan) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, MSGNO(MsgBilibinLordMccoyWouldNeverHireAnyone) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIDidntThinkTheydLetYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBilibinDontLeaveIsaacBeMyMentor) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_MCCOY_PALACE_GROUNDS_WELL, CONDITION_ALWAYS, SCRIPT_REF(scMcCoyPalaceGroundsSearchWell) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FLOATING_NUT, FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT, SCRIPT_REF(scMcCoyPalaceGroundsCatchFloatingNut) },
	{ CUE_TABLE_END },
};
/* clang-format on */
