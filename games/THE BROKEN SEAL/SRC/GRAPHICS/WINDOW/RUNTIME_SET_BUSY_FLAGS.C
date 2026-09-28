#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
extern u8 Data_03001e8c[];

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void UiWork_SetBusyFlags(s32 flags)
{
    void *work;

    work = *(void **)((u32)&Data_03001e8c);
    if (work != NULL) {
        if (flags & 1) {
            FIELD_AT_OFFSET(work, s8 *, RENDER_BUSY_OFS + 1) = 1;
        }
        if (2 & flags) {
            FIELD_AT_OFFSET(work, s8 *, RENDER_BUSY_OFS + 2) = 1;
        }
    }
}
