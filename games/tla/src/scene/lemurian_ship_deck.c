/* The Lemurian ship, in two scenes: the East Indra Shore, where the ship lies
   moored below the cliffs (SceneIdLemurianShipMainDeck), and its deck at the
   tiller once it sails (SceneIdLemurianShipTiller).

   The party first comes down the sandy path to the shore before the ceremony at
   the Gabomba Statue, and Kraden takes the ship for Menardi's, drifted there
   with no orb to sail it. A puddle on the path west of the ship freezes with
   Frost into a pillar of ice to the wooden box on the ledge. Back with Piers
   after the ceremony, he agrees on the shore to take the party to Lemuria once
   he has seen the mayor of Madra; boarding by the rope ladder on the ship's
   side, he holds the Black Crystal up to the cabin door, which opens, and leads
   the way down to the engine (lemurian_ship.c). At sea the deck by the tiller
   plays Piers teaching Felix to sail, and later the party's talk after a
   whirlpool of the Sea of Time throws the ship back. */

#include "graphics/palette_buffers.h"
#include "item_ids.h"
#include "field/field_mode.h"
#include "game/game.h"
#include "graphics/glyph.h"
#include "field/script_inline.h"
#include "field/field_mode_script.h"
#include "scene/scene.h"
#include "edition.h"
#include "macros.h"
#include "field/field_sprite.h"
#include "field/call.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

Sint32 LemurianShipDeckBobPhase	  BSS_DATA; /* angle, TRIG_TURN / 128 a tick */
Sint32 LemurianShipDeckWaveScroll BSS_DATA; /* 16.16 pixels, to 32 */

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

/* The deck's actors, by the rows of both of its actor placement tables;
   Jenna's, Sheba's and Piers' rows go to their party slots. */
enum {
	/* SPRITE_PUDDLE, on the path west of the ship */
	ACTOR_PUDDLE = ACTOR_FIRST_PLACED,
	ACTOR_KRADEN
};

/* The shore's metatiles, as map.txt has them; the cabin door's step metatiles
   are the exit to the cabins and leave by it. */
typedef enum LEMURIAN_SHIP_DECK_METATILE_BEHAVIOR {
	/* The head of the sandy path down to the shore, metatiles (14, 4) to (19,
	   4). */
	METATILE_BEHAVIOR_HEAD_OF_PATH = 10,
	/* The row below it, metatiles (12, 5) to (21, 5). */
	METATILE_BEHAVIOR_TOP_OF_SHORE,
	/* The shallows at the foot of the rope ladder on the ship's side, metatile
	   (27, 27). */
	METATILE_BEHAVIOR_FOOT_OF_LADDER,
	/* The cabin door's face, metatile (22, 24), which Felix checks. */
	METATILE_BEHAVIOR_CABIN_DOOR = 14,
	/* The deck east of the cabin door, metatiles (72, 25) and (72, 26). */
	METATILE_BEHAVIOR_EAST_OF_DOOR,
	/* The metatiles either side of those, (71, 25), (73, 25), (71, 26) and (73,
	   26). */
	METATILE_BEHAVIOR_BESIDE_EAST_OF_DOOR
} LEMURIAN_SHIP_DECK_METATILE_BEHAVIOR;

#define FLAG_PUDDLE_FROZEN \
	FLAG_SCENE_TEMP_00 /* Frost froze the path's puddle */
/* Piers opened it this visit */
#define FLAG_CABIN_DOOR_OPEN		  FLAG_SCENE_TEMP_01
/* Felix's first and second steps east of the cabin door, while Kraden waits. */
#define FLAG_FIRST_STEP_EAST_OF_DOOR  FLAG_SCENE_TEMP_10
#define FLAG_SECOND_STEP_EAST_OF_DOOR FLAG_SCENE_TEMP_11
/* Felix stands east of the cabin door; stepping beside it clears this. */
#define FLAG_STANDING_EAST_OF_DOOR	  FLAG_SCENE_TEMP_12

/* Draws the cabin door open, from spare metatiles of the map at (51, 38). */
#define scLemurianShipDeckDrawCabinDoorOpen() \
	fdMapGridCopyMetatilesTo(51, 38, 70, 22, 1, 2)

/* Draws plain hull over the metatiles at (67, 16) of the ship's middle layer:
   the copy that Lemuria's dock makes there among those that hide the ship's
   wings until it can fly (lemuria_dock.c), one metatile wider. */
#define scLemurianShipDeckHideWing() \
	fdMapGridCopyMetatilesTo(60, 53, 67, 16, 7, 3)

extern ACTOR_PLACE LemurianShipDeckPlacements[];
extern ACTOR_PLACE LemurianShipDeckTillerPlacements[];

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_LEMURIAN_SHIP_TILLER)
		return LemurianShipDeckTillerPlacements;
	return LemurianShipDeckPlacements;
}

/* Felix walks north into the cabin door on the ship's deck, metatile (22, 25),
   once Piers has opened it: he steps into the doorway and the screen closes on
   the cabins below. Before then nothing happens.

   If the door is not yet open on this visit (`FLAG_CABIN_DOOR_OPEN`), it swings
   open first. Every placed actor stops where it is. */
void scLemurianShipDeckEnterCabin(void)
{
	if(fdScriptGetFlag(FLAG_PIERS_BACK_ON_HIS_SHIP)) {
		FIELD_STATE *state = RamHeapSlots->fieldState;
		Uint32		 i;

		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		for(i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
			ACTOR *actor = fdActorGet(i);

			if(actor != 0) {
				actor->motionFlg = 0;
				actor->y.fixed = toFIXED(32);
			}
		}
		if(!fdScriptGetFlag(FLAG_CABIN_DOOR_OPEN)) {
			fdScriptPlaySnd(SE_DOOR_OPEN);
			scLemurianShipDeckDrawCabinDoorOpen();
		}
		fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
		fdActorCenterAndWalk(MainRoutineStateWork.leaderActorId, 2, -4);
		syFieldScriptWait(8);
		fdScriptPlaySnd(SE_MAP_EXIT);
		syFieldScriptSetExitRequest(state->stepCue);
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionTicks();
		fdScriptEnd();
	}
}

extern CUE LemurianShipDeckCues[];
extern CUE LemurianShipDeckMainDeckCues[];

const CUE *scGetCues(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_LEMURIAN_SHIP_MAIN_DECK)
		return LemurianShipDeckMainDeckCues;
	return LemurianShipDeckCues;
}

/* Felix checks the cabin door on the ship's deck, metatile (22, 24), before
   Piers has come aboard: "The door is held closed by a powerful force." */
void scLemurianShipDeckTryHeldDoor(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	mnMessageShowCentered(MsgLemurianShipDeckDoorIsHeldClosed, 1);
	fdScriptEnd();
}

/* Defines an event that moves colours last-1 .. first of LEMURIAN_SHIP_PALETTE
   one place up and the last round to first, every eighth tick. */
#define DEFINE_ROTATE_PALETTE_COLORS(name, last, first) \
	void name(void) \
	{ \
		if((TickCount & 7) == 0) { \
			Sint16 saved = BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, last); \
			Sint32 i; \
\
			for(i = last; i != first; i--) \
				BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, i) = \
					BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, i - 1); \
			BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, first) = saved; \
		} \
	}

/* An event: turns the surf's colours round by one. */
DEFINE_ROTATE_PALETTE_COLORS(
	LemurianShipDeckRotateSurfColors, SURF_LAST_COLOR, SURF_FIRST_COLOR)

/* An event: turns the blues of the band along the hull round by one. */
DEFINE_ROTATE_PALETTE_COLORS(LemurianShipDeckRotateHullBandColors,
	HULL_BAND_LAST_COLOR, HULL_BAND_FIRST_COLOR)

/* The entrance whose arrival lights the band along the hull. */
#define MAIN_DECK_ARRIVAL_ENTRANCE 99

enum {
	/* The map's two tile animations, held until that arrival, that light the
	   band one after the other. */
	HULL_BAND_FIRST_ANIMATION = 13,
	HULL_BAND_SECOND_ANIMATION
};

/* The party arrives on the shore by MAIN_DECK_ARRIVAL_ENTRANCE: the screen
   opens and the blue band along the ship's hull lights up in two steps.

   No exit table or script in the game names entrance 99 of the main deck: the
   cabins' and the world map's exits lead in by entrances 1 and 2, so this
   arrival is reached by none of them. */
void scLemurianShipDeckShowArrival(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptStartScreenTransition();
	syFieldSetTickEvent(LemurianShipDeckRotateHullBandColors, EVENT_PASS_TICK);
	fdScriptWaitTransitionTicks();
	syFieldScriptWait(60);
	fdTilesetAnimResume(HULL_BAND_FIRST_ANIMATION);
	syFieldScriptWait(60);
	syFieldScriptWait(60);
	fdTilesetAnimResume(HULL_BAND_SECOND_ANIMATION);
	fdScriptEnd();
}

/* An event: scrolls the waves' layer half a pixel a tick. */
void scLemurianShipDeckScrollWaves(void)
{
	MAP_LAYER *layer = fdMapGridGetLayer(0);
	FIXED	   phase = LemurianShipDeckWaveScroll;

	layer->ofsX = phase;
	phase += toFIXED(1) / 2;
	LemurianShipDeckWaveScroll = phase;
	if(phase == toFIXED(32))
		LemurianShipDeckWaveScroll = 0;
}

/* An event: bobs the camera's actor and the first map layer together on a sine
   wave, a full wave every 128 ticks. */
void scLemurianShipDeckBobDeck(void)
{
	MAP_LAYER *layer = fdMapGridGetLayer(0);
	ACTOR	  *actor = syScriptCameraGetActor();
	Sint32	   ofs = syMathSin(LemurianShipDeckBobPhase) >> 2;

	actor->y.fixed += ofs;
	layer->ofsY += ofs;
	LemurianShipDeckBobPhase += TRIG_TURN / 128;
}

extern Sint32 LemurianShipDeckJennaLooksAround[];

extern Sint32 LemurianShipDeckShebaLooksAround[];

extern Sint32 LemurianShipDeckKradenLooksAround[];

extern Sint16 LemurianShipDeckPuddles[];

extern Uint32 LemurianShipDeckSparkleFades[];
extern Sint32 LemurianShipDeckReturnToShipScript[];

extern Sint32 LemurianShipDeckReachPiersShipScript[];
extern Uint32 LemurianShipDeckSplashFades[];

/* Places an actor facing an angle, the angle taken as the Uint16 an actor's
   facing is, as Madra's and Loho's scenes also place them. */
static inline void scLemurianShipDeckPlaceFacing(
	Sint32 id, Sint32 x, Sint32 z, Uint16 facing)
{
	fdActorSetPosAndFacing(id, x, z, facing);
}

/* The first time the ship sails, on the deck by the tiller: Piers tells Felix,
   "Take the tiller, Felix!", the party talks over where to go (Felix's answer
   to Piers's wish to see Vale changes Kraden's reply), and Kraden ends, "To the
   open sea, Felix!" The player then steers the ship on the world map.

   It runs from entrance 10 and sets FLAG_SHIP_CAN_SAIL. */
void scLemurianShipDeckTakeTheTiller(void)
{
	Sint32 turn;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraGetActor()->motionFlg = 0;
	fdCameraMoveTo(342, 48, 408, 0);
	syWait(1);
	scLemurianShipDeckHideWing();
	scLemurianShipDeckPlaceFacing(MainRoutineStateWork.leaderActorId,
		toFIXED(552), toFIXED(376), FACING_EAST);
	scLemurianShipDeckPlaceFacing(
		ACTOR_KRADEN, toFIXED(558), toFIXED(345), FACING_NORTH_NORTHEAST);
	scLemurianShipDeckPlaceFacing(
		ACTOR_PIERS, toFIXED(534), toFIXED(404), FACING_EAST);
	turn = FACING_SOUTHEAST;
	scLemurianShipDeckPlaceFacing(
		ACTOR_SHEBA, toFIXED(568), toFIXED(408), turn);
	scLemurianShipDeckPlaceFacing(
		ACTOR_JENNA, toFIXED(546), toFIXED(440), FACING_SOUTH);
	fdMapGridCenterOnFollowedPoint();
	syWait(1);
	fdActorSetScript(ACTOR_JENNA, LemurianShipDeckJennaLooksAround);
	fdActorSetScript(ACTOR_SHEBA, LemurianShipDeckShebaLooksAround);
	fdActorSetScript(ACTOR_KRADEN, LemurianShipDeckKradenLooksAround);
	fdScriptStartScreenTransition();
	LemurianShipDeckWaveScroll = 0;
	sySetTickEvent(scLemurianShipDeckScrollWaves, EVENT_PASS_TICK);
	LemurianShipDeckBobPhase = 0;
	sySetTickEvent(scLemurianShipDeckBobDeck, EVENT_PASS_TICK);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(542, 44, 396, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(80);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
	syFieldScriptSetMessageId((Sint32)MsgTakeTheTiller);
	/* "Take the tiller, Felix!" */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
		EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	FieldStateWork->msg++;
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_SOUTHWEST, 20);
	/* "If you hope to reach Lemuria, you'll have to learn a few things..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	scFieldActorStop(ACTOR_JENNA);
	scFieldActorStop(ACTOR_KRADEN);
	scFieldActorStop(ACTOR_SHEBA);
	syWait(1);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "Kraden, shouldn't we be heading straight for Lemuria?" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, turn);
	/* "Well, I suppose if we have to learn to sail, it's better to do it..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "Then I want to go to Lalivero! And Tolbi! Oh! And Vale! Can we go
	   to..." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	/* "I'd love to see your hometown, Jenna! Shall we? Let's do it! To
	   Vale..." */
	fdScriptOpenMessage(ACTOR_PIERS, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 0) {
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
		/* Yes: "I don't think that's a good idea." */
		fdScriptShowMessage(ACTOR_KRADEN, 0);
		FieldStateWork->msg++;
	} else {
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
		FieldStateWork->msg++;
		/* No: "I'm with Felix. I don't think that would be a wise course
		   of..." */
		fdScriptShowMessage(ACTOR_KRADEN, 0);
	}
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	/* "Come, now! Why not?" */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	/* "Think about the roads we have traveled to get where we are..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "I can't... I was never any good at geography." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
	/* "Then allow me to explain... The coastline of Angara, the
	   continent..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "Oh... You don't think Piers's boat could land there?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	/* "And as for Gondowan, I would guess the people from Lalivero..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	/* "He's right. Saturos and Menardi went a little wild in Lalivero,
	   and..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_NORTHEAST);
	/* "Oh... But Sheba can explain the situation for us. Faran and the..." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorSetAttachedEffect(
		ACTOR_SHEBA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHWEST);
	/* "And then they'd make me stay in Lalivero! Would you just leave me..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
	/* "Oh yeah... Maybe that wouldn't be such a good idea after all..." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	/* "Then it's decided!" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "What's decided?" */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "The sea is vast... We can go wherever we please! So, let's explore
	   a..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	scFieldActorJump(ACTOR_SHEBA, 4, 10);
	/* "That's a great idea! We've already met werewolves and discovered
	   new..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	scFieldActorJump(ACTOR_JENNA, 2, 10);
	scFieldActorJump(ACTOR_JENNA, 4, 10);
	/* "Wow! This is going to be great!" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	/* "Hmm... The seas can be quite dangerous. It won't be my fault if..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	/* "To the open sea, Felix!" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	MainRoutineStateWork.movementMode = LEADER_MOVEMENT_SHIP;
	MainRoutineStateWork.ownerOverride = 1;
	fdScriptSetFlag(FLAG_SHIP_CAN_SAIL);
	syFieldScriptSetExitRequest(LEMURIAN_SHIP_TILLER_TO_WORLD_MAP_C);
}

/* Sends actor id at a run to where Felix stands, and off the deck. */
static inline void scLemurianShipDeckRunToFelix(Sint32 id)
{
	ACTOR *tgt;

	fdActorSetSpeed(id, 1.2);
	fdActorSetAnimation(id, ANIM_WALK);
	tgt = fdActorGetById(ACTOR_FELIX);
	if(tgt != NULL)
		fdActorSetDestination(
			id, tgt->x.part[0].integer, tgt->z.part[0].integer);
	fdActorWaitForMoveById(id);
	fdActorSetPos(id, 0, 0);
}

/* The first time a whirlpool of the Sea of Time throws the ship back, the party
   stands round Felix at the tiller as the deck rocks: Sheba cries "Hold on to
   that tiller!", Piers admits he has never had to find his way back into
   Lemuria, and Kraden says they will need a way past the currents. The others
   then run to Felix and the ship is back on the world map.

   It runs from entrance 20; the waves scroll and the deck bobs throughout. */
void scLemurianShipDeckHoldOnToTiller(void)
{
#if !BOB_DECK_DEAD_CALL_DROPPED
	/* FAKEMATCH(dead): a call that never runs. */
	if(0)
		sySetTickEvent(scLemurianShipDeckBobDeck, EVENT_PASS_TICK);
#endif
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraGetActor()->motionFlg = 0;
	fdCameraMoveTo(458, 48, 408, 0);
	syFieldScriptSetMessageId(MsgLemurianShipDeckHoldOnToThatTiller);
	fdActorSetPosAndFacing(
		ACTOR_FELIX, toFIXED(470), toFIXED(392), FACING_EAST);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, 0, -24, FACING_SOUTH);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, -16, -16, FACING_SOUTHEAST);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, -24, 8, FACING_EAST_NORTHEAST);
	scFieldActorLaunchFromLeader(ACTOR_PIERS, -4, 24, FACING_NORTHEAST);
	scLemurianShipDeckHideWing();
	fdMapGridCenterOnFollowedPoint();
	syWait(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	LemurianShipDeckWaveScroll = 0;
	sySetTickEvent(scLemurianShipDeckScrollWaves, EVENT_PASS_TICK);
	LemurianShipDeckBobPhase = 0;
	sySetTickEvent(scLemurianShipDeckBobDeck, EVENT_PASS_TICK);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTH);
	syFieldScriptWait(10);
	/* "Hold on to that tiller! Do you want us to get swallowed by the
	   waves!?" */
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_NORTHEAST);
	syFieldScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	/* "I know you're worried, Sheba, but he's doing the best he can..." */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 5);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
	syFieldScriptWait(10);
	/* "That was an incredibly strong whirlpool, but I doubt it could
	   sink..." */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
	syFieldScriptWait(30);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_NORTHEAST);
	syFieldScriptWait(10);
	/* "See, Sheba? There's nothing to worry about. Even if we hit a..." */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHWEST);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTH);
	/* "But still... Piers, can't you show Felix how to get past the..." */
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 5);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
	syFieldScriptWait(10);
	/* "Er, yes, well..." */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTHEAST);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "What's wrong, Piers? Lemuria's just beyond these whirlpools, isn't
	   it?" */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHWEST);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Yeah, I...think so..." */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorFaceEachOther(ACTOR_SHEBA, ACTOR_JENNA, 0);
	syFieldScriptWait(30);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "You..."think so"!?" */
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldScriptWait(5);
	/* "Aren't you...from Lemuria? Shouldn't you know exactly where it is?" */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHWEST);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	syFieldScriptWait(10);
	/* "Yes, I'm from Lemuria... I've never had to get back into it
	   before!..." */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 5);
	/* "Oh, I see. So that must mean... You don't actually know which..." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	syFieldScriptWait(10);
	/* "Don't be silly! I mean... I just need to get my bearings." */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Hm... That doesn't sound good." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH);
	syFieldScriptWait(10);
	/* "If Lemuria really is beyond those currents, then we'll need to
	   find..." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTHEAST);
	syFieldScriptWait(10);
	/* "And until we do..." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH);
	syFieldScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "We won't be getting into Lemuria." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldScriptWait(10);
	fdActorFaceEachOther(ACTOR_KRADEN, ACTOR_PIERS, 0);
	fdActorFaceEachOther(ACTOR_SHEBA, ACTOR_JENNA, 0);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	syFieldScriptWait(40);
	scLemurianShipDeckRunToFelix(ACTOR_PIERS);
	scLemurianShipDeckRunToFelix(ACTOR_JENNA);
	scLemurianShipDeckRunToFelix(ACTOR_SHEBA);
	scLemurianShipDeckRunToFelix(ACTOR_KRADEN);
	syFieldScriptSetExitRequest(LEMURIAN_SHIP_TILLER_TO_WORLD_MAP_A);
}

/* The deck by the tiller (SCENE_LEMURIAN_SHIP_TILLER): the surf and the band
   along the hull shimmer, and entrances 10 and 20 play their scenes. */
void scLemurianShipDeckSetUpTillerDeck(void)
{
	fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	syFieldSetTickEvent(LemurianShipDeckRotateSurfColors, EVENT_PASS_TICK);
	syFieldSetTickEvent(LemurianShipDeckRotateHullBandColors, EVENT_PASS_TICK);
	if(MainRoutineStateWork.entrance == LEMURIAN_SHIP_TILLER_FROM_WORLD_MAP_C)
		scLemurianShipDeckTakeTheTiller();
	if(MainRoutineStateWork.entrance ==
		LEMURIAN_SHIP_TILLER_FROM_SEA_OF_TIME_SWIRLS)
		scLemurianShipDeckHoldOnToTiller();
}

Sint32 scInit(void)
{
	MAP_BITS *map = (MAP_BITS *)RamHeapSlots->mapGrid;

	map->wading = 1;
	if(MainRoutineStateWork.next == SCENE_LEMURIAN_SHIP_TILLER) {
		scLemurianShipDeckSetUpTillerDeck();
	} else {
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
		fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_IGNORES_RISES;
		fdActorSetAnimation(ACTOR_PUDDLE, PUDDLE_ANIMATION_PUDDLE);
		fdFlagBlockPlaceAll(LemurianShipDeckPuddles);
		syFieldSetTickEvent(LemurianShipDeckRotateSurfColors, EVENT_PASS_TICK);
		if(MainRoutineStateWork.entrance == MAIN_DECK_ARRIVAL_ENTRANCE)
			scLemurianShipDeckShowArrival();
		else
			syFieldSetTickEvent(
				LemurianShipDeckRotateHullBandColors, EVENT_PASS_TICK);
	}
	return 0;
}

/* At sea, by the tiller, the ship lies at no pier: open water is drawn over the
   pier and the gangplank the map shares with Lemuria's dock, on its ground and
   overhead layers. Returns 0. */
Sint32 scPrepareMap(void)
{
	if(MainRoutineStateWork.next == SCENE_LEMURIAN_SHIP_TILLER) {
		fdMapGridCopyMetatilesTo(2, 2, 10, 24, 18, 10);
		fdMapGridCopyMetatilesTo(0, 62, 26, 62, 3, 8);
		fdMapGridCopyMetatilesTo(50, 65, 25, 61, 6, 1);
	}
	return 0;
}

/* The first time Felix comes down the sandy path to the East Indra Shore,
   metatiles (14, 4) to (19, 4), before the ceremony at the Gabomba Statue: the
   party gathers, Kraden cries "Look... It's Menardi's ship!", and the view pans
   over the moored ship to its dragon prow and back. Felix answers whether it
   could have drifted here; either way they agree they cannot sail it without
   the orb Saturos had.

   Coming later, after the ceremony, only marks the ship seen
   (`FLAG_MENARDIS_SHIP_SEEN`). */
void scLemurianShipDeckSightMenardisShip(void)
{
	Sint32 answer;
	Sint32 leader;

	if(fdScriptGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) != 0) {
		fdScriptSetFlag(FLAG_MENARDIS_SHIP_SEEN);
		return;
	}
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorSetSpeed(ACTOR_FELIX, 0.8);
	fdActorWalkToAndWait(ACTOR_FELIX, 264, 136);
	syFieldScriptSetMessageId(MsgLemurianShipDeckLookItsMenardisShip);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, 16, -16, FACING_SOUTH);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 32, -16, FACING_SOUTH);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, -16, -16, FACING_SOUTH);
	syFieldScriptWait(80);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
	fdActorSetSpeed(ACTOR_KRADEN, 1.6);
	fdActorWalkToAndWait(ACTOR_KRADEN, 300, 152);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	/* "Look... It's Menardi's ship!" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(200, CAMERA_KEEP, 344, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	fdCameraSetSpeed(1.2);
	fdCameraMoveTo(600, CAMERA_KEEP, 344, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(40);
	fdCameraSetSpeed(6.4);
	fdCameraMoveTo(264, CAMERA_KEEP, 128, 1);
	fdCameraWaitForArrival();
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	/* "It doesn't look like they sailed here, though..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
	/* "Of course not... They were killed at the lighthouse, weren't they?" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	/* "Do you think it could have drifted here?" */
	fdScriptOpenMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST, 0);
	answer = syFieldScriptAskYesNoWithActorAnimation(
		MainRoutineStateWork.leaderActorId, 0);
	leader = MainRoutineStateWork.leaderActorId;
	if(answer == 0) {
		fdActorWaitForAnimationChange(leader);
		fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
		/* Yes: "Yes, Felix, That would be logical..." */
		fdScriptShowMessage(ACTOR_KRADEN, 0);
		FieldStateWork->msg++;
	} else {
		fdActorWaitForAnimationChange(leader);
		syFieldScriptSetMessageId(MsgLemurianShipDeckLookItsMenardisShip + 5);
		/* No: "How else could it have come to be here, Felix?" */
		fdScriptShowMessage(ACTOR_KRADEN, 0);
	}
	fdActorSetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	/* "If we had that thingie Saturos had, we could just take their boat." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
	/* "For all we know, the orb is at the bottom of the sea...with them..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "Well, there's no point in hanging around here all day." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorGet(ACTOR_KRADEN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_KRADEN, 288, 144);
	syFieldScriptWait(1);
	fdActorGet(ACTOR_KRADEN)->steeringFlg |= ACTOR_FACES_MOTION;
	/* "Let's go see about finding ourselves a ship we can use." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetSpeed(ACTOR_SHEBA, 1.2);
	fdActorSetSpeed(ACTOR_JENNA, 1.2);
	{
		ACTOR *tgt;

		fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
		tgt = fdActorGet(ACTOR_FELIX);
		if(tgt != NULL)
			fdActorSetDestination(
				ACTOR_KRADEN, tgt->x.part[0].integer, tgt->z.part[0].integer);
		fdActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
		tgt = fdActorGet(ACTOR_FELIX);
		if(tgt != NULL)
			fdActorSetDestination(
				ACTOR_JENNA, tgt->x.part[0].integer, tgt->z.part[0].integer);
		fdActorSetAnimation(ACTOR_SHEBA, ANIM_WALK);
		tgt = fdActorGet(ACTOR_FELIX);
		if(tgt != NULL)
			fdActorSetDestination(
				ACTOR_SHEBA, tgt->x.part[0].integer, tgt->z.part[0].integer);
	}
	scFieldActorWaitForMoveById(ACTOR_SHEBA);
	fdActorSetPos(ACTOR_SHEBA, 0, 0);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdScriptEnd();
	fdScriptSetFlag(FLAG_MENARDIS_SHIP_SEEN);
}

/* Felix casts Frost on the puddle on the path west of the ship: it freezes into
   a pillar of ice to jump across to the wooden box on the ledge, with its
   Cookie. */
void scLemurianShipDeckFreezePuddle(void)
{
	fdFlagBlockApplyFlags(LemurianShipDeckPuddles);
}

/* Felix steps onto the deck just east of the cabin door, metatiles (72, 25) and
   (72, 26), before Piers has come aboard. The first two times nothing visible
   happens; the third time Kraden comes out, walks east and says, "Well, there's
   no point in hanging around here all day. Let's go see about finding ourselves
   a ship we can use.", and goes back into the party.

   A step counts only after Felix has stepped beside those metatiles
   (scLemurianShipDeckStepBesideEastOfDoor). The scene's temporary flags count
   the steps, and FLAG_KRADEN_WENT_TO_FIND_A_SHIP ends it for good. */
void scLemurianShipDeckKradenGoesToFindShip(void)
{
	ACTOR *actor;

	if(fdScriptGetFlag(FLAG_STANDING_EAST_OF_DOOR))
		return;
	if(fdScriptGetFlag(FLAG_KRADEN_WENT_TO_FIND_A_SHIP))
		return;
	fdScriptSetFlag(FLAG_STANDING_EAST_OF_DOOR);
	if(fdScriptGetFlag(FLAG_SECOND_STEP_EAST_OF_DOOR)) {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		fdActorPlaceOnActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
		syWait(1);
		fdActorFaceDirection(
			MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
		fdActorSetSpeed(ACTOR_KRADEN, 0.8);
		fdActorWalkToAndWait(ACTOR_KRADEN, 416, 428);
		fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
		syFieldScriptSetMessageId(MsgLemurianShipDeckWellTheresNoPoint);
		/* "Well, there's no point in hanging around here all day. Let's go
		   see..." */
		fdScriptShowMessage(ACTOR_KRADEN, 0);
		fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
		actor = fdActorGet(MainRoutineStateWork.leaderActorId);
		if(actor != 0)
			fdActorSetDestination(ACTOR_KRADEN, actor->x.part[0].integer,
				actor->z.part[0].integer);
		scFieldActorWaitForMoveById(ACTOR_KRADEN);
		fdActorSetPos(ACTOR_KRADEN, 0, 0);
		fdScriptSetFlag(FLAG_KRADEN_WENT_TO_FIND_A_SHIP);
		fdScriptEnd();
	} else if(fdScriptGetFlag(FLAG_FIRST_STEP_EAST_OF_DOOR)) {
		fdScriptSetFlag(FLAG_SECOND_STEP_EAST_OF_DOOR);
	} else {
		fdScriptSetFlag(FLAG_FIRST_STEP_EAST_OF_DOOR);
	}
}

/* Nothing visible happens as Felix steps beside the metatiles east of the cabin
   door, before Piers has come aboard; his next step back onto them counts
   toward Kraden's going to find a ship. */
void scLemurianShipDeckStepBesideEastOfDoor(void)
{
	syGameFlagClear(FLAG_STANDING_EAST_OF_DOOR);
}

/* An event: as Piers opens the cabin door, sparkles drift in from around the
   Black Crystal toward it. */
void scLemurianShipDeckEmitSparkle(void)
{
	Sint32 pos[3];
	ACTOR *actor;

	SET_VECTOR(pos, toFIXED(360), toFIXED(64), toFIXED(424));
	syFieldVectorAddPolarOffset(32, syRandom(), pos);
	actor = scFieldActorCreate(SPRITE_GLINT, pos[0], pos[1], pos[2]);
	if(actor != NULL) {
		actor->motionFlg = 0;
		actor->speedLimit = toFIXED(2);
		actor->acceleration = toFIXED(1) / 2;
		fdActorSetSprFlags(actor, 0);
		fdActorSelectAnimation(actor, 0);
		scFieldActorSetMoveTarget(actor, 360, 64, 424);
		fdActorStartScript(actor, LemurianShipDeckSparkleFades);
	}
}

/* The Black Crystal, drawn from its item icon, as an actor on the deck. */
ACTOR *scLemurianShipDeckCreateBlackCrystal(void)
{
	ACTOR *actor = (ACTOR *)syImportActorCreate(
		SPRITE_UNSEEN_MARKER, toFIXED(360), toFIXED(64), toFIXED(424));
	Uint8	  *buf = 0;
	FIELD_SPR *spr;

	if(actor == 0)
		return 0;
	spr = actor->spr;
	spr->fullCol = 0;
	spr->pal = 0;
	spr->pri = 1;
	actor->motionFlg = 0;
	actor->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
	spr->flg = 0;
	spr->cnt = 0;
	buf = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	mnIconDrawItem(ITEM_BLACK_CRYSTAL);
	syVRAMCacheLoad(spr->slot, 128, ((ICON_TRANSFER *)buf)->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return actor;
}

/* The first time Felix comes to the cabin door on the ship's deck, metatile
   (22, 25), with Piers in the party: Piers jumps for joy, "At last, I'm back on
   my ship.", holds up the Black Crystal before the door, and as light gathers
   the screen glows red and flashes white and the door opens. "To set sail, we
   must go down to the power chamber belowdecks."

   It sets FLAG_PIERS_BACK_ON_HIS_SHIP. */
void scLemurianShipDeckReturnToShip(void)
{
	ACTOR *crystal;
	void (*sparkles)(void);
	ACTOR *actor;

	if(fdScriptGetFlag(FLAG_PIERS_BACK_ON_HIS_SHIP) != 0)
		return;
	if(fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)) == 0)
		return;
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorPlaceOnActor(ACTOR_PIERS, MainRoutineStateWork.leaderActorId);
	syWait(1);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorSetSpeed(ACTOR_PIERS, 1.6);
	fdActorWalkToAndWait(ACTOR_PIERS, 412, 424);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHWEST);
	syFieldScriptSetMessageId((Sint32)MsgLemurianShipDeckAtLastImBack);
	scFieldActorJump(ACTOR_PIERS, 4, 10);
	/* "At last, I'm back on my ship." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
	/* "Finally, I can return to my beloved Lemuria." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorWalkToAndWait(ACTOR_PIERS, 386, 424);
	fdActorFaceActor(ACTOR_PIERS, MainRoutineStateWork.leaderActorId, 0);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	/* "Wait there a moment." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorSetScript(
		MainRoutineStateWork.leaderActorId, LemurianShipDeckReturnToShipScript);
	fdActorSetSpeed(ACTOR_PIERS, 0.8);
	fdActorWalkToAndWait(ACTOR_PIERS, 360, 424);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
	fdScriptPlaySnd(SE_GEM_SET);
	crystal = scLemurianShipDeckCreateBlackCrystal();
	syFieldScriptWait(20);
	fdScriptPlaySnd(SE_SHIMMER);
	sparkles = scLemurianShipDeckEmitSparkle;
	sySetEvent(sparkles, EVENT_PASS_TICK);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
	fdPaletteFadeInterpolate(20);
	syFieldScriptWait(20);
	fdPaletteFadeApplyTarget(PALETTE_TINT_RED, PALETTES_BG);
	fdPaletteFadeInterpolate(20);
	syFieldScriptWait(20);
	fdPaletteFadeApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
	fdPaletteFadeInterpolate(20);
	syFieldScriptWait(20);
	scLemurianShipDeckDrawCabinDoorOpen();
	fdScriptSetFlag(FLAG_CABIN_DOOR_OPEN);
	syFieldCloseEvent(sparkles);
	if(crystal != 0)
		fdActorRelease(crystal);
	syFieldScriptWait(20);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeInterpolate(20);
	syFieldScriptWait(20);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
	/* "To set sail, we must go down to the power chamber belowdecks." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	/* "Let's head down." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_WALK);
	actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(actor != 0)
		fdActorSetDestination(
			ACTOR_PIERS, actor->x.part[0].integer, actor->z.part[0].integer);
	scFieldActorWaitForMoveById(ACTOR_PIERS);
	fdActorSetPos(ACTOR_PIERS, 0, 0);
	fdScriptSetFlag(FLAG_PIERS_BACK_ON_HIS_SHIP);
	fdScriptEnd();
}

/* Felix comes down the sandy path to the shore, metatiles (12, 5) to (21, 5),
   with Piers after the ceremony at the Gabomba Statue: "My ship! At long
   last... I have returned to my ship." Kraden asks him to take the party to
   Lemuria, and after Felix's two answers Piers agrees: "I will take you to
   Lemuria." He then asks to stop in Madra first to tell the mayor he has his
   orb back.

   It runs until the farewells in Madra and sets FLAG_PIERS_SETTING_SAIL. */
void scLemurianShipDeckReachPiersShip(void)
{
	if(fdScriptGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) == 0)
		return;
	if(fdScriptGetFlag(FLAG_MADRA_FAREWELL_SAID) != 0)
		return;
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId(MsgLemurianShipDeckMyShipAtLongLast);
	fdActorWalkTo(ACTOR_FELIX, 264, 176);
	scFieldActorWaitForMoveById(ACTOR_FELIX);
	scFieldActorLaunchFromLeader(ACTOR_PIERS, 16, 8, FACING_SOUTH);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(288, CAMERA_KEEP, 208, 1);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 32, 0, FACING_SOUTH);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, 48, -8, FACING_SOUTH);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, -16, -8, FACING_SOUTH);
	syFieldScriptWait(40);
	fdActorSetSpeed(ACTOR_PIERS, 2.4);
	fdActorWalkToAndWait(ACTOR_PIERS, 280, 224);
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	scFieldActorJump(ACTOR_PIERS, 4, 10);
	scFieldActorJump(ACTOR_PIERS, 4, 20);
	/* "My ship! At long last... I have returned to my ship." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "Well, Piers?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(288, CAMERA_KEEP, 184, 1);
	fdActorSetSpeed(ACTOR_PIERS, 0.8);
	fdActorWalkByAndWait(ACTOR_PIERS, 0, -16);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	/* "I'm thrilled... I'm just happy to be back with my ship." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHWEST);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
	/* "Uh, that's not what Kraden's asking..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	/* "You forgot, didn't you, Piers? When we talked in Kibombo?
	   Remember..." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
	/* "He wanted you to take us to your home... To Lemuria." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	/* "Remember how we said we'd talk about it later? Well, it's later
	   now..." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHWEST);
	fdActorSetAttachedEffect(
		ACTOR_PIERS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(20);
	/* "Oh... Is it? I suppose we have to talk about it sooner or later.
	   You..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "Piers...if you do not wish to take us, you do not have to. Just
	   tell..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	/* "And if not, well, we'll just have to get ourselves a boat and
	   find..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
	/* "Right, Felix?" Yes: "What's with the guilt trip? Piers hasn't..." No:
	   "Sheba is right! You guys just let Piers give..." */
	fdScriptOpenMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
		/* Yes: "What's with the guilt trip? Piers hasn't even given us
		   his..." */
		fdScriptShowMessage(ACTOR_JENNA, 0);
		/* "Kraden! Felix! Stop trying to pressure Piers!" */
		fdScriptShowMessage(ACTOR_SHEBA, 0);
		FieldStateWork->msg += 2;
	} else {
		FieldStateWork->msg += 2;
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
		fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
		/* No: "Sheba is right! You guys just let Piers give us his answer!" */
		fdScriptShowMessage(ACTOR_JENNA, 0);
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
		fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
		/* "I agree with Felix. We shouldn't try to pressure Piers, Kraden." */
		fdScriptShowMessage(ACTOR_SHEBA, 0);
	}
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTHEAST);
	/* "But that's not what I was°" */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "Don't worry, Sheba... I've actually been thinking about this quite
	   a..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
	/* "I just never got the opportunity to mention it before." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "Well? Well!?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 40);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHWEST);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(20);
	/* "It's not nice to test the patience of an old man. Will you take
	   us..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "We will go to Lemuria, Kraden." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	/* "I figured it was too much to..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 20);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_EAST);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH);
	/* "What did you just say!?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	/* "I will take you to Lemuria." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorSetAttachedEffect(
		ACTOR_JENNA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(20);
	/* "Kraden? Are you OK?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 2);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
	/* "Is he OK!?" */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 2);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "Piers! Are you... Are you serious?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	/* "After what we've been through, it would be wrong to deny you.
	   That's..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "What's what made up your mind?" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	/* "The fact that you are truly good people." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHWEST);
	/* "You have gone to great trouble to help one whose problems needn't..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	/* "Witnessing that kindness... It made me want to help, too." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
	/* "Piers, you..." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHWEST);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "No, Jenna. Please, don't say a word. I've made up my mind." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
	/* "I shall be leaving immediately, but I want you to come, Kraden." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHWEST);
	/* "Let us set sail, Felix! To my ship! And the Eastern Sea!" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "So at long last, I will finally reach Lemuria..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	/* "I wonder what kind of advanced civilization we'll find in Lemuria..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	/* "I'm actually excited to find out!" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorWalkByAndWait(ACTOR_PIERS, -16, 0);
	syFieldScriptWait(40);
	fdActorWalkByAndWait(ACTOR_PIERS, -16, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorWalkByAndWait(ACTOR_JENNA, 0, 16);
	/* "What's wrong, Piers?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	/* "It just occurred to me... The mayor of Madra and his wife must be..." */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 20);
	/* "We should go tell them that I've retrieved my orb. Then they..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
	/* "Is that acceptable, Felix? Can we stop by Madra briefly before we..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	/* "What do you say, Felix? Shall we go see the mayor?" */
	fdScriptOpenMessage(ACTOR_KRADEN, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 1) {
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
		/* No: "Don't worry about me. I've waited this long. I can wait a..." */
		fdScriptShowMessage(ACTOR_KRADEN, 0);
		FieldStateWork->msg += 1;
	} else {
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		FieldStateWork->msg += 1;
		/* Yes: "I have no objection, so why not? Let's go." */
		fdScriptShowMessage(ACTOR_KRADEN, 0);
	}
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "But we must depart as soon as we're done at the mayor's house,
	   Piers." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	/* "Oh, thank you!" */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdActorSetSpeed(ACTOR_JENNA, 1.2);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetSpeed(ACTOR_SHEBA, 1.2);
	fdActorSetSpeed(ACTOR_PIERS, 1.2);
	fdActorSetScript(ACTOR_JENNA, LemurianShipDeckReachPiersShipScript);
	fdActorSetScript(ACTOR_KRADEN, LemurianShipDeckReachPiersShipScript);
	fdActorSetScript(ACTOR_SHEBA, LemurianShipDeckReachPiersShipScript);
	fdActorRunScriptAndWait(ACTOR_PIERS, LemurianShipDeckReachPiersShipScript);
	syFieldScriptWait(20);
	fdScriptSetFlag(FLAG_PIERS_SETTING_SAIL);
	fdScriptEnd();
}

/* A wading splash rises at an actor's feet every eighth tick while it
   walks. */
void scLemurianShipDeckEmitSplash(ACTOR *actor)
{
	ACTOR *spawned;

	if((TickCount & 7) == 0) {
		spawned = scFieldActorCreate(SPRITE_WADING_SPLASH, actor->x.fixed,
			actor->y.fixed, actor->z.fixed);
		if(spawned != 0) {
			fdActorSetSprFlags(spawned, 0);
			fdActorSelectAnimation(spawned, 1);
			fdActorStartScript(spawned, LemurianShipDeckSplashFades);
		}
	}
}

/* Felix wades to the foot of the rope ladder on the ship's side, metatile (27,
   27), with Piers in the party before the farewells in Madra: Piers splashes
   out beside him, "Then let's waste no time! We should set out for Madra at
   once!", and rejoins the party. */
void scLemurianShipDeckSetOutForMadra(void)
{
	if(fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		ACTOR *actor;

		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		fdActorSetSpeed(ACTOR_FELIX, 0.8);
		fdActorGet(ACTOR_PIERS)->update =
			(OBJECT_UPDATE)scLemurianShipDeckEmitSplash;
		scFieldActorLaunchFromLeader(ACTOR_PIERS, 16, 0, FACING_WEST);
		fdActorSetSprFlags(fdActorGet(ACTOR_PIERS), 0);
		fdActorFaceDirection(
			MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
		syFieldScriptSetMessageId(MsgLemurianShipDeckThenLetsWasteNoTime);
		scFieldActorWaitForMoveById(ACTOR_PIERS);
		fdActorGet(ACTOR_PIERS)->update = NULL;
		/* "Then let's waste no time! We should set out for Madra at once!" */
		fdScriptShowMessage(ACTOR_PIERS, 0);
		fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
		fdActorGet(MainRoutineStateWork.leaderActorId)->update =
			(OBJECT_UPDATE)scLemurianShipDeckEmitSplash;
		fdActorWalkByAndWait(ACTOR_FELIX, 0, 16);
		fdActorGet(MainRoutineStateWork.leaderActorId)->update = NULL;
		fdActorGet(ACTOR_PIERS)->update =
			(OBJECT_UPDATE)scLemurianShipDeckEmitSplash;
		fdActorSetAnimation(ACTOR_PIERS, ANIM_WALK);
		actor = fdActorGet(ACTOR_FELIX);
		if(actor != 0)
			fdActorSetDestination(ACTOR_PIERS, actor->x.part[0].integer,
				actor->z.part[0].integer);
		scFieldActorWaitForMoveById(ACTOR_PIERS);
		fdActorSetPos(ACTOR_PIERS, 0, 0);
		fdActorGet(ACTOR_PIERS)->update = NULL;
		fdActorSetSprFlags(fdActorGet(ACTOR_PIERS), 1);
		fdScriptEnd();
	}
}

/* clang-format off */
/* The puddle on the path west of the ship, by actor id and flag. */
Sint16 LemurianShipDeckPuddles[] = {
	ACTOR_PUDDLE, FLAG_PUDDLE_FROZEN, -1, 0,
};

/* The party walking off across the shore, into Felix's party, once Piers has */
/* agreed to sail for Lemuria. */
Sint32 LemurianShipDeckReachPiersShipScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(263, 0, 176),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

/* Felix stepping back from the cabin door and turning to watch Piers open */
/* it. */
Sint32 LemurianShipDeckReturnToShipScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(368, 0, 448),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTHEAST),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(10),
	SC_END,
};

/* Jenna looking about the deck as the ship first sails. */
Sint32 LemurianShipDeckJennaLooksAround[] = {
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(30),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(50),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_WAIT(70),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Sheba looking about the deck as the ship first sails. */
Sint32 LemurianShipDeckShebaLooksAround[] = {
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(50),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_WAIT(70),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Kraden looking about the deck as the ship first sails. */
Sint32 LemurianShipDeckKradenLooksAround[] = {
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_WAIT(70),
	SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
	SC_WAIT(50),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(30),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
	SCENE_EXITS(LemurianShipMainDeck),
	SCENE_EXIT(LEMURIAN_SHIP_MAIN_DECK_TO_LEMURIAN_SHIP_CABINS, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_MAIN_DECK),
	SCENE_EXIT(LEMURIAN_SHIP_MAIN_DECK_TO_LEMURIAN_SHIP_CABINS, WorldMap, WORLD_MAP_FIRST_VOYAGE),
	SCENE_EXIT(LEMURIAN_SHIP_MAIN_DECK_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_LEMURIAN_SHIP_MAIN_DECK_NORTH),
	SCENE_EXITS(LemurianShipTiller),
	SCENE_EXIT(LEMURIAN_SHIP_TILLER_TO_WORLD_MAP_A, WorldMap, WORLD_MAP_FROM_SEA_OF_TIME_SWIRLS),
	SCENE_EXIT(LEMURIAN_SHIP_TILLER_TO_WORLD_MAP_B, WorldMap, WORLD_MAP_FROM_LEMURIAN_SHIP_MAIN_DECK_NORTH),
	SCENE_EXIT(LEMURIAN_SHIP_TILLER_TO_WORLD_MAP_C, WorldMap, WORLD_MAP_FROM_LEMURIAN_SHIP_TILLER_B),
	SCENE_EXITS_END,
};

/* The East Indra Shore's actors. */
ACTOR_PLACE LemurianShipDeckPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

/* The tiller deck's actors. */
ACTOR_PLACE LemurianShipDeckTillerPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

/* The tiller deck's cues: none. */
CUE LemurianShipDeckCues[] = {
	{CUE_TABLE_END},
};

/* The East Indra Shore's cues. */
CUE LemurianShipDeckMainDeckCues[] = {
	{STEP_CUE_WALKING_INTO(FACING_NORTH), LEMURIAN_SHIP_MAIN_DECK_TO_LEMURIAN_SHIP_CABINS, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipDeckEnterCabin)},
	{CUE_EXIT, LEMURIAN_SHIP_MAIN_DECK_TO_WORLD_MAP_NORTH, CONDITION_ALWAYS, LEMURIAN_SHIP_MAIN_DECK_TO_WORLD_MAP_NORTH},
	{CUE_STEP, METATILE_BEHAVIOR_HEAD_OF_PATH, FLAG_MENARDIS_SHIP_SEEN, SCRIPT_REF(scLemurianShipDeckSightMenardisShip)},
	{CUE_STEP, METATILE_BEHAVIOR_TOP_OF_SHORE, FLAG_PIERS_SETTING_SAIL, SCRIPT_REF(scLemurianShipDeckReachPiersShip)},
	{CUE_STEP, METATILE_BEHAVIOR_FOOT_OF_LADDER, FLAG_MADRA_FAREWELL_SAID, SCRIPT_REF(scLemurianShipDeckSetOutForMadra)},
	{CUE_STEP, LEMURIAN_SHIP_MAIN_DECK_TO_LEMURIAN_SHIP_CABINS, FLAG_PIERS_BACK_ON_HIS_SHIP, SCRIPT_REF(scLemurianShipDeckReturnToShip)},
	{CUE_EXAMINE, METATILE_BEHAVIOR_CABIN_DOOR, FLAG_PIERS_BACK_ON_HIS_SHIP, SCRIPT_REF(scLemurianShipDeckTryHeldDoor)},
	{CUE_STEP, METATILE_BEHAVIOR_EAST_OF_DOOR, FLAG_PIERS_BACK_ON_HIS_SHIP, SCRIPT_REF(scLemurianShipDeckKradenGoesToFindShip)},
	{CUE_STEP, METATILE_BEHAVIOR_BESIDE_EAST_OF_DOOR, FLAG_PIERS_BACK_ON_HIS_SHIP, SCRIPT_REF(scLemurianShipDeckStepBesideEastOfDoor)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_PUDDLE_FROZEN, SCRIPT_REF(scLemurianShipDeckFreezePuddle)},
	{CUE_TABLE_END},
};

/* A sparkle drifting in to the Black Crystal, gone as it arrives. */
Uint32 LemurianShipDeckSparkleFades[] = {
	SC_WAIT(10),
	SC_SYNC,
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_RELEASE,
};

/* A wading splash, gone after a quarter of a second. */
Uint32 LemurianShipDeckSplashFades[] = {
	SC_WAIT(15),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_RELEASE,
};
/* clang-format on */
