/* @file */
/* The insides of Yallam's huts and shops. One map holds four huts: the two */
/* houses behind the south-east and south doors, the inn behind the north-east
 */
/* door, and Sunshine's house and smithy behind the west door by the log. The */
/* other holds the three doors in the ledge: the weapon and armor shop (the */
/* west door, under the sword sign), the item shop (the north door) and an old
 */
/* couple's house (the east door). */
/* Sunshine the blacksmith lies in bed: "if it ain't a'rainin', I ain't */
/* a'workin'!", since without rain he cannot get into Taopo Swamp for his */
/* materials. Once the party carries a rusty weapon or a rare material, he gets
 */
/* up and forges it; after a night at the inn or a trip out of Yallam, his */
/* wife hands over what he made, for a price. */
#include "item_ids.h"
#include "version.h"
#include "field/actor_script.h"
#include "field/event_inline.h"
#include "field/field_sequence.h"
#include "field/field_state.h"
#include "game/game.h"
#include "scene/scene.h"
#include "window/message_argument.h"
#include "text/msg_ids.h"
#include "shop/shop.h"

/* Sunshine is at work on the party's material; a night at the inn, or leaving
 */
/* Yallam, finishes it. */
#define FLAG_SUNSHINE_FORGING FLAG_AREA_TEMP_01

/* The actors of SceneId_YallamRoomsInn, numbering the rows of */
/* YallamRoomsInnPlacements: its map holds two houses, the inn and Sunshine's */
/* house and smithy. */
typedef enum YALLAM_ROOMS_INN_ACTOR_ID {
	/* "Yepp had three different songs he used to sing." */
	ACTOR_YEPP_SONGS_VILLAGER = ACTOR_FIRST_PLACED,
	/* "Do you like children's songs?" (scYallamRoomsAskLikeChildrensSongs) */
	ACTOR_CHILDRENS_SONGS_VILLAGER,
	/* "It's been really dry the past few months." */
	ACTOR_DRY_MONTHS_VILLAGER,
	/* "We're supposed to be right in the middle of the rainy season..." */
	ACTOR_RAINY_SEASON_VILLAGER,
	ACTOR_INNKEEPER,
	/* "Taopo Swamp has dried up, and now it's inaccessible." */
	ACTOR_DRIED_SWAMP_VILLAGER,
	/* Sunshine the blacksmith, lying in bed until rain comes or the party */
	/* brings him something to forge. */
	ACTOR_SUNSHINE,
	ACTOR_SUNSHINES_WIFE,
	ACTOR_SUNSHINES_CHILD,
	/* An unseen marker at the far end of Sunshine's bed, so that he answers */
	/* from either end as he lies there. */
	ACTOR_SUNSHINES_BED
} YALLAM_ROOMS_INN_ACTOR_ID;

/* The shops' actors (SceneId_YallamRoomsShops), numbering the rows of */
/* YallamRoomsShopsPlacements. */
typedef enum YALLAM_ROOMS_SHOPS_ACTOR_ID {
	/* "Sunshine has started complaining that he can't work lately." */
	ACTOR_COMPLAINT_VILLAGER = ACTOR_FIRST_PLACED,
	/* "Our blacksmith, Sunshine, makes the kinds of items you can't..." */
	ACTOR_BLACKSMITH_VILLAGER,
	ACTOR_WEAPON_MERCHANT,
	ACTOR_ARMOR_MERCHANT,
	ACTOR_ITEM_MERCHANT,
	/* Unseen markers on the three counters, so that talking across one reaches
	 */
	/* its merchant. */
	/* @{ */
	ACTOR_WEAPON_COUNTER,
	ACTOR_ARMOR_COUNTER,
	ACTOR_ITEM_COUNTER
	/* @} */
} YALLAM_ROOMS_SHOPS_ACTOR_ID;

extern ACTOR_PLACE YallamRooms_Placements[];
extern ACTOR_PLACE YallamRoomsInnPlacements[];
extern ACTOR_PLACE YallamRoomsShopsPlacements[];
extern TRIGGER YallamRooms_Triggers[];
extern TRIGGER YallamRoomsInnTriggers[];
extern TRIGGER YallamRoomsShopsTriggers[];

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_YALLAM_ROOMS_INN)
		return YallamRoomsInnPlacements;
	if(scene == SCENE_YALLAM_ROOMS_SHOPS)
		return YallamRoomsShopsPlacements;
	return YallamRooms_Placements;
}

/* Sunshine the blacksmith thinks of the swamp he cannot reach without rain, */
/* and once the party has had him forge, of his joy at working. */
s32 scYallamRoomsMindReadSunshine(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(fdEventGetFlag(FLAG_SUNSHINE_FORGING)) {
		/* "It's been a long time since I've been able to work. I'm so
		 * excited!..." */
		fdEventSetMessageId((s32)MsgYallamRoomsItsBeenALong);
	}
	else {
		/* "If it's not raining, I can't get into Taopo Swamp. And if I
		 * can't..." */
		fdEventSetMessageId((s32)MsgYallamRoomsIfItsNotRaining);
	}
	fdEventShowMessage(ACTOR_SUNSHINE, 0);
	fdEventEnd();
	return 0;
}

/* Sunshine's wife thinks him stubborn, and once he forges, wonders what came */
/* over him. */
s32 scYallamRoomsMindReadSunshinesWife(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(fdEventGetFlag(FLAG_SUNSHINE_FORGING)) {
		/* "What happened to Sunshine? He's so happy, and he just talks
		 * about..." */
		fdEventSetMessageId((s32)MsgYallamRoomsWhatHappenedToSunshine);
	}
	else {
		/* "Sunshine's a pretty stubborn guy. If he says he isn't going to
		 * work..." */
		fdEventSetMessageId((s32)MsgYallamRoomsSunshinesAPrettyStubborn);
	}
	fdEventShowMessage(ACTOR_SUNSHINES_WIFE, 0);
	fdEventEnd();
	return 0;
}

/* Sunshine's child thinks of the artist in Dad, and once he forges, of Dad at
 */
/* work again. */
s32 scYallamRoomsMindReadSunshinesChild(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(fdEventGetFlag(FLAG_SUNSHINE_FORGING)) {
		/* "Now that my dad's working again, I have to watch his every move..."
		 */
		fdEventSetMessageId((s32)MsgYallamRoomsNowThatMyDads);
	}
	else {
		/* "Everyone thinks Dad is stubborn, but he's not... He's a true
		 * artist..." */
		fdEventSetMessageId((s32)MsgYallamRoomsEveryoneThinksDadIs);
	}
	fdEventShowMessage(ACTOR_SUNSHINES_CHILD, 0);
	fdEventEnd();
	return 0;
}

/* "Dad's not doing any work right now, but I'm making some basic things for */
/* the villagers." */
s32 scYallamRoomsTalkToSunshinesChild(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(fdEventGetFlag(FLAG_SUNSHINE_FORGING)) {
		/* "It's so weird, but something really inspired my father to work!..."
		 */
		fdEventSetMessageId((s32)MsgYallamRoomsItsSoWeirdBut);
	}
	else {
		/* "Dad's not doing any work right now, but I'm making some basic
		 * things..." */
		fdEventSetMessageId((s32)MsgYallamRoomsDadsNotDoingAny);
	}
	fdEventShowMessage(ACTOR_SUNSHINES_CHILD, 0);
	fdEventEnd();
	return 0;
}

/* The weapon merchant, read across the counter, thinks of the money Sunshine */
/* could make. */
s32 scYallamRoomsMindReadWeaponMerchant(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgYallamRoomsIfSunshineSetHis);
	/* "If Sunshine set his mind to it, he could make a lot of money! What a..."
	 */
	fdEventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
	fdEventEnd();
	return 0;
}

/* The armor merchant, read across the counter, finds his own stock boring. */
s32 scYallamRoomsMindReadArmorMerchant(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgYallamRoomsIShouldntTellHim);
	/* "I shouldn't tell him how boring all this stuff is. I wish Sunshine..."
	 */
	fdEventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
	fdEventEnd();
	return 0;
}

/* The item merchant, read across the counter, wonders what Sunshine wants in */
/* Taopo Swamp. */
s32 scYallamRoomsMindReadItemMerchant(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgYallamRoomsWhatDoesSunshineWant);
	/* "What does Sunshine want in Taopo Swamp? There's nothing up there but..."
	 */
	fdEventShowMessage(ACTOR_ITEM_MERCHANT, 0);
	fdEventEnd();
	return 0;
}

const TRIGGER *scGetTriggers(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_YALLAM_ROOMS_INN)
		return YallamRoomsInnTriggers;
	if(scene == SCENE_YALLAM_ROOMS_SHOPS)
		return YallamRoomsShopsTriggers;
	return YallamRooms_Triggers;
}

/* The woman pacing among the crates of the house behind Yallam's south-east */
/* door asks "Do you like children's songs?". Yes, and she says she doesn't */
/* care for them herself, lyrics that make no sense and a stupid dance; no, */
/* and she agrees she hates them and will write a better one some day. */
void scYallamRoomsAskLikeChildrensSongs(void) {
	s32 msg;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	msg = (s32)MsgYallamRoomsDoYouLikeChildrens;
	fdEventSetMessageId(msg);
	/* "Do you like children's songs?" */
	fdEventOpenMessage(ACTOR_CHILDRENS_SONGS_VILLAGER, 0);
	if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdEventWait(10);
		fdEventSetMessageId(msg + 1);
	}
	else {
		fdEventWait(20);
		fdEventSetMessageId(msg + 2);
	}
	fdEventShowMessage(ACTOR_CHILDRENS_SONGS_VILLAGER, 0);
	fdEventEnd();
}

/* Yallam's shops, as mnShopRun numbers them. */
enum {
	SHOP_YALLAM_WEAPONS = 20,
	SHOP_YALLAM_ARMOR = 21,
	SHOP_YALLAM_ITEMS = 22
};

/* Yallam's inn, as mnInnCheckIn numbers the inns. */
enum { YALLAM_INN = 7 };

/* Yallam's weapon merchant sells across the counter, and otherwise wishes */
/* Sunshine would work. */
void scYallamRoomsTalkToWeaponMerchant(s32 actor) {
	u16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnShopRun(SHOP_YALLAM_WEAPONS, actor);
	}
	else {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		fdEventSetMessageId((s32)MsgYallamRoomsIfTheBlacksmithIsnt);
		/* "If the blacksmith isn't willing to work, then how am I supposed
		 * to..." */
		fdEventShowMessage(actor, 0);
		fdEventEnd();
	}
}

/* Yallam's armor merchant sells across the counter, and otherwise sends the */
/* party to Sunshine for something special. */
void scYallamRoomsTalkToArmorMerchant(s32 actor) {
	u16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnShopRun(SHOP_YALLAM_ARMOR, actor);
	}
	else {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		fdEventSetMessageId((s32)MsgYallamRoomsIfYouWantPlain);
		/* "If you want plain old, boring armor, have a look around. If
		 * you're..." */
		fdEventShowMessage(actor, 0);
		fdEventEnd();
	}
}

/* Yallam's item merchant sells across the counter, and otherwise points the */
/* way to Taopo Swamp. */
void scYallamRoomsTalkToItemMerchant(s32 actor) {
	u16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnShopRun(SHOP_YALLAM_ITEMS, actor);
	}
	else {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		fdEventSetMessageId((s32)MsgYallamRoomsIfYoureLookingFor);
		/* "If you're looking for Taopo Swamp, head west around the northern..."
		 */
		fdEventShowMessage(actor, 0);
		fdEventEnd();
	}
}

/* Yallam's innkeeper takes the party in across the counter, and otherwise */
/* talks of the town that never gets tourists. */
void scYallamRoomsTalkToInnkeeper(s32 actor) {
	u16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnInnCheckIn(YALLAM_INN, actor);
		fdEventClearFlag(FLAG_SUNSHINE_FORGING);
	}
	else {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		fdEventSetMessageId((s32)MsgYallamRoomsItsABitStrange);
		/* "It's a bit strange trying to run an inn in a city that never
		 * gets..." */
		fdEventShowMessage(actor, 0);
		fdEventEnd();
	}
}

/* Unused: Yallam's sanctum is a room of MikasallaRoomsSanctum, whose own */
/* scMikasallaRoomsTalkToYallamHealer runs it, and nothing in Yallam's rooms */
/* calls this copy. The healer tends the party across the counter, and */
/* otherwise speaks of the hope a Sanctum brings to far-off shores. */
UNUSED void scYallamRoomsTalkToHealer(s32 actor) {
	u16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnSanctumRun(actor);
	}
	else {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		fdEventSetMessageId((s32)MsgEvenOutInFarOffShores);
		/* "Even out in these far-off shores, people need the services and
		 * hope..." */
		fdEventShowMessage(actor, 0);
		fdEventEnd();
	}
}

/* Unused: Sunshine asks whether the party wants something forged, and either */
/* way will only work in the rain; no trigger runs it. */
/* "Ah, so you want me to make you a little something special?" */
UNUSED void scYallamRoomsOfferToForge(void) {
	s32 msg;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorStartRepeatedMotion(ACTOR_SUNSHINE, 2);
	msg = (s32)MsgYallamRoomsAhSoYouWant;
	fdEventSetMessageId(msg);
	/* "Ah, so you want me to make you a little something special?" */
	fdEventOpenMessage(ACTOR_SUNSHINE, 0);
	if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdEventWait(10);
		fdEventSetMessageId(msg + 1);
	}
	else {
		fdEventWait(20);
		fdEventSetMessageId(msg + 2);
	}
	fdEventShowMessage(ACTOR_SUNSHINE, 0);
	fdEventEnd();
}

/* Whether anyone in the party carries something Sunshine can forge: a rusty */
/* weapon, or a material from Tear Stone to Orihalcon. */
s32 scYallamRoomsPartyHasForgeMaterial(void) {
	s32 item;

	for(item = ITEM_TEAR_STONE; item <= ITEM_ORIHALCON; item++) {
		if(gmPartyInventoryFindMember(item) != -1)
			return 1;
	}
	for(item = ITEM_RUSTY_SWORD; item <= ITEM_RUSTY_STAFF_GOBLINS_ROD; item++) {
		if(gmPartyInventoryFindMember(item) != -1)
			return 1;
	}
	return 0;
}

/* Sunshine, talked to as he lies in bed, asks whether the party is after a */
/* really unique item and either way won't work till it rains. Once the party */
/* carries a rusty weapon or a rare material he asks instead whether they want
 */
/* something special made: a yes, and he gets out of bed, meets Felix by the */
/* forge and picks what to work on, then walks off to the anvil as his child */
/* steps aside. While he works, or while his wife holds what he made, he says */
/* so. */
/* The forge stays open (`FLAG_FORGE_OPEN`) while he works or his wife holds */
/* the item, even once the party carries no more material. */
void scYallamRoomsTalkToSunshine(void) {
	s32 message;

	if(scYallamRoomsPartyHasForgeMaterial())
		fdEventSetFlag(FLAG_FORGE_OPEN);
	else if(!fdEventGetFlag(FLAG_SUNSHINE_FORGING) && !mnForgeGetItem())
		fdEventClearFlag(FLAG_FORGE_OPEN);

	if(!fdEventGetFlag(FLAG_FORGE_OPEN)) {
		message = (s32)MsgAhSoYou;
		fdEventSetMessageId(message);
		/* "Ah, so you're looking for a really unique item, huh?" */
		fdEventOpenMessage(ACTOR_SUNSHINE, 0);
		if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
			syFieldEventWait(10);
			fdEventSetMessageId(message + 1);
		}
		else {
			syFieldEventWait(20);
			fdEventSetMessageId(message + 2);
		}
		fdEventShowMessage(ACTOR_SUNSHINE, 0);
	}
	else if(fdEventGetFlag(FLAG_SUNSHINE_FORGING)) {
		fdEventSetMessageId((s32)MsgICanTWait);
		/* "I can't wait to see what I'm going to wind up making with
		 * this......" */
		fdEventShowMessage(ACTOR_SUNSHINE, 0);
	}
	else if(mnForgeGetItem()) {
		fdEventSetMessageId((s32)MsgMyWifeHas);
		/* "My wife has the item I made you. She can give it to you." */
		fdEventShowMessage(ACTOR_SUNSHINE, 0);
	}
	else {
		message = (s32)MsgYouWantMe;
		fdEventSetMessageId(message);
		/* "You want me to make you a little something special?" */
		fdEventOpenMessage(ACTOR_SUNSHINE, 0);
		if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
			fdEventSetMessageId(message + 1);
			/* "Well, look at that! You went to get some raw materials for
			 * me..." */
			fdEventShowMessage(ACTOR_SUNSHINE, 0);
			fdActorFallOverById(ACTOR_SUNSHINE, DEGtoANG(270));
			fdActorSetSprFlags(fdActorGetById(ACTOR_SUNSHINE),
							   SPRITE_FLAG_SHADOW);
			fdActorSetDestination(MainRoutineStateWork.leaderActorId, 488, 408);
			fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
			fdActorFaceDirection(MainRoutineStateWork.leaderActorId,
								 FACING_EAST, 0);
			fdActorSetDestination(ACTOR_SUNSHINE, 504, 408);
			fdActorWaitForMoveById(ACTOR_SUNSHINE);
			fdActorFaceActor(ACTOR_SUNSHINE, MainRoutineStateWork.leaderActorId,
							 0);
			mnForgeRun(ACTOR_SUNSHINE);
			if(mnForgeGetItem()) {
				fdActorSetSpeed(ACTOR_SUNSHINE, 0.4);
				fdActorWalkTo(ACTOR_SUNSHINE, 568, 408);
				fdActorWaitForMoveById(ACTOR_SUNSHINE);
				fdActorWalkTo(ACTOR_SUNSHINE, 568, 360);
				fdActorWaitForMoveById(ACTOR_SUNSHINE);
				fdActorWalkTo(ACTOR_SUNSHINES_CHILD, 632, 346);
				fdActorFaceDirection(ACTOR_SUNSHINES_CHILD, FACING_SOUTH, 0);
				fdActorSetAnimation(ACTOR_SUNSHINES_CHILD, ANIM_STILL);
				fdActorWalkTo(ACTOR_SUNSHINE, 600, 360);
				fdActorWaitForMoveById(ACTOR_SUNSHINE);
				fdActorSetAnimation(ACTOR_SUNSHINE, ANIM_STILL);
				fdActorFaceDirection(ACTOR_SUNSHINE, FACING_SOUTH, 0);
				fdActorGetById(ACTOR_SUNSHINE)->collisionFlg =
					ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID;
				fdActorGetById(ACTOR_SUNSHINE)->priFlg =
					ACTOR_PRIORITY_AUTOMATIC;
				fdActorSetSprPriority(ACTOR_SUNSHINE, 1);
				fdActorGetById(ACTOR_SUNSHINE)->priFlg |=
					ACTOR_PRIORITY_AUTOMATIC;
#if TLA_PATCH_1
				/* The bed no longer answers for him while he works. */
				fdActorSetPos(ACTOR_SUNSHINES_BED, 0, 0);
#endif
				fdEventSetFlag(FLAG_SUNSHINE_FORGING);
			}
			else {
				fdActorSetDestination(ACTOR_SUNSHINE, 491, 398);
				fdActorWaitForMoveById(ACTOR_SUNSHINE);
				fdActorFaceDirection(ACTOR_SUNSHINE, FACING_SOUTH, 0);
				fdActorSetSprFlags(fdActorGetById(ACTOR_SUNSHINE), 0);
				fdActorFallOverById(ACTOR_SUNSHINE, 0);
			}
		}
		else {
			fdEventSetMessageId(message + 2);
			/* "Hm... Maybe I made a mistake. It's hard to tell... If you
			 * want..." */
			fdEventShowMessage(ACTOR_SUNSHINE, 0);
		}
	}
}

/* Sunshine's wife keeps the smithy: she explains the forge, hands over what */
/* Sunshine made and takes the payment. */
/* "This? This is the blacksmith Sunshine's smithy, the best forge in Weyard!"
 */
void scYallamRoomsTalkToSunshinesWife(s32 actor) {
	if(!fdEventGetFlag(FLAG_FORGE_OPEN)) {
		fdEventSetMessageId((s32)MsgThisThisIs);
		/* "This? This is the blacksmith Sunshine's smithy, the best forge
		 * in..." */
		fdEventShowMessage(actor, 0);
	}
	else if(fdEventGetFlag(FLAG_SUNSHINE_FORGING)) {
		fdEventSetMessageId((s32)MsgItTakesOur);
		/* "It takes our Sunshine a long time to finish the items he works
		 * on...." */
		fdEventShowMessage(actor, 0);
	}
	else if(mnForgeGetItem()) {
		s32 item = mnForgeGetItem();
		u32 price = gmItemGetDirect(item)->price;
		s32 message;

		mnMessageArgumentPush(item, MESSAGE_ARGUMENT_ITEM);
		mnMessageArgumentPush(price, MESSAGE_ARGUMENT_NUMBER);
		message = (s32)MsgHereSThe;
		fdEventSetMessageId(message);
		/* "Here's the item you wanted. Now, about the payment... We only
		 * take..." */
		fdEventOpenMessage(actor, 0);
		if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) != 0) {
			fdEventSetMessageId(message + 1);
			/* "Hey, we're not a charity! If you can't pay for it, you don't get
			 * it!" */
			fdEventShowMessage(actor, 0);
		}
		else if(gmPartyInventoryFindRoom(item) < 0) {
			fdEventSetMessageId(message + 2);
			/* "Whoops! Looks like you've got a full load there! This'll have
			 * to..." */
			fdEventShowMessage(actor, 0);
		}
		else if(MainRoutineStateWork.coins < price) {
			fdEventSetMessageId(message + 3);
			/* "You don't enough money! What are you trying to pull? I can't
			 * give..." */
			fdEventShowMessage(actor, 0);
		}
		else {
			fdEventSetMessageId(message + 4);
			/* "OK, here you go!" */
			fdEventShowMessage(actor, 0);
			fdItemDispShowItem(item,
							   ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
			gmPartyGiveItem(item, 0);
			mnForgeSetItem(0);
			gmPartyAdjustCoins(-price);
		}
	}
	else {
		fdEventSetMessageId((s32)MsgManSunshineWould);
		/* "Man, Sunshine would be rich if he worked more. What a waste..." */
		fdEventShowMessage(actor, 0);
	}
}

/* Opens the houses through the window transition: in the inn Sunshine lies in
 */
/* bed or, while he forges, stands at work; in the shops the merchants are */
/* talked to across their counters. */
s32 scInit(void) {
	s32 scene;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	{
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

		scene = party->next;
	}
	if(scene == SCENE_YALLAM_ROOMS_INN) {
		if(fdEventGetFlag(FLAG_SUNSHINE_FORGING)) {
			fdActorSetPos(ACTOR_SUNSHINE, 600, 360);
			fdActorSetPos(ACTOR_SUNSHINES_CHILD, 632, 346);
			fdActorFaceDirection(ACTOR_SUNSHINES_CHILD, FACING_SOUTH, 0);
#if TLA_PATCH_1
			fdActorSetPos(ACTOR_SUNSHINES_BED, 0, 0);
#endif
		}
		else {
			fdActorFallOverById(ACTOR_SUNSHINE, 0);
		}
		fdActorSetSprFlags(fdActorGetById(ACTOR_SUNSHINES_BED), 0);
		fdActorSetSprPriority(ACTOR_RAINY_SEASON_VILLAGER, 2);
	}
	if(MainRoutineStateWork.next == SCENE_YALLAM_ROOMS_SHOPS) {
		fdEffectStartBlankOverlay(0);
		fdActorSetSprPriority(ACTOR_ITEM_MERCHANT, 2);
		fdActorGetById(ACTOR_ITEM_MERCHANT)->collisionFlg |= ACTOR_NEAR_TALK;
		fdActorSetSprPriority(ACTOR_WEAPON_MERCHANT, 2);
		fdActorGetById(ACTOR_WEAPON_MERCHANT)->collisionFlg |= ACTOR_NEAR_TALK;
		fdActorSetSprPriority(ACTOR_ARMOR_MERCHANT, 2);
		fdActorGetById(ACTOR_ARMOR_MERCHANT)->collisionFlg |= ACTOR_NEAR_TALK;
	}
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* clang-format off */
/* The woman who asks about children's songs paces among the crates of the */
/* house behind Yallam's south-east door. */
u32 YallamRooms_ChildrensSongsVillagerPacing[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
    SC_ACCELERATION(0.1),
    SC_SPEED(0.3),
    SC_LABEL(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(120, 0, 104),
    SC_SYNC,
    SC_MOVE(128, 0, 92),
    SC_SYNC,
    SC_WAIT(20),
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(20),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(120, 0, 104),
    SC_SYNC,
    SC_MOVE(88, 0, 104),
    SC_SYNC,
    SC_WAIT(20),
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
    /* Unused: a second walk, between two spots of the house behind the south */
    /* door, which no row takes. */
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.1),
    SC_SPEED(0.3),
    SC_LABEL(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(437, 0, 127),
    SC_SYNC,
    SC_WAIT(20),
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(60),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(453, 0, 114),
    SC_SYNC,
    SC_WAIT(20),
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

#include "shared/scene_entrances_default.c"

u32 SceneExits[] = {
    SCENE_EXITS(YallamRoomsInn),
    SCENE_EXIT_UNLESS(YALLAM_ROOMS_INN_TO_YALLAM_SOUTHEAST, Yallam, YALLAM_FROM_YALLAM_ROOMS_INN_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(YALLAM_ROOMS_INN_TO_YALLAM_SOUTH, Yallam, YALLAM_FROM_YALLAM_ROOMS_INN_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(YALLAM_ROOMS_INN_TO_YALLAM_NORTHEAST, Yallam, YALLAM_FROM_YALLAM_ROOMS_INN_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(YALLAM_ROOMS_INN_TO_YALLAM_WEST, Yallam, YALLAM_FROM_YALLAM_ROOMS_INN_WEST, EXIT_FLAG_NONE),
    SCENE_EXITS(YallamRoomsShops),
    SCENE_EXIT_UNLESS(YALLAM_ROOMS_SHOPS_TO_YALLAM_EAST, Yallam, YALLAM_FROM_YALLAM_ROOMS_SHOPS_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(YALLAM_ROOMS_SHOPS_TO_YALLAM_WEST, Yallam, YALLAM_FROM_YALLAM_ROOMS_SHOPS_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(YALLAM_ROOMS_SHOPS_TO_YALLAM_NORTH, Yallam, YALLAM_FROM_YALLAM_ROOMS_SHOPS_NORTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/* Unused: the actors of a room that is neither the inn nor the shops, which */
/* this overlay has none of. */
ACTOR_PLACE YallamRooms_Placements[] = {
    {SCENE_TABLE_END},
};
/* clang-format on */

/* The two houses, the inn and Sunshine's house and smithy, in */
/* YALLAM_ROOMS_INN_ACTOR_ID order. */
ACTOR_PLACE YallamRoomsInnPlacements[] = {
#if !TLA_PATCH_2
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 152, 0, 136, FACING_SOUTHWEST,
					TALK_FACING_PARTY_AND_BACK, 0),
#else
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 152, 0, 140, FACING_SOUTHWEST,
					TALK_FACING_PARTY_AND_BACK, 0),
#endif
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
					(s32)YallamRooms_ChildrensSongsVillagerPacing, 88, 0, 104,
					FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 424, 0, 88, FACING_SOUTHEAST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 454, 0, 120, FACING_SOUTHEAST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_INNKEEPER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					294, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
					184, 0, 392, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 491,
					0, 398, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_WIFE, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 560, 0, 440, FACING_WEST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 600, 0, 346, FACING_NORTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					506, 0, 398, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

/* clang-format off */
/* The shops, in YALLAM_ROOMS_SHOPS_ACTOR_ID order. */
ACTOR_PLACE YallamRoomsShopsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 188, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 405, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 405, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 348, 0, 405, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 348, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/* Unused: the exits of a room that is neither the inn nor the shops. */
TRIGGER YallamRooms_Triggers[] = {
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_TABLE_END},
};
/* clang-format on */

/* The inn's exits, its people talked to and read, and the forge's oven. */
TRIGGER YallamRoomsInnTriggers[] = {
	{TRIGGER_EXIT, YALLAM_ROOMS_INN_TO_YALLAM_SOUTHEAST, CONDITION_ALWAYS,
	 YALLAM_ROOMS_INN_TO_YALLAM_SOUTHEAST},
	{TRIGGER_EXIT, YALLAM_ROOMS_INN_TO_YALLAM_SOUTH, CONDITION_ALWAYS,
	 YALLAM_ROOMS_INN_TO_YALLAM_SOUTH},
	{TRIGGER_EXIT, YALLAM_ROOMS_INN_TO_YALLAM_NORTHEAST, CONDITION_ALWAYS,
	 YALLAM_ROOMS_INN_TO_YALLAM_NORTHEAST},
	{TRIGGER_EXIT, YALLAM_ROOMS_INN_TO_YALLAM_WEST, CONDITION_ALWAYS,
	 YALLAM_ROOMS_INN_TO_YALLAM_WEST},
	{TRIGGER_TALK, ACTOR_YEPP_SONGS_VILLAGER, CONDITION_ALWAYS,
	 MSGNO(MsgYallamRoomsYeppHadThreeDifferent)},
	{TRIGGER_TALK, ACTOR_CHILDRENS_SONGS_VILLAGER, CONDITION_ALWAYS,
	 EVENT(scYallamRoomsAskLikeChildrensSongs)},
	{TRIGGER_TALK, ACTOR_DRY_MONTHS_VILLAGER, CONDITION_ALWAYS,
	 MSGNO(MsgYallamRoomsItsBeenReallyDry)},
	{TRIGGER_TALK, ACTOR_RAINY_SEASON_VILLAGER, CONDITION_ALWAYS,
	 MSGNO(MsgYallamRoomsWereSupposedToBe)},
	{TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
	 EVENT(scYallamRoomsTalkToInnkeeper)},
	{TRIGGER_TALK, ACTOR_DRIED_SWAMP_VILLAGER, CONDITION_ALWAYS,
	 MSGNO(MsgYallamRoomsTaopoSwampHasDried)},
	{TRIGGER_TALK, ACTOR_SUNSHINE, CONDITION_ALWAYS,
	 EVENT(scYallamRoomsTalkToSunshine)},
	{TRIGGER_TALK, ACTOR_SUNSHINES_WIFE, CONDITION_ALWAYS,
	 EVENT(scYallamRoomsTalkToSunshinesWife)},
	{TRIGGER_TALK, ACTOR_SUNSHINES_CHILD, CONDITION_ALWAYS,
	 EVENT(scYallamRoomsTalkToSunshinesChild)},
	{TRIGGER_TALK, ACTOR_SUNSHINES_BED, CONDITION_ALWAYS,
	 EVENT(scYallamRoomsTalkToSunshine)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YEPP_SONGS_VILLAGER,
	 CONDITION_ALWAYS, MSGNO(MsgYallamRoomsTheSongsAreSupposed)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILDRENS_SONGS_VILLAGER,
	 CONDITION_ALWAYS, MSGNO(MsgYallamRoomsIWonderWhyYepp)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DRY_MONTHS_VILLAGER,
	 CONDITION_ALWAYS, MSGNO(MsgYallamRoomsWhenTaopoSwampDries)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RAINY_SEASON_VILLAGER,
	 CONDITION_ALWAYS, MSGNO(MsgYallamRoomsICantStandThis)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
	 MSGNO(MsgYallamRoomsYallamWasAlwaysSuch)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DRIED_SWAMP_VILLAGER,
	 CONDITION_ALWAYS, MSGNO(MsgYallamRoomsThereWerentALot)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUNSHINE, CONDITION_ALWAYS,
	 EVENT(scYallamRoomsMindReadSunshine)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUNSHINES_WIFE,
	 CONDITION_ALWAYS, EVENT(scYallamRoomsMindReadSunshinesWife)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUNSHINES_CHILD,
	 CONDITION_ALWAYS, EVENT(scYallamRoomsMindReadSunshinesChild)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUNSHINES_BED,
	 CONDITION_ALWAYS, EVENT(scYallamRoomsMindReadSunshine)},
	{EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgYallamRoomsTheSteelRestingIn))},
	{TRIGGER_TABLE_END},
};

/* The shops' exits and their people, talked to and read; each merchant answers
 */
/* across his counter too. */
TRIGGER YallamRoomsShopsTriggers[] = {
	{TRIGGER_EXIT, YALLAM_ROOMS_SHOPS_TO_YALLAM_EAST, CONDITION_ALWAYS,
	 YALLAM_ROOMS_SHOPS_TO_YALLAM_EAST},
	{TRIGGER_EXIT, YALLAM_ROOMS_SHOPS_TO_YALLAM_WEST, CONDITION_ALWAYS,
	 YALLAM_ROOMS_SHOPS_TO_YALLAM_WEST},
	{TRIGGER_EXIT, YALLAM_ROOMS_SHOPS_TO_YALLAM_NORTH, CONDITION_ALWAYS,
	 YALLAM_ROOMS_SHOPS_TO_YALLAM_NORTH},
	{TRIGGER_TALK, ACTOR_COMPLAINT_VILLAGER, CONDITION_ALWAYS,
	 MSGNO(MsgYallamRoomsSunshineHasStartedComplaining)},
	{TRIGGER_TALK, ACTOR_BLACKSMITH_VILLAGER, CONDITION_ALWAYS,
	 MSGNO(MsgYallamRoomsOurBlacksmithSunshineMakes)},
	{TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
	 EVENT(scYallamRoomsTalkToWeaponMerchant)},
	{TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
	 EVENT(scYallamRoomsTalkToArmorMerchant)},
	{TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
	 EVENT(scYallamRoomsTalkToItemMerchant)},
	{TRIGGER_TALK, ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS,
	 EVENT(scYallamRoomsTalkToWeaponMerchant)},
	{TRIGGER_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS,
	 EVENT(scYallamRoomsTalkToArmorMerchant)},
	{TRIGGER_TALK, ACTOR_ITEM_COUNTER, CONDITION_ALWAYS,
	 EVENT(scYallamRoomsTalkToItemMerchant)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COMPLAINT_VILLAGER,
	 CONDITION_ALWAYS, MSGNO(MsgYallamRoomsWhatKindOfPerson)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BLACKSMITH_VILLAGER,
	 CONDITION_ALWAYS, MSGNO(MsgYallamRoomsSunshinesALittleEccentric)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT,
	 CONDITION_ALWAYS, MSGNO(MsgYallamRoomsIfSunshineSetHis)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT,
	 CONDITION_ALWAYS, MSGNO(MsgYallamRoomsIShouldntTellHim)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT,
	 CONDITION_ALWAYS, MSGNO(MsgYallamRoomsWhatDoesSunshineWant)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_COUNTER,
	 CONDITION_ALWAYS, EVENT(scYallamRoomsMindReadWeaponMerchant)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER,
	 CONDITION_ALWAYS, EVENT(scYallamRoomsMindReadArmorMerchant)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_COUNTER, CONDITION_ALWAYS,
	 EVENT(scYallamRoomsMindReadItemMerchant)},
	{TRIGGER_TABLE_END},
};
