#ifndef ALCHEMY_CURSOR_H
#define ALCHEMY_CURSOR_H

#include "types.h"
#include "layout_guard.h"
#include "render_input.h"

// 目標へ寄っていくカーソル（店、名前入力、設定の画面で同じもの）
struct ShopCursor
{
	struct RenderOutput *anchor;
	u16					 x;
	u16					 y;
	s16					 target_x;
	s16					 target_y;
	s8					 active;
	s8					 kind;
};

LAYOUT_SIZE_GUARD(ShopCursor_Size, struct ShopCursor, 0x10);
LAYOUT_OFFSET_GUARD(ShopCursor_TargetX, struct ShopCursor, target_x, 0x08);
LAYOUT_OFFSET_GUARD(ShopCursor_Active, struct ShopCursor, active, 0x0c);

// 絵のポインタだけを書く画面は union を通す（store が並べ替えられない）
union MenuCursor
{
	struct RenderOutput *output;
	struct ShopCursor	 cursor;
};

#endif
