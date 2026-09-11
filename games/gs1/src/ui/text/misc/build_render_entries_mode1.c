#include "types.h"
#include "scene.h"


s32 UiText_BuildRenderEntries(s32, s32);

s32 UiText_BuildRenderEntriesMode1(s32 arg0)
{
    return UiText_BuildRenderEntries(arg0, 1);
}
