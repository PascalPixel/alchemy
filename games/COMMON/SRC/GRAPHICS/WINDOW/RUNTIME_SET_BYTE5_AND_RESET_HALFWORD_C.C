//--------------------------------------------------------------------------
// RUNTIME_SET_BYTE5_AND_RESET_HALFWORD_C.C
// 表示：スプライト出力のアニメを切りかえる
//--------------------------------------------------------------------------

#include "TYPES.H"
#include "RENDER_INPUT.H"

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
