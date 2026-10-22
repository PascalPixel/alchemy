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
#include "field_obj.h"
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
#include "field_scene.h"
#include "fx_scene.h"
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

/// The room's actors, numbering the rows of gItemLevelPlacements; the game
/// names none of them, so each is named for what talking to it does
/// (gItemLevelEvents).
enum ItemLevelActor {
  ACTOR_ITEM_BROWSER = ACTOR_FIRST_PLACED, ///< ItemLevel_SelectItem
  ACTOR_ICON_BROWSER,                      ///< ItemLevel_OpenIconBrowser
  /// ItemLevel_OpenGlyphBrowser; Force cast at it runs the Force tests.
  ACTOR_GLYPH_BROWSER,
  /// ItemLevel_RunLevelEditor, drawn larger than the others.
  ACTOR_LEVEL_EDITOR,
  /// ItemLevel_SelectAbility; placed outside Japanese only, like the two
  /// emote testers after it.
  ACTOR_ABILITY_BROWSER,
  ACTOR_FIRST_EMOTE_TESTER, ///< ItemLevel_ShowMessageThenEmote
  ACTOR_SECOND_EMOTE_TESTER ///< ItemLevel_ShowEmoteThenMessage
};

u8 *ItemLevel_GetEntrances(void) { return (u8 *)gItemLevelEntrances; }

/// The room has no exit regions: its regions entry returns none.
s32 SceneData_ReturnZero(void) { return 0; }

u8 *ItemLevel_GetExits(void) { return (u8 *)gItemLevelExits; }

u8 *ItemLevel_GetPlacements(void) { return (u8 *)gItemLevelPlacements; }

#if TBS_PATCH_1
/// Sets the test message before displaying the first emote on the actor
/// spoken to, ACTOR_FIRST_EMOTE_TESTER.
static void ItemLevel_ShowMessageThenEmote(void) {
  Engine_EventSetMessage(MsgDebugWontStopYou);
  Actor_ShowEmote(ACTOR_FIRST_EMOTE_TESTER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_EventShowMessage(ACTOR_FIRST_EMOTE_TESTER, 0);
}

/// Displays the other test emote before setting ACTOR_FIRST_EMOTE_TESTER's
/// message. Talking to
/// ACTOR_SECOND_EMOTE_TESTER, though the emote and the message are
/// ACTOR_FIRST_EMOTE_TESTER's.
static void ItemLevel_ShowEmoteThenMessage(void) {
  Actor_ShowEmote(ACTOR_FIRST_EMOTE_TESTER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Engine_EventSetMessage(MsgDebugWontStopYou);
  Engine_EventShowMessage(ACTOR_FIRST_EMOTE_TESTER, 0);
}
#endif

u8 *ItemLevel_GetEvents(void) { return (u8 *)gItemLevelEvents; }

/// Raises one owner's level by amount, then recalculates that unit.
static void ItemLevel_AdjustOwnerLevel(s32 ownerId, s32 amount) {
  struct BattleUnit *unit = UnitGet(ownerId);

  Unit_LevelUpTo(ownerId, unit->level + amount);
  UnitCalc(ownerId);
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
/// ACTOR_LEVEL_EDITOR: under `Raise everyone's level.` it shows the leader's
/// name and level; Start or Select raises the whole party five levels with
/// fanfare MUS_093, A one level with MUS_091.
static void ItemLevel_RunLevelEditor(void) {
  struct BattleUnit *unit;
  struct UiWindow *window;
  s32 redraw;
  s32 messageId;

  unit = UnitGet(gGameState.selectedActor);
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
      Engine_AudioPlayCue(MUS_093);
      redraw = 1;
    }

    if ((gKeysPressed & KEY_A) != 0) {
      ItemLevel_AdjustPartyLevels(1);
      Engine_AudioPlayCue(MUS_091);
      redraw = 1;
    }

    if ((gKeysPressed & KEY_B) != 0) {
      Engine_AudioPlayCue(SE_MENU_CANCEL);
      RenderOutput_RedrawSavedRect(window);
      Engine_TaskWait(1);
      UiWork_Finalize(window, 1);

      UnitCalc(ISAAC);
      UnitCalc(GARET);
      UnitCalc(MIA);
      UnitCalc(IVAN);
      return;
    }

    Engine_TaskWait(1);
  }
}

/// Browses nonempty item definitions and gives the selected item to the party.
/// Talking to ACTOR_ITEM_BROWSER. Up and Down step one number, Right and Left
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
/// ACTOR_ABILITY_BROWSER in English and German; in French, Spanish and Italian
/// the room keeps the function and the actor but no event, and moves the actor
/// to x 80.
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

/// Unused: requests an exit through TRANSITION_WINDOW over 24 frames; no event
/// row names it.
UNUSED static void ItemLevel_RequestExit(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  gEventWork->transitionFrames = 24;
  Engine_EventRequestExit();
}

/// Starts the window transition and edition-specific actor scale/animation
/// setup. Returns zero through the exported scene entry. The room's
/// initialize entry, which Field_RunScene runs on entry and whose result it
/// ignores: ACTOR_LEVEL_EDITOR is drawn 1.6 times its size (1.7 in Japanese;
/// 16.16 scales), and outside Japanese the two emote testers start animations
/// 5 and 2.
s32 FieldScene_RunEntrySetup(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  gEventWork->transitionFrames = 24;
#if TBS_PATCH_1
  Object_GetById(ACTOR_LEVEL_EDITOR)->scaleY = 0x19999;
  Object_GetById(ACTOR_LEVEL_EDITOR)->scaleX = 0x19999;
  Engine_ActorSetAnimation(ACTOR_FIRST_EMOTE_TESTER, 5);
  Engine_ActorSetAnimation(ACTOR_SECOND_EMOTE_TESTER, 2);
#else
  Object_GetById(ACTOR_LEVEL_EDITOR)->scaleY = 0x1b333;
  Object_GetById(ACTOR_LEVEL_EDITOR)->scaleX = 0x1b333;
#endif
  return 0;
}

#if !TBS_PATCH_1
/// Runs the Japanese animation test, then moves ACTOR_LEVEL_EDITOR to its test
/// position. Force cast at ACTOR_GLYPH_BROWSER: it takes ANIM_WALK and
/// ACTOR_LEVEL_EDITOR ANIM_SHAKE_HEAD; once
/// ACTOR_GLYPH_BROWSER's animation has changed, ACTOR_LEVEL_EDITOR moves to
/// (312, 416).
static void ItemLevel_RunMotionTest(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_GLYPH_BROWSER, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_LEVEL_EDITOR, ANIM_SHAKE_HEAD);
  ObjectMotion_WaitForAnimationChange(ACTOR_GLYPH_BROWSER);
  Actor_SetPosition(ACTOR_LEVEL_EDITOR, PIXELS(312), PIXELS(416));
  Engine_EventEnd();
}
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
/// Displays the three character-set caption rows in a debug window. Force cast
/// at ACTOR_GLYPH_BROWSER, outside Japanese. The window is left open.
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

/// Keeps Force from moving ACTOR_GLYPH_BROWSER: run at phase 1 of Force cast
/// at it, this sets the effect's childMode, and Force's shot then leaves its
/// target where it is (object/psynergy_effects/force.c).
static void ItemLevel_EnableEffectChildMode(void) {
  struct FieldFxScene *effect = (struct FieldFxScene *)gEffectWork;

  effect->childMode = 1;
}

/// Returns the icon browser's status to the event dispatcher. Talking to
/// ACTOR_ICON_BROWSER.
static int ItemLevel_OpenIconBrowser(void) { return DebugMenu_BrowseIcons(); }

/// Returns the entry-glyph browser's status to the event dispatcher. Talking
/// to ACTOR_GLYPH_BROWSER.
static int ItemLevel_OpenGlyphBrowser(void) {
  return DebugMenu_BrowseEntryGlyphs();
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

struct SceneEntrance gItemLevelEntrances[] = {
    {0, CONDITION_ALWAYS, 824, 0, 864, FACING_NORTH, 0, 280, 504, 912, 896, 0},
    {SCENE_TABLE_END},
};

/// No exits: 511 is SCENE_EXITS_END's value.
u32 gItemLevelExits[] = {
    SCENE_EXITS_END,
};

/// The room's actors, in the order of enum ItemLevelActor: the item, icon and
/// glyph browsers and the level editor; outside Japanese also the ability
/// browser (reachable in English and German only) and the two emote testers.
// clang-format off
struct ScenePlacement gItemLevelPlacements[] = {
    {0x1f4, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(792), 0, PIXELS(864), FACING_SOUTHEAST,
        TALK_FACE_PARTY, 0},
    {0x20, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(792), 0, PIXELS(480), FACING_SOUTHEAST,
        TALK_FACE_PARTY, 0},
    {0x21, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(792), 0, PIXELS(504), FACING_SOUTHEAST,
        TALK_FACE_PARTY, 0},
#if !TBS_PATCH_1
    {0x88, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(792), 0, PIXELS(800), FACING_SOUTHEAST,
        TALK_FACE_PARTY, 0},
#elif TBS_PATCH_1 && !TBS_PATCH_3
    {0x88, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(792), 0, PIXELS(800), FACING_EAST, TALK_FACE_PARTY,
        0},
    {0x8f, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(848), 0, PIXELS(864), FACING_SOUTH + FACING_STEP,
        TALK_FACE_PARTY_AND_BACK, 0},
    {0x15, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(848), 0, PIXELS(688), FACING_SOUTH + FACING_STEP,
        TALK_KEEP_FACING, 0},
    {0x15, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(816), 0, PIXELS(688), FACING_SOUTH + FACING_STEP,
        TALK_KEEP_FACING, 0},
#else
    {0x88, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(792), 0, PIXELS(800), FACING_EAST, TALK_FACE_PARTY,
        0},
    {0x8f, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(80), 0, PIXELS(864), FACING_SOUTH + FACING_STEP,
        TALK_FACE_PARTY_AND_BACK, 0},
    {0x15, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(848), 0, PIXELS(688), FACING_SOUTH + FACING_STEP,
        TALK_KEEP_FACING, 0},
    {0x15, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(816), 0, PIXELS(688), FACING_SOUTH + FACING_STEP,
        TALK_KEEP_FACING, 0},
#endif
    {SCENE_TABLE_END},
};
// clang-format on

struct SceneEvent gItemLevelEvents[] = {
    {EVENT_TALK, ACTOR_ITEM_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_SelectItem)},
    {EVENT_TALK, ACTOR_ICON_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_OpenIconBrowser)},
    {EVENT_TALK, ACTOR_GLYPH_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_OpenGlyphBrowser)},
    {EVENT_TALK, ACTOR_LEVEL_EDITOR, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_RunLevelEditor)},
#if !TBS_PATCH_1
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_GLYPH_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_RunMotionTest)},
#else
#if !TBS_PATCH_3
    {EVENT_TALK, ACTOR_ABILITY_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_SelectAbility)},
#endif
    {EVENT_TALK, ACTOR_FIRST_EMOTE_TESTER, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_ShowMessageThenEmote)},
    {EVENT_TALK, ACTOR_SECOND_EMOTE_TESTER, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_ShowEmoteThenMessage)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_GLYPH_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_ShowGlyphCaptions)},
#endif
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_GLYPH_BROWSER, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_EnableEffectChildMode)},
    {SCENE_EVENTS_END},
};
