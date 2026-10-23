#include "game_state.h"
#include "unit/debug/level.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
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
#include "object_event.h"
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
// MsgItemPlainName line is drawn as tiles (UiText_DrawCharacterAtOffset)
// rather than into the window's canvas (UiText_DrawResource).
#if LANGUAGE != LANGUAGE_JAPANESE
#define ITEM_DEBUG_DETAILS_WIDTH 28
#define ITEM_DEBUG_DRAW_ITEM_NAME UiText_DrawCharacterAtOffset
#else
#define ITEM_DEBUG_DETAILS_WIDTH 13
#define ITEM_DEBUG_DRAW_ITEM_NAME UiText_DrawResource
#endif

#define ITEM_COUNT 270      ///< The browser's item numbers, 0 to 269, wrapping.
#define ABILITY_COUNT 270   ///< The ability browser's numbers, 0 to 269.
#define BROWSER_ROW_STEP 10 ///< Right and Left move ten numbers.
#define BROWSER_PAGE_STEP 30 ///< R and L move thirty.

/// The room's object events, numbering the rows of
/// gItemLevelObjectEventTemplates; the game names none of them, so each is
/// named for what talking to it opens (gItemLevelEvents), as a render of each
/// talk shows it.
enum ItemLevelLocalId {
  /// ItemLevel_SelectItem.
  /// ★ Named from what the player sees: `Item No:` over an item's name and
  /// what equipping it does, with `A:Get Item  B:Return` (its render)
  LOCALID_ITEM_BROWSER = LOCALID_FIRST_PLACED,
  /// ItemLevel_OpenIconBrowser.
  /// ★ Named from what the player sees: a page of 32 item icons headed
  /// `ITEM PAGE:0` (its render)
  LOCALID_ICON_BROWSER,
  /// ItemLevel_OpenPortraitBrowser; Force cast at it runs the Force tests.
  /// ★ Named from what the player sees: Isaac's portrait over `0 Isaac`, the
  /// first of the portraits Left and Right step through (its render)
  LOCALID_PORTRAIT_BROWSER,
  /// ItemLevel_RunLevelEditor, drawn larger than the others.
  /// ★ Named from what the player sees: `Raise everyone's level.` over the
  /// leader's name and level, A raising it one and Start five (its render)
  LOCALID_LEVEL_EDITOR,
  /// ItemLevel_SelectAbility; placed outside Japanese only, like the two
  /// emote testers after it.
  /// ★ Named from what the player sees: `Psy No:` over an ability's name,
  /// with `B:Return` (its render)
  LOCALID_ABILITY_BROWSER,
  /// ItemLevel_ShowMessageThenEmote.
  /// ★ Named from what the player sees: a wince over the object event, then its
  /// message (its render)
  LOCALID_FIRST_EMOTE_TESTER,
  /// ItemLevel_ShowEmoteThenMessage.
  /// ★ Named from what the player sees: an ellipsis over the first tester,
  /// then the first tester's message (its render)
  LOCALID_SECOND_EMOTE_TESTER
};

u8 *ItemLevel_GetWarpDests(void) { return (u8 *)gItemLevelWarpDests; }

/// The room has no warp regions: its regions entry returns none.
s32 MapData_ReturnZero(void) { return 0; }

u8 *ItemLevel_GetWarps(void) { return (u8 *)gItemLevelWarps; }

u8 *ItemLevel_GetObjectEventTemplates(void) {
  return (u8 *)gItemLevelObjectEventTemplates;
}

#if TBS_PATCH_1
/// Sets the test message before displaying the first emote on the object event
/// spoken to, LOCALID_FIRST_EMOTE_TESTER.
static void ItemLevel_ShowMessageThenEmote(void) {
  Engine_ScriptSetMessage(MsgDebugWontStopYou);
  ObjectEvent_ShowEmote(LOCALID_FIRST_EMOTE_TESTER,
                        EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptShowMessage(LOCALID_FIRST_EMOTE_TESTER, 0);
}

/// Displays the other test emote before setting LOCALID_FIRST_EMOTE_TESTER's
/// message. Talking to
/// LOCALID_SECOND_EMOTE_TESTER, though the emote and the message are
/// LOCALID_FIRST_EMOTE_TESTER's.
static void ItemLevel_ShowEmoteThenMessage(void) {
  ObjectEvent_ShowEmote(LOCALID_FIRST_EMOTE_TESTER,
                        EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Engine_ScriptSetMessage(MsgDebugWontStopYou);
  Engine_ScriptShowMessage(LOCALID_FIRST_EMOTE_TESTER, 0);
}
#endif

u8 *ItemLevel_GetEvents(void) { return (u8 *)gItemLevelEvents; }

/// Raises one owner's level by amount, then recalculates that unit.
static void ItemLevel_AdjustOwnerLevel(s32 ownerId, s32 amount) {
  struct BattleUnit *unit = Unit_Get(ownerId);

  Unit_LevelUpTo(ownerId, unit->level + amount);
  Unit_Calculate(ownerId);
}

/// Applies the level change to the owners returned by the active-party service
/// (FieldParty_ListActiveOwners): every member of the party.
static void ItemLevel_AdjustPartyLevels(s32 amount) {
  u16 ownerIds[16];
  u16 *owner;
  s32 ownerCount;

  ownerCount = FieldParty_ListActiveOwners(ownerIds);
  if (ownerCount > 0) {
    s32 remaining;
    owner = ownerIds;
    remaining = ownerCount;
    do {
      ItemLevel_AdjustOwnerLevel(*owner++, amount);
    } while (--remaining != 0);
  }
}

/// Displays the selected owner's level and changes all active owners.
/// B closes the window and recalculates the original four heroes. Talking to
/// LOCALID_LEVEL_EDITOR: under `Raise everyone's level.` it shows the leader's
/// name and level; Start or Select raises the whole party five levels with
/// fanfare MUS_FANFARE_LARGE, A one level with MUS_FANFARE_SMALL.
static void ItemLevel_RunLevelEditor(void) {
  struct BattleUnit *unit;
  struct UiWindow *window;
  s32 redraw;
  s32 messageId;

  unit = Unit_Get(gGameState.selectedLocalId);
  window = UiWindow_Create(0, 0, 30, 9, 2);

  messageId = MsgDebugRaiseEveryonesLevel;
  UiText_DrawResource(messageId, window, 0, 0);
  UiText_DrawResource(messageId + 1, window, 0, 16);
  messageId += 2;
  redraw = 1;
  UiText_DrawResource(messageId, window, 0, 32);

  for (;;) {
    if (redraw != 0) {
      RenderOutput_RedrawSavedRect(window);
      UiText_DrawStringAtOffset(unit->name, window, 0, 48);
      UiText_DrawStringInWindow(gItemLevelLvLabel, window, 48, 48);
      redraw = 0;
      UiText_DrawNumberInWindow(unit->level, 0, window, 72, 48);
    }

    // Start/Select add five levels; A adds one. Tests remain independent.
    if ((gKeysPressed & KEY_START) != 0 || (gKeysPressed & KEY_SELECT) != 0) {
      ItemLevel_AdjustPartyLevels(5);
      Engine_AudioPlayCue(MUS_FANFARE_LARGE);
      redraw = 1;
    }

    if ((gKeysPressed & KEY_A) != 0) {
      ItemLevel_AdjustPartyLevels(1);
      Engine_AudioPlayCue(MUS_FANFARE_SMALL);
      redraw = 1;
    }

    if ((gKeysPressed & KEY_B) != 0) {
      Engine_AudioPlayCue(SE_MENU_CANCEL);
      RenderOutput_RedrawSavedRect(window);
      Engine_TaskWait(1);
      UiWork_Finalize(window, 1);

      Unit_Calculate(ISAAC);
      Unit_Calculate(GARET);
      Unit_Calculate(MIA);
      Unit_Calculate(IVAN);
      return;
    }

    Engine_TaskWait(1);
  }
}

/// Browses nonempty item definitions and gives the selected item to the party.
/// Talking to LOCALID_ITEM_BROWSER. Up and Down step one number, Right and Left
/// ten, R and L thirty, passing over numbers whose item has no icon; A gives
/// the item to the first member with room (SE_175), and B, or A with every bag
/// full, closes the browser.
static void ItemLevel_SelectItem(void) {
  // Preserve independent key tests and the direction-specific empty-icon scans.
  // Ordinary while scans grow the native function from 0x29c to 0x2a0; retain
  // its current polling edges.
  struct UiWindow *window;
  struct UiWindow *details;
  s32 selectedItemId;
  s32 redraw;
  s32 messageOrDefinitionId;
  s32 entryCount;
  s8 seekDirection;

  entryCount = ITEM_COUNT;
  seekDirection = 0;
  selectedItemId = 1;
  Engine_AudioPlayCue(SE_MENU_CONFIRM);
  redraw = 1;
  window = UiWindow_Create(0, 0, 30, 7, 2);
  details = UiWindow_Create(0, 8, ITEM_DEBUG_DETAILS_WIDTH, 10, 2);
  // Copy the existing sprite palette into the debug window palettes: sprite
  // palette 0 into BG palette 14, then BG palette 15's colour 4 over BG
  // palette 14's colour 14, as the battle background does for BG palette 5.
  // XXX: which layer draws with palette 14 here would say why.
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(14, 0),
          DMA_ENABLE << 16 | 0x10, REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(15, 4), (void *)&BG_PLTT_COLOR(14, 14),
          DMA_ENABLE << 16 | 1, REG_DMA3);
  Engine_TaskWait(1);
  while (1) {
    if (redraw) {
      redraw = 0;
      // Wrap the number into 0 to 269 (a step back from 0 is 269).
      selectedItemId = (selectedItemId + ITEM_COUNT) % entryCount;
      RenderOutput_RedrawSavedRect(window);
      Engine_DebugClearWindow(window);
      UiText_DrawStringInWindow(gItemLevelItemPrompt, window, 0, 0);
      UiText_DrawNumberAtOffset(selectedItemId, 0, window, 80, 0);
      if (PartyInventory_HasSpace()) {
        UiText_DrawStringInWindow(gItemLevelItemHelp, window, 0, 32);
        messageOrDefinitionId = selectedItemId & 0x1ff;
        Engine_DebugGetItem(messageOrDefinitionId);
        UiText_DrawCharacterAtOffset(messageOrDefinitionId + MsgItemName,
                                     window, 120, 0);
        ITEM_DEBUG_DRAW_ITEM_NAME(messageOrDefinitionId + MsgItemPlainName,
                                  window, 0, 16);
        RenderOutput_RedrawSavedRect(details);
        Engine_DebugDrawItemDetails(details, selectedItemId);
      } else {
        UiText_DrawStringInWindow(gItemLevelItemFull, window, 0, 32);
      }
    }
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
      seekDirection = -1;
      selectedItemId--;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_DOWN) {
      seekDirection = 1;
      selectedItemId++;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_RIGHT) {
      seekDirection = 1;
      selectedItemId += BROWSER_ROW_STEP;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_LEFT) {
      seekDirection = -1;
      selectedItemId -= BROWSER_ROW_STEP;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_R) {
      seekDirection = 1;
      selectedItemId += BROWSER_PAGE_STEP;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_L) {
      seekDirection = -1;
      selectedItemId -= BROWSER_PAGE_STEP;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    // Skip definitions with no icon/availability byte in the chosen direction:
    // for an item, its definition's icon.
    if (seekDirection == -1) {
      selectedItemId = (selectedItemId + ITEM_COUNT) % entryCount;
      goto check_back;
    back:
      selectedItemId = (selectedItemId + ITEM_COUNT - 1) % entryCount;
    check_back:
      if (Engine_DebugGetItem(selectedItemId & 0x1ff)->icon == 0)
        goto back;
    }
    if (seekDirection == 1) {
      selectedItemId = (selectedItemId + ITEM_COUNT) % entryCount;
      goto check_ahead;
    ahead:
      selectedItemId = (selectedItemId + ITEM_COUNT + 1) % entryCount;
    check_ahead:
      if (Engine_DebugGetItem(selectedItemId & 0x1ff)->icon == 0)
        goto ahead;
    }
    seekDirection = 0;
    Engine_TaskWait(1);
  }
  RenderOutput_RedrawSavedRect(window);
  Engine_TaskWait(1);
  UiWork_Finalize(window, 1);
  UiWork_Finalize(details, 1);
}

#if TBS_PATCH_1
/// Browses ability names and descriptions, skipping unavailable entries. B
/// closes the sole window created by this browser. Talking to
/// LOCALID_ABILITY_BROWSER in English and German; in French, Spanish and
/// Italian the room keeps the function and the object event but no event, and
/// moves the object event to x 80.
UNUSED static void ItemLevel_SelectAbility(void) {
  // Ordinary while scans grow the native function from 0x248 to 0x260; retain
  // the original polling edges.
  struct UiWindow *window;
#ifndef UBFIX
  struct UiWindow *details;
#endif
  s32 selectedAbilityId;
  s32 redraw;
  s32 messageOrDefinitionId;
  s32 entryCount;
  s8 seekDirection;

  entryCount = ABILITY_COUNT;
  seekDirection = 0;
  selectedAbilityId = 1;
  Engine_AudioPlayCue(SE_MENU_CONFIRM);
  redraw = 1;
  window = UiWindow_Create(0, 0, 30, 12, 2);
  // Copy the existing sprite palette into the debug window palettes.
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(14, 0),
          DMA_ENABLE << 16 | 0x10, REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(15, 4), (void *)&BG_PLTT_COLOR(14, 14),
          DMA_ENABLE << 16 | 1, REG_DMA3);
  Engine_TaskWait(1);
  while (1) {
    if (redraw) {
      redraw = 0;
      selectedAbilityId = (selectedAbilityId + ABILITY_COUNT) % entryCount;
      RenderOutput_RedrawSavedRect(window);
      Engine_DebugClearWindow(window);
      UiText_DrawStringInWindow(gItemLevelPsyPrompt, window, 0, 0);
      UiText_DrawNumberAtOffset(selectedAbilityId, 0, window, 80, 0);
      UiText_DrawStringInWindow(gItemLevelPsyHelp, window, 0, 72);
      messageOrDefinitionId = selectedAbilityId & 0x3fff;
      UiText_DrawCharacterAtOffset(messageOrDefinitionId + MsgAbilityName,
                                   window, 120, 0);
      messageOrDefinitionId += MsgAbilityDescription;
      UiText_DrawCharacterAtOffset(messageOrDefinitionId, window, 0, 24);
      UiText_DrawResource(messageOrDefinitionId, window, 0, 48);
    }
    if (gKeysPressed & KEY_B) {
      Engine_AudioPlayCue(SE_MENU_CANCEL);
      break;
    }
    if (gKeysRepeat & KEY_UP) {
      seekDirection = -1;
      selectedAbilityId--;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_DOWN) {
      seekDirection = 1;
      selectedAbilityId++;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_RIGHT) {
      seekDirection = 1;
      selectedAbilityId += BROWSER_ROW_STEP;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_LEFT) {
      seekDirection = -1;
      selectedAbilityId -= BROWSER_ROW_STEP;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_R) {
      seekDirection = 1;
      selectedAbilityId += BROWSER_PAGE_STEP;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_L) {
      seekDirection = -1;
      selectedAbilityId -= BROWSER_PAGE_STEP;
      redraw = 1;
      Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
    }
    // Skip definitions with no icon/availability byte in the chosen direction:
    // byte 4 of an ability is the icon the menus draw (BattleAct).
    if (seekDirection == -1) {
      selectedAbilityId = (selectedAbilityId + ABILITY_COUNT) % entryCount;
      goto check_back;
    back:
      selectedAbilityId = (selectedAbilityId + ABILITY_COUNT - 1) % entryCount;
    check_back:
      if (Engine_DebugGetAbility(selectedAbilityId & 0x3fff)[4] == 0)
        goto back;
    }
    if (seekDirection == 1) {
      selectedAbilityId = (selectedAbilityId + ABILITY_COUNT) % entryCount;
      goto check_ahead;
    ahead:
      selectedAbilityId = (selectedAbilityId + ABILITY_COUNT + 1) % entryCount;
    check_ahead:
      if (Engine_DebugGetAbility(selectedAbilityId & 0x3fff)[4] == 0)
        goto ahead;
    }
    seekDirection = 0;
    Engine_TaskWait(1);
  }
  RenderOutput_RedrawSavedRect(window);
  Engine_TaskWait(1);
  UiWork_Finalize(window, 1);
#ifndef UBFIX
  // The original never initializes details or creates a second window.
  // Reading this indeterminate pointer is UB; modern builds omit that call.
  UiWork_Finalize(details, 1);
#endif
}
#endif

/// Unused: requests a warp through TRANSITION_WINDOW over 24 frames; no event
/// row names it.
UNUSED static void ItemLevel_RequestWarp(void) {
  gFieldWork->startTransition = MAP_TRANSITION(TRANSITION_WINDOW, 1);
  gFieldWork->transitionFrames = 24;
  Engine_ScriptRequestWarp();
}

/// Starts the window transition and edition-specific object event
/// scale/animation setup. Returns zero through the exported map entry. The
/// room's initialize entry, which Map_Run runs on entry and whose result
/// it ignores: LOCALID_LEVEL_EDITOR is drawn 1.6 times its size (1.7 in
/// Japanese; 16.16 scales), and outside Japanese the two emote testers start
/// animations 5 and 2.
s32 Map_RunLevelRoomSetup(void) {
  gFieldWork->startTransition = MAP_TRANSITION(TRANSITION_WINDOW, 4);
  gFieldWork->transitionFrames = 24;
#if TBS_PATCH_1
  ObjectEvent_GetById(LOCALID_LEVEL_EDITOR)->scaleY = 0x19999;
  ObjectEvent_GetById(LOCALID_LEVEL_EDITOR)->scaleX = 0x19999;
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_EMOTE_TESTER, 5);
  Engine_ObjectEventSetAnimation(LOCALID_SECOND_EMOTE_TESTER, 2);
#else
  ObjectEvent_GetById(LOCALID_LEVEL_EDITOR)->scaleY = 0x1b333;
  ObjectEvent_GetById(LOCALID_LEVEL_EDITOR)->scaleX = 0x1b333;
#endif
  return 0;
}

#if !TBS_PATCH_1
/// Runs the Japanese animation test, then moves LOCALID_LEVEL_EDITOR to its
/// test position. Force cast at LOCALID_PORTRAIT_BROWSER: it takes
/// ANIMATION_WALK and LOCALID_LEVEL_EDITOR ANIMATION_SHAKE_HEAD; once
/// LOCALID_PORTRAIT_BROWSER's animation has changed, LOCALID_LEVEL_EDITOR moves
/// to (312, 416).
static void ItemLevel_RunMotionTest(void) {
  Engine_ScriptStart();
  Engine_ObjectEventSetAnimation(LOCALID_PORTRAIT_BROWSER, ANIMATION_WALK);
  Engine_ObjectEventSetAnimation(LOCALID_LEVEL_EDITOR, ANIMATION_SHAKE_HEAD);
  ObjectMotion_WaitForAnimationChange(LOCALID_PORTRAIT_BROWSER);
  ObjectEvent_SetPosition(LOCALID_LEVEL_EDITOR, PIXELS(312), PIXELS(416));
  Engine_ScriptEnd();
}
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
/// Displays the three character-set caption rows in a debug window. Force cast
/// at LOCALID_PORTRAIT_BROWSER, outside Japanese. The window is left open.
static void ItemLevel_ShowGlyphCaptions(void) {
  struct UiWindow *window;
  /* FAKEMATCH(dead): an unread text buffer for the frame's 32 bytes; as no
   * buffer, 2 of 29 instructions differ in tbs-en. */
  UNUSED u8 text[32];

  window = UiWindow_Create(0, 13, 30, 6, 2);
  UiText_DrawStringInWindow(gItemLevelGlyphsUpper, window, 0, 0);
  UiText_DrawStringInWindow(gItemLevelGlyphsLower, window, 0, 8);
  UiText_DrawStringInWindow(gItemLevelGlyphsMarks, window, 0, 16);
}
#endif

/// Keeps Force from moving LOCALID_PORTRAIT_BROWSER: run at phase 1 of Force
/// cast at it, this sets the effect's childMode, and Force's shot then leaves
/// its target where it is (object/psynergy_effects/force.c).
static void ItemLevel_EnableEffectChildMode(void) {
  struct FieldEffectWork *effectWork =
      (struct FieldEffectWork *)gFieldEffectWork;

  effectWork->childMode = 1;
}

/// Returns the icon browser's status to the event dispatcher. Talking to
/// LOCALID_ICON_BROWSER.
static int ItemLevel_OpenIconBrowser(void) { return DebugMenu_BrowseIcons(); }

/// Returns the portrait browser's status to the event dispatcher. Talking
/// to LOCALID_PORTRAIT_BROWSER.
static int ItemLevel_OpenPortraitBrowser(void) {
  return DebugMenu_BrowsePortraits();
}

char gItemLevelLvLabel[4] = "Lv";

char gItemLevelItemPrompt[12] = "Item No:";

char gItemLevelItemHelp[24] = "A:Get Item  B:Return";

char gItemLevelItemFull[20] = "ITEM FULL........";

#if (LANGUAGE != LANGUAGE_JAPANESE)
char gItemLevelPsyPrompt[8] = "Psy No:";
#endif

#if (LANGUAGE != LANGUAGE_JAPANESE)
char gItemLevelPsyHelp[12] = "B:Return";
#endif

#if (LANGUAGE != LANGUAGE_JAPANESE)
/// The glyph captions' first row; it has no I.
char gItemLevelGlyphsUpper[28] = "A B C D E F G H J K L M N O";
#endif

#if (LANGUAGE != LANGUAGE_JAPANESE)
char gItemLevelGlyphsLower[28] = "P Q R S T U V W X Y Z a b c";
#endif

#if (LANGUAGE != LANGUAGE_JAPANESE)
char gItemLevelGlyphsMarks[12] = "! ? # $ %";
#endif

struct MapWarpDest gItemLevelWarpDests[] = {
    {0, CONDITION_ALWAYS, 824, 0, 864, FACING_NORTH, 0, 280, 504, 912, 896, 0},
    {MAP_TABLE_END},
};

/// No warps: 511 is MAP_WARPS_END's value.
u32 gItemLevelWarps[] = {
    MAP_WARPS_END,
};

/// The room's object events, in the order of enum ItemLevelLocalId: the item,
/// icon and portrait browsers and the level editor; outside Japanese also the
/// ability browser (reachable in English and German only) and the two emote
/// testers.
// clang-format off
struct ObjectEventTemplate gItemLevelObjectEventTemplates[] = {
    {0x1f4, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(792), 0, PIXELS(864), FACING_SOUTHEAST,
        TALK_FACE_PARTY, 0},
    {0x20, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(792), 0, PIXELS(480), FACING_SOUTHEAST,
        TALK_FACE_PARTY, 0},
    {0x21, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(792), 0, PIXELS(504), FACING_SOUTHEAST,
        TALK_FACE_PARTY, 0},
#if !TBS_PATCH_1
    {0x88, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(792), 0, PIXELS(800), FACING_SOUTHEAST,
        TALK_FACE_PARTY, 0},
#elif TBS_PATCH_1 && !TBS_PATCH_3
    {0x88, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(792), 0, PIXELS(800), FACING_EAST, TALK_FACE_PARTY,
        0},
    {0x8f, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(848), 0, PIXELS(864), FACING_SOUTH + FACING_STEP,
        TALK_FACE_PARTY_AND_BACK, 0},
    {0x15, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(848), 0, PIXELS(688), FACING_SOUTH + FACING_STEP,
        TALK_KEEP_FACING, 0},
    {0x15, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(816), 0, PIXELS(688), FACING_SOUTH + FACING_STEP,
        TALK_KEEP_FACING, 0},
#else
    {0x88, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(792), 0, PIXELS(800), FACING_EAST, TALK_FACE_PARTY,
        0},
    {0x8f, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(80), 0, PIXELS(864), FACING_SOUTH + FACING_STEP,
        TALK_FACE_PARTY_AND_BACK, 0},
    {0x15, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(848), 0, PIXELS(688), FACING_SOUTH + FACING_STEP,
        TALK_KEEP_FACING, 0},
    {0x15, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(816), 0, PIXELS(688), FACING_SOUTH + FACING_STEP,
        TALK_KEEP_FACING, 0},
#endif
    {MAP_TABLE_END},
};
// clang-format on

struct MapEvent gItemLevelEvents[] = {
    {MAP_EVENT_TALK, LOCALID_ITEM_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_SelectItem)},
    {MAP_EVENT_TALK, LOCALID_ICON_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_OpenIconBrowser)},
    {MAP_EVENT_TALK, LOCALID_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_OpenPortraitBrowser)},
    {MAP_EVENT_TALK, LOCALID_LEVEL_EDITOR, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_RunLevelEditor)},
#if !TBS_PATCH_1
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE), LOCALID_PORTRAIT_BROWSER,
     CONDITION_ALWAYS, EVENT_SCRIPT(ItemLevel_RunMotionTest)},
#else
#if !TBS_PATCH_3
    {MAP_EVENT_TALK, LOCALID_ABILITY_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_SelectAbility)},
#endif
    {MAP_EVENT_TALK, LOCALID_FIRST_EMOTE_TESTER, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_ShowMessageThenEmote)},
    {MAP_EVENT_TALK, LOCALID_SECOND_EMOTE_TESTER, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_ShowEmoteThenMessage)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE), LOCALID_PORTRAIT_BROWSER,
     CONDITION_ALWAYS, EVENT_SCRIPT(ItemLevel_ShowGlyphCaptions)},
#endif
    {PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST),
     LOCALID_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_EnableEffectChildMode)},
    {MAP_EVENTS_END},
};
