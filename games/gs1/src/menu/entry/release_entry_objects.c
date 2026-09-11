#include "types.h"
#include "scene.h"
#include "abi/menu/entry/release_entry_objects.h"

extern u8 *gIw;

void ScheduleCallback(void (*callback)(void));
extern u8 gRom;

void Menu_ReleaseEntryObjects(void)
{
    u32 buf[7];
    u8 *base = gIw;
    s32 count;
    void **p;
    s32 i;

    count = (u16)Menu_Check(buf);
    if (count != 0) {
        p = (void **)(base + 276);
        i = count;
        do {
            void *entry = *p++;

            if (entry != 0) {
                Menu_Do(entry);
            }
        } while (--i != 0);
    }
    ScheduleCallback((void (*)(void))&gRom);
}
