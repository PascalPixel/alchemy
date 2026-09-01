#include "types.h"

struct PsynergyPageIcon {
    u8 reserved_00[5];
    u8 state;
    u16 x;
    u16 y;
};

struct PsynergyPageMenu {
    u8 reserved_000[0x48];
    struct PsynergyPageIcon *icons[32];
    u8 reserved_0c8[0x150];
    u8 entry_count;
};

extern struct PsynergyPageMenu *Data_03001f2c;

void Func_080a17c4(struct PsynergyPageIcon *icon);

#define Menu_SetPageIcons Func_080a2324

void Menu_SetPageIcons(
    s32 page_size,
    s32 first,
    s32 window,
    s32 x,
    s32 y)
{
    struct PsynergyPageMenu *menu = Data_03001f2c;
    struct PsynergyPageIcon **cursor;
    struct PsynergyPageIcon *icon;
    s32 current;
    s32 end;

    {
        struct PsynergyPageIcon **clear_cursor;
        s32 clear_state;

        clear_state = 13;
        clear_cursor = menu->icons;
        current = 31;
        do {
            icon = *clear_cursor++;
            if (icon != 0)
                icon->state = clear_state;
            current--;
        } while (current >= 0);
    }

    current = first;
    {
        s32 initial_end;

        initial_end = page_size + current;
        if (current >= initial_end)
            return;
        icon = menu->icons[current];
        if (icon == 0)
            return;
        if (current > (s32)(menu->entry_count - 1))
            return;
        end = initial_end;
    }
    {
        s32 draw_y;

        cursor = current + menu->icons;
        draw_y = y;
        do {
            icon->x = x;
            icon->y = draw_y;
            Func_080a17c4(icon);
            current++;
            icon->state = 1;
            draw_y += 16;
            if (current >= end)
                return;
            cursor++;
            icon = *cursor;
            if (icon == 0)
                return;
        } while (current <= (s32)(menu->entry_count - 1));
    }

    (void)window;
}
