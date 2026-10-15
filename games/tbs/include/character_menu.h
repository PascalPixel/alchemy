#ifndef ALCHEMY_CHARACTER_MENU_H
#define ALCHEMY_CHARACTER_MENU_H

#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/menu/menu.h"
#include "types.h"
#include "global_cells.h"
#include "layout_guard.h"
#include "animspr.h"
#include "window.h"

struct BattleUnit;
struct OwnerActionSlot;

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
  s16 ownerX[8];
  s16 ownerY[8];
  s32 ownerScale[8];
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
  s16 rowX[4];
  s16 rowY[4];
};

LAYOUT_OFFSET_GUARD(CharacterMenuState_OwnerWindow, struct CharacterMenuState,
                    ownerWindow, 0x10);
LAYOUT_OFFSET_GUARD(CharacterMenuState_OwnerCursors, struct CharacterMenuState,
                    ownerCursors, 0x14);
LAYOUT_OFFSET_GUARD(CharacterMenuState_StatusWindow, struct CharacterMenuState,
                    statusWindow, 0x24);
LAYOUT_OFFSET_GUARD(CharacterMenuState_EntryIcons, struct CharacterMenuState,
                    entryIcons, 0x48);
LAYOUT_OFFSET_GUARD(CharacterMenuState_SelectorWindow,
                    struct CharacterMenuState, selectorWindow, 0x10c);
LAYOUT_OFFSET_GUARD(CharacterMenuState_SelectedColumn,
                    struct CharacterMenuState, selectedColumn, 0x110);
LAYOUT_OFFSET_GUARD(CharacterMenuState_OwnerObjects, struct CharacterMenuState,
                    ownerObjects, 0x114);
LAYOUT_OFFSET_GUARD(CharacterMenuState_OwnerY, struct CharacterMenuState,
                    ownerY, 0x144);
LAYOUT_OFFSET_GUARD(CharacterMenuState_SelectedSlots, struct CharacterMenuState,
                    selectedSlots, 0x174);
LAYOUT_OFFSET_GUARD(CharacterMenuState_StatusCursor, struct CharacterMenuState,
                    statusCursor, 0x17c);
LAYOUT_OFFSET_GUARD(CharacterMenuState_OwnerIds, struct CharacterMenuState,
                    ownerIds, 0x208);
LAYOUT_OFFSET_GUARD(CharacterMenuState_SelectedIds, struct CharacterMenuState,
                    selectedIds, 0x21a);
LAYOUT_OFFSET_GUARD(CharacterMenuState_Page, struct CharacterMenuState, page,
                    0x220);
LAYOUT_OFFSET_GUARD(CharacterMenuState_RowObjects, struct CharacterMenuState,
                    rowObjects, 0x224);
LAYOUT_OFFSET_GUARD(CharacterMenuState_RowX, struct CharacterMenuState, rowX,
                    0x234);
LAYOUT_SIZE_GUARD(CharacterMenuState_PrefixSize, struct CharacterMenuState,
                  0x244);

enum CharacterAilment {
  CHARACTER_DOWN,
  CHARACTER_POISON,
  CHARACTER_VENOM,
  CHARACTER_CURSE,
  CHARACTER_HAUNT,
  CHARACTER_AILMENT_COUNT
};

#endif
