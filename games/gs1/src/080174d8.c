#include "types.h"
#include "global_cells.h"

void UiWork_Finalize(struct Work *work, s32 release);

void Func_080174d8(void) {
    struct Work **slot;
    struct Work *work;

    slot = *(struct Work ***)ADDR_03001EE4;
    work = *slot;
    if (work != 0) {
        UiWork_Finalize(work, 1);
        *slot = 0;
    }
}
