#include "types.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"

struct MenuEntryWork {
    u8 unknown_000[0xf8];
    void *objects[8];
    u8 unknown_118[0x50a - 0x118];
    u8 hidden;
};

void ResourceObject_ReleaseFar(void *object);
void Menu_UpdateEntryObjectTransforms(void);

void Menu_ReleaseEntryObjects(void)
{
    struct MenuEntryWork *menu = Ram_HeapSlots->menu_runtime;
    void **objects = menu->objects;
    s32 i;

    for (i = 7; i >= 0; i--) {
        void *object = *objects++;

        if (object != NULL)
            ResourceObject_ReleaseFar(object);
    }
    Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
}

void Menu_SetEntryObjectsHidden(s32 hidden)
{
    struct MenuEntryWork *menu = Ram_HeapSlots->menu_runtime;

    /* FAKEMATCH: ⚓️ loads the menu work before it builds the field offset */
    __asm__ volatile("" : "+l"(menu));
    menu->hidden = hidden;
}
