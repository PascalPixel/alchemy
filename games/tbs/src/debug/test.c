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
#include "debug/debug.h"
#include "field/actor.h"
#include "field/event.h"
#include "field/field_psynergy_state.h"
#include "field/scene_tables.h"
#include "game/character.h"
#include "game/game.h"
#include "gamepak/far_call.h"
#include "sound/sound.h"
#include "system/dma.h"
#include "system/heap_slots.h"
#include "system/input.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "system/system_call.h"
#include "window/message_speed.h"
#include "window/screens.h"
#include "text/msg_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern u32 gTest_UnusedRows[];
extern char gMenuTest_ItemPrompt[12];
extern char gMenuTest_ItemHelp[24];
extern char gMenuTest_ItemFull[20];
extern struct SceneEntrance gMenuTest_Entrances[];
extern u32 gMenuTest_Exits[];
extern struct ActorPlacement gMenuTest_ActorPlacements[];
extern struct Trigger gMenuTest_Triggers[];
extern s32 gMenuTest_NextLevel;
extern s32 gMenuTest_FillLetter;

/// The level Scene_Initialize raises Garet, Mia and Ivan to.
#define DEBUG_PARTY_LEVEL 30
/// The level Scene_Initialize raises Isaac to, higher in the international
/// editions.
#if TBS_PATCH_1
#define DEBUG_ISAAC_LEVEL 50
#else
#define DEBUG_ISAAC_LEVEL DEBUG_PARTY_LEVEL
#endif

/// The placed actors, numbered in the order of gMenuTest_ActorPlacements after
/// its four party rows, which keep their characters' ids. Each runs its test
/// when talked to.
enum MenuTestActorId {
  /// Kraden: the picker a full party drops an item with.
  ACTOR_KRADEN = ACTOR_FIRST_PLACED,
  ACTOR_DORA,    ///< gives the party Djinn: "You got a Djinni!"
  ACTOR_SATUROS, ///< hurts the party: "The body is being torn apart."
  ACTOR_MENARDI, ///< gives the party treasure: "You got treasure!"
  /// Vale's armor merchant: the debug shop.
  ACTOR_ARMOR_MERCHANT,
  /// Dora again: the sanctum in Japanese, English and German, a Nut in French,
  /// and in Spanish and Italian "That IS dangerous!"
  ACTOR_SECOND_DORA,
  /// Saturos again: "Kyle... Will they be able to stop the Boulder?"
  ACTOR_SECOND_SATUROS,
  /// The ivy: the icon browser. Whirlwind, Force and Catch leave it be.
  ACTOR_IVY,
  /// Felix: in Japanese the party member chooser; in the other editions the
  /// lines of the weapon, armor and item shops and the sanctum, one for each
  /// way the leader faces him.
  ACTOR_FELIX,
  /// Felix again: in Japanese the item browser; in the other editions the
  /// warrior shops' lines in the same way, and in German, French, Spanish and
  /// Italian a message test while the leader faces south.
  ACTOR_SECOND_FELIX
};

/// Recalculates the four heroes after a debug inventory or stat change.
static __inline__ void MenuTest_RecalculateParty(void) {
  Unit_Calculate(ISAAC);
  Unit_Calculate(GARET);
  Unit_Calculate(MIA);
  Unit_Calculate(IVAN);
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gMenuTest_Entrances;
}

/// The scene has no exit regions: its regions entry returns none.
const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gMenuTest_Exits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gMenuTest_ActorPlacements;
}

#if TBS_PATCH_1
/// The lines of one shop, as many as the weapon shop's.
#define SHOP_MESSAGES (MsgArmorShopWelcome - MsgWeaponShopWelcome)

/// Closes the finished message windows, opens the message and waits until it
/// is typed out, then a frame more.
static void MenuTest_ShowMessageAndWait(s32 messageId) {
  Message_CloseFinishedWindows();
  Message_OpenWindow(messageId, 5, 0,
                     MESSAGE_WINDOW_FIXED_X | MESSAGE_WINDOW_UNREAD);
  while (Message_AllTyped() == 0) {
    wait(1);
  }
  wait(1);
}

/// Browses sample dialogue with A/Down and Up; B closes the window.
static void MenuTest_BrowseMessageRange(s32 messageId, s32 messageCount) {
  struct Window *window;
  s32 messageIndex;

  gMainRoutineState.messageSpeed = MESSAGE_SPEED_FAST;
  window = Portrait_OpenWindow(SPRITE_YOUNG_MAN_IN_GREEN, 0, 0, 0);
  for (messageIndex = 0; messageIndex < messageCount; messageIndex++) {
    // A sample name, item and number for each message to show: Garet, the
    // Virtuous Armlet and a number of six digits.
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

/// The shop message browsers, run by talking to ACTOR_FELIX and
/// ACTOR_SECOND_FELIX while the leader faces a given way (gMenuTest_Triggers).
static void MenuTest_BrowseWeaponShopMessages(void) {
  MenuTest_BrowseMessageRange(MsgWeaponShopWelcome, SHOP_MESSAGES);
}

static void MenuTest_BrowseArmorShopMessages(void) {
  MenuTest_BrowseMessageRange(MsgArmorShopWelcome, SHOP_MESSAGES);
}

static void MenuTest_BrowseItemShopMessages(void) {
  MenuTest_BrowseMessageRange(MsgItemShopWelcome, SHOP_MESSAGES);
}

static void MenuTest_BrowseSanctumMessages(void) {
  MenuTest_BrowseMessageRange(MsgSanctumWelcome,
                              MsgWarriorShopWelcome - MsgSanctumWelcome);
}

static void MenuTest_BrowseWarriorShopMessages(void) {
  MenuTest_BrowseMessageRange(MsgWarriorShopWelcome, SHOP_MESSAGES);
}

static void MenuTest_BrowseWarriorArmorShopMessages(void) {
  MenuTest_BrowseMessageRange(MsgWarriorArmorShopWelcome, SHOP_MESSAGES);
}

static void MenuTest_BrowseWarriorItemShopMessages(void) {
  MenuTest_BrowseMessageRange(MsgWarriorItemShopWelcome, SHOP_MESSAGES);
}
#endif

#if TBS_PATCH_2
/// Sets the event's message twice, so the second is the one shown, with no
/// speaker, and waits until it closes.
static void MenuTest_RunMessageTest(void) {
  Event_SetMessageId(MSGNO(MsgBattleAdditionalPsynergyHelp));
  Event_SetMessageId(MSGNO(MsgValeComeOnHurry));
  // "Come on, Isaac. We have to go-now!"
  Event_ShowMessage(-1, 0);
}
#endif

/// Talking to ACTOR_ARMOR_MERCHANT: the debug shop.
static void MenuTest_OpenDebugShop(void) { Shop_OpenDebugShop(1); }

/// Unused: an event that does nothing.
UNUSED static void MenuTest_NoOp(void) {}

/// Talking to ACTOR_KRADEN: the picker a full party drops an item with, whose
/// choice of member and slot nothing uses.
static void MenuTest_OpenItemPicker(void) {
  s32 selectedOwner;
  s32 selectedSlot;
  ItemPicker_Run(&selectedOwner, &selectedSlot);
}

/// Talking to Mia: the name entry screen for Isaac.
static void MenuTest_EditIsaacName(void) { NameEntry_EditMemberName(ISAAC); }

/// Whirlwind, Force and Catch cast at ACTOR_IVY: nothing happens to it.
static void MenuTest_IgnorePsynergy(void) {}

/// Keeps Force from pushing ACTOR_IVY: run as Force is cast at it, before the
/// shot, this sets the effect's leaveTarget, and the shot then leaves its
/// target where it is (field/event_commands_force.c).
static void MenuTest_KeepIvyInPlace(void) {
  ((struct FieldEffectState *)gFieldEffectState)->leaveTarget = 1;
}

#if TBS_PATCH_3
/// Gives a Nut through the ordinary party gift and full-inventory dialogue.
/// Talking to ACTOR_SECOND_DORA in French only.
UNUSED static void MenuTest_GiveItemToParty(void) {
  PartyInventory_GiveItem(ITEM_NUT, 0);
}
#endif

const struct Trigger *Scene_GetTriggers(void) { return gMenuTest_Triggers; }

/// Talking to Garet: levels the four to gMenuTest_NextLevel, then raises it by
/// ten.
static void MenuTest_RaisePartyLevels(void) {
  // "You went up a level!"
  FieldMessage_Show(MsgDebugWentLevel, FIELD_MESSAGE_ZOOM);
  Unit_LevelUpTo(ISAAC, gMenuTest_NextLevel);
  Unit_LevelUpTo(GARET, gMenuTest_NextLevel);
  Unit_LevelUpTo(MIA, gMenuTest_NextLevel);
  Unit_LevelUpTo(IVAN, gMenuTest_NextLevel);
  gMenuTest_NextLevel += 10;
  MenuTest_RecalculateParty();
}

/// Talking to Ivan: gives the four stacks of items to use: healing items, Psy
/// Crystals, Sacred Feathers and the foods that raise a stat.
static void MenuTest_GiveConsumableTestStock(void) {
  // "You got an item!"
  FieldMessage_Show(MsgDebugGotItem, FIELD_MESSAGE_ZOOM);
  // Isaac: 17 Antidotes, 14 Herbs, 11 Nuts, 13 Vials and 9 Potions.
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
  // Garet: 13 Psy Crystals and 12 Antidotes.
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
  // Ivan: 14 Elixirs, 11 Waters of Life and 16 Sacred Feathers.
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
  // Mia: 11 Power Breads, 11 Cookies, 11 Apples, 7 Hard Nuts, 13 Mints and 8
  // Lucky Peppers.
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

#if !TBS_PATCH_1
/// The Japanese release's leftover loop fills the buffer this many times, with
/// this many letters the first time and one more each time after.
#define FILL_PASSES 16
#define FILL_FIRST_LENGTH 32
#endif

/// Talking to ACTOR_DORA: gives each of the four seven Djinn of their element
/// (Ivan six) and sets them.
static void MenuTest_GivePartyDjinn(void) {
  /* LEFTOVER(frame): the buffer the Japanese release's loop below fills
     FILL_PASSES times. */
  UNUSED u8 fillBuffer[256];
#if !TBS_PATCH_1
  s32 pass;
  s32 characterIndex;

  for (pass = 0; pass < FILL_PASSES; pass++) {
    for (characterIndex = 0; characterIndex < (s32)sizeof(fillBuffer);
         characterIndex++) {
      fillBuffer[characterIndex] = 0;
    }
    for (characterIndex = 0; characterIndex < pass + FILL_FIRST_LENGTH;
         characterIndex++) {
      fillBuffer[characterIndex] = gMenuTest_FillLetter + 1;
    }
    gMenuTest_FillLetter++;
    if (gMenuTest_FillLetter >= 'a') {
      gMenuTest_FillLetter = 'A';
    }
  }
#endif
  // "You got a Djinni!"
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

/// Talking to ACTOR_SATUROS: takes HP and PP from the four, then poisons and
/// haunts Isaac, and binds Garet and gives him deadly poison.
static void MenuTest_DamageAndAfflictParty(void) {
  struct Unit *isaac;
  struct Unit *garet;

  // "The body is being torn apart."
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
  garet->curse = CURSE_BOUND;
  garet->poison = POISON_VENOM;
  MenuTest_RecalculateParty();
}

/// Talking to Jenna: gives the four weapons and armor.
static void MenuTest_GiveEquipmentTestStock(void) {
  // "You got sturdy equipment!"
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

/// The Japanese item browser: `Item No:` over an item's name, its icon and what
/// equipping it does; A gives it to the party, and B, or A once the bags are
/// full, leaves. Unlike the Increase item and level room's, it stops on numbers
/// with no item.
static void MenuTest_SelectItem(void) {
  struct Window *window;
  struct Window *details;
  s32 selectedItemId;
  s32 redraw;
  s32 itemId;
  s32 itemCount;

  itemCount = DEBUG_ITEM_COUNT;
  Sound_Play(SE_MENU_CONFIRM);
  window = Window_Create(0, 0, 30, 7, WINDOW_FLAG_INSTANT);
  details = Window_Create(0, 8, 13, 10, WINDOW_FLAG_INSTANT);
  selectedItemId = 1;
  redraw = 1;
  // Lend HIGHLIGHT_PALETTE to the item icons, as the Increase item and level
  // room's browser does.
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
          DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
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
      Text_DrawStringInWindow(gMenuTest_ItemPrompt, window, 0, 0);
      Text_DrawNumberAtOffset(selectedItemId, 0, window, 80, 0);
      if (PartyInventory_HasSpace()) {
        Text_DrawStringInWindow(gMenuTest_ItemHelp, window, 0, 32);
        itemId = selectedItemId & INVENTORY_ITEM_ID_MASK;
        // DEAD: the item's definition is looked up and not used; the ROM keeps
        // the call.
        Item_GetDirect(itemId);
        Text_DrawMessageAsTiles(itemId + MsgItemName, window, 120, 0);
        Text_DrawMessage(itemId + MsgItemPlainName, window, 0, 16);
        WindowSprite_RedrawSavedRect(details);
        ItemMenu_DrawItemDetails(details, selectedItemId);
      } else {
        Text_DrawStringInWindow(gMenuTest_ItemFull, window, 0, 32);
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
      selectedItemId += DEBUG_BROWSER_ROW_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_LEFT) {
      selectedItemId -= DEBUG_BROWSER_ROW_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_R) {
      selectedItemId += DEBUG_BROWSER_PAGE_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_L) {
      selectedItemId -= DEBUG_BROWSER_PAGE_STEP;
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

/// Talking to ACTOR_SECOND_FELIX in Japanese: the item browser.
UNUSED static void MenuTest_OpenItemBrowser(void) { MenuTest_SelectItem(); }

/// Talking to ACTOR_MENARDI: gives the four treasure, eleven Game Tickets and
/// eight Lucky Medals among it.
static void MenuTest_GiveTreasureTestStock(void) {
  // "You got treasure!"
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

/// Sets up the room: Isaac, Garet, Mia and Ivan as the party, a stock of rare
/// items for the shops, everyone's levels, and in Japanese 777,777 coins.
s32 Scene_Initialize(void) {
  // Jenna leaves; Garet, Mia and Ivan join.
  Party_RemoveMember(JENNA);
  Party_AddMember(GARET);
  Party_AddMember(MIA);
  Party_AddMember(IVAN);
  // How many of each rare item the shops may sell: three Shamshirs, two Silver
  // Blades, a Masamune and eight rare pieces of armor.
  Item_AdjustRareItemStock(ITEM_SHAMSHIR, 1);
  Item_AdjustRareItemStock(ITEM_SHAMSHIR, 1);
  Item_AdjustRareItemStock(ITEM_SHAMSHIR, 1);
  Item_AdjustRareItemStock(ITEM_SILVER_BLADE, 1);
  Item_AdjustRareItemStock(ITEM_SILVER_BLADE, 1);
  Item_AdjustRareItemStock(ITEM_MASAMUNE, 1);
  Item_AdjustRareItemStock(ITEM_CHINA_DRESS, 1);
  Item_AdjustRareItemStock(ITEM_COCKTAIL_DRESS, 1);
  Item_AdjustRareItemStock(ITEM_BLESSED_ROBE, 1);
  Item_AdjustRareItemStock(ITEM_ORACLES_ROBE, 1);
  Item_AdjustRareItemStock(ITEM_DRAGON_SHIELD, 1);
  Item_AdjustRareItemStock(ITEM_VAMBRACE, 1);
  Item_AdjustRareItemStock(ITEM_SPIRIT_ARMLET, 1);
  Item_AdjustRareItemStock(ITEM_WARRIORS_HELM, 1);
#if !TBS_PATCH_1
  gMainRoutineState.holdings.coins = 777777;
#endif
  Unit_LevelUpTo(ISAAC, DEBUG_ISAAC_LEVEL);
  Unit_LevelUpTo(GARET, DEBUG_PARTY_LEVEL);
  Unit_LevelUpTo(MIA, DEBUG_PARTY_LEVEL);
  Unit_LevelUpTo(IVAN, DEBUG_PARTY_LEVEL);
  MenuTest_RecalculateParty();
  return 0;
}

/// Talking to ACTOR_IVY: the icon browser.
static s32 MenuTest_OpenIconBrowser(void) { return DebugBrowser_Icons(); }

#if !TBS_PATCH_1
/// Talking to ACTOR_FELIX in Japanese: the party member chooser, whose choice
/// nothing uses.
static s32 MenuTest_SelectCharacter(void) { return PartyMemberChooser_Open(); }
#endif

#if !TBS_PATCH_3
/// Talking to ACTOR_SECOND_DORA in Japanese, English and German: the sanctum.
static s32 MenuTest_OpenSanctum(s32 unitId) { return Sanctum_Run(unitId); }
#endif

#include "test_rows.c"

char gMenuTest_ItemPrompt[12] = "Item No:";

char gMenuTest_ItemHelp[24] = "A:Get Item  B:Return";

char gMenuTest_ItemFull[20] = "ITEM FULL........";

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
     EVENT(MenuTest_OpenItemPicker)},
    {TRIGGER_TALK, ACTOR_DORA, CONDITION_ALWAYS,
     EVENT(MenuTest_GivePartyDjinn)},
    {TRIGGER_TALK, ACTOR_SATUROS, CONDITION_ALWAYS,
     EVENT(MenuTest_DamageAndAfflictParty)},
    {TRIGGER_TALK, ACTOR_MENARDI, CONDITION_ALWAYS,
     EVENT(MenuTest_GiveTreasureTestStock)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
     EVENT(MenuTest_OpenDebugShop)},
#if TBS_PATCH_3 && !TBS_PATCH_4
    {TRIGGER_TALK, ACTOR_SECOND_DORA, CONDITION_ALWAYS,
     EVENT(MenuTest_GiveItemToParty)},
#elif TBS_PATCH_4
    {TRIGGER_TALK, ACTOR_SECOND_DORA, CONDITION_ALWAYS,
     MSGNO(MsgTolbiTolbiShipThenTheyAreDangerous)},
#else
    {TRIGGER_TALK, ACTOR_SECOND_DORA, CONDITION_ALWAYS,
     EVENT(MenuTest_OpenSanctum)},
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
     ACTOR_IVY, CONDITION_ALWAYS, EVENT(MenuTest_KeepIvyInPlace)},
    {TRIGGER_TABLE_END},
};

/// The level MenuTest_RaisePartyLevels levels the four to next: 10 at first,
/// ten more after each use.
s32 gMenuTest_NextLevel = 10;

/// One less than the character the Japanese loop of MenuTest_GivePartyDjinn
/// fills with: 'A' at first, so 'B', one more each pass, and back to 'A' when
/// it reaches 'a'.
s32 gMenuTest_FillLetter = 'A';
