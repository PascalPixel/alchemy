#include "types.h"

#define UiText_BuildRenderEntriesMode1 Func_08019ba0

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 UiText_BuildRenderEntries(s32, s32);

s32 UiText_BuildRenderEntriesMode1(s32 arg0) {
    return UiText_BuildRenderEntries(arg0, 1);
}
