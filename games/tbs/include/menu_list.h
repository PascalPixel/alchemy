#ifndef ALCHEMY_MENU_LIST_H
#define ALCHEMY_MENU_LIST_H

#include "types.h"
#include "resource.h"
#include "callback_scheduler.h"
#include "tbs_edition.h"
#include "text_render_runtime.h"
#include "window.h"
#include "layout_guard.h"
#include "keystate.h"
#include "farcall.h"

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

extern struct MenuCell *gLinkCountdownWork;
extern volatile u32		gFrameCount;
extern u8				Resource_FixedBlockBTiles[];
extern const u8			Menu_AnimatedCursorTiles[];

void WaitFrames(s32 frames);
void Runtime_SetMainState19(void);
void RenderOutput_RedrawSavedRect(struct UiWindow *window);
void RenderOutput_RedrawSavedRectFar(struct UiWindow *window);
void RenderOutput_ClearListFar(struct UiWindow *window);
s32	 UiText_CopyMessageString(s32 message, u16 *buffer, u32 count);
void UiText_RenderWideStringAtOffset(u16 *buffer, struct UiWindow *window, s32 x, s32 y);
void UiWindow_SetTilemapEntry(struct UiWindow *window, s32 tile, s32 x, s32 y, u32 layer);
void UiWork_SetParamNibble(s32 param);
void Ui_SetRectHighlight(s32 x, s32 y, s32 width, s32 height, s32 value);
s32	 Ui_FillVramBlockPattern(void);
void Vram_CopyTile(u32 source_tile, u32 destination_tile);

#endif
