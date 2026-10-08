//--------------------------------------------------------------------------
// is_complete.c
// 表示：メッセージのチャンネルがすべて書き終えたか
//--------------------------------------------------------------------------

#include "types.h"
#include "edition.h"
#include "window.h"
#if GAME_TLA
#include "ram_buffer.h"
#else
#include "global_cells.h"
#endif

//--------------------------------------------------------------------------
// 3 つのチャンネルのうち、ウィンドウがあって書き終えていない（state が 0）
// ものがあれば 0、なければ 1 を返す
//--------------------------------------------------------------------------
s32 UiWork_IsComplete(void)
{
	struct UiChannelSlot *ch;
	s32					  i;

#if GAME_TLA
	ch = ((struct UiRenderWork *)Ram_HeapSlots->window_tiles)->channels;
#else
	ch = ((struct UiRenderWork *)gWindowWork[0])->channels;
#endif
	for (i = 0; i != 3; i++, ch++)
	{
		if (ch->work != NULL && ch->work->state == 0)
			return 0;
	}
	return 1;
}
