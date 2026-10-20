#ifndef ALCHEMY_CHARACTER_MENU_H
#define ALCHEMY_CHARACTER_MENU_H

/// @file
/// The character menu's state, and the ailments it shows.

#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/menu/menu.h"
#include "types.h"
#include "global_cells.h"
#include "static_assert.h"
#include "animspr.h"
#include "window.h"

struct BattleUnit;
struct OwnerActionSlot;

/// The work of the member chooser and the Status menu, in the HEAP_SLOT_MENU
/// work slot, which each takes as it opens and frees before it returns.
struct CharacterMenuState {
  u8 unk000[8];      ///< not named by TBS code
  s32 selectedOwner; ///< the party member chosen
  u8 unk00c[4];      ///< not named by TBS code
  struct UiWindow *ownerWindow;
  struct RenderOutput *ownerCursors[2];
  s8 ownerIndex[2]; ///< each pane's member, in ownerIds
  s8 ownerCount[2]; ///< members each pane chooses from
  struct UiWindow *actionWindow;
  struct UiWindow *statusWindow;
  struct UiWindow *unknownWindow028; ///< only cleared by name in TBS
  struct UiWindow *helpWindow;
  u8 unk030[0x18]; ///< not named by TBS code
  struct RenderOutput *entryIcons[32];
  struct RenderOutput *categoryIcons[8];
  u8 unk0e8[0x24]; ///< not named by TBS code
  struct UiWindow *selectorWindow;
  u8 selectedColumn;
  u8 selectedRow;
  u8 columnCount;
  u8 rowCount;
  struct AnimationObject *ownerObjects[8];
  s16 ownerX[8];     ///< screen pixels
  s16 ownerY[8];     ///< screen pixels
  s32 ownerScale[8]; ///< 16.16 fixed point, 0x10000 full size
  u16 selectedSlots[2];
  u16 selectedActions[2];
  struct RenderOutput *statusCursor;
  u8 unk180[0x48]; ///< not named by TBS code
  u16 entries[32];
  u16 ownerIds[8]; ///< the party in order (Party_ListActiveOwners)
  u8 entryCount;
  u8 partyCount;
  u8 selectedIds[2]; ///< each pane's member
  struct RenderOutput *selectedIcon;
  u16 page;     ///< 3 for the member chooser, 0 for the Status menu
  u8 unk222[2]; ///< not named by TBS code
  struct AnimationObject *rowObjects[4];
  s16 rowX[4]; ///< screen pixels
  s16 rowY[4]; ///< screen pixels
};

STATIC_ASSERT(offsetof(struct CharacterMenuState, ownerWindow) == 0x10);
STATIC_ASSERT(offsetof(struct CharacterMenuState, ownerCursors) == 0x14);
STATIC_ASSERT(offsetof(struct CharacterMenuState, statusWindow) == 0x24);
STATIC_ASSERT(offsetof(struct CharacterMenuState, entryIcons) == 0x48);
STATIC_ASSERT(offsetof(struct CharacterMenuState, selectorWindow) == 0x10c);
STATIC_ASSERT(offsetof(struct CharacterMenuState, selectedColumn) == 0x110);
STATIC_ASSERT(offsetof(struct CharacterMenuState, ownerObjects) == 0x114);
STATIC_ASSERT(offsetof(struct CharacterMenuState, ownerY) == 0x144);
STATIC_ASSERT(offsetof(struct CharacterMenuState, selectedSlots) == 0x174);
STATIC_ASSERT(offsetof(struct CharacterMenuState, statusCursor) == 0x17c);
STATIC_ASSERT(offsetof(struct CharacterMenuState, ownerIds) == 0x208);
STATIC_ASSERT(offsetof(struct CharacterMenuState, selectedIds) == 0x21a);
STATIC_ASSERT(offsetof(struct CharacterMenuState, page) == 0x220);
STATIC_ASSERT(offsetof(struct CharacterMenuState, rowObjects) == 0x224);
STATIC_ASSERT(offsetof(struct CharacterMenuState, rowX) == 0x234);
STATIC_ASSERT(sizeof(struct CharacterMenuState) == 0x244);

/// The ailments the character menus show, as CharacterMenu_BuildAvailability
/// fills them: one byte each, nonzero while the member has it.
enum CharacterAilment {
  CHARACTER_DOWN,   ///< HP 0; listed only when the caller asks for it
  CHARACTER_POISON, ///< BattleUnit.poison 1
  CHARACTER_VENOM,  ///< BattleUnit.poison any other nonzero value
  CHARACTER_CURSE,  ///< BattleUnit.restraint nonzero
  CHARACTER_HAUNT,  ///< BattleUnit.haunt nonzero
  CHARACTER_AILMENT_COUNT
};

#endif
