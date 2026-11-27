/* @file */
/* Atteka Inlet, where Felix ties up the Lemurian ship on the way to Contigo, */
/* in two scenes that share this overlay: the harbor, a sandy beach where the */
/* ship lies and the Wings of Anemos wait on their trestles */
/* (SceneId_AttekaInlet), and the grassy heights above it, with a house, the */
/* path north towards Contigo and the way west out to the world map */
/* (SceneId_AttekaInletGeode). On the deck, walking towards the ship's wheel */
/* brings Kraden up to ask whether to set sail */
/* (scAttekaInletKradenAsksToSetSail). Cyclone on the bush beside the round */
/* boulder on the heights uncovers Geode, a Venus Djinni */
/* (scAttekaInletCycloneUncoversGeode), and on eight other clumps of weeds only
 */
/* scatters them. Once Isaac's party has joined Felix at Contigo, two */
/* shipwrights block the way west, wanting to see their handiwork fly */
/* (scAttekaInletShipwrightsStopTheParty); Hamma waits at the foot of the */
/* gangplank to show the ship its new wings (scAttekaInletHammaShowsTheWings);
 */
/* and in a game linked with The Broken Seal, a messenger runs up the */
/* gangplank with Master Hammet's gift for Isaac */
/* (scAttekaInletHammetsMessengerArrives). */

#include "scene/event_layer.h"
#include "field/field.h"
#include "game/game.h"
#include "field/event_inline.h"
#include "field/field_sprite.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "scene/ground_effects.h"
#include "text/msg_ids.h"

/* The inlet's own story, which no other scene reads. */
#define FLAG_SHIPWRIGHTS_STOPPED_PARTY                                         \
	0x9b0 /* "We worked really hard on that ship" */
#define FLAG_HAMMA_SHOWED_THE_WINGS 0x9b1 /* Hamma showed the ship's wings */
#define FLAG_HAMMETS_GIFT_DELIVERED 0x9ad /* Master Hammet's messenger came */
#define FLAG_GEODE_UNCOVERED		FLAG_AREA_TEMP_00 /* Cyclone uncovered Geode */
/* The party told Kraden not to set out yet: "Please don't rush on my account."
 */
#define FLAG_TOLD_KRADEN_NOT_YET FLAG_SCENE_TEMP_01

/* The map cells the inlet's step and Cyclone triggers answer. */
typedef enum ATTEKA_INLET_CELL_BEHAVIOR {
	/* The harbor: the grass at the foot of the gangplank, where Hamma waits; */
	/* the deck before the ship's wheel; the deck at the head of the gangplank.
	 */
	CELL_BEHAVIOR_GANGPLANK_FOOT = 10,
	CELL_BEHAVIOR_BEFORE_THE_WHEEL = 11,
	CELL_BEHAVIOR_GANGPLANK_HEAD = 12,
	/* The heights: the way west out to the world map, where the shipwrights */
	/* stop the party. */
	CELL_BEHAVIOR_WAY_WEST = 10,
	/* The bush beside the round boulder, where Geode hides, and the nine clumps
	 */
	/* of weeds, in the order of scAttekaInletCycloneFirstWeeds and the rest. */
	CELL_BEHAVIOR_GEODES_BUSH = 20,
	CELL_BEHAVIOR_FIRST_WEEDS,
	CELL_BEHAVIOR_SECOND_WEEDS,
	CELL_BEHAVIOR_THIRD_WEEDS,
	CELL_BEHAVIOR_FOURTH_WEEDS,
	CELL_BEHAVIOR_FIFTH_WEEDS,
	CELL_BEHAVIOR_SIXTH_WEEDS,
	CELL_BEHAVIOR_SEVENTH_WEEDS,
	CELL_BEHAVIOR_EIGHTH_WEEDS,
	CELL_BEHAVIOR_NINTH_WEEDS
} ATTEKA_INLET_CELL_BEHAVIOR;

extern ACTOR_PLACE AttekaInlet_ActorPlacements[];
extern ACTOR_PLACE AttekaInlet_HarborActorPlacements[];
extern ACTOR_PLACE AttekaInlet_HarborActorPlacementsOnceWinged[];
extern ACTOR_PLACE AttekaInlet_HarborActorPlacementsOnceSetOut[];
extern ACTOR_PLACE AttekaInlet_HeightsActorPlacementsOnceWinged[];
extern ACTOR_PLACE AttekaInlet_HeightsActorPlacements[];
extern TRIGGER AttekaInlet_Triggers[];
extern TRIGGER AttekaInlet_HeightsTriggers[];
void scAttekaInletHammetsMessengerArrives(void);
void scAttekaInletSetUpTheHarbor(void);
void scAttekaInletSetUpTheHeights(void);

/* The harbor's people, numbering AttekaInlet_HarborActorPlacements and */
/* the tables after it. */
typedef enum ATTEKA_INLET_HARBOR_ACTOR_ID {
	/* Purple posts with glowing tops, along the shore and up the slope. */
	/* @{ */
	ACTOR_GLOWING_POST_A = ACTOR_FIRST_PLACED,
	ACTOR_GLOWING_POST_B,
	ACTOR_GLOWING_POST_C,
	ACTOR_GLOWING_POST_D,
	ACTOR_GLOWING_POST_E,
	/* @} */
	/* "I've been watching the skies, hoping I could catch another glimpse of */
	/* the flying ship." */
	ACTOR_YOUNG_MAN_WATCHING_THE_SKIES,
	/* "I wonder what's on the other side of the ocean..." */
	ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN,
	/* "We brought these wings from Contigo, and now we're guarding them." */
	ACTOR_WOMAN_GUARDING_THE_WINGS,
	/* "I wonder if these giant wings will really make it fly..." */
	ACTOR_BOY_ON_THE_WINGS,
	/* The cannon on the ship's deck, once the ship carries it. */
	ACTOR_CANNON,
	/* Kraden, who asks whether to set sail. */
	ACTOR_KRADEN,
	/* Once the ship has wings, four robed men come to see it fly, and Hamma. */
	/* "It's almost time... Time to watch a legend become reality!" */
	ACTOR_MAN_AWAITING_THE_LEGEND,
	/* "If everyone will just obey Hamma's request, the ship will surely fly."
	 */
	ACTOR_MAN_ON_HAMMAS_REQUEST,
	/* "When the winged ship of Anemos flies once again, the whole land shall be
	 */
	/* made free..." */
	ACTOR_MAN_ON_THE_WINGED_SHIP,
	/* "Once you remove the stone anchors weighing the ship down, it should */
	/* fly..." */
	ACTOR_MAN_ON_THE_STONE_ANCHORS,
	ACTOR_HAMMA,
	/* Master Hammet's messenger, in a game linked with The Broken Seal. */
	ACTOR_HAMMETS_MESSENGER
} ATTEKA_INLET_HARBOR_ACTOR_ID;

/* The heights above the harbor (AttekaInlet_HeightsActorPlacements). */
typedef enum ATTEKA_INLET_HEIGHTS_ACTOR_ID {
	/* "All of our myths and legends have taught us that the sea is a */
	/* dangerous, fearsome power. But on the day the ship left, the sea was so
	 */
	/* calm and romantic looking..." */
	ACTOR_WOMAN_ON_THE_CALM_SEA = ACTOR_FIRST_PLACED,
	/* The shipwrights who stop the party on its way west out of the inlet: */
	/* "Where do you think you're going?" */
	/* @{ */
	ACTOR_SHIPWRIGHT,
	ACTOR_SECOND_SHIPWRIGHT,
	/* @} */
	/* "We sent everyone back to the village so that their hopes would not
	   impair */
	/* your flight." */
	ACTOR_MAN_WHO_SENT_EVERYONE_BACK,
	/* The round boulder in front of Geode's bush, which Lift raises. */
	ACTOR_ROUND_BOULDER = ACTOR_FIRST_PLACED + 5,
	/* Geode, the Venus Djinni Cyclone uncovers. */
	ACTOR_GEODE
} ATTEKA_INLET_HEIGHTS_ACTOR_ID;

/* The chest Master Hammet's messenger leaves, holding his gift: hidden until */
/* he comes. */
#define ACTOR_HAMMETS_GIFT_CHEST (FIELD_TREASURE_ACTOR + 1)

/* Talking to Geode, facing east towards him where Cyclone uncovered him by the
 */
/* round boulder, makes him hop to the leader and join the party, the screen */
/* washing green as he does. */
void scAttekaInletGeodeJoins(void) {
	fdDjinnJoinParty(ACTOR_GEODE, DJINNI_GEODE / DJINN_PER_ELEMENT,
					 DJINNI_INDEX(DJINNI_GEODE));
	syGameFlagClear(FLAG_GEODE_UNCOVERED);
}

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_ATTEKA_INLET) {
		if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS))
			return AttekaInlet_HarborActorPlacementsOnceSetOut;
		if(syGameFlagGet(FLAG_REUNION_AT_CONTIGO))
			return AttekaInlet_HarborActorPlacementsOnceWinged;
		return AttekaInlet_HarborActorPlacements;
	}
	if(scene == SCENE_ATTEKA_INLET_GEODE) {
		if(syGameFlagGet(FLAG_REUNION_AT_CONTIGO))
			return AttekaInlet_HeightsActorPlacementsOnceWinged;
		return AttekaInlet_HeightsActorPlacements;
	}
	return AttekaInlet_ActorPlacements;
}

extern TRIGGER AttekaInlet_HarborTriggers[];

const TRIGGER *scGetTriggers(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_ATTEKA_INLET)
		return AttekaInlet_HarborTriggers;
	if(scene == SCENE_ATTEKA_INLET_GEODE)
		return AttekaInlet_HeightsTriggers;
	return AttekaInlet_Triggers;
}

/* Talking to Kraden on the deck has him ask "Felix, are we setting off now?":
 */
/* yes sails the ship out of the inlet onto the world map, no has him say */
/* "Please don't rush on my account" and walk back into the leader. */
/* Once the parties have met again at Contigo, setting out takes the wings */
/* along (FLAG_SHIP_FLIES, FLAG_SET_OUT_WITH_WINGS). */
void scAttekaInletAskKradenToSetSail(void) {
	ACTOR *actor;

	/* The player never finds him to talk to: Kraden is placed on the harbor's
	 */
	/* stage only by scAttekaInletKradenAsksToSetSail, and answered No there he
	 */
	/* walks back into Felix and is gone from the deck. */
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId(MsgAreWeSetting);
	/* "Felix, are we setting off now?" Yes: "Then, to sea we go!" No: "Please
	 */
	/* don't rush on my account. Let's enjoy..." */
	fdMessageShowAtActor(ACTOR_KRADEN, 0);
	if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
										 0) == 0) {
		fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
		/* "Then, to sea we go!" */
		fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
		MainRoutineStateWork.ownerOverride = 1;
		if(fdEventGetFlag(FLAG_REUNION_AT_CONTIGO)) {
			syGameFlagSet(FLAG_SHIP_FLIES);
			syGameFlagSet(FLAG_SET_OUT_WITH_WINGS);
		}
		syImportEventSetExitRequest(ATTEKA_INLET_TO_WORLD_MAP_WEST);
	}
	else {
		(FieldStateWork->msg)++;
		/* "Please don't rush on my account. Let's enjoy our stay." */
		fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
		fdActorSetSpeed(ACTOR_KRADEN, 1.6);
		fdActorSetAnimationById(ACTOR_KRADEN, ANIM_WALK);
		actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
		if(actor != NULL)
			fdActorSetDestinationById(ACTOR_KRADEN, actor->x.part[0].integer,
									  actor->z.part[0].integer);
		fdActorWaitForMoveById(ACTOR_KRADEN);
		fdActorSetPos(ACTOR_KRADEN, 0, 0);
		syGameFlagSet(FLAG_TOLD_KRADEN_NOT_YET);
		fdEventEnd();
	}
}

/* Walking west across the heights towards the way out to the world map (cells
 */
/* (5, 18) to (6, 20)), once Isaac's party has joined Felix and before the ship
 */
/* has flown, two shipwrights jump up from below: "Where do you think you're */
/* going?" They want to see their handiwork take flight, and stay standing in */
/* the way, so the party can only leave by the ship. Walls copied onto the */
/* cells beside them (`fdMapCopyCellAttributeRect`) close the path for good. */
void scAttekaInletShipwrightsStopTheParty(void) {
	if(syGameFlagGet(FLAG_REUNION_AT_CONTIGO) &&
	   !syGameFlagGet(FLAG_SET_OUT_WITH_WINGS)) {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		fdCameraMoveTo(96, CAMERA_KEEP, 308, 1);
		fdActorSetPosAndFacing(ACTOR_SHIPWRIGHT, toFIXED(60), toFIXED(362),
							   FACING_NORTH_NORTHEAST);
		sySynch(1);
		fdActorSetSpeed(ACTOR_SHIPWRIGHT, 2);
		fdActorJump(ACTOR_SHIPWRIGHT, 6, 0);
		fdActorMoveToAndWait(ACTOR_SHIPWRIGHT, 64, 336);
		fdActorWalkToAndWait(ACTOR_SHIPWRIGHT, 70, 308);
		fdActorFaceDirectionById(ACTOR_SHIPWRIGHT, FACING_EAST, 0);
		fdActorSetPosAndFacing(ACTOR_SECOND_SHIPWRIGHT, toFIXED(94),
							   toFIXED(272), FACING_SOUTH_SOUTHWEST);
		sySynch(1);
		fdActorSetSpeed(ACTOR_SECOND_SHIPWRIGHT, 2);
		fdActorJump(ACTOR_SECOND_SHIPWRIGHT, 6, 0);
		fdActorMoveToAndWait(ACTOR_SECOND_SHIPWRIGHT, 90, 288);
		fdActorWalkToAndWaitById(ACTOR_SECOND_SHIPWRIGHT, 80, 316);
		fdActorFaceDirectionAndRefreshById(ACTOR_SECOND_SHIPWRIGHT,
										   FACING_EAST);
		fdActorGetById(ACTOR_SHIPWRIGHT)->velY = toFIXED(4);
		fdEventWait(10);
		fdEventSetMessageId((s32)MsgWhereDoYouThink);
		/* "Where do you think you're going?" */
		fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHIPWRIGHT, 0);
		fdActorShowEmoteById(ACTOR_SECOND_SHIPWRIGHT,
							 EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
		/* "We worked really hard on that ship of yours!" */
		fdEventShowMessageAndWait(ACTOR_SECOND_SHIPWRIGHT, 0);
		fdActorFaceDirection(ACTOR_SHIPWRIGHT, FACING_SOUTH_SOUTHEAST, 0);
		/* "I want to see our handiwork take flight. Don't you, buddy?" */
		fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHIPWRIGHT, 0);
		fdActorFaceDirectionAndWait(ACTOR_SECOND_SHIPWRIGHT,
									FACING_NORTH_NORTHWEST);
		fdActorSetAnimationById(ACTOR_SECOND_SHIPWRIGHT, ANIM_NOD);
		/* "I want to! Boy, do I want to!" */
		fdEventShowMessageAndWait(ACTOR_SECOND_SHIPWRIGHT, 0);
		fdActorFaceDirectionById(ACTOR_SECOND_SHIPWRIGHT, FACING_EAST, 0);
		fdActorShowEmoteById(ACTOR_SHIPWRIGHT, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
		fdActorFaceDirectionById(ACTOR_SHIPWRIGHT, FACING_EAST, 20);
		/* "So hurry up! We want to see the boat fly!" */
		fdEventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_SHIPWRIGHT, 0);
		/* "Thanks, buddy!" */
		fdEventShowMessageAndWait(ACTOR_SECOND_SHIPWRIGHT, 0);
		fdMapCopyCellAttributeRect(1, 16, 1, 1, 3, 19);
		fdMapCopyCellAttributeRect(1, 16, 1, 1, 4, 19);
		fdMapCopyCellAttributeRect(1, 16, 1, 1, 4, 20);
		fdMapCopyCellAttributeRect(1, 16, 1, 1, 4, 21);
		syGameFlagSet(FLAG_SHIPWRIGHTS_STOPPED_PARTY);
		fdEventEnd();
	}
}

/* Stepping off the gangplank onto the grass below the ship (cells (31, 24) to
 */
/* (34, 24)), once Isaac's party has joined Felix, has Hamma greet them, "I've
 */
/* been waiting for you, Felix", while the camera looks over the ship and its */
/* new wings; she tells them to raise it with Hover and set sail. */
void scAttekaInletHammaShowsTheWings(void) {
	if(syGameFlagGet(FLAG_REUNION_AT_CONTIGO)) {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		fdCameraSetSpeed(2.4);
		fdCameraMoveTo(520, CAMERA_KEEP, 372, 1);
		fdCameraWaitForArrival();
		fdActorShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
		fdActorFaceDirection(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST, 20);
		fdEventSetMessageId((s32)MsgIVeBeenWaiting);
		/* "I've been waiting for you, Felix." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		fdActorFaceDirectionAndWait(ACTOR_HAMMA, FACING_NORTH_NORTHWEST);
		/* "Take a look at your ship." */
		fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMA, 0);
		fdCameraGetActor()->motionFlg = 0;
		fdCameraSetSpeed(4.8);
		fdCameraMoveTo(440, 32, 368, 1);
		fdCameraWaitForArrival();
		fdEventWait(40);
		fdCameraMoveTo(432, 32, 232, 1);
		fdCameraWaitForArrival();
		fdEventWait(40);
		fdCameraSetSpeed(4.8);
		fdCameraMoveTo(520, 32, 372, 1);
		fdCameraWaitForArrival();
		fdEventWait(20);
		fdActorFaceDirectionAndWait(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST);
		fdActorSetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
		/* "Now that you have been given wings, all obstacles in your path
		 * will..." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		fdActorShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
		/* "However, it is not the wings alone that grant your ship the power
		 * of..." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		fdActorSetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
		/* "Psynergy is the force that powers the Wings of Anemos." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		/* "If the ship is to fly, you will need to focus the power of your..."
		 */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		fdActorFaceDirectionAndWait(ACTOR_HAMMA, FACING_NORTH_NORTHWEST);
		/* "You have visited Shaman Village recently, have you not?" */
		fdEventShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMA, 0);
		fdActorFaceDirectionAndWait(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST);
		/* "Now is the time to make use of the powers you acquired there." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		fdActorSetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
		/* "Now... Use the power of Hover to raise your ship!" */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		fdActorSetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
		/* "At first, you may not want to move your ship too high or too far..."
		 */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		fdActorShowEmoteById(MainRoutineStateWork.leaderActorId,
							 EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
		fdActorShowEmoteById(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
							 20);
		/* "It will be difficult at first, but that is to be expected...
		 * After..." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		fdActorShowEmoteById(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
		/* "But your quest should not demand much more of you..." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		fdActorSetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
		/* "Now, be confident and set sail!" */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		fdActorSetSpeed(ACTOR_HAMMA, 0.8);
		fdActorGetById(ACTOR_HAMMA)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkToAndWaitById(ACTOR_HAMMA, 504, 360);
		fdEventWait(1);
		fdActorGetById(ACTOR_HAMMA)->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorFaceDirectionAndRefreshById(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST);
		fdActorSetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
		/* "All of Contigo is watching... Show them your power, and make
		 * your..." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		syGameFlagSet(FLAG_HAMMA_SHOWED_THE_WINGS);
		fdEventEnd();
	}
}

/* Walking east along the deck towards the ship's wheel (cell (34, 16)) brings
 */
/* Kraden out from the party to ask "Felix, are we setting off now?": yes sails
 */
/* the ship out of the inlet onto the world map, no sends him back into the */
/* party and Felix a step south, off the spot, so that Kraden asks again each */
/* time he comes back to it. */
void scAttekaInletKradenAsksToSetSail(void) {
	ACTOR *actor;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorPlaceOnActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
	sySynch(1);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,
							 FACING_NORTHWEST, 0);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorWalkToAndWaitById(ACTOR_KRADEN, 532, 248);
	fdActorFaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
	fdEventSetMessageId(MsgAreWeSetting);
	/* "Felix, are we setting off now?" Yes: "Then, to sea we go!" No: "Please
	 */
	/* don't rush on my account. Let's enjoy..." */
	fdMessageShowAtActor(ACTOR_KRADEN, 0);
	if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
										 0) == 0) {
		fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
		/* "Then, to sea we go!" */
		fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
		MainRoutineStateWork.ownerOverride = 1;
		if(fdEventGetFlag(FLAG_REUNION_AT_CONTIGO)) {
			syGameFlagSet(FLAG_SHIP_FLIES);
			syGameFlagSet(FLAG_SET_OUT_WITH_WINGS);
		}
		syImportEventSetExitRequest(ATTEKA_INLET_TO_WORLD_MAP_WEST);
	}
	else {
		(FieldStateWork->msg)++;
		/* "Please don't rush on my account. Let's enjoy our stay." */
		fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
		fdActorSetSpeed(ACTOR_KRADEN, 1.6);
		fdActorSetAnimationById(ACTOR_KRADEN, ANIM_WALK);
		actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
		if(actor != NULL)
			fdActorSetDestinationById(ACTOR_KRADEN, actor->x.part[0].integer,
									  actor->z.part[0].integer);
		fdActorWaitForMoveById(ACTOR_KRADEN);
		fdActorSetPos(ACTOR_KRADEN, 0, 0);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
		fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 528, 276);
		fdEventEnd();
	}
}

/* Stepping onto the deck at the head of the gangplank (cell (32, 20)) after */
/* Hamma has shown the wings, in a game whose Broken Seal data has Dodonpa */
/* locked up, brings Master Hammet's messenger running after the party, once. */
void scAttekaInletCheckForHammetsMessenger(void) {
	if(syGameFlagGet(FLAG_TRANSFERRED_DODONPA_LOCKED_UP) &&
	   syGameFlagGet(FLAG_HAMMA_SHOWED_THE_WINGS) &&
	   !syGameFlagGet(FLAG_HAMMETS_GIFT_DELIVERED))
		scAttekaInletHammetsMessengerArrives();
}

/* Cyclone on the bush beside the round boulder, below the cliff on the */
/* heights (cell (7, 23)), blows its leaves away and, unless the party has him
 */
/* already, Geode hops out of it; a leader standing in the bush hops aside to */
/* the west first. */
void scAttekaInletCycloneUncoversGeode(void) {
	ACTOR *actor;
	s32 moved;

	syFieldTileCopiesCopyTouchedCells(9, 23, 1, 1, 7, 23, 0);
	if(!syGameFlagGet(FLAG_DJINNI(DJINNI_GEODE))) {
		actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		moved = 0;
		if(FIELD_CELL(actor->x.fixed) == 7 &&
		   FIELD_CELL(actor->z.fixed) == 23) {
			fdActorJump(MainRoutineStateWork.leaderActorId, 6, 0);
			fdActorSetDestination(MainRoutineStateWork.leaderActorId, 104, 376);
			moved = 1;
		}
		fdActorSetPos(ACTOR_GEODE, 120, 376);
		sySynch(1);
		fdActorJump(ACTOR_GEODE, 4, 10);
		syGameFlagSet(FLAG_GEODE_UNCOVERED);
		if(moved)
			fdActorFaceDirectionAndRefreshById(
				MainRoutineStateWork.leaderActorId, FACING_EAST);
		fdEventEnd();
	}
}

/* Cyclone on the westmost clump of weeds on the heights, south-west of the two
 */
/* pillars on the path north (cell (4, 14)), scatters it. Each clump's event */
/* copies bare ground from a cell nearby over the clump's cells that the whirl
 */
/* touched; on raised ground, as all nine are, a clump is drawn four rows north
 */
/* of the cell it answers on. */
void scAttekaInletCycloneFirstWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(9, 23, 1, 1, 4, 10, 0);
}

/* Cyclone on the clump of weeds just north-east of the westmost one, west of */
/* the two pillars on the path north (cell (6, 13)), scatters it. */
void scAttekaInletCycloneSecondWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(9, 23, 1, 1, 6, 9, 0);
}

/* Cyclone on the clump of weeds just north-west of the west pillar on the */
/* path north (cell (6, 10)), scatters it. */
void scAttekaInletCycloneThirdWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(9, 23, 1, 1, 6, 6, 0);
}

/* Cyclone on the clump of weeds just north-east of the east pillar on the */
/* path north (cell (12, 10)), scatters it. */
void scAttekaInletCycloneFourthWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(12, 5, 1, 1, 12, 6, 0);
}

/* Cyclone on the clump of weeds east of the east pillar on the path north */
/* (cell (13, 11)), scatters it. */
void scAttekaInletCycloneFifthWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(9, 23, 1, 1, 13, 7, 0);
}

/* Cyclone on the clump of weeds north-east of the east pillar, by the cliff's
 */
/* edge (cell (14, 10)), scatters it. */
void scAttekaInletCycloneSixthWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(9, 23, 1, 1, 14, 6, 0);
}

/* Cyclone on the west clump of the three on the clifftop above the house */
/* (cell (21, 10)) scatters it. */
void scAttekaInletCycloneSeventhWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(9, 23, 1, 1, 21, 6, 0);
}

/* Cyclone on the middle clump of the three on the clifftop above the house */
/* (cell (22, 10)) scatters it. */
void scAttekaInletCycloneEighthWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(9, 23, 1, 1, 22, 6, 0);
}

/* Cyclone on the east clump of the three on the clifftop above the house */
/* (cell (23, 11)) scatters it. */
void scAttekaInletCycloneNinthWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(9, 23, 1, 1, 23, 7, 0);
}

/* Master Hammet's messenger runs up the gangplank calling "Hey, wait!", sets */
/* down a chest with the gift he was to bring Isaac and runs off again; Hamma */
/* tells how Hammet, hurt that Isaac never came back to Kalay, sent it in */
/* thanks (FLAG_HAMMETS_GIFT_DELIVERED). */
void scAttekaInletHammetsMessengerArrives(void) {
	s32 angle;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	angle = FACING_NORTH_NORTHEAST;
	fdActorSetHorizontalPosAndAngWithTerrain(ACTOR_HAMMETS_MESSENGER,
											 toFIXED(504), toFIXED(460), angle);
	sySynch(1);
	fdEventSetMessageId((s32)MsgAttekaInletHeyWait);
	/* "Hey, wait!" */
	fdEventShowMessageAndWait(ACTOR_HAMMETS_MESSENGER, 0);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_SOUTH,
							 0);
	fdActorFaceDirection(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST, 0);
	fdCameraSetSpeed(1.6);
	fdCameraGetActor()->motionFlg = 0;
	fdCameraMoveTo(512, CAMERA_KEEP, 380, 1);
	fdActorSetSpeed(ACTOR_HAMMETS_MESSENGER, 1.6);
	fdActorWalkToAndWait(ACTOR_HAMMETS_MESSENGER, 520, 432);
	fdActorWalkToAndWait(ACTOR_HAMMETS_MESSENGER, 520, 388);
	fdActorFaceDirection(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST, 0);
	/* "Master Hammet requested that I deliver this to Isaac, and I totally..."
	 */
	fdEventShowMessageAndWait(ACTOR_HAMMETS_MESSENGER, 0);
	fdActorShowEmoteById(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
	/* "Well, you'd better hurry." */
	fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
	fdActorShowEmote(ACTOR_HAMMETS_MESSENGER, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
					 0);
	/* "I'm supposed to deliver this to Isaac... It's not too late, is it?" */
	fdEventShowMessageAndWait(ACTOR_HAMMETS_MESSENGER, 0);
	fdActorSetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
	fdActorFaceDirectionAndRefreshById(ACTOR_HAMMETS_MESSENGER, angle);
	fdActorGetById(ACTOR_HAMMETS_MESSENGER)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_HAMMETS_MESSENGER, 520, 384);
	fdEventWait(1);
	fdActorGetById(ACTOR_HAMMETS_MESSENGER)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorRunRepeatedMotion(ACTOR_HAMMETS_MESSENGER, 2);
	fdActorSetPos(ACTOR_HAMMETS_GIFT_CHEST, 520, 380);
	sySynch(1);
	fdActorGetById(ACTOR_HAMMETS_MESSENGER)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_HAMMETS_MESSENGER, 520, 388);
	fdEventWait(1);
	fdActorGetById(ACTOR_HAMMETS_MESSENGER)->steeringFlg |= ACTOR_FACES_MOTION;
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_HAMMETS_MESSENGER, FACING_NORTH_NORTHWEST, 10);
	fdActorSetAnimationAndWait(ACTOR_HAMMETS_MESSENGER, ANIM_NOD);
	fdActorWalkToAndWait(ACTOR_HAMMETS_MESSENGER, 520, 434);
	fdCameraMoveTo(512, CAMERA_KEEP, 348, 1);
	fdActorWalkToAndWait(ACTOR_HAMMETS_MESSENGER, 504, 460);
	fdActorSetPos(ACTOR_HAMMETS_MESSENGER, 0, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_HAMMA, angle);
	/* "On my way here, I stopped in Kalay briefly to speak with Master Hammet."
	 */
	fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
	fdActorShowEmoteById(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
	/* "He was quite upset that you hadn't returned to visit him so that he..."
	 */
	fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
	fdActorSetAnimationById(ACTOR_HAMMA, ANIM_NOD);
	/* "So he requested that I take this with me to Atteka." */
	fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
	fdActorSetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
	fdActorShowEmoteById(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	/* "It is a gift of thanks from Master Hammet. Take it, please." */
	fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
	syGameFlagSet(FLAG_HAMMETS_GIFT_DELIVERED);
	fdEventEnd();
}

/* Nothing visible happens: each frame in the harbor, the boy by the wings and
 */
/* two of the glowing posts are layered with the leader, so that he passes in */
/* front of or behind them as he does the scenery around them. Each takes the */
/* leader's sprite priority. */
void scAttekaInletShareLeadersPriority(void) {
	s32 variant =
		((FIELD_SPR *)fdActorGetById(MainRoutineStateWork.leaderActorId)->spr)
			->pri;

	fdActorSetSprPriority(ACTOR_BOY_ON_THE_WINGS, variant);
	fdActorSetSprPriority(ACTOR_GLOWING_POST_A, variant);
	fdActorSetSprPriority(ACTOR_GLOWING_POST_C, variant);
}

/* Nothing visible happens: each frame on the heights, the round boulder is */
/* layered with the leader, as the harbor's posts are, by taking his sprite */
/* priority. */
void scAttekaInletShareLeadersPriorityWithBoulder(void) {
	fdActorSetSprPriority(
		ACTOR_ROUND_BOULDER,
		((FIELD_SPR *)fdActorGetById(MainRoutineStateWork.leaderActorId)->spr)
			->pri);
}

/* Turns the blues of the band along the ship's hull and the surf's colours on
 */
/* one step every eight frames. */
void scAttekaInletRotateHullBandAndSurfCols(void) {
	if((*Ram_FrameCount & 7) == 0) {
		s16 saved;
		s32 index;

		saved = BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, HULL_BAND_LAST_COLOR);
		for(index = HULL_BAND_LAST_COLOR; index != HULL_BAND_FIRST_COLOR;
			index--)
			BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, index) =
				BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, index - 1);
		BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, HULL_BAND_FIRST_COLOR) = saved;
	}
	if((*Ram_FrameCount & 7) == 0) {
		s16 saved;
		s32 index;

		saved = BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, SURF_LAST_COLOR);
		for(index = SURF_LAST_COLOR; index != SURF_FIRST_COLOR; index--)
			BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, index) =
				BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, index - 1);
		BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, SURF_FIRST_COLOR) = saved;
	}
}

/* Walking north into the door of the cabin on the ship's main deck (cell */
/* (27, 18)) opens it; Felix walks in and the screen fades out to the cabins */
/* below. The exit taken is the one the door's cell names. */
void scAttekaInletOpenDoorAndEnter(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridCopyCellsTo(51, 38, 70, 22, 1, 2);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorCenterAndWalk(MainRoutineStateWork.leaderActorId, 2, -4);
	fdEventWait(8);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(state->stepTrigger);
}

s32 scInit(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_ATTEKA_INLET)
		scAttekaInletSetUpTheHarbor();
	else if(scene == SCENE_ATTEKA_INLET_GEODE)
		scAttekaInletSetUpTheHeights();
	return 0;
}

s32 scPrepareMap(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_ATTEKA_INLET && syGameFlagGet(FLAG_REUNION_AT_CONTIGO)) {
		fdMapGridCopyCellTilesIndicesRect(54, 47, 4, 15, 10, 4);
		fdMapGridCopyCellTilesIndicesRect(54, 38, 73, 7, 10, 4);
		fdMapGridCopyCellTilesIndicesRect(54, 43, 21, 55, 10, 3);
		fdMapGridCopyCellTilesIndicesRect(65, 43, 21, 20, 10, 3);
		fdMapGridCopyCellTilesIndicesRect(4, 47, 4, 50, 10, 3);
	}
	return 0;
}

/* The harbor on entry: the ship is moored here, the waves along its hull and */
/* the surf on the beach move, Hammet's chest stays hidden until his messenger
 */
/* brings it, and once the ship has its wings the four robed men and Hamma */
/* stand watching the party until it has set out. */
void scAttekaInletSetUpTheHarbor(void) {
	MAP_STATE_GROUND_EFFECTS *map =
		(MAP_STATE_GROUND_EFFECTS *)Ram_HeapSlots->mapGrid;
	MAIN_ROUTINE_STATE *party;

	syGameFlagSet(FLAG_SHIP_AT_ATTEKA_INLET);
	if(!syGameFlagGet(FLAG_HAMMETS_GIFT_DELIVERED))
		fdActorSetPos(ACTOR_HAMMETS_GIFT_CHEST, 0, 0);
	map->wading = MAP_WADING_UPPER_LEVEL;
	party = &MainRoutineStateWork;
	fdActorGetById(party->leaderActorId)->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	syFieldTaskAddFrame(scAttekaInletRotateHullBandAndSurfCols,
						TASK_PASS_FRAME);
	syFieldTaskAddFrame(scAttekaInletShareLeadersPriority, TASK_PASS_FRAME);
	fdActorSetSprFlags(fdActorGetById(ACTOR_CANNON), 0);
	fdActorGetById(ACTOR_CANNON)->motionFlg = 0;
	if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS)) {
		fdActorSetPos(ACTOR_MAN_AWAITING_THE_LEGEND, 0, 0);
		fdActorSetPos(ACTOR_MAN_ON_HAMMAS_REQUEST, 0, 0);
		fdActorSetPos(ACTOR_MAN_ON_THE_WINGED_SHIP, 0, 0);
		fdActorSetPos(ACTOR_MAN_ON_THE_STONE_ANCHORS, 0, 0);
		fdActorSetPos(ACTOR_HAMMA, 0, 0);
	}
	else {
		if(syGameFlagGet(FLAG_HAMMA_SHOWED_THE_WINGS))
			fdActorSetPosAndFacing(ACTOR_HAMMA, toFIXED(504), toFIXED(360),
								   FACING_SOUTH_SOUTHEAST);
		fdActorWatchAndSetScript(ACTOR_MAN_AWAITING_THE_LEGEND,
								 party->leaderActorId);
		fdActorWatchAndSetScript(ACTOR_MAN_ON_HAMMAS_REQUEST,
								 party->leaderActorId);
		fdActorWatchAndSetScript(ACTOR_MAN_ON_THE_WINGED_SHIP,
								 party->leaderActorId);
		fdActorWatchAndSetScript(ACTOR_MAN_ON_THE_STONE_ANCHORS,
								 party->leaderActorId);
	}
}

/* The heights on entry: the shipwrights and the robed man are gone once the */
/* ship has flown, and until then, once they have stopped the party, the */
/* shipwrights stand blocking the way west; Geode stands by the round boulder */
/* once uncovered, until he joins. */
void scAttekaInletSetUpTheHeights(void) {
	if(MainRoutineStateWork.entrance == ATTEKA_INLET_GEODE_FROM_CONTIGO_SHOPS)
		syGameFlagClear(FLAG_SHOW_AREA_NAME);
	if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS)) {
		fdActorSetPos(ACTOR_SHIPWRIGHT, 0, 0);
		fdActorSetPos(ACTOR_SECOND_SHIPWRIGHT, 0, 0);
		fdActorSetPos(ACTOR_MAN_WHO_SENT_EVERYONE_BACK, 0, 0);
	}
	else if(syGameFlagGet(FLAG_SHIPWRIGHTS_STOPPED_PARTY)) {
		fdActorSetPosAndFacing(ACTOR_SHIPWRIGHT, toFIXED(70), toFIXED(308),
							   FACING_EAST);
		fdActorSetPosAndFacing(ACTOR_SECOND_SHIPWRIGHT, toFIXED(80),
							   toFIXED(316), FACING_EAST);
		fdMapCopyCellAttributeRect(1, 16, 1, 1, 3, 19);
		fdMapCopyCellAttributeRect(1, 16, 1, 1, 4, 19);
		fdMapCopyCellAttributeRect(1, 16, 1, 1, 4, 20);
		fdMapCopyCellAttributeRect(1, 16, 1, 1, 4, 21);
		sySynch(1);
	}
	if(syGameFlagGet(FLAG_GEODE_UNCOVERED))
		fdActorSetPos(ACTOR_GEODE, 120, 376);
	sySetEvent(scAttekaInletShareLeadersPriorityWithBoulder, TASK_PASS_FRAME);
}

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 72, 0, 295, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 SceneExits[] = {
	SCENE_EXITS(AttekaInlet),
	SCENE_EXIT(ATTEKA_INLET_TO_LEMURIAN_SHIP_CABINS, LemurianShipCabins,
			   LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_MAIN_DECK),
	SCENE_EXIT(ATTEKA_INLET_TO_ATTEKA_INLET_GEODE_SOUTH, AttekaInletGeode,
			   ATTEKA_INLET_GEODE_FROM_ATTEKA_INLET_SOUTH),
	SCENE_EXIT(ATTEKA_INLET_TO_ATTEKA_INLET_GEODE_NORTH, AttekaInletGeode,
			   ATTEKA_INLET_GEODE_FROM_ATTEKA_INLET_NORTH),
	SCENE_EXIT(ATTEKA_INLET_TO_WORLD_MAP_WEST, WorldMap,
			   WORLD_MAP_FROM_ATTEKA_INLET_EAST),
	SCENE_EXITS(AttekaInletGeode),
	SCENE_EXIT(ATTEKA_INLET_GEODE_TO_ATTEKA_INLET_SOUTH, AttekaInlet,
			   ATTEKA_INLET_FROM_ATTEKA_INLET_GEODE_SOUTH),
	SCENE_EXIT(ATTEKA_INLET_GEODE_TO_WORLD_MAP_NORTH, WorldMap,
			   WORLD_MAP_FROM_ATTEKA_INLET_GEODE_NORTH),
	SCENE_EXIT(ATTEKA_INLET_GEODE_TO_CONTIGO_SHOPS, ContigoShops,
			   CONTIGO_SHOPS_FROM_ATTEKA_INLET_GEODE),
	SCENE_EXIT(4, AttekaInletGeode, 4),
	SCENE_EXIT(ATTEKA_INLET_GEODE_TO_ATTEKA_INLET_NORTH, AttekaInlet,
			   ATTEKA_INLET_FROM_ATTEKA_INLET_GEODE_NORTH),
	SCENE_EXIT(ATTEKA_INLET_GEODE_TO_WORLD_MAP_SOUTH, WorldMap,
			   WORLD_MAP_FROM_ATTEKA_INLET_GEODE_NORTH),
	SCENE_EXITS_END,
};

/* The scene's other rooms' actors. */
ACTOR_PLACE AttekaInlet_ActorPlacements[] = {
	{SCENE_TABLE_END},
};

/* The harbor's actors, in ATTEKA_INLET_HARBOR_ACTOR_ID's order. */
ACTOR_PLACE AttekaInlet_HarborActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 88, 0, 376, FACING_SOUTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 216, 0, 328, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 168, 0, 184, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 488, 0, 408, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 552, 0, 424, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 152, 0, 296, FACING_SOUTHEAST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 80, 0, 304, FACING_NORTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 208, 0, 280, FACING_WEST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_SPIKY_HAIR, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 184, 0, 224, FACING_SOUTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_CANNON, IF_FLAG_SET(FLAG_SHIP_CARRIES_CANNON),
					MOVEMENT_TYPE_STAND, 464, 0, 264, FACING_EAST,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
					0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

/* The harbor's actors once the party has met again at Contigo and the ship */
/* has its wings. */
ACTOR_PLACE AttekaInlet_HarborActorPlacementsOnceWinged[] = {
	ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 88, 0, 376, FACING_SOUTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 216, 0, 328, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 168, 0, 184, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 488, 0, 408, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 552, 0, 424, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 216, 0, 168, FACING_SOUTHEAST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 55, 0, 234, FACING_NORTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_SPIKY_HAIR, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 260, 0, 158, FACING_SOUTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_CANNON, IF_FLAG_SET(FLAG_SHIP_CARRIES_CANNON),
					MOVEMENT_TYPE_STAND, 464, 0, 264, FACING_EAST,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
					0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 504, 0, 424, FACING_EAST,
					TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 536, 0, 424, FACING_WEST,
					TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 504, 0, 408, FACING_EAST,
					TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 536, 0, 408, FACING_EAST,
					TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_HAMMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 516, 0,
					360, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH,
					0),
	ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

/* The harbor's actors once the ship has set out with its wings. */
ACTOR_PLACE AttekaInlet_HarborActorPlacementsOnceSetOut[] = {
	ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 88, 0, 376, FACING_SOUTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 216, 0, 328, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 168, 0, 184, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 488, 0, 408, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 552, 0, 424, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 162, 0, 300, FACING_SOUTHEAST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 111, 0, 327, FACING_NORTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_SPIKY_HAIR, CONDITION_ALWAYS,
					MOVEMENT_TYPE_WANDER_NEAR, 204, 0, 204, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CANNON, IF_FLAG_SET(FLAG_SHIP_CARRIES_CANNON),
					MOVEMENT_TYPE_STAND, 464, 0, 264, FACING_EAST,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
					0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

/* The heights' actors once the ship has its wings. */
ACTOR_PLACE AttekaInlet_HeightsActorPlacementsOnceWinged[] = {
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
					MOVEMENT_TYPE_WANDER_NEAR, 264, 0, 352, FACING_WEST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 380, 0, 280, FACING_SOUTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_CANNON, IF_FLAG_SET(FLAG_SHIP_CARRIES_CANNON),
					MOVEMENT_TYPE_STAND, 464, 0, 264, FACING_EAST,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					104, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_GEODE),
					MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

/* The heights' actors, in ATTEKA_INLET_HEIGHTS_ACTOR_ID's order. */
ACTOR_PLACE AttekaInlet_HeightsActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
					MOVEMENT_TYPE_WANDER_NEAR, 0, 0, 0, FACING_WEST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_CANNON, IF_FLAG_SET(FLAG_SHIP_CARRIES_CANNON),
					MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP,
					0),
	ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					104, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_GEODE),
					MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

/* The scene's other rooms' triggers. */
TRIGGER AttekaInlet_Triggers[] = {
	{TRIGGER_TABLE_END},
};

/* The harbor's exits, the cabin door, Hamma's and Kraden's spots, talks and */
/* the messenger's check. */
TRIGGER AttekaInlet_HarborTriggers[] = {
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 ATTEKA_INLET_TO_LEMURIAN_SHIP_CABINS, CONDITION_ALWAYS,
	 EVENT(scAttekaInletOpenDoorAndEnter)},
	{TRIGGER_EXIT, ATTEKA_INLET_TO_ATTEKA_INLET_GEODE_SOUTH, CONDITION_ALWAYS,
	 ATTEKA_INLET_TO_ATTEKA_INLET_GEODE_SOUTH},
	{TRIGGER_EXIT, ATTEKA_INLET_TO_ATTEKA_INLET_GEODE_NORTH, CONDITION_ALWAYS,
	 ATTEKA_INLET_TO_ATTEKA_INLET_GEODE_NORTH},
	{TRIGGER_STEP, CELL_BEHAVIOR_GANGPLANK_FOOT, FLAG_HAMMA_SHOWED_THE_WINGS,
	 EVENT(scAttekaInletHammaShowsTheWings)},
	{TRIGGER_TALK, ACTOR_KRADEN, CONDITION_ALWAYS,
	 EVENT(scAttekaInletAskKradenToSetSail)},
	{TRIGGER_STEP, CELL_BEHAVIOR_BEFORE_THE_WHEEL, CONDITION_ALWAYS,
	 EVENT(scAttekaInletKradenAsksToSetSail)},
	{TRIGGER_STEP, CELL_BEHAVIOR_GANGPLANK_HEAD, CONDITION_ALWAYS,
	 EVENT(scAttekaInletCheckForHammetsMessenger)},
	{TRIGGER_TALK, ACTOR_YOUNG_MAN_WATCHING_THE_SKIES, FLAG_REUNION_AT_CONTIGO,
	 MSGNO(MsgWeVeMade)},
	{TRIGGER_TALK, ACTOR_YOUNG_MAN_WATCHING_THE_SKIES, FLAG_SET_OUT_WITH_WINGS,
	 MSGNO(MsgHammaReturnedTo)},
	{TRIGGER_TALK, ACTOR_YOUNG_MAN_WATCHING_THE_SKIES, CONDITION_ALWAYS,
	 MSGNO(MsgIVeBeenWatching)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WATCHING_THE_SKIES,
	 FLAG_REUNION_AT_CONTIGO, MSGNO(MsgWeFirstHeard)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WATCHING_THE_SKIES,
	 FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgWeVeBeen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WATCHING_THE_SKIES,
	 CONDITION_ALWAYS, MSGNO(MsgWheneverILook)},
	{TRIGGER_TALK, ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN,
	 FLAG_REUNION_AT_CONTIGO, MSGNO(MsgWeMustBe)},
	{TRIGGER_TALK, ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN,
	 FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgWasYourBoat)},
	{TRIGGER_TALK, ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN, CONDITION_ALWAYS,
	 MSGNO(MsgIWonderWhat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN, FLAG_REUNION_AT_CONTIGO,
	 MSGNO(MsgTheseWingsAre)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN, FLAG_SET_OUT_WITH_WINGS,
	 MSGNO(MsgWhoBuiltThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN, CONDITION_ALWAYS,
	 MSGNO(MsgIWishI)},
	{TRIGGER_TALK, ACTOR_WOMAN_GUARDING_THE_WINGS, CONDITION_ALWAYS,
	 MSGNO(MsgWeBroughtThese)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_GUARDING_THE_WINGS,
	 CONDITION_ALWAYS, MSGNO(MsgIDoubtAnyone)},
	{TRIGGER_TALK, ACTOR_BOY_ON_THE_WINGS, FLAG_REUNION_AT_CONTIGO,
	 MSGNO(MsgIWonderIfThese)},
	{TRIGGER_TALK, ACTOR_BOY_ON_THE_WINGS, FLAG_SET_OUT_WITH_WINGS,
	 MSGNO(MsgAMassiveRock)},
	{TRIGGER_TALK, ACTOR_BOY_ON_THE_WINGS, CONDITION_ALWAYS,
	 MSGNO(MsgYouReGoingNorth)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_WINGS,
	 FLAG_REUNION_AT_CONTIGO, MSGNO(MsgItSKind)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_WINGS,
	 FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgIfThatBoat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_WINGS,
	 CONDITION_ALWAYS, MSGNO(MsgIVeDecided)},
	{TRIGGER_TALK, ACTOR_MAN_AWAITING_THE_LEGEND, CONDITION_ALWAYS,
	 MSGNO(MsgItSAlmost)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AWAITING_THE_LEGEND,
	 CONDITION_ALWAYS, MSGNO(MsgTheDayHas)},
	{TRIGGER_TALK, ACTOR_MAN_ON_HAMMAS_REQUEST, CONDITION_ALWAYS,
	 MSGNO(MsgIfEveryoneWill)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_HAMMAS_REQUEST,
	 CONDITION_ALWAYS, MSGNO(MsgTheAnemosWere)},
	{TRIGGER_TALK, ACTOR_MAN_ON_THE_WINGED_SHIP, CONDITION_ALWAYS,
	 MSGNO(MsgWhenTheWinged)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_WINGED_SHIP,
	 CONDITION_ALWAYS, MSGNO(MsgTheseWarriorsWill)},
	{TRIGGER_TALK, ACTOR_MAN_ON_THE_STONE_ANCHORS, CONDITION_ALWAYS,
	 MSGNO(MsgOnceYouRemove)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STONE_ANCHORS,
	 CONDITION_ALWAYS, MSGNO(MsgIHearThatThe)},
	{TRIGGER_TALK, ACTOR_HAMMA, CONDITION_ALWAYS, MSGNO(MsgUseYourPsynergy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMMA, CONDITION_ALWAYS,
	 MSGNO(MsgTravelSafelyThe)},
	{TRIGGER_TABLE_END},
};

/* The heights' exits, the shipwrights' spot, talks, Geode and Cyclone on its */
/* weeds. */
TRIGGER AttekaInlet_HeightsTriggers[] = {
	{TRIGGER_EXIT, ATTEKA_INLET_GEODE_TO_ATTEKA_INLET_SOUTH, CONDITION_ALWAYS,
	 ATTEKA_INLET_GEODE_TO_ATTEKA_INLET_SOUTH},
	{TRIGGER_EXIT, ATTEKA_INLET_GEODE_TO_WORLD_MAP_NORTH, CONDITION_ALWAYS,
	 ATTEKA_INLET_GEODE_TO_WORLD_MAP_NORTH},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH,
	 ATTEKA_INLET_GEODE_TO_CONTIGO_SHOPS, CONDITION_ALWAYS,
	 ATTEKA_INLET_GEODE_TO_CONTIGO_SHOPS},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 4, CONDITION_ALWAYS, 4},
	{TRIGGER_EXIT, ATTEKA_INLET_GEODE_TO_ATTEKA_INLET_NORTH, CONDITION_ALWAYS,
	 ATTEKA_INLET_GEODE_TO_ATTEKA_INLET_NORTH},
	{TRIGGER_EXIT, ATTEKA_INLET_GEODE_TO_WORLD_MAP_SOUTH, CONDITION_ALWAYS,
	 ATTEKA_INLET_GEODE_TO_WORLD_MAP_SOUTH},
	{TRIGGER_STEP, CELL_BEHAVIOR_WAY_WEST, FLAG_SHIPWRIGHTS_STOPPED_PARTY,
	 EVENT(scAttekaInletShipwrightsStopTheParty)},
	{TALK_TRIGGER_FACING(FACING_EAST), ACTOR_GEODE, CONDITION_ALWAYS,
	 EVENT(scAttekaInletGeodeJoins)},
	{TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CALM_SEA, FLAG_SET_OUT_WITH_WINGS,
	 MSGNO(MsgEveryoneSWaiting)},
	{TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CALM_SEA, CONDITION_ALWAYS,
	 MSGNO(MsgAllOfOur)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CALM_SEA,
	 FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgHammaIsThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CALM_SEA,
	 CONDITION_ALWAYS, MSGNO(MsgEveryoneInContigo)},
	{TRIGGER_TALK, ACTOR_SHIPWRIGHT, CONDITION_ALWAYS,
	 MSGNO(MsgTheVillagersAre)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHIPWRIGHT, CONDITION_ALWAYS,
	 MSGNO(MsgItSAmazing)},
	{TRIGGER_TALK, ACTOR_SECOND_SHIPWRIGHT, CONDITION_ALWAYS,
	 MSGNO(MsgHammaWouldnT)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_SHIPWRIGHT,
	 CONDITION_ALWAYS, MSGNO(MsgHammaSaysThat)},
	{TRIGGER_TALK, ACTOR_MAN_WHO_SENT_EVERYONE_BACK, CONDITION_ALWAYS,
	 MSGNO(MsgWhenPeopleWork)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_SENT_EVERYONE_BACK,
	 CONDITION_ALWAYS, MSGNO(MsgWeCanT)},
	{PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_ROUND_BOULDER, CONDITION_ALWAYS, 0},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_GEODES_BUSH, CONDITION_ALWAYS,
	 EVENT(scAttekaInletCycloneUncoversGeode)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_FIRST_WEEDS, CONDITION_ALWAYS,
	 EVENT(scAttekaInletCycloneFirstWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_SECOND_WEEDS, CONDITION_ALWAYS,
	 EVENT(scAttekaInletCycloneSecondWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_THIRD_WEEDS, CONDITION_ALWAYS,
	 EVENT(scAttekaInletCycloneThirdWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_FOURTH_WEEDS, CONDITION_ALWAYS,
	 EVENT(scAttekaInletCycloneFourthWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_FIFTH_WEEDS, CONDITION_ALWAYS,
	 EVENT(scAttekaInletCycloneFifthWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_SIXTH_WEEDS, CONDITION_ALWAYS,
	 EVENT(scAttekaInletCycloneSixthWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_SEVENTH_WEEDS, CONDITION_ALWAYS,
	 EVENT(scAttekaInletCycloneSeventhWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_EIGHTH_WEEDS, CONDITION_ALWAYS,
	 EVENT(scAttekaInletCycloneEighthWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_NINTH_WEEDS, CONDITION_ALWAYS,
	 EVENT(scAttekaInletCycloneNinthWeeds)},
	{TRIGGER_TABLE_END},
};
