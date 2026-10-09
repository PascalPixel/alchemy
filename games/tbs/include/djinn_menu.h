#ifndef ALCHEMY_DJINN_MENU_H
#define ALCHEMY_DJINN_MENU_H

#include "types.h"
#include "global_cells.h"
#include "render_input.h"

#define DJINN_ENTRY_INDEX 31
#define DJINN_ENTRY_ELEMENT 0xe0
#define DJINN_ENTRY_OWNER 0xf00
#define DJINN_ENTRY_IS_SET 0x8000

#define DJINN_ELEMENTS 4
#define DJINN_PER_ELEMENT 20
#define DJINN_LIST_OWNERS 8
#define DJINN_LIST_ROWS 10

struct DjinnListTable
{
	u16 ids[DJINN_LIST_OWNERS][DJINN_LIST_ROWS];
	s8	counts[DJINN_LIST_OWNERS];
};

struct DjinnMenuBackdrop
{
	struct DjinnListTable list;
	u8					  tiles[0x2000];
	u16					  palette[64];
	s32					  tutorial_timer;
	long				  tutorial_step;
};

struct DjinnMenuWork
{
	u8						  unknown_000[8];
	s32						  owner;
	u8						  unknown_00c[4];
	s32						  status_window;
	struct RenderOutput		 *cursor;
	u8						  unknown_018[4];
	s8						  selected_column[2];
	u8						  unknown_01e[6];
	s32						  window;
	s32						  unknown_window_028;
	u8						  unknown_02c[4];
	s32						  list_window;
	s32						  second_window;
	u8						  unknown_038[0xd4];
	s32						  help_window;
	u8						  unknown_110[0x24];
	s16						  owner_x[8];
	s16						  owner_y[8];
	u8						  unknown_154[0x20];
	u16						  selection[2];
	u16						  pair_entries[2];
	struct RenderOutput		 *second_cursor;
	u8						  unknown_180[0x04];
	struct DjinnMenuBackdrop *backdrop;
	u8						  unknown_188[0x80];
	u16						  owners[DJINN_LIST_OWNERS];
	u8						  pair_count;
	u8						  owner_count;
	u8						  shown_owner[2];
	u8						  unknown_21c[4];
	u16						  pending;
	u8						  unknown_222[0x12];
	u16						  slot_x[4];
	u16						  slot_y[4];
	u8						  unknown_244[0x10];
	u8						  pair_index[2];
	u8						  pair_element[2];
	u8						  pair_owner[2];
};

LAYOUT_OFFSET_GUARD(DjinnMenuWork_StatusWindow, struct DjinnMenuWork, status_window, 0x10);
LAYOUT_OFFSET_GUARD(DjinnMenuWork_SelectedColumn, struct DjinnMenuWork, selected_column, 0x1c);
LAYOUT_OFFSET_GUARD(DjinnMenuWork_OwnerX, struct DjinnMenuWork, owner_x, 0x134);
LAYOUT_OFFSET_GUARD(DjinnMenuWork_OwnerY, struct DjinnMenuWork, owner_y, 0x144);
LAYOUT_OFFSET_GUARD(DjinnMenuWork_Selection, struct DjinnMenuWork, selection, 0x174);
LAYOUT_OFFSET_GUARD(DjinnMenuWork_Pending, struct DjinnMenuWork, pending, 0x220);
LAYOUT_OFFSET_GUARD(DjinnMenuWork_PairIndex, struct DjinnMenuWork, pair_index, 0x254);
LAYOUT_OFFSET_GUARD(
	DjinnMenuBackdrop_TutorialTimer, struct DjinnMenuBackdrop, tutorial_timer, 0x2128);
LAYOUT_OFFSET_GUARD(
	DjinnMenuBackdrop_TutorialStep, struct DjinnMenuBackdrop, tutorial_step, 0x212c);

#endif
