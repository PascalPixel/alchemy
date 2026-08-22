#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

void Func_080198dc(void)
{
    s32 no;
    u32 *word_base;
    u16 *half_base;
    volatile u32 *word_ptr;
    volatile u16 *half_ptr;

    word_base = (u32 *)((u8 *)*(u32 *)ADDR_03001E8C + RENDER_VALUE_TBL_OFS);
    half_base = (u16 *)((u8 *)*(u32 *)ADDR_03001E8C + RENDER_NAME_TBL_OFS);
    no = 0;

    /* 対応する値と識別子は同じ順序で消去する。 */
    do {
        word_ptr = word_base + no;
        half_ptr = half_base + no;
        *word_ptr = 0;
        *half_ptr = 0;
        no++;
    } while (no != 8);
}
