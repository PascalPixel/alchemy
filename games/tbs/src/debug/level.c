#include "save_block.h"
#include "unit/debug/level.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
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
#include "level.h"
#include "field_service.h"
#include "actor.h"
#include "party_state.h"
#include "types.h"
#include "io_reg.h"
#include "inventory.h"
#include "dma.h"
#include "text_render_runtime.h"
#include "item.h"
#include "edition.h"
#include "uiflow.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "map_tables.h"
#include "fx_work.h"
#include "songs.h"

// Edition-specific text and window layout values: outside Japanese the item
// browser's details window is 28 tiles wide rather than 13, and its
// MsgItemPlainName line is drawn as tiles (Text_DrawMessageAsTiles)
// rather than into the window's canvas (Text_DrawMessage).
#if LANGUAGE != LANGUAGE_JAPANESE
#define ITEM_DEBUG_DETAILS_WIDTH 28
#define ITEM_DEBUG_DRAW_ITEM_NAME Text_DrawMessageAsTiles
#else
#define ITEM_DEBUG_DETAILS_WIDTH 13
#define ITEM_DEBUG_DRAW_ITEM_NAME Text_DrawMessage
#endif

#define ITEM_COUNT 270      ///< The browser's item numbers, 0 to 269, wrapping.
#define ABILITY_COUNT 270   ///< The ability browser's numbers, 0 to 269.
#define BROWSER_ROW_STEP 10 ///< Right and Left move ten numbers.
#define BROWSER_PAGE_STEP 30 ///< R and L move thirty.

/// The room's actors, numbering the rows of
/// gIncreaseItemAndLevel_ActorTemplates; the game names none of them, so
/// each is named for what talking to it opens (gIncreaseItemAndLevel_Events),
/// as a render of each talk shows it.
enum IncreaseItemAndLevelActorId {
  /// IncreaseItemAndLevel_SelectItem.
  /// ★ Named from what the player sees: `Item No:` over an item's name and
  /// what equipping it does, with `A:Get Item  B:Return` (its render)
  ACTOR_ITEM_BROWSER = ACTOR_FIRST_PLACED,
  /// IncreaseItemAndLevel_OpenIconBrowser.
  /// ★ Named from what the player sees: a page of 32 item icons headed
  /// `ITEM PAGE:0` (its render)
  ACTOR_ICON_BROWSER,
  /// IncreaseItemAndLevel_OpenPortraitBrowser; Force cast at it runs the Force
  /// tests.
  /// ★ Named from what the player sees: Isaac's portrait over `0 Isaac`, the
  /// first of the portraits Left and Right step through (its render)
  ACTOR_PORTRAIT_BROWSER,
  /// IncreaseItemAndLevel_RunLevelEditor, drawn larger than the others.
  /// ★ Named from what the player sees: `Raise everyone's level.` over the
  /// leader's name and level, A raising it one and Start five (its render)
  ACTOR_LEVEL_EDITOR,
  /// IncreaseItemAndLevel_SelectAbility; placed outside Japanese only, like the
  /// two
  /// emote testers after it.
  /// ★ Named from what the player sees: `Psy No:` over an ability's name,
  /// with `B:Return` (its render)
  ACTOR_ABILITY_BROWSER,
  /// IncreaseItemAndLevel_ShowMessageThenEmote.
  /// ★ Named from what the player sees: a wince over the actor, then its
  /// message (its render)
  ACTOR_FIRST_EMOTE_TESTER,
  /// IncreaseItemAndLevel_ShowEmoteThenMessage.
  /// ★ Named from what the player sees: an ellipsis over the first tester,
  /// then the first tester's message (its render)
  ACTOR_SECOND_EMOTE_TESTER
};

u8 *IncreaseItemAndLevel_GetEntrances(void) {
  return (u8 *)gIncreaseItemAndLevel_Entrances;
}

/// The room has no exit regions: its regions entry returns none.
s32 Scene_GetExitRegions(void) { return 0; }

u8 *IncreaseItemAndLevel_GetExits(void) {
  return (u8 *)gIncreaseItemAndLevel_Exits;
}

u8 *IncreaseItemAndLevel_GetActorTemplates(void) {
  return (u8 *)gIncreaseItemAndLevel_ActorTemplates;
}

#if TBS_PATCH_1
/// Sets the test message before displaying the first emote on the actor
/// spoken to, ACTOR_FIRST_EMOTE_TESTER.
static void IncreaseItemAndLevel_ShowMessageThenEmote(void) {
  FieldScript_SetMessageId(MsgDebugWontStopYou);
  Actor_ShowEmote(ACTOR_FIRST_EMOTE_TESTER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_ShowMessageAndWait(ACTOR_FIRST_EMOTE_TESTER, 0);
}

/// Displays the other test emote before setting ACTOR_FIRST_EMOTE_TESTER's
/// message. Talking to
/// ACTOR_SECOND_EMOTE_TESTER, though the emote and the message are
/// ACTOR_FIRST_EMOTE_TESTER's.
static void IncreaseItemAndLevel_ShowEmoteThenMessage(void) {
  Actor_ShowEmote(ACTOR_FIRST_EMOTE_TESTER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  FieldScript_SetMessageId(MsgDebugWontStopYou);
  FieldScript_ShowMessageAndWait(ACTOR_FIRST_EMOTE_TESTER, 0);
}
#endif

u8 *IncreaseItemAndLevel_GetEvents(void) {
  return (u8 *)gIncreaseItemAndLevel_Events;
}

/// Raises one member's level by amount, then recalculates that unit.
static void IncreaseItemAndLevel_AdjustOwnerLevel(s32 ownerId, s32 amount) {
  struct Unit *unit = Unit_Get(ownerId);

  Unit_LevelUpTo(ownerId, unit->level + amount);
  Unit_Calculate(ownerId);
}

/// Applies the level change to the members returned by the active-party service
/// (FieldParty_ListMembers): every member of the party.
static void IncreaseItemAndLevel_AdjustPartyLevels(s32 amount) {
  u16 ownerIds[16];
  u16 *owner;
  s32 ownerCount;

  ownerCount = FieldParty_ListMembers(ownerIds);
  if (ownerCount > 0) {
    s32 remaining;
    owner = ownerIds;
    remaining = ownerCount;
    do {
      IncreaseItemAndLevel_AdjustOwnerLevel(*owner++, amount);
    } while (--remaining != 0);
  }
}

/// Displays the selected member's level and changes the whole party.
/// B closes the window and recalculates the original four heroes. Talking to
/// ACTOR_LEVEL_EDITOR: under `Raise everyone's level.` it shows the leader's
/// name and level; Start or Select raises the whole party five levels with
/// fanfare MUS_FANFARE_LARGE, A one level with MUS_FANFARE_SMALL.
static void IncreaseItemAndLevel_RunLevelEditor(void) {
  struct Unit *unit;
  struct Window *window;
  s32 redraw;
  s32 messageId;

  unit = Unit_Get(gSaveBlock.leaderActorId);
  window = Window_Create(0, 0, 30, 9, WINDOW_FLAG_INSTANT);

  messageId = MsgDebugRaiseEveryonesLevel;
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

    // Start/Select add five levels; A adds one. Tests remain independent.
    if ((gKeysPressed & KEY_START) != 0 || (gKeysPressed & KEY_SELECT) != 0) {
      IncreaseItemAndLevel_AdjustPartyLevels(5);
      Sound_PlaySong(MUS_FANFARE_LARGE);
      redraw = 1;
    }

    if ((gKeysPressed & KEY_A) != 0) {
      IncreaseItemAndLevel_AdjustPartyLevels(1);
      Sound_PlaySong(MUS_FANFARE_SMALL);
      redraw = 1;
    }

    if ((gKeysPressed & KEY_B) != 0) {
      Sound_PlaySong(SE_MENU_CANCEL);
      WindowSprite_RedrawSavedRect(window);
      WaitFrames(1);
      Window_Close(window, 1);

      Unit_Calculate(ISAAC);
      Unit_Calculate(GARET);
      Unit_Calculate(MIA);
      Unit_Calculate(IVAN);
      return;
    }

    WaitFrames(1);
  }
}

/// Browses nonempty item definitions and gives the selected item to the party.
/// Talking to ACTOR_ITEM_BROWSER. Up and Down step one number, Right and Left
/// ten, R and L thirty, passing over numbers whose item has no icon; A gives
/// the item to the first member with room (SE_NOTE_HIGH), and B, or A with
/// every bag full, closes the browser.
static void IncreaseItemAndLevel_SelectItem(void) {
  // Preserve independent key tests and the direction-specific empty-icon scans.
  // Ordinary while scans grow the native function from 0x29c to 0x2a0; retain
  // its current polling edges.
  struct Window *window;
  struct Window *details;
  s32 selectedItemId;
  s32 redraw;
  s32 messageOrDefinitionId;
  s32 entryCount;
  s8 seekDirection;

  entryCount = ITEM_COUNT;
  seekDirection = 0;
  selectedItemId = 1;
  Sound_PlaySong(SE_MENU_CONFIRM);
  redraw = 1;
  window = Window_Create(0, 0, 30, 7, WINDOW_FLAG_INSTANT);
  details =
      Window_Create(0, 8, ITEM_DEBUG_DETAILS_WIDTH, 10, WINDOW_FLAG_INSTANT);
  // Lend HIGHLIGHT_PALETTE to the item icons, as
  // Palette_LoadIconsIntoHighlight does: sprite palette 0 into it, then BG
  // WINDOW_BACKGROUND_COLOR over its TEXT_BACKGROUND_COLOR.
  // ItemMenu_DrawItemDetails draws the details window's element icons in
  // HIGHLIGHT_PALETTE.
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
          DMA_ENABLE << 16 | 0x10, REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
          (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR),
          DMA_ENABLE << 16 | 1, REG_DMA3);
  WaitFrames(1);
  while (1) {
    if (redraw) {
      redraw = 0;
      // Wrap the number into 0 to 269 (a step back from 0 is 269).
      selectedItemId = (selectedItemId + ITEM_COUNT) % entryCount;
      WindowSprite_RedrawSavedRect(window);
      WindowSprite_ClearList(window);
      Text_DrawStringInWindow(gIncreaseItemAndLevel_ItemPrompt, window, 0, 0);
      Text_DrawNumberAtOffset(selectedItemId, 0, window, 80, 0);
      if (PartyInventory_HasSpace()) {
        Text_DrawStringInWindow(gIncreaseItemAndLevel_ItemHelp, window, 0, 32);
        messageOrDefinitionId = selectedItemId & 0x1ff;
        Item_GetDirect(messageOrDefinitionId);
        Text_DrawMessageAsTiles(messageOrDefinitionId + MsgItemName, window,
                                120, 0);
        ITEM_DEBUG_DRAW_ITEM_NAME(messageOrDefinitionId + MsgItemPlainName,
                                  window, 0, 16);
        WindowSprite_RedrawSavedRect(details);
        ItemMenu_DrawItemDetails(details, selectedItemId);
      } else {
        Text_DrawStringInWindow(gIncreaseItemAndLevel_ItemFull, window, 0, 32);
      }
    }
    if (gKeysPressed & KEY_A) {
      if (PartyInventory_Add(selectedItemId) == -1) {
        Sound_PlaySong(SE_MENU_CANCEL);
        break;
      }
      Sound_PlaySong(SE_NOTE_HIGH);
    }
    if (gKeysPressed & KEY_B) {
      Sound_PlaySong(SE_MENU_CANCEL);
      break;
    }
    if (gKeysRepeat & KEY_UP) {
      seekDirection = -1;
      selectedItemId--;
      redraw = 1;
      Sound_PlaySong(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_DOWN) {
      seekDirection = 1;
      selectedItemId++;
      redraw = 1;
      Sound_PlaySong(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_RIGHT) {
      seekDirection = 1;
      selectedItemId += BROWSER_ROW_STEP;
      redraw = 1;
      Sound_PlaySong(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_LEFT) {
      seekDirection = -1;
      selectedItemId -= BROWSER_ROW_STEP;
      redraw = 1;
      Sound_PlaySong(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_R) {
      seekDirection = 1;
      selectedItemId += BROWSER_PAGE_STEP;
      redraw = 1;
      Sound_PlaySong(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_L) {
      seekDirection = -1;
      selectedItemId -= BROWSER_PAGE_STEP;
      redraw = 1;
      Sound_PlaySong(SE_MENU_CURSOR_MOVE);
    }
    // Skip definitions with no icon/availability byte in the chosen direction:
    // for an item, its definition's icon.
    if (seekDirection == -1) {
      selectedItemId = (selectedItemId + ITEM_COUNT) % entryCount;
      goto check_back;
    back:
      selectedItemId = (selectedItemId + ITEM_COUNT - 1) % entryCount;
    check_back:
      if (Item_GetDirect(selectedItemId & 0x1ff)->icon == 0)
        goto back;
    }
    if (seekDirection == 1) {
      selectedItemId = (selectedItemId + ITEM_COUNT) % entryCount;
      goto check_ahead;
    ahead:
      selectedItemId = (selectedItemId + ITEM_COUNT + 1) % entryCount;
    check_ahead:
      if (Item_GetDirect(selectedItemId & 0x1ff)->icon == 0)
        goto ahead;
    }
    seekDirection = 0;
    WaitFrames(1);
  }
  WindowSprite_RedrawSavedRect(window);
  WaitFrames(1);
  Window_Close(window, 1);
  Window_Close(details, 1);
}

#if TBS_PATCH_1
/// Browses ability names and descriptions, skipping unavailable entries. B
/// closes the sole window created by this browser. Talking to
/// ACTOR_ABILITY_BROWSER in English and German; in French, Spanish and
/// Italian the room keeps the function and the actor but no event, and
/// moves the actor to x 80.
UNUSED static void IncreaseItemAndLevel_SelectAbility(void) {
  // Ordinary while scans grow the native function from 0x248 to 0x260; retain
  // the original polling edges.
  struct Window *window;
#ifndef UBFIX
  struct Window *details;
#endif
  s32 selectedAbilityId;
  s32 redraw;
  s32 messageOrDefinitionId;
  s32 entryCount;
  s8 seekDirection;

  entryCount = ABILITY_COUNT;
  seekDirection = 0;
  selectedAbilityId = 1;
  Sound_PlaySong(SE_MENU_CONFIRM);
  redraw = 1;
  window = Window_Create(0, 0, 30, 12, WINDOW_FLAG_INSTANT);
  // Copy the existing sprite palette into the debug window palettes.
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
          DMA_ENABLE << 16 | 0x10, REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
          (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR),
          DMA_ENABLE << 16 | 1, REG_DMA3);
  WaitFrames(1);
  while (1) {
    if (redraw) {
      redraw = 0;
      selectedAbilityId = (selectedAbilityId + ABILITY_COUNT) % entryCount;
      WindowSprite_RedrawSavedRect(window);
      WindowSprite_ClearList(window);
      Text_DrawStringInWindow(gIncreaseItemAndLevel_PsyPrompt, window, 0, 0);
      Text_DrawNumberAtOffset(selectedAbilityId, 0, window, 80, 0);
      Text_DrawStringInWindow(gIncreaseItemAndLevel_PsyHelp, window, 0, 72);
      messageOrDefinitionId = selectedAbilityId & 0x3fff;
      Text_DrawMessageAsTiles(messageOrDefinitionId + MsgAbilityName, window,
                              120, 0);
      messageOrDefinitionId += MsgAbilityDescription;
      Text_DrawMessageAsTiles(messageOrDefinitionId, window, 0, 24);
      Text_DrawMessage(messageOrDefinitionId, window, 0, 48);
    }
    if (gKeysPressed & KEY_B) {
      Sound_PlaySong(SE_MENU_CANCEL);
      break;
    }
    if (gKeysRepeat & KEY_UP) {
      seekDirection = -1;
      selectedAbilityId--;
      redraw = 1;
      Sound_PlaySong(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_DOWN) {
      seekDirection = 1;
      selectedAbilityId++;
      redraw = 1;
      Sound_PlaySong(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_RIGHT) {
      seekDirection = 1;
      selectedAbilityId += BROWSER_ROW_STEP;
      redraw = 1;
      Sound_PlaySong(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_LEFT) {
      seekDirection = -1;
      selectedAbilityId -= BROWSER_ROW_STEP;
      redraw = 1;
      Sound_PlaySong(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_R) {
      seekDirection = 1;
      selectedAbilityId += BROWSER_PAGE_STEP;
      redraw = 1;
      Sound_PlaySong(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_L) {
      seekDirection = -1;
      selectedAbilityId -= BROWSER_PAGE_STEP;
      redraw = 1;
      Sound_PlaySong(SE_MENU_CURSOR_MOVE);
    }
    // Skip definitions with no icon/availability byte in the chosen direction:
    // byte 4 of an ability is the icon the menus draw (AbilityDefinition).
    if (seekDirection == -1) {
      selectedAbilityId = (selectedAbilityId + ABILITY_COUNT) % entryCount;
      goto check_back;
    back:
      selectedAbilityId = (selectedAbilityId + ABILITY_COUNT - 1) % entryCount;
    check_back:
      if (Ability_GetDirect(selectedAbilityId & 0x3fff)->icon == 0)
        goto back;
    }
    if (seekDirection == 1) {
      selectedAbilityId = (selectedAbilityId + ABILITY_COUNT) % entryCount;
      goto check_ahead;
    ahead:
      selectedAbilityId = (selectedAbilityId + ABILITY_COUNT + 1) % entryCount;
    check_ahead:
      if (Ability_GetDirect(selectedAbilityId & 0x3fff)->icon == 0)
        goto ahead;
    }
    seekDirection = 0;
    WaitFrames(1);
  }
  WindowSprite_RedrawSavedRect(window);
  WaitFrames(1);
  Window_Close(window, 1);
#ifndef UBFIX
  // The original never initializes details or creates a second window.
  // Reading this indeterminate pointer is UB; modern builds omit that call.
  Window_Close(details, 1);
#endif
}
#endif

/// Unused: requests an exit through TRANSITION_WINDOW over 24 frames; no event
/// row names it.
UNUSED static void IncreaseItemAndLevel_RequestExit(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  gFieldWork->transitionFrames = 24;
  Import_ScriptRequestExit();
}

/// Starts the window transition and edition-specific actor
/// scale/animation setup. Returns zero through the exported map entry. The
/// room's initialize entry, which SceneManager_Run runs on entry and whose
/// result it ignores: ACTOR_LEVEL_EDITOR is drawn 1.6 times its size (1.7 in
/// Japanese; 16.16 scales), and outside Japanese the two emote testers start
/// animations 5 and 2.
s32 IncreaseItemAndLevel_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  gFieldWork->transitionFrames = 24;
#if TBS_PATCH_1
  Actor_GetById(ACTOR_LEVEL_EDITOR)->scaleY = 0x19999;
  Actor_GetById(ACTOR_LEVEL_EDITOR)->scaleX = 0x19999;
  Import_ActorSetAnimation(ACTOR_FIRST_EMOTE_TESTER, 5);
  Import_ActorSetAnimation(ACTOR_SECOND_EMOTE_TESTER, 2);
#else
  Actor_GetById(ACTOR_LEVEL_EDITOR)->scaleY = 0x1b333;
  Actor_GetById(ACTOR_LEVEL_EDITOR)->scaleX = 0x1b333;
#endif
  return 0;
}

#if !TBS_PATCH_1
/// Runs the Japanese animation test, then moves ACTOR_LEVEL_EDITOR to its
/// test position. Force cast at ACTOR_PORTRAIT_BROWSER: it takes
/// ANIM_WALK and ACTOR_LEVEL_EDITOR ANIM_SHAKE_HEAD; once
/// ACTOR_PORTRAIT_BROWSER's animation has changed, ACTOR_LEVEL_EDITOR moves
/// to (312, 416).
static void IncreaseItemAndLevel_RunMotionTest(void) {
  FieldScript_Start();
  Import_ActorSetAnimation(ACTOR_PORTRAIT_BROWSER, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_LEVEL_EDITOR, ANIM_SHAKE_HEAD);
  ObjectMotion_WaitForAnimationChange(ACTOR_PORTRAIT_BROWSER);
  Actor_SetPosition(ACTOR_LEVEL_EDITOR, PIXELS(312), PIXELS(416));
  FieldScript_End();
}
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
/// Displays the three character-set caption rows in a debug window. Force cast
/// at ACTOR_PORTRAIT_BROWSER, outside Japanese. The window is left open.
static void IncreaseItemAndLevel_ShowGlyphCaptions(void) {
  struct Window *window;
  /* FAKEMATCH(dead): an unread text buffer for the frame's 32 bytes; as no
   * buffer, 2 of 29 instructions differ in tbs-en. */
  UNUSED u8 text[32];

  window = Window_Create(0, 13, 30, 6, WINDOW_FLAG_INSTANT);
  Text_DrawStringInWindow(gIncreaseItemAndLevel_GlyphsUpper, window, 0, 0);
  Text_DrawStringInWindow(gIncreaseItemAndLevel_GlyphsLower, window, 0, 8);
  Text_DrawStringInWindow(gIncreaseItemAndLevel_GlyphsMarks, window, 0, 16);
}
#endif

/// Keeps Force from moving ACTOR_PORTRAIT_BROWSER: run at phase 1 of Force
/// cast at it, this sets the effect's childMode, and Force's shot then leaves
/// its target where it is (overworld/field_services/force.c).
static void IncreaseItemAndLevel_EnableEffectChildMode(void) {
  struct FieldEffectWork *effectWork =
      (struct FieldEffectWork *)gFieldEffectWork;

  effectWork->childMode = 1;
}

/// Returns the icon browser's status to the event dispatcher. Talking to
/// ACTOR_ICON_BROWSER.
static int IncreaseItemAndLevel_OpenIconBrowser(void) {
  return DebugBrowser_Icons();
}

/// Returns the portrait browser's status to the event dispatcher. Talking
/// to ACTOR_PORTRAIT_BROWSER.
static int IncreaseItemAndLevel_OpenPortraitBrowser(void) {
  return DebugBrowser_Portraits();
}

char gIncreaseItemAndLevel_LvLabel[4] = "Lv";

char gIncreaseItemAndLevel_ItemPrompt[12] = "Item No:";

char gIncreaseItemAndLevel_ItemHelp[24] = "A:Get Item  B:Return";

char gIncreaseItemAndLevel_ItemFull[20] = "ITEM FULL........";

#if (LANGUAGE != LANGUAGE_JAPANESE)
char gIncreaseItemAndLevel_PsyPrompt[8] = "Psy No:";
#endif

#if (LANGUAGE != LANGUAGE_JAPANESE)
char gIncreaseItemAndLevel_PsyHelp[12] = "B:Return";
#endif

#if (LANGUAGE != LANGUAGE_JAPANESE)
/// The glyph captions' first row; it has no I.
char gIncreaseItemAndLevel_GlyphsUpper[28] = "A B C D E F G H J K L M N O";
#endif

#if (LANGUAGE != LANGUAGE_JAPANESE)
char gIncreaseItemAndLevel_GlyphsLower[28] = "P Q R S T U V W X Y Z a b c";
#endif

#if (LANGUAGE != LANGUAGE_JAPANESE)
char gIncreaseItemAndLevel_GlyphsMarks[12] = "! ? # $ %";
#endif

struct SceneEntrance gIncreaseItemAndLevel_Entrances[] = {
    {0, CONDITION_ALWAYS, 824, 0, 864, FACING_NORTH, 0, 280, 504, 912, 896, 0},
    {MAP_TABLE_END},
};

/// No exits: 511 is SCENE_EXITS_END's value.
u32 gIncreaseItemAndLevel_Exits[] = {
    SCENE_EXITS_END,
};

/// The room's actors, in the order of enum IncreaseItemAndLevelActorId:
/// the item, icon and portrait browsers and the level editor; outside Japanese
/// also the ability browser (reachable in English and German only) and the two
/// emote testers.
// clang-format off
struct ActorTemplate gIncreaseItemAndLevel_ActorTemplates[] = {
    {0x1f4, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(792), 0, PIXELS(864), FACING_SOUTHEAST,
        TALK_FACING_PARTY, 0},
    {0x20, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(792), 0, PIXELS(480), FACING_SOUTHEAST,
        TALK_FACING_PARTY, 0},
    {0x21, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(792), 0, PIXELS(504), FACING_SOUTHEAST,
        TALK_FACING_PARTY, 0},
#if !TBS_PATCH_1
    {0x88, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(792), 0, PIXELS(800), FACING_SOUTHEAST,
        TALK_FACING_PARTY, 0},
#elif TBS_PATCH_1 && !TBS_PATCH_3
    {0x88, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(792), 0, PIXELS(800), FACING_EAST, TALK_FACING_PARTY,
        0},
    {0x8f, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(848), 0, PIXELS(864), FACING_SOUTH + FACING_STEP,
        TALK_FACING_PARTY_AND_BACK, 0},
    {0x15, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(848), 0, PIXELS(688), FACING_SOUTH + FACING_STEP,
        TALK_FACING_KEEP, 0},
    {0x15, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(816), 0, PIXELS(688), FACING_SOUTH + FACING_STEP,
        TALK_FACING_KEEP, 0},
#else
    {0x88, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(792), 0, PIXELS(800), FACING_EAST, TALK_FACING_PARTY,
        0},
    {0x8f, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(80), 0, PIXELS(864), FACING_SOUTH + FACING_STEP,
        TALK_FACING_PARTY_AND_BACK, 0},
    {0x15, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(848), 0, PIXELS(688), FACING_SOUTH + FACING_STEP,
        TALK_FACING_KEEP, 0},
    {0x15, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(816), 0, PIXELS(688), FACING_SOUTH + FACING_STEP,
        TALK_FACING_KEEP, 0},
#endif
    {MAP_TABLE_END},
};
// clang-format on

/// Talking to each tester runs its tool. The ability browser has a talk row
/// only in English and German (TBS_PATCH_1 without TBS_PATCH_3); French,
/// Spanish and Italian keep its code and its actor. Force cast at the
/// portrait browser runs a motion test in Japanese and shows the three
/// character-set caption rows elsewhere; in every edition a second row on the
/// same person, run before the cast (PSYNERGY_PHASE_BEFORE_CAST), sets the
/// effect's child mode, so Force's shot leaves the portrait browser where it
/// stands.
struct MapEvent gIncreaseItemAndLevel_Events[] = {
    {MAP_EVENT_TALK, ACTOR_ITEM_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(IncreaseItemAndLevel_SelectItem)},
    {MAP_EVENT_TALK, ACTOR_ICON_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(IncreaseItemAndLevel_OpenIconBrowser)},
    {MAP_EVENT_TALK, ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(IncreaseItemAndLevel_OpenPortraitBrowser)},
    {MAP_EVENT_TALK, ACTOR_LEVEL_EDITOR, CONDITION_ALWAYS,
     EVENT_SCRIPT(IncreaseItemAndLevel_RunLevelEditor)},
#if !TBS_PATCH_1
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(IncreaseItemAndLevel_RunMotionTest)},
#else
#if !TBS_PATCH_3
    {MAP_EVENT_TALK, ACTOR_ABILITY_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(IncreaseItemAndLevel_SelectAbility)},
#endif
    {MAP_EVENT_TALK, ACTOR_FIRST_EMOTE_TESTER, CONDITION_ALWAYS,
     EVENT_SCRIPT(IncreaseItemAndLevel_ShowMessageThenEmote)},
    {MAP_EVENT_TALK, ACTOR_SECOND_EMOTE_TESTER, CONDITION_ALWAYS,
     EVENT_SCRIPT(IncreaseItemAndLevel_ShowEmoteThenMessage)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(IncreaseItemAndLevel_ShowGlyphCaptions)},
#endif
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(IncreaseItemAndLevel_EnableEffectChildMode)},
    {MAP_EVENTS_END},
};
