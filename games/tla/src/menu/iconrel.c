#include "types.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"

struct MenuIconWork {
    u8 unknown_000[0xc08];
    u16 resources[5];
};

void MenuIcon_PushSlots(void);
s32 Resource_ResetEntry(u32 resource);

void MenuIcon_ReleaseSlots(void)
{
    struct MenuIconWork *menu = Ram_HeapSlots->menu_runtime;
    u16 *resource;
    s32 i;

    Scheduler_RemoveCallback(MenuIcon_PushSlots);
    resource = menu->resources;
    for (i = 4; i >= 0; i--)
        Resource_ResetEntry(*resource++);
}
