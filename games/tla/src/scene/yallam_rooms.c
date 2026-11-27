/* The insides of Yallam's huts and shops. One map holds four huts: the two
   houses behind the south-east and south doors, the inn behind the north-east
   door, and Sunshine's house and smithy behind the west door by the log. The
   other holds the three doors in the ledge: the weapon and armor shop (the west
   door, under the sword sign), the item shop (the north door) and an old
   couple's house (the east door). Sunshine the blacksmith lies in bed: "if it
   ain't a'rainin', I ain't a'workin'!", since without rain he cannot get into
   Taopo Swamp for his materials. Once the party carries a rusty weapon or a
   rare material, he gets up and forges it; after a night at the inn or a trip
   out of Yallam, his wife hands over what he made, for a price. */
#include "item_ids.h"
#include "edition.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "field/field_mode.h"
#include "field/field_state.h"
#include "game/game.h"
#include "scene/scene.h"
#include "window/message_argument.h"
#include "text/msg_ids.h"
#include "shop/shop.h"

/* Sunshine is at work on the party's material; a night at the inn, or leaving
   Yallam, finishes it. */
#define FLAG_SUNSHINE_FORGING FLAG_AREA_TEMP_01

/* The actors of SceneIdYallamRoomsInn, numbering the rows of
   YallamRoomsInnPlacements: its map holds two houses, the inn and Sunshine's
   house and smithy. */
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
	/* Sunshine the blacksmith, lying in bed until rain comes or the party
	   brings him something to forge. */
	ACTOR_SUNSHINE,
	ACTOR_SUNSHINES_WIFE,
	ACTOR_SUNSHINES_CHILD,
	/* An unseen marker at the far end of Sunshine's bed, so that he answers
	   from either end as he lies there. */
	ACTOR_SUNSHINES_BED
} YALLAM_ROOMS_INN_ACTOR_ID;

/* The shops' actors (SceneIdYallamRoomsShops), numbering the rows of
   YallamRoomsShopsPlacements. */
typedef enum YALLAM_ROOMS_SHOPS_ACTOR_ID {
	/* "Sunshine has started complaining that he can't work lately." */
	ACTOR_COMPLAINT_VILLAGER = ACTOR_FIRST_PLACED,
	/* "Our blacksmith, Sunshine, makes the kinds of items you can't..." */
	ACTOR_BLACKSMITH_VILLAGER,
	ACTOR_WEAPON_MERCHANT,
	ACTOR_ARMOR_MERCHANT,
	ACTOR_ITEM_MERCHANT,
	/* Unseen markers on the three counters, so that talking across one reaches
	   its merchant. */
	ACTOR_WEAPON_COUNTER,
	ACTOR_ARMOR_COUNTER,
	ACTOR_ITEM_COUNTER
} YALLAM_ROOMS_SHOPS_ACTOR_ID;

extern ACTOR_PLACE YallamRoomsPlacements[];
extern ACTOR_PLACE YallamRoomsInnPlacements[];
extern ACTOR_PLACE YallamRoomsShopsPlacements[];
extern CUE		   YallamRoomsCues[];
extern CUE		   YallamRoomsInnCues[];
extern CUE		   YallamRoomsShopsCues[];

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

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_YALLAM_ROOMS_INN)
		return YallamRoomsInnPlacements;
	if(scene == SCENE_YALLAM_ROOMS_SHOPS)
		return YallamRoomsShopsPlacements;
	return YallamRoomsPlacements;
}

/* Sunshine the blacksmith thinks of the swamp he cannot reach without rain, and
   once the party has had him forge, of his joy at working. */
Sint32 scYallamRoomsMindReadSunshine(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if(fdScriptGetFlag(FLAG_SUNSHINE_FORGING)) {
		/* "It's been a long time since I've been able to work. I'm so
		   excited!..." */
		fdScriptSetMessageId((Sint32)MsgYallamRoomsItsBeenALong);
	} else {
		/* "If it's not raining, I can't get into Taopo Swamp. And if I
		   can't..." */
		fdScriptSetMessageId((Sint32)MsgYallamRoomsIfItsNotRaining);
	}
	fdScriptShowMessage(ACTOR_SUNSHINE, 0);
	fdScriptEnd();
	return 0;
}

/* Sunshine's wife thinks him stubborn, and once he forges, wonders what came
   over him. */
Sint32 scYallamRoomsMindReadSunshinesWife(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if(fdScriptGetFlag(FLAG_SUNSHINE_FORGING)) {
		/* "What happened to Sunshine? He's so happy, and he just talks
		   about..." */
		fdScriptSetMessageId((Sint32)MsgYallamRoomsWhatHappenedToSunshine);
	} else {
		/* "Sunshine's a pretty stubborn guy. If he says he isn't going to
		   work..." */
		fdScriptSetMessageId((Sint32)MsgYallamRoomsSunshinesAPrettyStubborn);
	}
	fdScriptShowMessage(ACTOR_SUNSHINES_WIFE, 0);
	fdScriptEnd();
	return 0;
}

/* Sunshine's child thinks of the artist in Dad, and once he forges, of Dad at
   work again. */
Sint32 scYallamRoomsMindReadSunshinesChild(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if(fdScriptGetFlag(FLAG_SUNSHINE_FORGING)) {
		/* "Now that my dad's working again, I have to watch his every
		   move..." */
		fdScriptSetMessageId((Sint32)MsgYallamRoomsNowThatMyDads);
	} else {
		/* "Everyone thinks Dad is stubborn, but he's not... He's a true
		   artist..." */
		fdScriptSetMessageId((Sint32)MsgYallamRoomsEveryoneThinksDadIs);
	}
	fdScriptShowMessage(ACTOR_SUNSHINES_CHILD, 0);
	fdScriptEnd();
	return 0;
}

/* "Dad's not doing any work right now, but I'm making some basic things for the
   villagers." */
Sint32 scYallamRoomsTalkToSunshinesChild(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if(fdScriptGetFlag(FLAG_SUNSHINE_FORGING)) {
		/* "It's so weird, but something really inspired my father to
		   work!..." */
		fdScriptSetMessageId((Sint32)MsgYallamRoomsItsSoWeirdBut);
	} else {
		/* "Dad's not doing any work right now, but I'm making some basic
		   things..." */
		fdScriptSetMessageId((Sint32)MsgYallamRoomsDadsNotDoingAny);
	}
	fdScriptShowMessage(ACTOR_SUNSHINES_CHILD, 0);
	fdScriptEnd();
	return 0;
}

/* The weapon merchant, read across the counter, thinks of the money Sunshine
   could make. */
Sint32 scYallamRoomsMindReadWeaponMerchant(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgYallamRoomsIfSunshineSetHis);
	/* "If Sunshine set his mind to it, he could make a lot of money! What
	   a..." */
	fdScriptShowMessage(ACTOR_WEAPON_MERCHANT, 0);
	fdScriptEnd();
	return 0;
}

/* The armor merchant, read across the counter, finds his own stock boring. */
Sint32 scYallamRoomsMindReadArmorMerchant(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgYallamRoomsIShouldntTellHim);
	/* "I shouldn't tell him how boring all this stuff is. I wish
	   Sunshine..." */
	fdScriptShowMessage(ACTOR_ARMOR_MERCHANT, 0);
	fdScriptEnd();
	return 0;
}

/* The item merchant, read across the counter, wonders what Sunshine wants in
   Taopo Swamp. */
Sint32 scYallamRoomsMindReadItemMerchant(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgYallamRoomsWhatDoesSunshineWant);
	/* "What does Sunshine want in Taopo Swamp? There's nothing up there
	   but..." */
	fdScriptShowMessage(ACTOR_ITEM_MERCHANT, 0);
	fdScriptEnd();
	return 0;
}

const CUE *scGetCues(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_YALLAM_ROOMS_INN)
		return YallamRoomsInnCues;
	if(scene == SCENE_YALLAM_ROOMS_SHOPS)
		return YallamRoomsShopsCues;
	return YallamRoomsCues;
}

/* The woman pacing among the crates of the house behind Yallam's south-east
   door asks "Do you like children's songs?". Yes, and she says she doesn't care
   for them herself, lyrics that make no sense and a stupid dance; no, and she
   agrees she hates them and will write a better one some day. */
void scYallamRoomsAskLikeChildrensSongs(void)
{
	Sint32 msg;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	msg = (Sint32)MsgYallamRoomsDoYouLikeChildrens;
	fdScriptSetMessageId(msg);
	/* "Do you like children's songs?" */
	fdScriptOpenMessage(ACTOR_CHILDRENS_SONGS_VILLAGER, 0);
	if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdScriptWait(10);
		fdScriptSetMessageId(msg + 1);
	} else {
		fdScriptWait(20);
		fdScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessage(ACTOR_CHILDRENS_SONGS_VILLAGER, 0);
	fdScriptEnd();
}

/* Yallam's shops, as mnShopRun numbers them. */
enum {
	SHOP_YALLAM_WEAPONS = 20,
	SHOP_YALLAM_ARMOR = 21,
	SHOP_YALLAM_ITEMS = 22
};

/* Yallam's inn, as mnInnCheckIn numbers the inns. */
enum { YALLAM_INN = 7 };

/* Yallam's weapon merchant sells across the counter, and otherwise wishes
   Sunshine would work. */
void scYallamRoomsTalkToWeaponMerchant(Sint32 actor)
{
	Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnShopRun(SHOP_YALLAM_WEAPONS, actor);
	} else {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgYallamRoomsIfTheBlacksmithIsnt);
		/* "If the blacksmith isn't willing to work, then how am I supposed
		   to..." */
		fdScriptShowMessage(actor, 0);
		fdScriptEnd();
	}
}

/* Yallam's armor merchant sells across the counter, and otherwise sends the
   party to Sunshine for something special. */
void scYallamRoomsTalkToArmorMerchant(Sint32 actor)
{
	Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnShopRun(SHOP_YALLAM_ARMOR, actor);
	} else {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgYallamRoomsIfYouWantPlain);
		/* "If you want plain old, boring armor, have a look around. If
		   you're..." */
		fdScriptShowMessage(actor, 0);
		fdScriptEnd();
	}
}

/* Yallam's item merchant sells across the counter, and otherwise points the way
   to Taopo Swamp. */
void scYallamRoomsTalkToItemMerchant(Sint32 actor)
{
	Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnShopRun(SHOP_YALLAM_ITEMS, actor);
	} else {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgYallamRoomsIfYoureLookingFor);
		/* "If you're looking for Taopo Swamp, head west around the
		   northern..." */
		fdScriptShowMessage(actor, 0);
		fdScriptEnd();
	}
}

/* Yallam's innkeeper takes the party in across the counter, and otherwise talks
   of the town that never gets tourists. */
void scYallamRoomsTalkToInnkeeper(Sint32 actor)
{
	Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnInnCheckIn(YALLAM_INN, actor);
		fdScriptClearFlag(FLAG_SUNSHINE_FORGING);
	} else {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgYallamRoomsItsABitStrange);
		/* "It's a bit strange trying to run an inn in a city that never
		   gets..." */
		fdScriptShowMessage(actor, 0);
		fdScriptEnd();
	}
}

/* Unused: Yallam's sanctum is a room of MikasallaRoomsSanctum, whose own
   scMikasallaRoomsTalkToYallamHealer runs it, and nothing in Yallam's rooms
   calls this copy. The healer tends the party across the counter, and otherwise
   speaks of the hope a Sanctum brings to far-off shores. */
UNUSED void scYallamRoomsTalkToHealer(Sint32 actor)
{
	Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnSanctumRun(actor);
	} else {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgEvenOutInFarOffShores);
		/* "Even out in these far-off shores, people need the services and
		   hope..." */
		fdScriptShowMessage(actor, 0);
		fdScriptEnd();
	}
}

/* Unused: Sunshine asks whether the party wants something forged, and either
   way will only work in the rain; no cue runs it. "Ah, so you want me to make
   you a little something special?" */
UNUSED void scYallamRoomsOfferToForge(void)
{
	Sint32 msg;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorStartRepeatedMotion(ACTOR_SUNSHINE, 2);
	msg = (Sint32)MsgYallamRoomsAhSoYouWant;
	fdScriptSetMessageId(msg);
	/* "Ah, so you want me to make you a little something special?" */
	fdScriptOpenMessage(ACTOR_SUNSHINE, 0);
	if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdScriptWait(10);
		fdScriptSetMessageId(msg + 1);
	} else {
		fdScriptWait(20);
		fdScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessage(ACTOR_SUNSHINE, 0);
	fdScriptEnd();
}

/* Whether anyone in the party carries something Sunshine can forge: a rusty
   weapon, or a material from Tear Stone to Orihalcon. */
Bool scYallamRoomsPartyHasForgeMaterial(void)
{
	Sint32 item;

	for(item = ITEM_TEAR_STONE; item <= ITEM_ORIHALCON; item++) {
		if(gmPartyInventoryFindMember(item) != -1)
			return TRUE;
	}
	for(item = ITEM_RUSTY_SWORD; item <= ITEM_RUSTY_STAFF_GOBLINS_ROD; item++) {
		if(gmPartyInventoryFindMember(item) != -1)
			return TRUE;
	}
	return FALSE;
}

/* Sunshine, talked to as he lies in bed, asks whether the party is after a
   really unique item and either way won't work till it rains. Once the party
   carries a rusty weapon or a rare material he asks instead whether they want
   something special made: a yes, and he gets out of bed, meets Felix by the
   forge and picks what to work on, then walks off to the anvil as his child
   steps aside. While he works, or while his wife holds what he made, he says
   so. The forge stays open (`FLAG_FORGE_OPEN`) while he works or his wife holds
   the item, even once the party carries no more material. */
void scYallamRoomsTalkToSunshine(void)
{
	Sint32 msg;

	if(scYallamRoomsPartyHasForgeMaterial())
		fdScriptSetFlag(FLAG_FORGE_OPEN);
	else if(!fdScriptGetFlag(FLAG_SUNSHINE_FORGING) && !mnForgeGetItem())
		fdScriptClearFlag(FLAG_FORGE_OPEN);

	if(!fdScriptGetFlag(FLAG_FORGE_OPEN)) {
		msg = (Sint32)MsgAhSoYou;
		fdScriptSetMessageId(msg);
		/* "Ah, so you're looking for a really unique item, huh?" */
		fdScriptOpenMessage(ACTOR_SUNSHINE, 0);
		if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) ==
			0) {
			syFieldScriptWait(10);
			fdScriptSetMessageId(msg + 1);
		} else {
			syFieldScriptWait(20);
			fdScriptSetMessageId(msg + 2);
		}
		fdScriptShowMessage(ACTOR_SUNSHINE, 0);
	} else if(fdScriptGetFlag(FLAG_SUNSHINE_FORGING)) {
		fdScriptSetMessageId((Sint32)MsgICanTWait);
		/* "I can't wait to see what I'm going to wind up making with
		   this......" */
		fdScriptShowMessage(ACTOR_SUNSHINE, 0);
	} else if(mnForgeGetItem()) {
		fdScriptSetMessageId((Sint32)MsgMyWifeHas);
		/* "My wife has the item I made you. She can give it to you." */
		fdScriptShowMessage(ACTOR_SUNSHINE, 0);
	} else {
		msg = (Sint32)MsgYouWantMe;
		fdScriptSetMessageId(msg);
		/* "You want me to make you a little something special?" */
		fdScriptOpenMessage(ACTOR_SUNSHINE, 0);
		if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) ==
			0) {
			fdScriptSetMessageId(msg + 1);
			/* "Well, look at that! You went to get some raw materials for
			   me..." */
			fdScriptShowMessage(ACTOR_SUNSHINE, 0);
			fdActorFallOverById(ACTOR_SUNSHINE, DEGtoANG(270));
			fdActorSetSpriteFlags(
				fdActorGetById(ACTOR_SUNSHINE), SPRITE_FLAG_SHADOW);
			fdActorSetDestination(MainRoutineStateWork.leaderActorId, 488, 408);
			fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
			fdActorFaceDirection(
				MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
			fdActorSetDestination(ACTOR_SUNSHINE, 504, 408);
			fdActorWaitForMoveById(ACTOR_SUNSHINE);
			fdActorFaceActor(
				ACTOR_SUNSHINE, MainRoutineStateWork.leaderActorId, 0);
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
				fdActorGetById(ACTOR_SUNSHINE)->collisionFlags =
					ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID;
				fdActorGetById(ACTOR_SUNSHINE)->priorityFlags =
					ACTOR_PRIORITY_AUTOMATIC;
				fdActorSetSpritePriority(ACTOR_SUNSHINE, 1);
				fdActorGetById(ACTOR_SUNSHINE)->priorityFlags |=
					ACTOR_PRIORITY_AUTOMATIC;
#if SUNSHINE_BED_SILENT_WHILE_FORGING
				/* The bed no longer answers for him while he works. */
				fdActorSetPos(ACTOR_SUNSHINES_BED, 0, 0);
#endif
				fdScriptSetFlag(FLAG_SUNSHINE_FORGING);
			} else {
				fdActorSetDestination(ACTOR_SUNSHINE, 491, 398);
				fdActorWaitForMoveById(ACTOR_SUNSHINE);
				fdActorFaceDirection(ACTOR_SUNSHINE, FACING_SOUTH, 0);
				fdActorSetSpriteFlags(fdActorGetById(ACTOR_SUNSHINE), 0);
				fdActorFallOverById(ACTOR_SUNSHINE, 0);
			}
		} else {
			fdScriptSetMessageId(msg + 2);
			/* "Hm... Maybe I made a mistake. It's hard to tell... If you
			   want..." */
			fdScriptShowMessage(ACTOR_SUNSHINE, 0);
		}
	}
}

/* Sunshine's wife keeps the smithy: she explains the forge, hands over what
   Sunshine made and takes the payment. "This? This is the blacksmith Sunshine's
   smithy, the best forge in Weyard!" */
void scYallamRoomsTalkToSunshinesWife(Sint32 actor)
{
	if(!fdScriptGetFlag(FLAG_FORGE_OPEN)) {
		fdScriptSetMessageId((Sint32)MsgThisThisIs);
		/* "This? This is the blacksmith Sunshine's smithy, the best forge
		   in..." */
		fdScriptShowMessage(actor, 0);
	} else if(fdScriptGetFlag(FLAG_SUNSHINE_FORGING)) {
		fdScriptSetMessageId((Sint32)MsgItTakesOur);
		/* "It takes our Sunshine a long time to finish the items he works
		   on...." */
		fdScriptShowMessage(actor, 0);
	} else if(mnForgeGetItem()) {
		Sint32 item = mnForgeGetItem();
		Uint32 price = gmItemGetDirect(item)->price;
		Sint32 msg;

		mnMessageArgumentPush(item, MESSAGE_ARGUMENT_ITEM);
		mnMessageArgumentPush(price, MESSAGE_ARGUMENT_NUMBER);
		msg = (Sint32)MsgHereSThe;
		fdScriptSetMessageId(msg);
		/* "Here's the item you wanted. Now, about the payment... We only
		   take..." */
		fdScriptOpenMessage(actor, 0);
		if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) !=
			0) {
			fdScriptSetMessageId(msg + 1);
			/* "Hey, we're not a charity! If you can't pay for it, you don't get
			   it!" */
			fdScriptShowMessage(actor, 0);
		} else if(gmPartyInventoryFindRoom(item) < 0) {
			fdScriptSetMessageId(msg + 2);
			/* "Whoops! Looks like you've got a full load there! This'll have
			   to..." */
			fdScriptShowMessage(actor, 0);
		} else if(MainRoutineStateWork.coins < price) {
			fdScriptSetMessageId(msg + 3);
			/* "You don't enough money! What are you trying to pull? I can't
			   give..." */
			fdScriptShowMessage(actor, 0);
		} else {
			fdScriptSetMessageId(msg + 4);
			/* "OK, here you go!" */
			fdScriptShowMessage(actor, 0);
			fdItemDisplayShowItem(
				item, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
			gmPartyGiveItem(item, 0);
			mnForgeSetItem(0);
			gmPartyAdjustCoins(-price);
		}
	} else {
		fdScriptSetMessageId((Sint32)MsgManSunshineWould);
		/* "Man, Sunshine would be rich if he worked more. What a waste..." */
		fdScriptShowMessage(actor, 0);
	}
}

/* Opens the houses through the window transition: in the inn Sunshine lies in
   bed or, while he forges, stands at work; in the shops the merchants are
   talked to across their counters. */
Sint32 scInit(void)
{
	Sint32 scene;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	{
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

		scene = party->next;
	}
	if(scene == SCENE_YALLAM_ROOMS_INN) {
		if(fdScriptGetFlag(FLAG_SUNSHINE_FORGING)) {
			fdActorSetPos(ACTOR_SUNSHINE, 600, 360);
			fdActorSetPos(ACTOR_SUNSHINES_CHILD, 632, 346);
			fdActorFaceDirection(ACTOR_SUNSHINES_CHILD, FACING_SOUTH, 0);
#if SUNSHINE_BED_SILENT_WHILE_FORGING
			fdActorSetPos(ACTOR_SUNSHINES_BED, 0, 0);
#endif
		} else {
			fdActorFallOverById(ACTOR_SUNSHINE, 0);
		}
		fdActorSetSpriteFlags(fdActorGetById(ACTOR_SUNSHINES_BED), 0);
		fdActorSetSpritePriority(ACTOR_RAINY_SEASON_VILLAGER, 2);
	}
	if(MainRoutineStateWork.next == SCENE_YALLAM_ROOMS_SHOPS) {
		fdEffectStartBlankOverlay(0);
		fdActorSetSpritePriority(ACTOR_ITEM_MERCHANT, 2);
		fdActorGetById(ACTOR_ITEM_MERCHANT)->collisionFlags |= ACTOR_NEAR_TALK;
		fdActorSetSpritePriority(ACTOR_WEAPON_MERCHANT, 2);
		fdActorGetById(ACTOR_WEAPON_MERCHANT)->collisionFlags |=
			ACTOR_NEAR_TALK;
		fdActorSetSpritePriority(ACTOR_ARMOR_MERCHANT, 2);
		fdActorGetById(ACTOR_ARMOR_MERCHANT)->collisionFlags |= ACTOR_NEAR_TALK;
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* clang-format off */
/* The woman who asks about children's songs paces among the crates of the */
/* house behind Yallam's south-east door. */
Uint32 YallamRoomsChildrensSongsVillagerPacing[] = {
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

Uint32 SceneExits[] = {
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
ACTOR_PLACE YallamRoomsPlacements[] = {
	{SCENE_TABLE_END},
};
/* clang-format on */

/* The two houses, the inn and Sunshine's house and smithy, in
   YALLAM_ROOMS_INN_ACTOR_ID order. */
ACTOR_PLACE YallamRoomsInnPlacements[] = {
#if !YEPP_SONGS_VILLAGER_FURTHER_SOUTH
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 152, 0, 136, FACING_SOUTHWEST,
		TALK_FACING_PARTY_AND_BACK, 0),
#else
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 152, 0, 140, FACING_SOUTHWEST,
		TALK_FACING_PARTY_AND_BACK, 0),
#endif
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
		(Sint32)YallamRoomsChildrensSongsVillagerPacing, 88, 0, 104,
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
CUE YallamRoomsCues[] = {
	{CUE_EXIT, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{CUE_TABLE_END},
};
/* clang-format on */

/* The inn's exits, its people talked to and read, and the forge's oven. */
CUE YallamRoomsInnCues[] = {
	{CUE_EXIT, YALLAM_ROOMS_INN_TO_YALLAM_SOUTHEAST, CONDITION_ALWAYS,
		YALLAM_ROOMS_INN_TO_YALLAM_SOUTHEAST},
	{CUE_EXIT, YALLAM_ROOMS_INN_TO_YALLAM_SOUTH, CONDITION_ALWAYS,
		YALLAM_ROOMS_INN_TO_YALLAM_SOUTH},
	{CUE_EXIT, YALLAM_ROOMS_INN_TO_YALLAM_NORTHEAST, CONDITION_ALWAYS,
		YALLAM_ROOMS_INN_TO_YALLAM_NORTHEAST},
	{CUE_EXIT, YALLAM_ROOMS_INN_TO_YALLAM_WEST, CONDITION_ALWAYS,
		YALLAM_ROOMS_INN_TO_YALLAM_WEST},
	{CUE_TALK, ACTOR_YEPP_SONGS_VILLAGER, CONDITION_ALWAYS,
		MSGNO(MsgYallamRoomsYeppHadThreeDifferent)},
	{CUE_TALK, ACTOR_CHILDRENS_SONGS_VILLAGER, CONDITION_ALWAYS,
		SCRIPT_REF(scYallamRoomsAskLikeChildrensSongs)},
	{CUE_TALK, ACTOR_DRY_MONTHS_VILLAGER, CONDITION_ALWAYS,
		MSGNO(MsgYallamRoomsItsBeenReallyDry)},
	{CUE_TALK, ACTOR_RAINY_SEASON_VILLAGER, CONDITION_ALWAYS,
		MSGNO(MsgYallamRoomsWereSupposedToBe)},
	{CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(scYallamRoomsTalkToInnkeeper)},
	{CUE_TALK, ACTOR_DRIED_SWAMP_VILLAGER, CONDITION_ALWAYS,
		MSGNO(MsgYallamRoomsTaopoSwampHasDried)},
	{CUE_TALK, ACTOR_SUNSHINE, CONDITION_ALWAYS,
		SCRIPT_REF(scYallamRoomsTalkToSunshine)},
	{CUE_TALK, ACTOR_SUNSHINES_WIFE, CONDITION_ALWAYS,
		SCRIPT_REF(scYallamRoomsTalkToSunshinesWife)},
	{CUE_TALK, ACTOR_SUNSHINES_CHILD, CONDITION_ALWAYS,
		SCRIPT_REF(scYallamRoomsTalkToSunshinesChild)},
	{CUE_TALK, ACTOR_SUNSHINES_BED, CONDITION_ALWAYS,
		SCRIPT_REF(scYallamRoomsTalkToSunshine)},
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
		SCRIPT_REF(scYallamRoomsMindReadSunshine)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUNSHINES_WIFE,
		CONDITION_ALWAYS, SCRIPT_REF(scYallamRoomsMindReadSunshinesWife)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUNSHINES_CHILD,
		CONDITION_ALWAYS, SCRIPT_REF(scYallamRoomsMindReadSunshinesChild)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUNSHINES_BED,
		CONDITION_ALWAYS, SCRIPT_REF(scYallamRoomsMindReadSunshine)},
	{EXAMINE_CUE(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgYallamRoomsTheSteelRestingIn))},
	{CUE_TABLE_END},
};

/* The shops' exits and their people, talked to and read; each merchant answers
   across his counter too. */
CUE YallamRoomsShopsCues[] = {
	{CUE_EXIT, YALLAM_ROOMS_SHOPS_TO_YALLAM_EAST, CONDITION_ALWAYS,
		YALLAM_ROOMS_SHOPS_TO_YALLAM_EAST},
	{CUE_EXIT, YALLAM_ROOMS_SHOPS_TO_YALLAM_WEST, CONDITION_ALWAYS,
		YALLAM_ROOMS_SHOPS_TO_YALLAM_WEST},
	{CUE_EXIT, YALLAM_ROOMS_SHOPS_TO_YALLAM_NORTH, CONDITION_ALWAYS,
		YALLAM_ROOMS_SHOPS_TO_YALLAM_NORTH},
	{CUE_TALK, ACTOR_COMPLAINT_VILLAGER, CONDITION_ALWAYS,
		MSGNO(MsgYallamRoomsSunshineHasStartedComplaining)},
	{CUE_TALK, ACTOR_BLACKSMITH_VILLAGER, CONDITION_ALWAYS,
		MSGNO(MsgYallamRoomsOurBlacksmithSunshineMakes)},
	{CUE_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
		SCRIPT_REF(scYallamRoomsTalkToWeaponMerchant)},
	{CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
		SCRIPT_REF(scYallamRoomsTalkToArmorMerchant)},
	{CUE_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
		SCRIPT_REF(scYallamRoomsTalkToItemMerchant)},
	{CUE_TALK, ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS,
		SCRIPT_REF(scYallamRoomsTalkToWeaponMerchant)},
	{CUE_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS,
		SCRIPT_REF(scYallamRoomsTalkToArmorMerchant)},
	{CUE_TALK, ACTOR_ITEM_COUNTER, CONDITION_ALWAYS,
		SCRIPT_REF(scYallamRoomsTalkToItemMerchant)},
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
		CONDITION_ALWAYS, SCRIPT_REF(scYallamRoomsMindReadWeaponMerchant)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER,
		CONDITION_ALWAYS, SCRIPT_REF(scYallamRoomsMindReadArmorMerchant)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_COUNTER, CONDITION_ALWAYS,
		SCRIPT_REF(scYallamRoomsMindReadItemMerchant)},
	{CUE_TABLE_END},
};
