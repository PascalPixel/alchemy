/* The insides of Naribwe's huts, behind the village's five doors.

   The northwest hut's back door leads into the hut beside it, whose own door
   opens on the fenced back yard. The southwest hut is the shop, its weapon and
   armor merchants side by side behind one mat, and the southeast hut the inn.
   At the top of the village is the fortune-teller's hut, where for 20 coins he
   reads an item the party sets on his table: a weapon for the foe ahead, a
   piece of armor for a Djinni still to be found, a prong of the Trident for the
   prong still missing, the Trident for the beast it was forged against, a
   Psynergy item for the next power to gain, and anything else for the path to
   follow.

   Once Piers has carried Gabomba's message (`FLAG_GABOMBA_MESSAGE_CARRIED`) the
   villagers have other things to say (`NaribweRoomsCuesOnReturn`). */

#include "graphics/palette_buffers.h"
#include "window/message_argument.h"
#include "game/game.h"
#include "field/field_mode.h"
#include "field/field_mode_script.h"
#include "scene/scene.h"
#include "field/script_inline.h"
#include "edition.h"
#include "field/call.h"
#include "item_ids.h"
#include "text/msg_ids.h"
#include "shop/shop.h"

/* The placed actors, numbering the rows of `ActorPlacements`. */
typedef enum NARIBWE_ROOMS_ACTOR_ID {
	/* "The fortune-teller lives at the highest house in Naribwe..." */
	ACTOR_BOY_DIRECTING_TO_THE_FORTUNE_TELLER = ACTOR_FIRST_PLACED,
	/* "We're a small village, but if we all cooperate..." */
	ACTOR_WOMAN_ON_A_SMALL_VILLAGE,
	/* "I am a teller of fortunes. Would you have a reading?" */
	ACTOR_FORTUNE_TELLER,
	/* "My father is a fortune-teller, not a witch doctor." */
	ACTOR_FORTUNE_TELLERS_CHILD,
	/* "By painting our bodies, we bring about good luck." */
	ACTOR_VILLAGER_ON_BODY_PAINT,
	/* Behind the swords on the shop's mat; across it, the weapon shop. */
	ACTOR_WEAPON_MERCHANT,
	/* Behind the helms and shields beside them; across the mat, the armor
	   shop. */
	ACTOR_ARMOR_MERCHANT,
	/* "If you like adventure, you should head out west across the river..." */
	ACTOR_VILLAGER_ON_ADVENTURE,
	/* A woman behind the inn's counter; across it, the inn. */
	ACTOR_INNKEEPER,
	/* An unseen actor on the fortune-teller's table, which talking to him
	   across it reaches: "If you desire a reading, stand before the
	   table..." */
	ACTOR_FORTUNE_TABLE,
	/* The icon of the item the party sets on the table. */
	ACTOR_PLACED_ITEM,
	/* Thin wisps of smoke that rise from the table after the cloud. */
	ACTOR_RISING_SMOKE,
	/* A lilac cloud of smoke that bursts on the table as the dance ends. */
	ACTOR_SMOKE_CLOUD
} NARIBWE_ROOMS_ACTOR_ID;

/* The fortune-teller's dance: arms spread with his staff, the staff raised
   high, then a crouch, over and over. */
#define ANIM_FORTUNE_TELLER_DANCE 5

extern CUE NaribweRoomsCues[];
extern CUE NaribweRoomsCuesOnReturn[];

void scNaribweRoomsReadFortune(void);

void scNaribweRoomsReadProng(void);

void scNaribweRoomsReadWeapon(void);

void scNaribweRoomsReadArmor(void);

void scNaribweRoomsReadOtherItem(void);

/* "It costs 20 coins to hear what your future holds. Will you pay?" */
#define FORTUNE_PRICE 20

/* The Trident's reading, as messages after MsgNaribweRoomsAreYouPrepared. */
typedef enum TRIDENT_READING_MESSAGE {
	/* "That object you have placed before me... It is a fearsome weapon to a
	   particular beast." */
	TRIDENT_FEARSOME_WEAPON = 29,
	/* "If you wield this weapon, you can defeat the legendary beast!" */
	TRIDENT_DEFEAT_THE_BEAST = 30,
	/* "But there are no more beasts for you to fear in this part of the
	   world." */
	TRIDENT_NO_MORE_BEASTS = 31
} TRIDENT_READING_MESSAGE;

/* "From the garb you have placed before me, I sense the presence of the one you
   await.", which opens a Djinni's hint, as the message this many before
   MsgNaribweRoomsInItemYouHavePlaced. */
#define ARMOR_PRESENCE_SENSED_BEFORE 42

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
	return ActorPlacements;
}

/* Felix casts Mind Read on the fortune-teller across his table, in his hut at
   the top of Naribwe: "The items that one carries are filled with one's hopes
   and desires..." */
Sint32 scNaribweRoomsMindReadFortuneTeller(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsItemsThatOneCarries);
	/* "The items that one carries are filled with one's hopes and
	   desires...." */
	fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
	fdScriptEnd();
	return 0;
}

/* Felix casts Mind Read on the fortune-teller across his table once Piers has
   carried Gabomba's message: "Items placed before me are filled with the
   desires of those who..." */
Sint32 scNaribweRoomsMindReadFortuneTellerOnReturn(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsItemsPlacedBeforeMe);
	/* "Items placed before me are filled with the desires of those who..." */
	fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
	fdScriptEnd();
	return 0;
}

const CUE *scGetCues(void)
{
	if(fdScriptGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
		return NaribweRoomsCuesOnReturn;
	return NaribweRoomsCues;
}

/* The back door between the northwest hut and the west hut, whose own door
   opens on the back yard: each way's exit, and the entrance it arrives by. */
enum {
	NARIBWE_ROOMS_TO_WEST_HUT = 6,
	NARIBWE_ROOMS_TO_NORTHWEST_HUT = 7,
	NARIBWE_ROOMS_FROM_NORTHWEST_HUT = 7,
	NARIBWE_ROOMS_FROM_WEST_HUT = 6
};

/* Naribwe's shops, as `mnShopRun` numbers them. */
enum { SHOP_NARIBWE_WEAPONS = 14, SHOP_NARIBWE_ARMOR = 15 };

/* Naribwe's inn, as `mnInnCheckIn` numbers the inns. */
enum { NARIBWE_INN = 5 };

/* Felix talks to the weapon merchant behind the swords in Naribwe's shop:
   across the mat, he can buy; from elsewhere the merchant advises taking a
   weapon to the fortune-teller. */
void scNaribweRoomsTalkToWeaponMerchant(Sint32 speaker)
{
	Uint16 facing = fdActorGet(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnShopRun(SHOP_NARIBWE_WEAPONS, speaker);
	} else {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsIfYouAreBurdened);
		/* "If you are burdened with many concerns, take a weapon to the..." */
		fdScriptShowMessage(speaker, 0);
		fdScriptEnd();
	}
}

/* Felix talks to the weapon merchant in Naribwe's shop once Piers has carried
   Gabomba's message: across the mat, he can buy; from elsewhere the merchant
   sends out-of-town customers to the fortune-teller. */
void scNaribweRoomsTalkToWeaponMerchantOnReturn(Sint32 speaker)
{
	Uint16 facing = fdActorGet(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnShopRun(SHOP_NARIBWE_WEAPONS, speaker);
	} else {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsWheneverIGetCustomers);
		/* "Whenever I get customers from out of town, I always tell them
		   the..." */
		fdScriptShowMessage(speaker, 0);
		fdScriptEnd();
	}
}

/* Felix talks to the armor merchant behind the helms and shields in Naribwe's
   shop: across the mat, he can buy; from elsewhere the merchant advises
   visiting the fortune-teller while in the village. */
void scNaribweRoomsTalkToArmorMerchant(Sint32 speaker)
{
	Uint16 facing = fdActorGet(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnShopRun(SHOP_NARIBWE_ARMOR, speaker);
	} else {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		syFieldScriptSetMessageId(
			(Sint32)MsgNaribweRoomsYouShouldVisitFortuneTeller);
		/* "You should visit the fortune- teller while you are in our
		   village...." */
		fdScriptShowMessage(speaker, 0);
		fdScriptEnd();
	}
}

/* Felix talks to the armor merchant in Naribwe's shop once Piers has carried
   Gabomba's message: across the mat, he can buy; from elsewhere the merchant
   sends the party to the fortune-teller for anything it wishes to know. */
void scNaribweRoomsTalkToArmorMerchantOnReturn(Sint32 speaker)
{
	Uint16 facing = fdActorGet(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnShopRun(SHOP_NARIBWE_ARMOR, speaker);
	} else {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsIfYouWishToKnow);
		/* "If you wish to know anything, go to the fortune-teller. He's
		   fast..." */
		fdScriptShowMessage(speaker, 0);
		fdScriptEnd();
	}
}

/* Felix talks to the woman behind the counter of Naribwe's inn: across it, he
   can stay the night; from elsewhere she talks of the village's cooking. */
void scNaribweRoomsTalkToInnkeeper(Sint32 speaker)
{
	Uint16 facing = fdActorGet(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnInnCheckIn(NARIBWE_INN, speaker);
	} else {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsOurCookingIsFamousAll);
		/* "Our cooking is famous all across Gondowan. If you find you like
		   it..." */
		fdScriptShowMessage(speaker, 0);
		fdScriptEnd();
	}
}

/* Felix talks to the innkeeper once Piers has carried Gabomba's message: across
   the counter, he can stay the night; from elsewhere she sees too many
   travelers to remember Piers. */
void scNaribweRoomsTalkToInnkeeperOnReturn(Sint32 speaker)
{
	Uint16 facing = fdActorGet(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnInnCheckIn(NARIBWE_INN, speaker);
	} else {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsISeeSoManyTravelers);
		/* "I see so many travelers... How am I supposed to remember who
		   Piers..." */
		fdScriptShowMessage(speaker, 0);
		fdScriptEnd();
	}
}

/* Felix talks to the fortune-teller across his table, in his hut at the top of
   Naribwe: he offers a reading for 20 coins, and a No to either question ends
   it. */
void scNaribweRoomsTalkToFortuneTeller(void)
{
	Uint8 *msg;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	msg = MsgNaribweRoomsIAmTellerOfFortunes;
	syFieldScriptSetMessageId((Sint32)msg);
	/* "I am a teller of fortunes. Would you have a reading?" */
	fdScriptOpenMessage(ACTOR_FORTUNE_TELLER, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(
		   syFieldPartyGetLeaderActor(), 0) == 0) {
		syFieldScriptSetMessageId((Sint32)(msg + 2));
		/* "It costs 20 coins to hear what your future holds. Will you pay?" */
		fdScriptOpenMessage(ACTOR_FORTUNE_TELLER, 0);
		if(syFieldScriptAskYesNoWithActorAnimation(
			   syFieldPartyGetLeaderActor(), 0) == 0) {
			if(MainRoutineStateWork.coins > FORTUNE_PRICE - 1) {
				scNaribweRoomsReadFortune();
				fdScriptEnd();
			} else {
				syFieldScriptSetMessageId((Sint32)(msg + 4));
				/* "Many want things they cannot afford. If you do not have
				   the..." */
				fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
				fdScriptEnd();
			}
			return;
		}
	}
	syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsThenStandBack);
	/* "Then stand back." */
	fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
	fdScriptEnd();
}

void scNaribweRoomsReadShiningItem(void);

/* The fortune-teller steps sideways behind his table to x, still facing the
   party, and dances on. */
#define DANCE_TO(x) \
	(fdActorGet(ACTOR_FORTUNE_TELLER)->steeringFlg &= ~ACTOR_FACES_MOTION, \
		fdActorWalkTo(ACTOR_FORTUNE_TELLER, x, 96), \
		fdActorSetAnimation(ACTOR_FORTUNE_TELLER, ANIM_FORTUNE_TELLER_DANCE))

/* The reading: the party sets one of a member's items on the table, the room
   turns red while the fortune-teller dances, and he reads the item by its type,
   a prong of the Trident and the Trident itself on their own, then takes his 20
   coins. */
void scNaribweRoomsReadFortune(void)
{
	Sint32				   slot, unit;
	UNIT				  *unitData;
	const Uint8			  *message, *refuse;
	const ITEM_DEFINITION *info;
#if !FORTUNE_TELLER_READS_EQUIPPED_TRIDENT
	Sint32 item;
#else
	Uint32 item;
#endif
	Uint8  type;
	Sint32 rawType;

	syFieldScriptSetMessageId(MsgNaribweRoomsVeryWellShowMeOne);
	/* "Very well. Show me one of your items." */
	fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
	syFieldScriptWait(30);
	for(;;) {
#if !FORTUNE_TELLER_READS_EQUIPPED_TRIDENT
		fdActorSetAnimationAndWait(
			MainRoutineStateWork.leaderActorId, ANIM_NOD);
		/* In the Japanese edition the item variable first holds the choice's
		   result. */
		item = mnItemPickerRunToShow(&unit, &slot);
		if(item == -1) {
#else
		fdActorSetAnimationAndWait(
			MainRoutineStateWork.leaderActorId, ANIM_NOD);
		if(mnItemPickerRunToShow(&unit, &slot) == -1) {
#endif
			refuse = MsgNaribweRoomsShallIGiveYouAnother;
			syFieldScriptSetMessageId(refuse);
			/* "Shall I give you another reading for a different item?" */
			fdScriptOpenMessage(ACTOR_FORTUNE_TELLER, 0);
			if(syFieldScriptAskYesNoWithActorAnimation(
				   syFieldPartyGetLeaderActor(), 0) != 1)
				continue;
			syFieldScriptSetMessageId(refuse + 1);
			/* "Then away with you." */
			fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
			return;
		}
		unitData = syUnitGet(unit);
		gmItemGetDirect(unitData->inventory[slot]);
		fdActorSetItemIcon(
			fdActorGet(ACTOR_PLACED_ITEM), unitData->inventory[slot]);
		fdActorSetPos(ACTOR_PLACED_ITEM, 672, 104);
		syFieldScriptWait(60);
		mnMessageArgumentPush(unitData->inventory[slot], MESSAGE_ARGUMENT_ITEM);
		/* "... placed on the table." */
		mnMessageShowCentered(MsgPlacedOnThe, 1);
		message = MsgNaribweRoomsAreYouPrepared;
		syFieldScriptSetMessageId(message);
		/* "Are you prepared?" */
		fdScriptOpenMessage(ACTOR_FORTUNE_TELLER, 0);
		if(syFieldScriptAskYesNoWithActorAnimation(
			   syFieldPartyGetLeaderActor(), 0) == 1) {
			fdActorSetPos(ACTOR_PLACED_ITEM, 0, 0);
			mnMessageArgumentPush(
				unitData->inventory[slot], MESSAGE_ARGUMENT_ITEM);
			/* "... changed his mind and took back the item." */
			mnMessageShowCentered(message + 1, 1);
			syFieldScriptSetMessageId(message + 2);
			/* "Shall I give you another reading for a different item?" */
			fdScriptOpenMessage(ACTOR_FORTUNE_TELLER, 0);
			if(syFieldScriptAskYesNoWithActorAnimation(
				   syFieldPartyGetLeaderActor(), 0) != 1)
				continue;
			syFieldScriptSetMessageId(message + 3);
			/* "Then away with you." */
			fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
			return;
		}
		break;
	}
	syFieldScriptSetMessageId(message + 4);
	/* "I shall begin my reading..." */
	fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	syFieldScriptWait(30);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(13, 8, 9), PALETTES_BG);
	fdPaletteFadeInterpolate(30);
	syFieldScriptWait(120);
	fdScriptPlaySnd(MUS_RITUALS);
	fdActorSetAnimation(ACTOR_FORTUNE_TELLER, ANIM_FORTUNE_TELLER_DANCE);
	fdActorWaitForAnimationChange(ACTOR_FORTUNE_TELLER);
	DANCE_TO(664);
	syFieldScriptWait(30);
	DANCE_TO(680);
	scFieldActorJump(ACTOR_FORTUNE_TELLER, 4, 90);
	DANCE_TO(664);
	syFieldScriptWait(30);
	DANCE_TO(680);
	scFieldActorJump(ACTOR_FORTUNE_TELLER, 4, 90);
	DANCE_TO(672);
	scFieldActorJump(ACTOR_FORTUNE_TELLER, 4, 30);
	scFieldActorJump(ACTOR_FORTUNE_TELLER, 4, 30);
	fdActorSetAnimation(ACTOR_FORTUNE_TELLER, ANIM_FORTUNE_TELLER_DANCE);
	fdActorSetAnimation(ACTOR_FORTUNE_TELLER, ANIM_STILL);
	fdScriptPlaySnd(MUS_RITUALS);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
	fdPaletteFadeInterpolate(30);
	fdActorSetPos(ACTOR_SMOKE_CLOUD, 672, 104);
	fdScriptPlaySnd(SE_CLATTER);
	fdActorSetAnimation(ACTOR_SMOKE_CLOUD, 2);
	fdActorWaitForAnimationChange(ACTOR_SMOKE_CLOUD);
	fdActorSetPos(ACTOR_SMOKE_CLOUD, 0, 0);
	syFieldScriptWait(4);
	fdActorSetPos(ACTOR_RISING_SMOKE, 672, 104);
	fdActorSetAnimation(ACTOR_RISING_SMOKE, 4);
	fdActorWaitForAnimationChange(ACTOR_RISING_SMOKE);
	syFieldScriptWait(15);
	fdActorSetPos(ACTOR_RISING_SMOKE, 0, 0);
	syFieldScriptWait(30);
	fdActorShowEmote(
		MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	syFieldScriptWait(60);
	fdScriptPlaySnd(MUS_KIBOMBO);
	fdActorShowEmote(
		ACTOR_FORTUNE_TELLER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	/* "I see..." */
	fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
	unitData = syUnitGet(unit);
	info = gmItemGetDirect(unitData->inventory[slot]);
#if !FORTUNE_TELLER_READS_EQUIPPED_TRIDENT
	item = unitData->inventory[slot];
#else
	item = unitData->inventory[slot] & INVENTORY_ITEM_ID_MASK;
#endif
	if((Uint16)(item - ITEM_RIGHT_PRONG) <=
		ITEM_CENTER_PRONG - ITEM_RIGHT_PRONG) {
		scNaribweRoomsReadProng();
	} else if((Uint16)item == ITEM_TRIDENT) {
		syFieldScriptSetMessageId(message + TRIDENT_FEARSOME_WEAPON);
		/* "That object you have placed before me... It is a fearsome weapon
		   to..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		if(!fdScriptGetFlag(FLAG_SEA_OF_TIME_OPENED)) {
			syFieldScriptSetMessageId(message + TRIDENT_DEFEAT_THE_BEAST);
			/* "If you wield this weapon, you can defeat the legendary
			   beast!" */
			fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		} else {
			syFieldScriptSetMessageId(message + TRIDENT_NO_MORE_BEASTS);
			/* "But there are no more beasts for you to fear in this part of
			   the..." */
			fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		}
	} else {
		rawType = info->type;
		type = rawType;
		if(type == ITEM_TYPE_PSYNERGY)
			scNaribweRoomsReadShiningItem();
		else if(type == ITEM_TYPE_WEAPON)
			scNaribweRoomsReadWeapon();
		else if((Uint8)((Uint8)rawType - ITEM_TYPE_ARMOR) <=
			ITEM_TYPE_BOOTS - ITEM_TYPE_ARMOR)
			scNaribweRoomsReadArmor();
		else
			scNaribweRoomsReadOtherItem();
	}
	fdActorSetPos(ACTOR_PLACED_ITEM, 0, 0);
	gmPartyAdjustCoins(-FORTUNE_PRICE);
}

/* The fortune-teller's readings, one numbering for all four kinds: the foe a
   weapon shows, the power a Psynergy item shows, the path any other item shows
   and the prong a prong of the Trident shows. */
enum FORTUNE_READING {
	/* Before Gabomba's message is carried, for a weapon "Many enemies await
	   you, far beyond the mountains.", for any other item "I see you inside a
	   sacred icon." */
	READING_BEFORE_GABOMBAS_MESSAGE = 0,
	READING_FOE_IN_MISTY_SEA = 1, /* "In a misty sea, a mighty foe..." */
	READING_EAST_CONQUERED = 2,	  /* "None in the eastern world can stand..." */
	/* The first power still lacking: Reveal, Sand, Parch, the Burst Brooch, the
	   Grindstone, or none. */
	READING_SEEK_REVEAL = 3,
	READING_SEEK_SAND = 4,
	READING_SEEK_PARCH = 5,
	READING_SEEK_BURST_BROOCH = 6,
	READING_SEEK_GRINDSTONE = 7,
	READING_ALL_POWERS = 8,
	READING_FORGE_WEAPON = 9,  /* "You must forge a weapon..." */
	READING_RAISE_COLUMN = 10, /* "You will raise a mighty column..." */
	/* "...help from a village on the eastern..." */
	READING_EASTERN_VILLAGE = 11,
	READING_SEA_BEAST = 12, /* "You have the weapon you need..." */
	READING_WEST = 13,		/* "The object that you seek lies...west." */
	/* Where the prongs not yet held lie: the ruins by the beach, the
	   moss-covered ruins, the icy cape's tower, or all held. */
	READING_PRONG_BY_BEACH = 14,
	READING_PRONG_IN_MOSSY_RUINS = 15,
	READING_PRONG_IN_TOWER = 16,
	READING_ALL_PRONGS = 17
};

Sint32 scNaribweRoomsFoeAhead(void);
Sint32 scNaribweRoomsNextPowerToGain(void);
Sint32 scNaribweRoomsNextPath(void);
Sint32 scNaribweRoomsProngToSeek(void);
Uint8 *scNaribweRoomsGetDjinniHint(void);

/* A weapon: the foe that lies beyond it. */
void scNaribweRoomsReadWeapon(void)
{
	Uint8 *msg = MsgNaribweRoomsBeyondWeaponYouSetDown;

	syFieldScriptSetMessageId((Sint32)msg);
	/* "Beyond the weapon you set down lies a terrible foe." */
	fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
	switch(scNaribweRoomsFoeAhead()) {
	case READING_BEFORE_GABOMBAS_MESSAGE:
		syFieldScriptSetMessageId((Sint32)(msg + 1));
		/* "Many enemies await you, far beyond the mountains. If you move
		   to..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	case READING_FOE_IN_MISTY_SEA:
		syFieldScriptSetMessageId((Sint32)(msg + 2));
		/* "In a misty sea, a mighty foe... A fiersome battle awaits you..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	case READING_EAST_CONQUERED:
		syFieldScriptSetMessageId((Sint32)(msg + 3));
		/* "None in the eastern world can stand against the power you
		   possess." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	}
}

/* A Psynergy item: a power shining beyond it, and where the next power to gain
   waits. */
void scNaribweRoomsReadShiningItem(void)
{
	Uint8 *msg = MsgNaribweRoomsIFeelShiningPowerGlowing;

	syFieldScriptSetMessageId((Sint32)msg);
	/* "I feel a shining power glowing beyond the item before me." */
	fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
	switch(scNaribweRoomsNextPowerToGain()) {
	case READING_SEEK_REVEAL:
		syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsYouAreLost);
		/* "You are lost in your way... Another continent needs your
		   attention..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	case READING_SEEK_SAND:
		syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsDistantIsleLiesFar);
		/* "A distant isle lies far to the northeast... Sail there. High atop
		   a..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	case READING_SEEK_PARCH:
		syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsISeeParadiseAwaiting);
		/* "I see a paradise awaiting you... An island in the east. But
		   even..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	case READING_SEEK_BURST_BROOCH:
		syFieldScriptSetMessageId(
			(Sint32)MsgNaribweRoomsTinyFigureStandsAgainstWasteland);
		/* "A tiny figure stands against a wasteland wrought in snow...
		   The..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	case READING_SEEK_GRINDSTONE:
		syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsISeeYouLost);
		/* "I see you lost in mists, lost in time, lost in the seas...
		   And..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	default:
		syFieldScriptSetMessageId((Sint32)(msg + 6));
		/* "The eastern lands hold little for you now. You will not
		   remain..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	}
}

/* Armor, a shield, a helm or boots: where a Djinni still to be found waits, or
   nothing of the future once none is left. */
void scNaribweRoomsReadArmor(void)
{
	Sint32 msg = scNaribweRoomsGetDjinniHint();

	if(msg == (Sint32)MsgNaribweRoomsInItemYouHavePlaced) {
		syFieldScriptSetMessageId(msg);
		/* "In the item you have placed before me, I can see nothing of
		   the..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
	} else {
		syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsInItemYouHavePlaced -
			ARMOR_PRESENCE_SENSED_BEFORE);
		/* "From the garb you have placed before me, I sense the presence of
		   the..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		syFieldScriptSetMessageId(msg);
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
	}
}

/* Any other item: the path the party should follow. */
void scNaribweRoomsReadOtherItem(void)
{
	syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsBeyondItemBeforeMe);
	/* "Beyond the item before me, I see a path for you to follow." */
	fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
	switch(scNaribweRoomsNextPath()) {
	case READING_BEFORE_GABOMBAS_MESSAGE:
		syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsISeeYouInsideSacred);
		/* "I see you inside a sacred icon. By listening to the voices, you
		   will..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	case READING_FORGE_WEAPON:
		syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsYouMustForgeWeapon);
		/* "You must forge a weapon to defeat a mortal enemy. Seek out the
		   pieces!" */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	case READING_RAISE_COLUMN:
		syFieldScriptSetMessageId(
			(Sint32)MsgNaribweRoomsYouWillRaiseMightyColumn);
		/* "You will raise a mighty column and gain assistance where it is
		   least..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	case READING_EASTERN_VILLAGE:
		syFieldScriptSetMessageId(
			(Sint32)MsgNaribweRoomsYouWillGainTremendousHelp);
		/* "You will gain tremendous help from a village on the eastern
		   reaches..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	case READING_SEA_BEAST:
		syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsYouHaveWeaponYouNeed);
		/* "You have the weapon you need to defeat the sea beast. Go to the
		   sea..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	case READING_WEST:
		syFieldScriptSetMessageId((Sint32)MsgNaribweRoomsObjectThatYouSeekLies);
		/* "The object that you seek lies somewhere in the west. Why do
		   you..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	}
}

/* A prong of the Trident: the power it holds over a foe, and where a prong
   still missing lies. */
void scNaribweRoomsReadProng(void)
{
	Uint8 *msg = MsgNaribweRoomsObjectYouHavePlacedBefore;

	syFieldScriptSetMessageId((Sint32)msg);
	/* "The object you have placed before me wields a mighty power over a..." */
	fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
	switch(scNaribweRoomsProngToSeek()) {
	case READING_PRONG_BY_BEACH:
		syFieldScriptSetMessageId((Sint32)(msg + 1));
		/* "Near the beach where you washed ashore lie ancient ruins. Seek
		   them..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	case READING_PRONG_IN_MOSSY_RUINS:
		syFieldScriptSetMessageId((Sint32)(msg + 2));
		/* "In the northern reaches of the Eastern Sea lie moss-covered,..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	case READING_PRONG_IN_TOWER:
		syFieldScriptSetMessageId((Sint32)(msg + 3));
		/* "In the distant south lies an icy cape. Above the rocky shore
		   stands..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	case READING_ALL_PRONGS:
		syFieldScriptSetMessageId((Sint32)(msg + 4));
		/* "You have the weapon you need in many fragments.. Bring them to
		   the..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		break;
	}
}

/* Which foe a weapon's reading names: none before Gabomba's message is carried,
   then the foe in the misty sea, and none once Poseidon is beaten. */
Sint32 scNaribweRoomsFoeAhead(void)
{
	if(fdScriptGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) == 0)
		return READING_BEFORE_GABOMBAS_MESSAGE;
	return (fdScriptGetFlag(FLAG_SEA_OF_TIME_OPENED) != 0) +
		READING_FOE_IN_MISTY_SEA;
}

/* The first power the party still lacks, for a Psynergy item's reading: three
   Psynergy (Reveal, Sand and Parch) and two items (the Burst Brooch and the
   Grindstone), or READING_ALL_POWERS once it has them all. */
Sint32 scNaribweRoomsNextPowerToGain(void)
{
	if(gmPartyFindOwnerWithAction(ABILITY_REVEAL) == -1)
		return READING_SEEK_REVEAL;
	if(gmPartyFindOwnerWithAction(ABILITY_SAND) == -1)
		return READING_SEEK_SAND;
	if(gmPartyFindOwnerWithAction(ABILITY_PARCH) == -1)
		return READING_SEEK_PARCH;
	if(syFieldPartyInventoryFindMember(ITEM_BURST_BROOCH) == -1)
		return READING_SEEK_BURST_BROOCH;
	if(syFieldPartyInventoryFindMember(ITEM_GRINDSTONE) == -1)
		return READING_SEEK_GRINDSTONE;
	return READING_ALL_POWERS;
}

/* The path an ordinary item's reading points along, by the story's flags, the
   Trident's prongs and the Trident. */
Sint32 scNaribweRoomsNextPath(void)
{
	if(fdScriptGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) == 0)
		return READING_BEFORE_GABOMBAS_MESSAGE;
	if(syFieldPartyInventoryFindMember(ITEM_TRIDENT) == -1) {
		if(fdScriptGetFlag(FLAG_SEA_OF_TIME_OPENED) == 0) {
			if(syFieldPartyInventoryFindMember(ITEM_RIGHT_PRONG) == -1 ||
				syFieldPartyInventoryFindMember(ITEM_LEFT_PRONG) == -1 ||
				syFieldPartyInventoryFindMember(ITEM_CENTER_PRONG) == -1)
				return READING_FORGE_WEAPON;
			return (fdScriptGetFlag(FLAG_WORKERS_TOLD_MAYOR) != 0) +
				READING_RAISE_COLUMN;
		}
	} else {
		if(fdScriptGetFlag(FLAG_SEA_OF_TIME_OPENED) == 0)
			return READING_SEA_BEAST;
	}
	return READING_WEST;
}

/* Where a prong's reading sends the party: to the ruins by the beach while the
   Right Prong is missing, the moss-covered ruins while the Left Prong is, the
   icy cape's tower while only the Center Prong is, and on once it holds all
   three. */
Sint32 scNaribweRoomsProngToSeek(void)
{
	if(syFieldPartyInventoryFindMember(ITEM_RIGHT_PRONG) == -1 &&
		(syFieldPartyInventoryFindMember(ITEM_CENTER_PRONG) != -1 ||
			syFieldPartyInventoryFindMember(ITEM_LEFT_PRONG) != -1))
		return READING_PRONG_BY_BEACH;
	if(syFieldPartyInventoryFindMember(ITEM_RIGHT_PRONG) != -1 &&
		syFieldPartyInventoryFindMember(ITEM_LEFT_PRONG) == -1 &&
		syFieldPartyInventoryFindMember(ITEM_CENTER_PRONG) == -1)
		return READING_PRONG_IN_MOSSY_RUINS;
	if(syFieldPartyInventoryFindMember(ITEM_RIGHT_PRONG) != -1 &&
		syFieldPartyInventoryFindMember(ITEM_LEFT_PRONG) == -1 &&
		syFieldPartyInventoryFindMember(ITEM_CENTER_PRONG) != -1)
		return READING_PRONG_IN_MOSSY_RUINS;
	if(syFieldPartyInventoryFindMember(ITEM_RIGHT_PRONG) != -1 &&
		syFieldPartyInventoryFindMember(ITEM_LEFT_PRONG) != -1 &&
		syFieldPartyInventoryFindMember(ITEM_CENTER_PRONG) == -1)
		return READING_PRONG_IN_TOWER;
	if(syFieldPartyInventoryFindMember(ITEM_RIGHT_PRONG) != -1 &&
		syFieldPartyInventoryFindMember(ITEM_LEFT_PRONG) != -1 &&
		syFieldPartyInventoryFindMember(ITEM_CENTER_PRONG) != -1)
		return READING_ALL_PRONGS;
}

/* One past the last Djinni of an element the party has found, of those this
   game hides and the fortune-teller hints at, or -1 when it has found none. */
Sint32 scNaribweRoomsDjinniHintLimit(Sint32 group)
{
	Sint32 i = DJINNI_INDEX(DJINNI_CRYSTAL);

	do {
		Sint32 flag = FLAG_DJINNI(group * DJINN_PER_ELEMENT + i);

		if(group == ELEMENT_VENUS) {
			if(i == DJINNI_INDEX(DJINNI_ECHO))
				continue;
		}
		if(group == ELEMENT_MERCURY) {
			if(i == DJINNI_INDEX(DJINNI_SPRING))
				continue;
			if(i == DJINNI_INDEX(DJINNI_SHADE))
				continue;
		}
		if(fdScriptGetFlag(flag))
			return i + 1;
	} while(--i >= TRANSFERRED_DJINN_PER_ELEMENT);
	return -1;
}

/* Lists the flags of an element's Djinn still to be found, from the first
   Djinni not carried over from Isaac's party up to the limit and skipping Echo,
   Spring and Shade, which the fortune-teller never hints at; returns how
   many. */
Sint32 scNaribweRoomsListMissingDjinn(Sint32 limit, Sint32 group, Sint32 *out)
{
	Sint32 cnt = 0;
	Sint32 i = TRANSFERRED_DJINN_PER_ELEMENT;

	if(i < limit) {
		Sint32 *cursor = out;

		for(; i < limit; i++) {
			Sint32 flg;

			if(group == ELEMENT_VENUS && i == DJINNI_INDEX(DJINNI_ECHO))
				continue;
			if(group == ELEMENT_MERCURY) {
				if(i == DJINNI_INDEX(DJINNI_SPRING))
					continue;
				if(i == DJINNI_INDEX(DJINNI_SHADE))
					continue;
			}
			flg = FLAG_DJINNI(group * DJINN_PER_ELEMENT + i);
			if(fdScriptGetFlag(flg) == 0) {
				*cursor++ = flg;
				cnt++;
			}
		}
	}
	return cnt;
}

/* Higher than any element's hint limit, so that the first one is lower. */
#define HINT_LIMIT_ABOVE_ALL 9999

/* The message telling where a Djinni still to be found waits, or
   MsgNaribweRoomsInItemYouHavePlaced when none is left to find.

   A Djinni the party has passed by, one before the last of its element it has
   found, is picked at random; when it has passed none by, the first still to be
   found of the element it has found the fewest of. */
Uint8 *scNaribweRoomsGetDjinniHint(void)
{
	Sint32 *hints = (Sint32 *)Value2((Sint32 (*)())syHeapSlotAllocEWRAM,
		HEAP_SLOT_DJINNI_HINTS,
		ELEMENT_COUNT * DJINN_PER_ELEMENT * sizeof(Sint32));
	Sint32	limits[ELEMENT_COUNT];
	Sint32	cnt = 0;
	Sint32	lowest = HINT_LIMIT_ABOVE_ALL;
	Sint32	lowestGroup = 0;
	Uint32	group;
	Sint32	idx;
	Uint8  *msg;

	for(group = 0; group < ELEMENT_COUNT; group++) {
		Sint32 limit = scNaribweRoomsDjinniHintLimit(group);

		/* DEAD: each element's limit, which nothing reads; the ROM keeps its
		   store. */
		limits[group] = limit;
		if(lowest > limit) {
			lowest = limit;
			lowestGroup = group;
		}
		if(limit != -1)
			cnt += scNaribweRoomsListMissingDjinn(limit, group, hints + cnt);
	}
	if(cnt == 0) {
		for(group = 0; group < ELEMENT_COUNT; group++)
			cnt += scNaribweRoomsListMissingDjinn(
				DJINNI_INDEX(DJINNI_CRYSTAL) + 1, group, hints + cnt);
		if(cnt == 0) {
			syHeapSlotRelease(HEAP_SLOT_DJINNI_HINTS);
			return MsgNaribweRoomsInItemYouHavePlaced;
		}
		scNaribweRoomsListMissingDjinn(
			DJINNI_INDEX(DJINNI_CRYSTAL) + 1, lowestGroup, hints);
		idx = 0;
	} else {
		idx = syRandom() % cnt;
	}
	switch(hints[idx]) {
	case FLAG_DJINNI(DJINNI_IRON):
		msg = MsgNaribweRoomsToEastLiesContinentMoved;
		break;
	case FLAG_DJINNI(DJINNI_STEEL):
		msg = MsgNaribweRoomsToNorthInsideSacredIcon;
		break;
	case FLAG_DJINNI(DJINNI_MUD):
		msg = MsgNaribweRoomsToNorthBeneathSacredIcon;
		break;
	case FLAG_DJINNI(DJINNI_FLOWER):
		msg = MsgNaribweRoomsHeadEastBySoutheast;
		break;
	case FLAG_DJINNI(DJINNI_MELD):
		msg = MsgNaribweRoomsHeadEastByNortheast;
		break;
	case FLAG_DJINNI(DJINNI_PETRA):
		msg = MsgNaribweRoomsHeadNorthByNorthwest;
		break;
	case FLAG_DJINNI(DJINNI_SALT):
		msg = MsgNaribweRoomsToNorthwestInRuins;
		break;
	case FLAG_DJINNI(DJINNI_GEODE):
		msg = MsgNaribweRoomsHeadWestByNorthwest;
		break;
	case FLAG_DJINNI(DJINNI_MOLD):
		msg = MsgNaribweRoomsToNorthInVillage;
		break;
	case FLAG_DJINNI(DJINNI_CRYSTAL):
		msg = MsgNaribweRoomsHeadEastBySoutheastInDesert;
		break;
	case FLAG_DJINNI(DJINNI_FOG):
		msg = MsgNaribweRoomsHeadEastByNortheastInCave;
		break;
	case FLAG_DJINNI(DJINNI_SOUR):
		msg = MsgNaribweRoomsToSoutheastInPlains;
		break;
	case FLAG_DJINNI(DJINNI_CHILL):
		msg = MsgNaribweRoomsToSouthInForestOne;
		break;
	case FLAG_DJINNI(DJINNI_STEAM):
		msg = MsgNaribweRoomsToEastBesideBoulder;
		break;
	case FLAG_DJINNI(DJINNI_RIME):
		msg = MsgHeadEastByNortheastInThe;
		break;
	case FLAG_DJINNI(DJINNI_GEL):
		msg = MsgNaribweRoomsToNorthSomewhereOnSmall;
		break;
	case FLAG_DJINNI(DJINNI_EDDY):
		msg = MsgNaribweRoomsHeadNorthByNorthwestInCave;
		break;
	case FLAG_DJINNI(DJINNI_BALM):
		msg = MsgNaribweRoomsToNorthInRuinsChilled;
		break;
	case FLAG_DJINNI(DJINNI_SERAC):
		msg = MsgHeadEastByNortheastInA;
		break;
	case FLAG_DJINNI(DJINNI_CANNON):
		msg = MsgNaribweRoomsToEastInMountainPaths;
		break;
	case FLAG_DJINNI(DJINNI_SPARK):
		msg = MsgNaribweRoomsToSoutheastInVillageLulled;
		break;
	case FLAG_DJINNI(DJINNI_KINDLE):
		msg = MsgNaribweRoomsHeadSouthBySoutheast;
		break;
	case FLAG_DJINNI(DJINNI_CHAR):
		msg = MsgNaribweRoomsHeadEastBySoutheastInTown;
		break;
	case FLAG_DJINNI(DJINNI_COAL):
		msg = MsgNaribweRoomsHeadEastByNortheastOnIsland;
		break;
	case FLAG_DJINNI(DJINNI_REFLUX):
		msg = MsgNaribweRoomsHeadSouthBySoutheastInRuins;
		break;
	case FLAG_DJINNI(DJINNI_CORE):
		msg = MsgNaribweRoomsToNorthwestInForest;
		break;
	case FLAG_DJINNI(DJINNI_TINDER):
		msg = MsgNaribweRoomsToNorthwestAlongCoast;
		break;
	case FLAG_DJINNI(DJINNI_SHINE):
		msg = MsgNaribweRoomsToNorthwestBesideRuins;
		break;
	case FLAG_DJINNI(DJINNI_FURY):
		msg = MsgNaribweRoomsHeadNorthByNorthwestAtGreat;
		break;
	case FLAG_DJINNI(DJINNI_FUGUE):
		msg = MsgNaribweRoomsToNorthInRuinsChilledByIcy;
		break;
	case FLAG_DJINNI(DJINNI_BREATH):
		msg = MsgHeadEastByNortheastInACave;
		break;
	case FLAG_DJINNI(DJINNI_BLITZ):
		msg = MsgHeadEastBySoutheastInThe;
		break;
	case FLAG_DJINNI(DJINNI_ETHER):
		msg = MsgHeadEastBySoutheastInA;
		break;
	case FLAG_DJINNI(DJINNI_WAFT):
		msg = MsgToTheNorthInTheMountain;
		break;
	case FLAG_DJINNI(DJINNI_HAZE):
		msg = MsgNaribweRoomsToEastSomewhereOnTiny;
		break;
	case FLAG_DJINNI(DJINNI_WHEEZE):
		msg = MsgNaribweRoomsToEastInForest;
		break;
	case FLAG_DJINNI(DJINNI_AROMA):
		msg = MsgNaribweRoomsHeadNorthByNorthwestInVillage;
		break;
	case FLAG_DJINNI(DJINNI_WHORL):
		msg = MsgNaribweRoomsInNorthwestInRuins;
		break;
	case FLAG_DJINNI(DJINNI_GASP):
		msg = MsgNaribweRoomsHeadNorthByNorthwestNear;
		break;
	case FLAG_DJINNI(DJINNI_LULL):
		msg = MsgToTheNorthInTheRuins;
		break;
	case FLAG_DJINNI(DJINNI_GALE):
		msg = MsgNaribweRoomsHeadNorthByNortheast;
		break;
	default:
		msg = MsgNaribweRoomsInItemYouHavePlaced;
		break;
	}
	syHeapSlotRelease(HEAP_SLOT_DJINNI_HINTS);
	return msg;
}

/* Opens the houses through the window transition, and keeps the reading's item,
   smoke and cloud out of sight until a reading needs them. */
Sint32 scInit(void)
{
	ACTOR *actor;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	fdActorDisableById(ACTOR_FORTUNE_TABLE);
	fdActorDisableById(ACTOR_PLACED_ITEM);
	scFieldActorSetSprPriority(ACTOR_FORTUNE_TELLER, 2);
	fdActorGet(ACTOR_VILLAGER_ON_BODY_PAINT)->collisionFlg |= ACTOR_NEAR_TALK;
	scFieldActorSetSprPriority(ACTOR_INNKEEPER, 2);
	fdActorGet(ACTOR_INNKEEPER)->collisionFlg |= ACTOR_NEAR_TALK;

	actor = fdActorGet(ACTOR_PLACED_ITEM);
	actor->motionFlg = 0;
	actor->y.fixed = -toFIXED(4);
	actor->groundHeight = -toFIXED(4);
	fdActorGet(ACTOR_PLACED_ITEM)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	scFieldActorSetSprPriority(ACTOR_PLACED_ITEM, 1);
	fdActorSetPos(ACTOR_PLACED_ITEM, 0, 0);

	actor = fdActorGet(ACTOR_RISING_SMOKE);
	actor->motionFlg = 0;
	actor->y.fixed = toFIXED(12);
	actor->groundHeight = toFIXED(12);
	fdActorGet(ACTOR_RISING_SMOKE)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	scFieldActorSetSprPriority(ACTOR_RISING_SMOKE, 1);
	fdActorSetPos(ACTOR_RISING_SMOKE, 0, 0);

	actor = fdActorGet(ACTOR_SMOKE_CLOUD);
	actor->motionFlg = 0;
	actor->y.fixed = toFIXED(12);
	actor->groundHeight = toFIXED(12);
	fdActorGet(ACTOR_SMOKE_CLOUD)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	scFieldActorSetSprPriority(ACTOR_SMOKE_CLOUD, 1);
	fdActorSetPos(ACTOR_SMOKE_CLOUD, 0, 0);

	fdActorSetSprFlags(fdActorGet(ACTOR_RISING_SMOKE), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_SMOKE_CLOUD), 0);
	scFieldActorSetSprCol(fdActorGet(ACTOR_SMOKE_CLOUD), SPRITE_COLOR_GBR);
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
	SCENE_EXITS(NaribweRooms),
	SCENE_EXIT_UNLESS(NARIBWE_ROOMS_TO_NARIBWE_NORTHWEST, Naribwe,
		NARIBWE_FROM_NARIBWE_ROOMS_NORTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(NARIBWE_ROOMS_TO_NARIBWE_WEST, Naribwe,
		NARIBWE_FROM_NARIBWE_ROOMS_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(NARIBWE_ROOMS_TO_NARIBWE_SOUTHWEST, Naribwe,
		NARIBWE_FROM_NARIBWE_ROOMS_SOUTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(NARIBWE_ROOMS_TO_NARIBWE_SOUTHEAST, Naribwe,
		NARIBWE_FROM_NARIBWE_ROOMS_SOUTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(NARIBWE_ROOMS_TO_NARIBWE_NORTHEAST, Naribwe,
		NARIBWE_FROM_NARIBWE_ROOMS_NORTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(NARIBWE_ROOMS_TO_WEST_HUT, NaribweRooms,
		NARIBWE_ROOMS_FROM_NORTHWEST_HUT, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(NARIBWE_ROOMS_TO_NORTHWEST_HUT, NaribweRooms,
		NARIBWE_ROOMS_FROM_WEST_HUT, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
		MOVEMENT_TYPE_WANDER, 120, 0, 104, FACING_SOUTHWEST,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 88, 0, 360, FACING_SOUTHEAST,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_FORTUNE_TELLER, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 672, 0, 94, FACING_SOUTH,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_SPIKY_HAIR, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 651, 0, 106, FACING_SOUTHEAST,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_WIFE, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 693, 0, 106, FACING_SOUTH,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 392, 0, 104, FACING_SOUTH,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 408, 0, 104, FACING_SOUTH,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS,
		MOVEMENT_TYPE_WANDER, 424, 0, 360, FACING_SOUTHWEST,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		328, 0, 368, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		672, 0, 112, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		672, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FLAME_AND_SMOKE, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 672, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DUST_CLOUD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		672, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

/* The ways out, the villagers talked to and read, and the fortune-teller's
   table, until Gabomba's message is carried. */
CUE NaribweRoomsCues[] = {
	{CUE_EXIT, NARIBWE_ROOMS_TO_NARIBWE_NORTHWEST, CONDITION_ALWAYS,
		NARIBWE_ROOMS_TO_NARIBWE_NORTHWEST},
	{CUE_EXIT, NARIBWE_ROOMS_TO_NARIBWE_WEST, CONDITION_ALWAYS,
		NARIBWE_ROOMS_TO_NARIBWE_WEST},
	{CUE_EXIT, NARIBWE_ROOMS_TO_NARIBWE_SOUTHWEST, CONDITION_ALWAYS,
		NARIBWE_ROOMS_TO_NARIBWE_SOUTHWEST},
	{CUE_EXIT, NARIBWE_ROOMS_TO_NARIBWE_SOUTHEAST, CONDITION_ALWAYS,
		NARIBWE_ROOMS_TO_NARIBWE_SOUTHEAST},
	{CUE_EXIT, NARIBWE_ROOMS_TO_NARIBWE_NORTHEAST, CONDITION_ALWAYS,
		NARIBWE_ROOMS_TO_NARIBWE_NORTHEAST},
	{CUE_EXIT, NARIBWE_ROOMS_TO_WEST_HUT, CONDITION_ALWAYS,
		NARIBWE_ROOMS_TO_WEST_HUT},
	{CUE_EXIT, NARIBWE_ROOMS_TO_NORTHWEST_HUT, CONDITION_ALWAYS,
		NARIBWE_ROOMS_TO_NORTHWEST_HUT},
	{CUE_TALK, ACTOR_BOY_DIRECTING_TO_THE_FORTUNE_TELLER, CONDITION_ALWAYS,
		MSGNO(MsgNaribweRoomsFortuneTellerLivesAtHighest)},
	{CUE_TALK, ACTOR_WOMAN_ON_A_SMALL_VILLAGE, CONDITION_ALWAYS,
		MSGNO(MsgNaribweRoomsWereSmallVillage)},
	{CUE_TALK, ACTOR_FORTUNE_TELLER, CONDITION_ALWAYS,
		MSGNO(MsgNaribweRoomsIfYouDesireReadingStand)},
	{CUE_TALK, ACTOR_FORTUNE_TELLERS_CHILD, CONDITION_ALWAYS,
		MSGNO(MsgNaribweRoomsMyFatherIsFortuneTeller)},
	{CUE_TALK, ACTOR_VILLAGER_ON_BODY_PAINT, CONDITION_ALWAYS,
		MSGNO(MsgNaribweRoomsByPaintingOurBodies)},
	{CUE_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
		SCRIPT_REF(scNaribweRoomsTalkToWeaponMerchant)},
	{CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
		SCRIPT_REF(scNaribweRoomsTalkToArmorMerchant)},
	{CUE_TALK, ACTOR_VILLAGER_ON_ADVENTURE, CONDITION_ALWAYS,
		MSGNO(MsgNaribweRoomsIfYouLikeAdventure)},
	{CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(scNaribweRoomsTalkToInnkeeper)},
	{CUE_TALK, ACTOR_FORTUNE_TABLE, CONDITION_ALWAYS,
		SCRIPT_REF(scNaribweRoomsTalkToFortuneTeller)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
		ACTOR_BOY_DIRECTING_TO_THE_FORTUNE_TELLER, CONDITION_ALWAYS,
		MSGNO(MsgNaribweRoomsFortuneTellerInNaribweHas)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_A_SMALL_VILLAGE,
		CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsAsLongAsWeListen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TELLER,
		CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsItemsThatOneCarries)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TELLERS_CHILD,
		CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsMyFatherMayNot)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_BODY_PAINT,
		CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsBodyPaintingAlsoProtectsUs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT,
		CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsICanLookAtAny)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT,
		CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsWheneverIGetMyFortune)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_ADVENTURE,
		CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsRiverIsProbablyVeryDangerous)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
		MSGNO(MsgNaribweRoomsOurSpicesAreOurSecret)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TABLE,
		CONDITION_ALWAYS, SCRIPT_REF(scNaribweRoomsMindReadFortuneTeller)},
	{CUE_TABLE_END},
};

/* The same, once Gabomba's message is carried. */
CUE NaribweRoomsCuesOnReturn[] = {
	{CUE_EXIT, NARIBWE_ROOMS_TO_NARIBWE_NORTHWEST, CONDITION_ALWAYS,
		NARIBWE_ROOMS_TO_NARIBWE_NORTHWEST},
	{CUE_EXIT, NARIBWE_ROOMS_TO_NARIBWE_WEST, CONDITION_ALWAYS,
		NARIBWE_ROOMS_TO_NARIBWE_WEST},
	{CUE_EXIT, NARIBWE_ROOMS_TO_NARIBWE_SOUTHWEST, CONDITION_ALWAYS,
		NARIBWE_ROOMS_TO_NARIBWE_SOUTHWEST},
	{CUE_EXIT, NARIBWE_ROOMS_TO_NARIBWE_SOUTHEAST, CONDITION_ALWAYS,
		NARIBWE_ROOMS_TO_NARIBWE_SOUTHEAST},
	{CUE_EXIT, NARIBWE_ROOMS_TO_NARIBWE_NORTHEAST, CONDITION_ALWAYS,
		NARIBWE_ROOMS_TO_NARIBWE_NORTHEAST},
	{CUE_EXIT, NARIBWE_ROOMS_TO_WEST_HUT, CONDITION_ALWAYS,
		NARIBWE_ROOMS_TO_WEST_HUT},
	{CUE_EXIT, NARIBWE_ROOMS_TO_NORTHWEST_HUT, CONDITION_ALWAYS,
		NARIBWE_ROOMS_TO_NORTHWEST_HUT},
	{CUE_TALK, ACTOR_BOY_DIRECTING_TO_THE_FORTUNE_TELLER, CONDITION_ALWAYS,
		MSGNO(MsgNaribweRoomsIGuessThoseWarriorsLearned)},
	{CUE_TALK, ACTOR_WOMAN_ON_A_SMALL_VILLAGE, CONDITION_ALWAYS,
		MSGNO(MsgNaribweRoomsOurFortuneTellerHasBeen)},
	{CUE_TALK, ACTOR_FORTUNE_TELLER, CONDITION_ALWAYS,
		MSGNO(MsgNaribweRoomsIfYouDesireReadingStand)},
	{CUE_TALK, ACTOR_FORTUNE_TELLERS_CHILD, CONDITION_ALWAYS,
		MSGNO(MsgNaribweRoomsIfTroubleResides)},
	{CUE_TALK, ACTOR_VILLAGER_ON_BODY_PAINT, CONDITION_ALWAYS,
		MSGNO(MsgNaribweRoomsReadingFortunesInPeoplesItems)},
	{CUE_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
		SCRIPT_REF(scNaribweRoomsTalkToWeaponMerchantOnReturn)},
	{CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
		SCRIPT_REF(scNaribweRoomsTalkToArmorMerchantOnReturn)},
	{CUE_TALK, ACTOR_VILLAGER_ON_ADVENTURE, CONDITION_ALWAYS,
		MSGNO(MsgIfIsTraveling)},
	{CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(scNaribweRoomsTalkToInnkeeperOnReturn)},
	{CUE_TALK, ACTOR_FORTUNE_TABLE, CONDITION_ALWAYS,
		SCRIPT_REF(scNaribweRoomsTalkToFortuneTeller)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
		ACTOR_BOY_DIRECTING_TO_THE_FORTUNE_TELLER, CONDITION_ALWAYS,
		MSGNO(MsgNaribweRoomsPowerOfFortuneTeller)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_A_SMALL_VILLAGE,
		CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsFortuneTellerSaidThatPower)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TELLER,
		CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsItemsPlacedBeforeMe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TELLERS_CHILD,
		CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsFatherShouldChargeMore)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_BODY_PAINT,
		CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsAfterFathersBeenReadingAll)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT,
		CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsWhenILookAtWeapons)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT,
		CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsILikeOfferingArmor)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_ADVENTURE,
		CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsHePaidMeLot)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
		MSGNO(MsgWhyWouldTravel)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TABLE,
		CONDITION_ALWAYS,
		SCRIPT_REF(scNaribweRoomsMindReadFortuneTellerOnReturn)},
	{CUE_TABLE_END},
};
