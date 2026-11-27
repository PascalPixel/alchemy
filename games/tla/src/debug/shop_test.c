// The debug room Shop test, whose people each run a test when talked to: the inn, the sanctum and
// four shops, Sunshine's forge, the shop and sanctum messages, the party's names, items, Djinn and
// levels, and the icon and portrait browsers. Its tables sit among its functions, before
// dbShopTestDamageAndAfflictParty.

#include "debug/debug.h"
#include "game/game.h"
#include "menu/menu.h"
#include "text/msg_ids.h"
#include "field/script_inline.h"

#include "item_ids.h"
#include "edition.h"
#include "field/field_mode_script.h"
#include "field/field_mode.h"
#include "window/window.h"
#include "shop/shop.h"
#include "window/screens.h"

// The entrance that brings Isaac's party in (gmPartyAddIsaacsParty).
#define ENTRANCE_WITH_ISAACS_PARTY      99

// Set while Sunshine forges what the party brought him, as Yallam's FLAG_SUNSHINE_FORGING is; here
// only leaving the room clears it.
#define FLAG_SHOP_TEST_SUNSHINE_FORGING FLAG_SCENE_TEMP_00

// The room's people, numbered in the order of ActorPlacements: the shops' people, then the testers,
// each named for what talking to it opens (SceneCues).
typedef enum SHOP_TEST_ACTOR_ID
{
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
    // Talking to her takes HP and PP and afflicts the party (dbShopTestDamageAndAfflictParty).
    ACTOR_FORTUNE_TELLER,
    // Four light bulbs, which do nothing when talked to (dbShopTestNoOperation).
    ACTOR_LIGHT_BULB_A,
    ACTOR_LIGHT_BULB_B,
    ACTOR_LIGHT_BULB_C,
    ACTOR_LIGHT_BULB_D
} SHOP_TEST_ACTOR_ID;

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

// Browses `count` messages from `msg`, each typed out at once: A for the next, B to close; each
// moves on by itself after 10 ticks held, or 60 ticks before it is typed out.
void dbShopTestBrowseMessageRange(Sint32 msg, Sint32 count)
{
    Sint32 i;
    Sint32 ticks;
    void  *window;
    Sint32 typed;

    window = mnPortraitOpenWindow(SPRITE_YOUNG_MAN_IN_GREEN, 0, 0, 0);
    for (i = 0; i < count; i++)
    {
        // A sample name, item and number for each message to show: Garet, the Shamshir and a number
        // of six digits.
        mnMessageArgumentPush(GARET, MESSAGE_ARGUMENT_ACTOR);
        mnMessageArgumentPush(ITEM_SHAMSHIR, MESSAGE_ARGUMENT_ITEM);
        mnMessageArgumentPush(123456, MESSAGE_ARGUMENT_NUMBER);
        mnMessageCloseFinishedWindows();
        mnMessageOpenWindow(msg, 5, 0, MESSAGE_WINDOW_FIXED_X | MESSAGE_WINDOW_UNREAD);
        ticks = 0;
        if (!(PerWork.data & (KEY_A | KEY_B)))
        {
            do
            {
                sySynch(1);
                ticks++;
                if (ticks > 59)
                    break;
            } while (!(PerWork.data & (KEY_A | KEY_B)));
        }
        for (;;)
        {
            typed = mnMessageAllTyped();
            if (typed != 0)
                break;
            PerWork.data   = KEY_A;
            PerWork.push   = KEY_A;
            PerWork.repeat = KEY_A;
            sySynch(1);
        }
        ticks = 0;
        if (PerWork.data & KEY_B)
            break;
        if (!(PerWork.data & KEY_A))
        {
            do
            {
                sySynch(1);
                ticks++;
                if (ticks > 9)
                    break;
                if (PerWork.data & KEY_B)
                    goto done;
            } while (!(PerWork.data & KEY_A));
        }
        msg++;
    }
done:
    mnMessageCloseFinishedWindows();
    mnWindowClose(window, 2);
}

// The shop and sanctum message browsers: talking to ACTOR_SANCTUM_MESSAGES and the three after it.
void dbShopTestBrowseWeaponShopMessages(void)
{
    dbShopTestBrowseMessageRange((Sint32)MsgWeaponShopWelcome, SHOP_MESSAGES);
}

// Browses the armor shop's messages.
void dbShopTestBrowseArmorShopMessages(void)
{
    dbShopTestBrowseMessageRange((Sint32)MsgArmorShopWelcome, SHOP_MESSAGES);
}

// Browses the item shop's messages.
void dbShopTestBrowseItemShopMessages(void)
{
    dbShopTestBrowseMessageRange((Sint32)MsgItemShopWelcome, SHOP_MESSAGES);
}

// Browses the Sanctum's messages.
void dbShopTestBrowseSanctumMessages(void)
{
    dbShopTestBrowseMessageRange((Sint32)MsgWelcomeWearyWanderers, SANCTUM_MESSAGES);
}

// The shops and the inn, each passed the keeper's actor id: Daila's weapon shop.
void dbShopTestOpenWeaponShop(Sint32 keeper)
{
    mnShopRun(SHOP_DAILA_WEAPONS, keeper);
}

// Daila's armor shop.
void dbShopTestOpenArmorShop(Sint32 keeper)
{
    mnShopRun(SHOP_DAILA_ARMOR, keeper);
}

// Daila's item shop.
void dbShopTestOpenItemShop(Sint32 keeper)
{
    mnShopRun(SHOP_DAILA_ITEMS, keeper);
}

// Izumo's general store.
void dbShopTestOpenGeneralStore(Sint32 keeper)
{
    mnShopRun(SHOP_IZUMO, keeper);
}

// Daila's inn.
void dbShopTestOpenInn(Sint32 keeper)
{
    mnInnCheckIn(DAILA_INN, keeper);
}

// Talking to ACTOR_SANCTUM_PRIEST opens the Sanctum.
void dbShopTestOpenSanctum(Sint32 keeper)
{
    mnSanctumRun(keeper);
}

// Sunshine, as in Yallam (scYallamRoomsTalkToSunshine) without his walk to the anvil: while
// FLAG_FORGE_OPEN is set he forges what the party brings.
void dbShopTestTalkToSunshine(Sint32 actor)
{
    Sint32 msg;

    if (!fdScriptGetFlag(FLAG_FORGE_OPEN))
    {
        msg = (Sint32)MsgAhSoYou;
        fdScriptSetMessageId(msg);
        // "Ah, so you're looking for a really unique item, huh?"
        fdMessageShowAtActor(actor, 0);
        if (fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0)
        {
            fdScriptWait(10);
            fdScriptSetMessageId(msg + 1);
        }
        else
        {
            fdScriptWait(20);
            fdScriptSetMessageId(msg + 2);
        }
        fdScriptShowMessageAndWait(actor, 0);
    }
    else if (fdScriptGetFlag(FLAG_SHOP_TEST_SUNSHINE_FORGING))
    {
        fdScriptSetMessageId((Sint32)MsgICanTWait);
        // "I can't wait to see what I'm going to wind up making with this......"
        fdScriptShowMessageAndWait(actor, 0);
    }
    else if (mnForgeGetItem())
    {
        fdScriptSetMessageId((Sint32)MsgMyWifeHas);
        // "My wife has the item I made you. She can give it to you."
        fdScriptShowMessageAndWait(actor, 0);
    }
    else
    {
        msg = (Sint32)MsgYouWantMe;
        fdScriptSetMessageId(msg);
        // "You want me to make you a little something special?"
        fdMessageShowAtActor(actor, 0);
        if (fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0)
        {
            fdScriptSetMessageId(msg + 1);
            // "Well, look at that! You went to get some raw materials for me..."
            fdScriptShowMessageAndWait(actor, 0);
            mnForgeRun(actor);
            if (mnForgeGetItem())
                fdScriptSetFlag(FLAG_SHOP_TEST_SUNSHINE_FORGING);
        }
        else
        {
            fdScriptSetMessageId(msg + 2);
            // "Hm... Maybe I made a mistake. It's hard to tell... If you want..."
            fdScriptShowMessageAndWait(actor, 0);
        }
    }
}

// Sunshine's wife, as in Yallam (scYallamRoomsTalkToSunshinesWife): she hands over what Sunshine
// made and takes the payment.
void dbShopTestTalkToSunshinesWife(Sint32 actor)
{
    if (!fdScriptGetFlag(FLAG_FORGE_OPEN))
    {
        fdScriptSetMessageId((Sint32)MsgThisThisIs);
        // "This? This is the blacksmith Sunshine's smithy, the best forge in..."
        fdScriptShowMessageAndWait(actor, 0);
    }
    else if (fdScriptGetFlag(FLAG_SHOP_TEST_SUNSHINE_FORGING))
    {
        fdScriptSetMessageId((Sint32)MsgItTakesOur);
        // "It takes our Sunshine a long time to finish the items he works on...."
        fdScriptShowMessageAndWait(actor, 0);
    }
    else if (mnForgeGetItem())
    {
        Sint32 item  = mnForgeGetItem();
        Uint32 price = (Uint16)gmItemGetDirect(item)->price;
        Sint32 msg;

        mnMessageArgumentPush(item, MESSAGE_ARGUMENT_ITEM);
        mnMessageArgumentPush(price, MESSAGE_ARGUMENT_NUMBER);
        msg = (Sint32)MsgHereSThe;
        fdScriptSetMessageId(msg);
        // "Here's the item you wanted. Now, about the payment... We only take..."
        fdMessageShowAtActor(actor, 0);
        if (fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) != 0)
        {
            fdScriptSetMessageId(msg + 1);
            // "Hey, we're not a charity! If you can't pay for it, you don't get it!"
            fdScriptShowMessageAndWait(actor, 0);
        }
        else if (gmPartyInventoryFindRoom(item) < 0)
        {
            fdScriptSetMessageId(msg + 2);
            // "Whoops! Looks like you've got a full load there! This'll have to..."
            fdScriptShowMessageAndWait(actor, 0);
        }
        else if (MainRoutineStateWork.coins < price)
        {
            fdScriptSetMessageId(msg + 3);
            // "You don't enough money! What are you trying to pull? I can't give..."
            fdScriptShowMessageAndWait(actor, 0);
        }
        else
        {
            fdScriptSetMessageId(msg + 4);
            // "OK, here you go!"
            fdScriptShowMessageAndWait(actor, 0);
            fdItemDisplayShowItem(item, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
            fdPartyInventoryGiveItem(item, 0);
            mnForgeSetItem(0);
            gmPartyAdjustCoins(-price);
        }
    }
    else
    {
        fdScriptSetMessageId((Sint32)MsgManSunshineWould);
        // "Man, Sunshine would be rich if he worked more. What a waste..."
        fdScriptShowMessageAndWait(actor, 0);
    }
}

// Opens the item picker for an item to show, as Naribwe's fortune-teller asks for one, and discards
// the member and slot chosen.
void dbShopTestOpenItemPicker(void)
{
    Sint32 selection[2];

    mnItemPickerRunToShow(&selection[1], &selection[0]);
}

// The name entry screen for each of the eight characters in turn.
void dbShopTestEditPartyNames(void)
{
    Sint32 i;

    for (i = 0; i < PARTY_UNIT_COUNT; i++)
        mnNameEntryEditMemberName(i);
}

// Adds an item to a member's inventory, from a script.
static inline void dbScriptGiveItemTo(Sint32 item, Sint32 owner)
{
    gmInventoryAddItem(owner, item);
}

// Gives 777,777 coins, ten each of Game Tickets and Lucky Medals and test equipment to Felix,
// Jenna, Sheba and Piers; then Isaac, Garet, Ivan and Mia leave the party. Talking to
// ACTOR_TREASURE.
void dbShopTestGiveEquipmentTestStock(void)
{
    Sint32 i;

    // "You got treasure!"
    mnFieldMessageShow((Sint32)MsgYouGotTreasure, FIELD_MESSAGE_ZOOM);
    MainRoutineStateWork.coins = 777777;
    for (i = 0; i < 10; i++)
    {
        dbScriptGiveItemTo(ITEM_GAME_TICKET, FELIX);
        dbScriptGiveItemTo(ITEM_LUCKY_MEDAL, FELIX);
    }
    dbScriptGiveItemTo(ITEM_HERMES_WATER, FELIX);
    dbScriptGiveItemTo(ITEM_HALT_GEM, FELIX);
    dbScriptGiveItemTo(ITEM_MYTHRIL_BAG, FELIX);
    dbScriptGiveItemTo(ITEM_MURAMASA, FELIX);
    dbScriptGiveItemTo(ITEM_FAIRY_RING, FELIX);
    dbScriptGiveItemTo(ITEM_MYTHRIL_BAG_JUPITER_STAR, FELIX);
    dbScriptGiveItemTo(ITEM_SMOKE_BOMB, JENNA);
    dbScriptGiveItemTo(ITEM_SLEEP_BOMB, JENNA);
    dbScriptGiveItemTo(ITEM_DRAGONS_EYE, JENNA);
    dbScriptGiveItemTo(ITEM_ANCHOR_CHARM, JENNA);
    dbScriptGiveItemTo(ITEM_BONE, JENNA);
    dbScriptGiveItemTo(ITEM_MYSTIC_DRAUGHT, JENNA);
    dbScriptGiveItemTo(ITEM_HEALING_RING, JENNA);
    dbScriptGiveItemTo(ITEM_BLACK_CRYSTAL, SHEBA);
    dbScriptGiveItemTo(ITEM_FUR_BOOTS, SHEBA);
    dbScriptGiveItemTo(ITEM_RUNNING_SHIRT, SHEBA);
    dbScriptGiveItemTo(ITEM_TEAR_STONE, SHEBA);
    dbScriptGiveItemTo(ITEM_TEAR_STONE, SHEBA);
    dbScriptGiveItemTo(ITEM_TEAR_STONE, SHEBA);
    dbScriptGiveItemTo(ITEM_SALAMANDER_TAIL, SHEBA);
    dbScriptGiveItemTo(ITEM_MYTHRIL_SILVER, SHEBA);
    dbScriptGiveItemTo(ITEM_RUSTY_SWORD, SHEBA);
    dbScriptGiveItemTo(ITEM_RUSTY_SWORD, SHEBA);
    dbScriptGiveItemTo(ITEM_RUSTY_SWORD_ROBBERS_BLADE, SHEBA);
    dbScriptGiveItemTo(ITEM_RUSTY_SWORD_PIRATES_SABRE, SHEBA);
    dbScriptGiveItemTo(ITEM_WATER_OF_LIFE, PIERS);
    dbScriptGiveItemTo(ITEM_ORB_OF_FORCE, PIERS);
    dbScriptGiveItemTo(ITEM_DOUSE_DROP, PIERS);
    dbScriptGiveItemTo(ITEM_FROST_JEWEL, PIERS);
    dbScriptGiveItemTo(ITEM_LIFTING_GEM, PIERS);
    dbScriptGiveItemTo(ITEM_HALT_GEM, PIERS);
    dbScriptGiveItemTo(ITEM_CATCH_BEADS, PIERS);
    gmPartyRemoveMember(ISAAC);
    gmPartyRemoveMember(GARET);
    gmPartyRemoveMember(IVAN);
    gmPartyRemoveMember(MIA);
}

void           dbShopTestDamageAndAfflictParty(void);
void           dbShopTestGivePartyDjinn(void);
void           dbShopTestGiveMuramasaAndSummon(void);
void           dbShopTestRunLevelEditor(void);
void           dbShopTestSelectItem(void);
void           dbShopTestOpenIconBrowser(void);
void           dbShopTestOpenPortraitBrowser(void);

void           dbShopTestNoOperation(void);

// The level editor's label before a member's level, as IncreaseItemAndLevelLvLabel.
char           ShopTestLvLabel[4]     = "Lv";

// The item selector's prompt, before the item number.
char           ShopTestItemPrompt[12] = "Item No:";

// The item selector's help line, while the bags have room.
char           ShopTestItemHelp[24]   = "A:Get Item  B:Return";

// The item selector's line when the party's bags are full.
char           ShopTestItemFull[20]   = "ITEM FULL........";

SCENE_ENTRANCE SceneEntrances[]       = {
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
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    392,
                    0,
                    280,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    440,
                    0,
                    280,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    488,
                    0,
                    280,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    536,
                    0,
                    280,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    584,
                    0,
                    280,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_ORANGE_PONYTAIL,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    632,
                    0,
                    280,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_PICKAXE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    680,
                    0,
                    280,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_BLUE_DRESS,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    728,
                    0,
                    280,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_EMOTE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    440,
                    0,
                    248,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_EMOTE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    488,
                    0,
                    248,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_EMOTE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    536,
                    0,
                    248,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_EMOTE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    584,
                    0,
                    248,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_SIGN,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    728,
                    0,
                    360,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_SIGN,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    712,
                    0,
                    136,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_SIGN,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    712,
                    0,
                    200,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_PISTON,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    640,
                    0,
                    96,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_SEATED_IDOL,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    672,
                    0,
                    96,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_WINGED_FIGURE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    328,
                    0,
                    360,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    424,
                    0,
                    360,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_CHEST,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    456,
                    0,
                    360,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    488,
                    0,
                    360,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_FORTUNE_TELLER,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    520,
                    0,
                    360,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_EMOTE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    360,
                    0,
                    280,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_EMOTE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    536,
                    0,
                    200,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_EMOTE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    680,
                    0,
                    360,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_EMOTE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    712,
                    0,
                    160,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    {SCENE_TABLE_END},
};

CUE SceneCues[] = {
    {CUE_TALK, ACTOR_WEAPON_SHOPKEEPER, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestOpenWeaponShop)},
    {CUE_TALK, ACTOR_ARMOR_SHOPKEEPER, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestOpenArmorShop)},
    {CUE_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestOpenItemShop)},
    {CUE_TALK, ACTOR_GENERAL_STOREKEEPER, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestOpenGeneralStore)},
    {CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestOpenInn)},
    {CUE_TALK, ACTOR_SANCTUM_PRIEST, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestOpenSanctum)},
    {CUE_TALK, ACTOR_SUNSHINE, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestTalkToSunshine)},
    {CUE_TALK, ACTOR_SUNSHINES_WIFE, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestTalkToSunshinesWife)},
    {CUE_TALK,
     ACTOR_WEAPON_SHOP_MESSAGES, CONDITION_ALWAYS,
     SCRIPT_REF(dbShopTestBrowseWeaponShopMessages)},
    {CUE_TALK,
     ACTOR_ARMOR_SHOP_MESSAGES, CONDITION_ALWAYS,
     SCRIPT_REF(dbShopTestBrowseArmorShopMessages)},
    {CUE_TALK,
     ACTOR_ITEM_SHOP_MESSAGES, CONDITION_ALWAYS,
     SCRIPT_REF(dbShopTestBrowseItemShopMessages)},
    {CUE_TALK,
     ACTOR_SANCTUM_MESSAGES, CONDITION_ALWAYS,
     SCRIPT_REF(dbShopTestBrowseSanctumMessages)},
    {CUE_TALK, ACTOR_ITEM_PICKER, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestOpenItemPicker)},
    {CUE_TALK, ACTOR_NAME_ENTRY, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestEditPartyNames)},
    {CUE_TALK,
     ACTOR_MURAMASA_AND_SUMMON, CONDITION_ALWAYS,
     SCRIPT_REF(dbShopTestGiveMuramasaAndSummon)},
    {CUE_TALK, ACTOR_ICON_BROWSER, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestOpenIconBrowser)},
    {CUE_TALK, ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestOpenPortraitBrowser)},
    {CUE_TALK, ACTOR_LEVEL_EDITOR, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestRunLevelEditor)},
    {CUE_TALK, ACTOR_ITEM_SELECTOR, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestSelectItem)},
    {CUE_TALK, ACTOR_FORTUNE_TELLER, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestDamageAndAfflictParty)},
    {CUE_TALK, ACTOR_TREASURE, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestGiveEquipmentTestStock)},
    {CUE_TALK, ACTOR_DJINN, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestGivePartyDjinn)},
    {CUE_TALK, ACTOR_LIGHT_BULB_A, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestNoOperation)},
    {CUE_TALK, ACTOR_LIGHT_BULB_B, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestNoOperation)},
    {CUE_TALK, ACTOR_LIGHT_BULB_C, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestNoOperation)},
    {CUE_TALK, ACTOR_LIGHT_BULB_D, CONDITION_ALWAYS, SCRIPT_REF(dbShopTestNoOperation)},
    {CUE_TABLE_END},
};

// The functions after the tables.

// Takes HP and PP from Felix, Jenna and Sheba, poisons and haunts Felix, curses and envenoms Jenna,
// and breaks the first item each of Felix and Jenna holds that may break when used. Talking to
// ACTOR_FORTUNE_TELLER.
void dbShopTestDamageAndAfflictParty(void)
{
    UNIT            *unit;
    ITEM_DEFINITION *item;
    Sint32           i;

    // "The body is being torn apart."
    mnFieldMessageShow((Sint32)MsgTheBodyIs, FIELD_MESSAGE_ZOOM);
    scFieldUnitAddHp(FELIX, -100);
    scFieldUnitAddHp(JENNA, -100);
    scFieldUnitAddHp(SHEBA, -33);
    gmUnitAddPp(FELIX, -50);
    gmUnitAddPp(JENNA, -40);
    gmUnitAddPp(SHEBA, -35);
    unit         = syUnitGet(FELIX);
    unit->poison = POISON_NORMAL;
    unit->haunt  = 1;
    unit         = syUnitGet(JENNA);
    unit->curse  = CURSE_BOUND;
    unit->poison = POISON_VENOM;
    unit         = syUnitGet(FELIX);
    for (i = 0; i < INVENTORY_SLOTS; i++)
    {
        item = gmItemGetDirect(unit->inventory[i]);
        if (unit->inventory[i] == 0)
            break;
        if (item->useType == ITEM_USE_MAY_BREAK)
        {
            gmInventoryBreak(FELIX, i);
            break;
        }
    }
    unit = syUnitGet(JENNA);
    for (i = 0; i < INVENTORY_SLOTS; i++)
    {
        item = gmItemGetDirect(unit->inventory[i]);
        if (unit->inventory[i] == 0)
            break;
        if (item->useType == ITEM_USE_MAY_BREAK)
        {
            gmInventoryBreak(JENNA, i);
            break;
        }
    }
    gmFlagRefreshLureCap();
}

// Gives an owner one Djinni, marks it found and sets it if `activate` is 1.
void dbShopTestGiveDjinni(Sint32 owner, Sint32 element, Sint32 djinn, Sint32 activate)
{
    syGameFlagSet(FLAG_DJINNI(element * DJINN_PER_ELEMENT + djinn));
    gmDjinnGive(owner, element, djinn);
    if (activate == 1)
        gmDjinnSet(owner, element, djinn);
}

#if !SHOP_TEST_GIVES_EVERY_DJINNI
// Gives and sets the first Djinn of each element that The Lost Age adds: Felix Echo to Meld, Jenna
// Cannon to Core, Sheba Breath to Wheeze and Piers Fog to Rime. Talking to ACTOR_DJINN.
void dbShopTestGivePartyDjinn(void)
{
    Sint32 djinn;

    // "You got a Djinni!"
    mnFieldMessageShow((Sint32)MsgYouGotA, FIELD_MESSAGE_ZOOM);
    for (djinn = DJINNI_INDEX(DJINNI_ECHO); djinn <= DJINNI_INDEX(DJINNI_MELD); djinn++)
        dbShopTestGiveDjinni(FELIX, ELEMENT_VENUS, djinn, 1);
    for (djinn = DJINNI_INDEX(DJINNI_CANNON); djinn <= DJINNI_INDEX(DJINNI_CORE); djinn++)
        dbShopTestGiveDjinni(JENNA, ELEMENT_MARS, djinn, 1);
    for (djinn = DJINNI_INDEX(DJINNI_BREATH); djinn <= DJINNI_INDEX(DJINNI_WHEEZE); djinn++)
        dbShopTestGiveDjinni(SHEBA, ELEMENT_JUPITER, djinn, 1);
    for (djinn = DJINNI_INDEX(DJINNI_FOG); djinn <= DJINNI_INDEX(DJINNI_RIME); djinn++)
        dbShopTestGiveDjinni(PIERS, ELEMENT_MERCURY, djinn, 1);
    gmFlagRefreshLureCap();
}
#else
// Gives the party every Djinni.
void dbShopTestGivePartyDjinn(void)
{
    Sint32 element;
    Sint32 djinn;

    // "You got a Djinni!"
    mnFieldMessageShow((Sint32)MsgYouGotA, FIELD_MESSAGE_ZOOM);
    for (element = 0; element < ELEMENT_COUNT; element++)
    {
        for (djinn = 0; djinn < DJINNI_SPARE_FIRST; djinn++)
            gmDjinnGiveToParty(element, djinn);
    }
    gmFlagRefreshLureCap();
}
#endif

// The script for talking to ACTOR_LIGHT_BULB_A to ACTOR_LIGHT_BULB_D.
void dbShopTestNoOperation(void)
{
}

// Gives a Muramasa, then announces and unlocks Eclipse. Talking to ACTOR_MURAMASA_AND_SUMMON.
void dbShopTestGiveMuramasaAndSummon(void)
{
    fdPartyInventoryGiveItem(ITEM_MURAMASA, 0);
    mnSummonShowLearnedMessage(SUMMON_ECLIPSE);
    gmSummonLearn(SUMMON_ECLIPSE);
}

const CUE *scGetCues(void)
{
    return SceneCues;
}

// Opens on the backdrop fade at the fast message speed, shows "..." over the message browsers and a
// light bulb over ACTOR_LIGHT_BULB_A to ACTOR_LIGHT_BULB_D, and brings Isaac's party in when the
// party came in by ENTRANCE_WITH_ISAACS_PARTY.
Sint32 scInit(void)
{
    FIELD_STATE        *state      = RamHeapSlots->fieldState;
    Sint32             *transition = &state->transition;
    MAIN_ROUTINE_STATE *mainState  = &MainRoutineStateWork;

    *transition                    = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    mainState->msgSpeed            = MESSAGE_SPEED_FAST;
    fdActorSetAnimationById(ACTOR_SANCTUM_MESSAGES, EMOTE_ELLIPSIS);
    fdActorSetAnimationById(ACTOR_WEAPON_SHOP_MESSAGES, EMOTE_ELLIPSIS);
    fdActorSetAnimationById(ACTOR_ARMOR_SHOP_MESSAGES, EMOTE_ELLIPSIS);
    fdActorSetAnimationById(ACTOR_ITEM_SHOP_MESSAGES, EMOTE_ELLIPSIS);
    fdActorSetAnimationById(ACTOR_LIGHT_BULB_A, EMOTE_LIGHT_BULB);
    fdActorSetAnimationById(ACTOR_LIGHT_BULB_B, EMOTE_LIGHT_BULB);
    fdActorSetAnimationById(ACTOR_LIGHT_BULB_C, EMOTE_LIGHT_BULB);
    fdActorSetAnimationById(ACTOR_LIGHT_BULB_D, EMOTE_LIGHT_BULB);
    if (mainState->entrance == ENTRANCE_WITH_ISAACS_PARTY)
        gmPartyAddIsaacsParty();
    return 0;
}

Sint32 scPrepareMap(void)
{
    return 0;
}

// Raises every party member `levels` levels.
void dbShopTestRaisePartyLevels(Sint32 levels)
{
    Sint16 owners[16];
    Sint32 count = syFieldPartyListMembers(owners);
    Sint32 i;

    for (i = 0; i < count;)
    {
        Uint16 owner = owners[i++];

        gmUnitLevelUpTo(owner, syUnitGet(owner)->level + levels);
        gmUnitCalculate(owner);
    }
}

// The level-raising window, as Increase item and level's (dbIncreaseItemAndLevelRunLevelEditor):
// shows the leader's name and level, raises every party member 5 levels on Start or Select and 1 on
// A, and closes on B. Talking to ACTOR_LEVEL_EDITOR.
void dbShopTestRunLevelEditor(void)
{
    UNIT        *unit   = syUnitGet(MainRoutineStateWork.leaderActorId);
    void        *window = mnWindowCreate(0, 0, 30, 9, WINDOW_FLAG_INSTANT);
    const Uint8 *msg    = MsgRaiseEveryoneS;
    Sint32       redraw;

    mnTextDrawMessage(msg, window, 0, 0);
    mnTextDrawMessage(msg + 1, window, 0, 16);
    redraw = 1;
    mnTextDrawMessage(msg + 2, window, 0, 32);
    for (;;)
    {
        if (redraw)
        {
            mnWindowSpriteRedrawSavedRect(window);
            mnTextDrawStringAtOffset(unit->name, window, 0, 48);
            mnTextDrawStringInWindow(ShopTestLvLabel, window, 48, 48);
            redraw = 0;
            mnTextDrawNumberInWindow(unit->level, 0, window, 72, 48);
        }
        sySynch(1);
        if ((PerWork.push & KEY_START) || (PerWork.push & KEY_SELECT))
        {
            dbShopTestRaisePartyLevels(5);
            sySoundPlay(MUS_SUPER_LUCKY);
            redraw = 1;
        }
        if (PerWork.push & KEY_A)
        {
            dbShopTestRaisePartyLevels(1);
            sySoundPlay(MUS_LUCKY);
            redraw = 1;
        }
        if (PerWork.push & KEY_B)
            break;
    }
    sySoundPlay(SE_MENU_CANCEL);
    mnWindowClose(window, 2);
}

// The selector's item numbers, 0 to 499, wrapping.
#define ITEM_SELECTOR_ITEM_COUNT 500

// The item selector, as Increase item and level's (dbIncreaseItemAndLevelSelectItem): Up and Down
// move one item, Right and Left ten, R and L thirty, skipping items with no icon; A gives the item,
// B closes. Talking to ACTOR_ITEM_SELECTOR.
void dbShopTestSelectItem(void)
{
    void  *window;
    void  *details;
    Sint32 item;
    Sint32 redraw;
    Sint32 entryCount;
    Sint8  step;

    entryCount = ITEM_SELECTOR_ITEM_COUNT;
    step       = 0;
    item       = 1;
    redraw     = 1;
    window     = mnWindowCreate(0, 0, 30, 7, WINDOW_FLAG_INSTANT);
    details    = mnWindowCreate(0, 8, 13, 10, WINDOW_FLAG_INSTANT);
    syDMAXCopy((const void *)OBJ_PLTT,
               (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
               DMA_COPY_HALFWORDS(16),
               REG_DMA3);
    syDMAXCopy((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
               (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR),
               DMA_COPY_HALFWORDS(1),
               REG_DMA3);
    sySoundPlay(SE_MENU_CONFIRM);
    sySynch(1);
    while (1)
    {
        if (redraw)
        {
            redraw = 0;
            mnWindowSpritePrepareForRedraw(window);
            mnWindowSpritePrepareForRedraw(details);
            mnTextDrawStringInWindow(ShopTestItemPrompt, window, 0, 0);
            mnTextDrawNumberAtOffset(item, 0, window, 80, 0);
            if (gmPartyInventoryHasSpace())
            {
                mnTextDrawStringInWindow(ShopTestItemHelp, window, 0, 32);
                mnTextDrawMessageAsTiles((Sint32)(item + MsgItemName), window, 120, 0);
                mnTextDrawMessage((Sint32)(item + MsgItemDescription), window, 0, 16);
                mnItemMenuDrawItemDetails((Sint32)details, item);
            }
            else
            {
                mnTextDrawStringInWindow(ShopTestItemFull, window, 0, 32);
            }
        }
        sySynch(1);
        if (PerWork.push & KEY_A)
        {
            if (gmPartyInventoryAdd(item) == -1)
            {
                sySoundPlay(SE_MENU_CANCEL);
                break;
            }
            sySoundPlay(SE_NOTE_HIGH);
        }
        if (PerWork.push & KEY_B)
        {
            sySoundPlay(SE_MENU_CANCEL);
            break;
        }
        if (PerWork.repeat & KEY_DOWN)
        {
            step = 1;
            item++;
            redraw = 1;
            sySoundPlay(SE_MENU_CURSOR_MOVE);
        }
        if (PerWork.repeat & KEY_UP)
        {
            step = -1;
            item--;
            redraw = 1;
            sySoundPlay(SE_MENU_CURSOR_MOVE);
        }
        if (PerWork.repeat & KEY_RIGHT)
        {
            step    = 1;
            item   += DEBUG_BROWSER_ROW_STEP;
            redraw  = 1;
            sySoundPlay(SE_MENU_CURSOR_MOVE);
        }
        if (PerWork.repeat & KEY_LEFT)
        {
            step    = -1;
            item   -= DEBUG_BROWSER_ROW_STEP;
            redraw  = 1;
            sySoundPlay(SE_MENU_CURSOR_MOVE);
        }
        if (PerWork.repeat & KEY_R)
        {
            step    = 1;
            item   += DEBUG_BROWSER_PAGE_STEP;
            redraw  = 1;
            sySoundPlay(SE_MENU_CURSOR_MOVE);
        }
        if (PerWork.repeat & KEY_L)
        {
            step    = -1;
            item   -= DEBUG_BROWSER_PAGE_STEP;
            redraw  = 1;
            sySoundPlay(SE_MENU_CURSOR_MOVE);
        }
        if (step == -1)
        {
            for (item = (item + ITEM_SELECTOR_ITEM_COUNT) % entryCount;
                 gmItemGetDirect(item & INVENTORY_ITEM_ID_MASK)->icon == 0;
                 item = (item + ITEM_SELECTOR_ITEM_COUNT - 1) % entryCount)
                ;
        }
        if (step == 1)
        {
            for (item = (item + ITEM_SELECTOR_ITEM_COUNT) % entryCount;
                 gmItemGetDirect(item & INVENTORY_ITEM_ID_MASK)->icon == 0;
                 item = (item + ITEM_SELECTOR_ITEM_COUNT + 1) % entryCount)
                ;
        }
        step = 0;
    }
    mnWindowClose(window, 2);
    mnWindowClose(details, 2);
}

// Talking to ACTOR_ICON_BROWSER: pages of item, Psynergy and status icons.
void dbShopTestOpenIconBrowser(void)
{
    mnDebugBrowserIcons();
}

// Talking to ACTOR_PORTRAIT_BROWSER: the portraits, one at a time.
void dbShopTestOpenPortraitBrowser(void)
{
    mnDebugBrowserPortraits();
}

// Unused: the Psynergy test's dbPsynergyTestDigMetatiles again.
UNUSED void dbShopTestDigMetatiles(
    Sint32 stage, Sint32 srcX, Sint32 srcY, Sint32 width, Sint32 height, Sint32 destX, Sint32 destY)
{
    if (stage == SCOOP_STAGE_DUG)
        fdMapCopyMetatilesRect(srcX, srcY, width, height, destX, destY);
}

// Unused: the Psynergy test's dbPsynergyTestGrowActor again.
UNUSED void dbShopTestGrowActor(Sint32 stage, Sint32 id, Sint32 x, Sint32 z)
{
    ACTOR *actor;
    Sint32 scale;

    actor = fdActorGetById(id);
    if (stage == SCOOP_STAGE_DUG)
    {
        fdActorSetPos(id, x, z);
        actor->scaleX = 0;
        actor->scaleY = 0;
    }
    if (stage == SCOOP_STAGE_DUG_UP)
    {
        while (actor->scaleX < toFIXED(1))
        {
            sySynch(1);
            scale          = actor->scaleX;
            scale         += toFIXED(0.02);
            actor->scaleX  = scale;
            actor->scaleY  = scale;
        }
        actor->scaleX = toFIXED(1);
        actor->scaleY = toFIXED(1);
    }
}

// Unused: the Psynergy test's dbPsynergyTestDropActor again.
UNUSED void dbShopTestDropActor(Sint32 stage, Sint32 id, Sint32 x, Sint32 z)
{
    ACTOR *actor;

    actor = fdActorGetById(id);
    if (stage == SCOOP_STAGE_DUG_UP)
    {
        fdActorSetPos(id, x, z);
        actor->y.fixed     = toFIXED(32);
        actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        actor->gravity     = toFIXED(0.09);
        sySynch(50);
    }
}

// Unused: the Psynergy test's dbPsynergyTestRaiseStatue again, which starts the battle in slot
// `slot` of encounter zone `zone`.
UNUSED void dbShopTestRaiseStatue(Sint32 stage, Sint32 x, Sint32 z, Sint32 zone, Sint32 slot)
{
    ACTOR *actor;

    if (stage == SCOOP_STAGE_DUG)
    {
        actor = scFieldActorCreate(SPRITE_BROWN_BOULDER, toFIXED(x), 0, toFIXED(z));
        if (actor != NULL)
        {
            fdActorSetSpriteFlags(actor, 0);
            actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        }
    }
    if (stage == SCOOP_STAGE_DUG_UP)
    {
        actor = ((ACTOR *)syImportActorCreate(SPRITE_WINGED_STATUE, toFIXED(x), 0, toFIXED(z)));
        if (actor != NULL)
        {
            Sint32 scale;

            fdActorSetSpriteFlags(actor, 0);
            actor->velocityY = toFIXED(6);
            actor->scaleX    = toFIXED(2) / 5;
            actor->scaleY    = toFIXED(2) / 5;
            while (actor->scaleX < toFIXED(1))
            {
                sySynch(1);
                scale          = actor->scaleX + toFIXED(0.02);
                actor->facing += FACING_STEP;
                actor->scaleX  = scale;
                actor->scaleY  = scale;
            }
            actor->scaleX = toFIXED(1);
            actor->scaleY = toFIXED(1);
            actor->facing =
                fdActorGetById(MainRoutineStateWork.leaderActorId)->facing + DEGtoANG(180);
            fdRequestBattleFromEncounterTable(zone, slot);
        }
    }
}
