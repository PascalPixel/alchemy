#include "TYPES.H"
#include "WINDOW.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"


void UiWork_SetAltFlagAndClearTable(s32 flag)
{
    s32 i;
    s32 j;
    s8 *p;
    s8 *q;
    u8 *work;

    work = gWindowWork[0];
    if (flag != 0) {
        ((struct UiRenderWork *)work)->alt = 1;
        flag = 0;
        for (i = 0x80, p = (s8 *)((struct UiRenderWork *)work)->tile_attributes + 0x80; i <= 0xFF; i += 1) {
            *p = flag;
            p += 1;
        }
        return;
    }
    ((struct UiRenderWork *)work)->alt = 0;
    flag = 0;
    q = (s8 *)((struct UiRenderWork *)work)->tile_attributes + 0x80;
    j = 0x7F;
    do {
        j -= 1;
        *q = flag;
        q += 1;
    } while (j >= 0);
}

void UiRender_ReservedNoOp(void)
{
}
