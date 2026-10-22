#include "game_state.h"
#include "unit/import.h"
#include "unit/debug/test.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/menu/menu.h"
#include "unit/shop/shop.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "menu_test.h"
#include "engine_call.h"
#include "types.h"
#include "io_reg.h"
#include "inventory.h"
#include "dma.h"
#include "text_render_runtime.h"
#include "item.h"
#include "item_ids.h"
#include "item_gift.h"
#include "tbs_edition.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "songs.h"

void Engine_EventSetMessage(s16 message);

void Engine_AudioPlayCue(s32 cue);

/// The debug item browser covers IDs 0 through 269.
#define DEBUG_ITEM_COUNT 270
#define DEBUG_ITEM_ROW_STEP 10  ///< Right and Left move ten numbers.
#define DEBUG_ITEM_PAGE_STEP 30 ///< R and L move thirty.

/// The placed actors, numbered in the order of gMenuTest_Placements after its
/// four party rows, which keep their characters' ids. Each is drawn with the
/// sprite of someone or something in the game and named for it; what talking
/// to each does is in gMenuTest_Events.
enum MenuTestActor {
  /// Kraden's sprite, 0x1e (ACTOR_KRADEN in sol_sanctum_elemental_star_room.c);
  /// opens the picker a full party drops an item with.
  ACTOR_KRADEN = ACTOR_FIRST_PLACED,
  /// Dora's sprite, 0x1f (vale_eruption.c). `You got a Djinni!`
  ACTOR_DORA,
  /// Saturos's sprite, 0x20 (sol_sanctum_elemental_star_room.c).
  /// `The body is being torn apart.`
  ACTOR_SATUROS,
  /// Menardi's sprite, 0x21 (sol_sanctum_elemental_star_room.c).
  /// `You got treasure!`
  ACTOR_MENARDI,
  /// The sprite of Vale's armor shopkeeper, 0x7d (vale_interiors_shops.c);
  /// opens
  /// the debug shop.
  ACTOR_ARMOR_SHOPKEEPER,
  /// Dora's sprite again: the sanctum's service menu, in French a Nut, and in
  /// Spanish and Italian `That IS dangerous!`
  ACTOR_SECOND_DORA,
  /// Saturos's sprite again. "Kyle... Will they be able to stop the Boulder?"
  ACTOR_SECOND_SATUROS,
  /// The ivy's sprite, 0xf8 ("trapped in the ivy", goma_cave_entrance.c);
  /// opens the icon browser, and takes Whirlwind, Force and Catch.
  ACTOR_IVY,
  /// Felix's sprite, 0x23 (sol_sanctum_elemental_star_room.c); a shop message
  /// browser, or in Japanese the character selector.
  ACTOR_FELIX,
  /// Felix's sprite again: a shop message browser, or in Japanese the item
  /// browser.
  ACTOR_SECOND_FELIX
};

/// Recalculates the four heroes after a debug inventory or stat change.
/// Preserve the original Isaac, Garet, Mia, Ivan order.
static __inline__ void DebugMenu_RecalculateParty(void) {
  UnitCalc(ISAAC);
  UnitCalc(GARET);
  UnitCalc(MIA);
  UnitCalc(IVAN);
}

u8 *MenuTest_GetEntrances(void) { return (u8 *)gMenuTest_Entrances; }

/// The room has no exit regions: its regions entry returns none.
s32 SceneData_ReturnZero(void) { return 0; }

u8 *MenuTest_GetExits(void) { return (u8 *)gMenuTest_Exits; }

u8 *MenuTest_GetPlacements(void) { return (u8 *)gMenuTest_Placements; }

#if TBS_PATCH_1
/// Finishes pending UI work, displays a message, and waits one final frame.
static void SceneDialogue_ShowMessageAndWait(s32 messageId) {
  UiWork_FinalizePendingCore();
  UiText_OpenMessageWindow(messageId, 5, 0, 0x22);
  while (UiWork_IsComplete() == 0) {
    Engine_TaskWait(1);
  }
  Engine_TaskWait(1);
}

/// Browses sample dialogue with A/Down and Up; B closes the window. Shows the
/// messages from messageId on, one at a time: A or Down goes on, Up goes
/// back, and any other held key shows the same message again. Ends after
/// messageCount have been shown, whichever way the player went. The message
/// speed is left at 2.
static void DebugMenu_BrowseMessageRange(s32 messageId, s32 messageCount) {
  struct UiWindow *window;
  s32 messageIndex;

  gGameState.messageSpeed = 2;
  window = UiWindow_CreateWithSideObject(125, 0, 0, 0);
  for (messageIndex = 0; messageIndex < messageCount; messageIndex++) {
    // Supply sample name, item and number substitutions before each message:
    // Garet, the Virtuous Armlet and 123456.
    UiWork_PushValueSlot(GARET, 1);
    UiWork_PushValueSlot(ITEM_VIRTUOUS_ARMLET, 2);
    UiWork_PushValueSlot(0x1e240, 5);
    SceneDialogue_ShowMessageAndWait(messageId);
    for (;;) {
      if ((gKeysHeld & KEY_B) != 0) {
        goto end;
      }
      if ((gKeysHeld & KEY_A) != 0 || (gKeysHeld & KEY_DOWN) != 0) {
        messageId++;
        break;
      }
      if ((gKeysHeld & KEY_UP) != 0) {
        messageId--;
        break;
      }
      if (gKeysHeld == 0) {
        Engine_TaskWait(1);
      } else {
        break;
      }
    }
  }
end:
  UiWork_FinalizePendingCore();
  Engine_DebugFinalizeWindow(window, 2);
}

// The shop message browsers, run by talking to ACTOR_FELIX and
// ACTOR_SECOND_FELIX while the leader faces a given way (gMenuTest_Events).
// Each runs for the weapon shop's 43 messages; the Sanctum's count is its own,
// also 43.
static void DebugMenu_BrowseWeaponShopMessages(void) {
  DebugMenu_BrowseMessageRange(MsgWeaponShopWelcome,
                               MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

static void DebugMenu_BrowseArmorShopMessages(void) {
  DebugMenu_BrowseMessageRange(MsgArmorShopWelcome,
                               MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

static void DebugMenu_BrowseItemShopMessages(void) {
  DebugMenu_BrowseMessageRange(MsgItemShopWelcome,
                               MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

static void DebugMenu_BrowseSanctumMessages(void) {
  DebugMenu_BrowseMessageRange(MsgSanctumWelcome,
                               MsgWarriorShopWelcome - MsgSanctumWelcome);
}

static void DebugMenu_BrowseWarriorShopMessages(void) {
  DebugMenu_BrowseMessageRange(MsgWarriorShopWelcome,
                               MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

static void DebugMenu_BrowseWarriorArmorShopMessages(void) {
  DebugMenu_BrowseMessageRange(MsgWarriorArmorShopWelcome,
                               MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

static void DebugMenu_BrowseWarriorItemShopMessages(void) {
  DebugMenu_BrowseMessageRange(MsgWarriorItemShopWelcome,
                               MsgArmorShopWelcome - MsgWeaponShopWelcome);
}
#endif

#if TBS_PATCH_2
/// Sets the event's message twice, so the second, MsgValeComeOnHurry, is the
/// one shown, with no speaker (actor -1), and waits until it closes. Talking
/// to ACTOR_SECOND_FELIX while facing south, in German, Spanish, French and
/// Italian.
static void DebugMenu_RunMessageTest(void) {
  Engine_EventSetMessage(MSGNO(MsgBattleAdditionalPsynergyHelp));
  Engine_EventSetMessage(MSGNO(MsgValeComeOnHurry));
  FieldEvent_ShowMessageAndWait(-1, 0);
}
#endif

/// Runs the shop test's preset inventory, money and ailment setup. Talking
/// to ACTOR_ARMOR_SHOPKEEPER: the debug shop, with 200000 coins, broken
/// equipment and afflicted members (Shop_OpenDebugShop).
static void DebugMenu_RunPresetShop(void) { Shop_OpenDebugShop(1); }

/// Unused: an empty function nothing calls.
UNUSED static void DebugMenu_NoOp(void) {}

/// Opens the shop inventory selector and discards its selected owner and slot.
/// Talking to ACTOR_KRADEN: the picker a full party drops an item with.
static void DebugMenu_TestItemSelection(void) {
  s32 selectedOwner;
  s32 selectedSlot;
  Shop_PickUnitItem(&selectedOwner, &selectedSlot);
}

/// Talking to Mia: the name entry screen for Isaac.
static void DebugMenu_EditIsaacName(void) { NameEntry_EditOwnerName(ISAAC); }

/// Leaves the test actor unchanged when targeted by Psynergy. The event for
/// Whirlwind, Force and Catch cast at ACTOR_IVY.
static void DebugMenu_IgnorePsynergy(void) {}

/// Keeps Force from moving ACTOR_IVY: run at phase 1 of Force cast at it, this
/// sets the effect's childMode, and Force's shot then leaves its target where
/// it is (object/psynergy_effects/force.c).
static void DebugMenu_EnableEffectChildMode(void) {
  ((struct FieldFxScene *)gEffectWork)->childMode = 1;
}

#if TBS_PATCH_3
/// Gives a Nut through the ordinary party gift and full-inventory dialogue.
/// Talking to ACTOR_SECOND_DORA in French only.
UNUSED static void DebugMenu_GiveItemToParty(void) {
  PartyInventory_GiveItem(ITEM_NUT, 0);
}
#endif

s32 MenuTest_GetEvents(void) { return (s32)gMenuTest_Events; }

/// Levels the four heroes to the current debug target, then raises it by ten.
/// Talking to Garet: `You went up a level!`, then Isaac, Garet, Mia and Ivan
/// are levelled up to 10 the first time, 20 the next, and so on.
/// Unit_LevelUpTo only raises a level, and entering the room has already set
/// them to 30 (Isaac to 50 outside Japanese), so the first talks change
/// nothing.
static void DebugMenu_RaisePartyLevels(void) {
  s32 *targetLevel;

  UiText_ShowPositionedMessageAndWait(MsgDebugWentLevel, 1);
  targetLevel = &gMenuTest_SlotOffsets;
  Unit_LevelUpTo(ISAAC, *targetLevel);
  Unit_LevelUpTo(GARET, *targetLevel);
  Unit_LevelUpTo(MIA, *targetLevel);
  Unit_LevelUpTo(IVAN, *targetLevel);
  *targetLevel += 10;
  DebugMenu_RecalculateParty();
}

/// Attempts each consumable grant in order, including calls after a bag fills.
/// Talking to Ivan: `You got an item!`. The numbered items are 180 the Herb,
/// 183 the Potion, 186 the Psy Crystal, 192 the Cookie and 236 the Sacred
/// Feather (MsgItemName + the number).
static void DebugMenu_GiveConsumableTestStock(void) {
  // A repeated-grant loop shrinks this native function from 0x626 to 0x12c;
  // retain the individual calls and their original insertion order.
  UiText_ShowPositionedMessageAndWait(MsgDebugGotItem, 1);
  Inventory_AddItem(ISAAC, ITEM_ANTIDOTE);
  Inventory_AddItem(ISAAC, ITEM_ANTIDOTE);
  Inventory_AddItem(ISAAC, ITEM_ANTIDOTE);
  Inventory_AddItem(ISAAC, ITEM_ANTIDOTE);
  Inventory_AddItem(ISAAC, ITEM_ANTIDOTE);
  Inventory_AddItem(ISAAC, ITEM_ANTIDOTE);
  Inventory_AddItem(ISAAC, ITEM_ANTIDOTE);
  Inventory_AddItem(ISAAC, ITEM_ANTIDOTE);
  Inventory_AddItem(ISAAC, ITEM_ANTIDOTE);
  Inventory_AddItem(ISAAC, ITEM_ANTIDOTE);
  Inventory_AddItem(ISAAC, ITEM_ANTIDOTE);
  Inventory_AddItem(ISAAC, ITEM_ANTIDOTE);
  Inventory_AddItem(ISAAC, ITEM_ANTIDOTE);
  Inventory_AddItem(ISAAC, ITEM_ANTIDOTE);
  Inventory_AddItem(ISAAC, ITEM_ANTIDOTE);
  Inventory_AddItem(ISAAC, ITEM_ANTIDOTE);
  Inventory_AddItem(ISAAC, ITEM_ANTIDOTE);
  Inventory_AddItem(ISAAC, ITEM_HERB);
  Inventory_AddItem(ISAAC, ITEM_HERB);
  Inventory_AddItem(ISAAC, ITEM_HERB);
  Inventory_AddItem(ISAAC, ITEM_HERB);
  Inventory_AddItem(ISAAC, ITEM_HERB);
  Inventory_AddItem(ISAAC, ITEM_HERB);
  Inventory_AddItem(ISAAC, ITEM_HERB);
  Inventory_AddItem(ISAAC, ITEM_HERB);
  Inventory_AddItem(ISAAC, ITEM_HERB);
  Inventory_AddItem(ISAAC, ITEM_HERB);
  Inventory_AddItem(ISAAC, ITEM_HERB);
  Inventory_AddItem(ISAAC, ITEM_HERB);
  Inventory_AddItem(ISAAC, ITEM_HERB);
  Inventory_AddItem(ISAAC, ITEM_HERB);
  Inventory_AddItem(ISAAC, ITEM_NUT);
  Inventory_AddItem(ISAAC, ITEM_NUT);
  Inventory_AddItem(ISAAC, ITEM_NUT);
  Inventory_AddItem(ISAAC, ITEM_NUT);
  Inventory_AddItem(ISAAC, ITEM_NUT);
  Inventory_AddItem(ISAAC, ITEM_NUT);
  Inventory_AddItem(ISAAC, ITEM_NUT);
  Inventory_AddItem(ISAAC, ITEM_NUT);
  Inventory_AddItem(ISAAC, ITEM_NUT);
  Inventory_AddItem(ISAAC, ITEM_NUT);
  Inventory_AddItem(ISAAC, ITEM_NUT);
  Inventory_AddItem(ISAAC, ITEM_VIAL);
  Inventory_AddItem(ISAAC, ITEM_VIAL);
  Inventory_AddItem(ISAAC, ITEM_VIAL);
  Inventory_AddItem(ISAAC, ITEM_VIAL);
  Inventory_AddItem(ISAAC, ITEM_VIAL);
  Inventory_AddItem(ISAAC, ITEM_VIAL);
  Inventory_AddItem(ISAAC, ITEM_VIAL);
  Inventory_AddItem(ISAAC, ITEM_VIAL);
  Inventory_AddItem(ISAAC, ITEM_VIAL);
  Inventory_AddItem(ISAAC, ITEM_VIAL);
  Inventory_AddItem(ISAAC, ITEM_VIAL);
  Inventory_AddItem(ISAAC, ITEM_VIAL);
  Inventory_AddItem(ISAAC, ITEM_VIAL);
  Inventory_AddItem(ISAAC, ITEM_POTION);
  Inventory_AddItem(ISAAC, ITEM_POTION);
  Inventory_AddItem(ISAAC, ITEM_POTION);
  Inventory_AddItem(ISAAC, ITEM_POTION);
  Inventory_AddItem(ISAAC, ITEM_POTION);
  Inventory_AddItem(ISAAC, ITEM_POTION);
  Inventory_AddItem(ISAAC, ITEM_POTION);
  Inventory_AddItem(ISAAC, ITEM_POTION);
  Inventory_AddItem(ISAAC, ITEM_POTION);
  Inventory_AddItem(GARET, ITEM_PSY_CRYSTAL);
  Inventory_AddItem(GARET, ITEM_PSY_CRYSTAL);
  Inventory_AddItem(GARET, ITEM_PSY_CRYSTAL);
  Inventory_AddItem(GARET, ITEM_PSY_CRYSTAL);
  Inventory_AddItem(GARET, ITEM_PSY_CRYSTAL);
  Inventory_AddItem(GARET, ITEM_PSY_CRYSTAL);
  Inventory_AddItem(GARET, ITEM_PSY_CRYSTAL);
  Inventory_AddItem(GARET, ITEM_PSY_CRYSTAL);
  Inventory_AddItem(GARET, ITEM_PSY_CRYSTAL);
  Inventory_AddItem(GARET, ITEM_PSY_CRYSTAL);
  Inventory_AddItem(GARET, ITEM_PSY_CRYSTAL);
  Inventory_AddItem(GARET, ITEM_PSY_CRYSTAL);
  Inventory_AddItem(GARET, ITEM_PSY_CRYSTAL);
  Inventory_AddItem(GARET, ITEM_ANTIDOTE);
  Inventory_AddItem(GARET, ITEM_ANTIDOTE);
  Inventory_AddItem(GARET, ITEM_ANTIDOTE);
  Inventory_AddItem(GARET, ITEM_ANTIDOTE);
  Inventory_AddItem(GARET, ITEM_ANTIDOTE);
  Inventory_AddItem(GARET, ITEM_ANTIDOTE);
  Inventory_AddItem(GARET, ITEM_ANTIDOTE);
  Inventory_AddItem(GARET, ITEM_ANTIDOTE);
  Inventory_AddItem(GARET, ITEM_ANTIDOTE);
  Inventory_AddItem(GARET, ITEM_ANTIDOTE);
  Inventory_AddItem(GARET, ITEM_ANTIDOTE);
  Inventory_AddItem(GARET, ITEM_ANTIDOTE);
  Inventory_AddItem(IVAN, ITEM_ELIXIR);
  Inventory_AddItem(IVAN, ITEM_ELIXIR);
  Inventory_AddItem(IVAN, ITEM_ELIXIR);
  Inventory_AddItem(IVAN, ITEM_ELIXIR);
  Inventory_AddItem(IVAN, ITEM_ELIXIR);
  Inventory_AddItem(IVAN, ITEM_ELIXIR);
  Inventory_AddItem(IVAN, ITEM_ELIXIR);
  Inventory_AddItem(IVAN, ITEM_ELIXIR);
  Inventory_AddItem(IVAN, ITEM_ELIXIR);
  Inventory_AddItem(IVAN, ITEM_ELIXIR);
  Inventory_AddItem(IVAN, ITEM_ELIXIR);
  Inventory_AddItem(IVAN, ITEM_ELIXIR);
  Inventory_AddItem(IVAN, ITEM_ELIXIR);
  Inventory_AddItem(IVAN, ITEM_ELIXIR);
  Inventory_AddItem(IVAN, ITEM_WATER_OF_LIFE);
  Inventory_AddItem(IVAN, ITEM_WATER_OF_LIFE);
  Inventory_AddItem(IVAN, ITEM_WATER_OF_LIFE);
  Inventory_AddItem(IVAN, ITEM_WATER_OF_LIFE);
  Inventory_AddItem(IVAN, ITEM_WATER_OF_LIFE);
  Inventory_AddItem(IVAN, ITEM_WATER_OF_LIFE);
  Inventory_AddItem(IVAN, ITEM_WATER_OF_LIFE);
  Inventory_AddItem(IVAN, ITEM_WATER_OF_LIFE);
  Inventory_AddItem(IVAN, ITEM_WATER_OF_LIFE);
  Inventory_AddItem(IVAN, ITEM_WATER_OF_LIFE);
  Inventory_AddItem(IVAN, ITEM_WATER_OF_LIFE);
  Inventory_AddItem(IVAN, ITEM_SACRED_FEATHER);
  Inventory_AddItem(IVAN, ITEM_SACRED_FEATHER);
  Inventory_AddItem(IVAN, ITEM_SACRED_FEATHER);
  Inventory_AddItem(IVAN, ITEM_SACRED_FEATHER);
  Inventory_AddItem(IVAN, ITEM_SACRED_FEATHER);
  Inventory_AddItem(IVAN, ITEM_SACRED_FEATHER);
  Inventory_AddItem(IVAN, ITEM_SACRED_FEATHER);
  Inventory_AddItem(IVAN, ITEM_SACRED_FEATHER);
  Inventory_AddItem(IVAN, ITEM_SACRED_FEATHER);
  Inventory_AddItem(IVAN, ITEM_SACRED_FEATHER);
  Inventory_AddItem(IVAN, ITEM_SACRED_FEATHER);
  Inventory_AddItem(IVAN, ITEM_SACRED_FEATHER);
  Inventory_AddItem(IVAN, ITEM_SACRED_FEATHER);
  Inventory_AddItem(IVAN, ITEM_SACRED_FEATHER);
  Inventory_AddItem(IVAN, ITEM_SACRED_FEATHER);
  Inventory_AddItem(IVAN, ITEM_SACRED_FEATHER);
  Inventory_AddItem(MIA, ITEM_POWER_BREAD);
  Inventory_AddItem(MIA, ITEM_POWER_BREAD);
  Inventory_AddItem(MIA, ITEM_POWER_BREAD);
  Inventory_AddItem(MIA, ITEM_POWER_BREAD);
  Inventory_AddItem(MIA, ITEM_POWER_BREAD);
  Inventory_AddItem(MIA, ITEM_POWER_BREAD);
  Inventory_AddItem(MIA, ITEM_POWER_BREAD);
  Inventory_AddItem(MIA, ITEM_POWER_BREAD);
  Inventory_AddItem(MIA, ITEM_POWER_BREAD);
  Inventory_AddItem(MIA, ITEM_POWER_BREAD);
  Inventory_AddItem(MIA, ITEM_POWER_BREAD);
  Inventory_AddItem(MIA, ITEM_COOKIE);
  Inventory_AddItem(MIA, ITEM_COOKIE);
  Inventory_AddItem(MIA, ITEM_COOKIE);
  Inventory_AddItem(MIA, ITEM_COOKIE);
  Inventory_AddItem(MIA, ITEM_COOKIE);
  Inventory_AddItem(MIA, ITEM_COOKIE);
  Inventory_AddItem(MIA, ITEM_COOKIE);
  Inventory_AddItem(MIA, ITEM_COOKIE);
  Inventory_AddItem(MIA, ITEM_COOKIE);
  Inventory_AddItem(MIA, ITEM_COOKIE);
  Inventory_AddItem(MIA, ITEM_COOKIE);
  Inventory_AddItem(MIA, ITEM_APPLE);
  Inventory_AddItem(MIA, ITEM_APPLE);
  Inventory_AddItem(MIA, ITEM_APPLE);
  Inventory_AddItem(MIA, ITEM_APPLE);
  Inventory_AddItem(MIA, ITEM_APPLE);
  Inventory_AddItem(MIA, ITEM_APPLE);
  Inventory_AddItem(MIA, ITEM_APPLE);
  Inventory_AddItem(MIA, ITEM_APPLE);
  Inventory_AddItem(MIA, ITEM_APPLE);
  Inventory_AddItem(MIA, ITEM_APPLE);
  Inventory_AddItem(MIA, ITEM_APPLE);
  Inventory_AddItem(MIA, ITEM_HARD_NUT);
  Inventory_AddItem(MIA, ITEM_HARD_NUT);
  Inventory_AddItem(MIA, ITEM_HARD_NUT);
  Inventory_AddItem(MIA, ITEM_HARD_NUT);
  Inventory_AddItem(MIA, ITEM_HARD_NUT);
  Inventory_AddItem(MIA, ITEM_HARD_NUT);
  Inventory_AddItem(MIA, ITEM_HARD_NUT);
  Inventory_AddItem(MIA, ITEM_MINT);
  Inventory_AddItem(MIA, ITEM_MINT);
  Inventory_AddItem(MIA, ITEM_MINT);
  Inventory_AddItem(MIA, ITEM_MINT);
  Inventory_AddItem(MIA, ITEM_MINT);
  Inventory_AddItem(MIA, ITEM_MINT);
  Inventory_AddItem(MIA, ITEM_MINT);
  Inventory_AddItem(MIA, ITEM_MINT);
  Inventory_AddItem(MIA, ITEM_MINT);
  Inventory_AddItem(MIA, ITEM_MINT);
  Inventory_AddItem(MIA, ITEM_MINT);
  Inventory_AddItem(MIA, ITEM_MINT);
  Inventory_AddItem(MIA, ITEM_MINT);
  Inventory_AddItem(MIA, ITEM_LUCKY_PEPPER);
  Inventory_AddItem(MIA, ITEM_LUCKY_PEPPER);
  Inventory_AddItem(MIA, ITEM_LUCKY_PEPPER);
  Inventory_AddItem(MIA, ITEM_LUCKY_PEPPER);
  Inventory_AddItem(MIA, ITEM_LUCKY_PEPPER);
  Inventory_AddItem(MIA, ITEM_LUCKY_PEPPER);
  Inventory_AddItem(MIA, ITEM_LUCKY_PEPPER);
  Inventory_AddItem(MIA, ITEM_LUCKY_PEPPER);
  DebugMenu_RecalculateParty();
}

/// Grants and sets test Djinn. Talking to ACTOR_DORA: `You got a Djinni!`,
/// then Isaac takes the first seven Venus Djinn, Garet seven Mars, Mia seven
/// Mercury and Ivan six Jupiter, each set. In Japanese it first fills a local
/// buffer sixteen times with 32 to 47 copies of a letter; nothing reads the
/// buffer, so the loop's only lasting effect is gMenuTest_FillLetter, which
/// nothing else reads either. The call the string presumably fed is gone from
/// every edition.
static void DebugMenu_GivePartyDjinn(void) {
  /* FAKEMATCH(dead): the Japanese loop's buffer, unread outside
   * Japanese, for the frame's 256 bytes; as declared inside the #if, 2 of 232
   * instructions differ in tbs-en. */
  UNUSED u8 fillBuffer[256];
#if !TBS_PATCH_1
  s32 pass;
  s32 characterIndex;

  for (pass = 0; pass < 16; pass++) {
    for (characterIndex = 0; characterIndex < 256; characterIndex++) {
      fillBuffer[characterIndex] = 0;
    }
    for (characterIndex = 0; characterIndex < pass + 32; characterIndex++) {
      fillBuffer[characterIndex] = gMenuTest_FillLetter + 1;
    }
    gMenuTest_FillLetter++;
    if (gMenuTest_FillLetter > 96) {
      gMenuTest_FillLetter = 65;
    }
  }
#endif
  // Add and set each element in separate passes, preserving recovery updates.
  UiText_ShowPositionedMessageAndWait(MsgDebugGotDjinni, 1);
  Djinn_AddToOwner(ISAAC, ELEMENT_VENUS, 0);
  Djinn_AddToOwner(ISAAC, ELEMENT_VENUS, 1);
  Djinn_AddToOwner(ISAAC, ELEMENT_VENUS, 2);
  Djinn_AddToOwner(ISAAC, ELEMENT_VENUS, 3);
  Djinn_AddToOwner(ISAAC, ELEMENT_VENUS, 4);
  Djinn_AddToOwner(ISAAC, ELEMENT_VENUS, 5);
  Djinn_AddToOwner(ISAAC, ELEMENT_VENUS, 6);
  Djinn_Activate(ISAAC, ELEMENT_VENUS, 0);
  Djinn_Activate(ISAAC, ELEMENT_VENUS, 1);
  Djinn_Activate(ISAAC, ELEMENT_VENUS, 2);
  Djinn_Activate(ISAAC, ELEMENT_VENUS, 3);
  Djinn_Activate(ISAAC, ELEMENT_VENUS, 4);
  Djinn_Activate(ISAAC, ELEMENT_VENUS, 5);
  Djinn_Activate(ISAAC, ELEMENT_VENUS, 6);
  Djinn_AddToOwner(GARET, ELEMENT_MARS, 0);
  Djinn_AddToOwner(GARET, ELEMENT_MARS, 1);
  Djinn_AddToOwner(GARET, ELEMENT_MARS, 2);
  Djinn_AddToOwner(GARET, ELEMENT_MARS, 3);
  Djinn_AddToOwner(GARET, ELEMENT_MARS, 4);
  Djinn_AddToOwner(GARET, ELEMENT_MARS, 5);
  Djinn_AddToOwner(GARET, ELEMENT_MARS, 6);
  Djinn_Activate(GARET, ELEMENT_MARS, 0);
  Djinn_Activate(GARET, ELEMENT_MARS, 1);
  Djinn_Activate(GARET, ELEMENT_MARS, 2);
  Djinn_Activate(GARET, ELEMENT_MARS, 3);
  Djinn_Activate(GARET, ELEMENT_MARS, 4);
  Djinn_Activate(GARET, ELEMENT_MARS, 5);
  Djinn_Activate(GARET, ELEMENT_MARS, 6);
  Djinn_AddToOwner(MIA, ELEMENT_MERCURY, 0);
  Djinn_AddToOwner(MIA, ELEMENT_MERCURY, 1);
  Djinn_AddToOwner(MIA, ELEMENT_MERCURY, 2);
  Djinn_AddToOwner(MIA, ELEMENT_MERCURY, 3);
  Djinn_AddToOwner(MIA, ELEMENT_MERCURY, 4);
  Djinn_AddToOwner(MIA, ELEMENT_MERCURY, 5);
  Djinn_AddToOwner(MIA, ELEMENT_MERCURY, 6);
  Djinn_Activate(MIA, ELEMENT_MERCURY, 0);
  Djinn_Activate(MIA, ELEMENT_MERCURY, 1);
  Djinn_Activate(MIA, ELEMENT_MERCURY, 2);
  Djinn_Activate(MIA, ELEMENT_MERCURY, 3);
  Djinn_Activate(MIA, ELEMENT_MERCURY, 4);
  Djinn_Activate(MIA, ELEMENT_MERCURY, 5);
  Djinn_Activate(MIA, ELEMENT_MERCURY, 6);
  Djinn_AddToOwner(IVAN, ELEMENT_JUPITER, 0);
  Djinn_AddToOwner(IVAN, ELEMENT_JUPITER, 1);
  Djinn_AddToOwner(IVAN, ELEMENT_JUPITER, 2);
  Djinn_AddToOwner(IVAN, ELEMENT_JUPITER, 3);
  Djinn_AddToOwner(IVAN, ELEMENT_JUPITER, 4);
  Djinn_AddToOwner(IVAN, ELEMENT_JUPITER, 5);
  Djinn_Activate(IVAN, ELEMENT_JUPITER, 0);
  Djinn_Activate(IVAN, ELEMENT_JUPITER, 1);
  Djinn_Activate(IVAN, ELEMENT_JUPITER, 2);
  Djinn_Activate(IVAN, ELEMENT_JUPITER, 3);
  Djinn_Activate(IVAN, ELEMENT_JUPITER, 4);
  Djinn_Activate(IVAN, ELEMENT_JUPITER, 5);
  DebugMenu_RecalculateParty();
}

/// Reduces HP and PP, then poisons and haunts Isaac and binds and venoms Garet.
/// Talking to ACTOR_SATUROS: `The body is being torn apart.` Isaac is poisoned
/// and haunted, Garet given deadly poison. Garet's restraint
/// is set to 1 too, but the closing UnitCalc clears the curse bits again
/// unless he wears cursed equipment (game/game/units.c).
static void DebugMenu_DamageAndAfflictParty(void) {
  // Named BattleUnit stores change address setup and store ordering in the
  // native build; retain the byte cursor for poison (0x131), haunt (0x140) and
  // restraint (0x130).
  u8 *record;

  UiText_ShowPositionedMessageAndWait(MsgDebugBodyTornApart, 1);
  FieldUnit_AddHp(ISAAC, -100);
  FieldUnit_AddHp(GARET, -100);
  FieldUnit_AddHp(IVAN, -33);
  FieldUnit_AddHp(MIA, -100);
  Unit_AddPp(ISAAC, -50);
  Unit_AddPp(GARET, -40);
  Unit_AddPp(IVAN, -35);
  Unit_AddPp(MIA, -20);
  record = (u8 *)UnitGet(ISAAC);
  record[0x131] = POISON_NORMAL;
  record += 0x140;
  *record = 1;
  record = (u8 *)UnitGet(GARET);
  record[0x130] = 1;
  record[0x131] = POISON_DEADLY;
  DebugMenu_RecalculateParty();
}

/// Adds the test equipment in each owner's original inventory order. Talking
/// to Jenna: `You got sturdy equipment!`, then weapons and armor for each of
/// the four, from the Gaia Blade to the Glittering Tiara.
static void DebugMenu_GiveEquipmentTestStock(void) {
  UiText_ShowPositionedMessageAndWait(MsgDebugGotSturdyEquipment, 1);
  Inventory_AddItem(ISAAC, ITEM_SPIKED_ARMOR);
  Inventory_AddItem(ISAAC, ITEM_ASURAS_ARMOR);
  Inventory_AddItem(ISAAC, ITEM_EARTH_SHIELD);
  Inventory_AddItem(ISAAC, ITEM_DRAGON_SHIELD);
  Inventory_AddItem(ISAAC, ITEM_GAIA_BLADE);
  Inventory_AddItem(ISAAC, ITEM_MURAMASA);
  Inventory_AddItem(ISAAC, ITEM_BANDITS_SWORD);
  Inventory_AddItem(ISAAC, ITEM_KUSANAGI);
  Inventory_AddItem(GARET, ITEM_DEMON_AXE);
  Inventory_AddItem(GARET, ITEM_BURNING_AXE);
  Inventory_AddItem(GARET, ITEM_WICKED_MACE);
  Inventory_AddItem(GARET, ITEM_BLESSED_MACE);
  Inventory_AddItem(GARET, ITEM_DEMON_MAIL);
  Inventory_AddItem(GARET, ITEM_DRAGON_SCALES);
  Inventory_AddItem(GARET, ITEM_AURA_GLOVES);
  Inventory_AddItem(GARET, ITEM_BATTLE_GLOVES);
  Inventory_AddItem(GARET, ITEM_ADEPTS_HELM);
  Inventory_AddItem(IVAN, ITEM_ZODIAC_WAND);
  Inventory_AddItem(IVAN, ITEM_SHAMANS_ROD);
  Inventory_AddItem(IVAN, ITEM_NINJA_GARB);
  Inventory_AddItem(IVAN, ITEM_KIMONO);
  Inventory_AddItem(IVAN, ITEM_EARTH_SHIELD);
  Inventory_AddItem(IVAN, ITEM_WAR_GLOVES);
  Inventory_AddItem(IVAN, ITEM_VIRTUOUS_ARMLET);
  Inventory_AddItem(IVAN, ITEM_PROPHETS_HAT);
  Inventory_AddItem(MIA, ITEM_ANGELIC_ANKH);
  Inventory_AddItem(MIA, ITEM_CRYSTAL_ROD);
  Inventory_AddItem(MIA, ITEM_STORM_GEAR);
  Inventory_AddItem(MIA, ITEM_WATER_JACKET);
  Inventory_AddItem(MIA, ITEM_ORACLES_ROBE);
  Inventory_AddItem(MIA, ITEM_FEATHERED_ROBE);
  Inventory_AddItem(MIA, ITEM_VAMBRACE);
  Inventory_AddItem(MIA, ITEM_GUARDIAN_ARMLET);
  Inventory_AddItem(MIA, ITEM_GLITTERING_TIARA);
  DebugMenu_RecalculateParty();
}

/// Browses item definitions and gives the selected item while party bags allow
/// it. Talking to ACTOR_SECOND_FELIX in Japanese (DebugMenu_OpenItemBrowser).
/// Unlike the level room's browser it stops on every number, iconless ones too.
static void DebugMenu_SelectItem(void) {
  struct UiWindow *window;
  struct UiWindow *details;
  s32 selectedItemId;
  s32 redraw;
  s32 definitionId;
  s32 itemCount;

  itemCount = DEBUG_ITEM_COUNT;
  Engine_AudioPlayCue(SE_MENU_CONFIRM);
  window = Engine_DebugCreateWindow(0, 0, 30, 7, 2);
  details = Engine_DebugCreateWindow(0, 8, 13, 10, 2);
  selectedItemId = 1;
  redraw = 1;
  // Reuse the item palette for both debug windows: sprite palette 0 into BG
  // palette 14, and BG palette 15's colour 4 over BG palette 14's colour 14,
  // as the level room's browser does.
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(14, 0),
          DMA_ENABLE << 16 | 0x10, REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(15, 4), (void *)&BG_PLTT_COLOR(14, 14),
          DMA_ENABLE << 16 | 1, REG_DMA3);
  Engine_TaskWait(1);
  while (1) {
    if (redraw) {
      redraw = 0;
      selectedItemId = (selectedItemId + DEBUG_ITEM_COUNT) % itemCount;
      Engine_DebugRedrawWindow(window);
      Engine_DebugClearWindow(window);
      UiText_DrawStringInWindow(gDebugItemPrompt, window, 0, 0);
      UiText_DrawNumberAtOffset(selectedItemId, 0, window, 80, 0);
      if (PartyInventory_HasSpace()) {
        UiText_DrawStringInWindow(gDebugItemCapacityLabel, window, 0, 32);
        definitionId = selectedItemId & 0x1ff;
        Engine_DebugGetItem(definitionId);
        UiText_DrawCharacterAtOffset(definitionId + MsgItemName, window, 120,
                                     0);
        UiText_DrawResource(definitionId + MsgItemPlainName, window, 0, 16);
        Engine_DebugRedrawWindow(details);
        Engine_DebugDrawItemDetails(details, selectedItemId);
      } else {
        UiText_DrawStringInWindow(gDebugItemFullLabel, window, 0, 32);
      }
    }
    // These are independent tests: a single frame can add and move the cursor.
    if (gKeysPressed & KEY_A) {
      if (PartyInventory_Add(selectedItemId) == -1) {
        Engine_AudioPlayCue(SE_MENU_CANCEL);
        break;
      }
      Engine_AudioPlayCue(SE_175);
    }
    if (gKeysPressed & KEY_B) {
      Engine_AudioPlayCue(SE_MENU_CANCEL);
      break;
    }
    if (gKeysRepeat & KEY_UP) {
      selectedItemId--;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_DOWN) {
      selectedItemId++;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_RIGHT) {
      selectedItemId += DEBUG_ITEM_ROW_STEP;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_LEFT) {
      selectedItemId -= DEBUG_ITEM_ROW_STEP;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_R) {
      selectedItemId += DEBUG_ITEM_PAGE_STEP;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_L) {
      selectedItemId -= DEBUG_ITEM_PAGE_STEP;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    Engine_TaskWait(1);
  }
  Engine_DebugRedrawWindow(window);
  Engine_TaskWait(1);
  Engine_DebugFinalizeWindow(window, 1);
  Engine_DebugFinalizeWindow(details, 1);
}

UNUSED static void DebugMenu_OpenItemBrowser(void) { DebugMenu_SelectItem(); }

/// Adds the original treasure test stock, retaining duplicate tickets and
/// medals. Talking to ACTOR_MENARDI: `You got treasure!`. Among the items
/// are the Orb of Force, Douse Drop, Frost Jewel, Lifting Gem, Halt Gem and
/// Catch Beads, the four Elemental Stars and the Mythril Bag.
static void DebugMenu_GiveTreasureTestStock(void) {
  // Sharing the duplicate grants through a counted loop shrinks this function
  // from 0x194 to 0x120; retain the original calls.
  UiText_ShowPositionedMessageAndWait(MsgDebugGotTreasure, 1);
  Inventory_AddItem(ISAAC, ITEM_HERMES_WATER);
  Inventory_AddItem(ISAAC, ITEM_HALT_GEM);
  Inventory_AddItem(ISAAC, ITEM_VENUS_STAR);
  Inventory_AddItem(ISAAC, ITEM_MERCURY_STAR);
  Inventory_AddItem(ISAAC, ITEM_MARS_STAR);
  Inventory_AddItem(ISAAC, ITEM_JUPITER_STAR);
  Inventory_AddItem(ISAAC, ITEM_MYTHRIL_BAG);
  Inventory_AddItem(GARET, ITEM_SMOKE_BOMB);
  Inventory_AddItem(GARET, ITEM_SLEEP_BOMB);
  Inventory_AddItem(GARET, ITEM_DRAGONS_EYE);
  Inventory_AddItem(GARET, ITEM_GAME_TICKET);
  Inventory_AddItem(GARET, ITEM_GAME_TICKET);
  Inventory_AddItem(GARET, ITEM_GAME_TICKET);
  Inventory_AddItem(GARET, ITEM_GAME_TICKET);
  Inventory_AddItem(GARET, ITEM_GAME_TICKET);
  Inventory_AddItem(GARET, ITEM_GAME_TICKET);
  Inventory_AddItem(GARET, ITEM_GAME_TICKET);
  Inventory_AddItem(GARET, ITEM_GAME_TICKET);
  Inventory_AddItem(GARET, ITEM_GAME_TICKET);
  Inventory_AddItem(GARET, ITEM_GAME_TICKET);
  Inventory_AddItem(GARET, ITEM_GAME_TICKET);
  Inventory_AddItem(GARET, ITEM_LUCKY_MEDAL);
  Inventory_AddItem(GARET, ITEM_LUCKY_MEDAL);
  Inventory_AddItem(GARET, ITEM_LUCKY_MEDAL);
  Inventory_AddItem(GARET, ITEM_LUCKY_MEDAL);
  Inventory_AddItem(GARET, ITEM_LUCKY_MEDAL);
  Inventory_AddItem(GARET, ITEM_LUCKY_MEDAL);
  Inventory_AddItem(GARET, ITEM_LUCKY_MEDAL);
  Inventory_AddItem(GARET, ITEM_LUCKY_MEDAL);
  Inventory_AddItem(GARET, ITEM_ANCHOR_CHARM);
  Inventory_AddItem(GARET, ITEM_BONE);
  Inventory_AddItem(GARET, ITEM_MYSTIC_DRAUGHT);
  Inventory_AddItem(IVAN, ITEM_BLACK_ORB);
  Inventory_AddItem(IVAN, ITEM_FUR_BOOTS);
  Inventory_AddItem(IVAN, ITEM_FAIRY_RING);
  Inventory_AddItem(IVAN, ITEM_HEALING_RING);
  Inventory_AddItem(IVAN, ITEM_RUNNING_SHIRT);
  Inventory_AddItem(MIA, ITEM_WATER_OF_LIFE);
  Inventory_AddItem(MIA, ITEM_ORB_OF_FORCE);
  Inventory_AddItem(MIA, ITEM_DOUSE_DROP);
  Inventory_AddItem(MIA, ITEM_FROST_JEWEL);
  Inventory_AddItem(MIA, ITEM_LIFTING_GEM);
  Inventory_AddItem(MIA, ITEM_HALT_GEM);
  Inventory_AddItem(MIA, ITEM_CATCH_BEADS);
  DebugMenu_RecalculateParty();
}

/// Sets the debug party, stock counters and levels; returns zero. The room's
/// initialize entry, which Field_RunScene runs on every entry and whose
/// result it ignores: so each visit adds to the shops' stock again, but
/// Unit_LevelUpTo only raises, so a later visit changes no level unless a
/// member is below 30 (Isaac 50 outside Japanese).
s32 CommandTable_ConfigureCommandList(void) {
  // Assemble the test party before adding stock and setting levels: Jenna
  // leaves, Garet, Mia and Ivan join.
  Party_RemoveActiveOwner(JENNA);
  Party_AddActiveOwner(GARET);
  Party_AddActiveOwner(MIA);
  Party_AddActiveOwner(IVAN);
  // Populate the original shop counters, retaining repeated increments: how
  // many of each rare item the shops may sell, here three Shamshirs, two
  // Silver Blades, a Masamune and eight rare pieces of armor.
  Item_AdjustCounter(ITEM_SHAMSHIR, 1);
  Item_AdjustCounter(ITEM_SHAMSHIR, 1);
  Item_AdjustCounter(ITEM_SHAMSHIR, 1);
  Item_AdjustCounter(ITEM_SILVER_BLADE, 1);
  Item_AdjustCounter(ITEM_SILVER_BLADE, 1);
  Item_AdjustCounter(ITEM_MASAMUNE, 1);
  Item_AdjustCounter(ITEM_CHINA_DRESS, 1);
  Item_AdjustCounter(ITEM_COCKTAIL_DRESS, 1);
  Item_AdjustCounter(ITEM_BLESSED_ROBE, 1);
  Item_AdjustCounter(ITEM_ORACLES_ROBE, 1);
  Item_AdjustCounter(ITEM_DRAGON_SHIELD, 1);
  Item_AdjustCounter(ITEM_VAMBRACE, 1);
  Item_AdjustCounter(ITEM_SPIRIT_ARMLET, 1);
  Item_AdjustCounter(ITEM_WARRIORS_HELM, 1);
#if !TBS_PATCH_1
  gGameState.coins = 777777;
#endif
#if TBS_PATCH_1
  Unit_LevelUpTo(ISAAC, 50);
#else
  Unit_LevelUpTo(ISAAC, 30);
#endif
  Unit_LevelUpTo(GARET, 30);
  Unit_LevelUpTo(MIA, 30);
  Unit_LevelUpTo(IVAN, 30);
  DebugMenu_RecalculateParty();
  return 0;
}

/// Returns the icon browser's status to the event dispatcher. Talking to
/// ACTOR_IVY.
static s32 DebugMenu_OpenIconBrowser(void) { return DebugMenu_BrowseIcons(); }

#if !TBS_PATCH_1
/// Returns the Japanese character selector's status. Talking to ACTOR_FELIX in
/// Japanese: the party member chooser, whose choice nothing uses.
static s32 DebugMenu_SelectCharacter(void) {
  return Menu_OpenCharacterSelector();
}
#endif

#if !TBS_PATCH_3
/// Forwards the selected owner and shop confirmation status. Talking to
/// ACTOR_SECOND_DORA in Japanese, English and German: the sanctum's service
/// menu, with ACTOR_SECOND_DORA as its keeper, since Scene_TalkToActor passes
/// the actor's id.
static s32 DebugMenu_ConfirmShopAction(s32 unitId) {
  return Shop_ConfirmAct(unitId);
}
#endif

/// Eight rows of seven words that nothing reads by name, the same 56 words as
/// the Djinn room's gTestRooms_EntrancesHead (rooms.c), which says why their
/// meaning is unknown.
u32 gDebugItemPromptHead[] = {
    0x500050, 0, 80,  0x500000, 0x10003, 2, 0,
    0x780078, 0, 120, 0x780000, 0x10003, 2, 0,
    0x28003c, 0, 128, 0x800000, 0x10002, 1, 0,
    0x28003c, 0, 128, 0x800000, 0x10002, 3, 0,
    0x800080, 0, 128, 0x800000, 0x20003, 1, 0,
    0x800080, 0, 128, 0x800000, 0x20003, 1, 0,
    0x200020, 0, 0,   0,        0,       0, 0,
    0x800080, 0, 128, 0x800000, 0x20003, 1, 0,
};

char gDebugItemPrompt[12] = "Item No:";

char gDebugItemCapacityLabel[24] = "A:Get Item  B:Return";

char gDebugItemFullLabel[20] = "ITEM FULL........";

struct SceneEntrance gMenuTest_Entrances[] = {
    {0, CONDITION_ALWAYS, 60, 0, 94, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

/// No exits: 511 is SCENE_EXITS_END's value.
u32 gMenuTest_Exits[] = {
    SCENE_EXITS_END,
};

struct ScenePlacement gMenuTest_Placements[] = {
    {GARET, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(40), 0, PIXELS(48),
     FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {JENNA, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(40), 0, PIXELS(80),
     FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {IVAN, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(40), 0, PIXELS(112),
     FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {MIA, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(40), 0, PIXELS(144),
     FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {0x1e, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(80), 0, PIXELS(48),
     FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {0x1f, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(80), 0, PIXELS(80),
     FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {0x20, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(80), 0, PIXELS(112),
     FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {0x21, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(80), 0, PIXELS(144),
     FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {0x7d, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(120), 0, PIXELS(48),
     FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {0x1f, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(120), 0, PIXELS(80),
     FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {0x20, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(120), 0, PIXELS(112),
     FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {0xf8, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(120), 0, PIXELS(144),
     FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {0x23, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(160), 0, PIXELS(48),
     FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {0x23, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(160), 0, PIXELS(80),
     FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {SCENE_TABLE_END},
};

/// Who runs what: Garet levels, Jenna equipment, Ivan consumables, Mia
/// Isaac's name, and the placed actors (enum MenuTestActor) the rest. Outside
/// Japanese, ACTOR_FELIX and ACTOR_SECOND_FELIX run a different browser for
/// each way the leader faces (bits 15..12 of the control: 0xc north, 0x8 west,
/// 0x0 east, 0x4 south); in Japanese they open the character selector and the
/// item browser.
struct SceneEvent gMenuTest_Events[] = {
    {EVENT_TALK, GARET, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_RaisePartyLevels)},
    {EVENT_TALK, JENNA, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_GiveEquipmentTestStock)},
    {EVENT_TALK, IVAN, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_GiveConsumableTestStock)},
    {EVENT_TALK, MIA, CONDITION_ALWAYS, EVENT_SCRIPT(DebugMenu_EditIsaacName)},
    {EVENT_TALK, ACTOR_KRADEN, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_TestItemSelection)},
    {EVENT_TALK, ACTOR_DORA, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_GivePartyDjinn)},
    {EVENT_TALK, ACTOR_SATUROS, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_DamageAndAfflictParty)},
    {EVENT_TALK, ACTOR_MENARDI, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_GiveTreasureTestStock)},
    {EVENT_TALK, ACTOR_ARMOR_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_RunPresetShop)},
#if TBS_PATCH_3 && !TBS_PATCH_4
    {EVENT_TALK, ACTOR_SECOND_DORA, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_GiveItemToParty)},
#elif TBS_PATCH_4
    {EVENT_TALK, ACTOR_SECOND_DORA, CONDITION_ALWAYS,
     MSGNO(MsgTolbiTolbiShipThenTheyAreDangerous)},
#else
    {EVENT_TALK, ACTOR_SECOND_DORA, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_ConfirmShopAction)},
#endif
    {EVENT_TALK, ACTOR_SECOND_SATUROS, CONDITION_ALWAYS,
     MSGNO(MsgValeKyleAbleStop)},
    {EVENT_TALK, ACTOR_IVY, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_OpenIconBrowser)},
#if !TBS_PATCH_1
    {EVENT_TALK, ACTOR_FELIX, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_SelectCharacter)},
    {EVENT_TALK, ACTOR_SECOND_FELIX, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_OpenItemBrowser)},
#else
    {0xc400, ACTOR_FELIX, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_BrowseWeaponShopMessages)},
    {0x8400, ACTOR_FELIX, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_BrowseArmorShopMessages)},
    {0x400, ACTOR_FELIX, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_BrowseItemShopMessages)},
    {0x4400, ACTOR_FELIX, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_BrowseSanctumMessages)},
    {0xc400, ACTOR_SECOND_FELIX, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_BrowseWarriorShopMessages)},
    {0x8400, ACTOR_SECOND_FELIX, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_BrowseWarriorArmorShopMessages)},
    {0x400, ACTOR_SECOND_FELIX, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_BrowseWarriorItemShopMessages)},
#if TBS_PATCH_2
    {0x4400, ACTOR_SECOND_FELIX, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_RunMessageTest)},
#endif
#endif
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_IgnorePsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_IVY, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_IgnorePsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_IVY, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_IgnorePsynergy)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_IVY, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_EnableEffectChildMode)},
    {SCENE_EVENTS_END},
};

/// The level DebugMenu_RaisePartyLevels levels the four to next: 10 at first,
/// ten more after each use.
s32 gMenuTest_SlotOffsets = 10;

/// One less than the character the Japanese loop of DebugMenu_GivePartyDjinn
/// fills with: 65 at first, so 'B', one more each pass, back to 65 after 96.
s32 gMenuTest_FillLetter = 65;
