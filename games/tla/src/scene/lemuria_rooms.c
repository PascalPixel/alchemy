/* Lemuria's rooms (SceneId_LemuriaRooms), a rooms map behind the city's doors:

   - Piers's uncle's house (LEMURIA_ROOMS_TO_LEMURIA_WEST_A), where Piers comes
	 home to learn his mother has died (scLemuriaRoomsPiersVisitsUncle) and his
	 uncle sends a bird with a letter to Lunpa
	 (scLemuriaRoomsLeavePiersToMourn);
   - a house where a girl who has never left Lemuria lives
	 (LEMURIA_ROOMS_TO_LEMURIA_WEST_B);
   - the Senate's hall (LEMURIA_ROOMS_TO_LEMURIA_WEST_C) and the two rooms
	 behind its arches (WAY_HALL_WEST_ARCH, WAY_HALL_EAST_ARCH), where the
	 senators hold to the sealing of Alchemy and scoff at the party's Mind
	 Read;
   - Lunpa's tower, three rooms one above another, each with its own door to
	 the city (LEMURIA_ROOMS_TO_LEMURIA_EAST_C, _EAST_A and _EAST_B, from the
	 bottom), joined by stairs and a ladder: in the bottom one, among his
	 books, Lunpa hears the party out and storms off to the king
	 (scLemuriaRoomsMeetLunpa). */

#include "game/game.h"
#include "field/script_inline.h"
#include "edition.h"
#include "scene/scene.h"
#include "field/field_script.h"
#include "text/msg_ids.h"
#include "scene/cycle_pictures.h"
#include "field/actor_script.h"
#include "field/field_mode.h"

/* Set once Piers has come home to his uncle
   (scLemuriaRoomsPiersVisitsUncle). */
#define FLAG_PIERS_VISITED_UNCLE 0x965
/* Set once an old man of the houses has felt Mind Read: "Was that some form of
   Psynergy?" */
#define FLAG_MIND_READ_NOTICED	 0x967

/* The placed actors, numbering the rows of ActorPlacements from
   ACTOR_FIRST_PLACED; the rows for Jenna, Sheba and Piers go to their party
   slots. */
typedef enum LEMURIA_ROOMS_ACTOR_ID {
	/* "I have never been outside of Lemuria." */
	ACTOR_GIRL_WHO_NEVER_LEFT = ACTOR_FIRST_PLACED,
	/* "Now that Piers has returned, maybe he'll share his tales of the outside
	   world with me." */
	ACTOR_WOMAN_AWAITING_PIERS_TALES,
	/* "Piers! You are alive!" */
	ACTOR_PIERS_UNCLE,
	/* The uncle's white bird, which carries his letter to Lunpa: "All I need is
	   a bird to carry it to him in his tower." */
	ACTOR_BIRD,
	ACTOR_KRADEN,
	/* The senators, who hold to the sealing of Alchemy. "Are you the warriors
	   from beyond the Sea of Time...?" */
	ACTOR_OLD_MAN_ASKING_OF_THE_OUTSIDE,
	/* "Long ago, the power of Alchemy was abused..." */
	ACTOR_OLD_MAN_ON_ALCHEMYS_ABUSE,
	/* "And now, rumors abound of a group of fools..." */
	ACTOR_OLD_MAN_ON_THE_RUMORS,
	/* "Come what may, I cannot approve of anyone setting Alchemy loose..." */
	ACTOR_OLD_MAN_AGAINST_ALCHEMY,
	/* "Has Lemuria fallen into decay, I ask you!? Nay!" */
	ACTOR_OLD_MAN_DENYING_DECAY,
	/* "But... Aren't you Lunpa, the Righteous Thief?" */
	ACTOR_LUNPA,
	/* "Do you seek to know more about the draughts...?" */
	ACTOR_OLD_MAN_ON_THE_DRAUGHTS
} LEMURIA_ROOMS_ACTOR_ID;

/* The metatile behaviors the rooms' step cues answer. */
typedef enum LEMURIA_ROOMS_METATILE_BEHAVIOR {
	/* Inside the door of Piers's uncle's house, where his uncle stops the party
	   on its way out (scLemuriaRoomsLeavePiersToMourn). */
	METATILE_BEHAVIOR_UNCLES_DOORWAY = 30,
	/* The foot of the stairs in Lunpa's tower, where Lunpa comes to meet the
	   party (scLemuriaRoomsMeetLunpa). */
	METATILE_BEHAVIOR_FOOT_OF_LUNPAS_STAIRS = 31
} LEMURIA_ROOMS_METATILE_BEHAVIOR;

/* The ways between the rooms, each an exit and the entrance beside it, in
   pairs: each way leads to its partner's entrance and back. */
typedef enum LEMURIA_ROOMS_WAY {
	WAY_HALL_WEST_ARCH = 4, /* to the senators' west back room */
	WAY_HALL_EAST_ARCH = 5, /* to the senators' east back room */
	WAY_WEST_BACK_ROOM = 6, /* to the hall's west arch */
	WAY_EAST_BACK_ROOM = 7, /* to the hall's east arch */
	/* The top of the stairs in Lunpa's room, its west and east halves, up to
	   the tower's middle room. */
	WAY_LUNPAS_STAIRS_WEST = 8,
	WAY_LUNPAS_STAIRS_EAST = 9,
	/* Either side of the stairwell in the middle room, down to Lunpa's. */
	WAY_MIDDLE_ROOM_WEST = 10,
	WAY_MIDDLE_ROOM_EAST = 11,
	/* The top of the middle room's stairs, up to the top room
	   (SceneRegions). */
	WAY_MIDDLE_ROOM_STAIRS = 13,
	/* The ladder down through the top room's floor (SceneRegions). */
	WAY_TOP_ROOM_LADDER = 14,
} LEMURIA_ROOMS_WAY;

/* SPRITE_WHITE_BIRD's own animations: standing, now and then stirring, and
   beating its spread wings. */
enum { ANIM_WHITE_BIRD_PERCH = 1, ANIM_WHITE_BIRD_FLAP = 3 };

/* How the rooms open: the Japanese edition opens them through a shape, the
   others from around the leader. */
#define ROOM_WINDOW_TRANSITION \
	WHEN(REVISED_ENTRANCE_TRANSITIONS, WINDOW_TRANSITION_LEADER, \
		WINDOW_TRANSITION_SHAPE)

void scLemuriaRoomsPiersVisitsUncle(void);

extern SCENE_REGION SceneRegions[];
extern Uint32		QuestionMarkScript[];

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return SceneRegions;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

/* Piers's uncle asks whether the party came with Piers, then, once Piers has
   visited him, is relieved Piers is alive, and later sends the party to Lunpa's
   tower. */
void scLemuriaRoomsTalkToPiersUncle(Sint32 actorId)
{
	if(fdScriptGetFlag(FLAG_PIERS_AT_MOTHERS_GRAVE) != 0) {
		fdScriptSetMessageId((Sint32)MsgLemuriaRoomsGoToLunpasTowerHe);
		/* "Go to Lunpa's tower. He will see you." */
		fdScriptShowMessage(actorId, 0);
		fdActorWatchAndSetScript(ACTOR_PIERS_UNCLE, ACTOR_FELIX);
	} else if(fdScriptGetFlag(FLAG_PIERS_VISITED_UNCLE) != 0) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgLemuriaRoomsIAmSoRelieved);
		/* "I am so relieved that Piers has returned to me alive. He is the
		   last..." */
		fdScriptShowMessage(actorId, 0);
		fdActorFaceDirection(actorId, FACING_SOUTH_SOUTHEAST, 0);
		fdScriptEnd();
	} else {
		fdScriptSetMessageId((Sint32)MsgLemuriaRoomsAreYouTravelersThatCame);
		/* "Are you the travelers that came here with Piers?" */
		fdScriptOpenMessage(actorId, 0);
		if(syFieldScriptAskYesNoWithActorAnimation(
			   syFieldPartyGetLeaderActor(), 0) == 0) {
			scLemuriaRoomsPiersVisitsUncle();
		} else {
			fdScriptWait(20);
			/* No: "No? Ah, interesting. I wonder when Piers will come to see
			   me." */
			fdScriptShowMessage(actorId, 0);
			fdActorFaceDirection(actorId, FACING_WEST, 0);
		}
	}
}

/* Mind Read on Piers's uncle: his thoughts follow how far Piers's homecoming
   has gone. */
void scLemuriaRoomsMindReadPiersUncle(Sint32 unused, Sint32 id)
{
	if(fdScriptGetFlag(FLAG_PIERS_AT_MOTHERS_GRAVE)) {
		fdScriptSetMessageId(
			(Sint32)MsgLemuriaRoomsLunpaHasGivenKingTremendous);
		/* "Lunpa has given the king tremendous aid on this matter. He will
		   have..." */
		fdScriptShowMessage(id, 0);
	} else if(fdScriptGetFlag(FLAG_PIERS_VISITED_UNCLE)) {
		fdScriptSetMessageId((Sint32)MsgLemuriaRoomsPoorPiersBeingBorn);
		/* "Poor Piers... Being born in Lemuria and gifted with long
		   life......" */
		fdScriptShowMessage(id, 0);
	} else {
		fdScriptSetMessageId((Sint32)MsgLemuriaRoomsIWaswhatIsWordSurprised);
		/* "I was°what is the word?° surprised when I heard Piers had..." */
		fdScriptShowMessage(id, 0);
	}
}

/* In the hall of the House of the Senate, an old senator asks whether the party
   came from beyond the Sea of Time, and scorns either answer. */
void scLemuriaRoomsAskFromBeyondSeaOfTime(Sint32 actorId)
{
	fdScriptSetMessageId((Sint32)MsgLemuriaRoomsAreYouWarriorsFromBeyond);
	/* "Are you the warriors from beyond the Sea of Time who returned
	   with..." */
	fdScriptOpenMessage(actorId, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(
		   syFieldPartyGetLeaderActor(), 0) == 0) {
		fdScriptWait(10);
		/* Yes: "What can you uncivilized louts hope to bring to Lemuria?" */
		fdScriptSetMessageId(
			(Sint32)MsgLemuriaRoomsAreYouWarriorsFromBeyond + 1);
	} else {
		fdScriptWait(20);
		/* No: "Only savages and barbarians are wont to tell lies when
		   confronted..." */
		fdScriptSetMessageId(
			(Sint32)MsgLemuriaRoomsAreYouWarriorsFromBeyond + 2);
	}
	fdScriptShowMessage(actorId, 0);
}

/* In a room behind the Senate hall's arches, a senator offers to tell of the
   draughts that kept Lemurians long-lived, and tells of them either way. */
void scLemuriaRoomsAskAboutDraughts(Sint32 actorId)
{
	fdScriptSetMessageId((Sint32)MsgLemuriaRoomsDoYouSeekToKnow);
	/* "Do you seek to know more about the draughts that provide us with..." */
	fdScriptOpenMessage(actorId, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(
		   syFieldPartyGetLeaderActor(), 0) == 0) {
		fdScriptWait(10);
		/* Yes: "Once, Lemurians bottled special draughts to leave here and
		   travel..." */
		fdScriptSetMessageId((Sint32)MsgLemuriaRoomsDoYouSeekToKnow + 1);
	} else {
		fdScriptWait(20);
		/* No: "Then I shall tell you this: So long as we drink of the
		   Lemurian..." */
		fdScriptSetMessageId((Sint32)MsgLemuriaRoomsDoYouSeekToKnow + 2);
	}
	fdScriptShowMessage(actorId, 0);
}

/* The first senator Mind Read touches turns on the party, asks whether that was
   Psynergy, and laughs it off: Lemurians are masters of Psynergy. */
void scLemuriaRoomsNoticeMindRead(Sint32 unused, Sint32 actorId)
{
	Sint32 facing;

	facing = fdActorGetById(actorId)->facing;
	fdScriptSetFlag(FLAG_MIND_READ_NOTICED);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceActor(actorId, ACTOR_FELIX, 20);
	fdScriptSetMessageId((Sint32)MsgLemuriaRoomsWasThatSomeForm);
	/* "Was that some form of Psynergy?" */
	fdScriptOpenMessage(actorId, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(
		   syFieldPartyGetLeaderActor(), 0) == 0) {
		fdScriptWait(10);
		/* Yes: "Ho ho ho! We Lemurians are masters of Psynergy all! Your..." */
		fdScriptSetMessageId((Sint32)MsgLemuriaRoomsWasThatSomeForm + 1);
	} else {
		fdScriptWait(20);
		/* No: "No? Curious. But it does not matter. Lemurians are masters
		   of..." */
		fdScriptSetMessageId((Sint32)MsgLemuriaRoomsWasThatSomeForm + 2);
	}
	fdScriptShowMessage(actorId, 0);
	fdActorFaceDirection(actorId, facing, 0);
	fdScriptWait(10);
	fdScriptEnd();
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* Sets the rooms up: once Piers is at his mother's grave his uncle stands where
   he sent his bird from, and once Lunpa has gone to the king his tower is
   empty. */
Sint32 scInit(void)
{
	ACTOR *actor;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, ROOM_WINDOW_TRANSITION);
	fdActorSetSprPriority(ACTOR_BIRD, 1);
	fdActorSetSprPriority(ACTOR_OLD_MAN_ON_ALCHEMYS_ABUSE, 2);
	fdActorSetSprPriority(ACTOR_PIERS_UNCLE, 2);
	actor = fdActorGetById(ACTOR_OLD_MAN_ON_THE_RUMORS);
	actor->motionFlg = 0;
	actor->groundHeight = 0;
	actor->y.fixed = 0;
	actor = fdActorGetById(ACTOR_OLD_MAN_DENYING_DECAY);
	actor->motionFlg = 0;
	actor->groundHeight = 0;
	actor->y.fixed = 0;
	if(fdScriptGetFlag(FLAG_PIERS_AT_MOTHERS_GRAVE)) {
		fdActorWalkToAndWait(ACTOR_PIERS_UNCLE, 100, 112);
		fdActorGetById(ACTOR_PIERS_UNCLE)->facing = FACING_SOUTH_SOUTHEAST;
		sySynch(1);
	}
	if(fdScriptGetFlag(FLAG_MET_LUNPA))
		fdActorSetPos(ACTOR_LUNPA, 0, 0);
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* Piers comes home to his uncle, who is relieved to find him alive, and learns
   that his mother has died; he runs out to her grave. */
void scLemuriaRoomsPiersVisitsUncle(void)
{
	ACTOR *leader;

	fdScriptSetFlag(FLAG_PIERS_VISITED_UNCLE);
	gmPartyRemoveMember(PIERS);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgLemuriaRoomsIAmSorryItHas);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTH_SOUTHEAST, 0);
	fdActorWalkToAndWait(ACTOR_FELIX, 112, 128);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 16, 0, FACING_NORTH_NORTHWEST);
	scFieldActorLaunchFromLeader(ACTOR_PIERS, 16, -16, FACING_WEST);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorWaitForMoveById(ACTOR_PIERS);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdScriptWait(10);
	/* "I am sorry it has taken me so long to visit, Uncle." */
	fdScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_EAST, 0);
	fdScriptWait(20);
	fdActorJump(ACTOR_PIERS_UNCLE, 6, 15);
	fdActorJump(ACTOR_PIERS_UNCLE, 6, 23);
	/* "Piers! You are alive!" */
	fdScriptShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdScriptWait(10);
	/* "Yes, I've managed to survive against a great many trials." */
	fdScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 45);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdScriptWait(20);
	/* "What a fascinating coincidence! We've stumbled across Piers's..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTHEAST, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "I must thank you all for looking after my nephew." */
	fdScriptShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
	fdScriptWait(30);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdScriptWait(45);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdScriptWait(20);
	/* "This is Felix and Kraden. They are my friends, and have aided me..." */
	fdScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_EAST, 0);
	fdScriptWait(20);
	/* "If my sister could only hear how timid young Piers has become a..." */
	fdScriptShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_PIERS_UNCLE, 2);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTH_SOUTHEAST, 0);
	fdScriptWait(30);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 45);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	fdScriptWait(35);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdScriptWait(35);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	fdScriptWait(35);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdScriptWait(35);
	/* "Where is my mother? We've been so busy since we arrived that I..." */
	fdScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_PIERS_UNCLE, 2);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorWalkByAndWait(ACTOR_PIERS, 48, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	fdScriptWait(40);
	fdActorWalkByAndWait(ACTOR_PIERS, -24, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdScriptWait(40);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Now that I think about it, this place is a mess. What happened?..." */
	fdScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "...No! She hasn't...fallen ill again, has she?" */
	fdScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(20);
	/* "There is still illness in Lemuria?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "My mother was born with a weak heart..." */
	fdScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_EAST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_SHAKE_HEAD);
	/* "Piers... I'm sorry... Your return comes too late..." */
	fdScriptShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdScriptWait(50);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_PIERS, 3);
	fdActorSetAttachedEffect(
		ACTOR_PIERS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(50);
	fdActorWalkByAndWait(ACTOR_PIERS, -12, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "After the tidal wave washed you to sea, your mother suddenly fell
	   ill..." */
	fdScriptShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_NOD);
	fdScriptWait(10);
	/* "The shock of losing you was too great for her poor heart." */
	fdScriptShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_SHAKE_HEAD);
	/* "She held out hope for your safe return until the very end." */
	fdScriptShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_NORTH_NORTHWEST, 0);
	fdScriptWait(30);
	/* "Perhaps your mother gave herself up to ensure your survival..." */
	fdScriptShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_NOD);
	fdScriptWait(10);
	/* "Does that not sound like your mother?" */
	fdScriptShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_LEADER_LOOK_AROUND);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(20);
	/* "Where is she now?" */
	fdScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_EAST, 0);
	fdScriptWait(20);
	/* "She rests in the cemetery now. Check the headstones... You will
	   find..." */
	fdScriptShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_STAND);
	fdScriptWait(30);
	fdActorSetSpeed(ACTOR_PIERS, 0.3);
	fdActorGetById(ACTOR_PIERS)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_PIERS, 12, 0);
	fdScriptWait(1);
	fdActorGetById(ACTOR_PIERS)->steeringFlg |= ACTOR_FACES_MOTION;
	fdScriptWait(10);
	fdActorStartRepeatedMotion(ACTOR_PIERS, 3);
	fdActorSetAttachedEffect(
		ACTOR_PIERS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(55);
	fdActorWatchAndSetScript(ACTOR_FELIX, ACTOR_PIERS);
	fdActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_PIERS);
	fdActorWatchAndSetScript(ACTOR_PIERS_UNCLE, ACTOR_PIERS);
	fdActorSetSpeed(ACTOR_PIERS, 2.2);
	fdActorWalkByAndWait(ACTOR_PIERS, -8, 88);
	fdActorStop(ACTOR_FELIX);
	fdActorStop(ACTOR_KRADEN);
	fdActorStop(ACTOR_PIERS_UNCLE);
	fdActorSetPos(ACTOR_PIERS, 0, 0);
	fdScriptWait(30);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdScriptWait(20);
	/* "What should we do, Felix? Should we follow Piers?" */
	fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdScriptWait(20);
		/* Yes: "After all we've been through, the least we can do is
		   offer..." */
		fdScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
		FieldStateWork->msg += 1;
	} else {
		fdScriptWait(40);
		FieldStateWork->msg += 1;
		/* No: "You're right. Piers probably needs some time alone right
		   now." */
		fdScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
	}
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestination(
			ACTOR_KRADEN, leader->x.part[0].integer, leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdScriptEnd();
}

/* Ticks of each of the bird's two loops round the room. */
#define BIRD_FLIGHT_TICKS 192

/* Once Piers has gone to his mother's grave, his uncle stops the party on its
   way out, asks it to leave Piers to his mourning, tells of the mission the
   king gave Piers, and sends his bird with a letter so that Lunpa will see the
   party. */
void scLemuriaRoomsLeavePiersToMourn(void)
{
	ACTOR  *actor;
	Sint32	startBuf[3], tgtBuf[3], posBuf[3];
	Sint32 *start, *tgt, *pos;
	FIXED	radius;
	Sint32	ang;
	Sint32	result, frm, phase, sine, answer;

	actor = fdActorGetById(ACTOR_BIRD);
	result = fdScriptGetFlag(FLAG_PIERS_VISITED_UNCLE);
	if(result == 0)
		return;
	fdScriptSetFlag(FLAG_PIERS_AT_MOTHERS_GRAVE);
	{
		ACTOR *other = fdActorGetById(ACTOR_PIERS_UNCLE);
		other->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	}
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgLemuriaRoomsWait);
	fdActorSetSpeed(ACTOR_PIERS_UNCLE, 0.8);
	fdActorShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Wait..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	fdActorWalkToAndWait(ACTOR_FELIX, 136, 152);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_SHAKE_HEAD);
	/* "I ask that you leave Piers to his mourning for now..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	fdActorShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Now he is struggling with the loss of his mother. He needs time." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
	syFieldScriptWait(30);
	fdActorWalkByAndWait(ACTOR_FELIX, 8, 16);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorRunRepeatedMotion(ACTOR_PIERS_UNCLE, 2);
	/* "You came to Lemuria to learn more of Alchemy, did you not?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
	syFieldScriptWait(30);
	fdActorWalkByAndWait(ACTOR_FELIX, -8, -16);
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_NOD);
	syFieldScriptWait(10);
	/* "I suspected as much. Piers was right to bring you here." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_SHAKE_HEAD);
	/* "Piers told me his secret... The mission he was given by His
	   Highness..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_NOD);
	syFieldScriptWait(10);
	/* "Even had the tidal wave not carried Piers off, he would have left..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	scFieldActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorSetAttachedEffect(
		ACTOR_FELIX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(50);
	fdActorWalkByAndWait(ACTOR_PIERS_UNCLE, 12, 12);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "You look confused... Piers told you about this, did he not?" */
	fdScriptOpenMessage(ACTOR_PIERS_UNCLE, 0);
	answer = fdScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0);
	if(answer == 0) {
		syFieldScriptWait(30);
		/* Yes: "Of course... He must trust you a great deal." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
		FieldStateWork->msg = (FieldStateWork->msg) + 1;
	} else {
		syFieldScriptWait(40);
		FieldStateWork->msg = (FieldStateWork->msg) + 1;
		/* No: "Curious... I can't understand why he would do that..." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	}
	fdActorSetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_NOD);
	syFieldScriptWait(10);
	/* "If Lord Conservato had not led the senate to oppose him, he would..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(30);
	/* "I wonder what news Piers has brought with him." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_PIERS_UNCLE, 2);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTH_SOUTHEAST, 0);
	syFieldScriptWait(20);
	/* "Either way, I do know why he has brought you here." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	scFieldActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorSetAttachedEffect(
		ACTOR_FELIX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(50);
	fdActorShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Felix... Don't tell me that he hasn't at least told you that much!" */
	fdScriptOpenMessage(ACTOR_PIERS_UNCLE, 0);
	answer = fdScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0);
	if(answer == 0) {
		syFieldScriptWait(30);
		/* Yes: "You say he's told you, but you look rather confused." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
		FieldStateWork->msg = (FieldStateWork->msg) + 1;
	} else {
		syFieldScriptWait(40);
		FieldStateWork->msg = (FieldStateWork->msg) + 1;
		/* No: "Did Piers tell you nothing before bringing you here to
		   Lemuria?" */
		syFieldScriptShowMessageThenWait(
			SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	}
	fdActorShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Unfortunately, I know little more than that..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 20);
	fdActorShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	/* "While Piers confronts his sorrow over the loss of his mother, you..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	fdActorWalkToAndWait(ACTOR_PIERS_UNCLE, 89, 92);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_WEST, 0);
	syFieldScriptWait(20);
	/* "I shall write you a letter..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_PIERS_UNCLE, 2);
	syFieldScriptWait(10);
	/* "So that you can see Lunpa..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTHEAST, 0);
	syFieldScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_SHAKE_HEAD);
	/* "Lunpa has locked himself in his tower and rarely comes out, but if
	   I..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_WEST, 0);
	syFieldScriptWait(30);
	fdActorShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
	fdActorSetAnimation(ACTOR_PIERS_UNCLE, ANIM_NOD);
	syFieldScriptWait(10);
	fdScriptPlaySnd(SE_BIRD_CALL);
	syFieldScriptWait(60);
	scFieldActorStartRepeatedMotion(ACTOR_BIRD, 3);
	fdActorSetAttachedEffect(
		ACTOR_BIRD, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(10);
	scFieldActorStartRepeatedMotion(ACTOR_BIRD, 3);
	fdActorSetAttachedEffect(
		ACTOR_BIRD, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(50);
	fdActorSetAnimation(ACTOR_BIRD, ANIM_WHITE_BIRD_FLAP);
	actor->motionFlg = 0;
	start = startBuf;
	start[0] = actor->x.fixed;
	start[1] = actor->y.fixed;
	start[2] = actor->z.fixed;
	tgt = tgtBuf;
	tgt[0] = toFIXED(116);
	tgt[1] = 0;
	tgt[2] = toFIXED(110);
	ang = FACING_NORTH_NORTHWEST;
	for(frm = 0; frm < BIRD_FLIGHT_TICKS; frm++) {
		phase = frm * (DEGtoANG(180)) / BIRD_FLIGHT_TICKS;
		radius = sySin(phase) * 32;
		ang += TRIG_TURN / 128;
		pos = posBuf;
		pos[0] = start[0] + (tgt[0] - start[0]) * frm / BIRD_FLIGHT_TICKS;
		sine = sySin(phase);
		pos[1] = start[1] + (tgt[1] - start[1]) * frm / BIRD_FLIGHT_TICKS +
			sine * 56;
		pos[2] = start[2] + (tgt[2] - start[2]) * frm / BIRD_FLIGHT_TICKS;
		syVectorAddPolarOffset(radius, ang, pos);
		actor->x.fixed = pos[0];
		actor->y.fixed = pos[1];
		actor->z.fixed = pos[2];
		actor->facing = ang + DEGtoANG(90);
		if(frm == 128) {
			fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTH_SOUTHEAST, 0);
		}
		syWait(1);
	}
	actor->facing = FACING_WEST;
	syFieldScriptWait(30);
	fdActorSetAnimation(ACTOR_BIRD, ANIM_WHITE_BIRD_PERCH);
	syFieldScriptWait(20);
	fdActorSetSpeed(ACTOR_PIERS_UNCLE, 0.8);
	fdActorWalkToAndWait(ACTOR_PIERS_UNCLE, 100, 112);
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_EAST, 0);
	syFieldScriptWait(20);
	fdActorSetSpeed(ACTOR_PIERS_UNCLE, 0.4);
	fdActorWalkByAndWait(ACTOR_PIERS_UNCLE, 8, 0);
	syFieldScriptWait(20);
	fdActorSetAnimation(ACTOR_BIRD, ANIM_WHITE_BIRD_FLAP);
	syFieldScriptWait(40);
	fdActorSetAnimation(ACTOR_BIRD, ANIM_WHITE_BIRD_PERCH);
	syFieldScriptWait(20);
	{
		ACTOR *other = fdActorGetById(ACTOR_PIERS_UNCLE);
		other->steeringFlg &= ~ACTOR_FACES_MOTION;
	}
	fdActorWalkByAndWait(ACTOR_PIERS_UNCLE, -8, 0);
	syFieldScriptWait(1);
	{
		ACTOR *other = fdActorGetById(ACTOR_PIERS_UNCLE);
		other->steeringFlg |= ACTOR_FACES_MOTION;
	}
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTH_SOUTHEAST, 0);
	syFieldScriptWait(20);
	/* "All I need is a bird to carry it to him in his tower." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_EAST, 0);
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	fdActorSetAnimation(ACTOR_BIRD, ANIM_WHITE_BIRD_FLAP);
	syFieldScriptWait(40);
	actor->motionFlg = 0;
	start[0] = actor->x.fixed;
	start[1] = actor->y.fixed;
	start[2] = actor->z.fixed;
	tgt[0] = toFIXED(132);
	tgt[1] = toFIXED(32);
	tgt[2] = toFIXED(100);
	ang = FACING_SOUTH + TRIG_TURN / 128;
	for(frm = 0; frm < BIRD_FLIGHT_TICKS; frm++) {
		radius = sySin(frm * (DEGtoANG(90)) / BIRD_FLIGHT_TICKS) * 40;
		ang += TRIG_TURN * 3 / 256;
		pos[0] = start[0] + (tgt[0] - start[0]) * frm / BIRD_FLIGHT_TICKS;
		sine = sySin(frm * (DEGtoANG(180)) / BIRD_FLIGHT_TICKS);
		pos[1] = start[1] + (tgt[1] - start[1]) * frm / BIRD_FLIGHT_TICKS +
			(sine <<= 5);
		pos[2] = start[2] + (tgt[2] - start[2]) * frm / BIRD_FLIGHT_TICKS;
		syVectorAddPolarOffset(radius, ang, pos);
		actor->x.fixed = pos[0];
		actor->y.fixed = pos[1];
		actor->z.fixed = pos[2];
		actor->facing = ang + DEGtoANG(90);
		if(frm == 153) {
			fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_WEST_SOUTHWEST, 0);
		}
		syWait(1);
	}
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_NORTH_NORTHWEST, 0);
	actor->facing = FACING_NORTH_NORTHEAST;
	start[0] = actor->x.fixed;
	start[1] = actor->y.fixed;
	start[2] = actor->z.fixed;
	tgt[0] = toFIXED(90);
	tgt[1] = toFIXED(4);
	tgt[2] = toFIXED(64);
	for(frm = 0; frm < 16; frm++) {
		actor->x.fixed = start[0] + (tgt[0] - start[0]) * frm / 16;
		actor->y.fixed = start[1] + (tgt[1] - start[1]) * frm / 16;
		actor->z.fixed = start[2] + (tgt[2] - start[2]) * frm / 16;
		syWait(1);
	}
	fdActorSetPos(ACTOR_BIRD, 0, 0);
	syFieldScriptWait(30);
	fdActorFaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTH_SOUTHEAST, 0);
	syFieldScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_NOD);
	syFieldScriptWait(10);
	/* "There. You should be able to see him now." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0, 10);
	scFieldActorSetSprPriority(ACTOR_PIERS_UNCLE, 2);
	fdScriptEnd();
}

#include "shared/question_mark_pop.h"

#include "shared/actor_pop_question_marks.h"

#include "shared/actor_cycle_pictures.h"

/* Lunpa comes to meet the party in his tower and asks after Piers, learns from
   Kraden that his old friend Babi has died and that his grandson Dodonpa rules
   the village he settled, hears Kraden's theory of the world's decay, and
   bursts his stuck door open to take the party to the king. */
void scLemuriaRoomsMeetLunpa(void)
{
	ACTOR *leader;
	ACTOR *actor;
	ACTOR *tgt;
	Sint32 result;

	fdScriptSetFlag(FLAG_MET_LUNPA);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgLemuriaRoomsHonestlyImAmazedThatAnyone);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorSetSpeed(ACTOR_LUNPA, 1);
	fdActorWalkByAndWait(ACTOR_LUNPA, 32, 0);
	fdActorWalkByAndWait(ACTOR_LUNPA, 0, -32);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(10);
	/* "Honestly, I'm amazed that anyone can travel so far across the
	   world..." */
	fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	fdActorWalkToAndWait(ACTOR_FELIX, 440, 352);
	fdActorSetPos(ACTOR_FELIX, 440, 352);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 16, 0, FACING_SOUTH_SOUTHWEST);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, -16, 0, FACING_SOUTH);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, -32, 0, FACING_SOUTH);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorWaitForMoveById(ACTOR_SHEBA);
	fdScriptWait(10);
	fdCameraMoveTo(432, CAMERA_KEEP, 368, 1);
	fdCameraWaitForArrival();
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Say, Piers isn't with you! Er...why not?" */
	fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	/* "His mother passed away... He's gone to visit her grave." */
	fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptWait(30);
	/* "Of course... His mother... He loved her dearly. A terrible
	   loss......" */
	fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(30);
	/* "Well, Piers may not be here, but we still have much to discuss." */
	fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "If Piers brought you, you must be working to break the seal on..." */
	fdScriptOpenMessage(ACTOR_LUNPA, 0);
	result = syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0);
	if(result == 0) {
		fdScriptWait(20);
		/* Yes: "Ah, Piers has done quite well in finding you!" */
		fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
		FieldStateWork->msg++;
	} else {
		fdScriptWait(35);
		FieldStateWork->msg++;
		/* No: "That doesn't make sense... He wouldn't have brought you
		   to..." */
		fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	}
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "What mission could the good King Hydros have assigned to Piers?" */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "What? Piers has told you nothing of his mission?" */
	fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdActorStartRepeatedMotion(ACTOR_LUNPA, 3);
	fdActorSetAttachedEffect(
		ACTOR_LUNPA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	/* "...Really?" */
	fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdActorRunRepeatedMotion(ACTOR_LUNPA, 2);
	fdScriptWait(10);
	fdScriptWait(15);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "So you actually have nothing at all to do with Alchemy!?" */
	fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdScriptWait(25);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Our parents were kidnapped by the Fire Clan, far to the north. We
	   are..." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "King Hydros told me that Venus and Mercury have been ignited once..." */
	fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdScriptWait(15);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, ANIM_SHAKE_HEAD);
	/* "I'm afraid I know nothing of any northern "Fire Clan."" */
	fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 20);
	fdActorWalkByAndWait(ACTOR_LUNPA, 12, 0);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "But that doesn't explain you, sir. You're clearly not related to..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	fdScriptWait(10);
	/* "Kraden joined us on this quest on behalf of Lord Babi of Tolbi." */
	fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorSetSpeed(ACTOR_LUNPA, 2);
	fdActorWalkByAndWait(ACTOR_LUNPA, -12, 0);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHWEST, 0);
	fdScriptWait(10);
	/* "...Babi?" */
	fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdScriptWait(30);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "I only know of one man named Babi... But it couldn't..." */
	fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Master Lunpa, the Babi we speak of is the same man you knew long
	   ago." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
	fdScriptWait(30);
	/* "Ah! So...he's still out there, living in the outside world?" */
	fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	/* "Well, that might not be the most accurate way of putting it." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 45);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_LUNPA, 0.8);
	fdActorWalkByAndWait(ACTOR_LUNPA, 12, 0);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(20);
	/* "Most...accurate? My, you certainly have an odd way of turning a
	   phrase..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "He was living, yes... Until recently, that is..." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdScriptWait(10);
	/* "So, Babi passed away... But only recently, you say?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);
	/* "When he finally ran out of his mystic draughts, his spirit began to
	   wane." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_LUNPA, 2);
	fdScriptWait(10);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, ANIM_SHAKE_HEAD);
	/* "Then...I no longer know anyone in the world outside." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	/* "But... Aren't you Lunpa, the Righteous Thief?" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorWalkByAndWait(ACTOR_LUNPA, -12, 0);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHWEST, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "I was called that once, long, long ago. Why do you ask?" */
	fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "I'll bet there's at least one person in the outside world whom
	   you..." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptWait(30);
	scActorPopQuestionMarks(ACTOR_LUNPA);
	fdScriptPlaySnd(SE_LIGHT_APPEAR);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHWEST, 0);
	fdScriptWait(25);
	/* "Who would that be?" */
	fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(10);
	/* "There's a town called Lunpa in northern Angara. A man named Donpa..." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "I settled that village! And Donpa... He is my son..." */
	fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Now, your grandson, Dodonpa, rules over your village." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorSetSpeed(ACTOR_LUNPA, 2);
	fdActorWalkByAndWait(ACTOR_LUNPA, 12, 0);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Rules? Rules is an ill- sounding word." */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	/* "But it's an accurate one, Lunpa. Dodonpa is an evil thief, who
	   has..." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorWalkByAndWait(ACTOR_LUNPA, -12, 0);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHWEST, 0);
	fdScriptWait(10);
	fdActorJump(ACTOR_LUNPA, 6, 15);
	fdActorJump(ACTOR_LUNPA, 6, 23);
	/* "What has become of my son? How could he permit such a thing to
	   happen?" */
	fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Master Lunpa, don't you have any idea how old Donpa is?" */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(20);
	scFieldActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "He is as I am. An old man... He may be older even than I." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	scFieldActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	fdScriptWait(10);
	/* "If this news outrages you, why don't you go and set Dodonpa
	   straight?" */
	fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHWEST, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "If only I could..." */
	fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdScriptWait(20);
	/* "Is this the only reason we're here? To bring news of the outside
	   world?" */
	fdScriptOpenMessage(ACTOR_JENNA, 0);
	result = syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0);
	if(result == 0) {
		fdScriptWait(20);
		fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
		fdScriptWait(10);
		/* Yes: "You are kind, Felix, but there are much more important
		   matters..." */
		fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
		FieldStateWork->msg++;
	} else {
		fdScriptWait(35);
		fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_LUNPA, ANIM_SHAKE_HEAD);
		FieldStateWork->msg++;
		/* No: "Yes, yes... I am sorry we were sidetracked. We have much
		   more..." */
		fdScriptShowMessageThenWait(ACTOR_LUNPA, 0, 10);
	}
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(10);
	/* "But before we begin, there is one thing I must say..." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdScriptWait(25);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);
	/* "You don't mind, do you, Felix?" */
	fdScriptOpenMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	result = syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0);
	if(result == 0) {
		fdScriptWait(20);
		/* Yes: "Clearly, your judgment on this matter is fogged, Felix. I must
		   speak." */
		fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
		FieldStateWork->msg++;
	} else {
		fdScriptWait(35);
		FieldStateWork->msg++;
		/* No: "I knew you'd agree." */
		fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	}
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	/* A sliver short of west: from the east Felix, and Jenna below, turn round
	   by the south, toward the viewer, where west itself would take them round
	   by the north. */
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST - 1, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);
	/* "I have discussed this with none but Felix, but...I have my own..." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST - 1, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdScriptWait(40);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdScriptWait(30);
	fdActorStartRepeatedMotion(ACTOR_SHEBA, 3);
	fdActorSetAttachedEffect(
		ACTOR_SHEBA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	/* "What do you mean?" */
	fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);
	/* "Only that I have my own reasons for wanting to see the beacons
	   lit..." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST - 1, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdScriptWait(40);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 3);
	fdActorSetAttachedEffect(
		ACTOR_JENNA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_SHEBA, 3);
	fdActorSetAttachedEffect(
		ACTOR_SHEBA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdScriptWait(30);
	/* "Why are you telling us all of this now?" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);
	/* "Because I feel this quest is deeply linked to Lemuria." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptWait(25);
	/* "I thought it odd that Lord Babi should order me to research Alchemy." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Hey, wait a minute... Is this what you promised to tell us once
	   we..." */
	fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);
	/* "Yes, yes... May I continue?" */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_KRADEN, 0.6);
	fdActorWalkByAndWait(ACTOR_KRADEN, 12, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdScriptWait(20);
	fdActorWalkByAndWait(ACTOR_KRADEN, 12, 0);
	fdScriptWait(10);
	/* "Long ago, before Alchemy's power was sealed... There were many..." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "At the moment that Alchemy was sealed away, most of those..." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(10);
	/* "I have formed certain theories now, suggesting that this may be..." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_LUNPA, 3);
	fdActorSetAttachedEffect(
		ACTOR_LUNPA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(50);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Let me get this straight... You felt you couldn't tell us this
	   until..." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	/* A sliver past west: from the east Kraden turns round by the north, his
	   back to the viewer. */
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST + 1, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "And you only wanted to come to Lemuria to test your theories and
	   see..." */
	fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_LUNPA, 2);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	/* "But even if everything you said is true, what does it all mean?" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_LUNPA, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_LUNPA, 1);
	fdActorWalkByAndWait(ACTOR_LUNPA, 12, 0);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(10);
	/* "If your theories are correct, the world itself will wane and..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 45);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Piers never spoke of this to you, did he?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorWalkByAndWait(ACTOR_KRADEN, -12, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptWait(15);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
	fdScriptWait(10);
	/* "If you came to this theory on your own, you are indeed a wise..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "What do you mean?" */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "King Hydros and I, as well as others in Lemuria, hold the same..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdScriptWait(40);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "You do!?!" */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdScriptWait(25);
	/* "Civilizations thrive by building upon the knowledge of the past." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptWait(25);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
	fdScriptWait(10);
	/* "Knowledge gained in one generation should be taken even further
	   by..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, ANIM_SHAKE_HEAD);
	/* "But where is the world today?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptWait(30);
	/* "The great civilizations of old have all withered and vanished. We
	   are..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdScriptWait(30);
	/* "You have encountered many cultures in your travels. Are any of
	   them..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptWait(30);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_SHAKE_HEAD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, ANIM_SHAKE_HEAD);
	/* "No, they are not... But this is not the only sign of the decay
	   you..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorRunRepeatedMotion(ACTOR_LUNPA, 2);
	fdScriptWait(10);
	/* "I think it best that you hear the rest directly from His Majesty..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST + FACING_NUDGE, 0);
	fdScriptWait(20);
	/* "Yes, Kraden! We shall go to visit the king!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Ah, but King Hydros is in counsel with Lord Conservato. We might
	   not..." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SCOWL, 42);
	fdActorJump(ACTOR_LUNPA, 6, 15);
	fdActorJump(ACTOR_LUNPA, 6, 23);
	/* "Conservato? That fool. He refuses to acknowledge what he can see
	   with..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorWalkByAndWait(ACTOR_LUNPA, -2, -12);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
	fdScriptWait(10);
	/* "I will meet with the king and press our evidence upon Conservato...." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorSetSpeed(ACTOR_LUNPA, 1.6);
	fdActorWalkByAndWait(ACTOR_LUNPA, -8, 40);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorWalkByAndWait(ACTOR_LUNPA, 0, 20);
	fdMapGridSetShake(1, 1, 1);
	fdActorSetSpeed(ACTOR_LUNPA, 1.9);
	fdScriptPlaySnd(SE_IMPACT);
	fdActorJump(ACTOR_LUNPA, 2, 0);
	fdActorGetById(ACTOR_LUNPA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_LUNPA, 0, -12);
	fdScriptWait(1);
	fdActorGetById(ACTOR_LUNPA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdMapGridSetShake(0, 0, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Gah! Stupid door!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorSetSpeed(ACTOR_LUNPA, 0.6);
	fdActorGetById(ACTOR_LUNPA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_LUNPA, 0, -24);
	fdScriptWait(1);
	fdActorGetById(ACTOR_LUNPA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
	fdScriptWait(20);
	fdActorJump(ACTOR_LUNPA, 6, 15);
	fdActorJump(ACTOR_LUNPA, 6, 23);
	/* Lunpa flashes white as he gathers himself to charge the door. */
	actor = fdActorGetById(ACTOR_LUNPA);
	actor->tmr = 0;
	actor->update = (OBJECT_UPDATE)scActorCyclePictures;
	/* "HAHHH-AHH!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorSetSpeed(ACTOR_LUNPA, 2.2);
	fdActorWalkByAndWait(ACTOR_LUNPA, 0, 32);
	tgt = fdActorGetById(ACTOR_LUNPA);
	fdScriptPlaySnd(SE_HEAVY_IMPACT);
	fdMapGridSetShake(4, 4, 1);
	fdActorGetById(ACTOR_LUNPA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_LUNPA, 0, -16);
	fdScriptWait(1);
	fdActorGetById(ACTOR_LUNPA)->steeringFlg |= ACTOR_FACES_MOTION;
	/* One more flash, back to his own colours, and the flashing stops. */
	tgt->tmr = 3;
	sySynch(1);
	tgt->update = NULL;
	fdActorSetSprFlags((Sint32)fdActorGetById(ACTOR_LUNPA), 1);
	fdScriptWait(40);
	fdMapGridSetShake(0, 0, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Pah! At last, you are open!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorSetAttachedEffect(
		ACTOR_FELIX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 3);
	fdActorSetAttachedEffect(
		ACTOR_JENNA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_SHEBA, 3);
	fdActorSetAttachedEffect(
		ACTOR_SHEBA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(25);
	/* "To the king's chambers!" */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
	fdActorWalkByAndWait(ACTOR_LUNPA, 0, 48);
	fdActorSetPos(ACTOR_LUNPA, 0, 0);
	fdScriptWait(30);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	/* "Lunpa ought to be as old as Lord Babi was, but he has so much
	   energy!" */
	fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	/* "He's got a fire in him." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorJump(ACTOR_KRADEN, 6, 15);
	fdActorJump(ACTOR_KRADEN, 6, 23);
	/* "I do wish I could have heard more about Lord Babi, though." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdScriptWait(15);
	/* "But come, Felix! To the king's palace we go!" */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestination(
			ACTOR_KRADEN, leader->x.part[0].integer, leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdActorSetSpeed(ACTOR_JENNA, 1.2);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestination(
			ACTOR_JENNA, leader->x.part[0].integer, leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_JENNA);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorSetSpeed(ACTOR_SHEBA, 1.2);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestination(
			ACTOR_SHEBA, leader->x.part[0].integer, leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_SHEBA);
	fdActorSetPos(ACTOR_SHEBA, 0, 0);
	fdScriptEnd();
}

#include "shared/scene_entrances_default.c"

/* The tower's ways between floors: the top of the stairs in the room below, and
   the ladder down through the floor of the room above. */
SCENE_REGION SceneRegions[] = {
	{416, 32, 592, 432, 48, 608, CONDITION_ALWAYS, WAY_MIDDLE_ROOM_STAIRS},
	{688, -24, 608, 704, -8, 624, CONDITION_ALWAYS, WAY_TOP_ROOM_LADDER},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(LemuriaRooms),
	SCENE_EXIT_UNLESS(LEMURIA_ROOMS_TO_LEMURIA_WEST_A, Lemuria,
		LEMURIA_FROM_LEMURIA_ROOMS_WEST_A, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LEMURIA_ROOMS_TO_LEMURIA_WEST_B, Lemuria,
		LEMURIA_FROM_LEMURIA_ROOMS_WEST_B, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LEMURIA_ROOMS_TO_LEMURIA_WEST_C, Lemuria,
		LEMURIA_FROM_LEMURIA_ROOMS_WEST_C, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(
		WAY_HALL_WEST_ARCH, LemuriaRooms, WAY_WEST_BACK_ROOM, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(
		WAY_HALL_EAST_ARCH, LemuriaRooms, WAY_EAST_BACK_ROOM, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(
		WAY_WEST_BACK_ROOM, LemuriaRooms, WAY_HALL_WEST_ARCH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(
		WAY_EAST_BACK_ROOM, LemuriaRooms, WAY_HALL_EAST_ARCH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_LUNPAS_STAIRS_WEST, LemuriaRooms,
		WAY_MIDDLE_ROOM_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_LUNPAS_STAIRS_EAST, LemuriaRooms,
		WAY_MIDDLE_ROOM_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_MIDDLE_ROOM_WEST, LemuriaRooms,
		WAY_LUNPAS_STAIRS_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_MIDDLE_ROOM_EAST, LemuriaRooms,
		WAY_LUNPAS_STAIRS_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LEMURIA_ROOMS_TO_LEMURIA_EAST_A, Lemuria,
		LEMURIA_FROM_LEMURIA_ROOMS_EAST_A, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_MIDDLE_ROOM_STAIRS, LemuriaRooms, WAY_TOP_ROOM_LADDER,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_TOP_ROOM_LADDER, LemuriaRooms, WAY_MIDDLE_ROOM_STAIRS,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LEMURIA_ROOMS_TO_LEMURIA_EAST_B, Lemuria,
		LEMURIA_FROM_LEMURIA_ROOMS_EAST_B, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LEMURIA_ROOMS_TO_LEMURIA_EAST_C, Lemuria,
		LEMURIA_FROM_LEMURIA_ROOMS_EAST_C, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_LEMURIAN_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 480, 0, 144, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_LEMURIAN_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 136, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_LEMURIAN_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 96, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WHITE_BIRD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 64, 0, 88, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LEMURIAN_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 328, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_LEMURIAN_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 588, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_LEMURIAN_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 552, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_LEMURIAN_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 328, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_LEMURIAN_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 80, 0, 552, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_LUNPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 400, 0, 416, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_LEMURIAN_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 96, 0, 792, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};
/* clang-format on */

/* The ways out, the steps that start Piers's uncle's letter and Lunpa's talk,
   the people talked to and read, and the bookcases. */
CUE SceneCues[] = {
	{CUE_EXIT, LEMURIA_ROOMS_TO_LEMURIA_WEST_A, CONDITION_ALWAYS,
		LEMURIA_ROOMS_TO_LEMURIA_WEST_A},
	{CUE_EXIT, LEMURIA_ROOMS_TO_LEMURIA_WEST_B, CONDITION_ALWAYS,
		LEMURIA_ROOMS_TO_LEMURIA_WEST_B},
	{CUE_EXIT, LEMURIA_ROOMS_TO_LEMURIA_WEST_C, CONDITION_ALWAYS,
		LEMURIA_ROOMS_TO_LEMURIA_WEST_C},
	{CUE_EXIT, WAY_HALL_WEST_ARCH, CONDITION_ALWAYS, WAY_HALL_WEST_ARCH},
	{CUE_EXIT, WAY_HALL_EAST_ARCH, CONDITION_ALWAYS, WAY_HALL_EAST_ARCH},
	{CUE_EXIT, WAY_WEST_BACK_ROOM, CONDITION_ALWAYS, WAY_WEST_BACK_ROOM},
	{CUE_EXIT, WAY_EAST_BACK_ROOM, CONDITION_ALWAYS, WAY_EAST_BACK_ROOM},
	{CUE_EXIT, WAY_LUNPAS_STAIRS_WEST, CONDITION_ALWAYS,
		WAY_LUNPAS_STAIRS_WEST},
	{CUE_EXIT, WAY_LUNPAS_STAIRS_EAST, CONDITION_ALWAYS,
		WAY_LUNPAS_STAIRS_EAST},
	{CUE_EXIT, WAY_MIDDLE_ROOM_WEST, CONDITION_ALWAYS, WAY_MIDDLE_ROOM_WEST},
	{CUE_EXIT, WAY_MIDDLE_ROOM_EAST, CONDITION_ALWAYS, WAY_MIDDLE_ROOM_EAST},
	{CUE_EXIT, LEMURIA_ROOMS_TO_LEMURIA_EAST_A, CONDITION_ALWAYS,
		LEMURIA_ROOMS_TO_LEMURIA_EAST_A},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_SOUTH, LEMURIA_ROOMS_TO_LEMURIA_EAST_B,
		CONDITION_ALWAYS, LEMURIA_ROOMS_TO_LEMURIA_EAST_B},
	{CUE_EXIT, LEMURIA_ROOMS_TO_LEMURIA_EAST_C, CONDITION_ALWAYS,
		LEMURIA_ROOMS_TO_LEMURIA_EAST_C},
	{CUE_STEP, METATILE_BEHAVIOR_UNCLES_DOORWAY, FLAG_PIERS_AT_MOTHERS_GRAVE,
		SCRIPT_REF(scLemuriaRoomsLeavePiersToMourn)},
	{CUE_STEP, METATILE_BEHAVIOR_FOOT_OF_LUNPAS_STAIRS, FLAG_MET_LUNPA,
		SCRIPT_REF(scLemuriaRoomsMeetLunpa)},
	{CUE_TALK, ACTOR_GIRL_WHO_NEVER_LEFT, CONDITION_ALWAYS,
		MSGNO(MsgLemuriaRoomsIHaveNeverBeenOutside)},
	{CUE_TALK, ACTOR_WOMAN_AWAITING_PIERS_TALES, CONDITION_ALWAYS,
		MSGNO(MsgLemuriaRoomsNowThatPiersHasReturned)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_WHO_NEVER_LEFT,
		CONDITION_ALWAYS, MSGNO(MsgLemuriaRoomsWhoKnowsWhatWondersLie)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_AWAITING_PIERS_TALES,
		CONDITION_ALWAYS, MSGNO(MsgLemuriaRoomsSenatePassedLawThatProhibits)},
	{CUE_TALK, ACTOR_PIERS_UNCLE, CONDITION_ALWAYS,
		SCRIPT_REF(scLemuriaRoomsTalkToPiersUncle)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIERS_UNCLE, CONDITION_ALWAYS,
		SCRIPT_REF(scLemuriaRoomsMindReadPiersUncle)},
	{CUE_TALK, ACTOR_OLD_MAN_ASKING_OF_THE_OUTSIDE, CONDITION_ALWAYS,
		SCRIPT_REF(scLemuriaRoomsAskFromBeyondSeaOfTime)},
	{CUE_TALK, ACTOR_OLD_MAN_ON_ALCHEMYS_ABUSE, CONDITION_ALWAYS,
		MSGNO(MsgLemuriaRoomsLongAgoPowerOfAlchemy)},
	{CUE_TALK, ACTOR_OLD_MAN_ON_THE_RUMORS, CONDITION_ALWAYS,
		MSGNO(MsgLemuriaRoomsAndNowRumorsAbound)},
	{CUE_TALK, ACTOR_OLD_MAN_AGAINST_ALCHEMY, CONDITION_ALWAYS,
		MSGNO(MsgLemuriaRoomsComeWhatMayICannot)},
	{CUE_TALK, ACTOR_OLD_MAN_DENYING_DECAY, CONDITION_ALWAYS,
		MSGNO(MsgLemuriaRoomsHasLemuriaFallenIntoDecay)},
	{CUE_TALK, ACTOR_OLD_MAN_ON_THE_DRAUGHTS, CONDITION_ALWAYS,
		SCRIPT_REF(scLemuriaRoomsAskAboutDraughts)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
		ACTOR_OLD_MAN_ASKING_OF_THE_OUTSIDE | CUE_SUBJECT_OWN_EFFECT,
		FLAG_MIND_READ_NOTICED, SCRIPT_REF(scLemuriaRoomsNoticeMindRead)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
		ACTOR_OLD_MAN_ON_ALCHEMYS_ABUSE | CUE_SUBJECT_OWN_EFFECT,
		FLAG_MIND_READ_NOTICED, SCRIPT_REF(scLemuriaRoomsNoticeMindRead)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
		ACTOR_OLD_MAN_ON_THE_RUMORS | CUE_SUBJECT_OWN_EFFECT,
		FLAG_MIND_READ_NOTICED, SCRIPT_REF(scLemuriaRoomsNoticeMindRead)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
		ACTOR_OLD_MAN_AGAINST_ALCHEMY | CUE_SUBJECT_OWN_EFFECT,
		FLAG_MIND_READ_NOTICED, SCRIPT_REF(scLemuriaRoomsNoticeMindRead)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
		ACTOR_OLD_MAN_DENYING_DECAY | CUE_SUBJECT_OWN_EFFECT,
		FLAG_MIND_READ_NOTICED, SCRIPT_REF(scLemuriaRoomsNoticeMindRead)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
		ACTOR_OLD_MAN_ON_THE_DRAUGHTS | CUE_SUBJECT_OWN_EFFECT,
		FLAG_MIND_READ_NOTICED, SCRIPT_REF(scLemuriaRoomsNoticeMindRead)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ASKING_OF_THE_OUTSIDE,
		CONDITION_ALWAYS, MSGNO(MsgLemuriaRoomsWhatDoesHydrosThinkWill)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_ALCHEMYS_ABUSE,
		CONDITION_ALWAYS, MSGNO(MsgLemuriaRoomsEverSinceAlchemyWasSealed)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_RUMORS,
		CONDITION_ALWAYS, MSGNO(MsgLemuriaRoomsIfAlchemyWereLoosedUpon)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_AGAINST_ALCHEMY,
		CONDITION_ALWAYS, MSGNO(MsgLemuriaRoomsAllOfMembersOfSenate)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_DENYING_DECAY,
		CONDITION_ALWAYS, MSGNO(MsgLemuriaRoomsCertainlyLemuriaHasLostSome)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_DRAUGHTS,
		CONDITION_ALWAYS, MSGNO(MsgLemuriaRoomsEvenThoughWeNoLonger)},
	{EXAMINE_CUE(EXAMINE_BOOKCASE), 200, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgLemuriaRoomsTheresBookCalledGreatAdventures))},
	{EXAMINE_CUE(EXAMINE_BOOKCASE), 201, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgLemuriaRoomsIFoundLetterFromBabi))},
	{EXAMINE_CUE(EXAMINE_BOOKCASE), 202, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgLemuriaRoomsHeresBookCalledHistory))},
	{EXAMINE_CUE(EXAMINE_BOOKCASE), 203, CONDITION_ALWAYS,
		FIND_MESSAGE(
			MSGNO(MsgLemuriaRoomsHeresBookCalledElementalLighthouses))},
	{EXAMINE_CUE(EXAMINE_BOOKCASE), 204, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgLemuriaRoomsHeresBookCalledGreatLemurian))},
	{EXAMINE_CUE(EXAMINE_BOOKCASE), 205, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgLemuriaRoomsHeresBookCalledAlchemy))},
	{EXAMINE_CUE(EXAMINE_BOOKCASE), 206, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgLemuriaRoomsHeresBookCalledAlchemyAndAncient))},
	{EXAMINE_CUE(EXAMINE_BOOKCASE), 207, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgLemuriaRoomsHeresBookCalledDiary))},
	{EXAMINE_CUE(EXAMINE_WARDROBE), 208, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgLemuriaRoomsWhatInWhatsWithAll))},
	{CUE_TABLE_END},
};

/* The script each "?" bubble runs: it is released once its ticks are up. */
Uint32 QuestionMarkScript[] = {
	SC_RELEASE,
};
