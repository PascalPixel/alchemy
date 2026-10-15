#ifndef ALCHEMY_PSYNERGY_MENU_H
#define ALCHEMY_PSYNERGY_MENU_H

#include "unit/menu/menu.h"
#include "global_cells.h"
#include "layout_guard.h"
#include "menu_result.h"
#include "render_input.h"
#include "text/msg_ids.h"

struct AnimationObject;

struct BattleUnit;

struct PsynergyMenuState {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[4];
  s32 auxiliaryWindow;
  struct RenderOutput *paneIcon[2];
  s8 tabIndex[2];
  s8 tabCounts[2];
  s32 psynergyWindow;
  s32 statusWindow;
  s32 shortcutWindow;
  struct UiWindow *infoWindow;
  u8 unk030[4];
  s32 listWindow;
  u8 unk038[0x0c];
  struct RenderOutput *entryGridCursor;
  struct RenderOutput *entryIcons[32];
  u8 unk0c8[0x44];
  s32 messageWindow;
  u8 selectedColumn;
  u8 selectedRow;
  u8 columnCount;
  u8 rowCount;
  struct AnimationObject *tabObjects[8];
  s16 tabX[8];
  s16 rowPositions[8];
  s32 ownerScale[8];
  u16 paneRow[2];
  u16 paneAction[2];
  u8 unk17c[0x4c];
  u16 psynergies[32];
  u16 ownerTable[8];
  u8 psynergyCount;
  u8 ownerCount;
  u8 ownerIds[2];
  struct RenderOutput *cursorIcon;
  u16 flags;
  u16 skipSlide;
  u8 unk224[0x18];
  s16 slotY[4];
  u8 unk244[0x16];
  s16 useResult;
  u8 unk25c[4];
  s8 selectedIndexByOwner[8];
  u8 mode;
};

LAYOUT_OFFSET_GUARD(PsynergyMenuState_AuxiliaryWindow, struct PsynergyMenuState,
                    auxiliaryWindow, 0x010);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_PsynergyWindow, struct PsynergyMenuState,
                    psynergyWindow, 0x020);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_EntryGridCursor, struct PsynergyMenuState,
                    entryGridCursor, 0x044);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_EntryIcons, struct PsynergyMenuState,
                    entryIcons, 0x048);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_MessageWindow, struct PsynergyMenuState,
                    messageWindow, 0x10c);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_SelectedColumn, struct PsynergyMenuState,
                    selectedColumn, 0x110);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_SelectedRow, struct PsynergyMenuState,
                    selectedRow, 0x111);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_ColumnCount, struct PsynergyMenuState,
                    columnCount, 0x112);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_RowCount, struct PsynergyMenuState,
                    rowCount, 0x113);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_SelectedPsynergy,
                    struct PsynergyMenuState, paneAction[0], 0x178);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_Psynergies, struct PsynergyMenuState,
                    psynergies, 0x1c8);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_PsynergyCount, struct PsynergyMenuState,
                    psynergyCount, 0x218);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_OwnerIds, struct PsynergyMenuState,
                    ownerIds, 0x21a);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_SelectedIndexByOwner,
                    struct PsynergyMenuState, selectedIndexByOwner, 0x260);

LAYOUT_OFFSET_GUARD(PsynergyMenuState_SkipSlide, struct PsynergyMenuState,
                    skipSlide, 0x222);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_TabObjects, struct PsynergyMenuState,
                    tabObjects, 0x114);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_OwnerScale, struct PsynergyMenuState,
                    ownerScale, 0x154);

#endif
