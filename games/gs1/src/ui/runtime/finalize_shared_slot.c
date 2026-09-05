#include "types.h"
#include "global_cells.h"

#define UiWork_FinalizeSharedSlot Func_080174d8

void UiWork_Finalize(struct Work *work, s32 release);

void UiWork_FinalizeSharedSlot(void) {
    struct Work **slot;
    struct Work *work;

    slot = *(struct Work ***)ADDR_03001EE4;
    work = *slot;
    if (work != 0) {
        UiWork_Finalize(work, 1);
        *slot = 0;
    }
}
