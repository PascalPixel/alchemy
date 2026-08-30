#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 ScheduleCallback(s32);
void UiWork_Finalize(struct Work *work, s32 release);
void UiTimedNotice_Tick(void);

void UiTimedNotice_CloseIfActive(void) {
    void *notice_work;

    notice_work = FIELD_AT_OFFSET(*(void **)ADDR_03001EBC, void **, 0x230);
    if ((notice_work != NULL) && (FIELD_AT_OFFSET(notice_work, u16 *, 0x16) != 0)) {
        UiWork_Finalize(notice_work, 2);
        ScheduleCallback((s32)UiTimedNotice_Tick);
    }
}
