#include "types.h"
#include "call.h"

void Func_02004214(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height);
void Func_0200423c(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);

void Func_02000d88(void)
{
    Call6((void (*)())Func_02004214, 46, 68, 40, 73, 5, 6);
    Call6((void (*)())Func_0200423c, 104, 9, 5, 6, 40, 9);
}
