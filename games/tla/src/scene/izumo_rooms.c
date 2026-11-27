/* @file */
/* Inside Izumo's houses on stilts, one room each: two houses in the east, the
 */
/* inn, the shop, Kushinada's house, the Sanctum, and the hall of Susa's house
 */
/* in the north, which has a cast and a story of its own. */
/* */
/* Before the Serpent is slain the village mourns: Kushinada sits on her seat */
/* of stones and, taking Felix for Susa, begs him not to come for her, and in */
/* Susa's house Lady Uzume says he has gone to fight the Serpent. Once it is */
/* slain the party's first visit to Susa's house becomes the heroes' welcome */
/* (scIzumoRoomsRecognizeTheHeroes): Lady Uzume and Kushinada learn who slew */
/* it, Susa comes home, and Kraden explains Psynergy. Afterwards Lady Uzume */
/* takes the Dancing Idol and gives the party Coal, the Mars Djinni */
/* (scIzumoRoomsGiveUzumeTheDoll). */

#include "item_ids.h"
#include "field/event_inline.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "field/actor_script.h"
#include "shop/shop.h"
#include "text/msg_ids.h"

/* Story flags: Lady Uzume has told the party where Susa went; her prayer for */
/* him has been read with Mind Read; Kushinada has begged "Susa" not to come */
/* for her; and Lady Uzume has caught the party reading her mind ("Trying */
/* again, Felix?"). */
#define FLAG_UZUME_SPOKE_OF_SUSA			  0x922
#define FLAG_UZUMES_PRAYER_READ				  0x923
#define FLAG_KUSHINADA_ASKED_SUSA_NOT_TO_COME 0x925
#define FLAG_UZUME_MIND_READ				  0x92b
/* Set as Susa's house welcomes the heroes (and again as Lady Uzume takes the */
/* Dancing Idol), and cleared on entering any other room: while it is set, the
 */
/* two women in red and the old men by the drums stay off stage. */
#define FLAG_HEROES_JUST_RECOGNIZED FLAG_AREA_TEMP_50

/* The cells across the hall of Susa's house, on its east and west sides, */
/* where the heroes' welcome starts. */
enum { CELL_EAST_SIDE_OF_THE_HALL = 22, CELL_WEST_SIDE_OF_THE_HALL };

extern ACTOR_PLACE IzumoRooms_ActorPlacements[];
extern ACTOR_PLACE IzumoRooms_SusasHouseActorPlacements[];
extern ACTOR_PLACE IzumoRooms_ActorPlacementsOnceTheSerpentIsSlain[];
extern TRIGGER IzumoRooms_Triggers[];
extern TRIGGER IzumoRooms_SusasHouseTriggers[];
extern TRIGGER IzumoRooms_TriggersOnceTheSerpentIsSlain[];

/* The side of the hall of Susa's house Felix came up for the heroes' welcome.
 */
enum { HEROES_FROM_THE_EAST, HEROES_FROM_THE_WEST };

void scIzumoRoomsRecognizeTheHeroes(s32 side);

void scIzumoRoomsShowMove(void);
extern s32 IzumoRooms_FirstDancerScript[];
extern s32 IzumoRooms_SecondDancerScript[];
extern s32 IzumoRooms_HeavingScript[];

/* Whether the other of the two women dancing in Susa's house is clear of the */
/* spot by the doorway this one steps to; each one's dance waits on it. */
#define DEFINE_IZUMO_ROOMS_IS_CLEAR(name, n1, n2)                              \
	s32 name(void) {                                                           \
		ACTOR *actor = fdActorGetById(n1);                                     \
                                                                               \
		if((u32)(actor->x.fixed - n2) <= toFIXED(8)) {                         \
			s32 z = actor->z.fixed;                                            \
                                                                               \
			if(z >= toFIXED(620)) {                                            \
				if(z <= toFIXED(628))                                          \
					return 0;                                                  \
			}                                                                  \
		}                                                                      \
		return 1;                                                              \
	}

/* Whether the second dancer is clear of the spot the first steps to; the first
 */
/* dancer's script waits on it. */
DEFINE_IZUMO_ROOMS_IS_CLEAR(IzumoRooms_IsFirstDancerClear, 10, toFIXED(148))

/* Whether the first dancer is clear of the spot the second steps to; the */
/* second dancer's script waits on it. */
DEFINE_IZUMO_ROOMS_IS_CLEAR(IzumoRooms_IsSecondDancerClear, 9, toFIXED(132))

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	if(syGameFlagGet(FLAG_SERPENT_SLAIN))
		return IzumoRooms_ActorPlacementsOnceTheSerpentIsSlain;
	if(MainRoutineStateWork.entrance == IZUMO_ROOMS_FROM_IZUMO_NORTH)
		return IzumoRooms_SusasHouseActorPlacements;
	return IzumoRooms_ActorPlacements;
}

const TRIGGER *scGetTriggers(void) {
	if(syGameFlagGet(FLAG_SERPENT_SLAIN))
		return IzumoRooms_TriggersOnceTheSerpentIsSlain;
	if(MainRoutineStateWork.entrance == IZUMO_ROOMS_FROM_IZUMO_NORTH)
		return IzumoRooms_SusasHouseTriggers;
	return IzumoRooms_Triggers;
}

/* Before the Serpent is slain: Kushinada, whose turn it is to be offered to */
/* it, and the Sanctum's priest ("Why would our guardian spirit demand a */
/* sacrifice?"), who stands behind the same counter once it is slain. */
enum { ACTOR_KUSHINADA = 17, ACTOR_PRIEST = 22, ACTOR_PRIEST_ONCE_SLAIN = 12 };

/* Susa's house before the Serpent is slain: Lady Uzume, Susa's sister ("If */
/* the serpent defeats Susa, I'll...be alone."), with two women dancing */
/* ("There's no point dancing if Okuni's not here...") and two old men by the */
/* drums. */
enum {
	ACTOR_SUSAS_HOUSE_UZUME = 8,
	ACTOR_SUSAS_HOUSE_FIRST_DANCER,
	ACTOR_SUSAS_HOUSE_SECOND_DANCER,
	ACTOR_SUSAS_HOUSE_WEST_VILLAGER,
	ACTOR_SUSAS_HOUSE_EAST_VILLAGER
};

/* Once the Serpent is slain, Susa's house holds Lady Uzume ("This is my home,
 */
/* and I am having a private conversation."), the same four, Kushinada, Susa, */
/* the villager who saw the heroes come down the mountain ("Yes... After the */
/* serpent's roar, these warriors appeared."), the party for the welcome, and */
/* Coal. */
enum {
	ACTOR_UZUME = 15,
	ACTOR_FIRST_DANCER,
	ACTOR_SECOND_DANCER,
	ACTOR_WEST_VILLAGER,
	ACTOR_EAST_VILLAGER,
	ACTOR_SAVED_KUSHINADA,
	ACTOR_SUSA,
	ACTOR_WITNESS,
	ACTOR_KRADEN,
	ACTOR_COAL /* the Mars Djinni Uzume gives the party */
};

/* Talking to the priest in the Sanctum across his counter opens the Sanctum's
 */
/* healing. From any other side he asks why the guardian spirit would demand a
 */
/* sacrifice, or once the Serpent is slain says the island is flourishing. */
void scIzumoRoomsTalkToPriest(s32 actor) {
	u16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnSanctumRun(actor);
	}
	else {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		if(syGameFlagGet(FLAG_SERPENT_SLAIN)) {
			fdEventSetMessageId((s32)MsgNowThatPeace);
			/* "Now that peace has been restored, our island is flourishing
			 * once..." */
			fdEventShowMessageAndWait(ACTOR_PRIEST_ONCE_SLAIN, 0);
		}
		else {
			fdEventSetMessageId((s32)MsgWhyWouldOur);
			/* "Why would our guardian spirit demand a sacrifice? It's..." */
			fdEventShowMessageAndWait(ACTOR_PRIEST, 0);
		}
		fdEventEnd();
	}
}

/* The fifth animation of these villagers' sprites, as Kushinada's room and */
/* Susa's house show them before the Serpent is slain: Kushinada kneeling on */
/* her seat, the women in red dancing with their arms out, and the old men */
/* beating the drums. */
enum { ANIM_KNEEL = 5, ANIM_DANCE = 5, ANIM_BEAT_DRUM = 5 };

/* Izumo's inn, as mnInnCheckIn numbers the inns. */
enum { IZUMO_INN = 9 };

/* Talking to the innkeeper across his counter offers a night at Izumo's inn. */
/* From any other side he says he never believed the legend of the Serpent, */
/* or once it is slain that Briggs from Champa hasn't been by selling fish. */
void scIzumoRoomsTalkToInnkeeper(s32 actor) {
	u16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnInnCheckIn(IZUMO_INN, actor);
	}
	else {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		if(syGameFlagGet(FLAG_SERPENT_SLAIN))
			fdEventSetMessageId((s32)MsgBriggsFromChampa);
		else
			fdEventSetMessageId((s32)MsgOfCourseI);
		/* "Of course, I knew the legend of the great serpent... Everyone
		 * does!..." */
		fdEventShowMessageAndWait(actor, 0);
		fdEventEnd();
	}
}

/* Talking to the shopkeeper across her counter opens Izumo's shop. From any */
/* other side she speaks of the special powers of Izumo's leaders, or once the
 */
/* Serpent is slain of the wonders the party must see on its travels. */
void scIzumoRoomsTalkToMerchant(s32 actor) {
	u16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnShopRun(SHOP_IZUMO, actor);
	}
	else {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		if(syGameFlagGet(FLAG_SERPENT_SLAIN))
			fdEventSetMessageId((s32)MsgYouMustSee);
		else
			fdEventSetMessageId((s32)MsgIzumoSLeaders);
		/* "Izumo's leaders have always possessed certain special powers...." */
		fdEventShowMessageAndWait(actor, 0);
		fdEventEnd();
	}
}

/* Talking to Lady Uzume in Susa's house before the Serpent is slain: the */
/* first time she says Susa has gone to fight the Serpent to save Kushinada, */
/* all for naught since a hero will surely appear, and each time she fears */
/* she will be alone if it defeats him. */
void scIzumoRoomsAskAfterSusa(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(!fdEventGetFlag(FLAG_UZUME_SPOKE_OF_SUSA)) {
		fdEventSetMessageId((s32)MsgSusaSNot);
		/* "Susa's not here. I believe he went to Mt. Mikage." */
		fdEventShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
		fdEventWait(30);
		fdActorShowEmoteById(ACTOR_SUSAS_HOUSE_UZUME,
							 EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
		fdEventWait(60);
		/* "He believes he can save Kushinada, and so he has gone to fight
		 * the..." */
		fdEventShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
		fdEventWait(30);
		fdActorShowEmoteById(ACTOR_SUSAS_HOUSE_UZUME,
							 EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
		fdEventWait(60);
		/* "He truly believes he can defeat the Great Serpent of Mikage." */
		fdEventShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
		fdEventWait(30);
		fdActorStartRepeatedMotion(ACTOR_SUSAS_HOUSE_UZUME, 1);
		fdEventWait(60);
		/* "It's all for naught, though, as a hero will certainly appear and..."
		 */
		fdEventShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
		fdEventWait(30);
		syGameFlagSet(FLAG_UZUME_SPOKE_OF_SUSA);
	}
	fdActorSetAnimationById(ACTOR_SUSAS_HOUSE_UZUME, ANIM_SHAKE_HEAD);
	fdEventWait(60);
	fdEventSetMessageId((s32)MsgIfTheSerpent);
	/* "If the serpent defeats Susa, I'll...be alone." */
	fdEventShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
	fdEventEnd();
}

/* Mind Read on Lady Uzume before the Serpent is slain shows her prayer that */
/* the guardian's anger is not aroused and that Susa will be protected. */
void scIzumoRoomsReadPrayerForSusa(void) {
	syGameFlagSet(FLAG_UZUMES_PRAYER_READ);
	fdEventSetMessageId((s32)MsgIPrayThat);
	/* "I pray that our guardian's anger is not aroused and that Susa will..."
	 */
	fdEventShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
}

/* Talking to Lady Uzume in Susa's house once the Serpent is slain: she */
/* invites the party to enjoy the festival, and if one of the party carries the
 */
/* Dancing Idol the field menu opens so that it can be given to her. Once she */
/* has it, she says she can explore Mt. Mikage on her own with it. */
void scIzumoRoomsTalkToUzume(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
	if(syGameFlagGet(FLAG_UZUME_HAS_THE_DOLL)) {
		fdEventSetMessageId((s32)MsgWithThisDancing);
		/* "With this dancing doll, I can explore Mt. Mikage on my own. Take..."
		 */
		fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	}
	else {
		fdEventSetMessageId((s32)MsgIfYouHave);
		/* "If you have time, won't you enjoy a little of our celebratory
		 * festival?" */
		fdEventShowMessageAndWait(ACTOR_UZUME, 0);
		if(gmPartyInventoryFindMember(ITEM_DANCING_IDOL) != -1)
			state->fieldMenu = 1;
	}
	fdActorFaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
	fdEventEnd();
}

/* The west one of the two women in red in Susa's house, once the Serpent is */
/* slain, asks whether the party went into the mountain too: either way she */
/* tells of the legend of a dancing doll of mysterious power hidden up there, */
/* which Lady Uzume would love to have. */
void scIzumoRoomsAskAboutMtMikage(void) {
	s32 msg;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	msg = (s32)MsgTheyVeBeen;
	fdEventSetMessageId(msg);
	/* "They've been saying that you went into Mt. Mikage, too... Is that true?"
	 */
	fdMessageShowAtActor(ACTOR_FIRST_DANCER, 0);
	if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdEventWait(10);
		fdEventSetMessageId(msg + 1);
	}
	else {
		fdEventWait(20);
		fdEventSetMessageId(msg + 2);
	}
	fdEventShowMessageAndWait(ACTOR_FIRST_DANCER, 0);
	fdEventEnd();
}

/* Mind Read on Lady Uzume once the Serpent is slain: she notices it ("And what
 */
/* were you planning to do, once you'd read my mind, hm?"), and scolds Felix */
/* if her prayer was read before; tried again, she calls him a pest who can see
 */
/* her mind but not her heart. */
void scIzumoRoomsReadUzumesMind(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
	if(fdEventGetFlag(FLAG_UZUME_MIND_READ)) {
		fdEventSetMessageId((s32)MsgTryingAgainWhat);
		/* "Trying again, Felix? What a pest. You can see my mind, but you
		 * are..." */
		fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	}
	else {
		fdEventSetMessageId((s32)MsgAndWhatWere);
		/* "And what were you planning to do, once you'd read my mind, hm?" */
		fdEventShowMessageAndWait(ACTOR_UZUME, 0);
		if(syGameFlagGet(FLAG_UZUMES_PRAYER_READ)) {
			fdActorShowEmoteById(MainRoutineStateWork.leaderActorId,
								 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
			fdActorShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
								 30);
			fdEventShowMessageAndWait(ACTOR_UZUME, 0);
			fdActorShowEmoteById(MainRoutineStateWork.leaderActorId,
								 EMOTE_IN_FRONT | EMOTE_WINCE, 30);
			fdActorSetAnimationById(ACTOR_UZUME, ANIM_SHAKE_HEAD);
			fdEventWait(30);
			fdEventShowMessageAndWait(ACTOR_UZUME, 0);
		}
		syGameFlagSet(FLAG_UZUME_MIND_READ);
	}
	fdActorFaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
	fdEventEnd();
}

/* Talking to Kushinada on her seat of stones in her house in the north-west, */
/* before the Serpent is slain: she takes Felix for Susa without looking up. */
/* The first time she begs him not to come for her, lest she lose the courage */
/* to be offered for Izumo; after that only "Susa, please... Don't try to save
 */
/* me." Each time Felix steps back from her. */
s32 scIzumoRoomsKushinadaAsksSusaNotToCome(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(!fdEventGetFlag(FLAG_KUSHINADA_ASKED_SUSA_NOT_TO_COME)) {
		fdEventSetMessageId((s32)MsgSusaDonT);
		/* "Susa, don't come for me..." */
		fdEventShowMessageAndWait(ACTOR_KUSHINADA, 0);
		fdEventWait(60);
		/* "If I see you, I'll surely lose the courage to sacrifice myself
		 * for..." */
		fdEventShowMessageAndWait(ACTOR_KUSHINADA, 0);
		fdEventWait(60);
		fdEventWait(15);
		/* "If I run, Lady Uzume and all of Izumo will suffer greatly......" */
		fdEventShowMessageAndWait(ACTOR_KUSHINADA, 0);
		fdEventWait(60);
		fdEventWait(15);
		/* "Lady Uzume has foretold the coming of a hero, and I believe her..."
		 */
		fdEventShowMessageAndWait(ACTOR_KUSHINADA, 0);
		fdEventWait(60);
		fdActorStartRepeatedMotion(ACTOR_KUSHINADA, 1);
		fdEventWait(30);
		/* "And so I shall remain here, until the coming of the next full..." */
		fdEventShowMessageAndWait(ACTOR_KUSHINADA, 0);
		fdEventWait(60);
		fdEventSetFlag(FLAG_KUSHINADA_ASKED_SUSA_NOT_TO_COME);
	}
	else {
		fdEventSetMessageId((s32)MsgSusaPleaseDon);
		/* "Susa, please... Don't try to save me." */
		fdEventShowMessageAndWait(ACTOR_KUSHINADA, 0);
	}
	fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlg &=
		~ACTOR_FACES_MOTION;
	fdActorWalkByById(MainRoutineStateWork.leaderActorId, 0, 16);
	fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlg |=
		ACTOR_FACES_MOTION;
	fdEventEnd();
	return 0;
}

/* The first time the party comes into Susa's house after the Serpent is */
/* slain, Felix walking up the east side of the hall (cell (12, 35)) comes upon
 */
/* Lady Uzume and Kushinada hearing that warriors came out of the mountain, */
/* and the welcome of the heroes begins. */
void scIzumoRoomsHeroesComeFromTheEast(void) {
	scIzumoRoomsRecognizeTheHeroes(HEROES_FROM_THE_EAST);
}

/* The first time the party comes into Susa's house after the Serpent is */
/* slain, Felix walking up the west side of the hall (cell (5, 35)) comes upon
 */
/* Lady Uzume and Kushinada hearing that warriors came out of the mountain, */
/* and the welcome of the heroes begins. */
void scIzumoRoomsHeroesComeFromTheWest(void) {
	scIzumoRoomsRecognizeTheHeroes(HEROES_FROM_THE_WEST);
}

/* Susa's answers to Sheba's "Don't forget, Susa dealt the finishing blow, */
/* didn't he?", from MsgYouReNotImplying on. */
typedef enum FINISHING_BLOW_MESSAGE {
	/* "Finishing blow? All I did was stab it with the Cloud Brand." */
	SUSA_ALL_I_DID_WAS_STAB_IT = 10,
	/* "That's right. Piercing it with the Cloud Brand was hardly a finishing */
	/* blow." */
	SUSA_HARDLY_A_FINISHING_BLOW = 11
} FINISHING_BLOW_MESSAGE;

/* The party's answers to Kraden's "Even Felix agrees, don't you, Felix?", */
/* from MsgIDisagree on. */
typedef enum SUSAS_REWARD_MESSAGE {
	/* Yes: "I do, too.", "We weren't fighting in hopes of getting a reward,
	   were */
	/* we?", "We'll be satisfied just learning your Psynergy." */
	PARTY_WANTS_NO_REWARD = 4,
	/* No: "Are you asking for a reward? That's just plain greedy!", "We */
	/* couldn't possibly take any sort of reward.", "Learning new Psynergy will
	 */
	/* be reward enough." */
	PARTY_CALLS_FELIX_GREEDY = 7
} SUSAS_REWARD_MESSAGE;

/* The heroes' welcome in Susa's house. A villager tells Lady Uzume and */
/* Kushinada that warriors came out of the mountain after the Serpent's roar; */
/* Lady Uzume orders the strangers out of her home until the villager knows */
/* them for the heroes, and she bids them welcome. The party introduce */
/* themselves, and Lady Uzume shows them her Move. Susa walks in from the */
/* entrance, Kraden explains Psynergy and Alchemy, and Felix answers three */
/* questions: whether the party slew the Serpent only for its quest, whether */
/* Susa dealt the finishing blow, and whether Susa deserves the reward. The */
/* party say they must leave for Lemuria and vow to return. */
/* */
/* The party, Lady Uzume, the villager and Susa walk on the side of the hall */
/* Felix came up. */
void scIzumoRoomsRecognizeTheHeroes(s32 side) {
	ACTOR *leader;
	s32 answer, message;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdCameraMoveTo(143, CAMERA_KEEP, 560, 1);
	if(side == HEROES_FROM_THE_EAST) {
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 184, 568);
	}
	else {
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 104, 568);
	}
	syFieldEventWait(15);
	syFieldEventSetMessageId((s32)MsgWarriorsAppearedFrom);
	/* "Warriors appeared from within Mt. Mikage?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
	fdActorSetAnimation(ACTOR_WITNESS, ANIM_NOD);
	/* "Yes... After the serpent's roar, these warriors appeared." */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WITNESS, 0);
	fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "So, it wasn't Susa who defeated the serpent, but those heroes..." */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
	syFieldEventWait(30);
	fdActorSetAnimation(ACTOR_WITNESS, ANIM_NOD);
	syFieldEventWait(15);
	/* "It might have been..." */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WITNESS, 0);
	fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	scFieldActorStartRepeatedMotion(ACTOR_WITNESS, 3);
	fdActorSetAttachedEffect(ACTOR_WITNESS,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(30);
	/* "Lady Kushinada, please forgive me..." */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WITNESS, 0);
	fdActorFaceDirection(ACTOR_SAVED_KUSHINADA, FACING_EAST, 30);
	fdActorSetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_SHAKE_HEAD);
	/* "I don't care who defeated the serpent... I'm more worried about what..."
	 */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
	syFieldEventWait(30);
	fdActorFaceDirection(ACTOR_UZUME, FACING_WEST, 30);
	fdActorSetAnimation(ACTOR_UZUME, ANIM_NOD);
	syFieldEventWait(30);
	/* "So, Susa never came down from Mt. Mikage?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
	fdActorSetAnimation(ACTOR_WITNESS, ANIM_NOD);
	syFieldEventWait(15);
	/* "He returned to the village after the heroes appeared, and he knew..." */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WITNESS, 0);
	scFieldActorStartRepeatedMotion(ACTOR_SAVED_KUSHINADA, 2);
	syFieldEventWait(15);
	/* "Oh, Susa..." */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
	fdActorFaceActor(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
	syFieldEventWait(30);
	fdActorSetDestinationOffset(ACTOR_UZUME, 0, 16);
	scFieldActorWaitForMoveById(ACTOR_UZUME);
	syFieldEventWait(15);
	fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	if(side == HEROES_FROM_THE_EAST) {
		fdActorWalkToAndWait(ACTOR_UZUME, 168, 536);
	}
	else {
		fdActorWalkToAndWait(ACTOR_UZUME, 120, 536);
	}
	fdActorFaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
	fdActorFaceActor(ACTOR_SAVED_KUSHINADA, MainRoutineStateWork.leaderActorId,
					 0);
	/* "Who are you?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
	fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	syFieldEventWait(30);
	/* "This is my home, and I am having a private conversation. Leave at once!"
	 */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
	scFieldActorJump(ACTOR_WITNESS, 4, 0);
	fdActorSetAttachedEffect(ACTOR_WITNESS,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	syFieldEventWait(30);
	fdActorFaceDirection(ACTOR_SAVED_KUSHINADA, FACING_SOUTH, 30);
	fdActorShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_QUESTION,
					 30);
	/* "What is it?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
	fdActorFaceActor(ACTOR_WITNESS, ACTOR_SAVED_KUSHINADA, 0);
	fdActorSetAttachedEffect(ACTOR_WITNESS,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	/* "There can be no doubt... These are the heroes who appeared on Mt...." */
	fdEventShowMessage(ACTOR_WITNESS, 0);
	syFieldEventWait(30);
	syFieldEventWait(30);
	/* "Are you the ones who defeated the serpent?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
	scFieldActorStartRepeatedMotion(ACTOR_SAVED_KUSHINADA, 2);
	syFieldEventWait(30);
	fdActorFaceActor(ACTOR_SAVED_KUSHINADA, MainRoutineStateWork.leaderActorId,
					 0);
	/* "So it was you..." */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
	fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "My apologies... You are welcome here." */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
	fdActorWalkTo(ACTOR_UZUME, 144, 520);
	scFieldActorWaitForMoveById(ACTOR_UZUME);
	fdActorFaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SAVED_KUSHINADA, FACING_SOUTH, 0);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	syFieldEventWait(30);
	if(side == HEROES_FROM_THE_EAST) {
		fdActorWalkTo(ACTOR_WITNESS, 104, 568);
	}
	else {
		fdActorWalkTo(ACTOR_WITNESS, 184, 568);
	}
	scFieldActorWaitForMoveById(ACTOR_WITNESS);
	if(side == HEROES_FROM_THE_EAST) {
		fdActorFaceDirection(ACTOR_WITNESS, FACING_EAST, 0);
	}
	else {
		fdActorFaceDirection(ACTOR_WITNESS, FACING_WEST, 0);
	}
	fdActorWalkTo(MainRoutineStateWork.leaderActorId, 144, 560);
	scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, 8, 24, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, 24, 24, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_PIERS, -8, 24, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, -24, 24, FACING_NORTH);
	syFieldEventWait(60);
	/* "Who are you?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(30);
	/* "I'm Kraden, and this is Felix." */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	syFieldEventWait(30);
	fdActorSetAnimation(ACTOR_UZUME, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
	syFieldEventWait(30);
	fdActorFaceActor(ACTOR_KRADEN, ACTOR_PIERS, 0);
	syFieldEventWait(30);
	/* "Behind him is Piers." */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	syFieldEventWait(30);
	/* "Pleased to meet you." */
	fdEventShowMessage(ACTOR_PIERS, 0);
	fdActorSetAnimation(ACTOR_UZUME, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
	syFieldEventWait(30);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	syFieldEventWait(30);
	/* "I'm Jenna, and this is Sheba." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	syFieldEventWait(30);
	fdActorSetAnimation(ACTOR_UZUME, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
	syFieldEventWait(30);
	fdActorFaceActor(ACTOR_JENNA, ACTOR_KRADEN, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(30);
	/* "We're on a quest. Our travels across the Great Eastern Sea brought..."
	 */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorSetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
	syFieldEventWait(30);
	/* "You were seen coming down from Mt. Mikage. What brought you to such..."
	 */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	syFieldEventWait(30);
	/* "To complete our quest, we need many types of Psynergy." */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Psynergy?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
	fdActorFaceEachOther(ACTOR_UZUME, ACTOR_SAVED_KUSHINADA, 0);
	syFieldEventWait(60);
	fdActorFaceActor(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
	fdActorFaceActor(ACTOR_SAVED_KUSHINADA, MainRoutineStateWork.leaderActorId,
					 0);
	syFieldEventWait(30);
	fdActorSetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_SHAKE_HEAD);
	syFieldEventWait(30);
	/* "What is Psynergy?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
	fdActorFaceActor(MainRoutineStateWork.leaderActorId, ACTOR_KRADEN, 0);
	fdActorFaceActor(ACTOR_JENNA, ACTOR_KRADEN, 0);
	fdActorFaceActor(ACTOR_SHEBA, ACTOR_KRADEN, 0);
	fdActorFaceActor(ACTOR_PIERS, ACTOR_KRADEN, 0);
	syFieldEventWait(30);
	fdActorFaceActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId, 0);
	syFieldEventWait(30);
	fdActorFaceActor(ACTOR_KRADEN, ACTOR_UZUME, 0);
	syFieldEventWait(30);
	/* "Psynergy is many things... The power to move things with one's mind..."
	 */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 0);
	syFieldEventWait(30);
	/* "All with one's mind?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
	fdActorFaceActor(MainRoutineStateWork.leaderActorId, ACTOR_UZUME, 0);
	fdActorFaceActor(ACTOR_JENNA, ACTOR_UZUME, 0);
	fdActorFaceActor(ACTOR_SHEBA, ACTOR_UZUME, 0);
	fdActorFaceActor(ACTOR_PIERS, ACTOR_UZUME, 0);
	syFieldEventWait(30);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(30);
	fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Tell me, is it something like...this?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
	syFieldEventWait(60);
	fdActorFaceDirection(ACTOR_UZUME, FACING_EAST, 0);
	syFieldEventWait(30);
	scIzumoRoomsShowMove();
	syFieldEventWait(30);
	fdActorFaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
	syFieldEventWait(30);
	/* "Is that the power you're talking about?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
					 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	syFieldEventWait(30);
	scFieldActorStartRepeatedMotion(ACTOR_JENNA, 1);
	syFieldEventWait(15);
	/* "That power? Lady Uzume?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorFaceActor(ACTOR_UZUME, ACTOR_SAVED_KUSHINADA, 0);
	syFieldEventWait(30);
	fdActorFaceActor(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
	syFieldEventWait(30);
	/* "It's not mine alone. Kushinada and Susa possess it as well." */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
	fdActorShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_QUESTION,
					 30);
	syFieldEventWait(30);
	/* "This is the power we possess. I assume you have something like it..." */
	fdEventOpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
	answer = fdEventAskYesNoWithActorAnimation(
		MainRoutineStateWork.leaderActorId, 0);
	if(answer == 1) {
		syFieldEventSetMessageId((s32)MsgIThoughtSo);
		fdActorShowEmote(ACTOR_SAVED_KUSHINADA,
						 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
		/* "I thought so. Only those who can wield the power can see it. And
		 * you..." */
		fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
		fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
		fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
		syFieldEventWait(30);
		fdActorFaceActor(ACTOR_PIERS, MainRoutineStateWork.leaderActorId, 0);
		syFieldEventWait(15);
		fdActorSetAnimation(ACTOR_PIERS, ANIM_SHAKE_HEAD);
		syFieldEventWait(30);
		/* "Felix, there's no need to hide our abilities from them, is there?"
		 */
		fdEventShowMessage(ACTOR_PIERS, 0);
	}
	syFieldEventSetMessageId((s32)MsgSoYouCall);
	fdActorSetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
	syFieldEventWait(30);
	/* "So, you call what we do "Psynergy," and you can do it, too?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(30);
	fdActorSetAnimation(ACTOR_UZUME, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
	syFieldEventWait(60);
	fdActorShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_QUESTION,
					 30);
	fdActorFaceActor(ACTOR_SAVED_KUSHINADA, MainRoutineStateWork.leaderActorId,
					 0);
	syFieldEventWait(30);
	/* "If you already have Psynergy, why do you need more of this power?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
	syFieldEventWait(90);
	/* "Their powers are different from our own. There seem to be many forms..."
	 */
	fdEventShowMessage(ACTOR_SUSA, 0);
	fdActorSetPos(ACTOR_SUSA, 144, 680);
	fdActorFaceActor(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
	fdActorFaceActor(ACTOR_JENNA, ACTOR_SUSA, 0);
	fdActorFaceActor(ACTOR_SHEBA, ACTOR_SUSA, 0);
	fdActorFaceActor(ACTOR_PIERS, ACTOR_SUSA, 0);
	fdActorFaceActor(ACTOR_KRADEN, ACTOR_SUSA, 0);
	fdActorFaceActor(ACTOR_WITNESS, ACTOR_SUSA, 0);
	syFieldEventWait(120);
	/* "Susa!" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
	syFieldEventWait(30);
	syEventCameraFollowActor(ACTOR_SUSA, 1);
	syFieldEventWait(120);
	fdActorSetSpeed(ACTOR_SUSA, 0.6);
	fdActorSetSpeed(ACTOR_SUSA, 0.6);
	fdActorGetById(ACTOR_SUSA)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorWalkTo(ACTOR_SUSA, 144, 648);
	if(side == HEROES_FROM_THE_EAST) {
		fdActorWalkToAndWait(ACTOR_SUSA, 88, 648);
		fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
		fdActorWalkToAndWait(ACTOR_SUSA, 56, 616);
		fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
		fdActorWalkToAndWait(ACTOR_SUSA, 56, 584);
		fdActorGetById(ACTOR_WITNESS)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkToAndWait(ACTOR_WITNESS, 88, 568);
		fdEventWait(1);
		fdActorGetById(ACTOR_WITNESS)->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorFaceDirectionById(ACTOR_WITNESS, FACING_EAST, 0);
		fdActorSetSprPriority(ACTOR_SUSA, 1);
		fdActorWalkToAndWait(ACTOR_SUSA, 104, 568);
		fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
		fdActorSetSprPriority(ACTOR_KRADEN, 1);
		fdActorSetSprPriority(ACTOR_SUSA, 2);
		fdActorWalkToAndWaitById(ACTOR_SUSA, 120, 568);
		fdActorSetSprPriority(ACTOR_SUSA, 3);
		fdActorSetSprPriority(ACTOR_KRADEN, 3);
		fdActorWalkToAndWait(ACTOR_SUSA, 120, 536);
		fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
		fdActorSetAnimationById(ACTOR_WITNESS, ANIM_NOD);
		fdActorWalkToAndWait(ACTOR_WITNESS, 56, 584);
		fdActorWalkToAndWait(ACTOR_WITNESS, 88, 648);
		fdActorSetPos(ACTOR_WITNESS, 0, 0);
	}
	else {
		fdActorWalkToAndWait(ACTOR_SUSA, 200, 648);
		fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
		fdActorWalkToAndWait(ACTOR_SUSA, 232, 616);
		fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
		fdActorWalkToAndWait(ACTOR_SUSA, 232, 584);
		fdActorGetById(ACTOR_WITNESS)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkToAndWait(ACTOR_WITNESS, 200, 568);
		fdEventWait(1);
		fdActorGetById(ACTOR_WITNESS)->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorFaceDirection(ACTOR_WITNESS, FACING_WEST, 0);
		fdActorSetSprPriority(ACTOR_SUSA, 1);
		fdActorWalkToAndWait(ACTOR_SUSA, 184, 568);
		fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
		fdActorSetSprPriority(ACTOR_SUSA, 2);
		fdActorSetSprPriority(ACTOR_SHEBA, 1);
		fdActorWalkToAndWaitById(ACTOR_SUSA, 168, 568);
		fdActorSetSprPriority(ACTOR_SUSA, 3);
		fdActorSetSprPriority(ACTOR_SHEBA, 3);
		fdActorWalkToAndWait(ACTOR_SUSA, 168, 536);
		fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
		fdActorFaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
		fdActorSetAnimationById(ACTOR_WITNESS, ANIM_NOD);
		fdActorWalkToAndWait(ACTOR_WITNESS, 232, 584);
		fdActorWalkToAndWait(ACTOR_WITNESS, 200, 648);
		fdActorSetPos(ACTOR_WITNESS, 0, 0);
	}
	fdActorGetById(ACTOR_SUSA)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_NORTH, 0);
	fdEventWait(15);
	/* "Susa..." */
	fdEventShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
	fdEventWait(15);
	fdActorShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "So, you lived after all, Susa..." */
	fdEventShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
	fdActorFaceActorById(ACTOR_SUSA, ACTOR_SAVED_KUSHINADA, 0);
	fdEventWait(30);
	/* "Yes, I've survived, and I've made it home to you both." */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	fdActorFaceActorById(ACTOR_SUSA, MainRoutineStateWork.leaderActorId, 0);
	fdEventWait(60);
	fdActorFaceActorById(ACTOR_SUSA, ACTOR_SAVED_KUSHINADA, 0);
	fdEventWait(30);
	/* "Thanks to them, actually..." */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	fdActorTurnToFaceEachOther(ACTOR_UZUME, ACTOR_SAVED_KUSHINADA, 0);
	fdEventWait(30);
	fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
	fdActorFaceActorById(ACTOR_SAVED_KUSHINADA,
						 MainRoutineStateWork.leaderActorId, 0);
	/* "Felix, our gratitude to you and your friends grows deeper each..." */
	fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	fdActorSetAnimationById(ACTOR_UZUME, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
	fdEventWait(30);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(30);
	fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Susa, you spoke of different types of power. Tell me, what did you
	 * mean?" */
	fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	fdActorFaceActorById(ACTOR_SUSA, MainRoutineStateWork.leaderActorId, 0);
	/* "I can only describe what I saw, my sister. Ask Felix here about it." */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
	fdActorFaceActorById(ACTOR_SAVED_KUSHINADA,
						 MainRoutineStateWork.leaderActorId, 0);
	fdEventWait(30);
	fdActorShowEmoteById(MainRoutineStateWork.leaderActorId,
						 EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(30);
	/* "Ah, yes. Psynergy... I have dedicated most of my life to its study..."
	 */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetAnimationById(ACTOR_UZUME, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
	fdEventWait(30);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	/* "The power of Psynergy comes from the four basic forces of earth..." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdEventWait(30);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "You don't know Alchemy? The four elements? Why, they're the very..." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "All existing matter comes from Alchemy and this Psynergy?" */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
	fdEventWait(30);
	/* "Yes, that's right. Everything you see around you is some combination..."
	 */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorShowEmoteById(ACTOR_SAVED_KUSHINADA,
						 EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	/* "Does this mean that there are as many types of Psynergy as there are..."
	 */
	fdEventShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "That's right, Kushinada. You're a smart lass." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(30);
	/* "Mt. Mikage was a place of great earth powers. I'd imagine your..." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "That's right, but how did you know this?" */
	fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	fdActorFaceActorById(ACTOR_PIERS, ACTOR_KRADEN, 0);
	fdEventWait(30);
	/* "Kraden... Mt. Mikage is Gaia Rock, isn't it?" */
	fdEventShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorFaceActorById(ACTOR_KRADEN, ACTOR_PIERS, 0);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(30);
	fdActorShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceActorById(ACTOR_KRADEN, ACTOR_UZUME, 0);
	fdActorFaceActorById(ACTOR_PIERS, ACTOR_UZUME, 0);
	fdEventWait(30);
	/* "...Gaia Rock? Mt. Mikage is the sacred mount of our people!" */
	fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Yes, but there are many mountains like your Mt. Mikage all across..." */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "You have seen other mountains like our Mt. Mikage?" */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdEventWait(30);
	/* "Yes! In fact, it seems that each of these rocks contains a different..."
	 */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorSetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
	fdEventWait(30);
	/* "And now, you are questing to gather the power of these elements, yes?"
	 */
	fdEventShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(90);
	fdActorSetAnimationById(ACTOR_UZUME, ANIM_NOD);
	/* "And you only defeated the Great Serpent of Mikage to accomplish this..."
	 */
	fdEventOpenMessage(ACTOR_UZUME, 0);
	answer = fdEventAskYesNoWithActorAnimation(
		MainRoutineStateWork.leaderActorId, 0);
	if(answer == 0) {
		fdEventSetMessageId((s32)MsgRegardlessOfYour);
		fdActorShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
		/* "Regardless of your motives, you have saved us all." */
		fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	}
	else {
		fdEventSetMessageId((s32)MsgYouTookPity);
		/* "You took pity on Kushinada? You are very kind to help Susa, Felix."
		 */
		fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	}
	message = (s32)MsgYouReNotImplying;
	fdEventSetMessageId(message);
	fdActorStartRepeatedMotion(ACTOR_KRADEN, 1);
	fdEventWait(30);
	/* "You're...not implying that we defeated the serpent, are you?" */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceActorById(ACTOR_UZUME, ACTOR_KRADEN, 0);
	fdActorShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(30);
	/* "It's true that we did fight the creature..." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
	fdEventWait(30);
	/* "But without Susa's power, the battle might not have ended so happily."
	 */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorTurnToFaceEachOther(ACTOR_UZUME, ACTOR_SAVED_KUSHINADA, 0);
	fdEventWait(30);
	/* "What... What are you°" */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
	fdEventWait(30);
	/* "Susa had given the beast a lot of Dragonsbane in order to weaken it." */
	fdEventShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorFaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
	fdActorFaceDirectionById(ACTOR_SAVED_KUSHINADA, FACING_SOUTH, 0);
	fdEventWait(30);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdEventWait(30);
	/* "We could never have beaten the beast if it had not been in..." */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "We were victorious only because Susa risked everything to weaken the..."
	 */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdEventWait(15);
	fdActorFaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
	/* "Susa, perhaps, after all, you are..." */
	fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	fdActorShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "No, I didn't..." */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	fdActorShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	fdActorFaceActorById(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId, 0);
	/* "Don't forget, Susa dealt the finishing blow, didn't he?" */
	fdEventOpenMessage(ACTOR_SHEBA, 0);
	fdActorFaceActorById(ACTOR_JENNA, MainRoutineStateWork.leaderActorId, 0);
	fdActorFaceActorById(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId, 0);
	fdActorFaceActorById(ACTOR_PIERS, MainRoutineStateWork.leaderActorId, 0);
	fdActorFaceActorById(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId, 0);
	fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
	fdActorFaceActorById(ACTOR_SAVED_KUSHINADA,
						 MainRoutineStateWork.leaderActorId, 0);
	fdActorFaceActorById(ACTOR_SUSA, MainRoutineStateWork.leaderActorId, 0);
	fdEventWait(30);
	answer = fdEventAskYesNoWithActorAnimation(
		MainRoutineStateWork.leaderActorId, 0);
	if(answer == 0) {
		fdEventSetMessageId(message + SUSA_ALL_I_DID_WAS_STAB_IT);
		fdActorSetAnimationById(ACTOR_SUSA, ANIM_SHAKE_HEAD);
		fdEventWait(30);
		/* "Finishing blow? All I did was stab it with the Cloud Brand." */
		fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	}
	else {
		fdEventSetMessageId(message + SUSA_HARDLY_A_FINISHING_BLOW);
		fdActorSetAnimationById(ACTOR_SUSA, ANIM_NOD);
		fdEventWait(30);
		/* "That's right. Piercing it with the Cloud Brand was hardly a..." */
		fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	}
	message = (s32)MsgIDisagree;
	fdEventSetMessageId(message);
	fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "I disagree..." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(30);
	/* "You weakened the serpent. You dealt the final blow. I'd say you were..."
	 */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
	fdEventWait(30);
	fdActorFaceActorById(ACTOR_KRADEN, ACTOR_UZUME, 0);
	fdEventWait(30);
	/* "So, Lady Uzume, wouldn't you say that Susa deserves the reward for..."
	 */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
	fdActorFaceActorById(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId, 0);
	fdEventWait(30);
	/* "Even Felix agrees, don't you, Felix?" */
	fdEventOpenMessage(ACTOR_KRADEN, 0);
	answer = fdEventAskYesNoWithActorAnimation(
		MainRoutineStateWork.leaderActorId, 0);
	if(answer == 0) {
		fdEventSetMessageId(message + PARTY_WANTS_NO_REWARD);
		fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
		fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
		fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
		fdEventWait(30);
		/* "I do, too." */
		fdEventShowMessageAndWait(ACTOR_JENNA, 0);
		/* "We weren't fighting in hopes of getting a reward, were we?" */
		fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
		/* "We'll be satisfied just learning your Psynergy." */
		fdEventShowMessageAndWait(ACTOR_PIERS, 0);
	}
	else {
		fdEventSetMessageId(message + PARTY_CALLS_FELIX_GREEDY);
		fdActorSetAnimationById(ACTOR_JENNA, ANIM_SHAKE_HEAD);
		fdActorSetAnimationById(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
		fdActorSetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
		fdEventWait(30);
		/* "Are you asking for a reward? That's just plain greedy!" */
		fdEventShowMessageAndWait(ACTOR_JENNA, 0);
		/* "We couldn't possibly take any sort of reward." */
		fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
		/* "Learning new Psynergy will be reward enough." */
		fdEventShowMessageAndWait(ACTOR_PIERS, 0);
	}
	fdEventSetMessageId((s32)MsgAsYouCanSee);
	fdActorFaceActorById(ACTOR_KRADEN, ACTOR_UZUME, 0);
	fdEventWait(30);
	/* "As you can see." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorTurnToFaceEachOther(ACTOR_UZUME, ACTOR_SAVED_KUSHINADA, 0);
	fdEventWait(30);
	fdActorSetAnimationById(ACTOR_UZUME, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
	fdEventWait(30);
	fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
	fdActorFaceActorById(ACTOR_SAVED_KUSHINADA,
						 MainRoutineStateWork.leaderActorId, 0);
	fdActorStartRepeatedMotion(ACTOR_SUSA, 1);
	fdEventWait(15);
	fdActorShowEmote(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	fdActorFaceActorById(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId, 0);
	fdEventWait(30);
	/* "Now, it's about time for us to leave Izumo Village." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_KRADEN, 0);
	fdEventWait(15);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdEventWait(30);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(30);
	/* "Yes, indeed! We're on our way to Lemuria." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceActorById(ACTOR_JENNA, ACTOR_KRADEN, 0);
	fdActorFaceActorById(ACTOR_SHEBA, ACTOR_KRADEN, 0);
	fdActorFaceActorById(ACTOR_PIERS, ACTOR_KRADEN, 0);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdEventWait(30);
	fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 0);
	fdActorShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdEventWait(30);
	/* "You're leaving so soon?" */
	fdEventShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
	fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_UZUME, 0);
	fdActorFaceActorById(ACTOR_JENNA, ACTOR_UZUME, 0);
	fdActorFaceActorById(ACTOR_SHEBA, ACTOR_UZUME, 0);
	fdActorFaceActorById(ACTOR_PIERS, ACTOR_UZUME, 0);
	fdActorFaceActorById(ACTOR_KRADEN, ACTOR_UZUME, 0);
	fdEventWait(30);
	/* "We are in a great hurry." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Then you must return to Izumo someday." */
	fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdEventWait(30);
	/* "I'm sure we'll be back once our mission is completed." */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorSetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
	fdEventWait(30);
	/* "Excellent! I shall accept that as your oath!" */
	fdEventShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
	fdActorShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdEventWait(30);
	/* "...Oath?" */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorFaceActorById(ACTOR_SHEBA, ACTOR_JENNA, 0);
	fdEventWait(30);
	/* "Oh, by promising to return, we'll be ensuring that our quest will..." */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorStartRepeatedMotion(ACTOR_PIERS, 1);
	fdEventWait(30);
	/* "I understand... We must vow to return, everyone." */
	fdEventShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorTurnToFaceEachOther(MainRoutineStateWork.leaderActorId, ACTOR_PIERS,
							   0);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
	fdActorTurnToFaceEachOther(ACTOR_JENNA, ACTOR_SHEBA, 0);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdEventWait(15);
	fdActorTurnToFaceEachOther(MainRoutineStateWork.leaderActorId, ACTOR_JENNA,
							   0);
	fdActorTurnToFaceEachOther(ACTOR_PIERS, ACTOR_SHEBA, 0);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdEventWait(30);
	fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_UZUME, 0);
	fdActorFaceActorById(ACTOR_JENNA, ACTOR_UZUME, 0);
	fdActorFaceActorById(ACTOR_SHEBA, ACTOR_UZUME, 0);
	fdActorFaceActorById(ACTOR_PIERS, ACTOR_UZUME, 0);
	fdActorFaceActorById(ACTOR_KRADEN, ACTOR_UZUME, 0);
	fdActorSetAnimationById(ACTOR_UZUME, ANIM_NOD);
	fdEventWait(30);
	/* "We'll be waiting." */
	fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(15);
	fdActorSetAnimationById(ACTOR_UZUME, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SUSA, ANIM_NOD);
	fdEventWait(60);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(ACTOR_JENNA, leader->x.part[0].integer,
								  leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_JENNA);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(ACTOR_SHEBA, leader->x.part[0].integer,
								  leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_SHEBA);
	fdActorSetPos(ACTOR_SHEBA, 0, 0);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(ACTOR_PIERS, leader->x.part[0].integer,
								  leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_PIERS);
	fdActorSetPos(ACTOR_PIERS, 0, 0);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(ACTOR_KRADEN, leader->x.part[0].integer,
								  leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	syGameFlagSet(FLAG_IZUMO_RECOGNIZED_THE_HEROES);
	syGameFlagSet(FLAG_HEROES_JUST_RECOGNIZED);
	fdEventEnd();
}

/* Lady Uzume's Move in the welcome has nothing to move. */
s32 scIzumoRoomsFindNoMoveTarget(void) { return -1; }

/* Lady Uzume casts Move in the welcome ("Tell me, is it something */
/* like...this?") with nothing to move. */
void scIzumoRoomsShowMove(void) {
	fdPsynergyLoadResources(ABILITY_MOVE, 1);
	fdPsynergySetupActorPair(ACTOR_UZUME, -1);
	((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->finishFunc =
		(void (*)(void))scIzumoRoomsFindNoMoveTarget;
	((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->aimAhead = 1;
	fdPsynergyStartCast();
	fdEventWait(10);
	fdPsynergyRunHandler(0);
	fdBattleEffectStopMainActor();
	fdPsynergyEnd();
}

/* Using the Dancing Idol on Lady Uzume in Susa's house: Felix steps up to */
/* her, she asks whether it was found at the peak of the mountain, and in */
/* thanks gives the party her pet, Coal, the Mars Djinni. */
void scIzumoRoomsGiveUzumeTheDoll(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)&MsgAhCouldIt);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 144, 536);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_NORTH,
							 0);
	fdEventWait(15);
	fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	/* "Ah... Could it be? Is this the dancing doll of Mt. Mikage?" */
	fdEventOpenMessage(ACTOR_UZUME, 0);
	if(!fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
										  0)) {
		fdEventSetMessageId((s32)&MsgAhCouldIt + 1);
		/* "You found this at the peak of Mt. Mikage?" */
		fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	}
	else {
		fdEventSetMessageId((s32)&MsgAhCouldIt + 2);
		/* "Don't lie to me. The style, the craftsmanship... You must have
		 * found..." */
		fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	}
	fdActorShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 60);
	fdEventSetMessageId((s32)&MsgAgainYouHave);
	/* "Again, you have done more for me than I can properly reward you for..."
	 */
	fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	fdActorShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
	fdActorShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
	/* "I think I may have something that might please you." */
	fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	fdBattleEffectStartArcEffect();
	fdBattleEffectSetObjectUpdateMode(fdActorGetById(ACTOR_UZUME),
									  CASTER_LOOK_CYCLING_COLORS);
	sySndPlay(SE_ASCEND);
	fdEventWait(60);
	fdActorSetPos(ACTOR_COAL, 144, 520);
	fdActorJump(ACTOR_COAL, 6, 0);
	fdActorSetDestination(ACTOR_COAL, 160, 520);
	fdActorWaitForMoveById(ACTOR_COAL);
	fdActorFaceDirectionById(ACTOR_COAL, FACING_SOUTH, 0);
	fdEventWait(60);
	fdBattleEffectSetObjectUpdateMode(fdActorGetById(ACTOR_UZUME),
									  CASTER_LOOK_NORMAL);
	fdBattleEffectWaitForArcGroups();
	fdBattleEffectEndArcEffect();
	fdEventWait(30);
	fdEventWait(30);
	fdActorFaceActorById(ACTOR_UZUME, ACTOR_COAL, 0);
	fdEventWait(30);
	/* "This is my pet. It's very dear to me." */
	fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
	fdEventWait(60);
	/* "I am unsure how it works, but this fellow seems to magnify our power..."
	 */
	fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	fdEventWait(30);
	fdActorSetAnimationById(ACTOR_UZUME, ANIM_SHAKE_HEAD);
	fdEventWait(60);
	/* "Though it saddens me to part with it, I would have you take him, as..."
	 */
	fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	fdActorTurnToFaceEachOther(ACTOR_UZUME, ACTOR_COAL, 0);
	/* "Felix and his friends have agreed to care for you from now on." */
	fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	fdEventWait(60);
	fdActorJump(ACTOR_COAL, 4, 20);
	fdEventWait(60);
	fdActorFaceActorById(ACTOR_UZUME, MainRoutineStateWork.leaderActorId, 0);
	fdActorFaceActorById(ACTOR_COAL, MainRoutineStateWork.leaderActorId, 0);
	fdEventWait(60);
	/* "Please, accept this gift." */
	fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	fdActorJump(ACTOR_COAL, 2, 20);
	fdDjinnJoinParty(ACTOR_COAL, DJINNI_COAL / DJINN_PER_ELEMENT,
					 DJINNI_INDEX(DJINNI_COAL));
	fdEventEnd();
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_UZUME, 0);
	fdEventWait(60);
	fdActorStartRepeatedMotion(ACTOR_UZUME, 2);
	fdEventWait(30);
	/* "With this dancing doll, I can explore Mt. Mikage on my own. Take..." */
	fdEventShowMessageAndWait(ACTOR_UZUME, 0);
	fdEventWait(60);
	syGameFlagSet(FLAG_UZUME_HAS_THE_DOLL);
	gmPartyInventoryRemove(ITEM_DANCING_IDOL);
	fdEventEnd();
}

/* Sets up the room the party enters: the window in, Susa's house for the */
/* heroes' welcome or after it, Kushinada sobbing on her seat before the */
/* Serpent is slain, and the dancers and old men in Susa's house before it. */
/* Returns 0. */
s32 scInit(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	if(MainRoutineStateWork.entrance != IZUMO_ROOMS_FROM_IZUMO_NORTH)
		fdEventClearFlag(FLAG_HEROES_JUST_RECOGNIZED);
	if(syGameFlagGet(FLAG_SERPENT_SLAIN) &&
	   !syGameFlagGet(FLAG_IZUMO_RECOGNIZED_THE_HEROES) &&
	   MainRoutineStateWork.entrance == IZUMO_ROOMS_FROM_IZUMO_NORTH) {
		fdEventPlaySnd(MUS_IZUMO);
		fdActorSetPos(ACTOR_WITNESS, 144, 568);
		fdActorSetPos(ACTOR_SUSA, 0, 0);
		fdActorSetPos(ACTOR_FIRST_DANCER, 0, 0);
		fdActorSetPos(ACTOR_SECOND_DANCER, 0, 0);
		fdActorSetPos(ACTOR_WEST_VILLAGER, 0, 0);
		fdActorSetPos(ACTOR_EAST_VILLAGER, 0, 0);
	}
	if(syGameFlagGet(FLAG_SERPENT_SLAIN) &&
	   syGameFlagGet(FLAG_IZUMO_RECOGNIZED_THE_HEROES) &&
	   MainRoutineStateWork.entrance == IZUMO_ROOMS_FROM_IZUMO_NORTH &&
	   syGameFlagGet(FLAG_HEROES_JUST_RECOGNIZED)) {
		fdEventPlaySnd(MUS_IZUMO);
		fdActorSetPos(ACTOR_FIRST_DANCER, 0, 0);
		fdActorSetPos(ACTOR_SECOND_DANCER, 0, 0);
		fdActorSetPos(ACTOR_WEST_VILLAGER, 0, 0);
		fdActorSetPos(ACTOR_EAST_VILLAGER, 0, 0);
	}
	if(MainRoutineStateWork.entrance == IZUMO_ROOMS_FROM_IZUMO_NORTHWEST &&
	   !syGameFlagGet(FLAG_SERPENT_SLAIN)) {
		fdActorSetAnimation(ACTOR_KUSHINADA, ANIM_KNEEL);
		fdActorSetAttachedEffect(ACTOR_KUSHINADA, ATTACHED_EFFECT_IN_FRONT |
													  ATTACHED_EFFECT_ATTACH);
	}
	if(MainRoutineStateWork.entrance == IZUMO_ROOMS_FROM_IZUMO_NORTH &&
	   !syGameFlagGet(FLAG_SERPENT_SLAIN)) {
		ACTOR *actor;

		fdActorSetAnimation(ACTOR_SUSAS_HOUSE_FIRST_DANCER, ANIM_DANCE);
		fdActorSetAnimation(ACTOR_SUSAS_HOUSE_SECOND_DANCER, ANIM_DANCE);
		fdActorSetAnimation(ACTOR_SUSAS_HOUSE_WEST_VILLAGER, ANIM_BEAT_DRUM);
		fdActorSetAnimation(ACTOR_SUSAS_HOUSE_EAST_VILLAGER, ANIM_BEAT_DRUM);
		actor = fdActorGetById(ACTOR_SUSAS_HOUSE_WEST_VILLAGER);
		actor->scaleX =
			-fdActorGetById(ACTOR_SUSAS_HOUSE_WEST_VILLAGER)->scaleX;
	}
	return 0;
}

/* Nothing to prepare before the map is built. Returns 0. */
s32 scPrepareMap(void) { return 0; }

extern s32 IzumoRooms_KushinadaHeavingScript[];

/* The two women dancing in Susa's house before the Serpent is slain step to */
/* the doorway and back with a hop, each waiting until the other is clear of */
/* the spot. */
s32 IzumoRooms_FirstDancerScript[] = {
	SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
	SC_ACCELERATION(0.2),
	SC_SET(SW_GRAVITY, toFIXED(0.4)),
	SC_SPEED(0.6),
	SC_SET(SW_STEERING, 2),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_LABEL(1),
	SC_CALL(IzumoRooms_IsFirstDancerClear),
	SC_MOVE(112, 0, 624),
	SC_SYNC,
	SC_MOVE(112, 0, 600),
	SC_SYNC,
	SC_MOVE(136, 0, 600),
	SC_SYNC,
	SC_MOVE(136, 0, 624),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(5),
	SC_ADD(SW_Y, toFIXED(4)),
	SC_WAIT(30),
	SC_MOVE(112, 0, 624),
	SC_SYNC,
	SC_MOVE(136, 0, 600),
	SC_SYNC,
	SC_MOVE(112, 0, 600),
	SC_SYNC,
	SC_MOVE(136, 0, 624),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(5),
	SC_ADD(SW_Y, toFIXED(4)),
	SC_WAIT(30),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	-1,
};

s32 IzumoRooms_SecondDancerScript[] = {
	SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
	SC_ACCELERATION(0.2),
	SC_SET(SW_GRAVITY, toFIXED(0.4)),
	SC_SPEED(0.6),
	SC_SET(SW_STEERING, 2),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_LABEL(1),
	SC_CALL(IzumoRooms_IsSecondDancerClear),
	SC_MOVE(176, 0, 624),
	SC_SYNC,
	SC_MOVE(176, 0, 600),
	SC_SYNC,
	SC_MOVE(152, 0, 600),
	SC_SYNC,
	SC_MOVE(152, 0, 624),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(5),
	SC_ADD(SW_Y, toFIXED(4)),
	SC_WAIT(30),
	SC_MOVE(176, 0, 624),
	SC_SYNC,
	SC_MOVE(152, 0, 600),
	SC_SYNC,
	SC_MOVE(176, 0, 600),
	SC_SYNC,
	SC_MOVE(152, 0, 624),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(5),
	SC_ADD(SW_Y, toFIXED(4)),
	SC_WAIT(30),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	-1,
};

/* clang-format off */
/* A girl in the Sanctum heaves with sobs ("*Sniff* How could we sacrifice our */
/* beloved Kushinada?"): her sprite is squashed and stretched three times, then */
/* rests. */
s32 IzumoRooms_HeavingScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ANIMATION(0),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(40),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(5),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
    /* Unused: a walk north and back again, which nothing starts. */
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
    SC_ACCELERATION(0.1),
    SC_SPEED(0.3),
    SC_ANIMATION(2),
    SC_LABEL(1),
    SC_MOVE(264, 0, 120),
    SC_SYNC,
    SC_WAIT(60),
    SC_MOVE(264, 0, 96),
    SC_SYNC,
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    -1,
};

/* Kushinada on her seat of stones heaves with sobs as the girl in the */
/* Sanctum does, with longer rests. */
s32 IzumoRooms_KushinadaHeavingScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(60),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(10),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
    /* Unused: a hop, which nothing starts. */
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.2),
    SC_SET(SW_GRAVITY, toFIXED(0.1)),
    SC_SPEED(0.4),
    SC_ANIMATION(0),
    SC_LABEL(1),
    SC_SET(SW_Y, toFIXED(4)),
    SC_WAIT(30),
    SC_END,
};
/* clang-format on */

#include "shared/scene_entrances_default.c"

u32 SceneExits[] = {
	SCENE_EXITS(IzumoRooms),
	SCENE_EXIT_UNLESS(IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_A, Izumo,
					  IZUMO_FROM_IZUMO_ROOMS_SOUTHEAST_A, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_B, Izumo,
					  IZUMO_FROM_IZUMO_ROOMS_SOUTHEAST_B, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(IZUMO_ROOMS_TO_IZUMO_SOUTHWEST, Izumo,
					  IZUMO_FROM_IZUMO_ROOMS_SOUTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(IZUMO_ROOMS_TO_IZUMO_SOUTH, Izumo,
					  IZUMO_FROM_IZUMO_ROOMS_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(IZUMO_ROOMS_TO_IZUMO_NORTHWEST, Izumo,
					  IZUMO_FROM_IZUMO_ROOMS_NORTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(IZUMO_ROOMS_TO_IZUMO_NORTHEAST, Izumo,
					  IZUMO_FROM_IZUMO_ROOMS_NORTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(IZUMO_ROOMS_TO_IZUMO_NORTH, Izumo,
					  IZUMO_FROM_IZUMO_ROOMS_NORTH, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE IzumoRooms_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 120, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 152, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 88, 0, 88, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 96, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 80, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 384, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 84, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 136, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 120, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KUSHINADA, CONDITION_ALWAYS, (s32)IzumoRooms_KushinadaHeavingScript, 352, 0, 328, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_CREAM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 362, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 108, 0, 320, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 632, 0, 88, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 560, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_GIRL, CONDITION_ALWAYS, (s32)IzumoRooms_HeavingScript, 504, 0, 360, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 392, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
/* clang-format on */

/* clang-format off */
ACTOR_PLACE IzumoRooms_SusasHouseActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LADY_UZUME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 520, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, (s32)IzumoRooms_FirstDancerScript, 136, 0, 624, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, (s32)IzumoRooms_SecondDancerScript, 152, 0, 624, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 90, 0, 622, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 198, 0, 622, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
/* clang-format on */

/* clang-format off */
ACTOR_PLACE IzumoRooms_ActorPlacementsOnceTheSerpentIsSlain[] = {
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_CREAM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 362, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 108, 0, 320, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 632, 0, 88, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 560, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_GIRL, CONDITION_ALWAYS, (s32)IzumoRooms_HeavingScript, 600, 0, 360, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 376, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_UZUME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 624, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 624, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 616, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 616, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KUSHINADA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 520, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUSA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 552, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_COAL), MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};
/* clang-format on */

TRIGGER IzumoRooms_Triggers[] = {
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_A, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_A},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_B, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_B},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTHWEST, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_SOUTHWEST},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTH, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_SOUTH},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTHWEST, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_NORTHWEST},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTHEAST, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_NORTHEAST},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTH, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_NORTH},
	{TRIGGER_STEP, 21, CONDITION_ALWAYS,
	 EVENT(scIzumoRoomsKushinadaAsksSusaNotToCome)},
	{TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgOkuniKushinadaAnd)},
	{TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgIfLadyUzume)},
	{TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgIMSoRelieved)},
	{TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgWhatIWant)},
	{TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAWhileBack)},
	{TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgThatDragonsbaneSeems)},
	{TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgICanTFind)},
	{TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgIAmOld)},
	{TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTheSerpentCan)},
	{TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(scIzumoRoomsTalkToMerchant)},
	{TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgOneDayKushinada)},
	{TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(scIzumoRoomsTalkToInnkeeper)},
	{TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgIfIWere)},
	{TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(scIzumoRoomsTalkToPriest)},
	{TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgSniffHowCould)},
	{TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgWhenIGet)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
	 MSGNO(MsgSusaSBeen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
	 MSGNO(MsgLadyUzumeS)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
	 MSGNO(MsgIOnlySuggested)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
	 MSGNO(MsgTheSerpentIs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
	 MSGNO(MsgIWonderIfAll)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
	 MSGNO(MsgTheSerpentWill)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
	 MSGNO(MsgAccordingToLegend)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
	 MSGNO(MsgOurGuardianSerpent)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
	 MSGNO(MsgThereSNo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
	 MSGNO(MsgThePowerThat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
	 MSGNO(MsgItWasThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
	 MSGNO(MsgThingsLikeThis)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
	 MSGNO(MsgWhatDoesThat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
	 MSGNO(MsgIfOnlyI)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
	 MSGNO(MsgIDonTWantThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
	 MSGNO(MsgNobodyWantsTo)},
	{TRIGGER_TABLE_END},
};

TRIGGER IzumoRooms_SusasHouseTriggers[] = {
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_A, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_A},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_B, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_B},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTHWEST, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_SOUTHWEST},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTH, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_SOUTH},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTHWEST, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_NORTHWEST},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTHEAST, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_NORTHEAST},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTH, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_NORTH},
	{TRIGGER_TALK, ACTOR_SUSAS_HOUSE_UZUME, CONDITION_ALWAYS,
	 EVENT(scIzumoRoomsAskAfterSusa)},
	{TRIGGER_TALK, ACTOR_SUSAS_HOUSE_FIRST_DANCER, CONDITION_ALWAYS,
	 MSGNO(MsgThereSNoPoint)},
	{TRIGGER_TALK, ACTOR_SUSAS_HOUSE_SECOND_DANCER, CONDITION_ALWAYS,
	 MSGNO(MsgTheSerpentAttacked)},
	{TRIGGER_TALK, ACTOR_SUSAS_HOUSE_WEST_VILLAGER, CONDITION_ALWAYS,
	 MSGNO(MsgTheSerpentAte)},
	{TRIGGER_TALK, ACTOR_SUSAS_HOUSE_EAST_VILLAGER, CONDITION_ALWAYS,
	 MSGNO(MsgTheSerpentLives)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSAS_HOUSE_UZUME,
	 CONDITION_ALWAYS, EVENT(scIzumoRoomsReadPrayerForSusa)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSAS_HOUSE_FIRST_DANCER,
	 CONDITION_ALWAYS, MSGNO(MsgOkuniSSuch)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSAS_HOUSE_SECOND_DANCER,
	 CONDITION_ALWAYS, MSGNO(MsgTheSerpentWould)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSAS_HOUSE_WEST_VILLAGER,
	 CONDITION_ALWAYS, MSGNO(MsgBeforeTheSerpent)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSAS_HOUSE_EAST_VILLAGER,
	 CONDITION_ALWAYS, MSGNO(MsgLegendHasIt)},
	{TRIGGER_TABLE_END},
};

TRIGGER IzumoRooms_TriggersOnceTheSerpentIsSlain[] = {
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_A, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_A},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_B, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_SOUTHEAST_B},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTHWEST, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_SOUTHWEST},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_SOUTH, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_SOUTH},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTHWEST, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_NORTHWEST},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTHEAST, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_NORTHEAST},
	{TRIGGER_EXIT, IZUMO_ROOMS_TO_IZUMO_NORTH, CONDITION_ALWAYS,
	 IZUMO_ROOMS_TO_IZUMO_NORTH},
	{TRIGGER_STEP, CELL_EAST_SIDE_OF_THE_HALL, FLAG_IZUMO_RECOGNIZED_THE_HEROES,
	 EVENT(scIzumoRoomsHeroesComeFromTheEast)},
	{TRIGGER_STEP, CELL_WEST_SIDE_OF_THE_HALL, FLAG_IZUMO_RECOGNIZED_THE_HEROES,
	 EVENT(scIzumoRoomsHeroesComeFromTheWest)},
	{TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(scIzumoRoomsTalkToMerchant)},
	{TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgMyGoodsAre)},
	{TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(scIzumoRoomsTalkToInnkeeper)},
	{TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAsLadyUzume)},
	{TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(scIzumoRoomsTalkToPriest)},
	{TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgNowThatTheSerpent)},
	{TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgIfYouReA)},
	{TRIGGER_TALK, ACTOR_UZUME, CONDITION_ALWAYS,
	 EVENT(scIzumoRoomsTalkToUzume)},
	{TRIGGER_TALK, ACTOR_FIRST_DANCER, CONDITION_ALWAYS,
	 EVENT(scIzumoRoomsAskAboutMtMikage)},
	{TRIGGER_TALK, ACTOR_SECOND_DANCER, CONDITION_ALWAYS,
	 MSGNO(MsgSupposedlyTheDoll)},
	{TRIGGER_TALK, ACTOR_WEST_VILLAGER, CONDITION_ALWAYS,
	 MSGNO(MsgLadyUzumeHas)},
	{TRIGGER_TALK, ACTOR_EAST_VILLAGER, CONDITION_ALWAYS,
	 MSGNO(MsgLadyUzumeSNew)},
	{TRIGGER_TALK, ACTOR_SAVED_KUSHINADA, CONDITION_ALWAYS,
	 MSGNO(MsgSomedaySusaAnd)},
	{TRIGGER_TALK, ACTOR_SUSA, CONDITION_ALWAYS, MSGNO(MsgIMSureYou)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
	 MSGNO(MsgIDLoveTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
	 MSGNO(MsgThisMayBe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
	 MSGNO(MsgIfBriggsS)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
	 MSGNO(MsgIUnderstandNow)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
	 MSGNO(MsgIThankThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
	 MSGNO(MsgMaybeNowKushinada)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
	 MSGNO(MsgThatDragonWanted)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_UZUME | TRIGGER_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS,
	 EVENT(scIzumoRoomsReadUzumesMind)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_DANCER, CONDITION_ALWAYS,
	 MSGNO(MsgWhyDoesLadyUzume)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_DANCER,
	 CONDITION_ALWAYS, MSGNO(MsgIWonderWhatSort)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_VILLAGER,
	 CONDITION_ALWAYS, MSGNO(MsgIHeardThat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_VILLAGER,
	 CONDITION_ALWAYS, MSGNO(MsgIDonTKnowWhatIt)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAVED_KUSHINADA,
	 CONDITION_ALWAYS, MSGNO(MsgIfSDescendants)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSA, CONDITION_ALWAYS,
	 MSGNO(MsgIMNo)},
	{USE_ITEM_ON_ACTOR(ITEM_DANCING_IDOL), ACTOR_UZUME, FLAG_UZUME_HAS_THE_DOLL,
	 EVENT(scIzumoRoomsGiveUzumeTheDoll)},
	{TRIGGER_TABLE_END},
};
