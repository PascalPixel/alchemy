/* The debug room Menu test, whose actors each run a test of the menus, the
   shops or the bags when talked to. */

#include "ability_ids.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script.h"
#include "field/field_psynergy_state.h"
#include "game/system_call.h"
#include "window/message_speed.h"
#include "text/msg_ids.h"

const SCENE_ENTRANCE *scGetEntrances(void);
const Uint32		 *scGetExits(void);
const ACTOR_PLACE	 *scGetActorPlacements(void);
const CUE			 *scGetCues(void);
Sint32				  scInit(void);
extern Uint32		  Test_UnusedRows[];
extern char			  MenuTest_ItemPrompt[12];
extern char			  MenuTest_ItemHelp[24];
extern char			  MenuTest_ItemFull[20];
extern SCENE_ENTRANCE MenuTest_Entrances[];
extern Uint32		  MenuTest_Exits[];
extern ACTOR_PLACE	  MenuTest_ActorPlacements[];
extern CUE			  MenuTest_Cues[];
extern Sint32		  MenuTest_NextLevel;
extern Sint32		  MenuTest_FillLetter;

/* The level scInit raises Garet, Mia and Ivan to. */
#define DEBUG_PARTY_LEVEL 30
/* The level scInit raises Isaac to, higher in the international editions. */
#define DEBUG_ISAAC_LEVEL \
	WHEN(DEBUG_MENU_TEST_ISAAC_LEVEL_50, 50, DEBUG_PARTY_LEVEL)

/* The placed actors, numbered in the order of MenuTest_ActorPlacements after
   its four party rows, which keep their characters' ids. Each runs its test
   when talked to. */
typedef enum MENU_TEST_ACTOR_ID {
	/* Kraden: the picker a full party drops an item with. */
	ACTOR_KRADEN = ACTOR_FIRST_PLACED,
	ACTOR_DORA,	   /* gives the party Djinn: "You got a Djinni!" */
	ACTOR_SATUROS, /* hurts the party: "The body is being torn apart." */
	ACTOR_MENARDI, /* gives the party treasure: "You got treasure!" */
	/* Vale's armor merchant: the debug shop. */
	ACTOR_ARMOR_MERCHANT,
	/* Dora again: the sanctum in Japanese, English and German, a Nut in French,
	   and in Spanish and Italian "That IS dangerous!" */
	ACTOR_SECOND_DORA,
	/* Saturos again: "Kyle... Will they be able to stop the Boulder?" */
	ACTOR_SECOND_SATUROS,
	/* The ivy: the icon browser. Whirlwind, Force and Catch leave it be. */
	ACTOR_IVY,
	/* Felix: in Japanese the party member chooser; in the other editions the
	   lines of the weapon, armor and item shops and the sanctum, one for each
	   way the leader faces him. */
	ACTOR_FELIX,
	/* Felix again: in Japanese the item browser; in the other editions the
	   warrior shops' lines in the same way, and in German, French, Spanish and
	   Italian a message test while the leader faces south. */
	ACTOR_SECOND_FELIX
} MENU_TEST_ACTOR_ID;

/* Recalculates the four heroes after a debug inventory or stat change. */
static __inline__ void dbMenuTestRecalculateParty(void)
{
	gmUnitCalc(ISAAC);
	gmUnitCalc(GARET);
	gmUnitCalc(MIA);
	gmUnitCalc(IVAN);
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return MenuTest_Entrances;
}

/* The scene has no exit regions: its regions entry returns none. */
const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return MenuTest_Exits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return MenuTest_ActorPlacements;
}

#if DEBUG_SHOP_MESSAGE_BROWSER
/* The lines of one shop, as many as the weapon shop's. */
#define SHOP_MESSAGES (MsgArmorShopWelcome - MsgWeaponShopWelcome)

/* Closes the finished message windows, opens the message and waits until it is
   typed out, then a frame more. */
static void dbMenuTestShowMessageAndWait(Sint32 messageId)
{
	mnMessageCloseFinishedWindows();
	mnMessageOpenWindow(
		messageId, 5, 0, MESSAGE_WINDOW_FIXED_X | MESSAGE_WINDOW_UNREAD);
	while(mnMessageAllTyped() == 0) {
		sySynch(1);
	}
	sySynch(1);
}

/* Browses sample dialogue with A/Down and Up; B closes the window. */
static void dbMenuTestBrowseMessageRange(Sint32 messageId, Sint32 messageCount)
{
	WINDOW *window;
	Sint32	messageIndex;

	MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_FAST;
	window = mnPortraitOpenWindow(SPRITE_YOUNG_MAN_IN_GREEN, 0, 0, 0);
	for(messageIndex = 0; messageIndex < messageCount; messageIndex++) {
		/* A sample name, item and number for each message to show: Garet, the
		   Virtuous Armlet and a number of six digits. */
		mnMessageArgumentPush(GARET, MESSAGE_ARGUMENT_ACTOR);
		mnMessageArgumentPush(ITEM_VIRTUOUS_ARMLET, MESSAGE_ARGUMENT_ITEM);
		mnMessageArgumentPush(123456, MESSAGE_ARGUMENT_NUMBER);
		dbMenuTestShowMessageAndWait(messageId);
		for(;;) {
			if((PerData & KEY_B) != 0) {
				goto end;
			}
			if((PerData & KEY_A) != 0 || (PerData & KEY_DOWN) != 0) {
				messageId++;
				break;
			}
			if((PerData & KEY_UP) != 0) {
				messageId--;
				break;
			}
			if(PerData == 0) {
				sySynch(1);
			} else {
				break;
			}
		}
	}
end:
	mnMessageCloseFinishedWindows();
	mnWindowClose(window, 2);
}

/* The shop message browsers, run by talking to ACTOR_FELIX and
   ACTOR_SECOND_FELIX while the leader faces a given way (MenuTest_Cues). */
static void dbMenuTestBrowseWeaponShopMessages(void)
{
	dbMenuTestBrowseMessageRange(MsgWeaponShopWelcome, SHOP_MESSAGES);
}

static void dbMenuTestBrowseArmorShopMessages(void)
{
	dbMenuTestBrowseMessageRange(MsgArmorShopWelcome, SHOP_MESSAGES);
}

static void dbMenuTestBrowseItemShopMessages(void)
{
	dbMenuTestBrowseMessageRange(MsgItemShopWelcome, SHOP_MESSAGES);
}

static void dbMenuTestBrowseSanctumMessages(void)
{
	dbMenuTestBrowseMessageRange(
		MsgSanctumWelcome, MsgWarriorShopWelcome - MsgSanctumWelcome);
}

static void dbMenuTestBrowseWarriorShopMessages(void)
{
	dbMenuTestBrowseMessageRange(MsgWarriorShopWelcome, SHOP_MESSAGES);
}

static void dbMenuTestBrowseWarriorArmorShopMessages(void)
{
	dbMenuTestBrowseMessageRange(MsgWarriorArmorShopWelcome, SHOP_MESSAGES);
}

static void dbMenuTestBrowseWarriorItemShopMessages(void)
{
	dbMenuTestBrowseMessageRange(MsgWarriorItemShopWelcome, SHOP_MESSAGES);
}
#endif

#if DEBUG_SECOND_FELIX_MESSAGE_TEST
/* Sets the script's message twice, so the second is the one shown, with no
   speaker, and waits until it closes. */
static void dbMenuTestRunMessageTest(void)
{
	fdScriptSetMessageId(MSGNO(MsgBattleAdditionalPsynergyHelp));
	fdScriptSetMessageId(MSGNO(MsgValeComeOnHurry));
	/* "Come on, Isaac. We have to go-now!" */
	fdScriptShowMessage(-1, 0);
}
#endif

/* Talking to ACTOR_ARMOR_MERCHANT: the debug shop. */
static void dbMenuTestOpenDebugShop(void)
{
	mnShopOpenDebugShop(1);
}

/* Unused: a script that does nothing. */
UNUSED static void dbMenuTestNoOperation(void)
{
}

/* Talking to ACTOR_KRADEN: the picker a full party drops an item with, whose
   choice of member and slot nothing uses. */
static void dbMenuTestOpenItemPicker(void)
{
	Sint32 selectedOwner;
	Sint32 selectedSlot;
	mnItemPickerRun(&selectedOwner, &selectedSlot);
}

/* Talking to Mia: the name entry screen for Isaac. */
static void dbMenuTestEditIsaacName(void)
{
	mnNameEntryEditMemberName(ISAAC);
}

/* Whirlwind, Force and Catch cast at ACTOR_IVY: nothing happens to it. */
static void dbMenuTestIgnorePsynergy(void)
{
}

/* Keeps Force from pushing ACTOR_IVY: run as Force is cast at it, before the
   shot, this sets the effect's leaveTgt, and the shot then leaves its target
   where it is (field/script_commands_force.c). */
static void dbMenuTestKeepIvyInPlace(void)
{
	((FIELD_EFFECT_STATE *)FieldEffectStateWork)->leaveTgt = 1;
}

#if DEBUG_SECOND_DORA_NUT_GIFT
/* Gives a Nut through the ordinary party gift and full-inventory dialogue.
   Talking to ACTOR_SECOND_DORA in French only. */
UNUSED static void dbMenuTestGiveItemToParty(void)
{
	fdPartyInventoryGiveItem(ITEM_NUT, 0);
}
#endif

const CUE *scGetCues(void)
{
	return MenuTest_Cues;
}

/* Talking to Garet: levels the four to MenuTest_NextLevel, then raises it by
   ten. */
static void dbMenuTestRaisePartyLevels(void)
{
	/* "You went up a level!" */
	mnFieldMessageShow(MsgDebugWentLevel, FIELD_MESSAGE_ZOOM);
	gmUnitLevelUpTo(ISAAC, MenuTest_NextLevel);
	gmUnitLevelUpTo(GARET, MenuTest_NextLevel);
	gmUnitLevelUpTo(MIA, MenuTest_NextLevel);
	gmUnitLevelUpTo(IVAN, MenuTest_NextLevel);
	MenuTest_NextLevel += 10;
	dbMenuTestRecalculateParty();
}

/* Talking to Ivan: gives the four stacks of items to use: healing items, Psy
   Crystals, Sacred Feathers and the foods that raise a stat. */
static void dbMenuTestGiveConsumableTestStock(void)
{
	/* "You got an item!" */
	mnFieldMessageShow(MsgDebugGotItem, FIELD_MESSAGE_ZOOM);
	/* Isaac: 17 Antidotes, 14 Herbs, 11 Nuts, 13 Vials and 9 Potions. */
	gmInventoryAddItem(ISAAC, ITEM_ANTIDOTE);
	gmInventoryAddItem(ISAAC, ITEM_ANTIDOTE);
	gmInventoryAddItem(ISAAC, ITEM_ANTIDOTE);
	gmInventoryAddItem(ISAAC, ITEM_ANTIDOTE);
	gmInventoryAddItem(ISAAC, ITEM_ANTIDOTE);
	gmInventoryAddItem(ISAAC, ITEM_ANTIDOTE);
	gmInventoryAddItem(ISAAC, ITEM_ANTIDOTE);
	gmInventoryAddItem(ISAAC, ITEM_ANTIDOTE);
	gmInventoryAddItem(ISAAC, ITEM_ANTIDOTE);
	gmInventoryAddItem(ISAAC, ITEM_ANTIDOTE);
	gmInventoryAddItem(ISAAC, ITEM_ANTIDOTE);
	gmInventoryAddItem(ISAAC, ITEM_ANTIDOTE);
	gmInventoryAddItem(ISAAC, ITEM_ANTIDOTE);
	gmInventoryAddItem(ISAAC, ITEM_ANTIDOTE);
	gmInventoryAddItem(ISAAC, ITEM_ANTIDOTE);
	gmInventoryAddItem(ISAAC, ITEM_ANTIDOTE);
	gmInventoryAddItem(ISAAC, ITEM_ANTIDOTE);
	gmInventoryAddItem(ISAAC, ITEM_HERB);
	gmInventoryAddItem(ISAAC, ITEM_HERB);
	gmInventoryAddItem(ISAAC, ITEM_HERB);
	gmInventoryAddItem(ISAAC, ITEM_HERB);
	gmInventoryAddItem(ISAAC, ITEM_HERB);
	gmInventoryAddItem(ISAAC, ITEM_HERB);
	gmInventoryAddItem(ISAAC, ITEM_HERB);
	gmInventoryAddItem(ISAAC, ITEM_HERB);
	gmInventoryAddItem(ISAAC, ITEM_HERB);
	gmInventoryAddItem(ISAAC, ITEM_HERB);
	gmInventoryAddItem(ISAAC, ITEM_HERB);
	gmInventoryAddItem(ISAAC, ITEM_HERB);
	gmInventoryAddItem(ISAAC, ITEM_HERB);
	gmInventoryAddItem(ISAAC, ITEM_HERB);
	gmInventoryAddItem(ISAAC, ITEM_NUT);
	gmInventoryAddItem(ISAAC, ITEM_NUT);
	gmInventoryAddItem(ISAAC, ITEM_NUT);
	gmInventoryAddItem(ISAAC, ITEM_NUT);
	gmInventoryAddItem(ISAAC, ITEM_NUT);
	gmInventoryAddItem(ISAAC, ITEM_NUT);
	gmInventoryAddItem(ISAAC, ITEM_NUT);
	gmInventoryAddItem(ISAAC, ITEM_NUT);
	gmInventoryAddItem(ISAAC, ITEM_NUT);
	gmInventoryAddItem(ISAAC, ITEM_NUT);
	gmInventoryAddItem(ISAAC, ITEM_NUT);
	gmInventoryAddItem(ISAAC, ITEM_VIAL);
	gmInventoryAddItem(ISAAC, ITEM_VIAL);
	gmInventoryAddItem(ISAAC, ITEM_VIAL);
	gmInventoryAddItem(ISAAC, ITEM_VIAL);
	gmInventoryAddItem(ISAAC, ITEM_VIAL);
	gmInventoryAddItem(ISAAC, ITEM_VIAL);
	gmInventoryAddItem(ISAAC, ITEM_VIAL);
	gmInventoryAddItem(ISAAC, ITEM_VIAL);
	gmInventoryAddItem(ISAAC, ITEM_VIAL);
	gmInventoryAddItem(ISAAC, ITEM_VIAL);
	gmInventoryAddItem(ISAAC, ITEM_VIAL);
	gmInventoryAddItem(ISAAC, ITEM_VIAL);
	gmInventoryAddItem(ISAAC, ITEM_VIAL);
	gmInventoryAddItem(ISAAC, ITEM_POTION);
	gmInventoryAddItem(ISAAC, ITEM_POTION);
	gmInventoryAddItem(ISAAC, ITEM_POTION);
	gmInventoryAddItem(ISAAC, ITEM_POTION);
	gmInventoryAddItem(ISAAC, ITEM_POTION);
	gmInventoryAddItem(ISAAC, ITEM_POTION);
	gmInventoryAddItem(ISAAC, ITEM_POTION);
	gmInventoryAddItem(ISAAC, ITEM_POTION);
	gmInventoryAddItem(ISAAC, ITEM_POTION);
	/* Garet: 13 Psy Crystals and 12 Antidotes. */
	gmInventoryAddItem(GARET, ITEM_PSY_CRYSTAL);
	gmInventoryAddItem(GARET, ITEM_PSY_CRYSTAL);
	gmInventoryAddItem(GARET, ITEM_PSY_CRYSTAL);
	gmInventoryAddItem(GARET, ITEM_PSY_CRYSTAL);
	gmInventoryAddItem(GARET, ITEM_PSY_CRYSTAL);
	gmInventoryAddItem(GARET, ITEM_PSY_CRYSTAL);
	gmInventoryAddItem(GARET, ITEM_PSY_CRYSTAL);
	gmInventoryAddItem(GARET, ITEM_PSY_CRYSTAL);
	gmInventoryAddItem(GARET, ITEM_PSY_CRYSTAL);
	gmInventoryAddItem(GARET, ITEM_PSY_CRYSTAL);
	gmInventoryAddItem(GARET, ITEM_PSY_CRYSTAL);
	gmInventoryAddItem(GARET, ITEM_PSY_CRYSTAL);
	gmInventoryAddItem(GARET, ITEM_PSY_CRYSTAL);
	gmInventoryAddItem(GARET, ITEM_ANTIDOTE);
	gmInventoryAddItem(GARET, ITEM_ANTIDOTE);
	gmInventoryAddItem(GARET, ITEM_ANTIDOTE);
	gmInventoryAddItem(GARET, ITEM_ANTIDOTE);
	gmInventoryAddItem(GARET, ITEM_ANTIDOTE);
	gmInventoryAddItem(GARET, ITEM_ANTIDOTE);
	gmInventoryAddItem(GARET, ITEM_ANTIDOTE);
	gmInventoryAddItem(GARET, ITEM_ANTIDOTE);
	gmInventoryAddItem(GARET, ITEM_ANTIDOTE);
	gmInventoryAddItem(GARET, ITEM_ANTIDOTE);
	gmInventoryAddItem(GARET, ITEM_ANTIDOTE);
	gmInventoryAddItem(GARET, ITEM_ANTIDOTE);
	/* Ivan: 14 Elixirs, 11 Waters of Life and 16 Sacred Feathers. */
	gmInventoryAddItem(IVAN, ITEM_ELIXIR);
	gmInventoryAddItem(IVAN, ITEM_ELIXIR);
	gmInventoryAddItem(IVAN, ITEM_ELIXIR);
	gmInventoryAddItem(IVAN, ITEM_ELIXIR);
	gmInventoryAddItem(IVAN, ITEM_ELIXIR);
	gmInventoryAddItem(IVAN, ITEM_ELIXIR);
	gmInventoryAddItem(IVAN, ITEM_ELIXIR);
	gmInventoryAddItem(IVAN, ITEM_ELIXIR);
	gmInventoryAddItem(IVAN, ITEM_ELIXIR);
	gmInventoryAddItem(IVAN, ITEM_ELIXIR);
	gmInventoryAddItem(IVAN, ITEM_ELIXIR);
	gmInventoryAddItem(IVAN, ITEM_ELIXIR);
	gmInventoryAddItem(IVAN, ITEM_ELIXIR);
	gmInventoryAddItem(IVAN, ITEM_ELIXIR);
	gmInventoryAddItem(IVAN, ITEM_WATER_OF_LIFE);
	gmInventoryAddItem(IVAN, ITEM_WATER_OF_LIFE);
	gmInventoryAddItem(IVAN, ITEM_WATER_OF_LIFE);
	gmInventoryAddItem(IVAN, ITEM_WATER_OF_LIFE);
	gmInventoryAddItem(IVAN, ITEM_WATER_OF_LIFE);
	gmInventoryAddItem(IVAN, ITEM_WATER_OF_LIFE);
	gmInventoryAddItem(IVAN, ITEM_WATER_OF_LIFE);
	gmInventoryAddItem(IVAN, ITEM_WATER_OF_LIFE);
	gmInventoryAddItem(IVAN, ITEM_WATER_OF_LIFE);
	gmInventoryAddItem(IVAN, ITEM_WATER_OF_LIFE);
	gmInventoryAddItem(IVAN, ITEM_WATER_OF_LIFE);
	gmInventoryAddItem(IVAN, ITEM_SACRED_FEATHER);
	gmInventoryAddItem(IVAN, ITEM_SACRED_FEATHER);
	gmInventoryAddItem(IVAN, ITEM_SACRED_FEATHER);
	gmInventoryAddItem(IVAN, ITEM_SACRED_FEATHER);
	gmInventoryAddItem(IVAN, ITEM_SACRED_FEATHER);
	gmInventoryAddItem(IVAN, ITEM_SACRED_FEATHER);
	gmInventoryAddItem(IVAN, ITEM_SACRED_FEATHER);
	gmInventoryAddItem(IVAN, ITEM_SACRED_FEATHER);
	gmInventoryAddItem(IVAN, ITEM_SACRED_FEATHER);
	gmInventoryAddItem(IVAN, ITEM_SACRED_FEATHER);
	gmInventoryAddItem(IVAN, ITEM_SACRED_FEATHER);
	gmInventoryAddItem(IVAN, ITEM_SACRED_FEATHER);
	gmInventoryAddItem(IVAN, ITEM_SACRED_FEATHER);
	gmInventoryAddItem(IVAN, ITEM_SACRED_FEATHER);
	gmInventoryAddItem(IVAN, ITEM_SACRED_FEATHER);
	gmInventoryAddItem(IVAN, ITEM_SACRED_FEATHER);
	/* Mia: 11 Power Breads, 11 Cookies, 11 Apples, 7 Hard Nuts, 13 Mints and 8
	   Lucky Peppers. */
	gmInventoryAddItem(MIA, ITEM_POWER_BREAD);
	gmInventoryAddItem(MIA, ITEM_POWER_BREAD);
	gmInventoryAddItem(MIA, ITEM_POWER_BREAD);
	gmInventoryAddItem(MIA, ITEM_POWER_BREAD);
	gmInventoryAddItem(MIA, ITEM_POWER_BREAD);
	gmInventoryAddItem(MIA, ITEM_POWER_BREAD);
	gmInventoryAddItem(MIA, ITEM_POWER_BREAD);
	gmInventoryAddItem(MIA, ITEM_POWER_BREAD);
	gmInventoryAddItem(MIA, ITEM_POWER_BREAD);
	gmInventoryAddItem(MIA, ITEM_POWER_BREAD);
	gmInventoryAddItem(MIA, ITEM_POWER_BREAD);
	gmInventoryAddItem(MIA, ITEM_COOKIE);
	gmInventoryAddItem(MIA, ITEM_COOKIE);
	gmInventoryAddItem(MIA, ITEM_COOKIE);
	gmInventoryAddItem(MIA, ITEM_COOKIE);
	gmInventoryAddItem(MIA, ITEM_COOKIE);
	gmInventoryAddItem(MIA, ITEM_COOKIE);
	gmInventoryAddItem(MIA, ITEM_COOKIE);
	gmInventoryAddItem(MIA, ITEM_COOKIE);
	gmInventoryAddItem(MIA, ITEM_COOKIE);
	gmInventoryAddItem(MIA, ITEM_COOKIE);
	gmInventoryAddItem(MIA, ITEM_COOKIE);
	gmInventoryAddItem(MIA, ITEM_APPLE);
	gmInventoryAddItem(MIA, ITEM_APPLE);
	gmInventoryAddItem(MIA, ITEM_APPLE);
	gmInventoryAddItem(MIA, ITEM_APPLE);
	gmInventoryAddItem(MIA, ITEM_APPLE);
	gmInventoryAddItem(MIA, ITEM_APPLE);
	gmInventoryAddItem(MIA, ITEM_APPLE);
	gmInventoryAddItem(MIA, ITEM_APPLE);
	gmInventoryAddItem(MIA, ITEM_APPLE);
	gmInventoryAddItem(MIA, ITEM_APPLE);
	gmInventoryAddItem(MIA, ITEM_APPLE);
	gmInventoryAddItem(MIA, ITEM_HARD_NUT);
	gmInventoryAddItem(MIA, ITEM_HARD_NUT);
	gmInventoryAddItem(MIA, ITEM_HARD_NUT);
	gmInventoryAddItem(MIA, ITEM_HARD_NUT);
	gmInventoryAddItem(MIA, ITEM_HARD_NUT);
	gmInventoryAddItem(MIA, ITEM_HARD_NUT);
	gmInventoryAddItem(MIA, ITEM_HARD_NUT);
	gmInventoryAddItem(MIA, ITEM_MINT);
	gmInventoryAddItem(MIA, ITEM_MINT);
	gmInventoryAddItem(MIA, ITEM_MINT);
	gmInventoryAddItem(MIA, ITEM_MINT);
	gmInventoryAddItem(MIA, ITEM_MINT);
	gmInventoryAddItem(MIA, ITEM_MINT);
	gmInventoryAddItem(MIA, ITEM_MINT);
	gmInventoryAddItem(MIA, ITEM_MINT);
	gmInventoryAddItem(MIA, ITEM_MINT);
	gmInventoryAddItem(MIA, ITEM_MINT);
	gmInventoryAddItem(MIA, ITEM_MINT);
	gmInventoryAddItem(MIA, ITEM_MINT);
	gmInventoryAddItem(MIA, ITEM_MINT);
	gmInventoryAddItem(MIA, ITEM_LUCKY_PEPPER);
	gmInventoryAddItem(MIA, ITEM_LUCKY_PEPPER);
	gmInventoryAddItem(MIA, ITEM_LUCKY_PEPPER);
	gmInventoryAddItem(MIA, ITEM_LUCKY_PEPPER);
	gmInventoryAddItem(MIA, ITEM_LUCKY_PEPPER);
	gmInventoryAddItem(MIA, ITEM_LUCKY_PEPPER);
	gmInventoryAddItem(MIA, ITEM_LUCKY_PEPPER);
	gmInventoryAddItem(MIA, ITEM_LUCKY_PEPPER);
	dbMenuTestRecalculateParty();
}

#if !DEBUG_DJINN_GIFT_FILL_LOOP_DROPPED
/* The Japanese release's leftover loop fills the buffer this many times, with
   this many letters the first time and one more each time after. */
#define FILL_PASSES		  16
#define FILL_FIRST_LENGTH 32
#endif

/* Talking to ACTOR_DORA: gives each of the four seven Djinn of their element
   (Ivan six) and sets them. */
static void dbMenuTestGivePartyDjinn(void)
{
	/* LEFTOVER(frame): the buffer the Japanese release's loop below fills
	   FILL_PASSES times. */
	UNUSED Uint8 fillBuffer[256];
#if !DEBUG_DJINN_GIFT_FILL_LOOP_DROPPED
	Sint32 pass;
	Sint32 characterIndex;

	for(pass = 0; pass < FILL_PASSES; pass++) {
		for(characterIndex = 0; characterIndex < (Sint32)sizeof(fillBuffer);
			characterIndex++) {
			fillBuffer[characterIndex] = 0;
		}
		for(characterIndex = 0; characterIndex < pass + FILL_FIRST_LENGTH;
			characterIndex++) {
			fillBuffer[characterIndex] = MenuTest_FillLetter + 1;
		}
		MenuTest_FillLetter++;
		if(MenuTest_FillLetter >= 'a') {
			MenuTest_FillLetter = 'A';
		}
	}
#endif
	/* "You got a Djinni!" */
	mnFieldMessageShow(MsgDebugGotDjinni, FIELD_MESSAGE_ZOOM);
	gmDjinnGive(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_FLINT));
	gmDjinnGive(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GRANITE));
	gmDjinnGive(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_QUARTZ));
	gmDjinnGive(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_VINE));
	gmDjinnGive(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_SAP));
	gmDjinnGive(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GROUND));
	gmDjinnGive(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_BANE));
	gmDjinnSet(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_FLINT));
	gmDjinnSet(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GRANITE));
	gmDjinnSet(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_QUARTZ));
	gmDjinnSet(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_VINE));
	gmDjinnSet(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_SAP));
	gmDjinnSet(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GROUND));
	gmDjinnSet(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_BANE));
	gmDjinnGive(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FORGE));
	gmDjinnGive(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FEVER));
	gmDjinnGive(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_CORONA));
	gmDjinnGive(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_SCORCH));
	gmDjinnGive(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_EMBER));
	gmDjinnGive(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FLASH));
	gmDjinnGive(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_TORCH));
	gmDjinnSet(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FORGE));
	gmDjinnSet(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FEVER));
	gmDjinnSet(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_CORONA));
	gmDjinnSet(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_SCORCH));
	gmDjinnSet(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_EMBER));
	gmDjinnSet(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FLASH));
	gmDjinnSet(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_TORCH));
	gmDjinnGive(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_FIZZ));
	gmDjinnGive(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_SLEET));
	gmDjinnGive(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_MIST));
	gmDjinnGive(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_SPRITZ));
	gmDjinnGive(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_HAIL));
	gmDjinnGive(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_TONIC));
	gmDjinnGive(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_DEW));
	gmDjinnSet(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_FIZZ));
	gmDjinnSet(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_SLEET));
	gmDjinnSet(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_MIST));
	gmDjinnSet(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_SPRITZ));
	gmDjinnSet(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_HAIL));
	gmDjinnSet(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_TONIC));
	gmDjinnSet(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_DEW));
	gmDjinnGive(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_GUST));
	gmDjinnGive(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_BREEZE));
	gmDjinnGive(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_ZEPHYR));
	gmDjinnGive(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_SMOG));
	gmDjinnGive(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_KITE));
	gmDjinnGive(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_SQUALL));
	gmDjinnSet(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_GUST));
	gmDjinnSet(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_BREEZE));
	gmDjinnSet(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_ZEPHYR));
	gmDjinnSet(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_SMOG));
	gmDjinnSet(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_KITE));
	gmDjinnSet(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_SQUALL));
	dbMenuTestRecalculateParty();
}

/* Talking to ACTOR_SATUROS: takes HP and PP from the four, then poisons and
   haunts Isaac, and binds Garet and gives him deadly poison. */
static void dbMenuTestDamageAndAfflictParty(void)
{
	UNIT *isaac;
	UNIT *garet;

	/* "The body is being torn apart." */
	mnFieldMessageShow(MsgDebugBodyTornApart, FIELD_MESSAGE_ZOOM);
	syFieldUnitAddHp(ISAAC, -100);
	syFieldUnitAddHp(GARET, -100);
	syFieldUnitAddHp(IVAN, -33);
	syFieldUnitAddHp(MIA, -100);
	gmUnitAddPp(ISAAC, -50);
	gmUnitAddPp(GARET, -40);
	gmUnitAddPp(IVAN, -35);
	gmUnitAddPp(MIA, -20);
	isaac = gmUnitGet(ISAAC);
	isaac->poison = POISON_NORMAL;
	isaac->haunt = 1;
	garet = gmUnitGet(GARET);
	garet->curse = CURSE_BOUND;
	garet->poison = POISON_VENOM;
	dbMenuTestRecalculateParty();
}

/* Talking to Jenna: gives the four weapons and armor. */
static void dbMenuTestGiveEquipmentTestStock(void)
{
	/* "You got sturdy equipment!" */
	mnFieldMessageShow(MsgDebugGotSturdyEquipment, FIELD_MESSAGE_ZOOM);
	gmInventoryAddItem(ISAAC, ITEM_SPIKED_ARMOR);
	gmInventoryAddItem(ISAAC, ITEM_ASURAS_ARMOR);
	gmInventoryAddItem(ISAAC, ITEM_EARTH_SHIELD);
	gmInventoryAddItem(ISAAC, ITEM_DRAGON_SHIELD);
	gmInventoryAddItem(ISAAC, ITEM_GAIA_BLADE);
	gmInventoryAddItem(ISAAC, ITEM_MURAMASA);
	gmInventoryAddItem(ISAAC, ITEM_BANDITS_SWORD);
	gmInventoryAddItem(ISAAC, ITEM_KUSANAGI);
	gmInventoryAddItem(GARET, ITEM_DEMON_AXE);
	gmInventoryAddItem(GARET, ITEM_BURNING_AXE);
	gmInventoryAddItem(GARET, ITEM_WICKED_MACE);
	gmInventoryAddItem(GARET, ITEM_BLESSED_MACE);
	gmInventoryAddItem(GARET, ITEM_DEMON_MAIL);
	gmInventoryAddItem(GARET, ITEM_DRAGON_SCALES);
	gmInventoryAddItem(GARET, ITEM_AURA_GLOVES);
	gmInventoryAddItem(GARET, ITEM_BATTLE_GLOVES);
	gmInventoryAddItem(GARET, ITEM_ADEPTS_HELM);
	gmInventoryAddItem(IVAN, ITEM_ZODIAC_WAND);
	gmInventoryAddItem(IVAN, ITEM_SHAMANS_ROD);
	gmInventoryAddItem(IVAN, ITEM_NINJA_GARB);
	gmInventoryAddItem(IVAN, ITEM_KIMONO);
	gmInventoryAddItem(IVAN, ITEM_EARTH_SHIELD);
	gmInventoryAddItem(IVAN, ITEM_WAR_GLOVES);
	gmInventoryAddItem(IVAN, ITEM_VIRTUOUS_ARMLET);
	gmInventoryAddItem(IVAN, ITEM_PROPHETS_HAT);
	gmInventoryAddItem(MIA, ITEM_ANGELIC_ANKH);
	gmInventoryAddItem(MIA, ITEM_CRYSTAL_ROD);
	gmInventoryAddItem(MIA, ITEM_STORM_GEAR);
	gmInventoryAddItem(MIA, ITEM_WATER_JACKET);
	gmInventoryAddItem(MIA, ITEM_ORACLES_ROBE);
	gmInventoryAddItem(MIA, ITEM_FEATHERED_ROBE);
	gmInventoryAddItem(MIA, ITEM_VAMBRACE);
	gmInventoryAddItem(MIA, ITEM_GUARDIAN_ARMLET);
	gmInventoryAddItem(MIA, ITEM_GLITTERING_TIARA);
	dbMenuTestRecalculateParty();
}

/* The Japanese item browser: `Item No:` over an item's name, its icon and what
   equipping it does; A gives it to the party, and B, or A once the bags are
   full, leaves. Unlike the Increase item and level room's, it stops on numbers
   with no item. */
static void dbMenuTestSelectItem(void)
{
	WINDOW *window;
	WINDOW *details;
	Sint32	selectedItemId;
	Sint32	redraw;
	Sint32	itemId;
	Sint32	itemCount;

	itemCount = DEBUG_ITEM_COUNT;
	sySndPlay(SE_MENU_CONFIRM);
	window = mnWindowCreate(0, 0, 30, 7, WINDOW_FLAG_INSTANT);
	details = mnWindowCreate(0, 8, 13, 10, WINDOW_FLAG_INSTANT);
	selectedItemId = 1;
	redraw = 1;
	/* Lend HIGHLIGHT_PALETTE to the item icons, as the Increase item and level
	   room's browser does. */
	syDMAXCopy((const void *)OBJ_PLTT,
		(void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
		DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
	syDMAXCopy(
		(const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
		(void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR),
		DMA_COPY_HALFWORDS(1), REG_DMA3);
	sySynch(1);
	while(1) {
		if(redraw) {
			redraw = 0;
			selectedItemId = (selectedItemId + DEBUG_ITEM_COUNT) % itemCount;
			mnWindowSprRedrawSavedRect(window);
			mnWindowSprClearList(window);
			mnTextDrawStringInWindow(MenuTest_ItemPrompt, window, 0, 0);
			mnTextDrawNumAtOffset(selectedItemId, 0, window, 80, 0);
			if(gmPartyInventoryHasSpace()) {
				mnTextDrawStringInWindow(MenuTest_ItemHelp, window, 0, 32);
				itemId = selectedItemId & INVENTORY_ITEM_ID_MASK;
				/* DEAD: the item's definition is looked up and not used; the
				   ROM keeps the call. */
				gmItemGetDirect(itemId);
				mnTextDrawMessageAsTiles(itemId + MsgItemName, window, 120, 0);
				mnTextDrawMessage(itemId + MsgItemPlainName, window, 0, 16);
				mnWindowSprRedrawSavedRect(details);
				mnItemMenuDrawItemDetails(details, selectedItemId);
			} else {
				mnTextDrawStringInWindow(MenuTest_ItemFull, window, 0, 32);
			}
		}
		if(PerPush & KEY_A) {
			if(gmPartyInventoryAdd(selectedItemId) == -1) {
				sySndPlay(SE_MENU_CANCEL);
				break;
			}
			sySndPlay(SE_NOTE_HIGH);
		}
		if(PerPush & KEY_B) {
			sySndPlay(SE_MENU_CANCEL);
			break;
		}
		if(PerRepeat & KEY_UP) {
			selectedItemId--;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(PerRepeat & KEY_DOWN) {
			selectedItemId++;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(PerRepeat & KEY_RIGHT) {
			selectedItemId += DEBUG_BROWSER_ROW_STEP;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(PerRepeat & KEY_LEFT) {
			selectedItemId -= DEBUG_BROWSER_ROW_STEP;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(PerRepeat & KEY_R) {
			selectedItemId += DEBUG_BROWSER_PAGE_STEP;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(PerRepeat & KEY_L) {
			selectedItemId -= DEBUG_BROWSER_PAGE_STEP;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		sySynch(1);
	}
	mnWindowSprRedrawSavedRect(window);
	sySynch(1);
	mnWindowClose(window, 1);
	mnWindowClose(details, 1);
}

/* Talking to ACTOR_SECOND_FELIX in Japanese: the item browser. */
UNUSED static void dbMenuTestOpenItemBrowser(void)
{
	dbMenuTestSelectItem();
}

/* Talking to ACTOR_MENARDI: gives the four treasure, eleven Game Tickets and
   eight Lucky Medals among it. */
static void dbMenuTestGiveTreasureTestStock(void)
{
	/* "You got treasure!" */
	mnFieldMessageShow(MsgDebugGotTreasure, FIELD_MESSAGE_ZOOM);
	gmInventoryAddItem(ISAAC, ITEM_HERMES_WATER);
	gmInventoryAddItem(ISAAC, ITEM_HALT_GEM);
	gmInventoryAddItem(ISAAC, ITEM_VENUS_STAR);
	gmInventoryAddItem(ISAAC, ITEM_MERCURY_STAR);
	gmInventoryAddItem(ISAAC, ITEM_MARS_STAR);
	gmInventoryAddItem(ISAAC, ITEM_JUPITER_STAR);
	gmInventoryAddItem(ISAAC, ITEM_MYTHRIL_BAG);
	gmInventoryAddItem(GARET, ITEM_SMOKE_BOMB);
	gmInventoryAddItem(GARET, ITEM_SLEEP_BOMB);
	gmInventoryAddItem(GARET, ITEM_DRAGONS_EYE);
	gmInventoryAddItem(GARET, ITEM_GAME_TICKET);
	gmInventoryAddItem(GARET, ITEM_GAME_TICKET);
	gmInventoryAddItem(GARET, ITEM_GAME_TICKET);
	gmInventoryAddItem(GARET, ITEM_GAME_TICKET);
	gmInventoryAddItem(GARET, ITEM_GAME_TICKET);
	gmInventoryAddItem(GARET, ITEM_GAME_TICKET);
	gmInventoryAddItem(GARET, ITEM_GAME_TICKET);
	gmInventoryAddItem(GARET, ITEM_GAME_TICKET);
	gmInventoryAddItem(GARET, ITEM_GAME_TICKET);
	gmInventoryAddItem(GARET, ITEM_GAME_TICKET);
	gmInventoryAddItem(GARET, ITEM_GAME_TICKET);
	gmInventoryAddItem(GARET, ITEM_LUCKY_MEDAL);
	gmInventoryAddItem(GARET, ITEM_LUCKY_MEDAL);
	gmInventoryAddItem(GARET, ITEM_LUCKY_MEDAL);
	gmInventoryAddItem(GARET, ITEM_LUCKY_MEDAL);
	gmInventoryAddItem(GARET, ITEM_LUCKY_MEDAL);
	gmInventoryAddItem(GARET, ITEM_LUCKY_MEDAL);
	gmInventoryAddItem(GARET, ITEM_LUCKY_MEDAL);
	gmInventoryAddItem(GARET, ITEM_LUCKY_MEDAL);
	gmInventoryAddItem(GARET, ITEM_ANCHOR_CHARM);
	gmInventoryAddItem(GARET, ITEM_BONE);
	gmInventoryAddItem(GARET, ITEM_MYSTIC_DRAUGHT);
	gmInventoryAddItem(IVAN, ITEM_BLACK_ORB);
	gmInventoryAddItem(IVAN, ITEM_FUR_BOOTS);
	gmInventoryAddItem(IVAN, ITEM_FAIRY_RING);
	gmInventoryAddItem(IVAN, ITEM_HEALING_RING);
	gmInventoryAddItem(IVAN, ITEM_RUNNING_SHIRT);
	gmInventoryAddItem(MIA, ITEM_WATER_OF_LIFE);
	gmInventoryAddItem(MIA, ITEM_ORB_OF_FORCE);
	gmInventoryAddItem(MIA, ITEM_DOUSE_DROP);
	gmInventoryAddItem(MIA, ITEM_FROST_JEWEL);
	gmInventoryAddItem(MIA, ITEM_LIFTING_GEM);
	gmInventoryAddItem(MIA, ITEM_HALT_GEM);
	gmInventoryAddItem(MIA, ITEM_CATCH_BEADS);
	dbMenuTestRecalculateParty();
}

/* Sets up the room: Isaac, Garet, Mia and Ivan as the party, a stock of rare
   items for the shops, everyone's levels, and in Japanese 777,777 coins. */
Sint32 scInit(void)
{
	/* Jenna leaves; Garet, Mia and Ivan join. */
	gmPartyRemoveMember(JENNA);
	gmPartyAddMember(GARET);
	gmPartyAddMember(MIA);
	gmPartyAddMember(IVAN);
	/* How many of each rare item the shops may sell: three Shamshirs, two
	   Silver Blades, a Masamune and eight rare pieces of armor. */
	gmItemAdjustRareItemStock(ITEM_SHAMSHIR, 1);
	gmItemAdjustRareItemStock(ITEM_SHAMSHIR, 1);
	gmItemAdjustRareItemStock(ITEM_SHAMSHIR, 1);
	gmItemAdjustRareItemStock(ITEM_SILVER_BLADE, 1);
	gmItemAdjustRareItemStock(ITEM_SILVER_BLADE, 1);
	gmItemAdjustRareItemStock(ITEM_MASAMUNE, 1);
	gmItemAdjustRareItemStock(ITEM_CHINA_DRESS, 1);
	gmItemAdjustRareItemStock(ITEM_COCKTAIL_DRESS, 1);
	gmItemAdjustRareItemStock(ITEM_BLESSED_ROBE, 1);
	gmItemAdjustRareItemStock(ITEM_ORACLES_ROBE, 1);
	gmItemAdjustRareItemStock(ITEM_DRAGON_SHIELD, 1);
	gmItemAdjustRareItemStock(ITEM_VAMBRACE, 1);
	gmItemAdjustRareItemStock(ITEM_SPIRIT_ARMLET, 1);
	gmItemAdjustRareItemStock(ITEM_WARRIORS_HELM, 1);
#if !DEBUG_MENU_TEST_NO_START_COINS
	MainRoutineStateWork.holdings.coins = 777777;
#endif
	gmUnitLevelUpTo(ISAAC, DEBUG_ISAAC_LEVEL);
	gmUnitLevelUpTo(GARET, DEBUG_PARTY_LEVEL);
	gmUnitLevelUpTo(MIA, DEBUG_PARTY_LEVEL);
	gmUnitLevelUpTo(IVAN, DEBUG_PARTY_LEVEL);
	dbMenuTestRecalculateParty();
	return 0;
}

/* Talking to ACTOR_IVY: the icon browser. */
static Sint32 dbMenuTestOpenIconBrowser(void)
{
	return mnDebugBrowserIcons();
}

#if !DEBUG_SHOP_MESSAGE_BROWSER
/* Talking to ACTOR_FELIX in Japanese: the party member chooser, whose choice
   nothing uses. */
static Sint32 dbMenuTestSelectCharacter(void)
{
	return mnPartyMemberChooserOpen();
}
#endif

#if !DEBUG_SECOND_DORA_NUT_GIFT
/* Talking to ACTOR_SECOND_DORA in Japanese, English and German: the sanctum. */
static Sint32 dbMenuTestOpenSanctum(Sint32 unitId)
{
	return mnSanctumRun(unitId);
}
#endif

#include "test_rows.c"

char MenuTest_ItemPrompt[12] = "Item No:";

char MenuTest_ItemHelp[24] = "A:Get Item  B:Return";

char MenuTest_ItemFull[20] = "ITEM FULL........";

SCENE_ENTRANCE MenuTest_Entrances[] = {
	{0, CONDITION_ALWAYS, 60, 0, 94, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

/* No exits. */
Uint32 MenuTest_Exits[] = {
	SCENE_EXITS_END,
};

ACTOR_PLACE MenuTest_ActorPlacements[] = {
	ACTOR_PLACEMENT(GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 48,
		FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 80,
		FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 112,
		FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 144,
		FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 80, 0,
		48, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 80, 0,
		80, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 80,
		0, 112, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MENARDI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 80,
		0, 144, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 120, 0, 48, FACING_SOUTHEAST, TALK_FACING_PARTY,
		0),
	ACTOR_PLACEMENT(SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0,
		80, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120,
		0, 112, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0,
		144, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FELIX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0,
		48, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FELIX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0,
		80, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

/* Who runs what: Garet levels, Jenna equipment, Ivan consumables, Mia Isaac's
   name, and the placed actors (MENU_TEST_ACTOR_ID) the rest. */
CUE MenuTest_Cues[] = {
	{CUE_TALK, GARET, CONDITION_ALWAYS, SCRIPT_REF(dbMenuTestRaisePartyLevels)},
	{CUE_TALK, JENNA, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestGiveEquipmentTestStock)},
	{CUE_TALK, IVAN, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestGiveConsumableTestStock)},
	{CUE_TALK, MIA, CONDITION_ALWAYS, SCRIPT_REF(dbMenuTestEditIsaacName)},
	{CUE_TALK, ACTOR_KRADEN, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestOpenItemPicker)},
	{CUE_TALK, ACTOR_DORA, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestGivePartyDjinn)},
	{CUE_TALK, ACTOR_SATUROS, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestDamageAndAfflictParty)},
	{CUE_TALK, ACTOR_MENARDI, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestGiveTreasureTestStock)},
	{CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestOpenDebugShop)},
#if DEBUG_SECOND_DORA_NUT_GIFT && !DEBUG_SECOND_DORA_DANGER_MESSAGE
	{CUE_TALK, ACTOR_SECOND_DORA, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestGiveItemToParty)},
#elif DEBUG_SECOND_DORA_DANGER_MESSAGE
	{CUE_TALK, ACTOR_SECOND_DORA, CONDITION_ALWAYS,
		MSGNO(MsgTolbiTolbiShipThenTheyAreDangerous)},
#else
	{CUE_TALK, ACTOR_SECOND_DORA, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestOpenSanctum)},
#endif
	{CUE_TALK, ACTOR_SECOND_SATUROS, CONDITION_ALWAYS,
		MSGNO(MsgValeKyleAbleStop)},
	{CUE_TALK, ACTOR_IVY, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestOpenIconBrowser)},
#if !DEBUG_SHOP_MESSAGE_BROWSER
	{CUE_TALK, ACTOR_FELIX, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestSelectCharacter)},
	{CUE_TALK, ACTOR_SECOND_FELIX, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestOpenItemBrowser)},
#else
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_FELIX, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestBrowseWeaponShopMessages)},
	{TALK_CUE_FACING(FACING_WEST), ACTOR_FELIX, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestBrowseArmorShopMessages)},
	{TALK_CUE_FACING(FACING_EAST), ACTOR_FELIX, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestBrowseItemShopMessages)},
	{TALK_CUE_FACING(FACING_SOUTH), ACTOR_FELIX, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestBrowseSanctumMessages)},
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_SECOND_FELIX, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestBrowseWarriorShopMessages)},
	{TALK_CUE_FACING(FACING_WEST), ACTOR_SECOND_FELIX, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestBrowseWarriorArmorShopMessages)},
	{TALK_CUE_FACING(FACING_EAST), ACTOR_SECOND_FELIX, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestBrowseWarriorItemShopMessages)},
#if DEBUG_SECOND_FELIX_MESSAGE_TEST
	{TALK_CUE_FACING(FACING_SOUTH), ACTOR_SECOND_FELIX, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestRunMessageTest)},
#endif
#endif
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestIgnorePsynergy)},
	{PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_IVY, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestIgnorePsynergy)},
	{PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_IVY, CONDITION_ALWAYS,
		SCRIPT_REF(dbMenuTestIgnorePsynergy)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST),
		ACTOR_IVY, CONDITION_ALWAYS, SCRIPT_REF(dbMenuTestKeepIvyInPlace)},
	{CUE_TABLE_END},
};

/* The level dbMenuTestRaisePartyLevels levels the four to next: 10 at first,
   ten more after each use. */
Sint32 MenuTest_NextLevel = 10;

/* One less than the character the Japanese loop of dbMenuTestGivePartyDjinn
   fills with: 'A' at first, so 'B', one more each pass, and back to 'A' when it
   reaches 'a'. */
Sint32 MenuTest_FillLetter = 'A';
