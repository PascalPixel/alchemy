/* Champa, the hungry seafaring village on the edge of Angara, its huts on
   cliffs and rafts about two waterfalls, whose people wait for Briggs to come
   home with food. The party comes in from the south over a plank bridge and up
   the path past six gravestones, where in a game linked with The Broken Seal
   Feizhi runs up to give Felix the gift she meant for Isaac
   (scChampaFeizhiArrives). Leaving by the same bridge the first time, the party
   meets Alex, who left it on Indra, with Karst and Agatio (scChampaMeetAlex);
   after he has gone, Kraden's hints leave Jenna and Sheba puzzled
   (scChampaJennaAndShebaLookPuzzled). Once Briggs has sailed from Alhafra, the
   people and cues change, and two sailors at the mouth of Briggs's cave spot
   Felix and send Briggs running inside (scChampaSailorsSpotFelix). */

#include "field/field.h"
#include "game/game.h"
#include "item_ids.h"
#include "field/script_inline.h"
#include "field/field_script.h"
#include "scene/scene.h"
#include "window/window.h"
#include "field/call.h"
#include "edition.h"
#include "field/actor_script.h"

struct ACTOR;
extern ACTOR_PLACE				   ChampaActorPlacementsOnceBriggsSailed[];
extern MAP_GRID_METATILE_ANIMATION ChampaDoors[];
extern CUE						   ChampaCues[];
extern CUE						   ChampaCuesOnceBriggsSailed[];
extern Uint16					   ChampaNoDoor[];
extern Uint16					   ChampaSanctumDoorOpening[];
/* Closes at once the portrait window of a speaker, if one is open
   (mnPortraitCloseWindow, through the overlay's own import row). */
Sint32 mnWindowCloseMatchingId();

/* Sends an actor walking to where another stands, if it is on the map. */
static inline void scActorHeadToward(Sint32 id, Sint32 tgt)
{
	ACTOR *actor = fdActorGet(tgt);

	if(actor != NULL)
		fdActorSetDestination(
			id, actor->x.part[0].integer, actor->z.part[0].integer);
}

/* The town's own story, which no other scene reads. */
#define FLAG_TOLD_OF_OBABA			 0x972 /* a villager told of Obaba */
#define FLAG_SAILORS_SPOTTED_FELIX	 0x973 /* two sailors ran to warn Briggs */
#define FLAG_FEIZHIS_GIFT_WAITING	 0xa74 /* Feizhi holds out her gift */
#define FLAG_FEIZHI_CAME			 0xa75 /* Feizhi came, taking Felix for a friend */
/* The party walked up the path into the village, so Alex meets it on the plank
   bridge on its way back out (scChampaMeetAlex). */
#define FLAG_WALKED_INTO_THE_VILLAGE 0x970

/* Kraden's sprite's animation 8: shaking his head again and again, slower than
   ANIM_SHAKE_HEAD, until another animation is set. */
#define ANIM_KRADEN_KEEP_SHAKING_HEAD 8

/* The map metatiles Champa's step and examine cues answer; a door's metatile
   takes the number of the exit it leaves by. */
typedef enum CHAMPA_METATILE_BEHAVIOR {
	/* Across the path up from the plank bridge (scChampaWalkIntoTheVillage),
	   and the plank bridge itself (scChampaMeetAlex). */
	METATILE_BEHAVIOR_PATH_INTO_THE_VILLAGE = 19,
	METATILE_BEHAVIOR_PLANK_BRIDGE = 20,
	/* The sign at the village's south end: "Champa ahead!" */
	METATILE_BEHAVIOR_VILLAGE_SIGN = 24,
	/* Before the two sailors at the mouth of Briggs's cave
	   (scChampaSailorsSpotFelix). */
	METATILE_BEHAVIOR_BEFORE_BRIGGS_CAVE = 26,
	/* The path past the six gravestones (scChampaFeizhiArrives). */
	METATILE_BEHAVIOR_GRAVESTONE_PATH = 100
} CHAMPA_METATILE_BEHAVIOR;

/* The last of the two waterfalls' colours in background palettes 3, 4 and 5,
   which run from colour 1, each a band of blues from light to dark and back. */
#define WATERFALL_LAST_COLOR		14
/* The map's tile animation that swings the sanctum's stone door open, once. */
#define SANCTUM_DOOR_ANIMATION		8
/* The same frames of the sanctum's door over and over, kept paused. */
#define SANCTUM_DOOR_LOOP_ANIMATION 3

/* Champa's people and things, numbering the rows of ChampaActorPlacements
   whose sprites are not the party's; ChampaActorPlacementsOnceBriggsSailed
   keeps them and adds the sailors who spot Felix. */
typedef enum CHAMPA_ACTOR_ID {
	/* The ends of the tightrope over the gap (fdTightropePlace). */
	ACTOR_TIGHTROPE_START = ACTOR_FIRST_PLACED,
	ACTOR_TIGHTROPE_END,
	/* The village's two flags either side of its road, Champa's anchor on
	   red. */
	ACTOR_WEST_FLAG,
	ACTOR_EAST_FLAG,
	/* "Oh, I'm so hungry... And there won't be anything to eat until..." */
	ACTOR_HUNGRY_YOUNG_MAN,
	/* "When is Briggs going to return with food for the village?" */
	ACTOR_WOMAN_WAITING_FOR_FOOD,
	/* "How did you find your way here, to the edge of Angara?" */
	ACTOR_MAN_AT_THE_EDGE_OF_ANGARA,
	/* "Champa is impoverished, but soon, we will prosper again." */
	ACTOR_WOMAN_ON_PROSPERITY,
	/* The old man who asks about Ankohl Ruins. */
	ACTOR_OLD_MAN_ON_ANKOHL_RUINS,
	/* "The ruins at Ankohl are all that remains of a great sanctum..." */
	ACTOR_OLD_WOMAN_ON_ANKOHL_RUINS,
	/* The boy practicing Psynergy: "If I could pick up things without using my
	   hands..." */
	ACTOR_PRACTICING_BOY,
	/* "Once Briggs comes back, I can play with Eoleo again..." */
	ACTOR_EOLEOS_FRIEND,
	/* Sailors at the cliff caves: "The cliff caves are a lot quieter with
	   Briggs gone..." */
	ACTOR_SAILOR_AT_THE_QUIET_CAVES,
	/* "Our matriarch is the last successor of the Ankohl." */
	ACTOR_SAILOR_ON_THE_MATRIARCH,
	/* "The Champa sailors aren't...well liked outside of here, are we?" */
	ACTOR_SAILOR_ASKING_IF_WELL_LIKED,
	/* A small rock by the practicing boy, drawn at half size. */
	ACTOR_ROCK,
	/* "I bet you're wondering why only sailors and their families..." */
	ACTOR_WOMAN_ON_THE_CLIFF,
	/* Who meet the party on the plank bridge at the village's south end
	   (scChampaMeetAlex). */
	ACTOR_ALEX,
	ACTOR_KARST,
	ACTOR_AGATIO,
	ACTOR_KRADEN,
	/* Feizhi, in a game linked with The Broken Seal (scChampaFeizhiArrives),
	   and Kraden as he meets her. */
	ACTOR_FEIZHI,
	ACTOR_KRADEN_MEETING_FEIZHI,
	/* Feizhi's gift, until it is taken (scChampaTakeFeizhisGift). */
	ACTOR_FEIZHIS_GIFT,
	/* Once Briggs has sailed: "Whoa! You... You're Felix!" */
	ACTOR_SAILOR_WHO_SPOTS_FELIX,
	/* "What? Really? It's Felix?" and "Briggs, get outta here!" */
	ACTOR_SAILOR_WHO_WARNS_BRIGGS,
	ACTOR_BRIGGS
} CHAMPA_ACTOR_ID;

/* The chest Reveal shows by the six gravestones, holding the Viking Helm. */
#define ACTOR_VIKING_HELM_CHEST FIELD_TREASURE_ACTOR

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

extern ACTOR_PLACE ChampaActorPlacements[];

const Sint16 *scGetActorPlacements(void)
{
	if(fdScriptGetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA))
		return ChampaActorPlacementsOnceBriggsSailed;
	return ChampaActorPlacements;
}

/* Felix, each tick as he walks into the tent on one of the two rafts: he
   shrinks by 3/128 of his size, as if going in under the canvas. */
void scChampaShrinkIntoTent(ACTOR *actor)
{
	actor->scaleX += -(toFIXED(3) / 128);
	actor->scaleY += -(toFIXED(3) / 128);
}

/* Walking into a door of Champa: it opens with a sound, Felix walks in (lined
   up on the door first when `mode` is 0, straight on when 1) and the screen
   goes to the room behind it. The door's map edit is not played. */
void scChampaOpenDoorAndEnter(
	const MAP_GRID_METATILE_ANIMATION *opening, Sint32 door, Sint32 mode)
{
	Sint32 id;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptPlaySnd(SE_DOOR_OPEN);
	fdActorGet(MainRoutineStateWork.leaderActorId)->motionFlg = ACTOR_FALLS;
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	id = MainRoutineStateWork.leaderActorId;
	if(mode == 0)
		fdActorCenterAndWalk(id, 2, -8);
	else
		fdActorSetDestinationOffset(id, 0, -8);
	syFieldScriptWait(10);
	syFieldScriptSetExitRequest(door);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptEnd();
}

/* Walking north into one of Champa's doors or cave mouths takes the party in:
   into the tents on the two rafts Felix shrinks as he goes, the sanctum's stone
   door swings open first (SANCTUM_DOOR_ANIMATION), and the others he simply
   walks through. */
void scChampaEnterDoor(Sint32 door)
{
	MAP_GRID_METATILE_ANIMATION *tbl = ChampaDoors;

	if((Uint32)(door - CHAMPA_TO_CHAMPA_ROOMS_SOUTHWEST_A) <= 1)
		fdActorGet(MainRoutineStateWork.leaderActorId)->update =
			(OBJECT_UPDATE)scChampaShrinkIntoTent;
	if(door == CHAMPA_TO_CHAMPA_SANCTUM) {
		fdTilesetAnimResume(SANCTUM_DOOR_ANIMATION);
		scChampaOpenDoorAndEnter(&tbl[0], CHAMPA_TO_CHAMPA_SANCTUM, 1);
	} else {
		scChampaOpenDoorAndEnter(&tbl[1], door, 0);
	}
}

/* Reveal cast anywhere in Champa shows the hidden chest among the six
   gravestones east of the path into the village, holding the Viking Helm, for
   as long as the Psynergy lasts. */
void scChampaRevealTreasureSpot(void)
{

	fdTreasureSpotSetPos(ACTOR_VIKING_HELM_CHEST, toFIXED(392), toFIXED(568));
	fdActorGet(ACTOR_VIKING_HELM_CHEST)->motionFlg = 0;
	fdActorGet(ACTOR_VIKING_HELM_CHEST)->y.fixed = toFIXED(32);
}

/* When Reveal fades, the chest among the six gravestones is hidden again. */
void scChampaHideTreasureSpot(void)
{
	fdTreasureSpotSetPos(ACTOR_VIKING_HELM_CHEST, 0, 0);
}

#include "text/msg_ids.h"

/* Walking up to the two sailors by the mouth of Briggs's cave in the cliff
   (metatiles (12, 19) and (12, 20)) after Briggs has escaped from Alhafra: they
   turn, "Whoa! You... You're Felix!", and yell to Briggs, who comes out, sees
   that it is really him and runs back into the cave, the sailors after him. */
void scChampaSailorsSpotFelix(void)
{

	fdScriptSetFlag(FLAG_SAILORS_SPOTTED_FELIX);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgChampaWhoaYouYoureFelix);
	fdActorFaceDirection(
		ACTOR_SAILOR_WHO_SPOTS_FELIX, FACING_SOUTH_SOUTHEAST, 0);
	scFieldActorJump(ACTOR_SAILOR_WHO_SPOTS_FELIX, 6, 20);
	/* "Whoa! You... You're Felix!" */
	syFieldScriptShowMessageThenWait(ACTOR_SAILOR_WHO_SPOTS_FELIX, 0, 5);
	fdCameraMoveTo(176, CAMERA_KEEP, 256, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(10);
	scFieldActorStartRepeatedMotion(ACTOR_SAILOR_WHO_SPOTS_FELIX, 3);
	fdActorSetAttachedEffect(ACTOR_SAILOR_WHO_SPOTS_FELIX,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	fdActorFaceDirectionAndWait(ACTOR_SAILOR_WHO_WARNS_BRIGGS, FACING_EAST);
	syFieldScriptWait(10);
	fdActorShowEmote(
		ACTOR_SAILOR_WHO_WARNS_BRIGGS, EMOTE_IN_FRONT | EMOTE_QUESTION, 35);
	fdActorFaceDirectionAndWait(
		ACTOR_SAILOR_WHO_WARNS_BRIGGS, FACING_SOUTH_SOUTHEAST);
	syFieldScriptWait(10);
	fdActorShowEmote(
		ACTOR_SAILOR_WHO_WARNS_BRIGGS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "What? Really? It's Felix?" */
	syFieldScriptShowMessageThenWait(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 0, 5);
	scFieldActorJump(ACTOR_SAILOR_WHO_SPOTS_FELIX, 6, 0);
	scFieldActorJump(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 6, 0);
	fdActorSetSpeed(ACTOR_SAILOR_WHO_SPOTS_FELIX, 1.8);
	fdActorSetSpeed(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 1.8);
	fdActorGet(ACTOR_SAILOR_WHO_SPOTS_FELIX)->steeringFlg &=
		~ACTOR_FACES_MOTION;
	fdActorWalkBy(ACTOR_SAILOR_WHO_SPOTS_FELIX, 0, -16);
	fdActorGet(ACTOR_SAILOR_WHO_WARNS_BRIGGS)->steeringFlg &=
		~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 0, -16);
	syFieldScriptWait(1);
	fdActorGet(ACTOR_SAILOR_WHO_WARNS_BRIGGS)->steeringFlg |=
		ACTOR_FACES_MOTION;
	syFieldScriptWait(15);
	fdActorShowEmote(
		ACTOR_SAILOR_WHO_SPOTS_FELIX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "You're awfully persistent, aren't you?" */
	syFieldScriptShowMessageThenWait(ACTOR_SAILOR_WHO_SPOTS_FELIX, 0, 5);
	fdActorFaceDirection(
		ACTOR_SAILOR_WHO_SPOTS_FELIX, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirectionAndWait(
		ACTOR_SAILOR_WHO_WARNS_BRIGGS, FACING_NORTH_NORTHEAST);
	syFieldScriptWait(5);
	/* "Briggs, get outta here! Felix is here to finish you off!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_BELOW | ACTOR_SAILOR_WHO_WARNS_BRIGGS, 0, 5);
	/* "What was that!? Felix? Here?" */
	syFieldScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	scFieldActorWatchAndSetScript(ACTOR_SAILOR_WHO_SPOTS_FELIX, ACTOR_BRIGGS);
	scFieldActorWatchAndSetScript(ACTOR_SAILOR_WHO_WARNS_BRIGGS, ACTOR_BRIGGS);
	fdActorSetSpeed(ACTOR_BRIGGS, 1.2);
	fdActorWalkByAndWait(ACTOR_BRIGGS, 0, 32);
	fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_SOUTH_SOUTHEAST);
	fdCameraMoveTo(208, CAMERA_KEEP, 296, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(15);
	scFieldActorJump(ACTOR_BRIGGS, 6, 15);
	scFieldActorJump(ACTOR_BRIGGS, 6, 23);
	/* "Oh no! It's really him! I've got to get out of here!!!" */
	syFieldScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	scFieldActorJump(ACTOR_BRIGGS, 6, 20);
	fdActorSetSpeed(ACTOR_BRIGGS, 2);
	fdActorWalkByAndWait(ACTOR_BRIGGS, 0, -48);
	fdActorFaceDirection(
		ACTOR_SAILOR_WHO_SPOTS_FELIX, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(
		ACTOR_SAILOR_WHO_WARNS_BRIGGS, FACING_NORTH_NORTHEAST, 0);
	fdActorSetPos(ACTOR_BRIGGS, 0, 0);
	fdActorSetSpeed(ACTOR_SAILOR_WHO_SPOTS_FELIX, 2);
	fdActorSetSpeed(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 2);
	fdActorWalkByAndWait(ACTOR_SAILOR_WHO_SPOTS_FELIX, -16, -16);
	fdActorWalkBy(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 16, -16);
	fdActorWalkByAndWait(ACTOR_SAILOR_WHO_SPOTS_FELIX, 0, -16);
	fdActorSetPos(ACTOR_SAILOR_WHO_SPOTS_FELIX, 0, 0);
	fdActorWalkByAndWait(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 0, -16);
	fdActorSetPos(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 0, 0);
	fdScriptEnd();
}

/* Talking to the boy practising Psynergy by his small rock: he stands up
   straight, tells of Eoleo's strange ability, and goes back to straining at the
   rock, squashed down with the effort. */
void scChampaTalkToPracticingBoy(Sint32 actorId)
{
	ACTOR *actor = fdActorGet(actorId);

	actor->scaleY = toFIXED(1);
	actor->scaleX = toFIXED(1);
	syFieldScriptSetMessageId((Sint32)MsgChampaEoleoBriggssSonHasA);
	/* "Eoleo, Briggs's son, has a really strange ability... I'll bet if
	   I..." */
	fdScriptShowMessage(actorId, 0);
	fdActorFaceDirectionAndWait(actorId, FACING_EAST);
	fdActorSetMovementType(actor, MOVEMENT_TYPE_SQUASH);
}

/* Talking to the woman who waited for Champa to prosper, once Briggs has
   sailed: "Hey, you just came to Champa not too long ago, didn't you?" Either
   way she tells how Briggs found ancient pirate treasure and fed the
   village. */
void scChampaTellOfBriggsTreasure(Sint32 actor)
{
	Sint32 msg = (Sint32)&MsgChampaHeyYouJustCameTo;

	syFieldScriptSetMessageId(msg);
	/* "Hey, you just came to Champa not too long ago, didn't you?" Yes: "You'll
	   be pleased to know that Briggs found ancient pirate treasure..." No:
	   "Well, you wouldn't know it to look at us now, but Champa was quite
	   poor..." */
	fdScriptOpenMessage(actor, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(
		   syFieldPartyGetLeaderActor(), 0) == 0) {
		syFieldScriptWait(10);
		syFieldScriptSetMessageId(msg + 1);
	} else {
		syFieldScriptWait(20);
		syFieldScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessage(actor, 0);
}

/* Talking to the old man by the old woman: he asks whether the party has been
   to Ankohl Ruins, or, once Briggs has sailed, whether they want to know why
   Briggs took so long to return, and answers the yes or the no. */
void scChampaAskAboutAnkohlRuins(Sint32 actor)
{
	if(fdScriptGetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA)) {
		syFieldScriptSetMessageId((Sint32)MsgChampaBriggsTookALongTime);
		/* "Briggs took a long time to return to us. You want to know why?" Yes:
		   "He was being held by an evil man who stole his money..." No: "Briggs
		   overcame great danger to return home to us. I admire him." */
		fdScriptOpenMessage(actor, 0);
		if(syFieldScriptAskYesNoWithActorAnimation(
			   syFieldPartyGetLeaderActor(), 0) == 0) {
			syFieldScriptWait(10);
			syFieldScriptSetMessageId((Sint32)MsgChampaBriggsTookALongTime + 1);
		} else {
			syFieldScriptWait(20);
			syFieldScriptSetMessageId((Sint32)MsgChampaBriggsTookALongTime + 2);
		}
		fdScriptShowMessage(actor, 0);
	} else {
		syFieldScriptSetMessageId((Sint32)MsgChampaYouveTheLookOfA);
		/* "You've the look of a traveler about you... Have you been to Ankohl
		   Ruins?" Yes: "Only those who have mastered the powers of the earth
		   can enter the ruins." No: "My mistake... These old eyes aren't what
		   they used to be..." */
		fdScriptOpenMessage(actor, 0);
		if(syFieldScriptAskYesNoWithActorAnimation(
			   syFieldPartyGetLeaderActor(), 0) == 0) {
			syFieldScriptWait(10);
			syFieldScriptSetMessageId((Sint32)MsgChampaYouveTheLookOfA + 1);
		} else {
			syFieldScriptWait(20);
			syFieldScriptSetMessageId((Sint32)MsgChampaYouveTheLookOfA + 2);
		}
		fdScriptShowMessage(actor, 0);
	}
}

/* Talking to the sailor who speaks of the matriarch, once Briggs has sailed:
   the first time he tells of Obaba, last of the Ankohl, and after that of how
   she acts now that Briggs is back. */
void scChampaTellOfObaba(Sint32 actor)
{

	if(!fdScriptGetFlag(FLAG_TOLD_OF_OBABA)) {
		fdScriptSetFlag(FLAG_TOLD_OF_OBABA);
		syFieldScriptSetMessageId((Sint32)MsgChampaObabaIsTheLastPossessor);
	} else {
		syFieldScriptSetMessageId((Sint32)MsgChampaYoudThinkObabaWouldBe);
	}
	/* "Obaba is the last possessor of the ancient secrets of Ankhol..." the
	   first time, then "You'd think Obaba would be glad that Briggs was
	   back." */
	fdScriptShowMessage(actor, 0);
}

/* Talking to the sailor who asks "The Champa sailors aren't...well liked
   outside of here, are we?", and hearing his answer to the yes or the no. */
void scChampaAskIfSailorsAreWellLiked(Sint32 actor)
{
	Sint32 msg = (Sint32)&MsgChampaTheChampaSailorsArentWell;

	syFieldScriptSetMessageId(msg);
	/* "The Champa sailors aren't...well liked outside of here, are we?" Yes:
	   "Hey, that hurts... You could at least lie to me, just to be polite..."
	   No: "Really? That's nice to hear. But if you do hear anything, don't
	   believe a word of it." */
	fdScriptOpenMessage(actor, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(
		   syFieldPartyGetLeaderActor(), 0) == 0) {
		syFieldScriptWait(10);
		syFieldScriptSetMessageId(msg + 1);
	} else {
		syFieldScriptWait(20);
		syFieldScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessage(actor, 0);
}

/* Talking to the charm Feizhi left on the path takes it, the Golden Ring; if
   the party cannot carry it, it stays there for later. */
void scChampaTakeFeizhisGift(Sint32 actor)
{

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(!Value3(fdScriptPickUpItem, actor, ITEM_GOLDEN_RING, -1)) {
		fdScriptClearFlag(FLAG_FEIZHIS_GIFT_WAITING);
	}
	fdScriptEnd();
}

/* Feizhi's gift, the Golden Ring, shown as an item on the path where she set it
   down, and marked as waiting to be taken. */
void scChampaFeizhiHoldsHerGift(void)
{
	scFieldActorShowItemIconById(ACTOR_FEIZHIS_GIFT, ITEM_GOLDEN_RING);
	fdActorSetPosAndFacing(
		ACTOR_FEIZHIS_GIFT, toFIXED(312), toFIXED(593), FACING_NORTH);
	fdScriptSetFlag(FLAG_FEIZHIS_GIFT_WAITING);
}

/* Walking up the path past the six gravestones (metatiles (18, 35) to (20,
   35)), in a game linked with a Broken Seal save in which Hsu was rescued,
   brings Feizhi running up behind with "Isaac!". She takes Felix for Isaac,
   finds he is not, is teased by Kraden, and leaves the party the good luck
   charm she made for Isaac, the Golden Ring, before she goes home. In every
   edition but the Japanese she no longer comes once Isaac has joined the
   party. */
void scChampaFeizhiArrives(void)
{
	if(!fdScriptGetFlag(FLAG_TRANSFERRED_ISAAC_SAVED_ULMUCH)) {
	} else {
#if FEIZHI_SKIPPED_WHEN_ISAAC_IN_PARTY
		/* Once Isaac is in the party Feizhi no longer comes; the Japanese
		   edition has no such test. */
		if(fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_ISAAC))) {
		} else
#endif
		{
			fdScriptSetFlag(FLAG_FEIZHI_CAME);
			fdScriptStart();
			syFieldScriptEndLastingPsynergy(0);
			syFieldScriptSetMessageId((Sint32)MsgChampaIsaac);
			fdActorSetPosAndFacing(
				ACTOR_FEIZHI, toFIXED(314), toFIXED(679), FACING_NORTH);
			/* "Isaac!" */
			syFieldScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
			fdActorWalkToAndWait(ACTOR_FELIX, 314, 580);
			fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
			syFieldScriptWait(10);
			syScriptCameraFollowActor(ACTOR_FEIZHI, 1);
			fdCameraWaitForArrival();
			syFieldScriptWait(40);
			fdActorWalkToAndWait(ACTOR_FEIZHI, 314, 624);
			fdActorShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
			/* "Isaac, is it you? Do my eyes deceive me?" */
			fdScriptOpenMessage(ACTOR_FEIZHI, 0);
			if(!syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
				/* "Oh. I must have been mistaken." */
				syFieldScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
				FieldStateWork->msg = (FieldStateWork->msg + 1);
			} else {
				FieldStateWork->msg = (FieldStateWork->msg + 1);
				/* "No, you are not Isaac, are you?" */
				syFieldScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
			}
			fdActorShowEmote(
				ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
			scFieldActorLaunchFromLeader(
				ACTOR_KRADEN_MEETING_FEIZHI, 16, 0, FACING_SOUTH);
			scFieldActorWaitForMoveById(ACTOR_KRADEN_MEETING_FEIZHI);
			syFieldScriptWait(20);
			fdActorShowEmote(ACTOR_KRADEN_MEETING_FEIZHI,
				EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
			/* "Young lady, are you looking for someone by the name of
			   Isaac?" */
			syFieldScriptShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
			fdActorRunRepeatedMotion(ACTOR_FEIZHI, 2);
			syFieldScriptWait(5);
			/* "Yes, old man! I am looking for one called Isaac!" */
			syFieldScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
			fdActorSetAnimationAndWait(ACTOR_KRADEN_MEETING_FEIZHI, ANIM_NOD);
			syFieldScriptWait(10);
			fdActorFaceDirectionAndWait(ACTOR_FEIZHI, FACING_SOUTH);
			syFieldScriptWait(20);
			fdActorFaceDirectionAndWait(ACTOR_FEIZHI, FACING_NORTH);
			syFieldScriptWait(20);
			fdActorFaceDirectionAndWait(ACTOR_FEIZHI, FACING_SOUTH);
			syFieldScriptWait(20);
			fdActorFaceDirectionAndWait(ACTOR_FEIZHI, FACING_NORTH);
			/* "Where is Isaac?" */
			syFieldScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
			fdActorFaceEachOther(ACTOR_KRADEN_MEETING_FEIZHI, ACTOR_FELIX, 0);
			syFieldScriptWait(40);
			fdActorFaceDirection(ACTOR_KRADEN_MEETING_FEIZHI, FACING_SOUTH, 0);
			fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
			syFieldScriptWait(20);
			fdActorSetAnimationAndWait(
				ACTOR_KRADEN_MEETING_FEIZHI, ANIM_SHAKE_HEAD);
			/* "I'm sorry... Isaac isn't with us..." */
			syFieldScriptShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
			fdActorShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
			scFieldActorJump(ACTOR_FEIZHI, 2, 0);
			/* "Well then, where can I find Isaac?" */
			syFieldScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
			fdActorShowEmote(ACTOR_KRADEN_MEETING_FEIZHI,
				EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
			/* "Why do you want to find him so badly?" */
			syFieldScriptShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
			fdActorRunRepeatedMotion(ACTOR_FEIZHI, 2);
			syFieldScriptWait(5);
			/* "Me? I am Feizhi... Isaac helped me once." */
			syFieldScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
			/* "He helped Ulmuch. Ulmuch was trapped, but Isaac saved him. He is
			   a..." */
			syFieldScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
			fdActorShowEmote(
				ACTOR_KRADEN_MEETING_FEIZHI, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
			/* "I suppose... But why are you looking for him? Do you like him?
			   Is..." */
			syFieldScriptShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
			fdActorShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
			/* "You're a mean old man. It is not polite to tease me." */
			syFieldScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
			fdActorSetAnimationAndWait(ACTOR_KRADEN_MEETING_FEIZHI, ANIM_NOD);
			syFieldScriptWait(10);
			/* "That's what it is! You like Isaac!" */
			syFieldScriptShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
			/* "No! I am so embarrassed!" */
			syFieldScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
			fdActorFaceDirectionAndWait(ACTOR_FEIZHI, FACING_SOUTH);
			syFieldScriptWait(10);
			fdActorShowEmote(
				ACTOR_KRADEN_MEETING_FEIZHI, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
			/* "Don't worry, Feizhi... We'll tell Isaac how you feel!" */
			syFieldScriptShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
			fdActorFaceDirectionAndWait(ACTOR_FEIZHI, FACING_NORTH);
			syFieldScriptWait(10);
			fdActorShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
			/* "I... Really?" */
			syFieldScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
			fdActorSetAnimationAndWait(ACTOR_KRADEN_MEETING_FEIZHI, ANIM_NOD);
			syFieldScriptWait(10);
			fdActorRunRepeatedMotion(ACTOR_FEIZHI, 2);
			syFieldScriptWait(5);
			/* "Then...will you give him this when you tell him?" */
			syFieldScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
			scChampaFeizhiHoldsHerGift();
			fdActorFaceEachOther(ACTOR_KRADEN_MEETING_FEIZHI, ACTOR_FELIX, 0);
			syFieldScriptWait(40);
			fdActorFaceDirection(ACTOR_KRADEN_MEETING_FEIZHI, FACING_SOUTH, 0);
			fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
			syFieldScriptWait(20);
			/* "What is it?" */
			syFieldScriptShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
			fdActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
			syFieldScriptWait(10);
			/* "Isaac is on a dangerous journey. I am worried about him, so I
			   made..." */
			syFieldScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
			fdActorShowEmote(ACTOR_KRADEN_MEETING_FEIZHI,
				EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
			/* "Ah, a good luck charm... I see." */
			syFieldScriptShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
			fdActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
			syFieldScriptWait(10);
			/* "I am going home now, old man. Thank you for doing this favor for
			   me." */
			syFieldScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
			fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
			fdActorSetAnimationAndWait(ACTOR_KRADEN_MEETING_FEIZHI, ANIM_NOD);
			syFieldScriptWait(10);
			fdActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
			syFieldScriptWait(10);
			syScriptCameraFollowActor(ACTOR_FELIX, 1);
			fdActorSetSpeed(ACTOR_FEIZHI, 1.2);
			fdActorWalkBy(ACTOR_FEIZHI, 0, 100);
			syFieldScriptWait(10);
			fdActorFaceEachOther(ACTOR_FELIX, ACTOR_KRADEN_MEETING_FEIZHI, 0);
			syFieldScriptWait(30);
			fdActorSetAnimation(ACTOR_KRADEN_MEETING_FEIZHI, ANIM_NOD);
			fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
			syFieldScriptWait(30);
			fdActorSetAnimation(ACTOR_KRADEN_MEETING_FEIZHI, ANIM_WALK);
			scActorHeadToward(ACTOR_KRADEN_MEETING_FEIZHI, ACTOR_FELIX);
			scFieldActorWaitForMoveById(ACTOR_KRADEN_MEETING_FEIZHI);
			fdActorSetPos(ACTOR_KRADEN_MEETING_FEIZHI, 0, 0);
			fdActorSetPos(ACTOR_FEIZHI, 0, 0);
			fdScriptEnd();
		}
	}
}

const CUE *scGetCues(void)
{
	if(fdScriptGetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA))
		return ChampaCuesOnceBriggsSailed;
	return ChampaCues;
}

/* Champa's two waterfalls flow: every other tick, the bands of blues they are
   drawn in, colours 1 to WATERFALL_LAST_COLOR of background palettes 3, 4 and
   5, turn one place on. */
void scChampaCycleWaterfallPalettes(void)
{
	if((TickCount & 1) == 0) {
		Sint16 saved;
		Sint32 idx;

		saved = BG_PLTT_COLOR(3, WATERFALL_LAST_COLOR);
		for(idx = WATERFALL_LAST_COLOR; idx != 1; idx--)
			BG_PLTT_COLOR(3, idx) = BG_PLTT_COLOR(3, idx - 1);
		BG_PLTT_COLOR(3, 1) = saved;
		saved = BG_PLTT_COLOR(4, WATERFALL_LAST_COLOR);
		for(idx = WATERFALL_LAST_COLOR; idx != 1; idx--)
			BG_PLTT_COLOR(4, idx) = BG_PLTT_COLOR(4, idx - 1);
		BG_PLTT_COLOR(4, 1) = saved;
		saved = BG_PLTT_COLOR(5, WATERFALL_LAST_COLOR);
		for(idx = WATERFALL_LAST_COLOR; idx != 1; idx--)
			BG_PLTT_COLOR(5, idx) = BG_PLTT_COLOR(5, idx - 1);
		BG_PLTT_COLOR(5, 1) = saved;
	}
}

/* Sets up the village as the party enters: the waterfalls flowing, the
   sanctum's door shut, the tightrope strung, the sailors and Briggs as the
   story leaves them, and Feizhi's gift where she left it. Returns 0. */
Sint32 scInit(void)
{

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	sySetTickEvent((Sint32)scChampaCycleWaterfallPalettes, EVENT_PASS_VBLANK);
	fdTilesetAnimPause(SANCTUM_DOOR_ANIMATION);
	fdActorSetSprFlags((Sint32)fdActorGet(ACTOR_WEST_FLAG), 0);
	fdActorSetSprFlags((Sint32)fdActorGet(ACTOR_EAST_FLAG), 0);
	scFieldActorSetSprPriority(ACTOR_WOMAN_WAITING_FOR_FOOD, 3);
	fdActorGet(ACTOR_ROCK)->scaleX = toFIXED(1) / 2;
	fdActorGet(ACTOR_ROCK)->scaleY = toFIXED(1) / 2;
	fdTightropeInit();
	fdTightropePlace(0, ACTOR_TIGHTROPE_START, ACTOR_TIGHTROPE_END);
	fdTilesetAnimPause(SANCTUM_DOOR_LOOP_ANIMATION);
	if(fdScriptGetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA)) {
		scFieldActorSetSprPriority(ACTOR_SAILOR_AT_THE_QUIET_CAVES, 2);
		scFieldActorSetSprPriority(ACTOR_SAILOR_ON_THE_MATRIARCH, 3);
		scFieldActorSetSprPriority(ACTOR_SAILOR_ASKING_IF_WELL_LIKED, 3);
		scFieldActorSetSprPriority(ACTOR_EOLEOS_FRIEND, 3);
		fdActorSetMovementType(
			fdActorGet(ACTOR_PRACTICING_BOY), MOVEMENT_TYPE_TAKE_LEADER_LAYER);
		if(fdScriptGetFlag(FLAG_SAILORS_SPOTTED_FELIX)) {
			fdActorSetPos(ACTOR_SAILOR_WHO_SPOTS_FELIX, 0, 0);
			fdActorSetPos(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 0, 0);
			fdActorSetPos(ACTOR_BRIGGS, 0, 0);
		}
	}
	if(fdScriptGetFlag(FLAG_FEIZHIS_GIFT_WAITING)) {
		scChampaFeizhiHoldsHerGift();
	}
	return 0;
}

/* Once Briggs has sailed, crates of the food he brought stand in the village.
   Returns 0. */
Sint32 scPrepareMap(void)
{

	if(fdScriptGetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA)) {
		fdMapGridCopyMetatilesLayered(11, 51, 5, 4, 16, 23);
		fdMapGridCopyMetatileAttributes(11, 51, 5, 6, 16, 23);
	}
	return 0;
}

extern Uint32 ChampaQuestionMarkScript[];

/* A question mark over someone's head, the emote sprite's, turned to `ang`,
   for `ticks` ticks. */
void scChampaSpawnQuestionMark(
	Sint32 x, Sint32 y, Sint32 z, Sint32 ang, Sint32 ticks)
{
	Sint16	   value = ang;
	ACTOR	  *actor;
	FIELD_SPR *spr;

	actor = scFieldActorCreate(SPRITE_EMOTE, x, y, z);
	if(actor != 0) {
		spr = actor->spr;
		fdActorSelectAnimation(actor, 1);
		fdActorStartScript(actor, ChampaQuestionMarkScript);
		actor->motionFlg = 0;
		actor->wait = ticks;

		spr->flg = 0;
		spr->rotation = value;
	}
}

/* Jenna and Sheba, turned to each other while Kraden talks of what he has
   learned, puzzle at each other: a white bubble with a question mark in it,
   tilted towards the other, pops up over Jenna's head and then over Sheba's,
   twice each, each with a chime, before they both say they do not follow him.
   The bubble is the emote sprite's question mark, turned 22.5 degrees one way
   or the other. */
void scChampaJennaAndShebaLookPuzzled(Sint32 firstId, Sint32 secondId)
{
	ACTOR *first = fdActorGet(firstId);
	ACTOR *second = fdActorGet(secondId);
	Sint32 rise = toFIXED(28);
	FIXED  y;

	y = first->y.fixed;
	scChampaSpawnQuestionMark(
		first->x.fixed, y + rise, first->z.fixed, DEGtoANG(22.5), 15);
	fdScriptPlaySnd(SE_LIGHT_APPEAR);
	syFieldScriptWait(15);
	y = second->y.fixed;
	scChampaSpawnQuestionMark(
		second->x.fixed, y + rise, second->z.fixed, DEGtoANG(337.5), 15);
	fdScriptPlaySnd(SE_LIGHT_APPEAR);
	syFieldScriptWait(15);
	scChampaSpawnQuestionMark(first->x.fixed, first->y.fixed + rise,
		first->z.fixed, DEGtoANG(22.5), 15);
	fdScriptPlaySnd(SE_LIGHT_APPEAR);
	syFieldScriptWait(15);
	scChampaSpawnQuestionMark(second->x.fixed, second->y.fixed + rise,
		second->z.fixed, DEGtoANG(337.5), 15);
	fdScriptPlaySnd(SE_LIGHT_APPEAR);
	syFieldScriptWait(15);
}

/* fdScriptShowTwoMessagesAndWait's first half, kept in the scene: two actors
   speak at once, the second message waiting for the first when delay is
   nonzero; scChampaCloseTwoMessages closes them. */
void scChampaShowTwoMessages(Sint32 first, Sint32 firstX, Sint32 firstY,
	Sint32 firstPortraitX, Sint32 firstPortraitY, Sint32 second, Sint32 secondX,
	Sint32 secondY, Sint32 secondPortraitX, Sint32 secondPortraitY,
	Sint32 delay)
{
	FIELD_STATE *event = RamHeapSlots->fieldState;
	Sint32		 firstSpr = fdActorGetSprById(first);
	Sint32		 secondSpr = fdActorGetSprById(second);

	mnMessageOpenWindow((Sint16)event->msg++, firstX, firstY,
		MESSAGE_WINDOW_VOICE(fdSpeakerGetVoice(firstSpr)));
	mnPortraitOpenWindow(firstSpr, 0, firstPortraitX, firstPortraitY);
	if(delay != 0) {
		while(!mnMessageAllTyped())
			syWait(delay);
	}
	mnMessageOpenWindow((Sint16)event->msg++, secondX, secondY,
		MESSAGE_WINDOW_VOICE(fdSpeakerGetVoice(secondSpr)));
	mnPortraitOpenWindow(secondSpr, 0, secondPortraitX, secondPortraitY);
	while(!mnMessageAllTyped())
		syWait(1);
	syWait(1);
}

/* Closes the two windows scChampaShowTwoMessages opened. */
void scChampaCloseTwoMessages(Sint32 first, Sint32 second)
{
	Sint32 firstSpr;
	Sint32 secondSpr;

	firstSpr = fdActorGetSprById(first);
	secondSpr = fdActorGetSprById(second);
	mnWindowCloseMatchingId(firstSpr);
	mnWindowCloseMatchingId(secondSpr);
}

/* Nothing visible happens when the party, coming in from the south, walks up
   the path past the plank bridge (metatiles (17, 40) to (22, 40)): the game
   notes that it has entered the village, so that Alex meets it on the bridge on
   its way back out. */
void scChampaWalkIntoTheVillage(void)
{
	syGameFlagSet(FLAG_WALKED_INTO_THE_VILLAGE);
}

/* Shows the speaker's message by its actor (fdMessageShowAtActor). */
static __inline__ void scChampaMessageOpen(Sint32 speaker, Sint32 flg)
{
	fdMessageShowAtActor(speaker, flg);
}

/* Walking back south onto the plank bridge out of Champa (metatile (19, 42))
   the first time, after going into the village, has Alex call "I see you've
   caught up with me again, Felix" and come down the path to meet the party.
   Karst and Agatio walk up from either side to press it on to Jupiter
   Lighthouse, Alex tells Kraden that Lord Babi is dead, and once they have
   gone, Kraden's hints about what he has learned leave Jenna and Sheba
   demanding to be told. */
void scChampaMeetAlex(void)
{

	if(!fdScriptGetFlag(FLAG_WALKED_INTO_THE_VILLAGE)) {
		return;
	}
	fdScriptSetFlag(FLAG_CHAMPA_MET_ALEX);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgChampaISeeYouveCaughtUp);
	fdActorSetSpeed(ACTOR_ALEX, 0.8);
	fdActorSetSpeed(ACTOR_KARST, 1);
	fdActorSetSpeed(ACTOR_AGATIO, 1);
	fdActorSetPos(ACTOR_ALEX, 312, 520);
	fdActorSetPos(ACTOR_KARST, 456, 616);
	fdActorSetPos(ACTOR_AGATIO, 168, 616);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	/* "I see you've caught up with me again, Felix." */
	fdScriptShowMessage(ACTOR_ALEX, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	fdActorWalkToAndWait(ACTOR_FELIX, 312, 616);
	fdCameraMoveTo(312, CAMERA_KEEP, 600, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(5);
	fdScriptPlaySnd(MUS_ALEX);
	fdActorWalkToAndWait(ACTOR_ALEX, 304, 576);
	syFieldScriptWait(5);
	scFieldActorLaunchFromLeader(ACTOR_PIERS, -16, 0, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, 0, 16, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, -16, 16, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, -32, 0, FACING_NORTH_NORTHEAST);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Alex!" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(20);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
	/* "Alex? Who is Alex?" */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
	syFieldScriptWait(10);
	/* "Alex is a Water Adept. We had been traveling together, but he
	   left..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "A Water Adept?" */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Ah, and I see you've made a new friend! You..." Yes: "And you've found
	   yourself a new Water Adept..." No: "Please... It's quite obvious that you
	   decided..." */
	scChampaMessageOpen(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		syFieldScriptWait(15);
		/* "And you've found yourself a new Water Adept! Do you mean to cast
		   me..." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
	} else {
		syFieldScriptWait(35);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
		/* "Please... It's quite obvious that you decided to replace me with
		   a..." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	}
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_WEST);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "What do you want with us now, Alex? Or did you forget you were the
	   one..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "My dear Jenna..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	syFieldScriptWait(5);
	/* "Have you come to despise me so, after all..." Yes: "That's right, Felix!
	   Why should we care about..." No: "Duh! You're the one who left us. It's
	   pretty..." */
	fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
	if(syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		syFieldScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
		syFieldScriptWait(10);
		fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTH);
		syFieldScriptWait(10);
		/* "That's right, Felix! Why should we care about someone who just
		   left..." */
		fdScriptShowMessage(ACTOR_SHEBA, 0);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
	} else {
		syFieldScriptWait(40);
		fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
		syFieldScriptWait(10);
		fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTH);
		syFieldScriptWait(10);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
		/* "Duh! You're the one who left us. It's pretty clear who despises
		   whom..." */
		fdScriptShowMessage(ACTOR_SHEBA, 0);
	}
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	/* "My, aren't we a bitter bunch? And I was just trying to help you
	   with..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "We have no need of your help. We can light the lighthouses without
	   you." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	/* "Oh... Can you really?" */
	fdScriptShowMessage(ACTOR_KARST, 0);
	scFieldActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorSetAttachedEffect(
		ACTOR_FELIX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_JENNA, 3);
	fdActorSetAttachedEffect(
		ACTOR_JENNA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_PIERS, 3);
	fdActorSetAttachedEffect(
		ACTOR_PIERS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_SHEBA, 3);
	fdActorSetAttachedEffect(
		ACTOR_SHEBA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
	fdActorWalkToAndWait(ACTOR_KARST, 344, 616);
	syFieldScriptWait(5);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	/* "Karst!" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorSetAnimationAndWait(ACTOR_KARST, ANIM_NOD);
	/* "That's right, Karst! I'm flattered that you remember!" */
	fdScriptShowMessage(ACTOR_KARST, 0);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_NORTH);
	/* "What is this, Alex? What's going on?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Ah, how can I explain this..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	/* "What's going on is Alex is demonstrating his remarkable foresight
	   once..." */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	scFieldActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorSetAttachedEffect(
		ACTOR_FELIX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_JENNA, 3);
	fdActorSetAttachedEffect(
		ACTOR_JENNA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_PIERS, 3);
	fdActorSetAttachedEffect(
		ACTOR_PIERS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_SHEBA, 3);
	fdActorSetAttachedEffect(
		ACTOR_SHEBA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
	fdActorWalkToAndWait(ACTOR_AGATIO, 232, 616);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Oh, have you not had the pleasure of an introduction?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_NORTH_NORTHWEST);
	/* "He stayed back at the ship when I saw them in Madra." */
	fdScriptShowMessage(ACTOR_KARST, 0);
	fdActorWalkByAndWait(ACTOR_AGATIO, 16, 0);
	/* "I am Agatio." */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	scFieldActorJump(ACTOR_AGATIO, 4, 25);
	/* "Let's see... That guy's Felix, and the girls are Jenna and Sheba..." */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTH_NORTHEAST);
	/* "They look like an unreliable bunch of ragamuffins." */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(15);
	/* "Yes, they are rather useless, aren't they? That's why I finally had
	   to..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "But now, I believe I may have been...hasty in my judgment." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
	syFieldScriptWait(5);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "You are Piers?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "H-How do you know my name?" */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(10);
	/* "How soon they forget! We met you before, when you were
	   being...detained." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	/* "Who could forget the amateur Adept changing water to ice in that..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 42);
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, -16);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
	/* "Ah! You saw that! That must be how you knew Piers was a Water Adept." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_NORTH_NORTHEAST);
	syFieldScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "In fact, I was even thinking of borrowing his ship at one point." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "You would steal someone's boat? That's so typical, Alex!" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
	/* "You wound me, Jenna... I only meant to borrow it." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Whatever you might have meant, that doesn't make it right!" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
	syFieldScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "And just when I was about to seize the opportunity, you had to come
	   back." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	/* "You mean from Kibombo..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(10);
	/* "I was mere moments too late." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorWalkByAndWait(ACTOR_KRADEN, 8, -8);
	syFieldScriptWait(5);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Isn't that a shame." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorSetSpeed(ACTOR_ALEX, 1);
	fdActorGet(ACTOR_ALEX)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_ALEX, -7, 7);
	syFieldScriptWait(1);
	fdActorGet(ACTOR_ALEX)->steeringFlg |= ACTOR_FACES_MOTION;
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	/* "Still, because of that, I did meet Karst and Agatio, so perhaps it
	   was..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	/* "Can't you just stop talking about yourself for one second, Alex?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	scFieldActorStartRepeatedMotion(ACTOR_ALEX, 3);
	fdActorSetAttachedEffect(
		ACTOR_ALEX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_KARST, 2);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_NORTH_NORTHWEST);
	/* "Enough of your childish gibes! Alex, I want to know what makes
	   these..." */
	fdScriptShowMessage(ACTOR_KARST, 0);
	fdActorSetSpeed(ACTOR_ALEX, 0.6);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_EAST);
	fdActorWalkBy(ACTOR_ALEX, 8, 0);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
	syFieldScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(10);
	/* "Because they brought us the orb that Akafubu had taken. That
	   shows..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "How so?" */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
	/* "The Kibombo are quite warlike... or were, at any rate. Recovering
	   the..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	/* "They were just typical villagers, easily frightened with a small..." */
	fdScriptShowMessage(ACTOR_KARST, 0);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "No... They would have used less abrupt methods than those you
	   favor..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_EAST);
	syFieldScriptWait(5);
	/* "Oh? What methods would those be?" */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
	syFieldScriptWait(30);
	fdActorSetSpeed(ACTOR_KRADEN, 0.6);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	syFieldScriptWait(2);
	fdActorGet(ACTOR_KRADEN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_KRADEN, -8, 8);
	syFieldScriptWait(1);
	fdActorGet(ACTOR_KRADEN)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "Alex said it clearly enough. We wouldn't terrorize people into
	   doing..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 42);
	scFieldActorJump(ACTOR_KARST, 6, 15);
	scFieldActorJump(ACTOR_KARST, 6, 23);
	/* "Enough of this!" */
	fdScriptShowMessage(ACTOR_KARST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_WEST);
	/* "Stop changing the subject! When are you going to light the
	   remaining..." */
	fdScriptShowMessage(ACTOR_KARST, 0);
	scFieldActorJump(ACTOR_AGATIO, 6, 15);
	scFieldActorJump(ACTOR_AGATIO, 6, 23);
	/* "Yes! Our patience is beginning to wear thin!" */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	fdActorSetSpeed(ACTOR_KARST, 1.8);
	fdActorWalkByAndWait(ACTOR_KARST, -12, 0);
	syFieldScriptWait(5);
	/* "If you waste much more of our time, we'll take those Elemental
	   Stars..." */
	fdScriptShowMessage(ACTOR_KARST, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 45);
	fdActorSetSpeed(ACTOR_AGATIO, 1.8);
	fdActorWalkByAndWait(ACTOR_AGATIO, 12, 0);
	syFieldScriptWait(5);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "If we had a Wind Adept like Sheba and the Elemental Stars, we
	   wouldn't..." */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
	/* "Excuse me?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "I am, of course, opposed to their line of thinking." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorFaceDirection(ACTOR_KARST, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_AGATIO, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
	syFieldScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "My experience with Saturos and Menardi suggests that, while they
	   were..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "You mean to say that they failed to solve the riddles?" */
	fdScriptShowMessage(ACTOR_KARST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "Miserably..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 42);
	scFieldActorJump(ACTOR_AGATIO, 6, 15);
	scFieldActorJump(ACTOR_AGATIO, 6, 23);
	/* "Are you suggesting they were simple brutes, incapable of logic
	   and..." */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "I'm afraid so..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorRunRepeatedMotion(ACTOR_KARST, 2);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_NORTH_NORTHWEST);
	/* "Alex... Whose side are you on?" */
	fdScriptShowMessage(ACTOR_KARST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "I am on no one's side. My only concern is to see the lighthouse..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorRunRepeatedMotion(ACTOR_AGATIO, 2);
	/* "So you mean to leave the event to Felix?" */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	syFieldScriptWait(15);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(25);
	fdActorRunRepeatedMotion(ACTOR_KARST, 2);
	/* "Then why have we come all this way looking for Felix?" */
	fdScriptShowMessage(ACTOR_KARST, 0);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
	/* "I merely wanted to confirm that he and his companions had not..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	syFieldScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_AGATIO, 2);
	/* "Is that truly all?" */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "I also wanted to introduce Felix to the two of you." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Why would you want to do that?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "With Saturos and Menardi gone, you probably felt very little
	   pressure..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "What do you mean by that!?" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "With Saturos and Menardi gone, I felt another pair might... How
	   shall..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	/* "Is that some kind of threat?" */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_NORTH_NORTHEAST);
	syFieldScriptWait(15);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Take it as you wish. I just wanted to provide you with the proper..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorSetAnimationAndWait(ACTOR_KARST, ANIM_NOD);
	/* "So, what? You've used us to your satisfaction, and now, our role is
	   done?" */
	fdScriptShowMessage(ACTOR_KARST, 0);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
	syFieldScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_AGATIO, 2);
	/* "You only wanted us around to flex a little muscle and scare them
	   into..." */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_EAST);
	/* "I suppose we will let you go today..." */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
	syFieldScriptWait(20);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_AGATIO);
	scFieldActorWatchAndSetScript(ACTOR_PIERS, ACTOR_AGATIO);
	scFieldActorWatchAndSetScript(ACTOR_JENNA, ACTOR_AGATIO);
	scFieldActorWatchAndSetScript(ACTOR_SHEBA, ACTOR_AGATIO);
	scFieldActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_AGATIO);
	fdActorSetSpeed(ACTOR_AGATIO, 1);
	fdActorWalkByAndWait(ACTOR_AGATIO, 20, 0);
	fdActorWalkByAndWait(ACTOR_AGATIO, 2, 32);
	fdActorWalkByAndWait(ACTOR_AGATIO, 32, 32);
	fdActorWalkByAndWait(ACTOR_AGATIO, 0, 32);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_WEST);
	fdActorSetAnimationAndWait(ACTOR_KARST, ANIM_NOD);
	/* "But we will always be nearby... pushing you to make your way to..." */
	fdScriptShowMessage(ACTOR_KARST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
	syFieldScriptWait(20);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_KARST);
	scFieldActorWatchAndSetScript(ACTOR_PIERS, ACTOR_KARST);
	scFieldActorWatchAndSetScript(ACTOR_JENNA, ACTOR_KARST);
	scFieldActorWatchAndSetScript(ACTOR_SHEBA, ACTOR_KARST);
	scFieldActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_KARST);
	fdActorSetSpeed(ACTOR_KARST, 1);
	fdActorWalkByAndWait(ACTOR_KARST, 0, 32);
	fdActorWalkByAndWait(ACTOR_KARST, -20, 24);
	fdActorWalkByAndWait(ACTOR_KARST, 0, 48);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
	syFieldScriptWait(20);
	fdActorSetPos(ACTOR_KARST, 0, 0);
	fdActorSetPos(ACTOR_AGATIO, 0, 0);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(10);
	/* "I look forward to seeing you soon, Felix, somewhere very near
	   Jupiter..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_ALEX);
	scFieldActorWatchAndSetScript(ACTOR_PIERS, ACTOR_ALEX);
	scFieldActorWatchAndSetScript(ACTOR_JENNA, ACTOR_ALEX);
	scFieldActorWatchAndSetScript(ACTOR_SHEBA, ACTOR_ALEX);
	scFieldActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_ALEX);
	fdActorSetSpeed(ACTOR_ALEX, 1);
	fdActorWalkByAndWait(ACTOR_ALEX, 22, 24);
	syScriptCameraFollowActor(ACTOR_ALEX, 1);
	fdActorWalkByAndWait(ACTOR_ALEX, 0, 32);
	fdActorWalkByAndWait(ACTOR_ALEX, -16, 16);
	fdActorWalkByAndWait(ACTOR_ALEX, 0, 8);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_NORTH_NORTHWEST);
	fdCameraMoveTo(304, CAMERA_KEEP, 648, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(10);
	/* "Kraden, am I right in recalling that you pursue this quest on the..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(10);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, 32);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
	syFieldScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "This is true. I'm studying Alchemy because Lord Babi has ordered it." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "What about it?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "Ah, what a pity. It would seem all your studies... have been for
	   naught." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "What are you getting at, Alex?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Only that it seems your Lord Babi has at last succumbed to the
	   ravages..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
	syFieldScriptWait(25);
	fdActorSetSpeed(ACTOR_KRADEN, 0.4);
	fdActorGet(ACTOR_KRADEN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, -24);
	syFieldScriptWait(1);
	fdActorGet(ACTOR_KRADEN)->steeringFlg |= ACTOR_FACES_MOTION;
	syFieldScriptWait(10);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_KRADEN_KEEP_SHAKING_HEAD);
	syFieldScriptWait(30);
	/* "Lord Babi is dead?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "Oh, so you didn't know?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTH);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Lord Babi? Dead? Could that really be true?" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(25);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	/* "Alex! How would you know such a thing?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	/* "Do not mistake me! We had no hand in Babi's passing." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "However, the people of Tolbi may be under the impression that our..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Alex! Cut the melodrama and get to the point for a change!" */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(10);
	/* "Simply put, I would recommend against traveling to north Gondowan
	   for..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "I just thought it might be better if you focused on your quest..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	/* "How kind of you." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Take it as you will..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
	syFieldScriptWait(10);
	/* "Very well. May we meet again." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	fdActorWalkByAndWait(ACTOR_ALEX, 0, 80);
	fdActorSetPos(ACTOR_ALEX, 0, 0);
	syFieldScriptWait(30);
	fdCameraMoveTo(304, CAMERA_KEEP, 624, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(10);
	fdScriptPlaySnd(MUS_CHAMPA);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "So the only reason Alex came here was to threaten us?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "And to bring us news of Babi's death, despite the fact that we
	   asked..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Perhaps he had his own reasons for telling Kraden..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTH);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "What do you mean, Piers?" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	/* "If Kraden undertook this quest on behalf of Babi, then he no
	   longer..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_NORTHWEST);
	syFieldScriptWait(10);
	/* "Oh. Of course! I hadn't thought of that. Your orders were to find..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "That's right. But if Babi really has died, you're free to go where
	   you..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
	syFieldScriptWait(10);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, 16);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
	syFieldScriptWait(5);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "That's not true, Sheba." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "Certainly, my original reason for researching Alchemy was for
	   Lord..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* ""Was for Lord Babi"? That's the past tense! What are your reasons
	   now?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "This is no longer solely for research... I do this because of
	   what..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	scFieldActorStartRepeatedMotion(ACTOR_PIERS, 3);
	fdActorSetAttachedEffect(
		ACTOR_PIERS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(45);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Your research?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "What you learned on this quest?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST + 1);
	syFieldScriptWait(25);
	scChampaJennaAndShebaLookPuzzled(ACTOR_JENNA, ACTOR_SHEBA);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
	syFieldScriptWait(10);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
	/* Jenna: "I'm not following you at all!" Sheba: "I don't understand a word
	   of this." */
	syFieldScriptShowTwoMessagesAndWait(
		ACTOR_JENNA, 5, 5, 18, 8, ACTOR_SHEBA, 6, 14, 8, 9, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "Of course you don't! Felix is the only one I've spoken to about any
	   of..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	scFieldActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorSetAttachedEffect(
		ACTOR_FELIX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTHEAST);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* Jenna: "What!? You knew!?" Sheba: "That's not fair! Tell us!" Yes:
	   "Please, Felix... Let's not say too much..." No: "Felix is right. Now is
	   not the time to tell you about it all..." */
	scChampaShowTwoMessages(
		ACTOR_JENNA, 8, 5, 18, 8, ACTOR_SHEBA, 8, 14, 8, 9, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTHWEST);
	if(syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		scChampaCloseTwoMessages(ACTOR_JENNA, ACTOR_SHEBA);
		syFieldScriptWait(15);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		/* "Please, Felix... Let's not say too much about it just yet." */
		fdScriptShowMessage(ACTOR_KRADEN, 0);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
	} else {
		scChampaCloseTwoMessages(ACTOR_JENNA, ACTOR_SHEBA);
		syFieldScriptWait(35);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
		/* "Felix is right. Now is not the time to tell you about it all..." */
		fdScriptShowMessage(ACTOR_KRADEN, 0);
	}
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 45);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "It's...far too complicated." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	scFieldActorJump(ACTOR_JENNA, 6, 15);
	scFieldActorJump(ACTOR_JENNA, 6, 23);
	/* "Well, when WILL you tell us?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "I am sure we will tell you once we get to Lemuria." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	scFieldActorJump(ACTOR_SHEBA, 6, 15);
	scFieldActorJump(ACTOR_SHEBA, 6, 23);
	/* "Do you promise?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "I promise..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, -8);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
	syFieldScriptWait(5);
	/* "Piers? You don't mind waiting?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "I cannot read minds, like Sheba, but I have a guess at what
	   you're..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "A guess? What do you mean?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	syFieldScriptWait(10);
	/* "You are a great scholar, Kraden, are you not?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	syFieldScriptWait(5);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 45);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	syFieldScriptWait(10);
	/* "I believe your theory is correct. And to prove it, I want to return
	   to..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldScriptWait(15);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldScriptWait(20);
	fdActorSetSpeed(ACTOR_JENNA, 1.2);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
	scActorHeadToward(ACTOR_JENNA, MainRoutineStateWork.leaderActorId);
	scFieldActorWaitForMoveById(ACTOR_JENNA);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorSetSpeed(ACTOR_SHEBA, 1.2);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_WALK);
	scActorHeadToward(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId);
	scFieldActorWaitForMoveById(ACTOR_SHEBA);
	fdActorSetPos(ACTOR_SHEBA, 0, 0);
	fdActorSetSpeed(ACTOR_PIERS, 1.2);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_WALK);
	scActorHeadToward(ACTOR_PIERS, MainRoutineStateWork.leaderActorId);
	scFieldActorWaitForMoveById(ACTOR_PIERS);
	fdActorSetPos(ACTOR_PIERS, 0, 0);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	scActorHeadToward(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdScriptEnd();
}

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 280, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(Champa),
	SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_ROOMS_SOUTHWEST_A, ChampaRooms,
		CHAMPA_ROOMS_FROM_CHAMPA_SOUTHWEST_A, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_ROOMS_SOUTHWEST_B, ChampaRooms,
		CHAMPA_ROOMS_FROM_CHAMPA_SOUTHWEST_B, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_ROOMS_NORTH, ChampaRooms,
		CHAMPA_ROOMS_FROM_CHAMPA_NORTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_ROOMS_WEST, ChampaRooms,
		CHAMPA_ROOMS_FROM_CHAMPA_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_ROOMS_NORTHEAST, ChampaRooms,
		CHAMPA_ROOMS_FROM_CHAMPA_NORTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_ROOMS_EAST, ChampaRooms,
		CHAMPA_ROOMS_FROM_CHAMPA_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_SANCTUM, ChampaSanctum,
		CHAMPA_SANCTUM_FROM_CHAMPA, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_CLIFF_CAVES_BRIGGS,
		ChampaCliffCavesBriggs, CHAMPA_CLIFF_CAVES_BRIGGS_FROM_CHAMPA,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(
		CHAMPA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_CHAMPA, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_CLIFF_CAVES_HOUSES,
		ChampaCliffCavesHouses, CHAMPA_CLIFF_CAVES_HOUSES_FROM_CHAMPA,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_CLIFF_CAVES_OBABA, ChampaCliffCavesObaba,
		CHAMPA_CLIFF_CAVES_OBABA_FROM_CHAMPA, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

/* clang-format off */
/* Champa's actors, in CHAMPA_ACTOR_ID's order. */
ACTOR_PLACE ChampaActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_RED_FLAG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 240, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_RED_FLAG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 296, 0, 408, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 392, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 328, 0, 312, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 432, 0, 312, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 280, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 280, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_SQUASH, 440, 0, 392, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 408, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 280, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 296, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 280, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 392, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 344, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_KARST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_AGATIO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FEIZHI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

/* Champa's actors once Briggs has sailed from Alhafra: the sailors at the */
/* mouth of his cave and Briggs too. */
ACTOR_PLACE ChampaActorPlacementsOnceBriggsSailed[] = {
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_RED_FLAG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 240, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_RED_FLAG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 424, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 392, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 328, 0, 312, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 432, 0, 312, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 280, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 280, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 400, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 392, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 392, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 272, 0, 472, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 320, 0, 472, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 392, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 344, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_KARST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_AGATIO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FEIZHI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 296, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 296, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_BRIGGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 264, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

/* A doorway with no door: nothing to open. */
Uint16 ChampaNoDoor[] = {
	MAPGRID_ANIMATION_STEPS_END,
};

/* The sanctum's door opening in two steps. */
Uint16 ChampaSanctumDoorOpening[] = {
	MAPGRID_ANIMATION_STEP(36, 2, 2, 2, 6),
	MAPGRID_ANIMATION_STEP(36, 4, 2, 2, 6),
	MAPGRID_ANIMATION_STEPS_END,
};
/* clang-format on */

/* The doors' map edits for scChampaOpenDoorAndEnter, which does not play them:
   the sanctum's, then the others'. The sanctum's door opens through
   SANCTUM_DOOR_ANIMATION instead. */
MAP_GRID_METATILE_ANIMATION ChampaDoors[] = {
	{ChampaSanctumDoorOpening, 29, 9},
	{ChampaNoDoor, 0, 0},
};

/* Champa's exits, doors, talks, Mind Read and Psynergy. */
CUE ChampaCues[] = {
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_SOUTHWEST_A,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_SOUTHWEST_B,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_NORTH,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_WEST,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_NORTHEAST,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_EAST,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_SANCTUM,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_CLIFF_CAVES_BRIGGS,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{CUE_EXIT, CHAMPA_TO_WORLD_MAP, CONDITION_ALWAYS, CHAMPA_TO_WORLD_MAP},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_CLIFF_CAVES_HOUSES,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_CLIFF_CAVES_OBABA,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{EXAMINE_CUE_FACING(FACING_NORTH), METATILE_BEHAVIOR_VILLAGE_SIGN,
		CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgChampaChampaAhead))},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaRevealTreasureSpot)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaHideTreasureSpot)},
	{CUE_STEP, METATILE_BEHAVIOR_GRAVESTONE_PATH, FLAG_FEIZHI_CAME,
		SCRIPT_REF(scChampaFeizhiArrives)},
	{CUE_STEP, METATILE_BEHAVIOR_PATH_INTO_THE_VILLAGE,
		FLAG_WALKED_INTO_THE_VILLAGE, SCRIPT_REF(scChampaWalkIntoTheVillage)},
	{CUE_STEP, METATILE_BEHAVIOR_PLANK_BRIDGE, FLAG_CHAMPA_MET_ALEX,
		SCRIPT_REF(scChampaMeetAlex)},
	{CUE_TALK, ACTOR_HUNGRY_YOUNG_MAN, CONDITION_ALWAYS,
		MSGNO(MsgChampaOhImSoHungryAnd)},
	{CUE_TALK, ACTOR_WOMAN_WAITING_FOR_FOOD, CONDITION_ALWAYS,
		MSGNO(MsgChampaWhenIsBriggsGoingTo)},
	{CUE_TALK, ACTOR_MAN_AT_THE_EDGE_OF_ANGARA, CONDITION_ALWAYS,
		MSGNO(MsgChampaHowDidYouFindYour)},
	{CUE_TALK, ACTOR_WOMAN_ON_PROSPERITY, CONDITION_ALWAYS,
		MSGNO(MsgChampaChampaIsImpoverishedButSoon)},
	{CUE_TALK, ACTOR_OLD_MAN_ON_ANKOHL_RUINS, CONDITION_ALWAYS,
		SCRIPT_REF(scChampaAskAboutAnkohlRuins)},
	{CUE_TALK, ACTOR_OLD_WOMAN_ON_ANKOHL_RUINS, CONDITION_ALWAYS,
		MSGNO(MsgChampaTheRuinsAtAnkohlAre)},
	{CUE_TALK, ACTOR_PRACTICING_BOY, CONDITION_ALWAYS,
		SCRIPT_REF(scChampaTalkToPracticingBoy)},
	{CUE_TALK, ACTOR_EOLEOS_FRIEND, CONDITION_ALWAYS,
		MSGNO(MsgChampaOnceBriggsComesBackI)},
	{CUE_TALK, ACTOR_SAILOR_AT_THE_QUIET_CAVES, CONDITION_ALWAYS,
		MSGNO(MsgChampaTheCliffCavesAreA)},
	{CUE_TALK, ACTOR_SAILOR_ON_THE_MATRIARCH, CONDITION_ALWAYS,
		MSGNO(MsgChampaOurMatriarchIsTheLast)},
	{CUE_TALK, ACTOR_SAILOR_ASKING_IF_WELL_LIKED, CONDITION_ALWAYS,
		SCRIPT_REF(scChampaAskIfSailorsAreWellLiked)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_YOUNG_MAN,
		CONDITION_ALWAYS, MSGNO(MsgChampaWhereCouldBriggsBeWasting)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WAITING_FOR_FOOD,
		CONDITION_ALWAYS, MSGNO(MsgChampaBriggsPromisedToBringEnough)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AT_THE_EDGE_OF_ANGARA,
		CONDITION_ALWAYS, MSGNO(MsgChampaChampaIsAllAloneOn)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_PROSPERITY,
		CONDITION_ALWAYS, MSGNO(MsgChampaIBetBriggsHasAlready)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_ANKOHL_RUINS,
		CONDITION_ALWAYS, MSGNO(MsgChampaIWantSomeoneToSolve)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_ANKOHL_RUINS,
		CONDITION_ALWAYS, MSGNO(MsgChampaTheySayTheChamberAt)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRACTICING_BOY,
		CONDITION_ALWAYS, MSGNO(MsgChampaIfICouldPickUp)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EOLEOS_FRIEND,
		CONDITION_ALWAYS, MSGNO(MsgChampaEoleosSoMuchYoungerThan)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_AT_THE_QUIET_CAVES,
		CONDITION_ALWAYS, MSGNO(MsgChampaWhyHaventBriggsAndHis)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ON_THE_MATRIARCH,
		CONDITION_ALWAYS, MSGNO(MsgChampaObabasLockedHerselfInHer)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ASKING_IF_WELL_LIKED,
		CONDITION_ALWAYS, MSGNO(MsgChampaIfBriggsWouldJustBring)},
	{CUE_TALK, ACTOR_WOMAN_ON_THE_CLIFF, CONDITION_ALWAYS,
		MSGNO(MsgChampaIBetYoureWonderingWhy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CLIFF,
		CONDITION_ALWAYS, MSGNO(MsgChampaStandingHereStaringAcrossThe)},
	{CUE_TALK, ACTOR_FEIZHIS_GIFT, CONDITION_ALWAYS,
		SCRIPT_REF(scChampaTakeFeizhisGift)},
	{CUE_TABLE_END},
};

/* Champa's cues once Briggs has sailed, the sailors' warning among them. */
CUE ChampaCuesOnceBriggsSailed[] = {
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_SOUTHWEST_A,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_SOUTHWEST_B,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_NORTH,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_WEST,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_NORTHEAST,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_EAST,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_SANCTUM,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_CLIFF_CAVES_BRIGGS,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{CUE_EXIT, CHAMPA_TO_WORLD_MAP, CONDITION_ALWAYS, CHAMPA_TO_WORLD_MAP},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_CLIFF_CAVES_HOUSES,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_CLIFF_CAVES_OBABA,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaEnterDoor)},
	{EXAMINE_CUE_FACING(FACING_NORTH), METATILE_BEHAVIOR_VILLAGE_SIGN,
		CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgChampaChampaAhead))},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaRevealTreasureSpot)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
		CONDITION_ALWAYS, SCRIPT_REF(scChampaHideTreasureSpot)},
	{CUE_STEP, METATILE_BEHAVIOR_GRAVESTONE_PATH, FLAG_FEIZHI_CAME,
		SCRIPT_REF(scChampaFeizhiArrives)},
	{CUE_STEP, METATILE_BEHAVIOR_PATH_INTO_THE_VILLAGE,
		FLAG_WALKED_INTO_THE_VILLAGE, SCRIPT_REF(scChampaWalkIntoTheVillage)},
	{CUE_STEP, METATILE_BEHAVIOR_PLANK_BRIDGE, FLAG_CHAMPA_MET_ALEX,
		SCRIPT_REF(scChampaMeetAlex)},
	{CUE_STEP, METATILE_BEHAVIOR_BEFORE_BRIGGS_CAVE, FLAG_SAILORS_SPOTTED_FELIX,
		SCRIPT_REF(scChampaSailorsSpotFelix)},
	{CUE_TALK, ACTOR_HUNGRY_YOUNG_MAN, CONDITION_ALWAYS,
		MSGNO(MsgChampaBriggsCameBackWithA)},
	{CUE_TALK, ACTOR_WOMAN_WAITING_FOR_FOOD, CONDITION_ALWAYS,
		MSGNO(MsgChampaDontTalkToMeAbout)},
	{CUE_TALK, ACTOR_MAN_AT_THE_EDGE_OF_ANGARA, CONDITION_ALWAYS,
		MSGNO(MsgChampaLetsSeeThemCallChampa)},
	{CUE_TALK, ACTOR_WOMAN_ON_PROSPERITY, CONDITION_ALWAYS,
		SCRIPT_REF(scChampaTellOfBriggsTreasure)},
	{CUE_TALK, ACTOR_OLD_MAN_ON_ANKOHL_RUINS, CONDITION_ALWAYS,
		SCRIPT_REF(scChampaAskAboutAnkohlRuins)},
	{CUE_TALK, ACTOR_OLD_WOMAN_ON_ANKOHL_RUINS, CONDITION_ALWAYS,
		MSGNO(MsgChampaBriggsLooksSoReliableSo)},
	{CUE_TALK, ACTOR_PRACTICING_BOY, CONDITION_ALWAYS,
		MSGNO(MsgChampaThoseBagsAreFullOf)},
	{CUE_TALK, ACTOR_EOLEOS_FRIEND, CONDITION_ALWAYS,
		MSGNO(MsgChampaIWantToPlayWith)},
	{CUE_TALK, ACTOR_SAILOR_AT_THE_QUIET_CAVES, CONDITION_ALWAYS,
		MSGNO(MsgChampaBriggsFoughtTheOddsTo)},
	{CUE_TALK, ACTOR_SAILOR_ON_THE_MATRIARCH, CONDITION_ALWAYS,
		SCRIPT_REF(scChampaTellOfObaba)},
	{CUE_TALK, ACTOR_SAILOR_ASKING_IF_WELL_LIKED, CONDITION_ALWAYS,
		MSGNO(MsgChampaNowNobodyHasToDo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_YOUNG_MAN,
		CONDITION_ALWAYS, MSGNO(MsgChampaBriggsIsSoGenerousHe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WAITING_FOR_FOOD,
		CONDITION_ALWAYS, MSGNO(MsgChampaErrrrgIWasSoFull)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AT_THE_EDGE_OF_ANGARA,
		CONDITION_ALWAYS, MSGNO(MsgChampaGoshHowMuchAreAll)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_PROSPERITY,
		CONDITION_ALWAYS, MSGNO(MsgChampaThingsAreGoingToBe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_ANKOHL_RUINS,
		CONDITION_ALWAYS, MSGNO(MsgChampaWereLuckyBriggsHadntFound)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_ANKOHL_RUINS,
		CONDITION_ALWAYS, MSGNO(MsgChampaBriggsWasAlwaysSuchA)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRACTICING_BOY,
		CONDITION_ALWAYS, MSGNO(MsgChampaAllOfOurStockhousesAre)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EOLEOS_FRIEND,
		CONDITION_ALWAYS, MSGNO(MsgChampaEoleoSaidHeCouldntPlay)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_AT_THE_QUIET_CAVES,
		CONDITION_ALWAYS, MSGNO(MsgChampaIfChampaGetsEnoughMoney)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ON_THE_MATRIARCH,
		CONDITION_ALWAYS, MSGNO(MsgChampaIsObabaInAGood)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ASKING_IF_WELL_LIKED,
		CONDITION_ALWAYS, MSGNO(MsgChampaNobodyReallyLikesToPlay)},
	{CUE_TALK, ACTOR_WOMAN_ON_THE_CLIFF, CONDITION_ALWAYS,
		MSGNO(MsgChampaWhenBriggsSetSailWe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CLIFF,
		CONDITION_ALWAYS, MSGNO(MsgChampaIWasJustGazingOut)},
	{CUE_TALK, ACTOR_FEIZHIS_GIFT, CONDITION_ALWAYS,
		SCRIPT_REF(scChampaTakeFeizhisGift)},
	{CUE_TABLE_END},
};

/* A question mark's script: it is released when its wait runs out. */
Uint32 ChampaQuestionMarkScript[] = {
	SC_RELEASE,
};
