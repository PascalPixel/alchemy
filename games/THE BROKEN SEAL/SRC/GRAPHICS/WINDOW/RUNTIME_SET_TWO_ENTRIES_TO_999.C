#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
extern u8 Data_03001e8c[];

#if defined(TBS_EDITION_JA)
#define WORK_NO 0x8BE
#else
#define WORK_NO 0x976
#endif

/* 連続する2要素へ0x3e7を設定する。 */
void UiWork_SetTwoEntriesTo999(void)
{
    s16 *work = (s16 *)*(void **)((u32)&Data_03001e8c);
    s32 no = WORK_NO;

    do {
        work[no] = 0x3e7;
        no++;
    } while (no != WORK_NO + 2);
}
