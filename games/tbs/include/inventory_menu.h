#ifndef ALCHEMY_INVENTORY_MENU_H
#define ALCHEMY_INVENTORY_MENU_H

#include "unit/menu/item/menu.h"
#include "unit/menu/item/menu2.h"
#include "unit/menu/item/menu3.h"
#include "unit/menu/item/menu4.h"
#include "unit/menu/item/menu5.h"
#include "unit/menu/item/menu6.h"
#include "unit/menu/item/menu7.h"
#include "unit/menu/item/menu8.h"
#include "global_cells.h"
#include "layout_guard.h"
#include "menu_result.h"
#include "render_input.h"
#include "text/msg_ids.h"
#include "farcall.h"

struct AnimationObject;

struct UiWindow;

struct InventoryMenuState
{
	u8						unknown_000[8];
	s32						selected_owner;
	u8						unknown_00c[4];
	struct UiWindow		   *main_window;
	struct RenderOutput	   *pane_icons[2];
	s8						pane_index[2];
	s8						pane_count[2];
	struct UiWindow		   *item_window;
	struct UiWindow		   *status_window;
	struct UiWindow		   *help_window;
	struct UiWindow		   *info_window;
	struct UiWindow		   *equip_window;
	struct UiWindow		   *list_window;
	struct UiWindow		   *unknown_window_38;
	struct UiWindow		   *modal_window;
	struct UiWindow		   *unknown_window_40;
	struct RenderOutput	   *grid_cursor;
	struct RenderOutput	   *entry_icons[32];
	struct RenderOutput	   *category_icons[8];
	u8						unknown_0e8[0x24];
	struct UiWindow		   *message_window;
	u8						selected_column;
	u8						selected_row;
	u8						column_count;
	u8						row_count;
	struct AnimationObject *owner_objects[8];
	s16						owner_x[8];
	s16						owner_y[8];
	s32						owner_scale[8];
	u16						selected_slots[2];
	u16						selected_items[2];
	struct RenderOutput	   *cursor;
	u8						unknown_180[0x48];
	u16						items[32];
	u16						owner_ids[8];
	u8						item_count;
	u8						party_count;
	u8						pane_owner[2];
	struct RenderOutput	   *selected_item_icon;
	u16						flags;
	s16						completion_flag;
	u8						unknown_224[0x36];
	s16						message_offset;
	s8						equip_preview;
	u8						list_mode;
	u8						unknown_25e[2];
	s8						selected_index_by_owner[8];
	u8						mode;
	u8						unknown_269[0x129];
	s16						resource_slots[2];
};

LAYOUT_OFFSET_GUARD(InventoryMenuState_PaneIcons, struct InventoryMenuState, pane_icons, 0x14);
LAYOUT_OFFSET_GUARD(InventoryMenuState_PaneIndex, struct InventoryMenuState, pane_index, 0x1c);
LAYOUT_OFFSET_GUARD(InventoryMenuState_PaneCount, struct InventoryMenuState, pane_count, 0x1e);
LAYOUT_OFFSET_GUARD(InventoryMenuState_ItemWindow, struct InventoryMenuState, item_window, 0x20);
LAYOUT_OFFSET_GUARD(InventoryMenuState_InfoWindow, struct InventoryMenuState, info_window, 0x2c);
LAYOUT_OFFSET_GUARD(InventoryMenuState_GridCursor, struct InventoryMenuState, grid_cursor, 0x44);
LAYOUT_OFFSET_GUARD(InventoryMenuState_EntryIcons, struct InventoryMenuState, entry_icons, 0x48);
LAYOUT_OFFSET_GUARD(
	InventoryMenuState_CategoryIcons, struct InventoryMenuState, category_icons, 0xc8);
LAYOUT_OFFSET_GUARD(
	InventoryMenuState_MessageWindow, struct InventoryMenuState, message_window, 0x10c);
LAYOUT_OFFSET_GUARD(
	InventoryMenuState_SelectedColumn, struct InventoryMenuState, selected_column, 0x110);
LAYOUT_OFFSET_GUARD(InventoryMenuState_SelectedRow, struct InventoryMenuState, selected_row, 0x111);
LAYOUT_OFFSET_GUARD(InventoryMenuState_ColumnCount, struct InventoryMenuState, column_count, 0x112);
LAYOUT_OFFSET_GUARD(InventoryMenuState_RowCount, struct InventoryMenuState, row_count, 0x113);
LAYOUT_OFFSET_GUARD(
	InventoryMenuState_OwnerObjects, struct InventoryMenuState, owner_objects, 0x114);
LAYOUT_OFFSET_GUARD(InventoryMenuState_OwnerX, struct InventoryMenuState, owner_x, 0x134);
LAYOUT_OFFSET_GUARD(InventoryMenuState_OwnerY, struct InventoryMenuState, owner_y, 0x144);
LAYOUT_OFFSET_GUARD(InventoryMenuState_OwnerScale, struct InventoryMenuState, owner_scale, 0x154);
LAYOUT_OFFSET_GUARD(
	InventoryMenuState_SelectedSlots, struct InventoryMenuState, selected_slots, 0x174);
LAYOUT_OFFSET_GUARD(
	InventoryMenuState_SelectedItems, struct InventoryMenuState, selected_items, 0x178);
LAYOUT_OFFSET_GUARD(InventoryMenuState_Cursor, struct InventoryMenuState, cursor, 0x17c);
LAYOUT_OFFSET_GUARD(InventoryMenuState_Items, struct InventoryMenuState, items, 0x1c8);
LAYOUT_OFFSET_GUARD(InventoryMenuState_OwnerIds, struct InventoryMenuState, owner_ids, 0x208);
LAYOUT_OFFSET_GUARD(InventoryMenuState_ItemCount, struct InventoryMenuState, item_count, 0x218);
LAYOUT_OFFSET_GUARD(InventoryMenuState_PaneOwner, struct InventoryMenuState, pane_owner, 0x21a);
LAYOUT_OFFSET_GUARD(
	InventoryMenuState_SelectedItemIcon, struct InventoryMenuState, selected_item_icon, 0x21c);
LAYOUT_OFFSET_GUARD(
	InventoryMenuState_MessageOffset, struct InventoryMenuState, message_offset, 0x25a);
LAYOUT_OFFSET_GUARD(
	InventoryMenuState_EquipPreview, struct InventoryMenuState, equip_preview, 0x25c);
LAYOUT_OFFSET_GUARD(
	InventoryMenuState_SelectedIndex, struct InventoryMenuState, selected_index_by_owner, 0x260);
LAYOUT_OFFSET_GUARD(InventoryMenuState_Mode, struct InventoryMenuState, mode, 0x268);
LAYOUT_OFFSET_GUARD(
	InventoryMenuState_ResourceSlots, struct InventoryMenuState, resource_slots, 0x392);
LAYOUT_SIZE_GUARD(InventoryMenuState_Size, struct InventoryMenuState, 0x398);

enum ItemMenuCommand
{
	ITEM_COMMAND_USE,
	ITEM_COMMAND_EQUIP,
	ITEM_COMMAND_INSPECT,
	ITEM_COMMAND_GIVE,
	ITEM_COMMAND_REMOVE,
	ITEM_COMMAND_DROP
};

struct BtlUnit;

#endif
