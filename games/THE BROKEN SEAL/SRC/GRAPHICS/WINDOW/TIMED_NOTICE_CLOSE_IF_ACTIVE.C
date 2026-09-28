#include "TYPES.H"
#include "GLOBAL_CELLS.H"
extern u8 Data_03001ebc[];

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Scheduler_RemoveCallback(s32);
void UiWork_Finalize(struct Work *work, s32 release);
void UiTimedNotice_Tick(void);

void UiTimedNotice_CloseIfActive(void)
{
    void *work;

    work = FIELD_AT_OFFSET(*(void **)((u32)&Data_03001ebc), void **, 0x230);
    if ((work != NULL) && (FIELD_AT_OFFSET(work, u16 *, 0x16) != 0)) {
        UiWork_Finalize(work, 2);
        Scheduler_RemoveCallback((s32)UiTimedNotice_Tick);
    }
}
