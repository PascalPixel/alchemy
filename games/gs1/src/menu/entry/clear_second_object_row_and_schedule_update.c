#include "types.h"
#include "scene.h"
#include "abi/menu/entry/clear_second_object_row_and_schedule_update.h"

extern u8 *gIw;
void ScheduleCallback(s32);

void Menu_ClearSecondObjectRowAndScheduleUpdate(void)
{
    u8 *base = gIw;
    s32 offset = 137;
    s32 zero;
    s32 count;

    ScheduleCallback((s32)Menu_Run);
    zero = 0;
    offset *= 4;
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
