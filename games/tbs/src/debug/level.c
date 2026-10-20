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
#include "field_scene.h"
#include "fx_scene.h"
#include "songs.h"

// Edition-specific text and window layout values: outside Japanese the item
// browser's details window is 28 tiles wide rather than 13, and its
// MsgItemPlainName line is drawn as tiles (UiText_DrawCharacterAtOffset)
// rather than into the window's canvas (UiText_DrawResource).
#if TBS_PATCH_1
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

u8 *ItemLevel_GetEntrances(void) { return (u8 *)gItemLevelEntrances; }

/// The room has no exit regions: its regions entry returns none.
s32 SceneData_ReturnZero(void) { return 0; }

u8 *ItemLevel_GetExits(void) { return (u8 *)gItemLevelExits; }

u8 *ItemLevel_GetPlacements(void) { return (u8 *)gItemLevelPlacements; }

#if TBS_PATCH_1
/// Sets the test message before displaying the first emote on actor 13.
/// Talking to actor 13. 0x102 is bubble animation 2 in front of the actor
/// (EMOTE_IN_FRONT); it stays a literal because what bubble animation 2 shows
/// is not known.
static void ItemLevel_ShowMessageThenEmote(void) {
  Engine_EventSetMessage(MsgDebugWontStopYou);
  Actor_ShowEmote(13, 0x102, 0);
  Engine_EventShowMessage(13, 0);
}

/// Displays the other test emote before setting actor 13's message: bubble
/// animation 5 in front (0x105). Talking to actor 14, though the emote and
/// the message are actor 13's.
static void ItemLevel_ShowEmoteThenMessage(void) {
  Actor_ShowEmote(13, 0x105, 0);
  Engine_EventSetMessage(MsgDebugWontStopYou);
  Engine_EventShowMessage(13, 0);
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
/// actor 11: under `Raise everyone's level.` it shows the leader's name and
/// level; Start or Select raises the whole party five levels with fanfare
/// MUS_093, A one level with MUS_091.
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
/// Talking to actor 8. Up and Down step one number, Right and Left ten, R and
/// L thirty, passing over numbers whose item has no icon; A gives the item to
/// the first member with room (SE_175), and B, or A with every bag full,
/// closes the browser.
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
/// closes the sole window created by this browser. Talking to actor 12 in
/// English and German; in French, Spanish and Italian the room keeps the
/// function and the actor but no event, and moves the actor to x 80.
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

/// Requests a window-style exit with variant one over 24 frames.
UNUSED static void ItemLevel_RequestExit(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  gEventWork->transitionFrames = 24;
  Engine_EventRequestExit();
}

/// Starts the window transition and edition-specific actor scale/animation
/// setup. Returns zero through the exported scene entry. The room's
/// initialize entry, which Field_RunScene runs on entry and whose result it
/// ignores: the level editor, actor 11, is drawn 1.6 times its size (1.7 in
/// Japanese; 16.16 scales), and outside Japanese actors 13 and 14 start
/// animations 5 and 2.
s32 FieldScene_RunEntrySetup(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  gEventWork->transitionFrames = 24;
#if TBS_PATCH_1
  Object_GetById(11)->scaleY = 0x19999;
  Object_GetById(11)->scaleX = 0x19999;
  Engine_ActorSetAnimation(13, 5);
  Engine_ActorSetAnimation(14, 2);
#else
  Object_GetById(11)->scaleY = 0x1b333;
  Object_GetById(11)->scaleX = 0x1b333;
#endif
  return 0;
}

#if !TBS_PATCH_1
/// Runs the Japanese animation test, then moves actor 11 to its test position.
/// Force cast at actor 10: actor 10 takes animation 2 (ANIM_WALK) and actor
/// 11 animation 4 (ANIM_SHAKE_HEAD); once actor 10's animation has changed,
/// actor 11 moves to (312, 416).
static void ItemLevel_RunMotionTest(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(10, 2);
  Engine_ActorSetAnimation(11, 4);
  ObjectMotion_WaitForAnimationChange(10);
  Actor_SetPosition(11, PIXELS(312), PIXELS(416));
  Engine_EventEnd();
}
#endif

#if TBS_PATCH_1
/// Displays the three character-set caption rows in a debug window. Force cast
/// at actor 10, outside Japanese. The window is left open.
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

/// Keeps Force from moving actor 10: run at phase 1 of Force cast at it, this
/// sets the effect's childMode, and Force's shot then leaves its target where
/// it is (object/psynergy_effects/force.c).
static void ItemLevel_EnableEffectChildMode(void) {
  struct BattleFxScene *effect = (struct BattleFxScene *)gEffectWork;

  effect->childMode = 1;
}

/// Returns the icon browser's status to the event dispatcher. Talking to
/// actor 9.
static int ItemLevel_OpenIconBrowser(void) { return DebugMenu_BrowseIcons(); }

/// Returns the entry-glyph browser's status to the event dispatcher. Talking
/// to actor 10.
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
    SCENE_EXITS(511),
};

/// The room's actors, numbered from 8 in this order: 8 the item browser, 9
/// the icon browser, 10 the glyph browser and the Force tests, 11 the level
/// editor; outside Japanese 12 the ability browser (English and German only)
/// and 13 and 14 the emote tests (gItemLevelEvents).
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
    {EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(ItemLevel_SelectItem)},
    {EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(ItemLevel_OpenIconBrowser)},
    {EVENT_TALK, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_OpenGlyphBrowser)},
    {EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ItemLevel_RunLevelEditor)},
#if !TBS_PATCH_1
    {PSYNERGY_ON_ACTOR(0x8e), 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_RunMotionTest)},
#else
#if !TBS_PATCH_3
    {EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(ItemLevel_SelectAbility)},
#endif
    {EVENT_TALK, 13, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_ShowMessageThenEmote)},
    {EVENT_TALK, 14, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_ShowEmoteThenMessage)},
    {PSYNERGY_ON_ACTOR(0x8e), 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_ShowGlyphCaptions)},
#endif
    {0x10008e15, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(ItemLevel_EnableEffectChildMode)},
    {SCENE_EVENTS_END},
};
