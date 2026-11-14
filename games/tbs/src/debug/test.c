/// @file
/// The debug room Menu test, whose actors each run a test of the menus, the
/// shops or the bags when talked to.

#include "ability_ids.h"
#include "djinn_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "battle/battle_action_run.h"
#include "battle/playback_queue.h"
#include "field/actor.h"
#include "field/field_psynergy_work.h"
#include "field/scene_tables.h"
#include "game/character.h"
#include "game/game.h"
#include "gamepak/far_call.h"
#include "sound/sound.h"
#include "system/dma.h"
#include "system/heap_slots.h"
#include "system/input.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "system/system_call.h"
#include "window/screens.h"
#include "text/msg_ids.h"

u8 *Scene_GetEntrances(void);
u8 *Scene_GetExits(void);
u8 *Scene_GetActorPlacements(void);
s32 Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern u32 gTest_UnusedRows[];
extern char gDebugItemPrompt[12];
extern char gDebugItemCapacityLabel[24];
extern char gDebugItemFullLabel[20];
extern struct SceneEntrance gMenuTest_Entrances[];
extern u32 gMenuTest_Exits[];
extern struct ActorPlacement gMenuTest_ActorPlacements[];
extern struct Trigger gMenuTest_Triggers[];
extern s32 gMenuTest_NextLevel;
extern s32 gMenuTest_FillLetter;

/// The debug item browser covers IDs 0 through 269.
#define DEBUG_ITEM_COUNT 270
#define DEBUG_ITEM_ROW_STEP 10  ///< Right and Left move ten numbers.
#define DEBUG_ITEM_PAGE_STEP 30 ///< R and L move thirty.

// The level Scene_Initialize raises Isaac to, which the international
// editions changed.
#if TBS_PATCH_1
#define DEBUG_ISAAC_LEVEL 50
#else
#define DEBUG_ISAAC_LEVEL 30
#endif

/// The placed actors, numbered in the order of gMenuTest_ActorPlacements after
/// its four party rows, which keep their characters' ids.
enum MenuTestActorId {
  /// Kraden's sprite, 30 (ACTOR_KRADEN in
  /// sol_sanctum_elemental_star_room.c);
  /// opens the picker a full party drops an item with.
  ACTOR_KRADEN = ACTOR_FIRST_PLACED,
  /// Dora's sprite, 31 (vale_eruption.c). `You got a Djinni!`
  ACTOR_DORA,
  /// Saturos's sprite, 32 (sol_sanctum_elemental_star_room.c).
  /// `The body is being torn apart.`
  ACTOR_SATUROS,
  /// Menardi's sprite, 33 (sol_sanctum_elemental_star_room.c).
  /// `You got treasure!`
  ACTOR_MENARDI,
  /// The sprite of Vale's armor merchant, 125 (cottage_rooms_a.c);
  /// opens
  /// the debug shop.
  ACTOR_ARMOR_MERCHANT,
  /// Dora's sprite again: the sanctum's service menu, in French a Nut, and in
  /// Spanish and Italian `That IS dangerous!`
  ACTOR_SECOND_DORA,
  /// Saturos's sprite again. "Kyle... Will they be able to stop the Boulder?"
  ACTOR_SECOND_SATUROS,
  /// The ivy's sprite, 248 ("trapped in the ivy", goma_cave_entrance.c);
  /// opens the icon browser, and takes Whirlwind, Force and Catch.
  ACTOR_IVY,
  /// Felix's sprite, 35 (sol_sanctum_elemental_star_room.c); a shop message
  /// browser, or in Japanese the character selector.
  ACTOR_FELIX,
  /// Felix's sprite again: a shop message browser, or in Japanese the item
  /// browser.
  ACTOR_SECOND_FELIX
};

/// Recalculates the four heroes after a debug inventory or stat change.
static __inline__ void MenuTest_RecalculateParty(void) {
  Unit_Calculate(ISAAC);
  Unit_Calculate(GARET);
  Unit_Calculate(MIA);
  Unit_Calculate(IVAN);
}

u8 *Scene_GetEntrances(void) { return (u8 *)gMenuTest_Entrances; }

/// The scene has no exit regions: its regions entry returns none.
s32 Scene_GetExitRegions(void) { return 0; }

u8 *Scene_GetExits(void) { return (u8 *)gMenuTest_Exits; }

u8 *Scene_GetActorPlacements(void) { return (u8 *)gMenuTest_ActorPlacements; }

#if TBS_PATCH_1
/// Finishes pending UI work, displays a message, and waits one final frame.
static void MenuTest_ShowMessageAndWait(s32 messageId) {
  Message_CloseFinishedWindows();
  Message_OpenWindow(messageId, 5, 0, 0x22);
  while (Message_AllTyped() == 0) {
    wait(1);
  }
  wait(1);
}

/// Browses sample dialogue with A/Down and Up; B closes the window.
static void MenuTest_BrowseMessageRange(s32 messageId, s32 messageCount) {
  struct Window *window;
  s32 messageIndex;

  gMainRoutineWork.messageSpeed = 2;
  window = Portrait_OpenWindow(SPRITE_YOUNG_MAN_IN_GREEN, 0, 0, 0);
  for (messageIndex = 0; messageIndex < messageCount; messageIndex++) {
    // Supply sample name, item and number substitutions before each message:
    // Garet, the Virtuous Armlet and 123456.
    MessageArgument_Push(GARET, MESSAGE_ARGUMENT_ACTOR);
    MessageArgument_Push(ITEM_VIRTUOUS_ARMLET, MESSAGE_ARGUMENT_ITEM);
    MessageArgument_Push(123456, MESSAGE_ARGUMENT_NUMBER);
    MenuTest_ShowMessageAndWait(messageId);
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
        wait(1);
      } else {
        break;
      }
    }
  }
end:
  Message_CloseFinishedWindows();
  Window_Close(window, 2);
}

// The shop message browsers, run by talking to ACTOR_FELIX and
// ACTOR_SECOND_FELIX while the leader faces a given way (gMenuTest_Triggers).
static void MenuTest_BrowseWeaponShopMessages(void) {
  MenuTest_BrowseMessageRange(MsgWeaponShopWelcome,
                              MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

static void MenuTest_BrowseArmorShopMessages(void) {
  MenuTest_BrowseMessageRange(MsgArmorShopWelcome,
                              MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

static void MenuTest_BrowseItemShopMessages(void) {
  MenuTest_BrowseMessageRange(MsgItemShopWelcome,
                              MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

static void MenuTest_BrowseSanctumMessages(void) {
  MenuTest_BrowseMessageRange(MsgSanctumWelcome,
                              MsgWarriorShopWelcome - MsgSanctumWelcome);
}

static void MenuTest_BrowseWarriorShopMessages(void) {
  MenuTest_BrowseMessageRange(MsgWarriorShopWelcome,
                              MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

static void MenuTest_BrowseWarriorArmorShopMessages(void) {
  MenuTest_BrowseMessageRange(MsgWarriorArmorShopWelcome,
                              MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

static void MenuTest_BrowseWarriorItemShopMessages(void) {
  MenuTest_BrowseMessageRange(MsgWarriorItemShopWelcome,
                              MsgArmorShopWelcome - MsgWeaponShopWelcome);
}
#endif

#if TBS_PATCH_2
/// Sets the event's message twice, so the second, MsgValeComeOnHurry, is the
/// one shown, with no speaker (actor id -1), and waits until it closes.
static void MenuTest_RunMessageTest(void) {
  Event_SetMessageId(MSGNO(MsgBattleAdditionalPsynergyHelp));
  Event_SetMessageId(MSGNO(MsgValeComeOnHurry));
  Event_ShowMessageAndWait(-1, 0);
}
#endif

/// Runs the shop test's preset inventory, money and status setup.
static void DebugMenu_RunPresetShop(void) { Shop_OpenDebugShop(1); }

UNUSED static void MenuTest_NoOp(void) {}

/// Opens the shop inventory selector and discards its selected party member and
/// slot. Talking to ACTOR_KRADEN: the picker a full party drops an item with.
static void MenuTest_TestItemSelection(void) {
  s32 selectedOwner;
  s32 selectedSlot;
  ItemPicker_Run(&selectedOwner, &selectedSlot);
}

/// Talking to Mia: the name entry screen for Isaac.
static void MenuTest_EditIsaacName(void) { NameEntry_EditMemberName(ISAAC); }

/// Leaves the test actor unchanged when targeted by Psynergy. The event
/// for Whirlwind, Force and Catch cast at ACTOR_IVY.
static void MenuTest_IgnorePsynergy(void) {}

/// Keeps Force from moving ACTOR_IVY: run at phase 1 of Force cast at it,
/// this sets the effect's childMode, and Force's shot then leaves its target
/// where it is (field/event_commands_force.c).
static void MenuTest_EnableEffectChildMode(void) {
  ((struct FieldEffectWork *)gFieldEffectWork)->childMode = 1;
}

#if TBS_PATCH_3
/// Gives a Nut through the ordinary party gift and full-inventory dialogue.
/// Talking to ACTOR_SECOND_DORA in French only.
UNUSED static void MenuTest_GiveItemToParty(void) {
  PartyInventory_GiveItem(ITEM_NUT, 0);
}
#endif

s32 Scene_GetTriggers(void) { return (s32)gMenuTest_Triggers; }

/// Levels the four heroes to the current debug target, then raises it by ten.
static void MenuTest_RaisePartyLevels(void) {
  s32 *targetLevel;

  FieldMessage_Show(MsgDebugWentLevel, FIELD_MESSAGE_ZOOM);
  targetLevel = &gMenuTest_NextLevel;
  Unit_LevelUpTo(ISAAC, *targetLevel);
  Unit_LevelUpTo(GARET, *targetLevel);
  Unit_LevelUpTo(MIA, *targetLevel);
  Unit_LevelUpTo(IVAN, *targetLevel);
  *targetLevel += 10;
  MenuTest_RecalculateParty();
}

/// Attempts each consumable grant in order, including calls after a bag fills.
/// Talking to Ivan: `You got an item!`.
static void MenuTest_GiveConsumableTestStock(void) {
  FieldMessage_Show(MsgDebugGotItem, FIELD_MESSAGE_ZOOM);
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
  MenuTest_RecalculateParty();
}

/// Grants and sets test Djinn.
static void MenuTest_GivePartyDjinn(void) {
  /* LEFTOVER(frame): the buffer tbs-ja's loop below fills sixteen times. */
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
  FieldMessage_Show(MsgDebugGotDjinni, FIELD_MESSAGE_ZOOM);
  Djinn_Give(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_FLINT));
  Djinn_Give(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GRANITE));
  Djinn_Give(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_QUARTZ));
  Djinn_Give(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_VINE));
  Djinn_Give(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_SAP));
  Djinn_Give(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GROUND));
  Djinn_Give(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_BANE));
  Djinn_Set(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_FLINT));
  Djinn_Set(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GRANITE));
  Djinn_Set(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_QUARTZ));
  Djinn_Set(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_VINE));
  Djinn_Set(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_SAP));
  Djinn_Set(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GROUND));
  Djinn_Set(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_BANE));
  Djinn_Give(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FORGE));
  Djinn_Give(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FEVER));
  Djinn_Give(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_CORONA));
  Djinn_Give(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_SCORCH));
  Djinn_Give(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_EMBER));
  Djinn_Give(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FLASH));
  Djinn_Give(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_TORCH));
  Djinn_Set(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FORGE));
  Djinn_Set(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FEVER));
  Djinn_Set(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_CORONA));
  Djinn_Set(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_SCORCH));
  Djinn_Set(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_EMBER));
  Djinn_Set(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FLASH));
  Djinn_Set(GARET, ELEMENT_MARS, DJINNI_INDEX(DJINNI_TORCH));
  Djinn_Give(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_FIZZ));
  Djinn_Give(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_SLEET));
  Djinn_Give(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_MIST));
  Djinn_Give(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_SPRITZ));
  Djinn_Give(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_HAIL));
  Djinn_Give(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_TONIC));
  Djinn_Give(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_DEW));
  Djinn_Set(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_FIZZ));
  Djinn_Set(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_SLEET));
  Djinn_Set(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_MIST));
  Djinn_Set(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_SPRITZ));
  Djinn_Set(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_HAIL));
  Djinn_Set(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_TONIC));
  Djinn_Set(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_DEW));
  Djinn_Give(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_GUST));
  Djinn_Give(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_BREEZE));
  Djinn_Give(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_ZEPHYR));
  Djinn_Give(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_SMOG));
  Djinn_Give(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_KITE));
  Djinn_Give(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_SQUALL));
  Djinn_Set(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_GUST));
  Djinn_Set(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_BREEZE));
  Djinn_Set(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_ZEPHYR));
  Djinn_Set(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_SMOG));
  Djinn_Set(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_KITE));
  Djinn_Set(IVAN, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_SQUALL));
  MenuTest_RecalculateParty();
}

/// Reduces HP and PP, then poisons and haunts Isaac and binds and venoms Garet.
static void MenuTest_DamageAndAfflictParty(void) {
  struct Unit *isaac;
  struct Unit *garet;

  FieldMessage_Show(MsgDebugBodyTornApart, FIELD_MESSAGE_ZOOM);
  FieldUnit_AddHp(ISAAC, -100);
  FieldUnit_AddHp(GARET, -100);
  FieldUnit_AddHp(IVAN, -33);
  FieldUnit_AddHp(MIA, -100);
  Unit_AddPp(ISAAC, -50);
  Unit_AddPp(GARET, -40);
  Unit_AddPp(IVAN, -35);
  Unit_AddPp(MIA, -20);
  isaac = Unit_Get(ISAAC);
  isaac->poison = POISON_NORMAL;
  isaac->haunt = 1;
  garet = Unit_Get(GARET);
  garet->curse = 1;
  garet->poison = POISON_VENOM;
  MenuTest_RecalculateParty();
}

/// Adds the test equipment in each party member's original inventory order.
static void MenuTest_GiveEquipmentTestStock(void) {
  FieldMessage_Show(MsgDebugGotSturdyEquipment, FIELD_MESSAGE_ZOOM);
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
  MenuTest_RecalculateParty();
}

/// Browses item definitions and gives the selected item while party bags allow
/// it. Talking to ACTOR_SECOND_FELIX in Japanese (MenuTest_OpenItemBrowser).
static void MenuTest_SelectItem(void) {
  struct Window *window;
  struct Window *details;
  s32 selectedItemId;
  s32 redraw;
  s32 definitionId;
  s32 itemCount;

  itemCount = DEBUG_ITEM_COUNT;
  Sound_Play(SE_MENU_CONFIRM);
  window = Window_Create(0, 0, 30, 7, WINDOW_FLAG_INSTANT);
  details = Window_Create(0, 8, 13, 10, WINDOW_FLAG_INSTANT);
  selectedItemId = 1;
  redraw = 1;
  // Reuse the item palette for both debug windows: sprite palette 0 into BG
  // HIGHLIGHT_PALETTE, and WINDOW_BACKGROUND_COLOR over its
  // TEXT_BACKGROUND_COLOR, as the Increase item and level scene's browser does.
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
          (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR),
          DMA_COPY_HALFWORDS(1), REG_DMA3);
  wait(1);
  while (1) {
    if (redraw) {
      redraw = 0;
      selectedItemId = (selectedItemId + DEBUG_ITEM_COUNT) % itemCount;
      WindowSprite_RedrawSavedRect(window);
      WindowSprite_ClearList(window);
      Text_DrawStringInWindow(gDebugItemPrompt, window, 0, 0);
      Text_DrawNumberAtOffset(selectedItemId, 0, window, 80, 0);
      if (PartyInventory_HasSpace()) {
        Text_DrawStringInWindow(gDebugItemCapacityLabel, window, 0, 32);
        definitionId = selectedItemId & INVENTORY_ITEM_ID_MASK;
        Item_GetDirect(definitionId);
        Text_DrawMessageAsTiles(definitionId + MsgItemName, window, 120, 0);
        Text_DrawMessage(definitionId + MsgItemPlainName, window, 0, 16);
        WindowSprite_RedrawSavedRect(details);
        ItemMenu_DrawItemDetails(details, selectedItemId);
      } else {
        Text_DrawStringInWindow(gDebugItemFullLabel, window, 0, 32);
      }
    }
    if (gKeysPressed & KEY_A) {
      if (PartyInventory_Add(selectedItemId) == -1) {
        Sound_Play(SE_MENU_CANCEL);
        break;
      }
      Sound_Play(SE_NOTE_HIGH);
    }
    if (gKeysPressed & KEY_B) {
      Sound_Play(SE_MENU_CANCEL);
      break;
    }
    if (gKeysRepeat & KEY_UP) {
      selectedItemId--;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_DOWN) {
      selectedItemId++;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_RIGHT) {
      selectedItemId += DEBUG_ITEM_ROW_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_LEFT) {
      selectedItemId -= DEBUG_ITEM_ROW_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_R) {
      selectedItemId += DEBUG_ITEM_PAGE_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_L) {
      selectedItemId -= DEBUG_ITEM_PAGE_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    wait(1);
  }
  WindowSprite_RedrawSavedRect(window);
  wait(1);
  Window_Close(window, 1);
  Window_Close(details, 1);
}

UNUSED static void MenuTest_OpenItemBrowser(void) { MenuTest_SelectItem(); }

/// Gives the party treasure: key items, eleven Game Tickets and eight Lucky
/// Medals. Talking to ACTOR_MENARDI: `You got treasure!`.
static void MenuTest_GiveTreasureTestStock(void) {
  FieldMessage_Show(MsgDebugGotTreasure, FIELD_MESSAGE_ZOOM);
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
  MenuTest_RecalculateParty();
}

/// Sets the debug party, stock counters and levels; returns zero.
s32 Scene_Initialize(void) {
  // Jenna leaves; Garet, Mia and Ivan join.
  Party_RemoveMember(JENNA);
  Party_AddMember(GARET);
  Party_AddMember(MIA);
  Party_AddMember(IVAN);
  // How many of each rare item the shops may sell: three Shamshirs, two Silver
  // Blades, a Masamune and eight rare pieces of armor.
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
  gMainRoutineWork.coins = 777777;
#endif
  Unit_LevelUpTo(ISAAC, DEBUG_ISAAC_LEVEL);
  Unit_LevelUpTo(GARET, 30);
  Unit_LevelUpTo(MIA, 30);
  Unit_LevelUpTo(IVAN, 30);
  MenuTest_RecalculateParty();
  return 0;
}

/// Returns the icon browser's status to the event dispatcher. Talking to
/// ACTOR_IVY.
static s32 MenuTest_OpenIconBrowser(void) { return DebugBrowser_Icons(); }

#if !TBS_PATCH_1
/// Returns the Japanese character selector's status. Talking to ACTOR_FELIX
/// in Japanese: the party member chooser, whose choice nothing uses.
static s32 MenuTest_SelectCharacter(void) { return PartyMemberChooser_Open(); }
#endif

#if !TBS_PATCH_3
/// Forwards the selected party member and shop confirmation status.
static s32 MenuTest_ConfirmShopAction(s32 unitId) {
  return Sanctum_Run(unitId);
}
#endif

#include "test_rows.c"

char gDebugItemPrompt[12] = "Item No:";

char gDebugItemCapacityLabel[24] = "A:Get Item  B:Return";

char gDebugItemFullLabel[20] = "ITEM FULL........";

struct SceneEntrance gMenuTest_Entrances[] = {
    {0, CONDITION_ALWAYS, 60, 0, 94, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

/// No exits.
u32 gMenuTest_Exits[] = {
    SCENE_EXITS_END,
};

struct ActorPlacement gMenuTest_ActorPlacements[] = {
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
                    MOVEMENT_TYPE_STAND, 120, 0, 48, FACING_SOUTHEAST,
                    TALK_FACING_PARTY, 0),
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

/// Who runs what: Garet levels, Jenna equipment, Ivan consumables, Mia Isaac's
/// name, and the placed actors (enum MenuTestActorId) the rest.
struct Trigger gMenuTest_Triggers[] = {
    {TRIGGER_TALK, GARET, CONDITION_ALWAYS, EVENT(MenuTest_RaisePartyLevels)},
    {TRIGGER_TALK, JENNA, CONDITION_ALWAYS,
     EVENT(MenuTest_GiveEquipmentTestStock)},
    {TRIGGER_TALK, IVAN, CONDITION_ALWAYS,
     EVENT(MenuTest_GiveConsumableTestStock)},
    {TRIGGER_TALK, MIA, CONDITION_ALWAYS, EVENT(MenuTest_EditIsaacName)},
    {TRIGGER_TALK, ACTOR_KRADEN, CONDITION_ALWAYS,
     EVENT(MenuTest_TestItemSelection)},
    {TRIGGER_TALK, ACTOR_DORA, CONDITION_ALWAYS,
     EVENT(MenuTest_GivePartyDjinn)},
    {TRIGGER_TALK, ACTOR_SATUROS, CONDITION_ALWAYS,
     EVENT(MenuTest_DamageAndAfflictParty)},
    {TRIGGER_TALK, ACTOR_MENARDI, CONDITION_ALWAYS,
     EVENT(MenuTest_GiveTreasureTestStock)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
     EVENT(DebugMenu_RunPresetShop)},
#if TBS_PATCH_3 && !TBS_PATCH_4
    {TRIGGER_TALK, ACTOR_SECOND_DORA, CONDITION_ALWAYS,
     EVENT(MenuTest_GiveItemToParty)},
#elif TBS_PATCH_4
    {TRIGGER_TALK, ACTOR_SECOND_DORA, CONDITION_ALWAYS,
     MSGNO(MsgTolbiTolbiShipThenTheyAreDangerous)},
#else
    {TRIGGER_TALK, ACTOR_SECOND_DORA, CONDITION_ALWAYS,
     EVENT(MenuTest_ConfirmShopAction)},
#endif
    {TRIGGER_TALK, ACTOR_SECOND_SATUROS, CONDITION_ALWAYS,
     MSGNO(MsgValeKyleAbleStop)},
    {TRIGGER_TALK, ACTOR_IVY, CONDITION_ALWAYS,
     EVENT(MenuTest_OpenIconBrowser)},
#if !TBS_PATCH_1
    {TRIGGER_TALK, ACTOR_FELIX, CONDITION_ALWAYS,
     EVENT(MenuTest_SelectCharacter)},
    {TRIGGER_TALK, ACTOR_SECOND_FELIX, CONDITION_ALWAYS,
     EVENT(MenuTest_OpenItemBrowser)},
#else
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_FELIX, CONDITION_ALWAYS,
     EVENT(MenuTest_BrowseWeaponShopMessages)},
    {TALK_TRIGGER_FACING(FACING_WEST), ACTOR_FELIX, CONDITION_ALWAYS,
     EVENT(MenuTest_BrowseArmorShopMessages)},
    {TALK_TRIGGER_FACING(FACING_EAST), ACTOR_FELIX, CONDITION_ALWAYS,
     EVENT(MenuTest_BrowseItemShopMessages)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), ACTOR_FELIX, CONDITION_ALWAYS,
     EVENT(MenuTest_BrowseSanctumMessages)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_SECOND_FELIX, CONDITION_ALWAYS,
     EVENT(MenuTest_BrowseWarriorShopMessages)},
    {TALK_TRIGGER_FACING(FACING_WEST), ACTOR_SECOND_FELIX, CONDITION_ALWAYS,
     EVENT(MenuTest_BrowseWarriorArmorShopMessages)},
    {TALK_TRIGGER_FACING(FACING_EAST), ACTOR_SECOND_FELIX, CONDITION_ALWAYS,
     EVENT(MenuTest_BrowseWarriorItemShopMessages)},
#if TBS_PATCH_2
    {TALK_TRIGGER_FACING(FACING_SOUTH), ACTOR_SECOND_FELIX, CONDITION_ALWAYS,
     EVENT(MenuTest_RunMessageTest)},
#endif
#endif
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY, CONDITION_ALWAYS,
     EVENT(MenuTest_IgnorePsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_IVY, CONDITION_ALWAYS,
     EVENT(MenuTest_IgnorePsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_IVY, CONDITION_ALWAYS,
     EVENT(MenuTest_IgnorePsynergy)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_IVY, CONDITION_ALWAYS, EVENT(MenuTest_EnableEffectChildMode)},
    {TRIGGER_TABLE_END},
};

/// The level MenuTest_RaisePartyLevels levels the four to next: 10 at first,
/// ten more after each use.
s32 gMenuTest_NextLevel = 10;

/// One less than the character the Japanese loop of MenuTest_GivePartyDjinn
/// fills with: 65 at first, so 'B', one more each pass, back to 65 after 96.
s32 gMenuTest_FillLetter = 65;
