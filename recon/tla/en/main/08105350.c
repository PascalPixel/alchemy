#include "types.h"
#include "ram_buffer.h"

struct MenuIconWork {
    u8 unknown_000[0x220];
    void *objects[4];
    s16 x[4];
    s16 y[4];
    s32 scale[4];
    s16 frame[4];
};

void MenuIcon_SetScale(s32 index, s32 scale)
{
    struct MenuIconWork *menu = Ram_HeapSlots->menu_runtime;



    if (menu->objects[index] != NULL)
        menu->scale[index] = scale;
}
