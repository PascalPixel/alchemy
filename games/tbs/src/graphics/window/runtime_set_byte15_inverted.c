#include "types.h"
#include "render_input.h"

void RenderOutput_SetInvertedSentinel(void *arg0, s32 arg1)
{
    if (arg0 != 0)
        ((struct RenderOutput *)arg0)->sentinel = ~arg1;
}
