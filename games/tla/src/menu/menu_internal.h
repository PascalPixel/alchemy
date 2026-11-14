#ifndef ALCHEMY_MENU_INTERNAL_H
#define ALCHEMY_MENU_INTERNAL_H

/// @file
/// What every part of the menu object shares: its includes and the declarations
/// its parts disagreed on. menu.c includes it before the parts, and each part
/// includes it too, so that a part read on its own still resolves.

#include "types.h"
#include "battle/battle.h"
#include "module/window/window.h"
#include "flags.h"

#define TEXT_COLOR_NORMAL TEXT_COLOR_WHITE ///< the window text's default colour

/// WindowSprite.motion as the menu sets it: shown, or hidden but kept.
#define ICON_SHOWN 1   ///< WINDOW_SPRITE_MOTION_STILL
#define ICON_HIDDEN 13 ///< WINDOW_SPRITE_MOTION_HIDDEN
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "minigame.h"
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
#include "field_event.h"
#include "game/item.h"
#include "battle/battle_unit.h"
#include "window.h"
#include "graphics/uiwindow.h"
#include "graphics/render_input.h"
#include "graphics/render_placement.h"
#include "field/sprite.h"
#include "menu/m7_interfaces.h"
#include "static_assert.h"
#include "text/msg_ids.h"
#include "system/irq.h"
#include "owner_state.h"
#include "inventory.h"
#include "system/heap_release.h"
#include "menu/menu_result.h"
#include "battle_type.h"
#include "battle/battle_system.h"
#include "graphics/display_transition.h"
#include "field/a9_motion.h"
#include "battle/battle_effect.h"
#include "menu/djinn_menu.h"
#include "macros.h"
#include "graphics/vram_tab.h"
#include "item_icon.h"

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
  u8 unk020[4];
  struct RenderInput *itemWindow;
  struct RenderInput *statusWindow;
  struct RenderInput *buttonHelpWindow;
  struct RenderInput *infoWindow;
  struct RenderInput *equipWindow;
  struct RenderInput *listWindow;
  u8 unk03c[4];
  struct RenderInput *modalWindow;
  u8 unk044[4];
  struct WindowSprite *gridCursor;
  struct WindowSprite *entryIcons[32];
  u8 unk0cc[0x24];
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
  u8 unk174[4];
  /// The up and down arrows beside the party row (MenuCursor_Create).
  struct WindowSprite *pageArrows[2];
  u8 unk180[0x44];
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
  u8 unk258[6];
  s16 messageOffset; ///< the item use's result, after MsgItemUseResult
  s8 equipPreview;   ///< 1 while the stats preview a removal, not an equip
  s8 listMode;       ///< the command chosen last; -1 for none
  u8 unk262[2];
  s8 selectedIndexByMember[8]; ///< each party member's entry chosen last
  u8 unk26c[0x282];
  u16 resourceSlots[2]; ///< VRAM cache slots of two arrow pictures
  u8 unk4f2[0x17];
  /// Nonzero while Menu_UpdateEntryObjectTransforms places the party figures
  /// (memberSprites); the shops clear it while a details window covers them.
  s8 memberSpritesShown;
  /// Nonzero while the row of four figures is drawn
  /// (DjinnFigures_SetShown).
  s8 figuresShown;
  u8 unk50b[0x6c1];
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
  s8 memberIndex[2]; ///< each side's party member, in memberIds
  s8 memberCount[2]; ///< party members each side chooses from
  u8 unk020[4];
  struct RenderInput *actionWindow;
  struct RenderInput *statusWindow;
  struct RenderInput *unopenedWindow; ///< never given a window; only cleared
  struct RenderInput *helpWindow;
  /// ItemMenuWork's equipWindow to gridCursor; not named here.
  u8 itemWindows[24];
  struct WindowSprite *entryIcons[32];
  u8 unk0cc[0x24];
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
  u8 unk174[4];
  /// The up and down arrows beside the party row (MenuCursor_Create).
  struct WindowSprite *pageArrows[2];
  u8 unk180[0x44];
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
  s8 tabIndex[2];  ///< each side's party member, in memberTable
  s8 tabCounts[2]; ///< party members each side chooses from
  u8 unk020[4];
  struct RenderInput *psynergyWindow;
  struct RenderInput *statusWindow;
  struct RenderInput *shortcutWindow;
  struct RenderInput *infoWindow;
  struct RenderInput *equipWindow; ///< ItemMenuWork's; not named here
  struct RenderInput *listWindow;
  u8 unk03c[0xc];
  struct WindowSprite *entryGridCursor;
  struct WindowSprite *entryIcons[32];
  u8 unk0cc[0x24];
  struct RenderInput *messageWindow;
  u8 unk0f4[4];
  void *tabSprites[8];    ///< the party members' figures
  s16 tabX[8];            ///< screen pixels
  s16 rowPositions[8];    ///< screen pixels
  s32 memberScale[8];     ///< 16.16 fixed point, Q16_ONE full size
  s16 memberSpriteIds[8]; ///< ItemMenuWork's; not named here
  u16 sideRow[2];         ///< each side's Psynergy row chosen
  u16 sideAction[2]; ///< each side's Psynergy chosen, as a party member action
                     ///< entry
  u8 unk170[8];
  /// The up and down arrows beside the party row (MenuCursor_Create).
  struct WindowSprite *pageArrows[2];
  u8 unk180[0x44];
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
  u8 unk220[0x3e];
  s16 useResult; ///< the use's result message, after MsgItemUseResult
  u8 unk260[4];
  s8 selectedIndexByMember[8]; ///< each party member's entry chosen last
  u8 mode; ///< 0 to use Psynergy; 1 or 2 to set the L or R shortcut
  u8 unk26d[3];
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
void StatusPanel_DrawHpPp(struct BattleUnit *unit, s32 window);
void Text_DrawStatComparison(const struct BattleUnit *alt,
                             const struct BattleUnit *base, s32 work);
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
void Animation_ApplyChildArgumentFar(void *object, s32 argument);
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
void Render_ApplyProjectedPlacementAtDepthFar(struct AnimationObject *object,
                                              struct SpritePlacement *position,
                                              struct SpriteScale *scale,
                                              s32 depth, s32 mode);

#endif
