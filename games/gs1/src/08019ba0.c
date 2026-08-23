#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 UiText_BuildRenderEntries(s32, s32);

s32 Func_08019ba0(s32 arg0) {
    return UiText_BuildRenderEntries(arg0, 1);
}
