/* differ 4: the native loads the window work before building the resource-id offset; only ARM9TDMI load scheduling differs */
#include "types.h"
#include "graphics.h"
#include "window.h"
#include "ram_buffer.h"

extern const s16 Data_0805eb58[];
extern const s16 Data_0805eb7c[];

struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
s32 CreateSideObject(s32 id, s32 kind, s32 slot, struct UiWindow *window, s32 x, s32 y);

void UiWork_SetTwoEntriesTo999(void)
{
    struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
    s32 index = 0;

    do {
        work->resource_ids[index] = 999;
        index++;
    } while (index != 2);
}
