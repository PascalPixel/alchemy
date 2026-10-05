#include "TYPES.H"
#include "RENDER_INPUT.H"

void RenderOutput_SetAnimation(struct RenderOutput *output, s32 mode)
{
    if (output != NULL) {
        output->active = mode;
        output->anim_step = 0;
    }
}
