#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

#define UiWork_SetTwoEntriesTo999 Func_08019d0c

#if defined(GS1_EDITION_JA)
#define WORK_NO 0x8BE
#else
#define WORK_NO 0x976
#endif

/* 連続する2要素へ0x3e7を設定する。 */
void UiWork_SetTwoEntriesTo999(void)
{
    s16 *work = (s16 *)*(void **)ADDR_03001E8C;
    s32 no = WORK_NO;

    do {
        work[no] = 0x3e7;
        no++;
    } while (no != WORK_NO + 2);
}
