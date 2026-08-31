#include "colosso_log_rolling_stage.h"


u8 *Func_020077bc();           /* veneer, item/party record by id, established */
s32 Func_020077ce();           /* veneer, established (handle, item) */
void Func_020077ea();          /* veneer, established (handle, slot) */

void ColossoLogRollingStage_ApplyItemToMatchingSlots(s32 handle, s32 item)
{
    u8 *record;
    s32 slot;

    record = Func_020077bc(handle);
    Func_020077ce(handle, item);

    for (slot = 0; slot <= 14; slot++) {
        if (*(u16 *)(record + 216 + slot * 2) == item) {
            Func_020077ea(handle, slot);
        }
    }
}
