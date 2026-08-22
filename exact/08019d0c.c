#include "types.h"
#include "global_cells.h"

/* 連続する2要素へ0x3e7を設定する。 */
void Func_08019d0c(void)
{
    s16 *work = (s16 *)*(void **)ADDR_03001E8C;
    s32 no = 0x976;

    do {
        work[no] = 0x3e7;
        no++;
    } while (no != 0x978);
}
