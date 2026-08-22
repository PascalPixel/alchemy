#include "text_render_runtime.h"
#include "types.h"

u8 *Func_08017dd4(u8 *, s32, s32);

void Func_0801e9d4(
    s32 arg0,
    s32 arg1,
    struct TextRenderWork *arg2,
    s32 arg3,
    s32 arg4)
{
    u8 data[16];

    /* 16バイト一時領域を介して次の処理へ渡す。 */
    Func_0801e8b0(Func_08017dd4(data, arg0, arg1), arg2, arg3, arg4);
}
