#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
extern u8 Data_03001e8c[];

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void UiWork_SetAltFlagAndClearTable(s32 flag)
{
    s32 i;
    s32 j;
    s8 *p;
    s8 *q;
    void *work;

    work = *(void **)((u32)&Data_03001e8c);
    if (flag != 0) {
        FIELD_AT_OFFSET(work, s8 *, RENDER_ALT_OFS) = 1;
        flag = 0;
        for (i = 0x80, p = work + 0xE20; i <= 0xFF; i += 1) {
            *p = flag;
            p += 1;
        }
        return;
    }
    FIELD_AT_OFFSET(work, s8 *, RENDER_ALT_OFS) = 0;
    flag = 0;
    q = work + 0xE20;
    j = 0x7F;
    do {
        j -= 1;
        *q = flag;
        q += 1;
    } while (j >= 0);
}
