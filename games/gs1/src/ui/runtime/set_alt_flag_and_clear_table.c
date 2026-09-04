#include "types.h"
#include "global_cells.h"

#define UiWork_SetAltFlagAndClearTable Func_0801e3c8

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void UiWork_SetAltFlagAndClearTable(s32 flag) {
    s32 i;
    s32 j;
    s8 *p;
    s8 *q;
    void *work;

    work = *(void **)ADDR_03001E8C;
    if (flag != 0) {
        FIELD_AT_OFFSET(work, s8 *, 0xEA2) = 1;
        flag = 0;
        for (i = 0x80, p = work + 0xE20; i <= 0xFF; i += 1) {
            *p = flag;
            p += 1;
        }
        return;
    }
    FIELD_AT_OFFSET(work, s8 *, 0xEA2) = 0;
    flag = 0;
    q = work + 0xE20;
    j = 0x7F;
    do {
        j -= 1;
        *q = flag;
        q += 1;
    } while (j >= 0);
}
