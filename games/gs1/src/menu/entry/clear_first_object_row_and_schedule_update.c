#include "types.h"
#include "scene.h"

/* menu/entry/clear_first_object_row_and_schedule_update.c */
void ScheduleCallback(s32);

void Menu_ClearFirstObjectRowAndScheduleUpdate(void)
{
    u8 *base = gIw;
    s32 offset = 138;
    s32 zero;
    s32 count;

    ScheduleCallback((s32)FunctionHead_0801ff58);
    zero = 0;
    offset *= 2;
    count = 3;
    do {
        void *entry = *(void **)(offset + (unsigned int)base);

        if (entry != 0) {
            Menu_Do(entry);
            *(s32 *)(offset + (unsigned int)base) = zero;
        }
        count--;
        offset += 4;
    } while (count >= 0);
}
