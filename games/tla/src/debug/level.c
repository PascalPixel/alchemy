/// @file
/// The debug room Increase item and level, as The Broken Seal's: its actors
/// raise the party's levels, give any item and show the game's icons and
/// portraits. Through entrances 96 to 99 the same scene is instead one of four
/// rooms of people in rows, each of whom says their name when talked to.

#include "battle/battle.h"
#include "sounds.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/input.h"
#include "text/msg_ids.h"
#include "game/item.h"
#include "system/dma.h"
#include "battle/battle_unit.h"
#include "game/character_ids.h"

extern struct ActorPlacement gIncreaseItemAndLevel_ActorPlacements[];
extern struct ActorPlacement gIncreaseItemAndLevel_FirstNamePlacements[];
extern struct ActorPlacement gIncreaseItemAndLevel_SecondNamePlacements[];
extern struct ActorPlacement gIncreaseItemAndLevel_ThirdNamePlacements[];
extern struct ActorPlacement gIncreaseItemAndLevel_FourthNamePlacements[];
extern struct Trigger gIncreaseItemAndLevel_NameTriggers[];
extern struct Trigger gIncreaseItemAndLevel_Triggers[];
extern char gIncreaseItemAndLevel_LvLabel[];
extern char gIncreaseItemAndLevel_ItemPrompt[];
extern char gIncreaseItemAndLevel_ItemHelp[];
extern char gIncreaseItemAndLevel_ItemFull[];
void IncreaseItemAndLevel_RunMotionTest(void);
void IncreaseItemAndLevel_EnableEffectChildMode(void);
void IncreaseItemAndLevel_OpenIconBrowser(void);
void IncreaseItemAndLevel_OpenPortraitBrowser(void);

#include "edition.h"
#include "field/field_sequence_event.h"

/// The entrances that open a room of names in place of the debug room: each
/// has its own actor placements and the triggers of
/// gIncreaseItemAndLevel_NameTriggers.
enum {
  ENTRANCE_NAMES_FOURTH = 96,
  ENTRANCE_NAMES_THIRD = 97,
  ENTRANCE_NAMES_SECOND = 98,
  ENTRANCE_NAMES_FIRST = 99
};

/// The debug room's actors, numbering the rows of
/// gIncreaseItemAndLevel_ActorPlacements, as The Broken Seal's.
enum IncreaseItemAndLevelActorId {
  ACTOR_ITEM_BROWSER = 8, ///< IncreaseItemAndLevel_SelectItem
  ACTOR_ICON_BROWSER,     ///< IncreaseItemAndLevel_OpenIconBrowser
  /// IncreaseItemAndLevel_OpenPortraitBrowser; Force cast at it runs
  /// IncreaseItemAndLevel_RunMotionTest.
  ACTOR_PORTRAIT_BROWSER,
  /// IncreaseItemAndLevel_RunLevelEditor, drawn larger than the others.
  ACTOR_LEVEL_EDITOR
};

#include "scene_tables.h"
#include "module/game/game.h"
#include "module/window/screens.h"
#include "module/window/window.h"
#include "graphics/text_render.h"
#include "graphics/uiflow.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// The scene's actor placements: a room of names for its entrance, else the
/// debug room's.
const struct ActorPlacement *IncreaseItemAndLevel_GetActorPlacements(void) {
  if (gMainRoutineWork.entrance == ENTRANCE_NAMES_FIRST)
    return gIncreaseItemAndLevel_FirstNamePlacements;
  if (gMainRoutineWork.entrance == ENTRANCE_NAMES_SECOND)
    return gIncreaseItemAndLevel_SecondNamePlacements;
  if (gMainRoutineWork.entrance == ENTRANCE_NAMES_THIRD)
    return gIncreaseItemAndLevel_ThirdNamePlacements;
  if (gMainRoutineWork.entrance == ENTRANCE_NAMES_FOURTH)
    return gIncreaseItemAndLevel_FourthNamePlacements;
  return gIncreaseItemAndLevel_ActorPlacements;
}

/// Shows the name of the person talked to in a room of names: actor `actorId`
/// says the message its room's list of names gives it. The first room's list
/// starts at Master Poi, the third's at Alex.
void IncreaseItemAndLevel_ShowName(s32 actorId) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (gMainRoutineWork.entrance == ENTRANCE_NAMES_FIRST)
    Event_SetMessageId(actorId + (s32)MsgFixingThisWill);
  else if (gMainRoutineWork.entrance == ENTRANCE_NAMES_SECOND)
    Event_SetMessageId(actorId + (s32)MsgFortuneTeller2);
  else if (gMainRoutineWork.entrance == ENTRANCE_NAMES_THIRD)
    Event_SetMessageId(actorId + (s32)MsgConfirmGraphic2);
  else
    Event_SetMessageId(actorId + (s32)MsgPiers2);
  Event_ShowMessageAndWait(actorId, 0);
  Event_End();
}

/// Shows a party member's name when they are talked to in a room of names:
/// Isaac's for ISAAC, and so on, with the character as the message's actor
/// argument.
void IncreaseItemAndLevel_ShowCharacterName(s32 character) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(character + (s32)MsgIsaac2);
  MessageArgument_Push(character, 1);
  Event_ShowMessageAndWait(character, 0);
  // XXX: why Garet's name is followed by a message from Isaac; the room's
  // render, talking to Garet, would settle it.
  if (character == GARET)
    Event_ShowMessageAndWait(ISAAC, 0);
  Event_End();
}

/// The scene's triggers: the rooms of names' for any entrance above 94, else
/// the debug room's.
const struct Trigger *IncreaseItemAndLevel_GetTriggers(void) {
  if (gMainRoutineWork.entrance > 94)
    return gIncreaseItemAndLevel_NameTriggers;
  return gIncreaseItemAndLevel_Triggers;
}

/// Raises one party member's level by amount, then recalculates that unit.
void IncreaseItemAndLevel_AdjustOwnerLevel(s32 ownerId, s32 amount) {
  Unit_LevelUpTo(ownerId,
                 ((struct BattleUnit *)Unit_Get(ownerId))->level + amount);
  Unit_Calculate(ownerId);
}

/// Applies the level change to every party member.
void IncreaseItemAndLevel_AdjustPartyLevels(s32 amount) {
  u16 ownerIds[16];
  s32 ownerCount;
  s32 i;

  ownerCount = FieldParty_ListMembers(ownerIds);
  for (i = 0; i < ownerCount; i++)
    IncreaseItemAndLevel_AdjustOwnerLevel(ownerIds[i], amount);
}

/// The pressed keys under a mask, for the last test of a group: the next
/// test loads gInput's address again.
static inline u32 Input_PressedFenced(u32 mask) {
  u32 keys = gInput.pressed & mask;

  return keys;
}

/// Displays the leader's level and changes the whole party's, as The Broken
/// Seal's: talking to ACTOR_LEVEL_EDITOR shows `Raise everyone's level.` over
/// the leader's name and level; Start or Select raises the whole party five
/// levels with a jingle (sound 93), A one level (sound 91). B closes the
/// window and recalculates the original four heroes.
void IncreaseItemAndLevel_RunLevelEditor(void) {
  struct BattleUnit *unit;
  void *window;
  s32 redraw;
  s32 messageId;

  unit = Unit_Get(gMainRoutineWork.currentOwner);
  window = Window_Create(0, 0, 30, 9, 2);

  messageId = (s32)MsgRaiseEveryoneS;
  Text_DrawMessage(messageId, window, 0, 0);
  Text_DrawMessage(messageId + 1, window, 0, 16);
  messageId += 2;
  redraw = 1;
  Text_DrawMessage(messageId, window, 0, 32);

  for (;;) {
    if (redraw != 0) {
      WindowSprite_RedrawSavedRect(window);
      Text_DrawStringAtOffset(unit->name, window, 0, 48);
      Text_DrawStringInWindow(gIncreaseItemAndLevel_LvLabel, window, 48, 48);
      redraw = 0;
      Text_DrawNumberInWindow(unit->level, 0, window, 72, 48);
    }

    if ((gInput.pressed & KEY_START) != 0 ||
        Input_PressedFenced(KEY_SELECT) != 0) {
      IncreaseItemAndLevel_AdjustPartyLevels(5);
      Sound_Play(MUS_093);
      redraw = 1;
    }

    if ((gInput.pressed & KEY_A) != 0) {
      IncreaseItemAndLevel_AdjustPartyLevels(1);
      Sound_Play(MUS_091);
      redraw = 1;
    }

    if ((gInput.pressed & KEY_B) != 0) {
      Sound_Play(SE_MENU_CANCEL);
      WindowSprite_RedrawSavedRect(window);
      wait(1);
      Window_Close(window, 1);

      Unit_Calculate(ISAAC);
      Unit_Calculate(GARET);
      Unit_Calculate(MIA);
      Unit_Calculate(IVAN);
      return;
    }

    wait(1);
  }
}

#define ITEM_COUNT 512      ///< The browser's item numbers, 0 to 511, wrapping.
#define BROWSER_ROW_STEP 10 ///< Right and Left move ten numbers.
#define BROWSER_PAGE_STEP 30 ///< R and L move thirty.
#define ITEM_ID_MASK 0x1ff

void ItemMenu_DrawItemDetails(void *window, s32 item);

/// Browses nonempty item definitions and gives the selected item to the party,
/// as The Broken Seal's: talking to ACTOR_ITEM_BROWSER. Up and Down step one
/// number, Right and Left ten, R and L thirty, passing over numbers whose item
/// has no icon; A gives the item to the first party member with room, and B,
/// or A with every bag full, closes the browser.
void IncreaseItemAndLevel_SelectItem(void) {
  void *window;
  void *details;
  s32 selectedItemId;
  s32 redraw;
  s32 messageOrDefinitionId;
  s32 entryCount;
  s8 seekDirection;

  entryCount = ITEM_COUNT;
  seekDirection = 0;
  Sound_Play(SE_MENU_CONFIRM);
  selectedItemId = 1;
  redraw = 1;
  window = Window_Create(0, 0, 30, 7, 2);
#if LANGUAGE < LANGUAGE_GERMAN
  details = Window_Create(0, 8, 13, 10, 2);
#else
  details = Window_Create(0, 9, 18, 10, 2);
#endif
  // Lend the highlight palette (BG palette 14) to the item icons, as The
  // Broken Seal's: sprite palette 0 into it, then the window background colour
  // over its text background.
  Dma_Set((const void *)OBJ_PLTT, (void *)(BG_PLTT + 14 * 16), 0x80000010,
          (volatile u32 *)0x040000d4);
  Dma_Set((const void *)(BG_PLTT + 244), (void *)(BG_PLTT + 238), 0x80000001,
          (volatile u32 *)0x040000d4);
  wait(1);
  while (1) {
    if (redraw) {
      redraw = 0;
      // Wrap the number into 0 to 511 (a step back from 0 is 511).
      selectedItemId = (selectedItemId + ITEM_COUNT) % entryCount;
      WindowSprite_RedrawSavedRect(window);
      WindowSprite_ClearList(window);
      Text_DrawStringInWindow(gIncreaseItemAndLevel_ItemPrompt, window, 0, 0);
      Text_DrawNumberAtOffset(selectedItemId, 0, window, 80, 0);
      if (PartyInventory_HasSpace()) {
        Text_DrawStringInWindow(gIncreaseItemAndLevel_ItemHelp, window, 0, 32);
        messageOrDefinitionId = selectedItemId & ITEM_ID_MASK;
        Item_GetDirect(messageOrDefinitionId);
        Text_DrawMessageAsTiles(messageOrDefinitionId + (s32)MsgItemName,
                                window, 120, 0);
        Text_DrawMessage(messageOrDefinitionId + (s32)MsgItemPlainName, window,
                         0, 16);
        WindowSprite_RedrawSavedRect(details);
        ItemMenu_DrawItemDetails(details, selectedItemId);
      } else {
        Text_DrawStringInWindow(gIncreaseItemAndLevel_ItemFull, window, 0, 32);
      }
    }
    if (Input_PressedFenced(KEY_A)) {
      if (PartyInventory_Add(selectedItemId) == -1) {
        Sound_Play(SE_MENU_CANCEL);
        break;
      }
      Sound_Play(SE_NOTE_HIGH);
    }
    if (gInput.pressed & KEY_B) {
      Sound_Play(SE_MENU_CANCEL);
      break;
    }
    if (gInput.repeat & KEY_UP) {
      seekDirection = -1;
      selectedItemId--;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_DOWN) {
      seekDirection = 1;
      selectedItemId++;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_RIGHT) {
      seekDirection = 1;
      selectedItemId += BROWSER_ROW_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_LEFT) {
      seekDirection = -1;
      selectedItemId -= BROWSER_ROW_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_R) {
      seekDirection = 1;
      selectedItemId += BROWSER_PAGE_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_L) {
      seekDirection = -1;
      selectedItemId -= BROWSER_PAGE_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    // Skip items with no icon in the chosen direction.
    if (seekDirection == -1) {
      selectedItemId = (selectedItemId + ITEM_COUNT) % entryCount;
      goto check_back;
    back:
      selectedItemId = (selectedItemId + ITEM_COUNT - 1) % entryCount;
    check_back:
      if (Item_GetDirect(selectedItemId & ITEM_ID_MASK)->icon == 0)
        goto back;
    }
    if (seekDirection == 1) {
      selectedItemId = (selectedItemId + ITEM_COUNT) % entryCount;
      goto check_ahead;
    ahead:
      selectedItemId = (selectedItemId + ITEM_COUNT + 1) % entryCount;
    check_ahead:
      if (Item_GetDirect(selectedItemId & ITEM_ID_MASK)->icon == 0)
        goto ahead;
    }
    seekDirection = 0;
    wait(1);
  }
  WindowSprite_RedrawSavedRect(window);
  wait(1);
  Window_Close(window, 1);
  Window_Close(details, 1);
}

char gIncreaseItemAndLevel_LvLabel[4] = "Lv";

char gIncreaseItemAndLevel_ItemPrompt[12] = "Item No:";

char gIncreaseItemAndLevel_ItemHelp[24] = "A:Get Item  B:Return";

char gIncreaseItemAndLevel_ItemFull[20] = "ITEM FULL........";

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 80, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 80, 0, 272, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {ENTRANCE_NAMES_FOURTH, CONDITION_ALWAYS, 192, 0, 240, FACING_NORTH, 0, -1,
     -1, -1, -1, 0},
    {ENTRANCE_NAMES_THIRD, CONDITION_ALWAYS, 192, 0, 240, FACING_NORTH, 0, -1,
     -1, -1, -1, 0},
    {ENTRANCE_NAMES_SECOND, CONDITION_ALWAYS, 176, 0, 240, FACING_NORTH, 0, -1,
     -1, -1, -1, 0},
    {ENTRANCE_NAMES_FIRST, CONDITION_ALWAYS, 176, 0, 240, FACING_NORTH, 0, -1,
     -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS_END,
};

/// The debug room's actors, in the order of enum IncreaseItemAndLevelActorId.
// clang-format off
struct ActorPlacement gIncreaseItemAndLevel_ActorPlacements[] = {
    {336, CONDITION_ALWAYS, 1, PIXELS(48), 0, PIXELS(272), FACING_SOUTHEAST, 0, 0},
    {252, CONDITION_ALWAYS, 1, PIXELS(48), 0, PIXELS(72), FACING_SOUTHEAST, 0, 0},
    {253, CONDITION_ALWAYS, 1, PIXELS(48), 0, PIXELS(136), FACING_SOUTHEAST, 0, 0},
    {216, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(272), FACING_SOUTHEAST, 0, 0},
    {SCENE_TABLE_END, 0, 0, 0, 0, 0, 0, 0, 0},
};

/// The first room of names, from Master Poi.
struct ActorPlacement gIncreaseItemAndLevel_FirstNamePlacements[] = {
    {21, CONDITION_ALWAYS, 1, PIXELS(160), 0, PIXELS(160), FACING_SOUTHEAST, 2, 0},
    {18, CONDITION_ALWAYS, 1, PIXELS(160), 0, PIXELS(192), FACING_SOUTHEAST, 2, 0},
    {17, CONDITION_ALWAYS, 1, PIXELS(160), 0, PIXELS(224), FACING_SOUTHEAST, 2, 0},
    {20, CONDITION_ALWAYS, 1, PIXELS(160), 0, PIXELS(256), FACING_SOUTHEAST, 2, 0},
    {19, CONDITION_ALWAYS, 1, PIXELS(160), 0, PIXELS(288), FACING_SOUTHEAST, 2, 0},
    {24, CONDITION_ALWAYS, 1, PIXELS(160), 0, PIXELS(320), FACING_SOUTHEAST, 2, 0},
    {25, CONDITION_ALWAYS, 1, PIXELS(160), 0, PIXELS(352), FACING_SOUTHEAST, 2, 0},
    {26, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(160), FACING_SOUTHEAST, 2, 0},
    {41, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(192), FACING_SOUTHEAST, 2, 0},
    {42, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(224), FACING_SOUTHEAST, 2, 0},
    {16, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(256), FACING_SOUTHEAST, 2, 0},
    {37, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(288), FACING_SOUTHEAST, 2, 0},
    {38, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(320), FACING_SOUTHEAST, 2, 0},
    {36, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(352), FACING_SOUTHEAST, 2, 0},
    {SCENE_TABLE_END, 0, 0, 0, 0, 0, 0, 0, 0},
};

struct ActorPlacement gIncreaseItemAndLevel_SecondNamePlacements[] = {
    {29, CONDITION_ALWAYS, 1, PIXELS(256), 0, PIXELS(160), FACING_SOUTHEAST, 2, 0},
    {30, CONDITION_ALWAYS, 1, PIXELS(256), 0, PIXELS(192), FACING_SOUTHEAST, 2, 0},
    {32, CONDITION_ALWAYS, 1, PIXELS(256), 0, PIXELS(224), FACING_SOUTHEAST, 2, 0},
    {31, CONDITION_ALWAYS, 1, PIXELS(256), 0, PIXELS(256), FACING_SOUTHEAST, 2, 0},
    {34, CONDITION_ALWAYS, 1, PIXELS(256), 0, PIXELS(288), FACING_SOUTHEAST, 2, 0},
    {23, CONDITION_ALWAYS, 1, PIXELS(256), 0, PIXELS(320), FACING_SOUTHEAST, 2, 0},
    {33, CONDITION_ALWAYS, 1, PIXELS(256), 0, PIXELS(352), FACING_SOUTHEAST, 2, 0},
    {60, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(160), FACING_SOUTHEAST, 2, 0},
    {61, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(192), FACING_SOUTHEAST, 2, 0},
    {62, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(224), FACING_SOUTHEAST, 2, 0},
    {43, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(256), FACING_SOUTHEAST, 2, 0},
    {44, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(288), FACING_SOUTHEAST, 2, 0},
    {46, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(320), FACING_SOUTHEAST, 2, 0},
    {45, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(352), FACING_SOUTHEAST, 2, 0},
    {SCENE_TABLE_END, 0, 0, 0, 0, 0, 0, 0, 0},
};

/// The third room of names, from Alex.
struct ActorPlacement gIncreaseItemAndLevel_ThirdNamePlacements[] = {
    {0, CONDITION_ALWAYS, 1, PIXELS(160), 0, PIXELS(160), FACING_SOUTHEAST, 2, 0},
    {1, CONDITION_ALWAYS, 1, PIXELS(160), 0, PIXELS(192), FACING_SOUTHEAST, 2, 0},
    {2, CONDITION_ALWAYS, 1, PIXELS(160), 0, PIXELS(224), FACING_SOUTHEAST, 2, 0},
    {3, CONDITION_ALWAYS, 1, PIXELS(160), 0, PIXELS(256), FACING_SOUTHEAST, 2, 0},
    {5, CONDITION_ALWAYS, 1, PIXELS(160), 0, PIXELS(288), FACING_SOUTHEAST, 2, 0},
    {6, CONDITION_ALWAYS, 1, PIXELS(160), 0, PIXELS(320), FACING_SOUTHEAST, 2, 0},
    {7, CONDITION_ALWAYS, 1, PIXELS(160), 0, PIXELS(352), FACING_SOUTHEAST, 2, 0},
    {57, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(160), FACING_SOUTHEAST, 2, 0},
    {56, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(192), FACING_SOUTHEAST, 2, 0},
    {59, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(224), FACING_SOUTHEAST, 2, 0},
    {201, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(256), FACING_SOUTHEAST, 2, 0},
    {202, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(288), FACING_SOUTHEAST, 2, 0},
    {64, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(336), FACING_SOUTHEAST, 2, 0},
    {65, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(368), FACING_SOUTHEAST, 2, 0},
    {63, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(400), FACING_SOUTHEAST, 2, 0},
    {SCENE_TABLE_END, 0, 0, 0, 0, 0, 0, 0, 0},
};

struct ActorPlacement gIncreaseItemAndLevel_FourthNamePlacements[] = {
    {205, CONDITION_ALWAYS, 1, PIXELS(128), 0, PIXELS(160), FACING_SOUTHEAST, 2, 0},
    {206, CONDITION_ALWAYS, 1, PIXELS(128), 0, PIXELS(192), FACING_SOUTHEAST, 2, 0},
    {198, CONDITION_ALWAYS, 1, PIXELS(128), 0, PIXELS(224), FACING_SOUTHEAST, 2, 0},
    {121, CONDITION_ALWAYS, 1, PIXELS(128), 0, PIXELS(256), FACING_SOUTHEAST, 2, 0},
    {119, CONDITION_ALWAYS, 1, PIXELS(128), 0, PIXELS(288), FACING_SOUTHEAST, 2, 0},
    {123, CONDITION_ALWAYS, 1, PIXELS(128), 0, PIXELS(320), FACING_SOUTHEAST, 2, 0},
    {104, CONDITION_ALWAYS, 1, PIXELS(176), 0, PIXELS(160), FACING_SOUTHEAST, 2, 0},
    {105, CONDITION_ALWAYS, 1, PIXELS(176), 0, PIXELS(192), FACING_SOUTHEAST, 2, 0},
    {129, CONDITION_ALWAYS, 1, PIXELS(176), 0, PIXELS(224), FACING_SOUTHEAST, 2, 0},
    {130, CONDITION_ALWAYS, 1, PIXELS(176), 0, PIXELS(256), FACING_SOUTHEAST, 2, 0},
    {199, CONDITION_ALWAYS, 1, PIXELS(176), 0, PIXELS(288), FACING_SOUTHEAST, 2, 0},
    {209, CONDITION_ALWAYS, 1, PIXELS(176), 0, PIXELS(320), FACING_SOUTHEAST, 2, 0},
    {108, CONDITION_ALWAYS, 1, PIXELS(224), 0, PIXELS(160), FACING_SOUTHEAST, 2, 0},
    {109, CONDITION_ALWAYS, 1, PIXELS(224), 0, PIXELS(192), FACING_SOUTHEAST, 2, 0},
    {110, CONDITION_ALWAYS, 1, PIXELS(224), 0, PIXELS(224), FACING_SOUTHEAST, 2, 0},
    {111, CONDITION_ALWAYS, 1, PIXELS(224), 0, PIXELS(256), FACING_SOUTHEAST, 2, 0},
    {134, CONDITION_ALWAYS, 1, PIXELS(224), 0, PIXELS(288), FACING_SOUTHEAST, 2, 0},
    {135, CONDITION_ALWAYS, 1, PIXELS(224), 0, PIXELS(320), FACING_SOUTHEAST, 2, 0},
    {SCENE_TABLE_END, 0, 0, 0, 0, 0, 0, 0, 0},
};
// clang-format on

// The control words of the trigger rows: 0 a talk; FORCE_ON_ACTOR Force cast
// at an actor (kind 5, on an actor, ability 142), and with
// PSYNERGY_PHASE_BEFORE_CAST the row run at phase 1, before the cast.
#define TRIGGER_TALK 0
#define FORCE_ON_ACTOR 0x8e15
#define PSYNERGY_PHASE_BEFORE_CAST 0x10000000

/// Talking to each tester runs its tool, as The Broken Seal's Japanese
/// edition has it: Force cast at the portrait browser runs a motion test, and
/// a second row on the same person, run before the cast, sets the effect's
/// child mode, so Force's shot leaves the portrait browser where it stands.
struct Trigger gIncreaseItemAndLevel_Triggers[] = {
    {TRIGGER_TALK, ACTOR_ITEM_BROWSER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_SelectItem)},
    {TRIGGER_TALK, ACTOR_ICON_BROWSER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_OpenIconBrowser)},
    {TRIGGER_TALK, ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_OpenPortraitBrowser)},
    {TRIGGER_TALK, ACTOR_LEVEL_EDITOR, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_RunLevelEditor)},
    {FORCE_ON_ACTOR, ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_RunMotionTest)},
    {PSYNERGY_PHASE_BEFORE_CAST | FORCE_ON_ACTOR, ACTOR_PORTRAIT_BROWSER,
     CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_EnableEffectChildMode)},
    {TRIGGER_TABLE_END},
};

/// The rooms of names' triggers: each party member says their name, and each
/// placed person from actor 8 to 32 theirs. The debug room's two Force rows
/// stay at the end.
struct Trigger gIncreaseItemAndLevel_NameTriggers[] = {
    {TRIGGER_TALK, ISAAC, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowCharacterName)},
    {TRIGGER_TALK, GARET, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowCharacterName)},
    {TRIGGER_TALK, IVAN, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowCharacterName)},
    {TRIGGER_TALK, MIA, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowCharacterName)},
    {TRIGGER_TALK, JENNA, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowCharacterName)},
    {TRIGGER_TALK, SHEBA, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowCharacterName)},
    {TRIGGER_TALK, PIERS, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowCharacterName)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, 32, CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_ShowName)},
    {FORCE_ON_ACTOR, ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_RunMotionTest)},
    {PSYNERGY_PHASE_BEFORE_CAST | FORCE_ON_ACTOR, ACTOR_PORTRAIT_BROWSER,
     CONDITION_ALWAYS, EVENT(IncreaseItemAndLevel_EnableEffectChildMode)},
    {TRIGGER_TABLE_END},
};

/// Unused: requests an exit through a window transition over 24 frames; no
/// trigger row names it.
// XXX: what the event work's words at 428 and 436 are; The Broken Seal's
// FieldWork has transition and transitionFrames there, which a shared
// declaration here would settle.
void IncreaseItemAndLevel_RequestExit(s32 countdown) {
  u8 *event = Ram_HeapSlots->fieldWork;

  *(s32 *)(event + 428) = 513;
  *(s32 *)(event + 436) = 24;
  Import_EventSetExitRequest(countdown);
}

/// The scene's initialize entry: starts the window transition, and in the
/// debug room (entrances up to 89) draws ACTOR_LEVEL_EDITOR 1.7 times its
/// size, as The Broken Seal's Japanese edition does.
s32 IncreaseItemAndLevel_Initialize(void) {
  u8 *event = Ram_HeapSlots->fieldWork;

  *(s32 *)(event + 428) = 516;
  *(s32 *)(event + 436) = 24;
  if (gMainRoutineWork.entrance <= 89) {
    Actor_GetById(ACTOR_LEVEL_EDITOR)->scaleY = 0x1b333;
    Actor_GetById(ACTOR_LEVEL_EDITOR)->scaleX = 0x1b333;
  }
  return 0;
}

/// Runs the animation test, then moves ACTOR_LEVEL_EDITOR to its test
/// position, as The Broken Seal's Japanese edition: Force cast at
/// ACTOR_PORTRAIT_BROWSER gives it mode 2 and ACTOR_LEVEL_EDITOR mode 4; once
/// ACTOR_PORTRAIT_BROWSER's animation has changed, ACTOR_LEVEL_EDITOR moves to
/// (156, 208).
void IncreaseItemAndLevel_RunMotionTest(void) {
  s32 x;
  s32 z;
  s32 id;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetModeById(ACTOR_PORTRAIT_BROWSER, 2);
  Actor_SetModeById(ACTOR_LEVEL_EDITOR, 4);
  Actor_WaitForAnimationChange(ACTOR_PORTRAIT_BROWSER);
  x = 156;
  z = 208;
  id = ACTOR_LEVEL_EDITOR;
  /* FAKEMATCH(barrier): x, z and the actor held at an empty asm. */
  asm("" : "+l"(x), "+l"(z), "+l"(id));
  Actor_SetHorizontalPositionWithTerrain(id, x << 17, z << 17);
  Event_End();
}

/// Keeps Force from moving ACTOR_PORTRAIT_BROWSER: run at phase 1 of Force
/// cast at it, this sets the effect's childMode, and Force's shot then leaves
/// its target where it is.
void IncreaseItemAndLevel_EnableEffectChildMode(void) {
  *((u8 *)Ram_HeapSlots->effectWork + 53) = 1;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Opens the icon browser. Talking to ACTOR_ICON_BROWSER.
void IncreaseItemAndLevel_OpenIconBrowser(void) { DebugBrowser_Icons(); }

/// Opens the portrait browser. Talking to ACTOR_PORTRAIT_BROWSER.
void IncreaseItemAndLevel_OpenPortraitBrowser(void) {
  DebugBrowser_Portraits();
}
