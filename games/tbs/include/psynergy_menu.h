#ifndef ALCHEMY_PSYNERGY_MENU_H
#define ALCHEMY_PSYNERGY_MENU_H

/// @file
/// The Psynergy menu's state.

#include "unit/menu/menu.h"
#include "global_cells.h"
#include "static_assert.h"
#include "menu_result.h"
#include "render_input.h"
#include "text/msg_ids.h"

struct AnimationObject;

struct BattleUnit;

/// The Psynergy menu: its windows, icons and cursors, the party members' tabs
/// and the Psynergy listed for the chosen member. It is the HEAP_SLOT_MENU
/// block as this menu reads it, from the menu's opening to its closing.
struct PsynergyMenuState {
  u8 unk000[8];      ///< not named by TBS code
  s32 selectedOwner; ///< the party member chosen
  u8 unk00c[4];      ///< not named by TBS code
  struct UiWindow *auxiliaryWindow;
  struct RenderOutput *paneIcon[2]; ///< each pane's cursor
  s8 tabIndex[2];                   ///< each pane's member, in ownerTable
  s8 tabCounts[2];                  ///< members each pane chooses from
  struct UiWindow *psynergyWindow;
  struct UiWindow *statusWindow;
  struct UiWindow *shortcutWindow; ///< the L and R shortcuts
  struct UiWindow *infoWindow;
  u8 unk030[4]; ///< not named by TBS code
  struct UiWindow *listWindow;
  u8 unk038[0x0c]; ///< not named by TBS code
  struct RenderOutput *entryGridCursor;
  struct RenderOutput *entryIcons[32];
  u8 unk0c8[0x44]; ///< not named by TBS code
  struct UiWindow *messageWindow;
  /// The cursor grid: its place and its size.
  ///@{
  u8 selectedColumn;
  u8 selectedRow;
  u8 columnCount;
  u8 rowCount;
  ///@}
  struct AnimationObject *tabObjects[8]; ///< the party members' figures
  s16 tabX[8];                           ///< screen pixels
  s16 rowPositions[8];                   ///< screen pixels
  s32 ownerScale[8];  ///< 16.16 fixed point, 0x10000 full size
  u16 paneRow[2];     ///< each pane's Psynergy row chosen
  u16 paneAction[2];  ///< each pane's Psynergy chosen, as an owner action entry
  u8 unk17c[0x4c];    ///< not named by TBS code
  u16 psynergies[32]; ///< the chosen member's Psynergy listed
  u16 ownerTable[8];  ///< the party in order (Party_ListActiveOwners)
  u8 psynergyCount;
  u8 ownerCount;
  /// The user and the target: [1] is MENU_TARGET_WHOLE_PARTY for Psynergy
  /// on the whole party.
  u8 ownerIds[2];
  struct RenderOutput *cursorIcon;
  /// bits: 1 the list shows the shortcut help | 0 set after a use or a
  /// cancelled target
  u16 flags;
  u16 skipSlide;   ///< 1 skips the cursor's next slide once
  u8 unk224[0x18]; ///< not named by TBS code
  s16 slotY[4];    ///< screen pixels
  u8 unk244[0x16]; ///< not named by TBS code
  s16 useResult;   ///< the use's result message, after MsgItemUseResult
  u8 unk25c[4];    ///< not named by TBS code
  s8 selectedIndexByOwner[8]; ///< each member's entry chosen last
  u8 mode; ///< 0 to use Psynergy; 1 or 2 to set the L or R shortcut
};

STATIC_ASSERT(offsetof(struct PsynergyMenuState, auxiliaryWindow) == 0x010);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, psynergyWindow) == 0x020);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, entryGridCursor) == 0x044);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, entryIcons) == 0x048);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, messageWindow) == 0x10c);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, selectedColumn) == 0x110);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, selectedRow) == 0x111);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, columnCount) == 0x112);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, rowCount) == 0x113);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, paneAction[0]) == 0x178);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, psynergies) == 0x1c8);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, psynergyCount) == 0x218);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, ownerIds) == 0x21a);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, selectedIndexByOwner) ==
              0x260);

STATIC_ASSERT(offsetof(struct PsynergyMenuState, skipSlide) == 0x222);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, tabObjects) == 0x114);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, ownerScale) == 0x154);

#endif
