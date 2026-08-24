#include "types.h"
#include "global_cells.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 ScheduleCallback(s32);
void UiWork_Finalize(struct Work *work, s32 release);
void UiTimedNotice_Tick(void);

void UiTimedNotice_CloseIfActive(void) {
    void *temp_r0;

    temp_r0 = M2C_FIELD(*(void **)ADDR_03001EBC, void **, 0x230);
    if ((temp_r0 != NULL) && (M2C_FIELD(temp_r0, u16 *, 0x16) != 0)) {
        UiWork_Finalize(temp_r0, 2);
        ScheduleCallback((s32)UiTimedNotice_Tick);
    }
}
