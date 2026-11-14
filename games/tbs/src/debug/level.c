/// @file
/// The debug room Increase item and level, whose actors raise the party's
/// levels, give any item and show the game's icons and portraits.

#include "ability_ids.h"
#include "edition.h"
#include "types.h"
#include "battle/battle.h"
#include "battle/battle_action_run.h"
#include "battle/playback_queue.h"
#include "field/event_inline.h"
#include "field/field_psynergy_work.h"
#include "field/field_work.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "gamepak/far_call.h"
#include "system/dma.h"
#include "system/heap_slots.h"
#include "system/input.h"
#include "system/system.h"
#include "window/window.h"
#include "text/msg_ids.h"

u8 *Scene_GetEntrances(void);
u8 *Scene_GetExits(void);
u8 *Scene_GetActorPlacements(void);
u8 *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern char gIncreaseItemAndLevel_LvLabel[4];
extern char gIncreaseItemAndLevel_ItemPrompt[12];
extern char gIncreaseItemAndLevel_ItemHelp[24];
extern char gIncreaseItemAndLevel_ItemFull[20];
extern char gIncreaseItemAndLevel_PsyPrompt[8];
extern char gIncreaseItemAndLevel_PsyHelp[12];
extern char gIncreaseItemAndLevel_GlyphsUpper[28];
extern char gIncreaseItemAndLevel_GlyphsLower[28];
extern char gIncreaseItemAndLevel_GlyphsMarks[12];
extern struct SceneEntrance gIncreaseItemAndLevel_Entrances[];
extern u32 gIncreaseItemAndLevel_Exits[];
extern struct ActorPlacement gIncreaseItemAndLevel_ActorPlacements[];
extern struct Trigger gIncreaseItemAndLevel_Triggers[];

void Import_ActorSetPosition(u32 actorId, s32 x, s32 z);

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

// ACTOR_LEVEL_EDITOR's scale in 16.16, which the international editions
// changed (Scene_Initialize).
#if TBS_PATCH_1
#define LEVEL_EDITOR_SCALE (Q16_ONE * 8 / 5)
#else
#define LEVEL_EDITOR_SCALE (Q16_ONE * 17 / 10)
#endif

/// The scene's actors, numbering the rows of
/// gIncreaseItemAndLevel_ActorPlacements; the game names none of them, so
/// each is named for what talking to it opens (gIncreaseItemAndLevel_Triggers).
enum IncreaseItemAndLevelActorId {
  /// IncreaseItemAndLevel_SelectItem: `Item No:` over an item's name and what
  /// equipping it does, with `A:Get Item  B:Return`.
  ACTOR_ITEM_BROWSER = ACTOR_FIRST_PLACED,
  /// IncreaseItemAndLevel_OpenIconBrowser: pages of 32 item icons headed
  /// `ITEM PAGE:0`.
  ACTOR_ICON_BROWSER,
  /// IncreaseItemAndLevel_OpenPortraitBrowser: the portraits, Isaac's first,
  /// stepped through with Left and Right; Force cast at it runs the Force
  /// tests.
  ACTOR_PORTRAIT_BROWSER,
  /// IncreaseItemAndLevel_RunLevelEditor: `Raise everyone's level.` over the
  /// leader's name and level, A raising it one and Start five.
  ACTOR_LEVEL_EDITOR,
  /// IncreaseItemAndLevel_SelectAbility: `Psy No:` over an ability's name, with
  /// `B:Return`.
  ACTOR_ABILITY_BROWSER,
  /// IncreaseItemAndLevel_ShowMessageThenEmote: its message, then a wince.
  ACTOR_FIRST_EMOTE_TESTER,
  /// IncreaseItemAndLevel_ShowEmoteThenMessage: an ellipsis over the first
  /// tester, then its message.
  ACTOR_SECOND_EMOTE_TESTER
};

u8 *Scene_GetEntrances(void) { return (u8 *)gIncreaseItemAndLevel_Entrances; }

/// The scene has no exit regions: its regions entry returns none.
s32 Scene_GetExitRegions(void) { return 0; }

u8 *Scene_GetExits(void) { return (u8 *)gIncreaseItemAndLevel_Exits; }

u8 *Scene_GetActorPlacements(void) {
  return (u8 *)gIncreaseItemAndLevel_ActorPlacements;
}

#if TBS_PATCH_1
/// Sets the test message before displaying the first emote on the actor
/// spoken to, ACTOR_FIRST_EMOTE_TESTER.
static void IncreaseItemAndLevel_ShowMessageThenEmote(void) {
  Event_SetMessageId(MsgDebugWontStopYou);
  Actor_ShowEmote(ACTOR_FIRST_EMOTE_TESTER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_ShowMessageAndWait(ACTOR_FIRST_EMOTE_TESTER, 0);
}

/// Displays the other test emote before setting ACTOR_FIRST_EMOTE_TESTER's
/// message.
static void IncreaseItemAndLevel_ShowEmoteThenMessage(void) {
  Actor_ShowEmote(ACTOR_FIRST_EMOTE_TESTER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_SetMessageId(MsgDebugWontStopYou);
  Event_ShowMessageAndWait(ACTOR_FIRST_EMOTE_TESTER, 0);
}
#endif

u8 *Scene_GetTriggers(void) { return (u8 *)gIncreaseItemAndLevel_Triggers; }

/// Raises one party member's level by amount, then recalculates that unit.
static void IncreaseItemAndLevel_AdjustOwnerLevel(s32 ownerId, s32 amount) {
  struct Unit *unit = Unit_Get(ownerId);

  Unit_LevelUpTo(ownerId, unit->level + amount);
  Unit_Calculate(ownerId);
}

/// Changes every party member's level by amount.
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

/// Displays the selected party member's level and changes the whole party. B
/// closes the window and recalculates the original four heroes.
static void IncreaseItemAndLevel_RunLevelEditor(void) {
  struct Unit *unit;
  struct Window *window;
  s32 redraw;
  s32 messageId;

  unit = Unit_Get(gMainRoutineWork.leaderActorId);
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

    // Start or Select adds five levels, A one.
    if ((gKeysPressed & KEY_START) != 0 || (gKeysPressed & KEY_SELECT) != 0) {
      IncreaseItemAndLevel_AdjustPartyLevels(5);
      Sound_Play(MUS_JINGLE_LARGE);
      redraw = 1;
    }

    if ((gKeysPressed & KEY_A) != 0) {
      IncreaseItemAndLevel_AdjustPartyLevels(1);
      Sound_Play(MUS_JINGLE_SMALL);
      redraw = 1;
    }

    if ((gKeysPressed & KEY_B) != 0) {
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

/// Browses nonempty item definitions and gives the selected item to the party.
/// Talking to ACTOR_ITEM_BROWSER.
static void IncreaseItemAndLevel_SelectItem(void) {
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
  Sound_Play(SE_MENU_CONFIRM);
  redraw = 1;
  window = Window_Create(0, 0, 30, 7, WINDOW_FLAG_INSTANT);
  details =
      Window_Create(0, 8, ITEM_DEBUG_DETAILS_WIDTH, 10, WINDOW_FLAG_INSTANT);
  // Lend HIGHLIGHT_PALETTE to the item icons, as Palette_LoadIconsIntoHighlight
  // does: sprite palette 0 into it, then BG WINDOW_BACKGROUND_COLOR over its
  // TEXT_BACKGROUND_COLOR.
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
          (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR),
          DMA_COPY_HALFWORDS(1), REG_DMA3);
  wait(1);
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
        messageOrDefinitionId = selectedItemId & INVENTORY_ITEM_ID_MASK;
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
      seekDirection = -1;
      selectedItemId--;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_DOWN) {
      seekDirection = 1;
      selectedItemId++;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_RIGHT) {
      seekDirection = 1;
      selectedItemId += BROWSER_ROW_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_LEFT) {
      seekDirection = -1;
      selectedItemId -= BROWSER_ROW_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_R) {
      seekDirection = 1;
      selectedItemId += BROWSER_PAGE_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_L) {
      seekDirection = -1;
      selectedItemId -= BROWSER_PAGE_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    // Step on past numbers with no item, in the direction moved.
    if (seekDirection == -1) {
      selectedItemId = (selectedItemId + ITEM_COUNT) % entryCount;
      goto check_back;
    back:
      selectedItemId = (selectedItemId + ITEM_COUNT - 1) % entryCount;
    check_back:
      if (Item_GetDirect(selectedItemId & INVENTORY_ITEM_ID_MASK)->icon == 0)
        goto back;
    }
    if (seekDirection == 1) {
      selectedItemId = (selectedItemId + ITEM_COUNT) % entryCount;
      goto check_ahead;
    ahead:
      selectedItemId = (selectedItemId + ITEM_COUNT + 1) % entryCount;
    check_ahead:
      if (Item_GetDirect(selectedItemId & INVENTORY_ITEM_ID_MASK)->icon == 0)
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

#if TBS_PATCH_1
/// Browses ability names and descriptions, skipping unavailable entries. B
/// closes the sole window created by this browser.
UNUSED static void IncreaseItemAndLevel_SelectAbility(void) {
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
  Sound_Play(SE_MENU_CONFIRM);
  redraw = 1;
  window = Window_Create(0, 0, 30, 12, WINDOW_FLAG_INSTANT);
  // Lend HIGHLIGHT_PALETTE to the icons, as the item browser does.
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
          (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR),
          DMA_COPY_HALFWORDS(1), REG_DMA3);
  wait(1);
  while (1) {
    if (redraw) {
      redraw = 0;
      selectedAbilityId = (selectedAbilityId + ABILITY_COUNT) % entryCount;
      WindowSprite_RedrawSavedRect(window);
      WindowSprite_ClearList(window);
      Text_DrawStringInWindow(gIncreaseItemAndLevel_PsyPrompt, window, 0, 0);
      Text_DrawNumberAtOffset(selectedAbilityId, 0, window, 80, 0);
      Text_DrawStringInWindow(gIncreaseItemAndLevel_PsyHelp, window, 0, 72);
      messageOrDefinitionId = selectedAbilityId & ACTION_ID_MASK;
      Text_DrawMessageAsTiles(messageOrDefinitionId + MsgAbilityName, window,
                              120, 0);
      messageOrDefinitionId += MsgAbilityDescription;
      Text_DrawMessageAsTiles(messageOrDefinitionId, window, 0, 24);
      Text_DrawMessage(messageOrDefinitionId, window, 0, 48);
    }
    if (gKeysPressed & KEY_B) {
      Sound_Play(SE_MENU_CANCEL);
      break;
    }
    if (gKeysRepeat & KEY_UP) {
      seekDirection = -1;
      selectedAbilityId--;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_DOWN) {
      seekDirection = 1;
      selectedAbilityId++;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_RIGHT) {
      seekDirection = 1;
      selectedAbilityId += BROWSER_ROW_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_LEFT) {
      seekDirection = -1;
      selectedAbilityId -= BROWSER_ROW_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_R) {
      seekDirection = 1;
      selectedAbilityId += BROWSER_PAGE_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_L) {
      seekDirection = -1;
      selectedAbilityId -= BROWSER_PAGE_STEP;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    // Step on past numbers with no ability, in the direction moved.
    if (seekDirection == -1) {
      selectedAbilityId = (selectedAbilityId + ABILITY_COUNT) % entryCount;
      goto check_back;
    back:
      selectedAbilityId = (selectedAbilityId + ABILITY_COUNT - 1) % entryCount;
    check_back:
      if (Ability_GetDirect(selectedAbilityId & ACTION_ID_MASK)->icon == 0)
        goto back;
    }
    if (seekDirection == 1) {
      selectedAbilityId = (selectedAbilityId + ABILITY_COUNT) % entryCount;
      goto check_ahead;
    ahead:
      selectedAbilityId = (selectedAbilityId + ABILITY_COUNT + 1) % entryCount;
    check_ahead:
      if (Ability_GetDirect(selectedAbilityId & ACTION_ID_MASK)->icon == 0)
        goto ahead;
    }
    seekDirection = 0;
    wait(1);
  }
  WindowSprite_RedrawSavedRect(window);
  wait(1);
  Window_Close(window, 1);
#ifndef UBFIX
  // UB: details is never created, but the browser closes it.
  Window_Close(details, 1);
#endif
}
#endif

/// Unused: requests an exit through TRANSITION_WINDOW over 24 frames; no event
/// row names it.
UNUSED static void IncreaseItemAndLevel_RequestExit(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  gFieldWork->transitionFrames = 24;
  Import_EventSetExitRequest();
}

/// Sets up the scene: the window transition in, and the level editor's size.
s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  gFieldWork->transitionFrames = 24;
  Actor_GetById(ACTOR_LEVEL_EDITOR)->scaleY = LEVEL_EDITOR_SCALE;
  Actor_GetById(ACTOR_LEVEL_EDITOR)->scaleX = LEVEL_EDITOR_SCALE;
#if TBS_PATCH_1
  Import_ActorSetAnimation(ACTOR_FIRST_EMOTE_TESTER, 5);
  Import_ActorSetAnimation(ACTOR_SECOND_EMOTE_TESTER, 2);
#endif
  return 0;
}

#if !TBS_PATCH_1
/// Runs the Japanese animation test, then moves ACTOR_LEVEL_EDITOR to its test
/// position.
static void IncreaseItemAndLevel_RunMotionTest(void) {
  Event_Start();
  Import_ActorSetAnimation(ACTOR_PORTRAIT_BROWSER, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_LEVEL_EDITOR, ANIM_SHAKE_HEAD);
  Actor_WaitForAnimationChange(ACTOR_PORTRAIT_BROWSER);
  Actor_SetPosition(ACTOR_LEVEL_EDITOR, 312, 416);
  Event_End();
}
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
/// Displays the three character-set caption rows in a debug window. Force cast
/// at ACTOR_PORTRAIT_BROWSER, outside Japanese. The window is left open.
static void IncreaseItemAndLevel_ShowGlyphCaptions(void) {
  struct Window *window;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED u8 text[32];

  window = Window_Create(0, 13, 30, 6, WINDOW_FLAG_INSTANT);
  Text_DrawStringInWindow(gIncreaseItemAndLevel_GlyphsUpper, window, 0, 0);
  Text_DrawStringInWindow(gIncreaseItemAndLevel_GlyphsLower, window, 0, 8);
  Text_DrawStringInWindow(gIncreaseItemAndLevel_GlyphsMarks, window, 0, 16);
}
#endif

/// Keeps Force from moving ACTOR_PORTRAIT_BROWSER: run at phase 1 of Force
/// cast at it, this sets the effect's childMode, and Force's shot then leaves
/// its target where it is (field/event_commands_force.c).
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

#if LANGUAGE != LANGUAGE_JAPANESE
char gIncreaseItemAndLevel_PsyPrompt[8] = "Psy No:";
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
char gIncreaseItemAndLevel_PsyHelp[12] = "B:Return";
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
/// The glyph captions' first row; it has no I.
char gIncreaseItemAndLevel_GlyphsUpper[28] = "A B C D E F G H J K L M N O";
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
char gIncreaseItemAndLevel_GlyphsLower[28] = "P Q R S T U V W X Y Z a b c";
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
char gIncreaseItemAndLevel_GlyphsMarks[12] = "! ? # $ %";
#endif

struct SceneEntrance gIncreaseItemAndLevel_Entrances[] = {
    {0, CONDITION_ALWAYS, 824, 0, 864, FACING_NORTH, 0, 280, 504, 912, 896, 0},
    {SCENE_TABLE_END},
};

/// No exits.
u32 gIncreaseItemAndLevel_Exits[] = {
    SCENE_EXITS_END,
};

/// The scene's actors, in the order of enum IncreaseItemAndLevelActorId:
/// the item, icon and portrait browsers and the level editor; outside Japanese
/// also the ability browser (reachable in English and German only) and the two
/// emote testers.
// clang-format off
struct ActorPlacement gIncreaseItemAndLevel_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 864, FACING_SOUTHEAST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 480, FACING_SOUTHEAST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MENARDI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 504, FACING_SOUTHEAST,
        TALK_FACING_PARTY, 0),
#if !TBS_PATCH_1
    ACTOR_PLACEMENT(SPRITE_CAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 800, FACING_SOUTHEAST,
        TALK_FACING_PARTY, 0),
#elif TBS_PATCH_1 && !TBS_PATCH_3
    ACTOR_PLACEMENT(SPRITE_CAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 800, FACING_EAST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 848, 0, 864,
        FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_EMOTE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 848, 0, 688, FACING_SOUTH_SOUTHWEST,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EMOTE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 816, 0, 688, FACING_SOUTH_SOUTHWEST,
        TALK_FACING_KEEP, 0),
#else
    ACTOR_PLACEMENT(SPRITE_CAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 800, FACING_EAST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 80, 0, 864,
        FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_EMOTE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 848, 0, 688, FACING_SOUTH_SOUTHWEST,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EMOTE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 816, 0, 688, FACING_SOUTH_SOUTHWEST,
        TALK_FACING_KEEP, 0),
#endif
    {SCENE_TABLE_END},
};
// clang-format on

/// Talking to each tester runs its tool.
struct Trigger gIncreaseItemAndLevel_Triggers[] = {
    {TRIGGER_TALK, ACTOR_ITEM_BROWSER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_SelectItem)},
    {TRIGGER_TALK, ACTOR_ICON_BROWSER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_OpenIconBrowser)},
    {TRIGGER_TALK, ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_OpenPortraitBrowser)},
    {TRIGGER_TALK, ACTOR_LEVEL_EDITOR, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_RunLevelEditor)},
#if !TBS_PATCH_1
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_RunMotionTest)},
#else
#if !TBS_PATCH_3
    {TRIGGER_TALK, ACTOR_ABILITY_BROWSER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_SelectAbility)},
#endif
    {TRIGGER_TALK, ACTOR_FIRST_EMOTE_TESTER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowMessageThenEmote)},
    {TRIGGER_TALK, ACTOR_SECOND_EMOTE_TESTER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowEmoteThenMessage)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_ShowGlyphCaptions)},
#endif
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
     EVENT(IncreaseItemAndLevel_EnableEffectChildMode)},
    {TRIGGER_TABLE_END},
};
