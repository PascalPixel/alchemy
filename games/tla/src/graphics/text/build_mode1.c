#include "types.h"

s32 UiText_BuildRenderEntries(s32 message, s32 mode);

s32 UiText_BuildRenderEntriesMode1(s32 message)
{
    return UiText_BuildRenderEntries(message, 1);
}
