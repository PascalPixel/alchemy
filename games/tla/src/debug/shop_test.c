/* @file */
/* The debug room Shop test, whose people each run a test when talked to: the */
/* inn, the sanctum and four shops, Sunshine's forge, the shop and sanctum */
/* messages, the party's names, items, Djinn and levels, and the icon and */
/* portrait browsers. Its tables sit among its functions, before */
/* dbShopTestDamageAndAfflictParty. */

#include "debug/debug.h"
#include "game/game.h"
#include "menu/menu.h"
#include "text/msg_ids.h"
#include "field/event_inline.h"

#include "item_ids.h"
#include "version.h"
#include "field/field_sequence_event.h"
#include "field/field_sequence.h"
#include "window/window.h"
#include "shop/shop.h"
#include "window/screens.h"

/* The entrance that brings Isaac's party in (gmPartyAddIsaacsParty). */
#define ENTRANCE_WITH_ISAACS_PARTY 99

/* Set while Sunshine forges what the party brought him, as Yallam's */
/* FLAG_SUNSHINE_FORGING is; here only leaving the room clears it. */
#define FLAG_SHOP_TEST_SUNSHINE_FORGING FLAG_SCENE_TEMP_00

/* The room's people, numbered in the order of ActorPlacements: the shops' */
/* people, then the testers, each named for what talking to it opens */
/* (SceneEvents). */
typedef enum SHOP_TEST_ACTOR_ID {
	ACTOR_INNKEEPER = ACTOR_FIRST_PLACED,
	ACTOR_SANCTUM_PRIEST,
	ACTOR_WEAPON_SHOPKEEPER,
	ACTOR_ARMOR_SHOPKEEPER,
	ACTOR_ITEM_SHOPKEEPER,
	ACTOR_GENERAL_STOREKEEPER,
	ACTOR_SUNSHINE,
	ACTOR_SUNSHINES_WIFE,
	ACTOR_SANCTUM_MESSAGES,
	ACTOR_WEAPON_SHOP_MESSAGES,
	ACTOR_ARMOR_SHOP_MESSAGES,
	ACTOR_ITEM_SHOP_MESSAGES,
	ACTOR_ITEM_PICKER,
	ACTOR_NAME_ENTRY,
	ACTOR_MURAMASA_AND_SUMMON,
	ACTOR_ICON_BROWSER,
	ACTOR_PORTRAIT_BROWSER,
	ACTOR_LEVEL_EDITOR,
	ACTOR_ITEM_SELECTOR,
	ACTOR_TREASURE,
	ACTOR_DJINN,
	/* Talking to her takes HP and PP and afflicts the party */
	/* (dbShopTestDamageAndAfflictParty). */
	ACTOR_FORTUNE_TELLER,
	/* Four light bulbs, which do nothing when talked to (dbShopTestNoOp). */
	ACTOR_LIGHT_BULB_A,
	ACTOR_LIGHT_BULB_B,
	ACTOR_LIGHT_BULB_C,
	ACTOR_LIGHT_BULB_D
} SHOP_TEST_ACTOR_ID;

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

/* Browses `count` messages from `message`, each typed out at once: A for the */
/* next, B to close; each moves on by itself after 10 frames held, or 60 */
/* frames before it is typed out. */
void dbShopTestBrowseMessageRange(s32 message, s32 count) {
	s32 i;
	s32 frames;
	void *window;
	s32 typed;

	window = mnPortraitOpenWindow(SPRITE_YOUNG_MAN_IN_GREEN, 0, 0, 0);
	for(i = 0; i < count; i++) {
		/* A sample name, item and number for each message to show: Garet, the
		 */
		/* Shamshir and a number of six digits. */
		mnMessageArgumentPush(GARET, MESSAGE_ARGUMENT_ACTOR);
		mnMessageArgumentPush(ITEM_SHAMSHIR, MESSAGE_ARGUMENT_ITEM);
		mnMessageArgumentPush(123456, MESSAGE_ARGUMENT_NUMBER);
		mnMessageCloseFinishedWindows();
		mnMessageOpenWindow(message, 5, 0,
							MESSAGE_WINDOW_FIXED_X | MESSAGE_WINDOW_UNREAD);
		frames = 0;
		if(!(InputWork.held & (KEY_A | KEY_B))) {
			do {
				sySynch(1);
				frames++;
				if(frames > 59)
					break;
			} while(!(InputWork.held & (KEY_A | KEY_B)));
		}
		for(;;) {
			typed = mnMessageAllTyped();
			if(typed != 0)
				break;
			InputWork.held = KEY_A;
			InputWork.pressed = KEY_A;
			InputWork.repeat = KEY_A;
			sySynch(1);
		}
		frames = 0;
		if(InputWork.held & KEY_B)
			break;
		if(!(InputWork.held & KEY_A)) {
			do {
				sySynch(1);
				frames++;
				if(frames > 9)
					break;
				if(InputWork.held & KEY_B)
					goto done;
			} while(!(InputWork.held & KEY_A));
		}
		message++;
	}
done:
	mnMessageCloseFinishedWindows();
	mnWindowClose(window, 2);
}

/* The shop and sanctum message browsers: talking to ACTOR_SANCTUM_MESSAGES and
 */
/* the three after it. */
void dbShopTestBrowseWeaponShopMessages(void) {
	dbShopTestBrowseMessageRange((s32)MsgWeaponShopWelcome, SHOP_MESSAGES);
}

/* Browses the armor shop's messages. */
void dbShopTestBrowseArmorShopMessages(void) {
	dbShopTestBrowseMessageRange((s32)MsgArmorShopWelcome, SHOP_MESSAGES);
}

/* Browses the item shop's messages. */
void dbShopTestBrowseItemShopMessages(void) {
	dbShopTestBrowseMessageRange((s32)MsgItemShopWelcome, SHOP_MESSAGES);
}

/* Browses the Sanctum's messages. */
void dbShopTestBrowseSanctumMessages(void) {
	dbShopTestBrowseMessageRange((s32)MsgWelcomeWearyWanderers,
								 SANCTUM_MESSAGES);
}

/* The shops and the inn, each passed the keeper's actor id: Daila's weapon */
/* shop. */
void dbShopTestOpenWeaponShop(s32 keeper) {
	mnShopRun(SHOP_DAILA_WEAPONS, keeper);
}

/* Daila's armor shop. */
void dbShopTestOpenArmorShop(s32 keeper) {
	mnShopRun(SHOP_DAILA_ARMOR, keeper);
}

/* Daila's item shop. */
void dbShopTestOpenItemShop(s32 keeper) { mnShopRun(SHOP_DAILA_ITEMS, keeper); }

/* Izumo's general store. */
void dbShopTestOpenGeneralStore(s32 keeper) { mnShopRun(SHOP_IZUMO, keeper); }

/* Daila's inn. */
void dbShopTestOpenInn(s32 keeper) { mnInnCheckIn(DAILA_INN, keeper); }

/* Talking to ACTOR_SANCTUM_PRIEST opens the Sanctum. */
void dbShopTestOpenSanctum(s32 keeper) { mnSanctumRun(keeper); }

/* Sunshine, as in Yallam (scYallamRoomsTalkToSunshine) without his walk to */
/* the anvil: while FLAG_FORGE_OPEN is set he forges what the party */
/* brings. */
void dbShopTestTalkToSunshine(s32 actor) {
	s32 message;

	if(!fdEventGetFlag(FLAG_FORGE_OPEN)) {
		message = (s32)MsgAhSoYou;
		fdEventSetMessageId(message);
		/* "Ah, so you're looking for a really unique item, huh?" */
		fdMessageShowAtActor(actor, 0);
		if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
			fdEventWait(10);
			fdEventSetMessageId(message + 1);
		}
		else {
			fdEventWait(20);
			fdEventSetMessageId(message + 2);
		}
		fdEventShowMessageAndWait(actor, 0);
	}
	else if(fdEventGetFlag(FLAG_SHOP_TEST_SUNSHINE_FORGING)) {
		fdEventSetMessageId((s32)MsgICanTWait);
		/* "I can't wait to see what I'm going to wind up making with
		 * this......" */
		fdEventShowMessageAndWait(actor, 0);
	}
	else if(mnForgeGetItem()) {
		fdEventSetMessageId((s32)MsgMyWifeHas);
		/* "My wife has the item I made you. She can give it to you." */
		fdEventShowMessageAndWait(actor, 0);
	}
	else {
		message = (s32)MsgYouWantMe;
		fdEventSetMessageId(message);
		/* "You want me to make you a little something special?" */
		fdMessageShowAtActor(actor, 0);
		if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
			fdEventSetMessageId(message + 1);
			/* "Well, look at that! You went to get some raw materials for
			 * me..." */
			fdEventShowMessageAndWait(actor, 0);
			mnForgeRun(actor);
			if(mnForgeGetItem())
				fdEventSetFlag(FLAG_SHOP_TEST_SUNSHINE_FORGING);
		}
		else {
			fdEventSetMessageId(message + 2);
			/* "Hm... Maybe I made a mistake. It's hard to tell... If you
			 * want..." */
			fdEventShowMessageAndWait(actor, 0);
		}
	}
}

/* Sunshine's wife, as in Yallam (scYallamRoomsTalkToSunshinesWife): she */
/* hands over what Sunshine made and takes the payment. */
void dbShopTestTalkToSunshinesWife(s32 actor) {
	if(!fdEventGetFlag(FLAG_FORGE_OPEN)) {
		fdEventSetMessageId((s32)MsgThisThisIs);
		/* "This? This is the blacksmith Sunshine's smithy, the best forge
		 * in..." */
		fdEventShowMessageAndWait(actor, 0);
	}
	else if(fdEventGetFlag(FLAG_SHOP_TEST_SUNSHINE_FORGING)) {
		fdEventSetMessageId((s32)MsgItTakesOur);
		/* "It takes our Sunshine a long time to finish the items he works
		 * on...." */
		fdEventShowMessageAndWait(actor, 0);
	}
	else if(mnForgeGetItem()) {
		s32 item = mnForgeGetItem();
		u32 price = (u16)gmItemGetDirect(item)->price;
		s32 message;

		mnMessageArgumentPush(item, MESSAGE_ARGUMENT_ITEM);
		mnMessageArgumentPush(price, MESSAGE_ARGUMENT_NUMBER);
		message = (s32)MsgHereSThe;
		fdEventSetMessageId(message);
		/* "Here's the item you wanted. Now, about the payment... We only
		 * take..." */
		fdMessageShowAtActor(actor, 0);
		if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) != 0) {
			fdEventSetMessageId(message + 1);
			/* "Hey, we're not a charity! If you can't pay for it, you don't get
			 * it!" */
			fdEventShowMessageAndWait(actor, 0);
		}
		else if(gmPartyInventoryFindRoom(item) < 0) {
			fdEventSetMessageId(message + 2);
			/* "Whoops! Looks like you've got a full load there! This'll have
			 * to..." */
			fdEventShowMessageAndWait(actor, 0);
		}
		else if(MainRoutineStateWork.coins < price) {
			fdEventSetMessageId(message + 3);
			/* "You don't enough money! What are you trying to pull? I can't
			 * give..." */
			fdEventShowMessageAndWait(actor, 0);
		}
		else {
			fdEventSetMessageId(message + 4);
			/* "OK, here you go!" */
			fdEventShowMessageAndWait(actor, 0);
			fdItemDispShowItem(item,
							   ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
			fdPartyInventoryGiveItem(item, 0);
			mnForgeSetItem(0);
			gmPartyAdjustCoins(-price);
		}
	}
	else {
		fdEventSetMessageId((s32)MsgManSunshineWould);
		/* "Man, Sunshine would be rich if he worked more. What a waste..." */
		fdEventShowMessageAndWait(actor, 0);
	}
}

/* Opens the item picker for an item to show, as Naribwe's fortune-teller asks
 */
/* for one, and discards the member and slot chosen. */
void dbShopTestOpenItemPicker(void) {
	s32 selection[2];

	mnItemPickerRunToShow(&selection[1], &selection[0]);
}

/* The name entry screen for each of the eight characters in turn. */
void dbShopTestEditPartyNames(void) {
	s32 i;

	for(i = 0; i < PARTY_UNIT_COUNT; i++)
		mnNameEntryEditMemberName(i);
}

/* Adds an item to a member's inventory, from an event. */
static inline void dbEventGiveItemTo(s32 item, s32 owner) {
	gmInventoryAddItem(owner, item);
}

/* Gives 777,777 coins, ten each of Game Tickets and Lucky Medals and test */
/* equipment to Felix, Jenna, Sheba and Piers; then Isaac, Garet, Ivan and Mia
 */
/* leave the party. Talking to ACTOR_TREASURE. */
void dbShopTestGiveEquipmentTestStock(void) {
	s32 i;

	/* "You got treasure!" */
	mnFieldMessageShow((s32)MsgYouGotTreasure, FIELD_MESSAGE_ZOOM);
	MainRoutineStateWork.coins = 777777;
	for(i = 0; i < 10; i++) {
		dbEventGiveItemTo(ITEM_GAME_TICKET, FELIX);
		dbEventGiveItemTo(ITEM_LUCKY_MEDAL, FELIX);
	}
	dbEventGiveItemTo(ITEM_HERMES_WATER, FELIX);
	dbEventGiveItemTo(ITEM_HALT_GEM, FELIX);
	dbEventGiveItemTo(ITEM_MYTHRIL_BAG, FELIX);
	dbEventGiveItemTo(ITEM_MURAMASA, FELIX);
	dbEventGiveItemTo(ITEM_FAIRY_RING, FELIX);
	dbEventGiveItemTo(ITEM_MYTHRIL_BAG_JUPITER_STAR, FELIX);
	dbEventGiveItemTo(ITEM_SMOKE_BOMB, JENNA);
	dbEventGiveItemTo(ITEM_SLEEP_BOMB, JENNA);
	dbEventGiveItemTo(ITEM_DRAGONS_EYE, JENNA);
	dbEventGiveItemTo(ITEM_ANCHOR_CHARM, JENNA);
	dbEventGiveItemTo(ITEM_BONE, JENNA);
	dbEventGiveItemTo(ITEM_MYSTIC_DRAUGHT, JENNA);
	dbEventGiveItemTo(ITEM_HEALING_RING, JENNA);
	dbEventGiveItemTo(ITEM_BLACK_CRYSTAL, SHEBA);
	dbEventGiveItemTo(ITEM_FUR_BOOTS, SHEBA);
	dbEventGiveItemTo(ITEM_RUNNING_SHIRT, SHEBA);
	dbEventGiveItemTo(ITEM_TEAR_STONE, SHEBA);
	dbEventGiveItemTo(ITEM_TEAR_STONE, SHEBA);
	dbEventGiveItemTo(ITEM_TEAR_STONE, SHEBA);
	dbEventGiveItemTo(ITEM_SALAMANDER_TAIL, SHEBA);
	dbEventGiveItemTo(ITEM_MYTHRIL_SILVER, SHEBA);
	dbEventGiveItemTo(ITEM_RUSTY_SWORD, SHEBA);
	dbEventGiveItemTo(ITEM_RUSTY_SWORD, SHEBA);
	dbEventGiveItemTo(ITEM_RUSTY_SWORD_ROBBERS_BLADE, SHEBA);
	dbEventGiveItemTo(ITEM_RUSTY_SWORD_PIRATES_SABRE, SHEBA);
	dbEventGiveItemTo(ITEM_WATER_OF_LIFE, PIERS);
	dbEventGiveItemTo(ITEM_ORB_OF_FORCE, PIERS);
	dbEventGiveItemTo(ITEM_DOUSE_DROP, PIERS);
	dbEventGiveItemTo(ITEM_FROST_JEWEL, PIERS);
	dbEventGiveItemTo(ITEM_LIFTING_GEM, PIERS);
	dbEventGiveItemTo(ITEM_HALT_GEM, PIERS);
	dbEventGiveItemTo(ITEM_CATCH_BEADS, PIERS);
	gmPartyRemoveMember(ISAAC);
	gmPartyRemoveMember(GARET);
	gmPartyRemoveMember(IVAN);
	gmPartyRemoveMember(MIA);
}

void dbShopTestDamageAndAfflictParty(void);
void dbShopTestGivePartyDjinn(void);
void dbShopTestGiveMuramasaAndSummon(void);
void dbShopTestRunLevelEditor(void);
void dbShopTestSelectItem(void);
void dbShopTestOpenIconBrowser(void);
void dbShopTestOpenPortraitBrowser(void);

void dbShopTestNoOp(void);

/* The level editor's label before a member's level, as */
/* IncreaseItemAndLevel_LvLabel. */
char ShopTest_LvLabel[4] = "Lv";

/* The item selector's prompt, before the item number. */
char ShopTest_ItemPrompt[12] = "Item No:";

/* The item selector's help line, while the bags have room. */
char ShopTest_ItemHelp[24] = "A:Get Item  B:Return";

/* The item selector's line when the party's bags are full. */
char ShopTest_ItemFull[20] = "ITEM FULL........";

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 408, 0, 312, FACING_NORTH, 0, 280, 0, 780, 464, 0},
	{1, CONDITION_ALWAYS, 616, 0, 328, FACING_SOUTH, 0, 280, 0, 780, 464, 0},
	{2, CONDITION_ALWAYS, 648, 0, 152, FACING_NORTH, 0, 280, 0, 780, 464, 0},
	{3, CONDITION_ALWAYS, 408, 0, 88, FACING_SOUTH, 0, 280, 0, 780, 464, 0},
	{4, CONDITION_ALWAYS, 408, 0, 408, FACING_NORTH, 0, 280, 0, 780, 464, 0},
	{5, CONDITION_ALWAYS, 504, 0, 232, FACING_SOUTH, 0, 280, 0, 780, 464, 0},
	{SCENE_TABLE_END},
};

#include "../scene/shared/psynergy_test_exits.c"

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 392, 0, 280, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 440, 0, 280, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 488, 0, 280, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 536, 0, 280, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 584, 0, 280, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 632, 0, 280, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 680, 0, 280, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_BLUE_DRESS, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 728, 0, 280, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_EMOTE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0,
					248, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_EMOTE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0,
					248, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_EMOTE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0,
					248, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_EMOTE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0,
					248, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0,
					360, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0,
					136, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0,
					200, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 640,
					0, 96, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SEATED_IDOL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					672, 0, 96, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WINGED_FIGURE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					328, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 424, 0, 360, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0,
					360, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					488, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FORTUNE_TELLER, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 520, 0, 360, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_EMOTE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0,
					280, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_EMOTE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0,
					200, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_EMOTE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0,
					360, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_EMOTE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0,
					160, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

TRIGGER SceneEvents[] = {
	{TRIGGER_TALK, ACTOR_WEAPON_SHOPKEEPER, CONDITION_ALWAYS,
	 EVENT(dbShopTestOpenWeaponShop)},
	{TRIGGER_TALK, ACTOR_ARMOR_SHOPKEEPER, CONDITION_ALWAYS,
	 EVENT(dbShopTestOpenArmorShop)},
	{TRIGGER_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
	 EVENT(dbShopTestOpenItemShop)},
	{TRIGGER_TALK, ACTOR_GENERAL_STOREKEEPER, CONDITION_ALWAYS,
	 EVENT(dbShopTestOpenGeneralStore)},
	{TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(dbShopTestOpenInn)},
	{TRIGGER_TALK, ACTOR_SANCTUM_PRIEST, CONDITION_ALWAYS,
	 EVENT(dbShopTestOpenSanctum)},
	{TRIGGER_TALK, ACTOR_SUNSHINE, CONDITION_ALWAYS,
	 EVENT(dbShopTestTalkToSunshine)},
	{TRIGGER_TALK, ACTOR_SUNSHINES_WIFE, CONDITION_ALWAYS,
	 EVENT(dbShopTestTalkToSunshinesWife)},
	{TRIGGER_TALK, ACTOR_WEAPON_SHOP_MESSAGES, CONDITION_ALWAYS,
	 EVENT(dbShopTestBrowseWeaponShopMessages)},
	{TRIGGER_TALK, ACTOR_ARMOR_SHOP_MESSAGES, CONDITION_ALWAYS,
	 EVENT(dbShopTestBrowseArmorShopMessages)},
	{TRIGGER_TALK, ACTOR_ITEM_SHOP_MESSAGES, CONDITION_ALWAYS,
	 EVENT(dbShopTestBrowseItemShopMessages)},
	{TRIGGER_TALK, ACTOR_SANCTUM_MESSAGES, CONDITION_ALWAYS,
	 EVENT(dbShopTestBrowseSanctumMessages)},
	{TRIGGER_TALK, ACTOR_ITEM_PICKER, CONDITION_ALWAYS,
	 EVENT(dbShopTestOpenItemPicker)},
	{TRIGGER_TALK, ACTOR_NAME_ENTRY, CONDITION_ALWAYS,
	 EVENT(dbShopTestEditPartyNames)},
	{TRIGGER_TALK, ACTOR_MURAMASA_AND_SUMMON, CONDITION_ALWAYS,
	 EVENT(dbShopTestGiveMuramasaAndSummon)},
	{TRIGGER_TALK, ACTOR_ICON_BROWSER, CONDITION_ALWAYS,
	 EVENT(dbShopTestOpenIconBrowser)},
	{TRIGGER_TALK, ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
	 EVENT(dbShopTestOpenPortraitBrowser)},
	{TRIGGER_TALK, ACTOR_LEVEL_EDITOR, CONDITION_ALWAYS,
	 EVENT(dbShopTestRunLevelEditor)},
	{TRIGGER_TALK, ACTOR_ITEM_SELECTOR, CONDITION_ALWAYS,
	 EVENT(dbShopTestSelectItem)},
	{TRIGGER_TALK, ACTOR_FORTUNE_TELLER, CONDITION_ALWAYS,
	 EVENT(dbShopTestDamageAndAfflictParty)},
	{TRIGGER_TALK, ACTOR_TREASURE, CONDITION_ALWAYS,
	 EVENT(dbShopTestGiveEquipmentTestStock)},
	{TRIGGER_TALK, ACTOR_DJINN, CONDITION_ALWAYS,
	 EVENT(dbShopTestGivePartyDjinn)},
	{TRIGGER_TALK, ACTOR_LIGHT_BULB_A, CONDITION_ALWAYS, EVENT(dbShopTestNoOp)},
	{TRIGGER_TALK, ACTOR_LIGHT_BULB_B, CONDITION_ALWAYS, EVENT(dbShopTestNoOp)},
	{TRIGGER_TALK, ACTOR_LIGHT_BULB_C, CONDITION_ALWAYS, EVENT(dbShopTestNoOp)},
	{TRIGGER_TALK, ACTOR_LIGHT_BULB_D, CONDITION_ALWAYS, EVENT(dbShopTestNoOp)},
	{TRIGGER_TABLE_END},
};

/* The functions after the tables. */

/* Takes HP and PP from Felix, Jenna and Sheba, poisons and haunts Felix, */
/* curses and envenoms Jenna, and breaks the first item each of Felix and */
/* Jenna holds that may break when used. Talking to ACTOR_FORTUNE_TELLER. */
void dbShopTestDamageAndAfflictParty(void) {
	UNIT *unit;
	ITEM_DEFINITION *item;
	s32 i;

	/* "The body is being torn apart." */
	mnFieldMessageShow((s32)MsgTheBodyIs, FIELD_MESSAGE_ZOOM);
	scFieldUnitAddHp(FELIX, -100);
	scFieldUnitAddHp(JENNA, -100);
	scFieldUnitAddHp(SHEBA, -33);
	gmUnitAddPp(FELIX, -50);
	gmUnitAddPp(JENNA, -40);
	gmUnitAddPp(SHEBA, -35);
	unit = syUnitGet(FELIX);
	unit->poison = POISON_NORMAL;
	unit->haunt = 1;
	unit = syUnitGet(JENNA);
	unit->curse = CURSE_BOUND;
	unit->poison = POISON_VENOM;
	unit = syUnitGet(FELIX);
	for(i = 0; i < INVENTORY_SLOTS; i++) {
		item = gmItemGetDirect(unit->inventory[i]);
		if(unit->inventory[i] == 0)
			break;
		if(item->useType == ITEM_USE_MAY_BREAK) {
			gmInventoryBreak(FELIX, i);
			break;
		}
	}
	unit = syUnitGet(JENNA);
	for(i = 0; i < INVENTORY_SLOTS; i++) {
		item = gmItemGetDirect(unit->inventory[i]);
		if(unit->inventory[i] == 0)
			break;
		if(item->useType == ITEM_USE_MAY_BREAK) {
			gmInventoryBreak(JENNA, i);
			break;
		}
	}
	gmFlagRefreshLureCap();
}

/* Gives an owner one Djinni, marks it found and sets it if `activate` is 1. */
void dbShopTestGiveDjinni(s32 owner, s32 element, s32 djinn, s32 activate) {
	syGameFlagSet(FLAG_DJINNI(element * DJINN_PER_ELEMENT + djinn));
	gmDjinnGive(owner, element, djinn);
	if(activate == 1)
		gmDjinnSet(owner, element, djinn);
}

#if !TLA_PATCH_1
/* Gives and sets the first Djinn of each element that The Lost Age adds: */
/* Felix Echo to Meld, Jenna Cannon to Core, Sheba Breath to Wheeze and Piers */
/* Fog to Rime. Talking to ACTOR_DJINN. */
void dbShopTestGivePartyDjinn(void) {
	s32 djinn;

	/* "You got a Djinni!" */
	mnFieldMessageShow((s32)MsgYouGotA, FIELD_MESSAGE_ZOOM);
	for(djinn = DJINNI_INDEX(DJINNI_ECHO); djinn <= DJINNI_INDEX(DJINNI_MELD);
		djinn++)
		dbShopTestGiveDjinni(FELIX, ELEMENT_VENUS, djinn, 1);
	for(djinn = DJINNI_INDEX(DJINNI_CANNON); djinn <= DJINNI_INDEX(DJINNI_CORE);
		djinn++)
		dbShopTestGiveDjinni(JENNA, ELEMENT_MARS, djinn, 1);
	for(djinn = DJINNI_INDEX(DJINNI_BREATH);
		djinn <= DJINNI_INDEX(DJINNI_WHEEZE); djinn++)
		dbShopTestGiveDjinni(SHEBA, ELEMENT_JUPITER, djinn, 1);
	for(djinn = DJINNI_INDEX(DJINNI_FOG); djinn <= DJINNI_INDEX(DJINNI_RIME);
		djinn++)
		dbShopTestGiveDjinni(PIERS, ELEMENT_MERCURY, djinn, 1);
	gmFlagRefreshLureCap();
}
#else
/* Gives the party every Djinni. */
void dbShopTestGivePartyDjinn(void) {
	s32 element;
	s32 djinn;

	/* "You got a Djinni!" */
	mnFieldMessageShow((s32)MsgYouGotA, FIELD_MESSAGE_ZOOM);
	for(element = 0; element < ELEMENT_COUNT; element++) {
		for(djinn = 0; djinn < DJINNI_SPARE_FIRST; djinn++)
			gmDjinnGiveToParty(element, djinn);
	}
	gmFlagRefreshLureCap();
}
#endif

/* The event for talking to ACTOR_LIGHT_BULB_A to ACTOR_LIGHT_BULB_D. */
void dbShopTestNoOp(void) {}

/* Gives a Muramasa, then announces and unlocks Eclipse. Talking to */
/* ACTOR_MURAMASA_AND_SUMMON. */
void dbShopTestGiveMuramasaAndSummon(void) {
	fdPartyInventoryGiveItem(ITEM_MURAMASA, 0);
	mnSummonShowLearnedMessage(SUMMON_ECLIPSE);
	gmSummonLearn(SUMMON_ECLIPSE);
}

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

/* Opens on the backdrop fade at the fast message speed, shows "..." over the */
/* message browsers and a light bulb over ACTOR_LIGHT_BULB_A to */
/* ACTOR_LIGHT_BULB_D, and brings Isaac's party in when the party came in by */
/* ENTRANCE_WITH_ISAACS_PARTY. */
s32 scInit(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s32 *transition = &state->transition;
	MAIN_ROUTINE_STATE *mainState = &MainRoutineStateWork;

	*transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	mainState->msgSpeed = MESSAGE_SPEED_FAST;
	fdActorSetAnimationById(ACTOR_SANCTUM_MESSAGES, EMOTE_ELLIPSIS);
	fdActorSetAnimationById(ACTOR_WEAPON_SHOP_MESSAGES, EMOTE_ELLIPSIS);
	fdActorSetAnimationById(ACTOR_ARMOR_SHOP_MESSAGES, EMOTE_ELLIPSIS);
	fdActorSetAnimationById(ACTOR_ITEM_SHOP_MESSAGES, EMOTE_ELLIPSIS);
	fdActorSetAnimationById(ACTOR_LIGHT_BULB_A, EMOTE_LIGHT_BULB);
	fdActorSetAnimationById(ACTOR_LIGHT_BULB_B, EMOTE_LIGHT_BULB);
	fdActorSetAnimationById(ACTOR_LIGHT_BULB_C, EMOTE_LIGHT_BULB);
	fdActorSetAnimationById(ACTOR_LIGHT_BULB_D, EMOTE_LIGHT_BULB);
	if(mainState->entrance == ENTRANCE_WITH_ISAACS_PARTY)
		gmPartyAddIsaacsParty();
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* Raises every party member `levels` levels. */
void dbShopTestRaisePartyLevels(s32 levels) {
	s16 owners[16];
	s32 count = syFieldPartyListMembers(owners);
	s32 i;

	for(i = 0; i < count;) {
		u16 owner = owners[i++];

		gmUnitLevelUpTo(owner, syUnitGet(owner)->level + levels);
		gmUnitCalc(owner);
	}
}

/* The level-raising window, as Increase item and level's */
/* (dbIncreaseItemAndLevelRunLevelEditor): shows the leader's name and level, */
/* raises every party member 5 levels on Start or Select and 1 on A, and */
/* closes on B. Talking to ACTOR_LEVEL_EDITOR. */
void dbShopTestRunLevelEditor(void) {
	UNIT *unit = syUnitGet(MainRoutineStateWork.leaderActorId);
	void *window = mnWindowCreate(0, 0, 30, 9, WINDOW_FLAG_INSTANT);
	const u8 *message = MsgRaiseEveryoneS;
	s32 redraw;

	mnTextDrawMessage(message, window, 0, 0);
	mnTextDrawMessage(message + 1, window, 0, 16);
	redraw = 1;
	mnTextDrawMessage(message + 2, window, 0, 32);
	for(;;) {
		if(redraw) {
			mnWindowSprRedrawSavedRect(window);
			mnTextDrawStringAtOffset(unit->name, window, 0, 48);
			mnTextDrawStringInWindow(ShopTest_LvLabel, window, 48, 48);
			redraw = 0;
			mnTextDrawNumInWindow(unit->level, 0, window, 72, 48);
		}
		sySynch(1);
		if((InputWork.pressed & KEY_START) ||
		   (InputWork.pressed & KEY_SELECT)) {
			dbShopTestRaisePartyLevels(5);
			sySndPlay(MUS_SUPER_LUCKY);
			redraw = 1;
		}
		if(InputWork.pressed & KEY_A) {
			dbShopTestRaisePartyLevels(1);
			sySndPlay(MUS_LUCKY);
			redraw = 1;
		}
		if(InputWork.pressed & KEY_B)
			break;
	}
	sySndPlay(SE_MENU_CANCEL);
	mnWindowClose(window, 2);
}

/* The selector's item numbers, 0 to 499, wrapping. */
#define ITEM_SELECTOR_ITEM_COUNT 500

/* The item selector, as Increase item and level's */
/* (dbIncreaseItemAndLevelSelectItem): Up and Down move one item, Right and */
/* Left ten, R and L thirty, skipping items with no icon; A gives the item, B */
/* closes. Talking to ACTOR_ITEM_SELECTOR. */
void dbShopTestSelectItem(void) {
	void *window;
	void *details;
	s32 item;
	s32 redraw;
	s32 entryCount;
	s8 step;

	entryCount = ITEM_SELECTOR_ITEM_COUNT;
	step = 0;
	item = 1;
	redraw = 1;
	window = mnWindowCreate(0, 0, 30, 7, WINDOW_FLAG_INSTANT);
	details = mnWindowCreate(0, 8, 13, 10, WINDOW_FLAG_INSTANT);
	syDMAXCopy((const void *)OBJ_PLTT,
			   (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
			   DMA_COPY_HALFWORDS(16), REG_DMA3);
	syDMAXCopy(
		(const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
		(void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR),
		DMA_COPY_HALFWORDS(1), REG_DMA3);
	sySndPlay(SE_MENU_CONFIRM);
	sySynch(1);
	while(1) {
		if(redraw) {
			redraw = 0;
			mnWindowSprPrepareForRedraw(window);
			mnWindowSprPrepareForRedraw(details);
			mnTextDrawStringInWindow(ShopTest_ItemPrompt, window, 0, 0);
			mnTextDrawNumAtOffset(item, 0, window, 80, 0);
			if(gmPartyInventoryHasSpace()) {
				mnTextDrawStringInWindow(ShopTest_ItemHelp, window, 0, 32);
				mnTextDrawMessageAsTiles((s32)(item + MsgItemName), window, 120,
										 0);
				mnTextDrawMessage((s32)(item + MsgItemDescription), window, 0,
								  16);
				mnItemMenuDrawItemDetails((s32)details, item);
			}
			else {
				mnTextDrawStringInWindow(ShopTest_ItemFull, window, 0, 32);
			}
		}
		sySynch(1);
		if(InputWork.pressed & KEY_A) {
			if(gmPartyInventoryAdd(item) == -1) {
				sySndPlay(SE_MENU_CANCEL);
				break;
			}
			sySndPlay(SE_NOTE_HIGH);
		}
		if(InputWork.pressed & KEY_B) {
			sySndPlay(SE_MENU_CANCEL);
			break;
		}
		if(InputWork.repeat & KEY_DOWN) {
			step = 1;
			item++;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(InputWork.repeat & KEY_UP) {
			step = -1;
			item--;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(InputWork.repeat & KEY_RIGHT) {
			step = 1;
			item += DEBUG_BROWSER_ROW_STEP;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(InputWork.repeat & KEY_LEFT) {
			step = -1;
			item -= DEBUG_BROWSER_ROW_STEP;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(InputWork.repeat & KEY_R) {
			step = 1;
			item += DEBUG_BROWSER_PAGE_STEP;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(InputWork.repeat & KEY_L) {
			step = -1;
			item -= DEBUG_BROWSER_PAGE_STEP;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(step == -1) {
			for(item = (item + ITEM_SELECTOR_ITEM_COUNT) % entryCount;
				gmItemGetDirect(item & INVENTORY_ITEM_ID_MASK)->icon == 0;
				item = (item + ITEM_SELECTOR_ITEM_COUNT - 1) % entryCount)
				;
		}
		if(step == 1) {
			for(item = (item + ITEM_SELECTOR_ITEM_COUNT) % entryCount;
				gmItemGetDirect(item & INVENTORY_ITEM_ID_MASK)->icon == 0;
				item = (item + ITEM_SELECTOR_ITEM_COUNT + 1) % entryCount)
				;
		}
		step = 0;
	}
	mnWindowClose(window, 2);
	mnWindowClose(details, 2);
}

/* Talking to ACTOR_ICON_BROWSER: pages of item, Psynergy and status icons. */
void dbShopTestOpenIconBrowser(void) { mnDebugBrowserIcons(); }

/* Talking to ACTOR_PORTRAIT_BROWSER: the portraits, one at a time. */
void dbShopTestOpenPortraitBrowser(void) { mnDebugBrowserPortraits(); }

/* Unused: the Psynergy test's dbPsynergyTestDigCells again. */
UNUSED void dbShopTestDigCells(s32 stage, s32 sourceX, s32 sourceY, s32 width,
							   s32 height, s32 destinationX, s32 destinationY) {
	if(stage == SCOOP_STAGE_DUG)
		fdMapCopyCellsRect(sourceX, sourceY, width, height, destinationX,
						   destinationY);
}

/* Unused: the Psynergy test's dbPsynergyTestGrowActor again. */
UNUSED void dbShopTestGrowActor(s32 stage, s32 id, s32 x, s32 z) {
	ACTOR *actor;
	s32 scale;

	actor = fdActorGetById(id);
	if(stage == SCOOP_STAGE_DUG) {
		fdActorSetPos(id, x, z);
		actor->scaleX = 0;
		actor->scaleY = 0;
	}
	if(stage == SCOOP_STAGE_DUG_UP) {
		while(actor->scaleX < FIXED_ONE) {
			sySynch(1);
			scale = actor->scaleX;
			scale += toFIXED(0.02);
			actor->scaleX = scale;
			actor->scaleY = scale;
		}
		actor->scaleX = FIXED_ONE;
		actor->scaleY = FIXED_ONE;
	}
}

/* Unused: the Psynergy test's dbPsynergyTestDropActor again. */
UNUSED void dbShopTestDropActor(s32 stage, s32 id, s32 x, s32 z) {
	ACTOR *actor;

	actor = fdActorGetById(id);
	if(stage == SCOOP_STAGE_DUG_UP) {
		fdActorSetPos(id, x, z);
		actor->y.fixed = toFIXED(32);
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		actor->gravity = toFIXED(0.09);
		sySynch(50);
	}
}

/* Unused: the Psynergy test's dbPsynergyTestRaiseStatue again, which starts */
/* the battle in slot `slot` of encounter zone `zone`. */
UNUSED void dbShopTestRaiseStatue(s32 stage, s32 x, s32 z, s32 zone, s32 slot) {
	ACTOR *actor;

	if(stage == SCOOP_STAGE_DUG) {
		actor =
			scFieldActorCreate(SPRITE_BROWN_BOULDER, toFIXED(x), 0, toFIXED(z));
		if(actor != NULL) {
			fdActorSetSprFlags(actor, 0);
			actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		}
	}
	if(stage == SCOOP_STAGE_DUG_UP) {
		actor = ((ACTOR *)syImportActorCreate(SPRITE_WINGED_STATUE, toFIXED(x),
											  0, toFIXED(z)));
		if(actor != NULL) {
			s32 scale;

			fdActorSetSprFlags(actor, 0);
			actor->velY = toFIXED(6);
			actor->scaleX = FIXED_ONE * 2 / 5;
			actor->scaleY = FIXED_ONE * 2 / 5;
			while(actor->scaleX < FIXED_ONE) {
				sySynch(1);
				scale = actor->scaleX + toFIXED(0.02);
				actor->facing += FACING_STEP;
				actor->scaleX = scale;
				actor->scaleY = scale;
			}
			actor->scaleX = FIXED_ONE;
			actor->scaleY = FIXED_ONE;
			actor->facing =
				fdActorGetById(MainRoutineStateWork.leaderActorId)->facing +
				DEGtoANG(180);
			fdRequestBattleFromEncounterTable(zone, slot);
		}
	}
}
