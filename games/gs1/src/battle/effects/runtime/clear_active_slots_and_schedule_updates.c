#include "types.h"
#include "global_cells.h"

void ScheduleCallback(void *);
void Func_0809bb34(void *);
void Func_08002dd8(s32);
void WaitFrames(s32);
extern u8 Data_08095885;

void BattleEffect_ClearActiveSlotsAndScheduleUpdates(void) {
    u8 *slot = *(u8 **)ADDR_03001F30;
    u8 *active_flag;
    s32 slot_index;

    ScheduleCallback(&Data_08095885);
    active_flag = slot + 157;
    slot += 88;
    for (slot_index = 23; slot_index >= 0; slot_index--) {
        u32 is_active = *(volatile u8 *)active_flag;
        active_flag += 72;
        if ((is_active << 24) != 0) {
            Func_0809bb34(slot);
        }
        slot += 72;
    }
    Func_08002dd8(56);
    WaitFrames(1);
}

void BattleEffect_AdvanceSpinAngle(void *object) {
    *(u16 *)((u8 *)object + 6) += 0x2000;
}
