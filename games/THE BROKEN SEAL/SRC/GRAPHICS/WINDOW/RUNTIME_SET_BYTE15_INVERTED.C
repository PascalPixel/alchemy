#include "TYPES.H"
#include "RENDER_INPUT.H"

void UiWork_SetByte15Inverted(void *arg0, s32 arg1)
{
    if (arg0 != 0)
        ((struct RenderOutput *)arg0)->sentinel = ~arg1;
}
