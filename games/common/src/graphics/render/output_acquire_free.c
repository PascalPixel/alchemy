//--------------------------------------------------------------------------
// output_acquire_free.c
// 表示：スプライト出力（RenderOutput）のあきリスト
//--------------------------------------------------------------------------

#include "types.h"
#include "edition.h"
#include "window.h"
#if GAME_TLA
#include "ram_buffer.h"
#endif

#if GAME_TLA
#define UI_WORK ((struct UiRenderWork *)Ram_HeapSlots->window_tiles)
#else
#define UI_WORK ((struct UiRenderWork *)gWindowWork[0])
#endif

//--------------------------------------------------------------------------
// あきリストの先頭をとり出す（なければ NULL）
//--------------------------------------------------------------------------
struct RenderOutput *RenderOutput_AcquireFree(void)
{
	struct UiRenderWork *work = UI_WORK;
	struct RenderOutput *out = work->free_outputs.head;

	if (out != NULL)
	{
		if (out->next == NULL)
			work->free_outputs.tail_link = &work->free_outputs.head; // 空になった
		work->free_outputs.head = out->next;
		out->next = NULL;
	}
	return out;
}

//--------------------------------------------------------------------------
// あきリストの最後に返す（outputs[] の外のものは返さない）
//--------------------------------------------------------------------------
void RenderOutput_ReleaseFree(struct RenderOutput *out)
{
	struct UiRenderWork *work = UI_WORK;

	if ((u32)out >= (u32)work->outputs && (u32)out < (u32)&work->free_outputs.head)
	{
		struct RenderOutput **tail = work->free_outputs.tail_link;

		work->free_outputs.tail_link = &out->next;
		*tail = out;
		out->next = NULL;
	}
}

//--------------------------------------------------------------------------
// outputs[] の 64 個をすべてつないで、あきリストにする
//--------------------------------------------------------------------------
void UiWork_InitFreeList(void)
{
	s32					 n;
	struct UiRenderWork *work;
	struct RenderOutput *out;
	struct RenderOutput *next;

	work = UI_WORK;
	out = work->outputs;
	work->free_outputs.head = out;
	n = UI_OUTPUT_COUNT - 2;
	do
	{
		next = out + 1;
		n--;
		out->next = next;
		out = next;
	} while (n >= 0);
	next->next = NULL;
	work->free_outputs.tail_link = &next->next;
}
