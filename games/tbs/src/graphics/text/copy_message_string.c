#include "types.h"
#include "window.h"

s32 UiText_CopyMessageString(s32 key, u16 *destination, u32 capacity)
{
    struct UiRenderWork *work;
    u32 count;

    work = (struct UiRenderWork *)gWindowWork[0];
    work->count = 0;
    UiText_BuildRenderEntries(key, 1);
    for (count = 0; count < capacity - 1 &&
        (destination[count] = work->entries[count]) != 0; count++) {
    }
    destination[count] = 0;
    return count;
}
