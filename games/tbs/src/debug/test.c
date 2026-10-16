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

void Engine_EventSetMessage(s16 message);

void Engine_AudioPlayCue(s32 cue);

// The debug item browser covers IDs 0 through 269.
#define DEBUG_ITEM_COUNT 270
#define DEBUG_ITEM_ROW_STEP 10
#define DEBUG_ITEM_PAGE_STEP 30

// Recalculates the four heroes after a debug inventory or stat change.
// Preserve the original Isaac, Garet, Mia, Ivan order.
static __inline__ void DebugMenu_RecalculateParty(void) {
  UnitCalc(ISAAC);
  UnitCalc(GARET);
  UnitCalc(MIA);
  UnitCalc(IVAN);
}

// Returns the debug room entrance table through the scene entry ABI.
u8 *MenuTest_GetEntrances(void) { return (u8 *)gMenuTest_Entrances; }

// Returns zero from this scene entry.
s32 SceneData_ReturnZero(void) { return 0; }

// Returns the debug room's packed exit table.
u8 *MenuTest_GetExits(void) { return (u8 *)gMenuTest_Exits; }

// Returns the debug actors and their fixed room positions.
u8 *MenuTest_GetPlacements(void) { return (u8 *)gMenuTest_Placements; }

#if TBS_PATCH_1
// Finishes pending UI work, displays a message, and waits one final frame.
static void SceneDialogue_ShowMessageAndWait(s32 messageId) {
  UiWork_FinalizePendingCore();
  UiText_OpenMessageWindow(messageId, 5, 0, 0x22);
  while (UiWork_IsComplete() == 0) {
    Engine_TaskWait(1);
  }
  Engine_TaskWait(1);
}

// Browses sample dialogue with A/Down and Up; B closes the window.
// Other held keys finish the current iteration without a wait.
static void DebugMenu_BrowseMessageRange(s32 messageId, s32 messageCount) {
  struct UiWindow *window;
  s32 messageIndex;

  gGameState.messageSpeed = 2;
  window = UiWindow_CreateWithSideObject(125, 0, 0, 0);
  for (messageIndex = 0; messageIndex < messageCount; messageIndex++) {
    // Supply sample name, item and number substitutions before each message.
    UiWork_PushValueSlot(1, 1);
    UiWork_PushValueSlot(141, 2);
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

// Browses the weapon shop message range.
static void DebugMenu_BrowseWeaponShopMessages(void) {
  DebugMenu_BrowseMessageRange(MsgWeaponShopWelcome,
                               MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

// Browses the armor shop message range.
static void DebugMenu_BrowseArmorShopMessages(void) {
  DebugMenu_BrowseMessageRange(MsgArmorShopWelcome,
                               MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

// Browses the item shop message range.
static void DebugMenu_BrowseItemShopMessages(void) {
  DebugMenu_BrowseMessageRange(MsgItemShopWelcome,
                               MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

// Browses the sanctum message range.
static void DebugMenu_BrowseSanctumMessages(void) {
  DebugMenu_BrowseMessageRange(MsgSanctumWelcome,
                               MsgWarriorShopWelcome - MsgSanctumWelcome);
}

// Browses the warrior shop message range.
static void DebugMenu_BrowseWarriorShopMessages(void) {
  DebugMenu_BrowseMessageRange(MsgWarriorShopWelcome,
                               MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

// Browses the warrior armor shop message range.
static void DebugMenu_BrowseWarriorArmorShopMessages(void) {
  DebugMenu_BrowseMessageRange(MsgWarriorArmorShopWelcome,
                               MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

// Browses the warrior item shop message range.
static void DebugMenu_BrowseWarriorItemShopMessages(void) {
  DebugMenu_BrowseMessageRange(MsgWarriorItemShopWelcome,
                               MsgArmorShopWelcome - MsgWeaponShopWelcome);
}
#endif

#if TBS_PATCH_2
// Submits two sample messages, then waits for the battle message service.
static void DebugMenu_RunMessageTest(void) {
  Engine_EventSetMessage(MSGNO(MsgBattleAdditionalPsynergyHelp));
  Engine_EventSetMessage(MSGNO(MsgValeComeOnHurry));
  FieldEvent_ShowMessageAndWait(-1, 0);
}
#endif

// Runs the shop test's preset inventory, money and ailment setup.
static void DebugMenu_RunPresetShop(void) {
  Battle_ApplyPresetItemsAndFlags(1);
}

// UNUSED: the original empty debug callback.
UNUSED static void DebugMenu_NoOp(void) {}

// Opens the shop inventory selector and discards its selected owner and slot.
static void DebugMenu_TestItemSelection(void) {
  s32 selectedOwner;
  s32 selectedSlot;
  Shop_PickUnitItem(&selectedOwner, &selectedSlot);
}

// Opens Isaac's name editor.
static void DebugMenu_EditIsaacName(void) { NameEntry_EditOwnerName(ISAAC); }

// Leaves the test actor unchanged when targeted by Psynergy.
static void DebugMenu_IgnorePsynergy(void) {}

// Sets the effect work's child-mode byte to one.
static void DebugMenu_EnableEffectChildMode(void) {
  ((struct BattleFxScene *)gEffectWork)->childMode = 1;
}

#if TBS_PATCH_3
// Gives a Nut through the ordinary party gift and full-inventory dialogue.
static void DebugMenu_GiveItemToParty(void) {
  PartyInventory_GiveItem(ITEM_NUT, 0);
}
#endif

// Returns the event table through the original encoded scene entry ABI.
s32 MenuTest_GetEvents(void) { return (s32)gMenuTest_Events; }

// Levels the four heroes to the current debug target, then raises it by ten.
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

// Attempts each consumable grant in order, including calls after a bag fills.
// A repeated-grant loop shrinks this native function from 0x626 to 0x12c;
// retain the individual calls and their original insertion order.
static void DebugMenu_GiveConsumableTestStock(void) {
  UiText_ShowPositionedMessageAndWait(MsgDebugGotItem, 1);
  // Isaac's test inventory.
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
  Inventory_AddItem(ISAAC, 180);
  Inventory_AddItem(ISAAC, 180);
  Inventory_AddItem(ISAAC, 180);
  Inventory_AddItem(ISAAC, 180);
  Inventory_AddItem(ISAAC, 180);
  Inventory_AddItem(ISAAC, 180);
  Inventory_AddItem(ISAAC, 180);
  Inventory_AddItem(ISAAC, 180);
  Inventory_AddItem(ISAAC, 180);
  Inventory_AddItem(ISAAC, 180);
  Inventory_AddItem(ISAAC, 180);
  Inventory_AddItem(ISAAC, 180);
  Inventory_AddItem(ISAAC, 180);
  Inventory_AddItem(ISAAC, 180);
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
  Inventory_AddItem(ISAAC, 183);
  Inventory_AddItem(ISAAC, 183);
  Inventory_AddItem(ISAAC, 183);
  Inventory_AddItem(ISAAC, 183);
  Inventory_AddItem(ISAAC, 183);
  Inventory_AddItem(ISAAC, 183);
  Inventory_AddItem(ISAAC, 183);
  Inventory_AddItem(ISAAC, 183);
  Inventory_AddItem(ISAAC, 183);
  // Garet's test inventory.
  Inventory_AddItem(GARET, 186);
  Inventory_AddItem(GARET, 186);
  Inventory_AddItem(GARET, 186);
  Inventory_AddItem(GARET, 186);
  Inventory_AddItem(GARET, 186);
  Inventory_AddItem(GARET, 186);
  Inventory_AddItem(GARET, 186);
  Inventory_AddItem(GARET, 186);
  Inventory_AddItem(GARET, 186);
  Inventory_AddItem(GARET, 186);
  Inventory_AddItem(GARET, 186);
  Inventory_AddItem(GARET, 186);
  Inventory_AddItem(GARET, 186);
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
  // Ivan's test inventory.
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
  Inventory_AddItem(IVAN, 236);
  Inventory_AddItem(IVAN, 236);
  Inventory_AddItem(IVAN, 236);
  Inventory_AddItem(IVAN, 236);
  Inventory_AddItem(IVAN, 236);
  Inventory_AddItem(IVAN, 236);
  Inventory_AddItem(IVAN, 236);
  Inventory_AddItem(IVAN, 236);
  Inventory_AddItem(IVAN, 236);
  Inventory_AddItem(IVAN, 236);
  Inventory_AddItem(IVAN, 236);
  Inventory_AddItem(IVAN, 236);
  Inventory_AddItem(IVAN, 236);
  Inventory_AddItem(IVAN, 236);
  Inventory_AddItem(IVAN, 236);
  Inventory_AddItem(IVAN, 236);
  // Mia's test inventory.
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
  Inventory_AddItem(MIA, 192);
  Inventory_AddItem(MIA, 192);
  Inventory_AddItem(MIA, 192);
  Inventory_AddItem(MIA, 192);
  Inventory_AddItem(MIA, 192);
  Inventory_AddItem(MIA, 192);
  Inventory_AddItem(MIA, 192);
  Inventory_AddItem(MIA, 192);
  Inventory_AddItem(MIA, 192);
  Inventory_AddItem(MIA, 192);
  Inventory_AddItem(MIA, 192);
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

// Runs the Japanese text-buffer benchmark, then grants and sets test Djinn.
// Ivan receives six Jupiter Djinn; the other heroes receive seven.
static void DebugMenu_GivePartyDjinn(void) {
  /* FAKEMATCH: never used in the international editions, which compile out the
   * benchmark that fills it; its 256 bytes are the native frame space no
   * instruction touches. Declared inside the benchmark's #if, tbs-en loses sub
   * sp, #256 and its add: differ 4. */
  u8 benchmarkBuffer[256];
#if !TBS_PATCH_1
  s32 pass;
  s32 characterIndex;

  for (pass = 0; pass < 16; pass++) {
    for (characterIndex = 0; characterIndex < 256; characterIndex++) {
      benchmarkBuffer[characterIndex] = 0;
    }
    for (characterIndex = 0; characterIndex < pass + 32; characterIndex++) {
      benchmarkBuffer[characterIndex] = gMenuTest_BenchmarkCharacter + 1;
    }
    gMenuTest_BenchmarkCharacter++;
    if (gMenuTest_BenchmarkCharacter > 96) {
      gMenuTest_BenchmarkCharacter = 65;
    }
  }
#endif
  // Add and set each element in separate passes, preserving recovery updates.
  UiText_ShowPositionedMessageAndWait(MsgDebugGotDjinni, 1);
  Djinn_AddToOwner(ISAAC, 0, 0);
  Djinn_AddToOwner(ISAAC, 0, 1);
  Djinn_AddToOwner(ISAAC, 0, 2);
  Djinn_AddToOwner(ISAAC, 0, 3);
  Djinn_AddToOwner(ISAAC, 0, 4);
  Djinn_AddToOwner(ISAAC, 0, 5);
  Djinn_AddToOwner(ISAAC, 0, 6);
  Djinn_Activate(ISAAC, 0, 0);
  Djinn_Activate(ISAAC, 0, 1);
  Djinn_Activate(ISAAC, 0, 2);
  Djinn_Activate(ISAAC, 0, 3);
  Djinn_Activate(ISAAC, 0, 4);
  Djinn_Activate(ISAAC, 0, 5);
  Djinn_Activate(ISAAC, 0, 6);
  Djinn_AddToOwner(GARET, 2, 0);
  Djinn_AddToOwner(GARET, 2, 1);
  Djinn_AddToOwner(GARET, 2, 2);
  Djinn_AddToOwner(GARET, 2, 3);
  Djinn_AddToOwner(GARET, 2, 4);
  Djinn_AddToOwner(GARET, 2, 5);
  Djinn_AddToOwner(GARET, 2, 6);
  Djinn_Activate(GARET, 2, 0);
  Djinn_Activate(GARET, 2, 1);
  Djinn_Activate(GARET, 2, 2);
  Djinn_Activate(GARET, 2, 3);
  Djinn_Activate(GARET, 2, 4);
  Djinn_Activate(GARET, 2, 5);
  Djinn_Activate(GARET, 2, 6);
  Djinn_AddToOwner(MIA, 1, 0);
  Djinn_AddToOwner(MIA, 1, 1);
  Djinn_AddToOwner(MIA, 1, 2);
  Djinn_AddToOwner(MIA, 1, 3);
  Djinn_AddToOwner(MIA, 1, 4);
  Djinn_AddToOwner(MIA, 1, 5);
  Djinn_AddToOwner(MIA, 1, 6);
  Djinn_Activate(MIA, 1, 0);
  Djinn_Activate(MIA, 1, 1);
  Djinn_Activate(MIA, 1, 2);
  Djinn_Activate(MIA, 1, 3);
  Djinn_Activate(MIA, 1, 4);
  Djinn_Activate(MIA, 1, 5);
  Djinn_Activate(MIA, 1, 6);
  Djinn_AddToOwner(IVAN, 3, 0);
  Djinn_AddToOwner(IVAN, 3, 1);
  Djinn_AddToOwner(IVAN, 3, 2);
  Djinn_AddToOwner(IVAN, 3, 3);
  Djinn_AddToOwner(IVAN, 3, 4);
  Djinn_AddToOwner(IVAN, 3, 5);
  Djinn_Activate(IVAN, 3, 0);
  Djinn_Activate(IVAN, 3, 1);
  Djinn_Activate(IVAN, 3, 2);
  Djinn_Activate(IVAN, 3, 3);
  Djinn_Activate(IVAN, 3, 4);
  Djinn_Activate(IVAN, 3, 5);
  DebugMenu_RecalculateParty();
}

// Reduces HP and PP, then poisons and haunts Isaac and binds and venoms Garet.
// Named BattleUnit stores change address setup and store ordering in the
// native build; retain the byte cursor for poison (0x131), haunt (0x140) and
// restraint (0x130).
static void DebugMenu_DamageAndAfflictParty(void) {
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
  record[0x131] = 1;
  record += 0x140;
  *record = 1;
  record = (u8 *)UnitGet(GARET);
  record[0x130] = 1;
  record[0x131] = 2;
  DebugMenu_RecalculateParty();
}

// Adds the test equipment in each owner's original inventory order.
static void DebugMenu_GiveEquipmentTestStock(void) {
  UiText_ShowPositionedMessageAndWait(MsgDebugGotSturdyEquipment, 1);
  // Isaac's test inventory.
  Inventory_AddItem(ISAAC, 85);
  Inventory_AddItem(ISAAC, 84);
  Inventory_AddItem(ISAAC, 124);
  Inventory_AddItem(ISAAC, 123);
  Inventory_AddItem(ISAAC, 9);
  Inventory_AddItem(ISAAC, 11);
  Inventory_AddItem(ISAAC, 27);
  Inventory_AddItem(ISAAC, 26);
  // Garet's test inventory.
  Inventory_AddItem(GARET, 38);
  Inventory_AddItem(GARET, 37);
  Inventory_AddItem(GARET, 50);
  Inventory_AddItem(GARET, 49);
  Inventory_AddItem(GARET, 83);
  Inventory_AddItem(GARET, 82);
  Inventory_AddItem(GARET, 134);
  Inventory_AddItem(GARET, 133);
  Inventory_AddItem(GARET, 152);
  // Ivan's test inventory.
  Inventory_AddItem(IVAN, 64);
  Inventory_AddItem(IVAN, 65);
  Inventory_AddItem(IVAN, 98);
  Inventory_AddItem(IVAN, 97);
  Inventory_AddItem(IVAN, 124);
  Inventory_AddItem(IVAN, 131);
  Inventory_AddItem(IVAN, 141);
  Inventory_AddItem(IVAN, 163);
  // Mia's test inventory.
  Inventory_AddItem(MIA, 61);
  Inventory_AddItem(MIA, 63);
  Inventory_AddItem(MIA, 96);
  Inventory_AddItem(MIA, 95);
  Inventory_AddItem(MIA, 113);
  Inventory_AddItem(MIA, 112);
  Inventory_AddItem(MIA, 130);
  Inventory_AddItem(MIA, 142);
  Inventory_AddItem(MIA, 171);
  DebugMenu_RecalculateParty();
}

// Browses item definitions and gives the selected item while party bags allow
// it.
static void DebugMenu_SelectItem(void) {
  struct UiWindow *window;
  struct UiWindow *details;
  s32 selectedItemId;
  s32 redraw;
  s32 definitionId;
  s32 itemCount;

  itemCount = DEBUG_ITEM_COUNT;
  Engine_AudioPlayCue(112);
  window = Engine_DebugCreateWindow(0, 0, 30, 7, 2);
  details = Engine_DebugCreateWindow(0, 8, 13, 10, 2);
  selectedItemId = 1;
  redraw = 1;
  // Reuse the item palette for both debug windows.
  Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010, REG_DMA3);
  Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001, REG_DMA3);
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
        Engine_AudioPlayCue(113);
        break;
      }
      Engine_AudioPlayCue(175);
    }
    if (gKeysPressed & KEY_B) {
      Engine_AudioPlayCue(113);
      break;
    }
    if (gKeysRepeat & KEY_UP) {
      selectedItemId--;
      redraw = 1;
      Engine_AudioPlayCue(111);
    }
    if (gKeysRepeat & KEY_DOWN) {
      selectedItemId++;
      redraw = 1;
      Engine_AudioPlayCue(111);
    }
    if (gKeysRepeat & KEY_RIGHT) {
      selectedItemId += DEBUG_ITEM_ROW_STEP;
      redraw = 1;
      Engine_AudioPlayCue(111);
    }
    if (gKeysRepeat & KEY_LEFT) {
      selectedItemId -= DEBUG_ITEM_ROW_STEP;
      redraw = 1;
      Engine_AudioPlayCue(111);
    }
    if (gKeysRepeat & KEY_R) {
      selectedItemId += DEBUG_ITEM_PAGE_STEP;
      redraw = 1;
      Engine_AudioPlayCue(111);
    }
    if (gKeysRepeat & KEY_L) {
      selectedItemId -= DEBUG_ITEM_PAGE_STEP;
      redraw = 1;
      Engine_AudioPlayCue(111);
    }
    Engine_TaskWait(1);
  }
  Engine_DebugRedrawWindow(window);
  Engine_TaskWait(1);
  Engine_DebugFinalizeWindow(window, 1);
  Engine_DebugFinalizeWindow(details, 1);
}

// Opens the debug item browser.
static void DebugMenu_OpenItemBrowser(void) { DebugMenu_SelectItem(); }

// Adds the original treasure test stock, retaining duplicate tickets and
// medals.
// Sharing the duplicate grants through a counted loop shrinks this function
// from 0x194 to 0x120; retain the original calls.
static void DebugMenu_GiveTreasureTestStock(void) {
  UiText_ShowPositionedMessageAndWait(MsgDebugGotTreasure, 1);
  // Isaac's test inventory.
  Inventory_AddItem(ISAAC, 0xb8);
  Inventory_AddItem(ISAAC, 0xcc);
  Inventory_AddItem(ISAAC, 0xdc);
  Inventory_AddItem(ISAAC, 0xdd);
  Inventory_AddItem(ISAAC, 0xde);
  Inventory_AddItem(ISAAC, 0xdf);
  Inventory_AddItem(ISAAC, 0xe0);
  // Garet's test inventory.
  Inventory_AddItem(GARET, 0xe2);
  Inventory_AddItem(GARET, 0xe3);
  Inventory_AddItem(GARET, 0xe6);
  Inventory_AddItem(GARET, 0xe4);
  Inventory_AddItem(GARET, 0xe4);
  Inventory_AddItem(GARET, 0xe4);
  Inventory_AddItem(GARET, 0xe4);
  Inventory_AddItem(GARET, 0xe4);
  Inventory_AddItem(GARET, 0xe4);
  Inventory_AddItem(GARET, 0xe4);
  Inventory_AddItem(GARET, 0xe4);
  Inventory_AddItem(GARET, 0xe4);
  Inventory_AddItem(GARET, 0xe4);
  Inventory_AddItem(GARET, 0xe4);
  Inventory_AddItem(GARET, 0xe5);
  Inventory_AddItem(GARET, 0xe5);
  Inventory_AddItem(GARET, 0xe5);
  Inventory_AddItem(GARET, 0xe5);
  Inventory_AddItem(GARET, 0xe5);
  Inventory_AddItem(GARET, 0xe5);
  Inventory_AddItem(GARET, 0xe5);
  Inventory_AddItem(GARET, 0xe5);
  Inventory_AddItem(GARET, 0xe8);
  Inventory_AddItem(GARET, 0xe7);
  Inventory_AddItem(GARET, 0xed);
  // Ivan's test inventory.
  Inventory_AddItem(IVAN, 0xf2);
  Inventory_AddItem(IVAN, 0x102);
  Inventory_AddItem(IVAN, 0x10b);
  Inventory_AddItem(IVAN, 0x109);
  Inventory_AddItem(IVAN, 0xfc);
  // Mia's test inventory.
  Inventory_AddItem(MIA, 0xbd);
  Inventory_AddItem(MIA, 0xc8);
  Inventory_AddItem(MIA, 0xc9);
  Inventory_AddItem(MIA, 0xca);
  Inventory_AddItem(MIA, 0xcb);
  Inventory_AddItem(MIA, 0xcc);
  Inventory_AddItem(MIA, 0xcf);
  DebugMenu_RecalculateParty();
}

// Sets the debug party, stock counters and levels; returns zero.
s32 CommandTable_ConfigureCommandList(void) {
  // Assemble the test party before adding stock and setting levels.
  Party_RemoveActiveOwner(5);
  Party_AddActiveOwner(GARET);
  Party_AddActiveOwner(MIA);
  Party_AddActiveOwner(IVAN);
  // Populate the original shop counters, retaining repeated increments.
  Item_AdjustCounter(5, 1);
  Item_AdjustCounter(5, 1);
  Item_AdjustCounter(5, 1);
  Item_AdjustCounter(6, 1);
  Item_AdjustCounter(6, 1);
  Item_AdjustCounter(7, 1);
  Item_AdjustCounter(106, 1);
  Item_AdjustCounter(108, 1);
  Item_AdjustCounter(109, 1);
  Item_AdjustCounter(113, 1);
  Item_AdjustCounter(123, 1);
  Item_AdjustCounter(130, 1);
  Item_AdjustCounter(140, 1);
  Item_AdjustCounter(151, 1);
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

// Returns the icon browser's status to the event dispatcher.
static s32 DebugMenu_OpenIconBrowser(void) { return DebugMenu_BrowseIcons(); }

#if !TBS_PATCH_1
// Returns the Japanese character selector's status.
static s32 DebugMenu_SelectCharacter(void) {
  return Menu_OpenCharacterSelector();
}
#endif

#if !TBS_PATCH_3
// Forwards the selected owner and shop confirmation status.
static s32 DebugMenu_ConfirmShopAction(s32 unitId) {
  return Shop_ConfirmAct(unitId);
}
#endif

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

u32 gMenuTest_Exits[] = {
    SCENE_EXITS(511),
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

struct SceneEvent gMenuTest_Events[] = {
    {EVENT_TALK, 1, CONDITION_ALWAYS, EVENT_SCRIPT(DebugMenu_RaisePartyLevels)},
    {EVENT_TALK, 5, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_GiveEquipmentTestStock)},
    {EVENT_TALK, 2, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_GiveConsumableTestStock)},
    {EVENT_TALK, 3, CONDITION_ALWAYS, EVENT_SCRIPT(DebugMenu_EditIsaacName)},
    {EVENT_TALK, 8, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_TestItemSelection)},
    {EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(DebugMenu_GivePartyDjinn)},
    {EVENT_TALK, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_DamageAndAfflictParty)},
    {EVENT_TALK, 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_GiveTreasureTestStock)},
    {EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(DebugMenu_RunPresetShop)},
#if TBS_PATCH_3 && !TBS_PATCH_4
    {EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(DebugMenu_GiveItemToParty)},
#elif TBS_PATCH_4
    {EVENT_TALK, 13, CONDITION_ALWAYS,
     MSGNO(MsgTolbiTolbiShipThenTheyAreDangerous)},
#else
    {EVENT_TALK, 13, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_ConfirmShopAction)},
#endif
    {EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeKyleAbleStop)},
    {EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(DebugMenu_OpenIconBrowser)},
#if !TBS_PATCH_1
    {EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(DebugMenu_SelectCharacter)},
    {EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(DebugMenu_OpenItemBrowser)},
#else
    {0xc400, 16, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_BrowseWeaponShopMessages)},
    {0x8400, 16, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_BrowseArmorShopMessages)},
    {0x400, 16, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_BrowseItemShopMessages)},
    {0x4400, 16, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_BrowseSanctumMessages)},
    {0xc400, 17, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_BrowseWarriorShopMessages)},
    {0x8400, 17, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_BrowseWarriorArmorShopMessages)},
    {0x400, 17, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_BrowseWarriorItemShopMessages)},
#if TBS_PATCH_2
    {0x4400, 17, CONDITION_ALWAYS, EVENT_SCRIPT(DebugMenu_RunMessageTest)},
#endif
#endif
    {PSYNERGY_ON_ACTOR(0x4e), 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_IgnorePsynergy)},
    {PSYNERGY_ON_ACTOR(0x8e), 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_IgnorePsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_CATCH), 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_IgnorePsynergy)},
    {0x10008e15, 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(DebugMenu_EnableEffectChildMode)},
    {SCENE_EVENTS_END},
};

s32 gMenuTest_SlotOffsets = 10;

s32 gMenuTest_BenchmarkCharacter = 65;
