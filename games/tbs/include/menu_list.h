#ifndef ALCHEMY_MENU_LIST_H
#define ALCHEMY_MENU_LIST_H

#include "unit/window/window.h"
#include "unit/window/select.h"
#include "unit/window/output.h"
#include "unit/window/save.h"
#include "unit/window/btlcmd.h"
#include "unit/window/debug.h"
#include "types.h"
#include "resource.h"
#include "callback_scheduler.h"
#include "tbs_edition.h"
#include "text_render_runtime.h"
#include "window.h"
#include "layout_guard.h"
#include "keystate.h"
#include "farcall.h"
#include "frame.h"

enum
{
	BATTLE_LIST_ROWS = 5
};

struct MenuSprite
{
	struct MenuSprite *next;
	union
	{
		struct
		{
			unsigned long attr01;
			unsigned long attr23;
		} word;
		struct
		{
			u16 y : 8;
			u16 affine : 2;
			u16 mode : 2;
			u16 mosaic : 1;
			u16 colors : 1;
			u16 shape : 2;
			u16 x : 9;
			u16 affine_index : 5;
			u16 size : 2;
			u16 tile : 10;
			u16 priority : 2;
			u16 palette : 4;
			u16 unused;
		} f;
	} oam;
};

struct CursorSprite
{
	struct CursorSprite *next;
	u16					 y : 8;
	u16					 affine : 2;
	u16					 mode : 2;
	u16					 mosaic : 1;
	u16					 colors : 1;
	u16					 shape : 2;
	u16					 x : 9;
	u16					 affine_index : 5;
	u16					 size : 2;
	u16					 tile : 10;
	u16					 priority : 2;
	u16					 palette : 4;
	u16					 unused;
};

#define MENU_SPRITE_SIZE_16 0x40000000

// スプライトの形 (attr0/attr1) を決め、残りを 0 にする
/* FAKEMATCH: the one-pass block keeps both attribute words ahead of the stores that follow (the x/y fields in the list setup loops of ItemList_SelectEntry, BattleMenu_RunActionSelection and SummonMenu_SelectSummon; the tile in UiText_DrawCharacter). Not source: the same two stores match only as plain statements in Battle_CollectPartyCommands (resmenu.c, five times) and Ui_RunOwnerStatusScreen (preview.c, three times). */
#define MenuSprite_Init(sprite, shape)      \
	do                                      \
	{                                       \
		(sprite).oam.word.attr01 = (shape); \
		(sprite).oam.word.attr23 = 0;       \
	} while (0)

struct PartyStatusWindow
{
	struct UiWindow *window;
	u16				 x;
	u16				 y;
	u16				 width;
	u16				 height;
	u16				 flags;
};

LAYOUT_SIZE_GUARD(PartyStatusWindow_Size, struct PartyStatusWindow, 16);
LAYOUT_OFFSET_GUARD(PartyStatusWindow_Flags, struct PartyStatusWindow, flags, 12);

struct MenuPoint
{
	s32 x;
	s32 y;
};

struct MenuCell
{
	u8	  unknown_00[0x28];
	s32	  slide_offset;
	u8	  unknown_2c[4];
	s32	  row;
	s32	  page;
	s32	  preferred_row;
	s32	  command_selection[2];
	void *work;
	s32	  busy;
	s32	  active;
	u8	  unknown_50[0x88];
	s32	  auto_enabled;
	s32	  auto_delay;
	s32	  auto_mode;
};

LAYOUT_OFFSET_GUARD(MenuCell_Slide, struct MenuCell, slide_offset, 0x28);
LAYOUT_OFFSET_GUARD(MenuCell_Row, struct MenuCell, row, 0x30);
LAYOUT_OFFSET_GUARD(MenuCell_CommandSelection, struct MenuCell, command_selection, 0x3c);
LAYOUT_OFFSET_GUARD(MenuCell_Work, struct MenuCell, work, 0x44);
LAYOUT_OFFSET_GUARD(MenuCell_Busy, struct MenuCell, busy, 0x48);
LAYOUT_OFFSET_GUARD(MenuCell_Active, struct MenuCell, active, 0x4c);
LAYOUT_OFFSET_GUARD(MenuCell_AutoEnabled, struct MenuCell, auto_enabled, 0xd8);
LAYOUT_OFFSET_GUARD(MenuCell_AutoDelay, struct MenuCell, auto_delay, 0xdc);
LAYOUT_OFFSET_GUARD(MenuCell_AutoMode, struct MenuCell, auto_mode, 0xe0);

// 前に選んでいた位置 (ページ、行、戻る行) を読み戻す
/* A statement macro (S2: a layer, 4 sites): the three cursor reads stay together at every use; as a plain block, tbs-en differs. */
#define MenuCell_GetCursor(page_, row_, preferred_)                         \
	do                                                                      \
	{                                                                       \
		(page_) = ((struct MenuCell *)gWindowWork[42])->page;               \
		(row_) = ((struct MenuCell *)gWindowWork[42])->row;                 \
		(preferred_) = ((struct MenuCell *)gWindowWork[42])->preferred_row; \
	} while (0)

// 選んでいる位置 (ページ、行、戻る行) を書き残す
/* FAKEMATCH: the do { } while (0) wrapper steers, so this is a device at each of its 7 sites, not a layer (S2: not every use matches through it). Battle_SelectAbility writes the same three stores plainly through its already loaded menu pointer, and through this macro it differs 2 in all six editions (measured 2026-10-13). At the 7 sites the work-cell pointer it loads stays apart from the reload after it, and the three stores finish before the keys are read; written as three plain stores, with the dead-store pads gone, SummonMenu_SelectSummon differs 84 (stores through its nav pointer; through gLinkCountdownWork, the macro's own stores unwrapped, it differs 4), ItemList_SelectEntry 46 and BattleMenu_RunActionSelection 42 in tbs-en, and elsewhere the row store goes first (differ 4) or the base is loaded before mode = 0 (differ 2 to 4). */
#define MenuCell_SetCursor(page_, row_, preferred_)       \
	do                                                    \
	{                                                     \
		gLinkCountdownWork->page = (page_);               \
		gLinkCountdownWork->row = (row_);                 \
		gLinkCountdownWork->preferred_row = (preferred_); \
	} while (0)

extern struct MenuCell *gLinkCountdownWork;
extern u8				Resource_FixedBlockBTiles[];
extern const u8			Menu_AnimatedCursorTiles[];

#endif
