#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "inn.h"

/* ui/ability_menu/build_available_list.c */
s32 Ability_GetMaximum(s32, s32);

extern void *gIw;

s32 AbilityMenu_BuildAvailableList(void)
{
    u8 *state;
    s16 *output;
    s32 index;
    s32 count;
    s32 offset;
    s8 mode;

    state = gIw;
    count = 0;
    index = 0;
    output = (s16 *)(state + 0x26c);
    do {
        mode = *(s8 *)(state + 0x3a9);
        if (mode == Ui_Check(index)&&
            Ability_GetMaximum(index, 0) != 0) {
            *output = index;
            count++;
            output++;
        }
        index++;
    } while (index <= 0x1ff);
    offset = count << 1;
    offset += 0x26c;
    *(u16 *)(state + offset) = 0;
    *(u8 *)(state + 0x3a6) = count;
    return count;
}

/* shop/cursor/step.c */
void Shop_StepCursor(void)
{
    ShopCursor_Advance(*(s32 *)ADDR_03001F2C + 0x380);
}

/* inn/cleanup.c */
s32 Resource_ResetEntry(u16);
s32 ScheduleCallback(s32);
s32 UiWork_FinalizePending();
extern u8 gRom;

void Inn_Cleanup(void)
{
    struct InnState *state;

    state = gIw;
    ScheduleCallback((s32)&gRom);
    UiWork_FinalizePending();
    Resource_ResetEntry(state->resource_entries[0]);
    Resource_ResetEntry(state->resource_entries[1]);
    Resource_ResetEntry(state->resource_entries[2]);
    Resource_ResetEntry(state->resource_entries[3]);
    Resource_ResetEntry(state->resource_entries[4]);
    Resource_ResetEntry(state->resource_entries[5]);
    Sys_CheckCleanup(0x37);
}
