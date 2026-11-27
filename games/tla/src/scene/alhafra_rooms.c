/* @file */
/* Inside Alhafra's houses, inn, shop and Sanctum, whose people are taking */
/* in neighbors who lost their homes to the tidal wave. */
/* */
/* The first time Felix walks into the inn's upstairs room he finds Alex */
/* there, also waiting for a boat; in the crowded house an old couple of */
/* houseguests send the lady of the house for tea. After Briggs is caught */
/* Alex asks about it, and once the boat's mast is cleared and Alex has gone, */
/* a game carried over from Golden Sun in which Isaac beat Deadbeard has two */
/* sailors in his room telling how Isaac picked Crossbone Isle clean. The */
/* people's tables change at those two points (FLAG_BRIGGS_CAUGHT and */
/* FLAG_ALHAFRA_MAST_CLEARED). */

#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "game/game.h"
#include "macros.h"
#include "scene/scene.h"
#include "field/event_inline.h"
#include "field/call.h"
#include "version.h"
#include "field/actor_script.h"
#include "shop/shop.h"

/* Talks each person gives once, the first time spoken to. */
#define FLAG_GUESTS_ASKED_FOR_TEA 0x8b1
#define FLAG_HOST_TOLD_OF_REPAIRS 0x8b8

/* Set, once the mast is cleared, when Felix comes into the rooms east of */
/* x 512 on the map; only then are the sailors of the Crossbone Isle tale */
/* placed. */
#define FLAG_ENTERED_IN_THE_EAST FLAG_SCENE_TEMP_00

EFFECT_SPR AlhafraRooms_SecondGuestSprite BSS_DATA ALIGNED(16);
EFFECT_SPR AlhafraRooms_FirstGuestSprite BSS_DATA ALIGNED(16);

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

#include "text/msg_ids.h"

/* Alhafra's houses' people, numbering the rows of all three placement tables:
 */
/* the same row is the same person in each, put out of sight (-56, -56) while */
/* away. The tables part only in their last rows. */
typedef enum ALHAFRA_ROOMS_ACTOR_ID {
	ACTOR_ITEM_SHOPKEEPER = ACTOR_FIRST_PLACED,
	/* The Sanctum's priest, who tells of Garoh's curse. */
	ACTOR_PRIEST,
	/* "I don't care if it WAS broken! The mayor shouldn't have up and sold..."
	 */
	ACTOR_WOMAN_ANGRY_AT_THE_SALE,
	/* "Those guys who bought the boat looked kinda shabby..." */
	ACTOR_WOMAN_ON_THE_SHABBY_BUYERS,
	/* The couple taking in neighbors whose house the tidal wave destroyed, and
	 */
	/* their two guests, the old man who keeps asking for tea and his wife. */
	ACTOR_HOST,
	ACTOR_HOSTESS,
	ACTOR_DEMANDING_GUEST,
	ACTOR_DEMANDING_GUESTS_WIFE,
	ACTOR_INNKEEPER,
	/* "You see how full our inn is? You'd think we'd be making money..." */
	ACTOR_INN_WORKER,
	/* The townsfolk lodged at the inn. */
	/* "The town ought to give money to all the people who suffered..." */
	ACTOR_MAN_ASKING_FOR_MONEY,
	/* "I can't believe the innkeeper is renting out more rooms..." */
	ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS,
	/* "The innkeeper told me to use only the cheapest ingredients..." */
	ACTOR_INN_COOK,
	/* "The innkeeper serves his paying guests better meals than we Alhafrans"
	 */
	ACTOR_MAN_ON_THE_MEALS,
	/* "We're all guests in the same inn, so why is my meal so awful?" */
	ACTOR_WOMAN_ON_THE_MEALS,
	/* "Ohhh... I'm hungry... My tummy hurts..." */
	ACTOR_HUNGRY_CHILD,
	/* "I don't think I've had a bite of meat or fish..." */
	ACTOR_CHILD_MISSING_MEAT,
	/* The family crowded into one room. */
	/* "I'll never forgive the mayor for failing to come to our aid..." */
	ACTOR_MAN_ANGRY_AT_THE_MAYOR,
	/* "Our guests are very upset with the mayor." */
	ACTOR_WOMAN_WORRIED_ABOUT_HER_GUESTS,
	/* "My pop's been lookin' around, an' he says the mayor keeps all his */
	/* treasure..." */
	ACTOR_BOY_ON_THE_MAYORS_TREASURE,
	/* "Why do we all have to be cooped up in such a small room?" */
	ACTOR_CHILD_COOPED_UP,
	/* "We all have to bear the discomfort of this place..." */
	ACTOR_MAN_BEARING_THE_DISCOMFORT,
	/* Who eyes the warrior next door: "...so suave and handsome." */
	ACTOR_WOMAN_EYEING_THE_WARRIOR,
	/* Alex, in the room next door, until the mast is cleared. */
	ACTOR_ALEX
} ALHAFRA_ROOMS_ACTOR_ID;

/* The last rows of AlhafraRooms_ActorPlacementsOnceTheMastIsCleared: */
/* two sailors in Alex's room, one telling the other of Crossbone Isle. */
typedef enum ALHAFRA_ROOMS_SAILORS_ACTOR_ID {
	ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE = ACTOR_ALEX,
	/* "If he's looking for a pirate crew, he can count me in!" */
	ACTOR_SAILOR_EAGER_TO_JOIN
} ALHAFRA_ROOMS_SAILORS_ACTOR_ID;

extern ACTOR_PLACE ActorPlacements[];
extern ACTOR_PLACE AlhafraRooms_ActorPlacementsOnceBriggsIsCaught[];
extern ACTOR_PLACE AlhafraRooms_ActorPlacementsOnceTheMastIsCleared[];
extern TRIGGER AlhafraRooms_Triggers[];
extern TRIGGER AlhafraRooms_TriggersOnceBriggsIsCaught[];
extern TRIGGER AlhafraRooms_TriggersOnceTheMastIsCleared[];

/* The houses' people: before Briggs is caught, after, and once the mast is */
/* cleared. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	if(fdEventGetFlag(FLAG_ALHAFRA_MAST_CLEARED))
		return AlhafraRooms_ActorPlacementsOnceTheMastIsCleared;
	if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT))
		return AlhafraRooms_ActorPlacementsOnceBriggsIsCaught;
	return ActorPlacements;
}

/* The first time Felix walks into the inn's upstairs room (cells (28, 19) */
/* to (28, 21)), he walks up to Alex, who says the town's only boat has been */
/* sold and that he too is resting here while he looks for one. */
void scAlhafraRoomsMeetAlex(void) {
	fdEventSetFlag(FLAG_ALHAFRA_MET_ALEX);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgAlhafraRoomsSoYouveFinallyCome);
	fdCameraMoveToActorById(ACTOR_ALEX, 1);
	fdActorWalkToAndWait(ACTOR_FELIX, 452, 335);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdCameraWaitForArrival();
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
	syFieldEventWait(20);
	/* "So, you've finally come." */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0, 10);
	scFieldActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorSetAttachedEffect(ACTOR_FELIX,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(50);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldEventWait(10);
	/* "I had suspected we might run into each other again, since we both..." */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "Unfortunately, the only boat this town possesses has already been sold."
	 */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
	syFieldEventWait(10);
	/* "I will be resting here in Alhafra a bit longer. I have traveled long..."
	 */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldEventWait(10);
	/* "Perhaps we can get along at least until we both depart." */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0, 10);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH, 0);
	syFieldEventWait(20);
	syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	fdEventEnd();
}

/* Alhafra's shops, as mnShopRun numbers them. */
enum { SHOP_ALHAFRA_ITEMS = 9 };

/* Alhafra's inn, as mnInnCheckIn numbers the inns. */
enum { ALHAFRA_INN = 2 };

/* Felix talks to the item merchant: across his counter, the shop opens; */
/* from anywhere else he asks Felix not to mention the tidal wave. */
void scAlhafraRoomsTalkToItemMerchant(s32 actor) {
	u16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnShopRun(SHOP_ALHAFRA_ITEMS, actor);
	}
	else {
		fdEventSetMessageId((s32)MsgAlhafraRoomsDontMentionTheTidalWave);
		/* "Don't mention the tidal wave to the other merchants... It did a
		 * lot..." */
		fdEventShowMessageAndWait(actor, 0);
	}
}

/* Felix talks to the innkeeper: across his counter, the inn; from anywhere */
/* else he tells of the townsfolk who lost their homes. */
void scAlhafraRoomsTalkToInnkeeper(s32 actor) {
	u16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnInnCheckIn(ALHAFRA_INN, actor);
	}
	else {
		fdEventSetMessageId((s32)MsgAlhafraRoomsMaybeYouveNoticedButA);
		/* "Maybe you've noticed, but a lot of folk here lost their homes in
		 * the..." */
		fdEventShowMessageAndWait(actor, 0);
	}
}

/* Felix talks to the priest: across his counter, the Sanctum; from anywhere */
/* else he tells of a terrible village in the middle of Osenia. */
void scAlhafraRoomsTalkToPriest(s32 actor) {
	u16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnSanctumRun(actor);
	}
	else {
		fdEventSetMessageId((s32)MsgAlhafraRoomsInTheMiddleOfOsenia);
		/* "In the middle of Osenia, there's a terrible village where they..."
		 */
		fdEventShowMessageAndWait(actor, 0);
	}
}

/* Felix talks to the lady of the house, who scowls, worn out by her */
/* houseguests. */
void scAlhafraRoomsTalkToHostess(s32 actor) {
	fdActorShowEmote(actor, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	syFieldEventSetMessageId((s32)MsgAlhafraRoomsAughWithAllTheseDemanding);
	/* "Augh! With all these demanding houseguests, I feel like I've lost..." */
	fdEventShowMessage(actor, 0);
}

/* The first time Felix talks to the old houseguest, or reads his mind, he */
/* steps up to the old couple, who ask the lady of the house for tea; she */
/* tells them to fix it themselves. */
void scAlhafraRoomsGuestsAskForTea(void) {
	fdEventSetFlag(FLAG_GUESTS_ASKED_FOR_TEA);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 152, 328);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	syFieldEventSetMessageId((s32)MsgAlhafraRoomsArentYouThirstyMyDear);
	fdActorFaceDirection(ACTOR_DEMANDING_GUEST, FACING_EAST, 0);
	syFieldEventWait(20);
	/* "Aren't you thirsty, my dear?" */
	syFieldEventShowMessageThenWait(ACTOR_DEMANDING_GUEST, 0, 10);
	fdActorFaceDirection(ACTOR_DEMANDING_GUESTS_WIFE, FACING_WEST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_DEMANDING_GUESTS_WIFE, ANIM_NOD);
	syFieldEventWait(10);
	/* "Oh, you're right, dear. I am!" */
	syFieldEventShowMessageThenWait(ACTOR_DEMANDING_GUESTS_WIFE, 0, 10);
	fdActorSetSpeed(ACTOR_DEMANDING_GUEST, 0.5);
	fdActorWalkByAndWait(ACTOR_DEMANDING_GUEST, -32, 0);
	syFieldEventWait(10);
	/* "Sorry to bother you, but we'd like some tea." */
	syFieldEventShowMessageThenWait(ACTOR_DEMANDING_GUEST, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_HOSTESS, 2);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_HOSTESS, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_HOSTESS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Are you talking to me?!" */
	syFieldEventShowMessageThenWait(ACTOR_HOSTESS, 0, 10);
	scFieldActorStartRepeatedMotion(ACTOR_DEMANDING_GUEST, 3);
	fdActorSetAttachedEffect(ACTOR_DEMANDING_GUEST,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_DEMANDING_GUESTS_WIFE, 3);
	fdActorSetAttachedEffect(ACTOR_DEMANDING_GUESTS_WIFE,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(50);
	fdActorRunRepeatedMotion(ACTOR_HOSTESS, 2);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_HOSTESS, FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(10);
	/* "If you want tea, you can go right ahead and fix it yourself!" */
	syFieldEventShowMessageThenWait(ACTOR_HOSTESS, 0, 10);
	syFieldEventWait(10);
	fdActorWalkToAndWait(ACTOR_DEMANDING_GUEST, 152, 312);
	syFieldEventWait(10);
	fdActorSetAnimation(ACTOR_DEMANDING_GUESTS_WIFE, ANIM_SHAKE_HEAD);
	fdActorSetAnimationAndWait(ACTOR_DEMANDING_GUEST, ANIM_SHAKE_HEAD);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_DEMANDING_GUEST, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_DEMANDING_GUESTS_WIFE, FACING_SOUTH, 0);
	fdEventEnd();
}

/* Felix talks to the old houseguest, who grumbles that the lady of the */
/* house is no lady at all. */
void scAlhafraRoomsTalkToDemandingGuest(s32 actor) {
	fdActorFaceEachOther(actor, MainRoutineStateWork.leaderActorId, 0);
	syFieldEventSetMessageId((s32)MsgAlhafraRoomsWellISayItSeems);
	/* "Well! I say! It seems the lady of the house is no lady at all!" */
	fdEventShowMessage(actor, 0);
	fdActorFaceDirection(ACTOR_DEMANDING_GUEST, FACING_SOUTH_SOUTHEAST, 0);
}

/* Felix casts Mind Read on Alex, who turns to him and asks him to stop. */
void scAlhafraRoomsReadAlexsMind(s32 unused, s32 actor) {
	fdActorFaceEachOther(actor, MainRoutineStateWork.leaderActorId, 0);
	syFieldEventSetMessageId((s32)MsgAlhafraRoomsReallyIDoWishYou);
	/* "Really, I do wish you would quit using your Mind Read on me... Trust..."
	 */
	fdEventShowMessage(actor, 0);
	fdActorFaceDirection(actor, FACING_NORTHEAST, 0);
}

/* The first time Felix talks to the master of the house, he tells that the */
/* town has started repairing the houses, and smiles that his guests will */
/* soon be gone. */
void scAlhafraRoomsHostTellsOfRepairs(s32 actor) {
	fdEventSetFlag(FLAG_HOST_TOLD_OF_REPAIRS);
	syFieldEventSetMessageId((s32)MsgAlhafraRoomsWeveAllStartedWorkingTogether);
	/* "We've all started working together to repair the houses!" */
	fdEventShowMessage(actor, 0);
	syFieldEventWait(10);
	fdActorShowEmote(actor, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Oooh... Not much longer, and then I can kick these folks out of my..."
	 */
	fdEventShowMessage(actor, 0);
}

/* Felix talks to Alex after Briggs is caught, and Alex asks whether it was */
/* he who caught him. */
void scAlhafraRoomsAlexHearsOfBriggs(s32 actor) {
	s32 msg = (s32)MsgAlhafraRoomsSoIHearYouCaught;

	syFieldEventSetMessageId(msg);
	/* "So, I hear you caught Briggs..." */
	/* Yes: "Mia, a young girl from my clan, was very kind, much like you
	 * are..." */
	/* No: "You have no need to hide such fine exploits from me. Working
	 * hard..." */
	fdEventOpenMessage(actor, 0);
	if(syFieldEventAskYesNoWithActorAnimation(syFieldPartyGetLeaderActor(),
											  0) == 0) {
		syFieldEventWait(10);
		syFieldEventSetMessageId(msg + 1);
	}
	else {
		syFieldEventWait(20);
		syFieldEventSetMessageId(msg + 2);
	}
	fdEventShowMessage(actor, 0);
}

/* The houses' triggers. */
const TRIGGER *scGetTriggers(void) {
	if(fdEventGetFlag(FLAG_ALHAFRA_MAST_CLEARED))
		return AlhafraRooms_TriggersOnceTheMastIsCleared;
	if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT))
		return AlhafraRooms_TriggersOnceBriggsIsCaught;
	return AlhafraRooms_Triggers;
}

/* The old houseguest's update: draws his head and upper body again, 12 pixels
 */
/* above him, over the stack of crates he stands in front of; without it the */
/* crates cover him from the waist up and only his legs show. */
s32 scAlhafraRoomsDrawFirstGuestSpr(ACTOR *guest) {
	fdEffectSprDrawAtWorldXYZ(&AlhafraRooms_FirstGuestSprite, guest->x.fixed,
							  guest->y.fixed + toFIXED(12), guest->z.fixed);
	return 0;
}

/* His wife's: the same, 8 pixels above her, so she too shows whole in front of
 */
/* the crates. */
s32 scAlhafraRoomsDrawSecondGuestSpr(ACTOR *guest) {
	fdEffectSprDrawAtWorldXYZ(&AlhafraRooms_SecondGuestSprite, guest->x.fixed,
							  guest->y.fixed + toFIXED(8), guest->z.fixed);
	return 0;
}

/* The Japanese edition sets no FLAG_ENTERED_IN_THE_EAST by the leader's */
/* position and places neither sailor of the Crossbone Isle tale. */
#if TLA_PATCH_1

/* Sets up the houses: the old couple's second sprites, and once the mast is */
/* cleared the two sailors in Alex's room. */
s32 scInit(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdActorGet(ACTOR_INNKEEPER)->collisionFlg |= ACTOR_NEAR_TALK;
	fdActorGet(ACTOR_INNKEEPER)->collisionFlg |= ACTOR_NEAR_PSYNERGY;
	scFieldActorSetSprPriority(ACTOR_PRIEST, 3);
	scFieldActorSetSprPriority(ACTOR_ALEX, 3);
	scFieldActorSetSprPriority(ACTOR_DEMANDING_GUEST, 3);
	scFieldActorSetSprPriority(ACTOR_DEMANDING_GUESTS_WIFE, 3);
	fdFadeOverlayFillTiles(0);
	Call4((void (*)())fdEffectSprInitWithFadeOverlay,
		  (s32)&AlhafraRooms_FirstGuestSprite, 8, 15, OBJ_SIZE_16X16);
	Call4((void (*)())fdEffectSprInitWithFadeOverlay,
		  (s32)&AlhafraRooms_SecondGuestSprite, 8, 15, OBJ_SIZE_16X16);
	AlhafraRooms_SecondGuestSprite.scaleY = FIXED_ONE * 3 / 2;
	fdActorGet(ACTOR_DEMANDING_GUEST)->update = scAlhafraRoomsDrawFirstGuestSpr;
	fdActorGet(ACTOR_DEMANDING_GUESTS_WIFE)->update =
		scAlhafraRoomsDrawSecondGuestSpr;
	if(fdEventGetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
		if(fdActorGet(MainRoutineStateWork.leaderActorId)->x.fixed >=
		   toFIXED(512))
			fdEventSetFlag(FLAG_ENTERED_IN_THE_EAST);
		else
			fdEventClearFlag(FLAG_ENTERED_IN_THE_EAST);
		if(fdEventGetFlag(FLAG_ENTERED_IN_THE_EAST) &&
		   fdEventGetFlag(FLAG_ISAAC_CLEARED_CROSSBONE_ISLE) &&
		   !fdEventGetFlag(FLAG_ALHAFRA_HEARD_OF_CROSSBONE_ISLE)) {
			fdActorSetPos(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 376, 316);
			fdActorSetPos(ACTOR_SAILOR_EAGER_TO_JOIN, 392, 344);
		}
		else {
			fdActorSetPos(ACTOR_ALEX, 0, 0);
			fdActorSetPos(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 0);
		}
	}
	else if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT)) {
		if(fdEventGetFlag(FLAG_CHAMPA_MET_ALEX))
			fdActorSetPos(ACTOR_ALEX, 0, 0);
	}
	else {
		scFieldActorSetSprPriority(ACTOR_CHILD_MISSING_MEAT, 2);
		scFieldActorSetSprPriority(ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS, 3);
		scFieldActorSetSprPriority(ACTOR_CHILD_COOPED_UP, 2);
		scFieldActorSetSprPriority(ACTOR_MAN_ANGRY_AT_THE_MAYOR, 3);
	}
	return 0;
}
#else

/* Sets up the houses and the old couple's second sprites. */
s32 scInit(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdActorGet(ACTOR_INNKEEPER)->collisionFlg |= ACTOR_NEAR_TALK;
	fdActorGet(ACTOR_INNKEEPER)->collisionFlg |= ACTOR_NEAR_PSYNERGY;
	scFieldActorSetSprPriority(ACTOR_PRIEST, 3);
	scFieldActorSetSprPriority(ACTOR_ALEX, 3);
	scFieldActorSetSprPriority(ACTOR_DEMANDING_GUEST, 3);
	scFieldActorSetSprPriority(ACTOR_DEMANDING_GUESTS_WIFE, 3);
	fdFadeOverlayFillTiles(0);
	Call4((void (*)())fdEffectSprInitWithFadeOverlay,
		  (s32)&AlhafraRooms_FirstGuestSprite, 8, 15, OBJ_SIZE_16X16);
	Call4((void (*)())fdEffectSprInitWithFadeOverlay,
		  (s32)&AlhafraRooms_SecondGuestSprite, 8, 15, OBJ_SIZE_16X16);
	AlhafraRooms_SecondGuestSprite.scaleY = FIXED_ONE * 3 / 2;
	fdActorGet(ACTOR_DEMANDING_GUEST)->update = scAlhafraRoomsDrawFirstGuestSpr;
	fdActorGet(ACTOR_DEMANDING_GUESTS_WIFE)->update =
		scAlhafraRoomsDrawSecondGuestSpr;
	if(fdEventGetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
		if(!fdEventGetFlag(FLAG_ISAAC_CLEARED_CROSSBONE_ISLE) ||
		   fdEventGetFlag(FLAG_ALHAFRA_HEARD_OF_CROSSBONE_ISLE)) {
			fdActorSetPos(ACTOR_ALEX, 0, 0);
			fdActorSetPos(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 0);
		}
	}
	else if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT)) {
		if(fdEventGetFlag(FLAG_CHAMPA_MET_ALEX))
			fdActorSetPos(ACTOR_ALEX, 0, 0);
	}
	else {
		scFieldActorSetSprPriority(ACTOR_CHILD_MISSING_MEAT, 2);
		scFieldActorSetSprPriority(ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS, 3);
		scFieldActorSetSprPriority(ACTOR_CHILD_COOPED_UP, 2);
		scFieldActorSetSprPriority(ACTOR_MAN_ANGRY_AT_THE_MAYOR, 3);
	}
	return 0;
}
#endif

s32 scPrepareMap(void) { return 0; }

extern u32 AlhafraRooms_TravelersWalkOffScript[];

/* In a game carried over from Golden Sun in which Isaac beat Deadbeard, Felix
 */
/* walks into Alex's old room upstairs in the inn after the mast is cleared */
/* (cells (31, 19) to (31, 22)) and overhears two sailors: Isaac picked */
/* Crossbone Isle clean and beat its ghost pirate, so he is their leader now. */
/* They walk out past Felix, and it happens once. */
/* */
/* It runs while FLAG_ISAAC_CLEARED_CROSSBONE_ISLE is set and sets */
/* FLAG_ALHAFRA_HEARD_OF_CROSSBONE_ISLE; outside the Japanese edition, only */
/* when Felix came in from the east of the map (FLAG_ENTERED_IN_THE_EAST). */
void scAlhafraRoomsOverhearCrossboneIsleTale(void) {
	if(fdEventGetFlag(FLAG_ALHAFRA_HEARD_OF_CROSSBONE_ISLE))
		return;
#if TLA_PATCH_1 /* The Japanese edition does not check                         \
				   FLAG_ENTERED_IN_THE_EAST. */
	if(!fdEventGetFlag(FLAG_ENTERED_IN_THE_EAST))
		return;
#endif
	fdEventSetFlag(FLAG_ALHAFRA_HEARD_OF_CROSSBONE_ISLE);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgAlhafraRoomsLegendsSaidThatCrossboneIsle);
	fdActorWalkToAndWait(ACTOR_FELIX, 504, 328);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_WEST);
	fdCameraMoveTo(408, CAMERA_KEEP, 328, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 2);
	syFieldEventWait(5);
	/* "Legends said that Crossbone Isle could be found in the middle of..." */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 0,
									5);
	fdActorShowEmote(ACTOR_SAILOR_EAGER_TO_JOIN,
					 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Ah... Crossbone Isle... Wait, did you say "could"? Is it not there..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 5);
	fdActorShowEmote(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE,
					 EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Well, it's still there, but there's no point in going. Someone..." */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 0,
									5);
	fdActorShowEmote(ACTOR_SAILOR_EAGER_TO_JOIN, EMOTE_IN_FRONT | EMOTE_WINCE,
					 30);
	/* "What, he took all the treasure?" */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE,
							   ANIM_NOD);
	syFieldEventWait(10);
	/* "Yeah. And apparently, he even defeated the ancient ghost pirate..." */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 0,
									5);
	scFieldActorJump(ACTOR_SAILOR_EAGER_TO_JOIN, 6, 15);
	scFieldActorJump(ACTOR_SAILOR_EAGER_TO_JOIN, 6, 23);
	/* "What about the stories that if you beat the ghost pirate, you..." */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE,
							   ANIM_NOD);
	syFieldEventWait(10);
	/* "Well, you see, that means that Isaac is our leader now." */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 0,
									5);
	fdActorShowEmote(ACTOR_SAILOR_EAGER_TO_JOIN,
					 EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Wow... just like that? I'll bet he's pretty tough." */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 2);
	syFieldEventWait(5);
	/* "No doubt. But you know, he must be a reliable guy, too..." */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 0,
									5);
	fdActorSetAnimationAndWait(ACTOR_SAILOR_EAGER_TO_JOIN, ANIM_NOD);
	syFieldEventWait(10);
	/* "You think?" */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE,
								FACING_NORTH_NORTHEAST);
	syFieldEventWait(15);
	/* "You'd have to be to beat that ghost. That's why we're looking for..." */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 0,
									5);
	fdActorSetAnimationAndWait(ACTOR_SAILOR_EAGER_TO_JOIN, ANIM_NOD);
	syFieldEventWait(10);
	/* "If he's looking for a pirate crew, he can count me in!" */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE,
								FACING_SOUTH_SOUTHEAST);
	syFieldEventWait(15);
	fdActorSetAnimationAndWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE,
							   ANIM_NOD);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_SAILOR_EAGER_TO_JOIN, ANIM_NOD);
	syFieldEventWait(20);
	fdActorSetSpeed(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 1);
	fdActorSetSpeed(ACTOR_SAILOR_EAGER_TO_JOIN, 1);
	scFieldActorSetSprPriority(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 2);
	scFieldActorSetSprPriority(ACTOR_SAILOR_EAGER_TO_JOIN, 2);
	syEventCameraFollowActor(ACTOR_SAILOR_EAGER_TO_JOIN, 1);
	fdActorWalkTo(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 394, 342);
	syFieldEventWait(10);
	fdActorWalkTo(ACTOR_SAILOR_EAGER_TO_JOIN, 450, 346);
	scFieldActorWaitForMoveById(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE);
	fdActorWalkTo(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 450, 346);
	scFieldActorWaitForMoveById(ACTOR_SAILOR_EAGER_TO_JOIN);
	fdActorWalkTo(ACTOR_SAILOR_EAGER_TO_JOIN, 470, 328);
	scFieldActorWaitForMoveById(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE);
	fdActorWalkTo(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 470, 328);
	scFieldActorWaitForMoveById(ACTOR_SAILOR_EAGER_TO_JOIN);
	fdActorWalkTo(ACTOR_SAILOR_EAGER_TO_JOIN, 486, 328);
	scFieldActorWaitForMoveById(ACTOR_SAILOR_EAGER_TO_JOIN);
	fdActorSetAnimation(ACTOR_SAILOR_EAGER_TO_JOIN, ANIM_STAND);
	fdActorFaceDirectionAndWait(ACTOR_SAILOR_EAGER_TO_JOIN, FACING_EAST);
	scFieldActorWaitForMoveById(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE);
	fdActorSetAnimation(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, ANIM_STAND);
	fdActorFaceDirectionAndWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE,
								FACING_EAST);
	fdActorWalkByAndWait(ACTOR_FELIX, 0, -16);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
	syFieldEventWait(10);
	fdActorSetScript(ACTOR_SAILOR_EAGER_TO_JOIN,
					 AlhafraRooms_TravelersWalkOffScript);
	syFieldEventWait(10);
	fdActorSetScript(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE,
					 AlhafraRooms_TravelersWalkOffScript);
	syFieldEventWait(10);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_SAILOR_EAGER_TO_JOIN);
	syFieldEventWait(60);
	scFieldActorSetSprPriority(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 3);
	scFieldActorSetSprPriority(ACTOR_SAILOR_EAGER_TO_JOIN, 3);
	scFieldActorWaitForScriptEndById(ACTOR_SAILOR_EAGER_TO_JOIN);
	scFieldActorStop(ACTOR_FELIX);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorSetPos(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 0);
	fdEventPlaySnd(SE_MAP_EXIT);
	scFieldActorWaitForScriptEndById(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE);
	fdActorSetPos(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 0, 0);
	fdEventPlaySnd(SE_MAP_EXIT);
	syFieldEventWait(10);
	fdEventEnd();
}

/* clang-format off */
u32 AlhafraRooms_TravelersWalkOffScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(520, 0, 328),
    SC_SYNC,
    SC_MOVE(520, 0, 312),
    SC_SYNC,
    SC_MOVE(552, 0, 312),
    SC_SYNC,
    SC_END,
};

#include "shared/scene_entrances_default.c"

u32 SceneExits[] = {
    SCENE_EXITS(AlhafraRooms),
    SCENE_EXIT_UNLESS(1, Alhafra, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Alhafra, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Alhafra, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, Alhafra, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, Alhafra, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(20, AlhafraRooms, 20, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(21, AlhafraRooms, 21, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(22, AlhafraRooms, 22, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(23, AlhafraRooms, 23, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(24, AlhafraRooms, 24, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(25, AlhafraRooms, 25, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 184, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 344, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 312, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 312, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 84, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 152, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 120, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 280, FACING_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_COOK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 762, 0, 84, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 864, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 136, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 152, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 112, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 520, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 536, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 552, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 544, 0, 544, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 344, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 360, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 312, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE AlhafraRooms_ActorPlacementsOnceBriggsIsCaught[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 184, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 344, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 312, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 312, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 84, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 152, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 120, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 864, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_COOK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 762, 0, 84, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 136, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 512, 0, 536, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 552, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 312, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE AlhafraRooms_ActorPlacementsOnceTheMastIsCleared[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 184, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 344, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 312, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 312, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 312, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 84, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 152, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 120, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 864, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_COOK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 762, 0, 84, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 136, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 512, 0, 536, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 552, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 316, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 344, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};
/* clang-format on */

TRIGGER AlhafraRooms_Triggers[] = {
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 1, CONDITION_ALWAYS, 1},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 2, CONDITION_ALWAYS, 2},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 5, CONDITION_ALWAYS, 5},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 6, CONDITION_ALWAYS, 6},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 7, CONDITION_ALWAYS, 7},
	{TRIGGER_EXIT, 20, CONDITION_ALWAYS, 20},
	{TRIGGER_EXIT, 21, CONDITION_ALWAYS, 21},
	{TRIGGER_EXIT, 22, CONDITION_ALWAYS, 22},
	{TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
	{TRIGGER_EXIT, 24, CONDITION_ALWAYS, 24},
	{TRIGGER_EXIT, 25, CONDITION_ALWAYS, 25},
	{TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
	 EVENT(scAlhafraRoomsTalkToItemMerchant)},
	{TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_INNKEEPER, CONDITION_ALWAYS,
	 EVENT(scAlhafraRoomsTalkToInnkeeper)},
	{TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_PRIEST, CONDITION_ALWAYS,
	 EVENT(scAlhafraRoomsTalkToPriest)},
	{EXAMINE_TRIGGER(EXAMINE_OVEN), 214, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgAlhafraRoomsSeasonalVeggiesBoilingWithSome))},
	{EXAMINE_TRIGGER(EXAMINE_OVEN), 215, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgAlhafraRoomsThereIsASeafoodPie))},
	{EXAMINE_TRIGGER(EXAMINE_OVEN), 216, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgAlhafraRoomsItLooksLikeSomeKind))},
	{EXAMINE_TRIGGER(EXAMINE_OVEN), 217, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgAlhafraRoomsItsAlhafrasFamousBroiledLobsterits))},
	{EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgAlhafraRoomsThatSeafoodStewHasAn))},
	{TRIGGER_STEP, 30, FLAG_ALHAFRA_MET_ALEX, EVENT(scAlhafraRoomsMeetAlex)},
	{TRIGGER_TALK, ACTOR_WOMAN_ANGRY_AT_THE_SALE, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsIDontCareIfIt)},
	{TRIGGER_TALK, ACTOR_WOMAN_ON_THE_SHABBY_BUYERS, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsThoseGuysWhoBoughtThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ANGRY_AT_THE_SALE,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsIWonderIfTheMayor)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_SHABBY_BUYERS,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsWhyWouldAnyonePaySo)},
	{TRIGGER_TALK, ACTOR_DEMANDING_GUEST, FLAG_GUESTS_ASKED_FOR_TEA,
	 EVENT(scAlhafraRoomsGuestsAskForTea)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_DEMANDING_GUEST | TRIGGER_SUBJECT_OWN_EFFECT,
	 FLAG_GUESTS_ASKED_FOR_TEA, EVENT(scAlhafraRoomsGuestsAskForTea)},
	{TRIGGER_TALK, ACTOR_HOST, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsWereTakingCareOfSome)},
	{TRIGGER_TALK, ACTOR_HOSTESS, CONDITION_ALWAYS,
	 EVENT(scAlhafraRoomsTalkToHostess)},
	{TRIGGER_TALK, ACTOR_DEMANDING_GUEST, CONDITION_ALWAYS,
	 EVENT(scAlhafraRoomsTalkToDemandingGuest)},
	{TRIGGER_TALK, ACTOR_DEMANDING_GUESTS_WIFE, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsWhenWeLostOurHouse)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOST, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsIJustWishThatOld)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOSTESS, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsIDontMindHelpingPeople)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DEMANDING_GUEST,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsAllIAskIsFor)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DEMANDING_GUESTS_WIFE,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsIsThisHowAYoung)},
	{TRIGGER_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsDontMentionTheTidalWave)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsIfAllMyGoodsWere)},
	{TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsMaybeYouveNoticedButA)},
	{TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsYouSeeHowFullOur)},
	{TRIGGER_TALK, ACTOR_MAN_ASKING_FOR_MONEY, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsTheTownOughtToGive)},
	{TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsICantBelieveTheInnkeeper)},
	{TRIGGER_TALK, ACTOR_INN_COOK, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsTheInnkeeperToldMeTo)},
	{TRIGGER_TALK, ACTOR_MAN_ON_THE_MEALS, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsTheInnkeeperServesHisPaying)},
	{TRIGGER_TALK, ACTOR_WOMAN_ON_THE_MEALS, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsWereAllGuestsInThe)},
	{TRIGGER_TALK, ACTOR_HUNGRY_CHILD, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsOhhhImHungryMyTummy)},
	{TRIGGER_TALK, ACTOR_CHILD_MISSING_MEAT, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsIDontThinkIveHad)},
	{TRIGGER_TALK, ACTOR_MAN_ANGRY_AT_THE_MAYOR, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsIllNeverForgiveTheMayor)},
	{TRIGGER_TALK, ACTOR_WOMAN_WORRIED_ABOUT_HER_GUESTS, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsOurGuestsAreVeryUpset)},
	{TRIGGER_TALK, ACTOR_BOY_ON_THE_MAYORS_TREASURE, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsMyPopsBeenLookinAround)},
	{TRIGGER_TALK, ACTOR_CHILD_COOPED_UP, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsWhyDoWeAllHave)},
	{TRIGGER_TALK, ACTOR_MAN_BEARING_THE_DISCOMFORT, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsWeAllHaveToBear)},
	{TRIGGER_TALK, ACTOR_WOMAN_EYEING_THE_WARRIOR, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsIHeardTheresOnlyOne)},
	{TRIGGER_TALK, ACTOR_ALEX, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsTheBrokenMastOnThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsINeverImaginedAnyoneFrom)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsPerhapsTheMayorWillCover)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ASKING_FOR_MONEY,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsIBetTheMasterOf)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsICantBelieveTheInnkeeperIs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_COOK, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsICantBelieveImOnly)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_MEALS,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsMaybeItsCauseAllWeve)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_MEALS,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsIThinkIveLostWeight)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_CHILD, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsMaybeImJustHungryBut)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_MISSING_MEAT,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsSniffSniffThatSmellThats)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ANGRY_AT_THE_MAYOR,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsIHopeTheMayorsTreasure)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WORRIED_ABOUT_HER_GUESTS,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsIKnowOurMayorsBad)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_MAYORS_TREASURE,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsEveryoneSaysTheHillBeneath)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_COOPED_UP,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsIThinkTheyPutMe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_BEARING_THE_DISCOMFORT,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsWhenYoureLivingPackedIn)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_EYEING_THE_WARRIOR,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsThatWarriorNextDoorIs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_ALEX | TRIGGER_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS,
	 EVENT(scAlhafraRoomsReadAlexsMind)},
	{TRIGGER_TALK, ACTOR_PRIEST, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsInTheMiddleOfOsenia)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRIEST, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsThePeopleOfGarohBrought)},
	{TRIGGER_TABLE_END},
};

TRIGGER AlhafraRooms_TriggersOnceBriggsIsCaught[] = {
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 1, CONDITION_ALWAYS, 1},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 2, CONDITION_ALWAYS, 2},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 5, CONDITION_ALWAYS, 5},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 6, CONDITION_ALWAYS, 6},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 7, CONDITION_ALWAYS, 7},
	{TRIGGER_EXIT, 20, CONDITION_ALWAYS, 20},
	{TRIGGER_EXIT, 21, CONDITION_ALWAYS, 21},
	{TRIGGER_EXIT, 22, CONDITION_ALWAYS, 22},
	{TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
	{TRIGGER_EXIT, 24, CONDITION_ALWAYS, 24},
	{TRIGGER_EXIT, 25, CONDITION_ALWAYS, 25},
	{TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
	 EVENT(scAlhafraRoomsTalkToItemMerchant)},
	{TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_INNKEEPER, CONDITION_ALWAYS,
	 EVENT(scAlhafraRoomsTalkToInnkeeper)},
	{TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_PRIEST, CONDITION_ALWAYS,
	 EVENT(scAlhafraRoomsTalkToPriest)},
	{EXAMINE_TRIGGER(EXAMINE_OVEN), 214, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgAlhafraRoomsSeasonalVeggiesBoilingWithSome))},
	{EXAMINE_TRIGGER(EXAMINE_OVEN), 215, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgAlhafraRoomsThereIsASeafoodPie))},
	{EXAMINE_TRIGGER(EXAMINE_OVEN), 216, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgAlhafraRoomsItLooksLikeSomeKind))},
	{EXAMINE_TRIGGER(EXAMINE_OVEN), 217, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgAlhafraRoomsItsAlhafrasFamousBroiledLobsterits))},
	{EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgAlhafraRoomsThatSeafoodStewHasAn))},
	{TRIGGER_TALK, ACTOR_WOMAN_EYEING_THE_WARRIOR,
	 IF_FLAG_SET(FLAG_CHAMPA_MET_ALEX),
	 MSGNO(MsgAlhafraRoomsThatCuteBoyNextDoor)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_EYEING_THE_WARRIOR,
	 IF_FLAG_SET(FLAG_CHAMPA_MET_ALEX),
	 MSGNO(MsgAlhafraRoomsTheFellowNextDoorWas)},
	{TRIGGER_TALK, ACTOR_WOMAN_ANGRY_AT_THE_SALE, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsImAmazedThoseGuysWho)},
	{TRIGGER_TALK, ACTOR_WOMAN_ON_THE_SHABBY_BUYERS, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsIThoughtItWasWeird)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ANGRY_AT_THE_SALE,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsPiratesHereInAlhafraThats)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_SHABBY_BUYERS,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsThoseGuysWereJustTrying)},
	{TRIGGER_TALK, ACTOR_HOST, FLAG_HOST_TOLD_OF_REPAIRS,
	 EVENT(scAlhafraRoomsHostTellsOfRepairs)},
	{TRIGGER_TALK, ACTOR_HOST, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsOoohNotMuchLongerAnd)},
	{TRIGGER_TALK, ACTOR_HOSTESS, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsOnceMyOldNeighborsHouse)},
	{TRIGGER_TALK, ACTOR_DEMANDING_GUEST, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsOnceEveryoneStartedWorkingTo)},
	{TRIGGER_TALK, ACTOR_DEMANDING_GUESTS_WIFE, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsIWonderWhatHappenedThats)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOST, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsAhhItsSoRelievingTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOSTESS, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsItWontBeLongUntil)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DEMANDING_GUEST,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsTheLadyOfThisHouse)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DEMANDING_GUESTS_WIFE,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsTheLadyOfTheHouse)},
	{TRIGGER_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsTheMadransDroppedInAnd)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsTheMadransMustHaveBeen)},
	{TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsAsSoonAsImDone)},
	{TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsWereAllTakingTurnsHelping)},
	{TRIGGER_TALK, ACTOR_MAN_ASKING_FOR_MONEY, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsEveryonesWorkingSoHardTo)},
	{TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsTheresStillPlentyOfWork)},
	{TRIGGER_TALK, ACTOR_INN_COOK, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsTonightsMealIsChockFull)},
	{TRIGGER_TALK, ACTOR_CHILD_MISSING_MEAT, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsMomSaidWereGonnaHave)},
	{TRIGGER_TALK, ACTOR_MAN_BEARING_THE_DISCOMFORT, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsMySonHasTerribleLuck)},
	{TRIGGER_TALK, ACTOR_WOMAN_EYEING_THE_WARRIOR, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsTheGuyNextToUs)},
	{TRIGGER_TALK, ACTOR_ALEX, CONDITION_ALWAYS,
	 EVENT(scAlhafraRoomsAlexHearsOfBriggs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsWhyDoIHaveTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsIMayNotLookVery)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ASKING_FOR_MONEY,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsWeHeardALoudPopping)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsIWonderWhatOurNew)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_COOK, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsIWasToldToUse)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_MISSING_MEAT,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsOhIfWeGetA)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_BEARING_THE_DISCOMFORT,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsIGuessIJustDont)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_EYEING_THE_WARRIOR,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsOurNeighborIsSoMellow)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALEX, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsFeelFreeToHelpWhomever)},
	{TRIGGER_TALK, ACTOR_PRIEST, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsCapturingTheDreadedBriggsWas)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRIEST, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsIThinkThisManMight)},
	{TRIGGER_TABLE_END},
};

TRIGGER AlhafraRooms_TriggersOnceTheMastIsCleared[] = {
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 1, CONDITION_ALWAYS, 1},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 2, CONDITION_ALWAYS, 2},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 5, CONDITION_ALWAYS, 5},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 6, CONDITION_ALWAYS, 6},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 7, CONDITION_ALWAYS, 7},
	{TRIGGER_EXIT, 20, CONDITION_ALWAYS, 20},
	{TRIGGER_EXIT, 21, CONDITION_ALWAYS, 21},
	{TRIGGER_EXIT, 22, CONDITION_ALWAYS, 22},
	{TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
	{TRIGGER_EXIT, 24, CONDITION_ALWAYS, 24},
	{TRIGGER_EXIT, 25, CONDITION_ALWAYS, 25},
	{TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
	 EVENT(scAlhafraRoomsTalkToItemMerchant)},
	{TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_INNKEEPER, CONDITION_ALWAYS,
	 EVENT(scAlhafraRoomsTalkToInnkeeper)},
	{TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_PRIEST, CONDITION_ALWAYS,
	 EVENT(scAlhafraRoomsTalkToPriest)},
	{EXAMINE_TRIGGER(EXAMINE_OVEN), 214, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgAlhafraRoomsSeasonalVeggiesBoilingWithSome))},
	{EXAMINE_TRIGGER(EXAMINE_OVEN), 215, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgAlhafraRoomsThereIsASeafoodPie))},
	{EXAMINE_TRIGGER(EXAMINE_OVEN), 216, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgAlhafraRoomsItLooksLikeSomeKind))},
	{EXAMINE_TRIGGER(EXAMINE_OVEN), 217, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgAlhafraRoomsItsAlhafrasFamousBroiledLobsterits))},
	{EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgAlhafraRoomsThatSeafoodStewHasAn))},
	{TRIGGER_STEP, 31, IF_FLAG_SET(FLAG_ISAAC_CLEARED_CROSSBONE_ISLE),
	 EVENT(scAlhafraRoomsOverhearCrossboneIsleTale)},
	{TRIGGER_TALK, ACTOR_WOMAN_ANGRY_AT_THE_SALE, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsIDontWantToSay)},
	{TRIGGER_TALK, ACTOR_WOMAN_ON_THE_SHABBY_BUYERS, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsBriggsDidntCommitAnyCrimes)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ANGRY_AT_THE_SALE,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsBriggsPaidForTheBoat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_SHABBY_BUYERS,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsIDontBelieveForOne)},
	{TRIGGER_TALK, ACTOR_HOST, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsFixingThatOldGuysHouse)},
	{TRIGGER_TALK, ACTOR_HOSTESS, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsTheRepairsOnOurNeighbors)},
	{TRIGGER_TALK, ACTOR_DEMANDING_GUEST, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsMyHouseIsDamagedMore)},
	{TRIGGER_TALK, ACTOR_DEMANDING_GUESTS_WIFE, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsMyHusbandAndIDont)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOST, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsWillTheyEverGetOut)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOSTESS, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsIDontHaveAnythingTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DEMANDING_GUEST,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsIDontMindNotBeing)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DEMANDING_GUESTS_WIFE,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsIHatedThisPlaceAt)},
	{TRIGGER_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsTheBridgeToTheWest)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsPeopleWillStartTravelingAgain)},
	{TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsWereStillRecoveringFromThe)},
	{TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsTheMayorUsedTheMoney)},
	{TRIGGER_TALK, ACTOR_MAN_ASKING_FOR_MONEY, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsItSeemsThatBriggsBroke)},
	{TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsBriggsMightHaveHadA)},
	{TRIGGER_TALK, ACTOR_INN_COOK, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsABiggerBoatCouldHaul)},
	{TRIGGER_TALK, ACTOR_CHILD_MISSING_MEAT, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsIJustWantAHouse)},
	{TRIGGER_TALK, ACTOR_MAN_BEARING_THE_DISCOMFORT, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsSomeoneSaidMySonAnd)},
	{TRIGGER_TALK, ACTOR_WOMAN_EYEING_THE_WARRIOR, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsSoThatAlexFellowLeft)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsAfterTheTidalWaveIt)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsTheMayorMayHavePulled)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ASKING_FOR_MONEY,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsBriggsAndHisWifeWere)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsIfIHadAKey)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_COOK, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsSoWhatIfTradeBrings)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_MISSING_MEAT,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsIdReallyLikeToHave)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_BEARING_THE_DISCOMFORT,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsIsntAnyoneHelpingMyFamily)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_EYEING_THE_WARRIOR,
	 CONDITION_ALWAYS, MSGNO(MsgAlhafraRoomsHeWasJustMyType)},
	{TRIGGER_TALK, ACTOR_PRIEST, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsIHeardYouWereThere)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRIEST, CONDITION_ALWAYS,
	 MSGNO(MsgAlhafraRoomsTheyWereSoConfidentWhen)},
	{TRIGGER_TABLE_END},
};
