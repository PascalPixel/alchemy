#include "types.h"
#include "scene.h"
#include "abi/battle/effects/runtime/misc/clear_active_slots_and_schedule_updates.h"
#include "global_cells.h"

void ScheduleCallback(void *);

void WaitFrames(s32);
extern u8 gRom;

void BattleFx_ClearActiveSlotsAndScheduleUpdates(void)
{
    u8 *slot = *(u8 **)ADDR_03001F30;
    u8 *active_flag;
    s32 slot_index;

    ScheduleCallback(&gRom);
    active_flag = slot + 157;
    slot += 88;
    for (slot_index = 23; slot_index >= 0; slot_index--) {
        u32 is_active = *(volatile u8 *)active_flag;
        active_flag += 72;
        if ((is_active << 24) != 0) {
            Battle_Do(slot);
        }
        slot += 72;
    }
    Battle_Do2(56);
    WaitFrames(1);
}

void BattleFx_AdvanceSpinAngle(void *object)
{
    *(u16 *)((u8 *)object + 6) += 0x2000;
}
