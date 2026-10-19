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
/// work slot.
struct CharacterMenuState {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[4];
  struct UiWindow *ownerWindow;
  struct RenderOutput *ownerCursors[2];
  s8 ownerIndex[2];
  s8 ownerCount[2];
  struct UiWindow *actionWindow;
  struct UiWindow *statusWindow;
  struct UiWindow *unknownWindow028;
  struct UiWindow *helpWindow;
  u8 unk030[0x18];
  struct RenderOutput *entryIcons[32];
  struct RenderOutput *categoryIcons[8];
  u8 unk0e8[0x24];
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
  u8 unk180[0x48];
  u16 entries[32];
  u16 ownerIds[8];
  u8 entryCount;
  u8 partyCount;
  u8 selectedIds[2];
  struct RenderOutput *selectedIcon;
  u16 page;
  u8 unk222[2];
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

enum CharacterAilment {
  CHARACTER_DOWN,
  CHARACTER_POISON,
  CHARACTER_VENOM,
  CHARACTER_CURSE,
  CHARACTER_HAUNT,
  CHARACTER_AILMENT_COUNT
};

#endif
