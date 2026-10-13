//--------------------------------------------------------------------------
// runtime_set_byte5_and_reset_halfword_c.c
// 表示：スプライト出力のアニメを切りかえる
//--------------------------------------------------------------------------

#include "types.h"
#include "render_input.h"

//--------------------------------------------------------------------------
// active を mode にして、アニメの段（anim_step）を 0 に戻す
// out : スプライト出力（NULL なら何もしない）
//--------------------------------------------------------------------------
void RenderOutput_SetAnimation(struct RenderOutput *out, s32 mode)
{
	if (out != NULL)
	{
		out->active = mode;
		out->anim_step = 0;
	}
}
