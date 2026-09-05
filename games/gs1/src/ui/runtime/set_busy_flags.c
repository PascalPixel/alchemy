#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

#define UiWork_SetBusyFlags Func_08017620

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void UiWork_SetBusyFlags(s32 flags) {
    void *work;

    work = *(void **)ADDR_03001E8C;
    if (work != NULL) {
        if (flags & 1) {
            FIELD_AT_OFFSET(work, s8 *, RENDER_BUSY_OFS + 1) = 1;
        }
        if (2 & flags) {
            FIELD_AT_OFFSET(work, s8 *, RENDER_BUSY_OFS + 2) = 1;
        }
    }
}
