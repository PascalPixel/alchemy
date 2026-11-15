#ifndef ALCHEMY_MENU_INTERNAL_H
#define ALCHEMY_MENU_INTERNAL_H

/// @file
/// What every part of the menu object shares: its includes and the declarations
/// its parts disagreed on. menu.c includes it before the parts, and each part
/// includes it too, so that a part read on its own still resolves.

#include "types.h"
#include "battle/battle.h"
#include "window/window.h"
#include "graphics/glyph.h"
#include "flags.h"
#include "item_ids.h"
#include "djinn_ids.h"

#define TEXT_COLOR_NORMAL TEXT_COLOR_WHITE ///< the window text's default colour

/// WindowSprite.motion as the menu sets it: shown, or hidden but kept.
#define ICON_SHOWN 1   ///< WINDOW_SPRITE_MOTION_STILL
#define ICON_HIDDEN 13 ///< WINDOW_SPRITE_MOTION_HIDDEN

/// The menus' list entries, each with an icon sprite.
#define MENU_ENTRY_ICONS 32
/// Where an entry icon waits, just past the screen's bottom right corner,
/// until a screen places it.
#define MENU_ICON_HIDDEN_X (DISPLAY_WIDTH + 8)
#define MENU_ICON_HIDDEN_Y (DISPLAY_HEIGHT + 8)

/// The bytes of the menu work, heap block HEAP_SLOT_MENU: ShopWork's size, the
/// largest of the screens' views of it. Each screen's run function takes the
/// block as it opens and releases it as it closes.
#define MENU_WORK_SIZE 3308

/// The Djinn change DjinnMenu_DrawStatPreview previews, with the Djinn in the
/// work's sides removePairSlot and addPairSlot.
enum DjinnPreviewMode {
  DJINN_PREVIEW_ADD_AND_SET = 0, ///< the other's Djinni is added and set
  DJINN_PREVIEW_UNSET = 1,       ///< the chosen Djinni is unset
  /// A trade: the own Djinni is unset if set, the other's is added, and set
  /// if it was.
  DJINN_PREVIEW_TRADE = 2,
  DJINN_PREVIEW_NO_CHANGE = 3, ///< nothing changes; maximum HP and PP shown
  DJINN_PREVIEW_ADD = 4        ///< the other's Djinni is added, set if it was
};

/// What the Djinn menu's choosers return.
enum DjinnMenuResult {
  DJINN_RESULT_START = -2, ///< Start: the Djinn menu closes
  DJINN_RESULT_BACK = -1,
  DJINN_RESULT_CHOSEN = 1,    ///< A on a Djinni
  DJINN_RESULT_CONFIRMED = 1, ///< A on the preview's "Is this OK?"
  DJINN_RESULT_SET = 1,
  DJINN_RESULT_STANDBY = 2,
  DJINN_RESULT_GIVE = 3,
  DJINN_RESULT_TRADE = 4,
  /// The four commands applied without their preview, which no chooser
  /// returns.
  DJINN_RESULT_SET_NOW = 5,
  DJINN_RESULT_STANDBY_NOW = 6,
  DJINN_RESULT_STATUS = 7, ///< L: the party member's status
  DJINN_RESULT_GIVE_NOW = 8,
  DJINN_RESULT_TRADE_NOW = 9,
  DJINN_RESULT_HELP = 10 ///< Select: the help
};

/// DjinnMenuBackdrop.tutorialStep: where the Djinn tutorial is.
enum DjinnTutorialStep {
  DJINN_TUTORIAL_OFF = 0,
  /// Giving a Djinni and setting it.
  DJINN_TUTORIAL_GIVE = 1,
  /// The Djinn's powers.
  DJINN_TUTORIAL_POWERS = 14,
  /// Recovery after a summon.
  DJINN_TUTORIAL_RECOVERY = 27,
  /// Waits for the player's A, then presses B.
  DJINN_TUTORIAL_CLOSE = 28
};

/// DjinnMenu_SelectDjinn's two choosers.
enum DjinnSelectMode {
  DJINN_SELECT_DJINNI = 0, ///< a Djinni in any party member's column
  DJINN_SELECT_ACTION = 1  ///< what to do with the Djinni chosen
};

/// The tile of BG character block 1 the Djinn backdrop's border tile is copied
/// to (menu_djinn.c); the preview screens blink it (menu_djinn_preview.c).
#define DJINN_BACKDROP_BORDER_TILE 150
#define DJINN_BACKDROP_BORDER_VRAM                                             \
  (BG_CHAR_BLOCK(1) + DJINN_BACKDROP_BORDER_TILE * TILE_SIZE_4BPP)

/// A Djinn list entry's party member, element and index, shifted down.
#define DJINN_MEMBER(v) (((v) & 0xf00) >> 8)
#define DJINN_ELEMENT(v) (((v) & 0xe0) >> 5)
#define DJINN_NUMBER(v) ((v) & 0x1f)

/// The preview loop's four operations, as its confirmation text describes them
/// (PsynergyList_RunCompareLoop's mode).
enum DjinnCompareOperation {
  DJINN_COMPARE_TRADE = 0,
  DJINN_COMPARE_GIVE = 1,
  DJINN_COMPARE_STANDBY = 2,
  DJINN_COMPARE_SET = 3
};
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "edition.h"
#include "tla_edition.h"
#include "version.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/iwram_call.h"
#include "system/frame.h"
#include "system/input.h"
#include "fixed_math.h"
#include "graphics/resource.h"
#include "system/task.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "field/field_event.h"
#include "game/item.h"
#include "battle/battle_unit.h"
#include "window/window.h"
#include "graphics/uiwindow.h"
#include "graphics/render_input.h"
#include "graphics/render_placement.h"
#include "field/sprite.h"
#include "menu/m7_interfaces.h"
#include "static_assert.h"
#include "text/msg_ids.h"
#include "system/irq.h"
#include "game/owner_state.h"
#include "game/inventory.h"
#include "system/heap_release.h"
#include "menu/menu_result.h"
#include "battle/battle_type.h"
#include "battle/battle_system.h"
#include "graphics/display_transition.h"
#include "field/a9_motion.h"
#include "battle/battle_effect.h"
#include "menu/djinn_menu.h"
#include "menu/select.h"
#include "macros.h"
#include "graphics/vram_tab.h"
#include "window/item_icon.h"

/// The item menu's work, Ram_HeapSlots->menuWork (heap slot 220): its
/// windows, the party figures, the items listed and the two choices a command
/// makes. The Broken Seal's ItemMenuWork, whose names its members take: from
/// itemWindow to the entry icons it lies 4 bytes further on than The Broken
/// Seal's, and from questionWindow on 28 bytes closer, with the figures' ids
/// after their scales. The status, Psynergy and Djinn screens read the
/// members they share with it through it too.
struct ItemMenuWork {
  u8 unused[8];                    ///< no view of the menu work names it
  s32 selectedMember;              ///< the party member chosen
  struct RenderInput *moneyWindow; ///< ShopWork's; not named here
  struct RenderInput *mainWindow;
  /// The two sides: the party member whose items are shown, and the one an item
  /// is given to.
  ///@{
  struct WindowSprite *sideIcons[2];
  s8 sideIndex[2]; ///< the party member chosen, in memberIds
  s8 sideCount[2]; ///< party members to choose from
  ///@}
  u8 unusedAfterSides[4]; ///< no view of the menu work names it
  struct RenderInput *itemWindow;
  struct RenderInput *statusWindow;
  struct RenderInput *buttonHelpWindow;
  struct RenderInput *infoWindow;
  struct RenderInput *equipWindow;
  struct RenderInput *listWindow;
  struct RenderInput *unopenedWindow; ///< never given a window; only closed
  struct RenderInput *modalWindow;
  struct RenderInput
      *secondUnopenedWindow; ///< never given a window; only closed
  struct WindowSprite *gridCursor;
  struct WindowSprite *entryIcons[MENU_ENTRY_ICONS];
  u8 unusedAfterIcons[36]; ///< no view of the menu work names it
  struct RenderInput *questionWindow;
  /// The cursor grid: its place and its size.
  ///@{
  u8 selectedColumn;
  u8 selectedRow;
  u8 columnCount;
  u8 rowCount;
  ///@}
  void *memberSprites[8]; ///< the party members' figures
  s16 memberX[8];         ///< screen pixels
  s16 memberY[8];         ///< screen pixels
  s32 memberScale[8];     ///< 16.16 fixed point, Q16_ONE full size
  /// The party member each figure shows, -1 for none
  /// (Menu_CreateOwnerEntryObjects).
  s16 memberSpriteIds[8];
  u16 selectedSlots[2]; ///< each side's inventory slot chosen
  u16 selectedItems[2]; ///< each side's inventory entry chosen
  struct WindowSprite *cursor;
  u8 unusedBeforeArrows[4]; ///< no view of the menu work names it
  /// The up and down arrows beside the party row (MenuCursor_Create).
  struct WindowSprite *pageArrows[2];
  /// DjinnMenuWork's backdrop pointer at 0x180; no view names the rest.
  u8 aroundBackdrop[68];
  u16 items[32];    ///< the inventory entries listed
  u16 memberIds[8]; ///< the party in order (Party_ListMembersFar)
  u8 itemCount;
  u8 partyCount;
  u8 sideMember[2]; ///< each side's party member
  struct WindowSprite *selectedItemIcon;
  /// bits: 1 the item's name and command grid are drawn (Details sets it in
  /// Japanese)
  u16 flags;
  s16 completionFlag; ///< set to 1 as item commands end
  /// The row of four figures the Djinn and status screens and the file list
  /// show (DjinnFigures_InitializeTopRow): each one's sprite, place in screen
  /// pixels and depth, and its scale in 16.16, which closes a quarter of its
  /// gap to Q16_ONE each frame, or holds -scale while negative.
  ///@{
  void *figures[4];
  s16 figureX[4];
  s16 figureY[4];
  s32 figureScales[4];
  u16 figureDepths[4];
  ///@}
  u8 unusedAfterFigures[6]; ///< DjinnMenuWork's pair; not named here
  s16 messageOffset;        ///< the item use's result, after MsgItemUseResult
  s8 equipPreview; ///< 1 while the stats preview a removal, not an equip
  s8 listMode;     ///< the command chosen last; -1 for none
  u8 unusedAfterListMode[2];   ///< no view of the menu work names it
  s8 selectedIndexByMember[8]; ///< each party member's entry chosen last
  u8 mode;                     ///< PsynergyMenuWork's; not named here
  u8 unusedAfterMode[3];       ///< no view of the menu work names it
  u8 shopStock[0x27e];         ///< ShopWork's stock and party; not named here
  u16 resourceSlots[2];        ///< VRAM cache slots of two arrow pictures
  u8 shopKeeper[0x17];         ///< ShopWork's cursor and keeper; not named here
  /// Nonzero while Menu_UpdateEntryObjectTransforms places the party figures
  /// (memberSprites); the shops clear it while a details window covers them.
  s8 memberSpritesShown;
  /// Nonzero while the row of four figures is drawn
  /// (DjinnFigures_SetShown).
  s8 figuresShown;
  u8 shopEffects[0x6c1]; ///< ShopWork's particles; not named here
  /// The five menu icons (MenuIcon_LoadSlots): a sprite and a VRAM cache
  /// slot each, queued while menuIconsShown is nonzero.
  ///@{
  struct IconSprite menuIcons[5];
  u16 menuIconSlots[5];
  s8 menuIconsShown;
  ///@}
};

STATIC_ASSERT(offsetof(struct ItemMenuWork, itemWindow) == 0x24);
STATIC_ASSERT(offsetof(struct ItemMenuWork, gridCursor) == 0x48);
STATIC_ASSERT(offsetof(struct ItemMenuWork, entryIcons) == 0x4c);
STATIC_ASSERT(offsetof(struct ItemMenuWork, questionWindow) == 0xf0);
STATIC_ASSERT(offsetof(struct ItemMenuWork, memberSprites) == 0xf8);
STATIC_ASSERT(offsetof(struct ItemMenuWork, memberY) == 0x128);
STATIC_ASSERT(offsetof(struct ItemMenuWork, memberSpriteIds) == 0x158);
STATIC_ASSERT(offsetof(struct ItemMenuWork, selectedSlots) == 0x168);
STATIC_ASSERT(offsetof(struct ItemMenuWork, pageArrows) == 0x178);
STATIC_ASSERT(offsetof(struct ItemMenuWork, items) == 0x1c4);
STATIC_ASSERT(offsetof(struct ItemMenuWork, sideMember) == 0x216);
STATIC_ASSERT(offsetof(struct ItemMenuWork, flags) == 0x21c);
STATIC_ASSERT(offsetof(struct ItemMenuWork, completionFlag) == 0x21e);
STATIC_ASSERT(offsetof(struct ItemMenuWork, messageOffset) == 0x25e);
STATIC_ASSERT(offsetof(struct ItemMenuWork, listMode) == 0x261);
STATIC_ASSERT(offsetof(struct ItemMenuWork, selectedIndexByMember) == 0x264);
STATIC_ASSERT(offsetof(struct ItemMenuWork, figures) == 0x220);
STATIC_ASSERT(offsetof(struct ItemMenuWork, figureDepths) == 0x250);
STATIC_ASSERT(offsetof(struct ItemMenuWork, resourceSlots) == 0x4ee);
STATIC_ASSERT(offsetof(struct ItemMenuWork, memberSpritesShown) == 0x509);
STATIC_ASSERT(offsetof(struct ItemMenuWork, menuIcons) == 0xbcc);
STATIC_ASSERT(offsetof(struct ItemMenuWork, menuIconsShown) == 0xc12);

/// The work of the party member chooser and the Status menu (heap slot 220),
/// which each takes as it opens and frees before it returns. The Broken Seal's
/// StatusScreenWork, laid out as ItemMenuWork is here.
struct StatusScreenWork {
  u8 unused[8];                    ///< no view of the menu work names it
  s32 selectedMember;              ///< the party member chosen
  struct RenderInput *moneyWindow; ///< ShopWork's; not named here
  struct RenderInput *memberWindow;
  struct WindowSprite *memberCursors[2];
  s8 memberIndex[2];      ///< each side's party member, in memberIds
  s8 memberCount[2];      ///< party members each side chooses from
  u8 unusedAfterSides[4]; ///< no view of the menu work names it
  struct RenderInput *actionWindow;
  struct RenderInput *statusWindow;
  struct RenderInput *unopenedWindow; ///< never given a window; only cleared
  struct RenderInput *helpWindow;
  /// ItemMenuWork's equipWindow to gridCursor; not named here.
  u8 itemWindows[24];
  struct WindowSprite *entryIcons[MENU_ENTRY_ICONS];
  u8 unusedAfterIcons[36]; ///< no view of the menu work names it
  struct RenderInput *selectorWindow;
  u8 selectedColumn;
  u8 selectedRow;
  u8 columnCount;
  u8 rowCount;
  void *memberSprites[8];
  s16 memberX[8];         ///< screen pixels
  s16 memberY[8];         ///< screen pixels
  s32 memberScale[8];     ///< 16.16 fixed point, Q16_ONE full size
  s16 memberSpriteIds[8]; ///< ItemMenuWork's; not named here
  u16 selectedSlots[2];
  u16 selectedActions[2];
  struct WindowSprite *statusCursor;
  u8 unusedBeforeArrows[4]; ///< no view of the menu work names it
  /// The up and down arrows beside the party row (MenuCursor_Create).
  struct WindowSprite *pageArrows[2];
  /// DjinnMenuWork's backdrop pointer at 0x180; no view names the rest.
  u8 aroundBackdrop[68];
  u16 entries[32];
  u16 memberIds[8]; ///< the party in order (Party_ListMembersFar)
  u8 entryCount;
  u8 partyCount;
  u8 selectedIds[2]; ///< each side's party member
  struct WindowSprite *selectedIcon;
  u16 page;           ///< 3 for the party member chooser, 0 for the Status menu
  s16 completionFlag; ///< ItemMenuWork's; not named here
  void *rowSprites[4];
  s16 rowX[4]; ///< screen pixels
  s16 rowY[4]; ///< screen pixels
};

STATIC_ASSERT(offsetof(struct StatusScreenWork, memberWindow) == 0x10);
STATIC_ASSERT(offsetof(struct StatusScreenWork, actionWindow) == 0x24);
STATIC_ASSERT(offsetof(struct StatusScreenWork, entryIcons) == 0x4c);
STATIC_ASSERT(offsetof(struct StatusScreenWork, selectorWindow) == 0xf0);
STATIC_ASSERT(offsetof(struct StatusScreenWork, selectedColumn) == 0xf4);
STATIC_ASSERT(offsetof(struct StatusScreenWork, memberY) == 0x128);
STATIC_ASSERT(offsetof(struct StatusScreenWork, selectedSlots) == 0x168);
STATIC_ASSERT(offsetof(struct StatusScreenWork, statusCursor) == 0x170);
STATIC_ASSERT(offsetof(struct StatusScreenWork, pageArrows) == 0x178);
STATIC_ASSERT(offsetof(struct StatusScreenWork, memberIds) == 0x204);
STATIC_ASSERT(offsetof(struct StatusScreenWork, selectedIds) == 0x216);
STATIC_ASSERT(offsetof(struct StatusScreenWork, page) == 0x21c);
STATIC_ASSERT(offsetof(struct StatusScreenWork, rowSprites) == 0x220);
STATIC_ASSERT(offsetof(struct StatusScreenWork, rowX) == 0x230);
STATIC_ASSERT(sizeof(struct StatusScreenWork) == 0x240);

/// The Psynergy menu's work (heap slot 220): its windows, icons and cursors,
/// the party members' tabs and the Psynergy listed for the chosen party
/// member. The Broken Seal's PsynergyMenuWork, laid out as ItemMenuWork is
/// here. The menu's shared routines (MenuCursor_Create, MenuCursor_Slide,
/// Menu_PlacePartyFigures) read their cursor and figures through it, as The
/// Broken Seal's do.
struct PsynergyMenuWork {
  u8 unused[8];                    ///< no view of the menu work names it
  s32 selectedMember;              ///< the party member chosen
  struct RenderInput *moneyWindow; ///< ShopWork's; not named here
  struct RenderInput *auxiliaryWindow;
  struct WindowSprite *sideIcon[2]; ///< each side's cursor
  s8 tabIndex[2];         ///< each side's party member, in memberTable
  s8 tabCounts[2];        ///< party members each side chooses from
  u8 unusedAfterSides[4]; ///< no view of the menu work names it
  struct RenderInput *psynergyWindow;
  struct RenderInput *statusWindow;
  struct RenderInput *shortcutWindow;
  struct RenderInput *infoWindow;
  struct RenderInput *equipWindow; ///< ItemMenuWork's; not named here
  struct RenderInput *listWindow;
  /// ItemMenuWork's modal window and the words around it; not named here.
  u8 modalWindows[12];
  struct WindowSprite *entryGridCursor;
  struct WindowSprite *entryIcons[MENU_ENTRY_ICONS];
  u8 unusedAfterIcons[36]; ///< no view of the menu work names it
  struct RenderInput *messageWindow;
  u8 cursorGrid[4];       ///< ItemMenuWork's cursor grid; not named here
  void *tabSprites[8];    ///< the party members' figures
  s16 tabX[8];            ///< screen pixels
  s16 rowPositions[8];    ///< screen pixels
  s32 memberScale[8];     ///< 16.16 fixed point, Q16_ONE full size
  s16 memberSpriteIds[8]; ///< ItemMenuWork's; not named here
  u16 sideRow[2];         ///< each side's Psynergy row chosen
  u16 sideAction[2]; ///< each side's Psynergy chosen, as a party member action
                     ///< entry
  /// ItemMenuWork's cursor and the word after it; not named here.
  u8 itemCursor[8];
  /// The up and down arrows beside the party row (MenuCursor_Create).
  struct WindowSprite *pageArrows[2];
  /// DjinnMenuWork's backdrop pointer at 0x180; no view names the rest.
  u8 aroundBackdrop[68];
  u16 psynergies[32]; ///< the chosen party member's Psynergy listed
  u16 memberTable[8]; ///< the party in order (Party_ListMembersFar)
  u8 psynergyCount;
  u8 memberCount;
  /// The user and the target: [1] is MENU_TARGET_WHOLE_PARTY for Psynergy on
  /// the whole party.
  u8 memberIds[2];
  struct WindowSprite *cursorIcon;
  /// bits: 0 set after a use or a cancelled target
  u16 flags;
  u16 skipSlide; ///< 1 skips the cursor's next slide once
  /// ItemMenuWork's row of figures and the pair after it; not named here.
  u8 figureRow[0x3e];
  s16 useResult; ///< the use's result message, after MsgItemUseResult
  /// ItemMenuWork's equipPreview and listMode; not named here.
  u8 listState[4];
  s8 selectedIndexByMember[8]; ///< each party member's entry chosen last
  u8 mode; ///< 0 to use Psynergy; 1 or 2 to set the L or R shortcut
  u8 unusedAfterMode[3]; ///< no view of the menu work names it
};

STATIC_ASSERT(offsetof(struct PsynergyMenuWork, auxiliaryWindow) == 0x010);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, psynergyWindow) == 0x024);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, entryGridCursor) == 0x048);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, messageWindow) == 0x0f0);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, tabSprites) == 0x0f8);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, memberScale) == 0x138);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, sideAction) == 0x16c);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, pageArrows) == 0x178);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, psynergies) == 0x1c4);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, memberIds) == 0x216);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, skipSlide) == 0x21e);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, useResult) == 0x25e);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, selectedIndexByMember) ==
              0x264);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, mode) == 0x26c);

// The menu object's functions its parts call before their definitions, or
// with declarations of their own that disagree.
void StatusPanel_DrawHpPp(struct Unit *unit, s32 window);
void Text_DrawStatComparison(const struct Unit *alt, const struct Unit *base,
                             s32 work);
void Text_DrawMessageAsTilesFar(s32 message, struct RenderInput *window, s32 x,
                                s32 y);

// Tags the parts name in prototypes before the part that defines them.
struct DjinnMenuBackdrop;

// What several parts call, declared once here for all of them.
s32 GameFlag_Get(s32);
void GameFlag_Clear(s32 flag_no);
s32 Inventory_EquipFar(s32 owner, s32 slot);
void Window_DrawBorderFar(s32 x, s32 y, s32 width, s32 height);
void Window_EraseBorderRectFar(s32 x, s32 y, s32 width, s32 height);
void Text_UseExtraTilesFar(s32 flag);
void Window_MarkVisibleTextTilesFar(void);
void Menu_CreateOwnerEntryObjects(s32 first);
void Menu_HighlightPageEntry(s32 unused, s32 index, s32 count);
s32 Menu_MoveGridCursor(s32 *selection, s32 count, s32 columns);
void Resource_LoadByModeIntoSlotFar(s32 mode, s32 id, s32 slot, s32 flags);
extern u8 MsgShortcutHelp[];
void Sprite_StartAnimationFar(void *object, s32 argument);
void Link_DrawPageArrowsFar(s32 addr);
void Menu_UpdateEntryObjectTransforms(void);
void Input_ResetKeyRepeat(void);
s32 StatusScreen_BuildAvailability(u8 *ailments, s32 requested, s32 owner);
void ItemMenu_DrawCategory(s32 window, s32 ownerId, s32 mode);
s32 Ability_ApplyToTargets(s32 entry, s32 member, s32 target, s32 mode);
s32 Ability_PlayUseAnimation(s32 action);
void DjinnFigures_InitializeTopRow(void);
void DjinnFigures_ReleaseTopRow(void);
void Menu_ResetTwoResourceEntries(void);
extern u8 gMenu_BackdropBorderTile[];
void WindowSprite_PrepareForRedrawFar(s32 window);
s32 Message_AllTypedFar(void);
void DjinnFigures_SetSlotPhase(s32 index, s32 scale);
void DjinnFigures_SetSlotPosition(s32 slot, s32 x, s32 y);
void Render_ApplyProjectedPlacementAtDepthFar(struct Sprite *object,
                                              struct SpritePlacement *position,
                                              struct SpriteScale *scale,
                                              s32 depth, s32 mode);

/// The result of using an item or Psynergy from the menu: its message,
/// counted from MsgItemUseResult (Ability_ApplyToTargets).
enum ItemUseResult {
  ITEM_USE_RESULT_HP_FULLY_RECOVERED = 0, ///< "You recovered all HP!"
  ITEM_USE_RESULT_HP_RECOVERED = 1,       ///< "You recovered HP!"
  ITEM_USE_RESULT_HP_NOT_RECOVERED = 2,   ///< "You couldn't recover HP."
  ITEM_USE_RESULT_PARTY_HP_RECOVERED = 3, ///< "Everyone recovered HP!"
  ITEM_USE_RESULT_HP_FULL = 4,            ///< "Your HP is maxed out!"
  ITEM_USE_RESULT_PP_FULLY_RECOVERED = 5, ///< "You recovered all PP!"
  ITEM_USE_RESULT_PP_RECOVERED = 6,       ///< "You recovered some PP!"
  ITEM_USE_RESULT_PP_FULL = 7,            ///< "Your PP is maxed out!"
  ITEM_USE_RESULT_PARTY_PP_RECOVERED = 8, ///< "Everyone recovered PP!"
  /// A second "Your PP is maxed out!", which no use chooses.
  ITEM_USE_RESULT_PP_FULL_UNUSED = 9,
  ITEM_USE_RESULT_POISON_PURGED = 10, ///< "The poison was purged!"
  ITEM_USE_RESULT_NOT_POISONED = 11,  ///< "You're not poisoned."
  ITEM_USE_RESULT_REVIVED = 12,       ///< "You were revived!"
  ITEM_USE_RESULT_NOT_DOWN = 13,      ///< "You're not down."
  /// "The curse was removed." and "You are not cursed.", which no use chooses.
  ITEM_USE_RESULT_CURSE_REMOVED = 14,
  ITEM_USE_RESULT_NOT_CURSED = 15,
  ITEM_USE_RESULT_MAX_HP_UP = 16,  ///< "Your maximum HP increased!"
  ITEM_USE_RESULT_MAX_PP_UP = 17,  ///< "Your maximum PP increased!"
  ITEM_USE_RESULT_AGILITY_UP = 18, ///< "Your Agility increased!"
  ITEM_USE_RESULT_LUCK_UP = 19,    ///< "Your Luck increased!"
  ITEM_USE_RESULT_ATTACK_UP = 20,  ///< "Your Attack increased!"
  ITEM_USE_RESULT_DEFENSE_UP = 21  ///< "Your Defense increased!"
};

/// StatusScreenWork.page: which screen the party member row belongs to.
enum StatusScreenPage {
  STATUS_SCREEN_PAGE_STATUS = 0, ///< the Status menu (StatusScreen_Run)
  /// The party member chooser (PartyMemberChooser_Open).
  STATUS_SCREEN_PAGE_CHOOSER = 3
};

/// Which Psynergy PsynergyMenu_CollectActions lists.
enum PsynergyListMode {
  /// Those with a field effect or usable in the field first, then the rest.
  PSYNERGY_LIST_ALL = 0,
  PSYNERGY_LIST_SHORTCUTS = 1, ///< only those that work on the field
  /// Those with a field effect or usable in the field.
  PSYNERGY_LIST_FIELD = 2
};

/// How the item menu can use an item (Item_ClassifyUseMode).
enum ItemUseMode {
  /// It cannot be used here: it has no action, it is equipment the party member
  /// cannot wear or that teaches Psynergy, or its action is usable only in
  /// battle.
  ITEM_USE_MODE_UNUSABLE = -1,
  /// It works on the field (Scene_HasPsynergyTriggerFar), or its action is
  /// usable
  /// neither in the field nor in battle, and is used once the menu closes.
  ITEM_USE_MODE_FROM_FIELD = 0,
  ITEM_USE_MODE_CHOOSE_MEMBER = 1, ///< choose a party member to use it on
  ITEM_USE_MODE_WHOLE_PARTY = 2 ///< ABILITY_RANGE_ALL: the whole party at once
};

/// Inventory icons occupy five columns in the fifteen-slot list.
#define MENU_ITEM_GRID_COLUMNS 5

/// The statuses the Status screens show, as StatusScreen_BuildAvailability
/// fills them: one byte each, nonzero while the party member has it.
enum UnitStatus {
  CHARACTER_DOWN,   ///< HP 0; listed only when the caller asks for it
  CHARACTER_POISON, ///< Unit.poison 1
  CHARACTER_VENOM,  ///< Unit.poison any other nonzero value
  CHARACTER_CURSE,  ///< Unit.curse nonzero
  CHARACTER_HAUNT,  ///< Unit.haunt nonzero
  CHARACTER_AILMENT_COUNT
};

// Status labels use the Latin font's pixel positions or the Japanese tile grid.
#if LANGUAGE == LANGUAGE_JAPANESE
#define MENU_DRAW_STATUS_LABEL Text_DrawStringInWindowFar
#else
#define MENU_DRAW_STATUS_LABEL Text_DrawStringAtOffsetFar
#endif

/// The info window holds a message the next selection must not draw over.
#define FLAG_MENU_INFO_KEPT 0x151

/// PsynergyList_RunCompareLoop's pages for a give or trade, which R cycles; the
/// first two also index its per-member page counts.
enum DjinnComparePage {
  DJINN_COMPARE_PAGE_GIVER = 0,    ///< the giver's stats and Psynergy
  DJINN_COMPARE_PAGE_RECEIVER = 1, ///< the receiver's stats and Psynergy
  DJINN_COMPARE_PAGE_BOTH = 2      ///< both party members' stats, shown first
};

#endif
