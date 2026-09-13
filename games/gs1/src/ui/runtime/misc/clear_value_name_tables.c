#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

void UiWork_ClearValueNameTables(void)
{
    s32 no;
    u32 *wtbl;
    u16 *htbl;
    volatile u32 *wp;
    volatile u16 *hp;

    wtbl = (u32 *)((u8 *)*(u32 *)ADDR_03001E8C + RENDER_VALUE_TBL_OFS);
    htbl = (u16 *)((u8 *)*(u32 *)ADDR_03001E8C + RENDER_NAME_TBL_OFS);
    no = 0;

    /* 対応する値と識別子は同じ順序で消去する。 */
    do {
        wp = wtbl + no;
        hp = htbl + no;
        *wp = 0;
        *hp = 0;
        no++;
    } while (no != 8);
}
