/// @file
/// The debug room Shop test, whose people each run a test when talked to: the
/// inn, the sanctum and four shops, Sunshine's forge, the shop and sanctum
/// messages, the party's names, items, Djinn and levels, and the icon and
/// portrait browsers. Its tables sit among its routines, before
/// ShopTest_DamageAndAfflictParty.

#include "flags.h"
#include "window/message_argument.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "system/io_reg.h"
#include "sounds.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "system/input.h"
#include "text/msg_ids.h"
#include "field/event_inline.h"
#include "system/main_routine_work.h"

#include "game/inventory.h"
#include "item_ids.h"
#include "game/owner_state.h"
#include "version.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "system/dma.h"
#include "field/actor.h"
#include "field/field_sequence.h"
#include "fixed_math.h"
#include "battle/battle_unit.h"
#include "window/window.h"
#include "shop/shop.h"

/// The entrance that brings Isaac's party in (Party_AddIsaacsParty).
#define ENTRANCE_WITH_ISAACS_PARTY 99

// Set while Sunshine forges what the party brought him, as Yallam's
// FLAG_SUNSHINE_FORGING is; here only leaving the room clears it.
#define FLAG_SHOP_TEST_SUNSHINE_FORGING FLAG_SCENE_TEMP_00

/// The room's people, numbered in the order of gActorPlacements: the shops'
/// people, then the testers, each named for what talking to it opens
/// (gSceneEvents).
enum ShopTestActorId {
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
  /// Talking to her takes HP and PP and afflicts the party
  /// (ShopTest_DamageAndAfflictParty).
  ACTOR_FORTUNE_TELLER,
  /// Four light bulbs, which do nothing when talked to (ShopTest_NoOp).
  ACTOR_LIGHT_BULB_A,
  ACTOR_LIGHT_BULB_B,
  ACTOR_LIGHT_BULB_C,
  ACTOR_LIGHT_BULB_D
};

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

void *Portrait_OpenWindow(s32, s32, s32, s32);
void Message_CloseFinishedWindows(void);
void Message_OpenWindow(s32, s32, s32, s32);
s32 Message_AllTyped(void);

/// Browses `count` messages from `message`, each typed out at once: A for the
/// next, B to close; each moves on by itself after 10 frames held, or 60
/// frames before it is typed out.
void ShopTest_BrowseMessageRange(s32 message, s32 count) {
  s32 i;
  s32 frames;
  void *window;
  s32 typed;

  window = Portrait_OpenWindow(SPRITE_YOUNG_MAN_IN_GREEN, 0, 0, 0);
  for (i = 0; i < count; i++) {
    // A sample name, item and number for each message to show: Garet, the
    // Shamshir and a number of six digits.
    MessageArgument_Push(GARET, MESSAGE_ARGUMENT_ACTOR);
    MessageArgument_Push(ITEM_SHAMSHIR, MESSAGE_ARGUMENT_ITEM);
    MessageArgument_Push(123456, MESSAGE_ARGUMENT_NUMBER);
    Message_CloseFinishedWindows();
    Message_OpenWindow(message, 5, 0,
                       MESSAGE_WINDOW_FIXED_X | MESSAGE_WINDOW_UNREAD);
    frames = 0;
    if (!(gInput.held & (KEY_A | KEY_B))) {
      do {
        wait(1);
        frames++;
        if (frames > 59)
          break;
      } while (!(gInput.held & (KEY_A | KEY_B)));
    }
    for (;;) {
      typed = Message_AllTyped();
      if (typed != 0)
        break;
      gInput.held = KEY_A;
      gInput.pressed = KEY_A;
      gInput.repeat = KEY_A;
      wait(1);
    }
    frames = 0;
    if (gInput.held & KEY_B)
      break;
    if (!(gInput.held & KEY_A)) {
      do {
        wait(1);
        frames++;
        if (frames > 9)
          break;
        if (gInput.held & KEY_B)
          goto done;
      } while (!(gInput.held & KEY_A));
    }
    message++;
  }
done:
  Message_CloseFinishedWindows();
  Window_Close(window, 2);
}

void Shop_Run(s32 stockRow, s32 keeper);
void Sanctum_Run(void);
void Inn_CheckIn(s32 innId, s32 keeper);
void ItemPicker_RunToShow(s32 *selectedUnit, s32 *selectedSlot);
void NameEntry_EditMemberName(s32 index);
void Summon_ShowLearnedMessage(s32 value);
void Summon_Learn(s32 summonId);
void DebugBrowser_Icons(void);
void DebugBrowser_Portraits(void);

/// The shop and sanctum message browsers: talking to ACTOR_SANCTUM_MESSAGES and
/// the three after it.
void ShopTest_BrowseWeaponShopMessages(void) {
  ShopTest_BrowseMessageRange((s32)MsgWeaponShopWelcome, SHOP_MESSAGES);
}

/// Browses the armor shop's messages.
void ShopTest_BrowseArmorShopMessages(void) {
  ShopTest_BrowseMessageRange((s32)MsgArmorShopWelcome, SHOP_MESSAGES);
}

/// Browses the item shop's messages.
void ShopTest_BrowseItemShopMessages(void) {
  ShopTest_BrowseMessageRange((s32)MsgItemShopWelcome, SHOP_MESSAGES);
}

/// Browses the Sanctum's messages.
void ShopTest_BrowseSanctumMessages(void) {
  ShopTest_BrowseMessageRange((s32)MsgWelcomeWearyWanderers, SANCTUM_MESSAGES);
}

/// The shops and the inn, each passed the keeper's actor id.
void ShopTest_OpenWeaponShop(s32 keeper) {
  Shop_Run(SHOP_DAILA_WEAPONS, keeper);
}

/// Opens the armor shop.
void ShopTest_OpenArmorShop(s32 keeper) { Shop_Run(SHOP_DAILA_ARMOR, keeper); }

/// Opens the item shop.
void ShopTest_OpenItemShop(s32 keeper) { Shop_Run(SHOP_DAILA_ITEMS, keeper); }

/// Opens the general store.
void ShopTest_OpenGeneralStore(s32 keeper) { Shop_Run(SHOP_IZUMO, keeper); }

/// Opens the inn.
void ShopTest_OpenInn(s32 keeper) { Inn_CheckIn(DAILA_INN, keeper); }

/// Opens the Sanctum.
void ShopTest_OpenSanctum(void) { Sanctum_Run(); }

s32 PartyInventory_FindRoom(s32 item);
void ItemDisplay_ShowItem(s32 item, s32 mode);
void Forge_Run(s32 actor);
void Forge_SetItem(s32 value);
s32 Forge_GetItem(void);

/// Sunshine, as in Yallam (YallamRooms_TalkToSunshine) without his walk to
/// the anvil: while FLAG_FORGE_OPEN is set he forges what the party
/// brings.
void ShopTest_TalkToSunshine(s32 actor) {
  s32 message;

  if (!Event_GetFlag(FLAG_FORGE_OPEN)) {
    message = (s32)MsgAhSoYou;
    Event_SetMessageId(message);
    // "Ah, so you're looking for a really unique item, huh?"
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(message + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(message + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
  } else if (Event_GetFlag(FLAG_SHOP_TEST_SUNSHINE_FORGING)) {
    Event_SetMessageId((s32)MsgICanTWait);
    // "I can't wait to see what I'm going to wind up making with this......"
    Event_ShowMessageAndWait(actor, 0);
  } else if (Forge_GetItem()) {
    Event_SetMessageId((s32)MsgMyWifeHas);
    // "My wife has the item I made you. She can give it to you."
    Event_ShowMessageAndWait(actor, 0);
  } else {
    message = (s32)MsgYouWantMe;
    Event_SetMessageId(message);
    // "You want me to make you a little something special?"
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(message + 1);
      // "Well, look at that! You went to get some raw materials for me..."
      Event_ShowMessageAndWait(actor, 0);
      Forge_Run(actor);
      if (Forge_GetItem())
        Event_SetFlag(FLAG_SHOP_TEST_SUNSHINE_FORGING);
    } else {
      Event_SetMessageId(message + 2);
      // "Hm... Maybe I made a mistake. It's hard to tell... If you want..."
      Event_ShowMessageAndWait(actor, 0);
    }
  }
}

/// Sunshine's wife, as in Yallam (YallamRooms_TalkToSunshinesWife): she
/// hands over what Sunshine made and takes the payment.
void ShopTest_TalkToSunshinesWife(s32 actor) {
  if (!Event_GetFlag(FLAG_FORGE_OPEN)) {
    Event_SetMessageId((s32)MsgThisThisIs);
    // "This? This is the blacksmith Sunshine's smithy, the best forge in..."
    Event_ShowMessageAndWait(actor, 0);
  } else if (Event_GetFlag(FLAG_SHOP_TEST_SUNSHINE_FORGING)) {
    Event_SetMessageId((s32)MsgItTakesOur);
    // "It takes our Sunshine a long time to finish the items he works on...."
    Event_ShowMessageAndWait(actor, 0);
  } else if (Forge_GetItem()) {
    s32 item = Forge_GetItem();
    u32 price = (u16)Item_GetDirect(item)->price;
    s32 message;

    MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
    MessageArgument_Push(price, MESSAGE_ARGUMENT_NUMBER);
    message = (s32)MsgHereSThe;
    Event_SetMessageId(message);
    // "Here's the item you wanted. Now, about the payment... We only take..."
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) != 0) {
      Event_SetMessageId(message + 1);
      // "Hey, we're not a charity! If you can't pay for it, you don't get it!"
      Event_ShowMessageAndWait(actor, 0);
    } else if (PartyInventory_FindRoom(item) < 0) {
      Event_SetMessageId(message + 2);
      // "Whoops! Looks like you've got a full load there! This'll have to..."
      Event_ShowMessageAndWait(actor, 0);
    } else if ((u32)gMainRoutineWork.coins < price) {
      Event_SetMessageId(message + 3);
      // "You don't enough money! What are you trying to pull? I can't give..."
      Event_ShowMessageAndWait(actor, 0);
    } else {
      Event_SetMessageId(message + 4);
      // "OK, here you go!"
      Event_ShowMessageAndWait(actor, 0);
      ItemDisplay_ShowItem(item, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
      PartyInventory_GiveItem(item, 0);
      Forge_SetItem(0);
      Party_AdjustCoins(-price);
    }
  } else {
    Event_SetMessageId((s32)MsgManSunshineWould);
    // "Man, Sunshine would be rich if he worked more. What a waste..."
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// Opens the item picker for an item to show, as Naribwe's fortune-teller asks
/// for one, and discards the member and slot chosen.
void ShopTest_TestItemSelection(void) {
  s32 selection[2];

  ItemPicker_RunToShow(&selection[1], &selection[0]);
}

/// The name entry screen for each of the eight characters in turn.
void ShopTest_EditPartyNames(void) {
  s32 i;

  for (i = 0; i < PARTY_UNIT_COUNT; i++)
    NameEntry_EditMemberName(i);
}

/// Adds an item to a member's inventory, from an event.
static inline void Event_GiveItemTo(s32 item, s32 owner) {
  Inventory_AddItem(owner, item);
}

/// Gives 777,777 coins, ten each of Game Tickets and Lucky Medals and test
/// equipment to Felix, Jenna, Sheba and Piers; then Isaac, Garet, Ivan and Mia
/// leave the party. Talking to ACTOR_TREASURE.
void ShopTest_GiveEquipmentTestStock(void) {
  s32 i;

  // "You got treasure!"
  FieldMessage_Show((s32)MsgYouGotTreasure, FIELD_MESSAGE_ZOOM);
  gMainRoutineWork.coins = 777777;
  for (i = 0; i < 10; i++) {
    Event_GiveItemTo(ITEM_GAME_TICKET, FELIX);
    Event_GiveItemTo(ITEM_LUCKY_MEDAL, FELIX);
  }
  Event_GiveItemTo(ITEM_HERMES_WATER, FELIX);
  Event_GiveItemTo(ITEM_HALT_GEM, FELIX);
  Event_GiveItemTo(ITEM_MYTHRIL_BAG, FELIX);
  Event_GiveItemTo(ITEM_MURAMASA, FELIX);
  Event_GiveItemTo(ITEM_FAIRY_RING, FELIX);
  Event_GiveItemTo(ITEM_MYTHRIL_BAG_JUPITER_STAR, FELIX);
  Event_GiveItemTo(ITEM_SMOKE_BOMB, JENNA);
  Event_GiveItemTo(ITEM_SLEEP_BOMB, JENNA);
  Event_GiveItemTo(ITEM_DRAGONS_EYE, JENNA);
  Event_GiveItemTo(ITEM_ANCHOR_CHARM, JENNA);
  Event_GiveItemTo(ITEM_BONE, JENNA);
  Event_GiveItemTo(ITEM_MYSTIC_DRAUGHT, JENNA);
  Event_GiveItemTo(ITEM_HEALING_RING, JENNA);
  Event_GiveItemTo(ITEM_BLACK_CRYSTAL, SHEBA);
  Event_GiveItemTo(ITEM_FUR_BOOTS, SHEBA);
  Event_GiveItemTo(ITEM_RUNNING_SHIRT, SHEBA);
  Event_GiveItemTo(ITEM_TEAR_STONE, SHEBA);
  Event_GiveItemTo(ITEM_TEAR_STONE, SHEBA);
  Event_GiveItemTo(ITEM_TEAR_STONE, SHEBA);
  Event_GiveItemTo(ITEM_SALAMANDER_TAIL, SHEBA);
  Event_GiveItemTo(ITEM_MYTHRIL_SILVER, SHEBA);
  Event_GiveItemTo(ITEM_RUSTY_SWORD, SHEBA);
  Event_GiveItemTo(ITEM_RUSTY_SWORD, SHEBA);
  Event_GiveItemTo(ITEM_RUSTY_SWORD_ROBBERS_BLADE, SHEBA);
  Event_GiveItemTo(ITEM_RUSTY_SWORD_PIRATES_SABRE, SHEBA);
  Event_GiveItemTo(ITEM_WATER_OF_LIFE, PIERS);
  Event_GiveItemTo(ITEM_ORB_OF_FORCE, PIERS);
  Event_GiveItemTo(ITEM_DOUSE_DROP, PIERS);
  Event_GiveItemTo(ITEM_FROST_JEWEL, PIERS);
  Event_GiveItemTo(ITEM_LIFTING_GEM, PIERS);
  Event_GiveItemTo(ITEM_HALT_GEM, PIERS);
  Event_GiveItemTo(ITEM_CATCH_BEADS, PIERS);
  Party_RemoveMember(ISAAC);
  Party_RemoveMember(GARET);
  Party_RemoveMember(IVAN);
  Party_RemoveMember(MIA);
}

void ShopTest_DamageAndAfflictParty(void);
void ShopTest_GivePartyDjinn(void);
void ShopTest_GiveMuramasaAndSummon(void);
void ShopTest_RunLevelEditor(void);
void ShopTest_SelectItem(void);
void ShopTest_OpenIconBrowser(void);
void ShopTest_OpenPortraitBrowser(void);

void ShopTest_NoOp(void);

/// The level editor's label before a member's level, as
/// gIncreaseItemAndLevel_LvLabel.
char gShopTest_LvLabel[4] = "Lv";

/// The item selector's prompt, before the item number.
char gShopTest_ItemPrompt[12] = "Item No:";

/// The item selector's help line, while the bags have room.
char gShopTest_ItemHelp[24] = "A:Get Item  B:Return";

/// The item selector's line when the party's bags are full.
char gShopTest_ItemFull[20] = "ITEM FULL........";

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 408, 0, 312, FACING_NORTH, 0, 280, 0, 780, 464, 0},
    {1, CONDITION_ALWAYS, 616, 0, 328, FACING_SOUTH, 0, 280, 0, 780, 464, 0},
    {2, CONDITION_ALWAYS, 648, 0, 152, FACING_NORTH, 0, 280, 0, 780, 464, 0},
    {3, CONDITION_ALWAYS, 408, 0, 88, FACING_SOUTH, 0, 280, 0, 780, 464, 0},
    {4, CONDITION_ALWAYS, 408, 0, 408, FACING_NORTH, 0, 280, 0, 780, 464, 0},
    {5, CONDITION_ALWAYS, 504, 0, 232, FACING_SOUTH, 0, 280, 0, 780, 464, 0},
    {SCENE_TABLE_END},
};

#include "../scene/shared/psynergy_test_exits.c"

struct ActorPlacement gActorPlacements[] = {
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

struct Trigger gSceneEvents[] = {
    {TRIGGER_TALK, ACTOR_WEAPON_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(ShopTest_OpenWeaponShop)},
    {TRIGGER_TALK, ACTOR_ARMOR_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(ShopTest_OpenArmorShop)},
    {TRIGGER_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(ShopTest_OpenItemShop)},
    {TRIGGER_TALK, ACTOR_GENERAL_STOREKEEPER, CONDITION_ALWAYS,
     EVENT(ShopTest_OpenGeneralStore)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(ShopTest_OpenInn)},
    {TRIGGER_TALK, ACTOR_SANCTUM_PRIEST, CONDITION_ALWAYS,
     EVENT(ShopTest_OpenSanctum)},
    {TRIGGER_TALK, ACTOR_SUNSHINE, CONDITION_ALWAYS,
     EVENT(ShopTest_TalkToSunshine)},
    {TRIGGER_TALK, ACTOR_SUNSHINES_WIFE, CONDITION_ALWAYS,
     EVENT(ShopTest_TalkToSunshinesWife)},
    {TRIGGER_TALK, ACTOR_WEAPON_SHOP_MESSAGES, CONDITION_ALWAYS,
     EVENT(ShopTest_BrowseWeaponShopMessages)},
    {TRIGGER_TALK, ACTOR_ARMOR_SHOP_MESSAGES, CONDITION_ALWAYS,
     EVENT(ShopTest_BrowseArmorShopMessages)},
    {TRIGGER_TALK, ACTOR_ITEM_SHOP_MESSAGES, CONDITION_ALWAYS,
     EVENT(ShopTest_BrowseItemShopMessages)},
    {TRIGGER_TALK, ACTOR_SANCTUM_MESSAGES, CONDITION_ALWAYS,
     EVENT(ShopTest_BrowseSanctumMessages)},
    {TRIGGER_TALK, ACTOR_ITEM_PICKER, CONDITION_ALWAYS,
     EVENT(ShopTest_TestItemSelection)},
    {TRIGGER_TALK, ACTOR_NAME_ENTRY, CONDITION_ALWAYS,
     EVENT(ShopTest_EditPartyNames)},
    {TRIGGER_TALK, ACTOR_MURAMASA_AND_SUMMON, CONDITION_ALWAYS,
     EVENT(ShopTest_GiveMuramasaAndSummon)},
    {TRIGGER_TALK, ACTOR_ICON_BROWSER, CONDITION_ALWAYS,
     EVENT(ShopTest_OpenIconBrowser)},
    {TRIGGER_TALK, ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT(ShopTest_OpenPortraitBrowser)},
    {TRIGGER_TALK, ACTOR_LEVEL_EDITOR, CONDITION_ALWAYS,
     EVENT(ShopTest_RunLevelEditor)},
    {TRIGGER_TALK, ACTOR_ITEM_SELECTOR, CONDITION_ALWAYS,
     EVENT(ShopTest_SelectItem)},
    {TRIGGER_TALK, ACTOR_FORTUNE_TELLER, CONDITION_ALWAYS,
     EVENT(ShopTest_DamageAndAfflictParty)},
    {TRIGGER_TALK, ACTOR_TREASURE, CONDITION_ALWAYS,
     EVENT(ShopTest_GiveEquipmentTestStock)},
    {TRIGGER_TALK, ACTOR_DJINN, CONDITION_ALWAYS,
     EVENT(ShopTest_GivePartyDjinn)},
    {TRIGGER_TALK, ACTOR_LIGHT_BULB_A, CONDITION_ALWAYS, EVENT(ShopTest_NoOp)},
    {TRIGGER_TALK, ACTOR_LIGHT_BULB_B, CONDITION_ALWAYS, EVENT(ShopTest_NoOp)},
    {TRIGGER_TALK, ACTOR_LIGHT_BULB_C, CONDITION_ALWAYS, EVENT(ShopTest_NoOp)},
    {TRIGGER_TALK, ACTOR_LIGHT_BULB_D, CONDITION_ALWAYS, EVENT(ShopTest_NoOp)},
    {TRIGGER_TABLE_END},
};

// The routines after the tables.

void Inventory_Break(s32 owner, s32 slot);
void GameFlag_RefreshLureCap(void);

/// Takes HP and PP from Felix, Jenna and Sheba, poisons and haunts Felix,
/// curses and envenoms Jenna, and breaks the first item each of Felix and
/// Jenna holds that may break when used. Talking to ACTOR_FORTUNE_TELLER.
void ShopTest_DamageAndAfflictParty(void) {
  struct Unit *unit;
  struct ItemDefinition *item;
  s32 i;

  // "The body is being torn apart."
  FieldMessage_Show((s32)MsgTheBodyIs, FIELD_MESSAGE_ZOOM);
  FieldUnit_AddHp(FELIX, -100);
  FieldUnit_AddHp(JENNA, -100);
  FieldUnit_AddHp(SHEBA, -33);
  Unit_AddPp(FELIX, -50);
  Unit_AddPp(JENNA, -40);
  Unit_AddPp(SHEBA, -35);
  unit = Unit_Get(FELIX);
  unit->poison = POISON_NORMAL;
  unit->haunt = 1;
  unit = Unit_Get(JENNA);
  unit->curse = CURSE_BOUND;
  unit->poison = POISON_VENOM;
  unit = Unit_Get(FELIX);
  for (i = 0; i < INVENTORY_SLOTS; i++) {
    item = Item_GetDirect(unit->inventory[i]);
    if (unit->inventory[i] == 0)
      break;
    if (item->useType == ITEM_USE_MAY_BREAK) {
      Inventory_Break(FELIX, i);
      break;
    }
  }
  unit = Unit_Get(JENNA);
  for (i = 0; i < INVENTORY_SLOTS; i++) {
    item = Item_GetDirect(unit->inventory[i]);
    if (unit->inventory[i] == 0)
      break;
    if (item->useType == ITEM_USE_MAY_BREAK) {
      Inventory_Break(JENNA, i);
      break;
    }
  }
  GameFlag_RefreshLureCap();
}

/// Gives an owner one Djinni, marks it found and sets it if `activate` is 1.
void ShopTest_GiveDjinni(s32 owner, s32 element, s32 djinn, s32 activate) {
  GameFlag_Set(FLAG_DJINNI(element * DJINN_PER_ELEMENT + djinn));
  Djinn_Give(owner, element, djinn);
  if (activate == 1)
    Djinn_Set(owner, element, djinn);
}

#if !TLA_PATCH_1
/// Gives and sets the first Djinn of each element that The Lost Age adds:
/// Felix Echo to Meld, Jenna Cannon to Core, Sheba Breath to Wheeze and Piers
/// Fog to Rime. Talking to ACTOR_DJINN.
void ShopTest_GivePartyDjinn(void) {
  s32 djinn;

  // "You got a Djinni!"
  FieldMessage_Show((s32)MsgYouGotA, FIELD_MESSAGE_ZOOM);
  for (djinn = DJINNI_INDEX(DJINNI_ECHO); djinn <= DJINNI_INDEX(DJINNI_MELD);
       djinn++)
    ShopTest_GiveDjinni(FELIX, ELEMENT_VENUS, djinn, 1);
  for (djinn = DJINNI_INDEX(DJINNI_CANNON); djinn <= DJINNI_INDEX(DJINNI_CORE);
       djinn++)
    ShopTest_GiveDjinni(JENNA, ELEMENT_MARS, djinn, 1);
  for (djinn = DJINNI_INDEX(DJINNI_BREATH);
       djinn <= DJINNI_INDEX(DJINNI_WHEEZE); djinn++)
    ShopTest_GiveDjinni(SHEBA, ELEMENT_JUPITER, djinn, 1);
  for (djinn = DJINNI_INDEX(DJINNI_FOG); djinn <= DJINNI_INDEX(DJINNI_RIME);
       djinn++)
    ShopTest_GiveDjinni(PIERS, ELEMENT_MERCURY, djinn, 1);
  GameFlag_RefreshLureCap();
}
#else
/// Gives the party every Djinni.
void ShopTest_GivePartyDjinn(void) {
  s32 element;
  s32 djinn;

  // "You got a Djinni!"
  FieldMessage_Show((s32)MsgYouGotA, FIELD_MESSAGE_ZOOM);
  for (element = 0; element < ELEMENT_COUNT; element++) {
    for (djinn = 0; djinn < DJINNI_SPARE_FIRST; djinn++)
      Djinn_GiveToParty(element, djinn);
  }
  GameFlag_RefreshLureCap();
}
#endif

/// The event for talking to ACTOR_LIGHT_BULB_A to ACTOR_LIGHT_BULB_D.
void ShopTest_NoOp(void) {}

/// Gives a Muramasa, then announces and unlocks Eclipse. Talking to
/// ACTOR_MURAMASA_AND_SUMMON.
void ShopTest_GiveMuramasaAndSummon(void) {
  PartyInventory_GiveItem(ITEM_MURAMASA, 0);
  Summon_ShowLearnedMessage(SUMMON_ECLIPSE);
  Summon_Learn(SUMMON_ECLIPSE);
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

void Party_AddIsaacsParty(void);

/// Opens on the backdrop fade at the fast message speed, shows "..." over the
/// message browsers and a light bulb over ACTOR_LIGHT_BULB_A to
/// ACTOR_LIGHT_BULB_D, and brings Isaac's party in when the party came in by
/// ENTRANCE_WITH_ISAACS_PARTY.
s32 Scene_Initialize(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 *effect = &work->transition;
  struct MainRoutineWork *party = &gMainRoutineWork;

  *effect = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  party->messageSpeed = MESSAGE_SPEED_FAST;
  Actor_SetAnimationById(ACTOR_SANCTUM_MESSAGES, EMOTE_ELLIPSIS);
  Actor_SetAnimationById(ACTOR_WEAPON_SHOP_MESSAGES, EMOTE_ELLIPSIS);
  Actor_SetAnimationById(ACTOR_ARMOR_SHOP_MESSAGES, EMOTE_ELLIPSIS);
  Actor_SetAnimationById(ACTOR_ITEM_SHOP_MESSAGES, EMOTE_ELLIPSIS);
  Actor_SetAnimationById(ACTOR_LIGHT_BULB_A, EMOTE_LIGHT_BULB);
  Actor_SetAnimationById(ACTOR_LIGHT_BULB_B, EMOTE_LIGHT_BULB);
  Actor_SetAnimationById(ACTOR_LIGHT_BULB_C, EMOTE_LIGHT_BULB);
  Actor_SetAnimationById(ACTOR_LIGHT_BULB_D, EMOTE_LIGHT_BULB);
  if (party->entrance == ENTRANCE_WITH_ISAACS_PARTY)
    Party_AddIsaacsParty();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Unit_Calculate(s32 owner);

/// Raises every party member `levels` levels.
void ShopTest_AdjustPartyLevels(s32 levels) {
  s16 owners[16];
  s32 count = FieldParty_ListMembers(owners);
  s32 i;

  for (i = 0; i < count;) {
    u16 owner = owners[i++];

    Unit_LevelUpTo(owner, Unit_Get(owner)->level + levels);
    Unit_Calculate(owner);
  }
}

void Text_DrawMessage(s32 message, s32 window, s32 x, s32 y);
void Text_DrawStringAtOffset(const void *text, void *window, s32 x, s32 y);

/// The level-raising window, as Increase item and level's
/// (IncreaseItemAndLevel_RunLevelEditor): shows the leader's name and level,
/// raises every party member 5 levels on Start or Select and 1 on A, and
/// closes on B. Talking to ACTOR_LEVEL_EDITOR.
void ShopTest_RunLevelEditor(void) {
  struct Unit *unit = Unit_Get(gMainRoutineWork.leaderActorId);
  void *window = Window_Create(0, 0, 30, 9, WINDOW_FLAG_INSTANT);
  const u8 *message = MsgRaiseEveryoneS;
  s32 redraw;

  Text_DrawMessage(message, window, 0, 0);
  Text_DrawMessage(message + 1, window, 0, 16);
  redraw = 1;
  Text_DrawMessage(message + 2, window, 0, 32);
  for (;;) {
    if (redraw) {
      WindowSprite_RedrawSavedRect(window);
      Text_DrawStringAtOffset(unit->name, window, 0, 48);
      Text_DrawStringInWindow(gShopTest_LvLabel, window, 48, 48);
      redraw = 0;
      Text_DrawNumberInWindow(unit->level, 0, window, 72, 48);
    }
    wait(1);
    if ((gInput.pressed & KEY_START) || (gInput.pressed & KEY_SELECT)) {
      ShopTest_AdjustPartyLevels(5);
      Sound_Play(MUS_SUPER_LUCKY);
      redraw = 1;
    }
    if (gInput.pressed & KEY_A) {
      ShopTest_AdjustPartyLevels(1);
      Sound_Play(MUS_LUCKY);
      redraw = 1;
    }
    if (gInput.pressed & KEY_B)
      break;
  }
  Sound_Play(SE_MENU_CANCEL);
  Window_Close(window, 2);
}

void WindowSprite_PrepareForRedraw(void *window);
void Text_DrawNumberAtOffset(s32 value, s32 digits, void *window, s32 x, s32 y);
void Text_DrawMessageAsTiles(s32 message, void *window, s32 x, s32 y);
s32 PartyInventory_HasSpace(void);
s32 PartyInventory_Add(s32 item);
void ItemMenu_DrawItemDetails(void *window, s32 item);

/// The selector's item numbers, 0 to 499, wrapping.
#define ITEM_SELECTOR_ITEM_COUNT 500

/// The item selector, as Increase item and level's
/// (IncreaseItemAndLevel_SelectItem): Up and Down move one item, Right and Left
/// ten, R and L thirty, skipping items with no icon; A gives the item, B
/// closes. Talking to ACTOR_ITEM_SELECTOR.
void ShopTest_SelectItem(void) {
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
  window = Window_Create(0, 0, 30, 7, WINDOW_FLAG_INSTANT);
  details = Window_Create(0, 8, 13, 10, WINDOW_FLAG_INSTANT);
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
          (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR),
          DMA_COPY_HALFWORDS(1), REG_DMA3);
  Sound_Play(SE_MENU_CONFIRM);
  wait(1);
  while (1) {
    if (redraw) {
      redraw = 0;
      WindowSprite_PrepareForRedraw(window);
      WindowSprite_PrepareForRedraw(details);
      Text_DrawStringInWindow(gShopTest_ItemPrompt, window, 0, 0);
      Text_DrawNumberAtOffset(item, 0, window, 80, 0);
      if (PartyInventory_HasSpace()) {
        Text_DrawStringInWindow(gShopTest_ItemHelp, window, 0, 32);
        Text_DrawMessageAsTiles((s32)(item + MsgItemName), window, 120, 0);
        Text_DrawMessage((s32)(item + MsgItemPlainName), window, 0, 16);
        ItemMenu_DrawItemDetails(details, item);
      } else {
        Text_DrawStringInWindow(gShopTest_ItemFull, window, 0, 32);
      }
    }
    wait(1);
    if (gInput.pressed & KEY_A) {
      if (PartyInventory_Add(item) == -1) {
        Sound_Play(SE_MENU_CANCEL);
        break;
      }
      Sound_Play(SE_NOTE_HIGH);
    }
    if (gInput.pressed & KEY_B) {
      Sound_Play(SE_MENU_CANCEL);
      break;
    }
    if (gInput.repeat & KEY_DOWN) {
      step = 1;
      item++;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_UP) {
      step = -1;
      item--;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_RIGHT) {
      step = 1;
      item += BROWSER_ROW_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_LEFT) {
      step = -1;
      item -= BROWSER_ROW_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_R) {
      step = 1;
      item += BROWSER_PAGE_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_L) {
      step = -1;
      item -= BROWSER_PAGE_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (step == -1) {
      for (item = (item + ITEM_SELECTOR_ITEM_COUNT) % entryCount;
           Item_GetDirect(item & INVENTORY_ITEM_ID_MASK)->icon == 0;
           item = (item + ITEM_SELECTOR_ITEM_COUNT - 1) % entryCount)
        ;
    }
    if (step == 1) {
      for (item = (item + ITEM_SELECTOR_ITEM_COUNT) % entryCount;
           Item_GetDirect(item & INVENTORY_ITEM_ID_MASK)->icon == 0;
           item = (item + ITEM_SELECTOR_ITEM_COUNT + 1) % entryCount)
        ;
    }
    step = 0;
  }
  Window_Close(window, 2);
  Window_Close(details, 2);
}

/// Opens the icon browser.
void ShopTest_OpenIconBrowser(void) { DebugBrowser_Icons(); }

/// Opens the portrait browser.
void ShopTest_OpenPortraitBrowser(void) { DebugBrowser_Portraits(); }

/// Unused: the Psynergy test's PsynergyTest_DigCells again.
UNUSED void ShopTest_DigCells(s32 mode, s32 sourceX, s32 sourceY, s32 width,
                              s32 height, s32 destinationX, s32 destinationY) {
  if (mode == 1)
    Map_CopyCellsRect(sourceX, sourceY, width, height, destinationX,
                      destinationY);
}

/// Unused: the Psynergy test's PsynergyTest_GrowActor again.
UNUSED void ShopTest_GrowActor(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *actor;
  s32 scale;

  actor = Actor_GetById(id);
  if (mode == 1) {
    Actor_SetPosition(id, x, z);
    actor->scaleX = 0;
    actor->scaleY = 0;
  }
  if (mode == 2) {
    while (actor->scaleX < FIXED_ONE) {
      wait(1);
      scale = actor->scaleX;
      scale += toFIXED(0.02);
      actor->scaleX = scale;
      actor->scaleY = scale;
    }
    actor->scaleX = FIXED_ONE;
    actor->scaleY = FIXED_ONE;
  }
}

/// Unused: the Psynergy test's PsynergyTest_DropActor again.
UNUSED void ShopTest_DropActor(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *actor;

  actor = Actor_GetById(id);
  if (mode == 2) {
    Actor_SetPosition(id, x, z);
    actor->y.fixed = toFIXED(32);
    actor->motionFlags = 3;
    actor->gravity = toFIXED(0.09);
    wait(50);
  }
}

void Field_RequestBattleFromEncounterTable(s32 zone, s32 slot);

/// Unused: the Psynergy test's PsynergyTest_RaiseStatue again. Mode 1 places a
/// brown boulder at (x, z) in pixels; mode 2 grows a winged statue there from
/// two fifths to full size while it turns, then turns it opposite the leader's
/// facing and starts the battle in slot `slot` of encounter zone `zone`.
UNUSED void ShopTest_RaiseStatue(s32 mode, s32 x, s32 z, s32 zone, s32 slot) {
  struct Actor *actor;

  if (mode == 1) {
    actor = FieldActor_Create(SPRITE_BROWN_BOULDER, toFIXED(x), 0, toFIXED(z));
    if (actor != NULL) {
      Actor_SetSpriteFlags(actor, 0);
      actor->priorityFlags = 2;
    }
  }
  if (mode == 2) {
    actor = ((struct Actor *)Import_ActorCreate(SPRITE_WINGED_STATUE,
                                                toFIXED(x), 0, toFIXED(z)));
    if (actor != NULL) {
      s32 scale;

      Actor_SetSpriteFlags(actor, 0);
      actor->velocityY = toFIXED(6);
      actor->scaleX = FIXED_ONE * 2 / 5;
      actor->scaleY = FIXED_ONE * 2 / 5;
      while (actor->scaleX < FIXED_ONE) {
        wait(1);
        scale = actor->scaleX + toFIXED(0.02);
        actor->facing += FACING_STEP;
        actor->scaleX = scale;
        actor->scaleY = scale;
      }
      actor->scaleX = FIXED_ONE;
      actor->scaleY = FIXED_ONE;
      actor->facing =
          Actor_GetById(gMainRoutineWork.leaderActorId)->facing + DEGtoANG(180);
      Field_RequestBattleFromEncounterTable(zone, slot);
    }
  }
}
#undef ITEM_SELECTOR_ITEM_COUNT
