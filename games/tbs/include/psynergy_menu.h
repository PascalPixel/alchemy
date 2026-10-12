#ifndef ALCHEMY_PSYNERGY_MENU_H
#define ALCHEMY_PSYNERGY_MENU_H

#include "unit/menu/item/menu.h"
#include "unit/menu/item/menu8.h"
#include "global_cells.h"
#include "layout_guard.h"
#include "menu_result.h"
#include "render_input.h"
#include "text/msg_ids.h"

struct AnimationObject;

struct BtlUnit;

struct PsynergyMenuState
{
	u8						unknown_000[8];
	s32						selected_owner;
	u8						unknown_00c[4];
	s32						auxiliary_window;
	struct RenderOutput	   *pane_icon[2];
	s8						tab_index[2];
	s8						tab_counts[2];
	s32						psynergy_window;
	s32						status_window;
	s32						shortcut_window;
	struct UiWindow		   *info_window;
	u8						unknown_030[4];
	s32						list_window;
	u8						unknown_038[0x0c];
	struct RenderOutput	   *entry_grid_cursor;
	struct RenderOutput	   *entry_icons[32];
	u8						unknown_0c8[0x44];
	s32						message_window;
	u8						selected_column;
	u8						selected_row;
	u8						column_count;
	u8						row_count;
	struct AnimationObject *tab_objects[8];
	s16						tab_x[8];
	s16						row_positions[8];
	s32						owner_scale[8];
	u16						pane_row[2];
	u16						pane_action[2];
	u8						unknown_17c[0x4c];
	u16						psynergies[32];
	u16						owner_table[8];
	u8						psynergy_count;
	u8						owner_count;
	u8						owner_ids[2];
	struct RenderOutput	   *cursor_icon;
	u16						flags;
	u16						skip_slide;
	u8						unknown_224[0x18];
	s16						slot_y[4];
	u8						unknown_244[0x16];
	s16						use_result;
	u8						unknown_25c[4];
	s8						selected_index_by_owner[8];
	u8						mode;
};

LAYOUT_OFFSET_GUARD(
	PsynergyMenuState_AuxiliaryWindow, struct PsynergyMenuState, auxiliary_window, 0x010);
LAYOUT_OFFSET_GUARD(
	PsynergyMenuState_PsynergyWindow, struct PsynergyMenuState, psynergy_window, 0x020);
LAYOUT_OFFSET_GUARD(
	PsynergyMenuState_EntryGridCursor, struct PsynergyMenuState, entry_grid_cursor, 0x044);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_EntryIcons, struct PsynergyMenuState, entry_icons, 0x048);
LAYOUT_OFFSET_GUARD(
	PsynergyMenuState_MessageWindow, struct PsynergyMenuState, message_window, 0x10c);
LAYOUT_OFFSET_GUARD(
	PsynergyMenuState_SelectedColumn, struct PsynergyMenuState, selected_column, 0x110);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_SelectedRow, struct PsynergyMenuState, selected_row, 0x111);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_ColumnCount, struct PsynergyMenuState, column_count, 0x112);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_RowCount, struct PsynergyMenuState, row_count, 0x113);
LAYOUT_OFFSET_GUARD(
	PsynergyMenuState_SelectedPsynergy, struct PsynergyMenuState, pane_action[0], 0x178);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_Psynergies, struct PsynergyMenuState, psynergies, 0x1c8);
LAYOUT_OFFSET_GUARD(
	PsynergyMenuState_PsynergyCount, struct PsynergyMenuState, psynergy_count, 0x218);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_OwnerIds, struct PsynergyMenuState, owner_ids, 0x21a);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_SelectedIndexByOwner, struct PsynergyMenuState,
	selected_index_by_owner, 0x260);

LAYOUT_OFFSET_GUARD(PsynergyMenuState_SkipSlide, struct PsynergyMenuState, skip_slide, 0x222);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_TabObjects, struct PsynergyMenuState, tab_objects, 0x114);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_OwnerScale, struct PsynergyMenuState, owner_scale, 0x154);

#endif
