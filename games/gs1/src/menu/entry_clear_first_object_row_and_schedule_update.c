#include "types.h"
#include "scene.h"
#include "inn.h"

/* menu/entry/clear_first_object_row_and_schedule_update.c */
void ScheduleCallback(s32);
void FunctionHead_0801ff58(void);
void Menu_Do(void *);

void Menu_ClearFirstObjectRowAndScheduleUpdate(void)
{
    u8 *base = (u8 *)gIw;
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

/* menu/update_first_object_row_positions.c */
s32 FunctionHead_08009008(s32, void *, void *, s32);

void Menu_UpdateFirstObjectRowPositions(void)
{
    u8 *base = (u8 *)gIw;
    s16 *offsets = (s16 *)(base + 0x134);
    s32 *entries = (s32 *)(base + 0x114);
    s32 source[2];
    s32 request[4];
    s32 index = 0;
    s32 handle;

    while (1) {
        handle = entries[index];
        if (handle != 0) {
            source[0] = entries[index + 0x10];
            source[1] = entries[index + 0x10];
            request[0] = offsets[index] << 16;
            request[1] = 0x01F40000;
            request[2] = (offsets[index + 8] << 16) + 0x01F40000;
            request[3] = 0;
            FunctionHead_08009008(handle, request, source, 0x4000);
        }
        index += 1;
        if (index > 3) {
            break;
        }
    }
}
