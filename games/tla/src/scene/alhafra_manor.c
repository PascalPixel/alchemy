/* @file */
/* The mayor of Alhafra's manor, where the party stays as his guests once */
/* Briggs is caught and hears in the night that Briggs has escaped */
/* (scAlhafraManorBriggsEscapes), with other people about once the boat's mast
 */
/* is cleared (FLAG_ALHAFRA_MAST_CLEARED). */

/* This unit declares its own form of fdActorTakeLeaderLayer, below. */
#define OWN_Actor_TakeLeaderLayer

#include "text/msg_ids.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "field/field_sequence_event.h"
#include "field/event_inline.h"
#include "graphics/palette_buffers.h"

/* Talks each person gives once, the first time spoken to: Madra's mayor */
/* complains of Alhafra's, Jiya defends him, and Alhafra's mayor tells of the */
/* Eastern Sea. */
#define FLAG_COMPLAINED_ABOUT_THE_MAYOR 0x8ba
#define FLAG_DEFENDED_THE_MAYOR			0x8bb
#define FLAG_TOLD_OF_THE_EASTERN_SEA	0x8bc

/* The manor's entrances its set-up tests or sets. */
typedef enum ALHAFRA_MANOR_ENTRANCE {
	/* The mayor's room, by its south door, where the party is left after */
	/* Briggs escapes. */
	ENTRANCE_MAYORS_ROOM = 8,
	/* The watchtower, where the man gazing at Air's Rock stands. */
	ENTRANCE_WATCHTOWER = 14,
	/* Where Eastern Alhafra sends the party to see Briggs escape from the */
	/* manor (scAlhafraManorBriggsEscapes). */
	ENTRANCE_BRIGGS_ESCAPES = 81,
	/* Unused: no exit leads here. The camera pans west across the room and */
	/* waits for a key, then the party enters again by */
	/* ALHAFRA_MANOR_FROM_WORLD_MAP_VIEW. */
	ENTRANCE_PAN_WEST = 99
} ALHAFRA_MANOR_ENTRANCE;

/* The household and guests of ActorPlacements, while the mast lies fallen. */
typedef enum ALHAFRA_MANOR_ACTOR_ID {
	/* Jiya, the mayor's aide: "Milord, these things take time..." */
	ACTOR_JIYA = ACTOR_FIRST_PLACED,
	/* Alhafra's mayor, who plans a sailing ship to trade across the sea. */
	ACTOR_MAYOR,
	/* "You should go to the watchtower and gaze southward." */
	ACTOR_BOY_ON_THE_WATCHTOWER = ACTOR_FIRST_PLACED + 2,
	/* Who asks what the party thinks of Alhafra; her thought: "Why do they */
	/* always tell me to send adventure-seekers to South Osenia?" */
	ACTOR_WOMAN_ON_SOUTH_OSENIA,
	/* "Our town was rife with pirates until just recently." */
	ACTOR_WOMAN_ON_THE_PIRATES,
	/* "The mountains to the east of Alhafra are the Balloo Range." */
	ACTOR_OLD_WOMAN_ON_THE_BALLOO_RANGE,
	/* "Lookee here! Can't you see Air's Rock..." */
	/* (scAlhafraManorPointOutAirsRock) */
	ACTOR_MAN_GAZING_AT_AIRS_ROCK = ACTOR_FIRST_PLACED + 6,
	/* "I'd been saying Indra wasn't in the right place for some time now." */
	ACTOR_OLD_MAN_ON_INDRA,
	/* Who asks whether the party are the heroes; her thought: "I wish someone
	 */
	/* would go do something about that cursed village south of us." */
	ACTOR_WOMAN_ON_THE_CURSED_VILLAGE,
	/* "I heard the pirates talking about the warriors who beat them." */
	ACTOR_GIRL_ON_THE_WARRIORS,
	ACTOR_MADRAS_MAYOR,
	ACTOR_MADRAS_ELDER
} ALHAFRA_MANOR_ACTOR_ID;

/* Who is in the manor the night Briggs escapes, once the mast is cleared */
/* (AlhafraManor_ActorPlacementsOnceTheMastIsCleared, Jiya and the mayor */
/* first as before). */
typedef enum ALHAFRA_MANOR_ONCE_THE_MAST_IS_CLEARED_ACTOR_ID {
	/* "Briggs destroyed the prison and escaped with his pirates!" */
	ACTOR_JAIL_GUARD = ACTOR_FIRST_PLACED + 3,
	/* "Mayor! The mast has been raised!" */
	ACTOR_WORKER,
	ACTOR_KRADEN
} ALHAFRA_MANOR_ONCE_THE_MAST_IS_CLEARED_ACTOR_ID;

void scAlhafraManorBriggsEscapes(void);
void scAlhafraManorKeepToTheLeadersLayer(void);

void scAlhafraManorComplainAboutTheMayor(void);

void scAlhafraManorDefendTheMayor(void);
void scAlhafraManorTellOfTheEasternSea(void);

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

extern ACTOR_PLACE ActorPlacements[];
extern ACTOR_PLACE AlhafraManor_ActorPlacementsOnceTheMastIsCleared[];

const ACTOR_PLACE *scGetActorPlacements(void) {
	if(syGameFlagGet(FLAG_ALHAFRA_MAST_CLEARED))
		return AlhafraManor_ActorPlacementsOnceTheMastIsCleared;
	return ActorPlacements;
}

/* The woman who asks what the party thinks of Alhafra, and answers yes or no.
 */
void scAlhafraManorAskAboutAlhafra(s32 actor) {
	s32 msg = (s32)&MsgAlhafraManorWhatDoYouThinkOf;

	fdEventSetMessageId(msg);
	/* "What do you think of Alhafra? Have you taken a fancy to it?" */
	fdMessageShowAtActor(actor, 0);
	if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdEventWait(10);
		/* Yes: "Then you are welcome to stay here and relax for as long as you
		 */
		/* please..." */
		fdEventSetMessageId(msg + 1);
	}
	else {
		fdEventWait(20);
		/* No: "Then why don't you follow your feet to the south?..." */
		fdEventSetMessageId(msg + 2);
	}
	fdEventShowMessageAndWait(actor, 0);
}

/* The man gazing south asks whether the party can see Air's Rock; on a yes */
/* the leader steps up beside him and the world map shows it. */
void scAlhafraManorPointOutAirsRock(s32 actor) {
	s32 msg = (s32)&MsgAlhafraManorLookeeHereCantYouSee;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId(msg);
	/* "Lookee here! Can't you see Air's Rock out there?" */
	fdMessageShowAtActor(actor, 0);
	if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdEventWait(20);
		fdActorWatchAndSetScript(actor, MainRoutineStateWork.leaderActorId);
		fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 760, 320);
		fdActorFaceDirection(ACTOR_MAN_GAZING_AT_AIRS_ROCK,
							 FACING_SOUTH_SOUTHWEST, 0);
		fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,
								 FACING_SOUTH, 0);
		fdEventWait(10);
		fdEventSetDestination(SCENE_WORLD_MAP, WORLD_MAP_ALHAFRA_MANOR_VIEW);
	}
	else {
		fdEventSetMessageId(msg + 2);
		/* No: "Oh... I guess today is a bit cloudier than usual..." */
		fdEventShowMessageAndWait(actor, 0);
	}
	fdEventEnd();
}

/* The woman who asks whether the party are the heroes who beat the pirates, */
/* and answers yes or no. */
void scAlhafraManorAskIfTheHeroes(s32 actor) {
	s32 msg = (s32)&MsgAlhafraManorAreYouTheBraveHero;

	fdEventSetMessageId(msg);
	/* "Are you the brave hero who defeated the pirates?" */
	fdMessageShowAtActor(actor, 0);
	if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdEventWait(10);
		/* Yes: "Well, I wonder if even a hero like you can stand up against a
		 */
		/* creature..." */
		fdEventSetMessageId(msg + 1);
	}
	else {
		fdEventWait(20);
		/* No: "I know you're the one who beat the pirates... Don't try to act
		 */
		/* humble..." */
		fdEventSetMessageId(msg + 2);
	}
	fdEventShowMessageAndWait(actor, 0);
}

/* Talking to the mayor of Madra, or reading his mind, once: he complains that
 */
/* Alhafra's mayor never meant to fix the mast. */
void scAlhafraManorComplainAboutTheMayor(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	syGameFlagSet(FLAG_COMPLAINED_ABOUT_THE_MAYOR);
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)&MsgAlhafraManorDarnThatMayor);
	if(state->psynergy != 0)
		fdActorShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
						 60);
	fdActorFaceActorById(ACTOR_MADRAS_MAYOR, MainRoutineStateWork.leaderActorId,
						 0);
	fdActorShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
	/* "Darn that mayor..." */
	fdEventShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
	fdEventWait(10);
	/* "I thought he'd get that mast fixed up for us right away..." */
	fdEventShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_MAYOR, 4);
	/* "I'm starting to think the mayor of Alhafra never planned on..." */
	fdEventShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdEventEnd();
}

/* Talking to Jiya, or reading his mind, once: he explains why the mayor keeps
 */
/* the pirates' money for Alhafra's own repairs. */
void scAlhafraManorDefendTheMayor(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	syGameFlagSet(FLAG_DEFENDED_THE_MAYOR);
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)&MsgAlhafraManorImSureTheMayorHas);
	if(state->psynergy != 0)
		fdActorShowEmote(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorWatchAndSetScript(ACTOR_JIYA, MainRoutineStateWork.leaderActorId);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 304, 152);
	fdActorFaceDirectionById(ACTOR_JIYA, FACING_EAST, 0);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_WEST,
							 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_JIYA, 2);
	fdEventWait(10);
	/* "I'm sure the mayor has his reasons for halting repairs on the..." */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_JIYA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_JIYA, 3);
	fdEventWait(10);
	/* "The mayor feels that Madra should only get back what was stolen from..."
	 */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_JIYA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_JIYA, 2);
	fdEventWait(10);
	/* "But now, it seems the Madrans are getting the better end of the deal..."
	 */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_JIYA, 0, 10);
	fdActorFaceDirectionById(ACTOR_JIYA, FACING_NORTHEAST, 0);
	fdEventWait(50);
	fdActorFaceDirectionById(ACTOR_JIYA, FACING_EAST, 0);
	fdEventWait(10);
	/* "The mayor thinks it unfair that Madra should benefit while Alhafra..."
	 */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_JIYA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_JIYA, 2);
	fdEventWait(10);
	/* "That's why he's keeping the pirates' money and using it to repair..." */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_JIYA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_JIYA, 4);
	/* "If Madra wants their boat fixed, then they can just pay us to have..."
	 */
	fdEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_JIYA, 0, 10);
	fdEventEnd();
}

/* Talking to Alhafra's mayor, or reading his mind, once: he tells of the towns
 */
/* Briggs speaks of across the eastern sea. */
void scAlhafraManorTellOfTheEasternSea(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	if(state->psynergy != 0) {
		fdActorFaceActorById(ACTOR_MAYOR, MainRoutineStateWork.leaderActorId,
							 0);
		fdEventWait(10);
	}
	syGameFlagSet(FLAG_TOLD_OF_THE_EASTERN_SEA);
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)&MsgAlhafraManorBriggsTellsMeThereAre);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_MAYOR, 2);
	fdEventWait(10);
	/* "Briggs tells me there are huge towns on a continent that lies across..."
	 */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MAYOR, 3);
	fdEventWait(10);
	/* "He tells me that, by trading with one another, all of these towns..." */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 10);
	fdActorFaceDirectionById(ACTOR_MAYOR, FACING_NORTH_NORTHEAST, 0);
	fdEventWait(30);
	/* "I've made a decision... If Alhafra is to prosper, then we must..." */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 10);
	fdActorFaceActorById(ACTOR_MAYOR, MainRoutineStateWork.leaderActorId, 0);
	fdEventWait(20);
	/* "I'm going to build an entire trade fleet and make lots of money!" */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 10);
	fdActorShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Still...there must be other pirates and freebooters still roaming..." */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 20);
	fdActorShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	/* "Felix, Briggs told me you and your friends fought with great strength."
	 */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MAYOR, 4);
	/* "If you could lend me that strength, we would need not fear any pirates."
	 */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MAYOR, 3);
	fdEventWait(10);
	/* "So if you'd just be willing to help us out, our town could begin..." */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_MAYOR, 2);
	fdEventWait(10);
	/* "What do you say? Do we have a deal?" Yes: "Great! Why don't you just
	 * stay */
	/* here in Alhafra..." No: "Ah, such a pity that you don't see the */
	/* potential..." */
	fdMessageShowAtActor(ACTOR_MAYOR, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdEventWait(20);
		/* "Great! Why don't you just stay here in Alhafra until we're ready
		 * for..." */
		fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 10);
		FieldStateWork->msg++;
	}
	else {
		fdEventWait(30);
		FieldStateWork->msg++;
		/* "Ah, such a pity that you don't see the potential of this..." */
		fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 10);
	}
	fdEventEnd();
	if(state->psynergy != 0)
		fdActorFaceDirection(ACTOR_MAYOR, FACING_NORTH_NORTHEAST, 0);
}

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

s32 scInit(void) {
	FIXED cameraY = -1;
	FIXED cameraZ = toFIXED(320);
	s16 *entrance;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdActorGetById(ACTOR_OLD_MAN_ON_INDRA)->update =
		scAlhafraManorKeepToTheLeadersLayer;
	if(MainRoutineStateWork.entrance == ALHAFRA_MANOR_FROM_WORLD_MAP_VIEW) {
		MainRoutineStateWork.entrance = MainRoutineStateWork.returnEntrance =
			ENTRANCE_WATCHTOWER;
		fdActorFaceDirectionById(ACTOR_MAN_GAZING_AT_AIRS_ROCK,
								 FACING_SOUTH_SOUTHWEST, 0);
		fdEventStartScrTransition();
		fdEventWaitTransitionFrames();
		fdEventWait(10);
		fdEventSetMessageId((s32)&MsgAlhafraManorFromHereItJustLooks);
		/* "From here, it just looks like a regular old rock, but from up
		 * close..." */
		fdEventShowMessageAndWait(ACTOR_MAN_GAZING_AT_AIRS_ROCK, 0);
		syGameFlagClear(FLAG_SHOW_AREA_NAME);
	}
	entrance = &MainRoutineStateWork.entrance;
	if(*entrance == ENTRANCE_BRIGGS_ESCAPES) {
		MainRoutineStateWork.entrance = MainRoutineStateWork.returnEntrance =
			ENTRANCE_MAYORS_ROOM;
		scAlhafraManorBriggsEscapes();
	}
	if(!syGameFlagGet(FLAG_ALHAFRA_MAST_CLEARED) &&
	   syGameFlagGet(FLAG_GABOMBA_MESSAGE_CARRIED)) {
		fdActorSetPos(ACTOR_MADRAS_MAYOR, 0, 0);
		fdActorSetPos(ACTOR_MADRAS_ELDER, 0, 0);
	}
	if(MainRoutineStateWork.entrance == ENTRANCE_PAN_WEST) {
		fdCameraMoveWithinMapBounds(toFIXED(304), cameraY, cameraZ, 0);
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
		fdEventStartScrTransition();
		fdEventWaitTransitionFrames();
		fdEventWait(10);
		fdCameraSetSpeed(2);
		fdCameraMoveWithinMapBounds(toFIXED(144), cameraY, cameraZ, 1);
		while(InputWork.held == 0)
			sySynch(1);
		fdEventSetDestination(SCENE_ALHAFRA_MANOR,
							  ALHAFRA_MANOR_FROM_WORLD_MAP_VIEW);
	}
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* Counts one more weird answer Felix gives (fdEventCountAnswer, through the */
/* overlay's own import row). */
void scAlhafraManorCountAnswer(void);

/* Walks actor `id` to the controlled character's cell, when it has one. */
#define WALK_TO_LEADER(id)                                                     \
	{                                                                          \
		ACTOR *leader_;                                                        \
		fdActorSetSpeed((id), 1.2);                                            \
		fdActorSetAnimationById((id), 2);                                      \
		leader_ = fdActorGetById(MainRoutineStateWork.leaderActorId);          \
		if(leader_ != 0)                                                       \
			fdActorSetDestinationById((id), leader_->x.part[0].integer,        \
									  leader_->z.part[0].integer);             \
		fdActorWaitForMoveById((id));                                          \
		fdActorSetPos((id), 0, 0);                                             \
	}

/* The night at the mayor's manor: the party thanks the mayor for his */
/* house, Jenna and Sheba doubt he will hand the boat over to Madra, the mayor
 */
/* asks Jiya after the repairs, and a guard runs in with news: Briggs has */
/* escaped. Felix saying yes to going after Briggs for the mayor counts among */
/* his weird answers. */
void scAlhafraManorBriggsEscapes(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgAlhafraManorThankYouAgainMayorYou);
	fdActorSetPos(ACTOR_MAYOR, 344, 96);
	fdActorSetPos(ACTOR_JIYA, 408, 136);
	fdActorSetPos(ACTOR_KRADEN, 312, 104);
	fdActorSetPos(ACTOR_PIERS, 296, 120);
	fdActorSetPos(ACTOR_SHEBA, 296, 136);
	fdActorSetPos(ACTOR_JENNA, 312, 136);
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
	fdEventWait(30);
	fdCameraMoveTo(344, CAMERA_KEEP, 120, 0);
	FieldStateWork->transitionFrm = 60;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, 3);
	fdEventWait(20);
	/* "Thank you again, Mayor. You keep a fine house." */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MAYOR, 4);
	/* "Don't be silly. You've done us a great deed." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAYOR, 0,
									10);
	fdActorShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "It was the least I could do." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAYOR, 0,
									10);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdEventWait(20);
	/* "Even if he gets the boat fixed, I don't think he's going to hand it..."
	 */
	fdEventShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdEventWait(20);
	/* "I agree... He's being way too nice for me not to be a little..." */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 10);
	fdActorShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	fdActorFaceDirection(ACTOR_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(20);
	/* "I'm sorry, did you girls have something you'd like to share with us?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAYOR, 0,
									10);
	fdActorStartRepeatedMotion(ACTOR_SHEBA, 3);
	fdActorSetAttachedEffect(ACTOR_SHEBA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 3);
	fdActorSetAttachedEffect(ACTOR_JENNA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdEventWait(30);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_MAYOR, 3);
	fdEventWait(20);
	/* "I see!" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAYOR, 0,
									10);
	fdActorFaceDirection(ACTOR_MAYOR, FACING_WEST, 0);
	fdEventWait(40);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdPaletteFadeApplyTarget(PALETTE_TINT_SEPIA, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(40);
	fdEventWaitTransitionFrames();
	fdActorSetAnimationAndWait(ACTOR_MAYOR, 3);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(50);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
	fdActorStartRepeatedMotion(ACTOR_MAYOR, 3);
	fdActorSetAttachedEffect(ACTOR_MAYOR,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	fdEventWait(35);
	fdActorFaceDirection(ACTOR_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(35);
	fdActorShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
	fdActorStartRepeatedMotion(ACTOR_SHEBA, 3);
	fdActorSetAttachedEffect(ACTOR_SHEBA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 3);
	fdActorSetAttachedEffect(ACTOR_JENNA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(60);
	fdActorSetAnimationAndWait(ACTOR_MAYOR, 3);
	fdEventWait(30);
	fdActorShowEmoteById(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_HEART, 50);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
	fdActorJump(ACTOR_FELIX, 6, 15);
	fdActorJump(ACTOR_FELIX, 6, 23);
	fdEventWait(15);
	fdActorShowEmoteById(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 70);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdPaletteFadeSetSource(PALETTE_TINT_SEPIA, PALETTES_ALL);
	fdPaletteFadeSetTarget(RGB(0, 0, 0), PALETTES_ALL);
	fdPaletteFadeStartInterpolation(60);
	fdEventWaitTransitionFrames();
	fdEventWait(60);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(20);
	scPlayMusic();
	fdActorSetSpeed(ACTOR_MAYOR, 0.8);
	fdActorSetSpeed(ACTOR_JIYA, 0.8);
	fdActorSetSpeed(ACTOR_JAIL_GUARD, 1.6);
	fdActorSetSpeed(ACTOR_WORKER, 1.6);
	fdActorWalkByAndWaitById(ACTOR_MAYOR, 16, 0);
	fdActorFaceDirection(ACTOR_MAYOR, FACING_SOUTHEAST, 0);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "Jiya, how are the repairs coming along? Things seem to be moving..." */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MAYOR, 0, 10);
	fdActorShowEmote(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_WINCE, 45);
	fdActorWalkByAndWait(ACTOR_JIYA, -16, 0);
	fdActorFaceDirection(ACTOR_JIYA, FACING_NORTHWEST, 0);
	fdEventWait(10);
	/* "Milord, these things take time..." */
	fdEventShowMessageThenWait(ACTOR_JIYA, 0, 20);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdActorSetPos(ACTOR_JAIL_GUARD, 344, 176);
	fdActorWalkByAndWait(ACTOR_JAIL_GUARD, 0, -16);
	/* "Sir, there's a problem!" */
	fdEventShowMessageThenWait(ACTOR_JAIL_GUARD, 0, 10);
	fdActorShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_JIYA, FACING_WEST, 0);
	fdEventWait(20);
	/* "Calm down, man! You're a soldier! What are you so flustered about!?" */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 10);
	fdActorSetSpeed(ACTOR_JAIL_GUARD, 1.8);
	fdActorWalkByAndWait(ACTOR_JAIL_GUARD, 16, -24);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JAIL_GUARD, FACING_NORTH_NORTHEAST, 0);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_JAIL_GUARD, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "Briggs destroyed the prison and escaped with his pirates!" */
	fdEventShowMessageThenWait(ACTOR_JAIL_GUARD, 0, 10);
	sySndPlay(MUS_BATTLE_JENNA);
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmoteById(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmoteById(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmoteById(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	fdActorShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "What, were you asleep while this was going on?" */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_JAIL_GUARD, 4);
	/* "Don't... Don't be silly!" */
	fdEventShowMessageThenWait(ACTOR_JAIL_GUARD, 0, 10);
	fdActorShowEmote(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "How could Briggs possibly break out of our prison? It's one of the..."
	 */
	fdEventShowMessageThenWait(ACTOR_JIYA, 0, 10);
	fdActorShowEmoteById(ACTOR_JAIL_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorFaceDirectionById(ACTOR_JAIL_GUARD, FACING_EAST, 0);
	fdEventWait(10);
	/* "I don't know what happened, but before I knew it, they'd smashed a..."
	 */
	fdEventShowMessageThenWait(ACTOR_JAIL_GUARD, 0, 20);
	fdActorSetPos(ACTOR_WORKER, 344, 176);
	fdActorWalkByAndWait(ACTOR_WORKER, 0, -16);
	fdEventWait(10);
	fdActorShowEmoteById(ACTOR_WORKER, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Mayor! The mast has been raised!" */
	fdEventShowMessageThenWait(ACTOR_WORKER, 0, 10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmoteById(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorFaceDirection(ACTOR_WORKER, FACING_NORTH_NORTHWEST, 0);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_WORKER, FACING_NORTH_NORTHEAST, 0);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_WORKER, FACING_NORTH_NORTHWEST, 0);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_WORKER, FACING_NORTH_NORTHEAST, 0);
	fdEventWait(30);
	fdActorShowEmoteById(ACTOR_WORKER, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Hey, what's up?" */
	fdEventShowMessageThenWait(ACTOR_WORKER, 0, 10);
	fdActorFaceDirection(ACTOR_JAIL_GUARD, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(30);
	fdActorShowEmoteById(ACTOR_WORKER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	fdActorWalkByAndWait(ACTOR_WORKER, 0, -8);
	fdEventWait(10);
	/* "Wait, you're supposed to be guarding the prisoners... What happened?" */
	fdEventShowMessageThenWait(ACTOR_WORKER, 0, 10);
	fdActorShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "It appears that Briggs has escaped." */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 10);
	fdActorShowEmoteById(ACTOR_WORKER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "That's no good..." */
	fdEventShowMessageThenWait(ACTOR_WORKER, 0, 10);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_JIYA, 2);
	fdEventWait(10);
	/* "What are you standing around for!? Search the area! Find Briggs at
	 * once!" */
	fdEventShowMessageThenWait(ACTOR_WORKER, 0, 10);
	fdActorFaceDirectionById(ACTOR_JAIL_GUARD, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_WORKER, FACING_EAST, 0);
	fdEventWait(30);
	fdActorRunRepeatedMotion(ACTOR_JAIL_GUARD, 2);
	fdEventWait(10);
	/* "It...might not be quite that easy..." */
	fdEventShowMessageThenWait(ACTOR_JAIL_GUARD, 0, 10);
	fdActorFaceDirection(ACTOR_JAIL_GUARD, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_WORKER, FACING_NORTH_NORTHEAST, 0);
	fdEventWait(35);
	fdActorRunRepeatedMotion(ACTOR_WORKER, 2);
	fdEventWait(10);
	/* "He is a pirate, after all... A famous one!" */
	fdEventShowMessageThenWait(ACTOR_WORKER, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_JAIL_GUARD, 3);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_WORKER, 3);
	fdEventWait(30);
	fdActorShowEmoteById(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "Enough of that! Move it!" */
	fdEventShowMessageThenWait(ACTOR_JIYA, 0, 10);
	fdActorJump(ACTOR_WORKER, 6, 0);
	fdActorJump(ACTOR_JAIL_GUARD, 6, 25);
	fdActorWalkByById(ACTOR_WORKER, 0, 24);
	fdActorWalkByAndWait(ACTOR_JAIL_GUARD, -16, 16);
	fdActorWalkByById(ACTOR_JAIL_GUARD, 0, 24);
	fdActorWaitForMoveById(ACTOR_WORKER);
	fdActorSetPos(ACTOR_WORKER, 0, 0);
	fdActorWaitForMoveById(ACTOR_JAIL_GUARD);
	fdActorSetPos(ACTOR_JAIL_GUARD, 0, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_MAYOR, 4);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	fdEventWait(20);
	/* "Regardless, I'm going to go inspect my boat..." */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 10);
	fdActorSetSpeed(ACTOR_MAYOR, 1);
	fdActorSetSpeed(ACTOR_JIYA, 1);
	fdActorWatchAndSetScript(ACTOR_FELIX, ACTOR_MAYOR);
	fdActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_MAYOR);
	fdActorWatchAndSetScript(ACTOR_PIERS, ACTOR_MAYOR);
	fdActorWatchAndSetScript(ACTOR_JENNA, ACTOR_MAYOR);
	fdActorWatchAndSetScript(ACTOR_SHEBA, ACTOR_MAYOR);
	fdActorWalkByAndWaitById(ACTOR_MAYOR, 16, 0);
	fdActorWalkByAndWaitById(ACTOR_MAYOR, 0, 50);
	fdActorWalkByAndWait(ACTOR_MAYOR, -32, 4);
	fdEventWait(20);
	fdActorStop(ACTOR_FELIX);
	fdActorStop(ACTOR_KRADEN);
	fdActorStop(ACTOR_PIERS);
	fdActorStop(ACTOR_JENNA);
	fdActorStop(ACTOR_SHEBA);
	fdActorShowEmoteById(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	/* "Oh, and Felix and his crew were there, too, weren't they?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAYOR, 0,
									10);
	fdActorFaceDirectionById(ACTOR_MAYOR, FACING_NORTH_NORTHWEST, 0);
	fdEventWait(20);
	/* "I treated you like an honored guest in my home..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAYOR, 0,
									10);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(50);
	fdActorShowEmoteById(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Usually, one feels a certain obligation to those who do them such a..."
	 */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAYOR, 0,
									10);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdEventWait(35);
	fdActorRunRepeatedMotion(ACTOR_MAYOR, 2);
	fdEventWait(10);
	/* "So, don't just stand there! Return the favor and find me Briggs!" */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAYOR, 0, 10);
	fdActorWalkByAndWaitById(ACTOR_JIYA, -32, 0);
	fdEventWait(10);
	fdActorJump(ACTOR_JIYA, 6, 15);
	fdActorJump(ACTOR_JIYA, 6, 23);
	/* "Come on! Hurry up! Let's go! Move it!" */
	fdEventShowMessageThenWait(ACTOR_JIYA, 0, 10);
	fdActorFaceDirectionById(ACTOR_MAYOR, FACING_NORTH_NORTHEAST, 0);
	fdEventWait(20);
	/* "Let's go see the boat." */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 10);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdActorFaceDirectionById(ACTOR_JIYA, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_JIYA, 3);
	fdEventWait(20);
	fdActorWalkByById(ACTOR_MAYOR, 0, 24);
	fdActorWalkByAndWait(ACTOR_JIYA, -16, 16);
	fdActorWalkByById(ACTOR_JIYA, 0, 24);
	fdActorWaitForMoveById(ACTOR_MAYOR);
	fdActorSetPos(ACTOR_MAYOR, 0, 0);
	fdActorWaitForMoveById(ACTOR_JIYA);
	fdActorSetPos(ACTOR_JIYA, 0, 0);
	fdEventWait(30);
	scPlayMusic();
	fdActorShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "The mayor of Alhafra certainly does know how to look out for his own..."
	 */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "That's one way of putting it!" */
	fdEventShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_NORTH, 0);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(20);
	/* "Well, so what do we do? Do we go after Briggs?" Yes: "Don't feel like
	 * you */
	/* have to do them any favors!" No: "That's right! Why should we help the */
	/* mayor..." */
	fdMessageShowAtActor(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		scAlhafraManorCountAnswer();
		fdEventWait(30);
		fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
		/* "Don't feel like you have to do them any favors!" */
		fdEventShowMessageThenWait(ACTOR_SHEBA, 0, 10);
		(FieldStateWork->msg)++;
	}
	else {
		fdEventWait(40);
		fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
		fdEventWait(10);
		(FieldStateWork->msg)++;
		/* "That's right! Why should we help the mayor, after the way he
		 * treated..." */
		fdEventShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	}
	fdActorSetAnimationAndWait(ACTOR_KRADEN, 3);
	fdEventWait(10);
	/* "Felix can decide for himself what to do..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									10);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
	fdEventWait(30);
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "At the very least, I want to go see the sailing ship now that it's..."
	 */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, 3);
	fdEventWait(10);
	/* "Let's just go outside and take a quick look." */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
	fdActorSetAnimationById(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdEventWait(25);
	WALK_TO_LEADER(ACTOR_PIERS);
	WALK_TO_LEADER(ACTOR_JENNA);
	WALK_TO_LEADER(ACTOR_SHEBA);
	WALK_TO_LEADER(ACTOR_KRADEN);
	fdCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdEventEnd();
	FieldStateWork->transitionFrm = 16;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	syGameFlagClear(FLAG_SHOW_AREA_NAME);
}
/* The ROM's caller here is compiled against this form, which takes nothing and
 */
/* returns nothing where the definition takes the actor and returns an s32. */
void fdActorTakeLeaderLayer(void);

/* The old man's update: he takes the party leader's display layer. */
void scAlhafraManorKeepToTheLeadersLayer(void) { fdActorTakeLeaderLayer(); }

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{ALHAFRA_MANOR_FROM_WORLD_MAP_VIEW, CONDITION_ALWAYS, 760, 0, 320,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{ENTRANCE_BRIGGS_ESCAPES, CONDITION_ALWAYS, 312, 0, 120, FACING_NORTHEAST,
	 0, 224, 16, 464, 192, 0},
	{ENTRANCE_PAN_WEST, CONDITION_ALWAYS, 520, 0, 520, FACING_NORTH, 0, -1, -1,
	 -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 SceneExits[] = {
	SCENE_EXITS(AlhafraManor),
	SCENE_EXIT_UNLESS(ALHAFRA_MANOR_TO_ALHAFRA, Alhafra,
					  ALHAFRA_FROM_ALHAFRA_MANOR, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(2, AlhafraManor, 7, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(3, AlhafraManor, ENTRANCE_MAYORS_ROOM, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(4, AlhafraManor, 9, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(5, AlhafraManor, 10, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(6, AlhafraManor, 15, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(7, AlhafraManor, 2, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(8, AlhafraManor, 3, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(9, AlhafraManor, 4, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(10, AlhafraManor, 5, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(11, AlhafraManor, 12, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(12, AlhafraManor, 11, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(13, AlhafraManor, ENTRANCE_WATCHTOWER, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(14, AlhafraManor, 13, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(15, AlhafraManor, 6, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(16, AlhafraManor, 17, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(17, AlhafraManor, 16, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 280, 0, 152, FACING_EAST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_ALHAFRAS_MAYOR, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 372, 0, 92, FACING_NORTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 296, 0, 344, FACING_NORTHEAST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 152, 0, 328, FACING_SOUTHEAST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 568, 0, 104, FACING_NORTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 616, 0, 120, FACING_EAST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 808, 0, 296, FACING_NORTHWEST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 728, 0, 248, FACING_NORTHWEST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 152, 0, 680, FACING_EAST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS,
					MOVEMENT_TYPE_WANDER, 104, 0, 504, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MADRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					88, 0, 120, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MADRAS_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					128, 0, 112, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE AlhafraManor_ActorPlacementsOnceTheMastIsCleared[] = {
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_WEST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_ALHAFRAS_MAYOR, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_WEST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 344, 0, 320, FACING_EAST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS,
					MOVEMENT_TYPE_WANDER, -56, 0, -56, FACING_NORTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56,
					0, -56, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0,
					-56, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0,
					-56, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0,
					-56, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

TRIGGER SceneEvents[] = {
	{TRIGGER_EXIT, ALHAFRA_MANOR_TO_ALHAFRA, CONDITION_ALWAYS,
	 ALHAFRA_MANOR_TO_ALHAFRA},
	{TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
	{TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
	{TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
	{TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
	{TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
	{TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
	{TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
	{TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
	{TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
	{TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
	{TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
	{TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
	{TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
	{TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
	{TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
	{TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
	{TRIGGER_TALK, ACTOR_BOY_ON_THE_WATCHTOWER,
	 IF_FLAG_SET(FLAG_ALHAFRA_MAST_CLEARED),
	 MSGNO(MsgAlhafraManorEveryoneInTownIsTalking)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_WATCHTOWER,
	 IF_FLAG_SET(FLAG_ALHAFRA_MAST_CLEARED),
	 MSGNO(MsgAlhafraManorIDontKnowWhatThe)},
	{TRIGGER_TALK, ACTOR_BOY_ON_THE_WATCHTOWER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraManorYouShouldGoToThe)},
	{TRIGGER_TALK, ACTOR_WOMAN_ON_SOUTH_OSENIA, CONDITION_ALWAYS,
	 EVENT(scAlhafraManorAskAboutAlhafra)},
	{TRIGGER_TALK, ACTOR_WOMAN_ON_THE_PIRATES, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraManorOurTownWasRifeWith)},
	{TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_BALLOO_RANGE, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraManorTheMountainsToTheEast)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_WATCHTOWER,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraManorThereAreNumerousTalesAbout)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_SOUTH_OSENIA,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraManorWhyDoTheyAlwaysTell)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_PIRATES,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraManorThosePiratesNeverDidUs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_BALLOO_RANGE,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraManorTheresNoWayToCross)},
	{TRIGGER_TALK, ACTOR_MAN_GAZING_AT_AIRS_ROCK, CONDITION_ALWAYS,
	 EVENT(scAlhafraManorPointOutAirsRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_GAZING_AT_AIRS_ROCK,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraManorTheLongerISitHere)},
	{TRIGGER_TALK, ACTOR_OLD_MAN_ON_INDRA, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraManorIdBeenSayingIndraWasnt)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_INDRA,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraManorISatAndGazedAt)},
	{TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CURSED_VILLAGE, CONDITION_ALWAYS,
	 EVENT(scAlhafraManorAskIfTheHeroes)},
	{TRIGGER_TALK, ACTOR_GIRL_ON_THE_WARRIORS, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraManorIHeardThePiratesTalking)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CURSED_VILLAGE,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraManorIWishSomeoneWouldGo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_THE_WARRIORS,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraManorIHopeTheWarriorsWho)},
	{TRIGGER_TALK, ACTOR_MADRAS_MAYOR, FLAG_COMPLAINED_ABOUT_THE_MAYOR,
	 EVENT(scAlhafraManorComplainAboutTheMayor)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_MADRAS_MAYOR | TRIGGER_SUBJECT_OWN_EFFECT,
	 FLAG_COMPLAINED_ABOUT_THE_MAYOR,
	 EVENT(scAlhafraManorComplainAboutTheMayor)},
	{TRIGGER_TALK, ACTOR_MADRAS_MAYOR, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraManorImStartingToThinkThe)},
	{TRIGGER_TALK, ACTOR_MADRAS_ELDER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraManorHesAlreadyBeenPaidTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MADRAS_MAYOR, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraManorIfWeLeaveTheBoat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MADRAS_ELDER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraManorTheMoneyBriggsUsedTo)},
	{TRIGGER_TALK, ACTOR_JIYA, FLAG_DEFENDED_THE_MAYOR,
	 EVENT(scAlhafraManorDefendTheMayor)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_JIYA | TRIGGER_SUBJECT_OWN_EFFECT, FLAG_DEFENDED_THE_MAYOR,
	 EVENT(scAlhafraManorDefendTheMayor)},
	{TRIGGER_TALK, ACTOR_MAYOR, FLAG_TOLD_OF_THE_EASTERN_SEA,
	 EVENT(scAlhafraManorTellOfTheEasternSea)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_MAYOR | TRIGGER_SUBJECT_OWN_EFFECT, FLAG_TOLD_OF_THE_EASTERN_SEA,
	 EVENT(scAlhafraManorTellOfTheEasternSea)},
	{TRIGGER_TALK, ACTOR_MAYOR, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraManorIAmPlanningAGlorious)},
	{TRIGGER_TALK, ACTOR_JIYA, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraManorTheMayorWontHelpFix)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraManorThisIsSuchAFantastic)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JIYA, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraManorTheMayorHatesSeeingAnyone)},
	{EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgAlhafraRoomsThatSeafoodStewHasAn))},
	{TRIGGER_TABLE_END},
};
