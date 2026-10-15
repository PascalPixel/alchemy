#include "types.h"
#include "ram_buffer.h"

struct MenuArrow
{
    u8  unk00[5];
    u8  mode;
    u8  unk06[6];
    u16 timer;
};

struct MenuPageWork
{
    u8                unk000[0x178];
    struct MenuArrow *up;
    struct MenuArrow *down;
};

void Menu_SetPageArrowsA(s32 index, s32 count)
{
    struct MenuPageWork *menu = Ram_HeapSlots->menuRuntime;
    struct MenuArrow    *arrow;

    if (index < 0)
        index += 3;
    index = index >> 2 << 2;
    arrow = menu->up;
    arrow->mode = 13;
    if (index != 0)
    {
        arrow->mode = 17;
        arrow->timer = 0;
    }
    arrow = menu->down;
    arrow->mode = 13;
    if (index + 4 < count)
    {
        arrow->mode = 15;
        arrow->timer = 0;
    }
}

void Menu_SetPageArrowsB(s32 index, s32 count)
{
    struct MenuPageWork *menu = Ram_HeapSlots->menuRuntime;
    struct MenuArrow    *arrow;

    if (index < 0)
        index += 3;
    index = index >> 2 << 2;
    arrow = menu->up;
    arrow->mode = 13;
    if (index != 0)
    {
        arrow->mode = 16;
        arrow->timer = 0;
    }
    arrow = menu->down;
    arrow->mode = 13;
    if (index + 4 < count)
    {
        arrow->mode = 14;
        arrow->timer = 0;
    }
}
