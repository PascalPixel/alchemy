#include "types.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"

struct MenuEntryWindow {
    u8 unknown_00[12];
    s16 x;
    s16 y;
};

struct MenuEntryWork {
    u8 unknown_000[0xf8];
    void *objects[8];
    s16 x[8];
    s16 y[8];
    s32 scale[8];
    u8 unknown_158[0x509 - 0x158];
    u8 active;
};

void Menu_CreateOwnerEntryObjects(s32 first);
void Menu_UpdateEntryObjectTransforms(void);

void Menu_PlaceOwnerEntryObjects(struct MenuEntryWindow *window, s32 origin_x, s32 origin_y)
{
    struct MenuEntryWork *menu = Ram_HeapSlots->menu_runtime;
    s32 i;

    for (i = 7; i >= 0; i--)
        menu->objects[i] = NULL;
    Menu_CreateOwnerEntryObjects(0);
    for (i = 0; i <= 3; i++) {
        if (menu->objects[i] != NULL) {
            menu->x[i] = (i * 3 + (window->x + origin_x)) * 8;
            menu->y[i] = (window->y + origin_y) * 8 + 16;
            menu->scale[i] = 0x10000;
        }
    }
    menu->active = 1;
    Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0x480);
}
