#include "types.h"

struct MessageWork {
    u8 unknown_000[0xeb0];
    u16 text[513];
    u16 count;
};

extern struct MessageWork *Data_03001e8c;
s32 UiText_BuildRenderEntries(s32 key, s32 mode);

s32 UiText_CopyMessageString(s32 key, u16 *destination, u32 capacity)
{
    struct MessageWork *work;
    u32 count;

    work = Data_03001e8c;
    work->count = 0;
    UiText_BuildRenderEntries(key, 1);
    for (count = 0; count < capacity - 1 &&
        (destination[count] = work->text[count]) != 0; count++) {
    }
    destination[count] = 0;
    return count;
}
