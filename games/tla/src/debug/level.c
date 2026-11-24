/// @file
/// The debug room Increase item and level, as The Broken Seal's: its actors
/// raise the party's levels, give any item and show the game's icons and
/// portraits. Through entrances 96 to 99 the same scene is instead one of four
/// rooms of people in rows, each of whom says their name when talked to.

#include "battle/battle.h"
#include "debug/debug.h"
#include "sounds.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/event.h"
#include "field/scene_tables.h"
#include "system/main_routine_state.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/input.h"
#include "text/msg_ids.h"
#include "game/item.h"
#include "system/dma.h"
#include "battle/battle_unit.h"
#include "game/character_ids.h"
#include "edition.h"
#include "field/field_sequence_event.h"
#include "window/screens.h"
#include "window/window.h"
#include "graphics/text_render.h"
#include "graphics/uiflow.h"
#include "fixed_math.h"
#include "field/field_psynergy_state.h"
#include "field/sprite.h"

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

/// The entrances that open a room of names in place of the debug room: each
/// has its own actor placements and the triggers of
/// gIncreaseItemAndLevel_NameTriggers.
enum {
  ENTRANCE_NAMES_FOURTH = 96,
  ENTRANCE_NAMES_THIRD = 97,
  ENTRANCE_NAMES_SECOND = 98,
  ENTRANCE_NAMES_FIRST = 99
};

/// Entrances up to ENTRANCE_DEBUG_ROOM_LAST open the debug room; those above
/// ENTRANCE_NAMES_ABOVE take the rooms of names' triggers.
enum { ENTRANCE_DEBUG_ROOM_LAST = 89, ENTRANCE_NAMES_ABOVE = 94 };

/// The debug room's actors, numbering the rows of
/// gIncreaseItemAndLevel_ActorPlacements, as The Broken Seal's.
enum IncreaseItemAndLevelActorId {
  ACTOR_ITEM_BROWSER = ACTOR_FIRST_PLACED, ///< IncreaseItemAndLevel_SelectItem
  ACTOR_ICON_BROWSER, ///< IncreaseItemAndLevel_OpenIconBrowser
  /// IncreaseItemAndLevel_OpenPortraitBrowser; Force cast at it runs
  /// IncreaseItemAndLevel_RunMotionTest.
  ACTOR_PORTRAIT_BROWSER,
  /// IncreaseItemAndLevel_RunLevelEditor, drawn larger than the others.
  ACTOR_LEVEL_EDITOR
};

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// The scene's actor placements: a room of names for its entrance, else the
/// debug room's.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineState.entrance == ENTRANCE_NAMES_FIRST)
    return gIncreaseItemAndLevel_FirstNamePlacements;
  if (gMainRoutineState.entrance == ENTRANCE_NAMES_SECOND)
    return gIncreaseItemAndLevel_SecondNamePlacements;
  if (gMainRoutineState.entrance == ENTRANCE_NAMES_THIRD)
    return gIncreaseItemAndLevel_ThirdNamePlacements;
  if (gMainRoutineState.entrance == ENTRANCE_NAMES_FOURTH)
    return gIncreaseItemAndLevel_FourthNamePlacements;
  return gIncreaseItemAndLevel_ActorPlacements;
}

/// How much larger ACTOR_LEVEL_EDITOR is drawn in the debug room.
#define LEVEL_EDITOR_SCALE (FIXED_ONE * 17 / 10)

/// Shows the name of the person talked to in a room of names: actor `actorId`
/// says the message its room's list of names gives it. Each base below is the
/// message ACTOR_FIRST_PLACED lines before its room's first name: "Master Poi"
/// in the first room, "Susa" in the second, "Alex" in the third and "Weapons
/// Shop" in the fourth.
void IncreaseItemAndLevel_ShowName(s32 actorId) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  if (gMainRoutineState.entrance == ENTRANCE_NAMES_FIRST)
    Event_SetMessageId(actorId + (s32)MsgFixingThisWill);
  else if (gMainRoutineState.entrance == ENTRANCE_NAMES_SECOND)
    Event_SetMessageId(actorId + (s32)MsgNamesFortuneTeller);
  else if (gMainRoutineState.entrance == ENTRANCE_NAMES_THIRD)
    Event_SetMessageId(actorId + (s32)MsgNamesConfirmGraphic);
  else
    Event_SetMessageId(actorId + (s32)MsgNamesPiers);
  Event_ShowMessageAndWait(actorId, 0);
  Event_End();
}

/// Shows a party member's name when they are talked to in a room of names, the
/// line of the list from MsgNamesIsaac at their character number, with the
/// character as the message's actor argument. The list runs Isaac, Garet,
/// Ivan, Mia, Jenna, Sheba, Piers with no Felix, so from JENNA on each says
/// the next name: Jenna "Sheba", Sheba "Piers" and Piers "Alex".
void IncreaseItemAndLevel_ShowCharacterName(s32 character) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId(character + (s32)MsgNamesIsaac);
  MessageArgument_Push(character, MESSAGE_ARGUMENT_ACTOR);
  Event_ShowMessageAndWait(character, 0);
  // Garet's name is followed by Isaac saying the list's next line, "Ivan".
  // XXX (no evidence): why; the debug room's events cannot be recorded, and no
  // text or code of either game says.
  if (character == GARET)
    Event_ShowMessageAndWait(ISAAC, 0);
  Event_End();
}

/// The scene's triggers: the rooms of names' for any entrance above
/// ENTRANCE_NAMES_ABOVE, else the debug room's.
const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineState.entrance > ENTRANCE_NAMES_ABOVE)
    return gIncreaseItemAndLevel_NameTriggers;
  return gIncreaseItemAndLevel_Triggers;
}

/// Raises one party member `amount` levels and works out their new stats.
void IncreaseItemAndLevel_AdjustOwnerLevel(s32 ownerId, s32 amount) {
  Unit_LevelUpTo(ownerId, Unit_Get(ownerId)->level + amount);
  Unit_Calculate(ownerId);
}

/// Raises every party member `amount` levels.
void IncreaseItemAndLevel_AdjustPartyLevels(s32 amount) {
  u16 ownerIds[16];
  s32 ownerCount;
  s32 i;

  ownerCount = FieldParty_ListMembers(ownerIds);
  for (i = 0; i < ownerCount; i++)
    IncreaseItemAndLevel_AdjustOwnerLevel(ownerIds[i], amount);
}

/// Displays the leader's level and changes the whole party's, as The Broken
/// Seal's: talking to ACTOR_LEVEL_EDITOR shows `Raise everyone's level.` over
/// the leader's name and level; Start or Select raises the whole party five
/// levels to the Super Lucky jingle, A one level to the Lucky one. B closes
/// the window and works out the stats of Isaac, Garet, Mia and Ivan again.
void IncreaseItemAndLevel_RunLevelEditor(void) {
  struct Unit *unit;
  struct Window *window;
  s32 redraw;
  s32 messageId;

  unit = Unit_Get(gMainRoutineState.leaderActorId);
  window = Window_Create(0, 0, 30, 9, WINDOW_FLAG_INSTANT);

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
        (gInput.pressed & KEY_SELECT) != 0) {
      IncreaseItemAndLevel_AdjustPartyLevels(5);
      Sound_Play(MUS_SUPER_LUCKY);
      redraw = 1;
    }

    if ((gInput.pressed & KEY_A) != 0) {
      IncreaseItemAndLevel_AdjustPartyLevels(1);
      Sound_Play(MUS_LUCKY);
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

/// The browser's item numbers, 0 to 511, wrapping.
#define ITEM_BROWSER_ITEM_COUNT 512

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
  s32 itemId;
  s32 entryCount;
  s8 seekDirection;

  entryCount = ITEM_BROWSER_ITEM_COUNT;
  seekDirection = 0;
  Sound_Play(SE_MENU_CONFIRM);
  selectedItemId = 1;
  redraw = 1;
  window = Window_Create(0, 0, 30, 7, WINDOW_FLAG_INSTANT);
#if LANGUAGE < LANGUAGE_GERMAN
  details = Window_Create(0, 8, 13, 10, WINDOW_FLAG_INSTANT);
#else
  details = Window_Create(0, 9, 18, 10, WINDOW_FLAG_INSTANT);
#endif
  // Lend the highlight palette to the item icons, as The Broken Seal's: sprite
  // palette 0 into it, then the window background colour over its text
  // background.
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
          (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR),
          DMA_COPY_HALFWORDS(1), REG_DMA3);
  wait(1);
  while (1) {
    if (redraw) {
      redraw = 0;
      // Wrap the number into 0 to 511 (a step back from 0 is 511).
      selectedItemId = (selectedItemId + ITEM_BROWSER_ITEM_COUNT) % entryCount;
      WindowSprite_RedrawSavedRect(window);
      WindowSprite_ClearList(window);
      Text_DrawStringInWindow(gIncreaseItemAndLevel_ItemPrompt, window, 0, 0);
      Text_DrawNumberAtOffset(selectedItemId, 0, window, 80, 0);
      if (PartyInventory_HasSpace()) {
        Text_DrawStringInWindow(gIncreaseItemAndLevel_ItemHelp, window, 0, 32);
        itemId = selectedItemId & INVENTORY_ITEM_ID_MASK;
        Item_GetDirect(itemId);
        Text_DrawMessageAsTiles(itemId + (s32)MsgItemName, window, 120, 0);
        Text_DrawMessage(itemId + (s32)MsgItemPlainName, window, 0, 16);
        WindowSprite_RedrawSavedRect(details);
        ItemMenu_DrawItemDetails(details, selectedItemId);
      } else {
        Text_DrawStringInWindow(gIncreaseItemAndLevel_ItemFull, window, 0, 32);
      }
    }
    if (gInput.pressed & KEY_A) {
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
      selectedItemId = (selectedItemId + ITEM_BROWSER_ITEM_COUNT) % entryCount;
      while (Item_GetDirect(selectedItemId & INVENTORY_ITEM_ID_MASK)->icon == 0)
        selectedItemId =
            (selectedItemId + ITEM_BROWSER_ITEM_COUNT - 1) % entryCount;
    }
    if (seekDirection == 1) {
      selectedItemId = (selectedItemId + ITEM_BROWSER_ITEM_COUNT) % entryCount;
      while (Item_GetDirect(selectedItemId & INVENTORY_ITEM_ID_MASK)->icon == 0)
        selectedItemId =
            (selectedItemId + ITEM_BROWSER_ITEM_COUNT + 1) % entryCount;
    }
    seekDirection = 0;
    wait(1);
  }
  WindowSprite_RedrawSavedRect(window);
  wait(1);
  Window_Close(window, 1);
  Window_Close(details, 1);
}

/// The level editor's label before a member's level.
char gIncreaseItemAndLevel_LvLabel[4] = "Lv";

/// The item selector's prompt, before the item number.
char gIncreaseItemAndLevel_ItemPrompt[12] = "Item No:";

/// The item selector's help line, while the bags have room.
char gIncreaseItemAndLevel_ItemHelp[24] = "A:Get Item  B:Return";

/// The item selector's line when the party's bags are full.
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

/// The room has no exits of its own.
u32 gSceneExits[] = {
    SCENE_EXITS_END,
};

/// The debug room's actors, in the order of enum IncreaseItemAndLevelActorId.
struct ActorPlacement gIncreaseItemAndLevel_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    48, 0, 272, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    48, 0, 72, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUN_DISC_STATUE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 48, 0, 136, FACING_SOUTHEAST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0,
                    272, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The first room of names, from Master Poi.
struct ActorPlacement gIncreaseItemAndLevel_FirstNamePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_POI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0,
                    160, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RIKI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0,
                    192, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TAVI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0,
                    224, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    160, 0, 256, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    160, 0, 288, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALHAFRAS_MAYOR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 160, 0, 320, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FORTUNE_TELLER, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 160, 0, 352, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_AKAFUBU, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208,
                    0, 160, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0,
                    192, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAHA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0,
                    224, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRIGGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208,
                    0, 256, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHAUCHA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208,
                    0, 288, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EOLEO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0,
                    320, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OBABA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0,
                    352, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The second room of names.
struct ActorPlacement gIncreaseItemAndLevel_SecondNamePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUSA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 256, 0,
                    160, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_UZUME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    256, 0, 192, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KUSHINADA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    256, 0, 224, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_HYDROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 256,
                    0, 256, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CONSERVATO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    256, 0, 288, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KARST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 256, 0,
                    320, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_AGATIO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 256,
                    0, 352, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNAS_FATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    208, 0, 160, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNAS_MOTHER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    208, 0, 192, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KYLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0,
                    224, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LUNPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0,
                    256, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUELLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208,
                    0, 288, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MASKED_SHAMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    208, 0, 320, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    208, 0, 352, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The third room of names, from Alex.
struct ActorPlacement gIncreaseItemAndLevel_ThirdNamePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0,
                    160, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0,
                    192, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0,
                    224, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0,
                    256, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0,
                    288, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0,
                    320, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0,
                    352, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0,
                    160, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208,
                    0, 192, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_HAMMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0,
                    224, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208,
                    0, 256, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 208, 0, 288, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TEAL_SHIRT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 208, 0, 336, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_HEADSCARF, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 208, 0, 368, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FEIZHI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208,
                    0, 400, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The fourth room of names.
struct ActorPlacement gIncreaseItemAndLevel_FourthNamePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 128, 0, 160, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 128, 0, 192, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 128, 0, 224, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_CREAM, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 128, 0, 256, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_IN_RED, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 128, 0, 288, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_ORANGE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 128, 0, 320, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176,
                    0, 160, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_BEARD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 176, 0, 192, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_LONG_BLONDE_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 176, 0, 224, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 176, 0, 256, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 176, 0, 288, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 176, 0, 320, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 224, 0, 160, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 224, 0, 192, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 224, 0, 224, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ARTIFACT_MERCHANT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 224, 0, 256, FACING_SOUTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_INNKEEPER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    224, 0, 288, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    224, 0, 320, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

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
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_RunMotionTest)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_EnableEffectChildMode)},
    {TRIGGER_TABLE_END},
};

/// The rooms of names' triggers: each party member says their name, and each
/// of the first 25 people placed theirs. The debug room's two Force rows stay
/// at the end.
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
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 0, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 1, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 2, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 3, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 4, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 5, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 6, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 7, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 8, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 9, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 10, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 11, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 12, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 13, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 14, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 15, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 16, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 17, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 18, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 19, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 20, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 21, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 22, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 23, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {TRIGGER_TALK, ACTOR_FIRST_PLACED + 24, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowName)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_RunMotionTest)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_EnableEffectChildMode)},
    {TRIGGER_TABLE_END},
};

/// Unused: requests an exit through a window transition over 24 frames; no
/// trigger row names it.
void IncreaseItemAndLevel_RequestExit(s32 countdown) {
  struct FieldState *event = Ram_HeapSlots->fieldState;

  event->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  event->transitionFrames = 24;
  Import_EventSetExitRequest(countdown);
}

/// Opens the room through the window transition, and in the debug room draws
/// ACTOR_LEVEL_EDITOR 1.7 times its size, as The Broken Seal's Japanese
/// edition does.
s32 Scene_Initialize(void) {
  struct FieldState *event = Ram_HeapSlots->fieldState;

  event->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  event->transitionFrames = 24;
  if (gMainRoutineState.entrance <= ENTRANCE_DEBUG_ROOM_LAST) {
    Actor_GetById(ACTOR_LEVEL_EDITOR)->scaleY = LEVEL_EDITOR_SCALE;
    Actor_GetById(ACTOR_LEVEL_EDITOR)->scaleX = LEVEL_EDITOR_SCALE;
  }
  return 0;
}

/// Runs the animation test, then moves ACTOR_LEVEL_EDITOR to its test
/// position, as The Broken Seal's Japanese edition: Force cast at
/// ACTOR_PORTRAIT_BROWSER walks it and ACTOR_LEVEL_EDITOR shakes its head; once
/// ACTOR_PORTRAIT_BROWSER's animation has changed, ACTOR_LEVEL_EDITOR moves to
/// (312, 416).
void IncreaseItemAndLevel_RunMotionTest(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_SetAnimationById(ACTOR_PORTRAIT_BROWSER, ANIM_WALK);
  Actor_SetAnimationById(ACTOR_LEVEL_EDITOR, ANIM_SHAKE_HEAD);
  Actor_WaitForAnimationChange(ACTOR_PORTRAIT_BROWSER);
  Actor_SetPosition(ACTOR_LEVEL_EDITOR, 312, 416);
  Event_End();
}

/// Keeps Force from moving ACTOR_PORTRAIT_BROWSER: run just before Force is
/// cast at it, this sets the effect's childMode, and Force's shot then leaves
/// its target where it is.
void IncreaseItemAndLevel_EnableEffectChildMode(void) {
  struct FieldEffectState *state = Ram_HeapSlots->fieldEffectState;

  state->childMode = 1;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Talking to ACTOR_ICON_BROWSER: pages of item, Psynergy and status icons.
void IncreaseItemAndLevel_OpenIconBrowser(void) { DebugBrowser_Icons(); }

/// Talking to ACTOR_PORTRAIT_BROWSER: the portraits, one at a time.
void IncreaseItemAndLevel_OpenPortraitBrowser(void) {
  DebugBrowser_Portraits();
}
